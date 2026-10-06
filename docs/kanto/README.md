# Kanto (Atto 2): fondamenta

Le mappe FRLG di Kanto che pokeemerald-expansion 1.17.1 già contiene (`data/maps/*_Frlg`) vengono
compilate anche nella ROM Emerald. Non contengono ancora dialoghi né logica di trama: ogni mappa ha uno
`scripts.inc` stub con un'etichetta per ogni evento, e chi scrive i testi deve solo riempirle.

- Strumento: `tools/kanto/port_kanto.py`. È idempotente, vale per qualsiasi albero 1.17.x e marca tutte le
  modifiche con `KANTO_PORT`.
- Inventario leggibile da macchina: `docs/kanto/mappe.json`. Per ogni mappa contiene MAPSEC, nome italiano,
  tipo, oggetti (gfx, posizione, tipo e visuale allenatore originali, costante `TRAINER_` FRLG, script
  FRLG originale, flag), cartelli, warp, connessioni, ball, oggetti nascosti, coord event, tabelle
  selvatiche e gli special FRLG usati dallo script originale.
- Albero di prova: `/home/user/work/kanto-full`, cioè `pex-orig` più `--all --test-start`.
  Screenshot: `/home/user/work/kanto-out/full/`.

## Come applicarlo all'albero principale

```sh
python3 tools/kanto/port_kanto.py --tree /home/user/pex --all --dry-run   # anteprima: ~783 file
python3 tools/kanto/port_kanto.py --tree /home/user/pex --all             # applica e rigenera docs/kanto/mappe.json
cd /home/user/pex && make -j2
```

- Si può rilanciare quante volte si vuole: la seconda esecuzione non cambia nulla.
- Uno `scripts.inc` stub, una volta creato, **non viene mai sovrascritto**. Fa eccezione `--force-stubs`,
  che distrugge le modifiche.
- Anche `data/scripts/kanto_travel.inc` viene scritto una volta sola.
- Il dry-run su `/home/user/pex` (rev. attuale, già in italiano) passa senza conflitti. I nomi italiani
  delle MAPSEC ci sono già, quindi lo strumento non li tocca.
- `--test-start MAP,X,Y` serve **solo per i test**: la nuova partita parte a Kanto con Pikachu e i flag
  da campione, e aggiunge l'hook di warp per l'emulatore. Non va usato sull'albero principale.

## Mappe incluse: 253

`--all` porta tutto il continente:

| Tipo | Mappe |
|---|---|
| Città | 12 |
| Percorsi | 26 |
| Palestre | 8 |
| Centri Pokémon | 24 |
| Market | 8 |
| Centro Commerciale | 7 |
| Case | 29 |
| Altri edifici | 26 |
| Porte/gate | 17 |
| Dungeon | 64 |
| M/N Anna | 26 |
| Lega | 6 |

I dungeon sono Bosco Smeraldo, Monte Luna, Tunnel Roccioso, Torre Pokémon, Zona Safari, Isole
Spumarine, Centrale, Villa Pokémon, Silph SpA, Rifugio Rocket, Via Vittoria, Grotta Celeste, Grotta
Diglett e Via Sotterranea. La Lega comprende l'Altopiano Blu, i Superquattro e la Sala d'Onore.

Restano esclusi le Isole Sette (si aggiungono con `--include-sevii`), la Torre Allenatori, Ombelico/Isola
Genesi e le stanze link.

## Cosa cambia (patch)

**Motore**, una volta sola:

- `include/constants/global.h`: aggiunge `KANTO_IN_EMERALD`.
- `tools/mapjson/mapjson.cpp`: le mappe e i layout con `"include_in_emerald": true` vengono compilati in
  Emerald, e i gruppi di mappe costruiti solo in parte tengono gli indici.
- `src/data/tilesets/*`, `src/data/object_events/*`, `src/event_object_movement.c`, `src/field_door.c`:
  tileset, sprite e porte FRLG anche in Emerald.
- `tools/wild_encounters/wild_encounters_to_header.py`: le tabelle selvatiche FireRed di Kanto entrano
  in Emerald. Sono quelle vanilla, `--wild-version leafgreen` è un'alternativa.
- `include/constants/flags.h`, più il nuovo `flags_kanto.h`: un blocco di **0x200 flag salvati** subito
  dopo quelli di Emerald. SaveBlock1 cresce di 64 byte (erano liberi 304 byte), quindi i vecchi salvataggi
  non sono più compatibili.
- `data/event_scripts.s`: un `.include "data/kanto_port_scripts.inc"` (lista generata) più gli script
  "flavor text" FRLG.
- Ritocchi a runtime (`field_specials.c`, `field_effect.c`, `field_control_avatar.c`): PC acceso e spento,
  monitor del Centro Pokémon e metatile "flavor text" in versione FRLG sulle mappe di Kanto.
- `src/data/region_map/region_map_sections.json`: nomi italiani delle sezioni di Kanto, solo dove sono
  ancora in inglese.

**Per mappa:**

- `map.json`: `include_in_emerald` ed eventi resi validi. I dati originali restano in `"kanto_port_orig"`.
- `scripts.inc`: lo stub. L'originale FRLG resta in `scripts_frlg_orig.inc`, che non viene compilato.
- `layouts.json`: 180 layout marcati.

**Collegamento Hoenn ↔ Kanto:**

- `data/scripts/kanto_travel.inc`: è un file nuovo e contiene tutta la logica.
- `data/maps/LilycoveCity_Harbor/scripts.inc`: **una sola riga** dopo `lock`/`faceplayer` di
  `LilycoveCity_Harbor_EventScript_FerryAttendant`:

  ```
  call_if_set FLAG_IS_CHAMPION, KantoTravel_EventScript_LilycoveOffer @ KANTO_PORT travel to Kanto
  ```

  Se la riga va in conflitto con la riscrittura italiana di Hoenn, basta reinserirla.

Il viaggio funziona così:

- A Porto Alghepoli, da Campione, l'addetta propone prima "Vuoi salpare per Kanto?". Con SÌ parte
  l'imbarco SS Acqua e si arriva al porto di Aranciopoli (24,32). Con NO si torna al menu normale dei
  traghetti.
- Il marinaio del porto di Aranciopoli (`VermilionCity_Frlg_EventScript_FerrySailor`) riporta il
  giocatore a Porto Alghepoli (8,11).
- Al primo viaggio `KantoPort_EventScript_InitFlags` imposta i flag "nascosto all'inizio" di FRLG.

## Regole degli eventi

| Evento | Come diventa |
|---|---|
| Oggetti, cartelli, trigger | Stub `<Mappa>_EventScript_<Nome>` con testo segnaposto. Il commento sopra l'etichetta riporta lo script FRLG originale, il flag e l'allenatore. |
| Infermiere | Script dell'infermiera di Emerald, funzionante. Ogni Centro Pokémon 1F ha `setrespawn` e `CableClub_OnResume`. |
| Commessi dei Market | `pokemart` con la lista di strumenti originale FRLG. |
| Ball a terra (129) | `Common_EventScript_FindItem` con lo strumento originale e un flag `FLAG_KANTO_*`: funzionano. |
| Oggetti nascosti (124) | Restano con lo strumento originale e un flag `FLAG_KANTO_HIDDEN_ITEM_*`: funzionano. |
| Flag FRLG degli oggetti | Diventano `FLAG_KANTO_<nome originale>`: 333/512 usati, il resto è libero. |
| Allenatori (344) | Messi a `TRAINER_TYPE_NONE`. Tipo, visuale e `TRAINER_` originali stanno in `kanto_port_orig` e in mappe.json. |
| Coord trigger (159) | Tenuti ma **disattivati** (`VAR_TEMP_F == 0x7FFF`). Var e valore originali stanno in `kanto_port_orig`. |
| "Pokémon Journal" (oggetti gfx 0) | Diventano cartelli. |
| Oggetti `OBJ_EVENT_GFX_VAR_n` | Lo stub imposta `VAR_OBJ_GFX_ID_n` come faceva l'originale. |
| Addetti link del 2F | Stub. Le stanze link non esistono e i loro warp puntano su se stessi. |

Gli allenatori non sono script `trainerbattle` perché nel build Emerald le costanti FRLG (es.
`TRAINER_LASS_ROBIN = 29`) coincidono con gli ID degli allenatori di Hoenn. Ogni flag FRLG coincide
anche con un flag di Emerald.

## Verifica (emulatore, `tools/emu/run.py`)

Gli script di test sono in `tools/kanto/emu/`.

- **253/253 mappe** raggiunte con l'hook di warp di test, con screenshot, senza crash e con la grafica
  corretta (`/home/user/work/kanto-out/full/*.png`, provini `_sheet_*.png`).
- **Warp:** 964 test, uno per ogni porta, scala, tappetino e connessione. 479 coppie mappa→destinazione
  su 491 funzionano camminando. Le 12 che non funzionano sono chiuse in FRLG stesso: le porte dei
  Superquattro le aprono gli script della Lega, Villa 1F→3F è a senso unico e c'è la guardia/sporgenza
  del Percorso 23. I pezzi laterali dei tappetini d'uscita sono muri anche in FRLG.
- **Connessioni:** camminate tutte. Fanno eccezione Pista Ciclabile (serve la bici), Percorso 20 → Isola
  Cannella (serve Surf) e le porte di Zafferanopoli (guardie).
- **Interazioni provate:** ball (trovata e poi sparita), oggetto nascosto, cura dall'infermiera, Market
  (lista strumenti), cartello, teletrasporti della palestra di Zafferanopoli, lotta selvatica (Caterpie
  Lv4 nel Bosco Smeraldo), viaggio di andata e ritorno Porto Alghepoli ↔ Aranciopoli, salvataggio a Kanto
  e CONTINUA.
- **Dimensione ROM:** 27.941.672 B su 32 MB, pari all'**83,27%**. Il baseline è al 79,72%, quindi Kanto
  occupa circa 1,19 MB.
- **Falso positivo del test:** nei Centri Pokémon, anche quelli di Hoenn, il controllo Union Room imposta
  `gWirelessCommType=1` e il gioco aspetta il VBlank girando a vuoto. L'harness lo segnala come `EVENT
  stuck`, ma il comportamento è vanilla e non è un blocco.
- **Bug trovato e risolto:** senza `CableClub_OnResume` l'infermiera di Emerald copiava in `gStringVar1`
  un nome Union Room non terminato. La copia azzerava `gFonts` e da lì in poi tutti i testi uscivano
  vuoti.

## Lacune / da fare (scrittura e logica)

**Testi e allenatori**

- **Testi:** tutti segnaposto, da scrivere in italiano.
- **Allenatori:** vanno creati nuovi allenatori in `src/data/trainers.party` (le squadre di riferimento
  sono in `trainers_frlg.party`). Bisogna alzare `MAX_TRAINERS_COUNT`, il che costa flag e byte di
  salvataggio. Poi si rimettono `trainer_type` e la visuale in map.json.

**Var e flag di trama**

- Le var FRLG coincidono con var di Emerald, quindi servono var nuove (`VAR_UNUSED_*` di Emerald, 29
  libere) per le scene e per riattivare i coord trigger.
- I flag di trama si aggiungono in coda a `include/constants/flags_kanto.h` (indici contigui). Lo
  strumento conserva le aggiunte.

**Puzzle e meccaniche** (special FRLG per mappa: `orig_specials_frlg_only` in mappe.json; gli special
FRLG sono comunque compilati anche in Emerald)

- **Palestra di Aranciopoli** (cestini): la logica originale usa solo `VAR_TEMP_0/1`, `setmetatile`,
  `special SetVermilionTrashCans` (C condiviso, disponibile) e un flag. Si può portare quasi 1:1
  dall'originale, rinominando il flag in `FLAG_KANTO_*`.
- **Palestra di Isola Cannella** (quiz): solo script, `VAR_TEMP_1`, `setmetatile` e 6 flag
  `FLAG_CINNABAR_GYM_QUIZ_*` da rifare come `FLAG_KANTO_*`.
- **Palestra di Zafferanopoli** (teletrasporti): funziona già, sono warp.
- **Palestra di Smeraldopoli e Rifugio Rocket** (tasselli rotanti): il codice del campo è condiviso, ma
  non è stato provato.
- **Ascensori** (Centro Commerciale, Silph, Rifugio Rocket): destinazione `MAP_DYNAMIC` più gli special
  `AnimateElevator`/`DrawElevatorCurrentFloorWindow`... Va riscritta la logica, usando come modello
  l'ascensore del Centro Commerciale di Porto Alghepoli.
- **Silph SpA:** le porte a Card Key si aprono negli `ON_LOAD` dell'originale, che lo stub non ha.
- **Lega** (porte dei Superquattro), **Torre Pokémon** (Silph Scope e Marowak), **Isole Spumarine**
  (correnti `SeafoamIslandsB4F_CurrentDumpsPlayerOnLand`) e **Pista Ciclabile** (`ForcePlayerOntoBike`):
  la logica va riscritta.
- **Zona Safari:** `EnterSafariMode`/`ExitSafariMode` di Emerald rimandano all'ingresso Safari di Hoenn,
  quindi serve una variante.
- Gli ostacoli di trama (Snorlax, guardie, Rocket) hanno flag `FLAG_KANTO_HIDE_*` già pronti.

**Interfaccia**

- **Volo e mappa regione:** il build ha solo la mappa di Hoenn. Le MAPSEC di Kanto hanno coordinate
  della mappa di Kanto e non ci sono punti di volo a Kanto. Lo sblocco del volo e una mappa di Kanto sono
  da progettare, mentre il respawn dopo una sconfitta funziona già (`setrespawn`).
- Il popup "CELADON DEPT." è una stringa inglese fissa in `src/map_name_popup.c`.

**Non fatto**

- Isole Sette, Torre Allenatori e M/N Anna come evento di trama (le mappe ci sono).
