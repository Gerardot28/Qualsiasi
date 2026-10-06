FortreeCity_House5_MapScripts::
	.byte 0

FortreeCity_House5_EventScript_PokefanF::
	msgbox FortreeCity_House5_Text_TreeHousesAreGreat, MSGBOX_NPC
	end

FortreeCity_House5_EventScript_Man::
	msgbox FortreeCity_House5_Text_AdaptedToNature, MSGBOX_NPC
	end

FortreeCity_House5_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox FortreeCity_House5_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FortreeCity_House5_Text_TreeHousesAreGreat:
	.string "Le case sugli alberi sono il massimo!\p"
	.string "Secondo me Forestopoli è la città\n"
	.string "migliore per vivere con i Pokémon.$"

FortreeCity_House5_Text_AdaptedToNature:
	.string "Pokémon e persone si sono adattati\n"
	.string "alla natura per sopravvivere.\p"
	.string "Non serve piegare la natura al nostro\n"
	.string "modo di vivere.\p"
	.string "Guarda i Pokémon arrivati dai Varchi:\n"
	.string "si sono adattati anche loro!$"

FortreeCity_House5_Text_Zigzagoon:
	.string "Zigzagoon: Bufuu!$"

