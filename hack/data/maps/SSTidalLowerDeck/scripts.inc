SSTidalLowerDeck_MapScripts::
	.byte 0

SSTidalLowerDeck_EventScript_Phillip::
	trainerbattle_single TRAINER_PHILLIP, SSTidalLowerDeck_Text_PhillipIntro, SSTidalLowerDeck_Text_PhillipDefeat
	msgbox SSTidalLowerDeck_Text_PhillipPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalLowerDeck_EventScript_Leonard::
	trainerbattle_single TRAINER_LEONARD, SSTidalLowerDeck_Text_LeonardIntro, SSTidalLowerDeck_Text_LeonardDefeat
	msgbox SSTidalLowerDeck_Text_LeonardPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalLowerDeck_Text_PhillipIntro:
	.string "Arrrgh! Sono stufo e stanco morto di\n"
	.string "lucidare questa stiva enorme!\p"
	.string "Facciamoci una lotta veloce!$"

SSTidalLowerDeck_Text_PhillipDefeat:
	.string "Fratellino, ho perso!$"

SSTidalLowerDeck_Text_PhillipPostBattle:
	.string "Siamo i Fratelli Spazzolone!\p"
	.string "Il grande versa il detersivo,\n"
	.string "e il piccolo strofina!$"

SSTidalLowerDeck_Text_LeonardIntro:
	.string "Questo è il fondo della stiva.\n"
	.string "C'è spazio in abbondanza, qui.\l"
	.string "Perfetto per una lotta Pokémon!$"

SSTidalLowerDeck_Text_LeonardDefeat:
	.string "Fratellone, ho perso!$"

SSTidalLowerDeck_Text_LeonardPostBattle:
	.string "Siamo i Fratelli Spazzolone!\p"
	.string "Il grande versa il detersivo, e il\n"
	.string "piccolo strofina. Cioè io. Sempre io.$"

