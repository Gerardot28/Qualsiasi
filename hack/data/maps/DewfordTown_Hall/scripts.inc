DewfordTown_Hall_MapScripts::
	.byte 0

DewfordTown_Hall_EventScript_Girl::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	special IsTrendyPhraseBoring
	goto_if_eq VAR_RESULT, TRUE, DewfordTown_Hall_EventScript_GirlBoredOfTrend
	msgbox DewfordTown_Hall_Text_CantImagineLifeWithoutTrend, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_GirlBoredOfTrend::
	msgbox DewfordTown_Hall_Text_GettingBoredOfTrend, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_Woman::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Hall_Text_TeachingMonAboutTrend, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_Man::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	special BufferDeepLinkPhrase
	msgbox DewfordTown_Hall_Text_DeepLinkBetweenXAndY, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, DewfordTown_Hall_EventScript_ConfirmTrendLink
	goto_if_eq VAR_RESULT, NO, DewfordTown_Hall_EventScript_RejectTrendLink
	end

DewfordTown_Hall_EventScript_ConfirmTrendLink::
	msgbox DewfordTown_Hall_Text_MyHunchWasRight, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_RejectTrendLink::
	msgbox DewfordTown_Hall_Text_NotEasyToKeepUp, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_ExpertM::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Hall_Text_TVShowAboutTrend, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DEWFORD_HALL_EXPERT_M, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	release
	end

DewfordTown_Hall_EventScript_Twin::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Hall_Text_IsTrendMorePopularAcrossSea, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DEWFORD_HALL_TWIN, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	release
	end

DewfordTown_Hall_EventScript_LittleBoy::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Hall_Text_CollectTrendMerchandise, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_EventScript_Bookshelf::
	lockall
	call Common_EventScript_BufferTrendyPhrase
	msgbox DewfordTown_Hall_Text_BooksAboutTrend, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_Painting::
	lockall
	call Common_EventScript_BufferTrendyPhrase
	special GetDewfordHallPaintingNameIndex
	switch VAR_RESULT
	case 0, DewfordTown_Hall_EventScript_ScreamTitle
	case 4, DewfordTown_Hall_EventScript_ScreamTitle
	case 1, DewfordTown_Hall_EventScript_SmileTitle
	case 5, DewfordTown_Hall_EventScript_ScreamTitle
	case 2, DewfordTown_Hall_EventScript_LastTitle
	case 6, DewfordTown_Hall_EventScript_LastTitle
	case 3, DewfordTown_Hall_EventScript_BirthTitle
	case 7, DewfordTown_Hall_EventScript_LastTitle
	end

DewfordTown_Hall_EventScript_ScreamTitle::
	msgbox DewfordTown_Hall_Text_TrendsScream, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_SmileTitle::
	msgbox DewfordTown_Hall_Text_TrendsSmile, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_LastTitle::
	msgbox DewfordTown_Hall_Text_LastTrend, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_BirthTitle::
	msgbox DewfordTown_Hall_Text_BirthOfTrend, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_SchoolKidM::
	lockall
	setvar VAR_0x8008, 0
	goto DewfordTown_Hall_EventScript_DoTrendDebate
	end

DewfordTown_Hall_EventScript_PsychicM::
	lockall
	setvar VAR_0x8008, 1
	goto DewfordTown_Hall_EventScript_DoTrendDebate
	end

DewfordTown_Hall_EventScript_DoTrendDebate::
	call Common_EventScript_BufferTrendyPhrase
	special GetDewfordHallPaintingNameIndex
	switch VAR_RESULT
	case 0, DewfordTown_Hall_EventScript_TrendDebate1
	case 1, DewfordTown_Hall_EventScript_TrendDebate1
	case 2, DewfordTown_Hall_EventScript_TrendDebate2
	case 3, DewfordTown_Hall_EventScript_TrendDebate2
	case 4, DewfordTown_Hall_EventScript_TrendDebate3
	case 5, DewfordTown_Hall_EventScript_TrendDebate3
	case 6, DewfordTown_Hall_EventScript_TrendDebate4
	case 7, DewfordTown_Hall_EventScript_TrendDebate5
	end

DewfordTown_Hall_EventScript_TrendDebate1::
	call DewfordTown_Hall_EventScript_DebateReact1
	msgbox DewfordTown_Hall_Text_SawTrendCoolestThing, MSGBOX_DEFAULT
	call DewfordTown_Hall_EventScript_DebateReact2
	msgbox DewfordTown_Hall_Text_AllegedTrendNotAuthentic, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_TrendDebate2::
	call DewfordTown_Hall_EventScript_DebateReact1
	msgbox DewfordTown_Hall_Text_ComposedTrendThemeSong, MSGBOX_DEFAULT
	call DewfordTown_Hall_EventScript_DebateReact2
	msgbox DewfordTown_Hall_Text_WorkOnYourSinging, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_TrendDebate3::
	call DewfordTown_Hall_EventScript_DebateReact1
	msgbox DewfordTown_Hall_Text_OrganizeTrendParty, MSGBOX_DEFAULT
	call DewfordTown_Hall_EventScript_DebateReact2
	msgbox DewfordTown_Hall_Text_BrilliantIndeed, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_TrendDebate4::
	call DewfordTown_Hall_EventScript_DebateReact1
	msgbox DewfordTown_Hall_Text_TrendHasBecomePartOfLife, MSGBOX_DEFAULT
	call DewfordTown_Hall_EventScript_DebateReact2
	msgbox DewfordTown_Hall_Text_TrendWeighsHeavilyOnMind, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_TrendDebate5::
	call DewfordTown_Hall_EventScript_DebateReact1
	msgbox DewfordTown_Hall_Text_IfWeTeamUpWellBeInvincible, MSGBOX_DEFAULT
	call DewfordTown_Hall_EventScript_DebateReact2
	msgbox DewfordTown_Hall_Text_WellBeTrendDuo, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Hall_EventScript_DontMovePlayer1::
	return

DewfordTown_Hall_EventScript_DebateReact1::
	applymovement LOCALID_DEWFORD_HALL_PSYCHIC_M, DewfordTown_Hall_Movement_PsychicWalkInPlaceLeft
	waitmovement 0
	goto_if_eq VAR_0x8008, 0, DewfordTown_Hall_EventScript_PlayerReactWest
	goto_if_eq VAR_0x8008, 1, DewfordTown_Hall_EventScript_DontMovePlayer2
	end

DewfordTown_Hall_EventScript_PlayerReactWest::
	goto_if_eq VAR_FACING, DIR_EAST, DewfordTown_Hall_EventScript_DontMovePlayer1
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

DewfordTown_Hall_EventScript_DontMovePlayer2::
	return

DewfordTown_Hall_EventScript_DebateReact2::
	applymovement LOCALID_DEWFORD_HALL_SCHOOL_KID_M, DewfordTown_Hall_Movement_SchoolKidWalkInPlaceRight
	waitmovement 0
	goto_if_eq VAR_0x8008, 0, DewfordTown_Hall_EventScript_PlayerReactNorthSouth
	goto_if_eq VAR_0x8008, 1, DewfordTown_Hall_EventScript_PlayerReactEast
	end

DewfordTown_Hall_EventScript_PlayerReactNorthSouth::
	call_if_eq VAR_FACING, DIR_NORTH, DewfordTown_Hall_EventScript_PlayerWalkInPlaceUp
	call_if_eq VAR_FACING, DIR_SOUTH, DewfordTown_Hall_EventScript_PlayerWalkInPlaceDown
	return

DewfordTown_Hall_EventScript_PlayerWalkInPlaceUp::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

DewfordTown_Hall_EventScript_PlayerWalkInPlaceDown::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	return

DewfordTown_Hall_EventScript_PlayerReactEast::
	goto_if_eq VAR_FACING, DIR_WEST, DewfordTown_Hall_EventScript_DontMovePlayer1
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

DewfordTown_Hall_Movement_PsychicWalkInPlaceLeft:
	walk_in_place_left
	step_end

DewfordTown_Hall_Movement_SchoolKidWalkInPlaceRight:
	walk_in_place_right
	step_end

DewfordTown_Hall_EventScript_SludgeBombMan::
	lock
	faceplayer
	call Common_EventScript_BufferTrendyPhrase
	goto_if_set FLAG_RECEIVED_TM_SLUDGE_BOMB, DewfordTown_Hall_EventScript_ReceivedSludgeBomb
	msgbox DewfordTown_Hall_Text_GiveYouSludgeBomb, MSGBOX_DEFAULT
	giveitem ITEM_TM_SLUDGE_BOMB
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_SLUDGE_BOMB
	release
	end

DewfordTown_Hall_EventScript_ReceivedSludgeBomb::
	msgbox DewfordTown_Hall_Text_LoveSludgeBombButTrendInToo, MSGBOX_DEFAULT
	release
	end

DewfordTown_Hall_Text_CantImagineLifeWithoutTrend:
	.string "Cosa va di moda? Ma è ovvio:\n"
	.string "“{STR_VAR_1}”!\p"
	.string "Non riesco proprio a immaginare\n"
	.string "la vita senza “{STR_VAR_1}”!$"

DewfordTown_Hall_Text_GettingBoredOfTrend:
	.string "Cosa va di moda? Ma è ovvio:\n"
	.string "“{STR_VAR_1}”!\p"
	.string "Però comincia ad annoiarmi un po'.\p"
	.string "Dovrei cercare la prossima novità…\n"
	.string "Magari arriverà da un Varco!$"

DewfordTown_Hall_Text_TeachingMonAboutTrend:
	.string "Sto spiegando ai miei Pokémon\n"
	.string "cos'è “{STR_VAR_1}”,\l"
	.string "ma non sta andando un granché.\p"
	.string "Forse è un concetto troppo avanzato.$"

DewfordTown_Hall_Text_DeepLinkBetweenXAndY:
	.string "Mi sto aggiornando sulla tendenza\n"
	.string "del momento: “{STR_VAR_1}”.\p"
	.string "È vero che c'è un legame profondo\n"
	.string "tra “{STR_VAR_1}”\l"
	.string "e “{STR_VAR_2}”?$"

DewfordTown_Hall_Text_MyHunchWasRight:
	.string "Oh!\n"
	.string "Allora avevo intuito giusto!\p"
	.string "Sono un passo più vicino a essere\n"
	.string "sempre sul pezzo! Evvai!$"

DewfordTown_Hall_Text_NotEasyToKeepUp:
	.string "Cosa?!\n"
	.string "Davvero?!\p"
	.string "Non è facile, per un signore della mia\n"
	.string "età, stare dietro alle mode!$"

DewfordTown_Hall_Text_TVShowAboutTrend:
	.string "Tutta questa storia di\n"
	.string "“{STR_VAR_1}”…\l"
	.string "Non ci fanno pure un programma in TV?$"

DewfordTown_Hall_Text_IsTrendMorePopularAcrossSea:
	.string "Al di là del mare… o di un Varco…\p"
	.string "“{STR_VAR_1}”\n"
	.string "va ancora più di moda?$"

DewfordTown_Hall_Text_CollectTrendMerchandise:
	.string "Colleziono gadget ufficiali di\n"
	.string "“{STR_VAR_1}”\l"
	.string "con tanto di licenza.\p"
	.string "Ho le bambole ufficiali di\n"
	.string "“{STR_VAR_1}”…\p"
	.string "I vestiti firmati\n"
	.string "“{STR_VAR_1}”…\p"
	.string "E i libri illustrati ufficiali di\n"
	.string "“{STR_VAR_1}”,\l"
	.string "con tanto di bollino.\p"
	.string "Eheh, sono tutti miei!\n"
	.string "Non te ne presto neanche uno!$"

DewfordTown_Hall_Text_BooksAboutTrend:
	.string "“{STR_VAR_1}”:\n"
	.string "pregi e difetti…\p"
	.string "“{STR_VAR_1}”\n"
	.string "e i Pokémon: un legame segreto…\p"
	.string "Usi pratici di\n"
	.string "“{STR_VAR_1}”…\p"
	.string "Lo scaffale è pieno zeppo di libri su\n"
	.string "“{STR_VAR_1}”.$"

DewfordTown_Hall_Text_TrendsScream:
	.string "Il titolo è “L'urlo di\n"
	.string "{STR_VAR_1}”.$"

DewfordTown_Hall_Text_TrendsSmile:
	.string "Il titolo è “Il sorriso di\n"
	.string "{STR_VAR_1}”.$"

DewfordTown_Hall_Text_LastTrend:
	.string "Il titolo è “Addio,\n"
	.string "{STR_VAR_1}”.$"

DewfordTown_Hall_Text_BirthOfTrend:
	.string "Il titolo è “La nascita di\n"
	.string "{STR_VAR_1}”.$"

DewfordTown_Hall_Text_SawTrendCoolestThing:
	.string "Ho visto “{STR_VAR_1}”!\n"
	.string "Forte, eh?\p"
	.string "È tipo la cosa più forte del momento!\p"
	.string "Una roba pazzesca!\n"
	.string "Era proprio quello vero, giuro!$"

DewfordTown_Hall_Text_AllegedTrendNotAuthentic:
	.string "Oh, no, no, no.\p"
	.string "Quel presunto\n"
	.string "“{STR_VAR_1}”\l"
	.string "che dici di aver visto? Era un falso.\p"
	.string "Quello autentico è molto più…\n"
	.string "Come posso dire?\l"
	.string "Più deciso, eppure più delicato!\p"
	.string "Ah, poco importa. È strabiliante!$"

DewfordTown_Hall_Text_ComposedTrendThemeSong:
	.string "Ehi, senti qua: ho composto una sigla\n"
	.string "per “{STR_VAR_1}”.\p"
	.string "“{STR_VAR_1}”\n"
	.string "va fortissimo in questo periodo.\p"
	.string "Pronto? Ascolta!\n"
	.string "… …\p"
	.string "Ooh!\n"
	.string "{STR_VAR_1}!\p"
	.string "Evviva\n"
	.string "{STR_VAR_1}!$"

DewfordTown_Hall_Text_WorkOnYourSinging:
	.string "… …\p"
	.string "Mi permetto di dirti, caro mio, che\n"
	.string "faresti bene a esercitarti col canto\l"
	.string "prima di cimentarti con\l"
	.string "“{STR_VAR_1}”.$"

DewfordTown_Hall_Text_OrganizeTrendParty:
	.string "Comunque, come dicevo prima,\n"
	.string "dovremmo organizzare una festa\l"
	.string "a tema “{STR_VAR_1}”\l"
	.string "qui sull'isola.$"

DewfordTown_Hall_Text_BrilliantIndeed:
	.string "Oh, un'idea a dir poco splendida!\p"
	.string "Così stabiliremo una volta per tutte\n"
	.string "chi è il migliore in fatto di\l"
	.string "“{STR_VAR_1}”.\l"
	.string "Davvero geniale!\p"
	.string "Da oggi in poi, la nostra vita ruoterà\n"
	.string "attorno a “{STR_VAR_1}”!$"

DewfordTown_Hall_Text_TrendHasBecomePartOfLife:
	.string "Però stavo pensando…\p"
	.string "Non trovi anche tu che\n"
	.string "“{STR_VAR_1}”,\l"
	.string "da semplice moda, sia diventato\l"
	.string "parte della nostra vita quotidiana?$"

DewfordTown_Hall_Text_TrendWeighsHeavilyOnMind:
	.string "Prego?\n"
	.string "Fino a questo punto?\p"
	.string "Comunque è vero che\n"
	.string "“{STR_VAR_1}”\l"
	.string "ti occupa la mente sia da sveglio\l"
	.string "sia mentre dormi.\p"
	.string "Non c'è dubbio: sei il Professore di\n"
	.string "“{STR_VAR_1}”,\l"
	.string "vecchio mio!$"

DewfordTown_Hall_Text_IfWeTeamUpWellBeInvincible:
	.string "Se io e te facciamo coppia fissa,\n"
	.string "saremo invincibili in tutto ciò che\l"
	.string "riguarda “{STR_VAR_1}”!$"

DewfordTown_Hall_Text_WellBeTrendDuo:
	.string "Hai fatto centro, amico mio!\p"
	.string "Saremo il Duo\n"
	.string "“{STR_VAR_1}”!\p"
	.string "Non è un'immagine perfetta?\n"
	.string "Ahahah!$"

DewfordTown_Hall_Text_GiveYouSludgeBomb:
	.string "Per me, la mossa più di moda in\n"
	.string "assoluto è Fangobomba. Non si discute.\p"
	.string "Eh? Mi stai dicendo che non conosci\n"
	.string "Fangobomba?\p"
	.string "Ma è una cosa penosa!\n"
	.string "Tieni, te ne regalo una.$"

DewfordTown_Hall_Text_LoveSludgeBombButTrendInToo:
	.string "Adoro Fangobomba.\p"
	.string "Però anche i Pokémon con la mossa\n"
	.string "“{STR_VAR_1}”\l"
	.string "vanno parecchio di moda.$"

