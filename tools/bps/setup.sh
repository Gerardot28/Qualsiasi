#!/usr/bin/env bash
# Build what make_patch.sh needs, outside the repository:
#   $BPS_WORK_DIR/vanilla_emerald.gba  original Emerald rebuilt from pret/pokeemerald
#                                      with agbcc (SHA-1 checked, never commit it)
#   $BPS_WORK_DIR/flips/flips          Floating IPS command-line binary
#
# usage: tools/bps/setup.sh [--flips-only | --vanilla-only]
#
# Environment: BPS_WORK_DIR (default /home/user/work/bps), JOBS (default 4).
# Idempotent: finished steps are skipped.
set -euo pipefail

ROOT=${BPS_WORK_DIR:-/home/user/work/bps}
JOBS=${JOBS:-4}
EMERALD_SHA1=f3ae088181bf583e55daf962a92bb46f4f1d07b7

do_flips=1; do_vanilla=1
case "${1:-}" in
  --flips-only) do_vanilla=0 ;;
  --vanilla-only) do_flips=0 ;;
  "") ;;
  *) echo "usage: $0 [--flips-only|--vanilla-only]" >&2; exit 2 ;;
esac

need() { command -v "$1" >/dev/null 2>&1 || { echo "missing tool: $1" >&2; exit 1; }; }
need git; need make; need gcc; need g++
sha1() { if command -v sha1sum >/dev/null 2>&1; then sha1sum "$1" | cut -d' ' -f1; else shasum -a 1 "$1" | cut -d' ' -f1; fi; }
clone() { [ -d "$2/.git" ] || git clone --depth 1 "$1" "$2"; echo ">> $(basename "$2") at $(git -C "$2" rev-parse --short HEAD)"; }

mkdir -p "$ROOT"

if [ $do_flips -eq 1 ]; then
  clone https://github.com/Alcaro/Flips "$ROOT/flips"
  if [ ! -x "$ROOT/flips/flips" ]; then
    echo ">> building Flips (CLI)"
    make -C "$ROOT/flips" TARGET=cli CFLAGS=-O2
  fi
  echo ">> flips: $ROOT/flips/flips ($("$ROOT/flips/flips" --version))"
fi

if [ $do_vanilla -eq 1 ]; then
  OUT="$ROOT/vanilla_emerald.gba"
  if [ -f "$OUT" ] && [ "$(sha1 "$OUT")" = "$EMERALD_SHA1" ]; then
    echo ">> $OUT already present (sha1 ok)"
  else
    need arm-none-eabi-as
    clone https://github.com/pret/agbcc "$ROOT/agbcc"
    clone https://github.com/pret/pokeemerald "$ROOT/pokeemerald"
    if [ ! -x "$ROOT/agbcc/agbcc" ] || [ ! -x "$ROOT/agbcc/old_agbcc" ]; then
      echo ">> building agbcc (a few minutes)"
      (cd "$ROOT/agbcc" && ./build.sh)
    fi
    if [ ! -x "$ROOT/pokeemerald/tools/agbcc/bin/agbcc" ]; then
      (cd "$ROOT/agbcc" && ./install.sh ../pokeemerald)
    fi
    echo ">> building pokeemerald (make -j$JOBS)"
    make -C "$ROOT/pokeemerald" -j"$JOBS"
    got=$(sha1 "$ROOT/pokeemerald/pokeemerald.gba")
    if [ "$got" != "$EMERALD_SHA1" ]; then
      echo "ERROR: pokeemerald.gba sha1 $got != $EMERALD_SHA1 (try the tested commits listed in tools/bps/README.md)" >&2
      exit 1
    fi
    cp "$ROOT/pokeemerald/pokeemerald.gba" "$OUT"
    echo ">> $OUT sha1 $got (matches Pokemon - Emerald Version (USA, Europe))"
  fi
fi
