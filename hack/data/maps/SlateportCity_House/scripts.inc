SlateportCity_House_MapScripts::
	.byte 0

SlateportCity_House_EventScript_PokefanM::
	msgbox SlateportCity_House_Text_NatureToDoWithStatGains, MSGBOX_NPC
	end

SlateportCity_House_EventScript_Girl::
	msgbox SlateportCity_House_Text_MustBeGoingToBattleTent, MSGBOX_NPC
	end

SlateportCity_House_Text_NatureToDoWithStatGains:
	.string "Il mio Pokémon ha natura Lesta.\p"
	.string "Ha la Velocità più alta di tutti\n"
	.string "gli altri Pokémon della mia squadra.\p"
	.string "Forse la natura influisce sulla\n"
	.string "crescita delle statistiche…$"

SlateportCity_House_Text_MustBeGoingToBattleTent:
	.string "Sei un Allenatore, vero?\p"
	.string "Se sei venuto a Porto Selcepoli,\n"
	.string "di sicuro punti alla Tenda Lotta!$"

