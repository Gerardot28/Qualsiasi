Route102_MapScripts::
	.byte 0

Route102_EventScript_LittleBoy::
	msgbox Route102_Text_ImNotVeryTall, MSGBOX_NPC
	end

Route102_EventScript_RouteSignOldale::
	msgbox Route102_Text_RouteSignOldale, MSGBOX_SIGN
	end

Route102_EventScript_RouteSignPetalburg::
	msgbox Route102_Text_RouteSignPetalburg, MSGBOX_SIGN
	end

Route102_EventScript_Boy::
	msgbox Route102_Text_CatchWholeBunchOfPokemon, MSGBOX_NPC
	end

Route102_EventScript_Calvin::
	trainerbattle_single TRAINER_CALVIN_1, Route102_Text_CalvinIntro, Route102_Text_CalvinDefeated, Route102_EventScript_CalvinRegisterMatchCallAfterBattle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route102_EventScript_CalvinRematch
	setvar VAR_0x8004, TRAINER_CALVIN_1
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, Route102_EventScript_CalvinTryRegister
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRegisterMatchCallAfterBattle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	goto_if_set FLAG_HAS_MATCH_CALL, Route102_EventScript_CalvinRegisterMatchCall
	release
	end

Route102_EventScript_CalvinRegisterMatchCall::
	msgbox Route102_Text_CalvinRegisterShort, MSGBOX_DEFAULT
	register_matchcall TRAINER_CALVIN_1
	release
	end

Route102_EventScript_CalvinTryRegister::
	goto_if_set FLAG_HAS_MATCH_CALL, Route102_EventScript_CalvinRegister
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRegister::
	msgbox Route102_Text_CalvinRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_CALVIN_1
	release
	end

Route102_EventScript_CalvinRematch::
	trainerbattle_rematch TRAINER_CALVIN_1, Route102_Text_CalvinRematchIntro, Route102_Text_CalvinRematchDefeated
	msgbox Route102_Text_CalvinRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Rick::
	trainerbattle_single TRAINER_RICK, Route102_Text_RickIntro, Route102_Text_RickDefeated
	msgbox Route102_Text_RickPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Tiana::
	trainerbattle_single TRAINER_TIANA, Route102_Text_TianaIntro, Route102_Text_TianaDefeated
	msgbox Route102_Text_TianaPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Allen::
	trainerbattle_single TRAINER_ALLEN, Route102_Text_AllenIntro, Route102_Text_AllenDefeated
	msgbox Route102_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_Text_WatchMeCatchPokemon:
	.string "Lino: {PLAYER}…\n"
	.string "I Pokémon si nascondono nell'erba\l"
	.string "alta come questa, vero?\p"
	.string "Ti prego, guardami… e dimmi se\n"
	.string "riesco a catturarne uno come si deve.\p"
	.string "…Oh!$"

Route102_Text_WallyIDidIt:
	.string "Lino: Ce… ce l'ho fatta! È il mio…\n"
	.string "il mio Pokémon!\p"
	.string "Ralts… adesso siamo amici.$"

Route102_Text_LetsGoBack:
	.string "Scusa… mi gira la testa… come se\n"
	.string "l'aria… si piegasse.\p"
	.string "…Ecco, è passato.\n"
	.string "{PLAYER}, grazie di tutto!\l"
	.string "Torniamo in Palestra!$"

Route102_Text_ImNotVeryTall:
	.string "Non sono molto alto… quindi nell'erba\n"
	.string "alta ci sprofondo dentro.\p"
	.string "L'erba mi entra nel naso e…\n"
	.string "Eh… eh… eeeh…\p"
	.string "Etciùùù!$"

Route102_Text_CatchWholeBunchOfPokemon:
	.string "Catturerò un sacco di Pokémon!\n"
	.string "Anche quelli usciti dai Varchi!$"

Route102_Text_RouteSignOldale:
	.string "Percorso 102\n"
	.string "{RIGHT_ARROW} Solarosa$"

Route102_Text_RouteSignPetalburg:
	.string "Percorso 102\n"
	.string "{LEFT_ARROW} Petalipoli$"
