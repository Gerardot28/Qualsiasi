RustboroCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

RustboroCity_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_RUSTBORO_CITY
	call Common_EventScript_UpdateBrineyLocation
	end

RustboroCity_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_RUSTBORO_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

RustboroCity_PokemonCenter_1F_EventScript_Man::
	msgbox RustboroCity_PokemonCenter_1F_Text_PokemonHavePersonalities, MSGBOX_NPC
	end

RustboroCity_PokemonCenter_1F_EventScript_Boy::
	msgbox RustboroCity_PokemonCenter_1F_Text_MaleAndFemalePokemon, MSGBOX_NPC
	end

RustboroCity_PokemonCenter_1F_EventScript_Girl::
	msgbox RustboroCity_PokemonCenter_1F_Text_HMCutNextDoor, MSGBOX_NPC
	end

RustboroCity_PokemonCenter_1F_Text_PokemonHavePersonalities:
	.string "Il mio Pokémon ha natura Ingenua,\n"
	.string "quello del mio amico natura Allegra.\p"
	.string "È affascinante che i Pokémon abbiano\n"
	.string "una personalità!$"

RustboroCity_PokemonCenter_1F_Text_MaleAndFemalePokemon:
	.string "Proprio come le persone, anche i\n"
	.string "Pokémon possono essere maschi o\l"
	.string "femmine.\p"
	.string "Ma nessuno sa bene in cosa siano\n"
	.string "diversi.$"

RustboroCity_PokemonCenter_1F_Text_HMCutNextDoor:
	.string "Il signore della casa accanto mi ha\n"
	.string "dato una MN!\p"
	.string "L'ho usata per insegnare Taglio al mio\n"
	.string "Pokémon: ora abbatte gli alberelli!$"

