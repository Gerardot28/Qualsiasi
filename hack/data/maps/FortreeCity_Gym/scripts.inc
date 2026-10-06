FortreeCity_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FortreeCity_Gym_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, FortreeCity_Gym_OnWarp
	.byte 0

FortreeCity_Gym_OnTransition:
	special RotatingGate_InitPuzzle
	setflag FLAG_TEMP_HIDE_FOLLOWER @ Hide follower bc of rotating gates
	end

FortreeCity_Gym_OnWarp:
	map_script_2 VAR_TEMP_0, VAR_TEMP_0, FortreeCity_Gym_EventScript_InitRotatingGates
	.2byte 0

@ NOTE: This rotating gate puzzle makes use of VAR_TEMP_0 - VAR_TEMP_3
FortreeCity_Gym_EventScript_InitRotatingGates::
	special RotatingGate_InitPuzzleAndGraphics
	end

FortreeCity_Gym_EventScript_Winona::
	trainerbattle_single TRAINER_WINONA_1, FortreeCity_Gym_Text_WinonaIntro, FortreeCity_Gym_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, FortreeCity_Gym_EventScript_WinonaRematch
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, FortreeCity_Gym_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeated::
	message FortreeCity_Gym_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox FortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FORTREE_GYM
	setflag FLAG_BADGE06_GET
	setvar VAR_0x8008, 6
	call Common_EventScript_SetGymTrainers
	call FortreeCity_Gym_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox FortreeCity_Gym_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_GiveAerialAce2::
	giveitem ITEM_TM_AERIAL_ACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox FortreeCity_Gym_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_AERIAL_ACE
	release
	end

FortreeCity_Gym_EventScript_GiveAerialAce::
	giveitem ITEM_TM_AERIAL_ACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox FortreeCity_Gym_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_AERIAL_ACE
	return

FortreeCity_Gym_EventScript_WinonaRematch::
	trainerbattle_rematch_double TRAINER_WINONA_1, FortreeCity_Gym_Text_WinonaPreRematch, FortreeCity_Gym_Text_WinonaRematchDefeat, FortreeCity_Gym_Text_WinonaRematchNeedTwoMons
	msgbox FortreeCity_Gym_Text_WinonaPostRematch, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Jared::
	trainerbattle_single TRAINER_JARED, FortreeCity_Gym_Text_JaredIntro, FortreeCity_Gym_Text_JaredDefeat
	msgbox FortreeCity_Gym_Text_JaredPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Edwardo::
	trainerbattle_single TRAINER_EDWARDO, FortreeCity_Gym_Text_EdwardoIntro, FortreeCity_Gym_Text_EdwardoDefeat
	msgbox FortreeCity_Gym_Text_EdwardoPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Flint::
	trainerbattle_single TRAINER_FLINT, FortreeCity_Gym_Text_FlintIntro, FortreeCity_Gym_Text_FlintDefeat
	msgbox FortreeCity_Gym_Text_FlintPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Ashley::
	trainerbattle_single TRAINER_ASHLEY, FortreeCity_Gym_Text_AshleyIntro, FortreeCity_Gym_Text_AshleyDefeat
	msgbox FortreeCity_Gym_Text_AshleyPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Humberto::
	trainerbattle_single TRAINER_HUMBERTO, FortreeCity_Gym_Text_HumbertoIntro, FortreeCity_Gym_Text_HumbertoDefeat
	msgbox FortreeCity_Gym_Text_HumbertoPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Darius::
	trainerbattle_single TRAINER_DARIUS, FortreeCity_Gym_Text_DariusIntro, FortreeCity_Gym_Text_DariusDefeat
	msgbox FortreeCity_Gym_Text_DariusPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_FORTREE_GYM, FortreeCity_Gym_EventScript_GymGuidePostVictory
	msgbox FortreeCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_GymGuidePostVictory::
	msgbox FortreeCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE06_GET, FortreeCity_Gym_EventScript_GymStatueCertified
	goto FortreeCity_Gym_EventScript_GymStatue
	end

FortreeCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE06_GET, FortreeCity_Gym_EventScript_GymStatueCertified
	goto FortreeCity_Gym_EventScript_GymStatue
	end

FortreeCity_Gym_EventScript_GymStatueCertified::
	msgbox FortreeCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

FortreeCity_Gym_EventScript_GymStatue::
	msgbox FortreeCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

FortreeCity_Gym_Text_GymGuideAdvice:
	.string "Ehilà, come va, futuro Campione\n"
	.string "{PLAYER}?\p"
	.string "Alice, la Capopalestra di Forestopoli,\n"
	.string "è una maestra dei Pokémon Volante.\p"
	.string "Ti aspetta in fondo alla Palestra,\n"
	.string "oltre le porte girevoli.\p"
	.string "Aspetta nuovi sfidanti pronti\n"
	.string "a spiccare il volo!\p"
	.string "Forza, vai!$"

FortreeCity_Gym_Text_GymGuidePostVictory:
	.string "Ce l'hai fatta!\n"
	.string "Decollo riuscito!$"

FortreeCity_Gym_Text_JaredIntro:
	.string "Ammira l'elegante stile di lotta\n"
	.string "dei Pokémon Uccello!$"

FortreeCity_Gym_Text_JaredDefeat:
	.string "Tu…\n"
	.string "Tu sei forte…$"

FortreeCity_Gym_Text_JaredPostBattle:
	.string "Un Allenatore deve essere sveglio per\n"
	.string "stare dietro a ogni imprevisto.\p"
	.string "Hai abbastanza sale in zucca per\n"
	.string "arrivare dalla nostra Capopalestra?$"

FortreeCity_Gym_Text_EdwardoIntro:
	.string "Chi ama i Pokémon Uccello sogna\n"
	.string "di entrare in questa Palestra.\p"
	.string "E io, da membro della Palestra di\n"
	.string "Forestopoli, non posso perdere!$"

FortreeCity_Gym_Text_EdwardoDefeat:
	.string "Troppo peso sulle mie ali…\n"
	.string "Sto perdendo quota…$"

FortreeCity_Gym_Text_EdwardoPostBattle:
	.string "Il mondo è enorme e pieno di\n"
	.string "Allenatori fortissimi.\p"
	.string "E con tutti questi Varchi, forse di\n"
	.string "mondi ce n'è più di uno!\p"
	.string "Continuerò ad allenarmi per\n"
	.string "diventare ancora più forte.$"

FortreeCity_Gym_Text_FlintIntro:
	.string "Alice, la nostra Capopalestra, non ha\n"
	.string "bisogno di scomodarsi per te!\p"
	.string "Basto e avanzo io!$"

FortreeCity_Gym_Text_FlintDefeat:
	.string "Alice, io…\n"
	.string "Ho perso!$"

FortreeCity_Gym_Text_FlintPostBattle:
	.string "Alice è carina ed è fortissima.\n"
	.string "È la Capopalestra perfetta!\p"
	.string "Ehm… sto arrossendo…$"

FortreeCity_Gym_Text_AshleyIntro:
	.string "Mi ha allenata Alice in persona!\n"
	.string "Non sarà facile battermi!$"

FortreeCity_Gym_Text_AshleyDefeat:
	.string "Mi hai battuta…$"

FortreeCity_Gym_Text_AshleyPostBattle:
	.string "Grazie ad Alice, a Forestopoli si vive\n"
	.string "senza paura…\l"
	.string "Anche quando in cielo si apre un Varco.$"

FortreeCity_Gym_Text_HumbertoIntro:
	.string "Quando Alice lotta, il suo viso brilla\n"
	.string "di una splendida determinazione…\p"
	.string "Non ti permetterò di ammirare\n"
	.string "uno spettacolo così bello!$"

FortreeCity_Gym_Text_HumbertoDefeat:
	.string "Argh!\n"
	.string "Non sono riuscito a fermarti.$"

FortreeCity_Gym_Text_HumbertoPostBattle:
	.string "Occhio, eh!\n"
	.string "Se ti distrai a guardare Alice,\l"
	.string "finisci per precipitare come un sasso!$"

FortreeCity_Gym_Text_DariusIntro:
	.string "Spero tu sappia che esistono\n"
	.string "Pokémon Volante di ogni genere.\p"
	.string "Lo sai, vero?$"

FortreeCity_Gym_Text_DariusDefeat:
	.string "Vedo che sei ben preparato!$"

FortreeCity_Gym_Text_DariusPostBattle:
	.string "D'accordo, mi hai battuto.\n"
	.string "Ma occhio! I Pokémon della nostra\l"
	.string "Capopalestra Alice non scherzano.$"

FortreeCity_Gym_Text_WinonaIntro:
	.string "Sono Alice, la Capopalestra della\n"
	.string "Palestra Pokémon di Forestopoli.\p"
	.string "Sono diventata una cosa sola con i\n"
	.string "Pokémon Uccello e ho solcato i cieli…\p"
	.string "Dall'alto si vede una linea di luce\n"
	.string "sopra il mare. La chiamano Cicatrice.\l"
	.string "Volando, la si sente respirare.\p"
	.string "Per quanto dura sia la lotta, noi\n"
	.string "trionfiamo sempre con grazia…\p"
	.string "Ammira la danza elegante che io e\n"
	.string "i miei Pokémon Uccello eseguiamo!$"

FortreeCity_Gym_Text_WinonaDefeat:
	.string "Mai avevo visto un Allenatore\n"
	.string "guidare i suoi Pokémon con più\l"
	.string "grazia di me…\p"
	.string "In riconoscimento della tua bravura,\n"
	.string "ti consegno questa Medaglia.$"

FortreeCity_Gym_Text_ReceivedFeatherBadge:
	.string "{PLAYER} riceve la Medaglia Piuma\n"
	.string "da Alice.$"

FortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis:
	.string "Con la Medaglia Piuma, ti obbediranno\n"
	.string "tutti i Pokémon fino al livello 70,\l"
	.string "anche quelli ricevuti con gli scambi.\p"
	.string "Potrai anche usare la mossa MN Volo\n"
	.string "fuori dalla lotta.\p"
	.string "E questo…\n"
	.string "Questo è un mio regalo.$"

FortreeCity_Gym_Text_ExplainAerialAce:
	.string "La MT40 contiene Aeroassalto.\p"
	.string "La sua velocità…\n"
	.string "Nessun Pokémon può schivarla.\p"
	.string "… … … … … …$"

FortreeCity_Gym_Text_RegisteredWinona:
	.string "La Capopalestra Alice è stata\n"
	.string "registrata nel PokéNav.$"

FortreeCity_Gym_Text_WinonaPostBattle:
	.string "Anche se ho perso contro di te,\n"
	.string "resterò fedele ai Pokémon Uccello.\p"
	.string "Quando volerai sopra il mare, guarda\n"
	.string "l'orizzonte: forse vedrai la Cicatrice.$"

FortreeCity_Gym_Text_GymStatue:
	.string "Palestra Pokémon di Forestopoli$"

FortreeCity_Gym_Text_GymStatueCertified:
	.string "Palestra Pokémon di Forestopoli\p"
	.string "Allenatori certificati da Alice:\n"
	.string "{PLAYER}$"

FortreeCity_Gym_Text_WinonaPreRematch:
	.string "Alice: Noi umani non possiamo sfuggire\n"
	.string "alla gravità che ci lega a terra.\p"
	.string "Ma puntando all'eccellenza, abbiamo\n"
	.string "il potere di spiccare il volo!\p"
	.string "Tu hai raggiunto la vetta più alta:\n"
	.string "sei il Campione in carica.\p"
	.string "Ti prego, mostrami la tua forza\n"
	.string "a pieno volo!$"

FortreeCity_Gym_Text_WinonaRematchDefeat:
	.string "Ancora una volta non ho raggiunto\n"
	.string "la tua altezza…$"

FortreeCity_Gym_Text_WinonaPostRematch:
	.string "Alice: Anche se ho perso, le ali\n"
	.string "del mio cuore restano intatte.\p"
	.string "Posso rialzarmi e volare ancora,\n"
	.string "e poi ancora.\p"
	.string "Ne sono certa!$"

FortreeCity_Gym_Text_WinonaRematchNeedTwoMons:
	.string "Alice: Noi umani non possiamo sfuggire\n"
	.string "alla gravità che ci lega a terra.\p"
	.string "Ma puntando all'eccellenza, abbiamo\n"
	.string "il potere di spiccare il volo!\p"
	.string "Tu hai raggiunto la vetta più alta:\n"
	.string "sei il Campione in carica.\p"
	.string "Ti prego, mostrami la tua forza\n"
	.string "a pieno volo!\p"
	.string "Oh, no… Hai con te un solo\n"
	.string "Pokémon?\p"
	.string "Torna con almeno due Pokémon,\n"
	.string "ti prego.$"
