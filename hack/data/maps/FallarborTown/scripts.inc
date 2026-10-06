FallarborTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FallarborTown_OnTransition
	.byte 0

FallarborTown_OnTransition:
	setflag FLAG_VISITED_FALLARBOR_TOWN
	setvar VAR_CONTEST_HALL_STATE, 0
	clearflag FLAG_CONTEST_SKETCH_CREATED
	end

FallarborTown_EventScript_ExpertM::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, FallarborTown_EventScript_ExpertMNormal
	msgbox FallarborTown_Text_ShadyCharactersCozmosHome, MSGBOX_DEFAULT
	release
	end

FallarborTown_EventScript_ExpertMNormal::
	msgbox FallarborTown_Text_RegionKnownForMeteors, MSGBOX_DEFAULT
	release
	end

FallarborTown_EventScript_Girl::
	msgbox FallarborTown_Text_MyPreciousAzurill, MSGBOX_NPC
	end

FallarborTown_EventScript_Gentleman::
	msgbox FallarborTown_Text_HaveYouChallengedFlannery, MSGBOX_NPC
	end

FallarborTown_EventScript_Azurill::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_AZURILL, CRY_MODE_NORMAL
	msgbox FallarborTown_Text_Azurill, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FallarborTown_EventScript_BattleTentSign::
	msgbox FallarborTown_Text_BattleTentSign, MSGBOX_SIGN
	end

FallarborTown_EventScript_TownSign::
	msgbox FallarborTown_Text_TownSign, MSGBOX_SIGN
	end

FallarborTown_EventScript_MoveTutorSign::
	msgbox FallarborTown_Text_MoveTutorSign, MSGBOX_SIGN
	end

FallarborTown_Text_ShadyCharactersCozmosHome:
	.string "Sta succedendo qualcosa,\n"
	.string "e non mi piace per niente!\p"
	.string "Ho visto dei tipi loschi coi mantelli\n"
	.string "rossi entrare e uscire da casa\l"
	.string "del Prof. Cozmo…$"

FallarborTown_Text_RegionKnownForMeteors:
	.string "Questa zona è famosa per le meteore\n"
	.string "fin dai tempi antichi.\p"
	.string "Si dice che le Cascate Meteora siano\n"
	.string "state scavate da un meteorite caduto\l"
	.string "tanto, tanto tempo fa.\p"
	.string "Il Prof. Cozmo giura che quei sassi\n"
	.string "vengono da un altro mondo. Chissà…$"

FallarborTown_Text_MyPreciousAzurill:
	.string "Guarda! Guarda qua!\n"
	.string "Questo è il mio adorato Azurill!\p"
	.string "È liscio, morbido e tutto coccoloso!$"

FallarborTown_Text_Azurill:
	.string "Azurill: Rurirurù.$"

FallarborTown_Text_HaveYouChallengedFlannery:
	.string "Hai già sfidato Fiammetta, la\n"
	.string "Capopalestra di Cuordilava?\p"
	.string "Suo nonno era famoso: un tempo\n"
	.string "faceva parte dei Superquattro\l"
	.string "della Lega Pokémon.\p"
	.string "È alle prime armi, ma non mi\n"
	.string "stupirei se un giorno diventasse\l"
	.string "una grande Allenatrice anche lei.$"

FallarborTown_Text_BattleTentSign:
	.string "Tenda Lotta di Brunifoglia\n"
	.string "“Raduno delle squadre più forti!”$"

FallarborTown_Text_TownSign:
	.string "Brunifoglia\n"
	.string "“Orticelli e campi sotto le stelle.”$"

FallarborTown_Text_MoveTutorSign:
	.string "Casa dell'Insegnamosse\n"
	.string "“Nuove mosse per i tuoi Pokémon.”$"

