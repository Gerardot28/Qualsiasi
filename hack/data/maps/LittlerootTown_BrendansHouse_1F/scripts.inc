LittlerootTown_BrendansHouse_1F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, LittlerootTown_BrendansHouse_1F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_BrendansHouse_1F_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_BrendansHouse_1F_OnFrame
	.byte 0

LittlerootTown_BrendansHouse_1F_OnLoad:
	call_if_lt VAR_LITTLEROOT_INTRO_STATE, 6, LittlerootTown_BrendansHouse_1F_EventScript_SetMovingBoxes
	call_if_set FLAG_RECEIVED_RUNNING_SHOES, LittlerootTown_BrendansHouse_1F_EventScript_CheckShowShoesManual
	end

LittlerootTown_BrendansHouse_1F_EventScript_SetMovingBoxes::
	setmetatile 5, 4, METATILE_BrendansMaysHouse_MovingBox_Open, TRUE
	setmetatile 5, 2, METATILE_BrendansMaysHouse_MovingBox_Closed, TRUE
	return

LittlerootTown_BrendansHouse_1F_EventScript_CheckShowShoesManual::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_BrendansHouse_1F_EventScript_ShowRunningShoesManual
	return

LittlerootTown_BrendansHouse_1F_EventScript_ShowRunningShoesManual::
	setmetatile 3, 7, METATILE_BrendansMaysHouse_BookOnTable, TRUE
	return

LittlerootTown_BrendansHouse_1F_OnTransition:
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 3, LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToDoor
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 5, LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToStairs
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 6, LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToTV
	end

LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToStairs::
	setobjectxyperm LOCALID_PLAYERS_HOUSE_1F_MOM, 8, 4
	setobjectmovementtype LOCALID_PLAYERS_HOUSE_1F_MOM, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToTV::
	setobjectxyperm LOCALID_PLAYERS_HOUSE_1F_MOM, 4, 5
	setobjectmovementtype LOCALID_PLAYERS_HOUSE_1F_MOM, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_BrendansHouse_1F_EventScript_MoveMomToDoor::
	setobjectxyperm LOCALID_PLAYERS_HOUSE_1F_MOM, 9, 8
	setobjectmovementtype LOCALID_PLAYERS_HOUSE_1F_MOM, MOVEMENT_TYPE_FACE_UP
	return

@ Many of the below scripts have no gender check because they assume youre in the correct house
@ The below SS Ticket script uses Mays house state by accident(?), but theyre both set identically after the intro
LittlerootTown_BrendansHouse_1F_OnFrame:
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 3, LittlerootTown_BrendansHouse_1F_EventScript_EnterHouseMovingIn
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 5, LittlerootTown_BrendansHouse_1F_EventScript_GoUpstairsToSetClock
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 6, LittlerootTown_BrendansHouse_1F_EventScript_PetalburgGymReport
	map_script_2 VAR_LITTLEROOT_HOUSES_STATE_MAY, 1, LittlerootTown_BrendansHouse_1F_EventScript_YoureNewNeighbor
	map_script_2 VAR_LITTLEROOT_HOUSES_STATE_MAY, 3, PlayersHouse_1F_EventScript_GetSSTicketAndSeeLatiTV
	.2byte 0

LittlerootTown_BrendansHouse_1F_EventScript_GoUpstairsToSetClock::
	lockall
	msgbox PlayersHouse_1F_Text_GoSetTheClock, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PushTowardStairs
	applymovement LOCALID_PLAYERS_HOUSE_1F_MOM, LittlerootTown_BrendansHouse_1F_Movement_PushTowardStairs
	waitmovement 0
	warp MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F, 7, 1
	waitstate
	releaseall
	end

LittlerootTown_BrendansHouse_1F_Movement_PushTowardStairs:
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_EventScript_EnterHouseMovingIn::
	lockall
	setvar VAR_0x8004, LOCALID_PLAYERS_HOUSE_1F_MOM
	setvar VAR_0x8005, MALE
	goto PlayersHouse_1F_EventScript_EnterHouseMovingIn
	end

LittlerootTown_BrendansHouse_1F_EventScript_PetalburgGymReport::
	lockall
	setvar VAR_0x8004, MALE
	setvar VAR_0x8005, LOCALID_PLAYERS_HOUSE_1F_MOM
	goto PlayersHouse_1F_EventScript_PetalburgGymReportMale
	end

LittlerootTown_BrendansHouse_1F_EventScript_YoureNewNeighbor::
	lockall
	playse SE_PIN
	applymovement LOCALID_RIVALS_HOUSE_1F_MOM, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_RIVALS_HOUSE_1F_MOM, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_RIVALS_HOUSE_1F_MOM, LittlerootTown_BrendansHouse_1F_Movement_RivalMomApproach
	waitmovement 0
	special GetRivalSonDaughterString
	msgbox RivalsHouse_1F_Text_OhYoureTheNewNeighbor, MSGBOX_DEFAULT
	setflag FLAG_MET_RIVAL_MOM
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 2
	releaseall
	end

LittlerootTown_BrendansHouse_1F_Movement_RivalMomApproach:
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

LittlerootTown_BrendansHouse_1F_EventScript_GoSeeRoom::
	lockall
	setvar VAR_0x8004, LOCALID_PLAYERS_HOUSE_1F_MOM
	setvar VAR_0x8005, MALE
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	goto PlayersHouse_1F_EventScript_MomGoSeeRoom
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival0::
	lockall
	setvar VAR_0x8008, 0
	goto LittlerootTown_BrendansHouse_1F_EventScript_MeetRival
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival1::
	lockall
	setvar VAR_0x8008, 1
	goto LittlerootTown_BrendansHouse_1F_EventScript_MeetRival
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival2::
	lockall
	setvar VAR_0x8008, 2
	goto LittlerootTown_BrendansHouse_1F_EventScript_MeetRival
	end

LittlerootTown_BrendansHouse_1F_EventScript_MeetRival::
	playse SE_EXIT
	delay 10
	addobject LOCALID_RIVALS_HOUSE_1F_RIVAL
	delay 30
	playse SE_PIN
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, Common_Movement_Delay48
	waitmovement 0
	call_if_ne VAR_0x8008, 1, LittlerootTown_BrendansHouse_1F_EventScript_PlayerFaceBrendan
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	call_if_eq VAR_0x8008, 0, LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer0
	call_if_eq VAR_0x8008, 1, LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer1
	call_if_eq VAR_0x8008, 2, LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer2
	msgbox RivalsHouse_1F_Text_BrendanWhoAreYou, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_0x8008, 0, LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs0
	call_if_eq VAR_0x8008, 1, LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs1
	call_if_eq VAR_0x8008, 2, LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs2
	playse SE_EXIT
	removeobject LOCALID_RIVALS_HOUSE_1F_RIVAL
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_BRENDAN
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_POKE_BALL
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	delay 30
	setvar VAR_LITTLEROOT_RIVAL_STATE, 3
	setvar VAR_LITTLEROOT_TOWN_STATE, 1
	savebgm MUS_DUMMY
	fadedefaultbgm
	releaseall
	end

LittlerootTown_BrendansHouse_1F_EventScript_PlayerFaceBrendan::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer0::
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer0
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer1::
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer1
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanApproachPlayer2::
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer2
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer0:
	walk_in_place_faster_left
	walk_left
	walk_in_place_faster_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer1:
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanApproachPlayer2:
	walk_in_place_faster_right
	walk_right
	walk_in_place_faster_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs0::
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit0
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs0
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs1::
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit1
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs1
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_EventScript_BrendanGoUpstairs2::
	applymovement LOCALID_PLAYER, LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit2
	applymovement LOCALID_RIVALS_HOUSE_1F_RIVAL, LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs2
	waitmovement 0
	return

LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit0:
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit1:
	delay_16
	delay_8
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_in_place_faster_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_PlayerWatchBrendanExit2:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs0:
	walk_in_place_faster_right
	walk_right
	walk_in_place_faster_up
	walk_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs1:
	walk_in_place_faster_right
	walk_right
	walk_in_place_faster_up
	walk_up
	walk_up
	walk_in_place_faster_left
	walk_left
	walk_in_place_faster_up
	walk_up
	step_end

LittlerootTown_BrendansHouse_1F_Movement_BrendanGoUpstairs2:
	walk_in_place_faster_left
	walk_left
	walk_in_place_faster_up
	walk_up
	walk_up
	step_end

PlayersHouse_1F_Text_IsntItNiceInHere:
	.string "Mamma: Visto, {PLAYER}?\n"
	.string "Non è carino anche qui dentro?$"

PlayersHouse_1F_Text_MoversPokemonGoSetClock:
	.string "I Pokémon della ditta di traslochi\n"
	.string "portano dentro tutto e poi puliscono\l"
	.string "pure! Che comodità!\p"
	.string "{PLAYER}, la tua stanza è di sopra.\n"
	.string "Vai a darle un'occhiata, tesoro!\p"
	.string "Papà ti ha comprato un orologio nuovo\n"
	.string "per festeggiare il trasloco.\l"
	.string "Non dimenticarti di regolarlo!$"

PlayersHouse_1F_Text_ArentYouInterestedInRoom:
	.string "Mamma: Allora, {PLAYER}?\p"
	.string "Non sei curioso di vedere la tua\n"
	.string "stanza nuova?$"

PlayersHouse_1F_Text_GoSetTheClock:
	.string "Mamma: {PLAYER}.\p"
	.string "Vai a regolare l'orologio in camera\n"
	.string "tua, tesoro.$"

PlayersHouse_1F_Text_OhComeQuickly:
	.string "Mamma: Oh! {PLAYER}, {PLAYER}!\n"
	.string "Presto! Vieni, presto!$"

PlayersHouse_1F_Text_MaybeDadWillBeOn:
	.string "Mamma: Guarda! È la Palestra di\n"
	.string "Petalipoli! Magari c'è anche papà!$"

PlayersHouse_1F_Text_ItsOverWeMissedHim:
	.string "Mamma: Oh… È già finito.\p"
	.string "Credo che papà fosse in onda, ma\n"
	.string "l'abbiamo perso. Peccato.$"

PlayersHouse_1F_Text_GoIntroduceYourselfNextDoor:
	.string "Ah, giusto!\n"
	.string "Un amico di papà abita qui in paese.\p"
	.string "Si chiama Prof. Birch, e studia i Varchi,\n"
	.string "quelle stranezze di cui parlano in TV.\p"
	.string "Abita proprio qui accanto: dovresti\n"
	.string "andare a presentarti.$"

PlayersHouse_1F_Text_SeeYouHoney:
	.string "Mamma: A dopo, tesoro!$"

PlayersHouse_1F_Text_DidYouMeetProfBirch:
	.string "Mamma: Ti sei presentato al\n"
	.string "Prof. Birch?$"

PlayersHouse_1F_Text_YouShouldRestABit:
	.string "Mamma: Come va, {PLAYER}?\n"
	.string "Mi sembri un po' stanco.\p"
	.string "Riposati un attimo… e mangia\n"
	.string "qualcosa!$"

PlayersHouse_1F_Text_TakeCareHoney:
	.string "Mamma: Stai attento, tesoro!$"

PlayersHouse_1F_Text_GotDadsBadgeHeresSomethingFromMom:
	.string "Mamma: Oh? Quella è la Medaglia di papà?\p"
	.string "Allora questo è un regalino da parte\n"
	.string "della tua mamma!$"

PlayersHouse_1F_Text_DontPushYourselfTooHard:
	.string "Non strafare, mi raccomando.\n"
	.string "Puoi sempre tornare a casa.\p"
	.string "Forza, tesoro!$"

PlayersHouse_1F_Text_IsThatAPokenav:
	.string "Mamma: Cos'è quello, tesoro? Un PokéNav?\n"
	.string "Te l'ha dato qualcuno della Devon?\p"
	.string "Bene! Allora che ne dici di registrare\n"
	.string "anche il numero della tua mamma?\p"
	.string "… … …$"

PlayersHouse_1F_Text_RegisteredMom:
	.string "Mamma è stata registrata\n"
	.string "nel PokéNav.$"

PlayersHouse_1F_Text_Vigoroth1:
	.string "Fughiiiiih!$"

PlayersHouse_1F_Text_Vigoroth2:
	.string "Uggoh, uggo uggo…$"

PlayersHouse_1F_Text_ReportFromPetalburgGym:
	.string "Giornalista: …e nessuno sa ancora\n"
	.string "spiegare gli strani bagliori visti\l"
	.string "sul Percorso 102.\p"
	.string "Dalla Palestra di Petalipoli è tutto.\n"
	.string "A voi lo studio!$"

PlayersHouse_1F_Text_TheresAMovieOnTV:
	.string "In TV danno un film.\p"
	.string "Due uomini ballano su un'enorme\n"
	.string "tastiera di pianoforte.\p"
	.string "Meglio darsi una mossa!$"

PlayersHouse_1F_Text_RunningShoesManual:
	.string "È il libretto di istruzioni delle\n"
	.string "Scarpe da Corsa.\p"
	.string "“Per correre, premi il pulsante B\n"
	.string "mentre indossi le Scarpe da Corsa.\p"
	.string "“Allaccia le Scarpe da Corsa e\n"
	.string "divora la strada!”$"

PlayersHouse_1F_Text_TicketFromBrineyCameForYou:
	.string "Papà: Hm?\p"
	.string "Ah, sei tu, {PLAYER}.\p"
	.string "…Campione.\n"
	.string "Ehm, ehm! Scusa, un po' di tosse.\p"
	.string "Ti trovo… più forte, in qualche modo.\n"
	.string "Si vede anche da come cammini.\p"
	.string "Ma il tuo vecchio non ha ancora detto\n"
	.string "l'ultima parola, sappilo!\p"
	.string "Ah, giusto, ho una cosa per te.\n"
	.string "Te la manda Capitan Scoglio.$"

PlayersHouse_1F_Text_PortsInSlateportLilycove:
	.string "Papà: È un biglietto per la M/N Acqua.\p"
	.string "Se non sbaglio, il traghetto parte da\n"
	.string "Porto Selcepoli e da Porto Alghepoli.$"

PlayersHouse_1F_Text_BetterGetBackToGym:
	.string "Devo tornare alla Palestra di\n"
	.string "Petalipoli. Qualcuno dovrà pur\l"
	.string "batterti, prima o poi.\p"
	.string "Cara, grazie per badare alla casa\n"
	.string "mentre sono via.$"

PlayersHouse_1F_Text_DadShouldStayLonger:
	.string "Mamma: Quel papà che ti ritrovi…\p"
	.string "Torna a casa dopo tanto tempo e non\n"
	.string "fa che parlare di Pokémon.\p"
	.string "Potrebbe rilassarsi e fermarsi\n"
	.string "un po' di più, no?$"

PlayersHouse_1F_Text_IsThatABreakingStory:
	.string "Mamma: È un'edizione straordinaria?$"

PlayersHouse_1F_Text_LatiEmergencyNewsFlash:
	.string "Interrompiamo le trasmissioni per\n"
	.string "un'edizione straordinaria!\p"
	.string "In diverse zone di Hoenn è stato\n"
	.string "avvistato un Pokémon di colore\l"
	.string "BZZT… in volo.\p"
	.string "C'è chi dice che sia sbucato dal\n"
	.string "Grande Varco, e chi giura di averlo\l"
	.string "già visto anni fa.\p"
	.string "Al momento l'identità di questo\n"
	.string "Pokémon resta sconosciuta.\p"
	.string "Riprendiamo ora la normale\n"
	.string "programmazione.$"

PlayersHouse_1F_Text_WhatColorDidTheySay:
	.string "Mamma: {PLAYER}, hai sentito?\p"
	.string "Di che colore ha detto che era\n"
	.string "quel Pokémon?$"

PlayersHouse_1F_Text_StillUnknownPokemon:
	.string "Mamma: Ma pensa un po'!\n"
	.string "Esistono ancora Pokémon sconosciuti.\p"
	.string "Con tutti questi Varchi, chissà quante\n"
	.string "altre sorprese ci aspettano!$"
