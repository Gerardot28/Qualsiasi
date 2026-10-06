MtChimney_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, MtChimney_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, MtChimney_OnTransition
	.byte 0

MtChimney_OnTransition:
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 1
	end

MtChimney_OnResume:
	setstepcallback STEP_CB_ASH
	end

MtChimney_EventScript_Archie::
	lock
	faceplayer
	call_if_unset FLAG_EVIL_LEADER_PLEASE_STOP, MtChimney_EventScript_ArchieGoStopTeamMagma
	call_if_set FLAG_EVIL_LEADER_PLEASE_STOP, MtChimney_EventScript_ArchieBusyFighting
	closemessage
	applymovement LOCALID_MT_CHIMNEY_ARCHIE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setflag FLAG_EVIL_LEADER_PLEASE_STOP
	release
	end

MtChimney_EventScript_ArchieGoStopTeamMagma::
	msgbox MtChimney_Text_ArchieGoStopTeamMagma, MSGBOX_DEFAULT
	return

MtChimney_EventScript_ArchieBusyFighting::
	msgbox MtChimney_Text_ArchieIHaveMyHandsFull, MSGBOX_DEFAULT
	return

MtChimney_EventScript_Maxie::
	lockall
	playbgm MUS_ENCOUNTER_MAGMA, FALSE
	msgbox MtChimney_Text_MeteoriteWillActivateVolcano, MSGBOX_DEFAULT
	applymovement LOCALID_MT_CHIMNEY_MAXIE, Common_Movement_FacePlayer
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_MT_CHIMNEY_MAXIE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MT_CHIMNEY_MAXIE, Common_Movement_Delay48
	waitmovement 0
	msgbox MtChimney_Text_MaxieIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_MAXIE_MT_CHIMNEY, MtChimney_Text_MaxieDefeat
	msgbox MtChimney_Text_MaxieYouHaventSeenLastOfMagma, MSGBOX_DEFAULT
	closemessage
	delay 30
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_MT_CHIMNEY_MAXIE
	removeobject LOCALID_MT_CHIMNEY_MAGMA_GRUNT_1
	removeobject LOCALID_MT_CHIMNEY_TABITHA
	removeobject LOCALID_MT_CHIMNEY_MAGMA_GRUNT_2
	setflag FLAG_HIDE_MT_CHIMNEY_TEAM_MAGMA
	fadescreenswapbuffers FADE_FROM_BLACK
	setobjectxyperm LOCALID_MT_CHIMNEY_ARCHIE, 10, 12
	addobject LOCALID_MT_CHIMNEY_ARCHIE
	call_if_eq VAR_FACING, DIR_EAST, MtChimney_EventScript_ArchieApproachPlayerEast
	call_if_eq VAR_FACING, DIR_NORTH, MtChimney_EventScript_ArchieApproachPlayerNorth
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox MtChimney_Text_ArchieThankYou, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_FACING, DIR_EAST, MtChimney_EventScript_ArchieExitEast
	call_if_eq VAR_FACING, DIR_NORTH, MtChimney_EventScript_ArchieExitNorth
	removeobject LOCALID_MT_CHIMNEY_ARCHIE
	setflag FLAG_HIDE_MT_CHIMNEY_TEAM_AQUA
	setflag FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY
	clearflag FLAG_HIDE_FALLARBOR_HOUSE_PROF_COZMO
	setflag FLAG_HIDE_METEOR_FALLS_1F_1R_COZMO
	clearflag FLAG_HIDE_MT_CHIMNEY_LAVA_COOKIE_LADY
	releaseall
	end

MtChimney_EventScript_ArchieApproachPlayerEast::
	applymovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieApproachPlayerEast
	waitmovement 0
	return

MtChimney_EventScript_ArchieApproachPlayerNorth::
	applymovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieApproachPlayerNorth
	waitmovement 0
	return

MtChimney_EventScript_ArchieExitEast::
	applymovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieExitEast
	waitmovement 0
	return

MtChimney_EventScript_ArchieExitNorth::
	applymovement LOCALID_MT_CHIMNEY_ARCHIE, MtChimney_Movement_ArchieExitNorth
	waitmovement 0
	return

MtChimney_EventScript_LavaCookieLady::
	lock
	faceplayer
	showmoneybox 0, 0
	msgbox MtChimney_Text_LavaCookiesJust200, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtChimney_EventScript_DeclineLavaCookie
	checkmoney 200
	goto_if_eq VAR_RESULT, FALSE, MtChimney_EventScript_NotEnoughMoney
	msgbox MtChimney_Text_ThankYouDear, MSGBOX_DEFAULT
	checkitemspace ITEM_LAVA_COOKIE
	call_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_RemoveMoney
.if OW_SHOW_ITEM_DESCRIPTIONS == OW_ITEM_DESCRIPTIONS_OFF
	giveitem ITEM_LAVA_COOKIE
	goto_if_eq VAR_RESULT, FALSE, MtChimney_EventScript_BagIsFull
	hidemoneybox
	release
	end

MtChimney_EventScript_BagIsFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	hidemoneybox
	release
	end
.else
	hidemoneybox
	giveitem ITEM_LAVA_COOKIE
	goto_if_eq VAR_RESULT, FALSE, MtChimney_EventScript_BagIsFull
	release
	end

MtChimney_EventScript_BagIsFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	release
	end
.endif @ OW_SHOW_ITEM_DESCRIPTIONS


MtChimney_EventScript_RemoveMoney::
	removemoney 200
	updatemoneybox
	return

MtChimney_EventScript_DeclineLavaCookie::
	msgbox MtChimney_Text_OhFineThen, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

MtChimney_EventScript_NotEnoughMoney::
	msgbox MtChimney_Text_YouveNotGotTheMoney, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

MtChimney_Movement_ArchieApproachPlayerEast:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	step_end

MtChimney_Movement_ArchieExitEast:
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_ArchieApproachPlayerNorth:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	step_end

MtChimney_Movement_ArchieExitNorth:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused1:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused2:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused3:
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused4:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused5:
	delay_16
	delay_16
	delay_16
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused6:
	delay_16
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused7:
	delay_16
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

MtChimney_Movement_Unused8:
	delay_16
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

MtChimney_Movement_Unused9:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused10:
	walk_down
	step_end

MtChimney_Movement_Unused11:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused12:
	delay_16
	delay_16
	walk_in_place_faster_left
	delay_16
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused13:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	walk_in_place_faster_left
	delay_16
	walk_down
	walk_down
	step_end

MtChimney_Movement_Unused14:
	lock_facing_direction
	walk_fast_left
	unlock_facing_direction
	face_right
	step_end

MtChimney_Movement_Unused15:
	walk_left
	walk_in_place_faster_right
	delay_16
	step_end

MtChimney_EventScript_BusyAquaGrunt1::
	msgbox MtChimney_Text_MagmaOutnumbersUs, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyAquaGrunt2::
	msgbox MtChimney_Text_LessHabitatForWaterPokemon, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyAquaGrunt3::
	msgbox MtChimney_Text_MagmasNameSimilar, MSGBOX_SIGN
	end

MtChimney_EventScript_Tabitha::
	trainerbattle_single TRAINER_TABITHA_MT_CHIMNEY, MtChimney_Text_TabithaIntro, MtChimney_Text_TabithaDefeat
	msgbox MtChimney_Text_TabithaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MT_CHIMNEY_2, MtChimney_Text_Grunt2Intro, MtChimney_Text_Grunt2Defeat
	msgbox MtChimney_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_BusyMagmaGrunt1::
	msgbox MtChimney_Text_TeamAquaAlwaysMessingWithPlans, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt2::
	msgbox MtChimney_Text_MeteoritesPackAmazingPower, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt3::
	msgbox MtChimney_Text_YouBetterNotMessWithUs, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt4::
	msgbox MtChimney_Text_AquasNameSimilar, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt5::
	msgbox MtChimney_Text_DouseThemInFire, MSGBOX_SIGN
	end

MtChimney_EventScript_BusyMagmaGrunt6::
	msgbox MtChimney_Text_KeepMakingMoreLand, MSGBOX_SIGN
	end

MtChimney_EventScript_MagmaPoochyena::
	msgbox MtChimney_Text_Bufoh, MSGBOX_SIGN
	end

MtChimney_EventScript_AquaPoochyena::
	msgbox MtChimney_Text_Bushaa, MSGBOX_SIGN
	end

MtChimney_EventScript_MeteoriteMachine::
	lockall
	goto_if_unset FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, MtChimney_EventScript_MachineOn
	goto_if_set FLAG_RECEIVED_METEORITE, MtChimney_EventScript_MachineOff
	msgbox MtChimney_Text_RemoveTheMeteorite, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtChimney_EventScript_LeaveMeteoriteAlone
	msgbox MtChimney_Text_PlayerRemovedMeteorite, MSGBOX_DEFAULT
	giveitem ITEM_METEORITE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_METEORITE
	releaseall
	end

MtChimney_EventScript_LeaveMeteoriteAlone::
	msgbox MtChimney_Text_PlayerLeftMeteorite, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_MachineOff::
	msgbox MtChimney_Text_MachineMakesNoResponse, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_MachineOn::
	msgbox MtChimney_Text_MeteoriteFittedOnMachine, MSGBOX_DEFAULT
	releaseall
	end

MtChimney_EventScript_RouteSign::
	msgbox MtChimney_Text_RouteSign, MSGBOX_SIGN
	end

MtChimney_EventScript_Shelby::
	trainerbattle_single TRAINER_SHELBY_1, MtChimney_Text_ShelbyIntro, MtChimney_Text_ShelbyDefeat, MtChimney_EventScript_DefeatedShelby
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_RematchShelby
	msgbox MtChimney_Text_ShelbyPostBattle, MSGBOX_DEFAULT
	release
	end

MtChimney_EventScript_DefeatedShelby::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtChimney_Text_ShelbyRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_SHELBY_1
	release
	end

MtChimney_EventScript_RematchShelby::
	trainerbattle_rematch TRAINER_SHELBY_1, MtChimney_Text_ShelbyRematchIntro, MtChimney_Text_ShelbyRematchDefeat
	msgbox MtChimney_Text_ShelbyPostRematch, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Melissa::
	trainerbattle_single TRAINER_MELISSA, MtChimney_Text_MelissaIntro, MtChimney_Text_MelissaDefeat
	msgbox MtChimney_Text_MelissaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Sheila::
	trainerbattle_single TRAINER_SHEILA, MtChimney_Text_SheilaIntro, MtChimney_Text_SheilaDefeat
	msgbox MtChimney_Text_SheilaPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Shirley::
	trainerbattle_single TRAINER_SHIRLEY, MtChimney_Text_ShirleyIntro, MtChimney_Text_ShirleyDefeat
	msgbox MtChimney_Text_ShirleyPostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MT_CHIMNEY_1, MtChimney_Text_Grunt1Intro, MtChimney_Text_Grunt1Defeat
	msgbox MtChimney_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MtChimney_EventScript_Sawyer::
	trainerbattle_single TRAINER_SAWYER_1, MtChimney_Text_SawyerIntro, MtChimney_Text_SawyerDefeat, MtChimney_EventScript_SawyerDefeated
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MtChimney_EventScript_SawyerRematch
	msgbox MtChimney_Text_SawyerPostBattle, MSGBOX_DEFAULT
	release
	end

MtChimney_EventScript_SawyerDefeated::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MtChimney_Text_SawyerRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_SAWYER_1
	release
	end

MtChimney_EventScript_SawyerRematch::
	trainerbattle_rematch TRAINER_SAWYER_1, MtChimney_Text_SawyerRematchIntro, MtChimney_Text_SawyerRematchDefeat
	msgbox MtChimney_Text_SawyerPostRematch, MSGBOX_AUTOCLOSE
	end

MtChimney_Text_MeteoriteWillActivateVolcano:
	.string "Il Meteorite amplifica l'energia\n"
	.string "della Soglia…\p"
	.string "Con questa macchina lo inietterò nel\n"
	.string "magma, e sveglierà ciò che dorme\l"
	.string "sotto Hoenn…\p"
	.string "E ogni Varco si chiuderà.\n"
	.string "Per sempre.\l"
	.string "Fufufu…$"

MtChimney_Text_MaxieIntro:
	.string "Ettore: Mh?\n"
	.string "E tu chi saresti?\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Ulisse si lamentava di un ragazzino\n"
	.string "che continua a ficcare il naso negli\l"
	.string "affari del Team Omega.\p"
	.string "Dunque parlava di te.\p"
	.string "Fufufu… E ora vorresti ostacolare\n"
	.string "anche noi, il Team Alpha?\p"
	.string "Ascoltami bene, ragazzino.\n"
	.string "Un tempo anche la terra era aperta\l"
	.string "e indifesa, come una casa senza porte.\p"
	.string "Negli ultimi dieci anni i Varchi si\n"
	.string "sono moltiplicati per sette. Pokémon\l"
	.string "e cose di altri mondi entrano a Hoenn\l"
	.string "senza chiedere il permesso.\p"
	.string "Ogni muro che costruisco è una casa\n"
	.string "che salvo. Hoenn ha bisogno di un\l"
	.string "muro… e io glielo darò.\p"
	.string "Per questo ci serve il potere di ciò\n"
	.string "che dorme in questa montagna…\p"
	.string "Oh. Non c'era bisogno che tu sapessi\n"
	.string "così tanto.\p"
	.string "Ma poco importa!\n"
	.string "Ti insegnerò cosa succede a chi\l"
	.string "intralcia il nostro grande disegno!$"

MtChimney_Text_MaxieDefeat:
	.string "Impossibile…\p"
	.string "I miei calcoli erano perfetti.\n"
	.string "Battuto da un ragazzino?!$"

MtChimney_Text_MaxieYouHaventSeenLastOfMagma:
	.string "Ettore: Basta così.\n"
	.string "Per questa volta mi ritiro.\p"
	.string "Ma non illuderti: non è l'ultima\n"
	.string "volta che vedi il Team Alpha.\p"
	.string "Fufufu…\p"
	.string "Anche senza il Meteorite… se avrò\n"
	.string "quella Gemma… Fufufu…$"

MtChimney_Text_TabithaIntro:
	.string "Hehehe!\p"
	.string "Sei arrivato fin quassù, eh?\p"
	.string "Ma è troppo tardi! Il Meteorite delle\n"
	.string "Cascate Meteora è già nelle mani del\l"
	.string "Capo! Per l'Alpha!$"

MtChimney_Text_TabithaDefeat:
	.string "Hehehe…\p"
	.string "Ho perso, sì… Ma se il Capo\n"
	.string "risveglia quella cosa…$"

MtChimney_Text_TabithaPostBattle:
	.string "Capo, presto!\n"
	.string "Inietti l'energia del Meteorite!\p"
	.string "Hehehe…$"

MtChimney_Text_Grunt2Intro:
	.string "Noi del Team Alpha lavoriamo sodo\n"
	.string "per il bene di tutti.\p"
	.string "Quando quella cosa si sveglierà, i\n"
	.string "Varchi si chiuderanno per sempre.\p"
	.string "Niente più stranieri, niente più\n"
	.string "sorprese. Saranno tutti felici!$"

MtChimney_Text_Grunt2Defeat:
	.string "Eh?\n"
	.string "Come sarebbe che ho perso?$"

MtChimney_Text_Grunt2PostBattle:
	.string "Il nostro Capo dice: “Così saremo\n"
	.string "tutti al sicuro”.\p"
	.string "Ma allora perché tutti continuano a\n"
	.string "metterci i bastoni tra le ruote?$"

MtChimney_Text_Grunt1Intro:
	.string "L'altra notte si è aperto un Varco\n"
	.string "proprio nel mio orto!\p"
	.string "E qualcosa di un altro mondo si è\n"
	.string "mangiato tutti i miei pomodori!\p"
	.string "Chiudiamoli tutti! Per l'Alpha!$"

MtChimney_Text_Grunt1Defeat:
	.string "I miei pomodori…$"

MtChimney_Text_Grunt1PostBattle:
	.string "Uno come te dovrebbe andare a\n"
	.string "sguazzare con quelli del Team Omega!$"

MtChimney_Text_TeamAquaAlwaysMessingWithPlans:
	.string "Quel Team Omega è una piaga…\n"
	.string "Ci rovinano sempre i piani!$"

MtChimney_Text_MeteoritesPackAmazingPower:
	.string "Che potenza, il Meteorite! Vibra\n"
	.string "come se sentisse la Soglia!$"

MtChimney_Text_YouBetterNotMessWithUs:
	.string "Non metterti contro di noi!\p"
	.string "Stiamo risvegliando quella cosa\n"
	.string "per il bene di tutta Hoenn!$"

MtChimney_Text_AquasNameSimilar:
	.string "Noi siamo il Team Alpha!\p"
	.string "Loro sono il Team Omega!\p"
	.string "L'Alpha viene sempre prima\n"
	.string "dell'Omega. Lo dice l'alfabeto!$"

MtChimney_Text_DouseThemInFire:
	.string "Sì!\n"
	.string "Bruciategli le vele!$"

MtChimney_Text_KeepMakingMoreLand:
	.string "Fuori gli stranieri!\n"
	.string "Hoenn resta Hoenn!$"

MtChimney_Text_Bufoh:
	.string "Grrbau!$"

MtChimney_Text_ArchieGoStopTeamMagma:
	.string "Ulisse: Per mille Varchi, {PLAYER}!\n"
	.string "Dovevo immaginare che saresti venuto!\p"
	.string "Guarda tu stesso cosa combinano\n"
	.string "quei fanatici del Team Alpha!\p"
	.string "Vogliono pompare l'energia del\n"
	.string "Meteorite rubato dentro il vulcano!\p"
	.string "Se ci riescono, ogni Varco si chiuderà\n"
	.string "di colpo, con chiunque ci sia dentro!\p"
	.string "Vai, mozzo! Fermali!$"

MtChimney_Text_ArchieIHaveMyHandsFull:
	.string "Ulisse: Grrr…\p"
	.string "Vorrei fermare Ettore con le mie\n"
	.string "mani, ma non posso!\p"
	.string "Non mentre lotto contro tre\n"
	.string "avversari alla volta! Ciurma, duri!$"

MtChimney_Text_ArchieThankYou:
	.string "Ulisse: {PLAYER}!\n"
	.string "Grazie, mozzo!\p"
	.string "Con il tuo aiuto abbiamo mandato a\n"
	.string "fondo il piano del Team Alpha!\p"
	.string "Però… tu…\n"
	.string "Da che parte stai, tu?\p"
	.string "Bah, poco importa.\p"
	.string "Noi continueremo a dare la caccia\n"
	.string "a quelle talpe rosse.\p"
	.string "{PLAYER}, ci rivedremo! Ahrrr-ha-ha!$"

MtChimney_Text_MagmaOutnumbersUs:
	.string "Maledizione… quelli dell'Alpha sono\n"
	.string "troppi! Non riusciamo a tenerli!$"

MtChimney_Text_LessHabitatForWaterPokemon:
	.string "Se chiudono i Varchi, i Pokémon di\n"
	.string "altri mondi resteranno schiacciati!$"

MtChimney_Text_MagmasNameSimilar:
	.string "Noi siamo il Team Omega!\p"
	.string "Loro sono il Team Alpha!\p"
	.string "Loro vogliono tornare al principio,\n"
	.string "noi andare oltre ogni confine! Arrr!$"

MtChimney_Text_Bushaa:
	.string "Gnarrr!$"

MtChimney_Text_LavaCookiesJust200:
	.string "I Lavottini sono la specialità del\n"
	.string "Monte Camino.\p"
	.string "Ne vuoi assaggiare uno? Solo 200¥.$"

MtChimney_Text_ThankYouDear:
	.string "Grazie, tesoro!$"

MtChimney_Text_YouveNotGotTheMoney:
	.string "Oh, caro. Senza soldi non si compra\n"
	.string "proprio niente.$"

MtChimney_Text_OhFineThen:
	.string "Oh, va bene, allora.$"

MtChimney_Text_MeteoriteFittedOnMachine:
	.string "Un Meteorite è incastrato in una\n"
	.string "strana macchina…\p"
	.string "La macchina sembra accumulare\n"
	.string "energia nel Meteorite. L'aria frigge.$"

MtChimney_Text_RemoveTheMeteorite:
	.string "Un Meteorite è incastrato in una\n"
	.string "strana macchina…\p"
	.string "Vuoi togliere il Meteorite?$"

MtChimney_Text_PlayerRemovedMeteorite:
	.string "{PLAYER} ha tolto il Meteorite dalla\n"
	.string "strana macchina.$"

MtChimney_Text_PlayerLeftMeteorite:
	.string "{PLAYER} ha lasciato il Meteorite\n"
	.string "dov'era.$"

MtChimney_Text_MachineMakesNoResponse:
	.string "Questa strana macchina…\n"
	.string "Non dà più alcun segno di vita.$"

MtChimney_Text_RouteSign:
	.string "{DOWN_ARROW} Passo Selvaggio\n"
	.string "Più avanti: Cuordilava$"

MtChimney_Text_ShelbyIntro:
	.string "Sono stata alle terme e ho rimesso\n"
	.string "in sesto queste vecchie ossa.\l"
	.string "Ora mi sento fortissima!$"

MtChimney_Text_ShelbyDefeat:
	.string "Oh, santo cielo.\n"
	.string "Ma guarda che bravo!$"

MtChimney_Text_ShelbyPostBattle:
	.string "Bene, bene, ho perso. Non posso più\n"
	.string "chiamarmi Esperta, vero?$"

MtChimney_Text_ShelbyRegister:
	.string "Grazie, caro. Mi sono divertita come\n"
	.string "se lottassi con mio nipote.\p"
	.string "Torna a trovarmi per una rivincita,\n"
	.string "mi raccomando.$"

MtChimney_Text_ShelbyRematchIntro:
	.string "Se il tuo cuore batte insieme a quello\n"
	.string "dei tuoi Pokémon, potrai fare\l"
	.string "grandi cose.$"

MtChimney_Text_ShelbyRematchDefeat:
	.string "Oh, santo cielo.\n"
	.string "Ma guarda che bravo!$"

MtChimney_Text_ShelbyPostRematch:
	.string "Forse il tuo cuore è già una cosa\n"
	.string "sola con quello dei tuoi Pokémon.$"

MtChimney_Text_MelissaIntro:
	.string "Ho il fuoco dentro, tesoro.\n"
	.string "Non resisto! Devo lottare!$"

MtChimney_Text_MelissaDefeat:
	.string "Uuh, che lotta rovente!$"

MtChimney_Text_MelissaPostBattle:
	.string "Il calore del Monte Camino mi scalda\n"
	.string "il cuore, tesoro!\p"
	.string "Dicono che persino i Pokémon arrivati\n"
	.string "dai Varchi vengano quassù a scaldarsi!$"

MtChimney_Text_SheilaIntro:
	.string "Finalmente sono arrivata sul Monte\n"
	.string "Camino. Ora voglio lottare!$"

MtChimney_Text_SheilaDefeat:
	.string "Il tuo modo di lottare… è come\n"
	.string "un'eruzione del Monte Camino!$"

MtChimney_Text_SheilaPostBattle:
	.string "Te l'ho detto, sono finalmente\n"
	.string "arrivata sul Monte Camino. Sarebbe\l"
	.string "un peccato fare solo la turista…\p"
	.string "Voglio lottare un po' e comprare\n"
	.string "dei Lavottini come souvenir.$"

MtChimney_Text_ShirleyIntro:
	.string "Dopo il bagno alle terme mi sento\n"
	.string "benissimo!\l"
	.string "Vincerò di sicuro!$"

MtChimney_Text_ShirleyDefeat:
	.string "Ahi!\n"
	.string "Fuori dall'acqua mi viene freddo.$"

MtChimney_Text_ShirleyPostBattle:
	.string "Mi sa che mi faccio un altro tuffo\n"
	.string "alle terme. Vieni anche tu?\p"
	.string "Scherzo!$"

MtChimney_Text_SawyerIntro:
	.string "Che montagna magnifica! E quanta\n"
	.string "gente calorosa da queste parti!$"

MtChimney_Text_SawyerDefeat:
	.string "Oh, anche tu sei una bella\n"
	.string "testa calda!$"

MtChimney_Text_SawyerPostBattle:
	.string "Mi ci vuole un bel bagno alle terme\n"
	.string "di Cuordilava con la gente del posto!$"

MtChimney_Text_SawyerRegister:
	.string "Mi piacciono i tipi focosi come te.\n"
	.string "Ti registro nel mio PokéNav.$"

MtChimney_Text_SawyerRematchIntro:
	.string "Da queste parti sono circondato da\n"
	.string "gente calorosa. Non perderò!$"

MtChimney_Text_SawyerRematchDefeat:
	.string "Accidenti, sei sempre la solita\n"
	.string "testa calda!$"

MtChimney_Text_SawyerPostRematch:
	.string "A dire il vero, qui fa caldissimo.\n"
	.string "Sono troppo vestito per queste parti.$"
