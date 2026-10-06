LilycoveCity_CoveLilyMotel_2F_MapScripts::
	.byte 0

LilycoveCity_CoveLilyMotel_2F_EventScript_GameDesigner::
	lock
	faceplayer
	call_if_unset FLAG_TEMP_2, LilycoveCity_CoveLilyMotel_2F_EventScript_ShowMeCompletedDex
	call_if_set FLAG_TEMP_2, LilycoveCity_CoveLilyMotel_2F_EventScript_ShowDiploma
	specialvar VAR_RESULT, HasAllHoennMons
	goto_if_eq VAR_RESULT, TRUE, LilycoveCity_CoveLilyMotel_2F_EventScript_AllHoennMonsFanfare
	release
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_ShowMeCompletedDex::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_ShowMeCompletedDex, MSGBOX_DEFAULT
	return

LilycoveCity_CoveLilyMotel_2F_EventScript_AllHoennMonsFanfare::
	setflag FLAG_TEMP_2
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	goto LilycoveCity_CoveLilyMotel_2F_EventScript_ShowDiploma
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_ShowDiploma::
	message LilycoveCity_CoveLilyMotel_2F_Text_FilledPokedexGiveYouThis
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	special Special_ShowDiploma
	release
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_Programmer::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_ImTheProgrammer, MSGBOX_NPC
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_GraphicArtist::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_ImTheGraphicArtist, MSGBOX_NPC
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_FatMan::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_GirlsAreCute, MSGBOX_NPC
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_Woman::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_SeaBreezeTicklesHeart, MSGBOX_NPC
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_GameBoyKid::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_NeverLeaveWithoutGameBoy, MSGBOX_NPC
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_Scott::
	lock
	faceplayer
	goto_if_set FLAG_MET_SCOTT_IN_LILYCOVE, LilycoveCity_CoveLilyMotel_2F_EventScript_MetScott
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_SnoozingPreferBattles, MSGBOX_DEFAULT
	addvar VAR_SCOTT_STATE, 1
	setflag FLAG_MET_SCOTT_IN_LILYCOVE
	release
	end

LilycoveCity_CoveLilyMotel_2F_EventScript_MetScott::
	msgbox LilycoveCity_CoveLilyMotel_2F_Text_ContestsDoTakeStrategy, MSGBOX_DEFAULT
	release
	end

LilycoveCity_CoveLilyMotel_2F_Text_ShowMeCompletedDex:
	.string "Sono il Game Designer.\p"
	.string "Oh, davvero?\n"
	.string "Stai completando un Pokédex?\p"
	.string "Con tutti i Pokémon sbucati dai Varchi\n"
	.string "è un'impresa, ma non arrenderti.\p"
	.string "Se riesci a completarlo, vieni a\n"
	.string "mostrarmelo, mi raccomando!$"

LilycoveCity_CoveLilyMotel_2F_Text_FilledPokedexGiveYouThis:
	.string "Wow! Pazzesco!\n"
	.string "Sì, è davvero pazzesco!\p"
	.string "Questo Pokédex è completo!\n"
	.string "Devi proprio amare i Pokémon!\p"
	.string "Sono colpitissimo!\p"
	.string "Lascia che ti dia qualcosa come\n"
	.string "riconoscimento per la tua impresa!$"

LilycoveCity_CoveLilyMotel_2F_Text_ImTheProgrammer:
	.string "Io? Dici a me?\n"
	.string "Sono il programmatore.\p"
	.string "Chissà come sono le slot machine\n"
	.string "da queste parti.$"

LilycoveCity_CoveLilyMotel_2F_Text_ImTheGraphicArtist:
	.string "Sono il grafico! Non trovi che i\n"
	.string "Pokémon di Hoenn siano interessanti?\p"
	.string "E quelli sbucati dai Varchi, poi…\n"
	.string "Quanta ispirazione!$"

LilycoveCity_CoveLilyMotel_2F_Text_GirlsAreCute:
	.string "Le Bagnanti… carine, eh?\n"
	.string "Lottare contro una Bagnante carina…\p"
	.string "Uuh, che spettacolo!\p"
	.string "E le Gemelle! Non sono adorabili?\n"
	.string "Una lotta 2 contro 2 con le Gemelle…\p"
	.string "Uuh, è divertente da morire!$"

LilycoveCity_CoveLilyMotel_2F_Text_SeaBreezeTicklesHeart:
	.string "La brezza marina mi accarezza il cuore.\n"
	.string "Qui si sta d'incanto!$"

LilycoveCity_CoveLilyMotel_2F_Text_NeverLeaveWithoutGameBoy:
	.string "Non sai mai quando e dove qualcuno\n"
	.string "ti sfiderà.\p"
	.string "Per questo non esco mai di casa senza\n"
	.string "il mio Game Boy Advance.$"

LilycoveCity_CoveLilyMotel_2F_Text_SnoozingPreferBattles:
	.string "Scott: … … … … …\n"
	.string "… … … … … Zzz…\p"
	.string "… … … … … Eh?!\n"
	.string "Oh, scusa, scusa! Mi ero appisolato!\p"
	.string "Sono venuto a dare un'occhiata a\n"
	.string "queste famose Gare Pokémon.\p"
	.string "Lo ammetto, sembrano davvero\n"
	.string "divertenti, ma…\p"
	.string "Io sono un purista: preferisco le lotte\n"
	.string "e gli Allenatori tosti.\p"
	.string "Meglio ancora se arrivano da lontano.\n"
	.string "Molto, molto lontano…\p"
	.string "Ma questo è solo il mio gusto.\p"
	.string "{PLAYER}{KUN}, spero che ti godrai tutto:\n"
	.string "Palestre, Gare, Tende Lotta…\l"
	.string "proprio tutto quanto!$"

LilycoveCity_CoveLilyMotel_2F_Text_ContestsDoTakeStrategy:
	.string "Scott: Secondo me, per vincere una\n"
	.string "Gara serve strategia.\p"
	.string "Studiare strategie per le Gare è un\n"
	.string "modo per diventare Allenatori migliori.$"

