SafariZone_EventScript_OutOfBallsMidBattle::
#if IS_FRLG
	setvar VAR_MAP_SCENE_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 3
#else
	setvar VAR_SAFARI_ZONE_STATE, 1
#endif
	special ExitSafariMode
#if IS_FRLG
	setwarp MAP_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 4, 1
#else
	setwarp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 2, 5
#endif
	end

SafariZone_EventScript_Exit::
#if IS_FRLG
	setvar VAR_MAP_SCENE_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 1
#else
	setvar VAR_SAFARI_ZONE_STATE, 1
#endif
	special ExitSafariMode
#if IS_FRLG
	warp MAP_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE, 4, 1
#else
	warp MAP_ROUTE121_SAFARI_ZONE_ENTRANCE, 2, 5
#endif
	waitstate
	end

SafariZone_EventScript_RetirePrompt::
	lockall
	msgbox SafariZone_Text_WouldYouLikeToExit, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SafariZone_EventScript_Retire
	releaseall
	end

SafariZone_EventScript_Retire::
	goto SafariZone_EventScript_Exit

SafariZone_EventScript_TimesUp::
	lockall
	playse SE_DING_DONG
	message SafariZone_Text_TimesUp
	waitmessage
	waitbuttonpress
	releaseall
	goto SafariZone_EventScript_Exit

SafariZone_EventScript_OutOfBalls::
	lockall
	playse SE_DING_DONG
	message SafariZone_Text_OutOfBalls
	waitmessage
	waitbuttonpress
	releaseall
	goto SafariZone_EventScript_Exit

EventScript_PokeBlockFeeder::
	lockall
	special GetPokeblockFeederInFront
	goto_if_ne VAR_RESULT, 0xFFFF, SafariZone_EventScript_PokeblockPresent
	msgbox SafariZone_Text_PlacePokeblockOnFeeder, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SafariZone_EventScript_ChoosePokeblock
	releaseall
	end

SafariZone_EventScript_ChoosePokeblock::
	fadescreen FADE_TO_BLACK
	special OpenPokeblockCaseOnFeeder
	goto_if_ne VAR_RESULT, 0xFFFF, SafariZone_EventScript_PokeblockPlaced
#ifdef BUGFIX
	releaseall  @ Only gets called from EventScript_PokeBlockFeeder which uses lockall.
#endif
	end

SafariZone_EventScript_PokeblockPlaced::
	message SafariZone_Text_PokeblockWasPlaced
	waitmessage
	waitbuttonpress
	releaseall
	end

SafariZone_EventScript_PokeblockPresent::
	message SafariZone_Text_PokeblockStillHere
	waitmessage
	waitbuttonpress
	releaseall
	end

SafariZone_Text_WouldYouLikeToExit:
	.string "Vuoi uscire subito dalla\n"
	.string "Zona Safari?$"

SafariZone_Text_TimesUp:
	.string "Din-don! Tempo scaduto!\n"
	.string "La tua partita Safari è finita.$"

SafariZone_Text_OutOfBalls:
	.string "Hai finito le Safari Ball!\n"
	.string "La tua partita Safari è finita.$"

SafariZone_Text_PlacePokeblockOnFeeder:
	.string "Vuoi mettere una Pokémella\n"
	.string "nella Mangiatoia?$"

SafariZone_Text_PokeblockStillHere:
	.string "La {STR_VAR_1} che avevi\n"
	.string "lasciato è ancora qui.$"

SafariZone_Text_PokeblockWasPlaced:
	.string "Hai messo la {STR_VAR_1}\n"
	.string "nella Mangiatoia.$"

Route121_SafariZoneEntrance_Text_WelcomeToSafariZone:
	.string "Benvenuto nella Zona Safari!\p"
	.string "Qui puoi ammirare tanti Pokémon\n"
	.string "che a Hoenn si vedono di rado.\p"
	.string "Molti sono sbucati da qualche Varco\n"
	.string "e hanno trovato casa proprio qui.\p"
	.string "Li vedrai allo stato brado, liberi\n"
	.string "nel loro ambiente naturale.\p"
	.string "E non è tutto: le porte sono aperte\n"
	.string "a tutti gli Allenatori di Pokémon.\p"
	.string "Puoi catturare tutti i Pokémon che\n"
	.string "vuoi e tenerli con te!\p"
	.string "Entra e divertiti nella Zona Safari!$"

Route121_SafariZoneEntrance_Text_WelcomeFirstTime:
	.string "Benvenuto nella Zona Safari!\n"
	.string "È la prima volta che vieni qui?$"

Route121_SafariZoneEntrance_Text_ComeInAndEnjoy:
	.string "Entra e divertiti nella Zona Safari!$"

Route121_SafariZoneEntrance_Text_FirstTimeInfo:
	.string "Quando entri nella Zona Safari,\n"
	.string "ricevi 30 Safari Ball per\l"
	.string "catturare i Pokémon.\p"
	.string "La partita Safari finisce quando hai\n"
	.string "esaurito le Safari Ball o quando\l"
	.string "hai fatto 500 passi.\p"
	.string "Entra e divertiti nella Zona Safari!$"

Route121_SafariZoneEntrance_Text_WouldYouLikeToPlay:
	.string "Benvenuto nella Zona Safari!\p"
	.string "Catture a volontà per soli ¥500!\n"
	.string "Vuoi fare una partita Safari?$"

Route121_SafariZoneEntrance_Text_PlayAnotherTime:
	.string "D'accordo.\n"
	.string "Torna a trovarci quando vuoi!$"

Route121_SafariZoneEntrance_Text_NotEnoughMoney:
	.string "Non hai abbastanza soldi.\n"
	.string "Mi dispiace.$"

Route121_SafariZoneEntrance_Text_ThatWillBe500Please:
	.string "Sono ¥500, grazie.$"

Route121_SafariZoneEntrance_Text_HereAreYourSafariBalls:
	.string "Ecco le tue Safari Ball.$"

Route121_SafariZoneEntrance_Text_Received30SafariBalls:
	.string "{PLAYER} ha ricevuto 30 Safari Ball.$"

Route121_SafariZoneEntrance_Text_PleaseEnjoyYourself:
	.string "Ti avviseremo noi quando la partita\n"
	.string "sarà finita.\p"
	.string "Fino ad allora, divertiti!\n"
	.string "Via, verso la tua avventura selvaggia!$"

Route121_SafariZoneEntrance_Text_PCIsFull:
	.string "Un momento!\n"
	.string "Il Box del tuo PC è pieno.$"

Route121_SafariZoneEntrance_Text_YouNeedPokeblockCase:
	.string "Un momento!\n"
	.string "Non hai un Porta Pokémelle.\p"
	.string "La tua partita Safari sarà molto più\n"
	.string "fruttuosa se usi le Pokémelle.\p"
	.string "Torna quando avrai un Porta Pokémelle.\p"
	.string "Puoi averne uno al Salone Gare\n"
	.string "di Porto Alghepoli.$"

SafariZone_South_Text_StillHaveTimeExit:
	.string "Hai ancora tempo. Vuoi uscire\n"
	.string "dalla Zona Safari adesso?$"

SafariZone_South_Text_EnjoyTheRestOfYourAdventure:
	.string "Allora goditi il resto della tua\n"
	.string "avventura selvaggia!$"

SafariZone_South_Text_ExitEarlyThankYouForPlaying:
	.string "D'accordo.\p"
	.string "Riprendo le Safari Ball che ti\n"
	.string "sono rimaste.\p"
	.string "Grazie per aver giocato.\n"
	.string "Speriamo di rivederti presto!$"

SafariZone_South_Text_GoodLuck:
	.string "Buona fortuna!\p"
	.string "Se ti serve qualcosa, non esitare\n"
	.string "a chiedere a me!$"

SafariZone_South_Text_Boy:
	.string "Lo sapevi?\p"
	.string "Se metti una Pokémella in quella\n"
	.string "cassetta quadrata, arrivano i Pokémon!$"

SafariZone_South_Text_Man:
	.string "Vorrei spingermi più in là, ma ho\n"
	.string "dimenticato a casa la Bici.\p"
	.string "Qualcosa mi dice che nelle zone più\n"
	.string "remote vivono Pokémon rari.$"

SafariZone_Southwest_Text_Woman:
	.string "A volte, quando lancio una Pokémella\n"
	.string "a un Pokémon, lui la ignora.\p"
	.string "Anche i Pokémon hanno i loro gusti\n"
	.string "in fatto di cibo?$"

SafariZone_Northwest_Text_Man:
	.string "Ansf… Ansf…\n"
	.string "Sono… arrivato fin qui… ma…\p"
	.string "Sono esausto… Non ho più la forza\n"
	.string "di catturare Pokémon…$"

SafariZone_North_Text_Fisherman:
	.string "Sono in missione: cerco Pokémon\n"
	.string "d'Acqua mai visti a Hoenn.\p"
	.string "Dicono che uno sia sbucato da un\n"
	.string "Varco sul fondo del lago…\p"
	.string "Sai per caso dov'è il lago?$"

SafariZone_North_Text_Man:
	.string "Catturerò un sacco di Pokémon rari\n"
	.string "e li scambierò con i miei amici!$"

SafariZone_South_Text_Youngster:
	.string "Ho messo una Pokémella nella\n"
	.string "Mangiatoia, ma è sparita.\p"
	.string "Forse un Pokémon se l'è mangiata\n"
	.string "senza che me ne accorgessi.$"

Route121_SafariZoneEntrance_Text_TrainerTip:
	.string "Consigli dalla Zona Safari!\p"
	.string "Lancia Pokémelle ai Pokémon selvatici:\n"
	.string "saranno meno propensi a fuggire.$"

SafariZone_Southwest_Text_RestHouseSign:
	.string "“Un po' di sollievo ai piedi stanchi.”\n"
	.string "Area Ristoro$"

SafariZone_RestHouse_Text_Youngster:
	.string "Non ho Pokémelle, ma ho catturato\n"
	.string "parecchi Pokémon lo stesso.\p"
	.string "Le probabilità di cattura aumentano\n"
	.string "se ti avvicini al Pokémon prima\l"
	.string "di lanciare la Safari Ball.$"

SafariZone_RestHouse_Text_PsychicM:
	.string "Con le Pokémelle, i Pokémon selvatici\n"
	.string "non scappano via così in fretta.\p"
	.string "Ma è inutile darle ai Pokémon che\n"
	.string "già di loro non fuggono facilmente.$"

SafariZone_RestHouse_Text_FatMan:
	.string "Se metti una Pokémella nella\n"
	.string "Mangiatoia, attiri i Pokémon.\p"
	.string "Credo che certe Pokémelle attirino\n"
	.string "i Pokémon di una certa Natura.$"

SafariZone_South_Text_AreaOffLimits1:
	.string "Quest'area è ancora in costruzione.\n"
	.string "Vietato l'accesso, mi spiace!$"

SafariZone_Southeast_Text_ExpansionIsFinished:
	.string "I lavori di ampliamento della Zona\n"
	.string "Safari sono finalmente conclusi.\p"
	.string "Dai Varchi arrivano sempre più\n"
	.string "Pokémon: serviva spazio per tutti!\p"
	.string "Speriamo che la nuova area ti piaccia!$"

SafariZone_South_Text_AreaOffLimits2:
	.string "Lavori in corso, qui si scava ancora!\n"
	.string "L'accesso è vietato, mi spiace!$"

SafariZone_Southeast_Text_LittleGirl:
	.string "Uau! Evviva! Non avevo mai visto\n"
	.string "nessuno di questi Pokémon!$"

SafariZone_Southeast_Text_FatMan:
	.string "I Pokémon di quest'area sono tutti\n"
	.string "nuovi per me.\p"
	.string "E posso pure catturarli, questi\n"
	.string "Pokémon rari! Troppo forte!$"

SafariZone_Southeast_Text_RichBoy:
	.string "Questi Pokémon non sono di Hoenn.\n"
	.string "Scommetto che vengono da un Varco!$"

SafariZone_Northeast_Text_Boy:
	.string "Mi restano solo un paio di Safari Ball.\p"
	.string "Non riesco proprio a decidere\n"
	.string "cosa catturare.$"

SafariZone_Northeast_Text_Woman:
	.string "Dicono che qui viva un Pikachu\n"
	.string "arrivato da un Varco. Dove sarà?$"

SafariZone_Northeast_Text_Girl:
	.string "Uffa!\n"
	.string "Non riesco a prendere niente!\p"
	.string "Se non catturo qualcosa, avrò\n"
	.string "sprecato i soldi del biglietto!$"
