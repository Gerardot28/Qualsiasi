MauvilleCity_BikeShop_MapScripts::
	.byte 0

MauvilleCity_BikeShop_EventScript_Rydel::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_BIKE, MauvilleCity_BikeShop_EventScript_AskSwitchBikes
	goto_if_set FLAG_DECLINED_BIKE, MauvilleCity_BikeShop_EventScript_SkipGreeting
	msgbox MauvilleCity_BikeShop_Text_RydelGreeting, MSGBOX_DEFAULT
	msgbox MauvilleCity_BikeShop_Text_DidYouComeFromFarAway, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_YesFar
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_NotFar
	end

MauvilleCity_BikeShop_EventScript_SkipGreeting::
	msgbox MauvilleCity_BikeShop_Text_DidYouComeFromFarAway, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_YesFar
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_NotFar
	end

MauvilleCity_BikeShop_EventScript_ChooseBike::
	message MauvilleCity_BikeShop_Text_ExplainBikesChooseWhichOne
	waitmessage
	multichoice 21, 8, MULTI_BIKE, TRUE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_GetMachBike
	case 1, MauvilleCity_BikeShop_EventScript_GetAcroBike
	end

MauvilleCity_BikeShop_EventScript_NotFar::
	setflag FLAG_DECLINED_BIKE
	msgbox MauvilleCity_BikeShop_Text_GuessYouDontNeedBike, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_YesFar::
	setflag FLAG_RECEIVED_BIKE
	goto MauvilleCity_BikeShop_EventScript_ChooseBike
	end

MauvilleCity_BikeShop_EventScript_GetMachBike::
	msgbox MauvilleCity_BikeShop_Text_ChoseMachBike, MSGBOX_DEFAULT
	giveitem ITEM_MACH_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_GetAcroBike::
	msgbox MauvilleCity_BikeShop_Text_ChoseAcroBike, MSGBOX_DEFAULT
	giveitem ITEM_ACRO_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_ComeBackToSwitchBikes, MSGBOX_DEFAULT
	special SwapRegisteredBike
	release
	end

MauvilleCity_BikeShop_EventScript_AskSwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_WantToSwitchBikes, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_BikeShop_EventScript_SwitchBikes
	goto_if_eq VAR_RESULT, NO, MauvilleCity_BikeShop_EventScript_KeepBike
	end

@ If the player does not have a bike on them Rydel assumes its stored in the PC
MauvilleCity_BikeShop_EventScript_SwitchBikes::
	msgbox MauvilleCity_BikeShop_Text_IllSwitchBikes, MSGBOX_DEFAULT
	checkitem ITEM_ACRO_BIKE
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_BikeShop_EventScript_SwitchAcroForMach
	checkitem ITEM_MACH_BIKE
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_BikeShop_EventScript_SwitchMachForAcro
	msgbox MauvilleCity_BikeShop_Text_OhYourBikeIsInPC, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_KeepBike::
	msgbox MauvilleCity_BikeShop_Text_HappyYouLikeIt, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_EventScript_SwitchAcroForMach::
	incrementgamestat GAME_STAT_TRADED_BIKES
	msgbox MauvilleCity_BikeShop_Text_ExchangedAcroForMach, MSGBOX_DEFAULT
	removeitem ITEM_ACRO_BIKE
	giveitem ITEM_MACH_BIKE
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_SwitchMachForAcro::
	incrementgamestat GAME_STAT_TRADED_BIKES
	msgbox MauvilleCity_BikeShop_Text_ExchangedMachForAcro, MSGBOX_DEFAULT
	removeitem ITEM_MACH_BIKE
	giveitem ITEM_ACRO_BIKE
	goto MauvilleCity_BikeShop_EventScript_ComeBackToSwitchBikes
	end

MauvilleCity_BikeShop_EventScript_Assistant::
	msgbox MauvilleCity_BikeShop_Text_HandbooksAreInBack, MSGBOX_NPC
	end

MauvilleCity_BikeShop_EventScript_MachBikeHandbook::
	message MauvilleCity_BikeShop_Text_MachHandbookWhichPage
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage::
	multichoice 0, 0, MULTI_MACH_BIKE_INFO, FALSE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_HowToRide
	case 1, MauvilleCity_BikeShop_EventScript_HowToTurn
	case 2, MauvilleCity_BikeShop_EventScript_SandySlopes
	case 3, MauvilleCity_BikeShop_EventScript_ExitMachHandbook
	case MULTI_B_PRESSED, MauvilleCity_BikeShop_EventScript_ExitMachHandbook
	end

MauvilleCity_BikeShop_EventScript_HowToRide::
	message MauvilleCity_BikeShop_Text_HowToRideMachBike
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_HowToTurn::
	message MauvilleCity_BikeShop_Text_HowToTurnMachBike
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_SandySlopes::
	message MauvilleCity_BikeShop_Text_SandySlopes
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseMachHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ExitMachHandbook::
	release
	end

MauvilleCity_BikeShop_EventScript_AcroBikeHandbook::
	message MauvilleCity_BikeShop_Text_AcroHandbookWhichPage
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage::
	multichoice 0, 0, MULTI_ACRO_BIKE_INFO, FALSE
	switch VAR_RESULT
	case 0, MauvilleCity_BikeShop_EventScript_Wheelies
	case 1, MauvilleCity_BikeShop_EventScript_BunnyHops
	case 2, MauvilleCity_BikeShop_EventScript_Jumps
	case 3, MauvilleCity_BikeShop_EventScript_ExitAcroHandbook
	case MULTI_B_PRESSED, MauvilleCity_BikeShop_EventScript_ExitAcroHandbook
	end

MauvilleCity_BikeShop_EventScript_Wheelies::
	message MauvilleCity_BikeShop_Text_Wheelies
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_BunnyHops::
	message MauvilleCity_BikeShop_Text_BunnyHops
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_Jumps::
	message MauvilleCity_BikeShop_Text_Jumps
	waitmessage
	goto MauvilleCity_BikeShop_EventScript_ChooseAcroHandbookPage
	end

MauvilleCity_BikeShop_EventScript_ExitAcroHandbook::
	release
	end

MauvilleCity_BikeShop_Text_RydelGreeting:
	.string "Bene, bene, chi abbiamo qui?\n"
	.string "Un cliente pieno di energia!\p"
	.string "Io? Puoi chiamarmi Rydel.\n"
	.string "Questo negozio di bici è mio!$"

MauvilleCity_BikeShop_Text_DidYouComeFromFarAway:
	.string "Rydel: Le tue Scarpe da Corsa…\n"
	.string "Sono tutte incrostate di polvere!\p"
	.string "Vieni da molto lontano?$"

MauvilleCity_BikeShop_Text_GuessYouDontNeedBike:
	.string "Rydel: Ah, sì?\p"
	.string "Allora immagino che le mie Bici\n"
	.string "non ti servano proprio.$"

MauvilleCity_BikeShop_Text_ExplainBikesChooseWhichOne:
	.string "Rydel: Mm, mm… … … … …\p"
	.string "Mi stai dicendo che sei arrivato\n"
	.string "fin qui da Albanova?\p"
	.string "Santo cielo!\n"
	.string "Ma è lontanissimo!\p"
	.string "Con una delle mie Bici potresti\n"
	.string "andare ovunque in un lampo, sentendo\l"
	.string "la dolce carezza del vento!\p"
	.string "Sai che ti dico?\n"
	.string "Ti regalo una Bici!\p"
	.string "Oh, aspetta un attimo!\p"
	.string "Dimenticavo di dirti che esistono\n"
	.string "due tipi di Bici!\p"
	.string "La Bici da Corsa e\n"
	.string "la Bici da Cross!\p"
	.string "La Bici da Corsa è per chi vuole\n"
	.string "sentire il vento in faccia!\p"
	.string "La Bici da Cross, invece, è per chi\n"
	.string "ama le acrobazie e la tecnica!\p"
	.string "Sono un tenerone, quindi puoi\n"
	.string "prendere quella che preferisci!\p"
	.string "Quale scegli?$"

MauvilleCity_BikeShop_Text_ChoseMachBike:
	.string "{PLAYER} ha scelto la Bici da Corsa.$"

MauvilleCity_BikeShop_Text_ChoseAcroBike:
	.string "{PLAYER} ha scelto la Bici da Cross.$"

MauvilleCity_BikeShop_Text_ComeBackToSwitchBikes:
	.string "Rydel: Se ti viene voglia di cambiare\n"
	.string "Bici, passa pure a trovarmi!$"

MauvilleCity_BikeShop_Text_WantToSwitchBikes:
	.string "Rydel: Oh? Stai pensando di\n"
	.string "cambiare Bici?$"

MauvilleCity_BikeShop_Text_IllSwitchBikes:
	.string "Rydel: Va bene, nessun problema!\n"
	.string "Te la cambio subito!$"

MauvilleCity_BikeShop_Text_ExchangedMachForAcro:
	.string "{PLAYER} ha scambiato la Bici da\n"
	.string "Corsa con una Bici da Cross.$"

MauvilleCity_BikeShop_Text_ExchangedAcroForMach:
	.string "{PLAYER} ha scambiato la Bici da\n"
	.string "Cross con una Bici da Corsa.$"

MauvilleCity_BikeShop_Text_HappyYouLikeIt:
	.string "Rydel: Bene, bene!\n"
	.string "Sono felice che ti piaccia!$"

MauvilleCity_BikeShop_Text_OhYourBikeIsInPC:
	.string "Oh? Che fine ha fatto la Bici\n"
	.string "che ti ho regalato?\p"
	.string "Ah, ho capito: l'hai lasciata nel PC.\p"
	.string "Allora ritirala dal PC e sarò\n"
	.string "felice di scambiartela!\p"
	.string "Che il vento ti soffi sempre alle\n"
	.string "spalle, durante il tuo viaggio!$"

MauvilleCity_BikeShop_Text_HandbooksAreInBack:
	.string "Lavoro qui e intanto imparo un sacco\n"
	.string "di cose sulle Bici.\p"
	.string "Se ti servono consigli su come usare\n"
	.string "la tua Bici, sul retro ci sono un paio\l"
	.string "di manuali.$"

MauvilleCity_BikeShop_Text_MachHandbookWhichPage:
	.string "È un manuale sulla Bici da Corsa.\p"
	.string "Quale pagina vuoi leggere?$"

MauvilleCity_BikeShop_Text_HowToRideMachBike:
	.string "La Bici va nella direzione in cui\n"
	.string "premi la Pulsantiera +.\p"
	.string "Una volta lanciata, prende velocità.\p"
	.string "Per fermarti, rilascia la Pulsantiera +.\n"
	.string "La Bici rallenterà fino a fermarsi.\p"
	.string "Vuoi leggere un'altra pagina?$"

MauvilleCity_BikeShop_Text_HowToTurnMachBike:
	.string "La Bici da Corsa è velocissima, ma\n"
	.string "non riesce a frenare di colpo.\p"
	.string "Prendere le curve può essere\n"
	.string "un po' complicato.\p"
	.string "Rilascia la Pulsantiera + poco prima\n"
	.string "della curva e rallenta.\p"
	.string "Vuoi leggere un'altra pagina?$"

MauvilleCity_BikeShop_Text_SandySlopes:
	.string "In tutta la regione di Hoenn ci sono\n"
	.string "piccoli pendii sabbiosi.\p"
	.string "La sabbia è così friabile che è\n"
	.string "impossibile risalirli a piedi.\p"
	.string "Ma con una Bici da Corsa puoi\n"
	.string "sfrecciare fino in cima!\p"
	.string "Vuoi leggere un'altra pagina?$"

MauvilleCity_BikeShop_Text_AcroHandbookWhichPage:
	.string "È un manuale sulla Bici da Cross.\p"
	.string "Quale pagina vuoi leggere?$"

MauvilleCity_BikeShop_Text_Wheelies:
	.string "Premi il Pulsante B mentre pedali\n"
	.string "e la ruota anteriore si solleva.\p"
	.string "Puoi sfrecciare con la ruota alzata\n"
	.string "usando la Pulsantiera +.\p"
	.string "Questa tecnica si chiama impennata.\p"
	.string "Vuoi leggere un'altra pagina?$"

MauvilleCity_BikeShop_Text_BunnyHops:
	.string "Tenendo premuto il Pulsante B,\n"
	.string "la Bici saltella sul posto.\p"
	.string "Questa tecnica si chiama saltello.\p"
	.string "Puoi anche avanzare saltellando.\p"
	.string "Vuoi leggere un'altra pagina?$"

MauvilleCity_BikeShop_Text_Jumps:
	.string "Premi insieme il Pulsante B e la\n"
	.string "Pulsantiera + per saltare.\p"
	.string "Premi la Pulsantiera + di lato\n"
	.string "per saltare lateralmente.\p"
	.string "Premila all'indietro per far girare\n"
	.string "la Bici mentre salti.\p"
	.string "Vuoi leggere un'altra pagina?$"

