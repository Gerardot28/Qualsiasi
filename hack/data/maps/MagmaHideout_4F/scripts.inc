MagmaHideout_4F_MapScripts::
	.byte 0

MagmaHideout_4F_EventScript_Maxie::
	lockall
	playbgm MUS_ENCOUNTER_MAGMA, FALSE
	msgbox MagmaHideout_4F_Text_MaxieAwakenGroudon, MSGBOX_DEFAULT
	closemessage
	delay 20
	setvar VAR_RESULT, 1
	playse SE_M_DETECT
	dofieldeffectsparkle 18, 42, 0
	waitfieldeffect FLDEFF_SPARKLE
	setvar VAR_RESULT, 1
	playfanfare MUS_AWAKEN_LEGEND
	playse SE_ORB
	special DoOrbEffect
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 150
	removeobject LOCALID_MAGMA_HIDEOUT_4F_GROUDON_SLEEPING
	addobject LOCALID_MAGMA_HIDEOUT_4F_GROUDON
	waitstate
	delay 60
	applymovement LOCALID_MAGMA_HIDEOUT_4F_GROUDON, MagmaHideout_4F_Movement_GroudonApproach
	waitmovement 0
	special FadeOutOrbEffect
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	applymovement LOCALID_MAGMA_HIDEOUT_4F_GROUDON, MagmaHideout_4F_Movement_GroudonExit
	waitmovement 0
	removeobject LOCALID_MAGMA_HIDEOUT_4F_GROUDON
	delay 4
	setvar VAR_0x8004, 2  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	delay 30
	applymovement LOCALID_MAGMA_HIDEOUT_4F_MAXIE, MagmaHideout_4F_Movement_MaxieLookAround
	waitmovement 0
	msgbox MagmaHideout_4F_Text_MaxieGroudonWhatsWrong, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 30
	applymovement LOCALID_MAGMA_HIDEOUT_4F_MAXIE, Common_Movement_FacePlayer
	waitmovement 0
	msgbox MagmaHideout_4F_Text_MaxieOhItWasYou, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_MAXIE_MAGMA_HIDEOUT, MagmaHideout_4F_Text_MaxieDefeat
	msgbox MagmaHideout_4F_Text_MaxieImGoingAfterGroudon, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_HIDE_SLATEPORT_CITY_CAPTAIN_STERN
	clearflag FLAG_HIDE_SLATEPORT_CITY_GABBY_AND_TY
	setvar VAR_SLATEPORT_CITY_STATE, 1
	setflag FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT
	setvar VAR_SLATEPORT_HARBOR_STATE, 1
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_MAGMA_HIDEOUT_4F_MAXIE
	removeobject LOCALID_MAGMA_HIDEOUT_4F_GRUNT_1
	removeobject LOCALID_MAGMA_HIDEOUT_4F_GRUNT_2
	removeobject LOCALID_MAGMA_HIDEOUT_4F_GRUNT_3
	removeobject LOCALID_MAGMA_HIDEOUT_4F_TABITHA
	setflag FLAG_HIDE_MAGMA_HIDEOUT_GRUNTS
	fadescreen FADE_FROM_BLACK
	releaseall
	end

MagmaHideout_4F_Movement_GroudonApproach:
	delay_16
	delay_16
	walk_slow_down
	delay_16
	delay_16
	delay_16
	walk_slow_down
	delay_16
	delay_16
	delay_16
	step_end

MagmaHideout_4F_Movement_GroudonExit:
	slide_up
	slide_up
	step_end

MagmaHideout_4F_Movement_MaxieLookAround:
	face_left
	delay_16
	face_right
	delay_16
	face_left
	delay_16
	face_right
	delay_16
	face_up
	delay_16
	delay_16
	step_end

MagmaHideout_4F_EventScript_Grunt11::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_11, MagmaHideout_4F_Text_Grunt11Intro, MagmaHideout_4F_Text_Grunt11Defeat
	msgbox MagmaHideout_4F_Text_Grunt11PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_EventScript_Grunt12::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_12, MagmaHideout_4F_Text_Grunt12Intro, MagmaHideout_4F_Text_Grunt12Defeat
	msgbox MagmaHideout_4F_Text_Grunt12PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_EventScript_Grunt13::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_13, MagmaHideout_4F_Text_Grunt13Intro, MagmaHideout_4F_Text_Grunt13Defeat
	msgbox MagmaHideout_4F_Text_Grunt13PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_EventScript_Tabitha::
	trainerbattle_single TRAINER_TABITHA_MAGMA_HIDEOUT, MagmaHideout_4F_Text_TabithaIntro, MagmaHideout_4F_Text_TabithaDefeat
	msgbox MagmaHideout_4F_Text_TabithaPostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_4F_Text_Grunt11Intro:
	.string "Anch'io voglio vedere Groudon, ma non\n"
	.string "mi fanno vedere neanche la coda…\p"
	.string "Che frustrazione, te lo giuro!\p"
	.string "Oh, no!\n"
	.string "Mi è scappato il nome di Groudon!$"

MagmaHideout_4F_Text_Grunt11Defeat:
	.string "Senza una mente calma, vincere\n"
	.string "è impossibile…$"

MagmaHideout_4F_Text_Grunt11PostBattle:
	.string "Chissà se Groudon ce l'ha davvero,\n"
	.string "una coda…$"

MagmaHideout_4F_Text_Grunt12Intro:
	.string "Fuhahaha!\n"
	.string "Presto! Prestissimo!\l"
	.string "Il nostro grande obiettivo sarà\l"
	.string "raggiunto! Per l'Alpha!$"

MagmaHideout_4F_Text_Grunt12Defeat:
	.string "Grrr…\n"
	.string "Arrivato fin qui, e ora questo?$"

MagmaHideout_4F_Text_Grunt12PostBattle:
	.string "Capo Ettore, signore!\n"
	.string "Intruso in avvicinamento!$"

MagmaHideout_4F_Text_Grunt13Intro:
	.string "Non hai ancora finito!\n"
	.string "Da qui non passi così facilmente!$"

MagmaHideout_4F_Text_Grunt13Defeat:
	.string "Sono stato così facile da battere?$"

MagmaHideout_4F_Text_Grunt13PostBattle:
	.string "D-dai, ancora una lotta…$"

MagmaHideout_4F_Text_TabithaIntro:
	.string "Hehehe!\n"
	.string "Sei arrivato fin qui? Allora te lo dico!\p"
	.string "Proprio così!\n"
	.string "Più avanti dorme Groudon, l'Ancora\l"
	.string "della Terra!\p"
	.string "Il Capo Ettore è andato da lui\n"
	.string "pochi secondi fa!\p"
	.string "Si sveglierà a momenti!\n"
	.string "Hehe! Hehehe! Per l'Alpha!$"

MagmaHideout_4F_Text_TabithaDefeat:
	.string "Battuto di nuovo…\n"
	.string "Hehe…$"

MagmaHideout_4F_Text_TabithaPostBattle:
	.string "…E mentre perdevi tempo con me, il\n"
	.string "Capo avrà già risvegliato Groudon…$"

MagmaHideout_4F_Text_MaxieAwakenGroudon:
	.string "Ettore: Fufufu… Sei arrivato fin qui,\n"
	.string "ragazzino? Allora guarda.\p"
	.string "L'Ancora della Terra. Groudon.\n"
	.string "Dorme nel magma da tremila anni.\p"
	.string "Con la Gemma Blu la sveglierò, e lei\n"
	.string "affonderà nella Soglia…\p"
	.string "…finché nessun Varco potrà più\n"
	.string "aprirsi. Mai più.\p"
	.string "Groudon! Ecco la Gemma Blu che\n"
	.string "cercavi! Che la sua luce ti svegli!\p"
	.string "E mostrami…\n"
	.string "Mostrami tutta la tua potenza!$"

MagmaHideout_4F_Text_MaxieGroudonWhatsWrong:
	.string "Ettore: Groudon!\n"
	.string "Che cosa succede?\p"
	.string "La Gemma Blu non era la chiave?\n"
	.string "Doveva… obbedire…\p"
	.string "Groudon!\n"
	.string "Dove stai andando…$"

MagmaHideout_4F_Text_MaxieOhItWasYou:
	.string "Ettore: Tu.\p"
	.string "Il ragazzino che ficca il naso\n"
	.string "ovunque, senza essere invitato…\p"
	.string "La probabilità che sia un caso è\n"
	.string "pari a zero. Che trucco hai usato?!$"

MagmaHideout_4F_Text_MaxieDefeat:
	.string "Come fai a capire i Pokémon\n"
	.string "così bene?$"

MagmaHideout_4F_Text_MaxieImGoingAfterGroudon:
	.string "Ettore: Groudon non è fuggito per\n"
	.string "caso. È questo che vuoi dirmi, vero?\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Hmpf…\n"
	.string "Credi che non lo sappia?\p"
	.string "Senza Groudon, questo maledetto\n"
	.string "vulcano non mi serve più.\p"
	.string "Devo ritrovare l'Ancora prima che\n"
	.string "sia troppo tardi.\p"
	.string "Fufufu…\n"
	.string "No. Non c'è niente da ridere.\p"
	.string "Le nostre strade si separano qui.$"
