Route110_TrickHousePuzzle4_MapScripts::
	.byte 0

Route110_TrickHousePuzzle4_EventScript_Scroll::
	lockall
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_4_STATE, 0, Route110_TrickHousePuzzle4_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle4_EventScript_FoundScroll::
	setvar VAR_TRICK_HOUSE_PUZZLE_4_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

Route110_TrickHousePuzzle4_EventScript_Cora::
	trainerbattle_single TRAINER_CORA, Route110_TrickHousePuzzle4_Text_CoraIntro, Route110_TrickHousePuzzle4_Text_CoraDefeat
	msgbox Route110_TrickHousePuzzle4_Text_CoraPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle4_EventScript_Yuji::
	trainerbattle_single TRAINER_YUJI, Route110_TrickHousePuzzle4_Text_YujiIntro, Route110_TrickHousePuzzle4_Text_YujiDefeat
	msgbox Route110_TrickHousePuzzle4_Text_YujiPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle4_EventScript_Paula::
	trainerbattle_single TRAINER_PAULA, Route110_TrickHousePuzzle4_Text_PaulaIntro, Route110_TrickHousePuzzle4_Text_PaulaDefeat
	msgbox Route110_TrickHousePuzzle4_Text_PaulaPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle4_Text_WroteSecretCodeLockOpened:
	.string "{PLAYER} ha scritto il codice segreto\n"
	.string "sulla porta.\p"
	.string "“Il Maestro Trucco è un mito.”\n"
	.string "… … … … … … … …\p"
	.string "La serratura è scattata!$"

Route110_TrickHousePuzzle4_Text_CoraIntro:
	.string "Che fatica ragionare! Io volevo\n"
	.string "solo lottare!$"

Route110_TrickHousePuzzle4_Text_CoraDefeat:
	.string "Ho perso, ma lottare resta la cosa\n"
	.string "che amo di più!$"

Route110_TrickHousePuzzle4_Text_CoraPostBattle:
	.string "Non sei d'accordo? Andresti ovunque,\n"
	.string "pur di trovare Allenatori da sfidare!$"

Route110_TrickHousePuzzle4_Text_YujiIntro:
	.string "Ah! Massi come questi li sposto\n"
	.string "con un dito solo!$"

Route110_TrickHousePuzzle4_Text_YujiDefeat:
	.string "I massi li sposto, ma l'enigma non\n"
	.string "lo risolvo…$"

Route110_TrickHousePuzzle4_Text_YujiPostBattle:
	.string "Non bastano i muscoli…\n"
	.string "Bisogna allenare anche la testa!$"

Route110_TrickHousePuzzle4_Text_PaulaIntro:
	.string "La Casa dei Trucchi diventa sempre\n"
	.string "più ingannevole, eh?$"

Route110_TrickHousePuzzle4_Text_PaulaDefeat:
	.string "Aaah!$"

Route110_TrickHousePuzzle4_Text_PaulaPostBattle:
	.string "Qualcuno è mai arrivato in fondo?$"

