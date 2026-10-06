MossdeepCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MossdeepCity_OnTransition
	.byte 0

MossdeepCity_OnTransition:
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_1
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_2
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_3
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_4
	call_if_set FLAG_SYS_WEATHER_CTRL, Common_EventScript_SetAbnormalWeather
	end

MossdeepCity_EventScript_PokefanF::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_DIVE, MossdeepCity_EventScript_PokefanFMagmaGone
	msgbox MossdeepCity_Text_SpaceCenterReceivedLetter, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_PokefanFMagmaGone::
	msgbox MossdeepCity_Text_SpaceCenterLaunchingRockets, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_Sailor::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_DIVE, MossdeepCity_EventScript_SailorMagmaGone
	msgbox MossdeepCity_Text_MossdeepTargetedByMagma, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_SailorMagmaGone::
	msgbox MossdeepCity_Text_FeelReliefOnLand, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_NinjaBoy::
	msgbox MossdeepCity_Text_WailmerWatching, MSGBOX_NPC
	end

MossdeepCity_EventScript_ExpertM::
	msgbox MossdeepCity_Text_LifeNeedsSeaToLive, MSGBOX_NPC
	end

MossdeepCity_EventScript_Girl::
	msgbox MossdeepCity_Text_NiceIfWorldCoveredByFlowers, MSGBOX_NPC
	end

MossdeepCity_EventScript_Woman::
	msgbox MossdeepCity_Text_SpecialSpaceCenterRock, MSGBOX_NPC
	end

MossdeepCity_EventScript_WhiteRock::
	msgbox MossdeepCity_Text_ItsAWhiteRock, MSGBOX_SIGN
	end

MossdeepCity_EventScript_GymSign::
	msgbox MossdeepCity_Text_GymSign, MSGBOX_SIGN
	end

MossdeepCity_EventScript_CitySign::
	msgbox MossdeepCity_Text_CitySign, MSGBOX_SIGN
	end

MossdeepCity_EventScript_SpaceCenterSign::
	msgbox MossdeepCity_Text_SpaceCenterSign, MSGBOX_SIGN
	end

MossdeepCity_EventScript_VisitedMossdeep::
	setflag FLAG_VISITED_MOSSDEEP_CITY
	setvar VAR_TEMP_1, 1
	end

MossdeepCity_EventScript_TeamMagmaEnterSpaceCenter::
	lockall
	applymovement LOCALID_MOSSDEEP_MAXIE, MossdeepCity_Movement_MaxieGestureToSpaceCenter
	waitmovement 0
	applymovement LOCALID_MOSSDEEP_GRUNT_1, MossdeepCity_Movement_GruntFaceSpaceCenter
	applymovement LOCALID_MOSSDEEP_GRUNT_2, MossdeepCity_Movement_GruntFaceSpaceCenter
	applymovement LOCALID_MOSSDEEP_GRUNT_3, MossdeepCity_Movement_GruntFaceSpaceCenter
	applymovement LOCALID_MOSSDEEP_GRUNT_4, MossdeepCity_Movement_GruntFaceSpaceCenter
	waitmovement 0
	applymovement LOCALID_MOSSDEEP_MAXIE, MossdeepCity_Movement_MaxieEnterSpaceCenter
	applymovement LOCALID_MOSSDEEP_GRUNT_1, MossdeepCity_Movement_Grunt1EnterSpaceCenter
	applymovement LOCALID_MOSSDEEP_GRUNT_2, MossdeepCity_Movement_Grunt2EnterSpaceCenter
	applymovement LOCALID_MOSSDEEP_GRUNT_3, MossdeepCity_Movement_Grunt3EnterSpaceCenter
	applymovement LOCALID_MOSSDEEP_GRUNT_4, MossdeepCity_Movement_Grunt4EnterSpaceCenter
	waitmovement 0
	removeobject LOCALID_MOSSDEEP_MAXIE
	removeobject LOCALID_MOSSDEEP_GRUNT_1
	removeobject LOCALID_MOSSDEEP_GRUNT_2
	removeobject LOCALID_MOSSDEEP_GRUNT_3
	removeobject LOCALID_MOSSDEEP_GRUNT_4
	delay 30
	setvar VAR_MOSSDEEP_CITY_STATE, 2
	setflag FLAG_HIDE_MOSSDEEP_CITY_TEAM_MAGMA
	releaseall
	end

MossdeepCity_Movement_MaxieGestureToSpaceCenter:
	delay_16
	face_right
	delay_16
	delay_16
	delay_16
	delay_16
	face_left
	delay_16
	delay_16
	step_end

MossdeepCity_Movement_GruntFaceSpaceCenter:
	face_right
	delay_16
	delay_16
	delay_16
	step_end

MossdeepCity_Movement_MaxieEnterSpaceCenter:
	walk_down
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

MossdeepCity_Movement_Grunt1EnterSpaceCenter:
	delay_16
	delay_8
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

MossdeepCity_Movement_Grunt2EnterSpaceCenter:
	delay_16
	delay_8
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

MossdeepCity_Movement_Grunt3EnterSpaceCenter:
	delay_16
	delay_8
	walk_down
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

MossdeepCity_Movement_Grunt4EnterSpaceCenter:
	delay_16
	delay_8
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

MossdeepCity_EventScript_Man::
	lock
	faceplayer
	msgbox MossdeepCity_Text_SurfExhilarating, MSGBOX_DEFAULT
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

MossdeepCity_EventScript_KingsRockBoy::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_KINGS_ROCK, MossdeepCity_EventScript_ReceivedKingsRock
	msgbox MossdeepCity_Text_WantKingsRockStevenGaveMe, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MossdeepCity_EventScript_DeclineKingsRock
	msgbox MossdeepCity_Text_YouCanKeepIt, MSGBOX_DEFAULT
	giveitem ITEM_KINGS_ROCK
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_KINGS_ROCK
	release
	end

MossdeepCity_EventScript_ReceivedKingsRock::
	msgbox MossdeepCity_Text_StevensHouseOverThere, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_DeclineKingsRock::
	msgbox MossdeepCity_Text_WhatToDoWithWeirdRock, MSGBOX_DEFAULT
	release
	end

MossdeepCity_EventScript_BlackBelt::
	msgbox MossdeepCity_Text_SootopolisNewGymLeader, MSGBOX_NPC
	end

MossdeepCity_EventScript_Scott::
	lock
	faceplayer
	msgbox MossdeepCity_Text_ScottSomethingWrongWithTown, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_FACING, DIR_NORTH, MossdeepCity_EventScript_ScottExitNorth
	call_if_eq VAR_FACING, DIR_EAST, MossdeepCity_EventScript_ScottExitEast
	addvar VAR_SCOTT_STATE, 1
	removeobject LOCALID_MOSSDEEP_SCOTT
	release
	end

MossdeepCity_EventScript_ScottExitNorth::
	applymovement LOCALID_PLAYER, MossdeepCity_Movement_PlayerWatchScottExit
	applymovement LOCALID_MOSSDEEP_SCOTT, MossdeepCity_Movement_ScottExitNorth
	waitmovement 0
	return

MossdeepCity_EventScript_ScottExitEast::
	applymovement LOCALID_PLAYER, MossdeepCity_Movement_PlayerWatchScottExit
	applymovement LOCALID_MOSSDEEP_SCOTT, MossdeepCity_Movement_ScottExitEast
	waitmovement 0
	return

MossdeepCity_Movement_PlayerWatchScottExit:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MossdeepCity_Movement_ScottExitNorth:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MossdeepCity_Movement_ScottExitEast:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MossdeepCity_Text_WantKingsRockStevenGaveMe:
	.string "Me l'ha regalata Rocco, ma non ho\n"
	.string "proprio idea di cosa farci.\p"
	.string "Mi pare che si chiami Roccia di Re.\n"
	.string "La vuoi tu?$"

MossdeepCity_Text_YouCanKeepIt:
	.string "E perché mai la vorresti?\n"
	.string "Sei proprio strano.\p"
	.string "Puoi tenerla, ma acqua in bocca con\n"
	.string "Rocco, eh!$"

MossdeepCity_Text_StevensHouseOverThere:
	.string "La casa di Rocco è proprio là!$"

MossdeepCity_Text_WhatToDoWithWeirdRock:
	.string "Vero? Lo pensi anche tu! Che ci si fa\n"
	.string "con un sasso così strano?$"

MossdeepCity_Text_WailmerWatching:
	.string "Dalle parti di Verdeazzupoli si\n"
	.string "vedono i Wailmer selvatici.\p"
	.string "Lo chiamano, ehm…\n"
	.string "Com'era, adesso…\p"
	.string "Wai, Wai, Wai…\p"
	.string "Wailmer-watching!$"

MossdeepCity_Text_SpaceCenterReceivedLetter:
	.string "Il Centro Spaziale dell'isola lancia\n"
	.string "dei razzi enormi.\p"
	.string "Ultimamente c'è un gran trambusto per\n"
	.string "una lettera che hanno ricevuto.$"

MossdeepCity_Text_SpaceCenterLaunchingRockets:
	.string "Il Centro Spaziale dell'isola lancia\n"
	.string "dei razzi enormi.\p"
	.string "Chissà se un giorno uno di loro\n"
	.string "arriverà a sfiorare la Cicatrice…$"

MossdeepCity_Text_MossdeepTargetedByMagma:
	.string "Un amico marinaio mi ha detto che il\n"
	.string "Team Omega si era rintanato a Porto\l"
	.string "Alghepoli.\p"
	.string "E pare che qualcuno li abbia\n"
	.string "spazzati via!\p"
	.string "Ma ora, qui a Verdeazzupoli, nel\n"
	.string "mirino c'è il Team Alpha.\p"
	.string "Se vuoi sapere cosa tramano, fai un\n"
	.string "salto al Centro Spaziale.$"

MossdeepCity_Text_FeelReliefOnLand:
	.string "Sono un marinaio, quindi per me il mare\n"
	.string "viene prima di tutto.\p"
	.string "Però, sai? Quando torno a terra dopo\n"
	.string "un lungo viaggio, che sollievo!$"

MossdeepCity_Text_NiceIfWorldCoveredByFlowers:
	.string "Non sarebbe bello?\p"
	.string "Se il mondo intero fosse coperto di\n"
	.string "piante e fiori come quest'isola?$"

MossdeepCity_Text_LifeNeedsSeaToLive:
	.string "Ogni forma di vita ha bisogno del mare,\n"
	.string "anche se vive sulla terraferma.\p"
	.string "E la vita, quando ha fatto il suo\n"
	.string "corso, torna alla terra.\p"
	.string "Il mare e la terra sono sempre uniti.\n"
	.string "Come questa riva, guarda.\p"
	.string "Nessuno dei due può fare a meno\n"
	.string "dell'altro. Ricordatelo.$"

MossdeepCity_Text_SurfExhilarating:
	.string "Viaggiare in nave va benissimo.\p"
	.string "Ma attraversare il mare con un\n"
	.string "Pokémon che usa Surf…\p"
	.string "Quella sì che è un'emozione!\n"
	.string "Non sei d'accordo, ragazzo?$"

MossdeepCity_Text_SpecialSpaceCenterRock:
	.string "Questa roccia è speciale per la gente\n"
	.string "del Centro Spaziale.\p"
	.string "L'hanno messa qui come augurio,\n"
	.string "perché i loro razzi volino sicuri.\p"
	.string "E tu, come esprimi un desiderio?\n"
	.string "Lo affidi a una stella cadente?\p"
	.string "Io lo scrivo su un bigliettino.\n"
	.string "Faccio sempre così.$"

MossdeepCity_Text_ItsAWhiteRock:
	.string "È una roccia bianca.$"

MossdeepCity_Text_GymSign:
	.string "Palestra Pokémon di Verdeazzupoli\n"
	.string "Capipalestra: Tell e Pat\p"
	.string "“La combinazione mistica!”$"

MossdeepCity_Text_CitySign:
	.string "Verdeazzupoli\n"
	.string "“Il nostro motto: amate i Pokémon!”$"

MossdeepCity_Text_SpaceCenterSign:
	.string "Centro Spaziale di Verdeazzupoli\n"
	.string "“Il posto più vicino alle stelle.”$"

MossdeepCity_Text_ScottSomethingWrongWithTown:
	.string "Scott: {PLAYER}{KUN}, come va?\n"
	.string "Io alla grande!\p"
	.string "Ho sentito che i Capipalestra di\n"
	.string "Verdeazzupoli sono fortissimi, così\l"
	.string "sono venuto a dare un'occhiata.\p"
	.string "Ma in questa città c'è qualcosa\n"
	.string "che non va.\p"
	.string "Tutti parlano di una lettera di\n"
	.string "avvertimento e del Centro Spaziale…\p"
	.string "Comunque, non credo che la cosa\n"
	.string "mi riguardi.$"

MossdeepCity_Text_SootopolisNewGymLeader:
	.string "Ahh… Che bello sentire le onde che\n"
	.string "ti bagnano i piedi…\p"
	.string "A proposito di onde, conosci Ceneride,\n"
	.string "la città sull'isola?\p"
	.string "Pare che la Palestra di laggiù abbia\n"
	.string "un nuovo Capopalestra.\p"
	.string "Dicono che sia stato il maestro di\n"
	.string "Adriano.$"

