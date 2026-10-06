#!/usr/bin/env python3
"""Independent BPS (beat patch format) reader/applier, used to double-check
the patches produced by Flips.

Spec: https://www.romhacking.net/documents/746/ (also bps_spec.md in Flips).

Sub-commands:
  info   PATCH [--verbose]            header, CRC32s, metadata, command stats
  apply  PATCH SOURCE OUT [--ignore-checksum]
  verify PATCH SOURCE TARGET          apply in memory and compare with TARGET
  hash   FILE...                      size, CRC32, MD5, SHA-1 (+ GBA header)

Exit status: 0 ok, 1 verification/patch error, 2 usage error.
Run as: python3 -I tools/bps/bps.py ...
"""

import argparse
import hashlib
import struct
import sys
import zlib

MAGIC = b"BPS1"
FOOTER = 12
CMD_NAMES = ("SourceRead", "TargetRead", "SourceCopy", "TargetCopy")


class BPSError(Exception):
    pass


def crc32(data):
    return zlib.crc32(data) & 0xFFFFFFFF


def decode_number(buf, pos, end):
    """Variable-length number as defined by the spec (with the -1 trick)."""
    data = 0
    shift = 1
    while True:
        if pos >= end:
            raise BPSError("truncated number at patch offset 0x%X" % pos)
        x = buf[pos]
        pos += 1
        data += (x & 0x7F) * shift
        if x & 0x80:
            return data, pos
        shift <<= 7
        data += shift


def parse_header(patch):
    if len(patch) < len(MAGIC) + 3 + FOOTER:
        raise BPSError("file too small to be a BPS patch (%d bytes)" % len(patch))
    if patch[:4] != MAGIC:
        raise BPSError("bad magic %r (expected b'BPS1')" % bytes(patch[:4]))
    end = len(patch) - FOOTER
    pos = 4
    src_size, pos = decode_number(patch, pos, end)
    tgt_size, pos = decode_number(patch, pos, end)
    meta_size, pos = decode_number(patch, pos, end)
    if pos + meta_size > end:
        raise BPSError("metadata runs past end of patch")
    meta = bytes(patch[pos:pos + meta_size])
    pos += meta_size
    src_crc, tgt_crc, patch_crc = struct.unpack("<III", patch[end:])
    return {
        "source_size": src_size,
        "target_size": tgt_size,
        "metadata": meta,
        "commands_offset": pos,
        "source_crc32": src_crc,
        "target_crc32": tgt_crc,
        "patch_crc32": patch_crc,
        "patch_crc32_actual": crc32(patch[:-4]),
    }


def iter_commands(patch, hdr):
    """Yield (cmd, length, offset_delta or None, patch_pos_of_payload)."""
    end = len(patch) - FOOTER
    pos = hdr["commands_offset"]
    while pos < end:
        data, pos = decode_number(patch, pos, end)
        cmd = data & 3
        length = (data >> 2) + 1
        delta = None
        payload = pos
        if cmd == 1:
            if pos + length > end:
                raise BPSError("TargetRead runs past end of patch")
            pos += length
        elif cmd >= 2:
            d, pos = decode_number(patch, pos, end)
            delta = -(d >> 1) if d & 1 else (d >> 1)
        yield cmd, length, delta, payload
    if pos != end:
        raise BPSError("command stream overruns footer")


def apply_patch(patch, source, check_crc=True):
    hdr = parse_header(patch)
    if hdr["patch_crc32"] != hdr["patch_crc32_actual"]:
        raise BPSError("patch CRC32 mismatch: stored %08X, computed %08X (patch is corrupt)"
                       % (hdr["patch_crc32"], hdr["patch_crc32_actual"]))
    if check_crc:
        if len(source) != hdr["source_size"]:
            raise BPSError("source size %d != expected %d" % (len(source), hdr["source_size"]))
        c = crc32(source)
        if c != hdr["source_crc32"]:
            raise BPSError("source CRC32 %08X != expected %08X (wrong source ROM)"
                           % (c, hdr["source_crc32"]))
    tsize = hdr["target_size"]
    out = bytearray(tsize)
    o = 0
    srel = 0
    trel = 0
    slen = len(source)
    for cmd, length, delta, payload in iter_commands(patch, hdr):
        if o + length > tsize:
            raise BPSError("command writes past target size at output 0x%X" % o)
        if cmd == 0:  # SourceRead
            if o + length > slen:
                raise BPSError("SourceRead past end of source at 0x%X" % o)
            out[o:o + length] = source[o:o + length]
        elif cmd == 1:  # TargetRead
            out[o:o + length] = patch[payload:payload + length]
        elif cmd == 2:  # SourceCopy
            srel += delta
            if srel < 0 or srel + length > slen:
                raise BPSError("SourceCopy out of source bounds (offset %d)" % srel)
            out[o:o + length] = source[srel:srel + length]
            srel += length
        else:  # TargetCopy (may overlap the bytes being written: RLE)
            trel += delta
            if trel < 0 or trel >= o:
                raise BPSError("TargetCopy reads unwritten data (offset %d, output %d)" % (trel, o))
            dist = o - trel
            if dist >= length:
                out[o:o + length] = out[trel:trel + length]
            else:
                pattern = bytes(out[trel:o])
                out[o:o + length] = (pattern * (length // dist + 1))[:length]
            trel += length
        o += length
    if o != tsize:
        raise BPSError("patch produced %d bytes, header says %d" % (o, tsize))
    if check_crc:
        c = crc32(out)
        if c != hdr["target_crc32"]:
            raise BPSError("target CRC32 %08X != expected %08X" % (c, hdr["target_crc32"]))
    return bytes(out), hdr


def read(path):
    with open(path, "rb") as f:
        return f.read()


def gba_header(data):
    """(title, code, maker, version, checksum_ok) or None if not a GBA ROM."""
    if len(data) < 0xC0 or data[0xB2] != 0x96:  # 0x96 is a fixed header byte
        return None
    title = data[0xA0:0xAC].split(b"\0")[0].decode("ascii", "replace")
    code = data[0xAC:0xB0].decode("ascii", "replace")
    maker = data[0xB0:0xB2].decode("ascii", "replace")
    version = data[0xBC]
    chk = (-sum(data[0xA0:0xBD]) - 0x19) & 0xFF
    return title, code, maker, version, chk == data[0xBD]


def cmd_hash(args):
    for p in args.files:
        d = read(p)
        print("%s\n  size   %d bytes (%.2f MiB)\n  crc32  %08X\n  md5    %s\n  sha1   %s"
              % (p, len(d), len(d) / 1048576, crc32(d), hashlib.md5(d).hexdigest(),
                 hashlib.sha1(d).hexdigest()))
        h = gba_header(d)
        if h:
            print("  gba    title=%r code=%s maker=%s rev=%d header-checksum=%s"
                  % (h[0], h[1], h[2], h[3], "ok" if h[4] else "BAD"))
    return 0


def cmd_info(args):
    patch = read(args.patch)
    hdr = parse_header(patch)
    ok = hdr["patch_crc32"] == hdr["patch_crc32_actual"]
    print("patch          %s (%d bytes)" % (args.patch, len(patch)))
    print("source size    %d  crc32 %08X" % (hdr["source_size"], hdr["source_crc32"]))
    print("target size    %d  crc32 %08X" % (hdr["target_size"], hdr["target_crc32"]))
    print("patch crc32    %08X (%s)" % (hdr["patch_crc32"], "ok" if ok else
                                         "MISMATCH, computed %08X" % hdr["patch_crc32_actual"]))
    meta = hdr["metadata"]
    print("metadata       %d bytes%s" % (len(meta), (": " + meta.decode("utf-8", "replace")) if meta else ""))
    counts = [0, 0, 0, 0]
    sizes = [0, 0, 0, 0]
    for cmd, length, delta, payload in iter_commands(patch, hdr):
        counts[cmd] += 1
        sizes[cmd] += length
        if args.verbose:
            extra = "" if delta is None else " delta=%+d" % delta
            print("  %-10s len=%d%s" % (CMD_NAMES[cmd], length, extra))
    for i, name in enumerate(CMD_NAMES):
        print("%-14s %8d commands, %10d bytes" % (name, counts[i], sizes[i]))
    return 0 if ok else 1


def cmd_apply(args):
    out, hdr = apply_patch(read(args.patch), read(args.source), not args.ignore_checksum)
    with open(args.out, "wb") as f:
        f.write(out)
    print("wrote %s (%d bytes, crc32 %08X, sha1 %s)"
          % (args.out, len(out), crc32(out), hashlib.sha1(out).hexdigest()))
    return 0


def cmd_verify(args):
    target = read(args.target)
    out, hdr = apply_patch(read(args.patch), read(args.source), True)
    if out != target:
        n = min(len(out), len(target))
        first = next((i for i in range(n) if out[i] != target[i]), n)
        print("FAIL: patched output differs from %s (sizes %d/%d, first difference at 0x%X)"
              % (args.target, len(out), len(target), first))
        return 1
    if crc32(target) != hdr["target_crc32"]:
        print("FAIL: target CRC32 in footer does not match %s" % args.target)
        return 1
    print("OK: %s + %s == %s (sha1 %s)" % (args.source, args.patch, args.target,
                                           hashlib.sha1(out).hexdigest()))
    return 0


def main(argv=None):
    ap = argparse.ArgumentParser(description="Independent BPS patch reader/applier.")
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("info", help="show header/footer and command statistics")
    p.add_argument("patch")
    p.add_argument("-v", "--verbose", action="store_true", help="list every command")
    p.set_defaults(func=cmd_info)
    p = sub.add_parser("apply", help="apply PATCH to SOURCE, write OUT")
    p.add_argument("patch")
    p.add_argument("source")
    p.add_argument("out")
    p.add_argument("--ignore-checksum", action="store_true")
    p.set_defaults(func=cmd_apply)
    p = sub.add_parser("verify", help="check that PATCH turns SOURCE into TARGET")
    p.add_argument("patch")
    p.add_argument("source")
    p.add_argument("target")
    p.set_defaults(func=cmd_verify)
    p = sub.add_parser("hash", help="print size/CRC32/MD5/SHA-1 (and GBA header)")
    p.add_argument("files", nargs="+")
    p.set_defaults(func=cmd_hash)
    args = ap.parse_args(argv)
    try:
        return args.func(args)
    except BPSError as e:
        print("ERROR: %s" % e, file=sys.stderr)
        return 1
    except OSError as e:
        print("ERROR: %s" % e, file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
