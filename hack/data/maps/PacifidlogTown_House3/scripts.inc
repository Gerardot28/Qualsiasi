PacifidlogTown_House3_MapScripts::
	.byte 0

PacifidlogTown_House3_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_PACIFIDLOG_NPC_TRADE_COMPLETED, PacifidlogTown_House3_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_HORSEA, PacifidlogTown_House3_Text_WillingToTradeIt, PacifidlogTown_House3_EventScript_DeclineTrade, PacifidlogTown_House3_EventScript_NotRequestedMon, PacifidlogTown_House3_Text_ItsSubtlyDifferentThankYou
	setflag FLAG_PACIFIDLOG_NPC_TRADE_COMPLETED
	release
	end

PacifidlogTown_House3_EventScript_DeclineTrade::
	msgbox PacifidlogTown_House3_Text_NotDesperateOrAnything, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox PacifidlogTown_House3_Text_WontAcceptAnyLessThanRealMon, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_TradeCompleted::
	msgbox PacifidlogTown_House3_Text_ReallyWantedToGetBagon, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House3_EventScript_Girl::
	msgbox PacifidlogTown_House3_Text_IsThatAPokedex, MSGBOX_NPC
	end

PacifidlogTown_House3_Text_WillingToTradeIt:
	.string "Guarda un po' questo {STR_VAR_2}!\p"
	.string "L'ho catturato ieri, tutto da solo,\n"
	.string "per festeggiare il mio compleanno!\p"
	.string "Oh, te lo leggo negli occhi: lo vuoi!\n"
	.string "Del resto, è un pezzo unico!\p"
	.string "Facciamo così: potrei anche\n"
	.string "scambiarlo con un {STR_VAR_1}.$"

PacifidlogTown_House3_Text_ItsSubtlyDifferentThankYou:
	.string "Oh, quindi questo è un {STR_VAR_1}?\p"
	.string "Somiglia un po' a un {STR_VAR_2},\n"
	.string "eppure ha qualcosa di diverso.\p"
	.string "Grazie mille!$"

PacifidlogTown_House3_Text_WontAcceptAnyLessThanRealMon:
	.string "No, no, no! Non accetto niente\n"
	.string "di meno di un vero {STR_VAR_1}!$"

PacifidlogTown_House3_Text_NotDesperateOrAnything:
	.string "Ah, quindi ti tiri indietro?\p"
	.string "Tranquillo, non è che muoia dalla\n"
	.string "voglia di fare uno scambio, eh.$"

PacifidlogTown_House3_Text_ReallyWantedToGetBagon:
	.string "Lo so, potrei anche andare a cercarne\n"
	.string "uno da solo, ma…\p"
	.string "Ci tenevo tanto ad avere un Bagon\n"
	.string "catturato da un altro Allenatore…$"

PacifidlogTown_House3_Text_IsThatAPokedex:
	.string "Ma quello è un Pokédex?\p"
	.string "Hai visto tanti Pokémon diversi?\n"
	.string "Anche quelli arrivati dai Varchi?\p"
	.string "Vorrei tanto essere come te!$"

