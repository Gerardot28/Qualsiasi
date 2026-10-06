#!/usr/bin/env python3
"""Render pokeemerald-expansion maps / layouts / tileset sheets to PNG.

Examples
  render.py --root TREE --map LittlerootTown -o out.png --events --grid
  render.py --root TREE --layout LAYOUT_ROUTE101 --collision -o r101.png
  render.py --root TREE --sheet gTileset_General gTileset_Petalburg --which secondary -o sheet.png
  render.py --root TREE --layout LittlerootTown --crop 2,3,6,5 -o crop.png   (x,y,w,h in blocks)
"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pexmap  # noqa: E402

DEFAULT_ROOT = os.environ.get('PEX_ROOT', '/home/user/work/mapgen-tree')


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--root', default=DEFAULT_ROOT, help='source tree (default %(default)s or $PEX_ROOT)')
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument('--map', help='map folder name under data/maps (e.g. LittlerootTown)')
    g.add_argument('--layout', help='layout id / name (LAYOUT_X, X_Layout or X)')
    g.add_argument('--sheet', nargs=2, metavar=('PRIMARY', 'SECONDARY'), help='render a metatile sheet')
    ap.add_argument('--which', default='secondary', choices=['primary', 'secondary'])
    ap.add_argument('-o', '--out', required=True)
    ap.add_argument('--scale', type=int, default=1)
    ap.add_argument('--collision', action='store_true', help='tint impassable blocks red')
    ap.add_argument('--behaviors', action='store_true', help='print non-zero behavior ids (hex)')
    ap.add_argument('--grid', action='store_true')
    ap.add_argument('--coords', action='store_true', help='print x/y block coordinates')
    ap.add_argument('--events', action='store_true', help='draw events from map.json (only with --map)')
    ap.add_argument('--border', type=int, default=0, help='draw N blocks of border around the map')
    ap.add_argument('--crop', help='x,y,w,h crop in blocks (before scaling)')
    a = ap.parse_args(argv)

    P = pexmap.Project(a.root)
    if a.sheet:
        img = pexmap.render_tileset_sheet(P, a.sheet[0], a.sheet[1], which=a.which, scale=max(1, a.scale))
        img.save(a.out)
        print(a.out)
        return
    mapj = None
    if a.map:
        mapj = P.map_json(a.map)
        layout = P.layout(mapj['layout'])
    else:
        layout = P.layout(a.layout)
    blocks = layout.blocks
    pad = a.border
    if a.crop:
        x, y, w, h = (int(v) for v in a.crop.split(','))
        blocks = blocks[y:y + h, x:x + w]
        pad = 0
    img = pexmap.render_blocks(P, layout.primary_symbol, layout.secondary_symbol, blocks, scale=a.scale,
                               collision=a.collision, grid=a.grid, coords=a.coords, border=layout.border,
                               border_pad=pad, behaviors=a.behaviors)
    if a.events and mapj is not None and not a.crop:
        pexmap.draw_events(img, mapj, scale=a.scale, pad=pad)
    os.makedirs(os.path.dirname(os.path.abspath(a.out)), exist_ok=True)
    img.save(a.out)
    print('%s  (%s %dx%d, %s + %s)' % (a.out, layout.name, layout.width, layout.height,
                                       layout.primary_symbol, layout.secondary_symbol))


if __name__ == '__main__':
    main()
