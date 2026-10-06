VerdanturfTown_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VerdanturfTown_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

VerdanturfTown_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_VERDANTURF_TOWN
	call Common_EventScript_UpdateBrineyLocation
	end

VerdanturfTown_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_VERDANTURF_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

VerdanturfTown_PokemonCenter_1F_EventScript_Gentleman::
	msgbox VerdanturfTown_PokemonCenter_1F_Text_FaithInYourPokemon, MSGBOX_NPC
	end

VerdanturfTown_PokemonCenter_1F_EventScript_ExpertM::
	msgbox VerdanturfTown_PokemonCenter_1F_Text_VisitForBattleTent, MSGBOX_NPC
	end

VerdanturfTown_PokemonCenter_1F_Text_FaithInYourPokemon:
	.string "Non puoi considerarti un vero\n"
	.string "Allenatore se non hai fiducia\l"
	.string "nei tuoi Pokémon.\p"
	.string "Solo chi crede nei Pokémon con cui\n"
	.string "lotta può arrivare fino in fondo.$"

VerdanturfTown_PokemonCenter_1F_Text_VisitForBattleTent:
	.string "Se qualcuno viene fin qui a\n"
	.string "Mentania, c'è un solo motivo…\p"
	.string "La Tenda Lotta, ovvio.\n"
	.string "Non c'è nemmeno bisogno di dirlo.\p"
	.string "O forse sei venuto a trovare\n"
	.string "qualcuno?$"

