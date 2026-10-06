#!/usr/bin/env python3
"""speciesdb.py - build a JSON species database from pokeemerald-expansion.

The species data of the expansion is full of config-dependent macros
(#if P_FAMILY_*, P_UPDATED_*, GEN_LATEST comparisons, MON_TYPES(), EVOLUTION(),
CONDITIONS(), PERCENT_FEMALE(), ...).  Instead of re-implementing them, this
tool runs the real C preprocessor on src/pokemon.c with the same flags the
Makefile uses (so the configs in include/config/*.h are honoured exactly),
then parses the expanded designated initializers of gSpeciesInfo, the level-up
learnsets, egg moves, form species tables and form change tables.

Usage:
    python3 -I speciesdb.py [--root DIR] [--out species.json]
                            [--families families.json] [--keep-preprocessed F]

See README.md for the meaning of every field and the 'encounterable' rules.
"""

import argparse
import collections
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True  # keep the tool directory free of __pycache__
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from cparse import CSource, Unevaluable  # noqa: E402

DEFAULT_ROOT = '/home/user/pex-orig'
DEFAULT_OUT = '/home/user/work/species.json'
DEFAULT_FAMILIES = '/home/user/work/families.json'
SCHEMA_VERSION = 1

# --------------------------------------------------------------------------
# Static knowledge (documented in README.md)
# --------------------------------------------------------------------------

GEN_RANGES = [(1, 151, 1), (152, 251, 2), (252, 386, 3), (387, 493, 4), (494, 649, 5),
              (650, 721, 6), (722, 809, 7), (810, 905, 8), (906, 1025, 9)]

# Official "baby" Pokémon (Gen 2-8).  Cross-checked at runtime against a
# heuristic (Undiscovered egg group + family root + evolves + not legendary).
BABY_SPECIES = [
    'SPECIES_PICHU', 'SPECIES_CLEFFA', 'SPECIES_IGGLYBUFF', 'SPECIES_TOGEPI', 'SPECIES_TYROGUE',
    'SPECIES_SMOOCHUM', 'SPECIES_ELEKID', 'SPECIES_MAGBY', 'SPECIES_AZURILL', 'SPECIES_WYNAUT',
    'SPECIES_BUDEW', 'SPECIES_CHINGLING', 'SPECIES_BONSLY', 'SPECIES_MIME_JR', 'SPECIES_HAPPINY',
    'SPECIES_MUNCHLAX', 'SPECIES_RIOLU', 'SPECIES_MANTYKE', 'SPECIES_TOXEL',
]

# Species that match the baby heuristic but are not babies.
NOT_BABY = {'SPECIES_GIMMIGHOUL_CHEST', 'SPECIES_GIMMIGHOUL_ROAMING'}

# Basic stage of the 27 starter families (3 per generation).
STARTER_ROOTS = [
    'SPECIES_BULBASAUR', 'SPECIES_CHARMANDER', 'SPECIES_SQUIRTLE',
    'SPECIES_CHIKORITA', 'SPECIES_CYNDAQUIL', 'SPECIES_TOTODILE',
    'SPECIES_TREECKO', 'SPECIES_TORCHIC', 'SPECIES_MUDKIP',
    'SPECIES_TURTWIG', 'SPECIES_CHIMCHAR', 'SPECIES_PIPLUP',
    'SPECIES_SNIVY', 'SPECIES_TEPIG', 'SPECIES_OSHAWOTT',
    'SPECIES_CHESPIN', 'SPECIES_FENNEKIN', 'SPECIES_FROAKIE',
    'SPECIES_ROWLET', 'SPECIES_LITTEN', 'SPECIES_POPPLIO',
    'SPECIES_GROOKEY', 'SPECIES_SCORBUNNY', 'SPECIES_SOBBLE',
    'SPECIES_SPRIGATITO', 'SPECIES_FUECOCO', 'SPECIES_QUAXLY',
]

# Forms that pass every automatic rule but are not normally obtainable / are
# special-purpose duplicates.  constant -> reason
EXPLICIT_EXCLUDE = {
    'SPECIES_PIKACHU_STARTER': 'Partner Pikachu (Let\'s Go starter, special stats)',
    'SPECIES_EEVEE_STARTER': 'Partner Eevee (Let\'s Go starter, special stats)',
    'SPECIES_FLOETTE_ETERNAL': 'Eternal Flower Floette (unreleased/event-only)',
    'SPECIES_ETERNATUS_ETERNAMAX': 'Eternamax Eternatus (story battle only)',
    'SPECIES_GIMMIGHOUL_ROAMING': 'Roaming Gimmighoul (Pokemon GO only, not catchable in core games)',
    'SPECIES_GRENINJA_BATTLE_BOND': 'Battle Bond Greninja (event-only duplicate of Greninja)',
    'SPECIES_ZYGARDE_10_POWER_CONSTRUCT': 'Power Construct Zygarde (duplicate of Zygarde 10% with another ability)',
    'SPECIES_ZYGARDE_50_POWER_CONSTRUCT': 'Power Construct Zygarde (duplicate of Zygarde 50% with another ability)',
    'SPECIES_PICHU_SPIKY_EARED': 'Spiky-eared Pichu (event-only, cannot evolve)',
}
# Name patterns for event-only cosmetic forms (in case their data ever stops
# being identical to the base form, they still must not be encounterable).
EXPLICIT_EXCLUDE_PATTERNS = [
    (re.compile(r'^SPECIES_PIKACHU_(COSPLAY|ROCK_STAR|BELLE|POP_STAR|PH_?D|LIBRE|ORIGINAL|HOENN|SINNOH|UNOVA|KALOS|ALOLA|PARTNER|WORLD)(_CAP)?$'),
     'Cap/Cosplay Pikachu (event-only cosmetic)'),
    (re.compile(r'^SPECIES_MINIOR_CORE_'),
     'Minior Core form (Shields Down state; wild/trainer Minior use the Meteor form)'),
]

# Forms that are reached through a held-item form change.  Encounter
# generators must give them the item, so they are not 'encounterable' but the
# required item is reported in 'requiredItem'.
HELD_ITEM_FORM_METHODS = {'FORM_CHANGE_ITEM_HOLD'}
# Form change methods that only happen inside a battle.
BATTLE_FORM_METHODS_PREFIX = 'FORM_CHANGE_BATTLE_'
BATTLE_FORM_METHODS_EXTRA = {'FORM_CHANGE_BEGIN_BATTLE'}
# Form change methods that just revert a battle form at the end of a battle.
REVERT_FORM_METHODS = {'FORM_CHANGE_FAINT', 'FORM_CHANGE_END_BATTLE'}
BATTLE_KINDS = {'mega', 'primal', 'ultraBurst', 'gigantamax', 'tera', 'battleOnly'}

REGION_FLAGS = [('isAlolanForm', 'alola', 7), ('isGalarianForm', 'galar', 8),
                ('isHisuianForm', 'hisui', 8), ('isPaldeanForm', 'paldea', 9)]

FLAG_FIELDS = [
    'isRestrictedLegendary', 'isSubLegendary', 'isMythical', 'isUltraBeast', 'isParadox',
    'isTotem', 'isMegaEvolution', 'isPrimalReversion', 'isUltraBurst', 'isGigantamax',
    'isTeraForm', 'isAlolanForm', 'isGalarianForm', 'isHisuianForm', 'isPaldeanForm',
    'cannotBeTraded', 'dexForceRequired', 'isFrontierBanned', 'isSkyBattleBanned',
    'isTelekinesisBanned',
]

STAT_FIELDS = [('hp', 'baseHP'), ('atk', 'baseAttack'), ('def', 'baseDefense'),
               ('spe', 'baseSpeed'), ('spa', 'baseSpAttack'), ('spd', 'baseSpDefense')]
EV_FIELDS = [('hp', 'evYield_HP'), ('atk', 'evYield_Attack'), ('def', 'evYield_Defense'),
             ('spe', 'evYield_Speed'), ('spa', 'evYield_SpAttack'), ('spd', 'evYield_SpDefense')]

MON_MALE, MON_FEMALE, MON_GENDERLESS = 0x00, 0xFE, 0xFF


def log(*a):
    print(*a, file=sys.stderr)


# --------------------------------------------------------------------------
# Preprocessing
# --------------------------------------------------------------------------

def find_cpp(user_cpp=None):
    if user_cpp:
        return user_cpp.split()
    for cand in (['arm-none-eabi-cpp'], ['cpp'], ['gcc', '-E']):
        if shutil.which(cand[0]):
            return cand
    raise SystemExit('no C preprocessor found (need arm-none-eabi-cpp, cpp or gcc)')


def makefile_game_version(root):
    try:
        with open(os.path.join(root, 'Makefile'), encoding='utf-8', errors='replace') as f:
            for line in f:
                m = re.match(r'\s*GAME_VERSION\s*\??=\s*(\w+)', line)
                if m:
                    return m.group(1)
    except OSError:
        pass
    return 'EMERALD'


def run_cpp(root, cpp, src, defines=(), game_version=None, extra_args=()):
    """Run cpp on *src* with the Makefile's CPPFLAGS.  Generated headers that
    do not exist in a clean tree (map_groups.h, teachable_learnsets.h, ...) are
    replaced by empty stubs; their identifiers then stay symbolic."""
    game_version = game_version or makefile_game_version(root)
    stubs = []
    with tempfile.TemporaryDirectory(prefix='speciesdb_stub_') as stubdir:
        for _ in range(100):
            cmd = cpp + ['-iquote', os.path.join(root, 'include'), '-iquote', stubdir,
                         '-Wno-trigraphs', '-DMODERN=1', '-DTESTING=0', '-D' + game_version,
                         '-std=gnu17'] + ['-D' + d for d in defines] + list(extra_args) + [src]
            p = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            err = p.stderr.decode('utf-8', 'replace')
            if p.returncode == 0:
                return p.stdout.decode('utf-8', 'replace'), stubs, cmd, game_version
            m = re.search(r'fatal error: ([^\s:]+): No such file', err)
            if not m:
                raise SystemExit('preprocessor failed:\n' + err[-4000:])
            rel = m.group(1)
            if rel in stubs:
                raise SystemExit('stub for %s did not help:\n%s' % (rel, err[-2000:]))
            path = os.path.join(stubdir, rel)
            os.makedirs(os.path.dirname(path), exist_ok=True)
            open(path, 'w').close()
            stubs.append(rel)
    raise SystemExit('too many missing headers')


def preprocess(root, cpp, defines=(), game_version=None):
    """Preprocess src/pokemon.c (which includes species_info.h, the learnsets,
    form tables, ...) exactly like the Makefile does."""
    src = os.path.join(root, 'src', 'pokemon.c')
    if not os.path.isfile(src):
        raise SystemExit('not an expansion tree (missing %s)' % src)
    return run_cpp(root, cpp, src, defines, game_version)


def config_snapshot(root, cpp, defines=(), game_version=None, prefixes=('P_',)):
    """Evaluate every object-like P_* config macro (include/config/*.h)."""
    head = '#include "global.h"\n#include "config/pokemon.h"\n#include "config/species_enabled.h"\n'
    with tempfile.TemporaryDirectory(prefix='speciesdb_cfg_') as td:
        w = os.path.join(td, 'cfg.c')
        with open(w, 'w') as f:
            f.write(head)
        text, _, _, _ = run_cpp(root, cpp, w, defines, game_version, ['-dM'])
        names = []
        for line in text.splitlines():
            m = re.match(r'#define (\w+)(\(?)', line)
            if m and not m.group(2) and m.group(1).startswith(prefixes):
                names.append(m.group(1))
        names = sorted(set(names))
        with open(w, 'w') as f:
            f.write(head + ''.join('const int cfg__%s[] = { %s };\n' % (n, n) for n in names))
        text, _, _, _ = run_cpp(root, cpp, w, defines, game_version)
    src = CSource(text)
    out = collections.OrderedDict()
    for n in names:
        lst = src.object('cfg__' + n)
        node = lst[0][1] if lst else None
        try:
            v = src.eval_int(node) if node is not None and node[0] != 'empty' else None
        except Unevaluable:
            v = src.text(*src.objects['cfg__' + n])[2:-2].strip()
        except SyntaxError:
            v = None
        out[n] = v
    return out


# --------------------------------------------------------------------------
# Extraction helpers
# --------------------------------------------------------------------------

class Extractor:
    def __init__(self, src):
        self.s = src
        self.anomalies = []
        self._array_cache = {}

    def anomaly(self, msg):
        self.anomalies.append(msg)

    def sym(self, node):
        """Identifier name if node is a bare identifier, else evaluated number."""
        if node is None:
            return None
        if node[0] == 'id':
            return node[1]
        if node[0] == 'empty':
            return None
        try:
            return self.s.eval_int(node)
        except Unevaluable as e:
            raise Unevaluable('%s in %r' % (e, node))

    def num(self, node, default=0):
        if node is None:
            return default
        return self.s.eval_int(node)

    def string(self, node):
        if node is None:
            return None
        if node[0] == 'call' and node[2]:
            node = node[2][0]
        if node[0] == 'str':
            return node[1]
        return None

    def init(self, node):
        """Brace list of an init / compound literal node, else None."""
        if node is None:
            return None
        if node[0] == 'init':
            return node[1]
        return None

    def array(self, name):
        """Top-level array initializer by name (cached)."""
        if name not in self._array_cache:
            self._array_cache[name] = self.s.object(name)
        return self._array_cache[name]

    def is_end(self, node, end_names, end_value):
        if node is None:
            return True
        if node[0] == 'id' and node[1] in end_names:
            return True
        try:
            v = self.s.eval_int(node)
        except Unevaluable:
            return False
        return v == end_value


def enum_value(src, name):
    return src.enums.get(name)


# --------------------------------------------------------------------------
# Main build
# --------------------------------------------------------------------------

def generation_of(dex):
    if not dex:
        return None
    for lo, hi, g in GEN_RANGES:
        if lo <= dex <= hi:
            return g
    return None


def build(root, cpp_cmd=None, defines=(), keep_preprocessed=None, game_version=None):
    cpp = find_cpp(cpp_cmd)
    text, stubs, cmd, game_version = preprocess(root, cpp, defines, game_version)
    if keep_preprocessed:
        with open(keep_preprocessed, 'w', encoding='utf-8') as f:
            f.write(text)
    src = CSource(text)
    ex = Extractor(src)
    if src.bad_chars:
        ex.anomaly('unexpected characters in preprocessed source: %r' % sorted(set(src.bad_chars))[:20])
    for name in src.enum_failures:
        if name.startswith(('SPECIES_', 'NATIONAL_DEX_', 'TYPE_', 'ABILITY_', 'MOVE_', 'EVO_', 'IF_')):
            ex.anomaly('could not evaluate enum value of %s' % name)

    enums = src.enums
    species_ids = {k: v for k, v in enums.items()
                   if k.startswith('SPECIES_') and src.enum_types.get(k) == 'Species'}
    id_to_names = collections.defaultdict(list)
    for k, v in species_ids.items():
        id_to_names[v].append(k)
    natdex_ids = {k: v for k, v in enums.items() if k.startswith('NATIONAL_DEX_')}

    table = src.object('gSpeciesInfo')
    if table is None:
        raise SystemExit('gSpeciesInfo not found in preprocessed source')

    raw = collections.OrderedDict()   # constant -> fields dict (nodes)
    for desig, val in table:
        if not desig or desig[0][0] != 'i':
            ex.anomaly('gSpeciesInfo entry without [SPECIES_X] designator')
            continue
        key = desig[0][1]
        if key[0] != 'id':
            ex.anomaly('non-identifier species designator %r' % (key,))
            continue
        name = key[1]
        lst = ex.init(val)
        if lst is None:
            ex.anomaly('species %s initializer is not a brace list' % name)
            continue
        dups = []
        fields = lst.fields(warn=dups.append)
        if name in raw:
            ex.anomaly('species %s initialized twice in gSpeciesInfo (later wins)' % name)
        raw[name] = (fields, dups)

    # canonical constant for each designator (designators may use aliases)
    species = collections.OrderedDict()
    duplicated_field_notes = collections.Counter()
    for name, (fields, dups) in raw.items():
        if name in ('SPECIES_NONE', 'SPECIES_EGG'):
            continue
        sid = species_ids.get(name)
        if sid is None:
            ex.anomaly('designator %s is not a Species enumerator' % name)
            continue
        for d in dups:
            duplicated_field_notes[d] += 1
        try:
            rec = extract_species(ex, name, sid, fields, natdex_ids)
        except (Unevaluable, SyntaxError, KeyError, TypeError, ValueError) as e:
            ex.anomaly('failed to extract %s: %s' % (name, e))
            continue
        if rec is None:
            continue
        aliases = sorted(n for n in id_to_names[sid] if n != name)
        rec['aliases'] = aliases
        species[name] = rec

    # ---------------- form tables / form changes ----------------
    form_change_cache = {}
    for name, rec in species.items():
        ft = rec.pop('_formSpeciesIdTable')
        rec['forms'] = []
        if ft:
            arr = ex.array(ft)
            if arr is None:
                ex.anomaly('%s: form table %s not found' % (name, ft))
            else:
                forms = []
                for _, node in arr:
                    if ex.is_end(node, ('FORM_SPECIES_END',), 0xFFFF):
                        break
                    forms.append(ex.sym(node))
                rec['forms'] = forms
        fct = rec.pop('_formChangeTable')
        rec['formChanges'] = []
        if fct:
            if fct not in form_change_cache:
                arr = ex.array(fct)
                changes = []
                if arr is None:
                    ex.anomaly('%s: form change table %s not found' % (name, fct))
                else:
                    for _, node in arr:
                        lst = ex.init(node)
                        if lst is None:
                            continue
                        pos = lst.positional()
                        if not pos or ex.is_end(pos[0], ('FORM_CHANGE_TERMINATOR',), 0):
                            break
                        ch = {'method': ex.sym(pos[0]),
                              'target': ex.sym(pos[1]) if len(pos) > 1 else None,
                              'params': [ex.sym(x) for x in pos[2:]]}
                        changes.append(ch)
                form_change_cache[fct] = changes
            rec['formChanges'] = form_change_cache[fct]

    # fusion tables (Kyurem, Necrozma, Calyrex)
    ptrs = src.object('gFusionTablePointers')
    fusion_tables = set()
    for desig, node in (ptrs or []):
        if node[0] == 'id':
            fusion_tables.add(node[1])
    for tname in sorted(fusion_tables):
        arr = ex.array(tname)
        if arr is None:
            ex.anomaly('fusion table %s not found' % tname)
            continue
        for _, node in arr:
            lst = ex.init(node)
            if lst is None:
                continue
            pos = lst.positional()
            if not pos or ex.is_end(pos[0], ('FUSION_TERMINATOR',), 0xFF):
                break
            item, a, b, into = (ex.sym(x) for x in pos[1:5])
            if into in species:
                species[into]['fusion'] = collections.OrderedDict(
                    [('item', item), ('components', [a, b])])
            else:
                ex.anomaly('fusion result %s undefined' % into)
    for rec in species.values():
        rec.setdefault('fusion', None)

    # learnsets
    for name, rec in species.items():
        lu = rec.pop('_levelUpLearnset')
        moves = []
        if lu:
            arr = ex.array(lu)
            if arr is None:
                ex.anomaly('%s: level up learnset %s not found' % (name, lu))
            else:
                for _, node in arr:
                    lst = ex.init(node)
                    if lst is None:
                        continue
                    f = lst.fields()
                    if f:
                        mv, lv = f.get('move'), f.get('level')
                    else:
                        pos = lst.positional()
                        mv, lv = pos[0], pos[1]
                    if ex.is_end(mv, ('LEVEL_UP_MOVE_END',), 0xFFFF):
                        break
                    moves.append([ex.num(lv), ex.sym(mv)])
        else:
            ex.anomaly('%s has no levelUpLearnset' % name)
        rec['levelUpLearnsetName'] = lu
        rec['levelUpLearnset'] = moves
        em = rec.pop('_eggMoveLearnset')
        eggs = []
        if em:
            arr = ex.array(em)
            if arr is None:
                ex.anomaly('%s: egg move learnset %s not found' % (name, em))
            else:
                for _, node in arr:
                    if ex.is_end(node, ('MOVE_UNAVAILABLE',), 0xFFFF):
                        break
                    eggs.append(ex.sym(node))
        rec['eggMoves'] = eggs

    derive(species, ex)

    meta = collections.OrderedDict()
    meta['schemaVersion'] = SCHEMA_VERSION
    meta['root'] = os.path.abspath(root)
    meta['gitCommit'] = git_commit(root)
    meta['preprocessor'] = ' '.join(cpp)
    meta['gameVersion'] = game_version
    meta['extraDefines'] = list(defines)
    meta['stubbedHeaders'] = stubs
    cfg = config_snapshot(root, cpp, defines, game_version)
    meta['configs'] = cfg
    fam = {k: v for k, v in cfg.items() if k.startswith('P_FAMILY_')}
    gens = {k: v for k, v in cfg.items() if re.match(r'P_GEN_\d_POKEMON$', k)}
    meta['configSummary'] = collections.OrderedDict([
        ('genFlags', gens),
        ('familyFlags', len(fam)),
        ('familiesEnabled', sum(1 for v in fam.values() if v)),
        ('familiesDisabled', sorted(k for k, v in fam.items() if not v)),
        ('formFlags', {k: cfg.get(k) for k in ('P_MEGA_EVOLUTIONS', 'P_GEN_9_MEGA_EVOLUTIONS', 'P_PRIMAL_REVERSIONS',
                                               'P_ULTRA_BURST_FORMS', 'P_GIGANTAMAX_FORMS', 'P_TERA_FORMS',
                                               'P_FUSION_FORMS', 'P_REGIONAL_FORMS', 'P_ALOLAN_FORMS',
                                               'P_GALARIAN_FORMS', 'P_HISUIAN_FORMS', 'P_PALDEAN_FORMS',
                                               'P_CROSS_GENERATION_EVOS', 'P_PIKACHU_EXTRA_FORMS',
                                               'P_COSPLAY_PIKACHU_FORMS', 'P_CAP_PIKACHU_FORMS')}),
        ('learnsets', cfg.get('P_LVL_UP_LEARNSETS')),
    ])
    for k, v in list(gens.items()) + list(fam.items()):
        if not v:
            ex.anomaly('config: %s is disabled' % k)
    meta['speciesEnumCount'] = len(species_ids)
    meta['speciesEnumDistinctIds'] = len(set(species_ids.values()))
    meta['duplicateFieldInitializers'] = dict(duplicated_field_notes)
    meta['anomalies'] = ex.anomalies
    return meta, species, src


def git_commit(root):
    try:
        p = subprocess.run(['git', '-C', root, 'rev-parse', 'HEAD'], stdout=subprocess.PIPE,
                           stderr=subprocess.DEVNULL)
        return p.stdout.decode().strip() or None
    except OSError:
        return None


def extract_species(ex, name, sid, f, natdex_ids):
    s = ex.s
    natdex_sym = ex.sym(f.get('natDexNum')) if 'natDexNum' in f else None
    if isinstance(natdex_sym, str):
        natdex = natdex_ids.get(natdex_sym)
        if natdex is None:
            ex.anomaly('%s: unknown natDexNum %s' % (name, natdex_sym))
    else:
        natdex = natdex_sym
    rec = collections.OrderedDict()
    rec['constant'] = name
    rec['id'] = sid
    rec['speciesName'] = ex.string(f.get('speciesName'))
    rec['natDexNum'] = natdex
    rec['natDexConstant'] = natdex_sym if isinstance(natdex_sym, str) else None
    rec['generation'] = generation_of(natdex)
    types = []
    tl = ex.init(f.get('types'))
    if tl is not None:
        for _, n in tl:
            t = ex.sym(n)
            if t not in types:
                types.append(t)
    rec['types'] = types
    stats = collections.OrderedDict((k, ex.num(f.get(fld))) for k, fld in STAT_FIELDS)
    rec['baseStats'] = stats
    rec['bst'] = sum(stats.values())
    abil = []
    al = ex.init(f.get('abilities'))
    if al is not None:
        abil = [ex.sym(n) for _, n in al]
    while len(abil) < 3:
        abil.append('ABILITY_NONE')
    rec['abilities'] = abil
    rec['catchRate'] = ex.num(f.get('catchRate'))
    rec['expYield'] = ex.num(f.get('expYield'))
    rec['evYield'] = collections.OrderedDict((k, ex.num(f.get(fld))) for k, fld in EV_FIELDS)
    gr = ex.num(f.get('genderRatio')) & 0xFF
    rec['genderRatio'] = gr
    if gr == MON_GENDERLESS:
        rec['gender'] = 'genderless'
        rec['femalePercent'] = None
    elif gr == MON_FEMALE:
        rec['gender'] = 'female'
        rec['femalePercent'] = 100.0
    elif gr == MON_MALE:
        rec['gender'] = 'male'
        rec['femalePercent'] = 0.0
    else:
        rec['gender'] = 'mixed'
        rec['femalePercent'] = round(gr * 100.0 / 255.0, 1)
    rec['eggCycles'] = ex.num(f.get('eggCycles'))
    rec['friendship'] = ex.num(f.get('friendship'))
    rec['growthRate'] = ex.sym(f.get('growthRate'))
    eg = []
    el = ex.init(f.get('eggGroups'))
    if el is not None:
        for _, n in el:
            g = ex.sym(n)
            if g not in eg:
                eg.append(g)
    rec['eggGroups'] = eg
    rec['itemCommon'] = ex.sym(f.get('itemCommon')) if 'itemCommon' in f else None
    rec['itemRare'] = ex.sym(f.get('itemRare')) if 'itemRare' in f else None
    rec['height'] = ex.num(f.get('height'))
    rec['weight'] = ex.num(f.get('weight'))
    rec['categoryName'] = ex.string(f.get('categoryName'))
    rec['bodyColor'] = ex.sym(f.get('bodyColor')) if 'bodyColor' in f else None
    flags = collections.OrderedDict()
    for fl in FLAG_FIELDS:
        flags[fl] = bool(ex.num(f.get(fl), 0))
    rec['flags'] = flags
    rec['perfectIVCount'] = ex.num(f.get('perfectIVCount'), 0)
    rec['forceTeraType'] = ex.sym(f.get('forceTeraType')) if 'forceTeraType' in f else None
    # evolutions
    evos = []
    evl = ex.init(f.get('evolutions'))
    if evl is not None:
        for _, node in evl:
            lst = ex.init(node)
            if lst is None:
                ex.anomaly('%s: evolution entry is not a brace list' % name)
                continue
            pos = lst.positional()
            if not pos or ex.is_end(pos[0], ('EVOLUTIONS_END',), 0xFFFF):
                break
            evo = collections.OrderedDict()
            evo['method'] = ex.sym(pos[0])
            evo['param'] = ex.sym(pos[1]) if len(pos) > 1 else 0
            evo['target'] = ex.sym(pos[2]) if len(pos) > 2 else None
            conds = []
            if len(pos) > 3:
                cl = ex.init(pos[3])
                if cl is None and pos[3][0] != 'empty':
                    ex.anomaly('%s: evolution conditions not parsed: %r' % (name, pos[3][:1]))
                for _, cn in (cl or []):
                    cpos = ex.init(cn)
                    if cpos is None:
                        continue
                    cpos = cpos.positional()
                    if not cpos or ex.is_end(cpos[0], ('CONDITIONS_END',), -1):
                        break
                    conds.append(collections.OrderedDict(
                        [('condition', ex.sym(cpos[0])), ('args', [ex.sym(x) for x in cpos[1:]])]))
            for cnd in conds:
                if cnd['condition'] == 'IF_GENDER' and cnd['args'] and isinstance(cnd['args'][0], int):
                    cnd['args'][0] = {MON_MALE: 'MON_MALE', MON_FEMALE: 'MON_FEMALE'}.get(cnd['args'][0], cnd['args'][0])
            evo['conditions'] = conds
            evos.append(evo)
    rec['evolutions'] = evos
    rec['_levelUpLearnset'] = ex.sym(f['levelUpLearnset']) if 'levelUpLearnset' in f else None
    rec['_eggMoveLearnset'] = ex.sym(f['eggMoveLearnset']) if 'eggMoveLearnset' in f else None
    rec['teachableLearnsetName'] = ex.sym(f['teachableLearnset']) if 'teachableLearnset' in f else None
    rec['_formSpeciesIdTable'] = ex.sym(f['formSpeciesIdTable']) if 'formSpeciesIdTable' in f else None
    rec['_formChangeTable'] = ex.sym(f['formChangeTable']) if 'formChangeTable' in f else None
    if rec['speciesName'] is None:
        ex.anomaly('%s has no speciesName' % name)
    if natdex is None:
        ex.anomaly('%s has no natDexNum' % name)
    return rec


# --------------------------------------------------------------------------
# Derived data
# --------------------------------------------------------------------------

def evo_category(evo):
    m = evo['method']
    conds = [c['condition'] for c in evo['conditions']]
    if m == 'EVO_NONE':
        return 'breed'
    if m in ('EVO_LEVEL', 'EVO_LEVEL_BATTLE_ONLY'):
        if isinstance(evo['param'], int) and evo['param'] > 0:
            return 'level'
        if 'IF_MIN_FRIENDSHIP' in conds:
            return 'friendship'
        if 'IF_HOLD_ITEM' in conds:
            return 'item'
        return 'other'
    if m == 'EVO_TRADE':
        return 'trade'
    if m == 'EVO_ITEM':
        return 'item'
    return 'other'


def region_of(rec):
    for fl, reg, _ in REGION_FLAGS:
        if rec['flags'][fl]:
            return reg
    return None


def battle_signature(rec):
    return (tuple(rec['types']), tuple(rec['baseStats'].values()), tuple(rec['abilities']))


def derive(species, ex):
    S = species
    # ---- evolution links ----
    parents = collections.defaultdict(list)        # target -> [(parent, evo)]
    breed_parents = collections.defaultdict(list)  # EVO_NONE links
    for name, rec in S.items():
        rec['canEvolve'] = False
        for evo in rec['evolutions']:
            evo['category'] = evo_category(evo)
            tgt = evo['target']
            if tgt not in S:
                ex.anomaly('%s evolves into undefined species %s' % (name, tgt))
                evo['targetDefined'] = False
                continue
            evo['targetDefined'] = True
            if evo['category'] == 'breed':
                breed_parents[tgt].append((name, evo))
            else:
                parents[tgt].append((name, evo))
                rec['canEvolve'] = True

    # ---- base forms ----
    for name, rec in S.items():
        forms = rec['forms']
        rec['region'] = region_of(rec)
        rec['formIntroGen'] = None
        for fl, reg, g in REGION_FLAGS:
            if rec['flags'][fl]:
                rec['formIntroGen'] = g
        if forms and name not in forms:
            ex.anomaly('%s is not listed in its own form table %s' % (name, forms))
        base = forms[0] if forms else name
        if base not in S:
            ex.anomaly('%s: base form %s undefined' % (name, base))
            base = name
        rec['baseForm'] = base
        rec['formOf'] = base if base != name else None
        rec['formIndex'] = forms.index(name) if name in forms else 0

    # ---- incoming form changes ----
    incoming = collections.defaultdict(list)  # target -> [(source, change)]
    seen_tables = set()
    for name, rec in S.items():
        key = id(rec['formChanges'])
        for ch in rec['formChanges']:
            tgt = ch['target']
            if tgt is None or tgt == name:
                continue
            incoming[tgt].append((name, ch))
        seen_tables.add(key)
    for name, rec in S.items():
        # de-duplicate (tables are shared between forms)
        uniq = {}
        for src_name, ch in incoming.get(name, []):
            k = (src_name, ch['method'], tuple(str(p) for p in ch['params']))
            uniq[k] = ch
        rec['formChangesInto'] = sorted({(k[0], k[1]) for k in uniq})
        rec['formChangesInto'] = [{'from': a, 'method': b} for a, b in rec['formChangesInto']]

    # ---- family / stage ----
    def choose_parent(name):
        cands = parents.get(name) or []
        kind = 'evolution'
        if not cands:
            cands = breed_parents.get(name) or []
            kind = 'breed'
        if not cands:
            return None, None, None
        # prefer non-cosmetic, earliest species id
        cands = sorted(cands, key=lambda pe: (S[pe[0]]['formIndex'] != 0 and S[pe[0]]['region'] is None, S[pe[0]]['id']))
        return cands[0][0], cands[0][1], kind

    for name, rec in S.items():
        p, evo, kind = choose_parent(name)
        rec['preEvolution'] = p
        rec['preEvolutionLink'] = kind
        all_p = sorted({pe[0] for pe in parents.get(name, [])}, key=lambda n: S[n]['id'])
        rec['preEvolutions'] = all_p
        rec['breedOnlyPreEvolutions'] = sorted({pe[0] for pe in breed_parents.get(name, [])}, key=lambda n: S[n]['id'])
        rec['_preEvo'] = evo
        if len(all_p) > 1:
            pass

    memo = {}

    def anchor(name, stack=()):
        """(chainRoot, depth) where chainRoot is the family root species."""
        if name in memo:
            return memo[name]
        if name in stack:
            ex.anomaly('evolution cycle through %s' % name)
            return name, 0
        rec = S[name]
        stack = stack + (name,)
        if rec['preEvolution']:
            r, d = anchor(rec['preEvolution'], stack)
            res = (r, d + 1)
        else:
            # no pre-evolution: alternate forms inherit from the first form of
            # the same region in their form table
            same = [fm for fm in rec['forms'] if fm in S and S[fm]['region'] == rec['region']]
            b = same[0] if same else name
            if b != name:
                res = anchor(b, stack)
            else:
                res = (name, 0)
        memo[name] = res
        return res

    for name, rec in S.items():
        r, d = anchor(name)
        rec['familyRoot'] = r
        rec['chainDepth'] = d

    babies = set(BABY_SPECIES)
    for name, rec in S.items():
        rec['isBaby'] = name in babies or (rec['baseForm'] in babies and rec['formOf'] is not None
                                            and rec['familyRoot'] == S[rec['baseForm']]['familyRoot']
                                            and rec['chainDepth'] == 0)
    for b in BABY_SPECIES:
        if b not in S:
            ex.anomaly('baby species %s not defined' % b)

    # official stage: babies and the basic they evolve into are both stage 0
    def walk_chain(name):
        chain = []
        cur = name
        seen = set()
        while cur and cur not in seen:
            seen.add(cur)
            chain.append(cur)
            rec = S[cur]
            if rec['preEvolution']:
                cur = rec['preEvolution']
            else:
                same = [fm for fm in rec['forms'] if fm in S and S[fm]['region'] == rec['region']]
                b = same[0] if same else cur
                cur = b if b != cur else None
        return chain

    for name, rec in S.items():
        chain = walk_chain(name)
        # count real evolution steps, skipping baby->basic and form hops
        stage = 0
        for a, b in zip(chain, chain[1:]):
            if S[a]['preEvolution'] == b:  # a evolved from b
                if S[b]['isBaby']:
                    continue
                stage += 1
        rec['stage'] = stage
        rec['chain'] = list(reversed([c for i, c in enumerate(chain)
                                      if i == 0 or S[chain[i - 1]]['preEvolution'] == c]))

    # ---- region / location locks on evolutions ----
    for name, rec in S.items():
        for e in rec['evolutions']:
            reg = [c['args'][0] for c in e['conditions'] if c['condition'] == 'IF_REGION' and c['args']]
            nreg = [c['args'][0] for c in e['conditions'] if c['condition'] == 'IF_NOT_REGION' and c['args']]
            loc = [c['args'][0] for c in e['conditions'] if c['condition'] in ('IF_IN_MAP', 'IF_IN_MAPSEC') and c['args']]
            e['requiresRegion'] = reg[0] if reg else None
            e['forbiddenRegion'] = nreg[0] if nreg else None
            e['requiresLocation'] = loc[0] if loc else None
    for name, rec in S.items():
        inc = [(p, e) for p, e in parents.get(name, [])]
        regs = {e['requiresRegion'] for _, e in inc}
        rec['evolutionRequiresRegion'] = regs.pop() if inc and len(regs) == 1 and None not in regs else None
        locs = {e['requiresLocation'] for _, e in inc}
        rec['evolutionRequiresLocation'] = locs.pop() if inc and len(locs) == 1 and None not in locs else None

    # ---- evolution summaries ----
    for name, rec in S.items():
        real = [e for e in rec['evolutions'] if e['category'] != 'breed' and e['targetDefined']]
        lv = [e['param'] for e in real if e['category'] == 'level']
        rec['evolvesAtLevel'] = min(lv) if lv else None
        cats = []
        for e in real:
            if e['category'] not in cats:
                cats.append(e['category'])
        rec['evoMethodSummary'] = '/'.join(cats) if cats else None
        rec['evolvesInto'] = []
        for e in real:
            if e['target'] not in rec['evolvesInto']:
                rec['evolvesInto'].append(e['target'])
        pe = rec.pop('_preEvo')
        if pe is not None and rec['preEvolutionLink'] == 'evolution':
            rec['evolvedBy'] = pe['category']
            rec['evolvesFromLevel'] = pe['param'] if pe['category'] == 'level' else None
        else:
            rec['evolvedBy'] = None
            rec['evolvesFromLevel'] = None
        rec['finalStage'] = not rec['canEvolve']

    # ---- flags summary ----
    for name, rec in S.items():
        fl = rec['flags']
        rec['isLegendary'] = fl['isRestrictedLegendary'] or fl['isSubLegendary']
        rec['isRestrictedLegendary'] = fl['isRestrictedLegendary']
        rec['isSubLegendary'] = fl['isSubLegendary']
        rec['isMythical'] = fl['isMythical']
        rec['isUltraBeast'] = fl['isUltraBeast']
        rec['isParadox'] = fl['isParadox']
        rec['isLegendaryish'] = (rec['isLegendary'] or rec['isMythical'] or rec['isUltraBeast']
                                 or rec['isParadox'])
        for k in ('isMegaEvolution', 'isPrimalReversion', 'isUltraBurst', 'isGigantamax', 'isTotem',
                  'isTeraForm', 'isAlolanForm', 'isGalarianForm', 'isHisuianForm', 'isPaldeanForm'):
            rec[k] = fl[k]
        rec['isRegionalForm'] = rec['region'] is not None

    classify_forms(S, ex)

    # starters
    starter_roots = set(STARTER_ROOTS)
    for r in STARTER_ROOTS:
        if r not in S:
            ex.anomaly('starter %s not defined' % r)
    for name, rec in S.items():
        rec['isStarter'] = rec['familyRoot'] in starter_roots

    suggest_levels(S)

    # ---- baby heuristic cross-check ----
    for name, rec in S.items():
        heur = (rec['chainDepth'] == 0 and rec['canEvolve'] and rec['formOf'] is None
                and rec['eggGroups'] == ['EGG_GROUP_NO_EGGS_DISCOVERED'] and not rec['isLegendaryish'])
        if heur and not rec['isBaby'] and name not in NOT_BABY:
            ex.anomaly('info: %s looks like a baby (Undiscovered egg group, evolves) but is not in the baby list' % name)
        if rec['isBaby'] and rec['formOf'] is None and not heur:
            ex.anomaly('info: baby %s fails the baby heuristic' % name)

    # final field order cleanup
    for name, rec in S.items():
        rec.pop('chain', None)


def classify_forms(S, ex):
    """Decide formKind / encounterable for every species (see README)."""
    # cosmetic duplicates: identical battle data to an earlier kept form of the
    # same form table (same region)
    for name, rec in S.items():
        reasons = []
        kind = 'base' if rec['formOf'] is None else 'alternate'
        if rec['region']:
            kind = 'regional'
        req_item = None
        fl = rec['flags']
        if fl['isMegaEvolution']:
            kind, reasons = 'mega', ['Mega Evolution (battle only)']
        elif fl['isPrimalReversion']:
            kind, reasons = 'primal', ['Primal Reversion (battle only)']
        elif fl['isUltraBurst']:
            kind, reasons = 'ultraBurst', ['Ultra Burst (battle only)']
        elif fl['isGigantamax']:
            kind, reasons = 'gigantamax', ['Gigantamax (battle only)']
        elif fl['isTotem']:
            kind, reasons = 'totem', ['Totem Pokemon']
        elif fl['isTeraForm']:
            kind, reasons = 'tera', ['Tera form (battle only)']
        rec['formKind'] = kind
        rec['_reasons'] = reasons
        rec['requiredItem'] = req_item
        rec['requiredItems'] = []

    # fusion forms (gFusionTablePointers)
    for name, rec in S.items():
        if rec['_reasons']:
            continue
        if rec.get('fusion'):
            rec['formKind'] = 'fusion'
            rec['_reasons'] = ['fusion form (%s + %s with %s)' % (rec['fusion']['components'][0],
                                                                 rec['fusion']['components'][1],
                                                                 rec['fusion']['item'])]

    # battle-only / held-item forms, judged from the form changes that lead
    # into a form.  Changes coming from forms that are themselves battle-only
    # (e.g. Ogerpon Tera -> Ogerpon at the end of battle) are ignored; iterate
    # until nothing changes.
    changed = True
    while changed:
        changed = False
        for name, rec in S.items():
            if rec['_reasons'] or rec['formOf'] is None:
                continue
            if rec['preEvolutions']:
                continue  # reachable by evolution: a real form
            inc = [c for c in rec['formChangesInto'] if not S[c['from']]['_reasons'] or
                   S[c['from']]['formKind'] not in BATTLE_KINDS]
            methods = {c['method'] for c in inc}
            if not methods:
                continue  # no way to change into it: obtainable as is (e.g. Tauros breeds, Unown letters)
            battle = {m for m in methods if m.startswith(BATTLE_FORM_METHODS_PREFIX) or m in BATTLE_FORM_METHODS_EXTRA}
            held = methods & HELD_ITEM_FORM_METHODS
            revert = methods & REVERT_FORM_METHODS
            persistent = methods - battle - held - revert
            if persistent:
                continue
            if held:
                its = set()
                for c in inc:
                    for ch in S[c['from']]['formChanges']:
                        if ch['target'] == name and ch['method'] in HELD_ITEM_FORM_METHODS and ch['params']:
                            its.add(str(ch['params'][0]))
                rec['formKind'] = 'heldItem'
                its_sorted = sorted(its, key=lambda i: (i.endswith('IUM_Z'), i))  # plates before Z-crystals
                rec['requiredItems'] = its_sorted
                rec['requiredItem'] = its_sorted[0] if its_sorted else None
                rec['_reasons'] = ['form requires holding %s' % ', '.join(sorted(its))]
                changed = True
            elif battle:
                rec['formKind'] = 'battleOnly'
                rec['_reasons'] = ['battle-only form (reached via %s)' % ', '.join(sorted(battle))]
                changed = True
            # reached only through FAINT/END_BATTLE reverts: it is the
            # persistent out-of-battle form (e.g. Greninja Battle Bond)

    # explicit exclusions
    for name, rec in S.items():
        if rec['_reasons']:
            continue
        if name in EXPLICIT_EXCLUDE:
            rec['formKind'] = 'special'
            rec['_reasons'] = [EXPLICIT_EXCLUDE[name]]
            continue
        for rx, why in EXPLICIT_EXCLUDE_PATTERNS:
            if rx.match(name):
                rec['formKind'] = 'special'
                rec['_reasons'] = [why]
                break

    # cosmetic duplicates (needs the battle-only pass first so that e.g.
    # Cherrim Sunshine is reported as battle-only)
    for name, rec in S.items():
        rec['cosmeticOf'] = None
    for name, rec in S.items():
        if rec['_reasons'] or rec['formOf'] is None:
            continue
        sig = battle_signature(rec)
        for other in rec['forms']:
            if other == name:
                break
            o = S.get(other)
            if o is None or o['_reasons'] or o['cosmeticOf']:
                continue
            if battle_signature(o) == sig and o['region'] == rec['region']:
                rec['formKind'] = 'cosmetic'
                rec['cosmeticOf'] = other
                rec['_reasons'] = ['cosmetic duplicate of %s (identical types/stats/abilities)' % other]
                break

    for name, rec in S.items():
        rec['encounterable'] = not rec['_reasons']
        rec['excludeReason'] = '; '.join(rec['_reasons']) if rec['_reasons'] else None
        rec['isBattleOnly'] = rec['formKind'] in BATTLE_KINDS
        rec['isCosmetic'] = rec['formKind'] == 'cosmetic'
        del rec['_reasons']


def suggest_levels(S):
    """Heuristic minimum level at which a species 'naturally' appears."""
    memo = {}

    def lvl(name, depth=0):
        if name in memo:
            return memo[name]
        rec = S[name]
        if depth > 20:
            return 1
        p = rec['preEvolution'] if rec['preEvolutionLink'] == 'evolution' else None
        if p is None:
            if rec['formOf'] and rec['familyRoot'] != name:
                v = lvl(rec['familyRoot'], depth + 1) if rec['chainDepth'] == 0 else None
                if v is None:
                    # alternate form of an evolved species: same as its base
                    same = [fm for fm in rec['forms'] if fm in S and S[fm]['region'] == rec['region']]
                    v = lvl(same[0], depth + 1) if same and same[0] != name else 1
            else:
                v = 1
        elif S[p]['isBaby']:
            # the basic a baby evolves into is normally found in the wild, so
            # it starts at 1 -- unless the baby evolves at an explicit level
            # (Tyrogue 20, Wynaut 15, Smoochum/Elekid/Magby/Toxel 30)
            v = rec['evolvesFromLevel'] if rec['evolvedBy'] == 'level' else 1
        elif rec['evolvedBy'] == 'level':
            v = max(rec['evolvesFromLevel'], lvl(p, depth + 1))
        else:
            base = lvl(p, depth + 1)
            v = max(base + 5, 20 if rec['stage'] <= 1 else 30)
        memo[name] = v
        return v

    for name, rec in S.items():
        rec['suggestedMinLevel'] = lvl(name)


# --------------------------------------------------------------------------
# Families
# --------------------------------------------------------------------------

def build_families(S):
    fam = collections.OrderedDict()
    for name, rec in S.items():
        if not rec['encounterable']:
            continue
        fam.setdefault(rec['familyRoot'], []).append(name)
    out = []
    for root, members in fam.items():
        members = sorted(members, key=lambda n: (S[n]['stage'], S[n]['chainDepth'], S[n]['id']))
        out.append(collections.OrderedDict([
            ('root', root),
            ('rootName', S[root]['speciesName']),
            ('rootEncounterable', S[root]['encounterable']),
            ('members', members),
            ('stages', collections.OrderedDict((m, S[m]['stage']) for m in members)),
            ('maxStage', max(S[m]['stage'] for m in members)),
            ('isLegendaryish', any(S[m]['isLegendaryish'] for m in members)),
            ('isLegendary', any(S[m]['isLegendary'] for m in members)),
            ('isMythical', any(S[m]['isMythical'] for m in members)),
            ('isUltraBeast', any(S[m]['isUltraBeast'] for m in members)),
            ('isParadox', any(S[m]['isParadox'] for m in members)),
            ('hasBaby', any(S[m]['isBaby'] for m in members)),
            ('isStarter', S[root]['isStarter']),
            ('gens', sorted({S[m]['generation'] for m in members if S[m]['generation']})),
            ('natDexNums', sorted({S[m]['natDexNum'] for m in members if S[m]['natDexNum']})),
        ]))
    return out


# --------------------------------------------------------------------------
# Reporting / sanity checks
# --------------------------------------------------------------------------

def summarize(meta, S, families):
    enc = [r for r in S.values() if r['encounterable']]
    c = collections.OrderedDict()
    c['totalSpeciesEntries'] = len(S)
    c['encounterable'] = len(enc)
    c['families'] = len(families)
    c['distinctNatDex'] = len({r['natDexNum'] for r in S.values() if r['natDexNum']})
    c['distinctNatDexEncounterable'] = len({r['natDexNum'] for r in enc if r['natDexNum']})
    c['maxNatDex'] = max((r['natDexNum'] or 0) for r in S.values())
    per_gen = collections.OrderedDict()
    for g in range(1, 10):
        per_gen[str(g)] = collections.OrderedDict([
            ('entries', sum(1 for r in S.values() if r['generation'] == g)),
            ('encounterable', sum(1 for r in enc if r['generation'] == g)),
            ('natDex', len({r['natDexNum'] for r in S.values() if r['generation'] == g})),
            ('families', sum(1 for f in families if f['gens'] and min(f['gens']) == g)),
        ])
    c['perGeneration'] = per_gen
    for k in ('isLegendary', 'isRestrictedLegendary', 'isSubLegendary', 'isMythical', 'isUltraBeast', 'isParadox',
              'isLegendaryish', 'isBaby', 'isStarter', 'isRegionalForm'):
        c[k] = collections.OrderedDict([
            ('entries', sum(1 for r in S.values() if r[k])),
            ('encounterable', sum(1 for r in enc if r[k])),
            ('natDex', len({r['natDexNum'] for r in S.values() if r[k]})),
        ])
    c['formKinds'] = collections.OrderedDict(sorted(collections.Counter(r['formKind'] for r in S.values()).items()))
    c['familiesLegendaryish'] = sum(1 for f in families if f['isLegendaryish'])
    c['regionLockedEvolutions'] = sorted(n for n, r in S.items() if r['evolutionRequiresRegion'])
    c['locationLockedEvolutions'] = sorted('%s->%s@%s' % (n, e['target'], e['requiresLocation'])
                                           for n, r in S.items() for e in r['evolutions'] if e['requiresLocation'])
    return c


def sanity_checks(S, families, full_config=True):
    """Built-in checks.  Each line starts with OK / FAIL / SKIP / INFO.

    A check whose species are not defined (families disabled in the config)
    is reported as SKIP instead of crashing.  Count checks that only hold on
    the default all-enabled config (1025 dex numbers, 27 starters, 19 babies,
    exact family member lists) are downgraded to INFO when full_config is
    False, so a reduced config does not make the tool fail."""
    out = []
    fam_by_root = {f['root']: f for f in families}

    alias = {}
    for n, r in S.items():
        for a in r['aliases']:
            alias[a] = n

    def canon(n):
        return alias.get(n, n)

    def g(n):
        return S.get(canon(n))

    def check(msg, cond, needs=(), full_only=False):
        """cond: callable returning (bool) or (bool, detail)."""
        missing = [n for n in needs if g(n) is None]
        if missing:
            out.append('SKIP %s (not defined in this config: %s)' % (msg, ', '.join(missing)))
            return None
        try:
            res = cond()
        except Exception as e:  # never crash on a reduced/modified tree
            res = (False, 'error: %s: %s' % (type(e).__name__, e))
        detail = None
        if isinstance(res, tuple):
            res, detail = res
        text = msg + (' %s' % (detail,) if detail is not None else '')
        if res:
            out.append('OK   ' + text)
        else:
            out.append(('INFO ' if full_only and not full_config else 'FAIL ') + text)
        return bool(res)

    b = fam_by_root.get('SPECIES_BULBASAUR')
    check('Bulbasaur family 3 members, stages 0/1/2 =',
          lambda: (b['members'] == ['SPECIES_BULBASAUR', 'SPECIES_IVYSAUR', 'SPECIES_VENUSAUR']
                   and list(b['stages'].values()) == [0, 1, 2], dict(b['stages'])),
          needs=('SPECIES_BULBASAUR', 'SPECIES_IVYSAUR', 'SPECIES_VENUSAUR'))
    check('Ivysaur evolves from Bulbasaur at 16',
          lambda: g('SPECIES_IVYSAUR')['evolvesFromLevel'] == 16 and g('SPECIES_BULBASAUR')['evolvesAtLevel'] == 16,
          needs=('SPECIES_BULBASAUR', 'SPECIES_IVYSAUR'))
    check('Venusaur at 32', lambda: g('SPECIES_VENUSAUR')['evolvesFromLevel'] == 32, needs=('SPECIES_VENUSAUR',))
    e = fam_by_root.get('SPECIES_EEVEE')
    check('Eevee family has 9 members:', lambda: (len(e['members']) == 9, e['members']),
          needs=('SPECIES_EEVEE',), full_only=True)
    p = fam_by_root.get('SPECIES_PICHU')
    check('Pikachu family rooted at baby Pichu:',
          lambda: ('SPECIES_PIKACHU' in p['members'] and 'SPECIES_RAICHU' in p['members']
                   and 'SPECIES_RAICHU_ALOLA' in p['members'] and g('SPECIES_PICHU')['isBaby'], p['members']),
          needs=('SPECIES_PICHU', 'SPECIES_PIKACHU', 'SPECIES_RAICHU', 'SPECIES_RAICHU_ALOLA'))
    check('Pikachu stage 0 (basic), Raichu stage 1',
          lambda: g('SPECIES_PIKACHU')['familyRoot'] == 'SPECIES_PICHU' and g('SPECIES_PIKACHU')['stage'] == 0
          and g('SPECIES_RAICHU')['stage'] == 1, needs=('SPECIES_PICHU', 'SPECIES_PIKACHU', 'SPECIES_RAICHU'))
    ra = fam_by_root.get('SPECIES_RATTATA_ALOLA')
    check('Alolan Rattata is its own family root:',
          lambda: (ra['members'] == ['SPECIES_RATTATA_ALOLA', 'SPECIES_RATICATE_ALOLA'], ra['members']),
          needs=('SPECIES_RATTATA_ALOLA', 'SPECIES_RATICATE_ALOLA'))
    check('Kantonian Rattata family = Rattata, Raticate',
          lambda: fam_by_root['SPECIES_RATTATA']['members'] == ['SPECIES_RATTATA', 'SPECIES_RATICATE'],
          needs=('SPECIES_RATTATA', 'SPECIES_RATICATE'))
    ap = fam_by_root.get('SPECIES_APPLIN')
    exp_ap = {'SPECIES_APPLIN', 'SPECIES_FLAPPLE', 'SPECIES_APPLETUN', 'SPECIES_DIPPLIN', 'SPECIES_HYDRAPPLE'}
    check('Applin family:', lambda: (set(ap['members']) == exp_ap, dict(ap['stages'])),
          needs=('SPECIES_APPLIN',), full_only=True)
    check('Kingambit evolves from Bisharp (stage 2, root Pawniard)',
          lambda: g('SPECIES_KINGAMBIT')['familyRoot'] == 'SPECIES_PAWNIARD'
          and g('SPECIES_KINGAMBIT')['preEvolution'] == 'SPECIES_BISHARP' and g('SPECIES_KINGAMBIT')['stage'] == 2,
          needs=('SPECIES_KINGAMBIT', 'SPECIES_BISHARP', 'SPECIES_PAWNIARD'))
    check('Wyrdeer evolves from Stantler',
          lambda: (g('SPECIES_WYRDEER')['preEvolution'] == 'SPECIES_STANTLER', '(%s)' % g('SPECIES_WYRDEER')['evolvedBy']),
          needs=('SPECIES_WYRDEER', 'SPECIES_STANTLER'))
    for n in ('SPECIES_SPRIGATITO', 'SPECIES_GHOLDENGO', 'SPECIES_PECHARUNT', 'SPECIES_TERAPAGOS'):
        check('%s exists and is encounterable' % n,
              lambda n=n: (g(n)['encounterable'], '(gen %s)' % g(n)['generation']), needs=(n,))
    check('Gholdengo root Gimmighoul',
          lambda: (g('SPECIES_GHOLDENGO')['familyRoot'] == canon('SPECIES_GIMMIGHOUL'), '(%s)' % canon('SPECIES_GIMMIGHOUL')),
          needs=('SPECIES_GHOLDENGO', 'SPECIES_GIMMIGHOUL'))
    nd = {r['natDexNum'] for r in S.values() if r['natDexNum']}
    check('national dex 1..1025 all present', lambda: (nd == set(range(1, 1026)), '(%d distinct)' % len(nd)),
          full_only=True)
    nde = {r['natDexNum'] for r in S.values() if r['encounterable']}
    check('every national dex number has an encounterable form', lambda: (nde == nd, '(%d)' % len(nde)))
    for n in ('SPECIES_VENUSAUR_MEGA', 'SPECIES_KYOGRE_PRIMAL', 'SPECIES_CHARIZARD_GMAX', 'SPECIES_AEGISLASH_BLADE',
              'SPECIES_DARMANITAN_ZEN', 'SPECIES_MIMIKYU_BUSTED', 'SPECIES_CRAMORANT_GULPING', 'SPECIES_EISCUE_NOICE',
              'SPECIES_ZYGARDE_COMPLETE', 'SPECIES_MINIOR_CORE_RED', 'SPECIES_VIVILLON_POLAR',
              'SPECIES_UNOWN_B', 'SPECIES_ALCREMIE_STRAWBERRY_RUBY_CREAM', 'SPECIES_ARCEUS_FIRE', 'SPECIES_SILVALLY_FIRE', 'SPECIES_GENESECT_DOUSE',
              'SPECIES_CASTFORM_SUNNY', 'SPECIES_FURFROU_HEART', 'SPECIES_PIKACHU_COSPLAY', 'SPECIES_PIKACHU_ORIGINAL',
              'SPECIES_BURMY_SANDY', 'SPECIES_FLABEBE_BLUE', 'SPECIES_RATICATE_ALOLA_TOTEM', 'SPECIES_NECROZMA_ULTRA',
              'SPECIES_KYUREM_BLACK', 'SPECIES_CALYREX_SHADOW', 'SPECIES_GIRATINA_ORIGIN', 'SPECIES_OGERPON_WELLSPRING',
              'SPECIES_PALAFIN_HERO', 'SPECIES_MORPEKO_HANGRY', 'SPECIES_WISHIWASHI_SCHOOL', 'SPECIES_CHERRIM_SUNSHINE',
              'SPECIES_MELOETTA_PIROUETTE', 'SPECIES_GRENINJA_ASH', 'SPECIES_TERAPAGOS_STELLAR', 'SPECIES_ZACIAN_CROWNED',
              'SPECIES_SHELLOS_EAST', 'SPECIES_DEERLING_SUMMER', 'SPECIES_XERNEAS_ACTIVE', 'SPECIES_EEVEE_STARTER'):
        check('%s excluded' % n, lambda n=n: (not g(n)['encounterable'],
                                               '(%s: %s)' % (g(n)['formKind'], g(n)['excludeReason'])), needs=(n,))
    for n in ('SPECIES_ROTOM_WASH', 'SPECIES_ORICORIO_POM_POM', 'SPECIES_LYCANROC_MIDNIGHT', 'SPECIES_LYCANROC_DUSK',
              'SPECIES_URSHIFU_RAPID_STRIKE', 'SPECIES_TAUROS_PALDEA_BLAZE', 'SPECIES_WORMADAM_SANDY',
              'SPECIES_DEOXYS_ATTACK', 'SPECIES_MEOWSTIC_F', 'SPECIES_TOXTRICITY_LOW_KEY', 'SPECIES_URSALUNA_BLOODMOON',
              'SPECIES_BASCULEGION_F', 'SPECIES_SNEASLER', 'SPECIES_CLODSIRE', 'SPECIES_DECIDUEYE_HISUI',
              'SPECIES_GOURGEIST_SUPER', 'SPECIES_HOOPA_UNBOUND', 'SPECIES_SHAYMIN_SKY', 'SPECIES_LANDORUS_THERIAN',
              'SPECIES_NECROZMA', 'SPECIES_UNOWN', 'SPECIES_VIVILLON', 'SPECIES_ALCREMIE', 'SPECIES_MINIOR_METEOR',
              'SPECIES_ARCEUS_NORMAL', 'SPECIES_CASTFORM_NORMAL', 'SPECIES_ZYGARDE_10', 'SPECIES_ZYGARDE_50',
              'SPECIES_ROCKRUFF_OWN_TEMPO', 'SPECIES_BASCULIN_WHITE_STRIPED', 'SPECIES_SQUAWKABILLY_YELLOW', 'SPECIES_GIMMIGHOUL'):
        check('%s encounterable' % n, lambda n=n: (g(n)['encounterable'], '(%s, formOf %s, root %s)' % (
            g(n)['formKind'], g(n)['formOf'], g(n)['familyRoot'])), needs=(n,))
    st = [f['root'] for f in families if f['isStarter']]
    check('27 starter families', lambda: (len(st) == 27, '(%d)' % len(st)), full_only=True)
    bb = [n for n, r in S.items() if r['isBaby'] and r['encounterable']]
    check('19 encounterable babies', lambda: (len(bb) == 19, '(%d)' % len(bb)), full_only=True)
    bad_roots = [f['root'] for f in families if not f['rootEncounterable']]
    check('all family roots encounterable', lambda: (not bad_roots, bad_roots[:10]))
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('--root', default=DEFAULT_ROOT, help='expansion tree (default %(default)s)')
    ap.add_argument('--out', default=DEFAULT_OUT, help='species JSON output (default %(default)s)')
    ap.add_argument('--families', default=DEFAULT_FAMILIES, help='families JSON output (default %(default)s)')
    ap.add_argument('--cpp', default=None, help='preprocessor command (default arm-none-eabi-cpp / cpp)')
    ap.add_argument('-D', '--define', action='append', default=[], help='extra -D define passed to cpp')
    ap.add_argument('--game-version', default=None, help='GAME_VERSION define (default: from Makefile)')
    ap.add_argument('--keep-preprocessed', default=None, help='save cpp output here (debug)')
    ap.add_argument('--quiet', action='store_true')
    args = ap.parse_args(argv)

    meta, species, _src = build(args.root, args.cpp, args.define, args.keep_preprocessed, args.game_version)
    families = build_families(species)
    counts = summarize(meta, species, families)
    meta['counts'] = counts
    cs = meta['configSummary']
    full_config = all(cs['genFlags'].values()) and not cs['familiesDisabled']
    checks = sanity_checks(species, families, full_config)
    meta['sanityChecks'] = checks

    def write_json(path, obj):
        # write to a temp file and rename, so other tools reading the shared
        # /home/user/work outputs never see a half-written file
        path = os.path.abspath(path)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        tmp = path + '.tmp%d' % os.getpid()
        with open(tmp, 'w', encoding='utf-8') as f:
            json.dump(obj, f, ensure_ascii=False, indent=1)
        os.replace(tmp, path)

    write_json(args.out, {'meta': meta, 'species': species})
    write_json(args.families, {'meta': {'schemaVersion': SCHEMA_VERSION, 'source': os.path.abspath(args.out),
                                        'count': len(families)}, 'families': families})

    if not args.quiet:
        log('wrote %s (%d species) and %s (%d families)' % (args.out, len(species), args.families, len(families)))
        log(json.dumps(counts, indent=1))
        log('--- sanity checks ---')
        for line in checks:
            log(line)
        log('--- anomalies (%d) ---' % len(meta['anomalies']))
        for a in meta['anomalies']:
            log('  ' + a)
        if meta['stubbedHeaders']:
            log('stubbed generated headers: %s' % ', '.join(meta['stubbedHeaders']))
    fails = [c for c in checks if c.startswith('FAIL')]
    return 1 if fails else 0


if __name__ == '__main__':
    sys.exit(main())
