PetalburgCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PetalburgCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

PetalburgCity_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_PETALBURG_CITY
	call Common_EventScript_UpdateBrineyLocation
	end

PetalburgCity_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_PETALBURG_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_FatMan::
	msgbox PetalburgCity_PokemonCenter_1F_Text_PCStorageSystem, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox PetalburgCity_PokemonCenter_1F_Text_OranBerryRegainedHP, MSGBOX_NPC
	end

PetalburgCity_PokemonCenter_1F_EventScript_Woman::
	lock
	faceplayer
	msgbox PetalburgCity_PokemonCenter_1F_Text_ManyTypesOfPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, IsStarterInParty
	goto_if_eq VAR_RESULT, TRUE, PetalburgCity_PokemonCenter_1F_EventScript_SayStarterTypeInfo
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_SayStarterTypeInfo::
	call_if_eq VAR_STARTER_MON, 0, PetalburgCity_PokemonCenter_1F_EventScript_SayTreeckoType
	call_if_eq VAR_STARTER_MON, 1, PetalburgCity_PokemonCenter_1F_EventScript_SayTorchicType
	call_if_eq VAR_STARTER_MON, 2, PetalburgCity_PokemonCenter_1F_EventScript_SayMudkipType
	release
	end

PetalburgCity_PokemonCenter_1F_EventScript_SayTreeckoType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_TreeckoIsGrassType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_EventScript_SayTorchicType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_TorchicIsFireType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_EventScript_SayMudkipType::
	msgbox PetalburgCity_PokemonCenter_1F_Text_MudkipIsWaterType, MSGBOX_DEFAULT
	return

PetalburgCity_PokemonCenter_1F_Text_PCStorageSystem:
	.string "Il Sistema Memoria Pokémon\n"
	.string "dei PC…\p"
	.string "Chi l'ha inventato dev'essere un vero\n"
	.string "genio della scienza!\p"
	.string "Dicono che a volte riceva perfino\n"
	.string "messaggi da altri mondi…$"

PetalburgCity_PokemonCenter_1F_Text_OranBerryRegainedHP:
	.string "Quando il mio Pokémon ha mangiato\n"
	.string "una Baccarancia, ha recuperato PS!$"

PetalburgCity_PokemonCenter_1F_Text_ManyTypesOfPokemon:
	.string "Esistono tanti tipi di Pokémon.\p"
	.string "Ogni tipo ha punti di forza e punti\n"
	.string "deboli contro gli altri tipi.\p"
	.string "A seconda dei tipi dei Pokémon, una\n"
	.string "lotta può essere facile o difficile.$"

PetalburgCity_PokemonCenter_1F_Text_TreeckoIsGrassType:
	.string "Per esempio, il tuo Turtwig\n"
	.string "è di tipo Erba.\p"
	.string "È forte contro i tipi Acqua\n"
	.string "e Terra.\p"
	.string "Però è debole contro i Pokémon\n"
	.string "di tipo Fuoco.\p"
	.string "Da queste parti non se ne vedono\n"
	.string "molti… ma i tipi valgono in ogni mondo!$"

PetalburgCity_PokemonCenter_1F_Text_TorchicIsFireType:
	.string "Per esempio, il tuo Fuecoco\n"
	.string "è di tipo Fuoco.\p"
	.string "È forte contro i tipi Erba\n"
	.string "e Coleottero.\p"
	.string "Però è debole contro i Pokémon\n"
	.string "di tipo Acqua.\p"
	.string "Da queste parti non se ne vedono\n"
	.string "molti… ma i tipi valgono in ogni mondo!$"

PetalburgCity_PokemonCenter_1F_Text_MudkipIsWaterType:
	.string "Per esempio, il tuo Froakie\n"
	.string "è di tipo Acqua.\p"
	.string "È forte contro il tipo Fuoco.\p"
	.string "Però è debole contro i Pokémon\n"
	.string "di tipo Erba e di tipo Elettro.\p"
	.string "Da queste parti non se ne vedono\n"
	.string "molti… ma i tipi valgono in ogni mondo!$"

