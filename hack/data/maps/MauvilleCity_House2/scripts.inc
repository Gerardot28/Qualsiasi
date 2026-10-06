MauvilleCity_House2_MapScripts::
	.byte 0

MauvilleCity_House2_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_COIN_CASE, MauvilleCity_House2_EventScript_ReceivedCoinCase
	msgbox MauvilleCity_House2_Text_BuyHarborMailAtSlateport, MSGBOX_DEFAULT
	checkitem ITEM_HARBOR_MAIL
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_House2_EventScript_AskToTradeForHarborMail
	release
	end

MauvilleCity_House2_EventScript_AskToTradeForHarborMail::
	playse SE_PIN
	applymovement VAR_LAST_TALKED, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_LAST_TALKED, Common_Movement_Delay48
	waitmovement 0
	msgbox MauvilleCity_House2_Text_TradeHarborMailForCoinCase, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_House2_EventScript_AcceptTrade
	goto_if_eq VAR_RESULT, NO, MauvilleCity_House2_EventScript_DeclineTrade
	end

MauvilleCity_House2_EventScript_AcceptTrade::
	msgbox MauvilleCity_House2_Text_IllTradeYouCoinCase, MSGBOX_DEFAULT
	giveitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	removeitem ITEM_HARBOR_MAIL
	setflag FLAG_RECEIVED_COIN_CASE
	goto MauvilleCity_House2_EventScript_ReceivedCoinCase
	end

MauvilleCity_House2_EventScript_ReceivedCoinCase::
	msgbox MauvilleCity_House2_Text_UseCoinCaseAtGameCorner, MSGBOX_DEFAULT
	release
	end

MauvilleCity_House2_EventScript_DeclineTrade::
	msgbox MauvilleCity_House2_Text_ThatsDisappointing, MSGBOX_DEFAULT
	release
	end

MauvilleCity_House2_Text_BuyHarborMailAtSlateport:
	.string "Se avessi una Bici, farei un salto\n"
	.string "a Porto Selcepoli a fare spese.\p"
	.string "Potrei comprare un Mess. Porto al\n"
	.string "Pokémon Market di Porto Selcepoli…$"

MauvilleCity_House2_Text_TradeHarborMailForCoinCase:
	.string "Oh! Hai un Mess. Porto?\n"
	.string "Me lo scambi con un Salvadanaio?$"

MauvilleCity_House2_Text_IllTradeYouCoinCase:
	.string "Oh, che gioia!\n"
	.string "Ecco, il Salvadanaio è tuo!$"

MauvilleCity_House2_Text_UseCoinCaseAtGameCorner:
	.string "Il Salvadanaio ti servirà per\n"
	.string "giocare al Casinò.$"

MauvilleCity_House2_Text_ThatsDisappointing:
	.string "Oh, che delusione…\p"
	.string "Per giocare al Casinò serve\n"
	.string "un Salvadanaio, sai?$"

