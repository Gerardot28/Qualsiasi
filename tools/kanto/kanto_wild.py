#!/usr/bin/env python3
"""Kanto (Act 2) wild encounters for Pokemon Multiverse.

Rewrites species and levels of the FireRed Kanto tables (base labels "s..._FireRed") of the maps built
in the Emerald ROM by port_kanto.py, with the Hoenn generator (tools/balance/wild_gen.py: habitat
letters, evolution sanity, power curve, variety) adapted to Kanto:
  * levels: the phases of docs/storia/bibbia_kanto.md section 7 (50-66); every table keeps the
    relative spread of its vanilla slots inside the phase range;
  * Gen 1-2 families get a score bonus (about 2/3 of the picks), the other generations stay possible;
  * Kanto habitats (forest, caves, tower, power plant, mansion, Seafoam, Safari Zone, routes);
  * Hoenn tables and every non-ported table (Sevii) are left untouched; legendaries are static
    encounters (scripts), never in the tables.
The input for the Kanto tables is always the vanilla file of /home/user/pex-orig (idempotent).

    python3 -I tools/kanto/kanto_wild.py --tree /home/user/pex
"""
import argparse, json, os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, '..', 'balance'))
import wild_common as wc  # noqa: E402
import wild_gen as wg  # noqa: E402

T = wg.T
PHASE_LEVELS = [(50, 53), (52, 55), (53, 56), (55, 58), (57, 60), (58, 61), (59, 62), (62, 66), (63, 66)]
PHASE_OF_MAPSEC = {
    'VERMILION_CITY': 0, 'ROUTE_6': 0, 'ROUTE_11': 0, 'S_S_ANNE': 0,
    'CERULEAN_CITY': 1, 'ROUTE_4': 1, 'ROUTE_24': 1, 'ROUTE_25': 1, 'MT_MOON': 1, 'ROUTE_5': 1,
    'PEWTER_CITY': 2, 'DIGLETTS_CAVE': 2, 'ROUTE_2': 2, 'ROUTE_3': 2, 'VIRIDIAN_FOREST': 2, 'ROUTE_1': 2,
    'ROUTE_22': 2, 'PALLET_TOWN': 2, 'VIRIDIAN_CITY': 2,
    'CELADON_CITY': 3, 'LAVENDER_TOWN': 3, 'ROCK_TUNNEL': 3, 'ROUTE_7': 3, 'ROUTE_8': 3, 'ROUTE_9': 3,
    'ROUTE_10': 3, 'POKEMON_TOWER': 3, 'POWER_PLANT': 3,
    'ROUTE_12': 4, 'ROUTE_13': 4, 'ROUTE_14': 4, 'ROUTE_15': 4, 'ROUTE_16': 4, 'ROUTE_17': 4,
    'ROUTE_18': 4, 'FUCHSIA_CITY': 4, 'KANTO_SAFARI_ZONE': 4,
    'ROUTE_19': 6, 'ROUTE_20': 6, 'ROUTE_21': 6, 'SEAFOAM_ISLANDS': 6, 'CINNABAR_ISLAND': 6, 'POKEMON_MANSION': 6,
    'CERULEAN_CAVE': 7,
    'ROUTE_23': 8, 'KANTO_VICTORY_ROAD': 8,
}


def kanto_habitat(map_id):
    m = map_id[4:]
    if m.startswith('VIRIDIAN_FOREST'):
        return 'forest', T('BUG', 'GRASS', 'POISON')
    if m.startswith(('MT_MOON', 'ROCK_TUNNEL', 'DIGLETTS', 'VICTORY_ROAD', 'CERULEAN_CAVE')):
        return 'cave', T('ROCK', 'GROUND', 'FIGHTING', 'POISON', 'DARK', 'STEEL', 'PSYCHIC')
    if m.startswith('SEAFOAM'):
        return 'icecave', T('ICE', 'WATER')
    if m.startswith('POKEMON_TOWER'):
        return 'tower', T('GHOST', 'DARK')
    if m.startswith('POWER_PLANT'):
        return 'plant', T('ELECTRIC', 'STEEL')
    if m.startswith('POKEMON_MANSION'):
        return 'mansion', T('FIRE', 'POISON', 'PSYCHIC', 'NORMAL')
    if m.startswith('SAFARI_ZONE'):
        return 'safari', set()
    if m in ('ROUTE19', 'ROUTE20', 'ROUTE21_NORTH', 'ROUTE21_SOUTH', 'CINNABAR_ISLAND', 'VERMILION_CITY',
             'SSANNE_EXTERIOR', 'PALLET_TOWN', 'VIRIDIAN_CITY', 'CERULEAN_CITY', 'FUCHSIA_CITY', 'CELADON_CITY'):
        return 'coast', T('WATER', 'FLYING', 'NORMAL')
    if m in ('ROUTE10', 'ROUTE23', 'ROUTE3', 'ROUTE4', 'ROUTE9'):
        return 'route', T('ROCK', 'GROUND', 'FIGHTING', 'NORMAL', 'FLYING', 'POISON')
    return 'route', T('NORMAL', 'FLYING', 'GRASS', 'BUG', 'POISON', 'ELECTRIC', 'FAIRY')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--tree', default='/home/user/pex')
    ap.add_argument('--vanilla', default='/home/user/pex-orig/src/data/wild_encounters.json')
    ap.add_argument('--data', default=wc.DATA_DIR)
    a = ap.parse_args()
    inv = json.load(open(os.path.join(HERE, '..', '..', 'docs/kanto/mappe.json')))['maps']
    phase = {m['id']: PHASE_OF_MAPSEC[m['mapsec'][7:]] for m in inv if m['wild_encounters']}
    out_path = os.path.join(a.tree, 'src/data/wild_encounters.json')
    cur = json.load(open(out_path, encoding='utf-8'))
    van = json.load(open(a.vanilla, encoding='utf-8'))
    vg = van['wild_encounter_groups'][0]
    vk = {e['base_label']: e for e in vg['encounters']}
    g = cur['wild_encounter_groups'][0]

    db = wc.DB(a.data)
    gen = wg.Gen(db, wg.SEED ^ 0x4B414E54)  # "KANT"
    gen.cov_set = set()  # Hoenn tables already cover every family
    gen.has_home = {}
    gen.tables = []
    gen.rates = {f['type']: f['encounter_rates'] for f in g['fields']}
    # Gen 1-2 bias: a negative starting usage is a score bonus (score -= 0.9 * usage)
    for root in gen.members:
        if min(db.fam_by_root[root].get('gens') or [9]) <= 2:
            gen.usage[root] = -5.0
    wg.habitat = kanto_habitat
    wc.map_level = lambda v: v  # levels are set below, per phase

    order = sorted((phase[e['map']], e['base_label']) for e in g['encounters']
                   if e['base_label'].endswith('_FireRed') and e['map'] in phase)
    done = 0
    for ph, label in order:
        idx = [i for i, e in enumerate(g['encounters']) if e['base_label'] == label][0]
        enc = json.loads(json.dumps(vk[label]))
        lo, hi = PHASE_LEVELS[ph]
        for field in wc.FIELDS:
            if field not in enc:
                continue
            mons = enc[field]['mons']
            vmin = min(m['min_level'] for m in mons)
            vmax = max(m['max_level'] for m in mons)
            span = max(1, vmax - vmin)
            for m in mons:
                m['min_level'] = lo + round((m['min_level'] - vmin) * (hi - lo) / span)
                m['max_level'] = max(m['min_level'], lo + round((m['max_level'] - vmin) * (hi - lo) / span))
            gen.gen_table(enc, field, enc[field], True, '%s/%s' % (label, field))
        g['encounters'][idx] = enc
        done += 1
    with open(out_path, 'w', encoding='utf-8') as f:
        f.write(json.dumps(cur, indent=2) + '\n')
    # summary
    gens12 = tot = 0
    for e in g['encounters']:
        if e['base_label'].endswith('_FireRed') and e['map'] in phase:
            for field in wc.FIELDS:
                for m in e.get(field, {}).get('mons', []):
                    tot += 1
                    gens12 += db.sp[m['species']].get('generation', 9) <= 2
    print('kanto_wild: %d Kanto tables rewritten, %d%% of the slots are Gen 1-2 species' % (done, 100 * gens12 // max(1, tot)))


if __name__ == '__main__':
    main()
