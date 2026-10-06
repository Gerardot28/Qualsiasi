LilycoveCity_House1_MapScripts::
	.byte 0

LilycoveCity_House1_EventScript_ExpertM::
	msgbox LilycoveCity_House1_Text_PokemonPartnersNotTools, MSGBOX_NPC
	end

LilycoveCity_House1_EventScript_Kecleon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_KECLEON, CRY_MODE_NORMAL
	msgbox LilycoveCity_House1_Text_Kecleon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

LilycoveCity_House1_Text_PokemonPartnersNotTools:
	.string "I Pokémon sono compagni delle persone,\n"
	.string "non attrezzi da usare.\p"
	.string "Purtroppo c'è ancora chi non riesce\n"
	.string "a capirlo…$"

LilycoveCity_House1_Text_Kecleon:
	.string "Kecleon: Ruroro?$"

