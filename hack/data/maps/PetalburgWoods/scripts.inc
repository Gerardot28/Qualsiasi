PetalburgWoods_MapScripts::
	.byte 0

PetalburgWoods_EventScript_DevonResearcherLeft::
	lockall
	call PetalburgWoods_EventScript_DevonResearcherIntro
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherApproachPlayerLeft
	waitmovement 0
	msgbox PetalburgWoods_Text_HaveYouSeenShroomish, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaEntrance
	waitmovement 0
	msgbox PetalburgWoods_Text_IWasGoingToAmbushYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaApproachResearcherLeft
	waitmovement 0
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox PetalburgWoods_Text_HandOverThosePapers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherFleeToPlayerLeft
	waitmovement 0
	msgbox PetalburgWoods_Text_YouHaveToHelpMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaApproachPlayer
	waitmovement 0
	msgbox PetalburgWoods_Text_NoOneCrossesTeamAqua, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GRUNT_PETALBURG_WOODS, PetalburgWoods_Text_YoureKiddingMe
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaBackOff
	waitmovement 0
	call PetalburgWoods_EventScript_DevonResearcherPostBattle
	applymovement LOCALID_PLAYER, PetalburgWoods_Movement_WatchResearcherLeave
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherExitLeft
	waitmovement 0
	goto PetalburgWoods_EventScript_RemoveDevonResearcher
	end

PetalburgWoods_EventScript_DevonResearcherRight::
	lockall
	call PetalburgWoods_EventScript_DevonResearcherIntro
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherApproachPlayerRight
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox PetalburgWoods_Text_HaveYouSeenShroomish, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, FALSE
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaEntrance
	waitmovement 0
	msgbox PetalburgWoods_Text_IWasGoingToAmbushYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaApproachResearcherRight
	waitmovement 0
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox PetalburgWoods_Text_HandOverThosePapers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherFleeToPlayerRight
	waitmovement 0
	msgbox PetalburgWoods_Text_YouHaveToHelpMe, MSGBOX_DEFAULT
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox PetalburgWoods_Text_NoOneCrossesTeamAqua, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GRUNT_PETALBURG_WOODS, PetalburgWoods_Text_YoureKiddingMe
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaBackOff
	waitmovement 0
	call PetalburgWoods_EventScript_DevonResearcherPostBattle
	applymovement LOCALID_PLAYER, PetalburgWoods_Movement_WatchResearcherLeave
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherExitRight
	waitmovement 0
	goto PetalburgWoods_EventScript_RemoveDevonResearcher
	end

PetalburgWoods_EventScript_DevonResearcherIntro::
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherLookAround
	waitmovement 0
	msgbox PetalburgWoods_Text_NotAOneToBeFound, MSGBOX_DEFAULT
	closemessage
	return

PetalburgWoods_EventScript_DevonResearcherPostBattle::
	msgbox PetalburgWoods_Text_YouveGotSomeNerve, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PETALBURG_WOODS_GRUNT, PetalburgWoods_Movement_AquaRunAway
	waitmovement 0
	removeobject LOCALID_PETALBURG_WOODS_GRUNT
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox PetalburgWoods_Text_ThatWasAwfullyClose, MSGBOX_DEFAULT
	giveitem ITEM_GREAT_BALL
	goto_if_eq VAR_RESULT, FALSE, PetalburgWoods_EventScript_BagFull
	goto PetalburgWoods_EventScript_DevonResearcherFinish
	end

PetalburgWoods_EventScript_BagFull::
	msgbox PetalburgWoods_Text_YoureLoadedWithItems, MSGBOX_DEFAULT
	goto PetalburgWoods_EventScript_DevonResearcherFinish
	end

PetalburgWoods_EventScript_DevonResearcherFinish::
	msgbox PetalburgWoods_Text_TeamAquaAfterSomethingInRustboro, MSGBOX_DEFAULT
	applymovement LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE, PetalburgWoods_Movement_DevonResearcherStartExit
	waitmovement 0
	msgbox PetalburgWoods_Text_ICantBeWastingTime, MSGBOX_DEFAULT
	closemessage
	return

PetalburgWoods_EventScript_RemoveDevonResearcher::
	removeobject LOCALID_PETALBURG_WOODS_DEVON_EMPLOYEE
	setvar VAR_PETALBURG_WOODS_STATE, 1
	releaseall
	end

PetalburgWoods_Movement_DevonResearcherLookAround:
	face_up
	delay_16
	delay_4
	face_right
	delay_16
	delay_8
	face_left
	delay_16
	delay_8
	face_down
	delay_16
	face_right
	delay_16
	delay_8
	face_up
	delay_16
	step_end

PetalburgWoods_Movement_DevonResearcherExitLeft:
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

PetalburgWoods_Movement_DevonResearcherApproachPlayerLeft:
	delay_16
	face_player
	walk_down
	walk_down
	step_end

PetalburgWoods_Movement_DevonResearcherApproachPlayerRight:
	delay_16
	face_player
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

PetalburgWoods_Movement_DevonResearcherExitRight:
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

PetalburgWoods_Movement_WatchResearcherLeave:
	delay_16
	delay_16
	walk_in_place_faster_up
	step_end

PetalburgWoods_Movement_DevonResearcherFleeToPlayerLeft:
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_in_place_faster_up
	step_end

PetalburgWoods_Movement_DevonResearcherFleeToPlayerRight:
	walk_fast_down
	walk_fast_right
	walk_in_place_faster_up
	step_end

PetalburgWoods_Movement_DevonResearcherStartExit:
	walk_in_place_faster_down
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	face_up
	step_end

PetalburgWoods_Movement_AquaApproachResearcherLeft:
	walk_fast_down
	walk_fast_down
	step_end

PetalburgWoods_Movement_AquaBackOff:
	lock_facing_direction
	walk_up
	unlock_facing_direction
	step_end

PetalburgWoods_Movement_AquaRunAway:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	delay_16
	delay_16
	step_end

PetalburgWoods_Movement_AquaApproachResearcherRight:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	step_end

PetalburgWoods_Movement_AquaEntrance:
	walk_down
	walk_down
	delay_16
	delay_16
	step_end

PetalburgWoods_Movement_AquaApproachPlayer:
	walk_down
	step_end

PetalburgWoods_EventScript_Boy1::
	msgbox PetalburgWoods_Text_StayOutOfTallGrass, MSGBOX_NPC
	end

PetalburgWoods_EventScript_Boy2::
	msgbox PetalburgWoods_Text_HiddenItemsExplanation, MSGBOX_NPC
	end

PetalburgWoods_EventScript_Girl::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_MIRACLE_SEED, PetalburgWoods_EventScript_ExplainMiracleSeed
	msgbox PetalburgWoods_Text_TryUsingThisItem, MSGBOX_DEFAULT
	giveitem ITEM_MIRACLE_SEED
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_MIRACLE_SEED
	release
	end

PetalburgWoods_EventScript_ExplainMiracleSeed::
	msgbox PetalburgWoods_Text_MiracleSeedExplanation, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_Sign1::
	msgbox PetalburgWoods_Text_TrainerTipsExperience, MSGBOX_SIGN
	end

PetalburgWoods_EventScript_Sign2::
	msgbox PetalburgWoods_Text_TrainerTipsPP, MSGBOX_SIGN
	end

PetalburgWoods_EventScript_Lyle::
	trainerbattle_single TRAINER_LYLE, PetalburgWoods_Text_GoBugPokemonTeam, PetalburgWoods_Text_ICouldntWin
	msgbox PetalburgWoods_Text_ImOutOfPokeBalls, MSGBOX_AUTOCLOSE
	end

PetalburgWoods_EventScript_James::
	trainerbattle_single TRAINER_JAMES_1, PetalburgWoods_Text_InstantlyPopularWithBugPokemon, PetalburgWoods_Text_CantBePopularIfILose, PetalburgWoods_EventScript_TryRegisterJames
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, PetalburgWoods_EventScript_JamesRematch
	setvar VAR_0x8004, TRAINER_JAMES_1
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, PetalburgWoods_EventScript_TryRegisterJames2
	msgbox PetalburgWoods_Text_PeopleRespectYou, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_TryRegisterJames::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	goto_if_set FLAG_HAS_MATCH_CALL, PetalburgWoods_EventScript_RegisterJames
	release
	end

PetalburgWoods_EventScript_RegisterJames::
	msgbox PetalburgWoods_Text_IWantRematch1, MSGBOX_DEFAULT
	register_matchcall TRAINER_JAMES_1
	release
	end

PetalburgWoods_EventScript_TryRegisterJames2::
	goto_if_set FLAG_HAS_MATCH_CALL, PetalburgWoods_EventScript_RegisterJames2
	msgbox PetalburgWoods_Text_PeopleRespectYou, MSGBOX_DEFAULT
	release
	end

PetalburgWoods_EventScript_RegisterJames2::
	msgbox PetalburgWoods_Text_IWantRematch2, MSGBOX_DEFAULT
	register_matchcall TRAINER_JAMES_1
	release
	end

PetalburgWoods_EventScript_JamesRematch::
	trainerbattle_rematch TRAINER_JAMES_1, PetalburgWoods_Text_MyPokemonHaveGrown, PetalburgWoods_Text_CantBePopularIfLose
	msgbox PetalburgWoods_Text_IveBeenTrainingHard, MSGBOX_AUTOCLOSE
	end

PetalburgWoods_Text_NotAOneToBeFound:
	.string "Mmm…\n"
	.string "Neanche l'ombra di uno…$"

PetalburgWoods_Text_HaveYouSeenShroomish:
	.string "Ciao! Hai visto per caso dei Pokémon\n"
	.string "chiamati Shroomish da queste parti?\p"
	.string "Io li adoro, quei Pokémon.$"

PetalburgWoods_Text_IWasGoingToAmbushYou:
	.string "Arrr! Volevo tenderti un'imboscata,\n"
	.string "ma tu te la sei presa comoda nel\l"
	.string "Bosco Petalo, eh?\p"
	.string "Mi sono stufato di aspettare,\n"
	.string "quindi eccomi qua!$"

PetalburgWoods_Text_HandOverThosePapers:
	.string "Tu! Ricercatore Devon!\p"
	.string "Consegnami quei documenti! Servono\n"
	.string "al Capo per aprire una rotta che\l"
	.string "nessuno ha mai solcato!$"

PetalburgWoods_Text_YouHaveToHelpMe:
	.string "Aiutoooo! Perché sempre a me?!\p"
	.string "Sei un Allenatore di Pokémon, vero?\n"
	.string "Devi aiutarmi, ti prego!$"

PetalburgWoods_Text_NoOneCrossesTeamAqua:
	.string "Eh? Che cosa credi di fare?\n"
	.string "Vorresti proteggerlo, tu?\p"
	.string "Chi taglia la rotta al Team Omega\n"
	.string "finisce in mare, anche se è un pivello!\p"
	.string "Vele al vento! Lottiamo!$"

PetalburgWoods_Text_YoureKiddingMe:
	.string "Scherzi?! Sei un osso duro!$"

PetalburgWoods_Text_YouveGotSomeNerve:
	.string "Grrr… Hai un bel fegato a metterti\n"
	.string "contro il Team Omega!\l"
	.string "Forza, lottiamo di nuovo!\p"
	.string "…Lo direi volentieri, ma ho finito\n"
	.string "i Pokémon…\p"
	.string "Arrr! Tanto il colpo vero lo facciamo\n"
	.string "a Ferrugipoli!\p"
	.string "Per oggi ti lascio andare!$"

PetalburgWoods_Text_ThatWasAwfullyClose:
	.string "Uff…\n"
	.string "C'è mancato davvero poco!\p"
	.string "Grazie a te, non mi ha rubato\n"
	.string "questi documenti importanti.\p"
	.string "Ecco, ti do una Mega Ball\n"
	.string "per ringraziarti!$"

PetalburgWoods_Text_TeamAquaAfterSomethingInRustboro:
	.string "Quel pirata del Team Omega non ha\n"
	.string "detto che puntano a qualcosa anche\l"
	.string "a Ferrugipoli?$"

PetalburgWoods_Text_ICantBeWastingTime:
	.string "Oh, no! È un'emergenza!\n"
	.string "Non posso perdere tempo!$"

PetalburgWoods_Text_YoureLoadedWithItems:
	.string "Hai la borsa piena di strumenti.\n"
	.string "Non posso darti questa Mega Ball.$"

PetalburgWoods_Text_GoBugPokemonTeam:
	.string "Ho catturato un sacco di Pokémon!\p"
	.string "Vai, vai, vai!\n"
	.string "Squadra Coleottero, all'attacco!$"

PetalburgWoods_Text_ICouldntWin:
	.string "Ho tutti questi Pokémon,\n"
	.string "eppure ho perso…$"

PetalburgWoods_Text_ImOutOfPokeBalls:
	.string "Ho catturato un mucchio di Pokémon.\n"
	.string "E adesso sono senza Poké Ball.$"

PetalburgWoods_Text_InstantlyPopularWithBugPokemon:
	.string "Il mio Pokémon Coleottero viene\n"
	.string "da un Varco! A scuola sarò una star!$"

PetalburgWoods_Text_CantBePopularIfILose:
	.string "Se perdo, addio popolarità…$"

PetalburgWoods_Text_PeopleRespectYou:
	.string "Se hai un Pokémon Coleottero grosso,\n"
	.string "tutti ti rispettano.$"

PetalburgWoods_Text_IWantRematch1:
	.string "Voglio la rivincita quando i miei\n"
	.string "Pokémon Coleottero saranno cresciuti!\p"
	.string "Ti registro nel mio PokéNav!$"

PetalburgWoods_Text_IWantRematch2:
	.string "Voglio la rivincita quando i miei\n"
	.string "Pokémon Coleottero saranno cresciuti!\p"
	.string "Ti registro nel mio PokéNav!$"

PetalburgWoods_Text_MyPokemonHaveGrown:
	.string "I miei Pokémon Coleottero sono\n"
	.string "cresciuti. Tornerò popolare!$"

PetalburgWoods_Text_CantBePopularIfLose:
	.string "Se perdo, addio popolarità…$"

PetalburgWoods_Text_IveBeenTrainingHard:
	.string "Con dei Pokémon forti diventi\n"
	.string "popolare, giusto?\l"
	.string "Per questo mi alleno sodo.$"

PetalburgWoods_Text_StayOutOfTallGrass:
	.string "Ehilà!\n"
	.string "I tuoi Pokémon stanno bene?\p"
	.string "Se sono deboli e vuoi evitare\n"
	.string "le lotte, stai alla larga\l"
	.string "dall'erba alta.$"

PetalburgWoods_Text_HiddenItemsExplanation:
	.string "A volte per terra ci sono oggetti\n"
	.string "anche se non si vedono.\p"
	.string "Per questo guardo sempre dove\n"
	.string "metto i piedi.$"

PetalburgWoods_Text_TryUsingThisItem:
	.string "Oh, forte! Quella è la Medaglia\n"
	.string "della Palestra di Ferrugipoli!\p"
	.string "Allora sei un Allenatore.\n"
	.string "Prova a usare questo strumento.$"

PetalburgWoods_Text_MiracleSeedExplanation:
	.string "È un Miracolseme. Se lo fai tenere\n"
	.string "a un Pokémon, le sue mosse di tipo\l"
	.string "Erba diventano più potenti.\p"
	.string "Oltre al Miracolseme, credo ci siano\n"
	.string "altri strumenti utili da far tenere\l"
	.string "ai Pokémon.$"

PetalburgWoods_Text_TrainerTipsExperience:
	.string "Consigli Utili\p"
	.string "Ogni Pokémon che scende in campo,\n"
	.string "anche una volta sola, riceve Punti Esp.\p"
	.string "Per allenare un Pokémon debole,\n"
	.string "mettilo al primo posto della squadra.\p"
	.string "Appena inizia la lotta, sostituiscilo:\n"
	.string "riceverà Punti Esp. senza correre\l"
	.string "alcun rischio.$"

PetalburgWoods_Text_TrainerTipsPP:
	.string "Consigli Utili\p"
	.string "Oltre ai Punti Salute (PS), i Pokémon\n"
	.string "hanno i Punti Potenza (PP), che servono\l"
	.string "per usare le mosse in lotta.\p"
	.string "Se un Pokémon esaurisce i PP, va\n"
	.string "portato in un Centro Pokémon.$"

