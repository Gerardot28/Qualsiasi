Route114_FossilManiacsHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route114_FossilManiacsHouse_OnTransition
	.byte 0

Route114_FossilManiacsHouse_OnTransition:
	setflag FLAG_LANDMARK_FOSSIL_MANIACS_HOUSE
	end

Route114_FossilManiacsHouse_EventScript_FossilManiacsBrother::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_DIG, Route114_FossilManiacsHouse_EventScript_ReceivedDig
	msgbox Route114_FossilManiacsHouse_Text_HaveThisToDigLikeMyBrother, MSGBOX_DEFAULT
	giveitem ITEM_TM_DIG
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_DIG
	release
	end

Route114_FossilManiacsHouse_EventScript_ReceivedDig::
	msgbox Route114_FossilManiacsHouse_Text_DigReturnsYouToEntrance, MSGBOX_DEFAULT
	release
	end

Route114_FossilManiacsHouse_EventScript_RockDisplay::
	msgbox Route114_FossilManiacsHouse_Text_RocksFillDisplayCase, MSGBOX_SIGN
	end

Route114_FossilManiacsHouse_EventScript_Bookshelf::
	msgbox Route114_FossilManiacsHouse_Text_CrammedWithBooks, MSGBOX_SIGN
	end

Route114_FossilManiacsHouse_Text_HaveThisToDigLikeMyBrother:
	.string "Il mio fratellone è l'Esperto Fossili…\n"
	.string "È un tipo buono, adora i fossili…\p"
	.string "E adora anche scavare buche…\n"
	.string "Questa l'ha scavata tutta da solo…\p"
	.string "Tieni, prendi questa. Così potrai\n"
	.string "usare Fossa proprio come lui…$"

Route114_FossilManiacsHouse_Text_DigReturnsYouToEntrance:
	.string "Se un Pokémon usa Fossa in una\n"
	.string "grotta, torni subito all'ingresso…$"

Route114_FossilManiacsHouse_Text_RocksFillDisplayCase:
	.string "La vetrina è piena di rocce dalle\n"
	.string "forme bizzarre…\p"
	.string "Su un cartellino c'è scritto:\n"
	.string "“Caduta da un Varco?”$"

Route114_FossilManiacsHouse_Text_CrammedWithBooks:
	.string "“La composizione degli strati”…\n"
	.string "“Come la pioggia modella la terra”…\l"
	.string "“Pietre, suoli e rocce”…\p"
	.string "Lo scaffale è pieno zeppo di libri.$"

