Route114_LanettesHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route114_LanettesHouse_OnTransition
	.byte 0

Route114_LanettesHouse_OnTransition:
	setflag FLAG_LANDMARK_LANETTES_HOUSE
	end

Route114_LanettesHouse_EventScript_Lanette::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_DOLL_LANETTE, Route114_LanettesHouse_EventScript_OfferAdvice
	setflag FLAG_SYS_PC_LANETTE
	msgbox Route114_LanettesHouse_Text_EverythingClutteredKeepThis, MSGBOX_DEFAULT
	givedecoration DECOR_LOTAD_DOLL
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowNoRoomForDecor
	setflag FLAG_RECEIVED_DOLL_LANETTE
	release
	end

Route114_LanettesHouse_EventScript_OfferAdvice::
	msgbox Route114_LanettesHouse_Text_OrganizeYourBoxes, MSGBOX_DEFAULT
	release
	end

Route114_LanettesHouse_EventScript_Notebook::
	lockall
	msgbox Route114_LanettesHouse_Text_ResearchNotesPage1, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route114_LanettesHouse_EventScript_NotebookPage2
	msgbox Route114_LanettesHouse_Text_ClosedTheNotebook, MSGBOX_DEFAULT
	releaseall
	end

Route114_LanettesHouse_EventScript_NotebookPage2::
	msgbox Route114_LanettesHouse_Text_ResearchNotesPage2, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, Route114_LanettesHouse_EventScript_NotebookPage3
	releaseall
	end

Route114_LanettesHouse_EventScript_NotebookPage3::
	msgbox Route114_LanettesHouse_Text_ResearchNotesPage3, MSGBOX_DEFAULT
	return

Route114_LanettesHouse_EventScript_PC::
	msgbox Route114_LanettesHouse_Text_EmailFromBill, MSGBOX_SIGN
	end

Route114_LanettesHouse_Text_EverythingClutteredKeepThis:
	.string "Lanette: Oh! {PLAYER}{KUN}!\p"
	.string "Scusa per tutto questo disordine…\n"
	.string "Quando mi immergo nella ricerca,\l"
	.string "finisce sempre così…\p"
	.string "Che figuraccia… Tieni questo, ma in\n"
	.string "cambio acqua in bocca, d'accordo?$"

Route114_LanettesHouse_Text_OrganizeYourBoxes:
	.string "Posso darti un consiglio sul mio\n"
	.string "Sistema Memoria Pokémon?\p"
	.string "Organizza bene i tuoi Box, così saprai\n"
	.string "sempre quali Pokémon ci sono dentro.$"

Route114_LanettesHouse_Text_ResearchNotesPage1:
	.string "Sono gli appunti di ricerca di Lanette.\n"
	.string "Parlano dei Box.\p"
	.string "Box da 30 Pokémon ciascuno.\p"
	.string "Ogni Allenatore deve poter depositare\n"
	.string "420 Pokémon nel sistema del PC.\p"
	.string "Vuoi continuare a leggere?$"

Route114_LanettesHouse_Text_ResearchNotesPage2:
	.string "Aggiungere un sistema di contrassegni\n"
	.string "per ordinare i Pokémon più facilmente.\p"
	.string "Il nome e lo sfondo di ogni Box\n"
	.string "saranno modificabili, per la gioia\l"
	.string "dei Pokémon depositati.\p"
	.string "Vuoi continuare a leggere?$"

Route114_LanettesHouse_Text_ResearchNotesPage3:
	.string "Un Pokémon depositato va inviato\n"
	.string "all'ultimo Box esaminato.\p"
	.string "Se quel Box è pieno, il Pokémon\n"
	.string "ricevuto va nel Box successivo.\p"
	.string "In altre parole: quando si esamina\n"
	.string "un Box, questo diventa in automatico\l"
	.string "il Box dove vengono inviati i Pokémon.$"

Route114_LanettesHouse_Text_ClosedTheNotebook:
	.string "{PLAYER} chiude il quaderno.$"

Route114_LanettesHouse_Text_EmailFromBill:
	.string "Sul PC c'è un'e-mail arrivata da\n"
	.string "un indirizzo mai visto prima…\p"
	.string "“… … … … … … …\p"
	.string "“Il tuo Sistema Memoria è molto\n"
	.string "più comodo del mio.\p"
	.string "“Ha tante funzioni semplici da usare\n"
	.string "che lo rendono utile e divertente.\p"
	.string "“Sono fiero di aver dato una mano\n"
	.string "a progettarlo, anche da così lontano.\p"
	.string "“Spero che continuerai le tue\n"
	.string "ricerche sui Sistemi Memoria.\p"
	.string "“Da Bill\n"
	.string "… … … … … … … …”\p"
	.string "In fondo c'è scritto: “Rete di origine:\n"
	.string "Kanto”. Kanto? E dove sarebbe?$"

