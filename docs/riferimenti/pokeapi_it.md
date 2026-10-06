# Dati italiani (PokeAPI + fonti di supporto) mappati sulle costanti di pokeemerald-expansion 1.17.1

Riferimento per la localizzazione italiana di **Pokémon Multiverse**.

- Dati generati: `/home/user/work/pokeapi_it/*.json`
- Script, rieseguibili e deterministici: `/home/user/work/pokeapi/scripts/`
  - `build_it.py`: genera tutti i JSON di nomi e descrizioni e il report di copertura.
  - `regioni.py`: verifica i nomi italiani dei personaggi e delle medaglie e scrive `regioni_personaggi.json`.
  - `render_regioni.py`: genera `docs/riferimenti/regioni.md`.
  - `corpus_lookup.py`: tool da riga di comando per cercare un nome inglese nei testi ufficiali e vedere com'è stato tradotto (`exact`, `cooc`).
- Sorgenti scaricati (solo GitHub): `/home/user/work/pokeapi/{repo,pkhex,poke-corpus,tcgdex,pokerogue-locales}`

## Fonti e ordine di priorità

| Priorità | Fonte | Cosa fornisce | Licenza |
|---|---|---|---|
| 1 | [PokeAPI](https://github.com/PokeAPI/pokeapi) `data/v2/csv` (clone sparse) | nomi IT di mosse, strumenti, abilità, natura, tipi, statistiche, gruppi uova, specie e categorie; descrizioni (flavor text) IT; luoghi (IT solo per Hoenn/Kalos/Alola) | BSD-3-Clause («Pokémon and Pokémon character names are trademarks of Nintendo») |
| 2 | [PKHeX](https://github.com/kwsch/PKHeX) `PKHeX.Core/Resources/text` | tabelle di stringhe estratte dai giochi (EN e IT allineate per indice): mosse, abilità, strumenti fino a Z-A, luoghi di ogni generazione (`locations/gen*/text_*_00000_it.txt`) | GPL-3.0 (usiamo solo i dati di stringa) |
| 3 | [poke-corpus](https://github.com/abcboy101/poke-corpus) `corpus/<Gioco>/{en,it}_*.txt` | testo **ufficiale** di 40+ giochi, EN e IT allineati riga per riga. Serve a riempire i buchi (mosse G-Max, abilità di Champions/Z-A, descrizioni Gen 9) e a verificare i nomi | GPL-3.0 (contenuto © Nintendo/Game Freak) |
| 4 | [TCGdex](https://github.com/tcgdex/cards-database) | nomi IT ufficiali delle carte Allenatore del GCC; serve a verificare i nomi dei personaggi | MIT |
| 5 | [pokerogue-locales](https://github.com/pagefaultgames/pokerogue-locales) `it/trainer-names.json` | nomi dei personaggi tradotti dalla community; solo come conferma secondaria | AGPL-3.0 |

**Licenze e diritti.** I dati PokeAPI sono distribuiti con licenza BSD-3-Clause (copyright Paul Hallett e collaboratori di PokeAPI) e vanno citati. Tutti i nomi, i testi e le descrizioni di Pokémon, mosse, strumenti, luoghi e personaggi sono © Nintendo / Game Freak / Creatures / The Pokémon Company. È così per qualunque romhack: i JSON sono un riferimento di traduzione, non materiale nostro. Non ridistribuire i dump completi del corpus (GPL-3.0, contenuto proprietario): nei JSON ci sono solo le stringhe che servono.

Codici lingua in PokeAPI: `languages.csv` → **italiano = id 8** (`it`), inglese = id 9.

## File prodotti (`/home/user/work/pokeapi_it/`)

| File | Chiave | Contenuto |
|---|---|---|
| `moves.json` | `MOVE_*` | `expansion_name`, `en`, `it`, `source`, `pokeapi_identifier`, `it_desc` (descrizione IT più recente) e `it_desc_version`, `issues` |
| `items.json` | `ITEM_*` | come sopra; MT/MN calcolati per regola (`ITEM_TM01` → «MT01», `ITEM_HM01` → «MN01») |
| `abilities.json` | `ABILITY_*` | come sopra; i 2 slot segnaposto `ABILITY_314`/`ABILITY_317` (nome «-------») sono esclusi |
| `natures.json` | `NATURE_*` | `en`, `it`, `increased_stat`, `decreased_stat` |
| `types.json` | `TYPE_*` | `TYPE_MYSTERY` → «???»; `TYPE_STELLAR` → «Astrale» |
| `stats.json` | `STAT_*` | PS, Attacco, Difesa, Velocità, Attacco Speciale, Difesa Speciale, precisione, elusione (minuscolo come in PokeAPI) |
| `egg_groups.json` | `EGG_GROUP_*` | nomi IT dei gruppi uova |
| `species.json` | `SPECIES_*` (1572, forme incluse) | `national_dex`, `it`, `it_genus` («Pokémon Seme»), `it_category` («Seme», pronto per `.categoryName`), `is_form` |
| `species_by_national_dex.json` | n° nazionale 1–1025 | nome, categoria, generazione, voce del Pokédex IT |
| `flavor_moves.json` / `flavor_items.json` / `flavor_abilities.json` | costante | solo le voci con descrizione IT |
| `flavor_pokedex.json` | `SPECIES_*` (solo forme base) | voce del Pokédex IT del gioco più recente disponibile in italiano |
| `regions.json` | identificatore PokeAPI | Kanto, Johto, Hoenn, Sinnoh, **Unima**, Kalos, Alola, Galar, Hisui, Paldea |
| `locations.json` | regione → identificatore PokeAPI | `en`, `it`, `source` (`pokeapi` / `pkhex:<file>` / `corpus` / `rule`), `issues` |
| `mapsec_hoenn_kanto.json` | `MAPSEC_*` (209) | `it_ingame`: il nome MAIUSCOLO ufficiale di Smeraldo/RFVF IT (es. `MAPSEC_LITTLEROOT_TOWN` → «ALBANOVA»); `it` in minuscolo da PKHeX |
| `regioni_personaggi.json` | regione → ruolo | Capipalestra, Superquattro, Campioni, professori, squadre, con evidenze e confidenza (vedi `regioni.md`) |
| `_coverage_report.json` | | report completo: copertura, fonti, voci senza nome IT, problemi di lunghezza e charset |

Il campo `issues` segnala problemi pratici per il ROM:

- `too_long:N>L`: il nome italiano supera il limite del motore. `MOVE_NAME_LENGTH` = 16, `ITEM_NAME_LENGTH` = 20, `ABILITY_NAME_LENGTH` = 16, `POKEMON_NAME_LENGTH` = 12, `TYPE_NAME_LENGTH` = 8, `MAP_NAME_LENGTH` = 16; per la categoria del Pokédex si usa 12 come euristica.
- `chars_not_in_charmap:…`: caratteri assenti in `charmap.txt`. Il charmap contiene già à è é ì ò ù À È e l'apostrofo `’`/`'` (B4).

## Copertura (costanti dell'expansion con nome italiano)

| Categoria | Costanti | Con nome IT | Solo PokeAPI | Fallback | Descrizione IT |
|---|---|---|---|---|---|
| Mosse `MOVE_*` (incluse Z, Max, G-Max) | 934 | **934 (100%)** | 901 (96,5%) | 33 mosse G-Max dal corpus di SpSc | 864 (92,5%) |
| Strumenti `ITEM_*` | 873 | **873 (100%)** | 750 (85,9%) + 108 MT/MN per regola = 858 (98,3%) | 14 da PKHeX, 1 dal corpus | 647 (74,1%); 646 su 764 escludendo MT/MN (84,6%) |
| Abilità `ABILITY_*` | 317 | **317 (100%)** | 314 (99,1%) | 3 dal corpus (Pokémon Champions) | 289 (91,2%) |
| Natura `NATURE_*` | 25 | 25 (100%) | 25 | – | – |
| Tipi `TYPE_*` | 20 | 20 (100%) | 20 | – | – |
| Statistiche `STAT_*` | 8 | 8 (100%) | 8 | – | – |
| Gruppi uova `EGG_GROUP_*` | 15 | 15 (100%) | 15 | – | – |
| Specie `SPECIES_*` (forme incluse) | 1572 | 1572 (100%) | 1572 | – | categoria: 1572 (100%; 120 categorie Gen 9 dal corpus) |
| Voci del Pokédex (specie base) | 1025 | **1018 (99,3%)** | 720 | 298 recuperate dal corpus (ScVi/LA) | – |
| Luoghi PokeAPI (10 regioni principali) | 995 | **964 (96,9%)** | 286 (solo Hoenn/Kalos/Alola) | PKHeX + corpus + regola | – |
| `MAPSEC_*` di pokeemerald | 209 | 209 (100%) | – | 203 con nome ufficiale in-game di Smeraldo/RFVF | – |

Luoghi per regione (con nome IT / totale in PokeAPI): Kanto 92/96, Johto 62/67, Hoenn 105/110, Sinnoh 126/128, Unima 116/122, Kalos 104/106, Alola 101/101, Galar 87/92, Hisui 87/89, Paldea 84/84. Mancano solo pseudo-luoghi di PokeAPI («Roaming Kanto», «Unknown; all Poliwag», «Max Dens in the Galar Wild Area», «Kanto Pokémart» senza nome…), che non sono luoghi reali.

Versioni usate per le voci del Pokédex (la più recente con testo IT): Spada/Scudo 493, Leggende Arceus 179, Violetto/Scarlatto 119, Alpha Sapphire 129, Ultraluna 68, Let's Go Eevee 30. Le descrizioni Gen 9 mancano in italiano su PokeAPI. Sono state recuperate cercando la stessa voce inglese, identica, nel corpus allineato di ScVi/LA e prendendo la riga italiana corrispondente.

### Costanti senza nome italiano in PokeAPI (coperte con fallback)

- **Mosse (33)**: tutte le `MOVE_G_MAX_*`, che PokeAPI non ha. I nomi vengono dal corpus di Spada/Scudo, es. `MOVE_G_MAX_VINE_LASH` «Gigasferzata», `MOVE_G_MAX_WILDFIRE` «Gigavampa», `MOVE_G_MAX_VOLT_CRASH` «Gigapikafolgori», `MOVE_G_MAX_STEELSURGE` «Gigaferroaculei», `MOVE_G_MAX_RAPID_FLOW` «Gigapluricolpo».
- **Strumenti (15)**:
  - da PKHeX: `ITEM_FRESH_START_MOCHI` «Mochi del ripristino», `ITEM_STELLAR_TERA_SHARD` «Teralite Astrale», `ITEM_JUBILIFE_MUFFIN` «Muffin Giubilo», `ITEM_REMEDY` / `FINE_REMEDY` / `SUPERB_REMEDY` «Preparato» / «Preparato buono» / «Preparato ottimo», `ITEM_AUX_EVASION` «Elusione X», `ITEM_AUX_GUARD` «Difensiva extra», `ITEM_AUX_POWER` «Offensiva extra», `ITEM_AUX_POWERGUARD` «Multi extra», `ITEM_CHOICE_DUMPLING` «Involtoscelta», `ITEM_SWAP_SNACK` «Invertigratin», `ITEM_TWICE_SPICED_RADISH` «Doppiaceto», `ITEM_POKESHI_DOLL` «Pokékokeshi»
  - dal corpus di Stadium 2/OAC: `ITEM_BERSERK_GENE` «Genefurioso»
- **Abilità (3)**, nuove di Pokémon Champions, dal corpus di Champions: `ABILITY_EELEVATE` «Rapidascesa», `ABILITY_FIRE_MANE` «Pirocriniera», `ABILITY_AURA_GUARD` «Ondascudo».
- **Nessuna costante resta senza nome italiano.**

### Descrizioni ancora mancanti

- Mosse (70): tutte le Z, Max e G-Max (non hanno descrizioni nei giochi), più circa 37 mosse Gen 9 senza flavor text IT su PokeAPI né corrispondenza esatta nel corpus, es. `MOVE_ESPER_WING`, `MOVE_LAST_RESPECTS`, `MOVE_ORDER_UP`, `MOVE_TERA_BLAST`, `MOVE_BLOOD_MOON`, `MOVE_UPPER_HAND`.
- Strumenti (118, MT/MN esclusi): soprattutto Teralite, maschere di Ogerpon, mochi, `ITEM_ABILITY_SHIELD`, `ITEM_CLEAR_AMULET`, `ITEM_COVERT_CLOAK`, `ITEM_LOADED_DICE`, `ITEM_BOOSTER_ENERGY`.
- Abilità (28): Gen 9 (`ABILITY_PROTOSYNTHESIS`, `ABILITY_QUARK_DRIVE`, `ABILITY_GOOD_AS_GOLD`, i quattro «…of Ruin», `ABILITY_TERA_SHELL`…) e le nuove di Champions.

Queste descrizioni vanno scritte a mano. Bisogna comunque riscriverle in forma breve: il box descrizione di pokeemerald ha 3 righe da circa 18 caratteri per gli strumenti e 1–2 righe per le mosse, mentre i testi di Spada/Scudo e Violetto sono più lunghi.

### Nomi da accorciare (limiti del motore)

- **Mosse (32)**: tutte le mosse Z hanno nomi italiani di 17–28 caratteri, oltre `MOVE_NAME_LENGTH` = 16. Esempi: «Fotodistruzione Apocalittica» (28), «Colpo Eptastellare Rubanima» (27), «Gigamacigno Polverizzante» (25). Le mosse normali stanno tutte nei 16 caratteri. Le G-Max più lunghe arrivano a 16 («Gigapugnointuito», «Gigarinnovamento»). Opzioni: alzare `MOVE_NAME_LENGTH` (costa RAM nei buffer di battaglia) oppure abbreviare a mano solo le mosse Z.
- **Tipi (1)**: «Coleottero» è 10 caratteri, oltre `TYPE_NAME_LENGTH` = 8. In pokeemerald i tipi sono quasi sempre icone, ma il nome testuale serve per esempio nella descrizione mossa in battaglia. Portare `TYPE_NAME_LENGTH` a 10 oppure usare «Coleott.».
- **Categoria (1)**: Reshiram, «Bianco Verità» (13).
- **Strumenti e abilità**: 0 sopra i limiti (strumenti 20 caratteri, abilità 16).
- **Specie**: 0 sopra i 12 caratteri. Il più lungo è «Fungofurioso» (12).
- **MAPSEC**: 0 sopra 16 se si usa `it_ingame` (es. «SOTT'ACQUA», «ZONA SAFARI»). I nomi PKHeX con disambiguazioni tra parentesi, tipo «(RZS)», non vanno usati così come sono.

## Note di metodo

- **Mappatura sulle costanti**:
  1. Prima per identificatore: `MOVE_KARATE_CHOP` → `karate-chop`, `ITEM_POKE_BALL` → `poke-ball`, `ABILITY_LEVITATE` → `levitate`.
  2. Poi per nome inglese normalizzato preso dai sorgenti dell'expansion: `.name` in `src/data/moves_info.h`, `src/data/items.h`, `src/data/abilities.h`.
  3. Infine via PKHeX o corpus.
  - Gli alias (es. `MOVE_VICEGRIP = MOVE_VISE_GRIP`, `ITEM_ITEMFINDER`) sono esclusi dal conteggio.
  - Gli MT si rimappano da `ITEM_TM_<MOSSA>` a `ITEM_TMnn` tramite `FOREACH_TM` in `include/constants/tms_hms.h`.
- **Specie**: mappate tramite il nome di `NATIONAL_DEX_*` (`include/constants/pokedex.h`). Le forme (`SPECIES_VENUSAUR_MEGA`, `SPECIES_RATTATA_ALOLA`…) ereditano nome e categoria della specie base. In italiano i nomi delle specie coincidono con quelli inglesi tranne 21 eccezioni: Tipo Zero e i Paradosso, es. Grandizanne, Codaurlante, Eroeferreo, Capoferreo.
- **Descrizioni**: si sceglie la versione più recente per ordine dei version group. Si scartano i testi segnaposto di Spada/Scudo («Questa mossa non può essere usata…», 146 voci) e quelli con `[VAR`.
- **Fallback dal corpus**: si accetta una traduzione solo se compare come riga intera nella tabella del gioco e ha la maggioranza, almeno il 60% delle occorrenze. Per i luoghi si usa il voto di maggioranza, preferendo le forme non abbreviate.
- **Nomi delle mappe di Smeraldo**: `it_ingame` viene dalle righe MAIUSCOLE di Smeraldo, RFVF e RZ italiani, quindi sono esattamente le stringhe del gioco originale.
