RustboroCity_House1_MapScripts::
	.byte 0

RustboroCity_House1_EventScript_Trader::
	lock
	faceplayer
	goto_if_set FLAG_RUSTBORO_NPC_TRADE_COMPLETED, RustboroCity_House1_EventScript_TradeCompleted
	ingame_trade INGAME_TRADE_SEEDOT, RustboroCity_House1_Text_IllTradeIfYouWant, RustboroCity_House1_EventScript_DeclineTrade, RustboroCity_House1_EventScript_NotRequestedMon, RustboroCity_House1_Text_PleaseBeGoodToMyPokemon
	setflag FLAG_RUSTBORO_NPC_TRADE_COMPLETED
	release
	end

RustboroCity_House1_EventScript_DeclineTrade::
	msgbox RustboroCity_House1_Text_YouDontWantToThatsOkay, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox RustboroCity_House1_Text_DoesntLookLikeMonToMe, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_TradeCompleted::
	msgbox RustboroCity_House1_Text_AnyPokemonCanBeCute, MSGBOX_DEFAULT
	release
	end

RustboroCity_House1_EventScript_Hiker::
	msgbox RustboroCity_House1_Text_AllSortsOfPlaces, MSGBOX_NPC
	end

RustboroCity_House1_Text_IllTradeIfYouWant:
	.string "Eh? Il mio Pokémon è carino?\n"
	.string "Certo, lo sapevo già.\p"
	.string "Ma se ci tieni davvero, sono\n"
	.string "disposto a scambiartelo.\p"
	.string "Ti do il mio {STR_VAR_2} in cambio\n"
	.string "di un {STR_VAR_1}, se ti va.$"

RustboroCity_House1_Text_PleaseBeGoodToMyPokemon:
	.string "Eheheh…\n"
	.string "Tratta bene il mio Pokémon, eh!$"

RustboroCity_House1_Text_DoesntLookLikeMonToMe:
	.string "Eh? Quello non mi sembra proprio\n"
	.string "un {STR_VAR_1}.$"

RustboroCity_House1_Text_YouDontWantToThatsOkay:
	.string "Oh, se non ti va, fa niente.\n"
	.string "Però il mio Pokémon è carino, eh…$"

RustboroCity_House1_Text_AnyPokemonCanBeCute:
	.string "Ogni Pokémon può essere carino, se\n"
	.string "lo cresci con cura e gentilezza.$"

RustboroCity_House1_Text_AllSortsOfPlaces:
	.string "In ogni posto ci sono Pokémon e\n"
	.string "persone di ogni tipo.\p"
	.string "Lo trovo affascinante, perciò giro\n"
	.string "per posti di ogni tipo.\p"
	.string "E con i Varchi, ogni giorno spunta\n"
	.string "qualche Pokémon nuovo!$"

