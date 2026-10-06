#!/usr/bin/env python3
"""
emu_drive.py - drive mGBA-Qt headlessly (Xvfb + xdotool) and take emulator screenshots.

Usage:
    xvfb-run -a -s "-screen 0 1280x1024x24" python3 emu_drive.py ROM.gba STEPS.txt OUTDIR

STEPS.txt is a list of commands, one per line ('#' starts a comment):
    wait SECONDS              sleep (real time; the emulator runs at 1x)
    tap KEY [N] [GAP_MS]      press KEY N times (default 1), GAP_MS between presses (default 250)
    hold KEY MS               hold KEY for MS milliseconds
    face DIR                  short tap: turn in place (steps once if already facing DIR)
    walk DIR N                N single steps (player must already face DIR)
    combo KEY1+KEY2 [MS]      hold several keys together (e.g. R+START for the debug menu)
    shot NAME                 save the emulator framebuffer (240x160) as OUTDIR/NAME.png (mGBA F12)
    fastforward on|off        toggle mGBA fast-forward (Shift+Tab)
    dump ADDR LEN NAME        (with EMU_GDB=1) save LEN bytes of GBA memory at ADDR (hex) to OUTDIR/NAME.bin

GBA keys (mGBA-Qt default keyboard map): A=x  B=z  L=a  R=s  START=Return  SELECT=BackSpace
UP/DOWN/LEFT/RIGHT = arrows. You can use the GBA names (A, B, L, R, START, SELECT, UP, ...).
"""
import os
import shutil
import subprocess
import sys
import tempfile
import time

KEYMAP = {
    'A': 'x', 'B': 'z', 'L': 'a', 'R': 's', 'START': 'Return', 'SELECT': 'BackSpace',
    'UP': 'Up', 'DOWN': 'Down', 'LEFT': 'Left', 'RIGHT': 'Right',
}


class Gdb:
    """Minimal GDB remote-serial-protocol client for mGBA's gdb stub (mgba-qt -g, port 2345)."""
    def __init__(self, port=2345):
        import socket
        for _ in range(50):
            try:
                self.s = socket.create_connection(('127.0.0.1', port), timeout=5)
                break
            except OSError:
                time.sleep(0.2)
        self.buf = b''

    def _recv_packet(self):
        while True:
            while b'#' not in self.buf or len(self.buf) < self.buf.index(b'#') + 3:
                self.buf += self.s.recv(65536)
            start = self.buf.find(b'$')
            end = self.buf.index(b'#')
            data = self.buf[start + 1:end]
            self.buf = self.buf[end + 3:]
            self.s.sendall(b'+')
            return data

    def send(self, data, expect_reply=True):
        pkt = b'$' + data + b'#' + ('%02x' % (sum(data) & 0xff)).encode()
        self.s.sendall(pkt)
        # wait for ack
        while not self.buf:
            self.buf += self.s.recv(65536)
        if self.buf[:1] == b'+':
            self.buf = self.buf[1:]
        return self._recv_packet() if expect_reply else None

    def cont(self):
        self.send(b'c', expect_reply=False)

    def interrupt(self):
        self.s.sendall(b'\x03')
        r = self._recv_packet()
        while not (r.startswith(b'S') or r.startswith(b'T')):
            r = self._recv_packet()
        return r

    def read(self, addr, length):
        out = b''
        while length > 0:
            n = min(length, 0x100)
            r = self.send(b'm%x,%x' % (addr, n))
            while len(r) != 2 * n:  # skip stray stop/notification packets
                if r.startswith(b'E'):
                    raise RuntimeError('gdb read error %r at %x' % (r, addr))
                r = self._recv_packet()
            out += bytes.fromhex(r.decode())
            addr += n
            length -= n
        return out


def xdo(*args):
    subprocess.run(['xdotool', *args], check=False)


def keyname(k):
    return KEYMAP.get(k.upper(), k)


def main():
    if len(sys.argv) != 4:
        print(__doc__)
        sys.exit(1)
    rom, steps_file, outdir = sys.argv[1:4]
    os.makedirs(outdir, exist_ok=True)
    work = tempfile.mkdtemp(prefix='emu_')
    wrom = os.path.join(work, 'game.gba')
    shutil.copy(rom, wrom)
    log = open(os.path.join(work, 'mgba.log'), 'w')
    use_gdb = os.environ.get('EMU_GDB') == '1'
    cmdline = ['/usr/games/mgba-qt', '-C', 'displayDriver=0', '-C', 'audioSync=0', '-C', 'videoSync=1', '-C', 'mute=1', '-2']
    if use_gdb:
        cmdline.append('-g')
    proc = subprocess.Popen(cmdline + [wrom], stdout=log, stderr=subprocess.STDOUT)
    gdb = None
    if use_gdb:
        time.sleep(2)
        gdb = Gdb()
        gdb.cont()
    wid = None
    for _ in range(50):
        time.sleep(0.2)
        r = subprocess.run(['xdotool', 'search', '--onlyvisible', '--name', 'mGBA - '], capture_output=True, text=True)
        if r.stdout.strip():
            wid = r.stdout.split()[0]
            break
    if wid is None:
        print('mGBA window not found')
        proc.kill()
        sys.exit(2)
    time.sleep(1.0)
    for _ in range(10):
        r = subprocess.run(['xdotool', 'windowfocus', '--sync', wid], capture_output=True, text=True)
        if r.returncode == 0:
            break
        time.sleep(0.5)
    time.sleep(0.5)
    shot_index = 0
    try:
        for raw in open(steps_file):
            line = raw.split('#', 1)[0].strip()
            if not line:
                continue
            parts = line.split()
            cmd = parts[0].lower()
            if cmd == 'wait':
                time.sleep(float(parts[1]))
            elif cmd == 'tap':
                k = keyname(parts[1])
                n = int(parts[2]) if len(parts) > 2 else 1
                gap = int(parts[3]) if len(parts) > 3 else 250
                for _ in range(n):
                    xdo('keydown', k)
                    time.sleep(0.07)
                    xdo('keyup', k)
                    time.sleep(gap / 1000.0)
            elif cmd == 'hold':
                k = keyname(parts[1])
                xdo('keydown', k)
                time.sleep(int(parts[2]) / 1000.0)
                xdo('keyup', k)
                time.sleep(0.05)
            elif cmd == 'face':
                # turn in place (only meaningful when not already facing that way: it would step otherwise)
                k = keyname(parts[1])
                xdo('keydown', k)
                time.sleep(0.04)
                xdo('keyup', k)
                time.sleep(0.3)
            elif cmd == 'walk':
                # N single steps in the direction the player already faces
                k = keyname(parts[1])
                for _ in range(int(parts[2]) if len(parts) > 2 else 1):
                    xdo('keydown', k)
                    time.sleep(0.15)
                    xdo('keyup', k)
                    time.sleep(0.22)
            elif cmd == 'combo':
                keys = [keyname(k) for k in parts[1].split('+')]
                ms = int(parts[2]) if len(parts) > 2 else 150
                for k in keys:
                    xdo('keydown', k)
                    time.sleep(0.05)
                time.sleep(ms / 1000.0)
                for k in reversed(keys):
                    xdo('keyup', k)
                time.sleep(0.05)
            elif cmd == 'dump':
                # dump ADDR LEN NAME  (needs EMU_GDB=1)
                gdb.interrupt()
                data = gdb.read(int(parts[1], 16), int(parts[2], 16))
                gdb.cont()
                with open(os.path.join(outdir, parts[3] + '.bin'), 'wb') as f:
                    f.write(data)
                print('dump', parts[3], len(data))
            elif cmd == 'fastforward':
                xdo('key', 'shift+Tab')
            elif cmd == 'shot':
                xdo('key', 'F12')
                time.sleep(0.4)
                src = None
                for _ in range(25):
                    found = sorted(f for f in os.listdir(work) if f.startswith('game-') and f.endswith('.png'))
                    if found:
                        src = os.path.join(work, found[0])
                        break
                    time.sleep(0.2)
                if src:
                    time.sleep(0.2)
                    shutil.move(src, os.path.join(outdir, parts[1] + '.png'))
                    print('shot', parts[1])
                else:
                    print('shot FAILED', parts[1])
                shot_index += 1
            else:
                print('unknown command', line)
    finally:
        proc.kill()
        proc.wait()
        shutil.rmtree(work, ignore_errors=True)


if __name__ == '__main__':
    main()
