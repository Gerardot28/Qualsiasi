MauvilleCity_PokemonCenter_1F_EventScript_MauvilleOldMan::
	special Script_GetCurrentMauvilleMan
	switch VAR_RESULT
	case MAUVILLE_MAN_BARD, MauvilleCity_PokemonCenter_1F_EventScript_Bard
	case MAUVILLE_MAN_HIPSTER, MauvilleCity_PokemonCenter_1F_EventScript_Hipster
	case MAUVILLE_MAN_TRADER, MauvilleCity_PokemonCenter_1F_EventScript_Trader
	case MAUVILLE_MAN_STORYTELLER, MauvilleCity_PokemonCenter_1F_EventScript_Storyteller
	case MAUVILLE_MAN_GIDDY, MauvilleCity_PokemonCenter_1F_EventScript_Giddy
	end

@ Bard
MauvilleCity_PokemonCenter_1F_EventScript_Bard::
	lock
	faceplayer
	msgbox MauvilleCity_PokemonCenter_1F_Text_WouldYouLikeToHearMySong, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_PokemonCenter_1F_EventScript_PlaySong
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineSong
	end

MauvilleCity_PokemonCenter_1F_EventScript_PlaySong::
	setvar VAR_0x8004, FALSE  @ Play his current song
	special PlayBardSong
	delay 60
	special HasBardSongBeenChanged
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_PokemonCenter_1F_EventScript_AskToWriteLyrics
	msgbox MauvilleCity_PokemonCenter_1F_Text_WishICouldPlaySongForOthers, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_DeclineSong::
	msgbox MauvilleCity_PokemonCenter_1F_Text_BardFeelingTheBlues1, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_AskToWriteLyrics::
	msgbox MauvilleCity_PokemonCenter_1F_Text_WouldYouLikeToWriteSomeLyrics, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_PokemonCenter_1F_EventScript_WriteLyrics
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineWritingLyrics
	end

MauvilleCity_PokemonCenter_1F_EventScript_WriteLyrics::
	setvar VAR_0x8004, EASY_CHAT_TYPE_BARD_SONG
	call Common_ShowEasyChatScreen
	lock
	faceplayer
	goto_if_eq VAR_RESULT, 0, MauvilleCity_PokemonCenter_1F_EventScript_DeclineWritingLyrics
	msgbox MauvilleCity_PokemonCenter_1F_Text_LetMeSingItForYou, MSGBOX_DEFAULT
	setvar VAR_0x8004, TRUE  @ Play the new song
	special PlayBardSong
	delay 60
	msgbox MauvilleCity_PokemonCenter_1F_Text_ThatHowYouWantedSongToGo, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_WriteLyrics
	special SaveBardSongLyrics
	msgbox MauvilleCity_PokemonCenter_1F_Text_IllSingThisSongForAWhile, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_DeclineWritingLyrics::
	msgbox MauvilleCity_PokemonCenter_1F_Text_BardFeelingTheBlues2, MSGBOX_DEFAULT
	release
	end

@ Hipster
MauvilleCity_PokemonCenter_1F_EventScript_Hipster::
	lock
	faceplayer
	setflag FLAG_UNLOCKED_TRENDY_SAYINGS
	msgbox MauvilleCity_PokemonCenter_1F_Text_TeachWhatsHipAndHappening, MSGBOX_DEFAULT
	special HasHipsterTaughtWord
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_PokemonCenter_1F_EventScript_TryTeachWord
	msgbox MauvilleCity_PokemonCenter_1F_Text_IAlreadyTaughtYou, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_TryTeachWord::
	special HipsterTryTeachWord
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_TeachWord
	msgbox MauvilleCity_PokemonCenter_1F_Text_IveGotNothingNewToTeach, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_TeachWord::
	msgbox MauvilleCity_PokemonCenter_1F_Text_HaveYouHeardOfWord, MSGBOX_DEFAULT
	special SetHipsterTaughtWord
	release
	end

@ Trader
MauvilleCity_PokemonCenter_1F_Text_WantToTradeDecor:
	.string "Ciao, sono il Rigattiere!\n"
	.string "Ti va di scambiare decorazioni?$"

MauvilleCity_PokemonCenter_1F_Text_TraderFeelingTheBlues:
	.string "Oh…\n"
	.string "Mi hai fatto venire la malinconia…$"

MauvilleCity_PokemonCenter_1F_Text_WeveAlreadyTraded:
	.string "Ma noi due il nostro scambio di\n"
	.string "decorazioni l'abbiamo già fatto!$"

MauvilleCity_PokemonCenter_1F_Text_PickADecorItem:
	.string "Se tra le mie decorazioni c'è\n"
	.string "qualcosa che ti piace, dimmelo pure.$"

MauvilleCity_PokemonCenter_1F_Text_YouDontWantAnything:
	.string "Non vuoi niente?\n"
	.string "Mi sento proprio di troppo…$"

MauvilleCity_PokemonCenter_1F_Text_OnceBelongedToPlayerDoYouWantIt:
	.string "Quella decorazione un tempo era\n"
	.string "di {STR_VAR_1}.\p"
	.string "La vuoi?$"

MauvilleCity_PokemonCenter_1F_Text_YouDontHaveAnyDecor:
	.string "Ehm… Un attimo. Tu non hai\n"
	.string "nemmeno una decorazione!$"

MauvilleCity_PokemonCenter_1F_Text_PickTheDecorToTrade:
	.string "D'accordo, scegli la decorazione\n"
	.string "che mi darai in cambio.$"

MauvilleCity_PokemonCenter_1F_Text_YouDontWantToTrade:
	.string "Non vuoi fare scambi con me?\n"
	.string "Mi sento proprio di troppo…$"

MauvilleCity_PokemonCenter_1F_Text_YouveNoRoomForThis:
	.string "Hai già il massimo di decorazioni\n"
	.string "di tipo {STR_VAR_2}. Non c'è posto!$"

MauvilleCity_PokemonCenter_1F_Text_SoWellTradeTheseDecor:
	.string "Allora ti cedo {STR_VAR_3}\n"
	.string "in cambio di {STR_VAR_2}. Ci stai?$"

MauvilleCity_PokemonCenter_1F_Text_ThatDecorIsInUse:
	.string "Quella decorazione è in uso.\n"
	.string "Non puoi scambiarla.$"

MauvilleCity_PokemonCenter_1F_Text_SendDecorToYourPC:
	.string "Allora affare fatto!\n"
	.string "Ti mando la mia decorazione sul PC.$"

MauvilleCity_PokemonCenter_1F_Text_CantTradeThatOne:
	.string "Ops! Scusa! Quella è una rarità:\n"
	.string "dicono che sia caduta da un Varco!\l"
	.string "Non posso proprio separarmene!\p"
	.string "Ti interessa qualcos'altro?$"

MauvilleCity_PokemonCenter_1F_EventScript_Trader::
	lock
	faceplayer
	msgbox MauvilleCity_PokemonCenter_1F_Text_WantToTradeDecor, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineTrade
	special GetTraderTradedFlag
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_AlreadyTraded
	message MauvilleCity_PokemonCenter_1F_Text_PickADecorItem
	waitmessage
	goto MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToReceive
	end

MauvilleCity_PokemonCenter_1F_EventScript_DeclineTrade::
	msgbox MauvilleCity_PokemonCenter_1F_Text_TraderFeelingTheBlues, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_AlreadyTraded::
	msgbox MauvilleCity_PokemonCenter_1F_Text_WeveAlreadyTraded, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToReceive::
	special TraderMenuGetDecoration
	goto_if_eq VAR_0x8004, 0, MauvilleCity_PokemonCenter_1F_EventScript_CancelPickDecor
	goto_if_eq VAR_0x8004, 0xFFFF, MauvilleCity_PokemonCenter_1F_EventScript_InvalidDecor
	msgbox MauvilleCity_PokemonCenter_1F_Text_OnceBelongedToPlayerDoYouWantIt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_PickDifferentDecor
	special DoesPlayerHaveNoDecorations
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_DontHaveAnyDecor
	goto MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToGive
	end

MauvilleCity_PokemonCenter_1F_EventScript_CancelPickDecor::
	msgbox MauvilleCity_PokemonCenter_1F_Text_YouDontWantAnything, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_InvalidDecor::
	message MauvilleCity_PokemonCenter_1F_Text_CantTradeThatOne
	waitmessage
	goto MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToReceive
	end

MauvilleCity_PokemonCenter_1F_EventScript_PickDifferentDecor::
	message MauvilleCity_PokemonCenter_1F_Text_PickADecorItem
	waitmessage
	goto MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToReceive
	end

MauvilleCity_PokemonCenter_1F_EventScript_DontHaveAnyDecor::
	msgbox MauvilleCity_PokemonCenter_1F_Text_YouDontHaveAnyDecor, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToGive::
	msgbox MauvilleCity_PokemonCenter_1F_Text_PickTheDecorToTrade, MSGBOX_DEFAULT
	special TraderShowDecorationMenu
	goto_if_eq VAR_0x8006, 0, MauvilleCity_PokemonCenter_1F_EventScript_CancelGiveDecor
	goto_if_eq VAR_0x8006, 0xFFFF, MauvilleCity_PokemonCenter_1F_EventScript_DecorInUse
	special IsDecorationCategoryFull
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_NoRoomForDecor
	msgbox MauvilleCity_PokemonCenter_1F_Text_SoWellTradeTheseDecor, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToGive
	special TraderDoDecorationTrade
	msgbox MauvilleCity_PokemonCenter_1F_Text_SendDecorToYourPC, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_CancelGiveDecor::
	msgbox MauvilleCity_PokemonCenter_1F_Text_YouDontWantToTrade, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_DecorInUse::
	msgbox MauvilleCity_PokemonCenter_1F_Text_ThatDecorIsInUse, MSGBOX_DEFAULT
	goto MauvilleCity_PokemonCenter_1F_EventScript_PickDecorToGive
	end

MauvilleCity_PokemonCenter_1F_EventScript_NoRoomForDecor::
	msgbox MauvilleCity_PokemonCenter_1F_Text_YouveNoRoomForThis, MSGBOX_DEFAULT
	release
	end

@ Storyteller
MauvilleCity_PokemonCenter_1F_Text_WillYouHearMyTale:
	.string "Sono il Cantastorie.\n"
	.string "Narro le gesta di Allenatori\l"
	.string "leggendari.\p"
	.string "Vuoi ascoltare una delle mie storie?$"

MauvilleCity_PokemonCenter_1F_Text_StorytellerFeelingTheBlues:
	.string "Oh…\n"
	.string "Nessuno ha più tempo per le storie…$"

MauvilleCity_PokemonCenter_1F_Text_WhichTaleToTell:
	.string "Conosco queste leggende.\n"
	.string "Quale vuoi che ti racconti?$"

MauvilleCity_PokemonCenter_1F_Text_IKnowNoTales:
	.string "Ahimè, non conosco nessun Allenatore\n"
	.string "leggendario… e quindi niente storie.\p"
	.string "Dove si troverà mai un Allenatore\n"
	.string "degno di una leggenda?$"

MauvilleCity_PokemonCenter_1F_Text_YouDidStatXTimes:
	.string "Cosa?!\n"
	.string "Tu… tu…\p"
	.string "{STR_VAR_2}…\n"
	.string "{STR_VAR_1}?! Davvero?!\p"
	.string "Ma è magnifico!\n"
	.string "È nata una nuova leggenda!$"

MauvilleCity_PokemonCenter_1F_Text_CouldThereBeOtherLegends:
	.string "Mi chiedo… Esisteranno altri\n"
	.string "Allenatori con leggende ancora più\l"
	.string "grandi da scoprire, magari oltre\l"
	.string "i Varchi?$"

MauvilleCity_PokemonCenter_1F_Text_HaveYouAnyLegendaryTales:
	.string "Sei un Allenatore?\p"
	.string "Allora dimmi: hai qualche impresa\n"
	.string "che sappia almeno un po' di leggenda?$"

@ Unused
MauvilleCity_PokemonCenter_1F_Text_HearAnotherLegendaryTale:
	.string "A proposito… Ti andrebbe di sentire\n"
	.string "un'altra storia leggendaria?$"

MauvilleCity_PokemonCenter_1F_Text_NotWorthyOfLegend:
	.string "Mmh…\n"
	.string "Non mi convince…\p"
	.string "Vorrei che mi portassi notizie degne\n"
	.string "di essere chiamate leggenda.$"

MauvilleCity_PokemonCenter_1F_Text_IWishMorePeopleWereInterested:
	.string "Vorrei che più gente volesse sentire\n"
	.string "le mie storie epiche di Allenatori\l"
	.string "leggendari.$"

MauvilleCity_PokemonCenter_1F_Text_SavedGameTitle::
	.string "L'Allenatore previdente$"

MauvilleCity_PokemonCenter_1F_Text_SavedGameAction::
	.string "Partite salvate$"

MauvilleCity_PokemonCenter_1F_Text_SavedGameStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte salvò la partita?\n"
	.string "{STR_VAR_1}! Hai sentito bene!\p"
	.string "Un Allenatore più prudente di\n"
	.string "{STR_VAR_3} non lo troverai mai!$"

MauvilleCity_PokemonCenter_1F_Text_TrendsStartedTitle::
	.string "L'Allenatore alla moda$"

MauvilleCity_PokemonCenter_1F_Text_TrendsStartedAction::
	.string "Mode lanciate$"

MauvilleCity_PokemonCenter_1F_Text_TrendsStartedStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante nuove mode lanciò?\n"
	.string "{STR_VAR_1}! Proprio così!\p"
	.string "{STR_VAR_3} detta le tendenze\n"
	.string "in tutta la regione di Hoenn!$"

MauvilleCity_PokemonCenter_1F_Text_BerriesPlantedTitle::
	.string "L'Allenatore pollice verde$"

MauvilleCity_PokemonCenter_1F_Text_BerriesPlantedAction::
	.string "Bacche piantate$"

MauvilleCity_PokemonCenter_1F_Text_BerriesPlantedStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante Bacche piantò?\n"
	.string "{STR_VAR_1}! Parola mia!\p"
	.string "Nessuno ama le Bacche quanto\n"
	.string "{STR_VAR_3}, in nessun mondo!$"

MauvilleCity_PokemonCenter_1F_Text_BikeTradesTitle::
	.string "L'Allenatore ciclista$"

MauvilleCity_PokemonCenter_1F_Text_BikeTradesAction::
	.string "Bici scambiate$"

MauvilleCity_PokemonCenter_1F_Text_BikeTradesStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte scambiò la Bici?\n"
	.string "{STR_VAR_1}! Roba da non credere!\p"
	.string "{STR_VAR_3} deve amare le Bici\n"
	.string "con una passione travolgente!$"

MauvilleCity_PokemonCenter_1F_Text_InterviewsTitle::
	.string "L'Allenatore da copertina$"

MauvilleCity_PokemonCenter_1F_Text_InterviewsAction::
	.string "Interviste rilasciate$"

MauvilleCity_PokemonCenter_1F_Text_InterviewsStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante interviste rilasciò?\n"
	.string "{STR_VAR_1}! Una vera celebrità!\p"
	.string "{STR_VAR_3} attira gli sguardi\n"
	.string "di tutta Hoenn!$"

MauvilleCity_PokemonCenter_1F_Text_TrainerBattlesTitle::
	.string "L'Allenatore battagliero$"

MauvilleCity_PokemonCenter_1F_Text_TrainerBattlesAction::
	.string "Lotte affrontate$"

MauvilleCity_PokemonCenter_1F_Text_TrainerBattlesStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante lotte affrontò?\n"
	.string "{STR_VAR_1}! Incredibile ma vero!\p"
	.string "{STR_VAR_3} non sa proprio dire\n"
	.string "di no a una bella lotta!$"

MauvilleCity_PokemonCenter_1F_Text_PokemonCaughtTitle::
	.string "L'Allenatore acchiappatutto$"

MauvilleCity_PokemonCenter_1F_Text_PokemonCaughtAction::
	.string "Pokémon catturati$"

MauvilleCity_PokemonCenter_1F_Text_PokemonCaughtStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quanti Pokémon catturò?\n"
	.string "{STR_VAR_1}! Proprio così!\p"
	.string "{STR_VAR_3} cattura di tutto, persino\n"
	.string "i Pokémon giunti dai Varchi!$"

MauvilleCity_PokemonCenter_1F_Text_FishingPokemonCaughtTitle::
	.string "L'Allenatore pescatore$"

MauvilleCity_PokemonCenter_1F_Text_FishingPokemonCaughtAction::
	.string "Pokémon pescati con la Canna$"

MauvilleCity_PokemonCenter_1F_Text_FishingPokemonCaughtStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quanti Pokémon pescò con la Canna?\n"
	.string "{STR_VAR_1}! Nientemeno!\p"
	.string "{STR_VAR_3} è un leggendario\n"
	.string "maestro della pesca!$"

MauvilleCity_PokemonCenter_1F_Text_EggsHatchedTitle::
	.string "L'Allenatore chioccia$"

MauvilleCity_PokemonCenter_1F_Text_EggsHatchedAction::
	.string "Uova schiuse$"

MauvilleCity_PokemonCenter_1F_Text_EggsHatchedStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quanti Pokémon nacquero dalle sue\n"
	.string "Uova? {STR_VAR_1}! Che tenerezza!\p"
	.string "Nessuno scalda le Uova con più\n"
	.string "amore di {STR_VAR_3}!$"

MauvilleCity_PokemonCenter_1F_Text_PokemonEvolvedTitle::
	.string "L'Allenatore evolutivo$"

MauvilleCity_PokemonCenter_1F_Text_PokemonEvolvedAction::
	.string "Pokémon evoluti$"

MauvilleCity_PokemonCenter_1F_Text_PokemonEvolvedStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quanti Pokémon fece evolvere?\n"
	.string "{STR_VAR_1}! Hai sentito bene!\p"
	.string "Nessuno fa evolvere i Pokémon\n"
	.string "come {STR_VAR_3}!$"

MauvilleCity_PokemonCenter_1F_Text_UsedPokemonCenterTitle::
	.string "L'Allenatore del Centro Pokémon$"

MauvilleCity_PokemonCenter_1F_Text_UsedPokemonCenterAction::
	.string "Cure ai Centri Pokémon$"

MauvilleCity_PokemonCenter_1F_Text_UsedPokemonCenterStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte curò i suoi Pokémon in\n"
	.string "un Centro Pokémon? {STR_VAR_1}!\p"
	.string "Nessuno ama i Centri Pokémon\n"
	.string "più di {STR_VAR_3}!$"

MauvilleCity_PokemonCenter_1F_Text_RestedAtHomeTitle::
	.string "L'Allenatore casalingo$"

MauvilleCity_PokemonCenter_1F_Text_RestedAtHomeAction::
	.string "Riposi a casa$"

MauvilleCity_PokemonCenter_1F_Text_RestedAtHomeStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte curò i suoi Pokémon\n"
	.string "a casa? {STR_VAR_1}! Proprio così!\p"
	.string "Nessuno ama stare a casa quanto\n"
	.string "{STR_VAR_3}!$"

MauvilleCity_PokemonCenter_1F_Text_SafariGamesTitle::
	.string "L'Allenatore del Safari$"

MauvilleCity_PokemonCenter_1F_Text_SafariGamesAction::
	.string "Ingressi alla Zona Safari$"

MauvilleCity_PokemonCenter_1F_Text_SafariGamesStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte entrò nella Zona Safari?\n"
	.string "{STR_VAR_1}! Nientemeno!\p"
	.string "Nella Zona Safari, {STR_VAR_3} tira\n"
	.string "fuori il suo lato più selvaggio!$"

MauvilleCity_PokemonCenter_1F_Text_UsedCutTitle::
	.string "L'Allenatore tagliatutto$"

MauvilleCity_PokemonCenter_1F_Text_UsedCutAction::
	.string "Usi di Taglio$"

MauvilleCity_PokemonCenter_1F_Text_UsedCutStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte usò Taglio?\n"
	.string "{STR_VAR_1}! Zac, zac, zac!\p"
	.string "Non c'è cespuglio che possa\n"
	.string "resistere a {STR_VAR_3}!$"

MauvilleCity_PokemonCenter_1F_Text_UsedRockSmashTitle::
	.string "L'Allenatore spaccasassi$"

MauvilleCity_PokemonCenter_1F_Text_UsedRockSmashAction::
	.string "Rocce frantumate$"

MauvilleCity_PokemonCenter_1F_Text_UsedRockSmashStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte usò Spaccaroccia?\n"
	.string "{STR_VAR_1}! Crash, crash!\p"
	.string "{STR_VAR_3} non riesce proprio a\n"
	.string "lasciare una roccia tutta intera!$"

MauvilleCity_PokemonCenter_1F_Text_MovedBasesTitle::
	.string "L'Allenatore nomade$"

MauvilleCity_PokemonCenter_1F_Text_MovedBasesAction::
	.string "Traslochi della Base Segreta$"

MauvilleCity_PokemonCenter_1F_Text_MovedBasesStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Traslocò la sua Base Segreta…\n"
	.string "quante volte? {STR_VAR_1}! Parola mia!\p"
	.string "{STR_VAR_3} adora cambiare casa\n"
	.string "in continuazione!$"

MauvilleCity_PokemonCenter_1F_Text_UsedSplashTitle::
	.string "L'Allenatore spruzzone$"

MauvilleCity_PokemonCenter_1F_Text_UsedSplashAction::
	.string "Usi di Splash$"

MauvilleCity_PokemonCenter_1F_Text_UsedSplashStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte usò Splash?\n"
	.string "{STR_VAR_1}! Splish, splash!\p"
	.string "{STR_VAR_3} deve proprio adorare\n"
	.string "sguazzare qua e là!$"

MauvilleCity_PokemonCenter_1F_Text_UsedStruggleTitle::
	.string "L'Allenatore tenace$"

MauvilleCity_PokemonCenter_1F_Text_UsedStruggleAction::
	.string "Ricorsi a Scontro$"

MauvilleCity_PokemonCenter_1F_Text_UsedStruggleStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte dovette ricorrere\n"
	.string "a Scontro? {STR_VAR_1}! Che grinta!\p"
	.string "{STR_VAR_3} non si arrende mai,\n"
	.string "nemmeno davanti alle avversità!$"

MauvilleCity_PokemonCenter_1F_Text_SlotJackpotsTitle::
	.string "Il Campione delle slot$"

MauvilleCity_PokemonCenter_1F_Text_SlotJackpotsAction::
	.string "Jackpot alle slot machine$"

MauvilleCity_PokemonCenter_1F_Text_SlotJackpotsStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte fece jackpot alle\n"
	.string "slot machine? {STR_VAR_1}! Che fortuna!\p"
	.string "Alle slot machine, la fortuna\n"
	.string "sorride sempre a {STR_VAR_3}!$"

MauvilleCity_PokemonCenter_1F_Text_RouletteWinsTitle::
	.string "Il Campione della roulette$"

MauvilleCity_PokemonCenter_1F_Text_RouletteWinsAction::
	.string "Vittorie di fila alla roulette$"

MauvilleCity_PokemonCenter_1F_Text_RouletteWinsStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante vittorie di fila ottenne\n"
	.string "alla roulette? {STR_VAR_1}! Che colpo!\p"
	.string "Alla roulette, la pallina rimbalza\n"
	.string "sempre dalla parte di {STR_VAR_3}!$"

MauvilleCity_PokemonCenter_1F_Text_BattleTowerChallengesTitle::
	.string "Lo Sfidante della Torre Lotta$"

MauvilleCity_PokemonCenter_1F_Text_BattleTowerChallengesAction::
	.string "Sfide alla Torre Lotta$"

MauvilleCity_PokemonCenter_1F_Text_BattleTowerChallengesStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte sfidò la Torre Lotta?\n"
	.string "{STR_VAR_1}! Nientemeno!\p"
	.string "{STR_VAR_3} insegue la perfezione\n"
	.string "alla Torre Lotta!$"

MauvilleCity_PokemonCenter_1F_Text_MadePokeblocksTitle::
	.string "L'Allenatore frullabacche$"

MauvilleCity_PokemonCenter_1F_Text_MadePokeblocksAction::
	.string "Pokémelle preparate$"

MauvilleCity_PokemonCenter_1F_Text_MadePokeblocksStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte preparò Pokémelle?\n"
	.string "{STR_VAR_1}! Parola mia!\p"
	.string "Nessuno usa il Frullabacche meglio\n"
	.string "di {STR_VAR_3}!$"

MauvilleCity_PokemonCenter_1F_Text_EnteredContestsTitle::
	.string "L'Allenatore delle Gare$"

MauvilleCity_PokemonCenter_1F_Text_EnteredContestsAction::
	.string "Gare Pokémon disputate$"

MauvilleCity_PokemonCenter_1F_Text_EnteredContestsStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "A quante Gare Pokémon partecipò?\n"
	.string "{STR_VAR_1}! Proprio così!\p"
	.string "{STR_VAR_3} adora mettere in mostra\n"
	.string "i suoi Pokémon!$"

MauvilleCity_PokemonCenter_1F_Text_WonContestsTitle::
	.string "Il Maestro delle Gare$"

MauvilleCity_PokemonCenter_1F_Text_WonContestsAction::
	.string "Gare Pokémon vinte$"

MauvilleCity_PokemonCenter_1F_Text_WonContestsStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante Gare Pokémon vinse?\n"
	.string "{STR_VAR_1}! Applausi!\p"
	.string "{STR_VAR_3} è un vero fuoriclasse\n"
	.string "delle Gare Pokémon!$"

MauvilleCity_PokemonCenter_1F_Text_TimesShoppedTitle::
	.string "L'Allenatore spendaccione$"

MauvilleCity_PokemonCenter_1F_Text_TimesShoppedAction::
	.string "Acquisti nei negozi$"

MauvilleCity_PokemonCenter_1F_Text_TimesShoppedStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte fece acquisti nei\n"
	.string "negozi? {STR_VAR_1}! Che portafoglio!\p"
	.string "{STR_VAR_3} ha lo shopping\n"
	.string "nel sangue!$"

MauvilleCity_PokemonCenter_1F_Text_UsedItemFinderTitle::
	.string "L'Allenatore cercatesori$"

MauvilleCity_PokemonCenter_1F_Text_UsedItemFinderAction::
	.string "Usi del Ricerca Strum.$"

MauvilleCity_PokemonCenter_1F_Text_UsedItemFinderStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Il suo Ricerca Strum. suonò…\n"
	.string "quante volte? {STR_VAR_1}! Bip, bip!\p"
	.string "{STR_VAR_3} adora setacciare il\n"
	.string "terreno in cerca di tesori nascosti!$"

MauvilleCity_PokemonCenter_1F_Text_TimesRainedTitle::
	.string "L'Allenatore inzuppato$"

MauvilleCity_PokemonCenter_1F_Text_TimesRainedAction::
	.string "Acquazzoni presi$"

MauvilleCity_PokemonCenter_1F_Text_TimesRainedStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte si prese la pioggia?\n"
	.string "{STR_VAR_1}! Che inzuppata!\p"
	.string "Il carisma di {STR_VAR_3} attira la\n"
	.string "pioggia… o saranno i Varchi?$"

MauvilleCity_PokemonCenter_1F_Text_CheckedPokedexTitle::
	.string "Il divoratore di Pokédex$"

MauvilleCity_PokemonCenter_1F_Text_CheckedPokedexAction::
	.string "Consultazioni del Pokédex$"

MauvilleCity_PokemonCenter_1F_Text_CheckedPokedexStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte consultò il Pokédex?\n"
	.string "{STR_VAR_1}! Che sete di sapere!\p"
	.string "{STR_VAR_3} adora studiare i\n"
	.string "Pokémon nel suo Pokédex!$"

MauvilleCity_PokemonCenter_1F_Text_ReceivedRibbonsTitle::
	.string "Il collezionista di Fiocchi$"

MauvilleCity_PokemonCenter_1F_Text_ReceivedRibbonsAction::
	.string "Fiocchi ricevuti$"

MauvilleCity_PokemonCenter_1F_Text_ReceivedRibbonsStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quanti Fiocchi ricevette?\n"
	.string "{STR_VAR_1}! Che bacheca!\p"
	.string "{STR_VAR_3} ha una vera passione\n"
	.string "per i Fiocchi!$"

MauvilleCity_PokemonCenter_1F_Text_LedgesJumpedTitle::
	.string "L'Allenatore canguro$"

MauvilleCity_PokemonCenter_1F_Text_LedgesJumpedAction::
	.string "Salti dalle sporgenze$"

MauvilleCity_PokemonCenter_1F_Text_LedgesJumpedStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte saltò giù da una\n"
	.string "sporgenza? {STR_VAR_1}! Hop, hop!\p"
	.string "Se c'è una sporgenza da saltare,\n"
	.string "{STR_VAR_3} non sa resistere!$"

MauvilleCity_PokemonCenter_1F_Text_TVWatchedTitle::
	.string "Il leggendario teledipendente$"

MauvilleCity_PokemonCenter_1F_Text_TVWatchedAction::
	.string "Visioni della TV$"

MauvilleCity_PokemonCenter_1F_Text_TVWatchedStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte guardò la TV?\n"
	.string "{STR_VAR_1}! Telecomando in pugno!\p"
	.string "{STR_VAR_3} deve proprio adorare\n"
	.string "la TV!$"

MauvilleCity_PokemonCenter_1F_Text_CheckedClockTitle::
	.string "L'Allenatore puntuale$"

MauvilleCity_PokemonCenter_1F_Text_CheckedClockAction::
	.string "Occhiate all'orologio$"

MauvilleCity_PokemonCenter_1F_Text_CheckedClockStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte controllò l'ora?\n"
	.string "{STR_VAR_1}! Tic, tac, tic, tac!\p"
	.string "{STR_VAR_3} non arriva mai in\n"
	.string "ritardo, nemmeno di un secondo!$"

MauvilleCity_PokemonCenter_1F_Text_WonLotteryTitle::
	.string "Il mago della Lotteria$"

MauvilleCity_PokemonCenter_1F_Text_WonLotteryAction::
	.string "Vincite alla Lotteria Pokémon$"

MauvilleCity_PokemonCenter_1F_Text_WonLotteryStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Vinse alla Lotteria Pokémon…\n"
	.string "quante volte? {STR_VAR_1}! Bingo!\p"
	.string "{STR_VAR_3} deve avere tanti amici\n"
	.string "con cui scambiare Pokémon!$"

MauvilleCity_PokemonCenter_1F_Text_UsedDaycareTitle::
	.string "L'Allenatore della Pensione$"

MauvilleCity_PokemonCenter_1F_Text_UsedDaycareAction::
	.string "Pokémon lasciati alla Pensione$"

MauvilleCity_PokemonCenter_1F_Text_UsedDaycareStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Lasciò Pokémon alla Pensione…\n"
	.string "quante volte? {STR_VAR_1}! Nientemeno!\p"
	.string "{STR_VAR_3} alleva Pokémon con\n"
	.string "una grinta da vendere!$"

MauvilleCity_PokemonCenter_1F_Text_RodeCableCarTitle::
	.string "L'Allenatore della Funivia$"

MauvilleCity_PokemonCenter_1F_Text_RodeCableCarAction::
	.string "Viaggi in Funivia$"

MauvilleCity_PokemonCenter_1F_Text_RodeCableCarStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quante volte prese la Funivia?\n"
	.string "{STR_VAR_1}! Proprio così!\p"
	.string "{STR_VAR_3} è sempre di corsa, su\n"
	.string "e giù per il Monte Camino!$"

MauvilleCity_PokemonCenter_1F_Text_HotSpringsTitle::
	.string "L'Allenatore delle terme$"

MauvilleCity_PokemonCenter_1F_Text_HotSpringsAction::
	.string "Bagni nelle sorgenti termali$"

MauvilleCity_PokemonCenter_1F_Text_HotSpringsStory::
	.string "Ascolta la leggenda di un Allenatore\n"
	.string "chiamato {STR_VAR_3}.\p"
	.string "Quanti bagni termali si concesse?\n"
	.string "{STR_VAR_1}! Che relax!\p"
	.string "{STR_VAR_3} avrà una pelle liscia\n"
	.string "come quella di un bebè!$"


MauvilleCity_PokemonCenter_1F_EventScript_Storyteller::
	lock
	faceplayer
	setvar VAR_0x8008, 0
	setvar VAR_0x8009, 0
	setvar VAR_0x800A, 0
	setvar VAR_0x800B, 0
	msgbox MauvilleCity_PokemonCenter_1F_Text_WillYouHearMyTale, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineStoryteller
	specialvar VAR_RESULT, StorytellerGetFreeStorySlot
	goto_if_eq VAR_RESULT, 0, MauvilleCity_PokemonCenter_1F_EventScript_KnowNoTales
	message MauvilleCity_PokemonCenter_1F_Text_WhichTaleToTell
	waitmessage
	special StorytellerStoryListMenu
	goto_if_eq VAR_RESULT, 0, MauvilleCity_PokemonCenter_1F_EventScript_CancelStorySelection
	setvar VAR_0x8008, 1
	special Script_StorytellerDisplayStory
	waitmessage
	waitbuttonpress
	specialvar VAR_RESULT, StorytellerUpdateStat
	goto_if_eq VAR_RESULT, 0, MauvilleCity_PokemonCenter_1F_EventScript_AreThereOtherTales
	goto MauvilleCity_PokemonCenter_1F_EventScript_TellPlayersTale

MauvilleCity_PokemonCenter_1F_EventScript_CancelStorySelection::
	goto MauvilleCity_PokemonCenter_1F_EventScript_DeclineStoryteller
	end

MauvilleCity_PokemonCenter_1F_EventScript_AreThereOtherTales::
	msgbox MauvilleCity_PokemonCenter_1F_Text_CouldThereBeOtherLegends, MSGBOX_DEFAULT
	specialvar VAR_RESULT, HasStorytellerAlreadyRecorded
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_StorytellerEnd
	goto MauvilleCity_PokemonCenter_1F_EventScript_DoYouHaveAnyTales

MauvilleCity_PokemonCenter_1F_EventScript_KnowNoTales::
	msgbox MauvilleCity_PokemonCenter_1F_Text_IKnowNoTales, MSGBOX_DEFAULT
MauvilleCity_PokemonCenter_1F_EventScript_DoYouHaveAnyTales::
	msgbox MauvilleCity_PokemonCenter_1F_Text_HaveYouAnyLegendaryTales, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineStoryteller
	specialvar VAR_RESULT, Script_StorytellerInitializeRandomStat
	goto_if_eq VAR_RESULT, 1, MauvilleCity_PokemonCenter_1F_EventScript_TellPlayersTale
	msgbox MauvilleCity_PokemonCenter_1F_Text_NotWorthyOfLegend, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_TellPlayersTale::
	msgbox MauvilleCity_PokemonCenter_1F_Text_YouDidStatXTimes, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_DeclineStoryteller::
	msgbox MauvilleCity_PokemonCenter_1F_Text_StorytellerFeelingTheBlues, MSGBOX_DEFAULT
	release
	end

@ Unused
MauvilleCity_PokemonCenter_1F_EventScript_WaitingForRecordMix::
	msgbox MauvilleCity_PokemonCenter_1F_Text_IWishMorePeopleWereInterested, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_StorytellerEnd::
	release
	end

@ Giddy
MauvilleCity_PokemonCenter_1F_Text_HearMyStory:
	.string "Sono Euforio!\n"
	.string "Ho una storia frizzantissima per te!\p"
	.string "Vuoi sentire la mia storia?$"

MauvilleCity_PokemonCenter_1F_Text_GiddyFeelingTheBlues:
	.string "Oh…\n"
	.string "Che tristezza, che malinconia…$"

MauvilleCity_PokemonCenter_1F_Text_AlsoIWasThinking:
	.string "E poi, stavo pensando…$"

MauvilleCity_PokemonCenter_1F_Text_WeShouldChatAgain:
	.string "Direi che è tutto, credo…\p"
	.string "Dobbiamo rifare due chiacchiere!\n"
	.string "Ciao ciao!$"

MauvilleCity_PokemonCenter_1F_EventScript_Giddy::
	lock
	faceplayer
	msgbox MauvilleCity_PokemonCenter_1F_Text_HearMyStory, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, MauvilleCity_PokemonCenter_1F_EventScript_TryTellTale
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_DeclineGiddy
	end

MauvilleCity_PokemonCenter_1F_EventScript_TryTellTale::
	special GiddyShouldTellAnotherTale
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_GiddyTellTale
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_PokemonCenter_1F_EventScript_ToldEnoughTales
	end

MauvilleCity_PokemonCenter_1F_EventScript_TryTellNewTale::
	special GiddyShouldTellAnotherTale
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_PokemonCenter_1F_EventScript_GiddyStartNewTale
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_PokemonCenter_1F_EventScript_ToldEnoughTales
	end

MauvilleCity_PokemonCenter_1F_EventScript_GiddyStartNewTale::
	msgbox MauvilleCity_PokemonCenter_1F_Text_AlsoIWasThinking, MSGBOX_DEFAULT
	goto MauvilleCity_PokemonCenter_1F_EventScript_GiddyTellTale
	end

@ Giddy will continue to tell stories regardless of whether yes or no is selected below.
@ Each story there is a 10% chance it will be his last. Otherwise he will stop at 10 stories.
MauvilleCity_PokemonCenter_1F_EventScript_GiddyTellTale::
	special GenerateGiddyLine
	special ShowFieldMessageStringVar4
	waitmessage
	yesnobox 20, 8
	goto_if_eq VAR_RESULT, YES, MauvilleCity_PokemonCenter_1F_EventScript_TryTellNewTale
	goto_if_eq VAR_RESULT, NO, MauvilleCity_PokemonCenter_1F_EventScript_TryTellNewTale
	end

MauvilleCity_PokemonCenter_1F_EventScript_DeclineGiddy::
	msgbox MauvilleCity_PokemonCenter_1F_Text_GiddyFeelingTheBlues, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_EventScript_ToldEnoughTales::
	msgbox MauvilleCity_PokemonCenter_1F_Text_WeShouldChatAgain, MSGBOX_DEFAULT
	release
	end

MauvilleCity_PokemonCenter_1F_Text_WouldYouLikeToHearMySong:
	.string "Ciao, sono il Menestrello.\n"
	.string "Vuoi sentire la mia canzone?$"

MauvilleCity_PokemonCenter_1F_Text_BardFeelingTheBlues1:
	.string "Oh…\n"
	.string "Mi hai fatto venire la malinconia…$"

MauvilleCity_PokemonCenter_1F_Text_WishICouldPlaySongForOthers:
	.string "Ah, che canzone commovente…\n"
	.string "Vorrei poterla suonare per tutti…$"

MauvilleCity_PokemonCenter_1F_Text_WouldYouLikeToWriteSomeLyrics:
	.string "Allora?\n"
	.string "Ti è piaciuta la mia canzone?\p"
	.string "Però il testo non mi convince\n"
	.string "per niente.\p"
	.string "Ti andrebbe di scrivermi delle\n"
	.string "parole nuove?$"

MauvilleCity_PokemonCenter_1F_Text_BardFeelingTheBlues2:
	.string "Oh…\n"
	.string "Mi è passata tutta l'ispirazione…$"

MauvilleCity_PokemonCenter_1F_Text_LetMeSingItForYou:
	.string "Grazie di cuore!\n"
	.string "Lascia che te la canti.$"

MauvilleCity_PokemonCenter_1F_Text_ThatHowYouWantedSongToGo:
	.string "È così che volevi che suonasse\n"
	.string "la tua canzone?$"

MauvilleCity_PokemonCenter_1F_Text_IllSingThisSongForAWhile:
	.string "Perfetto! Allora è deciso.\n"
	.string "Canterò questa canzone per un po'.$"

MauvilleCity_PokemonCenter_1F_Text_TeachWhatsHipAndHappening:
	.string "Ehi, yo! Mi chiamano il Modaiolo.\n"
	.string "Ti insegno cosa fa tendenza adesso!$"

MauvilleCity_PokemonCenter_1F_Text_IAlreadyTaughtYou:
	.string "Ma, ehi, ti ho già insegnato cosa\n"
	.string "fa tendenza.\p"
	.string "Ora voglio spargere la voce anche\n"
	.string "tra altra gente.$"

MauvilleCity_PokemonCenter_1F_Text_IveGotNothingNewToTeach:
	.string "Ma, ehi, tu sai già un sacco di cose\n"
	.string "su ciò che fa tendenza.\p"
	.string "Non ho niente di nuovo da insegnarti!$"

MauvilleCity_PokemonCenter_1F_Text_HaveYouHeardOfWord:
	.string "Ehi, hai mai sentito parlare di\n"
	.string "“{STR_VAR_1}”?\p"
	.string "Cosa vuol dire? Beh…\n"
	.string "Chiedilo a mamma o a papà, ok?$"
