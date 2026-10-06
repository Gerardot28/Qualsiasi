SlateportCity_OceanicMuseum_2F_MapScripts::
	.byte 0

SlateportCity_OceanicMuseum_2F_EventScript_CaptStern::
	lock
	faceplayer
	msgbox SlateportCity_OceanicMuseum_2F_Text_ThankYouForTheParts, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, TRUE
	addobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_FirstGruntEnter
	waitmovement 0
	addobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_SecondGruntEnter
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_FirstGruntApproach
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_SecondGruntApproach
	waitmovement 0
	call_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts
	call_if_eq VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts
	msgbox SlateportCity_OceanicMuseum_2F_Text_WellTakeThoseParts, MSGBOX_DEFAULT
	call_if_ne VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_2F_EventScript_SternFaceGrunts
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternWhoAreYou, MSGBOX_DEFAULT
	msgbox SlateportCity_OceanicMuseum_2F_Text_WereTeamAqua, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle
	waitmovement 0
	call_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntSouth
	call_if_eq VAR_FACING, DIR_WEST, SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntWest
	trainerbattle_no_intro TRAINER_GRUNT_MUSEUM_1, SlateportCity_OceanicMuseum_2F_Text_Grunt1Defeat
	msgbox SlateportCity_OceanicMuseum_2F_Text_BossGoingToBeFurious, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_LetMeTakeCareOfThis, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GRUNT_MUSEUM_2, SlateportCity_OceanicMuseum_2F_Text_Grunt2Defeat
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_MeddlingKid, MSGBOX_DEFAULT
	closemessage
	delay 35
	addobject LOCALID_OCEANIC_MUSEUM_2F_ARCHIE
	applymovement LOCALID_OCEANIC_MUSEUM_2F_ARCHIE, SlateportCity_OceanicMuseum_2F_Movement_ArchieEnter
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntMoveForArchie
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_CameToSeeWhatsTakingSoLong, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OCEANIC_MUSEUM_2F_ARCHIE, SlateportCity_OceanicMuseum_2F_Movement_ArchieApproachPlayer
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_ArchieWarning, MSGBOX_DEFAULT
	closemessage
	savebgm MUS_DUMMY
	fadedefaultbgm
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_OCEANIC_MUSEUM_2F_ARCHIE
	removeobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1
	removeobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2
	fadescreen FADE_FROM_BLACK
	delay 30
	setflag FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_AQUA_GRUNTS
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternThankYouForSavingUs, MSGBOX_DEFAULT
	setvar VAR_0x8004, ITEM_DEVON_PARTS
	call Common_EventScript_PlayerHandedOverTheItem
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternIveGotToGo, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	removeobject LOCALID_OCEANIC_MUSEUM_2F_CAPT_STERN
	setflag FLAG_HIDE_ROUTE_110_TEAM_AQUA
	call_if_eq VAR_REGISTER_BIRCH_STATE, 0, SlateportCity_OceanicMuseum_2F_EventScript_ReadyRegisterBirch
	setflag FLAG_DELIVERED_DEVON_GOODS
	clearflag FLAG_HIDE_ROUTE_116_DEVON_EMPLOYEE
	setflag FLAG_HIDE_RUSTBORO_CITY_DEVON_CORP_3F_EMPLOYEE
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1
	fadescreen FADE_FROM_BLACK
	release
	end

SlateportCity_OceanicMuseum_2F_EventScript_ReadyRegisterBirch::
	setvar VAR_REGISTER_BIRCH_STATE, 1
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_EventScript_SternFaceGrunts::
	applymovement LOCALID_OCEANIC_MUSEUM_2F_CAPT_STERN, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntSouth::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntSouth
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntWest::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntWest
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntSouth:
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntWest:
	walk_up
	walk_left
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

SlateportCity_OceanicMuseum_2F_Movement_Unused:
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_left
	walk_left
	walk_up
	walk_up
	delay_8
	step_end

SlateportCity_OceanicMuseum_2F_Movement_ArchieApproachPlayer:
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_ArchieEnter:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle:
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_FirstGruntEnter:
	walk_down
	walk_right
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_2F_Movement_FirstGruntApproach:
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated:
	lock_facing_direction
	walk_left
	unlock_facing_direction
	step_end

SlateportCity_OceanicMuseum_2F_Movement_SecondGruntEnter:
	walk_down
	step_end

SlateportCity_OceanicMuseum_2F_Movement_SecondGruntApproach:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntMoveForArchie:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_8
	walk_fast_down
	walk_in_place_faster_up
	step_end

SlateportCity_OceanicMuseum_2F_EventScript_WaterQualitySample1::
	msgbox SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample1, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_WaterQualitySample2::
	msgbox SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample2, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_PressureExperiment::
	msgbox SlateportCity_OceanicMuseum_2F_Text_PressureExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_HoennModel::
	msgbox SlateportCity_OceanicMuseum_2F_Text_HoennModel, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_DeepSeawaterDisplay::
	msgbox SlateportCity_OceanicMuseum_2F_Text_DeepSeawaterDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SurfaceSeawaterDisplay::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SurfaceSeawaterDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SSTidalReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SSTidalReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SubmarineReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SubmarineReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SubmersibleReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SumbersibleReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SSAnneReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SSAnneReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron1::
	msgbox SlateportCity_OceanicMuseum_2F_Text_RemindsMeOfAbandonedShip, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron2::
	msgbox SlateportCity_OceanicMuseum_2F_Text_DontRunInMuseum, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron3::
	lock
	faceplayer
	msgbox SlateportCity_OceanicMuseum_2F_Text_WantToRideSubmarine, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_OceanicMuseum_2F_Text_ThankYouForTheParts:
	.string "Sì? Se cerchi Stern, l'hai trovato:\n"
	.string "sono io.\p"
	.string "Ah! Questa dev'essere la Merce Devon\n"
	.string "che ho ordinato al Sig. Petri!\p"
	.string "Grazie mille! Con questo stabilizzatore\n"
	.string "di Varchi possiamo finalmente\l"
	.string "preparare la spedizione.$"

SlateportCity_OceanicMuseum_2F_Text_WellTakeThoseParts:
	.string "Arrr! Alt, fermi tutti!\n"
	.string "Quella merce la prendiamo noi!$"

SlateportCity_OceanicMuseum_2F_Text_SternWhoAreYou:
	.string "Capitan Stern: C-cosa?\n"
	.string "E voi chi sareste?$"

SlateportCity_OceanicMuseum_2F_Text_WereTeamAqua:
	.string "Siamo il Team Omega! Vele al vento!\p"
	.string "Quella merce serve al Capo!\n"
	.string "Chiudi il becco e mollala subito!$"

SlateportCity_OceanicMuseum_2F_Text_Grunt1Defeat:
	.string "Uaaaah!\n"
	.string "Battuto da un ragazzino!$"

SlateportCity_OceanicMuseum_2F_Text_BossGoingToBeFurious:
	.string "Che disastro, che naufragio…\n"
	.string "Il Capo andrà su tutte le furie…$"

SlateportCity_OceanicMuseum_2F_Text_LetMeTakeCareOfThis:
	.string "Bah, mozzo piagnucolone!\n"
	.string "Fatti da parte, ci penso io!$"

SlateportCity_OceanicMuseum_2F_Text_Grunt2Defeat:
	.string "Cosa?!\n"
	.string "Ho perso anch'io?!$"

SlateportCity_OceanicMuseum_2F_Text_MeddlingKid:
	.string "E adesso? Senza la merce siamo\n"
	.string "nei guai fino al collo!\p"
	.string "Arrgh! Non avevo messo in conto\n"
	.string "un ficcanaso tra i piedi!$"

SlateportCity_OceanicMuseum_2F_Text_CameToSeeWhatsTakingSoLong:
	.string "Ciurma! Tutto questo tempo per\n"
	.string "qualche bullone? E vi fate fermare\l"
	.string "da un mozzo?!$"

SlateportCity_OceanicMuseum_2F_Text_ArchieWarning:
	.string "Ahrrr-ha-ha! Allora sei tu il mozzo\n"
	.string "che ci rema contro!\p"
	.string "Io sono Ulisse,\n"
	.string "Capo del Team Omega!\p"
	.string "Perché ti metti in mezzo, eh?\p"
	.string "…No! Non dirmi che sei uno\n"
	.string "dell'Alpha?!\p"
	.string "Macché, non può essere.\n"
	.string "Non porti il mantello rosso.\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Pokémon, persone… Tutta la vita\n"
	.string "viene dal mare.\p"
	.string "E il mare non conosce confini!\n"
	.string "Perché Hoenn dovrebbe averne?\p"
	.string "Il Team Omega spalancherà le porte\n"
	.string "tra i mondi. Un'impresa magnifica, no?\p"
	.string "Ah, lascia stare…\n"
	.string "Sei ancora troppo giovane per capire\l"
	.string "il nostro sogno.\p"
	.string "Ma ti avverto, mozzo: non provare\n"
	.string "più a intralciare i nostri piani.\p"
	.string "Se ci riprovi… te ne pentirai\n"
	.string "amaramente!\p"
	.string "E vedi di non dimenticarlo!$"

SlateportCity_OceanicMuseum_2F_Text_SternThankYouForSavingUs:
	.string "Capitan Stern: Tu sei…\n"
	.string "Ah, giusto, sei {PLAYER}{KUN}…\p"
	.string "Che momento di tensione!\n"
	.string "Grazie per averci salvati!\p"
	.string "Ulisse… Un tempo era un oceanografo\n"
	.string "brillante. Che cosa gli è successo?\p"
	.string "Oh, quasi dimenticavo! Mi hai anche\n"
	.string "portato la Merce Devon!$"

SlateportCity_OceanicMuseum_2F_Text_SternIveGotToGo:
	.string "Capitan Stern: Ops!\n"
	.string "Non c'è tempo da perdere!\p"
	.string "Dobbiamo partire al più presto per\n"
	.string "la spedizione sui fondali.\p"
	.string "Laggiù si apre un Varco che nessuno\n"
	.string "ha mai visto da vicino!\p"
	.string "Grazie ancora, ma ora devo\n"
	.string "proprio scappare!\p"
	.string "Tu fai pure un giro e dai un'occhiata\n"
	.string "al nostro museo.$"

SlateportCity_OceanicMuseum_2F_Text_RemindsMeOfAbandonedShip:
	.string "Ho visto il modellino di una nave.\p"
	.string "Mi ha ricordato la Vecchia Nave che\n"
	.string "si trova vicino a Bluruvia…$"

SlateportCity_OceanicMuseum_2F_Text_DontRunInMuseum:
	.string "Non azzardarti a correre dentro\n"
	.string "il Museo!$"

SlateportCity_OceanicMuseum_2F_Text_WantToRideSubmarine:
	.string "Wow, quel sottomarino è fantastico!\n"
	.string "Voglio farci un giro!$"

SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample1:
	.string "“Campione d'acqua n. 1”, dice\n"
	.string "l'etichetta.\p"
	.string "Il mare è uno solo, eppure l'acqua\n"
	.string "sembra cambiare da zona a zona.$"

SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample2:
	.string "“Campione d'acqua n. 2”, dice\n"
	.string "l'etichetta.\p"
	.string "Anche la salinità del mare cambia\n"
	.string "da zona a zona?$"

SlateportCity_OceanicMuseum_2F_Text_PressureExperiment:
	.string "Una palla di gomma si gonfia e si\n"
	.string "sgonfia.\p"
	.string "“In mare, il peso stesso dell'acqua\n"
	.string "esercita una pressione.\p"
	.string "“In acque basse, la pressione non è\n"
	.string "molto elevata.\p"
	.string "“Negli abissi più profondi, invece,\n"
	.string "può arrivare a decine di migliaia di\l"
	.string "tonnellate su una piccola superficie.”$"

SlateportCity_OceanicMuseum_2F_Text_HoennModel:
	.string "“Plastico della regione di Hoenn”\p"
	.string "È un diorama in miniatura della\n"
	.string "regione di Hoenn.\p"
	.string "Dove sarà Albanova, qui sopra?$"

SlateportCity_OceanicMuseum_2F_Text_DeepSeawaterDisplay:
	.string "È un pannello sulle correnti marine.\p"
	.string "“Vicino al fondale, l'acqua si muove\n"
	.string "a causa delle differenze di fattori\l"
	.string "come la temperatura e la salinità.”$"

SlateportCity_OceanicMuseum_2F_Text_SurfaceSeawaterDisplay:
	.string "È un pannello sulle correnti marine.\p"
	.string "“In superficie, l'acqua di mare scorre\n"
	.string "in correnti spinte dai venti.”$"

SlateportCity_OceanicMuseum_2F_Text_SSTidalReplica:
	.string "“Il traghetto M/N Acqua\p"
	.string "“Modellino in scala della nave in\n"
	.string "costruzione al Cantiere di Stern.”$"

SlateportCity_OceanicMuseum_2F_Text_SubmarineReplica:
	.string "“Esploratore 1\p"
	.string "“Replica del sottomarino ad alte\n"
	.string "prestazioni per esplorare i fondali.”$"

SlateportCity_OceanicMuseum_2F_Text_SumbersibleReplica:
	.string "“Sonda sommergibile\p"
	.string "“Replica di una sonda compatta e senza\n"
	.string "equipaggio per esplorare i fondali.”$"

SlateportCity_OceanicMuseum_2F_Text_SSAnneReplica:
	.string "“M/N Anna\p"
	.string "“Ricostruita da progetti caduti da un\n"
	.string "Varco. Si dice che solchi i mari di\l"
	.string "un altro mondo.”$"
