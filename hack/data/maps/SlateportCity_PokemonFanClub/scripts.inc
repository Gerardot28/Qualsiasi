SlateportCity_PokemonFanClub_MapScripts::
	.byte 0

SlateportCity_PokemonFanClub_EventScript_Chairman::
	lock
	faceplayer
	goto_if_unset FLAG_ENTERED_CONTEST, SlateportCity_PokemonFanClub_EventScript_ChairmanNotEnteredContest
	call_if_unset FLAG_MET_SLATEPORT_FANCLUB_CHAIRMAN, SlateportCity_PokemonFanClub_EventScript_MeetChairman
	switch VAR_SLATEPORT_FAN_CLUB_STATE
	case 0, SlateportCity_PokemonFanClub_EventScript_ChairmanFirstAssessment
	case 1, SlateportCity_PokemonFanClub_EventScript_ChairmanTryAssessPokemon
	case 2, SlateportCity_PokemonFanClub_EventScript_NoMoreScarves
	release
	end

SlateportCity_PokemonFanClub_EventScript_ChairmanFirstAssessment::
	setvar VAR_SLATEPORT_FAN_CLUB_STATE, 1
	msgbox SlateportCity_PokemonFanClub_Text_AllowMeToExamineYourPokemon, MSGBOX_DEFAULT
	goto SlateportCity_PokemonFanClub_EventScript_ChairmanAssessLeadMon
	end

SlateportCity_PokemonFanClub_EventScript_ChairmanTryAssessPokemon::
	setvar VAR_TEMP_2, 0
	call_if_set FLAG_RECEIVED_YELLOW_SCARF, SlateportCity_PokemonFanClub_EventScript_CountReceivedScarf
	call_if_set FLAG_RECEIVED_GREEN_SCARF, SlateportCity_PokemonFanClub_EventScript_CountReceivedScarf
	call_if_set FLAG_RECEIVED_PINK_SCARF, SlateportCity_PokemonFanClub_EventScript_CountReceivedScarf
	call_if_set FLAG_RECEIVED_BLUE_SCARF, SlateportCity_PokemonFanClub_EventScript_CountReceivedScarf
	call_if_set FLAG_RECEIVED_RED_SCARF, SlateportCity_PokemonFanClub_EventScript_CountReceivedScarf
	call_if_eq VAR_TEMP_2, CONTEST_CATEGORIES_COUNT, SlateportCity_PokemonFanClub_EventScript_ReceivedAllScarves
	goto_if_eq VAR_SLATEPORT_FAN_CLUB_STATE, 2, SlateportCity_PokemonFanClub_EventScript_NoMoreScarves
	msgbox SlateportCity_PokemonFanClub_Text_HowIsYourPokemonGrowing, MSGBOX_DEFAULT
	goto SlateportCity_PokemonFanClub_EventScript_ChairmanAssessLeadMon
	end

SlateportCity_PokemonFanClub_EventScript_NoMoreScarves::
	msgbox SlateportCity_PokemonFanClub_Text_NothingElseToGiveYou, MSGBOX_DEFAULT
	release
	end

SlateportCity_PokemonFanClub_EventScript_ChairmanAssessLeadMon::
	msgbox SlateportCity_PokemonFanClub_Text_HmHmISee, MSGBOX_DEFAULT
	setvar VAR_TEMP_1, 0
	call_if_unset FLAG_RECEIVED_YELLOW_SCARF, SlateportCity_PokemonFanClub_EventScript_CheckMonTough
	call_if_unset FLAG_RECEIVED_GREEN_SCARF, SlateportCity_PokemonFanClub_EventScript_CheckMonSmart
	call_if_unset FLAG_RECEIVED_PINK_SCARF, SlateportCity_PokemonFanClub_EventScript_CheckMonCute
	call_if_unset FLAG_RECEIVED_BLUE_SCARF, SlateportCity_PokemonFanClub_EventScript_CheckMonBeauty
	call_if_unset FLAG_RECEIVED_RED_SCARF, SlateportCity_PokemonFanClub_EventScript_CheckMonCool
	bufferleadmonspeciesname STR_VAR_1
	switch VAR_TEMP_1
	case 0, SlateportCity_PokemonFanClub_EventScript_NoHighConditions
	case 1, SlateportCity_PokemonFanClub_EventScript_GiveRedScarf
	case 2, SlateportCity_PokemonFanClub_EventScript_GiveBlueScarf
	case 3, SlateportCity_PokemonFanClub_EventScript_GivePinkScarf
	case 4, SlateportCity_PokemonFanClub_EventScript_GiveGreenScarf
	case 5, SlateportCity_PokemonFanClub_EventScript_GiveYellowScarf
	release
	end

SlateportCity_PokemonFanClub_EventScript_ReceivedAllScarves::
	setvar VAR_SLATEPORT_FAN_CLUB_STATE, 2
	return

SlateportCity_PokemonFanClub_EventScript_CountReceivedScarf::
	addvar VAR_TEMP_2, 1
	return

SlateportCity_PokemonFanClub_EventScript_NoHighConditions::
	msgbox SlateportCity_PokemonFanClub_Text_GiveMonMorePokeblocks, MSGBOX_DEFAULT
	release
	end

SlateportCity_PokemonFanClub_EventScript_GiveRedScarf::
	checkitemspace ITEM_RED_SCARF
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_PokemonFanClub_EventScript_NoRoomForScarf
	msgbox SlateportCity_PokemonFanClub_Text_MonMostImpressiveGiveItThis, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_RED_SCARF
	giveitem ITEM_RED_SCARF
	msgbox SlateportCity_PokemonFanClub_Text_ExplainRedScarf, MSGBOX_DEFAULT
	release
	end

SlateportCity_PokemonFanClub_EventScript_GiveBlueScarf::
	checkitemspace ITEM_BLUE_SCARF
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_PokemonFanClub_EventScript_NoRoomForScarf
	msgbox SlateportCity_PokemonFanClub_Text_MonMostImpressiveGiveItThis, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_BLUE_SCARF
	giveitem ITEM_BLUE_SCARF
	msgbox SlateportCity_PokemonFanClub_Text_ExplainBlueScarf, MSGBOX_DEFAULT
	release
	end

SlateportCity_PokemonFanClub_EventScript_GivePinkScarf::
	checkitemspace ITEM_PINK_SCARF
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_PokemonFanClub_EventScript_NoRoomForScarf
	msgbox SlateportCity_PokemonFanClub_Text_MonMostImpressiveGiveItThis, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_PINK_SCARF
	giveitem ITEM_PINK_SCARF
	msgbox SlateportCity_PokemonFanClub_Text_ExplainPinkScarf, MSGBOX_DEFAULT
	release
	end

SlateportCity_PokemonFanClub_EventScript_GiveGreenScarf::
	checkitemspace ITEM_GREEN_SCARF
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_PokemonFanClub_EventScript_NoRoomForScarf
	msgbox SlateportCity_PokemonFanClub_Text_MonMostImpressiveGiveItThis, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_GREEN_SCARF
	giveitem ITEM_GREEN_SCARF
	msgbox SlateportCity_PokemonFanClub_Text_ExplainGreenScarf, MSGBOX_DEFAULT
	release
	end

SlateportCity_PokemonFanClub_EventScript_GiveYellowScarf::
	checkitemspace ITEM_YELLOW_SCARF
	goto_if_eq VAR_RESULT, FALSE, SlateportCity_PokemonFanClub_EventScript_NoRoomForScarf
	msgbox SlateportCity_PokemonFanClub_Text_MonMostImpressiveGiveItThis, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_YELLOW_SCARF
	giveitem ITEM_YELLOW_SCARF
	msgbox SlateportCity_PokemonFanClub_Text_ExplainYellowScarf, MSGBOX_DEFAULT
	release
	end

SlateportCity_PokemonFanClub_EventScript_NoRoomForScarf::
	msgbox SlateportCity_PokemonFanClub_Text_NoSpaceForReward, MSGBOX_DEFAULT
	release
	end

SlateportCity_PokemonFanClub_EventScript_CheckMonCool::
	specialvar VAR_RESULT, CheckLeadMonCool
	call_if_eq VAR_RESULT, TRUE, SlateportCity_PokemonFanClub_EventScript_SetMonCool
	return

SlateportCity_PokemonFanClub_EventScript_SetMonCool::
	setvar VAR_TEMP_1, 1
	return

SlateportCity_PokemonFanClub_EventScript_CheckMonBeauty::
	specialvar VAR_RESULT, CheckLeadMonBeauty
	call_if_eq VAR_RESULT, TRUE, SlateportCity_PokemonFanClub_EventScript_SetMonBeauty
	return

SlateportCity_PokemonFanClub_EventScript_SetMonBeauty::
	setvar VAR_TEMP_1, 2
	return

SlateportCity_PokemonFanClub_EventScript_CheckMonCute::
	specialvar VAR_RESULT, CheckLeadMonCute
	call_if_eq VAR_RESULT, TRUE, SlateportCity_PokemonFanClub_EventScript_SetMonCute
	return

SlateportCity_PokemonFanClub_EventScript_SetMonCute::
	setvar VAR_TEMP_1, 3
	return

SlateportCity_PokemonFanClub_EventScript_CheckMonSmart::
	specialvar VAR_RESULT, CheckLeadMonSmart
	call_if_eq VAR_RESULT, TRUE, SlateportCity_PokemonFanClub_EventScript_SetMonSmart
	return

SlateportCity_PokemonFanClub_EventScript_SetMonSmart::
	setvar VAR_TEMP_1, 4
	return

SlateportCity_PokemonFanClub_EventScript_CheckMonTough::
	specialvar VAR_RESULT, CheckLeadMonTough
	call_if_eq VAR_RESULT, TRUE, SlateportCity_PokemonFanClub_EventScript_SetMonTough
	return

SlateportCity_PokemonFanClub_EventScript_SetMonTough::
	setvar VAR_TEMP_1, 5
	return

SlateportCity_PokemonFanClub_EventScript_ChairmanNotEnteredContest::
	goto_if_set FLAG_MET_SLATEPORT_FANCLUB_CHAIRMAN, SlateportCity_PokemonFanClub_EventScript_ChairmanEnterContest
	msgbox SlateportCity_PokemonFanClub_Text_MeetChairman, MSGBOX_DEFAULT
	setflag FLAG_MET_SLATEPORT_FANCLUB_CHAIRMAN
	release
	end

SlateportCity_PokemonFanClub_EventScript_ChairmanEnterContest::
	msgbox SlateportCity_PokemonFanClub_Text_LikeToSeeEnteredContestPokemon, MSGBOX_DEFAULT
	release
	end

SlateportCity_PokemonFanClub_EventScript_MeetChairman::
	msgbox SlateportCity_PokemonFanClub_Text_MeetChairman, MSGBOX_DEFAULT
	setflag FLAG_MET_SLATEPORT_FANCLUB_CHAIRMAN
	return

SlateportCity_PokemonFanClub_EventScript_SootheBellWoman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_SOOTHE_BELL, SlateportCity_PokemonFanClub_EventScript_ReceivedSootheBell
	msgbox SlateportCity_PokemonFanClub_Text_ShowMePokemonThatLoveYou, MSGBOX_DEFAULT
	specialvar VAR_RESULT, GetLeadMonFriendshipScore
	goto_if_ge VAR_RESULT, FRIENDSHIP_150_TO_199, SlateportCity_PokemonFanClub_EventScript_GiveSootheBell
	release
	end

SlateportCity_PokemonFanClub_EventScript_GiveSootheBell::
	playse SE_PIN
	applymovement VAR_LAST_TALKED, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_LAST_TALKED, Common_Movement_Delay48
	waitmovement 0
	msgbox SlateportCity_PokemonFanClub_Text_PokemonAdoresYou, MSGBOX_DEFAULT
	giveitem ITEM_SOOTHE_BELL
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_SOOTHE_BELL
	release
	end

SlateportCity_PokemonFanClub_EventScript_ReceivedSootheBell::
	msgbox SlateportCity_PokemonFanClub_Text_TreatPokemonWithLove, MSGBOX_DEFAULT
	release
	end

SlateportCity_PokemonFanClub_EventScript_Man::
	msgbox SlateportCity_PokemonFanClub_Text_PokemonDontLikeFainting, MSGBOX_NPC
	end

SlateportCity_PokemonFanClub_EventScript_Twin::
	msgbox SlateportCity_PokemonFanClub_Text_MonEnjoyedProtein, MSGBOX_NPC
	end

SlateportCity_PokemonFanClub_EventScript_Skitty::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SKITTY, CRY_MODE_NORMAL
	msgbox SlateportCity_PokemonFanClub_Text_Skitty, MSGBOX_DEFAULT
	waitmoncry
	release
	end

SlateportCity_PokemonFanClub_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox SlateportCity_PokemonFanClub_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

SlateportCity_PokemonFanClub_EventScript_Azumarill::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_AZUMARILL, CRY_MODE_NORMAL
	msgbox SlateportCity_PokemonFanClub_Text_Azumarill, MSGBOX_DEFAULT
	waitmoncry
	release
	end

SlateportCity_PokemonFanClub_Text_MeetChairman:
	.string "Ehm-ehm! Sono il Presidente del\n"
	.string "Pokémon Fan Club!\p"
	.string "E in quanto Presidente, sono\n"
	.string "naturalmente il più importante!\p"
	.string "Nessuno mi batte nell'allevare\n"
	.string "Pokémon. Nessuno!\p"
	.string "Dunque, lascia che ti parli\n"
	.string "delle Gare Pokémon.\p"
	.string "Sono eventi in cui si mostrano i\n"
	.string "propri Pokémon al mondo intero!\p"
	.string "Purtroppo si tengono in città lontane,\n"
	.string "e io non posso parteciparvi spesso.\p"
	.string "Ecco perché ci riuniamo qui: per\n"
	.string "sfoggiare i nostri Pokémon e ammirare\l"
	.string "quelli degli altri.\p"
	.string "Ultimamente vediamo persino Pokémon\n"
	.string "arrivati dai Varchi. Che meraviglia!$"

SlateportCity_PokemonFanClub_Text_LikeToSeeEnteredContestPokemon:
	.string "I Pokémon di un Allenatore che ha\n"
	.string "partecipato a una Gara Pokémon…\l"
	.string "Quelli sì che vorrei vederli!$"

SlateportCity_PokemonFanClub_Text_AllowMeToExamineYourPokemon:
	.string "Ehm-ehm! Vedo che hai partecipato\n"
	.string "a una Gara Pokémon!\p"
	.string "Ti prego! Permettimi di esaminare\n"
	.string "come hai cresciuto il tuo Pokémon.\p"
	.string "Non c'è gioia più grande che vedere\n"
	.string "Pokémon cresciuti da altri Allenatori.\p"
	.string "Il musetto felice di un Pokémon\n"
	.string "allevato con le giuste cure…\p"
	.string "Lo sguardo gentile dell'Allenatore\n"
	.string "che l'ha nutrito e cresciuto…\p"
	.string "Al solo pensiero, il mio cuore\n"
	.string "trabocca di gioia!\p"
	.string "Oh! Ti chiedo scusa!\n"
	.string "Perdonami, mi sono dilungato!\p"
	.string "Su, su! Fammi vedere quanto è\n"
	.string "cresciuto il tuo Pokémon!$"

SlateportCity_PokemonFanClub_Text_HowIsYourPokemonGrowing:
	.string "Come sta crescendo il tuo Pokémon?\n"
	.string "Permettimi di esaminarlo.$"

SlateportCity_PokemonFanClub_Text_HmHmISee:
	.string "Mmh, mmh…\n"
	.string "Capisco…$"

SlateportCity_PokemonFanClub_Text_GiveMonMorePokeblocks:
	.string "Mmmh… Non è male, ma non è\n"
	.string "nemmeno granché…\p"
	.string "Tu, in quanto Allenatore, devi\n"
	.string "impegnarti di più con questo Pokémon!\p"
	.string "Per esempio, perché non provi\n"
	.string "a dargli più Pokémelle?$"

SlateportCity_PokemonFanClub_Text_NoSpaceForReward:
	.string "Oh, cielo…\p"
	.string "Il tuo Pokémon sta crescendo\n"
	.string "benissimo, e meriti un premio.\p"
	.string "Purtroppo, però, non hai spazio\n"
	.string "nella Borsa per riceverlo.$"

SlateportCity_PokemonFanClub_Text_MonMostImpressiveGiveItThis:
	.string "Il tuo {STR_VAR_1} sta crescendo\n"
	.string "in modo davvero straordinario!\l"
	.string "È un esemplare magnifico!\p"
	.string "Ma! Se gli dessi questo, crescerebbe\n"
	.string "ancora meglio! Proprio così!$"

SlateportCity_PokemonFanClub_Text_ExplainRedScarf:
	.string "Fai tenere la Fascia Rossa\n"
	.string "a un Pokémon.\p"
	.string "Tutti riconosceranno la classe\n"
	.string "di quel Pokémon!$"

SlateportCity_PokemonFanClub_Text_ExplainBlueScarf:
	.string "Fai tenere la Fascia Blu a un Pokémon.\p"
	.string "La sua bellezza risalterà molto\n"
	.string "più di adesso!$"

SlateportCity_PokemonFanClub_Text_ExplainPinkScarf:
	.string "Fai tenere la Fascia Rosa a un Pokémon.\p"
	.string "Ne farà risaltare ancora di più\n"
	.string "la grazia!$"

SlateportCity_PokemonFanClub_Text_ExplainGreenScarf:
	.string "Fai tenere la Fascia Verde a un Pokémon.\p"
	.string "Metterà in risalto l'acume\n"
	.string "del tuo Pokémon!$"

SlateportCity_PokemonFanClub_Text_ExplainYellowScarf:
	.string "Fai tenere la Fascia Gialla\n"
	.string "a un Pokémon.\p"
	.string "Rafforzerà moltissimo la grinta\n"
	.string "del tuo Pokémon!$"

SlateportCity_PokemonFanClub_Text_NothingElseToGiveYou:
	.string "Mi dispiace, ma non ho nient'altro\n"
	.string "da darti! Proprio niente!\p"
	.string "Del resto, hai il dono di crescere\n"
	.string "splendidi Pokémon senza bisogno\l"
	.string "di alcuno strumento!$"

SlateportCity_PokemonFanClub_Text_ShowMePokemonThatLoveYou:
	.string "Adoro vedere Pokémon che vogliono\n"
	.string "bene ai loro Allenatori.\p"
	.string "I Pokémon avvertono benissimo\n"
	.string "i sentimenti dei loro Allenatori.\p"
	.string "Se tratti il tuo Pokémon con amore\n"
	.string "e cura, lui ti ricambierà.\p"
	.string "Persino quelli giunti da un Varco,\n"
	.string "lontani da casa, si affezionano!\p"
	.string "Quando il tuo Pokémon ti vorrà\n"
	.string "un gran bene, vieni a mostrarmelo.$"

SlateportCity_PokemonFanClub_Text_PokemonAdoresYou:
	.string "Il tuo Pokémon ti adora davvero.\p"
	.string "Per te, Allenatore dal cuore grande,\n"
	.string "un regalo dal Fan Club!$"

SlateportCity_PokemonFanClub_Text_TreatPokemonWithLove:
	.string "I Pokémon avvertono benissimo\n"
	.string "i sentimenti dei loro Allenatori.\p"
	.string "Se tratti il tuo Pokémon con amore\n"
	.string "e cura, lui ti ricambierà.$"

SlateportCity_PokemonFanClub_Text_PokemonDontLikeFainting:
	.string "Se lasci che un Pokémon finisca spesso\n"
	.string "esausto in lotta, se la legherà al dito.\p"
	.string "Pian piano si fiderà sempre meno\n"
	.string "del suo Allenatore.\p"
	.string "In altre parole, di sicuro non\n"
	.string "ti vorrà poi così bene.$"

SlateportCity_PokemonFanClub_Text_MonEnjoyedProtein:
	.string "Ai Pokémon piacerà che si usino\n"
	.string "strumenti su di loro?\p"
	.string "Il mio sembrava felicissimo quando\n"
	.string "gli ho dato una Proteina.$"

SlateportCity_PokemonFanClub_Text_Skitty:
	.string "Skitty: Fffnyaaaah…$"

SlateportCity_PokemonFanClub_Text_Zigzagoon:
	.string "Zigzagoon: Kyuuu…$"

SlateportCity_PokemonFanClub_Text_Azumarill:
	.string "Azumarill: Marimari?$"

