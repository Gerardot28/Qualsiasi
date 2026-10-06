PacifidlogTown_House4_MapScripts::
	.byte 0

PacifidlogTown_House4_EventScript_LittleGirl::
	msgbox PacifidlogTown_House4_Text_SkyPokemon, MSGBOX_NPC
	end

PacifidlogTown_House4_EventScript_Woman::
	msgbox PacifidlogTown_House4_Text_PeopleSawHighFlyingPokemon, MSGBOX_NPC
	end

PacifidlogTown_House4_EventScript_Boy::
	lock
	faceplayer
	msgbox PacifidlogTown_House4_Text_WhereDidYouComeFrom, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, PacifidlogTown_House4_EventScript_Yes
	goto_if_eq VAR_RESULT, NO, PacifidlogTown_House4_EventScript_No
	end

PacifidlogTown_House4_EventScript_Yes::
	msgbox PacifidlogTown_House4_Text_YesTown, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House4_EventScript_No::
	msgbox PacifidlogTown_House4_Text_YouHaveToComeFromSomewhere, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House4_Text_PeopleSawHighFlyingPokemon:
	.string "Dicono di aver visto un Pokémon verde\n"
	.string "volare altissimo sopra Hoenn.\p"
	.string "Ma vola sempre, senza mai fermarsi?\n"
	.string "Non avrà bisogno di riposare?\p"
	.string "Magari ha il nido in cima a quella\n"
	.string "torre che si vede dal mare, laggiù…$"

PacifidlogTown_House4_Text_SkyPokemon:
	.string "Un Pokémon nel cielo!\n"
	.string "Un Pokémon nel cielo!$"

PacifidlogTown_House4_Text_WhereDidYouComeFrom:
	.string "Ehi, tu! Mi dici da dove vieni?$"

PacifidlogTown_House4_Text_YesTown:
	.string "Sì?\n"
	.string "Vieni da Sì? È una città?\p"
	.string "Mai sentito un posto con quel nome.$"

PacifidlogTown_House4_Text_YouHaveToComeFromSomewhere:
	.string "No? Non ha senso!\n"
	.string "Da qualche parte dovrai pur venire.\p"
	.string "Oh! Aspetta! Non vorrai dirmi che sei\n"
	.string "nato in fondo al mare? O peggio…\l"
	.string "che sei sbucato fuori da un Varco?!$"

