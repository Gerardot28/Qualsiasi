# Kanto inventory — FRLG content shipped in pokeemerald-expansion 1.17.1

Source: read-only tree `/home/user/pex-orig`. All paths are relative to that root.
Scope: every map folder ending in `_Frlg` (419 maps), the FRLG trainer party file, the Kanto wild-encounter tables, and the engine hooks the Kanto scripts need.
Purpose: decide what a **"Kanto act"** of the hack needs, and what the merge into the Emerald (Hoenn) build must solve.

How the numbers were made (by parsing the files below):

* **Map data** comes from `data/maps/<Map>_Frlg/map.json`.
* **NPC** = object events that are not item balls and not field obstacles. **FObj** = boulders, cuttable trees and breakable rocks. **Items** = item-ball objects. **Signs** = `bg_events` of type `sign`. **Hidden** = hidden items. **Trig** = coord triggers. **Warps** = `warp_events`.
* **Trn** = distinct `TRAINER_*` constants (from `include/constants/opponents_frlg.h`) reached by this map. A constant counts if it is in the map's `scripts.inc`, or in a script label used by one of its objects or triggers. The label is followed through `goto`/`call` up to depth 3, which covers `data/scripts/trainers_frlg.inc`, where most route trainers live. Rematch variants (`_2`, `_3`, …) are left out because only the VS Seeker reaches them.
* **Kind**: `town` / `route` / `dungeon` (`MAP_TYPE_UNDERGROUND`, plus the indoor dungeons: Rocket Hideout, Silph Co., Mansion, Tower, S.S. Anne, Power Plant and the Underground tunnels) / `gym` / `league` / `interior`.

---

## 1. Headline numbers

| Block | Maps | Distinct trainers | NPC | FObj | Item balls | Signs | Warps |
|---|--:|--:|--:|--:|--:|--:|--:|
| Kanto mainland, ESSENTIAL areas | 248 | 375 | 891 | 78 | 124 | 415 | 912 |
| Kanto mainland, OPTIONAL / KEY OPTIONAL (Diglett's Cave, Power Plant, Cerulean Cave) | 7 | 0 | 4 | 25 | 15 | 0 | 26 |
| Sevii Islands + Navel Rock + Birth Island (SKIP) | 159 | 99 | 353 | 107 | 39 | 104 | 343 |
| Link rooms (SKIP) | 5 | 0 | 12 | 0 | 0 | 0 | 13 |
| **Total `_Frlg`** | **419** | 474 | | | | | |

* Map types: 260 `MAP_TYPE_INDOOR`, 87 `MAP_TYPE_UNDERGROUND`, 53 `MAP_TYPE_ROUTE`, 19 `MAP_TYPE_TOWN`. There are 99 distinct `MAPSEC_*` sections.
* Trainers: `opponents_frlg.h` defines 623 IDs (`TRAINERS_COUNT_FRLG` 624, `MAX_TRAINERS_COUNT_FRLG` 768). Maps place 474 of them. Most of the other 149 are VS Seeker rematch slots. A few are unused, for example `TRAINER_PKMN_PROF_PROF_OAK`, `TRAINER_TEAM_ROCKET_GRUNT_22` and `TRAINER_COOLTRAINER_*` extras.
* Wild tables: 124 Kanto/Sevii maps have encounter data, each with a FireRed and a LeafGreen version (248 entries out of 388 in `src/data/wild_encounters.json`).
* Map groups: 42 `gMapGroup_*_Frlg` groups in `data/maps/map_groups.json`, after the Emerald groups. Biggest: `Dungeons_Frlg` (123), `TownsAndRoutes_Frlg` (62), `SpecialArea_Frlg` (60).

---

## 2. How Kanto is wired into the tree (what a Hoenn+Kanto ROM must change)

The tree builds **either** Emerald (`make`) **or** FireRed/LeafGreen (`make firered` / `make leafgreen`, which set `-DFIRERED`, so `IS_FRLG = 1`). Kanto content is present but **compiled out of the Emerald build**. The switches are:

| Layer | Where | Emerald build today | What the merge needs |
|---|---|---|---|
| Map headers/groups | `tools/mapjson/mapjson.cpp` (`groups` mode, ~L740-755). Maps with `"region": "REGION_KANTO"` are dropped when the version is `emerald`. | All 419 `_Frlg` maps dropped | Let both regions through (patch mapjson, or rely on `MAP_VERSION` + a new mode). Map IDs do **not** collide: the 14 FRLG maps whose name exists in Emerald are suffixed `_FRLG`. These are `MAP_VICTORY_ROAD_1F_FRLG`, `MAP_SAFARI_ZONE_NORTH_FRLG`, 5 Navel Rock maps, 2 Birth Island maps and 5 link rooms. |
| Layouts | `data/layouts/layouts.json`: `"layout_version": "frlg"` (344 layouts) is skipped by mapjson `layouts emerald` | Dropped | Same patch. No duplicate layout IDs. |
| Map scripts | `data/event_scripts.s` L603-1050 `.if IS_FRLG`: 421 `_Frlg/scripts.inc` plus 21 shared FRLG files (`trainers_frlg.inc`, `silphco_doors.inc`, `pokemon_mansion.inc`, `pokemon_league.inc`, `route23.inc`, `seagallop.inc`, `static_pokemon.inc`, `item_ball_scripts_frlg.inc`, `aide.inc`, `day_care_frlg.inc`, `move_tutors_frlg.inc`, `trainer_tower.inc`, `fame_checker_frlg.inc`, `cable_club_frlg.inc`, `trainer_card_frlg.inc`, `flavor_text.inc`, `pkmn_center_nurse_frlg.inc`, `mystery_event_club.inc`, text files) | Not assembled | Include the needed ones unconditionally. `set_gym_trainers.inc` (L1203) is already unconditional. |
| Tilesets | `src/data/tilesets/headers.h`, `graphics.h`, `metatiles.h`: `#if !IS_FRLG` (Hoenn) `#else` (Kanto). They are mutually exclusive. | Kanto tilesets absent (2 primary `general_frlg`, `building_frlg` + 62 secondary `*_frlg`) | Compile both halves. ROM space is the main cost. |
| Metatile engine | `src/fieldmap.c`: per-layout `isFrlg` flag (u32 attributes, 640-tile primary, variable border) | **Already runtime-mixed** | Nothing to do. Metatile behaviours are already one shared `MB_*` enum: spin tiles, warp pads, currents, thin ice and Strength buttons all resolve to Emerald handlers. |
| Metatile labels | `include/constants/metatile_labels.h` (unguarded) | Present | — |
| NPC sprites | `src/data/object_events/object_event_graphics_info_pointers.h` L648-793, `…_graphics_info.h` L4470-7227 and `…_pic_tables.h` L1358-2496 are `#if IS_FRLG`. Constants in `event_objects.h` are unguarded. | 115 of the 128 graphics IDs used by Kanto maps (1,362 objects) have **no graphics** | Un-guard them, or remap to Emerald look-alikes. The list is in §6.3. |
| Door animations | `src/field_door.c` L152-187 / L315-350 / L354-1027 | FRLG doors absent | Un-guard. |
| Flags | `include/constants/flags.h` L48: `#if IS_FRLG` includes `flags_frlg.h` **instead of** Emerald flags. Same numeric space. `FLAG_BADGE01..08_GET` are the same names. | FRLG flag names undefined | Renumber FRLG flags into free or extended space. Kanto badges need 8 new badge flags, and every `FLAG_BADGE0x_GET` in Kanto scripts must be retargeted (gym scripts, Route 22/23 guards, Viridian Gym door). |
| Vars | `include/constants/vars.h` always includes `vars_frlg.h` (252 defines). The names are distinct but the numbers **overlap** Emerald vars (0x4020-0x40FF). | Defined but aliasing Hoenn vars | Renumber. |
| Trainers | `opponents.h` always includes `opponents_frlg.h`. The IDs overlap (FRLG 1-623 vs Emerald 1-854). `src/data.c` L231-235 includes `trainers_frlg.h` **or** `trainers.h`. Trainer flags start at 0x500 in both builds. | FRLG IDs alias Hoenn trainers | Append FRLG trainers after Emerald (≈1,479 total). That exceeds `MAX_TRAINERS_COUNT_EMERALD` 864 and the trainer-flag block, so trainer flags must move or grow. |
| Trainer classes/pics | `src/battle_main.c` `gTrainerClasses` `*_FRLG` entries (50), `TRAINER_PIC_*_FRLG` | Present | — |
| Wild encounters | `tools/wild_encounters/wild_encounters_to_header.py` wraps `_FireRed` / `_LeafGreen` tables in `#ifdef FIRERED/LEAFGREEN` | Dropped | Keep one version (FireRed suggested) or rebalance. |
| Music | All 36 tracks used by Kanto maps (`MUS_RG_*`) exist in `include/constants/songs.h` | Present | — |
| Heal locations | `src/data/heal_locations.json`: 21 Kanto/Sevii entries. **Note:** `mapjson groups` rewrites this file in place and zeroes the entries of dropped maps. | Zeroed | Restore after the region filter is lifted. |
| Region map | `src/region_map.c` includes `region_map_layout_kanto.h` and the Sevii layouts in both builds; `MAPSEC_*` Kanto entries exist in `region_map_sections.json` | Present | — |
| Field-move badges | `src/field_move.c`: Cut / Flash / Rock Smash / Fly / Waterfall badge index swaps by `IS_FRLG` | Hoenn order | Decide one rule for 16 badges. |
| Misc. runtime `IS_FRLG` | Safari step counter 600 vs 500 (`safari_zone.c`), white-out script (`field_screen_effect.c`), Mach/Acro → single bike (`field_player_avatar.c`), `MUS_RG_SURF`/`MUS_RG_CYCLING` (`overworld.c`, `bike.c`), PC tiles (`field_specials.c`), credits (`credits.c` vs `credits_frlg.c`), Trainer Tower (`trainer_tower.c`, `FREE_TRAINER_TOWER`) | Hoenn behaviour | Make these per-region where it matters (surf/bike music, Safari steps). |

Other cross-links to fix if parts are skipped:

* 38 warps in 19 Kanto Pokémon Center 2F maps point to `MAP_TRADE_CENTER_FRLG` / `MAP_UNION_ROOM_FRLG` / `MAP_RECORD_CORNER_FRLG`. Retarget them to the Emerald link rooms if the FRLG link rooms are skipped.
* The Seagallop ferry (`data/scripts/seagallop.inc`, `src/seagallop.c`) is the only link between Vermilion/Cinnabar and the Sevii Islands.

---

## 3. Map inventory by area (Kanto mainland in game order, then Sevii, then link rooms)

Status per area: **ESSENTIAL** = needed for a full Kanto act (8 gyms + League + the story dungeons). **KEY OPTIONAL** = a famous dungeon or legendary off the critical path. **OPTIONAL** = shortcut only. **SKIP** = recommended to leave out.

#### Pallet Town — **ESSENTIAL**

Map sections: `MAPSEC_PALLET_TOWN`. Maps: 5. Distinct trainers: 3. Start town; Oak lab (starter, Pokédex, rival battle 1).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| PalletTown_Frlg | `MAP_PALLET_TOWN` | `LAYOUT_PALLET_TOWN` | town | 3 | 0 | 0 | 0 | 5 | 0 | 3 | 3 |
| PalletTown_PlayersHouse_1F_Frlg | `MAP_PALLET_TOWN_PLAYERS_HOUSE_1F` | `LAYOUT_PALLET_TOWN_PLAYERS_HOUSE_1F_FRLG` | interior | 1 | 0 | 0 | 0 | 1 | 0 | 4 | 0 |
| PalletTown_PlayersHouse_2F_Frlg | `MAP_PALLET_TOWN_PLAYERS_HOUSE_2F` | `LAYOUT_PALLET_TOWN_PLAYERS_HOUSE_2F_FRLG` | interior | 0 | 0 | 0 | 0 | 3 | 0 | 1 | 0 |
| PalletTown_ProfessorOaksLab_Frlg | `MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB` | `LAYOUT_PALLET_TOWN_PROFESSOR_OAKS_LAB` | interior | 7 | 0 | 3 | 3 | 4 | 0 | 3 | 6 |
| PalletTown_RivalsHouse_Frlg | `MAP_PALLET_TOWN_RIVALS_HOUSE` | `LAYOUT_PALLET_TOWN_RIVALS_HOUSE` | interior | 2 | 0 | 0 | 0 | 3 | 0 | 3 | 0 |

Trainers — PalletTown_ProfessorOaksLab: RIVAL_OAKS_LAB_BULBASAUR, RIVAL_OAKS_LAB_CHARMANDER, RIVAL_OAKS_LAB_SQUIRTLE

Connections — PalletTown → up `MAP_ROUTE1`, down `MAP_ROUTE21_NORTH`

FRLG-only hooks — PalletTown: `DisableMsgBoxWalkaway`, `SetWalkingIntoSignVars` · PalletTown_ProfessorOaksLab: `trainerbattle_earlyrival` · PalletTown_RivalsHouse: `DaisyMassageServices`, `GetLeadMonFriendship`

#### Route 1 — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_1`. Maps: 1. Distinct trainers: 0.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route1_Frlg | `MAP_ROUTE1` | `LAYOUT_ROUTE1` | route | 2 | 0 | 0 | 0 | 1 | 0 | 0 | 0 |

Connections — Route1 → up `MAP_VIRIDIAN_CITY`, down `MAP_PALLET_TOWN`

#### Viridian City (+ Gym 8) — **ESSENTIAL**

Map sections: `MAPSEC_VIRIDIAN_CITY`. Maps: 7. Distinct trainers: 9. Oak's Parcel at Mart; Old Man catching tutorial (StartOldManTutorialBattle); Gym locked until badges 2-7.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| ViridianCity_Frlg | `MAP_VIRIDIAN_CITY` | `LAYOUT_VIRIDIAN_CITY` | town | 6 | 2 | 1 | 0 | 5 | 0 | 5 | 4 |
| ViridianCity_Gym_Frlg | `MAP_VIRIDIAN_CITY_GYM` | `LAYOUT_VIRIDIAN_CITY_GYM` | gym | 10 | 0 | 0 | 9 | 2 | 1 | 3 | 0 |
| ViridianCity_House_Frlg | `MAP_VIRIDIAN_CITY_HOUSE` | `LAYOUT_VIRIDIAN_CITY_HOUSE` | interior | 3 | 0 | 0 | 0 | 1 | 0 | 3 | 0 |
| ViridianCity_Mart_Frlg | `MAP_VIRIDIAN_CITY_MART` | `LAYOUT_MART_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| ViridianCity_PokemonCenter_1F_Frlg | `MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| ViridianCity_PokemonCenter_2F_Frlg | `MAP_VIRIDIAN_CITY_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| ViridianCity_School_Frlg | `MAP_VIRIDIAN_CITY_SCHOOL` | `LAYOUT_VIRIDIAN_CITY_SCHOOL` | interior | 2 | 0 | 0 | 0 | 5 | 0 | 3 | 0 |

Trainers — ViridianCity_Gym: BLACK_BELT_ATSUSHI, BLACK_BELT_KIYO, BLACK_BELT_TAKASHI, COOLSAMUEL, COOLWARREN, COOLYUJI, LEADER_GIOVANNI, TAMER_COLE, TAMER_JASON

Connections — ViridianCity → up `MAP_ROUTE2`, down `MAP_ROUTE1`, left `MAP_ROUTE22`

FRLG-only hooks — ViridianCity: `StartOldManTutorialBattle`

#### Route 22 (+ League gate) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_22`. Maps: 2. Distinct trainers: 6. Early rival battle (trainerbattle_earlyrival), late rival battle; Route22_NorthEntrance = badge gate to Route 23.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route22_Frlg | `MAP_ROUTE22` | `LAYOUT_ROUTE22` | route | 1 | 0 | 0 | 6 | 1 | 0 | 2 | 6 |
| Route22_NorthEntrance_Frlg | `MAP_ROUTE22_NORTH_ENTRANCE` | `LAYOUT_ROUTE22_NORTH_ENTRANCE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 1 |

Trainers — Route22: RIVAL_ROUTE22_EARLY_BULBASAUR, RIVAL_ROUTE22_EARLY_CHARMANDER, RIVAL_ROUTE22_EARLY_SQUIRTLE, RIVAL_ROUTE22_LATE_BULBASAUR, RIVAL_ROUTE22_LATE_CHARMANDER, RIVAL_ROUTE22_LATE_SQUIRTLE

Connections — Route22 → up `MAP_ROUTE23`, right `MAP_VIRIDIAN_CITY`

FRLG-only hooks — Route22: `trainerbattle_earlyrival`

#### Route 2 (+ gatehouses, aide) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_2`. Maps: 5. Distinct trainers: 0. Route2_EastBuilding aide gives HM05 Flash (10 dex entries).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route2_Frlg | `MAP_ROUTE2` | `LAYOUT_ROUTE2` | route | 0 | 5 | 2 | 0 | 2 | 0 | 10 | 0 |
| Route2_EastBuilding_Frlg | `MAP_ROUTE2_EAST_BUILDING` | `LAYOUT_ROUTE2_ENTRANCE` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| Route2_House_Frlg | `MAP_ROUTE2_HOUSE` | `LAYOUT_HOUSE2_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| Route2_ViridianForest_NorthEntrance_Frlg | `MAP_ROUTE2_VIRIDIAN_FOREST_NORTH_ENTRANCE` | `LAYOUT_ROUTE2_ENTRANCE` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| Route2_ViridianForest_SouthEntrance_Frlg | `MAP_ROUTE2_VIRIDIAN_FOREST_SOUTH_ENTRANCE` | `LAYOUT_ROUTE2_ENTRANCE` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |

Connections — Route2 → up `MAP_PEWTER_CITY`, down `MAP_VIRIDIAN_CITY`

#### Viridian Forest — **ESSENTIAL**

Map sections: `MAPSEC_VIRIDIAN_FOREST`. Maps: 1. Distinct trainers: 5.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| ViridianForest_Frlg | `MAP_VIRIDIAN_FOREST` | `LAYOUT_VIRIDIAN_FOREST` | route | 7 | 0 | 4 | 5 | 6 | 2 | 6 | 0 |

Trainers — ViridianForest: BUG_CATCHER_ANTHONY, BUG_CATCHER_CHARLIE, BUG_CATCHER_DOUG, BUG_CATCHER_RICK, BUG_CATCHER_SAMMY

#### Pewter City (+ Gym 1) — **ESSENTIAL**

Map sections: `MAPSEC_PEWTER_CITY`. Maps: 9. Distinct trainers: 2. Museum (fossil picture specials, Old Amber); forced walk to Gym.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| PewterCity_Frlg | `MAP_PEWTER_CITY` | `LAYOUT_PEWTER_CITY` | town | 6 | 1 | 0 | 0 | 5 | 1 | 7 | 7 |
| PewterCity_Gym_Frlg | `MAP_PEWTER_CITY_GYM` | `LAYOUT_PEWTER_CITY_GYM` | gym | 3 | 0 | 0 | 2 | 2 | 0 | 3 | 0 |
| PewterCity_House1_Frlg | `MAP_PEWTER_CITY_HOUSE1` | `LAYOUT_HOUSE2_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| PewterCity_House2_Frlg | `MAP_PEWTER_CITY_HOUSE2` | `LAYOUT_HOUSE2_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| PewterCity_Mart_Frlg | `MAP_PEWTER_CITY_MART` | `LAYOUT_MART_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| PewterCity_Museum_1F_Frlg | `MAP_PEWTER_CITY_MUSEUM_1F` | `LAYOUT_PEWTER_CITY_MUSEUM_1F` | interior | 6 | 0 | 0 | 0 | 4 | 0 | 6 | 3 |
| PewterCity_Museum_2F_Frlg | `MAP_PEWTER_CITY_MUSEUM_2F` | `LAYOUT_PEWTER_CITY_MUSEUM_2F` | interior | 5 | 0 | 0 | 0 | 8 | 0 | 1 | 0 |
| PewterCity_PokemonCenter_1F_Frlg | `MAP_PEWTER_CITY_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 7 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| PewterCity_PokemonCenter_2F_Frlg | `MAP_PEWTER_CITY_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |

Trainers — PewterCity_Gym: CAMPER_LIAM, LEADER_BROCK

Connections — PewterCity → down `MAP_ROUTE2`, right `MAP_ROUTE3`

FRLG-only hooks — PewterCity: `DisableMsgBoxWalkaway` · PewterCity_Museum_1F: `CloseMuseumFossilPic`, `OpenMuseumFossilPic`

#### Route 3 — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_3`. Maps: 1. Distinct trainers: 8.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route3_Frlg | `MAP_ROUTE3` | `LAYOUT_ROUTE3` | route | 9 | 0 | 0 | 8 | 1 | 1 | 0 | 0 |

Trainers — Route3: BUG_CATCHER_COLTON, BUG_CATCHER_GREG, BUG_CATCHER_JAMES, LASS_JANICE, LASS_ROBIN, LASS_SALLY, YOUNGSTER_BEN, YOUNGSTER_CALVIN

Connections — Route3 → up `MAP_ROUTE4`, left `MAP_PEWTER_CITY`

#### Mt. Moon — **ESSENTIAL**

Map sections: `MAPSEC_MT_MOON`. Maps: 3. Distinct trainers: 12. Rocket grunts + Super Nerd fossil choice (Dome/Helix).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| MtMoon_1F_Frlg | `MAP_MT_MOON_1F` | `LAYOUT_MT_MOON_1F` | dungeon | 8 | 0 | 6 | 7 | 1 | 0 | 4 | 0 |
| MtMoon_B1F_Frlg | `MAP_MT_MOON_B1F` | `LAYOUT_MT_MOON_B1F` | dungeon | 0 | 0 | 0 | 0 | 0 | 6 | 8 | 0 |
| MtMoon_B2F_Frlg | `MAP_MT_MOON_B2F` | `LAYOUT_MT_MOON_B2F` | dungeon | 7 | 0 | 4 | 5 | 0 | 2 | 4 | 1 |

Trainers — MtMoon_1F: BUG_CATCHER_KENT, BUG_CATCHER_ROBBY, HIKER_MARCOS, LASS_IRIS, LASS_MIRIAM, SUPER_NERD_JOVAN, YOUNGSTER_JOSH · MtMoon_B2F: SUPER_NERD_MIGUEL, TEAM_ROCKET_GRUNT, TEAM_ROCKET_GRUNT_2, TEAM_ROCKET_GRUNT_3, TEAM_ROCKET_GRUNT_4

#### Route 4 (+ Pokémon Center) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_4`. Maps: 3. Distinct trainers: 1.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route4_Frlg | `MAP_ROUTE4` | `LAYOUT_ROUTE4` | route | 6 | 0 | 1 | 1 | 2 | 3 | 3 | 0 |
| Route4_PokemonCenter_1F_Frlg | `MAP_ROUTE4_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 6 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| Route4_PokemonCenter_2F_Frlg | `MAP_ROUTE4_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |

Trainers — Route4: LASS_CRISSY

Connections — Route4 → down `MAP_ROUTE3`, right `MAP_CERULEAN_CITY`

#### Cerulean City (+ Gym 2) — **ESSENTIAL**

Map sections: `MAPSEC_CERULEAN_CITY`. Maps: 11. Distinct trainers: 7. Rival battle; Rocket thief (TM28); Bike Shop (voucher -> Bicycle); Fame Checker given here.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| CeruleanCity_Frlg | `MAP_CERULEAN_CITY` | `LAYOUT_CERULEAN_CITY` | town | 10 | 2 | 0 | 4 | 7 | 1 | 14 | 5 |
| CeruleanCity_Gym_Frlg | `MAP_CERULEAN_CITY_GYM` | `LAYOUT_CERULEAN_CITY_GYM` | gym | 4 | 0 | 0 | 3 | 2 | 0 | 3 | 0 |
| CeruleanCity_BikeShop_Frlg | `MAP_CERULEAN_CITY_BIKE_SHOP` | `LAYOUT_CERULEAN_CITY_BIKE_SHOP` | interior | 3 | 0 | 0 | 0 | 8 | 0 | 3 | 0 |
| CeruleanCity_House1_Frlg | `MAP_CERULEAN_CITY_HOUSE1` | `LAYOUT_CERULEAN_CITY_HOUSE1` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| CeruleanCity_House2_Frlg | `MAP_CERULEAN_CITY_HOUSE2` | `LAYOUT_CERULEAN_CITY_HOUSE2` | interior | 2 | 0 | 0 | 0 | 1 | 0 | 4 | 0 |
| CeruleanCity_House3_Frlg | `MAP_CERULEAN_CITY_HOUSE3` | `LAYOUT_HOUSE1_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| CeruleanCity_House4_Frlg | `MAP_CERULEAN_CITY_HOUSE4` | `LAYOUT_HOUSE1_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| CeruleanCity_House5_Frlg | `MAP_CERULEAN_CITY_HOUSE5` | `LAYOUT_CERULEAN_CITY_HOUSE5` | interior | 1 | 0 | 0 | 0 | 1 | 0 | 1 | 0 |
| CeruleanCity_Mart_Frlg | `MAP_CERULEAN_CITY_MART` | `LAYOUT_MART_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| CeruleanCity_PokemonCenter_1F_Frlg | `MAP_CERULEAN_CITY_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 7 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| CeruleanCity_PokemonCenter_2F_Frlg | `MAP_CERULEAN_CITY_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |

Trainers — CeruleanCity: RIVAL_CERULEAN_BULBASAUR, RIVAL_CERULEAN_CHARMANDER, RIVAL_CERULEAN_SQUIRTLE, TEAM_ROCKET_GRUNT_5 · CeruleanCity_Gym: LEADER_MISTY, PICNICKER_DIANA, SWIMMER_MALE_LUIS

Connections — CeruleanCity → up `MAP_ROUTE24`, down `MAP_ROUTE5`, left `MAP_ROUTE4`, right `MAP_ROUTE9`

FRLG-only hooks — CeruleanCity_House4: `WonderNews_GetRewardInfo`

#### Route 24 (Nugget Bridge) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_24`. Maps: 1. Distinct trainers: 7. 5-trainer bridge + Rocket recruiter.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route24_Frlg | `MAP_ROUTE24` | `LAYOUT_ROUTE24` | route | 7 | 0 | 1 | 7 | 0 | 1 | 0 | 2 |

Trainers — Route24: BUG_CATCHER_CALE, CAMPER_ETHAN, CAMPER_SHANE, LASS_ALI, LASS_RELI, TEAM_ROCKET_GRUNT_6, YOUNGSTER_TIMMY

Connections — Route24 → down `MAP_CERULEAN_CITY`, right `MAP_ROUTE25`

#### Route 25 (+ Bill's Sea Cottage) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_25`. Maps: 2. Distinct trainers: 9. Bill teleporter cutscene (AnimateTeleporterHousing/Cable) -> S.S. Ticket.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route25_Frlg | `MAP_ROUTE25` | `LAYOUT_ROUTE25` | route | 11 | 1 | 1 | 9 | 1 | 4 | 1 | 0 |
| Route25_SeaCottage_Frlg | `MAP_ROUTE25_SEA_COTTAGE` | `LAYOUT_ROUTE25_SEA_COTTAGE` | interior | 2 | 0 | 0 | 0 | 1 | 0 | 3 | 0 |

Trainers — Route25: CAMPER_FLINT, HIKER_FRANKLIN, HIKER_NOB, HIKER_WAYNE, LASS_HALEY, PICNICKER_KELSEY, YOUNGSTER_CHAD, YOUNGSTER_DAN, YOUNGSTER_JOEY

Connections — Route25 → left `MAP_ROUTE24`

FRLG-only hooks — Route25_SeaCottage: `AnimateTeleporterCable`, `AnimateTeleporterHousing`, `SetSeenMon`

#### Route 5 (+ Day Care, Saffron gate) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_5`. Maps: 3. Distinct trainers: 0. Route 5 Day Care uses FRLG-only daycare specials; gate needs Tea.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route5_Frlg | `MAP_ROUTE5` | `LAYOUT_ROUTE5` | route | 0 | 0 | 0 | 0 | 1 | 0 | 4 | 0 |
| Route5_PokemonDayCare_Frlg | `MAP_ROUTE5_POKEMON_DAY_CARE` | `LAYOUT_ROUTE5_POKEMON_DAY_CARE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| Route5_SouthEntrance_Frlg | `MAP_ROUTE5_SOUTH_ENTRANCE` | `LAYOUT_SAFFRON_CITY_NORTH_SOUTH_ENTRANCE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 3 |

Connections — Route5 → up `MAP_CERULEAN_CITY`, down `MAP_SAFFRON_CITY_CONNECTION`

#### Underground Path (N-S and E-W) — **ESSENTIAL (N-S) / OPTIONAL (E-W)**

Map sections: `MAPSEC_UNDERGROUND_PATH`, `MAPSEC_UNDERGROUND_PATH_2`. Maps: 6. Distinct trainers: 0. N-S tunnel links Route 5-6 while Saffron is closed; E-W links Route 7-8 (shortcut).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| UndergroundPath_EastWestTunnel_Frlg | `MAP_UNDERGROUND_PATH_EAST_WEST_TUNNEL` | `LAYOUT_UNDERGROUND_PATH_EAST_WEST_TUNNEL` | dungeon | 0 | 0 | 0 | 0 | 0 | 7 | 2 | 0 |
| UndergroundPath_NorthSouthTunnel_Frlg | `MAP_UNDERGROUND_PATH_NORTH_SOUTH_TUNNEL` | `LAYOUT_UNDERGROUND_PATH_NORTH_SOUTH_TUNNEL` | dungeon | 0 | 0 | 0 | 0 | 0 | 7 | 2 | 0 |
| UndergroundPath_EastEntrance_Frlg | `MAP_UNDERGROUND_PATH_EAST_ENTRANCE` | `LAYOUT_UNDERGROUND_PATH_ENTRANCE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| UndergroundPath_NorthEntrance_Frlg | `MAP_UNDERGROUND_PATH_NORTH_ENTRANCE` | `LAYOUT_UNDERGROUND_PATH_ENTRANCE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| UndergroundPath_SouthEntrance_Frlg | `MAP_UNDERGROUND_PATH_SOUTH_ENTRANCE` | `LAYOUT_UNDERGROUND_PATH_ENTRANCE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| UndergroundPath_WestEntrance_Frlg | `MAP_UNDERGROUND_PATH_WEST_ENTRANCE` | `LAYOUT_UNDERGROUND_PATH_ENTRANCE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |

#### Route 6 (+ Saffron gate) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_6`. Maps: 3. Distinct trainers: 6.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route6_Frlg | `MAP_ROUTE6` | `LAYOUT_ROUTE6` | route | 6 | 0 | 0 | 6 | 1 | 2 | 3 | 0 |
| Route6_NorthEntrance_Frlg | `MAP_ROUTE6_NORTH_ENTRANCE` | `LAYOUT_SAFFRON_CITY_NORTH_SOUTH_ENTRANCE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 3 |
| Route6_UnusedHouse_Frlg | `MAP_ROUTE6_UNUSED_HOUSE` | `LAYOUT_HOUSE2_FRLG` | interior | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

Trainers — Route6: BUG_CATCHER_ELIJAH, BUG_CATCHER_KEIGO, CAMPER_JEFF, CAMPER_RICKY, PICNICKER_ISABELLE, PICNICKER_NANCY

Connections — Route6 → up `MAP_SAFFRON_CITY_CONNECTION`, down `MAP_VERMILION_CITY`

#### Vermilion City (+ Gym 3) — **ESSENTIAL**

Map sections: `MAPSEC_VERMILION_CITY`. Maps: 9. Distinct trainers: 4. Cut tree in front of Gym; Fan Club (Bike Voucher); VS Seeker at Pokémon Center.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| VermilionCity_Frlg | `MAP_VERMILION_CITY` | `LAYOUT_VERMILION_CITY` | town | 7 | 1 | 0 | 0 | 5 | 1 | 10 | 4 |
| VermilionCity_Gym_Frlg | `MAP_VERMILION_CITY_GYM` | `LAYOUT_VERMILION_CITY_GYM` | gym | 5 | 0 | 0 | 4 | 17 | 0 | 3 | 0 |
| VermilionCity_House1_Frlg | `MAP_VERMILION_CITY_HOUSE1` | `LAYOUT_HOUSE1_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| VermilionCity_House2_Frlg | `MAP_VERMILION_CITY_HOUSE2` | `LAYOUT_HOUSE1_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| VermilionCity_House3_Frlg | `MAP_VERMILION_CITY_HOUSE3` | `LAYOUT_HOUSE1_FRLG` | interior | 4 | 0 | 0 | 0 | 1 | 0 | 3 | 0 |
| VermilionCity_Mart_Frlg | `MAP_VERMILION_CITY_MART` | `LAYOUT_MART_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| VermilionCity_PokemonCenter_1F_Frlg | `MAP_VERMILION_CITY_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 7 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| VermilionCity_PokemonCenter_2F_Frlg | `MAP_VERMILION_CITY_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| VermilionCity_PokemonFanClub_Frlg | `MAP_VERMILION_CITY_POKEMON_FAN_CLUB` | `LAYOUT_VERMILION_CITY_POKEMON_FAN_CLUB` | interior | 6 | 0 | 0 | 0 | 2 | 0 | 3 | 0 |

Trainers — VermilionCity_Gym: ENGINEER_BAILY, GENTLEMAN_TUCKER, LEADER_LT_SURGE, SAILOR_DWAYNE

Connections — VermilionCity → up `MAP_ROUTE6`, right `MAP_ROUTE11`

FRLG-only hooks — VermilionCity_Gym: `SetVermilionTrashCans`

#### S.S. Anne — **ESSENTIAL**

Map sections: `MAPSEC_S_S_ANNE`. Maps: 26. Distinct trainers: 19. Rival battle; Captain gives HM01 Cut; departure cutscene (DoSSAnneDepartureCutscene).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| SSAnne_1F_Corridor_Frlg | `MAP_SSANNE_1F_CORRIDOR` | `LAYOUT_SSANNE_1F_CORRIDOR` | dungeon | 2 | 0 | 0 | 0 | 0 | 0 | 13 | 0 |
| SSAnne_1F_Room1_Frlg | `MAP_SSANNE_1F_ROOM1` | `LAYOUT_SSANNE_ROOM1` | dungeon | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SSAnne_1F_Room2_Frlg | `MAP_SSANNE_1F_ROOM2` | `LAYOUT_SSANNE_ROOM1` | dungeon | 3 | 0 | 1 | 2 | 0 | 0 | 1 | 0 |
| SSAnne_1F_Room3_Frlg | `MAP_SSANNE_1F_ROOM3` | `LAYOUT_SSANNE_ROOM1` | dungeon | 3 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SSAnne_1F_Room4_Frlg | `MAP_SSANNE_1F_ROOM4` | `LAYOUT_SSANNE_ROOM1` | dungeon | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SSAnne_1F_Room5_Frlg | `MAP_SSANNE_1F_ROOM5` | `LAYOUT_SSANNE_ROOM1` | dungeon | 1 | 0 | 0 | 1 | 0 | 0 | 1 | 0 |
| SSAnne_1F_Room6_Frlg | `MAP_SSANNE_1F_ROOM6` | `LAYOUT_SSANNE_ROOM1` | dungeon | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SSAnne_1F_Room7_Frlg | `MAP_SSANNE_1F_ROOM7` | `LAYOUT_SSANNE_ROOM1` | dungeon | 1 | 0 | 0 | 1 | 0 | 0 | 1 | 0 |
| SSAnne_2F_Corridor_Frlg | `MAP_SSANNE_2F_CORRIDOR` | `LAYOUT_SSANNE_2F_CORRIDOR` | dungeon | 2 | 0 | 0 | 3 | 0 | 0 | 9 | 3 |
| SSAnne_2F_Room1_Frlg | `MAP_SSANNE_2F_ROOM1` | `LAYOUT_SSANNE_ROOM2` | dungeon | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SSAnne_2F_Room2_Frlg | `MAP_SSANNE_2F_ROOM2` | `LAYOUT_SSANNE_ROOM2` | dungeon | 2 | 0 | 1 | 2 | 0 | 0 | 1 | 0 |
| SSAnne_2F_Room3_Frlg | `MAP_SSANNE_2F_ROOM3` | `LAYOUT_SSANNE_ROOM2` | dungeon | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SSAnne_2F_Room4_Frlg | `MAP_SSANNE_2F_ROOM4` | `LAYOUT_SSANNE_ROOM2` | dungeon | 2 | 0 | 1 | 2 | 0 | 0 | 1 | 0 |
| SSAnne_2F_Room5_Frlg | `MAP_SSANNE_2F_ROOM5` | `LAYOUT_SSANNE_ROOM2` | dungeon | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SSAnne_2F_Room6_Frlg | `MAP_SSANNE_2F_ROOM6` | `LAYOUT_SSANNE_ROOM2` | dungeon | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SSAnne_3F_Corridor_Frlg | `MAP_SSANNE_3F_CORRIDOR` | `LAYOUT_SSANNE_3F_CORRIDOR` | dungeon | 1 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| SSAnne_B1F_Corridor_Frlg | `MAP_SSANNE_B1F_CORRIDOR` | `LAYOUT_SSANNE_B1F_CORRIDOR` | dungeon | 0 | 0 | 0 | 0 | 0 | 1 | 6 | 0 |
| SSAnne_B1F_Room1_Frlg | `MAP_SSANNE_B1F_ROOM1` | `LAYOUT_SSANNE_ROOM2` | dungeon | 2 | 0 | 0 | 2 | 0 | 0 | 1 | 0 |
| SSAnne_B1F_Room2_Frlg | `MAP_SSANNE_B1F_ROOM2` | `LAYOUT_SSANNE_ROOM2` | dungeon | 1 | 0 | 1 | 1 | 0 | 0 | 1 | 0 |
| SSAnne_B1F_Room3_Frlg | `MAP_SSANNE_B1F_ROOM3` | `LAYOUT_SSANNE_ROOM2` | dungeon | 1 | 0 | 1 | 1 | 0 | 0 | 1 | 0 |
| SSAnne_B1F_Room4_Frlg | `MAP_SSANNE_B1F_ROOM4` | `LAYOUT_SSANNE_ROOM2` | dungeon | 2 | 0 | 0 | 2 | 0 | 0 | 1 | 0 |
| SSAnne_B1F_Room5_Frlg | `MAP_SSANNE_B1F_ROOM5` | `LAYOUT_SSANNE_ROOM2` | dungeon | 2 | 0 | 1 | 0 | 0 | 0 | 1 | 0 |
| SSAnne_CaptainsOffice_Frlg | `MAP_SSANNE_CAPTAINS_OFFICE` | `LAYOUT_SSANNE_CAPTAINS_OFFICE` | dungeon | 1 | 0 | 0 | 0 | 3 | 0 | 1 | 0 |
| SSAnne_Deck_Frlg | `MAP_SSANNE_DECK` | `LAYOUT_SSANNE_DECK` | dungeon | 5 | 0 | 0 | 2 | 0 | 0 | 2 | 0 |
| SSAnne_Exterior_Frlg | `MAP_SSANNE_EXTERIOR` | `LAYOUT_SSANNE_EXTERIOR` | dungeon | 1 | 0 | 0 | 0 | 0 | 1 | 5 | 0 |
| SSAnne_Kitchen_Frlg | `MAP_SSANNE_KITCHEN` | `LAYOUT_SSANNE_KITCHEN` | dungeon | 7 | 0 | 1 | 0 | 0 | 3 | 1 | 0 |

Trainers — SSAnne_1F_Room2: LASS_ANN, YOUNGSTER_TYLER · SSAnne_1F_Room5: GENTLEMAN_ARTHUR · SSAnne_1F_Room7: GENTLEMAN_THOMAS · SSAnne_2F_Corridor: RIVAL_SS_ANNE_BULBASAUR, RIVAL_SS_ANNE_CHARMANDER, RIVAL_SS_ANNE_SQUIRTLE · SSAnne_2F_Room2: FISHERMAN_DALE, GENTLEMAN_BROOKS · SSAnne_2F_Room4: GENTLEMAN_LAMAR, LASS_DAWN · SSAnne_B1F_Room1: FISHERMAN_BARNY, SAILOR_PHILLIP · SSAnne_B1F_Room2: SAILOR_HUEY · SSAnne_B1F_Room3: SAILOR_DYLAN · SSAnne_B1F_Room4: SAILOR_DUNCAN, SAILOR_LEONARD · SSAnne_Deck: SAILOR_EDMOND, SAILOR_TREVOR

FRLG-only hooks — SSAnne_2F_Room1: `SetSeenMon` · SSAnne_Exterior: `DoSSAnneDepartureCutscene`

#### Route 11 (+ gate with Itemfinder aide) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_11`. Maps: 3. Distinct trainers: 10. Connects Vermilion to Route 12 (Snorlax side).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route11_Frlg | `MAP_ROUTE11` | `LAYOUT_ROUTE11` | route | 10 | 0 | 3 | 10 | 1 | 1 | 3 | 0 |
| Route11_EastEntrance_1F_Frlg | `MAP_ROUTE11_EAST_ENTRANCE_1F` | `LAYOUT_ENTRANCE_1F` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 5 | 0 |
| Route11_EastEntrance_2F_Frlg | `MAP_ROUTE11_EAST_ENTRANCE_2F` | `LAYOUT_ENTRANCE_2F` | interior | 2 | 0 | 0 | 0 | 2 | 0 | 1 | 0 |

Trainers — Route11: ENGINEER_BERNIE, ENGINEER_BRAXTON, GAMER_DARIAN, GAMER_DIRK, GAMER_HUGO, GAMER_JASPER, YOUNGSTER_DAVE, YOUNGSTER_DILLON, YOUNGSTER_EDDIE, YOUNGSTER_YASU

Connections — Route11 → left `MAP_VERMILION_CITY`, right `MAP_ROUTE12`

#### Diglett's Cave — **OPTIONAL**

Map sections: `MAPSEC_DIGLETTS_CAVE`. Maps: 3. Distinct trainers: 0. Shortcut Route 11 <-> Route 2 (needed only to reach Route 2 aide early).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| DiglettsCave_B1F_Frlg | `MAP_DIGLETTS_CAVE_B1F` | `LAYOUT_DIGLETTS_CAVE_B1F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| DiglettsCave_NorthEntrance_Frlg | `MAP_DIGLETTS_CAVE_NORTH_ENTRANCE` | `LAYOUT_DIGLETTS_CAVE_NORTH_ENTRANCE` | dungeon | 1 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| DiglettsCave_SouthEntrance_Frlg | `MAP_DIGLETTS_CAVE_SOUTH_ENTRANCE` | `LAYOUT_DIGLETTS_CAVE_SOUTH_ENTRANCE` | dungeon | 1 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |

#### Route 9 — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_9`. Maps: 1. Distinct trainers: 9.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route9_Frlg | `MAP_ROUTE9` | `LAYOUT_ROUTE9` | route | 9 | 1 | 2 | 9 | 1 | 3 | 0 | 0 |

Trainers — Route9: BUG_CATCHER_BRENT, BUG_CATCHER_CONNER, CAMPER_CHRIS, CAMPER_DREW, HIKER_ALAN, HIKER_BRICE, HIKER_JEREMY, PICNICKER_ALICIA, PICNICKER_CAITLIN

Connections — Route9 → left `MAP_CERULEAN_CITY`, right `MAP_ROUTE10`

#### Route 10 (+ Pokémon Center) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_10`. Maps: 3. Distinct trainers: 6.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route10_Frlg | `MAP_ROUTE10` | `LAYOUT_ROUTE10` | route | 6 | 4 | 0 | 6 | 3 | 5 | 5 | 0 |
| Route10_PokemonCenter_1F_Frlg | `MAP_ROUTE10_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| Route10_PokemonCenter_2F_Frlg | `MAP_ROUTE10_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |

Trainers — Route10: HIKER_CLARK, HIKER_TRENT, PICNICKER_CAROL, PICNICKER_HEIDI, POKEMANIAC_HERMAN, POKEMANIAC_MARK

Connections — Route10 → down `MAP_LAVENDER_TOWN`, left `MAP_ROUTE9`

#### Rock Tunnel — **ESSENTIAL**

Map sections: `MAPSEC_ROCK_TUNNEL`. Maps: 2. Distinct trainers: 15. requires_flash dark cave.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| RockTunnel_1F_Frlg | `MAP_ROCK_TUNNEL_1F` | `LAYOUT_ROCK_TUNNEL_1F` | dungeon | 7 | 0 | 3 | 7 | 1 | 0 | 6 | 0 |
| RockTunnel_B1F_Frlg | `MAP_ROCK_TUNNEL_B1F` | `LAYOUT_ROCK_TUNNEL_B1F` | dungeon | 9 | 15 | 2 | 8 | 0 | 0 | 4 | 0 |

Trainers — RockTunnel_1F: HIKER_LENNY, HIKER_LUCAS, HIKER_OLIVER, PICNICKER_ARIANA, PICNICKER_DANA, PICNICKER_LEAH, POKEMANIAC_ASHTON · RockTunnel_B1F: HIKER_ALLEN, HIKER_DUDLEY, HIKER_ERIC, PICNICKER_MARTHA, PICNICKER_SOFIA, POKEMANIAC_COOPER, POKEMANIAC_STEVE, POKEMANIAC_WINSTON

#### Power Plant — **KEY OPTIONAL**

Map sections: `MAPSEC_POWER_PLANT`. Maps: 1. Distinct trainers: 0. Zapdos static encounter + Voltorb/Electrode item traps.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| PowerPlant_Frlg | `MAP_POWER_PLANT` | `LAYOUT_POWER_PLANT` | dungeon | 1 | 0 | 7 | 0 | 0 | 2 | 5 | 0 |

#### Lavender Town — **ESSENTIAL**

Map sections: `MAPSEC_LAVENDER_TOWN`. Maps: 7. Distinct trainers: 0. Mr. Fuji's Volunteer House gives Poké Flute.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| LavenderTown_Frlg | `MAP_LAVENDER_TOWN` | `LAYOUT_LAVENDER_TOWN` | town | 3 | 0 | 0 | 0 | 4 | 0 | 6 | 0 |
| LavenderTown_House1_Frlg | `MAP_LAVENDER_TOWN_HOUSE1` | `LAYOUT_HOUSE5_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| LavenderTown_House2_Frlg | `MAP_LAVENDER_TOWN_HOUSE2` | `LAYOUT_HOUSE5_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| LavenderTown_Mart_Frlg | `MAP_LAVENDER_TOWN_MART` | `LAYOUT_MART_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| LavenderTown_PokemonCenter_1F_Frlg | `MAP_LAVENDER_TOWN_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| LavenderTown_PokemonCenter_2F_Frlg | `MAP_LAVENDER_TOWN_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| LavenderTown_VolunteerPokemonHouse_Frlg | `MAP_LAVENDER_TOWN_VOLUNTEER_POKEMON_HOUSE` | `LAYOUT_LAVENDER_TOWN_VOLUNTEER_POKEMON_HOUSE` | interior | 6 | 0 | 0 | 0 | 3 | 0 | 3 | 0 |

Connections — LavenderTown → up `MAP_ROUTE10`, down `MAP_ROUTE12`, left `MAP_ROUTE8`

#### Pokémon Tower — **ESSENTIAL**

Map sections: `MAPSEC_POKEMON_TOWER`. Maps: 7. Distinct trainers: 19. Rival battle 3F; ghost blockade needs Silph Scope; Marowak ghost (StartMarowakBattle); Mr. Fuji rescue 7F.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| PokemonTower_1F_Frlg | `MAP_POKEMON_TOWER_1F` | `LAYOUT_POKEMON_TOWER_1F` | dungeon | 5 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| PokemonTower_2F_Frlg | `MAP_POKEMON_TOWER_2F` | `LAYOUT_POKEMON_TOWER_2F` | dungeon | 2 | 0 | 0 | 3 | 0 | 0 | 2 | 2 |
| PokemonTower_3F_Frlg | `MAP_POKEMON_TOWER_3F` | `LAYOUT_POKEMON_TOWER_3F` | dungeon | 3 | 0 | 1 | 3 | 0 | 0 | 2 | 0 |
| PokemonTower_4F_Frlg | `MAP_POKEMON_TOWER_4F` | `LAYOUT_POKEMON_TOWER_4F` | dungeon | 3 | 0 | 3 | 3 | 0 | 0 | 2 | 0 |
| PokemonTower_5F_Frlg | `MAP_POKEMON_TOWER_5F` | `LAYOUT_POKEMON_TOWER_5F` | dungeon | 5 | 0 | 2 | 4 | 0 | 1 | 2 | 17 |
| PokemonTower_6F_Frlg | `MAP_POKEMON_TOWER_6F` | `LAYOUT_POKEMON_TOWER_6F` | dungeon | 3 | 0 | 2 | 3 | 0 | 0 | 2 | 2 |
| PokemonTower_7F_Frlg | `MAP_POKEMON_TOWER_7F` | `LAYOUT_POKEMON_TOWER_7F` | dungeon | 4 | 0 | 0 | 3 | 0 | 1 | 1 | 0 |

Trainers — PokemonTower_2F: RIVAL_POKEMON_TOWER_BULBASAUR, RIVAL_POKEMON_TOWER_CHARMANDER, RIVAL_POKEMON_TOWER_SQUIRTLE · PokemonTower_3F: CHANNELER_CARLY, CHANNELER_HOPE, CHANNELER_PATRICIA · PokemonTower_4F: CHANNELER_JODY, CHANNELER_LAUREL, CHANNELER_PAULA · PokemonTower_5F: CHANNELER_JANAE, CHANNELER_KARINA, CHANNELER_RUTH, CHANNELER_TAMMY · PokemonTower_6F: CHANNELER_ANGELICA, CHANNELER_EMILIA, CHANNELER_JENNIFER · PokemonTower_7F: TEAM_ROCKET_GRUNT_19, TEAM_ROCKET_GRUNT_20, TEAM_ROCKET_GRUNT_21

FRLG-only hooks — PokemonTower_6F: `StartMarowakBattle`

#### Route 8 (+ Saffron gate) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_8`. Maps: 2. Distinct trainers: 12.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route8_Frlg | `MAP_ROUTE8` | `LAYOUT_ROUTE8` | route | 13 | 2 | 0 | 12 | 1 | 3 | 2 | 0 |
| Route8_WestEntrance_Frlg | `MAP_ROUTE8_WEST_ENTRANCE` | `LAYOUT_SAFFRON_CITY_EAST_WEST_ENTRANCE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 3 |

Trainers — Route8: BIKER_JAREN, BIKER_RICARDO, GAMER_RICH, GAMER_STAN, LASS_ANDREA, LASS_JULIA, LASS_MEGAN, LASS_PAIGE, SUPER_NERD_AIDAN, SUPER_NERD_GLENN, SUPER_NERD_LESLIE, TWINS_ELI_ANNE

Connections — Route8 → left `MAP_SAFFRON_CITY_CONNECTION`, right `MAP_LAVENDER_TOWN`

#### Route 7 (+ Saffron gate) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_7`. Maps: 2. Distinct trainers: 0.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route7_Frlg | `MAP_ROUTE7` | `LAYOUT_ROUTE7` | route | 0 | 1 | 0 | 0 | 1 | 1 | 2 | 0 |
| Route7_EastEntrance_Frlg | `MAP_ROUTE7_EAST_ENTRANCE` | `LAYOUT_SAFFRON_CITY_EAST_WEST_ENTRANCE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 3 |

Connections — Route7 → left `MAP_CELADON_CITY`, right `MAP_SAFFRON_CITY_CONNECTION`

#### Celadon City (+ Gym 4) — **ESSENTIAL**

Map sections: `MAPSEC_CELADON_CITY`. Maps: 21. Distinct trainers: 9. Condominiums 1F gives Tea (opens all four Saffron gates); Game Corner hides Rocket Hideout entrance; Dept. Store + elevator specials.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| CeladonCity_Frlg | `MAP_CELADON_CITY` | `LAYOUT_CELADON_CITY` | town | 11 | 3 | 1 | 0 | 8 | 1 | 13 | 0 |
| CeladonCity_Gym_Frlg | `MAP_CELADON_CITY_GYM` | `LAYOUT_CELADON_CITY_GYM` | gym | 8 | 3 | 0 | 8 | 2 | 0 | 3 | 0 |
| CeladonCity_Condominiums_1F_Frlg | `MAP_CELADON_CITY_CONDOMINIUMS_1F` | `LAYOUT_CELADON_CITY_CONDOMINIUMS_1F` | interior | 4 | 0 | 0 | 0 | 2 | 0 | 6 | 0 |
| CeladonCity_Condominiums_2F_Frlg | `MAP_CELADON_CITY_CONDOMINIUMS_2F` | `LAYOUT_CELADON_CITY_CONDOMINIUMS_2F` | interior | 2 | 0 | 0 | 0 | 2 | 0 | 4 | 0 |
| CeladonCity_Condominiums_3F_Frlg | `MAP_CELADON_CITY_CONDOMINIUMS_3F` | `LAYOUT_CELADON_CITY_CONDOMINIUMS_3F` | interior | 4 | 0 | 0 | 0 | 8 | 0 | 4 | 0 |
| CeladonCity_Condominiums_RoofRoom_Frlg | `MAP_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM` | `LAYOUT_CELADON_CITY_CONDOMINIUMS_ROOF_ROOM` | interior | 1 | 0 | 1 | 0 | 3 | 0 | 3 | 0 |
| CeladonCity_Condominiums_Roof_Frlg | `MAP_CELADON_CITY_CONDOMINIUMS_ROOF` | `LAYOUT_CELADON_CITY_CONDOMINIUMS_ROOF` | interior | 0 | 0 | 0 | 0 | 2 | 0 | 3 | 0 |
| CeladonCity_DepartmentStore_1F_Frlg | `MAP_CELADON_CITY_DEPARTMENT_STORE_1F` | `LAYOUT_CELADON_CITY_DEPARTMENT_STORE_1F` | interior | 1 | 0 | 0 | 0 | 2 | 0 | 8 | 0 |
| CeladonCity_DepartmentStore_2F_Frlg | `MAP_CELADON_CITY_DEPARTMENT_STORE_2F` | `LAYOUT_CELADON_CITY_DEPARTMENT_STORE_2F` | interior | 4 | 0 | 0 | 0 | 1 | 0 | 3 | 0 |
| CeladonCity_DepartmentStore_3F_Frlg | `MAP_CELADON_CITY_DEPARTMENT_STORE_3F` | `LAYOUT_CELADON_CITY_DEPARTMENT_STORE_3F` | interior | 5 | 0 | 0 | 0 | 11 | 0 | 3 | 0 |
| CeladonCity_DepartmentStore_4F_Frlg | `MAP_CELADON_CITY_DEPARTMENT_STORE_4F` | `LAYOUT_CELADON_CITY_DEPARTMENT_STORE_4F` | interior | 3 | 0 | 0 | 0 | 1 | 0 | 3 | 0 |
| CeladonCity_DepartmentStore_5F_Frlg | `MAP_CELADON_CITY_DEPARTMENT_STORE_5F` | `LAYOUT_CELADON_CITY_DEPARTMENT_STORE_5F` | interior | 4 | 0 | 0 | 0 | 1 | 0 | 3 | 0 |
| CeladonCity_DepartmentStore_Elevator_Frlg | `MAP_CELADON_CITY_DEPARTMENT_STORE_ELEVATOR` | `LAYOUT_CELADON_CITY_DEPARTMENT_STORE_ELEVATOR` | interior | 0 | 0 | 0 | 0 | 2 | 0 | 2 | 0 |
| CeladonCity_DepartmentStore_Roof_Frlg | `MAP_CELADON_CITY_DEPARTMENT_STORE_ROOF` | `LAYOUT_CELADON_CITY_DEPARTMENT_STORE_ROOF` | interior | 2 | 0 | 0 | 0 | 4 | 0 | 1 | 0 |
| CeladonCity_GameCorner_Frlg | `MAP_CELADON_CITY_GAME_CORNER` | `LAYOUT_CELADON_CITY_GAME_CORNER` | interior | 11 | 0 | 0 | 1 | 24 | 12 | 4 | 0 |
| CeladonCity_GameCorner_PrizeRoom_Frlg | `MAP_CELADON_CITY_GAME_CORNER_PRIZE_ROOM` | `LAYOUT_CELADON_CITY_GAME_CORNER_PRIZE_ROOM` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| CeladonCity_Hotel_Frlg | `MAP_CELADON_CITY_HOTEL` | `LAYOUT_CELADON_CITY_HOTEL` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| CeladonCity_House1_Frlg | `MAP_CELADON_CITY_HOUSE1` | `LAYOUT_HOUSE5_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| CeladonCity_PokemonCenter_1F_Frlg | `MAP_CELADON_CITY_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| CeladonCity_PokemonCenter_2F_Frlg | `MAP_CELADON_CITY_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| CeladonCity_Restaurant_Frlg | `MAP_CELADON_CITY_RESTAURANT` | `LAYOUT_CELADON_CITY_RESTAURANT` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |

Trainers — CeladonCity_Gym: BEAUTY_BRIDGET, BEAUTY_LORI, BEAUTY_TAMIA, COOLMARY, LASS_KAY, LASS_LISA, LEADER_ERIKA, PICNICKER_TINA · CeladonCity_GameCorner: TEAM_ROCKET_GRUNT_7

Connections — CeladonCity → left `MAP_ROUTE16`, right `MAP_ROUTE7`

FRLG-only hooks — CeladonCity_Condominiums_3F: `HasAllKantoMons` · CeladonCity_DepartmentStore_Elevator: `AnimateElevator`, `CloseElevatorCurrentFloorWindow`, `DrawElevatorCurrentFloorWindow`, `InitElevatorFloorSelectMenuPos` · CeladonCity_GameCorner: `GetRandomSlotMachineId`

#### Rocket Hideout — **ESSENTIAL**

Map sections: `MAPSEC_ROCKET_HIDEOUT`. Maps: 5. Distinct trainers: 12. Spin-tile mazes B2F/B3F; Lift Key B4F; Boss Giovanni #1 -> Silph Scope.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| RocketHideout_B1F_Frlg | `MAP_ROCKET_HIDEOUT_B1F` | `LAYOUT_ROCKET_HIDEOUT_B1F` | dungeon | 5 | 0 | 2 | 5 | 0 | 1 | 6 | 0 |
| RocketHideout_B2F_Frlg | `MAP_ROCKET_HIDEOUT_B2F` | `LAYOUT_ROCKET_HIDEOUT_B2F` | dungeon | 1 | 0 | 4 | 1 | 0 | 0 | 5 | 0 |
| RocketHideout_B3F_Frlg | `MAP_ROCKET_HIDEOUT_B3F` | `LAYOUT_ROCKET_HIDEOUT_B3F` | dungeon | 2 | 0 | 3 | 2 | 0 | 1 | 2 | 0 |
| RocketHideout_B4F_Frlg | `MAP_ROCKET_HIDEOUT_B4F` | `LAYOUT_ROCKET_HIDEOUT_B4F` | dungeon | 4 | 0 | 5 | 4 | 0 | 2 | 3 | 0 |
| RocketHideout_Elevator_Frlg | `MAP_ROCKET_HIDEOUT_ELEVATOR` | `LAYOUT_ROCKET_HIDEOUT_ELEVATOR` | interior | 0 | 0 | 0 | 0 | 1 | 0 | 2 | 0 |

Trainers — RocketHideout_B1F: TEAM_ROCKET_GRUNT_10, TEAM_ROCKET_GRUNT_11, TEAM_ROCKET_GRUNT_12, TEAM_ROCKET_GRUNT_8, TEAM_ROCKET_GRUNT_9 · RocketHideout_B2F: TEAM_ROCKET_GRUNT_13 · RocketHideout_B3F: TEAM_ROCKET_GRUNT_14, TEAM_ROCKET_GRUNT_15 · RocketHideout_B4F: BOSS_GIOVANNI, TEAM_ROCKET_GRUNT_16, TEAM_ROCKET_GRUNT_17, TEAM_ROCKET_GRUNT_18

FRLG-only hooks — RocketHideout_B1F: `call_if_not_defeated` · RocketHideout_Elevator: `AnimateElevator`, `CloseElevatorCurrentFloorWindow`, `DrawElevatorCurrentFloorWindow`, `InitElevatorFloorSelectMenuPos`

#### Saffron City (+ Gym 6) — **ESSENTIAL**

Map sections: `MAPSEC_SAFFRON_CITY`. Maps: 12. Distinct trainers: 13. Rocket-occupied until Silph Co cleared (FLAG_HIDE_SAFFRON_ROCKETS); Fighting Dojo (gift Hitmonlee/Hitmonchan); Mr. Psychic (TM29).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| SaffronCity_Connection_Frlg | `MAP_SAFFRON_CITY_CONNECTION` | `LAYOUT_SAFFRON_CITY_CONNECTION` | town | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| SaffronCity_Frlg | `MAP_SAFFRON_CITY` | `LAYOUT_SAFFRON_CITY` | town | 15 | 0 | 0 | 0 | 9 | 0 | 15 | 0 |
| SaffronCity_Gym_Frlg | `MAP_SAFFRON_CITY_GYM` | `LAYOUT_SAFFRON_CITY_GYM` | gym | 9 | 0 | 0 | 8 | 2 | 0 | 33 | 0 |
| SaffronCity_CopycatsHouse_1F_Frlg | `MAP_SAFFRON_CITY_COPYCATS_HOUSE_1F` | `LAYOUT_SAFFRON_CITY_COPYCATS_HOUSE_1F` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| SaffronCity_CopycatsHouse_2F_Frlg | `MAP_SAFFRON_CITY_COPYCATS_HOUSE_2F` | `LAYOUT_SAFFRON_CITY_COPYCATS_HOUSE_2F` | interior | 4 | 0 | 0 | 0 | 2 | 1 | 1 | 0 |
| SaffronCity_Dojo_Frlg | `MAP_SAFFRON_CITY_DOJO` | `LAYOUT_SAFFRON_CITY_DOJO` | interior | 5 | 0 | 2 | 5 | 4 | 0 | 3 | 2 |
| SaffronCity_House_Frlg | `MAP_SAFFRON_CITY_HOUSE` | `LAYOUT_HOUSE5_FRLG` | interior | 4 | 0 | 0 | 0 | 1 | 0 | 3 | 0 |
| SaffronCity_Mart_Frlg | `MAP_SAFFRON_CITY_MART` | `LAYOUT_MART_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| SaffronCity_MrPsychicsHouse_Frlg | `MAP_SAFFRON_CITY_MR_PSYCHICS_HOUSE` | `LAYOUT_HOUSE5_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| SaffronCity_PokemonCenter_1F_Frlg | `MAP_SAFFRON_CITY_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 6 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| SaffronCity_PokemonCenter_2F_Frlg | `MAP_SAFFRON_CITY_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| SaffronCity_PokemonTrainerFanClub_Frlg | `MAP_SAFFRON_CITY_POKEMON_TRAINER_FAN_CLUB` | `LAYOUT_SAFFRON_CITY_POKEMON_TRAINER_FAN_CLUB` | interior | 10 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |

Trainers — SaffronCity_Gym: CHANNELER_AMANDA, CHANNELER_STACY, CHANNELER_TASHA, LEADER_SABRINA, PSYCHIC_CAMERON, PSYCHIC_JOHAN, PSYCHIC_PRESTON, PSYCHIC_TYRON · SaffronCity_Dojo: BLACK_BELT_AARON, BLACK_BELT_HIDEKI, BLACK_BELT_HITOSHI, BLACK_BELT_KOICHI, BLACK_BELT_MIKE

Connections — SaffronCity_Connection → up `MAP_ROUTE5`, down `MAP_ROUTE6`, left `MAP_ROUTE7`, right `MAP_ROUTE8` · SaffronCity → up `MAP_ROUTE5`, down `MAP_ROUTE6`, left `MAP_ROUTE7`, right `MAP_ROUTE8`

FRLG-only hooks — SaffronCity_PokemonTrainerFanClub: `Script_BufferFanClubTrainerName`, `Script_GetNumFansOfPlayerInTrainerFanClub`, `Script_IsFanClubMemberFanOfPlayer`, `Script_TryLoseFansFromPlayTime`

#### Silph Co. — **ESSENTIAL**

Map sections: `MAPSEC_SILPH_CO`. Maps: 12. Distinct trainers: 34. 11 floors, Card Key doors (silphco_doors.inc), warp pads, elevator; rival 7F; Boss Giovanni #2 11F -> Master Ball, Lapras gift 7F.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| SilphCo_10F_Frlg | `MAP_SILPH_CO_10F` | `LAYOUT_SILPH_CO_10F` | dungeon | 3 | 0 | 3 | 2 | 5 | 1 | 6 | 0 |
| SilphCo_11F_Frlg | `MAP_SILPH_CO_11F` | `LAYOUT_SILPH_CO_11F` | dungeon | 5 | 0 | 1 | 3 | 5 | 1 | 3 | 2 |
| SilphCo_1F_Frlg | `MAP_SILPH_CO_1F` | `LAYOUT_SILPH_CO_1F` | dungeon | 1 | 0 | 0 | 0 | 1 | 0 | 5 | 0 |
| SilphCo_2F_Frlg | `MAP_SILPH_CO_2F` | `LAYOUT_SILPH_CO_2F` | dungeon | 5 | 0 | 0 | 4 | 9 | 1 | 7 | 0 |
| SilphCo_3F_Frlg | `MAP_SILPH_CO_3F` | `LAYOUT_SILPH_CO_3F` | dungeon | 3 | 0 | 1 | 2 | 9 | 1 | 10 | 0 |
| SilphCo_4F_Frlg | `MAP_SILPH_CO_4F` | `LAYOUT_SILPH_CO_4F` | dungeon | 4 | 0 | 4 | 3 | 9 | 1 | 7 | 0 |
| SilphCo_5F_Frlg | `MAP_SILPH_CO_5F` | `LAYOUT_SILPH_CO_5F` | dungeon | 6 | 0 | 3 | 4 | 16 | 2 | 7 | 0 |
| SilphCo_6F_Frlg | `MAP_SILPH_CO_6F` | `LAYOUT_SILPH_CO_6F` | dungeon | 8 | 0 | 2 | 3 | 5 | 1 | 5 | 0 |
| SilphCo_7F_Frlg | `MAP_SILPH_CO_7F` | `LAYOUT_SILPH_CO_7F` | dungeon | 9 | 0 | 2 | 7 | 13 | 1 | 6 | 2 |
| SilphCo_8F_Frlg | `MAP_SILPH_CO_8F` | `LAYOUT_SILPH_CO_8F` | dungeon | 5 | 0 | 1 | 3 | 5 | 1 | 7 | 0 |
| SilphCo_9F_Frlg | `MAP_SILPH_CO_9F` | `LAYOUT_SILPH_CO_9F` | dungeon | 4 | 0 | 0 | 3 | 17 | 2 | 5 | 0 |
| SilphCo_Elevator_Frlg | `MAP_SILPH_CO_ELEVATOR` | `LAYOUT_SILPH_CO_ELEVATOR` | interior | 0 | 0 | 0 | 0 | 1 | 0 | 1 | 0 |

Trainers — SilphCo_10F: SCIENTIST_TRAVIS, TEAM_ROCKET_GRUNT_39 · SilphCo_11F: BOSS_GIOVANNI_2, TEAM_ROCKET_GRUNT_40, TEAM_ROCKET_GRUNT_41 · SilphCo_2F: SCIENTIST_CONNOR, SCIENTIST_JERRY, TEAM_ROCKET_GRUNT_23, TEAM_ROCKET_GRUNT_24 · SilphCo_3F: SCIENTIST_JOSE, TEAM_ROCKET_GRUNT_25 · SilphCo_4F: SCIENTIST_RODNEY, TEAM_ROCKET_GRUNT_26, TEAM_ROCKET_GRUNT_27 · SilphCo_5F: JUGGLER_DALTON, SCIENTIST_BEAU, TEAM_ROCKET_GRUNT_28, TEAM_ROCKET_GRUNT_29 · SilphCo_6F: SCIENTIST_TAYLOR, TEAM_ROCKET_GRUNT_30, TEAM_ROCKET_GRUNT_31 · SilphCo_7F: RIVAL_SILPH_BULBASAUR, RIVAL_SILPH_CHARMANDER, RIVAL_SILPH_SQUIRTLE, SCIENTIST_JOSHUA, TEAM_ROCKET_GRUNT_33, TEAM_ROCKET_GRUNT_34, TEAM_ROCKET_GRUNT_35 · SilphCo_8F: SCIENTIST_PARKER, TEAM_ROCKET_GRUNT_32, TEAM_ROCKET_GRUNT_36 · SilphCo_9F: SCIENTIST_ED, TEAM_ROCKET_GRUNT_37, TEAM_ROCKET_GRUNT_38

FRLG-only hooks — SilphCo_Elevator: `AnimateElevator`, `CloseElevatorCurrentFloorWindow`, `DrawElevatorCurrentFloorWindow`, `InitElevatorFloorSelectMenuPos`

#### Route 12 (+ gate, Fishing House) — **ESSENTIAL (one of two paths)**

Map sections: `MAPSEC_ROUTE_12`. Maps: 4. Distinct trainers: 8. Snorlax #1 (Poké Flute); Super Rod; Magikarp size record.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route12_Frlg | `MAP_ROUTE12` | `LAYOUT_ROUTE12` | route | 10 | 2 | 2 | 8 | 2 | 3 | 4 | 0 |
| Route12_FishingHouse_Frlg | `MAP_ROUTE12_FISHING_HOUSE` | `LAYOUT_HOUSE4_FRLG` | interior | 1 | 0 | 0 | 0 | 1 | 0 | 3 | 0 |
| Route12_NorthEntrance_1F_Frlg | `MAP_ROUTE12_NORTH_ENTRANCE_1F` | `LAYOUT_ROUTE12_NORTH_ENTRANCE_1F` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 5 | 0 |
| Route12_NorthEntrance_2F_Frlg | `MAP_ROUTE12_NORTH_ENTRANCE_2F` | `LAYOUT_ENTRANCE_2F` | interior | 1 | 0 | 0 | 0 | 2 | 0 | 1 | 0 |

Trainers — Route12: CAMPER_JUSTIN, FISHERMAN_ANDREW, FISHERMAN_CHIP, FISHERMAN_ELLIOT, FISHERMAN_HANK, FISHERMAN_NED, ROCKER_LUCA, YOUNG_COUPLE_GIA_JES

Connections — Route12 → up `MAP_LAVENDER_TOWN`, down `MAP_ROUTE13`, left `MAP_ROUTE11`

FRLG-only hooks — Route12_FishingHouse: `CompareMagikarpSize`, `DoesPlayerPartyContainSpecies`, `GetMagikarpSizeRecordInfo`

#### Route 13 — **ESSENTIAL (one of two paths)**

Map sections: `MAPSEC_ROUTE_13`. Maps: 1. Distinct trainers: 10.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route13_Frlg | `MAP_ROUTE13` | `LAYOUT_ROUTE13` | route | 10 | 1 | 0 | 10 | 3 | 1 | 0 | 0 |

Trainers — Route13: BEAUTY_LOLA, BEAUTY_SHEILA, BIKER_JARED, BIRD_KEEPER_PERRY, BIRD_KEEPER_ROBERT, BIRD_KEEPER_SEBASTIAN, PICNICKER_ALMA, PICNICKER_GWEN, PICNICKER_SUSIE, PICNICKER_VALERIE

Connections — Route13 → up `MAP_ROUTE12`, left `MAP_ROUTE14`

#### Route 14 — **ESSENTIAL (one of two paths)**

Map sections: `MAPSEC_ROUTE_14`. Maps: 1. Distinct trainers: 11.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route14_Frlg | `MAP_ROUTE14` | `LAYOUT_ROUTE14` | route | 12 | 3 | 0 | 11 | 1 | 2 | 0 | 0 |

Trainers — Route14: BIKER_GERALD, BIKER_ISAAC, BIKER_LUKAS, BIKER_MALIK, BIRD_KEEPER_BECK, BIRD_KEEPER_BENNY, BIRD_KEEPER_CARTER, BIRD_KEEPER_DONALD, BIRD_KEEPER_MARLON, BIRD_KEEPER_MITCH, TWINS_KIRI_JAN

Connections — Route14 → left `MAP_ROUTE15`, right `MAP_ROUTE13`

#### Route 15 (+ gate, Exp. Share aide) — **ESSENTIAL (one of two paths)**

Map sections: `MAPSEC_ROUTE_15`. Maps: 3. Distinct trainers: 11.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route15_Frlg | `MAP_ROUTE15` | `LAYOUT_ROUTE15` | route | 12 | 1 | 1 | 11 | 1 | 0 | 2 | 0 |
| Route15_WestEntrance_1F_Frlg | `MAP_ROUTE15_WEST_ENTRANCE_1F` | `LAYOUT_ENTRANCE_1F` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 5 | 0 |
| Route15_WestEntrance_2F_Frlg | `MAP_ROUTE15_WEST_ENTRANCE_2F` | `LAYOUT_ENTRANCE_2F` | interior | 1 | 0 | 0 | 0 | 2 | 0 | 1 | 0 |

Trainers — Route15: BEAUTY_GRACE, BEAUTY_OLIVIA, BIKER_ALEX, BIKER_ERNEST, BIRD_KEEPER_CHESTER, BIRD_KEEPER_EDWIN, CRUSH_KIN_RON_MYA, PICNICKER_BECKY, PICNICKER_CELIA, PICNICKER_KINDRA, PICNICKER_YAZMIN

Connections — Route15 → left `MAP_FUCHSIA_CITY`, right `MAP_ROUTE14`

FRLG-only hooks — Route15_WestEntrance_2F: `SetSeenMon`

#### Route 16 (+ gate, Fly house) — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_16`. Maps: 4. Distinct trainers: 7. Snorlax #2; house gives HM02 Fly; gate forces bike (ForcePlayerOntoBike).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route16_Frlg | `MAP_ROUTE16` | `LAYOUT_ROUTE16` | route | 9 | 1 | 0 | 7 | 2 | 1 | 5 | 0 |
| Route16_House_Frlg | `MAP_ROUTE16_HOUSE` | `LAYOUT_HOUSE1_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| Route16_NorthEntrance_1F_Frlg | `MAP_ROUTE16_NORTH_ENTRANCE_1F` | `LAYOUT_ROUTE16_NORTH_ENTRANCE_1F` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 5 | 11 |
| Route16_NorthEntrance_2F_Frlg | `MAP_ROUTE16_NORTH_ENTRANCE_2F` | `LAYOUT_ENTRANCE_2F` | interior | 3 | 0 | 0 | 0 | 2 | 0 | 1 | 0 |

Trainers — Route16: BIKER_HIDEO, BIKER_LAO, BIKER_RUBEN, CUE_BALL_CAMRON, CUE_BALL_KOJI, CUE_BALL_LUKE, YOUNG_COUPLE_LEA_JED

Connections — Route16 → down `MAP_ROUTE17`, right `MAP_CELADON_CITY`

FRLG-only hooks — Route16: `ForcePlayerOntoBike`

#### Route 17 (Cycling Road) — **ESSENTIAL (one of two paths)**

Map sections: `MAPSEC_ROUTE_17`. Maps: 1. Distinct trainers: 10. Downhill pull tiles (MB_CYCLING_ROAD_PULL_DOWN).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route17_Frlg | `MAP_ROUTE17` | `LAYOUT_ROUTE17` | route | 10 | 0 | 0 | 10 | 6 | 5 | 0 | 0 |

Trainers — Route17: BIKER_BILLY, BIKER_JAXON, BIKER_NIKOLAS, BIKER_VIRGIL, BIKER_WILLIAM, CUE_BALL_COREY, CUE_BALL_ISAIAH, CUE_BALL_JAMAL, CUE_BALL_RAUL, CUE_BALL_ZEEK

Connections — Route17 → up `MAP_ROUTE16`, down `MAP_ROUTE18`

#### Route 18 (+ gate) — **ESSENTIAL (one of two paths)**

Map sections: `MAPSEC_ROUTE_18`. Maps: 3. Distinct trainers: 3.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route18_Frlg | `MAP_ROUTE18` | `LAYOUT_ROUTE18` | route | 3 | 0 | 0 | 3 | 2 | 0 | 2 | 0 |
| Route18_EastEntrance_1F_Frlg | `MAP_ROUTE18_EAST_ENTRANCE_1F` | `LAYOUT_ENTRANCE_1F` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 3 | 11 |
| Route18_EastEntrance_2F_Frlg | `MAP_ROUTE18_EAST_ENTRANCE_2F` | `LAYOUT_ENTRANCE_2F` | interior | 1 | 0 | 0 | 0 | 2 | 0 | 1 | 0 |

Trainers — Route18: BIRD_KEEPER_JACOB, BIRD_KEEPER_RAMIRO, BIRD_KEEPER_WILTON

Connections — Route18 → up `MAP_ROUTE17`, right `MAP_FUCHSIA_CITY`

FRLG-only hooks — Route18: `ForcePlayerOntoBike`

#### Fuchsia City (+ Gym 5) — **ESSENTIAL**

Map sections: `MAPSEC_FUCHSIA_CITY`. Maps: 11. Distinct trainers: 7. Warden's House: Gold Teeth -> HM04 Strength; Good Rod.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| FuchsiaCity_Frlg | `MAP_FUCHSIA_CITY` | `LAYOUT_FUCHSIA_CITY` | town | 12 | 4 | 0 | 0 | 11 | 1 | 11 | 0 |
| FuchsiaCity_Gym_Frlg | `MAP_FUCHSIA_CITY_GYM` | `LAYOUT_FUCHSIA_CITY_GYM` | gym | 8 | 0 | 0 | 7 | 2 | 0 | 3 | 0 |
| FuchsiaCity_House1_Frlg | `MAP_FUCHSIA_CITY_HOUSE1` | `LAYOUT_HOUSE1_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| FuchsiaCity_House2_Frlg | `MAP_FUCHSIA_CITY_HOUSE2` | `LAYOUT_FUCHSIA_CITY_HOUSE2` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| FuchsiaCity_House3_Frlg | `MAP_FUCHSIA_CITY_HOUSE3` | `LAYOUT_HOUSE1_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| FuchsiaCity_Mart_Frlg | `MAP_FUCHSIA_CITY_MART` | `LAYOUT_MART_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| FuchsiaCity_PokemonCenter_1F_Frlg | `MAP_FUCHSIA_CITY_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| FuchsiaCity_PokemonCenter_2F_Frlg | `MAP_FUCHSIA_CITY_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| FuchsiaCity_SafariZone_Entrance_Frlg | `MAP_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE` | `LAYOUT_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 4 | 3 |
| FuchsiaCity_SafariZone_Office_Frlg | `MAP_FUCHSIA_CITY_SAFARI_ZONE_OFFICE` | `LAYOUT_FUCHSIA_CITY_SAFARI_ZONE_OFFICE` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| FuchsiaCity_WardensHouse_Frlg | `MAP_FUCHSIA_CITY_WARDENS_HOUSE` | `LAYOUT_FUCHSIA_CITY_WARDENS_HOUSE` | interior | 2 | 1 | 1 | 0 | 4 | 0 | 3 | 0 |

Trainers — FuchsiaCity_Gym: JUGGLER_KAYDEN, JUGGLER_KIRK, JUGGLER_NATE, JUGGLER_SHAWN, LEADER_KOGA, TAMER_EDGAR, TAMER_PHIL

Connections — FuchsiaCity → down `MAP_ROUTE19`, left `MAP_ROUTE18`, right `MAP_ROUTE15`

FRLG-only hooks — FuchsiaCity: `SetSeenMon`

#### Safari Zone — **ESSENTIAL**

Map sections: `MAPSEC_KANTO_SAFARI_ZONE`. Maps: 9. Distinct trainers: 0. Secret House gives HM03 Surf; Gold Teeth item ball (West). Safari step counter 600 in FRLG vs 500 Emerald (IS_FRLG in safari_zone.c).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| SafariZone_Center_Frlg | `MAP_SAFARI_ZONE_CENTER` | `LAYOUT_SAFARI_ZONE_CENTER` | route | 0 | 0 | 1 | 0 | 3 | 1 | 13 | 0 |
| SafariZone_East_Frlg | `MAP_SAFARI_ZONE_EAST` | `LAYOUT_SAFARI_ZONE_EAST` | route | 0 | 0 | 4 | 0 | 3 | 0 | 7 | 0 |
| SafariZone_North_Frlg | `MAP_SAFARI_ZONE_NORTH_FRLG` | `LAYOUT_SAFARI_ZONE_NORTH_FRLG` | route | 0 | 0 | 3 | 0 | 5 | 0 | 13 | 0 |
| SafariZone_West_Frlg | `MAP_SAFARI_ZONE_WEST` | `LAYOUT_SAFARI_ZONE_WEST` | route | 0 | 0 | 4 | 0 | 4 | 1 | 11 | 0 |
| SafariZone_Center_RestHouse_Frlg | `MAP_SAFARI_ZONE_CENTER_REST_HOUSE` | `LAYOUT_SAFARI_ZONE_REST_HOUSE_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| SafariZone_East_RestHouse_Frlg | `MAP_SAFARI_ZONE_EAST_REST_HOUSE` | `LAYOUT_SAFARI_ZONE_REST_HOUSE_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| SafariZone_North_RestHouse_Frlg | `MAP_SAFARI_ZONE_NORTH_REST_HOUSE` | `LAYOUT_SAFARI_ZONE_REST_HOUSE_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| SafariZone_SecretHouse_Frlg | `MAP_SAFARI_ZONE_SECRET_HOUSE` | `LAYOUT_SAFARI_ZONE_SECRET_HOUSE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| SafariZone_West_RestHouse_Frlg | `MAP_SAFARI_ZONE_WEST_REST_HOUSE` | `LAYOUT_SAFARI_ZONE_REST_HOUSE_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |

#### Route 19 — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_19`. Maps: 2. Distinct trainers: 11. Surf route south of Fuchsia.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route19_Frlg | `MAP_ROUTE19` | `LAYOUT_ROUTE19` | route | 12 | 0 | 0 | 11 | 1 | 0 | 0 | 0 |
| Route19_UnusedHouse_Frlg | `MAP_ROUTE19_UNUSED_HOUSE` | `LAYOUT_HOUSE2_FRLG` | interior | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

Trainers — Route19: SIS_AND_BRO_LIA_LUC, SWIMMER_FEMALE_ALICE, SWIMMER_FEMALE_ANYA, SWIMMER_FEMALE_CONNIE, SWIMMER_MALE_AXLE, SWIMMER_MALE_DAVID, SWIMMER_MALE_DOUGLAS, SWIMMER_MALE_MATTHEW, SWIMMER_MALE_REECE, SWIMMER_MALE_RICHARD, SWIMMER_MALE_TONY

Connections — Route19 → up `MAP_FUCHSIA_CITY`, left `MAP_ROUTE20`

#### Route 20 — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_20`. Maps: 1. Distinct trainers: 10.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route20_Frlg | `MAP_ROUTE20` | `LAYOUT_ROUTE20` | route | 11 | 0 | 0 | 10 | 2 | 1 | 2 | 0 |

Trainers — Route20: BIRD_KEEPER_ROGER, PICNICKER_IRENE, PICNICKER_MISSY, SWIMMER_FEMALE_MELISSA, SWIMMER_FEMALE_NORA, SWIMMER_FEMALE_SHIRLEY, SWIMMER_FEMALE_TIFFANY, SWIMMER_MALE_BARRY, SWIMMER_MALE_DARRIN, SWIMMER_MALE_DEAN

Connections — Route20 → left `MAP_CINNABAR_ISLAND`, right `MAP_ROUTE19`

#### Seafoam Islands — **ESSENTIAL (path) / KEY**

Map sections: `MAPSEC_SEAFOAM_ISLANDS`. Maps: 5. Distinct trainers: 0. Boulders into holes stop the B3F/B4F currents; Articuno; ForcePlayerToStartSurfing + SeafoamIslandsB4F_CurrentDumpsPlayerOnLand.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| SeafoamIslands_1F_Frlg | `MAP_SEAFOAM_ISLANDS_1F` | `LAYOUT_SEAFOAM_ISLANDS_1F` | dungeon | 0 | 2 | 1 | 0 | 0 | 0 | 7 | 0 |
| SeafoamIslands_B1F_Frlg | `MAP_SEAFOAM_ISLANDS_B1F` | `LAYOUT_SEAFOAM_ISLANDS_B1F` | dungeon | 0 | 2 | 2 | 0 | 0 | 0 | 11 | 0 |
| SeafoamIslands_B2F_Frlg | `MAP_SEAFOAM_ISLANDS_B2F` | `LAYOUT_SEAFOAM_ISLANDS_B2F` | dungeon | 0 | 2 | 1 | 0 | 0 | 0 | 11 | 0 |
| SeafoamIslands_B3F_Frlg | `MAP_SEAFOAM_ISLANDS_B3F` | `LAYOUT_SEAFOAM_ISLANDS_B3F` | dungeon | 0 | 6 | 0 | 0 | 0 | 1 | 9 | 0 |
| SeafoamIslands_B4F_Frlg | `MAP_SEAFOAM_ISLANDS_B4F` | `LAYOUT_SEAFOAM_ISLANDS_B4F` | dungeon | 1 | 2 | 1 | 0 | 2 | 1 | 4 | 3 |

FRLG-only hooks — SeafoamIslands_B4F: `ForcePlayerToStartSurfing`, `SeafoamIslandsB4F_CurrentDumpsPlayerOnLand`

#### Cinnabar Island (+ Gym 7, Pokémon Lab) — **ESSENTIAL**

Map sections: `MAPSEC_CINNABAR_ISLAND`. Maps: 9. Distinct trainers: 8. Gym locked by Secret Key; Lab revives fossils + in-game trades; Bill offers Sevii trip after Gym 7 (can be declined).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| CinnabarIsland_Frlg | `MAP_CINNABAR_ISLAND` | `LAYOUT_CINNABAR_ISLAND` | town | 4 | 0 | 0 | 0 | 4 | 0 | 5 | 1 |
| CinnabarIsland_Gym_Frlg | `MAP_CINNABAR_ISLAND_GYM` | `LAYOUT_CINNABAR_ISLAND_GYM` | gym | 9 | 0 | 0 | 8 | 15 | 0 | 3 | 0 |
| CinnabarIsland_Mart_Frlg | `MAP_CINNABAR_ISLAND_MART` | `LAYOUT_MART_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| CinnabarIsland_PokemonCenter_1F_Frlg | `MAP_CINNABAR_ISLAND_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 7 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| CinnabarIsland_PokemonCenter_2F_Frlg | `MAP_CINNABAR_ISLAND_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| CinnabarIsland_PokemonLab_Entrance_Frlg | `MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE` | `LAYOUT_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE` | interior | 1 | 0 | 0 | 0 | 4 | 0 | 6 | 0 |
| CinnabarIsland_PokemonLab_ExperimentRoom_Frlg | `MAP_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM` | `LAYOUT_CINNABAR_ISLAND_POKEMON_LAB_EXPERIMENT_ROOM` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| CinnabarIsland_PokemonLab_Lounge_Frlg | `MAP_CINNABAR_ISLAND_POKEMON_LAB_LOUNGE` | `LAYOUT_CINNABAR_ISLAND_POKEMON_LAB_LOUNGE` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| CinnabarIsland_PokemonLab_ResearchRoom_Frlg | `MAP_CINNABAR_ISLAND_POKEMON_LAB_RESEARCH_ROOM` | `LAYOUT_CINNABAR_ISLAND_POKEMON_LAB_RESEARCH_ROOM` | interior | 2 | 0 | 0 | 0 | 2 | 0 | 1 | 0 |

Trainers — CinnabarIsland_Gym: BURGLAR_DUSTY, BURGLAR_QUINN, BURGLAR_RAMON, LEADER_BLAINE, SUPER_NERD_AVERY, SUPER_NERD_DEREK, SUPER_NERD_ERIK, SUPER_NERD_ZAC

Connections — CinnabarIsland → up `MAP_ROUTE21_SOUTH`, right `MAP_ROUTE20`

FRLG-only hooks — CinnabarIsland_PokemonLab_Lounge: `CreateInGameTradePokemon`, `DoInGameTradeScene`, `GetInGameTradeSpeciesInfo`, `GetTradeSpecies`

#### Pokémon Mansion — **ESSENTIAL**

Map sections: `MAPSEC_POKEMON_MANSION`. Maps: 4. Distinct trainers: 7. Statue switches toggle doors (data/scripts/pokemon_mansion.inc); Secret Key in B1F.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| PokemonMansion_1F_Frlg | `MAP_POKEMON_MANSION_1F` | `LAYOUT_POKEMON_MANSION_1F` | dungeon | 2 | 0 | 3 | 2 | 1 | 1 | 10 | 0 |
| PokemonMansion_2F_Frlg | `MAP_POKEMON_MANSION_2F` | `LAYOUT_POKEMON_MANSION_2F` | dungeon | 1 | 0 | 3 | 1 | 3 | 0 | 5 | 0 |
| PokemonMansion_3F_Frlg | `MAP_POKEMON_MANSION_3F` | `LAYOUT_POKEMON_MANSION_3F` | dungeon | 2 | 0 | 2 | 2 | 2 | 1 | 8 | 0 |
| PokemonMansion_B1F_Frlg | `MAP_POKEMON_MANSION_B1F` | `LAYOUT_POKEMON_MANSION_B1F` | dungeon | 2 | 0 | 4 | 2 | 3 | 1 | 1 | 0 |

Trainers — PokemonMansion_1F: SCIENTIST_TED, YOUNGSTER_JOHNSON · PokemonMansion_2F: BURGLAR_ARNIE · PokemonMansion_3F: BURGLAR_SIMON, SCIENTIST_BRAYDON · PokemonMansion_B1F: BURGLAR_LEWIS, SCIENTIST_IVAN

#### Route 21 — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_21`. Maps: 2. Distinct trainers: 9. Sea route Cinnabar <-> Pallet (alternative to Seafoam).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route21_North_Frlg | `MAP_ROUTE21_NORTH` | `LAYOUT_ROUTE21_NORTH` | route | 6 | 0 | 0 | 4 | 0 | 1 | 0 | 0 |
| Route21_South_Frlg | `MAP_ROUTE21_SOUTH` | `LAYOUT_ROUTE21_SOUTH` | route | 5 | 0 | 0 | 5 | 0 | 0 | 0 | 0 |

Trainers — Route21_North: FISHERMAN_RONALD, FISHERMAN_WADE, SIS_AND_BRO_LIL_IAN, SWIMMER_MALE_SPENCER · Route21_South: FISHERMAN_CLAUDE, FISHERMAN_NOLAN, SWIMMER_MALE_JACK, SWIMMER_MALE_JEROME, SWIMMER_MALE_ROLAND

Connections — Route21_North → up `MAP_PALLET_TOWN`, down `MAP_ROUTE21_SOUTH` · Route21_South → up `MAP_ROUTE21_NORTH`, down `MAP_CINNABAR_ISLAND`

#### Route 23 — **ESSENTIAL**

Map sections: `MAPSEC_ROUTE_23`. Maps: 1. Distinct trainers: 0. 8 badge guards (data/scripts/route23.inc).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| Route23_Frlg | `MAP_ROUTE23` | `LAYOUT_ROUTE23` | route | 7 | 0 | 0 | 0 | 1 | 8 | 4 | 42 |

Connections — Route23 → up `MAP_INDIGO_PLATEAU_EXTERIOR`, down `MAP_ROUTE22`

#### Victory Road — **ESSENTIAL**

Map sections: `MAPSEC_KANTO_VICTORY_ROAD`. Maps: 3. Distinct trainers: 12. Strength boulder switches (4 MB_STRENGTH_BUTTON tiles).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| VictoryRoad_1F_Frlg | `MAP_VICTORY_ROAD_1F_FRLG` | `LAYOUT_VICTORY_ROAD_1F_FRLG` | dungeon | 2 | 3 | 2 | 2 | 0 | 2 | 2 | 1 |
| VictoryRoad_2F_Frlg | `MAP_VICTORY_ROAD_2F` | `LAYOUT_VICTORY_ROAD_2F` | dungeon | 6 | 3 | 4 | 5 | 0 | 0 | 9 | 2 |
| VictoryRoad_3F_Frlg | `MAP_VICTORY_ROAD_3F` | `LAYOUT_VICTORY_ROAD_3F` | dungeon | 6 | 4 | 2 | 5 | 0 | 0 | 5 | 1 |

Trainers — VictoryRoad_1F: COOLNAOMI, COOLROLANDO · VictoryRoad_2F: BLACK_BELT_DAISUKE, JUGGLER_GREGORY, JUGGLER_NELSON, POKEMANIAC_DAWSON, TAMER_VINCENT · VictoryRoad_3F: COOLALEXA, COOLCAROLINE, COOLCOLBY, COOLGEORGE, COOL_COUPLE_RAY_TYRA

#### Indigo Plateau — **ESSENTIAL**

Map sections: `MAPSEC_INDIGO_PLATEAU`. Maps: 3. Distinct trainers: 0. League lobby; credits start from Exterior after Hall of Fame (CB2_StartCreditsSequence).

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| IndigoPlateau_Exterior_Frlg | `MAP_INDIGO_PLATEAU_EXTERIOR` | `LAYOUT_INDIGO_PLATEAU_EXTERIOR` | town | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| IndigoPlateau_PokemonCenter_1F_Frlg | `MAP_INDIGO_PLATEAU_POKEMON_CENTER_1F` | `LAYOUT_INDIGO_PLATEAU_POKEMON_CENTER_1F` | interior | 8 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| IndigoPlateau_PokemonCenter_2F_Frlg | `MAP_INDIGO_PLATEAU_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |

Connections — IndigoPlateau_Exterior → down `MAP_ROUTE23`

FRLG-only hooks — IndigoPlateau_Exterior: `CB2_StartCreditsSequence`, `player_run_down`

#### Pokémon League (Elite Four rooms) — **ESSENTIAL**

Map sections: `MAPSEC_POKEMON_LEAGUE`. Maps: 6. Distinct trainers: 14.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| PokemonLeague_AgathasRoom_Frlg | `MAP_POKEMON_LEAGUE_AGATHAS_ROOM` | `LAYOUT_POKEMON_LEAGUE_AGATHAS_ROOM` | league | 1 | 0 | 0 | 2 | 0 | 0 | 2 | 0 |
| PokemonLeague_BrunosRoom_Frlg | `MAP_POKEMON_LEAGUE_BRUNOS_ROOM` | `LAYOUT_POKEMON_LEAGUE_BRUNOS_ROOM` | league | 1 | 0 | 0 | 2 | 0 | 0 | 2 | 0 |
| PokemonLeague_ChampionsRoom_Frlg | `MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM` | `LAYOUT_POKEMON_LEAGUE_CHAMPIONS_ROOM` | league | 2 | 0 | 0 | 6 | 0 | 0 | 2 | 0 |
| PokemonLeague_HallOfFame_Frlg | `MAP_POKEMON_LEAGUE_HALL_OF_FAME` | `LAYOUT_POKEMON_LEAGUE_HALL_OF_FAME` | league | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| PokemonLeague_LancesRoom_Frlg | `MAP_POKEMON_LEAGUE_LANCES_ROOM` | `LAYOUT_POKEMON_LEAGUE_LANCES_ROOM` | league | 1 | 0 | 0 | 2 | 0 | 0 | 2 | 0 |
| PokemonLeague_LoreleisRoom_Frlg | `MAP_POKEMON_LEAGUE_LORELEIS_ROOM` | `LAYOUT_POKEMON_LEAGUE_LORELEIS_ROOM` | league | 1 | 0 | 0 | 2 | 0 | 0 | 2 | 0 |

Trainers — PokemonLeague_AgathasRoom: ELITE_FOUR_AGATHA, ELITE_FOUR_AGATHA_2 · PokemonLeague_BrunosRoom: ELITE_FOUR_BRUNO, ELITE_FOUR_BRUNO_2 · PokemonLeague_ChampionsRoom: CHAMPION_FIRST_BULBASAUR, CHAMPION_FIRST_CHARMANDER, CHAMPION_FIRST_SQUIRTLE, CHAMPION_REMATCH_BULBASAUR, CHAMPION_REMATCH_CHARMANDER, CHAMPION_REMATCH_SQUIRTLE · PokemonLeague_LancesRoom: ELITE_FOUR_LANCE, ELITE_FOUR_LANCE_2 · PokemonLeague_LoreleisRoom: ELITE_FOUR_LORELEI, ELITE_FOUR_LORELEI_2

FRLG-only hooks — PokemonLeague_ChampionsRoom: `GetStarterPokemon` · PokemonLeague_HallOfFame: `EnterHallOfFame` · PokemonLeague_LancesRoom: `Script_TryGainNewFanFromCounterFrlg`

#### Cerulean Cave — **KEY OPTIONAL (postgame)**

Map sections: `MAPSEC_CERULEAN_CAVE`. Maps: 3. Distinct trainers: 0. Mewtwo; opened after Hall of Fame + Sevii events in FRLG.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| CeruleanCave_1F_Frlg | `MAP_CERULEAN_CAVE_1F` | `LAYOUT_CERULEAN_CAVE_1F` | dungeon | 0 | 6 | 3 | 0 | 0 | 1 | 8 | 0 |
| CeruleanCave_2F_Frlg | `MAP_CERULEAN_CAVE_2F` | `LAYOUT_CERULEAN_CAVE_2F` | dungeon | 0 | 10 | 3 | 0 | 0 | 0 | 6 | 0 |
| CeruleanCave_B1F_Frlg | `MAP_CERULEAN_CAVE_B1F` | `LAYOUT_CERULEAN_CAVE_B1F` | dungeon | 1 | 9 | 2 | 0 | 0 | 0 | 1 | 0 |

#### One Island, Kindle Road, Treasure Beach, Mt. Ember — **SKIP**

Map sections: `MAPSEC_EMBER_SPA`, `MAPSEC_KINDLE_ROAD`, `MAPSEC_MT_EMBER`, `MAPSEC_ONE_ISLAND`, `MAPSEC_TREASURE_BEACH`. Maps: 22. Distinct trainers: 18. Sevii 1; Mt. Ember = Moltres + Ruby. HM06 Rock Smash at Ember Spa.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| OneIsland_Frlg | `MAP_ONE_ISLAND` | `LAYOUT_ONE_ISLAND` | town | 3 | 0 | 0 | 0 | 2 | 0 | 4 | 0 |
| MtEmber_Exterior_Frlg | `MAP_MT_EMBER_EXTERIOR` | `LAYOUT_MT_EMBER_EXTERIOR` | route | 6 | 11 | 3 | 5 | 0 | 2 | 6 | 3 |
| OneIsland_KindleRoad_Frlg | `MAP_ONE_ISLAND_KINDLE_ROAD` | `LAYOUT_ONE_ISLAND_KINDLE_ROAD` | route | 13 | 13 | 3 | 12 | 2 | 0 | 3 | 0 |
| OneIsland_TreasureBeach_Frlg | `MAP_ONE_ISLAND_TREASURE_BEACH` | `LAYOUT_ONE_ISLAND_TREASURE_BEACH` | route | 2 | 0 | 0 | 1 | 0 | 8 | 0 | 0 |
| MtEmber_RubyPath_1F_Frlg | `MAP_MT_EMBER_RUBY_PATH_1F` | `LAYOUT_MT_EMBER_RUBY_PATH_1F` | dungeon | 0 | 5 | 0 | 0 | 0 | 0 | 3 | 0 |
| MtEmber_RubyPath_B1F_Frlg | `MAP_MT_EMBER_RUBY_PATH_B1F` | `LAYOUT_MT_EMBER_RUBY_PATH_B1F` | dungeon | 0 | 4 | 0 | 0 | 0 | 0 | 2 | 0 |
| MtEmber_RubyPath_B1F_Stairs_Frlg | `MAP_MT_EMBER_RUBY_PATH_B1F_STAIRS` | `LAYOUT_MT_EMBER_RUBY_PATH_B1F_STAIRS` | dungeon | 0 | 1 | 0 | 0 | 0 | 0 | 2 | 0 |
| MtEmber_RubyPath_B2F_Frlg | `MAP_MT_EMBER_RUBY_PATH_B2F` | `LAYOUT_MT_EMBER_RUBY_PATH_B2F` | dungeon | 0 | 7 | 0 | 0 | 0 | 0 | 2 | 0 |
| MtEmber_RubyPath_B2F_Stairs_Frlg | `MAP_MT_EMBER_RUBY_PATH_B2F_STAIRS` | `LAYOUT_MT_EMBER_RUBY_PATH_B2F_STAIRS` | dungeon | 0 | 2 | 0 | 0 | 0 | 0 | 2 | 0 |
| MtEmber_RubyPath_B3F_Frlg | `MAP_MT_EMBER_RUBY_PATH_B3F` | `LAYOUT_MT_EMBER_RUBY_PATH_B3F` | dungeon | 0 | 10 | 0 | 0 | 0 | 0 | 3 | 0 |
| MtEmber_RubyPath_B4F_Frlg | `MAP_MT_EMBER_RUBY_PATH_B4F` | `LAYOUT_MT_EMBER_RUBY_PATH_B4F` | dungeon | 0 | 0 | 0 | 0 | 26 | 0 | 2 | 0 |
| MtEmber_RubyPath_B5F_Frlg | `MAP_MT_EMBER_RUBY_PATH_B5F` | `LAYOUT_MT_EMBER_RUBY_PATH_B5F` | dungeon | 1 | 0 | 0 | 0 | 1 | 0 | 1 | 0 |
| MtEmber_SummitPath_1F_Frlg | `MAP_MT_EMBER_SUMMIT_PATH_1F` | `LAYOUT_MT_EMBER_SUMMIT_PATH_1F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| MtEmber_SummitPath_2F_Frlg | `MAP_MT_EMBER_SUMMIT_PATH_2F` | `LAYOUT_MT_EMBER_SUMMIT_PATH_2F` | dungeon | 0 | 9 | 0 | 0 | 0 | 0 | 2 | 0 |
| MtEmber_SummitPath_3F_Frlg | `MAP_MT_EMBER_SUMMIT_PATH_3F` | `LAYOUT_MT_EMBER_SUMMIT_PATH_3F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| MtEmber_Summit_Frlg | `MAP_MT_EMBER_SUMMIT` | `LAYOUT_MT_EMBER_SUMMIT` | dungeon | 1 | 4 | 0 | 0 | 0 | 0 | 1 | 0 |
| OneIsland_KindleRoad_EmberSpa_Frlg | `MAP_ONE_ISLAND_KINDLE_ROAD_EMBER_SPA` | `LAYOUT_ONE_ISLAND_KINDLE_ROAD_EMBER_SPA` | dungeon | 6 | 0 | 0 | 0 | 0 | 0 | 1 | 1 |
| OneIsland_Harbor_Frlg | `MAP_ONE_ISLAND_HARBOR` | `LAYOUT_ISLAND_HARBOR_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| OneIsland_House1_Frlg | `MAP_ONE_ISLAND_HOUSE1` | `LAYOUT_HOUSE3_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| OneIsland_House2_Frlg | `MAP_ONE_ISLAND_HOUSE2` | `LAYOUT_HOUSE3_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| OneIsland_PokemonCenter_1F_Frlg | `MAP_ONE_ISLAND_POKEMON_CENTER_1F` | `LAYOUT_ONE_ISLAND_POKEMON_CENTER_1F` | interior | 6 | 0 | 0 | 0 | 9 | 0 | 2 | 4 |
| OneIsland_PokemonCenter_2F_Frlg | `MAP_ONE_ISLAND_POKEMON_CENTER_2F` | `LAYOUT_ONE_ISLAND_POKEMON_CENTER_2F` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |

Trainers — MtEmber_Exterior: CRUSH_GIRL_JOCELYN, PKMN_RANGER_BETH, PKMN_RANGER_LOGAN, TEAM_ROCKET_GRUNT_43, TEAM_ROCKET_GRUNT_44 · OneIsland_KindleRoad: BLACK_BELT_HUGH, BLACK_BELT_SHEA, CAMPER_BRYCE, CRUSH_GIRL_SHARON, CRUSH_GIRL_TANYA, CRUSH_KIN_MIK_KIA, FISHERMAN_TOMMY, PICNICKER_CLAIRE, SWIMMER_FEMALE_ABIGAIL, SWIMMER_FEMALE_MARIA, SWIMMER_MALE_FINN, SWIMMER_MALE_GARRETT · OneIsland_TreasureBeach: SWIMMER_FEMALE_AMARA

Connections — OneIsland → down `MAP_ONE_ISLAND_TREASURE_BEACH`, right `MAP_ONE_ISLAND_KINDLE_ROAD` · OneIsland_KindleRoad → left `MAP_ONE_ISLAND` · OneIsland_TreasureBeach → up `MAP_ONE_ISLAND`

FRLG-only hooks — MtEmber_RubyPath_B5F: `braillemessage_wait` · OneIsland_PokemonCenter_1F: `SetPostgameFlags`

#### Two Island, Cape Brink — **SKIP**

Map sections: `MAPSEC_CAPE_BRINK`, `MAPSEC_TWO_ISLAND`. Maps: 8. Distinct trainers: 0. Move Relearner (ChooseMonForMoveRelearner); Cape Brink tutor.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| TwoIsland_Frlg | `MAP_TWO_ISLAND` | `LAYOUT_TWO_ISLAND` | town | 7 | 1 | 1 | 0 | 3 | 0 | 4 | 0 |
| TwoIsland_CapeBrink_Frlg | `MAP_TWO_ISLAND_CAPE_BRINK` | `LAYOUT_TWO_ISLAND_CAPE_BRINK` | route | 0 | 0 | 0 | 0 | 0 | 2 | 1 | 0 |
| TwoIsland_CapeBrink_House_Frlg | `MAP_TWO_ISLAND_CAPE_BRINK_HOUSE` | `LAYOUT_HOUSE3_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| TwoIsland_Harbor_Frlg | `MAP_TWO_ISLAND_HARBOR` | `LAYOUT_ISLAND_HARBOR_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| TwoIsland_House_Frlg | `MAP_TWO_ISLAND_HOUSE` | `LAYOUT_HOUSE3_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| TwoIsland_JoyfulGameCorner_Frlg | `MAP_TWO_ISLAND_JOYFUL_GAME_CORNER` | `LAYOUT_TWO_ISLAND_JOYFUL_GAME_CORNER` | interior | 4 | 0 | 0 | 0 | 2 | 0 | 1 | 0 |
| TwoIsland_PokemonCenter_1F_Frlg | `MAP_TWO_ISLAND_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| TwoIsland_PokemonCenter_2F_Frlg | `MAP_TWO_ISLAND_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |

Connections — TwoIsland → up `MAP_TWO_ISLAND_CAPE_BRINK` · TwoIsland_CapeBrink → down `MAP_TWO_ISLAND`

FRLG-only hooks — TwoIsland_House: `ChooseMonForMoveRelearner`

#### Three Island, Bond Bridge, Berry Forest, Port — **SKIP**

Map sections: `MAPSEC_BERRY_FOREST`, `MAPSEC_BOND_BRIDGE`, `MAPSEC_THREE_ISLAND`, `MAPSEC_THREE_ISLE_PATH`, `MAPSEC_THREE_ISLE_PORT`. Maps: 14. Distinct trainers: 10.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| ThreeIsland_Frlg | `MAP_THREE_ISLAND` | `LAYOUT_THREE_ISLAND` | town | 12 | 1 | 1 | 4 | 1 | 1 | 7 | 10 |
| ThreeIsland_BerryForest_Frlg | `MAP_THREE_ISLAND_BERRY_FOREST` | `LAYOUT_THREE_ISLAND_BERRY_FOREST` | route | 1 | 10 | 3 | 0 | 2 | 13 | 3 | 0 |
| ThreeIsland_BondBridge_Frlg | `MAP_THREE_ISLAND_BOND_BRIDGE` | `LAYOUT_THREE_ISLAND_BOND_BRIDGE` | route | 7 | 2 | 0 | 6 | 2 | 3 | 2 | 0 |
| ThreeIsland_Port_Frlg | `MAP_THREE_ISLAND_PORT` | `LAYOUT_THREE_ISLAND_PORT` | route | 3 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| ThreeIsland_DunsparceTunnel_Frlg | `MAP_THREE_ISLAND_DUNSPARCE_TUNNEL` | `LAYOUT_THREE_ISLAND_DUNSPARCE_TUNNEL` | dungeon | 1 | 0 | 0 | 0 | 0 | 1 | 2 | 0 |
| ThreeIsland_Harbor_Frlg | `MAP_THREE_ISLAND_HARBOR` | `LAYOUT_ISLAND_HARBOR_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| ThreeIsland_House1_Frlg | `MAP_THREE_ISLAND_HOUSE1` | `LAYOUT_THREE_ISLAND_HOUSE1` | interior | 1 | 0 | 0 | 0 | 1 | 0 | 1 | 0 |
| ThreeIsland_House2_Frlg | `MAP_THREE_ISLAND_HOUSE2` | `LAYOUT_HOUSE3_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| ThreeIsland_House3_Frlg | `MAP_THREE_ISLAND_HOUSE3` | `LAYOUT_HOUSE3_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| ThreeIsland_House4_Frlg | `MAP_THREE_ISLAND_HOUSE4` | `LAYOUT_HOUSE3_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| ThreeIsland_House5_Frlg | `MAP_THREE_ISLAND_HOUSE5` | `LAYOUT_HOUSE3_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| ThreeIsland_Mart_Frlg | `MAP_THREE_ISLAND_MART` | `LAYOUT_MART_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| ThreeIsland_PokemonCenter_1F_Frlg | `MAP_THREE_ISLAND_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| ThreeIsland_PokemonCenter_2F_Frlg | `MAP_THREE_ISLAND_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |

Trainers — ThreeIsland: BIKER_GOON, BIKER_GOON_2, BIKER_GOON_3, CUE_BALL_PAXTON · ThreeIsland_BondBridge: AROMA_LADY_NIKKI, AROMA_LADY_VIOLET, SWIMMER_FEMALE_TISHA, TUBER_ALEXIS, TUBER_AMIRA, TWINS_JOY_MEG

Connections — ThreeIsland → down `MAP_THREE_ISLAND_PORT`, left `MAP_THREE_ISLAND_BOND_BRIDGE` · ThreeIsland_BondBridge → right `MAP_THREE_ISLAND` · ThreeIsland_Port → up `MAP_THREE_ISLAND`

#### Four Island, Icefall Cave — **SKIP**

Map sections: `MAPSEC_FOUR_ISLAND`, `MAPSEC_ICEFALL_CAVE`. Maps: 13. Distinct trainers: 1. Icefall Cave cracked-ice puzzle (SetIcefallCaveCrackedIceMetatiles); HM07 Waterfall.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| FourIsland_Frlg | `MAP_FOUR_ISLAND` | `LAYOUT_FOUR_ISLAND` | town | 9 | 1 | 2 | 0 | 2 | 2 | 8 | 0 |
| FourIsland_IcefallCave_1F_Frlg | `MAP_FOUR_ISLAND_ICEFALL_CAVE_1F` | `LAYOUT_FOUR_ISLAND_ICEFALL_CAVE_1F` | dungeon | 0 | 0 | 2 | 0 | 0 | 0 | 6 | 0 |
| FourIsland_IcefallCave_B1F_Frlg | `MAP_FOUR_ISLAND_ICEFALL_CAVE_B1F` | `LAYOUT_FOUR_ISLAND_ICEFALL_CAVE_B1F` | dungeon | 0 | 0 | 2 | 0 | 0 | 0 | 3 | 0 |
| FourIsland_IcefallCave_Back_Frlg | `MAP_FOUR_ISLAND_ICEFALL_CAVE_BACK` | `LAYOUT_FOUR_ISLAND_ICEFALL_CAVE_BACK` | dungeon | 4 | 0 | 0 | 1 | 0 | 0 | 1 | 3 |
| FourIsland_IcefallCave_Entrance_Frlg | `MAP_FOUR_ISLAND_ICEFALL_CAVE_ENTRANCE` | `LAYOUT_FOUR_ISLAND_ICEFALL_CAVE_ENTRANCE` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| FourIsland_Harbor_Frlg | `MAP_FOUR_ISLAND_HARBOR` | `LAYOUT_ISLAND_HARBOR_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| FourIsland_House1_Frlg | `MAP_FOUR_ISLAND_HOUSE1` | `LAYOUT_HOUSE3_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| FourIsland_House2_Frlg | `MAP_FOUR_ISLAND_HOUSE2` | `LAYOUT_HOUSE3_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| FourIsland_LoreleisHouse_Frlg | `MAP_FOUR_ISLAND_LORELEIS_HOUSE` | `LAYOUT_FOUR_ISLAND_LORELEIS_HOUSE` | interior | 15 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| FourIsland_Mart_Frlg | `MAP_FOUR_ISLAND_MART` | `LAYOUT_MART_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| FourIsland_PokemonCenter_1F_Frlg | `MAP_FOUR_ISLAND_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 4 | 0 | 0 | 0 | 2 | 0 | 2 | 0 |
| FourIsland_PokemonCenter_2F_Frlg | `MAP_FOUR_ISLAND_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| FourIsland_PokemonDayCare_Frlg | `MAP_FOUR_ISLAND_POKEMON_DAY_CARE` | `LAYOUT_FOUR_ISLAND_POKEMON_DAY_CARE` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |

Trainers — FourIsland_IcefallCave_Back: TEAM_ROCKET_GRUNT_45

FRLG-only hooks — FourIsland_IcefallCave_1F: `SetIcefallCaveCrackedIceMetatiles` · FourIsland_LoreleisHouse: `UpdateLoreleiDollCollection` · FourIsland_PokemonDayCare: `ChooseSendDaycareMon`

#### Five Island, Meadow, Memorial Pillar, Water Labyrinth, Resort Gorgeous, Lost Cave, Rocket Warehouse — **SKIP**

Map sections: `MAPSEC_FIVE_ISLAND`, `MAPSEC_FIVE_ISLE_MEADOW`, `MAPSEC_LOST_CAVE`, `MAPSEC_MEMORIAL_PILLAR`, `MAPSEC_RESORT_GORGEOUS`, `MAPSEC_ROCKET_WAREHOUSE`, `MAPSEC_WATER_LABYRINTH`. Maps: 27. Distinct trainers: 23.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| FiveIsland_Frlg | `MAP_FIVE_ISLAND` | `LAYOUT_FIVE_ISLAND` | town | 2 | 1 | 0 | 0 | 1 | 0 | 4 | 0 |
| FiveIsland_Meadow_Frlg | `MAP_FIVE_ISLAND_MEADOW` | `LAYOUT_FIVE_ISLAND_MEADOW` | route | 3 | 2 | 2 | 3 | 1 | 0 | 1 | 0 |
| FiveIsland_MemorialPillar_Frlg | `MAP_FIVE_ISLAND_MEMORIAL_PILLAR` | `LAYOUT_FIVE_ISLAND_MEMORIAL_PILLAR` | route | 4 | 0 | 1 | 3 | 1 | 4 | 0 | 0 |
| FiveIsland_ResortGorgeous_Frlg | `MAP_FIVE_ISLAND_RESORT_GORGEOUS` | `LAYOUT_FIVE_ISLAND_RESORT_GORGEOUS` | route | 8 | 0 | 0 | 7 | 1 | 4 | 2 | 0 |
| FiveIsland_WaterLabyrinth_Frlg | `MAP_FIVE_ISLAND_WATER_LABYRINTH` | `LAYOUT_FIVE_ISLAND_WATER_LABYRINTH` | route | 2 | 0 | 0 | 1 | 0 | 0 | 0 | 0 |
| FiveIsland_LostCave_Entrance_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ENTRANCE` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ENTRANCE` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| FiveIsland_LostCave_Room10_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM10` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM10` | dungeon | 1 | 0 | 1 | 1 | 0 | 0 | 1 | 0 |
| FiveIsland_LostCave_Room11_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM11` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM11` | dungeon | 0 | 0 | 1 | 0 | 0 | 0 | 1 | 0 |
| FiveIsland_LostCave_Room12_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM12` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM12` | dungeon | 0 | 0 | 1 | 0 | 0 | 0 | 1 | 0 |
| FiveIsland_LostCave_Room13_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM13` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM13` | dungeon | 0 | 0 | 1 | 0 | 0 | 0 | 1 | 0 |
| FiveIsland_LostCave_Room14_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM14` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM14` | dungeon | 0 | 0 | 1 | 0 | 0 | 0 | 1 | 0 |
| FiveIsland_LostCave_Room1_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM1` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM1` | dungeon | 1 | 0 | 0 | 1 | 0 | 0 | 5 | 0 |
| FiveIsland_LostCave_Room2_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM2` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM2` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| FiveIsland_LostCave_Room3_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM3` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM3` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| FiveIsland_LostCave_Room4_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM4` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM4` | dungeon | 1 | 0 | 0 | 1 | 0 | 0 | 4 | 0 |
| FiveIsland_LostCave_Room5_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM5` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM5` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| FiveIsland_LostCave_Room6_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM6` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM6` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| FiveIsland_LostCave_Room7_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM7` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM7` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| FiveIsland_LostCave_Room8_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM8` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM8` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| FiveIsland_LostCave_Room9_Frlg | `MAP_FIVE_ISLAND_LOST_CAVE_ROOM9` | `LAYOUT_FIVE_ISLAND_LOST_CAVE_ROOM9` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| FiveIsland_Harbor_Frlg | `MAP_FIVE_ISLAND_HARBOR` | `LAYOUT_ISLAND_HARBOR_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| FiveIsland_House1_Frlg | `MAP_FIVE_ISLAND_HOUSE1` | `LAYOUT_HOUSE3_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| FiveIsland_House2_Frlg | `MAP_FIVE_ISLAND_HOUSE2` | `LAYOUT_HOUSE3_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| FiveIsland_PokemonCenter_1F_Frlg | `MAP_FIVE_ISLAND_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| FiveIsland_PokemonCenter_2F_Frlg | `MAP_FIVE_ISLAND_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| FiveIsland_ResortGorgeous_House_Frlg | `MAP_FIVE_ISLAND_RESORT_GORGEOUS_HOUSE` | `LAYOUT_FIVE_ISLAND_RESORT_GORGEOUS_HOUSE` | interior | 2 | 0 | 0 | 0 | 2 | 0 | 1 | 0 |
| FiveIsland_RocketWarehouse_Frlg | `MAP_FIVE_ISLAND_ROCKET_WAREHOUSE` | `LAYOUT_FIVE_ISLAND_ROCKET_WAREHOUSE` | interior | 6 | 0 | 4 | 6 | 23 | 2 | 1 | 3 |

Trainers — FiveIsland_Meadow: TEAM_ROCKET_GRUNT_49, TEAM_ROCKET_GRUNT_50, TEAM_ROCKET_GRUNT_51 · FiveIsland_MemorialPillar: BIRD_KEEPER_CHAZ, BIRD_KEEPER_HAROLD, BIRD_KEEPER_MILO · FiveIsland_ResortGorgeous: LADY_GILLIAN, LADY_JACKI, PAINTER_CELINA, PAINTER_DAISY, PAINTER_RAYNA, SWIMMER_MALE_TOBY, YOUNGSTER_DESTIN · FiveIsland_WaterLabyrinth: PKMN_BREEDER_ALIZE · FiveIsland_LostCave_Room10: LADY_SELPHY · FiveIsland_LostCave_Room1: RUIN_MANIAC_LAWSON · FiveIsland_LostCave_Room4: PSYCHIC_LAURA · FiveIsland_RocketWarehouse: SCIENTIST_GIDEON, TEAM_ROCKET_ADMIN, TEAM_ROCKET_ADMIN_2, TEAM_ROCKET_GRUNT_42, TEAM_ROCKET_GRUNT_47, TEAM_ROCKET_GRUNT_48

Connections — FiveIsland → up `MAP_FIVE_ISLAND_WATER_LABYRINTH`, right `MAP_FIVE_ISLAND_MEADOW` · FiveIsland_Meadow → left `MAP_FIVE_ISLAND`, right `MAP_FIVE_ISLAND_MEMORIAL_PILLAR` · FiveIsland_MemorialPillar → left `MAP_FIVE_ISLAND_MEADOW` · FiveIsland_ResortGorgeous → down `MAP_FIVE_ISLAND_WATER_LABYRINTH` · FiveIsland_WaterLabyrinth → up `MAP_FIVE_ISLAND_RESORT_GORGEOUS`, down `MAP_FIVE_ISLAND`

FRLG-only hooks — FiveIsland_WaterLabyrinth: `GetLeadMonFriendship`, `PlayerPartyContainsSpeciesWithPlayerID` · FiveIsland_ResortGorgeous_House: `DoesPlayerPartyContainSpecies`, `SampleResortGorgeousMonAndReward`

#### Six Island, Water Path, Ruin Valley, Green Path, Outcast Island, Pattern Bush, Dotted Hole, Altering Cave — **SKIP**

Map sections: `MAPSEC_ALTERING_CAVE_FRLG`, `MAPSEC_DOTTED_HOLE`, `MAPSEC_GREEN_PATH`, `MAPSEC_OUTCAST_ISLAND`, `MAPSEC_PATTERN_BUSH`, `MAPSEC_RUIN_VALLEY`, `MAPSEC_SIX_ISLAND`, `MAPSEC_WATER_PATH`. Maps: 20. Distinct trainers: 29. Dotted Hole: braille + Sapphire.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| SixIsland_Frlg | `MAP_SIX_ISLAND` | `LAYOUT_SIX_ISLAND` | town | 2 | 0 | 0 | 0 | 1 | 1 | 4 | 0 |
| SixIsland_GreenPath_Frlg | `MAP_SIX_ISLAND_GREEN_PATH` | `LAYOUT_SIX_ISLAND_GREEN_PATH` | route | 1 | 0 | 0 | 1 | 2 | 1 | 4 | 0 |
| SixIsland_OutcastIsland_Frlg | `MAP_SIX_ISLAND_OUTCAST_ISLAND` | `LAYOUT_SIX_ISLAND_OUTCAST_ISLAND` | route | 6 | 0 | 1 | 5 | 0 | 2 | 1 | 0 |
| SixIsland_PatternBush_Frlg | `MAP_SIX_ISLAND_PATTERN_BUSH` | `LAYOUT_SIX_ISLAND_PATTERN_BUSH` | route | 12 | 0 | 0 | 12 | 0 | 0 | 6 | 0 |
| SixIsland_RuinValley_Frlg | `MAP_SIX_ISLAND_RUIN_VALLEY` | `LAYOUT_SIX_ISLAND_RUIN_VALLEY` | route | 6 | 8 | 3 | 5 | 1 | 0 | 1 | 0 |
| SixIsland_WaterPath_Frlg | `MAP_SIX_ISLAND_WATER_PATH` | `LAYOUT_SIX_ISLAND_WATER_PATH` | route | 7 | 0 | 2 | 6 | 2 | 3 | 2 | 0 |
| SixIsland_AlteringCave_Frlg | `MAP_SIX_ISLAND_ALTERING_CAVE` | `LAYOUT_SIX_ISLAND_ALTERING_CAVE` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SixIsland_DottedHole_1F_Frlg | `MAP_SIX_ISLAND_DOTTED_HOLE_1F` | `LAYOUT_SIX_ISLAND_DOTTED_HOLE_1F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 4 | 0 |
| SixIsland_DottedHole_B1F_Frlg | `MAP_SIX_ISLAND_DOTTED_HOLE_B1F` | `LAYOUT_SIX_ISLAND_DOTTED_HOLE_B1F` | dungeon | 0 | 0 | 0 | 0 | 1 | 0 | 5 | 0 |
| SixIsland_DottedHole_B2F_Frlg | `MAP_SIX_ISLAND_DOTTED_HOLE_B2F` | `LAYOUT_SIX_ISLAND_DOTTED_HOLE_B2F` | dungeon | 0 | 0 | 0 | 0 | 1 | 0 | 5 | 0 |
| SixIsland_DottedHole_B3F_Frlg | `MAP_SIX_ISLAND_DOTTED_HOLE_B3F` | `LAYOUT_SIX_ISLAND_DOTTED_HOLE_B3F` | dungeon | 0 | 0 | 0 | 0 | 1 | 0 | 5 | 0 |
| SixIsland_DottedHole_B4F_Frlg | `MAP_SIX_ISLAND_DOTTED_HOLE_B4F` | `LAYOUT_SIX_ISLAND_DOTTED_HOLE_B4F` | dungeon | 0 | 0 | 0 | 0 | 1 | 0 | 5 | 0 |
| SixIsland_DottedHole_SapphireRoom_Frlg | `MAP_SIX_ISLAND_DOTTED_HOLE_SAPPHIRE_ROOM` | `LAYOUT_SIX_ISLAND_DOTTED_HOLE_SAPPHIRE_ROOM` | dungeon | 2 | 0 | 0 | 0 | 1 | 0 | 2 | 0 |
| SixIsland_Harbor_Frlg | `MAP_SIX_ISLAND_HARBOR` | `LAYOUT_ISLAND_HARBOR_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SixIsland_House_Frlg | `MAP_SIX_ISLAND_HOUSE` | `LAYOUT_HOUSE3_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SixIsland_Mart_Frlg | `MAP_SIX_ISLAND_MART` | `LAYOUT_MART_FRLG` | interior | 3 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SixIsland_PokemonCenter_1F_Frlg | `MAP_SIX_ISLAND_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| SixIsland_PokemonCenter_2F_Frlg | `MAP_SIX_ISLAND_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| SixIsland_WaterPath_House1_Frlg | `MAP_SIX_ISLAND_WATER_PATH_HOUSE1` | `LAYOUT_HOUSE4_FRLG` | interior | 1 | 0 | 0 | 0 | 1 | 0 | 1 | 0 |
| SixIsland_WaterPath_House2_Frlg | `MAP_SIX_ISLAND_WATER_PATH_HOUSE2` | `LAYOUT_HOUSE3_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |

Trainers — SixIsland_GreenPath: PSYCHIC_JACLYN · SixIsland_OutcastIsland: FISHERMAN_TYLOR, SIS_AND_BRO_AVA_GEB, SWIMMER_FEMALE_NICOLE, SWIMMER_MALE_MYMO, TEAM_ROCKET_GRUNT_46 · SixIsland_PatternBush: BUG_CATCHER_GARRET, BUG_CATCHER_JONAH, BUG_CATCHER_VANCE, CAMPER_RILEY, LASS_DALIA, LASS_JOANA, PICNICKER_MARCY, PKMN_BREEDER_ALLISON, PKMN_BREEDER_BETHANY, RUIN_MANIAC_LAYTON, YOUNGSTER_CORDELL, YOUNGSTER_NASH · SixIsland_RuinValley: HIKER_DARYL, POKEMANIAC_HECTOR, RUIN_MANIAC_FOSTER, RUIN_MANIAC_LARRY, RUIN_MANIAC_STANLY · SixIsland_WaterPath: AROMA_LADY_ROSE, HIKER_EARL, JUGGLER_EDWARD, SWIMMER_FEMALE_DENISE, SWIMMER_MALE_SAMIR, TWINS_MIU_MIA

Connections — SixIsland → right `MAP_SIX_ISLAND_WATER_PATH` · SixIsland_GreenPath → up `MAP_SIX_ISLAND_OUTCAST_ISLAND`, right `MAP_SIX_ISLAND_WATER_PATH` · SixIsland_OutcastIsland → down `MAP_SIX_ISLAND_GREEN_PATH` · SixIsland_RuinValley → right `MAP_SIX_ISLAND_WATER_PATH` · SixIsland_WaterPath → left `MAP_SIX_ISLAND_GREEN_PATH`, left `MAP_SIX_ISLAND`, left `MAP_SIX_ISLAND_RUIN_VALLEY`

FRLG-only hooks — SixIsland_DottedHole_SapphireRoom: `ShakeScreen`, `braillemessage_wait` · SixIsland_WaterPath_House1: `CompareHeracrossSize`, `DoesPlayerPartyContainSpecies`, `GetHeracrossSizeRecordInfo`

#### Seven Island, Sevault Canyon, Tanoby Ruins, Trainer Tower — **SKIP**

Map sections: `MAPSEC_CANYON_ENTRANCE`, `MAPSEC_DILFORD_CHAMBER`, `MAPSEC_LIPTOO_CHAMBER`, `MAPSEC_MONEAN_CHAMBER`, `MAPSEC_RIXY_CHAMBER`, `MAPSEC_SCUFIB_CHAMBER`, `MAPSEC_SEVAULT_CANYON`, `MAPSEC_SEVEN_ISLAND`, `MAPSEC_TANOBY_KEY`, `MAPSEC_TANOBY_RUINS`, `MAPSEC_TRAINER_TOWER`, `MAPSEC_TRAINER_TOWER_2`, `MAPSEC_VIAPOIS_CHAMBER`, `MAPSEC_WEEPTH_CHAMBER`. Maps: 31. Distinct trainers: 18. Trainer Tower code is compiled only if IS_FRLG && !FREE_TRAINER_TOWER.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| SevenIsland_Frlg | `MAP_SEVEN_ISLAND` | `LAYOUT_SEVEN_ISLAND` | town | 3 | 0 | 0 | 0 | 1 | 0 | 4 | 0 |
| SevenIsland_SevaultCanyon_Entrance_Frlg | `MAP_SEVEN_ISLAND_SEVAULT_CANYON_ENTRANCE` | `LAYOUT_SEVEN_ISLAND_SEVAULT_CANYON_ENTRANCE` | route | 7 | 0 | 0 | 5 | 1 | 1 | 0 | 0 |
| SevenIsland_SevaultCanyon_Frlg | `MAP_SEVEN_ISLAND_SEVAULT_CANYON` | `LAYOUT_SEVEN_ISLAND_SEVAULT_CANYON` | route | 9 | 8 | 3 | 7 | 1 | 1 | 2 | 0 |
| SevenIsland_TanobyRuins_Frlg | `MAP_SEVEN_ISLAND_TANOBY_RUINS` | `LAYOUT_SEVEN_ISLAND_TANOBY_RUINS` | route | 4 | 0 | 0 | 4 | 0 | 4 | 7 | 0 |
| SevenIsland_TrainerTower_Frlg | `MAP_SEVEN_ISLAND_TRAINER_TOWER` | `LAYOUT_SEVEN_ISLAND_TRAINER_TOWER` | route | 2 | 0 | 0 | 2 | 2 | 3 | 1 | 0 |
| SevenIsland_SevaultCanyon_TanobyKey_Frlg | `MAP_SEVEN_ISLAND_SEVAULT_CANYON_TANOBY_KEY` | `LAYOUT_SEVEN_ISLAND_SEVAULT_CANYON_TANOBY_KEY` | dungeon | 0 | 7 | 0 | 0 | 0 | 0 | 1 | 7 |
| SevenIsland_TanobyRuins_DilfordChamber_Frlg | `MAP_SEVEN_ISLAND_TANOBY_RUINS_DILFORD_CHAMBER` | `LAYOUT_SEVEN_ISLAND_TANOBY_RUINS_DILFORD_CHAMBER` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SevenIsland_TanobyRuins_LiptooChamber_Frlg | `MAP_SEVEN_ISLAND_TANOBY_RUINS_LIPTOO_CHAMBER` | `LAYOUT_SEVEN_ISLAND_TANOBY_RUINS_LIPTOO_CHAMBER` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SevenIsland_TanobyRuins_MoneanChamber_Frlg | `MAP_SEVEN_ISLAND_TANOBY_RUINS_MONEAN_CHAMBER` | `LAYOUT_SEVEN_ISLAND_TANOBY_RUINS_MONEAN_CHAMBER` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SevenIsland_TanobyRuins_RixyChamber_Frlg | `MAP_SEVEN_ISLAND_TANOBY_RUINS_RIXY_CHAMBER` | `LAYOUT_SEVEN_ISLAND_TANOBY_RUINS_RIXY_CHAMBER` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SevenIsland_TanobyRuins_ScufibChamber_Frlg | `MAP_SEVEN_ISLAND_TANOBY_RUINS_SCUFIB_CHAMBER` | `LAYOUT_SEVEN_ISLAND_TANOBY_RUINS_SCUFIB_CHAMBER` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SevenIsland_TanobyRuins_ViapoisChamber_Frlg | `MAP_SEVEN_ISLAND_TANOBY_RUINS_VIAPOIS_CHAMBER` | `LAYOUT_SEVEN_ISLAND_TANOBY_RUINS_VIAPOIS_CHAMBER` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SevenIsland_TanobyRuins_WeepthChamber_Frlg | `MAP_SEVEN_ISLAND_TANOBY_RUINS_WEEPTH_CHAMBER` | `LAYOUT_SEVEN_ISLAND_TANOBY_RUINS_WEEPTH_CHAMBER` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SevenIsland_Harbor_Frlg | `MAP_SEVEN_ISLAND_HARBOR` | `LAYOUT_ISLAND_HARBOR_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SevenIsland_House_Room1_Frlg | `MAP_SEVEN_ISLAND_HOUSE_ROOM1` | `LAYOUT_SEVEN_ISLAND_HOUSE_ROOM1` | interior | 1 | 0 | 0 | 0 | 1 | 0 | 2 | 0 |
| SevenIsland_House_Room2_Frlg | `MAP_SEVEN_ISLAND_HOUSE_ROOM2` | `LAYOUT_SEVEN_ISLAND_HOUSE_ROOM2` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SevenIsland_Mart_Frlg | `MAP_SEVEN_ISLAND_MART` | `LAYOUT_MART_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| SevenIsland_PokemonCenter_1F_Frlg | `MAP_SEVEN_ISLAND_POKEMON_CENTER_1F` | `LAYOUT_POKEMON_CENTER_1F_FRLG` | interior | 6 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| SevenIsland_PokemonCenter_2F_Frlg | `MAP_SEVEN_ISLAND_POKEMON_CENTER_2F` | `LAYOUT_POKEMON_CENTER_2F_FRLG` | interior | 4 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| SevenIsland_SevaultCanyon_House_Frlg | `MAP_SEVEN_ISLAND_SEVAULT_CANYON_HOUSE` | `LAYOUT_HOUSE3_FRLG` | interior | 2 | 0 | 1 | 0 | 0 | 0 | 1 | 0 |
| TrainerTower_1F_Frlg | `MAP_TRAINER_TOWER_1F` | `LAYOUT_TRAINER_TOWER_1F` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 2 | 3 |
| TrainerTower_2F_Frlg | `MAP_TRAINER_TOWER_2F` | `LAYOUT_TRAINER_TOWER_2F` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 3 | 3 |
| TrainerTower_3F_Frlg | `MAP_TRAINER_TOWER_3F` | `LAYOUT_TRAINER_TOWER_3F` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 3 | 3 |
| TrainerTower_4F_Frlg | `MAP_TRAINER_TOWER_4F` | `LAYOUT_TRAINER_TOWER_4F` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 3 | 3 |
| TrainerTower_5F_Frlg | `MAP_TRAINER_TOWER_5F` | `LAYOUT_TRAINER_TOWER_5F` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 3 | 3 |
| TrainerTower_6F_Frlg | `MAP_TRAINER_TOWER_6F` | `LAYOUT_TRAINER_TOWER_6F` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 3 | 3 |
| TrainerTower_7F_Frlg | `MAP_TRAINER_TOWER_7F` | `LAYOUT_TRAINER_TOWER_7F` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 3 | 3 |
| TrainerTower_8F_Frlg | `MAP_TRAINER_TOWER_8F` | `LAYOUT_TRAINER_TOWER_8F` | interior | 5 | 0 | 0 | 0 | 0 | 0 | 3 | 3 |
| TrainerTower_Elevator_Frlg | `MAP_TRAINER_TOWER_ELEVATOR` | `LAYOUT_TRAINER_TOWER_ELEVATOR` | interior | 0 | 0 | 0 | 0 | 1 | 0 | 1 | 0 |
| TrainerTower_Lobby_Frlg | `MAP_TRAINER_TOWER_LOBBY` | `LAYOUT_TRAINER_TOWER_LOBBY` | interior | 5 | 0 | 0 | 0 | 1 | 0 | 3 | 1 |
| TrainerTower_Roof_Frlg | `MAP_TRAINER_TOWER_ROOF` | `LAYOUT_TRAINER_TOWER_ROOF` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |

Trainers — SevenIsland_SevaultCanyon_Entrance: AROMA_LADY_MIAH, JUGGLER_MASON, PKMN_RANGER_MADELINE, PKMN_RANGER_NICOLAS, YOUNG_COUPLE_EVE_JON · SevenIsland_SevaultCanyon: COOLLEROY, COOLMICHELLE, COOL_COUPLE_LEX_NYA, CRUSH_GIRL_CYNDY, PKMN_RANGER_JACKSON, PKMN_RANGER_KATELYN, TAMER_EVAN · SevenIsland_TanobyRuins: GENTLEMAN_CLIFFORD, PAINTER_EDNA, RUIN_MANIAC_BENJAMIN, RUIN_MANIAC_BRANDON · SevenIsland_TrainerTower: PSYCHIC_DARIO, PSYCHIC_RODETTE

Connections — SevenIsland → up `MAP_SEVEN_ISLAND_TRAINER_TOWER`, down `MAP_SEVEN_ISLAND_SEVAULT_CANYON_ENTRANCE` · SevenIsland_SevaultCanyon_Entrance → up `MAP_SEVEN_ISLAND`, right `MAP_SEVEN_ISLAND_SEVAULT_CANYON` · SevenIsland_SevaultCanyon → down `MAP_SEVEN_ISLAND_TANOBY_RUINS`, left `MAP_SEVEN_ISLAND_SEVAULT_CANYON_ENTRANCE` · SevenIsland_TanobyRuins → up `MAP_SEVEN_ISLAND_SEVAULT_CANYON` · SevenIsland_TrainerTower → down `MAP_SEVEN_ISLAND`

FRLG-only hooks — SevenIsland_SevaultCanyon_TanobyKey: `ShakeScreen` · TrainerTower_Elevator: `AnimateElevator`, `CloseElevatorCurrentFloorWindow`, `DrawElevatorCurrentFloorWindow`, `InitElevatorFloorSelectMenuPos`

#### Navel Rock (event island) — **SKIP**

Map sections: `MAPSEC_NAVEL_ROCK_FRLG`. Maps: 22. Distinct trainers: 0. Emerald already has its own Navel Rock maps.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| NavelRock_Exterior_Frlg | `MAP_NAVEL_ROCK_EXTERIOR_FRLG` | `LAYOUT_NAVEL_ROCK_EXTERIOR_FRLG` | route | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_1F_Frlg | `MAP_NAVEL_ROCK_1F` | `LAYOUT_NAVEL_ROCK_1F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_B1F_Frlg | `MAP_NAVEL_ROCK_B1F_FRLG` | `LAYOUT_NAVEL_ROCK_B1F_FRLG` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B10F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B10F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B10F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B11F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B11F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B11F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B1F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B1F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B1F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B2F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B2F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B2F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B3F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B3F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B3F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B4F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B4F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B4F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B5F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B5F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B5F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B6F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B6F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B6F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B7F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B7F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B7F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B8F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B8F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B8F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_BasePath_B9F_Frlg | `MAP_NAVEL_ROCK_BASE_PATH_B9F` | `LAYOUT_NAVEL_ROCK_BASE_PATH_B9F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_Base_Frlg | `MAP_NAVEL_ROCK_BASE_FRLG` | `LAYOUT_NAVEL_ROCK_BASE_FRLG` | dungeon | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| NavelRock_Fork_Frlg | `MAP_NAVEL_ROCK_FORK_FRLG` | `LAYOUT_NAVEL_ROCK_FORK_FRLG` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 3 | 0 |
| NavelRock_SummitPath_2F_Frlg | `MAP_NAVEL_ROCK_SUMMIT_PATH_2F` | `LAYOUT_NAVEL_ROCK_SUMMIT_PATH_2F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_SummitPath_3F_Frlg | `MAP_NAVEL_ROCK_SUMMIT_PATH_3F` | `LAYOUT_NAVEL_ROCK_SUMMIT_PATH_3F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_SummitPath_4F_Frlg | `MAP_NAVEL_ROCK_SUMMIT_PATH_4F` | `LAYOUT_NAVEL_ROCK_SUMMIT_PATH_4F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_SummitPath_5F_Frlg | `MAP_NAVEL_ROCK_SUMMIT_PATH_5F` | `LAYOUT_NAVEL_ROCK_SUMMIT_PATH_5F` | dungeon | 0 | 0 | 0 | 0 | 0 | 0 | 2 | 0 |
| NavelRock_Summit_Frlg | `MAP_NAVEL_ROCK_SUMMIT` | `LAYOUT_NAVEL_ROCK_SUMMIT` | dungeon | 1 | 0 | 0 | 0 | 0 | 1 | 1 | 1 |
| NavelRock_Harbor_Frlg | `MAP_NAVEL_ROCK_HARBOR_FRLG` | `LAYOUT_ISLAND_HARBOR_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |

FRLG-only hooks — NavelRock_Base: `ShakeScreen`

#### Birth Island (event island) — **SKIP**

Map sections: `MAPSEC_BIRTH_ISLAND_FRLG`. Maps: 2. Distinct trainers: 0. Emerald already has its own Birth Island maps.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| BirthIsland_Exterior_Frlg | `MAP_BIRTH_ISLAND_EXTERIOR_FRLG` | `LAYOUT_BIRTH_ISLAND_EXTERIOR_FRLG` | route | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
| BirthIsland_Harbor_Frlg | `MAP_BIRTH_ISLAND_HARBOR_FRLG` | `LAYOUT_ISLAND_HARBOR_FRLG` | interior | 2 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |

#### Link rooms (Colosseum, Trade Center, Record Corner, Union Room) — **SKIP**

Map sections: `MAPSEC_SPECIAL_AREA`. Maps: 5. Distinct trainers: 0. Emerald has its own link rooms.

| Map folder | MAP id | Layout | Kind | NPC | FObj | Items | Trn | Signs | Hidden | Warps | Trig |
|---|---|---|---|--:|--:|--:|--:|--:|--:|--:|--:|
| BattleColosseum_2P_Frlg | `MAP_BATTLE_COLOSSEUM_2P_FRLG` | `LAYOUT_BATTLE_COLOSSEUM_2P_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 2 | 2 |
| BattleColosseum_4P_Frlg | `MAP_BATTLE_COLOSSEUM_4P_FRLG` | `LAYOUT_BATTLE_COLOSSEUM_4P_FRLG` | interior | 0 | 0 | 0 | 0 | 0 | 0 | 4 | 4 |
| RecordCorner_Frlg | `MAP_RECORD_CORNER_FRLG` | `LAYOUT_RECORD_CORNER_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 4 | 4 |
| TradeCenter_Frlg | `MAP_TRADE_CENTER_FRLG` | `LAYOUT_TRADE_CENTER_FRLG` | interior | 1 | 0 | 0 | 0 | 0 | 0 | 2 | 2 |
| UnionRoom_Frlg | `MAP_UNION_ROOM_FRLG` | `LAYOUT_UNION_ROOM_FRLG` | interior | 9 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |

---

## 4. Kanto gyms

All eight gyms use `MUS_RG_GYM` and primary tileset `gTileset_BuildingFrlg`, plus one secondary tileset per gym. Each gym has 2 statue signs. The statues use the `famechecker` macro.
After the leader is beaten, each gym runs `set_gym_trainers_frlg N`, which calls `Common_EventScript_SetGymTrainers_Frlg` in `data/scripts/set_gym_trainers.inc` (already assembled in the Emerald build). That marks the remaining gym trainers as beaten.
Rewards are given with `giveitem_msg`. Every leader is fought with `trainerbattle_single …, NO_MUSIC`.

| # | Gym map (`MAP id`) | Layout / size / tileset | Leader const | Badge flag | TM | Gym trainers | Entry requirement | Puzzle mechanic | What it needs to run |
|---|---|---|---|---|---|--:|---|---|---|
| 1 | PewterCity_Gym_Frlg (`MAP_PEWTER_CITY_GYM`) | `LAYOUT_PEWTER_CITY_GYM` 13×16, `gTileset_PewterGym` | `TRAINER_LEADER_BROCK` | `FLAG_BADGE01_GET` | TM39 | 1 (Camper Liam) | none. A Pewter City trigger forces the player to walk to the gym or museum (`walk_to_gym` macros). | None (straight path). | Nothing special. |
| 2 | CeruleanCity_Gym_Frlg (`MAP_CERULEAN_CITY_GYM`) | 17×20, `gTileset_CeruleanGym` | `TRAINER_LEADER_MISTY` | `FLAG_BADGE02_GET` | TM03 | 2 | none | None (pool layout, 2 trainers in line). | Nothing special. |
| 3 | VermilionCity_Gym_Frlg (`MAP_VERMILION_CITY_GYM`) | 11×21, `gTileset_VermilionGym` | `TRAINER_LEADER_LT_SURGE` | `FLAG_BADGE03_GET` | TM34 | 3 | **Cut**: a cuttable tree (`EventScript_CutTree`) blocks the gym door in `VermilionCity_Frlg`. | **Trash-can switches.** There are 15 trash cans (sign `bg_events`, `VermilionCity_Gym_EventScript_TrashCan1..15`). On transition, `special SetVermilionTrashCans` (`src/field_specials.c` L4836) picks switch 1 at random and switch 2 in an adjacent can (`VAR_0x8004`/`0x8005`, copied to `VAR_TEMP_0/1`). The 2nd can must be found right after the 1st. A wrong can resets both (`FLAG_TEMP_1` = first found). Both found → `FLAG_FOUND_BOTH_VERMILION_GYM_SWITCHES`. Two electric beams are removed with `setmetatile` (`METATILE_VermilionGym_Beam_*`). | FRLG-only special `SetVermilionTrashCans` (C is compiled in Emerald too). Needs the `gTileset_VermilionGym` metatiles. |
| 4 | CeladonCity_Gym_Frlg (`MAP_CELADON_CITY_GYM`) | 13×20, `gTileset_CeladonGym` | `TRAINER_LEADER_ERIKA` | `FLAG_BADGE04_GET` | TM19 | 7 | none | **Cut trees.** 3 `OBJ_EVENT_GFX_CUTTABLE_TREE_FRLG` objects inside the garden gate the route to Erika. | Shared `EventScript_CutTree`. Needs the FRLG tree sprite (guarded graphics). |
| 5 | FuchsiaCity_Gym_Frlg (`MAP_FUCHSIA_CITY_GYM`) | 15×23, `gTileset_FuchsiaGym` | `TRAINER_LEADER_KOGA` | `FLAG_BADGE05_GET` | TM06 | 6 | none | **Invisible walls.** 50 blocks use metatile `0x282` (looks like floor, collision bit set) to make a hidden maze. There is no script logic: it is all in `data/layouts/FuchsiaCity_Gym_Frlg/map.bin`. | Nothing in script. Layout + tileset only. |
| 6 | SaffronCity_Gym_Frlg (`MAP_SAFFRON_CITY_GYM`) | 29×25, `gTileset_SaffronGym` | `TRAINER_LEADER_SABRINA` | `FLAG_BADGE06_GET` | TM04 | 7 | **Silph Co. cleared**: the door-guard grunt and Rocket NPCs in `SaffronCity_Frlg` are hidden by `FLAG_HIDE_SAFFRON_ROCKETS`. To reach Saffron at all you need **Tea** (`ITEM_TEA` from `CeladonCity_Condominiums_1F_Frlg`; the 4 gate maps run `removeitem ITEM_TEA`). | **Teleport pads.** 30 pad metatiles have behaviour `MB_AQUA_HIDEOUT_WARP` (= FRLG "regular warp"). There are 33 `warp_events`: 30 internal and 3 exits. The 9 rooms are linked only by pads. | Runtime handler `MetatileBehavior_IsAquaHideoutWarp` in `src/field_control_avatar.c` L995, which is unguarded. Pure map data. |
| 7 | CinnabarIsland_Gym_Frlg (`MAP_CINNABAR_ISLAND_GYM`) | 30×25, `gTileset_CinnabarGym` | `TRAINER_LEADER_BLAINE` | `FLAG_BADGE07_GET` | TM38 | 7 | **Secret Key**: the gym door in `CinnabarIsland_Frlg` stays locked until `FLAG_HIDE_POKEMON_MANSION_B1F_SECRET_KEY` is set (item ball in `PokemonMansion_B1F_Frlg`). | **Quiz doors.** 6 quiz machines (sign `bg_events`; 15 signs total) ask YES/NO questions (`MSGBOX_YESNO`). A right answer opens the next door with `setmetatile METATILE_CinnabarGym_*` and sets `FLAG_CINNABAR_GYM_QUIZ_1..6`. A wrong answer starts a battle with that room's trainer. OnLoad re-opens doors from the flags. **The correct answers are hard-coded and rewritten questions must keep them:** Q1 YES, Q2 NO, Q3 NO, Q4 NO, Q5 YES, Q6 NO (`CinnabarIsland_Gym_Text_QuizQuestion1..6`). | FRLG flags `FLAG_CINNABAR_GYM_QUIZ_1..6`. Needs the `gTileset_CinnabarGym` metatiles. |
| 8 | ViridianCity_Gym_Frlg (`MAP_VIRIDIAN_CITY_GYM`) | 20×24, `gTileset_ViridianGym` | `TRAINER_LEADER_GIOVANNI` | `FLAG_BADGE08_GET` | TM26 | 8 | **Badges 2-7**: `ViridianCity_EventScript_TryUnlockGym` checks `FLAG_BADGE02..07_GET` and sets `VAR_MAP_SCENE_VIRIDIAN_CITY_GYM_DOOR`. | **Spin tiles.** 12 arrow tiles (`MB_SPIN_RIGHT/LEFT/UP/DOWN`, 3 each) push the player until a `MB_STOP_SPINNING` tile (12). Giovanni leaves after the battle (`removeobject LOCALID_VIRIDIAN_GIOVANNI`) and the script sets `VAR_MAP_SCENE_ROUTE22 = 3`, which unlocks the late rival battle on Route 22. | Runtime spin handler in `src/field_player_avatar.c` L182-185 and L460-469 (unguarded). Pure map data. |

Gym-adjacent obstacles on the critical path (the same tile/sprite mechanics appear in the dungeons):

* **Spin-tile mazes**: `RocketHideout_B2F_Frlg` (54 spin tiles) and `RocketHideout_B3F_Frlg` (22). Same handler as the Viridian Gym.
* **Warp pads**: Silph Co. 2F-11F (`MB_AQUA_HIDEOUT_WARP`, 3-7 per floor).
* **Card-key doors**: `data/scripts/silphco_doors.inc` (`EventScript_NeedCardKey`, `EventScript_Close*Door*`). The Card Key is an item ball in `SilphCo_5F_Frlg`.
* **Mansion switches**: `data/scripts/pokemon_mansion.inc` (`PokemonMansion_EventScript_SecretSwitch`, `…PressSwitch_1F`). Statues toggle door metatiles.
* **Strength boulders + switches**: Victory Road (4 `MB_STRENGTH_BUTTON` tiles over 3 floors; vars `VAR_MAP_SCENE_VICTORY_ROAD_*`, reset on Route 23). Seafoam Islands: 2 boulders per floor fall through `MB_MT_PYRE_HOLE` holes and stop the B3F/B4F currents (`MB_*WARD_CURRENT`: 56 on B3F, 173 on B4F). `special ForcePlayerToStartSurfing` and `SeafoamIslandsB4F_CurrentDumpsPlayerOnLand` handle B4F.
* **Cycling Road**: `Route17_Frlg` has 2,107 `MB_CYCLING_ROAD_PULL_DOWN*` tiles. `special ForcePlayerOntoBike` runs in `Route16_Frlg` and `Route18_Frlg`.
* **Snorlax ×2**: `Route12_Frlg` and `Route16_Frlg`, through `data/scripts/static_pokemon.inc` `EventScript_AwakenSnorlax` (Poké Flute).
* **Ghost blockade**: `PokemonTower_6F_Frlg` uses `special StartMarowakBattle` (Silph Scope check). Rival battle on `PokemonTower_2F_Frlg`.

---

## 5. Indigo Plateau, Elite Four and Hall of Fame

| Map folder | MAP id | Layout (size) | Music | Contents / mechanics |
|---|---|---|---|---|
| Route23_Frlg | `MAP_ROUTE23` | `LAYOUT_ROUTE23` | — | 7 badge-guard NPCs (`data/scripts/route23.inc`, `Route23_EventScript_*BadgeGuard`, checks `FLAG_BADGE0x_GET`). The map script resets the Victory Road boulder vars. |
| Route22_NorthEntrance_Frlg | `MAP_ROUTE22_NORTH_ENTRANCE` | — | — | Boulder Badge guard (`Route22_NorthEntrance_EventScript_BadgeGuard` in `route23.inc`). |
| VictoryRoad_1F/2F/3F_Frlg | `MAP_VICTORY_ROAD_1F_FRLG`, `MAP_VICTORY_ROAD_2F`, `MAP_VICTORY_ROAD_3F` | — | — | 12 trainers. Strength puzzles (see §4). |
| IndigoPlateau_Exterior_Frlg | `MAP_INDIGO_PLATEAU_EXTERIOR` | 24×20, `gTileset_IndigoPlateau` | `MUS_RG_VICTORY_ROAD` | After the Hall of Fame: camera cutscene (`SpawnCameraObject`, `player_run_down`) → `special CB2_StartCreditsSequence`. The Emerald build would run the **Emerald** credits (two implementations: `src/credits.c` / `src/credits_frlg.c`). |
| IndigoPlateau_PokemonCenter_1F_Frlg | `MAP_INDIGO_PLATEAU_POKEMON_CENTER_1F` | 25×18 | `MUS_RG_GYM` | Lobby, nurse, mart. Its warp leads into Lorelei's room. `IsNationalPokedexEnabled` checks. |
| IndigoPlateau_PokemonCenter_2F_Frlg | `MAP_INDIGO_PLATEAU_POKEMON_CENTER_2F` | 15×10 | `MUS_RG_POKE_CENTER` | Link-room warps (FRLG Trade Center / Union Room). |
| PokemonLeague_LoreleisRoom_Frlg | `MAP_POKEMON_LEAGUE_LORELEIS_ROOM` | 13×13, `gTileset_PokemonLeague` | `MUS_RG_GYM` | `TRAINER_ELITE_FOUR_LORELEI` / `_2`, `trainerbattle_no_intro`, sets `FLAG_DEFEATED_LORELEI`. All four E4 rooms pick the `_2` rematch team when **`FLAG_IS_CHAMPION`** is set. That flag is only set in the Sevii postgame. |
| PokemonLeague_BrunosRoom_Frlg | `MAP_POKEMON_LEAGUE_BRUNOS_ROOM` | 13×13 | `MUS_RG_ROCKET_HIDEOUT` | `TRAINER_ELITE_FOUR_BRUNO` / `_2`. |
| PokemonLeague_AgathasRoom_Frlg | `MAP_POKEMON_LEAGUE_AGATHAS_ROOM` | 13×13 | `MUS_RG_POKE_TOWER` | `TRAINER_ELITE_FOUR_AGATHA` / `_2`. |
| PokemonLeague_LancesRoom_Frlg | `MAP_POKEMON_LEAGUE_LANCES_ROOM` | 28×24 | `MUS_RG_VICTORY_ROAD` | `TRAINER_ELITE_FOUR_LANCE` / `_2`. `special Script_TryGainNewFanFromCounterFrlg` (Trainer Fan Club). |
| PokemonLeague_ChampionsRoom_Frlg | `MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM` | 13×20 | `MUS_RG_VICTORY_ROAD` | Rival champion: `TRAINER_CHAMPION_FIRST_{SQUIRTLE,BULBASAUR,CHARMANDER}` / `TRAINER_CHAMPION_REMATCH_*` (rematch if `FLAG_IS_CHAMPION`; intro text by `FLAG_SYS_GAME_CLEAR`). The branch follows `specialvar VAR_RESULT, GetStarterPokemon`. Sets `FLAG_DEFEATED_CHAMP`. Oak walks in, then `warp MAP_POKEMON_LEAGUE_HALL_OF_FAME`. |
| PokemonLeague_HallOfFame_Frlg | `MAP_POKEMON_LEAGUE_HALL_OF_FAME` | 11×13, `gTileset_HallOfFame` | `MUS_RG_SLOW_PALLET` | `special EnterHallOfFame`. |

Shared League logic is in `data/scripts/pokemon_league.inc`:

* `PokemonLeague_EventScript_OpenDoor`, `EnterRoom`, `SetDoorOpen`, `PreventExit`, `CloseEntry`, `OpenDoorLance`.
* `PokemonLeague_EventScript_DoLightingEffect`, which calls `special DoPokemonLeagueLightingEffect` with `VAR_0x8004` = room index 0-4.
* Each room uses `FLAG_TEMP_2..5` for its door states.

---

## 6. FRLG-only engine hooks

### 6.1 Specials used only by Kanto/Sevii scripts (65)

None of these specials appear in any Hoenn script. Their C code **is compiled in the Emerald build too** (`data/specials.inc` has no guards), with two exceptions:

* `CB2_StartCreditsSequence` has two implementations, chosen by `#if IS_FRLG`.
* The Trainer Tower functions behind `CallTrainerTowerFunc` are stubbed unless `IS_FRLG && !FREE_TRAINER_TOWER`.

| Special | Used by | C location |
|---|---|---|
| `AnimateElevator` | CeladonCity_DepartmentStore_Elevator, RocketHideout_Elevator, SilphCo_Elevator, TrainerTower_Elevator | `src/field_specials.c` L5281 |
| `AnimateTeleporterCable` | Route25_SeaCottage | `src/field_specials.c` L4817 |
| `AnimateTeleporterHousing` | Route25_SeaCottage | `src/field_specials.c` L4753 |
| `CB2_StartCreditsSequence` | IndigoPlateau_Exterior | `src/credits.c` L397 |
| `CallTrainerTowerFunc` | trainer_tower.inc | `src/trainer_tower.c` L356 |
| `CapeBrinkGetMoveToTeachLeadPokemon` | move_tutors_frlg.inc | `src/field_specials.c` L4637 |
| `ChooseMonForMoveRelearner` | TwoIsland_House | `src/party_menu.c` L8123 |
| `ChooseSendDaycareMon` | FourIsland_PokemonDayCare, day_care_frlg.inc | `src/daycare.c` L1488 |
| `CloseElevatorCurrentFloorWindow` | CeladonCity_DepartmentStore_Elevator, RocketHideout_Elevator, SilphCo_Elevator, TrainerTower_Elevator | `src/field_specials.c` L5344 |
| `CloseMuseumFossilPic` | PewterCity_Museum_1F | `src/script_menu.c` L1215 |
| `CompareHeracrossSize` | SixIsland_WaterPath_House1 | `src/pokemon_size_record.c` L217 |
| `CompareMagikarpSize` | Route12_FishingHouse | `src/pokemon_size_record.c` L231 |
| `CreateInGameTradePokemon` | CinnabarIsland_PokemonLab_Lounge | `src/trade.c` L4639 |
| `DaisyMassageServices` | PalletTown_RivalsHouse | `src/field_specials.c` L4597 |
| `DisableMsgBoxWalkaway` | PalletTown, PewterCity, move_tutors_frlg.inc | `src/script.c` L739 |
| `DoInGameTradeScene` | CinnabarIsland_PokemonLab_Lounge | `src/trade.c` L4862 |
| `DoPokemonLeagueLightingEffect` | pokemon_league.inc | `src/field_specials.c` L5476 |
| `DoSSAnneDepartureCutscene` | SSAnne_Exterior | `src/ss_anne.c` L106 |
| `DoSeagallopFerryScene` | seagallop.inc | `src/seagallop.c` L179 |
| `DoesPlayerPartyContainSpecies` | FiveIsland_ResortGorgeous_House, Route12_FishingHouse, SixIsland_WaterPath_House1 | `src/field_specials.c` L4936 |
| `DrawElevatorCurrentFloorWindow` | CeladonCity_DepartmentStore_Elevator, RocketHideout_Elevator, SilphCo_Elevator, TrainerTower_Elevator | `src/field_specials.c` L5328 |
| `DrawSeagallopDestinationMenu` | seagallop.inc | `src/script_menu.c` L1226 |
| `EnterHallOfFame` | PokemonLeague_HallOfFame | `src/post_battle_event_funcs.c` L95 |
| `ForcePlayerOntoBike` | Route16, Route18 | `src/field_specials.c` L5395 |
| `ForcePlayerToStartSurfing` | SeafoamIslands_B4F | `src/field_specials.c` L5714 |
| `GetCostToWithdrawRoute5DaycareMon` | day_care_frlg.inc | `src/daycare.c` L1522 |
| `GetHeracrossSizeRecordInfo` | SixIsland_WaterPath_House1 | `src/pokemon_size_record.c` L210 |
| `GetInGameTradeSpeciesInfo` | CinnabarIsland_PokemonLab_Lounge | `src/trade.c` L4545 |
| `GetLeadMonFriendship` | FiveIsland_WaterLabyrinth, PalletTown_RivalsHouse | `src/field_specials.c` L4603 |
| `GetMagikarpSizeRecordInfo` | Route12_FishingHouse | `src/pokemon_size_record.c` L224 |
| `GetNumLevelsGainedForRoute5DaycareMon` | day_care_frlg.inc | `src/daycare.c` L1542 |
| `GetRandomSlotMachineId` | CeladonCity_GameCorner | `src/field_specials.c` L4973 |
| `GetSeagallopNumber` | seagallop.inc | `src/seagallop.c` L457 |
| `GetSelectedSeagallopDestination` | seagallop.inc | `src/script_menu.c` L1278 |
| `GetStarterPokemon` | PokemonLeague_ChampionsRoom | `src/starter_choose.c` L350 |
| `GetTradeSpecies` | CinnabarIsland_PokemonLab_Lounge | `src/trade.c` L4630 |
| `HasAllKantoMons` | CeladonCity_Condominiums_3F | `src/pokedex.c` L4610 |
| `HasLearnedAllMovesFromCapeBrinkTutor` | move_tutors_frlg.inc | `src/field_specials.c` L4685 |
| `InitElevatorFloorSelectMenuPos` | CeladonCity_DepartmentStore_Elevator, RocketHideout_Elevator, SilphCo_Elevator, TrainerTower_Elevator | `src/field_specials.c` L5163 |
| `IsPlayerLeftOfVermilionSailor` | seagallop.inc | `src/seagallop.c` L499 |
| `IsPlayerNotInTrainerTowerLobby` | pkmn_center_nurse_frlg.inc | `src/field_specials.c` L5403 |
| `IsThereMonInRoute5Daycare` | day_care_frlg.inc | `src/daycare.c` L1532 |
| `OpenMuseumFossilPic` | PewterCity_Museum_1F | `src/script_menu.c` L1182 |
| `PlayerPartyContainsSpeciesWithPlayerID` | FiveIsland_WaterLabyrinth | `src/field_specials.c` L5622 |
| `PutMonInRoute5Daycare` | day_care_frlg.inc | `src/daycare.c` L1514 |
| `SampleResortGorgeousMonAndReward` | FiveIsland_ResortGorgeous_House | `src/field_specials.c` L5573 |
| `Script_BufferFanClubTrainerName` | SaffronCity_PokemonTrainerFanClub | `src/trainer_fan_club.c` L239 |
| `Script_GetNumFansOfPlayerInTrainerFanClub` | SaffronCity_PokemonTrainerFanClub | `src/trainer_fan_club.c` L169 |
| `Script_IsFanClubMemberFanOfPlayer` | SaffronCity_PokemonTrainerFanClub | `src/trainer_fan_club.c` L222 |
| `Script_TryGainNewFanFromCounterFrlg` | PokemonLeague_LancesRoom | `src/trainer_fan_club.c` L368 |
| `Script_TryLoseFansFromPlayTime` | SaffronCity_PokemonTrainerFanClub | `src/trainer_fan_club.c` L188 |
| `SeafoamIslandsB4F_CurrentDumpsPlayerOnLand` | SeafoamIslands_B4F | `src/field_player_avatar.c` L1972 |
| `SetIcefallCaveCrackedIceMetatiles` | FourIsland_IcefallCave_1F | `src/field_tasks.c` L987 |
| `SetPostgameFlags` | OneIsland_PokemonCenter_1F | `src/save_location.c` L145 |
| `SetSeenMon` | FuchsiaCity, Route15_WestEntrance_2F, Route25_SeaCottage, SSAnne_2F_Room1 | `src/field_specials.c` L4706 |
| `SetVermilionTrashCans` | VermilionCity_Gym | `src/field_specials.c` L4836 |
| `SetWalkingIntoSignVars` | PalletTown | `src/script.c` L744 |
| `ShakeScreen` | NavelRock_Base, SevenIsland_SevaultCanyon_TanobyKey, SixIsland_DottedHole_SapphireRoom | `src/field_specials.c` L5642 |
| `StartMarowakBattle` | PokemonTower_6F | `src/battle_setup.c` L556 |
| `StartOldManTutorialBattle` | ViridianCity | `src/battle_setup.c` L523 |
| `StickerManGetBragFlags` | trainer_card_frlg.inc | `src/field_specials.c` L5742 |
| `TakePokemonFromRoute5Daycare` | day_care_frlg.inc | `src/daycare.c` L1551 |
| `UpdateLoreleiDollCollection` | FourIsland_LoreleisHouse | `src/field_specials.c` L5550 |
| `UpdateTrainerCardPhotoIcons` | trainer_card_frlg.inc | `src/field_specials.c` L5719 |
| `WonderNews_GetRewardInfo` | CeruleanCity_House4 | `src/wonder_news.c` L55 |

All other specials used by Kanto scripts (103) are shared with Hoenn scripts.

### 6.2 Script macros used only by Kanto/Sevii scripts

All of these are defined unguarded in `asm/macros/event.inc` (or the Trainer Tower macro file), so they assemble in either build:

| Macro | Used in | Notes |
|---|---|---|
| `set_gym_trainers_frlg N` | the 8 Kanto gyms | → `Common_EventScript_SetGymTrainers_Frlg` (`data/scripts/set_gym_trainers.inc`). |
| `famechecker PERSON, idx` | gyms, cities, E4 rooms | Fame Checker key item (`src/fame_checker.c`, unguarded). Can be dropped with the text. |
| `giveitem_msg`, `msgreceiveditem` | rewards everywhere | Wrapper around `additem` + fanfare. |
| `setworldmapflag FLAG_WORLD_MAP_*` | every town/dungeon OnTransition | FRLG fly/region-map visit flags (`FLAG_WORLD_MAP_PALLET_TOWN` = `SYS_FLAGS+0x90`…). These must be renumbered with the other FRLG flags. |
| `trainerbattle_earlyrival` | `PalletTown_ProfessorOaksLab`, `Route22` | The battle can be lost without a white-out (first rival fights). |
| `call_if_not_defeated` | `RocketHideout_B1F` | |
| `signmsg` / `normalmsg`, `walk_to_*` movement macros | `PalletTown`, `PewterCity`, `OneIsland`, `move_tutors_frlg.inc` | Forced-walk cutscenes. |
| `braillemessage_wait`, `restore_anim` | `MtEmber_RubyPath_B5F`, `SixIsland_DottedHole_SapphireRoom` | Sevii only. |
| `player_run_down` | `IndigoPlateau_Exterior` | Post-HoF cutscene. |
| `ttower_*` (20 macros) | `TrainerTower_Lobby`, `data/scripts/trainer_tower.inc` | Sevii Trainer Tower only. The C is stubbed outside FRLG builds. |

### 6.3 Object-event graphics compiled only under `IS_FRLG`

115 of the 128 graphics IDs used by Kanto/Sevii maps (1,362 objects) are only compiled under `IS_FRLG`:

`AGATHA ARTICUNO BALDING_MAN BEAUTY_FRLG BIKER BILL BLACK_BELT_FRLG BLAINE BLUE BOY BREAKABLE_ROCK_FRLG BROCK BRUNO BUG_CATCHER_FRLG CABLE_CLUB_RECEPTIONIST CAMPER_FRLG CAPTAIN CELIO CHANNELER CHANSEY CHEF CLEFAIRY CLERK CLIPBOARD COOLTRAINER_F COOLTRAINER_M CRUSH_GIRL CUBONE CUTTABLE_TREE_FRLG DAISY DODUO ERIKA FAT_MAN_FRLG FEAROW FISHER FOSSIL_FRLG GBA_KID GENTLEMAN_FRLG GIOVANNI GYM_GUY HIKER_FRLG JIGGLYPUFF KANGASKHAN KOGA LANCE LAPRAS LAPRAS_DOLL LASS_FRLG LITTLE_BOY_FRLG LITTLE_GIRL_FRLG LORELEI LT_SURGE MACHOKE MACHOP MAN MEOWTH METEORITE MEWTWO MG_DELIVERYMAN MISTY MOLTRES MOM_FRLG MR_FUJI NIDORAN_F NIDORAN_M NIDORINO NURSE_FRLG OLD_AMBER OLD_MAN_1 OLD_MAN_2 OLD_WOMAN_FRLG PICNICKER_FRLG PIDGEOT PIDGEY PIKACHU_FRLG POKEDEX POKE_MANIAC_FRLG POLICEMAN POLIWRATH PROF_OAK PSYDUCK PUSHABLE_BOULDER_FRLG ROCKER ROCKET_F ROCKET_M RUBY SABRINA SAILOR_FRLG SAPPHIRE SCIENTIST SEAGALLOP SEEL SLOWBRO SLOWPOKE SNORLAX SPEAROW SS_ANNE SWIMMER_F_LAND SWIMMER_F_WATER SWIMMER_M_LAND SWIMMER_M_WATER TOWN_MAP TRAINER_TOWER_DUDE TUBER_F_FRLG TUBER_M_WATER UNION_ROOM_RECEPTIONIST VOLTORB WIGGLYTUFF WOMAN_1_FRLG WOMAN_2_FRLG WOMAN_3_FRLG WORKER_F WORKER_M YOUNGSTER_FRLG ZAPDOS` (all `OBJ_EVENT_GFX_` prefixed).

The other 13 are the `OBJ_EVENT_GFX_VAR_0..7` placeholders, `0`, and Emerald-shared sprites (item ball etc.). The **Rocket grunts**, **Giovanni**, the **rival** (`BLUE`) and **Prof. Oak** are in the guarded list. If the new villain organisation replaces Team Rocket in Kanto too, `ROCKET_M/F` and `GIOVANNI` can be remapped to the hack's own villain sprites instead of being un-guarded.

---

## 7. Kanto wild-encounter tables (`src/data/wild_encounters.json`, group `gWildMonHeaders`)

There are 124 maps, each with a `…_FireRed` and a `…_LeafGreen` base label (248 entries). The table below shows the **FireRed** land and surf species (species order = first slot appearance; level range and encounter rate), plus every species difference between the FR and LG versions. Types: land / water (surf) / rock (Rock Smash) / fishing (old+good+super rod).
Act column: **E** = essential Kanto area, **O** = optional/key-optional Kanto area, **S** = Sevii (skip).

| Act | Map | Types | Land (FireRed table) | Surf (FireRed table) | FR vs LG differences |
|---|---|---|---|---|---|
| E | CeladonCity (`MAP_CELADON_CITY`) | water,fishing |  | Psyduck/Koffing (Lv5-40, rate 1) | water: FR+Psyduck / LG+Slowpoke |
| E | CeruleanCity (`MAP_CERULEAN_CITY`) | water,fishing |  | Tentacool (Lv5-40, rate 1) | fishing: FR+Psyduck / LG+Slowpoke |
| E | CinnabarIsland (`MAP_CINNABAR_ISLAND`) | water,fishing |  | Tentacool (Lv5-40, rate 1) | fishing: FR+Psyduck,Seadra,Shellder / LG+Slowbro,Slowpoke,Staryu |
| E | FuchsiaCity (`MAP_FUCHSIA_CITY`) | water,fishing |  | Psyduck (Lv20-40, rate 1) | water: FR+Psyduck / LG+Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | MtMoon_1F (`MAP_MT_MOON_1F`) | land | Zubat/Geodude/Paras/Clefairy (Lv7-10, rate 7) |  |  |
| E | MtMoon_B1F (`MAP_MT_MOON_B1F`) | land | Paras (Lv5-10, rate 5) |  |  |
| E | MtMoon_B2F (`MAP_MT_MOON_B2F`) | land | Zubat/Geodude/Paras/Clefairy (Lv8-12, rate 7) |  |  |
| E | PalletTown (`MAP_PALLET_TOWN`) | water,fishing |  | Tentacool (Lv5-40, rate 1) | fishing: FR+Psyduck,Seadra,Shellder / LG+Kingler,Slowpoke,Staryu |
| E | PokemonMansion_1F (`MAP_POKEMON_MANSION_1F`) | land | Koffing/Raticate/Growlithe/Rattata/Grimer/Weezing (Lv26-36, rate 7) |  | land: FR+Growlithe,Weezing / LG+Muk,Vulpix |
| E | PokemonMansion_2F (`MAP_POKEMON_MANSION_2F`) | land | Koffing/Raticate/Growlithe/Rattata/Grimer/Weezing (Lv26-36, rate 7) |  | land: FR+Growlithe,Weezing / LG+Muk,Vulpix |
| E | PokemonMansion_3F (`MAP_POKEMON_MANSION_3F`) | land | Koffing/Raticate/Growlithe/Rattata/Grimer/Weezing (Lv26-36, rate 7) |  | land: FR+Growlithe,Weezing / LG+Muk,Vulpix |
| E | PokemonMansion_B1F (`MAP_POKEMON_MANSION_B1F`) | land | Koffing/Raticate/Ditto/Growlithe/Grimer/Weezing/Rattata (Lv26-38, rate 5) |  | land: FR+Growlithe,Weezing / LG+Muk,Vulpix |
| E | PokemonTower_3F (`MAP_POKEMON_TOWER_3F`) | land | Gastly/Cubone/Haunter (Lv13-20, rate 2) |  |  |
| E | PokemonTower_4F (`MAP_POKEMON_TOWER_4F`) | land | Gastly/Haunter/Cubone (Lv13-20, rate 4) |  |  |
| E | PokemonTower_5F (`MAP_POKEMON_TOWER_5F`) | land | Gastly/Haunter/Cubone (Lv13-20, rate 6) |  |  |
| E | PokemonTower_6F (`MAP_POKEMON_TOWER_6F`) | land | Gastly/Haunter/Cubone (Lv14-23, rate 8) |  |  |
| E | PokemonTower_7F (`MAP_POKEMON_TOWER_7F`) | land | Gastly/Haunter/Cubone (Lv15-25, rate 10) |  |  |
| E | RockTunnel_1F (`MAP_ROCK_TUNNEL_1F`) | land | Zubat/Geodude/Mankey/Machop/Onix (Lv13-17, rate 7) |  |  |
| E | RockTunnel_B1F (`MAP_ROCK_TUNNEL_B1F`) | land,rock | Zubat/Geodude/Mankey/Machop/Onix (Lv13-17, rate 7) |  |  |
| E | Route1 (`MAP_ROUTE1`) | land | Pidgey/Rattata (Lv2-5, rate 21) |  |  |
| E | Route10 (`MAP_ROUTE10`) | land,water,fishing | Spearow/Voltorb/Ekans (Lv11-17, rate 21) | Tentacool (Lv5-40, rate 2) | land: FR+Ekans / LG+Sandshrew; fishing: FR+Psyduck / LG+Slowpoke |
| E | Route11 (`MAP_ROUTE11`) | land,water,fishing | Ekans/Spearow/Drowzee (Lv11-17, rate 21) | Tentacool (Lv5-40, rate 2) | land: FR+Ekans / LG+Sandshrew; fishing: FR+Psyduck / LG+Slowpoke |
| E | Route12 (`MAP_ROUTE12`) | land,water,fishing | Oddish/Venonat/Pidgey/Gloom (Lv22-30, rate 21) | Tentacool (Lv5-40, rate 2) | land: FR+Gloom,Oddish / LG+Bellsprout,Weepinbell; fishing: FR+Psyduck / LG+Slowpoke |
| E | Route13 (`MAP_ROUTE13`) | land,water,fishing | Oddish/Venonat/Pidgey/Ditto/Pidgeotto/Gloom (Lv22-30, rate 21) | Tentacool (Lv5-40, rate 2) | land: FR+Gloom,Oddish / LG+Bellsprout,Weepinbell; fishing: FR+Psyduck / LG+Slowpoke |
| E | Route14 (`MAP_ROUTE14`) | land | Oddish/Venonat/Ditto/Pidgey/Gloom/Pidgeotto (Lv22-30, rate 21) |  | land: FR+Gloom,Oddish / LG+Bellsprout,Weepinbell |
| E | Route15 (`MAP_ROUTE15`) | land | Oddish/Venonat/Pidgey/Ditto/Pidgeotto/Gloom (Lv22-30, rate 21) |  | land: FR+Gloom,Oddish / LG+Bellsprout,Weepinbell |
| E | Route16 (`MAP_ROUTE16`) | land | Spearow/Doduo/Rattata/Raticate (Lv18-25, rate 21) |  |  |
| E | Route17 (`MAP_ROUTE17`) | land | Spearow/Doduo/Raticate/Rattata/Fearow (Lv20-29, rate 21) |  |  |
| E | Route18 (`MAP_ROUTE18`) | land | Spearow/Doduo/Raticate/Fearow/Rattata (Lv20-29, rate 21) |  |  |
| E | Route19 (`MAP_ROUTE19`) | water,fishing |  | Tentacool (Lv5-40, rate 2) | fishing: FR+Psyduck,Seadra / LG+Kingler,Slowpoke |
| E | Route2 (`MAP_ROUTE2`) | land | Rattata/Pidgey/Caterpie/Weedle (Lv2-5, rate 21) |  |  |
| E | Route20 (`MAP_ROUTE20`) | water,fishing |  | Tentacool (Lv5-40, rate 2) | fishing: FR+Psyduck,Seadra / LG+Kingler,Slowpoke |
| E | Route21_North (`MAP_ROUTE21_NORTH`) | land,water,fishing | Tangela (Lv17-28, rate 14) | Tentacool (Lv5-40, rate 2) | fishing: FR+Psyduck,Seadra / LG+Kingler,Slowpoke |
| E | Route21_South (`MAP_ROUTE21_SOUTH`) | land,water,fishing | Tangela (Lv17-28, rate 14) | Tentacool (Lv5-40, rate 2) | fishing: FR+Psyduck,Seadra / LG+Kingler,Slowpoke |
| E | Route22 (`MAP_ROUTE22`) | land,water,fishing | Rattata/Mankey/Spearow (Lv2-5, rate 21) | Psyduck (Lv20-40, rate 2) | water: FR+Psyduck / LG+Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | Route23 (`MAP_ROUTE23`) | land,water,fishing | Mankey/Fearow/Spearow/Ekans/Primeape/Arbok (Lv32-44, rate 21) | Psyduck (Lv20-40, rate 2) | land: FR+Arbok,Ekans / LG+Sandshrew,Sandslash; water: FR+Psyduck / LG+Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | Route24 (`MAP_ROUTE24`) | land,water,fishing | Weedle/Caterpie/Pidgey/Oddish/Abra/Kakuna/Metapod (Lv7-14, rate 21) | Tentacool (Lv5-40, rate 2) | land: FR+Oddish / LG+Bellsprout; fishing: FR+Psyduck / LG+Slowpoke |
| E | Route25 (`MAP_ROUTE25`) | land,water,fishing | Weedle/Caterpie/Pidgey/Oddish/Abra/Kakuna/Metapod (Lv8-14, rate 21) | Psyduck (Lv20-40, rate 2) | land: FR+Oddish / LG+Bellsprout; water: FR+Psyduck / LG+Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | Route3 (`MAP_ROUTE3`) | land | Spearow/Pidgey/Mankey/Nidoran_M/Jigglypuff/Nidoran_F (Lv3-8, rate 21) |  |  |
| E | Route4 (`MAP_ROUTE4`) | land,water,fishing | Spearow/Rattata/Ekans/Mankey (Lv6-12, rate 21) | Tentacool (Lv5-40, rate 2) | land: FR+Ekans / LG+Sandshrew; fishing: FR+Psyduck / LG+Slowpoke |
| E | Route5 (`MAP_ROUTE5`) | land | Meowth/Pidgey/Oddish (Lv10-16, rate 21) |  | land: FR+Oddish / LG+Bellsprout |
| E | Route6 (`MAP_ROUTE6`) | land,water,fishing | Meowth/Pidgey/Oddish (Lv10-16, rate 21) | Psyduck (Lv20-40, rate 2) | land: FR+Oddish / LG+Bellsprout; water: FR+Psyduck / LG+Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | Route7 (`MAP_ROUTE7`) | land | Pidgey/Meowth/Oddish/Growlithe (Lv17-22, rate 21) |  | land: FR+Growlithe,Oddish / LG+Bellsprout,Vulpix |
| E | Route8 (`MAP_ROUTE8`) | land | Pidgey/Meowth/Growlithe/Ekans (Lv15-20, rate 21) |  | land: FR+Ekans,Growlithe / LG+Sandshrew,Vulpix |
| E | Route9 (`MAP_ROUTE9`) | land | Spearow/Rattata/Ekans (Lv11-17, rate 21) |  | land: FR+Ekans / LG+Sandshrew |
| E | SSAnne_Exterior (`MAP_SSANNE_EXTERIOR`) | water,fishing |  | Tentacool (Lv5-40, rate 1) | fishing: FR+Psyduck,Shellder / LG+Slowpoke,Staryu |
| E | SafariZone_Center (`MAP_SAFARI_ZONE_CENTER`) | land,water,fishing | Rhyhorn/Nidoran_M/Exeggcute/Venonat/Nidorino/Nidorina/Parasect/Scyther/Chansey (Lv22-31, rate 21) | Psyduck (Lv20-40, rate 2) | land: FR+Nidoran_M,Scyther / LG+Nidoran_F,Pinsir; water: FR+Psyduck / LG+Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | SafariZone_East (`MAP_SAFARI_ZONE_EAST`) | land,water,fishing | Nidoran_M/Doduo/Exeggcute/Paras/Nidorino/Nidoran_F/Parasect/Kangaskhan/Scyther (Lv22-33, rate 21) | Psyduck (Lv20-40, rate 2) | land: FR+Nidorino,Scyther / LG+Nidorina,Pinsir; water: FR+Psyduck / LG+Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | SafariZone_North (`MAP_SAFARI_ZONE_NORTH_FRLG`) | land,water,fishing | Rhyhorn/Nidoran_M/Exeggcute/Paras/Nidorino/Nidorina/Venomoth/Chansey/Tauros (Lv23-32, rate 21) | Psyduck (Lv20-40, rate 2) | land: FR+Nidoran_M / LG+Nidoran_F; water: FR+Psyduck / LG+Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | SafariZone_West (`MAP_SAFARI_ZONE_WEST`) | land,water,fishing | Doduo/Nidoran_M/Exeggcute/Venonat/Nidorino/Nidoran_F/Venomoth/Tauros/Kangaskhan (Lv22-32, rate 21) | Psyduck (Lv20-40, rate 2) | land: FR+Nidorino / LG+Nidorina; water: FR+Psyduck / LG+Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | SeafoamIslands_1F (`MAP_SEAFOAM_ISLANDS_1F`) | land | Psyduck/Zubat/Golbat (Lv22-33, rate 7) |  | land: FR+Psyduck / LG+Slowpoke |
| E | SeafoamIslands_B1F (`MAP_SEAFOAM_ISLANDS_B1F`) | land | Psyduck/Seel/Zubat/Golbat/Golduck (Lv22-35, rate 7) |  | land: FR+Golduck,Psyduck / LG+Slowbro,Slowpoke |
| E | SeafoamIslands_B2F (`MAP_SEAFOAM_ISLANDS_B2F`) | land | Psyduck/Seel/Zubat/Golbat/Golduck (Lv22-34, rate 7) |  | land: FR+Golduck,Psyduck / LG+Slowbro,Slowpoke |
| E | SeafoamIslands_B3F (`MAP_SEAFOAM_ISLANDS_B3F`) | land,water,fishing | Seel/Psyduck/Golduck/Zubat/Golbat/Dewgong (Lv24-34, rate 7) | Seel/Horsea/Dewgong/Psyduck/Golduck (Lv25-40, rate 2) | land: FR+Golduck,Psyduck / LG+Slowbro,Slowpoke; water: FR+Golduck,Horsea,Psyduck / LG+Krabby,Slowbro,Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | SeafoamIslands_B4F (`MAP_SEAFOAM_ISLANDS_B4F`) | land,water,fishing | Seel/Psyduck/Golduck/Golbat/Dewgong (Lv26-36, rate 7) | Seel/Horsea/Dewgong/Psyduck/Golduck (Lv25-40, rate 2) | land: FR+Golduck,Psyduck / LG+Slowbro,Slowpoke; water: FR+Golduck,Horsea,Psyduck / LG+Krabby,Slowbro,Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | VermilionCity (`MAP_VERMILION_CITY`) | water,fishing |  | Tentacool (Lv5-40, rate 1) | fishing: FR+Psyduck,Shellder / LG+Slowpoke,Staryu |
| E | VictoryRoad_1F (`MAP_VICTORY_ROAD_1F_FRLG`) | land | Machop/Geodude/Onix/Zubat/Arbok/Golbat/Marowak/Machoke (Lv32-46, rate 7) |  | land: FR+Arbok / LG+Sandslash |
| E | VictoryRoad_2F (`MAP_VICTORY_ROAD_2F`) | land | Machop/Geodude/Primeape/Onix/Zubat/Arbok/Golbat/Marowak/Machoke (Lv34-48, rate 7) |  | land: FR+Arbok / LG+Sandslash |
| E | VictoryRoad_3F (`MAP_VICTORY_ROAD_3F`) | land | Machop/Geodude/Onix/Zubat/Arbok/Golbat/Marowak/Machoke (Lv32-46, rate 7) |  | land: FR+Arbok / LG+Sandslash |
| E | ViridianCity (`MAP_VIRIDIAN_CITY`) | water,fishing |  | Psyduck (Lv20-40, rate 1) | water: FR+Psyduck / LG+Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| E | ViridianForest (`MAP_VIRIDIAN_FOREST`) | land | Caterpie/Weedle/Metapod/Kakuna/Pikachu (Lv3-6, rate 14) |  |  |
| O | CeruleanCave_1F (`MAP_CERULEAN_CAVE_1F`) | land,water,rock,fishing | Magneton/Parasect/Golbat/Machoke/Primeape/Ditto/Electrode/Wobbuffet (Lv46-61, rate 7) | Psyduck/Golduck (Lv30-55, rate 2) | water: FR+Golduck,Psyduck / LG+Slowbro,Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| O | CeruleanCave_2F (`MAP_CERULEAN_CAVE_2F`) | land,rock | Golbat/Machoke/Magneton/Parasect/Kadabra/Ditto/Wobbuffet/Electrode (Lv49-64, rate 7) |  |  |
| O | CeruleanCave_B1F (`MAP_CERULEAN_CAVE_B1F`) | land,water,rock,fishing | Kadabra/Ditto/Magneton/Parasect/Golbat/Machoke/Electrode/Wobbuffet (Lv52-67, rate 7) | Psyduck/Golduck (Lv40-65, rate 2) | water: FR+Golduck,Psyduck / LG+Slowbro,Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| O | DiglettsCave_B1F (`MAP_DIGLETTS_CAVE_B1F`) | land | Diglett/Dugtrio (Lv15-31, rate 5) |  |  |
| O | PowerPlant (`MAP_POWER_PLANT`) | land | Voltorb/Magnemite/Pikachu/Magneton/Electabuzz (Lv22-35, rate 7) |  | land: FR+Electabuzz / LG+ |
| S | FiveIsland (`MAP_FIVE_ISLAND`) | water,fishing |  | Tentacool/Hoppip/Tentacruel (Lv5-40, rate 1) | fishing: FR+Horsea,Psyduck,Seadra,Shellder / LG+Kingler,Krabby,Slowpoke,Staryu |
| S | FiveIsland_LostCave_Room10 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM10`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv22-52, rate 10) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room11 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM11`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv15-52, rate 5) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room12 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM12`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv15-52, rate 5) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room13 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM13`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv15-52, rate 5) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room14 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM14`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv15-52, rate 5) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room1 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM1`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv22-52, rate 1) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room2 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM2`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv22-52, rate 2) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room3 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM3`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv22-52, rate 3) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room4 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM4`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv22-52, rate 4) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room5 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM5`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv22-52, rate 5) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room6 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM6`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv22-52, rate 6) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room7 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM7`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv22-52, rate 7) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room8 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM8`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv22-52, rate 8) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_LostCave_Room9 (`MAP_FIVE_ISLAND_LOST_CAVE_ROOM9`) | land | Gastly/Zubat/Haunter/Golbat/Murkrow (Lv22-52, rate 9) |  | land: FR+Murkrow / LG+Misdreavus |
| S | FiveIsland_Meadow (`MAP_FIVE_ISLAND_MEADOW`) | land,water,fishing | Pidgey/Sentret/Pidgeotto/Hoppip/Meowth/Psyduck/Persian (Lv10-50, rate 21) | Tentacool/Hoppip/Tentacruel (Lv5-40, rate 2) | land: FR+Psyduck / LG+Slowpoke; fishing: FR+Horsea,Psyduck,Qwilfish,Seadra / LG+Kingler,Krabby,Remoraid,Slowpoke |
| S | FiveIsland_MemorialPillar (`MAP_FIVE_ISLAND_MEMORIAL_PILLAR`) | land,water,fishing | Hoppip (Lv6-16, rate 21) | Tentacool/Hoppip/Tentacruel (Lv5-40, rate 2) | fishing: FR+Horsea,Psyduck,Qwilfish,Seadra / LG+Kingler,Krabby,Remoraid,Slowpoke |
| S | FiveIsland_ResortGorgeous (`MAP_FIVE_ISLAND_RESORT_GORGEOUS`) | water,fishing |  | Tentacool/Hoppip/Tentacruel (Lv5-40, rate 2) | fishing: FR+Horsea,Psyduck,Qwilfish,Seadra / LG+Kingler,Krabby,Remoraid,Slowpoke |
| S | FiveIsland_WaterLabyrinth (`MAP_FIVE_ISLAND_WATER_LABYRINTH`) | water,fishing |  | Tentacool/Hoppip/Tentacruel (Lv5-40, rate 2) | fishing: FR+Horsea,Psyduck,Qwilfish,Seadra / LG+Kingler,Krabby,Remoraid,Slowpoke |
| S | FourIsland (`MAP_FOUR_ISLAND`) | water,fishing |  | Wooper/Psyduck (Lv5-35, rate 2) | water: FR+Psyduck,Wooper / LG+Marill,Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| S | FourIsland_IcefallCave_1F (`MAP_FOUR_ISLAND_ICEFALL_CAVE_1F`) | land | Swinub/Golbat/Seel/Zubat/Delibird (Lv23-48, rate 7) |  | land: FR+Delibird / LG+Sneasel |
| S | FourIsland_IcefallCave_B1F (`MAP_FOUR_ISLAND_ICEFALL_CAVE_B1F`) | land | Swinub/Golbat/Seel/Zubat/Delibird (Lv23-48, rate 7) |  | land: FR+Delibird / LG+Sneasel |
| S | FourIsland_IcefallCave_Back (`MAP_FOUR_ISLAND_ICEFALL_CAVE_BACK`) | land,water,fishing | Seel/Golbat/Zubat/Dewgong/Psyduck (Lv40-53, rate 7) | Tentacool/Tentacruel/Lapras (Lv5-45, rate 2) | land: FR+Psyduck / LG+Slowpoke; fishing: FR+Horsea,Psyduck,Seadra,Shellder / LG+Kingler,Krabby,Slowpoke,Staryu |
| S | FourIsland_IcefallCave_Entrance (`MAP_FOUR_ISLAND_ICEFALL_CAVE_ENTRANCE`) | land,water,fishing | Seel/Golbat/Zubat/Dewgong/Psyduck (Lv40-53, rate 7) | Seel/Psyduck/Dewgong/Wooper (Lv5-40, rate 2) | land: FR+Psyduck / LG+Slowpoke; water: FR+Psyduck,Wooper / LG+Marill,Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| S | MtEmber_Exterior (`MAP_MT_EMBER_EXTERIOR`) | land,rock | Ponyta/Fearow/Spearow/Machop/Geodude/Rapidash (Lv30-42, rate 21) |  | land: FR+ / LG+Magmar |
| S | MtEmber_RubyPath_1F (`MAP_MT_EMBER_RUBY_PATH_1F`) | land,rock | Geodude/Machop/Machoke (Lv32-42, rate 7) |  |  |
| S | MtEmber_RubyPath_B1F (`MAP_MT_EMBER_RUBY_PATH_B1F`) | land,rock | Geodude/Slugma (Lv24-42, rate 7) |  |  |
| S | MtEmber_RubyPath_B1F_Stairs (`MAP_MT_EMBER_RUBY_PATH_B1F_STAIRS`) | land,rock | Geodude/Slugma (Lv22-44, rate 7) |  |  |
| S | MtEmber_RubyPath_B2F (`MAP_MT_EMBER_RUBY_PATH_B2F`) | land,rock | Geodude/Slugma (Lv22-44, rate 7) |  |  |
| S | MtEmber_RubyPath_B2F_Stairs (`MAP_MT_EMBER_RUBY_PATH_B2F_STAIRS`) | land,rock | Geodude/Slugma (Lv24-42, rate 7) |  |  |
| S | MtEmber_RubyPath_B3F (`MAP_MT_EMBER_RUBY_PATH_B3F`) | land,rock | Slugma (Lv18-36, rate 7) |  |  |
| S | MtEmber_SummitPath_1F (`MAP_MT_EMBER_SUMMIT_PATH_1F`) | land | Geodude/Machop (Lv29-39, rate 7) |  |  |
| S | MtEmber_SummitPath_2F (`MAP_MT_EMBER_SUMMIT_PATH_2F`) | land,rock | Geodude/Machop/Machoke (Lv30-40, rate 7) |  |  |
| S | MtEmber_SummitPath_3F (`MAP_MT_EMBER_SUMMIT_PATH_3F`) | land | Geodude/Machop (Lv29-39, rate 7) |  |  |
| S | OneIsland (`MAP_ONE_ISLAND`) | water,fishing |  | Tentacool/Tentacruel (Lv5-40, rate 1) | fishing: FR+Horsea,Psyduck,Seadra,Shellder / LG+Kingler,Krabby,Slowpoke,Staryu |
| S | OneIsland_KindleRoad (`MAP_ONE_ISLAND_KINDLE_ROAD`) | land,water,rock,fishing | Spearow/Ponyta/Fearow/Geodude/Meowth/Psyduck/Rapidash/Persian (Lv30-40, rate 21) | Tentacool/Tentacruel (Lv5-40, rate 2) | land: FR+Psyduck / LG+Slowpoke; fishing: FR+Horsea,Psyduck,Seadra / LG+Kingler,Krabby,Slowpoke |
| S | OneIsland_TreasureBeach (`MAP_ONE_ISLAND_TREASURE_BEACH`) | land,water,fishing | Spearow/Tangela/Fearow/Meowth/Psyduck/Persian (Lv31-40, rate 21) | Tentacool/Tentacruel (Lv5-40, rate 2) | land: FR+Psyduck / LG+Slowpoke; fishing: FR+Horsea,Psyduck,Seadra / LG+Kingler,Krabby,Slowpoke |
| S | SevenIsland_SevaultCanyon_Entrance (`MAP_SEVEN_ISLAND_SEVAULT_CANYON_ENTRANCE`) | land | Spearow/Sentret/Phanpy/Fearow/Meowth/Psyduck/Persian (Lv10-50, rate 21) |  | land: FR+Psyduck / LG+Slowpoke |
| S | SevenIsland_SevaultCanyon (`MAP_SEVEN_ISLAND_SEVAULT_CANYON`) | land,rock | Geodude/Phanpy/Cubone/Fearow/Marowak/Meowth/Onix/Skarmory/Larvitar/Persian (Lv15-54, rate 21) |  | land: FR+Skarmory / LG+ |
| S | SevenIsland_TanobyRuins_DilfordChamber (`MAP_SEVEN_ISLAND_TANOBY_RUINS_DILFORD_CHAMBER`) | land | Unown (Lv25-25, rate 7) |  |  |
| S | SevenIsland_TanobyRuins (`MAP_SEVEN_ISLAND_TANOBY_RUINS`) | water,fishing |  | Tentacool/Tentacruel (Lv5-40, rate 2) | water: FR+ / LG+Mantine; fishing: FR+Horsea,Psyduck,Qwilfish,Seadra / LG+Kingler,Krabby,Remoraid,Slowpoke |
| S | SevenIsland_TanobyRuins_LiptooChamber (`MAP_SEVEN_ISLAND_TANOBY_RUINS_LIPTOO_CHAMBER`) | land | Unown (Lv25-25, rate 7) |  |  |
| S | SevenIsland_TanobyRuins_MoneanChamber (`MAP_SEVEN_ISLAND_TANOBY_RUINS_MONEAN_CHAMBER`) | land | Unown (Lv25-25, rate 7) |  |  |
| S | SevenIsland_TanobyRuins_RixyChamber (`MAP_SEVEN_ISLAND_TANOBY_RUINS_RIXY_CHAMBER`) | land | Unown (Lv25-25, rate 7) |  |  |
| S | SevenIsland_TanobyRuins_ScufibChamber (`MAP_SEVEN_ISLAND_TANOBY_RUINS_SCUFIB_CHAMBER`) | land | Unown (Lv25-25, rate 7) |  |  |
| S | SevenIsland_TanobyRuins_ViapoisChamber (`MAP_SEVEN_ISLAND_TANOBY_RUINS_VIAPOIS_CHAMBER`) | land | Unown (Lv25-25, rate 7) |  |  |
| S | SevenIsland_TanobyRuins_WeepthChamber (`MAP_SEVEN_ISLAND_TANOBY_RUINS_WEEPTH_CHAMBER`) | land | Unown (Lv25-25, rate 7) |  |  |
| S | SevenIsland_TrainerTower (`MAP_SEVEN_ISLAND_TRAINER_TOWER`) | water,fishing |  | Tentacool/Tentacruel (Lv5-40, rate 2) | water: FR+ / LG+Mantine; fishing: FR+Horsea,Psyduck,Qwilfish,Seadra / LG+Kingler,Krabby,Remoraid,Slowpoke |
| S | SixIsland_AlteringCave (`MAP_SIX_ISLAND_ALTERING_CAVE`) | land | Smeargle (Lv18-28, rate 5) |  |  |
| S | SixIsland_GreenPath (`MAP_SIX_ISLAND_GREEN_PATH`) | water,fishing |  | Tentacool/Tentacruel (Lv5-40, rate 2) | fishing: FR+Horsea,Psyduck,Qwilfish,Seadra / LG+Kingler,Krabby,Remoraid,Slowpoke |
| S | SixIsland_OutcastIsland (`MAP_SIX_ISLAND_OUTCAST_ISLAND`) | water,fishing |  | Tentacool/Tentacruel (Lv5-40, rate 2) | fishing: FR+Horsea,Psyduck,Qwilfish,Seadra / LG+Kingler,Krabby,Remoraid,Slowpoke |
| S | SixIsland_PatternBush (`MAP_SIX_ISLAND_PATTERN_BUSH`) | land | Spinarak/Kakuna/Caterpie/Weedle/Heracross/Metapod/Ledyba (Lv6-30, rate 21) |  |  |
| S | SixIsland_RuinValley (`MAP_SIX_ISLAND_RUIN_VALLEY`) | land,water,fishing | Natu/Spearow/Yanma/Wooper/Fearow/Meowth/Wobbuffet/Psyduck/Persian (Lv15-52, rate 21) | Wooper (Lv5-25, rate 2) | land: FR+Psyduck,Wooper / LG+Marill,Slowpoke; water: FR+Wooper / LG+Marill; fishing: FR+Psyduck / LG+Slowpoke |
| S | SixIsland_WaterPath (`MAP_SIX_ISLAND_WATER_PATH`) | land,water,fishing | Spearow/Sentret/Oddish/Fearow/Meowth/Gloom/Psyduck/Persian (Lv10-50, rate 21) | Tentacool/Tentacruel (Lv5-40, rate 2) | land: FR+Gloom,Oddish,Psyduck / LG+Bellsprout,Slowpoke,Weepinbell; fishing: FR+Horsea,Psyduck,Qwilfish,Seadra / LG+Kingler,Krabby,Remoraid,Slowpoke |
| S | ThreeIsland_BerryForest (`MAP_THREE_ISLAND_BERRY_FOREST`) | land,water,fishing | Pidgeotto/Gloom/Pidgey/Oddish/Venonat/Drowzee/Exeggcute/Psyduck/Venomoth/Hypno (Lv30-40, rate 21) | Psyduck/Golduck (Lv5-40, rate 2) | land: FR+Gloom,Oddish,Psyduck / LG+Bellsprout,Slowpoke,Weepinbell; water: FR+Golduck,Psyduck / LG+Slowbro,Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
| S | ThreeIsland_BondBridge (`MAP_THREE_ISLAND_BOND_BRIDGE`) | land,water,fishing | Pidgey/Oddish/Gloom/Pidgeotto/Meowth/Venonat/Psyduck/Persian (Lv29-40, rate 21) | Tentacool/Tentacruel (Lv5-40, rate 2) | land: FR+Gloom,Oddish,Psyduck / LG+Bellsprout,Slowpoke,Weepinbell; fishing: FR+Horsea,Psyduck,Seadra / LG+Kingler,Krabby,Slowpoke |
| S | ThreeIsland_Port (`MAP_THREE_ISLAND_PORT`) | land | Dunsparce (Lv5-35, rate 1) |  |  |
| S | TwoIsland_CapeBrink (`MAP_TWO_ISLAND_CAPE_BRINK`) | land,water,fishing | Spearow/Oddish/Gloom/Fearow/Meowth/Psyduck/Golduck/Persian (Lv30-40, rate 21) | Psyduck/Golduck (Lv5-40, rate 2) | land: FR+Gloom,Golduck,Oddish,Psyduck / LG+Bellsprout,Slowbro,Slowpoke,Weepinbell; water: FR+Golduck,Psyduck / LG+Slowbro,Slowpoke; fishing: FR+Psyduck / LG+Slowpoke |
Counts: 61 tables in essential areas, 5 in optional areas (Cerulean Cave ×3, Diglett's Cave B1F, Power Plant), 58 in Sevii.
Every essential dungeon has a table: Viridian Forest, Mt. Moon ×3, Rock Tunnel ×2, Pokémon Tower 3F-7F, Safari Zone ×4, Seafoam ×5, Pokémon Mansion ×4, Victory Road ×3, Cerulean Cave ×3, Power Plant.

---

## 8. FRLG trainer party file — `src/data/trainers_frlg.party`

* Format: the same trainerproc `.party` format as `src/data/trainers.party`. It is compiled to `src/data/trainers_frlg.h` (rule in `trainer_rules.mk`) and included by `src/data.c` **only** when `IS_FRLG`.
* 624 entries (`TRAINER_NONE` + 623). Class names use `* Frlg` classes (`Leader Frlg`, `Elite Four Frlg`, `Champion Frlg`, `Youngster Frlg`…). Pics are `* Frlg`.
* All IVs are 0. AI flags: `Check Bad Move / Try To Faint / Check Viability` for leaders/E4; route trainers mostly `Check Bad Move`.
* The rival's default name in the party file is **TERRY**. Rival/champion entries come in 3 versions keyed on the player's starter. The suffix is the **rival's** starter, e.g. `_SQUIRTLE` = player picked Charmander.

### 8.1 Gym leaders

| Leader | Const | Items | Team (species level) |
|---|---|---|---|
| Brock | `TRAINER_LEADER_BROCK` | — | Geodude 12, Onix 14 |
| Misty | `TRAINER_LEADER_MISTY` | Super Potion | Staryu 18, Starmie 21 |
| Lt. Surge | `TRAINER_LEADER_LT_SURGE` | Super Potion, Full Heal | Voltorb 21, Pikachu 18, Raichu 24 |
| Erika | `TRAINER_LEADER_ERIKA` | Hyper Potion, Full Heal | Victreebel 29, Tangela 24, Vileplume 29 |
| Koga | `TRAINER_LEADER_KOGA` | Hyper Potion ×2, Full Heal | Koffing 37, Muk 39, Koffing 37, Weezing 43 |
| Sabrina | `TRAINER_LEADER_SABRINA` | Hyper Potion ×2, Full Heal | Kadabra 38, Mr. Mime 37, Venomoth 38, Alakazam 43 |
| Blaine | `TRAINER_LEADER_BLAINE` | Hyper Potion ×2, Full Heal | Growlithe 42, Ponyta 40, Rapidash 42, Arcanine 47 |
| Giovanni | `TRAINER_LEADER_GIOVANNI` | Hyper Potion ×2, Full Heal | Rhyhorn 45, Dugtrio 42, Nidoqueen 44, Nidoking 45, **Rhyhorn 50** (the source says Rhyhorn, not Rhydon; check before rebalancing) |

There are no gym-leader rematch entries in the FRLG file.

### 8.2 Elite Four and Champion

| Trainer | Const | Team (first run) | Rematch const → team |
|---|---|---|---|
| Lorelei | `TRAINER_ELITE_FOUR_LORELEI` | Dewgong 52, Cloyster 51, Slowbro 52, Jynx 54, Lapras 54 @Sitrus | `_2`: Dewgong 64, Cloyster 63, Piloswine 63, Jynx 66, Lapras 66 |
| Bruno | `TRAINER_ELITE_FOUR_BRUNO` | Onix 51, Hitmonchan 53, Hitmonlee 53, Onix 54, Machamp 56 @Sitrus | `_2`: Steelix 65, Hitmonchan 65, Hitmonlee 65, Steelix 66, Machamp 68 |
| Agatha | `TRAINER_ELITE_FOUR_AGATHA` | Gengar 54, Golbat 54, Haunter 53, Arbok 56, Gengar 58 @Sitrus | `_2`: Gengar 66, Crobat 66, Misdreavus 65, Arbok 68, Gengar 70 |
| Lance | `TRAINER_ELITE_FOUR_LANCE` | Gyarados 56, Dragonair 54, Dragonair 54, Aerodactyl 58, Dragonite 60 @Sitrus | `_2`: Gyarados 68, Dragonite 66, Kingdra 66, Aerodactyl 70, Dragonite 72 |
| Champion (rival) | `TRAINER_CHAMPION_FIRST_SQUIRTLE` | Pidgeot 59, Alakazam 57, Rhydon 59, Arcanine 59, Exeggutor 61, Blastoise 63 | `TRAINER_CHAMPION_REMATCH_SQUIRTLE`: Heracross 72, Alakazam 73, Tyranitar 72, Arcanine 73, Exeggutor 73, Blastoise 75 |
| | `…_FIRST_BULBASAUR` | Pidgeot 59, Alakazam 57, Rhydon 59, Gyarados 59, Arcanine 61, Venusaur 63 | `…_REMATCH_BULBASAUR` |
| | `…_FIRST_CHARMANDER` | Pidgeot 59, Alakazam 57, Rhydon 59, Exeggutor 59, Gyarados 61, Charizard 63 | `…_REMATCH_CHARMANDER` |

### 8.3 Story bosses (rival chain + Rocket)

| Battle | Const (Squirtle variant shown) | Map | Team |
|---|---|---|---|
| Rival 1 | `TRAINER_RIVAL_OAKS_LAB_SQUIRTLE` | PalletTown_ProfessorOaksLab | Squirtle 5 (`trainerbattle_earlyrival`) |
| Rival 2 | `TRAINER_RIVAL_ROUTE22_EARLY_SQUIRTLE` | Route22 | Pidgey 9, Squirtle 9 |
| Rival 3 | `TRAINER_RIVAL_CERULEAN_SQUIRTLE` | CeruleanCity | Pidgeotto 17, Abra 16, Rattata 15, Squirtle 18 |
| Rival 4 | `TRAINER_RIVAL_SS_ANNE_SQUIRTLE` | SSAnne_2F_Corridor | Pidgeotto 19, Raticate 16, Kadabra 18, Wartortle 20 |
| Rival 5 | `TRAINER_RIVAL_POKEMON_TOWER_SQUIRTLE` | PokemonTower_2F | Pidgeotto 25, Growlithe 23, Exeggcute 22, Kadabra 20, Wartortle 25 |
| Rival 6 | `TRAINER_RIVAL_SILPH_SQUIRTLE` | SilphCo_7F | Pidgeot 37, Growlithe 38, Exeggcute 35, Alakazam 35, Blastoise 40 |
| Rival 7 | `TRAINER_RIVAL_ROUTE22_LATE_SQUIRTLE` | Route22 (after Gym 8) | Pidgeot 47, Rhyhorn 45, Growlithe 45, Exeggcute 45, Alakazam 47, Blastoise 53 |
| Boss 1 | `TRAINER_BOSS_GIOVANNI` | RocketHideout_B4F | Onix 25, Rhyhorn 24, Kangaskhan 29 |
| Boss 2 | `TRAINER_BOSS_GIOVANNI_2` | SilphCo_11F | Nidorino 37, Kangaskhan 35, Rhyhorn 37, Nidoqueen 41 |
| Boss 3 | `TRAINER_LEADER_GIOVANNI` | ViridianCity_Gym | see §8.1 |
| (Sevii) | `TRAINER_TEAM_ROCKET_ADMIN` / `_2` | FiveIsland_RocketWarehouse | Muk 52, Arbok 53, Vileplume 54 / Golbat 53, Weezing 54, Houndoom 55 |

Rocket grunt battles (`TRAINER_TEAM_ROCKET_GRUNT…`) by map: Mt. Moon B2F 4, Cerulean City 1, Route 24 1, Celadon Game Corner 1, Rocket Hideout 11 (B1F 5, B2F 1, B3F 2, B4F 3), Pokémon Tower 7F 3, Silph Co. 21 (2F-11F). Sevii adds Five Island Meadow 3 + Rocket Warehouse 5, Mt. Ember 2, Icefall Cave 1, Outcast Island 1. The 8 Rocket NPCs in `SaffronCity_Frlg` are not trainers; they are hidden by `FLAG_HIDE_SAFFRON_ROCKETS`.

---

## 9. The "Kanto act": what to keep and what to skip

### 9.1 Essential (keep): 248 maps, 375 trainers

Order of the forced path (same gates as FRLG):

1. **Pallet** (starter, rival 1).
2. **Route 1 → Viridian** (Oak's Parcel → back to Pallet → Pokédex). Old Man tutorial.
3. **Route 22** (rival 2).
4. **Route 2 → Viridian Forest → Pewter** (Gym 1).
5. **Route 3 → Mt. Moon** (fossil choice) **→ Route 4 → Cerulean** (rival 3, Gym 2).
6. **Route 24/25** (Nugget Bridge; Bill → S.S. Ticket).
7. **Route 5 → Underground Path N-S → Route 6 → Vermilion**.
8. **S.S. Anne** (rival 4, HM01 Cut) → Cut tree → Gym 3.
9. **Route 11 → Route 9/10 → Rock Tunnel** (Flash optional; Route 2 aide gives HM05) **→ Lavender**.
10. **Route 8/7 → Celadon**: Gym 4, Tea from the Condominiums, Game Corner → **Rocket Hideout** (Boss 1, Silph Scope).
11. **Pokémon Tower** (rival 5 on 2F, Marowak ghost, Mr. Fuji → Poké Flute).
12. Tea opens the Saffron gates → **Silph Co.** (Card Key 5F, rival 6 on 7F, Lapras, Boss 2 on 11F, Master Ball) → Saffron Gym 6 (unlocked by `FLAG_HIDE_SAFFRON_ROCKETS`).
13. Snorlax (Route 12 or Route 16) → **Routes 12-15** or **Routes 16-18** (HM02 Fly at the Route 16 house) → **Fuchsia**: Gym 5, **Safari Zone** (HM03 Surf, Gold Teeth → Warden → HM04 Strength).
14. **Routes 19/20 → Seafoam** (or Pallet → Route 21) → **Cinnabar**.
15. **Pokémon Mansion** (Secret Key) → Gym 7 (quiz).
16. Bill's Sevii offer (declinable) → **Viridian Gym 8** (needs badges 2-7) → Route 22 (rival 7) → **Route 23** (badge guards) → **Victory Road** → **Indigo Plateau** → E4 → Champion → Hall of Fame → credits.

Interiors kept in essential areas include optional rooms whose cost is mostly text: houses, marts, gatehouse 2F aides, Fan Clubs, Copycat, Dojo, Museum, Game Corner, Hotel. They can be trimmed later. The ones with FRLG-only specials are listed per area in §3.

### 9.2 Key optional (recommended to keep)

* **Power Plant** (`MAP_POWER_PLANT`, 1 map): Zapdos Lv50 + Electrode traps.
* **Cerulean Cave** (3 maps): Mewtwo Lv70. In FRLG the guard (`FLAG_HIDE_CERULEAN_CAVE_GUARD`) is removed only at the end of the Sevii postgame (`OneIsland_PokemonCenter_1F_Frlg`, Sapphire delivery: also `special SetPostgameFlags`, `FLAG_IS_CHAMPION`, `special InitRoamer`). **If Sevii is skipped, the hack must set this flag elsewhere**, e.g. after the Kanto Hall of Fame.
* Articuno is inside the essential Seafoam Islands. Moltres is on Mt. Ember (Sevii, skipped). Re-home Moltres if all three birds are wanted.
* **Diglett's Cave** (3 maps): shortcut only.

### 9.3 Skip: 159 Sevii/event maps + 5 link rooms

* **Sevii Islands 1-7** (154 maps incl. Mt. Ember 13, Lost Cave 15, Tanoby 9, Dotted Hole 6, Icefall Cave 4, Trainer Tower 11, Rocket Warehouse; 99 trainers, 58 wild tables). Costs: Seagallop ferry, Trainer Tower save-block data, braille, Lorelei's dolls, Move Relearner (`TwoIsland_House`), Cape Brink tutor. Hoenn already provides HM06 Rock Smash / HM07 Waterfall (given in Sevii in FRLG) and a Move Relearner.
* **Navel Rock `_Frlg` (22) and Birth Island `_Frlg` (2)**: duplicates of Emerald event islands.
* **Link rooms `_Frlg` (5)**: retarget the 38 Pokémon Center 2F warps to the Emerald link rooms.
* **Unused houses**: `Route6_UnusedHouse_Frlg` and `Route19_UnusedHouse_Frlg` (no script content).

### 9.4 Things to resolve if Sevii is skipped

* Cinnabar `VAR_MAP_SCENE_CINNABAR_ISLAND` = 1 (set by Blaine) triggers Bill's "come to One Island" scene. It can be declined, but it needs new text or removal.
* Postgame unlocks tied to One Island: Cerulean Cave guard, `FLAG_IS_CHAMPION`, `SetPostgameFlags`, roamer init. National Pokédex checks (`IsNationalPokedexEnabled` at the Indigo lobby).
* The E4 and Champion rematch **teams** (`_2`, `CHAMPION_REMATCH_*`) are chosen by `FLAG_IS_CHAMPION`, which is set only at One Island. The Champion's rematch **intro text** is chosen by `FLAG_SYS_GAME_CLEAR`. In a merged save that flag would be shared with Hoenn, so give Kanto its own "Kanto cleared" flag.
