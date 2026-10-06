Route111_OldLadysRestStop_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route111_OldLadysRestStop_OnTransition
	.byte 0

Route111_OldLadysRestStop_OnTransition:
	setflag FLAG_LANDMARK_OLD_LADY_REST_SHOP
	end

Route111_OldLadysRestStop_EventScript_OldLady::
	lock
	faceplayer
	msgbox Route111_OldLadysRestStop_Text_RestUpHere, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_OldLadysRestStop_EventScript_Rest
	goto_if_eq VAR_RESULT, NO, Route111_OldLadysRestStop_EventScript_DeclineRest
	end

Route111_OldLadysRestStop_EventScript_Rest::
	msgbox Route111_OldLadysRestStop_Text_TakeYourTimeRestUp, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox Route111_OldLadysRestStop_Text_StillTiredTakeAnotherRest, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_OldLadysRestStop_EventScript_Rest
	goto_if_eq VAR_RESULT, NO, Route111_OldLadysRestStop_EventScript_DeclineRest
	end

Route111_OldLadysRestStop_EventScript_DeclineRest::
	msgbox Route111_OldLadysRestStop_Text_DontNeedToBeShy, MSGBOX_DEFAULT
	release
	end

Route111_OldLadysRestStop_Text_RestUpHere:
	.string "Oh, poverino, poverino.\n"
	.string "I tuoi Pokémon sono sfiniti, vero?\p"
	.string "Se vuoi, riposati un po' qui.\n"
	.string "Ma sì, che bella idea! Fallo, dai.$"

Route111_OldLadysRestStop_Text_TakeYourTimeRestUp:
	.string "Bravo, così si fa.\n"
	.string "Prenditi tutto il tempo che vuoi!$"

Route111_OldLadysRestStop_Text_StillTiredTakeAnotherRest:
	.string "Oh, poverino, poverino.\n"
	.string "I tuoi Pokémon sono ancora stanchi?\p"
	.string "Fatti un altro riposino qui.\n"
	.string "Ma sì, che bella idea! Fallo, dai.$"

Route111_OldLadysRestStop_Text_DontNeedToBeShy:
	.string "Ah, no?\n"
	.string "Guarda che non devi farti problemi.$"

