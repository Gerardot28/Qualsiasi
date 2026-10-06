MagmaHideout_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MagmaHideout_1F_OnTransition
	.byte 0

MagmaHideout_1F_OnTransition:
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 0
	end

MagmaHideout_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_1, MagmaHideout_1F_Text_Grunt1Intro, MagmaHideout_1F_Text_Grunt1Defeat
	msgbox MagmaHideout_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_2, MagmaHideout_1F_Text_Grunt2Intro, MagmaHideout_1F_Text_Grunt2Defeat
	msgbox MagmaHideout_1F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_Text_Grunt1Intro:
	.string "Quando il Team Alpha fa l'appello,\n"
	.string "i turni di guardia si assegnano in\l"
	.string "ordine di arrivo.\p"
	.string "Ecco perché sono finito in questo\n"
	.string "angolo. Arrivo sempre tardi all'appello!$"

MagmaHideout_1F_Text_Grunt1Defeat:
	.string "E arrivo tardi pure agli\n"
	.string "addestramenti!\p"
	.string "Lo ammetto: sono una schiappa…$"

MagmaHideout_1F_Text_Grunt1PostBattle:
	.string "Va bene, da domani giuro che mi\n"
	.string "impegno di più… Per l'Alpha!$"

MagmaHideout_1F_Text_Grunt2Intro:
	.string "Il Capo ci ha ordinato di scavare nel\n"
	.string "Monte Camino, e noi giù a scavare.\p"
	.string "E scava che ti scava, ci siamo imbattuti\n"
	.string "in qualcosa di incredibile!\p"
	.string "Cosa abbiamo trovato?\p"
	.string "Fuhahaha!\n"
	.string "Te lo dico se mi batti!$"

MagmaHideout_1F_Text_Grunt2Defeat:
	.string "Argh!\n"
	.string "Abbattuto!$"

MagmaHideout_1F_Text_Grunt2PostBattle:
	.string "Ripensandoci, non te lo dico.\n"
	.string "Lo scoprirai quando arriverai laggiù!\p"
	.string "Le sorprese è meglio tenerle per la\n"
	.string "fine, non ti pare?$"

