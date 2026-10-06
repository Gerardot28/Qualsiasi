CaveOfOrigin_B1F_MapScripts::
	.byte 0

CaveOfOrigin_B1F_EventScript_Wallace::
	lock
	faceplayer
	msgbox CaveOfOrigin_B1F_Text_WallaceStory, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 60
	playse SE_PIN
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_Delay48
	waitmovement 0
	delay 30
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_FacePlayer
	waitmovement 0
	message CaveOfOrigin_B1F_Text_WhereIsRayquaza
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_WheresRayquaza::
	multichoice 0, 0, MULTI_WHERES_RAYQUAZA, FALSE
	switch VAR_RESULT
	case 0, CaveOfOrigin_B1F_EventScript_AtCaveOfOrigin
	case 1, CaveOfOrigin_B1F_EventScript_AtMtPyre
	case 2, CaveOfOrigin_B1F_EventScript_AtSkyPillar
	case 3, CaveOfOrigin_B1F_EventScript_DontRemember
	goto CaveOfOrigin_B1F_EventScript_DontRemember
	end

CaveOfOrigin_B1F_EventScript_AtCaveOfOrigin::
	message CaveOfOrigin_B1F_Text_ButWereInCaveOfOrigin
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_AtMtPyre::
	message CaveOfOrigin_B1F_Text_OldLadyDidntMentionThat
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_DontRemember::
	message CaveOfOrigin_B1F_Text_CantYouRememberSomehow
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_AtSkyPillar::
	msgbox CaveOfOrigin_B1F_Text_WellHeadToSkyPillar, MSGBOX_DEFAULT
	closemessage
	playse SE_EXIT
	fadescreenspeed FADE_TO_BLACK, 4
	setflag FLAG_WALLACE_GOES_TO_SKY_PILLAR
	setvar VAR_SOOTOPOLIS_CITY_STATE, 3
	removeobject LOCALID_CAVE_OF_ORIGIN_WALLACE
	clearflag FLAG_HIDE_SKY_PILLAR_WALLACE
	fadescreen FADE_FROM_BLACK
	release
	end

CaveOfOrigin_B1F_Text_WallaceStory:
	.string "Ah, dunque tu sei {PLAYER}{KUN}?\n"
	.string "Ho sentito parlare delle tue imprese.\p"
	.string "Il mio nome è Adriano.\p"
	.string "Un tempo ero il Capopalestra di\n"
	.string "Ceneride, ma poi è successo qualcosa.\p"
	.string "Quando la Cicatrice ha ripreso a\n"
	.string "pulsare, ho affidato la Palestra\l"
	.string "al mio maestro, Rodolfo.\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Groudon e Kyogre, i due Pokémon che\n"
	.string "stanno devastando la città, sono\l"
	.string "le Ancore di Hoenn.\p"
	.string "L'Ancora della Terra e l'Ancora del\n"
	.string "Mare lottano come tremila anni fa,\l"
	.string "quando il cielo si aprì.\p"
	.string "Ma allora le Ancore non erano sole.\p"
	.string "C'era un terzo Pokémon, antico quanto\n"
	.string "loro: Rayquaza, il Custode della Volta.\p"
	.string "Si narra che fu Rayquaza a placare\n"
	.string "le due Ancore e a ricucire il cielo,\l"
	.string "in un passato lontanissimo.\p"
	.string "Solo il Custode della Volta può\n"
	.string "fermarle di nuovo. Ma nemmeno io so\l"
	.string "dove dorma Rayquaza…$"

CaveOfOrigin_B1F_Text_WhereIsRayquaza:
	.string "Adriano: {PLAYER}{KUN}, sai per caso\n"
	.string "dove si trovi Rayquaza adesso?\p"
	.string "Se lo sai, ti prego, dimmelo.$"

CaveOfOrigin_B1F_Text_ButWereInCaveOfOrigin:
	.string "Adriano: La Grotta dei Tempi?\p"
	.string "Ma è proprio qui dove siamo!\n"
	.string "Mi serve qualcosa di meglio!\p"
	.string "Ti prego, pensa bene a dove potrebbe\n"
	.string "trovarsi Rayquaza in questo momento.$"

CaveOfOrigin_B1F_Text_OldLadyDidntMentionThat:
	.string "Adriano: Il Monte Pira?\p"
	.string "Ma quando ho parlato con l'anziana\n"
	.string "Custode, poco fa, non ne ha fatto\l"
	.string "parola.\p"
	.string "Dubito molto che la Custode possa\n"
	.string "nascondermi qualcosa…\p"
	.string "{PLAYER}{KUN}, potresti pensarci\n"
	.string "con più attenzione?$"

CaveOfOrigin_B1F_Text_CantYouRememberSomehow:
	.string "Adriano: Eh? Non te lo ricordi?\n"
	.string "Mmh… Questo è un bel problema…\p"
	.string "Non riesci proprio a ricordare?$"

CaveOfOrigin_B1F_Text_WellHeadToSkyPillar:
	.string "Adriano: La Torre dei Cieli?\p"
	.string "Ma certo!\n"
	.string "Dev'essere la Torre dei Cieli!\p"
	.string "{PLAYER}{KUN}, non c'è tempo da perdere!\n"
	.string "Corriamo subito alla Torre dei Cieli!$"

