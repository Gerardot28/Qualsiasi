# Segment D — Story skeleton: Endgame, post-game and optional systems (Hoenn)

Source: pokeemerald-expansion 1.17.1, read-only copy at `/home/user/pex-orig`.
Every path below is relative to that root. Map scripts are in `data/maps/<MAP>/scripts.inc`. Object, warp and trigger placement is in `data/maps/<MAP>/map.json`.

This document is the **mechanical skeleton** that story writers must follow for the last part of the Hoenn game and everything that becomes available after it. Writers may change every line of text and every motivation. They may **not** change who appears where, who walks where, which battles happen, which items or Pokémon are given, or which flags and vars gate progress. The "HC" lines list what a re-interpretation must still explain on screen.

Conventions are the same as in Segment A (§0 of `segmento_A.md`): gender branch (male player → rival **MAY**, female player → rival **BRENDAN**; both branches must be written), `{PLAYER}`, `{RIVAL}`, `{KUN}`, `{STR_VAR_1}`; **HC** = hard constraint; **[M]** mandatory, **[O]** optional, **[S]** system / repeatable.

---

## 0. Entry state (what is already true when Segment D starts)

Segment C ends with the Sootopolis crisis resolved by the sky Pokémon (Rayquaza). At the start of Segment D:

* 7 badges are owned (`FLAG_BADGE01_GET` … `FLAG_BADGE07_GET`).
* `VAR_SOOTOPOLIS_CITY_STATE == 5` (Rayquaza has calmed the two giants; normal weather is back).
* `VAR_SKY_PILLAR_STATE` is 2 or 3 (Rayquaza is back on top of the Sky Pillar and can be fought, see D03).
* In Sootopolis, **Maxie** and **Archie** stand south of the Gym (33,35)/(34,35), **Steven** at (29,33), **Wallace** at (31,33), directly below the **locked Gym door** (31,32). The Gym door stays closed (`SootopolisCity_EventScript_LockGymDoor`) while `FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE` is unset.
* The villain leaders' last lines in Sootopolis (`SootopolisCity_Text_AfterAllOurScheming`, `SootopolisCity_Text_TryingMeaninglessToPokemon`) are the hand-off from Segment C. They are listed again in D01 because they gate the start of this segment.

---

## 1. Critical path at a glance (verified order)

"→" = forced order (a var or flag gates the next step). "∥" = free order.

1. Sootopolis: talk to **both** villain leaders → they leave (`FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE`) → **Wallace gives HM Waterfall** and steps aside from the Gym door.
2. **Sootopolis Gym**: ice-floor puzzle → **Juan → Rain Badge (badge 8)**, TM Water Pulse, Juan registered in the PokéNav. `VAR_SOOTOPOLIS_CITY_STATE = 6`.
3. Surf east (Route 126/127/128) → **Ever Grande City** (Waterfall) → Pokémon Center (optional Scott farewell).
4. **Victory Road 1F**: Wally ambush battle (coordinate trigger) → cross Victory Road (1F → B1F → B2F → 1F north exit).
5. **Pokémon League 1F**: the two door guards check badges (`FLAG_ENTERED_ELITE_FOUR`).
6. **Sidney → Phoebe → Glacia → Drake** (`VAR_ELITE_4_STATE` 0→4). A loss resets the whole run (`EventScript_WhiteOut`).
7. **Champion Wallace** → rival bursts in → Prof. Birch rates the Pokédex → Wallace leads the player into the **Hall of Fame**.
8. Hall of Fame record → `EverGrandeCity_HallOfFame_EventScript_SetGameClearFlags` → `special GameClear` (`FLAG_SYS_GAME_CLEAR`) → Hall of Fame screen → **credits** → title screen.
9. Continue: the player wakes up in the bedroom (2F) → going downstairs triggers **Dad gives the S.S. Ticket** + **emergency TV news about a red/blue flying Pokémon** (roamer chosen by the player's answer).
10. Stepping outside triggers **Prof. Birch + rival** → lab → **National Pokédex**. This arms Scott's call.
11. After 10 steps on any outdoor map: **Scott's PokéNav call** (invitation, ferry at Slateport or Lilycove).
12. Post-game, all optional and parallel (∥): S.S. Tidal voyage (Scott on board → Battle Frontier unlocked) → Battle Frontier; Steven's letter + Beldum (Mossdeep); Steven battle (Meteor Falls, cave opened by `FLAG_SYS_GAME_CLEAR`); Wally rematch (Victory Road); Johto starter (complete Hoenn Dex); Diploma (Lilycove Motel); roaming Lati@s; Weather Institute → Terra Cave (Groudon) / Marine Cave (Kyogre); Desert Underpass fossil; Mirage Island; Trainer Hill; Safari Zone expansion; Gym Leader rematches; ticket islands (Southern, Birth, Faraway, Navel Rock: these need event items that the vanilla game never hands out).

Optional detours already open at the start of the segment (∥ with steps 2–8): Mt. Pyre summit, where the leaders return the orbs (D02), and the Sky Pillar Rayquaza (D03).

---

## 2. State variables and flags (reference)

| Var / flag | Values in this segment | Set by | Meaning |
|---|---|---|---|
| `FLAG_MET_MAXIE_SOOTOPOLIS` / `FLAG_MET_ARCHIE_SOOTOPOLIS` | set | Sootopolis (talk to each leader at state 5) | Both must be set before the leaders leave |
| `FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE` | set | `SootopolisCity_EventScript_MaxieArchieLeave` | Leaders gone; Gym door and house doors unlocked; Wallace can give Waterfall |
| `VAR_MT_PYRE_STATE` | 2 leaders waiting at the summit → 3 orbs returned | Sootopolis / `MtPyre_Summit` | Optional orb-return scene |
| `FLAG_RETURNED_RED_OR_BLUE_ORB` | set | Mt. Pyre old lady | Old lady dialogue only |
| `FLAG_RECEIVED_HM_WATERFALL` | set | Wallace (Sootopolis) | HM07 Waterfall |
| `VAR_SOOTOPOLIS_WALLACE_STATE` | 0 blocks door → 1 stepped right / 2 stepped left | Wallace script | Wallace's position next to the Gym |
| `VAR_ICE_STEP_COUNT` | 1 on entry; 8/28/67 open the 1st/2nd/3rd stairs; 0 = fell through | `SootopolisCity_Gym_1F` + `STEP_CB_SOOTOPOLIS_ICE` | Gym ice puzzle |
| `FLAG_DEFEATED_SOOTOPOLIS_GYM`, `FLAG_BADGE08_GET` | set | Juan | Badge 8 |
| `FLAG_RECEIVED_TM_WATER_PULSE` | set | Juan | TM |
| `FLAG_ENABLE_JUAN_MATCH_CALL` | set | Juan | Juan in the PokéNav |
| `VAR_SOOTOPOLIS_CITY_STATE` | 5 → **6** | Juan's defeat script | City back to normal; Steven, Wallace and the spectators are hidden; an Expert blocks the Cave of Origin |
| `FLAG_MET_SCOTT_IN_EVERGRANDE` | set | Ever Grande PC Scott, or entering Sidney's room | Scott's farewell is missable |
| `VAR_SCOTT_STATE` | +1 per Scott meeting (13 possible) | Scott scenes in many maps | Battle Points Scott gives at the Frontier (1–4) |
| `VAR_VICTORY_ROAD_1F_STATE` | 0 → 1/2 (which trigger tile) | Victory Road 1F | Wally battle done; Wally's standing position |
| `FLAG_DEFEATED_WALLY_VICTORY_ROAD` | set | Victory Road 1F | Wally's family dialogue, Wally Match Call #7, allows Wally rematches |
| `FLAG_HIDE_VICTORY_ROAD_ENTRANCE_WALLY` / `FLAG_HIDE_VICTORY_ROAD_EXIT_WALLY` | entrance shown after the battle; at HoF: entrance hidden, exit shown | VR 1F, `hall_of_fame.inc` | Wally moves to the exit after the HoF |
| `FLAG_ENTERED_ELITE_FOUR` | set | League 1F guards | Guards step aside permanently |
| `VAR_ELITE_4_STATE` | 0 → 1 Sidney → 2 Phoebe → 3 Glacia → 4 Drake (entering each room) | E4 room `OnFrame` | Entry door closes behind the player |
| `FLAG_DEFEATED_ELITE_4_SIDNEY/PHOEBE/GLACIA/DRAKE` | set | each room | Exit door opens. All reset by whiteout and by the HoF |
| `FLAG_HIDE_PETALBURG_GYM_GREETER` | set | Champion's room | Petalburg Gym greeter removed |
| `FLAG_IS_CHAMPION`, `FLAG_SYS_GAME_CLEAR` | set | HoF script / `GameClear` (`src/post_battle_event_funcs.c`) | Game cleared; unlocks almost every post-game check |
| `VAR_LITTLEROOT_HOUSES_STATE_MAY` / `_BRENDAN` | 3 at HoF (if no S.S. Ticket yet) → 4 after the TV event | HoF / `players_house.inc` | Dad + S.S. Ticket + Lati TV scene |
| `FLAG_HIDE_PLAYERS_HOUSE_DAD` | cleared at HoF | HoF | Dad is at home |
| `FLAG_RECEIVED_SS_TICKET` | set | Dad | Key item `ITEM_SS_TICKET` |
| `FLAG_SYS_TV_LATIAS_LATIOS`, `FLAG_LATIOS_OR_LATIAS_ROAMING` | TV on/off; roaming set | `players_house.inc` | Roaming legendary |
| `VAR_ROAMER_POKEMON` | 0 = "RED" answer (Latias roams, Latios on Southern Island) / 1 = "BLUE" (Latios roams, Latias on the island) | `special InitRoamer` (`src/roamer.c`) | Which Eon Pokémon roams |
| `VAR_DEX_UPGRADE_JOHTO_STARTER_STATE` | 1 HoF → 2 National Dex → 3 left lab → 4 entered lab with the Hoenn Dex complete → 5 told to choose → 6 chose | HoF, Littleroot, lab | National Dex and Johto starter |
| `FLAG_SYS_NATIONAL_DEX` | set | lab | National Dex |
| `FLAG_SCOTT_CALL_BATTLE_FRONTIER`, `VAR_SCOTT_BF_CALL_STEP_COUNTER` | set / counts to 10 | lab; `ShouldDoScottBattleFrontierCall` (`src/field_specials.c`) | Scott's invitation call |
| `FLAG_HIDE_SLATEPORT_CITY_HARBOR_SS_TIDAL`, `FLAG_HIDE_LILYCOVE_HARBOR_SSTIDAL` | cleared at HoF | HoF | The ferry is docked |
| `FLAG_HIDE_SS_TIDAL_CORRIDOR_MR_BRINEY` (cleared), `FLAG_HIDE_SLATEPORT_CITY_STERNS_SHIPYARD_MR_BRINEY` (set) | HoF | HoF | Briney and Peeko travel on the ferry |
| `VAR_SS_TIDAL_STATE` | see §4 | harbors, ferry | Voyage progress |
| `VAR_SS_TIDAL_SCOTT_STATE`, `FLAG_MET_SCOTT_ON_SS_TIDAL` | 0 → 1 / set | S.S. Tidal corridor | Battle Frontier destination unlocked |
| `VAR_HAS_ENTERED_BATTLE_FRONTIER`, `FLAG_SYS_FRONTIER_PASS` | 1 / set | Frontier reception gate | Frontier Pass |
| `FLAG_SCOTT_GIVES_BATTLE_POINTS`, `FLAG_COLLECTED_ALL_SILVER_SYMBOLS`, `FLAG_COLLECTED_ALL_GOLD_SYMBOLS`, `FLAG_RECEIVED_SILVER_SHIELD`, `FLAG_RECEIVED_GOLD_SHIELD` | set | Scott's house | Scott's rewards |
| `FLAG_SYS_<FACILITY>_SILVER/GOLD` | set | Frontier Brains | Symbols |
| `VAR_STEVENS_HOUSE_STATE` | 2 at HoF | HoF | Steven's house layout |
| `FLAG_HIDE_MOSSDEEP_CITY_STEVENS_HOUSE_INVISIBLE_NINJA_BOY` (cleared), `FLAG_HIDE_MOSSDEEP_CITY_STEVENS_HOUSE_BELDUM_POKEBALL` (cleared if no Beldum) | HoF | HoF | Steven's letter + Beldum ball |
| `FLAG_RECEIVED_BELDUM` | set | Steven's house | Beldum |
| `FLAG_DEFEATED_METEOR_FALLS_STEVEN` | set | Meteor Falls | Steven battle done (his cave only opens with `FLAG_SYS_GAME_CLEAR`) |
| `VAR_FOSSIL_MANIAC_STATE` | 0 → 1 at HoF → 2 after his cave-in line | HoF, Fossil Maniac's tunnel | Desert Underpass hint |
| `VAR_ABNORMAL_WEATHER_LOCATION`, `VAR_ABNORMAL_WEATHER_STEP_COUNTER`, `VAR_SHOULD_END_ABNORMAL_WEATHER` | random location / steps / 1 | Weather Institute 2F, Terra/Marine Cave | Groudon/Kyogre hunt |
| `FLAG_DEFEATED_GROUDON` / `FLAG_DEFEATED_KYOGRE` / `FLAG_DEFEATED_RAYQUAZA` | set | caves / Sky Pillar | Legendary handled (caught or defeated) |
| `FLAG_DEFEATED_MEW/LATIAS_OR_LATIOS/DEOXYS/LUGIA/HO_OH` | cleared at every HoF | HoF | Event legendaries respawn after each HoF if they were defeated, not caught |
| `FLAG_ENABLE_SHIP_SOUTHERN_ISLAND/BIRTH_ISLAND/FARAWAY_ISLAND/NAVEL_ROCK` + key items | set only by Mystery Gift / record mixing | `data/scripts/gift_*.inc`, `cable_club.inc` | Ticket islands (dormant in vanilla) |
| `FLAG_SHOWN_EON_TICKET/AURORA_TICKET/OLD_SEA_MAP/MYSTIC_TICKET` | set | Lilycove harbor | First-voyage cutscene done |
| `VAR_LILYCOVE_FAN_CLUB_STATE` | 1 at HoF (`UpdateTrainerFanClubGameClear`) | `src/field_specials.c` | Trainer Fan Club's first fans |
| `FLAG_HIDE_SAFARI_ZONE_SOUTH_CONSTRUCTION_WORKERS` (set) / `FLAG_HIDE_SAFARI_ZONE_SOUTH_EAST_EXPANSION` (cleared) | HoF | HoF | Safari Zone expansion opens |
| `FLAG_HIDE_LILYCOVE_MOTEL_GAME_DESIGNERS` | cleared at HoF | HoF | Diploma NPCs appear |
| `FLAG_HIDE_LILYCOVE_CITY_RIVAL`, `FLAG_HIDE_LITTLEROOT_TOWN_*_RIVAL_BEDROOM` | set at HoF | HoF | The rival moves to the lab |

---

## 3. Beats

### D01 — Sootopolis after the crisis: the leaders leave, Wallace gives HM Waterfall [M]
* **Maps:** `SootopolisCity`.
* **Mechanics:** At `VAR_SOOTOPOLIS_CITY_STATE == 5`, Maxie and Archie stand south of the Gym. Talking to Maxie (`SootopolisCity_EventScript_MaxieRayquaza`) sets `FLAG_MET_MAXIE_SOOTOPOLIS`, and talking to Archie sets `FLAG_MET_ARCHIE_SOOTOPOLIS`. When both are set, `SootopolisCity_EventScript_MaxieArchieLeave` hides both leaders, sets `FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE`, shows Maxie and Archie at the Mt. Pyre summit (`VAR_MT_PYRE_STATE = 2`) and silently re-warps the player in place (31,34), so the Gym door and the house doors are now open. Until then, Wallace only says "they didn't mean harm, hear them out". Afterwards, talking to Wallace (`..._GiveWaterfall`) gives **`ITEM_HM_WATERFALL`** (`FLAG_RECEIVED_HM_WATERFALL`). He explains that the Rain Badge is needed to use it and points at the Gym door, then steps one tile right or left (`VAR_SOOTOPOLIS_WALLACE_STATE` 1/2). He then repeats "you'll be dazzled by my mentor's elegant style". Steven: "Both leaders have gone away… maybe to Mt. Pyre to return the orbs." The Cave of Origin Expert says "the awakened Pokémon clash" (state 5), then "the cave, too, shall sleep" (state ≥ 6).
* **Original gist:** Maxie: "After all our scheming… [the Pokémon] were never ours to control." Archie: "What we were trying to do was meaningless to Pokémon…" Wallace: "My eyes didn't deceive me. Thanks to your help, Sootopolis — no, all of Hoenn — was saved. This is a gift from me… That HM contains Waterfall… You need the Rain Badge… beat the Sootopolis Gym Leader. When you're set, step through that door."
* **Characters:** Wallace (`OBJ_EVENT_GFX_WALLACE`, future Champion), Steven (`OBJ_EVENT_GFX_STEVEN`), Maxie / Archie (villain-leader sprites), Cave of Origin Expert, residents/spectators.
* **Text labels:** `SootopolisCity_Text_AfterAllOurScheming`, `..._TryingMeaninglessToPokemon`, `..._AquaMagmaDidntMeanHarm`, `..._ThankYouForHelpAcceptThis`, `..._ExplainWaterfallGoToGym`, `..._DazzledByMentor`, `..._MaxieArchieLeft`, `..._HaventYouScaledSkyPillar`, `..._CaveOfOriginSleepsToo`, `..._AwakenedPokemonClash`, plus the resident lines with "Rayquaza" / "PostLegendaries" variants (Kiri, Woman1/2, NinjaBoy, Boy1/2, BlackBelt, Girl, Maniac, Man).
* **HC:** Two leaders (the leaders of the new villain organisation in our story) are standing in the city after the catastrophe was stopped. The player must speak to **both**, and only then do they vanish from the city. A powerful trainer stands **in front of the locked Gym door** and gives the player **HM Waterfall** as thanks for saving the region. He then steps aside, which opens the Gym.

### D02 — (Optional) Mt. Pyre summit: the leaders return the orbs [O]
* **Maps:** `MtPyre_Summit`.
* **Mechanics:** `VAR_MT_PYRE_STATE == 2` places Maxie and Archie at the summit (23,6)/(22,6). Three coordinate triggers (`MtPyre_Summit_EventScript_ArchieMaxieTrigger0/1/2`) start the scene. The player faces north. Maxie gets a "!", walks to the player and says only `{PLAYER}… … …`, then walks to Archie. Both walk out and are removed. `VAR_MT_PYRE_STATE = 3`. No battle, no item. The old lady then says the two men returned the orbs (`FLAG_RETURNED_RED_OR_BLUE_ORB`). After that she explains that "the land, sea and sky Pokémon embody a living world". The old man offers the **"new legend of Hoenn"** (land vs sea, calmed by the green sky Pokémon) once `FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE` is set.
* **Original gist:** Maxie: "{PLAYER}… … …" (silence). Old lady: "The two men who took the orbs came back to return them on their own… Perhaps they are not so evil after all." Old man: "the crisis in Sootopolis rewrote a legend… the Pokémon of the sky descended from a storm… becalmed the two… then flew off".
* **Characters:** Maxie, Archie, Mt. Pyre old couple (keepers of the orbs).
* **Text labels:** `MtPyre_Summit_Text_MaxieSilence`, `..._ThoseTwoMenReturnedOrbs`, `..._SuperAncientPokemonTaughtUs`, `..._HearTheNewLegendOfHoenn`, `..._HoennTrioTale`, `..._WellThatTooIsFine`.
* **HC:** The two former leaders appear together at the mountain shrine, one of them silently faces the player, and they leave together. The shrine keepers then report that the stolen artefacts were returned voluntarily. A storyteller tells the "new legend": two titans clash and a third one from the sky calms them.

### D03 — (Optional, open from now on) Sky Pillar: Rayquaza at the top [O]
* **Maps:** `SkyPillar_Outside`, `SkyPillar_1F`–`5F`, `SkyPillar_Top`.
* **Mechanics:** With `VAR_SKY_PILLAR_STATE >= 2`, the floors use their cracked layout (the Mach Bike is needed for the cracked tiles; `call_if_lt VAR_SKY_PILLAR_STATE, 2, …CleanFloor` keeps the clean layout before the crisis). At the top, `FLAG_HIDE_SKY_PILLAR_TOP_RAYQUAZA_STILL` is cleared unless `FLAG_DEFEATED_RAYQUAZA` is set. Talking to it plays the cry and starts `setwildbattle SPECIES_RAYQUAZA, 70` (legendary battle). Catch, defeat or flee all set `FLAG_DEFEATED_RAYQUAZA`. A defeat or flight fades the screen and removes it. No text except the cry and the generic "flew away" message.
* **HC:** The sky dragon that ended the crisis rests on top of the tower and can be challenged or caught once. It does **not** respawn.

### D04 — Sootopolis Gym: the ice floor [M]
* **Maps:** `SootopolisCity_Gym_1F`, `SootopolisCity_Gym_B1F`.
* **Mechanics:** On entry `VAR_ICE_STEP_COUNT = 1`. `STEP_CB_SOOTOPOLIS_ICE` counts the tiles cracked by walking on them. At 8, 28 and 67 the next flight of stairs turns from ice into stairs (`SE_ICE_STAIRS`). The three sections must be fully cracked in order. Stepping on an already-cracked tile sets the count to 0 → the player falls through (`warphole MAP_SOOTOPOLIS_CITY_GYM_B1F`). B1F holds the Gym trainers: **TRAINER_ANDREA, CRISSY, BRIANNA, CONNIE, BRIDGET, OLIVIA, TIFFANY, BETHANY, ANNIKA, DAPHNE** (all `trainerbattle_single`). Gym guide at the entrance. Two statues ("Juan's certified trainers: {PLAYER}" after badge 8).
* **Original gist:** Guide: "Yo, CHAMPION-bound {PLAYER}! Juan is a master of Water types… an icy floor will hamper your progress… the rest is up to you." Trainers: devoted pupils of Juan ("Please forgive me, Juan…", "Watch what happens if you crack all the floor tiles").
* **Text labels:** `SootopolisCity_Gym_1F_Text_GymGuideAdvice`, `..._GymGuidePostVictory`, `..._GymStatue`, `..._GymStatueCertified`; `SootopolisCity_Gym_B1F_Text_<Name>Intro/Defeat/PostBattle`.
* **HC:** The Gym is an ice-floor puzzle: cracked tiles break if stepped on twice and drop the player one floor down. The stairs appear only after a whole section is cleared. Ten trainers wait on the lower floor.

### D05 — Juan: Rain Badge (badge 8) [M]
* **Maps:** `SootopolisCity_Gym_1F`.
* **Mechanics:** `trainerbattle_single TRAINER_JUAN_1` (Leader Juan pic; `NO_MUSIC` flag). Defeat script: badge fanfare, `FLAG_DEFEATED_SOOTOPOLIS_GYM`, `FLAG_BADGE08_GET`, hides the Sootopolis spectators/Steven/Wallace, `VAR_SOOTOPOLIS_CITY_STATE = 6`, shows `FLAG_HIDE_SOOTOPOLIS_CITY_MAN_1`, `Common_EventScript_SetGymTrainers` (case 8, the remaining Gym trainers count as beaten), gives **`ITEM_TM_WATER_PULSE`** (`FLAG_RECEIVED_TM_WATER_PULSE`), then `MUS_REGISTER_MATCH_CALL` → `FLAG_ENABLE_JUAN_MATCH_CALL`. Afterwards: "go to the Pokémon League on Ever Grande, the easternmost island". If `FLAG_BADGE06_GET` is missing he says "get the last badge in Fortree" (Ruby/Sapphire leftover; normally unreachable). Post-game rematch: `trainerbattle_rematch_double TRAINER_JUAN_1` (needs 2 Pokémon) via the Match Call rematch system (`src/gym_leader_rematch.c`, game clear only).
* **Original gist:** Juan: "It was I who taught Wallace everything… once I gave up my position and entrusted the Gym to Wallace… a compelling reason arose for me to make a comeback… bear witness to our artistry, a grand illusion of water." Defeat: "From you I sense the brilliant shine of skill… but you lack elegance. Perhaps I should loan you my outfit?… I jest! Take the Rain Badge." Rematch: "our young typhoon has returned… I would like to make a gift of my coat… you will refuse… a sign of nobility."
* **Characters:** Juan (Leader Juan pic; Gym Leader, Wallace's mentor).
* **Text labels:** `SootopolisCity_Gym_1F_Text_JuanIntro`, `..._JuanDefeat` (**warning:** it overflows the battle string buffer by ~50 bytes. Keep the Italian text shorter than the English), `..._ReceivedRainBadge`, `..._ExplainRainBadgeTakeThis`, `..._ExplainWaterPulse`, `..._RegisteredJuan`, `..._JuanPostBattle`, `..._GoGetFortreeBadge`, `..._JuanPreRematch`, `..._JuanRematchDefeat`, `..._JuanPostRematch`, `..._JuanRematchNeedTwoMons`.
* **HC:** The last Gym Leader is the mentor of the region's strongest trainer (Wallace). He gives badge 8, which enables Waterfall, and a Water TM, and joins the PokéNav. He sends the player east to the League.

### D06 — Ever Grande City and its Pokémon Center; Scott's farewell [M / O]
* **Maps:** `Route128` (sea) → `EverGrandeCity` (the south part is reached by surfing and climbing a waterfall) → `EverGrandeCity_PokemonCenter_1F`.
* **Mechanics:** A row of coordinate triggers on y = 58 sets `FLAG_VISITED_EVER_GRANDE_CITY` (Fly point). The Pokémon Center sets the respawn to `HEAL_LOCATION_EVER_GRANDE_CITY`. **Scott** appears only if `FLAG_BADGE06_GET` is set and `FLAG_MET_SCOTT_IN_EVERGRANDE` is unset. Talking to him: speech, he walks out, `VAR_SCOTT_STATE += 1`, `FLAG_MET_SCOTT_IN_EVERGRANDE`. Entering Sidney's room also sets that flag and hides him, so the scene is **missable**. Signs: "Entering Victory Road", "Entering Pokémon League Center Gate", city sign.
* **Original gist:** Scott: "You've clawed your way up to face the Pokémon League! You made my cheering worthwhile! If you become Champion… I'll get in touch with you then. Go for greatness!" Woman: "The League is a short distance after Victory Road." Expert: "Victory Road… like reliving the path one has travelled in life. Believe in your Pokémon."
* **Text labels:** `EverGrandeCity_Text_EnteringVictoryRoad`, `..._EnteringPokemonLeague`, `..._CitySign`; `EverGrandeCity_PokemonCenter_1F_Text_ScottHappyForYou`, `..._LeagueAfterVictoryRoad`, `..._BelieveInYourPokemon`.
* **HC:** The "scout" character who has followed the player all game says goodbye before the League and **promises to contact the player if they become Champion** (this is paid off by the call in D19).

### D07 — Victory Road: Wally's ambush [M]
* **Maps:** `VictoryRoad_1F`.
* **Mechanics:** Coordinate triggers at (2,23)/(3,23) with `VAR_VICTORY_ROAD_1F_STATE == 0`. Wally (`OBJ_EVENT_GFX_WALLY`, hidden at (12,25)) is added, walks ~10 tiles west and one north to stand below the player. The player turns south. Speech → `trainerbattle_no_intro TRAINER_WALLY_VR_1` → post line. `FLAG_DEFEATED_WALLY_VICTORY_ROAD` is set, the entrance Wally stays there permanently (`copyobjectxytoperm`), and `VAR_VICTORY_ROAD_1F_STATE = 1/2` stores which tile was used. Until the HoF he repeats his post-battle line (`VictoryRoad_1F_EventScript_EntranceWally`). Side effects: Wally's parents (Petalburg) don't believe the player met him in Ever Grande; his uncle and aunt (Verdanturf) are amazed; Match Call `MatchCall_Text_Wally7` ("Before I met you I hardly left my house… thank you").
* **Original gist:** Wally: "Hi! I bet you're surprised to see me here! I made it all the way here, and it's all thanks to you! Losing to you that time made me stronger! But I'm not going to lose anymore! For the Pokémon who gave me courage and strength! Here I come!" Defeat: "You are strong, after all!" After: "I couldn't beat you today, but one of these days I'll catch up!"
* **Characters:** Wally (Rival class, Wally pic; the frail boy from Petalburg, now a determined trainer).
* **Text labels:** `VictoryRoad_1F_Text_WallyNotGoingToLoseAnymore`, `..._WallyEntranceDefeat`, `..._WallyPostEntranceBattle`; `PetalburgCity_WallysHouse_Text_YouMetWallyInEverGrandeCity`; `VerdanturfTown_WandasHouse_Text_WallyGoneThatFar`, `..._WallyWasInEverGrande`.
* **HC:** The once-sick boy the player helped at the start walks up **unannounced** inside Victory Road and demands a battle, crediting the player for his growth. He loses and stays standing in the cave.

### D08 — Crossing Victory Road; the League gate [M]
* **Maps:** `VictoryRoad_1F`, `VictoryRoad_B1F`, `VictoryRoad_B2F` → `EverGrandeCity` (north) → `EverGrandeCity_PokemonLeague_1F`.
* **Mechanics:** Victory Road has Strength boulders and Rock Smash rocks (B1F) and items (Max Elixir, PP Up, TM Psychic, Full Restore, Full Heal, hidden Ultra Ball/Elixir/Max Repel). Trainers: 1F **EDGAR, HOPE, ALBERT, KATELYNN, QUINCY**; B1F **SAMUEL, SHANNON, MICHELLE, MITCHELL, HALLE**; B2F **VITO, OWEN, CAROLINE, JULIE, DIANNE, FELIX**. The 1F north exit leads to the League. **League 1F:** nurse (`HEAL_LOCATION_EVER_GRANDE_CITY_POKEMON_LEAGUE` respawn), clerk (Ultra Ball, Hyper Potion, Max Potion, Full Restore, Full Heal, Revive, Max Repel), two door guards (`OBJ_EVENT_GFX_MAN_3`) standing on the door until `FLAG_ENTERED_ELITE_FOUR`. Talking to a guard: the player is walked in front of the door, "let us confirm your badges", 2 s pause, check (**only `FLAG_BADGE06_GET` is tested**), guards step aside, `MUS_OBTAIN_BADGE` fanfare, "Believe in yourself and go forth!", `FLAG_ENTERED_ELITE_FOUR`. 2F is an unused cable-club room (Ruby/Sapphire leftover). The door leads through `EverGrandeCity_Hall5` to Sidney's room.
* **Original gist:** Edgar: "I've made it this far a couple times, but the last stretch is so long…" Guards: "Beyond this point only trainers with all the Gym badges… Trainer! Believe in yourself and your Pokémon, and go forth!"
* **Text labels:** `VictoryRoad_<floor>_Text_<Name>Intro/Defeat/PostBattle`; `EverGrandeCity_PokemonLeague_1F_Text_MustHaveAllGymBadges`, `..._HaventObtainedAllBadges`, `..._GoForth`.
* **HC:** A long cave full of veteran trainers leads to the League. Two guards physically block the inner door and step aside after a badge check.

### D09–D12 — The Elite Four (Sidney → Phoebe → Glacia → Drake) [M]
* **Maps:** `EverGrandeCity_Hall5` → `SidneysRoom` → `Hall1` → `PhoebesRoom` → `Hall2` → `GlaciasRoom` → `Hall3` → `DrakesRoom` → `Hall4` (long corridor) → `ChampionsRoom`. The halls are empty corridors (they only turn the player north).
* **Mechanics (identical for the 4 rooms):** On entering, `OnFrame` with `VAR_ELITE_4_STATE == n` → the player walks 6 tiles in, the entry door slams (`SE_TRUCK_DOOR`, `PokemonLeague_EliteFour_EventScript_WalkInCloseDoor`), `VAR_ELITE_4_STATE = n+1` (Sidney 0→1, Phoebe 1→2, Glacia 2→3, Drake 3→4). The member stands at (6,5). Talk → `MUS_ENCOUNTER_ELITE_FOUR` → intro → `trainerbattle_no_intro` → defeat: `FLAG_DEFEATED_ELITE_4_<NAME>`, the far door opens and the spotlights switch off (`PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles`) → post-battle speech. Drake also gives a fan-counter increment (`FANCOUNTER_DEFEATED_DRAKE`). Entering Sidney's room hides Ever Grande Scott. A whiteout anywhere runs `EventScript_WhiteOut` → `EverGrandeCity_HallOfFame_EventScript_ResetEliteFour` (all 4 flags cleared, `VAR_ELITE_4_STATE = 0`), so the challenge restarts.
* **D09 Sidney** — `TRAINER_SIDNEY` (Elite Four Sidney pic, Dark type in the original). Gist: "I like that look you're giving me… let's enjoy a battle that can only be staged here!" / "I lost! Eh, it was fun." / "You've got what it takes to go far. Go on to the next room."
* **D10 Phoebe** — `TRAINER_PHOEBE` (Ghost). Gist: "I trained on Mt. Pyre… I gained the ability to commune with Ghost Pokémon… just try to inflict damage!" / "Oh, darn." / "There's a definite bond between you and your Pokémon, too… go ahead."
* **D11 Glacia** — `TRAINER_GLACIA` (Ice). Gist: "I've travelled from afar to Hoenn to hone my ice skills… all I've seen are weak trainers… what about you?" / "How hot your spirits burn!" / "Advance and confirm the truly fearsome side of the League."
* **D12 Drake** — `TRAINER_DRAKE` (Dragon). Gist: "Pokémon in their natural state are free… for us to battle as partners, do you know what is needed?" / "Superb." / "What a trainer needs is a virtuous heart… Go! The Champion is waiting!"
* **Text labels:** `EverGrandeCity_<Name>sRoom_Text_IntroSpeech`, `..._Defeat`, `..._PostBattleSpeech` (×4).
* **HC:** Four sealed rooms in a row. The door locks behind the player, each master must be beaten in order, and losing resets the run. Each member has a fixed specialty and a fixed sprite. The writer may change their personalities and lines, not the order or the battles.

### D13 — Champion Wallace [M]
* **Maps:** `EverGrandeCity_ChampionsRoom`.
* **Mechanics:** `OnFrame` (`VAR_TEMP_1 == 0`): the player walks 4 tiles, pauses, walks 2 more to face Wallace (`LOCALID_CHAMPIONS_ROOM_WALLACE` at (6,5)). **No interaction needed:** `MUS_ENCOUNTER_CHAMPION`, intro speech, `trainerbattle_no_intro TRAINER_WALLACE` (Champion Wallace pic).
* **Original gist:** "Welcome, {PLAYER}. That incident in Sootopolis… superb work, ending that crisis all by yourself. Oops! It wouldn't be fair to say you alone… you overcame it working as one with your Pokémon… We trainers also learn many things from Pokémon… Who can most elegantly dance with their Pokémon in Hoenn? Show me!" Defeat: "I, the Champion, fall in defeat… You were elegant, infuriatingly so… You are a truly noble trainer!"
* **Text labels:** `EverGrandeCity_ChampionsRoom_Text_IntroSpeech`, `..._Defeat`.
* **HC:** The Champion is the same person who gave HM Waterfall in Sootopolis and **refers explicitly to the crisis the player solved**. The battle starts automatically when the player reaches the throne.

### D14 — After the battle: the rival bursts in, Birch arrives [M]
* **Maps:** `EverGrandeCity_ChampionsRoom`.
* **Mechanics:** The far door opens (`METATILE_EliteFour_OpenDoorChampion_*`). Wallace starts his proclamation (`..._PostBattleSpeech`, which ends mid-sentence: "I now proclaim you to be the new Hoenn region…"). A door sound plays, then the rival's theme (`MUS_ENCOUNTER_MAY` / `MUS_ENCOUNTER_BRENDAN`). **The rival** (`OBJ_EVENT_GFX_VAR_0`, set by `Common_EventScript_SetupRivalGfxId`) runs in from the south and stops left of the player: "Here's some advice before you challenge the Champion…" → "!" → looks back and forth → "It's already over?" **Prof. Birch** (`OBJ_EVENT_GFX_PROF_BIRCH`) walks in from the south: "See? What did I tell you, {RIVAL}?… You defeated your own father… become the Champion! What became of your Pokédex?" → `ProfBirch_EventScript_RatePokedex` (standard Pokédex rating text, `data/text/pokedex_rating.inc`) → "Congratulations! Go proudly into the final room!" Wallace looks up, then down: "No, let me rephrase that. The new Champion! Come with me." Wallace and the player walk to the north door. Birch faces up, and the rival follows one step and is stopped: "From here on, only Champions may enter. Wait outside with Prof. Birch." Rival: (May) "Groan… I'm just joking! That's the rule! Honestly, congratulations!" / (Brendan) "Whaaaat?!… It can't be helped. Way to go!" Wallace and the player walk out. `setflag FLAG_HIDE_PETALBURG_GYM_GREETER`, warp to `EverGrandeCity_HallOfFame` (7,16).
* **Text labels:** `EverGrandeCity_ChampionsRoom_Text_PostBattleSpeech`, `..._MayAdvice`, `..._MayItsAlreadyOver`, `..._BrendanAdvice`, `..._BrendanYouveWon`, `..._BirchArriveRatePokedex`, `..._BirchCongratulations`, `..._WallaceComeWithMe`, `..._WallaceWaitOutside`, `..._MayCongratulations`, `..._BrendanCongratulations`.
* **Characters:** Wallace, rival (May/Brendan), Prof. Birch.
* **HC:** The Champion's proclamation is **cut off** by the rival rushing in to give advice, too late. The professor follows, praises the player, checks the Pokédex, and the Champion takes **only the player** through the north door. The rival is told to wait outside and accepts it.

### D15 — The Hall of Fame record [M]
* **Maps:** `EverGrandeCity_HallOfFame`.
* **Mechanics:** `OnFrame`: Wallace (`LOCALID_HALL_OF_FAME_WALLACE`) and the player walk 6 tiles side by side and face each other: "This room is where we keep records of Pokémon that prevailed… where League Champions are honoured." They walk 5 more and face each other: "Let's record your name… and the names of the partners who battled with you." Both face the machine: `FLDEFF_HALL_OF_FAME_RECORD` (Poké Ball machine animation). Then `EverGrandeCity_HallOfFame_EventScript_SetGameClearFlags` runs (`data/scripts/hall_of_fame.inc`): `SetChampionSaveWarp`, `FLAG_IS_CHAMPION`, reset of the event legendaries (Mew, Lati@s, Deoxys, Lugia, Ho-Oh "defeated" flags), `VAR_FOSSIL_MANIAC_STATE = 1` (if 0), Lilycove Motel designers shown, E4 reset, Briney moves from Stern's shipyard to the S.S. Tidal, Steven's house letter shown (`VAR_STEVENS_HOUSE_STATE = 2`), Victory Road Wally moves from the entrance to the exit, both S.S. Tidal objects shown, Safari Zone expansion opened, Lilycove rival hidden, `UpdateTrainerFanClubGameClear`, (no S.S. Ticket yet →) `VAR_LITTLEROOT_HOUSES_STATE_MAY/BRENDAN = 3` + Dad shown, (no Beldum yet →) Beldum ball shown, rival bedrooms emptied, (if 0 →) `VAR_DEX_UPGRADE_JOHTO_STARTER_STATE = 1`. Respawn set to the player's own bedroom (`HEAL_LOCATION_LITTLEROOT_TOWN_BRENDANS/MAYS_HOUSE_2F`). Fade to black, `special GameClear`.
* **Text labels:** `EverGrandeCity_HallOfFame_Text_HereWeHonorLeagueChampions`, `..._LetsRecordYouAndYourPartnersNames`.
* **HC:** The Champion personally escorts the player into a record room, and the party is registered on a machine. Everything in the post-game is unlocked at this moment.

### D16 — Hall of Fame screen, credits, "The End" [M]
* **Code:** `src/post_battle_event_funcs.c` (`GameClear`: heals the party, sets `FLAG_SYS_GAME_CLEAR`, stores the first-HoF play time, sets the continue warp to the bedroom, gives the **Champion Ribbon** to every party Pokémon, may air the "Spot the Cuties" TV show), then `src/hall_of_fame.c` (`CB2_DoHallOfFameScreen`: saves the game, "SAVING… DON'T TURN OFF THE POWER", shows each party Pokémon with "Welcome to the HALL OF FAME!", then the player's trainer picture with name/ID/time and "LEAGUE CHAMPION! CONGRATULATIONS!") → `CB2_StartCreditsSequence` (`src/credits.c`, `src/data/credits.h`).
* **Credits structure (`CheckChangeScene`):** 9 scenes cycling with the credit pages: 5 bike scenes (**player on a bicycle**: ocean morning, ocean sunset, forest sunset where **the rival arrives**, forest sunset where the player **catches up with the rival**, town at night) alternating with 4 Pokémon interludes. The Pokémon shown come from the player's caught list (`DeterminePokemonToShow`), and **the starter is always last**. It ends with the letters "THE END" (`sTheEnd_LetterMap_*`) and returns to the title screen.
* **Strings:** `src/data/credits.h` (`sCreditsText_PkmnEmeraldVersion` "POKéMON EMERALD VERSION", "Credits", all staff roles and names); `src/strings.c` `gText_WelcomeToHOF`, `gText_LeagueChamp`, `gText_HOFNumber`, `gText_HOFCorrupted`, `gText_PickCancel`, `gText_HallOfFame` (PC menu entry).
* **HC:** The credits show the protagonist **riding a bicycle**, joined by the rival, plus the Pokémon caught on the journey. **Sprite impact:** the new protagonist (red spiky hair, black modern clothes) needs replacement art for the HoF trainer picture (front pic) and the credits bike sprites (the intro/credits bike graphics). The rival bike sprites stay.

### D17 — Waking up at home: Dad's S.S. Ticket and the emergency TV report [M, automatic]
* **Maps:** `LittlerootTown_<Own>House_2F` (respawn) → `LittlerootTown_<Own>House_1F`. The logic is in `data/scripts/players_house.inc` (`PlayersHouse_1F_EventScript_GetSSTicketAndSeeLatiTV`). Both houses trigger it on `VAR_LITTLEROOT_HOUSES_STATE_MAY == 3`.
* **Mechanics:** Coming downstairs: **Dad** (Norman sprite, `LOCALID_PLAYERS_HOUSE_1F_DAD`) faces the player with "!", walks over, speaks, **gives `ITEM_SS_TICKET`** ("came for you from someone named Mr. Briney"), mentions the ports in Slateport and Lilycove. Mom walks to Dad. Dad: "I'd better get back to Petalburg Gym. Mom, thanks for looking after the house." He walks out (door sound, removed). `FLAG_RECEIVED_SS_TICKET`. Mom complains that Dad should stay longer. The **TV turns on by itself** (`FLAG_SYS_TV_LATIAS_LATIOS`, `TurnOnTVScreen`). Mom: "Is that a breaking news story?" The player walks to the TV: emergency news flash, "reports of a **BZZT**…coloured Pokémon in flight… identity unknown". TV off, `FLAG_LATIOS_OR_LATIAS_ROAMING`. Mom: "What colour did the announcer say?" → multichoice **RED / BLUE** (`MULTI_TV_LATI`) → `special InitRoamer` (RED → **Latias** roams, BLUE → **Latios** roams, both level 40) → `VAR_ROAMER_POKEMON` = answer. Mom: "There are still unknown Pokémon." `VAR_LITTLEROOT_HOUSES_STATE_* = 4` (Mom: "Don't push yourself too hard… you can always come home").
* **Original gist:** Dad: "It's been a while… you look stronger, somehow. But your old man hasn't given up yet! This came to you from someone named Mr. Briney." TV: "We bring you this emergency news flash! In various Hoenn locales there have been reports of a BZZT…coloured Pokémon in flight…"
* **Text labels:** stored in `LittlerootTown_BrendansHouse_1F/scripts.inc` and shared: `PlayersHouse_1F_Text_TicketFromBrineyCameForYou`, `..._PortsInSlateportLilycove`, `..._BetterGetBackToGym`, `..._DadShouldStayLonger`, `..._IsThatABreakingStory`, `..._LatiEmergencyNewsFlash`, `..._WhatColorDidTheySay`, `..._StillUnknownPokemon`, `..._DontPushYourselfTooHard`. Multichoice strings "RED"/"BLUE" are in `src/data/script_menu.h` (`MultichoiceList_TVLati`).
* **HC:** The father visits, hands over a **ferry ticket sent by the old sailor**, and leaves for his Gym. A TV news flash about a **mysterious flying Pokémon whose colour is drowned by static** follows. The player's answer (red/blue) decides which of the two roams. The garbled colour word must stay garbled, because the player has to choose.

### D18 — Outside: Prof. Birch and the rival; National Pokédex [M, automatic]
* **Maps:** `LittlerootTown` → `LittlerootTown_ProfessorBirchsLab`.
* **Mechanics:** With `VAR_DEX_UPGRADE_JOHTO_STARTER_STATE == 1`, Birch and the rival are placed outside the player's house (`..._SetRivalBirchPosForDexUpgrade`, map name popup hidden). `OnFrame` → Birch "!" → speech → warp into the lab (6,5) with both present. Lab `OnFrame` state 1: "I've had the two of you help me study Pokémon… in Hoenn there are also Pokémon from other regions… I'll upgrade your Pokédex to the National Mode." Birch walks to the machine (four clicks). The rival comments (May: "so cool even my Pokédex is getting updated, because you caught so many"; Brendan: "you can thank me… I went all over Hoenn"). PC sound → "Okay, all done!" → fanfare "{PLAYER}'s Pokédex was upgraded to the National Mode!" → `FLAG_SYS_NATIONAL_DEX`, `EnableNationalPokedex` → "You've become the Champion, but your journey isn't over… there is no end to the road that is Pokémon. Somewhere a grassy patch is waiting for you!" `VAR_DEX_UPGRADE_JOHTO_STARTER_STATE = 2` (→ 3 on leaving), `VAR_SCOTT_BF_CALL_STEP_COUNTER = 0`, `FLAG_SCOTT_CALL_BATTLE_FRONTIER`. From now on the rival stays in the lab ("taking a break from fieldwork to help the Prof", later "Have you gone to the Battle Frontier?").
* **Text labels:** `LittlerootTown_Text_BirchSomethingToShowYouAtLab`; `LittlerootTown_ProfessorBirchsLab_Text_OtherRegionsUpgradeToNational`, `..._MayUpgradeSoCool`, `..._BrendanYouCanThankMe`, `..._OkayAllDone`, `..._PokedexUpgradedToNational`, `..._GrassyPatchWaiting2`, `..._May/BrendanTakeBreakFromFieldwork`, `..._May/BrendanHaveYouGoneToBattleFrontier`, `..._May/BrendanWhereShouldIGoNext`.
* **HC:** The professor says that **Pokémon from other regions have been found in Hoenn** and upgrades the Pokédex. This is the natural narrative hinge for the multi-region expansion: writers can extend this speech to announce the other regions.

### D19 — Scott's PokéNav call: the invitation [M, automatic]
* **Code:** `ShouldDoScottBattleFrontierCall` (`src/field_specials.c`): with `FLAG_SCOTT_CALL_BATTLE_FRONTIER` set, every step on a town/city/route/ocean-route map increments `VAR_SCOTT_BF_CALL_STEP_COUNTER`. At 10, `src/field_control_avatar.c` runs `LittlerootTown_ProfessorBirchsLab_EventScript_ScottAboardSSTidalCall` (`pokenavcall`), then the flag is cleared.
* **Original gist:** "Beep! Scott: Hi, hi! I'm aboard the S.S. Tidal now… There's a place I'd like to invite you to. Board a ferry at either Slateport or Lilycove. I'll fill you in when we meet!"
* **Text label:** `LittlerootTown_ProfessorBirchsLab_Text_ScottAboardSSTidalCall`.
* **HC:** The scout keeps the promise from D06 and invites the Champion to "a place", reachable **by ferry from Slateport or Lilycove**.

### D20 — First S.S. Tidal voyage: Scott on board; the Battle Frontier unlocked [O, but the only way to the Frontier]
* **Maps:** `SlateportCity_Harbor` / `LilycoveCity_Harbor` → `SSTidalCorridor` (+ `SSTidalRooms`, `SSTidalLowerDeck`). Full ferry mechanics are in §4.
* **Mechanics:** The first time the player enters the corridor (`VAR_SS_TIDAL_SCOTT_STATE == 0`), **Scott** walks to the player: invitation speech. Scott walks to the exit while the exit sailor steps aside, then Scott leaves (`SE_EXIT`, removed). `FLAG_MET_SCOTT_ON_SS_TIDAL`, `VAR_SS_TIDAL_SCOTT_STATE = 1`. From now on all three ferry menus offer **BATTLE FRONTIER**. Also on board after the HoF: **Mr. Briney + Peeko** ("Welcome aboard"), 8 trainers (Lower deck: **PHILLIP, LEONARD**; cabins: **COLTON, MICAH, THOMAS, LEA_AND_JED** (double), **GARRET, NAOMI**), a sailor who notices when all of them are beaten (`FLAG_DEFEATED_SS_TIDAL_TRAINERS`), the **Snatch TM giver** (a "not suspicious" Maniac: `ITEM_TM_SNATCH`, `FLAG_RECEIVED_TM_SNATCH`; he disappears after the voyage), and a bed that heals the party and advances the voyage.
* **Original gist:** Scott: "Well, hi, hi! Something's come up so I have to disembark, but am I glad to see you! Congratulations, League Champion! There's a place I'd like to invite someone like you… the BATTLE FRONTIER! You'll understand when you see it! I've spoken with the ship's captain: the next time you take a ferry you can sail to the Battle Frontier."
* **Text labels:** `SSTidalCorridor_Text_ScottBattleFrontierInvite`, `SSTidalCorridor_Text_BrineyWelcomeAboard`, `..._Peeko`, `..._VisitOtherCabins`, `..._EnjoyYourCruise`, `..._CanRestInCabin2`, `..._WeveArrived`, `..._HorizonSpreadsBeyondPorthole`, `..._Cabin1–4`; `SSTidalRooms_Text_NotSuspiciousTakeThis`, `..._ExplainSnatch`, `..._TakeRestOnBed`, trainer texts; `SSTidal_Text_FastCurrentsHopeYouEnjoyVoyage`, `..._HopeYouEnjoyVoyage`, `..._MadeLandInSlateport`, `..._MadeLandInLilycove`.
* **HC:** The scout is met **on the ferry**, invites the player and leaves the ship. The new destination exists only after this meeting.

### D21 — Arrival at the Battle Frontier: Frontier Pass [O]
* **Maps:** `BattleFrontier_OutsideWest` (ferry dock at (19,67)) → `BattleFrontier_ReceptionGate`.
* **Mechanics:** First entry (`VAR_HAS_ENTERED_BATTLE_FRONTIER == 0`): greeter (`OBJ_EVENT_GFX_TEALA`) "!" → "First time here? This way." → the player walks to the counter → welcome → **Frontier Pass** issued (fanfare; "placed your Trainer Card in the Frontier Pass"; `FLAG_SYS_FRONTIER_PASS`) → a voice: "Well, if it isn't {PLAYER}! You came out here!" → everyone "!" → guide: "Oh! Mr. Scott, sir!" → **Scott** walks in: "Great to see you here… explore everywhere… experience the pure essence of battling. I have my quarters here, visit if you have time." → he leaves. The facility guide explains the facilities (scrollable menu: Tower, Dome, Palace, Arena, Factory, Pike, Pyramid, Ranking Hall, Exchange Corner). The rules guide and the Frontier Pass guide stand nearby.
* **Text labels:** `BattleFrontier_ReceptionGate_Text_FirstTimeHereThisWay`, `..._WelcomeToBattleFrontier`, `..._IssueFrontierPass`, `..._ObtainedFrontierPass`, `..._PlacedTrainerCardInFrontierPass`, `..._EnjoyBattleFrontier`, `..._IfItIsntPlayerYouCame`, `..._OhMrScottGoodDay`, `..._ScottGreatToSeeYouHere`, `..._YourGuideToFacilities`, `..._LearnAboutWhich2`, `..._<Facility>Info`.
* **HC:** The Frontier is **Scott's creation**. The staff address him as the boss, and he lives on site.

### D22 — Scott's house: the memento and the symbol rewards [O, repeatable checks]
* **Maps:** `BattleFrontier_ScottsHouse` (door at `BattleFrontier_OutsideWest` (44,5)).
* **Mechanics:** First talk: "Let me formally welcome you… this is my dream come true." He turns away: "I left home alone to find strong trainers… no one can imagine how much effort it took." He faces the player: "Have this as a memento of all the times our paths crossed." He gives **Battle Points depending on `VAR_SCOTT_STATE`**: 13 → 4 BP, ≥ 9 → 3, ≥ 6 → 2, otherwise 1 (`special GiveFrontierBattlePoints`; `FLAG_SCOTT_GIVES_BATTLE_POINTS`). Later checks, in order: all 7 **silver** symbols → `ITEM_LANSAT_BERRY`; all 7 **gold** symbols → `ITEM_STARF_BERRY`; Battle Tower single-battle streak ≥ 50 (level 50 or Open) → `DECOR_SILVER_SHIELD`; ≥ 100 → `DECOR_GOLD_SHIELD`. Otherwise one of three random comments (why he scouts trainers; have you met the Frontier Brains, whom he hand-picked; there may be wild Pokémon in the Frontier, a hint for Artisan Cave and Sudowoodo).
* **Text labels:** `BattleFrontier_ScottsHouse_Text_*` (WelcomeToBattleFrontier, HowMuchEffortItTookToMakeReal, HaveThisAsMementoOfOurPathsCrossing, ObtainedXBattlePoints, ExplainBattlePoints, ExpectingGreatThings, WhyIGoSeekingTrainers, HaveYouMetFrontierBrain, MayFindWildMonsInFrontier, YouveCollectedAllSilverSymbols, YouveCollectedAllGoldSymbols, SoGladIBroughtYouHere, BerryPocketStuffed, Beat50TrainersInARow, Beat100TrainersInARow, ExpectingToHearEvenGreaterThings, ComeBackForThisLater).
* **HC:** Scott's reward grows with **how many times the player met him** during the main story. His backstory is "a lonely search for strong trainers turned into this place".

### D23 — The seven Frontier Brains [O / S]
See §5 for the facility mechanics. Each Brain appears inside their facility at fixed win-streak thresholds, once for a **Silver Symbol** and once for a **Gold Symbol**. Data: `gFrontierBrainInfo` in `src/frontier_util.c` (trainer, sprite, win/lose quips, `streakAppearances`, symbol flags). Parties: `sFrontierBrainsMons`.

| Facility (map) | Brain (trainer const / class) | Silver / Gold at (streakAppearances[0]/[1]) | Intro gist (first meeting) | Text labels |
|---|---|---|---|---|
| Battle Tower (`BattleFrontier_BattleTowerBattleRoom`) | **Anabel**, `TRAINER_ANABEL`, Salon Maiden | 35 / 70 | "Greetings… my name is Anabel, the Salon Maiden, in charge of the Tower… I have heard rumours about you… there is but one reason I've come…" | `BattleFrontier_BattleTowerBattleRoom_Text_GreetingsImAnabel`, `..._AnabelTalentShallBeRecognized`, `..._AnabelCongratsYourPassPlease`, `..._AnabelYouCameBack` |
| Battle Dome (`BattleFrontier_BattleDomeBattleRoom`, `…PreBattleRoom`, `…Lobby`) | **Tucker**, `TRAINER_TUCKER`, Dome Ace | 4 / 9 (tourney wins; he is the next final) | "Do you hear this crowd?… I'm the no. 1 star of the Battle Dome! I, Tucker the Dome Ace, will bathe you in my brilliant glow!" | `..._TuckerSilverIntro`, `..._TuckerGoldIntro`, `..._MakeWayForDomeAceTucker`, `..._LegendHasReturnedDomeAceTucker`, `..._SpectatorTuckerChant`, `BattleFrontier_BattleDomeLobby_Text_CongratsDefeatedTucker` |
| Battle Palace (`BattleFrontier_BattlePalaceBattleRoom`) | **Spenser**, `TRAINER_SPENSER`, Palace Maven | 21 / 42 | "My physical being is with Pokémon always! My heart beats as one with them! Do you believe in your Pokémon through and through?" | `..._SpenserFirstIntro`, `..._AnnounceArrivalOfSpenser`, `..._SpenserYourTeamIsAdmirable`, `..._SpenserPostSilverBattle`, `..._SpenserThisTimeWontHoldBack`, … |
| Battle Arena (`BattleFrontier_BattleArenaBattleRoom`) | **Greta**, `TRAINER_GRETA`, Arena Tycoon | 28 / 56 | "Hey! Howdy! …Wait, are you the challenger?" | `..._MakeWayForGreta`, `..._GretaYoureChallenger`, `..._GretaYoureToughAfterAll`, `..._GretaBlownAway`, `..._GretaLookingForwardToSeeingAgain` |
| Battle Factory (`BattleFrontier_BattleFactoryBattleRoom`) | **Noland**, `TRAINER_NOLAND`, Factory Head | 21 / 42 | "I'm the Factory Head… knowledge isn't only about books… experience things with your heart and body… I'll use rental Pokémon too!" | `..._NolandImFactoryHead`, `..._NolandLetsSeeFrontierPass` |
| Battle Pike (`BattleFrontier_BattlePikeRoomNormal`) | **Lucy**, `TRAINER_LUCY`, Pike Queen | 28 / 140 (rooms) | "… … … Show me your Frontier Pass…" | `..._LucyShowMeFrontierPass`, `..._LucyFrontierPass`, `..._LucyYouAgain` |
| Battle Pyramid (`BattleFrontier_BattlePyramidTop`) | **Brandon**, `TRAINER_BRANDON`, Pyramid King | 21 / 70 (floors) | "Aah, this life is grand! I'm Brandon, the Pyramid King… most call me the chief!" | `..._ImPyramidKingBrandon`, `..._BrandonFrontierPassPlease`, `..._BrandonRemarkableHaveThis`, `..._BrandonYouveReturned` |

The in-battle win/lose quips are `COMPOUND_STRING`s inside `gFrontierBrainInfo` (`lostTexts` / `wonTexts`, silver and gold variants). They are **not** in `.inc` files.
* **HC:** Seven bosses hand-picked by Scott. Each runs one facility with its own rule set, appears only after a winning streak, stamps a symbol on the Frontier Pass, and comes back stronger for the gold symbol. The sprites (`OBJ_EVENT_GFX_ANABEL` … `BRANDON`) and trainer pics are fixed.

### D24 — Mossdeep: Steven's letter and Beldum [O]
* **Maps:** `MossdeepCity_StevensHouse`.
* **Mechanics:** After the HoF, Steven himself is absent (hidden since Segment C). The **letter** (an invisible object at the table, (6,4)) and a **Poké Ball** on the desk are present (`FLAG_HIDE_MOSSDEEP_CITY_STEVENS_HOUSE_BELDUM_POKEBALL` cleared at HoF if `FLAG_RECEIVED_BELDUM` is unset). Reading the letter: Steven is leaving to train and asks the player to take the ball. The ball: "It contained Beldum. Take it?" → `givemon SPECIES_BELDUM, 5` (party or PC, nickname option) → `FLAG_RECEIVED_BELDUM`. The rock collection displays remain.
* **Original gist:** "To {PLAYER}… I've decided to do a little soul-searching and train on the road. I don't plan to return home for some time. I want you to take the Poké Ball on the desk. Inside is Beldum, my favourite Pokémon. I'm counting on you. May our paths cross someday. — Steven Stone"
* **Text labels:** `MossdeepCity_StevensHouse_Text_LetterFromSteven`, `..._TakeBallContainingBeldum`, `..._ObtainedBeldum`, `..._LeftPokeBallWhereItWas`, `..._CollectionOfRareRocks`.
* **HC:** The mentor-type ally has **left his home** and leaves a written farewell plus **his favourite Pokémon (Beldum, Lv 5)** for the player.

### D25 — Meteor Falls: Steven's challenge [O]
* **Maps:** `MeteorFalls_1F_1R` → `MeteorFalls_StevensCave` (an inner cave in the Waterfall-gated part of Meteor Falls).
* **Mechanics:** The cave mouth at (3–5,1–2) of `MeteorFalls_1F_1R` is solid rock until `FLAG_SYS_GAME_CLEAR`. `MeteorFalls_1F_1R_OnLoad` → `..._OpenStevensCave` then draws the entrance metatiles, so the cave is **post-game only**. Steven (`OBJ_EVENT_GFX_STEVEN`, no hide flag) stands at the back (19,3). Talk → "!" → faces the player → speech → `trainerbattle_no_intro TRAINER_STEVEN` (Rival class, Steven pic) → `FLAG_DEFEATED_METEOR_FALLS_STEVEN`. Afterwards he repeats his closing line. No rematch.
* **Original gist:** "I'm amazed you knew where to find me. Do you think of me as just a rock maniac?… We battled alongside each other at the **Sootopolis** Space Center (*sic*: it was Mossdeep)… If you're going to mount a serious challenge, expect the worst!" Defeat: "I had no idea you'd become so strong…" After: "Ever since we met in Granite Cave I had this feeling you'd become Champion. My predictions usually come true. And where will you go from here?… Even I couldn't tell you that."
* **Text labels:** `MeteorFalls_StevensCave_Text_ShouldKnowHowGoodIAmExpectWorst` (fix the Sootopolis/Mossdeep slip in the rewrite), `..._StevenDefeat`, `..._MyPredictionCameTrue`.
* **HC:** The ally who fought beside the player against the villains (Space Center double battle) is found **training alone in a hidden cave** and offers the toughest optional battle. His closing line about "where will you go from here?" is a free hook toward the other regions.

### D26 — Wally at the Victory Road exit (rematches) [O / S]
* **Maps:** `VictoryRoad_1F` (Wally at (31,9), near the north exit).
* **Mechanics:** After the HoF the entrance Wally is hidden and the exit Wally is shown. `trainerbattle_single TRAINER_WALLY_VR_2` with a normal intro. After the first win it becomes `trainerbattle_rematch` through the rematch table (`REMATCH_WALLY_VR`, allowed only if `FLAG_DEFEATED_WALLY_VICTORY_ROAD`; `src/battle_setup.c`).
* **Original gist:** "I've gotten stronger since last time! I wanted to show you!" / "You are strong, after all!" / "One of these days I'm going to catch up… and challenge the Pokémon League!"
* **Text labels:** `VictoryRoad_1F_Text_WallyIntro`, `..._WallyDefeat`, `..._WallyPostBattle`.
* **HC:** Wally keeps training at the doorstep of the League, still aiming to challenge it.

### D27 — Prof. Birch's gift for a complete Hoenn Pokédex: a Johto starter [O]
* **Maps:** `LittlerootTown_ProfessorBirchsLab`.
* **Mechanics:** When the lab is entered with `VAR_DEX_UPGRADE_JOHTO_STARTER_STATE == 3` and `specialvar HasAllHoennMons` returns TRUE: state 4 → lab layout with a table holding **three Poké Balls** (`LAYOUT_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB_WITH_TABLE`) → the player is walked in, Birch checks the Dex: "You really have completed the Hoenn Pokédex… My gift is a rare Pokémon only found in another region! You can have any one of these three." State 5. Each ball shows the picture and asks "You'll take Cyndaquil/Totodile/Chikorita?" → `givemon` Lv 5 → nickname → "Somewhere a grassy patch is waiting" → state 6. The other balls: "You received the promised Pokémon. Better leave the others alone." The rival comments ("What are you going to do next?… I'm staying here to help the Prof." / Brendan: "I prefer slowly raising the one I chose").
* **Text labels:** `LittlerootTown_ProfessorBirchsLab_Text_CompletedDexChoosePokemon`, `..._CanHaveAnyOneOfRarePokemon`, `..._YoullTakeCyndaquil/Totodile/Chikorita`, `..._ReceivedJohtoStarter`, `..._GrassyPatchWaiting`, `..._TakeYourTimeAllInvaluable`, `..._BetterLeaveOthersAlone`, `..._MayWhatNextImStayingHere`, `..._BrendanPreferCollectingSlowly`.
* **HC:** The professor rewards a complete regional Dex with **one of three starters from another region** (Johto). This is a natural bridge to the Johto content in the multi-region plan.

### D28 — Lilycove Motel: the game designers' Diploma [O]
* **Maps:** `LilycoveCity_CoveLilyMotel_2F` (NPCs shown at HoF via `FLAG_HIDE_LILYCOVE_MOTEL_GAME_DESIGNERS`).
* **Mechanics:** The game designer checks `HasAllHoennMons` → fanfare → `special Special_ShowDiploma`. The programmer, graphic artist and other guests only have flavour lines.
* **Text labels:** `LilycoveCity_CoveLilyMotel_2F_Text_ShowMeCompletedDex`, `..._FilledPokedexGiveYouThis`, `..._ImTheProgrammer`, `..._ImTheGraphicArtist`, … The Diploma screen strings are in `src/diploma.c`.
* **HC:** None story-wise. It is the "developer room" joke. We can turn it into the hack's own credits room.

### D29 — The roaming Eon Pokémon and Southern Island [O]
* **Roamer:** `src/roamer.c`. Latias or Latios (level 40) moves between Hoenn routes. It is fought as a normal wild encounter that flees. Catching it ends the roaming.
* **Southern Island** (`SouthernIsland_Exterior`, `SouthernIsland_Interior`): reachable only by ferry from Lilycove with **`ITEM_EON_TICKET` + `FLAG_ENABLE_SHIP_SOUTHERN_ISLAND`**. In the vanilla game both come only from Mystery Gift / record mixing (`CableClub_EventScript_DistributeEonTicket`). On the first trip the harbour attendant leaves and an old sailor appears: "Aye, mate… a tiny spit of an island far in the south… That shivers my timbers! All aboard!" (`FLAG_SHOWN_EON_TICKET`). Interior: walking up triggers `SouthernIsland_Interior_EventScript_TryLatiEncounter`: the camera pans up, a cry, the **other** Eon Pokémon (the one *not* roaming: Latios if `VAR_ROAMER_POKEMON == 0`) flies down → `seteventmon … 50, ITEM_SOUL_DEW` → `BattleSetup_StartLatiBattle`. Caught → `FLAG_CAUGHT_LATIAS_OR_LATIOS`. Defeated → `FLAG_DEFEATED_LATIAS_OR_LATIOS` (reset at each HoF). Signs: "All dreams are but another reality. Never forget…" / "Those whose memories fade seek to carve them in their hearts…". The sailor outside takes the player back to Lilycove.
* **Text labels:** `EventTicket_Text_ThatPass`, `..._ShowEonTicket`, `..._SouthernIslandSailBack`, `..._SailHome`, `..._AsYouLike` (`data/text/event_ticket_1.inc`); `SouthernIsland_Interior_Text_Sign`, `SouthernIsland_Exterior_Text_Sign`; `gText_LegendaryFlewAway`.
* **HC:** The two Eon Pokémon are a pair: one flies freely around the region (colour chosen in D17), the other waits on a remote island linked to a dream / memory theme. A leftover object with the player-lookalike graphic (`OBJ_EVENT_GFX_VAR_0`, `FLAG_HIDE_SOUTHERN_ISLAND_EON_STONE`) is permanently hidden and can be ignored.

### D30 — Abnormal weather: Groudon in Terra Cave, Kyogre in Marine Cave [O]
* **Maps:** `Route119_WeatherInstitute_2F` (trigger) → one of 8 land spots (Routes 114 N/S, 115 W/E, 116 N/S, 118 E/W: a cave entrance appears, **drought**) → `TerraCave_Entrance` → `TerraCave_End`; or one of 8 sea spots (Routes 105 N/S, 125 W/E, 127 N/S, 129 W/E: rough deep water, **downpour**, Dive) → `Underwater_MarineCave` → `MarineCave_Entrance` → `MarineCave_End`.
* **Mechanics:** After `FLAG_SYS_GAME_CLEAR`, the scientist on Weather Institute 2F runs `special CreateAbnormalWeatherEvent` once per visit (`FLAG_TEMP_2`). If Kyogre is already done it always picks a Terra spot, if Groudon is done a Marine spot, otherwise random. He announces: "Presently a drought has been recorded in {STR_VAR_1}… could that mean, somewhere near…" or "heavy rainfall over {STR_VAR_1}". The spot gets its map tiles and weather from `data/scripts/abnormal_weather.inc`. It expires after a step budget (`AbnormalWeatherHasExpired`). At the cave end: the legendary approaches (affine animation), cry, `setwildbattle SPECIES_GROUDON/KYOGRE, 70` → `FLAG_DEFEATED_GROUDON/KYOGRE` and `VAR_SHOULD_END_ABNORMAL_WEATHER = 1` (on leaving: "The intense sunshine appears to have subsided…" / "The massive downpour appears to have stopped…"). With both done: "Abnormal weather is no longer being reported." The Slateport harbour sailor also comments on the abnormal weather until both are done.
* **Text labels:** `Route119_WeatherInstitute_2F_Text_GroudonWeather`, `..._KyogreWeather`, `..._NoAbnormalWeather`, `..._ChangingWeatherRidiculous`; `gText_AbnormalWeatherEnded_Rain`, `gText_AbnormalWeatherEnded_Sun` (`data/text/abnormal_weather.inc`); `SlateportCity_Harbor_Text_AbnormalWeather`, `..._LoveToGoDeepUnderwaterSomeday`.
* **HC:** The two titans that fought in Sootopolis did **not** disappear. Their presence shows up as localized droughts and downpours that a weather researcher reports, and each can be met **once** in a cave that opens only while the weather lasts.

### D31 — Desert Underpass: the second fossil [O]
* **Maps:** `Route114_FossilManiacsHouse` → `Route114_FossilManiacsTunnel` → `DesertUnderpass`.
* **Mechanics:** Before game clear, the tunnel's north wall is solid rock (`..._CloseDesertUnderpass`). After game clear the Fossil Maniac stands aside (6,5) and the passage is open. With `VAR_FOSSIL_MANIAC_STATE == 1` (set at HoF), stepping near him: "It's not safe that way… I was digging when the whole wall collapsed… a giant cavern underneath… no fossils there, I think" → state 2. In the Desert Underpass (one long cave, `DesertUnderpass`, entered from the tunnel at (6,2)) the fossil the player did **not** pick at Mirage Tower lies at the far end (132,10): Claw Fossil if `FLAG_CHOSE_ROOT_FOSSIL`, Root Fossil if `FLAG_CHOSE_CLAW_FOSSIL` (`FLAG_HIDE_DESERT_UNDERPASS_FOSSIL` was cleared when Mirage Tower collapsed in Segment B/C).
* **Text labels:** `Route114_FossilManiacsTunnel_Text_NotSafeThatWay`, `..._LookInDesertForFossils`, `..._DevonCorpRevivingFossils`, `..._FossilsAreWonderful`.
* **HC:** A collapse dug by the fossil collector opens a cavern where **the other fossil, lost when the tower sank**, can be found.

### D32 — Mirage Island [O]
* **Maps:** `Route130` (layout swapped to `LAYOUT_ROUTE130_MIRAGE_ISLAND`), `PacifidlogTown_House5` (the watcher).
* **Mechanics:** `IsMirageIslandPresent` (`src/time_events.c`) is TRUE when the low 16 bits of any party Pokémon's personality equal the daily random value. The island then appears, with Wynaut encounters and a Liechi Berry tree (`FLAG_TEMP_HIDE_MIRAGE_ISLAND_BERRY_TREE`). The old man in Pacifidlog House 5 says whether it is visible today. Not gated by the game clear.
* **HC:** A "phantom island" that only rarely appears. No story text beyond the watcher's lines.

### D33 — Birth Island (Aurora Ticket): Deoxys [O, dormant in vanilla]
* **Maps:** `BirthIsland_Harbor`, `BirthIsland_Exterior`.
* **Mechanics:** Ferry from Lilycove with `ITEM_AURORA_TICKET` + `FLAG_ENABLE_SHIP_BIRTH_ISLAND` (Mystery Gift: `data/scripts/gift_aurora_ticket.inc`). First trip: "Is it you who brought that odd ticket?… an island far, far away… Get on board, youngster!" The exterior has a **triangle rock** (`OBJ_EVENT_GFX_DEOXYS_TRIANGLE`). Each interaction (`special DoDeoxysRockInteraction`) moves it, a wrong order resets it (`VAR_DEOXYS_ROCK_LEVEL`, `VAR_DEOXYS_ROCK_STEP_COUNT`). When solved, the rock shatters (`FLDEFF_DESTROY_DEOXYS_ROCK`, `MUS_RG_ENCOUNTER_DEOXYS`), Deoxys descends → `seteventmon SPECIES_DEOXYS_NORMAL, 30` → `FLAG_BATTLED_DEOXYS` / `FLAG_DEFEATED_DEOXYS`. Harbour sailor: "What an oddly shaped island, eh? Return to Lilycove?"
* **Text labels:** `EventTicket_Text_OddTicketGetOnBoard`, `BirthIsland_Harbor_Text_SailorReturn` (`data/text/event_ticket_2.inc`).
* **HC:** A triangular stone must be "chased" across the island until a space Pokémon emerges.

### D34 — Faraway Island (Old Sea Map): Mew, and Mr. Briney's loyalty [O, dormant in vanilla]
* **Maps:** `LilycoveCity_Harbor`, `FarawayIsland_Entrance`, `FarawayIsland_Interior`.
* **Mechanics:** Needs `ITEM_OLD_SEA_MAP` + `FLAG_ENABLE_SHIP_FARAWAY_ISLAND` (`data/scripts/gift_old_sea_map.inc`). **First time only** (`FLAG_SHOWN_OLD_SEA_MAP`): the attendant is puzzled and fetches a sailor, who says "this is quite a ways away, I'm afraid I can't help…" → "!" → **Mr. Briney** (`LOCALID_LILYCOVE_HARBOR_BRINEY`, Expert M sprite) appears: "Hold on a second! What's the idea of turning down someone I owe so much to?… Let's find this island on the Old Sea Map!" They sail together. Entrance sign: a faded diary ("…ber, 6th day. If any human… sets foot here again… let it be a kindhearted person… with that hope, I depart…"). Interior: Mew hides in the tall grass and moves every few steps (`VAR_FARAWAY_ISLAND_STEP_COUNTER`). Facing it reveals it ("Myuu…") → `seteventmon SPECIES_MEW, 30` → caught `FLAG_CAUGHT_MEW` / defeated `FLAG_DEFEATED_MEW` (reset at HoF). The sailor at the entrance: "Capt. Briney can be so maddeningly fickle… return to Lilycove?"
* **Text labels:** `EventTicket_Text_ShowOldSeaMap`, `..._OldSeaMapTooFar`, `..._BrineyHoldOnASecond`, `..._BrineyLetsSail`, `FarawayIsland_Entrance_Text_SailorReturn`, `FarawayIsland_Entrance_Text_Sign`, `FarawayIsland_Interior_Text_Mew`.
* **HC:** The old sailor from the prologue steps in to repay his debt to the player. The island carries a sad, ancient diary entry. The mythical Pokémon plays hide-and-seek in the grass.

### D35 — Navel Rock (Mystic Ticket): Lugia and Ho-Oh [O, dormant in vanilla]
* **Maps:** `NavelRock_Harbor`, `NavelRock_Exterior`, `NavelRock_*` (Up/Down/Fork mazes), `NavelRock_Top`, `NavelRock_Bottom`.
* **Mechanics:** `ITEM_MYSTIC_TICKET` + `FLAG_ENABLE_SHIP_NAVEL_ROCK`. Ho-Oh at the top, Lugia at the bottom, both `seteventmon … 70`, `FLAG_CAUGHT_/DEFEATED_HO_OH/LUGIA` (the "defeated" flags reset at HoF). Harbour sailor: "Did you hear that? That low growling from deep in there… Do you think we should leave?"
* **Text labels:** `EventTicket_Text_OddTicketGetOnBoard`, `NavelRock_Harbor_Text_SailorReturn`.
* **HC:** A rock island with two Johto legendaries at opposite ends (top and bottom). Note: Ho-Oh and Lugia are Johto Pokémon, so in our multi-region plan this beat may be better placed or echoed in Johto.

### D36 — Battle Frontier wilds: Artisan Cave and the Sudowoodo [O]
* **Maps:** `ArtisanCave_B1F` (entrance at `BattleFrontier_OutsideWest` (39,55)), `ArtisanCave_1F` (exit at `BattleFrontier_OutsideEast` (28,7)). Wild Smeargle, items (Carbos, HP Up). No script beyond landmark flags.
* **Sudowoodo:** `BattleFrontier_OutsideEast` (54,62) blocks a path. Talking to it just makes it shake. Using the **Wailmer Pail** on it (`..._WaterSudowoodo`): "The weird tree doesn't like the Wailmer Pail! It attacked!" (`gText_Sudowoodo_Attacked`) → `setwildbattle SPECIES_SUDOWOODO, 40` → `FLAG_DEFEATED_SUDOWOODO` whatever the result. Removed if caught or beaten.
* **HC:** Scott's hint that there are wild Pokémon in the Frontier is true: a hidden cave of painter Pokémon and a fake tree that attacks when watered.

### D37 — Trainer Hill [O / S]
* **Maps:** `Route111` (entrance) → `TrainerHill_Entrance` → `TrainerHill_1F`–`4F`, `TrainerHill_Roof`, `TrainerHill_Elevator`.
* **Mechanics:** The reception trigger (9,6) pushes the player back with "We're still getting ready" (`..._Closed`) until `FLAG_SYS_GAME_CLEAR`. Afterwards: welcome → forced save → choose **Normal / Variety / Unique / Expert** (`MULTI_TAG_MATCH_TYPE`) → party healed → timed run up 4 floors of trainers (`trainerhill_*` script macros, `src/trainer_hill.c`) → the **Owner** (Gentleman, roof) gives a prize and comments on the time. The lobby has a nurse, a clerk (expanded mart), records and two NPCs. Floor trainers' names, parties and speeches are in `src/data/battle_frontier/trainer_hill.h` (names are Japanese placeholders; speeches are **Easy Chat word arrays**, `EC_WORD_*`).
* **Text labels:** `TrainerHill_Entrance_Text_*` (WelcomeToTrainerHill, TrainersUpToFloorX, TrainersInEveryRoom, LikeToChallengeTrainers, ExplainTrainerHill, TimeProgessGetSetGo, PleaseVisitUsAgain, SaveGameBeforeEnter, StillGettingReady, HopeYouGiveItYourBest, ThankYouForPlaying); `TrainerHill_Roof_Text_YouFinallyCameBravo`, `..._HaveTheMostMarvelousGift`, `..._FullUpBeBackLaterForThis`.
* **HC:** A post-game time-attack tower run by an eccentric owner waiting on the roof.

### D38 — Other things the HoF unlocks or changes [O]
* **Safari Zone expansion** (`SafariZone_Southeast`/`Northeast`): construction workers removed, the expansion attendant says it's finished (`SafariZone_Southeast_Text_ExpansionIsFinished`). The new zones hold Johto species.
* **Lilycove Trainer Fan Club** (`LilycoveCity_PokemonTrainerFanClub`): `VAR_LILYCOVE_FAN_CLUB_STATE = 1` → on the next visit the player meets their first fans (`..._MeetFirstFans`). Fans come and go with play time, Drake wins, contests, Battle Tower and Secret Base battles (`TryGainNewFanFromCounter`). This feeds a TV show (`TryPutTrainerFanClubOnAir`).
* **Gym Leader rematches** (`src/gym_leader_rematch.c`): after game clear, leaders randomly become available and call through Match Call. Their rematch lines are in each Gym script (`..._PreRematch`, `..._RematchDefeat`, `..._PostRematch`, `..._RematchNeedTwoMons`; rematches are double battles).
* **Elite Four & Champion**: can be re-challenged with the same flow (the flags are reset at HoF). Each new HoF entry is saved and viewable from any PC ("HALL OF FAME" option appears in the PC menu after game clear: `EventScript_AccessHallOfFame`).
* **Post-game Match Call lines** (`data/text/match_call.inc`, availability in `src/pokenav_match_call_data.c`): Mom3 ("Wear those Running Shoes until they fall apart"), Dad `MatchCall_Text_Norman_PreparingPostGame` ("Who would've thought… I won't be left behind!" → rematch), Steven7 ("Congratulations for entering the Hall of Fame… I hope we meet again"), May15/Brendan15, Scott7 ("out of the PokéNav's service area"), MrStone11, Wally7, plus the leaders' `_PostRematch` lines.
* **Ferry is back:** Capt. Stern at Slateport Harbor: "We've finished making the ferry" (`SlateportCity_Harbor_Text_FinishedMakingFerry`). He still trades the Scanner for a Deep Sea Tooth/Scale.
* **Sootopolis after badge 8:** the Cave of Origin Expert now blocks the entrance ("With the passing of the crisis, the cave, too, shall sleep"). Resident lines switch to their "PostLegendaries" / "GameClear" variants (e.g. `SootopolisCity_EventScript_Boy1GameClear`).
* **Gold Card:** the Pokémon Center nurse reacts specially when the Trainer Card has 4 stars (stars = HoF, Hoenn Dex complete, 5 Museum paintings from Master-rank contests, all Frontier gold symbols; `CountPlayerTrainerStars` in `src/trainer_card.c`): `gText_NoticesGoldCard`, then `gText_YouWantTheUsual` on later visits.

---

## 4. The S.S. Tidal / ferry system (mechanics for adding new destinations)

### 4.1 Who sails where, and what gates it

| Departure (map, NPC) | Gate checked by the script | Menu | Destinations (warp target) |
|---|---|---|---|
| `SlateportCity_Harbor`, ferry attendant (Beauty, (8,10)) | `FLAG_SYS_GAME_CLEAR` only (before: "ferry service unavailable"). **The S.S. Ticket item is not checked** (`..._NoTicket` is unused) | static: `MULTI_SSTIDAL_SLATEPORT_NO_BF` (Lilycove/Exit) or, with `FLAG_MET_SCOTT_ON_SS_TIDAL`, `MULTI_SSTIDAL_SLATEPORT_WITH_BF` (Lilycove/Battle Frontier/Exit) | **Lilycove** → `setvar VAR_SS_TIDAL_STATE, SS_TIDAL_BOARD_SLATEPORT`, warp `MAP_SS_TIDAL_CORRIDOR` (1,10) (interior voyage); **Battle Frontier** → warp `MAP_BATTLE_FRONTIER_OUTSIDE_WEST` (19,67) (instant) |
| `LilycoveCity_Harbor`, ferry attendant | `FLAG_SYS_GAME_CLEAR` only | **dynamic**: `special ScriptMenu_CreateLilycoveSSTidalMultichoice` + `GetLilycoveSSTidalSelection` (`src/script_menu.c`) | Slateport (interior voyage, `SS_TIDAL_BOARD_LILYCOVE`), Battle Frontier (if Scott met), Southern Island `MAP_SOUTHERN_ISLAND_EXTERIOR` (13,22), Navel Rock `MAP_NAVEL_ROCK_HARBOR` (8,4), Birth Island `MAP_BIRTH_ISLAND_HARBOR` (8,4), Faraway Island `MAP_FARAWAY_ISLAND_ENTRANCE` (13,38). Each island needs its key item **and** its `FLAG_ENABLE_SHIP_*` |
| `BattleFrontier_OutsideWest`, ferry attendant ((19,68)) | `checkitem ITEM_SS_TICKET` (the only place that checks the ticket) | static `MULTI_SSTIDAL_BATTLE_FRONTIER` | Slateport Harbor (8,11), Lilycove Harbor (8,11) (instant) |
| Island harbours (`SouthernIsland_Exterior`, `BirthIsland_Harbor`, `FarawayIsland_Entrance`, `NavelRock_Harbor`), sailor | none | yes/no | back to `MAP_LILYCOVE_CITY_HARBOR` (8,11) via `Common_EventScript_FerryDepartIsland` |

* **Departure animation:** `Common_EventScript_FerryDepart` (`data/event_scripts.s`). The attendant turns and hides, the player walks onto the boat and is hidden, and the boat object (`OBJ_EVENT_GFX_SS_TIDAL`, local ids `LOCALID_SLATEPORT_HARBOR_SS_TIDAL`, `LOCALID_LILYCOVE_HARBOR_SS_TIDAL`, `LOCALID_FRONTIER_SS_TIDAL`, island `…_SS_TIDAL`) moves right (`Movement_FerryDepart`). Then the `warp`.
* **The ship is visible** at Slateport/Lilycove only after HoF (`FLAG_HIDE_SLATEPORT_CITY_HARBOR_SS_TIDAL`, `FLAG_HIDE_LILYCOVE_HARBOR_SSTIDAL` cleared). Slateport's `OnTransition` also clears its flag whenever `FLAG_SYS_GAME_CLEAR` is set.
* **First-time ticket cutscenes** (Lilycove only): `FLAG_SHOWN_<TICKET>` unset → the attendant is replaced by a sailor (`LOCALID_LILYCOVE_HARBOR_FERRY_SAILOR`). For the Old Sea Map, Mr. Briney (`LOCALID_LILYCOVE_HARBOR_BRINEY`) appears instead. Several new tickets at once → `EventTicket_Text_OddTicketsWhereTo` + the first-time menu (`VAR_0x8004 = 1`).

### 4.2 The Slateport ↔ Lilycove voyage (interior)
* `VAR_SS_TIDAL_STATE` (`include/constants/field_specials.h`): `SS_TIDAL_BOARD_SLATEPORT` 1 → `DEPART_SLATEPORT` → `HALFWAY_LILYCOVE` → `LAND_LILYCOVE`; `BOARD_LILYCOVE` → `DEPART_LILYCOVE` → `HALFWAY_SLATEPORT` → `LAND_SLATEPORT`; plus `EXIT_CURRENTS_RIGHT/LEFT`.
* Boarding: `SSTidalCorridor` `OnFrame` turns BOARD into DEPART, plays a ding-dong announcement and sets cruise mode (`special SetSSTidalFlag` → `FLAG_SYS_CRUISE_MODE`, `VAR_CRUISE_STEP_COUNT = 0`).
* Progress: **walking** (`SS_TIDAL_MAX_STEPS` = 205 steps → `SSTidalCorridor_EventScript_ReachedStepCount`) or **sleeping in the cabin bed** (`SSTidalRooms_EventScript_Bed`: heals the party, `..._ProgessCruiseAfterBed` jumps to the next state). Each stage plays an announcement.
* Landing: the exit sailor (`LOCALID_SS_TIDAL_EXIT_SAILOR`, (1,11)) lets the player off only in a LAND state. He sets the respawn to `HEAL_LOCATION_LILYCOVE_CITY` / `…SLATEPORT_CITY` and warps to that harbour (8,11). Before that: "You can rest in cabin 2". The Snatch giver is hidden after the trip if the TM was taken.
* Location while aboard: the ship's map section is `MAPSEC_DYNAMIC`. `GetSSTidalLocation` (`src/field_specials.c`) maps the voyage state and step count onto Slateport / Route 131 / Lilycove / Route 124 / "currents" for the region map (`src/region_map.c`) and for the **porthole** (`special LookThroughPorthole`, `src/field_special_scene.c`, which shows the ship crossing the real route map).

### 4.3 How to add destinations (practical notes for the multi-region plan)
* **Slateport and Frontier menus** are static multichoice lists: add entries in `src/data/script_menu.h` (`MultichoiceList_SSTidalSlateportWithBF`, `…NoBF`, `…BattleFrontier`) and new `MULTI_*` constants (`include/constants/script_menu.h`), then new `case`s in `SlateportCity_Harbor_EventScript_ChooseDestination*` / `BattleFrontier_OutsideWest_EventScript_ChooseFerryDestination`.
* **Lilycove menu** is built in code: add a `SSTIDAL_SELECTION_<NEW>` constant before `SSTIDAL_SELECTION_EXIT` (currently 0–6, `SSTIDAL_SELECTION_COUNT` 7), the string to `sLilycoveSSTidalDestinations`, the condition (flag/item) to `CreateLilycoveSSTidalMultichoice`, update the scrollable-list size for `SCROLL_MULTI_SS_TIDAL_DESTINATION` in `src/field_specials.c` (`tNumItems = 7`) and add a `case` to `LilycoveCity_Harbor_EventScript_FerryRegularLocationSelect`. String names already present: `gText_SlateportCity`, `gText_LilycoveCity`, `gText_BattleFrontier`, `gText_SouthernIsland`, `gText_BirthIsland`, `gText_FarawayIsland`, `gText_NavelRock`, `gText_Exit` (`src/strings.c`).
* **Instant trips** (Frontier/islands) simply `warp` after `Common_EventScript_FerryDepart`. A new region's harbour can be added the same way. The arrival map needs its own sailor + boat object and a "sail back" script modelled on `SouthernIsland_Exterior_EventScript_Sailor`.
* **Interior voyages** reuse `SSTidalCorridor` and the `VAR_SS_TIDAL_STATE` machine. A third route needs new states, new `GetSSTidalLocation` cases and new exit-sailor branches.
* **Existing ferry model from FRLG:** the Seagallop ferry (`data/scripts/seagallop.inc`, `src/seagallop.c` spawn table keyed by `SEAGALLOP_*`, `special DoSeagallopFerryScene`, menus built in `src/script_menu.c`) is a ready-made animated sea-crossing between Vermilion and the Sevii Islands, gated by passes (`VAR_MAP_SCENE_*`). It is the natural template for Kanto connections. FRLG Vermilion also checks `FLAG_ENABLE_SHIP_NAVEL_ROCK` / `_BIRTH_ISLAND` (`VermilionCity_EventScript_CheckHasMysticTicket` / `…AuroraTicket`).
* **Story gate convention to keep:** every ferry destination in vanilla is gated by **game clear** (+ Scott for the Frontier, + key item and enable flag for the islands). New regions can follow the same pattern: ticket item + `FLAG_ENABLE_SHIP_<REGION>` + a first-time cutscene flag.

---

## 5. Battle Frontier facilities (system reference)

All facilities share the lobby pattern (`VAR_TEMP_CHALLENGE_STATUS` states: saving, paused, won, lost; a forced save before the challenge; level 50 or Open level via `FRONTIER_DATA_LVL_MODE`; Battle Points rewards; Frontier Pass records). Ordinary facility trainers come from `src/data/battle_frontier/battle_frontier_trainers.h` (300 entries). Their names are `_()` strings and their speeches are **Easy Chat word arrays** (`speechBefore/Win/Lose`), localised through the Easy Chat word list, not through `.inc` text.

| Facility | Maps | Format (from the guide's text) | Brain |
|---|---|---|---|
| Battle Tower | `BattleFrontier_BattleTowerLobby`, `…Elevator`, `…Corridor`, `…BattleRoom`, `…MultiPartnerRoom`, `…MultiCorridor`, `…MultiBattleRoom` | Single, Double, Multi (NPC partner) and Link Multi rooms; 7-battle rounds | Anabel |
| Battle Dome | `…BattleDomeLobby`, `…PreBattleRoom`, `…Corridor`, `…BattleRoom` | "Battle Tourney" brackets, Single/Double | Tucker |
| Battle Palace | `…BattlePalaceLobby`, `…Corridor`, `…BattleRoom` | Pokémon act on their own by Nature; Single/Double halls | Spenser |
| Battle Arena | `…BattleArenaLobby`, `…Corridor`, `…BattleRoom` | "Set KO Tourney": 3 vs 3 in a fixed order, judged after 3 turns | Greta |
| Battle Factory | `…BattleFactoryLobby`, `…PreBattleRoom`, `…BattleRoom` | "Battle Swap": rental Pokémon, swap after wins; Single/Double | Noland |
| Battle Pike | `…BattlePikeLobby`, `…Corridor`, `…ThreePathRoom`, `…RoomNormal`, `…RoomWildMons`, `…RoomFinal` | "Battle Choice": choose one of 3 doors (battles, healing, status, wild Pokémon) | Lucy |
| Battle Pyramid | `…BattlePyramidLobby`, `BattlePyramidSquare01–16` (floor layouts), `…BattlePyramidFloor`, `…BattlePyramidTop` | "Battle Quest": dark maze floors with trainers, wild Pokémon and a separate Pyramid Bag | Brandon |
| Ranking Hall | `BattleFrontier_RankingHall` | record boards | — |
| Exchange Service Corner | `BattleFrontier_ExchangeServiceCorner` | BP → items, vitamins, held items, dolls/decor (`src/data/battle_frontier/battle_frontier_exchange_corner.h`) | — |
| Lounges 1–9 | `BattleFrontier_Lounge1`…`9` | 1: IV-judging breeder; 2: "Frontier maniac" gossip on streaks; 3: gambler who takes BP bets on other players; 4/8: chat; 5: Nature girl (reads Natures); 6: in-game trade **Meowth "MEOWOW" for Skitty** (`INGAME_TRADE_MEOWTH`); 7: BP move tutors (e.g. Softboiled, Seismic Toss, Dream Eater, Mega Punch…); 9: empty room (no scripts) | — |
| Services | `BattleFrontier_PokemonCenter_1F/2F`, `BattleFrontier_Mart`, `BattleFrontier_ScottsHouse` | standard | — |

Also: the Battle Tower **Apprentice** system (`data/scripts/apprentice.inc`, `data/text/apprentice.inc`, `src/apprentice.c`, `src/data/battle_frontier/apprentice.h`) and the **Battle Tower reporter** (`FLAG_HIDE_BATTLE_TOWER_REPORTER`, TV interview). Both are record-mixing/TV features.

---

## 6. Optional systems and common texts (available throughout; mechanics only)

| System | Where | What it does mechanically | Main text sources |
|---|---|---|---|
| **Battle Tents** (mini-Frontier, pre-game) | `SlateportCity_BattleTent*` (Battle Factory rules, rentals), `VerdanturfTown_BattleTent*` (Palace rules), `FallarborTown_BattleTent*` (Arena "Set KO Tourney") | 3 battles in a row; prize items (`src/battle_tent.c`: Slateport Full Heal, Verdanturf Nest Ball, Fallarbor Hyper Potion); one-time TMs from lobby NPCs (Slateport `ITEM_TM_TORMENT`, Verdanturf `ITEM_TM_ATTRACT`); Scott cameos (`VAR_SCOTT_STATE`) | `data/text/battle_tent.inc`, lobby `scripts.inc`, `src/data/battle_frontier/battle_tent.h` (tent trainers: Easy Chat speeches) |
| **Contests** | `LilycoveCity_ContestLobby`, `ContestHall*` (Cool/Beauty/Cute/Smart/Tough), `LilycoveCity_LilycoveMuseum_2F` | Receptionist gives the **Pokéblock Case** on the first visit and silently adds `ITEM_CONTEST_PASS`; Normal/Super/Hyper/Master ranks × 5 categories; audience voting + appeal rounds; ribbons (`FLAG_SYS_RIBBON_GET`); Master-rank winners can have a **painting** hung in the museum (`FLAG_<CAT>_PAINTING_MADE`, a trainer-card star); Berry Blender NPCs | `data/scripts/contest_hall.inc`, `LilycoveCity_ContestLobby/scripts.inc`, `src/contest.c`, `src/contest_util.c`, `src/data/contest_text_tables.h`, `src/data/contest_moves.h` (appeal descriptions), `src/data/contest_opponents.h` (NPC names), `data/text/blend_master.inc` |
| **Secret Bases** | trees/rock walls/shrubs on routes; `SecretBase_*` maps (24 layouts) | TM Secret Power from the man on Route 111 (`data/scripts/secret_power_tm.inc`); enter/set base; decorate (`src/decoration.c`, shop in Fortree); record-mixed bases of other players with battles (`SecretBase_EventScript_BattleTrainer`) | `data/scripts/secret_base.inc`, `data/text/secret_base_trainers.inc` (167 lines, owner speeches by class), `src/secret_base.c` |
| **Pokémon Center** | every `*_PokemonCenter_1F` | `Common_EventScript_PkmnCenterNurse` (`data/scripts/pkmn_center_nurse.inc`): heal, Pokérus explanation (`FLAG_POKERUS_EXPLAINED`), Gold Card variant, Union Room/Trainer Hill checks; whiteout heal (`EventScript_AfterWhiteOutHeal`, `gText_FirstShouldRestoreMonsHealth`) | `data/text/pkmn_center_nurse.inc`, `src/strings.c` |
| **Poké Mart** | every `*_Mart`, League, Frontier | `pokemart` lists per map; greeting/farewell strings | `data/scripts/mart_clerk.inc` (`gText_HowMayIServeYou`, `gText_PleaseComeAgain`), `src/shop.c`, item names/descriptions `src/data/items.h` |
| **PC / Lanette's PC** | any PC | `EventScript_PC` (`data/scripts/pc.inc`): Player's PC (items/mailbox/decor), Pokémon Storage. The storage owner is shown as "SOMEONE'S PC" until **Lanette** (`Route114_LanettesHouse`) is met, which sets `FLAG_SYS_PC_LANETTE` (she also gives `DECOR_LOTAD_DOLL`); "HALL OF FAME" entry after game clear. The FRLG build reuses the flag for Bill (`gText_AccessedBillsPC`) | `data/text/pc.inc`, `data/text/pc_transfer.inc`, `src/player_pc.c`, `src/pokemon_storage_system.c` |
| **Trade / Union Room / Cable Club** | `*_PokemonCenter_2F`, `TradeCenter`, `UnionRoom`, `BattleColosseum_2P/4P`, `RecordCorner` | Link battle/trade/record mixing attendants, Wireless Union Room, Mystery Gift man (`CableClub_EventScript_DistributeEonTicket`, `trywondercardscript`) | `data/scripts/cable_club.inc`, `data/text/cable_club.inc`, `src/union_room.c`, `src/union_room_chat.c`, `src/data/union_room.h`, `src/trade.c`, `data/text/record_mix.inc` |
| **In-game trades (Hoenn)** | Rustboro House 1 (Seedot "DOTS" for Ralts), Fortree House 1 (Plusle "PLUSES" for Volbeat), Pacifidlog House 3 (Horsea "SEASOR" for Bagon), Frontier Lounge 6 (Meowth "MEOWOW" for Skitty) | `ingame_trade INGAME_TRADE_*` | nicknames/OT names in `src/data/trade.h` |
| **Day-Care** | `Route117` (old man outside), `Route117_PokemonDayCare` (old woman) | deposit 2 Pokémon, levelling, compatibility, Egg pickup, fees | `data/scripts/day_care.inc`, `src/daycare.c` |
| **Fan clubs** | `SlateportCity_PokemonFanClub`: chairman rates the lead Pokémon's contest condition and gives the 5 **scarves**; a member gives the **Soothe Bell** for high friendship. `LilycoveCity_PokemonTrainerFanClub`: the player's fan club (post-game, see D38) | — | map scripts; `src/field_specials.c` (fan club logic) |
| **Name Rater** | `SlateportCity_NameRatersHouse` | rename Pokémon whose OT is the player | map script |
| **Move Relearner / Move Deleter / Friendship Rater** | `FallarborTown_MoveRelearnersHouse` (Heart Scale), `LilycoveCity_MoveDeletersHouse`, `VerdanturfTown_FriendshipRatersHouse` | services | map scripts, `data/text/move_relearner.inc` |
| **TV / record mixing / Gabby & Ty / Mauville man / Lilycove lady / interviews / Pokémon News** | everywhere | dynamic shows built from player actions | `data/scripts/tv.inc`, `data/text/tv.inc`, `src/tv.c`, `data/scripts/gabby_and_ty.inc`, `data/scripts/mauville_man.inc` + `data/text/mauville_man.inc`, `data/scripts/lilycove_lady.inc`, `data/scripts/interview.inc`, `data/text/pokemon_news.inc` |
| **Match Call** | PokéNav | all phone texts | `data/text/match_call.inc`, `src/data/text/match_call_messages.h`, `src/pokenav_match_call_data.c` |
| **Sootopolis side houses** | `SootopolisCity_House1` (TM Brick Break), `House6` (`DECOR_WAILMER_DOLL`), `LotadAndSeedotHouse` (size contest → Elixir), `MysteryEventsHouse_1F/B1F` (e-Reader trainer battles), `Mart`; Kiri gives a daily berry (`FLAG_DAILY_SOOTOPOLIS_RECEIVED_BERRY`) | — | map scripts |
| **Altering Cave** | `AlteringCave` (Route 103) | species change only via Mystery Event (`data/scripts/gift_altering_cave.inc`) | — |

---

## 7. Characters in this segment

| Original name | Sprite / trainer pic | Role in the original | Must remain (mechanically) |
|---|---|---|---|
| **Wallace** | `OBJ_EVENT_GFX_WALLACE`; `TRAINER_WALLACE` (Champion Wallace) | Former Sootopolis Gym Leader, Juan's pupil, **Champion**; gives HM Waterfall; escorts the player into the HoF | Same person in Sootopolis (D01), the Champion's room and the HoF |
| **Juan** | Leader Juan pic; `TRAINER_JUAN_1`–`_5` | 8th Gym Leader (Water), Wallace's mentor | Badge 8, TM Water Pulse, Match Call, double rematches |
| **Wally** | `OBJ_EVENT_GFX_WALLY`; `TRAINER_WALLY_VR_1`, `_VR_2`… | Former sick boy, now a rival | Ambush in Victory Road; exit-side rematches post-game |
| **Sidney / Phoebe / Glacia / Drake** | `OBJ_EVENT_GFX_SIDNEY/PHOEBE/GLACIA/DRAKE`; Elite Four pics | Elite Four (Dark / Ghost / Ice / Dragon) | Fixed order, sealed rooms |
| **Rival (May / Brendan)** | `OBJ_EVENT_GFX_VAR_0` (May/Brendan normal) | Prof. Birch's child | Bursts into the Champion's room; National Dex scene; helps in the lab post-game |
| **Prof. Birch** | `OBJ_EVENT_GFX_PROF_BIRCH` | Professor | Rates the Dex in the Champion's room; National Dex; Johto starter |
| **Dad (Norman)** | Norman sprite in the player's house | Petalburg Gym Leader | Brings the S.S. Ticket, leaves for his Gym |
| **Mom** | Mom sprite | — | TV scene, red/blue question |
| **Scott** | `OBJ_EVENT_GFX_SCOTT` | Trainer scout, founder of the Battle Frontier | Ever Grande farewell, PokéNav call, S.S. Tidal invite, reception gate, his house (BP) |
| **Steven Stone** | `OBJ_EVENT_GFX_STEVEN`; `TRAINER_STEVEN` | Devon heir, Stone collector, ally | Letter + Beldum (Mossdeep); battle in Meteor Falls |
| **Maxie / Archie** | villain-leader sprites | Leaders of Magma / Aqua (→ our new villain organisation) | Leave Sootopolis after both are talked to; silent orb return at Mt. Pyre |
| **Mt. Pyre old couple** | Old man / old woman | Orb keepers, legend tellers | New legend; orbs returned |
| **Mr. Briney + Peeko** | `OBJ_EVENT_GFX_EXPERT_M` + Wingull | Old sailor from the prologue | On the S.S. Tidal; sails the player to Faraway Island |
| **Capt. Stern** | Scientist sprite | Ferry builder | Ferry finished; Scanner trade |
| **Frontier Brains**: Anabel, Tucker, Spenser, Greta, Noland, Lucy, Brandon | dedicated `OBJ_EVENT_GFX_*` and trainer pics | Facility heads | Streak-gated symbol battles |
| **Frontier staff** | `OBJ_EVENT_GFX_TEALA` (greeter/guide) and others | Reception | Frontier Pass |
| **Fossil Maniac** | Maniac | Collector | Desert Underpass hint |
| **Weather Institute scientist** | Scientist | Weather tracker | Reports the abnormal weather |
| **Legendaries** | Rayquaza, Groudon, Kyogre (static, Lv 70); Latias/Latios (roamer Lv 40 / island Lv 50); Deoxys, Mew (Lv 30); Lugia, Ho-Oh (Lv 70); Sudowoodo (Lv 40) | — | Encounter scripts as described |
| **Player** | new sprite (red spiky hair, black modern clothes) | — | Replace: overworld/bike/surf sprites, **HoF trainer pic**, **credits bike sprite** |

---

## 8. Strings outside map scripts that belong to this segment

* `src/data/credits.h`: game title line, "Credits", all staff pages (`PAGE_*`). Hack credits go here.
* `src/strings.c`: `gText_WelcomeToHOF`, `gText_LeagueChamp`, `gText_HOFNumber`, `gText_HOFCorrupted`, `gText_PickCancel`, `gText_HallOfFame`, ferry destination names (`gText_SlateportCity`, `gText_LilycoveCity`, `gText_BattleFrontier`, `gText_SouthernIsland`, `gText_BirthIsland`, `gText_FarawayIsland`, `gText_NavelRock`, `gText_Exit`).
* `src/frontier_util.c`: Frontier Brain `lostTexts` / `wonTexts`.
* `src/data/battle_frontier/*.h`: Frontier, Trainer Hill and Battle Tent trainer names (Easy Chat speeches); Exchange Corner item lists.
* `src/data/script_menu.h`: multichoice labels (`MultichoiceList_TVLati` "RED"/"BLUE", the SS Tidal lists, `MULTI_TAG_MATCH_TYPE`, `SCROLL_MULTI_BF_RECEPTIONIST` …).
* `data/event_scripts.s`: `gText_LegendaryFlewAway` caller (`Common_EventScript_LegendaryFlewAway`).
* `data/text/`: `abnormal_weather.inc`, `event_ticket_1.inc`, `event_ticket_2.inc`, `match_call.inc`, `pokedex_rating.inc`, `pc.inc`, `pkmn_center_nurse.inc`, `cable_club.inc`, `battle_tent.inc`, `secret_base_trainers.inc`, `tv.inc`, `record_mix.inc`.
* `src/diploma.c` (Diploma screen), `src/trainer_card.c` (card/star strings), `src/hall_of_fame.c`.

---

## 9. Cross-segment hooks and notes for the multi-region expansion

* **Natural hinges** already present in the scripts: Birch's National Dex speech ("Pokémon from other regions are found in Hoenn"), the Johto starter gift, Steven's "where will you go from here?", the S.S. Ticket "for a ferry", Scott's "place I'd like to invite you to", the Safari Zone expansion (Johto species), and Navel Rock (Lugia/Ho-Oh).
* **Gate to reuse for new regions:** `FLAG_SYS_GAME_CLEAR` (vanilla post-game), or a new region-ticket item + `FLAG_ENABLE_SHIP_<X>` checked in `CreateLilycoveSSTidalMultichoice`, following the island pattern (§4.3).
* **Dormant content:** the Eon, Aurora and Mystic tickets and the Old Sea Map are never given in a normal game. If the hack wants Southern/Birth/Faraway/Navel to be playable, a script must give the item **and** set the matching `FLAG_ENABLE_SHIP_*` (see `data/scripts/gift_*.inc` for the exact pairs).
* **Respawning event legendaries:** at every HoF the "defeated" flags of Mew, Lati@s, Deoxys, Lugia and Ho-Oh are cleared (`EverGrandeCity_HallOfFame_EventScript_ResetDefeatedEventLegendaries`). Rayquaza, Groudon, Kyogre and Sudowoodo do **not** respawn.
* **Text-length warning:** `SootopolisCity_Gym_1F_Text_JuanDefeat` already overflows `gDisplayedStringBattle` in English. Italian is usually longer, so trim it.
* **Original text slip to fix:** Steven's Meteor Falls line says "Sootopolis Space Center" (the Space Center is in Mossdeep).
* **League badge check** only tests badge 6 (`FLAG_BADGE06_GET`). If new regions or badges change the order, the guard script must be updated.

---

## 10. Condensed hard-constraint checklist

1. Two villain leaders linger in the rescued city. Only after the player talks to **both** do they leave, and then the Gym opens.
2. A future Champion stands **before the Gym door**, gives **HM Waterfall** as the region's thanks, and steps aside.
3. (Optional) The two leaders silently return the stolen artefacts at the mountain shrine. A storyteller tells the "new legend" of a sky Pokémon calming two titans.
4. (Optional) The sky dragon waits atop its tower for a one-time battle (Lv 70).
5. The 8th Gym is an **ice-floor puzzle** (floors break, the player falls to the lower floor). The Leader is the **Champion's mentor**, gives badge 8 + a Water TM, joins the PokéNav, and points to the League in the east.
6. The scout says goodbye in Ever Grande and **promises to call if the player becomes Champion** (missable).
7. The boy the player helped at the start **ambushes the player inside Victory Road**, credits the player, loses, and stays there.
8. Two guards block the inner League door and step aside after a badge check.
9. Four sealed rooms: the door shuts behind, four masters in fixed order, and a loss restarts everything.
10. The Champion is the man from point 2, recalls the crisis, and battles as soon as the player reaches him.
11. His proclamation is **interrupted by the rival** rushing in with useless advice. The professor arrives and checks the Dex. Only the player follows the Champion into the record room, and the rival waits outside.
12. The party is recorded on a machine; credits show the **player on a bicycle with the rival** and the caught Pokémon.
13. The player wakes at home. **Dad** delivers a **ferry ticket from the old sailor** and returns to his Gym. A **TV flash with a garbled colour** reports a flying Pokémon, and the player's red/blue answer decides which one roams.
14. The professor waits outside, reveals that **Pokémon from other regions** live in Hoenn, and upgrades the Dex.
15. Ten steps later the scout **calls** with an invitation reachable by ferry.
16. On the first voyage the scout is aboard, invites the player to his **Battle Frontier**, and leaves. Only then does the ferry offer the Frontier.
17. The Frontier staff treat the scout as their boss. He gives a memento whose value depends on how often the player met him, and later rewards the symbol sets.
18. Seven hand-picked bosses guard symbols behind win streaks.
19. The ally from the Space Center leaves a **farewell letter and his favourite Pokémon**, and waits for a final battle in a hidden cave of Meteor Falls that opens only after the Hall of Fame.
20. The professor gives **one of three foreign-region starters** for a complete regional Dex.
21. The two titans can be tracked through **reported droughts/downpours** and met once each in temporary caves.
22. The fossil lost with the sunken tower turns up in a newly collapsed cavern.
23. Ticket islands (dormant unless the hack hands out the tickets): the Eon Pokémon twin on a dream-themed island, a space Pokémon behind a moving triangle, the mythical Pokémon hiding in grass (sailor Briney repays his debt), and two legendary birds at the top and bottom of a rock island.
