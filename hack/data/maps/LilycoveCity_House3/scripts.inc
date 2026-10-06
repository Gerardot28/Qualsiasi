LilycoveCity_House3_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LilycoveCity_House3_OnTransition
	.byte 0

LilycoveCity_House3_OnTransition:
	random 4
	copyvar VAR_TEMP_1, VAR_RESULT
	end

LilycoveCity_House3_EventScript_PokefanF::
	lock
	faceplayer
	msgbox LilycoveCity_House3_Text_LearnFromMasterOfPokeblocks, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, LilycoveCity_House3_EventScript_DeclinePokeblockLearn
	msgbox LilycoveCity_House3_Text_ExplainPokeblocks, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_House3_EventScript_DeclinePokeblockLearn::
	msgbox LilycoveCity_House3_Text_OhAreYouSure, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_House3_EventScript_Man::
	lock
	faceplayer
	msgbox LilycoveCity_House3_Text_HappyToHaveQuadruplets, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

LilycoveCity_House3_EventScript_GameBoyKid1::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_GameBoyKid2::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_GameBoyKid3::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_GameBoyKid4::
	lock
	faceplayer
	switch VAR_TEMP_1
	case 0, LilycoveCity_House3_EventScript_WereDoingMultiBattle
	case 1, LilycoveCity_House3_EventScript_WereMixingRecords
	case 2, LilycoveCity_House3_EventScript_WereBlendingBerries
	case 3, LilycoveCity_House3_EventScript_WereDoingContest
	end

LilycoveCity_House3_EventScript_WereDoingMultiBattle::
	msgbox LilycoveCity_House3_Text_GoingToWinMultiBattles, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_EventScript_WereMixingRecords::
	msgbox LilycoveCity_House3_Text_LikeMixingAtRecordCorner, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_EventScript_WereBlendingBerries::
	msgbox LilycoveCity_House3_Text_MakePokeblocksWithBerryBlender, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_EventScript_WereDoingContest::
	msgbox LilycoveCity_House3_Text_GoingToEnterContest, MSGBOX_DEFAULT
	release
	end

LilycoveCity_House3_Text_LearnFromMasterOfPokeblocks:
	.string "Oh, cielo! Viaggi tutto solo?\n"
	.string "Ma sei così giovane! Bravo!\p"
	.string "Scommetto che i miei figli avrebbero\n"
	.string "parecchio da imparare da te!\p"
	.string "Io? Sono una maestra di Pokémelle.\p"
	.string "Se mi ci metto d'impegno, sai, posso\n"
	.string "preparare Pokémelle eccezionali.\p"
	.string "Vuoi imparare qualcosa da me,\n"
	.string "la maestra delle Pokémelle?$"

LilycoveCity_House3_Text_OhAreYouSure:
	.string "Oh? Sei sicuro?\p"
	.string "Non devi sempre cercare di fare\n"
	.string "tutto da solo, tesoro!$"

LilycoveCity_House3_Text_ExplainPokeblocks:
	.string "Oh, bene! Sei un tipo sveglio!\n"
	.string "È un po' lunga, quindi ascolta bene!\p"
	.string "Sei pronto?\p"
	.string "Se guardi le Pokémelle, vedrai che\n"
	.string "hanno un voto di consistenza.\p"
	.string "Più il voto è basso, meglio è.\n"
	.string "Non dimenticarlo!\p"
	.string "Una buona Pokémella ha un voto di\n"
	.string "consistenza basso e un livello alto.\p"
	.string "E un Pokémon riesce a mangiare più\n"
	.string "Pokémelle buone che normali, sai?\p"
	.string "Adesso viene la parte importante.\p"
	.string "Se vuoi Pokémelle morbide, usa\n"
	.string "tanti tipi diversi di Bacche.\p"
	.string "Non fare il tirchio: il tipo di Bacche\n"
	.string "decide quanto saranno morbide!\p"
	.string "E un'altra cosa.\p"
	.string "Più persone frullano le Bacche,\n"
	.string "più morbide vengono le Pokémelle.\p"
	.string "Per questo dovresti parlare con gli\n"
	.string "altri e preparare Pokémelle insieme.\p"
	.string "Ecco, questo è tutto quello che ti\n"
	.string "serve per fare delle buone Pokémelle.\p"
	.string "Se tutti avessero Pokémon da amare,\n"
	.string "Pokémelle morbide e una famiglia\l"
	.string "affettuosa, il mondo sarebbe più bello.\p"
	.string "Non mollare, tesoro!$"

LilycoveCity_House3_Text_HappyToHaveQuadruplets:
	.string "Quando mia moglie ha dato alla luce\n"
	.string "quattro gemelli, ero sconvolto.\p"
	.string "Ma ora, a vederli giocare insieme,\n"
	.string "sono l'uomo più felice del mondo.$"

LilycoveCity_House3_Text_GoingToWinMultiBattles:
	.string "Stiamo facendo delle Lotte Multiple,\n"
	.string "ma so già che vincerò io.$"

LilycoveCity_House3_Text_LikeMixingAtRecordCorner:
	.string "Ci piace mescolare roba\n"
	.string "all'Angolo Record.\p"
	.string "Ma cosa si mescola, di preciso?$"

LilycoveCity_House3_Text_MakePokeblocksWithBerryBlender:
	.string "Faremo delle super Pokémelle\n"
	.string "con il Mixer Bacche!$"

LilycoveCity_House3_Text_GoingToEnterContest:
	.string "Voglio far vedere a tutti quanto è\n"
	.string "forte il mio Pokémon, quindi andiamo\l"
	.string "insieme a una Gara!$"

