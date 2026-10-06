MtPyre_3F_MapScripts::
	.byte 0

MtPyre_3F_EventScript_William::
	trainerbattle_single TRAINER_WILLIAM, MtPyre_3F_Text_WilliamIntro, MtPyre_3F_Text_WilliamDefeat
	msgbox MtPyre_3F_Text_WilliamPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_3F_EventScript_Kayla::
	trainerbattle_single TRAINER_KAYLA, MtPyre_3F_Text_KaylaIntro, MtPyre_3F_Text_KaylaDefeat
	msgbox MtPyre_3F_Text_KaylaPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_3F_EventScript_Gabrielle::
	trainerbattle_single TRAINER_GABRIELLE_1, MtPyre_3F_Text_GabrielleIntro, MtPyre_3F_Text_GabrielleDefeat, MtPyre_3F_EventScript_RegisterGabrielle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtPyre_3F_EventScript_RematchGabrielle
	msgbox MtPyre_3F_Text_GabriellePostBattle, MSGBOX_DEFAULT
	release
	end

MtPyre_3F_EventScript_RegisterGabrielle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtPyre_3F_Text_GabrielleRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_GABRIELLE_1
	release
	end

MtPyre_3F_EventScript_RematchGabrielle::
	trainerbattle_rematch TRAINER_GABRIELLE_1, MtPyre_3F_Text_GabrielleRematchIntro, MtPyre_3F_Text_GabrielleRematchDefeat
	msgbox MtPyre_3F_Text_GabriellePostRematch, MSGBOX_AUTOCLOSE
	end

MtPyre_3F_Text_WilliamIntro:
	.string "L'aria densa di questo monte ha\n"
	.string "potenziato i miei poteri psichici!\p"
	.string "Un moccioso come te…\n"
	.string "Sogni davvero di vincere?$"

MtPyre_3F_Text_WilliamDefeat:
	.string "Che pena… Mi faccio pena da solo…$"

MtPyre_3F_Text_WilliamPostBattle:
	.string "I miei poteri psichici sono cresciuti\n"
	.string "parecchio, quassù. Eppure…\p"
	.string "Eppure non ho previsto la tua vittoria.$"

MtPyre_3F_Text_KaylaIntro:
	.string "Ahahahahah!\p"
	.string "Questo non è posto per bambini,\n"
	.string "e tu meno di tutti!$"

MtPyre_3F_Text_KaylaDefeat:
	.string "Battuta senza appello…$"

MtPyre_3F_Text_KaylaPostBattle:
	.string "Vuol dire che il mio allenamento\n"
	.string "non basta ancora…\p"
	.string "Devo continuare a salire verso\n"
	.string "la vetta…\p"
	.string "Forza, Kayla! Ce la puoi fare!$"

MtPyre_3F_Text_GabrielleIntro:
	.string "Perché sei venuto fin quassù?$"

MtPyre_3F_Text_GabrielleDefeat:
	.string "Straordinario!\n"
	.string "Sei un Allenatore davvero speciale.$"

MtPyre_3F_Text_GabriellePostBattle:
	.string "I Pokémon che hanno lasciato questo\n"
	.string "mondo. I Pokémon che sono con te ora.\p"
	.string "E quelli che incontrerai in futuro,\n"
	.string "da qualunque mondo arrivino…\p"
	.string "Meritano tutti lo stesso affetto.\n"
	.string "Ricordatelo, ti prego.$"

MtPyre_3F_Text_GabrielleRegister:
	.string "Vorrei rivedere i tuoi Pokémon\n"
	.string "quando saranno cresciuti ancora…\p"
	.string "Ti prego, fammi vedere il tuo PokéNav.$"

MtPyre_3F_Text_GabrielleRematchIntro:
	.string "Oh, sei tu…\p"
	.string "Sei venuto a mostrarmi quanto sono\n"
	.string "cresciuti i tuoi Pokémon?$"

MtPyre_3F_Text_GabrielleRematchDefeat:
	.string "Che meraviglia!\n"
	.string "Sei una persona speciale.$"

MtPyre_3F_Text_GabriellePostRematch:
	.string "I Pokémon che hanno lasciato questo\n"
	.string "mondo. I Pokémon che sono con te ora.\p"
	.string "E quelli che incontrerai in futuro,\n"
	.string "da qualunque mondo arrivino…\p"
	.string "Meritano tutti lo stesso affetto.\n"
	.string "Vedo che non l'hai dimenticato.$"

