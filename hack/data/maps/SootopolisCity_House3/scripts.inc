SootopolisCity_House3_MapScripts::
	.byte 0

SootopolisCity_House3_EventScript_Woman::
	lock
	faceplayer
	msgbox SootopolisCity_House3_Text_JuanHasManyFansDoYou, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SootopolisCity_House3_EventScript_HaveFans
	msgbox SootopolisCity_House3_Text_LonesomeTryWorkingHarder, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House3_EventScript_HaveFans::
	msgbox SootopolisCity_House3_Text_YouMustBePrettyStrong, MSGBOX_DEFAULT
	release
	end

SootopolisCity_House3_EventScript_Girl::
	msgbox SootopolisCity_House3_Text_TrainerFanClubWasWild, MSGBOX_NPC
	end

SootopolisCity_House3_Text_JuanHasManyFansDoYou:
	.string "Sei un Allenatore di Pokémon, vero?\p"
	.string "Rodolfo di Ceneride ha tantissimi fan.\n"
	.string "Più perfino del suo allievo Adriano!\p"
	.string "E tu, ne hai qualcuno?$"

SootopolisCity_House3_Text_YouMustBePrettyStrong:
	.string "Oh, allora devi essere proprio forte.$"

SootopolisCity_House3_Text_LonesomeTryWorkingHarder:
	.string "Oh, povero…\n"
	.string "È un po' tristino, sai?\p"
	.string "Impegnati un pochino di più, e vedrai\n"
	.string "che i fan arriveranno.$"

SootopolisCity_House3_Text_TrainerFanClubWasWild:
	.string "I fan più sfegatati arrivano da ogni\n"
	.string "angolo di Hoenn. C'è chi giura perfino\l"
	.string "da oltre i Varchi!\p"
	.string "Quando sono andata al Fan Club\n"
	.string "Allenatori di Porto Alghepoli,\l"
	.string "è stato un vero delirio!$"

