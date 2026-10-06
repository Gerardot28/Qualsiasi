LilycoveCity_CoveLilyMotel_1F_MapScripts::
	.byte 0

LilycoveCity_CoveLilyMotel_1F_EventScript_MotelOwner::
	lockall
	goto_if_set FLAG_SYS_GAME_CLEAR, LilycoveCity_CoveLilyMotel_1F_EventScript_GameClear
	goto_if_set FLAG_BADGE07_GET, LilycoveCity_CoveLilyMotel_1F_EventScript_AquaHideoutBusted
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_GuestsDoubledByMascot, MSGBOX_DEFAULT
	applymovement LOCALID_MOTEL_OWNER, Common_Movement_FacePlayer
	waitmovement 0
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_NoGuestsWithTeamAqua, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MOTEL_OWNER, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

LilycoveCity_CoveLilyMotel_1F_EventScript_AquaHideoutBusted::
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_MonFoundLostItem, MSGBOX_DEFAULT
	applymovement LOCALID_MOTEL_OWNER, Common_Movement_FacePlayer
	waitmovement 0
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_HeardAquaHideoutBusted, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MOTEL_OWNER, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

LilycoveCity_CoveLilyMotel_1F_EventScript_GameClear::
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_HouseSittingMonCaughtBurglar, MSGBOX_DEFAULT
	applymovement LOCALID_MOTEL_OWNER, Common_Movement_FacePlayer
	waitmovement 0
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_BetterGetWorkingOnGuestsDinner, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MOTEL_OWNER, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

LilycoveCity_CoveLilyMotel_1F_EventScript_BlockingTV::
	lockall
	playse SE_PIN
	applymovement LOCALID_MOTEL_OWNER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MOTEL_OWNER, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox LilycoveCity_CoveLilyMotel_1F_Text_CantSeeTheTV, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, LilycoveCity_CoveLilyMotel_1F_Movement_PlayerPushFromTV
	applymovement LOCALID_MOTEL_OWNER, LilycoveCity_CoveLilyMotel_1F_Movement_OwnerPushPlayer
	waitmovement 0
	applymovement LOCALID_MOTEL_OWNER, LilycoveCity_CoveLilyMotel_1F_Movement_OwnerReturn
	waitmovement 0
	release
	end

LilycoveCity_CoveLilyMotel_1F_Movement_PlayerPushFromTV:
	face_right
	lock_facing_direction
	walk_left
	unlock_facing_direction
	step_end

LilycoveCity_CoveLilyMotel_1F_Movement_OwnerPushPlayer:
	walk_up
	step_end

LilycoveCity_CoveLilyMotel_1F_Movement_OwnerReturn:
	face_down
	walk_down
	face_up
	step_end

LilycoveCity_CoveLilyMotel_1F_Text_GuestsDoubledByMascot:
	.string "Mmh, hanno raddoppiato gli ospiti\n"
	.string "usando i Pokémon come attrazione?\p"
	.string "Mmh… E se anch'io facessi di un\n"
	.string "Pokémon carino la mascotte dell'hotel?\p"
	.string "Chissà se così arriverebbero\n"
	.string "più clienti…$"

LilycoveCity_CoveLilyMotel_1F_Text_NoGuestsWithTeamAqua:
	.string "Oh, scusa, scusa!\n"
	.string "Ero tutto preso dalla TV!\p"
	.string "Da quando è arrivato quel Team Omega,\n"
	.string "i turisti girano al largo.\p"
	.string "Pirati in porto e Varchi in giro…\n"
	.string "Chi ci viene più in vacanza, qui?$"

LilycoveCity_CoveLilyMotel_1F_Text_CantSeeTheTV:
	.string "Ehi, spostati!\n"
	.string "Non vedo la TV!$"

LilycoveCity_CoveLilyMotel_1F_Text_MonFoundLostItem:
	.string "Incredibile! Un Pokémon ha ritrovato\n"
	.string "un oggetto smarrito?\p"
	.string "Mica male. Se avessimo anche noi\n"
	.string "un Pokémon così sveglio…\p"
	.string "Potremmo ritrovare tutto quello che\n"
	.string "i nostri ospiti dimenticano…$"

LilycoveCity_CoveLilyMotel_1F_Text_HeardAquaHideoutBusted:
	.string "Oh, scusa, scusa!\n"
	.string "Ero tutto preso dalla TV!\p"
	.string "Ho sentito che qualcuno ha smantellato\n"
	.string "il Covo Omega.\p"
	.string "E così abbiamo appena ricevuto la\n"
	.string "prenotazione di un grosso gruppo.\p"
	.string "Era una ditta che si chiama… Ehm…\n"
	.string "Game qualcosa…$"

LilycoveCity_CoveLilyMotel_1F_Text_HouseSittingMonCaughtBurglar:
	.string "Incredibile! Un Pokémon lasciato di\n"
	.string "guardia alla casa ha preso un ladro?\p"
	.string "Mica male. Se avessimo un Pokémon\n"
	.string "forzuto come guardia giurata…\p"
	.string "Potremmo garantire ai nostri ospiti\n"
	.string "molta più sicurezza.$"

LilycoveCity_CoveLilyMotel_1F_Text_BetterGetWorkingOnGuestsDinner:
	.string "Oh, scusa, scusa!\n"
	.string "Ero tutto preso dalla TV.\p"
	.string "Ah, sì. Poco fa è arrivato un grosso\n"
	.string "gruppo di ospiti.\p"
	.string "Si sono registrati come Game Freak,\n"
	.string "quindi credo che facciano videogiochi.\p"
	.string "Oh, no! Devo mettermi subito a\n"
	.string "preparare la loro cena!$"

