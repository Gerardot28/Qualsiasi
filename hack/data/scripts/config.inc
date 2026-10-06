Debug_FlagsAndVarNotSetBattleConfigMessage::
	lockall
	message Debug_FlagsAndVarNotSetBattleConfigMessage_Text
	waitmessage
	waitbuttonpress
	releaseall
	end

Debug_FlagsAndVarNotSetBattleConfigMessage_Text:
	.string "Funzione non disponibile! Definisci\n"
	.string "un flag e una variabile utilizzabili\l"
	.string "in “include/config/battle.h”!$"
