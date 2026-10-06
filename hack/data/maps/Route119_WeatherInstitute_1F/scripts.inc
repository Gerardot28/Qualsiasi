Route119_WeatherInstitute_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route119_WeatherInstitute_1F_OnTransition
	.byte 0

Route119_WeatherInstitute_1F_OnTransition:
	call_if_eq VAR_WEATHER_INSTITUTE_STATE, 0, Route119_WeatherInstitute_1F_EventScript_SetLittleBoyPos
	end

Route119_WeatherInstitute_1F_EventScript_SetLittleBoyPos::
	setobjectxyperm LOCALID_WEATHER_INSTITUTE_LITTLE_BOY, 0, 5
	setobjectmovementtype LOCALID_WEATHER_INSTITUTE_LITTLE_BOY, MOVEMENT_TYPE_FACE_RIGHT
	return

Route119_WeatherInstitute_1F_EventScript_LittleBoy::
	lock
	faceplayer
	special GetPlayerBigGuyGirlString
	goto_if_eq VAR_WEATHER_INSTITUTE_STATE, 0, Route119_WeatherInstitute_1F_EventScript_LittleBoyTeamAquaHere
	msgbox Route119_WeatherInstitute_1F_Text_WowYoureStrong, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_LittleBoyTeamAquaHere::
	msgbox Route119_WeatherInstitute_1F_Text_EveryoneWentUpstairs, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_InstituteWorker1::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_GAME_CLEAR, Route119_WeatherInstitute_1F_EventScript_StudyingRain
	setvar VAR_0x8004, 0
	call_if_set FLAG_DEFEATED_KYOGRE, Route119_WeatherInstitute_1F_EventScript_LegendaryDefeated
	call_if_set FLAG_DEFEATED_GROUDON, Route119_WeatherInstitute_1F_EventScript_LegendaryDefeated
	goto_if_eq VAR_0x8004, 2, Route119_WeatherInstitute_1F_EventScript_StudyingRain  @ Both defeated
	msgbox Route119_WeatherInstitute_1F_Text_NoticingAbnormalWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_LegendaryDefeated::
	addvar VAR_0x8004, 1
	return

Route119_WeatherInstitute_1F_EventScript_StudyingRain::
	msgbox Route119_WeatherInstitute_1F_Text_ProfStudyingRain, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_InstituteWorker2::
	msgbox Route119_WeatherInstitute_1F_Text_WhatWereAquasUpTo, MSGBOX_NPC
	end

Route119_WeatherInstitute_1F_EventScript_Bed::
	lockall
	msgbox Route119_WeatherInstitute_1F_Text_TakeRestInBed, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	releaseall
	end

Route119_WeatherInstitute_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_1, Route119_WeatherInstitute_1F_Text_Grunt1Intro, Route119_WeatherInstitute_1F_Text_Grunt1Defeat
	msgbox Route119_WeatherInstitute_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_1F_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_4, Route119_WeatherInstitute_1F_Text_Grunt4Intro, Route119_WeatherInstitute_1F_Text_Grunt4Defeat
	msgbox Route119_WeatherInstitute_1F_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_1F_Text_Grunt1Intro:
	.string "Il Capo si è interessato alle ricerche\n"
	.string "che fanno qui, così ci ha mandati\l"
	.string "all'arrembaggio.\p"
	.string "E tu smettila di ficcanasare, arrr!$"

Route119_WeatherInstitute_1F_Text_Grunt1Defeat:
	.string "Maledizione…\n"
	.string "Affondato da un ragazzino…$"

Route119_WeatherInstitute_1F_Text_Grunt1PostBattle:
	.string "Il nostro Capo sa tutto.\p"
	.string "Ma io sono solo una Recluta. Che ne so\n"
	.string "di cosa gli passa per la testa?$"

Route119_WeatherInstitute_1F_Text_Grunt4Intro:
	.string "Eh?\n"
	.string "Che ci fa un ragazzino qui?$"

Route119_WeatherInstitute_1F_Text_Grunt4Defeat:
	.string "Eh?\n"
	.string "Ho perso?!$"

Route119_WeatherInstitute_1F_Text_Grunt4PostBattle:
	.string "Oh, no… Mi faranno una bella predica\n"
	.string "per aver perso contro un ragazzino…\p"
	.string "Tanto vale farmi un pisolino nel letto…$"

Route119_WeatherInstitute_1F_Text_EveryoneWentUpstairs:
	.string "Mentre dormivo, sono saliti tutti\n"
	.string "al piano di sopra!$"

Route119_WeatherInstitute_1F_Text_WowYoureStrong:
	.string "Wow, sei fortissimo!\p"
	.string "Vorrei diventare un Allenatore di\n"
	.string "Pokémon come te!$"

Route119_WeatherInstitute_1F_Text_ProfStudyingRain:
	.string "Il Professore adora la pioggia.\n"
	.string "Questo è un fatto.\p"
	.string "Ma se piove sempre, la gente finisce\n"
	.string "nei guai. Anche questo è un fatto.\p"
	.string "E così il Professore studia come\n"
	.string "sfruttare la pioggia a fin di bene.$"

Route119_WeatherInstitute_1F_Text_NoticingAbnormalWeather:
	.string "Al secondo piano dell'Istituto\n"
	.string "studiamo il clima della regione\l"
	.string "di Hoenn.\p"
	.string "Di recente abbiamo notato casi isolati\n"
	.string "e temporanei di siccità e di piogge\l"
	.string "torrenziali…\p"
	.string "Qualcuno dice che le Ancore\n"
	.string "dormono leggere…$"

Route119_WeatherInstitute_1F_Text_WhatWereAquasUpTo:
	.string "Ciao!\n"
	.string "Ci hai salvati tutti!\p"
	.string "Quello che non capisco è che cosa\n"
	.string "cercasse qui il Team Omega.$"

Route119_WeatherInstitute_1F_Text_TakeRestInBed:
	.string "C'è un letto…\n"
	.string "Riposiamoci un po'.$"

