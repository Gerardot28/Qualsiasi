Route110_TrickHousePuzzle6_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route110_TrickHousePuzzle6_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Route110_TrickHousePuzzle6_OnWarp
	.byte 0

Route110_TrickHousePuzzle6_OnTransition:
	special RotatingGate_InitPuzzle
	end

Route110_TrickHousePuzzle6_OnWarp:
	map_script_2 VAR_TEMP_0, VAR_TEMP_0, Route110_TrickHousePuzzle6_EventScript_InitPuzzle
	.2byte 0

@ NOTE: This rotating gate puzzle makes use of VAR_TEMP_0 - VAR_TEMP_5
Route110_TrickHousePuzzle6_EventScript_InitPuzzle::
	special RotatingGate_InitPuzzleAndGraphics
	end

Route110_TrickHousePuzzle6_EventScript_Scroll::
	lockall
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_6_STATE, 0, Route110_TrickHousePuzzle6_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle6_EventScript_FoundScroll::
	setvar VAR_TRICK_HOUSE_PUZZLE_6_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

Route110_TrickHousePuzzle6_EventScript_Sophia::
	trainerbattle_single TRAINER_SOPHIA, Route110_TrickHousePuzzle6_Text_SophiaIntro, Route110_TrickHousePuzzle6_Text_SophiaDefeat
	msgbox Route110_TrickHousePuzzle6_Text_SophiaPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle6_EventScript_Benny::
	trainerbattle_single TRAINER_BENNY, Route110_TrickHousePuzzle6_Text_BennyIntro, Route110_TrickHousePuzzle6_Text_BennyDefeat
	msgbox Route110_TrickHousePuzzle6_Text_BennyPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle6_EventScript_Sebastian::
	trainerbattle_single TRAINER_SEBASTIAN, Route110_TrickHousePuzzle6_Text_SebastianIntro, Route110_TrickHousePuzzle6_Text_SebastianDefeat
	msgbox Route110_TrickHousePuzzle6_Text_SebastianPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle6_Text_WroteSecretCodeLockOpened:
	.string "{PLAYER} ha scritto il codice segreto\n"
	.string "sulla porta.\p"
	.string "“Il Maestro Trucco è la mia vita.”\n"
	.string "… … … … … … … …\p"
	.string "La serratura è scattata!$"

Route110_TrickHousePuzzle6_Text_SophiaIntro:
	.string "Quando ho sentito di una casa strana,\n"
	.string "dovevo per forza venire a vederla.$"

Route110_TrickHousePuzzle6_Text_SophiaDefeat:
	.string "Ho scoperto un Allenatore tosto!$"

Route110_TrickHousePuzzle6_Text_SophiaPostBattle:
	.string "Mi sto proprio divertendo a esplorare\n"
	.string "questo posto.\p"
	.string "È una sfida che vale la pena\n"
	.string "ripetere!$"

Route110_TrickHousePuzzle6_Text_BennyIntro:
	.string "E se i miei Pokémon Volante mi\n"
	.string "portassero in volo oltre il muro…?$"

Route110_TrickHousePuzzle6_Text_BennyDefeat:
	.string "Gwaaah! Mi hanno tarpato le ali!$"

Route110_TrickHousePuzzle6_Text_BennyPostBattle:
	.string "Eheheh… Ho perso perché cercavo\n"
	.string "di barare, vero?$"

Route110_TrickHousePuzzle6_Text_SebastianIntro:
	.string "Mi gira la testa con queste porte\n"
	.string "girevoli…$"

Route110_TrickHousePuzzle6_Text_SebastianDefeat:
	.string "Gira tutto, gira tutto…\n"
	.string "Non ce la faccio più…$"

Route110_TrickHousePuzzle6_Text_SebastianPostBattle:
	.string "Tu sembri non accorgerti di nulla.\n"
	.string "O hai solo una gran faccia da poker?$"

