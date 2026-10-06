MagmaHideout_3F_1R_MapScripts::
	.byte 0

MagmaHideout_3F_1R_EventScript_Grunt9::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_9, MagmaHideout_3F_1R_Text_Grunt9Intro, MagmaHideout_3F_1R_Text_Grunt9Defeat
	msgbox MagmaHideout_3F_1R_Text_Grunt9PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_1R_EventScript_Grunt16::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_16, MagmaHideout_3F_1R_Text_Grunt16Intro, MagmaHideout_3F_1R_Text_Grunt16Defeat
	msgbox MagmaHideout_3F_1R_Text_Grunt16PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_1R_Text_Grunt9Intro:
	.string "Cosa ho fatto per meritarmi questo\n"
	.string "turno di guardia?\p"
	.string "Mi va a fuoco l'orecchio sinistro!$"

MagmaHideout_3F_1R_Text_Grunt9Defeat:
	.string "Mi sta venendo un colpo di calore…$"

MagmaHideout_3F_1R_Text_Grunt9PostBattle:
	.string "Non trovi strano che portiamo il\n"
	.string "cappuccio dentro un vulcano pieno\l"
	.string "di magma?$"

MagmaHideout_3F_1R_Text_Grunt16Intro:
	.string "Ci siamo arruolati per aiutare il Capo\n"
	.string "a realizzare il suo grande sogno:\l"
	.string "una Hoenn sigillata e al sicuro.\p"
	.string "Che tu sia del Team Omega o solo un\n"
	.string "ragazzino di passaggio, non importa.\p"
	.string "Nessuno ci mette i bastoni tra le\n"
	.string "ruote e la passa liscia!$"

MagmaHideout_3F_1R_Text_Grunt16Defeat:
	.string "Oh, no!\n"
	.string "Di te non ci si può fidare!$"

MagmaHideout_3F_1R_Text_Grunt16PostBattle:
	.string "Ascoltami bene.\n"
	.string "Il Team Alpha ha ragione!\p"
	.string "Non dare retta al Team Omega.\n"
	.string "Non credere alle loro bugie!$"

