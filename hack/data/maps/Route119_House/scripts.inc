Route119_House_MapScripts::
	.byte 0

Route119_House_EventScript_Woman::
	msgbox Route119_House_Text_RumorAboutCaveOfOrigin, MSGBOX_NPC
	end

Route119_House_EventScript_Wingull::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_WINGULL, CRY_MODE_NORMAL
	msgbox Route119_House_Text_Wingull, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Route119_House_Text_RumorAboutCaveOfOrigin:
	.string "Ho sentito parlare di una grotta\n"
	.string "chiamata Grotta dei Tempi.\p"
	.string "Si dice che lì gli spiriti dei Pokémon\n"
	.string "tornino in vita. Può davvero succedere\l"
	.string "una cosa del genere?$"

Route119_House_Text_Wingull:
	.string "Wingull: Piiyoh!$"

