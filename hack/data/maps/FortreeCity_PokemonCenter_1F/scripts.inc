FortreeCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FortreeCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

FortreeCity_PokemonCenter_1F_OnTransition:
	setrespawn HEAL_LOCATION_FORTREE_CITY
	end

FortreeCity_PokemonCenter_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_FORTREE_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

FortreeCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox FortreeCity_PokemonCenter_1F_Text_GoToSafariZone, MSGBOX_NPC
	end

FortreeCity_PokemonCenter_1F_EventScript_Man::
	msgbox FortreeCity_PokemonCenter_1F_Text_RecordCornerIsNeat, MSGBOX_NPC
	end

FortreeCity_PokemonCenter_1F_EventScript_Boy::
	msgbox FortreeCity_PokemonCenter_1F_Text_DoYouKnowAboutPokenav, MSGBOX_NPC
	end

FortreeCity_PokemonCenter_1F_Text_GoToSafariZone:
	.string "Senti, ragazzo, stai completando\n"
	.string "il Pokédex?\p"
	.string "Mmh… Fai un salto alla Zona Safari.\n"
	.string "Ecco il mio consiglio.$"

FortreeCity_PokemonCenter_1F_Text_RecordCornerIsNeat:
	.string "Hai mai provato il\n"
	.string "Banco Registro?\p"
	.string "È forte! Mescola e confronta i dati\n"
	.string "degli Allenatori.\p"
	.string "Non ho ben capito come funzioni,\n"
	.string "ma è fico. Anzi, è entusiasmante!$"

FortreeCity_PokemonCenter_1F_Text_DoYouKnowAboutPokenav:
	.string "Oh, wow, hai un PokéNav!\n"
	.string "Ed è proprio come il mio!\p"
	.string "Conosci lo Sfidafonino\n"
	.string "del PokéNav?\p"
	.string "Con quello puoi chiacchierare quando\n"
	.string "vuoi con gli Allenatori registrati.\p"
	.string "Ti mostra anche quali Allenatori\n"
	.string "vogliono la rivincita.\p"
	.string "È comodissimo! Quelli della Devon\n"
	.string "sanno proprio il fatto loro!$"

