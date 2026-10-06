TrainerHill_Entrance_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, TrainerHill_Entrance_OnResume
	map_script MAP_SCRIPT_ON_RETURN_TO_FIELD, TrainerHill_Entrance_OnReturn
	map_script MAP_SCRIPT_ON_TRANSITION, TrainerHill_Entrance_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, TrainerHill_Entrance_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, TrainerHill_Entrance_OnFrame
	.byte 0

TrainerHill_Entrance_OnTransition:
	setflag FLAG_LANDMARK_TRAINER_HILL
	getplayerxy VAR_TEMP_D, VAR_RESULT
	end

@ Unused
TrainerHill_Entrance_OnWarp:
	end

TrainerHill_Entrance_OnResume:
	trainerhill_resumetimer
	setvar VAR_TEMP_0, 0
	trainerhill_getusingereader
	goto_if_eq VAR_RESULT, FALSE, TrainerHill_Entrance_EventScript_TryFaceAttendant  @ VAR_RESULT always FALSE here
	setobjectxy LOCALID_PLAYER, 9, 6
	applymovement LOCALID_PLAYER, TrainerHill_Entrance_Movement_PlayerFaceAttendant
	end

TrainerHill_Entrance_EventScript_TryFaceAttendant::
	trainerhill_getwon
	goto_if_eq VAR_RESULT, TRUE, TrainerHill_Entrance_EventScript_PlayerDontFaceAttendant
	applymovement LOCALID_PLAYER, TrainerHill_Entrance_Movement_PlayerFaceAttendant
	end

TrainerHill_Entrance_EventScript_PlayerDontFaceAttendant::
	end

TrainerHill_Entrance_OnReturn:
	addobject LOCALID_TRAINER_HILL_NURSE
	addobject LOCALID_TRAINER_HILL_ATTENDANT
	addobject LOCALID_TRAINER_HILL_MAN
	addobject LOCALID_TRAINER_HILL_GIRL
	end

TrainerHill_Entrance_OnLoad:
	call_if_eq VAR_TEMP_D, 17, TrainerHill_Entrance_EventScript_OpenCounterDoor
	end

TrainerHill_Entrance_EventScript_OpenCounterDoor::
	setmetatile 17, 10, METATILE_TrainerHill_GreenFloorTile, FALSE
	return

TrainerHill_Entrance_OnFrame:
	map_script_2 VAR_TEMP_0, 0, TrainerHill_Entrance_EventScript_ExitChallenge
	map_script_2 VAR_TEMP_D, 17, TrainerHill_Entrance_EventScript_ExitElevator
	map_script_2 VAR_TEMP_5, 1, TrainerHill_Entrance_EventScript_EntryTrigger
	map_script_2 VAR_TEMP_1, 1, TrainerHill_EventScript_WarpToEntranceCounter
	.2byte 0

TrainerHill_Entrance_EventScript_ExitElevator::
	lockall
	applymovement LOCALID_PLAYER, TrainerHill_Entrance_Movement_PlayerExitElevator
	waitmovement 0
	setmetatile 17, 10, METATILE_TrainerHill_CounterDoor, TRUE
	special DrawWholeMapView
	playse SE_CLICK
	waitse
	setvar VAR_TEMP_D, 0
	releaseall
	end

TrainerHill_Entrance_EventScript_ExitChallenge::
	setvar VAR_TEMP_0, 1
	trainerhill_getstatus
	switch VAR_RESULT
	case TRAINER_HILL_PLAYER_STATUS_LOST, TrainerHill_Entrance_EventScript_ExitChallengeLost
	case TRAINER_HILL_PLAYER_STATUS_ECARD_SCANNED, TrainerHill_Entrance_EventScript_ExitChallengeECard
	case TRAINER_HILL_PLAYER_STATUS_NORMAL, TrainerHill_Entrance_EventScript_EndExitChallenge

TrainerHill_Entrance_EventScript_ExitChallengeLost::
	lockall
	applymovement LOCALID_PLAYER, TrainerHill_Entrance_Movement_PlayerFaceAttendant
	msgbox TrainerHill_Entrance_Text_TooBadTremendousEffort, MSGBOX_DEFAULT
	goto TrainerHill_Entrance_EventScript_PlayerExitChallenge

TrainerHill_Entrance_EventScript_ExitChallengeECard::
	lockall
	applymovement LOCALID_PLAYER, TrainerHill_Entrance_Movement_PlayerFaceAttendant
	msgbox TrainerHill_Entrance_Text_MovedReceptionHereForSwitch, MSGBOX_DEFAULT
TrainerHill_Entrance_EventScript_PlayerExitChallenge::
	closemessage
	applymovement LOCALID_PLAYER, TrainerHill_Entrance_Movement_PushPlayerBackFromCounter
	waitmovement 0
	setvar VAR_TRAINER_HILL_IS_ACTIVE, 0
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	releaseall
TrainerHill_Entrance_EventScript_EndExitChallenge::
	end

TrainerHill_Entrance_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_TRAINER_HILL_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

TrainerHill_Entrance_EventScript_Attendant::
	lock
	faceplayer
	trainerhill_inchallenge
	goto_if_eq VAR_RESULT, FALSE, TrainerHill_Entrance_EventScript_ThanksForPlaying
	msgbox TrainerHill_Entrance_Text_HopeYouGiveItYourBest, MSGBOX_DEFAULT
	goto TrainerHill_Entrance_EventScript_AttendantEnd

TrainerHill_Entrance_EventScript_ThanksForPlaying::
	msgbox TrainerHill_Entrance_Text_ThankYouForPlaying, MSGBOX_DEFAULT
TrainerHill_Entrance_EventScript_AttendantEnd::
	release
	end

TrainerHill_Entrance_EventScript_EntryTrigger::
	lockall
	applymovement LOCALID_PLAYER, TrainerHill_Entrance_Movement_PlayerFaceAttendant
	goto_if_unset FLAG_SYS_GAME_CLEAR, TrainerHill_Entrance_EventScript_Closed
	msgbox TrainerHill_Entrance_Text_WelcomeToTrainerHill, MSGBOX_DEFAULT
	trainerhill_getsaved
	call_if_eq VAR_RESULT, FALSE, TrainerHill_Entrance_EventScript_SaveGame
	trainerhill_allfloorsused
	goto_if_eq VAR_RESULT, TRUE, TrainerHill_Entrance_EventScript_AllFloorsUsed
	msgbox TrainerHill_Entrance_Text_TrainersUpToFloorX, MSGBOX_DEFAULT
	goto TrainerHill_Entrance_EventScript_AskChallengeTrainers

TrainerHill_Entrance_EventScript_AllFloorsUsed::
	msgbox TrainerHill_Entrance_Text_TrainersInEveryRoom, MSGBOX_DEFAULT
TrainerHill_Entrance_EventScript_AskChallengeTrainers::
	message TrainerHill_Entrance_Text_LikeToChallengeTrainers
	waitmessage
	multichoice 15, 6, MULTI_YESNOINFO, FALSE
	switch VAR_RESULT
	case 0, TrainerHill_Entrance_EventScript_ChooseChallenge
	case 1, TrainerHill_Entrance_EventScript_CancelEntry
	case 2, TrainerHill_Entrance_EventScript_Info
	case MULTI_B_PRESSED, TrainerHill_Entrance_EventScript_CancelEntry
	end

TrainerHill_Entrance_EventScript_Info::
	msgbox TrainerHill_Entrance_Text_ExplainTrainerHill, MSGBOX_DEFAULT
	goto TrainerHill_Entrance_EventScript_AskChallengeTrainers
	end

TrainerHill_Entrance_EventScript_ChooseChallenge::
	multichoice 13, 2, MULTI_TAG_MATCH_TYPE, FALSE
	switch VAR_RESULT
	case 4, TrainerHill_Entrance_EventScript_CancelEntry
	case MULTI_B_PRESSED, TrainerHill_Entrance_EventScript_CancelEntry
	trainerhill_setmode VAR_RESULT
	setvar VAR_TRAINER_HILL_IS_ACTIVE, 1
	setvar VAR_TEMP_5, 0
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	msgbox TrainerHill_Entrance_Text_TimeProgessGetSetGo, MSGBOX_DEFAULT
	trainerhill_start
	releaseall
	end

TrainerHill_Entrance_EventScript_CancelEntry::
	setvar VAR_TEMP_5, 0
	msgbox TrainerHill_Entrance_Text_PleaseVisitUsAgain, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, TrainerHill_Entrance_Movement_PushPlayerBackFromCounter
	waitmovement 0
	releaseall
	end

TrainerHill_Entrance_EventScript_SaveGame::
	msgbox TrainerHill_Entrance_Text_SaveGameBeforeEnter, MSGBOX_DEFAULT
	trainerhill_setsaved
	setvar VAR_TEMP_5, 1
	call Common_EventScript_SaveGame
	goto_if_eq VAR_RESULT, FALSE, TrainerHill_Entrance_EventScript_SaveFailed
	trainerhill_setsaved
	return

TrainerHill_Entrance_EventScript_SaveFailed::
	trainerhill_clearsaved
	goto TrainerHill_Entrance_EventScript_CancelEntry
	end

TrainerHill_Entrance_EventScript_Closed::
	msgbox TrainerHill_Entrance_Text_StillGettingReady, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, TrainerHill_Entrance_Movement_PushPlayerBackFromCounter
	waitmovement 0
	releaseall
	end

TrainerHill_Entrance_Movement_PlayerFaceAttendant:
	face_right
	step_end

TrainerHill_Entrance_Movement_PushPlayerBackFromCounter:
	walk_down
	step_end

@ Unused
TrainerHill_Entrance_Movement_FaceUp:
	face_up
	step_end

TrainerHill_Entrance_Movement_PlayerExitElevator:
	walk_down
	walk_down
	step_end

TrainerHill_Entrance_EventScript_Records::
	lockall
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, 0
	special ShowTrainerHillRecords
	releaseall
	end

TrainerHill_Entrance_EventScript_Man::
	goto_if_unset FLAG_SYS_GAME_CLEAR, TrainerHill_Entrance_EventScript_ManTrainerHillClosed
	msgbox TrainerHill_Entrance_Text_WhatSortOfTrainersAreAhead, MSGBOX_NPC
	end

TrainerHill_Entrance_EventScript_ManTrainerHillClosed::
	msgbox TrainerHill_Entrance_Text_CantWaitToTestTheWaters, MSGBOX_NPC
	end

TrainerHill_Entrance_EventScript_Girl::
	goto_if_unset FLAG_SYS_GAME_CLEAR, TrainerHill_Entrance_EventScript_GirlTrainerHillClosed
	msgbox TrainerHill_Entrance_Text_FriendsTryingToReachTimeBoardTop, MSGBOX_NPC
	end

TrainerHill_Entrance_EventScript_GirlTrainerHillClosed::
	msgbox TrainerHill_Entrance_Text_DoYouKnowWhenTheyOpen, MSGBOX_NPC
	end

TrainerHill_Entrance_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	goto_if_set FLAG_SYS_GAME_CLEAR, TrainerHill_Entrance_EventScript_ExpandedPokemart
	pokemart TrainerHill_Entrance_Pokemart_Basic
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
TrainerHill_Entrance_Pokemart_Basic:
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_SP_ATK
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	.2byte ITEM_DIRE_HIT
	.2byte ITEM_GUARD_SPEC
	.2byte ITEM_X_ACCURACY
	pokemartlistend

TrainerHill_Entrance_EventScript_ExpandedPokemart::
	pokemart TrainerHill_Entrance_Pokemart_Expanded
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
TrainerHill_Entrance_Pokemart_Expanded:
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_SP_ATK
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	.2byte ITEM_DIRE_HIT
	.2byte ITEM_GUARD_SPEC
	.2byte ITEM_X_ACCURACY
	pokemartlistend

TrainerHill_Entrance_Text_StillGettingReady:
	.string "Questo è il Colle Allenatori, dove\n"
	.string "puoi goderti tante lotte in coppia\l"
	.string "contro un sacco di Allenatori.\p"
	.string "Purtroppo stiamo ancora preparando\n"
	.string "tutto. Torna più avanti, per favore!$"

TrainerHill_Entrance_Text_WelcomeToTrainerHill:
	.string "Benvenuto!\p"
	.string "Questo è il Colle Allenatori, dove\n"
	.string "puoi goderti tante lotte in coppia\l"
	.string "contro un sacco di Allenatori!$"

TrainerHill_Entrance_Text_SaveGameBeforeEnter:
	.string "È la tua prima visita?\p"
	.string "Prima di entrare, per favore,\n"
	.string "salva la partita.$"

TrainerHill_Entrance_Text_TrainersUpToFloorX:
	.string "Vediamo…\n"
	.string "Al momento ci sono Allenatori…\p"
	.string "…fino al piano n. {STR_VAR_1}.$"

TrainerHill_Entrance_Text_TrainersInEveryRoom:
	.string "Vediamo…\n"
	.string "Al momento ci sono Allenatori…\p"
	.string "…radunati in ogni stanza!$"

TrainerHill_Entrance_Text_LikeToChallengeTrainers:
	.string "Vuoi sfidare gli Allenatori\n"
	.string "che ti aspettano?$"

TrainerHill_Entrance_Text_TimeProgessGetSetGo:
	.string "Cronometrerò la tua scalata.\n"
	.string "In bocca al lupo!\p"
	.string "Ai posti…\p"
	.string "Pronti…\p"
	.string "Via!$"

TrainerHill_Entrance_Text_PleaseVisitUsAgain:
	.string "Torna a trovarci presto!$"

TrainerHill_Entrance_Text_TooBadTremendousEffort:
	.string "Peccato!\p"
	.string "Però hai lottato con un impegno\n"
	.string "davvero straordinario.\p"
	.string "Torna a riprovarci, mi raccomando!$"

TrainerHill_Entrance_Text_HopeYouGiveItYourBest:
	.string "Mi raccomando, dai il massimo!$"

TrainerHill_Entrance_Text_MovedReceptionHereForSwitch:
	.string "Quando gli Allenatori si danno il\n"
	.string "cambio, c'è un gran trambusto.\p"
	.string "Per evitare la calca, abbiamo spostato\n"
	.string "qui il bancone della reception.\p"
	.string "Ci scusiamo per il disagio.$"

TrainerHill_Entrance_Text_ThankYouForPlaying:
	.string "Grazie per aver partecipato!$"

TrainerHill_Entrance_Text_ExplainTrainerHill:
	.string "Qui al Colle Allenatori organizziamo\n"
	.string "un evento chiamato Corsa a Tempo.\p"
	.string "È una gara che misura quanto ci metti\n"
	.string "ad arrivare da questo bancone fino al\l"
	.string "nostro proprietario, sul tetto.\p"
	.string "I tempi migliori finiscono sul\n"
	.string "Tabellone dei Tempi: sfida i tuoi\l"
	.string "amici a batterli!\p"
	.string "Nelle lotte contro gli Allenatori lungo\n"
	.string "la salita non si ricevono né Punti\l"
	.string "Esperienza né soldi.$"

@ Unused
TrainerHill_Entrance_Text_NeedAtLeastTwoPokemon:
	.string "Oh, mi dispiace, ma sembra che tu\n"
	.string "abbia con te un solo Pokémon.\p"
	.string "Per partecipare a questo evento\n"
	.string "servono almeno due Pokémon.$"

TrainerHill_Roof_Text_YouFinallyCameBravo:
	.string "Hm! Hm!\p"
	.string "Finalmente sei arrivato!\n"
	.string "Sì, sei proprio qui!\p"
	.string "Aspetta! Non dirlo!\n"
	.string "So perché sei salito fin quassù\l"
	.string "tutto da solo!\p"
	.string "Volevi vedere me, il proprietario\n"
	.string "del Colle Allenatori, perché…\p"
	.string "…vuoi fare coppia con me in lotta!\n"
	.string "Iuu-huuu!\p"
	.string "…Eh?\n"
	.string "Non è così?\p"
	.string "Comunque, ti ho seguito mentre salivi.\n"
	.string "Lotte meravigliose! Bravo, davvero!$"

TrainerHill_Roof_Text_HaveTheMostMarvelousGift:
	.string "Per una persona meravigliosa come te,\n"
	.string "ho il regalo più meraviglioso che ci sia!$"

TrainerHill_Roof_Text_FullUpBeBackLaterForThis:
	.string "Oh, no, sei pieno zeppo di roba!\n"
	.string "Tornerai più tardi a prenderlo!$"

TrainerHill_Roof_Text_GotHereMarvelouslyQuickly:
	.string "Oh, un momento! Non sarai per caso\n"
	.string "arrivato quassù a tempo di record?\p"
	.string "Splendido! Non c'era bisogno di\n"
	.string "correre così tanto per vedermi!\p"
	.string "Che delizia! Farò aggiornare il Tabellone\n"
	.string "dei Tempi alla reception!$"

TrainerHill_Roof_Text_YouWerentVeryQuick:
	.string "Però, oh…\n"
	.string "Non sei stato granché veloce.$"

TrainerHill_Roof_Text_ArriveZippierNextTime:
	.string "Forse mi faresti più contento se la\n"
	.string "prossima volta arrivassi più in fretta.\p"
	.string "Allora sì che sarei lieto di fare\n"
	.string "coppia con te!\p"
	.string "Alla prossima, amigo!$"

@ Unused
TrainerHill_Roof_Text_BuiltTrainerHillToFindPartner:
	.string "Ho fatto costruire il Colle Allenatori\n"
	.string "per un motivo, e uno soltanto!\p"
	.string "Trovare il compagno ideale con cui\n"
	.string "formare una coppia imbattibile!$"

TrainerHill_Entrance_Text_ChallengeTime:
	.string "{STR_VAR_1} min. {STR_VAR_2},{STR_VAR_3} sec.$"

TrainerHill_Entrance_Text_WhatSortOfTrainersAreAhead:
	.string "Chissà che razza di Allenatori e\n"
	.string "squadre di Pokémon mi aspettano?\p"
	.string "So solo che spazzerò via chiunque\n"
	.string "si metta sulla mia strada!$"

TrainerHill_Entrance_Text_CantWaitToTestTheWaters:
	.string "Ho sentito che al Colle Allenatori\n"
	.string "arrivano Allenatori forti da ogni dove.\p"
	.string "C'è chi giura di averne visto uno\n"
	.string "sbucare da un Varco!\p"
	.string "Non vedo l'ora di mettermi alla prova!\p"
	.string "Spazzerò via chiunque si metta\n"
	.string "sulla mia strada!$"

TrainerHill_Entrance_Text_FriendsTryingToReachTimeBoardTop:
	.string "Vedi il Tabellone dei Tempi laggiù?\p"
	.string "Io e i miei amici facciamo a gara\n"
	.string "a chi arriva in cima nel minor tempo.$"

TrainerHill_Entrance_Text_DoYouKnowWhenTheyOpen:
	.string "Sai quando apriranno questo posto?\p"
	.string "Aspetto qui per essere la prima\n"
	.string "sfidante in assoluto!$"

TrainerHill_Elevator_Text_ReturnToReception:
	.string "Vuoi tornare al bancone\n"
	.string "della reception?$"
