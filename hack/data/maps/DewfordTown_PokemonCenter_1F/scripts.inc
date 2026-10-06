DewfordTown_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DewfordTown_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

DewfordTown_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_DEWFORD_TOWN
	call Common_EventScript_UpdateBrineyLocation
	end

DewfordTown_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_DEWFORD_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

DewfordTown_PokemonCenter_1F_EventScript_PokefanF::
	msgbox DewfordTown_PokemonCenter_1F_Text_StoneCavern, MSGBOX_NPC
	end

DewfordTown_PokemonCenter_1F_EventScript_Man::
	msgbox DewfordTown_PokemonCenter_1F_Text_FaintedMonCanUseHM, MSGBOX_NPC
	end

DewfordTown_PokemonCenter_1F_Text_StoneCavern:
	.string "Ai margini del paese c'è una grotta:\n"
	.string "la Grotta Pietrosa.\p"
	.string "Dicono che ci si trovino pietre rare.\p"
	.string "Ultimamente, qualcuna sembra perfino\n"
	.string "arrivata da un altro mondo…$"

DewfordTown_PokemonCenter_1F_Text_FaintedMonCanUseHM:
	.string "Anche se un Pokémon è esausto e non\n"
	.string "può lottare, può comunque usare una\l"
	.string "mossa insegnata da una Macchina\l"
	.string "Nascosta (MN).$"

