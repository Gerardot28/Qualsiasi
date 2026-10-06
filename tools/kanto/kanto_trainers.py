#!/usr/bin/env python3
"""Kanto (Act 2) trainers for Pokemon Multiverse (pokeemerald-expansion 1.17.1, Emerald build).

Idempotent.  For every Kanto trainer object ported by port_kanto.py (docs/kanto/mappe.json) it:
  * creates a TRAINER_KANTO_<frlg name> constant (include/constants/opponents.h, after the Emerald ones,
    raising TRAINERS_COUNT_EMERALD / MAX_TRAINERS_COUNT_EMERALD),
  * generates its party (src/data/trainers.party, appended block "=== TRAINER_KANTO_*"), themed by FRLG
    class with the Hoenn generator (tools/balance/trainer_gen.py), Gen 1-2 weighted, Kanto level curve,
  * merges the hand-authored bosses of tools/kanto/kanto_bosses.party (leaders, E4, Blu, Giovanni, Arianna),
  * turns the stub script of every regular trainer object into a trainerbattle script with Italian
    placeholder texts (Intro / Defeat / PostBattle [/ NotEnough]) and restores trainer_type/sight in map.json.
Bosses (leaders, E4, Giovanni, ...) are scripted by hand in their stubs: this tool only makes their parties.

    python3 -I tools/kanto/kanto_trainers.py --tree /home/user/pex [--dry-run]
"""
import argparse, json, os, re, sys, zlib, random

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, '..', 'balance'))
import trainer_gen as tg  # noqa: E402

DOCS = os.path.join(HERE, '..', '..', 'docs', 'kanto', 'mappe.json')
BOSSES = os.path.join(HERE, 'kanto_bosses.party')
REF_FRLG = '/home/user/pex-orig/src/data/trainers_frlg.party'
MARK_OPP_BEGIN = '// KANTO_TRAINERS begin (tools/kanto/kanto_trainers.py)'
MARK_OPP_END = '// KANTO_TRAINERS end'

# ------------------------------------------------------------------ level curve (Kanto, Act 2)
# segment -> base level of regular trainers (party ace); canonical FRLG order
SEG_LEVEL = [55, 56, 57, 58, 60, 61, 63, 65]
SEG_OF_MAPSEC = {
    'PALLET_TOWN': 0, 'ROUTE_1': 0, 'VIRIDIAN_CITY': 0, 'ROUTE_2': 0, 'ROUTE_22': 0, 'VIRIDIAN_FOREST': 0,
    'PEWTER_CITY': 0,
    'ROUTE_3': 1, 'MT_MOON': 1, 'ROUTE_4': 1, 'CERULEAN_CITY': 1, 'ROUTE_24': 1, 'ROUTE_25': 1,
    'ROUTE_5': 2, 'ROUTE_6': 2, 'UNDERGROUND_PATH': 2, 'UNDERGROUND_PATH_2': 2, 'VERMILION_CITY': 2,
    'S_S_ANNE': 2, 'ROUTE_11': 2, 'DIGLETTS_CAVE': 2,
    'ROUTE_9': 3, 'ROUTE_10': 3, 'ROCK_TUNNEL': 3, 'POWER_PLANT': 3, 'LAVENDER_TOWN': 3, 'ROUTE_8': 3,
    'ROUTE_7': 3, 'CELADON_CITY': 3, 'ROCKET_HIDEOUT': 3,
    'POKEMON_TOWER': 4, 'ROUTE_12': 4, 'ROUTE_13': 4, 'ROUTE_14': 4, 'ROUTE_15': 4, 'ROUTE_16': 4,
    'ROUTE_17': 4, 'ROUTE_18': 4, 'FUCHSIA_CITY': 4, 'KANTO_SAFARI_ZONE': 4,
    'SAFFRON_CITY': 5, 'SILPH_CO': 5,
    'ROUTE_19': 6, 'ROUTE_20': 6, 'ROUTE_21': 6, 'SEAFOAM_ISLANDS': 6, 'CINNABAR_ISLAND': 6,
    'POKEMON_MANSION': 6,
    'ROUTE_23': 7, 'KANTO_VICTORY_ROAD': 7, 'INDIGO_PLATEAU': 7, 'POKEMON_LEAGUE': 7, 'CERULEAN_CAVE': 7,
}
# gym map -> ace level of the leader (gym trainers: ace - 3)
GYM_ACE = {'PewterCity_Gym_Frlg': 57, 'CeruleanCity_Gym_Frlg': 58, 'VermilionCity_Gym_Frlg': 59,
           'CeladonCity_Gym_Frlg': 60, 'FuchsiaCity_Gym_Frlg': 62, 'SaffronCity_Gym_Frlg': 63,
           'CinnabarIsland_Gym_Frlg': 64, 'ViridianCity_Gym_Frlg': 66}

T = tg.T
CLASS_THEME = {
    'Youngster': (T('normal', 'bug', 'poison', 'flying', 'ground'), False),
    'Bug Catcher': (T('bug'), False),
    'Lass': (T('normal', 'fairy', 'grass'), False),
    'Sailor': (T('water', 'fighting'), False),
    'Camper': (T('ground', 'bug', 'normal', 'fire'), False),
    'Picnicker': (T('grass', 'fairy', 'normal', 'bug'), False),
    'Pokemaniac': (T('dragon', 'rock', 'ground', 'normal'), False),
    'Super Nerd': (T('electric', 'poison', 'steel', 'psychic'), False),
    'Hiker': (T('rock', 'ground', 'fighting'), False),
    'Biker': (T('poison', 'dark', 'fire'), False),
    'Burglar': (T('fire', 'dark'), False),
    'Engineer': (T('electric', 'steel'), False),
    'Fisherman': (T('water'), False),
    'Swimmer M': (T('water'), False),
    'Swimmer F': (T('water'), False),
    'Cue Ball': (T('fighting', 'normal', 'dark'), False),
    'Gamer': (T('normal', 'electric', 'psychic', 'fairy'), False),
    'Beauty': (T('fairy', 'grass', 'normal', 'water'), False),
    'Psychic': (T('psychic'), False),
    'Rocker': (T('electric'), False),
    'Juggler': (T('psychic', 'ghost', 'normal'), False),
    'Tamer': (T('dark', 'normal', 'poison', 'fighting'), False),
    'Bird Keeper': (T('flying'), False),
    'Black Belt': (T('fighting'), False),
    'Scientist': (T('electric', 'poison', 'steel'), False),
    'Team Rocket': (T('poison', 'dark', 'normal'), False),
    'Cooltrainer': (None, True),
    'Gentleman': (T('normal', 'fire', 'electric', 'steel'), False),
    'Channeler': (T('ghost'), False),
    'Twins': (T('normal', 'fairy', 'psychic', 'electric'), False),
    'Cool Couple': (None, True),
    'Young Couple': (T('normal', 'fairy', 'grass', 'psychic'), False),
    'Crush Kin': (T('fighting'), False),
    'Crush Girl': (T('fighting'), False),
    'Sis And Bro': (T('water', 'normal', 'fire', 'electric'), False),
    'Tuber': (T('water'), False),
    'Pkmn Breeder': (T('normal', 'fairy', 'water', 'grass'), False),
    'Pkmn Ranger': (T('grass', 'bug', 'flying', 'normal', 'ground'), True),
    'Aroma Lady': (T('grass', 'fairy'), False),
    'Ruin Maniac': (T('ground', 'rock', 'ghost'), False),
    'Lady': (T('fairy', 'normal', 'psychic'), False),
    'Painter': (T('normal', 'fairy'), False),
}
FISH = tg.FISH


def kconst(frlg):
    return 'TRAINER_KANTO_' + frlg[len('TRAINER_'):]


# ------------------------------------------------------------------ placeholders
def it_class_names(tree):
    out = {}
    for m in re.finditer(r'\[TRAINER_CLASS_(\w+)\]\s*=\s*\{\s*_\("([^"]*)"\)', open(os.path.join(tree, 'src/battle_main.c')).read()):
        out[m.group(1)] = m.group(2)
    return out


def class_key(cls):
    return re.sub(r'[^A-Z0-9]+', '_', cls.upper()).strip('_')


def ph(who, what):
    return '\t.string "[TESTO: %s\\n%s]$"\n' % (who, what)


# ------------------------------------------------------------------ party generation
def pick_species(tid, cls, levels, strong):
    base = re.sub(r' Frlg$', '', cls)
    types, strong_c = CLASS_THEME.get(base, (None, False))
    strong = strong or strong_c
    extra = FISH if base == 'Fisherman' else None
    d = tg.db()
    rng = random.Random(zlib.crc32(('kanto' + tid).encode()))
    used, picks = set(), []
    for lvl in levels:
        cands = []
        for relax in (0, 1, 2):
            cands = tg.candidates(lvl, types, extra, strong and lvl >= 35, used, relax)
            if extra is not None and not cands and relax == 1:
                cands = tg.candidates(lvl, types, None, strong and lvl >= 35, used, 1)
            if cands:
                break
        cands.sort()
        weights = [4 if min(d.families[r].get('gens') or [9]) <= 2 else 1 for r, _ in cands]
        root, sp = rng.choices(cands, weights=weights, k=1)[0]
        used.add(root)
        picks.append(sp)
    return picks


def regular_entry(tid, hdr, nmons, base_level, double):
    cls = tg.header_get(hdr, 'Class') or ''
    strong = cls.startswith('Cooltrainer') or cls.startswith('Cool Couple')
    n = max(2 if not strong else 3, min(4, nmons))
    if double:
        n = max(n, 2)
    levels = [base_level - ((n - 1 - i) + 1) // 2 for i in range(n)]
    species = pick_species(tid, cls, levels, strong)
    seed0 = zlib.crc32(tid.encode())
    header = tg.header_set(list(hdr), 'AI', 'Basic Trainer')
    header = tg.header_set(header, 'Items', None)
    blocks = []
    iv = 22 if strong else 20
    for i, (sp, lvl) in enumerate(zip(species, levels)):
        moves = tg.capped_moveset(sp, lvl, seed0 + i, True)
        blocks.append(tg.fmt_mon(sp, lvl, iv, moves))
    return tg.emit({'id': tid}, header, blocks)


def title_name(n):
    return ' '.join(w.capitalize() if w.isupper() else w for w in n.split(' ')).replace('&', '&')


# ------------------------------------------------------------------ main
def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument('--tree', default='/home/user/pex')
    ap.add_argument('--dry-run', action='store_true')
    a = ap.parse_args(argv)
    tree = a.tree
    maps = json.load(open(DOCS))['maps']
    _, frlg = tg.parse_party(open(REF_FRLG).read())
    frlg = {e['id']: e for e in frlg}
    cls_it = it_class_names(tree)

    # ---- collect trainer objects
    trainers = {}  # frlg id -> dict(maps=[...], objs=[(map, obj)])
    for m in maps:
        for o in m['objects']:
            t = o.get('orig_trainer')
            if not t:
                continue
            trainers.setdefault(t, {'objs': []})['objs'].append((m, o))
    # ---- bosses
    _, bosses = tg.parse_party(open(BOSSES).read())
    boss_ids = [b['id'] for b in bosses]

    log, entries = [], []
    tg.BOSS_IV31_CLASSES |= {'Leader Frlg', 'Elite Four Frlg', 'Champion Frlg', 'Boss Frlg'}
    for t in sorted(trainers):
        kid = kconst(t)
        if kid in boss_ids:
            continue
        e = frlg[t]
        m0 = trainers[t]['objs'][0][0]
        if m0['map'] in GYM_ACE:
            lvl = GYM_ACE[m0['map']] - 3
        else:
            lvl = SEG_LEVEL[SEG_OF_MAPSEC[m0['mapsec'][len('MAPSEC_'):]]]
        hdr = [ln for ln in e['header'] if ln.split(':')[0] in ('Name', 'Class', 'Pic', 'Gender', 'Music', 'Double Battle')]
        hdr = tg.header_set(hdr, 'Name', title_name(tg.header_get(hdr, 'Name')))
        entries.append(regular_entry(kid, hdr, len(e['mons']), lvl, tg.is_double(e['header'])))
    for b in bosses:
        hdr = [ln for ln in b['header'] if ln.split(':')[0] in ('Name', 'Class', 'Pic', 'Gender', 'Music', 'Double Battle', 'Mugshot')]
        entries.append(tg.process_boss({'id': b['id'], 'header': hdr}, b, log))
    for ln in log:
        if ln.startswith(('ERROR', 'WARN')):
            print(ln)
    all_ids = [re.match(r'=== (\w+) ===', x).group(1) for x in entries]
    assert len(set(all_ids)) == len(all_ids)

    # ---- trainers.party
    pp = os.path.join(tree, 'src/data/trainers.party')
    txt = open(pp).read()
    i = txt.find('=== TRAINER_KANTO_')
    if i >= 0:
        txt = txt[:i]
    txt = txt.rstrip('\n') + '\n\n' + '\n'.join(entries)
    # ---- opponents.h
    op = os.path.join(tree, 'include/constants/opponents.h')
    o = open(op).read()
    if MARK_OPP_BEGIN in o:
        o = o[:o.index(MARK_OPP_BEGIN)] + o[o.index(MARK_OPP_END) + len(MARK_OPP_END) + 1:]
    m = re.search(r'#define TRAINER_MAY_PLACEHOLDER\s+(\d+)\n', o)
    first = int(m.group(1)) + 1
    block = [MARK_OPP_BEGIN]
    for k, tid in enumerate(all_ids):
        block.append('#define %-40s %d' % (tid, first + k))
    block.append(MARK_OPP_END)
    o = o[:m.end()] + '\n'.join(block) + '\n' + o[m.end():]
    count = first + len(all_ids)
    maxc = (count + 7) // 8 * 8 + 8
    o = re.sub(r'#define TRAINERS_COUNT_EMERALD\s+\d+', '#define TRAINERS_COUNT_EMERALD     %d' % count, o)
    o = re.sub(r'#define MAX_TRAINERS_COUNT_EMERALD\s+\d+.*', '#define MAX_TRAINERS_COUNT_EMERALD %d // KANTO_PORT: was 864 (+%d trainer flags, +%d saved bytes)'
               % (maxc, maxc - 864, (maxc - 864) // 8), o)
    print('trainers: %d Kanto entries (%d regular, %d bosses), TRAINERS_COUNT %d, MAX %d'
          % (len(all_ids), len(all_ids) - len(bosses), len(bosses), count, maxc))

    # ---- scripts + map.json
    changed_scripts = 0
    pending = {}
    for t, info in trainers.items():
        kid = kconst(t)
        if kid in boss_ids:
            continue
        e = frlg[t]
        dbl = tg.is_double(e['header'])
        cls = tg.header_get(e['header'], 'Class')
        who = '%s %s' % (cls_it.get(class_key(cls), cls), title_name(tg.header_get(e['header'], 'Name')))
        for m, ob in info['objs']:
            pending.setdefault(m['map'], []).append((ob, kid, dbl, who))
    for mp, objs in sorted(pending.items()):
        sp = os.path.join(tree, 'data/maps', mp, 'scripts.inc')
        s = open(sp).read()
        s0 = s
        for ob, kid, dbl, who in objs:
            lab = ob['script']
            base = lab.replace('_EventScript_', '_Text_')
            pat = re.compile(r'(?m)^%s::\n\tmsgbox %s, MSGBOX_NPC\n\tend\n' % (re.escape(lab), re.escape(base)))
            if not pat.search(s):
                continue  # already converted / edited by hand
            if dbl:
                body = ('%s::\n\ttrainerbattle_double %s, %sIntro, %sDefeat, %sNotEnough\n'
                        '\tmsgbox %sPostBattle, MSGBOX_AUTOCLOSE\n\tend\n') % (lab, kid, base, base, base, base)
            else:
                body = ('%s::\n\ttrainerbattle_single %s, %sIntro, %sDefeat\n'
                        '\tmsgbox %sPostBattle, MSGBOX_AUTOCLOSE\n\tend\n') % (lab, kid, base, base, base)
            s = pat.sub(lambda _: body, s, count=1)
            tpat = re.compile(r'(?m)^%s:\n\t\.string "[^\n]*\n' % re.escape(base))
            texts = '%sIntro:\n%s\n%sDefeat:\n%s\n%sPostBattle:\n%s' % (
                base, ph(who, 'prima della lotta'), base, ph(who, 'alla sconfitta'), base, ph(who, 'dopo la lotta'))
            if dbl:
                texts += '\n%sNotEnough:\n%s' % (base, ph(who, 'servono due Pokémon'))
            s, n = tpat.subn(lambda _: texts, s, count=1)
            assert n == 1, (mp, base)
        if s != s0:
            changed_scripts += 1
            if not a.dry_run:
                open(sp, 'w').write(s)
        # map.json: restore trainer type/sight of every converted regular trainer object
        jp = os.path.join(tree, 'data/maps', mp, 'map.json')
        j = json.load(open(jp))
        jchanged = False
        labs = {ob['script'] for ob, *_ in objs}
        for ev in j['object_events']:
            orig = ev.get('kanto_port_orig') or {}
            if ev.get('script') in labs and orig.get('trainer_type', 'TRAINER_TYPE_NONE') != 'TRAINER_TYPE_NONE':
                if ev['trainer_type'] != orig['trainer_type'] or ev['trainer_sight_or_berry_tree_id'] != orig['trainer_sight_or_berry_tree_id']:
                    ev['trainer_type'] = orig['trainer_type']
                    ev['trainer_sight_or_berry_tree_id'] = orig['trainer_sight_or_berry_tree_id']
                    jchanged = True
        if jchanged and not a.dry_run:
            with open(jp, 'w') as f:
                json.dump(j, f, indent=2, ensure_ascii=False)
                f.write('\n')
    print('scripts converted in %d map(s)' % changed_scripts)
    if not a.dry_run:
        open(pp, 'w').write(txt)
        open(op, 'w').write(o)


if __name__ == '__main__':
    main()
