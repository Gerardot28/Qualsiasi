BattleFrontier_BattlePyramidLobby_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_BattlePyramidLobby_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, BattleFrontier_BattleDomeLobby_OnWarp
	.byte 0

@ Unused. Pyramid uses Dome's OnWarp (presumably by mistake). Their effects are identical
BattleFrontier_BattlePyramidLobby_OnWarp:
	map_script_2 VAR_TEMP_1, 0, BattleFrontier_BattlePyramidLobby_EventScript_TurnPlayerNorth
	.2byte 0

BattleFrontier_BattlePyramidLobby_EventScript_TurnPlayerNorth:
	setvar VAR_TEMP_1, 1
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

BattleFrontier_BattlePyramidLobby_OnFrame:
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, 0, BattleFrontier_BattlePyramidLobby_EventScript_GetChallengeStatus
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_SAVING, BattleFrontier_BattlePyramidLobby_EventScript_QuitWithoutSaving
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_PAUSED, BattleFrontier_BattlePyramidLobby_EventScript_ResumeChallenge
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_WON, BattleFrontier_BattlePyramidLobby_EventScript_WonChallenge
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_LOST, BattleFrontier_BattlePyramidLobby_EventScript_LostChallenge
	.2byte 0

BattleFrontier_BattlePyramidLobby_EventScript_GetChallengeStatus::
	frontier_getstatus
	end

BattleFrontier_BattlePyramidLobby_EventScript_QuitWithoutSaving::
	lockall
	message BattleFrontier_BattlePyramidLobby_Text_DidntSaveBeforeQuittingTakeBag
	waitmessage
	playse SE_EXP_MAX
	waitse
	closemessage
	pyramid_set PYRAMID_DATA_WIN_STREAK, 0
	pyramid_set PYRAMID_DATA_WIN_STREAK_ACTIVE, FALSE
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattlePyramidLobby_EventScript_WonChallenge::
	lockall
	frontier_isbrain
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_BattlePyramidLobby_EventScript_DefeatedKing
	msgbox BattleFrontier_BattlePyramidLobby_Text_YouveConqueredPyramid, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_GiveBattlePoints

BattleFrontier_BattlePyramidLobby_EventScript_DefeatedKing::
	msgbox BattleFrontier_BattlePyramidLobby_Text_YouveDefeatedPyramidKing, MSGBOX_DEFAULT
BattleFrontier_BattlePyramidLobby_EventScript_GiveBattlePoints::
	special DoBattlePyramidMonsHaveHeldItem
	call_if_eq VAR_RESULT, TRUE, BattleFrontier_BattlePyramidLobby_EventScript_StoreHeldItemsInPyramidBag
	clearflag FLAG_STORING_ITEMS_IN_PYRAMID_BAG
	frontier_checkairshow
	special LoadPlayerParty
	pyramid_clearhelditems
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	message BattleFrontier_BattlePyramidLobby_Text_UsedBattleBagWillBeKept
	waitmessage
	playse SE_EXP_MAX
	waitse
	msgbox BattleFrontier_BattlePyramidLobby_Text_GiveYouTheseBattlePoints, MSGBOX_DEFAULT
	frontier_givepoints
	msgbox BattleFrontier_Text_ObtainedXBattlePoints, MSGBOX_GETPOINTS
	message BattleFrontier_BattlePyramidLobby_Text_RecordResultsWait
	waitmessage
	pyramid_setprize
	pyramid_save 0
	playse SE_SAVE
	waitse
	msgbox BattleFrontier_BattlePyramidLobby_Text_LookForwardToNextChallenge, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattlePyramidLobby_EventScript_LostChallenge::
	frontier_checkairshow
	special LoadPlayerParty
	pyramid_clearhelditems
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	lockall
	message BattleFrontier_BattlePyramidLobby_Text_DisappointingHereIsBag
	waitmessage
	playse SE_EXP_MAX
	waitse
	message BattleFrontier_BattlePyramidLobby_Text_RecordResultsWait
	waitmessage
	pyramid_set PYRAMID_DATA_WIN_STREAK_ACTIVE, FALSE
	pyramid_save 0
	playse SE_SAVE
	waitse
	msgbox BattleFrontier_BattlePyramidLobby_Text_LookForwardToNextChallenge, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattlePyramidLobby_EventScript_ResumeChallenge::
	goto BattleFrontier_BattlePyramidLobby_EventScript_EnterChallenge

BattleFrontier_BattlePyramidLobby_EventScript_Attendant::
	lock
	faceplayer
	setvar VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_PYRAMID
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES
	special SavePlayerParty
	msgbox BattleFrontier_BattlePyramidLobby_Text_WelcomeToBattlePyramid, MSGBOX_DEFAULT
BattleFrontier_BattlePyramidLobby_EventScript_AskTakeChallenge::
	message BattleFrontier_BattlePyramidLobby_Text_EmbarkOnChallenge
	waitmessage
	multichoice 17, 6, MULTI_CHALLENGEINFO, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_BattlePyramidLobby_EventScript_TryEnterChallenge
	case 1, BattleFrontier_BattlePyramidLobby_EventScript_ExplainChallenge
	case 2, BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge

BattleFrontier_BattlePyramidLobby_EventScript_TryEnterChallenge::
	message BattleFrontier_BattlePyramidLobby_Text_WhichLevelMode
	waitmessage
	multichoice 17, 6, MULTI_LEVEL_MODE, FALSE
	switch VAR_RESULT
	case FRONTIER_LVL_TENT, BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge
	frontier_checkineligible
	goto_if_eq VAR_0x8004, TRUE, BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMons
	frontier_set FRONTIER_DATA_LVL_MODE, VAR_RESULT
	msgbox BattleFrontier_BattlePyramidLobby_Text_SelectThreeMons, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	callnative RemoveFollowingPokemon
	call BattleFrontier_EventScript_GetLvlMode
	copyvar VAR_0x8004, VAR_RESULT
	setvar VAR_0x8005, FRONTIER_PARTY_SIZE
	special ChoosePartyForBattleFrontier
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidLobby_EventScript_LoadPartyAndCancelChallenge
	msgbox BattleFrontier_BattlePyramidLobby_Text_OkayToSaveBeforeChallenge, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, BattleFrontier_BattlePyramidLobby_EventScript_LoadPartyAndCancelChallenge
	case YES, BattleFrontier_BattlePyramidLobby_EventScript_SaveBeforeChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_LoadPartyAndCancelChallenge

BattleFrontier_BattlePyramidLobby_EventScript_SaveBeforeChallenge::
	setvar VAR_TEMP_CHALLENGE_STATUS, 0
	frontier_set FRONTIER_DATA_SELECTED_MON_ORDER
	pyramid_init
	pyramid_set PYRAMID_DATA_WIN_STREAK_ACTIVE, TRUE
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, CHALLENGE_STATUS_SAVING
	frontier_set FRONTIER_DATA_PAUSED, FALSE
	pyramid_seedfloor
	pyramid_settrainers
	special LoadPlayerParty
	closemessage
	delay 2
	call Common_EventScript_SaveGame
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidLobby_EventScript_CancelChallengeSaveFailed
BattleFrontier_BattlePyramidLobby_EventScript_EnterChallenge::
	special SavePlayerParty
	frontier_setpartyorder FRONTIER_PARTY_SIZE
	msgbox BattleFrontier_BattlePyramidLobby_Text_ShowYouIntoPyramid, MSGBOX_DEFAULT
	closemessage
	call BattleFrontier_BattlePyramidLobby_EventScript_WalkToPanelAndReceiveBag
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	setvar VAR_RESULT, 0
	special HealPlayerParty
	warp MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR, 1, 1
	setvar VAR_TEMP_CHALLENGE_STATUS, 0
	waitstate
	end

BattleFrontier_BattlePyramidLobby_EventScript_ExplainChallenge::
	msgbox BattleFrontier_BattlePyramidLobby_Text_ExplainBattlePyramid, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_AskTakeChallenge

BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMons::
	switch VAR_RESULT
	case FRONTIER_LVL_50, BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMonsLv50
	case FRONTIER_LVL_OPEN, BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMonsLvOpen

BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMonsLv50::
	msgbox BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsLv50, MSGBOX_DEFAULT
	call BattleFrontier_ShowCaughtBannedSpecies
	msgbox BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsEnd, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_EndCancelChallenge

BattleFrontier_BattlePyramidLobby_EventScript_NotEnoughValidMonsLvOpen::
	msgbox BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsLvOpen, MSGBOX_DEFAULT
	call BattleFrontier_ShowCaughtBannedSpecies
	msgbox BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsEnd, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_EndCancelChallenge

BattleFrontier_BattlePyramidLobby_EventScript_CancelChallengeSaveFailed::
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	goto BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge

BattleFrontier_BattlePyramidLobby_EventScript_LoadPartyAndCancelChallenge::
	special LoadPlayerParty
	callnative UpdateFollowingPokemon
BattleFrontier_BattlePyramidLobby_EventScript_CancelChallenge::
	msgbox BattleFrontier_BattlePyramidLobby_Text_AwaitFutureChallenge, MSGBOX_DEFAULT
BattleFrontier_BattlePyramidLobby_EventScript_EndCancelChallenge::
	release
	end

BattleFrontier_BattlePyramidLobby_EventScript_HintGiver::
	lockall
	applymovement LOCALID_PYRAMID_LOBBY_HINT_GIVER, Common_Movement_FacePlayer
	waitmovement 0
	msgbox BattleFrontier_BattlePyramidLobby_Text_TellYouWhatMisfortunesAwait, MSGBOX_DEFAULT
	call BattleFrontier_BattlePyramidLobby_EventScript_GiveHint
	msgbox BattleFrontier_BattlePyramidLobby_Text_BelieveMyFortunesOrNot, MSGBOX_DEFAULT
	releaseall
	end

BattleFrontier_BattlePyramidLobby_EventScript_GiveHint::
	multichoice 17, 6, MULTI_LEVEL_MODE, FALSE
	switch VAR_RESULT
	case FRONTIER_LVL_50, BattleFrontier_BattlePyramidLobby_EventScript_GiveHintLv50
	case FRONTIER_LVL_OPEN, BattleFrontier_BattlePyramidLobby_EventScript_GiveHintLvOpen
	case FRONTIER_LVL_TENT, BattleFrontier_BattlePyramidLobby_EventScript_NoHint
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_NoHint
	return

@ Shouldnt occur
BattleFrontier_BattlePyramidLobby_EventScript_NoHint::
	return

BattleFrontier_BattlePyramidLobby_EventScript_GiveHintLv50::
	msgbox BattleFrontier_BattlePyramidLobby_Text_Aah, MSGBOX_DEFAULT
	pyramid_get PYRAMID_DATA_WIN_STREAK_ACTIVE_50
	goto_if_ne VAR_RESULT, FALSE, BattleFrontier_BattlePyramidLobby_EventScript_GiveHintGetLv50Streak
	setvar VAR_RESULT, 0
	goto BattleFrontier_BattlePyramidLobby_EventScript_DoHintComment
	return

BattleFrontier_BattlePyramidLobby_EventScript_GiveHintGetLv50Streak::
	pyramid_get PYRAMID_DATA_WIN_STREAK_50
	goto BattleFrontier_BattlePyramidLobby_EventScript_DoHintComment
	return

BattleFrontier_BattlePyramidLobby_EventScript_GiveHintLvOpen::
	msgbox BattleFrontier_BattlePyramidLobby_Text_Aah, MSGBOX_DEFAULT
	pyramid_get PYRAMID_DATA_WIN_STREAK_ACTIVE_OPEN
	goto_if_ne VAR_RESULT, FALSE, BattleFrontier_BattlePyramidLobby_EventScript_GiveHintGetLvOpenStreak
	setvar VAR_RESULT, 0
	goto BattleFrontier_BattlePyramidLobby_EventScript_DoHintComment
	return

BattleFrontier_BattlePyramidLobby_EventScript_GiveHintGetLvOpenStreak::
	pyramid_get PYRAMID_DATA_WIN_STREAK_OPEN
	goto BattleFrontier_BattlePyramidLobby_EventScript_DoHintComment
	return

BattleFrontier_BattlePyramidLobby_EventScript_DoHintComment::
	copyvar VAR_0x8004, VAR_RESULT
	special GetBattlePyramidHint
	switch VAR_RESULT
	case 0, BattleFrontier_BattlePyramidLobby_EventScript_HintParalysis
	case 1, BattleFrontier_BattlePyramidLobby_EventScript_HintPoison
	case 2, BattleFrontier_BattlePyramidLobby_EventScript_HintBurn
	case 3, BattleFrontier_BattlePyramidLobby_EventScript_HintPPWaste
	case 4, BattleFrontier_BattlePyramidLobby_EventScript_HintLevitate
	case 5, BattleFrontier_BattlePyramidLobby_EventScript_HintTrapAbility
	case 6, BattleFrontier_BattlePyramidLobby_EventScript_HintIce
	case 7, BattleFrontier_BattlePyramidLobby_EventScript_HintExplosion
	case 8, BattleFrontier_BattlePyramidLobby_EventScript_HintPsychic
	case 9, BattleFrontier_BattlePyramidLobby_EventScript_HintRock
	case 10, BattleFrontier_BattlePyramidLobby_EventScript_HintFighting
	case 11, BattleFrontier_BattlePyramidLobby_EventScript_HintWeather
	case 12, BattleFrontier_BattlePyramidLobby_EventScript_HintBug
	case 13, BattleFrontier_BattlePyramidLobby_EventScript_HintDark
	case 14, BattleFrontier_BattlePyramidLobby_EventScript_HintWater
	case 15, BattleFrontier_BattlePyramidLobby_EventScript_HintGhost
	case 16, BattleFrontier_BattlePyramidLobby_EventScript_HintSteel
	case 17, BattleFrontier_BattlePyramidLobby_EventScript_HintFlyingDragon
	case 18, BattleFrontier_BattlePyramidLobby_EventScript_HintStoneEvolve
	case 19, BattleFrontier_BattlePyramidLobby_EventScript_HintNormal
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintParalysis::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintParalysis, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintPoison::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintPoison, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintBurn::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintBurn, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintPPWaste::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintPPWaste, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintLevitate::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintLevitate, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintTrapAbility::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintTrapAbility, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintIce::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintIce, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintExplosion::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintExplosion, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintPsychic::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintPsychic, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintRock::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintRock, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintFighting::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintFighting, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintWeather::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintWeather, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintBug::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintBug, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintDark::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintDark, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintWater::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintWater, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintGhost::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintGhost, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintSteel::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintSteel, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintFlyingDragon::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintFlyingDragon, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintStoneEvolve::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintStoneEvolve, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_HintNormal::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HintNormal, MSGBOX_DEFAULT
	return

BattleFrontier_BattlePyramidLobby_EventScript_ShowResults::
	lockall
	frontier_results FRONTIER_FACILITY_PYRAMID
	waitbuttonpress
	special RemoveRecordsWindow
	releaseall
	end

BattleFrontier_BattlePyramidLobby_EventScript_WalkToPanelAndReceiveBag::
	applymovement LOCALID_PYRAMID_LOBBY_ATTENDANT, BattleFrontier_BattlePyramidLobby_Movement_AttendantWalkToPanel
	applymovement LOCALID_PLAYER, BattleFrontier_BattlePyramidLobby_Movement_PlayerWalkToPanel
	waitmovement 0
	msgbox BattleFrontier_BattlePyramidLobby_Text_WeWillHoldBagForSafekeeping, MSGBOX_DEFAULT
	pyramid_get PYRAMID_DATA_WIN_STREAK
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidLobby_EventScript_ReceiveNewBattleBag
	msgbox BattleFrontier_BattlePyramidLobby_Text_PleaseTakePreviousBattleBag, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReceiveBattleBag

BattleFrontier_BattlePyramidLobby_EventScript_ReceiveNewBattleBag::
	msgbox BattleFrontier_BattlePyramidLobby_Text_PleaseTakeThisBattleBag, MSGBOX_DEFAULT
BattleFrontier_BattlePyramidLobby_EventScript_ReceiveBattleBag::
	message BattleFrontier_BattlePyramidLobby_Text_ExchangedBagForBattleBag
	waitmessage
	playse SE_EXP_MAX
	waitse
	msgbox BattleFrontier_BattlePyramidLobby_Text_StepOnFloorPanel, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PYRAMID_LOBBY_ATTENDANT, BattleFrontier_BattlePyramidLobby_Movement_AttendantMoveAside
	waitmovement 0
	applymovement LOCALID_PLAYER, BattleFrontier_BattlePyramidLobby_Movement_PlayerStepOnPanel
	waitmovement 0
	return

BattleFrontier_BattlePyramidLobby_Movement_AttendantWalkToPanel:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	face_down
	step_end

BattleFrontier_BattlePyramidLobby_Movement_PlayerWalkToPanel:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

BattleFrontier_BattlePyramidLobby_Movement_AttendantMoveAside:
	walk_right
	face_left
	step_end

BattleFrontier_BattlePyramidLobby_Movement_PlayerStepOnPanel:
	walk_up
	walk_up
	step_end

BattleFrontier_BattlePyramidLobby_EventScript_StoreHeldItemsInPyramidBag::
	msgbox BattleFrontier_BattlePyramidLobby_Text_MonHoldingItemCannotTake, MSGBOX_DEFAULT
	setflag FLAG_STORING_ITEMS_IN_PYRAMID_BAG
	special TryStoreHeldItemsInPyramidBag
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattlePyramidLobby_EventScript_HeldItemsStoredInPyramidBag
	message BattleFrontier_BattlePyramidLobby_Text_BagCannotHoldPickItemsToKeep
	waitmessage
	goto BattleFrontier_BattlePyramidLobby_EventScript_PickItemsToKeep
	end

BattleFrontier_BattlePyramidLobby_EventScript_HeldItemsStoredInPyramidBag::
	msgbox BattleFrontier_BattlePyramidLobby_Text_HeldItemsMovedToBag, MSGBOX_DEFAULT
	return

@ When exiting Battle Pyramid with a full pyramid bag and held items the player must select to keep/toss party held items and make room for any kept items by tossing from the pyramid bag
BattleFrontier_BattlePyramidLobby_EventScript_PickItemsToKeep::
	multichoice 17, 6, MULTI_FRONTIER_ITEM_CHOOSE, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_BattlePyramidLobby_EventScript_PickItemsFromBag
	case 1, BattleFrontier_BattlePyramidLobby_EventScript_PickItemsFromParty
	case 2, BattleFrontier_BattlePyramidLobby_EventScript_ExitPickItems
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_ExitPickItems
	end

BattleFrontier_BattlePyramidLobby_EventScript_PickItemsFromBag::
	special ChooseItemsToTossFromPyramidBag
	message BattleFrontier_BattlePyramidLobby_Text_PickItemsToKeep
	waitmessage
	goto BattleFrontier_BattlePyramidLobby_EventScript_PickItemsToKeep
	end

BattleFrontier_BattlePyramidLobby_EventScript_PickItemsFromParty::
	special BattlePyramidChooseMonHeldItems
	message BattleFrontier_BattlePyramidLobby_Text_PickItemsToKeep
	waitmessage
	goto BattleFrontier_BattlePyramidLobby_EventScript_PickItemsToKeep
	end

BattleFrontier_BattlePyramidLobby_EventScript_ExitPickItems::
	special DoBattlePyramidMonsHaveHeldItem
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_BattlePyramidLobby_EventScript_PartyStillHasHeldItems
	return

BattleFrontier_BattlePyramidLobby_EventScript_PartyStillHasHeldItems::
	msgbox BattleFrontier_BattlePyramidLobby_Text_LeastOneMonHoldingItem, MSGBOX_DEFAULT
	message BattleFrontier_BattlePyramidLobby_Text_PickItemsToKeep
	waitmessage
	goto BattleFrontier_BattlePyramidLobby_EventScript_PickItemsToKeep
	end

BattleFrontier_BattlePyramidLobby_EventScript_Woman::
	msgbox BattleFrontier_BattlePyramidLobby_Text_TrainersNoticeRunning, MSGBOX_NPC
	end

BattleFrontier_BattlePyramidLobby_EventScript_FatMan::
	msgbox BattleFrontier_BattlePyramidLobby_Text_LostLotOfItems, MSGBOX_NPC
	end

BattleFrontier_BattlePyramidLobby_EventScript_RulesBoard::
	lockall
	msgbox BattleFrontier_BattlePyramidLobby_Text_RulesAreListed, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard::
	message BattleFrontier_BattlePyramidLobby_Text_ReadWhichHeading
	waitmessage
	multichoice 15, 2, MULTI_BATTLE_PYRAMID_RULES, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_BattlePyramidLobby_EventScript_RulesPokemon
	case 1, BattleFrontier_BattlePyramidLobby_EventScript_RulesTrainers
	case 2, BattleFrontier_BattlePyramidLobby_EventScript_RulesMaze
	case 3, BattleFrontier_BattlePyramidLobby_EventScript_RulesBag
	case 4, BattleFrontier_BattlePyramidLobby_EventScript_ExitRules
	case MULTI_B_PRESSED, BattleFrontier_BattlePyramidLobby_EventScript_ExitRules
	end

BattleFrontier_BattlePyramidLobby_EventScript_RulesPokemon::
	msgbox BattleFrontier_BattlePyramidLobby_Text_ExplainMonRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattlePyramidLobby_EventScript_RulesTrainers::
	msgbox BattleFrontier_BattlePyramidLobby_Text_ExplainTrainerRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattlePyramidLobby_EventScript_RulesMaze::
	msgbox BattleFrontier_BattlePyramidLobby_Text_ExplainMazeRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattlePyramidLobby_EventScript_RulesBag::
	msgbox BattleFrontier_BattlePyramidLobby_Text_ExplainBagRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattlePyramidLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattlePyramidLobby_EventScript_ExitRules::
	releaseall
	end

BattleFrontier_BattlePyramidLobby_Text_WelcomeToBattlePyramid:
	.string "Dove il coraggio degli Allenatori\n"
	.string "viene messo alla prova!\p"
	.string "Benvenuto alla Piramide Lotta!\p"
	.string "Sono la guida della\n"
	.string "Spedizione Lotta.$"

BattleFrontier_BattlePyramidLobby_Text_EmbarkOnChallenge:
	.string "Hai il coraggio di lanciarti nella\n"
	.string "sfida della Spedizione Lotta?$"

BattleFrontier_BattlePyramidLobby_Text_AwaitFutureChallenge:
	.string "Ti aspettiamo per la prossima sfida!$"

BattleFrontier_BattlePyramidLobby_Text_ExplainBattlePyramid:
	.string "La Spedizione Lotta è una prova in\n"
	.string "cui devi esplorare la Piramide e\l"
	.string "cercare di raggiungerne la cima.\p"
	.string "La Piramide ha sette piani di labirinti,\n"
	.string "pieni non solo di Allenatori esperti,\l"
	.string "ma anche di Pokémon selvatici.\p"
	.string "Devi entrare con tre Pokémon,\n"
	.string "nessuno dei quali con uno strumento.\p"
	.string "Durante la salita riceverai una\n"
	.string "Borsa Lotta per gli strumenti.\p"
	.string "Se fallisci la sfida, gli strumenti\n"
	.string "nella Borsa Lotta andranno perduti.\p"
	.string "Se vuoi interrompere la sfida,\n"
	.string "devi scegliere “Riposa” e salvare\l"
	.string "la partita.\p"
	.string "Se interrompi senza salvare,\n"
	.string "verrai squalificato.$"

BattleFrontier_BattlePyramidLobby_Text_WhichLevelMode:
	.string "La Piramide offre due percorsi:\n"
	.string "Livello 50 e Livello Libero.\l"
	.string "Quale scegli?$"

BattleFrontier_BattlePyramidLobby_Text_SelectThreeMons:
	.string "Benissimo. Ora scegli i tre Pokémon\n"
	.string "che ti accompagneranno.$"

BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsLvOpen:
	.string "C'è un piccolo problema, avventuriero!\p"
	.string "Pare che tu non abbia tre Pokémon\n"
	.string "idonei per la sfida.\p"
	.string "Ricorda anche di togliere tutti gli\n"
	.string "strumenti ai tuoi Pokémon.\p"
	.string "Le Uova{STR_VAR_1}.$"

BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsLv50:
	.string "C'è un piccolo problema, avventuriero!\p"
	.string "Pare che tu non abbia tre Pokémon\n"
	.string "idonei per la sfida.\p"
	.string "I tre Pokémon devono essere diversi\n"
	.string "e non superare il livello 50.\p"
	.string "Ricorda anche di togliere tutti gli\n"
	.string "strumenti ai tuoi Pokémon.\p"
	.string "Le Uova{STR_VAR_1}.$"

BattleFrontier_BattlePyramidLobby_Text_NotEnoughValidMonsEnd:
	.string "Quando sarai pronto, vieni pure\n"
	.string "a parlarmi.$"

BattleFrontier_BattlePyramidLobby_Text_OkayToSaveBeforeChallenge:
	.string "Prima di entrare nella Piramide Lotta\n"
	.string "bisogna salvare la partita. Va bene?$"

BattleFrontier_BattlePyramidLobby_Text_ShowYouIntoPyramid:
	.string "Benissimo. Ora ti accompagno\n"
	.string "nella Piramide Lotta.$"

BattleFrontier_BattlePyramidLobby_Text_WeWillHoldBagForSafekeeping:
	.string "Custodiremo noi la tua Borsa,\n"
	.string "{PLAYER}, mentre sei in esplorazione.$"

BattleFrontier_BattlePyramidLobby_Text_PleaseTakePreviousBattleBag:
	.string "In cambio, prendi questa Borsa Lotta:\n"
	.string "è quella che hai usato l'ultima volta.$"

BattleFrontier_BattlePyramidLobby_Text_PleaseTakeThisBattleBag:
	.string "In cambio, prendi questa\n"
	.string "Borsa Lotta.$"

BattleFrontier_BattlePyramidLobby_Text_ExchangedBagForBattleBag:
	.string "{PLAYER} ha scambiato la Borsa con\n"
	.string "la Borsa Lotta.$"

BattleFrontier_BattlePyramidLobby_Text_StepOnFloorPanel:
	.string "Salendo su questa piattaforma verrai\n"
	.string "trasportato a un piano superiore\l"
	.string "della Piramide.\p"
	.string "Spero, per il tuo bene, che la\n"
	.string "spedizione fili liscia!$"

BattleFrontier_BattlePyramidLobby_Text_DidntSaveBeforeQuittingTakeBag:
	.string "C'è un grosso problema, esploratore!\p"
	.string "L'ultima volta hai interrotto la sfida\n"
	.string "senza salvare.\p"
	.string "Di conseguenza, mi spiace dirlo,\n"
	.string "la tua sfida è stata annullata.\p"
	.string "Ecco la Borsa che custodivamo\n"
	.string "per te.\p"
	.string "{PLAYER} ha riavuto la Borsa.$"

BattleFrontier_BattlePyramidLobby_Text_YouveConqueredPyramid:
	.string "Che piacere rivederti!\p"
	.string "Hai conquistato la Piramide!\n"
	.string "Magnifico!$"

BattleFrontier_BattlePyramidLobby_Text_MonHoldingItemCannotTake:
	.string "Ah, c'è un piccolo problema.\p"
	.string "Almeno un Pokémon tiene\n"
	.string "uno strumento.\p"
	.string "Mi spiace, ma gli strumenti ottenuti\n"
	.string "nella Piramide devono restare qui.$"

BattleFrontier_BattlePyramidLobby_Text_HeldItemsMovedToBag:
	.string "{PLAYER}, tutti gli strumenti dei tuoi\n"
	.string "Pokémon passeranno nella Borsa Lotta.$"

BattleFrontier_BattlePyramidLobby_Text_BagCannotHoldPickItemsToKeep:
	.string "Mi spiace, ma la Borsa Lotta non può\n"
	.string "contenere tutti i tuoi strumenti.\p"
	.string "Scegli gli strumenti da tenere nella\n"
	.string "Borsa Lotta e quelli da dare ai Pokémon.$"

BattleFrontier_BattlePyramidLobby_Text_LeastOneMonHoldingItem:
	.string "Almeno un Pokémon tiene ancora\n"
	.string "uno strumento.$"

BattleFrontier_BattlePyramidLobby_Text_PickItemsToKeep:
	.string "Scegli gli strumenti da tenere nella\n"
	.string "Borsa Lotta e quelli da dare ai Pokémon.$"

@ Unused
BattleFrontier_BattlePyramidLobby_Text_ReturnedEverythingMonsHeld:
	.string "{PLAYER} ha restituito tutto ciò\n"
	.string "che tenevano i Pokémon.$"

BattleFrontier_BattlePyramidLobby_Text_UsedBattleBagWillBeKept:
	.string "La Borsa Lotta che hai usato resterà\n"
	.string "qui, pronta per la prossima sfida.\p"
	.string "{PLAYER} ha consegnato la Borsa Lotta\n"
	.string "e ha riavuto la Borsa.$"

BattleFrontier_BattlePyramidLobby_Text_RecordResultsWait:
	.string "Devo registrare i tuoi risultati.\n"
	.string "Attendi un momento.$"

@ Unused
BattleFrontier_BattlePyramidLobby_Text_ForConqueringPyramidTakeThis:
	.string "Come ricordo della conquista della\n"
	.string "Piramide Lotta, accetta questo.$"

@ Unused
BattleFrontier_BattlePyramidLobby_Text_ReceivedPrizeItem:
	.string "{PLAYER} ha ricevuto in premio\n"
	.string "{STR_VAR_1}.$"

@ Unused
BattleFrontier_BattlePyramidLobby_Text_BagIsFull:
	.string "…Ah…\n"
	.string "Pare che la tua Borsa sia piena.\p"
	.string "Torna quando avrai fatto un po'\n"
	.string "d'ordine nella Borsa.$"

BattleFrontier_BattlePyramidLobby_Text_DisappointingHereIsBag:
	.string "Che peccato…\p"
	.string "Ecco la Borsa che custodivamo\n"
	.string "per te.\p"
	.string "{PLAYER} ha riavuto la Borsa.$"

BattleFrontier_BattlePyramidLobby_Text_LookForwardToNextChallenge:
	.string "Attendiamo con ansia la tua\n"
	.string "prossima sfida!$"

@ Unused
BattleFrontier_BattlePyramidLobby_Text_HereIsPrize:
	.string "Aspettavamo con ansia\n"
	.string "il tuo arrivo!\p"
	.string "Ecco il premio per aver conquistato\n"
	.string "la Piramide.$"

BattleFrontier_BattlePyramidLobby_Text_TellYouWhatMisfortunesAwait:
	.string "Benvenuto…\p"
	.string "Sarò lieta di svelarti quali sventure\n"
	.string "ti attendono nella Piramide…$"

BattleFrontier_BattlePyramidLobby_Text_Aah:
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "… … … … … …\n"
	.string "Aaah!$"

BattleFrontier_BattlePyramidLobby_Text_HintParalysis:
	.string "Vedo una pioggia di scintille…\p"
	.string "…E in mezzo vedo i tuoi Pokémon\n"
	.string "alle prese con la paralisi…$"

BattleFrontier_BattlePyramidLobby_Text_HintPoison:
	.string "Vedo del veleno…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "per gli effetti del veleno…$"

BattleFrontier_BattlePyramidLobby_Text_HintBurn:
	.string "Vedo fiamme di un rosso vivo…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "per le scottature…$"

BattleFrontier_BattlePyramidLobby_Text_HintPPWaste:
	.string "Percepisco l'enorme pressione di\n"
	.string "un rancore mai placato…\p"
	.string "È una maledizione…\p"
	.string "…E vedo i tuoi Pokémon senza più\n"
	.string "Punti Potenza, costretti a ricorrere\l"
	.string "a Scontro…$"

BattleFrontier_BattlePyramidLobby_Text_HintLevitate:
	.string "Vedo Pokémon che fluttuano in alto…\p"
	.string "…E vedo i tuoi Pokémon frustrati\n"
	.string "da mosse di tipo Terra senza effetto…$"

BattleFrontier_BattlePyramidLobby_Text_HintTrapAbility:
	.string "Percepisco un'energia tremenda che\n"
	.string "sale dal terreno…\p"
	.string "…E vedo i tuoi Pokémon incapaci\n"
	.string "di sfuggire alla sua morsa…$"

BattleFrontier_BattlePyramidLobby_Text_HintIce:
	.string "Vedo Pokémon di tipo Ghiaccio…\p"
	.string "…E vedo i tuoi Pokémon lottare\n"
	.string "contro il morso del gelo…$"

BattleFrontier_BattlePyramidLobby_Text_HintExplosion:
	.string "Vedo una raffica di mosse che mettono\n"
	.string "in pericolo chi le usa…\p"
	.string "…E vedo i tuoi Pokémon cadere\n"
	.string "sotto i loro colpi…$"

BattleFrontier_BattlePyramidLobby_Text_HintPsychic:
	.string "Vedo Pokémon di tipo Psico…\p"
	.string "…E vedo i tuoi Pokémon tormentati\n"
	.string "da mosse di tipo Psico…$"

BattleFrontier_BattlePyramidLobby_Text_HintRock:
	.string "Vedo Pokémon di tipo Roccia…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "sotto mosse di tipo Roccia…$"

BattleFrontier_BattlePyramidLobby_Text_HintFighting:
	.string "Vedo Pokémon di tipo Lotta…\p"
	.string "…E vedo i tuoi Pokémon martellati\n"
	.string "da mosse di tipo Lotta…$"

BattleFrontier_BattlePyramidLobby_Text_HintWeather:
	.string "Pioggiadanza… Giornodisole…\n"
	.string "Terrempesta… Grandine…\p"
	.string "Vedo Pokémon che diventano più forti\n"
	.string "grazie al clima…\p"
	.string "…E vedo i tuoi Pokémon confusi\n"
	.string "da mosse di tipi diversi…$"

BattleFrontier_BattlePyramidLobby_Text_HintBug:
	.string "Vedo Pokémon di tipo Coleottero…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "per attacchi di ogni genere…$"

BattleFrontier_BattlePyramidLobby_Text_HintDark:
	.string "Vedo Pokémon di tipo Buio…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "per mosse di tipo Buio…$"

BattleFrontier_BattlePyramidLobby_Text_HintWater:
	.string "Vedo Pokémon di tipo Acqua…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "per mosse di tipo Acqua…$"

BattleFrontier_BattlePyramidLobby_Text_HintGhost:
	.string "Vedo Pokémon di tipo Spettro…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "per mosse di tipo Spettro…$"

BattleFrontier_BattlePyramidLobby_Text_HintSteel:
	.string "Vedo Pokémon di tipo Acciaio…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "per mosse di enorme potenza…$"

BattleFrontier_BattlePyramidLobby_Text_HintFlyingDragon:
	.string "Vedo Pokémon che solcano il cielo…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "per mosse di enorme potenza…$"

BattleFrontier_BattlePyramidLobby_Text_HintStoneEvolve:
	.string "Vedo creature che si sono evolute\n"
	.string "grazie al potere delle pietre…\p"
	.string "Percepisco anche fuoco, acqua\n"
	.string "ed elettricità…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "per questi tre poteri…$"

BattleFrontier_BattlePyramidLobby_Text_HintNormal:
	.string "Vedo Pokémon di tipo Normale…\p"
	.string "…E vedo i tuoi Pokémon soffrire\n"
	.string "per mosse di enorme potenza…$"

BattleFrontier_BattlePyramidLobby_Text_BelieveMyFortunesOrNot:
	.string "Che tu creda o no alle mie profezie,\n"
	.string "la scelta spetta a te…\p"
	.string "Il futuro è come un Varco: può\n"
	.string "cambiare da un istante all'altro…\l"
	.string "Ti auguro un passaggio sicuro…$"

BattleFrontier_BattlePyramidLobby_Text_TrainersNoticeRunning:
	.string "Lo sapevi?\p"
	.string "Se corri, gli Allenatori potrebbero\n"
	.string "accorgersi di te e venire a sfidarti.\p"
	.string "Quindi, se vuoi evitarli, non farti\n"
	.string "notare: passa oltre con cautela\l"
	.string "e in punta di piedi.$"

BattleFrontier_BattlePyramidLobby_Text_LostLotOfItems:
	.string "Uaaaaaaaargh!\p"
	.string "Avevo un sacco di strumenti, ma li ho\n"
	.string "persi tutti quando ho perso!\p"
	.string "Uaaaaaaaargh!$"

BattleFrontier_BattlePyramidLobby_Text_YouveDefeatedPyramidKing:
	.string "Bentornato!\n"
	.string "Hai compiuto l'impensabile!\p"
	.string "Hai sconfitto il Re Piramide e\n"
	.string "conquistato la Piramide Lotta!$"

BattleFrontier_BattlePyramidLobby_Text_GiveYouTheseBattlePoints:
	.string "Giovane esploratore!\n"
	.string "In onore del tuo coraggio,\l"
	.string "ti consegniamo questi Punti Lotta!$"

BattleFrontier_BattlePyramidLobby_Text_RulesAreListed:
	.string "Ecco le regole della Spedizione Lotta.$"

BattleFrontier_BattlePyramidLobby_Text_ReadWhichHeading:
	.string "Quale voce vuoi leggere?$"

BattleFrontier_BattlePyramidLobby_Text_ExplainMonRules:
	.string "Quando la Piramide viene conquistata,\n"
	.string "i Pokémon selvatici al suo interno\l"
	.string "vengono sostituiti da specie diverse.\p"
	.string "Esplora, osserva e scopri quali\n"
	.string "Pokémon selvatici potresti incontrare.$"

BattleFrontier_BattlePyramidLobby_Text_ExplainTrainerRules:
	.string "Dentro la Piramide ci sono Allenatori\n"
	.string "pronti a tenderti un agguato.\p"
	.string "Su ogni piano ci sono fino a\n"
	.string "otto Allenatori.\p"
	.string "Ogni Allenatore sconfitto ti darà\n"
	.string "un consiglio utile per l'avventura.$"

BattleFrontier_BattlePyramidLobby_Text_ExplainMazeRules:
	.string "I labirinti della Piramide cambiano\n"
	.string "forma ogni volta che ci entri.\p"
	.string "Sono bui come il cuore di un Varco:\n"
	.string "avanza con cautela.\p"
	.string "La luce aumenta ogni volta che\n"
	.string "sconfiggi un Pokémon selvatico\l"
	.string "o un Allenatore.$"

BattleFrontier_BattlePyramidLobby_Text_ExplainBagRules:
	.string "La Borsa Lotta sostituisce la tua\n"
	.string "Borsa all'interno della Piramide.\p"
	.string "Esistono due Borse Lotta distinte:\n"
	.string "una per le spedizioni di Livello 50\l"
	.string "e una per quelle di Livello Libero.\p"
	.string "Ognuna può contenere 10 tipi di\n"
	.string "strumenti, fino a 99 pezzi per tipo.\p"
	.string "Il contenuto della Borsa Lotta va\n"
	.string "perduto se la spedizione fallisce.$"

