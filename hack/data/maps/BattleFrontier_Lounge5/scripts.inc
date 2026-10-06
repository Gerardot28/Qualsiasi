BattleFrontier_Lounge5_MapScripts::
	.byte 0

BattleFrontier_Lounge5_EventScript_NatureGirl::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge5_Text_NatureGirlGreeting, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown
	chooseboxmon
	waitstate
	lock
	faceplayer
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, BattleFrontier_Lounge5_EventScript_NatureGirlEgg
	special ShowNatureGirlMessage
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_Lounge5_EventScript_NatureGirlEgg::
	msgbox BattleFrontier_Lounge5_Text_NatureGirlEgg, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown::
	msgbox BattleFrontier_Lounge5_Text_NatureGirlNoneShown, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge5_EventScript_Gentleman::
	msgbox BattleFrontier_Lounge5_Text_LadyClaimsSheUnderstandsPokemon, MSGBOX_NPC
	end

BattleFrontier_Lounge5_EventScript_BlackBelt::
	msgbox BattleFrontier_Lounge5_Text_GirlSayingSomethingProfound, MSGBOX_NPC
	end

BattleFrontier_Lounge5_EventScript_LittleBoy::
	msgbox BattleFrontier_Lounge5_Text_GirlPlaysAtRedHouseALot, MSGBOX_NPC
	end

BattleFrontier_Lounge5_Text_NatureGirlGreeting::
	.string "Eheheh!\n"
	.string "Io so cosa pensano i Pokémon!\p"
	.string "Dai, ti prego!\n"
	.string "Mi fai vedere un tuo Pokémon?$"

BattleFrontier_Lounge5_Text_NatureGirlNoneShown::
	.string "Uffa!\n"
	.string "Che tirchio!$"

BattleFrontier_Lounge5_Text_NatureGirlAttackHighAttackLow::
	.string "Mmmh…\p"
	.string "Questo dice che adora lottare!\n"
	.string "Continua a lottare anche quando\l"
	.string "si è fatto tanta bua!$"

BattleFrontier_Lounge5_Text_NatureGirlAttackHighDefenseLow::
	.string "Mmmh…\p"
	.string "Questo dice che adora lottare!\n"
	.string "Ma se si fa troppa bua, comincia\l"
	.string "a preoccuparsi per sé stesso!$"

BattleFrontier_Lounge5_Text_NatureGirlAttackHighSupportLow::
	.string "Mmmh…\p"
	.string "Questo dice che adora lottare!\n"
	.string "Ma se si fa troppa bua,\l"
	.string "diventa un furbacchione!$"

BattleFrontier_Lounge5_Text_NatureGirlDefenseHighAttackLow::
	.string "Mmmh…\p"
	.string "Questo dice che pensa a proteggersi!\n"
	.string "Ma se si fa troppa bua,\l"
	.string "passa al contrattacco!$"

BattleFrontier_Lounge5_Text_NatureGirlDefenseHighDefenseLow::
	.string "Mmmh…\p"
	.string "Questo dice che pensa a proteggersi!\n"
	.string "E si preoccupa per sé stesso, che si\l"
	.string "sia fatto tanta bua oppure no!$"

BattleFrontier_Lounge5_Text_NatureGirlDefenseHighSupportLow::
	.string "Mmmh…\p"
	.string "Questo dice che pensa a proteggersi!\n"
	.string "Ma se si fa troppa bua,\l"
	.string "diventa un furbacchione!$"

BattleFrontier_Lounge5_Text_NatureGirlSupportHighAttackLow::
	.string "Mmmh…\p"
	.string "Questo dice che adora fare il furbo!\n"
	.string "Ma se si fa troppa bua,\l"
	.string "passa al contrattacco!$"

BattleFrontier_Lounge5_Text_NatureGirlSupportHighDefenseLow::
	.string "Mmmh…\p"
	.string "Questo dice che adora fare il furbo!\n"
	.string "Ma se si fa troppa bua, comincia\l"
	.string "a preoccuparsi per sé stesso!$"

BattleFrontier_Lounge5_Text_NatureGirlSupportHighSupportLow::
	.string "Mmmh…\p"
	.string "Questo dice che adora fare il furbo!\n"
	.string "E fa il furbetto anche quando\l"
	.string "si è fatto tanta bua!$"

BattleFrontier_Lounge5_Text_NatureGirlEgg::
	.string "Ma che sciocco! L'Uovo sta dormendo!\n"
	.string "Non posso mica parlarci!$"

BattleFrontier_Lounge5_Text_LadyClaimsSheUnderstandsPokemon::
	.string "Che amore!\n"
	.string "Quella signorinella sostiene di capire\l"
	.string "cosa pensano i Pokémon!$"

BattleFrontier_Lounge5_Text_GirlSayingSomethingProfound::
	.string "Ho la sensazione che quella bambina\n"
	.string "dica cose molto profonde.\p"
	.string "Chissà se capisce anche i Pokémon\n"
	.string "arrivati dai Varchi…$"

BattleFrontier_Lounge5_Text_GirlPlaysAtRedHouseALot::
	.string "Io so una cosa!\p"
	.string "Quella bambina va sempre a giocare\n"
	.string "nella casa rossa!$"

