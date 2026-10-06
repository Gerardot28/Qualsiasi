PetalburgCity_WallysHouse_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PetalburgCity_WallysHouse_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PetalburgCity_WallysHouse_OnWarp
	.byte 0

PetalburgCity_WallysHouse_OnWarp:
	map_script_2 VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_WallysHouse_EventScript_PlayerWallysDadFaceEachOther
	.2byte 0

PetalburgCity_WallysHouse_EventScript_PlayerWallysDadFaceEachOther::
	turnobject LOCALID_PLAYER, DIR_EAST
	turnobject LOCALID_WALLYS_HOUSE_WALLYS_DAD, DIR_WEST
	end

PetalburgCity_WallysHouse_OnFrame:
	map_script_2 VAR_PETALBURG_CITY_STATE, 4, PetalburgCity_WallysHouse_EventScript_GiveHMSurf
	.2byte 0

PetalburgCity_WallysHouse_EventScript_GiveHMSurf::
	lockall
	msgbox PetalburgCity_WallysHouse_Text_PleaseExcuseUs, MSGBOX_DEFAULT
	giveitem ITEM_HM_SURF
	setflag FLAG_RECEIVED_HM_SURF
	msgbox PetalburgCity_WallysHouse_Text_SurfGoAllSortsOfPlaces, MSGBOX_DEFAULT
	setvar VAR_PETALBURG_CITY_STATE, 5
	releaseall
	end

PetalburgCity_WallysHouse_EventScript_WallysDad::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WALLY_VICTORY_ROAD, PetalburgCity_WallysHouse_EventScript_DefeatedWallyInVictoryRoad
	goto_if_set FLAG_RECEIVED_HM_SURF, PetalburgCity_WallysHouse_EventScript_ReceievedHMSurf
	goto_if_set FLAG_THANKED_FOR_PLAYING_WITH_WALLY, PetalburgCity_WallysHouse_EventScript_PlayedWithWally
	msgbox PetalburgCity_WallysHouse_Text_ThanksForPlayingWithWally, MSGBOX_DEFAULT
	setflag FLAG_THANKED_FOR_PLAYING_WITH_WALLY
	release
	end

PetalburgCity_WallysHouse_EventScript_ReceievedHMSurf::
	msgbox PetalburgCity_WallysHouse_Text_WallyIsComingHomeSoon, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_DefeatedWallyInVictoryRoad::
	msgbox PetalburgCity_WallysHouse_Text_YouMetWallyInEverGrandeCity, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_PlayedWithWally::
	msgbox PetalburgCity_WallysHouse_Text_WonderHowWallyIsDoing, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_WallysMom::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_SURF, PetalburgCity_WallysHouse_EventScript_ReceivedHMSurf
	msgbox PetalburgCity_WallysHouse_Text_WallyWasReallyHappy, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_EventScript_ReceivedHMSurf::
	msgbox PetalburgCity_WallysHouse_Text_WallyLeftWithoutTelling, MSGBOX_DEFAULT
	release
	end

PetalburgCity_WallysHouse_Text_ThanksForPlayingWithWally:
	.string "Tu sei…\n"
	.string "Ah, devi essere {PLAYER}{KUN}, vero?\p"
	.string "Grazie per aver fatto compagnia\n"
	.string "al nostro Lino, poco fa.\p"
	.string "È sempre stato fragile e cagionevole,\n"
	.string "fin da quando era piccolo.\p"
	.string "Per un po' starà da suo zio,\n"
	.string "a Mentania.\p"
	.string "Lì l'aria è molto più pulita\n"
	.string "che qui.\p"
	.string "Come? Dov'è Lino?\n"
	.string "Oh, è già partito, il nostro Lino.\p"
	.string "Chissà dove sarà arrivato,\n"
	.string "a quest'ora…$"

PetalburgCity_WallysHouse_Text_WonderHowWallyIsDoing:
	.string "Chissà come se la cava il nostro Lino…$"

PetalburgCity_WallysHouse_Text_PleaseExcuseUs:
	.string "{PLAYER}{KUN}! Scusa se ti abbiamo\n"
	.string "trascinato qui in questo modo.\p"
	.string "Ma da quando è a Mentania, il nostro\n"
	.string "Lino sta davvero molto meglio.\p"
	.string "E lo dobbiamo a te!\p"
	.string "Prima di partire, lo hai aiutato a\n"
	.string "catturare un Pokémon, ricordi?\p"
	.string "Credo che questo l'abbia reso\n"
	.string "davvero felice.\p"
	.string "Anzi, non solo lui.\n"
	.string "Ha reso felice anche me, suo padre.\p"
	.string "Felice che abbia trovato un amico\n"
	.string "come te.\p"
	.string "Non è per comprarti, eh…\n"
	.string "ma vorrei davvero che avessi questa.$"

PetalburgCity_WallysHouse_Text_SurfGoAllSortsOfPlaces:
	.string "Se un tuo Pokémon conosce Surf,\n"
	.string "potrai raggiungere tantissimi posti.\p"
	.string "Chissà, magari perfino qualche\n"
	.string "Varco in mezzo al mare!$"

PetalburgCity_WallysHouse_Text_WallyIsComingHomeSoon:
	.string "Lino tornerà presto a casa.\n"
	.string "Non vedo l'ora di riabbracciarlo.$"

PetalburgCity_WallysHouse_Text_YouMetWallyInEverGrandeCity:
	.string "Come? Hai incontrato Lino\n"
	.string "a Iridopoli?\p"
	.string "Oh, {PLAYER}{KUN}, non scherzare.\p"
	.string "Sarà anche guarito, ma non può\n"
	.string "essere andato così lontano\l"
	.string "tutto da solo.$"

PetalburgCity_WallysHouse_Text_WallyWasReallyHappy:
	.string "Lino era felicissimo quando ci ha\n"
	.string "detto di aver catturato un Pokémon.\p"
	.string "Era da tanto che non lo vedevo\n"
	.string "sorridere così.$"

PetalburgCity_WallysHouse_Text_WallyLeftWithoutTelling:
	.string "Ti prego, che mio marito non lo\n"
	.string "sappia…\p"
	.string "Il nostro Lino se n'è andato da\n"
	.string "Mentania senza dirlo a nessuno.\p"
	.string "Si è portato dietro il suo Ralts.\n"
	.string "Ha lasciato solo un biglietto:\l"
	.string "“Voglio diventare forte.”\p"
	.string "Sai, Lino è fragile, ma ha una\n"
	.string "forza di volontà sorprendente.\p"
	.string "Sono sicura che un giorno tornerà\n"
	.string "sano e salvo!$"

