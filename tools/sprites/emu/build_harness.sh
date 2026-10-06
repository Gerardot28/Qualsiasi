#!/bin/sh
# Build the headless mGBA screenshot harness (gbashot) against a static libmgba 0.10.2.
# Usage: build_harness.sh [WORKDIR]   (default: ./_mgba) -> WORKDIR/gbashot
set -e
HERE=$(cd "$(dirname "$0")" && pwd)
W=${1:-$HERE/_mgba}
mkdir -p "$W"
if [ ! -d "$W/mgba-src" ]; then
  git clone --depth 1 -b 0.10.2 https://github.com/mgba-emu/mgba "$W/mgba-src"
fi
if [ ! -f "$W/mgba-src/build/libmgba.a" ]; then
  mkdir -p "$W/mgba-src/build" && cd "$W/mgba-src/build"
  cmake .. -DBUILD_QT=OFF -DBUILD_SDL=OFF -DBUILD_GL=OFF -DBUILD_GLES2=OFF -DBUILD_GLES3=OFF \
    -DUSE_FFMPEG=OFF -DUSE_LUA=OFF -DUSE_EDITLINE=OFF -DUSE_MINIZIP=OFF -DUSE_LIBZIP=OFF \
    -DUSE_SQLITE3=OFF -DUSE_ELF=OFF -DUSE_DEBUGGERS=OFF -DUSE_GDB_STUB=OFF -DUSE_DISCORD_RPC=OFF \
    -DM_CORE_GB=OFF -DBUILD_STATIC=ON -DBUILD_SHARED=OFF -DUSE_EPOXY=OFF -DENABLE_SCRIPTING=OFF \
    -DCMAKE_BUILD_TYPE=Release >/dev/null
  make -j2 mgba >/dev/null
fi
M="$W/mgba-src"
gcc -O2 -o "$W/gbashot" "$HERE/gbashot.c" -I"$M/include" -I"$M/build/include" -I"$M/src" \
  "$M/build/libmgba.a" -lz -lpng -lm -lpthread
echo "$W/gbashot"
