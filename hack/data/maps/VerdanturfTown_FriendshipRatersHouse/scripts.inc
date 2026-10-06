VerdanturfTown_FriendshipRatersHouse_MapScripts::
	.byte 0

VerdanturfTown_FriendshipRatersHouse_EventScript_FriendshipRater::
	lock
	faceplayer
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_SeeHowMuchPokemonLikesYou, MSGBOX_DEFAULT
	specialvar VAR_RESULT, GetLeadMonFriendshipScore
	switch VAR_RESULT
	case FRIENDSHIP_NONE, VerdanturfTown_FriendshipRatersHouse_EventScript_DetestsYou
	case FRIENDSHIP_1_TO_49, VerdanturfTown_FriendshipRatersHouse_EventScript_VeryWary
	case FRIENDSHIP_50_TO_99, VerdanturfTown_FriendshipRatersHouse_EventScript_NotUsedToYou
	case FRIENDSHIP_100_TO_149, VerdanturfTown_FriendshipRatersHouse_EventScript_GettingUsedToYou
	case FRIENDSHIP_150_TO_199, VerdanturfTown_FriendshipRatersHouse_EventScript_LikesYouQuiteALot
	case FRIENDSHIP_200_TO_254, VerdanturfTown_FriendshipRatersHouse_EventScript_VeryHappy
	case FRIENDSHIP_MAX, VerdanturfTown_FriendshipRatersHouse_EventScript_AdoresYou
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_DetestsYou::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_DetestsYou, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_VeryWary::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_VeryWary, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_NotUsedToYou::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_NotUsedToYou, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_GettingUsedToYou::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_GettingUsedToYou, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_LikesYouQuiteALot::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_LikesYouQuiteALot, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_VeryHappy::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_VeryHappy, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_AdoresYou::
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_AdoresYou, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_FriendshipRatersHouse_EventScript_Pikachu::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	msgbox VerdanturfTown_FriendshipRatersHouse_Text_Pikachu, MSGBOX_DEFAULT
	waitmoncry
	release
	end

VerdanturfTown_FriendshipRatersHouse_Text_SeeHowMuchPokemonLikesYou:
	.string "Fammi vedere il tuo Pokémon.\n"
	.string "Ti dirò quanto ti vuole bene.\p"
	.string "Oh.\n"
	.string "Il tuo Pokémon…$"

VerdanturfTown_FriendshipRatersHouse_Text_AdoresYou:
	.string "Ti adora!\n"
	.string "Non potrebbe volerti più bene di così.\l"
	.string "Mi scalda il cuore solo a guardarlo.$"

VerdanturfTown_FriendshipRatersHouse_Text_VeryHappy:
	.string "Sembra davvero felicissimo.\n"
	.string "Si vede che ti vuole un mondo di bene.$"

VerdanturfTown_FriendshipRatersHouse_Text_LikesYouQuiteALot:
	.string "Ti vuole parecchio bene.\n"
	.string "Direi che gradirebbe qualche coccola.$"

VerdanturfTown_FriendshipRatersHouse_Text_GettingUsedToYou:
	.string "Si sta abituando a te.\n"
	.string "Sembra che cominci a fidarsi.$"

VerdanturfTown_FriendshipRatersHouse_Text_NotUsedToYou:
	.string "Non si è ancora abituato a te.\n"
	.string "Non ti ama, ma nemmeno ti odia.$"

VerdanturfTown_FriendshipRatersHouse_Text_VeryWary:
	.string "È molto diffidente.\n"
	.string "Ha uno sguardo davvero minaccioso.\l"
	.string "Direi che non gli piaci granché.$"

VerdanturfTown_FriendshipRatersHouse_Text_DetestsYou:
	.string "È un po' difficile da dire…\p"
	.string "Il tuo Pokémon non ti sopporta proprio.\n"
	.string "Non ti mette un po' a disagio?$"

VerdanturfTown_FriendshipRatersHouse_Text_Pikachu:
	.string "Pikachu: Pika pika!$"

