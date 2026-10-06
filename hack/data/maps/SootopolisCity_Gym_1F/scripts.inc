SootopolisCity_Gym_1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SootopolisCity_Gym_1F_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, SootopolisCity_Gym_1F_OnResume
	map_script MAP_SCRIPT_ON_LOAD, SootopolisCity_Gym_1F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, SootopolisCity_Gym_1F_OnTransition
	.byte 0

SootopolisCity_Gym_1F_OnTransition:
	setvar VAR_ICE_STEP_COUNT, 1
	end

SootopolisCity_Gym_1F_OnResume:
	setstepcallback STEP_CB_SOOTOPOLIS_ICE
	end

SootopolisCity_Gym_1F_OnLoad:
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special SetSootopolisGymCrackedIceMetatiles
	end

SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles::
	goto_if_lt VAR_ICE_STEP_COUNT, 8, SootopolisCity_Gym_1F_EventScript_StopCheckingStairs  @ All stairs ice
	goto_if_lt VAR_ICE_STEP_COUNT, 28, SootopolisCity_Gym_1F_EventScript_OpenFirstStairs
	goto_if_lt VAR_ICE_STEP_COUNT, 67, SootopolisCity_Gym_1F_EventScript_OpenFirstAndSecondStairs
	setmetatile 8, 4, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 5, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_OpenFirstAndSecondStairs::
	setmetatile 8, 10, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 11, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_OpenFirstStairs::
	setmetatile 8, 15, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 16, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_StopCheckingStairs::
	return

SootopolisCity_Gym_1F_OnFrame:
	map_script_2 VAR_ICE_STEP_COUNT, 8, SootopolisCity_Gym_1F_EventScript_UnlockFirstStairs
	map_script_2 VAR_ICE_STEP_COUNT, 28, SootopolisCity_Gym_1F_EventScript_UnlockSecondStairs
	map_script_2 VAR_ICE_STEP_COUNT, 67, SootopolisCity_Gym_1F_EventScript_UnlockThirdStairs
	map_script_2 VAR_ICE_STEP_COUNT, 0, SootopolisCity_Gym_1F_EventScript_FallThroughIce
	.2byte 0

SootopolisCity_Gym_1F_EventScript_UnlockFirstStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_UnlockSecondStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_UnlockThirdStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_FallThroughIce::
	lockall
	delay 20
	applymovement LOCALID_PLAYER, SootopolisCity_Gym_1F_Movement_FallThroughIce
	waitmovement 0
	playse SE_FALL
	delay 60
	warphole MAP_SOOTOPOLIS_CITY_GYM_B1F
	waitstate
	end

SootopolisCity_Gym_1F_Movement_FallThroughIce:
	set_invisible
	step_end

SootopolisCity_Gym_1F_EventScript_Juan::
	trainerbattle_single TRAINER_JUAN_1, SootopolisCity_Gym_1F_Text_JuanIntro, SootopolisCity_Gym_1F_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, SootopolisCity_Gym_1F_EventScript_JuanRematch
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, SootopolisCity_Gym_1F_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeated::
	message SootopolisCity_Gym_1F_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox SootopolisCity_Gym_1F_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_SOOTOPOLIS_GYM
	setflag FLAG_BADGE08_GET
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_RESIDENTS
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_STEVEN
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_WALLACE
	setvar VAR_SOOTOPOLIS_CITY_STATE, 6
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_MAN_1
	setvar VAR_0x8008, 8
	call Common_EventScript_SetGymTrainers
	call SootopolisCity_Gym_1F_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox SootopolisCity_Gym_1F_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_GiveWaterPulse::
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox SootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_WATER_PULSE
	return

SootopolisCity_Gym_1F_EventScript_GiveWaterPulse2::
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox SootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_WATER_PULSE
	release
	end

SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge::
	msgbox SootopolisCity_Gym_1F_Text_GoGetFortreeBadge, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanRematch::
	trainerbattle_rematch_double TRAINER_JUAN_1, SootopolisCity_Gym_1F_Text_JuanPreRematch, SootopolisCity_Gym_1F_Text_JuanRematchDefeat, SootopolisCity_Gym_1F_Text_JuanRematchNeedTwoMons
	msgbox SootopolisCity_Gym_1F_Text_JuanPostRematch, MSGBOX_AUTOCLOSE
	end

SootopolisCity_Gym_1F_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SOOTOPOLIS_GYM, SootopolisCity_Gym_1F_EventScript_GymGuidePostVictory
	msgbox SootopolisCity_Gym_1F_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_GymGuidePostVictory::
	msgbox SootopolisCity_Gym_1F_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, SootopolisCity_Gym_1F_EventScript_GymStatueCertified
	goto SootopolisCity_Gym_1F_EventScript_GymStatue
	end

SootopolisCity_Gym_1F_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, SootopolisCity_Gym_1F_EventScript_GymStatueCertified
	goto SootopolisCity_Gym_1F_EventScript_GymStatue
	end

SootopolisCity_Gym_1F_EventScript_GymStatueCertified::
	msgbox SootopolisCity_Gym_1F_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_Gym_1F_EventScript_GymStatue::
	msgbox SootopolisCity_Gym_1F_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_Gym_1F_Text_GymGuideAdvice:
	.string "Ehilà, futuro Campione {PLAYER}!\n"
	.string "Come te la passi?\p"
	.string "Rodolfo, il Capopalestra di Ceneride,\n"
	.string "è un maestro dei Pokémon Acqua.\p"
	.string "Ma per arrivare fino a lui dovrai\n"
	.string "attraversare un pavimento di ghiaccio…\p"
	.string "Mi dispiace, ma stavolta i miei\n"
	.string "consigli finiscono qui.\p"
	.string "Il resto della strada dovrai\n"
	.string "percorrerlo con le tue gambe!$"

SootopolisCity_Gym_1F_Text_GymGuidePostVictory:
	.string "Wow! Hai battuto perfino Rodolfo,\n"
	.string "che dicevano il più forte di Hoenn!\p"
	.string "Bene! Ora dai un'occhiata alla tua\n"
	.string "Scheda Allenatore.\p"
	.string "Se hai tutte le Medaglie, sei pronto\n"
	.string "per la sfida alla Lega Pokémon!$"

SootopolisCity_Gym_1F_Text_JuanIntro:
	.string "Lascia che ti chieda una cosa.\n"
	.string "Lo sapevi?\l"
	.string "Ah, non dovrei fare il misterioso.\p"
	.string "Sono stato io a insegnare ad Adriano\n"
	.string "tutto ciò che sa sui Pokémon.\p"
	.string "Un tempo avevo rinunciato al mio\n"
	.string "ruolo di Capopalestra.\p"
	.string "Al mio posto, avevo affidato la\n"
	.string "Palestra ad Adriano.\p"
	.string "Ma la Cicatrice ha ripreso a pulsare,\n"
	.string "e Adriano doveva vegliare sulla\l"
	.string "Grotta dei Tempi. Così, eccomi qui.\p"
	.string "Ceneride custodisce la leggenda delle\n"
	.string "Ancore da tremila anni.\p"
	.string "E tu quella leggenda l'hai vissuta\n"
	.string "in prima persona.\p"
	.string "Ma basta chiacchiere.\n"
	.string "Cominciamo, vuoi?\p"
	.string "Mostrami la tua eleganza, e ammira\n"
	.string "la nostra arte:\p"
	.string "una grandiosa illusione d'acqua,\n"
	.string "scolpita da me e dai miei Pokémon!$"

@ NOTE: This defeat text actually causes a buffer overflow. It's about 50 bytes too long for
@ the gDisplayedStringBattle buffer that it's put into, and it stomps all over the gBattleTextBuffs
@ after, as well as the otherwise unused array after that. One wonders if that's the reason for
@ the existence of that unused array of ints.
SootopolisCity_Gym_1F_Text_JuanDefeat:
	.string "Ahahah, splendido!\n"
	.string "D'accordo, la vittoria è tua.\p"
	.string "In te vedo brillare un talento\n"
	.string "capace di superare ogni ostacolo.\p"
	.string "Eppure, rispetto a me o ad Adriano,\n"
	.string "ti manca un pizzico di eleganza.\p"
	.string "Forse dovrei prestarti il mio abito?\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Ahahah, scherzavo, naturalmente!\p"
	.string "Invece dei miei abiti, ti donerò\n"
	.string "la Medaglia Pioggia!$"

SootopolisCity_Gym_1F_Text_ReceivedRainBadge:
	.string "{PLAYER} riceve la Medaglia Pioggia\n"
	.string "da Rodolfo.$"

SootopolisCity_Gym_1F_Text_ExplainRainBadgeTakeThis:
	.string "Con la Medaglia Pioggia, tutti i tuoi\n"
	.string "Pokémon obbediranno a ogni tuo\l"
	.string "comando.\p"
	.string "La Medaglia Pioggia ti permette anche\n"
	.string "di usare la MN Cascata per risalire\l"
	.string "le pareti d'acqua delle cascate.\p"
	.string "E perché tu non dimentichi mai la\n"
	.string "lotta che abbiamo condiviso,\l"
	.string "prendi questa…$"

SootopolisCity_Gym_1F_Text_ExplainWaterPulse:
	.string "La Macchina Tecnica che ti ho dato\n"
	.string "contiene Idropulsar.\p"
	.string "Investe il bersaglio con onde\n"
	.string "ultrasoniche che a volte lo\l"
	.string "lasciano confuso.\p"
	.string "… … … … … …$"

SootopolisCity_Gym_1F_Text_RegisteredJuan:
	.string "Hai registrato il Capopalestra\n"
	.string "Rodolfo nel PokéNav.$"

SootopolisCity_Gym_1F_Text_JuanPostBattle:
	.string "Gli Allenatori che hanno raccolto\n"
	.string "tutte le Medaglie di Hoenn devono\l"
	.string "puntare verso l'ultima meta:\p"
	.string "la Lega Pokémon.\p"
	.string "Viaggia fino all'estremo est di Hoenn,\n"
	.string "sull'isola di Iridopoli.\p"
	.string "Là troverai la Lega Pokémon.\n"
	.string "Il mare stesso ti indicherà la via.$"

SootopolisCity_Gym_1F_Text_GoGetFortreeBadge:
	.string "Ti resta ancora una sola Medaglia\n"
	.string "da conquistare a Hoenn.\p"
	.string "Se vuoi sfidare la Lega Pokémon,\n"
	.string "devi ottenere l'ultima Medaglia\l"
	.string "nella Palestra di Forestopoli.$"

SootopolisCity_Gym_1F_Text_GymStatue:
	.string "Palestra Pokémon di Ceneride$"

SootopolisCity_Gym_1F_Text_GymStatueCertified:
	.string "Palestra Pokémon di Ceneride\p"
	.string "Allenatori certificati da Rodolfo:\n"
	.string "{PLAYER}$"

SootopolisCity_Gym_1F_Text_JuanPreRematch:
	.string "Rodolfo: Ah, la Palestra era tornata\n"
	.string "alla sua consueta quiete…\p"
	.string "Ma il nostro giovane tifone è tornato\n"
	.string "per metterci di nuovo alla prova!\p"
	.string "Ma certo, amico mio, con piacere!\p"
	.string "Sarò lieto di danzare con te\n"
	.string "tutte le volte che vorrai!$"

SootopolisCity_Gym_1F_Text_JuanRematchDefeat:
	.string "Ahahah, la vittoria è tua!\n"
	.string "Mi hai sconfitto ancora una volta!$"

SootopolisCity_Gym_1F_Text_JuanPostRematch:
	.string "Rodolfo: Se ti chiedessi di diventare\n"
	.string "mio allievo, rifiuteresti, ne sono certo.\p"
	.string "Vorrei farti dono del mio mantello.\p"
	.string "Ma anche stavolta rifiuteresti.\n"
	.string "Me lo immagino già.\p"
	.string "E questo, amico mio, è un segno\n"
	.string "inconfondibile di nobiltà!$"

SootopolisCity_Gym_1F_Text_JuanRematchNeedTwoMons:
	.string "Rodolfo: Ah, la Palestra era tornata\n"
	.string "alla sua consueta quiete…\p"
	.string "Ma il nostro giovane tifone è tornato\n"
	.string "per metterci di nuovo alla prova!\p"
	.string "Ma certo, amico mio, con piacere!\n"
	.string "Sarò lieto di danzare con te…\p"
	.string "Ah, no, no, no.\n"
	.string "Hai con te un solo Pokémon.\p"
	.string "Torna, ti prego, con almeno due\n"
	.string "Pokémon. Magari anche di più.$"

