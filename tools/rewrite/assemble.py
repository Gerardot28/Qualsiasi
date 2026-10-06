#!/usr/bin/env python3
"""Stitch rewritten part files back into the working tree (/home/user/pex)."""
import json, sys
WORK = '/home/user/work/rewrite'
DEST = sys.argv[1] if len(sys.argv) > 1 else '/home/user/pex'
d = json.load(open(f'{WORK}/units.json'))
for f, parts in d['splits'].items():
    text = ''.join(open(f'{WORK}/parts/new/{p}', encoding='utf-8').read() for p in parts)
    open(f'{DEST}/{f}', 'w', encoding='utf-8').write(text)
    print('assembled', f, len(parts), 'parts')
