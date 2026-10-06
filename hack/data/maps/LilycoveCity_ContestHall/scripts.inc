@ All the LOCALID constants used in this file are essentially redundant.
@ They're all used to have the NPC the player talked to turn back so they're facing
@ their original direction (because they're watching/participating in the contests).
@ They could all be substituted for VAR_LAST_TALKED.
LilycoveCity_ContestHall_MapScripts::
	.byte 0

LilycoveCity_ContestHall_EventScript_Boy1::
	msgbox LilycoveCity_ContestHall_Text_TodayWonSmartnessContest, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_Boy2::
	msgbox LilycoveCity_ContestHall_Text_EnteredBunchOfContests, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_Girl::
	msgbox LilycoveCity_ContestHall_Text_ManWhoWonEarlierHadPokeblocks, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_Sailor::
	msgbox LilycoveCity_ContestHall_Text_IsntThisPlaceHumongous, MSGBOX_NPC
	end

LilycoveCity_ContestHall_EventScript_SmartContestMC::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_GiveItBestSmartAppeal, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_MC, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestJudge::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_AreYouEnjoyingThisContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_JUDGE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_EnteredWrongContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_CONTESTANT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_RaisedMonToBeSmart, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_CONTESTANT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_IfMonPullsSmartMoveNext, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_CONTESTANT_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestant4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_DontAppreciateCuteLeechLife, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_CONTESTANT_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience1::
	msgbox LilycoveCity_ContestHall_Text_YoureBeautifulGrandpa, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_AllSeemToUseDifferentMoves, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_AUDIENCE_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_PokemonSmarterThanTrainers, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_AUDIENCE_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_SmartContestAudience4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_StillLoveSmartnessContests, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_SMART_AUDIENCE_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestMC::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_AreYouEnteringBeautyContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_MC, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestJudge::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_EveryPokemonPristineBeauty, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_JUDGE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_EyesWillBeGluedToMyBeauty, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_CONTESTANT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_OverdidGrooming, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_CONTESTANT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_JudgeWontSeeAuroraBeam, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_CONTESTANT_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestant4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_PokemonLooksLikeYoungerMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_CONTESTANT_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestAudience1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_WinBeautyContestMakesMeHappy, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_AUDIENCE_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestAudience2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_GanderAtAllThosePrettyPokemon, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_AUDIENCE_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyContestAudience3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_CantWinOnBeautyAlone, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_BEAUTY_AUDIENCE_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestMC::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_InTheMiddleOfContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_MC, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestJudge::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_SuchCharmingCuteAppeals, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_JUDGE, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant1::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_MyAzurillWasDistracted, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_CONTESTANT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_NeverWonBattleButContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_CONTESTANT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_PetalDanceIsMarvel, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_CONTESTANT_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestant4::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_MyMonAppealSoMuchCuter, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_CONTESTANT_4, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestAudience1::
	lockall
	applymovement LOCALID_CUTE_AUDIENCE_1, Common_Movement_FacePlayer
	waitmovement 0
	msgbox LilycoveCity_ContestHall_Text_MyChildIsInContest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_AUDIENCE_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	delay 25
	msgbox LilycoveCity_ContestHall_Text_ComeOnDear, MSGBOX_DEFAULT
	releaseall
	end

LilycoveCity_ContestHall_EventScript_CuteContestAudience2::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_WantCuteMonOfMyOwn, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_AUDIENCE_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_CuteContestAudience3::
	lock
	faceplayer
	msgbox LilycoveCity_ContestHall_Text_ThatGirlThereIsCutest, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CUTE_AUDIENCE_3, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_ContestHall_EventScript_BeautyStageSign::
	msgbox LilycoveCity_ContestHall_Text_BeautyContestStage, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_EventScript_CuteStageSign::
	msgbox LilycoveCity_ContestHall_Text_CuteContestStage, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_EventScript_SmartStageSign::
	msgbox LilycoveCity_ContestHall_Text_SmartContestStage, MSGBOX_SIGN
	end

LilycoveCity_ContestHall_Text_TodayWonSmartnessContest:
	.string "Questo Pokémon aveva già vinto una\n"
	.string "Gara di Bellezza, qui.\p"
	.string "E indovina un po'? Oggi gli ho fatto\n"
	.string "vincere una Gara di Acume!\p"
	.string "Il mio talento fa paura perfino a me…$"

LilycoveCity_ContestHall_Text_EnteredBunchOfContests:
	.string "Ho partecipato a un sacco di Gare,\n"
	.string "e comincio a capirne i segreti.\p"
	.string "In una Gara di Classe, per esempio,\n"
	.string "le mosse di classe fanno colpo.\p"
	.string "Invece le mosse acute o graziose\n"
	.string "non fanno una gran figura.$"

LilycoveCity_ContestHall_Text_ManWhoWonEarlierHadPokeblocks:
	.string "Il ragazzo che ha vinto prima aveva\n"
	.string "un sacco di Pokémelle diverse.\p"
	.string "Basta averne così tante\n"
	.string "per vincere?$"

LilycoveCity_ContestHall_Text_IsntThisPlaceHumongous:
	.string "Accidenti, che posto gigantesco!\p"
	.string "Che tensione nell'aria… Altro che le\n"
	.string "Arene delle Virtù di provincia!$"

LilycoveCity_ContestHall_Text_GiveItBestSmartAppeal:
	.string "Presentatrice: Pokémon acuti e\n"
	.string "Allenatori, siete pronti?!\p"
	.string "Date il meglio di voi!\n"
	.string "Che l'esibizione abbia inizio!$"

LilycoveCity_ContestHall_Text_AreYouEnjoyingThisContest:
	.string "Giudice: Ti piace questa Gara?\p"
	.string "Torna con tre amici, e potrete\n"
	.string "partecipare tutti a una Gara!$"

LilycoveCity_ContestHall_Text_EnteredWrongContest:
	.string "Ahiahiahi…\n"
	.string "Ho sbagliato Gara.\p"
	.string "Ho iscritto questo Pokémon grintoso\n"
	.string "alla Gara di Acume…\p"
	.string "Forza, stupiscili con uno\n"
	.string "Spaccaroccia… dall'aria intelligente!$"

LilycoveCity_ContestHall_Text_RaisedMonToBeSmart:
	.string "Ho speso tanti anni, e tutta la mia\n"
	.string "saggezza, per crescere questo\l"
	.string "Pokémon acuto e sveglio.\p"
	.string "Non si farà battere dal Pokémon\n"
	.string "di qualche pivellino.\p"
	.string "Moglie mia cara, mi stai guardando?$"

LilycoveCity_ContestHall_Text_IfMonPullsSmartMoveNext:
	.string "Eccolo!\p"
	.string "Se adesso il mio Pokémon sfodera\n"
	.string "una mossa acuta, il pubblico impazzirà!$"

LilycoveCity_ContestHall_Text_DontAppreciateCuteLeechLife:
	.string "Oh, no, no, no!\p"
	.string "La mossa Sanguisuga del mio Zubat è\n"
	.string "così graziosa che l'ho usata e riusata!\p"
	.string "Ma questa gente cattiva non la\n"
	.string "apprezza per niente!$"

LilycoveCity_ContestHall_Text_YoureBeautifulGrandpa:
	.string "Kyaaah! Sei bellissimo, nonno!$"

LilycoveCity_ContestHall_Text_AllSeemToUseDifferentMoves:
	.string "Anche quando gli Allenatori iscrivono\n"
	.string "lo stesso Pokémon, ognuno usa mosse\l"
	.string "diverse per esibirsi.\p"
	.string "Proprio come in lotta, dove ognuno\n"
	.string "ha il suo stile.$"

LilycoveCity_ContestHall_Text_PokemonSmarterThanTrainers:
	.string "Secondo me i Pokémon sembrano più\n"
	.string "acuti dei loro Allenatori. Di molto.$"

LilycoveCity_ContestHall_Text_StillLoveSmartnessContests:
	.string "Alla fine dei conti, io adoro le\n"
	.string "Gare di Acume.\p"
	.string "Quel verde così intellettuale…\n"
	.string "È così… così… di Classe.$"

LilycoveCity_ContestHall_Text_AreYouEnteringBeautyContest:
	.string "Presentatrice: Partecipi anche tu alla\n"
	.string "Gara di Bellezza? In bocca al lupo!$"

LilycoveCity_ContestHall_Text_EveryPokemonPristineBeauty:
	.string "Giudice: Ah, ogni Pokémon qui è un\n"
	.string "modello di bellezza purissima!\p"
	.string "Per poco non mi dimenticavo\n"
	.string "di dare i punteggi!$"

LilycoveCity_ContestHall_Text_EyesWillBeGluedToMyBeauty:
	.string "Un Pokémon così bello…\n"
	.string "Non ne esiste un altro uguale.\p"
	.string "Tutti gli sguardi saranno incollati\n"
	.string "alla mia bellezza.$"

LilycoveCity_ContestHall_Text_OverdidGrooming:
	.string "Uaaaah!\p"
	.string "Ho spazzolato e pettinato il mio\n"
	.string "Pokémon con cura per questa Gara…\p"
	.string "Ma ho esagerato…\n"
	.string "Adesso il suo pelo è tutto arruffato…\p"
	.string "Che cosa faccio?$"

LilycoveCity_ContestHall_Text_JudgeWontSeeAuroraBeam:
	.string "Il mio Raggiaurora è così abbagliante\n"
	.string "che il Giudice non lo vedrà nemmeno.\p"
	.string "Ehm, aspetta un attimo…\n"
	.string "Allora non serve a niente!$"

LilycoveCity_ContestHall_Text_PokemonLooksLikeYoungerMe:
	.string "Questo Pokémon così bello è\n"
	.string "identico a me da giovane.\p"
	.string "Appena l'ho detto, mio marito ha\n"
	.string "sputato il caffè che stava bevendo.\p"
	.string "Ho detto qualcosa di buffo?$"

LilycoveCity_ContestHall_Text_WinBeautyContestMakesMeHappy:
	.string "Sai, quando vinco una Gara di\n"
	.string "Bellezza sono felice. Ma felice\l"
	.string "più del solito, eh!$"

LilycoveCity_ContestHall_Text_GanderAtAllThosePrettyPokemon:
	.string "Uahahahah!\p"
	.string "Ma guarda quanti Pokémon\n"
	.string "uno più bello dell'altro!\l"
	.string "Adoro tutto questo sfarzo!$"

LilycoveCity_ContestHall_Text_CantWinOnBeautyAlone:
	.string "La bellezza da sola non sempre\n"
	.string "basta per vincere.\p"
	.string "Devi curare il tuo Pokémon finché\n"
	.string "non è lucido e splendente come il mio.$"

LilycoveCity_ContestHall_Text_InTheMiddleOfContest:
	.string "Presentatrice: Oh-oh! Salve!\n"
	.string "Siamo nel bel mezzo di una Gara!\p"
	.string "Prima iscriviti al bancone, e poi\n"
	.string "vieni a esibirti, d'accordo?$"

LilycoveCity_ContestHall_Text_SuchCharmingCuteAppeals:
	.string "Giudice: Oh, che esibizioni graziose\n"
	.string "e affascinanti!\p"
	.string "Santo cielo! Che adorabile\n"
	.string "esibizione di Docciascudo!$"

LilycoveCity_ContestHall_Text_MyAzurillWasDistracted:
	.string "Oh, no… Il mio dolce Azurill si è\n"
	.string "distratto per colpa di un altro Pokémon.$"

LilycoveCity_ContestHall_Text_NeverWonBattleButContest:
	.string "Il mio Pokémon non ha mai vinto una\n"
	.string "lotta, ma in una Gara è imbattibile!$"

LilycoveCity_ContestHall_Text_PetalDanceIsMarvel:
	.string "La Petalodanza del mio Pokémon\n"
	.string "è un capolavoro di eleganza.\p"
	.string "Non permetterò a nessuno di\n"
	.string "disturbare la sua esibizione.$"

LilycoveCity_ContestHall_Text_MyMonAppealSoMuchCuter:
	.string "I Pokémon di tutti sono molto graziosi.\p"
	.string "Però, se il mio Pokémon facesse\n"
	.string "un'esibizione graziosa…\p"
	.string "Sono sicura che sarebbe molto più\n"
	.string "grazioso degli altri.$"

LilycoveCity_ContestHall_Text_MyChildIsInContest:
	.string "Mio figlio partecipa a questa Gara.$"

LilycoveCity_ContestHall_Text_ComeOnDear:
	.string "Forza, tesoro, dai tutto!\n"
	.string "Il tuo Pokémon è il migliore!$"

LilycoveCity_ContestHall_Text_ThatGirlThereIsCutest:
	.string "Secondo me quella ragazza laggiù\n"
	.string "è la più carina di tutte.\p"
	.string "Come? Qui si giudicano i Pokémon\n"
	.string "per il loro aspetto?$"

LilycoveCity_ContestHall_Text_WantCuteMonOfMyOwn:
	.string "Ooh, con tutti questi Pokémon carini\n"
	.string "mi è venuta voglia di averne uno mio!\p"
	.string "Vado subito a catturarne qualcuno!$"

LilycoveCity_ContestHall_Text_BeautyContestStage:
	.string "Palco della Gara di Bellezza\n"
	.string "Fatevi incantare dai Pokémon belli!$"

LilycoveCity_ContestHall_Text_CuteContestStage:
	.string "Palco della Gara di Grazia\n"
	.string "Fatevi conquistare dai Pokémon graziosi!$"

LilycoveCity_ContestHall_Text_SmartContestStage:
	.string "Palco della Gara di Acume\n"
	.string "Fatevi stupire dai Pokémon acuti!$"

