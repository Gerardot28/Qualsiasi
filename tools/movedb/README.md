# movedb: move, learnset and item database for pokeemerald-expansion

Reads a pokeemerald-expansion 1.17.x source tree (read-only) and writes JSON
databases for moves, learnsets, TMs/HMs and items. `mdb.py` loads them and adds
helpers: legal moves for a species at a level, and a deterministic "sensible
4-move set" picker for trainer parties.

Only the Python standard library is used. You also need a C preprocessor:
`arm-none-eabi-cpp`, the same one the Makefile uses. The tool falls back to
`cpp`, `gcc -E` or `clang -E`.

## Usage

```sh
python3 -I tools/movedb/movedb.py --root /home/user/pex-orig --out /home/user/work/moves.json
python3 -I tools/movedb/mdb.py --demo                  # 15 sample species, level-up and +TM
python3 -I tools/movedb/mdb.py garchomp 60 --tm --seed 3 --explain
```

A full build takes about 10 s. The builder ends by validating against known
values and exits with status 1 if any check fails.

`movedb.py` options:

| option | meaning |
|---|---|
| `--root` | source tree. It is only read; generated files go to a private build directory |
| `--out` | path for `moves.json`. The other JSON files are written to the same directory |
| `--learnsets`, `--items` | override those two output paths |
| `--build-dir` | scratch directory. Default: `<outdir>/.movedb_build`. It keeps the wrapper `.c`/`.i` files |
| `--game-version` | `-D` define. Default `EMERALD`, as in the Makefile |
| `--cpp` | preprocessor command, e.g. `"gcc -E"` |
| `--keep-build` | keep the shadow tree that was used to generate the teachable learnsets |

From Python:

```python
import sys; sys.path.insert(0, '/home/user/Qualsiasi/tools/movedb')
import mdb
mdb.load_moves()['MOVE_FAKE_OUT']['priority']        # 3
mdb.legal_moves('SPECIES_GEODUDE', 12)               # level-up <= 12 + TM/HM + tutor
mdb.legal_moves('SPECIES_IVYSAUR', 20, include_tm=False, include_egg=True)
mdb.best_moveset('SPECIES_GYARADOS', 40, rng_seed=7)                 # level-up only
mdb.best_moveset('SPECIES_GYARADOS', 40, rng_seed=7, allow_tm=True)  # + TM/HM (+ tutor)
mdb.held_items('HOLD_EFFECT_LEFTOVERS')
```

`$MOVEDB_DIR` changes the data directory. The default is `/home/user/work`.

## How it works

1. **Preprocessing.** Three small wrapper files are preprocessed with the
   Makefile's modern-build `CPPFLAGS`: `-iquote include` (plus `src` and a
   stub directory), `-Wno-trigraphs -DMODERN=1 -DTESTING=0 -DEMERALD
   -std=gnu17`. The wrappers are:
   - `moves_wrap.c`: `data/moves_info.h`, included the way `src/move.c` does it.
   - `items_wrap.c`: `data/items.h`, included the way `src/item.c` does it.
   - `species_wrap.c`: the level-up learnset `#if P_LVL_UP_LEARNSETS` chain,
     copied from `src/pokemon.c`, plus the teachable and egg-move learnsets, the
     form tables and `species_info.h`.

   This resolves every config conditional exactly like the real build. Examples:
   `B_UPDATED_MOVE_DATA >= GEN_6 ? 90 : 95`, `P_FAMILY_*`, the type changes
   behind `P_UPDATED_TYPES`, and which `gen_N.h` level-up file is used.
2. **Generated headers.**
   - mapjson outputs (`map_groups.h`, `constants/map_groups.h`,
     `constants/region_map_sections.h`, ...) are missing from a clean tree. They
     are replaced by empty stubs, which the tables never need.
   - `src/data/pokemon/teachable_learnsets.h` is also git-ignored. The tool
     regenerates it by running the repo's own `tools/learnset_helpers` scripts
     (`make_tutors.py`, `make_teaching_types.py`, `make_teachables.py`) inside a
     shadow tree. The shadow tree uses symlinks to the read-only sources and
     copies of the scripts, so nothing in `--root` is written or touched.
   - If `P_LEARNSET_HELPER_TEACHABLE` is FALSE, the tool uses the committed file
     instead.
3. **Parsing.** The expanded designated initializers (`gMovesInfo`,
   `gItemsInfo`, `gSpeciesInfo`, and the `s*LevelUpLearnset`,
   `s*TeachableLearnset`, `s*EggMoveLearnset` and `s*FormSpeciesIdTable`
   arrays) go through a small C initializer and constant-expression parser.
   Enum-valued fields keep their names (`TYPE_FIRE`, `EFFECT_HIT`). All enums
   are evaluated, so numeric ids are available too. String fields
   (`COMPOUND_STRING("...")`, `ITEM_NAME("...")`) are decoded as UTF-8 text.

Config in effect for pex 1.17.1 defaults: `GEN_LATEST = GEN_9`.

| setting | value |
|---|---|
| `B_UPDATED_MOVE_DATA` | `GEN_LATEST` |
| `P_LVL_UP_LEARNSETS` | `GEN_LATEST`, so `level_up_learnsets/gen_9.h` (Scarlet/Violet) is used |
| `P_TM_LITERACY` | `GEN_LATEST` |
| `P_LEARNSET_HELPER_TEACHABLE` | `TRUE`, so the teachables are generated from `all_learnables.json` |

## Outputs (in the `--out` directory)

### `moves.json`

The file maps each `MOVE_X` to an object. It covers all 935 constants in
`gMovesInfo`, including `MOVE_NONE`, the Z-Moves and the Max Moves.

| field | contents |
|---|---|
| `id`, `name`, `description` | numeric id and display text |
| `type`, `category`, `effect`, `target` | symbol names |
| `power`, `accuracy`, `pp`, `priority` | numbers. `accuracy` 0 means the move never misses |
| `strikeCount` (1 by default), `multiHit` | multi-hit data: Dragon Darts has `strikeCount` 2, Population Bomb 10, Bullet Seed `multiHit` |
| `criticalHitStage`, `alwaysCriticalHit` | critical-hit data |
| `recoil` | % recoil: Double-Edge 33, Head Smash 50 |
| `recharge` | Hyper Beam family |
| `selfKO` | `explosion` flag or `EFFECT_FINAL_GAMBIT`, `MEMENTO`, `HEALING_WISH`, `LUNAR_DANCE` |
| `flags` | names of the true flags: `makesContact`, `soundMove`, `punchingMove`, `bitingMove`, `slicingMove`, `pulseMove`, `windMove`, `danceMove`, `powderMove`, `ballisticMove`, `healingMove`, ... |
| `bans` | `metronomeBanned`, `sketchBanned`, ... |
| `argument` | the effect argument union, e.g. `{"recoilPercentage": 33}`, `{"nonVolatileStatus": "MOVE_EFFECT_BURN"}` |
| `additionalEffects` | list of `{moveEffect, chance, self, attack, spAtk, ..., argument}` |
| `secondary` | readable summary, e.g. `"BURN 10%"`, `"STAT_MINUS(self; defense-1 spDef-1)"`, `"ABSORB(50% drain)"`, `"RECHARGE(self)"` |
| `tmhm` | `"TM26"` if the move is on a TM/HM |

### `learnsets.json`

Keyed by `SPECIES_X`, for every entry in `gSpeciesInfo` (1573, including
forms, Megas and Gigantamax forms):

```json
{"levelup": [[lvl, "MOVE_X"], ...],     // in learnset order; lvl 0 = learned on evolution
 "tm":      ["MOVE_...", ...],          // teachable moves that are on a TM/HM
 "tutor":   ["MOVE_...", ...],          // the other teachable moves (map-script tutors)
 "egg":     ["MOVE_...", ...],          // the species' own eggMoveLearnset (base forms only)
 "egg_inherited": [...], "egg_source": "SPECIES_BULBASAUR",  // evolved/alt forms: family's egg moves
 "prevo": "SPECIES_IVYSAUR", "base_form": "SPECIES_VENUSAUR", // when applicable
 "arrays": {"levelup": "sVenusaurLevelUpLearnset", "teachable": ..., "egg": ...}}
```

A NULL learnset pointer becomes an empty list. The game falls back to
`SPECIES_NONE`'s learnset in that case.

### `tmhm.json`

- `tm_hm`: TM01–TM50 and HM01–HM08 from `FOREACH_TM` and `FOREACH_HM` in
  `include/constants/tms_hms.h`. Each entry has `num`, `item` (`ITEM_TM_X`),
  `item_id`, `move`, `name`, `type`, `power`, `accuracy` and `category`.
- `tutors`: the 30 tutor moves that `make_tutors.py` finds in the map scripts.
- `meta`: the config values and preprocessor that were used.

### `items.json`

Keyed by the `ITEM_X` designator used in `gItemsInfo`. Each entry has:
`id`, `name`, `pluralName`, `price`, `pocket` (`POCKET_*`), `holdEffect`
(`HOLD_EFFECT_*`, or `HOLD_EFFECT_NONE`), `holdEffectParam`, `secondaryId`,
`importance`, `type`, `battleUsage`, `flingPower`, `sortType`, `aliases`
(other enum names with the same value, e.g. `ITEM_TM01` for
`ITEM_TM_FOCUS_PUNCH`) and `description`.

### `movedb_species.json`

Minimal species data: `id`, `name`, `types`, base stats, abilities,
evolutions, `prevo`, `baseForm` and form flags. `mdb.py` uses it as a
fallback when the speciesdb `species.json` is missing or incomplete.

## `mdb.py` API

| function | description |
|---|---|
| `load_moves()`, `load_learnsets()`, `load_items()`, `load_tmhm()` | cached JSON loaders |
| `load_species()` | `{SPECIES_X: {name, types, hp, atk, def, spa, spd, spe}}`. Uses `species.json` (speciesdb) when present, else `movedb_species.json`. If `species.json` has an unresolved numeric type, the type comes from movedb instead |
| `legal_moves(species, level, include_tm=True, include_egg=False, include_tutor=None, include_prevo=False)` | level-up moves learned at or below `level` (level 0, the evolution moves, is always included), then TM/HM, tutor (defaults to `include_tm`) and egg moves (own, else the family's). Returns a de-duplicated list |
| `best_moveset(species, level, rng_seed, allow_tm=False, allow_egg=False, allow_tutor=None, normal_trainer=True, explain=False)` | up to 4 `MOVE_` constants, damaging moves first |
| `held_items(hold_effect=None, pocket=None)` | items that have a hold effect |
| `describe(move)` | e.g. `"Flamethrower (Fire Spec 90/100)"` |

Species names are accepted as `SPECIES_MR_MIME`, `MR_MIME` or `"Mr. Mime"`.

### `best_moveset` heuristic

1. **Score every damaging candidate.** The score is power × hits × STAB(1.5)
   × accuracy × (stat for the move's category ÷ the higher of Atk and SpA)^1.5.
   This favours the species' stronger attacking side; Body Press uses Def.
   - Variable-power moves get nominal powers: Low Kick 60; Gyro Ball and Electro
     Ball from base Speed; Seismic Toss and Night Shade from level; Dragon Rage
     from its fixed damage.
   - Penalties: accuracy below 85 (an extra ×0.85), recoil, self stat drops
     (Overheat ×0.72), two-turn moves, first-turn-only moves (Fake Out),
     Focus Punch, False Swipe, and other awkward effects.
   - With `normal_trainer=True`, recharge moves (Hyper Beam) and self-KO moves
     (Explosion) are excluded. OHKO, Dream Eater, Snore, Last Resort and similar
     situational moves are never picked.
   - A seeded ±8 % jitter decides near-ties, so the result is deterministic for
     a given seed and differs between seeds.
2. **STAB.** Take the best damaging move of each of the species' types.
3. **Coverage.** Take the best damaging move of a new type. It must score at
   least 35 % of the best STAB move.
4. **Status.** Add at most one good status or setup move from the
   `GOOD_STATUS` table, e.g. Swords Dance, Nasty Plot, Dragon Dance, Will-O-Wisp,
   Thunder Wave, Toxic, Spore, Protect, Stealth Rock, Recover or Roost.
   - Physical setup moves need a physical-leaning or mixed attacker that
     already has a physical attack. Special setup moves work the same way.
   - Moves with accuracy below 85 are penalised.
5. **Fill.** Remaining slots are filled in this order:
   1. a new-type attack worth at least 35 % of the best one
   2. one minor status move (Leer, Growl, ...)
   3. same-type attacks
   4. more minor status moves
   5. weak attacks

   Useless moves are never used as filler. Examples: Splash, Celebrate,
   Teleport, Helping Hand, Ally Switch, Transform and Sketch.
6. If nothing qualifies, the last level-up moves are used, so Magikarp gets
   Splash, Ditto Transform and Smeargle Sketch.

## Validation

- `movedb.py` checks 116 hard-coded expectations every time it runs:
  - Moves: Tackle 40/100 Normal Physical with contact. Flamethrower and
    Thunderbolt 90/100 with a 10% burn or paralysis. Earthquake 100. Fake Out
    priority 3 with flinch. Will-O-Wisp: Status, 85 accuracy. Dragon Darts
    `strikeCount` 2. Body Press uses `EFFECT_BODY_PRESS`. Tera Blast: Special,
    80. Population Bomb 20/90 ×10, slicing. Hyper Beam recharges. Explosion is
    self-KO. Double-Edge 33 % recoil. Giga Drain 50 % drain. Make It Rain
    120/100.
  - Learnsets: Bulbasaur, Charizard, Pikachu, Garchomp, Gholdengo, Magikarp,
    Gyarados, Dragapult and Sprigatito.
  - Items: Leftovers, Choice Band, Potion, Poké Ball and Sitrus Berry.
- A one-off cross-check against a ROM compiled from the same 1.17.1 sources
  (`/home/user/pex/pokeemerald.gba`, whose only changes are to caps, item
  config and new game) decoded the `gMovesInfo`, `gItemsInfo` and
  `gSpeciesInfo` structs from the binary. Everything matched:
  - **moves**: 935 moves × 14 fields (13 090 values), including effect, type,
    category, power, accuracy, target, pp, priority, strikeCount, multiHit,
    explosion, crit stage, number of additional effects and all 33 flag bits.
    0 mismatches.
  - **items**: 874 items × 4 fields (price, holdEffect, holdEffectParam,
    pocket). 0 mismatches.
  - **learnsets**: 1573 species × 3 lists (level-up, teachable and egg),
    compared exactly. 0 mismatches.
  - **species**: base stats and types for 1573 species. 0 mismatches.

## Caveats

- The `tm` list is the repo's own TM/HM set: the 50 Emerald TMs and 8 HMs in
  `tms_hms.h`. If a hack edits `FOREACH_TM`, re-run the builder; the teachable
  learnsets are regenerated from the same file.
- Egg moves only exist on base forms. Evolved forms get `egg_inherited`,
  which `legal_moves(include_egg=True)` uses.
- `legal_moves` does not include pre-evolution level-up moves unless you pass
  `include_prevo=True`.
- The `best_moveset` scores are heuristics for trainer generation, not damage
  calculations. Abilities, items and weather are ignored.
