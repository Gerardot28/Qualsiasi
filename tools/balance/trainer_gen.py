#!/usr/bin/env python3
"""Deterministic trainer rebalancer for Pokemon Multiverse (pokeemerald-expansion 1.17.1).

Reads the vanilla src/data/trainers.party (read-only tree), regenerates every
regular overworld trainer (themed all-generation parties, mapped levels,
mdb.best_moveset movesets), merges the hand-authored boss file bosses.party and
writes the new trainers.party.  See README.md in this directory.

    python3 -I tools/balance/trainer_gen.py --root /home/user/pex-orig --out /path/trainers.party
"""
import argparse, json, os, re, sys, zlib, random

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, '..', 'movedb'))
import mdb  # noqa: E402

DATA = os.environ.get('MOVEDB_DIR', '/home/user/work')

# ---------------------------------------------------------------- level curve
CURVE = [(2, 2), (5, 6), (12, 13), (15, 15), (19, 19), (24, 24), (29, 29), (31, 33), (33, 37),
         (42, 44), (46, 49), (49, 52), (55, 57), (58, 62), (70, 72), (100, 100)]


def map_level(v):
    if v <= CURVE[0][0]:
        return max(1, v)
    for (a, x), (b, y) in zip(CURVE, CURVE[1:]):
        if a <= v <= b:
            return int(round(x + (y - x) * (v - a) / (b - a)))
    return min(100, v)


def iv_for(level, rematch_idx=0):
    iv = 8 + (level - 6) * 12.0 / 43.0
    iv = int(round(min(20, max(8, iv))))
    if rematch_idx >= 2:
        iv = min(25, iv + 2 * (rematch_idx - 1))
        if rematch_idx >= 4:
            iv = 25
    return iv

# ---------------------------------------------------------------- party parsing


def parse_party(text):
    """Return (preamble, [entry]) where entry = {id, raw, header:[lines], mons:[[lines]]}."""
    parts = re.split(r'(?m)^(?==== TRAINER_)', text)
    pre, out = parts[0], []
    for p in parts[1:]:
        tid = re.match(r'=== (TRAINER_\w+) ===', p).group(1)
        lines = p.rstrip('\n').split('\n')
        body = lines[1:]
        blocks, cur = [], []
        for ln in body:
            if ln.strip() == '':
                if cur:
                    blocks.append(cur)
                    cur = []
            else:
                cur.append(ln)
        if cur:
            blocks.append(cur)
        header = blocks[0] if blocks and ':' in blocks[0][0] and not blocks[0][0].startswith('Level') else []
        mons = blocks[1:] if header else blocks
        out.append({'id': tid, 'raw': p, 'header': header, 'mons': mons})
    return pre, out


def header_get(header, key):
    for ln in header:
        if ln.startswith(key + ':'):
            return ln.split(':', 1)[1].strip()
    return None


def header_set(header, key, value, after=('Double Battle', 'Battle Type', 'Music')):
    out = [ln for ln in header if not ln.startswith(key + ':')]
    if value is None:
        return out
    newl = '%s: %s' % (key, value)
    if key == 'Items':  # vanilla order: ... Music, Items, Double Battle, AI
        for i, ln in enumerate(out):
            if ln.startswith('Double Battle:') or ln.startswith('Battle Type:'):
                return out[:i] + [newl] + out[i:]
    for i, ln in enumerate(out):
        if ln.startswith('AI:') and key != 'AI':
            return out[:i + 1] + [newl] + out[i + 1:]
    for k in after:
        for i, ln in enumerate(out):
            if ln.startswith(k + ':'):
                return out[:i + 1] + [newl] + out[i + 1:]
    return out + [newl]


def is_double(header):
    v = header_get(header, 'Double Battle') or header_get(header, 'Battle Type') or ''
    return v.lower() in ('yes', 'doubles')


def mon_level(block):
    for ln in block:
        if ln.startswith('Level:'):
            return int(ln.split(':')[1])
    return 100

# ---------------------------------------------------------------- species db


class SpeciesDB:
    PSEUDO_ROOTS = {'SPECIES_DRATINI', 'SPECIES_LARVITAR', 'SPECIES_BAGON', 'SPECIES_BELDUM',
                    'SPECIES_GIBLE', 'SPECIES_DEINO', 'SPECIES_GOOMY', 'SPECIES_JANGMO_O',
                    'SPECIES_DREEPY', 'SPECIES_FRIGIBAX'}
    # never used by generated trainers (gimmicks / region-locked / one-trick)
    BANNED = {'SPECIES_SHEDINJA', 'SPECIES_SMEARGLE', 'SPECIES_DITTO', 'SPECIES_UNOWN', 'SPECIES_WOBBUFFET',
              'SPECIES_WYNAUT', 'SPECIES_URSALUNA', 'SPECIES_SLAKING', 'SPECIES_REGIGIGAS', 'SPECIES_MAGIKARP',
              'SPECIES_FEEBAS', 'SPECIES_KRICKETOT', 'SPECIES_CASTFORM', 'SPECIES_KECLEON', 'SPECIES_MINIOR',
              'SPECIES_COMBEE', 'SPECIES_BURMY', 'SPECIES_SCATTERBUG', 'SPECIES_SPEWPA', 'SPECIES_WURMPLE',
              'SPECIES_CATERPIE', 'SPECIES_WEEDLE', 'SPECIES_KAKUNA', 'SPECIES_METAPOD', 'SPECIES_SILCOON',
              'SPECIES_CASCOON', 'SPECIES_TYROGUE', 'SPECIES_TANDEMAUS', 'SPECIES_GIMMIGHOUL',
              'SPECIES_FLITTLE', 'SPECIES_TYNAMO', 'SPECIES_DUNSPARCE', 'SPECIES_DUDUNSPARCE',
              'SPECIES_GHOLDENGO', 'SPECIES_PALAFIN', 'SPECIES_WISHIWASHI', 'SPECIES_ARCHALUDON',
              'SPECIES_KINGAMBIT', 'SPECIES_DURALUDON', 'SPECIES_ANNIHILAPE', 'SPECIES_FARIGIRAF', 'SPECIES_HYDRAPPLE',
              'SPECIES_SINISTCHA', 'SPECIES_POLTCHAGEIST', 'SPECIES_SINISTEA', 'SPECIES_POLTEAGEIST'}

    def __init__(self):
        raw = json.load(open(os.path.join(DATA, 'species.json')))['species']
        fams = json.load(open(os.path.join(DATA, 'families.json')))['families']
        types = mdb.load_species()  # fixes numeric 9/19 types
        self.raw = raw
        self.types = {k: v['types'] for k, v in types.items()}
        self.fam_of, self.families = {}, {}
        for f in fams:
            bad = f['isLegendaryish'] or f['isLegendary'] or f['isMythical'] or f['isUltraBeast'] or f['isParadox']
            self.families[f['root']] = dict(f, banned=bad)
            for m in f['members']:
                self.fam_of[m] = f['root']
        self._min = {}
        self.base = [k for k, v in raw.items() if v.get('formIndex', 0) == 0 and v.get('formOf') is None
                     and k != 'SPECIES_NONE']

    def bst(self, s):
        return self.raw[s]['bst']

    def is_pseudo(self, s):
        return self.fam_of.get(s) in self.PSEUDO_ROOTS

    def is_banned_family(self, s):
        f = self.families.get(self.fam_of.get(s))
        return f is None or f['banned']

    def min_level(self, s):
        """Lowest level at which a trainer may plausibly own species s."""
        if s in self._min:
            return self._min[s]
        sp = self.raw[s]
        pre = sp.get('preEvolution')
        if not pre or pre not in self.raw:
            lvl = 1
        else:
            baby_step = self.raw[pre].get('isBaby') and sp.get('stage', 0) == 0
            th = None
            for e in self.raw[pre].get('evolutions') or []:
                if e['target'] != s:
                    continue
                if e['category'] == 'level' and isinstance(e['param'], int) and e['param'] > 0:
                    t = e['param']
                elif baby_step:
                    t = 1  # baby -> basic (friendship/incense): the basic is wild-catchable
                else:
                    t = 30 if sp.get('stage', 1) <= 1 else 38
                th = t if th is None else min(th, t)
            if th is None:
                th = 30 if sp.get('stage', 1) <= 1 else 38
            lvl = max(self.min_level(pre), th)
        if self.is_pseudo(s):
            lvl = max(lvl, 35)
        self._min[s] = lvl
        return lvl

    def next_evo_level(self, s):
        """Level at which s would evolve (for 'too unevolved' checks), or None."""
        best = None
        for e in self.raw[s].get('evolutions') or []:
            if e['target'] not in self.raw or e['category'] == 'breed':
                continue
            t = self.min_level(e['target'])
            best = t if best is None else min(best, t)
        return best


DB = None


def db():
    global DB
    if DB is None:
        DB = SpeciesDB()
    return DB

# ---------------------------------------------------------------- themes

T = lambda *xs: ['TYPE_' + x.upper() for x in xs]  # noqa: E731

FISH = {'SPECIES_GOLDEEN', 'SPECIES_SEAKING', 'SPECIES_REMORAID', 'SPECIES_OCTILLERY', 'SPECIES_BARBOACH',
        'SPECIES_WHISCASH', 'SPECIES_CARVANHA', 'SPECIES_SHARPEDO', 'SPECIES_FINNEON', 'SPECIES_LUMINEON',
        'SPECIES_BASCULIN', 'SPECIES_ARROKUDA', 'SPECIES_BARRASKEWDA', 'SPECIES_QWILFISH', 'SPECIES_CHINCHOU',
        'SPECIES_LANTURN', 'SPECIES_HORSEA', 'SPECIES_SEADRA', 'SPECIES_KINGDRA', 'SPECIES_TENTACOOL',
        'SPECIES_TENTACRUEL', 'SPECIES_CORPHISH', 'SPECIES_CRAWDAUNT', 'SPECIES_SHELLDER', 'SPECIES_CLOYSTER',
        'SPECIES_STARYU', 'SPECIES_STARMIE', 'SPECIES_CLAUNCHER', 'SPECIES_CLAWITZER', 'SPECIES_LUVDISC',
        'SPECIES_RELICANTH', 'SPECIES_MANTINE', 'SPECIES_BRUXISH', 'SPECIES_KRABBY', 'SPECIES_KINGLER',
        'SPECIES_POLIWAG', 'SPECIES_POLIWHIRL', 'SPECIES_POLIWRATH', 'SPECIES_POLITOED', 'SPECIES_WIGLETT',
        'SPECIES_WUGTRIO', 'SPECIES_FINIZEN', 'SPECIES_WAILMER', 'SPECIES_WAILORD', 'SPECIES_GYARADOS',
        'SPECIES_VELUZA', 'SPECIES_ALOMOMOLA', 'SPECIES_CHEWTLE', 'SPECIES_DREDNAW', 'SPECIES_SKRELP',
        'SPECIES_DRAGALGE', 'SPECIES_MAREANIE', 'SPECIES_TOXAPEX', 'SPECIES_CORSOLA', 'SPECIES_LAPRAS',
        'SPECIES_TATSUGIRI', 'SPECIES_FRILLISH', 'SPECIES_JELLICENT', 'SPECIES_PSYDUCK', 'SPECIES_GOLDUCK',
        'SPECIES_SLOWPOKE', 'SPECIES_SLOWBRO', 'SPECIES_SEEL', 'SPECIES_DEWGONG', 'SPECIES_TYMPOLE',
        'SPECIES_PALPITOAD', 'SPECIES_SEISMITOAD', 'SPECIES_BINACLE', 'SPECIES_BARBARACLE', 'SPECIES_PYUKUMUKU',
        'SPECIES_DHELMISE', 'SPECIES_CLAMPERL', 'SPECIES_HUNTAIL', 'SPECIES_GOREBYSS', 'SPECIES_MILOTIC'}
SOUND = {'SPECIES_WHISMUR', 'SPECIES_LOUDRED', 'SPECIES_EXPLOUD', 'SPECIES_KRICKETUNE', 'SPECIES_NOIBAT',
         'SPECIES_NOIVERN', 'SPECIES_TOXEL', 'SPECIES_TOXTRICITY_AMPED', 'SPECIES_TOXTRICITY_LOW_KEY', 'SPECIES_JIGGLYPUFF', 'SPECIES_WIGGLYTUFF',
         'SPECIES_CHATOT', 'SPECIES_MELOETTA', 'SPECIES_JANGMO_O', 'SPECIES_ALTARIA', 'SPECIES_SWABLU',
         'SPECIES_PRIMARINA', 'SPECIES_BRIONNE', 'SPECIES_RILLABOOM', 'SPECIES_THWACKEY', 'SPECIES_KOMMO_O',
         'SPECIES_HAKAMO_O', 'SPECIES_GRIMMSNARL', 'SPECIES_MORGREM', 'SPECIES_IMPIDIMP'}

# class -> (types, extra species set or None, strong?)
CLASS_THEME = {
    'Bug Catcher': (T('bug'), None, False),
    'Bug Maniac': (T('bug'), None, False),
    'Fisherman': (T('water'), FISH, False),
    'Hiker': (T('rock', 'ground', 'fighting'), None, False),
    'Swimmer M': (T('water'), None, False),
    'Swimmer F': (T('water'), None, False),
    'Tuber M': (T('water'), None, False),
    'Tuber F': (T('water'), None, False),
    'Bird Keeper': (T('flying'), None, False),
    'Psychic': (T('psychic'), None, False),
    'Black Belt': (T('fighting'), None, False),
    'Battle Girl': (T('fighting'), None, False),
    'Hex Maniac': (T('ghost', 'dark'), None, False),
    'Beauty': (T('fairy', 'grass', 'normal'), None, False),
    'Lady': (T('fairy', 'grass', 'normal'), None, False),
    'Aroma Lady': (T('fairy', 'grass', 'normal'), None, False),
    'Parasol Lady': (T('water', 'fairy', 'normal'), None, False),
    'Kindler': (T('fire'), None, False),
    'Guitarist': (T('electric'), SOUND, False),
    'Dragon Tamer': (T('dragon'), None, True),
    'Ruin Maniac': (T('ground', 'rock', 'ghost'), None, False),
    'Ninja Boy': (T('poison', 'bug', 'dark'), None, False),
    'Sailor': (T('water', 'fighting'), None, False),
    'Expert': (T('fighting', 'psychic', 'steel', 'dragon', 'dark', 'normal'), None, True),
    'Cooltrainer': (None, None, True),
    'Cooltrainer 2': (None, None, True),
    'Winstrate': (None, None, True),
    'Team Magma': (T('fire', 'ground', 'dark', 'poison'), None, False),
    'Team Aqua': (T('water', 'dark', 'poison', 'flying'), None, False),
    'Youngster': (T('normal', 'bug', 'poison', 'flying', 'ground'), None, False),
    'Lass': (T('normal', 'fairy', 'grass'), None, False),
    'Camper': (T('ground', 'bug', 'normal', 'fire'), None, False),
    'Picnicker': (T('grass', 'fairy', 'normal', 'bug'), None, False),
    'School Kid': (T('normal', 'psychic', 'grass', 'electric'), None, False),
    'Pkmn Breeder': (T('normal', 'fairy', 'water', 'grass'), None, False),
    'Pkmn Ranger': (T('grass', 'bug', 'flying', 'normal', 'ground'), None, True),
    'Pokefan': (T('normal', 'fairy', 'electric'), None, False),
    'Gentleman': (T('normal', 'fire', 'electric', 'steel'), None, False),
    'Rich Boy': (T('normal', 'fairy', 'steel', 'fire'), None, False),
    'Pokemaniac': (T('dragon', 'rock', 'ground', 'normal'), None, False),
    'Collector': (T('normal', 'steel', 'fairy', 'bug', 'ice'), None, False),
    'Interviewer': (T('normal', 'electric', 'steel'), None, False),
    'Twins': (T('normal', 'fairy', 'psychic', 'electric'), None, False),
    'Young Couple': (T('normal', 'fairy', 'grass', 'psychic'), None, False),
    'Old Couple': (T('normal', 'water', 'grass', 'psychic'), None, False),
    'Sis And Bro': (T('water', 'normal', 'fire', 'electric'), None, False),
    'Sr And Jr': (T('fighting', 'normal', 'grass'), None, False),
}


def theme_for(entry):
    cls = header_get(entry['header'], 'Class') or ''
    pic = header_get(entry['header'], 'Pic') or ''
    if cls == 'Triathlete':
        if 'Swimming' in pic:
            return T('water'), None, False
        if 'Cycling' in pic:
            return T('electric', 'steel', 'normal'), None, False
        return T('normal', 'fighting', 'ground', 'flying'), None, False
    return CLASS_THEME.get(cls, (None, None, False))


def max_bst(level, strong):
    if level >= 44:
        return 600
    cap = 300 + 7 * level + (15 if strong else 0)
    return min(600, cap)


def min_bst(level):
    if level < 30:
        return 0
    return min(460, 380 + (level - 30) * 4)


def candidates(level, types, extra, strong, used_fams, relax=0):
    d = db()
    out = []
    lo, hi = (min_bst(level) if relax < 1 else 0), max_bst(level, strong)
    for root, f in d.families.items():
        if f['banned'] or root in used_fams:
            continue
        if root in d.PSEUDO_ROOTS and not strong:
            continue
        legal = [m for m in f['members'] if m in d.raw and m not in d.BANNED and d.min_level(m) <= level
                 and d.raw[m].get('formIndex', 0) == 0]
        if not legal:
            continue
        rank = lambda m: (f['stages'].get(m, 0), not d.raw[m].get('isBaby'))  # noqa: E731
        top = max(rank(m) for m in legal)
        best = [m for m in legal if rank(m) == top]
        best = [m for m in best if lo <= d.bst(m) <= hi]
        if relax < 2 and types is not None:
            best = [m for m in best if set(d.types[m]) & set(types) and (extra is None or m in extra)]
        best = [m for m in best if not d.raw[m].get('flags', {}).get('cannotBeTraded')]
        for m in best:
            out.append((root, m))
    return out


def pick_party(entry, levels, strong_override=None):
    types, extra, strong = theme_for(entry)
    cls = header_get(entry['header'], 'Class') or ''
    if strong_override is not None:
        strong = strong_override
    rng = random.Random(zlib.crc32(entry['id'].encode()))
    used, picks = set(), []
    for lvl in levels:
        strong_here = strong and lvl >= 35
        cands = []
        for relax in (0, 1, 2):
            cands = candidates(lvl, types, extra, strong_here, used, relax)
            if extra is not None and not cands and relax == 1:
                cands = candidates(lvl, types, None, strong_here, used, 1)
            if cands:
                break
        cands.sort()
        root, sp = rng.choice(cands)
        used.add(root)
        picks.append(sp)
    return picks

# ---------------------------------------------------------------- names -> constants


def _norm(s):
    return re.sub(r'[^A-Z0-9]', '', s.upper())


_NAME_MAPS = {}


def name_map(kind):
    if kind in _NAME_MAPS:
        return _NAME_MAPS[kind]
    m = {}
    if kind == 'species':
        for k, v in db().raw.items():
            m.setdefault(_norm(v['speciesName']), k)
            m[_norm(k[8:])] = k
    elif kind == 'move':
        for k, v in mdb.load_moves().items():
            m.setdefault(_norm(v['name']), k)
            m[_norm(k[5:])] = k
    elif kind == 'item':
        for k, v in mdb.load_items().items():
            m.setdefault(_norm(v['name']), k)
            m[_norm(k[5:])] = k
    _NAME_MAPS[kind] = m
    return m


def to_const(kind, name):
    prefix = {'species': 'SPECIES_', 'move': 'MOVE_', 'item': 'ITEM_'}[kind]
    name = name.strip()
    if name.startswith(prefix):
        return name
    return name_map(kind).get(_norm(name))


def ability_const(name):
    name = name.strip()
    if name.startswith('ABILITY_'):
        return name
    return 'ABILITY_' + re.sub(r'[^A-Z0-9]+', '_', name.upper().replace("'", '')).strip('_')


def capped_moveset(sp, level, seed, allow_tm, allow_egg=False, keep=()):
    """best_moveset, but below Lv 30 TM-only moves stronger than the level allows
    (power > 60 under Lv 20, > 75 under Lv 30; TM and egg moves) are swapped for level-up moves."""
    moves = list(keep)
    cand = mdb.best_moveset(sp, level, seed, allow_tm=allow_tm, allow_egg=allow_egg, normal_trainer=True)
    if (allow_tm or allow_egg) and level < 30:
        cap = 60 if level < 20 else 75
        natural = set(mdb.legal_moves(sp, level, include_tm=False, include_egg=False, include_prevo=True))
        mv = mdb.load_moves()
        cand = [m for m in cand if m in natural or (mv[m].get('power') or 0) <= cap]
        cand += mdb.best_moveset(sp, level, seed, allow_tm=False, allow_egg=False, normal_trainer=True)
    for m in cand:
        if len(moves) >= 4:
            break
        if m not in moves:
            moves.append(m)
    return moves


# ---------------------------------------------------------------- emit


def fmt_mon(sp, level, ivs, moves, item=None, nature=None, ability=None, evs=None):
    lines = [sp + (' @ ' + item if item else '')]
    lines.append('Level: %d' % level)
    if isinstance(ivs, int):
        lines.append('IVs: {0} HP / {0} Atk / {0} Def / {0} SpA / {0} SpD / {0} Spe'.format(ivs))
    elif ivs:
        lines.append('IVs: ' + ivs)
    if evs:
        lines.append('EVs: ' + evs)
    if ability:
        lines.append('Ability: ' + ability)
    if nature:
        lines.append('Nature: ' + nature)
    lines += ['- ' + mv for mv in moves]
    return lines


def emit(entry, header, mon_blocks):
    out = ['=== %s ===' % entry['id']] + header + ['']
    for b in mon_blocks:
        out += b + ['']
    return '\n'.join(out) + '\n'

# ---------------------------------------------------------------- classification


def referenced_trainers(root):
    refs = set()
    paths = []
    for base in ('data/maps', 'data/scripts'):
        for dp, _, fs in os.walk(os.path.join(root, base)):
            paths += [os.path.join(dp, f) for f in fs if f.endswith('.inc') or f.endswith('.pory')]
    paths += [os.path.join(root, 'src', 'battle_setup.c')]
    paths += [os.path.join(root, 'data', f) for f in os.listdir(os.path.join(root, 'data'))
              if f.endswith('.s') or f.endswith('.inc')]
    for p in paths:
        try:
            refs |= set(re.findall(r'TRAINER_[A-Z0-9_]+', open(p, errors='replace').read()))
        except OSError:
            pass
    return refs


def rematch_idx(tid):
    m = re.search(r'_(\d)$', tid)
    return int(m.group(1)) if m else 0

# ---------------------------------------------------------------- bosses


BOSS_IV31_CLASSES = {'Leader', 'Elite Four', 'Champion'}


def process_boss(entry, bentry, log):
    """Merge one hand-authored boss entry onto the vanilla header."""
    d = db()
    header = list(entry['header'])
    cls = header_get(header, 'Class') or ''
    for key in ('Items', 'AI', 'Battle Type', 'Double Battle'):
        v = header_get(bentry['header'], key)
        if v is not None:
            header = header_set(header, key, v if v.upper() != 'NONE' else None)
    seed0 = zlib.crc32(entry['id'].encode())
    blocks = []
    for i, b in enumerate(bentry['mons']):
        m = re.match(r'^(.*?)(?:\s+@\s+(.*))?$', b[0].strip())
        sp = to_const('species', m.group(1))
        if sp is None or sp not in d.raw:
            log.append('ERROR %s: unknown species %r' % (entry['id'], m.group(1)))
            continue
        item = to_const('item', m.group(2)) if m.group(2) else None
        if m.group(2) and not item:
            log.append('WARN %s: unknown item %r dropped' % (entry['id'], m.group(2)))
        fields, moves = {}, []
        for ln in b[1:]:
            if ln.startswith('- '):
                mv = to_const('move', ln[2:])
                if mv is None:
                    log.append('WARN %s: unknown move %r dropped' % (entry['id'], ln[2:]))
                else:
                    moves.append(mv)
            elif ':' in ln:
                k, v = ln.split(':', 1)
                fields[k.strip()] = v.strip()
        level = int(fields.get('Level', '50'))
        if d.min_level(sp) > level:
            log.append('WARN %s: %s at L%d below obtainable level %d' % (entry['id'], sp, level, d.min_level(sp)))
        legal = set(mdb.legal_moves(sp, level, include_tm=True, include_egg=True, include_tutor=True,
                                    include_prevo=True))
        kept = []
        for mv in moves:
            if mv in legal and mv not in kept:
                kept.append(mv)
            else:
                log.append('INFO %s: %s cannot learn %s at L%d -> auto-replaced' % (entry['id'], sp, mv, level))
        if len(kept) < 4:
            kept = capped_moveset(sp, level, seed0 + i, True, True, keep=kept)
        if 'IVs' in fields:
            ivs = fields['IVs']
        elif cls in BOSS_IV31_CLASSES or entry['id'] in ('TRAINER_STEVEN',) or entry['id'].startswith('TRAINER_WALLY_VR'):
            ivs = 31
        else:
            ivs = min(31, iv_for(level) + 6)
        ability = None
        if 'Ability' in fields:
            ability = ability_const(fields['Ability'])
            if ability not in d.raw[sp]['abilities']:
                log.append('WARN %s: %s cannot have %s, dropped' % (entry['id'], sp, ability))
                ability = None
        nature = fields.get('Nature')
        if nature and not nature.startswith('NATURE_'):
            nature = nature.capitalize()
        blocks.append(fmt_mon(sp, level, ivs, kept[:4], item, nature, ability, fields.get('EVs')))
    return emit(entry, header, blocks)

# ---------------------------------------------------------------- regular


def process_regular(entry):
    d = db()
    header = header_set(list(entry['header']), 'AI', 'Basic Trainer')
    cls = header_get(header, 'Class') or ''
    ridx = rematch_idx(entry['id'])
    van_levels = [mon_level(b) for b in entry['mons']]
    levels = [map_level(v) for v in van_levels]
    if not levels:
        levels = [5]
    _, _, strong = theme_for(entry)
    if is_double(header) and len(levels) < 2:
        levels.append(levels[-1])
    if strong and max(levels) >= 33 and len(levels) < 3:
        levels.insert(0, max(1, min(levels) - 1))
    species = pick_party(entry, levels)
    seed0 = zlib.crc32(entry['id'].encode())
    blocks = []
    for i, (sp, lvl) in enumerate(zip(species, levels)):
        allow_tm = strong or lvl >= 30
        moves = capped_moveset(sp, lvl, seed0 + i, allow_tm)
        blocks.append(fmt_mon(sp, lvl, iv_for(lvl, ridx), moves))
    return emit(entry, header, blocks)

# ---------------------------------------------------------------- main


def generate(root, bosses_path, log):
    text = open(os.path.join(root, 'src/data/trainers.party')).read()
    pre, entries = parse_party(text)
    btext = re.sub(r'/\*.*?\*/', '', open(bosses_path).read(), flags=re.S)
    _, bents = parse_party(btext)
    bosses = {b['id']: b for b in bents}
    refs = referenced_trainers(root)
    stats = {'boss': 0, 'regular': 0, 'kept': 0}
    out = [pre]
    for e in entries:
        cls = header_get(e['header'], 'Class') or ''
        if e['id'] in bosses:
            out.append(process_boss(e, bosses[e['id']], log))
            stats['boss'] += 1
        elif e['id'] == 'TRAINER_NONE' or e['id'] not in refs or not e['mons'] or cls in (
                'Salon Maiden', 'Pyramid King', 'Pike Queen', 'Palace Maven', 'Factory Head', 'Dome Ace',
                'Arena Tycoon', 'RS Protag'):
            out.append(e['raw'] if e['raw'].endswith('\n') else e['raw'] + '\n')
            stats['kept'] += 1
        else:
            if cls in ('Leader', 'Elite Four', 'Champion', 'Rival', 'Magma Leader', 'Aqua Leader',
                       'Magma Admin', 'Aqua Admin'):
                log.append('WARN %s: boss-class trainer without a bosses.party entry, generated' % e['id'])
            out.append(process_regular(e))
            stats['regular'] += 1
    for bid in bosses:
        if bid not in {e['id'] for e in entries}:
            log.append('ERROR boss %s not in trainers.party' % bid)
    return ''.join(x if x.endswith('\n') else x + '\n' for x in out), stats


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--root', default='/home/user/pex-orig')
    ap.add_argument('--bosses', default=os.path.join(HERE, 'bosses.party'))
    ap.add_argument('--out', required=True)
    ap.add_argument('-v', '--verbose', action='store_true')
    a = ap.parse_args(argv)
    log = []
    text, stats = generate(a.root, a.bosses, log)
    open(a.out, 'w').write(text)
    for ln in log:
        if a.verbose or not ln.startswith('INFO'):
            print(ln)
    print('bosses %(boss)d, regular %(regular)d, kept as-is %(kept)d' % stats,
          '| %d auto-replaced boss moves' % sum(1 for x in log if x.startswith('INFO')))
    return 1 if any(x.startswith('ERROR') for x in log) else 0


if __name__ == '__main__':
    sys.exit(main())
