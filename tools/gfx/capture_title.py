#!/usr/bin/env python3
"""Boot a ROM headlessly and grab native 240x160 frames of the title screen.

Backends
  harness  (preferred) the shared deterministic runner tools/emu/run.py
           (libmgba, frame-accurate): boots, presses START at frame 600 to
           skip the intro, then shoots every --every frames.  Needs the
           harness binary (tools/emu/setup.sh builds it).
  mgba     fallback: SDL mGBA (/usr/games/mgba) in a private Xvfb, START
           via xdotool, F12 (mGBA's screenshot key) writes exact framebuffer
           PNGs.  Timing is wall-clock, so frames vary run to run.
           (/usr/games/mgba-qt renders a blank widget under Xvfb, hence SDL.)

Usage:
  capture_title.py ROM.gba OUTDIR [--backend auto|harness|mgba]
                   [--every 6] [--count 80] [--start 40] [--prefix title]

Writes OUTDIR/<prefix>_NNN.png (+ <prefix>_final.png and <prefix>_final_3x.png
with the harness, ~8 s after START) and OUTDIR/<prefix>_timeline.txt.
Nothing is written next to the ROM.
"""
import argparse
import os
import shutil
import subprocess
import sys
import tempfile
import textwrap

HERE = os.path.dirname(os.path.abspath(__file__))
RUN_PY = os.path.join(HERE, "..", "emu", "run.py")
HARNESS_BIN = os.path.join(os.environ.get("EMU_BUILD_DIR", "/home/user/work/emu-build"),
                           "bin", "emu-harness")
MGBA = "/usr/games/mgba"
SKIP_FRAME = 600          # START pressed here (inside the intro, skippable)


def harness_capture(args):
    work = tempfile.mkdtemp(prefix="gfxcap_")
    try:
        lines = [f"wait {SKIP_FRAME}", "press START", f"wait {args.start}"]
        tl = []
        f = args.start
        for i in range(args.count):
            lines.append(f"shot {args.prefix}_{i:03d}")
            tl.append(f"{args.prefix}_{i:03d}.png frame=START+{f}")
            lines.append(f"wait {args.every}")
            f += args.every
        rest = max(0, 480 - f)
        lines += [f"wait {rest}", f"shot {args.prefix}_final", f"shot {args.prefix}_final_3x 3"]
        tl.append(f"{args.prefix}_final.png frame=START+{f + rest}")
        script = os.path.join(work, "capture.txt")
        with open(script, "w") as fh:
            fh.write("\n".join(lines) + "\n")
        out = os.path.join(work, "out")
        r = subprocess.run([sys.executable, "-I", RUN_PY, "--rom", args.rom, "--script", script,
                            "--out", out], capture_output=True, text=True)
        if r.returncode != 0:
            print(r.stdout[-2000:], r.stderr[-2000:])
            sys.exit(f"harness failed ({r.returncode})")
        n = 0
        for name in os.listdir(out):
            if name.startswith(args.prefix) and name.endswith(".png"):
                shutil.copy(os.path.join(out, name), os.path.join(args.outdir, name))
                n += 1
        with open(os.path.join(args.outdir, f"{args.prefix}_timeline.txt"), "w") as fh:
            fh.write("\n".join(tl) + "\n")
        print(f"{n} frames -> {args.outdir} (harness)")
    finally:
        shutil.rmtree(work, ignore_errors=True)


def mgba_capture(args):
    if not os.path.exists(MGBA):
        sys.exit(f"{MGBA} not found")
    work = tempfile.mkdtemp(prefix="gfxcap_")
    try:
        shutil.copy(args.rom, os.path.join(work, "rom.gba"))
        os.makedirs(os.path.join(work, "cfg"), exist_ok=True)
        every = args.every / 60.0
        start = args.start / 60.0
        lines = []
        now = 0.0
        for i in range(args.count):
            t = start + i * every
            if t > now:
                lines.append(f"sleep {t - now:.3f}")
                now = t
            lines.append("xdotool key F12")           # frontend hotkey: a tap is enough
            lines.append(f'echo "{now:.3f}" >> times.txt')
        # game input is polled once per frame: hold START ~100 ms.
        # 11 s after launch the intro is running and skippable.
        script = textwrap.dedent(f"""\
            #!/bin/bash
            cd "{work}"
            export XDG_CONFIG_HOME="{work}/cfg" SDL_AUDIODRIVER=dummy
            {MGBA} -2 rom.gba > mgba.log 2>&1 &
            PID=$!
            sleep 2
            xdotool mousemove 240 160
            sleep 9
            xdotool keydown Return; sleep 0.1; xdotool keyup Return
            """) + "\n".join(lines) + "\nsleep 1\nkill $PID\nwait $PID 2>/dev/null\n"
        sh = os.path.join(work, "run.sh")
        with open(sh, "w") as f:
            f.write(script)
        os.chmod(sh, 0o755)
        subprocess.run(["xvfb-run", "-a", "-s", "-screen 0 640x480x24", sh], check=False,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=now + 90)
        times = []
        tp = os.path.join(work, "times.txt")
        if os.path.exists(tp):
            times = [ln.strip() for ln in open(tp)]
        shots = sorted((f for f in os.listdir(work) if f.startswith("rom-") and f.endswith(".png")),
                       key=lambda s: int(s[4:-4]))
        with open(os.path.join(args.outdir, f"{args.prefix}_timeline.txt"), "w") as tl:
            for i, s in enumerate(shots):
                dst = f"{args.prefix}_{i:03d}.png"
                shutil.copy(os.path.join(work, s), os.path.join(args.outdir, dst))
                tl.write(f"{dst} t=START+{times[i] if i < len(times) else '?'}s\n")
        print(f"{len(shots)} frames -> {args.outdir} (mgba/xvfb)")
    finally:
        shutil.rmtree(work, ignore_errors=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("rom")
    ap.add_argument("outdir")
    ap.add_argument("--backend", default="auto", choices=["auto", "harness", "mgba"])
    ap.add_argument("--start", type=int, default=40, help="frames after START before the first shot")
    ap.add_argument("--every", type=int, default=6, help="frames between shots")
    ap.add_argument("--count", type=int, default=80, help="number of shots")
    ap.add_argument("--prefix", default="title")
    args = ap.parse_args()
    args.rom = os.path.abspath(args.rom)
    os.makedirs(args.outdir, exist_ok=True)
    backend = args.backend
    if backend == "auto":
        backend = "harness" if (os.path.exists(RUN_PY) and os.path.exists(HARNESS_BIN)) else "mgba"
    (harness_capture if backend == "harness" else mgba_capture)(args)


if __name__ == "__main__":
    main()
