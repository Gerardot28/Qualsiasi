FallarborTown_BattleTentCorridor_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, FallarborTown_BattleTentCorridor_OnFrame
	.byte 0

FallarborTown_BattleTentCorridor_OnFrame:
	map_script_2 VAR_TEMP_0, 0, FallarborTown_BattleTentCorridor_EventScript_EnterCorridor
	.2byte 0

FallarborTown_BattleTentCorridor_EventScript_EnterCorridor::
	lockall
	setvar VAR_TEMP_0, 1
	applymovement LOCALID_FALLARBOR_TENT_CORRIDOR_ATTENDANT, FallarborTown_BattleTentCorridor_Movement_WalkToDoor
	applymovement LOCALID_PLAYER, FallarborTown_BattleTentCorridor_Movement_WalkToDoor
	waitmovement 0
	opendoor 2, 1
	waitdooranim
	applymovement LOCALID_FALLARBOR_TENT_CORRIDOR_ATTENDANT, FallarborTown_BattleTentCorridor_Movement_AttendantEnterDoor
	applymovement LOCALID_PLAYER, FallarborTown_BattleTentCorridor_Movement_PlayerEnterDoor
	waitmovement 0
	closedoor 2, 1
	waitdooranim
	setvar VAR_0x8006, 0
	warp MAP_FALLARBOR_TOWN_BATTLE_TENT_BATTLE_ROOM, 4, 4
	waitstate
	releaseall
	end

FallarborTown_BattleTentCorridor_Movement_WalkToDoor:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

FallarborTown_BattleTentCorridor_Movement_PlayerEnterDoor:
	walk_up
FallarborTown_BattleTentCorridor_Movement_AttendantEnterDoor:
	walk_up
	set_invisible
	step_end

@ Leftover text from when this was a Contest Hall in R/S
@ Unused
FallarborTown_ContestHall_Text_DoAllRightInPreliminary:
	.string "Nel turno preliminare ce la caviamo,\n"
	.string "ma nelle esibizioni non vinciamo mai…\p"
	.string "Forse dovrei guardare cosa fanno\n"
	.string "gli altri concorrenti…$"

@ Unused
FallarborTown_ContestHall_Text_MonAllTheseRibbons:
	.string "Guarda!\n"
	.string "Il mio Pokémon ha vinto questi Fiocchi!\p"
	.string "E i tuoi Pokémon ne hanno vinti?\n"
	.string "Puoi controllarli sul PokéNav.$"

@ Unused
FallarborTown_ContestHall_Text_CantWinEverywhere:
	.string "Non riesco a battere i Capipalestra…\p"
	.string "Non riesco a vincere le Gare…\p"
	.string "Sono stato qui, là e dappertutto,\n"
	.string "e tutto per niente…$"

@ Unused
FallarborTown_ContestHall_Text_SuperRankStage:
	.string "Gare Pokémon\n"
	.string "Palco del Livello Super!$"

