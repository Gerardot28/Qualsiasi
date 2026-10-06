RustboroCity_PokemonSchool_MapScripts::
	.byte 0

RustboroCity_PokemonSchool_EventScript_Blackboard::
	lockall
	msgbox RustboroCity_PokemonSchool_Text_BlackboardListsStatusChanges, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic::
	message RustboroCity_PokemonSchool_Text_ReadWhichTopic
	waitmessage
	multichoicegrid 8, 1, MULTI_STATUS_INFO, 3, FALSE
	switch VAR_RESULT
	case 0, RustboroCity_PokemonSchool_EventScript_Poison
	case 1, RustboroCity_PokemonSchool_EventScript_Paralysis
	case 2, RustboroCity_PokemonSchool_EventScript_Sleep
	case 3, RustboroCity_PokemonSchool_EventScript_Burn
	case 4, RustboroCity_PokemonSchool_EventScript_Freeze
	case 5, RustboroCity_PokemonSchool_EventScript_ExitTopicSelect
	case MULTI_B_PRESSED, RustboroCity_PokemonSchool_EventScript_ExitTopicSelect
	end

RustboroCity_PokemonSchool_EventScript_Poison::
	msgbox RustboroCity_PokemonSchool_Text_ExplainPoison, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Paralysis::
	msgbox RustboroCity_PokemonSchool_Text_ExplainParalysis, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Sleep::
	msgbox RustboroCity_PokemonSchool_Text_ExplainSleep, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Burn::
	msgbox RustboroCity_PokemonSchool_Text_ExplainBurn, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_Freeze::
	msgbox RustboroCity_PokemonSchool_Text_ExplainFreeze, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_ChooseBlackboardTopic
	end

RustboroCity_PokemonSchool_EventScript_ExitTopicSelect::
	releaseall
	end

RustboroCity_PokemonSchool_EventScript_GameboyKid1::
	msgbox RustboroCity_PokemonSchool_Text_TradingRightNow, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_GameboyKid2::
	msgbox RustboroCity_PokemonSchool_Text_AlwaysWantedSeedot, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_RichBoy::
	msgbox RustboroCity_PokemonSchool_Text_PokemontCantUseManMadeItems, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_Lass::
	msgbox RustboroCity_PokemonSchool_Text_ConfusedPokemonAttacksItself, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_SchoolKidM::
	msgbox RustboroCity_PokemonSchool_Text_PokemonHealItselfWithBerry, MSGBOX_NPC
	end

RustboroCity_PokemonSchool_EventScript_StudentNotebook::
	msgbox RustboroCity_PokemonSchool_Text_StudentsNotes, MSGBOX_SIGN
	end

RustboroCity_PokemonSchool_EventScript_Teacher::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_QUICK_CLAW, RustboroCity_PokemonSchool_EventScript_GaveQuickClaw
	call_if_eq VAR_FACING, DIR_EAST, RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsEast
	call_if_eq VAR_FACING, DIR_WEST, RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsWest
	msgbox RustboroCity_PokemonSchool_Text_StudentsWhoDontStudyGetQuickClaw, MSGBOX_DEFAULT
	giveitem ITEM_QUICK_CLAW
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	setflag FLAG_RECEIVED_QUICK_CLAW
	release
	end

RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsEast::
	applymovement VAR_LAST_TALKED, RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsEast
	waitmovement 0
	return

RustboroCity_PokemonSchool_EventScript_TeacherCheckOnStudentsWest::
	applymovement VAR_LAST_TALKED, RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsWest
	waitmovement 0
	return

RustboroCity_PokemonSchool_EventScript_GaveQuickClaw::
	msgbox RustboroCity_PokemonSchool_Text_ExplainQuickClaw, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	release
	end

RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsWest:
	walk_left
	walk_down
	walk_down
	walk_right
	walk_in_place_faster_down
	delay_16
	delay_16
	delay_16
	walk_down
	walk_in_place_faster_left
	delay_16
	delay_16
	walk_right
	delay_16
	delay_16
	delay_8
	walk_up
	walk_left
	walk_left
	walk_up
	walk_up
	walk_right
	step_end

RustboroCity_PokemonSchool_Movement_TeacherCheckOnStudentsEast:
	walk_right
	walk_right
	walk_down
	walk_down
	walk_left
	walk_left
	walk_in_place_faster_down
	delay_16
	delay_16
	delay_16
	walk_down
	walk_in_place_faster_left
	delay_16
	delay_16
	walk_right
	delay_16
	delay_16
	delay_8
	walk_up
	walk_right
	walk_up
	walk_up
	walk_left
	walk_left
	step_end

RustboroCity_PokemonSchool_EventScript_Scott::
	lock
	faceplayer
	goto_if_set FLAG_MET_SCOTT_AFTER_OBTAINING_STONE_BADGE, RustboroCity_PokemonSchool_EventScript_ScottWatchStudents
	goto_if_set FLAG_MET_SCOTT_RUSTBORO, RustboroCity_PokemonSchool_EventScript_ScottSpokeAlready
	goto_if_set FLAG_BADGE01_GET, RustboroCity_PokemonSchool_EventScript_ScottGreetHasBadge
	msgbox RustboroCity_PokemonSchool_Text_ScottMetAlreadyCut, MSGBOX_DEFAULT
	addvar VAR_SCOTT_STATE, 1
	setflag FLAG_MET_SCOTT_RUSTBORO
	release
	end

RustboroCity_PokemonSchool_EventScript_ScottSpokeAlready::
	goto_if_set FLAG_BADGE01_GET, RustboroCity_PokemonSchool_EventScript_ScottNoticeBadge
	msgbox RustboroCity_PokemonSchool_Text_StudentTalentLevelUnknown, MSGBOX_DEFAULT
	release
	end

RustboroCity_PokemonSchool_EventScript_ScottGreetHasBadge::
	msgbox RustboroCity_PokemonSchool_Text_ScottMetAlreadyStoneBadge, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_MetScottAfterBadge
	end

RustboroCity_PokemonSchool_EventScript_ScottNoticeBadge::
	msgbox RustboroCity_PokemonSchool_Text_ScottStoneBadge, MSGBOX_DEFAULT
	goto RustboroCity_PokemonSchool_EventScript_MetScottAfterBadge
	end

RustboroCity_PokemonSchool_EventScript_MetScottAfterBadge::
	addvar VAR_SCOTT_STATE, 1
	setflag FLAG_MET_SCOTT_AFTER_OBTAINING_STONE_BADGE
	release
	end

RustboroCity_PokemonSchool_EventScript_ScottWatchStudents::
	msgbox RustboroCity_PokemonSchool_Text_StudentTalentLevelUnknown, MSGBOX_DEFAULT
	release
	end

RustboroCity_PokemonSchool_Text_BlackboardListsStatusChanges:
	.string "La lavagna elenca le alterazioni di stato\n"
	.string "che possono colpire i Pokémon in lotta.$"

RustboroCity_PokemonSchool_Text_ReadWhichTopic:
	.string "Quale argomento vuoi leggere?$"

RustboroCity_PokemonSchool_Text_ExplainPoison:
	.string "Un Pokémon avvelenato perde PS\n"
	.string "a poco a poco.\p"
	.string "L'avvelenamento non svanisce alla\n"
	.string "fine della lotta.\p"
	.string "Un Pokémon avvelenato perde PS anche\n"
	.string "mentre siete in viaggio.\p"
	.string "Per curarlo, usa un Antidoto.$"

RustboroCity_PokemonSchool_Text_ExplainParalysis:
	.string "Se un Pokémon viene paralizzato,\n"
	.string "la sua Velocità cala.\p"
	.string "Inoltre, durante la lotta, a volte\n"
	.string "potrebbe non riuscire a muoversi.\p"
	.string "La paralisi resta anche dopo la lotta.\n"
	.string "Si cura con un Antiparalisi.$"

RustboroCity_PokemonSchool_Text_ExplainSleep:
	.string "Se un Pokémon si addormenta, non può\n"
	.string "attaccare.\p"
	.string "Può svegliarsi da solo, ma se la lotta\n"
	.string "finisce mentre sta ancora dormendo,\l"
	.string "resterà addormentato.\p"
	.string "Per svegliarlo, usa una Sveglia.$"

RustboroCity_PokemonSchool_Text_ExplainBurn:
	.string "La scottatura riduce l'Attacco e fa\n"
	.string "perdere PS a poco a poco.\p"
	.string "La scottatura resta dopo la lotta.\n"
	.string "Si cura con un Antiscottatura.$"

RustboroCity_PokemonSchool_Text_ExplainFreeze:
	.string "Un Pokémon congelato diventa\n"
	.string "del tutto indifeso.\p"
	.string "Il congelamento resta dopo la lotta.\n"
	.string "Si scongela con un Antigelo.$"

RustboroCity_PokemonSchool_Text_StudentsWhoDontStudyGetQuickClaw:
	.string "Agli studenti svogliati faccio\n"
	.string "assaggiare il mio Rapidartigli!\p"
	.string "Che tu sia un bravo studente o no,\n"
	.string "lo capirò da come userai questo\l"
	.string "strumento. Ripassa bene, mi raccomando!$"

RustboroCity_PokemonSchool_Text_ExplainQuickClaw:
	.string "Un Pokémon che tiene il Rapidartigli\n"
	.string "a volte scatta più in fretta e agisce\l"
	.string "prima del suo avversario.\p"
	.string "Esistono tanti altri strumenti pensati\n"
	.string "per essere tenuti dai Pokémon.\p"
	.string "Già quelli basterebbero a riempire\n"
	.string "un intero quaderno di appunti!$"

RustboroCity_PokemonSchool_Text_TradingRightNow:
	.string "Sto scambiando un Pokémon con il mio\n"
	.string "amico proprio adesso. Shh, non distrarmi!$"

RustboroCity_PokemonSchool_Text_AlwaysWantedSeedot:
	.string "Ho sempre desiderato un Seedot,\n"
	.string "e finalmente ne avrò uno!$"

RustboroCity_PokemonSchool_Text_PokemontCantUseManMadeItems:
	.string "I Pokémon possono tenere strumenti, ma\n"
	.string "non sanno usare quelli fatti dall'uomo,\l"
	.string "come la Pozione e l'Antidoto.$"

RustboroCity_PokemonSchool_Text_ConfusedPokemonAttacksItself:
	.string "Lo sai che certe mosse possono mandare\n"
	.string "in confusione un Pokémon?\p"
	.string "Un Pokémon confuso a volte finisce per\n"
	.string "colpire se stesso senza volerlo.\p"
	.string "Però, appena esce dalla lotta,\n"
	.string "torna come prima.$"

RustboroCity_PokemonSchool_Text_PokemonHealItselfWithBerry:
	.string "Un Pokémon che tiene una Bacca può\n"
	.string "curarsi da solo…\p"
	.string "Ci sono tanti tipi di strumenti che\n"
	.string "un Pokémon può tenere…\p"
	.string "Uff, prendere appunti è proprio\n"
	.string "una fatica…$"

RustboroCity_PokemonSchool_Text_StudentsNotes:
	.string "È il quaderno di uno studente…\p"
	.string "I Pokémon si catturano con le\n"
	.string "Poké Ball.\p"
	.string "Un Allenatore può portare con sé\n"
	.string "fino a sei Pokémon.\p"
	.string "Un Allenatore è chi cattura i Pokémon,\n"
	.string "li alleva e lotta al loro fianco.\p"
	.string "La missione di un Allenatore è battere\n"
	.string "i forti Allenatori che attendono\l"
	.string "gli sfidanti nelle Palestre Pokémon.\p"
	.string "In un angolo c'è scarabocchiato:\n"
	.string "“E i Pokémon dei Varchi? Chiedere!”$"

RustboroCity_PokemonSchool_Text_ScottMetAlreadyCut:
	.string "Ciao! Non ci siamo già visti?\n"
	.string "A Petalipoli, se non sbaglio.\p"
	.string "Permettimi di presentarmi.\n"
	.string "Mi chiamo Scott.\p"
	.string "Giro dappertutto in cerca di\n"
	.string "Allenatori fuori dal comune.\p"
	.string "Per la precisione, cerco veri esperti\n"
	.string "di lotte Pokémon.\p"
	.string "Con tutti questi Varchi, ne spuntano\n"
	.string "di nuovi ogni giorno. Interessante, no?\p"
	.string "Allora, cosa ti porta in questa Scuola?\n"
	.string "Sei un Allenatore anche tu?\p"
	.string "Per prima cosa, dovresti far imparare\n"
	.string "a un Pokémon la mossa Taglio.\p"
	.string "Se ricordo bene, qualcuno in questa\n"
	.string "città possiede la MN Taglio.$"

RustboroCity_PokemonSchool_Text_StudentTalentLevelUnknown:
	.string "Scott: Mmh…\p"
	.string "Il talento di questi studenti è ancora\n"
	.string "un mistero. Ma il potenziale c'è.$"

RustboroCity_PokemonSchool_Text_ScottStoneBadge:
	.string "Scott: Oh, e quella cos'è?\p"
	.string "È la Medaglia Pietra, vero?\n"
	.string "Niente male, direi.\p"
	.string "Però, sai, mi sarebbe piaciuto vederti\n"
	.string "lottare.\p"
	.string "Da una Medaglia non si capisce davvero\n"
	.string "che tipo di Allenatore sei.$"

RustboroCity_PokemonSchool_Text_ScottMetAlreadyStoneBadge:
	.string "Ciao! Non ci siamo già visti?\n"
	.string "A Petalipoli, se non sbaglio.\p"
	.string "Permettimi di presentarmi.\n"
	.string "Mi chiamo Scott.\p"
	.string "Giro dappertutto in cerca di\n"
	.string "Allenatori fuori dal comune.\p"
	.string "Per la precisione, cerco veri esperti\n"
	.string "di lotte Pokémon.\p"
	.string "Oh, e quella cos'è?\n"
	.string "È la Medaglia Pietra, vero?\p"
	.string "Niente male, direi, per un Allenatore\n"
	.string "alle prime armi come te.\p"
	.string "Però, sai, mi sarebbe piaciuto vederti\n"
	.string "lottare.\p"
	.string "Da una Medaglia non si capisce davvero\n"
	.string "che tipo di Allenatore sei.$"

