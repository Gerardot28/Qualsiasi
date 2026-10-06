Route116_TunnelersRestHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route116_TunnelersRestHouse_OnTransition
	.byte 0

Route116_TunnelersRestHouse_OnTransition:
	setflag FLAG_LANDMARK_TUNNELERS_REST_HOUSE
	end

Route116_TunnelersRestHouse_EventScript_Tunneler1::
	msgbox Route116_TunnelersRestHouse_Text_WeHadToStopBoring, MSGBOX_NPC
	end

Route116_TunnelersRestHouse_EventScript_Tunneler2::
	msgbox Route116_TunnelersRestHouse_Text_ManDiggingHisWayToVerdanturf, MSGBOX_NPC
	end

Route116_TunnelersRestHouse_EventScript_Tunneler3::
	lock
	faceplayer
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, Route116_TunnelersRestHouse_EventScript_TunnelOpened
	msgbox Route116_TunnelersRestHouse_Text_GetToVerdanturfWithoutTunnel, MSGBOX_DEFAULT
	release
	end

Route116_TunnelersRestHouse_EventScript_TunnelOpened::
	msgbox Route116_TunnelersRestHouse_Text_TunnelHasGoneThrough, MSGBOX_DEFAULT
	release
	end

Route116_TunnelersRestHouse_Text_WeHadToStopBoring:
	.string "Quel Tunnel Menferro, là fuori…\p"
	.string "All'inizio una squadra enorme scavava\n"
	.string "la roccia coi macchinari più moderni.\l"
	.string "Ma poi abbiamo dovuto fermarci.\p"
	.string "Pare che stessimo disturbando i\n"
	.string "Pokémon selvatici della zona. Alcuni\l"
	.string "sono arrivati da un Varco, poveretti,\l"
	.string "e si spaventano per un nonnulla.\p"
	.string "Così adesso non ci resta che starcene\n"
	.string "qui a poltrire senza fare niente.$"

Route116_TunnelersRestHouse_Text_ManDiggingHisWayToVerdanturf:
	.string "C'è un uomo che scava tutto da solo\n"
	.string "per arrivare fino a Mentania.\l"
	.string "Vuole passare a tutti i costi.\p"
	.string "Dice che, scavando poco alla volta\n"
	.string "e senza macchine, non disturberà\l"
	.string "i Pokémon e non rovinerà\l"
	.string "l'ambiente.\p"
	.string "Chissà se ce l'ha già fatta.$"

Route116_TunnelersRestHouse_Text_GetToVerdanturfWithoutTunnel:
	.string "Per arrivare a Mentania senza questo\n"
	.string "tunnel, dovresti attraversare il mare\l"
	.string "fino a Bluruvia, navigare fino a Porto\l"
	.string "Selcepoli e poi passare da Ciclamipoli.$"

Route116_TunnelersRestHouse_Text_TunnelHasGoneThrough:
	.string "Hai sentito? Il tunnel per Mentania\n"
	.string "è stato completato!\p"
	.string "A volte, se ci speri abbastanza forte,\n"
	.string "i sogni si avverano davvero.$"

