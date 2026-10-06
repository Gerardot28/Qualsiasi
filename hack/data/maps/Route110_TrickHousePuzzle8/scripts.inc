Route110_TrickHousePuzzle8_MapScripts::
	.byte 0

Route110_TrickHousePuzzle8_EventScript_Scroll::
	lockall
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_8_STATE, 0, Route110_TrickHousePuzzle8_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle8_EventScript_FoundScroll::
	setvar VAR_TRICK_HOUSE_PUZZLE_8_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

Route110_TrickHousePuzzle8_EventScript_Vincent::
	trainerbattle_single TRAINER_VINCENT, Route110_TrickHousePuzzle8_Text_VincentIntro, Route110_TrickHousePuzzle8_Text_VincentDefeat
	msgbox Route110_TrickHousePuzzle8_Text_VincentPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle8_EventScript_Keira::
	trainerbattle_single TRAINER_KEIRA, Route110_TrickHousePuzzle8_Text_KeiraIntro, Route110_TrickHousePuzzle8_Text_KeiraDefeat
	msgbox Route110_TrickHousePuzzle8_Text_KeiraPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle8_EventScript_Leroy::
	trainerbattle_single TRAINER_LEROY, Route110_TrickHousePuzzle8_Text_LeroyIntro, Route110_TrickHousePuzzle8_Text_LeroyDefeat
	msgbox Route110_TrickHousePuzzle8_Text_LeroyPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle8_Text_WroteSecretCodeLockOpened::
	.string "{PLAYER} ha scritto il codice segreto\n"
	.string "sulla porta.\p"
	.string "“Maestro Trucco, io ti adoro.”\n"
	.string "… … … … … … … …\p"
	.string "La serratura è scattata!$"

Route110_TrickHousePuzzle8_Text_VincentIntro:
	.string "Pochi Allenatori sono arrivati\n"
	.string "fin qui.$"

Route110_TrickHousePuzzle8_Text_VincentDefeat:
	.string "Vuol dire che sei tosto anche tu…$"

Route110_TrickHousePuzzle8_Text_VincentPostBattle:
	.string "Hai battuto il Campione della Lega\n"
	.string "Pokémon? Questo è troppo!$"

Route110_TrickHousePuzzle8_Text_KeiraIntro:
	.string "Considerati fortunato: lotterai\n"
	.string "contro di me!$"

Route110_TrickHousePuzzle8_Text_KeiraDefeat:
	.string "Non è possibile!\n"
	.string "Io non posso perdere!$"

Route110_TrickHousePuzzle8_Text_KeiraPostBattle:
	.string "È un miracolo che tu mi abbia battuta.\n"
	.string "Puoi vantartene in giro.$"

Route110_TrickHousePuzzle8_Text_LeroyIntro:
	.string "Anche tu stai sudando sette camicie\n"
	.string "con la Casa dei Trucchi, eh?$"

Route110_TrickHousePuzzle8_Text_LeroyDefeat:
	.string "Capisco…\n"
	.string "Hai uno stile straordinario.$"

Route110_TrickHousePuzzle8_Text_LeroyPostBattle:
	.string "Uno come te farà felice\n"
	.string "il Maestro Trucco.$"

