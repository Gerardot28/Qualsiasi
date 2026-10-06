SlateportCity_OceanicMuseum_1F_MapScripts::
	.byte 0

SlateportCity_OceanicMuseum_1F_EventScript_EntranceAttendant::
	msgbox SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFeeLeft::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFeeRight::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee
	end

SlateportCity_OceanicMuseum_1F_EventScript_PayEntranceFee::
	showmoneybox 0, 0
	msgbox SlateportCity_OceanicMuseum_1F_Text_WouldYouLikeToEnter, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SlateportCity_OceanicMuseum_1F_EventScript_CheckMoneyForFee
	closemessage
	hidemoneybox
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter
	waitmovement 0
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_CheckMoneyForFee::
	checkmoney 50
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_OceanicMuseum_1F_EventScript_NotEnoughMoney
	playse SE_SHOP
	removemoney 50
	updatemoneybox
	msgbox SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 1
	hidemoneybox
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_NotEnoughMoney::
	goto_if_unset FLAG_DELIVERED_DEVON_GOODS, SlateportCity_OceanicMuseum_1F_EventScript_AllowEntranceAnyway
	msgbox SlateportCity_OceanicMuseum_1F_Text_NotEnoughMoney, MSGBOX_DEFAULT
	closemessage
	hidemoneybox
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter
	waitmovement 0
	releaseall
	end

SlateportCity_OceanicMuseum_1F_EventScript_AllowEntranceAnyway::
	msgbox SlateportCity_OceanicMuseum_1F_Text_CatchUpWithYourGroup, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_MUSEUM_1F_STATE, 1
	hidemoneybox
	releaseall
	end

SlateportCity_OceanicMuseum_1F_Movement_PushPlayerBackFromCounter:
	walk_down
	step_end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_AquaExistForGoodOfAll, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OurBossIsntHere, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WouldStuffHereMakeMeRich, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt4::
	msgbox SlateportCity_OceanicMuseum_1F_Text_CanLearnForNefariousDeeds, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt5::
	msgbox SlateportCity_OceanicMuseum_1F_Text_RustboroBungled, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumGrunt6::
	msgbox SlateportCity_OceanicMuseum_1F_Text_DidntHaveMoney, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_WhirlpoolExperiment::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WhirlpoolExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_WaterfallExperiment::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WaterfallExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanSoilDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanSoilDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_BeachSandDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_BeachSandDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact1, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact2, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_OceanicMinifact3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact3, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_FossilDisplay::
	msgbox SlateportCity_OceanicMuseum_1F_Text_FossilDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_DepthMeasuringMachine::
	msgbox SlateportCity_OceanicMuseum_1F_Text_DepthMeasuringMachine, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron1::
	msgbox SlateportCity_OceanicMuseum_1F_Text_LearnAboutSeaForBattling, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron2::
	msgbox SlateportCity_OceanicMuseum_1F_Text_SternIsRoleModel, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron3::
	msgbox SlateportCity_OceanicMuseum_1F_Text_MustBePokemonWeDontKnow, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_MuseumPatron4::
	msgbox SlateportCity_OceanicMuseum_1F_Text_WantSeaPokemon, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGrunt::
	lock
	faceplayer
	delay 8
	playse SE_PIN
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, Common_Movement_Delay48
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_1F_Text_RememberMeTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_THIEF
	goto_if_eq VAR_RESULT, 0, SlateportCity_OceanicMuseum_1F_EventScript_NoRoomForThief
	setflag FLAG_RECEIVED_TM_THIEF
	msgbox SlateportCity_OceanicMuseum_1F_Text_HopeINeverSeeYouAgain, MSGBOX_DEFAULT
	closemessage
	goto_if_eq VAR_FACING, DIR_NORTH, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitNorth
	goto_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitSouth
	goto_if_eq VAR_FACING, DIR_WEST, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast
	goto_if_eq VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitNorth::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitNorth
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExitNorth
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitSouth::
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExitWestEast::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitWestEast
	applymovement LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT, SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit
	waitmovement 0
	goto SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited
	end

SlateportCity_OceanicMuseum_1F_EventScript_FamiliarGruntExited::
	setflag FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_FAMILIAR_AQUA_GRUNT
	playse SE_EXIT
	removeobject LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT
	release
	end

SlateportCity_OceanicMuseum_1F_EventScript_NoRoomForThief::
	msgbox SlateportCity_OceanicMuseum_1F_Text_YouHaveToTakeThis, MSGBOX_DEFAULT
	release
	end

SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitNorth:
	delay_16
	delay_8
	delay_4
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_1F_Movement_PlayerWatchGruntExitWestEast:
	delay_16
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExit:
	face_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	delay_8
	step_end

SlateportCity_OceanicMuseum_1F_Movement_FamiliarGruntExitNorth:
	walk_fast_right
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	delay_8
	step_end

SlateportCity_OceanicMuseum_1F_Text_WouldYouLikeToEnter:
	.string "Benvenuto al Museo Oceanografico!\p"
	.string "Il biglietto d'ingresso costa ¥50.\n"
	.string "Desideri entrare?$"

SlateportCity_OceanicMuseum_1F_Text_PleaseEnjoyYourself:
	.string "Buona visita!$"

SlateportCity_OceanicMuseum_1F_Text_NotEnoughMoney:
	.string "Oh? Mi dispiace, ma non sembri avere\n"
	.string "abbastanza soldi.$"

SlateportCity_OceanicMuseum_1F_Text_CatchUpWithYourGroup:
	.string "Oh? Sei con quel gruppo in bandana\n"
	.string "blu entrato poco fa?\p"
	.string "Manchi solo tu all'appello.\n"
	.string "Corri, che ti lasciano indietro!$"

SlateportCity_OceanicMuseum_1F_Text_AquaExistForGoodOfAll:
	.string "Noi del Team Omega lottiamo per il bene\n"
	.string "di tutti i mondi! Vele al vento!$"

SlateportCity_OceanicMuseum_1F_Text_OurBossIsntHere:
	.string "Ci hanno ordinato di radunarci qui,\n"
	.string "e così abbiamo fatto, però…\p"
	.string "Il Capo non c'è! E senza capitano,\n"
	.string "la nave non salpa!$"

SlateportCity_OceanicMuseum_1F_Text_WouldStuffHereMakeMeRich:
	.string "Se arraffassi qualche cimelio,\n"
	.string "diventerei ricco? Arrr, che bottino!$"

SlateportCity_OceanicMuseum_1F_Text_CanLearnForNefariousDeeds:
	.string "Studio queste carte nautiche…\n"
	.string "per scopi tutt'altro che onesti. Arrr…$"

SlateportCity_OceanicMuseum_1F_Text_RustboroBungled:
	.string "Se quei pasticcioni non avessero fatto\n"
	.string "naufragio a Ferrugipoli, a quest'ora\l"
	.string "non saremmo qui!$"

SlateportCity_OceanicMuseum_1F_Text_DidntHaveMoney:
	.string "Non avevo i ¥50 del biglietto, e ci ho\n"
	.string "messo un'eternità a superare la cassa.$"

SlateportCity_OceanicMuseum_1F_Text_LearnAboutSeaForBattling:
	.string "Voglio studiare il mare e usare quello\n"
	.string "che imparo nelle lotte.$"

SlateportCity_OceanicMuseum_1F_Text_SternIsRoleModel:
	.string "Quando vedo il mare mi sciolgo come\n"
	.string "un ghiacciolo al sole!\p"
	.string "Per me Capitan Stern è il modello\n"
	.string "numero uno!$"

SlateportCity_OceanicMuseum_1F_Text_MustBePokemonWeDontKnow:
	.string "Il mare è vasto, senza fine, e\n"
	.string "profondo all'infinito…\p"
	.string "E con tutti questi Varchi, chissà\n"
	.string "quanti Pokémon sconosciuti nuotano\l"
	.string "laggiù.$"

SlateportCity_OceanicMuseum_1F_Text_WantSeaPokemon:
	.string "Voglio un Pokémon marino.\p"
	.string "Dev'essere così fresco e piacevole\n"
	.string "da abbracciare!$"

SlateportCity_OceanicMuseum_1F_Text_RememberMeTakeThis:
	.string "Aaaaah!\n"
	.string "Tu?! Di nuovo?!\p"
	.string "Io? Sono la Recluta del Team Omega\n"
	.string "che hai strapazzato, ricordi?\l"
	.string "Nel Tunnel Menferro?\p"
	.string "Ecco, prendi questa!\n"
	.string "Ti prego, perdonami!$"

SlateportCity_OceanicMuseum_1F_Text_HopeINeverSeeYouAgain:
	.string "Quella MT sta meglio a te che a me,\n"
	.string "fidati.\p"
	.string "Spero di non incrociare mai più la\n"
	.string "tua rotta! Uahahah!$"

SlateportCity_OceanicMuseum_1F_Text_YouHaveToTakeThis:
	.string "Uffa, dai!\n"
	.string "Prendila e lasciami andare!$"

SlateportCity_OceanicMuseum_1F_Text_WhirlpoolExperiment:
	.string "Un liquido blu vortica dentro\n"
	.string "un recipiente di vetro.\p"
	.string "“Questo esperimento crea un mulinello\n"
	.string "artificiale usando il vento.”$"

SlateportCity_OceanicMuseum_1F_Text_WaterfallExperiment:
	.string "Una pallina rossa va su e giù\n"
	.string "dentro un recipiente di vetro.\p"
	.string "“Questo esperimento simula una cascata\n"
	.string "sfruttando il galleggiamento della\l"
	.string "pallina.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanSoilDisplay:
	.string "È un campione di terreno prelevato\n"
	.string "dal fondale oceanico.\p"
	.string "“Nel corso dei secoli, i resti degli\n"
	.string "esseri viventi si depositano sul fondo\l"
	.string "del mare, formando strati sedimentari.\p"
	.string "“Analizzando questi strati, si svela\n"
	.string "il passato più remoto.\p"
	.string "“Alcuni strati di tremila anni fa\n"
	.string "contengono minerali sconosciuti a\l"
	.string "Hoenn. La ricerca è ancora in corso.”$"

SlateportCity_OceanicMuseum_1F_Text_BeachSandDisplay:
	.string "È un campione di sabbia di spiaggia.\p"
	.string "“Le pietre delle montagne vengono\n"
	.string "trascinate a valle dai fiumi, dove\l"
	.string "si scheggiano e si consumano.\p"
	.string "“Ridotte in granelli, finiscono\n"
	.string "sulle spiagge sotto forma di sabbia.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact1:
	.string "“Curiosità Oceaniche n. 1\n"
	.string "Perché il mare è blu?\p"
	.string "“La luce è composta da molti colori.\p"
	.string "“Quando attraversa l'acqua, quasi\n"
	.string "tutti i colori vengono assorbiti.\p"
	.string "“La luce blu, invece, resiste:\n"
	.string "ecco perché il mare appare blu.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact2:
	.string "“Curiosità Oceaniche n. 2\n"
	.string "Perché il mare è salato?\p"
	.string "“L'acqua di mare contiene sale disciolto\n"
	.string "sotto forma di ioni di sodio e cloro.\p"
	.string "“Questi ioni si staccano dalle rocce\n"
	.string "e la pioggia li trasporta fino al mare.\p"
	.string "“È la concentrazione di sale disciolto\n"
	.string "a rendere salato il mare.”$"

SlateportCity_OceanicMuseum_1F_Text_OceanicMinifact3:
	.string "“Curiosità Oceaniche n. 3\n"
	.string "È più grande il mare o la terra?\p"
	.string "“Il mare copre circa il 70% del\n"
	.string "pianeta, il resto è terraferma.\p"
	.string "“Il mare, quindi, è più del doppio\n"
	.string "della terraferma.”$"

SlateportCity_OceanicMuseum_1F_Text_FossilDisplay:
	.string "È un fossile con delle creste ondulate.\p"
	.string "“Il terreno del fondale oceanico\n"
	.string "viene levigato dalle maree.\p"
	.string "“L'acqua in movimento incide il terreno\n"
	.string "con piccole creste e avvallamenti.\p"
	.string "“Quando questo terreno si fossilizza,\n"
	.string "si parla di increspature fossili.”$"

SlateportCity_OceanicMuseum_1F_Text_DepthMeasuringMachine:
	.string "Uno strano macchinario ruota sotto\n"
	.string "una cupola di vetro.\p"
	.string "Forse serve a misurare la profondità\n"
	.string "di qualcosa…$"

