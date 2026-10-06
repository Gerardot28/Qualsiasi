# Regioni ufficiali: luoghi, Capipalestra, Superquattro, Campioni (nomi italiani)

Documento di riferimento per **Pokémon Multiverse** (hack multi-regione su pokeemerald-expansion 1.17.1).
Generato da `/home/user/work/pokeapi/scripts/render_regioni.py` a partire da:

- `/home/user/work/pokeapi_it/locations.json`: nomi dei luoghi. Fonte primaria PokeAPI (`location_names.csv`, lingua 8 = italiano), che però ha l'italiano solo per Hoenn, Kalos e Alola. Il resto viene dalle tabelle dei luoghi di PKHeX (`PKHeX.Core/Resources/text/locations/*_it.txt`, cioè le stringhe estratte dai giochi) e dal corpus di testi ufficiali dei giochi.
- `/home/user/work/pokeapi_it/regioni_personaggi.json`: nomi dei personaggi verificati da `scripts/regioni.py` su tre fonti indipendenti:
  1. **giochi**: corpus dei testi ufficiali EN/IT allineati riga per riga, [abcboy101/poke-corpus](https://github.com/abcboy101/poke-corpus) (dump dei messaggi di RFVF, Smeraldo, HGSS, Pt, BDSP, NB/N2B2, XY, SL/USUL, SpSc, ScVi…). «giochi:X (n/m)» vuol dire che nelle righe in cui l'inglese è esattamente quel nome l'italiano è X per n righe su m.
  2. **GCC**: nomi delle carte Allenatore del GCC italiano ufficiale, da [tcgdex/cards-database](https://github.com/tcgdex/cards-database).
  3. **PokeRogue**: `it/trainer-names.json` di [pagefaultgames/pokerogue-locales](https://github.com/pagefaultgames/pokerogue-locales). È una traduzione della community, quindi conta meno: in 7 casi è sbagliata, vedi la sezione «Discrepanze».
- Le medaglie sono verificate allo stesso modo: si cerca il «Medaglia X» più frequente nelle righe italiane la cui riga inglese contiene «<Nome> Badge».

**Legenda dei luoghi**: nessun simbolo = PokeAPI; ¹ = PKHeX (stringhe dei giochi); ² = corpus dei testi di gioco; ³ = regola (es. «Sea Route N» → «Percorso N»); † = fornito da me (da verificare).
**Confidenza dei nomi**: *alta* = confermato dai testi ufficiali dei giochi o da GCC e PokeRogue concordi; *media* = una sola fonte, oppure ambiguità (omonimi o nome scelto dal giocatore); *bassa* = nessuna fonte trovata, nome dato a memoria.

I nomi delle specie Pokémon in italiano coincidono con quelli inglesi, tranne Tipo Zero (Type: Null) e i Pokémon Paradosso (es. Grandizanne = Great Tusk, Eroeferreo = Iron Valiant). Elenco completo in `species_by_national_dex.json`. Le categorie sono «Pokémon Seme», «Pokémon Topo» e così via.

Termini ricorrenti verificati nei testi di gioco: Capopalestra (Gym Leader), Superquattro (Elite Four), Campione/Campionessa (Champion), Lega Pokémon (Pokémon League), Medaglia (Badge), Kahuna, Capitano/Capitana (Trial Captain), giro delle isole (island challenge), GRANDE PROVA (grand trial), mossa Z, Ultracreature (Ultra Beasts), Megaevoluzione, Dynamax/Raid Dynamax, Terre Selvagge (Wild Area), Sfida delle Palestre (Gym Challenge), Raid Teracristal, Pokémon Paradosso, «tesori portatori di sciagura» (Treasures of Ruin), «beniamici» (Loyal Three).

---
## Kanto (Gen 1, specie #001–151)

**Città**: Biancavilla¹ (Pallet Town), Smeraldopoli¹ (Viridian City), Plumbeopoli¹ (Pewter City), Celestopoli¹ (Cerulean City), Aranciopoli¹ (Vermilion City), Lavandonia¹ (Lavender Town), Azzurropoli¹ (Celadon City), Fucsiapoli¹ (Fuchsia City), Zafferanopoli¹ (Saffron City), Isola Cannella¹ (Cinnabar Island), Altopiano Blu¹ (Indigo Plateau)

**Percorsi**: Percorso 1–25 (Route 1–25); i percorsi 19, 20 e 21 sono marini. Isole Settipelago (RFVF): Primisola¹ (One Island), Secondisola¹ (Two Island), Terzisola¹ (Three Island), Quartisola¹ (Four Island), Quintisola¹ (Five Island), Sestisola¹ (Six Island), Settimisola¹ (Seven Island).

**Luoghi chiave**: Bosco Smeraldo¹ (Viridian Forest), Monte Luna¹ (Mt. Moon), Grotta Diglett¹ (Diglett's Cave), Tunnel Roccioso¹ (Rock Tunnel), Torre Pokémon¹ (Pokémon Tower), Centrale Elett.¹ (Kanto Power Plant), M/N Anna¹ (S.S. Anne), Isole Spumarine¹ (Seafoam Islands), Villa Pokémon¹ (Pokémon Mansion), Zona Safari¹ (Safari Zone), Via Vittoria¹ (Victory Road), Grotta Celeste¹ (Cerulean Cave), Monte Brace¹ (Mt. Ember), Monte Cordone¹ (Navel Rock), Isola Materna¹ (Birth Island), Silph SpA¹ (Silph Co.), Rifugio Rocket¹ (Rocket Hideout).

### Capipalestra (RBG/RFVF/LGPE)
| # | Capopalestra (IT) | EN | Tipo | Città | Medaglia (IT) | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Brock** | Brock | Roccia | Plumbeopoli¹ | Medaglia Sasso | Boulder Badge | alta / alta | giochi:Brock (13/13); GCC:Brock; PokeRogue:Brock |
| 2 | **Misty** | Misty | Acqua | Celestopoli¹ | Medaglia Cascata | Cascade Badge | alta / alta | giochi:Misty (12/12); GCC:Misty; PokeRogue:Misty |
| 3 | **Lt. Surge** | Lt. Surge | Elettro | Aranciopoli¹ | Medaglia Tuono | Thunder Badge | alta / alta | giochi:Lt. Surge (5/7); GCC:Lt. Surge |
| 4 | **Erika** | Erika | Erba | Azzurropoli¹ | Medaglia Arcobaleno | Rainbow Badge | alta / alta | giochi:Erika (13/13); GCC:Erika; PokeRogue:Erika |
| 5 | **Koga** | Koga | Veleno | Fucsiapoli¹ | Medaglia Anima | Soul Badge | alta / alta | giochi:Koga (11/11); GCC:Koga; PokeRogue:Koga |
| 6 | **Sabrina** | Sabrina | Psico | Zafferanopoli¹ | Medaglia Palude | Marsh Badge | alta / alta | giochi:Sabrina (13/13); GCC:Sabrina; PokeRogue:Sabrina |
| 7 | **Blaine** | Blaine | Fuoco | Isola Cannella¹ | Medaglia Vulcano | Volcano Badge | alta / alta | giochi:Blaine (13/13); GCC:Blaine; PokeRogue:Blaine |
| 8 | **Giovanni** | Giovanni | Terra | Smeraldopoli¹ | Medaglia Terra | Earth Badge | alta / alta | giochi:Giovanni (8/8); GCC:Giovanni; PokeRogue:Giovanni |

In HGSS la Palestra di Fucsiapoli passa a **Nina** (Janine, Veleno: conf. alta; GCC «Nina», PokeRogue «Nina»), a Smeraldopoli c'è **Blu** (Blue) e a Isola Cannella la Palestra si sposta nelle Isole Spumarine.

### Superquattro e Campione
| Nome (IT) | EN | Tipo | Conf. | Evidenze |
|---|---|---|---|---|
| **Lorelei** | Lorelei | Ghiaccio | alta | giochi:Lorelei (9/9); PokeRogue:Lorelei |
| **Bruno** | Bruno | Lotta | alta | giochi:Bruno (11/11); GCC:Bruno; PokeRogue:Bruno |
| **Agatha** | Agatha | Spettro | alta | giochi:Agatha (10/10); GCC:Agatha; PokeRogue:Agatha |
| **Lance** | Lance | Drago | alta | giochi:Lance (14/14); GCC:Lance; PokeRogue:Lance |
| **Blu** | Blue | — | alta | giochi:Blu (9/11); GCC:Blu; PokeRogue:Blu |

Il Campione è il rivale **Blu**. In RFVF il suo nome predefinito si sceglie all'inizio del gioco, ma il nome canonico italiano è «Blu».

- **Professore**: Professor Oak (conf. alta: «Oak» nei testi di gioco, GCC «Professore Oak»)
- **Squadra malvagia**: Team Rocket, capo **Giovanni** (conf. alta)
- **Rivale**: Blu (Blue). Il protagonista canonico è **Rosso** (Red: GCC «Rosso», PokeRogue «Rosso»)
- **Leggendari e mitici**: Articuno, Zapdos, Moltres, Mewtwo; Mew (mitico)
- **Starter**: Bulbasaur, Charmander, Squirtle (Pikachu/Eevee in LGPE)

---
## Johto (Gen 2, specie #152–251)

**Città**: Borgo Foglianova¹ (New Bark Town), Fiorpescopoli¹ (Cherrygrove City), Violapoli¹ (Violet City), Azalina¹ (Azalea Town), Fiordoropoli¹ (Goldenrod City), Amarantopoli¹ (Ecruteak City), Olivinopoli¹ (Olivine City), Fiorlisopoli¹ (Cianwood City), Mogania¹ (Mahogany Town), Ebanopoli¹ (Blackthorn City)

**Percorsi**: Percorso 29–46 (i percorsi 26–28, tra Johto e Kanto, portano alla Lega). Percorsi 47–48 solo in HGSS.

**Luoghi chiave**: Torre Sprout¹ (Sprout Tower), Rovine d’Alfa¹ (Ruins of Alph), Grotta di mezzo¹ (Union Cave), Pozzo SLOWPOKE¹ (Slowpoke Well), Bosco di Lecci¹ (Ilex Forest), Parco Nazionale¹ (National Park), Torre Bruciata¹ (Burned Tower), Torre Campana¹ (Bell Tower), Faro¹ (Lighthouse), Isole Vorticose¹ (Whirl Islands), Monte Scodella¹ (Mt. Mortar), Lago d’Ira¹ (Lake of Rage), Covo Team Rocket¹ (Team Rocket HQ), Torre Radio¹ (Radio Tower), Via Gelata¹ (Ice Path), Tana del Drago¹ (Dragon's Den), Grotta Scura¹ (Dark Cave), Cascate Tohjo¹ (Tohjo Falls), Monte Argento¹ (Mt. Silver), Zona Safari¹ (Johto Safari Zone), Rovine Sinjoh¹ (Sinjoh Ruins). Lega Pokémon all'Altopiano Blu (Kanto).

### Capipalestra (OAC/HGSS)
| # | Capopalestra (IT) | EN | Tipo | Città | Medaglia (IT) | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Valerio** | Falkner | Volante | Violapoli¹ | Medaglia Zefiro | Zephyr Badge | alta / media | giochi:Valerio (5/5); GCC:Valerio; PokeRogue:Valerio |
| 2 | **Raffaello** | Bugsy | Coleottero | Azalina¹ | Medaglia Alveare | Hive Badge | alta / alta | giochi:Raffaello (4/4); PokeRogue:Raffaello |
| 3 | **Chiara** | Whitney | Normale | Fiordoropoli¹ | Medaglia Piana | Plain Badge | alta / alta | giochi:Chiara (5/5); GCC:Chiara; PokeRogue:Chiara |
| 4 | **Angelo** | Morty | Spettro | Amarantopoli¹ | Medaglia Nebbia | Fog Badge | alta / alta | giochi:Angelo (4/4); GCC:Angelo; PokeRogue:Angelo |
| 5 | **Furio** | Chuck | Lotta | Fiorlisopoli¹ | Medaglia Tempesta | Storm Badge | alta / alta | giochi:Furio (4/4); PokeRogue:Furio |
| 6 | **Jasmine** | Jasmine | Acciaio | Olivinopoli¹ | Medaglia Minerale | Mineral Badge | alta / media | giochi:Jasmine (6/6); GCC:Jasmine; PokeRogue:Jasmine |
| 7 | **Alfredo** | Pryce | Ghiaccio | Mogania¹ | Medaglia Gelo | Glacier Badge | alta / media | giochi:Alfredo (4/4); PokeRogue:Alfredo |
| 8 | **Sandra** | Clair | Drago | Ebanopoli¹ | Medaglia Levante | Rising Badge | alta / alta | giochi:Sandra (4/4); PokeRogue:Sandra |

### Superquattro e Campione
| Nome (IT) | EN | Tipo | Conf. | Evidenze |
|---|---|---|---|---|
| **Pino** | Will | Psico | alta | giochi:Pino (2/2); GCC:Pino; PokeRogue:Pino |
| **Koga** | Koga | Veleno | alta | giochi:Koga (3/3); GCC:Koga; PokeRogue:Koga |
| **Bruno** | Bruno | Lotta | alta | giochi:Bruno (2/2); GCC:Bruno; PokeRogue:Bruno |
| **Karen** | Karen | Buio | alta | giochi:Karen (2/2); PokeRogue:Karen |
| **Lance** | Lance | Drago | alta | giochi:Lance (5/5); GCC:Lance; PokeRogue:Lance |

- **Professore**: Professor Elm (conf. alta: «Elm» in 106 righe su 123 di HGSS)
- **Squadra malvagia**: Team Rocket. Nel corpus di HGSS il dirigente Archer resta «Archer» (conf. media); in GCC Petrel = «Maxus», Proton = «Milas», Ariana = «Atena»
- **Rivale**: Silver. In HGSS il nome lo sceglie il giocatore; in italiano resta «Silver» (conf. media, perché la ricerca esatta trova anche «Argento», che è l'oggetto o il colore). Protagonisti: **Armonio** (Ethan, GCC «Armonio») e Lyra
- **Leggendari e mitici**: Raikou, Entei, Suicune (cani leggendari), Lugia, Ho-Oh; Celebi (mitico)
- **Starter**: Chikorita, Cyndaquil, Totodile

---
## Hoenn (Gen 3, specie #252–386). È la base di pokeemerald

**Città**: Albanova (Littleroot Town), Solarosa (Oldale Town), Petalipoli (Petalburg City), Ferrugipoli (Rustboro City), Bluruvia (Dewford Town), Porto Selcepoli (Slateport City), Ciclamipoli (Mauville City), Mentania (Verdanturf Town), Brunifoglia (Fallarbor Town), Cuordilava (Lavaridge Town), Forestopoli (Fortree City), Porto Alghepoli (Lilycove City), Verdeazzupoli (Mossdeep City), Ceneride (Sootopolis City), Orocea (Pacifidlog Town), Iridopoli (Ever Grande City)

**Percorsi**: Percorso 101–134 (dal 105 al 134 soprattutto marini).

**Luoghi chiave**: Bosco Petalo (Petalburg Woods), Tunnel Menferro (Rusturf Tunnel), Grotta Pietrosa (Granite Cave), Ciclanova (New Mauville), Cammino Ardente (Fiery Path), Monte Camino (Mt. Chimney), Passo Selvaggio (Jagged Pass), Cascate Meteora (Meteor Falls), Monte Pira (Mt. Pyre), Rifugio Magma (Team Magma Hideout), Rifugio Idro (Team Aqua Hideout), Vecchia Nave¹ (Abandoned Ship), Antro Abissale (Seafloor Cavern), Grotta dei Tempi (Cave of Origin), Torre dei Cieli (Sky Pillar), Grotta Ondosa (Shoal Cave), Zona Safari (Safari Zone), Torre Miraggio¹ (Mirage Tower), Rovine Sabbiose (Desert Ruins), Grotta Insulare (Island Cave), Tomba Antica (Ancient Tomb), Sala Incisa (Sealed Chamber), Via Vittoria (Victory Road), Centro Spaziale di Verdeazzupoli² (Mossdeep Space Center), Isola Remota (Southern Island), Torre Lotta² (Battle Tower). Nella mappa di pokeemerald gli stessi nomi, in maiuscolo come nello Smeraldo italiano, stanno in `mapsec_hoenn_kanto.json` (campo `it_ingame`, preso dal testo ufficiale di Smeraldo/RFVF).

### Capipalestra (Smeraldo; in RZ/ROZA l'ottavo è Adriano)
| # | Capopalestra (IT) | EN | Tipo | Città | Medaglia (IT) | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Petra** | Roxanne | Roccia | Ferrugipoli | Medaglia Pietra | Stone Badge | alta / alta | giochi:Petra (8/8); GCC:Petra; PokeRogue:Petra |
| 2 | **Rudi** | Brawly | Lotta | Bluruvia | Medaglia Pugno | Knuckle Badge | alta / alta | giochi:Rudi (10/10); GCC:Rudi; PokeRogue:Rudi |
| 3 | **Walter** | Wattson | Elettro | Ciclamipoli | Medaglia Dinamo | Dynamo Badge | alta / alta | giochi:Walter (8/8); PokeRogue:Walter |
| 4 | **Fiammetta** | Flannery | Fuoco | Cuordilava | Medaglia Fiamma | Heat Badge | alta / alta | giochi:Fiammetta (8/8); GCC:Fiammetta; PokeRogue:Fiammetta |
| 5 | **Norman** | Norman | Normale | Petalipoli | Medaglia Armonia | Balance Badge | alta / alta | giochi:Norman (8/8); GCC:Norman; PokeRogue:Norman |
| 6 | **Alice** | Winona | Volante | Forestopoli | Medaglia Piuma | Feather Badge | alta / alta | giochi:Alice (10/10); GCC:Alice; PokeRogue:Alice |
| 7 | **Tell e Pat** | Tate & Liza | Psico | Verdeazzupoli | Medaglia Mente | Mind Badge | alta / alta | GCC «Tell e Pat»; PokeRogue «Tell», «Pat»; giochi: «Liza»→«Pat» in 23 righe su 28 (Smeraldo) |
| 8 | **Rodolfo** | Juan | Acqua | Ceneride | Medaglia Pioggia | Rain Badge | alta / alta | giochi:Rodolfo (5/5); PokeRogue:Rodolfo |

### Superquattro e Campione
| Nome (IT) | EN | Tipo | Conf. | Evidenze |
|---|---|---|---|---|
| **Fosco** | Sidney | Buio | alta | giochi:Fosco (5/5); GCC:Fosco; PokeRogue:Fosco |
| **Ester** | Phoebe | Spettro | alta | giochi:Ester (7/7); GCC:Ester; PokeRogue:Ester |
| **Frida** | Glacia | Ghiaccio | alta | giochi:Frida (7/7); PokeRogue:Frida |
| **Drake** | Drake | Drago | alta | giochi:Drake (5/5); PokeRogue:Drake |
| **Adriano** | Wallace | Acqua | alta | giochi:Adriano (8/8); GCC:Adriano; PokeRogue:Adriano |
| **Rocco** | Steven | Acciaio | alta | giochi:Rocco (13/13); GCC:Rocco; PokeRogue:Rocco |

In Smeraldo il Campione è **Adriano** (Wallace), in RZ e ROZA è **Rocco** (Steven). In Smeraldo Rocco è uno sfidante opzionale del post-game, nelle Cascate Meteora.

- **Professore**: Professor Birch (conf. alta: «Birch» in 52 righe su 52 di Smeraldo)
- **Squadre malvagie**: **Team Magma** (capo **Max**, Maxie) e **Team Idro** (capo **Ivan**, Archie). Tutti conf. alta. Amministratori secondo PokeRogue (conf. media): Ottavio (Tabitha), Rossella (Courtney), Ada (Shelly), Alan (Matt)
- **Rivali**: Brendon (Brendan) e Vera (May), entrambi conf. alta; Lino (Wally), conf. alta
- **Leggendari e mitici**: Regirock, Regice, Registeel, Latias, Latios, Kyogre, Groudon, Rayquaza; Jirachi e Deoxys (mitici)
- **Starter**: Treecko, Torchic, Mudkip

---
## Sinnoh (Gen 4, specie #387–493)

**Città**: Duefoglie¹ (Twinleaf Town), Sabbiafine¹ (Sandgem Town), Giubilopoli¹ (Jubilife City), Mineropoli¹ (Oreburgh City), Giardinfiorito¹ (Floaroma Town), Evopoli¹ (Eterna City), Cuoripoli¹ (Hearthome City), Flemminia¹ (Solaceon Town), Rupepoli¹ (Veilstone City), Pratopoli¹ (Pastoria City), Memoride¹ (Celestic Town), Canalipoli¹ (Canalave City), Nevepoli¹ (Snowpoint City), Arenipoli¹ (Sunyshore City), Area Svago¹ (Resort Area), Area Sfida¹ (Fight Area), Area Provviste¹ (Survival Area)

**Percorsi**: Percorso 201–230 (220, 223, 226 e 230 sono marini).

**Luoghi chiave**: Lago Verità¹ (Lake Verity), Lago Valore¹ (Lake Valor), Lago Arguzia¹ (Lake Acuity), Cava di Mineropoli¹ (Oreburgh Mine), Impianto Turbine¹ (Valley Windworks), Bosco di Evopoli¹ (Eterna Forest), Antico Château¹ (Old Chateau), Monte Corona² (Mt. Coronet), Vetta Lancia¹ (Spear Pillar), Gran Palude¹ (Great Marsh), Rovine di Flemminia¹ (Solaceon Ruins), Isola Ferrosa¹ (Iron Island), Torre Memoria² (Lost Tower), Covo Team Galassia (Covo del Team Galassia)¹ (Galactic HQ), Tempio di Nevepoli¹ (Snowpoint Temple), Grotta Ritorno¹ (Turnback Cave), Distortion World² (Distortion World), Monte Ostile¹ (Stark Mountain), Via Vittoria¹ (Victory Road), Torre Lotta¹ (Battle Tower), Parco Lotta² (Battle Frontier).

Il «Distortion World» resta in inglese nel testo italiano di Platino: 5 righe contro 1 «Mondo Distorto».

### Capipalestra (ordine di Platino; in DP Fannie è la quinta e Marzia la terza)
| # | Capopalestra (IT) | EN | Tipo | Città | Medaglia (IT) | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Pedro** | Roark | Roccia | Mineropoli¹ | Medaglia Carbone | Coal Badge | alta / media | giochi:Pedro (4/4); GCC:Pedro; PokeRogue:Pedro |
| 2 | **Gardenia** | Gardenia | Erba | Evopoli¹ | Medaglia Bosco | Forest Badge | alta / media | giochi:Gardenia (4/4); GCC:Gardenia; PokeRogue:Gardenia |
| 3 | **Fannie** | Fantina | Spettro | Cuoripoli¹ | Medaglia Vestigia | Relic Badge | alta / media | giochi:Fannie (7/7); GCC:Fannie; PokeRogue:Fannie |
| 4 | **Marzia** | Maylene | Lotta | Rupepoli¹ | Medaglia Ciottolo | Cobble Badge | alta / media | giochi:Marzia (4/4); PokeRogue:Marzia |
| 5 | **Omar** | Crasher Wake | Acqua | Pratopoli¹ | Medaglia Acquitrino | Fen Badge | alta / media | giochi: «Omar il Distruttore» (titolo completo); GCC «Omar il Distruttore»; PokeRogue «Omar» |
| 6 | **Ferruccio** | Byron | Acciaio | Canalipoli¹ | Medaglia Cava | Mine Badge | alta / media | giochi:Ferruccio (4/4); PokeRogue:Ferruccio |
| 7 | **Bianca** | Candice | Ghiaccio | Nevepoli¹ | Medaglia Ghiacciolo | Icicle Badge | alta / media | giochi:Bianca (4/4); GCC:Bianca; PokeRogue:Bianca |
| 8 | **Corrado** | Volkner | Elettro | Arenipoli¹ | Medaglia Faro | Beacon Badge | alta / media | giochi:Corrado (4/4); GCC:Corrado; PokeRogue:Corrado |

### Superquattro e Campione
| Nome (IT) | EN | Tipo | Conf. | Evidenze |
|---|---|---|---|---|
| **Aaron** | Aaron | Coleottero | alta | giochi:Aaron (4/4); PokeRogue:Aaron |
| **Terrie** | Bertha | Terra | alta | giochi:Terrie (4/4); PokeRogue:Terrie |
| **Vulcano** | Flint | Fuoco | alta | giochi:Vulcano (4/4); PokeRogue:Vulcano |
| **Luciano** | Lucian | Psico | alta | giochi:Luciano (4/4); GCC:Luciano; PokeRogue:Luciano |
| **Camilla** | Cynthia | — | alta | giochi:Camilla (5/5); GCC:Camilla; PokeRogue:Camilla |

- **Professore**: Professor Rowan (conf. alta: «Rowan» in 125 righe su 125 di Platino)
- **Squadra malvagia**: Team Galassia, capo **Cyrus** (conf. alta). Comandanti: Martes (Mars, GCC), Giovia (Jupiter) e Saturno (Saturn) da PokeRogue (conf. media); Plutinio/Charon non verificato
- **Rivale**: **Barry**, conf. media. In BDSP è «Barry»; nei testi di DP e Platino la riga «Barry» corrisponde invece ad «Alvise» (forse un allenatore omonimo). Protagonisti: Lucas e **Lucinda** (Dawn, GCC). Altri: **Bellocchio** (Looker, GCC), **Spino** (Thorton)
- **Leggendari e mitici**: Uxie, Mesprit, Azelf (trio dei laghi), Dialga, Palkia, Giratina, Heatran, Regigigas, Cresselia; Phione, Manaphy, Darkrai, Shaymin, Arceus (mitici)
- **Starter**: Turtwig, Chimchar, Piplup

---
## Unima, in inglese Unova (Gen 5, specie #494–649)

**Città**: Soffiolieve¹ (Nuvema Town), Quattroventi¹ (Accumula Town), Levantopoli¹ (Striaton City), Zefiropoli¹ (Nacrene City), Austropoli¹ (Castelia City), Sciroccopoli¹ (Nimbasa City), Libecciopoli¹ (Driftveil City), Ponentopoli¹ (Mistralton City), Mistralopoli¹ (Icirrus City), Boreduopoli¹ (Opelucid City), Fortebrezza¹ (Lacunosa Town), Spiraria¹ (Undella Town), Città Nera¹ (Black City), Foresta Bianca¹ (White Forest), Alisopoli¹ (Aspertia City), Venturia¹ (Floccesy Town), Zondopoli¹ (Virbank City), Poggiovento¹ (Lentimas Town), Grecalopoli¹ (Humilau City)

**Percorsi**: Percorso 1–23.

**Luoghi chiave**: Cantiere dei Sogni¹ (Dreamyard), Bosco Girandola¹ (Pinwheel Forest), Ponte Freccialuce¹ (Skyarrow Bridge), Deserto della Quiete¹ (Desert Resort), Castello Sepolto¹ (Relic Castle), Cava Pietrelettrica¹ (Chargestone Cave), Torre Cielo¹ (Celestial Tower), Monte Vite¹ (Twist Mountain), Torre Dragospira¹ (Dragonspiral Tower), Fossa Gigante¹ (Giant Chasm), Palazzo di N¹ (N's Castle), Via Vittoria¹ (Victory Road), Metrò Lotta¹ (Battle Subway), Pokéwood¹ (PokéStar Studios), Galleria Solidarietà¹ (Join Avenue), Fregata Plasma¹ (Plasma Frigate), Rovine degli Abissi¹ (Abyssal Ruins), Monte Antipodi¹ (Reversal Mountain), Lega Pokémon¹ (Pokémon League).

### Capipalestra (Nero e Bianco)
| # | Capopalestra (IT) | EN | Tipo | Città | Medaglia (IT) | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Spighetto** | Cilan | Erba | Levantopoli¹ | Medaglia Tris | Trio Badge | alta / alta | giochi:Spighetto (2/2); GCC:Spighetto; PokeRogue:Spighetto |
| 2 | **Chicco** | Chili | Fuoco | Levantopoli¹ | Medaglia Tris | Trio Badge | alta / alta | giochi:Chicco (2/2); PokeRogue:Chicco |
| 3 | **Maisello** | Cress | Acqua | Levantopoli¹ | Medaglia Tris | Trio Badge | alta / alta | giochi:Maisello (2/2); PokeRogue:Maisello |
| 4 | **Aloé** | Lenora | Normale | Zefiropoli¹ | Medaglia Base | Basic Badge | alta / alta | giochi:Aloé (3/3); PokeRogue:Aloé |
| 5 | **Artemisio** | Burgh | Coleottero | Austropoli¹ | Medaglia Scarabeo | Insect Badge | alta / alta | giochi:Artemisio (3/3); PokeRogue:Artemisio |
| 6 | **Camelia** | Elesa | Elettro | Sciroccopoli¹ | Medaglia Volt | Bolt Badge | alta / alta | giochi:Camelia (3/3); PokeRogue:Camelia |
| 7 | **Rafan** | Clay | Terra | Libecciopoli¹ | Medaglia Sisma | Quake Badge | alta / alta | giochi:Rafan (3/3); GCC:Rafan; PokeRogue:Rafan |
| 8 | **Anemone** | Skyla | Volante | Ponentopoli¹ | Medaglia Jet | Jet Badge | alta / alta | giochi:Anemone (3/3); GCC:Anemone; PokeRogue:Anemone |
| 9 | **Silvestro** | Brycen | Ghiaccio | Mistralopoli¹ | Medaglia Stalattite | Freeze Badge | alta / alta | giochi:Silvestro (2/2); PokeRogue:Silvestro |
| 10 | **Aristide** | Drayden | Drago | Boreduopoli¹ | Medaglia Leggenda | Legend Badge | alta / alta | giochi:Aristide (3/3); PokeRogue:Aristide |
| 11 | **Iris** | Iris | Drago | Boreduopoli¹ | Medaglia Leggenda | Legend Badge | alta / alta | giochi:Iris (1/1); GCC:Iris; PokeRogue:Iris |

A Levantopoli i tre fratelli sono alternativi: si affronta quello col tipo forte contro lo starter scelto. A Boreduopoli c'è Aristide in Nero e Iris in Bianco.

### Capipalestra (Nero 2 e Bianco 2)
| # | Capopalestra (IT) | EN | Tipo | Città | Medaglia (IT) | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Komor** | Cheren | Normale | Alisopoli¹ | Medaglia Base | Basic Badge | alta / alta | giochi:Komor (4/4); GCC:Komor; PokeRogue:Komor |
| 2 | **Velia** | Roxie | Veleno | Zondopoli¹ | Medaglia Arsenico | Toxic Badge | alta / alta | giochi:Velia (2/2); GCC:Velia; PokeRogue:Velia |
| 3 | **Artemisio** | Burgh | Coleottero | Austropoli¹ | Medaglia Scarabeo | Insect Badge | alta / alta | giochi:Artemisio (3/3); PokeRogue:Artemisio |
| 4 | **Camelia** | Elesa | Elettro | Sciroccopoli¹ | Medaglia Volt | Bolt Badge | alta / alta | giochi:Camelia (3/3); PokeRogue:Camelia |
| 5 | **Rafan** | Clay | Terra | Libecciopoli¹ | Medaglia Sisma | Quake Badge | alta / alta | giochi:Rafan (3/3); GCC:Rafan; PokeRogue:Rafan |
| 6 | **Anemone** | Skyla | Volante | Ponentopoli¹ | Medaglia Jet | Jet Badge | alta / alta | giochi:Anemone (3/3); GCC:Anemone; PokeRogue:Anemone |
| 7 | **Aristide** | Drayden | Drago | Boreduopoli¹ | Medaglia Leggenda | Legend Badge | alta / alta | giochi:Aristide (3/3); PokeRogue:Aristide |
| 8 | **Ciprian** | Marlon | Acqua | Grecalopoli¹ | Medaglia Onda | Wave Badge | alta / alta | giochi:Ciprian (2/2); PokeRogue:Ciprian |

### Superquattro e Campione
| Nome (IT) | EN | Tipo | Conf. | Evidenze |
|---|---|---|---|---|
| **Antemia** | Shauntal | Spettro | alta | giochi: Shauntal→Antemia 9/9 (NB), 12/12 (N2B2); GCC; PokeRogue |
| **Mirton** | Grimsley | Buio | alta | giochi: 4/4, 7/7; GCC; PokeRogue |
| **Catlina** | Caitlin | Psico | alta | giochi: 8/8, 13/13; GCC; PokeRogue |
| **Marzio** | Marshal | Lotta | alta | giochi: Marshal→Marzio 8/10 (NB), 15/18 (N2B2); PokeRogue |
| **Nardo** | Alder | Coleottero | alta | giochi:Nardo (1/1); PokeRogue:Nardo |
| **Iris** | Iris | Drago | alta | giochi:Iris (1/1); GCC:Iris; PokeRogue:Iris |

Il Campione è **Nardo** (Alder) in NB e **Iris** in N2B2.

- **Professoressa**: **Aralia** (Juniper), conf. alta: «Juniper»→«Aralia» in 85 righe su 87 di NB; GCC «Prof.ssa Aralia»
- **Squadra malvagia**: Team Plasma, con **Ghecis** (Ghetsis, conf. alta: 86/88) e **N** (alta); in N2B2 **Acromio** (Colress, alta: 33/38). I «Saggi» e il Trio Oscuro sono da GCC
- **Rivali**: **Komor** (Cheren) e **Belle** (Bianca) in NB, **Toni** (Hugh) in N2B2; tutti conf. alta. Protagonisti: Anita (Hilda, GCC) e **Rina** (Rosa, GCC)
- **Leggendari e mitici**: Cobalion, Terrakion, Virizion (Spadaccini Giusti), Tornadus, Thundurus, Landorus (Forze della Natura), Reshiram, Zekrom, Kyurem; Victini, Keldeo, Meloetta, Genesect (mitici)
- **Starter**: Snivy, Tepig, Oshawott

---
## Kalos (Gen 6, specie #650–721)

**Città**: Borgo Bozzetto (Vaniville Town), Rio Acquerello (Aquacorde Town), Novartopoli (Santalune City), Luminopoli (Lumiose City), Castel Vanità (Camphrier Town), Altoripoli (Cyllage City), Petroglifari (Ambrette Town), Cromleburgo (Geosenge Town), Yantaropoli (Shalour City), Temperopoli (Coumarine City), Romantopoli (Laverre City), Frescovilla (Dendemille Town), Fluxopoli (Anistar City), Ponte Mosaico (Couriway Town), Fractalopoli (Snowbelle City), Batikopoli (Kiloude City)

**Percorsi**: Percorso 1–22, ciascuno con un nome proprio, es. Vicolo Bozzetto (Vaniville Pathway), Via Progresso (Avance Trail), Via Aperta (Ouvert Way), Viale Parterre (Parterre Way), Via Versante (Versant Road), Boulevard Palazzo (Palais Lane), Via Fiume (Rivière Walk), Muraglia Costiera (Muraille Coast) …

**Luoghi chiave**: Bosco Novartopoli (Santalune Forest), Torre Prisma (Prism Tower), Laboratori Elisio (Lysandre Labs), Reggia Aurea (Parfum Palace), Grotta dei Bagliori (Glittering Cave), Grotta dei Riflessi (Reflection Cave), Torre Maestra (Tower of Mastery), Centrale di Kalos (Kalos Power Plant), Fabbrica Poké Ball (Poké Ball Factory), Caverna Gelata (Frost Cavern), Covo del Team Flare (Team Flare Secret HQ), Grotta Climax (Terminus Cave), Valle dei Pokémon (Pokémon Village), Via Vittoria (Victory Road), Villa Lotta (Battle Maison), Castello Lotta (Battle Chateau), Albergo Diroccato (Lost Hotel), Antro Talassico (Sea Spirit’s Den).

### Capipalestra (X e Y)
| # | Capopalestra (IT) | EN | Tipo | Città | Medaglia (IT) | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Violetta** | Viola | Coleottero | Novartopoli | Medaglia Insetto | Bug Badge | alta / alta | giochi:Violetta (4/4); PokeRogue:Violetta |
| 2 | **Lino** | Grant | Roccia | Altoripoli | Medaglia Rupe | Cliff Badge | alta / alta | giochi:Lino (3/3); GCC:Lino; PokeRogue:Lino |
| 3 | **Ornella** | Korrina | Lotta | Yantaropoli | Medaglia Lotta | Rumble Badge | alta / alta | giochi:Ornella (6/6); GCC:Ornella; PokeRogue:Ornella |
| 4 | **Amur** | Ramos | Erba | Temperopoli | Medaglia Pianta | Plant Badge | alta / alta | giochi:Amur (3/3); PokeRogue:Amur |
| 5 | **Lem** | Clemont | Elettro | Luminopoli | Medaglia Voltaggio | Voltage Badge | alta / alta | giochi:Lem (3/3); PokeRogue:Lem |
| 6 | **Valérie** | Valerie | Folletto | Romantopoli | Medaglia Folletto | Fairy Badge | alta / alta | giochi:Valérie (4/4); PokeRogue:Valérie |
| 7 | **Astra** | Olympia | Psico | Fluxopoli | Medaglia Psico | Psychic Badge | alta / alta | giochi:Astra (4/4); PokeRogue:Astra |
| 8 | **Edel** | Wulfric | Ghiaccio | Fractalopoli | Medaglia Iceberg | Iceberg Badge | alta / alta | giochi:Edel (3/3); PokeRogue:Edel |

### Superquattro e Campione
| Nome (IT) | EN | Tipo | Conf. | Evidenze |
|---|---|---|---|---|
| **Malva** | Malva | Fuoco | alta | giochi:Malva (3/3); PokeRogue:Malva |
| **Narciso** | Siebold | Acqua | alta | giochi:Narciso (2/2); GCC:Narciso; PokeRogue:Narciso |
| **Timeus** | Wikstrom | Acciaio | alta | giochi:Timeus (2/2); PokeRogue:Timeos |
| **Lilia** | Drasna | Drago | alta | giochi:Lilia (2/2); GCC:Lilia; PokeRogue:Lila |
| **Diantha** | Diantha | — | alta | giochi:Diantha (3/3); GCC:Diantha; PokeRogue:Diantha |

- **Professore**: **Platan** (Sycamore), conf. alta
- **Squadra malvagia**: Team Flare, capo **Elisio** (Lysandre, alta); scienziato **Xante** (Xerosic, GCC); ufficiali secondo PokeRogue (media): Bromelia (Bryony), Akebia (Aliana), Cytisia (Celosia), Martynia (Mable). **AZ** (alta)
- **Rivali e amici**: **Shana** (Shauna), Tierno, **Trovato** (Trevor), tutti alta; Calem/Serena (protagonisti, GCC «Serena»). **Lem** (Clemont) e **Clem** (Bonnie, GCC)
- **Leggendari e mitici**: Xerneas, Yveltal, Zygarde; Diancie, Hoopa, Volcanion (mitici)
- **Starter**: Chespin, Fennekin, Froakie

---
## Alola (Gen 7, specie #722–809)

**Isole** (giochi: «Melemele Island»→«Mele Mele» ecc.): Mele Mele, Akala, Ula Ula, Poni, più l'isola artificiale Æther Paradise.

**Città e villaggi**: Lili (Iki Town), Hau’oli¹ (Hau'oli City), Kantai (Heahea City), Ohana (Paniola Town), Konikoni (Konikoni City), Malie¹ (Malie City), Villaggio Tapu (Tapu Village), Poh (Po Town), Villaggio del Mare (Seafolk Village)

**Percorsi**: Percorso 1–17.

**Luoghi chiave**: Collina Diecicarati (Ten Carat Hill), Grotta Sottobosco¹ (Verdant Cavern), Tempio del Conflitto (Ruins of Conflict), Collina Scrosciante (Brooklet Hill), Parco Vulcano Wela (Wela Volcano Park), Giungla Ombrosa (Lush Jungle), Tempio della Vita (Ruins of Life), Æther Paradise (Aether Paradise), Deserto Haina (Haina Desert), Tempio del Raccolto (Ruins of Abundance), Picco Hokulani (Mount Hokulani), Osservatorio (Hokulani Observatory), Viale Royale¹ (Royal Avenue), Canyon di Poni (Vast Poni Canyon), Altare Solare (Altar of the Sunne), Altare Lunare (Altar of the Moone), Tempio del Passaggio (Ruins of Hope), Monte Lanakila (Mount Lanakila), Lega Pokémon (Pokémon League), Albero della Lotta (Battle Tree), Ultramondo (Ultra Space), Ultramegalopoli¹ (Ultra Megalopolis), Poké Resort² (Poké Pelago).

Ad Alola non ci sono Palestre. C'è il **giro delle isole**: prove dei Capitani e **GRANDE PROVA** del Kahuna di ogni isola, che come premio dà un Cristallo Z al posto della medaglia.

### Kahuna (GRANDE PROVA)
| # | Kahuna (IT) | EN | Tipo | Luogo | Cristallo Z (IT) | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Hala** | Hala | Lotta | Lili | Luctium Z | Fightinium Z | alta / alta | giochi:Hala (7/7); GCC:Hala; PokeRogue:Hala |
| 2 | **Alyxia** | Olivia | Roccia | Konikoni | Petrium Z | Rockium Z | alta / alta | giochi:Alyxia (8/8); GCC:Alyxia; PokeRogue:Olive |
| 3 | **Augusto** | Nanu | Buio | Malie¹ | Obscurium Z | Darkinium Z | alta / alta | giochi:Augusto (6/6); GCC:Augusto |
| 4 | **Hapi** | Hapu | Terra | Villaggio del Mare | Terrium Z | Groundium Z | alta / alta | giochi:Hapi (6/6); GCC:Hapi |

### Capitani (prove)
| # | Capitano/a (IT) | EN | Tipo | Luogo prova | Cristallo Z (IT) | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Liam** | Ilima | Normale | Grotta Sottobosco¹ | Normium Z | Normalium Z | alta / alta | giochi:Liam (12/12); GCC:Liam |
| 2 | **Suiren** | Lana | Acqua | Collina Scrosciante | Idrium Z | Waterium Z | alta / alta | giochi:Suiren (6/6); GCC:Suiren |
| 3 | **Kawe** | Kiawe | Fuoco | Parco Vulcano Wela | Pirium Z | Firium Z | alta / alta | giochi:Kawe (8/8); GCC:Kawe |
| 4 | **Ibis** | Mallow | Erba | Giungla Ombrosa | Herbium Z | Grassium Z | alta / alta | giochi:Ibis (8/8); GCC:Ibis |
| 5 | **Chrys** | Sophocles | Elettro | Osservatorio | Electrium Z | Electrium Z | alta / alta | giochi:Chrys (7/7); GCC:Chrys |
| 6 | **Malpi** | Acerola | Spettro | Supermarket Affaroni | Spectrium Z | Ghostium Z | alta / alta | giochi:Malpi (7/7); GCC:Malpi; PokeRogue:Malpi |
| 7 | **Rika** | Mina | Folletto | Canyon di Poni (indicativo) | Follectium Z | Fairium Z | alta / alta | giochi:Rika (5/5); GCC:Rika |

Rika (Mina) è la Capitana di tipo Folletto di Poni: nome con conf. alta (5 righe su 5 nei testi di gioco, GCC). In SL non ha una prova con Pokémon dominante durante la storia, in USUL sì; il luogo indicato in tabella è indicativo (conf. bassa). Hapi diventa Kahuna di Poni durante la storia.

### Superquattro e Campione (Lega di Monte Lanakila)
| Nome (IT) | EN | Tipo | Conf. | Evidenze |
|---|---|---|---|---|
| **Hala** | Hala | Lotta | alta | giochi:Hala (7/7); GCC:Hala; PokeRogue:Hala |
| **Tapso** | Molayne | Acciaio | alta | giochi:Tapso (6/6); GCC:Tapso; PokeRogue:Tapso |
| **Alyxia** | Olivia | Roccia | alta | giochi:Alyxia (8/8); GCC:Alyxia; PokeRogue:Olive |
| **Malpi** | Acerola | Spettro | alta | giochi:Malpi (7/7); GCC:Malpi; PokeRogue:Malpi |
| **Kahili** | Kahili | Volante | alta | giochi:Kahili (6/6); GCC:Kahili; PokeRogue:Kahili |
| **Kukui** | Kukui | — | alta | giochi:Kukui (14/14); PokeRogue:Kukui |
| **Hau** | Hau | — | alta | giochi:Hau (74/74); GCC:Hau; PokeRogue:Hau |

Superquattro di SL: Hala, Alyxia, Malpi, Kahili. In USUL **Tapso** (Molayne) prende il posto di Hala. Il giocatore diventa il **primo Campione**. Lo sfidante finale è il **Professor Kukui** in SL e **Hau** in USUL.

- **Professore**: Professor Kukui (alta). Professoressa **Magnolia** (Burnet: GCC «Professoressa Magnolia», giochi «Burnet»→«Magnolia» in 13 righe su 16)
- **Squadre malvagie**: Team Skull, capo **Guzman** (Guzma), amministratrice Plumeria; tutti alta. **Fondazione Æther** con la presidente **Samina** (Lusamine) e **Vicio** (Faba, GCC); alta. In USUL anche Ultrapattuglia e Team Rainbow Rocket
- **Rivali e amici**: **Hau**, **Iridio** (Gladion), **Lylia** (Lillie); tutti alta
- **Leggendari e mitici**: Tipo Zero/Silvally, Tapu Koko, Tapu Lele, Tapu Bulu, Tapu Fini, Cosmog, Cosmoem, Solgaleo, Lunala, Necrozma; Ultracreature (Nihilego, Buzzwole, Pheromosa, Xurkitree, Celesteela, Kartana, Guzzlord, Poipole/Naganadel, Stakataka, Blacephalon); Magearna, Marshadow, Zeraora, Meltan, Melmetal (mitici)
- **Starter**: Rowlet, Litten, Popplio

---
## Galar (Gen 8, specie #810–898; Hisui #899–905)

**Città**: Furlongham¹ (Postwick), Brassbury¹ (Wedgehurst), Steamington¹ (Motostoke), Turffield¹ (Turffield), Keelford¹ (Hulbury), Knuckleburgh¹ (Hammerlocke), Latermore¹ (Stow-on-Side), Piquedilly¹ (Ballonlea), Circhester¹ (Circhester), Spikeville¹ (Spikemuth), Goalwick¹ (Wyndon), Freezedale¹ (Freezington)

**Percorsi**: Percorso 1–10. Le **Terre Selvagge** (Wild Area, nei giochi) hanno zone come Pianura Serena¹ (Rolling Fields), Boschetto Ombraluce¹ (Dappled Grove), Torre Diroccata¹ (Watchtower Ruins), Lago Axew (est)¹ (East Lake Axewell), Lago Axew (ovest)¹ (West Lake Axewell), Occhio del Lago Axew¹ (Axew's Eye), Lago Milotic (sud)¹ (South Lake Miloch), Lago Milotic (nord)¹ (North Lake Miloch), Sedia del Gigante¹ (Giant's Seat), Fiume di Steamington¹ (Motostoke Riverbank), Piana dei Ponti¹ (Bridge Field), Landa delle Pietre¹ (Stony Wilderness), Conca delle Sabbie¹ (Dusty Bowl), Specchio del Gigante¹ (Giant's Mirror), Colle Knuckleburgh¹ (Hammerlocke Hills), Berretto del Gigante¹ (Giant's Cap), Lago Dragofuria¹ (Lake of Outrage).

**Luoghi chiave**: Bosco Assopito¹ (Slumbering Weald), Miniera di Galar¹ (Galar Mine), Miniera 2¹ (Galar Mine No. 2), Bosco Brillabirinto¹ (Glimwood Tangle), Centrale Energetica¹ (Energy Plant), Torre Lotta¹ (Battle Tower), Stadio di Goalwick² (Wyndon Stadium), Rose Tower² (Rose Tower). DLC: Isola dell'Armatura² (Isle of Armor), Landa Corona² (Crown Tundra), Dojo Master¹ (Master Dojo), Tempio Corona¹ (Crown Shrine), Dynatana max¹ (Max Lair).

La **Sfida delle Palestre** (Gym Challenge) si conclude con la **Coppa Campione** (Champion Cup) allo Stadio di Goalwick.

### Capipalestra (Spada/Scudo; la 4ª e la 6ª Palestra cambiano a seconda della versione)
| # | Capopalestra (IT) | EN | Tipo | Città | Medaglia (IT) | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Yarrow** | Milo | Erba | Turffield¹ | Medaglia Erba | Grass Badge | alta / alta | giochi:Yarrow (10/10); GCC:Yarrow; PokeRogue:Yarrow |
| 2 | **Azzurra** | Nessa | Acqua | Keelford¹ | Medaglia Acqua | Water Badge | alta / alta | giochi:Azzurra (9/9); GCC:Azzurra; PokeRogue:Azzurra |
| 3 | **Kabu** | Kabu | Fuoco | Steamington¹ | Medaglia Fuoco | Fire Badge | alta / alta | giochi:Kabu (7/7); GCC:Kabu; PokeRogue:Kabu |
| 4 | **Fabia** | Bea | Lotta | Latermore¹ | Medaglia Lotta | Fighting Badge | alta / alta | giochi:Fabia (6/6); GCC:Fabia; PokeRogue:Fabia |
| 5 | **Onion** | Allister | Spettro | Latermore¹ | Medaglia Spettro | Ghost Badge | alta / alta | giochi:Onion (6/6); GCC:Onion; PokeRogue:Onion |
| 6 | **Poppy** | Opal | Folletto | Piquedilly¹ | Medaglia Folletto | Fairy Badge | alta / media | giochi:Poppy (8/8); GCC:Poppy; PokeRogue:Poppy |
| 7 | **Milo** | Gordie | Roccia | Circhester¹ | Medaglia Roccia | Rock Badge | alta / alta | giochi:Milo (6/6); GCC:Milo; PokeRogue:Milo |
| 8 | **Melania** | Melony | Ghiaccio | Circhester¹ | Medaglia Ghiaccio | Ice Badge | alta / alta | giochi:Melania (6/6); GCC:Melania; PokeRogue:Melania |
| 9 | **Ginepro** | Piers | Buio | Spikeville¹ | Medaglia Buio | Dark Badge | alta / media | giochi:Ginepro (6/6); GCC:Ginepro; PokeRogue:Ginepro |
| 10 | **Laburno** | Raihan | Drago | Knuckleburgh¹ | Medaglia Drago | Dragon Badge | alta / alta | giochi:Laburno (6/6); GCC:Laburno; PokeRogue:Raihan |

Attenzione all'omonimia: in italiano **Milo** è Gordie (Roccia, Spada), mentre il Milo inglese (Erba) si chiama **Yarrow**. Fabia (Bea) e Milo (Gordie) sono in Spada, Onion (Allister) e Melania (Melony) in Scudo. La Palestra di Ginepro (Piers) a Spikeville non ha uno stadio Dynamax.

### Campione
| Nome (IT) | EN | Tipo | Conf. | Evidenze |
|---|---|---|---|---|
| **Dandel** | Leon | — | alta | giochi:Dandel (16/16); GCC:Dandel; PokeRogue:Dandel |

Prima di Dandel c'è il torneo finale, la Coppa Campione: semifinale contro Mary (Marnie) o Beet (Bede), poi finale contro Laburno (Raihan).

- **Professoressa**: **Flora** (Professor Magnolia, alta: «Magnolia»→«Flora» in 19 righe su 19 di SpSc). La nipote **Sonia** (alta). Attenzione: «Magnolia» in italiano è la professoressa di Alola (Burnet)
- **Antagonisti**: **Team Yell** (tifosi di Mary) e **Macro Cosmos**, con il presidente **Rose** (alta) e la segretaria **Olive** (Oleana, alta); Brandobaldo e Scudobaldo (Sordward e Shielbert, GCC). DLC: **Mustard** (alta), **Peony** (GCC «Peony»)
- **Rivali**: **Hop**, **Mary** (Marnie), **Beet** (Bede); tutti alta. Protagonisti: Victor/Gloria (GCC «Gloria»)
- **Leggendari e mitici**: Zacian, Zamazenta, Eternatus; DLC: Kubfu/Urshifu, Regieleki, Regidrago, Glastrier, Spectrier, Calyrex; Zarude (mitico)
- **Starter**: Grookey, Scorbunny, Sobble

---
## Paldea (Gen 9, specie #906–1025)

**Città**: Dosilla¹ (Cabo Poco), Platia¹ (Los Platos), Mesapoli¹ (Mesagoza), Moldulcia¹ (Cortondo), Los Tazones¹ (Artazon), Leudapoli¹ (Levincia), Garrafopoli¹ (Cascarrafa), Marinada¹ (Porto Marinada), Mesturia¹ (Medali), Neveria¹ (Montenevera), Las Brasas¹ (Alfornada), Picadia¹ (Zapapico)

**Zone** (al posto dei percorsi numerati ci sono le province): Area 1 Sud¹ (South Province (Area One)), Area 1 Est¹ (East Province (Area One)), Area 1 Ovest¹ (West Province (Area One)), Area 1 Nord¹ (North Province (Area One)) …

**Luoghi chiave**: Accademia Arancia¹ (Naranja Academy), Accademia Uva¹ (Uva Academy), Sentiero di Dosilla¹ (Poco Path), Grotta della Baia¹ (Inlet Grotto), Deserto Alasar¹ (Asado Desert), Boschetto dei Segni¹ (Tagtree Thicket), Lago Gran Caldero¹ (Casseroya Lake), Sierra Napada¹ (Glaseado Mountain), Passaggio Mescadia¹ (Dalizapa Passage), Bosco Torrado¹ (Socarrat Trail), Area Zero¹ (Area Zero), Laboratorio Zero¹ (Zero Lab), Lega Pokémon¹ (Pokémon League), Santuario del legno marcito² (Grasswither Shrine), Santuario del gelo lacerante² (Icerend Shrine), Santuario della terra impura² (Groundblight Shrine), Santuario del rogo funesto² (Firescourge Shrine), Voragine di Paldea² (Great Crater of Paldea). DLC «Maschera Turchese»/«Disco Indaco»: Verdegiada¹ (Mossui Town), Via Nordivia¹ (Kitakami Road), Monte Orco¹ (Oni Mountain), Foresta Perpetua¹ (Timeless Woods), Istituto Mirtillo² (Blueberry Academy), Bioterarium² (Terarium).

Tre storie, nomi verificati nel testo di ScVi: **Il cammino dei Campioni** (Victory Road, le Palestre), **Il sentiero leggendario** (Path of Legends, i Pokémon dominanti) e **Il viale della polvere di stelle** (Starfall Street, il Team Star).

### Capipalestra (Scarlatto/Violetto)
Le Palestre di Paldea assegnano una medaglia generica: nel testo inglese c'è solo «Gym Badge», senza nomi tematici. Nel corpus le Palestre si chiamano «Palestra di <città>», es. «Palestra di Moldulcia» (Cortondo Gym).

| # | Capopalestra (IT) | EN | Tipo | Città | Medaglia | EN | Conf. nome / medaglia | Evidenze |
|---|---|---|---|---|---|---|---|---|
| 1 | **Aceria** | Katy | Coleottero | Moldulcia¹ | — | — | alta / — | giochi:Aceria (5/5); GCC:Aceria; PokeRogue:Aceria |
| 2 | **Brassius** | Brassius | Erba | Los Tazones¹ | — | — | alta / — | giochi:Brassius (5/5); GCC:Brassius; PokeRogue:Brassius |
| 3 | **Kissara** | Iono | Elettro | Leudapoli¹ | — | — | alta / — | giochi:Kissara (5/5); GCC:Kissara; PokeRogue:Kissara |
| 4 | **Algaro** | Kofu | Acqua | Garrafopoli¹ | — | — | alta / — | giochi:Algaro (5/5); GCC:Algaro; PokeRogue:Algaro |
| 5 | **Ubaldo** | Larry | Normale | Mesturia¹ | — | — | alta / — | giochi:Ubaldo (5/5); GCC:Ubaldo; PokeRogue:Ubaldo |
| 6 | **Lima** | Ryme | Spettro | Neveria¹ | — | — | alta / — | giochi:Lima (5/5); GCC:Lima; PokeRogue:Ryme |
| 7 | **Tulipa** | Tulip | Psico | Las Brasas¹ | — | — | alta / — | giochi:Tulipa (5/5); GCC:Tulipa; PokeRogue:Tulipa |
| 8 | **Grusha** | Grusha | Ghiaccio | Sierra Napada¹ | — | — | alta / — | giochi:Grusha (5/5); GCC:Grusha; PokeRogue:Grusha |

### Superquattro e Campionessa Suprema
| Nome (IT) | EN | Tipo | Conf. | Evidenze |
|---|---|---|---|---|
| **Capsi** | Rika | Terra | alta | giochi:Capsi (3/3); GCC:Capsi; PokeRogue:Rika |
| **Verina** | Poppy | Acciaio | alta | giochi:Verina (3/3); GCC:Verina; PokeRogue:Poppy |
| **Ubaldo** | Larry | Volante | alta | giochi:Ubaldo (5/5); GCC:Ubaldo; PokeRogue:Ubaldo |
| **Oranzio** | Hassel | Drago | alta | giochi:Oranzio (3/3); GCC:Oranzio; PokeRogue:Oranzio |
| **Alisma** | Geeta | — | alta | giochi:Alisma (3/3); GCC:Alisma; PokeRogue:Alisma |
| **Nemi** | Nemona | — | alta | giochi:Nemi (3/3); GCC:Nemi; PokeRogue:Nemi |

**Alisma** (Geeta) è la Campionessa Suprema (Top Champion). **Nemi** (Nemona) è una rivale che ha già il grado di Campionessa. Ubaldo (Larry) è sia Capopalestra (Normale) sia Superquattro (Volante).

- **Professori**: **Olim** (Sada, Scarlatto) e **Turum** (Turo, Violetto). Conf. alta: «Sada»→«Olim» in 34 righe su 52 e «Turo»→«Turum» in 34 su 47 di ScVi; GCC «Professoressa Olim», «Professor Turum»
- **Squadra**: **Team Star**, capo **Penny** (alta, «Cassiopea» come nome in codice). Capibanda, tutti conf. alta dai testi di gioco: **Romelio** (Giacomo, Banda Segin), **Pruna** (Mela, Banda Schedar), **Henzo** (Atticus, Banda Tsih/Navi), **Ortiz** (Ortega, Banda Ruchbah), **Nespera** (Eri, Banda Caph). Il preside è **Clavel** (Clavell, alta). La missione si chiama «Operazione Stardust» (Operation Starfall)
- **Rivali**: **Nemi**, **Pepe** (Arven), **Penny**; tutti alta. DLC: **Riben** (Kieran) e **Rubra** (Carmine), alta. Superquattro dell'Istituto Mirtillo, alta: **Piros** (Crispin), **Erin** (Amarys), **Rupi** (Lacey), **Aris** (Drayton)
- **Leggendari e mitici**: Koraidon, Miraidon; i «tesori portatori di sciagura» (Treasures of Ruin): Wo-Chien, Chien-Pao, Ting-Lu, Chi-Yu; Pokémon Paradosso leggendari: Acquecrespe (Walking Wake), Fogliaferrea (Iron Leaves), Vampeaguzze (Gouging Fire), Furiatonante (Raging Bolt), Massoferreo (Iron Boulder), Capoferreo (Iron Crown); DLC: i «beniamici» Okidogi, Munkidori e Fezandipiti, Ogerpon, Terapagos; Pecharunt (mitico)
- **Starter**: Sprigatito, Fuecoco, Quaxly

---
## Discrepanze trovate (e risolte)

Ricordi errati che di solito circolano, smentiti dai testi ufficiali:

- **Raihan** in italiano è **Laburno** (giochi 6/6, GCC). PokeRogue lo lascia «Raihan», ed è un suo errore.
- **Ryme** è **Lima** (giochi 5/5, GCC); PokeRogue lascia «Ryme».
- **Olivia** (Kahuna di Akala) è **Alyxia** (giochi 8/8, GCC); PokeRogue la confonde con Oleana e scrive «Olive».
- **Rika** (Superquattro di Paldea) è **Capsi**; **Poppy** (Superquattro di Paldea) è **Verina**. PokeRogue li lascia in inglese. Attenzione: **Poppy** in italiano è il nome di **Opal** (Galar) e **Rika** quello di **Mina** (Alola).
- **Wikstrom** è **Timeus** (giochi 2/2), non «Timeos» come in PokeRogue. **Drasna** è **Lilia** (giochi e GCC), non «Lila».
- **Juan** (8° Capopalestra in Smeraldo) è **Rodolfo** (giochi 5/5).
- Le **medaglie** di Hoenn, Sinnoh e Unima differiscono da molte liste non ufficiali: Hoenn usa Medaglia Fiamma (non «Calore») e Medaglia Armonia (non «Equilibrio»); Sinnoh usa Carbone, Bosco, Vestigia, Ciottolo, Acquitrino, Cava, Ghiacciolo, Faro; Unima usa Tris, Base, Scarabeo, Volt, Sisma, Jet, Stalattite, Leggenda, più Arsenico e Onda in N2B2.
- **Magnolia** (Galar) in italiano è **Flora**; «Professoressa Magnolia» in italiano è **Burnet** (Alola).

## Note per Pokémon Multiverse

- Limiti di lunghezza di pokeemerald-expansion: nomi degli allenatori `TRAINER_NAME_LENGTH` = 10 (controllo automatico), nomi delle mappe `MAP_NAME_LENGTH` = 16. Nomi più lunghi di 10 caratteri: «Omar il Distruttore» (19). Per Crasher Wake usare «Omar». I titoli «Professor/Professoressa» vanno nella classe allenatore, non nel nome.
- L'apostrofo tipografico «’» (es. «Hau’oli», «Isola dell’Armatura») è nel charmap (`B4`, uguale a `'`).
- Per i nomi delle mappe di Hoenn e Kanto già presenti nel ROM usare `mapsec_hoenn_kanto.json`, campo `it_ingame`: sono le stringhe maiuscole ufficiali di Smeraldo e RFVF italiani.

