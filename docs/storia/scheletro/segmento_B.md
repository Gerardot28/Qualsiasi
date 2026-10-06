# Segment B — Hoenn "Middle 1" (Slateport → Surf)

Skeleton of scripted story beats, extracted from the pristine source
(`/home/user/pex-orig`, pokeemerald-expansion 1.17.1). Story writers must follow the
**order**, the **mechanics** and the **hard constraints (HC)** listed here. Only the
words and the motivations can change. Everything below was checked against the map
scripts (`data/maps/<Map>/scripts.inc`), the object and trigger tables (`map.json`),
`data/scripts/new_game.inc`, `data/event_scripts.s` and `src/*.c`.

- **Segment start:** the player has delivered Steven's letter in Granite Cave
  (`FLAG_DELIVERED_STEVEN_LETTER`). Mr. Briney in Dewford now offers the trip to Slateport.
- **Segment end:** the player has `ITEM_HM_SURF` (`FLAG_RECEIVED_HM_SURF`,
  `VAR_PETALBURG_CITY_STATE = 5`). Wattson's optional New Mauville errand opens at the same time.
  The next segment starts with the Surf crossing of Route 118 (Steven on the east bank) and Route 119.
- **Badges won in this segment:** #3 Dynamo (Wattson, Mauville), #4 Heat (Flannery, Lavaridge),
  #5 Balance (Norman, Petalburg).

Legend: **[M]** = mandatory for progress. **[S]** = soft-mandatory (not gated here, but the
reward is needed later). **[O]** = optional. **HC** = a hard constraint for the
re-interpretation, which must stay true in the new story because the event script does it.

---

## 0. Critical path at a glance

| # | Where | What unlocks the next step (gate) |
|---|-------|-----------------------------------|
| 1 | Dewford → Route 109 (boat) | `FLAG_DELIVERED_STEVEN_LETTER` lets Briney offer "Slateport" |
| 2 | Slateport, Stern's Shipyard | Talking to Dock sets `FLAG_HIDE_SLATEPORT_CITY_TEAM_AQUA`. The grunt queue that physically blocks the museum door (x30–31,y26) disappears |
| 3 | Oceanic Museum 1F→2F | Two forced grunt battles, then Archie. Sets `FLAG_DELIVERED_DEVON_GOODS` and `FLAG_HIDE_ROUTE_110_TEAM_AQUA`; the grunts blocking the Route 110 exit disappear |
| 4 | Route 110 | Birch trigger (`VAR_REGISTER_BIRCH_STATE` 1→2) and rival trigger (`VAR_ROUTE110_STATE` 0→1). Both are full-width coord triggers on the path |
| 5 | Mauville | Wally (x8,y6) stands in front of the gym door (x8,y5). You must battle him (`FLAG_DEFEATED_WALLY_MAUVILLE`) |
| 6 | Mauville Gym | Badge 3 (`FLAG_BADGE03_GET`) lets the player use Rock Smash outside battle |
| 7 | Mauville House 1 | `ITEM_HM_ROCK_SMASH`. Breakable rocks on Route 111 (x18–19,y100–101) gate the north |
| 8 | Route 112 | Two Magma grunts block the Cable Car station (`FLAG_HIDE_ROUTE_112_TEAM_MAGMA`) → detour through Fiery Path → Route 113 → Fallarbor → Route 114 |
| 9 | Meteor Falls 1F | Coord trigger (x14,y18, `VAR_METEOR_FALLS_STATE` 0→1). This scene hides the Route 112 grunts |
| 10 | Cable Car → Mt. Chimney | Maxie battle (`FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY`) |
| 11 | Jagged Pass → Lavaridge | Flannery: badge 4 (`FLAG_BADGE04_GET`) → `VAR_LAVARIDGE_TOWN_STATE = 1` → the rival gives `ITEM_GO_GOGGLES` |
| 12 | Petalburg Gym | Norman fights only at `VAR_PETALBURG_GYM_STATE == 6` (4 badges). Badge 5, then Wally's father → `ITEM_HM_SURF` |

Optional branches: Seashore House, Trick House, Cycling Road, Verdanturf/Rusturf (HM Strength, [S]),
Winstrates, desert/Mirage Tower (needs Go-Goggles + Mach Bike), Cozmo's Meteorite, New Mauville (needs Surf).

### State variables touched in this segment (verified with grep)

| Variable / flag | Values and where they change |
|---|---|
| `VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE` | 0 → **1** (Museum 2F, end of the Aqua scene) → **2** (Scott scene outside the museum) → **3** (Scott leaves the Slateport Battle Tent [O], OR forced to 3 when Wattson is beaten) |
| `VAR_SLATEPORT_MUSEUM_1F_STATE` | reset to 0 on every Slateport transition; 1 after paying ¥50 |
| `VAR_REGISTER_BIRCH_STATE` | 0 → **1** (Museum 2F) → **2** (Route 110 Birch scene) |
| `VAR_ROUTE110_STATE` | 0 → **1** (rival battle) |
| `VAR_PETALBURG_GYM_STATE` | arrives at **4** from segment A (2 after the Wally tutorial, +1 Roxanne, +1 Brawly); **+1 per badge** here (Wattson → 5, Flannery → 6); at **6** `Common_EventScript_ReadyPetalburgGymForBattle`; Norman sets **7** |
| `VAR_SCOTT_STATE` | +1 Slateport outside the museum, +1 Slateport tent [O], +1 Mauville after Wally, +1 Verdanturf tent [O], +1 Fallarbor tent [O] |
| `VAR_METEOR_FALLS_STATE` | 0 → **1** (Magma steals the Meteorite) |
| `VAR_CABLE_CAR_STATION_STATE` | 1 = arriving at the top, 2 = arriving at the bottom, 0 = idle |
| `VAR_LAVARIDGE_TOWN_STATE` | 0 → **1** (Flannery beaten) → **2** (Go-Goggles given) |
| `VAR_MIRAGE_TOWER_STATE` | 0 → 1 (fossil taken, collapse) → 2 (disintegration watched) → 3 (gone) [O] |
| `VAR_PETALBURG_CITY_STATE` | (3 from segment A) → **4** (Wally's father escorts the player) → **5** (Surf given) |
| `VAR_NEW_MAUVILLE_STATE` | 0 → 1 (door opened with the Basement Key) → 2 (generator off) [O] |
| `VAR_RUSTURF_TUNNEL_STATE` | 4/5 (rock smashed, set from C `TryUpdateRusturfTunnelState`) → 6 (reunion done) [O/S] |
| `VAR_CYCLING_CHALLENGE_STATE`, `VAR_TRICK_HOUSE_*` | side systems [O] |

---

## 1. Story beats (in play order)

### B01 — The boat lands on the beach of Route 109 [M]
- **Maps:** `DewfordTown` (departure), `Route109` (arrival)
- **Mechanics:** Mr. Briney's menu in Dewford (`MULTI_BRINEY_ON_DEWFORD`) offers Slateport only if
  `FLAG_DELIVERED_STEVEN_LETTER` is set. Script `DewfordTown_EventScript_SailToSlateport`: the
  boat-and-player sprite sails (music `Common_EventScript_PlayBrineysBoatMusic`) and lands on
  Route 109's beach. It clears `FLAG_HIDE_ROUTE_109_MR_BRINEY` and `FLAG_HIDE_ROUTE_109_MR_BRINEY_BOAT`.
  Briney and his boat now **stay on Route 109** as a return ferry to Dewford
  (`Route109_EventScript_MrBriney`; before the delivery he asks "go back to Dewford?", after it he shows the menu `MULTI_BRINEY_OFF_DEWFORD`).
  Briney stays until Norman is beaten (`EventScript_HideMrBriney`, B32).
- **Original text (gist):** BRINEY: "Ahoy! We've made land in SLATEPORT! I suppose you're going
  to visit CAPT. STERN and deliver the DEVON GOODS?"
- **Characters:** Mr. Briney (`OBJ_EVENT_GFX_EXPERT_M`, old sailor) + Peeko (Wingull pet).
- **HC:** an old captain ferries the player by boat to the beach. He stays parked there as a
  two-way ferry (Dewford ↔ Slateport) until badge 5. The player is still carrying the Devon package
  (`ITEM_DEVON_PARTS`) meant for "Stern".
- Route 109 (beach): 13 trainers (water and sand) + Seashore House (see §3).

### B02 — Slateport: a queue of villains in front of the museum [M]
- **Map:** `SlateportCity`
- **Mechanics:** 11 Team Aqua grunts (`OBJ_EVENT_GFX_AQUA_MEMBER_M/F`, flag
  `FLAG_HIDE_SLATEPORT_CITY_TEAM_AQUA`, local ids `LOCALID_SLATEPORT_GRUNT_1..11`) stand in a line
  at y=26–27, x20–31. The two at x30/x31 stand on the museum door tiles (warps at x30/31,y26).
  The door cannot be reached. The grunts can be talked to (no battles). Grunt 9 pretends to read
  the sign for the player. Woman1 (`WOMAN_4`) asks "What is that long line?"; after the queue is gone she
  says she loved the museum as a child. OnTransition sets `FLAG_VISITED_SLATEPORT_CITY`.
- **Original text (gist):** "Quit pushing! This is the line!" / "TEAM AQUA has a policy of
  assembling and dispersing at the operation site." / "Our BOSS is brilliant. What would he want with
  a MUSEUM?" / "When this operation's over, our leader said he'd take us to a hot spring spa!" /
  "Why are we even lining up and paying? We should just march in!"
- **Characters:** Team Aqua grunts (11, M/F sprites).
- **HC:** a long, orderly queue of villain grunts blocks the museum entrance. They are
  waiting to go in and do not fight. Their leader is not there yet.

### B03 — Stern's Shipyard: Dock sends you to look for Stern [M]
- **Map:** `SlateportCity_SternsShipyard_1F`
- **Mechanics:** Dock (`OBJ_EVENT_GFX_MAN_1`, `LOCALID_DOCK`) talks to himself about the parts,
  then turns to the player. He notices the Devon Goods but says it "won't do" and that Stern has gone off somewhere.
  It sets `FLAG_DOCK_REJECTED_DEVON_GOODS` **and `FLAG_HIDE_SLATEPORT_CITY_TEAM_AQUA`**: the queue
  outside disappears (in the story the grunts went inside). Two scientists (`SCIENTIST_1`) have flavour text.
  Later states of Dock: after the delivery he wants "advice from a veteran"; at badge 7 Briney has joined;
  after the game is cleared the ferry is ready.
- **Original text (gist):** DOCK: "I'm DOCK. CAPT. STERN commissioned me to design a ferry. Oh! Are
  those DEVON GOODS? … CAPT. STERN went off somewhere. Could you find him and deliver that to him?"
- **Characters:** Dock (ferry engineer), 2 shipyard scientists.
- **HC:** a shipwright who is building a ferry for Stern redirects the player. **This talk is
  the trigger that clears the queue at the museum door** (the queue has to "go somewhere" in the new story).

### B04 — Oceanic Museum 1F: paying in, grunts inside, the "familiar grunt" [M] (+[O] TM)
- **Map:** `SlateportCity_OceanicMuseum_1F`
- **Mechanics:** a coord trigger at the counter (x9/10,y7, `VAR_SLATEPORT_MUSEUM_1F_STATE == 0`) asks
  for ¥50. If the player cannot pay **before** the delivery, the receptionist lets them in anyway ("you're with that
  group that went in earlier? Go catch up!"). After the delivery, a player without the money is pushed back.
  Six Aqua grunts (`FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_AQUA_GRUNTS`) wander the hall (talk only).
  The **familiar grunt** (`LOCALID_OCEANIC_MUSEUM_FAMILIAR_GRUNT`, the Rusturf Tunnel thief from segment A)
  recognises the player, hands over `ITEM_TM_THIEF` (`FLAG_RECEIVED_TM_THIEF`) and runs out
  (`FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_FAMILIAR_AQUA_GRUNT`).
  Museum visitors (`FLAG_HIDE_SLATEPORT_MUSEUM_POPULATION`) are hidden at first. Arriving in Mauville clears this flag.
- **Original text (gist):** grunts: "We, TEAM AQUA, exist for the good of all!" / "Our BOSS, the
  linchpin, isn't here." / "If our goons hadn't bungled things in RUSTBORO, we wouldn't be here!" /
  "I didn't have ¥50, so it took a long time getting past the receptionist." Familiar grunt: "Aiyeeeh! I'm the
  one you thumped in RUSTURF TUNNEL! Here, take this! You have to forgive me! … Hope I never see you again!"
- **Characters:** receptionists (`BEAUTY` ×2), Aqua grunts, the recurring cowardly grunt.
- **HC:** the villains have occupied the museum ground floor. The coward from the Rusturf theft
  is there, gives the player a "stealing" TM as a peace offering and flees.

### B05 — Museum 2F: Stern, the robbery attempt, the leader's first appearance [M]
- **Map:** `SlateportCity_OceanicMuseum_2F`
- **Mechanics (in this order):**
  1. Talk to Capt. Stern (`SCIENTIST_1`, `LOCALID_OCEANIC_MUSEUM_2F_CAPT_STERN`). He recognises the parts.
  2. Aqua music. Grunt 1, then grunt 2, walk in from the stairs (x6,y1) and corner the player and Stern.
  3. Back-to-back forced battles, no intro: `TRAINER_GRUNT_MUSEUM_1` (Carvanha L15),
     then `TRAINER_GRUNT_MUSEUM_2` (Zubat L14, Carvanha L14). Class "Team Aqua", pic Aqua Grunt M.
  4. Archie (`OBJ_EVENT_GFX_ARCHIE`) comes down the stairs, the grunts step aside, he walks up to the player and gives his speech.
     **No battle with Archie.**
  5. Fade to black: Archie and both grunts are removed. `FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_AQUA_GRUNTS`
     (the 1F grunts are gone too).
  6. Stern thanks the player. `ITEM_DEVON_PARTS` is handed over (`Common_EventScript_PlayerHandedOverTheItem`).
     Stern says he must leave for his ocean-floor expedition. Fade, heal jingle, the **party is healed**, Stern is removed.
  7. Flags: `FLAG_HIDE_ROUTE_110_TEAM_AQUA`, `VAR_REGISTER_BIRCH_STATE = 1`,
     `FLAG_DELIVERED_DEVON_GOODS`, `clearflag FLAG_HIDE_ROUTE_116_DEVON_EMPLOYEE` (a Devon employee with a
     Repeat Ball now appears on Route 116), `setflag FLAG_HIDE_RUSTBORO_CITY_DEVON_CORP_3F_EMPLOYEE`,
     `VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE = 1`.
- **Original text (gist):** STERN: "If you're looking for STERN, that would be me. Ah! The parts I ordered from
  MR. STONE of DEVON!" GRUNT: "Hold it! We'll take those parts! We're TEAM AQUA! Our BOSS wants them!"
  After losing: "The BOSS is going to be furious…" / "Arrgh, meddled with by some meddling kid!"
  ARCHIE: "I came to see what was taking so long… We are TEAM AQUA, and we love the sea! I am ARCHIE.
  …You're not one of TEAM MAGMA? You're not dressed for the part. … All life depends on the sea, so TEAM AQUA
  is dedicated to the expansion of the sea. You're too young to understand. Don't interfere again—the
  consequences will cost you dearly!" STERN: "Thank you for saving us! … We have to set out on our
  ocean-floor expedition really soon."
- **Characters:** Capt. Stern (scientist, ocean explorer); Archie (villain leader A, unique sprite + `Aqua Leader` trainer pic, not fought here); 2 grunts.
- **HC:** two grunts try to seize the parts the player is delivering. The player beats both
  one after the other. **The boss of the same organisation walks in, gives a speech, threatens the player
  and leaves without fighting.** He mistakes the player for a member of a *second, rival* faction ("not dressed for
  the part"). That faction has not been introduced yet. The delivery is completed, then Stern leaves for an
  undersea expedition (this sets up a later segment: the "submarine" and the "seafloor cavern").

### B06 — Outside the museum: Scott introduces himself [M, automatic]
- **Map:** `SlateportCity`
- **Mechanics:** OnTransition places Scott (`OBJ_EVENT_GFX_SCOTT`) next to the player. OnFrame
  (`VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE == 1`) runs `SlateportCity_EventScript_ScottScene`: Scott walks up and talks,
  registers himself in the PokéNav (fanfare, `FLAG_ENABLE_SCOTT_MATCH_CALL`) and walks away. Then
  `VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE = 2`, `VAR_SCOTT_STATE += 1`, and Scott is moved to the Battle Tent door (x10,y12).
- **Original text (gist):** SCOTT: "I'm sure I met you before… my name's SCOTT. I just saw TEAM AQUA run away
  from here like they were stung. Let me guess—you drove them away? … Maybe, just maybe, this TRAINER… Let's
  register each other in our POKéNAVS. … I'll be off to roam other towns. Be seeing you!"
- **Characters:** Scott (talent scout; Battle Frontier recruiter later).
- **HC:** a friendly talent scout witnessed the aftermath. He registers his contact and keeps
  appearing near Battle Tents. His "secret" (he recruits strong trainers) must stay compatible with the later Frontier.

### B06b — Scott leaves the Slateport Battle Tent [O]
- **Map:** `SlateportCity`. Coord trigger at x10,y13 (just below the tent door) with `VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE == 2`.
  Scott comes out of the tent, pushes the player down one tile, cheers them on and leaves. State becomes 3, `VAR_SCOTT_STATE += 1`.
  If Wattson is beaten first, the state is forced to 3 and the scene never fires.
- **Gist:** "Let me guess—you're going to take the BATTLE TENT challenge? A tough TRAINER is the perfect fit!"

### B07 — Route 110 (south): the grunt barrier is gone; Prof. Birch registers [M]
- **Map:** `Route110`
- **Mechanics:** **before B05**, 5 Aqua grunts (`FLAG_HIDE_ROUTE_110_TEAM_AQUA`, x7–10,y82–83) block the
  north exit of Slateport (lines: "TEAM AQUA's activities… we can't talk about them yet", "I want to get going
  to SLATEPORT and kick up a ruckus!", "My first job after joining TEAM AQUA", "TEAM AQUA's actions should bring
  smiles to people's faces!"). After B05 they are hidden. Coord trigger x7–10,y85
  (`VAR_REGISTER_BIRCH_STATE == 1`): Prof. Birch (`OBJ_EVENT_GFX_PROF_BIRCH`) walks in, asks where his child (the
  rival) is, registers in the PokéNav (`FLAG_ENABLE_PROF_BIRCH_MATCH_CALL`, so the Pokédex can be rated remotely)
  and leaves. `VAR_REGISTER_BIRCH_STATE = 2`.
  Vandalised sign: "TEAM AQUA was here!" painted over with "TEAM MAGMA rules!" (the first hint of the second faction).
- **Original text (gist):** BIRCH: "Imagine seeing you here! And where might my {RIVAL} be? … You two are running
  separately. … I heard your POKéNAV has MATCH CALL; I should register you… Please keep an eye out for my {RIVAL}."
- **Characters:** Prof. Birch; Aqua grunts (gone at this point).
- **HC:** the villains' road block physically closes the route north until the museum is cleared.
  The mentor professor shows up on the route to register a remote contact.
  The graffiti sign reveals two rival gangs.

### B08 — Route 110 (middle): rival battle #2 + Itemfinder [M]
- **Map:** `Route110`
- **Mechanics:** coord trigger x33–35,y56 (`VAR_ROUTE110_STATE == 0`, the whole path width). The rival
  (`OBJ_EVENT_GFX_VAR_0`, May or Brendan depending on the player's gender, rival music) notices the player (!), walks over, gives a one-line
  intro and fights a forced battle: `TRAINER_MAY_ROUTE_110_{TREECKO|TORCHIC|MUDKIP}` /
  `TRAINER_BRENDAN_ROUTE_110_*` chosen by `VAR_STARTER_MON` (e.g. Wingull L18, Lombre L18, Combusken L20).
  Afterwards: `giveitem ITEM_DOWSING_MACHINE` (the "ITEMFINDER"). The rival swaps to the bike sprite
  (`LOCALID_ROUTE110_RIVAL_ON_BIKE`) and rides north. `VAR_ROUTE110_STATE = 1`.
- **Original text (gist):** MAY: "Long time no see! My POKéMON grew stronger. How about a little battle?" /
  "You're better than I expected! … You deserve a reward! That's an ITEMFINDER… train harder for next time."
  BRENDAN: "So this is where you were. I'll check your POKéMON." / "You've trained without me noticing… take this.
  I'm off to look for new POKéMON."
- **Characters:** the rival (Birch's child); its sprite varies with the player's gender.
- **HC:** the rival ambushes the player on the road between the two cities, battles, hands over a
  detection gadget and rides off northwards on a bike.

### B09 — Mauville: Wally wants to challenge the gym; battle with Wally [M]
- **Map:** `MauvilleCity`
- **Mechanics:** Wally (`OBJ_EVENT_GFX_WALLY`, x8,y6) and his Uncle (`OBJ_EVENT_GFX_POKEFAN_M`, x9,y6)
  stand **directly in front of the gym door** (x8,y5). Talking to Wally: an argument with the uncle, then Wally asks the player
  for a battle (YES/NO). NO sets `FLAG_DECLINED_WALLY_BATTLE_MAUVILLE` (the Uncle then asks you to battle him "just
  this once"). YES: `trainerbattle_no_intro TRAINER_WALLY_MAUVILLE` (Ralts L16). After the battle: Wally says he will
  go back to Verdanturf, the uncle comforts him, invites the player to Verdanturf, and both walk off.
  Flags: `FLAG_DEFEATED_WALLY_MAUVILLE`, `clearflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY` / `_WALLYS_UNCLE`,
  `VAR_WALLY_CALL_STEP_COUNTER = 0`, `FLAG_ENABLE_FIRST_WALLY_POKENAV_CALL`.
  Then **Scott** (`LOCALID_MAUVILLE_SCOTT`, added on the spot) walks up and comments, `VAR_SCOTT_STATE += 1`, and leaves.
- **Original text (gist):** WALLY: "UNCLE, please? I want to challenge this GYM." UNCLE: "Don't you think you're pushing
  it?" WALLY: "If I combine forces with RALTS, we can beat anyone! … {PLAYER}, will you battle me?" After the loss:
  "I'll go back to VERDANTURF… Being a TRAINER is tough. It's not enough just to have POKéMON and make them battle."
  UNCLE: "No need to be so down. Come on, let's go home." / "You must be the TRAINER who helped WALLY catch his POKéMON.
  Visit us in VERDANTURF." SCOTT: "I was watching that match! You didn't hold anything back… That's what a real
  battle is about! I'll be cheering for you!"
- **Characters:** Wally (frail boy, friend, sickly), Wally's Uncle (POKEFAN_M, **same sprite as Wally's father**), Scott.
- **HC:** the fragile friend and his guardian **block the gym door** until the player beats the friend
  (who has a single Ralts-line Pokémon). The friend loses and retreats to the green town to the west. A watcher (Scott) praises the player.

### B10 — Wally's first PokéNav call [M, automatic]
- **Mechanics:** C code `ShouldDoWallyCall` (src/field_specials.c) counts **250 steps** on outdoor maps after B09,
  then runs `MauvilleCity_EventScript_RegisterWallyCall`. Wally registers (`FLAG_ENABLE_WALLY_MATCH_CALL`).
- **Gist:** WALLY: "My uncle bought me a POKéNAV! Now I can get in touch with you anytime!"
- **HC:** the friend's call happens a little after B09, wherever the player happens to be.

### B11 — Mauville Gym: Wattson, Dynamo Badge [M]
- **Map:** `MauvilleCity_Gym`
- **Mechanics:** gym puzzle with floor switches that toggle electric barriers (`VAR_MAUVILLE_GYM_STATE`,
  `FLAG_MAUVILLE_GYM_BARRIERS_STATE`; both are reset in Mauville's OnTransition). Gym trainers: `TRAINER_KIRK`, `_SHAWN`,
  `_BEN`, `_VIVIAN`, `_ANGELO`. Leader `TRAINER_WATTSON_1` (Voltorb 20, Electrike 20, Magneton 22, Manectric 24).
  On victory: `FLAG_BADGE03_GET`, `FLAG_DEFEATED_MAUVILLE_GYM`, `VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE = 3`,
  `clearflag FLAG_HIDE_VERDANTURF_TOWN_SCOTT` (Scott now in the Verdanturf tent), `VAR_PETALBURG_GYM_STATE += 1`,
  gym trainers set as beaten, the puzzle is turned off, `ITEM_TM_SHOCK_WAVE`, Wattson is registered (`FLAG_ENABLE_WATTSON_MATCH_CALL`).
- **Original text (gist):** WATTSON: "I've given up on my plans to convert the city, I have. So I put my time into
  door traps in my GYM. … I, WATTSON, shall electrify you!" / "Wahahahah! Fine, I lost! You gave me a thrill!" /
  "With the DYNAMO BADGE, POKéMON can use ROCK SMASH out of battle."
- **Characters:** Wattson (Electric leader, jolly old man, `OBJ_EVENT_GFX_WATTSON`), gym guide.
- **HC:** an electric gym with switch-toggled barriers. The leader once had a failed city-renewal plan
  (this ties into New Mauville, B34). The badge enables Rock Smash.

### B12 — Mauville: the Rock Smash "dude" [M] and the bike [S]
- **Maps:** `MauvilleCity_House1`, `MauvilleCity_BikeShop`
- **Mechanics:** the Rock Smash Dude (`SCIENTIST_1`) gives `ITEM_HM_ROCK_SMASH` (`FLAG_RECEIVED_HM_ROCK_SMASH`).
  This **also hides the "tip guy" on Route 111** (`FLAG_HIDE_ROUTE_111_ROCK_SMASH_TIP_GUY`), a man who stands among the rocks
  saying his uncle in Mauville told him to bring Rock Smash. Rydel (`MAN_2`, Bike Shop): "Did you come from far
  away?" YES gives a choice of `ITEM_MACH_BIKE` or `ITEM_ACRO_BIKE` (they can be swapped later). A bike is needed for Cycling Road,
  Mirage Tower (Mach) and climbing Jagged Pass (Acro). No bike is needed on the critical path of this segment.
- **Gist:** "People call me the ROCK SMASH GUY, but I deserve more respect—the ROCK SMASH DUDE! Woohoo! Take this HM."
- **HC:** the field move that unblocks Route 111 is given by a Mauville resident right after the badge.

### B13 — Route 117 → Verdanturf: Wally's new resolve [O, narratively important]
- **Maps:** `Route117`, `VerdanturfTown`, `VerdanturfTown_WandasHouse`, `VerdanturfTown_BattleTentLobby`
- **Mechanics:** in Wanda's house: Wally (first talk sets `FLAG_WALLY_SPEECH`), Uncle, Aunt (`POKEFAN_F`), and
  (after B14) Wanda + her boyfriend. Uncle/Aunt lines change with `FLAG_RUSTURF_TUNNEL_OPENED`,
  `FLAG_DEFEATED_LAVARIDGE_GYM` ("WALLY's gone away… he slipped off on his own"), and later Victory Road.
  Scott is in the Verdanturf Battle Tent from B11 until the player first enters Fiery Path (B19): `FLAG_MET_SCOTT_IN_VERDANTURF`, `VAR_SCOTT_STATE += 1`.
  Route 117 has 9 trainers and the Pokémon Day Care (the Day-Care man moves to the fence when an egg is ready).
- **Original text (gist):** WALLY: "I lost to you, but I'm not feeling down anymore. I have a new purpose: together with
  RALTS I'll challenge GYMS and become a great TRAINER. Please watch me. I'm going to be stronger than you, and then I'll
  challenge you again." UNCLE: "This environment is doing wonders for WALLY's health… maybe it's POKéMON giving him hope."
  WANDA: "I'm WALLY's cousin."
- **HC:** after his defeat the friend recovers in his relatives' house and promises a rematch.
  **After badge 4 he secretly leaves** (`FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY` is set by Flannery's script). This prepares his Victory Road appearance.

### B14 — Rusturf Tunnel: the lovers reunited, HM Strength [O/S]
- **Map:** `RusturfTunnel` (reached from Verdanturf)
- **Mechanics:** Wanda (`WOMAN_2`) and her boyfriend (`BLACK_BELT`) are on opposite sides of one or two breakable rocks
  (`FLAG_HIDE_RUSTURF_TUNNEL_ROCK_1/2`). When the player smashes one, the C function `TryUpdateRusturfTunnelState` sets
  `VAR_RUSTURF_TUNNEL_STATE` to 4 or 5, and OnFrame runs `RusturfTunnel_EventScript_ClearTunnelScene`: the boyfriend
  thanks the player and gives `ITEM_HM_STRENGTH` (`FLAG_RECEIVED_HM_STRENGTH`), the couple reunite and leave.
  `RusturfTunnel_EventScript_SetRusturfTunnelOpen` sets `FLAG_RUSTURF_TUNNEL_OPENED` and `VAR_RUSTURF_TUNNEL_STATE = 6`
  and moves them into Wanda's house. Strength is only usable with badge 4 and is needed in later segments.
- **Gist:** WANDA: "My boyfriend is on the other side of this rock. He works his hands raw for everyone." BOYFRIEND: "Why can't
  they keep digging? My beloved awaits in VERDANTURF…" / "You shattered that boulder! Take this HM." / "WANDA! Now I can see you anytime!"
- **HC:** a man digging by hand to reach his beloved. The player's Rock Smash completes the tunnel
  (Rustboro ↔ Verdanturf shortcut). The reward is the Strength HM.

### B15 — Route 118 (west bank) [O]
- **Map:** `Route118`
- **Mechanics:** the Good Rod fisherman (`ITEM_GOOD_ROD` after a YES/NO, `FLAG_RECEIVED_GOOD_ROD`), 7 trainers on the
  west bank, and a girl hinting that Surf crosses rivers. Gabby & Ty's 2nd interview spot (x33–34,y8) is used only after
  their first battle (B17). **Steven's scene (coord x43–45,y11, `VAR_ROUTE118_STATE`) is on the east bank. It needs Surf, so it belongs to the next segment.**
- **HC:** the river blocks the way east until Surf.

### B16 — Route 111 (south): rocks, Winstrates, sandstorm [M for rocks, O for the rest]
- **Map:** `Route111`, `Route111_WinstrateFamilysHouse`
- **Mechanics:**
  - Breakable rocks (x18–19,y100–101) need Rock Smash.
  - **Winstrates** [O]: Victor (`MAN_1`, x13,y114 at the house door) offers "a series of battles with our family of four".
    YES starts 4 consecutive forced battles with door animations: `TRAINER_VICTOR`, `TRAINER_VICTORIA`, `TRAINER_VIVI`,
    `TRAINER_VICKY` (class "Winstrate"). Each member walks out of the house in turn. Inside the house: `ITEM_MACHO_BRACE`
    (`FLAG_RECEIVED_MACHO_BRACE`). OnTransition resets them if Vicky is not yet beaten.
  - **Desert** [locked]: the coord triggers `Route111_EventScript_ViciousSandstormTrigger*` push the player back
    with `gText_SandstormIsVicious` unless the player has `ITEM_GO_GOGGLES`. The Hiker explains the Mirage Tower ("seen sometimes,
    sometimes not"). The Desert Ruins door is walled shut (`FLAG_REGI_DOORS_OPENED`, postgame Regi puzzle).
  - Trainer Hill entrance (x31,y113) is closed until the game is cleared. Old Lady's Rest Stop (heal). Secret Power man
    (`ITEM_TM_SECRET_POWER`). Berry girl (daily Razz Berry).
- **Gist:** VICTOR: "What do you say to taking on our family of four?" VICKY (grandma): "How dare you make my granddaughter cry!"
- **HC:** a desert in the middle of the route that cannot be entered without goggles. A family of four
  that fights the player in sequence at their door.

### B17 — Route 111: Gabby & Ty's first interview [O]
- **Map:** `Route111` (x13–14,y86, north of the rocks)
- **Mechanics:** a double battle against `TRAINER_GABBY_AND_TY_1` (class "Interviewer", Magnemite 17 + Whismur 17), then the
  first TV interview (`GabbyAndTy_EventScript_FirstInterview`, data/scripts/gabby_and_ty.inc). `GabbyAndTy_EventScript_UpdateLocation`
  then rotates the pair across Route 111 / 118 / 120 for later rematches.
- **Gist:** GABBY: "Oh! We've just spotted a tough-looking TRAINER! Okay, roll camera! Let's get this interview."
- **Characters:** Gabby (`REPORTER_F`), Ty (`CAMERAMAN`), the same TV crew that films Stern in a later segment.

### B18 — Route 112: villain faction B blocks the cable car [M]
- **Map:** `Route112`
- **Mechanics:** two Team Magma grunts (`OBJ_EVENT_GFX_MAGMA_MEMBER_M`, x26–27,y30,
  `FLAG_HIDE_ROUTE_112_TEAM_MAGMA`) stand in front of the Cable Car station (warps x28–29,y27). Talking to either
  runs a 4-line dialogue between them (no battle). The player must take Fiery Path (warp x11,y36).
- **Original text (gist):** "Is our leader really going to awaken *that thing*?" / "Sounds like it. But I heard we need a
  METEORITE to do it." / "That's why the rest of the crew went to FALLARBOR." / "Until they come back, we're not to let anyone pass."
- **HC:** **first on-screen appearance of the second villain group.** Two grunts guard the cable car
  and openly mention 1) a leader who wants to awaken "that thing" (in the mountain), 2) the need for a meteorite,
  3) a squad sent to the town of Fallarbor. This is the hook that sends the player north-west.

### B19 — Fiery Path [M, pass-through]
- **Map:** `FieryPath`
- **Mechanics:** the cave links south Route 112 with north Route 112 (which then connects up to Route 113). On first entry
  (`FLAG_LANDMARK_FIERY_PATH` not yet set) it **moves Scott** from the Verdanturf tent to the Fallarbor tent
  (`setflag FLAG_HIDE_VERDANTURF_TOWN_SCOTT`, `clearflag FLAG_HIDE_FALLARBOR_TOWN_BATTLE_TENT_SCOTT`). Six Strength
  boulders (optional item area, TM). No trainers.
- **HC:** none for the story (it is a hot tunnel bypassing the blockade).

### B20 — Route 113: the ash road [M, pass-through]
- **Maps:** `Route113`, `Route113_GlassWorkshop`
- **Mechanics:** volcanic-ash weather (`WEATHER_VOLCANIC_ASH`, step callback `STEP_CB_ASH`) between x19 and x84. 10 trainers.
  The Glass Workshop gives `ITEM_SOOT_SACK` (collect ash → glass flutes and decorations) [O].
- **Gist:** GENTLEMAN: "Ash can be fashioned into glass." GLASSMAN: "I make glass out of volcanic ash, huff-puff."
- **HC:** ash constantly falls from the volcano (Mt. Chimney), which is visible from here.

### B21 — Fallarbor Town: Prof. Cozmo is missing [M as the narrative hook]
- **Maps:** `FallarborTown`, `FallarborTown_CozmosHouse`, `FallarborTown_BattleTentLobby`
- **Mechanics:** Cozmo is hidden at home (`FLAG_HIDE_FALLARBOR_HOUSE_PROF_COZMO`, set at new game); only his wife is there.
  The ExpertM in town gives a different line before and after `FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY`. Scott in the Fallarbor
  tent [O] (`FLAG_MET_SCOTT_IN_FALLARBOR`, `VAR_SCOTT_STATE += 1`) talks about looking for someone "bursting with the desire to win".
  The Meteor Falls scene (B22) removes him.
- **Original text (gist):** EXPERT: "I've seen shady characters wandering in and out of PROF. COZMO's home…"
  WIFE: "PROF. COZMO went off to METEOR FALLS on ROUTE 114 with some people from TEAM MAGMA."
- **Characters:** Cozmo's wife (`WOMAN_2`), townsfolk, Scott.
- **HC:** a scientist who studies meteorites was taken by the group-B grunts to the waterfall cave.

### B22 — Meteor Falls: the Meteorite is stolen; both factions meet [M]
- **Maps:** `Route114` (the way there), `MeteorFalls_1F_1R`
- **Mechanics:** coord trigger x14,y18 (`VAR_METEOR_FALLS_STATE == 0`), a few steps inside from the Route 114 entrance (x27,y18).
  Magma music. The player turns south. Magma grunt 1 (x12,y20) gloats about the meteorite, then both Magma grunts
  notice the player (!). Grunt 1 threatens the player. A voice calls "Hold it right there, TEAM MAGMA!". **Archie + 2 Aqua grunts**
  come in from the west (Aqua music). The Magma grunts taunt them and **push the player aside** as they run out east,
  carrying the Meteorite. Archie walks up to the player, explains the rivalry, his grunts urge a chase, Archie says goodbye and they leave.
  **No battles.** Flags: `FLAG_HIDE_ROUTE_112_TEAM_MAGMA` (the cable car is now free),
  `FLAG_MET_ARCHIE_METEOR_FALLS`, `FLAG_HIDE_FALLARBOR_TOWN_BATTLE_TENT_SCOTT`, `VAR_METEOR_FALLS_STATE = 1`.
  Prof. Cozmo (`SCIENTIST_1`, x13,y23, `FLAG_HIDE_METEOR_FALLS_1F_1R_COZMO`) stays in the cave and can be talked to (`FLAG_MET_PROF_COZMO`).
- **Original text (gist):** MAGMA GRUNT: "Hehehe! With this METEORITE, that thing in MT. CHIMNEY will…" / "If you get in the way of
  TEAM MAGMA, don't expect any mercy!" ARCHIE (off-screen): "Hold it right there, TEAM MAGMA! You're badly mistaken if you think you
  can have your way with the world!" MAGMA: "Even TEAM AQUA joins us! … We've got the METEORITE, so off to MT. CHIMNEY we go! Be seeing
  you, TEAM AQUA dingbats!" ARCHIE: "Didn't I see you at SLATEPORT's MUSEUM? I thought you were one of TEAM MAGMA's goons. TEAM MAGMA
  is a dangerous group of total fanatics who destroy things, claiming to expand the land mass. They are the rivals of the sea-loving
  TEAM AQUA!" GRUNT: "BOSS, we should give chase!" ARCHIE: "There's no telling what they'll do at MT. CHIMNEY! … Keep an eye out
  for TEAM MAGMA too. Farewell!" COZMO: "TEAM MAGMA asked me to guide them to METEOR FALLS… but they tricked me and took my METEORITE…
  What are they going to do with it at MT. CHIMNEY?"
- **Characters:** 2 Magma grunts, Archie + 2 Aqua grunts, Prof. Cozmo (`SCIENTIST_1`).
- **HC:** **group B steals a meteorite in the waterfall cave and runs off towards the volcano.**
  **Group A's leader arrives too late,** explains that the two groups are enemies and sets off in pursuit.
  The kidnapped/tricked scientist stays behind, shaken. The player is shoved aside and does not fight.

### B23 — Back to Route 112: the cable car to the summit [M]
- **Maps:** `Route112`, `Route112_CableCarStation`, `MtChimney_CableCarStation`
- **Mechanics:** the grunts are gone. The attendant: "The CABLE CAR is ready to go up. Would you like to be on it?"
  The ride sets `VAR_CABLE_CAR_STATION_STATE = 1`, runs `special CableCarWarp` + `special CableCar` (the cable-car animation),
  and the player arrives at the top station (OnFrame exit). The Mt. Chimney trainers are hidden (`FLAG_HIDE_MT_CHIMNEY_TRAINERS`) until
  after B24. Lavaridge's OnTransition clears the flag once Chimney is done.

### B24 — Mt. Chimney summit: the two factions brawl; Maxie at the machine [M]
- **Map:** `MtChimney`
- **Mechanics:**
  - The summit is a pitched brawl: Magma and Aqua grunts stand in pairs, each with a Poochyena facing an enemy Poochyena
    (sign-type talk only: "MAGMA outnumbers us!", "If they expand the land, there'll be less habitat for WATER POKéMON!",
    "It burns me up that they'd use such a confusing name!", "METEORITES pack amazing power!", "We're going to keep making more land!").
  - Archie (x24,y19) is fighting 3 opponents at once. Talking to him [O] sets `FLAG_EVIL_LEADER_PLEASE_STOP` ("See for yourself what the
    fanatics are up to! They're trying to inject the stolen METEORITE's power into the volcano! That will cause an eruption!").
  - Group B **trainers** in the way (sight trainers): `TRAINER_GRUNT_MT_CHIMNEY_1` (female, Numel 20),
    `TRAINER_GRUNT_MT_CHIMNEY_2` (Zubat 20), and **admin Tabitha** `TRAINER_TABITHA_MT_CHIMNEY` (class "Magma Admin", battle
    pic Magma Admin, **overworld sprite = generic `MAGMA_MEMBER_M`**; Numel 18, Poochyena 20, Numel 22, Zubat 22).
  - Maxie (`OBJ_EVENT_GFX_MAXIE`, x13,y6) stands **at the Meteorite machine**. Talking to him: monologue, notices the player (!),
    speech, then a forced battle `TRAINER_MAXIE_MT_CHIMNEY` (Mightyena 24, Zubat 24, Camerupt 25). After the loss he says he'll back
    off this time. Fade: Maxie, Tabitha and both grunt trainers are removed, `FLAG_HIDE_MT_CHIMNEY_TEAM_MAGMA`. Archie walks
    over to the player, thanks them, wonders "whose side are you on?" and leaves. Flags: `FLAG_HIDE_MT_CHIMNEY_TEAM_AQUA`,
    `FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY`, `clearflag FLAG_HIDE_FALLARBOR_HOUSE_PROF_COZMO`,
    `setflag FLAG_HIDE_METEOR_FALLS_1F_1R_COZMO` (Cozmo goes home), `clearflag FLAG_HIDE_MT_CHIMNEY_LAVA_COOKIE_LADY`.
  - The Meteorite machine: while the event is active, "A METEORITE is fitted on a mysterious machine… storing energy". Afterwards the player
    can take it out (YES/NO → `ITEM_METEORITE`, `FLAG_RECEIVED_METEORITE`), after which the machine "makes no response whatsoever".
- **Original text (gist):** MAXIE (to himself): "By amplifying the METEORITE's power with this machine, MT. CHIMNEY's volcanic activity will
  instantly intensify… its energy will grow deep inside the crater…" MAXIE: "I'd heard ARCHIE bemoaning a child meddling in TEAM AQUA's
  affairs. So you'd interfere with TEAM MAGMA? Long ago, living things used the land to live and grow. Land is the cradle of all!
  TEAM MAGMA is dedicated to the expansion of the land mass, for humankind and POKéMON. For that we need the power of what sleeps within this
  mountain… I'll teach you the consequences of meddling!" / "I, MAXIE, was caught off guard?!" / "I will back off this time. But even
  without the METEORITE, if we obtain that ORB… Fufufu…" TABITHA: "You're too late! I've already delivered the METEORITE to the BOSS!" /
  "If our leader awakens that thing…" / "BOSS, hurry! Give it the METEORITE's energy!" GRUNT: "If that thing's power made more land, there'd be
  more places to live!" / "I'd build a big house on hardened lava!" ARCHIE: "Thank you! With your help we thwarted TEAM MAGMA's destructive
  plan! But… whose side are you on? … We shall meet again!"
- **Characters:** Maxie (villain leader B, unique sprite), Tabitha (admin, generic grunt sprite), Magma and Aqua grunts, Archie.
- **HC:** both factions brawl at the crater (pairs of Poochyena facing each other). Group A's leader is tied up
  "fighting 3 opponents". The player beats 2 grunts and the admin and reaches **group B's leader, who stands at a machine at the crater
  fed by the stolen meteorite.** The leader is beaten and withdraws with his whole team. **No legendary awakens here**: the plan simply
  fails. He hints at a backup plan (an "ORB"). Group A's leader thanks the player, is suspicious of them, and leaves. The meteorite stays in the
  machine and the player may take it.

### B25 — Cozmo gets the Meteorite back [O]
- **Map:** `FallarborTown_CozmosHouse`
- **Mechanics:** Cozmo is home now (sad if the player has no Meteorite). With `ITEM_METEORITE` he asks for it (YES/NO; NO →
  "crushed with disappointment", and he asks again next time). The trade gives `ITEM_TM_RETURN` and takes the Meteorite (`FLAG_RECEIVED_TM_RETURN`).
- **Gist:** COZMO: "I never should have let myself be conned into telling TEAM MAGMA where to find METEORITES… Is that the METEORITE they took?
  May I have it? How about this TM in exchange?"
- **HC:** the scientist regrets helping the villains and swaps a TM for the recovered meteorite.

### B26 — Jagged Pass: descent and a suspicious lookout [M path, O battle]
- **Map:** `JaggedPass` (from the Mt. Chimney south exit x20–21,y41 down to Route 112 x6–7,y46)
- **Mechanics:** ash weather (`VAR_JAGGED_PASS_ASH_WEATHER`). Five trainers (`TRAINER_AUTUMN`, `_DIANA_1`, `_ERIC`, `_ETHAN_1`, `_JULIO`).
  A lone Magma grunt (`LOCALID_MAGMA_HIDEOUT_GUARD`, x16,y19, visible until `FLAG_HIDE_JAGGED_PASS_MAGMA_GUARD`, which is set in a **later**
  segment at Mt. Pyre) guards a rock wall that is the hidden entrance of the Magma Hideout (OnLoad keeps it closed while `VAR_JAGGED_PASS_STATE <= 1`).
  Talking to him starts a forced battle `TRAINER_GRUNT_JAGGED_PASS` (Poochyena 22, Numel 22) → `FLAG_BEAT_MAGMA_GRUNT_JAGGED_PASS`.
  The Magma Emblem logic (`VAR_JAGGED_PASS_STATE` 1→2, the rock shakes open) belongs to a later segment.
- **Gist:** GRUNT: "Wah! What are you doing here? What am I doing here? What business is it of yours?" / "I should've ducked into our HIDEOUT right away…" /
  "I admit it—you're strong! Go wherever you want!"
- **HC:** a group-B guard hangs around a suspicious rock face on the mountain slope. He lets slip that the group has a hideout here (payoff later).

### B27 — Lavaridge Town [M arrival, O services]
- **Maps:** `LavaridgeTown`, `LavaridgeTown_HerbShop`, PC, Mart
- **Mechanics:** reached from Route 112 (west). OnTransition: `FLAG_VISITED_LAVARIDGE_TOWN`, and `FLAG_HIDE_MT_CHIMNEY_TRAINERS` is cleared if Chimney is done.
  Egg woman [O] (YES/NO, party not full) → `giveegg SPECIES_WYNAUT` (`FLAG_RECEIVED_LAVARIDGE_EGG`). Herb Shop (`ITEM_CHARCOAL` gift + herbal medicines).
  Hot springs (stat `GAME_STAT_ENTERED_HOT_SPRINGS`).
- **Gist:** EGG WOMAN: "I hoped to hatch this EGG in the hot sand, but it's not enough… It's best kept with POKéMON. Will you take it?"

### B28 — Lavaridge Gym: Flannery, Heat Badge [M]
- **Maps:** `LavaridgeTown_Gym_1F`, `LavaridgeTown_Gym_B1F`
- **Mechanics:** puzzle with steam geysers (warps between floors) and trainers **buried in the sand** who pop up when the player is close
  (`MOVEMENT_TYPE_BURIED`): `TRAINER_COLE`, `_GERALD`, `_AXLE`, `_DANIELLE`, `_ELI`, `_JACE`, `_JEFF`, `_KEEGAN`. Leader `TRAINER_FLANNERY_1`
  (Numel 24, Slugma 24, Camerupt 26, Torkoal 29). On victory: `FLAG_BADGE04_GET`, `FLAG_DEFEATED_LAVARIDGE_GYM`,
  `FLAG_WHITEOUT_TO_LAVARIDGE`, `VAR_PETALBURG_GYM_STATE += 1` (→ 6 if this is the 4th badge → `Common_EventScript_ReadyPetalburgGymForBattle`:
  the Petalburg gym greeter appears, Petalburg Mart expands), **`setflag FLAG_HIDE_VERDANTURF_TOWN_WANDAS_HOUSE_WALLY`** (Wally runs away),
  `VAR_LAVARIDGE_TOWN_STATE = 1`, `ITEM_TM_OVERHEAT`, Flannery registered.
- **Original text (gist):** FLANNERY: "Welcome… No, wait. Puny TRAINER… I have been entrusted with the… No, wait. I am FLANNERY, the GYM LEADER!
  Don't underestimate me, though I've been LEADER only a short time! With skills inherited from my grandfather…" / "I was trying too hard to be
  someone I'm not… Thanks for teaching me that." / "HEAT BADGE: POKéMON up to Lv 50 obey, and STRENGTH works outside battle." /
  "You battle like NORMAN, the GYM LEADER of PETALBURG."
- **Characters:** Flannery (new, insecure young leader, granddaughter of the old leader; `OBJ_EVENT_GFX_FLANNERY`).
- **HC:** a fire gym with geysers and buried trainers. The leader is a novice who inherited the role and is acting tougher than she is.

### B29 — The rival gives the Go-Goggles [M]
- **Map:** `LavaridgeTown`
- **Mechanics:** OnFrame `VAR_LAVARIDGE_TOWN_STATE == 1` (fires when the player walks out of the gym into town). If the player is at x9, the rival is already
  in town (x11,y9) and notices them. Otherwise the rival comes out of the Herb Shop door (x12,y15). Rival music, walks over, `ITEM_GO_GOGGLES`
  (`FLAG_RECEIVED_GO_GOGGLES`), switches to the bike sprite and rides off. `VAR_LAVARIDGE_TOWN_STATE = 2`. **No battle.**
- **Original text (gist):** MAY: "While I visited the hot springs, you got the LAVARIDGE GYM BADGE! … I guess it's okay for you to have this. With these
  GO-GOGGLES you'll have no trouble in the desert near ROUTE 111. … I think I should challenge your dad in PETALBURG GYM. See you!"
  BRENDAN: "That's a decent collection of BADGES. You may as well have this. Keep them if you go into that desert near ROUTE 111. … I'm considering
  challenging NORMAN. Unlike you, your dad looks really tough."
- **HC:** right after badge 4 the rival hands over the item that opens the desert and points the player towards the father's gym.

### B30 — Desert and Mirage Tower [O]
- **Maps:** `Route111` (desert), `MirageTower_1F`..`_4F`, `DesertRuins` (sealed)
- **Mechanics:** with the Go-Goggles the sandstorm triggers let the player through (`VAR_TEMP_3 = 1`). The Mirage Tower is visible on each Route 111
  load with a **50% chance** (`SetMirageTowerVisibility`, src/mirage_tower.c). Entering it sets `FLAG_FORCE_MIRAGE_TOWER_VISIBLE`. It needs
  the **Mach Bike** (cracked floors) and Rock Smash. On 4F: choose `ITEM_ROOT_FOSSIL` or `ITEM_CLAW_FOSSIL` (YES/NO, "the ground will likely
  crumble"). Taking one makes the other vanish, the screen shakes, the ceiling crumbles, the player is warped to Route 111 (x19,y59), and OnFrame
  (`VAR_MIRAGE_TOWER_STATE == 1`) plays the player-falling and tower-disintegration animation. The unchosen fossil "disappeared into the sand"
  (it can be found later in the postgame Desert Underpass, `FLAG_HIDE_DESERT_UNDERPASS_FOSSIL`). Fossil revival happens at Devon in Rustboro.
  The Desert Ruins (Regirock) stay sealed (postgame Braille puzzle).
- **HC:** a sand tower that appears and disappears. Taking the fossil makes it collapse for good.

### B31 — Petalburg Gym: battle with the father, Balance Badge [M]
- **Map:** `PetalburgCity_Gym`
- **Mechanics:** Norman (`OBJ_EVENT_GFX_NORMAN`) is at the back of the gym once `VAR_PETALBURG_GYM_STATE >= 6` (with fewer badges he stands at
  the entrance and says "you've gotten stronger"). Puzzle: rooms whose doors open when the trainer inside is beaten. Rooms are named by the item each trainer uses:
  Speed (`TRAINER_RANDALL`), Accuracy (`_MARY`), Confusion (`_PARKER`), Defense (`_ALEXIA`), Recovery (`_GEORGE`), "Strength" (`_JODY`),
  "OHKO" (`_BERKE`); per a source comment, the last two are really critical-hit rooms. Battle: `TRAINER_NORMAN_1` (Spinda 27, Vigoroth 27, Linoone 29, Slaking 31).
  On victory: `FLAG_DEFEATED_PETALBURG_GYM`, `VAR_PETALBURG_GYM_STATE = 7`, `FLAG_BADGE05_GET`, `special ResetHealLocationFromDewford`,
  **`EventScript_HideMrBriney`** (Briney and his boat leave Dewford, Route 104 and Route 109; `VAR_BRINEY_LOCATION = 0`), **`setflag FLAG_HIDE_MAUVILLE_GYM_WATTSON`,
  `clearflag FLAG_HIDE_MAUVILLE_CITY_WATTSON`** (Wattson now waits in the street, B33), `clearflag FLAG_HIDE_DEWFORD_HALL_SLUDGE_BOMB_MAN`, all doors open,
  `ITEM_TM_FACADE`.
- **Original text (gist):** DAD: "So, you did get four GYM BADGES. As I promised, we will battle. I'm so happy I can have a real battle with my own child.
  But a battle is a battle!" / "I… can't believe it. I lost to {PLAYER}? But rules are rules!" / "With that BADGE… POKéMON that know SURF can travel over
  water." / "As GYM LEADER, I'm upset. As a father, I'm both happy and a little sad."
- **Characters:** Norman (the player's father, Normal-type leader).
- **HC:** the 5th leader is the protagonist's father. He only accepts the fight after 4 badges (a promise made in segment A). The fight is emotional
  (father vs child). The badge enables Surf. **In the new hack the protagonist is a new character (red spiky hair, black modern clothes), but this
  map and script are built on "the leader is the father". The relationship can be renamed (e.g. a guardian or mentor), but the leader must be someone
  personally close to the player who promised a fight after 4 badges.**

### B32 — Wally's father: HM Surf [M]
- **Maps:** `PetalburgCity_Gym` → `PetalburgCity` → `PetalburgCity_WallysHouse`
- **Mechanics:** straight after B31: door sound, Wally's father (`OBJ_EVENT_GFX_POKEFAN_M`, `LOCALID_PETALBURG_GYM_WALLYS_DAD`) walks in, notices the player (!),
  asks the player to come with him, asks Norman to "borrow" the player, and leads them out ("follow me" music). Then `VAR_PETALBURG_CITY_STATE = 4`,
  `clearflag FLAG_HIDE_PETALBURG_CITY_WALLYS_DAD`, warp to Petalburg. OnFrame `PetalburgCity_EventScript_WalkToWallyHouse` walks both to Wally's house.
  Inside, OnFrame (state 4): speech → `ITEM_HM_SURF` (`FLAG_RECEIVED_HM_SURF`) → `VAR_PETALBURG_CITY_STATE = 5`.
  Wally's mother (`WOMAN_4`) then **reveals that Wally left Verdanturf without telling anyone**.
- **Original text (gist):** WALLY'S DAD: "Please come with me. I have something I want you to have." / "NORMAN, let me borrow your {PLAYER} for a minute." /
  "Our WALLY's become very healthy since he went to VERDANTURF. We owe it all to you! You helped him catch a POKéMON… It made me, his father, happy too.
  This isn't a bribe, but I'd really like you to have this." / "If your POKéMON can SURF, you'll be able to go to all sorts of places." WALLY'S MOM:
  "Keep this a secret from my husband… our WALLY left VERDANTURF TOWN without telling anyone. He's frail, but surprisingly strong-willed."
- **Characters:** Wally's father (POKEFAN_M, same sprite as the Uncle), Wally's mother.
- **HC:** the parent of the player's friend interrupts right after the father fight, escorts the player home and gives Surf out of gratitude. The mother hints at
  the friend's secret departure. **Surf is the end gate of the segment.** It opens Routes 107/108 (the Abandoned Ship), the Route 118 crossing, New Mauville and more.

### B33 — Wattson's favour: New Mauville [O, available after B32]
- **Maps:** `MauvilleCity`, `Route110` (Surf to the entrance at x35,y24), `NewMauville_Entrance`, `NewMauville_Inside`
- **Mechanics:** Wattson is now in the street (x29,y9). First talk: `ITEM_BASEMENT_KEY` (`FLAG_GOT_BASEMENT_KEY_FROM_WATTSON`). Locked door → "Use the
  BASEMENT KEY?" → `VAR_NEW_MAUVILLE_STATE = 1`. Inside: blue and green floor buttons toggle barriers (`VAR_TEMP_1/2`), three Voltorb objects (static Lv 25 battles,
  `FLAG_DEFEATED_VOLTORB_1..3_NEW_MAUVILLE`), the generator ("radiating heat… should be turned off"). The red switch: "The generator appears to have stopped…"
  → `VAR_NEW_MAUVILLE_STATE = 2`. Back to Wattson: `ITEM_TM_THUNDERBOLT` (`FLAG_GOT_TM_THUNDERBOLT_FROM_WATTSON`). From then on Mauville's OnTransition sends Wattson
  back into the gym (`FLAG_WATTSON_REMATCH_AVAILABLE`).
- **Original text (gist):** WATTSON: "MAUVILLE has an underground sector called NEW MAUVILLE. I'd like you to switch off the GENERATOR. It's been running haywire;
  it's getting unsafe. Here's the KEY. … The entrance is a short SURF away from ROUTE 110." / "I knew I'd made the right choice! Take THUNDERBOLT!"
- **HC:** an abandoned underground city sector whose generator has gone wild. The leader trusts the player to switch it off.

### Boundary — next segment
- Route 118 east bank: Steven jumps down a ledge (`Route118_EventScript_StevenTrigger*`, `VAR_ROUTE118_STATE` 0→1) and talks about raising many kinds of Pokémon.
  Needs Surf, so it opens segment C.
- Routes 107/108 (Surf trainers, `AbandonedShip_*` on Route 108) also need Surf.

---

## 2. Content on these maps that belongs to LATER segments (do not write it here, but keep it consistent)

| Map | Later event | Trigger |
|---|---|---|
| `SlateportCity` + `SlateportCity_Harbor` | Stern's TV interview with Gabby & Ty; group A announces by megaphone that they will take Stern's submarine; Archie steals the sub at the harbour | `VAR_SLATEPORT_CITY_STATE = 1` and `VAR_SLATEPORT_HARBOR_STATE = 1`, both set in `MagmaHideout_4F` |
| `SlateportCity_SternsShipyard_1F` | Briney joins Dock (badge 7); the S.S. Tidal is ready (game clear) | `FLAG_BADGE07_GET`, `FLAG_SYS_GAME_CLEAR` |
| `JaggedPass` | The Magma Emblem opens the hideout; the guard disappears | `FLAG_HIDE_JAGGED_PASS_MAGMA_GUARD` (Mt. Pyre summit), `VAR_JAGGED_PASS_STATE` |
| `Route118`, `Route114`, ... | Primal "abnormal weather" events | `VAR_ABNORMAL_WEATHER_LOCATION` (postgame) |
| `MeteorFalls_1F_1R` | Steven's cave | `FLAG_SYS_GAME_CLEAR` |
| `Route110_TrickHouse*` | puzzles 2–8 | badges 3–8, then game clear |
| `TrainerHill_*`, `DesertUnderpass`, `DesertRuins` (Regirock) | postgame | `FLAG_SYS_GAME_CLEAR`, `FLAG_REGI_DOORS_OPENED` |

Things set **in this segment** that pay off later:
- Archie has now met the player 3 times (museum, Meteor Falls, Chimney) and is suspicious ("whose side are you on?").
- Maxie's "ORB" line → Mt. Pyre (orbs) / Groudon.
- Stern's seafloor expedition and his "discovery" → later scenes (submarine, Seafloor Cavern).
- The Magma hideout guard on Jagged Pass.
- Wally runs away from Verdanturf after badge 4 → Victory Road.
- Rival intends to challenge Norman → Route 119 rival battle.
- Scott's recruiting arc (`VAR_SCOTT_STATE`) → Battle Frontier.
- Route 116 Devon employee (Repeat Ball, `FLAG_HIDE_ROUTE_116_DEVON_EMPLOYEE` cleared at B05).

---

## 3. Notable non-story NPCs and systems (what they do mechanically)

| Map | NPC / system | Mechanics |
|---|---|---|
| `Route109` | Soft Sand girl | `ITEM_SOFT_SAND` once (`FLAG_RECEIVED_SOFT_SAND`) |
| `Route109_SeashoreHouse` | Mr. Sea (owner, `POKEFAN_M`) | Beat `TRAINER_DWAYNE`, `_JOHANNA`, `_SIMON` → `FLAG_DEFEATED_SEASHORE_HOUSE` → 6× `ITEM_SODA_POP`; afterwards he sells Soda Pop for ¥300 |
| `SlateportCity` (market) | Energy Guru | Mart: vitamins (Protein, Iron, Carbos, Zinc, Calcium, HP Up) |
| `SlateportCity` (market) | Berry Powder clerk | `ITEM_POWDER_JAR` once, then trades berry powder for items |
| `SlateportCity` (market) | Decor / Doll / Power TM clerks | Decoration marts (bricks and mats need `FLAG_RECEIVED_SECRET_POWER`); TM Hidden Power and TM Secret Power for sale |
| `SlateportCity` | Effort Ribbon woman | Ribbon if the lead Pokémon's EVs are maxed |
| `SlateportCity_PokemonFanClub` | Chairman and members | Condition scarves; `ITEM_SOOTHE_BELL` for friendship ≥150 |
| `SlateportCity_NameRatersHouse` | Name Rater | Nickname changes |
| `SlateportCity_BattleTent*` | Slateport Battle Tent | Rental-Pokémon challenge (Frontier preview) |
| `SlateportCity_Harbor` | Ferry desk | "S.S. TIDAL under construction" (sign); ferry only after game clear |
| `Route110` | Cycling Road | Gates refuse walkers ("too dangerous, come back with a BIKE"). Mach Bike from the north gate starts a timed challenge (`VAR_CYCLING_CHALLENGE_STATE`); results on a sign |
| `Route110_TrickHouse*` | Trick Master (`MAN_1`) | Hides in a new spot each time ("You're being watched…"); puzzle 1 open now (needs Cut, trainers Sally, Eddie, Robin), reward `ITEM_RARE_CANDY`; later puzzles gated by badges 3→8 and game clear (rewards Timer Ball, Hard Stone, Smoke Ball, TM Taunt, Magnet, PP Max, Red/Blue Tent) |
| `MauvilleCity` | Rollout tutor, TV-explaining kid | Move tutor; sets `FLAG_TV_EXPLAINED` |
| `MauvilleCity_BikeShop` | Rydel | Free Mach/Acro bike, can be swapped (B12) |
| `MauvilleCity_GameCorner` | Game Corner | Coins (needs `ITEM_COIN_CASE` from `MauvilleCity_House2`), starter dolls, free 20 coins once |
| `MauvilleCity_PokemonCenter_1F` | Mauville "old man" (Bard / Storyteller / Trader / Giddy / Hipster) | Record-mixing-dependent feature |
| `Route117` + `Route117_PokemonDayCare` | Day Care couple | Breeding; the Day-Care man moves to the fence when an egg is waiting (`FLAG_PENDING_DAYCARE_EGG`) |
| `VerdanturfTown_*` | Battle Tent, Friendship Rater, Mart | Tent challenge; friendship check |
| `Route111` | Old Lady's Rest Stop | Free heal |
| `Route111` | Secret Power man (`BOY_1`) | `ITEM_TM_SECRET_POWER` (secret bases) |
| `Route111` | Berry girl | Daily `ITEM_RAZZ_BERRY` |
| `Route113_GlassWorkshop` | Glassman | `ITEM_SOOT_SACK`; ash steps → glass flutes and furniture |
| `FallarborTown_MoveRelearnersHouse` | Move Relearner | Relearns moves for Heart Scales |
| `FallarborTown_BattleTent*` | Fallarbor Battle Tent | Tent challenge |
| `Route114` | Roar gentleman | `ITEM_TM_ROAR` |
| `Route114` | Berry man | Daily random berry (Razz/Pinap) |
| `Route114_LanettesHouse` | Lanette | The PC storage system becomes "Lanette's PC" (`FLAG_SYS_PC_LANETTE`), doll gift |
| `Route114_FossilManiacsHouse/Tunnel` | Fossil Maniac (and his little brother) | `ITEM_TM_DIG`; hints at fossils in the desert and Devon's revival research; the tunnel collapse is opened postgame |
| `MtChimney` | Lava Cookie lady | Appears after B24; sells `ITEM_LAVA_COOKIE` for ¥200 |
| `MtChimney` | Hikers/Beauties (`TRAINER_SHELBY_1`, `_MELISSA`, `_SHEILA`, `_SHIRLEY`, `_SAWYER_1`) | Appear only after B24 |
| `LavaridgeTown_HerbShop` | Herb seller | `ITEM_CHARCOAL` gift; sells herbs |
| `LavaridgeTown` | Hot springs | Flavour only (old ladies' lines); increments `GAME_STAT_ENTERED_HOT_SPRINGS` |
| `Route118` | Good Rod fisherman | `ITEM_GOOD_ROD` |
| All gyms | Gym guide ("CHAMPION-bound {PLAYER}") | Type advice |

Route trainers (non-story, normal sight battles): Route 107 (6), Route 108 (6) [Surf], Route 109 (13), Route 110 (14 + rival),
Route 117 (9), Route 118 west (7), Route 111 (17 + Winstrates), Route 112 (6), Route 113 (10), Route 114 (12), Meteor Falls 1F_2R (2, needs
Waterfall), Jagged Pass (5), Rusturf Tunnel (`TRAINER_MIKE_2`).

---

## 4. Character index for this segment

| Original name | Overworld gfx | Battle pic / class | Role in segment B | What the script needs from them |
|---|---|---|---|---|
| Mr. Briney | `EXPERT_M` (+ boat) | — | Boat taxi Dewford ↔ Slateport; retires at badge 5 | Sails and parks on Route 109 |
| Capt. Stern | `SCIENTIST_1` | — | Recipient of the Devon parts; ocean explorer | Waits on Museum 2F, is attacked, heals the party, leaves |
| Dock | `MAN_1` | — | Ferry designer | Redirects the player; his talk clears the museum queue |
| Archie (leader A) | `ARCHIE` | Aqua Leader (not fought here) | Antagonist A: speeches at the museum, Meteor Falls and Chimney | Threatens without battling; ends up "allied" against B at Chimney |
| Team Aqua grunts | `AQUA_MEMBER_M/F` | Team Aqua / Aqua Grunt M | Queue, museum, Route 110 block, Meteor Falls, Chimney | 2 forced battles (museum) |
| The Rusturf grunt | `AQUA_MEMBER_M` | — | Recurring coward | Gives TM Thief and flees |
| Scott | `SCOTT` | — | Talent scout | Slateport (registers), Mauville, the 3 Battle Tents |
| Prof. Birch | `PROF_BIRCH` | — | Mentor | Route 110 PokéNav registration |
| Rival (May/Brendan) | `VAR_0` / bike `VAR_3` | Rival | Battle on Route 110 (Itemfinder); Go-Goggles in Lavaridge | Rides off on a bike both times |
| Wally | `WALLY` | Rival (Wally) | Friend: fight in Mauville, speech in Verdanturf, runs away after badge 4 | Blocks the Mauville gym with his Uncle |
| Wally's Uncle | `POKEFAN_M` | — | Guardian in Verdanturf | Same sprite as Wally's father |
| Wanda / boyfriend | `WOMAN_2` / `BLACK_BELT` | — | Separated lovers | Tunnel reunion, HM Strength |
| Wattson | `WATTSON` | Leader | Gym 3; New Mauville errand | Moves to the street after badge 5 |
| Rock Smash Dude | `SCIENTIST_1` | — | Gives HM06 | — |
| Rydel | `MAN_2` | — | Bike shop | — |
| Gabby & Ty | `REPORTER_F` / `CAMERAMAN` | Interviewer | TV duo | Double battle + interview |
| Winstrate family | `MAN_1`, `POKEFAN_F`, `LASS`, `EXPERT_F` | Winstrate | Gauntlet family | 4 battles in a row |
| Magma grunts | `MAGMA_MEMBER_M/F` | Team Magma | Route 112 block, Meteor Falls thieves, Chimney, Jagged Pass guard | 2 battles at Chimney + 1 optional (Jagged) |
| Tabitha | `MAGMA_MEMBER_M` (generic!) | Magma Admin | B's admin at Chimney | Delivered the meteorite to the boss |
| Maxie (leader B) | `MAXIE` | Magma Leader | Antagonist B: meteorite machine | First boss fight vs B; retreats, mentions the ORB |
| Prof. Cozmo | `SCIENTIST_1` | — | Meteorite scientist, tricked | In the cave after B22, home after B24, Meteorite → TM Return |
| Cozmo's wife | `WOMAN_2` | — | Hook in Fallarbor | "He went to Meteor Falls with Team Magma" |
| Flannery | `FLANNERY` | Leader | Gym 4 | Novice leader |
| Lavaridge egg lady | `EXPERT_F` | — | Gives the Wynaut egg | — |
| Norman | `NORMAN` | Leader | Gym 5, the player's father | Fight only at 4 badges |
| Wally's father / mother | `POKEFAN_M` / `WOMAN_4` | — | Give Surf / reveal that Wally ran away | Escort scene from the gym |
| Trick Master | `MAN_1` | — | Trick House host | Hides; puzzles |

---

## 5. Short re-interpretation checklist (what the writers MUST keep)

1. A boat captain brings the player to a beach near a port city and stays there as a ferry until badge 5.
2. In the port city, faction A's grunts queue to enter a museum. They block the door until the player talks to a shipyard engineer.
3. In the museum, 2 grunts try to steal the parcel the player is delivering. The player beats both, then **faction A's leader appears, threatens, mistakes the player for a member of faction B, and leaves without a fight.**
4. The scientist who receives the parcel heals the party and leaves for an undersea expedition.
5. A scout (Scott) registers the player's contact outside the museum and keeps showing up at Battle Tents.
6. The road north is blocked by faction A's grunts until the museum is cleared; then the professor registers by phone, and later the rival battles the player and gives a detection gadget.
7. The fragile friend and his guardian block the electric gym's door. The player must beat the friend (a single Ralts-line Pokémon); he goes off to recover in the green town and later disappears after badge 4.
8. An electric gym with switch barriers → badge 3 → the Rock Smash HM from a resident → rocks on the north route can be broken.
9. **Faction B is introduced:** 2 grunts guard a cable car and say their leader wants to awaken "that thing" with a meteorite; a squad has gone to the meteor town.
10. A scientist has been tricked; **faction B steals the meteorite in the waterfall cave, pushes the player aside and runs off to the volcano; faction A's leader arrives too late and explains the rivalry.**
11. At the volcano summit the two factions brawl. Faction A's leader is busy with 3 enemies. The player beats 2 grunts, the admin, and **faction B's leader at a machine fed by the meteorite; no legendary awakens**; the leader retreats, hinting at an "orb". Faction A's leader thanks the player and is suspicious. The meteorite can be taken out of the machine.
12. A faction B lookout loiters by a suspicious rock wall on the descent (a hidden base, paid off later).
13. A fire gym run by a novice leader → badge 4 → the rival gives goggles for the desert and says they will challenge the father.
14. The father (or the equivalent close figure) fights only at 4 badges → badge 5 → the friend's father takes the player to his house and gives Surf; the friend's mother reveals he has run away.
15. (Optional) The electric leader asks the player to switch off a runaway generator in an abandoned underground sector.
