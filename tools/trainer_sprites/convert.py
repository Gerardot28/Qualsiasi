#!/usr/bin/env python3
"""Trainer front-pic converter for pokeemerald-expansion (64x64, 4bpp, 16 colours).

Turns any trainer sprite (GBA 64x64 pics with embedded or external palettes,
DS 80x80 front sprites, Platinum animation sheets, Showdown-style trimmed
PNGs) into a drop-in `graphics/trainers/front_pics/*.png`:

  * 64x64, PNG colour type 3 (indexed), bit depth 4, exactly 16 palette slots
  * palette index 0 = transparent colour (default (115,197,164), the colour
    the stock expansion pics use), never used by a visible pixel
  * at most 15 visible colours, distinct after GBA 15-bit (BGR555) truncation

Fitting strategy (see README.md for the rationale):
  * the opaque bounding box already fits in 64x64 -> pure crop/re-place, the
    pixels are untouched (lossless);
  * it overflows by <= 4 px (--max-trim) -> the sparsest edge rows/columns
    are shaved off, still pixel-exact;
  * otherwise `--fit scale` (default) shrinks it with an
    outline-aware area-majority resampler that only ever outputs colours of
    the source palette (no blending, no new colours), or `--fit crop` keeps
    1:1 pixels and cuts the lower body (DS-style "waist-up" look).

Usage examples
  convert.py one SRC.png OUT.png [--pal X.pal] [--frame 0,0,80,80] [--fit scale|crop]
  convert.py check OUT.png [...]                 # validate finished pics
  convert.py sheet OUT_SHEET.png A.png B.png ... [--zoom 4] [--cols 8]
"""
import argparse
import json
import math
import os
import struct
import sys
import zlib

from PIL import Image, ImageDraw

SIZE = 64
TRANSPARENT_RGB = (115, 197, 164)


# --------------------------------------------------------------------------
# palettes
# --------------------------------------------------------------------------
def read_pal(path):
    """Read a JASC-PAL text palette or a raw .gbapal (BGR555 little endian)."""
    data = open(path, 'rb').read()
    if data.startswith(b'JASC-PAL'):
        lines = data.decode('ascii', 'replace').split()
        n = int(lines[2])
        vals = [int(v) for v in lines[3:3 + 3 * n]]
        return [tuple(vals[i:i + 3]) for i in range(0, 3 * n, 3)]
    out = []
    for (v,) in struct.iter_unpack('<H', data[: len(data) // 2 * 2]):
        r, g, b = v & 31, (v >> 5) & 31, (v >> 10) & 31
        out.append((r << 3 | r >> 2, g << 3 | g >> 2, b << 3 | b >> 2))
    return out


def gba15(c):
    """24-bit colour -> 15-bit key exactly as gbagfx truncates it."""
    return (c[0] >> 3, c[1] >> 3, c[2] >> 3)


def gba_expand(c):
    r, g, b = gba15(c)
    return (r << 3 | r >> 2, g << 3 | g >> 2, b << 3 | b >> 2)


# --------------------------------------------------------------------------
# loading
# --------------------------------------------------------------------------
def load_rgba(path, pal=None, frame=None, index0_transparent=None, bg_key=None):
    """Load any source sprite as RGBA (alpha is 0 or 255).

    pal      : external palette file to apply to an indexed PNG
    frame    : (x, y, w, h) sub-rectangle (e.g. frame 0 of a DS sheet)
    index0_transparent : treat palette index 0 as transparent (default: yes,
               unless a tRNS alpha table really hides some index)
    bg_key   : RGB colour to treat as transparent (for flat-background rips)
    """
    im = Image.open(path)
    im.load()
    if frame:
        x, y, w, h = frame
        im = im.crop((x, y, x + w, y + h))
    if im.mode == 'P':
        if pal:
            cols = read_pal(pal)
            flat = [v for c in cols for v in c]
            im.putpalette(flat + [0] * (768 - len(flat)))
        # GBA/DS convention: palette index 0 is the transparent colour.
        # A tRNS chunk is honoured only when it is a per-index alpha table
        # that really hides something; an integer tRNS pointing at another
        # index (Graphics Gale exports) or an all-0xFF table is ignored.
        trns = im.info.get('transparency')
        has_trns = isinstance(trns, (bytes, bytearray)) and any(v < 128 for v in trns)
        if index0_transparent is None:
            index0_transparent = not has_trns
        if not has_trns:
            im.info.pop('transparency', None)
        idx = im.copy()
        rgba = im.convert('RGBA')
        if index0_transparent:
            px = idx.load()
            out = rgba.load()
            for yy in range(im.height):
                for xx in range(im.width):
                    if px[xx, yy] == 0:
                        out[xx, yy] = (0, 0, 0, 0)
        im = rgba
    else:
        im = im.convert('RGBA')
        # flat-background rips (RGB, or RGBA without any transparent pixel):
        # key out the corner colour when all four corners agree
        if bg_key is None and im.getchannel('A').getextrema()[0] >= 128:
            w, h = im.size
            cs = {im.getpixel(p)[:3] for p in [(0, 0), (w - 1, 0), (0, h - 1), (w - 1, h - 1)]}
            if len(cs) == 1:
                bg_key = cs.pop()
    px = im.load()
    for yy in range(im.height):
        for xx in range(im.width):
            r, g, b, a = px[xx, yy]
            if a < 128 or (bg_key and (r, g, b) == tuple(bg_key)):
                px[xx, yy] = (0, 0, 0, 0)
            else:
                px[xx, yy] = (r, g, b, 255)
    return im


def opaque_bbox(im):
    return im.getchannel('A').getbbox()


# --------------------------------------------------------------------------
# colour helpers
# --------------------------------------------------------------------------
def _lin(v):
    v /= 255.0
    return v / 12.92 if v <= 0.04045 else ((v + 0.055) / 1.055) ** 2.4


def to_lab(c):
    r, g, b = (_lin(v) for v in c)
    x = (0.4124 * r + 0.3576 * g + 0.1805 * b) / 0.95047
    y = (0.2126 * r + 0.7152 * g + 0.0722 * b)
    z = (0.0193 * r + 0.1192 * g + 0.9505 * b) / 1.08883

    def f(t):
        return t ** (1 / 3) if t > 0.008856 else 7.787 * t + 16 / 116
    fx, fy, fz = f(x), f(y), f(z)
    return (116 * fy - 16, 500 * (fx - fy), 200 * (fy - fz))


def lab_dist(a, b):
    return math.sqrt(sum((p - q) ** 2 for p, q in zip(a, b)))


def luminance(c):
    return 0.299 * c[0] + 0.587 * c[1] + 0.114 * c[2]


# --------------------------------------------------------------------------
# resampling
# --------------------------------------------------------------------------
def downscale_majority(im, factor, alpha_keep=0.40, outline_boost=1.6):
    """Area-coverage majority vote. Output colours are a subset of the input.

    For every destination pixel the source footprint (1/factor wide) is
    intersected with the source grid; each source colour gets its covered
    area as vote. The pixel stays opaque when opaque coverage >= alpha_keep
    (a 1-px line that straddles two destination pixels covers ~0.4 of each,
    so thin strands, eyes and outlines survive). Among opaque colours the
    darkest ones (the outline/line-art colours, bottom 20% luminance) get
    `outline_boost` so 1-px outlines are not eaten by the fill colours.
    """
    sw, sh = im.size
    dw, dh = max(1, round(sw * factor)), max(1, round(sh * factor))
    src = im.load()
    colours = {}
    for y in range(sh):
        for x in range(sw):
            p = src[x, y]
            if p[3]:
                colours[p[:3]] = colours.get(p[:3], 0) + 1
    if not colours:
        return Image.new('RGBA', (dw, dh), (0, 0, 0, 0))
    lums = sorted(luminance(c) for c in colours)
    dark_cut = lums[max(0, int(len(lums) * 0.2) - 1)]
    boost = {c: (outline_boost if luminance(c) <= dark_cut else 1.0) for c in colours}
    out = Image.new('RGBA', (dw, dh), (0, 0, 0, 0))
    op = out.load()
    inv = 1.0 / factor
    for ty in range(dh):
        y0, y1 = ty * inv, (ty + 1) * inv
        for tx in range(dw):
            x0, x1 = tx * inv, (tx + 1) * inv
            votes = {}
            transp = 0.0
            total = 0.0
            for sy in range(int(y0), min(sh, int(math.ceil(y1)))):
                hy = min(y1, sy + 1) - max(y0, sy)
                if hy <= 0:
                    continue
                for sx in range(int(x0), min(sw, int(math.ceil(x1)))):
                    wx = min(x1, sx + 1) - max(x0, sx)
                    if wx <= 0:
                        continue
                    w = wx * hy
                    total += w
                    p = src[sx, sy]
                    if p[3]:
                        votes[p[:3]] = votes.get(p[:3], 0) + w
                    else:
                        transp += w
            if not votes or (total - transp) < alpha_keep * total:
                continue
            best = max(votes, key=lambda c: votes[c] * boost[c])
            op[tx, ty] = best + (255,)
    return out


def downscale_nearest_phase(im, factor):
    """Nearest neighbour with the sampling phase that best matches a box
    filter (kept for comparison; usually worse than majority)."""
    sw, sh = im.size
    dw, dh = max(1, round(sw * factor)), max(1, round(sh * factor))
    ref = im.resize((dw, dh), Image.BOX).load()
    src = im.load()
    best = None
    inv = 1.0 / factor
    for oy in (0.0, 0.25, 0.5, 0.75):
        for ox in (0.0, 0.25, 0.5, 0.75):
            cand = Image.new('RGBA', (dw, dh), (0, 0, 0, 0))
            cp = cand.load()
            err = 0
            for ty in range(dh):
                sy = min(sh - 1, int((ty + oy) * inv))
                for tx in range(dw):
                    sx = min(sw - 1, int((tx + ox) * inv))
                    p = src[sx, sy]
                    cp[tx, ty] = p
                    r = ref[tx, ty]
                    err += sum(abs(a - b) for a, b in zip(p, r))
            if best is None or err < best[0]:
                best = (err, cand)
    return best[1]


def cleanup(im):
    """Remove isolated single opaque pixels and fill single-pixel holes."""
    w, h = im.size
    px = im.load()
    snap = im.copy().load()

    def opaque(x, y):
        return 0 <= x < w and 0 <= y < h and snap[x, y][3] > 0
    for y in range(h):
        for x in range(w):
            n4 = [(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)]
            n8 = n4 + [(x + 1, y + 1), (x - 1, y - 1), (x + 1, y - 1), (x - 1, y + 1)]
            if snap[x, y][3]:
                if not any(opaque(a, b) for a, b in n8):
                    px[x, y] = (0, 0, 0, 0)
            else:
                if all(opaque(a, b) for a, b in n4):
                    cols = {}
                    for a, b in n4:
                        c = snap[a, b][:3]
                        cols[c] = cols.get(c, 0) + 1
                    px[x, y] = max(cols, key=cols.get) + (255,)
    return im


# --------------------------------------------------------------------------
# fitting into 64x64
# --------------------------------------------------------------------------
def fit_64(im, fit='scale', bottom_margin=0, crop_keep='top', scale=None, max_trim=4):
    """Return (64x64 RGBA, info dict)."""
    bb = opaque_bbox(im)
    if bb is None:
        raise ValueError('empty sprite')
    spr = im.crop(bb)
    w, h = spr.size
    info = {'bbox': bb, 'src_size': [w, h]}
    over_w, over_h = w - SIZE, h - (SIZE - bottom_margin)
    if scale is None and over_w <= 0 and over_h <= 0:
        info['method'] = 'crop'
    elif scale is None and fit != 'scale-only' and max(over_w, over_h) <= max_trim:
        # a few pixels too big: shave the sparsest edge rows/columns instead
        # of resampling the whole sprite (keeps it pixel-exact)
        spr, cut = _trim_edges(spr, max(0, over_w), max(0, over_h))
        w, h = spr.size
        info['method'] = 'crop-trim'
        info['trimmed'] = cut
    elif fit == 'crop' and scale is None:
        info['method'] = 'crop-cut'
        # keep the head: top-aligned, centred on the opaque mass horizontally
        if w > SIZE:
            cx = _mass_center_x(spr)
            left = int(round(min(max(cx - SIZE / 2, 0), w - SIZE)))
            spr = spr.crop((left, 0, left + SIZE, h))
            w = SIZE
        if h > SIZE - bottom_margin:
            top = 0 if crop_keep == 'top' else h - (SIZE - bottom_margin)
            spr = spr.crop((0, top, w, top + SIZE - bottom_margin))
            h = SIZE - bottom_margin
    else:
        f = scale if scale else min(SIZE / w, (SIZE - bottom_margin) / h)
        info['method'] = 'scale'
        info['factor'] = round(f, 4)
        spr = cleanup(downscale_majority(spr, f))
        bb2 = opaque_bbox(spr)
        spr = spr.crop(bb2)
        w, h = spr.size
        if w > SIZE or h > SIZE:
            spr = spr.crop((max(0, (w - SIZE) // 2), 0, max(0, (w - SIZE) // 2) + min(w, SIZE), min(h, SIZE)))
            w, h = spr.size
    canvas = Image.new('RGBA', (SIZE, SIZE), (0, 0, 0, 0))
    x = (SIZE - w) // 2
    y = SIZE - bottom_margin - h  # feet (or the cut edge) on the bottom row
    canvas.paste(spr, (x, y), spr)
    info['placed'] = [x, y, w, h]
    return canvas, info


def _trim_edges(im, n_cols, n_rows):
    """Remove n_cols columns and n_rows rows, each time from whichever edge
    currently has fewer opaque pixels. Returns (image, {'left':..,...})."""
    cut = {'left': 0, 'right': 0, 'top': 0, 'bottom': 0}
    a = im.getchannel('A')
    box = [0, 0, im.width, im.height]

    def count(x0, y0, x1, y1):
        return sum(1 for v in a.crop((x0, y0, x1, y1)).tobytes() if v)
    for _ in range(n_cols):
        l = count(box[0], box[1], box[0] + 1, box[3])
        r = count(box[2] - 1, box[1], box[2], box[3])
        if l <= r:
            box[0] += 1
            cut['left'] += 1
        else:
            box[2] -= 1
            cut['right'] += 1
    for _ in range(n_rows):
        t = count(box[0], box[1], box[2], box[1] + 1)
        b = count(box[0], box[3] - 1, box[2], box[3])
        if t < b:
            box[1] += 1
            cut['top'] += 1
        else:
            box[3] -= 1
            cut['bottom'] += 1
    return im.crop(tuple(box)), {k: v for k, v in cut.items() if v}


def _mass_center_x(im):
    a = im.getchannel('A').load()
    w, h = im.size
    tot = sx = 0
    for y in range(h):
        for x in range(w):
            if a[x, y]:
                tot += 1
                sx += x
    return sx / tot if tot else w / 2


# --------------------------------------------------------------------------
# quantisation to a GBA 16-colour palette
# --------------------------------------------------------------------------
def reduce_colours(im, max_colours=15, order=None):
    """Snap to BGR555, merge duplicates, then merge the closest colour pairs
    (least used into most used, CIELAB) until <= max_colours remain.
    Returns (new RGBA image, palette list, merges list)."""
    px = im.load()
    w, h = im.size
    counts = {}
    for y in range(h):
        for x in range(w):
            p = px[x, y]
            if p[3]:
                c = gba_expand(p[:3])
                px[x, y] = c + (255,)
                counts[c] = counts.get(c, 0) + 1
    merges = []
    mapping = {c: c for c in counts}
    labs = {c: to_lab(c) for c in counts}
    while len(counts) > max_colours:
        cs = list(counts)
        best = None
        for i in range(len(cs)):
            for j in range(i + 1, len(cs)):
                d = lab_dist(labs[cs[i]], labs[cs[j]])
                # weight by the smaller population: merging a rare colour is cheaper
                cost = d * math.sqrt(min(counts[cs[i]], counts[cs[j]]))
                if best is None or cost < best[0]:
                    best = (cost, cs[i], cs[j], d)
        _, a, b, d = best
        keep, drop = (a, b) if counts[a] >= counts[b] else (b, a)
        merges.append({'from': drop, 'to': keep, 'px': counts[drop], 'dE': round(d, 1)})
        counts[keep] += counts.pop(drop)
        for k, v in mapping.items():
            if v == drop:
                mapping[k] = keep
    if merges:
        for y in range(h):
            for x in range(w):
                p = px[x, y]
                if p[3]:
                    px[x, y] = mapping[p[:3]] + (255,)
    cols = list(counts)
    if order:
        pos = {gba_expand(c): i for i, c in enumerate(order)}
        cols.sort(key=lambda c: (pos.get(c, 999), -luminance(c)))
    else:
        cols.sort(key=lambda c: -luminance(c))
    return im, cols, merges


def to_indexed(im, palette, transparent_rgb=TRANSPARENT_RGB):
    t = gba_expand(transparent_rgb)
    if t in palette:  # collision: pick an unused magenta-ish key
        for cand in [(255, 0, 255), (0, 255, 255), (0, 128, 128)]:
            if gba_expand(cand) not in palette:
                t = gba_expand(cand)
                break
    full = [t] + palette + [(0, 0, 0)] * (15 - len(palette))
    lut = {c: i + 1 for i, c in enumerate(palette)}
    out = Image.new('P', im.size, 0)
    flat = [v for c in full for v in c]
    out.putpalette(flat)
    op = out.load()
    ip = im.load()
    for y in range(im.height):
        for x in range(im.width):
            p = ip[x, y]
            if p[3]:
                op[x, y] = lut[p[:3]]
    return out


def save_4bpp(im, path):
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    im.save(path, bits=4, optimize=False)


def source_palette_order(path, pal=None):
    im = Image.open(path)
    if im.mode != 'P':
        return None
    if pal:
        cols = read_pal(pal)
    else:
        flat = im.getpalette() or []
        cols = [tuple(flat[i:i + 3]) for i in range(0, min(48, len(flat) - len(flat) % 3), 3)]
    return [c for c in cols[1:16] if len(c) == 3]


def convert(src, out, pal=None, frame=None, fit='scale', bottom_margin=0,
            scale=None, bg_key=None, index0_transparent=None, keep_order=True,
            transparent_rgb=TRANSPARENT_RGB, max_trim=4):
    im = load_rgba(src, pal=pal, frame=frame, index0_transparent=index0_transparent, bg_key=bg_key)
    canvas, info = fit_64(im, fit=fit, bottom_margin=bottom_margin, scale=scale, max_trim=max_trim)
    order = source_palette_order(src, pal) if keep_order else None
    canvas, palette, merges = reduce_colours(canvas, 15, order)
    pim = to_indexed(canvas, palette, transparent_rgb)
    save_4bpp(pim, out)
    info.update({'colours': len(palette), 'merges': merges})
    issues = check(out)
    info['issues'] = issues
    return info


# --------------------------------------------------------------------------
# validation
# --------------------------------------------------------------------------
def png_bit_depth(path):
    with open(path, 'rb') as f:
        data = f.read(33)
    return data[24], data[25]  # bit depth, colour type


def check(path):
    issues = []
    im = Image.open(path)
    if im.size != (SIZE, SIZE):
        issues.append('size %s' % (im.size,))
    if im.mode != 'P':
        issues.append('mode %s (not indexed)' % im.mode)
        return issues
    depth, ctype = png_bit_depth(path)
    if ctype != 3:
        issues.append('PNG colour type %d' % ctype)
    if depth not in (4, 8):
        issues.append('bit depth %d' % depth)
    used = set(im.tobytes())
    if max(used) > 15:
        issues.append('uses palette index %d > 15' % max(used))
    pal = im.getpalette()
    ncols = len(pal) // 3
    pal = [tuple(pal[i:i + 3]) for i in range(0, min(48, len(pal)), 3)]
    if ncols > 16 and depth == 8 and max(used) > 15:
        issues.append('palette > 16 entries in use')
    # corners must be transparent (index 0)
    for xy in [(0, 0), (SIZE - 1, 0)]:
        if im.getpixel(xy) != 0:
            issues.append('top corner %s not index 0' % (xy,))
    vis = [pal[i] for i in used if i != 0 and i < len(pal)]
    keys = [gba15(c) for c in vis]
    if len(set(keys)) != len(keys):
        issues.append('visible colours collide after BGR555 truncation')
    if gba15(pal[0]) in keys:
        issues.append('transparent colour also used by a visible index')
    if 0 not in used:
        issues.append('no transparent pixels?')
    return issues


# --------------------------------------------------------------------------
# contact sheet
# --------------------------------------------------------------------------
def render(path, bg=None):
    im = Image.open(path)
    rgba = im.convert('RGBA')
    if im.mode == 'P':
        px = im.load()
        o = rgba.load()
        for y in range(im.height):
            for x in range(im.width):
                if px[x, y] == 0:
                    o[x, y] = (0, 0, 0, 0)
    base = Image.new('RGBA', im.size, bg or (0, 0, 0, 0))
    if bg is None:  # checkerboard shows transparency
        b = base.load()
        for y in range(im.height):
            for x in range(im.width):
                b[x, y] = (200, 200, 208, 255) if ((x // 4 + y // 4) & 1) else (232, 232, 240, 255)
    base.alpha_composite(rgba)
    return base.convert('RGB')


def sheet(paths, out, zoom=4, cols=8, labels=None, bg=(248, 248, 248)):
    n = len(paths)
    rows = (n + cols - 1) // cols
    cell_w, cell_h = SIZE * zoom + 8, SIZE * zoom + 22
    s = Image.new('RGB', (cols * cell_w, rows * cell_h), (255, 255, 255))
    d = ImageDraw.Draw(s)
    for i, p in enumerate(paths):
        x, y = (i % cols) * cell_w + 4, (i // cols) * cell_h + 4
        try:
            img = render(p, bg).resize((SIZE * zoom, SIZE * zoom), Image.NEAREST)
            s.paste(img, (x, y))
        except Exception as e:  # pragma: no cover
            d.text((x, y), 'ERR %s' % e, fill=(255, 0, 0))
        lab = labels[i] if labels else os.path.splitext(os.path.basename(p))[0]
        d.text((x, y + SIZE * zoom + 3), lab[:40], fill=(0, 0, 0))
    os.makedirs(os.path.dirname(os.path.abspath(out)), exist_ok=True)
    s.save(out)


# --------------------------------------------------------------------------
# CLI
# --------------------------------------------------------------------------
def _frame(s):
    return tuple(int(v) for v in s.split(',')) if s else None


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    o = sub.add_parser('one', help='convert one sprite')
    o.add_argument('src')
    o.add_argument('out')
    o.add_argument('--pal')
    o.add_argument('--frame', help='x,y,w,h sub-rectangle of the source')
    o.add_argument('--fit', choices=['scale', 'crop'], default='scale')
    o.add_argument('--scale', type=float, help='force a scale factor')
    o.add_argument('--bottom-margin', type=int, default=0)
    o.add_argument('--max-trim', type=int, default=4,
                   help='shave up to N edge px instead of rescaling (0 = never)')
    o.add_argument('--bg-key', help='r,g,b background colour to make transparent')
    c = sub.add_parser('check', help='validate finished pics')
    c.add_argument('paths', nargs='+')
    s = sub.add_parser('sheet', help='contact sheet')
    s.add_argument('out')
    s.add_argument('paths', nargs='+')
    s.add_argument('--zoom', type=int, default=4)
    s.add_argument('--cols', type=int, default=8)
    a = ap.parse_args(argv)
    if a.cmd == 'one':
        info = convert(a.src, a.out, pal=a.pal, frame=_frame(a.frame), fit=a.fit,
                       bottom_margin=a.bottom_margin, scale=a.scale,
                       bg_key=_frame(a.bg_key), max_trim=a.max_trim)
        print(json.dumps(info, default=list))
    elif a.cmd == 'check':
        bad = 0
        for p in a.paths:
            iss = check(p)
            if iss:
                bad += 1
                print('%s: %s' % (p, '; '.join(iss)))
        print('%d/%d ok' % (len(a.paths) - bad, len(a.paths)))
        return 1 if bad else 0
    elif a.cmd == 'sheet':
        sheet(a.paths, a.out, zoom=a.zoom, cols=a.cols)
    return 0


if __name__ == '__main__':
    sys.exit(main())
