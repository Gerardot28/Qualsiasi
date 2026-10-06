MossdeepCity_House1_MapScripts::
	.byte 0

MossdeepCity_House1_EventScript_BlackBelt::
	lock
	faceplayer
	bufferleadmonspeciesname STR_VAR_1
	msgbox MossdeepCity_House1_Text_HmmYourPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, GetPokeblockNameByMonNature
	goto_if_eq VAR_RESULT, 0, MossdeepCity_House1_EventScript_NeutralNature
	msgbox MossdeepCity_House1_Text_ItLikesXPokeblocks, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House1_EventScript_NeutralNature::
	msgbox MossdeepCity_House1_Text_DoesntLikeOrDislikePokeblocks, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House1_EventScript_Woman::
	msgbox MossdeepCity_House1_Text_HusbandCanTellPokeblockMonLikes, MSGBOX_NPC
	end

MossdeepCity_House1_Text_HmmYourPokemon:
	.string "Hmm!\n"
	.string "Il tuo {STR_VAR_1}…$"

MossdeepCity_House1_Text_ItLikesXPokeblocks:
	.string "Ha un debole per un tipo preciso di\n"
	.string "Pokémella: {STR_VAR_1}, vero?\p"
	.string "No, ne sono sicurissimo! La sua\n"
	.string "preferita è: {STR_VAR_1}!$"

MossdeepCity_House1_Text_DoesntLikeOrDislikePokeblocks:
	.string "Non sembra avere gusti particolari\n"
	.string "in fatto di Pokémelle.$"

MossdeepCity_House1_Text_HusbandCanTellPokeblockMonLikes:
	.string "Mio marito capisce al primo sguardo\n"
	.string "che Pokémelle piacciono a un Pokémon.\p"
	.string "E giura che funziona anche con quelli\n"
	.string "arrivati dai Varchi!$"

