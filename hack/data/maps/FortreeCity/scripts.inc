FortreeCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FortreeCity_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, FortreeCity_OnResume
	.byte 0

FortreeCity_OnTransition:
	setflag FLAG_VISITED_FORTREE_CITY
	end

FortreeCity_OnResume:
	setstepcallback STEP_CB_FORTREE_BRIDGE
	end

FortreeCity_EventScript_Man::
	msgbox FortreeCity_Text_SawGiganticPokemonInSky, MSGBOX_NPC
	end

FortreeCity_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_KECLEON_FLED_FORTREE, FortreeCity_EventScript_WomanGymAccessible
	msgbox FortreeCity_Text_SomethingBlockingGym, MSGBOX_DEFAULT
	release
	end

FortreeCity_EventScript_WomanGymAccessible::
	msgbox FortreeCity_Text_ThisTimeIllBeatWinona, MSGBOX_DEFAULT
	release
	end

FortreeCity_EventScript_Girl::
	msgbox FortreeCity_Text_TreesGrowByDrinkingRainwater, MSGBOX_NPC
	end

FortreeCity_EventScript_OldMan::
	msgbox FortreeCity_Text_EveryoneHealthyAndLively, MSGBOX_NPC
	end

FortreeCity_EventScript_Boy::
	msgbox FortreeCity_Text_BugPokemonComeThroughWindow, MSGBOX_NPC
	end

FortreeCity_EventScript_GameboyKid::
	msgbox FortreeCity_Text_PokemonThatEvolveWhenTraded, MSGBOX_NPC
	end

FortreeCity_EventScript_CitySign::
	msgbox FortreeCity_Text_CitySign, MSGBOX_SIGN
	end

FortreeCity_EventScript_GymSign::
	msgbox FortreeCity_Text_GymSign, MSGBOX_SIGN
	end

FortreeCity_EventScript_Kecleon::
	lock
	faceplayer
	checkitem ITEM_DEVON_SCOPE
	goto_if_eq VAR_RESULT, TRUE, FortreeCity_EventScript_AskUseDevonScope
	msgbox FortreeCity_Text_SomethingUnseeable, MSGBOX_DEFAULT
	release
	end

FortreeCity_EventScript_AskUseDevonScope::
	msgbox FortreeCity_Text_UnseeableUseDevonScope, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FortreeCity_EventScript_UseDevonScope
	release
	end

FortreeCity_EventScript_UseDevonScope::
	msgbox FortreeCity_Text_UsedDevonScopePokemonFled, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Movement_KecleonAppears
	waitmovement 0
	waitse
	playmoncry SPECIES_KECLEON, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	applymovement VAR_LAST_TALKED, FortreeCity_Movement_KecleonFlee
	waitmovement 0
	removeobject VAR_LAST_TALKED
	setflag FLAG_KECLEON_FLED_FORTREE
	release
	end

FortreeCity_Movement_KecleonFlee:
	walk_right
	step_end

FortreeCity_Text_SawGiganticPokemonInSky:
	.string "Nessuno mi crede, ma ho visto un\n"
	.string "Pokémon gigantesco nel cielo.\p"
	.string "Si contorceva come un nastro mentre\n"
	.string "volava verso il Percorso 131.\l"
	.string "Dietro di lui, il cielo brillava.\p"
	.string "A proposito… Sniff, sniff…\n"
	.string "Ehm… Tu… sai un po' di bruciato.\p"
	.string "Sei stato su un vulcano, per caso?$"

FortreeCity_Text_SomethingBlockingGym:
	.string "Voglio entrare in Palestra, ma\n"
	.string "qualcosa mi sbarra la strada.\p"
	.string "E pensare a quanto mi sono allenata\n"
	.string "sul Percorso 120…$"

FortreeCity_Text_ThisTimeIllBeatWinona:
	.string "Ho con me i miei Pokémon migliori.\n"
	.string "Stavolta Alice la batto io!$"

FortreeCity_Text_TreesGrowByDrinkingRainwater:
	.string "La terra assorbe la pioggia e gli\n"
	.string "alberi crescono bevendo quell'acqua…\p"
	.string "La nostra Forestopoli esiste perché\n"
	.string "qui ci sono sia l'acqua che la terra.$"

FortreeCity_Text_EveryoneHealthyAndLively:
	.string "Questa città è fatta di case costruite\n"
	.string "sugli alberi.\p"
	.string "Forse è merito di questo stile di vita,\n"
	.string "ma qui sono tutti sani e pieni di brio.\p"
	.string "Persino io… mi sento ringiovanito\n"
	.string "di trent'anni!$"

FortreeCity_Text_BugPokemonComeThroughWindow:
	.string "Vivere in cima agli alberi è bello.\p"
	.string "Però a volte i Pokémon Coleottero\n"
	.string "entrano dalla finestra.\l"
	.string "Ti fanno prendere un colpo!\p"
	.string "L'altra notte ne è entrato uno mai\n"
	.string "visto. Secondo me veniva da un Varco!$"

FortreeCity_Text_PokemonThatEvolveWhenTraded:
	.string "Esistono Pokémon che si evolvono\n"
	.string "quando li scambi! Me l'hanno detto.$"

FortreeCity_Text_SomethingUnseeable:
	.string "Qualcosa di invisibile sbarra la strada.$"

FortreeCity_Text_UnseeableUseDevonScope:
	.string "Qualcosa di invisibile sbarra la strada.\p"
	.string "Vuoi usare il Devonscopio?$"

FortreeCity_Text_UsedDevonScopePokemonFled:
	.string "{PLAYER} usa il Devonscopio.\p"
	.string "Un Pokémon invisibile si rivela\n"
	.string "all'improvviso!\p"
	.string "Il Pokémon, spaventato, fugge via!$"

FortreeCity_Text_CitySign:
	.string "Forestopoli\n"
	.string "“La città tra le fronde, dove natura\l"
	.string "e cielo si toccano.”$"

FortreeCity_Text_GymSign:
	.string "Palestra Pokémon di Forestopoli\n"
	.string "Capopalestra: Alice\p"
	.string "“L'Allenatrice di Pokémon Uccello\n"
	.string "che spicca il volo verso il mondo.”$"

