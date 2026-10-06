BattleFrontier_Lounge4_MapScripts::
	.byte 0

BattleFrontier_Lounge4_EventScript_Woman::
	msgbox BattleFrontier_Lounge4_Text_WonderIfInterviewsAiring, MSGBOX_NPC
	end

BattleFrontier_Lounge4_EventScript_Cook::
	msgbox BattleFrontier_Lounge4_Text_IfIOpenedRestaurantHere, MSGBOX_NPC
	end

BattleFrontier_Lounge4_EventScript_Man::
	msgbox BattleFrontier_Lounge4_Text_NeedBreatherAfterBattles, MSGBOX_NPC
	end

BattleFrontier_Lounge4_Text_WonderIfInterviewsAiring:
	.string "Chissà se oggi in TV trasmettono\n"
	.string "qualche intervista agli Allenatori forti…$"

BattleFrontier_Lounge4_Text_IfIOpenedRestaurantHere:
	.string "Se aprissi un ristorante qui,\n"
	.string "farei soldi a palate, garantito.\p"
	.string "E quando arriveranno gli Allenatori\n"
	.string "dal Grande Varco? Piatti di tutti\l"
	.string "i mondi! Ho già in mente il menù!$"

BattleFrontier_Lounge4_Text_NeedBreatherAfterBattles:
	.string "Uff…\p"
	.string "Dopo lotte così intense ho proprio\n"
	.string "bisogno di tirare il fiato…\p"
	.string "Ma anche adesso non smetto mai\n"
	.string "di studiare strategie e combinazioni.$"

