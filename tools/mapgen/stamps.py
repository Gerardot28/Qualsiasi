#!/usr/bin/env python3
"""Stamp library: reusable rectangles of blocks cut out of existing layouts.

  stamps.py build     [--root TREE]   (re)extract stamps -> stamps/<family>.json
  stamps.py previews  [--root TREE] [--out DIR]   render PNG previews + contact sheets
  stamps.py list      [--family F]    print the catalogue

Two sources of stamps:
  * automatic: every door / cave-mouth / ladder warp of every Emerald map is
    traced back to the building (connected region of non-terrain metatiles)
    that contains it -> houses, Pokemon Centers, Marts, gyms, labs, caves...
  * manual: stamps_manual.json lists extra rectangles (layout + x,y,w,h), e.g.
    decorations, bridges, statues; cells can be masked out.

A stamp is family-independent ("general/...") when all its metatiles belong to
the primary tileset, otherwise it requires the secondary tileset it came from.
"""
import argparse
import collections
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import pexmap  # noqa: E402
import terrain  # noqa: E402

STAMP_DIR = os.path.join(HERE, 'stamps')
DEFAULT_ROOT = os.environ.get('PEX_ROOT', '/home/user/work/mapgen-tree')
DEFAULT_OUT = '/home/user/work/mapgen-out/stamps'

OUTDOOR_PRIMARIES = {'gTileset_General'}
CAVE_SECONDARIES = {'gTileset_Cave', 'gTileset_MeteorFalls', 'gTileset_RusturfTunnel', 'gTileset_NavelRock'}

DOOR_BEHAVIORS = {
    'MB_ANIMATED_DOOR': 'door', 'MB_NON_ANIMATED_DOOR': 'door', 'MB_PETALBURG_GYM_DOOR': 'door',
    'MB_LADDER': 'ladder', 'MB_SOUTH_ARROW_WARP': 'exit_south', 'MB_NORTH_ARROW_WARP': 'exit_north',
    'MB_EAST_ARROW_WARP': 'exit_east', 'MB_WEST_ARROW_WARP': 'exit_west', 'MB_DEEP_SOUTH_WARP': 'exit_south',
    'MB_WATER_DOOR': 'door', 'MB_UP_RIGHT_STAIR_WARP': 'stairs', 'MB_UP_LEFT_STAIR_WARP': 'stairs',
    'MB_DOWN_RIGHT_STAIR_WARP': 'stairs', 'MB_DOWN_LEFT_STAIR_WARP': 'stairs',
}

KIND_RULES = [
    ('POKEMON_CENTER', 'pokecenter'), ('DEPARTMENT_STORE', 'dept_store'), ('_MART', 'mart'),
    ('GYM', 'gym'), ('LAB', 'lab'), ('HOUSE', 'house'), ('FLAT', 'apartment'), ('MOTEL', 'hotel'),
    ('MUSEUM', 'museum'), ('SCHOOL', 'school'), ('HARBOR', 'harbor'), ('SHOP', 'shop'),
    ('BATTLE_TENT', 'battle_tent'), ('CONTEST', 'contest_hall'), ('GAME_CORNER', 'game_corner'),
    ('FAN_CLUB', 'fan_club'), ('SPACE_CENTER', 'space_center'), ('DEVON', 'office'),
    ('POKEMON_LEAGUE', 'league'), ('HALL', 'hall'), ('DAY_CARE', 'daycare'), ('CABLE_CAR', 'cable_car'),
    ('CAVE', 'cave'), ('TUNNEL', 'cave'), ('VICTORY_ROAD', 'cave'), ('FALLS', 'cave'), ('MT_', 'cave'),
    ('WOODS', 'woods_gate'),
]


def family_of(secondary):
    if not secondary:
        return 'general'
    s = secondary.replace('gTileset_', '')
    out = ''
    for i, ch in enumerate(s):
        if ch.isupper() and i:
            out += '_'
        out += ch.lower()
    return out


def kind_for(dest_map, behavior_kind, in_cave=False):
    if behavior_kind == 'ladder':
        return 'ladder'
    if behavior_kind.startswith('exit') or behavior_kind == 'stairs':
        return behavior_kind
    if in_cave:
        return 'passage'
    d = dest_map.replace('MAP_', '')
    for pat, k in KIND_RULES:
        if pat in d:
            return k
    if behavior_kind == 'ladder':
        return 'ladder'
    if behavior_kind.startswith('exit'):
        return behavior_kind
    return 'building'


def load_classes(project, layout):
    n_primary = project.consts_for(layout.is_frlg)['metatiles_primary']
    prim = terrain.load_class_table(layout.primary_symbol)
    sec = terrain.load_class_table(layout.secondary_symbol, offset=n_primary)
    return n_primary, prim, sec


def class_of(m, n_primary, prim, sec):
    return (prim.get(m) if m < n_primary else sec.get(m)) or terrain.UNKNOWN


def trace_component(blocks, cls_fn, x0, y0, max_up=10, max_side=8, max_down=2):
    """4-connected region of UNKNOWN-class cells containing (x0,y0)."""
    h, w = blocks.shape
    seen = {(x0, y0)}
    todo = [(x0, y0)]
    while todo:
        x, y = todo.pop()
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            xx, yy = x + dx, y + dy
            if not (0 <= xx < w and 0 <= yy < h) or (xx, yy) in seen:
                continue
            if abs(xx - x0) > max_side or y0 - yy > max_up or yy - y0 > max_down:
                continue
            if cls_fn(int(blocks[yy, xx]) & 0x3FF) != terrain.UNKNOWN:
                continue
            seen.add((xx, yy))
            todo.append((xx, yy))
    return seen


def make_stamp(project, layout, cells, doors, meta):
    xs = [c[0] for c in cells]
    ys = [c[1] for c in cells]
    x0, y0, x1, y1 = min(xs), min(ys), max(xs), max(ys)
    w, h = x1 - x0 + 1, y1 - y0 + 1
    n_primary = project.consts_for(layout.is_frlg)['metatiles_primary']
    rows = []
    uses_secondary = False
    for y in range(y0, y1 + 1):
        row = []
        for x in range(x0, x1 + 1):
            if (x, y) in cells:
                v = int(layout.blocks[y, x])
                row.append(v)
                if (v & 0x3FF) >= n_primary:
                    uses_secondary = True
            else:
                row.append(None)
        rows.append(row)
    st = {
        'primary': layout.primary_symbol,
        'secondary': layout.secondary_symbol if uses_secondary else None,
        'w': w, 'h': h,
        'blocks': rows,
        'doors': [dict(d, x=d['x'] - x0, y=d['y'] - y0) for d in doors],
        'source': dict(meta, layout=layout.id, x=x0, y=y0),
    }
    return st


def auto_extract(project, verbose=False):
    P = project
    stamps = []
    for name in P.map_names():
        try:
            j = P.map_json(name)
            L = P.layout(j['layout'])
            blocks = L.blocks
        except Exception:
            continue
        if L.is_frlg or L.primary_symbol not in OUTDOOR_PRIMARIES:
            continue
        n_primary, prim, sec = load_classes(P, L)
        cls_fn = lambda m: class_of(m, n_primary, prim, sec)  # noqa: E731
        warps = j.get('warp_events') or []
        used = set()
        groups = []
        for wi, w in enumerate(warps):
            x, y = int(w['x']), int(w['y'])
            if not (0 <= x < L.width and 0 <= y < L.height):
                continue
            m = int(blocks[y, x]) & 0x3FF
            _, _, b, _ = P.metatile_info(L.primary_symbol, L.secondary_symbol, m)
            bname = P.behaviors.get(b, '')
            bk = DOOR_BEHAVIORS.get(bname)
            if not bk:
                continue
            if (x, y) in used:
                # second warp on an already traced building (double doors)
                for g in groups:
                    if (x, y) in g['cells']:
                        g['doors'].append({'x': x, 'y': y, 'behavior': bname, 'warp_index': wi,
                                           'dest_map': w['dest_map'], 'dest_warp_id': str(w['dest_warp_id'])})
                continue
            if bk == 'door':
                cells = trace_component(blocks, cls_fn, x, y)
            else:
                cells = {(x, y)} | {c for c in trace_component(blocks, cls_fn, x, y, max_up=2, max_side=2, max_down=1)}
            if cls_fn(m) != terrain.UNKNOWN:
                cells = {(x, y)}
            if len(cells) == 1 and bk in ('door', 'exit_south', 'exit_north', 'exit_east', 'exit_west'):
                # a lone door/exit tile (cave mouth, passage, exit gap): keep its frame so the
                # surrounding rock/wall matches when it is stamped somewhere else
                cells = {(xx, yy) for xx in range(x - 1, x + 2) for yy in range(y - 1, y + 1)
                         if 0 <= xx < L.width and 0 <= yy < L.height}
            used |= cells
            groups.append({'cells': cells, 'doors': [{'x': x, 'y': y, 'behavior': bname, 'warp_index': wi,
                                                      'dest_map': w['dest_map'],
                                                      'dest_warp_id': str(w['dest_warp_id'])}],
                           'bk': bk})
        for g in groups:
            if len(g['cells']) > 200:
                continue
            d0 = g['doors'][0]
            kind = kind_for(d0['dest_map'], g['bk'], in_cave=L.secondary_symbol in CAVE_SECONDARIES)
            st = make_stamp(P, L, g['cells'], g['doors'], {'map': name, 'dest_map': d0['dest_map']})
            st['kind'] = kind
            stamps.append(st)
    return stamps


def manual_extract(project):
    p = os.path.join(HERE, 'stamps_manual.json')
    if not os.path.exists(p):
        return [], {}
    out = []
    data = json.load(open(p))
    for e in data.get('stamps', []):
        L = project.layout(e['layout'])
        x0, y0, w, h = e['x'], e['y'], e['w'], e['h']
        mask = e.get('mask')  # list of strings, '.' = transparent, anything else = keep
        cells = set()
        for yy in range(h):
            for xx in range(w):
                if mask and mask[yy][xx] == '.':
                    continue
                cells.add((x0 + xx, y0 + yy))
        doors = [dict(d, x=d['x'] + x0, y=d['y'] + y0) for d in e.get('doors', [])]
        st = make_stamp(project, L, cells, doors, {'map': e.get('map', L.name), 'manual': True})
        # keep the declared rectangle even if the mask trims a border
        st['kind'] = e['kind']
        st['name'] = e['id']
        if e.get('notes'):
            st['notes'] = e['notes']
        if e.get('tags'):
            st['tags'] = e['tags']
        if 'force_secondary' in e:
            st['secondary'] = e['force_secondary']
        out.append(st)
    return out, {k: v for k, v in data.get('aliases', {}).items() if not k.startswith('_')}


def signature(st):
    return (st['secondary'], st['w'], st['h'],
            tuple(tuple((v & 0x3FF) if v is not None else -1 for v in r) for r in st['blocks']))


def snake(name):
    s = re.sub(r'([a-z])([A-Z])', r'\1_\2', name)
    return re.sub(r'[^A-Za-z0-9]+', '_', s).lower().strip('_')


def build(project, verbose=True):
    """Extract all stamps. Ids are stable: <family>/<kind>_<source map>[_<dest>|_<x>_<y>]."""
    autos = auto_extract(project)
    manual, aliases = manual_extract(project)
    lib = collections.OrderedDict()
    sigs = {}
    for st in manual + autos:
        sig = signature(st)
        if sig in sigs:
            other = sigs[sig]
            other.setdefault('also_in', []).append(st['source'].get('map'))
            continue
        fam = family_of(st['secondary'])
        if 'name' in st:
            sid = '%s/%s' % (fam, st.pop('name'))
        else:
            src = snake(st['source'].get('map', 'x'))
            sid = '%s/%s_%s' % (fam, st['kind'], src)
            if sid in lib:
                dest = snake(st['source'].get('dest_map', '').replace('MAP_', '').title().replace('_', ''))
                sid = '%s/%s_%s' % (fam, st['kind'], dest or src)
            if sid in lib:
                sid = '%s/%s_%s_%d_%d' % (fam, st['kind'], src, st['source']['x'], st['source']['y'])
        st['id'] = sid
        st['family'] = fam
        sigs[sig] = st
        lib[sid] = st
    for alias, target in aliases.items():
        if target not in lib:
            print('WARNING: alias %s -> %s: target missing' % (alias, target))
            continue
        st = dict(lib[target])
        st['id'] = alias
        st['alias_of'] = target
        st['family'] = alias.split('/')[0]
        lib[alias] = st
    os.makedirs(STAMP_DIR, exist_ok=True)
    byfam = collections.defaultdict(list)
    for st in lib.values():
        byfam[st['family']].append(st)
    for f in os.listdir(STAMP_DIR):
        if f.endswith('.json'):
            os.remove(os.path.join(STAMP_DIR, f))
    for fam, sts in byfam.items():
        with open(os.path.join(STAMP_DIR, fam + '.json'), 'w') as f:
            json.dump({'family': fam, 'stamps': sts}, f, indent=None, separators=(',', ':'))
            f.write('\n')
    if verbose:
        for fam, sts in sorted(byfam.items()):
            print('%-28s %3d stamps' % (fam, len(sts)))
    return lib


def load_library():
    lib = collections.OrderedDict()
    if not os.path.isdir(STAMP_DIR):
        return lib
    for f in sorted(os.listdir(STAMP_DIR)):
        if f.endswith('.json'):
            for st in json.load(open(os.path.join(STAMP_DIR, f)))['stamps']:
                lib[st['id']] = st
    return lib


def compatible(st, primary, secondary):
    return st['primary'] == primary and (st['secondary'] is None or st['secondary'] == secondary)


def render_stamp(project, st, secondary=None, scale=2, ground=None):
    """Render a stamp; transparent cells are drawn as `ground` metatile or a checkerboard."""
    import numpy as np
    from PIL import Image, ImageDraw
    sec = st['secondary'] or secondary or st['source'].get('secondary_hint') or 'gTileset_Petalburg'
    if st['secondary'] is None and secondary is None:
        # render general stamps in the context of the layout they came from
        try:
            sec = project.layout(st['source']['layout']).secondary_symbol
        except Exception:
            pass
    w, h = st['w'], st['h']
    img = Image.new('RGBA', (w * 16, h * 16), (0, 0, 0, 0))
    for y, row in enumerate(st['blocks']):
        for x, v in enumerate(row):
            if v is None:
                if ground is not None:
                    img.paste(project.render_metatile(st['primary'], sec, ground), (x * 16, y * 16))
                else:
                    t = Image.new('RGBA', (16, 16), (200, 200, 200, 255))
                    d = ImageDraw.Draw(t)
                    d.rectangle([0, 0, 7, 7], fill=(150, 150, 150, 255))
                    d.rectangle([8, 8, 15, 15], fill=(150, 150, 150, 255))
                    img.paste(t, (x * 16, y * 16))
            else:
                img.paste(project.render_metatile(st['primary'], sec, v & 0x3FF), (x * 16, y * 16))
    img = img.resize((w * 16 * scale, h * 16 * scale), Image.NEAREST)
    d = ImageDraw.Draw(img)
    for dr in st.get('doors', []):
        px, py = dr['x'] * 16 * scale, dr['y'] * 16 * scale
        d.rectangle([px + 1, py + 1, px + 16 * scale - 2, py + 16 * scale - 2], outline=(255, 0, 200, 255), width=2)
    return img


def previews(project, outdir, lib=None):
    from PIL import Image, ImageDraw
    lib = lib or load_library()
    os.makedirs(outdir, exist_ok=True)
    byfam = collections.defaultdict(list)
    for st in lib.values():
        byfam[st['family']].append(st)
    font = pexmap._font(11)
    for fam, sts in sorted(byfam.items()):
        fdir = os.path.join(outdir, fam)
        os.makedirs(fdir, exist_ok=True)
        tiles = []
        for st in sts:
            im = render_stamp(project, st, scale=2, ground=1 if st['primary'] == 'gTileset_General' and fam != 'cave' else None)
            im.save(os.path.join(fdir, st['id'].split('/')[1] + '.png'))
            tiles.append((st, render_stamp(project, st, scale=1, ground=None)))
        # contact sheet
        W = 1100
        x = y = 0
        rowh = 0
        placed = []
        for st, im in tiles:
            cw = max(im.width, 90) + 8
            ch = im.height + 26
            if x + cw > W:
                x = 0
                y += rowh
                rowh = 0
            placed.append((st, im, x, y))
            x += cw
            rowh = max(rowh, ch)
        sheet = Image.new('RGBA', (W, y + rowh + 4), (40, 40, 40, 255))
        d = ImageDraw.Draw(sheet)
        for st, im, x, y in placed:
            sheet.paste(im, (x + 2, y + 2), im)
            d.text((x + 2, y + im.height + 3), st['id'].split('/')[1], fill=(255, 255, 0, 255), font=font)
            d.text((x + 2, y + im.height + 13), '%dx%d' % (st['w'], st['h']), fill=(200, 200, 200, 255), font=font)
        sheet.save(os.path.join(outdir, 'sheet_%s.png' % fam))
        print('%s: %d previews' % (fam, len(sts)))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('cmd', choices=['build', 'previews', 'list'])
    ap.add_argument('--root', default=None,
                    help='tree to read (build: default /home/user/pex-orig, the pristine source; '
                         'previews: default %s)' % DEFAULT_ROOT)
    ap.add_argument('--out', default=DEFAULT_OUT)
    ap.add_argument('--family')
    a = ap.parse_args()
    root = a.root or ('/home/user/pex-orig' if a.cmd == 'build' and os.path.isdir('/home/user/pex-orig') else DEFAULT_ROOT)
    P = pexmap.Project(root)
    if a.cmd == 'build':
        build(P)
    elif a.cmd == 'previews':
        previews(P, a.out)
    else:
        for sid, st in load_library().items():
            if a.family and st['family'] != a.family and st['family'] != 'general':
                continue
            doors = ' '.join('door@%d,%d' % (d['x'], d['y']) for d in st.get('doors', []))
            print('%-40s %2dx%-2d %-12s %s  (from %s)' % (sid, st['w'], st['h'], st['kind'], doors,
                                                     st['source'].get('map')))


if __name__ == '__main__':
    main()
