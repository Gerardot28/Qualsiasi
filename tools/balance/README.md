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
  seeded noise, +6 when the family is not covered yet, −0.9 per previous use,
  −3 when used in one of the 4 previously generated tables (nearby maps),
  +1 type match, and a penalty for species far too weak for the level.
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
