MagmaHideout_2F_1R_MapScripts::
	.byte 0

MagmaHideout_2F_1R_EventScript_Grunt14::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_14, MagmaHideout_2F_1R_Text_Grunt14Intro, MagmaHideout_2F_1R_Text_Grunt14Defeat
	msgbox MagmaHideout_2F_1R_Text_Grunt14PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_1R_EventScript_Grunt3::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_3, MagmaHideout_2F_1R_Text_Grunt3Intro, MagmaHideout_2F_1R_Text_Grunt3Defeat
	msgbox MagmaHideout_2F_1R_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_1R_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_4, MagmaHideout_2F_1R_Text_Grunt4Intro, MagmaHideout_2F_1R_Text_Grunt4Defeat
	msgbox MagmaHideout_2F_1R_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_1R_EventScript_Grunt5::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_5, MagmaHideout_2F_1R_Text_Grunt5Intro, MagmaHideout_2F_1R_Text_Grunt5Defeat
	msgbox MagmaHideout_2F_1R_Text_Grunt5PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_2F_1R_Text_Grunt14Intro:
	.string "Cosa, cosa, cosa?\p"
	.string "Qui dentro può entrare solo chi fa\n"
	.string "parte del Team Alpha!\p"
	.string "Ma tu non porti il mantello rosso del\n"
	.string "Team Alpha…\p"
	.string "Sei sospetto!\n"
	.string "In guardia!$"

MagmaHideout_2F_1R_Text_Grunt14Defeat:
	.string "Ahiahiahi…\n"
	.string "Il mio orgoglio di Recluta Alpha…$"

MagmaHideout_2F_1R_Text_Grunt14PostBattle:
	.string "Se soffri il freddo, arruolati nel\n"
	.string "Team Alpha. Qui si suda parecchio!$"

MagmaHideout_2F_1R_Text_Grunt3Intro:
	.string "Fermo lì!\p"
	.string "Credi davvero che me ne starò zitto\n"
	.string "a guardarti passare tranquillo?$"

MagmaHideout_2F_1R_Text_Grunt3Defeat:
	.string "Uau!\n"
	.string "Mi arrendo!$"

MagmaHideout_2F_1R_Text_Grunt3PostBattle:
	.string "Forse era meglio se ti lasciavo\n"
	.string "passare senza fare storie…$"

MagmaHideout_2F_1R_Text_Grunt4Intro:
	.string "Ah-ha!\n"
	.string "Un intruso!$"

MagmaHideout_2F_1R_Text_Grunt4Defeat:
	.string "Graaah!$"

MagmaHideout_2F_1R_Text_Grunt4PostBattle:
	.string "Ho già perso.\p"
	.string "Devo davvero continuare la ronda\n"
	.string "girando in tondo?$"

MagmaHideout_2F_1R_Text_Grunt5Intro:
	.string "Allarme, allarme!\n"
	.string "Un intruso!$"

MagmaHideout_2F_1R_Text_Grunt5Defeat:
	.string "Mmmh… Mugugno…$"

MagmaHideout_2F_1R_Text_Grunt5PostBattle:
	.string "A dire il vero, non sono un asso\n"
	.string "nelle lotte…$"

