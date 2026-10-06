MauvilleCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MauvilleCity_OnTransition
	.byte 0

MauvilleCity_OnTransition:
	setflag FLAG_VISITED_MAUVILLE_CITY
	clearflag FLAG_FORCE_MIRAGE_TOWER_VISIBLE
	clearflag FLAG_HIDE_SLATEPORT_MUSEUM_POPULATION
	setflag FLAG_SYS_TV_START
	clearflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	setvar VAR_MAUVILLE_GYM_STATE, 0
	call_if_set FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON, MauvilleCity_EventScript_MoveWattsonBackToGym
	end

MauvilleCity_EventScript_MoveWattsonBackToGym::
	clearflag FLAG_HIDE_MAUVILLE_GYM_WATTSON
	setflag FLAG_HIDE_MAUVILLE_CITY_WATTSON
	setflag FLAG_WATTSON_REMATCH_AVAILABLE
	return

MauvilleCity_EventScript_Boy::
	msgbox MauvilleCity_Text_NurseHurtMonBackToHealth, MSGBOX_NPC
	end

MauvilleCity_EventScript_Maniac::
	msgbox MauvilleCity_Text_AllSortsOfPeopleComeThrough, MSGBOX_NPC
	end

MauvilleCity_EventScript_Woman::
	msgbox MauvilleCity_Text_RydelVeryGenerous, MSGBOX_NPC
	end

MauvilleCity_EventScript_RichBoy::
	msgbox MauvilleCity_Text_PokemonCanJumpYouOnBike, MSGBOX_NPC
	end

MauvilleCity_EventScript_CitySign::
	msgbox MauvilleCity_Text_CitySign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_GymSign::
	msgbox MauvilleCity_Text_GymSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_BikeShopSign::
	msgbox MauvilleCity_Text_BikeShopSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_GameCornerSign::
	msgbox MauvilleCity_Text_GameCornerSign, MSGBOX_SIGN
	end

MauvilleCity_EventScript_SchoolKidM::
	lock
	faceplayer
	goto_if_set FLAG_TV_EXPLAINED, MauvilleCity_EventScript_TVExplained
	msgbox MauvilleCity_Text_ExplainTV, MSGBOX_DEFAULT
	setflag FLAG_TV_EXPLAINED
	release
	end

MauvilleCity_EventScript_TVExplained::
	msgbox MauvilleCity_Text_BeenCheckingOutTV, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_WallysUncle::
	lock
	faceplayer
	goto_if_set FLAG_DECLINED_WALLY_BATTLE_MAUVILLE, MauvilleCity_EventScript_UncleAskPlayerToBattleWally
	msgbox MauvilleCity_Text_UncleHesTooPeppy, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

MauvilleCity_EventScript_UncleAskPlayerToBattleWally::
	msgbox MauvilleCity_Text_UncleCanYouBattleWally, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

MauvilleCity_EventScript_Wally::
	lockall
	goto_if_set FLAG_DECLINED_WALLY_BATTLE_MAUVILLE, MauvilleCity_EventScript_WallyRequestBattleAgain
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_WallyWantToChallengeGym, MSGBOX_DEFAULT
	msgbox MauvilleCity_Text_UncleYourePushingIt, MSGBOX_DEFAULT
	msgbox MauvilleCity_Text_WallyWeCanBeatAnyone, MSGBOX_DEFAULT
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_Delay48
	waitmovement 0
	msgbox MauvilleCity_Text_WallyWillYouBattleMe, MSGBOX_YESNO
	goto MauvilleCity_EventScript_BattleWallyPrompt
	end

MauvilleCity_EventScript_BattleWallyPrompt::
	call_if_eq VAR_RESULT, YES, MauvilleCity_EventScript_BattleWally
	goto_if_eq VAR_RESULT, NO, MauvilleCity_EventScript_DeclineWallyBattle
	closemessage
	switch VAR_FACING
	case DIR_NORTH, MauvilleCity_EventScript_WallyAndUncleExitNorth
	case DIR_EAST, MauvilleCity_EventScript_WallyAndUncleExitEast
	end

MauvilleCity_EventScript_WallyAndUncleExitNorth::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitNorth1
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitNorth1
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitNorth1
	waitmovement 0
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 30
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerFaceUncleNorth
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleApproachPlayerNorth
	waitmovement 0
	msgbox MauvilleCity_Text_UncleVisitUsSometime, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitNorth2
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitNorth2
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitNorth2
	waitmovement 0
	goto MauvilleCity_EventScript_DefeatedWally
	end

MauvilleCity_EventScript_WallyAndUncleExitEast::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitEast1
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitEast1
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitEast1
	waitmovement 0
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 30
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleApproachPlayerEast
	waitmovement 0
	msgbox MauvilleCity_Text_UncleVisitUsSometime, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchWallyExitEast2
	applymovement LOCALID_MAUVILLE_WALLYS_UNCLE, MauvilleCity_Movement_WallysUncleExitEast2
	applymovement LOCALID_MAUVILLE_WALLY, MauvilleCity_Movement_WallyExitEast2
	waitmovement 0
	goto MauvilleCity_EventScript_DefeatedWally
	end

MauvilleCity_EventScript_DefeatedWally::
	removeobject LOCALID_MAUVILLE_WALLY
	removeobject LOCALID_MAUVILLE_WALLYS_UNCLE
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY
	clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLYS_UNCLE
	setflag FLAG_DEFEATED_WALLY_MAUVILLE
	setvar VAR_WALLY_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL
	call_if_eq VAR_FACING, DIR_NORTH, MauvilleCity_EventScript_ScottApproachPlayerNorth
	call_if_eq VAR_FACING, DIR_EAST, MauvilleCity_EventScript_ScottApproachPlayerEast
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox MauvilleCity_Text_ScottYouDidntHoldBack, MSGBOX_DEFAULT
	closemessage
	addvar VAR_SCOTT_STATE, 1
	call_if_eq VAR_FACING, DIR_NORTH, MauvilleCity_EventScript_ScottExitNorth
	call_if_eq VAR_FACING, DIR_EAST, MauvilleCity_EventScript_ScottExitEast
	removeobject LOCALID_MAUVILLE_SCOTT
	releaseall
	end

MauvilleCity_EventScript_ScottApproachPlayerNorth::
	addobject LOCALID_MAUVILLE_SCOTT
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottApproachPlayerNorth
	waitmovement 0
	return

MauvilleCity_EventScript_ScottApproachPlayerEast::
	setobjectxyperm LOCALID_MAUVILLE_SCOTT, 12, 13
	addobject LOCALID_MAUVILLE_SCOTT
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottApproachPlayerEast
	waitmovement 0
	return

MauvilleCity_EventScript_ScottExitNorth::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchScottExitNorth
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottExitNorth
	waitmovement 0
	return

MauvilleCity_EventScript_ScottExitEast::
	applymovement LOCALID_PLAYER, MauvilleCity_Movement_PlayerWatchScottExitEast
	applymovement LOCALID_MAUVILLE_SCOTT, MauvilleCity_Movement_ScottExitEast
	waitmovement 0
	return

MauvilleCity_EventScript_BattleWally::
	msgbox MauvilleCity_Text_WallyHereICome, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLY_MAUVILLE, MauvilleCity_Text_WallyDefeat
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_WallyIllGoBackToVerdanturf, MSGBOX_DEFAULT
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MauvilleCity_Text_ThankYouNotEnoughToBattle, MSGBOX_DEFAULT
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox MauvilleCity_Text_UncleNoNeedToBeDown, MSGBOX_DEFAULT
	return

MauvilleCity_EventScript_DeclineWallyBattle::
	setflag FLAG_DECLINED_WALLY_BATTLE_MAUVILLE
	msgbox MauvilleCity_Text_WallyMyUncleWontKnowImStrong, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_WallyRequestBattleAgain::
	applymovement LOCALID_MAUVILLE_WALLY, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MauvilleCity_Text_WallyPleaseBattleMe, MSGBOX_YESNO
	goto MauvilleCity_EventScript_BattleWallyPrompt
	end

MauvilleCity_Movement_WallyExitNorth1:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	step_end

MauvilleCity_Movement_WallyExitEast1:
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_WallyExitNorth2:
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	delay_8
	step_end

MauvilleCity_Movement_WallyExitEast2:
	delay_16
	delay_16
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	delay_8
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitNorth2:
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitEast2:
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchScottExitNorth:
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchScottExitEast:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitEast1:
	delay_16
	walk_in_place_faster_down
	step_end

MauvilleCity_Movement_PlayerWatchWallyExitNorth1:
	delay_16
	walk_in_place_faster_left
	step_end

MauvilleCity_Movement_WallysUncleExitNorth1:
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end

MauvilleCity_Movement_WallysUncleExitEast1:
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_PlayerFaceUncleNorth:
	delay_16
	delay_8
	delay_4
	walk_in_place_faster_down
	step_end

MauvilleCity_Movement_WallysUncleApproachPlayerNorth:
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

MauvilleCity_Movement_WallysUncleApproachPlayerEast:
	walk_right
	walk_up
	step_end

MauvilleCity_Movement_WallysUncleExitNorth2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_WallysUncleExitEast2:
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

MauvilleCity_Movement_ScottApproachPlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

MauvilleCity_Movement_ScottApproachPlayerEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

MauvilleCity_Movement_ScottExitNorth:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_Movement_ScottExitEast:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MauvilleCity_EventScript_Wattson::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON, MauvilleCity_EventScript_ReceivedThunderbolt
	goto_if_eq VAR_NEW_MAUVILLE_STATE, 2, MauvilleCity_EventScript_CompletedNewMauville
	goto_if_set FLAG_GOT_BASEMENT_KEY_FROM_WATTSON, MauvilleCity_EventScript_BegunNewMauville
	msgbox MauvilleCity_Text_WattsonNeedFavorTakeKey, MSGBOX_DEFAULT
	giveitem ITEM_BASEMENT_KEY
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_BASEMENT_KEY_FROM_WATTSON
	msgbox MauvilleCity_Text_WattsonWontBeChallenge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_BegunNewMauville::
	msgbox MauvilleCity_Text_WattsonWontBeChallenge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_CompletedNewMauville::
	msgbox MauvilleCity_Text_WattsonThanksTakeTM, MSGBOX_DEFAULT
	giveitem ITEM_TM_THUNDERBOLT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON
	msgbox MauvilleCity_Text_WattsonYoungTakeCharge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_ReceivedThunderbolt::
	msgbox MauvilleCity_Text_WattsonYoungTakeCharge, MSGBOX_DEFAULT
	release
	end

MauvilleCity_EventScript_RegisterWallyCall::
	lockall
	pokenavcall MauvilleCity_Text_WallyPokenavCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MauvilleCity_Text_RegisteredWally, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WALLY_MATCH_CALL
	clearflag FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL
	releaseall
	end

MauvilleCity_Text_UncleHesTooPeppy:
	.string "Zio: È merito dei Pokémon se questo\n"
	.string "ragazzo ha tanta energia, credo…\l"
	.string "Ma adesso ne ha fin troppa!$"

MauvilleCity_Text_WallyWantToChallengeGym:
	.string "Lino: Dai, zio, ti prego…\p"
	.string "Voglio sfidare questa Palestra e\n"
	.string "vedere quanto sono migliorato.\p"
	.string "Posso? Per favore… posso?$"

MauvilleCity_Text_UncleYourePushingIt:
	.string "Zio: Calma, Lino.\p"
	.string "Da quando vivi insieme ai Pokémon\n"
	.string "sei diventato molto più forte.\p"
	.string "Ma sfidare subito una Palestra…\n"
	.string "non ti sembra di esagerare?$"

MauvilleCity_Text_WallyWeCanBeatAnyone:
	.string "Lino: Non sto esagerando.\p"
	.string "Se io e Ralts uniamo le forze,\n"
	.string "possiamo battere chiunque!$"

MauvilleCity_Text_WallyWillYouBattleMe:
	.string "Lino: Oh! Ciao, {PLAYER}!\p"
	.string "Sono diventato molto più forte\n"
	.string "da quando ci siamo conosciuti.\p"
	.string "{PLAYER}, voglio che tu e mio zio\n"
	.string "lo vediate con i vostri occhi.\p"
	.string "{PLAYER}, ti prego… vuoi lottare\n"
	.string "con me?$"

MauvilleCity_Text_WallyMyUncleWontKnowImStrong:
	.string "Lino: Oh… Se non lotti con me,\n"
	.string "{PLAYER}, mio zio non capirà mai che\l"
	.string "sono diventato forte davvero.$"

MauvilleCity_Text_UncleCanYouBattleWally:
	.string "Zio: {PLAYER}{KUN}, giusto?\n"
	.string "Posso chiederti di lottare con Lino,\l"
	.string "almeno per questa volta?\p"
	.string "Così com'è adesso, non credo che\n"
	.string "darà retta a nessun ragionamento.$"

MauvilleCity_Text_WallyPleaseBattleMe:
	.string "Lino: {PLAYER}, ti prego!\n"
	.string "Lotta con me, per favore.$"

MauvilleCity_Text_WallyHereICome:
	.string "Lino: {PLAYER}, grazie.\p"
	.string "Io… ce la posso fare.\n"
	.string "Arrivo!$"

MauvilleCity_Text_WallyDefeat:
	.string "Lino: … … … … … … …\p"
	.string "… … … … … … … …\p"
	.string "Ho perso…$"

MauvilleCity_Text_WallyIllGoBackToVerdanturf:
	.string "Lino: Zio…\n"
	.string "Torno a Mentania…$"

MauvilleCity_Text_ThankYouNotEnoughToBattle:
	.string "{PLAYER}, grazie.\n"
	.string "Fare l'Allenatore è difficile, vero?\p"
	.string "Ho perso… ma Ralts era felice.\n"
	.string "Lo sento.\p"
	.string "Non basta avere dei Pokémon e farli\n"
	.string "lottare. Essere un vero Allenatore\l"
	.string "significa molto di più.$"

MauvilleCity_Text_UncleNoNeedToBeDown:
	.string "Zio: Lino, non c'è bisogno di\n"
	.string "buttarsi giù così.\p"
	.string "Chi ti impedisce di diventare\n"
	.string "sempre più forte, eh?\p"
	.string "Dai, torniamo a casa.\n"
	.string "Ci stanno aspettando tutti.$"

MauvilleCity_Text_UncleVisitUsSometime:
	.string "Zio: {PLAYER}{KUN}, ora che ci penso…\n"
	.string "Sei tu l'Allenatore che ha aiutato\l"
	.string "Lino a catturare il suo primo\l"
	.string "Pokémon!\p"
	.string "Perché non vieni a trovarci a\n"
	.string "Mentania, una volta o l'altra?\l"
	.string "Sono sicuro che Lino ne sarà felice.$"


MauvilleCity_Text_WallyPokenavCall:
	.string "… … … … … …\n"
	.string "… … … … … Bip!\p"
	.string "Lino: Oh, ciao, {PLAYER}!\p"
	.string "Volevo solo dirti che mio zio\n"
	.string "mi ha regalato un PokéNav!\p"
	.string "Così possiamo sentirci quando\n"
	.string "vogliamo, {PLAYER}!\p"
	.string "… … … … … …\n"
	.string "… … … … … Clic!$"

MauvilleCity_Text_RegisteredWally:
	.string "Lino è stato registrato\n"
	.string "nel PokéNav.$"

MauvilleCity_Text_ScottYouDidntHoldBack:
	.string "Scott: Eheh…\n"
	.string "Ho visto tutta la lotta!\p"
	.string "Tu e quel ragazzo, Lino, siete amici,\n"
	.string "vero?\p"
	.string "Eppure non ti sei trattenuto e l'hai\n"
	.string "battuto alla grande.\p"
	.string "Sì! È questo il vero spirito di una\n"
	.string "lotta Pokémon!\p"
	.string "Allenatori così, un giorno, mi\n"
	.string "serviranno per il mio progetto…\p"
	.string "… … … … … …\n"
	.string "Farò il tifo per te!$"

MauvilleCity_Text_WattsonNeedFavorTakeKey:
	.string "Walter: Oh, {PLAYER}{KUN}!\n"
	.string "Hai l'aria di uno pieno di scintille!\l"
	.string "Mi piace, uahahahah!\p"
	.string "Bene! Ho deciso!\n"
	.string "Ho un favore da chiederti, {PLAYER}{KUN}!\p"
	.string "Sotto Ciclamipoli c'è un settore\n"
	.string "sotterraneo chiamato Ciclanova.\p"
	.string "{PLAYER}{KUN}, vorrei che andassi laggiù\n"
	.string "a spegnere il generatore.\p"
	.string "Quel generatore è ancora… instabile.\n"
	.string "Perché? Una vecchia storia, lasciamo\l"
	.string "perdere. Ma è diventato pericoloso.\p"
	.string "Ecco, questa è la chiave per entrare\n"
	.string "a Ciclanova.$"

MauvilleCity_Text_WattsonWontBeChallenge:
	.string "Walter: Non preoccuparti.\n"
	.string "Per te sarà una passeggiata.\p"
	.string "L'ingresso di Ciclanova è a un breve\n"
	.string "tratto in Surf dal Percorso 110.\p"
	.string "Allora è deciso: mi fido di te!\n"
	.string "Uahahahah!$"

MauvilleCity_Text_WattsonThanksTakeTM:
	.string "Walter: Uahahahah!\p"
	.string "Lo sapevo, {PLAYER}{KUN}! Sapevo di aver\n"
	.string "chiesto alla persona giusta!\p"
	.string "Finalmente quel vecchio macchinario\n"
	.string "riposa in pace. Grazie davvero.\p"
	.string "Per ringraziarti, ecco una MT\n"
	.string "che contiene Fulmine!\p"
	.string "Prendila, te la sei meritata!$"

MauvilleCity_Text_WattsonYoungTakeCharge:
	.string "Walter: Uahahahah!\p"
	.string "Non c'è niente di più bello che\n"
	.string "vedere i giovani rimboccarsi\l"
	.string "le maniche e prendere il comando!$"

MauvilleCity_Text_NurseHurtMonBackToHealth:
	.string "Le lotte Pokémon sono fighissime,\n"
	.string "e tutto il resto…\p"
	.string "Ma se un tuo Pokémon si fa male,\n"
	.string "devi curarlo finché non sta bene.$"

MauvilleCity_Text_AllSortsOfPeopleComeThrough:
	.string "Le strade di questa città vanno a nord\n"
	.string "e a sud, a est e a ovest.\p"
	.string "Per questo di qui passa gente di ogni\n"
	.string "tipo… e ultimamente pure Pokémon\l"
	.string "sbucati da chissà quale Varco!$"

MauvilleCity_Text_RydelVeryGenerous:
	.string "Sei già stato da Bici Rydel?\p"
	.string "Rydel, il proprietario, è un uomo\n"
	.string "molto generoso.$"

MauvilleCity_Text_PokemonCanJumpYouOnBike:
	.string "Anche se vai in Bici, i Pokémon\n"
	.string "selvatici possono saltarti addosso.$"

MauvilleCity_Text_CitySign:
	.string "Ciclamipoli\n"
	.string "“La città che non si spegne mai!”$"

MauvilleCity_Text_GymSign:
	.string "Palestra Pokémon di Ciclamipoli\n"
	.string "Capopalestra: Walter\l"
	.string "“L'uomo che elettrizza di allegria!”$"

MauvilleCity_Text_BikeShopSign:
	.string "“Ghiaia sotto le ruote, brividi\n"
	.string "nell'anima!”\l"
	.string "Bici Rydel$"

MauvilleCity_Text_GameCornerSign:
	.string "“Il divertimento per tutti!”\n"
	.string "Sala Giochi di Ciclamipoli$"

MauvilleCity_Text_ExplainTV:
	.string "Ciao! Guardi mai la TV?\p"
	.string "Ultimamente ci sono un sacco di\n"
	.string "programmi nuovi. Pure sui Varchi!\p"
	.string "La TV è fantastica. Ogni tanto ci vedo\n"
	.string "i miei amici, così, all'improvviso.\p"
	.string "E a volte ti mettono in TV senza\n"
	.string "nemmeno avvisarti!\p"
	.string "Mi piacerebbe tanto finirci anch'io.\p"
	.string "Per questo ti dico: guarda la TV\n"
	.string "ogni volta che puoi!$"

MauvilleCity_Text_BeenCheckingOutTV:
	.string "Ciao! Hai guardato un po' di TV?$"
