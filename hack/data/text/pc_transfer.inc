gText_PkmnTransferredSomeonesPC::
	.string "{STR_VAR_2} è stato trasferito\n"
	.string "nel PC di qualcuno.\p"
	.string "È stato depositato nel\n"
	.string "Box “{STR_VAR_1}”.$"

gText_PkmnTransferredLanettesPC::
	.string "{STR_VAR_2} è stato trasferito\n"
#if IS_FRLG
	.string "nel PC di Bill.\p"
#else
	.string "nel PC di Lanette.\p"
#endif
	.string "È stato depositato nel\n"
	.string "Box “{STR_VAR_1}”.$"

gText_PkmnTransferredSomeonesPCBoxFull::
	.string "Il Box “{STR_VAR_3}” nel\n"
	.string "PC di qualcuno è pieno.\p"
	.string "{STR_VAR_2} è stato trasferito\n"
	.string "nel Box “{STR_VAR_1}”.$"

gText_PkmnTransferredLanettesPCBoxFull::
	.string "Il Box “{STR_VAR_3}” nel\n"
#if IS_FRLG
	.string "PC di Bill è pieno.\p"
#else
	.string "PC di Lanette è pieno.\p"
#endif
	.string "{STR_VAR_2} è stato trasferito\n"
	.string "nel Box “{STR_VAR_1}”.$"

gText_PkmnSentToPCAfterCatch::
	.string "{STR_VAR_2} è stato inviato\n"
	.string "al PC {B_PC_CREATOR_NAME}.\p"
	.string "È stato depositato nel\n"
	.string "Box “{STR_VAR_1}”.$"

gText_PlayerObtainedTheMon::
	.string "{PLAYER} ha ottenuto\n"
	.string "{STR_VAR_1}!$"

gText_NoMoreRoomForPokemon::
	.string "Non c'è più posto per altri Pokémon!\p"
	.string "I Box Pokémon sono pieni e non\n"
	.string "possono accoglierne altri!$"

gText_NicknameThisPokemon::
	.string "Vuoi dare un soprannome\n"
	.string "a questo {STR_VAR_1}?$"
