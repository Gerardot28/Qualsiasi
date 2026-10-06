PacifidlogTown_House1_MapScripts::
	.byte 0

PacifidlogTown_House1_EventScript_Man::
	msgbox PacifidlogTown_House1_Text_RegiStory, MSGBOX_NPC
	end

PacifidlogTown_House1_EventScript_Woman::
	msgbox PacifidlogTown_House1_Text_SixDotsOpenThreeDoors, MSGBOX_NPC
	end

PacifidlogTown_House1_Text_RegiStory:
	.string "Nella regione di Hoenn ci sono tre\n"
	.string "Pokémon che incarnano la forza della\l"
	.string "roccia, del ghiaccio e dell'acciaio.\p"
	.string "Si dice che ognuno di loro si nasconda\n"
	.string "nelle profondità di una grotta.\p"
	.string "Un popolo antico li avrebbe sigillati\n"
	.string "là sotto, ai tempi del Grande Varco.\p"
	.string "È la storia che mi raccontavano\n"
	.string "quand'ero piccolo.$"

PacifidlogTown_House1_Text_SixDotsOpenThreeDoors:
	.string "“Sei punti aprono tre porte.”\p"
	.string "Il nonno lo ripeteva sempre, ma non\n"
	.string "ho mai capito cosa volesse dire.$"

