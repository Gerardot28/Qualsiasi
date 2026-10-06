MtPyre_Summit_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtPyre_Summit_OnTransition
	.byte 0

MtPyre_Summit_OnTransition:
	call_if_eq VAR_MT_PYRE_STATE, 2, MtPyre_Summit_EventScript_SetArchieMaxiePositions
	end

MtPyre_Summit_EventScript_SetArchieMaxiePositions::
	setobjectxyperm LOCALID_MT_PYRE_SUMMIT_MAXIE, 23, 6
	setobjectxyperm LOCALID_MT_PYRE_SUMMIT_ARCHIE, 22, 6
	end

MtPyre_Summit_EventScript_TeamAquaTrigger0::
	lockall
	setvar VAR_0x8008, 0
	goto MtPyre_Summit_EventScript_TeamAquaExits
	end

MtPyre_Summit_EventScript_TeamAquaTrigger1::
	lockall
	setvar VAR_0x8008, 1
	goto MtPyre_Summit_EventScript_TeamAquaExits
	end

MtPyre_Summit_EventScript_TeamAquaTrigger2::
	lockall
	setvar VAR_0x8008, 2
	goto MtPyre_Summit_EventScript_TeamAquaExits
	end

MtPyre_Summit_EventScript_TeamAquaExits::
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	applymovement LOCALID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 50
	call_if_eq VAR_0x8008, 0, MtPyre_Summit_EventScript_ArchieFacePlayer0
	call_if_eq VAR_0x8008, 1, MtPyre_Summit_EventScript_ArchieFacePlayer1
	call_if_eq VAR_0x8008, 2, MtPyre_Summit_EventScript_ArchieFacePlayer2
	msgbox MtPyre_Summit_Text_ArchieWeGotTheOrbLetsGo, MSGBOX_DEFAULT
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_MT_PYRE_SUMMIT_ARCHIE
	removeobject LOCALID_MT_PYRE_SUMMIT_GRUNT_1
	removeobject LOCALID_MT_PYRE_SUMMIT_GRUNT_2
	removeobject LOCALID_MT_PYRE_SUMMIT_GRUNT_3
	removeobject LOCALID_MT_PYRE_SUMMIT_GRUNT_4
	setflag FLAG_HIDE_MT_PYRE_SUMMIT_ARCHIE
	setflag FLAG_HIDE_MT_PYRE_SUMMIT_TEAM_AQUA
	fadedefaultbgm
	fadescreenswapbuffers FADE_FROM_BLACK
	delay 20
	setvar VAR_MT_PYRE_STATE, 1
	call_if_eq VAR_0x8008, 0, MtPyre_Summit_EventScript_OldLadyApproachPlayer0
	call_if_eq VAR_0x8008, 1, MtPyre_Summit_EventScript_OldLadyApproachPlayer1
	call_if_eq VAR_0x8008, 2, MtPyre_Summit_EventScript_OldLadyApproachPlayer2
	msgbox MtPyre_Summit_Text_BothOrbsTakenMagmaLeftThis, MSGBOX_DEFAULT
	giveitem ITEM_MAGMA_EMBLEM
	setflag FLAG_RECEIVED_RED_OR_BLUE_ORB
	setflag FLAG_HIDE_JAGGED_PASS_MAGMA_GUARD
	releaseall
	end

MtPyre_Summit_EventScript_ArchieFacePlayer0::
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, MtPyre_Summit_Movement_ArchieFacePlayer0
	waitmovement 0
	return

@ Archie is already facing player
MtPyre_Summit_EventScript_ArchieFacePlayer1::
	return

MtPyre_Summit_EventScript_ArchieFacePlayer2::
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, MtPyre_Summit_Movement_ArchieFacePlayer2
	waitmovement 0
	return

MtPyre_Summit_EventScript_OldLadyApproachPlayer0::
	applymovement LOCALID_MT_PYRE_SUMMIT_OLD_LADY, MtPyre_Summit_Movement_OldLadyApproachPlayer0
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

MtPyre_Summit_EventScript_OldLadyApproachPlayer1::
	applymovement LOCALID_MT_PYRE_SUMMIT_OLD_LADY, MtPyre_Summit_Movement_OldLadyApproachPlayer1
	waitmovement 0
	return

MtPyre_Summit_EventScript_OldLadyApproachPlayer2::
	applymovement LOCALID_MT_PYRE_SUMMIT_OLD_LADY, MtPyre_Summit_Movement_OldLadyApproachPlayer2
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

MtPyre_Summit_Movement_OldLadyApproachPlayer0:
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

MtPyre_Summit_Movement_OldLadyApproachPlayer1:
	walk_down
	step_end

MtPyre_Summit_Movement_OldLadyApproachPlayer2:
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

MtPyre_Summit_Movement_ArchieFacePlayer0:
	walk_left
	walk_in_place_faster_down
	step_end

MtPyre_Summit_Movement_ArchieFacePlayer2:
	walk_right
	walk_in_place_faster_down
	step_end

MtPyre_Summit_EventScript_OldMan::
	lock
	faceplayer
	goto_if_set FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE, MtPyre_Summit_EventScript_OldManAfterRayquaza
	msgbox MtPyre_Summit_Text_WillYouHearOutMyTale, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, MtPyre_Summit_EventScript_OldManTale
	call_if_eq VAR_RESULT, NO, MtPyre_Summit_EventScript_DeclineOldManTale
	release
	end

MtPyre_Summit_EventScript_OldManAfterRayquaza::
	msgbox MtPyre_Summit_Text_HearTheNewLegendOfHoenn, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, MtPyre_Summit_EventScript_OldManNewTale
	call_if_eq VAR_RESULT, NO, MtPyre_Summit_EventScript_DeclineOldManTale
	release
	end

MtPyre_Summit_EventScript_OldManTale::
	msgbox MtPyre_Summit_Text_GroudonKyogreTale, MSGBOX_DEFAULT
	return

MtPyre_Summit_EventScript_DeclineOldManTale::
	msgbox MtPyre_Summit_Text_WellThatTooIsFine, MSGBOX_DEFAULT
	return

MtPyre_Summit_EventScript_OldManNewTale::
	msgbox MtPyre_Summit_Text_HoennTrioTale, MSGBOX_DEFAULT
	return

MtPyre_Summit_EventScript_OldLady::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_RED_OR_BLUE_ORB, MtPyre_Summit_EventScript_OldLadyAfterOrbsReturned
	call_if_ge VAR_MT_PYRE_STATE, 3, MtPyre_Summit_EventScript_OldLadyOrbsReturned
	goto_if_set FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN, MtPyre_Summit_EventScript_OldLadyLegendariesAwake
	msgbox MtPyre_Summit_Text_OrbsHaveBeenTaken, MSGBOX_DEFAULT
	release
	end

MtPyre_Summit_EventScript_OldLadyOrbsReturned::
	msgbox MtPyre_Summit_Text_ThoseTwoMenReturnedOrbs, MSGBOX_DEFAULT
	setflag FLAG_RETURNED_RED_OR_BLUE_ORB
	release
	end

MtPyre_Summit_EventScript_OldLadyLegendariesAwake::
	msgbox MtPyre_Summit_Text_GroudonKyogreAwakened, MSGBOX_DEFAULT
	release
	end

MtPyre_Summit_EventScript_OldLadyAfterOrbsReturned::
	msgbox MtPyre_Summit_Text_SuperAncientPokemonTaughtUs, MSGBOX_DEFAULT
	release
	end

MtPyre_Summit_EventScript_ArchieMaxieTrigger0::
	lockall
	setvar VAR_0x8008, 0
	goto MtPyre_Summit_EventScript_ArchieMaxieReturnOrbs
	end

MtPyre_Summit_EventScript_ArchieMaxieTrigger1::
	lockall
	setvar VAR_0x8008, 1
	goto MtPyre_Summit_EventScript_ArchieMaxieReturnOrbs
	end

MtPyre_Summit_EventScript_ArchieMaxieTrigger2::
	lockall
	setvar VAR_0x8008, 2
	goto MtPyre_Summit_EventScript_ArchieMaxieReturnOrbs
	end

MtPyre_Summit_EventScript_ArchieMaxieReturnOrbs::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 60
	call_if_eq VAR_0x8008, 0, MtPyre_Summit_EventScript_ArchieMaxieBeginExit0
	call_if_eq VAR_0x8008, 1, MtPyre_Summit_EventScript_ArchieMaxieBeginExit1
	call_if_eq VAR_0x8008, 2, MtPyre_Summit_EventScript_ArchieMaxieBeginExit2
	playse SE_PIN
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, Common_Movement_Delay48
	waitmovement 0
	delay 30
	call_if_eq VAR_0x8008, 0, MtPyre_Summit_EventScript_MaxieApproachPlayer0
	call_if_eq VAR_0x8008, 1, MtPyre_Summit_EventScript_MaxieApproachPlayer1
	call_if_eq VAR_0x8008, 2, MtPyre_Summit_EventScript_MaxieApproachPlayer2
	msgbox MtPyre_Summit_Text_MaxieSilence, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_0x8008, 0, MtPyre_Summit_EventScript_MaxieApproachArchie0
	call_if_eq VAR_0x8008, 1, MtPyre_Summit_EventScript_MaxieApproachArchie1
	call_if_eq VAR_0x8008, 2, MtPyre_Summit_EventScript_MaxieApproachArchie2
	delay 30
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, MtPyre_Summit_Movement_ArchieExit
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, MtPyre_Summit_Movement_MaxieExit
	waitmovement 0
	removeobject LOCALID_MT_PYRE_SUMMIT_ARCHIE
	removeobject LOCALID_MT_PYRE_SUMMIT_MAXIE
	setvar VAR_MT_PYRE_STATE, 3
	releaseall
	end

MtPyre_Summit_EventScript_ArchieMaxieBeginExit0::
	applymovement LOCALID_PLAYER, MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit0
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, MtPyre_Summit_Movement_ArchieExit
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, MtPyre_Summit_Movement_MaxieExit
	waitmovement 0
	return

MtPyre_Summit_EventScript_ArchieMaxieBeginExit1::
	applymovement LOCALID_PLAYER, MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit1
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, MtPyre_Summit_Movement_ArchieExit
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, MtPyre_Summit_Movement_MaxieExit
	waitmovement 0
	return

MtPyre_Summit_EventScript_ArchieMaxieBeginExit2::
	applymovement LOCALID_PLAYER, MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit2
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, MtPyre_Summit_Movement_ArchieExit
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, MtPyre_Summit_Movement_MaxieExit
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachPlayer0::
	applymovement LOCALID_PLAYER, MtPyre_Summit_Movement_PlayerFaceMaxie0
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, MtPyre_Summit_Movement_ArchieWatchMaxie
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, MtPyre_Summit_Movement_MaxieApproachPlayer0
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachPlayer1::
	applymovement LOCALID_PLAYER, MtPyre_Summit_Movement_PlayerFaceMaxie
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, MtPyre_Summit_Movement_ArchieWatchMaxie
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, MtPyre_Summit_Movement_MaxieApproachPlayer1
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachPlayer2::
	applymovement LOCALID_PLAYER, MtPyre_Summit_Movement_PlayerFaceMaxie
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, MtPyre_Summit_Movement_ArchieWatchMaxie
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, MtPyre_Summit_Movement_MaxieApproachPlayer2
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachArchie0::
	applymovement LOCALID_PLAYER, MtPyre_Summit_Movement_PlayerWatchMaxie
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, MtPyre_Summit_Movement_MaxieApproachArchie0
	waitmovement 0
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachArchie1::
	applymovement LOCALID_PLAYER, MtPyre_Summit_Movement_PlayerWatchMaxie
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, MtPyre_Summit_Movement_MaxieApproachArchie1
	waitmovement 0
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

MtPyre_Summit_EventScript_MaxieApproachArchie2::
	applymovement LOCALID_PLAYER, MtPyre_Summit_Movement_PlayerWatchMaxie
	applymovement LOCALID_MT_PYRE_SUMMIT_MAXIE, MtPyre_Summit_Movement_MaxieApproachArchie2
	waitmovement 0
	applymovement LOCALID_MT_PYRE_SUMMIT_ARCHIE, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

MtPyre_Summit_Movement_PlayerFaceMaxie0:
	delay_16
	delay_16
	walk_in_place_faster_right
	step_end

MtPyre_Summit_Movement_PlayerFaceMaxie:
	delay_16
	delay_16
	walk_in_place_faster_left
	step_end

MtPyre_Summit_Movement_ArchieExit:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtPyre_Summit_Movement_MaxieExit:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit0:
	delay_16
	delay_8
	walk_left
	walk_in_place_faster_right
	delay_16
	walk_in_place_faster_down
	step_end

MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit1:
	delay_16
	delay_8
	walk_right
	walk_in_place_faster_left
	delay_16
	walk_in_place_faster_down
	step_end

MtPyre_Summit_Movement_PlayerWatchArchieMaxieExit2:
	delay_16
	delay_8
	walk_in_place_faster_left
	delay_16
	walk_in_place_faster_down
	step_end

MtPyre_Summit_Movement_MaxieApproachPlayer0:
	walk_up
	walk_up
	walk_up
	walk_left
	step_end

MtPyre_Summit_Movement_MaxieApproachPlayer1:
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_right
	step_end

MtPyre_Summit_Movement_MaxieApproachPlayer2:
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_right
	step_end

MtPyre_Summit_Movement_ArchieWatchMaxie:
	delay_16
	walk_in_place_faster_up
	step_end

MtPyre_Summit_Movement_PlayerWatchMaxie:
	delay_16
	walk_in_place_faster_down
	step_end

MtPyre_Summit_Movement_MaxieApproachArchie0:
	walk_down
	walk_right
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

MtPyre_Summit_Movement_MaxieApproachArchie1:
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

MtPyre_Summit_Movement_MaxieApproachArchie2:
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_left
	step_end

MtPyre_Summit_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MT_PYRE_1, MtPyre_Summit_Text_Grunt1Intro, MtPyre_Summit_Text_Grunt1Defeat
	msgbox MtPyre_Summit_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_Summit_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MT_PYRE_2, MtPyre_Summit_Text_Grunt2Intro, MtPyre_Summit_Text_Grunt2Defeat
	msgbox MtPyre_Summit_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_Summit_EventScript_Grunt3::
	trainerbattle_single TRAINER_GRUNT_MT_PYRE_3, MtPyre_Summit_Text_Grunt3Intro, MtPyre_Summit_Text_Grunt3Defeat
	msgbox MtPyre_Summit_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_Summit_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_MT_PYRE_4, MtPyre_Summit_Text_Grunt4Intro, MtPyre_Summit_Text_Grunt4Defeat
	msgbox MtPyre_Summit_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_Summit_Text_Grunt1Intro:
	.string "No! Quelle talpe rosse dell'Alpha\n"
	.string "sono arrivate prima di noi!\p"
	.string "Non possiamo restare indietro! Arrr!$"

MtPyre_Summit_Text_Grunt1Defeat:
	.string "Credevo fossi uno dell'Alpha\n"
	.string "tornato a fare il furbo…$"

MtPyre_Summit_Text_Grunt1PostBattle:
	.string "D'accordo, sei forte…\n"
	.string "Ma noi siamo una ciurma intera!$"

MtPyre_Summit_Text_Grunt2Intro:
	.string "Ah!\n"
	.string "Che sfortuna, la tua!\p"
	.string "Se arrivavi prima, ti toccava solo\n"
	.string "quei mollaccioni del Team Alpha.\p"
	.string "E invece adesso devi vedertela\n"
	.string "con noi, i veri lupi di mare!$"

MtPyre_Summit_Text_Grunt2Defeat:
	.string "Urgh… Dovevo lasciarti prima\n"
	.string "a quelli dell'Alpha…$"

MtPyre_Summit_Text_Grunt2PostBattle:
	.string "Tu non sai niente di niente!\n"
	.string "Perché ti immischi nei nostri affari?$"

MtPyre_Summit_Text_Grunt3Intro:
	.string "Tu…\n"
	.string "Ti abbiamo visto al Monte Camino.\p"
	.string "Non stai né con l'Alpha né con noi.\n"
	.string "Allora che ci fai quassù?$"

MtPyre_Summit_Text_Grunt3Defeat:
	.string "Se proprio vuoi attaccar briga,\n"
	.string "prenditela con l'Alpha…$"

MtPyre_Summit_Text_Grunt3PostBattle:
	.string "Eheh, tanto non importa!\n"
	.string "Ti abbiamo fatto perdere tempo!\p"
	.string "A quest'ora il Capo avrà già preso\n"
	.string "quello che cercava! Arrr!$"

MtPyre_Summit_Text_Grunt4Intro:
	.string "Ah, ho capito!\n"
	.string "Volevi entrare nel Team Alpha,\l"
	.string "ma non ti hanno voluto!\p"
	.string "Beh, non sperare di salire a bordo\n"
	.string "del Team Omega!$"

MtPyre_Summit_Text_Grunt4Defeat:
	.string "Se ci tieni tanto a entrare nel\n"
	.string "Team Omega, possiamo parlarne…$"

MtPyre_Summit_Text_Grunt4PostBattle:
	.string "Noi della ciurma e il nostro Capo\n"
	.string "siamo un equipaggio perfetto!$"

MtPyre_Summit_Text_ArchieWeGotTheOrbLetsGo:
	.string "Ulisse: Ahrrr! Ettore ci ha battuti sul\n"
	.string "tempo e si è preso la Gemma Blu…\p"
	.string "Ma anche noi abbiamo avuto quel che\n"
	.string "volevamo. La Gemma Rossa del Monte\l"
	.string "Pira ora è nelle mani di Ulisse!\p"
	.string "Con questa sveglierò l'Ancora del Mare.\n"
	.string "Leveremo l'ancora, mozzo!\p"
	.string "Hoenn salperà verso tutti i mondi!\n"
	.string "Ahrrr-ha-ha!\p"
	.string "Avanti, ciurma!\n"
	.string "Si torna a bordo!$"

MtPyre_Summit_Text_BothOrbsTakenMagmaLeftThis:
	.string "Oh, no…\n"
	.string "Non può essere…\p"
	.string "Prima la Gemma Blu, e adesso anche\n"
	.string "la Gemma Rossa…\p"
	.string "Le Gemme non devono mai essere\n"
	.string "separate. Tremila anni fa, quando\l"
	.string "il cielo si aprì, l'Ancora della Terra\l"
	.string "e l'Ancora del Mare lottarono.\p"
	.string "Solo le Gemme, unite, riuscirono\n"
	.string "a calmarle…\p"
	.string "Che cosa vogliono fare quegli uomini\n"
	.string "con le due Gemme?\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Ah, sì. Quelli che sono venuti prima\n"
	.string "a prendere la Gemma Blu…\l"
	.string "erano del Team Alpha, vero?\p"
	.string "Nella fretta, hanno lasciato qui\n"
	.string "questa cosa.\p"
	.string "Non saprei proprio dire cosa sia.\p"
	.string "Vorrei che la prendessi tu.\n"
	.string "Forse ti sarà utile, in qualche modo.$"

MtPyre_Summit_Text_OrbsHaveBeenTaken:
	.string "La Gemma Blu e la Gemma Rossa, prese\n"
	.string "da quegli uomini senza scrupoli…\p"
	.string "Non devono mai stare separate…\n"
	.string "Temo che accadrà qualcosa di terribile…$"

MtPyre_Summit_Text_GroudonKyogreAwakened:
	.string "Oh, santo cielo…\p"
	.string "Dici che Groudon e Kyogre, le due\n"
	.string "Ancore, si sono risvegliati?\p"
	.string "Si dice che la Gemma Blu e la Gemma\n"
	.string "Rossa placarono la furia dei due\l"
	.string "Pokémon…\p"
	.string "Si dice anche che il vero padrone\n"
	.string "delle Gemme esista ancora.\p"
	.string "Ma nessuno sa chi, o che cosa,\n"
	.string "sia davvero il loro padrone.\p"
	.string "Potrebbe essere un uomo, un Pokémon…\n"
	.string "o qualcuno che veglia dall'alto del\l"
	.string "cielo. Nessuno lo sa.$"

MtPyre_Summit_Text_ThoseTwoMenReturnedOrbs:
	.string "I due uomini che avevano preso le\n"
	.string "Gemme sono tornati a restituirle.\l"
	.string "Di loro spontanea volontà.\p"
	.string "Quei due…\n"
	.string "Forse non sono poi così malvagi…$"

MtPyre_Summit_Text_SuperAncientPokemonTaughtUs:
	.string "La terra, il mare e il cielo…\p"
	.string "Si dice che i Pokémon antichissimi\n"
	.string "ne siano l'incarnazione: le due\l"
	.string "Ancore e il Custode della Volta…\p"
	.string "Come noi umani e come i Pokémon,\n"
	.string "anche il nostro mondo è vivo…\p"
	.string "E ogni cosa viva ha bisogno di porte\n"
	.string "custodite, non di muri né di voragini.\p"
	.string "Questo ci hanno insegnato i Pokémon\n"
	.string "antichissimi…$"

MtPyre_Summit_Text_WillYouHearOutMyTale:
	.string "Il Monte Pira è il luogo dove si\n"
	.string "placano gli spiriti dei Pokémon\l"
	.string "che ci hanno lasciato…\p"
	.string "Da quassù si vede tutta Hoenn…\p"
	.string "Gli spiriti dei Pokémon trovano\n"
	.string "conforto in questo luogo…\p"
	.string "Sarà un caso che tu sia qui…?\p"
	.string "Vuoi ascoltare il mio racconto? Una\n"
	.string "leggenda che si tramanda a Hoenn\l"
	.string "da tremila anni?$"

MtPyre_Summit_Text_GroudonKyogreTale:
	.string "Accadde tanto, tanto tempo fa…\p"
	.string "Il cielo sopra Hoenn si squarciò,\n"
	.string "e da quella ferita piovvero frammenti\l"
	.string "di altri mondi…\p"
	.string "Le due Ancore di Hoenn, il Pokémon\n"
	.string "della terra e il Pokémon del mare,\l"
	.string "impazzirono e si scontrarono…\p"
	.string "Il Pokémon della terra sollevò\n"
	.string "montagne e creò continenti…\p"
	.string "Il Pokémon del mare sollevò onde\n"
	.string "immense e scavò gli oceani…\p"
	.string "Il loro feroce duello non aveva fine.\p"
	.string "Il mondo fu travolto da incendi\n"
	.string "immensi e da tempeste furiose.\p"
	.string "Furono la Gemma Blu e la Gemma Rossa\n"
	.string "a porre fine alla catastrofe.\p"
	.string "La luce delle due Gemme placò\n"
	.string "i due avversari infuriati.\p"
	.string "Ormai docili, i due Pokémon si\n"
	.string "inabissarono nel mare, finché\l"
	.string "scomparvero…$"

MtPyre_Summit_Text_WellThatTooIsFine:
	.string "Capisco…\n"
	.string "Va bene anche così…$"

MtPyre_Summit_Text_MaxieSilence:
	.string "Ettore: {PLAYER}…\p"
	.string "… … … … … …\n"
	.string "… … … … … …$"

MtPyre_Summit_Text_HearTheNewLegendOfHoenn:
	.string "Il mio compito è tramandare le\n"
	.string "leggende di Hoenn a chi verrà dopo.\p"
	.string "E la crisi appena conclusa a Ceneride\n"
	.string "ha riscritto una leggenda…\p"
	.string "Hai tempo per ascoltare la nuova\n"
	.string "leggenda di Hoenn?$"

MtPyre_Summit_Text_HoennTrioTale:
	.string "Accadde tanto, tanto tempo fa…\p"
	.string "Il cielo sopra Hoenn si squarciò:\n"
	.string "si era aperto il Grande Varco.\p"
	.string "Le due Ancore, il Pokémon della terra\n"
	.string "e il Pokémon del mare, impazzirono\l"
	.string "e si scontrarono…\p"
	.string "Il Pokémon della terra sollevò\n"
	.string "montagne e creò continenti…\p"
	.string "Il Pokémon del mare sollevò onde\n"
	.string "immense e scavò gli oceani…\p"
	.string "Il loro feroce duello non aveva fine.\p"
	.string "Il mondo fu travolto da incendi\n"
	.string "e da tempeste furiose.\p"
	.string "Fu allora che il Pokémon del cielo,\n"
	.string "il Custode della Volta, scese\l"
	.string "dalla tempesta.\p"
	.string "Splendente di un verde vivo, placò\n"
	.string "i due Pokémon infuriati.\p"
	.string "Poi ricucì il cielo, e il Grande Varco\n"
	.string "divenne una Cicatrice.\p"
	.string "Compiuto il suo dovere, il Pokémon\n"
	.string "verde volò via, oltre l'azzurro…$"

