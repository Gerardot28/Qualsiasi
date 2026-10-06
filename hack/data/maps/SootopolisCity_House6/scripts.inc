SootopolisCity_House6_MapScripts::
	.byte 0

SootopolisCity_House6_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_WAILMER_DOLL, SootopolisCity_House6_EventScript_ReceivedWailmerDoll
	msgbox SootopolisCity_House6_Text_FirstGuestInWhileTakeDoll, MSGBOX_YESNO
	call_if_eq VAR_RESULT, NO, SootopolisCity_House6_EventScript_DeclineWailmerDoll
	msgbox SootopolisCity_House6_Text_TakeGoodCareOfIt, MSGBOX_DEFAULT
	givedecoration DECOR_WAILMER_DOLL
	goto_if_eq VAR_RESULT, FALSE, SootopolisCity_House6_EventScript_NoRoomForWailmerDoll
	setflag FLAG_RECEIVED_WAILMER_DOLL
	release
	end

SootopolisCity_House6_EventScript_DeclineWailmerDoll::
	msgbox SootopolisCity_House6_Text_DontWantThisDoll, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House6_EventScript_ReceivedWailmerDoll::
	msgbox SootopolisCity_House6_Text_LovePlushDolls, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House6_EventScript_NoRoomForWailmerDoll::
	bufferdecorationname STR_VAR_2, DECOR_WAILMER_DOLL
	msgbox gText_NoRoomLeftForAnother, MSGBOX_DEFAULT
	msgbox SootopolisCity_House6_Text_IllHoldItForYou, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House6_Text_FirstGuestInWhileTakeDoll:
	.string "Ciao! Sei il primo ospite che\n"
	.string "riceviamo da un bel pezzo.\p"
	.string "Mi hai rallegrato la giornata, quindi\n"
	.string "ti regalo una grossa Bambola Wailmer.\l"
	.string "La vuoi?$"

SootopolisCity_House6_Text_TakeGoodCareOfIt:
	.string "Trattala bene, mi raccomando!$"

SootopolisCity_House6_Text_IllHoldItForYou:
	.string "Oh, la vuoi, ma non adesso?\n"
	.string "Va bene, allora te la tengo da parte.$"

SootopolisCity_House6_Text_DontWantThisDoll:
	.string "Sei sicuro?\n"
	.string "Non vuoi questa Bambola?$"

SootopolisCity_House6_Text_LovePlushDolls:
	.string "Adoro le bambole di peluche!$"

