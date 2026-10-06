AquaHideout_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, AquaHideout_B2F_OnTransition
	.byte 0

AquaHideout_B2F_OnTransition:
	call_if_set FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, AquaHideout_B2F_EventScript_PreventMattNoticing
	end

AquaHideout_B2F_EventScript_PreventMattNoticing::
	setvar VAR_TEMP_1, 1
	return

AquaHideout_B2F_EventScript_MattNoticePlayer::
	lockall
	setvar VAR_0x8008, LOCALID_AQUA_HIDEOUT_MATT
	playse SE_PIN
	applymovement VAR_0x8008, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_0x8008, Common_Movement_FacePlayer
	waitmovement 0
	setvar VAR_TEMP_1, 1
	releaseall
	end

AquaHideout_B2F_EventScript_Matt::
	trainerbattle_single TRAINER_MATT, AquaHideout_B2F_Text_MattIntro, AquaHideout_B2F_Text_MattDefeat, AquaHideout_B2F_EventScript_SubmarineEscape
	msgbox AquaHideout_B2F_Text_MattPostBattle, MSGBOX_DEFAULT
	release
	end

AquaHideout_B2F_EventScript_SubmarineEscape::
	setvar VAR_0x8008, LOCALID_AQUA_HIDEOUT_MATT
	setvar VAR_0x8009, LOCALID_AQUA_HIDEOUT_SUBMARINE
	applymovement VAR_0x8008, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	delay 20
	applymovement VAR_0x8008, Common_Movement_FacePlayer
	waitmovement 0
	msgbox AquaHideout_B2F_Text_OurBossGotThroughHisPreparations, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_0x8008, Common_Movement_WalkInPlaceFasterLeft
	applymovement VAR_0x8009, AquaHideout_B2F_Movement_SumbarineDepartLeft
	waitmovement 0
	removeobject VAR_0x8009
	delay 20
	applymovement VAR_0x8008, Common_Movement_FacePlayer
	waitmovement 0
	msgbox AquaHideout_B2F_Text_MattPostBattle, MSGBOX_DEFAULT
	setflag FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE
	setflag FLAG_HIDE_LILYCOVE_CITY_AQUA_GRUNTS
	release
	end

AquaHideout_B2F_Movement_SumbarineDepartLeft:
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

@ Unused
AquaHideout_B2F_Movement_SumbarineDepartRight:
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

AquaHideout_B2F_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_4, AquaHideout_B2F_Text_Grunt4Intro, AquaHideout_B2F_Text_Grunt4Defeat, AquaHideout_B2F_EventScript_Grunt4Defeated
	msgbox AquaHideout_B2F_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_B2F_EventScript_Grunt4Defeated::
	msgbox AquaHideout_B2F_Text_Grunt4PostBattle, MSGBOX_DEFAULT
	release
	end

AquaHideout_B2F_EventScript_Grunt6::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_6, AquaHideout_B2F_Text_Grunt6Intro, AquaHideout_B2F_Text_Grunt6Defeat
	msgbox AquaHideout_B2F_Text_Grunt6PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_B2F_EventScript_Grunt8::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_8, AquaHideout_B2F_Text_Grunt8Intro, AquaHideout_B2F_Text_Grunt8Defeat
	msgbox AquaHideout_B2F_Text_Grunt8PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_B2F_Text_MattIntro:
	.string "Ahoy! Ahahah!\p"
	.string "Sei già arrivato fin qui, eh?\n"
	.string "Ti avevamo sottovalutato!\p"
	.string "Ma la tua corsa finisce qui!\p"
	.string "Sono Baldo, Admin del Team Omega.\n"
	.string "Altro che le Reclute di prima!\p"
	.string "E non sto mica prendendo tempo, eh!\n"
	.string "Ti riduco in briciole! Ahahah!$"

AquaHideout_B2F_Text_MattDefeat:
	.string "Ahahah…\n"
	.string "Ho perso anch'io, eh…$"

AquaHideout_B2F_Text_OurBossGotThroughHisPreparations:
	.string "Ahahah!\p"
	.string "Mentre giocavo con te, il Capo ha\n"
	.string "finito i preparativi!$"

AquaHideout_B2F_Text_MattPostBattle:
	.string "Ahahah!\p"
	.string "Il Capo è già salpato verso una grotta\n"
	.string "in fondo al mare!\p"
	.string "Se vuoi inseguirlo, cerca nel mare\n"
	.string "aperto oltre Porto Alghepoli.\p"
	.string "Ma riuscirai a trovarla?\n"
	.string "Ahoy! Ahahah!$"

AquaHideout_B2F_Text_Grunt4Intro:
	.string "Wahahah, mi sono stufato di\n"
	.string "aspettare! Mi devi una lotta!$"

AquaHideout_B2F_Text_Grunt4Defeat:
	.string "Stufo di aspettare…\n"
	.string "E pure suonato…$"

AquaHideout_B2F_Text_Grunt4PostBattle:
	.string "Capo…\n"
	.string "Ho fatto abbastanza?$"

AquaHideout_B2F_Text_Grunt6Intro:
	.string "I pannelli di teletrasporto: il vanto\n"
	.string "del nostro covo!\p"
	.string "Non hai idea di dove ti trovi,\n"
	.string "vero?\p"
	.string "Confondi il nemico, sfiancalo e poi\n"
	.string "colpisci! Ecco il nostro piano! Arrr!$"

AquaHideout_B2F_Text_Grunt6Defeat:
	.string "Ma che hai?\n"
	.string "Non sei per niente stanco!$"

AquaHideout_B2F_Text_Grunt6PostBattle:
	.string "A proposito… Non mi ricordo dove ho\n"
	.string "messo la Master Ball.\p"
	.string "Se non la sorveglio, il Capo mi farà\n"
	.string "una bella lavata di capo…$"

AquaHideout_B2F_Text_Grunt8Intro:
	.string "Quando sono entrato nel Team Omega,\n"
	.string "la prima cosa che ho dovuto imparare\l"
	.string "è come si collegano questi pannelli.$"

AquaHideout_B2F_Text_Grunt8Defeat:
	.string "Ero troppo concentrato sui\n"
	.string "pannelli di teletrasporto…$"

AquaHideout_B2F_Text_Grunt8PostBattle:
	.string "Dovrò imparare a lottare in modo\n"
	.string "più efficace…$"
