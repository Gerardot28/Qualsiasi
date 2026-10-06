MauvilleCity_House1_MapScripts::
	.byte 0

MauvilleCity_House1_EventScript_RockSmashDude::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_ROCK_SMASH, MauvilleCity_House1_EventScript_ReceivedRockSmash
	msgbox MauvilleCity_House1_Text_ImRockSmashDudeTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_HM_ROCK_SMASH
	setflag FLAG_RECEIVED_HM_ROCK_SMASH
	setflag FLAG_HIDE_ROUTE_111_ROCK_SMASH_TIP_GUY
	msgbox MauvilleCity_House1_Text_ExplainRockSmash, MSGBOX_DEFAULT
	release
	end

MauvilleCity_House1_EventScript_ReceivedRockSmash::
	msgbox MauvilleCity_House1_Text_MonCanFlyOutOfSmashedRock, MSGBOX_DEFAULT
	release
	end

MauvilleCity_House1_Text_ImRockSmashDudeTakeThis:
	.string "Iuhuuu!\p"
	.string "Ho sentito che mi chiamano “il tizio\n"
	.string "dello Spaccaroccia”. Che tristezza!\p"
	.string "Merito un po' più di rispetto, no?\n"
	.string "Tipo… “il Re dello Spaccaroccia”!\p"
	.string "Iuhuuu!\p"
	.string "Comunque, i tuoi Pokémon sembrano\n"
	.string "belli forti.\p"
	.string "Mi piace!\n"
	.string "Tieni, prendi questa Macchina Nascosta!$"

MauvilleCity_House1_Text_ExplainRockSmash:
	.string "Quella MN contiene Spaccaroccia.\p"
	.string "Se trovi dei grossi massi che ti\n"
	.string "sbarrano la strada…\p"
	.string "Beh, usa quella mossa MN e riducili\n"
	.string "in briciole!\p"
	.string "Sissignore! Via i sassi dai piedi!\n"
	.string "Iuhuuu!$"

MauvilleCity_House1_Text_MonCanFlyOutOfSmashedRock:
	.string "Ah, sì: se spacchi una roccia, potrebbe\n"
	.string "saltarne fuori un Pokémon nascosto.\p"
	.string "Magari persino uno arrivato da un\n"
	.string "Varco! Iuhuuu!$"

