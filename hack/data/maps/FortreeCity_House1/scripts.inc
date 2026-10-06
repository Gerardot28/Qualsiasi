FortreeCity_House1_MapScripts::
	.byte 0

FortreeCity_House1_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_FORTREE_NPC_TRADE_COMPLETED, FortreeCity_House1_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_PLUSLE, FortreeCity_House1_Text_YouWillTradeWontYou, FortreeCity_House1_EventScript_DeclineTrade, FortreeCity_House1_EventScript_NotRequestedMon, FortreeCity_House1_Text_MonYouTakeCare
	setflag FLAG_FORTREE_NPC_TRADE_COMPLETED
	release
	end

FortreeCity_House1_EventScript_DeclineTrade::
	msgbox FortreeCity_House1_Text_YouWontTradeMe, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox FortreeCity_House1_Text_ThisIsntAMon, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_TradeCompleted::
	msgbox FortreeCity_House1_Text_GoingToMakeVolbeatStrong, MSGBOX_DEFAULT
	release
	end

FortreeCity_House1_EventScript_ExpertF::
	msgbox FortreeCity_House1_Text_TradingMemoriesWithOthers, MSGBOX_NPC
	end

FortreeCity_House1_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox FortreeCity_House1_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FortreeCity_House1_Text_YouWillTradeWontYou:
	.string "Uoooaaargh! Mi serve!\n"
	.string "Devo avere un {STR_VAR_1}!\l"
	.string "Farei qualsiasi cosa!\p"
	.string "…Ehm… Hai sentito?\n"
	.string "Il grido che viene dal mio cuore?\p"
	.string "Dopo averlo sentito, mi darai il tuo\n"
	.string "{STR_VAR_1} per il mio {STR_VAR_2},\l"
	.string "vero?$"

FortreeCity_House1_Text_MonYouTakeCare:
	.string "Oh, sì, grandioso!\p"
	.string "{STR_VAR_1}, benvenuto!\n"
	.string "{STR_VAR_2}, abbi cura di te!$"

FortreeCity_House1_Text_ThisIsntAMon:
	.string "Ehm, no, non direi proprio.\n"
	.string "Quello non è un {STR_VAR_1}.$"

FortreeCity_House1_Text_YouWontTradeMe:
	.string "No? Non vuoi fare lo scambio?\n"
	.string "Dopo che ti ho aperto il mio cuore?$"

FortreeCity_House1_Text_GoingToMakeVolbeatStrong:
	.string "Da questo momento renderò il mio\n"
	.string "Volbeat fortissimo!\p"
	.string "Spero che tu faccia lo stesso\n"
	.string "con Plusle!$"

FortreeCity_House1_Text_TradingMemoriesWithOthers:
	.string "Scambiare Pokémon con gli altri…\p"
	.string "È come scambiarsi un pezzetto dei\n"
	.string "propri ricordi.$"

FortreeCity_House1_Text_Zigzagoon:
	.string "Zigzagoon: Gumomoh?$"

