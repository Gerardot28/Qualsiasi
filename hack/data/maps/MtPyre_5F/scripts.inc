MtPyre_5F_MapScripts::
	.byte 0

@ Seems like the scripts for the 4F and 5F were swapped

MtPyre_4F_EventScript_Tasha::
	trainerbattle_single TRAINER_TASHA, MtPyre_4F_Text_TashaIntro, MtPyre_4F_Text_TashaDefeat
	msgbox MtPyre_4F_Text_TashaPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_4F_Text_TashaIntro:
	.string "Adoro tutto ciò che fa paura…\n"
	.string "È come una malattia…\p"
	.string "Quando sono qui…\n"
	.string "tremo di terrore…$"

MtPyre_4F_Text_TashaDefeat:
	.string "Perdere… non mi piace…$"

MtPyre_4F_Text_TashaPostBattle:
	.string "Voglio vedere cose spaventose…\n"
	.string "Non riesco ad andarmene…\p"
	.string "Resta…\n"
	.string "Non vuoi restare qui con me?$"

