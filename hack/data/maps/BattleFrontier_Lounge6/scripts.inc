BattleFrontier_Lounge6_MapScripts::
	.byte 0

BattleFrontier_Lounge6_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_BATTLE_FRONTIER_TRADE_DONE, BattleFrontier_Lounge6_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_MEOWTH, BattleFrontier_Lounge6_Text_WouldYouLikeToTrade, BattleFrontier_Lounge6_EventScript_DeclineTrade, BattleFrontier_Lounge6_EventScript_NotRequestedMon, BattleFrontier_Lounge6_Text_PromiseIllBeGoodToIt
	setflag FLAG_BATTLE_FRONTIER_TRADE_DONE
	release
	end

BattleFrontier_Lounge6_EventScript_DeclineTrade::
	msgbox BattleFrontier_Lounge6_Text_WellThatsFineToo, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox BattleFrontier_Lounge6_Text_DontTradeForAnythingButMon, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_EventScript_TradeCompleted::
	msgbox BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImagined, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge6_Text_WouldYouLikeToTrade:
	.string "Il mio Pokémon è {STR_VAR_2}.\n"
	.string "Lo conosci?\l"
	.string "È tenerissimo, e anche gentile.\p"
	.string "L'ho trovato vicino a un Varco, e\n"
	.string "lo scambierei con orgoglio!\p"
	.string "Mi daresti {STR_VAR_1}\n"
	.string "in cambio del mio {STR_VAR_2}?$"

BattleFrontier_Lounge6_Text_PromiseIllBeGoodToIt:
	.string "Oh, che amore!\n"
	.string "Grazie!\l"
	.string "Prometto che lo tratterò benissimo!\p"
	.string "E spero che anche tu tratterai bene\n"
	.string "il mio {STR_VAR_2}!$"

BattleFrontier_Lounge6_Text_DontTradeForAnythingButMon:
	.string "Oh, mi dispiace!\n"
	.string "Lo scambierei solo ed esclusivamente\l"
	.string "con {STR_VAR_1}.$"

BattleFrontier_Lounge6_Text_WellThatsFineToo:
	.string "Oh, non vuoi?\n"
	.string "Pazienza, va bene lo stesso.\l"
	.string "Torna a trovarci quando vuoi.$"

BattleFrontier_Lounge6_Text_SkittySoMuchCuterThanImagined:
	.string "Ihihi!\n"
	.string "Skitty è ancora più carino di quanto\l"
	.string "immaginassi. Sono felicissima!$"
