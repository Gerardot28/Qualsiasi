MtPyre_2F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CaveHole_CheckFallDownHole
	map_script MAP_SCRIPT_ON_TRANSITION, CaveHole_FixCrackedGround
	map_script MAP_SCRIPT_ON_RESUME, MtPyre_2F_SetHoleWarp
	.byte 0

MtPyre_2F_SetHoleWarp:
	setstepcallback STEP_CB_CRACKED_FLOOR
	setholewarp MAP_MT_PYRE_1F
	end

MtPyre_2F_EventScript_Woman::
	msgbox MtPyre_2F_Text_MemoriesOfSkitty, MSGBOX_NPC
	end

MtPyre_2F_EventScript_PokefanM::
	msgbox MtPyre_2F_Text_TumbledFromFloorAbove, MSGBOX_NPC
	end

MtPyre_2F_EventScript_Mark::
	trainerbattle_single TRAINER_MARK, MtPyre_2F_Text_MarkIntro, MtPyre_2F_Text_MarkDefeat
	msgbox MtPyre_2F_Text_MarkPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Luke::
	trainerbattle_double TRAINER_DEZ_AND_LUKE, MtPyre_2F_Text_LukeIntro, MtPyre_2F_Text_LukeDefeat, MtPyre_2F_Text_LukeNotEnoughMons
	msgbox MtPyre_2F_Text_LukePostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Dez::
	trainerbattle_double TRAINER_DEZ_AND_LUKE, MtPyre_2F_Text_DezIntro, MtPyre_2F_Text_DezDefeat, MtPyre_2F_Text_DezNotEnoughMons
	msgbox MtPyre_2F_Text_DezPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Leah::
	trainerbattle_single TRAINER_LEAH, MtPyre_2F_Text_LeahIntro, MtPyre_2F_Text_LeahDefeat
	msgbox MtPyre_2F_Text_LeahPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Zander::
	trainerbattle_single TRAINER_ZANDER, MtPyre_2F_Text_ZanderIntro, MtPyre_2F_Text_ZanderDefeat
	msgbox MtPyre_2F_Text_ZanderPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_Text_MemoriesOfSkitty:
	.string "Il ricordo del mio adorato Skitty…\n"
	.string "Solo a pensarci mi vengono le lacrime.$"

MtPyre_2F_Text_TumbledFromFloorAbove:
	.string "Ahi, ahi… Qua e là nel pavimento\n"
	.string "ci sono dei buchi.\p"
	.string "Non me ne sono accorto e sono\n"
	.string "precipitato dal piano di sopra.$"

MtPyre_2F_Text_MarkIntro:
	.string "Ehi! Anche tu a caccia di Pokémon?\n"
	.string "Mi sei venuto dietro! Che sfacciato!$"

MtPyre_2F_Text_MarkDefeat:
	.string "Aaaaaah!\n"
	.string "Scusa, scusa, perdonami!$"

MtPyre_2F_Text_MarkPostBattle:
	.string "Qui non viene mai nessuno, così\n"
	.string "speravo di trovare Pokémon rari.\p"
	.string "Magari qualcuno arrivato da un Varco…\n"
	.string "Ma ho trovato solo te.$"

MtPyre_2F_Text_LukeIntro:
	.string "Luke: Siamo qui per una scommessa.\p"
	.string "Eheh, se le faccio vedere quanto sono\n"
	.string "in gamba, si innamorerà di me. Sicuro!\p"
	.string "Ci sono! Ti straccio e le mostro\n"
	.string "quanto sono forte!$"

MtPyre_2F_Text_LukeDefeat:
	.string "Luke: Ops!$"

MtPyre_2F_Text_LukePostBattle:
	.string "Luke: Abbiamo perso, ma pazienza!\n"
	.string "Io resto qui al tuo fianco.\l"
	.string "La scommessa la vinciamo lo stesso!$"

MtPyre_2F_Text_LukeNotEnoughMons:
	.string "Luke: Se vuoi sfidarmi, porta\n"
	.string "qualche Pokémon in più.\p"
	.string "Sennò come faccio a far vedere alla\n"
	.string "mia ragazza quanto sono forte?$"

MtPyre_2F_Text_DezIntro:
	.string "Dez: Sono qui per una scommessa\n"
	.string "con il mio ragazzo.\p"
	.string "Ho una paura tremenda, ma c'è lui\n"
	.string "con me. Quindi va tutto bene.\p"
	.string "Ci sono! Ti batto, così il mio ragazzo\n"
	.string "farà un figurone!$"

MtPyre_2F_Text_DezDefeat:
	.string "Dez: Uaaah! Che paura!$"

MtPyre_2F_Text_DezPostBattle:
	.string "Dez: Siamo così innamorati che\n"
	.string "perdere non ci importa!$"

MtPyre_2F_Text_DezNotEnoughMons:
	.string "Dez: Se vuoi sfidarci, porta almeno\n"
	.string "due Pokémon.\p"
	.string "Il mio ragazzo è fortissimo.\n"
	.string "Con un Pokémon solo non hai speranze.$"

MtPyre_2F_Text_LeahIntro:
	.string "Il tuo è un volto sconosciuto…\n"
	.string "Vattene, prima che ti accada qualcosa!$"

MtPyre_2F_Text_LeahDefeat:
	.string "Mmh…\n"
	.string "Sei un osso duro.$"

MtPyre_2F_Text_LeahPostBattle:
	.string "La mia famiglia allena Pokémon su\n"
	.string "questo monte dai tempi della bisnonna…\p"
	.string "Proteggerlo è il mio dovere…\n"
	.string "Ora più che mai, con i Varchi che\l"
	.string "turbano il sonno degli spiriti…$"

MtPyre_2F_Text_ZanderIntro:
	.string "Kiyaaaaah!\n"
	.string "Che terrore!$"

MtPyre_2F_Text_ZanderDefeat:
	.string "Nooooooo!\n"
	.string "Ho perso la testa!$"

MtPyre_2F_Text_ZanderPostBattle:
	.string "Mi prende un colpo ogni volta che vedo\n"
	.string "qualcosa muoversi…\p"
	.string "Non dovevo venire ad allenarmi qui…$"

