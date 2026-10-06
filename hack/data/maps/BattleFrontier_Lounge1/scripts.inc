BattleFrontier_Lounge1_MapScripts::
	.byte 0

@ NPC that rates pokemon based on their IVs
BattleFrontier_Lounge1_EventScript_Breeder::
	lock
	faceplayer
	call_if_unset FLAG_MET_BATTLE_FRONTIER_BREEDER, BattleFrontier_Lounge1_EventScript_BreederIntro
	call_if_set FLAG_MET_BATTLE_FRONTIER_BREEDER, BattleFrontier_Lounge1_EventScript_AlreadyMetBreeder
	setflag FLAG_MET_BATTLE_FRONTIER_BREEDER
	goto BattleFrontier_Lounge1_EventScript_ChooseMonToShowBreeder
	end

BattleFrontier_Lounge1_EventScript_ChooseMonToShowBreeder::
	chooseboxmon
	waitstate
	goto_if_ne VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge1_EventScript_ShowMonToBreeder
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge1_EventScript_CancelMonSelect
	end

BattleFrontier_Lounge1_EventScript_BreederIntro::
	msgbox BattleFrontier_Lounge1_Text_PokemonBreederIntro, MSGBOX_DEFAULT
	return

BattleFrontier_Lounge1_EventScript_AlreadyMetBreeder::
	msgbox BattleFrontier_Lounge1_Text_LetsLookAtYourPokemon, MSGBOX_DEFAULT
	return

@ BufferVarsForIVRater buffers the following values
@ VAR_0x8005: Sum of the mons IVs
@ VAR_0x8006: Stat id of highest IV stat
@ VAR_0x8007: IV of the highest IV stat
BattleFrontier_Lounge1_EventScript_ShowMonToBreeder::
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, BattleFrontier_Lounge1_EventScript_ShowEggToBreeder
	special BufferVarsForIVRater
	goto_if_le VAR_0x8005, 90, BattleFrontier_Lounge1_EventScript_AverageTotalIVs       @ Average of 15
	goto_if_le VAR_0x8005, 120, BattleFrontier_Lounge1_EventScript_AboveAverageTotalIVs @ Average of 20
	goto_if_le VAR_0x8005, 150, BattleFrontier_Lounge1_EventScript_HighTotalIVs         @ Average of 25
	goto_if_ge VAR_0x8005, 151, BattleFrontier_Lounge1_EventScript_VeryHighTotalIVs     @ Average of > 25
	end

BattleFrontier_Lounge1_EventScript_ShowEggToBreeder::
	msgbox BattleFrontier_Lounge1_Text_EvenICantTell, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_ChooseMonToShowBreeder
	end

@ Comment on the highest IV stat
BattleFrontier_Lounge1_EventScript_HighestIVStat::
	goto_if_eq VAR_0x8006, STAT_HP, BattleFrontier_Lounge1_EventScript_HighestIVHP
	goto_if_eq VAR_0x8006, STAT_ATK, BattleFrontier_Lounge1_EventScript_HighestIVAtk
	goto_if_eq VAR_0x8006, STAT_DEF, BattleFrontier_Lounge1_EventScript_HighestIVDef
	goto_if_eq VAR_0x8006, STAT_SPEED, BattleFrontier_Lounge1_EventScript_HighestIVSpeed
	goto_if_eq VAR_0x8006, STAT_SPATK, BattleFrontier_Lounge1_EventScript_HighestIVSpAtk
	goto_if_eq VAR_0x8006, STAT_SPDEF, BattleFrontier_Lounge1_EventScript_HighestIVSpDef
	end

@ Comment on the highest IV value
BattleFrontier_Lounge1_EventScript_HighestIVValue::
	goto_if_le VAR_0x8007, 15, BattleFrontier_Lounge1_EventScript_HighestIVLow
	goto_if_le VAR_0x8007, 25, BattleFrontier_Lounge1_EventScript_HighestIVMid
	goto_if_le VAR_0x8007, 30, BattleFrontier_Lounge1_EventScript_HighestIVHigh
	goto_if_ge VAR_0x8007, 31, BattleFrontier_Lounge1_EventScript_HighestIVMax
	end

BattleFrontier_Lounge1_EventScript_EndBreederComments::
	release
	end

BattleFrontier_Lounge1_EventScript_AverageTotalIVs::
	msgbox BattleFrontier_Lounge1_Text_AverageAbility, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_HighestIVStat
	end

BattleFrontier_Lounge1_EventScript_AboveAverageTotalIVs::
	msgbox BattleFrontier_Lounge1_Text_BetterThanAverageAbility, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_HighestIVStat
	end

BattleFrontier_Lounge1_EventScript_HighTotalIVs::
	msgbox BattleFrontier_Lounge1_Text_ImpressiveAbility, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_HighestIVStat
	end

BattleFrontier_Lounge1_EventScript_VeryHighTotalIVs::
	msgbox BattleFrontier_Lounge1_Text_OutstandingAbility, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_HighestIVStat
	end

BattleFrontier_Lounge1_EventScript_HighestIVHP::
	msgbox BattleFrontier_Lounge1_Text_BestAspectHP, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_HighestIVValue
	end

BattleFrontier_Lounge1_EventScript_HighestIVAtk::
	msgbox BattleFrontier_Lounge1_Text_BestAspectAtk, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_HighestIVValue
	end

BattleFrontier_Lounge1_EventScript_HighestIVDef::
	msgbox BattleFrontier_Lounge1_Text_BestAspectDef, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_HighestIVValue
	end

BattleFrontier_Lounge1_EventScript_HighestIVSpeed::
	msgbox BattleFrontier_Lounge1_Text_BestAspectSpeed, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_HighestIVValue
	end

BattleFrontier_Lounge1_EventScript_HighestIVSpAtk::
	msgbox BattleFrontier_Lounge1_Text_BestAspectSpAtk, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_HighestIVValue
	end

BattleFrontier_Lounge1_EventScript_HighestIVSpDef::
	msgbox BattleFrontier_Lounge1_Text_BestAspectSpDef, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_HighestIVValue
	end

BattleFrontier_Lounge1_EventScript_HighestIVLow::
	msgbox BattleFrontier_Lounge1_Text_StatRelativelyGood, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_EndBreederComments
	end

BattleFrontier_Lounge1_EventScript_HighestIVMid::
	msgbox BattleFrontier_Lounge1_Text_StatImpressive, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_EndBreederComments
	end

BattleFrontier_Lounge1_EventScript_HighestIVHigh::
	msgbox BattleFrontier_Lounge1_Text_StatOutstanding, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_EndBreederComments
	end

BattleFrontier_Lounge1_EventScript_HighestIVMax::
	msgbox BattleFrontier_Lounge1_Text_StatFlawless, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge1_EventScript_EndBreederComments
	end

BattleFrontier_Lounge1_EventScript_CancelMonSelect::
	msgbox BattleFrontier_Lounge1_Text_NoTimeForMyAdvice, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge1_EventScript_Boy1::
	msgbox BattleFrontier_Lounge1_Text_SaidMyMonIsOutstanding, MSGBOX_NPC
	end

BattleFrontier_Lounge1_EventScript_Boy2::
	msgbox BattleFrontier_Lounge1_Text_DidntDoAnythingSpecialRaisingIt, MSGBOX_NPC
	end

BattleFrontier_Lounge1_Text_PokemonBreederIntro:
	.string "Allevo Pokémon da settant'anni!\n"
	.string "Sono colui che tutti venerano come\l"
	.string "il leggendario Re degli Allevatori!\p"
	.string "Quando avrai la mia esperienza, ti\n"
	.string "basterà un'occhiata per capire\l"
	.string "il potenziale di un Pokémon.\p"
	.string "Ne ho esaminati a migliaia… persino\n"
	.string "quelli arrivati dai Varchi!\p"
	.string "Tu sei un Allenatore, no? Non vuoi\n"
	.string "conoscere il vero potenziale\l"
	.string "dei tuoi Pokémon?\p"
	.string "Su, su!\n"
	.string "Fammi dare un'occhiata ai tuoi Pokémon!$"

BattleFrontier_Lounge1_Text_AverageAbility:
	.string "…Mmh…\p"
	.string "Nel complesso, direi che questo ha\n"
	.string "un potenziale nella media.$"

BattleFrontier_Lounge1_Text_BetterThanAverageAbility:
	.string "…Mmh…\p"
	.string "Nel complesso, direi che questo ha\n"
	.string "un potenziale sopra la media.$"

BattleFrontier_Lounge1_Text_ImpressiveAbility:
	.string "…Mmh…\p"
	.string "Nel complesso, direi che questo ha\n"
	.string "un potenziale davvero notevole!$"

BattleFrontier_Lounge1_Text_OutstandingAbility:
	.string "…Mmh…\p"
	.string "Nel complesso, direi che questo ha\n"
	.string "un potenziale straordinario!$"

BattleFrontier_Lounge1_Text_BestAspectHP:
	.string "A proposito, il suo punto di forza,\n"
	.string "direi, sono i PS…$"

BattleFrontier_Lounge1_Text_BestAspectAtk:
	.string "A proposito, il suo punto di forza,\n"
	.string "direi, è l'Attacco…$"

BattleFrontier_Lounge1_Text_BestAspectDef:
	.string "A proposito, il suo punto di forza,\n"
	.string "direi, è la Difesa…$"

BattleFrontier_Lounge1_Text_BestAspectSpAtk:
	.string "A proposito, il suo punto di forza,\n"
	.string "direi, è l'Attacco Speciale…$"

BattleFrontier_Lounge1_Text_BestAspectSpDef:
	.string "A proposito, il suo punto di forza,\n"
	.string "direi, è la Difesa Speciale…$"

BattleFrontier_Lounge1_Text_BestAspectSpeed:
	.string "A proposito, il suo punto di forza,\n"
	.string "direi, è la Velocità…$"

BattleFrontier_Lounge1_Text_StatRelativelyGood:
	.string "Una statistica piuttosto buona.\n"
	.string "…Mmh… Questo è il mio verdetto.$"

BattleFrontier_Lounge1_Text_StatImpressive:
	.string "Una statistica davvero notevole.\n"
	.string "…Mmh… Questo è il mio verdetto.$"

BattleFrontier_Lounge1_Text_StatOutstanding:
	.string "Una statistica eccezionale!\n"
	.string "…Mmh… Questo è il mio verdetto.$"

BattleFrontier_Lounge1_Text_StatFlawless:
	.string "È impeccabile! Pura perfezione!\n"
	.string "…Mmh… Questo è il mio verdetto.$"

BattleFrontier_Lounge1_Text_NoTimeForMyAdvice:
	.string "Come?\n"
	.string "Non hai tempo per i miei consigli?\p"
	.string "Bisogna sempre imparare dall'esperienza\n"
	.string "di chi ha più anni di te!$"

@ Unused
BattleFrontier_Lounge1_Text_HaveBusinessNeedsTending:
	.string "Sì, che c'è adesso?\p"
	.string "Ho delle faccende da sbrigare!\n"
	.string "Rimandiamo alla prossima volta!$"

BattleFrontier_Lounge1_Text_LetsLookAtYourPokemon:
	.string "Ah, giovanotto! Sei curioso di sapere\n"
	.string "che potenziale hanno i tuoi Pokémon?\p"
	.string "Su, su!\n"
	.string "Fammi dare un'occhiata ai tuoi Pokémon!$"

BattleFrontier_Lounge1_Text_EvenICantTell:
	.string "Sarò anche un esperto, ma non posso\n"
	.string "giudicare un Pokémon ancora nell'Uovo!\p"
	.string "Fammi vedere un Pokémon!\n"
	.string "Un Pokémon vero, non un Uovo!$"

BattleFrontier_Lounge1_Text_SaidMyMonIsOutstanding:
	.string "Dice che il mio Pokémon è eccezionale!\n"
	.string "Ho fatto bene a crescerlo con cura!$"

BattleFrontier_Lounge1_Text_DidntDoAnythingSpecialRaisingIt:
	.string "Dice che il mio Pokémon è eccezionale!\n"
	.string "Ma non ho fatto niente di speciale…\l"
	.string "Sarà merito del Varco da cui è uscito?$"

