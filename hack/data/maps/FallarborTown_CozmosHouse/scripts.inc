FallarborTown_CozmosHouse_MapScripts::
	.byte 0

FallarborTown_CozmosHouse_EventScript_ProfCozmo::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_RETURN, FallarborTown_CozmosHouse_EventScript_GaveMeteorite
	checkitem ITEM_METEORITE
	goto_if_eq VAR_RESULT, TRUE, FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite
	msgbox FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite::
	call_if_unset FLAG_TEMP_2, FallarborTown_CozmosHouse_EventScript_NoticeMeteorite
	call_if_set FLAG_TEMP_2, FallarborTown_CozmosHouse_EventScript_AskForMeteorite
	goto_if_eq VAR_RESULT, NO, FallarborTown_CozmosHouse_EventScript_DeclineGiveMeteorite
	msgbox FallarborTown_CozmosHouse_Text_PleaseUseThisTM, MSGBOX_DEFAULT
	giveitem ITEM_TM_RETURN
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setvar VAR_0x8004, ITEM_METEORITE
	call Common_EventScript_PlayerHandedOverTheItem
	setflag FLAG_RECEIVED_TM_RETURN
	msgbox FallarborTown_CozmosHouse_Text_ReallyGoingToHelpMyResearch, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_NoticeMeteorite::
	msgbox FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow, MSGBOX_DEFAULT
	msgbox FallarborTown_CozmosHouse_Text_IsThatMeteoriteMayIHaveIt, MSGBOX_YESNO
	return

FallarborTown_CozmosHouse_EventScript_AskForMeteorite::
	msgbox FallarborTown_CozmosHouse_Text_MayIHaveMeteorite, MSGBOX_YESNO
	return

FallarborTown_CozmosHouse_EventScript_DeclineGiveMeteorite::
	setflag FLAG_TEMP_2
	msgbox FallarborTown_CozmosHouse_Text_CrushedWithDisappointment, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_GaveMeteorite::
	msgbox FallarborTown_CozmosHouse_Text_ReallyGoingToHelpMyResearch, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_CozmosWife::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_RETURN, FallarborTown_CozmosHouse_EventScript_CozmoIsHappy
	goto_if_set FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, FallarborTown_CozmosHouse_EventScript_CozmoIsSad
	msgbox FallarborTown_CozmosHouse_Text_CozmoWentToMeteorFalls, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_CozmoIsSad::
	msgbox FallarborTown_CozmosHouse_Text_FeelSorryForCozmo, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_CozmoIsHappy::
	msgbox FallarborTown_CozmosHouse_Text_CozmoIsSoHappy, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow:
	.string "Prof. Cozmo: Oh…\n"
	.string "Non avrei mai dovuto farmi\l"
	.string "abbindolare dal Team Alpha e dire\l"
	.string "loro dove trovare i Meteoriti…\p"
	.string "Il Meteorite delle Cascate Meteora…\n"
	.string "un frammento del Grande Varco…\l"
	.string "Ormai non sarà mai più mio…$"

FallarborTown_CozmosHouse_Text_IsThatMeteoriteMayIHaveIt:
	.string "Oh!\n"
	.string "Eh?\p"
	.string "Quell'oggetto…\p"
	.string "Non sarà mica…?\p"
	.string "È il Meteorite che il Team Alpha ha\n"
	.string "preso alle Cascate Meteora?\p"
	.string "Ti prego, potresti darmelo?\p"
	.string "Non te lo chiedo gratis.\n"
	.string "Che ne dici di questa MT in cambio?$"

FallarborTown_CozmosHouse_Text_PleaseUseThisTM:
	.string "Prof. Cozmo: Questa MT è il segno\n"
	.string "della mia gratitudine.\l"
	.string "Usala, ti prego!$"

FallarborTown_CozmosHouse_Text_ReallyGoingToHelpMyResearch:
	.string "Prof. Cozmo: Oh, non ci posso credere.\n"
	.string "È davvero, davvero fantastico!\p"
	.string "Qui, al sicuro, brillerà di nuovo.\n"
	.string "E aiuterà tantissimo le mie ricerche!$"

FallarborTown_CozmosHouse_Text_CrushedWithDisappointment:
	.string "Prof. Cozmo: Oh, ma…\n"
	.string "Che delusione tremenda…$"

FallarborTown_CozmosHouse_Text_MayIHaveMeteorite:
	.string "Prof. Cozmo: Ti prego, potresti\n"
	.string "darmi quel Meteorite?\p"
	.string "Non te lo chiedo gratis.\n"
	.string "Che ne dici di questa MT in cambio?$"

FallarborTown_CozmosHouse_Text_CozmoWentToMeteorFalls:
	.string "Il Prof. Cozmo è partito per le\n"
	.string "Cascate Meteora, sul Percorso 114,\l"
	.string "con certi uomini vestiti di rosso.\p"
	.string "Dicevano di essere del Team Alpha…\n"
	.string "Spero tanto che stia bene.$"

FallarborTown_CozmosHouse_Text_FeelSorryForCozmo:
	.string "Povero Prof. Cozmo…\n"
	.string "È così abbattuto… mi fa tanta pena.$"

FallarborTown_CozmosHouse_Text_CozmoIsSoHappy:
	.string "Guarda il Prof. Cozmo…\n"
	.string "È così felice! Non è adorabile?$"

