MauvilleCity_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, MauvilleCity_Gym_OnLoad
	.byte 0

MauvilleCity_Gym_OnLoad:
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_DeactivatePuzzle
	switch VAR_MAUVILLE_GYM_STATE
	case 0, MauvilleCity_Gym_EventScript_UpdateBarriers
	case 1, MauvilleCity_Gym_EventScript_Switch1Pressed
	case 2, MauvilleCity_Gym_EventScript_Switch2Pressed
	case 3, MauvilleCity_Gym_EventScript_Switch3Pressed
	case 4, MauvilleCity_Gym_EventScript_Switch4Pressed
	end

MauvilleCity_Gym_EventScript_UpdateBarriers::
	goto_if_set FLAG_MAUVILLE_GYM_BARRIERS_STATE, MauvilleCity_Gym_EventScript_SetAltBarriers
	end

MauvilleCity_Gym_EventScript_SetAltBarriers::
	setmetatile 3, 11, METATILE_MauvilleGym_RedBeamV1_On, TRUE
	setmetatile 3, 12, METATILE_MauvilleGym_RedBeamV2_On, TRUE
	setmetatile 3, 13, METATILE_MauvilleGym_PoleTop_On, TRUE
	setmetatile 4, 10, METATILE_MauvilleGym_RedBeamH1_On, FALSE
	setmetatile 5, 10, METATILE_MauvilleGym_RedBeamH2_On, FALSE
	setmetatile 4, 11, METATILE_MauvilleGym_RedBeamH3_On, TRUE
	setmetatile 5, 11, METATILE_MauvilleGym_RedBeamH4_On, TRUE
	setmetatile 7, 10, METATILE_MauvilleGym_RedBeamH1_On, FALSE
	setmetatile 8, 10, METATILE_MauvilleGym_RedBeamH2_On, FALSE
	setmetatile 7, 11, METATILE_MauvilleGym_RedBeamH3_On, TRUE
	setmetatile 8, 11, METATILE_MauvilleGym_RedBeamH4_On, TRUE
	setmetatile 4, 13, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 5, 13, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 4, 14, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 5, 14, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	setmetatile 1, 10, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 2, 10, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 1, 11, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 2, 11, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	setmetatile 6, 8, METATILE_MauvilleGym_PoleBottom_On, TRUE
	setmetatile 6, 9, METATILE_MauvilleGym_FloorTile, FALSE
	setmetatile 6, 10, METATILE_MauvilleGym_PoleTop_Off, FALSE
	setmetatile 4, 6, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 5, 6, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 4, 7, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 5, 7, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	end

MauvilleCity_Gym_EventScript_Switch1Pressed::
	setvar VAR_0x8004, 0
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_Switch2Pressed::
	setvar VAR_0x8004, 1
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_Switch3Pressed::
	setvar VAR_0x8004, 2
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_Switch4Pressed::
	setvar VAR_0x8004, 3
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_DeactivatePuzzle::
	special MauvilleGymDeactivatePuzzle
	end

MauvilleCity_Gym_EventScript_Wattson::
	trainerbattle_single TRAINER_WATTSON_1, MauvilleCity_Gym_Text_WattsonIntro, MauvilleCity_Gym_Text_WattsonDefeat, MauvilleCity_Gym_EventScript_WattsonDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_Gym_EventScript_WattsonRematch
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, MauvilleCity_Gym_EventScript_GiveShockWave2
	goto_if_eq VAR_NEW_MAUVILLE_STATE, 2, MauvilleCity_Gym_EventScript_CompletedNewMauville
	msgbox MauvilleCity_Gym_Text_WattsonPostBattle, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_WattsonDefeated::
	message MauvilleCity_Gym_Text_ReceivedDynamoBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox MauvilleCity_Gym_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 3
	clearflag FLAG_HIDE_VERDANTURF_TOWN_SCOTT
	setflag FLAG_DEFEATED_MAUVILLE_GYM
	setflag FLAG_BADGE03_GET
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 3
	call Common_EventScript_SetGymTrainers
	special MauvilleGymDeactivatePuzzle
	special DrawWholeMapView
	playse SE_UNLOCK
	call MauvilleCity_Gym_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MauvilleCity_Gym_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WATTSON_MATCH_CALL
	release
	end

MauvilleCity_Gym_EventScript_GiveShockWave2::
	giveitem ITEM_TM_SHOCK_WAVE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox MauvilleCity_Gym_Text_ExplainShockWave, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_SHOCK_WAVE
	release
	end

MauvilleCity_Gym_EventScript_GiveShockWave::
	giveitem ITEM_TM_SHOCK_WAVE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox MauvilleCity_Gym_Text_ExplainShockWave, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_SHOCK_WAVE
	return

MauvilleCity_Gym_EventScript_CompletedNewMauville::
	msgbox MauvilleCity_Gym_Text_WattsonGoForthAndEndeavor, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_WattsonRematch::
	trainerbattle_rematch_double TRAINER_WATTSON_1, MauvilleCity_Gym_Text_WattsonPreRematch, MauvilleCity_Gym_Text_WattsonRematchDefeat, MauvilleCity_Gym_Text_WattsonRematchNeedTwoMons
	msgbox MauvilleCity_Gym_Text_WattsonPostRematch, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Switch1::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 1, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 1
	setvar VAR_0x8004, 0
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_Switch2::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 2, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 2
	setvar VAR_0x8004, 1
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_Switch3::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 3, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 3
	setvar VAR_0x8004, 2
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_Switch4::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 4, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 4
	setvar VAR_0x8004, 3
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_PressFloorSwitch::
	special MauvilleGymSetDefaultBarriers
	special MauvilleGymPressSwitch
	special DrawWholeMapView
	playse SE_UNLOCK
	goto_if_set FLAG_MAUVILLE_GYM_BARRIERS_STATE, MauvilleCity_Gym_EventScript_ClearBarriersAltState
	goto_if_unset FLAG_MAUVILLE_GYM_BARRIERS_STATE, MauvilleCity_Gym_EventScript_SetBarriersAltState
	releaseall
	end

MauvilleCity_Gym_EventScript_SwitchDoNothing::
	releaseall
	end

MauvilleCity_Gym_EventScript_SetBarriersAltState::
	setflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	releaseall
	end

MauvilleCity_Gym_EventScript_ClearBarriersAltState::
	clearflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	releaseall
	end

MauvilleCity_Gym_EventScript_Kirk::
	trainerbattle_single TRAINER_KIRK, MauvilleCity_Gym_Text_KirkIntro, MauvilleCity_Gym_Text_KirkDefeat
	msgbox MauvilleCity_Gym_Text_KirkPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Shawn::
	trainerbattle_single TRAINER_SHAWN, MauvilleCity_Gym_Text_ShawnIntro, MauvilleCity_Gym_Text_ShawnDefeat
	msgbox MauvilleCity_Gym_Text_ShawnPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Ben::
	trainerbattle_single TRAINER_BEN, MauvilleCity_Gym_Text_BenIntro, MauvilleCity_Gym_Text_BenDefeat
	msgbox MauvilleCity_Gym_Text_BenPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Vivian::
	trainerbattle_single TRAINER_VIVIAN, MauvilleCity_Gym_Text_VivianIntro, MauvilleCity_Gym_Text_VivianDefeat
	msgbox MauvilleCity_Gym_Text_VivianPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Angelo::
	trainerbattle_single TRAINER_ANGELO, MauvilleCity_Gym_Text_AngeloIntro, MauvilleCity_Gym_Text_AngeloDefeat
	msgbox MauvilleCity_Gym_Text_AngeloPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_GymGuidePostVictory
	msgbox MauvilleCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_GymGuidePostVictory::
	msgbox MauvilleCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE03_GET, MauvilleCity_Gym_EventScript_GymStatueCertified
	goto MauvilleCity_Gym_EventScript_GymStatue
	end

MauvilleCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE03_GET, MauvilleCity_Gym_EventScript_GymStatueCertified
	goto MauvilleCity_Gym_EventScript_GymStatue
	end

MauvilleCity_Gym_EventScript_GymStatueCertified::
	msgbox MauvilleCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

MauvilleCity_Gym_EventScript_GymStatue::
	msgbox MauvilleCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

MauvilleCity_Gym_Text_GymGuideAdvice:
	.string "Ehi, come va, {PLAYER}?\n"
	.string "Pronto a diventare Campione?\p"
	.string "Walter, il Capopalestra di Ciclamipoli,\n"
	.string "usa Pokémon di tipo Elettro.\p"
	.string "Se lo sfidi con Pokémon di tipo Acqua,\n"
	.string "te li friggerà! Bzzzt!\p"
	.string "E poi ha riempito la Palestra di porte\n"
	.string "comandate da interruttori! Che tipo!\p"
	.string "Forza, dacci dentro!$"

MauvilleCity_Gym_Text_GymGuidePostVictory:
	.string "Wow, sei proprio elettrizzante!\n"
	.string "Hai mandato in corto pure le porte!$"

MauvilleCity_Gym_Text_KirkIntro:
	.string "La mia anima è elettrica, la tua\n"
	.string "sconfitta è matematica! Yeah-eah!$"

MauvilleCity_Gym_Text_KirkDefeat:
	.string "Che concerto ad alto voltaggio,\n"
	.string "amplificato a palla, amico!$"

MauvilleCity_Gym_Text_KirkPostBattle:
	.string "Pokémon e rock: è tutta una questione\n"
	.string "di cuore, oh-yeah!$"

MauvilleCity_Gym_Text_ShawnIntro:
	.string "Mi ha allenato Walter in persona!\n"
	.string "Non ho nessuna intenzione di perdere!$"

MauvilleCity_Gym_Text_ShawnDefeat:
	.string "Spina staccata… amplificatore spento…$"

MauvilleCity_Gym_Text_ShawnPostBattle:
	.string "Walter, il nostro Capopalestra, è sulla\n"
	.string "piazza da tantissimo tempo.\p"
	.string "Lottava già prima che nascesse tuo\n"
	.string "papà, quel vecchio osso duro!$"

MauvilleCity_Gym_Text_BenIntro:
	.string "Questa Palestra è piena di enigmi!\n"
	.string "Non è uno spasso?$"

MauvilleCity_Gym_Text_BenDefeat:
	.string "Perdere non è per niente divertente…$"

MauvilleCity_Gym_Text_BenPostBattle:
	.string "Walter dice che adora piazzare\n"
	.string "trappoline con gli interruttori.$"

MauvilleCity_Gym_Text_VivianIntro:
	.string "Col mio fascino e le mosse dei miei\n"
	.string "Pokémon, resterai fulminato!$"

MauvilleCity_Gym_Text_VivianDefeat:
	.string "Sono io quella fulminata!$"

MauvilleCity_Gym_Text_VivianPostBattle:
	.string "Si dice che sia stato Walter a fondare\n"
	.string "Ciclamipoli.\p"
	.string "Era un Allenatore molto prima di noi.\n"
	.string "Chissà quanti segreti custodisce…\l"
	.string "magari anche su Ciclanova.$"

MauvilleCity_Gym_Text_AngeloIntro:
	.string "Adoro le cose che luccicano!$"

MauvilleCity_Gym_Text_AngeloDefeat:
	.string "Oh…\n"
	.string "Ho gli occhi abbagliati…$"

MauvilleCity_Gym_Text_AngeloPostBattle:
	.string "Walter, il Capopalestra, ha una fronte\n"
	.string "lucidissima. Mi mette di buonumore!$"

MauvilleCity_Gym_Text_WattsonIntro:
	.string "Ho rinunciato ai miei progetti per\n"
	.string "rinnovare la città, eh già.\p"
	.string "Questa città ha visto cose che non\n"
	.string "immagini, ragazzo… ma non ne parliamo.\p"
	.string "Così ora passo il tempo a costruire\n"
	.string "porte trappola nella mia Palestra.\p"
	.string "Oh? E tu che ci fai qui?\p"
	.string "Come? Dici di aver superato tutte\n"
	.string "le mie porte truccate?\p"
	.string "Uahahahah!\n"
	.string "Questa sì che è bella!\p"
	.string "Allora io, Walter, Capopalestra di\n"
	.string "Ciclamipoli, ti darò la scossa!\l"
	.string "Corrente piena!$"

MauvilleCity_Gym_Text_WattsonDefeat:
	.string "Uahahahah!\n"
	.string "E va bene, ho perso!\p"
	.string "Mi hai fatto prendere una scossa\n"
	.string "di emozione! Ecco la Medaglia!$"

MauvilleCity_Gym_Text_ReceivedDynamoBadge:
	.string "{PLAYER} ha ricevuto la Medaglia\n"
	.string "Dinamo da Walter!$"

MauvilleCity_Gym_Text_ExplainDynamoBadgeTakeThis:
	.string "Con la Medaglia Dinamo i Pokémon\n"
	.string "possono usare Spaccaroccia anche\l"
	.string "fuori dalla lotta.\p"
	.string "E poi rende i tuoi Pokémon un\n"
	.string "pochino più veloci.\p"
	.string "Mmh…\n"
	.string "Prendi anche questa!$"

MauvilleCity_Gym_Text_ExplainShockWave:
	.string "Quella MT34 contiene Ondashock.\p"
	.string "È una mossa affidabile che non manca\n"
	.string "mai il bersaglio! Puoi contarci!\p"
	.string "… … … … … …$"

MauvilleCity_Gym_Text_RegisteredWattson:
	.string "Il Capopalestra Walter è stato\n"
	.string "registrato nel PokéNav.$"

MauvilleCity_Gym_Text_WattsonPostBattle:
	.string "Uahahahah! Vedere un Allenatore così\n"
	.string "promettente mi ricarica le batterie!$"

MauvilleCity_Gym_Text_WattsonGoForthAndEndeavor:
	.string "Uahahahah!\n"
	.string "Grazie a te, Ciclanova dorme tranquilla.\p"
	.string "Ora va', ragazzo, e dacci dentro!$"

MauvilleCity_Gym_Text_GymStatue:
	.string "Palestra Pokémon di Ciclamipoli$"

MauvilleCity_Gym_Text_GymStatueCertified:
	.string "Palestra Pokémon di Ciclamipoli\p"
	.string "Allenatori riconosciuti da Walter:\n"
	.string "{PLAYER}$"

MauvilleCity_Gym_Text_WattsonPreRematch:
	.string "Walter: Ah-ha! Eccoti, finalmente!\n"
	.string "So bene cosa vuoi.\l"
	.string "Vuoi sfidare i miei Pokémon!\p"
	.string "Uahahahah!\p"
	.string "Ti farò sprizzare scintille!\n"
	.string "Non dire che non ti avevo avvisato!$"

MauvilleCity_Gym_Text_WattsonRematchDefeat:
	.string "Uff…\n"
	.string "Le nostre batterie sono a secco…$"

MauvilleCity_Gym_Text_WattsonPostRematch:
	.string "Walter: Dovremo ricaricare di nuovo\n"
	.string "le batterie.\p"
	.string "Quando saremo carichi al massimo,\n"
	.string "accetteremo volentieri la tua sfida.\p"
	.string "Quindi torna a trovarci,\n"
	.string "d'accordo?$"

MauvilleCity_Gym_Text_WattsonRematchNeedTwoMons:
	.string "Walter: Ah-ha! Eccoti, finalmente!\n"
	.string "So bene cosa vuoi.\l"
	.string "Vuoi sfidare i miei Pokémon!\p"
	.string "Uahahahah!\p"
	.string "Ops! Aspetta un attimo!\n"
	.string "Hai un solo Pokémon con te!\p"
	.string "Torna da me con almeno due\n"
	.string "Pokémon, d'accordo?$"

