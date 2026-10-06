PacifidlogTown_House5_MapScripts::
	.byte 0

PacifidlogTown_House5_EventScript_MirageIslandWatcher::
	lock
	faceplayer
	specialvar VAR_RESULT, IsMirageIslandPresent
	goto_if_eq VAR_RESULT, TRUE, PacifidlogTown_House5_EventScript_MirageIslandPresent
	msgbox PacifidlogTown_House5_Text_CantSeeMirageIslandToday, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House5_EventScript_MirageIslandPresent::
	msgbox PacifidlogTown_House5_Text_CanSeeMirageIslandToday, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House5_EventScript_Gentleman::
	msgbox PacifidlogTown_House5_Text_MirageIslandAppearDependingOnWeather, MSGBOX_NPC
	end

PacifidlogTown_House5_Text_CantSeeMirageIslandToday:
	.string "Oggi l'Isola Miraggio non si vede…$"

PacifidlogTown_House5_Text_CanSeeMirageIslandToday:
	.string "Oh! Ma guarda un po'!\n"
	.string "Oggi si vede l'Isola Miraggio!$"

PacifidlogTown_House5_Text_MirageIslandAppearDependingOnWeather:
	.string "L'Isola Miraggio…\p"
	.string "Forse appare e scompare a seconda del\n"
	.string "tempo, come succede con i miraggi.\p"
	.string "O forse appare e scompare davvero,\n"
	.string "come un Varco che non si decide?$"

