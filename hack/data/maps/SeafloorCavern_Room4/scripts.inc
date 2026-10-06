SeafloorCavern_Room4_MapScripts::
	.byte 0

SeafloorCavern_Room4_EventScript_Grunt3::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_3, SeafloorCavern_Room4_Text_Grunt3Intro, SeafloorCavern_Room4_Text_Grunt3Defeat
	msgbox SeafloorCavern_Room4_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room4_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_4, SeafloorCavern_Room4_Text_Grunt4Intro, SeafloorCavern_Room4_Text_Grunt4Defeat
	msgbox SeafloorCavern_Room4_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room4_Text_Grunt3Intro:
	.string "Chi sei tu?\n"
	.string "Da dove sei sbucato?$"

SeafloorCavern_Room4_Text_Grunt3Defeat:
	.string "Affondato…$"

SeafloorCavern_Room4_Text_Grunt3PostBattle:
	.string "Non trovo più l'uscita!\p"
	.string "Non ho paura, eh! Non fraintendermi!$"

SeafloorCavern_Room4_Text_Grunt4Intro:
	.string "Chi sei tu?\n"
	.string "Dove credi di andare?$"

SeafloorCavern_Room4_Text_Grunt4Defeat:
	.string "Niente vittoria? Arrr!$"

SeafloorCavern_Room4_Text_Grunt4PostBattle:
	.string "Il mio compare ha lasciato la mappa\n"
	.string "nel sottomarino!\p"
	.string "Più inutile di così, impossibile!$"

