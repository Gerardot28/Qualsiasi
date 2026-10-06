#!/usr/bin/env bash
# Fetch + build mGBA (as a shared library, no frontends) and compile the
# headless harness (harness.c) against it.
#
#   tools/emu/setup.sh            # build everything (idempotent)
#   tools/emu/setup.sh --harness  # only recompile harness.c
#   tools/emu/setup.sh --clean    # wipe the mGBA build dir and rebuild
#
# Environment overrides:
#   EMU_BUILD_DIR  (default /home/user/work/emu-build)
#   MGBA_TAG       (default 0.10.5 = newest stable 0.10.x)
#   MGBA_REPO      (default https://github.com/mgba-emu/mgba.git)
#
# Nothing is written inside the repository: sources, objects, the library
# and the harness binary all live under $EMU_BUILD_DIR.
set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="${EMU_BUILD_DIR:-/home/user/work/emu-build}"
TAG="${MGBA_TAG:-0.10.5}"
REPO="${MGBA_REPO:-https://github.com/mgba-emu/mgba.git}"
SRC="$ROOT/mgba-src"
BLD="$ROOT/mgba-build"
PREFIX="$ROOT/prefix"
BIN="$ROOT/bin"

only_harness=0
case "${1:-}" in
  --harness) only_harness=1 ;;
  --clean) rm -rf "$BLD" "$PREFIX" ;;
  "") ;;
  *) echo "usage: $0 [--harness|--clean]" >&2; exit 2 ;;
esac

need() { command -v "$1" >/dev/null 2>&1 || { echo "missing tool: $1 (apt-get install -y $2)" >&2; exit 1; }; }
need git git; need cmake cmake; need gcc gcc; need pkg-config pkg-config
if ! pkg-config --exists libpng zlib; then
  echo "missing libpng-dev / zlib1g-dev (apt-get install -y libpng-dev zlib1g-dev)" >&2; exit 1
fi

mkdir -p "$ROOT" "$BIN"

if [ "$only_harness" -eq 0 ]; then
  if [ ! -d "$SRC/.git" ]; then
    echo ">> cloning mGBA $TAG"
    git clone --depth 1 --branch "$TAG" "$REPO" "$SRC"
  else
    have="$(git -C "$SRC" describe --tags --exact-match 2>/dev/null || echo '?')"
    if [ "$have" != "$TAG" ]; then
      echo ">> switching mGBA checkout $have -> $TAG"
      git -C "$SRC" fetch --depth 1 origin "refs/tags/$TAG:refs/tags/$TAG"
      git -C "$SRC" checkout -q "$TAG"
      rm -rf "$BLD" "$PREFIX"
    fi
  fi

  gen=()
  if command -v ninja >/dev/null 2>&1; then gen=(-G Ninja); fi

  echo ">> configuring libmgba (GBA core only, no frontends/optional deps)"
  cmake -S "$SRC" -B "$BLD" "${gen[@]}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$PREFIX" \
    -DCMAKE_INSTALL_LIBDIR=lib \
    -DBUILD_QT=OFF -DBUILD_SDL=OFF -DBUILD_SHARED=ON -DBUILD_STATIC=OFF \
    -DBUILD_GL=OFF -DBUILD_GLES2=OFF -DBUILD_GLES3=OFF -DUSE_EPOXY=OFF \
    -DBUILD_LIBRETRO=OFF -DBUILD_PERF=OFF -DBUILD_TEST=OFF -DBUILD_SUITE=OFF \
    -DBUILD_CINEMA=OFF -DBUILD_ROM_TEST=OFF -DBUILD_EXAMPLE=OFF -DBUILD_PYTHON=OFF \
    -DM_CORE_GB=OFF -DM_CORE_GBA=ON \
    -DUSE_DEBUGGERS=OFF -DUSE_EDITLINE=OFF -DUSE_GDB_STUB=OFF \
    -DUSE_FFMPEG=OFF -DUSE_LUA=OFF -DUSE_LIBZIP=OFF -DUSE_MINIZIP=OFF \
    -DUSE_SQLITE3=OFF -DUSE_ELF=OFF -DUSE_LZMA=OFF -DUSE_DISCORD_RPC=OFF \
    -DUSE_ZLIB=ON -DUSE_PNG=ON \
    -DSKIP_GIT=ON >/dev/null

  echo ">> building libmgba"
  cmake --build "$BLD" -j"$(nproc)"
  cmake --install "$BLD" >/dev/null
fi

if [ ! -f "$PREFIX/include/mgba/flags.h" ]; then
  echo "libmgba not installed in $PREFIX; run $0 without --harness first" >&2; exit 1
fi

echo ">> compiling harness"
# shellcheck disable=SC2046
gcc -O2 -std=gnu11 -Wall -Wextra -Wno-unused-parameter \
  -I"$PREFIX/include" -o "$BIN/emu-harness" "$HERE/harness.c" \
  -L"$PREFIX/lib" -Wl,-rpath,"$PREFIX/lib" -lmgba $(pkg-config --cflags --libs libpng zlib) -lm

echo ">> ok: $BIN/emu-harness"
"$BIN/emu-harness" --version
