Route110_TrickHousePuzzle1_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Route110_TrickHousePuzzle1_OnLoad
	.byte 0

Route110_TrickHousePuzzle1_OnLoad:
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_1_STATE, 2, Route110_TrickHousePuzzle1_EventScript_OpenDoor
	end

Route110_TrickHousePuzzle1_EventScript_OpenDoor::
	setmetatile 13, 1, METATILE_TrickHousePuzzle_Stairs_Down, FALSE
	end

Route110_TrickHousePuzzle1_EventScript_Scroll::
	lockall
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_1_STATE, 0, Route110_TrickHousePuzzle1_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle1_EventScript_FoundScroll::
	setvar VAR_TRICK_HOUSE_PUZZLE_1_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

Route110_TrickHousePuzzle1_EventScript_Sally::
	trainerbattle_single TRAINER_SALLY, Route110_TrickHousePuzzle1_Text_SallyIntro, Route110_TrickHousePuzzle1_Text_SallyDefeat
	msgbox Route110_TrickHousePuzzle1_Text_SallyPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle1_EventScript_Eddie::
	trainerbattle_single TRAINER_EDDIE, Route110_TrickHousePuzzle1_Text_EddieIntro, Route110_TrickHousePuzzle1_Text_EddieDefeat
	msgbox Route110_TrickHousePuzzle1_Text_EddiePostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle1_EventScript_Robin::
	trainerbattle_single TRAINER_ROBIN, Route110_TrickHousePuzzle1_Text_RobinIntro, Route110_TrickHousePuzzle1_Text_RobinDefeat
	msgbox Route110_TrickHousePuzzle1_Text_RobinPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle1_Text_WroteSecretCodeLockOpened::
	.string "{PLAYER} ha scritto il codice segreto\n"
	.string "sulla porta.\p"
	.string "“Il Maestro Trucco è favoloso.”\n"
	.string "… … … … … … … …\p"
	.string "La serratura è scattata!$"

Route110_TrickHousePuzzle1_Text_SallyIntro:
	.string "Mi aprirò la strada a colpi di Taglio,\n"
	.string "la mossa che abbiamo appena imparato!$"

Route110_TrickHousePuzzle1_Text_SallyDefeat:
	.string "Perché prendi tutto così sul serio?$"

Route110_TrickHousePuzzle1_Text_SallyPostBattle:
	.string "Tagliare cespugli non mi stanca mai!\n"
	.string "Zac! Zac! Zac!$"

Route110_TrickHousePuzzle1_Text_EddieIntro:
	.string "Credevo di aver trovato un Varco…\n"
	.string "e invece sono finito in questa casa!$"

Route110_TrickHousePuzzle1_Text_EddieDefeat:
	.string "E adesso ho pure perso…$"

Route110_TrickHousePuzzle1_Text_EddiePostBattle:
	.string "Ho perso la strada, ho perso la lotta…\n"
	.string "e ora mi sono perso pure io! Aiuto!$"

Route110_TrickHousePuzzle1_Text_RobinIntro:
	.string "Ma chi è questo Maestro Trucco?$"

Route110_TrickHousePuzzle1_Text_RobinDefeat:
	.string "Ho perso mentre ero persa nei pensieri!$"

Route110_TrickHousePuzzle1_Text_RobinPostBattle:
	.string "Sei forte!\n"
	.string "Ma tu chi sei, scusa?$"

