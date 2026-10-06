BattleFrontier_Lounge7_MapScripts::
	.byte 0

@special ChooseMonForMoveTutor
BattleFrontier_Lounge7_EventScript_LeftMoveTutor::
	lock
	faceplayer
	setvar VAR_TEMP_C, SCROLL_MULTI_BF_MOVE_TUTOR_1
	goto_if_set FLAG_MET_FRONTIER_BEAUTY_MOVE_TUTOR, BattleFrontier_Lounge7_EventScript_AlreadyMetLeftTutor
	msgbox BattleFrontier_Lounge7_Text_LeftTutorIntro, MSGBOX_DEFAULT
	setflag FLAG_MET_FRONTIER_BEAUTY_MOVE_TUTOR
	goto BattleFrontier_Lounge7_EventScript_ChooseLeftTutorMove
	end

BattleFrontier_Lounge7_EventScript_AlreadyMetLeftTutor::
	msgbox BattleFrontier_Lounge7_Text_LeftTutorWelcomeBack, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge7_EventScript_ChooseLeftTutorMove
	end

BattleFrontier_Lounge7_EventScript_ChooseLeftTutorMove::
	message BattleFrontier_Lounge7_Text_TeachWhichMove
	waitmessage
	special ShowBattlePointsWindow
	setvar VAR_TEMP_FRONTIER_TUTOR_ID, 0
	setvar VAR_0x8004, SCROLL_MULTI_BF_MOVE_TUTOR_1
	setvar VAR_0x8006, 0
	special ShowScrollableMultichoice
	copyvar VAR_TEMP_FRONTIER_TUTOR_SELECTION, VAR_RESULT
	switch VAR_RESULT
	case 0, BattleFrontier_Lounge7_EventScript_Softboiled
	case 1, BattleFrontier_Lounge7_EventScript_SeismicToss
	case 2, BattleFrontier_Lounge7_EventScript_DreamEater
	case 3, BattleFrontier_Lounge7_EventScript_MegaPunch
	case 4, BattleFrontier_Lounge7_EventScript_MegaKick
	case 5, BattleFrontier_Lounge7_EventScript_BodySlam
	case 6, BattleFrontier_Lounge7_EventScript_RockSlide
	case 7, BattleFrontier_Lounge7_EventScript_Counter
	case 8, BattleFrontier_Lounge7_EventScript_ThunderWave
	case 9, BattleFrontier_Lounge7_EventScript_SwordsDance
	case 10, BattleFrontier_Lounge7_EventScript_ExitTutorMoveSelect
	case MULTI_B_PRESSED, BattleFrontier_Lounge7_EventScript_ExitTutorMoveSelect
	end

BattleFrontier_Lounge7_EventScript_ChooseNewLeftTutorMove::
	message BattleFrontier_Lounge7_Text_TeachWhichMove
	waitmessage
	setvar VAR_TEMP_FRONTIER_TUTOR_ID, 0
	setvar VAR_0x8004, SCROLL_MULTI_BF_MOVE_TUTOR_1
	setvar VAR_0x8006, 1
	special ShowScrollableMultichoice
	copyvar VAR_TEMP_FRONTIER_TUTOR_SELECTION, VAR_RESULT
	switch VAR_RESULT
	case 0, BattleFrontier_Lounge7_EventScript_Softboiled
	case 1, BattleFrontier_Lounge7_EventScript_SeismicToss
	case 2, BattleFrontier_Lounge7_EventScript_DreamEater
	case 3, BattleFrontier_Lounge7_EventScript_MegaPunch
	case 4, BattleFrontier_Lounge7_EventScript_MegaKick
	case 5, BattleFrontier_Lounge7_EventScript_BodySlam
	case 6, BattleFrontier_Lounge7_EventScript_RockSlide
	case 7, BattleFrontier_Lounge7_EventScript_Counter
	case 8, BattleFrontier_Lounge7_EventScript_ThunderWave
	case 9, BattleFrontier_Lounge7_EventScript_SwordsDance
	case 10, BattleFrontier_Lounge7_EventScript_ExitTutorMoveSelect
	case MULTI_B_PRESSED, BattleFrontier_Lounge7_EventScript_ExitTutorMoveSelect
	end

BattleFrontier_Lounge7_EventScript_Softboiled::
	setvar VAR_0x8008, 16
	setvar VAR_0x8005, MOVE_SOFT_BOILED
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_SeismicToss::
	setvar VAR_0x8008, 24
	setvar VAR_0x8005, MOVE_SEISMIC_TOSS
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_DreamEater::
	setvar VAR_0x8008, 24
	setvar VAR_0x8005, MOVE_DREAM_EATER
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_MegaPunch::
	setvar VAR_0x8008, 24
	setvar VAR_0x8005, MOVE_MEGA_PUNCH
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_MegaKick::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_MEGA_KICK
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_BodySlam::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_BODY_SLAM
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_RockSlide::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_ROCK_SLIDE
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_Counter::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_COUNTER
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_ThunderWave::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_THUNDER_WAVE
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_SwordsDance::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_SWORDS_DANCE
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_RightMoveTutor::
	lock
	faceplayer
	setvar VAR_TEMP_C, SCROLL_MULTI_BF_MOVE_TUTOR_2
	goto_if_set FLAG_MET_FRONTIER_SWIMMER_MOVE_TUTOR, BattleFrontier_Lounge7_EventScript_AlreadyMetRightTutor
	msgbox BattleFrontier_Lounge7_Text_RightTutorIntro, MSGBOX_DEFAULT
	setflag FLAG_MET_FRONTIER_SWIMMER_MOVE_TUTOR
	goto BattleFrontier_Lounge7_EventScript_ChooseRightTutorMove
	end

BattleFrontier_Lounge7_EventScript_AlreadyMetRightTutor::
	msgbox BattleFrontier_Lounge7_Text_RightTutorWelcomeBack, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge7_EventScript_ChooseRightTutorMove
	end

BattleFrontier_Lounge7_EventScript_ChooseRightTutorMove::
	message BattleFrontier_Lounge7_Text_TeachWhichMove
	waitmessage
	special ShowBattlePointsWindow
	setvar VAR_TEMP_FRONTIER_TUTOR_ID, 1
	setvar VAR_0x8004, SCROLL_MULTI_BF_MOVE_TUTOR_2
	setvar VAR_0x8006, 0
	special ShowScrollableMultichoice
	copyvar VAR_TEMP_FRONTIER_TUTOR_SELECTION, VAR_RESULT
	switch VAR_RESULT
	case 0, BattleFrontier_Lounge7_EventScript_DefenseCurl
	case 1, BattleFrontier_Lounge7_EventScript_Snore
	case 2, BattleFrontier_Lounge7_EventScript_MudSlap
	case 3, BattleFrontier_Lounge7_EventScript_Swift
	case 4, BattleFrontier_Lounge7_EventScript_IcyWind
	case 5, BattleFrontier_Lounge7_EventScript_Endure
	case 6, BattleFrontier_Lounge7_EventScript_PsychUp
	case 7, BattleFrontier_Lounge7_EventScript_IcePunch
	case 8, BattleFrontier_Lounge7_EventScript_ThunderPunch
	case 9, BattleFrontier_Lounge7_EventScript_FirePunch
	case 10, BattleFrontier_Lounge7_EventScript_ExitTutorMoveSelect
	case MULTI_B_PRESSED, BattleFrontier_Lounge7_EventScript_ExitTutorMoveSelect
	end

BattleFrontier_Lounge7_EventScript_ChooseNewRightTutorMove::
	message BattleFrontier_Lounge7_Text_TeachWhichMove
	waitmessage
	setvar VAR_TEMP_FRONTIER_TUTOR_ID, 1
	setvar VAR_0x8004, SCROLL_MULTI_BF_MOVE_TUTOR_2
	setvar VAR_0x8006, 1
	special ShowScrollableMultichoice
	copyvar VAR_TEMP_FRONTIER_TUTOR_SELECTION, VAR_RESULT
	switch VAR_RESULT
	case 0, BattleFrontier_Lounge7_EventScript_DefenseCurl
	case 1, BattleFrontier_Lounge7_EventScript_Snore
	case 2, BattleFrontier_Lounge7_EventScript_MudSlap
	case 3, BattleFrontier_Lounge7_EventScript_Swift
	case 4, BattleFrontier_Lounge7_EventScript_IcyWind
	case 5, BattleFrontier_Lounge7_EventScript_Endure
	case 6, BattleFrontier_Lounge7_EventScript_PsychUp
	case 7, BattleFrontier_Lounge7_EventScript_IcePunch
	case 8, BattleFrontier_Lounge7_EventScript_ThunderPunch
	case 9, BattleFrontier_Lounge7_EventScript_FirePunch
	case 10, BattleFrontier_Lounge7_EventScript_ExitTutorMoveSelect
	case MULTI_B_PRESSED, BattleFrontier_Lounge7_EventScript_ExitTutorMoveSelect
	end

BattleFrontier_Lounge7_EventScript_DefenseCurl::
	setvar VAR_0x8008, 16
	setvar VAR_0x8005, MOVE_DEFENSE_CURL
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_Snore::
	setvar VAR_0x8008, 24
	setvar VAR_0x8005, MOVE_SNORE
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_MudSlap::
	setvar VAR_0x8008, 24
	setvar VAR_0x8005, MOVE_MUD_SLAP
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_Swift::
	setvar VAR_0x8008, 24
	setvar VAR_0x8005, MOVE_SWIFT
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_IcyWind::
	setvar VAR_0x8008, 24
	setvar VAR_0x8005, MOVE_ICY_WIND
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_Endure::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_ENDURE
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_PsychUp::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_PSYCH_UP
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_IcePunch::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_ICE_PUNCH
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_ThunderPunch::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_THUNDER_PUNCH
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_FirePunch::
	setvar VAR_0x8008, 48
	setvar VAR_0x8005, MOVE_FIRE_PUNCH
	goto BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection
	end

BattleFrontier_Lounge7_EventScript_ExitTutorMoveSelect::
	special CloseBattleFrontierTutorWindow
	special CloseBattlePointsWindow
	msgbox BattleFrontier_Lounge7_Text_YouDontWantTo, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge7_EventScript_CancelChooseMon::
	msgbox BattleFrontier_Lounge7_Text_YouDontWantTo, MSGBOX_DEFAULT
	release
	end

@ VAR_0x8008 is the price
@ VAR_TEMP_C is the scroll multichoice ID
@ VAR_TEMP_FRONTIER_TUTOR_SELECTION is the move selection
@ VAR_TEMP_FRONTIER_TUTOR_ID is which move tutor was spoken to
BattleFrontier_Lounge7_EventScript_ConfirmMoveSelection::
	special BufferBattleFrontierTutorMoveName
	buffernumberstring STR_VAR_2, VAR_0x8008
	msgbox BattleFrontier_Lounge7_Text_MoveWillBeXBattlePoints, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge7_EventScript_ChooseNewMove
	specialvar VAR_TEMP_1, GetFrontierBattlePoints
	goto_if_ge VAR_TEMP_1, VAR_0x8008, BattleFrontier_Lounge7_EventScript_TeachTutorMove
	msgbox BattleFrontier_Lounge7_Text_HaventGotEnoughPoints, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge7_EventScript_ChooseNewMove
	end


BattleFrontier_Lounge7_EventScript_TeachTutorMove::
	msgbox BattleFrontier_Lounge7_Text_TeachMoveToWhichMon, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	special CloseBattlePointsWindow
	special CloseBattleFrontierTutorWindow
	chooseboxmon SELECT_PC_MON_MOVE_TUTOR
	waitstate
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_Lounge7_EventScript_CancelChooseMon
	msgbox BattleFrontier_Lounge7_Text_IllTakeBattlePoints, MSGBOX_DEFAULT
	copyvar VAR_0x8004, VAR_0x8008
	special TakeFrontierBattlePoints
	release
	end

BattleFrontier_Lounge7_EventScript_ChooseNewMove::
	goto_if_eq VAR_TEMP_FRONTIER_TUTOR_ID, 0, BattleFrontier_Lounge7_EventScript_ChooseNewLeftTutorMove
	goto BattleFrontier_Lounge7_EventScript_ChooseNewRightTutorMove
	end

BattleFrontier_Lounge7_EventScript_Sailor::
	msgbox BattleFrontier_Lounge7_Text_ThinkLadiesDontGetAlong, MSGBOX_NPC
	end

BattleFrontier_Lounge7_EventScript_Gentleman::
	msgbox BattleFrontier_Lounge7_Text_LadiesWereStrongAndBeautiful, MSGBOX_NPC
	end

BattleFrontier_Lounge7_Text_LeftTutorIntro:
	.string "Buhahaha!\p"
	.string "A guardarmi adesso non si direbbe,\n"
	.string "ma un tempo ero un'Allenatrice tosta.\p"
	.string "Ero famosa come la Bellezza più forte\n"
	.string "in circolazione, te lo dico io!\p"
	.string "… … … … … …\p"
	.string "E adesso che c'è?\n"
	.string "Non mi credi, eh?\p"
	.string "Io non sono come quella sbruffona\n"
	.string "laggiù. Io il talento ce l'ho davvero!\p"
	.string "Lascia che te lo dimostri.\n"
	.string "Posso insegnare ai tuoi Pokémon mosse\l"
	.string "speciali e, al tempo stesso, carine.\p"
	.string "Ma le mie lezioni non sono gratis.\n"
	.string "Che ne dici di pagarmele con\l"
	.string "qualche Punto Lotta?$"

BattleFrontier_Lounge7_Text_LeftTutorWelcomeBack:
	.string "Buhahaha!\p"
	.string "Sei tornato per imparare mosse\n"
	.string "speciali e, al tempo stesso, carine?$"

BattleFrontier_Lounge7_Text_TeachWhichMove:
	.string "Bene, bene, guarda qui!\n"
	.string "Quale mossa devo insegnare?$"

BattleFrontier_Lounge7_Text_MoveWillBeXBattlePoints:
	.string "La mossa {STR_VAR_1}, dici?\n"
	.string "Costa {STR_VAR_2} Punti Lotta, va bene?$"

BattleFrontier_Lounge7_Text_TeachMoveToWhichMon:
	.string "Bene, bene. Ora scegli il Pokémon\n"
	.string "a cui devo insegnare la mossa.$"

BattleFrontier_Lounge7_Text_HaventGotEnoughPoints:
	.string "Ma come…\n"
	.string "Non hai abbastanza Punti Lotta!$"

BattleFrontier_Lounge7_Text_IllTakeBattlePoints:
	.string "Hai visto che talento, eh?\n"
	.string "Mi prendo i tuoi Punti Lotta, grazie!$"

BattleFrontier_Lounge7_Text_YouDontWantTo:
	.string "Come sarebbe?\n"
	.string "Hai cambiato idea…\p"
	.string "Se vuoi vedere di cosa sono capace,\n"
	.string "torna a trovarmi quando vuoi!$"

BattleFrontier_Lounge7_Text_RightTutorIntro:
	.string "Ihihihi!\p"
	.string "So che adesso è difficile crederci, ma\n"
	.string "un tempo ero un'Allenatrice fantastica.\p"
	.string "Ero famosa come la Nuotatrice più\n"
	.string "imbattibile in circolazione, sai?\p"
	.string "… … … … … …\p"
	.string "Cosa c'è che non va?\n"
	.string "Non mi credi, eh?\p"
	.string "Io non sono come quella buffona\n"
	.string "laggiù. Io ho esperienza da vendere!\p"
	.string "Posso dimostrartelo.\n"
	.string "Posso insegnare ai tuoi Pokémon mosse\l"
	.string "potenti e, al tempo stesso, graziose.\p"
	.string "Ma le mie lezioni non sono gratis.\n"
	.string "Che ne dici di pagarmele con\l"
	.string "qualche Punto Lotta?$"

BattleFrontier_Lounge7_Text_RightTutorWelcomeBack:
	.string "Ihihihi!\p"
	.string "Sei venuto a imparare mosse Pokémon\n"
	.string "potenti e, al tempo stesso, graziose?$"

BattleFrontier_Lounge7_Text_ThinkLadiesDontGetAlong:
	.string "Quelle signore si sparlano a vicenda,\n"
	.string "e verrebbe da pensare che non vadano\l"
	.string "per niente d'accordo.\p"
	.string "Ma se fosse davvero così, non starebbero\n"
	.string "sempre qui insieme, non credi?$"

BattleFrontier_Lounge7_Text_LadiesWereStrongAndBeautiful:
	.string "Quando ero un giovanotto alle prime armi,\n"
	.string "quelle signore erano forti e bellissime.\p"
	.string "Erano gli idoli di noi Allenatori.\p"
	.string "Ancora oggi, l'età non ha scalfito\n"
	.string "le loro capacità.\p"
	.string "Anzi, le loro mosse Pokémon sono\n"
	.string "diventate ancora più raffinate.\p"
	.string "Eppure… non so perché, ma non riesco\n"
	.string "a fare a meno di pensarlo…\p"
	.string "Il tempo sa essere così crudele…$"

BattleFrontier_Lounge7_Text_SoftboiledDesc::
	.string "Recupera fino a\n"
	.string "metà dei PS\n"
	.string "massimi.$"

BattleFrontier_Lounge7_Text_SeismicTossDesc::
	.string "Infligge danni\n"
	.string "pari al livello\n"
	.string "di chi la usa.$"

BattleFrontier_Lounge7_Text_DreamEaterDesc::
	.string "Assorbe metà dei\n"
	.string "danni inflitti a un\n"
	.string "nemico che dorme.$"

BattleFrontier_Lounge7_Text_MegaPunchDesc::
	.string "Un pugno tirato\n"
	.string "con una potenza\n"
	.string "incredibile.$"

BattleFrontier_Lounge7_Text_MegaKickDesc::
	.string "Un calcio\n"
	.string "estremamente\n"
	.string "potente.$"

BattleFrontier_Lounge7_Text_BodySlamDesc::
	.string "Una carica con\n"
	.string "tutto il corpo che\n"
	.string "può paralizzare.$"

BattleFrontier_Lounge7_Text_RockSlideDesc::
	.string "Scaglia grossi\n"
	.string "massi. Il nemico\n"
	.string "può tentennare.$"

BattleFrontier_Lounge7_Text_CounterDesc::
	.string "Restituisce i\n"
	.string "colpi fisici con\n"
	.string "forza doppia.$"

BattleFrontier_Lounge7_Text_ThunderWaveDesc::
	.string "Una scossa debole\n"
	.string "che paralizza\n"
	.string "il nemico.$"

BattleFrontier_Lounge7_Text_SwordsDanceDesc::
	.string "Danza guerriera\n"
	.string "che aumenta molto\n"
	.string "l'Attacco.$"

BattleFrontier_Lounge7_Text_DefenseCurlDesc::
	.string "Si raggomitola e\n"
	.string "alza la propria\n"
	.string "Difesa.$"

BattleFrontier_Lounge7_Text_SnoreDesc::
	.string "Attacco rumoroso\n"
	.string "usabile solo\n"
	.string "mentre dorme.$"

BattleFrontier_Lounge7_Text_MudSlapDesc::
	.string "Fango in faccia al\n"
	.string "nemico: ne riduce\n"
	.string "la precisione.$"

BattleFrontier_Lounge7_Text_SwiftDesc::
	.string "Raggi a forma di\n"
	.string "stella che non\n"
	.string "falliscono mai.$"

BattleFrontier_Lounge7_Text_IcyWindDesc::
	.string "Vento gelido che\n"
	.string "riduce la Velocità\n"
	.string "del nemico.$"

BattleFrontier_Lounge7_Text_EndureDesc::
	.string "Resiste a ogni\n"
	.string "colpo per 1 turno,\n"
	.string "restando a 1 PS.$"

BattleFrontier_Lounge7_Text_PsychUpDesc::
	.string "Copia le modifiche\n"
	.string "alle statistiche\n"
	.string "del nemico.$"

BattleFrontier_Lounge7_Text_IcePunchDesc::
	.string "Pugno di ghiaccio\n"
	.string "che può congelare\n"
	.string "il nemico.$"

BattleFrontier_Lounge7_Text_ThunderPunchDesc::
	.string "Pugno elettrico\n"
	.string "che può paraliz-\n"
	.string "zare il nemico.$"

BattleFrontier_Lounge7_Text_FirePunchDesc::
	.string "Pugno infuocato\n"
	.string "che può scottare\n"
	.string "il nemico.$"
