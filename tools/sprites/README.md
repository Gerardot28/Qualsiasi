# New male protagonist (replaces Brendan)

Spiky red hair (no hat), black hooded bomber with red ribbed hem/cuffs and a grey hood,
white tee, black slim trousers, red/white high-top sneakers, black fingerless gloves.
Target: pokeemerald-expansion 1.17.1. Wherever the game draws the male player (or Brendan as
the rival when the player picks the girl), it now draws this character.

## Layout

| Path | What |
|---|---|
| `make_protagonist.py` | Renders the text grids in `data/` to the indexed 4bpp PNGs and JASC palettes, writes them into a tree, applies the C patches, makes preview sheets |
| `data/palettes.txt` | All palettes (letter, RGB per slot; slot 0 transparent) |
| `data/ow/heads.txt` | Overworld head stamps (`down`, `up`, `side`) shared by every overworld sheet |
| `data/ow/*.txt` | Overworld sheets, one `== frame` block per frame |
| `data/trainer/front.txt`, `back.txt` | 64x64 trainer front pic, 4-frame back pic |
| `data/intro/intro_bike.txt`, `credits.txt` | Title-sequence and credits bike rider |
| `data/icons/region_map.txt`, `frontier_head.txt` | PokéNav/Fly map head, Frontier Pass map head |
| `seed_from_original.py` | Authoring aid: dumps an original PNG as a letter grid. The grids were drawn starting from these dumps. |
| `emu/` | Headless mGBA screenshot harness (`gbashot.c`, `build_harness.sh`, `shots.sh`, scripts) |

### Grid format

Each `== name` block holds one frame: exactly H rows of W letters. A letter is a palette slot
(see `palettes.txt`) and `.` is transparent. Directives go before the rows and run in file order
after the rows are laid down:

* `@stamp NAME X Y [flip] [map=ab,cd]` pastes a head stamp. In a stamp, `' '`/`?` keeps the pixel underneath and `_` clears it.
* `@from FILE N` / `@copy N` start the frame from another frame. Acro Bike frames 0-8 are `@from ow/mach_bike.txt`.
* `@put X Y SEGMENT` paints one row segment on top. Used to redraw fishing rods and Poké Balls that overlap the hair.
* `@outline LETTER` adds an 8-neighbour outline around the silhouette (the white rim of the Frontier head).
* `@flip`, `@shift DX DY`

## Usage

```sh
# write graphics + palettes + C patches into a tree (idempotent; re-running reports "already")
python3 make_protagonist.py --tree /path/to/pokeemerald-expansion
# graphics only, no source changes
python3 make_protagonist.py --tree T --no-code
# before/after preview sheets at 4x (originals read from --source, default --tree)
python3 make_protagonist.py --source /path/to/pristine --previews OUTDIR
python3 make_protagonist.py --check          # validate data only
```

Then build with `make`. To apply to another tree, either run the script on it, or apply
`/home/user/work/sprite-out/protagonist.diff` (`git apply --binary`), which is the full diff
against a pristine 1.17.1. Both were checked to produce identical files.

## Files written

Graphics: `graphics/object_events/pics/people/brendan/{walking,running,mach_bike,acro_bike,surfing,field_move,fishing,watering,decorating,underwater}.png`,
`graphics/trainers/front_pics/brendan.png`, `graphics/trainers/back_pics/brendan.png`,
`graphics/intro/scene_2/brendan.png`, `graphics/intro/scene_2/brendan_credits.png`,
`graphics/pokenav/region_map/brendan_icon.png`, `graphics/frontier_pass/map_heads.png` (male half only).

Palettes: `graphics/object_events/palettes/brendan.pal`, `brendan_reflection.pal` (derived),
`brendan_underwater.pal` (new), `graphics/trainers/palettes/brendan.pal`,
`graphics/intro/scene_2/brendan.pal` (new).

C patches (dedicated palettes where Brendan used to share one):

* Underwater: Brendan and May shared `OBJ_EVENT_PAL_TAG_PLAYER_UNDERWATER`. The patch adds `OBJ_EVENT_PAL_TAG_BRENDAN_UNDERWATER` (0x1134, `include/constants/event_objects.h`) and `gObjectEventPal_BrendanUnderwater` (`object_event_graphics.h`, `include/graphics.h`). It registers the palette in `sObjectEventSpritePalettes` and the player reflection sets in `src/event_object_movement.c`, then points `gObjectEventGraphicsInfo_BrendanUnderwater` at it.
* Link Brendan, used for the player sprite in contests and link rooms, drew Brendan's pics with May's palette. It now uses `OBJ_EVENT_PAL_TAG_BRENDAN`.
* Intro bike sprite: `intro/scene_2/player.pal` was shared with May. The patch adds `gIntroBrendan_Pal` (`src/data/graphics/intro_scene.h`, `include/graphics.h`) and uses it for `TAG_BRENDAN` in `gSpritePalettes_IntroPlayerFlygon` (`src/intro_credits_graphics.c`). The bicycle sprite also uses this tag, so the bike slots (1-6, 8, 9, 15) keep their colours.

Left unchanged on purpose:

* `graphics/decorations/brendan.pal` (the decoration put-away cursor only uses skin slot 1 and black, both kept).
* The R/S Brendan sprites (`rs_brendan`, `ruby_sapphire_brendan/`, `brendan_rs`), which only stand for link partners playing Ruby/Sapphire.
* `battle_transitions/unused_brendan.png` (unused).
* The ORAS dowsing sprite (only the dowsing rod and hands, disabled by default).

## Screenshots

```sh
emu/build_harness.sh                 # clones mGBA 0.10.2, builds static libmgba + emu/_mgba/gbashot
emu/shots.sh ROM.gba OUTDIR          # intro.txt + showcase.txt -> OUTDIR/shot_*.png + contact sheet
```

`gbashot ROM SCRIPT [SAV]` runs headless. Script commands are `wait N`, `press KEYS [hold] [release]`,
`hold KEYS N`, `repeat C KEYS ...`, `shot F.ppm`, `savestate/loadstate F` and `rtc MS`. The clock is frozen at
2026-01-01 12:00:01 UTC (override with `GBASHOT_RTC`) so every run takes the same path.
`showcase.txt` starts a new game and takes these shots:

1. Birch's intro with the front pic.
2. The scripted walk in Littleroot.
3. The debug-menu "Cheat start".
4. The clock moved to noon, then free walking and running in Littleroot.
5. The trainer card.
6. Riding the Acro Bike.
7. The Fly map head icon.
8. A debug battle: battle intro and back-pic throw.
