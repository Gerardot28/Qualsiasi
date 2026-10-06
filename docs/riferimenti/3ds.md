# Giocare a Pokémon Multiverse su Nintendo 3DS (guida per utenti macOS)

Riferimento tecnico e guida passo passo. Le parti sul codice sono verificate sul sorgente di pokeemerald-expansion 1.17.1 (`/home/user/pex-orig`) e sul nostro build (`/home/user/pex`). Le parti sugli strumenti 3DS vengono:

- dai README ufficiali su GitHub: [open_agb_firm](https://github.com/profi200/open_agb_firm), [mGBA](https://github.com/mgba-emu/mgba) (`CHANGES` e sorgenti del port 3DS e del forwarder);
- dove indicato **(da verificare)**, dalla conoscenza della community. Le wiki e GBAtemp non sono raggiungibili da qui.

## In breve

| Metodo | Come gira | RTC (orologio) | Salvataggi | Prestazioni | Su Mac | Consigliato per |
|---|---|---|---|---|---|---|
| **A. open_agb_firm** | hardware GBA nativo della 3DS (payload FIRM di Luma3DS) | **sì** (`saveType=flash_1m_macronix_rtc`) | `.sav` direttamente su SD | identiche al GBA | **sì, basta copiare file** | **tutti: è la scelta migliore** |
| **B. Iniezione VC (AGB_FIRM)** con NSUI | hardware GBA nativo, icona nel menu HOME | **no** (AGB_FIRM non emula l'RTC) | salvataggio interno al titolo, scritto all'uscita | identiche al GBA | NSUI è solo Windows: serve Wine/CrossOver o una VM | chi vuole l'icona in HOME e non usa open_agb_firm |
| **C. mGBA per 3DS** | emulatore (homebrew) | sì (orologio della console) | `.sav` su SD + savestate | **New 3DS: piena velocità; Old 3DS: spesso sotto il 100%** | sì, basta copiare file; forwarder `.cia` dalle build di sviluppo di mGBA per macOS | New 3DS, chi vuole savestate e velocizzazione |

Il nostro build ha già **`OW_USE_FAKE_RTC` TRUE**. La stessa ROM funziona quindi in tutti e tre i metodi, compreso AGB_FIRM senza RTC, senza il messaggio «La batteria interna è scarica» e con bacche ed eventi a tempo funzionanti.

---

## 1. Requisiti del ROM (verificati sul codice)

- **Dimensione massima: 32 MiB** (33.554.432 byte). È il limite fisico dello spazio cartuccia GBA e vale per AGB_FIRM, open_agb_firm («>32 MiB games» è tra le *Hardware Limitations*) e iniettori. Oggi il ROM occupa ~26,8 MB, il 79,9% (misura del linker `--print-memory-usage`). Il `.gba` è già riempito fino a 32 MiB.
- **Tipo di salvataggio: Flash 1 Mbit = 128 KB**, come Smeraldo. Il codice include la libreria Nintendo `FLASH1M_V103` (`src/agb_flash_1m.c`, stringa marcata `KEEP_SECTION USED`), con i driver Macronix (`agb_flash_mx.c`) e Sanyo (`agb_flash_le.c`). Gli iniettori e open_agb_firm riconoscono il tipo proprio da quella stringa: **non va rimossa**.
- **Codice gioco**: il Makefile usa `GAME_CODE ?= BPEE` (codice di Smeraldo USA). Tenerlo così: mGBA e alcuni strumenti applicano le impostazioni di Smeraldo (Flash 1M + RTC) in base al codice. `TITLE` è già `PKMULTIVERSE`.
- **Niente cavo link e niente adattatore wireless** su 3DS, in nessuno dei tre metodi. Union Room, scambi e lotte link non sono utilizzabili.

## 2. Il problema dell'RTC e la soluzione dell'expansion (`OW_USE_FAKE_RTC`)

Smeraldo legge l'orologio della cartuccia (chip Seiko S-3511 via GPIO, `src/siirtc.c`). **AGB_FIRM non lo emula**. Con un ROM vanilla si ottengono:

- il messaggio «La batteria interna è scarica. È possibile giocare. Tuttavia, non si verificheranno gli eventi collegati all'orologio.» (testo ufficiale IT di Smeraldo) (`gText_BatteryRunDry`, `Task_MainMenuCheckBattery` in `src/main_menu.c`);
- orologio fermo: bacche che non crescono, maree della Grotta Ondosa bloccate, Isola Miraggio e lotteria giornaliere ferme, nessun ciclo giorno/notte.

**Come funziona il fake RTC dell'expansion** (`include/config/overworld.h`, `src/fake_rtc.c`, `src/rtc.c`, `src/play_time.c`):

- `#define OW_USE_FAKE_RTC TRUE` fa sì che `RtcInit()` esca subito, senza nessun accesso GPIO al chip. `RtcGetErrorStatus()` restituisce sempre 0, quindi il messaggio sulla batteria non compare più. `RtcGetInfo()` legge l'ora da `gSaveBlock3Ptr->fakeRTC`, cioè **l'ora è salvata nel salvataggio**.
- L'orologio avanza **solo mentre si gioca**: `PlayTimeCounter_Update()`, ogni 60 frame (1 s), chiama `FakeRtc_TickTimeForward()`, che aggiunge `FakeRtc_GetSecondsRatio()` secondi.
- Velocità del tempo, `OW_ALTERED_TIME_RATIO`:
  - `GEN_LATEST` (= `GEN_9`): **20×**. 1 s reale = 20 s di gioco, una giornata dura **72 minuti** di gioco. È l'impostazione attuale.
  - `GEN_8_PLA`: 60×, una giornata ogni 24 minuti.
  - `TIME_DEBUG`: 1:1, cioè tempo reale, ma solo mentre si gioca.
- Strumenti per gli script (`asm/macros/event.inc`):
  - `pausefakertc` / `resumefakertc` / `togglefakertc`: fermano o riprendono il tempo, insieme al flag `OW_FLAG_PAUSE_TIME`.
  - `fwdtime ore, minuti`: avanza fino a un orario, utile per un NPC «salta alla notte» o un oggetto orologio.
  - `fwdweekday giorno`: avanza fino a un giorno della settimana.
  - Esiste uno `STATIC_ASSERT`: `OW_FLAG_PAUSE_TIME` può valere ≠ 0 solo se `OW_USE_FAKE_RTC` è TRUE.
- Su mGBA e open_agb_firm l'RTC vero funzionerebbe, ma con il fake RTC attivo il gioco lo ignora. Il vantaggio è **un unico ROM per tutti i metodi** e il comportamento identico ovunque.

**Impostazioni consigliate** per la massima compatibilità con 3DS:

```c
// include/config/overworld.h
#define OW_USE_FAKE_RTC         TRUE        // OBBLIGATORIO per AGB_FIRM (iniezione VC); già attivo
#define OW_ALTERED_TIME_RATIO   GEN_9       // 20x: giorno di 72 min di gioco (buono per DNS e bacche)
#define OW_FLAG_PAUSE_TIME      FLAG_...    // un flag libero: ferma il tempo nelle cutscene/al chiuso se serve
```

- Prevedere in gioco un modo per cambiare l'ora: un NPC o un orologio in camera che usa `fwdtime`. È utile per gli eventi notturni e per gli incontri per fascia oraria se si attiva `OW_TIME_OF_DAY_ENCOUNTERS`.
- La scena del **«regola l'orologio»** a inizio Smeraldo (orologio a muro in camera) **continua a funzionare** e conviene tenerla. Con il fake RTC, `RtcCalcLocalTimeOffset()` (`src/rtc.c`) chiama `FakeRtc_ManuallySetTime()`, quindi l'ora scelta dal giocatore diventa l'ora iniziale dell'orologio interno. È il modo naturale per far scegliere l'orario di partenza.

## 3. Altre impostazioni di build consigliate per l'hardware vero

| Impostazione | Valore | Perché |
|---|---|---|
| `make release` per la build da distribuire | — | Definisce `RELEASE` → `NDEBUG`. Disattiva i menu di debug (`DEBUG_OVERWORLD_MENU`, `DEBUG_BATTLE_MENU`, `DEBUG_POKEMON_SPRITE_VISUALIZER` sono `DISABLED_ON_RELEASE`) e il Quickstart, e toglie il printf di debug: `general.h` avverte che *«Some emulators or real hardware might (and is allowed to) crash»* con gli handler di stampa. Abilita anche l'LTO (`USE_LTO_ON_RELEASE`), quindi binario più efficiente. |
| `OW_USE_FAKE_RTC` | `TRUE` | vedi §2 |
| Dimensione ROM | ≤ 32 MiB, con margine | tenere almeno 1–2 MB liberi per patch future; vedi `grafica.md` per le voci che pesano di più |
| `GAME_CODE` | `BPEE` | rilevamento automatico di Flash 1M e RTC nei tool |
| IA molto complessa (`AI_FLAG_PREDICTION`…) | misurare | sull'hardware vero (AGB_FIRM e open_agb_firm sono GBA reali) il tempo di calcolo dell'IA si vede. Misurarlo con `DEBUG_AI_DELAY_TIMER` (debug.h) prima di attivarla per tutti |
| `OW_OBJECT_VANILLA_SHADOWS` | `FALSE` (default) va bene | ogni oggetto usa 2 sprite: con tanti NPC, follower e OWE c'è più rischio di sfarfallio, ma non è un'incompatibilità |
| Funzioni link (Union Room, scambi) | lasciarle, ma non renderle obbligatorie | su 3DS il link non esiste |

---

## 4. Guida passo passo per macOS

### 4.0 Prerequisito comune: 3DS con Luma3DS

Tutti e tre i metodi richiedono una 3DS (o 2DS / New 3DS / New 2DS XL) con **custom firmware Luma3DS**. Si installa seguendo la guida ufficiale della community (in italiano): **https://3ds.hacks.guide/it_IT/** (non raggiungibile da qui, da verificare). È un passaggio a parte: lo diamo per fatto.

Dal Mac servono solo un lettore di schede SD (microSD per i New 3DS) e il Finder.

> Nota per Mac: formattare la SD con «Utility Disco» come **MS-DOS (FAT32)**, schema «Master Boot Record». Per schede oltre 32 GB Utility Disco a volte non offre FAT32 direttamente: in quel caso usare `diskutil` da Terminale o uno strumento dedicato **(da verificare sulla guida 3ds.hacks.guide)**. Dopo aver copiato i file, **espellere la scheda** dal Finder prima di toglierla: macOS crea file `._*` e una scrittura interrotta può corrompere la SD.

### 4.A Metodo consigliato: open_agb_firm (nativo, con RTC, nessuna iniezione)

1. Scaricare l'ultima release da https://github.com/profi200/open_agb_firm/releases/latest ed estrarla.
2. Con la SD nel Mac:
   - copiare `open_agb_firm.firm` in **`/luma/payloads/`**;
   - copiare la cartella **`3ds`** della release nella radice della SD, unendola a quella esistente.
3. Creare la cartella `/roms/gba/`, o un'altra a scelta, e copiarci **`pokemon_multiverse.gba`**. Se distribuiamo una patch: open_agb_firm applica da solo le patch **IPS e UPS** che hanno lo stesso nome del ROM (`pokemon_multiverse.ups` accanto a `pokemon_multiverse.gba`). Tenendo X premuto all'avvio la patch non viene applicata.
4. **Impostazioni per il gioco**: creare il file `/3ds/open_agb_firm/saves/pokemon_multiverse.ini` (stesso nome del ROM) con:

   ```ini
   [game]
   saveType=flash_1m_macronix
   ```

   - Con il nostro build (fake RTC) basta `flash_1m_macronix`.
   - Se un giorno si producesse una variante con RTC vero (`OW_USE_FAKE_RTC FALSE`), usare `flash_1m_macronix_rtc`.
   - Il tipo va scritto esplicitamente: il ROM non è nel database `gba_db.bin` (che contiene solo giochi ufficiali) e l'autorilevamento funziona ma non sa dire se c'è l'RTC.
5. Avvio: spegnere la console, poi **tenere premuto START mentre la si accende**. Luma apre il payload di avvio rapido (se START è assegnato a `open_agb_firm.firm`, oppure dal menu di scelta dei payload). Nel file browser scegliere il `.gba`.
6. Salvataggi: si salva in gioco come al solito. Il file `.sav` (128 KB) viene scritto **direttamente sulla SD** in `/3ds/open_agb_firm/saves/`, perché `useSavesFolder=true` è il default. Per uscire **tenere premuto POWER** per spegnere: il README indica che il cambio gioco richiede un riavvio. Per sicurezza salvare in gioco **prima** di spegnere.
7. Opzionale, in `/3ds/open_agb_firm/config.ini`:
   - `[video] colorProfile=gba` (o `nds`) per colori più fedeli;
   - `scaler=matrix` (default) oppure `none` per l'1:1 con bordo;
   - `[general] directBoot=true` per saltare l'intro del BIOS.
8. Il salvataggio è un normale `.sav` Flash 128 KB, compatibile con mGBA su Mac: si può copiare avanti e indietro tra Mac e 3DS.

Limiti noti (README di open_agb_firm):

- niente savestate;
- modalità riposo non completa;
- audio con aliasing (bug dell'hardware);
- niente link;
- ROM al massimo 32 MiB.

Vantaggio rispetto ad AGB_FIRM: **niente bug delle righe corrotte in fondo allo schermo**, che con AGB_FIRM dipende dalla dimensione del cluster della SD.

### 4.B Iniezione Virtual Console (AGB_FIRM) dal Mac

Lo strumento standard è **New Super Ultimate Injector (NSUI)**, che **funziona solo su Windows** (.NET). Su Mac ci sono tre strade:

1. **Macchina virtuale Windows**. Su Mac Apple Silicon: UTM o Parallels con Windows 11 ARM, che esegue le app x86 in emulazione. Su Mac Intel: Boot Camp o VirtualBox. È la strada più affidabile.
2. **CrossOver o Wine**: NSUI usa .NET Framework. In alcuni casi funziona installando il runtime .NET in una «bottle», ma **non è garantito (da verificare)**.
3. **Farsi generare il `.cia`** da qualcuno con Windows. Noi potremmo distribuire un `.cia` ufficiale di Pokémon Multiverse, così l'utente Mac non deve iniettare nulla.

Procedura con NSUI (da verificare sull'ultima versione dell'interfaccia):

1. In NSUI: **GBA → scegliere `pokemon_multiverse.gba`**.
2. Titolo «Pokémon Multiverse», editore, icona 48×48 e banner (opzionali).
3. **Tipo di salvataggio: Flash 1 Mbit (128 KB)**. NSUI di solito lo rileva da solo dalla stringa `FLASH1M_V`; controllare che non scelga SRAM o EEPROM.
4. Generare il `.cia`, copiarlo sulla SD (per esempio in `/cia/`) e installarlo con **FBI** o con **Gestore di Titoli di Luma** dal menu HOME.
5. Avviare dall'icona nel menu HOME.
   - Tenendo premuto START o SELECT durante l'avvio si ottiene l'immagine 1:1 non stirata **(da verificare)**.
6. **Salvataggi in AGB_FIRM: punto critico.** Il gioco scrive nella memoria Flash emulata e AGB_FIRM copia i dati nel salvataggio del titolo **quando si esce dal gioco**. Procedura sicura:
   1. salvare in gioco;
   2. premere POWER (pressione breve) e confermare la chiusura del software;
   3. la console si riavvia verso il menu HOME.
   - **Non spegnere** la console tenendo premuto POWER e non togliere la batteria (rischio di perdere i progressi dall'ultimo avvio) **(da verificare)**.
   - Per esportare o importare il `.sav` tra VC ed emulatore usare lo script **GBAVCSM** per GodMode9 (https://github.com/TurdPooCharger/GBAVCSM).
7. Limiti di AGB_FIRM:
   - **niente RTC**: risolto nel ROM con `OW_USE_FAKE_RTC`;
   - niente modalità riposo;
   - niente savestate o punti di ripristino;
   - possibili righe corrotte in fondo allo schermo, legate al cluster della SD: se compaiono, provare a riformattare la SD con un'altra dimensione del cluster (32 KB è la più consigliata) **(da verificare)**, oppure passare a open_agb_firm.

### 4.C mGBA per 3DS (emulatore)

1. Scaricare mGBA per 3DS dalle release di https://github.com/mgba-emu/mgba/releases (pacchetto «3DS»: `.3dsx` per l'Homebrew Launcher, oppure `.cia` da installare con FBI).
2. Copiare il ROM sulla SD, per esempio in `/roms/gba/`, e avviarlo dal file browser di mGBA. Il `.sav` viene creato accanto al ROM. Sono disponibili savestate, velocizzazione e filtri.
3. RTC: mGBA usa l'orologio della console. Con il nostro fake RTC il gioco usa comunque il suo orologio interno.
4. Prestazioni:
   - **New 3DS / New 2DS XL**: mGBA attiva la modalità veloce a 804 MHz (`osSetSpeedupEnable(true)` nel sorgente del port) e gira a piena velocità.
   - **Old 3DS / 2DS**: CPU a 268 MHz, molti giochi GBA vanno sotto il 100%. Le risorse della community indicano mGBA come «New 3DS only» **(da verificare con il nostro ROM)**. Il codice dell'expansion è più pesante di Smeraldo, lascia meno tempo idle alla CPU emulata e quindi il rallentamento può aumentare. Su Old 3DS usare open_agb_firm o AGB_FIRM, che non emulano nulla.
   - Nelle build di sviluppo è arrivata una modalità di sincronizzazione «loose» più veloce (`CHANGES` 0.11: *«3DS: Add faster "loose" sync mode, default enabled»*).
5. **Forwarder `.cia` creato dal Mac**, per avere un'icona Multiverse nel menu HOME che apre mGBA con il nostro ROM:
   - Le **build di sviluppo di mGBA 0.11** per macOS hanno **Strumenti → «Create forwarder…»** (`ForwarderView.ui`). Si sceglie il ROM, il sistema «3DS», titolo e immagini; il programma scarica la base del forwarder («Latest stable/development build») e produce un `.cia`.
   - Servono questi programmi nel `PATH`: **`ctrtool`** e **`makerom`** (da Project_CTR), **`3dstool`** e **`bannertool`**. È quello che chiama `ForwarderGenerator3DS.cpp`. Per Mac esistono build di ctrtool, makerom e 3dstool; bannertool potrebbe andare compilato **(da verificare)**.
   - È la soluzione **tutta macOS** più vicina a un'iniezione VC, ma su Old 3DS ha i limiti di velocità di mGBA.

---

## 5. Quale scegliere

| Console | Scelta consigliata |
|---|---|
| **New 3DS / New 2DS XL** | open_agb_firm, oppure mGBA se si vogliono savestate e velocizzazione |
| **Old 3DS / 2DS** | **open_agb_firm**; in alternativa l'iniezione VC (AGB_FIRM) |
| Voglio l'icona nel menu HOME | iniezione VC con NSUI (VM Windows), oppure forwarder mGBA (solo New 3DS) |

## 6. Checklist di compatibilità per il team

- [x] `OW_USE_FAKE_RTC TRUE` (già attivo).
- [ ] `OW_FLAG_PAUSE_TIME` assegnato a un flag libero, e un NPC o un oggetto con `fwdtime` per cambiare l'orario.
- [ ] Tenere la scena dell'orologio a muro: con il fake RTC imposta l'ora iniziale via `FakeRtc_ManuallySetTime`.
- [ ] Distribuire build fatte con **`make release`**.
- [ ] Tenere il ROM sotto i 32 MiB con un margine di 1–2 MB (controllare `ROM:` in `--print-memory-usage` a ogni build).
- [ ] Non rimuovere la libreria Flash (`FLASH1M_V103`); non cambiare `GAME_CODE`.
- [ ] Testare su hardware vero almeno: avvio, salvataggio e caricamento (open_agb_firm e AGB_FIRM), una lotta con IA «Smart» (tempi di calcolo), mappe con molti follower e NPC (sfarfallio), passaggio giorno/notte.
- [ ] Distribuire la patch in formato **UPS**: open_agb_firm la applica in automatico. Per AGB_FIRM e mGBA la si applica prima sul Mac, per esempio con un patcher web o MultiPatch per macOS.
