OldaleTown_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OldaleTown_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

OldaleTown_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_OLDALE_TOWN
	call Common_EventScript_UpdateBrineyLocation
	end

OldaleTown_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_OLDALE_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

OldaleTown_PokemonCenter_1F_EventScript_Gentleman::
	msgbox OldaleTown_PokemonCenter_1F_Text_TrainersCanUsePC, MSGBOX_NPC
	end

OldaleTown_PokemonCenter_1F_EventScript_Boy::
	msgbox OldaleTown_PokemonCenter_1F_Text_PokemonCentersAreGreat, MSGBOX_NPC
	end

OldaleTown_PokemonCenter_1F_EventScript_Girl::
	lock
	faceplayer
	goto_if_set FLAG_SYS_POKEDEX_GET, OldaleTown_PokemonCenter_1F_EventScript_WirelessClubAvailable
	msgbox OldaleTown_PokemonCenter_1F_Text_WirelessClubNotAvailable, MSGBOX_DEFAULT
	release
	end

OldaleTown_PokemonCenter_1F_EventScript_WirelessClubAvailable::
	msgbox OldaleTown_PokemonCenter_1F_Text_TradedInWirelessClub, MSGBOX_DEFAULT
	release
	end

OldaleTown_PokemonCenter_1F_Text_TrainersCanUsePC:
	.string "Quel PC nell'angolo può usarlo\n"
	.string "qualsiasi Allenatore di Pokémon.\p"
	.string "Quindi, naturalmente, puoi usarlo\n"
	.string "anche tu.$"

OldaleTown_PokemonCenter_1F_Text_PokemonCentersAreGreat:
	.string "I Centri Pokémon sono fantastici!\p"
	.string "Puoi usare i loro servizi quanto vuoi,\n"
	.string "ed è tutto completamente gratis.\l"
	.string "Non c'è mai da preoccuparsi!$"

OldaleTown_PokemonCenter_1F_Text_WirelessClubNotAvailable:
	.string "Il Club Wireless Pokémon al piano\n"
	.string "di sopra è stato aperto da poco.\p"
	.string "Però dicono che stanno ancora\n"
	.string "sistemando qualcosa.$"

OldaleTown_PokemonCenter_1F_Text_TradedInWirelessClub:
	.string "Il Club Wireless Pokémon al piano\n"
	.string "di sopra è stato aperto da poco.\p"
	.string "Io ci ho subito scambiato dei Pokémon!$"

