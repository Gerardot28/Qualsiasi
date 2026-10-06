# emu: headless GBA test harness

This harness boots a ROM in mGBA with no window, no audio and no frame limiter. It feeds the ROM
button inputs from a small script language and writes PNG screenshots, savestates, memory reads
and crash/reset/stuck reports. It runs at about 2000 to 2500 frames per second, which is 35 to 40
times real time. Runs are deterministic: the same ROM and the same script give byte-identical
screenshots.

The harness has three parts:

* `harness.c` is a small C program linked against **libmgba 0.10.5**. It is built from source
  without the Qt/SDL frontends or optional dependencies.
* `run.py` is the front end. It adds symbol and map-name support, script macros, `run.log` and
  `run.json`.
* `selftest.py` is a one-command smoke test for a pokeemerald-expansion build.

## Setup

```sh
bash tools/emu/setup.sh            # clone mGBA 0.10.5, build libmgba, compile the harness (~1 min)
bash tools/emu/setup.sh --harness  # recompile only harness.c
```

All build output goes under `/home/user/work/emu-build/`. You can change that with
`EMU_BUILD_DIR`, and the mGBA version with `MGBA_TAG`. The paths are:

* mGBA source: `mgba-src/`
* build directory: `mgba-build/`
* installed library: `prefix/lib/libmgba.so`
* harness binary: `bin/emu-harness`

The build needs `cmake gcc pkg-config libpng-dev zlib1g-dev`. Nothing is written into this
repository.

## Usage

```sh
python3 -I tools/emu/run.py --rom ROM.gba --script inputs.txt --out OUTDIR \
    [--save-state-out F] [--load-state F] [--sav F [--sav-readonly]] \
    [--elf pokeemerald.elf | --map pokeemerald.map] [--src SOURCE_TREE] \
    [--scale 2] [--rtc 2025-06-01T10:00:00] [--trace-crash] [--stop-on-crash] \
    [--allow-reset] [--stuck-frames N] [--timeout SEC] [--quiet] [-- extra emu-harness args]

python3 -I tools/emu/selftest.py --rom pokeemerald.gba --elf pokeemerald.elf
```

`OUTDIR` receives:

* the screenshots (`NAME.png`, 240x160 or scaled)
* states (`NAME.state`)
* `run.log`, which holds everything the harness printed
* `run.json`, a summary with the shots, reads, maps, events, expects/untils and the final `DONE`
  line

**Symbols.** An ELF is preferred, with `.map` as the fallback. If you don't pass `--elf`,
`ROM.elf` next to the ROM is used when it exists (for example `pokeemerald.gba` and
`pokeemerald.elf`). The ELF also contains `static` functions and the map header labels, so the
map names come straight from the ROM's `gMapGroups` table. `MAP_*` ids are read from
`data/maps/*/map.json` of `--src`. The default for `--src` is the tree containing the ROM, then
`/home/user/pex-orig`.

**Saves.**

* `--sav F` attaches a real cartridge save file. It is created if missing. For Emerald it is
  Flash 128K, and mGBA appends 16 bytes for the RTC, so the file is 131088 bytes. You can open it
  with any emulator.
* `--sav-readonly` loads the file but never writes it.
* Without `--sav`, a blank in-memory save is used. In-game saving still works for the whole run,
  but nothing touches the disk.

Save type and RTC are auto-detected. mGBA forces Flash1M+RTC for anything with the
"pokemon emerald version" header. If you need to override that, pass
`-- --savetype flash1m --force-rtc`.

**Determinism.**

* The harness never uses the BIOS (it uses mGBA's HLE BIOS) and never reads config files.
* `TZ=UTC` is forced.
* The RTC starts at a fixed time (`--rtc`, default 2025-06-01T10:00:00) and advances with
  *emulated* time.
* Savestates store the full state, including Flash and RTC. A run that loads a savestate gives
  the same screenshots as the uninterrupted run. This was verified on all 23 later shots of
  `new_game_intro.txt`.

**Exit status.**

| Code | Meaning |
|------|---------|
| 0 | ok |
| 1 | error |
| 2 | script syntax error (nothing was run) |
| 3 | an `expect`, `until`, `waitstable` or `waitsavedata` failed or timed out |
| 4 | crash, stuck or unexpected reset detected |

## Script language

There is one command per line. `#` starts a comment. Frame counts are emulated frames (59.73
per second). Keys are `A B START SELECT UP DOWN LEFT RIGHT L R`, and you can combine them with
`+`, as in `A+B+START+SELECT`.

### Input and timing

| command | effect |
|---|---|
| `wait N` | run N frames |
| `press KEY [N [AFTER]]` | hold KEY for N frames (default 6), release, then wait AFTER frames (default 10) |
| `hold KEY N` | hold KEY for N frames, then release (no extra wait) |
| `keydown KEY` / `keyup KEY` / `release` | keep keys held across commands, release some, release all |
| `mash KEY TOTAL [PERIOD [HOLD]]` | for TOTAL frames, tap KEY every PERIOD frames (default 20), holding it HOLD frames (default 5) |
| `autotap KEY [PERIOD [HOLD]]` / `autotap off` | keep tapping KEY in the background during every following command |
| `reset` | power-cycle the console |
| `quit` | stop the script |

### Output

| command | effect |
|---|---|
| `shot NAME [SCALE]` | save `OUTDIR/NAME.png` (native 240x160, or SCALE 2..8 with nearest neighbour; `--scale` sets the default) |
| `savestate NAME` / `loadstate NAME` | `OUTDIR/NAME.state`; a NAME containing `/` is a path |
| `dump NAME ADDR LEN` | raw memory to `OUTDIR/NAME.bin` |
| `echo TEXT` | print TEXT |
| `regs` | print the CPU registers |

### Memory

`ADDR` is an expression made of numbers, symbols (`gMain`), `+`/`-` and `[x]`, where `[x]`
dereferences a 32-bit word. For example, `[gSaveBlock1Ptr]+4` is `gSaveBlock1Ptr->location`.
A `VALUE` written as `!V` means "not equal to V".

| command | effect |
|---|---|
| `read8/16/32 ADDR [LABEL]` | print the value |
| `write8/16/32 ADDR VALUE` | poke memory |
| `expect8/16/32 ADDR [!]VALUE [LABEL]` | the run fails (exit 3) on a mismatch |
| `until8/16/32 ADDR [!]VALUE MAXFRAMES [LABEL]` | run frames until the value matches |
| `untilany8/16/32 BASE STRIDE COUNT [!]VALUE MAXFRAMES` | same as `until`, but succeeds if any of COUNT elements matches |
| `waitstable N MAXFRAMES [X Y W H]` | run until the screen region shows **no new picture** for N frames. Repeating animations up to 64 frames long, such as the blinking text arrow, are ignored. Use it to wait until text has finished printing. |
| `waitsavedata MAXFRAMES` | run until the game has finished writing its save memory |

### Loops

`loop MAX` … `endloop` repeats the block. Inside a loop, `breakif8/16/32 ADDR [!]VALUE` and
`breakifany32 BASE STRIDE COUNT VALUE` leave it. `{n}` in a shot, state, dump or echo name is
replaced by the iteration number (`001`, `002`, …).

### Macros (`run.py`, pokeemerald-specific, need symbols)

| macro | meaning |
|---|---|
| `include FILE` | splice another script; the path is relative to the including script |
| `mapinfo` | print `MAP frame=… group=G num=N name=MAP_… label=…` plus the player x/y |
| `waitmap MAP [MAX]` / `expectmap MAP` | wait for, or assert, the current map. Accepts `MAP_LITTLEROOT_TOWN`, `LittlerootTown` or `0.9`. |
| `waitcb2 FUNC [MAX]` / `expectcb2 FUNC` | `gMain.callback2 == FUNC`, e.g. `CB2_Overworld` |
| `waittask FUNC [MAX]` | some `gTasks[i].func == FUNC`, e.g. `Task_NewGameBirchSpeech_ChooseGender` |
| `breakiftask FUNC…` / `breakifcb2 FUNC…` | loop exits on those conditions |
| `textbox NAME [MAX]` | `waitstable` on the message box (y 112..159), then `shot NAME` |

Pattern to screenshot **every** message box of a dialogue, whatever its length or language:

```
loop 120
  textbox prof_{n}
  breakiftask Task_NewGameBirchSpeech_WaitToShowGenderMenu
  press A
endloop
```

## Crash / reset / stuck detection

Detection is always on. Each finding is printed as an `EVENT` line and makes the run exit with
status 4.

* `EVENT crash`: an undefined or illegal opcode (with the exact PC), a jump to unmapped memory
  ("Jumped to invalid address"), or an unimplemented opcode.
* `EVENT reset`: a `SoftReset` SWI (0x00, which includes the A+B+START+SELECT soft reset), a
  `HardReset` SWI, or `gMain.vblankCounter1` going backwards. The last check catches a game that
  re-initialises without using the SWI. Pass `--allow-reset` to report resets without failing
  the run.
* `EVENT stuck`: the game made no Halt/IntrWait/VBlankIntrWait call for `--stuck-frames` frames
  (default 600, about 10 s). The event names the hottest code address. A normal new game plus a
  save peaks at 214 frames without a wait, during the Flash write.
* `EVENT savedata`: the game finished writing its save memory. This is informational.

`--trace-crash` adds a dump of the registers and the last 64 PC/SWI samples, plus
`crash_fNNN.png` and `crash_fNNN.state` (or `stuck_…` / `reset_…`) taken at the end of the frame
of the first event. You can load the state in any mGBA 0.10 to inspect it.

`--stop-on-crash` aborts at the first event. Without it, the script continues. A ROM that keeps
executing undefined opcodes slows down to a few hundred frames per second.

Emulator log messages work like this:

* `fatal`, `error` and `warn` messages are streamed as `LOG` lines.
* "Game errors" (DMA from address 0, writes to read-only registers, …) happen in retail games
  too. They are only summarised at the end as `LOGSUM count=…` lines. Use `-- --log-game-errors`
  to stream them.

These were tested with patched baseline ROMs, each patched at `Task_NewGameBirchSpeech_Init`:

| Patch | Result |
|---|---|
| thumb `udf` | `EVENT crash` at PC 0x0817A7EC in frame 1286 |
| `b .` | `EVENT stuck`, hottest code 0x0817A7E0-EF (95% of samples) in frame 1886 |
| `swi 0` | one `EVENT reset` |
| jump to 0x0F000000 | `EVENT crash` "Jumped to invalid address: 0F000000" |

The unpatched baseline reports no events.

## Provided scripts (`scripts/`)

All frames below are absolute, counted from power-on, on the unmodified pokeemerald-expansion
1.17.1 baseline.

**`boot_to_title.txt`**

| Frame | What happens / shot |
|---|---|
| 0-120 | copyright screen |
| 170-430 | "Powered by pret x RHH" splash; `boot_splash` at f300 |
| 480+ | Game Freak logo, then the intro; `boot_intro` at f600 |
| f600 | `press START` skips the intro |
| f816 | `title_logo` |
| f1044 | title screen fades in |
| f1100 | `title` / `title_2x`: full title with PRESS START. PRESS START blinks; it is visible e.g. f1060-1124. |

**`new_game_to_truck.txt` + `new_game_intro.txt`** use pure frame timings.

| Frame | What happens / shot |
|---|---|
| f1100 | START |
| f1206 | `ng_00_main_menu` (NEW GAME / OPTION), then A |
| f1942-4642 | Birch's speech, A every 60 frames. Text-complete shots: f1986 `ng_01_prof_hi`, f2166 `_welcome`, f2466 `_professor`, f2706 `_this_is_a_pokemon` (Lotad), f2946 `_inhabited`, f3186 `_playmates`, f4086 `_secrets`, f4326 `_research`, f4506 `_and_you_are` |
| f4842 | `ng_10_gender_menu`; A selects BOY |
| f4978 | `ng_11_whats_your_name` |
| f5084 | `ng_12_naming_screen`; START then A keeps the preset name |
| f5236 | `ng_13_so_its_name` ("So it's LANDON?"); A selects YES |
| f5656-6916 | closing speech: `ng_14`..`ng_18` |
| ~f7036 | `CB2_Overworld` |
| f7352 | `ng_19_truck` (MAP_INSIDE_OF_TRUCK) |
| f7952 | `ng_20_truck_door_open` |
| — | `hold RIGHT 80` |
| f8132 | `ng_21_littleroot` (MAP_LITTLEROOT_TOWN) |
| f8372 | `ng_22_littleroot_mom` ("MOM: LANDON, we're here, honey!") |

**`new_game_to_truck_robust.txt` + `new_game_intro_robust.txt`** follow the same path, but are
driven by game state (`waittask`/`waitcb2`/`waitmap`/`textbox`) instead of frame counts. They
capture **every** message box (`rb_prof_NNN`, `rb_closing_NNN`: 31 boxes on the baseline) and keep
working when the dialogue changes length, for example in Italian. They need the ELF. Littleroot is
reached at f8722.

**`save_in_truck.txt`** opens the START menu in the truck, then SAVE, then YES, and waits for the
Flash write (`waitsavedata`). **`continue_from_save.txt`** boots that save with CONTINUE and checks
that the game resumes in MAP_INSIDE_OF_TRUCK. Example:

```sh
python3 -I tools/emu/run.py --rom pokeemerald.gba --script tools/emu/scripts/save_in_truck.txt \
    --out /home/user/work/emu-out/save --sav /home/user/work/emu-out/save/game.sav
python3 -I tools/emu/run.py --rom pokeemerald.gba --script tools/emu/scripts/continue_from_save.txt \
    --out /home/user/work/emu-out/cont --sav /home/user/work/emu-out/save/game.sav --sav-readonly
```

`selftest.py` runs boot, robust new game, save, continue and a determinism check in about 18 s.
It passed 5/5 on the baseline.

## Notes and limits

* There is no audio output (audio is still emulated, then discarded). Video uses mGBA's
  software renderer. Screenshots are the exact frame buffer that mGBA shows, with 5-bit colour
  expanded to 8-bit.
* Frame-timed scripts break when text length, text speed or intro length changes. Use the
  `_robust` scripts, or `textbox` and `waittask`, for anything touching dialogue.
* The START-menu position of SAVE (`DOWN DOWN`) assumes the vanilla menu layout in the truck.
* Benign "game errors" also appear on the unmodified baseline, so don't mistake them for
  regressions:
  * `Bad memory Store8: 0x000001E8` during Lotad's appearance (f2754)
  * `Bad BIOS Load*` / `Bad memory Load8: 0xe3a02005` on the naming screen
  * `Bad BIOS Load8: 0x0000000A` (frequent)
