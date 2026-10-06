DewfordTown_House1_MapScripts::
	.byte 0

DewfordTown_House1_EventScript_Man::
	msgbox DewfordTown_House1_Text_LotToBeSaidForLivingOnIsland, MSGBOX_NPC
	end

DewfordTown_House1_EventScript_Woman::
	msgbox DewfordTown_House1_Text_LifeGoesSlowlyOnIsland, MSGBOX_NPC
	end

DewfordTown_House1_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox DewfordTown_House1_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

DewfordTown_House1_Text_LotToBeSaidForLivingOnIsland:
	.string "Vivere su un'isoletta come questa, in\n"
	.string "armonia con i Pokémon e la famiglia,\l"
	.string "ha davvero i suoi bei vantaggi.$"

DewfordTown_House1_Text_LifeGoesSlowlyOnIsland:
	.string "Ho lasciato la vivace Porto Selcepoli\n"
	.string "quando ho sposato mio marito, qui.\p"
	.string "Sull'isola la vita scorre lenta.\n"
	.string "Ma essere circondati da un mare così\l"
	.string "bello… anche questa è felicità.\p"
	.string "E poi, qui i Varchi si aprono di rado.\n"
	.string "Si dorme sonni tranquilli!$"

DewfordTown_House1_Text_Zigzagoon:
	.string "Zigzagoon: Guguuh!$"

