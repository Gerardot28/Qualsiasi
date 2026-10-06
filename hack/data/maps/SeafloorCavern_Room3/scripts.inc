SeafloorCavern_Room3_MapScripts::
	.byte 0

SeafloorCavern_Room3_EventScript_Shelly::
	trainerbattle_single TRAINER_SHELLY_SEAFLOOR_CAVERN, SeafloorCavern_Room3_Text_ShellyIntro, SeafloorCavern_Room3_Text_ShellyDefeat
	msgbox SeafloorCavern_Room3_Text_ShellyPostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room3_EventScript_Grunt5::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_5, SeafloorCavern_Room3_Text_Grunt5Intro, SeafloorCavern_Room3_Text_Grunt5Defeat
	msgbox SeafloorCavern_Room3_Text_Grunt5PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room3_Text_ShellyIntro:
	.string "Ahahahah!\p"
	.string "Come hai fatto ad arrivare fin qui\n"
	.string "senza un sottomarino?\l"
	.string "Che ragazzino sorprendente!\p"
	.string "Ma… non posso lasciarti ficcare\n"
	.string "il naso proprio qui, tesoro.\p"
	.string "E poi devo ancora fartela pagare per\n"
	.string "quello che è successo all'Istituto\l"
	.string "Meteo…\p"
	.string "Ti farò assaggiare un pizzico di\n"
	.string "dolore! Rassegnati, tesoro!$"

SeafloorCavern_Room3_Text_ShellyDefeat:
	.string "Ahahahah!\p"
	.string "Ahia!$"

SeafloorCavern_Room3_Text_ShellyPostBattle:
	.string "Ahahahah!\n"
	.string "Sei proprio forte, tesoro.\p"
	.string "Che peccato che tu non sia uno\n"
	.string "del Team Omega.\p"
	.string "Avresti potuto vivere con noi nel\n"
	.string "mondo senza confini che il Capo\l"
	.string "ci ha promesso…\p"
	.string "…Tesoro, posso dirti una cosa?\n"
	.string "E se il Capo si sbagliasse?\p"
	.string "No, niente. Dimentica quello che\n"
	.string "ho detto. Ahahah…$"

SeafloorCavern_Room3_Text_Grunt5Intro:
	.string "Per realizzare il nostro sogno\n"
	.string "ci serve la forza dei Pokémon.\p"
	.string "Ma i ficcanaso come te usano i Pokémon\n"
	.string "per metterci i bastoni tra le ruote\l"
	.string "perfino qui, in fondo al mare!\p"
	.string "La vita non va mai come vorremmo,\n"
	.string "arrr!$"

SeafloorCavern_Room3_Text_Grunt5Defeat:
	.string "Uaaargh!$"

SeafloorCavern_Room3_Text_Grunt5PostBattle:
	.string "Sai, noi non ci sogniamo nemmeno di\n"
	.string "mettere in dubbio gli ordini del Capo.\p"
	.string "Eppure eccoti qui: un mozzo qualunque\n"
	.string "che sfida Ulisse in persona.\p"
	.string "Forse…\n"
	.string "Forse non sei poi così qualunque…$"

