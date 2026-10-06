GraniteCave_StevensRoom_MapScripts::
	.byte 0

GraniteCave_StevensRoom_EventScript_Steven::
	lock
	faceplayer
	msgbox GraniteCave_StevensRoom_Text_ImStevenLetterForMe, MSGBOX_DEFAULT
	setvar VAR_0x8004, ITEM_LETTER
	call Common_EventScript_PlayerHandedOverTheItem
	setflag FLAG_DELIVERED_STEVEN_LETTER
	msgbox GraniteCave_StevensRoom_Text_ThankYouTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_STEEL_WING
	call_if_eq VAR_RESULT, FALSE, GraniteCave_StevensRoom_EventScript_BagFull
	msgbox GraniteCave_StevensRoom_Text_CouldBecomeChampionLetsRegister, MSGBOX_DEFAULT
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox GraniteCave_StevensRoom_Text_RegisteredSteven, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_REGISTERED_STEVEN_POKENAV
	msgbox GraniteCave_StevensRoom_Text_IveGotToHurryAlong, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_FACING, DIR_NORTH, GraniteCave_StevensRoom_EventScript_StevenExitNorth
	call_if_eq VAR_FACING, DIR_SOUTH, GraniteCave_StevensRoom_EventScript_StevenExitSouth
	call_if_eq VAR_FACING, DIR_WEST, GraniteCave_StevensRoom_EventScript_StevenExitWestEast
	call_if_eq VAR_FACING, DIR_EAST, GraniteCave_StevensRoom_EventScript_StevenExitWestEast
	playse SE_EXIT
	removeobject LOCALID_GRANITE_CAVE_STEVEN
	release
	end

GraniteCave_StevensRoom_EventScript_StevenExitNorth::
	applymovement LOCALID_GRANITE_CAVE_STEVEN, GraniteCave_StevensRoom_Movement_StevenExit
	waitmovement 0
	return

GraniteCave_StevensRoom_EventScript_StevenExitWestEast::
	applymovement LOCALID_PLAYER, GraniteCave_StevensRoom_Movement_PlayerTurnTowardExit
	applymovement LOCALID_GRANITE_CAVE_STEVEN, GraniteCave_StevensRoom_Movement_StevenExit
	waitmovement 0
	return

GraniteCave_StevensRoom_EventScript_StevenExitSouth::
	applymovement LOCALID_PLAYER, GraniteCave_StevensRoom_Movement_PlayerTurnTowardExit
	applymovement LOCALID_GRANITE_CAVE_STEVEN, GraniteCave_StevensRoom_Movement_StevenExitSouth
	waitmovement 0
	return

GraniteCave_StevensRoom_EventScript_BagFull::
	msgbox GraniteCave_StevensRoom_Text_OhBagIsFull, MSGBOX_DEFAULT
	return

GraniteCave_StevensRoom_Movement_StevenExit:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	delay_8
	step_end

GraniteCave_StevensRoom_Movement_PlayerTurnTowardExit:
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_up
	step_end

GraniteCave_StevensRoom_Movement_StevenExitSouth:
	walk_left
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	delay_8
	step_end

GraniteCave_StevensRoom_Text_ImStevenLetterForMe:
	.string "Questa roccia non è di Hoenn.\p"
	.string "È caduta da un altro mondo, forse\n"
	.string "mille anni fa… Interessante…\p"
	.string "Oh, scusa. Non ti avevo sentito.\n"
	.string "Mi chiamo Rocco.\p"
	.string "Mi appassionano le pietre rare, così\n"
	.string "viaggio un po' ovunque per cercarle.\p"
	.string "Soprattutto quelle cadute dai Varchi.\p"
	.string "Oh?\n"
	.string "Una Lettera per me?$"

GraniteCave_StevensRoom_Text_ThankYouTakeThis:
	.string "Rocco: È di mio padre…\p"
	.string "“Torna a casa, ogni tanto.”\n"
	.string "“Stai lontano dai Varchi.”… Il solito.\p"
	.string "Comunque, grazie. Ti sei preso tutto\n"
	.string "questo disturbo per consegnarmela.\l"
	.string "Devo ricambiare.\p"
	.string "Vediamo…\n"
	.string "Ecco, ti do questa MT.\p"
	.string "Contiene la mia mossa preferita:\n"
	.string "Alacciaio.$"

GraniteCave_StevensRoom_Text_CouldBecomeChampionLetsRegister:
	.string "Rocco: I tuoi Pokémon sembrano\n"
	.string "davvero in gamba.\p"
	.string "Se continui ad allenarti, un giorno\n"
	.string "potresti perfino diventare Campione\l"
	.string "della Lega Pokémon. Io la penso così.\p"
	.string "I Varchi si stanno moltiplicando…\n"
	.string "Ho la sensazione che ci rivedremo.\p"
	.string "Già che ci siamo, registriamoci a\n"
	.string "vicenda nei nostri PokéNav.\p"
	.string "… … … … … …$"

GraniteCave_StevensRoom_Text_RegisteredSteven:
	.string "Hai registrato Rocco\n"
	.string "nel PokéNav.$"

GraniteCave_StevensRoom_Text_IveGotToHurryAlong:
	.string "Ora devo proprio andare.\n"
	.string "Ci rivedremo, ne sono certo.$"

GraniteCave_StevensRoom_Text_OhBagIsFull:
	.string "Oh, la tua Borsa è piena…\n"
	.string "Che peccato, allora.$"

