RustboroCity_DevonCorp_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_DevonCorp_1F_OnTransition
	.byte 0

RustboroCity_DevonCorp_1F_OnTransition:
	call_if_unset FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_BlockStairs
	end

RustboroCity_DevonCorp_1F_EventScript_BlockStairs::
	setobjectxyperm LOCALID_DEVON_CORP_STAIR_GUARD, 14, 2
	setobjectmovementtype LOCALID_DEVON_CORP_STAIR_GUARD, MOVEMENT_TYPE_FACE_DOWN
	return

RustboroCity_DevonCorp_1F_EventScript_Employee::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_GoodsRecovered
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_RobberWasntBright
	msgbox RustboroCity_DevonCorp_1F_Text_ThoseShoesAreOurProduct, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_RobberWasntBright::
	msgbox RustboroCity_DevonCorp_1F_Text_RobberWasntVeryBright, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_GoodsRecovered::
	msgbox RustboroCity_DevonCorp_1F_Text_SoundsLikeStolenGoodsRecovered, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_StairGuard::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_AlwaysWelcome
	goto_if_set FLAG_RECOVERED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_GotRobbed
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_GotRobbed
	msgbox RustboroCity_DevonCorp_1F_Text_OnlyAuthorizedPeopleEnter, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_AlwaysWelcome::
	msgbox RustboroCity_DevonCorp_1F_Text_YoureAlwaysWelcomeHere, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_GotRobbed::
	msgbox RustboroCity_DevonCorp_1F_Text_HowCouldWeGetRobbed, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_Greeter::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_WelcomeToDevonCorp
	goto_if_set FLAG_RECOVERED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed
	msgbox RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_WelcomeToDevonCorp::
	msgbox RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed::
	msgbox RustboroCity_DevonCorp_1F_Text_StaffGotRobbed, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_RocksMetalDisplay::
	msgbox RustboroCity_DevonCorp_1F_Text_RocksMetalDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_1F_EventScript_ProductsDisplay::
	msgbox RustboroCity_DevonCorp_1F_Text_ProductDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp:
	.string "Buongiorno, e benvenuto alla\n"
	.string "Devon S.p.A.!\p"
	.string "Siamo orgogliosi di produrre strumenti\n"
	.string "e medicine che migliorano la vita.$"

RustboroCity_DevonCorp_1F_Text_StaffGotRobbed:
	.string "Un nostro ricercatore, da vero genio,\n"
	.string "si è fatto rubare un pacco importante.$"

RustboroCity_DevonCorp_1F_Text_ThoseShoesAreOurProduct:
	.string "Ehi, quelle sono Scarpe da Corsa!\n"
	.string "Sono un prodotto della Devon!\p"
	.string "Che soddisfazione vedere qualcuno\n"
	.string "usare una cosa fatta da noi!$"

RustboroCity_DevonCorp_1F_Text_RobberWasntVeryBright:
	.string "Quel pacco rubato…\p"
	.string "Sì, è importante, ma quei pezzi da\n"
	.string "soli non servono proprio a niente.\p"
	.string "Secondo me, quel ladro non era\n"
	.string "esattamente un genio.$"

RustboroCity_DevonCorp_1F_Text_SoundsLikeStolenGoodsRecovered:
	.string "Pare che abbiano recuperato la\n"
	.string "Merce Devon rubata. Che sollievo!$"

RustboroCity_DevonCorp_1F_Text_OnlyAuthorizedPeopleEnter:
	.string "Mi dispiace, qui può entrare solo\n"
	.string "il personale autorizzato.$"

RustboroCity_DevonCorp_1F_Text_HowCouldWeGetRobbed:
	.string "Assurdo. Come abbiamo fatto a farci\n"
	.string "derubare in pieno giorno?$"

RustboroCity_DevonCorp_1F_Text_YoureAlwaysWelcomeHere:
	.string "Ehilà, ciao!\n"
	.string "Qui sei sempre il benvenuto!$"

RustboroCity_DevonCorp_1F_Text_RocksMetalDisplay:
	.string "Nella teca sono esposti campioni\n"
	.string "di rocce e di metalli.\p"
	.string "C'è un pannello con una scritta…\p"
	.string "“La Devon S.p.A. nacque come\n"
	.string "produttrice di pietre di cava.\p"
	.string "“L'azienda ricavava anche il ferro\n"
	.string "dalla limatura presente nella sabbia.\p"
	.string "“Da questi umili inizi come fornitrice\n"
	.string "di materie prime, la Devon è cresciuta.\p"
	.string "“Oggi la Devon produce un'ampia\n"
	.string "gamma di prodotti industriali.”$"

RustboroCity_DevonCorp_1F_Text_ProductDisplay:
	.string "Prototipi e prodotti in fase di test\n"
	.string "riempiono la teca.\p"
	.string "C'è un pannello con una descrizione…\p"
	.string "“Oltre ai prodotti industriali, oggi\n"
	.string "la Devon vende articoli per la casa\l"
	.string "e farmaci per vivere meglio.\p"
	.string "“Di recente, la Devon ha lanciato\n"
	.string "strumenti per Allenatori di Pokémon,\l"
	.string "come le Poké Ball e il PokéNav.\p"
	.string "“E il futuro? Stiamo già lavorando\n"
	.string "a strumenti per studiare i Varchi!”$"

