# tools/balance

Balancing tools for Pokémon Multiverse.

## wild_gen.py — wild encounters for Hoenn

```
python3 -I tools/balance/wild_gen.py --root <tree> --out <tree>/src/data/wild_encounters.json
python3 -I tools/balance/check_wild.py --root <tree>      # validation + docs/bilanciamento/incontri_selvatici.md
```

`wild_gen.py` rewrites only the **species and levels** of the Hoenn tables of
`src/data/wild_encounters.json`. Groups, entries, maps, base labels, slot
counts and every `encounter_rate` stay as in vanilla. The Battle Pyramid and
Battle Pike groups and all FRLG/Kanto/Sevii tables (base labels `s…_FireRed` /
`s…_LeafGreen`) are copied unchanged. The input is always the **vanilla** file:
by default `/home/user/pex-orig/src/data/wild_encounters.json` when it exists,
else `<root>/src/data/wild_encounters.json` (use `--input` to override). Do
not feed it its own output: the level curve is not idempotent. The output is
deterministic (fixed seed `--seed`, sorted inputs, string-seeded RNG; the same
bytes on every run). Data: `species.json`, `families.json` and
`movedb_species.json` from `/home/user/work` (`--data`); the ~10 species whose
types were stored as raw numbers (9 = Steel, 19 = Fairy) are repaired from
`movedb_species.json`. Shared rules live in `wild_common.py`.

How a table is built:

* **Levels.** `min_level` and `max_level` of every slot go through the
  monotone piecewise-linear curve `(2,2) (5,6) (12,13) (15,15) (19,19) (24,24)
  (29,29) (31,33) (33,37) (42,44) (46,49) (49,52) (55,57) (58,62) (70,72)
  (100,100)` (rounded half up), fishing included.
* **Slot layout.** Land (12 slots, 20/20/10/10/10/10/5/5/4/4/1/1) uses
  `A B C D A B E F G H I I`: 9 distinct species, two commons at 30%, two at 10%,
  then 5/5/4/4/2%. Post-game land tables use `A B C D A B E E G H I L`, where `L`
  (slot 11, 1%) is the legendary slot. Surf and Rock Smash use 5 distinct
  species; fishing uses 10 (old rod A-B, good rod C-E, super rod F-J).
* **Habitat** (`habitat()`): the map name gives preferred types (forest,
  cave, desert, volcanic, Mt. Pyre ghost, Shoal Cave ice, Sky Pillar
  dragon/flying/psychic, power plant, meadow/coast routes, Safari Zone = any
  type). About 70% of the land letters must match a preferred type, the rest
  are free. Surf = Water type; fishing = Water type with Water 2/3 egg group
  (old rod: basic with BST ≤ 320; good rod: BST ≤ 450; super rod: strongest
  member allowed by the level); Rock Smash = Rock/Ground.
* **Evolution sanity** (`DB.obtainable`): basics (stage 0, not babies) at any
  level; level evolutions from evolution level + 2 (chained); stone, trade,
  friendship and other methods from Lv 30 and only in letters whose total
  rate is ≤ 5%. Babies, gimmick forms (mega, gmax, ...), cosmetic forms and
  alternate forms not reached by evolution are never used.
* **Power curve.** Own-BST cap by slot min level (`bst_cap`: ≤ 330 up to
  Lv 15, then rising to 545 / 560 for rare letters). Letters whose levels are
  all ≤ 15 only take families with final BST ≤ 535, except at most one strong
  family per table, in a single 1% slot. Pseudo-legendary families (final BST
  ≥ 600, Slakoth included) from Lv 30 in 1-5% letters; the 27 starter families
  from Lv 20 in 1-5% letters; at most one starter and one pseudo family per
  table. Legendary, mythical, Ultra Beast and Paradox species only in the
  post-game tables (`POSTGAME_LABELS`: Artisan Cave, Desert Underpass,
  Altering Cave, Sky Pillar 5F, Safari Zone NE/SE), one per table, 1% slot,
  Lv 60-65. Only non-restricted, non-mythical, single-stage, non-Hoenn
  legendaries are picked.
* **Choice and variety.** Each letter scores every allowed family:
  seeded noise, +6 when the family is not covered yet and fits the table's
  habitat (+1 if it does not; no bonus in the Safari Zone, which favours
  rarer, stronger families), −0.9 per previous use, −3 when used in one of
  the 4 previously generated tables (nearby maps), −2 when used on another
  floor/room of the same cave/tower/zone, +1 type match, and a penalty for
  species far too weak for the level. Uncovered Ice families (Alolan Vulpix and
  Sandshrew, Galarian Darumaka and Mr. Mime included) may only debut in Shoal
  Cave.
* **Coverage** (`repair`): every non-legendary family of `families.json`
  (regional families included) must have a basic non-baby member in a Hoenn
  table reachable in normal gameplay. Unreachable tables (`UNREACHABLE_LABELS`:
  unused Cave of Origin maps, Altering Cave sets 2-9, Mirage Island) do not
  count. A repair pass moves any uncovered family into a valid letter whose
  occupant is covered elsewhere.

`check_wild.py` re-checks all of this independently: JSON identical to vanilla
except species/levels, unchanged non-Hoenn tables, `SPECIES_` constants present
in `include/constants/species.h`, `min ≤ max`, levels equal to the curve,
evolution levels, power rules, habitat pools, 6-9 species per land table,
identical neighbouring tables (warning) and coverage. It writes the per-map
summary to `docs/bilanciamento/incontri_selvatici.md` and exits with 1 on errors.

Time-of-day encounter variants are not used: they would add new base labels
and change the JSON structure.

## trainer_gen.py — trainer rebalance for Hoenn

This tool regenerates `src/data/trainers.party` for pokeemerald-expansion 1.17.1.
The goal is high but fair difficulty, with Pokémon from every generation.
Everything is deterministic: the same inputs always give the same file. Only the
Python standard library and `tools/movedb/mdb.py` are used.

### Files

| file | role |
|---|---|
| `trainer_gen.py` | Generator. Reads the vanilla `trainers.party` (read-only tree), rebuilds the regular trainers, merges `bosses.party` and writes the output |
| `bosses_core.party` | Hand-authored bosses: leaders' first battles, Elite Four, Wallace, Steven, Wally (Mauville, Victory Road 1) and Magma/Aqua leaders and admins |
| `boss_templates.py` | Writes `bosses.party`: `bosses_core.party` plus the templated rival teams (30 entries) and leader/Wally rematches `_2`..`_5` (36 entries). Their rosters are hand-picked in the script |
| `bosses.party` | The merged boss file that the generator reads. It is generated, so edit the two sources above |
| `check_trainers.py` | Validator |
| `make_doc.py` | Writes `docs/bilanciamento/allenatori.md`, the Italian boss summary |

### Usage

```sh
python3 -I tools/balance/boss_templates.py                       # only after editing bosses_core.party / rosters
python3 -I tools/balance/trainer_gen.py --root /home/user/pex-orig --out OUT/trainers.party
python3 -I tools/balance/check_trainers.py OUT/trainers.party    # exit 1 on any ERROR
python3 -I tools/balance/make_doc.py OUT/trainers.party > docs/bilanciamento/allenatori.md
```

To validate a build, copy the tree with `cp -a /home/user/pex /home/user/work/trainer-tree`,
put the file in `src/data/` and run `make -j2` there. `trainerproc` must accept
the file. Run the build only in the copy, never in `/home/user/pex`.

### Rules implemented

#### Which trainers are touched

- Every `=== TRAINER_X ===` entry is kept, in the same order.
- Name, Class, Pic, Gender, Music, Double Battle, Mugshot and Multi Party are kept unchanged.
- These entries are copied verbatim:
  - `TRAINER_NONE`.
  - Every trainer that no `data/maps/**`, `data/scripts/**`, `data/*.s|inc` or
    `src/battle_setup.c` references:
    - the frontier brains (Anabel, Tucker, Spenser, Greta, Noland, Lucy, Brandon);
    - Red and Leaf;
    - the May/Brendan placeholders;
    - the unused `CINDY_2`, `DUDLEY`, `KAYLEE`, `AMY_AND_LIV_3`, `GINA_AND_MIA_2`,
      `GRUNT_UNUSED`, `TERRY`, `LUCAS_2` and `MIKE_1`.
  - That makes 21 entries. Kanto/FRLG trainers are in a separate file and are not touched.

#### Regular trainers (742)

- **Level:** the vanilla level is mapped through the curve
  (2,2)(5,6)(12,13)(15,15)(19,19)(24,24)(29,29)(31,33)(33,37)(42,44)(46,49)(49,52)(55,57)(58,62)(70,72)(100,100)
  with linear interpolation. Rematches map their own vanilla levels the same way.
- **Party size:** at least the vanilla size. Double battles get at least 2 Pokémon.
  Cooltrainer, Expert and Dragon Tamer get a third Pokémon when the party is at Lv 33+
  and has fewer than 3.
- **Species:** chosen by class theme (`CLASS_THEME`).
  - Triathletes are split by Pic: swimming, cycling or running.
  - Fishermen use a fish list. Guitarists use a sound-themed list.
- **Species filters:**
  - The candidate pool is every non-legendary, non-mythical, non-UB and non-paradox family.
    `SpeciesDB.BANNED` also removes some gimmick species, for example Shedinja, Smeargle,
    Ditto, Slaking and Magikarp.
  - From each family the generator takes the most evolved member that is legal at the level.
  - A BST band applies: max 300 + 7·Lv (+15 for strong classes) until Lv 44, then 600.
    From Lv 30 the minimum is 380 + 4·(Lv−30), up to 460.
  - Only one Pokémon per family in each party.
- **Legal stage vs. level (`SpeciesDB.min_level`):**
  - Level-up evolutions are legal from their evolution level.
  - Stone, trade, friendship and other evolutions are legal from Lv 30 for a first
    stage and Lv 38 for a second stage.
  - Baby→basic evolutions are free, so Pikachu and Chansey are legal from Lv 1.
  - Pseudo-legendary lines are legal only from Lv 35, and only for strong classes and bosses.
- **Moves:**
  - Moves come from `mdb.best_moveset(normal_trainer=True)`. TMs are allowed for strong
    classes and from Lv 30.
  - Below Lv 30, TM and egg moves stronger than 60 power (under Lv 20) or 75 power
    (under Lv 30) are swapped for level-up moves.
- **IVs:** 8 at Lv 6 rising to 20 at Lv 49. Rematch variants get +2 per tier up to 25,
  and `_4`/`_5` get 25.
- **AI:** `Basic Trainer`. Natures and abilities are left at their defaults.

#### Bosses (92)

- The boss file supplies the party, `Items:` and `AI:`.
- **Moves:**
  - A hand-written move is checked with
    `mdb.legal_moves(include_tm, include_egg, include_tutor, include_prevo)`.
  - If the species cannot learn it, the move is replaced with a `best_moveset` move and
    listed with `-v`.
  - Missing moves are filled the same way.
  - The TM pool is the 50 Emerald TMs only, so many modern moves are not legal. That is
    why about 90 hand-written moves get auto-replaced.
- **IVs:** 31 for Leader, Elite Four, Champion, Steven and Wally (Victory Road).
  Other bosses get the level scale + 6.
- **AI:**
  - Leaders, rivals and admins use `Basic Trainer / Smart Switching / Smart Mon Choices /
    HP Aware / Ace Pokemon` or a subset of it.
  - The Elite Four, Wallace and Steven use `Smart Trainer`.
  - `trainerproc` turns each name into `AI_FLAG_<UPPER_SNAKE>`. All of these flags exist in
    `include/constants/battle_ai.h`.
- **Leaders:**
  - Teams have 3, 4, 4, 5, 5, 5, 6 and 6 Pokémon.
  - The ace is last and is at the cap: 15, 19, 24, 29, 33, 37, 44, 49.
  - The others are 1-3 levels lower.
  - Only the ace holds an item. The trainer carries at most 2 healing items.
- **Elite Four and Champion:** aces at 54, 56, 58, 60 and 62.
- **Rivals:**
  - The vanilla suffix names the **player's** starter slot.
  - `_TREECKO` (player Turtwig) → rival Fuecoco line. `_TORCHIC` (player Fuecoco) →
    Froakie line. `_MUDKIP` (player Froakie) → Turtwig line.
  - The partners are the other two types: Houndour (fire), Chewtle (water) and Bounsweet
    (grass) lines. Rookidee line, Lucario and Hattrem are added later.
  - Team sizes are 1, 3, 4, 5, 6 with aces at Lv 6, 16, 21, 33, 39.
  - May and Brendan are identical.
- **Rematches** (leaders and Wally VR `_2`..`_5`): 6 Pokémon, aces at 60, 65, 70, 75, the
  others 1-3 levels lower, with EVs from `_4`.

### Validation (`check_trainers.py`)

- Same trainer list and order as vanilla, and the protected header fields are unchanged.
- Valid species, move, item, ability (for that species), nature and AI flag constants.
- Levels between 1 and 100, IVs and EVs within range.
- Stage vs. level legality, no legendaries in regular parties, at most 4 moves, and all
  moves legal (level-up, TM, tutor, egg, pre-evolution).
- Party size at least the vanilla size, doubles at least 2, at most 6.
- Ace level equal to the cap for the 8 leaders, the Elite Four and Wallace.

Last run: 855 trainers, 1926 Pokémon, 0 errors, 0 warnings. `make -j2` of the copied
tree succeeds.

### Caveats

- `min_level` is a heuristic. "Other" evolutions count like stones, so Malamar,
  Toxtricity and Magnezone are legal from 30 or 38, and region-locked forms are not
  modelled. Ursaluna is banned for that reason.
- The BST band leaves out strong single-stage species early in the game (Snorlax
  appears only once the band allows it).
- Generated movesets are heuristic (`best_moveset`). They ignore abilities, items and
  weather.
- Rematch rosters and rival partners are templated, so their movesets are not hand-tuned.
