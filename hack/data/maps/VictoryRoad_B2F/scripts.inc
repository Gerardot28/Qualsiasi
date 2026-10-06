VictoryRoad_B2F_MapScripts::
	.byte 0

VictoryRoad_B2F_EventScript_Vito::
	trainerbattle_single TRAINER_VITO, VictoryRoad_B2F_Text_VitoIntro, VictoryRoad_B2F_Text_VitoDefeat
	msgbox VictoryRoad_B2F_Text_VitoPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Owen::
	trainerbattle_single TRAINER_OWEN, VictoryRoad_B2F_Text_OwenIntro, VictoryRoad_B2F_Text_OwenDefeat
	msgbox VictoryRoad_B2F_Text_OwenPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Caroline::
	trainerbattle_single TRAINER_CAROLINE, VictoryRoad_B2F_Text_CarolineIntro, VictoryRoad_B2F_Text_CarolineDefeat
	msgbox VictoryRoad_B2F_Text_CarolinePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Julie::
	trainerbattle_single TRAINER_JULIE, VictoryRoad_B2F_Text_JulieIntro, VictoryRoad_B2F_Text_JulieDefeat
	msgbox VictoryRoad_B2F_Text_JuliePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Felix::
	trainerbattle_single TRAINER_FELIX, VictoryRoad_B2F_Text_FelixIntro, VictoryRoad_B2F_Text_FelixDefeat
	msgbox VictoryRoad_B2F_Text_FelixPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_EventScript_Dianne::
	trainerbattle_single TRAINER_DIANNE, VictoryRoad_B2F_Text_DianneIntro, VictoryRoad_B2F_Text_DianneDefeat
	msgbox VictoryRoad_B2F_Text_DiannePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B2F_Text_VitoIntro:
	.string "Mi sono allenato con tutta la mia\n"
	.string "famiglia, nessuno escluso!\l"
	.string "Non perderò contro nessuno!$"

VictoryRoad_B2F_Text_VitoDefeat:
	.string "Più forte della mia famiglia?!\n"
	.string "Ma com'è possibile?!$"

VictoryRoad_B2F_Text_VitoPostBattle:
	.string "In famiglia ero il più forte di tutti.\n"
	.string "Non avevo mai perso…\p"
	.string "Ho perso la fiducia in me stesso…\n"
	.string "Forse torno a casa…$"

VictoryRoad_B2F_Text_OwenIntro:
	.string "Ho sentito che in giro c'è un\n"
	.string "ragazzino tosto. Parlano di te?$"

VictoryRoad_B2F_Text_OwenDefeat:
	.string "Il moscerino è tosto davvero!$"

VictoryRoad_B2F_Text_OwenPostBattle:
	.string "Si dice che quel ragazzino tosto sia\n"
	.string "il figlio del Capopalestra di Petalipoli.$"

VictoryRoad_B2F_Text_CarolineIntro:
	.string "Comincerai a essere un po' stanco.$"

VictoryRoad_B2F_Text_CarolineDefeat:
	.string "Neanche un segno di stanchezza!$"

VictoryRoad_B2F_Text_CarolinePostBattle:
	.string "La Via Vittoria e la Lega Pokémon\n"
	.string "sono sfide lunghe e sfiancanti.\l"
	.string "Attento alla fatica!$"

VictoryRoad_B2F_Text_JulieIntro:
	.string "Non montarti la testa solo perché\n"
	.string "hai tante Medaglie.\p"
	.string "Ci sarà sempre qualcuno più forte\n"
	.string "di te, in questo mondo o in un altro!$"

VictoryRoad_B2F_Text_JulieDefeat:
	.string "Sei più forte di me!$"

VictoryRoad_B2F_Text_JuliePostBattle:
	.string "Guarda le Medaglie che hai raccolto\n"
	.string "e ricorda gli Allenatori che hai\l"
	.string "affrontato.$"

VictoryRoad_B2F_Text_FelixIntro:
	.string "Sono arrivato fin qui, ma la tensione\n"
	.string "mi sta facendo venire il mal di pancia…$"

VictoryRoad_B2F_Text_FelixDefeat:
	.string "Ohi ohi…\n"
	.string "Che male…$"

VictoryRoad_B2F_Text_FelixPostBattle:
	.string "Più mi avvicino alla Lega Pokémon,\n"
	.string "più mi agito. Non ci posso fare niente.\p"
	.string "Il massimo che riesco a fare\n"
	.string "è fingere di essere calmo.$"

VictoryRoad_B2F_Text_DianneIntro:
	.string "In questa grotta si riunisce\n"
	.string "il meglio del meglio.\p"
	.string "Allora, come ti sembra?$"

VictoryRoad_B2F_Text_DianneDefeat:
	.string "Neanche un po' scosso!$"

VictoryRoad_B2F_Text_DiannePostBattle:
	.string "Hai fegato! Mi piace!\n"
	.string "Continua così!$"

