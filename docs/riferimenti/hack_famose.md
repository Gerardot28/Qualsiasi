# Le ROM hack più amate: cosa le rende speciali e cosa ci dà già pokeemerald-expansion 1.17.1

Documento di ricerca per **Pokémon Multiverse**. Scopo: capire quali feature rendono amate le hack più famose e, per ciascuna, se in pokeemerald-expansion 1.17.1 basta una configurazione (`#define` in `include/config/*.h`), uno script o del codice nuovo.

**Fonti e limiti.** Le wiki e i siti delle hack sono bloccati da questo ambiente (si raggiunge solo GitHub). Le schede delle hack vengono quindi:

- da materiale su GitHub: la guida di Emerald Seaglass ricavata dalla documentazione ufficiale (`jimineybillybob1/PokemonEmeraldSeaglassGuide`), i repository di Elite Redux (calcolatore, dex e fork di Showdown), il sorgente di Emerald Rogue (`Pokabbie/pokeemerald-rogue`);
- per il resto, dalla mia conoscenza della scena fino al 2025-26.

Ogni scheda ha un livello di **affidabilità** (alta, media, bassa). Prima di citare una feature pubblicamente va controllata sulla pagina ufficiale della hack.

La parte sull'expansion invece è **verificata sul sorgente** (`/home/user/pex-orig`, tag 1.17.1): nomi dei `#define`, flag e variabili richiesti, `STATIC_ASSERT`. Accanto a ogni voce è indicato se è **già attiva nel nostro build** (`/home/user/pex`, diff rispetto a 1.17.1).

---

## 1. Schede delle hack

| Hack | Base | Cosa la rende amata (feature chiave) | Affid. |
|---|---|---|---|
| **Pokémon Unbound** (Skeli) | RossoFuoco + CFRU (binario) | Regione originale Borrius con storia lunga e curata. Modalità di difficoltà alla partenza (Vanilla / Difficult / Expert / Insane). **Sistema di missioni** (side quest tracciate). **Raid Dynamax**. Mega, mosse Z e Dynamax insieme. **DexNav**. Crafting e minigiochi. Post-game enorme con Battle Frontier. Personalizzazione del personaggio. Allenamento EV/IV e cambio natura accessibili. | media |
| **Pokémon Radical Red** (soupercell) | RossoFuoco + CFRU | Remake di Kanto **difficile ma giusto**: squadre «competitive» stile Smogon e IA intelligente. **Level cap**. Modalità **Hardcore** (niente strumenti in lotta, regole severe) e modalità a grinding minimo. Pokémon fino alla Gen 9 nelle versioni 4.x, con tutte le Mega. **EV/IV visibili e modificabili**, Mente per la natura, capsule abilità, ricordamosse e tutor facili da raggiungere. Molte QoL: MT infinite, corsa ovunque, testo veloce. | media |
| **Pokémon Emerald Rogue** (Pokabbie) | pokeemerald (+ parti di expansion), open source | **Roguelite**: ogni run attraversa percorsi, palestre e leggendari casuali. Un **hub** si sblocca e si potenzia tra una run e l'altra, con missioni (quest) e preset di difficoltà configurabili. **Follower** e **Pokémon cavalcabili**. **Outfit** del personaggio. Pokémon di tutte le generazioni. Multiplayer e integrazione Archipelago. | media-alta |
| **Pokémon Inclement Emerald** | pokeemerald-expansion (vecchia) | Smeraldo «rifatto difficile»: allenatori con squadre complete e set ragionati, IA migliorata. Pokémon di molte generazioni catturabili già in Hoenn. Meccaniche moderne (Fata, split fisico/speciale, Mega). Pensata per le **Nuzlocke**: tool di terzi come customizer e randomizer. QoL: MT riutilizzabili, controllo EV/IV, cambio natura. | media |
| **Pokémon Emerald Seaglass** | pokeemerald-expansion | **Restyling grafico completo** in stile GBC/«RPG anni '90» (tileset Zaebucca, sprite Gen 2). **Follower**. Pokédex in stile HGSS. Gen 1–3 + evoluzioni cross-gen fino alla Gen 9 + ritocchi a statistiche e tipi. **Condividi Esp. di squadra + level cap «soft»**. Mosse Z opzionali. **Efficacia e tipi visibili in lotta**, L per le info mossa. **Senza HM-slave**: basta avere la MN nella borsa più la medaglia e il Pokémon compatibile la usa. Corsa automatica con R, IV/EV nel sommario, almeno 2 IV perfetti, Box Link, modalità Difficile e disattivazione del level cap da un libro in camera. **DexNav**, minigiochi (Scuba Safari, flipper), Amuleto Cromatico. «*NON è una hack di difficoltà*». | **alta** (doc ufficiale) |
| **Pokémon Elite Redux** | Smeraldo (fork del motore expansion) | Difficoltà estrema e meta propria: **ogni Pokémon ha 3 abilità «innate» più 1 scelta**. Mosse e abilità ridisegnate, modalità Elite/Hell, level cap, IA molto aggressiva. Ecosistema di tool della community: calcolatore danni, dex online, fork di Pokémon Showdown per provare le squadre. | alta (repo GitHub) |
| **Pokémon Gaia** (Spherical Ice) | RossoFuoco (binario) | Regione originale Orbtus con trama e personaggi forti, mappe e tile originali. Mega Evoluzioni, tipo Folletto, split fisico/speciale, Pokémon fino alla Gen 6–7. Molte missioni secondarie. | media |
| **Pokémon Glazed** | Smeraldo (binario) | **Multi-regione**: regione originale Tunod più un viaggio in altre regioni classiche. Tanti Pokémon (Gen 1–5/6), giorno e notte, contenuti dopo la Lega. Un riferimento per le hack «multi-regione» come la nostra. | media |
| **Pokémon Light Platinum** (WaRtenis) | Rubino (binario) | Regione Zhery, Pokémon fino alla Gen 4, lunghezza notevole, mondiali/torneo nel post-game ed estensione verso altre regioni. Storica, ha fatto entrare molti giocatori nel romhacking. | bassa-media |
| **Pokémon Clover** | RossoFuoco (binario), community /vp/ | Pokédex **interamente di Fakemon** (Clovermon), umorismo irriverente e parodia, regione nuova. Amata per originalità e coraggio creativo più che per le QoL. | media |
| **Pokémon Crystal Clear** (ShockSlayer) | Cristallo (binario) | **Open world**: si sceglie qualunque starter tra i 251 e la città di partenza. Johto e Kanto aperti da subito, Palestre in qualsiasi ordine con **livelli che scalano** in base alle medaglie, tutti i Pokémon catturabili. | alta |
| **Pokémon Prism** (Koolboyman) | Cristallo (binario) | Regioni nuove (Naljo e Rijon), **tipi nuovi** (Gas, Suono), sistema di **estrazione/fusione/crafting** (minerali e fucina), tantissimi minigiochi e contenuti. Mostra quanto può essere grande un gioco su GB. | media |
| **Pokémon Dark Rising** (DarkDowner) | RossoFuoco (binario) | Trama cupa e adulta (divinità, leggendari al centro), difficoltà alta, regione nuova. Amata per l'atmosfera. | bassa-media |
| **Pokémon Sienna** | — | **Non verificata**: nessuna fonte raggiungibile (niente su GitHub, wiki bloccate). Non ne descrivo le feature per non inventare. Da ricontrollare a mano. | — |
| *(extra)* **Run & Bun**, **Emerald Imperium** | Smeraldo (expansion o fork) | Due hack recenti molto seguite su motore Smeraldo moderno: «Showdown-like», ogni allenatore con set competitivi e IA intelligente, level cap, pensate per le Nuzlocke, guide e calcolatori della community. | media |

### Cosa hanno in comune le hack amate (i «pilastri»)

1. **Rispetto del tempo del giocatore (QoL)**: niente HM-slave, MT infinite, corsa ovunque e automatica, testo e lotte veloci, PC portatile, ricordamosse e cambio natura a portata di mano, IV/EV visibili. È la richiesta più universale.
2. **Difficoltà scelta dal giocatore**: modalità di difficoltà, level cap (soft o hard), IA intelligente e squadre «vere» per chi vuole la sfida (Radical Red, Elite Redux, Run & Bun); un'esperienza rilassata per gli altri (Seaglass «non è una hack di difficoltà»).
3. **Tutti (o molti) Pokémon e meccaniche moderne**: split fisico/speciale, Fata, Mega, Z, Dynamax, Tera. I giocatori vogliono la squadra preferita.
4. **Mondo vivo**: follower, ciclo giorno/notte, Pokémon visibili nell'erba, DexNav, invasioni (outbreak), meteo.
5. **Contenuti oltre la trama**: missioni secondarie (Unbound), post-game e Battle Frontier, raid, minigiochi e collezionismo (cromatici con catene e amuleti).
6. **Identità**: estetica forte (Seaglass), regione o meta originale (Prism, Clover, Elite Redux con le innate), oppure una formula nuova (open world di Crystal Clear, roguelite di Emerald Rogue, **multi-regione** di Glazed).

Per **Pokémon Multiverse** il pilastro identitario è il **multi-regione** (Kanto → Paldea). I pilastri 1–4 si coprono quasi interamente con l'expansion; il 5 va costruito con script, o con codice per raid e missioni.

---

## 2. Mappa feature → pokeemerald-expansion 1.17.1

Legenda «Lavoro»: **C** = solo config; **C+F** = config più un flag o una variabile dedicata (da prendere tra i `FLAG_UNUSED_*` / `VAR_UNUSED_*` e rinominare); **S** = serve anche scripting o dati (mappe, `trainers.party`, mart); **X** = codice C nuovo (piccolo/medio/grande).
Colonna «Nostro build»: ✅ = già attivo in `/home/user/pex`; — = valore di default di 1.17.1.

### 2.1 QoL

| Feature | `#define` (file) → valore consigliato | Requisiti extra | Lavoro | Nostro build |
|---|---|---|---|---|
| Corsa al chiuso | `OW_RUNNING_INDOORS` (overworld.h) → `GEN_LATEST` | – | C | — (già GEN_LATEST) |
| Scarpe da corsa dall'inizio | nessun config | `setflag FLAG_SYS_B_DASH` nello script di nuova partita | S (1 riga) | da fare |
| Corsa automatica (toggle) | **non presente** | piccolo hook in `field_player_avatar.c` più un'opzione o un tasto | X piccolo | da fare |
| Testo istantaneo o veloce | `TEXT_SPEED_INSTANT` (text.h) → `FALSE`, ma con `FLAG_TEXT_SPEED_INSTANT` → flag dedicato, così il giocatore sceglie; `TEXT_SPEED_FAST_MODIFIER` → 4–8; `AUTO_SCROLL_TEXT` opzionale | flag per l'opzione | C+F | — |
| Lotte più rapide | `B_FAST_INTRO_PKMN_TEXT` TRUE, `B_FAST_HP_DRAIN` TRUE, `B_FAST_EXP_GROW` TRUE (default); `B_WAIT_TIME_MULTIPLIER` → 8–12 (vanilla 16); `B_FAST_INTRO_NO_SLIDE` → TRUE se si vuole il massimo; `B_QUICK_MOVE_CURSOR_TO_RUN` → TRUE | – | C | parz. (default) |
| Tipi ed efficacia in lotta | `B_SHOW_TYPES` → `SHOW_TYPES_SEEN`; `B_SHOW_EFFECTIVENESS` → `SHOW_EFFECTIVENESS_SEEN` (default) | – | C | da attivare `B_SHOW_TYPES` |
| Descrizione mossa in lotta (tasto L) | `B_SHOW_MOVE_DESCRIPTION` TRUE, `B_MOVE_DESCRIPTION_BUTTON` `L_BUTTON` (default) | – | C | — |
| PS avversari in % | `B_HP_PERCENTAGE_DISPLAY` → TRUE (opzionale, utile in modalità difficile) | – | C | — |
| Ultima Ball usata (R) | `B_LAST_USED_BALL` TRUE, `B_LAST_USED_BALL_CYCLE` TRUE (default) | – | C | — |
| Scambio in squadra alla cattura, esperienza alla cattura | `B_CATCH_SWAP_INTO_PARTY`, `B_EXP_CATCH` → `GEN_LATEST` (default) | – | C | — |
| Condividi Esp. di squadra (Gen 6+) | `I_EXP_SHARE_ITEM` (item.h) → `GEN_6`; `I_EXP_SHARE_FLAG` → flag dedicato | il flag deve essere **permanente** (`> TEMP_FLAGS_END`, `STATIC_ASSERT` in `item_use.c`); dare `ITEM_EXP_SHARE` via script | C+F+S | da fare |
| Level cap | `B_EXP_CAP_TYPE` (caps.h) → `EXP_CAP_SOFT`; `B_LEVEL_CAP_TYPE` → `LEVEL_CAP_FLAG_LIST` (tabella `sLevelCapFlagMap` in `src/caps.c`) oppure `LEVEL_CAP_VARIABLE` + `B_LEVEL_CAP_VARIABLE`; `B_RARE_CANDY_CAP` TRUE; `B_LEVEL_CAP_EXP_UP` TRUE | con molte regioni e medaglie conviene `LEVEL_CAP_VARIABLE` (una var aggiornata dagli script dei Capipalestra) invece della lista di flag | C(+F) | ✅ (soft + flag list) |
| EV cap | `B_EV_CAP_TYPE` → `EV_CAP_VARIABLE` o `EV_CAP_FLAG_LIST`, `B_EV_ITEMS_CAP` | var dedicata | C+F | — |
| IV/EV nel sommario | `P_SUMMARY_SCREEN_IV_EV_INFO` (summary_screen.h) → TRUE, oppure `P_FLAG_SUMMARY_SCREEN_IV_EV_INFO` → flag da sbloccare in gioco; `P_SUMMARY_SCREEN_IV_EV_VALUES` → TRUE per i numeri invece delle lettere; `P_SUMMARY_SCREEN_IV_EV_TILESET` → TRUE per l'etichetta corretta | flag opzionale | C(+F) | da fare |
| Colori natura, rinomina dal sommario | `P_SUMMARY_SCREEN_NATURE_COLORS` TRUE, `P_SUMMARY_SCREEN_RENAME` TRUE (default) | – | C | — |
| Ricordamosse ovunque | `P_SUMMARY_SCREEN_MOVE_RELEARNER` TRUE (default: opzione nella pagina mosse del sommario); `P_ENABLE_MOVE_RELEARNERS` → TRUE (uovo, MT, tutor); `P_TM_MOVES_RELEARNER` → TRUE; `P_PRE_EVO_MOVES` → TRUE; `P_SORT_MOVES` → TRUE; in alternativa i flag `P_FLAG_EGG_MOVES` / `P_FLAG_TUTOR_MOVES` per sbloccarli in gioco | flag opzionali | C(+F) | parz. |
| MT riutilizzabili | `I_REUSABLE_TMS` (item.h) → TRUE | – | C | da fare |
| Dimenticare le MN | `P_CAN_FORGET_HIDDEN_MOVE` (pokemon.h) → TRUE | – | C | da fare |
| **Niente HM-slave** (mossa di campo senza insegnarla, stile Seaglass) | **non presente** | controllo in `party_menu.c` / `field_move_*`: MN nella borsa + medaglia + specie compatibile → opzione di campo | X medio | da fare |
| Menta, Capsula/Cerotto Abilità | strumenti già funzionanti (`ItemUseOutOfBattle_Mint`, `_AbilityCapsule`, `_AbilityPatch`) | metterli nei Market o darli da NPC | S | da fare |
| Tappi (Allenamento Speciale) | campi `MON_DATA_HYPER_TRAINED_*` presenti; `ITEM_BOTTLE_CAP` non si può usare dalla borsa | NPC e special che imposta il campo; `P_SUMMARY_SCREEN_IV_HYPERTRAIN` TRUE (default) | X piccolo + S | da fare |
| PC ovunque | strumento `ITEM_POKEMON_BOX_LINK` | darlo via script | S | da fare |
| Scegliere dal PC per tutor e scambi | `OW_CHOOSE_FROM_PC_AND_PARTY` TRUE (default) | – | C | — |
| Evoluzione con strumenti dalla borsa | `I_USE_EVO_HELD_ITEMS_FROM_BAG` → TRUE | – | C | ✅ |
| Menù Repellente o Esca all'esaurimento | `I_REPEL_LURE_MENU` TRUE + `VAR_LAST_REPEL_LURE_USED` → var dedicata | var | C+F | parz. |
| Descrizione degli strumenti raccolti | `OW_SHOW_ITEM_DESCRIPTIONS` → `OW_ITEM_DESCRIPTIONS_ALWAYS` (FIRST_TIME rompe i salvataggi: usa SaveBlock3) | – | C | — |
| **Viaggio rapido** (Volo dalla mappa con R) | `OW_FLAG_POKE_RIDER` (overworld.h) → flag dedicato | flag attivato dopo aver ottenuto Volo o un oggetto | C+F | da fare |
| Cercasfide | `I_VS_SEEKER_CHARGING` → flag | `docs/tutorials/vs_seeker.md`; disattiva il Match Call per le rivincite | C+F+S | — |
| Ricerca Strumenti stile ROZA | `I_ORAS_DOWSING_FLAG` → flag | – | C+F | — |
| Niente conferma di sovrascrittura salvataggio | `SKIP_SAVE_CONFIRMATION` (save.h) → TRUE | – | C | — |
| Salto dei «buchi» nel Pokédex | `P_SKIP_POKEDEX_GAPS` → `SKIP_GAPS_EXCEPT_BEFORE_AFTER` | – | C | — |
| Pesca moderna e catena di pesca | `I_FISHING_BITE_ODDS` GEN_LATEST, `I_FISHING_CHAIN` → TRUE (cromatici), `I_FISHING_FOLLOWER_BOOST` → TRUE (con i follower) | – | C | — |

### 2.2 Difficoltà

| Feature | Implementazione nell'expansion | Requisiti | Lavoro | Nostro build |
|---|---|---|---|---|
| **IA intelligente** | per allenatore, in `src/data/trainers.party`: `AI: Smart Trainer` (= `AI_FLAG_SMART_TRAINER`: base + `OMNISCIENT` + `SMART_SWITCHING` + `SMART_MON_CHOICES` + `PP_STALL_PREVENTION` + `SMART_TERA` + `RANDOMIZE_SWITCHIN`); per Capipalestra e Superquattro aggiungere `AI_FLAG_PREDICTION` e `AI_FLAG_ACE_POKEMON`. Tuning in `include/config/ai.h` | squadre in sintassi Showdown nel file `.party` | S | da fare |
| **Modalità di difficoltà** (Facile/Normale/Difficile) | `B_VAR_DIFFICULTY` (battle.h) → var dedicata; in `trainers.party` sezioni `Difficulty: Easy/Normal/Hard` per allenatore; `Script_SetDifficulty` dopo `NewGameInitData`; testi dell'allenatore per difficoltà (`DIFFICULTY_*`) | var + menu di scelta a inizio partita (Dynamic Multichoice) | C+F+S | da fare |
| Squadre casuali da un pool | `Party Size` + pool in `trainers.party` (`docs/tutorials/how_to_trainer_party_pool.md`), regole `B_POOL_RULE_*` (Species/Item/Mega clause), `B_POOL_SETTING_CONSISTENT_RNG` TRUE | – | S | — |
| Modalità «Hardcore» (niente borsa in lotta) | `B_VAR_NO_BAG_USE` → var (1 = vs allenatori, 2 = sempre) | var | C+F | — |
| Niente fuga dalle lotte con allenatori | **`B_RUN_TRAINER_BATTLE` → FALSE** (il default 1.17.1 è TRUE, cioè fuga stile Gen 9 contata come sconfitta) | – | C | da decidere |
| Clausola Sonno | `B_FLAG_SLEEP_CLAUSE` → flag (o `B_SLEEP_CLAUSE` TRUE sempre) | flag; l'IA lo capisce con `AI_FLAG_CHECK_BAD_MOVE` | C+F | — |
| IA intelligente per i selvatici | `WE_SMART_WILD_AI_FLAG` → flag, `B_VAR_WILD_AI_FLAGS` | flag/var | C+F | — |
| Lotte inverse, Lotte aeree | `B_FLAG_INVERSE_BATTLE`; `B_FLAG_SKY_BATTLE` + `B_VAR_SKY_BATTLE` | flag/var | C+F | — |
| Lotte doppie selvatiche | `WE_DOUBLE_WILD_CHANCE` (wild_encounter.h) → 0–10 %, `WE_FLAG_FORCE_DOUBLE_WILD` | – | C | — |
| Niente sconfitta (modalità storia) | `B_FLAG_NO_WHITEOUT` | flag | C+F | — |
| Modalità Nuzlocke integrata | non presente (ci sono `WE_FLAG_NO_CATCHING` / `WE_FLAG_NO_RUNNING` come mattoni) | regole: primo incontro per zona, KO = morte, clausola duplicati | X medio | — |
| Livelli che scalano (open world) | non presente; si approssima con `Difficulty:` + pool, oppure con script dinamici degli allenatori (`how_to_dynamic_trainer_script.md`) per numero di medaglie | – | S / X medio | — |

### 2.3 Meccaniche di lotta moderne

| Feature | Requisiti nell'expansion | Lavoro |
|---|---|---|
| Mega Evoluzione | `P_MEGA_EVOLUTIONS` TRUE (default, include le Mega di Z-A con `P_GEN_9_MEGA_EVOLUTIONS`) + **`ITEM_MEGA_RING` nella borsa** + Megapietre | S (dare l'oggetto) |
| Mosse Z | **`ITEM_Z_POWER_RING`** + Cristalli Z (nomi IT in `items.json`) | S |
| Dynamax/Gigamax | **`ITEM_DYNAMAX_BAND`** + **`B_FLAG_DYNAMAX_BATTLE`** attivato solo nelle lotte volute (come gli stadi di Galar) | C+F+S |
| Teracristal | **`ITEM_TERA_ORB`** + `B_FLAG_TERA_ORB_CHARGED` (si ricarica al Centro Pokémon), oppure `B_TERA_ORB_ALWAYS_CHARGED` TRUE; `B_FLAG_TERA_ORB_NO_COST`; `Tera Type:` nei set degli allenatori | C+F+S |
| Archeo-risveglio, Ultraesplosione | `P_PRIMAL_REVERSIONS`, `P_ULTRA_BURST_FORMS` (default TRUE) | – |
| **Raid** (Dynamax o Tera) | **non presenti** nell'expansion: niente tana e niente tipo di lotta raid. Mattoni esistenti: boost iniziali «Totem» (stat stage in apertura), lotte multi con partner NPC (`FNPC_*`), `B_FLAG_DYNAMAX_BATTLE` | X grande |

Proposta per Multiverse: ogni gimmick resta legato alla sua regione. Megapietre a Kalos/Hoenn, Cristalli Z ad Alola, Dynamax solo negli stadi di Galar (flag attivato dallo script dello stadio), Teracristal a Paldea. Scelta coerente e facile da fare con gli oggetti chiave e `B_FLAG_DYNAMAX_BATTLE`.

### 2.4 Esplorazione e mondo vivo

| Feature | `#define` → valore | Requisiti | Lavoro | Nostro build |
|---|---|---|---|---|
| **Pokémon che seguono** (follower) | `OW_FOLLOWERS_ENABLED` TRUE, `OW_POKEMON_OBJECT_EVENTS` TRUE; `B_FLAG_FOLLOWERS_DISABLED` → flag (spegne il follower in grotte strette e cutscene); `OW_FOLLOWERS_POKEBALLS`, `OW_FOLLOWERS_BOBBING` | sprite inclusi per quasi tutte le specie (vedi `grafica.md`) | C+F | ✅ |
| Compagni NPC (Gen 4) e lotte in coppia | `FNPC_ENABLE_NPC_FOLLOWERS` → TRUE; `FNPC_FLAG_HEAL_AFTER_FOLLOWER_BATTLE`, `FNPC_FLAG_PARTNER_WILD_BATTLES` | `docs/tutorials/how_to_follower_npc.md`; usa SaveBlock3 | C+F+S | — |
| **DexNav** | `DEXNAV_ENABLED` → TRUE + **tutti** diversi da 0: `DN_FLAG_SEARCHING`, `DN_FLAG_DEXNAV_GET`, `DN_FLAG_DETECTOR_MODE`, `DN_VAR_SPECIES`, `DN_VAR_STEP_COUNTER` (`STATIC_ASSERT` in `dexnav.c`). **`USE_DEXNAV_SEARCH_LEVELS` lasciarlo FALSE**: 1 byte di salvataggio per specie, rischio di sforare il saveblock | 3 flag + 2 var + `setflag DN_FLAG_DEXNAV_GET` per sbloccarlo | C+F+S | da fare |
| Pokémon visibili sulla mappa | `WE_OW_ENCOUNTERS` → TRUE (richiede `OW_POKEMON_OBJECT_EVENTS`); `WE_OWE_SHINY_SPARKLE` TRUE; consigliato `OW_GFX_COMPRESS` FALSE per la VRAM (ma costa ROM) | `docs/tutorials/how_to_overworld_wild_encounters.md` | C | — |
| Incontri per fascia oraria | `OW_TIME_OF_DAY_ENCOUNTERS` → TRUE (+ tabelle per orario in `wild_encounters.json`) | DNS/RTC | C+S | — |
| Giorno/notte | `OW_ENABLE_DNS` TRUE (default) + palette `.pla` e oggetti luce (`docs/tutorials/dns.md`) | lavoro grafico sui tileset | C+S | — |
| Invasioni (swarm/outbreak) | comandi `startoutbreak`, `editoutbreak`, `clearactiveoutbreak` (`how_to_mass_outbreak.md`) | script, per esempio un notiziario TV | S | — |
| Meteo dinamico giornaliero | `WEATHER_DYNAMIC` nell'header della mappa (`how_to_dynamic_weather.md`) | – | S | — |
| Negozi che cambiano assortimento | Dynamic Shop (`how_to_dynamic_shop.md`) | – | S | — |
| Alberi Ghicocca | `how_to_apricorn_tree.md` | – | S | — |
| Bacche stile XY | `OW_BERRY_MUTATIONS`, `OW_BERRY_MOISTURE`, `OW_BERRY_WEEDS`, `OW_BERRY_PESTS`, `OW_BERRY_IMMORTAL` | – | C | — |
| Mosse di campo Gen 4 | `OW_DEFOG_FIELD_MOVE`, `OW_ROCK_CLIMB_FIELD_MOVE` → TRUE | mappe con le relative tile | C+S | — |
| Popup dei nomi di mappa B2W2 | `OW_POPUP_GENERATION` `GEN_5` | – | C | ✅ |
| Anteprime delle mappe | `MPS_ENABLE_MAP_PREVIEWS` TRUE | immagini e flag per mappa | C+S | ✅ |
| Nome di chi parla | `setspeaker` / `{SPEAKER …}` (name_box.h) | – | S | — |
| **Missioni / quest log** (Unbound, Rogue) | **non presente** | menu missioni più flag/var per ciascuna. Si possono riusare i branch della community («Quest Menu») | X medio | — |
| **Crafting / miniere** (Unbound, Prism) | **non presente** | – | X medio-grande | — |
| Battle Frontier | quello di Smeraldo è incluso (Torre Lotta, Fabbrica, Piramide…); `BATTLE_PYRAMID_RANDOM_ENCOUNTERS` → TRUE per incontri generati | si può tenere come post-game di Hoenn | — | — |
| Personalizzazione del personaggio (outfit) | **non presente** | – | X medio-grande | — |
| Caccia ai cromatici | `I_SHINY_CHARM_ADDITIONAL_ROLLS` (default 2), `I_FISHING_CHAIN`, catene DexNav; metodo Masuda **non presente** (X piccolo) | – | C / X piccolo | — |
| **Multi-regione** (Glazed) | nessun «sistema regioni»; 1.17.1 supporta però mappe FRLG con l'attributo `region` (`docs/tutorials/how_to_frlg.md`) e le mappe di regione di Kanto (`region_map_layout_kanto.h`, Settipelago). Cambio della mappa di regione, del Pokédex regionale, della musica e della Lega per regione: custom | – | X medio | in corso |

---

## 3. Funzioni che richiedono codice nuovo (stima)

| Funzione | Dove si interviene | Stima |
|---|---|---|
| Corsa automatica (toggle con R o opzione) | `field_player_avatar.c` (logica di corsa), menu Opzioni | 0,5 giorni |
| MN senza insegnarle (stile Seaglass) | `party_menu.c` (lista delle mosse di campo), `field_move_*`, `script` delle MN (Taglio, Surf, Forza, Cascata, Sub, Volo…) | 2–3 giorni |
| NPC Allenamento Speciale (Tappi) | special che imposta `MON_DATA_HYPER_TRAINED_*` più uno script | 0,5 giorni |
| Metodo Masuda | `daycare.c`, generazione del PID | 0,5 giorni |
| Menu missioni (quest log) | nuova UI + tabella missioni; si può prendere spunto dai branch della community | 4–7 giorni |
| Raid (Dynamax/Tera) | tipo di lotta raid, scudi, 4 giocatori NPC, tane, ricompense | 2–4 settimane |
| Crafting/miniere | UI e minigioco | 1–3 settimane |
| Modalità Nuzlocke | regole in cattura, KO e duplicati | 2–4 giorni |
| Cambio di regione (mappa, Pokédex, Lega) | `region_map.c`, `pokedex.c`, header delle mappe | dipende dall'architettura multi-regione già scelta |

---

## 4. Raccomandazioni per Pokémon Multiverse (prima alto impatto e basso sforzo)

Ordine: impatto sul giocatore diviso sforzo. Le voci 1–15 sono quasi tutte solo config, flag o script.

| # | Raccomandazione | Config / requisiti | Sforzo |
|---|---|---|---|
| 1 | **Tipi ed efficacia in lotta** | `B_SHOW_TYPES` → `SHOW_TYPES_SEEN`; `B_SHOW_EFFECTIVENESS` resta `SHOW_EFFECTIVENESS_SEEN` | minimo |
| 2 | **MT riutilizzabili e MN dimenticabili** | `I_REUSABLE_TMS` TRUE; `P_CAN_FORGET_HIDDEN_MOVE` TRUE | minimo |
| 3 | **IV/EV nel sommario** (sbloccabili con un oggetto o un NPC, oppure sempre) | `P_SUMMARY_SCREEN_IV_EV_INFO` TRUE (o `P_FLAG_SUMMARY_SCREEN_IV_EV_INFO` + flag), `P_SUMMARY_SCREEN_IV_EV_VALUES` TRUE, `P_SUMMARY_SCREEN_IV_EV_TILESET` TRUE | minimo |
| 4 | **Ricordamosse completo dal sommario** | `P_SUMMARY_SCREEN_MOVE_RELEARNER` TRUE (default), `P_ENABLE_MOVE_RELEARNERS` TRUE, `P_TM_MOVES_RELEARNER` TRUE, `P_PRE_EVO_MOVES` TRUE, `P_SORT_MOVES` TRUE | minimo |
| 5 | **Condividi Esp. Gen 6** (con i level cap soft già attivi) | `I_EXP_SHARE_ITEM` `GEN_6`, `I_EXP_SHARE_FLAG` = flag permanente; dare `ITEM_EXP_SHARE` presto | basso |
| 6 | **Level cap per variabile** (regge meglio molte regioni e più di 8 medaglie per regione) | `B_LEVEL_CAP_TYPE` `LEVEL_CAP_VARIABLE` + `B_LEVEL_CAP_VARIABLE` = var dedicata, aggiornata da ogni Capopalestra; mantenere `EXP_CAP_SOFT`, `B_RARE_CANDY_CAP` TRUE, `B_LEVEL_CAP_EXP_UP` TRUE | basso |
| 7 | **Viaggio rapido da mappa** | `OW_FLAG_POKE_RIDER` = flag attivato con Volo o la prima medaglia | basso |
| 8 | **Box Link, Menta, Capsula/Cerotto Abilità facili da trovare** | `ITEM_POKEMON_BOX_LINK` via script; Menta e Capsule nei Market dei Centri Commerciali | basso (script) |
| 9 | **Scarpe da corsa dall'inizio e testo veloce** | `setflag FLAG_SYS_B_DASH` in nuova partita; `FLAG_TEXT_SPEED_INSTANT` = flag (opzione «Istantaneo»), `TEXT_SPEED_FAST_MODIFIER` 4–8; `B_WAIT_TIME_MULTIPLIER` 12 | basso |
| 10 | **Selettore di difficoltà a inizio partita** | `B_VAR_DIFFICULTY` = var; sezioni `Difficulty: Hard` in `trainers.party` per Capipalestra, Superquattro e rivali; `B_RUN_TRAINER_BATTLE` FALSE; in Difficile `B_VAR_NO_BAG_USE` = 1 | medio (dati) |
| 11 | **IA intelligente per i boss** (Capipalestra, Superquattro, Campioni, capi delle squadre malvagie) | `AI: Smart Trainer` (+ `AI_FLAG_PREDICTION`, `AI_FLAG_ACE_POKEMON`) in `trainers.party`, set Showdown | medio (dati) |
| 12 | **DexNav** | `DEXNAV_ENABLED` TRUE + `DN_FLAG_SEARCHING`, `DN_FLAG_DEXNAV_GET`, `DN_FLAG_DETECTOR_MODE`, `DN_VAR_SPECIES`, `DN_VAR_STEP_COUNTER`; `USE_DEXNAV_SEARCH_LEVELS` FALSE | basso |
| 13 | **Gimmick per regione** | Mega (`ITEM_MEGA_RING`) da Kalos/Hoenn, Z (`ITEM_Z_POWER_RING`) ad Alola, Dynamax (`ITEM_DYNAMAX_BAND` + `B_FLAG_DYNAMAX_BATTLE` solo negli stadi di Galar), Tera (`ITEM_TERA_ORB` + `B_FLAG_TERA_ORB_CHARGED`) a Paldea | basso-medio |
| 14 | **Mondo vivo**: incontri per fascia oraria, invasioni, meteo dinamico, Cercasfide | `OW_TIME_OF_DAY_ENCOUNTERS` TRUE; `startoutbreak`; `WEATHER_DYNAMIC`; `I_VS_SEEKER_CHARGING` = flag | medio (dati) |
| 15 | **Menu Repellente e descrizione degli oggetti** | `I_REPEL_LURE_MENU` TRUE + `VAR_LAST_REPEL_LURE_USED` = var; `OW_SHOW_ITEM_DESCRIPTIONS` `OW_ITEM_DESCRIPTIONS_ALWAYS` | minimo |
| 16 | Disattivare il follower dove serve | `B_FLAG_FOLLOWERS_DISABLED` = flag (grotte strette, cutscene) | minimo |
| 17 | Catena di pesca e bonus follower | `I_FISHING_CHAIN` TRUE, `I_FISHING_FOLLOWER_BOOST` TRUE | minimo |
| 18 | **MN senza HM-slave** (codice) | custom, vedi §3 | 2–3 giorni |
| 19 | **Corsa automatica** (codice) | custom | 0,5 giorni |
| 20 | Pokémon visibili sulla mappa (opzionale) | `WE_OW_ENCOUNTERS` TRUE; valutare VRAM e ROM, vedi `grafica.md` | basso config, test lungo |
| 21 | Missioni secondarie con menu dedicato (identità «Unbound») | custom | 1 settimana |
| 22 | Raid (identità «Unbound») | custom grande, solo se c'è tempo | settimane |

**Da evitare o rimandare**:
- `USE_DEXNAV_SEARCH_LEVELS`: rischio sul saveblock.
- `OW_SHOW_ITEM_DESCRIPTIONS_FIRST_TIME`: rompe i salvataggi.
- Cambiare `P_GEN_*_POKEMON` a progetto avviato: cambia i flag del Pokédex e serve un nuovo salvataggio.
- `B_FAST_INTRO_NO_SLIDE`, solo se lo si vuole davvero: toglie l'«effetto Gen 3».
