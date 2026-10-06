RustboroCity_Flat1_2F_MapScripts::
	.byte 0

RustboroCity_Flat1_2F_EventScript_WaldasDad::
	lock
	faceplayer
	specialvar VAR_RESULT, TryBufferWaldaPhrase
	goto_if_eq VAR_RESULT, FALSE, RustboroCity_Flat1_2F_EventScript_WaldasDadFirstPhrase
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Flat1_2F_EventScript_WaldasDadNewPhrase

RustboroCity_Flat1_2F_EventScript_GivePhrase::
	special DoWaldaNamingScreen
	goto_if_eq VAR_0x8004, 1, RustboroCity_Flat1_2F_EventScript_CancelGivePhrase
	goto_if_eq VAR_0x8004, 2, RustboroCity_Flat1_2F_EventScript_CancelGiveFirstPhrase
	specialvar VAR_RESULT, TryGetWallpaperWithWaldaPhrase
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Flat1_2F_EventScript_WaldaLikesPhrase
	goto_if_eq VAR_RESULT, FALSE, RustboroCity_Flat1_2F_EventScript_WaldaDoesntLikePhrase
	end

RustboroCity_Flat1_2F_EventScript_WaldasDadFirstPhrase::
	msgbox RustboroCity_Flat1_2F_Text_HelloDoYouKnowFunnyPhrase, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase
	msgbox RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion, MSGBOX_DEFAULT
	goto RustboroCity_Flat1_2F_EventScript_GivePhrase

RustboroCity_Flat1_2F_EventScript_WaldasDadNewPhrase::
	msgbox RustboroCity_Flat1_2F_Text_BeenSayingXDoYouKnowBetterPhrase, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase
	msgbox RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion, MSGBOX_DEFAULT
	goto RustboroCity_Flat1_2F_EventScript_GivePhrase

RustboroCity_Flat1_2F_EventScript_DeclineGivePhrase::
	msgbox RustboroCity_Flat1_2F_Text_OhIsThatRight, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_CancelGivePhrase::
	msgbox RustboroCity_Flat1_2F_Text_OhYouDontKnowAny, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_CancelGiveFirstPhrase::
	msgbox RustboroCity_Flat1_2F_Text_ThinkOfMyOwnPhrase, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll2, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldaLikesPhrase::
	msgbox RustboroCity_Flat1_2F_Text_LetsGiveItATry2, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_OhShesLaughing, MSGBOX_DEFAULT
	applymovement LOCALID_WALDAS_DAD, Common_Movement_FacePlayer
	waitmovement 0
	msgbox RustboroCity_Flat1_2F_Text_ThankYouIllGiveYouWallpaper, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldaDoesntLikePhrase::
	msgbox RustboroCity_Flat1_2F_Text_LetsGiveItATry, MSGBOX_DEFAULT
	call RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda
	msgbox RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat1_2F_EventScript_WaldasDadFaceWalda::
	turnobject LOCALID_WALDAS_DAD, DIR_EAST
	return

RustboroCity_Flat1_2F_EventScript_WaldasMom::
	msgbox RustboroCity_Flat1_2F_Text_ComingUpWithMealsIsHard, MSGBOX_NPC
	end

RustboroCity_Flat1_2F_EventScript_PokeDoll::
	msgbox RustboroCity_Flat1_2F_Text_ItsAPokemonPlushDoll, MSGBOX_SIGN
	end

RustboroCity_Flat1_2F_Text_ComingUpWithMealsIsHard:
	.string "Uff, che fatica, ogni giorno…\p"
	.string "Cosa c'è di faticoso?\n"
	.string "E me lo chiedi pure?\p"
	.string "Decidere cosa cucinare,\n"
	.string "tutti i santi giorni!\p"
	.string "Inventarsi un pasto diverso ogni\n"
	.string "giorno non è affatto facile.$"

RustboroCity_Flat1_2F_Text_HelloDoYouKnowFunnyPhrase:
	.string "Oh, ciao!\n"
	.string "Benvenuto in casa Pepe.\p"
	.string "Ho una domanda per te.\n"
	.string "Hai mai fatto da babysitter?\p"
	.string "Vedi, sono un papà alle prime armi:\n"
	.string "crescere una bimba è tutto nuovo.\p"
	.string "E ho un problema: la mia piccola\n"
	.string "Walda non ride abbastanza.\p"
	.string "Credo che riderebbe, se le dicessi\n"
	.string "qualcosa di buffo.\p"
	.string "Conosci una parola o una frase\n"
	.string "divertente da suggerirmi?$"

RustboroCity_Flat1_2F_Text_BeenSayingXDoYouKnowBetterPhrase:
	.string "Ultimamente, per farla ridere,\n"
	.string "le dico “{STR_VAR_1}”.\p"
	.string "Conosci una parola o una frase\n"
	.string "che funzioni meglio?$"

RustboroCity_Flat1_2F_Text_WonderfulLetsHearSuggestion:
	.string "Oh, magnifico!\n"
	.string "Allora, sentiamo il tuo suggerimento.$"

RustboroCity_Flat1_2F_Text_OhIsThatRight:
	.string "Ah, no?\p"
	.string "Beh, se ti viene in mente qualcosa\n"
	.string "di buffo, sono tutto orecchi.$"

RustboroCity_Flat1_2F_Text_LetsGiveItATry2:
	.string "Ah, capisco.\n"
	.string "Allora proviamo, d'accordo?$"

RustboroCity_Flat1_2F_Text_OhShesLaughing:
	.string "{STR_VAR_1}.\n"
	.string "{STR_VAR_1}.\p"
	.string "Oh, sì! Sta ridendo!\n"
	.string "Sono felice quanto lei!$"

RustboroCity_Flat1_2F_Text_LetsGiveItATry:
	.string "Ah, capisco.\n"
	.string "Allora proviamo, d'accordo?$"

RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll:
	.string "{STR_VAR_1}.\n"
	.string "{STR_VAR_1}.\p"
	.string "Mmh… Non sorride per niente.\n"
	.string "Forse Walda è una bimba molto seria…$"

RustboroCity_Flat1_2F_Text_ThinkOfMyOwnPhrase:
	.string "Ah, allora non conosci frasi buffe.\n"
	.string "Dovrò inventarmene una io.\p"
	.string "Mmh…\n"
	.string "Che ne dici di “{STR_VAR_1}”?\l"
	.string "Vediamo se funziona.$"

RustboroCity_Flat1_2F_Text_ShesNotSmilingAtAll2:
	.string "{STR_VAR_1}.\n"
	.string "{STR_VAR_1}.\p"
	.string "Mmh… Non sorride per niente.\n"
	.string "Forse Walda è una bimba molto seria…$"

RustboroCity_Flat1_2F_Text_OhYouDontKnowAny:
	.string "Ah, allora non conosci frasi buffe.\n"
	.string "Proverò a farla ridere con la frase\l"
	.string "che usavo prima.\p"
	.string "Comunque, se ti viene un'idea,\n"
	.string "non esitare a dirmela, d'accordo?$"

RustboroCity_Flat1_2F_Text_ThankYouIllGiveYouWallpaper:
	.string "Grazie!\p"
	.string "Grazie a te, la mia adorata Walda\n"
	.string "ha riso!\p"
	.string "Sai, non sembrerò granché, ma sono\n"
	.string "uno dei migliori ricercatori della\l"
	.string "Devon S.p.A.\p"
	.string "Lascia che ti ricambi il favore.\p"
	.string "Ecco: aggiungerò nuovi sfondi per\n"
	.string "i Box del Sistema Memoria Pokémon\l"
	.string "del PC.\p"
	.string "Nel menu degli sfondi, seleziona\n"
	.string "“Amici”.\p"
	.string "Così potrai usare i nuovi sfondi.$"

RustboroCity_Flat1_2F_Text_ItsAPokemonPlushDoll:
	.string "È un peluche a forma di Pokémon!$"

