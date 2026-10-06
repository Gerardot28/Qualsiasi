SlateportCity_NameRatersHouse_MapScripts::
	.byte 0

SlateportCity_NameRatersHouse_EventScript_NameRater::
	lock
	faceplayer
	msgbox SlateportCity_NameRatersHouse_Text_PleasedToRateMonNickname, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_NameRatersHouse_EventScript_ChooseMonToRate
	goto_if_eq VAR_RESULT, NO, SlateportCity_NameRatersHouse_EventScript_DeclineNameRate
	end

SlateportCity_NameRatersHouse_EventScript_ChooseMonToRate::
	msgbox SlateportCity_NameRatersHouse_Text_CritiqueWhichMonNickname, MSGBOX_DEFAULT
	chooseboxmon
	waitstate
	goto_if_ne VAR_0x8004, PARTY_NOTHING_CHOSEN, SlateportCity_NameRatersHouse_EventScript_RateMonNickname
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, SlateportCity_NameRatersHouse_EventScript_DeclineNameRate
	end

SlateportCity_NameRatersHouse_EventScript_DeclineNameRate::
	msgbox SlateportCity_NameRatersHouse_Text_DoVisitAgain, MSGBOX_DEFAULT
	release
	end

SlateportCity_NameRatersHouse_EventScript_RateMonNickname::
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, SlateportCity_NameRatersHouse_EventScript_CantRateEgg
	special BufferMonNickname
	special IsMonOTIDNotPlayers
	goto_if_eq VAR_RESULT, TRUE, SlateportCity_NameRatersHouse_EventScript_PlayerNotMonsOT
	specialvar VAR_RESULT, MonOTNameNotPlayer
	special BufferMonNickname
	goto_if_eq VAR_RESULT, TRUE, SlateportCity_NameRatersHouse_EventScript_PlayerNotMonsOT
	msgbox SlateportCity_NameRatersHouse_Text_FineNameSuggestBetterOne, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_NameRatersHouse_EventScript_ChangeNickname
	goto_if_eq VAR_RESULT, NO, SlateportCity_NameRatersHouse_EventScript_DeclineNameRate
	end

SlateportCity_NameRatersHouse_EventScript_CantRateEgg::
	msgbox SlateportCity_NameRatersHouse_Text_ThatIsMerelyAnEgg, MSGBOX_DEFAULT
	release
	end

SlateportCity_NameRatersHouse_EventScript_PlayerNotMonsOT::
	msgbox SlateportCity_NameRatersHouse_Text_MagnificentName, MSGBOX_DEFAULT
	release
	end

SlateportCity_NameRatersHouse_EventScript_ChangeNickname::
	msgbox SlateportCity_NameRatersHouse_Text_WhatShallNewNameBe, MSGBOX_DEFAULT
	call Common_EventScript_NameReceivedPartyMon
	specialvar VAR_RESULT, TryPutNameRaterShowOnTheAir
	special BufferMonNickname
	goto_if_eq VAR_RESULT, TRUE, SlateportCity_NameRatersHouse_EventScript_NewNameDifferent
	msgbox SlateportCity_NameRatersHouse_Text_NameNoDifferentYetSuperior, MSGBOX_DEFAULT
	release
	end

SlateportCity_NameRatersHouse_EventScript_NewNameDifferent::
	msgbox SlateportCity_NameRatersHouse_Text_MonShallBeKnownAsName, MSGBOX_DEFAULT
	release
	end

SlateportCity_NameRatersHouse_Text_PleasedToRateMonNickname:
	.string "Ciao, ciao! Sono il Giudice dei Nomi!\n"
	.string "Leggo il destino nei nomi!\p"
	.string "Che vengano da Hoenn o da un Varco,\n"
	.string "i Pokémon meritano un gran nome!\p"
	.string "Sarò lieto di valutare il soprannome\n"
	.string "del tuo Pokémon.$"

SlateportCity_NameRatersHouse_Text_CritiqueWhichMonNickname:
	.string "Di quale Pokémon devo giudicare\n"
	.string "il soprannome?$"

SlateportCity_NameRatersHouse_Text_FineNameSuggestBetterOne:
	.string "Mmm… {STR_VAR_1}, dici? Davvero\n"
	.string "un bel nome, non c'è che dire.\p"
	.string "Ma! Che ne diresti se te ne\n"
	.string "suggerissi uno un tantino migliore?$"

SlateportCity_NameRatersHouse_Text_WhatShallNewNameBe:
	.string "Ah, bene. Allora, quale sarà\n"
	.string "il nuovo soprannome?$"

SlateportCity_NameRatersHouse_Text_MonShallBeKnownAsName:
	.string "Fatto! D'ora in poi questo Pokémon\n"
	.string "si chiamerà {STR_VAR_1}!\p"
	.string "È un nome migliore di prima!\n"
	.string "Che fortuna sfacciata!$"

SlateportCity_NameRatersHouse_Text_DoVisitAgain:
	.string "Capisco.\n"
	.string "Torna a trovarmi quando vuoi.$"

SlateportCity_NameRatersHouse_Text_NameNoDifferentYetSuperior:
	.string "Fatto! D'ora in poi questo Pokémon\n"
	.string "si chiamerà {STR_VAR_1}!\p"
	.string "Sembra identico a prima, eppure\n"
	.string "è infinitamente superiore!\p"
	.string "Che fortuna sfacciata!$"

SlateportCity_NameRatersHouse_Text_MagnificentName:
	.string "Mmm… {STR_VAR_1}, eh!\p"
	.string "Questo sì che è un soprannome magnifico!\n"
	.string "Impeccabile, perfetto, inattaccabile!\p"
	.string "Abbi cura del tuo {STR_VAR_1},\n"
	.string "oggi e per sempre.$"

SlateportCity_NameRatersHouse_Text_ThatIsMerelyAnEgg:
	.string "Su, su.\n"
	.string "Quello è soltanto un Uovo!$"

