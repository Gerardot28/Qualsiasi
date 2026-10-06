MossdeepCity_Mart_MapScripts::
	.byte 0

MossdeepCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart MossdeepCity_Mart_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
MossdeepCity_Mart_Pokemart:
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_NET_BALL
	.2byte ITEM_DIVE_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	pokemartlistend

MossdeepCity_Mart_EventScript_Woman::
	msgbox MossdeepCity_Mart_Text_ReviveIsFantastic, MSGBOX_NPC
	end

MossdeepCity_Mart_EventScript_Boy::
	msgbox MossdeepCity_Mart_Text_MaxRepelLastsLongest, MSGBOX_NPC
	end

MossdeepCity_Mart_EventScript_Sailor::
	msgbox MossdeepCity_Mart_Text_NetAndDiveBallsRare, MSGBOX_NPC
	end

MossdeepCity_Mart_Text_ReviveIsFantastic:
	.string "Il Revitalizzante è fantastico!\p"
	.string "Se lo dai a un Pokémon esausto,\n"
	.string "quel Pokémon torna in piedi.\p"
	.string "Ma attenzione: il Revitalizzante non\n"
	.string "ripristina i PP delle mosse.$"

MossdeepCity_Mart_Text_MaxRepelLastsLongest:
	.string "Il Repellente Max tiene alla larga\n"
	.string "tutti i Pokémon deboli.\p"
	.string "Fra tutti i Repellenti, è quello che\n"
	.string "dura più a lungo.$"

MossdeepCity_Mart_Text_NetAndDiveBallsRare:
	.string "Rete Ball e Sub Ball sono Poké Ball\n"
	.string "rare, fatte solo a Verdeazzupoli.\p"
	.string "La Rete Ball è efficace sui Pokémon\n"
	.string "di tipo Coleottero e di tipo Acqua.\p"
	.string "La Sub Ball funziona meglio sui\n"
	.string "Pokémon che vivono in fondo al mare.$"

