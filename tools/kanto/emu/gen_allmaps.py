#!/usr/bin/env python3
"""gen_allmaps.py TREE OUTDIR - harness script that warps (test hook gKantoTestWarp) to every ported Kanto map
and takes a screenshot. Needs a ROM built with port_kanto.py --test-start and a base state (base.txt)."""
import json, re, sys
root, out = sys.argv[1], sys.argv[2]
hdr = open(root + '/include/constants/map_groups.h').read()
ids = {m.group(1): (int(m.group(3)), int(m.group(2))) for m in re.finditer(r'(MAP_\w+)\s*=\s*\((\d+) \| \((\d+) << 8\)\)', hdr)}
L = {l['id']: l for l in json.load(open(root + '/data/layouts/layouts.json'))['layouts'] if 'id' in l}
lines = ['loadstate /home/user/work/kanto-out/emu/base/base.state', 'wait 30']
order = []
for n in json.load(open(root + '/data/kanto_port_report.json'))['ported_maps']:
    d = json.load(open('%s/data/maps/%s/map.json' % (root, n)))
    g, num = ids[d['id']]
    ws = d.get('warp_events') or []
    x, y = (ws[0]['x'], ws[0]['y']) if ws else (L[d['layout']]['width'] // 2, L[d['layout']]['height'] // 2)
    order.append((n, d['id'], g, num, x, y))
    lines += ['echo WARPTO %s %s' % (n, d['id']), 'write32 gKantoTestWarpXY %d' % (x | (y << 16)),
              'write32 gKantoTestWarp %d' % (0x80000000 | (g << 8) | num), 'wait 140', 'mapinfo', 'shot %s' % n]
open(out + '/allmaps.txt', 'w').write('\n'.join(lines) + '\n')
json.dump(order, open(out + '/allmaps.json', 'w'))
print(len(order), 'maps')
