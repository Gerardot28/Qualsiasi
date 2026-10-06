SeafloorCavern_Room9_MapScripts::
	.byte 0

SeafloorCavern_Room9_EventScript_ArchieAwakenKyogre::
	lockall
	setvar VAR_0x8004, LOCALID_SEAFLOOR_CAVERN_ARCHIE
	setvar VAR_0x8005, LOCALID_SEAFLOOR_CAVERN_MAXIE
	setvar VAR_0x8006, LOCALID_SEAFLOOR_CAVERN_GRUNT_1
	setvar VAR_0x8007, LOCALID_SEAFLOOR_CAVERN_GRUNT_2
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	applymovement LOCALID_PLAYER, SeafloorCavern_Room9_Movement_Delay32
	waitmovement 0
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	msgbox SeafloorCavern_Room9_Text_ArchieHoldItRightThere, MSGBOX_DEFAULT
	closemessage
	addobject VAR_0x8004
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement VAR_0x8004, SeafloorCavern_Room9_Movement_ArchieApproachPlayer
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieSoItWasYou, MSGBOX_DEFAULT
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieBeholdKyogre, MSGBOX_DEFAULT
	applymovement VAR_0x8004, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieYouMustDisappear, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_ARCHIE, SeafloorCavern_Room9_Text_ArchieDefeat
	msgbox SeafloorCavern_Room9_Text_ArchieWithThisRedOrb, MSGBOX_DEFAULT
	setweather WEATHER_NONE
	doweather
	special Script_FadeOutMapMusic
	msgbox SeafloorCavern_Room9_Text_RedOrbShinesByItself, MSGBOX_DEFAULT
	special WaitWeather
	setvar VAR_RESULT, 1
	playse SE_M_DETECT
	dofieldeffectsparkle 16, 42, 0
	waitfieldeffect FLDEFF_SPARKLE
	closemessage
	setvar VAR_RESULT, 0
	playfanfare MUS_AWAKEN_LEGEND
	playse SE_ORB
	special DoOrbEffect
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 150
	removeobject LOCALID_SEAFLOOR_CAVERN_KYOGRE_SLEEPING
	addobject LOCALID_SEAFLOOR_CAVERN_KYOGRE
	waitstate
	delay 60
	applymovement LOCALID_SEAFLOOR_CAVERN_KYOGRE, SeafloorCavern_Room9_Movement_KyogreApproach
	waitmovement 0
	special FadeOutOrbEffect
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	applymovement LOCALID_SEAFLOOR_CAVERN_KYOGRE, SeafloorCavern_Room9_Movement_KyogreExit
	waitmovement 0
	removeobject LOCALID_SEAFLOOR_CAVERN_KYOGRE
	delay 4
	setvar VAR_0x8004, 2  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	delay 30
	setvar VAR_0x8004, LOCALID_SEAFLOOR_CAVERN_ARCHIE
	setvar VAR_0x8005, LOCALID_SEAFLOOR_CAVERN_MAXIE
	setvar VAR_0x8006, LOCALID_SEAFLOOR_CAVERN_GRUNT_1
	setvar VAR_0x8007, LOCALID_SEAFLOOR_CAVERN_GRUNT_2
	msgbox SeafloorCavern_Room9_Text_ArchieWhereDidKyogreGo, MSGBOX_DEFAULT
	playse SE_PC_LOGIN
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieAMessageFromOutside, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_0x8004, SeafloorCavern_Room9_Movement_ArchieListenMessage
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieWhatRainingTooHard, MSGBOX_DEFAULT
	closemessage
	playse SE_PC_OFF
	delay 20
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieWhyDidKyogreDisappear, MSGBOX_DEFAULT
	closemessage
	addobject VAR_0x8005
	addobject VAR_0x8006
	addobject VAR_0x8007
	applymovement VAR_0x8007, SeafloorCavern_Room9_Movement_MagmaGruntArrive
	applymovement VAR_0x8006, SeafloorCavern_Room9_Movement_MagmaGruntArrive
	applymovement VAR_0x8005, SeafloorCavern_Room9_Movement_MaxieArrive
	waitmovement 0
	applymovement VAR_0x8004, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_MaxieWhatHaveYouWrought, MSGBOX_DEFAULT
	playse SE_PIN
	applymovement VAR_0x8004, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_0x8004, Common_Movement_Delay48
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_ArchieDontGetAllHighAndMighty, MSGBOX_DEFAULT
	msgbox SeafloorCavern_Room9_Text_MaxieWeDontHaveTimeToArgue, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_0x8005, SeafloorCavern_Room9_Movement_MaxieExit
	applymovement VAR_0x8004, SeafloorCavern_Room9_Movement_ArchieExit
	waitmovement 0
	msgbox SeafloorCavern_Room9_Text_MaxieComeOnPlayer, MSGBOX_DEFAULT
	setvar VAR_ROUTE128_STATE, 1
	setvar VAR_SOOTOPOLIS_CITY_STATE, 1
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_STEVEN
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_ARCHIE
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_MAXIE
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_RESIDENTS
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_GROUDON
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_KYOGRE
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_MAN_1
	setflag FLAG_LEGENDARIES_IN_SOOTOPOLIS
	clearflag FLAG_HIDE_ROUTE_128_ARCHIE
	clearflag FLAG_HIDE_ROUTE_128_MAXIE
	setflag FLAG_SYS_WEATHER_CTRL
	setflag FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN
	setflag FLAG_HIDE_MOSSDEEP_CITY_STEVENS_HOUSE_STEVEN
	setvar VAR_STEVENS_HOUSE_STATE, 2
	setvar VAR_SEAFLOOR_CAVERN_STATE, 1
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_ARCHIE
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_MAXIE
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_MAGMA_GRUNTS
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_KYOGRE
	setflag FLAG_HIDE_SEAFLOOR_CAVERN_AQUA_GRUNTS
	setflag FLAG_HIDE_MAP_NAME_POPUP
	warp MAP_ROUTE128, 38, 22
	waitstate
	releaseall
	end

SeafloorCavern_Room9_Movement_ArchieApproachPlayer:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SeafloorCavern_Room9_Movement_Unused1:
	walk_left
	walk_left
	step_end

SeafloorCavern_Room9_Movement_ArchieListenMessage:
	walk_left
	delay_16
	step_end

SeafloorCavern_Room9_Movement_Unused2:
	walk_right
	step_end

SeafloorCavern_Room9_Movement_ArchieExit:
	face_up
	lock_facing_direction
	walk_down
	unlock_facing_direction
	walk_in_place_faster_right
	step_end

SeafloorCavern_Room9_Movement_KyogreApproach:
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

SeafloorCavern_Room9_Movement_KyogreExit:
	slide_up
	slide_up
	step_end

SeafloorCavern_Room9_Movement_MaxieArrive:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

SeafloorCavern_Room9_Movement_MaxieExit:
	walk_right
	walk_right
	step_end

SeafloorCavern_Room9_Movement_MagmaGruntArrive:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

SeafloorCavern_Room9_Movement_Delay32:
	delay_16
	delay_16
	step_end

SeafloorCavern_Room9_Text_ArchieHoldItRightThere:
	.string "Ulisse: Alt! Fermo dove sei.$"

SeafloorCavern_Room9_Text_ArchieSoItWasYou:
	.string "Ulisse: Ahrrr-ha-ha…\n"
	.string "Allora eri proprio tu.$"

SeafloorCavern_Room9_Text_ArchieBeholdKyogre:
	.string "Ulisse: Guarda!\p"
	.string "Guarda com'è bello, mentre dorme:\n"
	.string "Kyogre, l'Ancora del Mare!\p"
	.string "Ho aspettato tanto, tanto a lungo\n"
	.string "questo giorno…$"

SeafloorCavern_Room9_Text_ArchieYouMustDisappear:
	.string "Ulisse: Sei arrivato fino al fondo del\n"
	.string "mare, mozzo. Questo te lo concedo.\p"
	.string "Ma la tua rotta finisce qui.\p"
	.string "Il mondo senza confini comincia\n"
	.string "proprio da questa grotta!\p"
	.string "E per realizzare il mio sogno,\n"
	.string "tu devi sparire! Ahrrr!$"

SeafloorCavern_Room9_Text_ArchieDefeat:
	.string "Cosa?!\n"
	.string "Battuto da un mozzo come te?!$"

SeafloorCavern_Room9_Text_ArchieWithThisRedOrb:
	.string "Ulisse: Ahrrr-ha-ha…\p"
	.string "Ti faccio i miei complimenti. Devo\n"
	.string "ammetterlo: hai un vero talento.\p"
	.string "Ma!\n"
	.string "Io ho questa!\p"
	.string "Con la Gemma Rossa, Kyogre farà\n"
	.string "tutto ciò che…$"

SeafloorCavern_Room9_Text_RedOrbShinesByItself:
	.string "La Gemma Rossa ha cominciato\n"
	.string "a brillare da sola!$"

SeafloorCavern_Room9_Text_ArchieWhereDidKyogreGo:
	.string "Ulisse: Cosa?!\p"
	.string "Io non ho fatto niente.\n"
	.string "Perché la Gemma Rossa…\p"
	.string "Dov'è finito Kyogre?$"

SeafloorCavern_Room9_Text_ArchieAMessageFromOutside:
	.string "Ulisse: Hm? Un messaggio dalla ciurma\n"
	.string "in superficie…$"

SeafloorCavern_Room9_Text_ArchieWhatRainingTooHard:
	.string "Ulisse: Sì, che c'è?\p"
	.string "Hm…\n"
	.string "Piove a dirotto?\p"
	.string "Bene… Era previsto.\n"
	.string "Abbiamo svegliato Kyogre per questo:\l"
	.string "per levare l'Ancora e far salpare\l"
	.string "Hoenn oltre ogni mare!\p"
	.string "Cosa?!\p"
	.string "Piove molto più forte del previsto?\n"
	.string "Siete in pericolo?\p"
	.string "Non può essere…\n"
	.string "Non è possibile…\p"
	.string "Restate in posizione e tenete\n"
	.string "d'occhio la situazione!$"

SeafloorCavern_Room9_Text_ArchieWhyDidKyogreDisappear:
	.string "Ulisse: C'è qualcosa che non va…\p"
	.string "La Gemma Rossa doveva svegliare\n"
	.string "Kyogre e renderlo docile…\p"
	.string "Ma… perché?\n"
	.string "Perché Kyogre è sparito?\p"
	.string "Perché?!$"

SeafloorCavern_Room9_Text_MaxieWhatHaveYouWrought:
	.string "Ettore: Pazzo! Che cosa hai fatto?\p"
	.string "Ulisse… Alla fine hai svegliato\n"
	.string "Kyogre, non è vero?\p"
	.string "Prima il sole, ora il diluvio…\n"
	.string "Le Ancore si cercano!\p"
	.string "Se questa pioggia durerà in eterno,\n"
	.string "che ne sarà del mondo?\p"
	.string "Ogni terra emersa annegherà nel\n"
	.string "mare che sale…$"

SeafloorCavern_Room9_Text_ArchieDontGetAllHighAndMighty:
	.string "Ulisse: C-cosa?!\n"
	.string "Non fare il superiore con me!\p"
	.string "Non siete stati voi del Team Alpha\n"
	.string "a far infuriare Groudon?\p"
	.string "Finché ho la Gemma Rossa, dovrei\n"
	.string "riuscire a controllare Kyogre…\p"
	.string "Dovrei riuscirci…$"

SeafloorCavern_Room9_Text_MaxieWeDontHaveTimeToArgue:
	.string "Ettore: Non abbiamo tempo per\n"
	.string "litigare qui sotto!\p"
	.string "Esci e guarda con i tuoi occhi!\p"
	.string "Guarda se ciò che hai scatenato è\n"
	.string "il mondo che sognavamo, un tempo!$"

SeafloorCavern_Room9_Text_MaxieComeOnPlayer:
	.string "Ettore: {PLAYER}, presto, devi\n"
	.string "uscire da qui anche tu!$"
