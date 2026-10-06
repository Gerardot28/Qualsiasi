RustboroCity_CuttersHouse_MapScripts::
	.byte 0

RustboroCity_CuttersHouse_EventScript_Cutter::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_CUT, RustboroCity_CuttersHouse_EventScript_ExplainCut
	msgbox RustboroCity_CuttersHouse_Text_YouCanPutThisHMToGoodUse, MSGBOX_DEFAULT
	giveitem ITEM_HM_CUT
	setflag FLAG_RECEIVED_HM_CUT
	msgbox RustboroCity_CuttersHouse_Text_ExplainCut, MSGBOX_DEFAULT
	release
	end

RustboroCity_CuttersHouse_EventScript_ExplainCut::
	msgbox RustboroCity_CuttersHouse_Text_ExplainCut, MSGBOX_DEFAULT
	release
	end

RustboroCity_CuttersHouse_EventScript_Lass::
	msgbox RustboroCity_CuttersHouse_Text_DadHelpedClearLandOfTrees, MSGBOX_NPC
	end

RustboroCity_CuttersHouse_Text_YouCanPutThisHMToGoodUse:
	.string "Quello sguardo deciso…\n"
	.string "Quel modo agile di muoverti…\l"
	.string "E quei Pokémon ben allenati…\p"
	.string "Sei di sicuro un Allenatore in gamba!\p"
	.string "No, aspetta, non dire una parola.\n"
	.string "Lo capisco solo guardandoti.\p"
	.string "Sono certo che saprai usare bene\n"
	.string "questa Macchina Nascosta.\p"
	.string "Niente modestie, niente timidezze.\n"
	.string "Su, prendila!$"

RustboroCity_CuttersHouse_Text_ExplainCut:
	.string "Quella Macchina Nascosta, o MN,\n"
	.string "contiene Taglio.\p"
	.string "Una mossa MN si può usare anche\n"
	.string "fuori dalla lotta.\p"
	.string "Un Pokémon che conosce Taglio può\n"
	.string "abbattere gli alberelli, se il suo\l"
	.string "Allenatore ha la Medaglia Pietra.\p"
	.string "E, a differenza di una MT, una MN\n"
	.string "si può usare tutte le volte che vuoi.$"

RustboroCity_CuttersHouse_Text_DadHelpedClearLandOfTrees:
	.string "Quando hanno ingrandito Ferrugipoli,\n"
	.string "il mio papà ha dato una mano.\p"
	.string "Ha fatto usare Taglio ai suoi Pokémon\n"
	.string "per liberare il terreno dagli alberi.\p"
	.string "Adesso lo chiamano per tagliare piante\n"
	.string "strane, mai viste a Hoenn…\l"
	.string "Dicono che arrivino dai Varchi!$"

