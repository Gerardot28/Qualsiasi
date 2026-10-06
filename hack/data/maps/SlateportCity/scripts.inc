SlateportCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SlateportCity_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SlateportCity_OnFrame
	.byte 0

SlateportCity_OnTransition:
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 0
	call SlateportCity_EventScript_EnterSlateport
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_MovePeopleForSternInterview
	call_if_eq VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1, SlateportCity_EventScript_SetReadyForScottScene
	end

SlateportCity_EventScript_EnterSlateport::
	setflag FLAG_VISITED_SLATEPORT_CITY
	setvar VAR_CONTEST_HALL_STATE, 0
	setflag FLAG_HIDE_SLATEPORT_CITY_CONTEST_REPORTER
	return

SlateportCity_EventScript_MovePeopleForSternInterview::
	setobjectxyperm LOCALID_SLATEPORT_CAPT_STERN, 28, 13
	setobjectxyperm LOCALID_SLATEPORT_OLD_WOMAN, 25, 13
	setobjectxyperm LOCALID_SLATEPORT_RICH_BOY, 25, 14
	setobjectxyperm LOCALID_SLATEPORT_COOK, 27, 16
	setobjectxyperm LOCALID_SLATEPORT_GIRL, 28, 16
	setobjectxyperm LOCALID_SLATEPORT_FAT_MAN, 29, 16
	setobjectxyperm LOCALID_SLATEPORT_MAN_1, 31, 14
	setobjectmovementtype LOCALID_SLATEPORT_CAPT_STERN, MOVEMENT_TYPE_FACE_DOWN
	setobjectmovementtype LOCALID_SLATEPORT_OLD_WOMAN, MOVEMENT_TYPE_FACE_DOWN_AND_RIGHT
	setobjectmovementtype LOCALID_SLATEPORT_RICH_BOY, MOVEMENT_TYPE_FACE_UP_AND_RIGHT
	setobjectmovementtype LOCALID_SLATEPORT_COOK, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_GIRL, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_FAT_MAN, MOVEMENT_TYPE_FACE_UP
	setobjectmovementtype LOCALID_SLATEPORT_MAN_1, MOVEMENT_TYPE_FACE_LEFT
	return

SlateportCity_EventScript_SetReadyForScottScene::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_eq VAR_0x8004, 30, SlateportCity_EventScript_MoveScottLeft
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 23, 27
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_RIGHT
	return

SlateportCity_EventScript_MoveScottLeft::
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 22, 27
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_RIGHT
	return

SlateportCity_OnFrame:
	map_script_2 VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1, SlateportCity_EventScript_ScottScene
	.2byte 0

SlateportCity_EventScript_ScottScene::
	lockall
	addobject LOCALID_SLATEPORT_SCOTT
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerFaceScott
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottApproachPlayer
	waitmovement 0
	msgbox SlateportCity_Text_YouDroveTeamAquaAway, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_SCOTT, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	delay 60
	msgbox SlateportCity_Text_MaybeThisTrainer, MSGBOX_DEFAULT
	applymovement LOCALID_SLATEPORT_SCOTT, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox SlateportCity_Text_LetsRegisterEachOther, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox SlateportCity_Text_RegisteredScott, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_SCOTT_MATCH_CALL
	msgbox SlateportCity_Text_KeepEyeOnTrainersBeSeeingYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExit
	waitmovement 0
	removeobject LOCALID_SLATEPORT_SCOTT
	setobjectxyperm LOCALID_SLATEPORT_SCOTT, 10, 12
	setobjectmovementtype LOCALID_SLATEPORT_SCOTT, MOVEMENT_TYPE_FACE_DOWN
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 2
	addvar VAR_SCOTT_STATE, 1
	releaseall
	end

SlateportCity_Movement_PlayerFaceScott:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_ScottApproachPlayer:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_Movement_ScottExit:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_EventScript_EnergyGuru::
	lock
	faceplayer
	message SlateportCity_Text_EnergyGuruSellWhatYouNeed
	waitmessage
	pokemart SlateportCity_Pokemart_EnergyGuru
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_Pokemart_EnergyGuru:
	.2byte ITEM_PROTEIN
	.2byte ITEM_IRON
	.2byte ITEM_CARBOS
	.2byte ITEM_ZINC
	.2byte ITEM_CALCIUM
	.2byte ITEM_HP_UP
	pokemartlistend

SlateportCity_EventScript_EffortRibbonWoman::
	lock
	faceplayer
	bufferleadmonspeciesname STR_VAR_1
	msgbox SlateportCity_Text_OhYourPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, LeadMonHasEffortRibbon
	call_if_eq VAR_RESULT, TRUE, SlateportCity_EventScript_MonHasEffortRibbon
	specialvar VAR_RESULT, Special_AreLeadMonEVsMaxedOut
	call_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_MonEVsNotMaxed
	msgbox SlateportCity_Text_PleaseGiveItThisEffortRibbon, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message SlateportCity_Text_ReceivedEffortRibbon
	waitfanfare
	msgbox SlateportCity_Text_PutEffortRibbonOnMon, MSGBOX_DEFAULT
	special GiveLeadMonEffortRibbon
	release
	end

SlateportCity_EventScript_MonEVsNotMaxed::
	msgbox SlateportCity_Text_GoForItLittleHarder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_MonHasEffortRibbon::
	msgbox SlateportCity_Text_EffortRibbonLooksGoodOnIt, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Cook::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_CookSternInterview
	msgbox SlateportCity_Text_SeaweedFullOfLife, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_CookSternInterview::
	msgbox SlateportCity_Text_CaptainComeBackWithBigFish, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_OldWoman::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_OldWomanSternInterview
	msgbox SlateportCity_Text_HowTownIsBornAndGrows, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_OldWomanSternInterview::
	msgbox SlateportCity_Text_CaptSternBeingInterviewed, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Girl::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_GirlSternInterview
	goto_if_set FLAG_RECEIVED_SECRET_POWER, SlateportCity_EventScript_GirlSecretBase
	msgbox SlateportCity_Text_SlateportWonderfulPlace, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_GirlSternInterview::
	msgbox SlateportCity_Text_InterviewerSoCool, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_GirlSecretBase::
	msgbox SlateportCity_Text_BuyBricksSoDecorWontGetDirty, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_RichBoy::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_RichBoySternInterview
	msgbox SlateportCity_Text_GoingToCompeteInBattleTent, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_RichBoySternInterview::
	msgbox SlateportCity_Text_SternSaysDiscoveredSomething, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_FatMan::
	goto_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_FatManSternInterview
	msgbox SlateportCity_Text_BushedHikingFromMauville, MSGBOX_NPC
	end

SlateportCity_EventScript_FatManSternInterview::
	msgbox SlateportCity_Text_AmIOnTV, MSGBOX_SIGN
	end

SlateportCity_EventScript_Man1::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_Man1SternInterview
	msgbox SlateportCity_Text_EveryoneCallsHimCaptStern, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Man1SternInterview::
	msgbox SlateportCity_Text_CaptainsACelebrity, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman1::
	lock
	faceplayer
	goto_if_set FLAG_DOCK_REJECTED_DEVON_GOODS, SlateportCity_EventScript_Woman1AquaGone
	msgbox SlateportCity_Text_WhatsLongLineOverThere, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman1AquaGone::
	msgbox SlateportCity_Text_VisitedMuseumOften, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_BattleTentSign::
	msgbox SlateportCity_Text_BattleTentSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_SternsShipyardSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_EventScript_SternsShipyardFerryComplete
	goto_if_set FLAG_BADGE07_GET, SlateportCity_EventScript_SternsShipyardNearsCompletion
	msgbox SlateportCity_Text_SternsShipyardWantedSign, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_SternsShipyardNearsCompletion::
	msgbox SlateportCity_Text_SternsShipyardNearsCompletion, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_SternsShipyardFerryComplete::
	msgbox SlateportCity_Text_SternsShipyardFerryComplete, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_PokemonFanClubSign::
	msgbox SlateportCity_Text_PokemonFanClubSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_OceanicMuseumSign::
	msgbox SlateportCity_Text_OceanicMuseumSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_CitySign::
	msgbox SlateportCity_Text_CitySign, MSGBOX_SIGN
	end

SlateportCity_EventScript_MarketSign::
	msgbox SlateportCity_Text_MarketSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_HarborSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_EventScript_HarborSignFerryComplete
	msgbox SlateportCity_Text_HarborFerryUnderConstruction, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_HarborSignFerryComplete::
	msgbox SlateportCity_Text_HarborSign, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_EventScript_NameRatersHouseSign::
	msgbox SlateportCity_Text_NameRatersHouseSign, MSGBOX_SIGN
	end

SlateportCity_EventScript_Maniac::
	lock
	faceplayer
	call_if_eq VAR_SLATEPORT_CITY_STATE, 1, SlateportCity_EventScript_ManiacSternInterview
	msgbox SlateportCity_Text_GetNameRaterToHelpYou, MSGBOX_DEFAULT
	release
	end

@ Unclear if the text here was meant to be different, but its not
SlateportCity_EventScript_ManiacSternInterview::
	msgbox SlateportCity_Text_GetNameRaterToHelpYou, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_Woman2::
	msgbox SlateportCity_Text_CantChangeTradeMonName, MSGBOX_NPC
	end

SlateportCity_EventScript_Sailor1::
	msgbox SlateportCity_Text_SeaIsSoWet, MSGBOX_NPC
	end

SlateportCity_EventScript_Sailor2::
	msgbox SlateportCity_Text_SinkOldBoats, MSGBOX_NPC
	end

SlateportCity_EventScript_PokefanF::
	msgbox SlateportCity_Text_BuyTooMuch, MSGBOX_NPC
	end

SlateportCity_EventScript_Man2::
	msgbox SlateportCity_Text_BattleTentBuiltRecently, MSGBOX_NPC
	end

SlateportCity_EventScript_AquaGrunt1::
	lock
	faceplayer
	msgbox SlateportCity_Text_QuitPushing, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt2::
	lock
	faceplayer
	msgbox SlateportCity_Text_AquaHasPolicy, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt3::
	lock
	faceplayer
	msgbox SlateportCity_Text_BossIsBrilliant, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt4::
	lock
	faceplayer
	msgbox SlateportCity_Text_WhatsNewSchemeIWonder, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt5::
	lock
	faceplayer
	msgbox SlateportCity_Text_ShouldTakeItAll, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_5, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt6::
	lock
	faceplayer
	msgbox SlateportCity_Text_DontButtIn, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_6, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt7::
	lock
	faceplayer
	msgbox SlateportCity_Text_RemindsMeOfLongLineForGames, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_7, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt8::
	lock
	faceplayer
	msgbox SlateportCity_Text_WhyAreWeLiningUp, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_8, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_AquaGrunt9::
	lock
	faceplayer
	playse SE_PIN
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_Delay48
	waitmovement 0
	msgbox SlateportCity_Text_WhatDoYouWant, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_Text_IllReadSignForYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	waitmovement 0
	msgbox SlateportCity_Text_SaysSomethingLikeSeaIsEndless, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FacePlayer
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, SlateportCity_Movement_DelayAquaGrunt
	waitmovement 0
	applymovement LOCALID_SLATEPORT_GRUNT_9, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_Movement_DelayAquaGrunt:
	delay_16
	delay_16
	step_end

SlateportCity_EventScript_AquaGrunt10::
	msgbox SlateportCity_Text_ShouldveBroughtMyGameBoy, MSGBOX_SIGN
	end

SlateportCity_EventScript_AquaGrunt11::
	lock
	faceplayer
	msgbox SlateportCity_Text_HotSpringsAfterOperation, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GRUNT_11, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_EventScript_DollClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemartdecoration SlateportCity_PokemartDecor_Dolls
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_PokemartDecor_Dolls:
	.2byte DECOR_AZURILL_DOLL
	.2byte DECOR_MARILL_DOLL
	.2byte DECOR_SKITTY_DOLL
	pokemartlistend

SlateportCity_EventScript_ComeBackWithSecretPower::
	msgbox gText_ComeBackWithSecretPower, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_DecorClerk::
	lock
	faceplayer
	goto_if_unset FLAG_RECEIVED_SECRET_POWER, SlateportCity_EventScript_ComeBackWithSecretPower
	message gText_HowMayIServeYou
	waitmessage
	pokemartdecoration SlateportCity_PokemartDecor
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_PokemartDecor:
	.2byte DECOR_RED_BRICK
	.2byte DECOR_BLUE_BRICK
	.2byte DECOR_YELLOW_BRICK
	.2byte DECOR_RED_BALLOON
	.2byte DECOR_BLUE_BALLOON
	.2byte DECOR_YELLOW_BALLOON
	.2byte DECOR_C_LOW_NOTE_MAT
	.2byte DECOR_D_NOTE_MAT
	.2byte DECOR_E_NOTE_MAT
	.2byte DECOR_F_NOTE_MAT
	.2byte DECOR_G_NOTE_MAT
	.2byte DECOR_A_NOTE_MAT
	.2byte DECOR_B_NOTE_MAT
	.2byte DECOR_C_HIGH_NOTE_MAT
	pokemartlistend

SlateportCity_EventScript_PowerTMClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart SlateportCity_Pokemart_PowerTMs
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
SlateportCity_Pokemart_PowerTMs:
	.2byte ITEM_TM_HIDDEN_POWER
	.2byte ITEM_TM_SECRET_POWER
	pokemartlistend

@ Scene with Capt Sterns interview and Team Aqua announcing plans to steal Submarine
SlateportCity_EventScript_CaptStern::
	lockall
	msgbox SlateportCity_Text_SternMoveAheadWithExploration, MSGBOX_DEFAULT
	msgbox SlateportCity_Text_GabbyWonderfulThanksForInterview, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_GABBY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 10
	applymovement LOCALID_SLATEPORT_TY, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 25
	applymovement LOCALID_SLATEPORT_CAPT_STERN, SlateportCity_Movement_SternWatchGabbyAndTyExit
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerFaceStern
	applymovement LOCALID_SLATEPORT_GABBY, SlateportCity_Movement_GabbyExit
	applymovement LOCALID_SLATEPORT_TY, SlateportCity_Movement_TyExit
	waitmovement 0
	removeobject LOCALID_SLATEPORT_GABBY
	removeobject LOCALID_SLATEPORT_TY
	msgbox SlateportCity_Text_SternWhewFirstInterview, MSGBOX_DEFAULT
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_Text_OhPlayerWeMadeDiscovery, MSGBOX_DEFAULT
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	msgbox SlateportCity_Text_AquaWillAssumeControlOfSubmarine, MSGBOX_DEFAULT
	applymovement LOCALID_SLATEPORT_COOK, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_SLATEPORT_FAT_MAN, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_SLATEPORT_OLD_WOMAN, SlateportCity_Movement_OldWomanConcern
	applymovement LOCALID_SLATEPORT_RICH_BOY, Common_Movement_QuestionMark
	applymovement LOCALID_SLATEPORT_MAN_1, SlateportCity_Movement_ManConcern
	waitmovement 0
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox SlateportCity_Text_SternWhatWasAllThat, MSGBOX_DEFAULT
	playse SE_PIN
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_SLATEPORT_CAPT_STERN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_Text_FromHarborTryingToTakeSub, MSGBOX_DEFAULT
	msgbox SlateportCity_Text_PleaseComeWithMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SLATEPORT_CAPT_STERN, SlateportCity_Movement_SternEnterHarbor
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerEnterHarbor
	waitmovement 0
	removeobject LOCALID_SLATEPORT_CAPT_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_CAPTAIN_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_SUBMARINE_SHADOW
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_AQUA_GRUNT
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_ARCHIE
	setvar VAR_SLATEPORT_CITY_STATE, 2
	warp MAP_SLATEPORT_CITY_HARBOR, 11, 14
	waitstate
	releaseall
	end

SlateportCity_Movement_OldWomanConcern:
	delay_16
	delay_16
	emote_question_mark
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_ManConcern:
	emote_question_mark
	walk_in_place_faster_up
	delay_16
	walk_in_place_faster_down
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

SlateportCity_Movement_GabbyExit:
	delay_16
	walk_left
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_Movement_TyExit:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

SlateportCity_Movement_Unused:
	walk_down
	walk_in_place_faster_up
	step_end

SlateportCity_Movement_SternEnterHarbor:
	walk_right
	walk_up
	walk_up
	set_invisible
	step_end

SlateportCity_Movement_SternWatchGabbyAndTyExit:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_down
	walk_left
	step_end

SlateportCity_Movement_PlayerEnterHarbor:
	walk_in_place_faster_right
	delay_16
	delay_16
	walk_right
	walk_up
	step_end

SlateportCity_Movement_PlayerFaceStern:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_down
	step_end

SlateportCity_EventScript_Ty::
	msgbox SlateportCity_Text_BigSmileForCamera, MSGBOX_SIGN
	end

SlateportCity_EventScript_Gabby::
	msgbox SlateportCity_Text_MostInvaluableExperience, MSGBOX_SIGN
	end

SlateportCity_EventScript_Man3::
	msgbox SlateportCity_Text_WonderIfLighthouseStartlesPokemon, MSGBOX_NPC
	end

SlateportCity_EventScript_BerryPowderClerk::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_POWDER_JAR, SlateportCity_EventScript_ReceivedPowderJar
	msgbox SlateportCity_Text_ExplainBerries, MSGBOX_DEFAULT
	giveitem ITEM_POWDER_JAR
	setflag FLAG_RECEIVED_POWDER_JAR
	msgbox SlateportCity_Text_ExplainBerryPowder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_ReceivedPowderJar::
	setvar VAR_0x8004, 1
	specialvar VAR_RESULT, HasEnoughBerryPowder
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_ExplainBerryPowder
	msgbox SlateportCity_Text_BroughtMeSomeBerryPowder, MSGBOX_DEFAULT
	special DisplayBerryPowderVendorMenu
	goto SlateportCity_EventScript_ChooseBerryPowderItem
	end

SlateportCity_EventScript_ExplainBerryPowder::
	msgbox SlateportCity_Text_ExplainBerryPowder, MSGBOX_DEFAULT
	release
	end

SlateportCity_EventScript_ChooseBerryPowderItem::
	message SlateportCity_Text_ExchangeWhatWithIt
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_BERRY_POWDER_VENDOR
	special ShowScrollableMultichoice
	switch VAR_RESULT
	case 0, SlateportCity_EventScript_EnergyPowder
	case 1, SlateportCity_EventScript_EnergyRoot
	case 2, SlateportCity_EventScript_HealPowder
	case 3, SlateportCity_EventScript_RevivalHerb
	case 4, SlateportCity_EventScript_Protein
	case 5, SlateportCity_EventScript_Iron
	case 6, SlateportCity_EventScript_Carbos
	case 7, SlateportCity_EventScript_Calcium
	case 8, SlateportCity_EventScript_Zinc
	case 9, SlateportCity_EventScript_HPUp
	case 10, SlateportCity_EventScript_PPUp
	case 11, SlateportCity_EventScript_CancelPowderItemSelect
	case MULTI_B_PRESSED, SlateportCity_EventScript_CancelPowderItemSelect
	end

SlateportCity_EventScript_EnergyPowder::
	bufferitemname STR_VAR_1, ITEM_ENERGY_POWDER
	setvar VAR_0x8008, ITEM_ENERGY_POWDER
	setvar VAR_0x8009, 50
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_EnergyRoot::
	bufferitemname STR_VAR_1, ITEM_ENERGY_ROOT
	setvar VAR_0x8008, ITEM_ENERGY_ROOT
	setvar VAR_0x8009, 80
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_HealPowder::
	bufferitemname STR_VAR_1, ITEM_HEAL_POWDER
	setvar VAR_0x8008, ITEM_HEAL_POWDER
	setvar VAR_0x8009, 50
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_RevivalHerb::
	bufferitemname STR_VAR_1, ITEM_REVIVAL_HERB
	setvar VAR_0x8008, ITEM_REVIVAL_HERB
	setvar VAR_0x8009, 300
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Protein::
	bufferitemname STR_VAR_1, ITEM_PROTEIN
	setvar VAR_0x8008, ITEM_PROTEIN
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Iron::
	bufferitemname STR_VAR_1, ITEM_IRON
	setvar VAR_0x8008, ITEM_IRON
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Carbos::
	bufferitemname STR_VAR_1, ITEM_CARBOS
	setvar VAR_0x8008, ITEM_CARBOS
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Calcium::
	bufferitemname STR_VAR_1, ITEM_CALCIUM
	setvar VAR_0x8008, ITEM_CALCIUM
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_Zinc::
	bufferitemname STR_VAR_1, ITEM_ZINC
	setvar VAR_0x8008, ITEM_ZINC
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_HPUp::
	bufferitemname STR_VAR_1, ITEM_HP_UP
	setvar VAR_0x8008, ITEM_HP_UP
	setvar VAR_0x8009, 1000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_PPUp::
	bufferitemname STR_VAR_1, ITEM_PP_UP
	setvar VAR_0x8008, ITEM_PP_UP
	setvar VAR_0x8009, 3000
	goto SlateportCity_EventScript_TryBuyBerryPowderItem
	end

SlateportCity_EventScript_CancelPowderItemSelect::
	msgbox SlateportCity_Text_ComeBackToTradeBerryPowder, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_TryBuyBerryPowderItem::
	msgbox SlateportCity_Text_ExchangeBerryPowderForItem, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SlateportCity_EventScript_ChooseBerryPowderItem
	copyvar VAR_0x8004, VAR_0x8009
	specialvar VAR_RESULT, HasEnoughBerryPowder
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_NotEnoughBerryPowder
	giveitem VAR_0x8008
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_EventScript_NoRoomForBerryPowderItem
	copyvar VAR_0x8004, VAR_0x8009
	special TakeBerryPowder
	special PrintPlayerBerryPowderAmount
	msgbox SlateportCity_Text_FineBerryPowderTradeSomethingElse, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_EventScript_ChooseBerryPowderItem
	msgbox SlateportCity_Text_WhenYouGetMoreBringItToMe, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_NoRoomForBerryPowderItem::
	msgbox gText_TheBagIsFull, MSGBOX_DEFAULT
	special RemoveBerryPowderVendorMenu
	release
	end

SlateportCity_EventScript_NotEnoughBerryPowder::
	msgbox SlateportCity_Text_DontHaveEnoughBerryPowder, MSGBOX_DEFAULT
	goto SlateportCity_EventScript_ChooseBerryPowderItem
	end

SlateportCity_EventScript_ScottBattleTentScene::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	opendoor 10, 12
	waitdooranim
	addobject LOCALID_SLATEPORT_SCOTT
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PushPlayerDown
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExitBattleTent
	waitmovement 0
	closedoor 10, 12
	waitdooranim
	msgbox SlateportCity_Text_TakingBattleTentChallenge, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, SlateportCity_Movement_PlayerWatchScottExit
	applymovement LOCALID_SLATEPORT_SCOTT, SlateportCity_Movement_ScottExitAfterBattleTent
	waitmovement 0
	removeobject LOCALID_SLATEPORT_SCOTT
	addvar VAR_SCOTT_STATE, 1
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 3
	releaseall
	end

SlateportCity_Movement_PushPlayerDown:
	lock_facing_direction
	walk_down
	unlock_facing_direction
	step_end

SlateportCity_Movement_PlayerWatchScottExit:
	delay_16
	walk_in_place_faster_right
	step_end

SlateportCity_Movement_ScottExitBattleTent:
	delay_8
	walk_down
	step_end

SlateportCity_Movement_ScottExitAfterBattleTent:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_EventScript_BerryCrushRankingsSign::
	lockall
	special ShowBerryCrushRankings
	releaseall
	end

SlateportCity_Text_EnergyGuruSellWhatYouNeed:
	.string "Sono l'Energivendolo! Ce la metto tutta\n"
	.string "e ti vendo proprio quello che ti serve!$"

SlateportCity_Text_OhYourPokemon:
	.string "Oh?\n"
	.string "Il tuo {STR_VAR_1}…$"

SlateportCity_Text_PleaseGiveItThisEffortRibbon:
	.string "Si è dato da fare in modo stupendo!\p"
	.string "Come premio, per favore, dagli\n"
	.string "questo Fiocco di Merito.$"

SlateportCity_Text_ReceivedEffortRibbon:
	.string "{PLAYER} riceve il Fiocco di Merito.$"

SlateportCity_Text_PutEffortRibbonOnMon:
	.string "{PLAYER} mette il Fiocco di Merito\n"
	.string "a {STR_VAR_1}.$"

SlateportCity_Text_GoForItLittleHarder:
	.string "Deve impegnarsi ancora un pochino.\p"
	.string "Quando ci riuscirà, darò al tuo\n"
	.string "Pokémon qualcosa di speciale.$"

SlateportCity_Text_EffortRibbonLooksGoodOnIt:
	.string "Oh! Il tuo {STR_VAR_1}! Quel Fiocco\n"
	.string "di Merito gli sta d'incanto!$"

SlateportCity_Text_WonderIfLighthouseStartlesPokemon:
	.string "La luce del faro arriva a decine\n"
	.string "e decine di chilometri.\p"
	.string "Chissà se spaventa i Pokémon del mare…\n"
	.string "o se fa da guida a quelli appena\l"
	.string "sbucati da un Varco.$"

SlateportCity_Text_SeaweedFullOfLife:
	.string "Ooh, guarda un po' qua!\p"
	.string "Le alghe di queste parti sono\n"
	.string "freschissime e piene di vita.\p"
	.string "Sembra quasi che possano alzarsi\n"
	.string "e saltarti addosso!$"

SlateportCity_Text_HowTownIsBornAndGrows:
	.string "Dove l'acqua è limpida, si raccolgono\n"
	.string "i frutti di un raccolto generoso.\p"
	.string "E dove si raduna la gente, presto\n"
	.string "nasce un mercato.\p"
	.string "È così che una città nasce e cresce.$"

SlateportCity_Text_SlateportWonderfulPlace:
	.string "Fare acquisti respirando il profumo\n"
	.string "del mare…\p"
	.string "Porto Selcepoli è una meraviglia!$"

SlateportCity_Text_BuyBricksSoDecorWontGetDirty:
	.string "Se metti Bambole o Cuscini sul\n"
	.string "pavimento, si sporcano.\p"
	.string "Comprerò dei Mattoni, così le mie\n"
	.string "Bambole e i miei Cuscini restano\l"
	.string "puliti quando li lascio in giro.$"

SlateportCity_Text_GoingToCompeteInBattleTent:
	.string "Wroooar! Anch'io voglio sfidare\n"
	.string "il Tendone Lotta!\p"
	.string "Ma prima devo catturare\n"
	.string "qualche Pokémon!$"

SlateportCity_Text_BushedHikingFromMauville:
	.string "Uff… sono distrutto…\p"
	.string "Sono venuto a piedi da Ciclamipoli.\n"
	.string "Ma quanto è grande questa città?!\p"
	.string "Se l'avessi saputo, sarei venuto\n"
	.string "in Bici.$"

SlateportCity_Text_EveryoneCallsHimCaptStern:
	.string "Stern, quello che ha fondato il Museo,\n"
	.string "guida anche una squadra di\l"
	.string "esplorazione sottomarina.\p"
	.string "Per questo tutti lo chiamano\n"
	.string "Capitan Stern.$"

SlateportCity_Text_WhatsLongLineOverThere:
	.string "Cos'è quella roba laggiù?\n"
	.string "Che fila lunghissima…$"

SlateportCity_Text_VisitedMuseumOften:
	.string "Da bambina venivo spesso al Museo.\p"
	.string "Dopo aver visto le sale, sognavo a\n"
	.string "occhi aperti i misteri del mare.$"

SlateportCity_Text_QuitPushing:
	.string "Ehi, tu! Smettila di spingere!\n"
	.string "Questa è la fila, non la vedi? Arrr!$"

SlateportCity_Text_AquaHasPolicy:
	.string "Regola della ciurma del Team Omega:\n"
	.string "ci si raduna sul luogo dell'operazione\l"
	.string "e poi si sparisce come la schiuma!$"

SlateportCity_Text_BossIsBrilliant:
	.string "Il nostro Capo è un genio.\p"
	.string "Ma che se ne farà mai\n"
	.string "di un Museo, adesso?$"

SlateportCity_Text_WhatsNewSchemeIWonder:
	.string "Chissà qual è il nuovo piano…\p"
	.string "Quando il Capo si arrabbia trema\n"
	.string "tutta la nave. Meglio non sbagliare…$"

SlateportCity_Text_ShouldTakeItAll:
	.string "Biglietto da 50, poi ci portiamo\n"
	.string "via tutto il Museo! Arrr!$"

SlateportCity_Text_DontButtIn:
	.string "Ehi, tu là!\n"
	.string "Niente sorpassi, marmocchio!$"

SlateportCity_Text_RemindsMeOfLongLineForGames:
	.string "Che fila lunga, eh?\p"
	.string "Mi ricorda quando facevo la coda\n"
	.string "per i videogiochi più attesi…$"

SlateportCity_Text_WhyAreWeLiningUp:
	.string "Ma perché facciamo la fila e paghiamo?\n"
	.string "Arrembaggio e via, no?!$"

SlateportCity_Text_WhatDoYouWant:
	.string "Eh?\n"
	.string "Che vuoi?$"

SlateportCity_Text_IllReadSignForYou:
	.string "Vuoi leggere il cartello?\n"
	.string "Te lo leggo io!$"

SlateportCity_Text_SaysSomethingLikeSeaIsEndless:
	.string "Vediamo…\p"
	.string "Ehm… mi pare che dica tipo:\n"
	.string "“la vita nel mare non ha confini.”\p"
	.string "Sì, sono quasi sicuro che dica\n"
	.string "proprio così.$"

SlateportCity_Text_ShouldveBroughtMyGameBoy:
	.string "Uffa…\p"
	.string "Dovevo portarmi il Game Boy Advance,\n"
	.string "così non mi annoiavo in fila…\p"
	.string "Uffa…$"

SlateportCity_Text_HotSpringsAfterOperation:
	.string "“Finita l'operazione, vi porto\n"
	.string "tutti alle terme!”\p"
	.string "L'ha detto il nostro Capo.\n"
	.string "Non vedo l'ora!$"

SlateportCity_Text_SeaIsSoWet:
	.string "Il mare è così immenso…\p"
	.string "Che sia nato dalle lacrime\n"
	.string "versate dai Pokémon?$"

SlateportCity_Text_SinkOldBoats:
	.string "Sai che fine fanno le vecchie navi\n"
	.string "troppo malandate per navigare?\p"
	.string "Le affondano in mare, così diventano\n"
	.string "la casa di tanti Pokémon.$"

SlateportCity_Text_BuyTooMuch:
	.string "Ogni volta che vengo qui mi faccio\n"
	.string "prendere la mano e compro troppo.$"

SlateportCity_Text_GetNameRaterToHelpYou:
	.string "Se vuoi cambiare il soprannome dei\n"
	.string "tuoi Pokémon, devi chiedere aiuto\l"
	.string "al Giudice Onomastico.$"

SlateportCity_Text_CantChangeTradeMonName:
	.string "Il soprannome di un Pokémon avuto\n"
	.string "con uno scambio non si può cambiare.\p"
	.string "In quel soprannome c'è tutto l'affetto\n"
	.string "del suo primo Allenatore.$"

SlateportCity_Text_BattleTentBuiltRecently:
	.string "Da poco hanno aperto un Tendone\n"
	.string "Lotta qui a Porto Selcepoli.\p"
	.string "Le Palestre sono divertenti, ma il\n"
	.string "Tendone Lotta ha un fascino tutto suo.\p"
	.string "Vai a scovare Pokémon forti\n"
	.string "per il Tendone Lotta!$"

SlateportCity_Text_CaptSternBeingInterviewed:
	.string "Speravo fosse una star famosa,\n"
	.string "per chiederle un autografo.\p"
	.string "Ma chi stanno intervistando?\n"
	.string "Non è Capitan Stern?$"

SlateportCity_Text_InterviewerSoCool:
	.string "Quella giornalista è bravissima\n"
	.string "e anche carina.\p"
	.string "Da grande farò la reporter! E sarò\n"
	.string "la prima a fare un servizio da un Varco!$"

SlateportCity_Text_SternSaysDiscoveredSomething:
	.string "Capitan Stern dice che hanno scoperto\n"
	.string "qualcosa in fondo al mare.\p"
	.string "Chissà cos'è?\n"
	.string "Cosa potrà mai essere?$"

SlateportCity_Text_CaptainComeBackWithBigFish:
	.string "Che succede qui?\p"
	.string "Il nostro Capitano è tornato dal fondo\n"
	.string "del mare con un pesce gigante?$"

SlateportCity_Text_AmIOnTV:
	.string "Ehi! Mi state guardando?\n"
	.string "Sono in TV?$"

SlateportCity_Text_CaptainsACelebrity:
	.string "Un'intervista in TV! Proprio qui!\n"
	.string "Il Capitano è una celebrità!$"

SlateportCity_Text_BigSmileForCamera:
	.string "Ty: Bene, Capitan Stern, un bel\n"
	.string "sorriso per la telecamera!$"

SlateportCity_Text_MostInvaluableExperience:
	.string "Gabby: Capisco, capisco. Che esperienza\n"
	.string "preziosa ha vissuto…$"

SlateportCity_Text_SternMoveAheadWithExploration:
	.string "Capitan Stern: Proprio così. Andremo\n"
	.string "avanti con l'esplorazione.$"

SlateportCity_Text_GabbyWonderfulThanksForInterview:
	.string "Gabby: Splendido, Capitan Stern!\n"
	.string "Grazie per averci dedicato un po'\l"
	.string "del suo tempo prezioso.\p"
	.string "Speriamo di intervistarla di nuovo\n"
	.string "per raccontare altre scoperte!$"

SlateportCity_Text_SternWhewFirstInterview:
	.string "Capitan Stern: Uff…\p"
	.string "Era la prima volta che mi riprendevano\n"
	.string "per la TV. Che agitazione!$"

SlateportCity_Text_OhPlayerWeMadeDiscovery:
	.string "Oh! {PLAYER}{KUN}!\n"
	.string "Ti trovo in gran forma!\p"
	.string "Nell'ultima esplorazione dei fondali\n"
	.string "abbiamo fatto una scoperta enorme.\p"
	.string "Sul Percorso 128 c'è una grotta\n"
	.string "sottomarina… e laggiù c'è un Varco!\p"
	.string "Crediamo che ci dorma un Pokémon\n"
	.string "che si pensava scomparso da secoli.$"

SlateportCity_Text_AquaWillAssumeControlOfSubmarine:
	.string "Ahrrr-ha-ha!\n"
	.string "Capitan Stern, se non sbaglio!\p"
	.string "Il Team Omega si prende il comando\n"
	.string "del tuo sottomarino!\p"
	.string "Le tue proteste? Acqua salata!\n"
	.string "Pretendiamo piena collaborazione!\p"
	.string "Ahrrr-ha-ha!\n"
	.string "Guarda e impara cosa sa fare\l"
	.string "il Team Omega! Oltre ogni mare!$"

SlateportCity_Text_SternWhatWasAllThat:
	.string "Capitan Stern: Ma che cos'era?\p"
	.string "Sembrava qualcuno che parlava\n"
	.string "al megafono…\p"
	.string "Da dove veniva?$"

SlateportCity_Text_FromHarborTryingToTakeSub:
	.string "Veniva dallo Scalo!\p"
	.string "Il sottomarino!\n"
	.string "Vogliono rubarlo!$"

SlateportCity_Text_PleaseComeWithMe:
	.string "{PLAYER}{KUN}!\n"
	.string "Vieni con me, presto!$"

SlateportCity_Text_BattleTentSign:
	.string "Tendone Lotta di Porto Selcepoli\n"
	.string "“Trova il Pokémon definitivo!”$"

SlateportCity_Text_SternsShipyardWantedSign:
	.string "Cantiere di Stern\p"
	.string "“Cercasi marinaio capace di navigare\n"
	.string "in qualsiasi corrente.”$"

SlateportCity_Text_SternsShipyardNearsCompletion:
	.string "Cantiere di Stern\p"
	.string "“Il traghetto M/N Acqua è quasi\n"
	.string "pronto. Collegherà i porti di\l"
	.string "Porto Selcepoli e Porto Alghepoli.”$"

SlateportCity_Text_SternsShipyardFerryComplete:
	.string "Cantiere di Stern\p"
	.string "“L'imbarco sul traghetto M/N Acqua\n"
	.string "per Porto Alghepoli si effettua\l"
	.string "allo Scalo.”$"

SlateportCity_Text_PokemonFanClubSign:
	.string "Pokémon Fan Club\n"
	.string "“Fan dei Pokémon di ogni mondo, unitevi!”$"

SlateportCity_Text_OceanicMuseumSign:
	.string "“Il mare infinito sostiene\n"
	.string "ogni forma di vita.”\p"
	.string "Museo Oceanografico$"

SlateportCity_Text_CitySign:
	.string "Porto Selcepoli\p"
	.string "“Il porto dove si incrociano le rotte\n"
	.string "di persone e Pokémon… di ogni mondo.”$"

SlateportCity_Text_MarketSign:
	.string "Mercato di Porto Selcepoli\n"
	.string "“Articoli introvabili in tutto Hoenn!”$"

SlateportCity_Text_HarborFerryUnderConstruction:
	.string "Scalo di Porto Selcepoli\p"
	.string "“Il traghetto M/N Acqua è in\n"
	.string "costruzione presso il Cantiere.”\p"
	.string "“Il servizio inizierà\n"
	.string "a breve.”$"

SlateportCity_Text_HarborSign:
	.string "Scalo di Porto Selcepoli\p"
	.string "“Concedetevi una splendida crociera\n"
	.string "a bordo del traghetto M/N Acqua.”$"

SlateportCity_Text_NameRatersHouseSign:
	.string "Casa del Giudice Onomastico\n"
	.string "“Valutiamo i soprannomi dei Pokémon.”$"

SlateportCity_Text_ExplainBerries:
	.string "Quando un Pokémon selvatico si ferisce,\n"
	.string "si cura masticando delle Bacche.\p"
	.string "Lo sapevi?\p"
	.string "Qualcuno l'ha notato e ha avuto l'idea\n"
	.string "di ricavare medicine dalle Bacche.\p"
	.string "Lo sapevi?\p"
	.string "Per trasformare le Bacche in medicine,\n"
	.string "bisogna ridurle in polvere.\p"
	.string "Lo sapevi?\p"
	.string "Vedo che le Bacche ti incuriosiscono\n"
	.string "parecchio!\p"
	.string "Ho proprio quello che fa per\n"
	.string "una persona come te.$"

SlateportCity_Text_ExplainBerryPowder:
	.string "Da poco ho fatto installare nei Centri\n"
	.string "Pokémon delle macchine macina-Bacche.\p"
	.string "I Macinabacche si trovano nella\n"
	.string "Sala Diretta, al piano di sopra.\p"
	.string "Lo sapevi?\p"
	.string "Potresti preparare per me un po' di\n"
	.string "Farina di Bacche con quelle macchine?\p"
	.string "Se me ne porti tanta, ti preparo\n"
	.string "medicine di ogni genere.$"

SlateportCity_Text_BroughtMeSomeBerryPowder:
	.string "Mi hai portato un po' di\n"
	.string "Farina di Bacche?$"

SlateportCity_Text_ExchangeWhatWithIt:
	.string "Con che cosa vorresti\n"
	.string "scambiarla?$"

SlateportCity_Text_ExchangeBerryPowderForItem:
	.string "Bene, vuoi scambiare la tua Farina\n"
	.string "di Bacche con 1 {STR_VAR_1}?$"

SlateportCity_Text_DontHaveEnoughBerryPowder:
	.string "Oh, cielo. Non hai abbastanza\n"
	.string "Farina di Bacche.$"

SlateportCity_Text_FineBerryPowderTradeSomethingElse:
	.string "Che ottima Farina di Bacche!\n"
	.string "Ne verrà una medicina eccellente.\p"
	.string "Vuoi scambiare altra Farina di\n"
	.string "Bacche con qualcos'altro?$"

SlateportCity_Text_WhenYouGetMoreBringItToMe:
	.string "D'accordo! Quando avrai altra Farina\n"
	.string "di Bacche, portamela, mi raccomando!$"

SlateportCity_Text_ComeBackToTradeBerryPowder:
	.string "Torna quando vuoi scambiare la tua\n"
	.string "Farina di Bacche con delle medicine.\p"
	.string "Il mio bazar è sempre aperto.\p"
	.string "Lo sapevi?$"

SlateportCity_Text_YouDroveTeamAquaAway:
	.string "Scott: Uh?\n"
	.string "Noi due ci siamo già visti, mi pare.\p"
	.string "Mi sono già presentato? Se no,\n"
	.string "piacere: mi chiamo Scott.\p"
	.string "Ho appena visto il Team Omega filare\n"
	.string "via di qui come punto da un Beedrill.\p"
	.string "Fammi indovinare… li hai cacciati tu?$"

SlateportCity_Text_MaybeThisTrainer:
	.string "Scott: Hmm…\n"
	.string "Forse, ma proprio forse, questo\l"
	.string "Allenatore…$"

SlateportCity_Text_LetsRegisterEachOther:
	.string "Scott: Deciso! Secondo me noi due\n"
	.string "diventeremo ottimi amici.\p"
	.string "Allora registriamoci a vicenda\n"
	.string "nei nostri PokéNav.\p"
	.string "… … … … … …$"

SlateportCity_Text_RegisteredScott:
	.string "Il contatto di Scott è stato\n"
	.string "registrato nel PokéNav.$"

SlateportCity_Text_KeepEyeOnTrainersBeSeeingYou:
	.string "Scott: Mi piacerebbe seguirti passo\n"
	.string "passo, ma devo tenere d'occhio anche\l"
	.string "il talento di altre persone.\p"
	.string "Sto costruendo qualcosa di grande…\n"
	.string "un posto per Allenatori di ogni mondo.\p"
	.string "Ops, ho parlato troppo! Farò ancora\n"
	.string "un giro per le altre città.\p"
	.string "Ci vediamo, {PLAYER}{KUN}!$"

SlateportCity_Text_TakingBattleTentChallenge:
	.string "Scott: Oh, ehi!\n"
	.string "Ma guarda chi c'è: {PLAYER}{KUN}!\p"
	.string "{PLAYER}{KUN}, fammi indovinare: vuoi\n"
	.string "tentare la sfida del Tendone Lotta?\p"
	.string "Sì! Ottima idea!\n"
	.string "Davvero un'idea eccellente!\p"
	.string "Dopotutto, un Allenatore forte è\n"
	.string "perfetto per il Tendone Lotta!\p"
	.string "Dai il massimo!$"
