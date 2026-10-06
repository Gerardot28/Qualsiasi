#!/usr/bin/env python3
"""Split big C string files into N parts at safe entry boundaries (for parallel translation).
usage: split_c.py FILE N    (FILE relative to /home/user/pex-orig) ; join: split_c.py --join FILE N DEST_ROOT"""
import re, sys
ROOT = '/home/user/pex-orig'; WORK = '/home/user/work/loc/parts'
def safe(lines, i):
    prev = lines[i-1].rstrip()
    cur = lines[i]
    return (prev.endswith('),') or prev.endswith('");') or prev.endswith('};') or prev == '') and \
           re.match(r'^(\s*\[[A-Z_0-9]+\]\s*=|(static )?const u8 |const u8 \*|$)', cur) is not None
if sys.argv[1] == '--join':
    f, n, dest = sys.argv[2], int(sys.argv[3]), sys.argv[4]
    flat = f.replace('/', '__')
    open(f'{dest}/{f}', 'w', encoding='utf-8').write(''.join(open(f'{WORK}/new/{flat}.part{k}', encoding='utf-8').read() for k in range(n)))
    print('joined', f); sys.exit()
f, n = sys.argv[1], int(sys.argv[2])
lines = open(f'{ROOT}/{f}', encoding='utf-8').read().splitlines(keepends=True)
cuts = [0]
for k in range(1, n):
    i = len(lines) * k // n
    while i < len(lines) and not safe(lines, i): i += 1
    cuts.append(i)
cuts.append(len(lines))
flat = f.replace('/', '__')
for k in range(n):
    chunk = ''.join(lines[cuts[k]:cuts[k+1]])
    for d in ('orig', 'new'):
        open(f'{WORK}/{d}/{flat}.part{k}', 'w', encoding='utf-8').write(chunk)
    print(f'{flat}.part{k}', cuts[k], cuts[k+1])
