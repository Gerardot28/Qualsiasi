AbandonedShip_CaptainsOffice_MapScripts::
	.byte 0

AbandonedShip_CaptainsOffice_EventScript_CaptSternAide::
	lock
	faceplayer
	goto_if_set FLAG_EXCHANGED_SCANNER, AbandonedShip_CaptainsOffice_EventScript_ThisIsSSCactus
	checkitem ITEM_SCANNER
	goto_if_eq VAR_RESULT, TRUE, AbandonedShip_CaptainsOffice_EventScript_CanYouDeliverScanner
	goto_if_set FLAG_ITEM_ABANDONED_SHIP_HIDDEN_FLOOR_ROOM_2_SCANNER, AbandonedShip_CaptainsOffice_EventScript_ThisIsSSCactus
	msgbox AbandonedShip_CaptainsOffice_Text_NoSuccessFindingScanner, MSGBOX_DEFAULT
	release
	end

AbandonedShip_CaptainsOffice_EventScript_CanYouDeliverScanner::
	msgbox AbandonedShip_CaptainsOffice_Text_OhCanYouDeliverScanner, MSGBOX_DEFAULT
	release
	end

AbandonedShip_CaptainsOffice_EventScript_ThisIsSSCactus::
	msgbox AbandonedShip_CaptainsOffice_Text_ThisIsSSCactus, MSGBOX_DEFAULT
	release
	end

AbandonedShip_CaptainsOffice_Text_NoSuccessFindingScanner:
	.string "Sto ispezionando questa nave per\n"
	.string "conto di Capitan Stern.\p"
	.string "Mi ha chiesto anche di ritrovare uno\n"
	.string "Scanner, un apparecchio che registra\l"
	.string "le onde dei Varchi marini…\p"
	.string "Ma finora non ho avuto fortuna…$"

AbandonedShip_CaptainsOffice_Text_OhCanYouDeliverScanner:
	.string "Oh! Ma quello è lo Scanner!\p"
	.string "Senti, potresti portarlo tu a\n"
	.string "Capitan Stern, a Porto Selcepoli?\p"
	.string "Io vorrei ispezionare questa nave\n"
	.string "ancora un po'.$"

AbandonedShip_CaptainsOffice_Text_ThisIsSSCactus:
	.string "Questa nave si chiama M/N Cactus.\n"
	.string "Sembra venire da un'altra epoca.$"

