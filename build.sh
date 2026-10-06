#!/usr/bin/env bash
# Pokémon Multiverse v1 (Atto 1: Hoenn) - compila la ROM dai sorgenti.
#
#   ./build.sh [CARTELLA_DI_LAVORO]      (default: ./build)
#
# Cosa fa:
#   1. clona rh-hideout/pokeemerald-expansion al tag expansion/1.17.1
#   2. copia sopra i file modificati della hack (cartella hack/)
#   3. esegue `make release` (build ottimizzata, senza menu di debug)
#   4. copia la ROM in out/Pokemon_Multiverse_v1.gba
#
# Funziona su macOS e Linux. Requisiti: git, make, un compilatore C (Xcode CLT
# su macOS, build-essential su Linux), libpng + pkg-config e la toolchain ARM
# (arm-none-eabi-gcc nel PATH, oppure devkitARM con la variabile DEVKITARM).
set -euo pipefail

EXPANSION_REPO="https://github.com/rh-hideout/pokeemerald-expansion.git"
EXPANSION_TAG="expansion/1.17.1"
EXPANSION_COMMIT="bb1093a7"   # commit "1.17.1 Release"

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORK="${1:-$HERE/build}"
SRC="$WORK/pokeemerald-expansion"
OUT="$HERE/out"

die() { echo "ERRORE: $*" >&2; exit 1; }
info() { echo "==> $*"; }

# ---- controlli -------------------------------------------------------------
[ -d "$HERE/hack" ] || die "cartella hack/ non trovata accanto a build.sh"
command -v git >/dev/null || die "git non trovato"
command -v make >/dev/null || die "make non trovato (macOS: xcode-select --install)"
command -v cc >/dev/null || command -v gcc >/dev/null || command -v clang >/dev/null \
    || die "compilatore C non trovato (macOS: xcode-select --install; Linux: build-essential)"
command -v pkg-config >/dev/null || die "pkg-config non trovato (macOS: brew install pkg-config)"
pkg-config --exists libpng || die "libpng non trovata (macOS: brew install libpng; Linux: libpng-dev)"
if [ -n "${DEVKITARM:-}" ] && [ -x "$DEVKITARM/bin/arm-none-eabi-gcc" ]; then
    info "toolchain: devkitARM ($DEVKITARM)"
elif command -v arm-none-eabi-gcc >/dev/null; then
    info "toolchain: $(command -v arm-none-eabi-gcc)"
else
    die "arm-none-eabi-gcc non trovato. macOS: brew install --cask gcc-arm-embedded (oppure devkitPro/devkitARM); Linux: gcc-arm-none-eabi + libnewlib-arm-none-eabi"
fi

if [ "$(uname -s)" = "Darwin" ]; then
    JOBS="$(sysctl -n hw.ncpu 2>/dev/null || echo 4)"
else
    JOBS="$(nproc 2>/dev/null || echo 4)"
fi

# ---- 1. sorgenti dell'expansion -------------------------------------------
mkdir -p "$WORK"
if [ -d "$SRC/.git" ]; then
    info "uso il clone esistente: $SRC (ripristino i file originali)"
    git -C "$SRC" fetch --depth 1 origin "refs/tags/$EXPANSION_TAG:refs/tags/$EXPANSION_TAG" >/dev/null 2>&1 || true
    git -C "$SRC" checkout -q -f "$EXPANSION_TAG"
    git -C "$SRC" clean -q -fd -e build/ -e tools/
else
    info "clono $EXPANSION_REPO ($EXPANSION_TAG)"
    git clone --depth 1 --branch "$EXPANSION_TAG" "$EXPANSION_REPO" "$SRC"
fi
HEAD_COMMIT="$(git -C "$SRC" rev-parse --short=8 HEAD)"
[ "$HEAD_COMMIT" = "$EXPANSION_COMMIT" ] || echo "ATTENZIONE: commit $HEAD_COMMIT, atteso $EXPANSION_COMMIT"

# ---- 2. file della hack ----------------------------------------------------
info "copio i file della hack"
cp -R "$HERE/hack/." "$SRC/"

# ---- 3. compilazione -------------------------------------------------------
info "compilo (make release -j$JOBS): la prima volta servono alcuni minuti"
make -C "$SRC" release -j"$JOBS"

# ---- 4. risultato ----------------------------------------------------------
mkdir -p "$OUT"
cp "$SRC/pokeemerald-release.gba" "$OUT/Pokemon_Multiverse_v1.gba"
info "ROM pronta: $OUT/Pokemon_Multiverse_v1.gba"
if command -v shasum >/dev/null; then
    shasum -a 1 "$OUT/Pokemon_Multiverse_v1.gba"
elif command -v sha1sum >/dev/null; then
    sha1sum "$OUT/Pokemon_Multiverse_v1.gba"
fi
echo "Nota: con un compilatore diverso lo SHA-1 può differire da quello della patch BPS; la ROM funziona lo stesso."
