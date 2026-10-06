ProfileMan_EventScript_Man::
	lock
	faceplayer
	goto_if_set FLAG_SYS_CHAT_USED, ProfileMan_EventScript_GivenProfileBefore
	msgbox ProfileMan_Text_CollectTrainerProfiles, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeProfile
	end

ProfileMan_EventScript_AskToSeeProfile::
	msgbox ProfileMan_Text_MayISeeYourProfile, MSGBOX_DEFAULT
	multichoice 17, 6, MULTI_YESNOINFO_2, FALSE
	switch VAR_RESULT
	case 0, ProfileMan_EventScript_CreateProfile
	case 1, ProfileMan_EventScript_DeclineShowProfile
	case 2, ProfileMan_EventScript_Info
	case MULTI_B_PRESSED, ProfileMan_EventScript_DeclineShowProfile
	end

ProfileMan_EventScript_Info::
	msgbox ProfileMan_Text_EasyChatExplanation, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeProfile
	end

ProfileMan_EventScript_CreateProfile::
	msgbox ProfileMan_Text_LetsSeeItThen, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, EASY_CHAT_TYPE_PROFILE
	call Common_ShowEasyChatScreen
	lock
	faceplayer
	goto_if_eq VAR_RESULT, 0, ProfileMan_EventScript_CancelShowProfile
	goto_if_eq VAR_RESULT, 1, ProfileMan_EventScript_ShowProfile
	end

ProfileMan_EventScript_CancelShowProfile::
	msgbox ProfileMan_Text_NotIntoItRightNow, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_ShowProfile::
	setvar VAR_0x8004, 0
	special ShowEasyChatProfile
	waitmessage
	delay 80
	msgbox ProfileMan_Text_FantasticProfile, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_DeclineShowProfile::
	msgbox ProfileMan_Text_ImagineYouWouldHaveWonderfulProfile, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_GivenProfileBefore::
	msgbox ProfileMan_Text_YouHaveWonderfulSmile, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeNewProfile
	end

ProfileMan_EventScript_AskToSeeNewProfile::
	msgbox ProfileMan_Text_MayISeeYourNewProfile, MSGBOX_DEFAULT
	multichoice 17, 6, MULTI_YESNOINFO_2, FALSE
	switch VAR_RESULT
	case 0, ProfileMan_EventScript_CreateNewProfile
	case 1, ProfileMan_EventScript_DeclineNewProfile
	case 2, ProfileMan_EventScript_InfoNewProfile
	case MULTI_B_PRESSED, ProfileMan_EventScript_DeclineNewProfile
	end

ProfileMan_EventScript_InfoNewProfile::
	msgbox ProfileMan_Text_EasyChatExplanation, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeNewProfile
	end

ProfileMan_EventScript_CreateNewProfile::
	msgbox ProfileMan_Text_EvenBetterThanLastProfile, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, EASY_CHAT_TYPE_PROFILE
	call Common_ShowEasyChatScreen
	lock
	faceplayer
	goto_if_eq VAR_RESULT, 0, ProfileMan_EventScript_CancelShowProfile
	goto_if_eq VAR_RESULT, 1, ProfileMan_EventScript_ShowProfile
	end

ProfileMan_EventScript_DeclineNewProfile::
	msgbox ProfileMan_Text_LikeProfileWayItIs, MSGBOX_DEFAULT
	release
	end

@ Unused
ProfileMan_EventScript_Ret::
	return

ProfileMan_Text_CollectTrainerProfiles:
	.string "Salve, Allenatore!\n"
	.string "Che sorriso meraviglioso che hai!\p"
	.string "Sai, ho un hobby: colleziono i profili\n"
	.string "degli Allenatori di Pokémon.$"

ProfileMan_Text_MayISeeYourProfile:
	.string "Allora, che ne dici?\n"
	.string "Posso vedere il tuo profilo?$"

ProfileMan_Text_EasyChatExplanation:
	.string "Il profilo si crea mettendo insieme\n"
	.string "quattro parole o espressioni.\p"
	.string "Ecco, ti mostro un esempio di\n"
	.string "profilo fatto con quattro pezzi.\p"
	.string "Puoi sostituire quei quattro pezzi\n"
	.string "con altri, combinandoli come vuoi,\l"
	.string "per creare il tuo profilo.\p"
	.string "Ci sono tantissime parole ed\n"
	.string "espressioni tra cui scegliere.\p"
	.string "Sono divise in gruppi, come Pokémon,\n"
	.string "stili di vita e hobby, così è più\l"
	.string "facile trovarle.\p"
	.string "Per prima cosa, scegli un gruppo per\n"
	.string "vedere l'elenco delle possibilità.\p"
	.string "Poi seleziona quella che vuoi.\p"
	.string "Ripeti con gli altri pezzi e avrai\n"
	.string "un profilo tutto tuo!$"

ProfileMan_Text_LetsSeeItThen:
	.string "Sì! Grazie!\n"
	.string "Allora, vediamolo!$"

ProfileMan_Text_ImagineYouWouldHaveWonderfulProfile:
	.string "Oh, no, davvero?\p"
	.string "Scommetto che uno come te avrebbe\n"
	.string "un profilo meraviglioso…$"

ProfileMan_Text_NotIntoItRightNow:
	.string "Oh? Adesso non ti va?\p"
	.string "Beh, per me qualsiasi momento\n"
	.string "va benissimo!$"

ProfileMan_Text_YouHaveWonderfulSmile:
	.string "Salve, Allenatore!\n"
	.string "Hai proprio un sorriso splendido.$"

ProfileMan_Text_MayISeeYourNewProfile:
	.string "Posso vedere il tuo nuovo profilo?$"

ProfileMan_Text_EvenBetterThanLastProfile:
	.string "Sì! Grazie!\p"
	.string "Spero che sia ancora meglio di quello\n"
	.string "che mi hai mostrato l'altra volta.$"

ProfileMan_Text_LikeProfileWayItIs:
	.string "Ah, preferisci tenere il tuo profilo\n"
	.string "così com'è.\p"
	.string "E come darti torto? È davvero\n"
	.string "magnifico così!$"

ProfileMan_Text_FantasticProfile:
	.string "Fa-fantastico!\p"
	.string "Il tuo profilo è meraviglioso!\n"
	.string "Dice proprio chi sei.\p"
	.string "Chiunque lo ascolti resterebbe\n"
	.string "incantato da te, in qualsiasi mondo!\p"
	.string "Grazie!$"

@ Unused
ProfileMan_Text_YouKnowSecretSaying:
	.string "Oh?\n"
	.string "Conosci la frase segreta!\p"
	.string "Significa che ora fai parte anche tu\n"
	.string "del Club dell'Evento Misterioso!$"
