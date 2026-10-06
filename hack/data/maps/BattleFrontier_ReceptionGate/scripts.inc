BattleFrontier_ReceptionGate_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_ReceptionGate_OnFrame
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_ReceptionGate_OnTransition
	.byte 0

BattleFrontier_ReceptionGate_OnTransition:
	setflag FLAG_LANDMARK_BATTLE_FRONTIER
	end

BattleFrontier_ReceptionGate_OnFrame:
	map_script_2 VAR_HAS_ENTERED_BATTLE_FRONTIER, 0, BattleFrontier_ReceptionGate_EventScript_FirstTimeEntering
	.2byte 0

BattleFrontier_ReceptionGate_EventScript_FirstTimeEntering::
	lockall
	setvar VAR_HAS_ENTERED_BATTLE_FRONTIER, 1
	playse SE_PIN
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_Delay48
	waitmovement 0
	msgbox BattleFrontier_ReceptionGate_Text_FirstTimeHereThisWay, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, BattleFrontier_ReceptionGate_Movement_PlayerApproachCounter
	waitmovement 0
	goto BattleFrontier_ReceptionGate_EventScript_ScottScene
	end

BattleFrontier_ReceptionGate_EventScript_ScottScene::
	msgbox BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier, MSGBOX_DEFAULT
	msgbox BattleFrontier_ReceptionGate_Text_IssueFrontierPass, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message BattleFrontier_ReceptionGate_Text_ObtainedFrontierPass
	waitfanfare
	waitmessage
	msgbox BattleFrontier_ReceptionGate_Text_PlacedTrainerCardInFrontierPass, MSGBOX_DEFAULT
	setflag FLAG_SYS_FRONTIER_PASS
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	msgbox BattleFrontier_ReceptionGate_Text_IfItIsntPlayerYouCame, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_FRONTIER_RECEPTION_GUIDE, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_ExclamationMark
	applymovement LOCALID_FRONTIER_RECEPTION_GUIDE, Common_Movement_ExclamationMark
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox BattleFrontier_ReceptionGate_Text_OhMrScottGoodDay, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, BattleFrontier_ReceptionGate_Movement_GreeterFaceScott
	applymovement LOCALID_FRONTIER_RECEPTION_GUIDE, BattleFrontier_ReceptionGate_Movement_FacilityGuideFaceScott
	applymovement LOCALID_PLAYER, BattleFrontier_ReceptionGate_Movement_PlayerFaceScott
	applymovement LOCALID_FRONTIER_RECEPTION_SCOTT, BattleFrontier_ReceptionGate_Movement_ScottEnter
	waitmovement 0
	msgbox BattleFrontier_ReceptionGate_Text_ScottGreatToSeeYouHere, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_RECEPTION_SCOTT, BattleFrontier_ReceptionGate_Movement_ScottExit
	waitmovement 0
	removeobject LOCALID_FRONTIER_RECEPTION_SCOTT
	releaseall
	end

BattleFrontier_ReceptionGate_Movement_PlayerApproachCounter:
	walk_up
	walk_up
	walk_left
	walk_left
	step_end

BattleFrontier_ReceptionGate_Movement_PlayerFaceScott:
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

@ Unused
BattleFrontier_ReceptionGate_Movement_WalkDown:
	walk_down
	walk_down
	step_end

BattleFrontier_ReceptionGate_Movement_ScottEnter:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	step_end

BattleFrontier_ReceptionGate_Movement_ScottExit:
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

BattleFrontier_ReceptionGate_Movement_GreeterFaceScott:
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

BattleFrontier_ReceptionGate_Movement_FacilityGuideFaceScott:
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

BattleFrontier_ReceptionGate_EventScript_Greeter::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier, MSGBOX_DEFAULT
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_EventScript_FacilityGuide::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_YourGuideToFacilities, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout::
	message BattleFrontier_ReceptionGate_Text_LearnAboutWhich2
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_BF_RECEPTIONIST
	special ShowScrollableMultichoice
	switch VAR_RESULT
	case 0, BattleFrontier_ReceptionGate_EventScript_BattleTower
	case 1, BattleFrontier_ReceptionGate_EventScript_BattleDome
	case 2, BattleFrontier_ReceptionGate_EventScript_BattlePalace
	case 3, BattleFrontier_ReceptionGate_EventScript_BattleArena
	case 4, BattleFrontier_ReceptionGate_EventScript_BattleFactory
	case 5, BattleFrontier_ReceptionGate_EventScript_BattlePike
	case 6, BattleFrontier_ReceptionGate_EventScript_BattlePyramid
	case 7, BattleFrontier_ReceptionGate_EventScript_RankingHall
	case 8, BattleFrontier_ReceptionGate_EventScript_ExchangeCorner
	case 9, BattleFrontier_ReceptionGate_EventScript_ExitFacilityGuide
	case MULTI_B_PRESSED, BattleFrontier_ReceptionGate_EventScript_ExitFacilityGuide
	end

BattleFrontier_ReceptionGate_EventScript_BattleTower::
	msgbox BattleFrontier_ReceptionGate_Text_BattleTowerInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattleDome::
	msgbox BattleFrontier_ReceptionGate_Text_BattleDomeInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePalace::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePalaceInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattleArena::
	msgbox BattleFrontier_ReceptionGate_Text_BattleArenaInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattleFactory::
	msgbox BattleFrontier_ReceptionGate_Text_BattleFactoryInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePike::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePikeInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePyramid::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePyramidInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_RankingHall::
	msgbox BattleFrontier_ReceptionGate_Text_RankingHallInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExchangeCorner::
	msgbox BattleFrontier_ReceptionGate_Text_ExchangeCornerInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExitFacilityGuide::
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_EventScript_RulesGuide::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_YourGuideToRules, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout::
	message BattleFrontier_ReceptionGate_Text_LearnAboutWhat
	waitmessage
	multichoice 15, 0, MULTI_FRONTIER_RULES, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_ReceptionGate_EventScript_LevelMode
	case 1, BattleFrontier_ReceptionGate_EventScript_Level50
	case 2, BattleFrontier_ReceptionGate_EventScript_OpenLevel
	case 3, BattleFrontier_ReceptionGate_EventScript_MonEntry
	case 4, BattleFrontier_ReceptionGate_EventScript_HoldItems
	case 5, BattleFrontier_ReceptionGate_EventScript_ExitRulesGuide
	case MULTI_B_PRESSED, BattleFrontier_ReceptionGate_EventScript_ExitRulesGuide
	end

BattleFrontier_ReceptionGate_EventScript_LevelMode::
	msgbox BattleFrontier_ReceptionGate_Text_LevelModeInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_Level50::
	msgbox BattleFrontier_ReceptionGate_Text_Level50Info, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_OpenLevel::
	msgbox BattleFrontier_ReceptionGate_Text_OpenLevelInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_MonEntry::
	msgbox BattleFrontier_ReceptionGate_Text_MonEntryInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_HoldItems::
	msgbox BattleFrontier_ReceptionGate_Text_HoldItemsInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExitRulesGuide::
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_EventScript_FrontierPassGuide::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_YourGuideToFrontierPass, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout::
	message BattleFrontier_ReceptionGate_Text_LearnAboutWhich1
	waitmessage
	multichoice 16, 4, MULTI_FRONTIER_PASS_INFO, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_ReceptionGate_EventScript_Symbols
	case 1, BattleFrontier_ReceptionGate_EventScript_RecordBattle
	case 2, BattleFrontier_ReceptionGate_EventScript_BattlePoints
	case 3, BattleFrontier_ReceptionGate_EventScript_ExitFrontierPassGuide
	case MULTI_B_PRESSED, BattleFrontier_ReceptionGate_EventScript_ExitFrontierPassGuide
	end

BattleFrontier_ReceptionGate_EventScript_Symbols::
	msgbox BattleFrontier_ReceptionGate_Text_SymbolsInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_RecordBattle::
	msgbox BattleFrontier_ReceptionGate_Text_RecordedBattleInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePoints::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePointsInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExitFrontierPassGuide::
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_Text_FirstTimeHereThisWay:
	.string "È la prima volta che vieni qui?\n"
	.string "Prego, da questa parte!$"

BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier:
	.string "L'avanguardia delle lotte Pokémon!\n"
	.string "Benvenuto al Parco Lotta!$"

BattleFrontier_ReceptionGate_Text_IssueFrontierPass:
	.string "A chi ci visita per la prima volta\n"
	.string "consegniamo il Pass Lotta.\p"
	.string "Serve per accedere a tutte le\n"
	.string "strutture del Parco Lotta.\p"
	.string "Ecco a te!$"

BattleFrontier_ReceptionGate_Text_ObtainedFrontierPass:
	.string "{PLAYER} ottiene\n"
	.string "il Pass Lotta!$"

BattleFrontier_ReceptionGate_Text_PlacedTrainerCardInFrontierPass:
	.string "{PLAYER} inserisce la Scheda\n"
	.string "Allenatore nel Pass Lotta.$"

BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier:
	.string "Speriamo che tu possa goderti tutto\n"
	.string "ciò che il Parco Lotta ha da offrire!$"

BattleFrontier_ReceptionGate_Text_IfItIsntPlayerYouCame:
	.string "???: Ma guarda chi si vede! {PLAYER}{KUN}!\n"
	.string "Sei venuto fin qui!$"

BattleFrontier_ReceptionGate_Text_OhMrScottGoodDay:
	.string "Guida: Oh! Signor Scott!\n"
	.string "I miei omaggi, signore!$"

BattleFrontier_ReceptionGate_Text_ScottGreatToSeeYouHere:
	.string "Scott: Che bello vederti qui,\n"
	.string "davvero!\p"
	.string "Spero che te la prenderai comoda\n"
	.string "e che esplorerai ogni angolo.\p"
	.string "E naturalmente spero che vivrai\n"
	.string "l'essenza più pura della lotta.\p"
	.string "Ho costruito questo posto per gli\n"
	.string "Allenatori di ogni mondo… anche per\l"
	.string "quelli che arriveranno dal Grande\l"
	.string "Varco.\p"
	.string "Ho anche il mio alloggio qui, quindi\n"
	.string "passa a trovarmi quando hai tempo.$"

BattleFrontier_ReceptionGate_Text_YourGuideToFacilities:
	.string "Ti illustrerò le varie strutture\n"
	.string "del Parco Lotta.$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhich2:
	.string "Di quale struttura vuoi sapere?$"

BattleFrontier_ReceptionGate_Text_BattleTowerInfo:
	.string "È la torre gigantesca considerata il\n"
	.string "simbolo del Parco Lotta.\p"
	.string "Al suo interno ci sono quattro tipi\n"
	.string "di Sale Lotta: Singola, Doppia,\l"
	.string "Multipla e Multi-Link.$"

BattleFrontier_ReceptionGate_Text_BattleDomeInfo:
	.string "La Cupola Lotta è il grande edificio\n"
	.string "a forma di uovo gigante.\p"
	.string "In questa struttura si svolgono\n"
	.string "gli eventi chiamati Tornei Lotta.\p"
	.string "I Tornei Lotta prevedono due\n"
	.string "categorie: Lotte in Singolo e Lotte\l"
	.string "in Doppio.$"

BattleFrontier_ReceptionGate_Text_BattlePalaceInfo:
	.string "Il Palazzo Lotta è l'edificio rosso\n"
	.string "sul lato destro del Parco Lotta.\p"
	.string "Ci sono due tipi di sale: per Lotte\n"
	.string "in Singolo e per Lotte in Doppio.$"

BattleFrontier_ReceptionGate_Text_BattleArenaInfo:
	.string "Il Dojo Lotta è l'edificio in stile\n"
	.string "orientale, nella zona centro-destra\l"
	.string "del Parco Lotta.\p"
	.string "Al Dojo Lotta si svolge un evento\n"
	.string "chiamato Torneo KO.$"

BattleFrontier_ReceptionGate_Text_BattleFactoryInfo:
	.string "La Azienda Lotta è il grande edificio\n"
	.string "più vicino a noi.\p"
	.string "Lì si svolge un evento chiamato\n"
	.string "Scambio Lotta.\p"
	.string "Lo Scambio Lotta prevede due\n"
	.string "categorie: Lotte in Singolo e Lotte\l"
	.string "in Doppio.$"

BattleFrontier_ReceptionGate_Text_BattlePikeInfo:
	.string "La Serpe Lotta è l'edificio a forma\n"
	.string "di Pokémon nella zona centro-sinistra\l"
	.string "del Parco Lotta.\p"
	.string "Lì si svolge un evento chiamato\n"
	.string "Scelta Lotta.$"

BattleFrontier_ReceptionGate_Text_BattlePyramidInfo:
	.string "La Piramide Lotta è quell'enorme\n"
	.string "piramide.\p"
	.string "Lì si svolge un evento chiamato\n"
	.string "Spedizione Lotta.$"

BattleFrontier_ReceptionGate_Text_RankingHallInfo:
	.string "La Sala dei Record si trova vicino\n"
	.string "alla Torre Lotta.\p"
	.string "Lì potrai ammirare i record più\n"
	.string "straordinari degli Allenatori che\l"
	.string "hanno affrontato le tante sfide\l"
	.string "del Parco Lotta.$"

BattleFrontier_ReceptionGate_Text_ExchangeCornerInfo:
	.string "L'Angolo Scambi si trova vicino\n"
	.string "alla Torre Lotta.\p"
	.string "Lì potrai scambiare i Punti Lotta\n"
	.string "guadagnati al Parco Lotta con dei\l"
	.string "premi favolosi.$"

BattleFrontier_ReceptionGate_Text_YourGuideToRules:
	.string "Ti illustrerò le regole di base\n"
	.string "comuni a tutte le sfide offerte\l"
	.string "dalle strutture del Parco Lotta.$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhat:
	.string "Di cosa vuoi sapere di più?$"

BattleFrontier_ReceptionGate_Text_LevelModeInfo:
	.string "Tutte le sfide delle strutture del\n"
	.string "Parco Lotta prevedono due categorie:\l"
	.string "Livello 50 e Livello Libero.$"

BattleFrontier_ReceptionGate_Text_Level50Info:
	.string "La categoria Livello 50 è aperta ai\n"
	.string "Pokémon fino al livello 50 compreso.\p"
	.string "Tieni presente, però, che nessun\n"
	.string "Allenatore che affronterai avrà\l"
	.string "Pokémon sotto il livello 50.\p"
	.string "Questa categoria è il primo passo\n"
	.string "per lottare al Parco Lotta.\p"
	.string "Per cominciare, ti consigliamo di\n"
	.string "sfidare questa categoria.$"

BattleFrontier_ReceptionGate_Text_OpenLevelInfo:
	.string "La categoria Livello Libero non pone\n"
	.string "limiti al livello dei Pokémon che\l"
	.string "partecipano alle sfide.\p"
	.string "Il livello degli avversari verrà\n"
	.string "adattato a quello dei tuoi\l"
	.string "Pokémon.\p"
	.string "Tuttavia, nessun Allenatore che\n"
	.string "affronterai avrà Pokémon sotto\l"
	.string "il livello 60.$"

BattleFrontier_ReceptionGate_Text_MonEntryInfo:
	.string "Quasi tutti i Pokémon, anche quelli\n"
	.string "arrivati dai Varchi, possono\l"
	.string "partecipare alle sfide di ogni\l"
	.string "struttura.\p"
	.string "Le Uova e alcuni tipi di Pokémon,\n"
	.string "però, non sono ammessi.\p"
	.string "Il numero di Pokémon richiesto per\n"
	.string "le sfide dipende dalla struttura.\p"
	.string "In tutte le strutture, comunque, non\n"
	.string "sono ammessi due o più Pokémon\l"
	.string "della stessa specie.$"

BattleFrontier_ReceptionGate_Text_HoldItemsInfo:
	.string "Quando partecipi a una sfida in una\n"
	.string "struttura del Parco Lotta, i Pokémon\l"
	.string "non possono tenere lo stesso\l"
	.string "strumento.\p"
	.string "Assicurati che tutti i Pokémon\n"
	.string "iscritti alla sfida tengano\l"
	.string "strumenti diversi.$"

BattleFrontier_ReceptionGate_Text_YourGuideToFrontierPass:
	.string "Sono qui per spiegarti il Pass Lotta.$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhich1:
	.string "Di cosa vuoi sapere di più?$"

BattleFrontier_ReceptionGate_Text_SymbolsInfo:
	.string "Il Parco Lotta conta sette\n"
	.string "strutture.\p"
	.string "Gli Allenatori che si distinguono per\n"
	.string "le loro capacità possono ricevere\l"
	.string "un Simbolo da ciascuna struttura.\p"
	.string "Per ottenere un Simbolo può servire\n"
	.string "battere più Allenatori di fila, o\l"
	.string "vincere più volte gli eventi.\p"
	.string "Conquistare i Simboli non è facile.\n"
	.string "Ti auguro buona fortuna!$"

BattleFrontier_ReceptionGate_Text_RecordedBattleInfo:
	.string "Sul Pass Lotta è possibile registrare\n"
	.string "una lotta.\p"
	.string "Puoi registrare una lotta che hai\n"
	.string "fatto con uno o più amici.\p"
	.string "Oppure puoi registrare una lotta\n"
	.string "svolta al Parco Lotta, esclusi gli\l"
	.string "incontri della Serpe Lotta e della\l"
	.string "Piramide Lotta.\p"
	.string "Alla fine di ogni lotta potrai\n"
	.string "scegliere se registrarla.$"

BattleFrontier_ReceptionGate_Text_BattlePointsInfo:
	.string "I Punti Lotta sono ricompense date\n"
	.string "agli Allenatori che hanno lottato in\l"
	.string "modo eccezionale al Parco Lotta.\p"
	.string "Puoi scambiare i tuoi Punti Lotta con\n"
	.string "dei premi all'Angolo Scambi.$"
