# speciesdb

Builds a JSON database of every species defined in pokeemerald-expansion
(1.17.1), plus a family table. Other tools use it to generate wild encounters
and trainer parties.

```
python3 -I tools/speciesdb/speciesdb.py [--root /home/user/pex-orig] \
        [--out /home/user/work/species.json] [--families /home/user/work/families.json] \
        [-D EXTRA_DEFINE] [--keep-preprocessed file.i] [--quiet]
python3 -I tools/speciesdb/validate_rom.py --rom baseline.gba --elf pokeemerald.elf [--db species.json]
python3 -I tools/speciesdb/sdb.py PIKACHU SPECIES_ROTOM_WASH      # quick lookup
```

The script exits with code 1 if any built-in sanity check fails. It takes about 6 s.
Requirements: python3, and `arm-none-eabi-cpp` (it falls back to `cpp` or `gcc -E`).
`validate_rom.py` also needs `arm-none-eabi-gcc`, `objcopy` and `nm`.

## How it works

1. **The real C preprocessor.** The script runs `src/pokemon.c` through the C
   preprocessor with the Makefile's `CPPFLAGS`:
   `-iquote include -Wno-trigraphs -DMODERN=1 -DTESTING=0 -DEMERALD -std=gnu17`.
   `pokemon.c` includes `species_info.h`, the level-up learnset file selected by
   `P_LVL_UP_LEARNSETS`, egg moves, form species/change tables and fusion tables.
   Because of this, every `#if P_FAMILY_*`, `P_GEN_*`, `P_UPDATED_*` or `GEN_LATEST`
   comparison, and every macro (`MON_TYPES`, `EVOLUTION`, `CONDITIONS`,
   `PERCENT_FEMALE`, ...), is evaluated exactly as the build evaluates it.
   Some generated headers do not exist in a clean tree (`map_groups.h`,
   `constants/map_groups.h`, `constants/region_map_sections.h`,
   `constants/map_event_ids.h`, `constants/layouts.h`,
   `data/pokemon/teachable_learnsets.h`). The script replaces them with empty
   stubs, so `MAP_*` and `MAPSEC_*` names stay symbolic in the output.
2. **A small C parser.** `cparse.py` tokenizes the preprocessed output. It
   evaluates every `enum` (species, national dex, types, moves, items, ...),
   parses designated initializers and compound literals, and evaluates constant
   expressions with C semantics (integer division, float to u8 truncation for
   `genderRatio`, `?:`, casts).
3. **Config snapshot.** Every object-like `P_*` config macro is evaluated and
   stored in `meta.configs`. A short summary is in `meta.configSummary`. On the
   unmodified tree, all 9 `P_GEN_x_POKEMON` flags, all 539 `P_FAMILY_*` flags and
   all form flags (mega, primal, gmax, tera, fusion, regional, Pikachu forms,
   cross-gen evos) are `TRUE`. `P_LVL_UP_LEARNSETS` is `GEN_LATEST`, which means
   the gen 9 (SV) learnsets. Any disabled flag is reported as an anomaly.
4. **Derived data and classification**, described below.

### Validation against the compiled ROM

`validate_rom.py` compiles a probe object with `arm-none-eabi-gcc`. The probe
gives the exact `struct SpeciesInfo`, `Evolution`, `EvolutionParam`,
`LevelUpMove` and `FormChange` layouts, including bitfield positions. The script
then reads `gSpeciesInfo` from a built ROM, using the ELF symbol address, and
follows every pointer it holds. Result on the baseline build of 1.17.1:

```
gSpeciesInfo @086DB95C, 1574 entries of 264 bytes
checked: 1571 species, 97489 fields, 695 evolutions, 291 evolution conditions,
         22955 level-up moves, 4387 egg moves, 8857 form-table entries, 2481 form changes
MISMATCHES: 0
```

The comparison covers stats, EV yields, types, abilities, catch rate, exp
yield, gender ratio, egg data, growth rate, dex number, the name (encoded
through `charmap.txt`), all flag bits, evolutions and their conditions,
learnsets, egg moves, form tables and form changes. The set of populated
`gSpeciesInfo` entries in the ROM is identical to the set of DB entries.
Only 3 symbols could not be compared, because their headers are generated:
`MAP_PETALBURG_WOODS`, `MAP_SHOAL_CAVE_LOW_TIDE_ICE_ROOM` and
`MAPSEC_NEW_MAUVILLE`. A mutation test confirms the validator works: 7
deliberate edits to a copy of the JSON produced 7 mismatches.

## Output: `species.json`

The file has the form `{"meta": {...}, "species": {CONSTANT: record}}`. Records
follow the species enum order. There is one record for each `[SPECIES_X]` entry
of `gSpeciesInfo`, which is 1571 on 1.17.1; `SPECIES_NONE` and `SPECIES_EGG` are
skipped. Enum aliases such as `SPECIES_CASTFORM = SPECIES_CASTFORM_NORMAL` are
listed in `aliases`, and `sdb.py` resolves them.

`meta` contains the git commit, the cpp command, the stubbed headers, the
configs, the `counts` summary, `sanityChecks` and `anomalies`.

Main record fields:

| field | meaning |
|---|---|
| `constant`, `id`, `aliases` | Species constant, its enum value, and its aliases. |
| `speciesName`, `natDexNum`, `natDexConstant` | Name and national dex number. |
| `generation` | Generation from the dex number: 1-151, 152-251, 252-386, 387-493, 494-649, 650-721, 722-809, 810-905 (includes the Legends Arceus dex 899-905), 906-1025. Regional forms get their base species' generation. `formIntroGen` gives 7/8/9 for Alolan/Galarian+Hisuian/Paldean forms. |
| `types` | Deduplicated `TYPE_*` list. |
| `baseStats` `{hp,atk,def,spe,spa,spd}`, `bst`, `evYield` | Base stats, their total, and EV yield. |
| `abilities` | `[slot1, slot2, hidden]`, padded with `ABILITY_NONE`. |
| `catchRate`, `expYield`, `growthRate`, `eggGroups`, `eggCycles`, `friendship` | As in the source. |
| `genderRatio` (raw u8), `gender` (`male`/`female`/`genderless`/`mixed`), `femalePercent` | Gender data. |
| `itemCommon`, `itemRare`, `height`, `weight`, `categoryName`, `bodyColor`, `perfectIVCount`, `forceTeraType` | As in the source. |
| `flags` | Every flag bit: `isRestrictedLegendary`, `isSubLegendary`, `isMythical`, `isUltraBeast`, `isParadox`, `isTotem`, `isMegaEvolution`, `isPrimalReversion`, `isUltraBurst`, `isGigantamax`, `isTeraForm`, `isAlolanForm`, `isGalarianForm`, `isHisuianForm`, `isPaldeanForm`, `cannotBeTraded`, `dexForceRequired`, `isFrontierBanned`, ... |
| `isLegendary` (= restricted OR sub-legendary), `isRestrictedLegendary`, `isSubLegendary`, `isMythical`, `isUltraBeast`, `isParadox`, `isLegendaryish` | Top-level copies of the flags. `isLegendaryish` is true for any of legendary, mythical, Ultra Beast or Paradox. |
| `isMegaEvolution`, `isPrimalReversion`, `isUltraBurst`, `isGigantamax`, `isTotem`, `isTeraForm`, `is{Alolan,Galarian,Hisuian,Paldean}Form`, `isRegionalForm`, `region` | Form flags. |
| `evolutions` | List of `{method, param, target, conditions:[{condition, args}], category, requiresRegion, forbiddenRegion, requiresLocation, targetDefined}`. `category` is `level` (EVO_LEVEL or EVO_LEVEL_BATTLE_ONLY with a level), `friendship`, `item` (stone, or level-up holding an item), `trade`, `other` (move known, party member, map, spin, script, ...), or `breed` (EVO_NONE, not a real evolution). |
| `canEvolve`, `finalStage`, `evolvesInto`, `evolvesAtLevel` | Whether the species can evolve, the targets, and the lowest level among its level-based evolutions. |
| `evoMethodSummary` | Categories of its own evolutions, for example `"item/friendship/other"` for Eevee. |
| `preEvolution`, `preEvolutions`, `preEvolutionLink`, `breedOnlyPreEvolutions` | The chosen parent and all parents. `preEvolutionLink` is `evolution` or `breed`; `breed` is only used through EVO_NONE, for Ursaluna-Bloodmoon and the Totems. |
| `evolvedBy`, `evolvesFromLevel` | How the species is reached from its pre-evolution. Ivysaur: `level`, 16. |
| `evolutionRequiresRegion`, `evolutionRequiresLocation` | Set when every way to evolve into this species needs a region (for example `REGION_ALOLA` for Raichu-Alola) or a map. |
| `familyRoot`, `chainDepth` | Root of the evolution family, and the number of steps from it (see below). |
| `stage` | Official stage: 0 = basic, 1, 2. **Babies and the basic they evolve into are both stage 0**: Pichu 0, Pikachu 0, Raichu 1. |
| `isBaby` | One of the 19 baby Pokémon: Pichu, Cleffa, Igglybuff, Togepi, Tyrogue, Smoochum, Elekid, Magby, Azurill, Wynaut, Budew, Chingling, Bonsly, Mime Jr., Happiny, Munchlax, Riolu, Mantyke, Toxel. The list is checked against a heuristic (Undiscovered egg group, family root, evolves, not legendary). Gimmighoul is the only expected exception. |
| `isStarter` | The family root is one of the 27 starters (Bulbasaur ... Quaxly), so Hisuian starter evolutions count too. |
| `suggestedMinLevel` | **Heuristic.** Root = 1. A basic that evolves from a baby = 1. Level evolution = its level. Any other method = max(parent + 5, 20 for stage 1, 30 for stage 2). Alternate forms copy their base form. |
| `levelUpLearnset` `[[level, MOVE_X], ...]`, `eggMoves` | From the learnset file the config selects (gen 9). Teachable (TM/tutor) learnsets are not included, because `teachable_learnsets.h` is generated at build time. |
| `forms`, `baseForm`, `formOf`, `formIndex` | From `formSpeciesIdTable`. The first entry is the base form. |
| `formChanges`, `formChangesInto` | The form change table, and the changes that lead into this form. |
| `fusion` | `{item, components}` for Kyurem-B/W, Necrozma-DM/DW and Calyrex-Ice/Shadow. |
| `formKind` | `base`, `regional`, `alternate`, `cosmetic`, `mega`, `primal`, `ultraBurst`, `gigantamax`, `totem`, `tera`, `battleOnly`, `heldItem`, `fusion` or `special`. |
| `cosmeticOf`, `requiredItem`, `requiredItems`, `isBattleOnly`, `isCosmetic` | See `formKind`. |
| `encounterable`, `excludeReason` | See the next section. |

### `encounterable` rules (applied in order)

A species is encounterable when it may appear as a wild or trainer Pokémon in
normal gameplay. The first matching rule excludes a species:

1. **Gimmick flags** → `mega`, `primal`, `ultraBurst`, `gigantamax`, `totem`, `tera`.
2. **Fusion results** (from `gFusionTablePointers`) → `fusion`. This covers
   Kyurem-White/Black, Necrozma-Dusk Mane/Dawn Wings and Calyrex-Ice/Shadow. A
   trainer generator can opt in to them; the components and item are in `fusion`.
3. **Battle-only and held-item forms**, decided from `formChangesInto`. The rule
   only applies to a non-base form that no evolution leads to. Changes coming
   from forms that are already battle-only are ignored, and the rule repeats
   until nothing changes. If the incoming methods are only `FORM_CHANGE_BATTLE_*`
   or `FORM_CHANGE_BEGIN_BATTLE`, plus FAINT and END_BATTLE reverts, the form is
   `battleOnly`. Examples: Castform weathers, Cherrim-Sunshine, Darmanitan-Zen
   (both), Meloetta-Pirouette, Greninja-Ash, Aegislash-Blade, Xerneas-Active,
   Zygarde-Complete, Wishiwashi-School, Minior-Core, Mimikyu-Busted,
   Cramorant-Gulping/Gorging, Eiscue-Noice, Morpeko-Hangry,
   Zacian/Zamazenta-Crowned, Palafin-Hero and Terapagos-Terastal.
   If the incoming methods are only `FORM_CHANGE_ITEM_HOLD`, the form is
   `heldItem`, and `requiredItem` names the item. Examples: Arceus types,
   Silvally types, Genesect drives, Giratina-Origin, Dialga-Origin,
   Palkia-Origin and the Ogerpon masks. A trainer generator may use these if it
   gives the Pokémon the item.
   A form reached only through FAINT or END_BATTLE reverts is the persistent
   form, so it is kept (for example Greninja-Battle Bond before rule 4).
4. **Explicit list** → `special`: event-only or unobtainable forms. These are the
   Cap and Cosplay Pikachu, Partner Pikachu and Eevee, Spiky-eared Pichu,
   Floette-Eternal, Eternatus-Eternamax, Gimmighoul-Roaming, Greninja-Battle Bond,
   Zygarde Power Construct (10% and 50%) and Minior Core (all colours). The list
   is in `EXPLICIT_EXCLUDE` and `EXPLICIT_EXCLUDE_PATTERNS` at the top of
   `speciesdb.py`.
5. **Cosmetic duplicates** → `cosmetic`. A non-base form is a duplicate when its
   types, base stats and abilities are identical to an earlier kept form of the
   same form table and the same region. `cosmeticOf` names that form. Examples:
   Unown B-?, Vivillon, Spewpa and Scatterbug patterns (Icy Snow is kept),
   Flabébé/Floette/Florges colours (Red is kept), Furfrou trims,
   Burmy/Mothim cloaks, Shellos/Gastrodon East, Deerling/Sawsbuck seasons,
   Keldeo-Resolute, Minior Meteor colours (Red is kept), Magearna-Original,
   Sinistea/Polteageist-Antique, all 62 extra Alcremie variants, Zarude-Dada,
   Dudunsparce three-segment, Maushold family of four, Squawkabilly Blue/White,
   Tatsugiri Droopy/Stretchy, Poltchageist/Sinistcha variants and Pikachu-PhD.
6. Everything else is encounterable. That includes:
   * all 1025 base forms;
   * the 57 regional forms, Paldean Tauros breeds included;
   * 39 distinct alternate forms, each with `formOf` set:
     - Rotom appliances, Oricorio styles, Lycanroc forms and Rockruff-Own Tempo;
     - Urshifu-Rapid Strike, Toxtricity-Low Key, Wormadam cloaks and Basculin stripes;
     - the female forms of Meowstic, Indeedee, Basculegion and Oinkologne;
     - Pumpkaboo and Gourgeist sizes, Therian forms and Shaymin-Sky;
     - Hoopa-Unbound, Zygarde-10%, Ursaluna-Bloodmoon and Squawkabilly-Yellow;
     - Deoxys Attack/Defense/Speed.

   **Deoxys decision:** its forms are kept. They are persistent out-of-battle
   forms (Meteorite item use, like the Rotom catalog) with distinct stats. They
   are mythical anyway, so normal pools filter them out through `isLegendaryish`.

### Families (`familyRoot`)

The root is found by following `preEvolution` links, using only real
evolutions. When a species has several parents, base and regional parents come
first, then the lowest species id; the only case on 1.17.1 is Gholdengo, whose
parent is Gimmighoul-Chest rather than Gimmighoul-Roaming. When there is no
evolution parent, an EVO_NONE breeding link is used instead (Ursaluna-Bloodmoon
→ Ursaring, and the Totems).

A form without any pre-evolution inherits the root of the first form in its
form table that has the same regional flag. So Rotom-Wash goes to Rotom,
Deoxys-Attack to Deoxys, Pumpkaboo-Small to Pumpkaboo-Average,
Rockruff-Own Tempo to Rockruff, and Tauros-Paldea-Blaze to
Tauros-Paldea-Combat. A regional form whose regional chain starts with itself
becomes its own root: Rattata-Alola, Meowth-Galar, Wooper-Paldea and
Sneasel-Hisui are separate families. Regional evolutions of base pre-evolutions
stay in the base family: Raichu-Alola is in Pichu's family, Wyrdeer in
Stantler's, Kleavor in Scyther's and Decidueye-Hisui in Rowlet's.

## Output: `families.json`

```
{"meta": {...}, "families": [{"root", "rootName", "rootEncounterable", "members", "stages",
  "maxStage", "isLegendaryish", "isLegendary", "isMythical", "isUltraBeast", "isParadox",
  "hasBaby", "isStarter", "gens", "natDexNums"}]}
```

Only encounterable members are listed. Every root is encounterable, and a
sanity check enforces this.

## Counts (pokeemerald-expansion 1.17.1, default config)

| | value |
|---|---|
| species entries | **1571** (`enum Species` has 1672 `SPECIES_*` names with 1574 distinct values, NONE and EGG included; the rest are aliases. There are 1025 distinct dex numbers, 1..1025.) |
| encounterable | **1121** (1025 base + 57 regional + 39 alternate). Every dex number 1..1025 has at least one encounterable form. |
| families (encounterable) | **568** (122 contain a legendary, mythical, UB or Paradox) |
| legendary (restricted + sub) | 71 dex numbers (27 restricted + 44 sub-legendary), 80 encounterable entries |
| mythical | 23 dex numbers, 28 encounterable entries |
| Ultra Beasts | 11 |
| Paradox | 20 |
| babies | 19 |
| starters | 27 families = 81 dex numbers |
| form kinds (all entries) | base 1025, regional 57, alternate 39, cosmetic 198, mega 97, gigantamax 34, heldItem 44, battleOnly 28, special 23, totem 12, fusion 6, tera 5, primal 2, ultraBurst 1 |

Per generation (`entries` / `encounterable` / dex numbers / families by the
lowest generation among the members):

| gen | entries | encounterable | dex | families |
|---|---|---|---|---|
| 1 | 238 | 187 | 151 | 95 |
| 2 | 143 | 106 | 100 | 55 |
| 3 | 167 | 140 | 135 | 74 |
| 4 | 153 | 115 | 107 | 45 |
| 5 | 195 | 170 | 156 | 86 |
| 6 | 183 | 84 | 72 | 37 |
| 7 | 148 | 95 | 88 | 56 |
| 8 | 197 | 102 | 96 | 48 |
| 9 | 147 | 122 | 120 | 72 |

## Things the generators must know

* **Region-locked evolutions.** These species can only be *evolved into* when the
  player's current region matches (`IF_REGION`): Raichu-A, Exeggutor-A,
  Marowak-A, Weezing-G, Mr. Mime-G, Typhlosion-H, Samurott-H, Decidueye-H,
  Lilligant-H, Braviary-H, Sliggoo-H, Avalugg-H and Ursaluna. They are listed in
  `meta.counts.regionLockedEvolutions`. The base evolutions carry
  `forbiddenRegion`.
* **Map-locked evolutions.** These reference Hoenn maps: Leafeon in
  MAP_PETALBURG_WOODS, Glaceon and Crabominable in
  MAP_SHOAL_CAVE_LOW_TIDE_ICE_ROOM, and Magnezone, Probopass and Vikavolt in
  MAPSEC_NEW_MAUVILLE. All of them also have a stone alternative. They are
  listed in `meta.counts.locationLockedEvolutions`.
* `suggestedMinLevel` is a heuristic. The exact data is in `evolvesFromLevel`
  and `evolutions`.

## `sdb.py` helper

```python
import sys; sys.path.insert(0, '/home/user/Qualsiasi/tools/speciesdb')
import sdb
db = sdb.load()                         # species.json + families.json from /home/user/work
db['SPECIES_CASTFORM']                  # aliases resolved
db.encounterable(gen=[1, 2], legendaryish=False)
db.family_of('SPECIES_RAICHU_ALOLA')['members']
db.moves_at_level('SPECIES_PIKACHU', 20)    # last 4 level-up moves <= 20
db.evolve_for_level('SPECIES_PICHU', 40, rng)  # follows evolutions using suggestedMinLevel
```

## Files

* `speciesdb.py`: the generator (CLI)
* `cparse.py`: the tokenizer, initializer parser and constant-expression evaluator
* `validate_rom.py`: the field-by-field cross-check against a built ROM and ELF
* `sdb.py`: the loader for other scripts
