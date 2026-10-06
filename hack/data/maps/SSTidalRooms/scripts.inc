SSTidalRooms_MapScripts::
	.byte 0

SSTidalRooms_EventScript_SnatchGiver::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_SNATCH, SSTidalRooms_EventScript_ExplainSnatch
	msgbox SSTidalRooms_Text_NotSuspiciousTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_SNATCH
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_SNATCH
	msgbox SSTidalRooms_Text_ExplainSnatch, MSGBOX_DEFAULT
	release
	end

SSTidalRooms_EventScript_ExplainSnatch::
	msgbox SSTidalRooms_Text_ExplainSnatch, MSGBOX_DEFAULT
	release
	end

SSTidalRooms_EventScript_Bed::
	lockall
	msgbox SSTidalRooms_Text_TakeRestOnBed, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	call SSTidalRooms_EventScript_ProgessCruiseAfterBed
	releaseall
	end

SSTidalRooms_EventScript_Colton::
	trainerbattle_single TRAINER_COLTON, SSTidalRooms_Text_ColtonIntro, SSTidalRooms_Text_ColtonDefeat
	msgbox SSTidalRooms_Text_ColtonPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Micah::
	trainerbattle_single TRAINER_MICAH, SSTidalRooms_Text_MicahIntro, SSTidalRooms_Text_MicahDefeat
	msgbox SSTidalRooms_Text_MicahPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Thomas::
	trainerbattle_single TRAINER_THOMAS, SSTidalRooms_Text_ThomasIntro, SSTidalRooms_Text_ThomasDefeat
	msgbox SSTidalRooms_Text_ThomasPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Jed::
	trainerbattle_double TRAINER_LEA_AND_JED, SSTidalRooms_Text_JedIntro, SSTidalRooms_Text_JedDefeat, SSTidalRooms_Text_JedNotEnoughMons
	msgbox SSTidalRooms_Text_JedPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Lea::
	trainerbattle_double TRAINER_LEA_AND_JED, SSTidalRooms_Text_LeaIntro, SSTidalRooms_Text_LeaDefeat, SSTidalRooms_Text_LeaNotEnoughMons
	msgbox SSTidalRooms_Text_LeaPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Garret::
	trainerbattle_single TRAINER_GARRET, SSTidalRooms_Text_GarretIntro, SSTidalRooms_Text_GarretDefeat
	msgbox SSTidalRooms_Text_GarretPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_EventScript_Naomi::
	trainerbattle_single TRAINER_NAOMI, SSTidalRooms_Text_NaomiIntro, SSTidalRooms_Text_NaomiDefeat
	msgbox SSTidalRooms_Text_NaomiPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalRooms_Text_TakeRestOnBed:
	.string "C'è un letto…\n"
	.string "Riposiamoci un po'.$"

SSTidalRooms_Text_ColtonIntro:
	.string "Vado spesso a Porto Alghepoli.\p"
	.string "Sai, adoro assistere alle\n"
	.string "Gare Pokémon.$"

SSTidalRooms_Text_ColtonDefeat:
	.string "Che lotta deliziosa, davvero!$"

SSTidalRooms_Text_ColtonPostBattle:
	.string "Mi emoziono solo a immaginare quali\n"
	.string "Pokémon vedrò alla prossima Gara.\l"
	.string "Magari qualcuno arrivato da un Varco!$"

SSTidalRooms_Text_MicahIntro:
	.string "I tuoi amici sono forti?$"

SSTidalRooms_Text_MicahDefeat:
	.string "I tuoi amici sono forti davvero.$"

SSTidalRooms_Text_MicahPostBattle:
	.string "Un amico non deve per forza essere\n"
	.string "umano: i Pokémon sono amici preziosi!$"

SSTidalRooms_Text_ThomasIntro:
	.string "Giovanotto…\n"
	.string "Hai forse bussato alla porta?$"

SSTidalRooms_Text_ThomasDefeat:
	.string "Una sconfitta si accetta senza fretta\n"
	.string "né panico.$"

SSTidalRooms_Text_ThomasPostBattle:
	.string "Non scomporsi mai, in nessun caso:\n"
	.string "è il codice d'onore del Gentiluomo.$"

SSTidalRooms_Text_JedIntro:
	.string "Jed: Mi vergogno un pochino, ma…\n"
	.string "Ecco a te il potere dei piccioncini!$"

SSTidalRooms_Text_JedDefeat:
	.string "Jed: Sob…$"

SSTidalRooms_Text_JedPostBattle:
	.string "Jed: È la prima volta che il potere\n"
	.string "dei piccioncini non basta!\l"
	.string "Sei proprio un Allenatore fantastico!$"

SSTidalRooms_Text_JedNotEnoughMons:
	.string "Jed: Hai un solo Pokémon?\n"
	.string "Non ti senti un po' solo?$"

SSTidalRooms_Text_LeaIntro:
	.string "Lea: Mi sento un po' sciocca, ma…\n"
	.string "Ecco a te il potere dei piccioncini!$"

SSTidalRooms_Text_LeaDefeat:
	.string "Lea: Uffa!$"

SSTidalRooms_Text_LeaPostBattle:
	.string "Lea: Non ci posso credere!\n"
	.string "Il potere dei piccioncini ha fallito…\l"
	.string "Sei proprio un Allenatore fantastico!$"

SSTidalRooms_Text_LeaNotEnoughMons:
	.string "Lea: Volevo tanto lottare…\n"
	.string "Ma non hai nemmeno due Pokémon…$"

SSTidalRooms_Text_GarretIntro:
	.string "Ah, arrivi giusto in tempo.\p"
	.string "Mi annoio, vedi.\n"
	.string "Puoi intrattenermi.$"

SSTidalRooms_Text_GarretDefeat:
	.string "…Può bastare così.$"

SSTidalRooms_Text_GarretPostBattle:
	.string "Forse chiederò a mio padre di comprarmi\n"
	.string "uno yacht.\l"
	.string "Uno yacht per me e i miei Pokémon!$"

SSTidalRooms_Text_NaomiIntro:
	.string "Oh, che Allenatore adorabile.\n"
	.string "Gradisci una tazza di tè?\l"
	.string "O preferisci lottare?$"

SSTidalRooms_Text_NaomiDefeat:
	.string "Capisco.\n"
	.string "Sei un tipo dinamico.$"

SSTidalRooms_Text_NaomiPostBattle:
	.string "Una crociera su uno yacht di lusso ha\n"
	.string "il suo fascino, devo ammetterlo…\p"
	.string "Ma anche girare Hoenn in traghetto\n"
	.string "ha un che di irresistibile.$"

SSTidalRooms_Text_NotSuspiciousTakeThis:
	.string "Ehm… Ciao! Io… non sto facendo niente\n"
	.string "di sospetto! Ehm… Tieni! È gratis!\p"
	.string "Non… Giuro, non l'ho scippata a\n"
	.string "nessuno! Non lo farei mai!\l"
	.string "È pulita! Puoi usarla!$"

SSTidalRooms_Text_ExplainSnatch:
	.string "Scippo ruba gli effetti positivi di\n"
	.string "certe mosse prima che possa sfruttarli\l"
	.string "un avversario o un alleato.$"

