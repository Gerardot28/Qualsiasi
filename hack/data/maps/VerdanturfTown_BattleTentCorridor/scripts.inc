VerdanturfTown_BattleTentCorridor_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VerdanturfTown_BattleTentCorridor_OnFrame
	.byte 0

VerdanturfTown_BattleTentCorridor_OnFrame:
	map_script_2 VAR_TEMP_0, 0, VerdanturfTown_BattleTentCorridor_EventScript_EnterCorridor
	.2byte 0

VerdanturfTown_BattleTentCorridor_EventScript_EnterCorridor::
	lockall
	setvar VAR_TEMP_0, 1
	applymovement LOCALID_VERDANTURF_TENT_CORRIDOR_ATTENDANT, VerdanturfTown_BattleTentCorridor_Movement_WalkToDoor
	applymovement LOCALID_PLAYER, VerdanturfTown_BattleTentCorridor_Movement_WalkToDoor
	waitmovement 0
	opendoor 2, 1
	waitdooranim
	applymovement LOCALID_VERDANTURF_TENT_CORRIDOR_ATTENDANT, VerdanturfTown_BattleTentCorridor_Movement_AttendantEnterDoor
	applymovement LOCALID_PLAYER, VerdanturfTown_BattleTentCorridor_Movement_PlayerEnterDoor
	waitmovement 0
	closedoor 2, 1
	waitdooranim
	setvar VAR_0x8006, 0
	warp MAP_VERDANTURF_TOWN_BATTLE_TENT_BATTLE_ROOM, 6, 5
	waitstate
	releaseall
	end

VerdanturfTown_BattleTentCorridor_Movement_WalkToDoor:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VerdanturfTown_BattleTentCorridor_Movement_PlayerEnterDoor:
	walk_up
VerdanturfTown_BattleTentCorridor_Movement_AttendantEnterDoor:
	walk_up
	set_invisible
	step_end


@ Leftover text from when this was a Contest Hall in R/S
@ Unused
VerdanturfTown_ContestHall_Text_WhichContestYouEntering:
	.string "A quale Gara partecipi?\n"
	.string "Vuoi un consiglio?\p"
	.string "In qualsiasi Gara, per esempio una\n"
	.string "Gara Grazia, non credo che al primo\l"
	.string "turno giudichino solo la grazia.\p"
	.string "Devi trovare modi migliori per\n"
	.string "allevare i tuoi Pokémon.$"

@ Unused
VerdanturfTown_ContestHall_Text_RaisedMonToBeCute:
	.string "Ho cresciuto il mio Pokémon perché\n"
	.string "fosse pieno di grazia.\p"
	.string "Poi ho scoperto che si possono\n"
	.string "iscrivere i Pokémon a una Gara Grazia!\p"
	.string "Sono così felice di averlo cresciuto\n"
	.string "con tanto amore…$"

@ Unused
VerdanturfTown_ContestHall_Text_MyMonRules:
	.string "Il mio Pokémon è il top!\p"
	.string "Ha classe, grinta, bellezza, grazia\n"
	.string "e acume. È perfetto!\p"
	.string "Tanto vale puntare alla vittoria\n"
	.string "in ogni singola Gara.$"

@ Unused
VerdanturfTown_ContestHall_Text_NormalRankStage:
	.string "Gare Pokémon\n"
	.string "Livello Normale!$"

