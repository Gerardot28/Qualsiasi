AbandonedShip_Rooms_1F_MapScripts::
	.byte 0

AbandonedShip_Rooms_1F_EventScript_Gentleman::
	msgbox AbandonedShip_Rooms_1F_Text_TakingALookAround, MSGBOX_NPC
	end

AbandonedShip_Rooms_1F_EventScript_Demetrius::
	trainerbattle_single TRAINER_DEMETRIUS, AbandonedShip_Rooms_1F_Text_DemetriusIntro, AbandonedShip_Rooms_1F_Text_DemetriusDefeat
	msgbox AbandonedShip_Rooms_1F_Text_DemetriusPostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms_1F_EventScript_Thalia::
	trainerbattle_single TRAINER_THALIA_1, AbandonedShip_Rooms_1F_Text_ThaliaIntro, AbandonedShip_Rooms_1F_Text_ThaliaDefeat, AbandonedShip_Rooms_1F_EventScript_RegisterThalia
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, AbandonedShip_Rooms_1F_EventScript_ThaliaRematch
	msgbox AbandonedShip_Rooms_1F_Text_ThaliaPostBattle, MSGBOX_DEFAULT
	release
	end

AbandonedShip_Rooms_1F_EventScript_RegisterThalia::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox AbandonedShip_Rooms_1F_Text_ThaliaRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_THALIA_1
	release
	end

AbandonedShip_Rooms_1F_EventScript_ThaliaRematch::
	trainerbattle_rematch TRAINER_THALIA_1, AbandonedShip_Rooms_1F_Text_ThaliaRematchIntro, AbandonedShip_Rooms_1F_Text_ThaliaRematchDefeat
	msgbox AbandonedShip_Rooms_1F_Text_ThaliaPostRematch, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms_1F_Text_TakingALookAround:
	.string "Navi di questo tipo sono rare, perciò\n"
	.string "ne approfitto per dare un'occhiata.\p"
	.string "Mhm…\n"
	.string "Pare che ci siano altre cabine…$"

AbandonedShip_Rooms_1F_Text_ThaliaIntro:
	.string "Cosa mai ti avrà spinto fin qui?\n"
	.string "Devi essere proprio un curiosone!$"

AbandonedShip_Rooms_1F_Text_ThaliaDefeat:
	.string "Non solo curioso, ma anche forte…$"

AbandonedShip_Rooms_1F_Text_ThaliaPostBattle:
	.string "L'uomo nella cabina accanto…\p"
	.string "Dice di essere un semplice turista,\n"
	.string "ma io non ne sono tanto sicura.$"

AbandonedShip_Rooms_1F_Text_ThaliaRegister:
	.string "Sei un Allenatore davvero tosto!\n"
	.string "Ti registro, così mi resta un ricordo!$"

AbandonedShip_Rooms_1F_Text_ThaliaRematchIntro:
	.string "Cosa mai ti avrà spinto a tornare?\n"
	.string "Devi essere proprio curiosissimo!$"

AbandonedShip_Rooms_1F_Text_ThaliaRematchDefeat:
	.string "Non sarai un po' troppo forte?$"

AbandonedShip_Rooms_1F_Text_ThaliaPostRematch:
	.string "Quell'uomo trama qualcosa, ne sono\n"
	.string "certa! Si comporta in modo sospetto!$"

AbandonedShip_Rooms_1F_Text_DemetriusIntro:
	.string "Uaaah!\n"
	.string "Mi hanno trovato! …Eh?$"

AbandonedShip_Rooms_1F_Text_DemetriusDefeat:
	.string "Ah, ma tu non sei la mia mamma.$"

AbandonedShip_Rooms_1F_Text_DemetriusPostBattle:
	.string "Ne ho combinata una e la mamma è\n"
	.string "furiosa, così sono scappato.\p"
	.string "Non dire a nessuno che sono qui!$"

