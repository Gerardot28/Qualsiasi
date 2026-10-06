EverGrandeCity_ChampionsRoom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EverGrandeCity_ChampionsRoom_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, EverGrandeCity_ChampionsRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, EverGrandeCity_ChampionsRoom_OnFrame
	.byte 0

EverGrandeCity_ChampionsRoom_OnTransition:
	call Common_EventScript_SetupRivalGfxId
	end

EverGrandeCity_ChampionsRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, EverGrandeCity_ChampionsRoom_EventScript_PlayerTurnNorth
	.2byte 0

EverGrandeCity_ChampionsRoom_EventScript_PlayerTurnNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

EverGrandeCity_ChampionsRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, EverGrandeCity_ChampionsRoom_EventScript_EnterRoom
	.2byte 0

EverGrandeCity_ChampionsRoom_EventScript_EnterRoom::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkUp4
	waitmovement 0
	delay 40
	applymovement LOCALID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerApproachWallace
	waitmovement 0
	setvar VAR_TEMP_1, 1
	goto EverGrandeCity_ChampionsRoom_EventScript_Wallace
	releaseall
	end

EverGrandeCity_ChampionsRoom_Movement_PlayerApproachWallace:
	walk_up
	walk_up
	step_end

EverGrandeCity_ChampionsRoom_EventScript_Wallace::
	playbgm MUS_ENCOUNTER_CHAMPION, FALSE
	msgbox EverGrandeCity_ChampionsRoom_Text_IntroSpeech, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLACE, EverGrandeCity_ChampionsRoom_Text_Defeat
	goto EverGrandeCity_ChampionsRoom_EventScript_Defeated
	end

EverGrandeCity_ChampionsRoom_EventScript_Defeated::
	playse SE_DOOR
	setmetatile 6, 1, METATILE_EliteFour_OpenDoorChampion_Frame, FALSE
	setmetatile 6, 2, METATILE_EliteFour_OpenDoorChampion_Opening, FALSE
	special DrawWholeMapView
	msgbox EverGrandeCity_ChampionsRoom_Text_PostBattleSpeech, MSGBOX_DEFAULT
	closemessage
	playse SE_DOOR
	checkplayergender
	call_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_PlayMayMusic
	call_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_PlayBrendanMusic
	addobject LOCALID_CHAMPIONS_ROOM_RIVAL
	call EverGrandeCity_ChampionsRoom_EventScript_RivalApproachPlayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_MayAdvice
	goto_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_BrendanAdvice
	end

EverGrandeCity_ChampionsRoom_EventScript_PlayMayMusic::
	playbgm MUS_ENCOUNTER_MAY, FALSE
	return

EverGrandeCity_ChampionsRoom_EventScript_PlayBrendanMusic::
	playbgm MUS_ENCOUNTER_BRENDAN, FALSE
	return

EverGrandeCity_ChampionsRoom_EventScript_MayAdvice::
	msgbox EverGrandeCity_ChampionsRoom_Text_MayAdvice, MSGBOX_DEFAULT
	delay 40
	playse SE_PIN
	applymovement LOCALID_CHAMPIONS_ROOM_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_CHAMPIONS_ROOM_RIVAL, Common_Movement_Delay48
	waitmovement 0
	call EverGrandeCity_ChampionsRoom_EventScript_RivalLookBackAndForth
	msgbox EverGrandeCity_ChampionsRoom_Text_MayItsAlreadyOver, MSGBOX_DEFAULT
	goto EverGrandeCity_ChampionsRoom_EventScript_BirchArrivesExitForHoF
	end

EverGrandeCity_ChampionsRoom_EventScript_BrendanAdvice::
	msgbox EverGrandeCity_ChampionsRoom_Text_BrendanAdvice, MSGBOX_DEFAULT
	delay 40
	playse SE_PIN
	applymovement LOCALID_CHAMPIONS_ROOM_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_CHAMPIONS_ROOM_RIVAL, Common_Movement_Delay48
	waitmovement 0
	call EverGrandeCity_ChampionsRoom_EventScript_RivalLookBackAndForth
	msgbox EverGrandeCity_ChampionsRoom_Text_BrendanYouveWon, MSGBOX_DEFAULT
	goto EverGrandeCity_ChampionsRoom_EventScript_BirchArrivesExitForHoF
	end

EverGrandeCity_ChampionsRoom_EventScript_BirchArrivesExitForHoF::
	closemessage
	addobject LOCALID_CHAMPIONS_ROOM_BIRCH
	applymovement LOCALID_CHAMPIONS_ROOM_BIRCH, EverGrandeCity_ChampionsRoom_Movement_BirchArrives
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox EverGrandeCity_ChampionsRoom_Text_BirchArriveRatePokedex, MSGBOX_DEFAULT
	call ProfBirch_EventScript_RatePokedex
	msgbox EverGrandeCity_ChampionsRoom_Text_BirchCongratulations, MSGBOX_DEFAULT
	applymovement LOCALID_CHAMPIONS_ROOM_WALLACE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 20
	applymovement LOCALID_CHAMPIONS_ROOM_WALLACE, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox EverGrandeCity_ChampionsRoom_Text_WallaceComeWithMe, MSGBOX_DEFAULT
	closemessage
	delay 30
	applymovement LOCALID_CHAMPIONS_ROOM_WALLACE, EverGrandeCity_ChampionsRoom_Movement_WallaceExitStart
	applymovement LOCALID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerExitStart
	applymovement LOCALID_CHAMPIONS_ROOM_BIRCH, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_CHAMPIONS_ROOM_RIVAL, EverGrandeCity_ChampionsRoom_Movement_RivalFollows
	waitmovement 0
	delay 20
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox EverGrandeCity_ChampionsRoom_Text_WallaceWaitOutside, MSGBOX_DEFAULT
	checkplayergender
	call_if_eq VAR_RESULT, MALE, EverGrandeCity_ChampionsRoom_EventScript_MayCongratulations
	call_if_eq VAR_RESULT, FEMALE, EverGrandeCity_ChampionsRoom_EventScript_BrendanCongratulations
	closemessage
	applymovement LOCALID_CHAMPIONS_ROOM_WALLACE, EverGrandeCity_ChampionsRoom_Movement_WallaceExit
	applymovement LOCALID_PLAYER, EverGrandeCity_ChampionsRoom_Movement_PlayerExit
	waitmovement 0
	setflag FLAG_HIDE_PETALBURG_GYM_GREETER
	warp MAP_EVER_GRANDE_CITY_HALL_OF_FAME, 7, 16
	waitstate
	releaseall
	end

EverGrandeCity_ChampionsRoom_EventScript_MayCongratulations::
	msgbox EverGrandeCity_ChampionsRoom_Text_MayCongratulations, MSGBOX_DEFAULT
	return

EverGrandeCity_ChampionsRoom_EventScript_BrendanCongratulations::
	msgbox EverGrandeCity_ChampionsRoom_Text_BrendanCongratulations, MSGBOX_DEFAULT
	return

EverGrandeCity_ChampionsRoom_EventScript_RivalApproachPlayer::
	applymovement LOCALID_CHAMPIONS_ROOM_RIVAL, EverGrandeCity_ChampionsRoom_Movement_RivalApproachPlayer
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

EverGrandeCity_ChampionsRoom_EventScript_RivalLookBackAndForth::
	applymovement LOCALID_CHAMPIONS_ROOM_RIVAL, EverGrandeCity_ChampionsRoom_Movement_RivalLookBackAndForth
	waitmovement 0
	return

EverGrandeCity_ChampionsRoom_Movement_PlayerExitStart:
	walk_up
	walk_up
	step_end

EverGrandeCity_ChampionsRoom_Movement_PlayerExit:
	walk_up
	delay_16
	walk_up
	delay_8
	set_invisible
	step_end

EverGrandeCity_ChampionsRoom_Movement_RivalApproachPlayer:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_in_place_faster_right
	step_end

EverGrandeCity_ChampionsRoom_Movement_RivalLookBackAndForth:
	walk_in_place_faster_up
	delay_16
	walk_in_place_faster_right
	delay_16
	walk_in_place_faster_up
	delay_8
	walk_in_place_faster_right
	delay_16
	step_end

EverGrandeCity_ChampionsRoom_Movement_RivalFollows:
	walk_right
	walk_up
	step_end

EverGrandeCity_ChampionsRoom_Movement_WallaceExitStart:
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

EverGrandeCity_ChampionsRoom_Movement_WallaceExit:
	walk_up
	delay_8
	set_invisible
	step_end

EverGrandeCity_ChampionsRoom_Movement_BirchArrives:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_in_place_faster_left
	step_end

EverGrandeCity_ChampionsRoom_Text_IntroSpeech:
	.string "Adriano: Benvenuto, {PLAYER}{KUN}.\p"
	.string "Quello che è successo a Ceneride…\n"
	.string "Hai fatto un lavoro superbo, mettendo\l"
	.string "fine alla crisi tutto da solo.\p"
	.string "Oops!\p"
	.string "Non sarebbe giusto dire che hai\n"
	.string "risolto la crisi da solo.\p"
	.string "Hai superato quel momento terribile\n"
	.string "lottando in armonia con i tuoi Pokémon.\p"
	.string "Hai visto le Ancore svegliarsi e il\n"
	.string "Custode scendere dal cielo.\p"
	.string "Noi Allenatori cresciamo i Pokémon\n"
	.string "donando loro strumenti e insegnando\l"
	.string "nuove tecniche di lotta.\p"
	.string "Ma anche noi impariamo tantissimo\n"
	.string "dai Pokémon.\p"
	.string "Ed è qui che i tuoi traguardi vengono\n"
	.string "messi alla prova, contro quelli di\l"
	.string "chi è venuto prima di te.\p"
	.string "Ora vediamo se sei degno di\n"
	.string "custodire Hoenn!\p"
	.string "Chi, in tutta Hoenn, sa danzare con più\n"
	.string "eleganza insieme ai propri Pokémon?\p"
	.string "Mostramelo, qui e ora!$"

EverGrandeCity_ChampionsRoom_Text_Defeat:
	.string "Io, il Campione, cado sconfitto…\p"
	.string "Che lotta meravigliosa.\n"
	.string "Eri elegante, quasi da far rabbia.\l"
	.string "Eppure, che splendore!\p"
	.string "Complimenti, {PLAYER}{KUN}!\p"
	.string "Sei un Allenatore di Pokémon\n"
	.string "davvero nobile!$"

EverGrandeCity_ChampionsRoom_Text_PostBattleSpeech:
	.string "Adriano: I Pokémon che hai mandato\n"
	.string "in campo…\p"
	.string "A tratti danzavano come una brezza\n"
	.string "di primavera, a tratti colpivano\l"
	.string "come un fulmine.\p"
	.string "Li hai guidati con un'eleganza\n"
	.string "leggera, eppure sicura.\p"
	.string "Persino io sono rimasto abbagliato\n"
	.string "dalla tua magistrale esibizione!\p"
	.string "Ora sei sulla vetta gloriosa\n"
	.string "della Lega Pokémon.\p"
	.string "Davanti a tutta la regione di Hoenn,\n"
	.string "io ti proclamo nuovo…$"

EverGrandeCity_ChampionsRoom_Text_MayAdvice:
	.string "Arianna: Allora, allora! {PLAYER}{KUN}!\p"
	.string "Prima di sfidare il Campione, ascolta\n"
	.string "il mio consiglio: ricordati di…$"

EverGrandeCity_ChampionsRoom_Text_MayItsAlreadyOver:
	.string "Arianna: …Ah.\n"
	.string "Aspetta, aspetta, aspetta.\p"
	.string "{PLAYER}{KUN}…\n"
	.string "Vuoi dire che…\l"
	.string "Hai già vinto?\p"
	.string "… … …\n"
	.string "Segnato!$"

EverGrandeCity_ChampionsRoom_Text_BrendanAdvice:
	.string "Arianna: Allora, allora! {PLAYER}!\p"
	.string "Vuoi un consiglio prima di sfidare\n"
	.string "il Campione? Ricordati di…$"

EverGrandeCity_ChampionsRoom_Text_BrendanYouveWon:
	.string "Arianna: {PLAYER}…\p"
	.string "Mi stai dicendo che…\n"
	.string "hai già battuto il Campione?! Segnato!$"

EverGrandeCity_ChampionsRoom_Text_BirchArriveRatePokedex:
	.string "Prof. Birch: Visto?\n"
	.string "Che ti dicevo, {RIVAL}?\p"
	.string "Non te l'avevo detto che non c'era\n"
	.string "da preoccuparsi per {PLAYER}{KUN}?\p"
	.string "… … … … … … … …\n"
	.string "{PLAYER}{KUN}, alla fine ce l'hai fatta.\p"
	.string "Quando ho saputo che avevi battuto\n"
	.string "tuo padre alla Palestra di Petalipoli,\l"
	.string "ho pensato che avessi una possibilità…\p"
	.string "Ma addirittura diventare Campione!\n"
	.string "Straordinario!\p"
	.string "Ah, giusto!\p"
	.string "E il tuo Pokédex, a che punto è?\n"
	.string "Su, fammi dare un'occhiata.$"

EverGrandeCity_ChampionsRoom_Text_BirchCongratulations:
	.string "Prof. Birch: Comunque sia,\n"
	.string "congratulazioni!\p"
	.string "Ora entra a testa alta nell'ultima\n"
	.string "sala!$"

EverGrandeCity_ChampionsRoom_Text_WallaceComeWithMe:
	.string "Adriano: {PLAYER}{KUN}…\p"
	.string "No, lascia che lo dica come si deve.\p"
	.string "Nuovo Campione!\p"
	.string "Vieni con me.$"

EverGrandeCity_ChampionsRoom_Text_WallaceWaitOutside:
	.string "Adriano: Mi dispiace, ma…\p"
	.string "da qui in poi possono entrare solo\n"
	.string "gli Allenatori diventati Campioni.\p"
	.string "Dovrai aspettare fuori insieme\n"
	.string "al Prof. Birch.$"

EverGrandeCity_ChampionsRoom_Text_MayCongratulations:
	.string "Arianna: Uffa…\p"
	.string "… … … … … … … …\n"
	.string "… … … … … … … …\p"
	.string "Scherzo! Va benissimo così!\n"
	.string "Le regole sono regole!\p"
	.string "{PLAYER}{KUN}!\n"
	.string "Congratulazioni, davvero!$"

EverGrandeCity_ChampionsRoom_Text_BrendanCongratulations:
	.string "Arianna: Cooosa?! … … … … …\n"
	.string "… … … … … … … …\p"
	.string "Beh, se la regola è questa,\n"
	.string "non ci posso fare niente.\p"
	.string "{PLAYER}, sei stato grande!\n"
	.string "Congratulazioni!$"

