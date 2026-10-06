MagmaHideout_3F_2R_MapScripts::
	.byte 0

MagmaHideout_3F_2R_EventScript_Grunt10::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_10, MagmaHideout_3F_2R_Text_Grunt10Intro, MagmaHideout_3F_2R_Text_Grunt10Defeat
	msgbox MagmaHideout_3F_2R_Text_Grunt10PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_3F_2R_Text_Grunt10Intro:
	.string "Capisco tutto quello che dice il Capo.\n"
	.string "Ma sai una cosa?\p"
	.string "Risvegliare un Pokémon antichissimo,\n"
	.string "rubare il Meteorite a un professore,\l"
	.string "chiudere i Varchi con chissà chi\l"
	.string "ancora dentro…\p"
	.string "Secondo me stiamo esagerando.\n"
	.string "Tu che ne pensi?$"

MagmaHideout_3F_2R_Text_Grunt10Defeat:
	.string "Già… credo proprio che stiamo\n"
	.string "sbagliando qualcosa.$"

MagmaHideout_3F_2R_Text_Grunt10PostBattle:
	.string "Sai, perdere con te mi ha schiarito\n"
	.string "le idee.\p"
	.string "La prossima volta che vedo il Capo,\n"
	.string "gli chiederò cosa stiamo facendo.$"

