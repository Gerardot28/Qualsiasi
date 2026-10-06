MeteorFalls_StevensCave_MapScripts::
	.byte 0

MeteorFalls_StevensCave_EventScript_Steven::
	lock
	goto_if_set FLAG_DEFEATED_METEOR_FALLS_STEVEN, MeteorFalls_StevensCave_EventScript_Defeated
	waitse
	playse SE_PIN
	applymovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MeteorFalls_StevensCave_Text_ShouldKnowHowGoodIAmExpectWorst, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_STEVEN, MeteorFalls_StevensCave_Text_StevenDefeat
	msgbox MeteorFalls_StevensCave_Text_MyPredictionCameTrue, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_METEOR_FALLS_STEVEN
	release
	end

MeteorFalls_StevensCave_EventScript_Defeated::
	applymovement LOCALID_METEOR_FALLS_STEVEN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MeteorFalls_StevensCave_Text_MyPredictionCameTrue, MSGBOX_DEFAULT
	release
	end

MeteorFalls_StevensCave_Text_ShouldKnowHowGoodIAmExpectWorst:
	.string "Rocco: Oh, {PLAYER}{KUN}!\n"
	.string "Come hai fatto a trovarmi qui?\p"
	.string "Pensi forse che io sia solo…\n"
	.string "un fanatico delle pietre?\p"
	.string "No, non può essere.\p"
	.string "Abbiamo lottato fianco a fianco al\n"
	.string "Centro Spaziale di Verdeazzupoli.\p"
	.string "Dovresti sapere bene di cosa\n"
	.string "sono capace.\p"
	.string "Questo è il punto dove tutto cadde,\n"
	.string "tremila anni fa. Interessante, no?\p"
	.string "Prima di guardare da vicino il Grande\n"
	.string "Varco, volevo mettermi alla prova.\p"
	.string "Allora, {PLAYER}{KUN}, se vuoi sfidarmi\n"
	.string "sul serio, preparati al peggio!$"

MeteorFalls_StevensCave_Text_StevenDefeat:
	.string "Tu… Sei diventato davvero forte.\n"
	.string "Non me l'aspettavo proprio…$"

MeteorFalls_StevensCave_Text_MyPredictionCameTrue:
	.string "Rocco: Sai, fin da quando ci siamo\n"
	.string "incontrati nella Grotta Pietrosa,\l"
	.string "a Bluruvia, avevo un presentimento.\p"
	.string "Pensavo che prima o poi saresti\n"
	.string "diventato Campione.\p"
	.string "Le mie previsioni, di solito,\n"
	.string "si avverano.\p"
	.string "E adesso, dove andrai?\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Eheh… Questo nemmeno io so dirlo.\p"
	.string "Però il Grande Varco è aperto…\n"
	.string "Interessante, non trovi?$"

