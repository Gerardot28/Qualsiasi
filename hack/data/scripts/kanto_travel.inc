@ KANTO_PORT: travel link Hoenn <-> Kanto (generated once by tools/kanto/port_kanto.py, then edited by hand).
@ Hook: one line in LilycoveCity_Harbor_EventScript_FerryAttendant (data/maps/LilycoveCity_Harbor/scripts.inc):
@     call_if_set FLAG_IS_CHAMPION, KantoTravel_EventScript_LilycoveOffer
@ Return: the ferry sailor of Vermilion City (VermilionCity_Frlg_EventScript_FerrySailor) jumps here.

@ Lilycove harbor, after the attendant faced the player. NO -> back to the normal ferry menu.
KantoTravel_EventScript_LilycoveOffer::
	msgbox KantoTravel_Text_SailToKanto, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Common_EventScript_NopReturn
	call KantoPort_EventScript_InitFlags
	call KantoStory_EventScript_Init @ KANTO_V2
	call LilycoveCity_Harbor_EventScript_BoardFerry
	warp MAP_VERMILION_CITY, 24, 32
	waitstate
	release
	end

@ Vermilion City harbor (Aranciopoli): the sailor takes the player back to Lilycove (Porto Alghepoli).
KantoTravel_EventScript_VermilionSailor::
	lock
	faceplayer
	msgbox KantoTravel_Text_SailToHoenn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, KantoTravel_EventScript_VermilionSailorNo
	msgbox KantoTravel_Text_AllAboard, MSGBOX_DEFAULT
	closemessage
	playse SE_SHIP
	warp MAP_LILYCOVE_CITY_HARBOR, 8, 11
	waitstate
	release
	end

KantoTravel_EventScript_VermilionSailorNo::
	msgbox KantoTravel_Text_SeeYou, MSGBOX_DEFAULT
	release
	end

KantoTravel_Text_SailToKanto:
	.string "Da quando sei Campione, la M/N Acqua\n"
	.string "fa scalo anche ad Aranciopoli, a Kanto.\p"
	.string "Vuoi salpare per Kanto?$"

KantoTravel_Text_SailToHoenn:
	.string "La M/N Acqua torna a Porto Alghepoli,\n"
	.string "a Hoenn. Vuoi salire a bordo?$"

KantoTravel_Text_AllAboard:
	.string "Si salpa! Tutti a bordo!$"

KantoTravel_Text_SeeYou:
	.string "Quando vuoi tornare a Hoenn,\n"
	.string "vieni pure da me.$"
