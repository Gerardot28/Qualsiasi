# trainer_sprites: front pics for every League, all regions

Collects, converts and checks GBA trainer front pics for the Gym Leaders,
Elite Four, Champions, Kahunas/Captains, professors and villain bosses of
Kanto through Paldea, in the format pokeemerald-expansion 1.17.1 uses in
`graphics/trainers/front_pics/`.

The tools only read the expansion tree (`/home/user/pex-orig`). They never
write into `/home/user/pex`. All output goes to `/home/user/work/trainer-sprites/`.

## Engine constraints (checked in the expansion source)

| what | where | value |
|------|-------|-------|
| pic size | `include/data.h` `TRAINER_PIC_WIDTH/HEIGHT` | 64 x 64, 4bpp (`TRAINER_PIC_SIZE` = 2048 bytes per frame) |
| palette | `struct TrainerFrontPicInfo` has one `paletteData`, loaded with `PLTT_SIZE_4BPP` (`src/trainer_pokemon_sprites.c`) | **one** 16-colour palette, index 0 transparent |
| frames | `MAX_TRAINER_PIC_FRAMES` (`include/data.h`) | up to 4 frames of 64x64 (optional `.animation`), all sharing the same palette |
| FRLG pics in the Emerald build | `src/data/graphics/trainers.h`, `include/constants/trainers.h` | no `IS_FRLG` guard. All `*_FRLG` pics and constants are always compiled, so they work in the Emerald build |

Pics larger than 64x64, or pics with 2 palettes, are not supported without
engine changes (sprite templates, `TRAINER_PIC_SIZE` buffers, the mugshot
and Pokénav code all assume a single 64x64 4bpp picture).

## Files

| file | purpose |
|------|---------|
| `fetch_sources.sh [DEST]` | shallow, blobless, sparse clones of every source repository. Only trainer graphics, palette tables and credit files are checked out. |
| `convert.py` | the converter and validator. It also writes contact sheets. |
| `sources.py` | finds the palette that a hack actually loads for a PNG: it reads `src/data/graphics/trainers.h`, or a sibling `X.pal` in asset packs |
| `roster.py` | the 153 canonical characters, with region, role and filename patterns |
| `survey.py SRC OUT` | finds every candidate for every character across all sources and renders comparison sheets (`OUT/<region>_N.png`, `candidates.json`) |
| `build_set.py SRC OUT` | builds the selected set: primary picks, alternates, `manifest.csv/json`, `CREDITS.md`, `SOURCES.txt` (pinned commits), contact sheets |

```sh
T=/home/user/Qualsiasi/tools/trainer_sprites
$T/fetch_sources.sh /home/user/work/trainer-sprites/src
python3 -I $T/survey.py    /home/user/work/trainer-sprites/src /home/user/work/trainer-sprites/survey
python3 -I $T/build_set.py /home/user/work/trainer-sprites/src /home/user/work/trainer-sprites/out
python3 -I $T/convert.py check /home/user/work/trainer-sprites/out/*/*.png
python3 -I $T/convert.py one SRC.png OUT.png [--pal X.pal] [--frame 0,0,80,80] [--fit scale|crop] [--max-trim 4]
python3 -I $T/convert.py sheet SHEET.png a.png b.png ... --zoom 4
```

The build is deterministic. A fresh `fetch_sources.sh` followed by
`build_set.py` gives byte-identical PNGs.

## What `convert.py` does

1. **Load.** Reads indexed PNGs (embedded palette, or an external JASC `.pal`
   or `.gbapal`), RGBA PNGs and flat-background rips. The corner colour is
   keyed out. It can also take a sub-rectangle, for example frame 0 of a
   Platinum `front.png` sheet. Palette index 0 is always treated as
   transparent. A tRNS chunk is honoured only if it really hides an index.
   Some fan PNGs carry an all-opaque tRNS, or an integer tRNS pointing at a
   different index (Graphics Gale exports).
2. **Fit into 64x64.** Characters stand on the bottom row and are centred.
   - The opaque bounding box fits: pure re-placement, pixel-exact (`crop`).
   - It overflows by 4 px or less: the sparsest edge rows or columns are
     shaved off, still pixel-exact (`crop-trim`). This covers many HGSS and
     BW sprites that are 65 to 68 px.
   - Otherwise, scale by `64/size`, usually 0.80 to 0.89 for 80x80 DS
     sprites (`scale`). The resampler is an **area-coverage majority vote**.
     It only outputs colours that exist in the source, with no blending and
     no new shades. The darkest 20% of colours (the line art) get a 1.6x
     vote boost so 1-px outlines survive. A pixel stays opaque at 40% or
     more coverage, so thin strands and eyes are kept. Isolated stray pixels
     and 1-px holes are cleaned afterwards. This was compared against
     phase-optimised nearest-neighbour and a 1:1 crop that cuts the legs:
     majority gave the cleanest outlines, and full body matches the GBA
     trainer-pic look.
3. **Colours.** Snaps to BGR555 the way gbagfx does (`x/8`) and merges
   duplicates. If more than 15 colours are left, it merges the closest
   CIELAB pair, cheapest first (distance x sqrt(population of the rarer
   colour)), until there are 15. The source palette order is kept when
   there is one.
4. **Write.** 64x64, PNG colour type 3, bit depth 4, 16 palette slots.
   Index 0 is (115,197,164), the transparent colour of the stock pics.
5. **Check.** Size, indexed 4-bit, indices < 16, transparent corners, no
   visible colours that collide after BGR555 truncation, and the transparent
   colour not reused. All outputs were also run through the expansion's own
   `gbagfx` (`.4bpp` = 2048 B, `.gbapal` = 32 B): 303 out of 303 OK.

## Source priority used by `build_set.py`

1. Already in the expansion (official GBA art, has a `TRAINER_PIC_*`
   constant). It is not copied, only rendered in `_existing_review/` for
   the sheets.
2. An official DS sprite that fits losslessly (`crop` or `crop-trim`):
   Platinum from pret/pokeplatinum, or DP, HGSS and BW from
   smogon/sprites "canonical".
3. Hand-made GBA conversions. These are preferred to automatic
   downscaling: Team Aqua's Asset Repo (TAAR, explicit "free to use and
   edit, credit the author"), then Pokémon Emerald Rogue, Heart & Soul, and
   sinnoh-remakes.
4. Automatic downscale of an official DS sprite.
5. Automatic downscale of a fan-made DS-style sprite (Pokémon Showdown set).
   This is only used for characters that have no GBA art anywhere: Elm,
   Rowan, Juniper, Sycamore, Magnolia, Sonia, Sada, Turo, Clavell, Rose,
   Lusamine, Guzma and Colress.

Change a pick by reordering its list in `SEL` in `build_set.py`. Every
non-primary candidate is already built in `out/_alternates/`.

## Adding a pic to the expansion (not done by these tools)

1. Copy `out/<region>/<key>.png` to `graphics/trainers/front_pics/<key>.png`.
2. `src/data/graphics/trainers.h`: add
   `const u32 gTrainerFrontPic_X[] = INCGFX_U32("graphics/trainers/front_pics/<key>.png", ".4bpp.smol");`
   and `const u16 gTrainerPalette_X[] = INCGFX_U16("graphics/trainers/front_pics/<key>.png", ".gbapal");`
3. `include/constants/trainers.h`: add `TRAINER_PIC_X,` before `TRAINER_PIC_COUNT`.
4. Add `[TRAINER_PIC_X] = { .frontPic = TRAINER_FRONT_PIC(gTrainerFrontPic_X, gTrainerPalette_X), },`
   to `gTrainerPicInfo`.

Prof. Birch needs no copy. `graphics/birch_speech/birch.png` is already a
64x64, 16-colour pic with an embedded palette (it is compiled in
`src/field_effect.c`). Point a new `TRAINER_PIC_PROF_BIRCH` at that same PNG.

## Licence notes (details per pic in `out/manifest.csv` and `out/CREDITS.md`)

- Official Game Freak art (the expansion's own pics, Platinum, and DS/BW
  "canonical" sprites) has the same status as the rest of the base-ROM assets.
- TAAR: free to use and edit, credit the original creator named by the folder.
- Emerald Rogue: no LICENSE file. Credit the project and its "Additional
  Sprites" roll (`src/data/credits.h`). Per-sprite artists are not recorded.
- Heart & Soul: README says it is open source. Credit its sprite artists.
- sinnoh-remakes, and the Showdown fan sprites: no licence, artists not
  recorded. These are used only as alternates or as last-resort picks.
  Verify before a public release.
- Emerald Enhanced: art needs written permission. It is only listed as the
  Lusamine alternate.
