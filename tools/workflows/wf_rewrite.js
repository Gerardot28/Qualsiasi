export const meta = {
  name: 'multiverse-text-rewrite',
  description: 'Rewrite a slice of in-game text units in Italian following the Pokémon Multiverse story bible',
  phases: [{ title: 'Rewrite', detail: 'one writer per unit, self-validated with textcheck' }],
}

const RULES = `
You are a professional Italian game writer working on "Pokémon Multiverse", an Italian ROM hack of Pokémon Emerald (pokeemerald-expansion 1.17.1). You rewrite in-game text.
READ FIRST (fully): /home/user/Qualsiasi/docs/storia/bibbia.md (story bible: plot, cast, style guide, glossary) and /home/user/Qualsiasi/docs/storia/nomi.json (official names). The scripted-event skeleton for context is in /home/user/Qualsiasi/docs/storia/scheletro/ (segmento_A..D.md — consult the parts relevant to your maps).
TASK: rewrite EVERY \`.string "..."\` line of your assigned files into natural, high-quality ITALIAN:
- Story scenes must tell the NEW original plot of the bible (new villains replacing Team Magma/Aqua, new rival, multiverse/rift lore, red-haired male protagonist, Norman is his father, new starters Turtwig/Fuecoco/Froakie). Canonical Hoenn places/leaders keep their official Italian names (see nomi.json). Never mention Team Magma/Aqua, Maxie/Archie, May/Brendan, Treecko/Torchic/Mudkip unless the bible says so.
- Generic NPCs, signs, trainers, shops, systems: adapt (not literal translation) into lively Italian with personality and occasional light references to the rifts/multiverse lore; keep the game-mechanical information exactly correct (what an item does, where to go, prices, numbers, menu instructions).
- Trainer intro/defeat/post-battle lines: short, punchy, varied, themed to the trainer class.
- Use official Italian terminology and item/move names from the bible's glossary; Title Case for proper names ("Pokémon", "Poké Ball", "Pokédex", "Centro Pokémon", "Prof. Birch", "Percorso 101"); no ALL CAPS except shouting; keep a "Nome: " speaker prefix only where the original line had one.
HARD TECHNICAL RULES (the ROM breaks otherwise):
1. Change ONLY the text inside .string "..." lines. You may add or remove .string lines INSIDE a text block (the consecutive .string lines under one label) to fit the text. Never touch labels, commands, comments, directives (.braille etc.), #if/.if lines or any other line.
2. A block ends with "$" exactly when the original block did; no "$" anywhere else.
3. Keep every control code/placeholder the original uses: {PLAYER} {RIVAL} {STR_VAR_1..3} {COLOR ...} {PAUSE ...} {PLAY_SE ...} {PLAY_BGM ...} {FONT_...} {CLEAR_TO ...} {B_...} etc. Never add {STR_VAR_n} that the original block did not use.
4. Message boxes show 2 lines: in each paragraph the first line break is \\n, further breaks are \\l (scroll); \\p starts a new box. Max width: field box 208 px for a line followed by \\p or \\l (216 px for the last line), battle box (trainer defeat/victory texts) 200/208 px, PokéNav 184/192 px; texts used by menus/C code (unclassified) must stay no wider than the original. Measure with: python3 -I /home/user/Qualsiasi/tools/textcheck/measure.py "your line" (or pipe lines).
5. Characters: apostrophe ' ; double quotes “ ” only; no « » – — ° € # * @ [ ] ; accents precomposed (à è é ì ò ù). "..." or … both fine.
6. VALIDATE until 0 errors (warnings about tokens: double-check them):
   - files in /home/user/pex: python3 -I /home/user/Qualsiasi/tools/textcheck/textcheck.py <path relative to /home/user/pex>
   - part files: python3 -I /home/user/Qualsiasi/tools/textcheck/textcheck.py --single <edit file> <orig file>
   Also make sure no English sentence is left (proper nouns aside).
Do NOT run make. Do NOT edit any file other than your assigned ones.
FINAL ANSWER: one short paragraph: files done, number of strings, validator result (0 errors), notable choices.
`

const units = args.units
phase('Rewrite')
const results = await pipeline(units, u => {
  const list = u.p.map(x => {
    const [file, part] = x.split('|')
    const edit = part ? `/home/user/work/rewrite/parts/new/${part}` : `/home/user/pex/${file}`
    const orig = part ? `/home/user/work/rewrite/parts/orig/${part}` : `/home/user/pex-orig/${file}`
    return `- EDIT: ${edit}\n  ORIG (read-only reference): ${orig}\n  (game file: ${file}${part ? ', this is one PART of it — keep its boundaries' : ''})`
  }).join('\n')
  return agent(`${RULES}\nYOUR UNIT ${u.id} (${u.lines} .string lines):\n${list}`, { label: `rewrite:${u.id}`, phase: 'Rewrite' })
})
return units.map((u, i) => ({ id: u.id, report: results[i] }))
