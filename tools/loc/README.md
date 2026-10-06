# tools/loc — C-source localization helpers

## ctextcheck.py

Validates translated game strings in the C sources of pokeemerald-expansion
(`/home/user/pex`) against the pristine tree (`/home/user/pex-orig`).
It imports `../textcheck/textcheck.py` for the charmap, the preproc string parser and the
font-width renderer, so it uses the same code as the script-text validator.

```
python3 -I tools/loc/ctextcheck.py [--root /home/user/pex] [--orig /home/user/pex-orig] FILE...
python3 -I tools/loc/ctextcheck.py --single NEW ORIG [--single NEW2 ORIG2] [--as src/data/items.h]
options: --json   --widths (list per-line widths of every changed string)
         --all (check unchanged strings too)   --no-warn   --allow PATH (default tools/loc/allow.json)
```
FILE paths are relative to `--root`. With `--single`, the rules for the file are worked out from the
ORIG name: a path under `--orig`, or a split part such as `src__data__items.h.part3` → `src/data/items.h`.
You can also set them directly with `--as`. The exit code is 1 when there is at least one ERROR.

### Which strings it treats as game strings
These are the string literals inside `_(...)`, `__(...)`, `COMPOUND_STRING(...)`, `ITEM_NAME(...)`,
`ITEM_PLURAL_NAME(...)` and `COMPOUND_STRING_SIZE_LIMIT(..., n)`. Those are the forms that
`tools/preproc/c_file.cpp` converts after cpp. Adjacent literals (`"a" "b"`, across lines, with
comments in between) count as one string. You may re-split a literal across lines.

### Checks
| check | severity | rule |
|---|---|---|
| STRUCTURE | error | Blank every game-string argument to `""`; the file must then match the original byte for byte. The first difference is reported with the line in both files. |
| CHARMAP | error | Every character, `\n \l \p` escape and `{TOKEN}` must be encodable. This uses the textcheck port of `string_parser.cpp`, the same parser preproc uses for C files. Unsupported characters get a suggested replacement. |
| TOKENS | error / warning | Functional `{…}` codes are placeholders and control codes, meaning any code whose bytes contain F7/F8/FA–FF. Each one must appear as many times as in the original. A missing or newly added code is an ERROR. A changed order is a WARNING. Glyph-only codes such as `{PKMN}` may be added or removed freely. |
| WIDTH | error | Each displayed line (split on `\n \l \p`) is measured in FONT_NORMAL. Limits by file: `src/battle_message.c` allows max(widest original line, 208), or 200 for a line ending in `\p`/`\l` because of the down arrow. Name fields that have a LENGTH rule allow max(widest original line, byte limit × 6 px). Everything else allows the widest original line, so no growth, unless `allow.json` sets a value. FONT_SMALL and FONT_NARROW widths also appear in `--widths` and `--json`. |
| LENGTH | error | Encoded bytes, terminator excluded. See the table below. If the original is already longer, as with Z-move names, the limit becomes the original length. |
| LINES | warning | More lines in one box or paragraph than the original has. For battle text the cap is 2. |

Length limits come from `include/constants/global.h` and the struct field sizes:

| where | field | max bytes |
|---|---|---|
| src/data/items.h | `ITEM_NAME(...)` | ITEM_NAME_LENGTH−1 = 19 (`COMPOUND_STRING_SIZE_LIMIT`: sizeof incl. EOS ≤ 20) |
| src/data/items.h | `ITEM_PLURAL_NAME(...)` | ITEM_NAME_PLURAL_LENGTH−1 = 21 |
| src/data/moves_info.h | `.name` | MOVE_NAME_LENGTH = 16 |
| src/data/abilities.h | `.name` | ABILITY_NAME_LENGTH = 16 (`u8 name[17]`) |
| species_info/*.h | `.speciesName` / `.categoryName` | POKEMON_NAME_LENGTH = 12 / 12 (`u8 categoryName[13]`) |
| src/data/types_info.h | `.name` / `.generic` | TYPE_NAME_LENGTH = 8 / 16 |
| src/battle_main.c | `gTrainerClasses` | 12 (`u8 name[13]`) |
| src/berry.c | `.name` | BERRY_NAME_LENGTH = 6 |
| src/data/decoration/header.h | `.name` | 15 (`u8 name[16]`) |

A fixed `u8 name[N] = _("...")` that is too long does not fail the build. GCC only warns about
"excess elements" and silently drops the terminator.

### Assumed placeholder widths
Widths are characters × 6 px. They matter mostly for battle text, because the limit there is absolute.

| placeholder | chars |
|---|---|
| `{B_*_NAME_WITH_PREFIX*}` (e.g. "Il Pikachu nemico" → assumed 13) | 13 |
| `{B_*_NAME_WITH_CLASS}` | 20 |
| `{B_*_MON*_NAME}`, `{B_DEF_NAME}`, `{B_*_PARTNER_NAME}` | 10 |
| `{B_CURRENT_MOVE}`, `{B_LAST_MOVE}`, `{B_LAST_ITEM}`, `{B_*_ABILITY}`, `{B_*_CLASS}` | 12 |
| `{B_BUFF1-3}`, `{B_COPY_VAR_n}`, `{STR_VAR_1-3}`, `{B_*_TEAMn}`, others | 10 |
| `{PLAYER}`, `{RIVAL}`, trainer/link names `{B_*_NAME}` | 7 |
| `{B_*_PREFIXn}` | 6 |
| `{KUN}`, `{VERSION}`, `{AQUA}`, ... | real width of the string in src/strings.c |
| `{B_TRAINER*_WIN/LOSE_TEXT}` | 0 |

Edit `PH_CHARS_EXACT` / `PH_CHARS_PATTERNS` / `PX_PER_CHAR` at the top of the script to change these values.

### allow.json
This file raises the width limit for chosen strings: `{"key": maxpx}`. Keys starting with `_` are ignored.
A key can be any of these forms:
- `src/strings.c:582`: the line of the string in the new file
- `src/strings.c:gText_SaveFailed`
- `gText_SaveFailed`, or a symbol as printed in reports: `gItemsInfo/ITEM_POTION.description`
- `ITEM_POTION.description`, `ITEM_POTION`
- a file path or glob (`src/data/items.h`, `src/data/pokemon/species_info/*.h`): applies to every string in the matching files

### Notes and limits
- When the structure check fails, the file's other checks are skipped. Fix the code first.
- A raw `"` inside the text ends the C literal and shows up as a STRUCTURE error. Use `“ ”` instead.
- Each string is paired with its original by position, which works because the structure is identical.
- Width for non-battle UI strings is relative to the original, so the window size does not matter.
  Use allow.json when a window is known to have room.
