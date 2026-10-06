AbandonedShip_Rooms2_1F_MapScripts::
	.byte 0

AbandonedShip_Rooms2_1F_EventScript_Dan::
	trainerbattle_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_DanIntro, AbandonedShip_Rooms2_1F_Text_DanDefeat, AbandonedShip_Rooms2_1F_Text_DanNotEnoughMons, AbandonedShip_Rooms2_1F_EventScript_RegisterDan
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, AbandonedShip_Rooms2_1F_EventScript_DanRematch
	msgbox AbandonedShip_Rooms2_1F_Text_DanPostBattle, MSGBOX_DEFAULT
	release
	end

AbandonedShip_Rooms2_1F_EventScript_RegisterDan::
	msgbox AbandonedShip_Rooms2_1F_Text_KiraRegister, MSGBOX_DEFAULT  @ Kira speaks for both when registering KiraAndDan
	register_matchcall TRAINER_KIRA_AND_DAN_1
	release
	end

AbandonedShip_Rooms2_1F_EventScript_DanRematch::
	trainerbattle_rematch_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_DanRematchIntro, AbandonedShip_Rooms2_1F_Text_DanRematchDefeat, AbandonedShip_Rooms2_1F_Text_DanRematchNotEnoughMons
	msgbox AbandonedShip_Rooms2_1F_Text_DanPostRematch, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_EventScript_Kira::
	trainerbattle_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_KiraIntro, AbandonedShip_Rooms2_1F_Text_KiraDefeat, AbandonedShip_Rooms2_1F_Text_KiraNotEnoughMons, AbandonedShip_Rooms2_1F_EventScript_RegisterKira
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, AbandonedShip_Rooms2_1F_EventScript_KiraRematch
	msgbox AbandonedShip_Rooms2_1F_Text_KiraPostBattle, MSGBOX_DEFAULT
	release
	end

AbandonedShip_Rooms2_1F_EventScript_RegisterKira::
	msgbox AbandonedShip_Rooms2_1F_Text_KiraRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_KIRA_AND_DAN_1
	release
	end

AbandonedShip_Rooms2_1F_EventScript_KiraRematch::
	trainerbattle_rematch_double TRAINER_KIRA_AND_DAN_1, AbandonedShip_Rooms2_1F_Text_KiraRematchIntro, AbandonedShip_Rooms2_1F_Text_KiraRematchDefeat, AbandonedShip_Rooms2_1F_Text_KiraRematchNotEnoughMons
	msgbox AbandonedShip_Rooms2_1F_Text_KiraPostRematch, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_EventScript_Jani::
	trainerbattle_single TRAINER_JANI, AbandonedShip_Rooms2_1F_Text_JaniIntro, AbandonedShip_Rooms2_1F_Text_JaniDefeat
	msgbox AbandonedShip_Rooms2_1F_Text_JaniPostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_EventScript_Garrison::
	trainerbattle_single TRAINER_GARRISON, AbandonedShip_Rooms2_1F_Text_GarrisonIntro, AbandonedShip_Rooms2_1F_Text_GarrisonDefeat
	msgbox AbandonedShip_Rooms2_1F_Text_GarrisonPostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Rooms2_1F_Text_DanIntro:
	.string "Dan: Cercavamo tesori, e invece\n"
	.string "abbiamo trovato un Allenatore!$"

AbandonedShip_Rooms2_1F_Text_DanDefeat:
	.string "Dan: Abbiamo perso, nonostante\n"
	.string "il gioco di squadra…$"

AbandonedShip_Rooms2_1F_Text_DanPostBattle:
	.string "Dan: Di tesori, neanche l'ombra…\n"
	.string "Che qualcuno ci abbia preceduti?$"

AbandonedShip_Rooms2_1F_Text_DanNotEnoughMons:
	.string "Dan: Non hai nemmeno due Pokémon.\n"
	.string "Così non puoi sperare di batterci.$"

AbandonedShip_Rooms2_1F_Text_KiraIntro:
	.string "Kira: Oh?\n"
	.string "Eravamo a caccia di tesori…\l"
	.string "E invece abbiamo scovato un Allenatore!$"

AbandonedShip_Rooms2_1F_Text_KiraDefeat:
	.string "Kira: Uuh, che forza!$"

AbandonedShip_Rooms2_1F_Text_KiraPostBattle:
	.string "Kira: Dove saranno finiti i tesori?\p"
	.string "Io ho già deciso cosa mi comprerò\n"
	.string "quando li avremo trovati!$"

AbandonedShip_Rooms2_1F_Text_KiraNotEnoughMons:
	.string "Kira: Oh, non hai due Pokémon?\n"
	.string "Allora lotteremo un'altra volta!$"

AbandonedShip_Rooms2_1F_Text_KiraRegister:
	.string "Kira: Uffa, mi hai fatto arrabbiare!\n"
	.string "Adesso ti registro, così impari!$"

AbandonedShip_Rooms2_1F_Text_DanRematchIntro:
	.string "Dan: Abbiamo continuato a cercare\n"
	.string "tesori per tutto questo tempo.\p"
	.string "E i nostri Pokémon sono diventati\n"
	.string "più forti. Te lo facciamo vedere, ok?$"

AbandonedShip_Rooms2_1F_Text_DanRematchDefeat:
	.string "Dan: Sei forte come sempre!$"

AbandonedShip_Rooms2_1F_Text_DanPostRematch:
	.string "Dan: Niente tesori, e perdiamo pure\n"
	.string "le lotte…\p"
	.string "Io vorrei tornare a casa… Ma se lo\n"
	.string "dico, lei si arrabbia da morire…$"

AbandonedShip_Rooms2_1F_Text_DanRematchNotEnoughMons:
	.string "Dan: Non hai nemmeno due Pokémon.\n"
	.string "Così non puoi sperare di batterci.$"

AbandonedShip_Rooms2_1F_Text_KiraRematchIntro:
	.string "Kira: Oh? Ci si rivede!\p"
	.string "Anche tu, come noi, non hai ancora\n"
	.string "smesso di cercare i tesori, vero?\p"
	.string "Facciamo così: chi perde rinuncia\n"
	.string "alla caccia al tesoro. Ci stai?$"

AbandonedShip_Rooms2_1F_Text_KiraRematchDefeat:
	.string "Kira: Oh, abbiamo perso di nuovo…$"

AbandonedShip_Rooms2_1F_Text_KiraPostRematch:
	.string "Kira: Non ce ne andiamo finché non\n"
	.string "avremo allenato meglio i Pokémon\l"
	.string "e trovato i tesori di questa nave!$"

AbandonedShip_Rooms2_1F_Text_KiraRematchNotEnoughMons:
	.string "Kira: Oh, non hai due Pokémon?\n"
	.string "Allora lotteremo un'altra volta!$"

AbandonedShip_Rooms2_1F_Text_JaniIntro:
	.string "Non so nuotare un granché,\n"
	.string "ma a lottare sono bravissima!$"

AbandonedShip_Rooms2_1F_Text_JaniDefeat:
	.string "Ops.\n"
	.string "Non è andata benissimo.$"

AbandonedShip_Rooms2_1F_Text_JaniPostBattle:
	.string "Camminare scalza su questa nave\n"
	.string "è un po' disgustoso… Bleah!$"

AbandonedShip_Rooms2_1F_Text_GarrisonIntro:
	.string "Forza e compassione…\n"
	.string "Ecco i veri tesori di un Allenatore!$"

AbandonedShip_Rooms2_1F_Text_GarrisonDefeat:
	.string "Ah, c'è qualcosa in te\n"
	.string "che scintilla.$"

AbandonedShip_Rooms2_1F_Text_GarrisonPostBattle:
	.string "In una cabina, da qualche parte a bordo,\n"
	.string "ho visto qualcosa scintillare.$"
