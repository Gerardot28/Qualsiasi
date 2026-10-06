#!/usr/bin/env python3
"""Build the Pokemon Multiverse trainer front-pic set from the selection below.

  build_set.py SRC_DIR OUT_DIR

For each roster character the first spec is the primary pick, written to
OUT_DIR/<region>/<key>.png; the others go to OUT_DIR/_alternates/<key>__*.png
so a different look can be swapped in without re-running the survey.
Also writes OUT_DIR/manifest.csv, OUT_DIR/manifest.json, OUT_DIR/CREDITS.md
and contact sheets OUT_DIR/contact_<region>.png (+ contact_alternates_*.png).

Spec syntax: 'SRC:stem' or 'SRC:stem@path-substring' (SRC labels as in
survey.py SOURCES). Characters already in the expansion use 'EXP:' and are
NOT copied (the build only renders them for the review sheet) because the
pic is already in graphics/trainers/front_pics with a TRAINER_PIC_* constant.
"""
import csv
import json
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import convert as C  # noqa: E402
import roster  # noqa: E402
import survey as S  # noqa: E402
from sources import palette_for  # noqa: E402

E = 'EXP:'
EXP_CONSTANTS = '/home/user/pex-orig/include/constants/trainers.h'
SEL = {
    # ------------------------------------------------------------ Kanto
    'brock': [E + 'leader_brock_frlg', 'HNS:brock'],
    'misty': [E + 'leader_misty_frlg', 'HNS:misty'],
    'lt_surge': [E + 'leader_lt_surge_frlg', 'HNS:surge'],
    'erika': [E + 'leader_erika_frlg', 'HNS:erika'],
    'koga': [E + 'leader_koga_frlg', 'HNS:elite_four_koga'],
    'sabrina': [E + 'leader_sabrina_frlg', 'HNS:sabrina'],
    'blaine': [E + 'leader_blaine_frlg', 'HNS:blaine'],
    'giovanni': [E + 'leader_giovanni_frlg', 'HNS:giovanni', 'TAAR:leader_giovanni'],
    'janine': ['HNS:janine', 'TAAR:janine', 'SMOGON:janine@heartgold'],
    'blue': [E + 'champion_rival_frlg', 'HNS:leader_blue', 'SMOGON:blue@heartgold'],
    'lorelei': [E + 'elite_four_lorelei_frlg'],
    'bruno': [E + 'elite_four_bruno_frlg', 'HNS:elite_four_bruno', 'SMOGON:bruno@heartgold'],
    'agatha': [E + 'elite_four_agatha_frlg'],
    'lance': [E + 'elite_four_lance_frlg', 'HNS:champion_lance', 'TAAR:lance_lets_go', 'SMOGON:lance@heartgold'],
    'red': [E + 'red', 'TAAR:red_ds', 'ROGUE:red@johto', 'SMOGON:red@heartgold'],
    'oak': [E + 'professor_oak_frlg', 'SHOWDOWN:oak'],
    # ------------------------------------------------------------ Johto
    'falkner': ['TAAR:falkner', 'ROGUE:falkner', 'HNS:leader_falkner', 'SMOGON:falkner@heartgold'],
    'bugsy': ['TAAR:bugsy@Black', 'ROGUE:bugsy', 'HNS:leader_bugsy', 'SMOGON:bugsy@heartgold'],
    'whitney': ['TAAR:whitney', 'ROGUE:whitney', 'HNS:leader_whitney', 'SMOGON:whitney@heartgold'],
    'morty': ['TAAR:morty', 'ROGUE:morty', 'HNS:leader_morty'],
    'chuck': ['TAAR:chuck', 'ROGUE:chuck', 'HNS:leader_chuck'],
    'jasmine': ['TAAR:jasmine@Black', 'TAAR:jasmine@Kasen', 'ROGUE:jasmine', 'HNS:leader_jasmine'],
    'pryce': ['TAAR:pryce', 'ROGUE:pryce', 'HNS:leader_pryce'],
    'clair': ['TAAR:clair', 'ROGUE:clair', 'HNS:leader_clair'],
    'will': ['TAAR:will', 'ROGUE:will', 'HNS:elite_four_will'],
    'karen': ['TAAR:karen', 'ROGUE:karen', 'HNS:elite_four_karen'],
    'elm': ['SHOWDOWN:elm'],
    'silver': ['ROGUE:silver', 'SMOGON:silver@heartgold'],
    # ------------------------------------------------------------ Hoenn
    'roxanne': [E + 'leader_roxanne'],
    'brawly': [E + 'leader_brawly'],
    'wattson': [E + 'leader_wattson'],
    'flannery': [E + 'leader_flannery'],
    'norman': [E + 'leader_norman'],
    'winona': [E + 'leader_winona'],
    'tate_and_liza': [E + 'leader_tate_and_liza'],
    'juan': [E + 'leader_juan'],
    'sidney': [E + 'elite_four_sidney'],
    'phoebe': [E + 'elite_four_phoebe'],
    'glacia': [E + 'elite_four_glacia'],
    'drake': [E + 'elite_four_drake'],
    'wallace': [E + 'champion_wallace'],
    'steven': [E + 'steven', E + 'champion_steven_frlg'],
    'birch': ['EXPINTRO:birch', 'SHOWDOWN:birch'],
    'maxie': [E + 'magma_leader_maxie'],
    'archie': [E + 'aqua_leader_archie'],
    'wally': [E + 'wally'],
    # ------------------------------------------------------------ Sinnoh
    'roark': ['ROGUE:roark', 'SINREM:roark_dp', 'TAAR:roark', 'PT:leader_roark'],
    'gardenia': ['ROGUE:gardenia', 'SINREM:gardenia_dp', 'TAAR:gardenia', 'PT:leader_gardenia'],
    'maylene': ['SMOGON:maylene@diamond', 'ROGUE:maylene', 'SINREM:maylene_dp', 'PT:leader_maylene'],
    'crasher_wake': ['ROGUE:crasherwake', 'SINREM:crasher_wake_dp', 'TAAR:wake', 'PT:leader_wake'],
    'fantina': ['ROGUE:fantina', 'SINREM:fantina_dp', 'TAAR:fantina', 'PT:leader_fantina'],
    'byron': ['ROGUE:byron', 'SINREM:byron_dp', 'TAAR:byron', 'PT:leader_byron'],
    'candice': ['PT:leader_candice', 'ROGUE:candice', 'SINREM:candice_dp', 'TAAR:candice'],
    'volkner': ['ROGUE:volkner', 'PT:leader_volkner', 'SINREM:volkner_dp', 'TAAR:volkner@iriv24'],
    'aaron': ['PT:elite_four_aaron', 'ROGUE:aaron', 'SMOGON:aaron@diamond'],
    'bertha': ['PT:elite_four_bertha', 'ROGUE:bertha', 'SINREM:bertha_dp'],
    'flint': ['ROGUE:flint', 'PT:elite_four_flint'],
    'lucian': ['ROGUE:lucian', 'SINREM:lucian_dp', 'PT:elite_four_lucian'],
    'cynthia': ['ROGUE:cynthia', 'TAAR:cynthia', 'SINREM:cynthia_dp', 'PT:champion_cynthia'],
    'rowan': ['SHOWDOWN:rowan'],
    'cyrus': ['ROGUE:galactic_cyrus', 'TAAR:cyrus', 'SINREM:cyrus_dp', 'PT:cyrus'],
    'barry': ['PT:barry', 'ROGUE:barry', 'SINREM:barry_dp'],
    # ------------------------------------------------------------ Unova
    'cilan': ['SMOGON:cilan', 'ROGUE:cilan'],
    'chili': ['SMOGON:chili', 'ROGUE:chili'],
    'cress': ['SMOGON:cress', 'ROGUE:cress'],
    'lenora': ['ROGUE:lenora', 'SMOGON:lenora'],
    'burgh': ['SMOGON:burgh', 'ROGUE:burgh'],
    'elesa': ['SMOGON:elesa@black-white', 'ROGUE:elesa', 'SMOGON:elesa@black2'],
    'clay': ['TAAR:leader_clay', 'ROGUE:clay', 'SMOGON:clay'],
    'skyla': ['TAAR:skyla', 'ROGUE:skyla', 'SMOGON:skyla'],
    'brycen': ['ROGUE:brycen', 'SMOGON:brycen'],
    'drayden': ['TAAR:drayden', 'ROGUE:drayden', 'SMOGON:drayden'],
    'roxie': ['TAAR:leader_roxie', 'ROGUE:roxie', 'SMOGON:roxie'],
    'marlon': ['SMOGON:marlon', 'ROGUE:marlon'],
    'cheren': ['SMOGON:cheren@black2', 'ROGUE:cheren', 'SMOGON:cheren@black-white'],
    'iris': ['TAAR:iris_bw2', 'TAAR:iris', 'ROGUE:iris', 'SMOGON:iris@black-white'],
    'shauntal': ['SMOGON:shauntal', 'ROGUE:shauntal'],
    'grimsley': ['ROGUE:grimsley', 'SMOGON:grimsley@black-white'],
    'caitlin': ['ROGUE:caitlin', 'SMOGON:caitlin'],
    'marshal': ['ROGUE:marshal', 'SMOGON:marshal'],
    'alder': ['SMOGON:alder', 'ROGUE:alder'],
    'juniper': ['SMOGON:juniper'],
    'ghetsis': ['ROGUE:plasma_ghetsis', 'SMOGON:ghetsis@black-white'],
    'n': ['ROGUE:n', 'SMOGON:n@black-white/N.png'],
    'colress': ['SMOGON:colress@black2'],
    # ------------------------------------------------------------ Kalos
    'viola': ['ROGUE:viola', 'SMOGON:viola'],
    'grant': ['ROGUE:grant', 'SHOWDOWN:grant'],
    'korrina': ['ROGUE:korrina', 'TAAR:leader_korrina', 'SMOGON:korrina'],
    'ramos': ['ROGUE:ramos', 'SMOGON:ramos'],
    'clemont': ['ROGUE:clemont', 'SMOGON:clemont'],
    'valerie': ['ROGUE:valerie', 'SMOGON:valerie'],
    'olympia': ['ROGUE:olympia', 'SMOGON:olympia'],
    'wulfric': ['ROGUE:wulfric', 'SMOGON:wulfric'],
    'malva': ['ROGUE:malva', 'SMOGON:malva'],
    'siebold': ['ROGUE:siebold', 'SMOGON:siebold'],
    'wikstrom': ['ROGUE:wikstrom', 'SMOGON:wikstrom'],
    'drasna': ['ROGUE:drasna', 'SMOGON:drasna'],
    'diantha': ['ROGUE:diantha', 'SHOWDOWN:diantha'],
    'sycamore': ['SHOWDOWN:sycamore'],
    'lysandre': ['ROGUE:flare_lysandre', 'SHOWDOWN:lysandre'],
    # ------------------------------------------------------------ Alola
    'ilima': ['ROGUE:ilima', 'SMOGON:ilima'],
    'lana': ['ROGUE:lana', 'SMOGON:lana'],
    'kiawe': ['ROGUE:kiawe', 'SMOGON:kiawe'],
    'mallow': ['ROGUE:mallow', 'SMOGON:mallow'],
    'sophocles': ['ROGUE:sophocles', 'SMOGON:sophocles'],
    'mina': ['ROGUE:mina', 'TAAR:mina', 'SMOGON:mina'],
    'acerola': ['ROGUE:acerola', 'TAAR:acerola', 'SMOGON:acerola'],
    'hala': ['ROGUE:hala', 'SMOGON:hala'],
    'olivia': ['ROGUE:olivia', 'SMOGON:olivia'],
    'nanu': ['ROGUE:nanu', 'SMOGON:nanu'],
    'hapu': ['ROGUE:hapu', 'SMOGON:hapu'],
    'kahili': ['ROGUE:kahili', 'SMOGON:kahili'],
    'molayne': ['ROGUE:molayne', 'SMOGON:molayne'],
    'kukui': ['ROGUE:kukui', 'SMOGON:kukui@sun-moon/Kukui.png'],
    'hau': ['ROGUE:hau', 'SMOGON:hau@sun-moon/Hau.png'],
    'lusamine': ['SMOGON:lusamine@sun-moon/Lusamine.png', 'EE:aether_leader_lusamine'],
    'guzma': ['SMOGON:guzma'],
    'gladion': ['ROGUE:gladion', 'SMOGON:gladion@sun-moon/Gladion.png'],
    # ------------------------------------------------------------ Galar
    'milo': ['ROGUE:milo', 'SHOWDOWN:milo'],
    'nessa': ['ROGUE:nessa', 'SHOWDOWN:nessa'],
    'kabu': ['ROGUE:kabu', 'SHOWDOWN:kabu'],
    'bea': ['ROGUE:bea', 'SHOWDOWN:bea'],
    'allister': ['ROGUE:allister', 'SHOWDOWN:allister'],
    'opal': ['ROGUE:opal', 'SHOWDOWN:opal'],
    'gordie': ['ROGUE:gordie', 'SHOWDOWN:gordie'],
    'melony': ['ROGUE:melony', 'SHOWDOWN:melony'],
    'piers': ['ROGUE:piers', 'SHOWDOWN:piers'],
    'raihan': ['ROGUE:raihan', 'TAAR:raihan', 'SHOWDOWN:raihan'],
    'bede': ['ROGUE:bede', 'SHOWDOWN:bede_leader'],
    'marnie': ['ROGUE:marnie', 'SHOWDOWN:marnie'],
    'leon': ['ROGUE:leon', 'SHOWDOWN:leon'],
    'hop': ['ROGUE:hop', 'SHOWDOWN:hop'],
    'magnolia': ['SHOWDOWN:magnolia'],
    'sonia': ['SHOWDOWN:sonia_professor', 'SHOWDOWN:sonia'],
    'rose': ['SHOWDOWN:rose'],
    # ------------------------------------------------------------ Paldea
    'katy': ['ROGUE:katy', 'SHOWDOWN:katy'],
    'brassius': ['ROGUE:brassius', 'SHOWDOWN:brassius'],
    'iono': ['ROGUE:iono', 'SHOWDOWN:iono'],
    'kofu': ['ROGUE:kofu', 'SHOWDOWN:kofu'],
    'ryme': ['ROGUE:ryme', 'SHOWDOWN:ryme'],
    'tulip': ['ROGUE:tulip', 'SHOWDOWN:tulip'],
    'grusha': ['ROGUE:grusha', 'SHOWDOWN:grusha'],
    'larry': ['ROGUE:larry', 'SHOWDOWN:larry'],
    'rika': ['ROGUE:rika', 'SHOWDOWN:rika'],
    'poppy': ['ROGUE:poppy', 'SHOWDOWN:poppy'],
    'hassel': ['ROGUE:hassel', 'SHOWDOWN:hassel'],
    'geeta': ['ROGUE:geeta', 'SHOWDOWN:geeta'],
    'nemona': ['ROGUE:nemona', 'SHOWDOWN:nemona_v'],
    'sada': ['SHOWDOWN:sada'],
    'turo': ['SHOWDOWN:turo'],
    'penny': ['ROGUE:penny', 'SHOWDOWN:penny'],
    'arven': ['ROGUE:arven', 'SHOWDOWN:arven_s'],
    'clavell': ['SHOWDOWN:clavell_s'],
}

# per-spec conversion overrides, e.g. {'PT:leader_volkner': {'fit': 'crop'}}
OVERRIDES = {}

REPO_URL = {
    'EXP': 'rh-hideout/pokeemerald-expansion 1.17.1 (local /home/user/pex-orig)',
    'EXPINTRO': 'rh-hideout/pokeemerald-expansion 1.17.1 (local /home/user/pex-orig)',
    'TAAR': 'https://github.com/TeamAquasHideout/Team-Aquas-Asset-Repo',
    'ROGUE': 'https://github.com/Pokabbie/pokeemerald-rogue (branch vanilla)',
    'HNS': 'https://github.com/PokemonHnS-Development/pokemonHnS',
    'SINREM': 'https://github.com/sinnoh-remakes/pokeemerald-platinum',
    'PT': 'https://github.com/pret/pokeplatinum',
    'SMOGON': 'https://github.com/smogon/sprites',
    'SHOWDOWN': 'https://github.com/DrSeil/Pokefirered_modified (copy of the Pokemon Showdown trainer sprite set)',
    'EE': 'https://github.com/Enhanced-Projects/Emerald-Enhanced',
}

TAAR_CREDIT = {
    'Black Fragrant': 'Black Fragrant (Pokemon FireGold)',
    'Galaxeeh': 'Galaxeeh (Bugsy base: Eren Jaeger)',
    'Kasen': 'Kasen',
    'iriv24': 'iriv24',
    'PurrfectDoodle': 'PurrfectDoodle',
    'RafaelSanna': 'RafaelSanna (commissioned by eatthepear)',
    'yoshord': 'yoshord',
    'ShinyDragonHunter': 'ShinyDragonHunter / Josh',
    'Pawkkie': 'Pawkkie',
    'BrandonXL': 'BrandonXL',
}


def credit_and_licence(f):
    src = f['src']
    p = f['path']
    if src in ('EXP', 'EXPINTRO'):
        return ('Game Freak (official GBA art)', 'official game asset, already shipped by the expansion')
    if src == 'TAAR':
        author = p.split('Trainer Front Sprites/')[1].split('/')[0]
        return (TAAR_CREDIT.get(author, author),
                "TAAR README: free to use and edit, credit the original creator")
    if src == 'ROGUE':
        return ('Pokemon Emerald Rogue (Pokabbie) - in-game "Additional Sprites" credits roll (src/data/credits.h); '
                'per-sprite artist not recorded',
                'no LICENSE file; community hack assets, credit required (not an explicit grant)')
    if src == 'HNS':
        return ('Pokemon Heart & Soul team; sprite artists per README: Cesare_CBass, AveonTrainer, PurpleZaffre, BatimaTheBat',
                'README: "completely open source ... feel free to take advantage"; no LICENSE file')
    if src == 'SINREM':
        return ('sinnoh-remakes/pokeemerald-platinum', 'no LICENSE, no per-sprite credit (unclear provenance)')
    if src == 'PT':
        return ('Game Freak (official Pokemon Platinum sprite, frame 0)', 'official game asset (same status as the base ROM art)')
    if src == 'SMOGON':
        if '/canonical/' in p:
            return ('Game Freak (official DS sprite) via smogon/sprites', 'official game asset')
        return ('fan-made DS-style sprite from the Pokemon Showdown set (smogon/sprites "noncanonical"); artist not recorded in repo',
                'no licence in repo; credit Pokemon Showdown / original artist (see play.pokemonshowdown.com sprite credits)')
    if src == 'SHOWDOWN':
        return ('Pokemon Showdown trainer sprite (mostly fan-made DS-style for Gen 6-9: Beliot419, Brumirage, Kyledove, '
                'and others); artist not recorded in the GitHub copy',
                'no licence; verify artist + permission before release')
    if src == 'EE':
        return ('Emerald Enhanced (Enhanced Projects)', 'RESTRICTED: README requires permission from Enhanced Projects')
    return ('?', '?')


def resolve(spec, files):
    src, rest = spec.split(':', 1)
    stem, _, hint = rest.partition('@')
    stem = re.sub(r'[^a-z0-9]+', '_', stem.lower()).strip('_')
    hits = [f for f in files if f['src'] == src and f['stem'] == stem and (not hint or hint in f['path'])]
    if not hits:
        raise KeyError('no file for spec %s' % spec)
    return hits[0]


def build_one(f, out, spec):
    ov = OVERRIDES.get(spec, {})
    pal = palette_for(f['repo'], f['path']) if f['kind'] == 'gba' else None
    frame = (0, 0, 80, 80) if f['kind'] == 'ds-sheet' else None
    info = C.convert(f['path'], out, pal=pal, frame=frame, **ov)
    info['pal'] = pal
    return info


def write_credits(rows, path):
    """Attribution list for the pics that are NOT already in the expansion."""
    groups = {}
    for r in rows:
        if r['source'] == 'EXP':
            continue
        groups.setdefault((r['source'], r['credit'], r['licence']), []).append(
            '%s%s' % (r['name'], '' if r['pick'] == 'primary' else ' (alt)'))
    with open(path, 'w') as fh:
        fh.write('# Trainer front pics - credits and licence notes\n\n')
        fh.write('Generated by tools/trainer_sprites/build_set.py. Pics marked (alt) are alternates.\n')
        fh.write('Official Game Freak art keeps the same status as the base-ROM assets.\n\n')
        for (src, credit, lic), names in sorted(groups.items()):
            fh.write('## %s - %s\n\n' % (src, REPO_URL.get(src, '')))
            fh.write('- Credit: %s\n- Licence: %s\n- Used for: %s\n\n' % (credit, lic, ', '.join(names)))


def main():
    src_dir, out_dir = sys.argv[1], sys.argv[2]
    files = S.gather(src_dir)
    names = {k: (reg, role, name) for k, reg, role, name, _ in roster.R}
    missing = [k for k in names if k not in SEL]
    if missing:
        print('WARNING: roster keys without selection:', missing)
    if os.path.isdir(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)
    rows = []
    sheets = {}
    alt_paths = []
    for key, (region, role, name) in names.items():
        for i, spec in enumerate(SEL.get(key, [])):
            f = resolve(spec, files)
            primary = i == 0
            if primary:
                out = os.path.join(out_dir, region, key + '.png')
                if f['src'] == 'EXP':  # already in the expansion: review copy only
                    out = os.path.join(out_dir, '_existing_review', key + '.png')
            else:
                out = os.path.join(out_dir, '_alternates', '%s__%s_%s.png' % (key, f['src'].lower(), f['stem']))
            info = build_one(f, out, spec)
            credit, lic = credit_and_licence(f)
            row = {
                'key': key, 'name': name, 'region': region, 'role': role,
                'pick': 'primary' if primary else 'alternate',
                'status': ('existing' if f['src'] == 'EXP' else
                           'existing-asset' if f['src'] == 'EXPINTRO' else
                           'gba-ready' if f['kind'] == 'gba' else
                           'converted-scaled' if info['method'] == 'scale' else 'converted-lossless'),
                'trainer_pic': ('TRAINER_PIC_' + f['stem'].upper()) if f['src'] == 'EXP' else '',
                'out': os.path.relpath(out, out_dir), 'source': f['src'], 'source_repo': REPO_URL.get(f['src'], ''),
                'source_path': os.path.relpath(f['path'], f['repo']), 'palette_file': os.path.relpath(info['pal'], f['repo']) if info['pal'] else '',
                'method': info['method'], 'factor': info.get('factor', ''), 'trimmed': json.dumps(info.get('trimmed', '')) if info.get('trimmed') else '',
                'colours': info['colours'], 'colour_merges': len(info['merges']),
                'issues': '; '.join(info['issues']), 'credit': credit, 'licence': lic,
            }
            rows.append(row)
            if primary:
                sheets.setdefault(region, []).append((out, '%s [%s%s]' % (name, f['src'], ' x%.2f' % info['factor'] if info.get('factor') else '')))
            else:
                alt_paths.append((out, '%s [%s:%s]' % (name, f['src'], f['stem'][:12])))
    # EXP picks: keep the constant only if the expansion really defines it
    consts = set(re.findall(r'\b(TRAINER_PIC_\w+)', open(EXP_CONSTANTS).read()))
    for r in rows:
        if r['trainer_pic'] and r['trainer_pic'] not in consts:
            r['trainer_pic'] = '(none: PNG present but not wired to a TRAINER_PIC_*)'
    with open(os.path.join(out_dir, 'manifest.csv'), 'w', newline='') as fh:
        w = csv.DictWriter(fh, fieldnames=list(rows[0].keys()))
        w.writeheader()
        w.writerows(rows)
    with open(os.path.join(out_dir, 'manifest.json'), 'w') as fh:
        json.dump(rows, fh, indent=1)
    write_credits(rows, os.path.join(out_dir, 'CREDITS.md'))
    with open(os.path.join(out_dir, 'SOURCES.txt'), 'w') as fh:  # pin the exact commits used
        for label, repo, _, _ in S.SOURCES:
            root = repo if os.path.isabs(repo) else os.path.join(src_dir, repo)
            try:
                sha = subprocess.run(['git', '-C', root, 'rev-parse', 'HEAD'], capture_output=True,
                                     text=True, check=True).stdout.strip()
            except Exception:
                sha = '(not a git checkout)'
            fh.write('%-9s %s  %s\n' % (label, sha, REPO_URL.get(label, root)))
    for region, items in sheets.items():
        C.sheet([p for p, _ in items], os.path.join(out_dir, 'contact_%s.png' % region), zoom=4, cols=6,
                labels=[l for _, l in items], bg=(115, 197, 164))
    allp = [p for r in roster.REGIONS for p, _ in sheets.get(r, [])]
    alll = [l for r in roster.REGIONS for _, l in sheets.get(r, [])]
    C.sheet(allp, os.path.join(out_dir, 'contact_all.png'), zoom=4, cols=12, labels=alll, bg=(115, 197, 164))
    for n in range(0, len(alt_paths), 48):
        chunk = alt_paths[n:n + 48]
        C.sheet([p for p, _ in chunk], os.path.join(out_dir, 'contact_alternates_%d.png' % (n // 48 + 1)), zoom=3, cols=8,
                labels=[l for _, l in chunk], bg=(115, 197, 164))
    bad = [r for r in rows if r['issues']]
    print('built %d pics (%d primary), %d with issues' % (len(rows), sum(r['pick'] == 'primary' for r in rows), len(bad)))
    for r in bad:
        print('  ISSUE', r['out'], r['issues'])


if __name__ == '__main__':
    main()
