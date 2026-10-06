MagmaHideout_2F_2R_MapScripts::
	.byte 0

MagmaHideout_2F_2R_EventScript_Grunt15::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_15, MagmaHideout_2F_2R_Text_Grunt15Intro, MagmaHideout_2F_2R_Text_Grunt15Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt15PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_EventScript_Grunt6::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_6, MagmaHideout_2F_2R_Text_Grunt6Intro, MagmaHideout_2F_2R_Text_Grunt6Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt6PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_EventScript_Grunt7::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_7, MagmaHideout_2F_2R_Text_Grunt7Intro, MagmaHideout_2F_2R_Text_Grunt7Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt7PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_EventScript_Grunt8::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_8, MagmaHideout_2F_2R_Text_Grunt8Intro, MagmaHideout_2F_2R_Text_Grunt8Defeat
	msgbox MagmaHideout_2F_2R_Text_Grunt8PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_2R_Text_Grunt15Intro:
	.string "Non ho niente contro di te.\n"
	.string "Ordini dall'alto, capisci?$"

MagmaHideout_2F_2R_Text_Grunt15Defeat:
	.string "Avrò anche perso, ma…$"

MagmaHideout_2F_2R_Text_Grunt15PostBattle:
	.string "Abbiamo dissotterrato qualcosa di\n"
	.string "incredibile! E abbiamo la Gemma Blu!\p"
	.string "Ora al Capo non resta che…\n"
	.string "Fufufu… Fwahahaha!$"

MagmaHideout_2F_2R_Text_Grunt6Intro:
	.string "Il caldo non lo sopporto. Forse dovevo\n"
	.string "arruolarmi nel Team Omega…$"

MagmaHideout_2F_2R_Text_Grunt6Defeat:
	.string "Già, forse il Team Alpha non fa\n"
	.string "proprio per me…$"

MagmaHideout_2F_2R_Text_Grunt6PostBattle:
	.string "Non ti manca il mare, in un posto\n"
	.string "come questo?$"

MagmaHideout_2F_2R_Text_Grunt7Intro:
	.string "Qui sotto ogni tanto si sentono\n"
	.string "delle scosse.\p"
	.string "Sarà il vulcano che brontola?\n"
	.string "O forse è Grou…\p"
	.string "Ops!\n"
	.string "No, no, lascia perdere!$"

MagmaHideout_2F_2R_Text_Grunt7Defeat:
	.string "Lotti con un fuoco pazzesco.\n"
	.string "Proprio come un vulcano!$"

MagmaHideout_2F_2R_Text_Grunt7PostBattle:
	.string "Stavolta hai vinto per caso, ma non\n"
	.string "importa.\p"
	.string "L'obiettivo del Team Alpha è a un\n"
	.string "passo! Hoenn resta Hoenn!$"

MagmaHideout_2F_2R_Text_Grunt8Intro:
	.string "Uno dei nostri è andato nel panico\n"
	.string "perché ha perso lo Stemma Alpha…\p"
	.string "Un momento!\n"
	.string "Non l'avrai mica trovato tu?$"

MagmaHideout_2F_2R_Text_Grunt8Defeat:
	.string "Non riesco a crederci…$"

MagmaHideout_2F_2R_Text_Grunt8PostBattle:
	.string "Ho la brutta sensazione che il nostro\n"
	.string "piano finirà in un disastro…$"

