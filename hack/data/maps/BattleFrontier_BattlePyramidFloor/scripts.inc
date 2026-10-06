BattleFrontier_BattlePyramidFloor_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, BattleFrontier_BattlePyramidFloor_OnResume
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_BattlePyramidFloor_OnFrame
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_BattlePyramidFloor_OnTransition
	.byte 0

BattleFrontier_BattlePyramidFloor_OnFrame:
	map_script_2 VAR_TEMP_D, 1, BattleFrontier_BattlePyramidFloor_EventScript_UpdateLight
	map_script_2 VAR_TEMP_PLAYING_PYRAMID_MUSIC, 0, BattleFrontier_BattlePyramidFloor_EventScript_PlayPyramidMusic
	map_script_2 VAR_TEMP_F, 1, BattleFrontier_BattlePyramidFloor_EventScript_ShowMapName
	.2byte 0

BattleFrontier_BattlePyramidFloor_EventScript_UpdateLight::
	lockall
	@ pyramid_updatelight, cant use macro because it straddles the loop
	setvar VAR_0x8004, BATTLE_PYRAMID_FUNC_UPDATE_LIGHT
	setvar VAR_0x8005, 4
	setvar VAR_0x8006, PYRAMID_LIGHT_INCR_RADIUS
	setvar VAR_0x8007, SE_SAVE
	setvar VAR_RESULT, 0
BattleFrontier_BattlePyramidFloor_EventScript_UpdateLightLoop::
	special CallBattlePyramidFunction
	delay 2
	goto_if_ne VAR_RESULT, 2, BattleFrontier_BattlePyramidFloor_EventScript_UpdateLightLoop
	setvar VAR_TEMP_D, 0
	releaseall
	end

BattleFrontier_BattlePyramidFloor_EventScript_ShowMapName::
	special ShowMapNamePopup
	setvar VAR_TEMP_F, 0
	end

BattleFrontier_BattlePyramidFloor_EventScript_PlayPyramidMusic::
	playbgm MUS_B_PYRAMID, FALSE
	setvar VAR_TEMP_PLAYING_PYRAMID_MUSIC, 1
	end

BattleFrontier_BattlePyramidFloor_OnResume:
	pyramid_setfloorpal
	frontier_getstatus
	switch VAR_TEMP_CHALLENGE_STATUS
	case 0, BattleFrontier_BattlePyramidFloor_EventScript_ReadyChallenge
	case CHALLENGE_STATUS_SAVING, BattleFrontier_BattlePyramid_EventScript_WarpToLobby
	case CHALLENGE_STATUS_PAUSED, BattleFrontier_BattlePyramidFloor_EventScript_ReadyChallenge
	frontier_get FRONTIER_DATA_BATTLE_OUTCOME
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BattleFrontier_BattlePyramidFloor_EventScript_ResetParty
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BattleFrontier_BattlePyramidFloor_EventScript_ResetParty
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidFloor_EventScript_ResetParty
	goto_if_eq VAR_RESULT, B_OUTCOME_LOST, BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	goto_if_eq VAR_RESULT, B_OUTCOME_DREW, BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	goto_if_eq VAR_RESULT, B_OUTCOME_FORFEITED, BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost
	frontier_isbattletype BATTLE_TYPE_TRAINER  @ VAR_RESULT seems to be ignored here
	setvar VAR_TEMP_D, 1
BattleFrontier_BattlePyramidFloor_EventScript_ResetParty::
	pyramid_resetparty
	end

BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost::
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, CHALLENGE_STATUS_LOST
	pyramid_set PYRAMID_DATA_TRAINER_FLAGS, 255
BattleFrontier_BattlePyramid_EventScript_WarpToLobby::
	pyramid_updatelight 0, PYRAMID_LIGHT_SET_RADIUS
	pyramid_clearhelditems
	special HealPlayerParty
	warpsilent MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY, 7, 13
	waitstate
	end

BattleFrontier_BattlePyramidFloor_EventScript_ReadyChallenge::
	pyramid_save CHALLENGE_STATUS_SAVING
	special SavePlayerParty
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	pyramid_settrainers
	frontier_setpartyorder FRONTIER_PARTY_SIZE
	setvar VAR_TEMP_F, 1
	end

BattleFrontier_BattlePyramidFloor_OnTransition:
	call BattleFrontier_BattlePyramidFloor_EventScript_SetLightRadius
	setvar VAR_TEMP_F, 1
	end

BattleFrontier_BattlePyramidFloor_EventScript_SetLightRadius::
	pyramid_updatelight 32, PYRAMID_LIGHT_SET_RADIUS
	return

BattlePyramid_WarpToNextFloor::
	call BattleFrontier_EventScript_IncrementWinStreak
	frontier_get FRONTIER_DATA_BATTLE_NUM  @ Floor number
	addvar VAR_RESULT, 1
	frontier_set FRONTIER_DATA_BATTLE_NUM, VAR_RESULT
	goto_if_eq VAR_RESULT, 7, BattlePyramid_WarpToTop
	pyramid_seedfloor
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	setvar VAR_RESULT, 0
	warp MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR, 1, 1
	waitstate
	end

BattlePyramid_WarpToTop::
	warp MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_TOP, 17, 17
	waitstate
	end

BattlePyramid_TrainerBattle::
	facilitytrainerbattle FACILITY_BATTLE_PYRAMID
	pyramid_showhint
	waitmessage
	waitbuttonpress
	closemessage
	releaseall
	end

BattlePyramid_FindItemBall::
	pyramid_setitem
	callstd STD_FIND_ITEM
	goto_if_eq VAR_0x8007, 0, BattlePyramid_FindItemBallEnd
	pyramid_hideitem
BattlePyramid_FindItemBallEnd::
	end

BattlePyramid_Retire::
	goto BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost

@ Also used by Trainer Hill
BattleFacility_TrainerBattle_PlaceholderText:
	.string "Questo è un messaggio di prova.$"

gText_BattlePyramidConfirmRest::
	.string "La sfida nella Piramide Lotta verrà\n"
	.string "salvata, così potrai sospenderla.\l"
	.string "Va bene?$"

gText_BattlePyramidConfirmRetire::
	.string "Vuoi davvero abbandonare la sfida\n"
	.string "nella Piramide Lotta?$"

BattlePyramid_Text_ExitHintUp1::
	.string "L'uscita di questo piano è\n"
	.string "da quella parte: {UP_ARROW}$"

BattlePyramid_Text_ExitHintLeft1::
	.string "L'uscita di questo piano è\n"
	.string "da quella parte: {LEFT_ARROW}$"

BattlePyramid_Text_ExitHintRight1::
	.string "L'uscita di questo piano è\n"
	.string "da quella parte: {RIGHT_ARROW}$"

BattlePyramid_Text_ExitHintDown1::
	.string "L'uscita di questo piano è\n"
	.string "da quella parte: {DOWN_ARROW}$"

BattlePyramid_Text_ExitHintUp2::
	.string "Su questo piano l'uscita si trova\n"
	.string "in direzione {UP_ARROW}.$"

BattlePyramid_Text_ExitHintLeft2::
	.string "Su questo piano l'uscita si trova\n"
	.string "in direzione {LEFT_ARROW}.$"

BattlePyramid_Text_ExitHintRight2::
	.string "Su questo piano l'uscita si trova\n"
	.string "in direzione {RIGHT_ARROW}.$"

BattlePyramid_Text_ExitHintDown2::
	.string "Su questo piano l'uscita si trova\n"
	.string "in direzione {DOWN_ARROW}.$"

BattlePyramid_Text_ExitHintUp3::
	.string "L'uscita è laggiù,\n"
	.string "verso {UP_ARROW}.$"

BattlePyramid_Text_ExitHintLeft3::
	.string "L'uscita è laggiù,\n"
	.string "verso {LEFT_ARROW}.$"

BattlePyramid_Text_ExitHintRight3::
	.string "L'uscita è laggiù,\n"
	.string "verso {RIGHT_ARROW}.$"

BattlePyramid_Text_ExitHintDown3::
	.string "L'uscita è laggiù,\n"
	.string "verso {DOWN_ARROW}.$"

BattlePyramid_Text_ExitHintUp4::
	.string "Su questo piano l'uscita è da qualche\n"
	.string "parte in direzione {UP_ARROW}.$"

BattlePyramid_Text_ExitHintLeft4::
	.string "Su questo piano l'uscita è da qualche\n"
	.string "parte in direzione {LEFT_ARROW}.$"

BattlePyramid_Text_ExitHintRight4::
	.string "Su questo piano l'uscita è da qualche\n"
	.string "parte in direzione {RIGHT_ARROW}.$"

BattlePyramid_Text_ExitHintDown4::
	.string "Su questo piano l'uscita è da qualche\n"
	.string "parte in direzione {DOWN_ARROW}.$"

BattlePyramid_Text_ExitHintUp5::
	.string "L'uscita?\n"
	.string "Di là, verso {UP_ARROW}.$"

BattlePyramid_Text_ExitHintLeft5::
	.string "L'uscita?\n"
	.string "Di là, verso {LEFT_ARROW}.$"

BattlePyramid_Text_ExitHintRight5::
	.string "L'uscita?\n"
	.string "Di là, verso {RIGHT_ARROW}.$"

BattlePyramid_Text_ExitHintDown5::
	.string "L'uscita?\n"
	.string "Di là, verso {DOWN_ARROW}.$"

BattlePyramid_Text_ExitHintUp6::
	.string "Qui le uscite cambiano come i Varchi,\n"
	.string "ma stavolta è in direzione {UP_ARROW}.$"

BattlePyramid_Text_ExitHintLeft6::
	.string "Qui le uscite cambiano come i Varchi,\n"
	.string "ma stavolta è in direzione {LEFT_ARROW}.$"

BattlePyramid_Text_ExitHintRight6::
	.string "Qui le uscite cambiano come i Varchi,\n"
	.string "ma stavolta è in direzione {RIGHT_ARROW}.$"

BattlePyramid_Text_ExitHintDown6::
	.string "Qui le uscite cambiano come i Varchi,\n"
	.string "ma stavolta è in direzione {DOWN_ARROW}.$"

BattlePyramid_Text_EightItemsRemaining1::
	.string "Stai cercando strumenti?\p"
	.string "Su questo piano ne restano ancora\n"
	.string "otto da trovare.$"

BattlePyramid_Text_SevenItemsRemaining1::
	.string "Stai cercando strumenti?\p"
	.string "Su questo piano ne restano ancora\n"
	.string "sette da trovare.$"

BattlePyramid_Text_SixItemsRemaining1::
	.string "Stai cercando strumenti?\p"
	.string "Su questo piano ne restano ancora\n"
	.string "sei da trovare.$"

BattlePyramid_Text_FiveItemsRemaining1::
	.string "Stai cercando strumenti?\p"
	.string "Su questo piano ne restano ancora\n"
	.string "cinque da trovare.$"

BattlePyramid_Text_FourItemsRemaining1::
	.string "Stai cercando strumenti?\p"
	.string "Su questo piano ne restano ancora\n"
	.string "quattro da trovare.$"

BattlePyramid_Text_ThreeItemsRemaining1::
	.string "Stai cercando strumenti?\p"
	.string "Su questo piano ne restano ancora\n"
	.string "tre da trovare.$"

BattlePyramid_Text_TwoItemsRemaining1::
	.string "Stai cercando strumenti?\p"
	.string "Su questo piano ne restano ancora\n"
	.string "due da trovare.$"

BattlePyramid_Text_OneItemRemaining1::
	.string "Stai cercando strumenti?\p"
	.string "Su questo piano ne resta ancora\n"
	.string "uno solo da trovare.$"

BattlePyramid_Text_ZeroItemsRemaining1::
	.string "Stai cercando strumenti?\p"
	.string "Qui non è rimasto più niente\n"
	.string "da trovare!$"

BattlePyramid_Text_EightItemsRemaining2::
	.string "Hai vinto, quindi ti svelo\n"
	.string "un piccolo segreto!\p"
	.string "Qui in giro ci sono\n"
	.string "otto strumenti.$"

BattlePyramid_Text_SevenItemsRemaining2::
	.string "Hai vinto, quindi ti svelo\n"
	.string "un piccolo segreto!\p"
	.string "Qui in giro sono rimasti\n"
	.string "sette strumenti.$"

BattlePyramid_Text_SixItemsRemaining2::
	.string "Hai vinto, quindi ti svelo\n"
	.string "un piccolo segreto!\p"
	.string "Qui in giro sono rimasti\n"
	.string "sei strumenti.$"

BattlePyramid_Text_FiveItemsRemaining2::
	.string "Hai vinto, quindi ti svelo\n"
	.string "un piccolo segreto!\p"
	.string "Qui in giro sono rimasti\n"
	.string "cinque strumenti.$"

BattlePyramid_Text_FourItemsRemaining2::
	.string "Hai vinto, quindi ti svelo\n"
	.string "un piccolo segreto!\p"
	.string "Qui in giro sono rimasti\n"
	.string "quattro strumenti.$"

BattlePyramid_Text_ThreeItemsRemaining2::
	.string "Hai vinto, quindi ti svelo\n"
	.string "un piccolo segreto!\p"
	.string "Qui in giro sono rimasti\n"
	.string "tre strumenti.$"

BattlePyramid_Text_TwoItemsRemaining2::
	.string "Hai vinto, quindi ti svelo\n"
	.string "un piccolo segreto!\p"
	.string "Qui in giro sono rimasti\n"
	.string "due strumenti.$"

BattlePyramid_Text_OneItemRemaining2::
	.string "Hai vinto, quindi ti svelo\n"
	.string "un piccolo segreto!\p"
	.string "Qui in giro è rimasto\n"
	.string "un solo strumento.$"

BattlePyramid_Text_ZeroItemsRemaining2::
	.string "Hai vinto, quindi ti svelo\n"
	.string "un piccolo segreto!\p"
	.string "Qui in giro non è rimasto\n"
	.string "più nessuno strumento.$"

BattlePyramid_Text_EightItemsRemaining3::
	.string "Come siamo messi a strumenti, eh?\p"
	.string "Mi sa che su questo piano ce ne sono\n"
	.string "ancora otto, che aspettano solo te.$"

BattlePyramid_Text_SevenItemsRemaining3::
	.string "Come siamo messi a strumenti, eh?\p"
	.string "Mi sa che su questo piano ce ne sono\n"
	.string "ancora sette, che aspettano solo te.$"

BattlePyramid_Text_SixItemsRemaining3::
	.string "Come siamo messi a strumenti, eh?\p"
	.string "Mi sa che su questo piano ce ne sono\n"
	.string "ancora sei, che aspettano solo te.$"

BattlePyramid_Text_FiveItemsRemaining3::
	.string "Come siamo messi a strumenti, eh?\p"
	.string "Mi sa che su questo piano ce ne sono\n"
	.string "ancora cinque, che aspettano solo te.$"

BattlePyramid_Text_FourItemsRemaining3::
	.string "Come siamo messi a strumenti, eh?\p"
	.string "Mi sa che su questo piano ce ne sono\n"
	.string "ancora quattro, che aspettano solo te.$"

BattlePyramid_Text_ThreeItemsRemaining3::
	.string "Come siamo messi a strumenti, eh?\p"
	.string "Mi sa che su questo piano ce ne sono\n"
	.string "ancora tre, che aspettano solo te.$"

BattlePyramid_Text_TwoItemsRemaining3::
	.string "Come siamo messi a strumenti, eh?\p"
	.string "Mi sa che su questo piano ce ne sono\n"
	.string "ancora due, che aspettano solo te.$"

BattlePyramid_Text_OneItemRemaining3::
	.string "Come siamo messi a strumenti, eh?\p"
	.string "Mi sa che su questo piano ne resta\n"
	.string "uno solo, che aspetta di essere scovato.$"

BattlePyramid_Text_ZeroItemsRemaining3::
	.string "Come siamo messi a strumenti, eh?\p"
	.string "Mi sa che su questo piano non c'è\n"
	.string "più niente da scovare.\l"
	.string "Abbi cura di te, eh!$"

BattlePyramid_Text_EightItemsRemaining4::
	.string "Sei forte, quindi ti meriti\n"
	.string "un indizio!\p"
	.string "Pare che per terra ci siano\n"
	.string "altri otto strumenti.$"

BattlePyramid_Text_SevenItemsRemaining4::
	.string "Sei forte, quindi ti meriti\n"
	.string "un indizio!\p"
	.string "Pare che per terra ci siano\n"
	.string "altri sette strumenti.$"

BattlePyramid_Text_SixItemsRemaining4::
	.string "Sei forte, quindi ti meriti\n"
	.string "un indizio!\p"
	.string "Pare che per terra ci siano\n"
	.string "altri sei strumenti.$"

BattlePyramid_Text_FiveItemsRemaining4::
	.string "Sei forte, quindi ti meriti\n"
	.string "un indizio!\p"
	.string "Pare che per terra ci siano\n"
	.string "altri cinque strumenti.$"

BattlePyramid_Text_FourItemsRemaining4::
	.string "Sei forte, quindi ti meriti\n"
	.string "un indizio!\p"
	.string "Pare che per terra ci siano\n"
	.string "altri quattro strumenti.$"

BattlePyramid_Text_ThreeItemsRemaining4::
	.string "Sei forte, quindi ti meriti\n"
	.string "un indizio!\p"
	.string "Pare che per terra ci siano\n"
	.string "altri tre strumenti.$"

BattlePyramid_Text_TwoItemsRemaining4::
	.string "Sei forte, quindi ti meriti\n"
	.string "un indizio!\p"
	.string "Pare che per terra ci siano\n"
	.string "altri due strumenti.$"

BattlePyramid_Text_OneItemRemaining4::
	.string "Sei forte, quindi ti meriti\n"
	.string "un indizio!\p"
	.string "Pare che per terra ci sia\n"
	.string "un solo altro strumento.$"

BattlePyramid_Text_ZeroItemsRemaining4::
	.string "Sei forte, quindi ti meriti\n"
	.string "un indizio!\p"
	.string "Pare che per terra non ci sia\n"
	.string "più nessuno strumento.$"

BattlePyramid_Text_EightItemsRemaining5::
	.string "Si dice che su questo piano della\n"
	.string "Piramide ci siano otto strumenti…$"

BattlePyramid_Text_SevenItemsRemaining5::
	.string "Si dice che su questo piano della\n"
	.string "Piramide ci siano sette strumenti…$"

BattlePyramid_Text_SixItemsRemaining5::
	.string "Si dice che su questo piano della\n"
	.string "Piramide ci siano sei strumenti…$"

BattlePyramid_Text_FiveItemsRemaining5::
	.string "Si dice che su questo piano della\n"
	.string "Piramide ci siano cinque strumenti…$"

BattlePyramid_Text_FourItemsRemaining5::
	.string "Si dice che su questo piano della\n"
	.string "Piramide ci siano quattro strumenti…$"

BattlePyramid_Text_ThreeItemsRemaining5::
	.string "Si dice che su questo piano della\n"
	.string "Piramide ci siano tre strumenti…$"

BattlePyramid_Text_TwoItemsRemaining5::
	.string "Si dice che su questo piano della\n"
	.string "Piramide ci siano due strumenti…$"

BattlePyramid_Text_OneItemRemaining5::
	.string "Si dice che su questo piano della\n"
	.string "Piramide ci sia un solo strumento…$"

BattlePyramid_Text_ZeroItemsRemaining5::
	.string "Si dice che su questo piano della\n"
	.string "Piramide non resti più nulla…$"

BattlePyramid_Text_EightItemsRemaining6::
	.string "Hai raccolto qualche strumento?\p"
	.string "Credo che su questo piano ce ne siano\n"
	.string "ancora otto.$"

BattlePyramid_Text_SevenItemsRemaining6::
	.string "Hai raccolto qualche strumento?\p"
	.string "Credo che su questo piano ce ne siano\n"
	.string "ancora sette.$"

BattlePyramid_Text_SixItemsRemaining6::
	.string "Hai raccolto qualche strumento?\p"
	.string "Credo che su questo piano ce ne siano\n"
	.string "ancora sei.$"

BattlePyramid_Text_FiveItemsRemaining6::
	.string "Hai raccolto qualche strumento?\p"
	.string "Credo che su questo piano ce ne siano\n"
	.string "ancora cinque.$"

BattlePyramid_Text_FourItemsRemaining6::
	.string "Hai raccolto qualche strumento?\p"
	.string "Credo che su questo piano ce ne siano\n"
	.string "ancora quattro.$"

BattlePyramid_Text_ThreeItemsRemaining6::
	.string "Hai raccolto qualche strumento?\p"
	.string "Credo che su questo piano ce ne siano\n"
	.string "ancora tre.$"

BattlePyramid_Text_TwoItemsRemaining6::
	.string "Hai raccolto qualche strumento?\p"
	.string "Credo che su questo piano ce ne siano\n"
	.string "ancora due.$"

BattlePyramid_Text_OneItemRemaining6::
	.string "Hai raccolto qualche strumento?\p"
	.string "Credo che su questo piano ce ne sia\n"
	.string "ancora uno.$"

BattlePyramid_Text_ZeroItemsRemaining6::
	.string "Hai raccolto qualche strumento?\p"
	.string "Credo che su questo piano non ce ne\n"
	.string "siano più.$"

BattlePyramid_Text_SevenTrainersRemaining1::
	.string "Sei stato davvero fortissimo!\p"
	.string "Ma qui ci sono altri sette Allenatori\n"
	.string "tosti, a parte me!$"

BattlePyramid_Text_SixTrainersRemaining1::
	.string "Sei stato davvero fortissimo!\p"
	.string "Ma qui ci sono altri sei Allenatori\n"
	.string "tosti, a parte me!$"

BattlePyramid_Text_FiveTrainersRemaining1::
	.string "Sei stato davvero fortissimo!\p"
	.string "Ma qui ci sono altri cinque Allenatori\n"
	.string "tosti, a parte me!$"

BattlePyramid_Text_FourTrainersRemaining1::
	.string "Sei stato davvero fortissimo!\p"
	.string "Ma qui ci sono altri quattro Allenatori\n"
	.string "tosti, a parte me!$"

BattlePyramid_Text_ThreeTrainersRemaining1::
	.string "Sei stato davvero fortissimo!\p"
	.string "Ma qui ci sono altri tre Allenatori\n"
	.string "tosti, a parte me!$"

BattlePyramid_Text_TwoTrainersRemaining1::
	.string "Sei stato davvero fortissimo!\p"
	.string "Ma qui ci sono altri due Allenatori\n"
	.string "tosti, a parte me!$"

BattlePyramid_Text_OneTrainersRemaining1::
	.string "Sei stato davvero fortissimo!\p"
	.string "Ma qui c'è ancora un Allenatore\n"
	.string "tosto, a parte me!$"

BattlePyramid_Text_ZeroTrainersRemaining1::
	.string "Sei stato davvero fortissimo!\p"
	.string "Non è rimasto nessuno\n"
	.string "in grado di batterti!$"

BattlePyramid_Text_SevenTrainersRemaining2::
	.string "Che rabbia, non ci posso credere!\p"
	.string "Ma restano altri sette Allenatori!\n"
	.string "Qualcuno ti farà abbassare la cresta!$"

BattlePyramid_Text_SixTrainersRemaining2::
	.string "Che rabbia, non ci posso credere!\p"
	.string "Ma restano altri sei Allenatori!\n"
	.string "Qualcuno ti farà abbassare la cresta!$"

BattlePyramid_Text_FiveTrainersRemaining2::
	.string "Che rabbia, non ci posso credere!\p"
	.string "Ma restano altri cinque Allenatori!\n"
	.string "Qualcuno ti farà abbassare la cresta!$"

BattlePyramid_Text_FourTrainersRemaining2::
	.string "Che rabbia, non ci posso credere!\p"
	.string "Ma restano altri quattro Allenatori!\n"
	.string "Qualcuno ti farà abbassare la cresta!$"

BattlePyramid_Text_ThreeTrainersRemaining2::
	.string "Che rabbia, non ci posso credere!\p"
	.string "Ma restano altri tre Allenatori!\n"
	.string "Qualcuno ti farà abbassare la cresta!$"

BattlePyramid_Text_TwoTrainersRemaining2::
	.string "Che rabbia, non ci posso credere!\p"
	.string "Ma restano altri due Allenatori!\n"
	.string "Qualcuno ti farà abbassare la cresta!$"

BattlePyramid_Text_OneTrainersRemaining2::
	.string "Che rabbia, non ci posso credere!\p"
	.string "Ma resta ancora un Allenatore!\n"
	.string "Vedrai che ti farà abbassare la cresta!$"

BattlePyramid_Text_ZeroTrainersRemaining2::
	.string "Che rabbia, non ci posso credere!\p"
	.string "Ma non ci sono più Allenatori\n"
	.string "in grado di sfidarti!$"

BattlePyramid_Text_SevenTrainersRemaining3::
	.string "Notevole, davvero!\p"
	.string "Ma su questo piano ci sono altri sette\n"
	.string "Allenatori. Li batterai tutti?$"

BattlePyramid_Text_SixTrainersRemaining3::
	.string "Notevole, davvero!\p"
	.string "Ma su questo piano ci sono altri sei\n"
	.string "Allenatori. Li batterai tutti?$"

BattlePyramid_Text_FiveTrainersRemaining3::
	.string "Notevole, davvero!\p"
	.string "Ma su questo piano ci sono altri cinque\n"
	.string "Allenatori. Li batterai tutti?$"

BattlePyramid_Text_FourTrainersRemaining3::
	.string "Notevole, davvero!\p"
	.string "Ma su questo piano ci sono altri quattro\n"
	.string "Allenatori. Li batterai tutti?$"

BattlePyramid_Text_ThreeTrainersRemaining3::
	.string "Notevole, davvero!\p"
	.string "Ma su questo piano ci sono altri tre\n"
	.string "Allenatori. Li batterai tutti?$"

BattlePyramid_Text_TwoTrainersRemaining3::
	.string "Notevole, davvero!\p"
	.string "Ma su questo piano ci sono altri due\n"
	.string "Allenatori. Li batterai entrambi?$"

BattlePyramid_Text_OneTrainersRemaining3::
	.string "Notevole, davvero!\p"
	.string "Ma su questo piano c'è ancora un\n"
	.string "Allenatore. Riuscirai a spuntarla?$"

BattlePyramid_Text_ZeroTrainersRemaining3::
	.string "Notevole, davvero!\p"
	.string "Hai superato tutti gli Allenatori\n"
	.string "di questo piano.$"

BattlePyramid_Text_SevenTrainersRemaining4::
	.string "Forse potresti fare piazza pulita\n"
	.string "dei sette Allenatori di questo piano.$"

BattlePyramid_Text_SixTrainersRemaining4::
	.string "Forse potresti fare piazza pulita\n"
	.string "dei sei Allenatori di questo piano.$"

BattlePyramid_Text_FiveTrainersRemaining4::
	.string "Forse potresti fare piazza pulita\n"
	.string "dei cinque Allenatori di questo piano.$"

BattlePyramid_Text_FourTrainersRemaining4::
	.string "Forse potresti fare piazza pulita\n"
	.string "dei quattro Allenatori di questo piano.$"

BattlePyramid_Text_ThreeTrainersRemaining4::
	.string "Forse potresti fare piazza pulita\n"
	.string "dei tre Allenatori di questo piano.$"

BattlePyramid_Text_TwoTrainersRemaining4::
	.string "Forse potresti fare piazza pulita\n"
	.string "dei due Allenatori di questo piano.$"

BattlePyramid_Text_OneTrainersRemaining4::
	.string "Forse potresti completare l'opera\n"
	.string "con l'ultimo Allenatore di questo piano.$"

BattlePyramid_Text_ZeroTrainersRemaining4::
	.string "Non c'è più nessuno, qui, che possa\n"
	.string "batterti, ormai…$"

BattlePyramid_Text_SevenTrainersRemaining5::
	.string "Forse hai la stoffa per battere\n"
	.string "i sette Allenatori esperti rimasti.$"

BattlePyramid_Text_SixTrainersRemaining5::
	.string "Forse hai la stoffa per battere\n"
	.string "i sei Allenatori esperti rimasti.$"

BattlePyramid_Text_FiveTrainersRemaining5::
	.string "Forse hai la stoffa per battere\n"
	.string "i cinque Allenatori esperti rimasti.$"

BattlePyramid_Text_FourTrainersRemaining5::
	.string "Forse hai la stoffa per battere\n"
	.string "i quattro Allenatori esperti rimasti.$"

BattlePyramid_Text_ThreeTrainersRemaining5::
	.string "Forse hai la stoffa per battere\n"
	.string "i tre Allenatori esperti rimasti.$"

BattlePyramid_Text_TwoTrainersRemaining5::
	.string "Forse hai la stoffa per battere\n"
	.string "i due Allenatori esperti rimasti.$"

BattlePyramid_Text_OneTrainersRemaining5::
	.string "Forse hai la stoffa per battere\n"
	.string "l'unico Allenatore esperto rimasto.$"

BattlePyramid_Text_ZeroTrainersRemaining5::
	.string "Le tue abilità sono impeccabili.\p"
	.string "Qui non c'è più nessun Allenatore\n"
	.string "che abbia speranza di batterti.$"

BattlePyramid_Text_SevenTrainersRemaining6::
	.string "Riuscirai a vincere ancora contro\n"
	.string "i sette Allenatori rimasti?$"

BattlePyramid_Text_SixTrainersRemaining6::
	.string "Riuscirai a vincere ancora contro\n"
	.string "i sei Allenatori rimasti?$"

BattlePyramid_Text_FiveTrainersRemaining6::
	.string "Riuscirai a vincere ancora contro\n"
	.string "i cinque Allenatori rimasti?$"

BattlePyramid_Text_FourTrainersRemaining6::
	.string "Riuscirai a vincere ancora contro\n"
	.string "i quattro Allenatori rimasti?$"

BattlePyramid_Text_ThreeTrainersRemaining6::
	.string "Riuscirai a vincere ancora contro\n"
	.string "i tre Allenatori rimasti?$"

BattlePyramid_Text_TwoTrainersRemaining6::
	.string "Riuscirai a vincere ancora contro\n"
	.string "i due Allenatori rimasti?$"

BattlePyramid_Text_OneTrainersRemaining6::
	.string "Riuscirai a vincere ancora contro\n"
	.string "l'ultimo Allenatore rimasto?$"

BattlePyramid_Text_ZeroTrainersRemaining6::
	.string "Non c'è più nessun Allenatore\n"
	.string "in grado di affrontarti…$"
