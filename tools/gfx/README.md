# gfx: title screen, icon and cover generator

Generators for the title-screen logo of the pokeemerald-expansion 1.17.x
base, plus the game icon (PNG set and macOS `.icns`), the 3DS icon and
banner, and a box-art cover. The title is "Pokémon Multiverse". Everything
is original pixel art. Shapes and text are rendered at native resolution
with a layered distance-field and selective-anti-aliasing engine
(`pixelart.py`, Python 3 + Pillow + numpy only).

| file | purpose |
|------|---------|
| `make_title.py` | writes `pokemon_logo.png/.pal/.bin`, `emerald_version.png` and optionally `rayquaza_and_clouds.pal`, all in the exact build format; also writes previews and can `--apply` the files to a source tree |
| `make_icon.py` | writes the icon set 16…1024 and `icon.icns`, the 3DS icons 48/24, the 3DS banner 256x128, `cover_512.png` and a preview sheet |
| `capture_title.py` | boots a ROM headlessly and screenshots the title animation, using `tools/emu/run.py` (frame-accurate) or, as a fallback, SDL mGBA under Xvfb |
| `pixelart.py` | shared engine: hinted text coverage, bounded EDT, layer compositing, selective AA, GBA colour snapping, indexed PNG/JASC writers, sparkles |
| `fonts/LilitaOne-Regular.ttf` | display font (SIL OFL 1.1, © 2011 Juan Montoreano, Reserved Font Name "Lilita"); licence in `fonts/OFL-lilitaone.txt`. Only rendered glyph shapes reach the game; the OFL allows redistributing the font with its licence |
| `patches/title_theme_colors.patch` | **optional** 2-constant C change that recolours the Emerald-green flash and the Rayquaza marking pulse (see below). It is not applied by the generators |

## Regenerate for a new word

```sh
cd /home/user/Qualsiasi
W=MULTIVERSE                         # letters only, <= 10 (À È É Ì Ò Ù accents supported)
TREE=/home/user/work/gfx-tree        # your build copy, never pex-orig
python3 tools/gfx/make_title.py --word $W --out /home/user/work/gfx-out/title \
        --preview /home/user/work/gfx-out/title/preview --apply $TREE
make -C $TREE -j4
python3 tools/gfx/capture_title.py $TREE/pokeemerald.gba /home/user/work/gfx-out/screenshots
python3 tools/gfx/make_icon.py --word $W --out /home/user/work/gfx-out/icons
```

Options:

- `--scheme cosmic|eclipse` picks the palette. `cosmic` is the default:
  indigo/violet space, a prismatic word, gold rims.
- `--color KEY=#hex[,#hex…]` overrides a colour or a ramp. A ramp given with
  a different number of stops is interpolated. Colour keys: `outline extrude
  gold prism top_fill top_rim sky silhouette cloud_hi cloud_lo`.
  Example: `--color prism=#ffffff,#ffe680,#ff9a3c,#d64ad8`.
- `--layout stacked|banner` chooses where the word goes (see "Design
  decision").
- `--word-cap` / `--top-cap` force the cap heights. By default the largest
  size that fits is chosen automatically.
- `--top TEXT` sets the top wordmark (default `POKÉMON`).
- `--no-bg-recolor` keeps the original teal Rayquaza/cloud palette.

`make_title.py` validates its own output: the PLTE sizes, the colour budget,
the index range, a blank tile 0 and the identity tilemap.

## How the title screen is built (pokeemerald-expansion 1.17.1)

Sources: `src/title_screen.c`, `src/graphics.c` (lines ~2109-2117),
`graphics_file_rules.mk`, `Makefile`, `tools/gbagfx`, and the
`INCGFX` handling in `tools/preproc` and `tools/scaninc`. The title assets
are not listed in `spritesheet_rules.mk`. Conversion flags come from the
`INCGFX_*("file.png", ".ext", "flags")` declarations in `graphics.c`, and
`scaninc` turns those into make rules that write to
`build/assets/graphics/title_screen/…`.

### Layers (DISPCNT mode 1)

| layer | asset | format | VRAM | priority / effects |
|-------|-------|--------|------|--------------------|
| BG2 (affine 256x256) | `pokemon_logo.png` + `pokemon_logo.bin` | 8bpp tiles `.8bpp.smol`; map `.smolTM` | char block 0, screen block 9 | prio 1 |
| BG0 (text 256x256) | `rayquaza.png` + `rayquaza.bin` | 4bpp, palette 14 | char 2, screen 26 | prio 3 |
| BG1 (text 256x256) | `clouds.png` + `clouds.bin` | 4bpp, palette 14 | char 3, screen 27 | prio 2; sine wave on BG1HOFS (scanline effect); VOFS scrolls slowly in phase 3 |
| OBJ x2 (64x32 8bpp) | `emerald_version.png`, the "version banner" | `.8bpp.smol -mwidth 8 -mheight 4`, sheet 0x1000 | OBJ palette 0 | prio 0 |
| OBJ x10 (32x8 4bpp) | `press_start.png` ("PRESS START" + "©2005 GAMEFREAK inc.") | `.4bpp.smol -mwidth 4 -mheight 1 -num_tiles 48` | its own PNG palette in OBJ slot 9 | blinks every 16 frames |
| OBJ (64x64 4bpp) | `logo_shine.png` | `.4bpp.smol`, sheet 0x800 | **OBJ window**, so its colours are irrelevant | — |

### `pokemon_logo.png` (main logo, BG2)

- **256x64, indexed, 8bpp.** It is 32x8 = 256 tiles of 64 bytes, which fills
  char block 0 exactly (16 KiB). 256 is also the hard limit, because affine
  maps use 8-bit tile numbers.
- **The palette does not come from the PNG.** `pokemon_logo.gbapal` is built
  from **`pokemon_logo.pal`** (JASC, 256 entries) with `-num_colors 224`.
  The build reads only the pixel indices from the PNG. Usable indices are
  **1..223**, which are BG palette rows 0-13; index 0 is transparent. Row 14 is
  `rayquaza_and_clouds.pal` (16 colours) and row 15 is unused/black. The
  palettes are loaded with
  `LoadPalette(gTitleScreenBgPalettes, BG_PLTT_ID(0), 15 * PLTT_SIZE_4BPP)`.
  BG colour 0 is the backdrop, which the shine code rewrites.
- **`pokemon_logo.bin` is a fixed, hand-made file**, not generated from the
  PNG. It is 1024 bytes: 32x32 one-byte affine entries holding
  `0,1,…,255` for the first 8 rows (identity) and `0` everywhere else. The
  build only compresses it (`compresSmolTilemap`). Consequence: **tile 0
  (pixels x 0-7, y 0-7) must be fully transparent**, because all 768 empty
  map cells point at it. The generator writes the same identity map and
  asserts that tile 0 is blank.
- **Placement:** `BG2X = -29` means texture x 0 is screen x 29, so texture x
  0..210 is visible and x 211-255 is off-screen. Screen centre x 120 is
  texture x 91, so the widest centred logo is **182 px** (texture x 0..181).
  Vertically, `BG2Y = -32` in phase 1 puts the logo at screen y 32..95.
  Phase 2 slides it up 1 px every 2 frames over 64 frames, and phase 3 pins
  `BG2Y = 0`, so the logo ends at screen y 0..63.

### `emerald_version.png` (version banner)

- **128x32, indexed, exactly 16 PLTE entries**, converted to **8bpp**. The
  PNG's own palette is the `.gbapal`, and it is loaded with
  `LoadPalette(…, OBJ_PLTT_ID(0), PLTT_SIZE_4BPP)`, so only 16 colours exist.
  Pixel values must stay **≤ 15**: higher values would read other OBJ palette
  rows, for example slot 9, which holds PRESS START. That leaves 15 colours
  plus transparency.
- `-mwidth 8 -mheight 4` splits it into two 64x32 halves (right half at
  tile offset 64). The sprites are centred at x 98 and x 162, so the banner
  covers **screen x 66..193 (its centre is 130, 10 px right of the screen
  centre)**.
- They are created at the end of phase 1 at y = 2 and move down 1 px per
  frame to **y = 66 (top-left 50)**, fading in through `gTitleScreenAlphaBlend`
  (BLDCNT OBJ blend over everything). They are drawn above the logo, and in
  the original layout they overlap its bottom 14 px.

### Logo shine

- `logo_shine.png` (64x64 4bpp, two diagonal stripes) is an **OBJ-window**
  sprite at y = 68 that moves right 4 px per frame (8 in "fast" mode). Inside
  the window, BLDCNT `TGT1_BG2 | LIGHTEN` with `BLDY = 12` brightens the logo
  pixels 12/16 of the way to white. The backdrop is not a target, so only
  opaque logo pixels flash.
- While the logo sits at y 32, the window covers **texture y 4..67**.
  Anything the logo needs shined must be inside texture y 4..63.
- There are three passes: one when the fade-in ends (no backdrop change),
  then a double pass at phase-1 frame 80 and a single pass at frame 192. During
  the double and single passes the **backdrop (BG colour 0) ramps grey to
  white to black and flashes `RGB(24, 31, 12)` green for 4 frames**. This is
  hard-coded in C. The logo therefore has to read on black, light grey, white
  and lime. Here that is handled by a dark outermost outline around light
  rims.

### Background (BG0/BG1)

- `rayquaza.png` (128x128) and `clouds.png` (128x56) both use palette 14.
  Some blank cells of `rayquaza.bin` carry palette bits 0/2/3/5; this is
  harmless because they point at the empty tile 0.
- Palette indices in `rayquaza_and_clouds.pal`:

  | index | use |
  |-------|-----|
  | 4..10 | sky gradient, bottom to top |
  | 11 | silhouette |
  | 15 | markings, overwritten every 4 frames by `UpdateLegendaryMarkingColor` (yellow ↔ dark teal) |
  | 2 | white (clouds, eyes) |
  | 12 | cloud fringe |
  | 0/1/3/13/14 | unused by the pixels |

- In phase 3, BG1 is alpha-blended over BG0 and the backdrop with
  `BLDALPHA(6, 15)`.

### Timeline (frames after START skips the intro)

| frames | what happens |
|--------|--------------|
| ~0-40 | white fade-in, then the first shine |
| ~40-300 | phase 1 (256 frames): the double and single shines with the backdrop flashes |
| ~300-445 | phase 2 (144 frames): the banner slides down and fades in while the logo slides up |
| afterwards | phase 3: BG0/BG1 on, PRESS START (y 108) and © line (y 148) |

The expansion's **QUICKSTART HUD** ("SEL New Game", `include/config/quickstart.h`)
sits in the top-right corner, around screen x ≥ 178, y 0-10. The logo stays
clear of it; disable it for release builds.

## Design decision: both lines on the BG logo

With a 10-letter word, the banner sprite (128 px, 15 colours) forces a
~11 px cap height. The `--layout banner` preview shows it is too small for
the hero word. The default **stacked** layout instead puts **POKÉMON (cap
16) above MULTIVERSE (cap 22)** on BG2:

- BG2 has up to 223 colours, enough for the prismatic gradients and AA.
- The shine sweeps both words.
- The tilemap, tile count and code stay exactly as they are.

The banner sprite becomes a **prismatic "rift" flare** with a 4-point star,
centred on screen just under the word (banner x 54, y 20). It keeps the
original reveal: it slides down over the logo and fades in. `--layout banner`
reproduces the original split (word in the sprite, centred on the screen
when it fits in 108 px) and suits words of about 6 letters or fewer.

Logo construction, from the inside out:

1. Fill: POKÉMON in gold; the word with a vertical prismatic ramp plus a
   per-letter hue drift.
2. A 1 px dark inner line. A morphological closing turns narrow counters
   and inter-letter gaps dark instead of flooding them with rim colour.
3. A 1 px rim that follows only the coarse outer silhouette: violet for the
   top line, gold for the word.
4. A 1 px dark outline.
5. A 2-3 px extrusion straight down.
6. Selective AA only between two opaque layers, keeping the hue/saturation of
   the brighter one. The outer edge stays hard because the GBA has no alpha.
7. Highlights on top-facing edges, custom bold accent strokes, a few
   sparkles on letter corners, and a cleanup pass for specks and spurs.

All colours are snapped to GBA 15-bit, so the previews match the hardware.
The recoloured `rayquaza_and_clouds.pal` turns the teal sky into a violet
nebula.

## Icons, cover, 3DS

`make_icon.py` draws an original **rift-portal emblem**:

- an iridescent ring with gold rims;
- a split disc, magenta above a starry void, which nods to a capture ball
  without copying it;
- a thin glowing rift as the equator, with flare tails;
- a gold medallion carrying the word's initial.

Each size is rendered **natively**: 16, 32, 64, 128, and 48/24 for the 3DS.
256/512/1024 are integer upscales of the 128 master, so the pixels stay crisp.

| output | details |
|--------|---------|
| `icon_{16,32,64,128,256,512,1024}.png` | RGBA, rounded tile with a transparent margin |
| `icon.icns` | written by our own writer: `is32/s8mk`, `il32/l8mk` (16/32 @1x, RLE) plus PNG `ic11 ic12 ic07 ic13 ic08 ic14 ic09 ic10`. Pillow's writer drops the 16/32 @1x entries. Every entry is read back and compared |
| `3ds_icon_48.png`, `3ds_icon_24.png` | opaque RGB, full bleed. SMDH icons are RGB565 with no alpha |
| `3ds_banner_256x128.png` | opaque RGB banner image for injectors (e.g. NSUI custom banner); important content kept centred |
| `cover_512.png` | box art: native 256x256 doubled. Nebula, portals ("other universes"), central vortex portal with the rift flare, the full title logo (rendered natively at a larger size) and a gold frame |

## Optional C patch (theme colours)

`patches/title_theme_colors.patch` changes two Emerald-specific colours in
`src/title_screen.c`:

- the 4-frame backdrop flash goes from `RGB(24,31,12)` green to
  `RGB(25,18,31)` light violet;
- the Rayquaza marking pulse goes from yellow ↔ teal to gold ↔ violet.

It was tested in a build copy; the screenshots are in
`/home/user/work/gfx-out/screenshots_optional_patch/`. Apply it with
`git -C TREE apply tools/gfx/patches/title_theme_colors.patch`.

## Other notes

- The ROM header title is the Makefile variable `TITLE ?= POKEMON EMER`.
  For "PKMULTIVERSE", build with `make TITLE=PKMULTIVERSE` or change the
  default; keep `GAME_CODE = BPEE`. The generators do not touch it.
- `press_start.png` ("PRESS START" / "©2005 GAMEFREAK inc.") is unchanged.
- **mGBA under Xvfb:** `/usr/games/mgba-qt` shows a blank widget under
  Xvfb, so `capture_title.py`'s fallback uses the SDL frontend
  (`/usr/games/mgba`). F12 saves exact 240x160 frames. Game keys must be held
  for about 100 ms, because input is polled once per frame.
