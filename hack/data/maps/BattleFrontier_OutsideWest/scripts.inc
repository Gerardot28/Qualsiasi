BattleFrontier_OutsideWest_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_OutsideWest_OnTransition
	.byte 0

BattleFrontier_OutsideWest_OnTransition:
	setvar VAR_BRAVO_TRAINER_BATTLE_TOWER_ON, 0
	setflag FLAG_HIDE_BATTLE_TOWER_REPORTER
	end

BattleFrontier_OutsideWest_EventScript_FerryAttendant::
	lock
	faceplayer
	msgbox BattleFrontier_OutsideWest_Text_MayISeeYourTicket, MSGBOX_DEFAULT
	checkitem ITEM_SS_TICKET
	goto_if_eq VAR_RESULT, FALSE, BattleFrontier_OutsideWest_EventScript_NoSSTicket
	message BattleFrontier_OutsideWest_Text_WhereWouldYouLikeToGo
	waitmessage
	goto BattleFrontier_OutsideWest_EventScript_ChooseFerryDestination
	end

BattleFrontier_OutsideWest_EventScript_ChooseFerryDestination::
	multichoicedefault 18, 6, MULTI_SSTIDAL_BATTLE_FRONTIER, 2, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_OutsideWest_EventScript_FerryToSlateport
	case 1, BattleFrontier_OutsideWest_EventScript_FerryToLilycove
	case 2, BattleFrontier_OutsideWest_EventScript_CancelFerrySelect
	case MULTI_B_PRESSED, BattleFrontier_OutsideWest_EventScript_CancelFerrySelect
	end

BattleFrontier_OutsideWest_EventScript_NoSSTicket::
	msgbox BattleFrontier_OutsideWest_Text_MustHaveTicketToBoard, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_FerryToSlateport::
	msgbox BattleFrontier_OutsideWest_Text_SlateportItIs, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_OutsideWest_EventScript_ChooseNewFerryDestination
	msgbox BattleFrontier_OutsideWest_Text_PleaseBoardFerry, MSGBOX_DEFAULT
	call BattleFrontier_OutsideWest_EventScript_BoardFerry
	warp MAP_SLATEPORT_CITY_HARBOR, 8, 11
	waitstate
	release
	end

BattleFrontier_OutsideWest_EventScript_FerryToLilycove::
	msgbox BattleFrontier_OutsideWest_Text_LilycoveItIs, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_OutsideWest_EventScript_ChooseNewFerryDestination
	msgbox BattleFrontier_OutsideWest_Text_PleaseBoardFerry, MSGBOX_DEFAULT
	call BattleFrontier_OutsideWest_EventScript_BoardFerry
	warp MAP_LILYCOVE_CITY_HARBOR, 8, 11
	waitstate
	release
	end

BattleFrontier_OutsideWest_EventScript_ChooseNewFerryDestination::
	message BattleFrontier_OutsideWest_Text_ThenWhereWouldYouLikeToGo
	waitmessage
	goto BattleFrontier_OutsideWest_EventScript_ChooseFerryDestination
	end

BattleFrontier_OutsideWest_EventScript_BoardFerry::
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 30
	hideobjectat LOCALID_FRONTIER_FERRY_ATTENDANT, MAP_BATTLE_FRONTIER_OUTSIDE_WEST
	setvar VAR_0x8004, LOCALID_FRONTIER_SS_TIDAL
	call Common_EventScript_FerryDepartIsland
	return

BattleFrontier_OutsideWest_EventScript_CancelFerrySelect::
	msgbox BattleFrontier_OutsideWest_Text_SailWithUsAnotherTime, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_BattleDomeSign::
	msgbox BattleFrontier_OutsideWest_Text_BattleDomeSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideWest_EventScript_BattleFactorySign::
	msgbox BattleFrontier_OutsideWest_Text_BattleFactorySign, MSGBOX_SIGN
	end

BattleFrontier_OutsideWest_EventScript_BattlePikeSign::
	msgbox BattleFrontier_OutsideWest_Text_BattlePikeSign, MSGBOX_SIGN
	end

@ Unused. Of note, Battle Tower is in the East section in the final release
BattleFrontier_OutsideWest_EventScript_UnusedNPC1::
	msgbox BattleFrontier_OutsideWest_Text_ThisIsBattleTower, MSGBOX_NPC
	end

@ Unused
BattleFrontier_OutsideWest_EventScript_UnusedNPC2::
	msgbox BattleFrontier_OutsideWest_Text_CantFindBattleTower, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Boy1::
	msgbox BattleFrontier_OutsideWest_Text_BestOutOfAllMyFriends, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Fisherman2::
	lock
	faceplayer
	message BattleFrontier_OutsideWest_Text_GotSeasickOnWayHere
	waitmessage
	applymovement LOCALID_FRONTIER_FISHERMAN_2, Common_Movement_FaceAwayPlayer
	waitmovement 0
	waitbuttonpress
	release
	end

BattleFrontier_OutsideWest_EventScript_Man1::
	msgbox BattleFrontier_OutsideWest_Text_OnlyToughTrainersBroughtHere, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Maniac1::
	lock
	goto BattleFrontier_OutsideWest_EventScript_FactoryChallengersTalk
	end

BattleFrontier_OutsideWest_EventScript_Maniac2::
	lock
	goto BattleFrontier_OutsideWest_EventScript_FactoryChallengersTalk
	end

BattleFrontier_OutsideWest_EventScript_FactoryChallengersTalk::
	applymovement LOCALID_FRONTIER_MANIAC_1, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox BattleFrontier_OutsideWest_Text_SureWeCanChallengeWithNoMons, MSGBOX_DEFAULT
	applymovement LOCALID_FRONTIER_MANIAC_2, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox BattleFrontier_OutsideWest_Text_BigGuySaidIllLendYouMons, MSGBOX_DEFAULT
	closemessage
	delay 25
	applymovement LOCALID_FRONTIER_MANIAC_1, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_FRONTIER_MANIAC_2, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	release
	end

BattleFrontier_OutsideWest_EventScript_Camper::
	lock
	faceplayer
	delay 20
	call_if_eq VAR_FACING, DIR_NORTH, BattleFrontier_OutsideWest_EventScript_CamperFaceFactory
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_OutsideWest_EventScript_CamperAlreadyFacingFactory
	call_if_eq VAR_FACING, DIR_WEST, BattleFrontier_OutsideWest_EventScript_CamperFaceFactory
	call_if_eq VAR_FACING, DIR_EAST, BattleFrontier_OutsideWest_EventScript_CamperFaceFactory
	msgbox BattleFrontier_OutsideWest_Text_WhosRaisingThoseRentalMons, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_CamperFaceFactory::
	applymovement LOCALID_FRONTIER_CAMPER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

BattleFrontier_OutsideWest_EventScript_CamperAlreadyFacingFactory::
	return

BattleFrontier_OutsideWest_EventScript_Girl::
	lock
	faceplayer
	message BattleFrontier_OutsideWest_Text_ScaredOfPikeBecauseSeviper
	waitmessage
	call_if_eq VAR_FACING, DIR_NORTH, BattleFrontier_OutsideWest_EventScript_GirlShudderNorth
	call_if_eq VAR_FACING, DIR_SOUTH, BattleFrontier_OutsideWest_EventScript_GirlShudderSouth
	call_if_eq VAR_FACING, DIR_WEST, BattleFrontier_OutsideWest_EventScript_GirlShudderWest
	call_if_eq VAR_FACING, DIR_EAST, BattleFrontier_OutsideWest_EventScript_GirlShudderEast
	waitbuttonpress
	release
	end

BattleFrontier_OutsideWest_EventScript_GirlShudderNorth::
	applymovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderNorth
	waitmovement 0
	return

BattleFrontier_OutsideWest_EventScript_GirlShudderSouth::
	applymovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderSouth
	waitmovement 0
	return

BattleFrontier_OutsideWest_EventScript_GirlShudderWest::
	applymovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderWest
	waitmovement 0
	return

BattleFrontier_OutsideWest_EventScript_GirlShudderEast::
	applymovement LOCALID_FRONTIER_GIRL, BattleFrontier_OutsideWest_Movement_GirlShudderEast
	waitmovement 0
	return

BattleFrontier_OutsideWest_Movement_GirlShudderNorth:
	walk_in_place_faster_down
	walk_in_place_faster_down
	step_end

BattleFrontier_OutsideWest_Movement_GirlShudderSouth:
	walk_in_place_faster_up
	walk_in_place_faster_up
	step_end

BattleFrontier_OutsideWest_Movement_GirlShudderWest:
	walk_in_place_faster_right
	walk_in_place_faster_right
	step_end

BattleFrontier_OutsideWest_Movement_GirlShudderEast:
	walk_in_place_faster_left
	walk_in_place_faster_left
	step_end

BattleFrontier_OutsideWest_EventScript_Woman2::
	lock
	faceplayer
	msgbox BattleFrontier_OutsideWest_Text_LetsPlayRockPaperScissors, MSGBOX_DEFAULT
	random 2
	goto_if_eq VAR_RESULT, 1, BattleFrontier_OutsideWest_EventScript_WomanWonRockPaperScissors
	goto BattleFrontier_OutsideWest_EventScript_WomanLostRockPaperScissors
	end

BattleFrontier_OutsideWest_EventScript_WomanWonRockPaperScissors::
	msgbox BattleFrontier_OutsideWest_Text_WonIllTakePikeChallenge, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_WomanLostRockPaperScissors::
	msgbox BattleFrontier_OutsideWest_Text_LostIllPutOffPikeChallenge, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_Fisherman1::
	msgbox BattleFrontier_OutsideWest_Text_ChooseFishingOverBattling, MSGBOX_NPC
	end

@ Unused
BattleFrontier_OutsideWest_EventScript_UnusedNPC3::
	msgbox BattleFrontier_OutsideWest_Text_DomeIsHereGrandpa, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Gentleman::
	msgbox BattleFrontier_OutsideWest_Text_YoureOffToChallengeDome, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Lass::
	msgbox BattleFrontier_OutsideWest_Text_KeepBattlingUntilIGetSymbol, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_ExpertM::
	msgbox BattleFrontier_OutsideWest_Text_WontLetGentlemenBeatMe, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Man2::
	msgbox BattleFrontier_OutsideWest_Text_NothingHereNotLongAgo, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Woman1::
	msgbox BattleFrontier_OutsideWest_Text_FinallyArrivedAtFrontier, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_FatMan1::
	msgbox BattleFrontier_OutsideWest_Text_SquareFilledWithToughPeople, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_FatMan2::
	msgbox BattleFrontier_OutsideWest_Text_MetOlderGirlAtPike, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Woman3::
	lock
	msgbox BattleFrontier_OutsideWest_Text_LastTimeOurEyesMet, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_Boy2::
	lock
	msgbox BattleFrontier_OutsideWest_Text_DomeAceLookedBecauseOfMyCheering, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideWest_EventScript_OldMan::
	msgbox BattleFrontier_OutsideWest_Text_DomeAceIsMine, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_EventScript_Man4::
	lock
	faceplayer
	msgbox BattleFrontier_OutsideWest_Text_FansOverThereUsedToBeTrainers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_MAN_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

BattleFrontier_OutsideWest_EventScript_PokefanF::
	msgbox BattleFrontier_OutsideWest_Text_MonWithLongTailInFrontier, MSGBOX_NPC
	end

BattleFrontier_OutsideWest_Text_BattleDomeSign:
	.string "Questa è la Cupola Lotta!\n"
	.string "Diventa una superstar invincibile!$"

BattleFrontier_OutsideWest_Text_BattleFactorySign:
	.string "Questa è la Azienda Lotta!\n"
	.string "Vai a caccia dei Pokémon più forti!$"

BattleFrontier_OutsideWest_Text_BattlePikeSign:
	.string "Questa è la Serpe Lotta!\n"
	.string "Scegli uno dei tre sentieri!$"

BattleFrontier_OutsideWest_Text_ThisIsBattleTower:
	.string "Questa è la Torre Lotta.\p"
	.string "Gli Allenatori portano qui i loro\n"
	.string "Pokémon migliori per sfide a\l"
	.string "eliminazione.$"

BattleFrontier_OutsideWest_Text_MayISeeYourTicket:
	.string "Salve, sei qui per il traghetto?\n"
	.string "Posso vedere il tuo biglietto?$"

BattleFrontier_OutsideWest_Text_MustHaveTicketToBoard:
	.string "{PLAYER} non ha il biglietto…\p"
	.string "Mi dispiace davvero tanto.\p"
	.string "Per salire a bordo del traghetto\n"
	.string "serve un biglietto.$"

BattleFrontier_OutsideWest_Text_WhereWouldYouLikeToGo:
	.string "{PLAYER} mostra il Biglietto Nave.\p"
	.string "Perfetto! Non serve altro!\p"
	.string "Dove desideri andare?$"

BattleFrontier_OutsideWest_Text_SlateportItIs:
	.string "Si va a Porto Selcepoli, allora!$"

BattleFrontier_OutsideWest_Text_LilycoveItIs:
	.string "Si va a Porto Alghepoli, allora!$"

BattleFrontier_OutsideWest_Text_SailWithUsAnotherTime:
	.string "Ti aspettiamo alla prossima traversata!$"

BattleFrontier_OutsideWest_Text_PleaseBoardFerry:
	.string "Prego, sali a bordo e attendi\n"
	.string "la partenza.$"

BattleFrontier_OutsideWest_Text_ThenWhereWouldYouLikeToGo:
	.string "Allora, dove desideri andare?$"

BattleFrontier_OutsideWest_Text_BestOutOfAllMyFriends:
	.string "Tra i miei amici sono il più forte.\n"
	.string "Ma qui…\l"
	.string "non ne ho azzeccata una!$"

BattleFrontier_OutsideWest_Text_CantFindBattleTower:
	.string "Voglio andare alla Torre Lotta, ma\n"
	.string "non riesco a trovarla, eppure ho\l"
	.string "una mappa del Parco Lotta!\p"
	.string "Questo posto è davvero troppo grande!$"

BattleFrontier_OutsideWest_Text_GotSeasickOnWayHere:
	.string "Volevo buttarmi in una sfida appena\n"
	.string "sbarcato.\p"
	.string "Ma durante la traversata mi è venuto\n"
	.string "il mal di mare… Io, un Pescatore…\l"
	.string "Blurp…$"

BattleFrontier_OutsideWest_Text_OnlyToughTrainersBroughtHere:
	.string "Mica può venire qui chiunque,\n"
	.string "sai?\p"
	.string "Vengono portati qui soltanto gli\n"
	.string "Allenatori riconosciuti come forti.\p"
	.string "Ecco perché molti Allenatori non\n"
	.string "sanno neanche che esiste il Parco\l"
	.string "Lotta.$"

BattleFrontier_OutsideWest_Text_SureWeCanChallengeWithNoMons:
	.string "Ehi, fratello…\p"
	.string "Sei sicuro che possiamo partecipare\n"
	.string "anche se non abbiamo Pokémon?$"

BattleFrontier_OutsideWest_Text_BigGuySaidIllLendYouMons:
	.string "Ehm…\n"
	.string "Sono sicuro che andrà tutto bene.\p"
	.string "Credo…\p"
	.string "Ti ricordi quell'omone che fa paura?\n"
	.string "Ha detto: “Vi presto io i Pokémon!”$"

BattleFrontier_OutsideWest_Text_WhosRaisingThoseRentalMons:
	.string "Quella è la Azienda Lotta.\n"
	.string "Lì puoi noleggiare Pokémon forti.\p"
	.string "Però mi chiedo una cosa.\n"
	.string "Chi li alleva, quei Pokémon a nolo?$"

BattleFrontier_OutsideWest_Text_ScaredOfPikeBecauseSeviper:
	.string "Ho paura di entrare nella Serpe\n"
	.string "Lotta per colpa dei Seviper…\p"
	.string "M-ma sono venuta fin qui, quindi\n"
	.string "proverò a conquistare tutto!\l"
	.string "…Brrr…$"

BattleFrontier_OutsideWest_Text_LetsPlayRockPaperScissors:
	.string "Giochiamo a sasso, carta, forbici!\n"
	.string "Bim, bum, bam!\p"
	.string "… … … … … …$"

BattleFrontier_OutsideWest_Text_WonIllTakePikeChallenge:
	.string "Evviva! Ho vinto!\n"
	.string "Allora affronterò la Serpe Lotta!$"

BattleFrontier_OutsideWest_Text_LostIllPutOffPikeChallenge:
	.string "Oh, no…\n"
	.string "Ho perso.\p"
	.string "Oggi la fortuna non è dalla mia.\n"
	.string "Rimando la sfida della Serpe Lotta\l"
	.string "a domani.$"

BattleFrontier_OutsideWest_Text_ChooseFishingOverBattling:
	.string "Credo di essere l'unico, qui, che per\n"
	.string "qualche misteriosa ragione preferisce\l"
	.string "la pesca alle lotte.\p"
	.string "Eh? Qui non abbocca niente?\n"
	.string "Che delusione…$"

BattleFrontier_OutsideWest_Text_KeepBattlingUntilIGetSymbol:
	.string "Oggi continuerò a lottare, costi quel\n"
	.string "che costi, finché non avrò un Simbolo.$"

BattleFrontier_OutsideWest_Text_YoureOffToChallengeDome:
	.string "Oh? Stai andando a sfidare\n"
	.string "la Cupola Lotta?\p"
	.string "Ti auguro tutta la fortuna possibile.\n"
	.string "Facciamoci strada a suon di vittorie,\l"
	.string "e magari ci sfideremo.$"

BattleFrontier_OutsideWest_Text_DomeIsHereGrandpa:
	.string "Nonno, di qua!\n"
	.string "La Cupola Lotta è qui!\l"
	.string "Falli neri, nonno!$"

BattleFrontier_OutsideWest_Text_WontLetGentlemenBeatMe:
	.string "Ah, dunque questa è la Cupola Lotta?\n"
	.string "Nessun Gentiluomo mi batterà!\p"
	.string "Ma dov'è l'ingresso?$"

BattleFrontier_OutsideWest_Text_NothingHereNotLongAgo:
	.string "Fino a non molto tempo fa, qui non\n"
	.string "c'era proprio niente.\p"
	.string "E guarda adesso che posto! Incredibile!\n"
	.string "Ci devo portare mia madre.$"

BattleFrontier_OutsideWest_Text_FinallyArrivedAtFrontier:
	.string "Finalmente sono arrivata al Parco\n"
	.string "Lotta!\p"
	.string "Con il mio fascino e il mio talento\n"
	.string "attirerò di sicuro l'attenzione!$"

BattleFrontier_OutsideWest_Text_SquareFilledWithToughPeople:
	.string "Gnam, gnam…\p"
	.string "Sembra che questa piazza sia piena\n"
	.string "di gente tosta.\p"
	.string "Gnam, gnam…$"

BattleFrontier_OutsideWest_Text_MetOlderGirlAtPike:
	.string "Crunch, gnam…\p"
	.string "Tempo fa, alla Serpe Lotta, ho\n"
	.string "incontrato una ragazza più grande.\p"
	.string "Aveva uno sguardo che faceva paura,\n"
	.string "ma paura sul serio.\p"
	.string "Crunch, gnam…\p"
	.string "Si è infuriata: “Vuoi smetterla di\n"
	.string "mangiare?!” E poi me le ha suonate.\p"
	.string "È stata un'esperienza da far rizzare\n"
	.string "i capelli, poco ma sicuro.\p"
	.string "Crunch, gnam…$"

BattleFrontier_OutsideWest_Text_LastTimeOurEyesMet:
	.string "Eh? Ma ti senti quando parli?\n"
	.string "Quello non è niente!\p"
	.string "L'ultima volta che ho tifato per lui,\n"
	.string "i nostri sguardi si sono incrociati\l"
	.string "ed è scoccata la scintilla!$"

BattleFrontier_OutsideWest_Text_DomeAceLookedBecauseOfMyCheering:
	.string "Coooosa?!\n"
	.string "Pfff!\p"
	.string "È successo solo perché per caso\n"
	.string "ero seduto vicino a te!\p"
	.string "L'Asso Cupola ha guardato da noi\n"
	.string "solo perché io tifavo fortissimo!$"

BattleFrontier_OutsideWest_Text_DomeAceIsMine:
	.string "Oh, silenzio!\n"
	.string "Abbassate la voce!\p"
	.string "Voi tifosi della domenica dovreste\n"
	.string "tifare per qualche Capopalestra!\p"
	.string "L'Asso Cupola è mio!\n"
	.string "È il mio unico idolo!$"

BattleFrontier_OutsideWest_Text_FansOverThereUsedToBeTrainers:
	.string "Quelli che litigano laggiù…\p"
	.string "È difficile da credere, ma un tempo\n"
	.string "erano Allenatori, e anche bravi.\p"
	.string "Erano incredibilmente forti,\n"
	.string "e pure famosi.\p"
	.string "Ma durante un torneo un certo\n"
	.string "Allenatore li ha fatti a pezzi.\p"
	.string "Le hanno prese così di brutto che\n"
	.string "si sono innamorati del vincitore.\p"
	.string "Ora sono solo dei fan sfegatati.\n"
	.string "Ogni giorno vanno alla Cupola Lotta\l"
	.string "a fare il tifo per il loro idolo.$"

BattleFrontier_OutsideWest_Text_MonWithLongTailInFrontier:
	.string "Ho sentito dire che qualcuno ha visto\n"
	.string "un Pokémon dalla coda lunghissima\l"
	.string "da qualche parte nel Parco Lotta.\p"
	.string "Dev'essere adorabile. Ne sono certa…\p"
	.string "Oh, le sfide possono aspettare! Devo\n"
	.string "trovare quella creaturina deliziosa!$"
