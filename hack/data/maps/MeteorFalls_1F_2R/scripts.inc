MeteorFalls_1F_2R_MapScripts::
	.byte 0

MeteorFalls_1F_2R_EventScript_Nicolas::
	trainerbattle_single TRAINER_NICOLAS_1, MeteorFalls_1F_2R_Text_NicolasIntro, MeteorFalls_1F_2R_Text_NicolasDefeat, MeteorFalls_1F_2R_EventScript_RegisterNicolas
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MeteorFalls_1F_2R_EventScript_RematchNicolas
	msgbox MeteorFalls_1F_2R_Text_NicolasPostBattle, MSGBOX_DEFAULT
	release
	end

MeteorFalls_1F_2R_EventScript_RegisterNicolas::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MeteorFalls_1F_2R_Text_NicolasRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_NICOLAS_1
	release
	end

MeteorFalls_1F_2R_EventScript_RematchNicolas::
	trainerbattle_rematch TRAINER_NICOLAS_1, MeteorFalls_1F_2R_Text_NicolasRematchIntro, MeteorFalls_1F_2R_Text_NicolasRematchDefeat
	msgbox MeteorFalls_1F_2R_Text_NicolasPostRematch, MSGBOX_AUTOCLOSE
	end

MeteorFalls_1F_2R_EventScript_John::
	trainerbattle_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JohnIntro, MeteorFalls_1F_2R_Text_JohnDefeat, MeteorFalls_1F_2R_Text_JohnNotEnoughMons, MeteorFalls_1F_2R_EventScript_RegisterJohn
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MeteorFalls_1F_2R_EventScript_RematchJohn
	msgbox MeteorFalls_1F_2R_Text_JohnPostBattle, MSGBOX_DEFAULT
	release
	end

MeteorFalls_1F_2R_EventScript_RegisterJohn::
	msgbox MeteorFalls_1F_2R_Text_JohnRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_JOHN_AND_JAY_1
	release
	end

MeteorFalls_1F_2R_EventScript_RematchJohn::
	trainerbattle_rematch_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JohnRematchIntro, MeteorFalls_1F_2R_Text_JohnRematchDefeat, MeteorFalls_1F_2R_Text_JohnRematchNotEnoughMons
	msgbox MeteorFalls_1F_2R_Text_JohnPostRematch, MSGBOX_AUTOCLOSE
	end

MeteorFalls_1F_2R_EventScript_Jay::
	trainerbattle_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JayIntro, MeteorFalls_1F_2R_Text_JayDefeat, MeteorFalls_1F_2R_Text_JayNotEnoughMons, MeteorFalls_1F_2R_EventScript_RegisterJay
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MeteorFalls_1F_2R_EventScript_RematchJay
	msgbox MeteorFalls_1F_2R_Text_JayPostBattle, MSGBOX_DEFAULT
	release
	end

MeteorFalls_1F_2R_EventScript_RegisterJay::
	msgbox MeteorFalls_1F_2R_Text_JohnRegister, MSGBOX_DEFAULT  @ John speaks for both during register
	register_matchcall TRAINER_JOHN_AND_JAY_1
	release
	end

MeteorFalls_1F_2R_EventScript_RematchJay::
	trainerbattle_rematch_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JayRematchIntro, MeteorFalls_1F_2R_Text_JayRematchDefeat, MeteorFalls_1F_2R_Text_JayRematchNotEnoughMons
	msgbox MeteorFalls_1F_2R_Text_JayPostRematch, MSGBOX_AUTOCLOSE
	end

MeteorFalls_1F_2R_Text_NicolasIntro:
	.string "Noi Domadraghi ci alleniamo qui,\n"
	.string "dove si dice che i primi Draghi\l"
	.string "siano piovuti dal cielo.\p"
	.string "Ci viene perfino il Campione.\n"
	.string "Capisci quanto è speciale, qui?$"

MeteorFalls_1F_2R_Text_NicolasDefeat:
	.string "Argh!\n"
	.string "Non pensavo fossi così forte!$"

MeteorFalls_1F_2R_Text_NicolasPostBattle:
	.string "La strada è ancora lunga e dura.\p"
	.string "Quando diventeremo i migliori,\n"
	.string "io e i miei Pokémon?$"

MeteorFalls_1F_2R_Text_NicolasRegister:
	.string "Voglio capire meglio la tua forza.\n"
	.string "Ti registro nel mio PokéNav.$"

MeteorFalls_1F_2R_Text_NicolasRematchIntro:
	.string "Da quando ci siamo visti, ci siamo\n"
	.string "allenati puntando al primo posto.\p"
	.string "Aiutaci a capire quanto siamo\n"
	.string "diventati forti!$"

MeteorFalls_1F_2R_Text_NicolasRematchDefeat:
	.string "Argh!\n"
	.string "Sei ancora più forte di prima!$"

MeteorFalls_1F_2R_Text_NicolasPostRematch:
	.string "Si vede che non hai mai smesso di\n"
	.string "allenare i tuoi Pokémon.\p"
	.string "Finché resterai forte, potrò\n"
	.string "diventare più forte anch'io!$"

MeteorFalls_1F_2R_Text_JohnIntro:
	.string "Ivo: Abbiamo sempre lottato in\n"
	.string "coppia, io e mia moglie.\l"
	.string "E abbiamo fiducia in noi stessi.$"

MeteorFalls_1F_2R_Text_JohnDefeat:
	.string "Ivo: Oh, cielo.\n"
	.string "Abbiamo perso, mia cara.$"

MeteorFalls_1F_2R_Text_JohnPostBattle:
	.string "Ivo: Siamo sposati da cinquant'anni.\p"
	.string "A pensarci bene, non sono ancora\n"
	.string "riuscito a battere mia moglie.$"

MeteorFalls_1F_2R_Text_JohnNotEnoughMons:
	.string "Ivo: Toh, che giovane Allenatore!\p"
	.string "Vuoi lottare con noi? Allora dovrai\n"
	.string "tornare con più Pokémon.$"

MeteorFalls_1F_2R_Text_JohnRegister:
	.string "Ivo: Giovane Allenatore, se capita\n"
	.string "l'occasione, lotterai ancora con noi?$"

MeteorFalls_1F_2R_Text_JayIntro:
	.string "Iva: Siamo sposati da cinquant'anni.\p"
	.string "Il legame che ci unisce non potrà\n"
	.string "mai spezzarsi.$"

MeteorFalls_1F_2R_Text_JayDefeat:
	.string "Iva: Oh, povera me.\n"
	.string "Abbiamo perso, caro marito.$"

MeteorFalls_1F_2R_Text_JayPostBattle:
	.string "Iva: Cinquant'anni di matrimonio…\p"
	.string "Ogni volta che litigavamo, facevamo\n"
	.string "pace con una lotta Pokémon…$"

MeteorFalls_1F_2R_Text_JayNotEnoughMons:
	.string "Iva: Oh, ma che giovane\n"
	.string "Allenatore!\p"
	.string "Se vuoi lottare con noi, dovrai\n"
	.string "tornare con più Pokémon.$"

MeteorFalls_1F_2R_Text_JohnRematchIntro:
	.string "Ivo: Noi due lottiamo sempre in\n"
	.string "coppia, da una vita.\l"
	.string "E la nostra intesa è perfetta.$"

MeteorFalls_1F_2R_Text_JohnRematchDefeat:
	.string "Ivo: Oh, cielo.\n"
	.string "Abbiamo perso di nuovo, cara.$"

MeteorFalls_1F_2R_Text_JohnPostRematch:
	.string "Ivo: Sposati da cinquant'anni…\p"
	.string "A ripensarci, io e la mia cara moglie\n"
	.string "abbiamo lottato tutti i santi giorni…$"

MeteorFalls_1F_2R_Text_JohnRematchNotEnoughMons:
	.string "Ivo: Toh, che giovane Allenatore!\p"
	.string "Vuoi lottare con noi? Allora dovrai\n"
	.string "tornare con più Pokémon.$"

MeteorFalls_1F_2R_Text_JayRematchIntro:
	.string "Iva: Siamo sposati da cinquant'anni.\p"
	.string "Ci siamo sempre sostenuti a vicenda.\n"
	.string "È così che siamo diventati forti.$"

MeteorFalls_1F_2R_Text_JayRematchDefeat:
	.string "Iva: Oh, povera me.\n"
	.string "Abbiamo perso, caro marito.$"

MeteorFalls_1F_2R_Text_JayPostRematch:
	.string "Iva: Cinquant'anni di matrimonio…\n"
	.string "Ne abbiamo viste di tutti i colori,\l"
	.string "e ora perfino i Varchi!\p"
	.string "Spero che continueremo a creare\n"
	.string "tanti bei ricordi insieme.$"

MeteorFalls_1F_2R_Text_JayRematchNotEnoughMons:
	.string "Iva: Oh, ma che giovane\n"
	.string "Allenatore!\p"
	.string "Se vuoi lottare con noi, dovrai\n"
	.string "tornare con più Pokémon.$"

