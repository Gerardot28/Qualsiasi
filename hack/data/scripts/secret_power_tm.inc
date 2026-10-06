Route111_Text_MakingRoomUseTMToMakeYourOwn:
	.string "Eh? Che c'è?\n"
	.string "Cosa sto facendo?\p"
	.string "Sto pensando di farmi una stanza tutta\n"
	.string "mia qui, usando una mossa Pokémon.\p"
	.string "Ho un'idea! Ti regalo questa MT.\n"
	.string "La userai per farti una stanza tua?$"

Route111_Text_ExplainSecretPower:
	.string "Cerca un grande albero da cui\n"
	.string "sembrano poter scendere delle liane.\p"
	.string "Usa Forza Segreta davanti all'albero.\n"
	.string "Qualche liana dovrebbe staccarsi e\l"
	.string "scendere, così potrai arrampicarti.\p"
	.string "Lassù troverai un sacco di spazio\n"
	.string "per sistemare le tue cose preferite.\p"
	.string "Sarà la tua stanza segreta…\n"
	.string "Una Base Segreta!\p"
	.string "Dovresti fartene una anche tu.\p"
	.string "E poi, non serve per forza fare la\n"
	.string "tua Base Segreta su un albero.\p"
	.string "Prova a usare Forza Segreta sulle\n"
	.string "pareti rocciose con piccole rientranze.\p"
	.string "Io vado a cercare altri posti… magari\n"
	.string "uno dove non si aprono Varchi! Ciao!$"

Route111_Text_DontWantThis:
	.string "Oh, non la vuoi?\n"
	.string "Se cambi idea, fammelo sapere, ok?$"

Route111_Text_DontHaveAnyRoom:
	.string "Oh, non hai spazio per questa MT.\p"
	.string "Te la tengo da parte: torna a\n"
	.string "prenderla un'altra volta, ok?$"

Route111_EventScript_SecretPowerMan::
	lock
	faceplayer
	msgbox Route111_Text_MakingRoomUseTMToMakeYourOwn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_EventScript_GiveSecretPower
	msgbox Route111_Text_DontWantThis, MSGBOX_DEFAULT
	release
	end

Route111_EventScript_GiveSecretPower::
	giveitem ITEM_TM_SECRET_POWER
	goto_if_eq VAR_RESULT, FALSE, Route111_EventScript_NoRoomForSecretPower
	msgbox Route111_Text_ExplainSecretPower, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_RECEIVED_SECRET_POWER
	clearflag FLAG_HIDE_SLATEPORT_CITY_TM_SALESMAN
	call_if_eq VAR_FACING, DIR_WEST, Route111_EventScript_SecretPowerManExit
	call_if_eq VAR_FACING, DIR_EAST, Route111_EventScript_SecretPowerManExit
	call_if_eq VAR_FACING, DIR_NORTH, Route111_EventScript_SecretPowerManExitNorth
	removeobject VAR_LAST_TALKED
	release
	end

Route111_EventScript_SecretPowerManExit::
	applymovement VAR_LAST_TALKED, Route111_Movement_SecretPowerManExit
	waitmovement 0
	return

Route111_EventScript_SecretPowerManExitNorth::
	applymovement VAR_LAST_TALKED, Route111_Movement_SecretPowerManExitNorth
	waitmovement 0
	return

Route111_EventScript_NoRoomForSecretPower::
	msgbox Route111_Text_DontHaveAnyRoom, MSGBOX_DEFAULT
	release
	end

Route111_Movement_SecretPowerManExit:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end

Route111_Movement_SecretPowerManExitNorth:
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end
