Route109_SeashoreHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route109_SeashoreHouse_OnTransition
	.byte 0

Route109_SeashoreHouse_OnTransition:
	setflag FLAG_LANDMARK_SEASHORE_HOUSE
	end

Route109_SeashoreHouse_EventScript_Owner::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_6_SODA_POP, Route109_SeashoreHouse_EventScript_AlreadyReceivedSodaPop
	goto_if_set FLAG_DEFEATED_SEASHORE_HOUSE, Route109_SeashoreHouse_EventScript_DefeatedTrainers
	goto_if_set FLAG_TEMP_2, Route109_SeashoreHouse_EventScript_AlreadyGaveIntroduction
	msgbox Route109_SeashoreHouse_Text_SeashoreHouseIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_2
	release
	end

Route109_SeashoreHouse_EventScript_AlreadyGaveIntroduction::
	msgbox Route109_SeashoreHouse_Text_ShowMeSomeHotMatches, MSGBOX_DEFAULT
	release
	end

Route109_SeashoreHouse_EventScript_DefeatedTrainers::
	msgbox Route109_SeashoreHouse_Text_TakeTheseSodaPopBottles, MSGBOX_DEFAULT
	giveitem ITEM_SODA_POP, 6
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_BagFull
	setflag FLAG_RECEIVED_6_SODA_POP
	release
	end

Route109_SeashoreHouse_EventScript_BagFull::
	msgbox Route109_SeashoreHouse_Text_BagFull, MSGBOX_DEFAULT
	release
	end

Route109_SeashoreHouse_EventScript_AlreadyReceivedSodaPop::
	showmoneybox 0, 0
	msgbox Route109_SeashoreHouse_Text_WantToBuySodaPop, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route109_SeashoreHouse_EventScript_BuySodaPop
	msgbox Route109_SeashoreHouse_Text_ThatsTooBad, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_BuySodaPop::
	checkmoney 300
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_NotEnoughMoney
	checkitemspace ITEM_SODA_POP
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_NotEnoughSpace
	msgbox Route109_SeashoreHouse_Text_HereYouGo, MSGBOX_DEFAULT
	removemoney 300
	updatemoneybox
.if OW_SHOW_ITEM_DESCRIPTIONS != OW_ITEM_DESCRIPTIONS_OFF
	hidemoneybox
	giveitem ITEM_SODA_POP
.else
	giveitem ITEM_SODA_POP
	hidemoneybox
.endif
	release
	end

Route109_SeashoreHouse_EventScript_NotEnoughMoney::
	msgbox Route109_SeashoreHouse_Text_NotEnoughMoney, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_NotEnoughSpace::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_Dwayne::
	trainerbattle_single TRAINER_DWAYNE, Route109_SeashoreHouse_Text_DwayneIntro, Route109_SeashoreHouse_Text_DwayneDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_DwaynePostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_Johanna::
	trainerbattle_single TRAINER_JOHANNA, Route109_SeashoreHouse_Text_JohannaIntro, Route109_SeashoreHouse_Text_JohannaDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_JohannaPostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_Simon::
	trainerbattle_single TRAINER_SIMON, Route109_SeashoreHouse_Text_SimonIntro, Route109_SeashoreHouse_Text_SimonDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_SimonPostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_CheckTrainersCompletion::
	goto_if_not_defeated TRAINER_DWAYNE, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	goto_if_not_defeated TRAINER_JOHANNA, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	goto_if_not_defeated TRAINER_SIMON, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	setflag FLAG_DEFEATED_SEASHORE_HOUSE
	release
	end

Route109_SeashoreHouse_EventScript_TrainersNotCompleted::
	release
	end

Route109_SeashoreHouse_Text_SeashoreHouseIntro:
	.string "Sono il proprietario della Casa sul\n"
	.string "Mare. Ma chiamami pure Sig. Salsedine!\p"
	.string "Quello che amo più di tutto è vedere\n"
	.string "lotte tra Pokémon belle infuocate.\p"
	.string "Fammi vedere che il tuo cuore brucia!\p"
	.string "Se riesci a battere tutti gli Allenatori\n"
	.string "qui dentro, premierò i tuoi sforzi.$"

Route109_SeashoreHouse_Text_ShowMeSomeHotMatches:
	.string "Fammi vedere qualche lotta rovente!\p"
	.string "Gestisco questa Casa sul Mare\n"
	.string "solo ed esclusivamente per questo!$"

Route109_SeashoreHouse_Text_TakeTheseSodaPopBottles:
	.string "Sei incandescente!\n"
	.string "Che lotte da far scintille!\l"
	.string "Sono più che soddisfatto!\p"
	.string "Per ringraziarti di questo spettacolo\n"
	.string "bollente, voglio che tu prenda queste.\p"
	.string "Mezza dozzina di bottiglie di Gassosa!$"

Route109_SeashoreHouse_Text_BagFull:
	.string "Oh, ma ehi, la tua Borsa è stracolma.\n"
	.string "Te le tengo da parte io.$"

Route109_SeashoreHouse_Text_WantToBuySodaPop:
	.string "Vuoi comprare un po' di Gassosa?\n"
	.string "I Pokémon ne vanno matti!\p"
	.string "Solo ¥300 a bottiglia!\n"
	.string "Dai, prendine una!$"

Route109_SeashoreHouse_Text_HereYouGo:
	.string "Ecco a te!$"

Route109_SeashoreHouse_Text_NotEnoughMoney:
	.string "Non hai abbastanza soldi.$"

Route109_SeashoreHouse_Text_ThatsTooBad:
	.string "No?\n"
	.string "Peccato.$"

Route109_SeashoreHouse_Text_DwayneIntro:
	.string "Se cerchi una lotta nella Casa sul\n"
	.string "Mare, non troverai Allenatore più\l"
	.string "infuocato di me, parola di marinaio!$"

Route109_SeashoreHouse_Text_DwayneDefeated:
	.string "Che lotta rovente!\n"
	.string "La sconfitta la accetto a testa alta!$"

Route109_SeashoreHouse_Text_DwaynePostBattle:
	.string "Quando passo da Porto Selcepoli, mi\n"
	.string "godo lotte roventi e Gassosa ghiacciata!$"

Route109_SeashoreHouse_Text_JohannaIntro:
	.string "Le lotte noiose non valgono la fatica.\p"
	.string "Sono le lotte infuocate a temprare\n"
	.string "Allenatori e Pokémon!$"

Route109_SeashoreHouse_Text_JohannaDefeated:
	.string "Che caldo!$"

Route109_SeashoreHouse_Text_JohannaPostBattle:
	.string "Uff, che sete.\n"
	.string "Quasi quasi mi prendo una Gassosa.$"

Route109_SeashoreHouse_Text_SimonIntro:
	.string "Ti faccio vedere quanto sono forti\n"
	.string "i miei Pokémon, ma non piangere!$"

Route109_SeashoreHouse_Text_SimonDefeated:
	.string "…Ho perso, ma non piango…$"

Route109_SeashoreHouse_Text_SimonPostBattle:
	.string "Se uno dei miei Pokémon sapesse la\n"
	.string "mossa per portarmi sul dorso in acqua,\l"
	.string "potrei buttare via questo salvagente.$"

