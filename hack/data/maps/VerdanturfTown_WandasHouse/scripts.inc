VerdanturfTown_WandasHouse_MapScripts::
	.byte 0

VerdanturfTown_WandasHouse_EventScript_Wally::
	lock
	faceplayer
	goto_if_set FLAG_WALLY_SPEECH, VerdanturfTown_WandasHouse_EventScript_WallyShortSpeech
	msgbox VerdanturfTown_WandasHouse_Text_StrongerSpeech, MSGBOX_DEFAULT
	setflag FLAG_WALLY_SPEECH
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallyShortSpeech::
	msgbox VerdanturfTown_WandasHouse_Text_StrongerSpeechShort, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysUncle::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WALLY_VICTORY_ROAD, VerdanturfTown_WandasHouse_EventScript_WallysUncleEverGrande
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, VerdanturfTown_WandasHouse_EventScript_WallysUncleSlippedOff
	msgbox VerdanturfTown_WandasHouse_Text_WallysNextDoor, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysUncleSlippedOff::
	msgbox VerdanturfTown_WandasHouse_Text_WallySlippedOff, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysUncleEverGrande::
	msgbox VerdanturfTown_WandasHouse_Text_WallyGoneThatFar, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WandasBoyfriend::
	msgbox VerdanturfTown_WandasHouse_Text_CanSeeGirlfriendEveryDay, MSGBOX_NPC
	end

VerdanturfTown_WandasHouse_EventScript_Wanda::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, VerdanturfTown_WandasHouse_EventScript_WandaDontWorry
	goto_if_set FLAG_DEFEATED_WALLY_MAUVILLE, VerdanturfTown_WandasHouse_EventScript_MeetWanda
	msgbox VerdanturfTown_WandasHouse_Text_DontWorryAboutWally, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_MeetWanda::
	msgbox VerdanturfTown_WandasHouse_Text_MeetWanda, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WandaDontWorry::
	msgbox VerdanturfTown_WandasHouse_Text_DontWorryAboutWally, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAunt::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WALLY_VICTORY_ROAD, VerdanturfTown_WandasHouse_EventScript_WallysAuntEverGrande
	goto_if_set FLAG_DEFEATED_LAVARIDGE_GYM, VerdanturfTown_WandasHouse_EventScript_WallysAuntAnythingHappened
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, VerdanturfTown_WandasHouse_EventScript_WallysAuntTunnelOpen
	msgbox VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendDriven, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAuntTunnelOpen::
	msgbox VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendWasDigging, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAuntAnythingHappened::
	msgbox VerdanturfTown_WandasHouse_Text_IfAnythingHappenedToWally, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_EventScript_WallysAuntEverGrande::
	msgbox VerdanturfTown_WandasHouse_Text_WallyWasInEverGrande, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_WandasHouse_Text_StrongerSpeech:
	.string "Lino: Ho perso contro di te, {PLAYER}…\n"
	.string "ma non mi sento più giù.\p"
	.string "Qui a Mentania respiro meglio.\n"
	.string "E adesso ho un nuovo obiettivo.\p"
	.string "Insieme al mio Ralts sfiderò le\n"
	.string "Palestre Pokémon e diventerò\l"
	.string "un grande Allenatore.\p"
	.string "Guardami, {PLAYER}, ti prego.\n"
	.string "Diventerò più forte di te… prometto.\p"
	.string "E quando ci sarò riuscito,\n"
	.string "ti sfiderò di nuovo.$"

VerdanturfTown_WandasHouse_Text_StrongerSpeechShort:
	.string "Lino: Guardami, {PLAYER}, ti prego.\n"
	.string "Io… diventerò più forte di te.\p"
	.string "E quando ci sarò riuscito,\n"
	.string "ti sfiderò di nuovo.$"

VerdanturfTown_WandasHouse_Text_WallysNextDoor:
	.string "Zio: Oh! {PLAYER}{KUN}!\n"
	.string "Lino è nella stanza qui accanto.\p"
	.string "Ma senti, ragazzo, c'è una cosa\n"
	.string "che devo dirti.\p"
	.string "L'aria di Mentania sta facendo\n"
	.string "miracoli per la salute di Lino.\p"
	.string "Però forse non è solo l'aria.\n"
	.string "Forse sono i Pokémon a dare\l"
	.string "speranza a quel ragazzo.$"

VerdanturfTown_WandasHouse_Text_WallySlippedOff:
	.string "Lino se n'è andato…\n"
	.string "È sparito da solo, senza dire niente…$"

VerdanturfTown_WandasHouse_Text_WallyGoneThatFar:
	.string "Zio: Ma davvero?\n"
	.string "Lino è arrivato fin laggiù tutto\l"
	.string "da solo…\p"
	.string "Be', devo riconoscerlo: è proprio\n"
	.string "il figlio del mio fratellino!$"

VerdanturfTown_WandasHouse_Text_MeetWanda:
	.string "Wanda: E tu chi sei?\n"
	.string "Ah, ma certo, ho capito!\p"
	.string "Sei {PLAYER}, il ragazzo di cui\n"
	.string "mi parlava sempre Lino!\p"
	.string "Io sono sua cugina.\n"
	.string "Piacere di conoscerti!\p"
	.string "Da quando è qui, Lino è molto più\n"
	.string "vivace, e sta molto meglio.$"

VerdanturfTown_WandasHouse_Text_DontWorryAboutWally:
	.string "Wanda: Non preoccuparti per Lino.\n"
	.string "Se la caverà benissimo.\p"
	.string "Conosco il mio cuginetto. E poi\n"
	.string "non è solo: ha i suoi Pokémon.$"

VerdanturfTown_WandasHouse_Text_CanSeeGirlfriendEveryDay:
	.string "Grazie a te posso vedere la mia\n"
	.string "ragazza tutti i giorni.\l"
	.string "Se sono felice? Puoi scommetterci!$"

VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendDriven:
	.string "Il ragazzo di mia figlia è un tipo\n"
	.string "molto determinato e appassionato.\p"
	.string "Scava un tunnel senza sosta pur\n"
	.string "di poter vedere mia figlia.\p"
	.string "Lei è un po' in pensiero, così\n"
	.string "va spesso a trovarlo al tunnel.$"

VerdanturfTown_WandasHouse_Text_DaughtersBoyfriendWasDigging:
	.string "Incredibile! Il ragazzo di mia figlia\n"
	.string "scavava il tunnel a mani nude!\p"
	.string "Che forza, l'amore!$"

VerdanturfTown_WandasHouse_Text_IfAnythingHappenedToWally:
	.string "Se dovesse succedere qualcosa a Lino,\n"
	.string "non potrei più guardare in faccia\l"
	.string "i suoi genitori a Petalipoli…$"

VerdanturfTown_WandasHouse_Text_WallyWasInEverGrande:
	.string "Lino era a Iridopoli?\p"
	.string "I suoi genitori a Petalipoli\n"
	.string "resteranno a bocca aperta!$"

