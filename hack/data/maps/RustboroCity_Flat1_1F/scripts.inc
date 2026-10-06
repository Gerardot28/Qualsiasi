RustboroCity_Flat1_1F_MapScripts::
	.byte 0

RustboroCity_Flat1_1F_EventScript_Man::
	msgbox RustboroCity_Flat1_1F_Text_EveryPokemonHasAbility, MSGBOX_NPC
	end

RustboroCity_Flat1_1F_EventScript_Woman::
	msgbox RustboroCity_Flat1_1F_Text_PokemonStrange, MSGBOX_NPC
	end

RustboroCity_Flat1_1F_Text_EveryPokemonHasAbility:
	.string "Ogni Pokémon ha un'Abilità speciale\n"
	.string "che può sfruttare.$"

RustboroCity_Flat1_1F_Text_PokemonStrange:
	.string "I Pokémon sono creature così strane…\n"
	.string "E quelli dei Varchi, ancora di più!$"

