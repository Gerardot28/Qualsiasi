#!/usr/bin/env python3
"""mdb -- helpers on top of the JSON databases written by movedb.py.

    import sys; sys.path.insert(0, '/home/user/Qualsiasi/tools/movedb')
    import mdb
    mdb.load_moves()['MOVE_FLAMETHROWER']['power']           # 90
    mdb.legal_moves('SPECIES_GEODUDE', 12)                     # level-up (<=12) + TM/HM + tutor
    mdb.best_moveset('SPECIES_GARCHOMP', 60, rng_seed=1)       # ['MOVE_DRAGON_CLAW', ...]

Data directory: $MOVEDB_DIR or /home/user/work (moves.json, learnsets.json,
items.json, tmhm.json, movedb_species.json).  Base stats / types come from
/home/user/work/species.json (speciesdb) when present, else movedb_species.json.

Run `python3 -I mdb.py --demo` to print sample movesets for 15 species, or
`python3 -I mdb.py SPECIES LEVEL [--seed N] [--tm] [--egg] [--explain]`.
Only the standard library is used.
"""

import hashlib
import json
import os
import random
import re
import sys

DATA_DIR = os.environ.get('MOVEDB_DIR', '/home/user/work')
_cache = {}


def _path(name, path=None):
    return path or os.path.join(DATA_DIR, name)


def _load(name, path=None):
    p = _path(name, path)
    if p not in _cache:
        with open(p, encoding='utf-8') as f:
            _cache[p] = json.load(f)
    return _cache[p]


def load_moves(path=None):
    """{MOVE_X: {name, type, power, accuracy, pp, priority, category, effect, ...}}"""
    return _load('moves.json', path)


def load_learnsets(path=None):
    """{SPECIES_X: {levelup: [[lvl, MOVE]], tm: [...], tutor: [...], egg: [...], ...}}"""
    return _load('learnsets.json', path)


def load_items(path=None):
    """{ITEM_X: {name, pocket, holdEffect, holdEffectParam, price, ...}}"""
    return _load('items.json', path)


def load_tmhm(path=None):
    """{'tm_hm': [{num, item, move, name, ...}], 'tutors': [...], 'meta': {...}}"""
    return _load('tmhm.json', path)


def load_species(path=None):
    """Normalised species data: {SPECIES_X: {name, types, hp, atk, def, spa, spd, spe}}.
    Prefers speciesdb's species.json; falls back to movedb_species.json."""
    key = ('species', path)
    if key in _cache:
        return _cache[key]
    out = {}
    fallback = os.path.join(DATA_DIR, 'movedb_species.json')
    if os.path.exists(fallback):
        for sp, d in _load('movedb_species.json').items():
            out[sp] = {'name': d.get('name'), 'types': d.get('types') or [],
                       'hp': d.get('baseHP', 0), 'atk': d.get('baseAttack', 0),
                       'def': d.get('baseDefense', 0), 'spa': d.get('baseSpAttack', 0),
                       'spd': d.get('baseSpDefense', 0), 'spe': d.get('baseSpeed', 0),
                       'source': 'movedb_species.json'}
    sp_path = path or os.path.join(DATA_DIR, 'species.json')
    if os.path.exists(sp_path):
        try:
            with open(sp_path, encoding='utf-8') as f:
                raw = json.load(f)
            table = raw.get('species', raw) if isinstance(raw, dict) else raw
            if isinstance(table, list):
                table = {d.get('constant') or d.get('species'): d for d in table if isinstance(d, dict)}
            for sp, d in table.items():
                if not isinstance(d, dict) or not str(sp).startswith('SPECIES_'):
                    continue
                bs = d.get('baseStats') or {}
                def g(*keys):
                    for k in keys:
                        if k in bs:
                            return bs[k]
                        if k in d:
                            return d[k]
                    return None
                ent = {
                    'name': d.get('speciesName') or d.get('name'),
                    'types': d.get('types') or [],
                    'hp': g('hp', 'baseHP'), 'atk': g('atk', 'attack', 'baseAttack'),
                    'def': g('def', 'defense', 'baseDefense'),
                    'spa': g('spa', 'spAttack', 'baseSpAttack'),
                    'spd': g('spd', 'spDefense', 'baseSpDefense'),
                    'spe': g('spe', 'speed', 'baseSpeed'), 'source': 'species.json',
                }
                if ent['atk'] is None or ent['spa'] is None:
                    continue
                base = out.get(sp, {})
                if base.get('types') and (not ent['types'] or
                                          not all(isinstance(t, str) for t in ent['types'])):
                    ent['types'] = base['types']      # species.json had an unresolved type

                base.update({k: v for k, v in ent.items() if v is not None})
                out[sp] = base
        except (OSError, ValueError, AttributeError):
            pass
    if not out:
        raise FileNotFoundError('no species data: run movedb.py (writes movedb_species.json)')
    _cache[key] = out
    return out


def norm_species(species):
    s = str(species).strip()
    if not s.upper().startswith('SPECIES_'):
        s = 'SPECIES_' + re.sub(r'[^A-Z0-9]+', '_', s.upper()).strip('_')
    return s.upper()


def legal_moves(species, level, include_tm=True, include_egg=False, include_tutor=None,
                include_prevo=False):
    """Moves `species` can legally know at `level`.

    level-up moves with learn level <= level (level 0 = learned on evolution),
    + TM/HM moves (include_tm) + tutor moves (include_tutor, defaults to include_tm)
    + egg moves (include_egg; evolved species use their family's egg moves)
    + pre-evolution level-up moves <= level (include_prevo).
    Returns a de-duplicated list of MOVE_ constants (level-up first)."""
    ls = load_learnsets()
    sp = norm_species(species)
    if sp not in ls:
        raise KeyError('unknown species %s' % species)
    if include_tutor is None:
        include_tutor = include_tm
    d = ls[sp]
    out = []
    seen = set()

    def add(m):
        if m not in seen:
            seen.add(m)
            out.append(m)

    for lvl, m in d['levelup']:
        if lvl <= level:
            add(m)
    if include_prevo:
        cur, hops = d.get('prevo'), 0
        while cur and cur in ls and hops < 4:
            for lvl, m in ls[cur]['levelup']:
                if lvl <= level:
                    add(m)
            cur, hops = ls[cur].get('prevo'), hops + 1
    if include_tm:
        for m in d['tm']:
            add(m)
    if include_tutor:
        for m in d['tutor']:
            add(m)
    if include_egg:
        for m in (d['egg'] or d.get('egg_inherited', [])):
            add(m)
    return out


def held_items(hold_effect=None, pocket=None):
    """Items with a hold effect (optionally filtered by HOLD_EFFECT_* / POCKET_*)."""
    out = {}
    for k, v in load_items().items():
        if v['holdEffect'] in (None, 'HOLD_EFFECT_NONE'):
            continue
        if hold_effect and v['holdEffect'] != hold_effect:
            continue
        if pocket and v['pocket'] != pocket:
            continue
        out[k] = v
    return out


# ---------------------------------------------------------------------------
# Moveset heuristics
# ---------------------------------------------------------------------------

PHYS, SPEC, STATUS = 'DAMAGE_CATEGORY_PHYSICAL', 'DAMAGE_CATEGORY_SPECIAL', 'DAMAGE_CATEGORY_STATUS'

# Nominal power for moves whose listed power is 1 / variable.  'level' = level-based
# fixed damage, 'fixed' = argument.fixedDamage.  0 = never pick as an attack.
VARIABLE_POWER = {
    'EFFECT_LOW_KICK': 60, 'EFFECT_HEAT_CRASH': 60, 'EFFECT_GYRO_BALL': 'gyro',
    'EFFECT_ELECTRO_BALL': 'electro', 'EFFECT_MAGNITUDE': 70, 'EFFECT_FLAIL': 30,
    'EFFECT_RETURN': 50, 'EFFECT_FRUSTRATION': 50, 'EFFECT_TRUMP_CARD': 50,
    'EFFECT_PUNISHMENT': 60, 'EFFECT_FIXED_PERCENT_DAMAGE': 50,
    'EFFECT_POWER_BASED_ON_TARGET_HP': 80, 'EFFECT_LEVEL_DAMAGE': 'level',
    'EFFECT_PSYWAVE': 'level', 'EFFECT_FIXED_HP_DAMAGE': 'fixed',
    'EFFECT_REFLECT_DAMAGE': 'reflect',     # Counter / Mirror Coat / Metal Burst
}
# Damaging effects that are never picked (situational / self-KO / random / need setup).
EXCLUDED_DAMAGE_EFFECTS = {
    'EFFECT_OHKO', 'EFFECT_DREAM_EATER', 'EFFECT_SNORE', 'EFFECT_BIDE', 'EFFECT_STRUGGLE',
    'EFFECT_PRESENT', 'EFFECT_SPIT_UP', 'EFFECT_NATURAL_GIFT', 'EFFECT_FLING',
    'EFFECT_LAST_RESORT', 'EFFECT_SYNCHRONOISE', 'EFFECT_BELCH', 'EFFECT_ENDEAVOR',
    'EFFECT_BEAT_UP', 'EFFECT_SHELL_TRAP', 'EFFECT_SKY_DROP',
    'EFFECT_STEEL_ROLLER', 'EFFECT_HYPERSPACE_FURY', 'EFFECT_FINAL_GAMBIT', 'EFFECT_MAX_MOVE',
    'EFFECT_STORED_POWER', 'EFFECT_DYNAMAX_DOUBLE_DMG', 'EFFECT_PLACEHOLDER',
}
# Multipliers for awkward-but-usable damaging effects.
EFFECT_FACTOR = {
    'EFFECT_FIRST_TURN_ONLY': 0.5, 'EFFECT_UPPER_HAND': 0.4, 'EFFECT_FOCUS_PUNCH': 0.4,
    'EFFECT_FUTURE_SIGHT': 0.55, 'EFFECT_TWO_TURNS_ATTACK': 0.5, 'EFFECT_SOLAR_BEAM': 0.55,
    'EFFECT_SEMI_INVULNERABLE': 0.8, 'EFFECT_ROLLOUT': 0.7, 'EFFECT_FURY_CUTTER': 0.8,
    'EFFECT_MAX_HP_50_RECOIL': 0.5, 'EFFECT_SUCKER_PUNCH': 0.85, 'EFFECT_POLTERGEIST': 0.85,
    'EFFECT_FALSE_SWIPE': 0.5, 'EFFECT_HIT_SWITCH_TARGET': 0.9, 'EFFECT_FAIL_IF_NOT_ARG_TYPE': 0.8,
    'EFFECT_RETURN': 0.8, 'EFFECT_FRUSTRATION': 0.8, 'EFFECT_FLAIL': 0.8,
    'EFFECT_RECOIL_IF_MISS': 0.9, 'EFFECT_PURSUIT': 0.95, 'EFFECT_REFLECT_DAMAGE': 0.6,
}
# Status moves that are pointless for a trainer's single-battle moveset.
USELESS_STATUS_EFFECTS = {
    'EFFECT_DO_NOTHING', 'EFFECT_CELEBRATE', 'EFFECT_HOLD_HANDS', 'EFFECT_HAPPY_HOUR',
    'EFFECT_TELEPORT', 'EFFECT_HELPING_HAND', 'EFFECT_ALLY_SWITCH', 'EFFECT_AFTER_YOU',
    'EFFECT_FOLLOW_ME', 'EFFECT_QUASH', 'EFFECT_INSTRUCT', 'EFFECT_SKETCH', 'EFFECT_MIMIC',
    'EFFECT_COPYCAT', 'EFFECT_MIRROR_MOVE', 'EFFECT_ME_FIRST', 'EFFECT_ASSIST',
    'EFFECT_SLEEP_TALK', 'EFFECT_SNATCH', 'EFFECT_IMPRISON', 'EFFECT_GRUDGE',
    'EFFECT_HEAL_PULSE', 'EFFECT_BESTOW', 'EFFECT_RECYCLE', 'EFFECT_HEALING_WISH',
    'EFFECT_LUNAR_DANCE', 'EFFECT_MEMENTO', 'EFFECT_REVIVAL_BLESSING', 'EFFECT_COURT_CHANGE',
    'EFFECT_FLOWER_SHIELD', 'EFFECT_ROTOTILLER', 'EFFECT_STAT_CHANGE_MAGNETIC',
    'EFFECT_CAMOUFLAGE', 'EFFECT_CONVERSION', 'EFFECT_CONVERSION_2', 'EFFECT_TELEKINESIS',
    'EFFECT_POWDER', 'EFFECT_ELECTRIFY', 'EFFECT_ION_DELUGE', 'EFFECT_LUCKY_CHANT',
    'EFFECT_MUD_SPORT', 'EFFECT_WATER_SPORT', 'EFFECT_FAIRY_LOCK', 'EFFECT_MAGIC_ROOM',
    'EFFECT_WONDER_ROOM', 'EFFECT_GRAVITY', 'EFFECT_TEATIME', 'EFFECT_STUFF_CHEEKS',
    'EFFECT_LOCK_ON', 'EFFECT_FORESIGHT', 'EFFECT_MIRACLE_EYE', 'EFFECT_EMBARGO',
    'EFFECT_PSYCH_UP', 'EFFECT_ENDURE', 'EFFECT_SWALLOW', 'EFFECT_DOODLE', 'EFFECT_ENTRAINMENT',
    'EFFECT_ROLE_PLAY', 'EFFECT_REFLECT_TYPE', 'EFFECT_SPEED_SWAP', 'EFFECT_POWER_TRICK',
    'EFFECT_HEART_SWAP', 'EFFECT_GUARD_SWAP', 'EFFECT_POWER_SWAP', 'EFFECT_DRAGON_CHEER',
    'EFFECT_PURIFY', 'EFFECT_CAPTIVATE', 'EFFECT_NIGHTMARE', 'EFFECT_BATON_PASS',
    'EFFECT_METRONOME', 'EFFECT_NATURE_POWER', 'EFFECT_BELLY_DRUM', 'EFFECT_DESTINY_BOND',
    'EFFECT_PERISH_SONG', 'EFFECT_SHED_TAIL', 'EFFECT_SPLASH', 'EFFECT_MAGNET_RISE',
    'EFFECT_TRICK', 'EFFECT_MAT_BLOCK', 'EFFECT_STAT_CHANGE_HALF_HP',
}
USELESS_MOVES = {'MOVE_STRUGGLE', 'MOVE_SPLASH', 'MOVE_CELEBRATE', 'MOVE_HOLD_HANDS',
                 'MOVE_TRANSFORM', 'MOVE_WIDE_GUARD', 'MOVE_QUICK_GUARD', 'MOVE_CRAFTY_SHIELD',
                 'MOVE_MAX_GUARD', 'MOVE_SWEET_SCENT', 'MOVE_SPOTLIGHT'}

# Good status / setup moves: MOVE -> (affinity, base score).  affinity 'phys' / 'spec'
# moves are only picked when the species leans that way and has an attack of that category.
GOOD_STATUS = {
    'MOVE_SWORDS_DANCE': ('phys', 10), 'MOVE_DRAGON_DANCE': ('phys', 10),
    'MOVE_VICTORY_DANCE': ('phys', 9), 'MOVE_SHIFT_GEAR': ('phys', 8), 'MOVE_BULK_UP': ('phys', 8),
    'MOVE_TIDY_UP': ('phys', 8), 'MOVE_COIL': ('phys', 7), 'MOVE_HONE_CLAWS': ('phys', 6),
    'MOVE_CURSE': ('phys', 6), 'MOVE_HOWL': ('phys', 5),
    'MOVE_NASTY_PLOT': ('spec', 10), 'MOVE_QUIVER_DANCE': ('spec', 10), 'MOVE_TAIL_GLOW': ('spec', 9),
    'MOVE_CALM_MIND': ('spec', 9), 'MOVE_TAKE_HEART': ('spec', 8),
    'MOVE_SHELL_SMASH': ('any', 9), 'MOVE_GROWTH': ('any', 5), 'MOVE_WORK_UP': ('any', 5),
    'MOVE_ROCK_POLISH': ('any', 5), 'MOVE_AGILITY': ('any', 4), 'MOVE_AUTOTOMIZE': ('any', 4),
    'MOVE_SPORE': ('any', 9), 'MOVE_WILL_O_WISP': ('any', 8), 'MOVE_THUNDER_WAVE': ('any', 8),
    'MOVE_TOXIC': ('any', 8), 'MOVE_GLARE': ('any', 7), 'MOVE_SLEEP_POWDER': ('any', 7),
    'MOVE_YAWN': ('any', 6), 'MOVE_STUN_SPORE': ('any', 6), 'MOVE_LEECH_SEED': ('any', 6),
    'MOVE_HYPNOSIS': ('any', 5), 'MOVE_CONFUSE_RAY': ('any', 5), 'MOVE_POISON_POWDER': ('any', 4),
    'MOVE_PROTECT': ('any', 6), 'MOVE_DETECT': ('any', 6), 'MOVE_KINGS_SHIELD': ('any', 7),
    'MOVE_SPIKY_SHIELD': ('any', 7), 'MOVE_BANEFUL_BUNKER': ('any', 7), 'MOVE_SILK_TRAP': ('any', 7),
    'MOVE_BURNING_BULWARK': ('any', 7),
    'MOVE_STEALTH_ROCK': ('any', 7), 'MOVE_SPIKES': ('any', 6), 'MOVE_STICKY_WEB': ('any', 6),
    'MOVE_TOXIC_SPIKES': ('any', 5),
    'MOVE_RECOVER': ('any', 7), 'MOVE_ROOST': ('any', 7), 'MOVE_SOFT_BOILED': ('any', 7),
    'MOVE_SLACK_OFF': ('any', 7), 'MOVE_MILK_DRINK': ('any', 7), 'MOVE_SHORE_UP': ('any', 7),
    'MOVE_HEAL_ORDER': ('any', 7), 'MOVE_STRENGTH_SAP': ('any', 7), 'MOVE_SYNTHESIS': ('any', 6),
    'MOVE_MOONLIGHT': ('any', 6), 'MOVE_MORNING_SUN': ('any', 6),
    'MOVE_REFLECT': ('any', 5), 'MOVE_LIGHT_SCREEN': ('any', 5), 'MOVE_TAUNT': ('any', 5),
    'MOVE_ENCORE': ('any', 5), 'MOVE_SUBSTITUTE': ('any', 4), 'MOVE_COSMIC_POWER': ('any', 5),
    'MOVE_IRON_DEFENSE': ('any', 4), 'MOVE_AMNESIA': ('any', 4),
}


TYPICAL_FOE_SPEED = 70


def _level_equiv_power(level):
    # power of a neutral move doing ~`level` damage at equal Atk/Def, capped
    return max(30.0, min(80.0, (level - 2) * 50.0 / (2.0 * level / 5 + 2)))


def _self_drop_factor(m):
    """Penalty for attacks that lower the user's stats (Overheat, Close Combat...)."""
    f = 1.0
    for e in m.get('additionalEffects', []):
        if e.get('self') and 'MINUS' in str(e.get('moveEffect')):
            off = sum(e.get(k, 0) for k in ('attack', 'spAtk'))
            dfn = sum(e.get(k, 0) for k in ('defense', 'spDef', 'speed'))
            f *= (0.85 ** off) * (0.95 ** dfn)
    return f


def score_damaging(move, m, sp, level, normal_trainer=True):
    """Estimated usefulness of a damaging move for species data `sp`.
    Returns (score, stab_score_type) or (0, None) if it should not be used."""
    if m['category'] == STATUS:
        return 0.0
    eff = m['effect']
    if move in USELESS_MOVES or eff in EXCLUDED_DAMAGE_EFFECTS:
        return 0.0
    power = m['power']
    stat_free = False
    if eff in VARIABLE_POWER or power <= 1:
        nominal = VARIABLE_POWER.get(eff)
        if nominal == 'level':
            power, stat_free = _level_equiv_power(level), True
        elif nominal == 'fixed':
            dmg = (m.get('argument') or {}).get('fixedDamage') or 0
            power = max(0.0, min(80.0, (dmg - 2) * 50.0 / (2.0 * level / 5 + 2))) if dmg else 0
            stat_free = True
        elif nominal == 'reflect':
            power, stat_free = 60.0, True
        elif nominal in ('gyro', 'electro'):
            spe = max(1, sp.get('spe') or 1)
            if nominal == 'gyro':            # 25 * foe speed / user speed (foe ~70)
                power = max(1.0, min(150.0, 25.0 * TYPICAL_FOE_SPEED / spe))
            else:
                r = spe / float(TYPICAL_FOE_SPEED)
                power = 150 if r >= 4 else 120 if r >= 3 else 80 if r >= 2 else 60 if r >= 1 else 40
        elif nominal:
            power = nominal
        else:
            return 0.0
    if power <= 0:
        return 0.0
    if normal_trainer and (m.get('recharge') or m.get('selfKO')):
        return 0.0
    hits = 1.0
    if m.get('multiHit'):
        hits = 3.0
    elif m.get('strikeCount', 1) > 1:
        hits = float(m['strikeCount'])
        if eff == 'EFFECT_POPULATION_BOMB':
            hits = 6.0
    score = float(power) * hits
    if not stat_free:
        if move == 'MOVE_BODY_PRESS':
            stat = sp.get('def') or 0
        else:
            stat = (sp.get('atk') or 0) if m['category'] == PHYS else (sp.get('spa') or 0)
        best = max(sp.get('atk') or 1, sp.get('spa') or 1)
        score *= max(0.25, float(stat) / best) ** 1.5
        if m['type'] in sp.get('types', []):
            score *= 1.5
    acc = m['accuracy']
    if acc:
        score *= acc / 100.0
        if acc < 85:
            score *= 0.85
    score *= EFFECT_FACTOR.get(eff, 1.0)
    if m.get('recoil'):
        score *= 1.0 - m['recoil'] / 200.0
    if m.get('recharge') or m.get('selfKO'):
        score *= 0.5
    if any(e.get('moveEffect') == 'MOVE_EFFECT_THRASH' for e in m.get('additionalEffects', [])):
        score *= 0.9
    if 'cantUseTwice' in m.get('flags', []):
        score *= 0.8
    score *= _self_drop_factor(m)
    if m.get('priority', 0) > 0 and eff not in EFFECT_FACTOR:
        score *= 1.05
    if m.get('secondary') and not m.get('recharge'):
        score *= 1.03
    return score


def _leaning(sp):
    atk, spa = sp.get('atk') or 0, sp.get('spa') or 0
    if atk >= spa * 1.1:
        return 'phys'
    if spa >= atk * 1.1:
        return 'spec'
    return 'mixed'


def _rng_for(*parts):
    h = hashlib.sha256('|'.join(str(p) for p in parts).encode()).digest()
    return random.Random(int.from_bytes(h[:8], 'big'))


def best_moveset(species, level, rng_seed=0, allow_tm=False, allow_egg=False,
                 allow_tutor=None, normal_trainer=True, explain=False):
    """Pick a sensible <=4-move set for a trainer's `species` at `level`.

    Strategy: best STAB damaging move for each of the species' types (scored by
    power x hits x STAB x accuracy, weighted toward the higher of Atk / Sp. Atk),
    then the best coverage damaging move of another type, then at most one good
    status / setup move (GOOD_STATUS, matched to the species' attacking side),
    then fill: new-type attacks > one minor status move > same-type attacks.
    Avoids useless moves (Splash, ...), accuracy < 85 where possible and, when
    normal_trainer=True, recharge (Hyper Beam) and self-KO (Explosion) moves.
    Deterministic for a given (species, level, rng_seed, options): a seeded
    +-8% jitter only breaks near-ties.

    Returns a list of MOVE_ constants (damaging moves first), or
    (moves, notes) when explain=True."""
    moves = load_moves()
    sp_name = norm_species(species)
    sp = load_species().get(sp_name)
    if sp is None:
        raise KeyError('no species data for %s' % species)
    if allow_tutor is None:
        allow_tutor = allow_tm
    rng = _rng_for(rng_seed, sp_name, level, allow_tm, allow_egg, allow_tutor, normal_trainer)
    pool = [m for m in legal_moves(sp_name, level, include_tm=allow_tm, include_egg=allow_egg,
                                   include_tutor=allow_tutor) if m in moves]
    types = []
    for t in sp.get('types', []):
        if t not in types:
            types.append(t)
    lean = _leaning(sp)
    notes = []

    dmg = {}
    for mv in pool:
        s = score_damaging(mv, moves[mv], sp, level, normal_trainer)
        if s > 0:
            dmg[mv] = s * rng.uniform(0.92, 1.08)
    chosen = []

    def mtype(mv):
        return moves[mv]['type']

    def chosen_types():
        return {mtype(x) for x in chosen if moves[x]['category'] != STATUS}

    def best_of(cands):
        cands = sorted(cands, key=lambda x: (-dmg[x], x))
        return cands[0] if cands else None

    # 1. STAB per type
    for t in types:
        mv = best_of([x for x in dmg if mtype(x) == t and x not in chosen])
        if mv:
            chosen.append(mv)
            notes.append('STAB %s: %s (%.0f)' % (t[5:], mv, dmg[mv]))
    # 2. coverage (must be worth >= 35% of the best STAB attack, else left to the fill step)
    top = max([dmg[x] for x in chosen] or [0.0])
    mv = best_of([x for x in dmg if x not in chosen and mtype(x) not in chosen_types()
                  and mtype(x) not in types and dmg[x] >= 0.35 * top])
    if mv:
        chosen.append(mv)
        notes.append('coverage %s: %s (%.0f)' % (mtype(mv)[5:], mv, dmg[mv]))

    # 3. one good status / setup move
    def status_ok(mv):
        m = moves[mv]
        if m['category'] != STATUS or mv not in GOOD_STATUS:
            return 0.0
        aff, base = GOOD_STATUS[mv]
        cats = {moves[x]['category'] for x in chosen if moves[x]['category'] != STATUS}
        if aff == 'phys' and (lean == 'spec' or PHYS not in cats):
            return 0.0
        if aff == 'spec' and (lean == 'phys' or SPEC not in cats):
            return 0.0
        if mv == 'MOVE_CURSE' and 'TYPE_GHOST' in types:
            return 0.0
        s = float(base)
        if m['accuracy'] and m['accuracy'] < 85:
            s *= 0.6
        return s * rng.uniform(0.9, 1.1)

    if len(chosen) < 4:
        st = sorted(((status_ok(x), x) for x in pool if x not in chosen), key=lambda t: (-t[0], t[1]))
        if st and st[0][0] > 0:
            chosen.append(st[0][1])
            notes.append('status: %s' % st[0][1])

    # 4. fill
    def minor_status():
        out = []
        for x in pool:
            m = moves[x]
            if x in chosen or m['category'] != STATUS or x in USELESS_MOVES:
                continue
            if m['effect'] in USELESS_STATUS_EFFECTS or x in GOOD_STATUS:
                continue
            if m['accuracy'] and m['accuracy'] < 60:
                continue
            out.append(x)
        return out

    filled_minor = False
    top = max([dmg[x] for x in chosen if x in dmg] or [0.0])
    while len(chosen) < 4:
        # a new-type attack must be worth >= 35% of our best attack to beat minor options
        mv = best_of([x for x in dmg if x not in chosen and mtype(x) not in chosen_types()
                      and dmg[x] >= 0.35 * top])
        if mv:
            chosen.append(mv)
            notes.append('fill (new type): %s' % mv)
            continue
        if not filled_minor:
            filled_minor = True
            ms = minor_status()
            if ms:
                mv = sorted(ms)[rng.randrange(len(ms))]
                chosen.append(mv)
                notes.append('fill (minor status): %s' % mv)
                continue
        mv = best_of([x for x in dmg if x not in chosen])
        if mv:
            chosen.append(mv)
            notes.append('fill (same type): %s' % mv)
            continue
        ms = minor_status()
        if ms:
            mv = sorted(ms)[rng.randrange(len(ms))]
            chosen.append(mv)
            notes.append('fill (minor status): %s' % mv)
            continue
        mv = best_of([x for x in dmg if x not in chosen])
        if mv:
            chosen.append(mv)
            notes.append('fill (weak attack): %s' % mv)
            continue
        break
    if not chosen and pool:
        # nothing sensible (e.g. Magikarp before Tackle): fall back to the latest moves learned
        chosen = pool[-4:]
        notes.append('fallback: last level-up moves')
    dmg_first = [x for x in chosen if moves[x]['category'] != STATUS]
    dmg_first.sort(key=lambda x: -dmg.get(x, 0))
    result = dmg_first + [x for x in chosen if moves[x]['category'] == STATUS]
    if explain:
        return result, notes
    return result


def describe(move):
    m = load_moves()[move]
    cat = {PHYS: 'Phys', SPEC: 'Spec', STATUS: 'Stat'}.get(m['category'], '?')
    return '%s (%s %s %s/%s)' % (m['name'], m['type'][5:].title() if m['type'] else '?', cat,
                                 m['power'] or '-', m['accuracy'] or '-')


DEMO = [('SPECIES_GEODUDE', 12), ('SPECIES_MACHOP', 18), ('SPECIES_MAGNEMITE', 22),
        ('SPECIES_GYARADOS', 40), ('SPECIES_GARCHOMP', 60), ('SPECIES_GHOLDENGO', 55),
        ('SPECIES_SPRIGATITO', 8), ('SPECIES_PIKACHU', 25), ('SPECIES_CHARIZARD', 50),
        ('SPECIES_MAGIKARP', 10), ('SPECIES_GENGAR', 45), ('SPECIES_DRAGAPULT', 65),
        ('SPECIES_LUCARIO', 40), ('SPECIES_SCIZOR', 45), ('SPECIES_ALAKAZAM', 50)]


def _main(argv):
    import argparse
    ap = argparse.ArgumentParser(description='moveset helper (see module docstring)')
    ap.add_argument('species', nargs='?')
    ap.add_argument('level', nargs='?', type=int)
    ap.add_argument('--seed', type=int, default=0)
    ap.add_argument('--tm', action='store_true', help='allow TM/HM (and tutor) moves')
    ap.add_argument('--egg', action='store_true', help='allow egg moves')
    ap.add_argument('--boss', action='store_true', help='allow recharge / self-KO moves')
    ap.add_argument('--explain', action='store_true')
    ap.add_argument('--demo', action='store_true', help='run the 15-species demo')
    a = ap.parse_args(argv)
    cases = DEMO if a.demo or not a.species else [(a.species, a.level or 50)]
    for sp, lvl in cases:
        for tm in ([False, True] if a.demo else [a.tm]):
            res, notes = best_moveset(sp, lvl, a.seed, allow_tm=tm, allow_egg=a.egg,
                                      normal_trainer=not a.boss, explain=True)
            print('%-20s L%-3d %-6s %s' % (norm_species(sp)[8:], lvl, 'TM' if tm else 'lvlup',
                                            ' | '.join(describe(x) for x in res)))
            if a.explain:
                for n in notes:
                    print('      ', n)


if __name__ == '__main__':
    _main(sys.argv[1:])
