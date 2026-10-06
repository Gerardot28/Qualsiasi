#!/usr/bin/env bash
# Shallow, blob-less, sparse clones of every repository the trainer-sprite
# build reads. Only the trainer graphics (+ README/credits and the
# src/data/graphics/trainers.h palette tables) are checked out.
#   fetch_sources.sh [DEST]   (default /home/user/work/trainer-sprites/src)
set -euo pipefail
DEST=${1:-/home/user/work/trainer-sprites/src}
mkdir -p "$DEST"; cd "$DEST"

gba_paths=('/README.md' '/CREDITS.md' '/graphics/trainers/front_pics/' '/graphics/trainers/palettes/' '/src/data/graphics/trainers.h')

clone() { # owner/repo dir [branch]
  local url="https://github.com/$1.git" dir=$2 br=${3:-}
  [ -d "$dir/.git" ] || git clone --filter=blob:none --no-checkout --depth 1 ${br:+--branch "$br"} "$url" "$dir"
  git -C "$dir" sparse-checkout init --no-cone >/dev/null
}
sparse() { local dir=$1; shift; git -C "$dir" sparse-checkout set --no-cone "$@"; git -C "$dir" checkout -q HEAD; }

clone Pokabbie/pokeemerald-rogue Pokabbie_pokeemerald-rogue vanilla
sparse Pokabbie_pokeemerald-rogue "${gba_paths[@]}" '/src/data/credits.h'
clone TeamAquasHideout/Team-Aquas-Asset-Repo TAAR
sparse TAAR '/README.md' '/Trainer Front Sprites/'
clone PokemonHnS-Development/pokemonHnS PokemonHnS-Development_pokemonHnS
sparse PokemonHnS-Development_pokemonHnS "${gba_paths[@]}"
clone sinnoh-remakes/pokeemerald-platinum sinnoh-remakes_pokeemerald-platinum
sparse sinnoh-remakes_pokeemerald-platinum "${gba_paths[@]}"
clone Sierraffinity/CrystalDust Sierraffinity_CrystalDust
sparse Sierraffinity_CrystalDust "${gba_paths[@]}"
clone Enhanced-Projects/Emerald-Enhanced Enhanced-Projects_Emerald-Enhanced
sparse Enhanced-Projects_Emerald-Enhanced "${gba_paths[@]}"
for r in BelialClover/RoweRepo evilchinesefood/PKMN-World StrangeQuark/pokeomnis PCG06/pokeyaeeh harmakanna/TARC2; do
  d=${r/\//_}; clone "$r" "$d"; sparse "$d" "${gba_paths[@]}"
done
clone pret/pokeplatinum pokeplatinum
sparse pokeplatinum '/README.md' '/res/trainers/classes/*/front.png' '/res/trainers/classes/*/front_anim.json' '/res/trainers/classes/*/front_cell.json'
clone smogon/sprites smogon_sprites
sparse smogon_sprites '/README.md' '/src/_uncategorized/canonical/trainers/' '/src/_uncategorized/noncanonical/trainers/'
clone DrSeil/Pokefirered_modified DrSeil_Pokefirered_modified
sparse DrSeil_Pokefirered_modified '/README.md' '/graphics/trainers/front_pics/80/'
echo "sources ready in $DEST"
