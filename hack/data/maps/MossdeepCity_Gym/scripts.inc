MossdeepCity_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MossdeepCity_Gym_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, MossdeepCity_Gym_OnLoad
	.byte 0

@ NOTE: Mossdeep Gym was redesigned between R/S and E. Leftover (and now functionally unused) scripts are commented below

MossdeepCity_Gym_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER @ Hide follower since it can collide with moving statues
	end

@ All the below checks are leftover from RS. FLAG_MOSSDEEP_GYM_SWITCH_X is never set
MossdeepCity_Gym_OnLoad:
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_1, MossdeepCity_Gym_EventScript_SetSwitch1Metatiles
	goto MossdeepCity_Gym_EventScript_CheckSwitch2
	end

MossdeepCity_Gym_EventScript_CheckSwitch2::
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_2, MossdeepCity_Gym_EventScript_SetSwitch2Metatiles
	goto MossdeepCity_Gym_EventScript_CheckSwitch3
	end

MossdeepCity_Gym_EventScript_CheckSwitch3::
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_3, MossdeepCity_Gym_EventScript_SetSwitch3Metatiles
	goto MossdeepCity_Gym_EventScript_CheckSwitch4
	end

MossdeepCity_Gym_EventScript_CheckSwitch4::
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_4, MossdeepCity_Gym_EventScript_SetSwitch4Metatiles
	end

@ All the below set metatile scripts are leftover from RS and are functionally unused
MossdeepCity_Gym_EventScript_SetSwitch1Metatiles::
	setmetatile 5, 5, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 2, 7, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_CheckSwitch2
	end

MossdeepCity_Gym_EventScript_SetSwitch2Metatiles::
	setmetatile 8, 14, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 8, 10, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_CheckSwitch3
	end

MossdeepCity_Gym_EventScript_SetSwitch3Metatiles::
	setmetatile 15, 17, METATILE_RSMossdeepGym_RedArrow_Left, FALSE
	setmetatile 17, 15, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_CheckSwitch4
	end

MossdeepCity_Gym_EventScript_SetSwitch4Metatiles::
	setmetatile 1, 23, METATILE_RSMossdeepGym_RedArrow_Up, FALSE
	setmetatile 5, 24, METATILE_RSMossdeepGym_Switch_Down, TRUE
	end

MossdeepCity_Gym_EventScript_TateAndLiza::
	trainerbattle_double TRAINER_TATE_AND_LIZA_1, MossdeepCity_Gym_Text_TateAndLizaIntro, MossdeepCity_Gym_Text_TateAndLizaDefeat, MossdeepCity_Gym_Text_TateAndLizaNeedTwoMons, MossdeepCity_Gym_EventScript_TateAndLizaDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MossdeepCity_Gym_EventScript_TateAndLizaRematch
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, MossdeepCity_Gym_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

MossdeepCity_Gym_EventScript_TateAndLizaDefeated::
	message MossdeepCity_Gym_Text_ReceivedMindBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox MossdeepCity_Gym_Text_ExplainMindBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_MOSSDEEP_GYM
	setflag FLAG_BADGE07_GET
	setflag FLAG_HIDE_AQUA_HIDEOUT_GRUNTS
	clearflag FLAG_HIDE_SLATEPORT_CITY_STERNS_SHIPYARD_MR_BRINEY
	clearflag FLAG_UNKNOWN_0x393
	setvar VAR_0x8008, 7
	call Common_EventScript_SetGymTrainers
	clearflag FLAG_HIDE_SLATEPORT_CITY_HARBOR_PATRONS
	clearflag FLAG_HIDE_MOSSDEEP_CITY_TEAM_MAGMA
	clearflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_1F_TEAM_MAGMA
	clearflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_2F_TEAM_MAGMA
	clearflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_2F_STEVEN
	setvar VAR_MOSSDEEP_CITY_STATE, 1
	setflag FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_1F_STEVEN
	setvar VAR_MOSSDEEP_SPACE_CENTER_STATE, 1
	call MossdeepCity_Gym_EventScript_GiveCalmMind
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MossdeepCity_Gym_Text_RegisteredTateAndLiza, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL
	release
	end

MossdeepCity_Gym_EventScript_GiveCalmMind2::
	giveitem ITEM_TM_CALM_MIND
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox MossdeepCity_Gym_Text_ExplainCalmMind, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_CALM_MIND
	release
	end

MossdeepCity_Gym_EventScript_GiveCalmMind::
	giveitem ITEM_TM_CALM_MIND
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox MossdeepCity_Gym_Text_ExplainCalmMind, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_CALM_MIND
	return

MossdeepCity_Gym_EventScript_TateAndLizaRematch::
	trainerbattle_rematch_double TRAINER_TATE_AND_LIZA_1, MossdeepCity_Gym_Text_TateAndLizaPreRematch, MossdeepCity_Gym_Text_TateAndLizaRematchDefeat, MossdeepCity_Gym_Text_TateAndLizaRematchNeedTwoMons
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostRematch, MSGBOX_AUTOCLOSE
	end

@ All the below switch scripts are leftover from RS and are functionally unused
MossdeepCity_Gym_EventScript_Switch1::
	lockall
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_1, MossdeepCity_Gym_EventScript_ClearSwitch1
	setflag FLAG_MOSSDEEP_GYM_SWITCH_1
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 5, 5, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 2, 7, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed::
	special DrawWholeMapView
	playse SE_CLICK
	releaseall
	end

MossdeepCity_Gym_EventScript_ClearSwitch1::
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_1
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 5, 5, METATILE_RSMossdeepGym_RedArrow_Left, FALSE
	setmetatile 2, 7, METATILE_RSMossdeepGym_Switch_Up, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_Switch2::
	lockall
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_2, MossdeepCity_Gym_EventScript_ClearSwitch2
	setflag FLAG_MOSSDEEP_GYM_SWITCH_2
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 8, 14, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 8, 10, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_ClearSwitch2::
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_2
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 8, 14, METATILE_RSMossdeepGym_RedArrow_Down, FALSE
	setmetatile 8, 10, METATILE_RSMossdeepGym_Switch_Up, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_Switch3::
	lockall
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_3, MossdeepCity_Gym_EventScript_ClearSwitch3
	setflag FLAG_MOSSDEEP_GYM_SWITCH_3
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 15, 17, METATILE_RSMossdeepGym_RedArrow_Left, FALSE
	setmetatile 17, 15, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_ClearSwitch3::
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_3
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 15, 17, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 17, 15, METATILE_RSMossdeepGym_Switch_Up, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_Switch4::
	lockall
	goto_if_set FLAG_MOSSDEEP_GYM_SWITCH_4, MossdeepCity_Gym_EventScript_ClearSwitch4
	setflag FLAG_MOSSDEEP_GYM_SWITCH_4
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 1, 23, METATILE_RSMossdeepGym_RedArrow_Up, FALSE
	setmetatile 5, 24, METATILE_RSMossdeepGym_Switch_Down, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_ClearSwitch4::
	clearflag FLAG_MOSSDEEP_GYM_SWITCH_4
	applymovement LOCALID_PLAYER, MossdeepCity_Gym_Movement_WaitAfterSwitchUse
	waitmovement 0
	setmetatile 1, 23, METATILE_RSMossdeepGym_RedArrow_Right, FALSE
	setmetatile 5, 24, METATILE_RSMossdeepGym_Switch_Up, TRUE
	goto MossdeepCity_Gym_EventScript_DrawMapAfterSwitchUsed
	end

MossdeepCity_Gym_EventScript_WarpToEntrance::
	lockall
	warpmossdeepgym MAP_MOSSDEEP_CITY_GYM, 7, 30
	waitstate
	releaseall
	end

MossdeepCity_Gym_EventScript_Preston::
	trainerbattle_single TRAINER_PRESTON, MossdeepCity_Gym_Text_PrestonIntro, MossdeepCity_Gym_Text_PrestonDefeat
	msgbox MossdeepCity_Gym_Text_PrestonPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Virgil::
	trainerbattle_single TRAINER_VIRGIL, MossdeepCity_Gym_Text_VirgilIntro, MossdeepCity_Gym_Text_VirgilDefeat
	msgbox MossdeepCity_Gym_Text_VirgilPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Blake::
	trainerbattle_single TRAINER_BLAKE, MossdeepCity_Gym_Text_BlakeIntro, MossdeepCity_Gym_Text_BlakeDefeat
	msgbox MossdeepCity_Gym_Text_BlakePostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Hannah::
	trainerbattle_single TRAINER_HANNAH, MossdeepCity_Gym_Text_HannahIntro, MossdeepCity_Gym_Text_HannahDefeat
	msgbox MossdeepCity_Gym_Text_HannahPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Samantha::
	trainerbattle_single TRAINER_SAMANTHA, MossdeepCity_Gym_Text_SamanthaIntro, MossdeepCity_Gym_Text_SamanthaDefeat
	msgbox MossdeepCity_Gym_Text_SamanthaPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Maura::
	trainerbattle_single TRAINER_MAURA, MossdeepCity_Gym_Text_MauraIntro, MossdeepCity_Gym_Text_MauraDefeat
	msgbox MossdeepCity_Gym_Text_MauraPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Sylvia::
	trainerbattle_single TRAINER_SYLVIA, MossdeepCity_Gym_Text_SylviaIntro, MossdeepCity_Gym_Text_SylviaDefeat
	msgbox MossdeepCity_Gym_Text_SylviaPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Nate::
	trainerbattle_single TRAINER_NATE, MossdeepCity_Gym_Text_NateIntro, MossdeepCity_Gym_Text_NateDefeat
	msgbox MossdeepCity_Gym_Text_NatePostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Macey::
	trainerbattle_single TRAINER_MACEY, MossdeepCity_Gym_Text_MaceyIntro, MossdeepCity_Gym_Text_MaceyDefeat
	msgbox MossdeepCity_Gym_Text_MaceyPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Clifford::
	trainerbattle_single TRAINER_CLIFFORD, MossdeepCity_Gym_Text_CliffordIntro, MossdeepCity_Gym_Text_CliffordDefeat
	msgbox MossdeepCity_Gym_Text_CliffordPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Nicholas::
	trainerbattle_single TRAINER_NICHOLAS, MossdeepCity_Gym_Text_NicholasIntro, MossdeepCity_Gym_Text_NicholasDefeat
	msgbox MossdeepCity_Gym_Text_NicholasPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_Kathleen::
	trainerbattle_single TRAINER_KATHLEEN, MossdeepCity_Gym_Text_KathleenIntro, MossdeepCity_Gym_Text_KathleenDefeat
	msgbox MossdeepCity_Gym_Text_KathleenPostBattle, MSGBOX_AUTOCLOSE
	end

MossdeepCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_MOSSDEEP_GYM, MossdeepCity_Gym_EventScript_GymGuidePostVictory
	msgbox MossdeepCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

MossdeepCity_Gym_EventScript_GymGuidePostVictory::
	msgbox MossdeepCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

@ Leftover from RS, functionally unused
MossdeepCity_Gym_Movement_WaitAfterSwitchUse:
	delay_16
	delay_16
	step_end

MossdeepCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE07_GET, MossdeepCity_Gym_EventScript_GymStatueCertified
	goto MossdeepCity_Gym_EventScript_GymStatue
	end

MossdeepCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE07_GET, MossdeepCity_Gym_EventScript_GymStatueCertified
	goto MossdeepCity_Gym_EventScript_GymStatue
	end

MossdeepCity_Gym_EventScript_GymStatueCertified::
	msgbox MossdeepCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

MossdeepCity_Gym_EventScript_GymStatue::
	msgbox MossdeepCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

MossdeepCity_Gym_EventScript_YellowFloorSwitch::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle FALSE
	moverotatingtileobjects 0
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

MossdeepCity_Gym_EventScript_BlueFloorSwitch::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle FALSE
	moverotatingtileobjects 1
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

MossdeepCity_Gym_EventScript_GreenFloorSwitch::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle FALSE
	moverotatingtileobjects 2
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

MossdeepCity_Gym_EventScript_PurpleFloorSwitch::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle FALSE
	moverotatingtileobjects 3
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

MossdeepCity_Gym_EventScript_RedFloorSwitch::
	playse SE_SWITCH
	waitse
	playse SE_REPEL
	initrotatingtilepuzzle FALSE
	moverotatingtileobjects 4
	waitmovement 0
	turnrotatingtileobjects
	waitmovement 0
	freerotatingtilepuzzle
	end

MossdeepCity_Gym_Text_GymGuideAdvice:
	.string "Ehi, come va, futuro Campione\n"
	.string "{PLAYER}?\p"
	.string "I Capipalestra di qui usano\n"
	.string "Pokémon di tipo Psico.\p"
	.string "Se li affronti con Pokémon di tipo\n"
	.string "Lotta o Veleno, ti faranno a pezzi!\p"
	.string "E come se non bastasse, attaccano\n"
	.string "con combinazioni micidiali.\p"
	.string "Dicono che i due gemelli sentano\n"
	.string "i Varchi prima ancora che si aprano.\l"
	.string "Brrr, roba da brividi!\p"
	.string "Per vincere, devi dimostrare quanto\n"
	.string "sei affiatato con i tuoi Pokémon.\l"
	.string "Dacci dentro!$"

MossdeepCity_Gym_Text_GymGuidePostVictory:
	.string "Uau, sei sbalorditivo!\n"
	.string "Sei un Allenatore coi fiocchi!$"

MossdeepCity_Gym_Text_PrestonIntro:
	.string "Le lotte si vincono con la forza dello\n"
	.string "spirito! Chi ha lo spirito debole, perde!$"

MossdeepCity_Gym_Text_PrestonDefeat:
	.string "Ho perso!\n"
	.string "Devo temprare di nuovo lo spirito!$"

MossdeepCity_Gym_Text_PrestonPostBattle:
	.string "Chi esita, perde.\n"
	.string "Prendilo come un avvertimento.$"

MossdeepCity_Gym_Text_VirgilIntro:
	.string "Fammi vedere il tuo talento!$"

MossdeepCity_Gym_Text_VirgilDefeat:
	.string "Hai un talento spettacolare!$"

MossdeepCity_Gym_Text_VirgilPostBattle:
	.string "Se al mondo esistono dei prodigi,\n"
	.string "sono i nostri Capipalestra!\p"
	.string "Però tu potresti avere ancora più\n"
	.string "talento di loro…$"

MossdeepCity_Gym_Text_BlakeIntro:
	.string "Fufufufu… Guarda come faccio levitare\n"
	.string "una Poké Ball col pensiero!\p"
	.string "Nnnnnnnnnnnnngh!\n"
	.string "… … … … … … …\p"
	.string "A volte perdo la sintonia…$"

MossdeepCity_Gym_Text_BlakeDefeat:
	.string "Anche in lotta ho perso\n"
	.string "la sintonia!$"

MossdeepCity_Gym_Text_BlakePostBattle:
	.string "Una Poké Ball era troppo pesante da\n"
	.string "sollevare col pensiero. Ma questo\l"
	.string "batuffolo di polvere…\p"
	.string "Fiuuuuuuuuuuuuuuuh!\n"
	.string "… … … … … …\p"
	.string "No, non sto barando!\n"
	.string "Non ci ho soffiato sopra! Giuro!$"

MossdeepCity_Gym_Text_HannahIntro:
	.string "Quando perderai contro di me, non\n"
	.string "prendertela troppo.\p"
	.string "Non è che tu sia debole… Sono io\n"
	.string "che sono troppo forte!$"

MossdeepCity_Gym_Text_HannahDefeat:
	.string "Sei tu a essere troppo forte!$"

MossdeepCity_Gym_Text_HannahPostBattle:
	.string "Non mi butterò giù per una sconfitta.\n"
	.string "Devo solo allenarmi di più.\p"
	.string "Pensare positivo: è questo il mio\n"
	.string "punto di forza!$"

MossdeepCity_Gym_Text_SamanthaIntro:
	.string "Lo vedo…\n"
	.string "Vedo il tuo futuro…\p"
	.string "Se riuscirai a battermi, ti rivelerò\n"
	.string "che cosa ti aspetta!$"

MossdeepCity_Gym_Text_SamanthaDefeat:
	.string "Mi arrendo!$"

MossdeepCity_Gym_Text_SamanthaPostBattle:
	.string "Affronterai una lotta intensissima\n"
	.string "contro i nostri Capipalestra…\p"
	.string "E l'esito…\n"
	.string "Non lo saprai da me!\p"
	.string "Vedo anche una porta di luce nel tuo\n"
	.string "futuro… Ma questa è un'altra storia.$"

MossdeepCity_Gym_Text_MauraIntro:
	.string "Le strade che hai percorso…\n"
	.string "Ciò che hai vissuto…\p"
	.string "Affrontami con tutto quello che\n"
	.string "hai imparato!$"

MossdeepCity_Gym_Text_MauraDefeat:
	.string "La tua è una strada di grandezza!$"

MossdeepCity_Gym_Text_MauraPostBattle:
	.string "Un futuro radioso attende chi si è\n"
	.string "impegnato con costanza.\p"
	.string "Per chi ha oziato, alla fine resta\n"
	.string "solo la disperazione.\p"
	.string "Si raccoglie ciò che si semina…$"

MossdeepCity_Gym_Text_SylviaIntro:
	.string "Vincerò questa lotta, dovessi\n"
	.string "rimetterci l'anima!$"

MossdeepCity_Gym_Text_SylviaDefeat:
	.string "Quello che fai…\n"
	.string "Mi fa rabbrividire…$"

MossdeepCity_Gym_Text_SylviaPostBattle:
	.string "Come osi battermi…\p"
	.string "Se ti capiterà qualcosa di orribile,\n"
	.string "non sarà colpa mia…$"

MossdeepCity_Gym_Text_NateIntro:
	.string "Hohoho.\p"
	.string "Lascia che ti mostri quanto sa essere\n"
	.string "tenace il tipo Psico.$"

MossdeepCity_Gym_Text_NateDefeat:
	.string "Oh! Ma guarda un po'!\n"
	.string "Il tuo stile di lotta è fantastico!$"

MossdeepCity_Gym_Text_NatePostBattle:
	.string "Io… io non mi lascio s-scuotere da\n"
	.string "una sconfitta, p-per quanto dura.$"

MossdeepCity_Gym_Text_KathleenIntro:
	.string "Lascia che ti faccia passare un\n"
	.string "momento davvero spaventoso!$"

MossdeepCity_Gym_Text_KathleenDefeat:
	.string "N-nooooo!$"

MossdeepCity_Gym_Text_KathleenPostBattle:
	.string "La tua vitalità è contagiosa!\n"
	.string "Stammi lontano, presto!$"

MossdeepCity_Gym_Text_CliffordIntro:
	.string "Sarò anche avanti con gli anni, ma ti\n"
	.string "sconsiglio di sottovalutarmi.$"

MossdeepCity_Gym_Text_CliffordDefeat:
	.string "Ah, trabocchi della forza\n"
	.string "della gioventù!$"

MossdeepCity_Gym_Text_CliffordPostBattle:
	.string "Pare che non sia riuscito a tenere\n"
	.string "testa alla tua energia giovanile.$"

MossdeepCity_Gym_Text_MaceyIntro:
	.string "Non arriverai in fondo alla Palestra!\n"
	.string "Non finché ci sono io!$"

MossdeepCity_Gym_Text_MaceyDefeat:
	.string "Come fai a essere così\n"
	.string "assurdamente forte?$"

MossdeepCity_Gym_Text_MaceyPostBattle:
	.string "Uffa! Sarai anche forte, ma non sei\n"
	.string "proprio portato per il tipo Psico!\p"
	.string "Il tuo modo di lottare è… brutale!$"

MossdeepCity_Gym_Text_NicholasIntro:
	.string "Uoooah! Assaggia il super potere\n"
	.string "lottatore dei miei Pokémon!$"

MossdeepCity_Gym_Text_NicholasDefeat:
	.string "Oh!\n"
	.string "Sono finito!$"

MossdeepCity_Gym_Text_NicholasPostBattle:
	.string "Bene! Prossimo obiettivo: un super\n"
	.string "potere Pokémon invincibile.\p"
	.string "Magari me lo insegna un Pokémon\n"
	.string "arrivato da un Varco!$"

MossdeepCity_Gym_Text_TateAndLizaIntro:
	.string "Tell: Hehehe… Sei sorpreso?\p"
	.string "Pat: Fufufu… Sei sorpreso?\p"
	.string "Tell: Che i Capipalestra siano due?\n"
	.string "Pat: Che i Capipalestra siano due?\p"
	.string "Tell: Siamo gemelli!\n"
	.string "Pat: Siamo gemelli!\p"
	.string "Tell: Non ci serve parlare, perché…\n"
	.string "Pat: …ognuno di noi sa cosa…\p"
	.string "Tell: …sta pensando l'altro…\n"
	.string "Pat: …direttamente nella mente!\p"
	.string "Tell: Questa nostra combinazione…\n"
	.string "Pat: …riuscirai a batterla?$"

MossdeepCity_Gym_Text_TateAndLizaDefeat:
	.string "Tell: Cosa?! La nostra intesa…\n"
	.string "Pat: …è andata in frantumi!\p"
	.string "Tell: Non c'è niente da fare. Hai vinto…\n"
	.string "Pat: …quindi, in segno di stima, tieni.$"

MossdeepCity_Gym_Text_ReceivedMindBadge:
	.string "{PLAYER} riceve la Medaglia Mente\n"
	.string "da Tell e Pat.$"

MossdeepCity_Gym_Text_ExplainMindBadgeTakeThis:
	.string "Tell: La Medaglia Mente aumenta\n"
	.string "l'Att. Sp. e la Dif. Sp. dei Pokémon.\p"
	.string "Pat: E ti permette di usare la MN Sub\n"
	.string "anche fuori dalla lotta.\p"
	.string "Tell: Prendi anche questa.$"

MossdeepCity_Gym_Text_ExplainCalmMind:
	.string "Tell: Quella MT04 contiene…\n"
	.string "Pat: …Calmamente!\p"
	.string "Tell: Aumenta l'Attacco Speciale…\n"
	.string "Pat: …e la Difesa Speciale!\p"
	.string "Tell: È la mossa perfetta…\n"
	.string "Pat: …per i Pokémon di tipo Psico!\p"
	.string "… … … … … …\p"
	.string "Tell: Lo senti anche tu? Un'eco…\n"
	.string "Pat: …che viene dall'alto.\p"
	.string "Tell: La Cicatrice nel cielo si sta…\n"
	.string "Pat: …aprendo.$"

MossdeepCity_Gym_Text_RegisteredTateAndLiza:
	.string "I Capipalestra Tell e Pat sono\n"
	.string "stati registrati nel PokéNav.$"

MossdeepCity_Gym_Text_TateAndLizaPostBattle:
	.string "Tell: A quanto pare, il legame tra te\n"
	.string "e i tuoi Pokémon è molto più forte\l"
	.string "di quello che unisce noi gemelli.\p"
	.string "Pat: Diventerai ancora più forte!\n"
	.string "Abbiamo lottato con te, lo sappiamo.\p"
	.string "Tell: E ne avrai bisogno…\n"
	.string "Pat: …l'abbiamo sognato.$"

MossdeepCity_Gym_Text_TateAndLizaNeedTwoMons:
	.string "Tell: Hehehe… Sei sorpreso?\p"
	.string "Pat: Che i Capipalestra siano due?\p"
	.string "Tell: Ops, hai soltanto un…\n"
	.string "Pat: …Pokémon in grado di lottare.\p"
	.string "Tell: Così non possiamo lottare!\p"
	.string "Pat: Se vuoi sfidarci, torna con\n"
	.string "qualche Pokémon in più.$"

MossdeepCity_Gym_Text_GymStatue:
	.string "Palestra Pokémon di Verdeazzupoli$"

MossdeepCity_Gym_Text_GymStatueCertified:
	.string "Palestra Pokémon di Verdeazzupoli\p"
	.string "Allenatori certificati da Tell e Pat:\n"
	.string "{PLAYER}$"

MossdeepCity_Gym_Text_TateAndLizaPreRematch:
	.string "Tell: Pokémon…\n"
	.string "Pat: Pokémon…\p"
	.string "Tell: Se cambia la squadra…\n"
	.string "Pat: …cambia lo stile di lotta.\p"
	.string "Tell: Ma io…\n"
	.string "Pat: Ma io…\p"
	.string "Tell: Starò sempre con Pat!\n"
	.string "Pat: Starò sempre con Tell!\p"
	.string "Tell: La nostra combinazione…\n"
	.string "Pat: …te la mostreremo ancora e ancora!$"

MossdeepCity_Gym_Text_TateAndLizaRematchDefeat:
	.string "Tell: Tu e i tuoi Pokémon…\n"
	.string "Pat: …sembrate fratelli!$"

MossdeepCity_Gym_Text_TateAndLizaPostRematch:
	.string "Tell: Puoi metterci al tappeto,\n"
	.string "ma non ci resteremo mai!\p"
	.string "Pat: Perché noi due ci sosteniamo\n"
	.string "sempre a vicenda!$"

MossdeepCity_Gym_Text_TateAndLizaRematchNeedTwoMons:
	.string "Tell: Sei tornato…\n"
	.string "Pat: Sei tornato…\p"
	.string "Tell: …per la rivincita.\n"
	.string "Pat: …per la rivincita.\p"
	.string "Tell: Però…\n"
	.string "Pat: …per la nostra sfida…\p"
	.string "Tell: Hai portato un solo Pokémon!\n"
	.string "Pat: Hai portato un solo Pokémon!\p"
	.string "Tell: Torna con almeno…\n"
	.string "Pat: …due Pokémon, la prossima volta.$"
