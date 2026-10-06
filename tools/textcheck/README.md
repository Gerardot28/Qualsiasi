# textcheck: validator for the rewritten in-game text

`textcheck.py` compares the script and text files of the working tree (`--root`,
default `/home/user/pex`) with the pristine tree (`--orig`, default
`/home/user/pex-orig`). It reports everything that could break the build or
the display after the `.string "..."` lines have been rewritten (for example
the Italian rewrite).

Files checked: `data/maps/*/scripts.inc`, `data/scripts/*.inc`,
`data/text/*.inc`. That is 1009 files and 39,253 `.string` lines in
expansion 1.17.1.

```sh
# whole tree (about 10 s)
python3 -I tools/textcheck/textcheck.py
# only some files (relative to --root, globs allowed)
python3 -I tools/textcheck/textcheck.py data/maps/Route104/scripts.inc 'data/text/t*.inc'
# an agent's draft file against its original, given as explicit paths (about 2 s)
python3 -I tools/textcheck/textcheck.py --single /tmp/draft/scripts.inc /home/user/pex-orig/data/maps/Route104/scripts.inc
# machine-readable output
python3 -I tools/textcheck/textcheck.py --json > report.json
# compare against a git commit of the working tree instead of pex-orig
python3 -I tools/textcheck/textcheck.py --orig-rev <commit-before-the-rewrite>
```

Options: `--changed-only` skips byte-identical files. `--no-warnings` hides
warnings. `--strict` makes warnings fail the run. `--arrow-warning` reports
the ▼ arrow rule as a warning instead of an error. `--list-unclassified` lists
the labels whose display window is unknown. `--max-issues N` limits the
output. `--player-width` and `--strvar-width` change the placeholder widths.
`--calibrate` prints the statistics of the vanilla tree.

Exit status: `0` means no errors, `1` means errors were found, `2` means a
usage problem.

## Checks

| check | severity | what |
|---|---|---|
| STRUCTURE | error | The sequence of non-`.string` lines (labels, commands, `#if`/`.if`, directives) must be identical after whitespace normalisation, ignoring blank lines and comments. The tool reports the first differing line and the total count. It also flags `.string` lines added where the original has none, which would inject bytes into a script, and text blocks that were removed completely. |
| TERMINATION | error | Each text block (the `.string` run of a label, with `#if` branches evaluated separately) ends with `$` if and only if the original does. It may not contain more or fewer `$` than the original, and nothing may follow a `$` on the same line. |
| CHARMAP | error | A faithful port of `tools/preproc` (`charmap.cpp`, `string_parser.cpp`, `asm_file.cpp`). It catches unknown characters, `{UNKNOWN_CONST}`, `{X }` (preproc rejects whitespace before `}`), bad escapes, `\"`, a raw `"` (which ends the string, leaving junk at the end of the line), control characters such as TAB, decomposed accents (e+U+0300), invalid UTF-8, a missing closing quote, and more than 1024 bytes on one line. Each finding comes with a suggested replacement. |
| WIDTH | error | Pixel width of every displayed line (`\n`, `\l`, `\p` and `$` end a line). Widths use the `src/fonts.c` glyph tables, mirroring `src/text.c`: font switches, `{CLEAR_TO}`, keypad icons and so on. Each line is checked against the window the label is shown in (see Limits). |
| ARROW | error | A line followed by `\p` or `\l` needs 8 px for the ▼ "more text" arrow, which is drawn at `currentX`. Vanilla never violates this rule (see Calibration). |
| BOXLINES | error | In a 2-line box (field, battle, PokéNav), the first break of a paragraph must be `\n` and the following breaks `\l`. A second `\n` draws a 3rd row outside the window. For unclassified texts this is only a warning, raised when the text uses more rows than the original did. |
| LENGTH | error | Placeholders are expanded with no bounds check. A text over 1000 bytes overflows `gStringVar4[1000]`, and a battle lose/win text over 423 bytes overflows `gDisplayedStringBattle[425]`. Both corrupt memory. `{PLAYER}` counts as 7 bytes and `{STR_VAR_n}` as 20. |
| TOKENS | warning | `{STR_VAR_n}` added although the original never uses it (the script will not fill it), or a placeholder dropped. Fewer `{PLAY_BGM}`, `{PLAY_SE}`, `{COLOR}`, `{PAUSE}`, `{FONT_*}` and similar control codes than the original. |
| STYLE | warning | `‘` (opening quote) used as an apostrophe between letters. |

## Apostrophes and quotes (from `charmap.txt`)

| write | glyph/byte | notes |
|---|---|---|
| `'` or `’` | 0xB4 | Identical result. Both are valid apostrophes; vanilla uses `'`. |
| `‘` | 0xB3 | Opening single quote: a different glyph. Do not use it as an apostrophe. |
| `“` and `”` | 0xB1 and 0xB2 | The only double quotes. |
| `"`, `\"` | not in charmap | A raw `"` ends the string, and `\"` is a preproc error. Use `“ ”`. |
| `«` and `»` | not in charmap | Use `“` and `”`. |
| `` ` `` `´` `′` `ʼ` | not in charmap | Use `'`. |
| `–` and `—` | not in charmap | Use `-`. |
| `…` | 0xB0 | Valid, and `...` is valid too. |
| `à è é ì ò ù À È É Ì Ò Ù` | yes | Also `á í ó ú ç ñ ä ö ü` and others. Write them precomposed (NFC). |
| `°`, `€`, `#`, `*`, `@`, `[ ]` | not in charmap | The tool suggests replacements where they exist (for example `°` becomes `º`). |

## Limits and how they were determined

Window geometry, all with `FONT_NORMAL` and x = 0 unless stated:

* **field message box** (`msgbox`, `message`, trainer intro and "not enough
  Pokémon" texts, `ShowFieldMessage` users): `sStandardTextBox_WindowTemplates`
  in `src/menu.c` is 27 tiles wide, so the limit is **216 px**. A line before
  `\p` or `\l` may use at most **208 px**.
* **battle box** (trainer defeat and victory texts, shown via
  `B_TRAINER1_LOSE_TEXT`): `[B_WIN_MSG]` in `src/battle_bg.c` is 26 tiles, so
  the limit is **208 px** (200 px before `\p` or `\l`). Spaces in these texts
  become NBSP, so the expansion's `BreakStringAutomatic` never re-wraps them.
* **PokéNav / match call** (`pokenavcall`, `src/match_call.c`,
  `src/pokenav_match_call_data.c`): the window is 28 tiles with the printer at
  x = 32, so the limit is **192 px** (184 px before `\p` or `\l`).
* **unclassified** (only referenced from C tables, menus, `bufferstring`, or
  unreferenced): the limit is max(widest vanilla line of that label, 216 px),
  with no hard box rule.

The usage class of every label comes from scanning all `data/**/*.inc` and
`data/*.s` command lines. The macros in `asm/macros/*.inc` are expanded
symbolically, so `giveitem_msg`, `trainerbattle_*` argument positions,
`ingame_trade`, `move_tutor` and the like are understood. `src/**/*.c` is
scanned too (`ShowFieldMessage(...)` plus a few verified files: `tv.c`,
`battle_pyramid.c`, `birch_pc.c`, apprentice and exchange corner texts as
field; match call files as pokenav). On vanilla this gives 8,928 field, 1,080
battle and 322 PokéNav labels, 355 unclassified and 874 unreferenced (mostly
FRLG texts and token-pasted apprentice texts).

Placeholders: `{PLAYER}` is 42 px (7 characters × 6 px, the widest naming-screen
glyph). `{RIVAL}` is the widest of the names in `src/strings.c` (BRENDAN is
42 px). `{STR_VAR_n}` is 60 px (10 × 6 px), lowered per label when a vanilla
line containing it could not fit otherwise; 144 labels get a lower value this
way. Fixed placeholders (`{KYOGRE}`, `{REGION}` and so on) are measured from
`src/strings.c`. Other tokens are 0 px.

### Calibration on vanilla (`--calibrate`)

| class | lines | max px | max without placeholders | max before `\p`/`\l` (no placeholders) | box-rule violations |
|---|---|---|---|---|---|
| field | 32,019 | 216 | 210 | **208** (8,951 lines) | 0 |
| battle | 1,757 | 206 | 206 | 192 | 0 |
| pokenav | 2,383 | 192 | 184 | **184** | 0 |

* Without placeholders, field lines before `\p`/`\l` reach exactly 216 − 8 px
  and PokéNav lines reach exactly 192 − 8 px. This confirms both the window
  widths and the arrow rule.
* With `{PLAYER}` at 42 px, none of the 1,199 vanilla lines that contain it is
  over its limit. At 54 px, 7 would be over.
* With an uncalibrated `{STR_VAR}` of 60 px, 141 vanilla lines would be over
  their limit, which is why the per-label calibration exists.
* Vanilla never breaks the box rule in field, battle or PokéNav texts, so it
  is enforced as an error.
* Longest vanilla texts: 823 bytes in the field box (limit 1000) and 357 bytes
  in a battle text (Juan; limit 423).
* The full vanilla tree (pex against pex-orig) gives **0 errors and 0
  warnings**.

## measure.py

```sh
python3 -I tools/textcheck/measure.py 'Ciao, {PLAYER}! Come va?\nTutto bene.$'
python3 -I tools/textcheck/measure.py --class battle 'Non ci credo!\nHo perso!$'
grep -A6 'Route104_Text_Foo:' data/maps/Route104/scripts.inc | python3 -I tools/textcheck/measure.py
echo 'Testo lungo in prosa...' | python3 -I tools/textcheck/measure.py --wrap   # prints ready .string lines
```

The output gives, for each displayed line: width, limit, free pixels, and
flags (`OVER`, `ARROW`, `3RD-LINE`, `L-FIRST`). `--wrap` word-wraps prose into
`.string` lines following the box rules: the first line ends with `\n`,
further lines with `\l`, paragraphs with `\p`, and the text with `$`.
`--style box` starts a new box every 2 lines instead of scrolling.

## selftest.py

```sh
python3 -I tools/textcheck/selftest.py --report
```

This copies four vanilla files to `/home/user/work/textcheck-test/` and plants
21 mistakes (23 expected findings): an overlong field line, a battle line, a PokéNav line, a missing
`$`, a `$` mid-block, text after `$`, `—«»`, a raw `"`, `{PLAYR}`,
`{PLAYER }`, a decomposed è, a changed command, an injected `.string`, a
deleted block, 3 rows in a box, `\l` as the first break, the arrow rule, an
added `{STR_VAR_2}`, `‘` used as an apostrophe, and Juan's defeat text grown
past 423 bytes. It then checks that every mistake is reported.
