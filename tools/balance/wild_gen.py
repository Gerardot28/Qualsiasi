#!/usr/bin/env python3
"""Generate balanced Hoenn wild encounters for Pokemon Multiverse.

    python3 -I wild_gen.py --root <tree> --out <tree>/src/data/wild_encounters.json

Reads <root>/src/data/wild_encounters.json (vanilla structure), keeps every
group, entry, map, base_label, slot count and encounter_rate, and only
rewrites species and levels of the Hoenn tables. Battle Pyramid / Battle Pike
groups and all FRLG/Kanto/Sevii tables are copied unchanged.
Deterministic: fixed seed, sorted inputs. See README.md (wild_gen section).
"""
import argparse
import json
import os
import random
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import wild_common as wc  # noqa: E402

SEED = 0x4D554C54  # "MULT"

T = lambda *xs: {'TYPE_' + x for x in xs}  # noqa: E731

# ---------------------------------------------------------------------------
# Habitat by map
# ---------------------------------------------------------------------------
ROUTE_HABITAT = {
    'ROUTE101': ('meadow', T('NORMAL', 'BUG', 'GRASS', 'FLYING')),
    'ROUTE102': ('meadow', T('NORMAL', 'BUG', 'GRASS', 'FLYING', 'WATER')),
    'ROUTE103': ('coast', T('WATER', 'FLYING', 'NORMAL', 'GROUND')),
    'ROUTE104': ('coast', T('WATER', 'FLYING', 'NORMAL', 'GRASS', 'BUG')),
    'ROUTE110': ('meadow', T('ELECTRIC', 'NORMAL', 'FLYING', 'POISON')),
    'ROUTE111': ('desert', T('GROUND', 'ROCK', 'FIRE')),
    'ROUTE112': ('mountain', T('ROCK', 'GROUND', 'FIRE', 'FIGHTING')),
    'ROUTE113': ('volcanic', T('FIRE', 'GROUND', 'ROCK', 'STEEL', 'POISON')),
    'ROUTE114': ('meadow', T('POISON', 'GRASS', 'GROUND', 'NORMAL', 'DARK')),
    'ROUTE115': ('coast', T('WATER', 'FLYING', 'NORMAL', 'FIGHTING')),
    'ROUTE116': ('meadow', T('NORMAL', 'BUG', 'GRASS', 'FIGHTING')),
    'ROUTE117': ('meadow', T('FAIRY', 'BUG', 'GRASS', 'NORMAL')),
    'ROUTE118': ('coast', T('WATER', 'FLYING', 'ELECTRIC', 'NORMAL')),
    'ROUTE119': ('forest', T('GRASS', 'BUG', 'POISON', 'WATER')),
    'ROUTE120': ('forest', T('GRASS', 'BUG', 'DARK', 'FAIRY')),
    'ROUTE121': ('meadow', T('GHOST', 'GRASS', 'DARK', 'NORMAL')),
    'ROUTE123': ('forest', T('GRASS', 'BUG', 'GHOST', 'FAIRY')),
    'ROUTE130': ('island', T('PSYCHIC', 'FAIRY', 'NORMAL')),
}


def habitat(map_id):
    m = map_id[4:] if map_id.startswith('MAP_') else map_id
    if m in ROUTE_HABITAT:
        return ROUTE_HABITAT[m]
    if m.startswith('SAFARI_ZONE'):
        return ('safari', set())
    if m.startswith('SKY_PILLAR'):
        return ('skypillar', T('DRAGON', 'FLYING', 'PSYCHIC'))
    if m.startswith('MT_PYRE'):
        if 'EXTERIOR' in m or 'SUMMIT' in m:
            return ('mountain_ghost', T('GHOST', 'PSYCHIC', 'DARK', 'FIRE', 'FAIRY'))
        return ('ghost', T('GHOST', 'PSYCHIC', 'DARK'))
    if m.startswith('SHOAL_CAVE'):
        return ('icecave', T('ICE', 'WATER'))
    if m.startswith('METEOR_FALLS'):
        return ('cave', T('ROCK', 'DRAGON', 'PSYCHIC', 'STEEL'))
    if m.startswith('VICTORY_ROAD'):
        return ('cave', T('FIGHTING', 'ROCK', 'STEEL', 'DRAGON', 'GROUND', 'DARK'))
    if m.startswith('SEAFLOOR_CAVERN'):
        return ('cave', T('WATER', 'DARK', 'ROCK', 'POISON'))
    if m.startswith('CAVE_OF_ORIGIN'):
        return ('cave', T('GHOST', 'DARK', 'PSYCHIC', 'ROCK', 'POISON'))
    if m.startswith('GRANITE_CAVE'):
        return ('cave', T('ROCK', 'STEEL', 'GROUND', 'DARK', 'FIGHTING'))
    if m.startswith('RUSTURF_TUNNEL'):
        return ('cave', T('NORMAL', 'FIGHTING', 'ROCK', 'GROUND'))
    if m.startswith('ARTISAN_CAVE') or m.startswith('ALTERING_CAVE'):
        return ('cave', T('ROCK', 'STEEL', 'GROUND', 'DARK', 'BUG', 'POISON'))
    if m.startswith('DESERT_UNDERPASS'):
        return ('cave', T('GROUND', 'ROCK', 'DRAGON', 'BUG'))
    if m.startswith(('FIERY_PATH', 'JAGGED_PASS', 'MAGMA_HIDEOUT', 'MT_CHIMNEY')):
        return ('volcanic', T('FIRE', 'GROUND', 'ROCK', 'STEEL', 'POISON'))
    if m.startswith('MIRAGE_TOWER'):
        return ('desert', T('GROUND', 'ROCK', 'FIRE'))
    if m.startswith('NEW_MAUVILLE'):
        return ('powerplant', T('ELECTRIC', 'STEEL'))
    if m.startswith('PETALBURG_WOODS'):
        return ('forest', T('GRASS', 'BUG', 'POISON', 'FAIRY'))
    if m.startswith('ABANDONED_SHIP'):
        return ('sea', T('WATER', 'GHOST'))
    if m.startswith('UNDERWATER'):
        return ('deepsea', T('WATER'))
    return ('sea', T('WATER'))


# ---------------------------------------------------------------------------
# Slot layouts: letter = distinct species of the table
# ---------------------------------------------------------------------------
LAYOUT = {
    'land_mons': ['A', 'B', 'C', 'D', 'A', 'B', 'E', 'F', 'G', 'H', 'I', 'I'],
    'land_mons_legend': ['A', 'B', 'C', 'D', 'A', 'B', 'E', 'E', 'G', 'H', 'I', 'L'],
    'water_mons': ['A', 'B', 'C', 'D', 'E'],
    'rock_smash_mons': ['A', 'B', 'C', 'D', 'E'],
    'fishing_mons': ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'],
}
OLD_ROD, GOOD_ROD = {'A', 'B'}, {'C', 'D', 'E'}


def bst_cap(level, rare):
    """Maximum own BST of a placed species at the slot's min level."""
    if level <= wc.EARLY_LEVEL:
        return wc.EARLY_OWN_BST
    for lv, common, rr in ((20, 405, 440), (25, 430, 470), (30, 460, 500), (35, 480, 520),
                           (40, 500, 535), (45, 515, 550), (52, 530, 555), (999, 545, 560)):
        if level <= lv:
            return rr if rare else common
    return 560


WATER_EGG = {'EGG_GROUP_WATER_1', 'EGG_GROUP_WATER_2', 'EGG_GROUP_WATER_3'}


class Gen:
    def __init__(self, db, seed=SEED):
        self.db = db
        self.seed = seed
        self.usage = {}           # family root -> times placed (any table)
        self.basic_cov = {}       # family root -> list of (table key, letter) where a basic appears (reachable)
        self.recent = []          # family sets of recently generated tables (nearby-map penalty)
        cov = db.coverage_families()
        self.cov_set = set(cov)
        # candidate members per family
        self.members = {}
        for f in db.families:
            ms = [m for m in f['members'] if db.usable(m)]
            if ms:
                self.members[f['root']] = ms
        self.legend_used = set()

    def rng(self, *key):
        return random.Random('%d|%s' % (self.seed, '|'.join(map(str, key))))

    # ----- constraints ------------------------------------------------------
    def pool_ok(self, field, letter, s):
        d = self.db.sp[s]
        types = set(d['types'])
        eggs = set(d.get('eggGroups') or [])
        if field == 'water_mons':
            return 'TYPE_WATER' in types or bool(eggs & WATER_EGG)
        if field == 'fishing_mons':
            return 'TYPE_WATER' in types and bool(eggs & WATER_EGG)
        if field == 'rock_smash_mons':
            return bool(types & T('ROCK', 'GROUND'))
        return True

    def member_ok(self, field, letter, s, ctx):
        """ctx: dict(min_level, max_level, rare, one_pct, early)."""
        db = self.db
        d = db.sp[s]
        root = d['familyRoot']
        fam = db.fam_by_root[root]
        lvl, low = db.obtainable(s)
        if ctx['min_level'] < lvl:
            return False
        if low and not ctx['rare']:
            return False
        if d['bst'] > bst_cap(ctx['min_level'], ctx['rare']):
            return False
        if fam['isStarter'] and (ctx['min_level'] < 20 or not ctx['rare']):
            return False
        if db.is_pseudo_family(root) and (ctx['min_level'] < 30 or not ctx['rare']):
            return False
        if db.is_strong_family(root):
            if ctx['early'] and not ctx['one_pct']:
                return False
            if ctx['min_level'] < 25 and not ctx['rare']:
                return False
        if field == 'fishing_mons':
            if letter in OLD_ROD and (not db.is_basic(s) or d['bst'] > 320):
                return False
            if letter in GOOD_ROD and d['bst'] > 450:
                return False
        if not self.pool_ok(field, letter, s):
            return False
        return True

    # ----- selection --------------------------------------------------------
    def pick_member(self, root, field, letter, ctx, prefer_basic):
        ok = [m for m in self.members.get(root, []) if self.member_ok(field, letter, m, ctx)]
        if not ok:
            return None
        if prefer_basic:
            b = [m for m in ok if self.db.is_basic(m)]
            if b:
                ok = b
        top = max(self.db.sp[m]['stage'] for m in ok)
        ok = [m for m in ok if self.db.sp[m]['stage'] == top]
        return ok[0] if len(ok) == 1 else self.rng('member', root, ctx['key'], letter).choice(sorted(ok))

    def choose(self, field, letter, ctx, pref_types, used_roots, habitat_letter, kind):
        db = self.db
        r = self.rng('choose', ctx['key'], field, letter)
        best = None
        for root in sorted(self.members):
            f = db.fam_by_root[root]
            if f['isLegendaryish'] or root in used_roots:
                continue
            if ctx['early'] and db.is_strong_family(root) and ctx.get('strong_used'):
                continue
            uncovered = root in self.cov_set and not self.basic_cov.get(root) and ctx['reachable']
            s = self.pick_member(root, field, letter, ctx, prefer_basic=uncovered)
            if s is None:
                continue
            types = set(db.sp[s]['types'])
            match = bool(types & pref_types)
            if habitat_letter and pref_types and not match:
                continue
            score = r.random()
            if uncovered and db.is_basic(s):
                score += 6.0
            score -= 0.9 * self.usage.get(root, 0)
            if any(root in rs for rs in self.recent[-4:]):
                score -= 3.0
            if match:
                score += 1.0
            if field == 'fishing_mons':
                eggs = set(db.sp[s].get('eggGroups') or [])
                if eggs & {'EGG_GROUP_WATER_2', 'EGG_GROUP_WATER_3'}:
                    score += 1.5
            if field == 'water_mons' and 'TYPE_WATER' in types:
                score += 1.0
            if kind == 'safari':
                score += 0.004 * (db.final_bst(root) - 400)  # rarer/stronger species
            if letter in ('A', 'B') and field == 'land_mons':
                # commons: prefer families with a weak basic (feel of a "common" Pokemon)
                score -= 0.002 * max(0, db.sp[s]['bst'] - 350)
            if best is None or score > best[0]:
                best = (score, root, s)
        return best

    def gen_table(self, enc, field, data, reachable, key):
        db = self.db
        hab_kind, pref = habitat(enc['map'])
        legend = (field == 'land_mons' and enc['base_label'] in wc.POSTGAME_LABELS)
        layout = LAYOUT['land_mons_legend' if legend else field]
        mons = data['mons']
        assert len(layout) == len(mons), (enc['base_label'], field)
        rates = self.rates[field]
        # new levels
        for m in mons:
            m['min_level'] = wc.map_level(m['min_level'])
            m['max_level'] = wc.map_level(m['max_level'])
        letters = []
        for L in layout:
            if L not in letters:
                letters.append(L)
        # which letters are habitat-typed (~70%) and which are free (~30%)
        if field == 'land_mons':
            r = self.rng('free', key)
            cand = [L for L in letters if L not in ('A', 'C', 'L')]
            free = set(r.sample(cand, max(1, round(len(letters) * 0.3)))) if pref else set(letters)
        elif field in ('water_mons', 'fishing_mons'):
            pref = T('WATER') | (pref - T('WATER'))
            free = set(letters)  # pool itself is the habitat
        else:
            pref = T('ROCK', 'GROUND')
            free = set(letters)
        used = set()
        chosen = {}
        strong_used = False
        order = [L for L in letters if L != 'L']
        for L in order:
            idx = [i for i, x in enumerate(layout) if x == L]
            ctx = {
                'key': key,
                'min_level': min(mons[i]['min_level'] for i in idx),
                'max_level': max(mons[i]['max_level'] for i in idx),
                'rare': all(rates[i] <= wc.LOW_RATE for i in idx),
                'one_pct': all(rates[i] <= 1 for i in idx),
                'reachable': reachable,
            }
            ctx['early'] = max(mons[i]['max_level'] for i in idx) <= wc.EARLY_LEVEL or \
                min(mons[i2]['max_level'] for i2 in range(len(mons))) <= wc.EARLY_LEVEL and \
                max(m['max_level'] for m in mons) <= wc.EARLY_LEVEL
            ctx['strong_used'] = strong_used
            best = self.choose(field, L, ctx, pref, used, L not in free, hab_kind)
            if best is None:
                best = self.choose(field, L, ctx, set(), used, False, hab_kind)
            if best is None:
                raise SystemExit('no candidate for %s %s %s' % (key, field, L))
            _, root, s = best
            if ctx['early'] and db.is_strong_family(root):
                strong_used = True
            used.add(root)
            chosen[L] = s
            self.usage[root] = self.usage.get(root, 0) + 1
            if reachable and db.is_basic(s):
                self.basic_cov.setdefault(root, []).append((key, field, L))
        if legend:
            chosen['L'] = self.pick_legend(enc, pref)
        for i, L in enumerate(layout):
            mons[i]['species'] = chosen[L]
            if L == 'L':
                mons[i]['min_level'], mons[i]['max_level'] = 60, 65
        self.recent.append(set(db.root(s) for s in chosen.values()))
        self.tables.append({'key': key, 'enc': enc, 'field': field, 'layout': layout,
                            'reachable': reachable, 'pref': pref, 'free': free, 'hab': hab_kind})

    def pick_legend(self, enc, pref):
        db = self.db
        r = self.rng('legend', enc['base_label'])
        best = None
        for f in db.families:
            if not f['isLegendaryish'] or f['isMythical'] or len(f['members']) != 1:
                continue
            s = f['root']
            d = db.sp[s]
            if not db.usable(s) or d.get('isRestrictedLegendary') or d['generation'] == 3:
                continue
            if s in self.legend_used:
                continue
            sc = r.random() + (2.0 if set(d['types']) & pref else 0)
            if best is None or sc > best[0]:
                best = (sc, s)
        self.legend_used.add(best[1])
        return best[1]

    # ----- coverage repair ----------------------------------------------------
    def repair(self, wild):
        db = self.db
        missing = [r for r in sorted(self.cov_set) if not self.basic_cov.get(r)]
        unfixable = []
        for root in missing:
            done = False
            cands = []
            for t in self.tables:
                if not t['reachable']:
                    continue
                enc, field, layout = t['enc'], t['field'], t['layout']
                mons = enc[field]['mons']
                rates = self.rates[field]
                roots_here = {db.root(m['species']) for m in mons}
                if root in roots_here:
                    continue
                letters = []
                for L in layout:
                    if L not in letters and L != 'L':
                        letters.append(L)
                for L in letters:
                    idx = [i for i, x in enumerate(layout) if x == L]
                    occ = mons[idx[0]]['species']
                    oroot = db.root(occ)
                    # occupant must stay covered elsewhere
                    if db.is_basic(occ) and len(self.basic_cov.get(oroot, [])) <= 1 and oroot in self.cov_set:
                        continue
                    ctx = {
                        'key': t['key'],
                        'min_level': min(mons[i]['min_level'] for i in idx),
                        'max_level': max(mons[i]['max_level'] for i in idx),
                        'rare': all(rates[i] <= wc.LOW_RATE for i in idx),
                        'one_pct': all(rates[i] <= 1 for i in idx),
                        'reachable': True,
                    }
                    ctx['early'] = max(m['max_level'] for m in mons) <= wc.EARLY_LEVEL
                    if ctx['early'] and db.is_strong_family(root):
                        if any(db.is_strong_family(db.root(m['species'])) for m in mons):
                            continue
                    s = self.pick_member(root, field, L, ctx, prefer_basic=True)
                    if s is None or not db.is_basic(s):
                        continue
                    match = bool(set(db.sp[s]['types']) & t['pref'])
                    if L not in t['free'] and t['pref'] and not match:
                        continue
                    sc = (2 if match else 0) + self.usage.get(oroot, 0) * 0.5 + \
                        self.rng('repair', root, t['key'], L).random()
                    cands.append((sc, t['key'], field, L, s, occ, idx, mons))
            if cands:
                cands.sort(key=lambda c: (-c[0], c[1], c[2], c[3]))
                sc, key, field, L, s, occ, idx, mons = cands[0]
                for i in idx:
                    mons[i]['species'] = s
                oroot = db.root(occ)
                self.usage[oroot] -= 1
                self.usage[root] = self.usage.get(root, 0) + 1
                if db.is_basic(occ):
                    self.basic_cov[oroot] = [x for x in self.basic_cov[oroot] if x != (key, field, L)]
                self.basic_cov.setdefault(root, []).append((key, field, L))
                done = True
            if not done:
                unfixable.append(root)
        return unfixable

    def run(self, wild):
        groups = wild['wild_encounter_groups']
        g = groups[0]
        self.rates = {f['type']: f['encounter_rates'] for f in g['fields']}
        self.tables = []
        for enc in g['encounters']:
            if not wc.is_hoenn_table(g, enc):
                continue
            reachable = enc['base_label'] not in wc.UNREACHABLE_LABELS
            for field in wc.FIELDS:
                if field in enc:
                    key = '%s/%s' % (enc['base_label'], field)
                    self.gen_table(enc, field, enc[field], reachable, key)
        return self.repair(wild)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[1])
    ap.add_argument('--root', required=True, help='source tree (reads src/data/wild_encounters.json)')
    ap.add_argument('--input', help='input JSON (default: <root>/src/data/wild_encounters.json)')
    ap.add_argument('--out', required=True)
    ap.add_argument('--data', default=wc.DATA_DIR, help='dir with species.json/families.json/movedb_species.json')
    ap.add_argument('--seed', type=int, default=SEED)
    a = ap.parse_args()
    src = a.input or os.path.join(a.root, 'src/data/wild_encounters.json')
    with open(src, encoding='utf-8') as f:
        wild = json.load(f)
    db = wc.DB(a.data)
    gen = Gen(db, a.seed)
    unfixable = gen.run(wild)
    with open(a.out, 'w', encoding='utf-8') as f:
        f.write(json.dumps(wild, indent=2) + '\n')
    cov = gen.cov_set
    covered = [r for r in cov if gen.basic_cov.get(r)]
    print('wild_gen: %d Hoenn tables, coverage %d/%d families' % (len(gen.tables), len(covered), len(cov)))
    if unfixable:
        print('uncovered:', ' '.join(unfixable))
    return 0


if __name__ == '__main__':
    sys.exit(main())
