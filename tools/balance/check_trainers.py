#!/usr/bin/env python3
"""Validates a generated trainers.party against the vanilla file and the data DBs.

    python3 -I tools/balance/check_trainers.py /path/trainers.party [--root /home/user/pex-orig]

Checks: species / moves / items / abilities / natures are valid constants,
levels 1-100, no species below its obtainable level (stage vs level), no
legendary/mythical/UB/paradox in regular trainers, every vanilla trainer kept
with identical Name/Class/Pic/Gender/Music/Double Battle, party sizes >=
vanilla (doubles >= 2), boss aces equal their level caps, moves legal.
Exit status 1 on any ERROR.
"""
import argparse, os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import trainer_gen as tg  # noqa: E402
mdb = tg.mdb

NATURES = {'HARDY', 'LONELY', 'BRAVE', 'ADAMANT', 'NAUGHTY', 'BOLD', 'DOCILE', 'RELAXED', 'IMPISH', 'LAX',
           'TIMID', 'HASTY', 'SERIOUS', 'JOLLY', 'NAIVE', 'MODEST', 'MILD', 'QUIET', 'BASHFUL', 'RASH',
           'CALM', 'GENTLE', 'SASSY', 'CAREFUL', 'QUIRKY'}
ACE_CAPS = {'TRAINER_ROXANNE_1': 15, 'TRAINER_BRAWLY_1': 19, 'TRAINER_WATTSON_1': 24, 'TRAINER_FLANNERY_1': 29,
            'TRAINER_NORMAN_1': 33, 'TRAINER_WINONA_1': 37, 'TRAINER_TATE_AND_LIZA_1': 44, 'TRAINER_JUAN_1': 49,
            'TRAINER_SIDNEY': 54, 'TRAINER_PHOEBE': 56, 'TRAINER_GLACIA': 58, 'TRAINER_DRAKE': 60,
            'TRAINER_WALLACE': 62}
HEADER_KEYS = ('Name', 'Class', 'Pic', 'Gender', 'Music', 'Double Battle', 'Battle Type', 'Mugshot', 'Multi Party')
AI_OK = {'AI_FLAG_' + x for x in (
    'CHECK_BAD_MOVE TRY_TO_FAINT CHECK_VIABILITY FORCE_SETUP_FIRST_TURN RISKY TRY_TO_2HKO PREFER_BATON_PASS '
    'DOUBLE_BATTLE HP_AWARE POWERFUL_STATUS NEGATE_UNAWARE WILL_SUICIDE PREFER_STATUS_MOVES STALL SMART_SWITCHING '
    'ACE_POKEMON OMNISCIENT SMART_MON_CHOICES CONSERVATIVE SEQUENCE_SWITCHING DOUBLE_ACE_POKEMON BASIC_TRAINER '
    'SMART_TRAINER PREDICTION ASSUMPTIONS').split()}


def ai_const(s):
    return 'AI_FLAG_' + re.sub(r'[^A-Z0-9]', '_', s.strip().upper().replace("'", ''))


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument('party')
    ap.add_argument('--root', default='/home/user/pex-orig')
    a = ap.parse_args(argv)
    d = tg.db()
    moves, items = mdb.load_moves(), mdb.load_items()
    _, van = tg.parse_party(open(os.path.join(a.root, 'src/data/trainers.party')).read())
    _, new = tg.parse_party(open(a.party).read())
    vmap, nmap = {e['id']: e for e in van}, {e['id']: e for e in new}
    errs, warns = [], []
    E = errs.append
    W = warns.append
    if [e['id'] for e in van] != [e['id'] for e in new]:
        E('trainer list/order differs from vanilla (%d vs %d)' % (len(van), len(new)))
    nmons = 0
    for tid, e in nmap.items():
        v = vmap.get(tid)
        if v is None:
            E('%s: not in vanilla' % tid)
            continue
        for k in HEADER_KEYS:
            if tg.header_get(e['header'], k) != tg.header_get(v['header'], k):
                E('%s: header %s changed' % (tid, k))
        if e['raw'] == v['raw']:
            continue  # untouched entry (frontier, unused, placeholders)
        cls = tg.header_get(e['header'], 'Class') or ''
        boss = cls in ('Leader', 'Elite Four', 'Champion', 'Rival', 'Magma Leader', 'Aqua Leader', 'Magma Admin',
                       'Aqua Admin') or tid == 'TRAINER_STEVEN'
        ai = tg.header_get(e['header'], 'AI') or ''
        for f in ai.split('/'):
            if f.strip() and ai_const(f) not in AI_OK:
                E('%s: unknown AI flag %r' % (tid, f))
        it = tg.header_get(e['header'], 'Items')
        if it:
            if len(it.split('/')) > 4:
                E('%s: more than 4 trainer items' % tid)
            for x in it.split('/'):
                if tg.to_const('item', x) not in items:
                    E('%s: bad trainer item %r' % (tid, x))
        if len(e['mons']) < len(v['mons']):
            E('%s: party %d < vanilla %d' % (tid, len(e['mons']), len(v['mons'])))
        if tg.is_double(e['header']) and len(e['mons']) < 2:
            E('%s: double battle with < 2 mons' % tid)
        if len(e['mons']) > 6:
            E('%s: party > 6' % tid)
        lv = []
        for b in e['mons']:
            nmons += 1
            m = re.match(r'^(.*?)(?:\s+@\s+(.*))?$', b[0].strip())
            sp = tg.to_const('species', m.group(1))
            if sp not in d.raw:
                E('%s: bad species %r' % (tid, m.group(1)))
                continue
            if m.group(2) and tg.to_const('item', m.group(2)) not in items:
                E('%s: bad held item %r' % (tid, m.group(2)))
            level = 100
            mvs = []
            for ln in b[1:]:
                if ln.startswith('- '):
                    mvs.append(tg.to_const('move', ln[2:]))
                    continue
                k, _, val = ln.partition(':')
                val = val.strip()
                if k == 'Level':
                    level = int(val)
                elif k == 'Ability':
                    if tg.ability_const(val) not in d.raw[sp]['abilities']:
                        E('%s: %s cannot have ability %s' % (tid, sp, val))
                elif k == 'Nature':
                    if val.upper().replace('NATURE_', '') not in NATURES:
                        E('%s: bad nature %s' % (tid, val))
                elif k in ('IVs', 'EVs'):
                    for part in val.split('/'):
                        n = int(part.split()[0])
                        if not 0 <= n <= (31 if k == 'IVs' else 255):
                            E('%s: %s out of range' % (tid, k))
            lv.append(level)
            if not 1 <= level <= 100:
                E('%s: level %d out of range' % (tid, level))
            if d.min_level(sp) > level:
                E('%s: %s at L%d but obtainable only from L%d' % (tid, sp, level, d.min_level(sp)))
            if d.is_banned_family(sp):
                (W if boss else E)('%s: legendary/mythical/UB/paradox %s' % (tid, sp))
            if len(mvs) > 4:
                E('%s: %s has > 4 moves' % (tid, sp))
            legal = set(mdb.legal_moves(sp, level, include_tm=True, include_egg=True, include_tutor=True,
                                        include_prevo=True))
            for mv in mvs:
                if mv not in moves:
                    E('%s: bad move %s' % (tid, mv))
                elif mv not in legal:
                    E('%s: %s cannot learn %s at L%d' % (tid, sp, mv, level))
            if not boss:
                nxt = d.next_evo_level(sp)
                if nxt is not None and level >= nxt + 15:
                    W('%s: %s L%d is very under-evolved (evolves at %d)' % (tid, sp, level, nxt))
        if tid in ACE_CAPS:
            if not lv or lv[-1] != ACE_CAPS[tid] or max(lv) != ACE_CAPS[tid]:
                E('%s: ace level %s != cap %d' % (tid, lv[-1] if lv else None, ACE_CAPS[tid]))
            if cls == 'Leader' and any(not (ACE_CAPS[tid] - 3 <= x <= ACE_CAPS[tid]) for x in lv):
                W('%s: non-ace levels outside cap-3..cap: %s' % (tid, lv))
    for w in warns:
        print('WARN ', w)
    for x in errs:
        print('ERROR', x)
    print('checked %d trainers, %d Pokemon: %d errors, %d warnings' % (len(nmap), nmons, len(errs), len(warns)))
    return 1 if errs else 0


if __name__ == '__main__':
    sys.exit(main())
