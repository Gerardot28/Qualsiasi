# Segment A — Story skeleton: Prologue → 2nd badge (Hoenn)

Source: pokeemerald-expansion 1.17.1, read-only copy at `/home/user/pex-orig`.
Every path below is relative to that root. Map scripts are in `data/maps/<MAP>/scripts.inc`. Object and trigger placement is in `data/maps/<MAP>/map.json`.

This document is the **mechanical skeleton** that story writers must follow. Writers may change every line of text and every motivation. They may **not** change who appears where, who walks where, which battles happen, which items or Pokémon are given, or which flags and vars gate progress. The "Hard constraints" lines list what a re-interpretation must still explain on screen.

---

## 0. Conventions

* **Gender branch.** Almost every rival scene exists twice. The rival is always Prof. Birch's child of the *opposite* gender to the player:
  * Male player: home = `LittlerootTown_BrendansHouse_*`, rival = **MAY** in `LittlerootTown_MaysHouse_*`.
  * Female player: home = `LittlerootTown_MaysHouse_*`, rival = **BRENDAN** in `LittlerootTown_BrendansHouse_*`.
  * The new protagonist (red spiky hair, black modern clothes) replaces the player sprite. Both branches still exist in the scripts, so writers must provide text for **both** rivals.
* **Rival sprite.** `Common_EventScript_SetupRivalGfxId` (`data/scripts/rival_graphics.inc`) sets `VAR_OBJ_GFX_ID_0` to `OBJ_EVENT_GFX_RIVAL_MAY_NORMAL` or `OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL`. Objects with `OBJ_EVENT_GFX_VAR_0` are the rival.
* **Rival party.** Rival battles switch on `VAR_STARTER_MON` (0 = Treecko, 1 = Torchic, 2 = Mudkip). The rival always uses the starter that is strong against the player's.
* **Text macros.** `{PLAYER}`, `{RIVAL}`, `{KUN}` (honorific suffix, empty in English), `{STR_VAR_1}` (buffered name), `{REGION}`.
* **Label names.** Labels in `code` are the text labels to rewrite. Some are stored in another map's file; this is noted where it applies.
* **HC** = hard constraint for the re-interpretation.

---

## 1. Critical path at a glance (verified order)

The following is the order the game **forces**. A "→" step cannot happen before the previous one because a var or flag gates it. "∥" marks steps whose order is free.

1. New-game intro (Birch speech, gender, name) → truck (`InsideOfTruck`).
2. Littleroot: step off the truck → house → set the clock upstairs → TV report from Petalburg Gym → "go meet Prof. Birch next door".
3. Rival's house: the rival's mom → meet the rival (upstairs, or the rival comes downstairs) → `VAR_LITTLEROOT_TOWN_STATE = 1`.
4. The Littleroot twin lets you pass to Route 101 → **Birch rescue** (starter choice and first battle) → warp to the lab, where the starter is officially yours.
5. Oldale (Mart employee gives a Potion; the west exit is blocked) → Route 103: **first rival battle** → (optional: the rival in Oldale says "let's go back") → lab: **Pokédex + 5 Poké Balls** → `FLAG_ADVENTURE_STARTED`.
6. Leaving Littleroot: Mom gives the **Running Shoes**. The Oldale west exit is now open.
7. Route 102 → Petalburg: the gym boy drags you into the Gym → **Norman meets the player, Wally asks for help** → **Wally's catch tutorial** → back in the Gym, Norman says "go to Rustboro". The route west is now open.
8. Petalburg west exit: **Scott** cameo → Route 104 (south) → **Petalburg Woods: Aqua grunt vs Devon researcher** → Route 104 (north) → Rustboro.
9. Rustboro: **Roxanne (badge 1)** → leaving the Gym: **Devon Goods stolen** → the Devon employee asks for help → Route 116: **Mr. Briney lost Peeko** → **Rusturf Tunnel: grunt battle, goods recovered, Peeko freed, Briney introduces himself**.
10. Rustboro: the goods are returned → **Devon 3F: Mr. Stone gives the LETTER (for Steven) and the PokéNav**, and asks you to deliver the goods to Slateport → outside: **Match Call upgrade** → **rival encounter (Match Call registration and optional battle)**, either at Rustboro's south exit or outside Briney's cottage.
11. Route 104: **Mr. Briney's cottage** → **boat trip to Dewford** (Dad calls through the PokéNav on the way).
12. Dewford: ( **Granite Cave → Steven gets the LETTER** ) ∥ ( **Dewford Gym → Brawly, badge 2** ).
13. Exit of the segment: once the letter is delivered, Briney offers **Slateport** (Segment B). Optional: Mr. Stone gives the Exp. Share.

The task brief lists Roxanne *after* Rusturf Tunnel. The scripts prove the opposite. The theft triggers only on `VAR_RUSTBORO_CITY_STATE == 1`, which is set **only** by Roxanne's defeat script (`RustboroCity_Gym_EventScript_RoxanneDefeated`). The theft, Route 116 and Rusturf Tunnel therefore all come **after** badge 1.

---

## 2. State variables and flags (reference)

| Var / flag | Values in this segment | Set by | Meaning |
|---|---|---|---|
| `VAR_LITTLEROOT_INTRO_STATE` | 1/2 in truck (M/F) → 3 entered house → 4 told to set clock → 5 went upstairs → 6 clock set → 7 watched the TV, told to meet Birch | `InsideOfTruck`, `LittlerootTown`, `data/scripts/players_house.inc` | Intro sequence |
| `VAR_LITTLEROOT_HOUSES_STATE_MAY` / `_BRENDAN` | 1 on truck (that gender) → 2 met rival's mom | truck, rival's house 1F | Rival-mom greeting |
| `VAR_LITTLEROOT_RIVAL_STATE` | 2 entered rival's bedroom → 3 met rival → 4 received Pokédex | rival 2F/1F, lab | Rival intro |
| `VAR_LITTLEROOT_TOWN_STATE` | 0 → 1 met rival → 2 twin sent you → 3 got Pokédex (Mom waits outside) → 4 got Running Shoes | rival house, `LittlerootTown`, lab | Littleroot gates |
| `VAR_ROUTE101_STATE` | 0 → 1 entered → 2 rescue running (exits blocked) → 3 done | `Route101` | Birch rescue |
| `VAR_BIRCH_LAB_STATE` | 2 chose starter → 3 starter confirmed → 4 beat rival on Route 103 → 5 got Pokédex | Route101, lab, Route103 | Lab scenes |
| `VAR_OLDALE_RIVAL_STATE` | 1 rival waiting in Oldale → 2 gone | Route103, Oldale/Littleroot | Optional "let's go back" |
| `VAR_OLDALE_TOWN_STATE` | 0 west path blocked → 1 open | lab (Pokédex) | Gate to Route 102 |
| `FLAG_SYS_POKEMON_GET`, `FLAG_RESCUED_BIRCH` | set | Route101 bag | Has a Pokémon |
| `FLAG_ADVENTURE_STARTED` | set | lab (Pokédex) | Opens Oldale west; footprints man moves |
| `FLAG_SYS_B_DASH`, `FLAG_RECEIVED_RUNNING_SHOES` | set | Littleroot Mom | Running |
| `VAR_PETALBURG_CITY_STATE` | 0 gym boy intercepts → 2 Wally tutorial pending → 3 done (4–5 belong to a later segment: Wally's dad, HM Surf) | Petalburg Gym, Petalburg City | Gate to Route 104 |
| `VAR_PETALBURG_GYM_STATE` | 0 → 1 Wally tutorial → 2 back in Gym → +1 per badge (3 after Roxanne, 4 after Brawly; 6 = Norman battle ready) | Petalburg Gym, each Gym leader script | Norman's dialogue and the Norman fight unlock |
| `VAR_SCOTT_PETALBURG_ENCOUNTER` / `VAR_SCOTT_STATE` | 0 → 1 / +1 per meeting | Petalburg City, Rustboro School | Scott cameos |
| `VAR_PETALBURG_WOODS_STATE` | 0 → 1 | Petalburg Woods | Woods ambush done |
| `FLAG_BADGE01_GET`, `FLAG_DEFEATED_RUSTBORO_GYM` | set | Rustboro Gym | Badge 1 |
| `VAR_RUSTBORO_CITY_STATE` | 1 badge 1 → 2 goods stolen → 3 employee asked for help → 4 goods recovered → 5 goods returned → 6 met Mr. Stone → 7 Match Call added → 8 rival met | Gym, Rustboro, Rusturf, Devon 3F, Route104, Briney house | Main Rustboro chain |
| `FLAG_DEVON_GOODS_STOLEN` / `FLAG_RECOVERED_DEVON_GOODS` / `FLAG_RETURNED_DEVON_GOODS` | set, then cleared / set / set | Rustboro, Rusturf | Goods subplot |
| `VAR_RUSTURF_TUNNEL_STATE` | 2 grunt inside → 3 grunt backed up (4–5 belong to the later "tunnel cleared" event) | Rustboro, Rusturf | Tunnel |
| `VAR_ROUTE116_STATE` | 1 Briney waiting at the tunnel → 2 spoke to Briney | Rustboro, Route116 | Briney panic line |
| `VAR_BRINEY_HOUSE_STATE` | 1 Briney and Peeko back home | Rusturf | Cottage layout |
| `VAR_DEVON_CORP_3F_STATE` | 0 → 1 | Devon 3F | Mr. Stone scene done |
| `FLAG_RECEIVED_POKENAV` / `FLAG_SYS_POKENAV_GET` | set | Devon 3F | PokéNav |
| `FLAG_HAS_MATCH_CALL`, `FLAG_ADDED_MATCH_CALL_TO_POKENAV` | set | Rustboro scientist | Match Call system on |
| `VAR_ROUTE104_STATE` | 1 rival may meet you at Briney's cottage → 2 rival met | Rustboro, Route104, Briney house | Rival fallback location |
| `FLAG_MET_RIVAL_RUSTBORO`, `FLAG_DEFEATED_RIVAL_RUSTBORO`, `FLAG_DEFEATED_RIVAL_ROUTE_104`, `FLAG_REGISTER_RIVAL_POKENAV`, `FLAG_ENABLE_RIVAL_MATCH_CALL` | set | Rustboro / Route104 | Rival encounter 2 |
| `VAR_BOARD_BRINEY_BOAT_STATE` | 1 boarding at Route 104 → 0 arrived (2 = sailing back to Petalburg) | Briney house, Route104, Dewford | Boat cutscene |
| `FLAG_ENABLE_NORMAN_MATCH_CALL` | set | boat cutscene (Dad's call) | Dad registered |
| `VAR_BRINEY_LOCATION` | 1 = house, then copied from `VAR_0x8008` (Dewford) | Devon 3F, boat scripts | Where Briney stands |
| `FLAG_DELIVERED_STEVEN_LETTER`, `FLAG_REGISTERED_STEVEN_POKENAV` | set | Steven's room | Letter delivered; unlocks the Slateport sail |
| `FLAG_BADGE02_GET`, `FLAG_DEFEATED_DEWFORD_GYM` | set | Dewford Gym | Badge 2 |

---

## 3. Beats

### A01 — New-game intro: Prof. Birch's speech
* **Maps:** none (code). `src/main_menu.c` runs the `Task_NewGameBirchSpeech_*` tasks; the text is in `data/text/birch_speech.inc`.
* **Mechanics:** Birch's picture fades in on a platform. A **Lotad** is released from a Poké Ball as the example Pokémon. The player chooses Boy/Girl (sprites slide in and out), names the character on the naming screen and confirms. The player sprite then shrinks away and the game starts in the truck.
* **Original gist:** "Welcome to the world of Pokémon! My name is Birch, people call me the Pokémon Professor… This is what we call a Pokémon… Are you a boy or a girl? What's your name? You're {PLAYER} who's moving to my hometown of Littleroot… Come see me in my Pokémon Lab."
* **Characters:** Prof. Birch (intro picture), Lotad.
* **Text labels:** `gText_Birch_Welcome`, `gText_Birch_Pokemon`, `gText_Birch_MainSpeech`, `gText_Birch_AndYouAre`, `gText_Birch_BoyOrGirl`, `gText_Birch_WhatsYourName`, `gText_Birch_SoItsPlayer`, `gText_Birch_YourePlayer`, `gText_Birch_AreYouReady`.
* **HC:** The narrator is the professor (Birch sprite) and shows a Lotad. Gender choice and naming happen here. The player is "moving to Littleroot" today.

### A02 — Inside the moving truck
* **Maps:** `InsideOfTruck`.
* **Mechanics:** The truck shakes (`STEP_CB_TRUCK`). `InsideOfTruck_EventScript_SetIntroFlags` sets the respawn point to the player's bedroom, `VAR_LITTLEROOT_INTRO_STATE` = 1 (male) or 2 (female), hides the unused house's Mom and truck and the rival family in the player's house, sets `VAR_LITTLEROOT_HOUSES_STATE_<own gender>` = 1, and sets the dynamic warp to Littleroot. The player can read the moving boxes. When the door opens, the player walks out.
* **Original gist:** Box: "printed with a Pokémon logo… a Pokémon brand moving and delivery service."
* **Text labels:** `InsideOfTruck_Text_BoxPrintedWithMonLogo`.
* **HC:** The story opens with the player riding in the back of a moving truck with boxes.

### A03 — Arrival in Littleroot; Mom greets you
* **Maps:** `LittlerootTown`.
* **Mechanics:** `OnFrame`, INTRO_STATE 1/2. The player jumps off the truck. Mom walks out of the house door, approaches and speaks. Both walk into the house. INTRO_STATE = 3. This also unhides the fat man NPC.
* **Original gist:** Mom: "We're here, honey! Must be tiring riding with our things in the truck. This is Littleroot Town, our new home… you get your own room! Let's go inside."
* **Characters:** Mom (`OBJ_EVENT_GFX_MOM`).
* **Text labels:** `LittlerootTown_Text_OurNewHomeLetsGoInside`.
* **HC:** Mother and child move into a new house next door to the professor's house. The truck is parked outside.

### A04 — The house: Vigoroth movers, "go see your room and set the clock"
* **Maps:** `LittlerootTown_BrendansHouse_1F` (male) / `LittlerootTown_MaysHouse_1F` (female). Logic is shared in `data/scripts/players_house.inc`.
* **Mechanics:** `OnFrame`, INTRO_STATE 3 → `PlayersHouse_1F_EventScript_EnterHouseMovingIn`: Mom speaks, INTRO_STATE = 4. Two **Vigoroth** objects (one carrying a box) are in the room; talking to them plays the cry and a text. If the player tries to leave by the door at INTRO_STATE 4, Mom stops them (`..._GoSeeRoom`, coord on the door tile). Moving boxes are drawn as metatiles while INTRO_STATE < 6.
* **Original gist:** Mom: "Isn't it nice in here? The mover's Pokémon do all the work… Your room is upstairs. DAD bought you a new clock to mark our move — don't forget to set it!" Vigoroth: "Fugiiiiih!" / "Huggoh, uggo uggo…"
* **Characters:** Mom, 2 Vigoroth movers (`OBJ_EVENT_GFX_VIGOROTH_CARRYING_BOX`, `OBJ_EVENT_GFX_VIGOROTH_FACING_AWAY`).
* **Text labels:** `PlayersHouse_1F_Text_IsntItNiceInHere`, `PlayersHouse_1F_Text_MoversPokemonGoSetClock`, `PlayersHouse_1F_Text_ArentYouInterestedInRoom`, `PlayersHouse_1F_Text_Vigoroth1/2`.
* **HC:** Pokémon (Vigoroth) are moving furniture. The clock is a gift from the absent father.

### A05 — Bedroom: setting the wall clock
* **Maps:** `LittlerootTown_<Own>House_2F`, `..._1F`.
* **Mechanics:** Entering 2F with INTRO_STATE 4 sets it to 5. At 5, going back downstairs makes Mom push you back up (`..._GoUpstairsToSetClock`). Interacting with the wall clock (`PlayersHouse_2F_EventScript_WallClock`) opens the clock-setting UI (`StartWallClock`; this sets the RTC). It then sets INTRO_STATE = 6 and `FLAG_SET_WALL_CLOCK`, and hides both Vigoroths. Mom comes upstairs, speaks and leaves. Also in the room: the PC (`BedroomPC`), the notebook (tutorial), the GameCube and the region map.
* **Original gist:** "The clock is stopped… Better set it and start it!" Mom: "How do you like your new room? Everything's put away neatly… Pokémon movers are so convenient! Make sure everything's all there on your desk."
* **Text labels:** `PlayersHouse_1F_Text_GoSetTheClock`, `PlayersHouse_2F_Text_ClockIsStopped`, `PlayersHouse_2F_Text_HowDoYouLikeYourRoom`, `PlayersHouse_2F_Text_Notebook`, `PlayersHouse_2F_Text_ItsAGameCube`, `Common_Text_LookCloserAtMap`.
* **HC:** The player must set a clock in the bedroom. Mechanically this is the real-time clock setup.

### A06 — TV report from Petalburg Gym; "introduce yourself to Prof. Birch"
* **Maps:** own house 1F.
* **Mechanics:** `OnFrame`, INTRO_STATE 6 → `PlayersHouse_1F_EventScript_PetalburgGymReport<Male|Female>`. Mom gets a "!" and calls you. The player walks to the TV, which shows an interviewer (`MUS_ENCOUNTER_INTERVIEWER`). The broadcast ends (`TurnOffTVScreen`). `FLAG_SYS_TV_HOME` is set and INTRO_STATE = 7. After this, Mom's idle lines change: "See you, honey!", then "Did you introduce yourself to Prof. Birch?", then she heals the party after the rescue (see A16 and §5).
* **Original gist:** Mom: "Quick! Come quickly! Look! It's Petalburg Gym! Maybe DAD will be on!" TV: "…We brought you this report from in front of Petalburg Gym." Mom: "Oh… it's over. I think DAD was on, but we missed him. One of DAD's friends, Prof. Birch, lives right next door — go introduce yourself."
* **Characters:** Mom, TV interviewer (text only).
* **Text labels:** `PlayersHouse_1F_Text_OhComeQuickly`, `PlayersHouse_1F_Text_MaybeDadWillBeOn`, `PlayersHouse_1F_Text_ReportFromPetalburgGym`, `PlayersHouse_1F_Text_ItsOverWeMissedHim`, `PlayersHouse_1F_Text_GoIntroduceYourselfNextDoor`, `PlayersHouse_1F_Text_SeeYouHoney`, `PlayersHouse_1F_Text_DidYouMeetProfBirch`.
* **HC:** A TV broadcast about the Petalburg Gym, tied to the player's father, is just missed. The next goal is the house next door.

### A07 — Rival's house: the rival's mother
* **Maps:** the *other* house's 1F (`LittlerootTown_MaysHouse_1F` for a male player).
* **Mechanics:** `OnFrame`, `VAR_LITTLEROOT_HOUSES_STATE_<rival-house>` == 1 → `..._YoureNewNeighbor`. The rival's mom (`OBJ_EVENT_GFX_WOMAN_4`) gets a "!", walks to the player and speaks. Sets `FLAG_MET_RIVAL_MOM` and HOUSES_STATE = 2. Also present: the rival's little sibling (`OBJ_EVENT_GFX_NINJA_BOY`; "Do you already have your own Pokémon?").
* **Original gist:** "Oh, you're {PLAYER}, our new next-door neighbor! We have a {son/daughter} about the same age… excited about making a new friend… upstairs, I think."
* **Rival-mom idle lines through the segment:** "Like child, like father… if he's not at his LAB, he's scrabbling about in grassy places" → (rival met) "too busy with Pokémon to notice you came" → (has Pokémon) "went out to Route 103" → (after the Route 103 battle) "go home every so often to let your mother know you're okay."
* **Text labels:** `RivalsHouse_1F_Text_OhYoureTheNewNeighbor`, `..._LikeChildLikeFather`, `..._TooBusyToNoticeVisit`, `..._WentOutToRoute103`, `..._ShouldGoHomeEverySoOften`, `..._DoYouHavePokemon`. They are stored in `LittlerootTown_MaysHouse_1F/scripts.inc` and shared by both houses.
* **HC:** The professor's family lives next door: a mother, a small sibling and the rival. The professor himself is never at home.

### A08 — Meeting the rival
* **Maps:** rival's house 2F (or 1F).
* **Mechanics:** Entering the rival's 2F sets `VAR_LITTLEROOT_RIVAL_STATE` = 2. Two possible triggers:
  * (a) Interacting with the Poké Ball on the rival's floor (`..._RivalsPokeBall` → `..._MeetMay/MeetBrendan`). The rival walks in, gets a "!", rival encounter music plays, the rival talks, then walks to the PC.
  * (b) If the player goes back down first, coord triggers near the 1F stairs (`..._MeetRival0/1/2`). The rival enters at 1F (8,8), approaches, talks and goes upstairs.

  Both set RIVAL_STATE = 3 and `VAR_LITTLEROOT_TOWN_STATE` = 1, hide the floor Poké Ball and show the rival in the bedroom. Re-entering Littleroot then sets `FLAG_RIVAL_LEFT_FOR_ROUTE103`. Talking to the rival in the bedroom later: "Pokémon fully restored… items ready…"
* **Original gist:**
  * May: "Who… are you? Oh, you're {PLAYER}, your move was today. I'm May. I have this dream of becoming friends with Pokémon all over the world… I heard about you from my dad, Prof. Birch… Oh no, I forgot! I'm supposed to go help Dad catch some wild Pokémon!"
  * Brendan: "Who are you? Oh, you're {PLAYER}… I didn't know you're a girl. Dad said our new neighbor is a GYM LEADER's kid, so I assumed you'd be a guy. I'm Brendan… Don't you have a Pokémon? …Aw, I'm supposed to help my dad catch wild Pokémon."
* **Characters:** Rival (May/Brendan, `RIVAL_*_NORMAL` sprites).
* **Text labels:** `RivalsHouse_1F_Text_MayWhoAreYou` / `BrendanWhoAreYou`, `RivalsHouse_2F_Text_MayWhoAreYou` / `BrendanWhoAreYou`, `RivalsHouse_2F_Text_ItsRivalsPokeBall`, `RivalsHouse_2F_Text_MayGettingReady` / `BrendanGettingReady`. Later idle lines: `RivalsHouse_2F_Text_*JustCheckingMyPokedex`, `*WhereShouldIGoNext`.
* **HC:** The rival is the professor's child, owns Pokémon already, and leaves to help the father in the field. The player is "the Gym Leader's kid" (Brendan's line).

### A09 — The twin: "scary Pokémon outside, go see what's happening"
* **Maps:** `LittlerootTown`.
* **Mechanics:**
  * `VAR_LITTLEROOT_TOWN_STATE` 0: the twin (`OBJ_EVENT_GFX_TWIN`) stands guard at (7,2). Coord triggers at the north exit (10,1)/(11,1) make her run over and push the player back ("dangerous without Pokémon").
  * State 1 (rival met), Birch not rescued: she stands at (10,1). The coord at (11,1) makes her ask the player to check on the noise → state 2 and the path north opens.
  * After the rescue she congratulates; after the Pokédex she says "Good luck".
  * Other NPCs: Boy ("Birch spends days in his lab, then suddenly goes out in the wild"), Fat man ("PC can store items and Pokémon").
* **Original gist:** "Um, hi! There are scary Pokémon outside! I can hear their cries! …Can you go see what's happening for me?"
* **Text labels:** `LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut`, `..._DangerousIfYouDontHavePokemon`, `..._CanYouGoSeeWhatsHappening`, `..._YouSavedBirch`, `..._GoodLuckCatchingPokemon`, `..._BirchSpendsDaysInLab`, `..._CanUsePCToStoreItems`, plus the signs `..._TownSign`, `..._ProfBirchsLab`, `..._PlayersHouse`, `..._ProfBirchsHouse`.
* **HC:** A child blocks the exit until the player has met the rival. Then she sends the player to investigate cries on Route 101.

### A10 — Route 101: Birch rescue and starter choice
* **Maps:** `Route101`. Code: `src/battle_setup.c` `ChooseStarter` / `CB2_GiveStarter`, `src/starter_choose.c`, `src/battle_controllers.c`.
* **Mechanics:** Entering sets ROUTE101_STATE 1 and hides the map name. A coord at the south entrance (10/11,19) starts `Route101_EventScript_StartBirchRescue`: music `MUS_HELP`, then **Birch is chased in circles by a Zigzagoon** (`OBJ_EVENT_GFX_ZIGZAGOON_1`). ROUTE101_STATE = 2 and the exits are blocked (coords → "Don't leave me!"). Interacting with **Birch's bag** (`OBJ_EVENT_GFX_BIRCHS_BAG`) runs `Route101_EventScript_BirchsBag`:
  * sets `FLAG_SYS_POKEMON_GET` and `FLAG_RESCUED_BIRCH`;
  * starter screen ("Prof. Birch is in trouble! Release a Pokémon and rescue him!"): Treecko / Torchic / Mudkip, Lv 5;
  * first battle (`BATTLE_TYPE_FIRST_BATTLE`) vs wild **Zigzagoon Lv 2**; party healed afterwards;
  * Birch speaks, then warps everyone to the lab. Sets `VAR_BIRCH_LAB_STATE` = 2 and ROUTE101_STATE = 3, unhides Birch in the lab and hides the rival in the bedroom.
* **Original gist:** "H-help me!" / "Hello! You over there! Please! Help! In my BAG! There's a Poké Ball!" / after: "Whew… I was in the tall grass studying wild Pokémon when I was jumped. You saved me… Come by my Pokémon Lab later."
* **Characters:** Prof. Birch (`OBJ_EVENT_GFX_PROF_BIRCH`), wild Zigzagoon.
* **Text labels:** `Route101_Text_HelpMe`, `Route101_Text_PleaseHelp`, `Route101_Text_DontLeaveMe`, `Route101_Text_YouSavedMe`, `Route101_Text_TakeTiredPokemonToPokeCenter`, `Route101_Text_WildPokemonInTallGrass`, `Route101_Text_RouteSign`; in `src/strings.c`: `gText_BirchInTrouble`, `gText_ConfirmStarterChoice`.
* **HC:** The professor is cornered by a wild Zigzagoon. His dropped bag contains three Poké Balls (Treecko, Torchic, Mudkip). The player's first battle is vs Zigzagoon. The professor survives and the scene cuts to the lab.

### A11 — Lab: the starter is yours; "go see my kid"
* **Maps:** `LittlerootTown_ProfessorBirchsLab`.
* **Mechanics:** `OnFrame`, BIRCH_LAB_STATE 2 → `..._GiveStarterEvent`: fanfare "received the {starter}", optional nickname, then a YES/NO loop "go see {RIVAL}?" (NO → "Don't be that way" → asks again). BIRCH_LAB_STATE = 3, and the Route 101 boy appears.
  * Aide (`OBJ_EVENT_GFX_SCIENTIST_1`): before the rescue, "the Prof is away on fieldwork"; after, "studying the habitats and distribution of Pokémon… enjoys {RIVAL}'s help."
* **Original gist:** Birch: "I've heard so much about you from your father… the way you battled, you have your father's blood in your veins after all! As thanks for rescuing me, I'd like you to have the Pokémon you used… My kid, {RIVAL}, is also studying Pokémon while helping me out. Go see {RIVAL}? …Get {RIVAL} to teach you what it means to be a Trainer."
* **Text labels:** `LittlerootTown_ProfessorBirchsLab_Text_LikeYouToHavePokemon`, `..._WhyNotGiveNicknameToMon`, `..._MightBeGoodIdeaToGoSeeRival`, `..._GetRivalToTeachYou`, `..._DontBeThatWay`, `..._BirchAwayOnFieldwork`, `..._BirchIsntOneForDeskWork`, `..._BirchEnjoysRivalsHelpToo`, `..._BirchRivalGoneHome`, plus the signs `..._SeriousLookingMachine`, `..._PCUsedForResearch`, `..._CrammedWithBooksOnPokemon`, `..._BookTooHardToRead`.
* **HC:** The professor gifts the starter as thanks and sends the player north to find the rival.

### A12 — Oldale Town (first visit)
* **Maps:** `OldaleTown`, `OldaleTown_PokemonCenter_1F`, `OldaleTown_Mart`.
* **Mechanics:**
  * The Mart employee (`OBJ_EVENT_GFX_MART_EMPLOYEE`) walks the player to the Mart (music `MUS_FOLLOW_ME`) and gives a **Potion** (`FLAG_RECEIVED_POTION_OLDALE`).
  * The **footprints man** (`OBJ_EVENT_GFX_MANIAC`) blocks the west exit to Route 102. While `FLAG_ADVENTURE_STARTED` is unset he is placed at (1,11), and a coord at (0,10) with `VAR_OLDALE_TOWN_STATE` 0 pushes the player back. After the Pokédex he moves.
  * The Mart sells the basic list until `FLAG_ADVENTURE_STARTED`, then the expanded list.
* **Original gist:** Mart employee: "I work at a Pokémon Mart… look for our blue roof… here's a promotional item." Footprints man: "Wait! Don't come in here! I just discovered the footprints of a rare Pokémon! Wait until I finish sketching them" → later "…they were only my own footprints."
* **Text labels:** `OldaleTown_Text_IWorkAtPokemonMart`, `..._ThisIsAPokemonMart`, `..._PotionExplanation`, `..._WaitDontComeInHere`, `..._DiscoveredFootprints`, `..._FinishedSketchingFootprints`, `..._SavingMyProgress`, `..._TownSign`.
* **HC:** A man blocks the westward path until the player has the Pokédex. A shop employee escorts the player and gives a Potion.

### A13 — Route 103: first rival battle
* **Maps:** `Route103`.
* **Mechanics:** The rival object stands at (10,3) facing the grass. **Talk-to trigger, not a coord trigger.** The rival speaks, rival music plays, the rival faces the player with "!", speaks again, then `trainerbattle_no_intro`:
  * `TRAINER_MAY_ROUTE_103_{TREECKO|TORCHIC|MUDKIP}` / `TRAINER_BRENDAN_ROUTE_103_*`: one Lv 5 counter-starter (Torchic vs Treecko, and so on).
  * After the battle the rival speaks and walks off south. Sets `VAR_BIRCH_LAB_STATE` = 4 (rival now in the lab), `FLAG_DEFEATED_RIVAL_ROUTE103`, `VAR_OLDALE_RIVAL_STATE` = 1 (rival appears in Oldale).
  * Other Route 103 trainers (Daisy, Amy & Liv, Andrew, Miguel, Marcos, Rhett, Pete, Isabelle) sit across water or behind Cut trees and are reachable only later.
* **Original gist:** "Let's see… the Pokémon found on Route 103 include…" → "Oh, hi! My dad gave you a Pokémon as a gift. Let's have a quick battle! I'll give you a taste of what being a Trainer is like." → loss: "I think I know why my dad has an eye out for you. You just got that Pokémon, but it already likes you. You might be able to befriend any kind of Pokémon easily. Time to head back to the lab."
* **Text labels:** `Route103_Text_MayRoute103Pokemon`, `..._MayLetsBattle`, `..._MayDefeated`, `..._MayTimeToHeadBack`, and the Brendan equivalents. NPCs: `..._ShouldHaveBroughtPotion`, `..._ShortcutToOldale`, `..._RouteSign`.
* **HC:** The rival is doing field research on Route 103 and challenges the player. The rival's team is one Lv 5 starter. Afterwards the rival returns to the lab.

### A14 — Oldale: "Let's hurry home!" (optional)
* **Maps:** `OldaleTown`.
* **Mechanics:** With `VAR_OLDALE_RIVAL_STATE` 1 the rival stands at (11,19) on the south side. Coords (8–10,19), or talking to the rival, show one line, then the rival walks off and state = 2. If the player reaches Littleroot first, `LittlerootTown_OnTransition` silently sets state 2.
* **Original gist:** May: "Over here! Let's hurry home!" / Brendan: "I'm heading back to my dad's lab now. You should hustle back, too."
* **Text labels:** `OldaleTown_Text_MayLetsGoBack`, `OldaleTown_Text_BrendanLetsGoBack`.

### A15 — Lab: Pokédex and Poké Balls
* **Maps:** `LittlerootTown_ProfessorBirchsLab`.
* **Mechanics:** `OnFrame`, BIRCH_LAB_STATE 4 → the player walks in, then `..._GivePokedex`:
  * Birch gives the **Pokédex** (`FLAG_SYS_POKEDEX_GET`, `FLAG_RECEIVED_POKEDEX_FROM_BIRCH`);
  * the rival walks up and gives **5 Poké Balls**;
  * sets BIRCH_LAB_STATE 5, `FLAG_ADVENTURE_STARTED`, `VAR_OLDALE_TOWN_STATE` 1, `VAR_LITTLEROOT_RIVAL_STATE` 4, `VAR_LITTLEROOT_TOWN_STATE` 3 (Mom now waits outside).

  The rival stays in the lab ("Where should I go next…") until the Norman scene hides it (A18).
* **Original gist:** Birch: "I heard you beat {RIVAL} on your first try… {RIVAL} has an extensive history as a Trainer. I ordered this for my research, but you should have this Pokédex… it automatically records any Pokémon you meet or catch. My kid goes everywhere with it." May: "You got a Pokédex too! I've got something for you! …If I find any cute Pokémon, I'll catch them!" / Brendan: "Huh… so you got a Pokédex too. Well then, here… If I find any cool Pokémon…"
* **Text labels:** `..._HeardYouBeatRivalTakePokedex`, `..._ReceivedPokedex`, `..._ExplainPokedex`, `..._MayGotPokedexTooTakeThese`, `..._CatchCutePokemonWithPokeBalls`, `..._BrendanGotPokedexTooTakeThese`, `..._CatchCoolPokemonWithPokeBalls`, `..._OhYourBagsFull`, `..._HeyYourBagsFull`, `..._CountlessPokemonAwait`, `..._MayWhereShouldIGoNext`, `..._BrendanWhereShouldIGoNext`.
* **HC:** The professor hands over the Pokédex. The rival gives 5 Poké Balls. This is the official "adventure start".

### A16 — Mom: Running Shoes
* **Maps:** `LittlerootTown`.
* **Mechanics:** With `VAR_LITTLEROOT_TOWN_STATE` 3, Mom stands in front of the house. Coord triggers at **both** town exits (north (10,2)/(11,2), east row (8–11,9)), or talking to her: "Wait, {PLAYER}!", Mom walks to the player, gives the **Running Shoes** (`FLAG_RECEIVED_RUNNING_SHOES`, `FLAG_SYS_B_DASH`), walks back inside. State = 4. A Running Shoes manual then appears on the home table.
* **Original gist:** "Did you introduce yourself to Prof. Birch? Oh! What an adorable Pokémon! You're your father's child, all right… If you're going on an adventure, wear these Running Shoes… To think you have your very own Pokémon now… Your father will be overjoyed. …But please be careful. If anything happens, you can come home."
* **Text labels:** `LittlerootTown_Text_WaitPlayer`, `..._WearTheseRunningShoes`, `..._SwitchShoesWithRunningShoes`, `..._ExplainRunningShoes`, `..._ComeHomeIfAnythingHappens`, `PlayersHouse_1F_Text_RunningShoesManual`.
* **HC:** Mom gives the running item at the town exit. "You can always come home": from here on, Mom heals the party at home (`PlayersHouse_1F_EventScript_MomHealsParty`).

### A17 — Route 102
* **Maps:** `Route102`.
* **Mechanics:** First "real" trainers: `TRAINER_CALVIN_1` (registers in Match Call after `FLAG_HAS_MATCH_CALL`), `TRAINER_RICK`, `TRAINER_TIANA`, `TRAINER_ALLEN`. Flavor NPCs. The Wally tutorial texts (`Route102_Text_WatchMeCatchPokemon`, `..._WallyIDidIt`, `..._LetsGoBack`) are stored in this file but used in Petalburg City (A20).
* **HC:** None.

### A18 — Petalburg City: the gym boy, then Norman and Wally in the Gym
* **Maps:** `PetalburgCity`, `PetalburgCity_Gym`.
* **Mechanics:**
  1. While `VAR_PETALBURG_CITY_STATE` == 0, the gym boy (`OBJ_EVENT_GFX_BOY_2`) stands near the west entrance. Coords at x = 8 (rows 10–13) make him intercept the player ("!", music `MUS_FOLLOW_ME`) and **lead the player to the Gym door**. This repeats every time until the Wally tutorial, so it **blocks the way west (Route 104)**.
  2. Inside the Gym, with `VAR_PETALBURG_GYM_STATE` < 6, Norman (`OBJ_EVENT_GFX_NORMAN`) is moved to the entrance (4,107). **Talking to him** at GYM_STATE 0:
     * Norman greets the player.
     * **Wally** (`OBJ_EVENT_GFX_WALLY`) walks in and asks for a Pokémon.
     * Dialogue exchange; Norman tells the player to go with Wally.
     * Norman **loans Wally his Zigzagoon** and gives him a Poké Ball.
     * Wally asks "Would you really come with me?"
     * Wally and the player exit together (music `MUS_FOLLOW_ME`).
  3. Script effects: GYM_STATE = 1, CITY_STATE = 2, hide Wally's mom in the city, show Wally in the city and the Gym, **hide the rival in Birch's lab**, `InitBirchState`. From now on Birch rotates between the lab, Route 101 and Route 103 on a 7-day cycle (`src/time_events.c`) and rates the Pokédex. Then the player is warped to Petalburg City (15,8).
* **Original gist:**
  * Gym boy: "Are you a rookie Trainer? Trainers first check what kind of Gym is in the town… This is Petalburg City's Gym."
  * Dad: "Well, if it isn't {PLAYER}! So you're all finished moving in? …You're with your Pokémon. Then I guess you're going to become a Trainer like me. That's great news!"
  * Wally: "Um… I'd like to get a Pokémon, please…" Dad: "You're Wally, right?" Wally: "I'm going to stay with my relatives in Verdanturf Town. I thought I'd be lonely, so I wanted to take a Pokémon along. But I've never caught one…"
  * Dad: "{PLAYER}, go with Wally and make sure he safely catches a Pokémon. Wally, I'll loan you my Pokémon (Zigzagoon) … and a Poké Ball."
* **Characters:** Norman = the player's DAD and Petalburg Gym Leader. Wally (frail boy). Wally's mom (`OBJ_EVENT_GFX_WOMAN_4`, in the city: "Where has our Wally gone? We have to leave for Verdanturf Town very soon…").
* **Text labels:** `PetalburgCity_Text_AreYouRookieTrainer`, `..._ThisIsPetalburgGym`, `..._ThisIsGymSign`, `..._WhereIsWally`, `..._GymSign`, `..._CitySign`, `..._WallyHouseSign`; `PetalburgCity_Gym_Text_DadYoureHereWithYourPokemon`, `..._WallyIdLikeAPokemon`, `..._DadOhYoureWallyRight`, `..._WallyIveNeverCaughtAPokemon`, `..._DadHmISee`, `..._DadPlayerGoWithWally`, `..._IllLoanYouMyZigzagoon`, `..._WallyThankYouAndDadGivesPokeBall`, `..._WallyOhWowThankYou`, `..._WouldYouReallyComeWithMe`.
* **HC:** The Petalburg Gym Leader knows the player personally. The script and text call him "DAD", and he later visits the player's home and gives the S.S. Ticket. He refuses to fight now. A shy boy arrives asking for help catching a Pokémon before moving to Verdanturf. The Leader lends him a Zigzagoon and a Poké Ball and sends the player along.

### A19 — Wally's catch tutorial
* **Maps:** `PetalburgCity` (grass at the east side of town).
* **Mechanics:** `OnFrame`, CITY_STATE 2 → `PetalburgCity_EventScript_WallyTutorial`: `SavePlayerParty` and `LoadWallyZigzagoon` (Zigzagoon Lv 7). Wally walks into the grass with the player following. Then `StartWallyTutorialBattle`: Wally's Zigzagoon vs **wild male Ralts Lv 5**, scripted capture (`BATTLE_TYPE_CATCH_TUTORIAL`). After it: CITY_STATE = 3, `LoadPlayerParty`, GYM_STATE = 1, warp into the Gym.
* **Original gist:** Wally: "Pokémon hide in tall grass like this, don't they? Please watch me and see if I can catch one properly… Whoa!" → "I did it… It's my… my Pokémon!" → "Thank you! Let's go back to the Gym!"
* **Text labels:** `Route102_Text_WatchMeCatchPokemon`, `Route102_Text_WallyIDidIt`, `Route102_Text_LetsGoBack` (stored in `Route102/scripts.inc`). The in-battle tutorial strings live in the battle engine (`src/battle_*`).
* **HC:** Wally catches a Ralts with a borrowed Zigzagoon while the player watches. The player does not battle.

### A20 — Back in the Gym: Wally leaves, Norman sets the goal
* **Maps:** `PetalburgCity_Gym`.
* **Mechanics:** `OnFrame`, GYM_STATE 1 → Wally (moved to the entrance) returns the Zigzagoon, thanks the player and walks out (`FLAG_HIDE_PETALBURG_CITY_WALLY`). Norman speaks. GYM_STATE = 2. **The west exit of Petalburg is now open** (the CITY_STATE 0 coords no longer fire).
  * Norman's idle line by GYM_STATE: 2 = "go to Rustboro… I'll battle you when you can show me **four** Gym badges"; 3 (after Roxanne) = "go across the sea to Dewford, challenge **Brawly**"; 4 (after Brawly) = "you have gotten stronger."
  * The Gym guide and the Gym trainers are for a later segment.
* **Original gist:** Dad: "So, did it work out?" Wally: "Thank you, yes… here's your Pokémon back. {PLAYER}, thank you for coming along… I promise I'll take really good care of it. My mom's waiting, bye!" Dad: "If you want to become a strong Trainer, head for Rustboro City and challenge the Gym Leader Roxanne. Then go on to other Gyms and collect badges. Of course, I'm a Gym Leader too. We'll battle one day — but only after you become stronger."
* **Text labels:** `PetalburgCity_Gym_Text_DadSoDidItWorkOut`, `..._WallyThankYouBye`, `..._DadGoCollectBadges`, `..._NormanGoToRustboro`, `..._NormanGoToDewford`, `..._YouHaveGottenStronger`. Wally's house (optional visit): `PetalburgCity_WallysHouse_Text_ThanksForPlayingWithWally` ("frail and sickly since he was a baby… sent to relatives in Verdanturf"), `..._WallyWasReallyHappy`.
* **HC:** Norman names Rustboro and Roxanne as the next goal and sets "4 badges" as the condition for fighting him. Mechanically the Norman battle unlocks at GYM_STATE 6 = 4 badges. Wally leaves town.

### A21 — Scott at Petalburg's west exit
* **Maps:** `PetalburgCity`.
* **Mechanics:** Coords at x = 4 (rows 10–13) with `VAR_SCOTT_PETALBURG_ENCOUNTER` 0. These are only reachable after A20, because the gym-boy coords at x = 8 block the way before. Scott (`OBJ_EVENT_GFX_SCOTT`) walks in, gets a "!", talks and leaves. Sets `VAR_SCOTT_STATE` 1 and ENCOUNTER 1.
* **Original gist:** "Excuse me! From the way you're dressed, are you a Pokémon Trainer? …Well, maybe not. Your clothes aren't all that dirty… I'm roaming the land in search of talented Trainers."
* **Text labels:** `PetalburgCity_Text_AreYouATrainer`, `..._WellMaybeNot`, `..._ImLookingForTalentedTrainers`.
* **HC:** A talent scout briefly sizes up the player. He reappears in the Rustboro Trainer's School (A26) and later throughout the game, leading to the Battle Frontier.

### A22 — Route 104 (south) and Mr. Briney's empty cottage
* **Maps:** `Route104`, `Route104_MrBrineysHouse`.
* **Mechanics:** Beach route with trainers (`TRAINER_CINDY_1`, `TRAINER_DARIAN`, `TRAINER_BILLY`, …). The **cottage is empty**: Briney and Peeko are hidden by `FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY` / `_PEEKO`, set in `data/scripts/new_game.inc`. The boat is moored but inactive. NPC: "That seaside cottage is where Mr. Briney lives… once a mighty sailor."
* **HC:** The cottage and the boat exist from the start. The owner is absent.

### A23 — Petalburg Woods: Aqua grunt ambushes the Devon researcher
* **Maps:** `PetalburgWoods`.
* **Mechanics:** Coords (26,23)/(27,23) with `VAR_PETALBURG_WOODS_STATE` 0:
  * The **Devon researcher** (`OBJ_EVENT_GFX_MAN_2`) looks around, then walks up to the player.
  * Music `MUS_ENCOUNTER_AQUA`. The **Aqua grunt** (`OBJ_EVENT_GFX_AQUA_MEMBER_M`) jumps out, approaches the researcher and demands the papers.
  * The researcher hides behind the player. The grunt approaches the player → `trainerbattle_no_intro TRAINER_GRUNT_PETALBURG_WOODS` (Poochyena Lv 9).
  * The grunt backs off, speaks, **runs away north**.
  * The researcher gives a **Great Ball**, says "It's a crisis! I can't be wasting time!" and runs off north.
  * State = 1.

  Other content: `TRAINER_LYLE`, `TRAINER_JAMES_1` (bug catchers). A girl gives a Miracle Seed **after badge 1**. Two trainer-tips signs.
* **Original gist:**
  * Researcher: "Hmmm… not a one to be found… Have you seen any Pokémon called Shroomish? I really love that Pokémon."
  * Grunt: "I was going to ambush you, but you had to dawdle in Petalburg Woods forever… You! Devon researcher! Hand over those papers!"
  * Researcher: "You're a Trainer? You've got to help me!"
  * Grunt: "No one who crosses Team Aqua gets any mercy, not even a kid!" → after: "I'm out of Pokémon… we of Team Aqua are also after something in Rustboro. I'll let you go today!"
  * Researcher: "Thanks to you, he didn't rob me of these important papers… Didn't that thug say they were after something in Rustboro, too?"
* **Text labels:** `PetalburgWoods_Text_NotAOneToBeFound`, `..._HaveYouSeenShroomish`, `..._IWasGoingToAmbushYou`, `..._HandOverThosePapers`, `..._YouHaveToHelpMe`, `..._NoOneCrossesTeamAqua`, `..._YoureKiddingMe`, `..._YouveGotSomeNerve`, `..._ThatWasAwfullyClose`, `..._YoureLoadedWithItems`, `..._TeamAquaAfterSomethingInRustboro`, `..._ICantBeWastingTime`, plus the trainer and NPC lines.
* **Characters:** Devon researcher (recurring, see §4). Aqua grunt #1 (male Aqua grunt sprite and trainer pic, class "TEAM AQUA").
* **HC:** A villain-faction grunt in Aqua uniform tries to rob a corporate researcher of documents. He loses to the player, mentions a bigger target in Rustboro, and flees north. The researcher gives a Great Ball and hurries north. **This is the first on-screen appearance of the villain faction.**

### A24 — Route 104 (north) → Rustboro arrival
* **Maps:** `Route104`, `Route104_PrettyPetalFlowerShop`, `RustboroCity`.
* **Mechanics:** Flower shop (gives the **Wailmer Pail**; berry tutorial). Boy gives **TM Bullet Seed**. Woman gives a **Chesto Berry**. Trainers `TRAINER_HALEY_1`, `TRAINER_WINSTON_1`, `TRAINER_GINA_AND_MIA_1` (double), `TRAINER_IVAN`. In Rustboro, Devon Corp 1F has a stair guard blocking upstairs until `FLAG_RETURNED_DEVON_GOODS` ("only authorized people").
* **HC:** None story-wise. Rustboro is a corporate city with Devon HQ (west, warp (11/12,15)), the Gym (warp (27,19)) and the Trainer's School.

### A25 — Rustboro Gym: Roxanne (badge 1)
* **Maps:** `RustboroCity_Gym`.
* **Mechanics:** Rock-type Gym. Trainers `TRAINER_JOSH` (Geodude 10), `TRAINER_TOMMY` (2× Geodude 8), `TRAINER_MARC` (Hiker, 2× Geodude 8). Gym guide (Rock type, weak to Water and Grass). `TRAINER_ROXANNE_1`: Geodude 12, Geodude 12, Nosepass 15 @ Oran Berry; 2 Potions. On win:
  * **Stone Badge**, `FLAG_BADGE01_GET`, `FLAG_DEFEATED_RUSTBORO_GYM`;
  * **`VAR_RUSTBORO_CITY_STATE = 1`**, `VAR_PETALBURG_GYM_STATE += 1`;
  * Gym trainers set to defeated;
  * **TM39 Rock Tomb**.

  The statue lists the player after the win.
* **Original gist:** Roxanne: "I am Roxanne, the Rustboro Gym Leader. I became a Gym Leader so that I might apply what I learned at the Pokémon Trainer's School in battle…" → loss: "It seems I still have much more to learn… please accept the official Pokémon League Stone Badge." Badge: "raises Attack, enables Cut outside battle."
* **Text labels:** `RustboroCity_Gym_Text_RoxanneIntro`, `..._RoxanneDefeat`, `..._ReceivedStoneBadge`, `..._StoneBadgeInfoTakeThis`, `..._ExplainRockTomb`, `..._RoxannePostBattle`, `..._GymGuideAdvice`, `..._GymGuidePostVictory`, `..._GymStatue`, `..._GymStatueCertified`, trainer lines. `..._RoxanneRegisterCall` and `..._RegisteredRoxanne` are her later PokéNav call: `FLAG_ENABLE_ROXANNE_FIRST_CALL` is set by Brawly's script and the call comes after a number of steps.
* **HC:** Leader Roxanne (sprite and trainer pic) uses Rock Pokémon and is a Trainer's School graduate. Beating her is what triggers the theft.

### A26 — (Parallel, any time in Rustboro) Trainer's School, Cutter, trade
* **Maps:** `RustboroCity_PokemonSchool`, `RustboroCity_CuttersHouse`, `RustboroCity_House1`.
* **Mechanics:**
  * School: the teacher gives a **Quick Claw**. **Scott** is there: first meeting text depends on badge 1; sets `FLAG_MET_SCOTT_RUSTBORO` / `FLAG_MET_SCOTT_AFTER_OBTAINING_STONE_BADGE` and `VAR_SCOTT_STATE` +1. He is hidden once the PokéNav is received (`FLAG_HIDE_RUSTBORO_CITY_POKEMON_SCHOOL_SCOTT`).
  * Cutter gives **HM01 Cut** (usable with the Stone Badge).
  * House 1: in-game trade, your **Ralts** for **Seedot "DOTS"** (OT KOBE) (`INGAME_TRADE_SEEDOT`).
* **Original gist (Scott):** "Didn't we meet in Petalburg? My name's Scott. I travel in search of outstanding Trainers… Oh, a Stone Badge? Impressive, but I'd love to see you in battle."
* **HC:** Scott recurs. Cut is obtained here.

### A27 — Devon Goods stolen
* **Maps:** `RustboroCity`.
* **Mechanics:** Coords at x = 23 (rows 20–24, the street just west of the Gym door) with **`VAR_RUSTBORO_CITY_STATE` 1** → `RustboroCity_EventScript_StolenGoodsScene`:
  * "Get out! Out of the way!"; music `MUS_ENCOUNTER_AQUA`;
  * the **Aqua grunt** appears near Devon (13,21) and **runs east past the player, then north** (toward the Route 116 exit);
  * the **Devon researcher** chases him, shouting, then disappears north. He is respawned at (30,10), the north-east exit.

  Sets `FLAG_DEVON_GOODS_STOLEN`, RUSTBORO_STATE 2, `VAR_RUSTURF_TUNNEL_STATE` 2, `VAR_ROUTE116_STATE` 1. Shows Briney on Route 116 and Peeko plus the grunt in Rusturf Tunnel. Hides Briney and Peeko in the cottage.
  * NPC hints: the fat man ("a shady-looking guy went around the corner"); Devon 1F staff ("one of our research staff stupidly got robbed… not anything that anyone can use").
* **Original gist:** Grunt: "Get out! Out of the way!" Researcher: "Wait! Pleeeaaase! Don't take my GOODS!"
* **Text labels:** `RustboroCity_Text_OutOfTheWay`, `..._WaitDontTakeMyGoods`, `..._SneakyLookingManWentAroundCorner`; `RustboroCity_DevonCorp_1F_Text_StaffGotRobbed`, `..._RobberWasntVeryBright`, `..._HowCouldWeGetRobbed`.
* **HC:** Right after badge 1, a grunt runs past the player carrying stolen goods from the corporation, with the same researcher chasing him toward Route 116.

### A28 — The researcher asks for help
* **Maps:** `RustboroCity`.
* **Mechanics:** Coords near the north-east exit ((30,9), (29,10), (30,11), (30,12)) with STATE 2 → `..._EmployeeAskToGetGoods`: "!", the researcher faces or approaches the player. Sets `FLAG_INTERACTED_WITH_DEVON_EMPLOYEE_GOODS_STOLEN`, STATE 3. Talking to him again: "he took off towards the tunnel."
* **Original gist:** "It's you! The fantastic Trainer who helped me in Petalburg Woods! Help me! I was robbed by Team Aqua! I have to get the Devon Goods back! If I don't… I'm going to be in serious trouble."
* **Text labels:** `RustboroCity_Text_HelpMeIWasRobbed`, `RustboroCity_Text_ShadyCharacterTookOffTowardsTunnel`.
* **HC:** The researcher recognises the player from the Woods and points east (tunnel).

### A29 — Route 116: Mr. Briney has lost Peeko
* **Maps:** `Route116`, `Route116_TunnelersRestHouse`.
* **Mechanics:** Briney (`OBJ_EVENT_GFX_EXPERT_M`) stands at the Rusturf Tunnel entrance (46,9). A coord at (47,9) with ROUTE116_STATE 1, or talking to him, shows one line and sets ROUTE116_STATE 2. Wanda's boyfriend (`OBJ_EVENT_GFX_BLACK_BELT`) is outside the tunnel, with lines depending on the goods flags. Rest House: the tunnel project was stopped because it disturbed wild Pokémon. Trainers: `TRAINER_JOEY`, `JOSE`, `JERRY_1`, `CLARK`, `JANICE`, `KAREN_1`, `SARAH`, `DAWSON`, `DEVAN`, `JOHNSON` (some behind Cut trees).
* **Original gist:**
  * Briney: "Ohhh, what am I to do? We were on our walk, Peeko and I, when we were jumped by an odd thug… The scoundrel made off with my darling Peeko! Wrrrooooaaar! PEEKO!"
  * Boyfriend: "I was digging the tunnel without any tools when some goon ordered me out! That tunnel's filled with Pokémon that react badly to loud noises… I'm worried the goon will startle them into an uproar."
* **Text labels:** `Route116_Text_ScoundrelMadeOffWithPeeko`, `..._WantToDigTunnel`, `..._DiggingTunnelWhenGoonOrderedMeOut`, `..._GoonHightailedItOutOfTunnel`, `..._RusturfTunnelSign`, `..._TunnelersRestHouse`; `Route116_TunnelersRestHouse_Text_*`.
* **HC:** An old sailor's pet Wingull has been taken by the fleeing thief, who is hiding in an unfinished tunnel.

### A30 — Rusturf Tunnel: grunt battle, goods recovered, Peeko freed
* **Maps:** `RusturfTunnel`.
* **Mechanics:** The tunnel is a dead end: the far side is blocked by breakable rocks, and Wanda waits behind them. The grunt and Peeko (`OBJ_EVENT_GFX_WINGULL`) stand at (13,5)/(13,4).
  * Coords (9,4)/(9,5) with TUNNEL_STATE 2: "Come and get some!", the grunt and Peeko back up. TUNNEL_STATE = 3.
  * **Talking to the grunt**: music `MUS_ENCOUNTER_AQUA` → `TRAINER_GRUNT_RUSTURF_TUNNEL` (Poochyena Lv 11).
  * The grunt hands over **`ITEM_DEVON_PARTS`** ("Devon Parts" = the Devon Goods), shoves the player aside and **runs out west**.
  * **Briney walks in**, reunites with Peeko, introduces himself and leaves with Peeko.
  * Sets `FLAG_RECOVERED_DEVON_GOODS` (and clears `_STOLEN`), RUSTBORO_STATE 4, `VAR_BRINEY_HOUSE_STATE` 1, hides Briney on Route 116.

  Also `TRAINER_MIKE_2` (Hiker). Wanda and her boyfriend become visible only after the Mr. Stone scene. The rocks are cleared much later (TUNNEL_STATE 4/5, HM Strength, a later segment).
* **Original gist:**
  * Grunt: "Grah, keelhaul it all! That hostage Pokémon turned out to be worthless! And I made a getaway… in this tunnel to nowhere!" → loss: "My career in crime comes to a dead end! …The BOSS told me this would be a slick-and-easy job. All I had to do was steal some package from Devon. You want it back that badly, take it!"
  * Briney: "Peeko! Am I glad to see you're safe! Peeko owes her life to you! They call me Mr. Briney… you can usually find me in my cottage by the sea near Petalburg Woods. Come, Peeko."
* **Text labels:** `RusturfTunnel_Text_ComeAndGetSome`, `..._Peeko`, `..._GruntIntro`, `..._GruntDefeat`, `..._GruntTakePackage`, `..._PeekoGladToSeeYouSafe`, `..._ThankYouLetsGoHomePeeko`, `..._WhyCantTheyKeepDigging`, `..._ToGetToVerdanturf`, `..._BoyfriendOnOtherSideOfRock`. The later-segment texts `..._YouShatteredBoulderTakeHM`, `..._ExplainStrength`, `..._WandaReunion` are stored here too.
* **Characters:** Aqua grunt #2 (a separate trainer from #1; nothing ties them together). Mr. Briney, Peeko.
* **HC:** The cornered thief, with a Wingull as hostage, loses, gives back the package and escapes. The sailor gets his Wingull back and offers future help. The grunt answers to "the BOSS".

### A31 — Returning the goods
* **Maps:** `RustboroCity`.
* **Mechanics:** Coords near the north-east exit ((30,9), (31,10), (30,11), (30,12)) with STATE 4, or talking to the researcher → `..._ReturnGoods`: thanks, **Great Ball**, "Please come with me!" → `FLAG_RETURNED_DEVON_GOODS`, STATE 5, **warp to Devon Corp 3F**. The player **keeps** `ITEM_DEVON_PARTS`; it is delivered in Slateport (Segment B).
* **Original gist:** "The Devon Goods? You got them back! You really are a great Trainer! As thanks, another Great Ball! …Please come with me!"
* **Text labels:** `RustboroCity_Text_YouGotItThankYou`, `..._YoureLoadedWithItems`, `..._PleaseComeWithMe`.

### A32 — Devon Corp 3F: Mr. Stone, the LETTER and the PokéNav
* **Maps:** `RustboroCity_DevonCorp_3F`.
* **Mechanics:** `OnFrame`, `VAR_DEVON_CORP_3F_STATE` 0:
  * the researcher (3F employee, `OBJ_EVENT_GFX_MAN_2`) speaks, walks off and returns, then leads the player to the president's desk (music `MUS_FOLLOW_ME`);
  * **Mr. Stone** (`OBJ_EVENT_GFX_GENTLEMAN`) gives the **LETTER** (`ITEM_LETTER`), then the **PokéNav** (`FLAG_SYS_POKENAV_GET`, `FLAG_RECEIVED_POKENAV`), then **heals the party**.

  Side effects:
  * hides Wanda's boyfriend on Route 116 and shows him and **Wanda** inside Rusturf;
  * shows **Briney and Peeko in the cottage**, `VAR_BRINEY_LOCATION` 1;
  * **shows the rival in Rustboro**;
  * hides Scott in the School;
  * DEVON_3F_STATE 1, **RUSTBORO_STATE 6**.

  Later, talking to Mr. Stone: "I'm counting on you!" After the letter is delivered: **Exp. Share** (`FLAG_RECEIVED_EXP_SHARE`).
* **Original gist:**
  * Researcher: "Could I get you to deliver that parcel to the Shipyard in Slateport? It would be awful if those robbers tried to take it again…"
  * Mr. Stone: "I'm Mr. Stone, the President of the Devon Corporation. You saved our staff not once, but twice! I understand you're delivering a package to Slateport's Shipyard. On the way, could you stop off in Dewford Town and deliver a LETTER to STEVEN? I'd never be so cheap as to ask a favor for nothing in return… (PokéNav) It's a Pokémon Navigator… it has a map of the Hoenn region. By the way, I've heard that sinister criminals — MAGMA and AQUA, I believe — have been making trouble far and wide. Rest up before you go. Go with caution and care!"
* **Text labels:** `RustboroCity_DevonCorp_3F_Text_ThisIs3rdFloorWaitHere`, `..._WordWithPresidentComeWithMe`, `..._PleaseGoAhead`, `..._MrStoneIHaveFavor`, `..._MrStoneWantYouToHaveThis`, `..._ReceivedPokenav`, `..._MrStoneExplainPokenavRestUp`, `..._MrStoneGoWithCautionAndCare`, `..._CountingOnYou`, `..._ThankYouForDeliveringLetter`, `..._ExplainExpShare`, `..._NotFamiliarWithTrends`, `..._VisitCaptSternShipyard`, `..._RareRocksDisplay`.
* **HC:** The corporation's president gives **two errands**: a LETTER for Steven in Dewford and the recovered package for Capt. Stern at Slateport's Shipyard. He also gives the PokéNav. **The text names two criminal groups (Magma and Aqua)**. Since the hack merges them into one organization, rewrite this line; it is pure text. Flavor NPCs (`RustboroCity_Flat2_3F`) say the president collects rare stones "and the president's son also collects rare stones", which sets up the Steven = son reveal.

### A33 — Match Call upgrade outside Devon
* **Maps:** `RustboroCity`.
* **Mechanics:** `OnFrame`, STATE 6 → `..._ScientistAddMatchCall`:
  * sets `VAR_ROUTE104_STATE` 1;
  * a Devon scientist (`OBJ_EVENT_GFX_SCIENTIST_1`) appears at the Devon door, "!", adds Match Call (`FLAG_HAS_MATCH_CALL`, `FLAG_ADDED_MATCH_CALL_TO_POKENAV`);
  * a forced tutorial: the player must open the PokéNav from the start menu (`ScriptMenu_CreateStartMenuForPokenavTutorial`, `OpenPokenavForTutorial`);
  * the scientist leaves; STATE 7.

  Calling Mr. Stone now plays `MatchCall_Text_MrStone1` ("I'm looking down at you from my office window! Wahahaha!"). After this point: Mom registers when talked to (`PlayersHouse_1F_Text_IsThatAPokenav`), Birch registers when talked to (`MatchCall_Text_BirchRegisterCall`), and route trainers offer registration after battles.
* **Original gist:** "I've been developing an added feature for the PokéNav… may I see the one our President gave you? There — Match Call. Our President Stone should be registered. Please give our President a call."
* **Text labels:** `RustboroCity_Text_DevelopedNewPokenavFeature`, `..._AddedMatchCallPleaseCallMrStone`, `..._PleaseSelectPokenav`, `..._IdBetterGetBackToWork`; `data/text/match_call.inc`: `MatchCall_Text_MrStone1..4`.

### A34 — Rival encounter #2 (Match Call registration, optional battle)
* **Maps:** `RustboroCity` (primary) or `Route104` (fallback).
* **Mechanics:**
  * **Rustboro:** the rival stands at (16,50). Coords along the south exit (x 12–19, y 53) with STATE 7, or talking to the rival: rival music, "!", the rival approaches.
    * First time (`FLAG_MET_RIVAL_RUSTBORO`): the rival registers in the PokéNav (`FLAG_ENABLE_RIVAL_MATCH_CALL`, fanfare `MUS_REGISTER_MATCH_CALL`). Sets STATE 8 and `VAR_ROUTE104_STATE` 2.
    * YES/NO battle offer. YES → `TRAINER_MAY_RUSTBORO_{TREECKO|TORCHIC|MUDKIP}` / `TRAINER_BRENDAN_RUSTBORO_*` (two Pokémon. Player chose Treecko → Lotad 13 + Torchic 15. Player chose Torchic → May: Torkoal 13 / Brendan: Slugma 13, + Mudkip 15. Player chose Mudkip → Wingull 13 + Treecko 15) → `FLAG_DEFEATED_RIVAL_RUSTBORO`.
    * NO → "Haven't you raised your Pokémon?"; asks again on the next talk.
  * **Route 104 fallback:** with `VAR_ROUTE104_STATE` 1, a coord at (17,51) in front of **Briney's cottage** makes the rival **walk out of the cottage** with the same content. It uses the same trainer IDs and `FLAG_DEFEATED_RIVAL_ROUTE_104`.
  * Boarding Briney's boat hides both rival objects (`FLAG_HIDE_RUSTBORO_CITY_RIVAL`, `FLAG_HIDE_ROUTE_104_RIVAL`), so the encounter can be **missed entirely**.
* **Original gist:**
  * May: "You had a Match Call feature put on your PokéNav! Let's register each other… By the way, I passed Mr. Briney in Petalburg Woods, on his way home to his cottage by the sea. How's your Pokédex? Mine's looking pretty decent! How about a little battle?" → "You just became a Trainer, I'm not going to lose!" → after: "Mr. Briney was once a revered seafarer." (Route 104 version: "you should be like Mr. Briney. It's important to become friends with Pokémon, too.")
  * Brendan: same beats, cockier ("Mine rules").
* **Text labels:** `RustboroCity_Text_MayHiLetsRegister`, `..._RegisteredMay`, `..._MayPassedBrineyWantToBattle`, `..._MayOhHaventRaisedPokemonEnough`, `..._MayWantToBattle`, `..._MayImNotGoingToLose`, `..._MayDefeat`, `..._MayMrBrineyHint`, and the Brendan equivalents. `Route104_Text_MayWeShouldRegister`, `..._RegisteredMay`, `..._MayHowsYourPokedex`, `..._MayMinesDecentLetsBattle`, `..._MayHaventRaisedPokemon`, `..._MayLetsBattle`, `..._MayIntro`, `..._MayDefeat`, `..._MayPostBattle`, and the Brendan equivalents. Rival PokéNav calls: `MatchCall_Text_May1/Brendan1` (always), `..._May2/Brendan2` (after the Dewford Gym).
* **HC:** The rival exchanges PokéNav contacts and offers an optional battle. Rustboro rival trainer IDs are reused on Route 104. The rival points the player toward Mr. Briney.

### A35 — Mr. Briney's cottage: setting sail
* **Maps:** `Route104_MrBrineysHouse`.
* **Mechanics:** With `VAR_BRINEY_HOUSE_STATE` 1, Briney and Peeko walk around the room. Talking to Briney:
  * first time (`FLAG_MR_BRINEY_SAILING_INTRO`): "Hold on, lass! Wait up, Peeko!" then a YES/NO sail offer;
  * if the letter is not delivered: "deliveries… set sail for Dewford";
  * after the letter, if the goods are not delivered: "package for Capt. Stern in Slateport… first Dewford".

  YES → `..._SailToDewford`: `VAR_BOARD_BRINEY_BOAT_STATE` 1, Briney shown on Route 104, hidden in the house, RUSTBORO_STATE 8, ROUTE104_STATE 2, both rival objects hidden, warp to Route 104 (13,51). NO → "Your deliveries can wait? Tell me whenever you want to set sail."
* **Original gist:** "You're {PLAYER}! You saved my darling Peeko! What's that? You want to sail with me? A LETTER bound for Dewford and a package for Slateport… Quite the busy life! You've come to the right man! We'll set sail for Dewford. Anchors aweigh! Peeko, we're setting sail, my darling!"
* **Text labels:** `Route104_MrBrineysHouse_Text_WaitUpPeeko`, `..._ItsYouLetsSailToDewford`, `..._SetSailForDewford`, `..._DeclineDeliverySail`, `..._NeedToMakeDeliveriesSailToDewford`, `..._NeedToDeliverPackageSailToDewford`, `..._WhereAreWeBound`, `..._TellMeWheneverYouWantToSail`, `..._Peeko`.
* **HC:** The grateful sailor ferries the player in his boat. This is the **only** way off the mainland in this segment, because Surf comes much later.

### A36 — The boat trip; Dad's first PokéNav call
* **Maps:** `Route104` → (visually) `Route105`, `Route106` → `DewfordTown`.
* **Mechanics:** `Route104` `OnFrame`, BOARD_STATE 1:
  * Briney boards the boat (`OBJ_EVENT_GFX_MR_BRINEYS_BOAT`), the player boards, boat music plays;
  * if `FLAG_ENABLE_NORMAN_MATCH_CALL` is unset, **Dad calls mid-voyage** (`pokenavcall`) and is registered (`FLAG_ENABLE_NORMAN_MATCH_CALL`);
  * the boat reaches Dewford, Briney and the boat are moved to Dewford, `VAR_BRINEY_LOCATION` is set, BOARD_STATE 0;
  * landing line: before the letter, "off to deliver that LETTER to STEVEN"; otherwise "We've hit land in Dewford."

  The boat passes through Routes 105 and 106, whose water trainers are reachable only with Surf (later).
* **Original gist:** Dad (call): "Oh, {PLAYER}? Where are you now? It sounds windy… I just heard from Devon's Mr. Stone about your PokéNav, so I decided to call. It sounds like you're doing fine. You take care now." Briney: "Ahoy! We've hit land in Dewford. I suppose you're off to deliver that LETTER to, who was it now, STEVEN!"
* **Text labels:** `Route104_Text_DadPokenavCall` and `Route104_Text_RegisteredDadInPokenav` (stored in **`Route105/scripts.inc`**), `Route104_Text_LandedInDewfordDeliverLetter` (stored in **`DewfordTown/scripts.inc`**), `DewfordTown_Text_BrineyLandedInDewford`. Dad's later calls: `MatchCall_Text_Norman1` (Cutter hint), `MatchCall_Text_Norman2` (after the Dewford Gym).
* **HC:** A sea crossing by boat with the old sailor. The father phones during the trip, having heard about the PokéNav from the corporation's president.

### A37 — Dewford Town
* **Maps:** `DewfordTown`, `DewfordTown_Hall`, `DewfordTown_House1/2`, `DewfordTown_PokemonCenter_1F`.
* **Mechanics:**
  * Briney at the dock: while `FLAG_DELIVERED_STEVEN_LETTER` is unset, he only offers to sail **back to Petalburg** (the boat lands at the Route 104 cottage, BOARD_STATE 2). After the letter: a menu of Petalburg / **Slateport** / cancel. **The letter gates Segment B.**
  * Fisherman gives the **Old Rod** (YES/NO).
  * Dewford Hall: trendy-phrase man (Easy Chat, `EASY_CHAT_TYPE_TRENDY_PHRASE`), **TM Sludge Bomb** man.
  * House 2: **Silk Scarf**, plus a Brawly fan.
  * Pokémon Center hint: "a stone cavern at the edge of town… rare stones there."
  * Route 106 (land part, west beach): fishermen `TRAINER_ELLIOT_1`, `TRAINER_NED`, leading to Granite Cave.
* **Original gist:** Briney: "Have you delivered your LETTER? Or were you meaning to sail back to Petalburg?" / "Then you go on and deliver the LETTER. I'll be waiting." Town sign: "A tiny island in the blue sea."
* **Text labels:** `DewfordTown_Text_SetSailBackToPetalburg`, `..._GoDeliverIllBeWaiting`, `..._PetalburgWereSettingSail(2)`, `..._SlateportWereSettingSail`, `..._WhereAreWeBound`, `..._JustTellMeWhenYouNeedToSetSail`, `..._GettingItchToFish`, `..._GiveYouOneOfMyRods`, `..._TinyIslandCommunity`, the signs, the trendy-phrase texts; `DewfordTown_PokemonCenter_1F_Text_StoneCavern`; `DewfordTown_House2_Text_BrawlySoCool`.

### A38 — Granite Cave: Flash and the dark depths
* **Maps:** `Route106` → `GraniteCave_1F`, `GraniteCave_B1F`, `GraniteCave_B2F`, `GraniteCave_StevensRoom`.
* **Mechanics:**
  * 1F: a hiker (`OBJ_EVENT_GFX_HIKER`) gives **HM05 Flash** (`FLAG_RECEIVED_HM_FLASH`). Flash needs the Dewford badge.
  * Lower floors are dark.
  * B1F has cracked floor tiles (`CaveHole_*`, `setholewarp` → B2F; mainly for the Mach Bike later).
  * B2F has Rock Smash rocks (later) and hidden Everstones.
  * No trainers. Items: Escape Rope, Poké Ball, Repel, Rare Candy.
  * Steven's room is at the end of the route through the cave.
* **Original gist:** Hiker: "It gets awfully dark ahead… That guy who came by earlier, STEVEN I think, knew how to use Flash… for us Hikers, helping out those we meet is our motto."
* **Text labels:** `GraniteCave_1F_Text_GetsDarkAheadHereYouGo`, `GraniteCave_1F_Text_ExplainFlash`.
* **HC:** A dark cave on the island. The recipient of the letter went in ahead of the player.

### A39 — Steven receives the LETTER
* **Maps:** `GraniteCave_StevensRoom`.
* **Mechanics:** Talking to Steven (`OBJ_EVENT_GFX_STEVEN`):
  * the LETTER is handed over (`Common_EventScript_PlayerHandedOverTheItem`), `FLAG_DELIVERED_STEVEN_LETTER`;
  * he gives **TM47 Steel Wing** and registers in the PokéNav (`FLAG_REGISTERED_STEVEN_POKENAV`);
  * he walks out and the object is removed.

  Side effects: Briney's Slateport option unlocks; Mr. Stone's next call (`MatchCall_Text_MrStone3`) invites you back for a reward (Exp. Share).
* **Original gist:** "My name is Steven. I'm interested in rare stones, so I travel here and there. Oh? A LETTER for me? …You went through all this trouble. I'll give you this TM, my favorite move, Steel Wing. Your Pokémon appear quite capable. If you keep training, you could even become the Champion of the Pokémon League one day… let's register one another in our PokéNavs. Now, I've got to hurry along."
* **Text labels:** `GraniteCave_StevensRoom_Text_ImStevenLetterForMe`, `..._ThankYouTakeThis`, `..._CouldBecomeChampionLetsRegister`, `..._RegisteredSteven`, `..._IveGotToHurryAlong`, `..._OhBagIsFull`; Steven's calls `MatchCall_Text_Steven1..`.
* **HC:** A stone-collecting young man in the depths of a cave receives the president's letter, rewards the player with Steel Wing, predicts they could become Champion, exchanges contacts and leaves immediately. **The letter's contents are never shown**, so writers are free to define them.

### A40 — Dewford Gym: Brawly (badge 2)
* **Maps:** `DewfordTown_Gym`.
* **Mechanics:** Dark Gym. The flash radius grows (7 steps, `animateflash`) for each defeated trainer: `TRAINER_TAKAO`, `TRAINER_JOCELYN`, `TRAINER_LAURA`, `TRAINER_BRENDEN`, `TRAINER_CRISTIAN`, `TRAINER_LILITH`. Gym guide. `TRAINER_BRAWLY_1`: Machop 16, Meditite 16, Makuhita 19 @ Sitrus Berry; 2 Super Potions. On win:
  * **Knuckle Badge**, `FLAG_BADGE02_GET`, `FLAG_DEFEATED_DEWFORD_GYM`, `VAR_PETALBURG_GYM_STATE += 1`;
  * **TM08 Bulk Up**;
  * **Brawly registers in the PokéNav** (`FLAG_ENABLE_BRAWLY_MATCH_CALL`);
  * arms Roxanne's first call (`FLAG_ENABLE_ROXANNE_FIRST_CALL`, step counter reset).

  Order relative to A38/A39 is **free**.
* **Original gist:** Brawly: "I'm Brawly! Dewford's Gym Leader! I've been churned in the rough waves of these parts, and I've grown tough in the pitch-black cave!" → "You made a much bigger splash than I expected! You swamped me!" Badge: "Pokémon up to Lv 30 obey; Flash outside battle." Guide: "the Gym is as dark as the ocean floor, but it gets brighter after defeating the trainers."
* **Text labels:** `DewfordTown_Gym_Text_BrawlyIntro`, `..._BrawlyDefeat`, `..._ReceivedKnuckleBadge`, `..._KnuckleBadgeInfoTakeThis`, `..._ExplainBulkUp`, `..._RegisteredBrawly`, `..._BrawlyPostBattle`, `..._GymGuideAdvice`, `..._GymGuidePostVictory`, the statue texts and the trainer lines.
* **HC:** Leader Brawly (surfer and fighter look, sprite) uses Fighting types in a dark Gym that brightens as you win.

### A41 — Segment exit (hooks into Segment B)
* Briney at Dewford now offers **Slateport** (Segment B start: deliver the Devon Parts to Capt. Stern).
* Optional backtracking: Mr. Stone (Devon 3F) gives the **Exp. Share** (`FLAG_DELIVERED_STEVEN_LETTER` → `FLAG_RECEIVED_EXP_SHARE`). Norman: "you have gotten stronger." Mom: Match Call registration and heal.
* PokéNav calls that fire around here: Roxanne's register call (`RustboroCity_Gym_Text_RoxanneRegisterCall`, "I heard from Brawly…"), the rival (`May2/Brendan2`), Dad (`Norman2`), Mr. Stone (`MrStone3/4`, including "Devon was digging Rusturf Tunnel but shut it down to protect the Pokémon").

---

## 4. Characters in this segment

| Original name | Sprite (overworld) / battle pic | Role in segment | Where | Constraint notes |
|---|---|---|---|---|
| Player | `BRENDAN_*` / `MAY_*` player gfx (to be replaced by the new red-haired protagonist) | Gym Leader's child, just moved to Littleroot | everywhere | Both genders remain selectable in the code |
| Mom | `MOM` | Moves in with the player; Running Shoes; heals at home; registers in PokéNav | Littleroot | Lives in the player's house |
| Dad / Norman | `NORMAN`; trainer pic Leader Norman | Petalburg Gym Leader, the player's father; sends Wally along; sets the 4-badge goal; calls on the boat | Petalburg Gym; PokéNav | Later comes home (S.S. Ticket) and is Gym 5 |
| Prof. Birch | `PROF_BIRCH`; intro pic | Intro narrator; rescued; gives starter and Pokédex; later rotates Lab/R101/R103 rating the Dex | Lab, Route 101/103 | Friend of Dad; field researcher |
| May / Brendan | `RIVAL_MAY_NORMAL` / `RIVAL_BRENDAN_NORMAL`; trainer pics May/Brendan | Birch's child, rival | Rival's house, Route 103, Oldale, lab, Rustboro / Route 104 | Opposite gender to the player; battles R103 (Lv 5) and Rustboro (Lv 13/15) |
| Rival's mom | `WOMAN_4` | Greets the new neighbor | Rival's house 1F | |
| Rival's sibling | `NINJA_BOY` | Flavor | Rival's house 1F | |
| Vigoroth ×2 | `VIGOROTH_CARRYING_BOX`, `VIGOROTH_FACING_AWAY` | Movers | Player's house 1F | |
| Twin girl | `TWIN` | Blocks Route 101, sends the player to the rescue | Littleroot | |
| Lab aide | `SCIENTIST_1` | Explains Birch's absence | Lab | |
| Oldale Mart employee | `MART_EMPLOYEE` | Shop tutorial, Potion | Oldale | |
| Footprints man | `MANIAC` | Blocks Route 102 until the Pokédex | Oldale | |
| Gym boy | `BOY_2` | Drags the player to the Petalburg Gym | Petalburg | |
| Wally | `WALLY` | Frail boy, catches Ralts, leaves for Verdanturf | Petalburg Gym and town | Recurs later (rival-like) |
| Wally's mom / dad | `WOMAN_4` / `POKEFAN_M` | Parents; dad gives HM Surf later | Petalburg, Wally's house | |
| Scott | `SCOTT` | Talent scout cameo | Petalburg west exit, Rustboro School | Recurs |
| Devon researcher | `MAN_2` | Robbed twice; guides the player to Mr. Stone; later gives the Repeat Ball on Route 116 | Petalburg Woods, Rustboro, Devon 3F | One recurring character across 3+ maps |
| Aqua grunt #1 | `AQUA_MEMBER_M`; pic Aqua Grunt M; class "TEAM AQUA" | Woods ambusher | Petalburg Woods | `TRAINER_GRUNT_PETALBURG_WOODS` |
| Aqua grunt #2 | same | Goods thief, Peeko kidnapper | Rustboro → Rusturf | `TRAINER_GRUNT_RUSTURF_TUNNEL` |
| Roxanne | `ROXANNE`; Leader pic | Gym 1 (Rock) | Rustboro Gym | |
| Cutter | (house NPC) | HM Cut | Rustboro | |
| Mr. Briney | `EXPERT_M` | Retired sailor; Peeko's owner; ferryman | Route 116, Rusturf, cottage, boat, Dewford | |
| Peeko | `WINGULL` (+ Wingull cry) | Briney's pet, hostage | Rusturf, cottage | |
| Wanda's boyfriend / Wanda | `BLACK_BELT` / `WOMAN_2` | Digging the tunnel by hand; lovers separated by rock | Route 116, Rusturf | Payoff in a later segment (HM Strength) |
| Mr. Stone | `GENTLEMAN` | Devon president; LETTER, PokéNav, Exp. Share | Devon 3F; PokéNav | Father of Steven (implied by NPCs) |
| Devon scientist | `SCIENTIST_1` | Match Call upgrade | Rustboro | |
| Granite Cave hiker | `HIKER` | HM Flash | Granite Cave 1F | |
| Steven | `STEVEN` | Letter recipient, stone collector, future Champion | Granite Cave | Recurs throughout the game |
| Brawly | `BRAWLY`; Leader pic | Gym 2 (Fighting) | Dewford Gym | |
| Gym guide | `MAN_2` | Gym advice in every Gym | Gyms | |

---

## 5. Notable non-story NPCs and systems (mechanical)

* **Mom heal:** after `FLAG_RESCUED_BIRCH`, talking to Mom heals the party (`GAME_STAT_RESTED_AT_HOME`). After Match Call she offers registration. After badge 5 (later) she gives the Amulet Coin.
* **Bedroom PC / Player's PC**, **Pokémon Centers** (Oldale, Petalburg, Rustboro, Dewford), **Marts** (Oldale: basic → expanded after the Pokédex; Petalburg; Rustboro: expanded after `FLAG_MET_DEVON_EMPLOYEE`). Dewford has no Mart.
* **Birch Pokédex rating:** `ProfBirch_EventScript_RatePokedexOrRegister` (lab, Route 101 and Route 103 objects). Location rotates with `VAR_BIRCH_STATE` (0–1 lab, 2–3 R101, 4–5 R103, 6 lab).
* **Match Call:** route trainers register after battles once `FLAG_HAS_MATCH_CALL` is set (Calvin, James, Haley, Winston, Cindy, Gina & Mia, Jerry, Karen, Andres, Elliot, …), which enables rematches. NPC call texts are in `data/text/match_call.inc`, gated in `src/pokenav_match_call_data.c`.
* **Item gifts (non-story):** Potion (Oldale Mart employee); Chesto Berry and TM Bullet Seed (Route 104); Wailmer Pail (Pretty Petal Flower Shop); Miracle Seed (Petalburg Woods girl, needs badge 1); Quick Claw (Rustboro School teacher); Premier Ball (Rustboro Flat2 2F); HM Cut (Cutter); Old Rod (Dewford); TM Sludge Bomb (Dewford Hall); Silk Scarf (Dewford House 2); HM Flash (Granite Cave).
* **Trade:** Rustboro House 1 — the player's Ralts for Seedot "DOTS".
* **Devon Corp 2F:** fossil revival (Root → Lileep, Claw → Anorith, Lv 20; fossils come later); devs talk about the PokéNav, new Poké Balls and "a device for talking with Pokémon".
* **Rustboro Flat1 2F:** Walda's father (wallpaper and Walda phrase), Devon researcher flavor.
* **Berry trees** on Routes 102, 103 and 104 (`BerryTreeScript`).
* **Trendy phrase** (Dewford Hall, Easy Chat).
* **TV** in the player's house (`FLAG_SYS_TV_HOME`) and in Pokémon Centers.
* **Abnormal-weather / Regi hooks** on Routes 105, 116 and 105's cave entrance are post-game. Writers can ignore them here.
* **Gym statues** show certified trainers after the badge.

---

## 6. Cross-segment hooks introduced in Segment A

* **The package → Capt. Stern, Slateport Shipyard** (`ITEM_DEVON_PARTS`, delivered in Segment B; `FLAG_DELIVERED_DEVON_GOODS`).
* **Steven** (stone collector, Mr. Stone's son) recurs and reaches the League.
* **Wally** leaves for Verdanturf (health). He recurs later (Mauville battle, Victory Road).
* **Norman** is Gym 5 (4 badges required) and comes home later with the S.S. Ticket. His Match Call texts continue.
* **Mr. Briney** keeps ferrying (Dewford ↔ Petalburg ↔ Slateport). "Once a revered seafarer."
* **Rusturf Tunnel**: Wanda and her boyfriend are separated by rocks (cleared later with Rock Smash; reward HM Strength).
* **Scott** continues to appear (Battle Frontier).
* **Devon** continues to appear: Repeat Ball, Route 116 employee after Slateport, fossils, Mr. Stone's calls.
* **The villain faction**: Woods grunt ("after something in Rustboro"), tunnel grunt ("the BOSS"). Mr. Stone's line explicitly names *two* criminal groups (Magma and Aqua). **Only Aqua-uniform grunts appear in Segment A.**

---

## 7. Strings outside map scripts that belong to this segment

* `data/text/birch_speech.inc`: the intro speech (A01).
* `src/strings.c`: `gText_BirchInTrouble`, `gText_ConfirmStarterChoice` (A10).
* `src/battle_main.c` `gTrainerClasses`: "TEAM AQUA", "LEADER", "{PKMN} TRAINER" (rival class), "YOUNGSTER", "HIKER", … These are the class names shown in battle for the grunts, leaders and rival.
* `src/data/trainers.party`: trainer names (MAY, BRENDAN, GRUNT, ROXANNE, BRAWLY, …).
* `src/data/items.h`: "Devon Parts", "Letter", "PokéNav" (key item), TM names.
* `data/text/match_call.inc`: Mr. Stone, Mom, Dad, rival, Steven, Brawly and Roxanne call scripts.
* `src/data/trade.h`: "DOTS" / "KOBE" (Rustboro trade).
* PokéNav tutorial and region-map names (`src/data/region_map/region_map_sections.json`: LITTLEROOT TOWN, ROUTE 101, …).

---

## 8. Condensed hard-constraint checklist (what any rewrite must still show)

1. The player arrives in a moving truck with Mom. The house is unpacked by Vigoroth. A clock is set. The father is a Gym Leader seen (missed) on TV.
2. The professor's family lives next door. The rival is the professor's opposite-gender child, who already owns Pokémon.
3. A girl blocks Route 101 until the rival is met. The professor is chased by a wild Zigzagoon. The player picks Treecko, Torchic or Mudkip from his bag and fights the Zigzagoon.
4. The rival battles the player on Route 103 with one Lv 5 counter-starter. The professor gives the Pokédex, the rival gives 5 Poké Balls, Mom gives Running Shoes.
5. A sketching man blocks Oldale west until the Pokédex.
6. A boy drags the player into the Petalburg Gym. Norman (father, Leader) refuses to battle. Wally borrows Norman's Zigzagoon and catches a Ralts. Norman points to Rustboro and Roxanne and requires 4 badges.
7. A scout (Scott) appraises the player at Petalburg's exit, then again at the Rustboro school.
8. In Petalburg Woods an Aqua-uniformed grunt tries to rob a Devon researcher of papers, loses (Poochyena 9), mentions a target in Rustboro and flees.
9. Roxanne (Rock) is beaten **before** the theft. Leaving her Gym, the player sees a grunt flee with Devon's goods, chased by the same researcher.
10. Old sailor Briney's Wingull Peeko was taken. The grunt hides in the dead-end Rusturf Tunnel, loses (Poochyena 11), returns the package and escapes. Briney thanks the player.
11. The package is returned. The Devon president Mr. Stone gives a **LETTER for Steven (Dewford)**, the **PokéNav**, and the errand to bring the package to **Capt. Stern in Slateport**. A scientist adds Match Call.
12. The rival exchanges contacts and offers an optional battle (Rustboro south exit or outside Briney's cottage).
13. Briney sails the player to Dewford. Dad phones on the way.
14. Granite Cave: a hiker gives Flash. Steven, deep in the cave, takes the letter, gives Steel Wing and exchanges contacts. This unlocks the Slateport sail.
15. Brawly (Fighting, dark Gym) gives badge 2. Order relative to Steven is free.
