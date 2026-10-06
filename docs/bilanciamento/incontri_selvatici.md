# Incontri selvatici di Hoenn (Pokémon Multiverse)

Generato da `tools/balance/wild_gen.py` e verificato da `tools/balance/check_wild.py`. Non modificare a mano: rigenerare.

## Riepilogo copertura

- Famiglie non leggendarie da coprire: **446**
- Famiglie coperte (forma base non baby presente in una tabella raggiungibile): **446** (100.0%)
- Famiglie regionali (Alola/Galar/Hisui/Paldea) coperte: **23/23**
- Tabelle Hoenn generate: **209** (slot: 1975)

### Copertura per generazione

| Gen | Famiglie coperte | Slot occupati |
|---|---|---|
| 1 | 87/87 | 404 |
| 2 | 49/49 | 211 |
| 3 | 64/64 | 301 |
| 4 | 31/31 | 146 |
| 5 | 73/73 | 285 |
| 6 | 31/31 | 131 |
| 7 | 34/34 | 156 |
| 8 | 37/37 | 142 |
| 9 | 40/40 | 199 |

## Regole applicate

- Livelli: curva vanilla → nuovo (2→2, 5→6, 12→13, 15→15, 19→19, 24→24, 29→29, 31→33, 33→37, 42→44, 46→49, 49→52, 55→57, 58→62, 70→72, 100→100), anche per la pesca.
- Evoluzioni: forme base sempre; evoluzioni per livello solo da livello evolutivo + 2; pietra/scambio/amicizia/altro solo da Lv 30 e in slot ≤ 5%.
- Potenza: slot ≤ Lv 15 solo specie con PS totali ≤ 330 e famiglie con BST finale ≤ 535 (max 1 famiglia forte in uno slot 1%); pseudo-leggendari da Lv 30 in slot 1-5%; famiglie degli starter da Lv 20 in slot 1-5% (max 1 per tabella); leggendari/misteriosi/UC/paradosso solo in aree post-game, slot 1%, Lv 60-70, max 1 per tabella.
- Habitat: ~70% degli slot terrestri con tipi dell'habitat, ~30% liberi; Surf = tipo Acqua; Pesca = pesci Acqua (gruppo uova Acqua 2/3), Amo Vecchio debole, Amo Buono medio, Super Amo forte; Spaccaroccia = Roccia/Terra.

## Tabelle per mappa

Percentuale = somma dei tassi degli slot della specie. *(non raggiungibile)* = tabella presente ma non usata nel gioco normale (esclusa dal calcolo della copertura).

| Mappa | Tipo | Livelli | Specie |
|---|---|---|---|
| Route101 | Erba/terreno | 2-3 | Bunnelby 30%, Karrablast 30%, Natu 10%, Rookidee 10%, Lillipup 5%, Sentret 5%, Shroodle 4%, Ducklett 4%, Shelmet 2% |
| Route102 | Erba/terreno | 3-5 | Binacle 31%, Capsakid 30%, Spearow 10%, Meowth 10%, Poliwag 5%, Poltchageist 5%, Chewtle 4%, Dewpider 4%, Teddiursa 1% |
| Route102 | Surf | 6-39 | Skrelp 60%, Krabby 30%, Basculin 5%, Spheal 4%, Frillish 1% |
| Route102 | Pesca | 6-48 | Goldeen 70%, Finizen 30%, Corphish 60%, Luvdisc 20%, Shellder 20%, Corsola 40%, Alomomola 40%, Clauncher 15%, Tatsugiri 4%, Qwilfish 1% |
| Route103 | Erba/terreno | 2-5 | Shellos 30%, Bidoof 30%, Tympole 10%, Surskit 10%, Psyduck 5%, Rattata (Alola) 5%, Marill 4%, Lotad 4%, Yungoos 2% |
| Route103 | Surf | 6-39 | Wimpod 60%, Wingull 30%, Horsea 5%, Omanyte 4%, Pyukumuku 1% |
| Route103 | Pesca | 6-48 | Wishiwashi 70%, Remoraid 30%, Arrokuda 60%, Finneon 20%, Chinchou 20%, Bruxish 40%, Veluza 40%, Wailmer 15%, Dondozo 4%, Relicanth 1% |
| Route104 | Erba/terreno | 3-6 | Mareanie 30%, Seedot 30%, Foongus 10%, Sizzlipede 10%, Cottonee 5%, Venipede 5%, Hoothoot 4%, Taillow 4%, Zigzagoon 2% |
| Route104 | Surf | 11-31 | Seel 60%, Slowpoke 30%, Barboach 5%, Staryu 4%, Tentacool 1% |
| Route104 | Pesca | 6-48 | Wiglett 70%, Carvanha 30%, Clauncher 60%, Binacle 20%, Krabby 20%, Tirtouga 40%, Kabuto 40%, Finizen 15%, Magikarp 4%, Cloyster 1% |
| Route105 | Surf | 6-39 | Buizel 60%, Panpour 30%, Wooper 5%, Clamperl 4%, Totodile 1% |
| Route105 | Pesca | 6-48 | Wimpod 70%, Wishiwashi 30%, Corphish 60%, Luvdisc 20%, Remoraid 20%, Omanyte 40%, Qwilfish 40%, Goldeen 15%, Lumineon 4%, Veluza 1% |
| Route110 | Erba/terreno | 13-14 | Doduo 30%, Oddish 30%, Voltorb (Hisui) 10%, Voltorb 10%, Skorupi 5%, Eevee 5%, Minccino 4%, Grimer 4%, Trubbish 2% |
| Route110 | Surf | 6-39 | Chewtle 60%, Surskit 30%, Feebas 5%, Squirtle 4%, Bibarel 1% |
| Route110 | Pesca | 6-48 | Wiglett 70%, Arrokuda 30%, Barboach 60%, Chinchou 20%, Finizen 20%, Wailmer 40%, Corsola 40%, Tentacool 15%, Alomomola 4%, Dondozo 1% |
| Route111 | Erba/terreno | 19-22 | Baltoy 30%, Dwebble 30%, Gligar 10%, Cubone 10%, Numel 5%, Anorith 5%, Hippopotas 4%, Toedscool 4%, Glimmet 2% |
| Route111 | Surf | 6-39 | Buizel 60%, Wingull 30%, Tauros (Paldea) 5%, Poliwag 4%, Mudkip 1% |
| Route111 | Spaccaroccia | 6-20 | Sandshrew 60%, Nincada 30%, Rockruff 5%, Sandile 4%, Nacli 1% |
| Route111 | Pesca | 6-48 | Carvanha 70%, Corphish 30%, Luvdisc 60%, Binacle 20%, Shellder 20%, Kabuto 40%, Bruxish 40%, Tirtouga 15%, Tatsugiri 4%, Starmie 1% |
| Route112 | Erba/terreno | 14-16 | Machop 30%, Golett 30%, Meditite 10%, Sandygast 10%, Salandit 5%, Aron 5%, Trapinch 4%, Stufful 4%, Pancham 2% |
| Route113 | Erba/terreno | 14-16 | Klink 30%, Grimer (Alola) 30%, Stunky 10%, Silicobra 10%, Growlithe (Hisui) 5%, Tinkatink 5%, Venonat 4%, Nosepass 4%, Amaura 2% |
| Route114 | Erba/terreno | 15-18 | Petilil 30%, Yamask (Galar) 30%, Phantump 10%, Gastly 10%, Snover 5%, Exeggcute 5%, Castform 4%, Ferroseed 4%, Varoom 2% |
| Route114 | Surf | 6-39 | Shellos 60%, Ducklett 30%, Arctovish 5%, Mareanie 4%, Froakie 1% |
| Route114 | Spaccaroccia | 6-20 | Diglett (Alola) 60%, Roggenrola 30%, Geodude (Alola) 5%, Drilbur 4%, Phanpy 1% |
| Route114 | Pesca | 6-48 | Barboach 70%, Wiglett 30%, Finneon 60%, Goldeen 20%, Remoraid 20%, Wailmer 40%, Basculin 40%, Staryu 15%, Relicanth 4%, Gyarados 1% |
| Route116 | Erba/terreno | 7-9 | Whismur 30%, Fomantis 30%, Fletchling 10%, Croagunk 10%, Pansage 5%, Tandemaus 5%, Jigglypuff 4%, Starly 4%, Sewaddle 2% |
| Route117 | Erba/terreno | 14-14 | Wooloo 30%, Cherubi 30%, Glameow 10%, Zorua (Hisui) 10%, Bellsprout 5%, Ditto 5%, Bramblin 4%, Fidough 4%, Paras 2% |
| Route117 | Surf | 6-39 | Clamperl 60%, Seel 30%, Mantine 5%, Panpour 4%, Quaxly 1% |
| Route117 | Pesca | 6-48 | Arrokuda 70%, Wishiwashi 30%, Clauncher 60%, Chinchou 20%, Krabby 20%, Tirtouga 40%, Bruxish 40%, Kabuto 15%, Golisopod 4%, Alomomola 1% |
| Route118 | Erba/terreno | 24-27 | Buneary 30%, Spinda 30%, Pincurchin 10%, Murkrow 10%, Rotom 5%, Dunsparce 5%, Scyther 4%, Farfetch'd 4%, Pachirisu 2% |
| Route118 | Surf | 6-39 | Psyduck 60%, Dewpider 30%, Skrelp 5%, Oshawott 4%, Frillish 1% |
| Route118 | Pesca | 6-48 | Wiglett 70%, Binacle 30%, Remoraid 60%, Barboach 20%, Luvdisc 20%, Veluza 40%, Tatsugiri 40%, Tentacool 15%, Qwilfish 4%, Dondozo 1% |
| Route124 | Surf | 6-39 | Lotad 60%, Tympole 30%, Slowpoke 5%, Popplio 4%, Azumarill 1% |
| Route124 | Pesca | 6-48 | Finizen 70%, Wimpod 30%, Shellder 60%, Goldeen 20%, Chinchou 20%, Corsola 40%, Basculin 40%, Omanyte 15%, Relicanth 4%, Gyarados 1% |
| PetalburgWoods | Erba/terreno | 6-7 | Burmy 31%, Gulpin 30%, Snubbull 10%, Joltik 10%, Rellor 5%, Milcery 5%, Ledyba 4%, Pineco 4%, Flabébé 1% |
| RusturfTunnel | Erba/terreno | 6-9 | Pikipek 30%, Skwovet 30%, Rattata 10%, Helioptile 10%, Smoliv 5%, Mankey 5%, Makuhita 4%, Geodude 4%, Skitty 2% |
| GraniteCave_1F | Erba/terreno | 7-11 | Houndour 30%, Zorua 30%, Honedge 10%, Impidimp 10%, Meowth (Alola) 5%, Diglett 5%, Cufant 4%, Zigzagoon (Galar) 4%, Meowth (Galar) 2% |
| GraniteCave_B1F | Erba/terreno | 10-12 | Nickit 30%, Sandshrew (Alola) 30%, Clobbopus 10%, Magnemite 10%, Purrloin 5%, Timburr 5%, Inkay 4%, Swinub 4%, Bronzor 2% |
| MtPyre_1F | Erba/terreno | 22-29 | Espurr 30%, Maschiff 30%, Mr. Mime (Galar) 10%, Corsola (Galar) 10%, Pawniard 5%, Drowzee 5%, Mimikyu 4%, Sneasel 4%, Ponyta (Galar) 2% |
| VictoryRoad_1F | Erba/terreno | 39-42 | Sneasel (Hisui) 30%, Stunfisk (Galar) 30%, Klawf 10%, Aerodactyl 10%, Hawlucha 5%, Solrock 5%, Minior 4%, Heracross 4%, Morpeko 2% |
| SafariZone_South | Erba/terreno | 25-29 | Growlithe 30%, Rhyhorn 30%, Larvesta 10%, Tyrunt 10%, Archen 5%, Lickitung 5%, Ponyta 4%, Magmar 4%, Togetic 2% |
| Underwater_Route126 | Surf | 20-39 | Clamperl 60%, Spheal 30%, Dracovish 5%, Cramorant 4%, Piplup 1% |
| AbandonedShip_Rooms_B1F | Surf | 6-39 | Wooper 60%, Tympole 30%, Horsea 5%, Feebas 4%, Sobble 1% |
| AbandonedShip_Rooms_B1F | Pesca | 6-39 | Wishiwashi 70%, Arrokuda 30%, Finneon 60%, Carvanha 20%, Krabby 20%, Omanyte 40%, Bruxish 40%, Veluza 15%, Kabuto 4%, Tirtouga 1% |
| GraniteCave_B2F | Erba/terreno | 11-13 | Rolycoly 31%, Poochyena 30%, Wooper (Paldea) 10%, Impidimp 10%, Tinkatink 5%, Yamper 5%, Pansear 4%, Roggenrola 4%, Gimmighoul 1% |
| GraniteCave_B2F | Spaccaroccia | 6-20 | Geodude (Alola) 60%, Hippopotas 30%, Golett 5%, Phanpy 4%, Nacli 1% |
| FieryPath | Erba/terreno | 14-16 | Darumaka 30%, Nidoran♀ 30%, Koffing 10%, Nidoran♂ 10%, Litleo 5%, Qwilfish (Hisui) 5%, Ekans 4%, Lileep 4%, Vulpix 2% |
| MeteorFalls_B1F_2R | Erba/terreno | 25-42 | Stonjourner 30%, Lunatone 30%, Mawile 10%, Drampa 10%, Girafarig 5%, Durant 5%, Klefki 4%, Oranguru 4%, Duraludon 2% |
| MeteorFalls_B1F_2R | Surf | 6-39 | Bibarel 60%, Cramorant 30%, Pyukumuku 5%, Skrelp 4%, Mareanie 1% |
| MeteorFalls_B1F_2R | Pesca | 6-48 | Corphish 70%, Barboach 30%, Clauncher 60%, Krabby 20%, Luvdisc 20%, Tentacool 40%, Alomomola 40%, Wailmer 15%, Basculegion 4%, Tatsugiri 1% |
| JaggedPass | Erba/terreno | 20-22 | Shieldon 30%, Cranidos 30%, Roselia 10%, Mudbray 10%, Torkoal 5%, Seviper 5%, Litten 4%, Togedemaru 4%, Sudowoodo 2% |
| Route106 | Surf | 6-39 | Buizel 60%, Dewpider 30%, Shellos 5%, Dewott 4%, Quagsire 1% |
| Route106 | Pesca | 6-48 | Arrokuda 70%, Wiglett 30%, Goldeen 60%, Remoraid 20%, Shellder 20%, Corsola 40%, Qwilfish 40%, Staryu 15%, Relicanth 4%, Gyarados 1% |
| Route107 | Surf | 6-39 | Marill 60%, Panpour 30%, Psyduck 5%, Drizzile 4%, Pyukumuku 1% |
| Route107 | Pesca | 6-48 | Carvanha 70%, Wimpod 30%, Finneon 60%, Corphish 20%, Binacle 20%, Wailmer 40%, Bruxish 40%, Kabuto 15%, Dondozo 4%, Alomomola 1% |
| Route108 | Surf | 6-39 | Surskit 60%, Seel 30%, Horsea 5%, Croconaw 4%, Bibarel 1% |
| Route108 | Pesca | 6-48 | Wishiwashi 70%, Finizen 30%, Chinchou 60%, Clauncher 20%, Luvdisc 20%, Omanyte 40%, Tatsugiri 40%, Tentacool 15%, Basculegion 4%, Carracosta 1% |
| Route109 | Surf | 6-39 | Poliwag 60%, Ducklett 30%, Spheal 5%, Wartortle 4%, Frillish 1% |
| Route109 | Pesca | 6-48 | Shellder 70%, Arrokuda 30%, Krabby 60%, Goldeen 20%, Barboach 20%, Corsola 40%, Qwilfish 40%, Staryu 15%, Veluza 4%, Relicanth 1% |
| Route115 | Erba/terreno | 23-26 | Scraggy 30%, Rufflet 30%, Emolga 10%, Farfetch'd (Galar) 10%, Squawkabilly 5%, Vullaby 5%, Crabrawler 4%, Aipom 4%, Stantler 2% |
| Route115 | Surf | 6-39 | Wingull 60%, Chewtle 30%, Slowpoke 5%, Frogadier 4%, Lombre 1% |
| Route115 | Pesca | 6-48 | Wimpod 70%, Remoraid 30%, Chinchou 60%, Clauncher 20%, Finneon 20%, Omanyte 40%, Bruxish 40%, Finizen 15%, Dondozo 4%, Gyarados 1% |
| NewMauville_Inside | Erba/terreno | 22-26 | Plusle 30%, Minun 30%, Pikachu 10%, Blitzle 10%, Orthworm 5%, Dedenne 5%, Skarmory 4%, Electrike 4%, Stunfisk 2% |
| Route119 | Erba/terreno | 24-27 | Volbeat 30%, Pumpkaboo 30%, Skiddo 10%, Illumise 10%, Carnivine 5%, Cacnea 5%, Tropius 4%, Maractus 4%, Tangela 2% |
| Route119 | Surf | 6-39 | Skrelp 60%, Spheal 30%, Panpour 5%, Quaxwell 4%, Pyukumuku 1% |
| Route119 | Pesca | 6-48 | Wishiwashi 70%, Binacle 30%, Corphish 60%, Carvanha 20%, Wiglett 20%, Kabuto 40%, Veluza 40%, Tirtouga 15%, Basculegion 4%, Wailord 1% |
| Route120 | Erba/terreno | 25-27 | Deerling 30%, Swirlix 30%, Yanma 10%, Clefairy 10%, Spiritomb 5%, Comfey 5%, Absol 4%, Pinsir 4%, Spritzee 2% |
| Route120 | Surf | 6-39 | Shellos 60%, Surskit 30%, Dracovish 5%, Buizel 4%, Marshtomp 1% |
| Route120 | Pesca | 6-48 | Goldeen 70%, Arrokuda 30%, Finneon 60%, Remoraid 20%, Shellder 20%, Corsola 40%, Alomomola 40%, Staryu 15%, Dondozo 4%, Qwilfish 1% |
| Route121 | Erba/terreno | 25-28 | Sableye 30%, Audino 30%, Kecleon 10%, Chansey 10%, Zangoose 5%, Indeedee 5%, Komala 4%, Bombirdier 4%, Misdreavus 2% |
| Route121 | Surf | 6-39 | Marill 60%, Chewtle 30%, Mareanie 5%, Prinplup 4%, Araquanid 1% |
| Route121 | Pesca | 6-48 | Barboach 70%, Wishiwashi 30%, Binacle 60%, Krabby 20%, Finizen 20%, Tentacool 40%, Tatsugiri 40%, Carvanha 15%, Lanturn 4%, Relicanth 1% |
| Route122 | Surf | 6-39 | Wingull 60%, Tympole 30%, Slowpoke 5%, Brionne 4%, Lombre 1% |
| Route122 | Pesca | 6-48 | Wiglett 70%, Wimpod 30%, Luvdisc 60%, Clauncher 20%, Corphish 20%, Qwilfish 40%, Bruxish 40%, Corsola 15%, Veluza 4%, Gyarados 1% |
| Route123 | Erba/terreno | 25-28 | Drifloon 30%, Wobbuffet 30%, Sinistea 10%, Cutiefly 10%, Carbink 5%, Rowlet 5%, Electabuzz 4%, Grubbin 4%, Shroomish 2% |
| Route123 | Surf | 6-39 | Ducklett 60%, Seel 30%, Horsea 5%, Marshtomp 4%, Quagsire 1% |
| Route123 | Pesca | 6-48 | Shellder 70%, Arrokuda 30%, Finizen 60%, Goldeen 20%, Carvanha 20%, Staryu 40%, Alomomola 40%, Tentacool 15%, Tatsugiri 4%, Kabutops 1% |
| MtPyre_2F | Erba/terreno | 22-29 | Jynx 30%, Spoink 30%, Chimecho 10%, Unown 10%, Woobat 5%, Elgyem 5%, Sigilyph 4%, Shuppet 4%, Slowpoke (Galar) 2% |
| MtPyre_3F | Erba/terreno | 22-29 | Munna 30%, Yamask 30%, Abra 10%, Onix 10%, Duskull 5%, Gothita 5%, Falinks 4%, Hatenna 4%, Porygon 2% |
| MtPyre_4F | Erba/terreno | 22-29 | Greavard 30%, Solosis 30%, Litwick 10%, Mienfoo 10%, Eiscue 5%, Flittle 5%, Ralts 4%, Passimian 4%, Misdreavus 2% |
| MtPyre_5F | Erba/terreno | 22-29 | Scraggy 30%, Vullaby 30%, Linoone (Galar) 10%, Mightyena 10%, Hitmonlee 5%, Stunky 5%, Thievul 4%, Sawk 4%, Chatot 2% |
| MtPyre_6F | Erba/terreno | 22-29 | Corsola (Galar) 30%, Maschiff 30%, Sneasel 10%, Espurr 10%, Mr. Mime (Galar) 5%, Honedge 5%, Flamigo 4%, Kangaskhan 4%, Throh 2% |
| MtPyre_Exterior | Erba/terreno | 25-29 | Morelull 30%, Delibird 30%, Charcadet 10%, Slugma 10%, Turtonator 5%, Fennekin 5%, Heatmor 4%, Oricorio 4%, Bouffalant 2% |
| MtPyre_Summit | Erba/terreno | 24-31 | Sableye 30%, Growlithe (Hisui) 30%, Jynx 10%, Togetic 10%, Charmander 5%, Xatu 5%, Oranguru 4%, Dhelmise 4%, Miltank 2% |
| GraniteCave_StevensRoom | Erba/terreno | 7-11 | Cubone 30%, Mareep 30%, Meditite 10%, Meowth (Galar) 10%, Yamask (Galar) 5%, Geodude 5%, Baltoy 4%, Bergmite 4%, Snorunt 2% |
| Route125 | Surf | 6-39 | Poliwag 60%, Psyduck 30%, Shellos 5%, Drizzile 4%, Bibarel 1% |
| Route125 | Pesca | 6-48 | Binacle 70%, Corphish 30%, Remoraid 60%, Clauncher 20%, Luvdisc 20%, Wailmer 40%, Basculin 40%, Omanyte 15%, Dondozo 4%, Carracosta 1% |
| Route126 | Surf | 6-39 | Surskit 60%, Ducklett 30%, Mareanie 5%, Quaxwell 4%, Clamperl 1% |
| Route126 | Pesca | 6-48 | Wimpod 70%, Wishiwashi 30%, Finneon 60%, Krabby 20%, Chinchou 20%, Corsola 40%, Qwilfish 40%, Kabuto 15%, Relicanth 4%, Gyarados 1% |
| Route127 | Surf | 6-39 | Panpour 60%, Buizel 30%, Horsea 5%, Frogadier 4%, Frillish 1% |
| Route127 | Pesca | 6-48 | Barboach 70%, Wiglett 30%, Carvanha 60%, Shellder 20%, Arrokuda 20%, Veluza 40%, Bruxish 40%, Tentacool 15%, Starmie 4%, Tatsugiri 1% |
| Route128 | Surf | 6-39 | Tympole 60%, Poliwag 30%, Chewtle 5%, Wartortle 4%, Bibarel 1% |
| Route128 | Pesca | 6-48 | Binacle 70%, Corphish 30%, Finizen 60%, Goldeen 20%, Luvdisc 20%, Wailmer 40%, Alomomola 40%, Basculin 15%, Dondozo 4%, Carracosta 1% |
| Route129 | Surf | 6-39 | Psyduck 60%, Slowpoke 30%, Seel 5%, Brionne 4%, Azumarill 1% |
| Route129 | Pesca | 6-48 | Remoraid 70%, Wimpod 30%, Clauncher 60%, Chinchou 20%, Krabby 20%, Corsola 40%, Omanyte 40%, Kabuto 15%, Relicanth 4%, Gyarados 1% |
| Route130 *(non raggiungibile)* | Erba/terreno | 6-53 | Dottler 30%, Furfrou 30%, Pidgeotto 10%, Oinkologne 10%, Patrat 5%, Unfezant 5%, Swablu 4%, Smeargle 4%, Rattata (Alola) 2% |
| Route130 *(non raggiungibile)* | Surf | 6-39 | Skrelp 60%, Spheal 30%, Feebas 5%, Dewott 4%, Clamperl 1% |
| Route130 *(non raggiungibile)* | Pesca | 6-48 | Wiglett 70%, Wishiwashi 30%, Barboach 60%, Finneon 20%, Goldeen 20%, Tatsugiri 40%, Veluza 40%, Staryu 15%, Bruxish 4%, Qwilfish 1% |
| Route131 | Surf | 6-39 | Wingull 60%, Dewpider 30%, Psyduck 5%, Croconaw 4%, Frillish 1% |
| Route131 | Pesca | 6-48 | Arrokuda 70%, Corphish 30%, Shellder 60%, Finizen 20%, Luvdisc 20%, Basculin 40%, Alomomola 40%, Omanyte 15%, Relicanth 4%, Wailord 1% |
| Route132 | Surf | 6-39 | Wooper 60%, Poliwag 30%, Chewtle 5%, Prinplup 4%, Pyukumuku 1% |
| Route132 | Pesca | 6-48 | Carvanha 70%, Remoraid 30%, Chinchou 60%, Binacle 20%, Krabby 20%, Tirtouga 40%, Corsola 40%, Tentacool 15%, Dondozo 4%, Gyarados 1% |
| Route133 | Surf | 6-39 | Lotad 60%, Slowpoke 30%, Horsea 5%, Brionne 4%, Bibarel 1% |
| Route133 | Pesca | 6-48 | Wishiwashi 70%, Barboach 30%, Clauncher 60%, Finneon 20%, Wiglett 20%, Qwilfish 40%, Bruxish 40%, Kabuto 15%, Starmie 4%, Tatsugiri 1% |
| Route134 | Surf | 6-39 | Ducklett 60%, Panpour 30%, Shellos 5%, Wartortle 4%, Araquanid 1% |
| Route134 | Pesca | 6-48 | Wimpod 70%, Goldeen 30%, Luvdisc 60%, Arrokuda 20%, Finizen 20%, Veluza 40%, Basculin 40%, Wailmer 15%, Alomomola 4%, Relicanth 1% |
| AbandonedShip_HiddenFloorCorridors | Surf | 6-39 | Marill 60%, Buizel 30%, Feebas 5%, Spheal 4%, Mantine 1% |
| AbandonedShip_HiddenFloorCorridors | Pesca | 6-39 | Carvanha 70%, Corphish 30%, Chinchou 60%, Binacle 20%, Remoraid 20%, Tentacool 40%, Omanyte 40%, Tirtouga 15%, Corsola 4%, Shellder 1% |
| SeafloorCavern_Room1 | Erba/terreno | 28-39 | Spinarak 30%, Cetoddle 30%, Beedrill 10%, Tauros 10%, Toxtricity 5%, Lapras 5%, Shuckle 4%, Deino 4%, Dracozolt 2% |
| SeafloorCavern_Room2 | Erba/terreno | 28-39 | Zubat 30%, Nidorina 30%, Lokix 10%, Dustox 10%, Lucario 5%, Venusaur 5%, Pupitar 4%, Cryogonal 4%, Arctozolt 2% |
| SeafloorCavern_Room3 | Erba/terreno | 28-39 | Lunatone 30%, Darumaka (Galar) 30%, Salazzle 10%, Solrock 10%, Meowscarada 5%, Venomoth 5%, Scolipede 4%, Druddigon 4%, Cyclizar 2% |
| SeafloorCavern_Room4 | Erba/terreno | 28-39 | Sneasel (Hisui) 30%, Nidorino 30%, Archen 10%, Furfrou 10%, Houndoom 5%, Toxicroak 5%, Kleavor 4%, Axew 4%, Goomy 2% |
| SeafloorCavern_Room5 | Erba/terreno | 28-39 | Anorith 30%, Cubchoo 30%, Seviper 10%, Grafaiai 10%, Overqwil 5%, Stonjourner 5%, Tynamo 4%, Zoroark 4%, Dreepy 2% |
| SeafloorCavern_Room6 | Erba/terreno | 28-39 | Weedle 30%, Onix 30%, Klawf 10%, Cacturne 10%, Bastiodon 5%, Noibat 5%, Smeargle 4%, Lapras 4%, Swalot 2% |
| SeafloorCavern_Room6 | Surf | 6-39 | Tympole 60%, Wingull 30%, Arctovish 5%, Tauros (Paldea) 4%, Cramorant 1% |
| SeafloorCavern_Room6 | Pesca | 6-48 | Barboach 70%, Wishiwashi 30%, Finneon 60%, Clauncher 20%, Wiglett 20%, Kabuto 40%, Kingler 40%, Tentacool 15%, Dondozo 4%, Gyarados 1% |
| SeafloorCavern_Room7 | Erba/terreno | 28-39 | Tyrunt 30%, Ariados 30%, Arbok 10%, Lairon 10%, Umbreon 5%, Absol 5%, Persian (Alola) 4%, Probopass 4%, Malamar 2% |
| SeafloorCavern_Room7 | Surf | 6-39 | Skrelp 60%, Surskit 30%, Dracovish 5%, Mantine 4%, Drizzile 1% |
| SeafloorCavern_Room7 | Pesca | 6-48 | Wimpod 70%, Carvanha 30%, Shellder 60%, Luvdisc 20%, Arrokuda 20%, Staryu 40%, Basculin 40%, Omanyte 15%, Relicanth 4%, Carracosta 1% |
| SeafloorCavern_Room8 | Erba/terreno | 28-39 | Gloom 30%, Swablu 30%, Clodsire 10%, Liepard 10%, Rampardos 5%, Lycanroc 5%, Weezing (Galar) 4%, Slowbro (Galar) 4%, Pangoro 2% |
| SeafloorCavern_Entrance | Surf | 6-39 | Seel 60%, Lotad 30%, Tauros (Paldea) 5%, Arctovish 4%, Frogadier 1% |
| SeafloorCavern_Entrance | Pesca | 6-48 | Wiglett 70%, Binacle 30%, Corphish 60%, Chinchou 20%, Remoraid 20%, Wailmer 40%, Bruxish 40%, Clauncher 15%, Veluza 4%, Qwilfish 1% |
| CaveOfOrigin_Entrance | Erba/terreno | 28-39 | Weepinbell 30%, Vanillite 30%, Kadabra 10%, Sudowoodo 10%, Skeledirge 5%, Metang 5%, Gengar 4%, Armarouge 4%, Roserade 2% |
| CaveOfOrigin_1F | Erba/terreno | 31-39 | Minior 30%, Rotom 30%, Murkrow 10%, Wattrel 10%, Zoroark (Hisui) 5%, Typhlosion (Hisui) 5%, Swoobat 4%, Bronzong 4%, Sinistcha 2% |
| CaveOfOrigin_UnusedRubySapphireMap1 *(non raggiungibile)* | Erba/terreno | 31-39 | Chimecho 30%, Golbat 30%, Girafarig 10%, Indeedee 10%, Espathra 5%, Meowscarada 5%, Wyrdeer 4%, Drifblim 4%, Aerodactyl 2% |
| CaveOfOrigin_UnusedRubySapphireMap2 *(non raggiungibile)* | Erba/terreno | 31-39 | Wobbuffet 30%, Carkol 30%, Grumpig 10%, Lokix 10%, Houndstone 5%, Brambleghast 5%, Gourgeist 4%, Crustle 4%, Incineroar 2% |
| CaveOfOrigin_UnusedRubySapphireMap3 *(non raggiungibile)* | Erba/terreno | 31-39 | Morpeko 30%, Vivillon 30%, Mimikyu 10%, Ponyta (Galar) 10%, Spiritomb 5%, Orbeetle 5%, Garbodor 4%, Polteageist 4%, Glimmora 2% |
| NewMauville_Entrance | Erba/terreno | 22-26 | Shinx 30%, Tadbulb 30%, Pawmi 10%, Vulpix (Alola) 10%, Orthworm 5%, Stunfisk 5%, Plusle 4%, Chimchar 4%, Wormadam 2% |
| SafariZone_Southwest | Erba/terreno | 25-29 | Applin 30%, Hoppip 30%, Gossifleur 10%, Combee 10%, Treecko 5%, Pidgey 5%, Pidove 4%, Lechonk 4%, Bounsweet 2% |
| SafariZone_Southwest | Surf | 20-39 | Clamperl 60%, Mareanie 30%, Marshtomp 5%, Dracovish 4%, Mantine 1% |
| SafariZone_Southwest | Pesca | 6-42 | Finizen 70%, Wimpod 30%, Goldeen 60%, Barboach 20%, Krabby 20%, Wailmer 40%, Tatsugiri 40%, Alomomola 15%, Dondozo 4%, Omanyte 1% |
| SafariZone_North | Erba/terreno | 27-33 | Tarountula 30%, Patrat 30%, Snom 10%, Caterpie 10%, Turtwig 5%, Kricketot 5%, Sunkern 4%, Scatterbug 4%, Wurmple 2% |
| SafariZone_North | Spaccaroccia | 6-31 | Trapinch 60%, Sandile 30%, Drilbur 5%, Lileep 4%, Amaura 1% |
| SafariZone_Northwest | Erba/terreno | 27-33 | Nymble 30%, Blipbug 30%, Chansey 10%, Floette 10%, Chikorita 5%, Lickitung 5%, Farfetch'd (Galar) 4%, Magmar 4%, Gurdurr 2% |
| SafariZone_Northwest | Surf | 20-42 | Frillish 60%, Psyduck 30%, Prinplup 5%, Tauros (Paldea) 4%, Pyukumuku 1% |
| SafariZone_Northwest | Pesca | 6-42 | Wimpod 70%, Barboach 30%, Finneon 60%, Shellder 20%, Arrokuda 20%, Tirtouga 40%, Alomomola 40%, Basculin 15%, Dondozo 4%, Staryu 1% |
| VictoryRoad_B1F | Erba/terreno | 41-44 | Dugtrio (Alola) 30%, Togedemaru 30%, Magneton 10%, Klang 10%, Hakamo-o 5%, Snorlax 5%, Chesnaught 4%, Dugtrio 4%, Falinks 2% |
| VictoryRoad_B1F | Spaccaroccia | 31-42 | Sandslash 60%, Stunfisk (Galar) 30%, Gabite 5%, Mudsdale 4%, Gliscor 1% |
| VictoryRoad_B2F | Erba/terreno | 42-47 | Ferrothorn 30%, Magcargo 30%, Breloom 10%, Skarmory 10%, Dragonair 5%, Blaziken 5%, Shiftry 4%, Corviknight 4%, Hariyama 2% |
| VictoryRoad_B2F | Surf | 25-42 | Cramorant 60%, Quagsire 30%, Lapras 5%, Vaporeon 4%, Feraligatr 1% |
| VictoryRoad_B2F | Pesca | 6-48 | Wishiwashi 70%, Wiglett 30%, Clauncher 60%, Carvanha 20%, Chinchou 20%, Corsola 40%, Qwilfish 40%, Tatsugiri 15%, Veluza 4%, Kabutops 1% |
| MeteorFalls_1F_1R | Erba/terreno | 14-20 | Exeggcute 30%, Solosis 30%, Rhyhorn 10%, Varoom 10%, Elgyem 5%, Cufant 5%, Pawniard 4%, Grookey 4%, Unown 2% |
| MeteorFalls_1F_1R | Surf | 6-39 | Mareanie 60%, Araquanid 30%, Dewott 5%, Poliwag 4%, Feebas 1% |
| MeteorFalls_1F_1R | Pesca | 6-48 | Corphish 70%, Remoraid 30%, Finizen 60%, Binacle 20%, Finneon 20%, Tentacool 40%, Bruxish 40%, Goldeen 15%, Relicanth 4%, Gyarados 1% |
| MeteorFalls_1F_2R | Erba/terreno | 37-42 | Drampa 30%, Klefki 30%, Durant 10%, Turtonator 10%, Arctibax 5%, Gallade 5%, Rabsca 4%, Shuckle 4%, Carbink 2% |
| MeteorFalls_1F_2R | Surf | 6-39 | Cramorant 60%, Pyukumuku 30%, Quaxwell 5%, Horsea 4%, Marill 1% |
| MeteorFalls_1F_2R | Pesca | 6-48 | Wishiwashi 70%, Wimpod 30%, Luvdisc 60%, Krabby 20%, Barboach 20%, Corsola 40%, Wailmer 40%, Veluza 15%, Alomomola 4%, Tatsugiri 1% |
| MeteorFalls_B1F_1R | Erba/terreno | 37-42 | Hypno 30%, Forretress 30%, Druddigon 10%, Sigilyph 10%, Shelgon 5%, Musharna 5%, Cyclizar 4%, Avalugg (Hisui) 4%, Sandslash (Alola) 2% |
| MeteorFalls_B1F_1R | Surf | 6-39 | Bibarel 60%, Palpitoad 30%, Brionne 5%, Buizel 4%, Panpour 1% |
| MeteorFalls_B1F_1R | Pesca | 6-48 | Goldeen 70%, Shellder 30%, Binacle 60%, Corphish 20%, Arrokuda 20%, Omanyte 40%, Basculin 40%, Bruxish 15%, Relicanth 4%, Gyarados 1% |
| ShoalCave_LowTideStairsRoom | Erba/terreno | 26-35 | Cetoddle 30%, Snover 30%, Sneasel 10%, Amaura 10%, Frigibax 5%, Froslass 5%, Eiscue 4%, Crabominable 4%, Frosmoth 2% |
| ShoalCave_LowTideLowerRoom | Erba/terreno | 26-35 | Delibird 30%, Buneary 30%, Jynx 10%, Spinda 10%, Jangmo-o 5%, Cryogonal 5%, Darmanitan (Galar) 4%, Ninetales (Alola) 4%, Arctozolt 2% |
| ShoalCave_LowTideInnerRoom | Erba/terreno | 26-35 | Vanillite 30%, Mawile 30%, Cubchoo 10%, Sandshrew (Alola) 10%, Piloswine 5%, Servine 5%, Mr. Mime (Galar) 4%, Vigoroth 4%, Arctovish 2% |
| ShoalCave_LowTideInnerRoom | Surf | 6-39 | Chewtle 60%, Shellos 30%, Frogadier 5%, Quagsire 4%, Clamperl 1% |
| ShoalCave_LowTideInnerRoom | Pesca | 6-48 | Wishiwashi 70%, Carvanha 30%, Krabby 60%, Finizen 20%, Remoraid 20%, Tirtouga 40%, Qwilfish 40%, Staryu 15%, Dondozo 4%, Tentacruel 1% |
| ShoalCave_LowTideEntranceRoom | Erba/terreno | 26-35 | Snover 30%, Fletchinder 30%, Cetoddle 10%, Sneasel 10%, Frosmoth 5%, Froslass 5%, Raboot 4%, Eiscue 4%, Crabominable 2% |
| ShoalCave_LowTideEntranceRoom | Surf | 6-39 | Surskit 60%, Slowpoke 30%, Prinplup 5%, Seel 4%, Frillish 1% |
| ShoalCave_LowTideEntranceRoom | Pesca | 6-48 | Wiglett 70%, Barboach 30%, Finneon 60%, Clauncher 20%, Chinchou 20%, Corsola 40%, Kabuto 40%, Wailmer 15%, Basculegion 4%, Gyarados 1% |
| LilycoveCity | Surf | 6-39 | Lotad 60%, Wingull 30%, Ducklett 5%, Drizzile 4%, Skrelp 1% |
| LilycoveCity | Pesca | 6-48 | Arrokuda 70%, Wimpod 30%, Luvdisc 60%, Corphish 20%, Carvanha 20%, Tentacool 40%, Qwilfish 40%, Staryu 15%, Tatsugiri 4%, Carracosta 1% |
| DewfordTown | Surf | 6-39 | Spheal 60%, Buizel 30%, Chewtle 5%, Wartortle 4%, Quagsire 1% |
| DewfordTown | Pesca | 6-48 | Remoraid 70%, Binacle 30%, Shellder 60%, Finizen 20%, Goldeen 20%, Omanyte 40%, Veluza 40%, Krabby 15%, Relicanth 4%, Dondozo 1% |
| SlateportCity | Surf | 6-39 | Slowpoke 60%, Shellos 30%, Tympole 5%, Dewott 4%, Clamperl 1% |
| SlateportCity | Pesca | 6-48 | Wiglett 70%, Wishiwashi 30%, Clauncher 60%, Chinchou 20%, Finneon 20%, Kabuto 40%, Bruxish 40%, Wailmer 15%, Alomomola 4%, Gyarados 1% |
| MossdeepCity | Surf | 6-39 | Skrelp 60%, Panpour 30%, Horsea 5%, Marshtomp 4%, Azumarill 1% |
| MossdeepCity | Pesca | 6-48 | Corphish 70%, Wimpod 30%, Luvdisc 60%, Arrokuda 20%, Carvanha 20%, Qwilfish 40%, Basculin 40%, Staryu 15%, Tatsugiri 4%, Carracosta 1% |
| PacifidlogTown | Surf | 6-39 | Ducklett 60%, Psyduck 30%, Spheal 5%, Quaxwell 4%, Lombre 1% |
| PacifidlogTown | Pesca | 6-48 | Binacle 70%, Finizen 30%, Remoraid 60%, Barboach 20%, Goldeen 20%, Corsola 40%, Veluza 40%, Omanyte 15%, Tentacruel 4%, Relicanth 1% |
| EverGrandeCity | Surf | 6-39 | Wingull 60%, Surskit 30%, Mareanie 5%, Croconaw 4%, Pyukumuku 1% |
| EverGrandeCity | Pesca | 6-48 | Wishiwashi 70%, Shellder 30%, Finneon 60%, Krabby 20%, Clauncher 20%, Alomomola 40%, Bruxish 40%, Wailmer 15%, Dondozo 4%, Gyarados 1% |
| PetalburgCity | Surf | 6-39 | Seel 60%, Dewpider 30%, Mantine 5%, Poliwag 4%, Feebas 1% |
| PetalburgCity | Pesca | 6-48 | Wiglett 70%, Corphish 30%, Luvdisc 60%, Chinchou 20%, Carvanha 20%, Kabuto 40%, Basculin 40%, Tirtouga 15%, Tatsugiri 4%, Starmie 1% |
| Underwater_Route124 | Surf | 20-39 | Frillish 60%, Shellos 30%, Arctovish 5%, Tauros (Paldea) 4%, Dracovish 1% |
| ShoalCave_LowTideIceRoom | Erba/terreno | 26-35 | Darumaka (Galar) 30%, Skiddo 30%, Delibird 10%, Furret 10%, Tepig 5%, Arctozolt 5%, Cryogonal 4%, Cubchoo 4%, Ninetales (Alola) 2% |
| SkyPillar_1F | Erba/terreno | 37-41 | Squawkabilly 30%, Jumpluff 30%, Tropius 10%, Toucannon 10%, Bombirdier 5%, Emolga 5%, Ninjask 4%, Dodrio 4%, Yanmega 2% |
| SootopolisCity | Surf | 6-39 | Mareanie 60%, Skrelp 30%, Panpour 5%, Quaxwell 4%, Bibarel 1% |
| SootopolisCity | Pesca | 6-48 | Finizen 70%, Wimpod 30%, Finneon 60%, Luvdisc 20%, Barboach 20%, Corsola 40%, Omanyte 40%, Relicanth 15%, Bruxish 4%, Krabby 1% |
| SkyPillar_3F | Erba/terreno | 37-41 | Noctowl 30%, Swellow 30%, Flamigo 10%, Fearow 10%, Duraludon 5%, Oricorio 5%, Raichu (Alola) 4%, Hawlucha 4%, Appletun 2% |
| SkyPillar_5F | Erba/terreno | 37-65 | Chatot 30%, Kilowattrel 30%, Staraptor 10%, Vespiquen 10%, Espathra 10%, Charizard 4%, Dracozolt 4%, Dragonair 1%, Cresselia 1% |
| SafariZone_Southeast | Erba/terreno | 37-65 | Ursaring 30%, Dunsparce 30%, Bewear 10%, Zebstrika 10%, Electabuzz 10%, Infernape 4%, Vigoroth 4%, Tangrowth 1%, Ting-Lu 1% |
| SafariZone_Southeast | Surf | 25-42 | Quagsire 60%, Azumarill 30%, Drizzile 5%, Mantine 4%, Lapras 1% |
| SafariZone_Southeast | Pesca | 25-42 | Magikarp 70%, Shellder 30%, Tentacool 60%, Kabuto 20%, Qwilfish 20%, Omanyte 40%, Staryu 40%, Alomomola 15%, Veluza 4%, Dondozo 1% |
| SafariZone_Northeast | Erba/terreno | 37-65 | Machoke 30%, Primeape 30%, Kangaskhan 10%, Comfey 10%, Lurantis 10%, Shelgon 4%, Blaziken 4%, Sandaconda 1%, Uxie 1% |
| SafariZone_Northeast | Spaccaroccia | 20-42 | Toedscool 60%, Numel 30%, Larvitar 5%, Archen 4%, Torterra 1% |
| MagmaHideout_1F | Erba/terreno | 27-37 | Larvesta 30%, Ponyta 30%, Litleo 10%, Torkoal 10%, Cyndaquil 5%, Beldum 5%, Diggersby 4%, Simisear 4%, Scovillain 2% |
| MagmaHideout_2F_1R | Erba/terreno | 27-37 | Growlithe 30%, Roselia 30%, Mawile 10%, Gligar 10%, Bulbasaur 5%, Gible 5%, Toxtricity 4%, Escavalier 4%, Ninetales 2% |
| MagmaHideout_2F_2R | Erba/terreno | 27-37 | Grimer 30%, Dustox 30%, Nidorina 10%, Sneasel (Hisui) 10%, Fuecoco 5%, Heatmor 5%, Sudowoodo 4%, Dratini 4%, Golbat 2% |
| MagmaHideout_3F_1R | Erba/terreno | 27-37 | Grimer (Alola) 30%, Nidorino 30%, Skorupi 10%, Qwilfish (Hisui) 10%, Scorbunny 5%, Sandslash 5%, Shuckle 4%, Slowking (Galar) 4%, Klawf 2% |
| MagmaHideout_3F_2R | Erba/terreno | 27-37 | Naclstack 30%, Gloom 30%, Nosepass 10%, Stunfisk 10%, Torchic 5%, Slakoth 5%, Lycanroc 4%, Bastiodon 4%, Wormadam 2% |
| MagmaHideout_4F | Erba/terreno | 27-37 | Fletchinder 30%, Mudbray 30%, Weepinbell 10%, Onix 10%, Braixen 5%, Gengar 5%, Oricorio 4%, Rampardos 4%, Excadrill 2% |
| MagmaHideout_3F_3R | Erba/terreno | 27-37 | Lileep 30%, Ponyta 30%, Lunatone 10%, Carkol 10%, Pignite 5%, Bagon 5%, Turtonator 4%, Golem 4%, Stunfisk (Galar) 2% |
| MagmaHideout_2F_3R | Erba/terreno | 27-37 | Tyrunt 30%, Farfetch'd 30%, Togedemaru 10%, Klefki 10%, Seviper 5%, Chespin 5%, Gigalith 4%, Grafaiai 4%, Carbink 2% |
| MirageTower_1F | Erba/terreno | 20-24 | Sizzlipede 30%, Sandygast 30%, Darumaka 10%, Salandit 10%, Snivy 5%, Litleo 5%, Phanpy 4%, Stonjourner 4%, Glimmet 2% |
| MirageTower_2F | Erba/terreno | 20-24 | Pansear 30%, Cubone 30%, Toedscool 10%, Houndour 10%, Sprigatito 5%, Growlithe 5%, Clodsire 4%, Loudred 4%, Growlithe (Hisui) 2% |
| MirageTower_3F | Erba/terreno | 20-24 | Silicobra 30%, Mienfoo 30%, Rhyhorn 10%, Hippopotas 10%, Spritzee 5%, Minior 5%, Monferno 4%, Solrock 4%, Larvesta 2% |
| MirageTower_4F | Erba/terreno | 20-24 | Aron 30%, Yamask (Galar) 30%, Anorith 10%, Pawmo 10%, Torracat 5%, Torkoal 5%, Diggersby 4%, Kricketune 4%, Swadloon 2% |
| DesertUnderpass | Erba/terreno | 39-65 | Galvantula 30%, Illumise 30%, Volbeat 10%, Spidops 10%, Pinsir 10%, Vikavolt 4%, Heracross 4%, Accelgor 1%, Iron Thorns 1% |
| ArtisanCave_B1F | Erba/terreno | 42-65 | Ribombee 30%, Amoonguss 30%, Parasect 10%, Butterfree 10%, Lucario 10%, Gholdengo 4%, Sliggoo (Hisui) 4%, Klinklang 1%, Heatran 1% |
| ArtisanCave_1F | Erba/terreno | 42-65 | Ledian 30%, Absol 30%, Durant 10%, Muk 10%, Arbok 10%, Corviknight 4%, Zweilous 4%, Drapion 1%, Cobalion 1% |
| AlteringCave1 | Erba/terreno | 7-65 | Baltoy 30%, Zorua 30%, Bronzor 10%, Tinkatink 10%, Sandygast 10%, Cufant 4%, Ferroseed 4%, Sizzlipede 1%, Iron Treads 1% |
| AlteringCave2 *(non raggiungibile)* | Erba/terreno | 3-65 | Grimer (Alola) 30%, Venonat 30%, Meowth (Galar) 10%, Magnemite 10%, Purrloin 10%, Diglett 4%, Sandile 4%, Golett 1%, Sandy Shocks 1% |
| AlteringCave3 *(non raggiungibile)* | Erba/terreno | 19-65 | Trubbish 30%, Nuzleaf 30%, Vullaby 10%, Murkrow 10%, Scraggy 10%, Mightyena 4%, Volbeat 4%, Dwebble 1%, Buzzwole 1% |
| AlteringCave4 *(non raggiungibile)* | Erba/terreno | 13-65 | Varoom 30%, Gulpin 30%, Cutiefly 10%, Koffing 10%, Grubbin 10%, Numel 4%, Beedrill 4%, Trapinch 1%, Okidogi 1% |
| AlteringCave5 *(non raggiungibile)* | Erba/terreno | 7-65 | Venipede 30%, Inkay 30%, Nincada 10%, Geodude (Alola) 10%, Spinarak 10%, Joltik 4%, Croagunk 4%, Pineco 1%, Fezandipiti 1% |
| AlteringCave6 *(non raggiungibile)* | Erba/terreno | 18-65 | Kricketune 30%, Yanma 30%, Ledian 10%, Spidops 10%, Maschiff 10%, Illumise 4%, Scyther 4%, Morpeko 1%, Brute Bonnet 1% |
| AlteringCave7 *(non raggiungibile)* | Erba/terreno | 18-65 | Raticate (Alola) 30%, Stunky 30%, Karrablast 10%, Vivillon 10%, Minccino 10%, Rufflet 4%, Spiritomb 4%, Pawniard 1%, Iron Jugulis 1% |
| AlteringCave8 *(non raggiungibile)* | Erba/terreno | 18-65 | Honedge 30%, Linoone (Galar) 30%, Shelmet 10%, Butterfree 10%, Foongus 10%, Pincurchin 4%, Sableye 4%, Voltorb (Hisui) 1%, Chien-Pao 1% |
| AlteringCave9 *(non raggiungibile)* | Erba/terreno | 18-65 | Dottler 30%, Swadloon 30%, Meowth (Alola) 10%, Thievul 10%, Pachirisu 10%, Aipom 4%, Ivysaur 4%, Luxio 1%, Terrakion 1% |
| MeteorFalls_StevensCave | Erba/terreno | 37-42 | Gothorita 30%, Meowstic 30%, Altaria 10%, Girafarig 10%, Rabsca 5%, Fraxure 5%, Swoobat 4%, Rapidash (Galar) 4%, Musharna 2% |

## Esito della verifica

- Errori: **0**
- Avvisi: **0**

