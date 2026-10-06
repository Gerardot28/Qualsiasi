#!/usr/bin/env python3
"""Smoke test a pokeemerald(-expansion) ROM with the headless harness.

    python3 -I tools/emu/selftest.py --rom pokeemerald.gba [--elf pokeemerald.elf] [--out DIR]

Checks, in order (all deterministic, ~15 s total):
  1. boot_to_title.txt         boots, title screen is not blank, no crash/reset/stuck
  2. new_game_intro_robust.txt new game -> every Birch text box -> truck -> Littleroot Town
  3. save_in_truck.txt         in-game SAVE writes the 128K Flash (--sav)
  4. continue_from_save.txt    CONTINUE from that save resumes in the truck
  5. determinism               boot_to_title twice -> byte-identical screenshots
Exit status 0 only if every check passes.  Screenshots: OUTDIR/<check>/.
"""
import argparse
import hashlib
import json
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
RUN = os.path.join(HERE, "run.py")
SCRIPTS = os.path.join(HERE, "scripts")


def run(args, name, script, extra=()):
    out = os.path.join(args.out, name)
    cmd = [sys.executable, "-I", RUN, "--rom", args.rom, "--script", os.path.join(SCRIPTS, script),
           "--out", out, "--quiet", "--trace-crash"] + (["--elf", args.elf] if args.elf else []) + list(extra)
    rc = subprocess.run(cmd).returncode
    try:
        with open(os.path.join(out, "run.json"), encoding="utf-8") as f:
            summary = json.load(f)
    except (OSError, ValueError):
        summary = {}
    return rc, summary, out


def png_hashes(d):
    return {f: hashlib.sha1(open(os.path.join(d, f), "rb").read()).hexdigest()
            for f in sorted(os.listdir(d)) if f.endswith(".png")}


def not_blank(path):
    try:
        from PIL import Image
    except ImportError:
        return True
    with Image.open(path) as im:
        return len(im.convert("RGB").getcolors(1 << 16) or [0] * 999) > 8


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--rom", required=True)
    ap.add_argument("--elf", help="symbols (default: ROM stem + .elf)")
    ap.add_argument("--out", help="output dir (default /home/user/work/emu-out/selftest-<rom name>)")
    args = ap.parse_args()
    stem = os.path.splitext(os.path.basename(args.rom))[0]
    args.out = args.out or f"/home/user/work/emu-out/selftest-{stem}"
    if not args.elf and not os.path.isfile(os.path.splitext(args.rom)[0] + ".elf"):
        sys.exit("selftest: symbols needed: pass --elf (pokeemerald.elf of this build)")
    os.makedirs(args.out, exist_ok=True)
    results = []

    def check(name, ok, detail):
        results.append((name, ok, detail))
        print(f"[{'PASS' if ok else 'FAIL'}] {name}: {detail}", flush=True)

    def info(s):
        d = s.get("done", {})
        return f"frames={d.get('frames')} fps={d.get('fps')} events={len(s.get('events', []))}"

    rc, s, out = run(args, "1_boot", "boot_to_title.txt")
    title = os.path.join(out, "title.png")
    check("boot_to_title", rc == 0 and os.path.isfile(title) and not_blank(title), f"rc={rc} {info(s)} {title}")

    rc, s, out = run(args, "2_new_game", "new_game_intro_robust.txt")
    boxes = len([f for f in os.listdir(out) if f.startswith(("rb_prof_", "rb_closing_"))]) if os.path.isdir(out) else 0
    maps = [m["id"] for m in s.get("maps", [])]
    check("new_game_to_littleroot", rc == 0 and "MAP_LITTLEROOT_TOWN" in maps,
          f"rc={rc} {info(s)} text_boxes={boxes} maps={maps}")

    sav = os.path.join(args.out, "selftest.sav")
    if os.path.exists(sav):
        os.remove(sav)
    rc, s, out = run(args, "3_save", "save_in_truck.txt", ["--sav", sav])
    data = open(sav, "rb").read() if os.path.isfile(sav) else b""
    used = sum(1 for b in data[:131072] if b != 0xFF)
    writes = s.get("done", {}).get("savedata_writes")
    check("in_game_save", rc == 0 and len(data) >= 131072 and used > 1000,
          f"rc={rc} {info(s)} sav={len(data)} bytes, {used} non-0xFF, savedata_writes={writes}")

    rc, s, out = run(args, "4_continue", "continue_from_save.txt", ["--sav", sav, "--sav-readonly"])
    maps = [m["id"] for m in s.get("maps", [])]
    check("continue_from_save", rc == 0 and maps[-1:] == ["MAP_INSIDE_OF_TRUCK"], f"rc={rc} {info(s)} maps={maps}")

    rc2, s2, out2 = run(args, "5_determinism", "boot_to_title.txt")
    h1, h2 = png_hashes(os.path.join(args.out, "1_boot")), png_hashes(out2)
    check("determinism", rc2 == 0 and h1 == h2 and len(h1) > 0, f"{len(h1)} screenshots identical={h1 == h2}")

    failed = [r for r in results if not r[1]]
    print(f"selftest: {len(results) - len(failed)}/{len(results)} passed, outputs in {args.out}")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
