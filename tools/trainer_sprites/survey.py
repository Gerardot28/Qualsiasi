#!/usr/bin/env python3
"""Survey every source repository for candidate front pics of the roster and
render side-by-side comparison sheets (each candidate already converted to the
final 64x64/16-colour format, so DS-sized sources show the auto-downscale).

  survey.py SRC_DIR OUT_DIR [--regions kanto,johto]

SRC_DIR is the directory holding the (sparse) clones, see README.md.
Writes OUT_DIR/candidates.json and OUT_DIR/<region>_<n>.png.
"""
import argparse
import glob
import json
import os
import re
import sys
import tempfile

from PIL import Image, ImageDraw

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import convert as C  # noqa: E402
import roster  # noqa: E402
from sources import palette_for  # noqa: E402

# label, repo dir (relative to SRC_DIR), glob, kind
SOURCES = [
    ('EXP', '/home/user/pex-orig', 'graphics/trainers/front_pics/*.png', 'gba'),
    ('EXPINTRO', '/home/user/pex-orig', 'graphics/birch_speech/birch.png', 'gba'),
    ('TAAR', 'TAAR', 'Trainer Front Sprites/**/*.[pP][nN][gG]', 'gba'),
    ('ROGUE', 'Pokabbie_pokeemerald-rogue', 'graphics/trainers/front_pics/**/*.png', 'gba'),
    ('HNS', 'PokemonHnS-Development_pokemonHnS', 'graphics/trainers/front_pics/**/*.png', 'gba'),
    ('CDUST', 'Sierraffinity_CrystalDust', 'graphics/trainers/front_pics/**/*.png', 'gba'),
    ('ROWE', 'BelialClover_RoweRepo', 'graphics/trainers/front_pics/*.png', 'gba'),
    ('PKW', 'evilchinesefood_PKMN-World', 'graphics/trainers/front_pics/*.png', 'gba'),
    ('OMNIS', 'StrangeQuark_pokeomnis', 'graphics/trainers/front_pics/*.png', 'gba'),
    ('SINREM', 'sinnoh-remakes_pokeemerald-platinum', 'graphics/trainers/front_pics/*.png', 'gba'),
    ('YAEEH', 'PCG06_pokeyaeeh', 'graphics/trainers/front_pics/**/*.png', 'gba'),
    ('TARC2', 'harmakanna_TARC2', 'graphics/trainers/front_pics/**/*.png', 'gba'),
    ('EE', 'Enhanced-Projects_Emerald-Enhanced', 'graphics/trainers/front_pics/*.png', 'gba'),
    ('PT', 'pokeplatinum', 'res/trainers/classes/*/front.png', 'ds-sheet'),
    ('SMOGON', 'smogon_sprites', 'src/_uncategorized/*/trainers/**/*.png', 'ds'),
    ('SHOWDOWN', 'DrSeil_Pokefirered_modified', 'graphics/trainers/front_pics/80/*.png', 'ds'),
]
SKIP = re.compile(r'(masters|anime|lgpe|_gen1|_gen2|_gen3|isekai|contest|pokeathlon|dueldisk|'
                  r'wonderlauncher|zerosuit|nihilego|_back|_ow|preview|overview|example|'
                  r'front_sprites_\d|assorted|adapted_trainer_fronts|_tower|_dojo|_league|stance|_z$)')
PT_ALIAS = {'rival': 'barry', 'galactic_boss': 'cyrus'}


def norm_stem(path, kind):
    if kind == 'ds-sheet':
        s = path.replace('\\', '/').split('/')[-2]
        s = PT_ALIAS.get(s, s)
    else:
        s = roster.stem(path)
    return re.sub(r'[^a-z0-9]+', '_', s.lower()).strip('_')


def gather(src_dir):
    out = []
    for label, repo, pat, kind in SOURCES:
        root = repo if os.path.isabs(repo) else os.path.join(src_dir, repo)
        for p in sorted(glob.glob(os.path.join(root, pat), recursive=True)):
            if p.lower().endswith('.pal'):
                continue
            st = norm_stem(p, kind)
            if SKIP.search(st) or re.search(r'/gen[123]/', p):
                continue
            out.append({'src': label, 'repo': root, 'path': p, 'stem': st, 'kind': kind})
    return out


def match(files):
    res = {}
    for key, region, role, name, rx in roster.R:
        res[key] = [f for f in files if rx.search(f['stem'])]
    return res


def render_candidate(f, tmpdir):
    pal = palette_for(f['repo'], f['path']) if f['kind'] == 'gba' else None
    frame = (0, 0, 80, 80) if f['kind'] == 'ds-sheet' else None
    out = os.path.join(tmpdir, '%s_%s.png' % (f['src'], re.sub(r'\W', '_', os.path.relpath(f['path'], f['repo']))))
    try:
        im = Image.open(f['path'])
        info = {}
        if f['kind'] == 'gba' and im.size == (64, 64):
            info = C.convert(f['path'], out, pal=pal, scale=None)
        else:
            info = C.convert(f['path'], out, pal=pal, frame=frame)
        f['pal'] = pal
        f['method'] = info.get('method')
        f['factor'] = info.get('factor')
        return out
    except Exception as e:
        f['error'] = str(e)
        return None


def region_sheets(cands, out_dir, regions, per_sheet=8, zoom=2):
    os.makedirs(out_dir, exist_ok=True)
    tmp = tempfile.mkdtemp(prefix='survey_')
    cell = 64 * zoom + 6
    for region in regions:
        keys = [r for r in roster.R if r[1] == region]
        for part in range(0, len(keys), per_sheet):
            chunk = keys[part:part + per_sheet]
            rows = []
            for key, _, role, name, _ in chunk:
                imgs = []
                for f in cands[key]:
                    p = render_candidate(f, tmp)
                    if p:
                        imgs.append((p, '%s:%s' % (f['src'], f['stem'][:14])))
                rows.append((name, imgs))
            ncol = max([len(r[1]) for r in rows] + [1])
            W = 90 + ncol * cell
            H = len(rows) * (cell + 14)
            s = Image.new('RGB', (W, H), (255, 255, 255))
            d = ImageDraw.Draw(s)
            for ri, (name, imgs) in enumerate(rows):
                y = ri * (cell + 14)
                d.text((4, y + cell // 2), name, fill=(0, 0, 0))
                for ci, (p, lab) in enumerate(imgs):
                    x = 90 + ci * cell
                    im = C.render(p, (115, 197, 164)).resize((64 * zoom, 64 * zoom), Image.NEAREST)
                    s.paste(im, (x, y))
                    d.text((x, y + 64 * zoom + 1), lab, fill=(0, 0, 0))
                if not imgs:
                    d.text((90, y + cell // 2), 'NO CANDIDATE', fill=(200, 0, 0))
            s.save(os.path.join(out_dir, '%s_%d.png' % (region, part // per_sheet + 1)))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('src_dir')
    ap.add_argument('out_dir')
    ap.add_argument('--regions', default=','.join(roster.REGIONS))
    a = ap.parse_args()
    files = gather(a.src_dir)
    cands = match(files)
    region_sheets(cands, a.out_dir, a.regions.split(','))
    with open(os.path.join(a.out_dir, 'candidates.json'), 'w') as fh:
        json.dump({k: [{kk: vv for kk, vv in f.items() if kk != 'repo'} for f in v]
                   for k, v in cands.items()}, fh, indent=1)
    for key, region, role, name, _ in roster.R:
        print('%-8s %-16s %2d  %s' % (region, name, len(cands[key]),
                                     ' '.join(sorted({f['src'] for f in cands[key]}))))


if __name__ == '__main__':
    main()
