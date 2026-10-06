BattleFrontier_ScottsHouse_MapScripts::
	.byte 0

BattleFrontier_ScottsHouse_EventScript_Scott::
	lock
	faceplayer
	goto_if_set FLAG_TEMP_4, BattleFrontier_ScottsHouse_EventScript_GivenBerry
	goto_if_set FLAG_TEMP_3, BattleFrontier_ScottsHouse_EventScript_GivenShield
	goto_if_set FLAG_TEMP_2, BattleFrontier_ScottsHouse_EventScript_GivenBattlePoints
	goto BattleFrontier_ScottsHouse_EventScript_CheckGiveItems
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveItems::
	goto_if_unset FLAG_SCOTT_GIVES_BATTLE_POINTS, BattleFrontier_ScottsHouse_EventScript_WelcomeToFrontier
	goto_if_unset FLAG_COLLECTED_ALL_SILVER_SYMBOLS, BattleFrontier_ScottsHouse_EventScript_CheckSilverSymbols
	goto_if_unset FLAG_COLLECTED_ALL_GOLD_SYMBOLS, BattleFrontier_ScottsHouse_EventScript_CheckGoldSymbols
	goto BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveShield::
	goto_if_unset FLAG_RECEIVED_SILVER_SHIELD, BattleFrontier_ScottsHouse_EventScript_CheckGiveSilverShield
	goto_if_unset FLAG_RECEIVED_GOLD_SHIELD, BattleFrontier_ScottsHouse_EventScript_CheckGiveGoldShield
	goto BattleFrontier_ScottsHouse_EventScript_RandomComment
	end

BattleFrontier_ScottsHouse_EventScript_CheckSilverSymbols::
	goto_if_unset FLAG_SYS_TOWER_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_DOME_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PALACE_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_ARENA_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_FACTORY_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PIKE_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PYRAMID_SILVER, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	msgbox BattleFrontier_ScottsHouse_Text_YouveCollectedAllSilverSymbols, MSGBOX_DEFAULT
	giveitem ITEM_LANSAT_BERRY
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_BerryPocketFull
	setflag FLAG_COLLECTED_ALL_SILVER_SYMBOLS
	setflag FLAG_TEMP_4
	release
	end

BattleFrontier_ScottsHouse_EventScript_CheckGoldSymbols::
	goto_if_unset FLAG_SYS_TOWER_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_DOME_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PALACE_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_ARENA_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_FACTORY_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PIKE_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	goto_if_unset FLAG_SYS_PYRAMID_GOLD, BattleFrontier_ScottsHouse_EventScript_CheckGiveShield
	msgbox BattleFrontier_ScottsHouse_Text_YouveCollectedAllGoldSymbols, MSGBOX_DEFAULT
	giveitem ITEM_STARF_BERRY
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_BerryPocketFull
	setflag FLAG_COLLECTED_ALL_GOLD_SYMBOLS
	setflag FLAG_TEMP_4
	release
	end

BattleFrontier_ScottsHouse_EventScript_BerryPocketFull::
	msgbox BattleFrontier_ScottsHouse_Text_BerryPocketStuffed, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_GivenBerry::
	msgbox BattleFrontier_ScottsHouse_Text_SoGladIBroughtYouHere, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_RandomComment::
	random 3
	goto_if_eq VAR_RESULT, 1, BattleFrontier_ScottsHouse_EventScript_FrontierBrainComment
	goto_if_eq VAR_RESULT, 2, BattleFrontier_ScottsHouse_EventScript_ArtisanCaveComment
	msgbox BattleFrontier_ScottsHouse_Text_WhyIGoSeekingTrainers, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_FrontierBrainComment::
	msgbox BattleFrontier_ScottsHouse_Text_HaveYouMetFrontierBrain, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_ArtisanCaveComment::
	msgbox BattleFrontier_ScottsHouse_Text_MayFindWildMonsInFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveSilverShield::
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_50
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 50, BattleFrontier_ScottsHouse_EventScript_GiveSilverShield
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_OPEN
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 50, BattleFrontier_ScottsHouse_EventScript_GiveSilverShield
	goto BattleFrontier_ScottsHouse_EventScript_RandomComment
	end

BattleFrontier_ScottsHouse_EventScript_GiveSilverShield::
	msgbox BattleFrontier_ScottsHouse_Text_Beat50TrainersInARow, MSGBOX_DEFAULT
	givedecoration DECOR_SILVER_SHIELD
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_NoRoomForShield
	setflag FLAG_RECEIVED_SILVER_SHIELD
	setflag FLAG_TEMP_3
	goto BattleFrontier_ScottsHouse_EventScript_GivenShield
	end

BattleFrontier_ScottsHouse_EventScript_NoRoomForShield::
	msgbox BattleFrontier_ScottsHouse_Text_ComeBackForThisLater, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_GivenShield::
	msgbox BattleFrontier_ScottsHouse_Text_ExpectingToHearEvenGreaterThings, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_CheckGiveGoldShield::
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_50
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 100, BattleFrontier_ScottsHouse_EventScript_GiveGoldShield
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_OPEN
	tower_get TOWER_DATA_WIN_STREAK
	goto_if_ge VAR_RESULT, 100, BattleFrontier_ScottsHouse_EventScript_GiveGoldShield
	goto BattleFrontier_ScottsHouse_EventScript_RandomComment
	end

BattleFrontier_ScottsHouse_EventScript_GiveGoldShield::
	msgbox BattleFrontier_ScottsHouse_Text_Beat100TrainersInARow, MSGBOX_DEFAULT
	givedecoration DECOR_GOLD_SHIELD
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_ScottsHouse_EventScript_NoRoomForShield
	setflag FLAG_RECEIVED_GOLD_SHIELD
	setflag FLAG_TEMP_3
	goto BattleFrontier_ScottsHouse_EventScript_GivenShield
	end

BattleFrontier_ScottsHouse_EventScript_GivenBattlePoints::
	msgbox BattleFrontier_ScottsHouse_Text_ExpectingGreatThings, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ScottsHouse_EventScript_WelcomeToFrontier::
	msgbox BattleFrontier_ScottsHouse_Text_WelcomeToBattleFrontier, MSGBOX_DEFAULT
	closemessage
	delay 30
	call_if_eq VAR_FACING, DIR_NORTH, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayNorth
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwaySouth
	call_if_eq VAR_FACING, DIR_EAST, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayEast
	call_if_eq VAR_FACING, DIR_WEST, BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayWest
	msgbox BattleFrontier_ScottsHouse_Text_HowMuchEffortItTookToMakeReal, MSGBOX_DEFAULT
	applymovement LOCALID_SCOTTS_HOUSE_SCOTT, Common_Movement_FacePlayer
	waitmovement 0
	msgbox BattleFrontier_ScottsHouse_Text_HaveThisAsMementoOfOurPathsCrossing, MSGBOX_DEFAULT
	goto_if_eq VAR_SCOTT_STATE, 13, BattleFrontier_ScottsHouse_EventScript_Give4BattlePoints
	goto_if_ge VAR_SCOTT_STATE, 9, BattleFrontier_ScottsHouse_EventScript_Give3BattlePoints
	goto_if_ge VAR_SCOTT_STATE, 6, BattleFrontier_ScottsHouse_EventScript_Give2BattlePoints
	goto BattleFrontier_ScottsHouse_EventScript_Give1BattlePoint
	end

BattleFrontier_ScottsHouse_EventScript_Give4BattlePoints::
	buffernumberstring STR_VAR_1, 4
	setvar VAR_0x8004, 4
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_Give3BattlePoints::
	buffernumberstring STR_VAR_1, 3
	setvar VAR_0x8004, 3
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_Give2BattlePoints::
	buffernumberstring STR_VAR_1, 2
	setvar VAR_0x8004, 2
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_Give1BattlePoint::
	buffernumberstring STR_VAR_1, 1
	setvar VAR_0x8004, 1
	goto BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints
	end

BattleFrontier_ScottsHouse_EventScript_GiveBattlePoints::
	special GiveFrontierBattlePoints
	msgbox BattleFrontier_ScottsHouse_Text_ObtainedXBattlePoints, MSGBOX_GETPOINTS
	msgbox BattleFrontier_ScottsHouse_Text_ExplainBattlePoints, MSGBOX_DEFAULT
	setflag FLAG_SCOTT_GIVES_BATTLE_POINTS
	setflag FLAG_TEMP_2
	release
	end

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayNorth::
	applymovement LOCALID_SCOTTS_HOUSE_SCOTT, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwaySouth::
	applymovement LOCALID_SCOTTS_HOUSE_SCOTT, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	return

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayEast::
	applymovement LOCALID_SCOTTS_HOUSE_SCOTT, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

BattleFrontier_ScottsHouse_EventScript_ScottFaceAwayWest::
	applymovement LOCALID_SCOTTS_HOUSE_SCOTT, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

BattleFrontier_ScottsHouse_Text_WelcomeToBattleFrontier:
	.string "Scott: Ehilà, benvenuto!\n"
	.string "Ehehe… Scusa se è un po' stretto.\p"
	.string "Comunque, {PLAYER}{KUN}, lascia che ti dia\n"
	.string "il benvenuto ufficiale al Parco Lotta.\p"
	.string "È il mio sogno che si avvera: il primo\n"
	.string "posto dove Allenatori di mondi diversi\l"
	.string "potranno sfidarsi!\p"
	.string "Ci ho messo anni e anni, ma alla fine\n"
	.string "gli ho dato una forma concreta.$"

BattleFrontier_ScottsHouse_Text_HowMuchEffortItTookToMakeReal:
	.string "A ripensarci, è stato un viaggio\n"
	.string "lunghissimo…\p"
	.string "Sono partito da solo, in cerca di\n"
	.string "Allenatori forti.\p"
	.string "E ogni volta che si apriva un Varco,\n"
	.string "mi chiedevo chi ci fosse di là…\p"
	.string "Nessuno può immaginare quanta fatica\n"
	.string "e quanto tempo ci siano voluti.$"

BattleFrontier_ScottsHouse_Text_HaveThisAsMementoOfOurPathsCrossing:
	.string "Ma ormai è acqua passata.\n"
	.string "Inutile rimuginarci sopra!\p"
	.string "Voglio solo che tu ti goda le lotte\n"
	.string "qui, quanto ti pare e piace.\p"
	.string "{PLAYER}{KUN}, accetta questo come ricordo\n"
	.string "di tutte le volte che le nostre strade\l"
	.string "si sono incrociate.$"

BattleFrontier_ScottsHouse_Text_ObtainedXBattlePoints:
	.string "{PLAYER} ha ottenuto\n"
	.string "{STR_VAR_1} PL (Punti Lotta)!$"

BattleFrontier_ScottsHouse_Text_ExplainBattlePoints:
	.string "Scott: Puoi controllare i tuoi Punti\n"
	.string "Lotta sul Pass Lotta.\p"
	.string "Più successi otterrai qui al Parco\n"
	.string "Lotta, più Punti Lotta accumulerai.\p"
	.string "Usali come preferisci: per esempio,\n"
	.string "scambiali con degli strumenti.$"

BattleFrontier_ScottsHouse_Text_ExpectingGreatThings:
	.string "Mi aspetto grandi cose da te!$"

BattleFrontier_ScottsHouse_Text_WhyIGoSeekingTrainers:
	.string "Scott: Ogni Allenatore è unico.\n"
	.string "Ognuno ha la sua vita, la sua storia.\p"
	.string "Ma quando lotta, non importa chi sia\n"
	.string "né da dove venga.\p"
	.string "Sono tutti uguali!\p"
	.string "Adoro la purezza della lotta,\n"
	.string "dove tutti partono alla pari.\p"
	.string "Per questo giro ovunque in cerca di\n"
	.string "Allenatori che fanno sul serio,\l"
	.string "e li invito qui.\p"
	.string "E ora che il Grande Varco è aperto…\n"
	.string "chissà chi arriverà da altri mondi!$"

BattleFrontier_ScottsHouse_Text_HaveYouMetFrontierBrain:
	.string "Scott: Hai già incontrato qualcuno\n"
	.string "degli Assi del Parco?\p"
	.string "Anzi, meglio: hai già ottenuto\n"
	.string "qualche simbolo da loro?\p"
	.string "Ho scelto gli Assi del Parco uno a uno:\n"
	.string "sono gli Allenatori più tosti in giro.\p"
	.string "Ma sono sicuro che persino loro\n"
	.string "resteranno a bocca aperta, con te!$"

BattleFrontier_ScottsHouse_Text_MayFindWildMonsInFrontier:
	.string "Scott: Tu non ti alleni solo per\n"
	.string "lottare, vero?\p"
	.string "Mi pare di ricordare che stai\n"
	.string "completando anche un Pokédex.\p"
	.string "Be', indovina un po'?\p"
	.string "Da qualche parte nel Parco Lotta\n"
	.string "potresti trovare Pokémon selvatici…\l"
	.string "Fufufu!$"

BattleFrontier_ScottsHouse_Text_YouveCollectedAllSilverSymbols:
	.string "Scott: Ti stai divertendo qui al\n"
	.string "Parco Lotta?\p"
	.string "…Aspetta un attimo…\n"
	.string "Oh!\p"
	.string "Il tuo Pass Lotta!\n"
	.string "Hai raccolto tutti quanti i simboli\l"
	.string "d'argento!\p"
	.string "È più che impressionante!\n"
	.string "Sei forte, proprio come sospettavo!\p"
	.string "Di solito non faccio cose del genere,\n"
	.string "ma questa è un'occasione speciale!\p"
	.string "Voglio che tu abbia questa.\n"
	.string "Sono sicuro che saprai usarla bene.$"

BattleFrontier_ScottsHouse_Text_YouveCollectedAllGoldSymbols:
	.string "Scott: Spero che ti stia godendo\n"
	.string "tutto ciò che offre il Parco Lotta.\p"
	.string "…Aspetta un attimo…\n"
	.string "Oh, non credo ai miei occhi!\p"
	.string "Il tuo Pass Lotta!\n"
	.string "Hai raccolto tutti quanti i simboli\l"
	.string "d'oro!\p"
	.string "È più che stupefacente!\n"
	.string "Sei incredibile, come sospettavo!\p"
	.string "… … … … … …\p"
	.string "{PLAYER}, devi accettare questa!\p"
	.string "Sono certo che saprai apprezzare\n"
	.string "il valore del mio regalo!$"

BattleFrontier_ScottsHouse_Text_SoGladIBroughtYouHere:
	.string "Devo dire che ho un dono speciale per\n"
	.string "riconoscere un buon Allenatore.\p"
	.string "Sono proprio contento di aver avuto\n"
	.string "il fiuto di portarti qui!$"

BattleFrontier_ScottsHouse_Text_BerryPocketStuffed:
	.string "La tua tasca Bacche sembra\n"
	.string "piena zeppa.$"

BattleFrontier_ScottsHouse_Text_Beat50TrainersInARow:
	.string "Scott: Oh, ho sentito parlare di te!\n"
	.string "Hai dominato la Torre Lotta!\l"
	.string "Più di 50 Allenatori battuti di fila?\p"
	.string "È fantastico!\n"
	.string "Voglio che tu abbia questo!$"

BattleFrontier_ScottsHouse_Text_Beat100TrainersInARow:
	.string "Scott: Accidenti! Ho sentito di te!\n"
	.string "Hai travolto la Torre Lotta!\l"
	.string "Più di 100 Allenatori battuti di fila?\p"
	.string "È spettacolare, roba da non credere!\n"
	.string "Questo deve essere tuo!$"

BattleFrontier_ScottsHouse_Text_ExpectingToHearEvenGreaterThings:
	.string "Ora mi aspetto di sentire cose ancora\n"
	.string "più grandi su di te!$"

BattleFrontier_ScottsHouse_Text_ComeBackForThisLater:
	.string "Ops… Be', se hai troppa roba,\n"
	.string "torna a prenderlo più tardi.$"

