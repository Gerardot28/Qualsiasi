Route119_WeatherInstitute_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route119_WeatherInstitute_2F_OnTransition
	.byte 0

Route119_WeatherInstitute_2F_OnTransition:
	call_if_eq VAR_WEATHER_INSTITUTE_STATE, 0, Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaHere
	call_if_eq VAR_WEATHER_INSTITUTE_STATE, 1, Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaGone
	call_if_set FLAG_SYS_GAME_CLEAR, Route119_WeatherInstitute_2F_EventScript_SetScientistPosGameClear
	end

Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaHere::
	setobjectxyperm LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, 1, 6
	setobjectmovementtype LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, MOVEMENT_TYPE_FACE_RIGHT
	return

Route119_WeatherInstitute_2F_EventScript_SetScientistPosAquaGone::
	setobjectxyperm LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, 4, 6
	setobjectmovementtype LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, MOVEMENT_TYPE_FACE_RIGHT
	return

Route119_WeatherInstitute_2F_EventScript_SetScientistPosGameClear::
	setobjectxyperm LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, 2, 2
	setobjectmovementtype LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, MOVEMENT_TYPE_FACE_UP
	return

Route119_WeatherInstitute_2F_EventScript_Grunt5::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_5, Route119_WeatherInstitute_2F_Text_Grunt5Intro, Route119_WeatherInstitute_2F_Text_Grunt5Defeat
	msgbox Route119_WeatherInstitute_2F_Text_Grunt5PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_2, Route119_WeatherInstitute_2F_Text_Grunt2Intro, Route119_WeatherInstitute_2F_Text_Grunt2Defeat
	msgbox Route119_WeatherInstitute_2F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_Grunt3::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_3, Route119_WeatherInstitute_2F_Text_Grunt3Intro, Route119_WeatherInstitute_2F_Text_Grunt3Defeat
	msgbox Route119_WeatherInstitute_2F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_Shelly::
	trainerbattle_single TRAINER_SHELLY_WEATHER_INSTITUTE, Route119_WeatherInstitute_2F_Text_ShellyIntro, Route119_WeatherInstitute_2F_Text_ShellyDefeat, Route119_WeatherInstitute_2F_EventScript_ShellyDefeated
	msgbox Route119_WeatherInstitute_2F_Text_ShellyPostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_2F_EventScript_ShellyDefeated::
	msgbox Route119_WeatherInstitute_2F_Text_ShellyPostBattle, MSGBOX_DEFAULT
	closemessage
	addobject LOCALID_WEATHER_INSTITUTE_2F_GRUNT_3
	applymovement LOCALID_WEATHER_INSTITUTE_2F_GRUNT_3, Route119_WeatherInstitute_2F_Movement_GruntApproachShelly
	applymovement LOCALID_PLAYER, Route119_WeatherInstitute_2F_Movement_ShovePlayerOutOfWay
	waitmovement 0
	msgbox Route119_WeatherInstitute_2F_Text_TeamMagmaJustPassedBy, MSGBOX_DEFAULT
	closemessage
	playse SE_PIN
	applymovement LOCALID_WEATHER_INSTITUTE_2F_SHELLY, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_WEATHER_INSTITUTE_2F_SHELLY, Common_Movement_Delay48
	waitmovement 0
	msgbox Route119_WeatherInstitute_2F_Text_WeHaveToHurryToMtPyre, MSGBOX_DEFAULT
	closemessage
	setvar VAR_WEATHER_INSTITUTE_STATE, 1
	clearflag FLAG_HIDE_WEATHER_INSTITUTE_2F_WORKERS
	fadedefaultbgm
	fadescreen FADE_TO_BLACK
	setflag FLAG_HIDE_ROUTE_119_TEAM_AQUA
	setflag FLAG_HIDE_WEATHER_INSTITUTE_2F_AQUA_GRUNT_M
	removeobject LOCALID_WEATHER_INSTITUTE_2F_GRUNT_1
	removeobject LOCALID_WEATHER_INSTITUTE_2F_GRUNT_2
	removeobject LOCALID_WEATHER_INSTITUTE_2F_SHELLY
	removeobject LOCALID_WEATHER_INSTITUTE_2F_GRUNT_4
	removeobject LOCALID_WEATHER_INSTITUTE_2F_GRUNT_3
	applymovement LOCALID_PLAYER, Route119_WeatherInstitute_2F_Movement_PlayerReturnToPosition
	waitmovement 0
	fadescreen FADE_FROM_BLACK
	applymovement LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST, Route119_WeatherInstitute_2F_Movement_ScientistApproachPlayer
	waitmovement 0
	copyobjectxytoperm LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST
	goto Route119_WeatherInstitute_2F_EventScript_ReceiveCastform
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastform::
	msgbox Route119_WeatherInstitute_2F_Text_ThanksPleaseTakePokemon, MSGBOX_DEFAULT
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CASTFORM_NORMAL
	givemon SPECIES_CASTFORM_NORMAL, 25, ITEM_MYSTIC_WATER
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, Route119_WeatherInstitute_2F_EventScript_ReceiveCastformParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, Route119_WeatherInstitute_2F_EventScript_ReceiveCastformPC
	goto Common_EventScript_NoMoreRoomForPokemon
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastformParty::
	call Route119_WeatherInstitute_2F_EventScript_ReceivedCastformFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route119_WeatherInstitute_2F_EventScript_ExplainCastform
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto Route119_WeatherInstitute_2F_EventScript_ExplainCastform
	end

Route119_WeatherInstitute_2F_EventScript_ReceiveCastformPC::
	call Route119_WeatherInstitute_2F_EventScript_ReceivedCastformFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route119_WeatherInstitute_2F_EventScript_SendCastformToPC
	call Common_EventScript_NameReceivedBoxMon
	goto Route119_WeatherInstitute_2F_EventScript_SendCastformToPC
	end

Route119_WeatherInstitute_2F_EventScript_SendCastformToPC::
	call Common_EventScript_TransferredToPC
	goto Route119_WeatherInstitute_2F_EventScript_ExplainCastform
	end

Route119_WeatherInstitute_2F_EventScript_ReceivedCastformFanfare::
	playfanfare MUS_OBTAIN_ITEM
	message Route119_WeatherInstitute_2F_Text_PlayerReceivedCastform
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_CASTFORM_NORMAL
	return

Route119_WeatherInstitute_2F_EventScript_ExplainCastform::
	msgbox Route119_WeatherInstitute_2F_Text_PokemonChangesWithWeather, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_CASTFORM
	release
	end

Route119_WeatherInstitute_2F_EventScript_ScientistMentionWeather::
	goto_if_set FLAG_SYS_GAME_CLEAR, Route119_WeatherInstitute_2F_EventScript_TryStartAbnormalWeather
	msgbox Route119_WeatherInstitute_2F_Text_ChangingWeatherRidiculous, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_EventScript_TryStartAbnormalWeather::
	setvar VAR_0x8004, 0
	call_if_set FLAG_DEFEATED_KYOGRE, Route119_WeatherInstitute_2F_EventScript_LegendaryDefeated
	call_if_set FLAG_DEFEATED_GROUDON, Route119_WeatherInstitute_2F_EventScript_LegendaryDefeated
	goto_if_eq VAR_0x8004, 2, Route119_WeatherInstitute_2F_EventScript_NoAbnormalWeather  @ Both defeated
	call_if_unset FLAG_TEMP_2, Route119_WeatherInstitute_2F_EventScript_CreateAbnormalWeather
	specialvar VAR_RESULT, GetAbnormalWeatherMapNameAndType
	goto_if_eq VAR_RESULT, 1, Route119_WeatherInstitute_2F_EventScript_KyogreWeather
	msgbox Route119_WeatherInstitute_2F_Text_GroudonWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_EventScript_KyogreWeather::
	msgbox Route119_WeatherInstitute_2F_Text_KyogreWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_EventScript_CreateAbnormalWeather::
	special CreateAbnormalWeatherEvent
	setflag FLAG_TEMP_2
	return

Route119_WeatherInstitute_2F_EventScript_LegendaryDefeated::
	addvar VAR_0x8004, 1
	return

Route119_WeatherInstitute_2F_EventScript_NoAbnormalWeather::
	msgbox Route119_WeatherInstitute_2F_Text_NoAbnormalWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_2F_Movement_GruntApproachShelly:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

Route119_WeatherInstitute_2F_Movement_ShovePlayerOutOfWay:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	ride_water_current_up
	walk_in_place_faster_down
	step_end

Route119_WeatherInstitute_2F_Movement_PlayerReturnToPosition:
	slide_down
	walk_in_place_faster_left
	step_end

Route119_WeatherInstitute_2F_Movement_ScientistApproachPlayer:
	walk_right
	walk_right
	walk_right
	step_end

Route119_WeatherInstitute_2F_EventScript_WeatherScientist::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_CASTFORM, Route119_WeatherInstitute_2F_EventScript_ScientistMentionWeather
	goto Route119_WeatherInstitute_2F_EventScript_ReceiveCastform
	end

Route119_WeatherInstitute_2F_Text_Grunt2Intro:
	.string "L'Istituto ha creato un Pokémon che\n"
	.string "ha a che fare con il meteo.\l"
	.string "Siamo qui per prenderlo, arrr!$"

Route119_WeatherInstitute_2F_Text_Grunt2Defeat:
	.string "Un ragazzino ci affonda il piano?!$"

Route119_WeatherInstitute_2F_Text_Grunt2PostBattle:
	.string "Se il Pokémon che hanno creato qui\n"
	.string "comanda il meteo a piacimento, al\l"
	.string "Team Omega serve eccome!$"

Route119_WeatherInstitute_2F_Text_Grunt3Intro:
	.string "Siamo il Team Omega!\n"
	.string "Arriviamo ovunque ci sia qualcosa di\l"
	.string "raro. Vele al vento!$"

Route119_WeatherInstitute_2F_Text_Grunt3Defeat:
	.string "Mi hai beccato!$"

Route119_WeatherInstitute_2F_Text_Grunt3PostBattle:
	.string "Non hai idea di quale rotta segua\n"
	.string "il Team Omega! Oltre ogni mare!\l"
	.string "Levati di mezzo!$"

Route119_WeatherInstitute_2F_Text_Grunt5Intro:
	.string "Non dirmi che cerchi anche tu quel\n"
	.string "Pokémon del meteo!\p"
	.string "Niente da fare!\n"
	.string "Siamo arrivati prima noi!$"

Route119_WeatherInstitute_2F_Text_Grunt5Defeat:
	.string "Oh, ma guarda i miei Pokémon!$"

Route119_WeatherInstitute_2F_Text_Grunt5PostBattle:
	.string "Hmpf, e allora?\n"
	.string "Quello che vogliamo…\p"
	.string "Quello che vogliamo davvero non è\n"
	.string "qui… Ihihihihi…$"

Route119_WeatherInstitute_2F_Text_ShellyIntro:
	.string "Ahahahah!\p"
	.string "Vuoi ficcare il naso negli affari\n"
	.string "del Team Omega?\p"
	.string "Ahahah, che carino. O non hai paura\n"
	.string "di niente, o sei ingenuo… o tutte e due!\p"
	.string "Ci servono i dati sulle piogge anomale,\n"
	.string "tesoro: ci diranno dove dorme\l"
	.string "l'Ancora del Mare.\p"
	.string "E tu non ti metterai in mezzo.\n"
	.string "Ti sistemo io, piccolino!$"

Route119_WeatherInstitute_2F_Text_ShellyDefeat:
	.string "Ahahahah!\n"
	.string "Sei odiosamente forte, tesoro!$"

Route119_WeatherInstitute_2F_Text_ShellyPostBattle:
	.string "Come se non bastasse il Team Alpha\n"
	.string "tra i piedi, adesso ci sei pure tu!\p"
	.string "Si può sapere perché ficchi il naso\n"
	.string "nei nostri affari, tesoro?$"

Route119_WeatherInstitute_2F_Text_TeamMagmaJustPassedBy:
	.string "Admin! Abbiamo un problema!\p"
	.string "Una squadra del Team Alpha è appena\n"
	.string "passata davanti all'Istituto Meteo.\p"
	.string "L'Alpha sta salendo al\n"
	.string "Monte Pira!$"

Route119_WeatherInstitute_2F_Text_WeHaveToHurryToMtPyre:
	.string "Cosa?!\p"
	.string "Non possiamo perdere altro tempo!\n"
	.string "Dobbiamo correre al Monte Pira!\p"
	.string "Vele al vento, ciurma!\n"
	.string "Si cambia rotta!\p"
	.string "Ahahahah!\n"
	.string "Team Alpha, aspettaci!$"

Route119_WeatherInstitute_2F_Text_ThanksPleaseTakePokemon:
	.string "Grazie!\n"
	.string "Grazie a te siamo salvi!\p"
	.string "Forse è uno strano modo di ringraziarti,\n"
	.string "ma prendi questo Pokémon.$"

@ Unused
Route119_WeatherInstitute_2F_Text_NoRoomForPokemon:
	.string "Hm? Sembra che tu non abbia posto\n"
	.string "per questo Pokémon.$"

Route119_WeatherInstitute_2F_Text_PlayerReceivedCastform:
	.string "{PLAYER} ha ricevuto Castform!$"

Route119_WeatherInstitute_2F_Text_PokemonChangesWithWeather:
	.string "Quel Pokémon cambia forma in base\n"
	.string "al tempo atmosferico…\l"
	.string "e impazzisce quando si apre un Varco!\p"
	.string "Qui all'Istituto ne abbiamo tanti:\n"
	.string "tienilo pure, è tuo.$"

Route119_WeatherInstitute_2F_Text_ChangingWeatherRidiculous:
	.string "Studio la pioggia da tanti anni.\n"
	.string "Qui la chiamano “pioggia dei Varchi”…\p"
	.string "Ma pensare che l'uomo possa cambiare\n"
	.string "il tempo a piacimento è ridicolo.$"

Route119_WeatherInstitute_2F_Text_GroudonWeather:
	.string "Seguo l'andamento del meteo nella\n"
	.string "regione di Hoenn.\p"
	.string "Al momento è stata registrata una\n"
	.string "siccità nell'area {STR_VAR_1}.\p"
	.string "Che l'Ancora della Terra si agiti\n"
	.string "vicino all'area {STR_VAR_1}…?$"

Route119_WeatherInstitute_2F_Text_KyogreWeather:
	.string "Seguo l'andamento del meteo nella\n"
	.string "regione di Hoenn.\p"
	.string "Al momento si registrano piogge\n"
	.string "torrenziali nell'area {STR_VAR_1}.\p"
	.string "Che l'Ancora del Mare si agiti\n"
	.string "vicino all'area {STR_VAR_1}…?$"

Route119_WeatherInstitute_2F_Text_NoAbnormalWeather:
	.string "Non si registrano più condizioni\n"
	.string "meteo anomale.\p"
	.string "Un po' di pioggia ogni tanto è una\n"
	.string "benedizione, in fondo.$"

