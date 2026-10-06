GraniteCave_1F_MapScripts::
	.byte 0

GraniteCave_1F_EventScript_Hiker::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_FLASH, GraniteCave_1F_EventScript_ReceivedFlash
	msgbox GraniteCave_1F_Text_GetsDarkAheadHereYouGo, MSGBOX_DEFAULT
	giveitem ITEM_HM_FLASH
	setflag FLAG_RECEIVED_HM_FLASH
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_EventScript_ReceivedFlash::
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_Text_GetsDarkAheadHereYouGo:
	.string "Ehi, tu.\n"
	.string "Più avanti è buio pesto.\l"
	.string "Esplorare sarà davvero dura.\p"
	.string "Quel tipo che è passato poco fa…\n"
	.string "Rocco, mi pare si chiamasse.\p"
	.string "Cercava una pietra “caduta da un altro\n"
	.string "mondo”, diceva. Che tipo strano.\p"
	.string "Lui sapeva usare Flash, quindi dovrebbe\n"
	.string "cavarsela, però…\p"
	.string "Beh, per noi Montanari aiutare chi si\n"
	.string "incontra per strada è un motto.\p"
	.string "Le salite sono dure, ma in compagnia\n"
	.string "pesano meno! Ecco, questa è per te.$"

GraniteCave_1F_Text_ExplainFlash:
	.string "Insegna la mossa MN Flash a un\n"
	.string "Pokémon e usala.\p"
	.string "Illumina perfino il buio più fitto\n"
	.string "delle grotte.\p"
	.string "Ma per usarla fuori dalla lotta ti\n"
	.string "serve la Medaglia della Palestra\l"
	.string "Pokémon di Bluruvia.$"

