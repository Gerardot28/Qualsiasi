VictoryRoad_B1F_MapScripts::
	.byte 0

VictoryRoad_B1F_EventScript_Samuel::
	trainerbattle_single TRAINER_SAMUEL, VictoryRoad_B1F_Text_SamuelIntro, VictoryRoad_B1F_Text_SamuelDefeat
	msgbox VictoryRoad_B1F_Text_SamuelPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Shannon::
	trainerbattle_single TRAINER_SHANNON, VictoryRoad_B1F_Text_ShannonIntro, VictoryRoad_B1F_Text_ShannonDefeat
	msgbox VictoryRoad_B1F_Text_ShannonPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Michelle::
	trainerbattle_single TRAINER_MICHELLE, VictoryRoad_B1F_Text_MichelleIntro, VictoryRoad_B1F_Text_MichelleDefeat
	msgbox VictoryRoad_B1F_Text_MichellePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Mitchell::
	trainerbattle_single TRAINER_MITCHELL, VictoryRoad_B1F_Text_MitchellIntro, VictoryRoad_B1F_Text_MitchellDefeat
	msgbox VictoryRoad_B1F_Text_MitchellPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_EventScript_Halle::
	trainerbattle_single TRAINER_HALLE, VictoryRoad_B1F_Text_HalleIntro, VictoryRoad_B1F_Text_HalleDefeat
	msgbox VictoryRoad_B1F_Text_HallePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_B1F_Text_SamuelIntro:
	.string "Al pensiero di essere così vicino\n"
	.string "alla Lega Pokémon…\p"
	.string "Mi tremano le ginocchia…$"

VictoryRoad_B1F_Text_SamuelDefeat:
	.string "Non sono riuscito a fare niente…$"

VictoryRoad_B1F_Text_SamuelPostBattle:
	.string "La Lega Pokémon si è allontanata\n"
	.string "di nuovo… Che delusione…$"

VictoryRoad_B1F_Text_ShannonIntro:
	.string "Per farsi strada nella Lega Pokémon\n"
	.string "serve la piena fiducia dei propri\l"
	.string "Pokémon.$"

VictoryRoad_B1F_Text_ShannonDefeat:
	.string "Il vostro legame si fonda\n"
	.string "su una fiducia solidissima.$"

VictoryRoad_B1F_Text_ShannonPostBattle:
	.string "Stando sempre insieme, tra Pokémon\n"
	.string "e Allenatori nasce la fiducia.\p"
	.string "Vale ancora di più per i Pokémon\n"
	.string "arrivati dai Varchi: lontani da casa,\l"
	.string "si affidano a chi gli sta accanto.$"

VictoryRoad_B1F_Text_MichelleIntro:
	.string "Questo non è il traguardo. È solo\n"
	.string "una tappa verso la Lega Pokémon.$"

VictoryRoad_B1F_Text_MichelleDefeat:
	.string "Così si fa!$"

VictoryRoad_B1F_Text_MichellePostBattle:
	.string "Andrà benissimo, ne sono sicura!\n"
	.string "I tuoi Pokémon scalpitano!$"

VictoryRoad_B1F_Text_MitchellIntro:
	.string "I miei Pokémon hanno una potenza\n"
	.string "cosmica… anzi, multiversale!$"

VictoryRoad_B1F_Text_MitchellDefeat:
	.string "Non ho mai incontrato\n"
	.string "nessuno come te.$"

VictoryRoad_B1F_Text_MitchellPostBattle:
	.string "Anche lontano dalla lotta, sento\n"
	.string "un'energia incredibile che emana\l"
	.string "da te e dai tuoi Pokémon.$"

VictoryRoad_B1F_Text_HalleIntro:
	.string "Ehi, niente musi lunghi!\n"
	.string "Rilassati, prendiamola con calma!$"

VictoryRoad_B1F_Text_HalleDefeat:
	.string "Wow!\n"
	.string "Fantastico!$"

VictoryRoad_B1F_Text_HallePostBattle:
	.string "Certo, questa è la Via Vittoria.\p"
	.string "Ma non è poi così diversa dalla\n"
	.string "strada che hai fatto finora.\p"
	.string "Cerca di goderti il resto\n"
	.string "del viaggio!$"

