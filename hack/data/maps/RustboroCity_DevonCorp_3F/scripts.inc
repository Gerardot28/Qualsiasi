RustboroCity_DevonCorp_3F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_DevonCorp_3F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, RustboroCity_DevonCorp_3F_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, RustboroCity_DevonCorp_3F_OnFrame
	.byte 0

RustboroCity_DevonCorp_3F_OnTransition:
	call_if_eq VAR_DEVON_CORP_3F_STATE, 0, RustboroCity_DevonCorp_3F_EventScript_SetEmployeePos
	end

RustboroCity_DevonCorp_3F_EventScript_SetEmployeePos::
	setobjectxyperm LOCALID_DEVON_CORP_3F_EMPLOYEE, 3, 2
	setobjectmovementtype LOCALID_DEVON_CORP_3F_EMPLOYEE, MOVEMENT_TYPE_FACE_LEFT
	return

RustboroCity_DevonCorp_3F_OnWarp:
	map_script_2 VAR_DEVON_CORP_3F_STATE, 0, RustboroCity_DevonCorp_3F_EventScript_PlayerFaceEast
	.2byte 0

RustboroCity_DevonCorp_3F_EventScript_PlayerFaceEast::
	turnobject LOCALID_PLAYER, DIR_EAST
	end

RustboroCity_DevonCorp_3F_OnFrame:
	map_script_2 VAR_DEVON_CORP_3F_STATE, 0, RustboroCity_DevonCorp_3F_EventScript_MeetPresident
	.2byte 0

RustboroCity_DevonCorp_3F_EventScript_MeetPresident::
	lockall
	msgbox RustboroCity_DevonCorp_3F_Text_ThisIs3rdFloorWaitHere, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_EmployeeWalkOffscreen
	waitmovement 0
	delay 80
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_EmployeeReturnToPlayer
	waitmovement 0
	msgbox RustboroCity_DevonCorp_3F_Text_WordWithPresidentComeWithMe, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_FOLLOW_ME, FALSE
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_LeadPlayerToPresident
	applymovement LOCALID_PLAYER, RustboroCity_DevonCorp_3F_Movement_PlayerFollowToPresident
	waitmovement 0
	msgbox RustboroCity_DevonCorp_3F_Text_PleaseGoAhead, MSGBOX_DEFAULT
	closemessage
	fadedefaultbgm
	applymovement LOCALID_DEVON_CORP_3F_EMPLOYEE, RustboroCity_DevonCorp_3F_Movement_EmployeeFaceDesk
	applymovement LOCALID_PLAYER, RustboroCity_DevonCorp_3F_Movement_PlayerApproachDesk
	waitmovement 0
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneIHaveFavor, MSGBOX_DEFAULT
	giveitem ITEM_LETTER
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneWantYouToHaveThis, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message RustboroCity_DevonCorp_3F_Text_ReceivedPokenav
	waitfanfare
	setflag FLAG_SYS_POKENAV_GET
	setflag FLAG_RECEIVED_POKENAV
	setflag FLAG_HIDE_RUSTBORO_CITY_POKEMON_SCHOOL_SCOTT
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneExplainPokenavRestUp, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox RustboroCity_DevonCorp_3F_Text_MrStoneGoWithCautionAndCare, MSGBOX_DEFAULT
	setflag FLAG_HIDE_ROUTE_116_WANDAS_BOYFRIEND
	clearflag FLAG_HIDE_RUSTURF_TUNNEL_WANDAS_BOYFRIEND
	clearflag FLAG_HIDE_RUSTURF_TUNNEL_WANDA
	clearflag FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY
	clearflag FLAG_HIDE_BRINEYS_HOUSE_PEEKO
	setvar VAR_BRINEY_LOCATION, 1
	clearflag FLAG_HIDE_RUSTBORO_CITY_RIVAL
	setvar VAR_DEVON_CORP_3F_STATE, 1
	setvar VAR_RUSTBORO_CITY_STATE, 6
	releaseall
	end

RustboroCity_DevonCorp_3F_Movement_Unused:
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_left
	step_end

RustboroCity_DevonCorp_3F_Movement_LeadPlayerToPresident:
	walk_right
	walk_right
	walk_right
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_left
	step_end

RustboroCity_DevonCorp_3F_Movement_EmployeeFaceDesk:
	delay_16
	walk_in_place_faster_down
	step_end

RustboroCity_DevonCorp_3F_Movement_EmployeeWalkOffscreen:
	walk_right
	walk_right
	walk_right
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

RustboroCity_DevonCorp_3F_Movement_EmployeeReturnToPlayer:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_left
	walk_left
	walk_left
	step_end

RustboroCity_DevonCorp_3F_Movement_PlayerFollowToPresident:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

RustboroCity_DevonCorp_3F_Movement_PlayerApproachDesk:
	delay_16
	delay_16
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

RustboroCity_DevonCorp_3F_EventScript_MrStone::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_EXP_SHARE, RustboroCity_DevonCorp_3F_EventScript_MrStoneAfterFavor
	goto_if_set FLAG_DELIVERED_STEVEN_LETTER, RustboroCity_DevonCorp_3F_EventScript_GiveExpShare
	msgbox RustboroCity_DevonCorp_3F_Text_CountingOnYou, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

RustboroCity_DevonCorp_3F_EventScript_GiveExpShare::
	msgbox RustboroCity_DevonCorp_3F_Text_ThankYouForDeliveringLetter, MSGBOX_DEFAULT
	giveitem ITEM_EXP_SHARE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_EXP_SHARE
	msgbox RustboroCity_DevonCorp_3F_Text_ExplainExpShare, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

RustboroCity_DevonCorp_3F_EventScript_MrStoneAfterFavor::
	msgbox RustboroCity_DevonCorp_3F_Text_NotFamiliarWithTrends, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

RustboroCity_DevonCorp_3F_EventScript_Employee::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_REPEAT_BALL, RustboroCity_DevonCorp_3F_EventScript_EmployeeBalls
	msgbox RustboroCity_DevonCorp_3F_Text_VisitCaptSternShipyard, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_3F_EventScript_EmployeeBalls::
	msgbox RustboroCity_DevonCorp_3F_Text_RepeatAndTimerHugelyPopular, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_3F_EventScript_RareRocksDisplay::
	msgbox RustboroCity_DevonCorp_3F_Text_RareRocksDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_3F_Text_MrStoneIHaveFavor:
	.string "Sono il Sig. Petri, presidente della\n"
	.string "Devon S.p.A.\p"
	.string "Mi hanno appena raccontato tutto!\p"
	.string "Hai salvato il mio personale non una,\n"
	.string "ma ben due volte!\p"
	.string "Ho un favore da chiedere a una\n"
	.string "persona straordinaria come te.\p"
	.string "So che porterai il nostro pacco al\n"
	.string "Cantiere di Porto Selcepoli.\p"
	.string "Sono i pezzi di uno stabilizzatore\n"
	.string "di Varchi per Capitan Stern…\p"
	.string "Ma non divaghiamo! Lungo la strada,\n"
	.string "potresti fermarti a Bluruvia?\p"
	.string "Vorrei che consegnassi una Lettera\n"
	.string "a mio figlio Rocco, che è laggiù.$"

RustboroCity_DevonCorp_3F_Text_MrStoneWantYouToHaveThis:
	.string "Sig. Petri: Ora, devi sapere che io\n"
	.string "sono un grande presidente.\p"
	.string "Non sarei mai così tirchio da chiedere\n"
	.string "un favore senza ricambiarlo.\p"
	.string "Per questo voglio darti questo!$"

RustboroCity_DevonCorp_3F_Text_ReceivedPokenav:
	.string "{PLAYER} ha ricevuto il PokéNav!$"

RustboroCity_DevonCorp_3F_Text_MrStoneExplainPokenavRestUp:
	.string "Sig. Petri: Quell'apparecchio…\p"
	.string "È un Navigatore Pokémon, o PokéNav\n"
	.string "per gli amici.\p"
	.string "È uno strumento indispensabile per\n"
	.string "ogni Allenatore in viaggio.\p"
	.string "Contiene la mappa della regione\n"
	.string "di Hoenn.\p"
	.string "Così troverai subito Bluruvia e\n"
	.string "Porto Selcepoli!\p"
	.string "A proposito… Due gruppi si contendono\n"
	.string "i Varchi: il Team Omega e il Team Alpha.\p"
	.string "Un tempo erano una cosa sola, la\n"
	.string "Società Soglia, e studiavano i Varchi.\p"
	.string "E a finanziarli ero io. Che sciocco!\p"
	.string "Ora seminano guai in lungo e in largo.\n"
	.string "Stai molto attento.\p"
	.string "E prima di rimetterti in viaggio,\n"
	.string "riposati un po'.$"

RustboroCity_DevonCorp_3F_Text_MrStoneGoWithCautionAndCare:
	.string "Sig. Petri: Bene, allora, {PLAYER}{KUN},\n"
	.string "vai con prudenza e con giudizio!$"

RustboroCity_DevonCorp_3F_Text_CountingOnYou:
	.string "Sig. Petri: Conto su di te!$"

RustboroCity_DevonCorp_3F_Text_ThankYouForDeliveringLetter:
	.string "Sig. Petri: Hai consegnato la mia\n"
	.string "Lettera? Grazie infinite!\p"
	.string "Ecco un piccolo segno di gratitudine.\n"
	.string "A un Allenatore farà comodo.$"

RustboroCity_DevonCorp_3F_Text_ExplainExpShare:
	.string "Sig. Petri: Un Pokémon che tiene il\n"
	.string "Condividi Esp. riceve una parte dei\l"
	.string "Punti Esperienza della lotta.\p"
	.string "Li ottiene anche se non ha\n"
	.string "partecipato davvero alla lotta.\p"
	.string "Direi che il Condividi Esp. è utilissimo\n"
	.string "per allenare i Pokémon più deboli.$"

RustboroCity_DevonCorp_3F_Text_NotFamiliarWithTrends:
	.string "Sig. Petri: Fin da giovane mi sono\n"
	.string "buttato a capofitto nel lavoro.\p"
	.string "Per questo non sono molto aggiornato\n"
	.string "sulle mode e cose simili.\p"
	.string "Ma è vero che tutti i giovani\n"
	.string "vogliono sfidare la Lega Pokémon?$"

RustboroCity_DevonCorp_3F_Text_ThisIs3rdFloorWaitHere:
	.string "Questo è il terzo piano della\n"
	.string "Devon S.p.A.\p"
	.string "Qui c'è l'ufficio del nostro\n"
	.string "presidente.\p"
	.string "Comunque, non so davvero come\n"
	.string "ringraziarti per quello che hai fatto.\p"
	.string "Ehm… A proposito, il pacco che hai\n"
	.string "recuperato per noi…\p"
	.string "Potresti consegnarlo tu al Cantiere\n"
	.string "di Porto Selcepoli?\p"
	.string "Sarebbe terribile se quei pirati\n"
	.string "provassero a rubarlo di nuovo.\p"
	.string "E poi derubano sempre me…\n"
	.string "Perché sempre a me?!\p"
	.string "Ehm… Ehehe, quindi… posso contare\n"
	.string "su di te?\p"
	.string "Ah, giusto!\n"
	.string "Potresti aspettare qui un secondo?$"

RustboroCity_DevonCorp_3F_Text_WordWithPresidentComeWithMe:
	.string "Il nostro presidente vorrebbe\n"
	.string "scambiare due parole con te.\p"
	.string "Vieni con me, per favore.$"

RustboroCity_DevonCorp_3F_Text_PleaseGoAhead:
	.string "Prego, accomodati.$"

RustboroCity_DevonCorp_3F_Text_VisitCaptSternShipyard:
	.string "Quando arrivi al Cantiere di Porto\n"
	.string "Selcepoli, cerca Capitan Stern.$"

RustboroCity_DevonCorp_3F_Text_RepeatAndTimerHugelyPopular:
	.string "I nuovi prodotti Devon, la Bis Ball\n"
	.string "e la Timer Ball, vanno a ruba tra\l"
	.string "gli Allenatori.$"

RustboroCity_DevonCorp_3F_Text_RareRocksDisplay:
	.string "È una collezione di rocce e pietre\n"
	.string "rare raccolte dal presidente.\p"
	.string "Alcune hanno uno strano riflesso…\n"
	.string "come se non fossero di questo mondo.$"

