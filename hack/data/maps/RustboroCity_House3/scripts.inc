RustboroCity_House3_MapScripts::
	.byte 0

RustboroCity_House3_EventScript_OldMan::
	msgbox RustboroCity_House3_Text_IGivePerfectlySuitedNicknames, MSGBOX_NPC
	end

RustboroCity_House3_EventScript_OldWoman::
	msgbox RustboroCity_House3_Text_NamingPikachuPekachu, MSGBOX_NPC
	end

@ Misspelling on purpose, see nickname
RustboroCity_House3_EventScript_Pekachu::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	msgbox RustboroCity_House3_Text_Pekachu, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RustboroCity_House3_Text_IGivePerfectlySuitedNicknames:
	.string "Ai miei Pokémon do sempre dei\n"
	.string "soprannomi perfetti!\p"
	.string "È la mia espressione di… ehm…\n"
	.string "originalità, sì, proprio così!$"

RustboroCity_House3_Text_NamingPikachuPekachu:
	.string "Ma chiamare Pekachu un Pikachu?\n"
	.string "Che senso ha?\p"
	.string "Va bene usare un nome facile da\n"
	.string "capire, però…$"

RustboroCity_House3_Text_Pekachu:
	.string "Pekachu: Peka!$"

