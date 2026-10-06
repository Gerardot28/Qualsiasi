#!/usr/bin/env python3
"""Headless, deterministic GBA test runner (wrapper around emu-harness / libmgba).

    python3 -I tools/emu/run.py --rom ROM.gba --script inputs.txt --out OUTDIR \
        [--save-state-out F] [--load-state F] [--sav F] [--elf ELF | --map MAP] [--src TREE]

The script language is documented in tools/emu/README.md.  On top of the
commands understood by the C harness, this wrapper adds:

    include FILE              splice another script (path relative to this script)
    mapinfo                   print current map: group/num + map name
    waitmap MAP [MAXFRAMES]   run until the player is on MAP (MAP_LITTLEROOT_TOWN or LittlerootTown)
    expectmap MAP             fail the run if the player is not on MAP
    waitcb2 FUNC [MAXFRAMES]  run until gMain.callback2 == FUNC (e.g. CB2_Overworld)
    expectcb2 FUNC            fail the run unless gMain.callback2 == FUNC
    waittask FUNC [MAXFRAMES] run until some gTasks[i].func == FUNC
                              (e.g. Task_NewGameBirchSpeech_ChooseGender)
    breakiftask FUNC [FUNC..] inside loop/endloop: leave the loop if a task runs FUNC
    breakifcb2 FUNC [FUNC..]  inside loop/endloop: leave the loop if gMain.callback2 == FUNC
    textbox NAME [MAXFRAMES]  wait until the message box (y 112-159) has finished printing
                              (periodic arrow animation ignored) and screenshot it

Symbols (for read/expect/until/write with names like gSaveBlock1Ptr, and for the
map commands) come from --elf (preferred: arm-none-eabi-nm) or --map (GNU ld
map file).  If neither is given, ROM.elf / ROM.map next to the ROM are used.

Everything the harness prints is echoed; additionally OUTDIR/run.log (full
output) and OUTDIR/run.json (machine readable summary) are written.
Exit status: 0 ok, 1 error, 2 script error, 3 expect/until failed,
4 crash / stuck / unexpected reset detected.
"""
import argparse
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import threading
import time

HERE = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.environ.get("EMU_BUILD_DIR", "/home/user/work/emu-build")
DEFAULT_HARNESS = os.path.join(BUILD_DIR, "bin", "emu-harness")

# offsets inside pokeemerald structs (include/global.h, include/main.h)
SAVEBLOCK1_LOCATION = 4          # struct SaveBlock1 { Coords16 pos; WarpData location; ...}
GMAIN_VBLANK_COUNTER1 = 0x20     # struct Main { ...; u32 vblankCounter1 @0x20 }
GMAIN_CALLBACK2 = 0x04           # struct Main { MainCallback callback1, callback2; ...}
TASK_SIZE = 0x28                 # struct Task { func; isActive, prev, next, priority; s16 data[16]; }
NUM_TASKS = 16


# ----------------------------------------------------------------- symbols

def load_symbols_elf(elf):
    nm = shutil.which("arm-none-eabi-nm") or shutil.which("nm")
    if not nm:
        raise SystemExit("run.py: arm-none-eabi-nm not found (needed for --elf)")
    outp = subprocess.run([nm, elf], check=True, capture_output=True, text=True).stdout
    syms = {}       # name -> addr (globals win over locals)
    by_addr = {}    # addr -> [names]
    for line in outp.splitlines():
        parts = line.split()
        if len(parts) != 3:
            continue
        addr_s, typ, name = parts
        if typ in "aNUw?" or not re.match(r"^[A-Za-z_][A-Za-z0-9_.]*$", name):
            continue
        addr = int(addr_s, 16)
        if name not in syms or typ.isupper():
            syms[name] = addr
        by_addr.setdefault(addr, []).append(name)
    return syms, by_addr


MAP_LINE = re.compile(r"^\s+0x([0-9a-fA-F]{8,16})\s+([A-Za-z_][A-Za-z0-9_.]*)\s*$")


def load_symbols_map(mapfile):
    syms, by_addr = {}, {}
    with open(mapfile, encoding="utf-8", errors="replace") as f:
        for line in f:
            m = MAP_LINE.match(line)
            if not m:
                continue
            addr = int(m.group(1), 16)
            name = m.group(2)
            syms.setdefault(name, addr)
            by_addr.setdefault(addr, []).append(name)
    return syms, by_addr


# ----------------------------------------------------------------- map names

def camel_to_const(label):
    s = re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", label)
    s = re.sub(r"([A-Z]+)([A-Z][a-z])", r"\1_\2", s)
    s = re.sub(r"_+", "_", s)
    return "MAP_" + s.upper()


def load_src_map_ids(src):
    """header label -> MAP_ id from data/maps/*/map.json of a source tree."""
    ids = {}
    maps_dir = os.path.join(src, "data", "maps")
    if not os.path.isdir(maps_dir):
        return ids
    for d in os.listdir(maps_dir):
        p = os.path.join(maps_dir, d, "map.json")
        if os.path.isfile(p):
            try:
                with open(p, encoding="utf-8") as f:
                    j = json.load(f)
                ids[j.get("name", d)] = j.get("id")
            except (OSError, ValueError):
                pass
    return ids


def load_src_map_table(src):
    """(group, num) -> (label, MAP_ id) using data/maps/map_groups.json (fallback when no ELF)."""
    p = os.path.join(src, "data", "maps", "map_groups.json")
    if not os.path.isfile(p):
        return {}
    with open(p, encoding="utf-8") as f:
        groups = json.load(f)
    ids = load_src_map_ids(src)
    table = {}
    for g, gname in enumerate(groups["group_order"]):
        for n, label in enumerate(groups.get(gname, [])):
            table[(g, n)] = (label, ids.get(label) or camel_to_const(label))
    return table


def rom_map_table(rom_bytes, syms, by_addr, src_ids):
    """(group, num) -> (label, MAP_ id) read from the ROM's gMapGroups table + ELF symbols."""
    base = syms.get("gMapGroups")
    if base is None:
        return {}

    def r32(addr):
        off = addr - 0x08000000
        if off < 0 or off + 4 > len(rom_bytes):
            return None
        return struct.unpack_from("<I", rom_bytes, off)[0]

    group_tables = sorted(a for n, a in syms.items() if n.startswith("gMapGroup_"))
    table = {}
    g = 0
    while True:
        ptr = r32(base + 4 * g)
        if ptr is None or ptr not in group_tables:
            break
        nxt = [a for a in group_tables if a > ptr] + [base]
        end = min(a for a in nxt if a > ptr)
        for n in range((end - ptr) // 4):
            hdr = r32(ptr + 4 * n)
            names = [x for x in by_addr.get(hdr, []) if not x.startswith("g")] or by_addr.get(hdr, [])
            if not names:
                continue
            label = names[0]
            table[(g, n)] = (label, src_ids.get(label) or camel_to_const(label))
        g += 1
    return table


# ----------------------------------------------------------------- script expansion

class ScriptError(Exception):
    pass


def expand_script(path, ctx, out_lines, origin, depth=0):
    if depth > 16:
        raise ScriptError(f"{path}: include nesting too deep")
    try:
        with open(path, encoding="utf-8") as f:
            lines = f.read().splitlines()
    except OSError as e:
        raise ScriptError(f"cannot read script {path}: {e}")
    for i, raw in enumerate(lines, 1):
        stripped = raw.strip()
        word = stripped.split(None, 1)[0].lower() if stripped else ""
        args = stripped.split("#", 1)[0].split()[1:] if word != "echo" else []
        where = (path, i, raw)

        def emit(text):
            out_lines.append(text)
            origin.append(where)

        if word == "include":
            if len(args) != 1:
                raise ScriptError(f"{path}:{i}: usage: include FILE")
            inc = args[0] if os.path.isabs(args[0]) else os.path.join(os.path.dirname(path), args[0])
            expand_script(inc, ctx, out_lines, origin, depth + 1)
        elif word in ("breakiftask", "breakifcb2"):
            if not args:
                raise ScriptError(f"{path}:{i}: usage: {word} FUNCTION [FUNCTION...]")
            for fn in args:
                ptr = func_ptr(fn, ctx, path, i)
                if word == "breakifcb2":
                    emit(f"breakif32 gMain+{GMAIN_CALLBACK2:#x} {ptr:#010x} cb2:{fn}")
                else:
                    emit(f"breakifany32 gTasks {TASK_SIZE:#x} {NUM_TASKS} {ptr:#010x} task:{fn}")
        elif word == "textbox":
            # wait until the standard message box (bottom 48 px) stops changing, then screenshot it
            if not args or len(args) > 2:
                raise ScriptError(f"{path}:{i}: usage: textbox NAME [MAXFRAMES]")
            maxf = args[1] if len(args) == 2 else "1200"
            emit(f"waitstable 30 {maxf} 0 112 240 48")
            emit(f"shot {args[0]}")
        elif word in ("waitcb2", "waittask", "expectcb2"):
            # pokeemerald callbacks: gMain.callback2 / any of the 16 gTasks[].func
            usage = f"{path}:{i}: usage: {word} FUNCTION" + (" [MAXFRAMES]" if word != "expectcb2" else "")
            if not args or len(args) > (1 if word == "expectcb2" else 2):
                raise ScriptError(usage)
            fn = args[0]
            ptr = func_ptr(fn, ctx, path, i)
            maxf = args[1] if len(args) == 2 else "3600"
            if word == "waitcb2":
                need = ("gMain",)
                line = f"until32 gMain+{GMAIN_CALLBACK2:#x} {ptr:#010x} {maxf} waitcb2:{fn}"
            elif word == "expectcb2":
                need = ("gMain",)
                line = f"expect32 gMain+{GMAIN_CALLBACK2:#x} {ptr:#010x} expectcb2:{fn}"
            else:
                need = ("gTasks",)
                line = f"untilany32 gTasks {TASK_SIZE:#x} {NUM_TASKS} {ptr:#010x} {maxf} waittask:{fn}"
            for n in need:
                if n not in ctx["syms"]:
                    raise ScriptError(f"{path}:{i}: '{word}' needs symbol {n}")
            emit(line)
        elif word in ("mapinfo", "waitmap", "expectmap"):
            if "gSaveBlock1Ptr" not in ctx["syms"]:
                if word == "mapinfo":
                    print(f"run.py: warning: {path}:{i}: mapinfo skipped (no symbols: pass --elf/--map)",
                          file=sys.stderr)
                    continue
                raise ScriptError(f"{path}:{i}: '{word}' needs symbols (--elf or --map)")
            loc = f"[gSaveBlock1Ptr]+{SAVEBLOCK1_LOCATION}"
            if word == "mapinfo":
                emit(f"read8 {loc} mapGroup")
                emit(f"read8 [gSaveBlock1Ptr]+{SAVEBLOCK1_LOCATION + 1} mapNum")
                emit(f"read16 [gSaveBlock1Ptr]+0 posX")
                emit(f"read16 [gSaveBlock1Ptr]+2 posY")
            else:
                if not args or len(args) > 2 or (word == "expectmap" and len(args) != 1):
                    raise ScriptError(f"{path}:{i}: usage: {word} MAP" + (" [MAXFRAMES]" if word == "waitmap" else ""))
                key = resolve_map(args[0], ctx)
                if key is None:
                    raise ScriptError(f"{path}:{i}: unknown map '{args[0]}'")
                g, n = key
                val = (n << 8) | g
                if word == "waitmap":
                    maxf = args[1] if len(args) == 2 else "3600"
                    emit(f"until16 {loc} {val:#06x} {maxf} waitmap:{args[0]}")
                else:
                    emit(f"expect16 {loc} {val:#06x} expectmap:{args[0]}")
        else:
            emit(raw)


def func_ptr(fn, ctx, path, i):
    if fn not in ctx["syms"]:
        raise ScriptError(f"{path}:{i}: unknown function symbol '{fn}' (static functions need --elf)")
    for n in ("gMain", "gTasks"):
        if n not in ctx["syms"]:
            raise ScriptError(f"{path}:{i}: symbol {n} missing")
    return ctx["syms"][fn] | 1      # Thumb code: function pointers have bit 0 set


def resolve_map(name, ctx):
    for key, (label, const) in ctx["maps"].items():
        if name in (label, const) or name.upper() == const or ("MAP_" + name.upper()) == const:
            return key
    m = re.match(r"^(\d+)[.:,/](\d+)$", name)
    if m:
        return int(m.group(1)), int(m.group(2))
    return None


# ----------------------------------------------------------------- main

def parse_kv(line):
    """parse 'KIND a=b c="d e" ...' into dict"""
    d = {}
    for m in re.finditer(r'(\w+)=("([^"]*)"|\S+)', line):
        d[m.group(1)] = m.group(3) if m.group(3) is not None else m.group(2)
    return d


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--rom", required=True)
    ap.add_argument("--script", required=True, help="input script ('-' = stdin)")
    ap.add_argument("--out", required=True, help="output directory (screenshots, states, run.log, run.json)")
    ap.add_argument("--save-state-out", help="savestate written after the script")
    ap.add_argument("--load-state", help="savestate loaded before the script")
    ap.add_argument("--sav", help="cartridge save file (Flash 128K; created if missing)")
    ap.add_argument("--sav-readonly", action="store_true", help="use --sav but never write it")
    ap.add_argument("--elf", help="ELF with symbols (default: ROM stem + .elf if present)")
    ap.add_argument("--map", dest="mapfile", help="GNU ld .map file (fallback symbol source)")
    ap.add_argument("--src", help="source tree for MAP_* ids (default: tree containing the ROM, then /home/user/pex-orig)")
    ap.add_argument("--scale", type=int, default=1, help="default screenshot scale (1=240x160, 2=480x320)")
    ap.add_argument("--rtc", help="RTC start, epoch seconds or YYYY-MM-DDTHH:MM:SS (UTC)")
    ap.add_argument("--trace-crash", action="store_true", help="dump PC trace/registers + crash_*.png/.state on crash")
    ap.add_argument("--stop-on-crash", action="store_true", help="abort at the first crash/stuck/reset")
    ap.add_argument("--allow-reset", action="store_true", help="soft resets do not fail the run")
    ap.add_argument("--no-reset-watch", action="store_true", help="do not watch gMain.vblankCounter1 for resets")
    ap.add_argument("--stuck-frames", type=int, help="stuck detection threshold in frames (0 = off)")
    ap.add_argument("--timeout", type=float, default=600, help="wall clock limit in seconds")
    ap.add_argument("--harness", default=os.environ.get("EMU_HARNESS", DEFAULT_HARNESS))
    ap.add_argument("--quiet", action="store_true", help="do not echo harness output (still in run.log)")
    ap.add_argument("extra", nargs="*", help="extra arguments passed verbatim to emu-harness (after --)")
    a = ap.parse_args()

    if not os.access(a.harness, os.X_OK):
        sys.exit(f"run.py: harness not built: {a.harness}\n        run: bash {os.path.join(HERE, 'setup.sh')}")
    if not os.path.isfile(a.rom):
        sys.exit(f"run.py: ROM not found: {a.rom}")
    os.makedirs(a.out, exist_ok=True)
    workdir = os.path.join(a.out, ".emu")
    os.makedirs(workdir, exist_ok=True)

    # --- symbols
    stem = os.path.splitext(a.rom)[0]
    elf = a.elf or (stem + ".elf" if not a.mapfile and os.path.isfile(stem + ".elf") else None)
    mapfile = a.mapfile or (stem + ".map" if not elf and os.path.isfile(stem + ".map") else None)
    syms, by_addr, symsrc = {}, {}, None
    if elf:
        syms, by_addr = load_symbols_elf(elf)
        symsrc = elf
    elif mapfile:
        syms, by_addr = load_symbols_map(mapfile)
        symsrc = mapfile

    # --- map names
    src = a.src
    if not src:
        for cand in (os.path.dirname(os.path.abspath(a.rom)), "/home/user/pex-orig"):
            if os.path.isfile(os.path.join(cand, "data", "maps", "map_groups.json")):
                src = cand
                break
    src_ids = load_src_map_ids(src) if src else {}
    maps = {}
    if syms and elf:
        with open(a.rom, "rb") as f:
            maps = rom_map_table(f.read(), syms, by_addr, src_ids)
    if not maps and src:
        maps = load_src_map_table(src)
    ctx = {"syms": syms, "maps": maps}

    # --- expand script
    lines, origin = [], []
    try:
        if a.script == "-":
            tmp_in = os.path.join(workdir, "stdin.txt")
            with open(tmp_in, "w", encoding="utf-8") as f:
                f.write(sys.stdin.read())
            expand_script(tmp_in, ctx, lines, origin)
        else:
            expand_script(a.script, ctx, lines, origin)
    except ScriptError as e:
        print(f"run.py: {e}", file=sys.stderr)
        return 2
    expanded = os.path.join(workdir, "script.expanded.txt")
    with open(expanded, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")

    cmd = [a.harness, "--rom", a.rom, "--script", expanded, "--out", a.out, "--scale", str(a.scale)]
    if syms:
        symfile = os.path.join(workdir, "symbols.txt")
        with open(symfile, "w", encoding="utf-8") as f:
            for name, addr in syms.items():
                f.write(f"{name} {addr:#010x}\n")
        cmd += ["--symbols", symfile]
        if "gMain" in syms and not a.no_reset_watch:
            cmd += ["--watch-counter", f"gMain+{GMAIN_VBLANK_COUNTER1:#x}"]
    for flag, val in (("--sav", a.sav), ("--load-state", a.load_state), ("--save-state-out", a.save_state_out),
                      ("--rtc", a.rtc)):
        if val:
            cmd += [flag, val]
    if a.stuck_frames is not None:
        cmd += ["--stuck-frames", str(a.stuck_frames)]
    for flag, on in (("--sav-readonly", a.sav_readonly), ("--trace-crash", a.trace_crash),
                     ("--stop-on-crash", a.stop_on_crash), ("--allow-reset", a.allow_reset)):
        if on:
            cmd.append(flag)
    cmd += a.extra

    # --- run
    summary = {"rom": os.path.abspath(a.rom), "script": a.script, "symbols": symsrc, "map_names": len(maps),
               "command": cmd, "shots": [], "reads": [], "events": [], "expects": [], "untils": [], "maps": [],
               "states": [], "logs": []}
    t0 = time.time()
    log_path = os.path.join(a.out, "run.log")
    pending_group = None
    with open(log_path, "w", encoding="utf-8") as logf:
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
                                encoding="utf-8", errors="replace", bufsize=1)
        err_chunks = []
        err_thread = threading.Thread(target=lambda: err_chunks.append(proc.stderr.read()), daemon=True)
        err_thread.start()
        timed_out = []

        def on_timeout():
            timed_out.append(True)
            proc.kill()

        timer = threading.Timer(a.timeout, on_timeout)
        timer.start()
        try:
            for line in proc.stdout:
                line = line.rstrip("\n")
                extra_lines = []
                kind = line.split(" ", 1)[0]
                kv = parse_kv(line)
                if kind == "SHOT":
                    summary["shots"].append({"frame": int(kv["frame"]), "file": kv["file"]})
                elif kind == "READ":
                    summary["reads"].append(kv)
                    if kv.get("label") == "mapGroup":
                        pending_group = int(kv["value"], 16)
                    elif kv.get("label") == "mapNum" and pending_group is not None:
                        key = (pending_group, int(kv["value"], 16))
                        label, const = maps.get(key, ("?", "?"))
                        info = {"frame": int(kv["frame"]), "group": key[0], "num": key[1], "label": label,
                                "id": const}
                        summary["maps"].append(info)
                        extra_lines.append(f"MAP frame={info['frame']} group={key[0]} num={key[1]} "
                                           f"name={const} label={label}")
                        pending_group = None
                elif kind == "EVENT":
                    summary["events"].append({"kind": line.split()[1], **kv})
                elif kind == "EXPECT":
                    summary["expects"].append({"ok": line.split()[1] == "ok", **kv})
                elif kind in ("UNTIL", "STABLE"):
                    summary["untils"].append({"ok": line.split()[1] == "ok", **kv})
                elif kind == "STATE":
                    summary["states"].append({"op": line.split()[1], **kv})
                elif kind in ("LOG", "LOGSUM"):
                    summary["logs"].append(kv)
                elif kind in ("DONE", "INFO"):
                    summary[kind.lower()] = kv
                for l2 in [line] + extra_lines:
                    logf.write(l2 + "\n")
                    if not a.quiet:
                        print(l2, flush=True)
            proc.wait()
        finally:
            timer.cancel()
        err_thread.join(5)
        # map expanded-script line numbers back to the user's files
        for line in "".join(err_chunks).splitlines():
            m = re.match(r"^(.*script\.expanded\.txt):(\d+):(.*)$", line)
            if m and 0 < int(m.group(2)) <= len(origin):
                p, ln, _raw = origin[int(m.group(2)) - 1]
                line = f"{p}:{ln}:{m.group(3)}"
            logf.write(line + "\n")
            print(line, file=sys.stderr)
        rc = proc.returncode
        if timed_out:
            msg = f"run.py: timeout after {a.timeout}s, harness killed"
            logf.write(msg + "\n")
            print(msg, file=sys.stderr)
            rc = 1
    summary["wall_seconds"] = round(time.time() - t0, 3)
    summary["exit"] = rc
    with open(os.path.join(a.out, "run.json"), "w", encoding="utf-8") as f:
        json.dump(summary, f, indent=1)
    return rc


if __name__ == "__main__":
    sys.exit(main())
