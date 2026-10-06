SootopolisCity_House4_MapScripts::
	.byte 0

SootopolisCity_House4_EventScript_Man::
	msgbox SootopolisCity_House4_Text_AncientTreasuresWaitingInSea, MSGBOX_NPC
	end

SootopolisCity_House4_EventScript_Woman::
	msgbox SootopolisCity_House4_Text_StrollUnderwaterWithPokemon, MSGBOX_NPC
	end

SootopolisCity_House4_EventScript_Azumarill::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_AZUMARILL, CRY_MODE_NORMAL
	msgbox SootopolisCity_House4_Text_Azumarill, MSGBOX_DEFAULT
	waitmoncry
	release
	end

SootopolisCity_House4_Text_AncientTreasuresWaitingInSea:
	.string "Ascolta bene, che ti racconto una\n"
	.string "cosa interessante.\p"
	.string "Pare che nel mare qui intorno ci sia\n"
	.string "un'antica rovina sommersa.\p"
	.string "Laggiù potrebbero esserci tesori\n"
	.string "che aspettano solo di essere trovati.$"

SootopolisCity_House4_Text_StrollUnderwaterWithPokemon:
	.string "Tesori antichi…\p"
	.string "Sarebbe bello se esistessero, ma anche\n"
	.string "se non ci fossero, sarebbe comunque\l"
	.string "stupendo fare una passeggiata\l"
	.string "sott'acqua con i miei Pokémon.$"

SootopolisCity_House4_Text_Azumarill:
	.string "Azumarill: Marurii.$"

