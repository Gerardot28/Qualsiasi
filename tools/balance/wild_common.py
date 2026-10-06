"""Shared data and rules for wild_gen.py and check_wild.py (Pokemon Multiverse).

Everything here is deterministic. The species data comes from speciesdb
(species.json / families.json); the ~10 species whose types were stored as
raw numbers (9 = Steel, 19 = Fairy) are repaired from movedb_species.json.
"""
import json
import math
import os

DATA_DIR = os.environ.get('PEX_DATA_DIR', '/home/user/work')

# vanilla level -> new level (monotone piecewise linear, rounded half up)
LEVEL_CURVE = [(2, 2), (5, 6), (12, 13), (15, 15), (19, 19), (24, 24), (29, 29), (31, 33),
               (33, 37), (42, 44), (46, 49), (49, 52), (55, 57), (58, 62), (70, 72), (100, 100)]


def map_level(v):
    pts = LEVEL_CURVE
    if v <= pts[0][0]:
        return max(1, v)
    if v >= pts[-1][0]:
        return 100
    for (x0, y0), (x1, y1) in zip(pts, pts[1:]):
        if x0 <= v <= x1:
            y = y0 + (y1 - y0) * (v - x0) / (x1 - x0)
            return int(math.floor(y + 0.5))
    return v


FIELDS = ('land_mons', 'water_mons', 'rock_smash_mons', 'fishing_mons')
LOW_RATE = 5            # a slot with rate <= 5 counts as a "rare" (1-5%) slot
EVO_MARGIN = 2          # level-evolved forms need min_level >= evo level + 2
NONLEVEL_EVO_MIN = 30   # stone/trade/friendship/other evolutions from level 30
PSEUDO_BST = 600
STRONG_BST = 535
EARLY_LEVEL = 15
EARLY_OWN_BST = 330
LEGEND_LEVELS = (60, 70)

# Tables that exist in the data but are not reachable in normal gameplay
UNREACHABLE_LABELS = {
    'gCaveOfOrigin_UnusedRubySapphireMap1', 'gCaveOfOrigin_UnusedRubySapphireMap2',
    'gCaveOfOrigin_UnusedRubySapphireMap3',
    # Altering Cave sets 2-9 are only selected by Mystery Event (VAR_ALTERING_CAVE_WILD_SET)
    'gAlteringCave2', 'gAlteringCave3', 'gAlteringCave4', 'gAlteringCave5', 'gAlteringCave6',
    'gAlteringCave7', 'gAlteringCave8', 'gAlteringCave9',
    # Mirage Island (Route 130 land) appears only on a random daily roll
    'gRoute130',
}
# Tables in which one legendary/mythical/UB/paradox may appear (1% slot, Lv 60-70)
POSTGAME_LABELS = {
    'gArtisanCave_B1F', 'gArtisanCave_1F', 'gDesertUnderpass', 'gSkyPillar_5F',
    'gSafariZone_Northeast', 'gSafariZone_Southeast',
    'gAlteringCave1', 'gAlteringCave2', 'gAlteringCave3', 'gAlteringCave4', 'gAlteringCave5',
    'gAlteringCave6', 'gAlteringCave7', 'gAlteringCave8', 'gAlteringCave9',
}


def is_kanto(enc):
    """FRLG/Kanto/Sevii tables: their base labels are sXxx_FireRed / sXxx_LeafGreen
    and their map ids are Kanto ones (folders ending _Frlg included)."""
    bl = enc.get('base_label', '')
    return (not bl.startswith('g')) or bl.endswith('_FireRed') or bl.endswith('_LeafGreen') \
        or enc.get('map', '').endswith('_FRLG')


def is_hoenn_table(group, enc):
    return group.get('label') == 'gWildMonHeaders' and not is_kanto(enc)


def load_json(name, data_dir=None):
    with open(os.path.join(data_dir or DATA_DIR, name), encoding='utf-8') as f:
        return json.load(f)


class DB:
    def __init__(self, data_dir=None):
        self.sp = load_json('species.json', data_dir)['species']
        mv = load_json('movedb_species.json', data_dir)
        self.type_fixes = []
        for k, v in self.sp.items():
            if any(not isinstance(t, str) for t in v['types']):
                fixed = list(dict.fromkeys(mv[k]['types']))
                self.type_fixes.append((k, v['types'], fixed))
                v['types'] = fixed
        self.families = load_json('families.json', data_dir)['families']
        self.fam_by_root = {f['root']: f for f in self.families}
        self._obt = {}

    # ---- per-species rules -------------------------------------------------
    def usable(self, s):
        """Species that may be placed in a wild slot at all."""
        d = self.sp.get(s)
        if not d or not d.get('encounterable') or d.get('isBaby'):
            return False
        fk = d.get('formKind')
        if fk in ('base', 'regional'):
            return True
        if fk == 'alternate':
            return bool(d.get('preEvolution')) and d.get('preEvolutionLink') == 'evolution'
        return False

    def is_basic(self, s):
        d = self.sp[s]
        return d.get('stage', 0) == 0 and not d.get('isBaby')

    def obtainable(self, s):
        """(min level, needs_low_rate) at which species s may appear in the wild.
        Basics: (1, False). Level evolutions: evo level + 2. Any other method
        (stone, trade, friendship, held item, move, ...): level 30 and low-rate slots only."""
        if s in self._obt:
            return self._obt[s]
        d = self.sp[s]
        if self.is_basic(s) or not d.get('preEvolution'):
            r = (1, False)
        else:
            pl, pr = self.obtainable(d['preEvolution'])
            if d.get('evolvedBy') == 'level' and d.get('evolvesFromLevel'):
                r = (max(pl, d['evolvesFromLevel'] + EVO_MARGIN), pr)
            else:
                r = (max(pl, NONLEVEL_EVO_MIN), True)
        self._obt[s] = r
        return r

    def root(self, s):
        return self.sp[s]['familyRoot']

    def fam(self, s):
        return self.fam_by_root[self.root(s)]

    def final_bst(self, root):
        return max(self.sp[m]['bst'] for m in self.fam_by_root[root]['members'])

    def is_pseudo_family(self, root):
        f = self.fam_by_root[root]
        return not f['isLegendaryish'] and self.final_bst(root) >= PSEUDO_BST

    def is_strong_family(self, root):
        return self.final_bst(root) > STRONG_BST

    def coverage_families(self):
        """Non-legendary families with at least one usable basic non-baby member."""
        out = []
        for f in self.families:
            if f['isLegendaryish']:
                continue
            out.append(f['root'])
        return out

    def basic_members(self, root):
        return [m for m in self.fam_by_root[root]['members'] if self.usable(m) and self.is_basic(m)]
