# Segment C — Hoenn "Middle 2" (Route 118 east → Sootopolis crisis resolved)

Skeleton of scripted story beats, extracted from the pristine source
(`/home/user/pex-orig`, pokeemerald-expansion 1.17.1). Story writers must follow the
**order**, the **mechanics** and the **hard constraints (HC)** listed here. Only the
words and the motivations can change. Everything below was checked against the map
scripts (`data/maps/<Map>/scripts.inc`), the object and trigger tables (`map.json`),
the map layouts (collision/behaviour of `data/layouts/*/map.bin`, used to check whether a
blocker or a trigger really closes a path), `data/scripts/*.inc`, `data/text/*.inc`,
`src/data/trainers.party`, `src/data/battle_partners.party` and `src/*.c`.

- **Segment start:** the player has `ITEM_HM_SURF` (`FLAG_RECEIVED_HM_SURF`, badge 5) and crosses
  the river of Route 118 eastward (end of segment B).
- **Segment end:** Rayquaza has stopped Groudon and Kyogre in Sootopolis, the two villain
  leaders have left (`FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE`), Wallace has given `ITEM_HM_WATERFALL`
  and the Sootopolis Gym door is unlocked. The next segment starts with the Juan gym
  (`VAR_SOOTOPOLIS_CITY_STATE` 5 → 6), then Ever Grande / Victory Road.
- **Badges won in this segment:** #6 Feather (Winona, Fortree), #7 Mind (Tate & Liza, Mossdeep).
- **HMs obtained:** HM02 Fly (rival), HM08 Dive (Steven), HM07 Waterfall (Wallace, at the very end).
- **Legendaries in the plot:** Groudon (awakened, flees), Kyogre (awakened, flees), Rayquaza (awakened,
  stops them, then catchable). Castform (gift), Kecleon (Devon Scope), Electrode (fake items),
  Beldum (post-game), Regirock/Regice/Registeel (optional braille puzzles).

Legend: **[M]** = mandatory for progress. **[S]** = soft-mandatory (not gated here, but needed later or
almost impossible to skip). **[O]** = optional. **HC** = a hard constraint for the
re-interpretation, which must stay true in the new story because the event script does it.
"Faction A" = Team Aqua (Archie, Shelly, Matt). "Faction B" = Team Magma (Maxie, Tabitha).
The new story replaces both with an original plot, but the two-faction choreography below is fixed.

---

## 0. Critical path at a glance

| # | Where | What unlocks the next step (gate) — verified on layout + scripts |
|---|-------|------------------------------------------------------------------|
| 1 | Route 118 east | Optional Steven scene (`VAR_ROUTE118_STATE` 0→1). The trigger covers only 1 row of a 5-tile corridor, so it can be missed |
| 2 | Route 119 (south) | 2 Aqua lookouts stand at the west end of the only bridge north (x13, y33–34). The river below is surfable but closed upstream by a waterfall (needs HM Waterfall). Gate = `FLAG_HIDE_ROUTE_119_TEAM_AQUA` |
| 3 | Weather Institute 1F→2F | Beat Shelly. This sets `FLAG_HIDE_ROUTE_119_TEAM_AQUA`, `VAR_WEATHER_INSTITUTE_STATE = 1` and gives Castform |
| 4 | Route 119 (north) | Rival coord trigger (25–26, y31) on a 2-tile-wide path, so it cannot be avoided. Gives HM Fly. Then Scott (`VAR_ROUTE119_STATE` 0→1) |
| 5 | Fortree City | Gym entrance blocked by an invisible Kecleon (`FortreeCity_EventScript_Kecleon`). Needs `ITEM_DEVON_SCOPE` |
| 6 | Route 120 (bridge) | Steven + invisible Kecleon on the bridge. The bridge is the **only** path south to Route 121/Lilycove. Battle → `ITEM_DEVON_SCOPE` (`FLAG_RECEIVED_DEVON_SCOPE`) → bridge clear |
| 7 | Fortree Gym | Kecleon flees (`FLAG_KECLEON_FLED_FORTREE`) → Winona → badge 6 (`FLAG_BADGE06_GET`). **Not checked by any later gate** (only Fly and some dialogue need it) |
| 8 | Route 121 | Coord line x25, y5–8 (full width): 3 Aqua grunts "move out to Mt. Pyre" (`VAR_ROUTE121_STATE` 0→1) |
| 9 | Lilycove | Rival waits in front of the Department Store door (battle optional, but it blocks the store). Aqua Hideout entrance guarded. Sea exit east blocked by Wailmer metatiles |
| 10 | Route 122 → Mt. Pyre summit | Coord line (22–24, y7) cannot be avoided (the column below is 3 tiles wide). Archie leaves with the Red Orb, the old lady gives `ITEM_MAGMA_EMBLEM` (`VAR_MT_PYRE_STATE` 0→1, `FLAG_RECEIVED_RED_OR_BLUE_ORB`) |
| 11 | Jagged Pass (segment B map) | Having the Emblem sets `VAR_JAGGED_PASS_STATE` = 1 on resume. Stepping on a trigger opens the hidden door (state 2) |
| 12 | Magma Hideout 4F | Maxie awakens Groudon, Groudon flees, battle with Maxie → `FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT`, `VAR_SLATEPORT_CITY_STATE = 1`, `VAR_SLATEPORT_HARBOR_STATE = 1` |
| 13 | Slateport (segment B map) | Talk to Stern (TV interview, in front of the harbor door) → Aqua megaphone → harbor (`VAR_SLATEPORT_CITY_STATE = 2`) |
| 14 | Slateport Harbor | Archie steals the submarine (`VAR_SLATEPORT_HARBOR_STATE = 2`, `FLAG_MET_TEAM_AQUA_HARBOR`) → the Aqua Hideout entrance guards disappear |
| 15 | Aqua Hideout B2F | Beat Matt → submarine leaves (`FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE`) → Lilycove Wailmer and grunts gone → Route 124 open |
| 16 | Mossdeep Gym | Tate & Liza → badge 7 → `VAR_MOSSDEEP_CITY_STATE = 1`, `VAR_MOSSDEEP_SPACE_CENTER_STATE = 1` |
| 17 | Mossdeep street | Diagonal coord line (x40–42, y21–26) closes the road to the Space Center: Maxie + 4 grunts walk in (`VAR_MOSSDEEP_CITY_STATE = 2`) |
| 18 | Space Center 1F→2F | Stair guard (forced battle). 2F: 3 grunts in a row (declining = warped out). Steven + player vs Maxie + Tabitha (multi) → `VAR_MOSSDEEP_CITY_STATE = 3`, `VAR_STEVENS_HOUSE_STATE = 1` |
| 19 | Steven's House | `ITEM_HM_DIVE` (`FLAG_RECEIVED_HM_DIVE`, state 2). Dive needs badge 7 |
| 20 | Route 128 underwater → Seafloor Cavern | Needs Dive + Surf (currents) + Strength + Rock Smash. Room 9 trigger (17,42): Archie, Kyogre awakens and flees (`FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN`, `VAR_SOOTOPOLIS_CITY_STATE = 1`, `VAR_ROUTE128_STATE = 1`, `FLAG_SYS_WEATHER_CTRL`) |
| 21 | Route 128 | Auto scene: Archie, Maxie, Steven flies to Sootopolis (`VAR_ROUTE128_STATE = 2`) |
| 22 | Route 126 underwater → Sootopolis | Groudon vs Kyogre cutscene (`VAR_SOOTOPOLIS_CITY_STATE` 1→2) |
| 23 | Sootopolis → Cave of Origin B1F | Steven leads the player (the guard steps aside). Wallace asks where Rayquaza is → answer "SKY PILLAR" (`VAR_SOOTOPOLIS_CITY_STATE = 3`) |
| 24 | Route 131 → Sky Pillar outside | Wallace opens the door, earthquake, goes back (`VAR_SOOTOPOLIS_CITY_STATE = 4`) |
| 25 | Sky Pillar Top | Rayquaza awakens and flies off (`VAR_SOOTOPOLIS_CITY_STATE = 5`, `VAR_SKY_PILLAR_STATE = 1`) |
| 26 | Sootopolis | Rayquaza cutscene: the fight stops, the weather clears (`VAR_SKY_PILLAR_STATE` 2/3, `FLAG_SYS_WEATHER_CTRL` cleared) |
| 27 | Sootopolis | Talk to Maxie **and** Archie → they leave (`FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE`, `VAR_MT_PYRE_STATE = 2`). Gym door unlocks. Wallace (standing on the gym door) gives HM Waterfall and steps aside |

Optional branches opened in this segment: Route 123 (one-way west to Route 118), Safari Zone (needs the
Pokéblock Case from the Contest Lobby), Lilycove Department Store / Contest Hall / Museum / Motel,
Route 120 Scorched Slab, Shoal Cave (Route 125), Pacifidlog and Routes 129–134 (after the Wailmer
are gone), Abandoned Ship hidden floor (after Dive), Sealed Chamber + the three Regi tombs (after Dive),
Mirage Island (Route 130), Rayquaza battle (after #26), Mt. Pyre orb return (after #27).

### Order of story milestones (official order, from the PokéNav match-call tables)

`src/pokenav_match_call_data.c` unlocks Steven / Mr. Stone / rival / Scott / Wally call texts on
these flags, in this order. This is the designers' own milestone order and confirms the table above:

`FLAG_RECEIVED_CASTFORM` (#3) → `FLAG_RECEIVED_RED_OR_BLUE_ORB` (#10) → `FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT` (#12)
→ `FLAG_MET_TEAM_AQUA_HARBOR` (#14) → `FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE` (#15) → `FLAG_DEFEATED_MOSSDEEP_GYM` (#16)
→ `FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN` (#20) → `FLAG_DEFEATED_SOOTOPOLIS_GYM` (next segment).

### State variables touched in this segment (verified with grep)

| Variable / flag | Values and where they change |
|---|---|
| `VAR_ROUTE118_STATE` | 0 → **1** (Route 118 Steven scene, optional) |
| `VAR_WEATHER_INSTITUTE_STATE` | 0 → **1** (Shelly beaten, `Route119_WeatherInstitute_2F`) → **2** (next load of Route 119: workers move downstairs). While 0, both floors play the "infiltrated" music (`src/overworld.c` `IsInfiltratedWeatherInstitute`) |
| `VAR_ROUTE119_STATE` | 0 → **1** (rival battle + Fly) |
| `VAR_SCOTT_STATE` | +1 Route 119 (after the rival), +1 PokéNav call 10 steps after the Fortree badge, +1 Lilycove Motel 2F [O], +1 Mossdeep street [O] |
| `VAR_ROUTE121_STATE` | 0 → **1** (Aqua trio leaves) |
| `VAR_MT_PYRE_STATE` | 0 → **1** (Archie leaves, emblem given) → **2** (set in Sootopolis when Maxie & Archie leave) → **3** (they return the orbs at the summit, [O]) |
| `VAR_JAGGED_PASS_STATE` | 0 → **1** (`OnResume`: player has `ITEM_MAGMA_EMBLEM`) → **2** (hideout door opened) |
| `VAR_SLATEPORT_CITY_STATE` | 0 → **1** (Magma Hideout 4F: Stern back in town for an interview) → **2** (interview scene done) |
| `VAR_SLATEPORT_HARBOR_STATE` | 0 → **1** (Magma Hideout 4F) → **2** (Aqua steals the submarine) |
| `VAR_MOSSDEEP_CITY_STATE` | 0 → **1** (badge 7) → **2** (Magma walk into the Space Center) → **3** (Maxie beaten). `src/overworld.c` plays the "infiltrated" music in the Space Center while it is 1–2 |
| `VAR_MOSSDEEP_SPACE_CENTER_STATE` | 0 → **1** (badge 7) → **2** (3 grunts on 2F beaten) → **3** (Maxie/Tabitha beaten) |
| `VAR_MOSSDEEP_SPACE_CENTER_STAIR_GUARD_STATE` | 0 → 1 or 2 (where the beaten stair guard stands). 3 is unreachable (vanilla bug, commented in the script) |
| `VAR_STEVENS_HOUSE_STATE` | 0 → **1** (Space Center won) → **2** (Dive given; Seafloor Room 9 also forces 2). 2 is set again by the Hall of Fame (post-game letter) |
| `VAR_SEAFLOOR_CAVERN_STATE` | 0 → **1** (Kyogre scene) |
| `VAR_ROUTE128_STATE` | 0 → **1** (Kyogre escaped) → **2** (Route 128 scene watched) |
| `VAR_SOOTOPOLIS_CITY_STATE` | 0 → **1** (Kyogre escaped) → **2** (Groudon/Kyogre cutscene watched) → **3** (Wallace goes to Sky Pillar) → **4** (Wallace scene at Sky Pillar) → **5** (Rayquaza awakened) → 6 (Juan, next segment) |
| `VAR_SKY_PILLAR_STATE` | 0 → **1** (Rayquaza leaves the top) → **2** (Sootopolis Rayquaza scene, arriving by Dive) or **3** (arriving from the Pokémon Center; `OnWarp` also turns 2 into 3). ≥2: Rayquaza battle available at the top, the pillar's floors become cracked |
| `VAR_SKY_PILLAR_RAYQUAZA_CRY_DONE` | 0 → 1 (top trigger used) |
| `VAR_SOOTOPOLIS_WALLACE_STATE` | 0 → 1 or 2 (side where Wallace steps after giving Waterfall) |
| `FLAG_SYS_WEATHER_CTRL` | set by Seafloor Room 9 (and again by the Sky Pillar Wallace scene), cleared by the Rayquaza scene. While set, Lilycove, Mossdeep, Sootopolis, Ever Grande and Routes 124–128 (129–131 too from state 4) get abnormal weather + `MUS_ABNORMAL_WEATHER` |
| `FLAG_LEGENDARIES_IN_SOOTOPOLIS` | set Seafloor Room 9, cleared by the Rayquaza scene |
| Step-counter calls | `VAR_SCOTT_FORTREE_CALL_STEP_COUNTER` (10 steps after badge 6), `VAR_RIVAL_RAYQUAZA_CALL_STEP_COUNTER` (250 steps after `FLAG_DEFEATED_MAGMA_SPACE_CENTER`) |

---

## 1. Story beats (in play order)

### C01 — Route 118 east bank: Steven jumps down from a ledge [O in practice]
- **Map:** `Route118`
- **Mechanics:** Steven (`OBJ_EVENT_GFX_STEVEN`, `LOCALID_ROUTE118_STEVEN`) stands at (44,7) on a ledge
  strip. He is visible from a new game, because `FLAG_HIDE_ROUTE_118_STEVEN` is never set. Coord triggers (43,11), (44,11), (45,11)
  run when `VAR_ROUTE118_STATE == 0`. Steven lines up, jumps down the ledge (`SE_LEDGE`), walks to the player, talks, walks
  8–10 tiles east and is removed. The script sets `VAR_ROUTE118_STATE = 1`. No battle and no item. The corridor at x43–45 is
  open on y9–13, but the trigger covers only y11, so a player can walk around it.
- **Original text (gist):** STEVEN: "Hi, it's me, STEVEN! We met in the cave near DEWFORD. Have you met
  many kinds of POKéMON? … If you wanted to raise only your favorites, that's fine… As a TRAINER, what do you
  think? … It would be nice if we were to meet again somewhere."
- **Characters:** Steven (mentor; sprite `STEVEN`).
- **HC:** the mentor drops from a ledge, asks one question about how to raise Pokémon, and walks off east.
  It cannot carry information that the player needs later, because the scene can be missed.
- Also on this map: the east-bank trainers Barny, Chester and Perry, the Gabby & Ty slot (see §3), and two Terra Cave warps
  (post-game). The Good Rod fisherman is segment B content.

### C02 — Route 119 (south): the lookouts at the bridge [M gate]
- **Map:** `Route119`
- **Mechanics:** two Aqua grunts (`AQUA_MEMBER_M`) stand at (13,33) and (13,34), facing east toward the bridge.
  Their flag `FLAG_HIDE_ROUTE_119_TEAM_AQUA` is not set by `new_game.inc`, so they are present from the start. They block the only land path
  north. The river under the bridge can be surfed, but upstream (y26–28) it becomes a waterfall that needs HM Waterfall. They only talk. The
  Weather Institute door (6,32) and its sign are right next to them. The route is long grass (`TallGrassSnaresBikeTires`, no biking)
  with periodic rain (`COORD_EVENT_WEATHER_ROUTE119_CYCLE`). 17 normal trainers (camouflaged Ninja Boys in trees,
  grass trainers that mirror the player's steps).
- **Original text (gist):** "We're standing lookout here. Hey, you! Stay away from the WEATHER INSTITUTE. It's not
  safe." / "Lookout duty is surprisingly boring. Hey, you! Please don't go near the WEATHER INSTITUTE."
- **Characters:** 2 generic grunts (faction A).
- **HC:** two villain lookouts block the bridge and point the player to the research building next to it. They disappear
  as soon as the boss inside is beaten (they share the flag with the Institute grunts).

### C03 — Weather Institute 1F: occupied building [M]
- **Map:** `Route119_WeatherInstitute_1F`
- **Mechanics:** `TRAINER_GRUNT_WEATHER_INST_1` (`AQUA_MEMBER_M` (15,3), sight 3) and `_4` (`AQUA_MEMBER_F` (10,5), sight 2),
  both with flag `FLAG_HIDE_ROUTE_119_TEAM_AQUA`. A little boy (`NINJA_BOY`) is moved to (0,5) while the state is 0. The staff are hidden
  (`FLAG_HIDE_WEATHER_INSTITUTE_1F_WORKERS`, set in `new_game.inc`). A bed in the corner (bg events at (0–1,2–3)) heals the party
  (`Common_EventScript_OutOfCenterPartyHeal`).
- **Original text (gist):** Grunt 1: "The BOSS got interested in the research they have going here, so he sent us out. You quit
  meddling!" / "Our BOSS knows everything. But I'm just a GRUNT." Grunt 4: "What's a kid doing here?" / "I should just take a nap
  in the bed…" Boy: "While I was sleeping, everyone went upstairs!"
- **HC:** the villains hold the ground floor, the staff have been herded upstairs, and a free bed heals the player before the boss.

### C04 — Weather Institute 2F: the admin, the messenger, Castform [M]
- **Map:** `Route119_WeatherInstitute_2F`
- **Mechanics (in this order):**
  1. Three sight battles: `TRAINER_GRUNT_WEATHER_INST_2` (`AQUA_MEMBER_M` (15,6)), `_3` (`AQUA_MEMBER_M` (10,8)),
     `_5` (`AQUA_MEMBER_F` (19,6)). The scientist (`SCIENTIST_1`, `LOCALID_WEATHER_INSTITUTE_2F_SCIENTIST`) is moved to (1,6), and two
     workers (`MAN_4`) at (0,6)/(1,7) are cornered at the west wall.
  2. **Shelly** stands at (4,6) between the player and the hostages. Her overworld sprite is the **generic female grunt** (`AQUA_MEMBER_F`), with sight 0,
     so the player must talk to her. Battle `TRAINER_SHELLY_WEATHER_INSTITUTE` (class Aqua Admin, pic "Aqua Admin F"; Carvanha 28, Mightyena 28).
  3. After the battle, a hidden messenger grunt (`LOCALID_WEATHER_INSTITUTE_2F_GRUNT_3`, flag `FLAG_HIDE_WEATHER_INSTITUTE_2F_AQUA_GRUNT_M`)
     is added. He runs up to Shelly and **shoves the player aside**, then reports. Shelly gets a "!" and reacts.
  4. Fade to black. All faction-A objects are removed. Flags: `VAR_WEATHER_INSTITUTE_STATE = 1`, `FLAG_HIDE_ROUTE_119_TEAM_AQUA`
     (this also clears the bridge lookouts of C02), `FLAG_HIDE_WEATHER_INSTITUTE_2F_AQUA_GRUNT_M`, and the 2F workers are shown again.
  5. The scientist walks to the player and gives **Castform L25 holding Mystic Water** (`givemon SPECIES_CASTFORM_NORMAL, 25, ITEM_MYSTIC_WATER`,
     nickname prompt, goes to the PC if the party is full, `FLAG_RECEIVED_CASTFORM`). If the player has no room, talking to him again repeats the gift.
- **Original text (gist):** Grunt 2: "The INSTITUTE created a type of POKéMON that has something to do with the weather. We're here
  to take them!" Grunt 5: "What we really want isn't here… Ihihihihi…" SHELLY: "Ahahahaha! You're going to meddle in TEAM AQUA's
  affairs? You're either absolutely fearless, simply ignorant, or both!" / "It's bad enough to have TEAM MAGMA blunder about, but now
  there's you!" Messenger: "We have a situation here! A TEAM MAGMA mob just passed the WEATHER INSTITUTE. They appear to be headed
  for MT. PYRE!" SHELLY: "We have to hurry to MT. PYRE, too! TEAM MAGMA, just you wait!" Scientist: "Thanks to you, we're safe! …
  take this POKéMON. … It changes shape according to the weather conditions."
- **Characters:** Shelly (faction A admin, female), 4 grunts + messenger, the weather scientist, 2 workers.
- **HC:** a female admin with a generic grunt sprite and a unique battle picture holds weather researchers hostage. The faction
  claims it wants "the weather Pokémon" but hints that the real target is elsewhere. Right after her defeat **a messenger brings news that
  faction B is heading for the mountain**, and the whole faction-A squad leaves at once. The rescued scientist gives a weather-form Pokémon.

### C05 — Route 119 (north): rival battle #3, HM Fly, Scott [M]
- **Map:** `Route119`
- **Mechanics:** coord triggers (25,31) and (26,31) with `VAR_ROUTE119_STATE == 0`. The path there is 2 tiles wide, so the trigger
  cannot be avoided. The rival on a bike (`VAR_3`, `LOCALID_ROUTE119_RIVAL_ON_BIKE`) rides in from the **west, across the bridge**, with the
  rival's encounter music. The rival gets off the bike (`VAR_0`) and talks, then a no-intro battle: `TRAINER_MAY_ROUTE_119_*` /
  `TRAINER_BRENDAN_ROUTE_119_*` by starter (e.g. May/Treecko: Pelipper 29, Lombre 29, Combusken 31).
  Then `giveitem ITEM_HM_FLY` and `FLAG_RECEIVED_HM_FLY`. The rival rides off north (toward Fortree) and `VAR_ROUTE119_STATE = 1` is set. Then **Scott**
  (`SCOTT`, `LOCALID_ROUTE119_SCOTT`) walks down from the north, talks (`VAR_SCOTT_STATE += 1`) and walks back north.
- **Original text (gist):** MAY: "Where were you? I was looking for you! How much stronger have you gotten?" → "And here! I have a
  present for you. … Use FLY… But to use FLY you have to get the GYM BADGE from FORTREE CITY. You should FLY home and visit LITTLEROOT.
  I bet your mom's worried." BRENDAN: "So this is where you've been looking for POKéMON? Let me see how good you got." → "Here, I'll
  give you this. … you need the FORTREE GYM BADGE. Anyway, I have to move along." SCOTT: "Way to go! I just passed by a TRAINER riding
  a BIKE… The kid looked really upset… Are you off to FORTREE GYM next?"
- **Characters:** rival May/Brendan (opposite gender of the player; `VAR_0`/`VAR_3` dynamic sprites), Scott (scout).
- **HC:** the rival intercepts the player right after the bridge, battles, gives the flying HM and says it only works with the next
  gym's badge. Then the scout appears and comments that he saw the rival leave angry.

### C06 — Route 119 north end and Fortree City: the gym is blocked [M]
- **Maps:** `Route119`, `FortreeCity`
- **Mechanics:** optional invisible Kecleon on Route 119: `Route119_EventScript_Kecleon1` (31,6) and `Kecleon2` (25,15),
  flags `FLAG_HIDE_ROUTE_119_KECLEON_1/2`. They are wild L30 battles via `EventScript_Kecleon` once the Devon Scope is owned;
  before that "Something unseeable is in the way." Fortree: `FLAG_VISITED_FORTREE_CITY`. The treetop city uses `STEP_CB_FORTREE_BRIDGE`
  (swaying bridges). An invisible Kecleon (`FortreeCity_EventScript_Kecleon`, (25,8), flag `FLAG_HIDE_FORTREE_CITY_KECLEON`) stands
  on the walkway to the gym door (22,11). A woman (`WOMAN_5`) says the gym is blocked.
- **Original text (gist):** Woman: "I want to go to the POKéMON GYM, but something's blocking the way. After all the bother I went
  through training on ROUTE 120…" Man: "No one believes me, but I saw this gigantic POKéMON in the sky. It seemed to squirm as it flew
  toward ROUTE 131. … You smell singed. Were you at a volcano or something?" (Rayquaza foreshadowing.)
- **HC:** an unseen obstacle blocks the gym. A townsperson has seen a giant creature fly toward the Route 131 area (the Sky Pillar).

### C07 — Route 120: Steven, the invisible Pokémon on the bridge, Devon Scope [M]
- **Map:** `Route120`
- **Mechanics:** Steven (`STEVEN`, `LOCALID_ROUTE120_STEVEN`, (13,15), flag `FLAG_HIDE_ROUTE_120_STEVEN` never set, so he is there from the start)
  stands on a narrow wooden bridge. An invisible Kecleon (`LOCALID_BRIDGE_KECLEON` (12,16)) and its reflection object block it. This bridge
  is the **only** land/water path from the Fortree side to the south of Route 120 and on to Route 121 (checked on the layout).
  Talk to Steven: yes/no "are your POKéMON ready?" (no → `FLAG_NOT_READY_FOR_BATTLE_ROUTE_120`, he waits). Then he uses the Devon
  Scope, the Kecleon flickers into view and attacks: wild `SPECIES_KECLEON` L30 (catchable). After the battle: `giveitem ITEM_DEVON_SCOPE`,
  `FLAG_RECEIVED_DEVON_SCOPE`. Steven **flies away** (`FLDEFF_NPCFLY_OUT`). The bridge metatiles are redrawn clear; `OnLoad` does the same
  later when the flag is set.
  Five more optional invisible Kecleon on the route (`Route120_EventScript_Kecleon1–5`); #1 at (20,11) guards a Nest Ball.
- **Original text (gist):** STEVEN: "Hm? Hi. It's been a while. There's something here that you can't see, right? Now, if I were to use this
  device on the invisible obstacle… No, I should just show you. That would be more fun." → "STEVEN used the DEVON SCOPE. An invisible
  POKéMON became completely visible! The startled POKéMON attacked!" → "Your battle style is intriguing… I'd like you to have this DEVON
  SCOPE. Who knows, there may be other concealed POKéMON." → "I enjoy seeing POKéMON and TRAINERS who strive together… Well, let's meet again somewhere."
- **Characters:** Steven (mentor; son of the Devon president; his flying exit implies a flying Pokémon).
- **HC:** the mentor reveals an invisible creature with a gadget made by his company, makes the player fight it, then gives the
  gadget and flies off. **The gadget is the key** for the gym door (C08) and for the optional camouflaged creatures.

### C08 — Fortree: revealing the gym blocker [M]
- **Map:** `FortreeCity`
- **Mechanics:** with `ITEM_DEVON_SCOPE` the yes/no prompt "Want to use the DEVON SCOPE?" appears. The Kecleon flickers, cries and **runs away (no battle)**,
  then `removeobject` and `FLAG_KECLEON_FLED_FORTREE` are applied. The woman's dialogue changes ("This time, I'll beat WINONA.").
- **HC:** the obstacle at the gym is the same kind of hidden creature. Once revealed it flees, with no fight.

### C09 — Fortree Gym: Winona, Feather Badge [M for the badge; time-flexible]
- **Map:** `FortreeCity_Gym`
- **Mechanics:** rotating-gate puzzle (`RotatingGate_InitPuzzle`; the follower is hidden). 6 trainers (Jared, Edwardo, Flint, Ashley, Humberto, Darius)
  and a gym guide. `TRAINER_WINONA_1` (Swablu 29, Tropius 29, Pelipper 30, Skarmory 31, Altaria 33 @ Oran Berry). Results: `FLAG_DEFEATED_FORTREE_GYM`,
  `FLAG_BADGE06_GET`, gym trainers set, `ITEM_TM_AERIAL_ACE` ("TM40"), Winona registered in the PokéNav, `FLAG_SCOTT_CALL_FORTREE_GYM` set (C10).
  Badge 6 = obedience up to L70 and **Fly usable outside battle**.
- **Ordering note:** no later script tests `FLAG_BADGE06_GET` as a gate (only the Lilycove rival dialogue branches on it, plus the
  Fly field move). Players can do Lilycove/Mt. Pyre first. The default story order is: right after the Devon Scope.
- **Original text (gist):** WINONA: "I am WINONA… I have become one with BIRD POKéMON and have soared the skies… Witness the elegant
  choreography of BIRD POKéMON and I!" / "Never before have I seen a TRAINER command POKéMON with more grace than I…"
- **HC:** flying-type leader in a treetop gym with turnstile gates. Her badge enables the flying HM.

### C10 — Scott's PokéNav call [automatic]
- **Script:** `Route119_EventScript_ScottWonAtFortreeGymCall`, run from `src/field_control_avatar.c` 10 steps after badge 6 on any town or route map.
  `VAR_SCOTT_STATE += 1`.
- **Gist:** "Just as I thought, you won at the FORTREE GYM. Perhaps you really are the TRAINER that I've been searching for. Remember, you have a fan in me."
- **HC:** the scout keeps tabs on the player remotely.

### C11 — Route 121: faction A moves out to the mountain [M, automatic]
- **Map:** `Route121`
- **Mechanics:** coord line (25,5)–(25,8) with `VAR_ROUTE121_STATE == 0`. It spans the whole passage. 3 Aqua grunts (`AQUA_MEMBER_M`,
  `LOCALID_ROUTE121_GRUNT_1–3`, flag `FLAG_HIDE_ROUTE_121_TEAM_AQUA_GRUNTS`, visible from the start) huddle at (30–31,7–8). Aqua music,
  one line, they all walk off east and are removed. `VAR_ROUTE121_STATE = 1`. The route has the Safari Zone gate (37,5) and the "MT. PYRE PIER" sign,
  where the player surfs south to Route 122.
- **Original text (gist):** "Okay! We're to move out to MT. PYRE!" Woman: "Ahead looms MT. PYRE… a natural monument to the spirits of departed POKéMON…"
- **HC:** a small faction-A squad is seen leaving for the mountain right in front of the player. No battle.

### C12 — Lilycove City: first arrival [M arrival; parts optional]
- **Map:** `LilycoveCity`
- **Mechanics:**
  - `FLAG_VISITED_LILYCOVE_CITY`. **Wailmer metatiles** at (76–78, 12–17) block the sea exit east (Route 124) while
    `FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE` is unset (`LilycoveCity_OnLoad`).
  - 5 Aqua grunts in town (flag `FLAG_HIDE_LILYCOVE_CITY_AQUA_GRUNTS`): one trains Wailmer at the cove (73,15) (first talk sets
    `FLAG_MET_WAILMER_TRAINER`), the others talk to themselves.
  - The cove cave (warp (70,5) → `AquaHideout_1F`) is reached by Surf.
  - **Rival** (`VAR_0`, flag `FLAG_HIDE_LILYCOVE_CITY_RIVAL`, visible from start) stands at (27,7), right on the Department Store door
    (27,6), so **the store cannot be entered until the rival is beaten**. Talk: yes/no battle. No → `FLAG_DECLINED_RIVAL_BATTLE_LILYCOVE`, rival stays.
    Yes → `TRAINER_MAY_LILYCOVE_*` / `TRAINER_BRENDAN_LILYCOVE_*` (e.g. Tropius 31, Pelipper 32, Ludicolo 32, Combusken 34). After the battle the rival says
    they are going home; the closing line depends on badges (no badge 8 → "collect badges"; badges 6 and 8 → League; after game clear → Frontier). The rival flies off
    (`FLDEFF_NPCFLY_OUT`), and the script sets `FLAG_MET_RIVAL_LILYCOVE` and the "rival is back in the bedroom" flags in Littleroot.
- **Original text (gist):** Grunts: "We moved more loot into our secret HIDEOUT today… Wh-who are you?! I was just talking to myself!" /
  "Don't go near the cave in the cove! … I'm an adult, so you just listen to me!" / "If this whole wide world becomes ours, TEAM AQUA's, it will be
  a happier place for POKéMON, too." Sailor: "TEAM AQUA's been training their WAILMER in the cove. We SAILORS can't get our boats out to sea."
  Woman: "Someone stole my POKéMON! … TEAM AQUA?" Old man: "They call themselves the 'nature-loving TEAM AQUA'! But what they do and what they say don't match."
  Sailor/Fat man: "I heard there's a tower somewhere out on the sea routes. It's called the SKY PILLAR." / "I saw this tall tower somewhere around ROUTE 131."
  Honeymooners: "We happened to see a DRAGON-type POKéMON flying way up in the sky." MAY: "Are you shopping, too? I bought a whole bunch of DOLLS…
  I'll battle with you." BRENDAN: "I'm running an errand for my dad. No, I'm not buying any DOLLS."
- **HC:** the port city is under the quiet control of faction A: their **trained sea creatures physically block the harbor**, a hidden base is in a sea cave,
  and townspeople complain of thefts. The rival blocks the department store until battled, then flies home. Several NPCs give the hint
  **"Sky Pillar, a tall tower near Route 131"**, which the player needs later (C39 multichoice).

### C13 — Aqua Hideout, first visit: the entrance guards [M as a signpost]
- **Map:** `AquaHideout_1F`
- **Mechanics:** two grunts (`AQUA_MEMBER_M` at (13,11)/(14,11), flags `FLAG_HIDE_AQUA_HIDEOUT_1F_GRUNT_1/2_BLOCKING_ENTRANCE`) block the corridor.
  They are not trainers. Their text **is the game's progress hint system**:
  - before the emblem: "Our BOSS … has gone off to snatch something important!" / "He's on his way to MT. PYRE on ROUTE 122!"
  - after `FLAG_RECEIVED_RED_OR_BLUE_ORB`: "Are you a TEAM MAGMA grunt? … TEAM MAGMA is trying to awaken an awesome POKéMON at their HIDEOUT.
    Where might it be?" / "TEAM MAGMA is after an awesome POKéMON at MT. CHIMNEY."
  - after `FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT`: "He's gone off to jack a submarine!" / "He's on his way to SLATEPORT CITY!"
  They are removed by the harbor scene (C23).
- **HC:** two door guards whose small talk always points to the next step: mountain → volcano base → port.

### C14 — Lilycove Motel 2F: Scott [O]
- **Map:** `LilycoveCity_CoveLilyMotel_2F`. Scott (`SCOTT`, flag `FLAG_HIDE_LILYCOVE_MOTEL_SCOTT`) can be found here **only until the harbor scene**
  (C23 hides him). First talk: `VAR_SCOTT_STATE += 1`, `FLAG_MET_SCOTT_IN_LILYCOVE`. Gist: he is snoozing and prefers battles to contests.
  Motel 1F owner: "Since that TEAM AQUA came to town, the tourists have been staying away."

### C15 — Contest Lobby: the Pokéblock Case [S]
- **Map:** `LilycoveCity_ContestLobby` (`data/scripts/contest_hall.inc`). The first talk to the receptionist gives `ITEM_POKEBLOCK_CASE`
  (`FLAG_RECEIVED_POKEBLOCK_CASE`). **The Safari Zone refuses entry without it** (`Route121_SafariZoneEntrance_EventScript_NoPokeblockCase`).
  See §3 for contests.

### C16 — Route 122 → Mt. Pyre 1F and the exterior path [M]
- **Maps:** `Route122` (a sea route from the Route 121 pier; no trainers), `MtPyre_1F`, `MtPyre_Exterior`
- **Mechanics:** `MtPyre_1F` gives `ITEM_CLEANSE_TAG` (old woman, `FLAG_RECEIVED_CLEANSE_TAG`). The **summit path does not go through the tower floors**:
  the 1F west door (3–4,6) leads to `MtPyre_Exterior` (fog/sun weather triggers), and from there to `MtPyre_Summit`. Floors 2F–6F are an optional
  graveyard climb (11 trainers incl. Hex Maniacs/Psychics; cracked floor on 2F drops to 1F; TM Shadow Ball on 6F; the 4F/5F scripts are swapped in the source).
- **Original text (gist):** Old woman 1F: "All sorts of beings wander the slopes of MT. PYRE… Take this. It's for your own good." Visitor: "Did you come
  to pay your respect to the spirits of departed POKéMON?"
- **HC:** a cemetery mountain (memorial tower inside, misty outdoor path to the summit). The story path is the outdoor one.

### C17 — Mt. Pyre summit: the orbs are stolen, the emblem is left behind [M]
- **Map:** `MtPyre_Summit`
- **Mechanics (in this order):**
  1. 4 Aqua grunts on the slope with sight 3 (`TRAINER_GRUNT_MT_PYRE_1–4`; flag `FLAG_HIDE_MT_PYRE_SUMMIT_TEAM_AQUA`, visible from start).
  2. At the shrine: Archie (`ARCHIE`, (23,6), facing up) in front of the old lady (`EXPERT_F`, (23,5)) and the old man (`OLD_MAN`, (22,5)).
     The coord line (22–24, 7) with `VAR_MT_PYRE_STATE == 0` cannot be avoided: the approach column below is exactly x22–24.
  3. Aqua music. Archie turns to the player and speaks (**no battle**). Fade: Archie and the 4 grunts removed (`FLAG_HIDE_MT_PYRE_SUMMIT_ARCHIE`,
     `FLAG_HIDE_MT_PYRE_SUMMIT_TEAM_AQUA`), `VAR_MT_PYRE_STATE = 1`.
  4. The old lady walks to the player: both orbs are gone; faction B took the Blue Orb first and "left this behind": `giveitem ITEM_MAGMA_EMBLEM`,
     `FLAG_RECEIVED_RED_OR_BLUE_ORB`, `FLAG_HIDE_JAGGED_PASS_MAGMA_GUARD` (the segment-B lookout on Jagged Pass disappears).
  - The old man tells the legend on request (yes/no). After the crisis he tells a "new legend" (post `FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE`).
  - Old lady later states: "orbs taken… must never be apart" → (after Kyogre) "both awakened… the true owner of the orbs still exists" →
    (after C44) "the two men returned the orbs".
- **Original text (gist):** Grunts: "Those TEAM MAGMA goons got here ahead of us!" / "We saw you at MT. CHIMNEY. You don't belong to either TEAM."
  / "The BOSS should have snatched what he was after!" ARCHIE: "TEAM MAGMA's MAXIE got ahead of us, but we also got what we wanted. The RED
  ORB preserved at MT. PYRE… I, ARCHIE, now have it! Now we can bring our ultimate objective to fruition! Okay, TEAM! We're pulling out!"
  OLD LADY: "Not only the BLUE ORB, but even the RED ORB has been taken… They belong together. … Was it TEAM MAGMA who took the BLUE ORB first?
  In their haste, they left this behind. … Perhaps it will be useful." OLD MAN (legend): "a ferocious clash between the POKéMON of the land and the
  POKéMON of the sea… The BLUE ORB and the RED ORB brought an end to the calamity… The pair, made docile, dove deep into the sea…"
- **Characters:** Archie (faction A leader), 4 grunts, the old lady and old man (keepers of the shrine).
- **HC:** **two sacred items** are kept at a mountain shrine. Faction B took one **off-screen before the player arrived** and dropped an item that opens its base.
  Faction A's leader takes the other in front of the player, does not fight, and withdraws. The keeper gives the player the dropped item.
  The legend tells of a land titan, a sea titan, and two items that calmed them.

### C18 — Route 123: the one-way road back [O]
- **Maps:** `Route123`, `Route123_BerryMastersHouse`. West-facing ledges make Route 123 a **one-way** shortcut from the south end of Route 122 to Route 118.
  It can't be used to come in from Route 118. 15 trainers, the Berry Master and his wife (berries), a Giga Drain TM girl (needs a Grass Pokémon in the party).

### C19 — Jagged Pass: the emblem opens the hidden base [M]
- **Map:** `JaggedPass` (segment-B map). `OnResume`: if `ITEM_MAGMA_EMBLEM` is owned, `VAR_JAGGED_PASS_STATE = 1`. Coord triggers (13–14,15), (21,15), (21–22,20)
  fire the camera shake: "This boulder is shaking in response to the MAGMA EMBLEM!" The rock wall at (16,17–18) becomes a cave mouth, then `VAR_JAGGED_PASS_STATE = 2`.
  The lookout of segment B is gone (`FLAG_HIDE_JAGGED_PASS_MAGMA_GUARD`).
- **HC:** the dropped emblem makes a fake rock wall on the volcano slope open by itself.

### C20 — Magma Hideout 1F–3F: inside the volcano [M]
- **Maps:** `MagmaHideout_1F`, `_2F_1R`, `_2F_2R`, `_2F_3R`, `_3F_1R`, `_3F_2R`, `_3F_3R`
- **Mechanics:** 13 grunts across the floors (`TRAINER_GRUNT_MAGMA_HIDEOUT_1–10, 14–16`, sight trainers, flag `FLAG_HIDE_MAGMA_HIDEOUT_GRUNTS`), items
  (Rare Candy, Max Elixir, Full Restore, Nugget, PP Max…), 3 Strength boulders on 1F near the Rare Candy. Entering 1F sets `VAR_JAGGED_PASS_ASH_WEATHER = 0`.
- **Original text (gist):** "Our leader told us to dig into MT. CHIMNEY, so we dug and dug. And we came across something that blew our minds!" /
  "We dug up something beyond belief! And, we got the BLUE ORB! All that's left is for our leader to…" / "One of our guys was freaking out that he lost his
  MAGMA EMBLEM… Was it you who found it?" / "You can hear tremors here sometimes. Is it GROU… Whoops!" / Doubting grunt: "digging up a super-ancient POKéMON
  and ripping off someone's METEORITE… I think we're going a little too far." / "Do you think it's odd that we're wearing hoods in this magma-filled volcano?"
- **HC:** faction B has dug a base inside the volcano where a titan sleeps. Some grunts start to doubt their leader.

### C21 — Magma Hideout 4F: the land titan awakens [M]
- **Map:** `MagmaHideout_4F`
- **Mechanics (in this order):**
  1. 3 grunts (`_11–13`) and **Tabitha** (`TRAINER_TABITHA_MAGMA_HIDEOUT`, generic `MAGMA_MEMBER_M` overworld sprite, class Magma Admin; Numel 26, Mightyena 28,
     Zubat 30, Camerupt 33) on the way, all sight trainers.
  2. Maxie (`MAXIE`, (16,21), facing up) stands before the sleeping Groudon (`GROUDON_ASLEEP` (16,17)). Talk to Maxie: magma music, speech, sparkle,
     `MUS_AWAKEN_LEGEND`, `DoOrbEffect` (orb light), the sleeping sprite swaps to `GROUDON_FRONT`, Groudon steps toward them, the screen shakes, Groudon **slides up
     and leaves**, more shaking. Maxie looks around, panicking.
  3. Maxie turns to the player: `trainerbattle_no_intro TRAINER_MAXIE_MAGMA_HIDEOUT` (Mightyena 37, Crobat 38, Camerupt 39).
  4. After the battle: Maxie says he is going after Groudon. Flags: `FLAG_GROUDON_AWAKENED_MAGMA_HIDEOUT`; Slateport prepared for the next beat (`FLAG_HIDE_SLATEPORT_CITY_CAPTAIN_STERN` and
     `FLAG_HIDE_SLATEPORT_CITY_GABBY_AND_TY` cleared, `VAR_SLATEPORT_CITY_STATE = 1`, `VAR_SLATEPORT_HARBOR_STATE = 1`). Fade: Maxie, Tabitha and the 3 grunts removed,
     and the whole hideout is emptied (`FLAG_HIDE_MAGMA_HIDEOUT_GRUNTS`). No legendary battle.
- **Original text (gist):** TABITHA: "Up ahead, GROUDON is sleeping! MAXIE went to GROUDON just seconds ago! It's going to awaken real soon!" MAXIE: "GROUDON…
  This BLUE ORB is what you sought. Wasn't it? Let its shine awaken you! And show me the full extent of your power!" → "GROUDON! What's wrong? Wasn't the
  BLUE ORB the key? Where have you gone…" → "Oh, so it was you? You must have pulled a cheap stunt!" → after battle: "There has to be some reason why
  GROUDON fled… You think I didn't know that? With GROUDON gone, there is no longer any need for this blasted volcano. I am going after GROUDON."
- **Characters:** Maxie (faction B leader, unique sprite), Tabitha (admin, generic sprite), grunts, Groudon (overworld sprites asleep/front).
- **HC:** **the faction-B leader uses the stolen item on a sleeping red titan; it wakes up, does not obey, and flees through the ceiling.** The leader
  blames the player, loses the fight, and leaves to chase the titan. The heat wave/drought that follows is told in later dialogue (Steven, C35).

### C22 — Slateport: Stern's TV interview and the megaphone [M]
- **Map:** `SlateportCity` (segment-B map)
- **Mechanics:** Capt. Stern (`SCIENTIST_1`, (28,13)) stands in front of the main harbor door (28,12) with Gabby & Ty (`REPORTER_F`, `CAMERAMAN`). Townsfolk are moved into an
  audience and their lines switch to "Stern interview" variants (`VAR_SLATEPORT_CITY_STATE == 1`). Talk to Stern: the interview ends, Gabby & Ty leave, Stern tells the
  player about his discovery, then Aqua music and an **off-screen megaphone** announcement. Bystanders react with "?" emotes. Stern: "It's from the HARBOR! … Please, come with me!"
  Both walk into the harbor. The script shows Stern, the submarine, a grunt and Archie in the harbor (clears 4 hide flags), sets `VAR_SLATEPORT_CITY_STATE = 2`, and warps the player into the harbor.
- **Original text (gist):** STERN: "We made a huge discovery on our last seafloor exploration. We found an underwater cavern on ROUTE 128. We think it's
  the habitat of a POKéMON that's said to have been long extinct." Megaphone: "Fufufu… CAPT. STERN, I presume. We of TEAM AQUA will assume control of your
  submarine! Your objections are meaningless!"
- **HC:** the explorer who received the player's parcel in segment B is on TV announcing **an undersea cave on Route 128**. Faction A interrupts by loudspeaker to seize his submarine.

### C23 — Slateport Harbor: the submarine is stolen [M]
- **Map:** `SlateportCity_Harbor`
- **Mechanics:** `OnTransition` with state 1 moves Stern to (12,13) and hides the ferry patrons (`FLAG_HIDE_SLATEPORT_CITY_HARBOR_PATRONS`). Coord column (8, 11–14):
  Archie (`ARCHIE`) and a grunt (`AQUA_MEMBER_M`) at the submarine (`SUBMARINE_SHADOW` (7,9)) turn to the player. Archie speaks (no battle), boards, and the
  **submarine sails away** (all three objects removed). `VAR_SLATEPORT_HARBOR_STATE = 2`, `FLAG_MET_TEAM_AQUA_HARBOR`, `FLAG_HIDE_LILYCOVE_MOTEL_SCOTT`.
  Stern walks to the player and laments. **Then `FLAG_HIDE_AQUA_HIDEOUT_1F_GRUNT_1/2_BLOCKING_ENTRANCE` are set**: the Lilycove hideout is open.
- **Original text (gist):** ARCHIE: "Oh? Not you again… You are tenacious to track us here. But now… No one can stop us! Or, will you follow us back to our
  HIDEOUT in LILYCOVE CITY? Fwahahahaha…" STERN: "Why would TEAM AQUA steal my SUBMARINE EXPLORER 1? They can't be after the slumbering POKéMON at the bottom of
  the sea… But even if I were to chase them, I don't stand a chance."
- **HC:** faction A's leader escapes in the explorer's submarine and **openly invites the player to the base in Lilycove**. No fight. The gate on the base opens because of this scene.

### C24 — Aqua Hideout: the submarine escapes again [M]
- **Maps:** `AquaHideout_1F`, `AquaHideout_B1F`, `AquaHideout_B2F` (the `AquaHideout_UnusedRubyMap1–3` maps are not used)
- **Mechanics:** **warp-panel maze** (grunts brag about it). Trainers: `TRAINER_GRUNT_AQUA_HIDEOUT_1–8` (`FLAG_HIDE_AQUA_HIDEOUT_GRUNTS`). B1F: four item balls in a 2×2 block;
  **two of them are Electrode** (`AquaHideout_B1F_EventScript_Electrode1/2`, wild L30, `FLAG_HIDE_AQUA_HIDEOUT_B1F_ELECTRODE_1/2`), the other two are
  **`ITEM_MASTER_BALL`** and a Nugget. B2F: Matt (`TRAINER_MATT`, generic `AQUA_MEMBER_M` overworld sprite, class Aqua Admin, pic "Aqua Admin M"; Mightyena 34, Golbat 34) stands next to the
  submarine dock. Coord (28,16–17) makes him notice the player ("!"). After his defeat the **submarine departs** (`SUBMARINE_SHADOW` slides left and is removed).
  Then `FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE` and `FLAG_HIDE_LILYCOVE_CITY_AQUA_GRUNTS` are set, so the Lilycove Wailmer metatiles and grunts are gone and **Route 124 opens**.
- **Original text (gist):** Grunts: "There's a submarine at the far end! But, by now… Kekekeke…" / "Fuel supply loaded A-OK! In-cruise snacks loaded A-OK!" /
  "I can't remember where I put the MASTER BALL." MATT: "Got here already? … I'm not stalling for time. I'm going to pulverize you!" → "While I was toying with
  you, our BOSS got through his preparations!" → "Our BOSS has already gone on his way to some cave under the sea! If you're going to give chase, you'd better
  search the big, wide sea beyond LILYCOVE."
- **Characters:** Matt (male admin, generic sprite), 8 grunts, Electrode.
- **HC:** the player storms the faction-A base but **arrives too late**: an admin stalls and the boss leaves in the stolen sub toward an undersea cave
  "beyond Lilycove". As a side effect the creatures blocking the harbor leave. The base contains the Master Ball and trap-items.

### C25 — Stern's advice: Dive and Mossdeep [S]
- **Map:** `SlateportCity_Harbor`. Talking to Stern after C24 sets `FLAG_EVIL_TEAM_ESCAPED_STERN_SPOKE` ("You would need a POKéMON that knows how to DIVE… Perhaps
  if you went out to MOSSDEEP CITY. A lot of divers live out there"). Many town NPCs after badge 7 also change lines.
- **HC:** the next destination (the island city) is suggested by the explorer.

### C26 — Route 124 → Mossdeep City: arrival [M]
- **Maps:** `Route124`, `MossdeepCity`, `MossdeepCity_SpaceCenter_1F`
- **Mechanics:** Route 124 (sea, 8 trainers, dark dive patches, Treasure Hunter's house). Mossdeep `FLAG_VISITED_MOSSDEEP_CITY` via the coord tiles (25–26,25), (32–33,27).
  - Steven waits in Space Center 1F (`STEVEN` (1,4), flag `FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_1F_STEVEN`, visible from start until badge 7) next to a
    **notice on the desk** (invisible object at (2,5), `FLAG_HIDE_MOSSDEEP_CITY_SPACE_CENTER_MAGMA_NOTE`; `OnLoad` draws a data pad while `VAR_MOSSDEEP_CITY_STATE ≤ 2`).
  - Scott on the beach [O] (`SCOTT` (61,29), flag `FLAG_HIDE_MOSSDEEP_CITY_SCOTT`): one talk, `VAR_SCOTT_STATE += 1`, walks off.
  - NPCs talk about the warning letter.
- **Original text (gist):** Notice: "An intent-to-steal notice? 'To the staff of the SPACE CENTER: How are you? We are doing fine. We will soon visit you to take your
  rocket fuel. Please don't try to stop us… Let there be more land! TEAM MAGMA'" STEVEN: "TEAM MAGMA is coming after the rocket fuel on this island…
  they can't be allowed to take it. I'll keep an eye on things… why don't you go check out the town?" Sailor: "MOSSDEEP here's been targeted by that TEAM MAGMA.
  If you want to know what they're up to, go visit the SPACE CENTER." Black belt: "The GYM in SOOTOPOLIS had a new LEADER come in… the new LEADER once mentored WALLACE."
- **HC:** faction B has **sent a polite written warning** that it will steal the space center's rocket fuel. The mentor keeps watch and sends the player to the gym first.

### C27 — Mossdeep Gym: Tate & Liza, Mind Badge [M]
- **Map:** `MossdeepCity_Gym`
- **Mechanics:** floor-switch puzzle (yellow/blue/green/purple/red switches; `initrotatingtilepuzzle`/`moverotatingtileobjects` rotate the arrow
  tiles and the statues (`TRICK_HOUSE_STATUE` objects) standing on them; the follower is hidden). 12 gym trainers. **Double battle** `TRAINER_TATE_AND_LIZA_1`
  (Claydol 41, Xatu 41, Lunatone 42, Solrock 42; needs 2 Pokémon). The gym leaders are 2 objects (`TATE`, `LIZA`).
  **On victory, many flags are set at once:** `FLAG_DEFEATED_MOSSDEEP_GYM`, `FLAG_BADGE07_GET`, `FLAG_HIDE_AQUA_HIDEOUT_GRUNTS` (Lilycove base emptied),
  Mr. Briney moves to Stern's shipyard (`FLAG_HIDE_SLATEPORT_CITY_STERNS_SHIPYARD_MR_BRINEY` cleared), harbor patrons back,
  Magma shown in town and in the Space Center 1F/2F, Steven moved from 1F to 2F, `VAR_MOSSDEEP_CITY_STATE = 1`, `VAR_MOSSDEEP_SPACE_CENTER_STATE = 1`,
  `ITEM_TM_CALM_MIND` ("TM04"), Tate&Liza registered in the PokéNav. Badge 7 = **Dive usable outside battle**.
- **Original text (gist):** TATE/LIZA (alternating lines): "Were you surprised? That there are two GYM LEADERS? We're twins! We don't need to talk because…
  we can each determine what the other is thinking… This combination of ours… Can you beat it?" → "What?! Our combination… was shattered!"
- **HC:** twin psychic leaders, double battle, rotating-statue puzzle. **Winning the badge is what triggers the villain raid** (the Space Center attack starts only after it).

### C28 — Mossdeep street: faction B enters the Space Center [M, automatic]
- **Map:** `MossdeepCity`
- **Mechanics:** Maxie (`MAXIE` (45,25)) and 4 grunts (`MAGMA_MEMBER_M` (44,23–26), flag `FLAG_HIDE_MOSSDEEP_CITY_TEAM_MAGMA`) stand on the road east.
  A diagonal coord line (42,21), (41,22–24), (40,25–26) with `VAR_MOSSDEEP_CITY_STATE == 1` closes the only crossing toward the Space Center (checked on
  the layout). Maxie gestures, everyone turns to the Space Center and walks in (removed). `VAR_MOSSDEEP_CITY_STATE = 2`, `FLAG_HIDE_MOSSDEEP_CITY_TEAM_MAGMA`.
  No dialogue.
- **HC:** silent scene: the faction-B leader and 4 grunts march into the space center in front of the player.

### C29 — Space Center 1F: the guarded stairs [M]
- **Map:** `MossdeepCity_SpaceCenter_1F`
- **Mechanics:** with state 2 the staff are moved to the west wall facing right (scared) and their lines switch to "Magma" variants. 4 Magma grunts:
  `TRAINER_GRUNT_SPACE_CENTER_1` (11,6), `_3` (`MAGMA_MEMBER_F` (12,9)), `_4` (10,2), sight 2; `_2` is the **stair guard** (13,2) on the stairs. Talking to him
  starts a forced battle (`FLAG_DEFEATED_GRUNT_SPACE_CENTER_1F`), then he steps aside (`VAR_MOSSDEEP_SPACE_CENTER_STAIR_GUARD_STATE`). Sun Stone gift from a sailor (both states).
- **Original text (gist):** "As promised, we've come for the rocket fuel!" / "We gave you fair warning! There's nothing sneaky about us!" → "Okay, I get it already! The next time,
  we'll come unannounced." / "What are we going to do with the rocket fuel? How would I know? Ask our leader upstairs!" Stair guard: "Our leader said no one, but no one,
  gets past me!" → "Please, tell our leader that I never abandoned my post." Staff: "Those MAGMA thugs have their sights set on our SPACE CENTER. But we can't allow anything that
  minor to interfere with our rocket launch!" / "TEAM AQUA should take care of TEAM MAGMA! But … TEAM AQUA will become bold and brazen, won't they?"

### C30 — Space Center 2F: three in a row, then the double battle with Steven [M]
- **Map:** `MossdeepCity_SpaceCenter_2F`
- **Mechanics (in this order):**
  1. `OnFrame` with `VAR_MOSSDEEP_SPACE_CENTER_STATE == 1`: "!" over the player. A yes/no: "You're outnumbered three to one, but you still want to take us on?"
     **No → "Good answer!" and the player is walked out and warped to 1F** (repeats on re-entry). Yes → 3 consecutive no-intro battles `TRAINER_GRUNT_SPACE_CENTER_5`, `_6`, `_7`
     (the player turns to each), then `VAR_MOSSDEEP_SPACE_CENTER_STATE = 2` and the beaten grunts stay in place.
  2. Steven (`STEVEN` (1,8)) faces Maxie (`MAXIE` (1,9)) and Tabitha (`MAGMA_MEMBER_M` (0,8)). First talk to Steven (`FLAG_INTERACTED_WITH_STEVEN_SPACE_CENTER`):
     Steven asks why; **Maxie explains the plan**; Steven makes a fighting stance. Second talk: "You're going to help me?" yes/no → **choose half the party** (`ChooseHalfPartyForBattle`)
     → `multi_2_vs_2 TRAINER_MAXIE_MOSSDEEP` (Mightyena 42, Crobat 43, Camerupt 44) + `TRAINER_TABITHA_MOSSDEEP` (Camerupt 36, Mightyena 38, Golbat 40), **partner `PARTNER_STEVEN`**
     (Metang 42, Skarmory 43, Aggron 44). Losing = white-out.
  3. After the win: Maxie's doubt speech, Maxie and Tabitha look at each other, then "We will give up on the fuel…". `VAR_MOSSDEEP_CITY_STATE = 3`,
     `VAR_MOSSDEEP_SPACE_CENTER_STATE = 3`, all faction-B objects in town and Space Center hidden, civilians put back. Steven thanks the player and invites them home:
     `FLAG_DEFEATED_MAGMA_SPACE_CENTER`, `VAR_STEVENS_HOUSE_STATE = 1`, Steven moved to his house, notice removed, Mossdeep Scott hidden.
- **Original text (gist):** STEVEN: "TEAM MAGMA… What's the point of stealing rocket fuel?" MAXIE: "We're going to jettison the entire load into MT. CHIMNEY! With GROUDON gone,
  we have no need for that slag heap of a mountain! So we'll use the fuel's power to make the volcano erupt! It will be savage!" Maxie (battle intro): "All I want… I just want
  to expand the land mass…" After: "We failed to make the volcano erupt… We failed to control GROUDON after we had awoken it… Is our goal to expand the land misguided? …
  If we, TEAM MAGMA, are wrong… Then might TEAM AQUA's goal to expand the sea also be equally misguided?" → "There appear to be more important matters that I must examine…"
  STEVEN: "Whew, that was too tense. Thank you. I have something to give you… Please come see me at home. I don't live in RUSTBORO CITY. I live right here on this island."
- **Characters:** Steven (partner trainer, unique battle team), Maxie, Tabitha, 3 grunts.
- **HC:** a 3-in-a-row gauntlet the player may refuse, then a **2-vs-2 tag battle with the mentor as partner** against the faction-B leader and admin, using half the party.
  The faction-B leader **reveals a desperate plan with the fuel** (an eruption) and, after losing, **starts doubting both factions' goals and gives up**. This is
  faction B's last fight in the story. From here Maxie becomes a reluctant ally.

### C31 — The rival's call about a green flying Pokémon [automatic]
- **Script:** `MossdeepCity_SpaceCenter_2F_EventScript_RivalRayquazaCall`. It fires after 250 steps on town/route maps once `FLAG_DEFEATED_MAGMA_SPACE_CENTER` is set,
  then clears the flag. Texts: `MatchCall_Text_MayRayquazaCall` / `BrendanRayquazaCall` (`data/text/match_call.inc`).
- **Gist:** MAY: "I was just in PACIFIDLOG a little while ago. I saw a giant green POKéMON flying high in the sky. I've never seen anything like it."
- **HC:** another foreshadowing of Rayquaza and the Pacifidlog/Sky Pillar area.

### C32 — Steven's House: HM Dive [M]
- **Map:** `MossdeepCity_StevensHouse`
- **Mechanics:** `OnFrame` with `VAR_STEVENS_HOUSE_STATE == 1`: Steven notices the player ("!"), walks over, `giveitem ITEM_HM_DIVE`, `FLAG_RECEIVED_HM_DIVE`,
  `FLAG_OMIT_DIVE_FROM_STEVEN_LETTER`, explains Dive, walks back. `VAR_STEVENS_HOUSE_STATE = 2`. This also sets `FLAG_HIDE_MOSSDEEP_CITY_SCOTT` and
  `FLAG_HIDE_SEAFLOOR_CAVERN_ENTRANCE_AQUA_GRUNT` (see C33 note). Later talk: "Apparently, there's an underwater cavern between MOSSDEEP and SOOTOPOLIS. You know,
  the one that CAPT. STERN found in his submarine." The rock collection is a sign. Beldum and the letter are post-game (§2).
- **Original text (gist):** STEVEN: "As you can see, there's not much here, but this is my home. Thank you for all that you've done. This is my token of appreciation.
  It's the HIDDEN MACHINE DIVE." → "While you're using SURF, you should notice dark patches of water. Use DIVE… When you want to come back up, use DIVE again."
- **HC:** the mentor gives the diving HM at his house and points to the undersea cave between the island and the crater city.

### C33 — Route 128 underwater → Seafloor Cavern: faction A's last stronghold [M]
- **Maps:** `Route127`, `Route128`, `Underwater_Route128`, `Underwater_SeafloorCavern`, `SeafloorCavern_Entrance`, `SeafloorCavern_Room1`–`Room8`
- **Mechanics:** dive spot on Route 128 → `Underwater_Route128` warp (38,26) → `Underwater_SeafloorCavern`. There the **stolen submarine** is docked (metatiles; 4 invisible
  sign objects with flag `FLAG_HIDE_UNDERWATER_SEA_FLOOR_CAVERN_STOLEN_SUBMARINE`). Surface → cavern. The cavern needs **Strength** (boulders in Rooms 1, 2, 3, 5 and a 12-boulder
  puzzle in Room 8), **Rock Smash** (Rooms 1, 2, 5) and **Surf on currents** (Rooms 6–7). Faction-A trainers: `TRAINER_GRUNT_SEAFLOOR_CAVERN_1–5` and
  **Shelly** again (`TRAINER_SHELLY_SEAFLOOR_CAVERN`, Room 3, `AQUA_MEMBER_F`; Sharpedo 37, Mightyena 37); all flagged `FLAG_HIDE_SEAFLOOR_CAVERN_AQUA_GRUNTS`.
  The entrance grunt (`SeafloorCavern_Entrance`, (10,2)) is **effectively never seen**: his flag `FLAG_HIDE_SEAFLOOR_CAVERN_ENTRANCE_AQUA_GRUNT` is set when Dive is
  received, before the cave can be reached. That is leftover content.
- **Original text (gist):** Submarine: "'SUBMARINE EXPLORER 1' is painted on the hull. This is the submarine TEAM AQUA stole in SLATEPORT! TEAM AQUA must have gone ashore here."
  Grunts: "That submarine we jacked, man, it's brutal as a ride. It's way too tight in there!" / "My partner forgot the map in that submarine!" SHELLY: "How did you
  manage to get here without a submarine? … I do want payback for what happened at the WEATHER INSTITUTE…" → "It's terribly disappointing that you're not a TEAM AQUA member.
  You could have enjoyed the fabulous world our BOSS has promised…"
- **HC:** the stolen submarine is found parked under the sea, and the cave is guarded by faction A's grunts and the female admin (rematch from C04).
  The cave demands three field moves (boulders, rocks, currents).

### C34 — Seafloor Cavern Room 9: the sea titan awakens [M]
- **Map:** `SeafloorCavern_Room9`
- **Mechanics (in this order):**
  1. Coord (17,42) with `VAR_SEAFLOOR_CAVERN_STATE == 0`: Aqua music. "Hold it right there." Archie (`ARCHIE`) appears and walks up. The sleeping Kyogre
     (`KYOGRE_ASLEEP` (17,38)) is shown. `trainerbattle_no_intro TRAINER_ARCHIE` (class Aqua Leader; Mightyena 41, Crobat 41, Sharpedo 43).
  2. After the battle Archie raises the Red Orb: the weather resets, "The RED ORB suddenly began shining by itself!", orb effect + `MUS_AWAKEN_LEGEND`, `KYOGRE_FRONT`
     approaches, the screen shakes, **Kyogre slides away**.
  3. Archie is confused. A radio message (`SE_PC_LOGIN`) reports that the rain is far heavier than planned and members are in danger.
  4. Maxie (`MAXIE`) + 2 Magma grunts (M, F) arrive and confront Archie, then both leaders leave. Maxie tells the player to get out.
  5. Flags: `VAR_ROUTE128_STATE = 1`, `VAR_SOOTOPOLIS_CITY_STATE = 1`, Sootopolis objects unhidden (Steven, Archie, Maxie, residents, Groudon, Kyogre),
     `FLAG_HIDE_SOOTOPOLIS_CITY_MAN_1`, `FLAG_LEGENDARIES_IN_SOOTOPOLIS`, Route 128 Archie/Maxie shown, **`FLAG_SYS_WEATHER_CTRL`** (abnormal weather across eastern Hoenn),
     `FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN`, Steven's house emptied (`VAR_STEVENS_HOUSE_STATE = 2`), `VAR_SEAFLOOR_CAVERN_STATE = 1`, all cavern villains hidden,
     the submarine removed (on the next underwater load the dock becomes rock wall). The player is warped to Route 128 (38,22).
- **Original text (gist):** ARCHIE: "Behold! See how beautiful it is, the sleeping form of the ancient POKéMON KYOGRE! I have waited so long for this day… For the
  realization of my dream, you must disappear now!" → "I commend you… But! I have this in my possession! With this RED ORB, I can make KYOGRE…" → "What?! I didn't do anything.
  Why did the RED ORB… Where did KYOGRE go?" → radio: "It's raining heavily? Good… That is why we awakened KYOGRE, to realize TEAM AQUA's vision of expanding the sea.
  What?! It's raining far harder than we envisioned? You're in danger?" MAXIE: "What have you wrought? ARCHIE… You've finally awoken KYOGRE… The world's landmass will
  drown in the deepening sea…" ARCHIE: "Wasn't it you, TEAM MAGMA, that infuriated GROUDON? So long as I have this RED ORB, I should be able to control KYOGRE…"
  MAXIE: "We don't have the time to argue about it here! Get outside and see for yourself!" / "{PLAYER}, come on, you have to get out of here, too!"
- **Characters:** Archie (fought once, here), Maxie, 2 Magma grunts, Kyogre (overworld sprites).
- **HC:** the faction-A leader fights the player at the sleeping blue titan, then **the stolen item activates by itself**, the titan wakes, ignores him
  and leaves. A radio report reveals the disaster. **The faction-B leader arrives, blames him, and both leaders rush out together**, now on the same side.
  This mirrors C21: each leader awakens "his" titan and loses control of it.

### C35 — Route 128: the two leaders' remorse; Steven arrives [M, automatic]
- **Map:** `Route128`
- **Mechanics:** `OnFrame` with `VAR_ROUTE128_STATE == 1` (map-name popup hidden). Archie (`ARCHIE` (37,22)) looks around and steps back. Maxie (`MAXIE` (38,21)) walks
  to him, then to the player. Archie runs off and both leave. Steven **flies in** (`FLDEFF_NPCFLY_OUT`; `FLAG_HIDE_ROUTE_128_STEVEN` object added), talks, and flies off.
  `VAR_ROUTE128_STATE = 2`. Abnormal weather is on.
- **Original text (gist):** ARCHIE: "What happened… What is this wretched scene… Did I make a horrible mistake? I… I only wanted…" MAXIE: "Do you understand now, ARCHIE?
  Do you finally see how disastrous your dream turned out to be? … {PLAYER}, don't say anything. I know that I have no right to be critical of ARCHIE… The responsibility
  for putting an end to this falls to ARCHIE and me…" → "Those super-ancient POKéMON… They've upset the balance of nature…" STEVEN: "What is happening? … After the
  scorching heat wave ended, this deluge began. If this doesn't stop, all of HOENN… No, the whole world will drown. This huge rain cloud is spreading from above
  SOOTOPOLIS… SOOTOPOLIS might provide answers… I don't know what you intend to do, but don't do anything reckless. I'm going to SOOTOPOLIS."
- **HC:** on a small island in the storm, one leader breaks down and the other takes joint responsibility. The mentor arrives by air, names the **heat wave followed
  by a deluge**, says the storm is centred on the crater city, and flies there.

### C36 — The crisis weather (system, active from C34 to C42)
- While `FLAG_SYS_WEATHER_CTRL` is set: `Common_EventScript_SetAbnormalWeather` runs on `LilycoveCity`, `MossdeepCity`, `SootopolisCity`, `EverGrandeCity`,
  `Route124`–`Route128`, and on `Route129`–`Route131`/`SkyPillar_Outside` once `VAR_SOOTOPOLIS_CITY_STATE ≥ 4`. `src/overworld.c`
  (`ShouldLegendaryMusicPlayAtLocation`) plays the crisis music there. The Lilycove Department Store rooftop is closed for `VAR_SOOTOPOLIS_CITY_STATE` 1–3
  (`LilycoveCity_DepartmentStore_5F`: a woman blocks the stairs: "They closed the rooftop because the weather is wild today").
- **HC:** for the whole crisis the eastern sea and its cities alternate between drought and downpour.

### C37 — Sootopolis: the two titans fight in the crater [M]
- **Map:** `SootopolisCity` (reached by Dive: Route 126 dive spot → `Underwater_Route126` (45,65) → `Underwater_SootopolisCity` → surface at (29,53))
- **Mechanics:** `OnLoad`: the gym door is locked while `FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE` is unset (this is true **from a new game**, so an early Dive visit finds the gym closed).
  While the crisis is on, 9 house doors are locked too. The layout swaps to `LAYOUT_SOOTOPOLIS_CITY_LEGENDS_BATTLE`, with downpour, while state is 1–4. `OnFrame` with state 1: the camera pans to the lake
  (two versions: from the dive spot or from the Pokémon Center door). Then the full-screen **`Script_DoRayquazaScene`** (special cutscene, `src/rayquaza_scene.c`, with the Groudon/Kyogre part only), then
  overworld `GROUDON_SIDE` (28,44) vs `KYOGRE_SIDE` (34,44) **trade three charges with screen shakes**, and the camera pans back. `VAR_SOOTOPOLIS_CITY_STATE = 2`.
  Residents (flag `FLAG_HIDE_SOOTOPOLIS_CITY_RESIDENTS`) stand watching. Maxie (29,33) and Archie (31,33) stand in front of the gym begging their titans. The cave guard (`EXPERT_M` (31,18))
  blocks the Cave of Origin.
- **Original text (gist):** MAXIE: "G… GROUDON… Please! Stop what you're doing! … If you keep going, all HOENN, not just SOOTOPOLIS, will be utterly ruined!" ARCHIE:
  "KYOGRE! What's wrong?! Look over here! It's the RED ORB! Calm down! … It's not responding at all!" Residents: "These giant POKéMON suddenly appeared in the middle
  of the city! Why are they smashing into each other?" / "I just get this sense that the two POKéMON aren't angry. They probably can't control their own power…" /
  "There's an ancient legend that claims the land and sea were shaped by a colossal battle between POKéMON. I'm seeing that happen with my very own eyes!"
  Guard: "This is the CAVE OF ORIGIN. The spirits of POKéMON, becalmed at MT. PYRE, are said to be revived here. Please leave."
- **HC:** both titans are in the crater lake of the city, fighting. The two leaders plead with them in vain. The citizens watch, and the cave of the spirits is guarded.

### C38 — Sootopolis: Steven leads the player to the cave [M]
- **Mechanics:** Steven (`STEVEN` (20,36)). First talk (`FLAG_STEVEN_GUIDES_TO_CAVE_OF_ORIGIN` unset): speech, they walk together across the city, the guard
  **steps aside**, Steven speaks at the entrance, `FLAG_STEVEN_GUIDES_TO_CAVE_OF_ORIGIN` is set and the player is warped into `CaveOfOrigin_Entrance`. While the state is 2–3 the guard
  stays aside; Steven stays at the entrance.
- **Original text (gist):** STEVEN: "The two super-ancient POKéMON were awakened from a long sleep… And now they are smashing each other with their uncontrollable energy…
  You being here now I'll take to mean that you're prepared to become involved in this crisis. There's someone I'd like you to meet. Come with me." → "Does seeing GROUDON
  and KYOGRE make you think POKéMON are to be feared? But that's not true… Why am I asking you this? You already know." → "Inside here you'll find someone named WALLACE.
  I think you have what's needed to help him…" Guard (state ≥2): "If a TRAINER with a strong will and superior talent were to appear, I was instructed by WALLACE to lead
  that TRAINER to this CAVE."
- **HC:** the mentor walks the player to the sacred cave and introduces a new ally inside.

### C39 — Cave of Origin B1F: Wallace and the question [M]
- **Maps:** `CaveOfOrigin_Entrance`, `CaveOfOrigin_1F`, `CaveOfOrigin_B1F` (the 3 `_UnusedRubySapphireMap` maps and the shake/cry scripts in
  `data/scripts/cave_of_origin.inc` are unused leftovers)
- **Mechanics:** Wallace (`WALLACE` (9,13), facing up, flag `FLAG_HIDE_CAVE_OF_ORIGIN_B1F_WALLACE`). Talk: story, "!", then a **looping multichoice**
  `MULTI_WHERES_RAYQUAZA` (strings hard-coded in `src/data/script_menu.h`: "CAVE OF ORIGIN" / "MT. PYRE" / "SKY PILLAR" / "Don't remember"). Only "SKY PILLAR" ends it. The others
  get a reply and the menu repeats. Then the screen fades, `FLAG_WALLACE_GOES_TO_SKY_PILLAR`, `VAR_SOOTOPOLIS_CITY_STATE = 3`, Wallace removed here and shown at the Sky Pillar
  (`FLAG_HIDE_SKY_PILLAR_WALLACE` cleared). No battle, no item.
- **Original text (gist):** WALLACE: "Ah, so you are {PLAYER}? I've heard tales of your exploits. My name is WALLACE. I was once the GYM LEADER of SOOTOPOLIS, but something came up.
  So now, I've entrusted my mentor JUAN with the GYM's operation. … GROUDON and KYOGRE … are super-ancient POKéMON. But there aren't just two… Somewhere, there is a super-ancient
  POKéMON named RAYQUAZA. It's said that it was RAYQUAZA that becalmed the two combatants in the distant past. But even I have no clue as to RAYQUAZA's whereabouts…" →
  "Do you perhaps know where RAYQUAZA is now?" → (Mt. Pyre) "when I met the old lady there earlier, she made no mention of it." → (Sky Pillar) "That's it! It must be the SKY PILLAR!
  There's not a moment to lose! We'll head to the SKY PILLAR right away!"
- **Characters:** Wallace (former gym leader of the city, guardian of the cave; unique sprite; future Champion).
- **HC:** a regal ally waits inside the sacred cave, explains that **a third, sky titan** once calmed the other two, and **the player must name its location** from hints given earlier
  (C06, C12, C31, Pacifidlog NPCs). The 4 menu options are fixed in number. The answer that works is the tower on Route 131.

### C40 — Route 131 → Sky Pillar outside: Wallace opens the door [M]
- **Maps:** `Route131` (always loads `LAYOUT_ROUTE131_SKY_PILLAR`), `SkyPillar_Entrance` (a cave, `FLAG_LANDMARK_SKY_PILLAR`), `SkyPillar_Outside`
- **Mechanics:** the tower door (14,4–5) is a closed metatile ("The door is closed.") until `FLAG_WALLACE_GOES_TO_SKY_PILLAR` (`OnLoad` draws it open).
  `OnFrame` with state 3: Wallace (`WALLACE` (13,7)) walks to the player and apologises. Short earthquake (shake). Both climb a few steps, a bigger earthquake, Wallace looks around,
  then `FLAG_SYS_WEATHER_CTRL` and `WEATHER_ABNORMAL` start here too. Wallace leaves (fade). `VAR_SOOTOPOLIS_CITY_STATE = 4`, Wallace shown back in Sootopolis (31,18 area,
  between the guard and Steven).
- **Original text (gist):** WALLACE: "Oh, my, I'm terribly sorry! In my haste, I didn't notice that I'd left you behind! I've opened the locked door of the SKY PILLAR. Let's be on our way!"
  → "It's an earthquake! There's not a moment to waste!" → "The situation is getting worse…" → "The weather distortion is spreading even here… RAYQUAZA should be farther up from here.
  I'm worried about SOOTOPOLIS. I've got to go back. Everything is in your hands now. Don't fail us!"
- **HC:** the ally unseals the ancient tower, the disaster reaches it, and the ally goes back to protect the city, **leaving the player to climb alone**.

### C41 — Sky Pillar climb and top: the sky titan wakes [M]
- **Maps:** `SkyPillar_1F`–`SkyPillar_5F`, `SkyPillar_Top`
- **Mechanics:** while `VAR_SKY_PILLAR_STATE < 2` every floor uses its `_CLEAN` layout (`LAYOUT_SKY_PILLAR_*_CLEAN`): 2F and the top have no cracked tiles, and 4F has 5. 2F/4F still
  have the cracked-floor step callback and hole warps (falling drops to the floor below). No trainers. Top: Rayquaza (`RAYQUAZA` (14,7), flag `FLAG_HIDE_SKY_PILLAR_TOP_RAYQUAZA`) sleeps.
  Coord (14,9) with `VAR_SKY_PILLAR_RAYQUAZA_CRY_DONE == 0`: music fades, camera pans up, Rayquaza stirs, cries twice with shakes, **flies off** (slides up), "The awakened RAYQUAZA flew off…",
  camera back. `VAR_SOOTOPOLIS_CITY_STATE = 5`, `VAR_SKY_PILLAR_STATE = 1`, `VAR_SKY_PILLAR_RAYQUAZA_CRY_DONE = 1`. **No battle here.**
- **HC:** at the top of the tower the green sky titan wakes up by itself when the player approaches, and flies away (toward the city). The player does not fight or catch it now.

### C42 — Sootopolis: Rayquaza ends the fight [M, automatic]
- **Map:** `SootopolisCity`
- **Mechanics:** `OnFrame` with `VAR_SKY_PILLAR_STATE == 1` (no music: `NoMusicInSootopolisWithLegendaries`). Camera pan, rough-water metatiles, Groudon/Kyogre removed,
  Rayquaza added, **`Script_DoRayquazaScene` full version** (Rayquaza descends between the two), thunder, Rayquaza cries twice, weather cleared, Rayquaza flies off (`RAYQUAZA` (31,41)).
  `FLAG_SYS_WEATHER_CTRL` and `FLAG_LEGENDARIES_IN_SOOTOPOLIS` are cleared and `VAR_SKY_PILLAR_STATE` becomes 3 (from the Pokémon Center door, warps to (43,32)) or 2 (arrived by Dive, warps to (29,53);
  `OnWarp` then turns it into 3). Map music restored. All residents get "Rayquaza" lines (state 5).
- **Original text (gist):** residents: "What is that green POKéMON?!" / "That flying POKéMON came down from the sky and stopped the rampaging POKéMON…" / "It's the green one that settles
  things! Talk about a huge turn of events!" / "It was you who brought that flying POKéMON here? Aren't you amazing!" Guard: "The clash between the two awakened POKéMON was quelled by the
  awakening of a third POKéMON…" STEVEN: "So that's RAYQUAZA… It's incredible how the two rampaging POKéMON would flee from it in fear…"
- **HC:** the sky titan descends on the crater, **both other titans flee**, and the storm/drought ends at once. Citizens credit the player.

### C43 — Sootopolis aftermath: the leaders leave, Wallace gives Waterfall, the gym opens [M]
- **Mechanics:** state 5 positions: Wallace **on the gym door** (31,33), Steven (29,33), Maxie (33,35), Archie (34,35), guard back on the cave (31,18).
  - Talk to Maxie → speech, `FLAG_MET_MAXIE_SOOTOPOLIS`. Talk to Archie → speech, `FLAG_MET_ARCHIE_SOOTOPOLIS`. When **both** are set (either order):
    `FLAG_HIDE_SOOTOPOLIS_CITY_MAXIE/ARCHIE`, **`FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE`**, Mt. Pyre summit Maxie/Archie shown, **`VAR_MT_PYRE_STATE = 2`**, and a silent warp
    to (31,34) so the map reloads with the **gym door unlocked** and the houses open.
  - Wallace: before the leaders leave → "I don't think they meant harm. It wouldn't hurt to hear what they have to say." After → gives **`ITEM_HM_WATERFALL`**
    (`FLAG_RECEIVED_HM_WATERFALL`) and steps aside (`VAR_SOOTOPOLIS_WALLACE_STATE` 1 or 2). Steven: "It looks like both MAXIE and ARCHIE have gone away somewhere. Perhaps they've gone to MT. PYRE to return those ORBS…"
- **Original text (gist):** MAXIE: "So the super-ancient POKéMON weren't only GROUDON and KYOGRE… After all our fruitless scheming and frantic efforts, that one POKéMON's simple
  action puts everything right again as if nothing had happened… Fuhahaha…" ARCHIE: "KYOGRE and GROUDON both flew off to who knows where. The weather in HOENN has returned to
  its normal state… Maybe what we were trying to do was something small, even meaningless, to POKéMON…" WALLACE: "My eyes didn't deceive me. Thanks to your help, SOOTOPOLIS…
  No, all of HOENN was saved. On behalf of the people, I thank you. This is a gift from me." → "That HIDDEN MACHINE contains WATERFALL. If you have the RAIN BADGE… you have to beat
  the SOOTOPOLIS GYM LEADER. When you're all set to go, step through that door." → "I'm sure that you will be dazzled by my mentor's breathtakingly elegant battle style."
- **HC:** the two humbled leaders say a last word each and **leave together** (the gym unlocks only after both have spoken). The ally rewards the player with the waterfall HM
  and points to the city gym, run by his mentor. Segment boundary.

### C44 — Mt. Pyre: the orbs are returned [O, after C43]
- **Map:** `MtPyre_Summit`. With `VAR_MT_PYRE_STATE == 2`, `OnTransition` puts Maxie (23,6) and Archie (22,6) at the shrine. Coord line (22–24, 9): they turn, walk past the player, and
  Maxie stops and faces the player ("MAXIE: {PLAYER}… … … …"), then both leave. `VAR_MT_PYRE_STATE = 3`. Old lady: "The two men who took the ORBS came back to return them on their own.
  Perhaps they are not so evil after all…" (`FLAG_RETURNED_RED_OR_BLUE_ORB`), later "The embodiments of the land, sea, and the sky…". The old man tells the **new legend**
  (green Pokémon of the sky).
- **HC:** the two former villains silently return what they stole. One of them looks at the player without speaking.

### C45 — Rayquaza at the Sky Pillar [O, after C42]
- **Map:** `SkyPillar_Top`. With `VAR_SKY_PILLAR_STATE ≥ 2` and not `FLAG_DEFEATED_RAYQUAZA`, `RAYQUAZA_STILL` (14,6) is shown: talk → `setwildbattle SPECIES_RAYQUAZA, 70` via
  `BattleSetup_StartLegendaryBattle` (catch, defeat or run all remove it; running shows "flew away"). From now on the tower uses the **normal (crumbled) layouts**: 2F has 38 cracked
  tiles, 4F has 34, the top has 42 cracked tiles and 80 holes. The Mach Bike is needed.
- **HC:** after the crisis the sky titan returns to the tower top and can be challenged at L70. The tower is now crumbling.

### Boundary — next segment
Juan's gym (`SootopolisCity_Gym_1F`/`B1F`, ice-floor puzzle, badge 8, sets `VAR_SOOTOPOLIS_CITY_STATE = 6`, hides Steven/Wallace/residents), then Route 128 → Ever Grande.
The Sootopolis "new leader once mentored Wallace" hint (C26) and Wallace's lines set this up.

---

## 2. Optional side content in this segment (where and when it opens)

| Id | Where | Opens when | Mechanics | Original gist | HC |
|---|---|---|---|---|---|
| O1 | `Route121_SafariZoneEntrance` + `SafariZone_*` | after C15 (Pokéblock Case) | ¥500, 30 Safari Balls, `EnterSafariMode`, Pokéblock feeders (`EventScript_PokeBlockFeeder`), `VAR_SAFARI_ZONE_STATE` 0/1/2; Mach/Acro Bike areas; south-east expansion with construction workers is **post-game** (`FLAG_HIDE_SAFARI_ZONE_SOUTH_EAST_EXPANSION`) | "Filled with rare POKéMON!" | entry needs the case from the contest hall |
| O2 | `LilycoveCity_DepartmentStore_1F–5F`, `_Rooftop`, `_Elevator` | after the Lilycove rival battle (rival blocks the door) | 1F daily lottery (Loto-ID vs party/PC Pokémon), floors of shops (TMs, vitamins, decor, dolls), rooftop vending machines + random clearance sale; roof closed during C34–C40 | "Overflowing with great merchandise and excitement!" | — |
| O3 | `LilycoveCity_ContestLobby` / `ContestHall` | any time | 4 categories × ranks (Normal→Master); Master wins make paintings for the museum; Blend Master; Lilycove Lady in the Pokémon Center | — | the town is a contest capital |
| O4 | `LilycoveCity_LilycoveMuseum_1F/2F` | any time; 2F after a curator scene | curator tour, 2F empty frames filled by the player's Master-rank paintings, `ITEM_GLASS_ORNAMENT` after all 5 (`FLAG_RECEIVED_GLASS_ORNAMENT`); `VAR_LILYCOVE_MUSEUM_2F_STATE` | "I'm the CURATOR of this MUSEUM of fine arts." | — |
| O5 | `LilycoveCity_MoveDeletersHouse`, `House2` (TM Rest), `House3` (Pokéblock tips / link kids), `PokemonCenter_1F` | any time | utility NPCs | — | — |
| O6 | `Route120` (Scorched Slab (19,23)), `Route120` berry beauty (daily berry by Trainer ID), Route 120 Kecleon ×5 | after C07 | Surf to the slab cave | — | — |
| O7 | `Route123` + Berry Master's house | after C17 (from Route 122) | one-way west; berries; Giga Drain TM | — | — |
| O8 | `Route124_DivingTreasureHuntersHouse` | after C24 | trades shards (found underwater) for evolution stones | — | — |
| O9 | `ShoalCave_*` (Route 125) | after C24 | tide changes on 6-hour cycles of the real-time clock (`UpdateShoalTideFlag`, `src/time_events.c`); 4 Shoal Salt + 4 Shoal Shell → `ITEM_SHELL_BELL` (repeatable); `ITEM_FOCUS_BAND` from a black belt; ice room items | "The penetrating cold… With this FOCUS BAND, buckle down!" | — |
| O10 | `MossdeepCity_*` houses | after C26 | Super Rod (`House3`), King's Rock boy ("I got this from STEVEN… keep it a secret"), Dynamic Punch tutor, Wingull mail errand with Fortree (`FortreeCity_House4` ↔ `MossdeepCity_House2`, Mental Herb), secret-base info | — | — |
| O11 | `Route127`–`Route131`, `PacifidlogTown` | after C24 (open sea from Mossdeep south) | trainers; Pacifidlog: Return/Frustration TM by friendship (Fan Club chairman's brother), Horsea↔Bagon trade, Mirage Island watcher, "Six dots open three doors" Regi hint, Sky Pillar hint | "The sea between PACIFIDLOG and SLATEPORT has a fast-running tide." | — |
| O12 | `Route132`–`Route134` | after O11 | **westward currents** (≈1200 tiles per route): one-way Pacifidlog → Slateport | — | one-way current back to the start of the region |
| O13 | `Route130` Mirage Island | random daily (`IsMirageIslandPresent`) | Liechi berry + wild Wynaut | — | — |
| O14 | `AbandonedShip_*` (Route 108) | surface part since segment B; hidden floor after C32 (Dive + badge 7) | S.S. Cactus: trainers, Storage Key (captain's office) → storage room; underwater hidden floor with Room 1/2/4/6 keys and sparkle hints; `ITEM_SCANNER` → Stern's aide asks to deliver it → Stern (Slateport Harbor, badge 7) trades it for Deep Sea Tooth or Scale (`FLAG_EXCHANGED_SCANNER`) | "This ship is called S.S. CACTUS. It seems to be from an earlier era." | — |
| O15 | `Underwater_Route134` → `Underwater_SealedChamber` → `SealedChamber_OuterRoom/InnerRoom` | after C32 (Dive) | braille alphabet walls; "DIG HERE." → use Dig on the wall (`FLAG_SYS_BRAILLE_DIG`); inner room: "FIRST COMES WAILORD. LAST COMES RELICANTH." → party slot 1 Wailord + last slot Relicanth → long quake, "a door opened far away" (`FLAG_REGI_DOORS_OPENED`) | braille: "IN THIS CAVE WE HAVE LIVED. WE OWE ALL TO THE POKEMON. BUT, WE SEALED THE POKEMON AWAY. WE FEARED IT. THOSE WITH COURAGE, THOSE WITH HOPE. OPEN A DOOR. AN ETERNAL POKEMON WAITS." | ancient people sealed three guardians away |
| O16 | `DesertRuins` (Route 111), `IslandCave` (Route 105), `AncientTomb` (Route 120) | after O15 (doors opened on all three routes) | each tomb has a braille wall: Desert Ruins "LEFT, LEFT, DOWN, DOWN. THEN, USE ROCK SMASH." (Rock Smash at (5–7,23)); Island Cave "STAY CLOSE TO THE WALL. RUN AROUND ONE LAP." (walk the perimeter, return to the wall); Ancient Tomb "THOSE WHO INHERIT OUR WILL, SHINE IN THE MIDDLE." (**Flash** at (8,25)); each opens an inner wall → static L40 Regirock / Regice / Registeel (`StartRegiBattle`) | — | braille text is fixed-format (`.braille`, `data/text/braille.inc`) |
| O17 | Gabby & Ty (Route 118 / 120) | after their segment-B battle | TV duo double battles cycling Route 111 → 118 → 120 (`data/scripts/gabby_and_ty.inc`) | — | — |

---

## 3. Content on these maps that belongs to LATER segments (keep it consistent, don't write it here)

- **Sootopolis Gym** (Juan, ice floors, badge 8), Wallace as Champion, Steven as post-game.
- **Steven's House post-game:** letter + Beldum (`FLAG_HIDE_MOSSDEEP_CITY_STEVENS_HOUSE_BELDUM_POKEBALL` cleared by the Hall of Fame; "I've decided to do a little soul-searching and train on the road… take the POKé BALL on the desk… BELDUM, my favorite POKéMON. STEVEN STONE").
- **Lilycove Harbor** S.S. Tidal (post-game: Slateport, Battle Frontier, event islands), Lilycove Pokémon Trainer Fan Club (`VAR_LILYCOVE_FAN_CLUB_STATE`, after the Champion), motel game designers (complete Dex diploma), Lilycove rival removed by the Hall of Fame.
- **Abnormal-weather legendaries** (post-game): the Weather Institute 2F scientist reports drought/rain spots (`CreateAbnormalWeatherEvent`); Terra Cave (warps on Route 118 east and other land routes, Groudon) and Marine Cave (`Underwater_Route125/127/129`, Kyogre).
- **Southern Island**, Safari Zone expansion, Mossdeep Space Center launch counter, Mt. Pyre "new legend", Pacifidlog rival lines.
- The Route 119 waterfall item area (needs HM Waterfall).

---

## 4. Notable non-story NPCs and systems (what they do mechanically)

| Where | NPC / system | Mechanics |
|---|---|---|
| Weather Institute 1F | Bed | Free full heal (bg event) |
| Route 119 / 120 / Fortree | Invisible Kecleon (×2 / ×5+1 / ×1) | Need `ITEM_DEVON_SCOPE`; wild L30 battle (Fortree one flees) |
| Fortree Gym | Rotating gates | `RotatingGate_*` puzzle; follower hidden |
| Fortree | Houses | Plusle↔Volbeat trade (`House1`), TM Hidden Power (`House2`), Wingull mail errand → Mental Herb (`House4`), decoration shop |
| Route 120 | Berry beauty | Daily berry (by Trainer ID digit) |
| Route 123 | Berry Master + wife, Giga Drain girl | Berries daily; TM if a Grass-type is in the party |
| Lilycove | Berry gentleman, school kid (Berry Blender), art dealer, Kanto tourist | Flavour + daily berry |
| Lilycove Contest Lobby | Receptionist | First visit gives Pokéblock Case (Safari gate) |
| Lilycove Dept Store | Lottery, shops, roof sale | Daily lottery by ID number |
| Lilycove Motel | Owner, Scott [O] | Scott hidden after the harbor scene |
| Mt. Pyre 1F | Old woman | `ITEM_CLEANSE_TAG` |
| Mt. Pyre summit | Old man | Legend (yes/no), new legend after the crisis |
| Aqua Hideout B1F | Fake item balls | Two are Electrode L30, two are Master Ball + Nugget |
| Mossdeep | King's Rock boy, Super Rod man, Dynamic Punch tutor, Space Center Sun Stone sailor | Gifts / tutor |
| Mossdeep Gym | Floor switches + statues | Rotating-tile puzzle (`MossdeepCity_Gym_EventScript_*FloorSwitch`); the Ruby/Sapphire switch scripts are unused leftovers |
| Sootopolis | Kiri (daily 2 berries), TM Brick Break (`House1`), Lotad/Seedot size contest, Mystery Events house (e-Reader trainers) | Daily / side |
| Pacifidlog | Fan Club chairman's brother | Weekly TM Return or Frustration by lead Pokémon friendship |
| Shoal Cave | Shell Bell man, Focus Band black belt | Tide-dependent collectibles |
| Abandoned Ship | Stern's aide | Scanner delivery → Stern trade (after badge 7) |
| Route 124 | Treasure hunter | Shards → stones |
| PokéNav | Story calls | Mr. Stone 7–10, Steven 3–6, rival 8–14, Scott 3–5, Wally 4–6, Norman 5 unlock on the milestone flags (§0). Mr. Stone 8–9 are "bad line" gags ("GROU… DON?", "Seaflo… Caver…?"). Steven 6 and Wally 6 = unreachable. Wally's location becomes "unknown" after Groudon awakens. Texts in `data/text/match_call.inc` |
| Route trainers (normal sight battles) | — | Route 118 east 3, Route 119 17 (+rival), Route 120 13, Route 121 10, Route 123 15, Route 124 8, Route 125 8, Route 126 8, Route 127 8, Route 128 7, Route 129 5, Route 130 3, Route 131 7, Route 132 8, Route 133 7, Route 134 9, Mt. Pyre 2F–6F 11, Abandoned Ship 7 |

---

## 5. Character index for this segment

| Original name | Overworld gfx | Battle class / pic | Role in segment C | What the script needs from them |
|---|---|---|---|---|
| Steven | `STEVEN` | partner `PARTNER_STEVEN` (Rival class, pic Steven) | Mentor: Route 118 (opt.), Route 120 (Devon Scope), Space Center 1F/2F (tag partner), home (Dive), Route 128 (flies in/out), Sootopolis guide | Flies away 3 times (`FLDEFF_NPCFLY_OUT`), is the player's battle partner once |
| Rival (May/Brendan) | `VAR_0` / bike `VAR_3` | Rival | Route 119 battle + HM Fly; Lilycove battle (door blocker), goes home; phone call about the green Pokémon | Rides in on a bike from the west; flies away in Lilycove |
| Scott | `SCOTT` | — | Scout: Route 119 after the rival, Fortree phone call, Lilycove motel [O], Mossdeep beach [O] | `VAR_SCOTT_STATE` increments (feeds the Battle Frontier later) |
| Winona | `WINONA` | Leader | Gym 6 (flying) | Rotating gates gym |
| Tate & Liza | `TATE`, `LIZA` | Leader (double) | Gym 7 (psychic twins) | Double battle; the badge triggers the Space Center raid |
| Wallace | `WALLACE` | — (Champion later) | Former leader of Sootopolis, guardian of the Cave of Origin | Asks the Rayquaza question; opens the Sky Pillar; gives HM Waterfall; blocks the gym door until then |
| Archie (leader A) | `ARCHIE` | Aqua Leader (fought **once**, Seafloor) | Takes the Red Orb (Mt. Pyre), steals the sub (Slateport), awakens Kyogre, remorse, leaves with Maxie, returns the orb | No battle at Mt. Pyre or harbor; one battle at Room 9 |
| Shelly | `AQUA_MEMBER_F` (generic) | Aqua Admin, pic Aqua Admin F | Weather Institute boss; Seafloor Cavern rematch | No unique overworld sprite |
| Matt | `AQUA_MEMBER_M` (generic) | Aqua Admin, pic Aqua Admin M | Aqua Hideout last guard | Stalls while the sub leaves |
| Aqua grunts | `AQUA_MEMBER_M/F` | Team Aqua / Aqua Grunt M/F | Lookouts, Institute, Route 121 squad, Lilycove, Mt. Pyre, hideout, harbor, Seafloor | Messenger who shoves the player (C04) |
| Maxie (leader B) | `MAXIE` | Magma Leader | Awakens Groudon (fought), Space Center (fought in tag battle), confronts Archie, remorse, leaves with him, returns the orb | 2 battles; becomes a reluctant ally after C30 |
| Tabitha | `MAGMA_MEMBER_M` (generic) | Magma Admin | Magma Hideout 4F, Space Center 2F (tag battle) | No unique overworld sprite |
| Magma grunts | `MAGMA_MEMBER_M/F` | Team Magma / Magma Grunt M/F | Hideout, Mossdeep street, Space Center, Room 9 escort | Polite "intent-to-steal notice" |
| Capt. Stern | `SCIENTIST_1` | — | TV interview, sub stolen, Dive hint, Scanner trade | Stands in front of the harbor door |
| Gabby & Ty | `REPORTER_F` / `CAMERAMAN` | Interviewer | Interview crew in Slateport (C22) + side battles | — |
| Weather scientist + workers | `SCIENTIST_1`, `MAN_4` | — | Hostages; Castform gift | — |
| Mt. Pyre old lady / old man | `EXPERT_F` / `OLD_MAN` | — | Shrine keepers: give the emblem; tell the legend | Old lady changes lines 4 times |
| Cave of Origin guard | `EXPERT_M` | — | Blocks the cave except during C38–C40 | Steps aside when Steven arrives |
| Groudon | `GROUDON_ASLEEP` / `_FRONT` / `_SIDE` | (not battled here) | Red land titan | Awakens in the volcano, fights in Sootopolis, flees |
| Kyogre | `KYOGRE_ASLEEP` / `_FRONT` / `_SIDE` | (not battled here) | Blue sea titan | Awakens under the sea, fights in Sootopolis, flees |
| Rayquaza | `RAYQUAZA`, `RAYQUAZA_STILL` | wild L70 [O] | Green sky titan | Sleeps on the tower, ends the fight, catchable later |
| Castform, Kecleon, Electrode | — | wild/gift | Gift / camouflaged blockers / trap items | — |

Cutscene-specific assets that must be kept or replaced 1:1: `Script_DoRayquazaScene` (`src/rayquaza_scene.c`, `graphics/rayquaza_scene/`),
the orb light effect (`DoOrbEffect`), `SUBMARINE_SHADOW`, `LAYOUT_SOOTOPOLIS_CITY_LEGENDS_BATTLE`, Lilycove Wailmer metatiles, and the Sky Pillar `_CLEAN` layouts.

---

## 6. Short re-interpretation checklist (what the writers MUST keep)

1. On the long rainy route north, two villain lookouts block the bridge by a weather research building. Faction A's female admin holds the scientists there. After her defeat
   **a messenger reports that faction B went to the mountain**, and faction A leaves at once. The scientists give a weather-changing Pokémon.
2. The rival intercepts the player after the bridge, battles, and gives the flying HM (usable with the next badge). The scout shows up right after.
3. Invisible creatures block the treetop gym and a bridge. The mentor reveals one with his company's gadget, fights it with the player watching, gives the gadget and flies off.
4. A flying-type leader (badge 6). The badge order is flexible, so nothing later may depend on it except flying.
5. Faction A squads head for the cemetery mountain. In the port city, faction A trains sea creatures that block the harbor, has a base in a cove cave, and is blamed for thefts.
   The rival blocks the department store until battled, then goes home. Several people mention **a tall tower near Route 131 and a green creature in the sky**.
6. At the mountain shrine, **faction B took the first sacred item off-screen** and dropped an emblem. **Faction A's leader takes the second item in front of the player**, does not fight, and leaves.
   The keeper gives the emblem to the player and tells the legend of a land titan and a sea titan calmed by the two items.
7. The emblem opens a hidden door on the volcano path. In the volcano base, **faction B's leader wakes the sleeping red titan with his item; it flees and does not obey**. He loses to the player and leaves to chase it.
8. Back in the first port, the ocean explorer announces an **undersea cave on Route 128** on TV. Faction A seizes his submarine by megaphone, and its leader sails away, inviting the player to the cove base.
9. In the warp-panel base the player arrives too late: an admin stalls and the sub leaves "for a cave under the sea". The harbor creatures go away and the sea route opens. The base holds the Master Ball and Electrode traps.
10. The island city (badge 7, psychic twins, double battle). **The badge triggers faction B's raid on the space center**, announced by a polite letter, to steal rocket fuel.
11. The space center: guarded stairs, 3-grunt gauntlet (refusable), then **a tag battle with the mentor** against faction B's leader and admin. The leader reveals the eruption plan, loses,
    **doubts both factions' goals and gives up**. The mentor invites the player home and gives the diving HM.
12. Under Route 128: the stolen sub, a cave needing boulders/rocks/currents, faction A's admin again, then **faction A's leader fights the player before the sleeping blue titan; his item lights up by itself;
    the titan escapes; a radio report says the rain is out of control; faction B's leader arrives and both leaders run out together.**
13. On the island outside, one leader breaks down and the other takes joint responsibility. The mentor flies in, names the heat wave then the deluge, and goes to the crater city.
14. Eastern Hoenn is in crisis weather until the end of the arc.
15. In the crater city both titans fight in the lake while their leaders plead. The mentor leads the player to the guarded **cave of the spirits**, where a regal ally (the city's former leader)
    explains the **third, sky titan** and **asks the player where it is** (4 fixed options, the tower is the right one).
16. The ally opens the ancient tower, an earthquake hits, and he returns to the city. The player climbs alone. **At the top the green titan wakes and flies off with no battle.**
17. It descends on the crater city, the other two flee, and the weather returns to normal. The two leaders speak a last time and leave together (both must be talked to). The ally gives the waterfall HM
    and opens the way to the city gym run by his mentor.
18. Afterwards [O]: the leaders silently return the two items to the shrine; the sky titan waits on the crumbling tower (L70); ancient braille chambers under the sea open the three guardians' tombs.
