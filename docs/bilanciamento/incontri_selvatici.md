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
| 1 | 87/87 | 401 |
| 2 | 49/49 | 215 |
| 3 | 64/64 | 310 |
| 4 | 31/31 | 134 |
| 5 | 73/73 | 278 |
| 6 | 31/31 | 130 |
| 7 | 34/34 | 162 |
| 8 | 37/37 | 140 |
| 9 | 40/40 | 205 |

## Regole applicate

- Livelli: curva vanilla → nuovo (2→2, 5→6, 12→13, 15→15, 19→19, 24→24, 29→29, 31→33, 33→37, 42→44, 46→49, 49→52, 55→57, 58→62, 70→72, 100→100), anche per la pesca.
- Evoluzioni: forme base sempre; evoluzioni per livello solo da livello evolutivo + 2; pietra/scambio/amicizia/altro solo da Lv 30 e in slot ≤ 5%.
- Potenza: slot ≤ Lv 15 solo specie con statistiche base totali (BST) ≤ 330 e famiglie con BST finale ≤ 535 (max 1 famiglia forte in uno slot 1%); pseudo-leggendari da Lv 30 in slot 1-5%; famiglie degli starter da Lv 20 in slot 1-5% (max 1 per tabella); leggendari/misteriosi/UC/paradosso solo in aree post-game, slot 1%, Lv 60-70, max 1 per tabella.
- Habitat: ~70% degli slot terrestri con tipi dell'habitat, ~30% liberi; Surf = tipo Acqua; Pesca = pesci Acqua (gruppo uova Acqua 2/3), Amo Vecchio debole, Amo Buono medio, Super Amo forte; Spaccaroccia = Roccia/Terra.

## Tabelle per mappa

Percentuale = somma dei tassi degli slot della specie. *(non raggiungibile)* = tabella presente ma non usata nel gioco normale (esclusa dal calcolo della copertura).

| Mappa | Tipo | Livelli | Specie |
|---|---|---|---|
| Route101 | Erba/terreno | 2-3 | Bunnelby 30%, Gossifleur 30%, Nincada 10%, Lechonk 10%, Meowth 5%, Tandemaus 5%, Ledyba 4%, Fomantis 4%, Shroodle 2% |
| Route102 | Erba/terreno | 3-5 | Bounsweet 30%, Capsakid 30%, Starly 10%, Minccino 10%, Poltchageist 5%, Jigglypuff 5%, Sunkern 4%, Zubat 4%, Venipede 2% |
| Route102 | Surf | 6-39 | Skrelp 60%, Krabby 30%, Basculin 5%, Ducklett 4%, Luvdisc 1% |
| Route102 | Pesca | 6-48 | Goldeen 70%, Finizen 30%, Corphish 60%, Clauncher 20%, Shellder 20%, Tirtouga 40%, Alomomola 40%, Finneon 15%, Tatsugiri 4%, Relicanth 1% |
| Route103 | Erba/terreno | 2-5 | Pidove 30%, Seedot 30%, Whismur 10%, Skwovet 10%, Rattata 5%, Sentret 5%, Zigzagoon 4%, Wattrel 4%, Zorua (Hisui) 2% |
| Route103 | Surf | 6-39 | Dewpider 60%, Tympole 30%, Psyduck 5%, Omanyte 4%, Pyukumuku 1% |
| Route103 | Pesca | 6-48 | Wishiwashi 70%, Carvanha 30%, Arrokuda 60%, Barboach 20%, Chinchou 20%, Qwilfish 40%, Veluza 40%, Wailmer 15%, Dondozo 4%, Corsola 1% |
| Route104 | Erba/terreno | 3-6 | Bramblin 30%, Tarountula 30%, Lillipup 10%, Skitty 10%, Smeargle 5%, Bidoof 5%, Hoothoot 4%, Rookidee 4%, Phantump 2% |
| Route104 | Surf | 11-31 | Seel 60%, Mareanie 30%, Horsea 5%, Staryu 4%, Clamperl 1% |
| Route104 | Pesca | 6-48 | Finizen 70%, Wimpod 30%, Finneon 60%, Corphish 20%, Binacle 20%, Kabuto 40%, Bruxish 40%, Tentacool 15%, Magikarp 4%, Cloyster 1% |
| Route105 | Surf | 6-39 | Poliwag 60%, Wingull 30%, Buizel 5%, Popplio 4%, Frillish 1% |
| Route105 | Pesca | 6-48 | Arrokuda 70%, Wishiwashi 30%, Chinchou 60%, Remoraid 20%, Clauncher 20%, Wailmer 40%, Kingler 40%, Luvdisc 15%, Seaking 4%, Veluza 1% |
| Route110 | Erba/terreno | 13-14 | Mareep 30%, Taillow 30%, Joltik 10%, Magnemite 10%, Eevee 5%, Pikachu 5%, Nidoran♂ 4%, Grimer 4%, Tadbulb 2% |
| Route110 | Surf | 6-39 | Panpour 60%, Surskit 30%, Lotad 5%, Totodile 4%, Wooper 1% |
| Route110 | Pesca | 6-48 | Wiglett 70%, Carvanha 30%, Barboach 60%, Binacle 20%, Finizen 20%, Omanyte 40%, Corsola 40%, Tirtouga 15%, Alomomola 4%, Dondozo 1% |
| Route111 | Erba/terreno | 19-22 | Lileep 30%, Dwebble 30%, Gligar 10%, Cubone 10%, Darumaka 5%, Houndour 5%, Salandit 4%, Sandygast 4%, Glimmet 2% |
| Route111 | Surf | 6-39 | Spheal 60%, Wingull 30%, Tauros (Paldea) 5%, Chewtle 4%, Sobble 1% |
| Route111 | Spaccaroccia | 6-20 | Sandshrew 60%, Phanpy 30%, Rockruff 5%, Sandile 4%, Hippopotas 1% |
| Route111 | Pesca | 6-48 | Arrokuda 70%, Corphish 30%, Luvdisc 60%, Goldeen 20%, Shellder 20%, Kabuto 40%, Qwilfish 40%, Tentacool 15%, Basculegion 4%, Tatsugiri 1% |
| Route112 | Erba/terreno | 14-16 | Machop 30%, Golett 30%, Nacli 10%, Meditite 10%, Nosepass 5%, Larvesta 5%, Vulpix 4%, Timburr 4%, Sneasel (Hisui) 2% |
| Route113 | Erba/terreno | 14-16 | Yamask (Galar) 30%, Grimer (Alola) 30%, Skorupi 10%, Aron 10%, Bronzor 5%, Cufant 5%, Drilbur 4%, Silicobra 4%, Anorith 2% |
| Route114 | Erba/terreno | 15-18 | Petilil 30%, Ekans 30%, Sewaddle 10%, Geodude 10%, Lickitung 5%, Mudbray 5%, Pansage 4%, Ferroseed 4%, Venonat 2% |
| Route114 | Surf | 6-39 | Staryu 60%, Buizel 30%, Cramorant 5%, Dewpider 4%, Squirtle 1% |
| Route114 | Spaccaroccia | 6-20 | Diglett (Alola) 60%, Baltoy 30%, Trapinch 5%, Geodude (Alola) 4%, Diglett 1% |
| Route114 | Pesca | 6-48 | Barboach 70%, Carvanha 30%, Chinchou 60%, Krabby 20%, Clauncher 20%, Corsola 40%, Bruxish 40%, Kabuto 15%, Relicanth 4%, Gyarados 1% |
| Route116 | Erba/terreno | 7-9 | Pidgey 30%, Grubbin 30%, Fletchling 10%, Rellor 10%, Patrat 5%, Mankey 5%, Hoppip 4%, Karrablast 4%, Shroomish 2% |
| Route117 | Erba/terreno | 14-14 | Oddish 30%, Snubbull 30%, Glameow 10%, Shelmet 10%, Clefairy 5%, Cutiefly 5%, Ditto 4%, Foongus 4%, Exeggcute 2% |
| Route117 | Surf | 6-39 | Clamperl 60%, Seel 30%, Mantine 5%, Surskit 4%, Quaxly 1% |
| Route117 | Pesca | 6-48 | Wishiwashi 70%, Wimpod 30%, Remoraid 60%, Finneon 20%, Wiglett 20%, Staryu 40%, Veluza 40%, Omanyte 15%, Qwilfish 4%, Alomomola 1% |
| Route118 | Erba/terreno | 24-27 | Drifloon 30%, Stufful 30%, Pincurchin 10%, Murkrow 10%, Rotom 5%, Zangoose 5%, Buneary 4%, Tauros 4%, Rufflet 2% |
| Route118 | Surf | 6-39 | Panpour 60%, Mareanie 30%, Feebas 5%, Piplup 4%, Bibarel 1% |
| Route118 | Pesca | 6-48 | Carvanha 70%, Arrokuda 30%, Shellder 60%, Corphish 20%, Barboach 20%, Tatsugiri 40%, Basculin 40%, Tentacool 15%, Carracosta 4%, Bruxish 1% |
| Route124 | Surf | 6-39 | Psyduck 60%, Tympole 30%, Slowpoke 5%, Froakie 4%, Azumarill 1% |
| Route124 | Pesca | 6-48 | Remoraid 70%, Wimpod 30%, Luvdisc 60%, Krabby 20%, Goldeen 20%, Wailmer 40%, Wugtrio 40%, Chinchou 15%, Dondozo 4%, Gyarados 1% |
| PetalburgWoods | Erba/terreno | 6-7 | Cottonee 31%, Stunky 30%, Tinkatink 10%, Fidough 10%, Bellsprout 5%, Voltorb (Hisui) 5%, Trubbish 4%, Smoliv 4%, Flabébé 1% |
| RusturfTunnel | Erba/terreno | 6-9 | Pikipek 30%, Doduo 30%, Rattata (Alola) 10%, Rolycoly 10%, Clobbopus 5%, Zigzagoon (Galar) 5%, Wooloo 4%, Spearow 4%, Helioptile 2% |
| GraniteCave_1F | Erba/terreno | 7-11 | Nickit 30%, Numel 30%, Purrloin 10%, Honedge 10%, Inkay 5%, Meowth (Galar) 5%, Roggenrola 4%, Varoom 4%, Makuhita 2% |
| GraniteCave_B1F | Erba/terreno | 10-12 | Impidimp 30%, Klink 30%, Zorua 10%, Meowth (Alola) 10%, Croagunk 5%, Poochyena 5%, Wooper (Paldea) 4%, Slowpoke (Galar) 4%, Bronzor 2% |
| MtPyre_1F | Erba/terreno | 22-29 | Espurr 30%, Unown 30%, Lunatone 10%, Pawniard 10%, Chimecho 5%, Elgyem 5%, Vullaby 4%, Absol 4%, Solrock 2% |
| VictoryRoad_1F | Erba/terreno | 39-42 | Stonjourner 30%, Turtonator 30%, Carbink 10%, Sawk 10%, Bombirdier 5%, Bagon 5%, Shuckle 4%, Falinks 4%, Flamigo 2% |
| SafariZone_South | Erba/terreno | 25-29 | Mienfoo 30%, Growlithe 30%, Porygon 10%, Archen 10%, Onix 5%, Dunsparce 5%, Togetic 4%, Qwilfish (Hisui) 4%, Electabuzz 2% |
| Underwater_Route126 | Surf | 20-39 | Skrelp 60%, Lombre 30%, Dracovish 5%, Mudkip 4%, Arctovish 1% |
| AbandonedShip_Rooms_B1F | Surf | 6-39 | Shellos 60%, Poliwag 30%, Horsea 5%, Ducklett 4%, Oshawott 1% |
| AbandonedShip_Rooms_B1F | Pesca | 6-39 | Finizen 70%, Wishiwashi 30%, Finneon 60%, Binacle 20%, Clauncher 20%, Omanyte 40%, Basculin 40%, Veluza 15%, Wailmer 4%, Tentacool 1% |
| GraniteCave_B2F | Erba/terreno | 11-13 | Geodude (Alola) 30%, Meditite 30%, Diglett 10%, Sandygast 10%, Drilbur 5%, Solosis 5%, Woobat 4%, Sandile 4%, Swablu 2% |
| GraniteCave_B2F | Spaccaroccia | 6-20 | Swinub 60%, Golett 30%, Hippopotas 5%, Numel 4%, Nacli 1% |
| FieryPath | Erba/terreno | 14-16 | Pansear 30%, Gulpin 30%, Cranidos 10%, Gastly 10%, Rhyhorn 5%, Roselia 5%, Sizzlipede 4%, Minior 4%, Charcadet 2% |
| MeteorFalls_B1F_2R | Erba/terreno | 25-42 | Klawf 30%, Orthworm 30%, Indeedee 10%, Skarmory 10%, Shieldon 5%, Togedemaru 5%, Oranguru 4%, Klefki 4%, Duraludon 2% |
| MeteorFalls_B1F_2R | Surf | 6-39 | Bibarel 60%, Cramorant 30%, Quagsire 5%, Spheal 4%, Mareanie 1% |
| MeteorFalls_B1F_2R | Pesca | 6-48 | Wiglett 70%, Barboach 30%, Shellder 60%, Finneon 20%, Goldeen 20%, Staryu 40%, Alomomola 40%, Tirtouga 15%, Relicanth 4%, Tatsugiri 1% |
| JaggedPass | Erba/terreno | 20-22 | Litleo 30%, Tyrunt 30%, Ponyta 10%, Toedscool 10%, Torchic 5%, Mawile 5%, Koffing 4%, Seviper 4%, Growlithe (Hisui) 2% |
| Route106 | Surf | 6-39 | Buizel 60%, Dewpider 30%, Shellos 5%, Dewott 4%, Pyukumuku 1% |
| Route106 | Pesca | 6-48 | Arrokuda 70%, Wishiwashi 30%, Clauncher 60%, Remoraid 20%, Luvdisc 20%, Corsola 40%, Bruxish 40%, Kabuto 15%, Qwilfish 4%, Dondozo 1% |
| Route107 | Surf | 6-39 | Marill 60%, Panpour 30%, Psyduck 5%, Drizzile 4%, Frillish 1% |
| Route107 | Pesca | 6-48 | Carvanha 70%, Wimpod 30%, Krabby 60%, Corphish 20%, Binacle 20%, Staryu 40%, Tatsugiri 40%, Omanyte 15%, Relicanth 4%, Gyarados 1% |
| Route108 | Surf | 6-39 | Surskit 60%, Skrelp 30%, Horsea 5%, Croconaw 4%, Quagsire 1% |
| Route108 | Pesca | 6-48 | Finizen 70%, Shellder 30%, Chinchou 60%, Barboach 20%, Goldeen 20%, Wailmer 40%, Basculin 40%, Tentacool 15%, Alomomola 4%, Carracosta 1% |
| Route109 | Surf | 6-39 | Poliwag 60%, Ducklett 30%, Tympole 5%, Brionne 4%, Seel 1% |
| Route109 | Pesca | 6-48 | Remoraid 70%, Arrokuda 30%, Clauncher 60%, Finneon 20%, Luvdisc 20%, Kabuto 40%, Corsola 40%, Wiglett 15%, Veluza 4%, Bruxish 1% |
| Route115 | Erba/terreno | 23-26 | Spinda 30%, Squawkabilly 30%, Emolga 10%, Farfetch'd (Galar) 10%, Chansey 5%, Scraggy 5%, Tropius 4%, Girafarig 4%, Chatot 2% |
| Route115 | Surf | 6-39 | Wingull 60%, Chewtle 30%, Spheal 5%, Frogadier 4%, Clamperl 1% |
| Route115 | Pesca | 6-48 | Wimpod 70%, Finizen 30%, Chinchou 60%, Binacle 20%, Carvanha 20%, Staryu 40%, Qwilfish 40%, Omanyte 15%, Dondozo 4%, Relicanth 1% |
| NewMauville_Inside | Erba/terreno | 22-26 | Minun 30%, Plusle 30%, Voltorb 10%, Pachirisu 10%, Durant 5%, Morpeko 5%, Electrike 4%, Dedenne 4%, Stunfisk (Galar) 2% |
| Route119 | Erba/terreno | 24-27 | Skiddo 30%, Pumpkaboo 30%, Yanma 10%, Volbeat 10%, Tangela 5%, Cacnea 5%, Deerling 4%, Chespin 4%, Illumise 2% |
| Route119 | Surf | 6-39 | Lotad 60%, Slowpoke 30%, Poliwag 5%, Marshtomp 4%, Pyukumuku 1% |
| Route119 | Pesca | 6-48 | Wishiwashi 70%, Corphish 30%, Krabby 60%, Luvdisc 20%, Remoraid 20%, Wailmer 40%, Veluza 40%, Tirtouga 15%, Basculegion 4%, Gyarados 1% |
| Route120 | Erba/terreno | 25-27 | Swirlix 30%, Spritzee 30%, Sableye 10%, Maschiff 10%, Scyther 5%, Heracross 5%, Turtwig 4%, Pinsir 4%, Maractus 2% |
| Route120 | Surf | 6-39 | Frillish 60%, Panpour 30%, Prinplup 5%, Buizel 4%, Bibarel 1% |
| Route120 | Pesca | 6-48 | Goldeen 70%, Carvanha 30%, Barboach 60%, Binacle 20%, Shellder 20%, Kabuto 40%, Alomomola 40%, Tentacool 15%, Corsola 4%, Bruxish 1% |
| Route121 | Erba/terreno | 25-28 | Aipom 30%, Kecleon 30%, Castform 10%, Audino 10%, Snivy 5%, Corsola (Galar) 5%, Mimikyu 4%, Miltank 4%, Teddiursa 2% |
| Route121 | Surf | 6-39 | Marill 60%, Chewtle 30%, Mareanie 5%, Wartortle 4%, Araquanid 1% |
| Route121 | Pesca | 6-48 | Arrokuda 70%, Wishiwashi 30%, Clauncher 60%, Krabby 20%, Finizen 20%, Omanyte 40%, Tatsugiri 40%, Staryu 15%, Qwilfish 4%, Gyarados 1% |
| Route122 | Surf | 6-39 | Wingull 60%, Shellos 30%, Slowpoke 5%, Quaxwell 4%, Lombre 1% |
| Route122 | Pesca | 6-48 | Wiglett 70%, Wimpod 30%, Corphish 60%, Chinchou 20%, Finneon 20%, Corsola 40%, Bruxish 40%, Tirtouga 15%, Relicanth 4%, Dondozo 1% |
| Route123 | Erba/terreno | 25-28 | Misdreavus 30%, Sinistea 30%, Pineco 10%, Duskull 10%, Comfey 5%, Spiritomb 5%, Bulbasaur 4%, Yamask 4%, Greavard 2% |
| Route123 | Surf | 6-39 | Skrelp 60%, Tympole 30%, Seel 5%, Frogadier 4%, Quagsire 1% |
| Route123 | Pesca | 6-48 | Shellder 70%, Arrokuda 30%, Finizen 60%, Goldeen 20%, Clauncher 20%, Wailmer 40%, Tatsugiri 40%, Tentacool 15%, Alomomola 4%, Kabutops 1% |
| MtPyre_2F | Erba/terreno | 22-29 | Wobbuffet 30%, Spoink 30%, Ponyta (Galar) 10%, Drowzee 10%, Natu 5%, Gothita 5%, Sigilyph 4%, Gimmighoul 4%, Abra 2% |
| MtPyre_3F | Erba/terreno | 22-29 | Shuppet 30%, Munna 30%, Hatenna 10%, Litwick 10%, Flittle 5%, Ralts 5%, Druddigon 4%, Linoone (Galar) 4%, Throh 2% |
| MtPyre_4F | Erba/terreno | 22-29 | Qwilfish (Hisui) 30%, Corsola (Galar) 30%, Scraggy 10%, Pancham 10%, Hitmontop 5%, Girafarig 5%, Rotom 4%, Sudowoodo 4%, Misdreavus 2% |
| MtPyre_5F | Erba/terreno | 22-29 | Haunter 30%, Raticate (Alola) 30%, Sableye 10%, Pumpkaboo 10%, Crabrawler 5%, Stunky 5%, Bombirdier 4%, Kangaskhan 4%, Torkoal 2% |
| MtPyre_6F | Erba/terreno | 22-29 | Mightyena 30%, Maschiff 30%, Liepard 10%, Murkrow 10%, Houndour 5%, Grimer (Alola) 5%, Furfrou 4%, Heatmor 4%, Stantler 2% |
| MtPyre_Exterior | Erba/terreno | 25-29 | Morelull 30%, Milcery 30%, Slugma 10%, Ponyta 10%, Oricorio 5%, Litten 5%, Magmar 4%, Farfetch'd 4%, Hawlucha 2% |
| MtPyre_Summit | Erba/terreno | 24-31 | Tinkatuff 30%, Growlithe (Hisui) 30%, Granbull 10%, Larvesta 10%, Fuecoco 5%, Togetic 5%, Dedenne 4%, Dhelmise 4%, Passimian 2% |
| GraniteCave_StevensRoom | Erba/terreno | 7-11 | Cubone 30%, Yungoos 30%, Yamask (Galar) 10%, Silicobra 10%, Mankey 5%, Geodude 5%, Baltoy 4%, Cherubi 4%, Tynamo 2% |
| Route125 | Surf | 6-39 | Surskit 60%, Ducklett 30%, Spheal 5%, Marshtomp 4%, Pyukumuku 1% |
| Route125 | Pesca | 6-48 | Binacle 70%, Corphish 30%, Remoraid 60%, Chinchou 20%, Luvdisc 20%, Veluza 40%, Basculin 40%, Corsola 15%, Relicanth 4%, Dondozo 1% |
| Route126 | Surf | 6-39 | Psyduck 60%, Slowpoke 30%, Feebas 5%, Brionne 4%, Clamperl 1% |
| Route126 | Pesca | 6-48 | Wimpod 70%, Wishiwashi 30%, Finneon 60%, Krabby 20%, Carvanha 20%, Qwilfish 40%, Wugtrio 40%, Wailmer 15%, Whiscash 4%, Gyarados 1% |
| Route127 | Surf | 6-39 | Poliwag 60%, Buizel 30%, Horsea 5%, Prinplup 4%, Bibarel 1% |
| Route127 | Pesca | 6-48 | Shellder 70%, Arrokuda 30%, Clauncher 60%, Finizen 20%, Goldeen 20%, Tatsugiri 40%, Alomomola 40%, Omanyte 15%, Starmie 4%, Tentacruel 1% |
| Route128 | Surf | 6-39 | Mareanie 60%, Dewpider 30%, Panpour 5%, Quaxwell 4%, Frillish 1% |
| Route128 | Pesca | 6-48 | Binacle 70%, Corphish 30%, Chinchou 60%, Luvdisc 20%, Remoraid 20%, Bruxish 40%, Veluza 40%, Basculin 15%, Relicanth 4%, Dondozo 1% |
| Route129 | Surf | 6-39 | Psyduck 60%, Chewtle 30%, Tympole 5%, Drizzile 4%, Azumarill 1% |
| Route129 | Pesca | 6-48 | Carvanha 70%, Wiglett 30%, Barboach 60%, Finneon 20%, Krabby 20%, Qwilfish 40%, Tirtouga 40%, Kabuto 15%, Golisopod 4%, Gyarados 1% |
| Route130 *(non raggiungibile)* | Erba/terreno | 6-53 | Dottler 30%, Drampa 30%, Furret 10%, Bouffalant 10%, Meowth 5%, Delphox 5%, Smeargle 4%, Zigzagoon 4%, Pikipek 2% |
| Route130 *(non raggiungibile)* | Surf | 6-39 | Shellos 60%, Surskit 30%, Seel 5%, Dewott 4%, Clamperl 1% |
| Route130 *(non raggiungibile)* | Pesca | 6-48 | Wishiwashi 70%, Corphish 30%, Remoraid 60%, Goldeen 20%, Binacle 20%, Corsola 40%, Wailmer 40%, Omanyte 15%, Bruxish 4%, Tentacruel 1% |
| Route131 | Surf | 6-39 | Skrelp 60%, Ducklett 30%, Feebas 5%, Wartortle 4%, Frillish 1% |
| Route131 | Pesca | 6-48 | Wiglett 70%, Carvanha 30%, Finizen 60%, Shellder 20%, Krabby 20%, Basculin 40%, Alomomola 40%, Kabuto 15%, Starmie 4%, Qwilfish 1% |
| Route132 | Surf | 6-39 | Wooper 60%, Spheal 30%, Horsea 5%, Croconaw 4%, Pyukumuku 1% |
| Route132 | Pesca | 6-48 | Barboach 70%, Arrokuda 30%, Clauncher 60%, Finneon 20%, Luvdisc 20%, Tatsugiri 40%, Veluza 40%, Tirtouga 15%, Relicanth 4%, Dondozo 1% |
| Route133 | Surf | 6-39 | Lotad 60%, Wingull 30%, Chewtle 5%, Brionne 4%, Bibarel 1% |
| Route133 | Pesca | 6-48 | Wishiwashi 70%, Wimpod 30%, Chinchou 60%, Goldeen 20%, Remoraid 20%, Bruxish 40%, Corsola 40%, Wailmer 15%, Tentacruel 4%, Gyarados 1% |
| Route134 | Surf | 6-39 | Ducklett 60%, Panpour 30%, Shellos 5%, Wartortle 4%, Araquanid 1% |
| Route134 | Pesca | 6-48 | Wiglett 70%, Finizen 30%, Carvanha 60%, Binacle 20%, Corphish 20%, Alomomola 40%, Qwilfish 40%, Kabuto 15%, Basculegion 4%, Omastar 1% |
| AbandonedShip_HiddenFloorCorridors | Surf | 6-39 | Marill 60%, Slowpoke 30%, Feebas 5%, Spheal 4%, Tauros (Paldea) 1% |
| AbandonedShip_HiddenFloorCorridors | Pesca | 6-39 | Arrokuda 70%, Barboach 30%, Krabby 60%, Shellder 20%, Luvdisc 20%, Tirtouga 40%, Tatsugiri 40%, Staryu 15%, Clauncher 4%, Finneon 1% |
| SeafloorCavern_Room1 | Erba/terreno | 28-39 | Nidoran♀ 30%, Spinarak 30%, Lokix 10%, Seviper 10%, Toxtricity 5%, Aerodactyl 5%, Deino 4%, Meowscarada 4%, Cyclizar 2% |
| SeafloorCavern_Room2 | Erba/terreno | 28-39 | Weedle 30%, Nidorino 30%, Dustox 10%, Rampardos 10%, Shiftry 5%, Pupitar 5%, Crustle 4%, Dracozolt 4%, Lucario 2% |
| SeafloorCavern_Room3 | Erba/terreno | 28-39 | Whirlipede 30%, Clodsire 30%, Klawf 10%, Lairon 10%, Glimmora 5%, Mantine 5%, Arctovish 4%, Venomoth 4%, Malamar 2% |
| SeafloorCavern_Room4 | Erba/terreno | 28-39 | Carkol 30%, Nosepass 30%, Minior 10%, Zebstrika 10%, Persian (Alola) 5%, Garbodor 5%, Cacturne 4%, Dratini 4%, Zoroark 2% |
| SeafloorCavern_Room5 | Erba/terreno | 28-39 | Vullaby 30%, Roselia 30%, Morpeko 10%, Sneasel (Hisui) 10%, Grafaiai 5%, Carbink 5%, Drampa 4%, Toxicroak 4%, Venusaur 2% |
| SeafloorCavern_Room6 | Erba/terreno | 28-39 | Tyrunt 30%, Weepinbell 30%, Stonjourner 10%, Lunatone 10%, Shuckle 5%, Lycanroc 5%, Weezing (Galar) 4%, Gigalith 4%, Dracovish 2% |
| SeafloorCavern_Room6 | Surf | 6-39 | Tympole 60%, Wingull 30%, Tauros (Paldea) 5%, Cramorant 4%, Drizzile 1% |
| SeafloorCavern_Room6 | Pesca | 6-48 | Wimpod 70%, Wishiwashi 30%, Chinchou 60%, Clauncher 20%, Corphish 20%, Kabuto 40%, Veluza 40%, Staryu 15%, Dondozo 4%, Relicanth 1% |
| SeafloorCavern_Room7 | Erba/terreno | 28-39 | Onix 30%, Lileep 30%, Solrock 10%, Spiritomb 10%, Shelgon 5%, Absol 5%, Slowbro (Galar) 4%, Tsareena 4%, Thievul 2% |
| SeafloorCavern_Room7 | Surf | 6-39 | Buizel 60%, Surskit 30%, Arctovish 5%, Mantine 4%, Frogadier 1% |
| SeafloorCavern_Room7 | Pesca | 6-48 | Wiglett 70%, Carvanha 30%, Shellder 60%, Luvdisc 20%, Arrokuda 20%, Corsola 40%, Basculin 40%, Omanyte 15%, Carracosta 4%, Gyarados 1% |
| SeafloorCavern_Room8 | Erba/terreno | 28-39 | Golbat 30%, Arbok 30%, Gloom 10%, Swalot 10%, Pangoro 5%, Incineroar 5%, Vaporeon 4%, Bastiodon 4%, Hakamo-o 2% |
| SeafloorCavern_Entrance | Surf | 6-39 | Psyduck 60%, Skrelp 30%, Dracovish 5%, Croconaw 4%, Quagsire 1% |
| SeafloorCavern_Entrance | Pesca | 6-48 | Goldeen 70%, Binacle 30%, Finizen 60%, Finneon 20%, Remoraid 20%, Tentacool 40%, Bruxish 40%, Wailmer 15%, Tatsugiri 4%, Qwilfish 1% |
| CaveOfOrigin_Entrance | Erba/terreno | 28-39 | Wobbuffet 30%, Archen 30%, Kadabra 10%, Grumpig 10%, Dreepy 5%, Exeggutor 5%, Toxtricity 4%, Typhlosion (Hisui) 4%, Dhelmise 2% |
| CaveOfOrigin_1F | Erba/terreno | 31-39 | Ariados 30%, Sudowoodo 30%, Xatu 10%, Beedrill 10%, Metang 5%, Decidueye 5%, Shiftry 4%, Rabsca 4%, Wyrdeer 2% |
| CaveOfOrigin_UnusedRubySapphireMap1 *(non raggiungibile)* | Erba/terreno | 31-39 | Amaura 30%, Sneasel 30%, Mr. Mime 10%, Jynx 10%, Froslass 5%, Aegislash 5%, Chimecho 4%, Sinistcha 4%, Aerodactyl 2% |
| CaveOfOrigin_UnusedRubySapphireMap2 *(non raggiungibile)* | Erba/terreno | 31-39 | Meowstic 30%, Dustox 30%, Ponyta (Galar) 10%, Mimikyu 10%, Swoobat 5%, Delphox 5%, Zoroark (Hisui) 4%, Avalugg (Hisui) 4%, Polteageist 2% |
| CaveOfOrigin_UnusedRubySapphireMap3 *(non raggiungibile)* | Erba/terreno | 31-39 | Indeedee 30%, Vivillon 30%, Lokix 10%, Nidorina 10%, Houndstone 5%, Orbeetle 5%, Kleavor 4%, Raichu (Alola) 4%, Ceruledge 2% |
| NewMauville_Entrance | Erba/terreno | 22-26 | Yamper 30%, Shinx 30%, Blitzle 10%, Pawmi 10%, Skarmory 5%, Stunfisk 5%, Charjabug 4%, Grookey 4%, Cufant 2% |
| SafariZone_Southwest | Erba/terreno | 25-29 | Flaaffy 30%, Axew 30%, Floette 10%, Gligar 10%, Grotle 5%, Tangela 5%, Lickitung 4%, Chansey 4%, Magmar 2% |
| SafariZone_Southwest | Surf | 20-39 | Lombre 60%, Clamperl 30%, Marshtomp 5%, Dracovish 4%, Mantine 1% |
| SafariZone_Southwest | Pesca | 6-42 | Barboach 70%, Wimpod 30%, Krabby 60%, Shellder 20%, Chinchou 20%, Wailmer 40%, Veluza 40%, Alomomola 15%, Dondozo 4%, Staryu 1% |
| SafariZone_North | Erba/terreno | 27-33 | Tranquill 30%, Skiddo 30%, Yanma 10%, Fletchinder 10%, Ursaring 5%, Combusken 5%, Gurdurr 4%, Donphan 4%, Farfetch'd (Galar) 2% |
| SafariZone_North | Spaccaroccia | 6-31 | Trapinch 60%, Nincada 30%, Aron 5%, Amaura 4%, Wormadam 1% |
| SafariZone_Northwest | Erba/terreno | 27-33 | Luxio 30%, Pawmo 30%, Audino 10%, Aipom 10%, Crocalor 5%, Corvisquire 5%, Flamigo 4%, Sigilyph 4%, Kangaskhan 2% |
| SafariZone_Northwest | Surf | 20-42 | Seel 60%, Poliwag 30%, Prinplup 5%, Tauros (Paldea) 4%, Frillish 1% |
| SafariZone_Northwest | Pesca | 6-42 | Arrokuda 70%, Binacle 30%, Clauncher 60%, Remoraid 20%, Goldeen 20%, Corsola 40%, Qwilfish 40%, Tatsugiri 15%, Carracosta 4%, Basculin 1% |
| VictoryRoad_B1F | Erba/terreno | 41-44 | Noibat 30%, Applin 30%, Magneton 10%, Bewear 10%, Sliggoo (Hisui) 5%, Emboar 5%, Armaldo 4%, Heracross 4%, Snorlax 2% |
| VictoryRoad_B1F | Spaccaroccia | 31-42 | Sandslash 60%, Wormadam 30%, Mudsdale 5%, Gabite 4%, Stunfisk (Galar) 1% |
| VictoryRoad_B2F | Erba/terreno | 42-47 | Piloswine 30%, Magcargo 30%, Breloom 10%, Passimian 10%, Chesnaught 5%, Dragonair 5%, Orthworm 4%, Diggersby 4%, Throh 2% |
| VictoryRoad_B2F | Surf | 25-42 | Cramorant 60%, Pyukumuku 30%, Lapras 5%, Vaporeon 4%, Arctovish 1% |
| VictoryRoad_B2F | Pesca | 6-48 | Wishiwashi 70%, Wiglett 30%, Krabby 60%, Carvanha 20%, Chinchou 20%, Tentacool 40%, Veluza 40%, Alomomola 15%, Relicanth 4%, Gyarados 1% |
| MeteorFalls_1F_1R | Erba/terreno | 14-20 | Diglett (Alola) 30%, Mawile 30%, Klink 10%, Paras 10%, Pawniard 5%, Munna 5%, Drowzee 4%, Treecko 4%, Rhyhorn 2% |
| MeteorFalls_1F_1R | Surf | 6-39 | Wingull 60%, Poliwhirl 30%, Quaxwell 5%, Horsea 4%, Feebas 1% |
| MeteorFalls_1F_1R | Pesca | 6-48 | Corphish 70%, Wimpod 30%, Finizen 60%, Luvdisc 20%, Binacle 20%, Kabuto 40%, Bruxish 40%, Omanyte 15%, Dondozo 4%, Wailord 1% |
| MeteorFalls_1F_2R | Erba/terreno | 37-42 | Espathra 30%, Turtonator 30%, Druddigon 10%, Altaria 10%, Goomy 5%, Escavalier 5%, Cyclizar 4%, Durant 4%, Gardevoir 2% |
| MeteorFalls_1F_2R | Surf | 6-39 | Azumarill 60%, Pyukumuku 30%, Dewott 5%, Panpour 4%, Ducklett 1% |
| MeteorFalls_1F_2R | Pesca | 6-48 | Wishiwashi 70%, Carvanha 30%, Arrokuda 60%, Remoraid 20%, Chinchou 20%, Tentacool 40%, Qwilfish 40%, Veluza 15%, Corsola 4%, Gyarados 1% |
| MeteorFalls_B1F_1R | Erba/terreno | 37-42 | Perrserker 30%, Forretress 30%, Gothorita 10%, Bouffalant 10%, Metang 5%, Gholdengo 5%, Vespiquen 4%, Revavroom 4%, Dracozolt 2% |
| MeteorFalls_B1F_1R | Surf | 6-39 | Palpitoad 60%, Cramorant 30%, Prinplup 5%, Buizel 4%, Chewtle 1% |
| MeteorFalls_B1F_1R | Pesca | 6-48 | Barboach 70%, Wimpod 30%, Krabby 60%, Clauncher 20%, Finneon 20%, Omanyte 40%, Basculin 40%, Alomomola 15%, Relicanth 4%, Dondozo 1% |
| ShoalCave_LowTideStairsRoom | Erba/terreno | 26-35 | Cetoddle 30%, Snover 30%, Sneasel 10%, Vulpix (Alola) 10%, Eiscue 5%, Arctozolt 5%, Cryogonal 4%, Jynx 4%, Mr. Mime (Galar) 2% |
| ShoalCave_LowTideLowerRoom | Erba/terreno | 26-35 | Delibird 30%, Vanillite 30%, Cubchoo 10%, Darumaka (Galar) 10%, Frigibax 5%, Bergmite 5%, Frosmoth 4%, Sandslash (Alola) 4%, Froslass 2% |
| ShoalCave_LowTideInnerRoom | Erba/terreno | 26-35 | Sandshrew (Alola) 30%, Drifloon 30%, Amaura 10%, Snorunt 10%, Crabominable 5%, Monferno 5%, Piloswine 4%, Sunflora 4%, Mantine 2% |
| ShoalCave_LowTideInnerRoom | Surf | 6-39 | Mareanie 60%, Shellos 30%, Dewott 5%, Bibarel 4%, Clamperl 1% |
| ShoalCave_LowTideInnerRoom | Pesca | 6-48 | Wishiwashi 70%, Shellder 30%, Goldeen 60%, Corphish 20%, Barboach 20%, Tirtouga 40%, Tatsugiri 40%, Staryu 15%, Relicanth 4%, Bruxish 1% |
| ShoalCave_LowTideEntranceRoom | Erba/terreno | 26-35 | Snom 30%, Loudred 30%, Cetoddle 10%, Sneasel 10%, Cryogonal 5%, Ninetales (Alola) 5%, Charmeleon 4%, Butterfree 4%, Eiscue 2% |
| ShoalCave_LowTideEntranceRoom | Surf | 6-39 | Surskit 60%, Slowpoke 30%, Quaxwell 5%, Araquanid 4%, Frillish 1% |
| ShoalCave_LowTideEntranceRoom | Pesca | 6-48 | Finizen 70%, Wiglett 30%, Finneon 60%, Luvdisc 20%, Remoraid 20%, Kabuto 40%, Veluza 40%, Wailmer 15%, Qwilfish 4%, Gyarados 1% |
| LilycoveCity | Surf | 6-39 | Lotad 60%, Seel 30%, Skrelp 5%, Brionne 4%, Quagsire 1% |
| LilycoveCity | Pesca | 6-48 | Arrokuda 70%, Wimpod 30%, Carvanha 60%, Binacle 20%, Goldeen 20%, Corsola 40%, Tatsugiri 40%, Tentacool 15%, Dondozo 4%, Carracosta 1% |
| DewfordTown | Surf | 6-39 | Spheal 60%, Mareanie 30%, Horsea 5%, Wartortle 4%, Bibarel 1% |
| DewfordTown | Pesca | 6-48 | Barboach 70%, Shellder 30%, Clauncher 60%, Corphish 20%, Chinchou 20%, Omanyte 40%, Bruxish 40%, Staryu 15%, Basculegion 4%, Relicanth 1% |
| SlateportCity | Surf | 6-39 | Psyduck 60%, Slowpoke 30%, Shellos 5%, Frogadier 4%, Clamperl 1% |
| SlateportCity | Pesca | 6-48 | Wiglett 70%, Finizen 30%, Luvdisc 60%, Krabby 20%, Finneon 20%, Kabuto 40%, Alomomola 40%, Wailmer 15%, Qwilfish 4%, Gyarados 1% |
| MossdeepCity | Surf | 6-39 | Skrelp 60%, Panpour 30%, Poliwag 5%, Drizzile 4%, Azumarill 1% |
| MossdeepCity | Pesca | 6-48 | Wishiwashi 70%, Wimpod 30%, Goldeen 60%, Arrokuda 20%, Carvanha 20%, Veluza 40%, Tatsugiri 40%, Tirtouga 15%, Dondozo 4%, Tentacruel 1% |
| PacifidlogTown | Surf | 6-39 | Tympole 60%, Surskit 30%, Horsea 5%, Croconaw 4%, Lombre 1% |
| PacifidlogTown | Pesca | 6-48 | Binacle 70%, Barboach 30%, Remoraid 60%, Clauncher 20%, Corphish 20%, Bruxish 40%, Corsola 40%, Omanyte 15%, Basculegion 4%, Starmie 1% |
| EverGrandeCity | Surf | 6-39 | Wingull 60%, Ducklett 30%, Buizel 5%, Marshtomp 4%, Quagsire 1% |
| EverGrandeCity | Pesca | 6-48 | Shellder 70%, Wiglett 30%, Finneon 60%, Krabby 20%, Chinchou 20%, Alomomola 40%, Wailmer 40%, Kabuto 15%, Relicanth 4%, Gyarados 1% |
| PetalburgCity | Surf | 6-39 | Mareanie 60%, Dewpider 30%, Dracovish 5%, Spheal 4%, Feebas 1% |
| PetalburgCity | Pesca | 6-48 | Wishiwashi 70%, Finizen 30%, Luvdisc 60%, Carvanha 20%, Wimpod 20%, Tirtouga 40%, Veluza 40%, Tentacool 15%, Qwilfish 4%, Tatsugiri 1% |
| Underwater_Route124 | Surf | 20-39 | Psyduck 60%, Seel 30%, Arctovish 5%, Tauros (Paldea) 4%, Croconaw 1% |
| ShoalCave_LowTideIceRoom | Erba/terreno | 26-35 | Snover 30%, Rufflet 30%, Darumaka (Galar) 10%, Squawkabilly 10%, Chikorita 5%, Arctozolt 5%, Sandslash (Alola) 4%, Delibird 4%, Crabominable 2% |
| SkyPillar_1F | Erba/terreno | 37-41 | Chatot 30%, Dodrio 30%, Tropius 10%, Swellow 10%, Staraptor 5%, Kilowattrel 5%, Pidgeot 4%, Dragonair 4%, Vespiquen 2% |
| SootopolisCity | Surf | 6-39 | Chewtle 60%, Skrelp 30%, Panpour 5%, Quaxwell 4%, Pyukumuku 1% |
| SootopolisCity | Pesca | 6-48 | Finizen 70%, Binacle 30%, Goldeen 60%, Remoraid 20%, Barboach 20%, Corsola 40%, Basculin 40%, Relicanth 15%, Bruxish 4%, Finneon 1% |
| SkyPillar_3F | Erba/terreno | 37-41 | Noctowl 30%, Oranguru 30%, Oricorio 10%, Hawlucha 10%, Duraludon 5%, Serperior 5%, Emolga 4%, Fearow 4%, Appletun 2% |
| SkyPillar_5F | Erba/terreno | 37-65 | Jumpluff 30%, Butterfree 30%, Vivillon 10%, Druddigon 10%, Espathra 10%, Cinderace 4%, Hakamo-o 4%, Spidops 1%, Cresselia 1% |
| SafariZone_Southeast | Erba/terreno | 37-65 | Machoke 30%, Dubwool 30%, Dusclops 10%, Komala 10%, Manectric 10%, Vigoroth 4%, Emboar 4%, Centiskorch 1%, Ting-Lu 1% |
| SafariZone_Southeast | Surf | 25-42 | Bibarel 60%, Quagsire 30%, Drizzile 5%, Arctovish 4%, Lapras 1% |
| SafariZone_Southeast | Pesca | 25-42 | Magikarp 70%, Corphish 30%, Omanyte 60%, Kabuto 20%, Wugtrio 20%, Tentacool 40%, Luvdisc 40%, Bruxish 15%, Relicanth 4%, Lumineon 1% |
| SafariZone_Northeast | Erba/terreno | 37-65 | Electrode 30%, Sawk 30%, Cofagrigus 10%, Comfey 10%, Tauros 10%, Shelgon 4%, Serperior 4%, Arboliva 1%, Uxie 1% |
| SafariZone_Northeast | Spaccaroccia | 20-42 | Toedscool 60%, Hippopotas 30%, Kleavor 5%, Archen 4%, Pupitar 1% |
| MagmaHideout_1F | Erba/terreno | 27-37 | Togedemaru 30%, Growlithe 30%, Litleo 10%, Torkoal 10%, Cyndaquil 5%, Gible 5%, Simisear 4%, Scovillain 4%, Klefki 2% |
| MagmaHideout_2F_1R | Erba/terreno | 27-37 | Graveler 30%, Skorupi 30%, Fletchinder 10%, Swalot 10%, Chimchar 5%, Beldum 5%, Ninetales 4%, Heatmor 4%, Toxtricity 2% |
| MagmaHideout_2F_2R | Erba/terreno | 27-37 | Grimer 30%, Larvesta 30%, Whirlipede 10%, Minior 10%, Scorbunny 5%, Larvitar 5%, Carbink 4%, Arbok 4%, Golbat 2% |
| MagmaHideout_3F_1R | Erba/terreno | 27-37 | Anorith 30%, Nosepass 30%, Boldore 10%, Qwilfish (Hisui) 10%, Charmander 5%, Marowak 5%, Lycanroc 4%, Lunatone 4%, Kricketune 2% |
| MagmaHideout_3F_2R | Erba/terreno | 27-37 | Naclstack 30%, Nidorino 30%, Gloom 10%, Stunfisk (Galar) 10%, Fennekin 5%, Aerodactyl 5%, Dugtrio 4%, Jangmo-o 4%, Wormadam 2% |
| MagmaHideout_4F | Erba/terreno | 27-37 | Ponyta 30%, Mudbray 30%, Weepinbell 10%, Stunfisk 10%, Tepig 5%, Gengar 5%, Mawile 4%, Carkol 4%, Clodsire 2% |
| MagmaHideout_3F_3R | Erba/terreno | 27-37 | Nidorina 30%, Sneasel (Hisui) 30%, Solrock 10%, Oricorio 10%, Ivysaur 5%, Roserade 5%, Seviper 4%, Golem (Alola) 4%, Slakoth 2% |
| MagmaHideout_2F_3R | Erba/terreno | 27-37 | Tinkatuff 30%, Lileep 30%, Gligar 10%, Orthworm 10%, Grafaiai 5%, Skarmory 5%, Shuckle 4%, Klawf 4%, Combusken 2% |
| MirageTower_1F | Erba/terreno | 20-24 | Darumaka 30%, Silicobra 30%, Sizzlipede 10%, Salandit 10%, Sprigatito 5%, Litleo 5%, Tyrunt 4%, Houndour 4%, Glimmet 2% |
| MirageTower_2F | Erba/terreno | 20-24 | Drilbur 30%, Dwebble 30%, Toedscool 10%, Phanpy 10%, Rowlet 5%, Shieldon 5%, Onix 4%, Cranidos 4%, Growlithe 2% |
| MirageTower_3F | Erba/terreno | 20-24 | Golett 30%, Pansear 30%, Numel 10%, Litwick 10%, Spritzee 5%, Rhyhorn 5%, Torracat 4%, Carnivine 4%, Sandslash 2% |
| MirageTower_4F | Erba/terreno | 20-24 | Vulpix 30%, Sandygast 30%, Sandile 10%, Minun 10%, Crocalor 5%, Growlithe (Hisui) 5%, Torkoal 4%, Hitmonlee 4%, Diggersby 2% |
| DesertUnderpass | Erba/terreno | 39-65 | Galvantula 30%, Illumise 30%, Volbeat 10%, Spidops 10%, Pinsir 10%, Ribombee 4%, Arctibax 4%, Fraxure 1%, Iron Thorns 1% |
| ArtisanCave_B1F | Erba/terreno | 42-65 | Amoonguss 30%, Ferrothorn 30%, Parasect 10%, Grimmsnarl 10%, Lucario 10%, Leavanny 4%, Accelgor 4%, Zweilous 1%, Heatran 1% |
| ArtisanCave_1F | Erba/terreno | 42-65 | Ledian 30%, Absol 30%, Durant 10%, Muk 10%, Toxicroak 10%, Sliggoo (Hisui) 4%, Aegislash 4%, Drapion 1%, Cobalion 1% |
| AlteringCave1 | Erba/terreno | 7-65 | Combee 30%, Kricketot 30%, Wurmple 10%, Nymble 10%, Scatterbug 10%, Burmy 4%, Caterpie 4%, Blipbug 1%, Iron Treads 1% |
| AlteringCave2 *(non raggiungibile)* | Erba/terreno | 3-65 | Grimer (Alola) 30%, Venonat 30%, Meowth (Galar) 10%, Magnemite 10%, Purrloin 10%, Karrablast 4%, Diglett (Alola) 4%, Varoom 1%, Sandy Shocks 1% |
| AlteringCave3 *(non raggiungibile)* | Erba/terreno | 19-65 | Trubbish 30%, Nuzleaf 30%, Vullaby 10%, Murkrow 10%, Cufant 10%, Thievul 4%, Volbeat 4%, Mightyena 1%, Buzzwole 1% |
| AlteringCave4 *(non raggiungibile)* | Erba/terreno | 13-65 | Paras 30%, Sewaddle 30%, Cutiefly 10%, Stunky 10%, Grubbin 10%, Foongus 4%, Sudowoodo 4%, Zorua 1%, Okidogi 1% |
| AlteringCave5 *(non raggiungibile)* | Erba/terreno | 7-65 | Inkay 30%, Trapinch 30%, Nincada 10%, Klink 10%, Ferroseed 10%, Joltik 4%, Meowth (Alola) 4%, Yamask (Galar) 1%, Fezandipiti 1% |
| AlteringCave6 *(non raggiungibile)* | Erba/terreno | 18-65 | Scraggy 30%, Yanma 30%, Sableye 10%, Morpeko 10%, Herdier 10%, Illumise 4%, Ledian 4%, Pawniard 1%, Brute Bonnet 1% |
| AlteringCave7 *(non raggiungibile)* | Erba/terreno | 18-65 | Maschiff 30%, Pachirisu 30%, Koffing 10%, Togedemaru 10%, Pansage 10%, Unown 4%, Ariados 4%, Shelmet 1%, Iron Jugulis 1% |
| AlteringCave8 *(non raggiungibile)* | Erba/terreno | 18-65 | Linoone (Galar) 30%, Spidops 30%, Bronzor 10%, Raticate (Alola) 10%, Buneary 10%, Quilladin 4%, Klefki 4%, Spinda 1%, Chien-Pao 1% |
| AlteringCave9 *(non raggiungibile)* | Erba/terreno | 18-65 | Baltoy 30%, Toedscool 30%, Beedrill 10%, Naclstack 10%, Mienfoo 10%, Furret 4%, Heracross 4%, Elgyem 1%, Terrakion 1% |
| MeteorFalls_StevensCave | Erba/terreno | 37-42 | Drampa 30%, Sigilyph 30%, Mr. Mime 10%, Girafarig 10%, Rabsca 5%, Hakamo-o 5%, Raichu (Alola) 4%, Alakazam 4%, Swoobat 2% |

## Esito della verifica

- Errori: **0**
- Avvisi: **0**

