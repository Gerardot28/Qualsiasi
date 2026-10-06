SeafloorCavern_Room1_MapScripts::
	.byte 0

SeafloorCavern_Room1_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_1, SeafloorCavern_Room1_Text_Grunt1Intro, SeafloorCavern_Room1_Text_Grunt1Defeat
	msgbox SeafloorCavern_Room1_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room1_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_2, SeafloorCavern_Room1_Text_Grunt2Intro, SeafloorCavern_Room1_Text_Grunt2Defeat
	msgbox SeafloorCavern_Room1_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room1_Text_Grunt1Intro:
	.string "Qui sotto i mocciosi non servono!\n"
	.string "Torna a casa, mozzo!$"

SeafloorCavern_Room1_Text_Grunt1Defeat:
	.string "Voglio tornare a casa…$"

SeafloorCavern_Room1_Text_Grunt1PostBattle:
	.string "Voglio una promozione, così potrò\n"
	.string "dare ordini alle Reclute…$"

SeafloorCavern_Room1_Text_Grunt2Intro:
	.string "Quel sottomarino… è minuscolo!\n"
	.string "Ho le ossa a pezzi!$"

SeafloorCavern_Room1_Text_Grunt2Defeat:
	.string "Perdere fa ancora più male!$"

SeafloorCavern_Room1_Text_Grunt2PostBattle:
	.string "Il sottomarino che abbiamo arrembato…\n"
	.string "che viaggio da incubo, arrr!\l"
	.string "Lì dentro si sta stretti come sardine!$"

