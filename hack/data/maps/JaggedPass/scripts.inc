JaggedPass_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, JaggedPass_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, JaggedPass_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, JaggedPass_OnLoad
	.byte 0

JaggedPass_OnResume:
	setstepcallback STEP_CB_ASH
	call_if_eq VAR_JAGGED_PASS_STATE, 0, JaggedPass_EventScript_CheckHasMagmaEmblem
	end

JaggedPass_EventScript_CheckHasMagmaEmblem::
	checkitem ITEM_MAGMA_EMBLEM
	goto_if_eq VAR_RESULT, TRUE, JaggedPass_EventScript_SetReadyToOpenHideout
	return

JaggedPass_EventScript_SetReadyToOpenHideout::
	setvar VAR_JAGGED_PASS_STATE, 1
	return

JaggedPass_OnTransition:
	call_if_eq VAR_JAGGED_PASS_ASH_WEATHER, 1, JaggedPass_EventScript_SetWeatherAsh
	end

JaggedPass_EventScript_SetWeatherAsh::
	setweather WEATHER_VOLCANIC_ASH
	doweather
	return

JaggedPass_OnLoad:
	goto_if_le VAR_JAGGED_PASS_STATE, 1, JaggedPass_EventScript_ConcealHideoutEntrance
	end

JaggedPass_EventScript_ConcealHideoutEntrance::
	setmetatile 16, 17, METATILE_Lavaridge_RockWall, TRUE
	setmetatile 16, 18, METATILE_Lavaridge_RockWall, TRUE
	end

JaggedPass_EventScript_OpenMagmaHideout::
	lockall
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 5  @ shake delay
	special ShakeCamera
	waitstate
	msgbox JaggedPass_Text_BoulderShakingInResponseToEmblem, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, 1   @ vertical pan
	setvar VAR_0x8005, 1   @ horizontal pan
	setvar VAR_0x8006, 16  @ num shakes
	setvar VAR_0x8007, 3   @ shake delay
	special ShakeCamera
	waitstate
	playse SE_EFFECTIVE
	setmetatile 16, 17, METATILE_Lavaridge_CaveEntrance_Top, TRUE
	setmetatile 16, 18, METATILE_Lavaridge_CaveEntrance_Bottom, FALSE
	special DrawWholeMapView
	delay 30
	setvar VAR_JAGGED_PASS_STATE, 2
	waitse
	releaseall
	end

JaggedPass_EventScript_MagmaHideoutGuard::
	lockall
	goto_if_set FLAG_BEAT_MAGMA_GRUNT_JAGGED_PASS, JaggedPass_EventScript_GuardDefeated
	waitse
	playse SE_PIN
	applymovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_FacePlayer
	waitmovement 0
	msgbox JaggedPass_Text_GruntIntro, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_GRUNT_JAGGED_PASS, JaggedPass_Text_GruntDefeat
	setflag FLAG_BEAT_MAGMA_GRUNT_JAGGED_PASS
	applymovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

JaggedPass_EventScript_GuardDefeated::
	applymovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_FacePlayer
	waitmovement 0
	msgbox JaggedPass_Text_GoWhereverYouWant, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MAGMA_HIDEOUT_GUARD, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

JaggedPass_EventScript_Eric::
	trainerbattle_single TRAINER_ERIC, JaggedPass_Text_EricIntro, JaggedPass_Text_EricDefeat
	msgbox JaggedPass_Text_EricPostBattle, MSGBOX_AUTOCLOSE
	end

JaggedPass_EventScript_Diana::
	trainerbattle_single TRAINER_DIANA_1, JaggedPass_Text_DianaIntro, JaggedPass_Text_DianaDefeat, JaggedPass_EventScript_RegisterDiana
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, JaggedPass_EventScript_DianaRematch
	msgbox JaggedPass_Text_DianaPostBattle, MSGBOX_DEFAULT
	release
	end

JaggedPass_EventScript_RegisterDiana::
	special PlayerFaceTrainerAfterBattle
	msgbox JaggedPass_Text_DianaRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_DIANA_1
	release
	end

JaggedPass_EventScript_DianaRematch::
	trainerbattle_rematch TRAINER_DIANA_1, JaggedPass_Text_DianaRematchIntro, JaggedPass_Text_DianaRematchDefeat
	msgbox JaggedPass_Text_DianaPostRematch, MSGBOX_AUTOCLOSE
	end

JaggedPass_EventScript_Ethan::
	trainerbattle_single TRAINER_ETHAN_1, JaggedPass_Text_EthanIntro, JaggedPass_Text_EthanDefeat, JaggedPass_EventScript_RegisterEthan
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, JaggedPass_EventScript_EthanRematch
	msgbox JaggedPass_Text_EthanPostBattle, MSGBOX_DEFAULT
	release
	end

JaggedPass_EventScript_RegisterEthan::
	special PlayerFaceTrainerAfterBattle
	msgbox JaggedPass_Text_EthanRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_ETHAN_1
	release
	end

JaggedPass_EventScript_EthanRematch::
	trainerbattle_rematch TRAINER_ETHAN_1, JaggedPass_Text_EthanRematchIntro, JaggedPass_Text_EthanRematchDefeat
	msgbox JaggedPass_Text_EthanPostRematch, MSGBOX_AUTOCLOSE
	end

JaggedPass_EventScript_Julio::
	trainerbattle_single TRAINER_JULIO, JaggedPass_Text_JulioIntro, JaggedPass_Text_JulioDefeat
	msgbox JaggedPass_Text_JulioPostBattle, MSGBOX_AUTOCLOSE
	end

JaggedPass_EventScript_Autumn::
	trainerbattle_single TRAINER_AUTUMN, JaggedPass_Text_AutumnIntro, JaggedPass_Text_AutumnDefeat
	msgbox JaggedPass_Text_AutumnPostBattle, MSGBOX_AUTOCLOSE
	end

JaggedPass_Text_EricIntro:
	.string "Il Passo Selvaggio del Monte Camino…\p"
	.string "Ecco cosa ho sempre cercato in una\n"
	.string "montagna.\p"
	.string "Queste rocce aguzze…\n"
	.string "Mi fanno vibrare l'anima!$"

JaggedPass_Text_EricDefeat:
	.string "Sconfitta amara come la cenere!$"

JaggedPass_Text_EricPostBattle:
	.string "Sì, ho perso la lotta Pokémon…\p"
	.string "Ma quanto ad amore per la montagna,\n"
	.string "non mi batte nessuno!$"

JaggedPass_Text_DianaIntro:
	.string "Questa non è una passeggiata qualsiasi.\n"
	.string "Altro che picnic!$"

JaggedPass_Text_DianaDefeat:
	.string "Ohhh, no!\n"
	.string "Il terreno è troppo accidentato…$"

JaggedPass_Text_DianaPostBattle:
	.string "Lo sapevi?\p"
	.string "C'è chi sale questo passo tutto buche\n"
	.string "in bicicletta, con dei saltelli furbi!$"

JaggedPass_Text_DianaRegister:
	.string "Ripasserai da queste parti?\n"
	.string "Se torni, voglio la rivincita!$"

JaggedPass_Text_DianaRematchIntro:
	.string "I picnic sono belli ovunque.\n"
	.string "Proprio come i Pokémon!$"

JaggedPass_Text_DianaRematchDefeat:
	.string "Ho perso solo perché il terreno è\n"
	.string "troppo accidentato!$"

JaggedPass_Text_DianaPostRematch:
	.string "Dimentico la sconfitta e mi godo\n"
	.string "questa gita tutta buche.$"

JaggedPass_Text_EthanIntro:
	.string "Il Passo Selvaggio è duro da scalare.\n"
	.string "Perfetto per allenarsi!$"

JaggedPass_Text_EthanDefeat:
	.string "Era già finita mentre cercavamo\n"
	.string "ancora un appoggio sicuro…$"

JaggedPass_Text_EthanPostBattle:
	.string "Se avessi una Bici Cross, potrei\n"
	.string "saltare su queste sporgenze.$"

JaggedPass_Text_EthanRegister:
	.string "Quando mi sarò abituato a queste buche,\n"
	.string "vincerò di sicuro!\p"
	.string "Mi registri nel tuo PokéNav?$"

JaggedPass_Text_EthanRematchIntro:
	.string "Ormai ci ho fatto l'abitudine.\n"
	.string "Adesso canto, mentre salgo!$"

JaggedPass_Text_EthanRematchDefeat:
	.string "Lottare su questo terreno è ancora\n"
	.string "un'impresa…$"

JaggedPass_Text_EthanPostRematch:
	.string "Dovrei farmi dare una Bici Cross da\n"
	.string "Rydel, a Ciclamipoli…$"

JaggedPass_Text_GruntIntro:
	.string "Uah!\n"
	.string "E tu che ci fai qui?\p"
	.string "Io? Cosa ci faccio io quassù?\p"
	.string "Non sono affari tuoi! Qui non c'è\n"
	.string "niente da vedere! Circolare!$"

JaggedPass_Text_GruntDefeat:
	.string "Urrrgh…\p"
	.string "Dovevo rintanarmi subito nel nostro\n"
	.string "Covo…$"

JaggedPass_Text_GoWhereverYouWant:
	.string "Va bene, va bene!\n"
	.string "Lo ammetto: sei forte!\p"
	.string "Non preoccuparti per me.\n"
	.string "Vai pure dove ti pare!\p"
	.string "E questa parete è solo una parete.\n"
	.string "Giuro!$"

JaggedPass_Text_BoulderShakingInResponseToEmblem:
	.string "Oh! Questo masso trema in risposta\n"
	.string "allo Stemma Alpha!$"

JaggedPass_Text_JulioIntro:
	.string "Aiiih! Che paura scendere dalla\n"
	.string "montagna tutto d'un fiato!$"

JaggedPass_Text_JulioDefeat:
	.string "Mi sento a pezzi…$"

JaggedPass_Text_JulioPostBattle:
	.string "La bici ha sobbalzato così tanto che\n"
	.string "mi fa male il fondoschiena…$"

JaggedPass_Text_AutumnIntro:
	.string "Salgo su questo pendio ogni giorno.\n"
	.string "Ho fiducia nella mia forza!$"

JaggedPass_Text_AutumnDefeat:
	.string "Mmh…\n"
	.string "Cos'è andato storto?$"

JaggedPass_Text_AutumnPostBattle:
	.string "Cos'è quella strana sporgenza di roccia\n"
	.string "un po' più su?\p"
	.string "L'altro giorno ci ho visto sparire\n"
	.string "dentro un tizio col mantello rosso…$"

