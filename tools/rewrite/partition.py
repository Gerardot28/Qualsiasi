#!/usr/bin/env python3
"""Partition the active Emerald text files into rewrite work units.

Large files are split at label boundaries into part files (orig + editable
copies under WORK/parts); assemble.py stitches them back into the tree.
"""
import json, os, re, shutil, sys

ROOT = '/home/user/pex-orig'
WORK = '/home/user/work/rewrite'
TARGET = 380      # .string lines per unit
SPLIT_OVER = 600  # split files with more .string lines than this

STORY_ORDER = """LittlerootTown Route101 OldaleTown Route103 Route102 PetalburgCity Route104 PetalburgWoods
RustboroCity Route116 RusturfTunnel Route105 Route106 DewfordTown GraniteCave Route107 Route108 Route109
SlateportCity Route110 MauvilleCity Route117 VerdanturfTown Route111 Route112 FieryPath Route113
FallarborTown Route114 MeteorFalls MtChimney JaggedPass LavaridgeTown Route115 Route118 Route119
Route120 FortreeCity Route121 SafariZone LilycoveCity MtPyre MagmaHideout AquaHideout Route122 Route123
Route124 MossdeepCity Route125 Route126 Route127 Route128 SeafloorCavern SootopolisCity CaveOfOrigin
SkyPillar Route129 Route130 Route131 Route132 Route133 Route134 PacifidlogTown AbandonedShip ShoalCave
SealedChamber EverGrandeCity VictoryRoad NewMauville MirageTower DesertRuins IslandCave AncientTomb
SouthernIsland MarineCave TerraCave ArtisanCave DesertUnderpass AlteringCave SSTidal""".split()

def nstr(text):
    return len(re.findall(r'^\s*\.string ', text, re.M))

def split_points(lines, target):
    """Return line indices where a new part may start (global labels / blank-line label starts)."""
    parts, cur, count = [], 0, 0
    for i, l in enumerate(lines):
        if re.match(r'^[A-Za-z_][A-Za-z0-9_]*::?\s*$', l) and count >= target and i > cur:
            parts.append((cur, i)); cur = i; count = 0
        if re.match(r'^\s*\.string ', l):
            count += 1
    parts.append((cur, len(lines)))
    return parts

def main():
    act = json.load(open('/home/user/work/active_counts.json'))['counts']
    files = [f for f in act if not f.endswith('data/scripts/debug.inc')]
    def area(f):
        return f.split('/')[2].split('_')[0] if f.startswith('data/maps/') else None
    def prio(f):
        a = area(f)
        if a in STORY_ORDER: return (0, STORY_ORDER.index(a), f)
        if a is not None: return (1, 0, f)
        return (2, 0, f)
    files.sort(key=prio)
    if os.path.exists(WORK): shutil.rmtree(WORK)
    os.makedirs(WORK + '/parts/orig'); os.makedirs(WORK + '/parts/new')
    pieces = []  # (kind, path, orig, editable, nlines, label)
    splits = {}
    for f in files:
        text = open(os.path.join(ROOT, f), encoding='utf-8').read()
        n = nstr(text)
        if n > SPLIT_OVER:
            lines = text.splitlines(keepends=True)
            rng = split_points(lines, TARGET)
            flat = f.replace('/', '__')
            splits[f] = []
            for k, (a, b) in enumerate(rng):
                name = f'{flat}.part{k:02d}'
                chunk = ''.join(lines[a:b])
                open(f'{WORK}/parts/orig/{name}', 'w', encoding='utf-8').write(chunk)
                open(f'{WORK}/parts/new/{name}', 'w', encoding='utf-8').write(chunk)
                splits[f].append(name)
                pieces.append({'file': f, 'part': name, 'edit': f'{WORK}/parts/new/{name}',
                               'orig': f'{WORK}/parts/orig/{name}', 'lines': nstr(chunk)})
        else:
            pieces.append({'file': f, 'part': None, 'edit': f'/home/user/pex/{f}',
                           'orig': f'{ROOT}/{f}', 'lines': n})
    units, cur, cnt = [], [], 0
    for p in pieces:
        if cur and cnt + p['lines'] > TARGET * 1.25:
            units.append(cur); cur, cnt = [], 0
        cur.append(p); cnt += p['lines']
    if cur: units.append(cur)
    out = [{'id': f'U{i:03d}', 'lines': sum(p['lines'] for p in u), 'pieces': u} for i, u in enumerate(units)]
    json.dump({'units': out, 'splits': splits}, open(f'{WORK}/units.json', 'w'), indent=1)
    print(len(out), 'units', sum(u['lines'] for u in out), 'lines;', len(splits), 'files split')

if __name__ == '__main__':
    main()
