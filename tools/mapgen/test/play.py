#!/usr/bin/env python3
"""Closed-loop emulator driver for map tests (uses tools/emu's emu-harness).

Every call to Emu.run() boots the harness from the previous savestate, runs a
short input script, reads the player position / map from RAM and saves a new
state, so a test can look at where the player really is and re-plan.

    from play import Emu
    e = Emu('/home/user/work/mapgen-tree', out='/home/user/work/mapgen-out/emu')
    e.boot_new_game()               # needs the test start patch (test_start_patch.py)
    e.walk_to(14, 8)                # BFS on the real collision map + NPCs, re-plans
    e.shot('littleroot')
"""
import json
import os
import re
import shutil
import subprocess
import sys
from collections import deque

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))
import pexmap  # noqa: E402

HARNESS = os.environ.get('EMU_HARNESS', '/home/user/work/emu-build/bin/emu-harness')
EMU_SCRIPTS = '/home/user/Qualsiasi/tools/emu/scripts'
DIRS = {'UP': (0, -1), 'DOWN': (0, 1), 'LEFT': (-1, 0), 'RIGHT': (1, 0)}


class EmuError(Exception):
    pass


class Emu:
    def __init__(self, tree, out, rom=None, verbose=True):
        self.tree = tree
        self.rom = rom or os.path.join(tree, 'pokeemerald.gba')
        self.out = out
        os.makedirs(out, exist_ok=True)
        self.state = None
        # continue numbering of an existing output dir (states are never overwritten)
        nums = [int(m.group(1)) for f in os.listdir(out) for m in [re.match(r'state_(\d+)\.ss$', f)] if m]
        self.n = max(nums) if nums else 0
        self.verbose = verbose
        self.P = pexmap.Project(tree)
        self.syms = os.path.join(out, 'syms.txt')
        elf = os.path.join(tree, 'pokeemerald.elf')
        nm = shutil.which('arm-none-eabi-nm') or 'nm'
        with open(self.syms, 'w') as f:
            subprocess.run([nm, elf], stdout=f, check=True)
        self.map_ids = self._map_ids()
        self.log = open(os.path.join(out, 'play.log'), 'a')

    def _map_ids(self):
        ids = {}
        txt = open(os.path.join(self.tree, 'include/constants/map_groups.h')).read()
        for name, num, grp in re.findall(r'(MAP_\w+)\s*=\s*\((\d+)\s*\|\s*\((\d+)\s*<<\s*8\)\)', txt):
            ids[(int(grp), int(num))] = name
        return ids

    def say(self, *a):
        msg = ' '.join(str(x) for x in a)
        if self.verbose:
            print(msg, flush=True)
        self.log.write(msg + '\n')
        self.log.flush()

    # ------------------------------------------------------------------ raw harness
    def run(self, lines, tag='step', load=True):
        self.n += 1
        name = '%03d_%s' % (self.n, tag)
        script = os.path.join(self.out, name + '.txt')
        newstate = os.path.join(self.out, 'state_%03d.ss' % self.n)
        body = list(lines) + [
            'read16 [gSaveBlock1Ptr]+0 px', 'read16 [gSaveBlock1Ptr]+2 py',
            'read8 [gSaveBlock1Ptr]+4 mg', 'read8 [gSaveBlock1Ptr]+5 mn',
            'read32 gMain+4 cb2',
        ]
        with open(script, 'w') as f:
            f.write('\n'.join(body) + '\n')
        cmd = [HARNESS, '--rom', self.rom, '--script', script, '--out', self.out, '--symbols', self.syms,
               '--save-state-out', newstate]
        if load and self.state:
            cmd += ['--load-state', self.state]
        p = subprocess.run(cmd, capture_output=True, text=True, cwd=EMU_SCRIPTS)
        outp = p.stdout + p.stderr
        with open(os.path.join(self.out, name + '.log'), 'w') as f:
            f.write(outp)
        if 'EVENT stuck' in outp:
            self.say('  (harness reported a "stuck" event - no VBlank wait for a while; continuing)')
        bad = p.returncode not in (0, 3, 4) or 'EVENT crash' in outp or 'EVENT reset' in outp
        if p.returncode == 4 and 'EVENT stuck' not in outp:
            bad = True
        if bad:
            raise EmuError('harness failed (%d): %s' % (p.returncode, outp[-2000:]))
        reads = {}
        for m in re.finditer(r'READ\s.*?value=(0x[0-9a-fA-F]+).*?label=(\w+)', outp):
            reads[m.group(2)] = int(m.group(1), 16)
        self.state = newstate
        self.last = reads
        self.last_out = outp
        return reads

    def where(self):
        r = self.last
        x, y = r['px'], r['py']
        x = x - 0x10000 if x >= 0x8000 else x
        y = y - 0x10000 if y >= 0x8000 else y
        return self.map_ids.get((r['mg'], r['mn']), '?%d.%d' % (r['mg'], r['mn'])), x, y

    def refresh(self):
        self.run(['wait 1'], 'where')
        return self.where()

    def shot(self, name, extra_wait=0):
        lines = ['wait %d' % extra_wait] if extra_wait else []
        self.run(lines + ['shot %s' % name], 'shot_' + name)
        p = os.path.join(self.out, name + '.png')
        self.say('  screenshot', p)
        return p

    # ------------------------------------------------------------------ game flow
    def boot_new_game(self):
        """Power on -> NEW GAME -> Birch speech (tools/emu robust script) -> overworld.

        Needs the test start patch (test_start_patch.py): the truck is skipped and
        the player appears directly on the map chosen by the patch."""
        script = os.path.join(HERE, 'boot_new_game.txt')
        self.n += 1
        state = os.path.join(self.out, 'state_%03d.ss' % self.n)
        p = subprocess.run([sys.executable, '-I', os.path.join(os.path.dirname(EMU_SCRIPTS), 'run.py'),
                            '--rom', self.rom, '--script', script, '--out', os.path.join(self.out, 'boot'),
                            '--save-state-out', state, '--elf', os.path.join(self.tree, 'pokeemerald.elf')],
                           capture_output=True, text=True)
        with open(os.path.join(self.out, '%03d_boot.log' % self.n), 'w') as f:
            f.write(p.stdout + p.stderr)
        if p.returncode not in (0, 3):
            raise EmuError('boot failed: ' + (p.stdout + p.stderr)[-3000:])
        self.state = state
        self.refresh()
        self.say('booted:', self.where())

    # ------------------------------------------------------------------ navigation
    def map_json_by_id(self, mapname):
        if not hasattr(self, '_mapcache'):
            self._mapcache = {}
            for n in self.P.map_names():
                try:
                    jj = self.P.map_json(n)
                except Exception:
                    continue
                self._mapcache[jj.get('id')] = (n, jj)
        return self._mapcache[mapname]

    def grid_for(self, mapname):
        """walkable grid of the current map from the tree (collision + objects)."""
        folder, j = self.map_json_by_id(mapname)
        L = self.P.layout(j['layout'])
        b = L.blocks
        h, w = b.shape
        coll = ((b >> 10) & 3) != 0
        block = set()
        for o in j.get('object_events', []):
            block.add((int(o['x']), int(o['y'])))
        beh = {}
        for y in range(h):
            for x in range(w):
                _, _, bb, _ = self.P.metatile_info(L.primary_symbol, L.secondary_symbol, int(b[y, x]) & 0x3FF)
                beh[(x, y)] = self.P.behaviors.get(bb, '')
        return coll, block, beh, j

    def plan(self, mapname, start, goal, avoid_warps=True, extra_block=()):
        coll, block, beh, j = self.grid_for(mapname)
        h, w = coll.shape
        warps = {(int(wv['x']), int(wv['y'])) for wv in j.get('warp_events', [])}
        block = set(block) | set(extra_block)
        prev = {start: None}
        q = deque([start])
        while q:
            c = q.popleft()
            if c == goal:
                break
            for d, (dx, dy) in DIRS.items():
                n = (c[0] + dx, c[1] + dy)
                if n in prev:
                    continue
                if n != goal:
                    if not (0 <= n[0] < w and 0 <= n[1] < h):
                        continue
                    if coll[n[1], n[0]] or n in block:
                        continue
                    if avoid_warps and n in warps:
                        continue
                    if beh.get(n, '').startswith('MB_TALL_GRASS') and n != goal:
                        pass
                prev[n] = (c, d)
                q.append(n)
        if goal not in prev:
            return None
        path = []
        c = goal
        while prev[c] is not None:
            c, d = prev[c][0], prev[c][1]
            path.append(d)
        return list(reversed(path))

    def walk_to(self, gx, gy, max_tries=8, frames_per_tile=16, final_wait=20):
        for attempt in range(max_tries):
            m, x, y = self.where()
            if (x, y) == (gx, gy):
                return True
            path = self.plan(m, (x, y), (gx, gy))
            if path is None:
                raise EmuError('no path on %s from %s to %s' % (m, (x, y), (gx, gy)))
            # group straight runs
            lines = []
            i = 0
            while i < len(path):
                d = path[i]
                k = 1
                while i + k < len(path) and path[i + k] == d:
                    k += 1
                lines.append('hold %s %d' % (d, frames_per_tile * k))
                i += k
            lines.append('wait %d' % final_wait)
            self.run(lines, 'walk_%d_%d' % (gx, gy))
            m2, x2, y2 = self.where()
            self.say('  walk %s (%d,%d)->(%d,%d): now %s (%d,%d)' % (m, x, y, gx, gy, m2, x2, y2))
            if m2 != m:
                return True   # warped / changed map on the way
            if (x2, y2) == (gx, gy):
                return True
            if (x2, y2) == (x, y):
                self.run(['wait 30'], 'unblock')
        return False

    def step(self, direction, n=1, wait=20, frames_per_tile=16):
        self.run(['hold %s %d' % (direction, frames_per_tile * n), 'wait %d' % wait], 'step_%s' % direction)
        return self.where()

    def face(self, direction):
        self.run(['press %s 3 12' % direction], 'face_%s' % direction)

    def press(self, key, n=1, gap=30):
        lines = []
        for _ in range(n):
            lines += ['press %s' % key, 'wait %d' % gap]
        self.run(lines, 'press_%s' % key)

    def talk(self, shotname, presses=6):
        """Press A, screenshot the first text box, then mash through the dialogue."""
        self.run(['press A', 'wait 12', 'waitstable 8 200', 'shot %s' % shotname], 'talk')
        self.say('  screenshot', os.path.join(self.out, shotname + '.png'))
        lines = []
        for _ in range(presses):
            lines += ['press B', 'wait 40']   # B advances/closes text without re-talking
        self.run(lines, 'talk_end')
