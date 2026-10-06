MossdeepCity_GameCorner_1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, MossdeepCity_GameCorner_1F_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, MossdeepCity_GameCorner_1F_OnWarp
	map_script MAP_SCRIPT_ON_LOAD, CableClub_OnLoad
	.byte 0

MossdeepCity_GameCorner_1F_OnWarp:
	map_script_2 VAR_CABLE_CLUB_STATE, USING_MINIGAME, CableClub_EventScript_CheckTurnAttendant
	.2byte 0

MossdeepCity_GameCorner_1F_OnFrame:
	map_script_2 VAR_CABLE_CLUB_STATE, USING_MINIGAME, CableClub_EventScript_ExitMinigameRoom
	.2byte 0

@ Script is redundant, the label in the goto also does lock and faceplayer
MossdeepCity_GameCorner_1F_EventScript_InfoMan::
	lock
	faceplayer
	goto MossdeepCity_GameCorner_1F_EventScript_InfoMan2
	release
	end

@ Script is redundant, the label in the goto also does lock and faceplayer
MossdeepCity_GameCorner_1F_EventScript_OldMan::
	lock
	faceplayer
	goto MossdeepCity_GameCorner_1F_EventScript_OldMan2
	release
	end


@ Everything below is leftover from RS
@ The house that was here in RS was moved to Sootopolis and replaced by the Game Corner
@ The text below is unused and duplicated in its replacement in Sootopolis City
@ And the BG Door event was moved inaccessibly into a wall

RS_MysteryEventsHouse_EventScript_Door::
	msgbox RS_MysteryEventsHouse_Text_DoorIsLocked, MSGBOX_SIGN
	end

RS_MysteryEventsHouse_Text_OldManGreeting:
	.string "Da giovane ho girato tutta Hoenn\n"
	.string "come Allenatore di Pokémon.\p"
	.string "Ora che sono un vecchio barbagianni,\n"
	.string "il mio unico svago è guardare lottare\l"
	.string "i giovani Allenatori.$"

RS_MysteryEventsHouse_Text_DoorIsLocked:
	.string "La porta sembra chiusa a chiave.$"

RS_MysteryEventsHouse_Text_ChallengeVisitingTrainer:
	.string "Un Allenatore di nome {STR_VAR_1}\n"
	.string "è ospite a casa mia.\p"
	.string "Vuoi sfidare\n"
	.string "{STR_VAR_1}?$"

RS_MysteryEventsHouse_Text_YouWontBattle:
	.string "Non vuoi lottare? Che delusione,\n"
	.string "non poterti vedere in azione…$"

RS_MysteryEventsHouse_Text_KeepItToA3On3:
	.string "Oh, bene, bene!\p"
	.string "Ma la mia casa non è poi così solida.\p"
	.string "Posso chiedervi di limitarvi a una\n"
	.string "lotta tre contro tre?$"

RS_MysteryEventsHouse_Text_SaveYourProgress:
	.string "Prima di lottare, vi conviene\n"
	.string "salvare la partita.$"

RS_MysteryEventsHouse_Text_HopeToSeeAGoodMatch:
	.string "Spero di assistere a una bella lotta!$"

RS_MysteryEventsHouse_Text_BattleTie:
	.string "Dunque, è finita in parità.\p"
	.string "Che lotta splendida! Nessuno dei due\n"
	.string "ha ceduto di un passo!$"

RS_MysteryEventsHouse_Text_BattleWon:
	.string "Superlativo!\p"
	.string "Mi è sembrato di rivedere me stesso\n"
	.string "da giovane!$"

RS_MysteryEventsHouse_Text_BattleLost:
	.string "Ah, peccato per te!\p"
	.string "Ma è stata una bella lotta.\n"
	.string "Spero che la prossima volta vincerai.$"
