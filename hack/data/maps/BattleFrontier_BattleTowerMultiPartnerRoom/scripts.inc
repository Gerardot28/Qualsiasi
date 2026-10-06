BattleFrontier_BattleTowerMultiPartnerRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, BattleFrontier_BattleTowerMultiPartnerRoom_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_BattleTowerMultiPartnerRoom_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, BattleFrontier_BattleTowerMultiPartnerRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_BattleTowerMultiPartnerRoom_OnFrame
	.byte 0

BattleFrontier_BattleTowerMultiPartnerRoom_OnResume:
	pyramid_resetparty
	end

BattleFrontier_BattleTowerMultiPartnerRoom_OnTransition:
	goto_if_set FLAG_CHOSEN_MULTI_BATTLE_NPC_PARTNER, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ChosePartner
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_1
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_2
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_3
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_4
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_5
	clearflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_6
	setflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_ALT_1
	setflag FLAG_HIDE_BATTLE_TOWER_MULTI_BATTLE_PARTNER_ALT_2
	tower_loadpartners
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ChosePartner::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_1, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_2, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_3, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_4, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_5, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_6, OBJ_EVENT_GFX_BOY_1
	setvar VAR_OBJ_GFX_ID_7, OBJ_EVENT_GFX_BOY_1
	setvar VAR_TEMP_1, 1
	setvar VAR_TEMP_3, 1
	setobjectxyperm LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, 10, 2
	end

BattleFrontier_BattleTowerMultiPartnerRoom_OnWarp:
	map_script_2 VAR_TEMP_3, 1, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TurnPlayerNorth
	.2byte 0

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

BattleFrontier_BattleTowerMultiPartnerRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterRoom
	map_script_2 VAR_TEMP_3, 1, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ExitRoom
	.2byte 0

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_ExitRoom::
	lockall
	setvar VAR_TEMP_3, 0
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterElevator
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterRoom::
	lockall
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterRoom
	waitmovement 0
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantBlockExit
	waitmovement 0
	copyobjectxytoperm LOCALID_TOWER_PARTNER_ROOM_ATTENDANT
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner, MSGBOX_DEFAULT
	special HealPlayerParty
	setvar VAR_TEMP_1, 1
	releaseall
	end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterRoom:
	walk_down
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantBlockExit:
	walk_left
	walk_in_place_faster_down
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Attendant::
	lock
	faceplayer
	message BattleFrontier_BattleTowerMultiPartnerRoom_Text_QuitLookingForPartner
	waitmessage
	multichoicedefault 20, 8, MULTI_YESNO, 1, FALSE
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_QuitChallenge
	msgbox BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner2, MSGBOX_DEFAULT
	release
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_QuitChallenge::
	frontier_set FRONTIER_DATA_RECORD_DISABLED, TRUE
	goto BattleFrontier_BattleTower_EventScript_WarpToLobbyLost
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_EnterElevator::
	msgbox BattleFrontier_BattleTowerMultiPartnerRoom_Text_ThankYouForChoosingPartner, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	opendoor 10, 1
	waitdooranim
	call BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevator
	closedoor 10, 1
	waitdooranim
	warp MAP_BATTLE_FRONTIER_BATTLE_TOWER_ELEVATOR, 1, 6
	waitstate
	releaseall
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevator::
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevator
	waitmovement 0
	return

@ Unused
BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevatorEast:
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorEast
	waitmovement 0
	return

@ Unused
BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_MoveToElevatorWest:
	applymovement LOCALID_TOWER_PARTNER_ROOM_ATTENDANT, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator
	applymovement LOCALID_PLAYER, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorWest
	waitmovement 0
	return

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevator:
	walk_up
	walk_up
	set_invisible
	step_end

@ Functionally unused
BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorEast:
	walk_right
	walk_up
	set_invisible
	step_end

@ Functionally unused
BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PlayerEnterElevatorWest:
	walk_left
	walk_up
	set_invisible
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_AttendantEnterElevator:
	walk_up
	set_invisible
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner1::
	lock
	faceplayer
	setvar VAR_TEMP_2, 0
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner::
	goto_if_set FLAG_CHOSEN_MULTI_BATTLE_NPC_PARTNER, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner
	tower_dopartnermsg PARTNER_MSGID_INTRO
	waitmessage
	waitbuttonpress
	tower_dopartnermsg PARTNER_MSGID_MON1
	waitmessage
	waitbuttonpress
	tower_dopartnermsg PARTNER_MSGID_MON2_ASK
	waitmessage
	multichoicedefault 20, 8, MULTI_YESNO, 1, FALSE
	switch VAR_RESULT
	case 1, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner
	case MULTI_B_PRESSED, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner
	tower_dopartnermsg PARTNER_MSGID_ACCEPT
	waitmessage
	waitbuttonpress
	closemessage
	call_if_ne VAR_FACING, DIR_SOUTH, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExit
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExitSouth
	removeobject VAR_LAST_TALKED
	setflag FLAG_CHOSEN_MULTI_BATTLE_NPC_PARTNER
	warpsilent MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_PARTNER_ROOM, 10, 3
	waitstate
	release
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_RejectPartner::
	tower_dopartnermsg PARTNER_MSGID_REJECT
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExit::
	applymovement VAR_LAST_TALKED, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExit
	waitmovement 0
	return

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_PartnerExitSouth::
	applymovement VAR_LAST_TALKED, BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExitSouth
	waitmovement 0
	return

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExit:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_Movement_PartnerExitSouth:
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner2::
	lock
	faceplayer
	setvar VAR_TEMP_2, 1
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner3::
	lock
	faceplayer
	setvar VAR_TEMP_2, 2
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner4::
	lock
	faceplayer
	setvar VAR_TEMP_2, 3
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner5::
	lock
	faceplayer
	setvar VAR_TEMP_2, 4
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner6::
	lock
	faceplayer
	setvar VAR_TEMP_2, 5
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner7::
	lock
	faceplayer
	setvar VAR_TEMP_2, 6
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_Partner8::
	lock
	faceplayer
	setvar VAR_TEMP_2, 7
	goto BattleFrontier_BattleTowerMultiPartnerRoom_EventScript_TalkToPotentialPartner
	end

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner:
	.string "Scegli un compagno tra gli Allenatori\n"
	.string "riuniti qui: arrivano da ogni dove!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_QuitLookingForPartner:
	.string "{PLAYER}, non hai ancora trovato\n"
	.string "un compagno per la Lotta Multipla.\p"
	.string "Vuoi rinunciare alla ricerca e\n"
	.string "tornare al banco dell'accettazione?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PleaseFindPartner2:
	.string "Allora scegli pure un compagno\n"
	.string "tra gli Allenatori riuniti qui.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ThankYouForChoosingPartner::
	.string "Grazie per aver scelto il tuo\n"
	.string "compagno di squadra.\p"
	.string "Ora ti accompagno nella\n"
	.string "Sala Lotta Multipla.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Intro::
	.string "Sono l'allievo n. {STR_VAR_2} di {STR_VAR_1}.\n"
	.string "Puoi chiamarmi {STR_VAR_3}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Mon1::
	.string "Su consiglio di {STR_VAR_1} ho portato\n"
	.string "un {STR_VAR_3} con {STR_VAR_2} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Mon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Ti prego, facciamo squadra insieme!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Accept::
	.string "Grazie!\n"
	.string "Corro subito a iscriverci.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice1Reject::
	.string "Ci tenevo tanto a fare squadra\n"
	.string "con te, {PLAYER}…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Intro::
	.string "Sono l'allievo n. {STR_VAR_2} di {STR_VAR_1}.\n"
	.string "Il mio nome è {STR_VAR_3}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Mon1::
	.string "Dopo aver sentito {STR_VAR_1}, ho scelto\n"
	.string "un {STR_VAR_3} con {STR_VAR_2} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Mon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Dai, formiamo una squadra!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Accept::
	.string "Grazie mille!\n"
	.string "Faccio l'iscrizione in un lampo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_UnusedApprentice2Reject::
	.string "{PLAYER}, speravo tanto di poter\n"
	.string "fare coppia con te…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Intro::
	.string "Ehm, mi chiamo {STR_VAR_3} e sono\n"
	.string "l'allievo n. {STR_VAR_2} di {STR_VAR_1}.\p"
	.string "Sniff…\p"
	.string "Scusa!\n"
	.string "Sono così teso che mi viene da piangere…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Mon1::
	.string "{STR_VAR_3} mi ha consigliato, quindi ho\n"
	.string "un {STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Mon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Sniff…\n"
	.string "Ti prego, ti prego, fai squadra con me!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Accept::
	.string "Davvero? Ci stai?\n"
	.string "Evviva! Fantastico! Uaaaaah!\p"
	.string "Oh… Scusa…\n"
	.string "Sono così felice che piango…\p"
	.string "Vado subito a iscriverci.\n"
	.string "Tu non andartene, eh!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice1Reject::
	.string "Oh, m-ma…\n"
	.string "Sob… Buaaaaah!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Intro::
	.string "Ciao! Io sono {STR_VAR_3}!\n"
	.string "Sono l'allievo n. {STR_VAR_2} di {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Mon1::
	.string "{STR_VAR_3} mi ha scelto la squadra.\n"
	.string "Un {STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Mon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}:\n"
	.string "ecco cosa ho! Forte, eh?\p"
	.string "Allora, dai!\n"
	.string "Formiamo una squadra!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Accept::
	.string "Sì! Grande!\n"
	.string "Vado a iscriverci, ok?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice2Reject::
	.string "Uffa, perché?\n"
	.string "Volevo fare squadra con te, {PLAYER}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Intro::
	.string "Sono {STR_VAR_3}, l'allievo n. {STR_VAR_2}\n"
	.string "di {STR_VAR_1}, una vera leggenda!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Mon1::
	.string "Ho chiesto consiglio a {STR_VAR_3}.\n"
	.string "Un {STR_VAR_2} che usa {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Mon2Ask::
	.string "e un {STR_VAR_2} con {STR_VAR_1}:\n"
	.string "ecco la mia coppia.\p"
	.string "Ti andrebbe di fare squadra\n"
	.string "con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Accept::
	.string "Grazie!\n"
	.string "Vado subito a iscriverci!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice3Reject::
	.string "{PLAYER}, speravo davvero di fare\n"
	.string "squadra con te…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Intro::
	.string "Ehm… sono davvero felice che tu\n"
	.string "abbia trovato il tempo di parlarmi.\p"
	.string "Mi chiamo {STR_VAR_3}.\n"
	.string "Sono l'allieva n. {STR_VAR_2} di {STR_VAR_1}…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Mon1::
	.string "{STR_VAR_3} mi ha dato dei consigli.\n"
	.string "Gliene sono davvero grata.\p"
	.string "La mia squadra è un {STR_VAR_2}\n"
	.string "con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Mon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}.\n"
	.string "Lo so, ti chiedo tanto…\p"
	.string "Forse non ti andrà, però…\n"
	.string "posso fare coppia con te?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Accept::
	.string "Davvero? Non ci posso credere!\n"
	.string "Mi lasci fare squadra con te!\l"
	.string "I-io… non ti deluderò!\p"
	.string "Ehm… se ti va bene, vado subito\n"
	.string "a fare l'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice4Reject::
	.string "Oh…\n"
	.string "Lo sapevo, non sono all'altezza…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Intro::
	.string "Ciao, sono {STR_VAR_3}.\n"
	.string "Sono l'allieva n. {STR_VAR_2} di {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Mon1::
	.string "{STR_VAR_3} dice che questa squadra\n"
	.string "è perfetta per me:\l"
	.string "un {STR_VAR_2} che usa {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Mon2Ask::
	.string "e un {STR_VAR_2} che sa usare\n"
	.string "{STR_VAR_1}.\p"
	.string "Mica male, eh?\n"
	.string "Vuoi che faccia squadra con te?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Accept::
	.string "Ok, ci sto volentieri!\n"
	.string "Però non farmi fare brutte figure!\l"
	.string "Vado a iscriverci.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice5Reject::
	.string "Eh? Mi hai scartata?\n"
	.string "Non sai proprio giudicare le persone!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Intro::
	.string "Sono {STR_VAR_3} e sono un Triatleta.\n"
	.string "Ho giornate pienissime tra corsa,\l"
	.string "allenamento e lezioni di rap.\p"
	.string "E, guarda caso, sono anche\n"
	.string "l'allievo n. {STR_VAR_2} di {STR_VAR_1}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Mon1::
	.string "Ecco cosa mi ha consigliato {STR_VAR_3}.\n"
	.string "Un {STR_VAR_2} che usa {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Mon2Ask::
	.string "e un {STR_VAR_2} che usa\n"
	.string "{STR_VAR_1}.\p"
	.string "Li ho allenati con impegno,\n"
	.string "anche con mille cose da fare.\p"
	.string "Formiamo una squadra\n"
	.string "e diamoci dentro!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Accept::
	.string "Grazie, questo è lo spirito giusto!\n"
	.string "Aspettami, vado a iscriverci, ok?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice6Reject::
	.string "Ho ritagliato del tempo apposta\n"
	.string "per venire qui! Ma dai!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Intro::
	.string "Ciao!\n"
	.string "Come va?\p"
	.string "Sono {STR_VAR_3}, l'allieva\n"
	.string "n. {STR_VAR_2} di {STR_VAR_1}. Piacere!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Mon1::
	.string "Senti, senti! Devi sapere quali\n"
	.string "Pokémon mi ha consigliato {STR_VAR_3}.\l"
	.string "Un {STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Mon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}.\n"
	.string "Allenati alla perfezione, ovvio!\p"
	.string "Allora, facciamo squadra?\n"
	.string "Saremo una coppia fantastica!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Accept::
	.string "Evviva, sarà divertentissimo!\n"
	.string "Vado a iscriverci!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice7Reject::
	.string "Uffa, che cattivo!\n"
	.string "Guarda che io vengo raccomandata!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Intro::
	.string "Permettimi di presentarmi.\n"
	.string "Il mio nome è {STR_VAR_3}.\p"
	.string "Ho l'onore di essere l'allieva\n"
	.string "n. {STR_VAR_2} di {STR_VAR_1}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Mon1::
	.string "Ho seguito i saggi consigli di\n"
	.string "{STR_VAR_3} e ho allevato\l"
	.string "un {STR_VAR_2} con {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Mon2Ask::
	.string "e un {STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Ti prego, accetta di fare coppia\n"
	.string "con me!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Accept::
	.string "Oh… Che gioia!\n"
	.string "Ti prometto che darò il massimo!\p"
	.string "All'iscrizione penso io!\n"
	.string "Attendi un istante!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice8Reject::
	.string "Speravo tanto di fare squadra con te,\n"
	.string "{PLAYER}…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Intro::
	.string "Iiih! Mi hai rivolto la parola!\n"
	.string "S-sono al settimo cielo!\p"
	.string "Sono {STR_VAR_3}, l'allieva\n"
	.string "n. {STR_VAR_2} di {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Mon1::
	.string "Su consiglio di {STR_VAR_3} ho allenato\n"
	.string "un {STR_VAR_2} che usa {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Mon2Ask::
	.string "e un {STR_VAR_2} che usa\n"
	.string "{STR_VAR_1}.\p"
	.string "Ti prego, esaudisci il mio desiderio!\n"
	.string "Voglio fare coppia con te!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Accept::
	.string "Iiih! M-mi gira la testa!\n"
	.string "Grazie infinite!\l"
	.string "Vado subito a iscriverci!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice9Reject::
	.string "Buaaah! Non hai un po' di pietà?\n"
	.string "Però così sei ancora più affascinante…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Intro::
	.string "Yuhuuu! Sono l'allievo\n"
	.string "n. {STR_VAR_2} di {STR_VAR_1}!\p"
	.string "Mi chiamo {STR_VAR_3}! Ciao, ciao!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Mon1::
	.string "Il mio mentore {STR_VAR_3} mi ha\n"
	.string "consigliato un {STR_VAR_2}\l"
	.string "maestro di {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Mon2Ask::
	.string "e un {STR_VAR_2} maestro\n"
	.string "di {STR_VAR_1}.\p"
	.string "Roba buona, eh?\n"
	.string "Farai squadra con me, vero?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Accept::
	.string "Ottimo, benissimo!\n"
	.string "Sbrigo l'iscrizione in un attimo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice10Reject::
	.string "Uaaargh!\n"
	.string "Sei proprio un calcolatore, {PLAYER}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Intro::
	.string "Ehi, guai grossi! Si è aperto un Varco\n"
	.string "proprio sotto la Torre Lotta!\p"
	.string "… … … … … …\n"
	.string "Scherzavo!\p"
	.string "Tutti mi chiamano {STR_VAR_3}, io pure!\n"
	.string "Sono l'allievo n. 1000 di {STR_VAR_1}!\l"
	.string "Anzi no, sono il n. {STR_VAR_2}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Mon1::
	.string "{STR_VAR_3} mi ha dato qualche dritta.\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Mon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}.\n"
	.string "Ne ho dieci per tipo!\l"
	.string "Anzi no, uno per tipo!\p"
	.string "Che ne dici?\n"
	.string "Proviamo le lotte in coppia?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Accept::
	.string "Urrà!\n"
	.string "Per ringraziarti, ti regalo un Pokémon!\p"
	.string "Scherzo! Però adesso vado davvero\n"
	.string "a fare l'iscrizione, ok?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice11Reject::
	.string "Che freddezza! Per vendetta,\n"
	.string "adesso demolisco la Torre Lotta!\p"
	.string "Ma figurati se lo faccio!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Intro::
	.string "Yo! Ho il ritmo nel sangue e nel cuore,\n"
	.string "coi Pokémon sono io il domatore!\p"
	.string "Sono {STR_VAR_3}, il Marinaio rapper.\n"
	.string "Non scambiarmi per un Wailmer!\p"
	.string "Di {STR_VAR_1} sono l'allievo\n"
	.string "numero {STR_VAR_2}, che onore!\l"
	.string "Snobbarmi sarebbe un grosso errore!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Mon1::
	.string "Coi consigli di {STR_VAR_3}, che lavoro,\n"
	.string "ho messo su una squadra che vale oro!\l"
	.string "Un {STR_VAR_2} che usa {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Mon2Ask::
	.string "e un {STR_VAR_2} che usa\n"
	.string "{STR_VAR_1}!\p"
	.string "Il nostro incontro va festeggiato,\n"
	.string "con un trionfo in coppia meritato!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Accept::
	.string "Ehi, ehi, hai proprio un bel flow!\n"
	.string "Vado a iscriverci, pronti allo show!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice12Reject::
	.string "Ehi, {PLAYER}, che freddo, che gelo!\n"
	.string "Lasciatelo dire: mi si rizza il pelo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Intro::
	.string "Yuppi-yahoo!\n"
	.string "Ehi, non scappare! Volevo solo giocare!\p"
	.string "Salve! Sono {STR_VAR_3}!\n"
	.string "Sono l'allievo n. {STR_VAR_2} di {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Mon1::
	.string "{STR_VAR_3} mi ha detto cosa fare.\n"
	.string "Così un {STR_VAR_2} che usa {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Mon2Ask::
	.string "e un {STR_VAR_2} che usa\n"
	.string "{STR_VAR_1} ho voluto portare.\p"
	.string "Allora, che ne dici?\n"
	.string "Dai, diventiamo grandi amici!\p"
	.string "Ti prego, non dirmi di no,\n"
	.string "senza di te mi spengo, lo so!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Accept::
	.string "Yeah! La fortuna è dalla mia!\n"
	.string "Che gioia, che allegria!\p"
	.string "Prima di scendere in campo a lottare,\n"
	.string "vado subito a farci registrare!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice13Reject::
	.string "Mi stai dicendo di no?\n"
	.string "{PLAYER}, il broncio mi metterò!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Intro::
	.string "Coff!\p"
	.string "Oh, scusa, ho il raffreddore.\n"
	.string "I miei Pokémon stanno benone, però.\p"
	.string "Mi chiamo {STR_VAR_3}, allievo\n"
	.string "n. {STR_VAR_2} di {STR_VAR_1}. Coff!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Mon1::
	.string "Ho seguito alla lettera i consigli di\n"
	.string "{STR_VAR_3} e ho formato una squadra:\l"
	.string "un {STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Mon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Li ho allenati ogni giorno,\n"
	.string "anche sotto vento e pioggia.\p"
	.string "Credo sia per questo che mi\n"
	.string "sono preso il raffreddore.\p"
	.string "Ecco, questo sono io.\n"
	.string "Vuoi fare coppia con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Accept::
	.string "Grazie, lo apprezzo davvero!\n"
	.string "Coff, coff!\l"
	.string "Aspetta qui, vado a iscriverci.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice14Reject::
	.string "Oh, non vuoi?\n"
	.string "{PLAYER}, saremmo stati una bella coppia.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Intro::
	.string "Oh, salve!\n"
	.string "Che agitazione…\p"
	.string "Sono {STR_VAR_3}.\n"
	.string "Sono l'allievo n. {STR_VAR_2} di {STR_VAR_1}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Mon1::
	.string "Secondo {STR_VAR_3} questa squadra\n"
	.string "è proprio ben assortita:\l"
	.string "un {STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Mon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Mi sento un po' in imbarazzo, ma…\n"
	.string "mi lasceresti fare squadra con te?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Accept::
	.string "Oh, accidenti, grazie!\n"
	.string "Sono timido, ma farò del mio meglio!\p"
	.string "Vado a sbrigare l'iscrizione.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice15Reject::
	.string "Oh, ti prego, non dire di no!\n"
	.string "Sono già abbastanza imbarazzato…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Intro::
	.string "Il mio nome è {STR_VAR_3}, parola d'onore.\n"
	.string "Sono l'allievo n. {STR_VAR_2} di {STR_VAR_1}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Mon1::
	.string "{STR_VAR_3} mi ha suggerito la squadra.\p"
	.string "Poiché il consiglio pareva sincero,\n"
	.string "ho deciso di portare con me\l"
	.string "il mio {STR_VAR_2} che usa {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Mon2Ask::
	.string "il mio {STR_VAR_2} che usa {STR_VAR_1}.\p"
	.string "Se possibile, gradirei che tu mi\n"
	.string "accettassi come compagno di squadra.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Accept::
	.string "Mi accetti davvero?\n"
	.string "Altrimenti sarebbe stato terribile.\p"
	.string "Ma visto che hai acconsentito, bene.\n"
	.string "Provvederò a iscrivere entrambi.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_Apprentice16Reject::
	.string "Uhm? E perché mai rifiuti?\n"
	.string "È forse il tuo modo di scherzare?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassIntro::
	.string "Ciao! Sono la Ragazzina {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassMon1::
	.string "Con me ho un {STR_VAR_2}\n"
	.string "che usa {STR_VAR_1} e un$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassMon2Ask::
	.string "{STR_VAR_2} che usa {STR_VAR_1}.\n"
	.string "Ecco la mia squadra.\p"
	.string "Vuoi essere il mio compagno?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassAccept::
	.string "Grazie!\n"
	.string "Vado a fare l'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LassReject::
	.string "Non vuoi fare coppia con me?\n"
	.string "Te ne pentirai!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterIntro::
	.string "Ciao!\p"
	.string "Sono il Giovanotto {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterMon1::
	.string "Vuoi sapere cosa ho in squadra?\n"
	.string "Un {STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterMon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Farai squadra con me,\n"
	.string "vero?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterAccept::
	.string "Evviva!\n"
	.string "Vado a iscriverci, ok?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_YoungsterReject::
	.string "Uffa! Tu insieme ai miei Pokémon…\n"
	.string "saremmo stati imbattibili!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerIntro::
	.string "Yahoo!\n"
	.string "Sono il Montanaro {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerMon1::
	.string "Sai cosa mi porto nello zaino?\n"
	.string "Il mio {STR_VAR_2} che usa {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerMon2Ask::
	.string "il mio {STR_VAR_2} che usa {STR_VAR_1}!\p"
	.string "Niente male, eh?\n"
	.string "Formiamo una cordata?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerAccept::
	.string "Yahoo!\n"
	.string "Allora vado a iscriverci.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HikerReject::
	.string "Mi sarebbe piaciuto scalare la Torre\n"
	.string "con te al mio fianco.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyIntro::
	.string "Ciao!\n"
	.string "Sono {STR_VAR_1}, una vera Bellezza!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyMon1::
	.string "Sai chi ho allevato con tanta cura?\n"
	.string "Un {STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyMon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Che ne pensi?\n"
	.string "Facciamo coppia, noi due?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyAccept::
	.string "Meraviglioso!\n"
	.string "Vado subito a fare l'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BeautyReject::
	.string "Che delusione!\p"
	.string "Noi due insieme… saremmo stati\n"
	.string "i migliori in assoluto!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanIntro::
	.string "Ehilà!\p"
	.string "Sai chi sono?\n"
	.string "Sono {STR_VAR_1}, il Pescatore!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanMon1::
	.string "Nella mia rete ci sono un\n"
	.string "{STR_VAR_2} che usa {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanMon2Ask::
	.string "un {STR_VAR_2} che usa {STR_VAR_1}.\p"
	.string "Allora, abbocchi?\n"
	.string "Lotterai al mio fianco?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanAccept::
	.string "Bene, bene!\n"
	.string "Lascia fare a me!\p"
	.string "Vado a iscriverci subito.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_FishermanReject::
	.string "E dire che eravamo perfetti insieme…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyIntro::
	.string "Lieta di fare la tua conoscenza.\n"
	.string "Io sono la Signorina {STR_VAR_1}.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyMon1::
	.string "Mi accompagna una squadra formata da\n"
	.string "un {STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyMon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Spero di avere la tua approvazione,\n"
	.string "poiché desidero te come compagno.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyAccept::
	.string "Ti ringrazio sentitamente.\n"
	.string "All'iscrizione penserò io.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_LadyReject::
	.string "Sono certa che rimpiangerai di non\n"
	.string "avermi scelta come compagna.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFIntro::
	.string "Sono la Triatleta {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFMon1::
	.string "Cosa ho con me…\n"
	.string "Un {STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFMon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}!\n"
	.string "Ecco la mia coppia!\p"
	.string "Allora?\n"
	.string "Pedaliamo insieme verso la vittoria?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFAccept::
	.string "Grazie mille!\n"
	.string "Vado a iscriverci al bancone.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteFReject::
	.string "Noi due insieme saremmo stati\n"
	.string "fortissimi, ne sono certa!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherIntro::
	.string "Ciao!\n"
	.string "Sono il Pigliamosche {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherMon1::
	.string "Guarda che squadra ho messo su!\n"
	.string "Un {STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherMon2Ask::
	.string "un {STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Allora, senti!\n"
	.string "Vuoi fare squadra con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherAccept::
	.string "Preso al volo!\p"
	.string "Vado a sbrigare le pratiche\n"
	.string "d'iscrizione al bancone.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugCatcherReject::
	.string "Uffa, i miei Pokémon sono fortissimi.\n"
	.string "Spero che non te ne pentirai!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMIntro::
	.string "Buongiorno!\n"
	.string "Sono lo Scolaro {STR_VAR_1}!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMMon1::
	.string "Ho allevato con cura un\n"
	.string "{STR_VAR_2} che usa {STR_VAR_1}$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMMon2Ask::
	.string "e poi il mio {STR_VAR_2}, che usa\n"
	.string "{STR_VAR_1}.\p"
	.string "Niente male, non trovi?\n"
	.string "Ti andrebbe di fare coppia con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMAccept::
	.string "Grazie mille!\n"
	.string "Vado subito a sbrigare l'iscrizione.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidMReject::
	.string "Che peccato…\p"
	.string "Speravo di imparare qualcosa\n"
	.string "facendo coppia con te…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyIntro::
	.string "Ehi! Lascia che mi presenti!\n"
	.string "Sono {STR_VAR_1}, il Riccastro!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyMon1::
	.string "Indovina cosa mi sono procurato!\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Ti concedo l'onore di fare coppia\n"
	.string "con me. Non capita a tutti!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyAccept::
	.string "Mossa furba!\p"
	.string "Sbrigo l'iscrizione in un lampo:\n"
	.string "il tempo è denaro!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RichBoyReject::
	.string "Rifiuti proprio me?!\n"
	.string "Te ne pentirai, puoi giurarci!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltIntro::
	.string "Hiyaa!\n"
	.string "Sono {STR_VAR_1}, Cinturanera.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltMon1::
	.string "Al mio fianco combattono\n"
	.string "{STR_VAR_2}, che usa {STR_VAR_1},$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltMon2Ask::
	.string "e {STR_VAR_2}, che usa\n"
	.string "{STR_VAR_1}.\p"
	.string "Ti prego, esaudisci il mio desiderio!\n"
	.string "Lasciami lottare in coppia con te!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltAccept::
	.string "Hiyaa!\n"
	.string "Corro a iscriverci all'istante!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BlackBeltReject::
	.string "Capisco… Spero in un'altra occasione\n"
	.string "la prossima volta che ci incontriamo…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFIntro::
	.string "Ciao ciao!\n"
	.string "Sono {STR_VAR_1} e sono una Bagnante!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFMon1::
	.string "Ti dico chi ho con me!\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\n"
	.string "Posso fare coppia con te, per favore?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFAccept::
	.string "Grazie!\n"
	.string "Vado subito a iscriverci!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberFReject::
	.string "Se avessimo fatto coppia, saremmo\n"
	.string "stati fortissimi!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacIntro::
	.string "Salve…\n"
	.string "Sono {STR_VAR_1}, l'Occultista…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacMon1::
	.string "Mi accompagna {STR_VAR_2}, che usa\n"
	.string "{STR_VAR_1}, insieme a…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacMon2Ask::
	.string "…{STR_VAR_2}, che usa {STR_VAR_1}…\p"
	.string "Gli spiriti mi sussurrano di te…\n"
	.string "Facciamo coppia, te ne supplico…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacAccept::
	.string "Ti ringrazio…\n"
	.string "Provvederò all'iscrizione…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_HexManiacReject::
	.string "Desideravo tanto unirmi a te…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMIntro::
	.string "Piacere! Sono {STR_VAR_1},\n"
	.string "e sono un Allevatore!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMMon1::
	.string "Ne sto crescendo un paio coi fiocchi!\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Che ne dici?\n"
	.string "Ti va di fare coppia con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMAccept::
	.string "Grazie di cuore!\p"
	.string "Vado a sbrigare le pratiche per\n"
	.string "l'iscrizione, tu aspettami qui!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederMReject::
	.string "Non vedevo l'ora di fare coppia\n"
	.string "con te…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFIntro::
	.string "Ehilà, ciao!\n"
	.string "Sono {STR_VAR_1}, Triatleta!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFMon1::
	.string "Sai con chi vado a correre?\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Allora?\n"
	.string "Ti va di fare coppia con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFAccept::
	.string "Ottima scelta!\p"
	.string "Volo a fare l'iscrizione,\n"
	.string "ci metto un attimo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteFReject::
	.string "Io e te saremmo stati imbattibili.\n"
	.string "Che peccato…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMIntro::
	.string "Ehi, ciao! Mi chiamo {STR_VAR_1}!\n"
	.string "Sono un Triatleta!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMMon1::
	.string "Corro sempre con la mia squadra:\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Mica male, eh?\n"
	.string "Dovremmo proprio fare coppia!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMAccept::
	.string "Grande!\n"
	.string "Corro a iscriverci in un lampo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RunningTriathleteMReject::
	.string "Ci tenevo davvero a lottare in\n"
	.string "coppia con te…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlIntro::
	.string "Sono {STR_VAR_1}, Lottatrice!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlMon1::
	.string "Mi sto allenando sodo con\n"
	.string "{STR_VAR_2}, che usa {STR_VAR_1}, e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlMon2Ask::
	.string "{STR_VAR_2}, che usa {STR_VAR_1}!\p"
	.string "Ti piace la combinazione?\n"
	.string "Che ne dici di unire le forze?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlAccept::
	.string "Evvai, grazie!\p"
	.string "Vado subito a sbrigare\n"
	.string "l'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BattleGirlReject::
	.string "Con te pensavo di formare la coppia\n"
	.string "più forte di tutte…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMIntro::
	.string "Sono {STR_VAR_1}, Triatleta!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMMon1::
	.string "Pedalo sempre con {STR_VAR_2},\n"
	.string "che sa usare {STR_VAR_1},$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMMon2Ask::
	.string "e {STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Potremmo fare coppia.\n"
	.string "Non sarebbe fantastico?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMAccept::
	.string "Grazie!\p"
	.string "Vado a iscriverci, e ci vado\n"
	.string "a tutta velocità!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CyclingTriathleteMReject::
	.string "Uffa, peccato. Saremmo stati la coppia\n"
	.string "più tosta in circolazione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMIntro::
	.string "Io?\n"
	.string "Sono {STR_VAR_1}, Bagnante!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMMon1::
	.string "Chi ho con me? Te lo dico subito!\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Ehi?\n"
	.string "Fai coppia con me, vero?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMAccept::
	.string "Okay!\p"
	.string "Vado a iscriverci!\n"
	.string "Insieme saremo fortissimi!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_TuberMReject::
	.string "I miei Pokémon sono forti, sai…\n"
	.string "Peccato che non vuoi fare coppia.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristIntro::
	.string "Yeah-yeah!\n"
	.string "Chiamami {STR_VAR_1}, il Chitarrista!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristMon1::
	.string "Ecco la mia band!\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Yeah! Che grinta, eh?\n"
	.string "Dobbiamo suonare un duetto in coppia!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristAccept::
	.string "Yeah-yeah! Grande!\n"
	.string "Vado a sbrigare l'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GuitaristReject::
	.string "I miei Pokémon fanno faville!\n"
	.string "Te ne pentirai, te lo dico io!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanIntro::
	.string "Lieto di fare la tua conoscenza.\n"
	.string "Sono {STR_VAR_1}, Gentiluomo.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanMon1::
	.string "Mi accompagnano il mio fidato\n"
	.string "{STR_VAR_2}, che usa {STR_VAR_1},$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanMon2Ask::
	.string "e {STR_VAR_2}, che usa {STR_VAR_1}.\p"
	.string "Posso chiederti l'onore di unirti\n"
	.string "a me in una Lotta Multipla?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanAccept::
	.string "Ah, ti ringrazio per la fiducia.\n"
	.string "Provvederò io all'iscrizione.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_GentlemanReject::
	.string "Davvero un peccato…\p"
	.string "Attenderò con piacere la prossima\n"
	.string "occasione…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMIntro::
	.string "Ciao, sono {STR_VAR_1},\n"
	.string "e sono un Pokéfan.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMMon1::
	.string "Oggi ho con me {STR_VAR_2}\n"
	.string "che conosce la mossa {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Ti piacciono?\n"
	.string "Perché non fai coppia con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMAccept::
	.string "Grazie!\n"
	.string "Mi occupo io dell'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanMReject::
	.string "I miei Pokémon sono di prima classe…\n"
	.string "Peccato che tu non sappia apprezzarli.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMIntro::
	.string "Hmf!\n"
	.string "Sono {STR_VAR_1}. Esperto, io sono!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMMon1::
	.string "I Pokémon che ho temprato sono\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Che ne dici di fare coppia con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMAccept::
	.string "Hmf!\n"
	.string "Vado subito a iscriverci!\l"
	.string "Diamo entrambi il massimo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertMReject::
	.string "Mi auguro che la tua scelta sia\n"
	.string "davvero quella giusta…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFIntro::
	.string "Salve, salve.\n"
	.string "Sono {STR_VAR_1}, e sono un'Esperta.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFMon1::
	.string "Ho cresciuto i miei Pokémon con cura.\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}:\n"
	.string "ecco chi ho con me.\p"
	.string "Non ti va di fare coppia con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFAccept::
	.string "Bene, bene.\n"
	.string "Mi occupo subito dell'iscrizione.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ExpertFReject::
	.string "Magari faremo coppia la prossima\n"
	.string "volta che ci incontreremo.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerIntro::
	.string "Sono {STR_VAR_1}, Domadraghi!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerMon1::
	.string "La squadra che sto temprando è\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Che ne dici?\n"
	.string "Vuoi essere il mio compagno?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerAccept::
	.string "D'accordo, darò il massimo!\n"
	.string "Vado a iscriverci, va bene?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_DragonTamerReject::
	.string "Non troverai molti compagni\n"
	.string "più tosti di me!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperIntro::
	.string "Sono {STR_VAR_1}, Ornitologo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperMon1::
	.string "Chi c'è nel mio stormo? Eccoli:\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Non pensi che in coppia\n"
	.string "voleremmo altissimo?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperAccept::
	.string "Grande, grazie!\n"
	.string "Mi occupo io dell'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BirdKeeperReject::
	.string "Io e i miei Pokémon siamo forti.\n"
	.string "Che delusione…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyIntro::
	.string "Sono {STR_VAR_1}, Bimbo Ninja!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyMon1::
	.string "La mia squadra segreta è formata da\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Facciamo coppia, dai!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyAccept::
	.string "Evviva!\n"
	.string "Corro a iscriverci, veloce come un ninja!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_NinjaBoyReject::
	.string "Ti pentirai di non avere i miei\n"
	.string "Pokémon fortissimi dalla tua parte!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyIntro::
	.string "Salve!\n"
	.string "Sono {STR_VAR_1}, Dama Parasol!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyMon1::
	.string "Mi fanno da scorta {STR_VAR_2},\n"
	.string "che usa {STR_VAR_1}, e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyMon2Ask::
	.string "{STR_VAR_2}, che usa\n"
	.string "{STR_VAR_1}.\p"
	.string "Non sono deliziosi?\n"
	.string "Ti va di fare coppia con noi?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyAccept::
	.string "Grazie mille!\p"
	.string "Vado a iscriverci al bancone.\n"
	.string "Non deludiamoci a vicenda!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_ParasolLadyReject::
	.string "I miei Pokémon sono fortissimi.\n"
	.string "Che delusione…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacIntro::
	.string "Ciao.\n"
	.string "Sono {STR_VAR_1}, un Insettomane!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacMon1::
	.string "Li ho scovati vicino a un Varco, sì.\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}:\n"
	.string "ecco cosa ho trovato.\p"
	.string "Posso convincerti a fare coppia\n"
	.string "con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacAccept::
	.string "Okay!\n"
	.string "Ricevuto!\p"
	.string "Non ci metterò molto con l'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_BugManiacReject::
	.string "Con i Pokémon che ho trovato io,\n"
	.string "non avremmo perso…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorIntro::
	.string "Ehi, di bordo!\n"
	.string "Sono {STR_VAR_1}, Marinaio!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorMon1::
	.string "Ti presento il mio orgoglio!\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Non vorrai mica dirmi di no.\n"
	.string "Facciamo coppia, giusto?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorAccept::
	.string "Non mi aspettavo niente di meno!\n"
	.string "Vado a iscriverci.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SailorReject::
	.string "Avremmo travolto tutti come una\n"
	.string "mareggiata! Peccato!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorIntro::
	.string "Ciao, sono {STR_VAR_1}.\n"
	.string "Sono un Raccoglitore.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorMon1::
	.string "I gioielli della mia collezione sono\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Fantastici, eh?\n"
	.string "Dovremmo fare squadra insieme.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorAccept::
	.string "Oh, sì!\p"
	.string "Bene, non perdiamo tempo.\n"
	.string "Vado a iscrivere tutti e due.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CollectorReject::
	.string "Beh, che delusione.\n"
	.string "Non sai apprezzare i miei Pokémon.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMIntro::
	.string "Salve, sono {STR_VAR_1}.\n"
	.string "Sono un Pokémon Ranger.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMMon1::
	.string "Mi fanno compagnia {STR_VAR_2},\n"
	.string "che usa {STR_VAR_1}, e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Non pensi che saremmo una coppia\n"
	.string "davvero formidabile?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMAccept::
	.string "Fantastico!\n"
	.string "Mi occupo subito dell'iscrizione.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerMReject::
	.string "La prossima volta scegli i miei\n"
	.string "Pokémon, d'accordo?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFIntro::
	.string "Mi chiamo {STR_VAR_1}.\n"
	.string "Sono una Pokémon Ranger!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFMon1::
	.string "Ti presento la mia squadra. Ho\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Che ne dici di fare coppia\n"
	.string "con la mia piccola banda?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFAccept::
	.string "Daremo il meglio!\n"
	.string "Sbrigo l'iscrizione in un attimo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnRangerFReject::
	.string "Spero che la prossima volta\n"
	.string "sceglierai i miei Pokémon.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyIntro::
	.string "Piacere, sono {STR_VAR_1}.\n"
	.string "Mi considero una Profumiera.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyMon1::
	.string "Viaggio insieme a {STR_VAR_2},\n"
	.string "che usa {STR_VAR_1}, e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyMon2Ask::
	.string "{STR_VAR_2}, che usa\n"
	.string "{STR_VAR_1}.\p"
	.string "Profumano di fiori… e di ozono,\n"
	.string "come l'aria vicino a un Varco.\p"
	.string "Vorresti essere il mio compagno?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyAccept::
	.string "Sono onorata che tu abbia accettato.\n"
	.string "Vado subito a iscriverci.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_AromaLadyReject::
	.string "Sarebbe meraviglioso fare coppia\n"
	.string "la prossima volta che ci vediamo.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacIntro::
	.string "Vuoi sapere chi sono?\n"
	.string "Sono {STR_VAR_1}, l'Archeologo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacMon1::
	.string "I Pokémon che porto con me sono\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Interessante, eh?\n"
	.string "Che ne dici di fare coppia?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacAccept::
	.string "Decisione assennata!\n"
	.string "Vado a compilare le scartoffie.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_RuinManiacReject::
	.string "Mmh…\n"
	.string "Eppure i miei Pokémon sono tosti…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMIntro::
	.string "Sono {STR_VAR_1}, Fantallenatore!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMMon1::
	.string "Al momento i miei Pokémon sono\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}.\n"
	.string "Forte, eh?\p"
	.string "Non sarebbe fortissimo se\n"
	.string "facessimo coppia?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMAccept::
	.string "Forte!\n"
	.string "Vado a iscriverci in un lampo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerMReject::
	.string "Pensavo che saremmo stati\n"
	.string "la coppia migliore di sempre.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFIntro::
	.string "Sono {STR_VAR_1}, Fantallenatrice!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFMon1::
	.string "La squadra che sto allenando ha\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Ti sembra una buona squadra?\n"
	.string "Che ne dici di fare coppia?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFAccept::
	.string "Perfetto!\n"
	.string "Allora vado a fare l'iscrizione.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CoolTrainerFReject::
	.string "E io che pensavo a che coppia tosta\n"
	.string "avremmo formato…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacIntro::
	.string "Ehilà!\n"
	.string "Sono {STR_VAR_1}, il Pokémaniaco!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacMon1::
	.string "Cos'ha uno come me? Tieniti forte!\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1},\n"
	.string "ecco cosa!\p"
	.string "Roba rara come un Varco aperto!\n"
	.string "In coppia faremo faville!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacAccept::
	.string "Ottima scelta!\n"
	.string "Iscrivo tutti e due!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokemaniacReject::
	.string "I miei Pokémon sono micidiali!\n"
	.string "Se te ne penti, non è colpa mia!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerIntro::
	.string "Ehi, tu!\n"
	.string "Sono {STR_VAR_1}, Fuochista!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerMon1::
	.string "Sai cosa ho sfornato in allenamento?\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Allora, che ne dici?\n"
	.string "Vuoi fare coppia con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerAccept::
	.string "Benissimo!\n"
	.string "Corro a iscriverci, a tutto fuoco!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_KindlerReject::
	.string "Promettimi che faremo coppia\n"
	.string "la prossima volta che ci incrociamo.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperIntro::
	.string "Sono {STR_VAR_1}, un Esploratore!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperMon1::
	.string "Sto allenando {STR_VAR_2}, che usa\n"
	.string "{STR_VAR_1}, e poi…$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperMon2Ask::
	.string "{STR_VAR_2}, che usa {STR_VAR_1}.\p"
	.string "Sarebbe divertente fare coppia, no?\n"
	.string "Io dico proprio di sì!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperAccept::
	.string "Sì!\n"
	.string "Vado a iscriverci!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_CamperReject::
	.string "La prossima volta, okay?\n"
	.string "Voglio fare squadra con te.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerIntro::
	.string "Ciao!\n"
	.string "Sono {STR_VAR_1}, e sono una Gitante!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerMon1::
	.string "I Pokémon che porto in gita sono\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Ti andrebbe di fare coppia\n"
	.string "con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerAccept::
	.string "Oh, grazie!\n"
	.string "Allora vado a fare l'iscrizione.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PicnickerReject::
	.string "Sarebbe bello fare coppia con te\n"
	.string "un'altra volta.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMIntro::
	.string "Sono {STR_VAR_1}, Sensitivo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMMon1::
	.string "Il duo che sto allenando è formato da\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Percepisco un legame tra noi…\n"
	.string "Vuoi fare coppia con me?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMAccept::
	.string "Certo!\n"
	.string "Mi occupo io dell'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicMReject::
	.string "Se ci rincontreremo, allora sì\n"
	.string "che vorrò fare coppia con te.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFIntro::
	.string "Sono {STR_VAR_1}.\n"
	.string "Sono una Sensitiva.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFMon1::
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}:\n"
	.string "ecco i miei discepoli.\p"
	.string "L'idea di formare una coppia\n"
	.string "non ti incuriosisce?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFAccept::
	.string "Grazie.\n"
	.string "Vado a occuparmi dell'iscrizione.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PsychicFReject::
	.string "Sento che ci sarà un'altra occasione\n"
	.string "per stringere un'alleanza.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFIntro::
	.string "Sono {STR_VAR_1}, Scolara!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFMon1::
	.string "{STR_VAR_2}, che usa {STR_VAR_1},$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFMon2Ask::
	.string "e {STR_VAR_2}, che usa\n"
	.string "{STR_VAR_1}: ecco il mio duo!\p"
	.string "Posso fare coppia con te in una\n"
	.string "Lotta Multipla, per favore?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFAccept::
	.string "Oh, grazie!\n"
	.string "Corro subito a iscriverci al bancone!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SchoolKidFReject::
	.string "Per favore…\n"
	.string "Posso unirmi a te la prossima volta?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFIntro::
	.string "Ciao! Mi chiamo {STR_VAR_1}!\n"
	.string "Sono un'Allevatrice!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFMon1::
	.string "I Pokémon che ho cresciuto sono\n"
	.string "{STR_VAR_2}, che usa {STR_VAR_1}, e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFMon2Ask::
	.string "{STR_VAR_2}, che usa {STR_VAR_1}.\p"
	.string "Ti incuriosisce?\n"
	.string "Allora che ne dici di fare coppia?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFAccept::
	.string "Benissimo!\n"
	.string "Lascia a me l'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PkmnBreederFReject::
	.string "La prossima volta devi fare coppia\n"
	.string "con me, d'accordo?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFIntro::
	.string "Sono {STR_VAR_1}, e sono fiera di\n"
	.string "essere una Pokéfan.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFMon1::
	.string "I tesorucci che ho cresciuto sono\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}.\p"
	.string "Non sono adorabili?\n"
	.string "Dovremmo proprio fare squadra!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFAccept::
	.string "Grazie, caro!\n"
	.string "Vado subito a iscriverci!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_PokefanFReject::
	.string "I miei tesorucci sono i migliori,\n"
	.string "sappilo. Che seccatura!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFIntro::
	.string "Ciao, sono {STR_VAR_1}, Nuotatrice.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFMon1::
	.string "{STR_VAR_2}, che usa {STR_VAR_1},$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFMon2Ask::
	.string "e {STR_VAR_2}, che usa\n"
	.string "{STR_VAR_1}: ecco chi ho allenato.\p"
	.string "Io e te: facciamo coppia, dai!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFAccept::
	.string "Forte!\n"
	.string "Iscrivo tutti e due.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerFReject::
	.string "Se ci rivediamo, mi devi una\n"
	.string "Lotta Multipla in coppia!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMIntro::
	.string "Come va?\n"
	.string "Sono {STR_VAR_1}, e sono un Triatleta.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMMon1::
	.string "Ho un paio di Pokémon niente male.\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Sarebbe forte fare coppia insieme.\n"
	.string "Allora, che ne dici?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMAccept::
	.string "Grande!\n"
	.string "Aspettami mentre ci iscrivo, okay?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteMReject::
	.string "La prossima volta fammi fare coppia\n"
	.string "con te, che ne dici?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFIntro::
	.string "Sono {STR_VAR_1}, Triatleta!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFMon1::
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFMon2Ask::
	.string "poi {STR_VAR_2}, che sa usare\n"
	.string "{STR_VAR_1}. Ecco il mio duo.\p"
	.string "Che ne pensi?\n"
	.string "Saremmo una bella squadra, direi.$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFAccept::
	.string "Mi piace questa risposta!\n"
	.string "Sbrigo l'iscrizione in un lampo!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmingTriathleteFReject::
	.string "Mi darai un'altra possibilità di fare\n"
	.string "coppia, vero?$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMIntro::
	.string "Ciao, ehilà!\n"
	.string "Sono {STR_VAR_1}, e sono un Nuotatore!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMMon1::
	.string "Guarda chi ho allenato!\n"
	.string "{STR_VAR_2} con {STR_VAR_1} e$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMMon2Ask::
	.string "{STR_VAR_2} con {STR_VAR_1}!\p"
	.string "Fantastici, eh?\n"
	.string "E sarebbe fantastico fare coppia!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMAccept::
	.string "Ti sono grato!\n"
	.string "Sbrigo subito l'iscrizione!$"

BattleFrontier_BattleTowerMultiPartnerRoom_Text_SwimmerMReject::
	.string "Se ci rivediamo, devi fare coppia\n"
	.string "con me. Lo farai, vero?$"

