PacifidlogTown_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PacifidlogTown_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

PacifidlogTown_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_PACIFIDLOG_TOWN
	end

PacifidlogTown_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_PACIFIDLOG_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

PacifidlogTown_PokemonCenter_1F_EventScript_Girl::
	msgbox PacifidlogTown_PokemonCenter_1F_Text_WhatColorTrainerCard, MSGBOX_NPC
	end

PacifidlogTown_PokemonCenter_1F_EventScript_Woman::
	msgbox PacifidlogTown_PokemonCenter_1F_Text_OnColonyOfCorsola, MSGBOX_NPC
	end

PacifidlogTown_PokemonCenter_1F_EventScript_OldMan::
	msgbox PacifidlogTown_PokemonCenter_1F_Text_AncestorsLivedOnBoats, MSGBOX_NPC
	end

PacifidlogTown_PokemonCenter_1F_Text_WhatColorTrainerCard:
	.string "Di che colore è la tua Scheda\n"
	.string "Allenatore? La mia è color rame!$"

PacifidlogTown_PokemonCenter_1F_Text_OnColonyOfCorsola:
	.string "Orocea galleggia sopra una colonia\n"
	.string "di Corsola.\p"
	.string "Se te lo dicessi, ci crederesti?$"

PacifidlogTown_PokemonCenter_1F_Text_AncestorsLivedOnBoats:
	.string "Si dice che gli antenati della gente\n"
	.string "di Orocea nascessero sulle barche,\l"
	.string "e lì vivessero fino all'ultimo giorno.\p"
	.string "Pare che vivessero così perché erano\n"
	.string "alla ricerca di qualcosa.\p"
	.string "Mio nonno diceva: una torre che tocca\n"
	.string "il cielo, e una porta lassù in cima…$"

