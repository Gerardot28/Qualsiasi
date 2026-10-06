BattleFrontier_Lounge3_MapScripts::
	.byte 0

	.set BET_AMOUNT_5,  5
	.set BET_AMOUNT_10, 10
	.set BET_AMOUNT_15, 15

BattleFrontier_Lounge3_EventScript_Gambler::
	lock
	faceplayer
	goto_if_set FLAG_MET_BATTLE_FRONTIER_GAMBLER, BattleFrontier_Lounge3_EventScript_AlreadyMetGambler
	call BattleFrontier_Lounge3_EventScript_CountSilverSymbols
	goto_if_le VAR_0x8004, 2, BattleFrontier_Lounge3_EventScript_NotEnoughSilverSymbols
	setflag FLAG_MET_BATTLE_FRONTIER_GAMBLER
	msgbox BattleFrontier_Lounge3_Text_YouLookToughExplainGambling, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_AskToEnterChallenge
	end

BattleFrontier_Lounge3_EventScript_AskToEnterChallenge::
	special ShowFrontierGamblerLookingMessage
	waitmessage
	waitbuttonpress
	msgbox BattleFrontier_Lounge3_Text_HowAboutEnteringEventForMe, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge3_EventScript_DeclineChallenge
	msgbox BattleFrontier_Lounge3_Text_SpotMeSomeBattlePoints, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge3_EventScript_DeclineChallenge
	message BattleFrontier_Lounge3_Text_HowMuchCanYouSpot
	waitmessage
	special ShowBattlePointsWindow
	goto BattleFrontier_Lounge3_EventScript_ChooseBetAmount
	end

BattleFrontier_Lounge3_EventScript_ChooseBetAmount::
	multichoice 20, 4, MULTI_FRONTIER_GAMBLER_BET, FALSE
	copyvar VAR_FRONTIER_GAMBLER_AMOUNT_BET, VAR_RESULT
	switch VAR_RESULT
	case FRONTIER_GAMBLER_BET_5, BattleFrontier_Lounge3_EventScript_Bet5
	case FRONTIER_GAMBLER_BET_10, BattleFrontier_Lounge3_EventScript_Bet10
	case FRONTIER_GAMBLER_BET_15, BattleFrontier_Lounge3_EventScript_Bet15
	case FRONTIER_GAMBLER_BET_CANCEL, BattleFrontier_Lounge3_EventScript_CancelBet
	case MULTI_B_PRESSED, BattleFrontier_Lounge3_EventScript_CancelBet
	end

BattleFrontier_Lounge3_EventScript_Bet5::
	setvar VAR_0x8008, BET_AMOUNT_5
	goto BattleFrontier_Lounge3_EventScript_TryPlaceBet
	end

BattleFrontier_Lounge3_EventScript_Bet10::
	setvar VAR_0x8008, BET_AMOUNT_10
	goto BattleFrontier_Lounge3_EventScript_TryPlaceBet
	end

BattleFrontier_Lounge3_EventScript_Bet15::
	setvar VAR_0x8008, BET_AMOUNT_15
	goto BattleFrontier_Lounge3_EventScript_TryPlaceBet
	end

BattleFrontier_Lounge3_EventScript_TryPlaceBet::
	specialvar VAR_TEMP_1, GetFrontierBattlePoints
	goto_if_ge VAR_TEMP_1, VAR_0x8008, BattleFrontier_Lounge3_EventScript_PlaceBet
	msgbox BattleFrontier_Lounge3_Text_YouDontHaveEnoughPoints, MSGBOX_DEFAULT
	message BattleFrontier_Lounge3_Text_HowMuchCanYouSpot
	waitmessage
	goto BattleFrontier_Lounge3_EventScript_ChooseBetAmount
	end

BattleFrontier_Lounge3_EventScript_PlaceBet::
	copyvar VAR_0x8004, VAR_0x8008
	special TakeFrontierBattlePoints
	setvar VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_PLACED_BET
	special UpdateBattlePointsWindow
	playse SE_SHOP
	msgbox BattleFrontier_Lounge3_Text_ThanksOffYouGo, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FinishBet
	end

BattleFrontier_Lounge3_EventScript_FinishBet::
	special ShowFrontierGamblerGoMessage
	waitmessage
	waitbuttonpress
	special CloseBattlePointsWindow
	release
	end

BattleFrontier_Lounge3_EventScript_CountSilverSymbols::
	setvar VAR_0x8004, 0
	call_if_set FLAG_SYS_TOWER_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_DOME_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_PALACE_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_ARENA_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_FACTORY_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_PIKE_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_PYRAMID_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	return

BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount::
	addvar VAR_0x8004, 1
	return

BattleFrontier_Lounge3_EventScript_NotEnoughSilverSymbols::
	msgbox BattleFrontier_Lounge3_Text_CantYouSeeWereBusyHere, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_AlreadyMetGambler::
	msgbox BattleFrontier_Lounge3_Text_Oh, MSGBOX_DEFAULT
	goto_if_ge VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_PLACED_BET, BattleFrontier_Lounge3_EventScript_CheckBetResults
	goto BattleFrontier_Lounge3_EventScript_AskToEnterChallenge
	end

BattleFrontier_Lounge3_EventScript_CheckBetResults::
	goto_if_eq VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_PLACED_BET, BattleFrontier_Lounge3_EventScript_ChallengeNotAttempted
	goto_if_eq VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_WON, BattleFrontier_Lounge3_EventScript_WonChallenge
	goto BattleFrontier_Lounge3_EventScript_LostChallenge
	end

BattleFrontier_Lounge3_EventScript_WonChallenge::
	msgbox BattleFrontier_Lounge3_Text_HelloChampHeresYourPoints, MSGBOX_DEFAULT
	call_if_eq VAR_FRONTIER_GAMBLER_AMOUNT_BET, FRONTIER_GAMBLER_BET_5, BattleFrontier_Lounge3_EventScript_RewardBet5
	call_if_eq VAR_FRONTIER_GAMBLER_AMOUNT_BET, FRONTIER_GAMBLER_BET_10, BattleFrontier_Lounge3_EventScript_RewardBet10
	call_if_eq VAR_FRONTIER_GAMBLER_AMOUNT_BET, FRONTIER_GAMBLER_BET_15, BattleFrontier_Lounge3_EventScript_RewardBet15
	msgbox BattleFrontier_Lounge3_Text_ObtainedBattlePoints, MSGBOX_GETPOINTS
	special GiveFrontierBattlePoints
	msgbox BattleFrontier_Lounge3_Text_ThinkOfMeForAnotherChallenge, MSGBOX_DEFAULT
	setvar VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_WAITING
	release
	end

BattleFrontier_Lounge3_EventScript_LostChallenge::
	msgbox BattleFrontier_Lounge3_Text_NiceTryCantReturnPoints, MSGBOX_DEFAULT
	setvar VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_WAITING
	release
	end

BattleFrontier_Lounge3_EventScript_RewardBet5::
	buffernumberstring STR_VAR_1, (BET_AMOUNT_5 * 2)
	setvar VAR_0x8004, (BET_AMOUNT_5 * 2)
	return

BattleFrontier_Lounge3_EventScript_RewardBet10::
	buffernumberstring STR_VAR_1, (BET_AMOUNT_10 * 2)
	setvar VAR_0x8004, (BET_AMOUNT_10 * 2)
	return

BattleFrontier_Lounge3_EventScript_RewardBet15::
	buffernumberstring STR_VAR_1, (BET_AMOUNT_15 * 2)
	setvar VAR_0x8004, (BET_AMOUNT_15 * 2)
	return

BattleFrontier_Lounge3_EventScript_ChallengeNotAttempted::
	special ShowFrontierGamblerGoMessage
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_Lounge3_EventScript_DeclineChallenge::
	msgbox BattleFrontier_Lounge3_Text_NotInterested, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge3_EventScript_CancelBet::
	special CloseBattlePointsWindow
	goto BattleFrontier_Lounge3_EventScript_DeclineChallenge
	end

BattleFrontier_Lounge3_EventScript_Man::
	msgbox BattleFrontier_Lounge3_Text_ShouldBeTakingChallenges, MSGBOX_NPC
	end

BattleFrontier_Lounge3_EventScript_Woman::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge3_Text_BackedWrongTrainer, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_PokefanF::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge3_Text_KnowWinnerWhenISeeOne, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_FatMan::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge3_Text_TrainerGoodButRattled, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_FaceOriginalDirection::
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

BattleFrontier_Lounge3_Text_CantYouSeeWereBusyHere::
	.string "…Eh? Che cosa vuoi?\p"
	.string "Non vedi che qui siamo un po' occupati?\n"
	.string "Non puoi ripassare più tardi?$"

BattleFrontier_Lounge3_Text_YouLookToughExplainGambling::
	.string "…Mmh?\n"
	.string "Tu mi sembri un Allenatore tosto.\p"
	.string "Eheh…\n"
	.string "Senti un po', ho una proposta per te.\p"
	.string "Qui abbiamo messo su un gruppetto,\n"
	.string "e facciamo un giochino con quello\l"
	.string "che succede al Parco Lotta.\p"
	.string "Le regole sono semplicissime.\p"
	.string "Per prima cosa, scegliamo uno degli\n"
	.string "edifici del Parco Lotta.\p"
	.string "Poi ognuno sceglie un Allenatore\n"
	.string "diverso tra quelli che affrontano\l"
	.string "la sfida di quell'edificio…\l"
	.string "e ci scommette i suoi Punti Lotta.\p"
	.string "Chi ha puntato sull'Allenatore con\n"
	.string "il risultato migliore si porta a casa\l"
	.string "tutti i Punti Lotta del piatto.\p"
	.string "Facile, no?\n"
	.string "Allora, veniamo al dunque…$"

BattleFrontier_Lounge3_Text_ChallengeBattleTowerSingle::
	.string "Mi serve un Allenatore che stia per\n"
	.string "affrontare le Sale Lotta in Singolo\l"
	.string "della Torre Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattleTowerDouble::
	.string "Mi serve un Allenatore che stia per\n"
	.string "affrontare le Sale Lotta in Doppio\l"
	.string "della Torre Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattleTowerMulti::
	.string "Mi serve un Allenatore che stia per\n"
	.string "affrontare le Sale Lotta Multipla\l"
	.string "della Torre Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattleDomeSingle::
	.string "Mi serve un Allenatore che stia per\n"
	.string "iscriversi al Torneo Lotta in Singolo\l"
	.string "della Cupola Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattleDomeDouble::
	.string "Mi serve un Allenatore che stia per\n"
	.string "iscriversi al Torneo Lotta in Doppio\l"
	.string "della Cupola Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattleFactorySingle::
	.string "Mi serve un Allenatore che stia per\n"
	.string "iscriversi al Torneo Lotta Scambio\l"
	.string "in Singolo dell'Azienda Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattleFactoryDouble::
	.string "Mi serve un Allenatore che stia per\n"
	.string "iscriversi al Torneo Lotta Scambio\l"
	.string "in Doppio dell'Azienda Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattlePalaceSingle::
	.string "Mi serve un Allenatore che stia per\n"
	.string "affrontare le Aree Lotta in Singolo\l"
	.string "del Palazzo Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattlePalaceDouble::
	.string "Mi serve un Allenatore che stia per\n"
	.string "affrontare le Aree Lotta in Doppio\l"
	.string "del Palazzo Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattleArena::
	.string "Mi serve un Allenatore che stia per\n"
	.string "iscriversi al Torneo Tre Turni\l"
	.string "del Dojo Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattlePike::
	.string "Mi serve un Allenatore che stia per\n"
	.string "tentare la Lotta Dilemma\l"
	.string "della Serpe Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_ChallengeBattlePyramid::
	.string "Mi serve un Allenatore che stia per\n"
	.string "tentare la Lotta Esplorazione\l"
	.string "della Piramide Lotta.\p"
	.string "Ma finora non ho visto nessuno\n"
	.string "con la stoffa del vincente.$"

BattleFrontier_Lounge3_Text_HowAboutEnteringEventForMe::
	.string "Ci guadagnerai anche tu, parola mia.\n"
	.string "Allora, che ne dici?\l"
	.string "Partecipi a quella sfida per me?$"

BattleFrontier_Lounge3_Text_SpotMeSomeBattlePoints::
	.string "Perfetto, è proprio quello che volevo.\n"
	.string "E allora… che ne dici di prestarmi\l"
	.string "un po' dei tuoi Punti Lotta?\p"
	.string "Fidati, dopo saprò dimostrarti\n"
	.string "tutta la mia gratitudine.$"

BattleFrontier_Lounge3_Text_HowMuchCanYouSpot::
	.string "Ottimo, ottimo!\n"
	.string "Allora, quanti me ne presti?$"

BattleFrontier_Lounge3_Text_YouDontHaveEnoughPoints::
	.string "Eh no, no, no!\n"
	.string "Non hai abbastanza Punti Lotta!\p"
	.string "Non farmi perdere tempo con questi\n"
	.string "scherzetti, che qui si lavora!$"

BattleFrontier_Lounge3_Text_ThanksOffYouGo::
	.string "Eheh! Grazie mille!\n"
	.string "E adesso, fila!$"

BattleFrontier_Lounge3_Text_GetToBattleTowerSingle::
	.string "Corri subito alle Sale Lotta\n"
	.string "in Singolo della Torre Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattleTowerDouble::
	.string "Corri subito alle Sale Lotta\n"
	.string "in Doppio della Torre Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattleTowerMulti::
	.string "Corri subito alle Sale Lotta\n"
	.string "Multipla della Torre Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattleDomeSingle::
	.string "Corri subito al Torneo Lotta\n"
	.string "in Singolo della Cupola Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattleDomeDouble::
	.string "Corri subito al Torneo Lotta\n"
	.string "in Doppio della Cupola Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattleFactorySingle::
	.string "Corri subito al Torneo Lotta Scambio\n"
	.string "in Singolo dell'Azienda Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattleFactoryDouble::
	.string "Corri subito al Torneo Lotta Scambio\n"
	.string "in Doppio dell'Azienda Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattlePalaceSingle::
	.string "Corri subito alle Aree Lotta\n"
	.string "in Singolo del Palazzo Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattlePalaceDouble::
	.string "Corri subito alle Aree Lotta\n"
	.string "in Doppio del Palazzo Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattleArena::
	.string "Corri subito al Torneo Tre Turni\n"
	.string "del Dojo Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattlePike::
	.string "Corri subito alla Lotta Dilemma\n"
	.string "della Serpe Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_GetToBattlePyramid::
	.string "Corri subito alla Lotta Esplorazione\n"
	.string "della Piramide Lotta!\p"
	.string "Qui bisogna vincere per forza!\n"
	.string "Non sprecare questa occasione!\l"
	.string "Il nostro futuro è nelle tue mani!$"

BattleFrontier_Lounge3_Text_NiceTryCantReturnPoints::
	.string "Ah, sei tu…\n"
	.string "Bel tentativo…\p"
	.string "Mi dispiace dirtelo, ma i tuoi\n"
	.string "Punti Lotta non posso restituirli…\p"
	.string "Prendiamola come una spinta a fare\n"
	.string "meglio la prossima volta!$"

BattleFrontier_Lounge3_Text_HelloChampHeresYourPoints::
	.string "Oh, sì!\n"
	.string "Eccolo qua, il mio campione!\p"
	.string "Lo sapevo che potevi farcela!\n"
	.string "Lo sapevo che ce l'avresti fatta!\l"
	.string "Abbiamo sbancato, tutti e due!\p"
	.string "Ecco i tuoi Punti Lotta e, ovviamente,\n"
	.string "un piccolo extra da parte mia!$"

BattleFrontier_Lounge3_Text_ObtainedBattlePoints::
	.string "{PLAYER} ha ricevuto\n"
	.string "{STR_VAR_1} Punti Lotta.$"

BattleFrontier_Lounge3_Text_ThinkOfMeForAnotherChallenge::
	.string "Se ti va di tentare un'altra sfida,\n"
	.string "ricordati di me, mi raccomando!\p"
	.string "E quando arriveranno sfidanti dal\n"
	.string "Grande Varco… pensa che quote!$"

BattleFrontier_Lounge3_Text_NotInterested::
	.string "Non ti interessa?! Chi non risica\n"
	.string "non rosica, lo sai?$"

BattleFrontier_Lounge3_Text_Oh::
	.string "Oh…$"

BattleFrontier_Lounge3_Text_BackedWrongTrainer::
	.string "Ho puntato di nuovo sull'Allenatore\n"
	.string "sbagliato!\p"
	.string "Forse dovrei lottare e basta,\n"
	.string "come fanno tutti gli altri…$"

BattleFrontier_Lounge3_Text_TrainerGoodButRattled::
	.string "Quell'Allenatore…\p"
	.string "È bravo, ma si agita troppo facilmente\n"
	.string "per resistere alla Cupola Lotta…$"

BattleFrontier_Lounge3_Text_KnowWinnerWhenISeeOne::
	.string "Ihihi!\n"
	.string "Io un vincente lo riconosco al volo!$"

BattleFrontier_Lounge3_Text_ShouldBeTakingChallenges::
	.string "Quegli Allenatori…\n"
	.string "Ma che stanno combinando?\l"
	.string "Dovrebbero andare a lottare, altroché!$"
