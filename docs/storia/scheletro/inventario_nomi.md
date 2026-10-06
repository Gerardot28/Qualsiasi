# Names inventory — Hoenn (Emerald build)

Source: pokeemerald-expansion 1.17.1, read-only copy at `/home/user/pex-orig`. Every path below is relative to that root unless it starts with `/`.

This is the list of **every name the story rewrite may want to change**: places, trainers, trainer classes, speaking characters, story items and the names hard-coded in C. For each one it gives the exact source location and the length limit. It covers the **Hoenn / Emerald build** only. Folders ending in `_Frlg` and files with `frlg` in their name belong to the Kanto build and are left out, except where a table is shared by both builds (then the Kanto rows are listed separately).

How to use it together with the segment skeletons (`segmento_A.md`, `segmento_B.md`, `segmento_D.md`): the skeletons say *what happens*; this file says *what everything is called and where the name lives*. Renaming a name is a text change only. It never changes a constant (`MAPSEC_*`, `TRAINER_*`, `TRAINER_CLASS_*`, `ITEM_*`, `FLAG_*`), so scripts and logic stay as they are.

## 0. Conventions

* **len** = bytes after charmap encoding (`charmap.txt`). One letter, digit, space or punctuation mark = 1 byte. `é` = 1 byte. `{PKMN}` = 2 bytes (glyphs PK+MN). `{POKEBLOCK}` = 5 bytes. A text placeholder such as `{PLAYER}` or `{AQUA}` = 2 bytes in the source, but it is replaced at run time.
* **Hoenn files** = `data/maps/<Map>/scripts.inc` for the 521 map folders that do not end in `_Frlg`, plus `data/scripts/*.inc` and `data/text/*.inc` without `frlg` in the file name, plus the 101 `.string` lines of `data/event_scripts.s` (shared texts such as the white-out messages; counted in 4.1, and only `MOM` occurs there among the names of 4.2–4.3).
* **IT (Smeraldo)** = the name used by the official Italian Emerald/FRLG, from `/home/user/work/pokeapi_it/mapsec_hoenn_kanto.json` (field `it_ingame`) and `/home/user/work/pokeapi_it/regioni_personaggi.json` (see `docs/riferimenti/regioni.md`). A name in *italics* has no in-game source and comes from the PKHeX/PokeAPI title-case name, upper-cased by me.
* **(n)** after a name = number of entries (for trainers this includes rematch copies `_2`…`_5`; for texts it is the number of occurrences).
* Text-width rules for dialogue (216 px field box, 208 px battle box, 192 px PokéNav) are in `/home/user/Qualsiasi/tools/textcheck/README.md`. This file deals only with **names**.

## 1. Place names

### 1.1 Map sections — `src/data/region_map/region_map_sections.json`

* **Limit: `MAP_NAME_LENGTH` = 16** (`include/region_map.h:7`). The region map and the Fly map print the name in a 16-character field (`src/region_map.c:779, 922, 999, 1994`); the buffer is `u8 mapSecName[20]` (`include/region_map.h:53`). The map-name pop-up uses a 27-byte buffer (`include/map_name_popup.h:13`), so 16 is the binding limit.
* The JSON is turned into `gRegionMapEntries[]` by `jsonproc` (template `src/data/region_map/region_map_sections.json.txt`; each `name` becomes `COMPOUND_STRING("…")`). Edit the `"name"` field only, never `"id"`, `x`, `y`, `width`, `height`.
* Lookup: `GetMapName()` (`src/region_map.c:1866`). Special cases: `MAPSEC_SECRET_BASE` prints "<owner>`gText_ApostropheSBase`" (`'s BASE`, `src/strings.c:349`); `MAPSEC_DYNAMIC` has no name and prints `gText_Ferry` "FERRY" (`src/strings.c:573`) through `GetMapNameGeneric()` (`src/region_map.c:1899`); `MAPSEC_AQUA_HIDEOUT_OLD` prints `gText_Hideout` "HIDEOUT" (`src/strings.c:575`) through `GetMapNameHandleAquaHideout()` (`src/region_map.c:1912`).
* `"name_clone": true` marks a second section that repeats another section's name (Kanto only).

**Hoenn sections (ids 0–87 and the Emerald additions 193–208).** "Hoenn maps" = number of `data/maps/*/map.json` (non-`_Frlg`) whose `region_map_section` is that id; 0 means the name is only reachable from the region map grid or not at all.

| # | `MAPSEC_…` | Name (EN) | len | IT (Smeraldo) | len | Hoenn maps |
|---|---|---|---|---|---|---|
| 0 | `LITTLEROOT_TOWN` | LITTLEROOT TOWN | 15 | ALBANOVA | 8 | 6 |
| 1 | `OLDALE_TOWN` | OLDALE TOWN | 11 | SOLAROSA | 8 | 6 |
| 2 | `DEWFORD_TOWN` | DEWFORD TOWN | 12 | BLURUVIA | 8 | 7 |
| 3 | `LAVARIDGE_TOWN` | LAVARIDGE TOWN | 14 | CUORDILAVA | 10 | 8 |
| 4 | `FALLARBOR_TOWN` | FALLARBOR TOWN | 14 | BRUNIFOGLIA | 11 | 9 |
| 5 | `VERDANTURF_TOWN` | VERDANTURF TOWN | 15 | MENTANIA | 8 | 10 |
| 6 | `PACIFIDLOG_TOWN` | PACIFIDLOG TOWN | 15 | OROCEA | 6 | 8 |
| 7 | `PETALBURG_CITY` | PETALBURG CITY | 14 | PETALIPOLI | 10 | 8 |
| 8 | `SLATEPORT_CITY` | SLATEPORT CITY | 14 | PORTO SELCEPOLI | 15 | 15 |
| 9 | `MAUVILLE_CITY` | MAUVILLE CITY | 13 | CICLAMIPOLI | 11 | 9 |
| 10 | `RUSTBORO_CITY` | RUSTBORO CITY | 13 | FERRUGIPOLI | 11 | 18 |
| 11 | `FORTREE_CITY` | FORTREE CITY | 12 | FORESTOPOLI | 11 | 11 |
| 12 | `LILYCOVE_CITY` | LILYCOVE CITY | 13 | PORTO ALGHEPOLI | 15 | 24 |
| 13 | `MOSSDEEP_CITY` | MOSSDEEP CITY | 13 | VERDEAZZUPOLI | 13 | 14 |
| 14 | `SOOTOPOLIS_CITY` | SOOTOPOLIS CITY | 15 | CENERIDE | 8 | 16 |
| 15 | `EVER_GRANDE_CITY` | EVER GRANDE CITY | 16 | IRIDOPOLI | 9 | 16 |
| 16 | `ROUTE_101` | ROUTE 101 | 9 | PERCORSO 101 | 12 | 1 |
| 17 | `ROUTE_102` | ROUTE 102 | 9 | PERCORSO 102 | 12 | 1 |
| 18 | `ROUTE_103` | ROUTE 103 | 9 | PERCORSO 103 | 12 | 1 |
| 19 | `ROUTE_104` | ROUTE 104 | 9 | PERCORSO 104 | 12 | 5 |
| 20 | `ROUTE_105` | ROUTE 105 | 9 | PERCORSO 105 | 12 | 1 |
| 21 | `ROUTE_106` | ROUTE 106 | 9 | PERCORSO 106 | 12 | 1 |
| 22 | `ROUTE_107` | ROUTE 107 | 9 | PERCORSO 107 | 12 | 1 |
| 23 | `ROUTE_108` | ROUTE 108 | 9 | PERCORSO 108 | 12 | 1 |
| 24 | `ROUTE_109` | ROUTE 109 | 9 | PERCORSO 109 | 12 | 2 |
| 25 | `ROUTE_110` | ROUTE 110 | 9 | PERCORSO 110 | 12 | 14 |
| 26 | `ROUTE_111` | ROUTE 111 | 9 | PERCORSO 111 | 12 | 3 |
| 27 | `ROUTE_112` | ROUTE 112 | 9 | PERCORSO 112 | 12 | 2 |
| 28 | `ROUTE_113` | ROUTE 113 | 9 | PERCORSO 113 | 12 | 2 |
| 29 | `ROUTE_114` | ROUTE 114 | 9 | PERCORSO 114 | 12 | 4 |
| 30 | `ROUTE_115` | ROUTE 115 | 9 | PERCORSO 115 | 12 | 1 |
| 31 | `ROUTE_116` | ROUTE 116 | 9 | PERCORSO 116 | 12 | 2 |
| 32 | `ROUTE_117` | ROUTE 117 | 9 | PERCORSO 117 | 12 | 2 |
| 33 | `ROUTE_118` | ROUTE 118 | 9 | PERCORSO 118 | 12 | 1 |
| 34 | `ROUTE_119` | ROUTE 119 | 9 | PERCORSO 119 | 12 | 4 |
| 35 | `ROUTE_120` | ROUTE 120 | 9 | PERCORSO 120 | 12 | 1 |
| 36 | `ROUTE_121` | ROUTE 121 | 9 | PERCORSO 121 | 12 | 2 |
| 37 | `ROUTE_122` | ROUTE 122 | 9 | PERCORSO 122 | 12 | 1 |
| 38 | `ROUTE_123` | ROUTE 123 | 9 | PERCORSO 123 | 12 | 2 |
| 39 | `ROUTE_124` | ROUTE 124 | 9 | PERCORSO 124 | 12 | 2 |
| 40 | `ROUTE_125` | ROUTE 125 | 9 | PERCORSO 125 | 12 | 1 |
| 41 | `ROUTE_126` | ROUTE 126 | 9 | PERCORSO 126 | 12 | 1 |
| 42 | `ROUTE_127` | ROUTE 127 | 9 | PERCORSO 127 | 12 | 1 |
| 43 | `ROUTE_128` | ROUTE 128 | 9 | PERCORSO 128 | 12 | 1 |
| 44 | `ROUTE_129` | ROUTE 129 | 9 | PERCORSO 129 | 12 | 1 |
| 45 | `ROUTE_130` | ROUTE 130 | 9 | PERCORSO 130 | 12 | 1 |
| 46 | `ROUTE_131` | ROUTE 131 | 9 | PERCORSO 131 | 12 | 1 |
| 47 | `ROUTE_132` | ROUTE 132 | 9 | PERCORSO 132 | 12 | 1 |
| 48 | `ROUTE_133` | ROUTE 133 | 9 | PERCORSO 133 | 12 | 1 |
| 49 | `ROUTE_134` | ROUTE 134 | 9 | PERCORSO 134 | 12 | 1 |
| 50 | `UNDERWATER_124` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 1 |
| 51 | `UNDERWATER_126` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 1 |
| 52 | `UNDERWATER_127` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 1 |
| 53 | `UNDERWATER_128` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 1 |
| 54 | `UNDERWATER_SOOTOPOLIS` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 1 |
| 55 | `GRANITE_CAVE` | GRANITE CAVE | 12 | GROTTA PIETROSA | 15 | 4 |
| 56 | `MT_CHIMNEY` | MT. CHIMNEY | 11 | MONTE CAMINO | 12 | 2 |
| 57 | `SAFARI_ZONE` | SAFARI ZONE | 11 | ZONA SAFARI | 11 | 7 |
| 58 | `BATTLE_FRONTIER` | BATTLE FRONTIER | 15 | PARCO LOTTA | 11 | 47 |
| 59 | `PETALBURG_WOODS` | PETALBURG WOODS | 15 | BOSCO PETALO | 12 | 1 |
| 60 | `RUSTURF_TUNNEL` | RUSTURF TUNNEL | 14 | TUNNEL MENFERRO | 15 | 1 |
| 61 | `ABANDONED_SHIP` | ABANDONED SHIP | 14 | VECCHIA NAVE | 12 | 13 |
| 62 | `NEW_MAUVILLE` | NEW MAUVILLE | 12 | CICLANOVA | 9 | 2 |
| 63 | `METEOR_FALLS` | METEOR FALLS | 12 | CASCATE METEORA | 15 | 5 |
| 64 | `METEOR_FALLS2` | METEOR FALLS | 12 | CASCATE METEORA | 15 | 0 |
| 65 | `MT_PYRE` | MT. PYRE | 8 | MONTE PIRA | 10 | 8 |
| 66 | `AQUA_HIDEOUT_OLD` | {AQUA} HIDEOUT | 10 | *RIFUGIO* | 7 | 0 |
| 67 | `SHOAL_CAVE` | SHOAL CAVE | 10 | GROTTA ONDOSA | 13 | 7 |
| 68 | `SEAFLOOR_CAVERN` | SEAFLOOR CAVERN | 15 | ANTRO ABISSALE | 14 | 10 |
| 69 | `UNDERWATER_SEAFLOOR_CAVERN` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 1 |
| 70 | `VICTORY_ROAD` | VICTORY ROAD | 12 | VIA VITTORIA | 12 | 3 |
| 71 | `MIRAGE_ISLAND` | MIRAGE ISLAND | 13 | ISOLA MIRAGGIO | 14 | 0 |
| 72 | `CAVE_OF_ORIGIN` | CAVE OF ORIGIN | 14 | GROTTA dei TEMPI | 16 | 6 |
| 73 | `SOUTHERN_ISLAND` | SOUTHERN ISLAND | 15 | ISOLA REMOTA | 12 | 2 |
| 74 | `FIERY_PATH` | FIERY PATH | 10 | CAMMINO ARDENTE | 15 | 1 |
| 75 | `FIERY_PATH2` | FIERY PATH | 10 | CAMMINO ARDENTE | 15 | 0 |
| 76 | `JAGGED_PASS` | JAGGED PASS | 11 | PASSO SELVAGGIO | 15 | 1 |
| 77 | `JAGGED_PASS2` | JAGGED PASS | 11 | PASSO SELVAGGIO | 15 | 0 |
| 78 | `SEALED_CHAMBER` | SEALED CHAMBER | 14 | SALA INCISA | 11 | 2 |
| 79 | `UNDERWATER_SEALED_CHAMBER` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 2 |
| 80 | `SCORCHED_SLAB` | SCORCHED SLAB | 13 | GROTTINO SOLARE | 15 | 1 |
| 81 | `ISLAND_CAVE` | ISLAND CAVE | 11 | GROTTA INSULARE | 15 | 1 |
| 82 | `DESERT_RUINS` | DESERT RUINS | 12 | ROVINE SABBIOSE | 15 | 1 |
| 83 | `ANCIENT_TOMB` | ANCIENT TOMB | 12 | TOMBA ANTICA | 12 | 1 |
| 84 | `INSIDE_OF_TRUCK` | INSIDE OF TRUCK | 15 | NEL CAMION | 10 | 1 |
| 85 | `SKY_PILLAR` | SKY PILLAR | 10 | TORRE dei CIELI | 15 | 8 |
| 86 | `SECRET_BASE` | SECRET BASE | 11 | BASE SEGRETA | 12 | 24 |
| 87 | `DYNAMIC` | *(none)* | 0 | *TRAGHETTO* | 9 | 36 |
| 193 | `AQUA_HIDEOUT` | AQUA HIDEOUT | 12 | RIFUGIO IDRO | 12 | 6 |
| 194 | `MAGMA_HIDEOUT` | MAGMA HIDEOUT | 13 | RIFUGIO MAGMA | 13 | 8 |
| 195 | `MIRAGE_TOWER` | MIRAGE TOWER | 12 | TORRE MIRAGGIO | 14 | 4 |
| 196 | `BIRTH_ISLAND` | BIRTH ISLAND | 12 | ISOLA MATERNA | 13 | 2 |
| 197 | `FARAWAY_ISLAND` | FARAWAY ISLAND | 14 | ISOLA SUPREMA | 13 | 2 |
| 198 | `ARTISAN_CAVE` | ARTISAN CAVE | 12 | GROTTA ARTISTICA | 16 | 2 |
| 199 | `MARINE_CAVE` | MARINE CAVE | 11 | GROTTA MARE | 11 | 2 |
| 200 | `UNDERWATER_MARINE_CAVE` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 1 |
| 201 | `TERRA_CAVE` | TERRA CAVE | 10 | GROTTA TERRA | 12 | 2 |
| 202 | `UNDERWATER_105` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 1 |
| 203 | `UNDERWATER_125` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 1 |
| 204 | `UNDERWATER_129` | UNDERWATER | 10 | SOTT'ACQUA | 10 | 1 |
| 205 | `DESERT_UNDERPASS` | DESERT UNDERPASS | 16 | GALLERIA DESERTO | 16 | 1 |
| 206 | `ALTERING_CAVE` | ALTERING CAVE | 13 | GROTTA MUTEVOLE | 15 | 1 |
| 207 | `NAVEL_ROCK` | NAVEL ROCK | 10 | MONTE CORDONE | 13 | 22 |
| 208 | `TRAINER_HILL` | TRAINER HILL | 12 | MONTE ALLENATORI | 16 | 7 |

Notes on the Hoenn sections:

* Duplicated names: `UNDERWATER` ×9 (ids 50–54, 69, 79, 200, 202–204), `METEOR FALLS` ×2, `FIERY PATH` ×2, `JAGGED PASS` ×2. Each copy has its own `"name"` field, so a rename must be applied to every copy.
* `{AQUA} HIDEOUT` (id 66, `MAPSEC_AQUA_HIDEOUT_OLD`) holds the `{AQUA}` placeholder (charmap `FD 08`). No Hoenn map uses this id; the real hideout maps use `MAPSEC_AQUA_HIDEOUT` (id 193, "AQUA HIDEOUT", 6 maps) and `MAPSEC_MAGMA_HIDEOUT` (id 194, "MAGMA HIDEOUT", 8 maps). These two literal names must be renamed by hand for the new villain organisation.
* `INSIDE OF TRUCK` (id 84) is the intro truck. `SECRET BASE` (86) is only a fallback, see above.
* The longest vanilla names already use the full 16 bytes: `EVER GRANDE CITY`, `DESERT UNDERPASS`. All official Italian names fit (longest: `GALLERIA DESERTO`, `MONTE ALLENATORI`, 16).

**Kanto / Sevii sections (ids 88–192), shared table, used by the FRLG build.** Listed for completeness because they live in the same JSON and the multi-region plan will reuse them.

| # | `MAPSEC_…` | Name (EN) | len | IT (Smeraldo) | len |
|---|---|---|---|---|---|
| 88 | `PALLET_TOWN` | PALLET TOWN | 11 | BIANCAVILLA | 11 |
| 89 | `VIRIDIAN_CITY` | VIRIDIAN CITY | 13 | SMERALDOPOLI | 12 |
| 90 | `PEWTER_CITY` | PEWTER CITY | 11 | PLUMBEOPOLI | 11 |
| 91 | `CERULEAN_CITY` | CERULEAN CITY | 13 | CELESTOPOLI | 11 |
| 92 | `LAVENDER_TOWN` | LAVENDER TOWN | 13 | LAVANDONIA | 10 |
| 93 | `VERMILION_CITY` | VERMILION CITY | 14 | ARANCIOPOLI | 11 |
| 94 | `CELADON_CITY` | CELADON CITY | 12 | AZZURROPOLI | 11 |
| 95 | `FUCHSIA_CITY` | FUCHSIA CITY | 12 | FUCSIAPOLI | 10 |
| 96 | `CINNABAR_ISLAND` | CINNABAR ISLAND | 15 | ISOLA CANNELLA | 14 |
| 97 | `INDIGO_PLATEAU` | INDIGO PLATEAU | 14 | ALTOPIANO BLU | 13 |
| 98 | `SAFFRON_CITY` | SAFFRON CITY | 12 | ZAFFERANOPOLI | 13 |
| 99 | `ROUTE_4_POKECENTER` | ROUTE 4 (name_clone) | 7 | PERCORSO 4 | 10 |
| 100 | `ROUTE_10_POKECENTER` | ROUTE 10 (name_clone) | 8 | PERCORSO 10 | 11 |
| 101 | `ROUTE_1` | ROUTE 1 | 7 | PERCORSO 1 | 10 |
| 102 | `ROUTE_2` | ROUTE 2 | 7 | PERCORSO 2 | 10 |
| 103 | `ROUTE_3` | ROUTE 3 | 7 | PERCORSO 3 | 10 |
| 104 | `ROUTE_4` | ROUTE 4 | 7 | PERCORSO 4 | 10 |
| 105 | `ROUTE_5` | ROUTE 5 | 7 | PERCORSO 5 | 10 |
| 106 | `ROUTE_6` | ROUTE 6 | 7 | PERCORSO 6 | 10 |
| 107 | `ROUTE_7` | ROUTE 7 | 7 | PERCORSO 7 | 10 |
| 108 | `ROUTE_8` | ROUTE 8 | 7 | PERCORSO 8 | 10 |
| 109 | `ROUTE_9` | ROUTE 9 | 7 | PERCORSO 9 | 10 |
| 110 | `ROUTE_10` | ROUTE 10 | 8 | PERCORSO 10 | 11 |
| 111 | `ROUTE_11` | ROUTE 11 | 8 | PERCORSO 11 | 11 |
| 112 | `ROUTE_12` | ROUTE 12 | 8 | PERCORSO 12 | 11 |
| 113 | `ROUTE_13` | ROUTE 13 | 8 | PERCORSO 13 | 11 |
| 114 | `ROUTE_14` | ROUTE 14 | 8 | PERCORSO 14 | 11 |
| 115 | `ROUTE_15` | ROUTE 15 | 8 | PERCORSO 15 | 11 |
| 116 | `ROUTE_16` | ROUTE 16 | 8 | PERCORSO 16 | 11 |
| 117 | `ROUTE_17` | ROUTE 17 | 8 | PERCORSO 17 | 11 |
| 118 | `ROUTE_18` | ROUTE 18 | 8 | PERCORSO 18 | 11 |
| 119 | `ROUTE_19` | ROUTE 19 | 8 | PERCORSO 19 | 11 |
| 120 | `ROUTE_20` | ROUTE 20 | 8 | PERCORSO 20 | 11 |
| 121 | `ROUTE_21` | ROUTE 21 | 8 | PERCORSO 21 | 11 |
| 122 | `ROUTE_22` | ROUTE 22 | 8 | PERCORSO 22 | 11 |
| 123 | `ROUTE_23` | ROUTE 23 | 8 | PERCORSO 23 | 11 |
| 124 | `ROUTE_24` | ROUTE 24 | 8 | PERCORSO 24 | 11 |
| 125 | `ROUTE_25` | ROUTE 25 | 8 | PERCORSO 25 | 11 |
| 126 | `VIRIDIAN_FOREST` | VIRIDIAN FOREST | 15 | BOSCO SMERALDO | 14 |
| 127 | `MT_MOON` | MT. MOON | 8 | MONTE LUNA | 10 |
| 128 | `S_S_ANNE` | S.S. ANNE | 9 | M/N ANNA | 8 |
| 129 | `UNDERGROUND_PATH` | UNDERGROUND PATH | 16 | VIA SOTTERRANEA | 15 |
| 130 | `UNDERGROUND_PATH_2` | UNDERGROUND PATH (name_clone) | 16 | VIA SOTTERRANEA | 15 |
| 131 | `DIGLETTS_CAVE` | DIGLETT'S CAVE | 14 | GROTTA DIGLETT | 14 |
| 132 | `KANTO_VICTORY_ROAD` | VICTORY ROAD | 12 | VIA VITTORIA | 12 |
| 133 | `ROCKET_HIDEOUT` | ROCKET HIDEOUT | 14 | RIFUGIO ROCKET | 14 |
| 134 | `SILPH_CO` | SILPH CO. | 9 | SILPH SpA | 9 |
| 135 | `POKEMON_MANSION` | POKéMON MANSION | 15 | *VILLA POKéMON* | 13 |
| 136 | `KANTO_SAFARI_ZONE` | SAFARI ZONE | 11 | ZONA SAFARI | 11 |
| 137 | `POKEMON_LEAGUE` | POKéMON LEAGUE | 14 | *LEGA POKéMON* | 12 |
| 138 | `ROCK_TUNNEL` | ROCK TUNNEL | 11 | TUNNEL ROCCIOSO | 15 |
| 139 | `SEAFOAM_ISLANDS` | SEAFOAM ISLANDS | 15 | ISOLE SPUMARINE | 15 |
| 140 | `POKEMON_TOWER` | POKéMON TOWER | 13 | *TORRE POKéMON* | 13 |
| 141 | `CERULEAN_CAVE` | CERULEAN CAVE | 13 | GROTTA CELESTE | 14 |
| 142 | `POWER_PLANT` | POWER PLANT | 11 | CENTRALE ELETT. | 15 |
| 143 | `ONE_ISLAND` | ONE ISLAND | 10 | PRIMISOLA | 9 |
| 144 | `TWO_ISLAND` | TWO ISLAND | 10 | SECONDISOLA | 11 |
| 145 | `THREE_ISLAND` | THREE ISLAND | 12 | TERZISOLA | 9 |
| 146 | `FOUR_ISLAND` | FOUR ISLAND | 11 | QUARTISOLA | 10 |
| 147 | `FIVE_ISLAND` | FIVE ISLAND | 11 | QUINTISOLA | 10 |
| 148 | `SEVEN_ISLAND` | SEVEN ISLAND | 12 | SETTIMISOLA | 11 |
| 149 | `SIX_ISLAND` | SIX ISLAND | 10 | SESTISOLA | 9 |
| 150 | `KINDLE_ROAD` | KINDLE ROAD | 11 | VIA VULCANICA | 13 |
| 151 | `TREASURE_BEACH` | TREASURE BEACH | 14 | RIVA del TESORO | 15 |
| 152 | `CAPE_BRINK` | CAPE BRINK | 10 | CAPO ESTREMO | 12 |
| 153 | `BOND_BRIDGE` | BOND BRIDGE | 11 | PONTE ABBRACCIO | 15 |
| 154 | `THREE_ISLE_PORT` | THREE ISLE PORT | 15 | PORTO TERZISOLA | 15 |
| 155 | `RESORT_GORGEOUS` | RESORT GORGEOUS | 15 | PERLA dei MARI | 14 |
| 156 | `WATER_LABYRINTH` | WATER LABYRINTH | 15 | LABIRINTO MARINO | 16 |
| 157 | `FIVE_ISLE_MEADOW` | FIVE ISLE MEADOW | 16 | PRATO QUINTISOLA | 16 |
| 158 | `MEMORIAL_PILLAR` | MEMORIAL PILLAR | 15 | COLONNA ROCCIOSA | 16 |
| 159 | `OUTCAST_ISLAND` | OUTCAST ISLAND | 14 | ISOLA SOLITARIA | 15 |
| 160 | `GREEN_PATH` | GREEN PATH | 10 | VIA VERDE | 9 |
| 161 | `WATER_PATH` | WATER PATH | 10 | VIA MARINA | 10 |
| 162 | `RUIN_VALLEY` | RUIN VALLEY | 11 | VALLE ANTICA | 12 |
| 163 | `TRAINER_TOWER` | TRAINER TOWER | 13 | TORRE ALLENATORI | 16 |
| 164 | `CANYON_ENTRANCE` | CANYON ENTRANCE | 15 | INGRESSO CANYON | 15 |
| 165 | `SEVAULT_CANYON` | SEVAULT CANYON | 14 | CANYON SEPTION | 14 |
| 166 | `TANOBY_RUINS` | TANOBY RUINS | 12 | ROVINE FLORABETO | 16 |
| 167 | `SEVII_ISLE_22` | SEVII ISLE 22 | 13 | SETTIPELAGO 22 | 14 |
| 168 | `SEVII_ISLE_23` | SEVII ISLE 23 | 13 | SETTIPELAGO 23 | 14 |
| 169 | `SEVII_ISLE_24` | SEVII ISLE 24 | 13 | SETTIPELAGO 24 | 14 |
| 170 | `NAVEL_ROCK_FRLG` | NAVEL ROCK | 10 | MONTE CORDONE | 13 |
| 171 | `MT_EMBER` | MT. EMBER | 9 | MONTE BRACE | 11 |
| 172 | `BERRY_FOREST` | BERRY FOREST | 12 | BOSCO BACCOSO | 13 |
| 173 | `ICEFALL_CAVE` | ICEFALL CAVE | 12 | GROTTA GELATA | 13 |
| 174 | `ROCKET_WAREHOUSE` | ROCKET WAREHOUSE | 16 | MAGAZZINO ROCKET | 16 |
| 175 | `TRAINER_TOWER_2` | TRAINER TOWER (name_clone) | 13 | TORRE ALLENATORI | 16 |
| 176 | `DOTTED_HOLE` | DOTTED HOLE | 11 | CRIPTA dei PUNTI | 16 |
| 177 | `LOST_CAVE` | LOST CAVE | 9 | GROTTA SPERDUTA | 15 |
| 178 | `PATTERN_BUSH` | PATTERN BUSH | 12 | BOSCO DISEGNATO | 15 |
| 179 | `ALTERING_CAVE_FRLG` | ALTERING CAVE | 13 | GROTTA MUTEVOLE | 15 |
| 180 | `TANOBY_CHAMBERS` | TANOBY CHAMBERS | 15 | SALE FLORABETO | 14 |
| 181 | `THREE_ISLE_PATH` | THREE ISLE PATH | 15 | VIA TERZISOLA | 13 |
| 182 | `TANOBY_KEY` | TANOBY KEY | 10 | CHIAVE FLORABETO | 16 |
| 183 | `BIRTH_ISLAND_FRLG` | BIRTH ISLAND | 12 | ISOLA MATERNA | 13 |
| 184 | `MONEAN_CHAMBER` | MONEAN CHAMBER | 14 | SALA A-LOE | 10 |
| 185 | `LIPTOO_CHAMBER` | LIPTOO CHAMBER | 14 | SALA B-ETULLA | 13 |
| 186 | `WEEPTH_CHAMBER` | WEEPTH CHAMBER | 14 | SALA C-ICLAMINO | 15 |
| 187 | `DILFORD_CHAMBER` | DILFORD CHAMBER | 15 | SALA D-AFNE | 11 |
| 188 | `SCUFIB_CHAMBER` | SCUFIB CHAMBER | 14 | SALA E-DERA | 11 |
| 189 | `RIXY_CHAMBER` | RIXY CHAMBER | 12 | SALA F-ELCE | 11 |
| 190 | `VIAPOIS_CHAMBER` | VIAPOIS CHAMBER | 15 | SALA G-ARDENIA | 14 |
| 191 | `EMBER_SPA` | EMBER SPA | 9 | TERME LAVICHE | 13 |
| 192 | `SPECIAL_AREA` | SPECIAL AREA | 12 | — |  |

### 1.2 Landmarks — `src/landmark.c`

Landmarks are the sub-place names that the **PokéNav region map** lists under a map section when the cursor is on a route (for example "PETALBURG WOODS" and "MR. BRINEY'S COTTAGE" on Route 104). They are printed by `PrintLandmarkNames()` (`src/pokenav_region_map.c:677`) with `FONT_NARROW` in a 12-tile (96 px) window (`sMapSecInfoWindowTemplate`, `src/pokenav_region_map.c:141`), padded to 12 characters by `StringCopyPadded(…, 12)`. Longer names are **not cut**, they just run on; the longest vanilla name has 21 bytes. **Practical limit: ≤ 21 bytes, and check it fits 96 px in the narrow font.** A landmark with a flag is shown only once that flag is set; `-1` = always shown.

| Line | Landmark | Name | len | Flag | Shown on (`MAPSEC`/sub-index) |
|---|---|---|---|---|---|
| 20 | `Landmark_FlowerShop` | FLOWER SHOP | 11 | `FLAG_LANDMARK_FLOWER_SHOP` | ROUTE_104/0 |
| 21 | `Landmark_PetalburgWoods` | PETALBURG WOODS | 15 | — | ROUTE_104/1 |
| 22 | `Landmark_MrBrineysCottage` | MR. BRINEY'S COTTAGE | 20 | `FLAG_LANDMARK_MR_BRINEY_HOUSE` | ROUTE_104/1 |
| 23 | `Landmark_AbandonedShip` | ABANDONED SHIP | 14 | `FLAG_LANDMARK_ABANDONED_SHIP` | ROUTE_108/0 |
| 24 | `Landmark_SeashoreHouse` | SEASHORE HOUSE | 14 | `FLAG_LANDMARK_SEASHORE_HOUSE` | ROUTE_109/0 |
| 25 | `Landmark_SlateportBeach` | SLATEPORT BEACH | 15 | — | ROUTE_109/0 |
| 26 | `Landmark_CyclingRoad` | CYCLING ROAD | 12 | — | ROUTE_110/0, ROUTE_110/1, ROUTE_110/2 |
| 27 | `Landmark_NewMauville` | NEW MAUVILLE | 12 | `FLAG_LANDMARK_NEW_MAUVILLE` | ROUTE_110/0 |
| 28 | `Landmark_TrickHouse` | TRICK HOUSE | 11 | `FLAG_LANDMARK_TRICK_HOUSE` | ROUTE_110/2 |
| 29 | `Landmark_OldLadysRestShop` | OLD LADY'S REST STOP | 20 | `FLAG_LANDMARK_OLD_LADY_REST_SHOP` | ROUTE_111/0 |
| 30 | `Landmark_Desert` | DESERT | 6 | — | ROUTE_111/1, ROUTE_111/2, ROUTE_111/3, ROUTE_111/4 |
| 31 | `Landmark_WinstrateFamily` | THE WINSTRATE FAMILY | 20 | `FLAG_LANDMARK_WINSTRATE_FAMILY` | ROUTE_111/4 |
| 32 | `Landmark_CableCar` | CABLE CAR | 9 | — | ROUTE_112/1, MT_CHIMNEY/2 |
| 33 | `Landmark_GlassWorkshop` | GLASS WORKSHOP | 14 | `FLAG_LANDMARK_GLASS_WORKSHOP` | ROUTE_113/1 |
| 34 | `Landmark_WeatherInstitute` | WEATHER INSTITUTE | 17 | — | ROUTE_119/1 |
| 35 | `Landmark_MeteorFalls` | METEOR FALLS | 12 | — | ROUTE_114/3, ROUTE_115/0, ROUTE_115/1 |
| 36 | `Landmark_TunnelersRestHouse` | TUNNELER'S RESTHOUSE | 20 | `FLAG_LANDMARK_TUNNELERS_REST_HOUSE` | ROUTE_116/1 |
| 37 | `Landmark_RusturfTunnel` | RUSTURF TUNNEL | 14 | — | ROUTE_116/1, ROUTE_116/2 |
| 38 | `Landmark_PokemonDayCare` | POKéMON DAY CARE | 16 | `FLAG_LANDMARK_POKEMON_DAYCARE` | ROUTE_117/2 |
| 39 | `Landmark_SafariZoneEntrance` | SAFARI ZONE ENTRANCE | 20 | — | ROUTE_121/2 |
| 40 | `Landmark_MtPyre` | MT. PYRE | 8 | — | ROUTE_122/0, ROUTE_122/1 |
| 41 | `Landmark_ShoalCave` | SHOAL CAVE | 10 | — | ROUTE_125/2 |
| 42 | `Landmark_SeafloorCavern` | SEAFLOOR CAVERN | 15 | `FLAG_LANDMARK_SEAFLOOR_CAVERN` | ROUTE_128/1 |
| 43 | `Landmark_GraniteCave` | GRANITE CAVE | 12 | — | ROUTE_106/1 |
| 44 | `Landmark_OceanCurrent` | OCEAN CURRENT | 13 | — | ROUTE_132/0, ROUTE_132/1, ROUTE_133/0, ROUTE_133/1, ROUTE_133/2, ROUTE_134/0, ROUTE_134/1, ROUTE_134/2 |
| 45 | `Landmark_LanettesHouse` | LANETTE'S HOUSE | 15 | `FLAG_LANDMARK_LANETTES_HOUSE` | ROUTE_114/2 |
| 46 | `Landmark_FieryPath` | FIERY PATH | 10 | `FLAG_LANDMARK_FIERY_PATH` | ROUTE_112/0, ROUTE_112/1 |
| 47 | `Landmark_JaggedPass` | JAGGED PASS | 11 | — | ROUTE_112/0, MT_CHIMNEY/2 |
| 48 | `Landmark_BerryMastersHouse` | BERRY MASTER'S HOUSE | 20 | `FLAG_LANDMARK_BERRY_MASTERS_HOUSE` | ROUTE_123/0 |
| 49 | `Landmark_IslandCave` | ISLAND CAVE | 11 | `FLAG_LANDMARK_ISLAND_CAVE` | ROUTE_105/0 |
| 50 | `Landmark_DesertRuins` | DESERT RUINS | 12 | `FLAG_LANDMARK_DESERT_RUINS` | ROUTE_111/3 |
| 51 | `Landmark_ScorchedSlab` | SCORCHED SLAB | 13 | `FLAG_LANDMARK_SCORCHED_SLAB` | ROUTE_120/0 |
| 52 | `Landmark_AncientTomb` | ANCIENT TOMB | 12 | `FLAG_LANDMARK_ANCIENT_TOMB` | ROUTE_120/2 |
| 53 | `Landmark_SealedChamber` | SEALED CHAMBER | 14 | `FLAG_LANDMARK_SEALED_CHAMBER` | ROUTE_134/2 |
| 54 | `Landmark_FossilManiacsHouse` | FOSSIL MANIAC'S HOUSE | 21 | `FLAG_LANDMARK_FOSSIL_MANIACS_HOUSE` | ROUTE_114/1 |
| 55 | `Landmark_HuntersHouse` | HUNTER'S HOUSE | 14 | `FLAG_LANDMARK_HUNTERS_HOUSE` | ROUTE_124/7 |
| 56 | `Landmark_SkyPillar` | SKY PILLAR | 10 | `FLAG_LANDMARK_SKY_PILLAR` | ROUTE_131/1 |
| 57 | `Landmark_MirageTower` | MIRAGE TOWER | 12 | `FLAG_LANDMARK_MIRAGE_TOWER` | ROUTE_111/2 |
| 58 | `Landmark_AlteringCave` | ALTERING CAVE | 13 | `FLAG_LANDMARK_ALTERING_CAVE` | ROUTE_103/2 |
| 59 | `Landmark_DesertUnderpass` | DESERT UNDERPASS | 16 | `FLAG_LANDMARK_DESERT_UNDERPASS` | ROUTE_114/1 |
| 60 | `Landmark_TrainerHill` | TRAINER HILL | 12 | `FLAG_LANDMARK_TRAINER_HILL` | ROUTE_111/4 |

Also in this file: `LandmarkName_MagmaHideout[] = _("MAGMA HIDEOUT")` at line 18, marked *Unused*. Story-relevant landmarks to re-theme: `MR. BRINEY'S COTTAGE`, `LANETTE'S HOUSE`, `THE WINSTRATE FAMILY`, `NEW MAUVILLE`, `WEATHER INSTITUTE`, `SEAFLOOR CAVERN`, `SKY PILLAR`, `MIRAGE TOWER`, and the names of people's houses (`BERRY MASTER'S`, `FOSSIL MANIAC'S`, `HUNTER'S`, `TUNNELER'S`, `OLD LADY'S`).

### 1.3 Other place names outside the JSON

| Where | String | Use |
|---|---|---|
| `src/strings.c:469` | SLATEPORT CITY | `gText_SlateportCity`, S.S. Tidal destination menu (`src/data/script_menu.h:308, 1362`) |
| `src/data/script_menu.h:297` | LILYCOVE CITY | `gText_LilycoveCity`, S.S. Tidal / ferry menus (lines 301, 309, 321) |
| `src/data/script_menu.h:4-5` | PETALBURG / SLATEPORT | multichoice (Mr. Briney boat destinations) |
| `src/data/script_menu.h:81` | DEWFORD | multichoice (Mr. Briney boat destinations) |
| `src/strings.c:560` | POKéMON LEAGUE | `gText_PokemonLeague` |
| `src/strings.c:573-575` | FERRY / SECRET BASE / HIDEOUT | map-name fallbacks (see 1.1) |
| `src/strings.c:349` | 's BASE | secret-base map name suffix |
| `src/strings.c:904, 909, 910` | HOENN / HOENN / KANTO | `gText_DexHoenn`, `gText_Hoenn` (the `{REGION}` placeholder), `gText_Kanto` |
| `data/text/birch_speech.inc:47` | LITTLEROOT | intro speech, "moving to my hometown of LITTLEROOT" |

Place names written literally inside dialogue (no placeholder) are counted in section 4.3.

## 2. Trainer names — `src/data/trainers.party`

* **Limit: `TRAINER_NAME_LENGTH` = 10** (`include/constants/global.h:171`); stored as `u8 trainerName[TRAINER_NAME_LENGTH + 1]` (`include/data.h:142`), written by `tools/trainerproc` (`tools/trainerproc/main.c:1831`). trainerproc emits the name as `_("…")`, which the preprocessor turns into a byte list, so a longer name gives an "excess elements in array initializer" diagnostic, an error under `-Werror` (`Makefile:170`).
* Format: one block per trainer, `=== TRAINER_XXX ===` then `Name:`, `Class:`, `Pic:` … Change only the `Name:` line. `Class:` selects `TRAINER_CLASS_<CLASS IN CAPS>` (section 3), `Pic:` the battle sprite (kept).
* The file has **855 entries** (including `TRAINER_NONE`), in **65 classes**. Battle text shows them as "<CLASS> <NAME>" (`B_TXT_TRAINER1_NAME_WITH_CLASS`, `src/battle_message.c:3296`), e.g. "TEAM AQUA GRUNT", "LEADER ROXANNE".
* Other trainer-name tables (not story characters, listed for completeness): `src/data/battle_partners.party` (`PARTNER_STEVEN`, Name `STEVEN`, Class `Rival`: the Mossdeep Space Center double battle partner); `src/data/battle_frontier/battle_frontier_trainers.h` (300 generated Frontier trainers); `src/data/battle_frontier/trainer_hill.h`; `src/data/contest_opponents.h` (contest NPCs, e.g. `.trainerName = _("DEVON")` at line 2285); `src/data/debug_trainers.party`.

### 2.1 Story characters

| Character | `TRAINER_*` ids (n) | Name | Class shown | Battled in (map) | IT official | Other places that hard-code the name |
|---|---|---|---|---|---|---|
| Rival (male player) | TRAINER_MAY_ROUTE_103_{MUDKIP,TREECKO,TORCHIC}, _ROUTE_110_*, _ROUTE_119_*, _RUSTBORO_*, _LILYCOVE_* (15) | MAY | {PKMN} TRAINER | Route103, RustboroCity + Route104 (RUSTBORO ids), Route110, Route119, LilycoveCity | Vera | `gText_ExpandedPlaceholder_May` `src/strings.c:20` (= `{RIVAL}`), Match Call `src/pokenav_match_call_data.c:282`, `src/trainer_fan_club.c:292, 308`, `src/quickstart.c:83` |
| Rival (female player) | TRAINER_BRENDAN_* same 15 ids | BRENDAN | {PKMN} TRAINER | same maps | Brendon | `gText_ExpandedPlaceholder_Brendan` `src/strings.c:19`, `src/pokenav_match_call_data.c:309`, `src/trainer_fan_club.c:294, 310`, `src/quickstart.c:82` |
| Wally | TRAINER_WALLY_MAUVILLE, TRAINER_WALLY_VR_1…_5 (6) | WALLY | {PKMN} TRAINER | MauvilleCity, VictoryRoad_1F | Lino | `gText_BattleWallyName` `src/battle_message.c:1507` (**must equal the trainers.party name**, see 2.3), `src/battle_message.c:3559` (catch tutorial) |
| Steven | TRAINER_STEVEN (1) + PARTNER_STEVEN | STEVEN | {PKMN} TRAINER | MeteorFalls_StevensCave (post-game); partner in MossdeepCity_SpaceCenter_2F | Rocco | Match Call `src/pokenav_match_call_data.c:261` (desc "HARD AS ROCK"), `src/field_specials.c:185` (fan club) |
| Gym 1 Rustboro | TRAINER_ROXANNE_1…5 (5) | ROXANNE | LEADER | RustboroCity_Gym | Petra | Match Call desc "ROCKIN' WHIZ" `src/pokenav_match_call_data.c:380` (name read from trainers.party) |
| Gym 2 Dewford | TRAINER_BRAWLY_1…5 (5) | BRAWLY | LEADER | DewfordTown_Gym | Rudi | desc "THE BIG HIT" `:397`; `src/field_specials.c:186` |
| Gym 3 Mauville | TRAINER_WATTSON_1…5 (5) | WATTSON | LEADER | MauvilleCity_Gym | Walter | desc "SWELL SHOCK" `:414` |
| Gym 4 Lavaridge | TRAINER_FLANNERY_1…5 (5) | FLANNERY | LEADER | LavaridgeTown_Gym_1F | Fiammetta | desc "PASSION BURN" `:431` |
| Gym 5 Petalburg (Dad) | TRAINER_NORMAN_1…5 (5) | NORMAN | LEADER | PetalburgCity_Gym | Norman | Match Call name "DAD" `:209` (desc "RELIABLE ONE"), `gText_Dad` `src/strings.c:395` |
| Gym 6 Fortree | TRAINER_WINONA_1…5 (5) | WINONA | LEADER | FortreeCity_Gym | Alice | desc "SKY TAMER" `:448`; `src/field_specials.c:187` |
| Gym 7 Mossdeep | TRAINER_TATE_AND_LIZA_1…5 (5) | TATE&LIZA | LEADER | MossdeepCity_Gym | Tell e Pat | desc "MYSTIC DUO" `:465`; 9 bytes, no spaces (limit 10) |
| Gym 8 Sootopolis | TRAINER_JUAN_1…5 (5) | JUAN | LEADER | SootopolisCity_Gym_1F | Rodolfo | desc "DANDY CHARM" `:482` |
| Elite Four 1 | TRAINER_SIDNEY (1) | SIDNEY | ELITE FOUR | EverGrandeCity_SidneysRoom | Fosco | desc `gText_EliteFourMatchCallDesc` "ELITE FOUR" `:493` |
| Elite Four 2 | TRAINER_PHOEBE (1) | PHOEBE | ELITE FOUR | EverGrandeCity_PhoebesRoom | Ester | `src/field_specials.c:188` |
| Elite Four 3 | TRAINER_GLACIA (1) | GLACIA | ELITE FOUR | EverGrandeCity_GlaciasRoom | Frida | `src/field_specials.c:189` |
| Elite Four 4 | TRAINER_DRAKE (1) | DRAKE | ELITE FOUR | EverGrandeCity_DrakesRoom | Drake |  |
| Champion | TRAINER_WALLACE (1) | WALLACE | CHAMPION | EverGrandeCity_ChampionsRoom (also `src/battle_setup.c`) | Adriano | Match Call desc "CHAMPION" `:557`; `src/field_specials.c:184` |
| Aqua leader | TRAINER_ARCHIE (1) | ARCHIE | AQUA LEADER | SeafloorCavern_Room9 | Ivan | `{ARCHIE}` → `gText_ExpandedPlaceholder_Archie` `src/strings.c:15` (placeholder unused in texts) |
| Aqua admin (M) | TRAINER_MATT (1) | MATT | AQUA ADMIN | AquaHideout_B2F | — | never named in dialogue |
| Aqua admin (F) | TRAINER_SHELLY_WEATHER_INSTITUTE, _SEAFLOOR_CAVERN (2) | SHELLY | AQUA ADMIN | Route119_WeatherInstitute_2F, SeafloorCavern_Room3 | — | never named in dialogue |
| Magma leader | TRAINER_MAXIE_MT_CHIMNEY, _MAGMA_HIDEOUT, _MOSSDEEP (3) | MAXIE | MAGMA LEADER | MtChimney, MagmaHideout_4F, MossdeepCity_SpaceCenter_2F | Max | `{MAXIE}` → `src/strings.c:16` (placeholder unused in texts) |
| Magma admin | TRAINER_TABITHA_MT_CHIMNEY, _MAGMA_HIDEOUT, _MOSSDEEP (3) | TABITHA | MAGMA ADMIN | MtChimney, MagmaHideout_4F, MossdeepCity_SpaceCenter_2F | — | never named in dialogue (Courtney does not exist in Emerald) |
| Grunts | TRAINER_GRUNT_* (Class Team Aqua 26, Team Magma 27) | GRUNT | TEAM AQUA / TEAM MAGMA | many | — | name is the same for all 53 |
| Frontier Brain, Tower | TRAINER_ANABEL | ANABEL | SALON MAIDEN | BattleFrontier_BattleTowerBattleRoom | — |  |
| Frontier Brain, Dome | TRAINER_TUCKER | TUCKER | DOME ACE | BattleFrontier_BattleDomeBattleRoom | — |  |
| Frontier Brain, Palace | TRAINER_SPENSER | SPENSER | PALACE MAVEN | BattleFrontier_BattlePalaceBattleRoom | — |  |
| Frontier Brain, Arena | TRAINER_GRETA | GRETA | ARENA TYCOON | BattleFrontier_BattleArenaBattleRoom | — |  |
| Frontier Brain, Factory | TRAINER_NOLAND | NOLAND | FACTORY HEAD | BattleFrontier_BattleFactoryBattleRoom | — |  |
| Frontier Brain, Pike | TRAINER_LUCY | LUCY | PIKE QUEEN | BattleFrontier_BattlePikeRoomNormal | — |  |
| Frontier Brain, Pyramid | TRAINER_BRANDON | BRANDON | PYRAMID KING | BattleFrontier_BattlePyramidTop | — |  |
| Interviewers | TRAINER_GABBY_AND_TY_1…6 (6) | GABBY & TY | INTERVIEWER | Route111, Route118, Route120 (moves after each battle; TV reports from SlateportCity) | — | 10 bytes = at the limit; `src/match_call.c:1710` "GABBY" |
| Winstrate family | TRAINER_VICTOR, _VICTORIA, _VICKY, _VIVI | VICTOR / VICTORIA / VICKY / VIVI | WINSTRATE | Route111_WinstrateFamilysHouse | — | landmark "THE WINSTRATE FAMILY" |
| Unused here | TRAINER_RED, TRAINER_LEAF, TRAINER_BRENDAN_PLACEHOLDER, TRAINER_MAY_PLACEHOLDER | RED, LEAF, BRENDAN, MAY | {PKMN} TRAINER | not referenced by any Hoenn script or C file | — | the last two use class `RS Protag` (Ruby/Sapphire protagonist pics) |

Hoenn leaders, Elite Four and Champion appear in Match Call with `.name = NULL`, so their PokéNav name is read from `trainers.party`; only the `.desc` strings (12 bytes max in vanilla) are hard-coded in `src/pokenav_match_call_data.c`.

### 2.2 All trainer names by class

Ordered as the classes first appear in `trainers.party`. "Entries" counts every block (rematches included); the name list gives each distinct name once, with (n) when it is used by more than one block.

| Class in `.party` | Entries | Distinct | Names |
|---|---|---|---|
| Pkmn Trainer 1 | 1 | 1 | *(empty)* |
| Hiker | 22 | 12 | SAWYER (5), ELI, MARC, BRICE, TRENT (5), LENNY, LUCAS (2), ALAN, CLARK, ERIC, MIKE (2), DEVAN |
| Team Aqua | 26 | 1 | GRUNT (26) |
| Pkmn Breeder | 17 | 5 | GABRIELLE (5), ISAAC (5), LYDIA (5), MYLES, PAT |
| Cooltrainer | 55 | 43 | MARCEL, FELIX, RANDALL, PARKER, GEORGE, BERKE, BRAXTON, VINCENT, LEROY, WILTON (5), EDGAR, ALBERT, SAMUEL, VITO, OWEN, WARREN, MARY, ALEXIA, JODY, WENDY, KEIRA, BROOKE (5), JENNIFER, HOPE, SHANNON, MICHELLE, CAROLINE, JULIE, QUINCY, KATELYNN, DIANNE, MARLEY, MITCHELL, HALLE, ATHENA, JONATHAN, GERALD, ALEXA, RUBEN, DARCY, CAROLINA, LEONEL, CRISTIN (5) |
| Bird Keeper | 23 | 19 | ALBERTO, PERRY, HUGH, PHIL, JARED, HUMBERTO, PRESLEY, EDWARDO, COLIN, ROBERT (5), BENNY, CHESTER, ALEX, BECK, AIDAN, COBY, JOSUE, ELIJAH, DARIUS |
| Collector | 7 | 3 | ED, EDWIN (5), HECTOR |
| Swimmer M | 34 | 30 | DECLAN, LUIS, DOMINIK, DOUGLAS, DARRIN, TONY (5), JEROME, MATTHEW, DAVID, SPENCER, ROLAND, NOLEN, STAN, BARRY, DEAN, RODNEY, RICHARD, HERMAN, SANTIAGO, GILBERT, FRANKLIN, KEVIN, JACK, DUDLEY, CHAD, LEONARDO, HARRISON, CLARENCE, REED, PETE |
| Team Magma | 27 | 1 | GRUNT (27) |
| Expert | 16 | 8 | FREDRICK, MOLLIE, TIMOTHY (5), SHELBY (5), AURON, CONOR, PAXTON, MAKAYLA |
| Aqua Admin | 3 | 2 | MATT, SHELLY (2) |
| Black Belt | 20 | 12 | ZANDER, TAKAO, HITOSHI, KIYO, KOICHI, NOB (5), YUJI, DAISUKE, ATSUSHI, CRISTIAN, KOJI (5), RHETT |
| Aqua Leader | 1 | 1 | ARCHIE |
| Hex Maniac | 12 | 8 | LEAH, PATRICIA, KINDRA, TAMMY, VALERIE (5), TASHA, SYLVIA, KATHLEEN |
| Aroma Lady | 9 | 5 | DAISY, ROSE (5), VIOLET, CELINA, SHAYLA |
| Ruin Maniac | 14 | 6 | DUSTY (5), CHIP, FOSTER, GARRISON, ANDRES (5), BRYAN |
| Interviewer | 6 | 1 | GABBY & TY (6) |
| Tuber F | 9 | 5 | LOLA (5), AUSTINA, GWEN, JANI, HAILEY |
| Tuber M | 8 | 4 | RICKY (5), SIMON, CHARLIE, CHANDLER |
| Lady | 10 | 5 | CINDY (6), DAPHNE, BRIANNA, NAOMI, SARAH |
| Beauty | 18 | 10 | MELISSA, SHEILA, SHIRLEY, JESSICA (5), CONNIE, BRIDGET, OLIVIA, TIFFANY, THALIA (5), JOHANNA |
| Rich Boy | 7 | 3 | WINSTON (5), GARRET, DAWSON |
| Pokemaniac | 7 | 3 | STEVE (5), MARK, WYATT |
| Guitarist | 15 | 7 | KIRK, SHAWN, FERNANDO (5), DALTON (5), JOSEPH, MARCOS, FABIAN |
| Kindler | 13 | 9 | COLE, JEFF, AXLE, JACE, KEEGAN, BERNIE (5), HAYDEN, BRYANT, DAYTON |
| Camper | 15 | 11 | DREW, BEAU, LARRY, SHANE, JUSTIN, ETHAN (5), TRAVIS, FLINT, TYRON, LAWRENCE, BRANDEN |
| Picnicker | 18 | 14 | AUTUMN, HEIDI, BECKY, CAROL, NANCY, MARTHA, DIANA (5), IRENE, ASHLEY, BIANCA, SOPHIE, ANGELINA, CHARLOTTE, CELIA |
| Bug Maniac | 11 | 7 | BRENT, DONALD, TAYLOR, JEFFREY (5), DEREK, CALE, ANGELO |
| Psychic | 31 | 23 | EDWARD, PRESTON, VIRGIL, BLAKE, WILLIAM, JOSHUA, CAMERON (5), JACLYN, HANNAH, SAMANTHA, MAURA, KAYLA, ALEXIS, JACKI (5), CEDRIC, TERRY, NICHOLAS, MACEY, ALIX, MARLENE, BRANDI, MARIELA, ALVARO |
| Gentleman | 10 | 6 | WALTER (5), MICAH, THOMAS, NATE, CLIFFORD, EVERETT |
| Elite Four | 4 | 4 | SIDNEY, PHOEBE, GLACIA, DRAKE |
| Leader | 40 | 8 | ROXANNE (5), BRAWLY (5), WATTSON (5), FLANNERY (5), NORMAN (5), WINONA (5), TATE&LIZA (5), JUAN (5) |
| School Kid | 13 | 5 | JERRY (5), TED, PAUL, KAREN (5), GEORGIA |
| Sr And Jr | 8 | 4 | KATE & JOY, ANNA & MEG (5), KIM & IRIS, TYRA & IVY |
| Winstrate | 4 | 4 | VICTOR, VICTORIA, VICKY, VIVI |
| Pokefan | 15 | 7 | MIGUEL (5), COLTON, VANESSA, BETHANY, ISABEL (5), ANNIKA, KALEB |
| Youngster | 18 | 14 | CALVIN (5), BILLY, JOSH, TOMMY, JOEY, BEN, JAYLEN, DILLON, EDDIE, ALLEN, TIMMY, DEMETRIUS, DEANDRE, JOHNSON |
| Champion | 1 | 1 | WALLACE |
| Fisherman | 22 | 18 | ANDREW, IVAN, CLAUDE, ELLIOT (5), NED, DALE, NOLAN, BARNY, WADE, CARTER, RONALD, JONAH, HENRY, ROGER, WAYNE, CHRIS, DARIAN, KAI |
| Triathlete | 50 | 22 | JACOB, ANTHONY, BENJAMIN (5), ABIGAIL (5), JASMINE, DYLAN (5), MARIA (5), CAMDEN, ISAIAH (5), PABLO (5), CHASE, ISOBEL, DONNY, TALIA, KATELYN (5), ALLISON, JULIO, ISABELLA, ALYSSA, CAMRON, KYRA, MELINA |
| Dragon Tamer | 6 | 2 | NICOLAS (5), AARON |
| Ninja Boy | 13 | 9 | YASU, TAKASHI, LAO (5), LUNG, JONAS, HIDEO, KEIGO, RILEY, JAIDEN |
| Battle Girl | 16 | 12 | JOCELYN, LAURA, CYNDY (5), CORA, PAULA, REYNA, LILITH, VIVIAN, DANIELLE, HELENE, AISHA, CALLIE |
| Parasol Lady | 9 | 5 | MADELINE (5), CLARISSA, ANGELICA, KAYLEY, RACHEL |
| Swimmer F | 30 | 26 | BEVERLY, IMANI, KYLA, DENISE, BETH, TARA, MISSY, ALICE, JENNY (5), GRACE, TANYA, SHARON, NIKKI, BRENDA, KATIE, SUSIE, KARA, DANA, SIENNA, DEBRA, LINDA, KAYLEE, LAUREL, CARLEE, TISHA, ISABELLE |
| Twins | 10 | 4 | AMY & LIV (6), GINA & MIA (2), MIU & YUKI, TORI & TIA |
| Sailor | 19 | 11 | HUEY, EDMOND, ERNEST (5), DWAYNE, PHILLIP, LEONARD, DUNCAN, KELVIN, HUDSON, BRENDEN, CORY (5) |
| Cooltrainer 2 | 1 | 1 | JAZMYN |
| Magma Admin | 3 | 1 | TABITHA (3) |
| Rival | 39 | 6 | WALLY (6), BRENDAN (15), MAY (15), STEVEN, RED, LEAF |
| Bug Catcher | 12 | 8 | DAVIS, RICK, LYLE, JOSE, DOUG, GREG, KENT, JAMES (5) |
| Pkmn Ranger | 14 | 6 | JACKSON (5), LORENZO, SEBASTIAN, CATHERINE (5), JENNA, SOPHIA |
| Magma Leader | 3 | 1 | MAXIE (3) |
| Lass | 11 | 7 | TIANA, HALEY (5), JANICE, SALLY, ROBIN, ANDREA, CRISSY |
| Young Couple | 8 | 4 | DEZ & LUKE, LEA & JED, KIRA & DAN (5), MEL & PAUL |
| Old Couple | 5 | 1 | JOHN & JAY (5) |
| Sis And Bro | 7 | 3 | RELI & IAN, LILA & ROY (5), LISA & RAY |
| Salon Maiden | 1 | 1 | ANABEL |
| Dome Ace | 1 | 1 | TUCKER |
| Palace Maven | 1 | 1 | SPENSER |
| Arena Tycoon | 1 | 1 | GRETA |
| Factory Head | 1 | 1 | NOLAND |
| Pike Queen | 1 | 1 | LUCY |
| Pyramid King | 1 | 1 | BRANDON |
| RS Protag | 2 | 2 | BRENDAN, MAY |

Names already at the 10-byte limit: `ANNA & MEG`, `DEZ & LUKE`, `GABBY & TY`, `GINA & MIA`, `JOHN & JAY`, `KATE & JOY`, `KIM & IRIS`, `KIRA & DAN`, `LILA & ROY`, `LISA & RAY`, `MEL & PAUL`, `MIU & YUKI`, `RELI & IAN`, `TORI & TIA`, `TYRA & IVY`. Double-battle "names" (`KATE & JOY`, `JOHN & JAY`, `TATE&LIZA`, …) are a single trainer whose name holds both people; the two speakers are also named separately in their texts (section 4.1).

### 2.3 Couplings to keep in sync

* **Wally**: `src/pokemon.c:5129` compares the opponent's trainer name with `gText_BattleWallyName` (`src/battle_message.c:1507`, `"WALLY"`) to play `MUS_VS_TRAINER` instead of `MUS_VS_RIVAL`. If Wally is renamed in `trainers.party`, the same new name must go into `gText_BattleWallyName`, or his battles get the rival music. The catch-tutorial battle prints the hard-coded `COMPOUND_STRING("WALLY")` at `src/battle_message.c:3559`.
* **Rival**: `{RIVAL}` in dialogue comes from `src/strings.c:19-20`, the battle name comes from the `TRAINER_MAY_*`/`TRAINER_BRENDAN_*` blocks. Both must be renamed together (15 blocks each).
* **Steven**: two blocks (`TRAINER_STEVEN` in `trainers.party`, `PARTNER_STEVEN` in `battle_partners.party`) plus Match Call and fan club strings.
* **Dad / Norman**: the Match Call contact is "DAD" (`src/pokenav_match_call_data.c:209`), the battle name is "NORMAN". Texts use both ("DAD" 40×, "NORMAN" 12×).

## 3. Trainer class names — `src/battle_main.c` `gTrainerClasses[]`

* **Limit: 12 bytes.** The field is `u8 name[13]` (`struct TrainerClass`, `include/data.h:150-155`): 12 bytes + terminator. `{PKMN}` counts 2 bytes, so "{PKMN} TRAINER" = 10. Vanilla already uses the full 12 in `DRAGON TAMER`, `PARASOL LADY`, `MAGMA LEADER`, `SALON MAIDEN`, `PALACE MAVEN`, `ARENA TYCOON`, `FACTORY HEAD`, `PYRAMID KING`, `YOUNG COUPLE`.
* Each entry is `[TRAINER_CLASS_X] = { _("NAME"), money, ball }`. Change only the string; a name over 12 bytes fails the build the same way as an over-long trainer name. The class is shown before the trainer name in battle ("<CLASS> <NAME>", `src/battle_message.c:3052-3066`) and is also read by PokéNav Match Call (`src/pokenav_match_call_data.c`, `src/pokenav_match_call_list.c`), the Battle Dome (`src/battle_dome.c:4202`) and the `buffertrainerclassname` script command (`src/scrcmd.c:3021`).
* "Uses" = number of `trainers.party` blocks with that class.

| Line | Constant | Name | len | Money | Uses | Story role |
|---|---|---|---|---|---|---|
| 300 | `PKMN_TRAINER_1` | {PKMN} TRAINER | 10 | 5 | 1 |  |
| 301 | `PKMN_TRAINER_2` | {PKMN} TRAINER | 10 | 5 | 0 |  |
| 302 | `HIKER` | HIKER | 5 | 10 | 22 |  |
| 303 | `TEAM_AQUA` | TEAM AQUA | 9 | 5 | 26 | villain grunts |
| 304 | `PKMN_BREEDER` | {PKMN} BREEDER | 10 | 10 | 17 |  |
| 305 | `COOLTRAINER` | COOLTRAINER | 11 | 12 | 55 |  |
| 306 | `BIRD_KEEPER` | BIRD KEEPER | 11 | 8 | 23 |  |
| 307 | `COLLECTOR` | COLLECTOR | 9 | 15 | 7 |  |
| 308 | `SWIMMER_M` | SWIMMER♂ | 8 | 2 | 34 |  |
| 309 | `TEAM_MAGMA` | TEAM MAGMA | 10 | 5 | 27 | villain grunts |
| 310 | `EXPERT` | EXPERT | 6 | 10 | 16 |  |
| 311 | `AQUA_ADMIN` | AQUA ADMIN | 10 | 10 | 3 | villain admin |
| 312 | `BLACK_BELT` | BLACK BELT | 10 | 8 | 20 |  |
| 313 | `AQUA_LEADER` | AQUA LEADER | 11 | 20 | 1 | villain boss |
| 314 | `HEX_MANIAC` | HEX MANIAC | 10 | 6 | 12 |  |
| 315 | `AROMA_LADY` | AROMA LADY | 10 | 10 | 9 |  |
| 316 | `RUIN_MANIAC` | RUIN MANIAC | 11 | 15 | 14 |  |
| 317 | `INTERVIEWER` | INTERVIEWER | 11 | 12 | 6 | Gabby & Ty |
| 318 | `TUBER_F` | TUBER | 5 | 1 | 9 |  |
| 319 | `TUBER_M` | TUBER | 5 | 1 | 8 |  |
| 320 | `LADY` | LADY | 4 | 50 | 10 |  |
| 321 | `BEAUTY` | BEAUTY | 6 | 20 | 18 |  |
| 322 | `RICH_BOY` | RICH BOY | 8 | 50 | 7 |  |
| 323 | `POKEMANIAC` | POKéMANIAC | 10 | 15 | 7 |  |
| 324 | `GUITARIST` | GUITARIST | 9 | 8 | 15 |  |
| 325 | `KINDLER` | KINDLER | 7 | 8 | 13 |  |
| 326 | `CAMPER` | CAMPER | 6 | 4 | 15 |  |
| 327 | `PICNICKER` | PICNICKER | 9 | 4 | 18 |  |
| 328 | `BUG_MANIAC` | BUG MANIAC | 10 | 15 | 11 |  |
| 329 | `PSYCHIC` | PSYCHIC | 7 | 6 | 31 |  |
| 330 | `GENTLEMAN` | GENTLEMAN | 9 | 20 | 10 |  |
| 331 | `ELITE_FOUR` | ELITE FOUR | 10 | 25 | 4 | Elite Four |
| 332 | `LEADER` | LEADER | 6 | 25 | 40 | gym leaders |
| 333 | `SCHOOL_KID` | SCHOOL KID | 10 | 5 | 13 |  |
| 334 | `SR_AND_JR` | SR. AND JR. | 11 | 4 | 8 |  |
| 335 | `WINSTRATE` | WINSTRATE | 9 | 10 | 4 | family name |
| 336 | `POKEFAN` | POKéFAN | 7 | 20 | 15 |  |
| 337 | `YOUNGSTER` | YOUNGSTER | 9 | 4 | 18 |  |
| 338 | `CHAMPION` | CHAMPION | 8 | 50 | 1 | champion |
| 339 | `FISHERMAN` | FISHERMAN | 9 | 10 | 22 |  |
| 340 | `TRIATHLETE` | TRIATHLETE | 10 | 10 | 50 |  |
| 341 | `DRAGON_TAMER` | DRAGON TAMER | 12 | 12 | 6 |  |
| 342 | `NINJA_BOY` | NINJA BOY | 9 | 3 | 13 |  |
| 343 | `BATTLE_GIRL` | BATTLE GIRL | 11 | 6 | 16 |  |
| 344 | `PARASOL_LADY` | PARASOL LADY | 12 | 10 | 9 |  |
| 345 | `SWIMMER_F` | SWIMMER♀ | 8 | 2 | 30 |  |
| 346 | `TWINS` | TWINS | 5 | 3 | 10 |  |
| 347 | `SAILOR` | SAILOR | 6 | 8 | 19 |  |
| 348 | `COOLTRAINER_2` | COOLTRAINER | 11 | 5 | 1 |  |
| 349 | `MAGMA_ADMIN` | MAGMA ADMIN | 11 | 10 | 3 | villain admin |
| 350 | `RIVAL` | {PKMN} TRAINER | 10 | 15 | 39 | rival, Wally, Steven |
| 351 | `BUG_CATCHER` | BUG CATCHER | 11 | 4 | 12 |  |
| 352 | `PKMN_RANGER` | {PKMN} RANGER | 9 | 12 | 14 |  |
| 353 | `MAGMA_LEADER` | MAGMA LEADER | 12 | 20 | 3 | villain boss |
| 354 | `LASS` | LASS | 4 | 4 | 11 |  |
| 355 | `YOUNG_COUPLE` | YOUNG COUPLE | 12 | 8 | 8 |  |
| 356 | `OLD_COUPLE` | OLD COUPLE | 10 | 10 | 5 |  |
| 357 | `SIS_AND_BRO` | SIS AND BRO | 11 | 3 | 7 |  |
| 358 | `SALON_MAIDEN` | SALON MAIDEN | 12 | 5 | 1 | Frontier Brain |
| 359 | `DOME_ACE` | DOME ACE | 8 | 5 | 1 | Frontier Brain |
| 360 | `PALACE_MAVEN` | PALACE MAVEN | 12 | 5 | 1 | Frontier Brain |
| 361 | `ARENA_TYCOON` | ARENA TYCOON | 12 | 5 | 1 | Frontier Brain |
| 362 | `FACTORY_HEAD` | FACTORY HEAD | 12 | 5 | 1 | Frontier Brain |
| 363 | `PIKE_QUEEN` | PIKE QUEEN | 10 | 5 | 1 | Frontier Brain |
| 364 | `PYRAMID_KING` | PYRAMID KING | 12 | 5 | 1 | Frontier Brain |
| 365 | `RS_PROTAG` | {PKMN} TRAINER | 10 | 5 | 2 | unused in Hoenn |

The 50 `_FRLG` classes that follow in the same table (lines 367–416) belong to the Kanto build: `YOUNGSTER_FRLG` "YOUNGSTER", `BUG_CATCHER_FRLG` "BUG CATCHER", `LASS_FRLG` "LASS", `SAILOR_FRLG` "SAILOR", `CAMPER_FRLG` "CAMPER", `PICNICKER_FRLG` "PICNICKER", `POKEMANIAC_FRLG` "POKéMANIAC", `SUPER_NERD_FRLG` "SUPER NERD", `HIKER_FRLG` "HIKER", `BIKER_FRLG` "BIKER", `BURGLAR_FRLG` "BURGLAR", `ENGINEER_FRLG` "ENGINEER", `FISHERMAN_FRLG` "FISHERMAN", `SWIMMER_M_FRLG` "SWIMMER♂", `CUE_BALL_FRLG` "CUE BALL", `GAMER_FRLG` "GAMER", `BEAUTY_FRLG` "BEAUTY", `SWIMMER_F_FRLG` "SWIMMER♀", `PSYCHIC_FRLG` "PSYCHIC", `ROCKER_FRLG` "ROCKER", `JUGGLER_FRLG` "JUGGLER", `TAMER_FRLG` "TAMER", `BIRD_KEEPER_FRLG` "BIRD KEEPER", `BLACK_BELT_FRLG` "BLACK BELT", `RIVAL_EARLY_FRLG` "RIVAL", `SCIENTIST_FRLG` "SCIENTIST", `BOSS_FRLG` "BOSS", `LEADER_FRLG` "LEADER", `TEAM_ROCKET_FRLG` "TEAM ROCKET", `COOLTRAINER_FRLG` "COOLTRAINER", `ELITE_FOUR_FRLG` "ELITE FOUR", `GENTLEMAN_FRLG` "GENTLEMAN", `RIVAL_LATE_FRLG` "RIVAL", `CHAMPION_FRLG` "CHAMPION", `CHANNELER_FRLG` "CHANNELER", `TWINS_FRLG` "TWINS", `COOL_COUPLE_FRLG` "COOL COUPLE", `YOUNG_COUPLE_FRLG` "YOUNG COUPLE", `CRUSH_KIN_FRLG` "CRUSH KIN", `SIS_AND_BRO_FRLG` "SIS AND BRO", `PKMN_PROF_FRLG` "{PKMN} PROF.", `PLAYER_FRLG` "{PKMN} TRAINER", `CRUSH_GIRL_FRLG` "CRUSH GIRL", `TUBER_FRLG` "TUBER", `PKMN_BREEDER_FRLG` "{PKMN} BREEDER", `PKMN_RANGER_FRLG` "{PKMN} RANGER", `AROMA_LADY_FRLG` "AROMA LADY", `RUIN_MANIAC_FRLG` "RUIN MANIAC", `LADY_FRLG` "LADY", `PAINTER_FRLG` "PAINTER".

Villain classes to rename for the new organisation: `TEAM AQUA`, `TEAM MAGMA`, `AQUA ADMIN`, `MAGMA ADMIN`, `AQUA LEADER`, `MAGMA LEADER` (all ≤ 12 bytes; e.g. an Italian "CAPO ___" or "RECLUTA" must still fit 12). `RIVAL` displays "{PKMN} TRAINER", so the rival, Wally and Steven show no special title.

## 4. Characters in dialogue

### 4.1 Speakers with a `NAME: ` prefix

Search: every `.string "` line in Hoenn files that **starts** with an upper-case name (or `{STR_VAR_1}`) followed by `: `. A prefix in the middle of a line does not occur in Hoenn texts (checked). The prefix is plain text: renaming a speaker means editing every line in the list. Length: no engine limit, but the prefix eats into the 216 px line.

Alternative: the expansion has a **name box** (`setspeaker <string or SP_NAME_*>`, `asm/macros/event.inc:2849`; table `gSpeakerNamesTable` in `src/data/speaker_names.h`, which holds only `MOM` and `{PLAYER}`; settings in `include/config/name_box.h`). Using it means adding a script command before each message, which is a script edit, not a text edit.

| Prefix | Lines | Files | Role | Where (file (lines)) |
|---|---|---|---|---|
| `MAY:` | 56 | 12 | rival (male player) | data/text/match_call.inc(16), LittlerootTown_ProfessorBirchsLab(7), Route104(6), RustboroCity(6), LilycoveCity(5), EverGrandeCity_ChampionsRoom(3), Route110(3), Route119(3), LavaridgeTown(2), LittlerootTown_MaysHouse_2F(2), Route103(2), OldaleTown(1) |
| `BRENDAN:` | 56 | 12 | rival (female player) | data/text/match_call.inc(16), LittlerootTown_ProfessorBirchsLab(7), Route104(6), RustboroCity(6), LilycoveCity(5), EverGrandeCity_ChampionsRoom(3), Route110(3), Route119(3), LavaridgeTown(2), LittlerootTown_MaysHouse_2F(2), Route103(2), OldaleTown(1) |
| `MC:` | 47 | 4 | contest / TV host | data/text/tv.inc(33), data/scripts/contest_hall.inc(10), LilycoveCity_ContestHall(3), SlateportCity_BattleTentCorridor(1) |
| `TATE:` | 42 | 2 | gym 7 (twin) | MossdeepCity_Gym(29), data/text/match_call.inc(13) |
| `LIZA:` | 41 | 2 | gym 7 (twin) | MossdeepCity_Gym(28), data/text/match_call.inc(13) |
| `SCOTT:` | 36 | 14 | scout, Frontier owner | BattleFrontier_ScottsHouse(9), data/text/match_call.inc(6), SlateportCity(5), FallarborTown_BattleTentLobby(2), LilycoveCity_CoveLilyMotel_2F(2), Route119(2), RustboroCity_PokemonSchool(2), VerdanturfTown_BattleTentLobby(2), BattleFrontier_ReceptionGate(1), EverGrandeCity_PokemonCenter_1F(1), LittlerootTown_ProfessorBirchsLab(1), MauvilleCity(1), MossdeepCity(1), SSTidalCorridor(1) |
| `STEVEN:` | 33 | 10 | Devon heir / ex-champion | SootopolisCity(7), Route120(6), data/text/match_call.inc(6), MossdeepCity_SpaceCenter_2F(4), MossdeepCity_StevensHouse(3), GraniteCave_StevensRoom(2), MeteorFalls_StevensCave(2), MossdeepCity_SpaceCenter_1F(1), Route118(1), Route128(1) |
| `DAD:` | 32 | 5 | Norman, gym 5 | PetalburgCity_Gym(19), data/text/match_call.inc(9), LittlerootTown_BrendansHouse_1F(2), Route105(1), data/text/berries.inc(1) |
| `PROF. BIRCH:` | 29 | 7 | professor | LittlerootTown_ProfessorBirchsLab(20), EverGrandeCity_ChampionsRoom(2), Route110(2), data/text/match_call.inc(2), LittlerootTown(1), Route101(1), data/text/pokedex_rating.inc(1) |
| `WALLY:` | 27 | 6 | friend / late rival | MauvilleCity(9), data/text/match_call.inc(6), PetalburgCity_Gym(4), VictoryRoad_1F(4), Route102(2), VerdanturfTown_WandasHouse(2) |
| `MOM:` | 26 | 5 | player's mother | LittlerootTown_BrendansHouse_1F(16), LittlerootTown(4), data/text/match_call.inc(3), data/event_scripts.s(2), LittlerootTown_BrendansHouse_2F(1) |
| `MR. BRINEY:` | 25 | 5 | retired sailor, boat | DewfordTown(9), Route104_MrBrineysHouse(7), Route109(7), SSTidalCorridor(1), SlateportCity_SternsShipyard_1F(1) |
| `FANS:` | 23 | 1 | TV | data/text/tv.inc(23) |
| `ARCHIE:` | 21 | 7 | Aqua leader | SeafloorCavern_Room9(10), MeteorFalls_1F_1R(3), MtChimney(3), SootopolisCity(2), MtPyre_Summit(1), Route128(1), SlateportCity_Harbor(1) |
| `WALLACE:` | 20 | 6 | champion / Sootopolis | CaveOfOrigin_B1F(5), EverGrandeCity_ChampionsRoom(4), SkyPillar_Outside(4), SootopolisCity(4), EverGrandeCity_HallOfFame(2), data/text/match_call.inc(1) |
| `MAXIE:` | 19 | 7 | Magma leader | MagmaHideout_4F(4), MossdeepCity_SpaceCenter_2F(4), Route128(3), SeafloorCavern_Room9(3), MtChimney(2), SootopolisCity(2), MtPyre_Summit(1) |
| `CAPT. STERN:` | 18 | 3 | Slateport researcher, submarine | SlateportCity_Harbor(12), SlateportCity(3), SlateportCity_OceanicMuseum_2F(3) |
| `MR. STONE:` | 15 | 2 | Devon president | data/text/match_call.inc(8), RustboroCity_DevonCorp_3F(7) |
| `BIG SIS:` | 15 | 1 | TV | data/text/tv.inc(15) |
| `BIG BRO:` | 15 | 1 | TV | data/text/tv.inc(15) |
| `GABBY:` | 14 | 2 | TV interviewer | data/text/tv.inc(12), SlateportCity(2) |
| `INTERVIEWER:` | 11 | 2 | TV | data/text/tv.inc(10), LittlerootTown_BrendansHouse_1F(1) |
| `WATTSON:` | 11 | 3 | gym 3 | MauvilleCity(4), data/text/match_call.inc(4), MauvilleCity_Gym(3) |
| `KIRA:` | 9 | 1 | Young Couple (with DAN) | AbandonedShip_Rooms2_1F(9) |
| `JOHN:` | 9 | 1 | Old Couple (with JAY) | MeteorFalls_1F_2R(9) |
| `LIV:` | 9 | 1 | Twins (AMY & LIV) | data/text/trainers.inc(9) |
| `ANNA:` | 9 | 1 | Sr. and Jr. (ANNA & MEG) | data/text/trainers.inc(9) |
| `ROY:` | 9 | 1 | Sis and Bro (LILA & ROY) | data/text/trainers.inc(9) |
| `DAN:` | 8 | 1 | Young Couple | AbandonedShip_Rooms2_1F(8) |
| `JAY:` | 8 | 1 | Old Couple | MeteorFalls_1F_2R(8) |
| `ROXANNE:` | 8 | 2 | gym 1 | RustboroCity_Gym(4), data/text/match_call.inc(4) |
| `AMY:` | 8 | 1 | Twins | data/text/trainers.inc(8) |
| `MEG:` | 8 | 1 | Sr. and Jr. | data/text/trainers.inc(8) |
| `LILA:` | 8 | 1 | Sis and Bro | data/text/trainers.inc(8) |
| `CLERK:` | 8 | 1 | TV | data/text/tv.inc(8) |
| `LEADER:` | 7 | 7 | gym statue signs ("LEADER: X") | DewfordTown(1), FortreeCity(1), LavaridgeTown(1), MauvilleCity(1), PetalburgCity(1), RustboroCity(1), SootopolisCity(1) |
| `BRAWLY:` | 7 | 2 | gym 2 | data/text/match_call.inc(4), DewfordTown_Gym(3) |
| `WINONA:` | 7 | 2 | gym 6 | data/text/match_call.inc(4), FortreeCity_Gym(3) |
| `FLANNERY:` | 7 | 2 | gym 4 | data/text/match_call.inc(4), LavaridgeTown_Gym_1F(3) |
| `UNCLE:` | 7 | 2 | Wally's uncle | MauvilleCity(5), VerdanturfTown_WandasHouse(2) |
| `RYDEL:` | 7 | 1 | bike shop owner | MauvilleCity_BikeShop(7) |
| `TY:` | 7 | 2 | TV cameraman | data/text/tv.inc(6), SlateportCity(1) |
| `JUAN:` | 7 | 2 | gym 8 | data/text/match_call.inc(4), SootopolisCity_Gym_1F(3) |
| `REFEREE:` | 6 | 1 | Battle Arena | BattleFrontier_BattleArenaBattleRoom(6) |
| `SPENSER:` | 6 | 1 | Frontier Brain | BattleFrontier_BattlePalaceBattleRoom(6) |
| `GUIDE:` | 6 | 2 | Frontier guide / TV | data/text/tv.inc(5), BattleFrontier_ReceptionGate(1) |
| `ZIGZAGOON:` | 6 | 6 | Pokémon cry | DewfordTown_House1(1), FortreeCity_House1(1), FortreeCity_House5(1), LavaridgeTown_House(1), Route109(1), SlateportCity_PokemonFanClub(1) |
| `PROF. COZMO:` | 6 | 2 | Fallarbor scientist (Meteorite) | FallarborTown_CozmosHouse(5), MeteorFalls_1F_1R(1) |
| `BEAUTY:` | 6 | 1 | TV | data/text/tv.inc(6) |
| `SKITTY:` | 5 | 5 | Pokémon cry | BattleFrontier_PokemonCenter_1F(1), FallarborTown_Mart(1), MossdeepCity_House4(1), RustboroCity_Flat2_1F(1), SlateportCity_PokemonFanClub(1) |
| `JUDGE:` | 5 | 3 | contest judge | LilycoveCity_ContestHall(3), SlateportCity_BattleTentCorridor(1), data/scripts/contest_hall.inc(1) |
| `{STR_VAR_1}:` | 5 | 1 | Lilycove lady (buffered name) | data/scripts/lilycove_lady.inc(5) |
| `GRETA:` | 4 | 1 | Frontier Brain | BattleFrontier_BattleArenaBattleRoom(4) |
| `TUCKER:` | 4 | 1 | Frontier Brain | BattleFrontier_BattleDomeBattleRoom(4) |
| `LUKE:` | 4 | 1 | Young Couple (DEZ & LUKE) | MtPyre_2F(4) |
| `DEZ:` | 4 | 1 | Young Couple | MtPyre_2F(4) |
| `PEEKO:` | 4 | 3 | Briney's Wingull | RusturfTunnel(2), Route104_MrBrineysHouse(1), SSTidalCorridor(1) |
| `JED:` | 4 | 1 | Young Couple (LEA & JED) | SSTidalRooms(4) |
| `LEA:` | 4 | 1 | Young Couple | SSTidalRooms(4) |
| `DOCK:` | 4 | 1 | shipyard worker | SlateportCity_SternsShipyard_1F(4) |
| `GINA:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `MIA:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `LISA:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `RAY:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `PAUL:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `MEL:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `TORI:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `TIA:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `TYRA:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `IVY:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `KATE:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `JOY:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `MIU:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `YUKI:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `KIM:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `IRIS:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `RELI:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `IAN:` | 4 | 1 |  | data/text/trainers.inc(4) |
| `ANNOUNCER:` | 4 | 1 | TV | data/text/tv.inc(4) |
| `REPORTER:` | 4 | 1 | TV | data/text/tv.inc(4) |
| `NOLAND:` | 3 | 1 | Frontier Brain | BattleFrontier_BattleFactoryBattleRoom(3) |
| `LUCY:` | 3 | 1 | Frontier Brain | BattleFrontier_BattlePikeRoomNormal(3) |
| `BRANDON:` | 3 | 1 | Frontier Brain | BattleFrontier_BattlePyramidTop(3) |
| `ANABEL:` | 3 | 1 | Frontier Brain | BattleFrontier_BattleTowerBattleRoom(3) |
| `AZURILL:` | 3 | 2 |  | PacifidlogTown_House2(2), FallarborTown(1) |
| `WINGULL:` | 3 | 3 |  | FortreeCity_House4(1), MossdeepCity_House2(1), Route119_House(1) |
| `AZUMARILL:` | 3 | 3 |  | LilycoveCity_DepartmentStore_1F(1), SlateportCity_PokemonFanClub(1), SootopolisCity_House4(1) |
| `WANDA:` | 3 | 2 | Wally's cousin | VerdanturfTown_WandasHouse(2), RusturfTunnel(1) |
| `KECLEON:` | 2 | 2 |  | LilycoveCity_House1(1), SootopolisCity_House1(1) |
| `BLEND MASTER:` | 2 | 1 | berry blender NPC | data/text/blend_master.inc(2) |
| `OAK:` | 2 | 1 | Kanto professor (dex rating) | data/text/pokedex_rating.inc(2) |
| `GULPIN:` | 2 | 1 |  | data/text/tv.inc(2) |
| `GURU:` | 2 | 1 | TV | data/text/tv.inc(2) |
| `LANETTE:` | 1 | 1 | PC system author | Route114_LanettesHouse(1) |
| `PEKACHU:` | 1 | 1 | nicknamed Pikachu | RustboroCity_House3(1) |
| `PIKACHU:` | 1 | 1 |  | VerdanturfTown_FriendshipRatersHouse(1) |
| `ERROR 404:` | 1 | 1 | follower joke text | data/scripts/follower.inc(1) |
| `BRINEY:` | 1 | 1 | event ticket text | data/text/event_ticket_2.inc(1) |
| `SIDNEY:` | 1 | 1 | E4 | data/text/match_call.inc(1) |
| `PHOEBE:` | 1 | 1 | E4 | data/text/match_call.inc(1) |
| `GLACIA:` | 1 | 1 | E4 | data/text/match_call.inc(1) |
| `DRAKE:` | 1 | 1 | E4 | data/text/match_call.inc(1) |

Excluded as not being speakers: `YOUR PC STATUS:` (Trick House), `ROOFTOP:` (store directory), `LEADERS:` (Mossdeep sign), `POKé BALLS:` (TV). Pokémon "speakers" (`ZIGZAGOON:`, `SKITTY:`, `WINGULL:`, `AZURILL:`, `AZUMARILL:`, `KECLEON:`, `PIKACHU:`, `GULPIN:`) are cries, kept as species names.

Double-battle pairs speak in turn in `data/text/trainers.inc` (`AMY:`/`LIV:`, `ANNA:`/`MEG:`, `LILA:`/`ROY:`, `GINA:`/`MIA:`, `LISA:`/`RAY:`, `PAUL:`/`MEL:`, `TORI:`/`TIA:`, `TYRA:`/`IVY:`, `KATE:`/`JOY:`, `MIU:`/`YUKI:`, `KIM:`/`IRIS:`, `RELI:`/`IAN:`); these single names must stay consistent with the combined `trainers.party` name.

### 4.2 Named characters mentioned without a prefix

Occurrences of the name anywhere inside `.string` lines of Hoenn files (whole word, upper case). Counts include the prefix lines of 4.1 where both exist.

| Name | Occ. | Who | Main files |
|---|---|---|---|
| PROF. BIRCH / BIRCH | 50 | the professor; also `PROF. BIRCH` hard-coded in C (6.3) | LittlerootTown_ProfessorBirchsLab (23), LittlerootTown (8), EverGrandeCity_ChampionsRoom, Route110, match_call.inc |
| MR. STONE / PRESIDENT / PRESIDENT STONE | 18 / 14 / 1 | Devon president, Steven's father | RustboroCity_DevonCorp_3F, RustboroCity, match_call.inc, RustboroCity_Flat2_3F |
| STEVEN / STEVEN STONE | 50 / 1 | Devon heir, ex-champion | match_call.inc (10), Route120 (7), SootopolisCity (7), MossdeepCity_StevensHouse, GraniteCave_StevensRoom, MossdeepCity_SpaceCenter_2F |
| DEVON / DEVON CORPORATION / DEVON CORP | 52 | the company | RustboroCity (7), RustboroCity_DevonCorp_1F (7), Route120, RustboroCity_DevonCorp_2F, match_call.inc |
| MR. BRINEY / CAPTAIN BRINEY / CAPT. BRINEY / MR. SEA | 44 / 1 / 1 / 1 | retired sailor with the boat (MR. SEA = Seashore House owner's nickname) | DewfordTown, RustboroCity, Route104_MrBrineysHouse, Route109, SSTidalCorridor, Route109_SeashoreHouse |
| PEEKO | 22 | Briney's Wingull | Route104_MrBrineysHouse, RusturfTunnel, DewfordTown, Route116, RustboroCity |
| CAPT. STERN / STERN | 40 / 48 | Slateport ocean researcher, submarine | SlateportCity_Harbor (14), SlateportCity (13), SlateportCity_OceanicMuseum_2F, SlateportCity_SternsShipyard_1F |
| DOCK | 7 | shipyard engineer | SlateportCity_SternsShipyard_1F |
| SCOTT / MR. SCOTT | 53 | talent scout, Battle Frontier owner | BattleFrontier_ScottsHouse, BattleFrontier_Lounge2, SlateportCity, match_call.inc |
| WALLY | 67 | friend / late rival | MauvilleCity (16), PetalburgCity_WallysHouse (13), VerdanturfTown_WandasHouse (12), PetalburgCity_Gym (9) |
| UNCLE / WANDA | 11 / 6 | Wally's uncle and cousin | MauvilleCity, VerdanturfTown_WandasHouse, RusturfTunnel |
| DAD / NORMAN / DAD NORMAN | 40 / 12 / 1 | player's father (gym 5) | PetalburgCity_Gym (20), LittlerootTown_BrendansHouse_1F (8), match_call.inc (9), LilycoveCity_PokemonTrainerFanClub |
| MOM | 29 | player's mother | LittlerootTown_BrendansHouse_1F (19), LittlerootTown (4), match_call.inc (3), data/event_scripts.s:1429, 1437 (white-out heal) |
| PROF. COZMO / COZMO | 10 / 11 | meteorite scientist | FallarborTown_CozmosHouse, MeteorFalls_1F_1R |
| LANETTE / BILL | 7 / 4 | PC system authors ("LANETTE'S PC", "BILL'S PC") | Route114_LanettesHouse, data/text/pc.inc, pc_transfer.inc; C: `src/strings.c:418-419`, `src/battle_message.c:179-180` |
| RYDEL | 59 | bike shop owner (Cycling Road) | Route110 (47), MauvilleCity_BikeShop |
| TRICK MASTER / MECHADOLL | 15 / 23 | Trick House | Route110_TrickHouse* |
| CUTTER | 5 | HM Cut giver ("CUTTER'S HOUSE") | RustboroCity, match_call.inc, move_tutors.inc |
| WALDA / PEPPER | 4 / 1 | girl and family of the wallpaper phrase | RustboroCity_Flat1_2F |
| KIRI | 3 | berry girl | data/text/berries.inc |
| TEALA | 2 | Wireless Club attendant | data/text/cable_club.inc |
| GABBY / TY / INTERVIEWER | 14 / 7 / 11 | TV crew | data/text/tv.inc, SlateportCity |
| ENERGY GURU / NAME RATER / MOVE DELETER / BERRY MASTER / BLEND MASTER / CURATOR / FOSSIL MANIAC | small | title-only NPCs | SlateportCity, LilycoveCity, Route123, Route114, LilycoveCity_LilycoveMuseum_1F |
| DEVON RESEARCHER / SUBMARINE EXPLORER | 3 / 3 | role names | PetalburgWoods, RustboroCity_DevonCorp_2F, SlateportCity_Harbor |
| Mauville man / Lilycove lady roles: GIDDY, BARD, HIPSTER, STORYTELLER, TRADER, FAVOR LADY, QUIZ LADY, CONTEST LADY | 1 each | record-mixing NPCs | data/scripts/mauville_man.inc, lilycove_lady.inc |
| KIRLIA, MIMI, ZIGG, PEKACHU, WAI | few | Pokémon nicknames / sounds | BattleFrontier_*, RustboroCity_House3, MossdeepCity |
| GROU… | 2 | Maxie stammering "GROU…DON" | MagmaHideout_2F_2R, match_call.inc |
| BRUNO, OAK / PROF. OAK, KANTO, INDIGO PLATEAU, S.S. ANNE | 1–3 | Kanto references | match_call.inc, pokedex_rating.inc, LilycoveCity, flavor_text.inc, SlateportCity_OceanicMuseum_2F |
| S.S. TIDAL / S.S. CACTUS | 12 / 1 | ships (ferry, Abandoned Ship) | SlateportCity, LilycoveCity, AbandonedShip_CaptainsOffice |
| CLEANUP BROTHERS | 2 | S.S. Tidal crew | SSTidalLowerDeck |
| GAME FREAK | 1 | developer room | LilycoveCity_CoveLilyMotel_1F |
| TEAM AQUA / TEAM MAGMA (+ bare AQUA / MAGMA) | 65 / 67 (70 / 73) | villain organisations | TEAM AQUA in 27 files (LilycoveCity 9, AquaHideout_1F, MtChimney, PetalburgWoods, Route110 …); TEAM MAGMA in 20 files (MeteorFalls_1F_1R 10, MtChimney 9, MtPyre_Summit 7, MossdeepCity_SpaceCenter_1F/2F …) |
| ARCHIE / MAXIE | 29 / 26 | villain bosses | SeafloorCavern_Room9 (11), MagmaHideout_4F (7), MtChimney, Route128, SootopolisCity, MossdeepCity_SpaceCenter_2F |
| Admins SHELLY / MATT / TABITHA | 0 | never named in dialogue (they speak without a prefix) | — |
| KYOGRE / GROUDON / RAYQUAZA | 17 / 23 / 8 | scripted legendaries (species names, keep) | SeafloorCavern_Room9, SootopolisCity, MagmaHideout_4F, CaveOfOrigin_B1F, SkyPillar |
| Gym leaders by name (ROXANNE … JUAN) | 20–47 each | see 2.1 | their gym map, match_call.inc, LilycoveCity_PokemonTrainerFanClub |
| Elite Four by name | 2 each | see 2.1 | their room + match_call.inc |
| Badges: STONE / KNUCKLE / DYNAMO / HEAT / BALANCE / FEATHER / MIND / RAIN BADGE | 7/2/2/2/1/2/2/6 | badge names exist **only** in dialogue (no C string); IT: Medaglia Pietra / Pugno / Dinamo / Fiamma / Armonia / Piuma / Mente / Pioggia | each gym map; Rustboro school, Sootopolis |

### 4.3 Region, version and organisation words in dialogue

| Word | Occ. | Files | Note |
|---|---|---|---|
| HOENN | 56 | 33 | LittlerootTown_ProfessorBirchsLab (5), MtPyre_Summit (4), SootopolisCity_Gym_1F (4), tv.inc (4), SootopolisCity (3), safari_zone.inc (3) … The `{REGION}` placeholder (→ `gText_Hoenn`) is used **once** (`LittlerootTown_BrendansHouse_2F/scripts.inc:296`); every other HOENN is literal. |
| EMERALD / Emerald | 2 / 6 | 2 | `data/scripts/contest_hall.inc:1700-1735` (E-MODE explanation: 2 + 5), `data/text/cable_club.inc:169` (1). `{VERSION}` (→ "EMERALD") is never used. |
| `{AQUA}` `{MAGMA}` `{ARCHIE}` `{MAXIE}` `{KYOGRE}` `{GROUDON}` | 0 in texts | — | Only `{AQUA}` appears, in map section 66 (unused). All villain names in dialogue are literal text. |
| `{RIVAL}` | 19 | 5 | LittlerootTown_ProfessorBirchsLab (12), LittlerootTown_MaysHouse_1F (3), Route110 (2), LittlerootTown_MaysHouse_2F (1), EverGrandeCity_ChampionsRoom (1). Elsewhere the rival is written literally as MAY / BRENDAN. |
| `{KUN}` | 398 | 45 maps + data/text | expands to an empty string in English (`src/strings.c:8-9`); can be left or deleted |
| RED ORB / BLUE ORB | 12 / 10 | 3 / 3 | MtPyre_Summit, SeafloorCavern_Room9, MagmaHideout_4F |
| METEORITE | 22 | 6 | MtChimney (11), MeteorFalls_1F_1R (5), FallarborTown_CozmosHouse (3) |
| SPACE CENTER / WEATHER INSTITUTE | 11 / 6 | 4 / 4 | MossdeepCity, Route119 |
| LITTLEROOT / SOOTOPOLIS (other town names similar) | 10 / 41 | 7 / 21 | place names are literal everywhere |

## 5. Story items — `src/data/items.h`

* **Limit: `ITEM_NAME_LENGTH` = 20** (`include/constants/global.h:154`), enforced at compile time by `ITEM_NAME(str)` = `COMPOUND_STRING_SIZE_LIMIT(str, ITEM_NAME_LENGTH)` (`src/data/items.h:17`). `pluralName` has 22 (`ITEM_PLURAL_NAME`).
* **Description**: 3 lines separated by `\n` in the bag window `WIN_DESCRIPTION` (14 tiles = 112 px wide, `src/item_menu.c:437`). Keep each line ≤ ~18 characters, as vanilla does.
* Item names are **Title Case** in the expansion ("Devon Parts"), while dialogue writes them in capitals ("DEVON GOODS"). The "Obtained the {STR_VAR_2}!" message (`data/text/obtain_item.inc:1`) inserts the `items.h` name, so the player sees both styles (section 7).
* Aliases in `include/constants/items.h`: `ITEM_DEVON_GOODS` = `ITEM_DEVON_PARTS` (line 896, "Pre-Gen VI name"), `ITEM_ITEMFINDER` = `ITEM_DOWSING_MACHINE` (870), `ITEM_EXP_ALL` = `ITEM_EXP_SHARE` (588). Scripts may use either constant.
* "Given / checked" = commands in Hoenn scripts (`giveitem`, `checkitem`, `removeitem`, `additem`, `finditem`); "item ball" = placed as an object in `map.json`.

| `ITEM_…` | items.h line | Name | len | Description (`\n` = new line) | Given / checked in | Story words in the description |
|---|---|---|---|---|---|---|
| `LETTER` | 14422 | Letter | 6 | A letter to Steven\nfrom the President\nof the Devon Corp. | giveitem RustboroCity_DevonCorp_3F:50 (Mr. Stone); handed over GraniteCave_StevensRoom:8 (setvar VAR_0x8004, ITEM_LETTER) | Steven, President, Devon Corp |
| `DEVON_PARTS` | 14438 | Devon Parts | 11 | A package that\ncontains Devon's\nmachine parts. | giveitem RusturfTunnel:306 (recovered from grunt); handed over SlateportCity_OceanicMuseum_2F:69. Dialogue calls it "DEVON GOODS" (8×) | Devon |
| `EXP_SHARE` | 9794 | Exp. Share | 10 | This device allows\nall party members\nto share in Exp. | giveitem RustboroCity_DevonCorp_3F:163 (Mr. Stone, after the letter) |  |
| `METEORITE` | 14604 | Meteorite | 9 | A rock that allows\na certain Pokémon\nto change Formes. | giveitem MtChimney (after Maxie leaves); checkitem FallarborTown_CozmosHouse | expansion text is about Deoxys forms; dialogue: Cozmo's meteorite |
| `MAGMA_EMBLEM` | 14620 | Magma Emblem | 12 | A medal-like item in\nthe same shape as\nTeam Magma's mark. | giveitem MtPyre_Summit:60 (old lady, after both orbs are taken); checkitem JaggedPass (opens the Magma Hideout) | Team Magma's mark |
| `RED_ORB` | 5618 | Red Orb | 7 | A red, glowing orb\nsaid to contain an\nancient power. | not given in Emerald; named in dialogue (stolen at MtPyre_Summit) | held item for Primal Groudon in the expansion |
| `BLUE_ORB` | 5635 | Blue Orb | 8 | A blue, glowing orb\nsaid to contain an\nancient power. | not given in Emerald; named in dialogue | held item for Primal Kyogre |
| `GO_GOGGLES` | 14455 | Go-Goggles | 10 | Nifty goggles that\nprotect eyes from\ndesert sandstorms. | giveitem LavaridgeTown (rival / Brendan-May); checkitem Route111 (desert) |  |
| `DEVON_SCOPE` | 14472 | Devon Scope | 11 | A device by Devon\nthat signals any\nunseeable Pokémon. | giveitem Route120 (Steven); checkitem FortreeCity, data/scripts/kecleon.inc | Devon |
| `BASEMENT_KEY` | 14488 | Basement Key | 12 | The key for New\nMauville beneath\nMauville City. | giveitem MauvilleCity (Wattson); checkitem NewMauville_Entrance | New Mauville, Mauville City |
| `SCANNER` | 14504 | Scanner | 7 | A device found\ninside the\nAbandoned Ship. | item ball AbandonedShip_HiddenFloorRooms; checkitem AbandonedShip_CaptainsOffice; checkitem + removeitem SlateportCity_Harbor (Stern trades it) | Abandoned Ship |
| `STORAGE_KEY` | 14520 | Storage Key | 11 | The key to the\nstorage inside the\nAbandoned Ship. | item ball AbandonedShip_CaptainsOffice; checkitem/removeitem AbandonedShip_Corridors_B1F | Abandoned Ship |
| `KEY_TO_ROOM_1` | 14536 | Key to Room 1 | 13 | A key that opens a\ndoor inside the\nAbandoned Ship. | item ball AbandonedShip_HiddenFloorRooms; checkitem/removeitem AbandonedShip_HiddenFloorCorridors | Abandoned Ship |
| `KEY_TO_ROOM_2` | 14553 | Key to Room 2 | 13 | A key that opens a\ndoor inside the\nAbandoned Ship. | same | Abandoned Ship |
| `KEY_TO_ROOM_4` | 14570 | Key to Room 4 | 13 | A key that opens a\ndoor inside the\nAbandoned Ship. | same | Abandoned Ship |
| `KEY_TO_ROOM_6` | 14587 | Key to Room 6 | 13 | A key that opens a\ndoor inside the\nAbandoned Ship. | same | Abandoned Ship |
| `ROOT_FOSSIL` | 3256 | Root Fossil | 11 | A fossil of an\nancient, seafloor-\ndwelling Pokémon. | giveitem DesertUnderpass, MirageTower_4F; checkitem Route114_FossilManiacsTunnel; revived RustboroCity_DevonCorp_2F | Bag pocket: Items |
| `CLAW_FOSSIL` | 3279 | Claw Fossil | 11 | A fossil of an\nancient, seafloor-\ndwelling Pokémon. | same as Root Fossil | Bag pocket: Items |
| `SS_TICKET` | 14341 | S.S. Ticket | 11 | The ticket required\nfor sailing on a\nferry. | giveitem data/scripts/players_house.inc:434 (Mom, post-game); checkitem BattleFrontier_OutsideWest |  |
| `EON_TICKET` | 14357 | Eon Ticket | 10 | The ticket for a\nferry to a distant\nsouthern island. | giveitem/checkitem data/scripts/cable_club.inc:34-38 (Mystery Gift); checkitem LilycoveCity_Harbor | "distant southern island" |
| `MYSTIC_TICKET` | 14374 | Mystic Ticket | 13 | A ticket required\nto board the ship\nto Navel Rock. | data/scripts/gift_mystic_ticket.inc; LilycoveCity_Harbor | Navel Rock |
| `AURORA_TICKET` | 14390 | Aurora Ticket | 13 | A ticket required\nto board the ship\nto Birth Island. | data/scripts/gift_aurora_ticket.inc; LilycoveCity_Harbor | Birth Island |
| `OLD_SEA_MAP` | 14406 | Old Sea Map | 11 | A faded sea chart\nthat shows the way\nto a certain island. | data/scripts/gift_old_sea_map.inc; LilycoveCity_Harbor | Faraway Island |
| `CONTEST_PASS` | 14636 | Contest Pass | 12 | The pass required\nfor entering\nPokémon Contests. | only `additem` in LilycoveCity_ContestLobby:357, inside `…_EventScript_SetDebug` ("Functionally unused"); Emerald never gives a pass |  |
| `WAILMER_PAIL` | 14226 | Wailmer Pail | 12 | A tool used for\nwatering Berries\nand plants. | giveitem Route104_PrettyPetalFlowerShop; checkitem data/scripts/berry_tree.inc |  |
| `POKEBLOCK_CASE` | 14258 | {POKEBLOCK} Case | 10 | A case for holding\n{POKEBLOCK}s made with\na Berry Blender. | giveitem data/scripts/contest_hall.inc:17; checkitem berry_blender.inc, lilycove_lady.inc, Route121_SafariZoneEntrance | name built from `{POKEBLOCK}` |
| `SOOT_SACK` | 14274 | Soot Sack | 9 | A sack used to\ngather and hold\nvolcanic ash. | giveitem/checkitem Route113_GlassWorkshop |  |
| `COIN_CASE` | 14195 | Coin Case | 9 | A case that holds\nup to 9,999 Coins. | giveitem MauvilleCity_House2; checkitem MauvilleCity_GameCorner, roulette.inc |  |
| `POWDER_JAR` | 14210 | Powder Jar | 10 | Stores Berry\nPowder made using\na Berry Crusher. | giveitem SlateportCity; checkitem cable_club.inc |  |
| `MACH_BIKE` | 14009 | Mach Bike | 9 | A folding bicycle\nthat doubles your\nspeed or better. | giveitem/checkitem/removeitem MauvilleCity_BikeShop (Rydel; swap) |  |
| `ACRO_BIKE` | 14026 | Acro Bike | 9 | A folding bicycle\ncapable of jumps\nand wheelies. | same |  |
| `BICYCLE` | 13992 | Bike | 4 | A folding bicycle\nthat is faster than\nthe Running Shoes. | debug only in Hoenn |  |
| `DOWSING_MACHINE` | 14094 | Dowsing Machine | 15 | A device that\nsignals an invisible\nitem by sound. | giveitem Route110:461 (rival, after the Route 110 battle). Dialogue: "ITEMFINDER" |  |
| `OLD_ROD` | 14043 | Old Rod | 7 | Use by any body of\nwater to fish for\nwild Pokémon. | giveitem DewfordTown |  |
| `GOOD_ROD` | 14060 | Good Rod | 8 | A decent fishing\nrod for catching\nwild Pokémon. | giveitem Route118 |  |
| `SUPER_ROD` | 14077 | Super Rod | 9 | The best fishing\nrod for catching\nwild Pokémon. | giveitem MossdeepCity_House3 |  |
| `LAVA_COOKIE` | 1151 | Lava Cookie | 11 | A local specialty\nthat heals all\nstatus problems. | giveitem MtChimney (bought for ¥200 from a lady) | Lavaridge specialty |

Story "objects" that are **not items** (flags + strings only): POKéNAV (83 occ. in dialogue; menu strings in `src/pokenav_*.c`), POKéDEX (69), RUNNING SHOES (12, `FLAG_RECEIVED_RUNNING_SHOES`), MATCH CALL (12), FRONTIER PASS (39), TRAINER CARD. Their names live in dialogue and in the C menu files; renaming them is a text job in both.

Bag pocket names: `gPocketNamesStringsTable` (`src/strings.c:142-148`), e.g. "KEY ITEMS"; dialogue refers to "KEY ITEMS POCKET" (3×).

## 6. Story names hard-coded in C

### 6.1 Text placeholders — `src/strings.c:7-22`, expanded in `src/string_util.c:440-550`

| Placeholder (charmap) | String | `src/strings.c` | Expander (`src/string_util.c`) | Used in Hoenn texts |
|---|---|---|---|---|
| `{PLAYER}` FD 01 | save block name (≤ 7) | — | `ExpandPlaceholder_PlayerName` :440 | 933 (100 maps) |
| `{KUN}` FD 05 | "" (male) / "" (female) | :8 Kun, :9 Chan | `ExpandPlaceholder_KunChan` :460 | 398, prints nothing |
| `{RIVAL}` FD 06 | MAY (male player) / BRENDAN (female) | :20 May, :19 Brendan (:21-22 RED/GREEN for FRLG) | `ExpandPlaceholder_RivalName` :468 | 19 (5 maps) |
| `{VERSION}` FD 07 | EMERALD | :12 (:10-11 SAPPHIRE/RUBY unused) | `ExpandPlaceholder_Version` :481 | 0 |
| `{AQUA}` FD 08 | AQUA | :13 | `ExpandPlaceholder_Aqua` :486 | map section 66 only |
| `{MAGMA}` FD 09 | MAGMA | :14 | :491 | 0 |
| `{ARCHIE}` FD 0A | ARCHIE | :15 | :496 | 0 |
| `{MAXIE}` FD 0B | MAXIE | :16 | :501 | 0 |
| `{KYOGRE}` FD 0C | KYOGRE | :17 | :506 | 0 |
| `{GROUDON}` FD 0D | GROUDON | :18 | :511 | 0 |
| `{REGION}` FD 0E | HOENN (`gText_Hoenn` :909) / KANTO (`gText_Kanto` :910) by build | — | `ExpandPlaceholder_Region` :516 | 1 |

Charmap definitions: `charmap.txt:339-354`. Because the villain placeholders are unused, renaming `gText_ExpandedPlaceholder_Aqua/Magma/Archie/Maxie` changes almost nothing on screen; the visible names are the literal ones listed in 4.3, 2.1 and 3.

### 6.2 "HOENN" and "EMERALD" in C

| file:line | String | What it is |
|---|---|---|
| `src/strings.c:904` | "HOENN" | `gText_DexHoenn`, Pokédex mode label |
| `src/strings.c:909` | "HOENN" | `gText_Hoenn`, value of `{REGION}` |
| `src/strings.c:809` | "あらたな トレーナーが\nホウエンに やってきた！" | `gJPText_NewTrainerHasComeToHoenn` (Japanese, unused in EN) |
| `src/pokedex.c:1184` | "HOENN region's POKéDEX", "HOENN DEX" | Pokédex search screen, dex mode option (`sDexModeOptions[DEX_MODE_HOENN]`) |
| `src/pokedex_plus_hgss.c:3830-3840` | "Kanto" "Johto" "Hoenn" "Sinnoh" "Unova" … | HGSS-style dex, evolution condition "in / out of <region>" (`enum Region`, `REGION_*`); one switch with all region names, useful for the multi-region plan |
| `src/pokenav_menu_handler_gfx.c:272` | "Check the map of the HOENN region" | PokéNav main menu help |
| `src/mystery_event_msg.c:11` | "A new TRAINER has arrived in\nHOENN." | Mystery Event message |
| `src/data/pokemon/species_info/gen_1_families.h:3440` | "…across the Hoenn region together." | Pokédex entry (Pikachu Hoenn Cap) |
| `src/data/pokemon/species_info/gen_9_families.h:7141` | "…occurs in Hoenn." | Pokédex entry |
| `src/strings.c:12` | "EMERALD" | `gText_ExpandedPlaceholder_Emerald` (`{VERSION}`) |
| `src/data/credits.h:66` | "POKéMON EMERALD VERSION" | credits title |
| `src/data/easy_chat/easy_chat_group_trainer.h:160` | "EMERALD" | Easy Chat word (also SAPPHIRE :22, RUBY :106) |
| `src/berry_fix_program.c:40` | "Emerald" | Berry Fix program |
| `Makefile:1-4` | `TITLE ?= POKEMON EMER`, `GAME_CODE ?= BPEE` | ROM header (title max 12 chars) |

Graphics that contain the word Hoenn or the Emerald logo (not text): `graphics/pokenav/left_headers/hoenn_map.png`, `graphics/pokedex/*hoenn*`, the title-screen logo (`src/title_screen.c`).

### 6.3 Other character names in C

| file:line | String | Use |
|---|---|---|
| `src/strings.c:53` | "…PROF. BIRCH's POKéDEX rating!\pPROF. BIRCH: Let's see…" | `gText_HOFDexRating` (PC / Hall of Fame) |
| `src/strings.c:63` | "PROF. BIRCH is in trouble!\nRelease a POKéMON and rescue him!" | `gText_BirchInTrouble`, starter bag screen |
| `src/battle_message.c:392` | "PROF. BIRCH: Don't leave me like this!" | `STRINGID_DONTLEAVEBIRCH`, trying to run in the first battle |
| `src/field_screen_effect.c:1471` | "PROF. BIRCH" | white-out at home: buffered into `{STR_VAR_1}` for Mom's "I just heard from {STR_VAR_1}." (`data/event_scripts.s:1439`; FRLG: PROF. OAK) |
| `src/pokenav_match_call_data.c:184-185` | "DEVON PRES" / "MR. STONE" | Match Call desc / name |
| `src/pokenav_match_call_data.c:208-209` | "RELIABLE ONE" / "DAD" |  |
| `src/pokenav_match_call_data.c:229-230` | "{PKMN} PROF." / "PROF. BIRCH" |  |
| `src/pokenav_match_call_data.c:245-246` | "CALM & KIND" / "MOM" |  |
| `src/pokenav_match_call_data.c:260-261` | "HARD AS ROCK" / "STEVEN" |  |
| `src/pokenav_match_call_data.c:274` | "RAD NEIGHBOR" | rival desc (both genders) |
| `src/pokenav_match_call_data.c:336, 360-361` | "{PKMN} LOVER" (Wally) / "ELUSIVE EYES" / "SCOTT" |  |
| `src/pokenav_match_call_data.c:645-656` | Steven / Brendan / May "check page" lines ("Ultimate STEEL POKéMON.", "I'll be a better POKéMON prof than my father is!", "My POKéMON and I help my father's research.") | PokéNav trainer profile |
| `src/strings.c:104` | "DAD's advice…" | `gText_DadsAdvice` (using an item at the wrong time) |
| `src/strings.c:395-396` | "DAD" / "MOM" | `gText_Dad`, `gText_Mom` |
| `src/data/speaker_names.h:3` | "MOM" | name-box speaker table |
| `src/strings.c:418-419` | "LANETTE'S PC" / "BILL'S PC" | PC menu |
| `src/battle_message.c:179-180` | "LANETTE's" / "BILL's" | "sent to … PC" after a catch |
| `src/battle_message.c:1507`, `:3559` | "WALLY" | see 2.3 |
| `src/field_specials.c:184-189` | "WALLACE" "STEVEN" "BRAWLY" "WINONA" "PHOEBE" "GLACIA" | `BufferFanClubTrainerName_` (Lilycove fan club, record mixing) |
| `src/trainer_fan_club.c:292-310` | `gText_ExpandedPlaceholder_May/Brendan` | fan club: rival name by gender |
| `src/match_call.c:1710` | "GABBY" | Match Call trainer name override |
| `src/main_menu.c:480-524` | preset names (`sMalePresetNames`: STU, MILTON, TOM, KENNY, REID, JUDE, JAXSON, EASTON, WALKER, TERU, JOHNNY, BRETT, SETH, TERRY, CASEY, DARREN, LANDON, COLLIN, STANLEY, QUINCY; female list KIMMY…HALIE) | random default player name before the naming screen (`NewGameBirchSpeech_SetDefaultPlayerName`, :2136) |
| `src/quickstart.c:82-83` | "BRENDAN" / "MAY" | debug quick-start player name |
| `src/data/contest_opponents.h:2285` | "DEVON" | a contest opponent's trainer name |
| `src/mystery_event_msg.c:3` | "…Dad has it at PETALBURG GYM." | Mystery Event berry |

### 6.4 New-game intro speech

* Text: `data/text/birch_speech.inc` (included from `data/event_scripts.s:1736`), labels `gText_Birch_Welcome` (line 1, "My name is BIRCH."), `gText_Birch_Pokemon` (9), `gText_Birch_MainSpeech` (14), `gText_Birch_AndYouAre` (31), `gText_Birch_BoyOrGirl` (34), `gText_Birch_WhatsYourName` (38), `gText_Birch_SoItsPlayer` (42), `gText_Birch_YourePlayer` (45, "moving to my hometown of LITTLEROOT"), `gText_Birch_AreYouReady` (52, "Come see me in my POKéMON LAB."). Declared in `include/strings.h:204-212`.
* Code: `src/main_menu.c`. The task chain is documented at lines 110-160 and implemented from `Task_NewGameBirchSpeech_Init` (:1297) to `Task_NewGameBirchSpeech_Cleanup`. Sprites: Birch, Lotad (the "This is a POKéMON" mon), Brendan and May front pics (`tBrendanSpriteId`, `tMaySpriteId`). The new protagonist's front pic replaces the Brendan pic used here.
* The Emerald build calls this from the main menu `ACTION_NEW_GAME` (:1078-1094); the FRLG build uses `src/oak_speech.c` instead.

### 6.5 Player gender: where it is chosen and how to force a male protagonist

**Where it is chosen.** `gSaveBlock2Ptr->playerGender` (`MALE` = 0, `FEMALE` = 1) is set only in `Task_NewGameBirchSpeech_ChooseGender` (`src/main_menu.c:1533-1563`), from the BOY/GIRL menu shown by `Task_NewGameBirchSpeech_BoyOrGirl` (:1516) → `Task_NewGameBirchSpeech_WaitToShowGenderMenu` (:1524). Moving the cursor swaps the Brendan/May sprite (`Task_NewGameBirchSpeech_SlideOutOldGenderSprite` :1565, `…SlideInNewGenderSprite` :1589). Answering NO to "So it's {PLAYER}?" goes back to the gender question (`Task_NewGameBirchSpeech_ProcessNameYesNoMenu` :1674). The only other writer is the debug menu (`src/debug.c:1982-1985`).

**Caveat.** The save block is cleared only when there is no valid save (`src/intro.c:1148` → `Sav2_ClearSetDefault`, `src/new_game.c:145`). Starting a New Game over an existing save keeps the old `playerGender` until the menu writes it. So forcing male needs an explicit assignment; skipping the menu alone is not enough.

**Minimal change (C only, no script changes):**

1. `Task_NewGameBirchSpeech_WaitForPlayerFadeIn` (`src/main_menu.c:1507-1514`): add `gSaveBlock2Ptr->playerGender = MALE;` and set `gTasks[taskId].func = Task_NewGameBirchSpeech_WhatsYourName;` instead of `…_BoyOrGirl`. The male sprite is already the one shown (`tPlayerGender = MALE`, `tPlayerSpriteId = tBrendanSpriteId` at :1492-1499).
2. `Task_NewGameBirchSpeech_ProcessNameYesNoMenu` (:1660-1676): on NO / B, go to `Task_NewGameBirchSpeech_WhatsYourName` instead of `Task_NewGameBirchSpeech_BoyOrGirl`.
3. Result: `Task_NewGameBirchSpeech_BoyOrGirl` is no longer referenced, so GCC prints an unused-function warning; this is not an error by default (`UNUSED_ERROR ?= 0`, `Makefile:39, 182-184`). Delete it or keep it. The gender menu, `gText_Birch_BoyOrGirl` and the two slide tasks become dead code. `NewGameBirchSpeech_SetDefaultPlayerName` (:2136) then always picks from `sMalePresetNames`, and the naming screen gets `MALE` (:1639).
4. Optional: `src/debug.c:1982` (gender toggle) and `src/quickstart.c:82-83` for testing builds.

**How the rival follows from the gender** (nothing to change; with a male player every branch picks May):

| Aspect | Mechanism | Male player gets |
|---|---|---|
| Name in text `{RIVAL}` | `ExpandPlaceholder_RivalName`, `src/string_util.c:468-479` | `gText_ExpandedPlaceholder_May` "MAY" (`src/strings.c:20`) |
| Overworld sprite | `Common_EventScript_SetupRivalGfxId` / `…OnBikeGfxId` (`data/scripts/rival_graphics.inc`) → `VAR_OBJ_GFX_ID_0` / `VAR_OBJ_GFX_ID_3` | `OBJ_EVENT_GFX_RIVAL_MAY_NORMAL`, `…_MAY_MACH_BIKE` |
| Battles | `checkplayergender` in the rival scenes, then `VAR_STARTER_MON` picks the party | `TRAINER_MAY_ROUTE_103_*`, `_RUSTBORO_*`, `_ROUTE_110_*`, `_ROUTE_119_*`, `_LILYCOVE_*` |
| Houses | `InsideOfTruck/scripts.inc:19-21` sets intro flags by gender; `data/scripts/players_house.inc:64, 418` | player lives in `LittlerootTown_BrendansHouse_*`, rival in `LittlerootTown_MaysHouse_*` |
| PokéNav | `sMayMatchCallHeader` `.playerGender = MALE` (`src/pokenav_match_call_data.c:276-300`), filter at :768 | May's 15 Match Call texts (`MatchCall_Text_May1…15`, `data/text/match_call.inc`) |
| Fan club | `src/trainer_fan_club.c:291, 307` | May |
| Player's-house TV | `src/tv.c:3150, 3175` (which house counts as the player's) | `LittlerootTown_BrendansHouse_1F` |
| Player avatar | `sPlayerAvatarGfxIds[][GENDER_COUNT]` (`src/field_player_avatar.c:273`), `sRivalAvatarGfxIds` (:260) | the `[MALE]` column = new protagonist sprites |

`checkplayergender` appears **59 times in 31 Hoenn maps** (LittlerootTown 9, ProfessorBirchsLab 6, MaysHouse_2F 6, BrendansHouse_2F 4, EverGrandeCity_ChampionsRoom 3, RustboroCity / Route119 / Route110 / Route104 / LavaridgeTown 2 each, 21 others 1 each, mostly Battle Frontier rooms) plus 5 in `data/scripts` (`rival_graphics.inc` 3, `players_house.inc` 2). With the gender forced, every `FEMALE` branch is unreachable: the BRENDAN-as-rival texts (60 occurrences, mirroring the 60 MAY ones, plus `MatchCall_Text_Brendan1…15`) and the female-player variants **need no rewriting**. This changes the rule in `segmento_A.md` §0 ("writers must provide text for both rivals"): only the male-player / MAY branch is needed.

Renaming the rival = `src/strings.c:20` + the 15 `TRAINER_MAY_*` names in `trainers.party` + the literal "MAY" in the male-branch texts (`MAY:` prefixes, 40 lines in maps + 16 in `match_call.inc`). Keep it ≤ 7 characters: `tools/textcheck` budgets `{RIVAL}` at the width of the widest name in `src/strings.c` (BRENDAN, 42 px).

## 7. Capitalization style

Two styles coexist in the expansion, and the player sees both on the same screen.

**Dialogue (`data/maps`, `data/scripts`, `data/text`)** follows Gen 3: sentences in normal case, but **every proper noun and game term in CAPITALS**: people, places, items, species, moves, types, organisations, game systems. Counts in Hoenn files: "POKéMON" 3,499 vs "Pokémon" 13.

```
data/maps/RustboroCity/scripts.inc:1103        "I have to get the DEVON GOODS back!\p"
data/maps/PetalburgWoods/scripts.inc:345       "No one who crosses TEAM AQUA\n"
data/maps/LittlerootTown/scripts.inc:924       "introduce yourself to PROF. BIRCH?\p"
data/maps/LittlerootTown_ProfessorBirchsLab/scripts.inc:774  "them with POKé BALLS!$"
data/maps/RustboroCity_Gym/scripts.inc:119     "ROXANNE, the GYM LEADER, is a user\n"
```

**Engine strings (`src/`)** are half converted to the modern style: "Pokémon" 1,101 vs "POKéMON" 334 in `src/`.

* Title Case (modern): species (`.speciesName = _("Treecko")`, `src/data/pokemon/species_info/gen_3_families.h:26`), moves (`"Karate Chop"`, `src/data/moves_info.h:69`), items (`"Devon Parts"`, `"Poké Ball"`, `src/data/items.h`), most battle messages (`"Oh no! The Pokémon broke free!"`, `src/battle_message.c:422`). Some lines carry the comment `//no decapitalize until it is everywhere` (e.g. `src/battle_message.c:179, 392`).
* ALL CAPS (legacy, still in place): map section names ("LITTLEROOT TOWN"), landmarks, trainer classes ("TEAM AQUA", "{PKMN} TRAINER"), trainer names ("ROXANNE"), Match Call names and descriptions ("PROF. BIRCH", "ROCKIN' WHIZ"), bag pocket names ("KEY ITEMS"), placeholders (`{RIVAL}` → "MAY", `{REGION}` → "HOENN"), menu strings such as "LANETTE'S PC".
* Mixed results on screen: a capitals sentence receives a Title Case insert. Examples: "Obtained the Devon Parts!" while the NPC says "DEVON GOODS" (`data/text/obtain_item.inc:1` + `items.h`); "You are challenged by LEADER ROXANNE!" in an otherwise sentence-case battle message (`src/battle_message.c:96`); "{PLAYER} put away the Devon Parts in the KEY ITEMS POCKET."
* The player name can be typed in lower case (naming screen), so `{PLAYER}` may be mixed case whatever style is chosen.

**Decision needed for the Italian rewrite** (for the story bible, not decided here). Official Italian Emerald wrote names in capitals ("ALBANOVA", "PROF. BIRCH", "TEAM IDRO"). If the dialogue adopts Title Case ("Albanova", "Prof. Birch", "Pokémon"), it matches the expansion's species, item and move names; the ALL CAPS engine lists (map sections, trainer names and classes, Match Call, pocket names, landmarks, `src/strings.c` placeholders) then need converting too, or they will stand out. If the dialogue keeps capitals, the engine Title Case inserts (items, species, moves) stay mixed as they are today. Whichever is chosen, apply it to every table in this file at the same time; the length limits do not change with case.

## Appendix: how this inventory was built

All counts were produced by scanning `/home/user/pex-orig` (read-only):

* Map sections: `src/data/region_map/region_map_sections.json`; map usage from `data/maps/*/map.json` (`region_map_section`).
* Trainers: `src/data/trainers.party` parsed block by block (comments removed), fields `Name`, `Class`, `Pic`.
* Trainer classes: the `gTrainerClasses[]` initialiser in `src/battle_main.c`.
* Speakers: `^\s*\.string "(NAME|{STR_VAR_1}): ` over the Hoenn file set defined in §0.
* Unprefixed names and word counts: whole-word, case-sensitive matches inside `.string` lines of the same file set.
* Items: `[ITEM_X] = { … }` blocks in `src/data/items.h`; uses from `giveitem/checkitem/removeitem/additem` in Hoenn scripts.

