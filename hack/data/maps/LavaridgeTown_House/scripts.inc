LavaridgeTown_House_MapScripts::
	.byte 0

LavaridgeTown_House_EventScript_OldMan::
	msgbox LavaridgeTown_House_Text_WifeWarmingEggInHotSprings, MSGBOX_NPC
	end

LavaridgeTown_House_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox LavaridgeTown_House_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

LavaridgeTown_House_Text_WifeWarmingEggInHotSprings:
	.string "Mia moglie sta scaldando un Uovo nelle\n"
	.string "terme. Almeno, così mi ha detto.\p"
	.string "Aveva lasciato due Pokémon alla\n"
	.string "Pensione… ed è saltato fuori l'Uovo!$"

LavaridgeTown_House_Text_Zigzagoon:
	.string "Zigzagoon: Pshuu!$"

