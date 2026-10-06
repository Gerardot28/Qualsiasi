# bps: distribuire la hack come patch BPS

La hack non si distribuisce come file `.gba`. Si pubblica una **patch BPS**
che ogni giocatore applica alla propria copia legale di
**Pokémon - Emerald Version (USA, Europe)**. La patch contiene soltanto le
differenze e i checksum (CRC32) della ROM originale e di quella finale, quindi
non contiene la ROM originale.

| file | a cosa serve |
|------|--------------|
| `setup.sh` | ricostruisce la ROM originale da pret/pokeemerald e compila Flips, tutto in `/home/user/work/bps/` |
| `make_patch.sh` | crea `hack.bps` con Flips e lo verifica prima di pubblicarlo |
| `bps.py` | lettore e applicatore BPS indipendente (Python, senza dipendenze), usato per il doppio controllo e per `info` e `hash` |

Nel repository non devono finire ROM né binari: il `.gitignore` esclude
`*.gba`. La patch `.bps`, che per la baseline pesa circa 16 MB, conviene
pubblicarla come allegato di una release (per esempio GitHub Releases) e non
come file del repository.

---

## 1. Preparazione (una sola volta)

```sh
tools/bps/setup.sh            # Flips e ROM vanilla (circa 3 minuti la prima volta)
tools/bps/setup.sh --flips-only
tools/bps/setup.sh --vanilla-only
```

Lo script fa questi passaggi, che si possono anche eseguire a mano:

```sh
cd /home/user/work/bps
git clone https://github.com/pret/agbcc && git clone https://github.com/pret/pokeemerald
cd agbcc && ./build.sh && ./install.sh ../pokeemerald && cd ..
cd pokeemerald && make -j4 && sha1sum pokeemerald.gba    # deve dare f3ae0881...
cp pokeemerald.gba ../vanilla_emerald.gba
cd .. && git clone https://github.com/Alcaro/Flips flips && make -C flips TARGET=cli CFLAGS=-O2
```

- Risultato: `/home/user/work/bps/vanilla_emerald.gba`, con SHA-1
  `f3ae088181bf583e55daf962a92bb46f4f1d07b7`. È identica byte per byte
  alla cartuccia (`make compare` dà `pokeemerald.gba: OK`).
- Flips si trova in `/home/user/work/bps/flips/flips` (Floating IPS, solo riga
  di comando). Il numero `vN` stampato da `--version` è il numero di commit
  del clone, quindi con un clone `--depth 1` risulta `v1`: è normale.
- Commit testati (6 ottobre 2026): pokeemerald `731ad5bf`, agbcc `da598c1d`,
  Flips `ff216a75`. Se in futuro lo SHA-1 non corrisponde, fare checkout di
  questi commit.

## 2. Creare la patch

```sh
tools/bps/make_patch.sh VANILLA.gba HACK.gba OUT.bps
# esempio (HACK = ROM compilata con `make release`)
tools/bps/make_patch.sh --notes /home/user/work/release/hack.txt \
    /home/user/work/bps/vanilla_emerald.gba /percorso/build/pokeemerald-release.gba \
    /home/user/work/release/hack.bps
```

Per la versione pubblica conviene usare la build `make release` dell'expansion.
Questa build attiva `NDEBUG`/`RELEASE`, toglie le funzioni di debug e produce
`pokeemerald-release.gba`. `make` normale produce invece `pokeemerald.gba`.

Cosa fa:

1. Controlla che `VANILLA` abbia lo SHA-1 dell'Emerald USA/Europe. Se non
   corrisponde si ferma; con `--allow-any-source` dà solo un avviso. Si ferma
   anche se la hack è identica alla vanilla.
2. Esegue `flips --create --bps-delta` e scrive la patch in un file
   temporaneo. Con `--linear` usa la modalità lineare: è più veloce ma la
   patch è molto più grande.
3. **Round trip 1**: `flips --apply` sulla vanilla, poi confronta lo SHA-1 del
   risultato con `HACK`.
4. **Round trip 2**: `python3 -I bps.py verify`, un'implementazione BPS
   indipendente, deve rigenerare `HACK` byte per byte.
5. Solo se entrambi i controlli passano sposta la patch in `OUT.bps`.
   `--notes FILE` scrive anche un testo con dimensione, CRC32, MD5 e SHA-1 di
   ROM richiesta, ROM risultante e patch, da copiare nella pagina della release.

Variabili d'ambiente:

- `FLIPS=/percorso/flips`. Se manca, lo script cerca `flips` nel `PATH` e poi
  `/home/user/work/bps/flips/flips`.
- `PYTHON=` (vuota) salta il controllo con `bps.py`.

Codici di uscita: 0 ok, 1 errore, 2 uso sbagliato.

**Nota sull'header della ROM.** mGBA riconosce le hack di Smeraldo dalla
stringa `pokemon emerald version` all'offset `0x108`
(`.gameName` in `src/rom_header_gf.c`) e attiva da solo il salvataggio Flash
128K e l'RTC. Lo stesso vale per OpenEmu, che usa mGBA come core GBA. Si può
cambiare il titolo interno (`0xA0`), ma conviene lasciare `.gameName` e il
codice `BPEE`. Sono le impostazioni della baseline.

Strumenti di analisi:

```sh
python3 -I tools/bps/bps.py info hack.bps          # dimensioni, CRC32, statistiche dei comandi
python3 -I tools/bps/bps.py hash rom.gba hack.bps  # dimensione, CRC32, MD5, SHA-1 e header GBA
python3 -I tools/bps/bps.py apply hack.bps rom.gba out.gba
python3 -I tools/bps/bps.py verify hack.bps rom.gba atteso.gba
```

### Numeri misurati (expansion 1.17.1 non modificata, `/home/user/work/baseline.gba`)

| | |
|---|---|
| ROM vanilla | 16 777 216 byte, CRC32 `1F1C08FB` |
| ROM baseline | 33 554 432 byte (32 MiB, di cui 25,5 MiB di contenuto), CRC32 `09A410BD`, SHA-1 `e2d4600d…` |
| patch delta (default) | **16 639 427 byte (15,9 MiB)**: creazione 14 s, creazione e doppia verifica 16 s |
| patch lineare | 23 227 108 byte |
| delta compressa | zip 15,1 MiB, xz 14,6 MiB (il contenuto nuovo è già compresso, quindi lo zip serve a poco) |

La patch è grande perché l'expansion ricompila tutto il codice e aggiunge
molti dati nuovi (grafica e cry delle specie fino alla Gen 9): circa 14 MB
sono dati che non esistono nella ROM originale (comandi TargetRead).

Patch applicata e verificata con 4 implementazioni diverse, tutte con
risultato SHA-1 `e2d4600d…` uguale alla baseline:

- Flips
- `bps.py`
- Rom Patcher JS (CLI Node, commit `31838840`)
- il codice di soft-patching di mGBA 0.10.5 (`loadPatch`/`applyPatch`)

`bps.py` è stato confrontato anche con Flips su 50 coppie di file casuali
(delta e linear, inserimenti, cancellazioni e sequenze RLE): 0 errori.

---

## 3. Guida per chi gioca (macOS)

*(Testo da copiare nella pagina di download.)*

### Cosa ti serve

- Il file della patch, `NOME-HACK.bps`.
- **La tua copia** della ROM originale, esattamente questa:

| | |
|---|---|
| Nome (No-Intro) | `Pokemon - Emerald Version (USA, Europe).gba` |
| Dimensione | 16 777 216 byte (16 MB) |
| CRC32 | `1F1C08FB` |
| MD5 | `605b89b67018abcea91e693a4dd25be3` |
| SHA-1 | `f3ae088181bf583e55daf962a92bb46f4f1d07b7` |
| Codice gioco | `BPEE` (titolo interno `POKEMON EMER`) |

Non funzionano:

- la versione italiana *Pokémon Versione Smeraldo* (codice `BPEI`, CRC32
  `A0AEC80A`) e le altre versioni non in inglese;
- Rubino, Zaffiro e Rosso Fuoco;
- ROM già modificate o "pulite" da altri tool.

Se la ROM è dentro un `.zip` o un `.7z`, estrai prima il file `.gba`.

Per controllare la ROM sul Mac, apri il Terminale, scrivi `shasum -a 1 `
(con lo spazio finale), trascina il file nella finestra e premi Invio. Deve
comparire `f3ae088181bf583e55daf962a92bb46f4f1d07b7`. Rom Patcher JS (metodo
A) mostra comunque CRC32, MD5 e SHA-1 appena carichi la ROM.

### Metodo A, consigliato: Rom Patcher JS nel browser

Funziona con Safari, Chrome e Firefox, senza installare niente. I file sono
elaborati nel browser e non vengono caricati su un server. Se il browser è in
italiano, il sito mostra le etichette tradotte, indicate qui tra parentesi.

1. Apri <https://www.marcrobledo.com/RomPatcher.js/>.
2. In **ROM file** (*File ROM*) scegli `Pokemon - Emerald Version (USA, Europe).gba`.
3. In **Patch file** (*File patch*) scegli `NOME-HACK.bps`.
4. Controlla i checksum mostrati sotto la ROM: il CRC32 deve essere
   `1F1C08FB`. Se il campo della ROM diventa rosso o compare
   **"Source ROM checksum mismatch"** (*"Checksum della ROM sorgente non
   valido"*), la ROM è sbagliata. **Fermati**: Rom
   Patcher JS crea comunque un file, ma non funzionerà.
5. Premi **Apply patch** (*Applica patch*). Il browser scarica un file
   `.gba` di 32 MB, per esempio
   `Pokemon - Emerald Version (USA, Europe) (patched).gba`. Puoi rinominarlo,
   per esempio in `NOME-HACK.gba`.

### Metodo B: MultiPatch (app per Mac)

1. Scarica MultiPatch da <https://projects.sappharad.com/tools/multipatch.html>
   oppure da <https://github.com/sappharad/MultiPatch/releases>.
   Al primo avvio macOS può bloccarla: fai clic destro sull'app, scegli
   **Apri** e conferma.
2. Indica la patch (`NOME-HACK.bps`), il file di input (la ROM originale) e il
   file di output (per esempio `NOME-HACK.gba`), poi applica.
3. Se MultiPatch segnala un errore di checksum, la ROM non è quella giusta.

### Metodo C: Flips dal Terminale (per utenti esperti)

Servono gli Xcode Command Line Tools (`xcode-select --install`).

```sh
git clone https://github.com/Alcaro/Flips && cd Flips && make TARGET=cli CFLAGS=-O2
./flips --apply NOME-HACK.bps "Pokemon - Emerald Version (USA, Europe).gba" NOME-HACK.gba
```

Flips rifiuta le ROM sbagliate, per esempio con *"This patch is not intended
for this ROM"*.

### Metodo D: mGBA senza creare un nuovo file (soft-patching)

Metti la patch nella stessa cartella della ROM, **con lo stesso nome**: per
esempio `Smeraldo.gba` e `Smeraldo.bps`. All'avvio mGBA applica la patch da
solo; in alternativa usa *File → Load patch…*. Se la ROM è sbagliata, mGBA
non applica la patch e, senza avvisare, avvia il gioco originale non
modificato. OpenEmu e Delta invece vogliono il file `.gba` già
patchato, quindi per loro usa i metodi A, B o C.

### Giocare

- **mGBA** (<https://mgba.io>, per macOS): apri il file `.gba` patchato.
- **OpenEmu** (<https://openemu.org>): trascina il `.gba` nella libreria. Il
  core GBA di OpenEmu è mGBA.
- **Delta** (iPhone/iPad): passa il `.gba` all'iPhone o all'iPad con
  AirDrop, iCloud Drive o l'app File, poi importalo in Delta con il pulsante
  **+**.

La ROM patchata pesa 32 MB, la dimensione massima per il GBA, ed è
supportata da questi emulatori. Salvataggio e orologio (RTC) funzionano come
in Smeraldo; mGBA li riconosce da solo.

I salvataggi dello Smeraldo originale **non** sono compatibili: inizia una
nuova partita.

**Aggiornamenti.** Applica la nuova patch sempre alla **ROM originale**, non a
quella già patchata. Tieni il file `.sav` con lo stesso nome della ROM se
vuoi provare a mantenere la partita. La compatibilità dei salvataggi tra
versioni diverse della hack non è garantita: lo diranno le note di ogni
versione.

### Problemi comuni

| sintomo | causa e rimedio |
|---|---|
| "Source ROM checksum mismatch" (*"Checksum della ROM sorgente non valido"*) o campo rosso in Rom Patcher JS | ROM sbagliata (italiana, già modificata, con un altro dump). Usa la ROM con CRC32 `1F1C08FB` |
| "This patch is not intended for this ROM" (Flips o MultiPatch) | come sopra |
| schermo bianco o crash all'avvio | la patch è stata applicata a una ROM sbagliata, oppure a una ROM già patchata |
| il file selezionato è `.zip` o `.7z` | estrai prima il `.gba` |
| la patch è stata applicata due volte | riparti dalla ROM originale |
