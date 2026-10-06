Route111_WinstrateFamilysHouse_MapScripts::
	.byte 0

Route111_WinstrateFamilysHouse_EventScript_Victor::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VICTOR
	msgbox Route111_WinstrateFamilysHouse_Text_MySonIsStrongerThanYou, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Victoria::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VICTORIA
	goto_if_set FLAG_RECEIVED_MACHO_BRACE, Route111_WinstrateFamilysHouse_EventScript_ReceivedMachoBrace
	msgbox Route111_WinstrateFamilysHouse_Text_LikeYouToHaveMachoBrace, MSGBOX_DEFAULT
	giveitem ITEM_MACHO_BRACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_MACHO_BRACE
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_ReceivedMachoBrace::
	msgbox Route111_WinstrateFamilysHouse_Text_PassionateAboutBattles, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Vivi::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VIVI
	msgbox Route111_WinstrateFamilysHouse_Text_StrongerFamilyMembers, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Vicky::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VICKY
	goto_if_set FLAG_TEMP_4, Route111_WinstrateFamilysHouse_EventScript_AlreadySpokenTo
	msgbox Route111_WinstrateFamilysHouse_Text_GrandsonStrong, MSGBOX_DEFAULT
	setflag FLAG_TEMP_4
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_AlreadySpokenTo::
	msgbox Route111_WinstrateFamilysHouse_Text_GrandsonStrongShort, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection::
	closemessage
	applymovement VAR_0x8008, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

Route111_WinstrateFamilysHouse_Text_MySonIsStrongerThanYou:
	.string "Non avevo mai visto un Allenatore\n"
	.string "guidare i Pokémon con tanta maestria.\p"
	.string "Però devo avvisarti: mio figlio\n"
	.string "è più forte di te.\p"
	.string "Pensa che ha persino affrontato\n"
	.string "la sfida della Lega Pokémon!$"

Route111_WinstrateFamilysHouse_Text_LikeYouToHaveMachoBrace:
	.string "Noi usiamo questo Crescicappa per\n"
	.string "allenare i nostri Pokémon in modo\l"
	.string "più efficace.\p"
	.string "Ci hai battuti tutti, quindi forse\n"
	.string "non ti serve… ma ci farebbe piacere\l"
	.string "regalarti il nostro Crescicappa.$"

Route111_WinstrateFamilysHouse_Text_PassionateAboutBattles:
	.string "Quando si tratta di lotte Pokémon,\n"
	.string "noi Vincenti ci mettiamo il cuore!$"

Route111_WinstrateFamilysHouse_Text_StrongerFamilyMembers:
	.string "La mamma è più forte del papà.\p"
	.string "Io sono più forte della mamma.\p"
	.string "E la nonna è più forte di me!\p"
	.string "Ma il mio fratellone è ancora più\n"
	.string "forte della nonna.$"

Route111_WinstrateFamilysHouse_Text_GrandsonStrong:
	.string "Che tu sia forte, non c'è dubbio.\p"
	.string "Ma se lottassi contro mio nipote,\n"
	.string "finiresti a piangere dalla rabbia.\p"
	.string "È più forte di qualsiasi Allenatore\n"
	.string "che la nostra famiglia conosca.\p"
	.string "Ormai starà sfidando il Campione\n"
	.string "della Lega Pokémon.\p"
	.string "Conoscendolo, potrebbe essere già\n"
	.string "diventato Campione!$"

Route111_WinstrateFamilysHouse_Text_GrandsonStrongShort:
	.string "Ormai mio nipote starà sfidando\n"
	.string "il Campione della Lega Pokémon.\p"
	.string "Conoscendolo, potrebbe essere già\n"
	.string "diventato Campione!$"

