SlateportCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SlateportCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

SlateportCity_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_SLATEPORT_CITY
	call Common_EventScript_UpdateBrineyLocation
	end

SlateportCity_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_SLATEPORT_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

SlateportCity_PokemonCenter_1F_EventScript_Sailor::
	msgbox SlateportCity_PokemonCenter_1F_Text_RaiseDifferentTypesOfPokemon, MSGBOX_NPC
	end

SlateportCity_PokemonCenter_1F_EventScript_Woman::
	msgbox SlateportCity_PokemonCenter_1F_Text_TradedMonWithFriend, MSGBOX_NPC
	end

SlateportCity_PokemonCenter_1F_Text_RaiseDifferentTypesOfPokemon:
	.string "Vuoi un consiglio per le lotte?\p"
	.string "Allena Pokémon di tipi diversi,\n"
	.string "in modo equilibrato.\p"
	.string "Non serve a niente avere un solo\n"
	.string "Pokémon fortissimo.\p"
	.string "Se finisce in svantaggio di tipo,\n"
	.string "rischia di non avere scampo.$"

SlateportCity_PokemonCenter_1F_Text_TradedMonWithFriend:
	.string "Scambio spesso Pokémon con i miei amici.\p"
	.string "Se il Pokémon che ricevo tiene\n"
	.string "uno strumento, sono felice il doppio!$"

