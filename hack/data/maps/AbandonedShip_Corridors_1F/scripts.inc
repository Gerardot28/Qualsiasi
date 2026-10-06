AbandonedShip_Corridors_1F_MapScripts::
	.byte 0

AbandonedShip_Corridors_1F_EventScript_Youngster::
	msgbox AbandonedShip_Corridors_1F_Text_IsntItFunHere, MSGBOX_NPC
	end

AbandonedShip_Corridors_1F_EventScript_Charlie::
	trainerbattle_single TRAINER_CHARLIE, AbandonedShip_Corridors_1F_Text_CharlieIntro, AbandonedShip_Corridors_1F_Text_CharlieDefeat
	msgbox AbandonedShip_Corridors_1F_Text_CharliePostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Corridors_1F_Text_CharlieIntro:
	.string "Che c'è da ridere? Non si può tenere\n"
	.string "il salvagente a bordo di una nave?$"

AbandonedShip_Corridors_1F_Text_CharlieDefeat:
	.string "Uaah, mi hai travolto!$"

AbandonedShip_Corridors_1F_Text_CharliePostBattle:
	.string "Non è facile lanciare Poké Ball mentre\n"
	.string "ti tieni stretto al salvagente!$"

AbandonedShip_Corridors_1F_Text_IsntItFunHere:
	.string "Che forte questo posto, eh?\n"
	.string "Mi emoziono solo a starci dentro!$"
