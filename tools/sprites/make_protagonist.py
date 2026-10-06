#!/usr/bin/env python3
"""Build the new male protagonist (spiky red hair, black streetwear) for pokeemerald-expansion.

All pixel art lives as hand-authored text grids in data/ (see README.md for the format).
This script renders those grids to the indexed 4bpp PNGs / JASC palettes the build expects,
writes them into a pokeemerald-expansion tree and (optionally) applies the small source
patches that give the new character dedicated palettes where Brendan used to share one.

Usage:
    python3 make_protagonist.py --tree /path/to/pokeemerald-expansion [--previews OUTDIR]
    python3 make_protagonist.py --previews OUTDIR            # previews only
    python3 make_protagonist.py --tree T --no-code           # graphics only, no C changes
    python3 make_protagonist.py --check                      # validate data files only
"""
import argparse
import os
import re
import sys

from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(HERE, 'data')
KEEP = ' ?'          # stamp chars that leave the underlying pixel unchanged
CLEAR = '_'          # stamp char that forces transparency
TRANSPARENT = '.'


# --------------------------------------------------------------------------- data loading

def load_palettes(path=os.path.join(DATA, 'palettes.txt')):
    pals, cur = {}, None
    for raw in open(path, encoding='utf-8'):
        line = raw.split('#', 1)[0].rstrip()
        if not line.strip():
            continue
        m = re.match(r'==\s*(\S+)', line)
        if m:
            cur = m.group(1)
            pals[cur] = []
            continue
        parts = line.split()
        letter, rgb = parts[0], tuple(int(v) for v in parts[1:4])
        pals[cur].append((letter, rgb))
    for name, entries in pals.items():
        if len(entries) != 16:
            raise SystemExit('palette %s has %d entries (need 16)' % (name, len(entries)))
        letters = [e[0] for e in entries]
        if len(set(letters)) != 16:
            raise SystemExit('palette %s has duplicate letters' % name)
        if letters[0] != TRANSPARENT:
            raise SystemExit('palette %s: slot 0 must be "."' % name)
    return pals


def parse_blocks(path):
    """Parse '== name' blocks. Lines starting with '@' are directives, '#' comments.
    Returns list of dicts {name, rows, directives}."""
    blocks, cur = [], None
    for lineno, raw in enumerate(open(path, encoding='utf-8'), 1):
        line = raw.rstrip('\n')
        if line.startswith('#'):
            continue
        m = re.match(r'==\s*(.*)$', line)
        if m:
            cur = {'name': m.group(1).strip(), 'rows': [], 'directives': [], 'file': path, 'line': lineno}
            blocks.append(cur)
            continue
        if cur is None or not line.strip():
            continue
        if line.startswith('@'):
            cur['directives'].append(line[1:].split())
            continue
        cur['rows'].append(line.split('#', 1)[0].rstrip() if ' #' in line else line.rstrip())
    return blocks


def load_stamps(path):
    stamps = {}
    for b in parse_blocks(path):
        w = max(len(r) for r in b['rows'])
        stamps[b['name']] = [r.ljust(w, ' ') for r in b['rows']]
    return stamps


def apply_stamp(grid, st, x0, y0, flip=False):
    out = [list(r) for r in grid]
    for y, row in enumerate(st):
        if flip:
            row = row[::-1]
        for x, c in enumerate(row):
            if c in KEEP:
                continue
            X, Y = x0 + x, y0 + y
            if 0 <= Y < len(out) and 0 <= X < len(out[0]):
                out[Y][X] = TRANSPARENT if c == CLEAR else c
    return [''.join(r) for r in out]


def build_frames(path, w, h, stamps=None):
    """Return list of (name, grid) for a sheet file. Supports directives:
       @stamp NAME X Y [flip]   -- composite a stamp from the stamp library
       @copy N                  -- start from frame N of this sheet
       @from FILE N             -- start from frame N of another sheet file (relative to data/)
       Grid rows given after a @copy/@from overlay the copied frame; '?' keeps the copied pixel.
       @flip                    -- mirror the final frame horizontally
       @shift DX DY             -- move the composed frame
       @put X Y SEGMENT         -- paint a row segment after stamping ('?' keep, '_' transparent)
    Directives are applied in file order after the grid.
    """
    frames = []
    for b in parse_blocks(path):
        rows = b['rows']
        base = None
        for d in b['directives']:
            if d[0] == 'copy':
                base = list(frames[int(d[1])][1])
            elif d[0] == 'from':      # @from other_sheet.txt N : start from a frame of another sheet
                other = build_frames(os.path.join(DATA, d[1]), w, h, stamps)
                base = list(other[int(d[2])][1])
        if base is None:
            base = [TRANSPARENT * w] * h
        if rows:
            if len(rows) != h:
                raise SystemExit('%s:%d frame "%s": %d rows (expected %d)' % (path, b['line'], b['name'], len(rows), h))
            for i, r in enumerate(rows):
                if len(r) != w:
                    raise SystemExit('%s:%d frame "%s" row %d has width %d (expected %d): %r'
                                     % (path, b['line'], b['name'], i, len(r), w, r))
            # rows overlay the copied base: '?' keeps the base pixel
            base = [''.join(bc if rc == '?' else rc for rc, bc in zip(r, br)) for r, br in zip(rows, base)]
        g = base
        for d in b['directives']:
            if d[0] == 'stamp':
                g = apply_stamp(g, stamps[d[1]], int(d[2]), int(d[3]), len(d) > 4 and d[4] == 'flip')
            elif d[0] == 'flip':
                g = [r[::-1] for r in g]
            elif d[0] == 'shift':
                dx, dy = int(d[1]), int(d[2])
                blank = TRANSPARENT * w
                g2 = [blank] * h
                for y in range(h):
                    if 0 <= y - dy < h:
                        src = g[y - dy]
                        g2[y] = ''.join(src[x - dx] if 0 <= x - dx < w else TRANSPARENT for x in range(w))
                g = g2
            elif d[0] == 'put':       # @put X Y SEGMENT : paint a row segment on top ('?' keep, '_' clear)
                x0, y0, seg = int(d[1]), int(d[2]), d[3]
                row = list(g[y0])
                for i, c in enumerate(seg):
                    if c in KEEP or not (0 <= x0 + i < w):
                        continue
                    row[x0 + i] = TRANSPARENT if c == CLEAR else c
                g = g[:y0] + [''.join(row)] + g[y0 + 1:]
        frames.append((b['name'], g))
    return frames


# --------------------------------------------------------------------------- rendering

def grid_to_image(grid, pal):
    lut = {letter: i for i, (letter, _) in enumerate(pal)}
    h, w = len(grid), len(grid[0])
    im = Image.new('P', (w, h), 0)
    px = im.load()
    for y, row in enumerate(grid):
        for x, c in enumerate(row):
            if c not in lut:
                raise ValueError('letter %r not in palette' % c)
            px[x, y] = lut[c]
    flat = []
    for _, rgb in pal:
        flat.extend(rgb)
    im.putpalette(flat)
    return im


def sheet_image(frames, pal, layout='h'):
    """Concatenate frames horizontally ('h') or vertically ('v')."""
    ims = [grid_to_image(g, pal) for _, g in frames]
    w, h = ims[0].size
    if layout == 'h':
        out = Image.new('P', (w * len(ims), h), 0)
        for i, im in enumerate(ims):
            out.paste(im, (i * w, 0))
    else:
        out = Image.new('P', (w, h * len(ims)), 0)
        for i, im in enumerate(ims):
            out.paste(im, (0, i * h))
    out.putpalette(ims[0].getpalette())
    return out


def save_png(im, path, transparency=False):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    kw = {'bits': 4, 'optimize': False}
    if transparency:
        kw['transparency'] = 0
    im.save(path, **kw)


def write_jasc(pal_rgb, path):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    lines = ['JASC-PAL', '0100', '16'] + ['%d %d %d' % rgb for rgb in pal_rgb]
    with open(path, 'w', newline='') as f:
        f.write('\r\n'.join(lines) + '\r\n')


def reflection_palette(pal_rgb):
    """Water reflection palette: same formula family as the vanilla brendan_reflection.pal
    (lightened and pulled toward a pale water tint)."""
    out = [pal_rgb[0]]
    for r, g, b in pal_rgb[1:]:
        out.append(tuple(min(255, int(round(c * 0.55 + t))) for c, t in zip((r, g, b), (115, 120, 125))))
    return out


def gba_round(rgb):
    """What the GBA will show (5-bit channels) -- used for previews only."""
    return tuple((c >> 3) << 3 for c in rgb)


# --------------------------------------------------------------------------- asset table

OW = 'graphics/object_events/pics/people/brendan/'
# (data file, frame w, h, palette, output png, layout)
SHEETS = [
    ('ow/walking.txt', 16, 32, 'ow', OW + 'walking.png', 'h'),
    ('ow/running.txt', 16, 32, 'ow', OW + 'running.png', 'h'),
    ('ow/mach_bike.txt', 32, 32, 'ow', OW + 'mach_bike.png', 'h'),
    ('ow/acro_bike.txt', 32, 32, 'ow', OW + 'acro_bike.png', 'h'),
    ('ow/surfing.txt', 32, 32, 'ow', OW + 'surfing.png', 'h'),
    ('ow/field_move.txt', 32, 32, 'ow', OW + 'field_move.png', 'h'),
    ('ow/fishing.txt', 32, 32, 'ow', OW + 'fishing.png', 'h'),
    ('ow/watering.txt', 32, 32, 'ow', OW + 'watering.png', 'h'),
    ('ow/decorating.txt', 16, 32, 'ow', OW + 'decorating.png', 'h'),
    ('ow/underwater.txt', 32, 32, 'ow_underwater', OW + 'underwater.png', 'h'),
    ('ow/dowsing.txt', 16, 32, 'ow', 'graphics/field_effects/pics/oras_dowsing_brendan.png', 'h'),
    ('trainer/front.txt', 64, 64, 'trainer', 'graphics/trainers/front_pics/brendan.png', 'v'),
    ('trainer/back.txt', 64, 64, 'trainer', 'graphics/trainers/back_pics/brendan.png', 'v'),
    ('intro/intro_bike.txt', 64, 64, 'intro', 'graphics/intro/scene_2/brendan.png', 'v'),
    ('intro/credits.txt', 64, 64, 'intro', 'graphics/intro/scene_2/brendan_credits.png', 'v'),
    ('icons/region_map.txt', 16, 16, 'ow', 'graphics/pokenav/region_map/brendan_icon.png', 'v'),
]

PALETTE_FILES = [
    ('ow', 'graphics/object_events/palettes/brendan.pal'),
    ('ow_underwater', 'graphics/object_events/palettes/brendan_underwater.pal'),
    ('trainer', 'graphics/trainers/palettes/brendan.pal'),
    ('intro', 'graphics/intro/scene_2/brendan.pal'),
]
TRANSPARENCY_CHUNK = {'graphics/trainers/front_pics/brendan.png', 'graphics/trainers/back_pics/brendan.png',
                      'graphics/pokenav/region_map/brendan_icon.png'}


def frontier_heads(pals, tree_src):
    """graphics/frontier_pass/map_heads.png: top 16x16 = male head (uses this PNG's palette),
    bottom 16x16 = female head (uses map_heads_female.pal) -- the female half is copied
    unchanged from the source tree; only the male half and the PNG palette are replaced."""
    path = os.path.join(DATA, 'icons/frontier_head.txt')
    frames = build_frames(path, 16, 16, load_stamps(os.path.join(DATA, 'ow/heads.txt')))
    male = grid_to_image(frames[0][1], pals['ow'])
    orig = Image.open(os.path.join(tree_src, 'graphics/frontier_pass/map_heads.png'))
    out = orig.copy()
    out.paste(male, (0, 0))
    flat = []
    for _, rgb in pals['ow']:
        flat.extend(rgb)
    out.putpalette(flat)
    return out


# --------------------------------------------------------------------------- source patches

CODE_PATCHES = [
    # 1) dedicated underwater palette for the male player
    ('include/constants/event_objects.h',
     '#define OBJ_EVENT_PAL_TAG_SS_ANNE                 0x1133\n',
     '#define OBJ_EVENT_PAL_TAG_SS_ANNE                 0x1133\n'
     '#define OBJ_EVENT_PAL_TAG_BRENDAN_UNDERWATER      0x1134 // new protagonist: dedicated underwater palette\n'),
    ('include/graphics.h',
     'extern const u16 gObjectEventPal_Brendan[];\n',
     'extern const u16 gObjectEventPal_Brendan[];\nextern const u16 gObjectEventPal_BrendanUnderwater[];\n'),
    ('src/data/object_events/object_event_graphics.h',
     'const u16 gObjectEventPal_Brendan[] = INCGFX_U16("graphics/object_events/palettes/brendan.pal", ".gbapal");\n',
     'const u16 gObjectEventPal_Brendan[] = INCGFX_U16("graphics/object_events/palettes/brendan.pal", ".gbapal");\n'
     'const u16 gObjectEventPal_BrendanUnderwater[] = INCGFX_U16("graphics/object_events/palettes/brendan_underwater.pal", ".gbapal");\n'),
    ('src/event_object_movement.c',
     '    {gObjectEventPal_PlayerUnderwater,      OBJ_EVENT_PAL_TAG_PLAYER_UNDERWATER},\n',
     '    {gObjectEventPal_PlayerUnderwater,      OBJ_EVENT_PAL_TAG_PLAYER_UNDERWATER},\n'
     '    {gObjectEventPal_BrendanUnderwater,     OBJ_EVENT_PAL_TAG_BRENDAN_UNDERWATER},\n'),
    ('src/event_object_movement.c',
     'static const struct PairedPalettes sPlayerReflectionPaletteSets[] = {\n',
     'static const u16 sReflectionPaletteTags_BrendanUnderwater[] = {\n'
     '    OBJ_EVENT_PAL_TAG_BRENDAN_UNDERWATER,\n'
     '    OBJ_EVENT_PAL_TAG_BRENDAN_UNDERWATER,\n'
     '    OBJ_EVENT_PAL_TAG_BRENDAN_UNDERWATER,\n'
     '    OBJ_EVENT_PAL_TAG_BRENDAN_UNDERWATER,\n'
     '};\n\n'
     'static const struct PairedPalettes sPlayerReflectionPaletteSets[] = {\n'
     '    {OBJ_EVENT_PAL_TAG_BRENDAN_UNDERWATER, sReflectionPaletteTags_BrendanUnderwater},\n'),
    ('src/data/object_events/object_event_graphics_info.h',
     'const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_BrendanUnderwater = {\n'
     '    .tileTag = TAG_NONE,\n'
     '    .paletteTag = OBJ_EVENT_PAL_TAG_PLAYER_UNDERWATER,\n',
     'const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_BrendanUnderwater = {\n'
     '    .tileTag = TAG_NONE,\n'
     '    .paletteTag = OBJ_EVENT_PAL_TAG_BRENDAN_UNDERWATER,\n'),
    # 2) Link Brendan (player in contests / link rooms) used May's palette with Brendan's pics;
    #    the new art needs its own palette.
    ('src/data/object_events/object_event_graphics_info.h',
     'const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_LinkBrendan = {\n'
     '    .tileTag = TAG_NONE,\n'
     '    .paletteTag = OBJ_EVENT_PAL_TAG_MAY,\n',
     'const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_LinkBrendan = {\n'
     '    .tileTag = TAG_NONE,\n'
     '    .paletteTag = OBJ_EVENT_PAL_TAG_BRENDAN,\n'),
    # 3) intro bike sprite: player.pal is shared by Brendan and May -> dedicated palette
    ('src/data/graphics/intro_scene.h',
     'const u16 gIntroPlayer_Pal[] = INCGFX_U16("graphics/intro/scene_2/player.pal", ".gbapal");\n',
     'const u16 gIntroPlayer_Pal[] = INCGFX_U16("graphics/intro/scene_2/player.pal", ".gbapal");\n'
     'const u16 gIntroBrendan_Pal[] = INCGFX_U16("graphics/intro/scene_2/brendan.pal", ".gbapal");\n'),
    ('include/graphics.h',
     'extern const u16 gIntroPlayer_Pal[];\n',
     'extern const u16 gIntroPlayer_Pal[];\nextern const u16 gIntroBrendan_Pal[];\n'),
    ('src/intro_credits_graphics.c',
     '    { .data = gIntroPlayer_Pal, .tag = TAG_BRENDAN },\n',
     '    { .data = gIntroBrendan_Pal, .tag = TAG_BRENDAN },\n'),
]


def apply_code_patches(tree, dry=False):
    results = []
    for rel, old, new in CODE_PATCHES:
        path = os.path.join(tree, rel)
        src = open(path, encoding='utf-8').read()
        if new in src:
            results.append(('already', rel))
            continue
        if src.count(old) != 1:
            raise SystemExit('patch anchor not found exactly once in %s:\n%s' % (rel, old))
        if not dry:
            src = src.replace(old, new, 1)
            with open(path, 'w', encoding='utf-8') as f:
                f.write(src)
        results.append(('patched', rel))
    return results


# --------------------------------------------------------------------------- previews

def preview(frames, pal, path, scale=4, cols=None, bg=((150, 196, 128), (160, 206, 138)), gap=2):
    rgb = {letter: gba_round(c) for letter, c in pal}
    w, h = len(frames[0][1][0]), len(frames[0][1])
    n = len(frames)
    cols = cols or n
    rows = (n + cols - 1) // cols
    W, H = cols * (w + gap) + gap, rows * (h + gap) + gap
    im = Image.new('RGB', (W, H), (90, 90, 96))
    px = im.load()
    for i, (_, g) in enumerate(frames):
        ox, oy = gap + (i % cols) * (w + gap), gap + (i // cols) * (h + gap)
        for y, row in enumerate(g):
            for x, c in enumerate(row):
                px[ox + x, oy + y] = bg[((x // 8) + (y // 8)) % 2] if c == TRANSPARENT else rgb[c]
    im = im.resize((W * scale, H * scale), Image.NEAREST)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    im.save(path)


# --------------------------------------------------------------------------- main

def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--tree', help='pokeemerald-expansion tree to write into')
    ap.add_argument('--source', help='pristine tree to copy untouched halves from (default: --tree)')
    ap.add_argument('--previews', help='directory for 4x preview sheets')
    ap.add_argument('--no-code', action='store_true', help='do not patch C sources')
    ap.add_argument('--check', action='store_true', help='only validate data')
    args = ap.parse_args()

    pals = load_palettes()
    stamps = load_stamps(os.path.join(DATA, 'ow/heads.txt'))
    built = []
    for rel, w, h, palname, out, layout in SHEETS:
        path = os.path.join(DATA, rel)
        if not os.path.exists(path):
            print('skip (no data yet): %s' % rel)
            continue
        frames = build_frames(path, w, h, stamps)
        for name, g in frames:
            letters = {l for l, _ in pals[palname]}
            bad = {c for r in g for c in r} - letters
            if bad:
                raise SystemExit('%s frame "%s": letters %s not in palette %s' % (rel, name, sorted(bad), palname))
        built.append((rel, frames, palname, out, layout))
    if args.check:
        print('data OK: %d sheets' % len(built))
        return

    if args.tree:
        src_tree = args.source or args.tree
        for rel, frames, palname, out, layout in built:
            im = sheet_image(frames, pals[palname], layout)
            save_png(im, os.path.join(args.tree, out), out in TRANSPARENCY_CHUNK)
            print('wrote %s (%d frames)' % (out, len(frames)))
        if os.path.exists(os.path.join(DATA, 'icons/frontier_head.txt')):
            heads = frontier_heads(pals, src_tree)
            save_png(heads, os.path.join(args.tree, 'graphics/frontier_pass/map_heads.png'))
            print('wrote graphics/frontier_pass/map_heads.png')
        for palname, out in PALETTE_FILES:
            write_jasc([rgb for _, rgb in pals[palname]], os.path.join(args.tree, out))
            print('wrote %s' % out)
        write_jasc(reflection_palette([rgb for _, rgb in pals['ow']]),
                   os.path.join(args.tree, 'graphics/object_events/palettes/brendan_reflection.pal'))
        print('wrote graphics/object_events/palettes/brendan_reflection.pal')
        if not args.no_code:
            for status, rel in apply_code_patches(args.tree):
                print('%s: %s' % (status, rel))

    if args.previews:
        for rel, frames, palname, out, layout in built:
            name = os.path.splitext(rel.replace('/', '_'))[0]
            cols = 8 if w == 64 else None
            preview(frames, pals[palname], os.path.join(args.previews, name + '.png'), 4,
                    cols=(4 if len(frames[0][1]) == 64 else None))
        print('previews in %s' % args.previews)


if __name__ == '__main__':
    main()
