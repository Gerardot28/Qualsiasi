RustboroCity_Gym_MapScripts::
	.byte 0

RustboroCity_Gym_EventScript_Roxanne::
	trainerbattle_single TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxanneIntro, RustboroCity_Gym_Text_RoxanneDefeat, RustboroCity_Gym_EventScript_RoxanneDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Gym_EventScript_RoxanneRematch
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, RustboroCity_Gym_EventScript_GiveRockTomb
	msgbox RustboroCity_Gym_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneDefeated::
	message RustboroCity_Gym_Text_ReceivedStoneBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox RustboroCity_Gym_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_RUSTBORO_GYM
	setflag FLAG_BADGE01_GET
	setvar VAR_RUSTBORO_CITY_STATE, 1
	addvar VAR_PETALBURG_GYM_STATE, 1
	setvar VAR_0x8008, 1
	call Common_EventScript_SetGymTrainers
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto RustboroCity_Gym_EventScript_GiveRockTomb
	end

RustboroCity_Gym_EventScript_GiveRockTomb::
	giveitem ITEM_TM_ROCK_TOMB
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_ROCK_TOMB
	msgbox RustboroCity_Gym_Text_ExplainRockTomb, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneRematch::
	trainerbattle_rematch_double TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxannePreRematch, RustboroCity_Gym_Text_RoxanneRematchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox RustboroCity_Gym_Text_RoxannePostRematch, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Josh::
	trainerbattle_single TRAINER_JOSH, RustboroCity_Gym_Text_JoshIntro, RustboroCity_Gym_Text_JoshDefeat
	msgbox RustboroCity_Gym_Text_JoshPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Tommy::
	trainerbattle_single TRAINER_TOMMY, RustboroCity_Gym_Text_TommyIntro, RustboroCity_Gym_Text_TommyDefeat
	msgbox RustboroCity_Gym_Text_TommyPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Marc::
	trainerbattle_single TRAINER_MARC, RustboroCity_Gym_Text_MarcIntro, RustboroCity_Gym_Text_MarcDefeat
	msgbox RustboroCity_Gym_Text_MarcPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_RUSTBORO_GYM, RustboroCity_Gym_EventScript_GymGuidePostVictory
	msgbox RustboroCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_GymGuidePostVictory::
	msgbox RustboroCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_GymStatueCertified::
	msgbox RustboroCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_GymStatue::
	msgbox RustboroCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_RegisterRoxanne::
	lockall
	pokenavcall RustboroCity_Gym_Text_RoxanneRegisterCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox RustboroCity_Gym_Text_RegisteredRoxanne, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_MATCH_CALL
	clearflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	releaseall
	end

RustboroCity_Gym_Text_GymGuideAdvice:
	.string "Ehilà, come va?\p"
	.string "Ascolta, amico mio!\n"
	.string "Vuoi diventare Campione?\p"
	.string "Io non sono un Allenatore, ma di\n"
	.string "consigli vincenti ne ho a bizzeffe.\p"
	.string "Allora è deciso! Puntiamo insieme\n"
	.string "al titolo di Campione!\p"
	.string "Il compito di un Allenatore è\n"
	.string "collezionare Medaglie, giusto?\p"
	.string "Ma i Capipalestra non sono\n"
	.string "avversari facili!\p"
	.string "Ed è qui che entro in gioco io,\n"
	.string "con i miei consigli da esperto!\p"
	.string "Petra, la Capopalestra, usa\n"
	.string "Pokémon di tipo Roccia.\p"
	.string "Il tipo Roccia è molto resistente,\n"
	.string "ma non sopporta le mosse di tipo\l"
	.string "Acqua e di tipo Erba.\p"
	.string "Il tuo compagno è di tipo Acqua o\n"
	.string "Erba? Allora parti avvantaggiato!\p"
	.string "Se batti la Capopalestra, torna\n"
	.string "a trovarmi.\p"
	.string "Forza, dacci dentro!$"

RustboroCity_Gym_Text_GymGuidePostVictory:
	.string "Wow! Che vittoria mozzafiato!\n"
	.string "Il mio tifo ha funzionato, eh?\p"
	.string "Grande!\n"
	.string "Controlla la tua Scheda Allenatore.\p"
	.string "La Medaglia Pietra che hai vinto\n"
	.string "dovrebbe essere registrata lì.\p"
	.string "In altre parole…\p"
	.string "Hai salito il primo gradino della\n"
	.string "scala che porta al titolo di Campione!\p"
	.string "Dev'essere una sensazione fantastica!$"

RustboroCity_Gym_Text_JoshIntro:
	.string "Non sottovalutare noi della Palestra!\n"
	.string "Ti faccio vedere chi è il migliore!$"

RustboroCity_Gym_Text_JoshDefeat:
	.string "Eri troppo forte per me…$"

RustboroCity_Gym_Text_JoshPostBattle:
	.string "Non hai ancora visto la vera,\n"
	.string "terribile potenza del tipo Roccia!$"

RustboroCity_Gym_Text_TommyIntro:
	.string "Se non riesci a battere me, contro\n"
	.string "Petra non hai nessuna speranza!$"

RustboroCity_Gym_Text_TommyDefeat:
	.string "Wow! Hai del potenziale!$"

RustboroCity_Gym_Text_TommyPostBattle:
	.string "Petra è dieci volte più forte\n"
	.string "di me, come minimo.\p"
	.string "Tieni gli occhi bene aperti!$"

RustboroCity_Gym_Text_MarcIntro:
	.string "Niente vette da scalare in città?\n"
	.string "Pazienza!\p"
	.string "I miei Pokémon Roccia si scatenano\n"
	.string "anche qui!$"

RustboroCity_Gym_Text_MarcDefeat:
	.string "Accidenti, accidenti!\n"
	.string "Questo sfidante è un osso duro!$"

RustboroCity_Gym_Text_MarcPostBattle:
	.string "Tanto di cappello alla nostra\n"
	.string "Capopalestra.\p"
	.string "Così giovane, ha scelto il tipo Roccia\n"
	.string "con testa e buon senso.\p"
	.string "E colleziona perfino i fossili\n"
	.string "arrivati dai Varchi!$"

RustboroCity_Gym_Text_RoxanneIntro:
	.string "Buongiorno! Sono Petra, Capopalestra\n"
	.string "di Ferrugipoli.\p"
	.string "Insegno alla Scuola Allenatori, e sono\n"
	.string "diventata Capopalestra per mettere in\l"
	.string "pratica ciò che ho studiato.\p"
	.string "Lezione numero uno: le rocce\n"
	.string "ricordano tutto.\p"
	.string "Vediamo cosa ricorderanno di te!\p"
	.string "Mostrami come lotti, e con quali\n"
	.string "Pokémon. Prego!$"

RustboroCity_Gym_Text_RoxanneDefeat:
	.string "Dunque…\n"
	.string "Ho perso…\p"
	.string "A quanto pare, ho ancora molto\n"
	.string "da imparare… e da ripassare.\p"
	.string "Ho capito.\p"
	.string "Secondo le regole della Lega\n"
	.string "Pokémon, chi sconfigge un\l"
	.string "Capopalestra riceve questa.\p"
	.string "Ti prego, accetta la Medaglia Pietra\n"
	.string "ufficiale della Lega Pokémon.$"

RustboroCity_Gym_Text_ReceivedStoneBadge:
	.string "{PLAYER} ha ricevuto la Medaglia\n"
	.string "Pietra da Petra!$"

RustboroCity_Gym_Text_StoneBadgeInfoTakeThis:
	.string "Con la Medaglia Pietra, i Pokémon\n"
	.string "fino al livello 20 ti obbediranno.\p"
	.string "Inoltre, potranno usare la mossa MN\n"
	.string "Taglio anche fuori dalla lotta.\p"
	.string "Prendi anche questa, per favore.$"

RustboroCity_Gym_Text_ExplainRockTomb:
	.string "Quella Macchina Tecnica, la MT39,\n"
	.string "contiene Rocciotomba.\p"
	.string "Infligge danni facendo cadere massi\n"
	.string "e in più riduce la Velocità.\p"
	.string "Una MT insegna all'istante la sua\n"
	.string "mossa a un Pokémon.\p"
	.string "Ricorda: ogni MT si usa una volta\n"
	.string "sola, quindi pensaci bene!$"

RustboroCity_Gym_Text_RoxannePostBattle:
	.string "Sei davvero forte. Dovresti sfidare\n"
	.string "anche gli altri Capipalestra.\p"
	.string "Lottando contro tanti Allenatori,\n"
	.string "imparerai moltissime cose.\p"
	.string "E ricorda: ripassare è importante!$"

RustboroCity_Gym_Text_GymStatue:
	.string "Palestra Pokémon di Ferrugipoli$"

RustboroCity_Gym_Text_GymStatueCertified:
	.string "Palestra Pokémon di Ferrugipoli\p"
	.string "Allenatori certificati da Petra:\n"
	.string "{PLAYER}$"

RustboroCity_Gym_Text_RoxanneRegisterCall:
	.string "… … … … … …\n"
	.string "… … … … … Bip!\p"
	.string "Petra: Pronto, {PLAYER}?\n"
	.string "Sono Petra, di Ferrugipoli.\p"
	.string "Rudi mi ha detto che stai scalando\n"
	.string "le classifiche degli Allenatori\l"
	.string "a una velocità incredibile.\p"
	.string "Beh, preferisco non pensare\n"
	.string "di non poterti battere.\p"
	.string "Prometto che rimetterò in forma\n"
	.string "la mia Palestra, così potrai\l"
	.string "sfidarci di nuovo.\p"
	.string "Spero che aspetterai quel momento\n"
	.string "quanto lo aspetto io!\p"
	.string "Intanto… io ripasso!\p"
	.string "… … … … … …\n"
	.string "… … … … … Clic!$"

RustboroCity_Gym_Text_RegisteredRoxanne:
	.string "Capopalestra Petra registrata\n"
	.string "nel PokéNav.$"

RustboroCity_Gym_Text_RoxannePreRematch:
	.string "Petra: Che piacere rivederti!\n"
	.string "Sono Petra, la Capopalestra.\p"
	.string "Da allora avremo affrontato entrambi\n"
	.string "moltissime lotte.\p"
	.string "Vediamo quanto siamo migliorati.\n"
	.string "Lottiamo! È ora di ripassare!$"

RustboroCity_Gym_Text_RoxanneRematchDefeat:
	.string "Grr…\n"
	.string "Ho perso di nuovo…$"

RustboroCity_Gym_Text_RoxannePostRematch:
	.string "Petra: Ho ancora tanto da imparare\n"
	.string "sulle lotte Pokémon.\p"
	.string "E saperlo me le fa amare\n"
	.string "ancora di più!$"

RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons:
	.string "Petra: Che piacere rivederti!\n"
	.string "Sono Petra, la Capopalestra.\p"
	.string "Da allora avremo affrontato entrambi\n"
	.string "moltissime lotte.\p"
	.string "Vorrei vedere quanto siamo\n"
	.string "migliorati…\p"
	.string "Oh, no…\n"
	.string "Hai con te un solo Pokémon?\p"
	.string "Torna con almeno un altro\n"
	.string "Pokémon, per favore.$"
