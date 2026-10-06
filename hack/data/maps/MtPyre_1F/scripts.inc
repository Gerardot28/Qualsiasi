MtPyre_1F_MapScripts::
	.byte 0

MtPyre_1F_EventScript_CleanseTagWoman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_CLEANSE_TAG, MtPyre_1F_EventScript_ReceivedCleanseTag
	msgbox MtPyre_1F_Text_TakeThisForYourOwnGood, MSGBOX_DEFAULT
	giveitem ITEM_CLEANSE_TAG
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_CLEANSE_TAG
	release
	end

MtPyre_1F_EventScript_ReceivedCleanseTag::
	msgbox MtPyre_1F_Text_ExplainCleanseTag, MSGBOX_DEFAULT
	release
	end

MtPyre_1F_EventScript_PokefanF::
	msgbox MtPyre_1F_Text_ComeToPayRespects, MSGBOX_NPC
	end

MtPyre_1F_EventScript_Man::
	msgbox MtPyre_1F_Text_RestingPlaceOfZigzagoon, MSGBOX_NPC
	end

MtPyre_1F_Text_TakeThisForYourOwnGood:
	.string "Sui pendii del Monte Pira vagano\n"
	.string "presenze di ogni sorta…\p"
	.string "E da quando si aprono i Varchi, forse\n"
	.string "anche spiriti venuti da altri mondi.\p"
	.string "Non si sa mai cosa può succedere.\n"
	.string "Prendi questo. È per il tuo bene.$"

MtPyre_1F_Text_ExplainCleanseTag:
	.string "Fai tenere quel Velopuro al primo\n"
	.string "Pokémon della tua squadra.\p"
	.string "Ti aiuterà a tenere lontani\n"
	.string "i Pokémon selvatici.$"

MtPyre_1F_Text_ComeToPayRespects:
	.string "Sei venuto a rendere omaggio agli\n"
	.string "spiriti dei Pokémon scomparsi?\p"
	.string "Devi volere davvero bene ai tuoi\n"
	.string "Pokémon.$"

MtPyre_1F_Text_RestingPlaceOfZigzagoon:
	.string "Qui riposa il mio Zigzagoon.\n"
	.string "Gli volevo un bene dell'anima…$"

