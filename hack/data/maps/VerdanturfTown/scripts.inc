VerdanturfTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VerdanturfTown_OnTransition
	.byte 0

VerdanturfTown_OnTransition:
	setflag FLAG_VISITED_VERDANTURF_TOWN
	setvar VAR_CONTEST_HALL_STATE, 0
	end

VerdanturfTown_EventScript_Twin::
	lock
	faceplayer
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, VerdanturfTown_EventScript_TwinTunnelOpen
	msgbox VerdanturfTown_Text_ManTryingToDigTunnel, MSGBOX_DEFAULT
	applymovement LOCALID_VERDANTURF_TWIN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

VerdanturfTown_EventScript_TwinTunnelOpen::
	msgbox VerdanturfTown_Text_ManDugTunnelForLove, MSGBOX_DEFAULT
	applymovement LOCALID_VERDANTURF_TWIN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

VerdanturfTown_EventScript_Man::
	msgbox VerdanturfTown_Text_AirCleanHere, MSGBOX_NPC
	end

VerdanturfTown_EventScript_Camper::
	msgbox VerdanturfTown_Text_MakeBattleTentDebut, MSGBOX_NPC
	end

VerdanturfTown_EventScript_Boy::
	lock
	faceplayer
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, VerdanturfTown_EventScript_BoyTunnelOpen
	msgbox VerdanturfTown_Text_GuyTryingToBustThroughCave, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_EventScript_BoyTunnelOpen::
	msgbox VerdanturfTown_Text_EasyToGetToRustboroNow, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_EventScript_TownSign::
	msgbox VerdanturfTown_Text_TownSign, MSGBOX_SIGN
	end

VerdanturfTown_EventScript_WandasHouseSign::
	msgbox VerdanturfTown_Text_WandasHouse, MSGBOX_SIGN
	end

VerdanturfTown_EventScript_BattleTentSign::
	msgbox VerdanturfTown_Text_BattleTentSign, MSGBOX_SIGN
	end

VerdanturfTown_EventScript_RusturfTunnelSign::
	msgbox VerdanturfTown_Text_RusturfTunnelSign, MSGBOX_SIGN
	end

VerdanturfTown_Text_ManTryingToDigTunnel:
	.string "Me l'ha detto il mio papà.\p"
	.string "Dice che in questo tunnel vivono\n"
	.string "tanti Pokémon timidi.\p"
	.string "Se sentono rumori forti si spaventano\n"
	.string "e fanno un gran baccano.\p"
	.string "Così hanno dovuto fermare i lavori\n"
	.string "del grande tunnel.\p"
	.string "Ma c'è un signore che sta provando\n"
	.string "a scavarlo tutto da solo!$"

VerdanturfTown_Text_ManDugTunnelForLove:
	.string "C'era una volta un signore che ha\n"
	.string "scavato un tunnel per la sua amata.\p"
	.string "Io non ci ho capito molto, ma vabbè!$"

VerdanturfTown_Text_AirCleanHere:
	.string "Per come soffiano i venti, la cenere\n"
	.string "del vulcano non arriva mai fin qui.\p"
	.string "L'aria è pulita e buonissima.\n"
	.string "Vivere qui fa bene perfino a chi ha\l"
	.string "una salute fragile e delicata.$"

VerdanturfTown_Text_MakeBattleTentDebut:
	.string "Io e i miei Pokémon siamo nel bel\n"
	.string "mezzo di una serie di vittorie!\p"
	.string "Così ho deciso di debuttare alla\n"
	.string "Tenda Lotta proprio qui a Mentania.$"

VerdanturfTown_Text_GuyTryingToBustThroughCave:
	.string "Hai visto la grotta accanto al\n"
	.string "Pokémon Market?\p"
	.string "Lì dentro c'è un tizio che cerca di\n"
	.string "frantumare i massi per sbucare\l"
	.string "dall'altra parte.\p"
	.string "Sarebbe fantastico poterci passare…\n"
	.string "Arrivare a Ferrugipoli sarebbe facile!$"

VerdanturfTown_Text_EasyToGetToRustboroNow:
	.string "La grotta accanto al Pokémon Market\n"
	.string "ora è un tunnel che porta di là.\p"
	.string "Fantastico! Ora è facile andare\n"
	.string "a Ferrugipoli a comprare le ultime\l"
	.string "novità della Devon S.p.A.!$"

VerdanturfTown_Text_TownSign:
	.string "Mentania\p"
	.string "“Altipiani battuti dal vento, col\n"
	.string "dolce profumo dell'erba.”$"

VerdanturfTown_Text_WandasHouse:
	.string "Casa di Wanda$"

VerdanturfTown_Text_BattleTentSign:
	.string "Tenda Lotta di Mentania\n"
	.string "“Lotte da lustrarsi gli occhi!”$"

VerdanturfTown_Text_RusturfTunnelSign:
	.string "Tunnel Menferro\n"
	.string "“Collega Ferrugipoli e Mentania”\p"
	.string "“Il progetto del tunnel è stato\n"
	.string "sospeso.”$"

