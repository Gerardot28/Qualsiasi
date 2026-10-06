#!/usr/bin/env python3
"""Validate a generated wild_encounters.json against vanilla and the Multiverse rules.

    python3 -I check_wild.py --root <tree> [--vanilla /home/user/pex-orig] \
        [--md /home/user/Qualsiasi/docs/bilanciamento/incontri_selvatici.md]

Checks: JSON structure identical to vanilla except species/levels of Hoenn
tables (Kanto/FRLG and Pyramid/Pike untouched), every SPECIES_ constant exists
in include/constants/species.h, min <= max, level curve, evolution/level sanity
(rule b), power rules (rule c), table variety (rule d), habitat pools for
surf/fishing/rock smash, and family coverage (rule e). Writes the per-map
summary in Italian. Exit code 1 on any error.
"""
import argparse
import collections
import json
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import wild_common as wc  # noqa: E402

FISH_EGG = {'EGG_GROUP_WATER_2', 'EGG_GROUP_WATER_3'}
FIELD_IT = {'land_mons': 'Erba/terreno', 'water_mons': 'Surf', 'rock_smash_mons': 'Spaccaroccia',
            'fishing_mons': 'Pesca'}


def strip(o):
    """Copy of the JSON with species and levels removed."""
    if isinstance(o, dict):
        return {k: strip(v) for k, v in o.items() if k not in ('species', 'min_level', 'max_level')}
    if isinstance(o, list):
        return [strip(x) for x in o]
    return o


def species_constants(root):
    txt = open(os.path.join(root, 'include/constants/species.h'), encoding='utf-8').read()
    return set(re.findall(r'\b(SPECIES_[A-Z0-9_]+)\s*(?:=|,)', txt)) | \
        set(re.findall(r'#define\s+(SPECIES_[A-Z0-9_]+)', txt))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--root', required=True)
    ap.add_argument('--json', help='file to check (default <root>/src/data/wild_encounters.json)')
    ap.add_argument('--vanilla', default='/home/user/pex-orig')
    ap.add_argument('--data', default=wc.DATA_DIR)
    ap.add_argument('--md', default='/home/user/Qualsiasi/docs/bilanciamento/incontri_selvatici.md')
    a = ap.parse_args()
    new = json.load(open(a.json or os.path.join(a.root, 'src/data/wild_encounters.json'), encoding='utf-8'))
    old = json.load(open(os.path.join(a.vanilla, 'src/data/wild_encounters.json'), encoding='utf-8'))
    db = wc.DB(a.data)
    consts = species_constants(a.root)
    errors, warns = [], []

    def err(msg):
        errors.append(msg)

    # ---- structure -----------------------------------------------------------
    if strip(new) != strip(old):
        err('structure differs from vanilla (groups/entries/maps/labels/slot counts/rates)')
        print('\n'.join(errors))
        return 1
    hoenn = []
    for gi, (gn, go) in enumerate(zip(new['wild_encounter_groups'], old['wild_encounter_groups'])):
        for en, eo in zip(gn['encounters'], go['encounters']):
            if not wc.is_hoenn_table(gn, en):
                if en != eo:
                    err('non-Hoenn table changed: %s %s' % (gn['label'], en.get('base_label')))
                continue
            hoenn.append((en, eo))
    rates = {f['type']: f['encounter_rates'] for f in new['wild_encounter_groups'][0]['fields']}

    # ---- per-slot checks --------------------------------------------------------
    cov = collections.defaultdict(list)       # root -> [label] (basic appearances, reachable)
    gen_slots = collections.Counter()
    summary = []
    n_slots = 0
    for en, eo in hoenn:
        label = en['base_label']
        reachable = label not in wc.UNREACHABLE_LABELS
        postgame = label in wc.POSTGAME_LABELS
        for field in wc.FIELDS:
            if field not in en:
                continue
            mons, omons = en[field]['mons'], eo[field]['mons']
            legends = 0
            strong_early = set()
            starters, pseudos = set(), set()
            share = collections.Counter()
            for i, m in enumerate(mons):
                share[m['species']] += rates[field][i]
            for i, (m, o) in enumerate(zip(mons, omons)):
                n_slots += 1
                s = m['species']
                where = '%s/%s[%d] %s' % (label, field, i, s)
                rate = rates[field][i]
                tot = share[s]  # total rate of this species in the table
                if s not in consts:
                    err('%s: unknown species constant' % where)
                    continue
                if s not in db.sp:
                    err('%s: species not in species DB' % where)
                    continue
                d = db.sp[s]
                if not (1 <= m['min_level'] <= m['max_level'] <= 100):
                    err('%s: bad levels %d-%d' % (where, m['min_level'], m['max_level']))
                if not db.usable(s):
                    err('%s: species not usable in the wild (baby/gimmick/cosmetic form)' % where)
                root = d['familyRoot']
                fam = db.fam_by_root[root]
                gen_slots[d['generation']] += 1
                if fam['isLegendaryish']:
                    legends += 1
                    if not postgame:
                        err('%s: legendary-like species outside post-game areas' % where)
                    if field != 'land_mons' or tot > 1:
                        err('%s: legendary-like species not in a 1%% land slot' % where)
                    if not (wc.LEGEND_LEVELS[0] <= m['min_level'] <= m['max_level'] <= wc.LEGEND_LEVELS[1]):
                        err('%s: legendary-like species outside Lv 60-70' % where)
                    continue
                # level curve
                exp = (wc.map_level(o['min_level']), wc.map_level(o['max_level']))
                if (m['min_level'], m['max_level']) != exp:
                    err('%s: levels %d-%d, curve gives %d-%d' % (where, m['min_level'], m['max_level'], *exp))
                # rule (b): obtainable level
                lvl, low = db.obtainable(s)
                if m['min_level'] < lvl:
                    err('%s: Lv %d < obtainable level %d' % (where, m['min_level'], lvl))
                if low and (m['min_level'] < wc.NONLEVEL_EVO_MIN or tot > wc.LOW_RATE):
                    err('%s: stone/trade/friendship evolution needs Lv 30+ and rate <= 5%%' % where)
                # rule (c): power curve
                fb = db.final_bst(root)
                if m['max_level'] <= wc.EARLY_LEVEL:
                    if d['bst'] > wc.EARLY_OWN_BST:
                        err('%s: early slot with own BST %d > 330' % (where, d['bst']))
                    if fb > wc.STRONG_BST:
                        strong_early.add(root)
                        if tot > 1:
                            err('%s: strong family (final BST %d) in early non-1%% slot' % (where, fb))
                if db.is_pseudo_family(root):
                    pseudos.add(root)
                    if m['min_level'] < 30 or tot > wc.LOW_RATE:
                        err('%s: pseudo-legendary family needs Lv 30+ and 1-5%% slot' % where)
                if fam['isStarter']:
                    starters.add(root)
                    if m['min_level'] < 20 or tot > wc.LOW_RATE:
                        err('%s: starter family needs Lv 20+ and 1-5%% slot' % where)
                # habitat pools
                types = set(d['types'])
                if field == 'water_mons' and 'TYPE_WATER' not in types:
                    err('%s: surfing species is not Water type' % where)
                if field == 'fishing_mons' and ('TYPE_WATER' not in types or
                                                not set(d.get('eggGroups') or []) & FISH_EGG):
                    err('%s: fishing species is not a Water fish (Water 2/3 egg group)' % where)
                if field == 'rock_smash_mons' and not types & {'TYPE_ROCK', 'TYPE_GROUND'}:
                    err('%s: rock smash species is not Rock/Ground' % where)
                if reachable and db.is_basic(s):
                    cov[root].append(label)
            if legends > 1:
                err('%s/%s: %d legendary-like species (max 1)' % (label, field, legends))
            if len(strong_early) > 1:
                err('%s/%s: %d strong families in early slots (max 1)' % (label, field, len(strong_early)))
            if len(starters) > 1 or len(pseudos) > 1:
                warns.append('%s/%s: %d starter / %d pseudo families' % (label, field, len(starters), len(pseudos)))
            distinct = list(dict.fromkeys(m['species'] for m in mons))
            if field == 'land_mons' and not (6 <= len(distinct) <= 9):
                err('%s/%s: %d distinct species (want 6-9)' % (label, field, len(distinct)))
            lv = (min(m['min_level'] for m in mons), max(m['max_level'] for m in mons))
            summary.append((en['map'], label, field, lv, [(sp, share[sp]) for sp in distinct], reachable))

    # nearby copies: identical species sets for consecutive land tables
    land = [x for x in summary if x[2] == 'land_mons']
    for p, q in zip(land, land[1:]):
        if set(s for s, _ in p[4]) == set(s for s, _ in q[4]):
            warns.append('identical species set: %s and %s' % (p[1], q[1]))

    # ---- coverage ---------------------------------------------------------------
    need = db.coverage_families()
    covered = [r for r in need if cov.get(r)]
    missing = [r for r in need if not cov.get(r)]
    for r in missing:
        err('coverage: family %s has no basic member in a reachable Hoenn table' % r)
    by_gen = collections.defaultdict(lambda: [0, 0])
    for r in need:
        g = min(db.fam_by_root[r]['gens'])
        by_gen[g][1] += 1
        if cov.get(r):
            by_gen[g][0] += 1
    regional = [r for r in need if db.sp[r].get('isRegionalForm')]

    # ---- markdown ---------------------------------------------------------------
    name = lambda s: db.sp[s]['speciesName'] + (  # noqa: E731
        ' (%s)' % db.sp[s]['region'].capitalize() if db.sp[s].get('region') else '')
    L = ['# Incontri selvatici di Hoenn (Pokémon Multiverse)', '',
         'Generato da `tools/balance/wild_gen.py` e verificato da `tools/balance/check_wild.py`. '
         'Non modificare a mano: rigenerare.', '',
         '## Riepilogo copertura', '',
         '- Famiglie non leggendarie da coprire: **%d**' % len(need),
         '- Famiglie coperte (forma base non baby presente in una tabella raggiungibile): **%d** (%.1f%%)'
         % (len(covered), 100.0 * len(covered) / len(need)),
         '- Famiglie regionali (Alola/Galar/Hisui/Paldea) coperte: **%d/%d**'
         % (sum(1 for r in regional if cov.get(r)), len(regional)),
         '- Tabelle Hoenn generate: **%d** (slot: %d)' % (len(summary), n_slots), '']
    if missing:
        L += ['Famiglie mancanti: ' + ', '.join(missing), '']
    L += ['### Copertura per generazione', '', '| Gen | Famiglie coperte | Slot occupati |', '|---|---|---|']
    for g in sorted(by_gen):
        L.append('| %d | %d/%d | %d |' % (g, by_gen[g][0], by_gen[g][1], gen_slots[g]))
    L += ['', '## Regole applicate', '',
          '- Livelli: curva vanilla → nuovo (2→2, 5→6, 12→13, 15→15, 19→19, 24→24, 29→29, 31→33, '
          '33→37, 42→44, 46→49, 49→52, 55→57, 58→62, 70→72, 100→100), anche per la pesca.',
          '- Evoluzioni: forme base sempre; evoluzioni per livello solo da livello evolutivo + 2; '
          'pietra/scambio/amicizia/altro solo da Lv 30 e in slot ≤ 5%.',
          '- Potenza: slot ≤ Lv 15 solo specie con PS totali ≤ 330 e famiglie con BST finale ≤ 535 '
          '(max 1 famiglia forte in uno slot 1%); pseudo-leggendari da Lv 30 in slot 1-5%; '
          'famiglie degli starter da Lv 20 in slot 1-5% (max 1 per tabella); leggendari/misteriosi/UC/paradosso '
          'solo in aree post-game, slot 1%, Lv 60-70, max 1 per tabella.',
          '- Habitat: ~70% degli slot terrestri con tipi dell\'habitat, ~30% liberi; Surf = tipo Acqua; '
          'Pesca = pesci Acqua (gruppo uova Acqua 2/3), Amo Vecchio debole, Amo Buono medio, Super Amo forte; '
          'Spaccaroccia = Roccia/Terra.', '',
          '## Tabelle per mappa', '',
          'Percentuale = somma dei tassi degli slot della specie. *(non raggiungibile)* = tabella presente '
          'ma non usata nel gioco normale (esclusa dal calcolo della copertura).', '',
          '| Mappa | Tipo | Livelli | Specie |', '|---|---|---|---|']
    for mp, label, field, lv, sps, reach in summary:
        L.append('| %s%s | %s | %d-%d | %s |' % (
            label[1:] if label.startswith('g') else label, '' if reach else ' *(non raggiungibile)*',
            FIELD_IT[field], lv[0], lv[1], ', '.join('%s %d%%' % (name(s), p) for s, p in sps)))
    L += ['', '## Esito della verifica', '',
          '- Errori: **%d**' % len(errors), '- Avvisi: **%d**' % len(warns), '']
    L += ['- ' + w for w in warns]
    if a.md:
        os.makedirs(os.path.dirname(a.md), exist_ok=True)
        with open(a.md, 'w', encoding='utf-8') as f:
            f.write('\n'.join(L) + '\n')

    print('check_wild: %d Hoenn tables, %d slots' % (len(summary), n_slots))
    print('coverage: %d/%d non-legendary families (%.1f%%), regional %d/%d'
          % (len(covered), len(need), 100.0 * len(covered) / len(need),
             sum(1 for r in regional if cov.get(r)), len(regional)))
    for g in sorted(by_gen):
        print('  gen %d: families %d/%d, slots %d' % (g, by_gen[g][0], by_gen[g][1], gen_slots[g]))
    for w in warns:
        print('WARN', w)
    for e in errors[:200]:
        print('ERROR', e)
    print('errors: %d, warnings: %d' % (len(errors), len(warns)))
    return 1 if errors else 0


if __name__ == '__main__':
    sys.exit(main())
