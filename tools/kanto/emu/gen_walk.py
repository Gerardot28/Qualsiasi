#!/usr/bin/env python3
"""Generate an emu harness script that tests every warp (door/stairs/mat) and every map connection of the
ported Kanto maps by spawning the player next to it and walking into it. Usage: gen_walk.py TREE OUTDIR"""
import json, os, re, struct, sys

root, out = sys.argv[1], sys.argv[2]
hdr = open(root + '/include/constants/map_groups.h').read()
ids = {m.group(1): (int(m.group(3)), int(m.group(2))) for m in re.finditer(r'(MAP_\w+)\s*=\s*\((\d+) \| \((\d+) << 8\)\)', hdr)}
L = {l['id']: l for l in json.load(open(root + '/data/layouts/layouts.json'))['layouts'] if 'id' in l}
rep = json.load(open(root + '/data/kanto_port_report.json'))
ported = set(rep['ported_maps'])
maps = {n: json.load(open('%s/data/maps/%s/map.json' % (root, n))) for n in ported}
byid = {d['id']: n for n, d in maps.items()}
grid = {}


def tiles(n):
    if n not in grid:
        lay = L[maps[n]['layout']]
        raw = open(root + '/' + lay['blockdata_filepath'], 'rb').read()
        w, h = lay['width'], lay['height']
        grid[n] = (w, h, struct.unpack('<%dH' % (w * h), raw[:2 * w * h]))
    return grid[n]


def free(n, x, y):
    w, h, t = tiles(n)
    if not (0 <= x < w and 0 <= y < h):
        return False
    if (t[y * w + x] >> 10) & 3:
        return False
    d = maps[n]
    if any(o.get('x') == x and o.get('y') == y for o in d.get('object_events') or []):
        return False
    return not any(wp['x'] == x and wp['y'] == y for wp in d.get('warp_events') or [])


tests = []  # (name, map, x, y, key, expect_map)
for n in sorted(ported):
    d = maps[n]
    for i, wp in enumerate(d.get('warp_events') or []):
        if 'kanto_port_orig_dest_map' in wp or wp['dest_map'] == d['id']:
            continue
        for dx, dy, key in ((0, 1, 'UP'), (0, -1, 'DOWN'), (-1, 0, 'RIGHT'), (1, 0, 'LEFT')):
            if free(n, wp['x'] + dx, wp['y'] + dy):
                tests.append(('warp%d' % i, n, wp['x'] + dx, wp['y'] + dy, key, wp['dest_map']))
                break
        else:
            tests.append(('warp%d' % i, n, None, None, None, wp['dest_map']))
    for c in d.get('connections') or []:
        if c['map'] not in byid:
            continue
        w, h, _ = tiles(n)
        nn = byid[c['map']]
        nw, nh, _ = tiles(nn)
        off = c['offset']
        cand = None
        if c['direction'] in ('up', 'down'):
            for x in range(w):
                y0, y1 = (0, 1) if c['direction'] == 'up' else (h - 1, h - 2)
                ny = nh - 1 if c['direction'] == 'up' else 0
                if free(n, x, y0) and free(n, x, y1) and free(nn, x - off, ny):
                    cand = (x, y1, 'UP' if c['direction'] == 'up' else 'DOWN')
                    break
        else:
            for y in range(h):
                x0, x1 = (0, 1) if c['direction'] == 'left' else (w - 1, w - 2)
                nx = nw - 1 if c['direction'] == 'left' else 0
                if free(n, x0, y) and free(n, x1, y) and free(nn, nx, y - off):
                    cand = (x1, y, 'LEFT' if c['direction'] == 'left' else 'RIGHT')
                    break
        tests.append(('conn_' + c['direction'], n, cand and cand[0], cand and cand[1], cand and cand[2], c['map']))

lines = ['loadstate /home/user/work/kanto-out/emu/base/base.state', 'wait 30']
plan = []
for t in tests:
    name, n, x, y, key, exp = t
    if x is None:
        plan.append(t + ('NOSPOT',))
        continue
    g, num = ids[maps[n]['id']]
    lines += ['echo TEST %d %s %s' % (len(plan), n, name),
              'write32 gKantoTestWarpXY %d' % (x | (y << 16)),
              'write32 gKantoTestWarp %d' % (0x80000000 | (g << 8) | num),
              'wait 110', 'hold %s 40' % key, 'wait 130', 'mapinfo']
    plan.append(t + ('RUN',))
open(out + '/walk.txt', 'w').write('\n'.join(lines) + '\n')
json.dump(plan, open(out + '/walk_plan.json', 'w'))
print('tests', len(plan), 'runnable', sum(1 for p in plan if p[-1] == 'RUN'))
