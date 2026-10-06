#!/usr/bin/env bash
# make_patch.sh - create a BPS patch (vanilla Emerald -> our hack) with Flips
# and verify the round trip before publishing it.
#
# usage: make_patch.sh [options] VANILLA.gba HACK.gba OUT.bps
#
# options:
#   --linear              use Flips' linear BPS mode instead of delta (bigger)
#   --allow-any-source    accept a VANILLA whose SHA-1 is not the official
#                         "Pokemon - Emerald Version (USA, Europe)" one
#   --notes FILE          also write a short text with the checksums for the
#                         release page (required ROM, result, patch)
#   -h, --help            show this help
#
# environment:
#   FLIPS    path to the flips binary. Default: `flips` in PATH, then
#            /home/user/work/bps/flips/flips
#   PYTHON   python interpreter for the independent check (default python3);
#            set PYTHON= (empty) to skip that check.
#
# The patch is written to a temporary file, applied again with Flips and with
# the independent applier bps.py, and moved to OUT.bps only if both rebuild
# HACK.gba byte for byte. Exit status: 0 ok, 1 failure, 2 usage error.

set -euo pipefail

EMERALD_SHA1=f3ae088181bf583e55daf962a92bb46f4f1d07b7
EMERALD_CRC32=1F1C08FB
EMERALD_NAME="Pokemon - Emerald Version (USA, Europe)"
HERE=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
BPS_PY="$HERE/bps.py"

usage() { sed -n '2,23p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'; exit "${1:-2}"; }
die() { echo "make_patch: ERROR: $*" >&2; exit 1; }

MODE=--bps-delta
ALLOW_ANY=0
NOTES=
POS=()
while [ $# -gt 0 ]; do
  case "$1" in
    --linear) MODE=--bps-linear ;;
    --allow-any-source) ALLOW_ANY=1 ;;
    --notes) [ $# -ge 2 ] || usage; NOTES=$2; shift ;;
    --notes=*) NOTES=${1#*=} ;;
    -h|--help) usage 0 ;;
    --) shift; POS+=("$@"); break ;;
    -*) echo "unknown option $1" >&2; usage ;;
    *) POS+=("$1") ;;
  esac
  shift
done
[ ${#POS[@]} -eq 3 ] || usage
VANILLA=${POS[0]}; HACK=${POS[1]}; OUT=${POS[2]}

# --- tools -----------------------------------------------------------------
if [ -z "${FLIPS:-}" ]; then
  if command -v flips >/dev/null 2>&1; then FLIPS=$(command -v flips)
  elif [ -x /home/user/work/bps/flips/flips ]; then FLIPS=/home/user/work/bps/flips/flips
  else die "flips not found: build it (git clone https://github.com/Alcaro/Flips; cd Flips; make TARGET=cli CFLAGS=-O2) and set FLIPS=/path/to/flips"
  fi
fi
[ -x "$FLIPS" ] || die "FLIPS=$FLIPS is not executable"
PYTHON=${PYTHON-python3}

sha1() {
  if command -v sha1sum >/dev/null 2>&1; then sha1sum "$1" | cut -d' ' -f1
  else shasum -a 1 "$1" | cut -d' ' -f1   # macOS
  fi
}
fsize() { wc -c < "$1" | tr -d ' '; }
# run_logged PREFIX CMD...: run CMD, show its output indented; keep its status
run_logged() {
  local prefix=$1; shift
  if "$@" > "$TMPD/cmd.log" 2>&1; then
    tr '\r' '\n' < "$TMPD/cmd.log" | grep -v '^[[:space:]]*$' | sed "s/^/  $prefix: /" || true
    return 0
  fi
  tr '\r' '\n' < "$TMPD/cmd.log" | sed "s/^/  $prefix: /" >&2
  return 1
}

# --- inputs ----------------------------------------------------------------
[ -f "$VANILLA" ] || die "vanilla ROM not found: $VANILLA"
[ -f "$HACK" ] || die "hack ROM not found: $HACK"
case "$OUT" in *.bps|*.BPS) ;; *) die "output name must end in .bps: $OUT" ;; esac
OUTDIR=$(dirname "$OUT")
[ -d "$OUTDIR" ] || die "output directory does not exist: $OUTDIR"
if [ -n "$NOTES" ]; then
  [ -d "$(dirname "$NOTES")" ] || die "notes directory does not exist: $(dirname "$NOTES")"
fi

V_SHA1=$(sha1 "$VANILLA")
H_SHA1=$(sha1 "$HACK")
echo "vanilla  $VANILLA  ($(fsize "$VANILLA") bytes, sha1 $V_SHA1)"
echo "hack     $HACK  ($(fsize "$HACK") bytes, sha1 $H_SHA1)"
if [ "$V_SHA1" != "$EMERALD_SHA1" ]; then
  if [ $ALLOW_ANY -eq 1 ]; then
    echo "WARNING: vanilla is NOT $EMERALD_NAME (sha1 $EMERALD_SHA1); users will need this exact file instead." >&2
  else
    die "vanilla sha1 $V_SHA1 != $EMERALD_SHA1 ($EMERALD_NAME). Use --allow-any-source to override."
  fi
fi
[ "$V_SHA1" != "$H_SHA1" ] || die "hack and vanilla are identical, nothing to patch"
[ "$(fsize "$HACK")" -le 33554432 ] || die "hack is larger than 32 MiB, the GBA cartridge limit"
# header sanity (warnings only): game code BPEE, .gameName, complement checksum
if [ -n "$PYTHON" ] && command -v "$PYTHON" >/dev/null 2>&1; then
  "$PYTHON" -I "$BPS_PY" check "$HACK" >&2 || true
fi

# --- create ----------------------------------------------------------------
TMPD=$(mktemp -d "${TMPDIR:-/tmp}/make_patch.XXXXXX")
trap 'rm -rf "$TMPD"' EXIT
TMP_BPS="$TMPD/patch.bps"
echo "flips    $FLIPS ($("$FLIPS" --version 2>&1 | head -n1)), mode $MODE"
start=$(date +%s)
run_logged flips "$FLIPS" --create "$MODE" "$VANILLA" "$HACK" "$TMP_BPS" || die "flips --create failed"
[ -s "$TMP_BPS" ] || die "flips produced no patch"
echo "created  $(fsize "$TMP_BPS") bytes in $(( $(date +%s) - start )) s"

# --- verify 1: Flips round trip --------------------------------------------
run_logged flips "$FLIPS" --apply "$TMP_BPS" "$VANILLA" "$TMPD/roundtrip.gba" || die "flips --apply failed on the new patch"
RT_SHA1=$(sha1 "$TMPD/roundtrip.gba")
[ "$RT_SHA1" = "$H_SHA1" ] || die "round trip mismatch: flips rebuilt sha1 $RT_SHA1, expected $H_SHA1"
echo "verify   flips apply: OK (sha1 $RT_SHA1)"
rm -f "$TMPD/roundtrip.gba"

# --- verify 2: independent applier (bps.py) ---------------------------------
if [ -n "$PYTHON" ] && command -v "$PYTHON" >/dev/null 2>&1; then
  run_logged bps.py "$PYTHON" -I "$BPS_PY" verify "$TMP_BPS" "$VANILLA" "$HACK" || die "independent check (bps.py verify) failed"
  echo "verify   bps.py apply: OK"
else
  echo "WARNING: independent check skipped (PYTHON is empty or not installed)" >&2
fi

# --- publish -----------------------------------------------------------------
mv -f "$TMP_BPS" "$OUT"
P_SHA1=$(sha1 "$OUT")
P_SIZE=$(fsize "$OUT")
echo "patch    $OUT  ($P_SIZE bytes, sha1 $P_SHA1)"

if [ -n "$NOTES" ]; then
  if [ -n "$PYTHON" ] && command -v "$PYTHON" >/dev/null 2>&1; then
    HASHES=$("$PYTHON" -I "$BPS_PY" hash --basename "$VANILLA" "$HACK" "$OUT")
  else
    HASHES="vanilla sha1 $V_SHA1
hack sha1 $H_SHA1
patch sha1 $P_SHA1"
  fi
  {
    echo "BPS patch: $(basename "$OUT") ($P_SIZE bytes)"
    if [ "$V_SHA1" = "$EMERALD_SHA1" ]; then
      echo "Required original ROM: $EMERALD_NAME  (16777216 bytes, crc32 $EMERALD_CRC32, sha1 $EMERALD_SHA1)"
    else
      echo "Required original ROM: $(basename "$VANILLA")  ($(fsize "$VANILLA") bytes, sha1 $V_SHA1) - NOT the official Emerald"
    fi
    echo "Created $(date -u +%Y-%m-%dT%H:%M:%SZ) with $("$FLIPS" --version 2>&1 | head -n1) $MODE"
    echo
    echo "$HASHES"
  } > "$NOTES"
  echo "notes    $NOTES"
fi
