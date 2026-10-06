LavaridgeTown_Gym_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LavaridgeTown_Gym_1F_OnTransition
	.byte 0

LavaridgeTown_Gym_1F_OnTransition:
	call LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars
	call LavaridgeTown_Gym_1F_EventScript_CheckBuryTrainers
	end

@ Unclear where/if these temp vars are getting checked
LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars::
	setvar VAR_TEMP_B, 0
	setvar VAR_TEMP_C, 0
	setvar VAR_TEMP_D, 0
	setvar VAR_TEMP_E, 0
	setvar VAR_TEMP_F, 0
	goto_if_defeated TRAINER_COLE, LavaridgeTown_Gym_1F_EventScript_SetGeraldTempVar
	setvar VAR_TEMP_B, 1
LavaridgeTown_Gym_1F_EventScript_SetGeraldTempVar::
	goto_if_defeated TRAINER_GERALD, LavaridgeTown_Gym_1F_EventScript_SetAxleTempVar
	setvar VAR_TEMP_C, 1
LavaridgeTown_Gym_1F_EventScript_SetAxleTempVar::
	goto_if_defeated TRAINER_AXLE, LavaridgeTown_Gym_1F_EventScript_SetDanielleTempVar
	setvar VAR_TEMP_D, 1
LavaridgeTown_Gym_1F_EventScript_SetDanielleTempVar::
	goto_if_defeated TRAINER_DANIELLE, LavaridgeTown_Gym_1F_EventScript_EndSetTrainerTempVars
	setvar VAR_TEMP_E, 1
LavaridgeTown_Gym_1F_EventScript_EndSetTrainerTempVars::
	return

LavaridgeTown_Gym_1F_EventScript_CheckBuryTrainers::
	goto_if_defeated TRAINER_COLE, LavaridgeTown_Gym_1F_EventScript_CheckBuryGerald
	setobjectmovementtype LOCALID_COLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryGerald::
	goto_if_defeated TRAINER_GERALD, LavaridgeTown_Gym_1F_EventScript_CheckBuryAxle
	setobjectmovementtype LOCALID_GERALD, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryAxle::
	goto_if_defeated TRAINER_AXLE, LavaridgeTown_Gym_1F_EventScript_CheckBuryDanielle
	setobjectmovementtype LOCALID_AXLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_CheckBuryDanielle::
	goto_if_defeated TRAINER_DANIELLE, LavaridgeTown_Gym_1F_EventScript_EndCheckBuryTrainers
	setobjectmovementtype LOCALID_DANIELLE, MOVEMENT_TYPE_BURIED
LavaridgeTown_Gym_1F_EventScript_EndCheckBuryTrainers::
	return

LavaridgeTown_Gym_1F_EventScript_Flannery::
	trainerbattle_single TRAINER_FLANNERY_1, LavaridgeTown_Gym_1F_Text_FlanneryIntro, LavaridgeTown_Gym_1F_Text_FlanneryDefeat, LavaridgeTown_Gym_1F_EventScript_FlanneryDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, LavaridgeTown_Gym_1F_EventScript_FlanneryRematch
	goto_if_unset FLAG_RECEIVED_TM_OVERHEAT, LavaridgeTown_Gym_1F_EventScript_GiveOverheat2
	msgbox LavaridgeTown_Gym_1F_Text_FlanneryPostBattle, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_FlanneryDefeated::
	message LavaridgeTown_Gym_1F_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LavaridgeTown_Gym_1F_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_WHITEOUT_TO_LAVARIDGE
	setflag FLAG_DEFEATED_LAVARIDGE_GYM
	setflag FLAG_BADGE04_GET
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 4
	call Common_EventScript_SetGymTrainers
	setflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY
	setvar VAR_LAVARIDGE_TOWN_STATE, 1
	call LavaridgeTown_Gym_1F_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LavaridgeTown_Gym_1F_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LavaridgeTown_Gym_1F_EventScript_GiveOverheat2::
	giveitem ITEM_TM_OVERHEAT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox LavaridgeTown_Gym_1F_Text_ExplainOverheat, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_OVERHEAT
	release
	end

LavaridgeTown_Gym_1F_EventScript_GiveOverheat::
	giveitem ITEM_TM_OVERHEAT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox LavaridgeTown_Gym_1F_Text_ExplainOverheat, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_OVERHEAT
	return

LavaridgeTown_Gym_1F_EventScript_FlanneryRematch::
	trainerbattle_rematch_double TRAINER_FLANNERY_1, LavaridgeTown_Gym_1F_Text_FlanneryPreRematch, LavaridgeTown_Gym_1F_Text_FlanneryRematchDefeat, LavaridgeTown_Gym_1F_Text_FlanneryRematchNeedTwoMons
	msgbox LavaridgeTown_Gym_1F_Text_FlanneryPostRematch, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Cole::
	trainerbattle_lavaridge LOCALID_COLE, TRAINER_COLE, LavaridgeTown_Gym_1F_Text_ColeIntro, LavaridgeTown_Gym_1F_Text_ColeDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	end

LavaridgeTown_Gym_EventScript_CheckTrainerScript::
	call LavaridgeTown_Gym_1F_EventScript_SetTrainerTempVars
	release
	special ShouldTryGetTrainerScript
	goto_if_eq VAR_RESULT, 1, EventScript_GotoTrainerScript
	end

LavaridgeTown_Gym_1F_EventScript_Axle::
	trainerbattle_lavaridge LOCALID_AXLE, TRAINER_AXLE, LavaridgeTown_Gym_1F_Text_AxleIntro, LavaridgeTown_Gym_1F_Text_AxleDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_1F_Text_AxlePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Keegan::
	trainerbattle_lavaridge LOCALID_KEEGAN, TRAINER_KEEGAN, LavaridgeTown_Gym_B1F_Text_KeeganIntro, LavaridgeTown_Gym_B1F_Text_KeeganDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_KeeganPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Danielle::
	trainerbattle_lavaridge LOCALID_DANIELLE, TRAINER_DANIELLE, LavaridgeTown_Gym_1F_Text_DanielleIntro, LavaridgeTown_Gym_1F_Text_DanielleDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_1F_Text_DaniellePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_Gerald::
	trainerbattle_lavaridge LOCALID_GERALD, TRAINER_GERALD, LavaridgeTown_Gym_1F_Text_GeraldIntro, LavaridgeTown_Gym_1F_Text_GeraldDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_1F_Text_GeraldPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Jace::
	trainerbattle_lavaridge LOCALID_JACE, TRAINER_JACE, LavaridgeTown_Gym_B1F_Text_JaceIntro, LavaridgeTown_Gym_B1F_Text_JaceDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_JacePostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Jeff::
	trainerbattle_lavaridge LOCALID_JEFF, TRAINER_JEFF, LavaridgeTown_Gym_B1F_Text_JeffIntro, LavaridgeTown_Gym_B1F_Text_JeffDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_JeffPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_B1F_EventScript_Eli::
	trainerbattle_lavaridge LOCALID_ELI, TRAINER_ELI, LavaridgeTown_Gym_B1F_Text_EliIntro, LavaridgeTown_Gym_B1F_Text_EliDefeat, LavaridgeTown_Gym_EventScript_CheckTrainerScript
	msgbox LavaridgeTown_Gym_B1F_Text_EliPostBattle, MSGBOX_AUTOCLOSE
	end

LavaridgeTown_Gym_1F_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, LavaridgeTown_Gym_1F_EventScript_GymGuidePostVictory
	msgbox LavaridgeTown_Gym_1F_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_GymGuidePostVictory::
	msgbox LavaridgeTown_Gym_1F_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_Gym_1F_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, LavaridgeTown_Gym_1F_EventScript_GymStatueCertified
	goto LavaridgeTown_Gym_1F_EventScript_GymStatue
	end

LavaridgeTown_Gym_1F_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, LavaridgeTown_Gym_1F_EventScript_GymStatueCertified
	goto LavaridgeTown_Gym_1F_EventScript_GymStatue
	end

LavaridgeTown_Gym_1F_EventScript_GymStatueCertified::
	msgbox LavaridgeTown_Gym_1F_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

LavaridgeTown_Gym_1F_EventScript_GymStatue::
	msgbox LavaridgeTown_Gym_1F_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

LavaridgeTown_Gym_1F_Text_GymGuideAdvice:
	.string "Ehilà, {PLAYER}, futuro Campione!\n"
	.string "Come va?\p"
	.string "Fiammetta, la Capopalestra di\n"
	.string "Cuordilava, usa Pokémon di tipo Fuoco.\p"
	.string "La sua passione per i Pokémon brucia\n"
	.string "più forte e più calda di un vulcano.\p"
	.string "E da quando ci sono i Varchi, il Monte\n"
	.string "Camino è nervoso… un po' come lei!\p"
	.string "Non avvicinarti troppo, o ti scotti!\n"
	.string "Rinfrescala con un bel getto d'acqua\l"
	.string "e poi vai all'attacco!$"

LavaridgeTown_Gym_1F_Text_GymGuidePostVictory:
	.string "Uff! Che lotta incandescente!$"

LavaridgeTown_Gym_1F_Text_ColeIntro:
	.string "Ahiahiahiahi!\n"
	.string "Scotta, scotta, scotta!$"

LavaridgeTown_Gym_1F_Text_ColeDefeat:
	.string "Il sudore mi cola negli occhi…$"

LavaridgeTown_Gym_1F_Text_ColePostBattle:
	.string "Stare sepolti nella sabbia calda fa\n"
	.string "bene alla circolazione.\p"
	.string "È un toccasana per i dolori alle\n"
	.string "articolazioni.$"

LavaridgeTown_Gym_1F_Text_AxleIntro:
	.string "Sono qui per scaricare lo stress.\n"
	.string "Non venire a stressarmi tu!$"

LavaridgeTown_Gym_1F_Text_AxleDefeat:
	.string "Spero che Fiammetta ti abbrustolisca!$"

LavaridgeTown_Gym_1F_Text_AxlePostBattle:
	.string "Haaah… Uff…\p"
	.string "Se resti troppo a lungo sepolto nella\n"
	.string "sabbia calda, poi sei uno straccio…$"

LavaridgeTown_Gym_B1F_Text_KeeganIntro:
	.string "Sarai stanco, a questo punto.\n"
	.string "Ti piacerebbe riposare nella sabbia\l"
	.string "calda, vero?\p"
	.string "Ma sappi che non mollare mai è una\n"
	.string "dote fondamentale per ogni\l"
	.string "Allenatore.$"

LavaridgeTown_Gym_B1F_Text_KeeganDefeat:
	.string "Chi gioca col fuoco si scotta…$"

LavaridgeTown_Gym_B1F_Text_KeeganPostBattle:
	.string "Sei bravo davvero…\n"
	.string "Ma Fiammetta, la nostra Capo, è forte.\p"
	.string "Se non stai attento, rischi di\n"
	.string "scottarti sul serio.$"

LavaridgeTown_Gym_1F_Text_GeraldIntro:
	.string "I tuoi Pokémon reggeranno un\n"
	.string "calore di 200 gradi?$"

LavaridgeTown_Gym_1F_Text_GeraldDefeat:
	.string "Non ho bruciato abbastanza…$"

LavaridgeTown_Gym_1F_Text_GeraldPostBattle:
	.string "La temperatura del magma è di\n"
	.string "200 gradi.\p"
	.string "Se i tuoi Pokémon mi hanno battuto,\n"
	.string "nel magma ci starebbero in vacanza.$"

LavaridgeTown_Gym_1F_Text_DanielleIntro:
	.string "Ehm…\n"
	.string "Va bene, lottiamo.$"

LavaridgeTown_Gym_1F_Text_DanielleDefeat:
	.string "Oh, ma sei troppo forte.$"

LavaridgeTown_Gym_1F_Text_DaniellePostBattle:
	.string "Diventerò un'Allenatrice bella e\n"
	.string "forte, proprio come Fiammetta.$"

LavaridgeTown_Gym_B1F_Text_JaceIntro:
	.string "Forza, datti una mossa!\n"
	.string "Lottiamo prima che mi raffreddi!$"

LavaridgeTown_Gym_B1F_Text_JaceDefeat:
	.string "Fa così caldo, eppure ho il cuore\n"
	.string "stretto nel ghiaccio…$"

LavaridgeTown_Gym_B1F_Text_JacePostBattle:
	.string "Con lo spirito di lotta che ti arde\n"
	.string "dentro, forse hai qualche speranza\l"
	.string "contro la nostra Capopalestra.$"

LavaridgeTown_Gym_B1F_Text_JeffIntro:
	.string "Vedi come divampano le fiamme?\n"
	.string "Festeggiano già la mia vittoria!$"

LavaridgeTown_Gym_B1F_Text_JeffDefeat:
	.string "Qualcosa è andato storto.$"

LavaridgeTown_Gym_B1F_Text_JeffPostBattle:
	.string "E allora? Dico: e allora?\n"
	.string "Io cammino a piedi nudi sulle braci!\p"
	.string "…E tu non provarci nemmeno!$"

LavaridgeTown_Gym_B1F_Text_EliIntro:
	.string "Adoro le montagne, ma più di tutto\n"
	.string "adoro i vulcani.$"

LavaridgeTown_Gym_B1F_Text_EliDefeat:
	.string "Ho perso senza mai avere la lotta\n"
	.string "sotto controllo.$"

LavaridgeTown_Gym_B1F_Text_EliPostBattle:
	.string "Resto qui perché sono diventato un\n"
	.string "fan della potenza di Fiammetta.\p"
	.string "Ehehehehe.$"

LavaridgeTown_Gym_1F_Text_FlanneryIntro:
	.string "Benvenuto… No, aspetta.\p"
	.string "Misero Allenatore, che piacere vedere\n"
	.string "che sei giunto fin qui!\p"
	.string "Mi è stato affidato il compito di…\n"
	.string "No, aspetta.\p"
	.string "Io sono Fiammetta, la Capopalestra\n"
	.string "di Cuordilava!\p"
	.string "Ehm…\n"
	.string "Non sottovalutarmi solo perché sono\l"
	.string "Capopalestra da poco!\p"
	.string "Sono nuova, ma il mio fuoco no!\p"
	.string "Con l'arte che mi ha tramandato mio\n"
	.string "nonno, ti mostrerò, ehm… le mosse\l"
	.string "roventi forgiate su questa terra!\p"
	.string "E… ehm… speriamo che il vulcano\n"
	.string "stia buono.$"

LavaridgeTown_Gym_1F_Text_FlanneryDefeat:
	.string "Oh…\n"
	.string "Forse mi sono sforzata troppo…\p"
	.string "Io… sono diventata Capopalestra\n"
	.string "da pochissimo.\p"
	.string "Ho cercato di essere qualcuno\n"
	.string "che non sono.\p"
	.string "Devo essere me stessa, o anche\n"
	.string "i miei Pokémon si confonderanno.\p"
	.string "Grazie per avermelo insegnato.\n"
	.string "Per questo, ti meriti questa.$"

LavaridgeTown_Gym_1F_Text_ReceivedHeatBadge:
	.string "{PLAYER} riceve la Medaglia Fiamma\n"
	.string "da Fiammetta.$"

LavaridgeTown_Gym_1F_Text_ExplainHeatBadgeTakeThis:
	.string "Con la Medaglia Fiamma, tutti i Pokémon\n"
	.string "fino al livello 50, anche quelli avuti\l"
	.string "con uno scambio, ti obbediranno\l"
	.string "senza fare storie.\p"
	.string "Inoltre, i Pokémon potranno usare\n"
	.string "la mossa MN Forza fuori dalla lotta.\p"
	.string "Questo è un segno della mia gratitudine.\n"
	.string "Non fare complimenti!$"

LavaridgeTown_Gym_1F_Text_ExplainOverheat:
	.string "La MT50 contiene Vampata.\p"
	.string "È una mossa che infligge danni\n"
	.string "ingenti all'avversario.\p"
	.string "Però riduce di molto l'Attacco Speciale\n"
	.string "del Pokémon che la usa. Forse non è\l"
	.string "adatta alle lotte lunghe.$"

LavaridgeTown_Gym_1F_Text_RegisteredFlannery:
	.string "Hai registrato la Capopalestra\n"
	.string "Fiammetta nel PokéNav.$"

LavaridgeTown_Gym_1F_Text_FlanneryPostBattle:
	.string "La tua forza mi ricorda qualcuno…\p"
	.string "Oh! Ho capito! Lotti come Norman,\n"
	.string "il Capopalestra di Petalipoli.\p"
	.string "Come? Sei suo figlio?!\n"
	.string "Beh, allora si spiega tutto!$"

LavaridgeTown_Gym_1F_Text_GymStatue:
	.string "Palestra Pokémon di Cuordilava$"

LavaridgeTown_Gym_1F_Text_GymStatueCertified:
	.string "Palestra Pokémon di Cuordilava\p"
	.string "Allenatori certificati da Fiammetta:\n"
	.string "{PLAYER}$"

LavaridgeTown_Gym_1F_Text_FlanneryPreRematch:
	.string "Fiammetta: Perdere una lotta non\n"
	.string "basta a spegnermi.\p"
	.string "Amo i Pokémon.\n"
	.string "Amo lottare.\l"
	.string "E… amo questa Palestra!\p"
	.string "Scambiamoci mosse roventi\n"
	.string "in un'altra lotta!$"

LavaridgeTown_Gym_1F_Text_FlanneryRematchDefeat:
	.string "Uff!\n"
	.string "Sono sull'orlo dell'eruzione!$"

LavaridgeTown_Gym_1F_Text_FlanneryPostRematch:
	.string "Fiammetta: Ho perso, ma sono\n"
	.string "soddisfatta al cento per cento.\p"
	.string "Non capita spesso di godersi una\n"
	.string "lotta così infuocata.\p"
	.string "Rifacciamone un'altra così,\n"
	.string "prima o poi!$"

LavaridgeTown_Gym_1F_Text_FlanneryRematchNeedTwoMons:
	.string "Fiammetta: Perdere una lotta non\n"
	.string "basta a spegnermi.\p"
	.string "Amo i Pokémon.\n"
	.string "Amo lottare.\l"
	.string "E… amo questa Palestra!\p"
	.string "Scambiamoci mosse roventi\n"
	.string "in un'altra lotta!\p"
	.string "Oh, aspetta. Hai solo un Pokémon\n"
	.string "in grado di lottare?\p"
	.string "Mi spiace, ma quando vieni a sfidarmi\n"
	.string "porta almeno due Pokémon.$"
