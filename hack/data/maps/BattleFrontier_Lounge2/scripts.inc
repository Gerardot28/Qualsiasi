BattleFrontier_Lounge2_MapScripts::
	.byte 0

@ This NPC gives hints about a random facility or battle mode.
@ For battle modes he gives generic advice
@ For facilities, depending on how far the player has progressed he will say either
@    The name of the Frontier Brain there
@    The type and description of the 3 pokemon they use in their silver battle
@    The type and description of the 3 pokemon they use in their gold battle
BattleFrontier_Lounge2_EventScript_FrontierManiac::
	lock
	faceplayer
	goto_if_set FLAG_MET_BATTLE_FRONTIER_MANIAC, BattleFrontier_Lounge2_EventScript_AlreadyMetManiac
	setflag FLAG_MET_BATTLE_FRONTIER_MANIAC
	msgbox BattleFrontier_Lounge2_Text_FrontierManiacIntro, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge2_EventScript_GiveAdvice
	end

BattleFrontier_Lounge2_EventScript_AlreadyMetManiac::
	msgbox BattleFrontier_Lounge2_Text_SwingByForTheLatestWord, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge2_EventScript_GiveAdvice
	end

BattleFrontier_Lounge2_EventScript_GiveAdvice::
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_SINGLES, BattleFrontier_Lounge2_EventScript_BufferSingle
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_DOUBLES, BattleFrontier_Lounge2_EventScript_BufferDouble
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_MULTIS, BattleFrontier_Lounge2_EventScript_BufferMulti
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_LINK, BattleFrontier_Lounge2_EventScript_BufferMultiLink
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_DOME, BattleFrontier_Lounge2_EventScript_BufferBattleDome
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_FACTORY, BattleFrontier_Lounge2_EventScript_BufferBattleFactory
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_PALACE, BattleFrontier_Lounge2_EventScript_BufferBattlePalace
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_ARENA, BattleFrontier_Lounge2_EventScript_BufferBattleArena
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_PIKE, BattleFrontier_Lounge2_EventScript_BufferBattlePike
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_PYRAMID, BattleFrontier_Lounge2_EventScript_BufferBattlePyramid
@ <= FRONTIER_MANIAC_TOWER_LINK is any Battle Tower mode
	call_if_le VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_LINK, BattleFrontier_Lounge2_EventScript_BattleTowerNews
@ >= FRONTIER_MANIAC_DOME is any facility other than Battle Tower
	call_if_ge VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_DOME, BattleFrontier_Lounge2_EventScript_FacilityNews
	special ShowFrontierManiacMessage
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_Lounge2_EventScript_BattleTowerNews::
	msgbox BattleFrontier_Lounge2_Text_BattleTowerIsHottest, MSGBOX_DEFAULT
	return

BattleFrontier_Lounge2_EventScript_FacilityNews::
	msgbox BattleFrontier_Lounge2_Text_FacilityIsHottest, MSGBOX_DEFAULT
	return

BattleFrontier_Lounge2_EventScript_BufferSingle::
	bufferstdstring STR_VAR_1, STDSTRING_SINGLE
	return

BattleFrontier_Lounge2_EventScript_BufferDouble::
	bufferstdstring STR_VAR_1, STDSTRING_DOUBLE
	return

BattleFrontier_Lounge2_EventScript_BufferMulti::
	bufferstdstring STR_VAR_1, STDSTRING_MULTI
	return

BattleFrontier_Lounge2_EventScript_BufferMultiLink::
	bufferstdstring STR_VAR_1, STDSTRING_MULTI_LINK
	return

BattleFrontier_Lounge2_EventScript_BufferBattleDome::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_DOME
	return

BattleFrontier_Lounge2_EventScript_BufferBattleFactory::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_FACTORY
	return

BattleFrontier_Lounge2_EventScript_BufferBattlePalace::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_PALACE
	return

BattleFrontier_Lounge2_EventScript_BufferBattleArena::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_ARENA
	return

BattleFrontier_Lounge2_EventScript_BufferBattlePike::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_PIKE
	return

BattleFrontier_Lounge2_EventScript_BufferBattlePyramid::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_PYRAMID
	return

BattleFrontier_Lounge2_EventScript_Maniac1::
	lock
	msgbox BattleFrontier_Lounge2_Text_NewsGatheringPower, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge2_EventScript_Maniac2::
	lock
	msgbox BattleFrontier_Lounge2_Text_AmazingPowersOfObservation, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge2_EventScript_Maniac3::
	lock
	msgbox BattleFrontier_Lounge2_Text_AmazingPowerOfPersuasion, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge2_EventScript_TriathleteF::
	msgbox BattleFrontier_Lounge2_Text_ThisPlaceIsScaringMe, MSGBOX_NPC
	end

BattleFrontier_Lounge2_Text_FrontierManiacIntro::
	.string "Ehilà! Se si parla di notizie sul\n"
	.string "Parco Lotta, il numero uno sono io!\p"
	.string "Puoi chiamarmi pure il Fanatico\n"
	.string "del Parco.\p"
	.string "Giusto per scrupolo: tu sei un\n"
	.string "Allenatore, vero?\p"
	.string "Lo sai che Scott ha costruito questo\n"
	.string "posto per gli Allenatori che un giorno\l"
	.string "arriveranno dal Grande Varco?\p"
	.string "Roba da brividi! Ti racconto volentieri\n"
	.string "le ultime novità sul Parco Lotta.$"

BattleFrontier_Lounge2_Text_SwingByForTheLatestWord::
	.string "Ehilà! Sei passato a spremermi per\n"
	.string "le ultime novità? E va bene, dai!$"

@ Unused
BattleFrontier_Lounge2_Text_MyInformationsBeenUsefulRight::
	.string "Allora? Allora? Allora?\p"
	.string "Le mie informazioni ti sono state\n"
	.string "utilissime, vero?$"

BattleFrontier_Lounge2_Text_FacilityIsHottest::
	.string "Vediamo un po'…\p"
	.string "Il posto più gettonato del momento?\n"
	.string "Senza dubbio: {STR_VAR_1}!$"

BattleFrontier_Lounge2_Text_BattleTowerIsHottest::
	.string "Vediamo un po'…\p"
	.string "Alla Torre Lotta va fortissimo\n"
	.string "la modalità {STR_VAR_1}!$"

BattleFrontier_Lounge2_Text_SalonMaidenIsThere::
	.string "Scommetto che questa non la sapevi!\p"
	.string "Lì c'è uno dei super Allenatori che\n"
	.string "Scott chiama gli Assi del Parco.\p"
	.string "È una misteriosa Allenatrice detta\n"
	.string "Dama Torre, ed è lei a comandare!$"

BattleFrontier_Lounge2_Text_SalonMaidenSilverMons::
	.string "Hai già sfidato la Dama Torre?\p"
	.string "Quando studia un avversario, pare\n"
	.string "che usi questi Pokémon:\p"
	.string "un Pokémon Psico (Psico),\n"
	.string "un Pokémon Vulcano (Fuoco)\l"
	.string "e un Pokémon Sonno (Normale).$"

BattleFrontier_Lounge2_Text_SalonMaidenGoldMons::
	.string "Hai già sfidato la Dama Torre quando\n"
	.string "fa sul serio?\p"
	.string "Quando lotta senza risparmiarsi,\n"
	.string "pare che usi questi Pokémon:\p"
	.string "un Pokémon Eone (Drago/Psico),\n"
	.string "un Pokémon Tuono (Elettro)\l"
	.string "e un Pokémon Sonno (Normale).$"

BattleFrontier_Lounge2_Text_DomeAceIsThere::
	.string "Scommetto che questa non la sapevi!\p"
	.string "Lì c'è uno dei super Allenatori che\n"
	.string "Scott chiama gli Assi del Parco.\p"
	.string "È uno sfavillante Allenatore detto\n"
	.string "Asso Cupola, ed è lui a comandare!$"

BattleFrontier_Lounge2_Text_DomeAceSilverMons::
	.string "Hai già sfidato l'Asso Cupola?\p"
	.string "Quando prende l'avversario alla\n"
	.string "leggera, usa questi tre Pokémon:\p"
	.string "un Pokémon Drago (Drago/Volante),\n"
	.string "un Pokémon Fango Pesce (Acqua/Terra)\l"
	.string "e un Pokémon Fiamma (Fuoco/Volante).$"

BattleFrontier_Lounge2_Text_DomeAceGoldMons::
	.string "Hai già sfidato l'Asso Cupola quando\n"
	.string "fa sul serio?\p"
	.string "Quando sfoggia la sua strategia,\n"
	.string "usa questi tre Pokémon:\p"
	.string "un Pokémon Eone (Drago/Psico),\n"
	.string "un Pokémon Fango Pesce (Acqua/Terra)\l"
	.string "e un Pokémon Ferrarto (Acciaio/Psico).$"

BattleFrontier_Lounge2_Text_FactoryHeadIsThere::
	.string "Scommetto che questa non la sapevi!\p"
	.string "Lì c'è uno dei super Allenatori che\n"
	.string "Scott chiama gli Assi del Parco.\p"
	.string "È uno stravagante Allenatore detto\n"
	.string "Capofabbrica, ed è lui a comandare!$"

BattleFrontier_Lounge2_Text_FactoryHeadSilverMons::
	.string "Hai già sfidato il Capofabbrica?\p"
	.string "Vediamo… Quando scende in campo,\n"
	.string "prende solo tre Pokémon a noleggio.\p"
	.string "Lotta più o meno nelle tue stesse\n"
	.string "condizioni.$"

BattleFrontier_Lounge2_Text_FactoryHeadGoldMons::
	.string "Hai già sfidato il Capofabbrica quando\n"
	.string "fa sul serio?\p"
	.string "Anche quando lotta sul serio, prende\n"
	.string "comunque tre Pokémon a noleggio.\p"
	.string "Lotta praticamente nelle tue stesse\n"
	.string "condizioni.$"

BattleFrontier_Lounge2_Text_PikeQueenIsThere::
	.string "Scommetto che questa non la sapevi!\p"
	.string "Lì c'è uno dei super Allenatori che\n"
	.string "Scott chiama gli Assi del Parco.\p"
	.string "È un'inquietante Allenatrice detta\n"
	.string "Regina Serpe, ed è lei a comandare!$"

BattleFrontier_Lounge2_Text_PikeQueenSilverMons::
	.string "Hai già sfidato la Regina Serpe?\p"
	.string "Quando è di buon umore, dicono che\n"
	.string "usi questi tre Pokémon:\p"
	.string "un Pokémon Zannaserpe (Veleno),\n"
	.string "un Pokémon Muffa (Coleottero/Roccia)\l"
	.string "e un Pokémon Tenerezza (Acqua).$"

BattleFrontier_Lounge2_Text_PikeQueenGoldMons::
	.string "Hai già sfidato la Regina Serpe quando\n"
	.string "fa sul serio?\p"
	.string "Quando è proprio furiosa, dicono che\n"
	.string "usi questi tre Pokémon:\p"
	.string "un Pokémon Zannaserpe (Veleno),\n"
	.string "un Pokémon Ferroserpe (Acciaio/Terra)\l"
	.string "e un Pokémon Atroce (Acqua/Volante).$"

BattleFrontier_Lounge2_Text_ArenaTycoonIsThere::
	.string "Scommetto che questa non la sapevi!\p"
	.string "Lì c'è uno dei super Allenatori che\n"
	.string "Scott chiama gli Assi del Parco.\p"
	.string "È un'adorabile Allenatrice detta\n"
	.string "Boss Dojo, ed è lei a comandare!$"

BattleFrontier_Lounge2_Text_ArenaTycoonSilverMons::
	.string "Hai già sfidato la Boss Dojo?\p"
	.string "Quando valuta le doti dell'avversario,\n"
	.string "si dice che usi questi Pokémon:\p"
	.string "un Pokémon Monocorno\n"
	.string "(Coleottero/Lotta), un Pokémon\l"
	.string "Lucelunare (Buio) e un Pokémon\l"
	.string "Cambiapelle (Coleottero/Spettro).$"

BattleFrontier_Lounge2_Text_ArenaTycoonGoldMons::
	.string "Hai già sfidato la Boss Dojo quando\n"
	.string "fa sul serio?\p"
	.string "Quando lotta per vincere davvero,\n"
	.string "si dice che usi questi Pokémon:\p"
	.string "un Pokémon Lucelunare (Buio),\n"
	.string "un Pokémon Ombra (Spettro/Veleno)\l"
	.string "e un Pokémon Fungo (Erba/Lotta).$"

BattleFrontier_Lounge2_Text_PalaceMavenIsThere::
	.string "Scommetto che questa non la sapevi!\p"
	.string "Lì c'è uno dei super Allenatori che\n"
	.string "Scott chiama gli Assi del Parco.\p"
	.string "È un Allenatore dall'aria sinistra detto\n"
	.string "Guru Palazzo, ed è lui a comandare!$"

BattleFrontier_Lounge2_Text_PalaceMavenSilverMons::
	.string "Hai già sfidato il Guru Palazzo?\p"
	.string "Quando mette alla prova lo spirito\n"
	.string "altrui, si dice che usi questi Pokémon:\p"
	.string "un Pokémon Pipistrello (Veleno/Volante),\n"
	.string "un Pokémon Pigrizia (Normale)\l"
	.string "e un Pokémon Trasporto (Acqua/Ghiaccio).$"

BattleFrontier_Lounge2_Text_PalaceMavenGoldMons::
	.string "Hai già sfidato il Guru Palazzo quando\n"
	.string "fa sul serio?\p"
	.string "Quando lotta con tutte le sue forze,\n"
	.string "si dice che usi questi Pokémon:\p"
	.string "un Pokémon Leggenda (Fuoco),\n"
	.string "un Pokémon Pigrizia (Normale)\l"
	.string "e un Pokémon Aurora (Acqua).$"

BattleFrontier_Lounge2_Text_PyramidKingIsThere::
	.string "Scommetto che questa non la sapevi!\p"
	.string "Lì c'è uno dei super Allenatori che\n"
	.string "Scott chiama gli Assi del Parco.\p"
	.string "È un focosissimo Allenatore detto\n"
	.string "Re Piramide, ed è lui a comandare!$"

BattleFrontier_Lounge2_Text_PyramidKingSilverMons::
	.string "Hai già sfidato il Re Piramide?\p"
	.string "Quando saggia la forza dell'avversario,\n"
	.string "pare che usi questi Pokémon:\p"
	.string "un Pokémon Picco (Roccia),\n"
	.string "un Pokémon Iceberg (Ghiaccio)\l"
	.string "e un Pokémon Ferro (Acciaio).$"

BattleFrontier_Lounge2_Text_PyramidKingGoldMons::
	.string "Hai già sfidato il Re Piramide quando\n"
	.string "fa sul serio?\p"
	.string "Quando è carico di energia bollente,\n"
	.string "pare che usi questi Pokémon:\p"
	.string "un Pokémon Gelo (Ghiaccio/Volante),\n"
	.string "un Pokémon Elettrico (Elettro/Volante)\l"
	.string "e un Pokémon Fiamma (Fuoco/Volante).$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice1::
	.string "Certo, ci sono vari posti dove fare\n"
	.string "Lotte in Doppio.\p"
	.string "Ma le sale per Lotte in Doppio della\n"
	.string "Torre Lotta sono, tipo, la base!\p"
	.string "È lì che devi imparare come\n"
	.string "funzionano le Lotte in Doppio\l"
	.string "qui al Parco Lotta.$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice2::
	.string "Occhio alle lotte, qui dentro.\p"
	.string "Ho sentito che certi Allenatori hanno\n"
	.string "strategie studiate apposta per le\l"
	.string "Lotte in Doppio.$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice3::
	.string "Quando ti sentirai sicuro e a tuo agio\n"
	.string "con le Lotte in Doppio qui, potrai\l"
	.string "pensare di sfidare anche gli altri\l"
	.string "posti dove si fanno Lotte in Doppio.$"

BattleFrontier_Lounge2_Text_MultiBattleAdvice::
	.string "Nel Salone Lotta si radunano\n"
	.string "Allenatori di ogni genere.\p"
	.string "Pensa: potresti trovarci i tuoi amici\n"
	.string "o perfino i tuoi fan!\l"
	.string "Guardati bene intorno!$"

BattleFrontier_Lounge2_Text_LinkMultiBattleAdvice::
	.string "Se sei con un amico, vai nella sala\n"
	.string "delle Lotte Multiple in Link.\p"
	.string "Se fai squadra con un amico forte,\n"
	.string "preparati ad avversari tostissimi!$"

BattleFrontier_Lounge2_Text_NewsGatheringPower::
	.string "Che fiuto per le notizie!\n"
	.string "Il mio maestro non ha rivali!$"

BattleFrontier_Lounge2_Text_AmazingPowersOfObservation::
	.string "Che spirito di osservazione!\n"
	.string "Il mio maestro non ha rivali!$"

BattleFrontier_Lounge2_Text_AmazingPowerOfPersuasion::
	.string "Che capacità di persuasione!\n"
	.string "Il mio maestro non ha rivali!$"

BattleFrontier_Lounge2_Text_ThisPlaceIsScaringMe::
	.string "…Ma che posto è questo?\n"
	.string "Mi mette i brividi…$"

