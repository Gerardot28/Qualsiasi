BattleFrontier_OutsideEast_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, BattleFrontier_OutsideEast_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_OutsideEast_OnTransition
	.byte 0

BattleFrontier_OutsideEast_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, BattleFrontier_OutsideEast_EventScript_TryRemoveSudowoodo
	end

BattleFrontier_OutsideEast_EventScript_TryRemoveSudowoodo::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

BattleFrontier_OutsideEast_OnTransition:
	setvar VAR_BRAVO_TRAINER_BATTLE_TOWER_ON, 0
	setflag FLAG_HIDE_BATTLE_TOWER_REPORTER
	call_if_unset FLAG_DEFEATED_SUDOWOODO, BattleFrontier_OutsideEast_EventScript_ShowSudowoodo
	end

BattleFrontier_OutsideEast_EventScript_ShowSudowoodo::
	clearflag FLAG_HIDE_BATTLE_FRONTIER_SUDOWOODO
	return

BattleFrontier_OutsideEast_EventScript_BattleTowerSign::
	msgbox BattleFrontier_OutsideEast_Text_BattleTowerSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattlePalaceSign::
	msgbox BattleFrontier_OutsideEast_Text_BattlePalaceSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattleArenaSign::
	msgbox BattleFrontier_OutsideEast_Text_BattleArenaSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattlePyramidSign::
	msgbox BattleFrontier_OutsideEast_Text_BattlePyramidSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_NinjaBoy::
	msgbox BattleFrontier_OutsideEast_Text_BattleTowerFeelsSpecial, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Man1::
	msgbox BattleFrontier_OutsideEast_Text_ConquerLeagueAndFrontier, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Hiker::
	msgbox BattleFrontier_OutsideEast_Text_PyramidTooHarsh, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_HexManiac::
	msgbox BattleFrontier_OutsideEast_Text_ThriveInDarkness, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_BlackBelt1::
	msgbox BattleFrontier_OutsideEast_Text_PutTogetherUltimateTeam, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Cook::
	lock
	msgbox BattleFrontier_OutsideEast_Text_BelieveInYouBuddy, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox BattleFrontier_OutsideEast_Text_ZigzagoonLooksVacant, MSGBOX_DEFAULT
	waitmoncry
	release
	end

BattleFrontier_OutsideEast_EventScript_RichBoy::
	msgbox BattleFrontier_OutsideEast_Text_PeopleCallMeBusybody, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_ExpertF::
	msgbox BattleFrontier_OutsideEast_Text_OnceBeatGymLeader, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_TriathleteF::
	msgbox BattleFrontier_OutsideEast_Text_FastOnBikeAndBattles, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Twin::
	msgbox BattleFrontier_OutsideEast_Text_BetterThanDaddyAtPokemon, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Man2::
	lock
	msgbox BattleFrontier_OutsideEast_Text_GoRackUpSomeWinsForDaddy, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_TriathleteM::
	msgbox BattleFrontier_OutsideEast_Text_DidScottBringYouHere, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Sudowoodo::
	lock
	faceplayer
	waitse
	playse SE_SUDOWOODO_SHAKE
	applymovement LOCALID_FRONTIER_SUDOWOODO, BattleFrontier_OutsideEast_Movement_SudowoodoShake
	waitmovement 0
	release
	end

BattleFrontier_OutsideEast_EventScript_WaterSudowoodo::
	lock
	faceplayer
	special DoWateringBerryTreeAnim
	waitse
	playse SE_SUDOWOODO_SHAKE
	applymovement LOCALID_FRONTIER_SUDOWOODO, BattleFrontier_OutsideEast_Movement_SudowoodoShake
	waitmovement 0
	msgbox gText_Sudowoodo_Attacked, MSGBOX_DEFAULT
	closemessage
	waitse
	playmoncry SPECIES_SUDOWOODO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_FRONTIER_SUDOWOODO
	setwildbattle SPECIES_SUDOWOODO, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	setflag FLAG_DEFEATED_SUDOWOODO
	release
	end

BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo::
	setflag FLAG_DEFEATED_SUDOWOODO
	goto Common_EventScript_RemoveStaticPokemon
	end

BattleFrontier_OutsideEast_Movement_SudowoodoShake:
	face_right
	delay_8
	face_down
	delay_8
	face_left
	delay_8
	face_down
	delay_8
	face_right
	delay_8
	face_down
	delay_8
	face_left
	delay_8
	face_down
	step_end

BattleFrontier_OutsideEast_EventScript_Maniac1::
	msgbox BattleFrontier_OutsideEast_Text_HeardPrettyGirlAtBattleArena, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Girl::
	msgbox BattleFrontier_OutsideEast_Text_SometimesImportantOldManInThere, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_PsychicM::
	msgbox BattleFrontier_OutsideEast_Text_LegendOfBattlePyramid, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Man3::
	msgbox BattleFrontier_OutsideEast_Text_GotWipedOut, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Woman1::
	msgbox BattleFrontier_OutsideEast_Text_ToughTrainerInBattleTower, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Woman2::
	msgbox BattleFrontier_OutsideEast_Text_EnoughBattlePointsForDoll, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_BlackBelt2::
	msgbox BattleFrontier_OutsideEast_Text_LikeToHaveNameRecordedHere, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_Maniac2::
	lock
	msgbox BattleFrontier_OutsideEast_Text_CanDoAnythingWithYou, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_Woman3::
	lock
	msgbox BattleFrontier_OutsideEast_Text_PowerOfOurLoveWillOvercome, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_RankingHallSign::
	msgbox BattleFrontier_OutsideEast_Text_RankingHallSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_ExchangeCornerSign::
	msgbox BattleFrontier_OutsideEast_Text_ExchangeCornerSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_Gentleman::
	msgbox BattleFrontier_OutsideEast_Text_FrontierNotExclusivelyForToughTrainers, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_EventScript_OldWoman::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SUDOWOODO, BattleFrontier_OutsideEast_EventScript_OldWomanSudowoodoGone
	msgbox BattleFrontier_OutsideEast_Text_OddTreeHereSeemsToWiggle, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_OldWomanSudowoodoGone::
	msgbox BattleFrontier_OutsideEast_Text_OnceAnOddTreePastHere, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_Camper::
	msgbox BattleFrontier_OutsideEast_Text_StickyMonWithLongTail, MSGBOX_NPC
	end

BattleFrontier_OutsideEast_Text_BattleTowerSign:
	.string "Questa è la Torre Lotta!\n"
	.string "Allunga la tua serie di vittorie e\l"
	.string "diventa l'Allenatore più forte!$"

BattleFrontier_OutsideEast_Text_BattlePalaceSign:
	.string "Questo è il Palazzo Lotta!\n"
	.string "Occhi puntati sulle lotte Pokémon!$"

BattleFrontier_OutsideEast_Text_BattleArenaSign:
	.string "Questo è il Dojo Lotta!\n"
	.string "Qui si sfidano le squadre più toste!$"

BattleFrontier_OutsideEast_Text_BattlePyramidSign:
	.string "Questa è la Piramide Lotta!\n"
	.string "Fatti strada nella Spedizione Lotta!$"

BattleFrontier_OutsideEast_Text_RankingHallSign:
	.string "Sala dei Record del Parco Lotta\n"
	.string "Punta dritto a nuovi record!$"

BattleFrontier_OutsideEast_Text_ExchangeCornerSign:
	.string "Angolo Scambi Punti Lotta\n"
	.string "Scambia qui i tuoi Punti Lotta!$"

BattleFrontier_OutsideEast_Text_BattleTowerFeelsSpecial:
	.string "Uaaah!\n"
	.string "È gigantesca!\p"
	.string "La Torre Lotta ha qualcosa di speciale.\n"
	.string "Non è come gli altri edifici.$"

BattleFrontier_OutsideEast_Text_ConquerLeagueAndFrontier:
	.string "Il mio grande sogno è conquistare\n"
	.string "sia il Parco Lotta che la Lega\l"
	.string "Pokémon.\p"
	.string "E se il mio sogno si avvera…\p"
	.string "Io… io chiederò a Mimì, la mia\n"
	.string "vicina di casa, se vuole essere\l"
	.string "mia amica!$"

BattleFrontier_OutsideEast_Text_PyramidTooHarsh:
	.string "La Piramide Lotta è troppo dura!\n"
	.string "Non riesco proprio ad arrivare in cima!\p"
	.string "Non ho più idee… Da bravo Montanaro,\n"
	.string "potrei sempre scalarla da fuori…$"

BattleFrontier_OutsideEast_Text_ThriveInDarkness:
	.string "L'oscurità è il mio elemento…\n"
	.string "Sì… E cosa può essere degno di me?\l"
	.string "Nient'altro che la Piramide Lotta…\p"
	.string "Che ne dici di vagare nelle tenebre,\n"
	.string "in preda alla disperazione più nera?$"

BattleFrontier_OutsideEast_Text_PutTogetherUltimateTeam:
	.string "Una settimana senza dormire, ma ho\n"
	.string "messo insieme la squadra definitiva!\p"
	.string "Perdere? Non esiste, parola di\n"
	.string "Cinturanera! Dojo Lotta, arriviamo!$"

BattleFrontier_OutsideEast_Text_DidScottBringYouHere:
	.string "Anche tu sei qui per merito di Scott?\n"
	.string "Ma chi è davvero quel tipo?\p"
	.string "Pare che sappia riconoscere al volo\n"
	.string "gli Allenatori di talento, però…\p"
	.string "Non l'ho mai visto lottare di persona.\n"
	.string "Neanche una volta.$"

BattleFrontier_OutsideEast_Text_BelieveInYouBuddy:
	.string "Zig!\n"
	.string "Credo in te, piccolo mio!\p"
	.string "Se vinci, il tuo cuoco personale ti\n"
	.string "preparerà tutte le leccornie che vuoi!$"

BattleFrontier_OutsideEast_Text_ZigzagoonLooksVacant:
	.string "Zigzagoon ha lo sguardo perso…$"

BattleFrontier_OutsideEast_Text_PeopleCallMeBusybody:
	.string "Tutti dicono che sono un ficcanaso,\n"
	.string "ma non ci posso fare niente.\p"
	.string "Hai i capelli tutti sparati in aria!\n"
	.string "Oh, no, una cartaccia per terra!\l"
	.string "Ops, è quasi ora di cena!\p"
	.string "Non so se riuscirò a starmene buono\n"
	.string "a guardare, al Palazzo Lotta…$"

BattleFrontier_OutsideEast_Text_OnceBeatGymLeader:
	.string "Ora non sembrerò granché, ma un\n"
	.string "tempo ho battuto un Capopalestra.\p"
	.string "Chissà… forse dovrei tentare di\n"
	.string "nuovo la strada della gloria.$"

BattleFrontier_OutsideEast_Text_FastOnBikeAndBattles:
	.string "Sono velocissima in Bici, e lo stesso\n"
	.string "vale per il mio modo di lottare.\p"
	.string "Vinco gli incontri in un batter\n"
	.string "d'occhio!$"

BattleFrontier_OutsideEast_Text_BetterThanDaddyAtPokemon:
	.string "Con i Pokémon sono più brava\n"
	.string "del mio papà.$"

BattleFrontier_OutsideEast_Text_GoRackUpSomeWinsForDaddy:
	.string "Vai, tesoro mio dolcissimo!\n"
	.string "Fai collezione di vittorie per papà!\p"
	.string "Sei tutta tua madre,\n"
	.string "quindi li stenderai tutti!$"

BattleFrontier_OutsideEast_Text_HeardPrettyGirlAtBattleArena:
	.string "Non è stato facile arrivare fin qui,\n"
	.string "ma avevo sentito parlare di una\l"
	.string "ragazza bellissima al Dojo Lotta.\p"
	.string "E invece di lei, sai che cosa\n"
	.string "ho trovato?\p"
	.string "Un'orda di lottatori sudati,\n"
	.string "puzzolenti e primitivi!$"

BattleFrontier_OutsideEast_Text_SometimesImportantOldManInThere:
	.string "Ogni tanto vedo entrare lì dentro un\n"
	.string "vecchietto dall'aria importantissima.\p"
	.string "Ha dei Pokémon davvero pazzeschi.\n"
	.string "Però sembra tanto gentile.$"

BattleFrontier_OutsideEast_Text_LegendOfBattlePyramid:
	.string "La conosci?\n"
	.string "La leggenda della Piramide Lotta?\p"
	.string "Quando giungerà un Allenatore sicuro\n"
	.string "di sé, proteso verso la vetta dorata,\l"
	.string "apparirà un uomo fra gli uomini.\p"
	.string "Non conosci questa leggenda?\n"
	.string "Beh, ovvio che no!\l"
	.string "Me la sono appena inventata!\p"
	.string "Che cosa significa, dici?\n"
	.string "Questo, amico mio, nemmeno i miei\l"
	.string "poteri psichici lo sanno!$"

BattleFrontier_OutsideEast_Text_GotWipedOut:
	.string "Accidenti! Accidenti!\n"
	.string "Non avevo mai perso, mai!\p"
	.string "E invece mi hanno spazzato via\n"
	.string "senza la minima pietà!\p"
	.string "E poi mi sento dire che non ho\n"
	.string "talento?! Accidenti!$"

BattleFrontier_OutsideEast_Text_ToughTrainerInBattleTower:
	.string "Questo tizio si è imbattuto in un\n"
	.string "Allenatore spaventosamente forte\l"
	.string "durante la sfida della Torre Lotta.\p"
	.string "Guardalo adesso…\p"
	.string "Se mai decidessi di sfidare la\n"
	.string "Torre Lotta, sta' molto attento.$"

BattleFrontier_OutsideEast_Text_EnoughBattlePointsForDoll:
	.string "Sì!\n"
	.string "Finalmente ho abbastanza Punti Lotta!\p"
	.string "Credo che li scambierò con un'altra\n"
	.string "bambola di peluche gigante!$"

BattleFrontier_OutsideEast_Text_LikeToHaveNameRecordedHere:
	.string "Uh, sì, eccoci qua!\n"
	.string "Questa è la Sala dei Record!\p"
	.string "Voglio passare alla storia come un\n"
	.string "supercampione e vedere il mio nome\l"
	.string "scritto qui, per i posteri.$"

BattleFrontier_OutsideEast_Text_CanDoAnythingWithYou:
	.string "Con te al mio fianco, posso fare tutto.$"

BattleFrontier_OutsideEast_Text_PowerOfOurLoveWillOvercome:
	.string "Oh, tesoro, sei così meraviglioso!\p"
	.string "Oh, non ce la faccio più ad aspettare!\p"
	.string "Andiamo subito in una Sala Lotta\n"
	.string "Multipla, in questo preciso istante!\p"
	.string "Se facciamo coppia nel Salone Lotta,\n"
	.string "la forza del nostro amore travolgerà\l"
	.string "chiunque incontreremo.\p"
	.string "Ma sì, tesoro, davanti a noi cadranno\n"
	.string "tutti come tessere del domino!$"

BattleFrontier_OutsideEast_Text_FrontierNotExclusivelyForToughTrainers:
	.string "Il Parco Lotta non è riservato\n"
	.string "soltanto agli Allenatori più forti.\p"
	.string "A quanto pare, sono state invitate\n"
	.string "molte persone dalle doti speciali.\p"
	.string "Quell'uomo, Scott…\p"
	.string "Forse ha costruito questo posto per\n"
	.string "un obiettivo ben più grande…\p"
	.string "Si mormora che aspetti Allenatori\n"
	.string "da oltre il Grande Varco…$"

BattleFrontier_OutsideEast_Text_OddTreeHereSeemsToWiggle:
	.string "Scusami, giovanotto.\n"
	.string "Hai una buona vista?\p"
	.string "Più avanti c'è un albero strano, e ai\n"
	.string "miei occhi stanchi pare che si muova.$"

BattleFrontier_OutsideEast_Text_OnceAnOddTreePastHere:
	.string "Scusami, giovanotto.\n"
	.string "Hai una buona memoria?\p"
	.string "Ho come l'impressione che una volta,\n"
	.string "più avanti, ci fosse un albero strano.$"

BattleFrontier_OutsideEast_Text_StickyMonWithLongTail:
	.string "L'ho…\n"
	.string "L'ho visto!\p"
	.string "Più avanti c'era un Pokémon tutto\n"
	.string "impiastricciato, con una lunga coda!\p"
	.string "Si nascondeva sotto un masso e\n"
	.string "non smetteva di fissarmi!$"
