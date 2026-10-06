# Pokémon Multiverse — Bibbia della storia (Atto 2: Kanto)

Continua `docs/storia/bibbia.md` (Atto 1), che resta valida per lore, cast di Hoenn e **guida di stile**. Lo scheletro meccanico (scene, flag, gating, Palestre) è in `docs/storia/kanto_scene.json`, l'inventario delle mappe in `docs/kanto/mappe.json`. **Nessuna mappa nuova.** Gli oggetti segnati "NUOVO" vanno aggiunti al `map.json` esistente. Tutti gli altri sono oggetti FRLG già presenti (indice @ x,y).

---

## 1. Premessa (per il README, senza spoiler)

Sopra Hoenn il cielo si è aperto: il **Grande Varco** non è più una Cicatrice, ma una porta. La **M/N Acqua**, rinforzata dalla Devon con lo stabilizzatore di Capitan Stern (la vecchia *Merce Devon*), è la prima nave della storia ad attraversarlo. A bordo ci sei tu, Campione di Hoenn, con Arianna e il suo *Taccuino dei Varchi*. Dall'altra parte c'è un mondo che conosci solo dalle leggende, **Kanto**: un porto, Aranciopoli, e una regione che ha appena visto una nave uscire dal cielo.

Ma il Varco ha due imboccature, e qualcuno a Kanto ha già capito quanto vale una porta tra i mondi… se ne hai la chiave. Otto nuove Medaglie, una nuova Lega, un mondo intero da scoprire. **Atto 2: Kanto.**

---

## 2. Lore di Kanto

- **Il Grande Varco** è un corridoio. L'imboccatura di Hoenn è sopra il mare di Iridopoli; quella di Kanto è sopra il mare a sud di **Aranciopoli**. La M/N Acqua fa la spola tra le due imboccature (la meccanica di `kanto_travel.inc` non cambia).
- **Kanto non ha Ancore.** È una regione "giovane": per millenni l'ha tenuta in equilibrio **Mew, la Prima Forma**. Mew porta in sé il seme di tutti i Pokémon *di tutti i mondi* e scivola tra i mondi senza strapparli. Nessuno lo vede da decenni (aggancio per il sequel).
- **Mewtwo, il Varco vivente.** Alla **Villa Pokémon** di Isola Cannella, anni fa, Mr. Fuji e un giovane Blaine clonarono Mew con i fondi della **Silph S.p.A.** Ne nacque Mewtwo, che ha il potere di Mew ma non il suo equilibrio: dove dorme, la Soglia si assottiglia. Fuggì nella **Grotta Celeste**, che da allora è la "Cicatrice" di Kanto. È da lì che partono quasi tutti i Varchi minori di Kanto, ed è lì che il Grande Varco ha messo radici.
- **I Tre Venti** (Articuno, Zapdos, Moltres) sono i Pokémon del cielo di Kanto. Da quando il Varco si è aperto sono agitati. Articuno è alle **Isole Spumarine**, Zapdos alla **Centrale Elettrica**. Moltres "è su un'isola che non compare su nessuna mappa" (aggancio per le Isole Sette).
- **Pietralunare.** Le pietre del **Monte Luna** sono cadute da un antico Varco, come il Meteorite di Hoenn: vibrano vicino alla Soglia e ne amplificano l'energia.
- **La Serratura Silph.** È un prototipo della Silph basato sulla tecnologia della Master Ball: una "Ball" capace di catturare un Varco e di aprirlo o chiuderlo a comando.
- **Il messaggio dell'Atto 2.** Nell'Atto 1 i Varchi non andavano né murati né spalancati. Ora si aggiunge che **non sono di nessuno**: una porta tra i mondi non si possiede, si custodisce insieme.
- Il lessico (Varco, Varchi minori, Grande Varco, Soglia, Convergenza, Cicatrice) è lo stesso dell'Atto 1. A Kanto i Varchi minori si conoscono, ma sono più rari e più recenti.

---

## 3. Antagonisti: Team Rocket e Morgana

**Team Rocket** (sprite `ROCKET_M`, classe "Team Rocket"). È un'organizzazione criminale di Kanto e non ha ideali: fa affari. Il piano di Giovanni è installare la Serratura Silph sul Grande Varco e trasformarlo in un **casello**: chi vuole passare (Pokémon, merci, persone) paga il Team Rocket. Le Reclute parlano come impiegati di una ditta losca ("carico", "pedaggio", "fattura", "il Capo vuole i numeri"). Sono comiche, ma non stupide.

**Morgana** (ex Admin Alpha, sprite `MAGMA_MEMBER_F`, classe "Admin Alpha", NUOVO oggetto dove serve). Nell'Atto 1 aveva solo il nome. Gelida, analitica, parla a frammenti. Tic: "…Analisi completata.", "Dato.", "Errore." Quando Ettore ha sciolto l'Alpha non l'ha accettato. Con i resti della macchina del Monte Camino ha forzato un Varco minore ed è arrivata a Kanto prima di tutti, sul Monte Luna. Si è venduta al Team Rocket come "consulente sui Varchi": senza di lei la Silph non avrebbe mai finito la Serratura.

**Il colpo di scena:** Morgana non vuole il casello. Vuole il **muro** di Ettore, fatto bene questa volta. Collegata a Mewtwo, la Serratura chiuderebbe il Grande Varco per sempre: Hoenn e Kanto separati, tu e Arianna bloccati a Kanto, i Pokémon in transito nei Varchi minori schiacciati. Battuta chiave: *"Ettore voleva un muro. Ulisse una porta spalancata. Errore doppio. La soluzione è una serratura… e la chiave la tengo io."* Ha usato Giovanni quanto Giovanni ha usato lei.

**Giovanni** (Capo del Team Rocket e Capopalestra di Smeraldopoli). Freddo, elegante, pragmatico, mai sopra le righe. Lo affronti tre volte: Rifugio Rocket, Silph S.p.A., Palestra. Quando scopre il tradimento di Morgana capisce di aver perso il controllo della propria "merce". Dopo la Medaglia Terra scioglie il Team Rocket ("Di nuovo.") e ti dice dov'è Morgana. Non si redime, si ritira.

---

## 4. Cast

| Personaggio | Sprite | Ruolo nell'Atto 2 | Tic / tono |
|---|---|---|---|
| Protagonista `{PLAYER}` | player | Campione di Hoenn, primo a sbarcare a Kanto dal Varco | muto |
| **Arianna** (`{RIVAL}`, classe Sfidante) | `MAY_NORMAL` (NUOVO dove serve) | Arco qui sotto | "Allora, allora!", "Segno tutto!", conta sulle dita |
| **Prof. Oak** | `PROF_OAK` | Studioso di Kanto. Ha studiato i Varchi da solo, senza sapere di Birch. Ti accoglie a Biancavilla, ti regala uno starter di Kanto, ti racconta di Mew e della Villa, offre ad Arianna un posto da ricercatrice sul campo. Lo si vede nella Sala del Campione | Bonario, professorale; "Ohoh!"; confonde i nomi dei nipoti |
| **Assistente di Oak** | `SCIENTIST` (Aranciopoli #8 @25,7) | Primo contatto al porto | Pignolo, entusiasta |
| **Blu** (Campione di Kanto) | `BLUE` | Nipote di Oak e specchio di Arianna ("il nipote del professore" contro "la figlia del professore"). Spavaldo, ma alla Torre Pokémon ha un lato umano | "Ehi, Hoenn!", ti chiama "il turista"; "Ci vediamo all'Altopiano." |
| **Morgana** | `MAGMA_MEMBER_F` | Antagonista vera | vedi §3 |
| **Giovanni** | `GIOVANNI` | Antagonista visibile | vedi §3 |
| **Mr. Fuji** | `MR_FUJI` | Ha creato Mewtwo e se n'è pentito: è lo specchio di Ettore e Ulisse. Prigioniero dei Rocket alla Torre Pokémon, poi ti dà il **Poké Flauto** | Lento, mite, colpevole |
| **Ettore e Ulisse** | `MAXIE` / `ARCHIE` (NUOVI) | Sono arrivati col secondo viaggio della M/N Acqua e fanno i volontari alla Casa di Mr. Fuji. Alla Grotta Celeste fermano Morgana insieme a te | Ettore: "Fufufu… no."; Ulisse: "Mozzo!", "Ahrrr-ha-ha!" (più sommesso) |
| **Bill** (opz.) | `BILL` | Il suo PC ora "parla" con quello di Lanette: il Box è diventato il primo ponte tra i mondi | Simpatico, smanettone |
| **Rocco** (cameo opz.) | `STEVEN` (NUOVO) | Al Museo di Plumbeopoli 2F studia la Pietralunare | "Interessante…" |
| Capitan Stern (solo testo) | — | Comanda la traversata della M/N Acqua | — |

### 4.1 Arco di Arianna
1. **Traversata/Aranciopoli:** è euforica: *"Primo: siamo i primi. Secondo: siamo a Kanto. Terzo… lo segno!"*
2. **Celestopoli:** Blu la deride ("Un quaderno? Mio nonno ha un laboratorio intero."). Lei si offende e corre da sola al Monte Luna per dimostrargli qualcosa.
3. **Monte Luna:** Morgana le ruba il Taccuino. È umiliata: *"Senza Taccuino non sono niente… sono solo la figlia del professore."*
4. **Silph S.p.A. 7F:** si è infiltrata da sola. Lotta con te, sfogandosi, poi ammette: *"Volevo farcela da sola. Come lui."* Riprende il Taccuino e accetta di fare squadra.
5. **Laboratorio di Oak:** Oak legge il Taccuino e lo trova più utile del Pokédex di Blu (gag). Le offre di restare come ricercatrice.
6. **Grotta Celeste:** con i dati del Taccuino si trova il "battito" della Serratura per spegnerla: *"Primo: vibra a tre battiti. Secondo: il mio Taccuino lo sa. Terzo… lo segno!"*
7. **Finale:** non torna a Hoenn. Resta con Oak, e sarà la prima ad attraversare il prossimo Varco, quello verso ovest (Johto).

---

## 5. Scene (sintesi; meccanica completa in `kanto_scene.json`)

Le scene "opz." sono facoltative. Var di trama: `VAR_KANTO_STORY` (presa da una `VAR_UNUSED_*`). I flag nuovi vanno in coda a `flags_kanto.h`.

| ID | Mappa | Cosa succede (battuta chiave) | Flag → sblocca |
|---|---|---|---|
| K01 | testo in `kanto_travel.inc` | La traversata: luce, silenzio, Stern al timone. Arianna: *"Stiamo… attraversando. Segno tutto. Tutto!"* | `FLAG_KANTO_STORY_CROSSED` |
| K02 | VermilionCity | Sbarco. L'Assistente di Oak (#8) e la **quarantena portuale**: "Una nave uscita dal cielo! Ordini del Capitano Surge: nessuno lascia il porto senza la sua ispezione." Arianna (NUOVO) resta a compilare moduli | VAR 1 |
| K03 | VermilionCity_Gym | **Lt. Surge**. Dopo: "Ispezione superata, soldato! Kanto è tua." | Medaglia Tuono; nasconde i 2 agenti della quarantena (Percorso 6 e 11 aperti) |
| K04 | CeruleanCity | Ponte nord, trigger rivale (22–24,6): **Blu** (#8) scende dal ponte e deride Arianna (NUOVO), che parte da sola per il Monte Luna. Si attiva dopo la Medaglia Cascata | `FLAG_KANTO_SCENE_CERULEAN_BLU` → mostra Morgana/Arianna sul Monte Luna |
| K05 | CeruleanCity_Gym | **Misty** | Medaglia Cascata |
| K06 | MtMoon_B2F | Accanto ai fossili (13–14,7): Morgana (NUOVO) ha preso il Taccuino ad Arianna (NUOVO). Lotta con Morgana. *"…Analisi completata. Il dato è già copiato."* Una Recluta scappa col Taccuino. Poi si sceglie il fossile come in FRLG | `FLAG_KANTO_SCENE_MTMOON` |
| K07 | PewterCity_Gym | **Brock** | Medaglia Sasso |
| K08 opz. | PewterCity_Museum_2F | Rocco (NUOVO) con una Pietralunare: "Viene da un Varco antico, come il nostro Meteorite. Interessante…" Te la regala | `FLAG_KANTO_SCENE_ROCCO` |
| K09 opz. | Route24 + Route25_SeaCottage | La Recluta del Ponte Pepita (#1) recluta "esperti di Varchi" (lotta). Bill (#1): "Il mio PC ha ricevuto una mail da una certa Lanette… da un altro mondo!" Regalo | `FLAG_KANTO_SCENE_BILL` |
| K10 | CeladonCity_Gym | **Erika** | Medaglia Arcobaleno |
| K11 | CeladonCity_GameCorner | Recluta (#11 @11,2) davanti al poster: "Qui non c'è nessun covo. Circolare." Lotta | `FLAG_KANTO_HIDE_GAME_CORNER_ROCKET` → scala del Rifugio |
| K12 | RocketHideout_B4F | **Giovanni** (#1 @19,4): *"Una porta tra i mondi e nessuno che riscuota il pedaggio. Che spreco."* Lotta, poi se ne va. Spettrosonda (ball #2) | `FLAG_KANTO_HIDE_HIDEOUT_GIOVANNI` → Torre Pokémon 6F |
| K13 | PokemonTower_2F | **Blu** (#1, trigger 17,5/16,6) davanti a una tomba, senza lotta: "Qui riposano anche Pokémon che i Varchi hanno portato lontano da casa. … Non dirlo alla tua amica." | `FLAG_KANTO_HIDE_TOWER_RIVAL` |
| K14 | PokemonTower_6F | Spettro (trigger 11,15/12,16): con la Spettrosonda si rivela il Marowak; lotta | `FLAG_KANTO_STORY_TOWER_GHOST` |
| K15 | PokemonTower_7F | Tre Reclute (#2–4) interrogano Mr. Fuji (#1) su Mewtwo. Le batti e Fuji torna a casa | `FLAG_KANTO_STORY_FUJI_RESCUED` (+ HIDE_TOWER_*) |
| K16 | LavenderTown_VolunteerPokemonHouse | Fuji (#1) con **Ettore e Ulisse** (NUOVI). Fuji: *"Mewtwo l'abbiamo creato noi. Dove dorme, il mondo si fa sottile."* Ettore: "Morgana. Era la mia migliore analista." Poké Flauto | `FLAG_KANTO_STORY_GOT_FLUTE` → Snorlax |
| K17 | Route12 / Route16 | Snorlax (#5 @14,70; #10 @31,13). Si svegliano col Poké Flauto: lotta selvatica | `FLAG_KANTO_HIDE_ROUTE_12_SNORLAX` / `_16_` |
| K18 | FuchsiaCity_Gym | **Koga**. Dopo: "La Silph è caduta. Ho fatto aprire i cancelli di Zafferanopoli." | Medaglia Anima; `FLAG_KANTO_STORY_SAFFRON_OPEN` |
| K19 | SilphCo_7F | Il vecchio oggetto Blu (#1 @2,6, gfx → `MAY_NORMAL`) diventa Arianna, con i trigger (2,4)/(2,5). Lotta da Sfidante. *"Volevo farcela da sola. Come lui."* Riprende il Taccuino | `FLAG_KANTO_HIDE_SILPH_RIVAL` |
| K20 | SilphCo_11F | Presidente (#1), Segretaria (#2), **Giovanni** (#3, trigger 5–6,15). Dopo la lotta Giovanni scopre che Morgana è sparita col prototipo: "Mi ha venduto la mia stessa merce." Il Presidente ti dà la Master Ball | `FLAG_KANTO_STORY_SILPH_FREED`; nasconde i Rocket di Silph e di Zafferanopoli (Palestra libera) |
| K21 | SaffronCity_Gym | **Sabrina** | Medaglia Palude |
| K22 | PalletTown_ProfessorOaksLab | Oak (#4) e Arianna (NUOVO). Lore di Mew e della Villa; Oak sfoglia il Taccuino ("Ohoh! Meglio del Pokédex di mio nipote!"). Starter di Kanto a scelta (ball #5–7). Offre il posto ad Arianna | `FLAG_KANTO_STORY_OAK_MET` |
| K23 | PokemonMansion_1F…B1F | I diari della Villa ("Pokémon Journal" → cartelli) raccontano il clone e il "Varco vivente". Chiave Segreta (ball B1F #6) | `FLAG_KANTO_HIDE_POKEMON_MANSION_B1F_SECRET_KEY` → porta della Palestra |
| K24 | CinnabarIsland_Gym | **Blaine** confessa di aver aiutato Fuji | Medaglia Vulcano |
| K25 | ViridianCity_Gym | **Giovanni** da Capopalestra. Dopo: "Il Team Rocket è sciolto. Di nuovo. Morgana è alla Grotta Celeste. Non è più affar mio." | Medaglia Terra; nasconde la guardia della Grotta Celeste |
| K26 | CeruleanCave_1F | **Climax.** Morgana (NUOVO) collega la Serratura alla Grotta. Arrivano Ettore e Ulisse (NUOVI) e Arianna (NUOVO). Lotta con Morgana (Admin Alpha). Arianna trova il battito e la Serratura si rompe; un'ondata psichica (flash): Mewtwo si sveglia, guarda e torna nel buio. Ettore porta via Morgana: "Il muro era un errore. Mio. Vieni." | `FLAG_KANTO_STORY_RIFT_CALMED` → Lega; Mewtwo in B1F |
| K27 | Route22_NorthEntrance, Route23 | Le guardie controllano le **Medaglie di Kanto** | — |
| K28 | PokemonLeague_* | Superquattro e Campione **Blu**; Oak entra (#2) | `FLAG_KANTO_STORY_CHAMPION` |
| K29 | IndigoPlateau_Exterior | Titoli: Blu (#1) e Oak (#2) con flag dei titoli; Arianna (NUOVO). Nel cielo a ovest il Grande Varco si apre in **un secondo corridoio**, e una figura rosa ci passa davanti (Mew). Oak: *"Oltre quelle montagne… un mondo chiamato Johto."* Arianna: *"Stavolta passo prima io. Segnato!"* Testo finale: **"Fine dell'Atto 2. Il multiverso vi aspetta."** | — |
| K30 opz. | PowerPlant / SeafoamIslands_B4F | Zapdos (#6) / Articuno (#3), agitati dal Varco. Cartello alle Spumarine: "Il terzo Vento vola su un'isola che nessuna mappa ricorda." | flag HIDE esistenti |
| K31 opz. | CeruleanCave_B1F | Mewtwo (#3), si può catturare dopo K26 (consigliato dopo la Lega) | `FLAG_KANTO_HIDE_MEWTWO` |

---

## 6. Gating

**Regola:** il giocatore ha già tutte le MN di Hoenn (Taglio, Surf, Forza, Spaccaroccia…), quindi alberi e massi non bloccano nulla. A tenere l'ordine sono solo i blocchi di trama qui sotto. Le MN FRLG in Emerald richiedono le Medaglie di Hoenn, già ottenute.

| Blocco | Mappa / oggetto o trigger | Si toglie con |
|---|---|---|
| Quarantena nord | VermilionCity, NUOVO `POLICEMAN` sull'uscita del Percorso 6 | `FLAG_KANTO_BADGE03` (oggetto con flag `FLAG_KANTO_HIDE_VERMILION_QUARANTINE`, impostato dopo Surge) |
| Quarantena est | VermilionCity, NUOVO `POLICEMAN` sull'uscita del Percorso 11 | idem |
| Monte Luna (scena) | MtMoon_B2F, Morgana e Arianna NUOVE | compaiono con `FLAG_KANTO_SCENE_CERULEAN_BLU` |
| Scala del Rifugio | CeladonCity_GameCorner #11 | `FLAG_KANTO_HIDE_GAME_CORNER_ROCKET` (dopo la lotta) |
| Ascensore del Rifugio | RocketHideout_Elevator, Chiave Ascensore (ball B4F #4) | va riscritta la logica dell'ascensore (README) |
| Spettro della Torre | PokemonTower_6F, trigger (11,15)(12,16) | `FLAG_KANTO_HIDE_HIDEOUT_GIOVANNI` (Spettrosonda) |
| Snorlax 12 / 16 | Route12 #5, Route16 #10 | `FLAG_KANTO_STORY_GOT_FLUTE` → lotta → `FLAG_KANTO_HIDE_ROUTE_1x_SNORLAX` |
| Cancelli di Zafferanopoli (4) | Route5_SouthEntrance, Route6_NorthEntrance, Route7_EastEntrance, Route8_WestEntrance: trigger "Guard" | `FLAG_KANTO_STORY_SAFFRON_OPEN` (dopo Koga) |
| Porte della Silph | SilphCo_*, Apriporta (5F ball #8) | come FRLG (logica `ON_LOAD` da reinserire) |
| Palestra di Zafferanopoli | SaffronCity #3 ROCKET_M @46,13 (davanti alla porta 46,12) | `FLAG_KANTO_HIDE_SAFFRON_ROCKETS` (dopo K20) |
| Sabrina | SaffronCity_Gym #7 | niente lotta senza `FLAG_KANTO_STORY_SILPH_FREED` (doppia sicurezza) |
| Palestra di Isola Cannella | CinnabarIsland, trigger (20,5) GymDoorLocked | Chiave Segreta (`FLAG_KANTO_HIDE_POKEMON_MANSION_B1F_SECRET_KEY`) |
| Palestra di Smeraldopoli | ViridianCity, trigger (36,11) GymDoorLocked | 7 Medaglie di Kanto + `FLAG_KANTO_STORY_OAK_MET` |
| Grotta Celeste | CeruleanCity #12 COOLTRAINER_M @1,13 | `FLAG_KANTO_BADGE08` → `FLAG_KANTO_HIDE_CERULEAN_CAVE_GUARD` |
| Strada per la Lega | Route22_NorthEntrance, trigger (7,2) | `FLAG_KANTO_STORY_RIFT_CALMED` |
| Guardie del Percorso 23 | Route23 #1–7 e i loro trigger | `FLAG_KANTO_BADGE02…08`, uno per guardia |
| Pista Ciclabile | Route16/18 "NeedBike" | controllare `ITEM_MACH_BIKE`/`ITEM_ACRO_BIKE` |

**Da verificare in Porymap:** che il Surf non aggiri gli Snorlax o la quarantena (acque di Aranciopoli e del Percorso 12). Se le aggira, servono altri agenti o Snorlax. La Grotta Diglett (dal Percorso 11 al Percorso 2) resta aperta, quindi Brock e Misty si possono fare in qualsiasi ordine.

**Medaglie di Kanto:** sono flag nuovi, `FLAG_KANTO_BADGE01` (Sasso) … `08` (Terra), in ordine canonico. La Tessera Allenatore per ora non le mostra (da progettare).

---

## 7. Curva dei livelli

| Fase | Selvatici | Allenatori | Boss (asso) |
|---|---|---|---|
| Aranciopoli, Percorsi 6 e 11 | 50–53 | 52–54 | Surge **55** |
| Celestopoli, Percorsi 4/24/25, Monte Luna | 52–55 | 54–56 | Misty **57**, Morgana 57 |
| Plumbeopoli, Grotta Diglett, Percorsi 2/3 | 53–56 | 55–57 | Brock **58** |
| Azzurropoli, Lavandonia, Tunnel Roccioso | 55–58 | 56–59 | Erika **59**, Giovanni (Rifugio) 60, Marowak 60 |
| Percorsi 12–18, Fucsiapoli, Snorlax | 57–60 | 58–61 | Snorlax 60, Koga **61** |
| Zafferanopoli, Silph | — | 60–62 | Arianna 62, Giovanni (Silph) 62, Sabrina **63** |
| Percorsi 19–21, Isola Cannella, Villa, Spumarine | 59–62 | 61–63 | Blaine **64** |
| Smeraldopoli, Grotta Celeste | 62–66 | 63–65 | Giovanni **66**, Morgana 66 |
| Via Vittoria | 63–66 | 64–67 | — |
| Lega | — | — | Lorelei **67**, Bruno **68**, Agatha **69**, Lance **71**, Blu **72** |
| Leggendari | — | — | Zapdos 65, Articuno 65, Mewtwo 70 |

Le tabelle dei selvatici sono quelle vanilla di FireRed (Lv 2–50) e vanno ricalibrate per fasce. Gli allenatori nuovi vanno in `trainers.party`, prendendo le squadre di riferimento da `trainers_frlg.party`.

---

## 8. Capipalestra (ordine consigliato)

| # | Capopalestra | Tipo / Medaglia | Personalità | Battuta chiave |
|---|---|---|---|---|
| 1 | **Lt. Surge** (Aranciopoli) | Elettro / Medaglia Tuono | Ex militare, comandante del porto, chiassoso, ti chiama "soldato" | *"Una nave uscita da un buco nel cielo? Nel MIO porto? Prima di sbarcare passi l'ispezione, soldato!"* |
| 2 | **Misty** (Celestopoli) | Acqua / Medaglia Cascata | Diretta, permalosa, competitiva | *"Campione di un altro mondo? E allora? Qui l'acqua è mia!"* |
| 3 | **Brock** (Plumbeopoli) | Roccia / Medaglia Sasso | Serio, fraterno, solido | *"Le rocce del Monte Luna sono cadute dal cielo. Io sono rimasto coi piedi per terra."* |
| 4 | **Erika** (Azzurropoli) | Erba / Medaglia Arcobaleno | Gentile, sonnolenta, profumiera | *"I fiori si chiudono quando passa un Varco… e anche quando passa il Team Rocket."* |
| 5 | **Koga** (Fucsiapoli) | Veleno / Medaglia Anima | Ninja, laconico, ti osserva da tempo | *"Un ninja osserva prima di colpire. Ti osservo da Lavandonia."* |
| 6 | **Sabrina** (Zafferanopoli) | Psico / Medaglia Palude | Glaciale, veggente | *"Ho visto una porta di luce a nord di Celestopoli. E davanti c'eri tu."* |
| 7 | **Blaine** (Isola Cannella) | Fuoco / Medaglia Vulcano | Vecchio scienziato, indovinelli, rimorsi | *"Indovinello: cosa si crea in laboratorio e non si può più distruggere? … Un errore."* |
| 8 | **Giovanni** (Smeraldopoli) | Terra / Medaglia Terra | Freddo, elegante | *"Una porta tra i mondi è un affare. Peccato che qualcuno l'abbia rubata a me."* |

## 9. Superquattro e Campione

- **Lorelei** (Ghiaccio): *"Avevo un'allieva che sparì in una tempesta di neve, anni fa. A Hoenn c'è una Superquattro di ghiaccio, dici? … Si chiama Frida?"* (Questo è l'aggancio al mistero di Frida dell'Atto 1. Non chiuderlo del tutto.)
- **Bruno** (Lotta): *"Muscoli e cuore! Nessun Varco spezza il legame tra un Allenatore e i suoi Pokémon!"*
- **Agatha** (Spettro): *"Le anime della Torre parlano di un ragazzo arrivato dal mare di luce. Fammi vedere se avevano ragione."*
- **Lance** (Drago): *"I Draghi sentono il cielo strapparsi prima di chiunque altro. Volevo conoscere chi l'ha attraversato per primo."*
- **Blu** (Campione): *"Il nonno dice che sei il primo arrivato dall'altra parte del Varco. Bene: allora sarai il primo a perdere qui!"* Dopo la sconfitta: *"…Ok. Il primo in tutto. Scrivilo pure sul quaderno della tua amica."* Oak entra: *"Blu, hai perso perché hai dimenticato una cosa: il rispetto. Per i Pokémon… e per chi viene da lontano."*

---

## 10. Classi di allenatore solo di Kanto

I nomi italiani esistono già in `src/battle_main.c` (`*_FRLG`) e vanno usati così: Secchione, Centauro, Ladro, Ingegnere, Teppista, Giocatore, Rocker, Giocoliere, Domatore, Scienziato, Medium, Coppia Top, Duo Lotta, Lottatrice, Pittrice, Team Rocket, Rivale, Prof. Uniche modifiche: `TRAINER_CLASS_BOSS_FRLG` "Capo" → **"Capo Rocket"** (11 caratteri, come Capo Alpha/Omega). Morgana usa "Admin Alpha", Arianna "Sfidante", Blu "Campione".

**Oggetti** (nomi del build): Poké Flauto, Spettrosonda, Apriporta, Chiave Segreta, Chiave Ascensore, Pietralunare, Master Ball, Biglietto Nave. Il Taccuino dei Varchi è solo un flag, non un oggetto.

---

## 11. Stile

Vale integralmente §7 di `bibbia.md`: Title Case, segnaposto intoccabili, circa 36 caratteri per riga, `…`, tono avventuroso e mai cinico, i Varchi come meraviglia prima che paura. In più, per Kanto:
- Gli abitanti di Kanto non conoscono Hoenn. Reagiscono con curiosità ("Vieni davvero dall'altra parte del Varco?"). Altri mondi (Johto, Sinnoh…) solo in modo vago.
- Reclute Rocket: gergo da ditta ("Il carico!", "Il Capo vuole i numeri!"), mai crudeltà esplicita sui Pokémon.
- Nomi di luogo ufficiali di FRLG: Biancavilla, Smeraldopoli, Plumbeopoli, Celestopoli, Aranciopoli, Lavandonia, Azzurropoli, Fucsiapoli, Zafferanopoli, Isola Cannella, Altopiano Blu, Percorso 1–25, Bosco Smeraldo, Monte Luna, Tunnel Roccioso, Torre Pokémon, Zona Safari, Isole Spumarine, Grotta Celeste, Villa Pokémon, Via Vittoria, Grotta Diglett, Via Sotterranea, M/N Anna. Nei dialoghi si scrive **Silph S.p.A.**, **Centrale Elettrica** e **Rifugio Rocket**. Nel popup MAPSEC restano "Silph SpA" e "Centrale Elett.", che sono abbreviati per la lunghezza.

## 12. NPC generici per città

| Luogo | Tema delle battute |
|---|---|
| Aranciopoli | Marinai e portuali stupiti dalla nave "uscita dal cielo", la quarantena, i veterani di Surge, il Fan Club Pokémon ("Hai Pokémon di un altro mondo? Fammeli vedere!") |
| Celestopoli | Il furto nella casa (Rocket), le voci sulla grotta "dove nessuno torna", il Ponte Pepita |
| Plumbeopoli | Museo, Pietralunare cadute dal cielo, orgoglio per Brock |
| Smeraldopoli | La Palestra chiusa da mesi ("il Capopalestra non c'è mai…"), un vecchio che ricorda come si cattura un Pokémon |
| Biancavilla | Paesino quieto, la gente parla di Oak e di Blu ("è diventato Campione, ma per me resta un monello") |
| Lavandonia | Lutto e calma, la Torre "piena di voci nuove", Mr. Fuji scomparso |
| Azzurropoli | Città ricca, Centro Commerciale, Casinò "frequentato da gente in nero", profumi di Erika |
| Fucsiapoli | Zona Safari ("dicono che dal Varco siano arrivati Pokémon mai visti"), ninja |
| Zafferanopoli | Prima: chiusa, dalle gate si sentono solo voci. Dopo K20: sollievo, la Silph ringrazia, il Dojo Karate |
| Isola Cannella | Il vulcano, la Villa bruciata "dove facevano esperimenti strani", il Laboratorio dei fossili |
| Altopiano Blu | Allenatori d'élite, rispetto per il Campione di un altro mondo |
| Percorsi | Come l'Atto 1, ma ogni tanto un allenatore parla di un Pokémon "arrivato dal Varco di Hoenn" |
