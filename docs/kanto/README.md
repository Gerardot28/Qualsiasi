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

## Atto 2 (v2): infrastruttura di gioco

Dalla v2 l'albero principale `/home/user/pex` contiene Kanto giocabile: allenatori, Palestre, Lega, trama
(scene K01-K31 di `docs/storia/kanto_scene.json`), gating, selvatici, Volo Taxi. Nella v2 **tutti i dialoghi
sono scritti** (nessun segnaposto rimasto, vedi sotto). Fonte di trama: `docs/storia/bibbia_kanto.md`.

### Come si rigenera (tutto idempotente)

```sh
python3 tools/kanto/port_kanto.py --tree /home/user/pex --all       # mappe (non tocca stub già scritti)
python3 -I tools/kanto/kanto_trainers.py --tree /home/user/pex      # allenatori + script trainerbattle
python3 -I tools/kanto/kanto_story.py --tree /home/user/pex         # Palestre, trama, gating, Lega, taxi
python3 -I tools/kanto/kanto_wild.py --tree /home/user/pex          # selvatici di Kanto
python3 -I tools/kanto/list_placeholders.py --tree /home/user/pex   # elenco testi da scrivere
cd /home/user/pex && make -j4 && make release -j4
```

Rilanciare gli strumenti dopo che gli scrittori hanno lavorato è sicuro: `kanto_trainers.py` converte solo
gli stub ancora intatti; `kanto_story.py` riscrive solo il blocco `@ KANTO_V2 BEGIN … END` in coda a ogni
`scripts.inc` e **conserva i testi già scritti** (quelli senza `[TESTO:`). Chi vuole modificare la *logica*
di uno di quei blocchi deve farlo in `tools/kanto/kanto_story.py`, non nel file generato.

### Testi da scrivere (scrittori)

Elenco completo, per file, in **`docs/kanto/testi_da_scrivere.md`** (generato da `list_placeholders.py`):
239 file, **1248 etichette `[TESTO: …]`** (con la descrizione di cosa deve dire il testo) e 792 etichette
generiche del port (`(testo Kanto da scrivere)`, NPC/cartelli/oggetti). Una etichetta = un testo.

- Allenatori: per ognuno `…_Text_<Nome>Intro`, `…Defeat`, `…PostBattle` (+ `…NotEnough` per le lotte in
  doppio), nello `scripts.inc` della mappa.
- Capipalestra: `…_Text_<Capo>Intro/Defeat/ExplainBadge/ExplainTM/PostBattle`, `…_Text_ReceivedBadge`,
  Guida (`…_Text_GymGuy`, `…GymGuyPostVictory`), statue (`…_Text_GymStatue`, `…Certified`).
- Scene di trama: etichette `…_Text_…` con prefisso della scena (`K02`…`K31`) nella descrizione.
- Testi condivisi (taxi, ascensori, interruttori della Villa, voce della Lega): `data/scripts/kanto_story.inc`
  (file generato: per questi testi cambiare le descrizioni in `kanto_story.py`, oppure chiedere di spostarli).
- I segnaposto si scrivono `[TESTO: …]` nel sorgente. **Stato v2: tutti scritti** (`grep -rl "TESTO:\|testo
  Kanto da scrivere" data/` è vuoto); i testi condivisi di `kanto_story.inc` stanno in `SHARED_TEXTS` di
  `kanto_story.py`. La mappatura temporanea di `[`/`]` in `charmap.txt` è stata tolta: per generare nuovi
  segnaposto va rimessa (`'['` = 5C, `']'` = 5D) finché non sono scritti.

### Allenatori

- `tools/kanto/kanto_trainers.py`: un allenatore `TRAINER_KANTO_<nome FRLG>` per ognuno dei **343 allenatori
  distinti** dei 351 oggetti allenatore di Kanto (330 normali + 13 boss già oggetto), più 7 boss di trama:
  Giovanni Silph, Campione Blu, Arianna Silph ×3 (squadra secondo `VAR_STARTER_MON`, come a Hoenn),
  Morgana ×2 (Monte Luna, Grotta Celeste). Totale **350** voci in coda a `src/data/trainers.party`.
- Costanti in `include/constants/opponents.h` (blocco `KANTO_TRAINERS`), `TRAINERS_COUNT_EMERALD` 1205,
  **`MAX_TRAINERS_COUNT_EMERALD` 864 → 1216**: +352 flag allenatore = **+44 byte** di SaveBlock1.
- **Salvataggio:** SaveBlock1 = 15.676 B su 15.872 disponibili (4 settori Flash), **liberi 196 B** (erano 304
  prima di Kanto, 240 dopo i flag di Kanto). Il layout Flash 128K (14 settori + 2 di backup) non cambia.
  I salvataggi della v1 non sono compatibili (già vero dal port).
- Squadre normali: generatore di Hoenn (`tools/balance/trainer_gen.py`) con temi per classe FRLG, peso ×4
  alle famiglie Gen 1-2 (resto di tutte le generazioni), 2-4 Pokémon (Fantallenatori 3+), IV 20 (22 forti),
  AI `Basic Trainer`. Livelli per fase della bibbia (§7): Aranciopoli/P6/P11/M/N Anna 53-54 · Celestopoli/
  P4-5/P24-25/Monte Luna 55-56 · Plumbeopoli/P1-3/P22/Bosco 56-57 · Azzurropoli/Lavandonia/P7-10/Torre/
  Rifugio 58-59 · P12-18/Fucsiapoli 60-61 · Zafferanopoli/Silph 61-62 · P19-21/Cannella/Villa/Spumarine
  62-63 · Grotta Celeste 64-65 · Via Vittoria/P23 66-67. Allenatori di Palestra: asso del Capo −3/−2.
- Boss scritti a mano in **`tools/kanto/kanto_bosses.party`** (mosse riempite/validate da `process_boss`):
  Lt. Surge 55, Misty 57, Brock 58, Erika 59, Koga 61, Sabrina 63, Blaine 64, Giovanni 66 (5-6 Pokémon,
  tipi canonici, strumenti tenuti, `Smart Trainer`), Lorelei 67, Bruno 68, Agatha 69, Lance 71, Blu 72
  (nucleo di Kanto + tutte le generazioni), Giovanni Rifugio 60 / Silph 62, Arianna 62, Morgana 57/66.
- Classi e sprite FRLG (`TRAINER_PIC_*_FRLG`); Blu usa la classe Emerald `Champion` ("Campione") perché
  `CHAMPION_FRLG` mostra il nome del rivale salvato (Arianna). `TRAINER_CLASS_BOSS_FRLG` = **"Capo Rocket"**.
  Morgana: classe Admin Alpha, sprite Recluta Alpha (F). Arianna: classe Rivale ("Sfidante"), sprite May.
- `map.json`: tipo/visuale originali ripristinati per tutti gli allenatori normali (doppi: `trainerbattle_double`).

### Medaglie, MT e livelli massimi

- Medaglie: `FLAG_KANTO_BADGE01` (Sasso) … `08` (Terra), ordine canonico; ordine di trama Surge → Misty →
  Brock → Erika → Koga → Sabrina → Blaine → Giovanni. MT: Surge MT24 Fulmine, Misty MT18 Pioggiadanza,
  Brock MT37 Terrempesta, Erika MT19 Gigassorbimento, Koga MT06 Tossina, Sabrina MT29 Psichico, Blaine MT38
  Fuocobomba, Giovanni MT26 Terremoto (flag `FLAG_KANTO_GOT_TM_*`; se la borsa è piena la MT si riceve
  riparlando). Vincere imposta anche i flag degli allenatori della Palestra.
- `src/caps.c`: dopo `FLAG_IS_CHAMPION`, se `FLAG_KANTO_STORY_CROSSED`, il livello massimo dipende dal
  **numero** di Medaglie di Kanto: 55, 57, 58, 59, 61, 63, 64, 66; con 8 Medaglie 67 (Lorelei), 68 (Bruno),
  69 (Agatha), 71 (Lance), 72 (Blu); dopo `FLAG_KANTO_STORY_CHAMPION` nessun limite. Nota: arrivando a Kanto
  il limite scende da 100 a 55 (scelta della bibbia: la squadra di Hoenn non guadagna esperienza oltre).

### Palestre

| Palestra | Logica |
|---|---|
| Aranciopoli | Cestini **portati 1:1** da FRLG (`special SetVermilionTrashCans`, `FLAG_KANTO_VERMILION_GYM_SWITCHES`). Vittoria: toglie la quarantena (`FLAG_KANTO_HIDE_VERMILION_QUARANTINE`, `VAR_KANTO_STORY`=2) |
| Isola Cannella | Quiz: 6 macchine, 6 flag `FLAG_KANTO_CINNABAR_GYM_QUIZ_1..6`. Risposta giusta = porta aperta; sbagliata = lotta con l'allenatore della porta (anche batterlo apre). Risposte: 1 SÌ, 2 NO, 3 NO, 4 NO, 5 SÌ, 6 NO (indicate nei segnaposto delle domande) |
| Zafferanopoli | Teletrasporti (warp, già funzionanti). Sabrina rifiuta la lotta senza `FLAG_KANTO_STORY_SILPH_FREED` |
| Smeraldopoli | Frecce rotanti: codice di campo condiviso (non percorsa a piedi nei test). Porta chiusa (trigger 36,11) senza 7 Medaglie + `FLAG_KANTO_STORY_OAK_MET` |
| Azzurropoli | Alberi Taglio: funzionano con le Medaglie di Hoenn |
| altre | nessun puzzle |

### Lega (Altopiano Blu)

- Atrio (`IndigoPlateau_PokemonCenter_1F`): a ogni ingresso `KantoLeague_EventScript_Reset` azzera
  `VAR_KANTO_LEAGUE` e `FLAG_KANTO_DEFEATED_LORELEI/BRUNO/AGATHA/LANCE` (anche dopo una sconfitta).
- Stanze: logica FRLG riscritta (porte di `data/scripts/pokemon_league.inc` copiate come `KantoLeague_*`):
  entrando si cammina in avanti e l'ingresso si chiude; vinto il Superquattro si apre la porta.
  `VAR_KANTO_LEAGUE`: 0 Lorelei, 1 Bruno, 2 Agatha, 3 Lance, 4 Blu, 5 Sala d'Onore, 6 scena finale, 7 fine.
- Campione: scena all'ingresso (come FRLG), lotta con Blu, Oak entra, Sala d'Onore: congratulazioni,
  `FLAG_KANTO_STORY_CHAMPION`, poi scena dei titoli (K29) davanti all'Altopiano con Blu, Oak e Arianna e il
  testo "Fine dell'Atto 2". Niente crediti (opzionali): il gioco continua (respawn all'Altopiano).

### Trama e gating (kanto_scene.json, 31 scene / 19 righe di gating: tutte implementate)

| Blocco | Implementazione |
|---|---|
| Quarantena di Aranciopoli (K02-K03) | 2 agenti NUOVI (24,1) e (46,19) + trigger su tutte le uscite (y=2 a nord, x=45 a est anche in acqua, quindi niente scorciatoie col Surf). Si toglie battendo Surge |
| Arrivo (K02) | `ON_FRAME` con `VAR_KANTO_STORY`=0: Arianna NUOVA (24,30), testo, VAR=1 |
| Blu a Celestopoli (K04) | trigger (22-24,6) riattivati, con Medaglia Cascata; mostra Morgana/Arianna al Monte Luna |
| Monte Luna B2F (K06) | Morgana (13,9) e Arianna (14,9) NUOVE; lotta, poi i fossili si possono prendere |
| Museo, Bill (K08-K09, opz.) | Rocco NUOVO (12,3) regala la Pietralunare; Bill regala l'Uovo Fortunato |
| Casinò (K11) | la Recluta #11 sparisce dopo la lotta; la scala del Rifugio è nascosta finché non la si batte, poi il poster la apre |
| Rifugio (K12) | ascensore con menu (serve la Chiave Ascensore, ball B4F ora visibile); Giovanni 60 → compare la Spettrosonda |
| Torre (K13-K15) | Blu a 2F (trigger, niente lotta); spettro 6F: senza Spettrosonda respinge, con la Spettrosonda lotta con **Marowak di Alola** Lv 60; 7F: 3 Reclute spariscono dopo la lotta, poi Fuji |
| Casa del Volontariato (K16) | Fuji dà il Poké Flauto; Ettore (9,3) e Ulisse (10,4) NUOVI |
| Snorlax P12/P16 (K17) | senza Flauto dorme; con il Flauto: lotta selvatica Lv 60 e sparisce |
| Cancelli di Zafferanopoli (K18) | 4 trigger "Guard" riattivati, aperti da `FLAG_KANTO_STORY_SAFFRON_OPEN` (Koga) |
| Silph 7F/11F (K19-K20) | Arianna (oggetto di Blu con sprite May): lotta, `NOTEBOOK_BACK`; Giovanni 62: libera Silph e Zafferanopoli, nasconde i Rocket; il Presidente dà la Master Ball |
| Laboratorio di Oak (K22) | Oak (ora visibile) + Arianna NUOVA (5,5); dopo la Silph un Pokémon di Kanto a scelta (Lv 50) |
| Villa (K23) | interruttori delle statue ripristinati (copia di `pokemon_mansion.inc` con `FLAG_KANTO_MANSION_SWITCH`); Chiave Segreta (ball B1F) apre la Palestra di Cannella (trigger 20,5) |
| Grotta Celeste (K25-K26) | la guardia sparisce con la Medaglia Terra; climax: Morgana, Ettore, Ulisse, Arianna NUOVI, trigger (32-34,20); dopo: `RIFT_CALMED`, Mewtwo visibile in B1F |
| Lega (K27) | P22 nord (7,2) chiuso fino a `RIFT_CALMED`; guardie del P23 (trigger riattivati) chiedono la Medaglia 2…8 |
| Leggendari (K30-K31) | Zapdos (Centrale) e Articuno (Spumarine B4F) Lv 65, Mewtwo Lv 70: lotta e l'oggetto sparisce. Moltres non c'è (Isole Sette) |

**Semplificazioni** (documentate, tutte reversibili):

- **Silph S.p.A.:** le porte a Card Key sono aperte (lo stub non le chiude più: il layout le ha aperte).
  Ascensori di Silph, Rifugio e Centro Commerciale: menu dei piani (`KantoElevator_*`, `dynmultichoice`) +
  `setdynamicwarp`, senza animazione.
- **Isole Spumarine:** correnti sempre calme (`setmaplayoutindex …_CURRENT_STOPPED` in B3F/B4F; i 2 layout
  sono ora compilati in Emerald). Articuno si raggiunge col Surf.
- **Pista Ciclabile:** la bici non è obbligatoria (i trigger "NeedBike" restano disattivati).
- **Zona Safari:** niente modalità Safari, lotte normali. **Torre:** gli spettri selvatici sono normali.
- Il Taglio/Forza/Surf/Spaccaroccia funzionano già grazie alle Medaglie di Hoenn.

### Selvatici

`tools/kanto/kanto_wild.py`: riscrive specie e livelli delle **66 tabelle FireRed** delle mappe portate
(Hoenn e Isole Sette intatte), con le regole di `wild_gen.py` (habitat, evoluzioni, potenza, varietà) e
habitat di Kanto (bosco, grotte, Spumarine ghiaccio, Torre spettro, Centrale elettro, Villa, Safari).
Livelli per fase: 50-53, 52-55, 53-56, 55-58, 57-60, 59-62 (P19-21/Cannella/Spumarine), Grotta Celeste
62-66, Via Vittoria/P23 63-66. **69%** degli slot sono specie Gen 1-2. Input sempre vanilla (`pex-orig`).

### Volo a Kanto

La mappa della regione è solo di Hoenn: niente punti di volo di Kanto. Al loro posto c'è il **Volo Taxi**
(NPC NUOVO in ogni Centro Pokémon di Kanto, 10 città + P4 + P10): menu con le sole città già visitate
(`FLAG_KANTO_VISITED_*`, impostati entrando nel Centro Pokémon; Biancavilla entrando in città) e warp
davanti al Centro. Nota: la MN Volo usata a Kanto apre la mappa di Hoenn e porta a Hoenn (comportamento
accettato). Il popup "CELADON DEPT." era già italiano ("Centro Azzurrop.", v1).

### Flag e var nuovi

- `include/constants/flags_kanto.h` (blocco Kanto, append-only): 64 flag nuovi dopo quelli del port,
  **397/512 usati**: i 15 di trama + `FLAG_KANTO_BADGE01..08` di kanto_scene.json, `GOT_TM_*` (8),
  `DEFEATED_LORELEI/BRUNO/AGATHA/LANCE`, `CINNABAR_GYM_QUIZ_1..6`, `VERMILION_GYM_SWITCHES`,
  `VISITED_*` (11), `HIDE_VERMILION_ARIANNA`, `HIDE_CERULEAN_ARIANNA`, `HIDE_MTMOON_SCENE`,
  `HIDE_OAKLAB_ARIANNA`, `HIDE_CAVE_SCENE`, `HIDE_CREDITS_ARIANNA`, `HIDE_FUJI_HOUSE_EXILES`,
  `STORY_INIT_DONE`, `GOT_MASTER_BALL`, `GOT_KANTO_STARTER`, `MANSION_SWITCH`.
- Var: `VAR_KANTO_STORY` = `VAR_UNUSED_0x40F7` (0 prima dell'arrivo, 1 quarantena, 2 dopo Surge),
  `VAR_KANTO_LEAGUE` = `VAR_UNUSED_0x40F8` (vedi Lega).
- Primo viaggio: `KantoTravel_EventScript_LilycoveOffer` chiama anche `KantoStory_EventScript_Init`
  (flag "nascosto all'inizio" della trama, `FLAG_KANTO_STORY_CROSSED`).

### Verifica v2 (emulatore)

ROM di prova (solo test, **non** nell'albero principale): `/home/user/work/kanto-v2-test` = `pex` +
`port_kanto.py --test-start MAP_LILYCOVE_CITY_HARBOR,8,11` + hook di test in `src/overworld.c`/`new_game.c`
(warp, set di flag/var, cura, PP infiniti, squadra di prova, `gKantoTestCap` = livello massimo corrente).
Script e macro: `/home/user/work/kanto-v2-emu` (`mk.py` espande WARP/TALK/BATTLE/KSET/KFLAG), screenshot
e log in **`/home/user/work/kanto-v2-shots/`**. Tutto senza crash:

- nave Porto Alghepoli → Aranciopoli, scena d'arrivo, limite 100 → 55; quarantena (respinge);
- lotta con allenatore (Gentiluomo, Palestra), **Lt. Surge**: Medaglia, MT, quarantena tolta, limite 57;
- lotta selvatica (Noctowl Lv 53, P6); **Volo Taxi** (menu, warp); salvataggio a Kanto e CONTINUA;
- scene: Blu a Celestopoli, Morgana al Monte Luna, Snorlax col Flauto, cancello di Zafferanopoli
  (chiuso/aperto), Arianna alla Silph 7F, spettro della Torre senza Spettrosonda, ascensore della Silph
  (1P → 3P), ascensore del Rifugio senza chiave, interruttore della Villa, Articuno Lv 65;
- **Lega**: 8 Medaglie → limite 67, ingresso da Lorelei (porta d'ingresso chiusa), lotta, porta aperta
  (limite 68), stanza di Bruno, Campione Blu (limite 72), Sala d'Onore, scena finale, `STORY_CHAMPION`.
- **ROM release** (`make release`): 28.291.228 B su 32 MB = **84,31%** (v1+port 83,45%); nuova partita OK.
- **v2 con i testi scritti**: 28.341.776 B = **84,47%**; SaveBlock1 15.676/15.872 B (liberi 196). Ritest
  completo (albero debug `/home/user/work/kanto-v2-dbg`, script `/home/user/work/v2-emu`, schermate
  `/home/user/work/v2-shots/`): nave, quarantena, allenatore, Surge + Medaglia, taxi, Monte Luna, Snorlax,
  cancello, Silph, Lega fino alla scena finale, nuova partita della ROM release fino ad Albanova.

### Lacune note

- Testi: scritti (v2). Nessun credito finale.
- Non provati a piedi: frecce di Smeraldopoli e del Rifugio, percorso completo della Villa e delle Spumarine.
- Tessera Allenatore: non mostra le Medaglie di Kanto. Nessuna mappa/volo di Kanto (solo taxi).
- M/N Anna, Isole Sette, Torre Allenatori, Zona Safari "vera": fuori dall'Atto 2.
