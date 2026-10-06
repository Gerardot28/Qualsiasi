SlateportCity_BattleTentCorridor_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SlateportCity_BattleTentCorridor_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, SlateportCity_BattleTentCorridor_OnWarp
	.byte 0

	@ This is Slateport Tent's version of the Battle Factory Pre-Battle Room

SlateportCity_BattleTentCorridor_OnWarp:
	map_script_2 VAR_TEMP_1, 0, SlateportCity_BattleTentCorridor_EventScript_SetUpObjects
	.2byte 0

SlateportCity_BattleTentCorridor_EventScript_SetUpObjects::
	setvar VAR_TEMP_1, 1
	goto_if_ne VAR_0x8006, 1, SlateportCity_BattleTentCorridor_EventScript_TurnPlayerNorth
	setobjectxy LOCALID_SLATEPORT_TENT_CORRIDOR_ATTENDANT, 2, 2
	turnobject LOCALID_SLATEPORT_TENT_CORRIDOR_ATTENDANT, DIR_SOUTH
SlateportCity_BattleTentCorridor_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

SlateportCity_BattleTentCorridor_OnFrame:
	map_script_2 VAR_TEMP_0, 0, SlateportCity_BattleTentCorridor_EventScript_EnterCorridor
	.2byte 0

SlateportCity_BattleTentCorridor_EventScript_EnterCorridor::
	goto_if_eq VAR_0x8006, 1, SlateportCity_BattleTentCorridor_EventScript_ReturnToRoomFromBattle
	setvar VAR_TEMP_0, 1
	applymovement LOCALID_SLATEPORT_TENT_CORRIDOR_ATTENDANT, SlateportCity_BattleTentCorridor_Movement_AttendantEnter
	applymovement LOCALID_PLAYER, SlateportCity_BattleTentCorridor_Movement_PlayerEnter
	waitmovement 0
	goto_if_eq VAR_0x8006, 2, SlateportCity_BattleTentCorridor_EventScript_ResumeChallenge
	slateporttent_generaterentalmons
	slateporttent_generateopponentmons
	msgbox BattleFrontier_BattleFactoryPreBattleRoom_Text_HoldMonsChooseFromSelection, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	slateporttent_rentmons
	waitstate
SlateportCity_BattleTentCorridor_EventScript_EnterBattleRoom::
	msgbox BattleFrontier_BattleFactoryPreBattleRoom_Text_RightThisWay, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_TENT_CORRIDOR_ATTENDANT, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	opendoor 2, 1
	waitdooranim
	applymovement LOCALID_SLATEPORT_TENT_CORRIDOR_ATTENDANT, SlateportCity_BattleTentCorridor_Movement_AttendantExit
	applymovement LOCALID_PLAYER, SlateportCity_BattleTentCorridor_Movement_PlayerExit
	waitmovement 0
	closedoor 2, 1
	waitdooranim
	warp MAP_SLATEPORT_CITY_BATTLE_TENT_BATTLE_ROOM, 4, 4
	waitstate
	end

SlateportCity_BattleTentCorridor_EventScript_ReturnToRoomFromBattle::
	factory_setopponentmons
	factory_resethelditems
	msgbox BattleFrontier_BattleFactoryPreBattleRoom_Text_LetUsRestoreMons, MSGBOX_DEFAULT
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
SlateportCity_BattleTentCorridor_EventScript_AskReadyForOpponent::
	frontier_get FRONTIER_DATA_BATTLE_NUM
	call_if_eq VAR_RESULT, 1, SlateportCity_BattleTentCorridor_EventScript_ReadyFor2ndOpponent
	call_if_eq VAR_RESULT, 2, SlateportCity_BattleTentCorridor_EventScript_ReadyFor3rdOpponent
	multichoice 20, 6, MULTI_GO_ON_REST_RETIRE, TRUE
	switch VAR_RESULT
	case 0, SlateportCity_BattleTentCorridor_EventScript_AskSwapMon
	case 1, SlateportCity_BattleTentCorridor_EventScript_AskPauseChallenge
	case 2, SlateportCity_BattleTentCorridor_EventScript_AskRetireChallenge

SlateportCity_BattleTentCorridor_EventScript_AskPauseChallenge::
	msgbox BattleFrontier_BattleFactoryPreBattleRoom_Text_SaveAndQuitGame, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, SlateportCity_BattleTentCorridor_EventScript_AskReadyForOpponent
	case YES, SlateportCity_BattleTentCorridor_EventScript_PauseChallenge
	case MULTI_B_PRESSED, SlateportCity_BattleTentCorridor_EventScript_AskReadyForOpponent

SlateportCity_BattleTentCorridor_EventScript_AskRetireChallenge::
	message BattleFrontier_BattleFactoryPreBattleRoom_Text_RetireFromChallenge
	waitmessage
	multichoicedefault 20, 8, MULTI_YESNO, 1, FALSE
	switch VAR_RESULT
	case 1, SlateportCity_BattleTentCorridor_EventScript_AskReadyForOpponent
	case 0, SlateportCity_BattleTent_EventScript_WarpToLobbyLost
	case MULTI_B_PRESSED, SlateportCity_BattleTentCorridor_EventScript_AskReadyForOpponent

SlateportCity_BattleTentCorridor_EventScript_AskSwapMon::
	slateporttent_generateopponentmons
	msgbox BattleFrontier_BattleFactoryPreBattleRoom_Text_LikeToSwapMon, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, SlateportCity_BattleTentCorridor_EventScript_EnterBattleRoom
	case YES, SlateportCity_BattleTentCorridor_EventScript_SwapMons
	case MULTI_B_PRESSED, SlateportCity_BattleTentCorridor_EventScript_EnterBattleRoom

SlateportCity_BattleTentCorridor_EventScript_SwapMons::
	fadescreen FADE_TO_BLACK
	slateporttent_swapmons
	waitstate
	goto_if_eq VAR_RESULT, 1, SlateportCity_BattleTentCorridor_EventScript_EnterBattleRoom
	msgbox BattleFrontier_BattleFactoryPreBattleRoom_Text_YourSwapIsComplete, MSGBOX_DEFAULT
	goto SlateportCity_BattleTentCorridor_EventScript_EnterBattleRoom

SlateportCity_BattleTentCorridor_EventScript_ReadyFor2ndOpponent::
	message BattleFrontier_BattleFactoryPreBattleRoom_Text_ReadyFor2ndOpponent
	waitmessage
	return

SlateportCity_BattleTentCorridor_EventScript_ReadyFor3rdOpponent::
	message BattleFrontier_BattleFactoryPreBattleRoom_Text_ReadyFor3rdOpponent
	waitmessage
	return

SlateportCity_BattleTentCorridor_EventScript_PauseChallenge::
	message BattleFrontier_BattleFactoryPreBattleRoom_Text_SavingDataPleaseWait
	waitmessage
	slateporttent_save CHALLENGE_STATUS_PAUSED
	playse SE_SAVE
	waitse
	fadescreen FADE_TO_BLACK
	frontier_reset
	end

SlateportCity_BattleTentCorridor_EventScript_ResumeChallenge::
	special SavePlayerParty
	factory_setparties 0
	goto SlateportCity_BattleTentCorridor_EventScript_AskReadyForOpponent

SlateportCity_BattleTentCorridor_Movement_PlayerEnter:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

SlateportCity_BattleTentCorridor_Movement_PlayerExit:
	walk_up
	walk_up
	set_invisible
	step_end

SlateportCity_BattleTentCorridor_Movement_AttendantEnter:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

SlateportCity_BattleTentCorridor_Movement_AttendantExit:
	walk_up
	set_invisible
	step_end

@ Leftover text from when this was a Contest Hall in R/S
@ Unused
SlateportCity_ContestHall_Text_AdviceForContests:
	.string "Vuoi un consiglio succoso\n"
	.string "sulle Gare?\p"
	.string "Usare una certa mossa subito dopo\n"
	.string "un certo altro tipo di mossa a volte\l"
	.string "ti fa guadagnare più attenzione.\p"
	.string "Se sai quello che fai, puoi\n"
	.string "fare faville con le esibizioni.\p"
	.string "Certo, gli avversari potrebbero\n"
	.string "cercare di disturbare il tuo Pokémon.$"

@ Unused
SlateportCity_ContestHall_Text_MyPapaIsContestJudge:
	.string "Il mio papà fa il Giudice delle Gare.\p"
	.string "Da grande cosa farò? Il Giudice\n"
	.string "o il Capopalestra?$"

@ Unused
SlateportCity_ContestHall_Text_ImLikeMajorlyCheesed:
	.string "Ehi, bello, sono tipo arrabbiatissimo,\n"
	.string "sai? Cioè, tipo, volevo solo sapere\l"
	.string "perché il mio Pokémon non vince\l"
	.string "mai, sai, tipo, eh?\p"
	.string "Allora, tipo, ho detto la mia\n"
	.string "al Giudice, sai, tanto è gratis.\p"
	.string "E lui, tipo, non mi ha neanche\n"
	.string "ascoltato! Che delusione, bello!\p"
	.string "Ehi, tipo, tu! Zitto, ok?\n"
	.string "Prendi questa e basta!$"

@ Unused
SlateportCity_ContestHall_Text_ExplainTorment:
	.string "Quella è, tipo, la MT41, capito?\n"
	.string "È Attaccalite, mi senti?\p"
	.string "Tipo, impedisce all'avversario di\n"
	.string "usare la stessa mossa due volte di fila.\p"
	.string "E ehi, ascolta bene, tipo:\n"
	.string "non sto mica attaccando lite con te!$"

@ Unused
SlateportCity_ContestHall_Text_MCStepUpTakePartInContest:
	.string "Presentatore: Oh, ma guarda, guarda!\n"
	.string "Che splendore di Pokémon!\p"
	.string "Prego! Si faccia avanti e partecipi\n"
	.string "alle nostre magnifiche Gare!\p"
	.string "Andrà benissimo! Ne sono certo!\n"
	.string "Il mio occhio non sbaglia mai!$"

@ Unused
SlateportCity_ContestHall_Text_JudgeWouldntDoToMissContest:
	.string "Giudice: Ma salve!\n"
	.string "Vedo che sei un Allenatore!\p"
	.string "Allora non puoi proprio perderti\n"
	.string "una Gara Pokémon!\p"
	.string "Prendi un Pass Gare a Mentania\n"
	.string "e iscriviti quando vuoi!$"

@ Unused
SlateportCity_ContestHall_Text_ItsAppealTime:
	.string "È il momento dell'esibizione!\n"
	.string "Con che cosa comincio?$"

@ Unused
SlateportCity_ContestHall_Text_DidntPayAttentionToAppeal:
	.string "Nessuno ha badato all'esibizione\n"
	.string "del mio Pokémon…\p"
	.string "Uffa, quel Giudice non riconosce\n"
	.string "una cosa bella neanche se la vede!$"

@ Unused
SlateportCity_ContestHall_Text_RewardWithSageAdvice:
	.string "Oh, ciao! Devi essere un gran fan,\n"
	.string "per arrivare così vicino al palco.\p"
	.string "Premierò il tuo entusiasmo con\n"
	.string "questo saggio consiglio.\p"
	.string "Se una mossa piace molto,\n"
	.string "il pubblico si scalda.\p"
	.string "E il Pokémon che si esibisce proprio\n"
	.string "quando tutti sono su di giri…\p"
	.string "Beh, viene da pensare che gli succeda\n"
	.string "qualcosa di buono!$"

@ Unused
SlateportCity_ContestHall_Text_MoreFreakedOutThanMon:
	.string "Non ce la faccio! Sono più agitato\n"
	.string "del mio Pokémon.\p"
	.string "Tremo tutto e il cuore mi batte\n"
	.string "all'impazzata!$"

@ Unused
SlateportCity_ContestHall_Text_BattleAndContestAlike:
	.string "Una lotta e una Gara non sono la\n"
	.string "stessa cosa, ma si somigliano.\p"
	.string "Bisogna impegnarsi e credere nei\n"
	.string "Pokémon che hai cresciuto.$"

@ Unused
SlateportCity_ContestHall_Text_MonLooksOnTopOfGame:
	.string "Quel Pokémon sembra in forma\n"
	.string "smagliante, eh?\p"
	.string "Un Pokémon che va bene nella seconda\n"
	.string "fase di giudizio sembra più\l"
	.string "rilassato durante le esibizioni.$"

@ Unused
SlateportCity_ContestHall_Text_MyMonBetterThanThatLot:
	.string "Ma guarda che spettacolo pietoso!\p"
	.string "Eh, il mio Pokémon è di gran lunga\n"
	.string "meglio di quella banda!$"

@ Unused
SlateportCity_ContestHall_Text_GetUrgeToMoveWithMon:
	.string "Non ti viene voglia di muoverti\n"
	.string "insieme ai Pokémon quando fanno\l"
	.string "un'esibizione piena di energia?$"

@ Unused
SlateportCity_ContestHall_Text_HyperRankStage:
	.string "Gare Pokémon\n"
	.string "Palco di Livello Iper!$"

