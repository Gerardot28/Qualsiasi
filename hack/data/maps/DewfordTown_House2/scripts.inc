DewfordTown_House2_MapScripts::
	.byte 0

DewfordTown_House2_EventScript_Man::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_SILK_SCARF, DewfordTown_House2_EventScript_ExplainSilkScarf
	msgbox DewfordTown_House2_Text_WantYouToHaveSilkScarf, MSGBOX_DEFAULT
	giveitem ITEM_SILK_SCARF
	goto_if_eq VAR_RESULT, FALSE, DewfordTown_House2_EventScript_NoRoomForScarf
	setflag FLAG_RECEIVED_SILK_SCARF
	release
	end

DewfordTown_House2_EventScript_NoRoomForScarf::
	msgbox DewfordTown_House2_Text_NoRoom, MSGBOX_DEFAULT
	release
	end

DewfordTown_House2_EventScript_ExplainSilkScarf::
	msgbox DewfordTown_House2_Text_ExplainSilkScarf, MSGBOX_DEFAULT
	release
	end

DewfordTown_House2_EventScript_Boy::
	msgbox DewfordTown_House2_Text_BrawlySoCool, MSGBOX_NPC
	end

DewfordTown_House2_Text_WantYouToHaveSilkScarf:
	.string "Rifatti gli occhi con questa!\p"
	.string "È una Sciarpa Seta. L'ultimissimo\n"
	.string "grido della moda, sì sì!\p"
	.string "Oh, vedo i tuoi occhi brillare!\n"
	.string "Apprezzi il mio stile sfavillante!\p"
	.string "Sei un tesoro!\n"
	.string "Tieni, voglio che sia tua!$"

DewfordTown_House2_Text_NoRoom:
	.string "Oh, non hai spazio?\p"
	.string "Ascoltami bene: questa Sciarpa è un\n"
	.string "must assoluto! Io venderei tutti i miei\l"
	.string "strumenti pur di averla!$"

DewfordTown_House2_Text_ExplainSilkScarf:
	.string "La Sciarpa Seta aumenta la potenza\n"
	.string "delle mosse di tipo Normale.\p"
	.string "È una Sciarpa favolosa che sta bene\n"
	.string "con quasi tutti i Pokémon!$"

DewfordTown_House2_Text_BrawlySoCool:
	.string "Uau, hai attraversato il mare apposta\n"
	.string "per venire a Bluruvia?\p"
	.string "Sei venuto perché hai sentito parlare\n"
	.string "di Rudi?\p"
	.string "È fortissimo… Tutti lo adorano.\p"
	.string "Dicono che una volta abbia cavalcato\n"
	.string "un'onda uscita dritta da un Varco!$"

