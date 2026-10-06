MossdeepCity_House3_MapScripts::
	.byte 0

MossdeepCity_House3_EventScript_SuperRodFisherman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_SUPER_ROD, MossdeepCity_House3_EventScript_ReceivedSuperRod
	msgbox MossdeepCity_House3_Text_YouWantSuperRod, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MossdeepCity_House3_EventScript_DeclineSuperRod
	msgbox MossdeepCity_House3_Text_SuperRodIsSuper, MSGBOX_DEFAULT
	giveitem ITEM_SUPER_ROD
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_SUPER_ROD
	msgbox MossdeepCity_House3_Text_TryDroppingRodInWater, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House3_EventScript_ReceivedSuperRod::
	msgbox MossdeepCity_House3_Text_GoAfterSeafloorPokemon, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House3_EventScript_DeclineSuperRod::
	msgbox MossdeepCity_House3_Text_DontYouLikeToFish, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House3_Text_YouWantSuperRod:
	.string "Ehilà, Allenatore!\n"
	.string "Il Superamo è davvero super!\p"
	.string "Di' quello che vuoi, ma questo gioiello\n"
	.string "pesca Pokémon dal fondo del mare!\p"
	.string "Allora, che ne dici?\n"
	.string "Lo vuoi, vero?$"

MossdeepCity_House3_Text_SuperRodIsSuper:
	.string "Lo sapevo, lo sapevo!\n"
	.string "Del resto, il Superamo è super davvero!$"

MossdeepCity_House3_Text_TryDroppingRodInWater:
	.string "Dove c'è acqua, prova a calare l'amo\n"
	.string "e guarda cosa abbocca!$"

MossdeepCity_House3_Text_DontYouLikeToFish:
	.string "Eh?\n"
	.string "Non ti piace pescare?$"

MossdeepCity_House3_Text_GoAfterSeafloorPokemon:
	.string "Va' a caccia dei Pokémon dei fondali\n"
	.string "con il tuo Superamo.\p"
	.string "Dicono che vicino ai Varchi in mare\n"
	.string "abbocchino pesci mai visti prima!$"

