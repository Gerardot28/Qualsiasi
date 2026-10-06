SlateportCity_SternsShipyard_1F_MapScripts::
	.byte 0

SlateportCity_SternsShipyard_1F_EventScript_Dock::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, SlateportCity_SternsShipyard_1F_EventScript_FerryReady
	goto_if_set FLAG_BADGE07_GET, SlateportCity_SternsShipyard_1F_EventScript_BrineyJoined
	goto_if_set FLAG_DELIVERED_DEVON_GOODS, SlateportCity_SternsShipyard_1F_EventScript_NeedVeteran
	goto_if_set FLAG_DOCK_REJECTED_DEVON_GOODS, SlateportCity_SternsShipyard_1F_EventScript_GoFindStern
	msgbox SlateportCity_SternsShipyard_1F_Text_CantMakeHeadsOrTails, MSGBOX_DEFAULT
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_MeetDockDeliverToStern, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DOCK, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag FLAG_DOCK_REJECTED_DEVON_GOODS
	setflag FLAG_HIDE_SLATEPORT_CITY_TEAM_AQUA
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_FerryReady::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_FerryIsReady, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_BrineyJoined::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_BrineyJoinedUs, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_GoFindStern::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_CouldYouFindStern, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DOCK, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_NeedVeteran::
	applymovement LOCALID_DOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SlateportCity_SternsShipyard_1F_Text_CouldUseAdviceFromVeteran, MSGBOX_DEFAULT
	releaseall
	end

SlateportCity_SternsShipyard_1F_EventScript_Scientist1::
	msgbox SlateportCity_SternsShipyard_1F_Text_SeaIsLikeLivingThing, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_EventScript_Scientist2::
	msgbox SlateportCity_SternsShipyard_1F_Text_GetSeasickEasily, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_EventScript_Briney::
	msgbox SlateportCity_SternsShipyard_1F_Text_DecidedToHelpDock, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_1F_Text_CantMakeHeadsOrTails:
	.string "Mmh… Se questo va qui, e quello\n"
	.string "va laggiù…\p"
	.string "Allora dove va questo coso?\n"
	.string "E quell'aggeggio, invece?\p"
	.string "Aaargh! Non ci capisco più\n"
	.string "un accidente!$"

SlateportCity_SternsShipyard_1F_Text_MeetDockDeliverToStern:
	.string "Mh?\n"
	.string "Ciao, io sono Dock.\p"
	.string "Capitan Stern mi ha incaricato di\n"
	.string "progettare un traghetto.\p"
	.string "Oh! Quella roba lì…\n"
	.string "È Merce Devon?\p"
	.string "Però, mmh…\n"
	.string "Non è roba per me…\p"
	.string "Quei pezzi servono al sottomarino\n"
	.string "del Capitano, non al mio traghetto.\p"
	.string "E Capitan Stern è andato al Museo\n"
	.string "Oceanografico, qui in città.\l"
	.string "Diceva di avere del lavoro da fare.\p"
	.string "Potresti andare a cercarlo e\n"
	.string "consegnargliela di persona?$"

SlateportCity_SternsShipyard_1F_Text_CouldYouFindStern:
	.string "Dock: Capitan Stern sarà ancora\n"
	.string "al Museo Oceanografico?\p"
	.string "Potresti andare a cercarlo e\n"
	.string "portargli quel pacco?$"

SlateportCity_SternsShipyard_1F_Text_CouldUseAdviceFromVeteran:
	.string "Dock: Costruire navi è un'arte.\p"
	.string "Tante cose non si risolvono\n"
	.string "solo facendo calcoli.\p"
	.string "Mi servirebbero i consigli di un\n"
	.string "veterano che conosca il mare…$"

SlateportCity_SternsShipyard_1F_Text_BrineyJoinedUs:
	.string "Dock: Ciao! Capitan Scoglio si è unito\n"
	.string "a noi per darci una mano.\p"
	.string "Grazie al vecchio lupo di mare, il\n"
	.string "traghetto prende forma ogni giorno.$"

SlateportCity_SternsShipyard_1F_Text_FerryIsReady:
	.string "Dock: Il traghetto è finalmente pronto!\p"
	.string "La nuova M/N Acqua è una vera\n"
	.string "meraviglia della tecnologia!\p"
	.string "Eppure ho la sensazione che si possa\n"
	.string "fare qualcosa di ancora migliore.\p"
	.string "Una nave capace di solcare persino\n"
	.string "un Varco, magari… Chissà!\p"
	.string "Del resto, la marcia della tecnologia\n"
	.string "non si ferma mai.$"

SlateportCity_SternsShipyard_1F_Text_DecidedToHelpDock:
	.string "Capitan Scoglio: Ah, {PLAYER}{KUN}!\n"
	.string "Quanto tempo!\p"
	.string "Sai, da quando ti ho conosciuto,\n"
	.string "questo vecchio lupo di mare si sente\l"
	.string "di nuovo un giovanotto!\p"
	.string "Così ho deciso di aiutare Dock\n"
	.string "a costruire un traghetto.\p"
	.string "Eh, sì, un traghetto può portare\n"
	.string "tanta, tanta gente.\p"
	.string "Ma sai, quel Dock è davvero\n"
	.string "un tipo speciale.\p"
	.string "Col suo talento per la tecnologia e\n"
	.string "la mia esperienza, costruiremo una\l"
	.string "nave coi fiocchi, poco ma sicuro!$"

SlateportCity_SternsShipyard_1F_Text_SeaIsLikeLivingThing:
	.string "Le stagioni, il tempo, la posizione\n"
	.string "della luna nel cielo…\p"
	.string "Queste e altre condizioni cambiano\n"
	.string "l'espressione del mare.\p"
	.string "E ultimamente ci si mettono\n"
	.string "pure i Varchi!\p"
	.string "Proprio così!\n"
	.string "Il mare è come un essere vivente!$"

SlateportCity_SternsShipyard_1F_Text_GetSeasickEasily:
	.string "Io soffro tantissimo il mal di mare.\n"
	.string "Così do una mano qui, all'asciutto.$"

