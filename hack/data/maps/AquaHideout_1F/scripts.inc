AquaHideout_1F_MapScripts::
	.byte 0

@ The below two entrance guards give hints about what to do to progress the story
AquaHideout_1F_EventScript_HideoutEntranceGrunt1::
	lock
	faceplayer
	goto_if_set FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT, AquaHideout_1F_EventScript_SlateportHint1
	goto_if_set FLAG_RECEIVED_RED_OR_BLUE_ORB, AquaHideout_1F_EventScript_MagmaHideoutHint1
	msgbox AquaHideout_1F_Text_OurBossIsSnatchingSomething, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_MagmaHideoutHint1::
	msgbox AquaHideout_1F_Text_WhereMightMagmaHideoutBe, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_SlateportHint1::
	msgbox AquaHideout_1F_Text_BossWentToJackASubmarine, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_HideoutEntranceGrunt2::
	lock
	faceplayer
	goto_if_set FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT, AquaHideout_1F_EventScript_SlateportHint2
	goto_if_set FLAG_RECEIVED_RED_OR_BLUE_ORB, AquaHideout_1F_EventScript_MagmaHideoutHint2
	msgbox AquaHideout_1F_Text_BossIsOnRoute122, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_MagmaHideoutHint2::
	msgbox AquaHideout_1F_Text_TeamMagmaAtMtChimney, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_SlateportHint2::
	msgbox AquaHideout_1F_Text_BossIsInSlateportCity, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_1, AquaHideout_1F_Text_Grunt1Intro, AquaHideout_1F_Text_Grunt1Defeat, AquaHideout_1F_EventScript_Grunt1Defeated
	msgbox AquaHideout_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_1F_EventScript_Grunt1Defeated::
	msgbox AquaHideout_1F_Text_Grunt1PostBattle, MSGBOX_DEFAULT
	release
	end

AquaHideout_1F_Text_OurBossIsSnatchingSomething:
	.string "Cosa? Cosa? Che vuoi dal\n"
	.string "Team Omega?\p"
	.string "Il Capo non c'è! È salpato per\n"
	.string "arraffare una cosa importantissima!\p"
	.string "… …\n"
	.string "Dov'è andato?\p"
	.string "Arrr-ha-ha! Credi davvero che ti\n"
	.string "spiffero un segreto del genere?$"

AquaHideout_1F_Text_WhereMightMagmaHideoutBe:
	.string "Cosa? Cosa?\n"
	.string "Sei una Recluta del Team Alpha?\p"
	.string "Ho sentito che il Team Alpha vuole\n"
	.string "risvegliare un Pokémon tremendo nel\l"
	.string "suo covo.\p"
	.string "Ma dove si nasconderanno, quelle\n"
	.string "talpe rosse?$"

AquaHideout_1F_Text_BossWentToJackASubmarine:
	.string "Cosa? Cosa? Che vuoi dal\n"
	.string "Team Omega?\p"
	.string "Il Capo non c'è!\n"
	.string "È andato ad abbordare un sottomarino!\p"
	.string "… …\n"
	.string "Dov'è andato?\p"
	.string "Arrr-ha-ha! Credi davvero che ti\n"
	.string "spiffero un segreto del genere?$"

AquaHideout_1F_Text_BossIsOnRoute122:
	.string "Cosa? Cosa? Che vuoi dal\n"
	.string "Team Omega?\p"
	.string "Il Capo non c'è! Ha fatto rotta verso\n"
	.string "il Monte Pira, sul Percorso 122!\p"
	.string "… …\n"
	.string "Perché ci è andato?\p"
	.string "Arrr-ha-ha! Credi davvero che ti\n"
	.string "spiffero un segreto del genere?$"

AquaHideout_1F_Text_TeamMagmaAtMtChimney:
	.string "Cosa? Cosa?\n"
	.string "Sei una Recluta del Team Alpha?\p"
	.string "Ho sentito che il Team Alpha cerca\n"
	.string "un Pokémon tremendo al Monte Camino.\p"
	.string "Ma che razza di Pokémon sarà?$"

AquaHideout_1F_Text_BossIsInSlateportCity:
	.string "Cosa? Cosa? Che vuoi dal\n"
	.string "Team Omega?\p"
	.string "Il Capo non c'è!\n"
	.string "Ha fatto rotta su Porto Selcepoli!\p"
	.string "… …\n"
	.string "Perché ci è andato?\p"
	.string "Arrr-ha-ha! Credi davvero che ti\n"
	.string "spiffero un segreto del genere?$"

AquaHideout_1F_Text_Grunt1Intro:
	.string "Ahiahiahi!\n"
	.string "Tipo sospetto avvistato! Arrr!$"

AquaHideout_1F_Text_Grunt1Defeat:
	.string "Grrrrr…\n"
	.string "Affondato!$"

AquaHideout_1F_Text_Grunt1PostBattle:
	.string "Ho perso per il bene della ciurma,\n"
	.string "ma il mio dovere l'ho fatto…$"

