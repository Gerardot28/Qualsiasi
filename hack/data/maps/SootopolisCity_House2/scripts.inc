SootopolisCity_House2_MapScripts::
	.byte 0

SootopolisCity_House2_EventScript_ExpertF::
	lock
	faceplayer
	msgbox SootopolisCity_House2_Text_DidYouKnowAboutMtPyreOrbs, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, SootopolisCity_House2_EventScript_KnowAboutOrbs
	call_if_eq VAR_RESULT, NO, SootopolisCity_House2_EventScript_DontKnowAboutOrbs
	release
	end

SootopolisCity_House2_EventScript_KnowAboutOrbs::
	msgbox SootopolisCity_House2_Text_YesTwoOrbsSideBySide, MSGBOX_DEFAULT
	return

SootopolisCity_House2_EventScript_DontKnowAboutOrbs::
	msgbox SootopolisCity_House2_Text_OughtToVisitAndSee, MSGBOX_DEFAULT
	return

SootopolisCity_House2_Text_DidYouKnowAboutMtPyreOrbs:
	.string "Il Monte Pira…\p"
	.string "Sulla sua vetta ci sono due gemme,\n"
	.string "una accanto all'altra. Lo sapevi?$"

SootopolisCity_House2_Text_YesTwoOrbsSideBySide:
	.string "Sì, due gemme fianco a fianco…\p"
	.string "Vederle insieme mette addosso\n"
	.string "una strana pace…\p"
	.string "Dicono che non debbano mai, mai\n"
	.string "essere separate.$"

SootopolisCity_House2_Text_OughtToVisitAndSee:
	.string "Davvero?\n"
	.string "Allora dovresti andare a vederle…$"

