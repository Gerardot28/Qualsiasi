LilycoveCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LilycoveCity_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, LilycoveCity_OnLoad
	.byte 0

LilycoveCity_OnTransition:
	setflag FLAG_VISITED_LILYCOVE_CITY
	setvar VAR_CONTEST_HALL_STATE, 0
	setflag FLAG_HIDE_LILYCOVE_CONTEST_HALL_REPORTER
	call_if_set FLAG_SYS_WEATHER_CTRL, Common_EventScript_SetAbnormalWeather
	call Common_EventScript_SetupRivalGfxId
	end

LilycoveCity_OnLoad:
	call_if_unset FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, LilycoveCity_EventScript_SetWailmerMetatiles
	end

LilycoveCity_EventScript_SetWailmerMetatiles::
	setmetatile 76, 12, METATILE_Lilycove_Wailmer0, TRUE
	setmetatile 77, 12, METATILE_Lilycove_Wailmer1, TRUE
	setmetatile 76, 13, METATILE_Lilycove_Wailmer2, TRUE
	setmetatile 77, 13, METATILE_Lilycove_Wailmer3, TRUE
	setmetatile 76, 14, METATILE_Lilycove_Wailmer0_Alt, TRUE
	setmetatile 77, 14, METATILE_Lilycove_Wailmer1_Alt, TRUE
	setmetatile 76, 15, METATILE_Lilycove_Wailmer2, TRUE
	setmetatile 77, 15, METATILE_Lilycove_Wailmer3, TRUE
	setmetatile 77, 16, METATILE_Lilycove_Wailmer0_Alt, TRUE
	setmetatile 78, 16, METATILE_Lilycove_Wailmer1_Alt, TRUE
	setmetatile 77, 17, METATILE_Lilycove_Wailmer2, TRUE
	setmetatile 78, 17, METATILE_Lilycove_Wailmer3, TRUE
	return

LilycoveCity_EventScript_BerryGentleman::
	lock
	faceplayer
	dotimebasedevents
	goto_if_set FLAG_DAILY_LILYCOVE_RECEIVED_BERRY, LilycoveCity_EventScript_ReceivedBerry
	msgbox LilycoveCity_Text_BerrySuitsYou, MSGBOX_DEFAULT
	giverandomberry BERRY_ID_CHERI, BERRY_ID_SITRUS
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_DAILY_LILYCOVE_RECEIVED_BERRY
	msgbox LilycoveCity_Text_BecauseYoureTrainer, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_ReceivedBerry::
	msgbox LilycoveCity_Text_PokeblocksSuitPokemon, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Man3::
	msgbox LilycoveCity_Text_ContestHallInTown, MSGBOX_NPC
	end

LilycoveCity_EventScript_Girl::
	lock
	faceplayer
	goto_if_set FLAG_BADGE07_GET, LilycoveCity_EventScript_GirlAquaGone
	msgbox LilycoveCity_Text_StrangeCaveInCove, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_GirlAquaGone::
	msgbox LilycoveCity_Text_GoingToMoveDeleterForHMs, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_RichBoy::
	msgbox LilycoveCity_Text_ImFromKanto, MSGBOX_NPC
	end

LilycoveCity_EventScript_Sailor2::
	lock
	faceplayer
	goto_if_set FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, LilycoveCity_EventScript_Sailor2AquaGone
	msgbox LilycoveCity_Text_TeamAquaBeenTrainingWailmer, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Sailor2AquaGone::
	msgbox LilycoveCity_Text_SomeonePuntedTeamAquaOut, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Woman2::
	lock
	faceplayer
	goto_if_set FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, LilycoveCity_EventScript_Woman2AquaGone
	msgbox LilycoveCity_Text_SomeoneStoleMyPokemon, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Woman2AquaGone::
	msgbox LilycoveCity_Text_MissingPokemonCameBack, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Man2::
	msgbox LilycoveCity_Text_ImArtDealer, MSGBOX_NPC
	end

LilycoveCity_EventScript_ExpertM1::
	msgbox LilycoveCity_Text_SeaRemainsForeverYoung, MSGBOX_SIGN
	end

LilycoveCity_EventScript_ExpertF::
	msgbox LilycoveCity_Text_SixtyYearsAgoHusbandProposed, MSGBOX_SIGN
	end

LilycoveCity_EventScript_ExpertM2::
	lock
	faceplayer
	goto_if_set FLAG_BADGE07_GET, LilycoveCity_EventScript_ExpertM2AquaGone
	msgbox LilycoveCity_Text_TeamAquaRenovatedCavern, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_ExpertM2AquaGone::
	msgbox LilycoveCity_Text_TeamAquaLotGoneForGood, MSGBOX_DEFAULT
	release
	end

LilycoveCity_EventScript_Sailor1::
	msgbox LilycoveCity_Text_HeardTowerCalledSkyPillar, MSGBOX_NPC
	end

LilycoveCity_EventScript_FatMan::
	msgbox LilycoveCity_Text_SawTallTowerOnRoute131, MSGBOX_NPC
	end

LilycoveCity_EventScript_Man1::
	lockall
	msgbox LilycoveCity_Text_JustArrivedAndSawRarePokemon, MSGBOX_NPC
	applymovement LOCALID_LILYCOVE_MAN_1, Common_Movement_FaceOriginalDirection
	end

LilycoveCity_EventScript_Woman1::
	lockall
	msgbox LilycoveCity_Text_HoneymoonVowToSeeRarePokemon, MSGBOX_NPC
	applymovement LOCALID_LILYCOVE_WOMAN_1, Common_Movement_FaceOriginalDirection
	end

LilycoveCity_EventScript_CitySign::
	msgbox LilycoveCity_Text_CitySign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_ContestHallSign::
	msgbox LilycoveCity_Text_ContestHallSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_MotelSign::
	msgbox LilycoveCity_Text_MotelSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_MuseumSign::
	lockall
	specialvar VAR_0x8004, CountPlayerMuseumPaintings
	switch VAR_0x8004
	case 0, LilycoveCity_EventScript_MuseumSignNoPaintings
	msgbox LilycoveCity_Text_MuseumSignPlayersExhibit, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_MuseumSignNoPaintings::
	msgbox LilycoveCity_Text_MuseumSign, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_HarborSign::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_EventScript_HarborSignFerryReady
	msgbox LilycoveCity_Text_HarborSignUnderConstruction, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_HarborSignFerryReady::
	msgbox LilycoveCity_Text_HarborSign, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_TrainerFanClubSign::
	msgbox LilycoveCity_Text_TrainerFanClubSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_DepartmentStoreSign::
	msgbox LilycoveCity_Text_DepartmentStoreSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_MoveDeletersHouseSign::
	msgbox LilycoveCity_Text_MoveDeletersHouseSign, MSGBOX_SIGN
	end

LilycoveCity_EventScript_WailmerTrainerGrunt::
	lockall
	goto_if_set FLAG_MET_WAILMER_TRAINER, LilycoveCity_EventScript_MetWailmerTrainer
	msgbox LilycoveCity_Text_WailmerLeapOutOfWater, MSGBOX_DEFAULT
	applymovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	waitmovement 0
	msgbox LilycoveCity_Text_GetLostMessingUpTraining, MSGBOX_DEFAULT
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag FLAG_MET_WAILMER_TRAINER
	releaseall
	end

LilycoveCity_EventScript_MetWailmerTrainer::
	msgbox LilycoveCity_Text_WailmerLeapOutOfWater, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_EventScript_AquaGrunt1::
	msgbox LilycoveCity_Text_MovedLootIntoHideoutToday, MSGBOX_NPC
	end

LilycoveCity_EventScript_AquaGrunt2::
	msgbox LilycoveCity_Text_ChanceToDoBigThings, MSGBOX_NPC
	end

LilycoveCity_EventScript_AquaGrunt3::
	msgbox LilycoveCity_Text_DontGoNearCaveInCove, MSGBOX_NPC
	end

LilycoveCity_EventScript_AquaGrunt4::
	msgbox LilycoveCity_Text_IfWorldBecomesOurs, MSGBOX_NPC
	end

LilycoveCity_EventScript_Rival::
	lock
	faceplayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LilycoveCity_EventScript_May
	goto_if_eq VAR_RESULT, FEMALE, LilycoveCity_EventScript_Brendan
	end

LilycoveCity_EventScript_May::
	playbgm MUS_ENCOUNTER_MAY, TRUE
	call_if_set FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_MayAskToBattleAgain
	call_if_unset FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_MayAskToBattle
	goto_if_eq VAR_RESULT, NO, LilycoveCity_EventScript_DeclineMayBattle
	msgbox LilycoveCity_Text_MayWontBeBeaten, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, LilycoveCity_EventScript_BattleMayTreecko
	case 1, LilycoveCity_EventScript_BattleMayTorchic
	case 2, LilycoveCity_EventScript_BattleMayMudkip
	end

LilycoveCity_EventScript_MayAskToBattleAgain::
	msgbox LilycoveCity_Text_MayBattleMe, MSGBOX_YESNO
	return

LilycoveCity_EventScript_MayAskToBattle::
	msgbox LilycoveCity_Text_MayShoppingLetsBattle, MSGBOX_YESNO
	return

LilycoveCity_EventScript_DeclineMayBattle::
	setflag FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE
	msgbox LilycoveCity_Text_MayNotRaisingPokemon, MSGBOX_DEFAULT
	savebgm MUS_DUMMY
	fadedefaultbgm
	release
	end

LilycoveCity_EventScript_Brendan::
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	call_if_set FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_BrendanAskToBattleAgain
	call_if_unset FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE, LilycoveCity_EventScript_BrendanAskToBattle
	goto_if_eq VAR_RESULT, NO, LilycoveCity_EventScript_DeclineBrendanBattle
	msgbox LilycoveCity_Text_BrendanWontBeBeaten, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, LilycoveCity_EventScript_BattleBrendanTreecko
	case 1, LilycoveCity_EventScript_BattleBrendanTorchic
	case 2, LilycoveCity_EventScript_BattleBrendanMudkip
	end

LilycoveCity_EventScript_BrendanAskToBattleAgain::
	msgbox LilycoveCity_Text_BrendanBattleMe, MSGBOX_YESNO
	return

LilycoveCity_EventScript_BrendanAskToBattle::
	msgbox LilycoveCity_Text_BrendanShoppingLetsBattle, MSGBOX_YESNO
	return

LilycoveCity_EventScript_DeclineBrendanBattle::
	setflag FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE
	msgbox LilycoveCity_Text_BrendanNoConfidence, MSGBOX_DEFAULT
	savebgm MUS_DUMMY
	fadedefaultbgm
	release
	end

LilycoveCity_EventScript_BattleMayTreecko::
	trainerbattle_no_intro TRAINER_MAY_LILYCOVE_TREECKO, LilycoveCity_Text_MayDefeat
	goto LilycoveCity_EventScript_DefeatedMay
	end

LilycoveCity_EventScript_BattleMayTorchic::
	trainerbattle_no_intro TRAINER_MAY_LILYCOVE_TORCHIC, LilycoveCity_Text_MayDefeat
	goto LilycoveCity_EventScript_DefeatedMay
	end

LilycoveCity_EventScript_BattleMayMudkip::
	trainerbattle_no_intro TRAINER_MAY_LILYCOVE_MUDKIP, LilycoveCity_Text_MayDefeat
	goto LilycoveCity_EventScript_DefeatedMay
	end

LilycoveCity_EventScript_BattleBrendanTreecko::
	trainerbattle_no_intro TRAINER_BRENDAN_LILYCOVE_TREECKO, LilycoveCity_Text_BrendanDefeat
	goto LilycoveCity_EventScript_DefeatedBrendan
	end

LilycoveCity_EventScript_BattleBrendanTorchic::
	trainerbattle_no_intro TRAINER_BRENDAN_LILYCOVE_TORCHIC, LilycoveCity_Text_BrendanDefeat
	goto LilycoveCity_EventScript_DefeatedBrendan
	end

LilycoveCity_EventScript_BattleBrendanMudkip::
	trainerbattle_no_intro TRAINER_BRENDAN_LILYCOVE_MUDKIP, LilycoveCity_Text_BrendanDefeat
	goto LilycoveCity_EventScript_DefeatedBrendan
	end

LilycoveCity_EventScript_DefeatedMay::
	msgbox LilycoveCity_Text_MayGoingBackToLittleroot, MSGBOX_DEFAULT
	setvar VAR_RESULT, FALSE
	call_if_set FLAG_BADGE06_GET, LilycoveCity_EventScript_CheckFinalBadge
	call_if_eq VAR_RESULT, FALSE, LilycoveCity_EventScript_MayCollectBadges
	call_if_eq VAR_RESULT, TRUE, LilycoveCity_EventScript_MayPokemonLeague
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_RIVAL_BEDROOM
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_2F_PICHU_DOLL
	goto LilycoveCity_EventScript_RivalFlyAway
	end

LilycoveCity_EventScript_CheckFinalBadge::
	goto_if_set FLAG_BADGE08_GET, LilycoveCity_EventScript_HasFinalBadge
	return

LilycoveCity_EventScript_HasFinalBadge::
	setvar VAR_RESULT, TRUE
	return

LilycoveCity_EventScript_MayCollectBadges::
	msgbox LilycoveCity_Text_MayYouGoingToCollectBadges, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_MayPokemonLeague::
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_EventScript_MayBattleFrontier
	msgbox LilycoveCity_Text_MayYouGoingToPokemonLeague, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_MayBattleFrontier::
	msgbox LilycoveCity_Text_MayYouGoingToBattleFrontier, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_DefeatedBrendan::
	msgbox LilycoveCity_Text_BrendanGoingBackToLittleroot, MSGBOX_DEFAULT
	setvar VAR_RESULT, FALSE
	call_if_set FLAG_BADGE06_GET, LilycoveCity_EventScript_CheckFinalBadge
	call_if_eq VAR_RESULT, FALSE, LilycoveCity_EventScript_BrendanCollectBadges
	call_if_eq VAR_RESULT, TRUE, LilycoveCity_EventScript_BrendanPokemonLeague
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_RIVAL_BEDROOM
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_2F_SWABLU_DOLL
	goto LilycoveCity_EventScript_RivalFlyAway
	end

LilycoveCity_EventScript_BrendanCollectBadges::
	msgbox LilycoveCity_Text_BrendanYouGoingToCollectBadges, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_BrendanPokemonLeague::
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_EventScript_BrendanBattleFrontier
	msgbox LilycoveCity_Text_BrendanYouGoingToPokemonLeague, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_BrendanBattleFrontier::
	msgbox LilycoveCity_Text_BrendanYouGoingToBattleFrontier, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_RivalFlyAway::
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 50
	setfieldeffectargument 0, 1
	dofieldeffect FLDEFF_NPCFLY_OUT
	delay 15
	removeobject VAR_LAST_TALKED
	waitfieldeffect FLDEFF_NPCFLY_OUT
	savebgm MUS_DUMMY
	fadedefaultbgm
	setflag FLAG_MET_RIVAL_LILYCOVE
	release
	end

LilycoveCity_EventScript_SchoolKidM::
	lock
	faceplayer
	msgbox LilycoveCity_Text_DoYouKnowAboutBerryBlender, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, LilycoveCity_EventScript_KnowAboutBerryBlender
	call_if_eq VAR_RESULT, NO, LilycoveCity_EventScript_DontKnowAboutBerryBlender
	release
	end

LilycoveCity_EventScript_KnowAboutBerryBlender::
	msgbox LilycoveCity_Text_FasterSpinBetterPokeblocks, MSGBOX_DEFAULT
	return

LilycoveCity_EventScript_DontKnowAboutBerryBlender::
	msgbox LilycoveCity_Text_ExplainBerryBlender, MSGBOX_DEFAULT
	return

LilycoveCity_Text_MayShoppingLetsBattle:
	.string "Arianna: Ehi, ma è {PLAYER}{KUN}!\n"
	.string "Anche tu qui a Porto Alghepoli?\p"
	.string "Ah, già… questa è la tua città!\n"
	.string "Un giorno mi farai da guida, eh?\p"
	.string "Ho girato il porto tutta la mattina.\n"
	.string "I Wailmer del Team Omega bloccano\l"
	.string "le barche, e la gente è furiosa.\p"
	.string "Ho pensato tanto a quella del\n"
	.string "Team Omega, sul Percorso 119…\l"
	.string "E credo di aver deciso una cosa.\p"
	.string "Ma prima voglio vedere come stanno\n"
	.string "i tuoi Pokémon, {PLAYER}{KUN}.\p"
	.string "Lottiamo? Dai, dimmi di sì!$"

LilycoveCity_Text_MayNotRaisingPokemon:
	.string "Arianna: Eh? Non te la senti,\n"
	.string "{PLAYER}{KUN}?\p"
	.string "Va bene, ti aspetto qui.\n"
	.string "Tanto da questa porta non passi!$"

LilycoveCity_Text_MayBattleMe:
	.string "Arianna: Allora? Lottiamo, così\n"
	.string "vedo come stanno i tuoi Pokémon!$"

LilycoveCity_Text_MayWontBeBeaten:
	.string "Arianna: Primo: non mi trattengo.\n"
	.string "Secondo: non perdo, {PLAYER}{KUN}!$"

LilycoveCity_Text_MayDefeat:
	.string "… … … … … … … …\p"
	.string "Ti ricordi la nostra prima lotta\n"
	.string "sul Percorso 103, {PLAYER}{KUN}?\p"
	.string "Da allora sei cresciuto tanto.\n"
	.string "Lo segno… anche questo.$"

LilycoveCity_Text_MayGoingBackToLittleroot:
	.string "Arianna: Ho deciso. Non andrò\n"
	.string "alla Lega Pokémon.\p"
	.string "Il Team Omega apre porte senza\n"
	.string "chiedersi cosa c'è dall'altra parte.\p"
	.string "Io voglio essere quella che lo\n"
	.string "scopre davvero. Con calma.\p"
	.string "Il mio filo porta altrove. Torno ad\n"
	.string "Albanova, a studiare i Varchi\l"
	.string "insieme a papà.$"

LilycoveCity_Text_MayYouGoingToCollectBadges:
	.string "Ma tu, {PLAYER}{KUN}… tu devi\n"
	.string "diventare Campione!\p"
	.string "Raccogli tutte le Medaglie e sfida\n"
	.string "la Lega Pokémon, promesso?\p"
	.string "Io intanto riempio il Pokédex e il\n"
	.string "mio Taccuino dei Varchi.\l"
	.string "Lì, almeno, ti batto io!\p"
	.string "Ci vediamo!$"

LilycoveCity_Text_MayYouGoingToPokemonLeague:
	.string "Ma tu, {PLAYER}{KUN}… tu devi\n"
	.string "diventare Campione!\p"
	.string "Hai già tutte le Medaglie, no?\n"
	.string "La Lega Pokémon ti aspetta, a est!\p"
	.string "Io intanto riempio il Pokédex e il\n"
	.string "mio Taccuino dei Varchi.\l"
	.string "Lì, almeno, ti batto io!\p"
	.string "Ci vediamo!$"

LilycoveCity_Text_MayYouGoingToBattleFrontier:
	.string "E tu, {PLAYER}{KUN}, che farai?\p"
	.string "Proverai la sfida del Parco Lotta?\p"
	.string "O preferisci completare il tuo\n"
	.string "Pokédex?\p"
	.string "In lotta mi hai battuta, ma con\n"
	.string "il Pokédex arrivo prima io!\p"
	.string "Ci vediamo!$"

LilycoveCity_Text_BrendanShoppingLetsBattle:
	.string "Arianna: Ehi, ma è {PLAYER}!\n"
	.string "Anche tu qui a Porto Alghepoli?\p"
	.string "Ah, già… questa è la tua città!\n"
	.string "Un giorno mi farai da guida, eh?\p"
	.string "Ho girato il porto tutta la mattina.\n"
	.string "I Wailmer del Team Omega bloccano\l"
	.string "le barche, e la gente è furiosa.\p"
	.string "Ho pensato tanto a quella del\n"
	.string "Team Omega, sul Percorso 119…\l"
	.string "E credo di aver deciso una cosa.\p"
	.string "Ma prima voglio vedere come stanno\n"
	.string "i tuoi Pokémon, {PLAYER}.\p"
	.string "Lottiamo? Dai, dimmi di sì!$"

LilycoveCity_Text_BrendanNoConfidence:
	.string "Arianna: Eh? Non te la senti?\n"
	.string "Va bene, ti aspetto qui!$"

LilycoveCity_Text_BrendanBattleMe:
	.string "Arianna: Allora? Lottiamo, così\n"
	.string "vedo come stanno i tuoi Pokémon!$"

LilycoveCity_Text_BrendanWontBeBeaten:
	.string "Arianna: Primo: non mi trattengo.\n"
	.string "Secondo: non perdo, {PLAYER}!$"

LilycoveCity_Text_BrendanDefeat:
	.string "Uff…\n"
	.string "Sei cresciuto tanto, sai?\p"
	.string "E pensare che ero partita\n"
	.string "prima di te…$"

LilycoveCity_Text_BrendanGoingBackToLittleroot:
	.string "Arianna: Ho deciso. Non andrò\n"
	.string "alla Lega Pokémon.\p"
	.string "Il Team Omega apre porte senza\n"
	.string "chiedersi cosa c'è dall'altra parte.\p"
	.string "Io voglio essere quella che lo\n"
	.string "scopre davvero. Con calma.\p"
	.string "Il mio filo porta altrove. Torno ad\n"
	.string "Albanova, a studiare i Varchi\l"
	.string "insieme a papà.$"

LilycoveCity_Text_BrendanYouGoingToCollectBadges:
	.string "Ma tu, {PLAYER}… tu devi\n"
	.string "diventare Campione!\p"
	.string "Raccogli tutte le Medaglie e sfida\n"
	.string "la Lega Pokémon, promesso?\p"
	.string "Io intanto segno tutto.\n"
	.string "Ci vediamo!$"

LilycoveCity_Text_BrendanYouGoingToPokemonLeague:
	.string "Ma tu, {PLAYER}… tu devi\n"
	.string "diventare Campione!\p"
	.string "Hai già tutte le Medaglie, no?\n"
	.string "La Lega Pokémon ti aspetta, a est!\p"
	.string "Io intanto segno tutto.\n"
	.string "Ci vediamo!$"

LilycoveCity_Text_BrendanYouGoingToBattleFrontier:
	.string "E tu, {PLAYER}, che farai?\p"
	.string "Proverai la sfida del Parco Lotta?\p"
	.string "O preferisci completare il tuo\n"
	.string "Pokédex?\p"
	.string "In lotta mi hai battuta, ma con\n"
	.string "il Pokédex arrivo prima io!\p"
	.string "Ci vediamo!$"

LilycoveCity_Text_MovedLootIntoHideoutToday:
	.string "Eh-eh-eh… arrr…\p"
	.string "Oggi abbiamo stivato altro bottino\n"
	.string "nel nostro Covo segreto…\p"
	.string "Ch-chi sei, tu?!\n"
	.string "Stavo parlando da solo, eh!$"

LilycoveCity_Text_ChanceToDoBigThings:
	.string "Sono solo una Recluta, non so cosa\n"
	.string "abbia in mente il Capo…\p"
	.string "Ma col Team Omega, prima o poi,\n"
	.string "salperò per grandi imprese! Arrr!\p"
	.string "Ch-chi sei, tu?!\n"
	.string "Stavo parlando da solo, eh!$"

LilycoveCity_Text_DontGoNearCaveInCove:
	.string "Ehi, tu!\p"
	.string "Stai alla larga dalla grotta\n"
	.string "nella baia!\p"
	.string "Perché? Niente perché, mozzo!\n"
	.string "Sono un adulto, quindi obbedisci!$"

LilycoveCity_Text_IfWorldBecomesOurs:
	.string "Quando la Soglia sarà spalancata\n"
	.string "e tutti i mondi saranno un solo mare,\l"
	.string "anche i Pokémon saranno più felici.\p"
	.string "Niente più confini!\n"
	.string "Oltre ogni mare!$"

LilycoveCity_Text_WailmerLeapOutOfWater:
	.string "Su, Wailmer!\n"
	.string "Salta fuori dall'acqua, forza!$"

LilycoveCity_Text_GetLostMessingUpTraining:
	.string "Eh? Che vuoi?\p"
	.string "Ci stai rovinando l'allenamento,\n"
	.string "quindi… leva l'ancora, ok?$"

LilycoveCity_Text_ContestHallInTown:
	.string "Qui in città c'è l'Arena delle Virtù,\n"
	.string "dove si tengono le Gare Pokémon!\p"
	.string "Arrivano Pokémon splendidi da tutta\n"
	.string "Hoenn… e, coi Varchi, anche da\l"
	.string "molto più lontano!\p"
	.string "Mi batte forte il cuore!$"

LilycoveCity_Text_StrangeCaveInCove:
	.string "Ehi, quei capelli rossi li conosco!\n"
	.string "Tu abitavi qui vicino al porto!\p"
	.string "Hai visto quella strana grotta nella\n"
	.string "baia, ai margini della città?$"

LilycoveCity_Text_GoingToMoveDeleterForHMs:
	.string "Voglio insegnare mosse nuove ai miei\n"
	.string "Pokémon per partecipare alle Gare.\p"
	.string "Quindi vado dall'Eliminamosse a far\n"
	.string "dimenticare loro le mosse MN.$"

LilycoveCity_Text_ImFromKanto:
	.string "Il mio Pokémon è uscito da un Varco.\n"
	.string "Dicono che venga da un mondo\l"
	.string "chiamato Kanto… credo.\p"
	.string "Ma Hoenn è bellissima, con tutta\n"
	.string "quest'acqua e tutto questo verde.\p"
	.string "Eh? Aspetta un attimo…\p"
	.string "Ci saranno Pokémon rari che vivono\n"
	.string "solo da queste parti?$"

LilycoveCity_Text_TeamAquaBeenTrainingWailmer:
	.string "Il Team Omega allena i suoi Wailmer\n"
	.string "proprio qui, nella baia.\p"
	.string "Con quei bestioni in mezzo, noi\n"
	.string "marinai non possiamo prendere il mare!$"

LilycoveCity_Text_SomeonePuntedTeamAquaOut:
	.string "Ahoy! Lo sai?\p"
	.string "Qualcuno ci ha tolto dai piedi\n"
	.string "il Team Omega!\p"
	.string "I Wailmer se ne sono andati dalla\n"
	.string "baia, e le barche tornano in mare!$"

LilycoveCity_Text_SomeoneStoleMyPokemon:
	.string "Stavo facendo un pisolino in un\n"
	.string "alberghetto sul mare, cullata\l"
	.string "dalle onde…\p"
	.string "quando qualcuno mi ha rubato\n"
	.string "il Pokémon!\p"
	.string "Chi è stato? I pirati blu?\n"
	.string "È stato il Team Omega?!$"

LilycoveCity_Text_MissingPokemonCameBack:
	.string "Il mio Pokémon scomparso…\n"
	.string "È tornato senza che me ne accorgessi!$"

LilycoveCity_Text_ImArtDealer:
	.string "Sono un mercante d'arte.\p"
	.string "Compro e vendo opere di pregio,\n"
	.string "soprattutto dipinti.\p"
	.string "Ho sentito che il Museo di questa\n"
	.string "città espone capolavori magnifici…$"

LilycoveCity_Text_SeaRemainsForeverYoung:
	.string "Noi invecchiamo e ci riempiamo di\n"
	.string "rughe, ma il mare resta giovane,\l"
	.string "splendente e pieno di vita…\p"
	.string "Fwohoho fwohohohoho…$"

LilycoveCity_Text_SixtyYearsAgoHusbandProposed:
	.string "Sessant'anni fa, proprio qui, mio\n"
	.string "marito mi chiese di sposarlo.\l"
	.string "Il mare è bello come allora.\p"
	.string "Mufufufu mufufufufufu…$"

LilycoveCity_Text_TeamAquaRenovatedCavern:
	.string "Hm, conosci la strana caverna\n"
	.string "in questa baia?\p"
	.string "Era una grotta naturale, finché non\n"
	.string "è arrivata quella ciurma del\l"
	.string "Team Omega a “ristrutturarla”.\p"
	.string "Dicono di voler abbattere ogni\n"
	.string "confine e liberare il mare…\p"
	.string "Eppure la grotta l'hanno sbarrata\n"
	.string "per bene! Predicano bene e\l"
	.string "razzolano male, quei pirati!$"

LilycoveCity_Text_TeamAquaLotGoneForGood:
	.string "Quella grotta nella baia…\p"
	.string "Non me n'ero accorto subito, ma\n"
	.string "adesso è completamente vuota.\p"
	.string "Quei pirati del Team Omega devono\n"
	.string "aver levato le tende per sempre.$"

LilycoveCity_Text_CitySign:
	.string "Porto Alghepoli\p"
	.string "“Dove finisce la terra comincia\n"
	.string "il mare. E dove finisce il mare…?”$"

LilycoveCity_Text_ContestHallSign:
	.string "Arena delle Virtù\n"
	.string "“Il ritrovo di tutti gli Allenatori!”$"

LilycoveCity_Text_MotelSign:
	.string "Hotel Alga\p"
	.string "“Ricordateci così: l'Alga\n"
	.string "di Porto Alghepoli!”$"

LilycoveCity_Text_MuseumSign:
	.string "Museo di Porto Alghepoli\p"
	.string "“In mostra: capolavori\n"
	.string "dedicati ai Pokémon!”$"

LilycoveCity_Text_MuseumSignPlayersExhibit:
	.string "Museo di Porto Alghepoli\p"
	.string "“In mostra: la collezione\n"
	.string "Pokémon di {PLAYER}!”$"

LilycoveCity_Text_HarborSignUnderConstruction:
	.string "Molo di Porto Alghepoli\p"
	.string "“Il traghetto M/N Acqua è in\n"
	.string "costruzione a Porto Selcepoli.\p"
	.string "“Il servizio inizierà\n"
	.string "a breve.”$"

LilycoveCity_Text_HarborSign:
	.string "Molo di Porto Alghepoli\p"
	.string "“Godetevi una splendida crociera\n"
	.string "a bordo del traghetto M/N Acqua.”$"

LilycoveCity_Text_TrainerFanClubSign:
	.string "Fan Club Allenatori Pokémon\p"
	.string "Il cartello è pieno di nomi di\n"
	.string "Allenatori scarabocchiati…$"

LilycoveCity_Text_DepartmentStoreSign:
	.string "Centro Commerciale di Porto Alghepoli\p"
	.string "“Merce favolosa ed emozioni\n"
	.string "a volontà!\p"
	.string "“Qui trovi proprio quello\n"
	.string "che ti serve!”$"

LilycoveCity_Text_MoveDeletersHouseSign:
	.string "Casa dell'Eliminamosse\p"
	.string "“Si cancellano mosse indesiderate.”$"

LilycoveCity_Text_DoYouKnowAboutBerryBlender:
	.string "Conosci quella macchina chiamata\n"
	.string "Mixer Bacche?$"

LilycoveCity_Text_FasterSpinBetterPokeblocks:
	.string "Bisogna lavorare insieme per farlo\n"
	.string "girare più veloce.\p"
	.string "E più gira veloce, migliori sono le\n"
	.string "Pokémelle che ottieni. Così dicono!$"

LilycoveCity_Text_ExplainBerryBlender:
	.string "Serve a preparare una specie di\n"
	.string "caramella chiamata Pokémella.\p"
	.string "Se dai una buona Pokémella a un\n"
	.string "Pokémon, le sue virtù migliorano\l"
	.string "parecchio.$"

LilycoveCity_Text_HeardTowerCalledSkyPillar:
	.string "Dicono che da qualche parte, lungo\n"
	.string "le rotte marine, ci sia una torre.\p"
	.string "La chiamano Torre dei Cieli.\n"
	.string "Pare che arrivi fino alle nuvole!$"

LilycoveCity_Text_SawTallTowerOnRoute131:
	.string "Ho visto una torre altissima, dalle\n"
	.string "parti del Percorso 131.\p"
	.string "Che sia proprio quella…?$"

LilycoveCity_Text_JustArrivedAndSawRarePokemon:
	.string "Siamo appena arrivati qui per il\n"
	.string "nostro viaggio di nozze.\p"
	.string "E abbiamo visto un Pokémon verde,\n"
	.string "tipo Drago, volare altissimo in cielo!\p"
	.string "Pokémon così vivono a Hoenn?\n"
	.string "O sarà arrivato da un Varco?$"

LilycoveCity_Text_HoneymoonVowToSeeRarePokemon:
	.string "In luna di miele ci siamo promessi\n"
	.string "di vedere più Pokémon rari possibile.\p"
	.string "E ne abbiamo visto uno appena\n"
	.string "arrivati! Che fortuna!\p"
	.string "Insomma, siamo al settimo cielo!$"

