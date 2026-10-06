VictoryRoad_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VictoryRoad_1F_OnTransition
	.byte 0

VictoryRoad_1F_OnTransition:
	call_if_eq VAR_VICTORY_ROAD_1F_STATE, 1, VictoryRoad_1F_EventScript_SetEntranceWallyPos1
	call_if_eq VAR_VICTORY_ROAD_1F_STATE, 2, VictoryRoad_1F_EventScript_SetEntranceWallyPos2
	end

VictoryRoad_1F_EventScript_SetEntranceWallyPos1::
	setobjectxyperm LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, 2, 24
	setobjectmovementtype LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, MOVEMENT_TYPE_FACE_DOWN
	return

VictoryRoad_1F_EventScript_SetEntranceWallyPos2::
	setobjectxyperm LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, 3, 24
	setobjectmovementtype LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, MOVEMENT_TYPE_FACE_DOWN
	return

VictoryRoad_1F_EventScript_WallyBattleTrigger1::
	lockall
	setvar VAR_0x8008, 1
	addobject LOCALID_VICTORY_ROAD_ENTRANCE_WALLY
	applymovement LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, VictoryRoad_1F_Movement_WallyApproachPlayer1
	waitmovement 0
	goto VictoryRoad_1F_EventScript_WallyEntranceBattle
	end

VictoryRoad_1F_EventScript_WallyBattleTrigger2::
	lockall
	setvar VAR_0x8008, 2
	addobject LOCALID_VICTORY_ROAD_ENTRANCE_WALLY
	applymovement LOCALID_VICTORY_ROAD_ENTRANCE_WALLY, VictoryRoad_1F_Movement_WallyApproachPlayer2
	waitmovement 0
	goto VictoryRoad_1F_EventScript_WallyEntranceBattle
	end

VictoryRoad_1F_EventScript_WallyEntranceBattle::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox VictoryRoad_1F_Text_WallyNotGoingToLoseAnymore, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLY_VR_1, VictoryRoad_1F_Text_WallyEntranceDefeat
	msgbox VictoryRoad_1F_Text_WallyPostEntranceBattle, MSGBOX_DEFAULT
	clearflag FLAG_HIDE_VICTORY_ROAD_ENTRANCE_WALLY
	copyobjectxytoperm LOCALID_VICTORY_ROAD_ENTRANCE_WALLY
	setflag FLAG_DEFEATED_WALLY_VICTORY_ROAD
	copyvar VAR_VICTORY_ROAD_1F_STATE, VAR_0x8008
	releaseall
	end

VictoryRoad_1F_Movement_WallyApproachPlayer1:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

VictoryRoad_1F_Movement_WallyApproachPlayer2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

@ This Wally appears near the entrance once his battle is triggered and remains there until the Hall of Fame is entered
VictoryRoad_1F_EventScript_EntranceWally::
	msgbox VictoryRoad_1F_Text_WallyPostEntranceBattle, MSGBOX_NPC
	end

@ This Wally appears and remains at the exit after the Hall of Fame is entered
VictoryRoad_1F_EventScript_ExitWally::
	trainerbattle_single TRAINER_WALLY_VR_2, VictoryRoad_1F_Text_WallyIntro, VictoryRoad_1F_Text_WallyDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_1F_EventScript_RematchWally
	msgbox VictoryRoad_1F_Text_WallyPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_RematchWally::
	trainerbattle_rematch TRAINER_WALLY_VR_2, VictoryRoad_1F_Text_WallyIntro, VictoryRoad_1F_Text_WallyDefeat
	msgbox VictoryRoad_1F_Text_WallyPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Edgar::
	trainerbattle_single TRAINER_EDGAR, VictoryRoad_1F_Text_EdgarIntro, VictoryRoad_1F_Text_EdgarDefeat
	msgbox VictoryRoad_1F_Text_EdgarPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Albert::
	trainerbattle_single TRAINER_ALBERT, VictoryRoad_1F_Text_AlbertIntro, VictoryRoad_1F_Text_AlbertDefeat
	msgbox VictoryRoad_1F_Text_AlbertPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Hope::
	trainerbattle_single TRAINER_HOPE, VictoryRoad_1F_Text_HopeIntro, VictoryRoad_1F_Text_HopeDefeat
	msgbox VictoryRoad_1F_Text_HopePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Quincy::
	trainerbattle_single TRAINER_QUINCY, VictoryRoad_1F_Text_QuincyIntro, VictoryRoad_1F_Text_QuincyDefeat
	msgbox VictoryRoad_1F_Text_QuincyPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_EventScript_Katelynn::
	trainerbattle_single TRAINER_KATELYNN, VictoryRoad_1F_Text_KatelynnIntro, VictoryRoad_1F_Text_KatelynnDefeat
	msgbox VictoryRoad_1F_Text_KatelynnPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_1F_Text_WallyNotGoingToLoseAnymore:
	.string "Lino: {PLAYER}! Ciao!\p"
	.string "Scommetto che non ti aspettavi\n"
	.string "di trovarmi proprio qui, vero?\p"
	.string "Io… ce l'ho fatta ad arrivare fin qui.\n"
	.string "E lo devo tutto a te.\p"
	.string "{PLAYER}, quella volta a Ciclamipoli\n"
	.string "ho perso… ma mi ha reso più forte.\p"
	.string "Adesso voglio vedere se posso\n"
	.string "batterti. Non ho più intenzione\l"
	.string "di perdere!\p"
	.string "Vincerò! Per i Pokémon che mi hanno\n"
	.string "dato coraggio e forza!\p"
	.string "Va bene… Arrivo!$"

VictoryRoad_1F_Text_WallyEntranceDefeat:
	.string "Ah…!\n"
	.string "{PLAYER}, sei davvero fortissimo!$"

VictoryRoad_1F_Text_WallyPostEntranceBattle:
	.string "Lino: Oggi non ce l'ho fatta,\n"
	.string "{PLAYER}… Ma un giorno ti\l"
	.string "raggiungerò, vedrai.\p"
	.string "E… grazie. Senza di te non avrei\n"
	.string "mai preso in mano una Poké Ball.$"

VictoryRoad_1F_Text_WallyIntro:
	.string "Lino: {PLAYER}! Ciao!\p"
	.string "Dall'ultima volta sono diventato\n"
	.string "ancora più forte! Volevo fartelo\l"
	.string "vedere, {PLAYER}!\p"
	.string "Va bene… Arrivo!$"

VictoryRoad_1F_Text_WallyDefeat:
	.string "Ah…!\n"
	.string "{PLAYER}, sei sempre fortissimo!$"

VictoryRoad_1F_Text_WallyPostBattle:
	.string "Lino: Neanche stavolta ce l'ho fatta…\n"
	.string "Ma un giorno, {PLAYER}, ti\l"
	.string "raggiungerò…\p"
	.string "E poi sfiderò la Lega Pokémon!\n"
	.string "Io… ce la posso fare.$"

VictoryRoad_1F_Text_EdgarIntro:
	.string "Sono arrivato fin qui già due volte,\n"
	.string "ma l'ultimo tratto non finisce mai…$"

VictoryRoad_1F_Text_EdgarDefeat:
	.string "Il mio sogno finisce qui…\n"
	.string "di nuovo…$"

VictoryRoad_1F_Text_EdgarPostBattle:
	.string "Ormai sei arrivato fin qui. Non\n"
	.string "fermarti proprio adesso e diventa\l"
	.string "Campione! Se qualcuno può farcela,\l"
	.string "quello sei tu!$"

VictoryRoad_1F_Text_AlbertIntro:
	.string "Non ho fatto tutta questa strada\n"
	.string "per perdere adesso. Non esiste!$"

VictoryRoad_1F_Text_AlbertDefeat:
	.string "Impossibile…\n"
	.string "Ho perso?$"

VictoryRoad_1F_Text_AlbertPostBattle:
	.string "Ho perso proprio qui…\p"
	.string "Vuol dire che non ho ancora\n"
	.string "la stoffa del Campione…$"

VictoryRoad_1F_Text_HopeIntro:
	.string "Questa strada dura e senza fine…\n"
	.string "se lo merita, il nome di Vittoria.$"

VictoryRoad_1F_Text_HopeDefeat:
	.string "Hai uno stile di lotta\n"
	.string "semplicemente fantastico…$"

VictoryRoad_1F_Text_HopePostBattle:
	.string "Hai tutta la stoffa per diventare\n"
	.string "Campione, lo sento.$"

VictoryRoad_1F_Text_QuincyIntro:
	.string "Che cos'è la Via Vittoria?\n"
	.string "Te lo dico se vinci!$"

VictoryRoad_1F_Text_QuincyDefeat:
	.string "D'accordo!\n"
	.string "Ben fatto!$"

VictoryRoad_1F_Text_QuincyPostBattle:
	.string "Uscire sani e salvi da qui è l'ultima\n"
	.string "prova per ogni Allenatore che sogna\l"
	.string "di diventare Campione.\p"
	.string "Per questo si chiama Via Vittoria.\n"
	.string "E con i Varchi che a volte si aprono\l"
	.string "tra queste rocce, è ancora più dura!$"

VictoryRoad_1F_Text_KatelynnIntro:
	.string "Non ho niente da dire a chi è\n"
	.string "arrivato fin qui. Fatti sotto!$"

VictoryRoad_1F_Text_KatelynnDefeat:
	.string "Che figuraccia…$"

VictoryRoad_1F_Text_KatelynnPostBattle:
	.string "Hmpf, prosegui pure.\n"
	.string "Tanto a me che importa.$"
