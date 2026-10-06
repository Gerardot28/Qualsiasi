MtPyre_6F_MapScripts::
	.byte 0

MtPyre_6F_EventScript_Valerie::
	trainerbattle_single TRAINER_VALERIE_1, MtPyre_6F_Text_ValerieIntro, MtPyre_6F_Text_ValerieDefeat, MtPyre_6F_EventScript_RegisterValerie
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtPyre_6F_EventScript_RematchValerie
	msgbox MtPyre_6F_Text_ValeriePostBattle, MSGBOX_DEFAULT
	release
	end

MtPyre_6F_EventScript_RegisterValerie::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtPyre_6F_Text_ValerieRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_VALERIE_1
	release
	end

MtPyre_6F_EventScript_RematchValerie::
	trainerbattle_rematch TRAINER_VALERIE_1, MtPyre_6F_Text_ValerieRematchIntro, MtPyre_6F_Text_ValerieRematchDefeat
	msgbox MtPyre_6F_Text_ValeriePostRematch, MSGBOX_AUTOCLOSE
	end

MtPyre_6F_EventScript_Cedric::
	trainerbattle_single TRAINER_CEDRIC, MtPyre_6F_Text_CedricIntro, MtPyre_6F_Text_CedricDefeat
	msgbox MtPyre_6F_Text_CedricPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_6F_Text_ValerieIntro:
	.string "Quando sono qui…\n"
	.string "uno strano potere scorre in me…$"

MtPyre_6F_Text_ValerieDefeat:
	.string "Il potere sta svanendo…$"

MtPyre_6F_Text_ValeriePostBattle:
	.string "Forse viene dagli spiriti dei Pokémon\n"
	.string "che qui dormono un sonno inquieto…\p"
	.string "O forse da qualcosa che sta\n"
	.string "al di là della Soglia…$"

MtPyre_6F_Text_ValerieRegister:
	.string "Ihihih… Ho perso la lotta, ma…\n"
	.string "ho un piccolo talento…\p"
	.string "Senza nemmeno sfiorare il tuo\n"
	.string "PokéNav… Hiyah!$"

MtPyre_6F_Text_ValerieRematchIntro:
	.string "Dietro di te…\n"
	.string "Che cos'è…$"

MtPyre_6F_Text_ValerieRematchDefeat:
	.string "Qualcosa è svanito…$"

MtPyre_6F_Text_ValeriePostRematch:
	.string "I Pokémon che riposano qui…\n"
	.string "A volte giocano…$"

MtPyre_6F_Text_CedricIntro:
	.string "Ti sei perso? Niente paura:\n"
	.string "l'avevo previsto, e sono qui io!$"

MtPyre_6F_Text_CedricDefeat:
	.string "Ma non ti eri perso?$"

MtPyre_6F_Text_CedricPostBattle:
	.string "Credevo che un Allenatore sperduto\n"
	.string "fosse nel panico e facile da battere.\p"
	.string "È un trucco sporco.\n"
	.string "Non ci riproverò…$"

