EventScript_StartDexNavBattle::
	lock
	playse SE_PIN
	applymovement OBJ_EVENT_ID_PLAYER Common_Movement_ExclamationMark
	waitmovement 0
	waitse
	dowildbattle	
	release
	end

EventScript_NotFoundNearby::
	msgbox sText_NotFoundNearby, MSGBOX_SIGN
	end

EventScript_MovedTooFast::
	msgbox sText_TryMovingSlower, MSGBOX_SIGN
	end

EventScript_PokemonGotAway::
	msgbox sText_PokemonGotAway, MSGBOX_SIGN
	end

EventScript_LostSignal::
	msgbox sText_LostSignal, MSGBOX_SIGN
	end

EventScript_TooDark::
	msgbox sText_TooDark, MSGBOX_SIGN
	end

sText_NotFoundNearby:
	.string "Qui intorno non c'è traccia.\n"
	.string "Prova a cercare in un'altra zona!$"

sText_TryMovingSlower:
	.string "Il Pokémon è scappato!\n"
	.string "Prova a muoverti più lentamente.$"

sText_PokemonGotAway:
	.string "Il Pokémon è scappato!$"

sText_LostSignal:
	.string "Nessuna reazione.\n"
	.string "Il segnale è andato perso!$"

sText_TooDark:
	.string "È troppo buio per cercare\nun Pokémon!$"
