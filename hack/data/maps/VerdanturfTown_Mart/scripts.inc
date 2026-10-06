VerdanturfTown_Mart_MapScripts::
	.byte 0

VerdanturfTown_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart VerdanturfTown_Mart_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
VerdanturfTown_Mart_Pokemart:
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_NEST_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_REPEL
	.2byte ITEM_X_SP_ATK
	.2byte ITEM_FLUFFY_TAIL
	pokemartlistend

VerdanturfTown_Mart_EventScript_Boy::
	msgbox VerdanturfTown_Mart_Text_XSpecialIsCrucial, MSGBOX_NPC
	end

VerdanturfTown_Mart_EventScript_ExpertF::
	msgbox VerdanturfTown_Mart_Text_NoStrategyGuidesForBattleTent, MSGBOX_NPC
	end

VerdanturfTown_Mart_EventScript_Lass::
	msgbox VerdanturfTown_Mart_Text_NestBallOnWeakenedPokemon, MSGBOX_NPC
	end

VerdanturfTown_Mart_Text_XSpecialIsCrucial:
	.string "In ogni lotta Pokémon, l'Att. Speciale X\n"
	.string "è fondamentale.\p"
	.string "Fa salire alle stelle la potenza\n"
	.string "di certe mosse, anche se dura solo\l"
	.string "per una lotta.$"

VerdanturfTown_Mart_Text_NoStrategyGuidesForBattleTent:
	.string "Pare che non vendano guide strategiche\n"
	.string "per vincere alla Tenda Lotta…\p"
	.string "Alla fine, bisogna contare solo\n"
	.string "sul proprio ingegno…$"

VerdanturfTown_Mart_Text_NestBallOnWeakenedPokemon:
	.string "La Minor Ball funziona meglio con\n"
	.string "i Pokémon più deboli.\p"
	.string "E Mentania è l'unico posto dove\n"
	.string "puoi comprarla.$"

