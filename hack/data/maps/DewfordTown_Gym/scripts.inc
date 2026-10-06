DewfordTown_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DewfordTown_Gym_OnTransition
	.byte 0

DewfordTown_Gym_OnTransition:
	call DewfordTown_Gym_EventScript_SetFlashLevel
	end

DewfordTown_Gym_EventScript_SetFlashLevel::
	goto_if_defeated TRAINER_BRAWLY_1, DewfordTown_Gym_EventScript_SetLightsOn
	call DewfordTown_Gym_EventScript_CountTrainersDefeated
	copyvar VAR_0x8001, VAR_0x8000
	goto_if_eq VAR_0x8000, 0, DewfordTown_Gym_EventScript_SetFlashLevel7
	goto_if_eq VAR_0x8000, 1, DewfordTown_Gym_EventScript_SetFlashLevel6
	goto_if_eq VAR_0x8000, 2, DewfordTown_Gym_EventScript_SetFlashLevel5
	goto_if_eq VAR_0x8000, 3, DewfordTown_Gym_EventScript_SetFlashLevel4
	goto_if_eq VAR_0x8000, 4, DewfordTown_Gym_EventScript_SetFlashLevel3
	goto_if_eq VAR_0x8000, 5, DewfordTown_Gym_EventScript_SetFlashLevel2
	goto DewfordTown_Gym_EventScript_SetFlashLevel1

DewfordTown_Gym_EventScript_SetLightsOn::
	setflashlevel 0
	return

@ Brightest
DewfordTown_Gym_EventScript_SetFlashLevel1::
	setflashlevel 1
	return

DewfordTown_Gym_EventScript_SetFlashLevel2::
	setflashlevel 2
	return

DewfordTown_Gym_EventScript_SetFlashLevel3::
	setflashlevel 3
	return

DewfordTown_Gym_EventScript_SetFlashLevel4::
	setflashlevel 4
	return

DewfordTown_Gym_EventScript_SetFlashLevel5::
	setflashlevel 5
	return

DewfordTown_Gym_EventScript_SetFlashLevel6::
	setflashlevel 6
	return

@ Darkest
DewfordTown_Gym_EventScript_SetFlashLevel7::
	setflashlevel 7
	return

DewfordTown_Gym_EventScript_BrightenRoom::
	call DewfordTown_Gym_EventScript_CountTrainersDefeated
	nop1
	goto_if_eq VAR_0x8000, VAR_0x8001, DewfordTown_Gym_EventScript_NoLightChange
	copyvar VAR_0x8001, VAR_0x8000
	goto_if_eq VAR_0x8000, 1, DewfordTown_Gym_EventScript_AnimateFlash1Trainer
	goto_if_eq VAR_0x8000, 2, DewfordTown_Gym_EventScript_AnimateFlash2Trainers
	goto_if_eq VAR_0x8000, 3, DewfordTown_Gym_EventScript_AnimateFlash3Trainers
	goto_if_eq VAR_0x8000, 4, DewfordTown_Gym_EventScript_AnimateFlash4Trainers
	goto_if_eq VAR_0x8000, 5, DewfordTown_Gym_EventScript_AnimateFlash5Trainers
	goto_if_eq VAR_0x8000, 6, DewfordTown_Gym_EventScript_AnimateFlash6Trainers

DewfordTown_Gym_EventScript_NoLightChange::
	return

@ NOTE: A little confusingly, a larger animateflash param value is a smaller flash radius
DewfordTown_Gym_EventScript_AnimateFlash1Trainer::
	playse SE_SWITCH
	animateflash 6
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash2Trainers::
	playse SE_SWITCH
	animateflash 5
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash3Trainers::
	playse SE_SWITCH
	animateflash 4
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash4Trainers::
	playse SE_SWITCH
	animateflash 3
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash5Trainers::
	playse SE_SWITCH
	animateflash 2
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlash6Trainers::
	playse SE_SWITCH
	animateflash 1
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_AnimateFlashFullBrightness::
	playse SE_SWITCH
	animateflash 0
	call DewfordTown_Gym_EventScript_SetFlashLevel
	return

DewfordTown_Gym_EventScript_CountTrainersDefeated::
	setvar VAR_0x8000, 0
	goto_if_not_defeated TRAINER_TAKAO, DewfordTown_Gym_EventScript_CheckJocelyn
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckJocelyn::
	goto_if_not_defeated TRAINER_JOCELYN, DewfordTown_Gym_EventScript_CheckLaura
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckLaura::
	goto_if_not_defeated TRAINER_LAURA, DewfordTown_Gym_EventScript_CheckBrenden
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckBrenden::
	goto_if_not_defeated TRAINER_BRENDEN, DewfordTown_Gym_EventScript_CheckCristian
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckCristian::
	goto_if_not_defeated TRAINER_CRISTIAN, DewfordTown_Gym_EventScript_CheckLilith
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_CheckLilith::
	goto_if_not_defeated TRAINER_LILITH, DewfordTown_Gym_EventScript_StopCountingTrainers
	addvar VAR_0x8000, 1
DewfordTown_Gym_EventScript_StopCountingTrainers::
	return

DewfordTown_Gym_EventScript_Brawly::
	trainerbattle_single TRAINER_BRAWLY_1, DewfordTown_Gym_Text_BrawlyIntro, DewfordTown_Gym_Text_BrawlyDefeat, DewfordTown_Gym_EventScript_BrawlyDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, DewfordTown_Gym_EventScript_BrawlyRematch
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_BrawlyDefeated::
	call DewfordTown_Gym_EventScript_AnimateFlashFullBrightness
	message DewfordTown_Gym_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox DewfordTown_Gym_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_DEWFORD_GYM
	setflag FLAG_BADGE02_GET
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 2
	call Common_EventScript_SetGymTrainers
	call DewfordTown_Gym_EventScript_GiveBulkUp
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox DewfordTown_Gym_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_BRAWLY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

DewfordTown_Gym_EventScript_GiveBulkUp::
	giveitem ITEM_TM_BULK_UP
	goto_if_eq VAR_RESULT, 0, Common_EventScript_BagIsFull
	msgbox DewfordTown_Gym_Text_ExplainBulkUp, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_BULK_UP
	return

DewfordTown_Gym_EventScript_GiveBulkUp2:
	giveitem ITEM_TM_BULK_UP
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	msgbox DewfordTown_Gym_Text_ExplainBulkUp, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_BULK_UP
	release
	end

DewfordTown_Gym_EventScript_BrawlyRematch::
	trainerbattle_rematch_double TRAINER_BRAWLY_1, DewfordTown_Gym_Text_BrawlyPreRematch, DewfordTown_Gym_Text_BrawlyRematchDefeat, DewfordTown_Gym_Text_BrawlyRematchNeedTwoMons
	msgbox DewfordTown_Gym_Text_BrawlyPostRematch, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_Takao::
	trainerbattle_single TRAINER_TAKAO, DewfordTown_Gym_Text_TakaoIntro, DewfordTown_Gym_Text_TakaoDefeat, DewfordTown_Gym_EventScript_TakaoBrightenRoom
	msgbox DewfordTown_Gym_Text_TakaoPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_TakaoBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Jocelyn::
	trainerbattle_single TRAINER_JOCELYN, DewfordTown_Gym_Text_JocelynIntro, DewfordTown_Gym_Text_JocelynDefeat, DewfordTown_Gym_EventScript_JocelynBrightenRoom
	msgbox DewfordTown_Gym_Text_JocelynPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_JocelynBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Laura::
	trainerbattle_single TRAINER_LAURA, DewfordTown_Gym_Text_LauraIntro, DewfordTown_Gym_Text_LauraDefeat, DewfordTown_Gym_EventScript_LauraBrightenRoom
	msgbox DewfordTown_Gym_Text_LauraPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_LauraBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Brenden::
	trainerbattle_single TRAINER_BRENDEN, DewfordTown_Gym_Text_BrendenIntro, DewfordTown_Gym_Text_BrendenDefeat, DewfordTown_Gym_EventScript_BrendenBrightenRoom
	msgbox DewfordTown_Gym_Text_BrendenPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_BrendenBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Cristian::
	trainerbattle_single TRAINER_CRISTIAN, DewfordTown_Gym_Text_CristianIntro, DewfordTown_Gym_Text_CristianDefeat, DewfordTown_Gym_EventScript_CristianBrightenRoom
	msgbox DewfordTown_Gym_Text_CristianPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_CristianBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_Lilith::
	trainerbattle_single TRAINER_LILITH, DewfordTown_Gym_Text_LilithIntro, DewfordTown_Gym_Text_LilithDefeat, DewfordTown_Gym_EventScript_LilithBrightenRoom
	msgbox DewfordTown_Gym_Text_LilithPostBattle, MSGBOX_AUTOCLOSE
	end

DewfordTown_Gym_EventScript_LilithBrightenRoom::
	call DewfordTown_Gym_EventScript_BrightenRoom
	release
	end

DewfordTown_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_DEWFORD_GYM, DewfordTown_Gym_EventScript_GymGuidePostVictory
	msgbox DewfordTown_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_GymGuidePostVictory::
	msgbox DewfordTown_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

DewfordTown_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, DewfordTown_Gym_EventScript_GymStatueCertified
	goto DewfordTown_Gym_EventScript_GymStatue
	end

DewfordTown_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, DewfordTown_Gym_EventScript_GymStatueCertified
	goto DewfordTown_Gym_EventScript_GymStatue
	end

DewfordTown_Gym_EventScript_GymStatueCertified::
	msgbox DewfordTown_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Gym_EventScript_GymStatue::
	msgbox DewfordTown_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

DewfordTown_Gym_Text_GymGuideAdvice:
	.string "Ehi, come va, futuro Campione\n"
	.string "{PLAYER}?\p"
	.string "Rudi, il Capopalestra di Bluruvia,\n"
	.string "usa Pokémon di tipo Lotta.\p"
	.string "Sfidarlo con Pokémon di tipo Normale\n"
	.string "è come andarsele a cercare.\p"
	.string "Le mosse Lotta picchiano durissimo\n"
	.string "sui Pokémon di tipo Normale,\l"
	.string "quindi occhio!\p"
	.string "E come se non bastasse, la Palestra è\n"
	.string "buia come il fondo dell'oceano.\p"
	.string "Rudi dice che si allena al buio da\n"
	.string "quando ha visto un Varco aprirsi\l"
	.string "sotto un'onda. Roba da brividi!\p"
	.string "Ma ogni Allenatore che sconfiggi\n"
	.string "riaccende un po' di luce.\p"
	.string "Forza, dacci dentro!$"

DewfordTown_Gym_Text_GymGuidePostVictory:
	.string "Uau! Qui dentro adesso c'è fin troppa\n"
	.string "luce!\p"
	.string "Ma il tuo futuro è ancora più radioso!$"

DewfordTown_Gym_Text_TakaoIntro:
	.string "La forza è tutto!\n"
	.string "Fatti sotto!$"

DewfordTown_Gym_Text_TakaoDefeat:
	.string "Non ho abbastanza forza…$"

DewfordTown_Gym_Text_TakaoPostBattle:
	.string "La tua tecnica ha battuto i miei\n"
	.string "muscoli! Tanto di cappello!$"

DewfordTown_Gym_Text_JocelynIntro:
	.string "Non serve scomodare Rudi\n"
	.string "per uno come te!\p"
	.string "Ti stendo io!$"

DewfordTown_Gym_Text_JocelynDefeat:
	.string "Ma… come?\n"
	.string "Come ho fatto a perdere così?$"

DewfordTown_Gym_Text_JocelynPostBattle:
	.string "Non sprecherò questa sconfitta.\n"
	.string "Mi servirà da carica per allenarmi!\p"
	.string "Un giorno diventerò Capopalestra…\p"
	.string "No, diventerò Campionessa della\n"
	.string "Lega Pokémon!$"

DewfordTown_Gym_Text_LauraIntro:
	.string "Non provare a snobbarmi!\n"
	.string "Se poi piangi, non è colpa mia!$"

DewfordTown_Gym_Text_LauraDefeat:
	.string "Uaaaaaaah!\n"
	.string "Ho perso!$"

DewfordTown_Gym_Text_LauraPostBattle:
	.string "Lotto al buio per rendere più forte\n"
	.string "il mio cuore.\p"
	.string "Però, con tutto questo buio, non\n"
	.string "riesco mai a vedere Rudi in faccia…$"

DewfordTown_Gym_Text_LilithIntro:
	.string "Ufufu…\n"
	.string "Brancoli nel buio, eh?$"

DewfordTown_Gym_Text_LilithDefeat:
	.string "Ah, tu…\n"
	.string "Non voglio più vederti!$"

DewfordTown_Gym_Text_LilithPostBattle:
	.string "Meriteresti di perderti al buio e di\n"
	.string "ritrovarti di nuovo all'ingresso!$"

DewfordTown_Gym_Text_BrendenIntro:
	.string "Ti faccio vedere io la grinta\n"
	.string "di un vero lupo di mare!$"

DewfordTown_Gym_Text_BrendenDefeat:
	.string "Com'è potuto succedere?\n"
	.string "Non è da me affondare così!$"

DewfordTown_Gym_Text_BrendenPostBattle:
	.string "Per mille cavalloni! Sei degno di\n"
	.string "incontrare il nostro Capopalestra.$"

DewfordTown_Gym_Text_CristianIntro:
	.string "Se vuoi passare, prima dovrai\n"
	.string "vedertela con me!$"

DewfordTown_Gym_Text_CristianDefeat:
	.string "Grrrrr!\n"
	.string "Mi hai schiacciato!$"

DewfordTown_Gym_Text_CristianPostBattle:
	.string "Hai vinto tu!\n"
	.string "Vai a prenderti quella Medaglia!$"

DewfordTown_Gym_Text_BrawlyIntro:
	.string "Sono Rudi,\n"
	.string "il Capopalestra di Bluruvia!\p"
	.string "Sono cresciuto tra le onde agitate\n"
	.string "di questi mari e mi sono temprato\l"
	.string "nel buio pesto della grotta!\p"
	.string "Le onde vanno e vengono, amico.\n"
	.string "Anche i Varchi: una volta ne ho visto\l"
	.string "uno aprirsi proprio sotto la tavola!\p"
	.string "Vuoi sfidarmi? Allora cavalca l'onda\n"
	.string "e fammi vedere di che pasta sei!$"

DewfordTown_Gym_Text_BrawlyDefeat:
	.string "Uau, wow! Hai sollevato un'onda\n"
	.string "molto più alta del previsto!\p"
	.string "Mi hai travolto!\p"
	.string "Va bene, hai vinto tu.\n"
	.string "Prendi questa Medaglia!$"

DewfordTown_Gym_Text_ReceivedKnuckleBadge:
	.string "{PLAYER} ha ricevuto\n"
	.string "la Medaglia Pugno da Rudi!$"

DewfordTown_Gym_Text_KnuckleBadgeInfoTakeThis:
	.string "Con la Medaglia Pugno tutti i Pokémon\n"
	.string "fino al livello 30, anche quelli\l"
	.string "ottenuti con gli scambi, ti\l"
	.string "obbediranno senza fiatare.\p"
	.string "In più, potrai usare la mossa MN\n"
	.string "Flash anche fuori dalla lotta.\p"
	.string "E se ti va, prendi anche questa\n"
	.string "Macchina Tecnica.$"

DewfordTown_Gym_Text_ExplainBulkUp:
	.string "La MT08 contiene Granfisico.\p"
	.string "È una mossa che aumenta sia l'Attacco\n"
	.string "sia la Difesa.\p"
	.string "… … … … … …$"

DewfordTown_Gym_Text_RegisteredBrawly:
	.string "Hai registrato il Capopalestra Rudi\n"
	.string "nel PokéNav.$"

DewfordTown_Gym_Text_BrawlyPostBattle:
	.string "Vedo il tuo talento crescere come\n"
	.string "un'onda gigante che un giorno lascerà\l"
	.string "a bocca aperta tutti gli Allenatori!\p"
	.string "Continua a cavalcarla, amico!$"

DewfordTown_Gym_Text_GymStatue:
	.string "Palestra Pokémon di Bluruvia$"

DewfordTown_Gym_Text_GymStatueCertified:
	.string "Palestra Pokémon di Bluruvia\p"
	.string "Allenatori approvati da Rudi:\n"
	.string "{PLAYER}$"

DewfordTown_Gym_Text_BrawlyPreRematch:
	.string "Rudi: Un'onda può ritirarsi, ma torna\n"
	.string "sempre a riva.\p"
	.string "E un'onda di talento come te…\n"
	.string "Sapevo che saresti tornato!\p"
	.string "Fammi vedere quanto in alto\n"
	.string "sei arrivato!$"

DewfordTown_Gym_Text_BrawlyRematchDefeat:
	.string "Wow!\n"
	.string "Travolto di nuovo!$"

DewfordTown_Gym_Text_BrawlyPostRematch:
	.string "Rudi: La lotta è vasta e profonda.\n"
	.string "Non se ne vede mai il fondo.\l"
	.string "Proprio come il mare di Hoenn!$"

DewfordTown_Gym_Text_BrawlyRematchNeedTwoMons:
	.string "Rudi: Un'onda può ritirarsi, ma torna\n"
	.string "sempre a riva.\p"
	.string "E un'onda di talento come te…\n"
	.string "Sapevo che saresti tornato!\p"
	.string "Oh? Hai un solo Pokémon in grado\n"
	.string "di lottare.\p"
	.string "Mi spiace, ma torna quando avrai\n"
	.string "almeno due Pokémon.$"
