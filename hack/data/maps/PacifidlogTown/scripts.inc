PacifidlogTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PacifidlogTown_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, PacifidlogTown_OnResume
	.byte 0

PacifidlogTown_OnTransition:
	setflag FLAG_VISITED_PACIFIDLOG_TOWN
	end

PacifidlogTown_OnResume:
	setstepcallback STEP_CB_PACIFIDLOG_BRIDGE
	end

PacifidlogTown_EventScript_NinjaBoy::
	msgbox PacifidlogTown_Text_NeatHousesOnWater, MSGBOX_NPC
	end

PacifidlogTown_EventScript_Girl::
	msgbox PacifidlogTown_Text_FastRunningCurrent, MSGBOX_NPC
	end

PacifidlogTown_EventScript_Fisherman::
	msgbox PacifidlogTown_Text_SkyPillarTooScary, MSGBOX_NPC
	end

PacifidlogTown_EventScript_TownSign::
	msgbox PacifidlogTown_Text_TownSign, MSGBOX_SIGN
	end

PacifidlogTown_Text_FastRunningCurrent:
	.string "Nel mare tra Orocea e Porto Selcepoli\n"
	.string "la corrente è fortissima.\p"
	.string "Se decidi di fare Surf, rischi di\n"
	.string "finire trascinato chissà dove.$"

PacifidlogTown_Text_NeatHousesOnWater:
	.string "Visto che forte?\n"
	.string "Queste case galleggiano sull'acqua!\p"
	.string "Io sono nato proprio qui!$"

PacifidlogTown_Text_SkyPillarTooScary:
	.string "La Torre dei Cieli?\p"
	.string "…Ah, intendi quella torre altissima\n"
	.string "un po' più al largo.\p"
	.string "Fossi in te, non ci salirei. Arrivare\n"
	.string "fin lassù mi metterebbe i brividi.\p"
	.string "E poi, certe notti, sopra la cima si\n"
	.string "vede una luce strana, come un'aurora…\p"
	.string "La vita a pelo d'acqua di Orocea,\n"
	.string "ecco cosa fa per me.$"

PacifidlogTown_Text_TownSign:
	.string "Orocea\p"
	.string "“Dove il sole del mattino sorride\n"
	.string "alle acque.”$"

