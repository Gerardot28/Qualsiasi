LavaridgeTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LavaridgeTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LavaridgeTown_OnFrame
	.byte 0

LavaridgeTown_OnTransition:
	setflag FLAG_VISITED_LAVARIDGE_TOWN
	call_if_set FLAG_WHITEOUT_TO_LAVARIDGE, LavaridgeTown_EventScript_ClearLavaridgeWhiteOut
	call_if_set FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, LavaridgeTown_EventScript_ShowMtChimneyTrainers
	call Common_EventScript_SetupRivalGfxId
	call Common_EventScript_SetupRivalOnBikeGfxId
	call_if_eq VAR_LAVARIDGE_TOWN_STATE, 1, LavaridgeTown_EventScript_CheckSetRivalPos
	call_if_eq VAR_LAVARIDGE_TOWN_STATE, 1, LavaridgeTown_EventScript_HideMapNamePopup
	end

LavaridgeTown_EventScript_ClearLavaridgeWhiteOut::
	clearflag FLAG_WHITEOUT_TO_LAVARIDGE
	return

LavaridgeTown_EventScript_CheckSetRivalPos::
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_eq VAR_0x8004, 9, LavaridgeTown_EventScript_SetRivalPos
	return

LavaridgeTown_EventScript_SetRivalPos::
	setobjectxyperm LOCALID_LAVARIDGE_RIVAL, 11, 9
	setobjectxyperm LOCALID_LAVARIDGE_RIVAL_ON_BIKE, 9, 8
	setobjectmovementtype LOCALID_LAVARIDGE_RIVAL_ON_BIKE, MOVEMENT_TYPE_FACE_UP
	clearflag FLAG_HIDE_LAVARIDGE_TOWN_RIVAL
	return

LavaridgeTown_EventScript_ShowMtChimneyTrainers::
	clearflag FLAG_HIDE_MT_CHIMNEY_TRAINERS
	return

LavaridgeTown_EventScript_HideMapNamePopup::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	return

LavaridgeTown_OnFrame:
	map_script_2 VAR_LAVARIDGE_TOWN_STATE, 1, LavaridgeTown_EventScript_RivalGiveGoGoggles
	.2byte 0

LavaridgeTown_EventScript_RivalGiveGoGoggles::
	lockall
	getplayerxy VAR_0x8008, VAR_0x8009
	call_if_eq VAR_0x8008, 9, LavaridgeTown_EventScript_RivalNoticePlayer
	call_if_ne VAR_0x8008, 9, LavaridgeTown_EventScript_RivalExitHerbShop
	delay 20
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LavaridgeTown_EventScript_PlayMayMusic
	call_if_eq VAR_RESULT, FEMALE, LavaridgeTown_EventScript_PlayBrendanMusic
	call_if_eq VAR_0x8008, 9, LavaridgeTown_EventScript_RivalApproachPlayer1
	call_if_ne VAR_0x8008, 9, LavaridgeTown_EventScript_RivalApproachPlayer2
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LavaridgeTown_EventScript_MayGiveGoGoggles
	goto_if_eq VAR_RESULT, FEMALE, LavaridgeTown_EventScript_BrendanGiveGoGoggles
	end

LavaridgeTown_EventScript_MayGiveGoGoggles::
	msgbox LavaridgeTown_Text_MayNiceBadgesTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_GO_GOGGLES
	setflag FLAG_RECEIVED_GO_GOGGLES
	msgbox LavaridgeTown_Text_MayExplainGoGogglesChallengeDad, MSGBOX_DEFAULT
	goto LavaridgeTown_EventScript_RivalExit
	end

LavaridgeTown_EventScript_BrendanGiveGoGoggles::
	msgbox LavaridgeTown_Text_BrendanNiceBadgesTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_GO_GOGGLES
	setflag FLAG_RECEIVED_GO_GOGGLES
	msgbox LavaridgeTown_Text_BrendanExplainGoGogglesChallengeDad, MSGBOX_DEFAULT
	goto LavaridgeTown_EventScript_RivalExit
	end

LavaridgeTown_EventScript_RivalExit::
	closemessage
	removeobject LOCALID_LAVARIDGE_RIVAL
	addobject LOCALID_LAVARIDGE_RIVAL_ON_BIKE
	delay 30
	call_if_eq VAR_0x8008, 9, LavaridgeTown_EventScript_RivalExit1
	call_if_ne VAR_0x8008, 9, LavaridgeTown_EventScript_RivalExit2
	removeobject LOCALID_LAVARIDGE_RIVAL_ON_BIKE
	setvar VAR_LAVARIDGE_TOWN_STATE, 2
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	savebgm MUS_DUMMY
	fadedefaultbgm
	releaseall
	end

LavaridgeTown_EventScript_PlayMayMusic::
	playbgm MUS_ENCOUNTER_MAY, TRUE
	return

LavaridgeTown_EventScript_PlayBrendanMusic::
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	return

LavaridgeTown_EventScript_RivalNoticePlayer::
	applymovement LOCALID_LAVARIDGE_RIVAL, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_LAVARIDGE_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_LAVARIDGE_RIVAL, Common_Movement_Delay48
	waitmovement 0
	return

LavaridgeTown_EventScript_RivalExitHerbShop::
	opendoor 12, 15
	waitdooranim
	addobject LOCALID_LAVARIDGE_RIVAL
	applymovement LOCALID_LAVARIDGE_RIVAL, LavaridgeTown_Movement_RivalExitHerbShop
	waitmovement 0
	closedoor 12, 15
	waitdooranim
	applymovement LOCALID_LAVARIDGE_RIVAL, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_LAVARIDGE_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_LAVARIDGE_RIVAL, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

LavaridgeTown_EventScript_RivalApproachPlayer1::
	applymovement LOCALID_LAVARIDGE_RIVAL, LavaridgeTown_Movement_RivalApproachPlayer1
	waitmovement 0
	return

LavaridgeTown_EventScript_RivalApproachPlayer2::
	applymovement LOCALID_LAVARIDGE_RIVAL, LavaridgeTown_Movement_RivalApproachPlayer2
	waitmovement 0
	return

LavaridgeTown_EventScript_RivalExit1::
	applymovement LOCALID_PLAYER, LavaridgeTown_Movement_PlayerWatchRivalExit
	applymovement LOCALID_LAVARIDGE_RIVAL_ON_BIKE, LavaridgeTown_Movement_RivalExit1
	waitmovement 0
	return

LavaridgeTown_EventScript_RivalExit2::
	applymovement LOCALID_LAVARIDGE_RIVAL_ON_BIKE, LavaridgeTown_Movement_RivalExit2
	waitmovement 0
	return

LavaridgeTown_Movement_RivalExit2:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

LavaridgeTown_Movement_PlayerWatchRivalExit:
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

LavaridgeTown_Movement_RivalExit1:
	walk_fast_down
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

LavaridgeTown_Movement_RivalApproachPlayer2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

LavaridgeTown_Movement_RivalApproachPlayer1:
	walk_left
	walk_left
	walk_up
	step_end

LavaridgeTown_Movement_RivalExitHerbShop:
	walk_down
	step_end

LavaridgeTown_EventScript_HotSpringsTrigger::
	specialvar VAR_RESULT, GetPlayerFacingDirection
	goto_if_eq VAR_RESULT, DIR_SOUTH, LavaridgeTown_EventScript_EnteredHotSprings
	end

LavaridgeTown_EventScript_EnteredHotSprings::
	incrementgamestat GAME_STAT_ENTERED_HOT_SPRINGS
	end

LavaridgeTown_EventScript_ExpertM::
	msgbox LavaridgeTown_Text_HotSpringsNeverRunDry, MSGBOX_NPC
	end

LavaridgeTown_EventScript_OldMan::
	msgbox LavaridgeTown_Text_PokemonNippedBackside, MSGBOX_SIGN
	end

LavaridgeTown_EventScript_Twin::
	msgbox LavaridgeTown_Text_BatheInHotSpringsEveryDay, MSGBOX_NPC
	end

LavaridgeTown_EventScript_HotSpringsOldWoman1::
	msgbox LavaridgeTown_Text_IfPokemonInHotSprings, MSGBOX_NPC
	end

LavaridgeTown_EventScript_HotSpringsOldWoman2::
	msgbox LavaridgeTown_Text_HotSpringsClaims, MSGBOX_NPC
	end

LavaridgeTown_EventScript_ExpertF::
	msgbox LavaridgeTown_Text_OhYouLikeHotSprings, MSGBOX_NPC
	end

LavaridgeTown_EventScript_EggWoman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_LAVARIDGE_EGG, LavaridgeTown_EventScript_ReceivedEgg
	msgbox LavaridgeTown_Text_HaveEggWillYouTakeIt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LavaridgeTown_EventScript_DeclineEgg
	getpartysize
	goto_if_eq VAR_RESULT, PARTY_SIZE, LavaridgeTown_EventScript_NoRoomForEgg
	msgbox LavaridgeTown_Text_HopeYoullWalkPlentyWithEgg, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_LAVARIDGE_EGG
	playfanfare MUS_OBTAIN_ITEM
	message LavaridgeTown_Text_ReceivedTheEgg
	waitfanfare
	giveegg SPECIES_WYNAUT
	release
	end

LavaridgeTown_EventScript_ReceivedEgg::
	msgbox LavaridgeTown_Text_EverySoOftenEggFoundAtDayCare, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_EventScript_NoRoomForEgg::
	msgbox LavaridgeTown_Text_NoRoomForThisEgg, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_EventScript_DeclineEgg::
	msgbox LavaridgeTown_Text_AsYouWishThen, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_EventScript_TownSign::
	msgbox LavaridgeTown_Text_TownSign, MSGBOX_SIGN
	end

LavaridgeTown_EventScript_GymSign::
	msgbox LavaridgeTown_Text_GymSign, MSGBOX_SIGN
	end

LavaridgeTown_EventScript_HerbShopSign::
	msgbox LavaridgeTown_Text_HerbShopSign, MSGBOX_SIGN
	end

LavaridgeTown_Text_MayNiceBadgesTakeThis:
	.string "Arianna: {PLAYER}{KUN}! Quanto tempo!\p"
	.string "Oh? Mentre io mi godevo le terme,\n"
	.string "tu hai vinto la Medaglia Fiamma!\p"
	.string "Allora, allora! Primo: hai una bella\n"
	.string "collezione di Medaglie.\l"
	.string "Secondo: i tuoi Pokémon ti adorano.\p"
	.string "Terzo… credo che questi servano più\n"
	.string "a te che a me.$"

LavaridgeTown_Text_MayExplainGoGogglesChallengeDad:
	.string "Arianna: Con gli Occhialoni passerai\n"
	.string "senza problemi nel deserto vicino al\l"
	.string "Percorso 111.\p"
	.string "E sai una cosa? Nel deserto c'è una\n"
	.string "torre che appare e scompare.\p"
	.string "Secondo me è un Varco che non si\n"
	.string "decide! Segno tutto…\p"
	.string "Però, {PLAYER}{KUN}… Una Palestra\n"
	.string "dopo l'altra… Scommetto che ti dai\l"
	.string "da fare per non perdere contro di me!\p"
	.string "{PLAYER}{KUN}, adesso vai a sfidare\n"
	.string "tuo padre alla Palestra di Petalipoli,\l"
	.string "campione!\p"
	.string "Ci vediamo!$"

LavaridgeTown_Text_BrendanNiceBadgesTakeThis:
	.string "Arianna: {PLAYER}, ehi! Quanto tempo!\n"
	.string "Come va?\p"
	.string "Mmh…\n"
	.string "Che bella collezione di Medaglie!\p"
	.string "Allora, allora! Tieni,\n"
	.string "questi ti serviranno.$"

LavaridgeTown_Text_BrendanExplainGoGogglesChallengeDad:
	.string "Arianna: Tienili con te, se pensi di\n"
	.string "attraversare il deserto vicino al\l"
	.string "Percorso 111.\p"
	.string "Laggiù vivono Pokémon che si trovano\n"
	.string "solo nel deserto.\l"
	.string "Dai un'occhiata in giro!\p"
	.string "E c'è anche una torre che appare e\n"
	.string "scompare… Secondo me è un Varco!\l"
	.string "Segno tutto!\p"
	.string "E tu, {PLAYER}, vai a sfidare tuo\n"
	.string "padre, Norman, alla Palestra di\l"
	.string "Petalipoli!\p"
	.string "Ci vediamo!$"

LavaridgeTown_Text_HaveEggWillYouTakeIt:
	.string "Ho qui un Uovo.\p"
	.string "Speravo di farlo schiudere coprendolo\n"
	.string "di sabbia calda vicino alle terme.\l"
	.string "Ma pare che non basti…\p"
	.string "Ho sentito dire che è meglio tenerlo\n"
	.string "insieme ai Pokémon e portarlo\l"
	.string "sempre con sé.\p"
	.string "Tu sei un Allenatore, vero?\n"
	.string "E i tuoi Pokémon sprizzano energia.\p"
	.string "Allora, che ne dici?\n"
	.string "Vuoi prendere quest'Uovo e schiuderlo?$"

LavaridgeTown_Text_HopeYoullWalkPlentyWithEgg:
	.string "Bene! Mi raccomando, cammina tanto\n"
	.string "insieme a quest'Uovo!$"

LavaridgeTown_Text_ReceivedTheEgg:
	.string "{PLAYER} ha ricevuto l'Uovo!$"

LavaridgeTown_Text_NoRoomForThisEgg:
	.string "Oh? Hai troppi Pokémon con te.\n"
	.string "Non c'è posto per quest'Uovo…$"

LavaridgeTown_Text_AsYouWishThen:
	.string "Come preferisci…\p"
	.string "Se cambi idea sull'Uovo,\n"
	.string "mi troverai qui.$"

LavaridgeTown_Text_EverySoOftenEggFoundAtDayCare:
	.string "Ogni tanto, alla Pensione Pokémon,\n"
	.string "spunta fuori un Uovo.\p"
	.string "O almeno, così si dice in giro.$"

LavaridgeTown_Text_HotSpringsNeverRunDry:
	.string "Prendiamo tutta l'acqua calda che\n"
	.string "vogliamo, eppure le terme non si\l"
	.string "esauriscono mai.\p"
	.string "Non è magico?\p"
	.string "Queste sorgenti nascono vicino ai\n"
	.string "vulcani attivi. Le falde sotterranee\l"
	.string "vengono scaldate dal magma e risalgono\l"
	.string "in superficie come acqua termale.\p"
	.string "Ultimamente, però, a volte l'acqua sa\n"
	.string "di ozono… Colpa dei Varchi, dicono.$"

LavaridgeTown_Text_PokemonNippedBackside:
	.string "Starsene sepolti in questa sabbia\n"
	.string "calda è… Aaah…\p"
	.string "Un tepore paradisiaco…\p"
	.string "Eh? Aaahi! Ahia!\p"
	.string "Un Pokémon mi ha morso il sedere!$"

LavaridgeTown_Text_OhYouLikeHotSprings:
	.string "Oh, ti piacciono le terme, eh?\p"
	.string "Strano, per uno giovane come te.$"

LavaridgeTown_Text_BatheInHotSpringsEveryDay:
	.string "Faccio il bagno alle terme ogni giorno.\p"
	.string "Voglio diventare una Capopalestra\n"
	.string "bella e forte come Fiammetta!$"

LavaridgeTown_Text_IfPokemonInHotSprings:
	.string "Se mettessimo dei Pokémon nelle terme,\n"
	.string "verrebbe fuori qualcosa di strano.\p"
	.string "Magari un bagno elettrico, o un bagno\n"
	.string "di bolle, o addirittura un bagno\l"
	.string "di lava…$"

LavaridgeTown_Text_HotSpringsClaims:
	.string "Dicono che queste terme calmino\n"
	.string "i nervi, sciolgano i muscoli,\l"
	.string "risolvano i problemi di cuore,\l"
	.string "attirino i soldi… e adesso pure\l"
	.string "che tengano lontani i Varchi!$"

LavaridgeTown_Text_TownSign:
	.string "Cuordilava\p"
	.string "“Le terme del Centro Pokémon:\n"
	.string "il posto ideale per rilassarsi!”$"

LavaridgeTown_Text_GymSign:
	.string "Palestra Pokémon di Cuordilava\n"
	.string "Capopalestra: Fiammetta\l"
	.string "“Una passione che arde come il fuoco!”$"

LavaridgeTown_Text_HerbShopSign:
	.string "Erboristeria Pokémon\n"
	.string "“Amaro in bocca, guarigione certa!”$"

