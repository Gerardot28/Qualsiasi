#!/usr/bin/env python3
"""Boot a ROM headlessly and grab native 240x160 frames of the title screen.

Fallback capture path used while the shared emulator harness
(tools/emu/run.py) is not available.  It runs the SDL mGBA frontend
(/usr/games/mgba) inside a private Xvfb, skips the intro with START
(Return) and presses F12 (mGBA's "take screenshot" key) at fixed times,
which writes exact framebuffer PNGs (240x160, no window chrome/scaling).

Note: /usr/games/mgba-qt renders a blank widget under Xvfb (no usable
GL/QPainter surface), so the SDL frontend is used instead.

Usage:
  capture_title.py ROM.gba OUTDIR [--skip-at 9.0] [--start 0.3] \
      [--every 0.25] [--count 48] [--prefix title]

Frames are written as OUTDIR/<prefix>_NNN.png (NNN = shot index) and a
timeline.txt with the wall-clock offset (seconds after START was pressed)
of every shot.  Nothing is written next to the ROM: it is copied into a
temporary directory first.
"""
import argparse
import os
import shutil
import subprocess
import sys
import tempfile
import textwrap

MGBA = "/usr/games/mgba"


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("rom")
    ap.add_argument("outdir")
    ap.add_argument("--skip-at", type=float, default=9.0,
                    help="seconds after boot to press START (skips the intro)")
    ap.add_argument("--start", type=float, default=0.3,
                    help="seconds after START before the first screenshot")
    ap.add_argument("--every", type=float, default=0.25,
                    help="seconds between screenshots")
    ap.add_argument("--count", type=int, default=48, help="number of screenshots")
    ap.add_argument("--press", action="append", default=[],
                    help="extra key presses as SECONDS:KEY (xdotool key name, "
                         "e.g. 3.0:Return), times relative to START")
    ap.add_argument("--prefix", default="title")
    args = ap.parse_args()

    if not os.path.exists(MGBA):
        sys.exit(f"{MGBA} not found")
    os.makedirs(args.outdir, exist_ok=True)
    work = tempfile.mkdtemp(prefix="gfxcap_")
    try:
        shutil.copy(args.rom, os.path.join(work, "rom.gba"))
        os.makedirs(os.path.join(work, "cfg"), exist_ok=True)
        # Build the timed key script: (time, key) relative to START press.
        events = [(i * args.every + args.start, "F12") for i in range(args.count)]
        for p in args.press:
            t, k = p.split(":", 1)
            events.append((float(t), k))
        events.sort()
        lines = []
        now = 0.0
        for t, k in events:
            if t > now:
                lines.append(f"sleep {t - now:.3f}")
                now = t
            if k == "F12":
                lines.append("xdotool key F12")  # frontend hotkey: a tap is enough
            else:  # game input is polled once per frame: hold the key ~100 ms
                lines.append(f"xdotool keydown {k}; sleep 0.1; xdotool keyup {k}")
            if k == "F12":
                lines.append(f'echo "{now:.3f}" >> times.txt')
        script = textwrap.dedent(f"""\
            #!/bin/bash
            cd "{work}"
            export XDG_CONFIG_HOME="{work}/cfg" SDL_AUDIODRIVER=dummy
            {MGBA} -2 rom.gba > mgba.log 2>&1 &
            PID=$!
            sleep 2
            xdotool mousemove 240 160
            sleep {max(args.skip_at - 2, 0):.3f}
            xdotool keydown Return; sleep 0.1; xdotool keyup Return
            """) + "\n".join(lines) + "\nsleep 1\nkill $PID\nwait $PID 2>/dev/null\n"
        sh = os.path.join(work, "run.sh")
        with open(sh, "w") as f:
            f.write(script)
        os.chmod(sh, 0o755)
        subprocess.run(["xvfb-run", "-a", "-s", "-screen 0 640x480x24", sh],
                       check=False, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                       timeout=args.skip_at + now + 60)
        times = []
        tp = os.path.join(work, "times.txt")
        if os.path.exists(tp):
            times = [l.strip() for l in open(tp)]
        shots = sorted((f for f in os.listdir(work) if f.startswith("rom-") and f.endswith(".png")),
                       key=lambda s: int(s[4:-4]))
        with open(os.path.join(args.outdir, f"{args.prefix}_timeline.txt"), "w") as tl:
            for i, s in enumerate(shots):
                dst = f"{args.prefix}_{i:03d}.png"
                shutil.copy(os.path.join(work, s), os.path.join(args.outdir, dst))
                tl.write(f"{dst} t={times[i] if i < len(times) else '?'}\n")
        print(f"{len(shots)} frames -> {args.outdir}")
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    main()
