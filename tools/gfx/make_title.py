#!/usr/bin/env python3
"""Generate the title-screen logo graphics for pokeemerald-expansion.

Produces, in the exact formats the build expects (see README.md):

  pokemon_logo.png          256x64, 8-bit indexed, 256-entry PLTE (indices <= 223)
  pokemon_logo.pal          JASC-PAL, 256 entries (only the first 224 are built)
  pokemon_logo.bin          1024-byte affine tilemap (identity map, unchanged format)
  emerald_version.png       128x32, 4-bit indexed, exactly 16 PLTE entries
  rayquaza_and_clouds.pal   (optional, --bg-recolor) 16-colour JASC palette

Layout (default, "--layout stacked"): the BG2 logo carries both lines,
"POKeMON" on top and the game word (e.g. MULTIVERSE) below it; the
"version banner" sprite (formerly "EMERALD VERSION") becomes a prismatic
"rift" flare that slides in under the word.  "--layout banner" puts only
POKeMON on the logo and the word in the 15-colour banner sprite, like the
original game (fine for words of up to ~6 letters).

Examples:
  make_title.py --word MULTIVERSE --out /tmp/title --preview /tmp/title/preview
  make_title.py --word MULTIVERSE --apply /home/user/work/gfx-tree
"""
import argparse
import os
import shutil
import struct
import sys

import numpy as np
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pixelart as px  # noqa: E402

# ---------------------------------------------------------------------------
# hardware / build constraints (pokeemerald-expansion 1.17.x, src/title_screen.c)
# ---------------------------------------------------------------------------
LOGO_W, LOGO_H = 256, 64          # 32x8 tiles, 8bpp, identity tilemap
LOGO_MAX_INDEX = 223              # pokemon_logo.gbapal is built with -num_colors 224
LOGO_SCREEN_X = 29                # BG2X = -29 << 8  -> texture x 0 is screen x 29
SCREEN_W = 240
LOGO_CENTER_X = SCREEN_W // 2 - LOGO_SCREEN_X   # texture x that lands on screen centre (91)
LOGO_MAX_W = 2 * LOGO_CENTER_X                   # widest centred logo (182)
BANNER_W, BANNER_H = 128, 32      # two 64x32 8bpp OBJs, 16 colours loaded
BANNER_SCREEN_X, BANNER_SCREEN_Y = 66, 50   # final top-left (centres 98/162, y 66)
SHINE_Y = 68                      # shine sprite centre y during phase 1 (logo at y 32)

FONT_WORD = "LilitaOne-Regular.ttf"
FONT_TOP = "LilitaOne-Regular.ttf"

# ---------------------------------------------------------------------------
# colour schemes
# ---------------------------------------------------------------------------
H = px.hexrgb
SCHEMES = {
    # cosmic multiverse: deep indigo/violet space, prismatic word, gold rims
    "cosmic": dict(
        outline=H("#140a33"),      # darkest outline
        extrude=[H("#4b2aa8"), H("#2f1878"), H("#1d0e52")],
        gold=[H("#fffbd0"), H("#ffe680"), H("#ffc94a"), H("#f39d2c"), H("#c8681c")],
        prism=[H("#ffffff"), H("#bff8ff"), H("#6fe3ff"), H("#58a6ff"), H("#7c6dff"),
               H("#a957f5"), H("#d64ad8"), H("#ff5fae")],
        prism_hue_spread=40.0,     # degrees of hue drift across the word
        top_fill=[H("#fffbe0"), H("#ffe680"), H("#ffcc4d"), H("#f6a531"), H("#d9741e")],
        sparkle={"#": H("#ffffff"), "+": H("#e9fbff"), "*": H("#9feaff"), ".": H("#7a8cff")},
        portal=[H("#ffffff"), H("#72f0ff"), H("#7c6dff"), H("#d64ad8"), H("#ff9a3c")],
        # background recolour (rayquaza_and_clouds.pal, 16 entries)
        sky=[H("#0b0626"), H("#140a3c"), H("#1f0f55"), H("#2d146b"), H("#3f1a7e"),
             H("#56208c"), H("#6e2896"), H("#86329c")],
        silhouette=H("#0d0724"),
        cloud_hi=H("#f3e8ff"), cloud_lo=H("#c49cff"),
    ),
    # eclipse: same structure, warmer gold word, violet extrusion
    "eclipse": dict(
        outline=H("#120826"),
        extrude=[H("#5b2fb0"), H("#3a1d80"), H("#22104f")],
        gold=[H("#fff6c8"), H("#ffe27a"), H("#ffc93c"), H("#f2a324"), H("#c9681a")],
        prism=[H("#fffef0"), H("#fff3b0"), H("#ffe070"), H("#ffc840"), H("#f7a430"),
               H("#e98226"), H("#d4641e"), H("#b84a1a")],
        prism_hue_spread=0.0,
        top_fill=[H("#f4ecff"), H("#d9c8ff"), H("#b79cff"), H("#9474f0"), H("#7050d0")],
        sparkle={"#": H("#ffffff"), "+": H("#fff6d0"), "*": H("#ffd46a"), ".": H("#b07cff")},
        portal=[H("#ffffff"), H("#ffe680"), H("#ffb040"), H("#a957f5"), H("#4b2aa8")],
        sky=[H("#080512"), H("#100a26"), H("#190f3a"), H("#22134c"), H("#2c175c"),
             H("#391b68"), H("#471f70"), H("#562476")],
        silhouette=H("#07040f"),
        cloud_hi=H("#ffe9b8"), cloud_lo=H("#b48cff"),
    ),
}


def parse_args():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--word", default="MULTIVERSE", help="game word (A-Z, <= 10 letters)")
    ap.add_argument("--top", default="POKÉMON", help="top wordmark text")
    ap.add_argument("--scheme", default="cosmic", choices=sorted(SCHEMES))
    ap.add_argument("--color", action="append", default=[], metavar="KEY=#RRGGBB[,#RRGGBB..]",
                    help="override a scheme colour (list keys accept comma-separated colours)")
    ap.add_argument("--layout", default="stacked", choices=["stacked", "banner"])
    ap.add_argument("--word-cap", type=int, default=0, help="force cap height of the word (px)")
    ap.add_argument("--top-cap", type=int, default=0, help="force cap height of POKeMON (px)")
    ap.add_argument("--no-bg-recolor", action="store_true",
                    help="keep the original Rayquaza/cloud palette")
    ap.add_argument("--out", default="./title_out", help="output directory for the build files")
    ap.add_argument("--preview", help="also write preview PNGs (mock title screen, 3x) here")
    ap.add_argument("--apply", metavar="TREE", help="copy the files into TREE/graphics/title_screen/")
    return ap.parse_args()


def apply_overrides(scheme, overrides):
    s = dict(scheme)
    for o in overrides:
        k, v = o.split("=", 1)
        cols = [px.hexrgb(c) for c in v.split(",")]
        if k not in s:
            sys.exit(f"unknown colour key {k}; known: {', '.join(sorted(s))}")
        s[k] = cols if isinstance(s[k], list) else cols[0]
    return s


# ---------------------------------------------------------------------------
# layered canvas
# ---------------------------------------------------------------------------
class Canvas:
    """High-resolution layer-id map + one colour function per layer."""

    def __init__(self, W, H, ss=6):
        self.W, self.H, self.ss = W, H, ss
        self.ids = np.full((H * ss, W * ss), px.TRANSPARENT, np.int16)
        self.fns = []

    def layer(self, fn):
        self.fns.append(fn)
        return len(self.fns) - 1

    def paint(self, mask_hr, lid):
        self.ids[mask_hr] = lid

    def resolve(self, cleanup=True):
        covs = px.downsample_ids(self.ids, self.ss, len(self.fns))
        dom, sec = px.resolve(covs)
        if cleanup:
            dom = px.cleanup_alpha(dom)
            sec[dom == px.TRANSPARENT] = -1
        return dom, sec

    def render(self, dom, sec):
        return px.paint(dom, sec, self.fns, self.H, self.W)


def band_colour(ramp_cols, top, bottom):
    """Vertical banded gradient between rows top..bottom."""
    cols = np.asarray(ramp_cols, np.float32)
    n = len(cols)

    def fn(ys, xs):
        t = (ys - top) / max(1.0, (bottom - top))
        k = np.clip(np.floor(t * n), 0, n - 1).astype(int)
        return cols[k]
    return fn


def flat(c):
    c = np.asarray(c, np.float32)
    return lambda ys, xs: np.tile(c, (len(ys), 1))


class Word:
    """One line of outlined, extruded, gradient-filled text on a Canvas."""

    def __init__(self, canvas, text, font, tracking, cx, top_y, rim_w, out_w, ext_d,
                 adjust=None):
        self.cv = canvas
        ss = canvas.ss
        self.run = px.TextRun(text, font, tracking=tracking, adjust=adjust)
        self.cap = self.run.cap
        ink_top = self.run.ink[1]           # negative (above baseline), includes accents
        pad = rim_w + out_w
        self.baseline = top_y + pad - ink_top
        self.cap_top = self.baseline - self.cap
        ox = int(round(cx - self.run.width / 2.0))
        self.ox = ox
        cov, letters = self.run.coverage(canvas.W, canvas.H, ox, self.baseline)
        self.cov = cov
        self.letters = letters
        self.owner = np.argmax(np.stack(letters), axis=0)
        self.glyph = px.upsample_mask(cov, ss)
        R = int(np.ceil((rim_w + out_w) * ss)) + 2
        self.dist = px.distance_outside(self.glyph, R) / ss
        self.rim_w, self.out_w, self.ext_d = rim_w, out_w, ext_d
        self.sil = self.dist <= rim_w + out_w
        # extrusion: silhouette swept straight down by ext_d pixels
        ext = np.zeros_like(self.sil)
        for k in range(1, int(ext_d * ss) + 1):
            ext |= px.shift(self.sil, k, 0)
        self.ext = ext & ~self.sil
        self.ext_out = px.dilate(self.ext | self.sil, out_w, ss) & ~(self.ext | self.sil)
        self.bottom = self.baseline + rim_w + out_w + ext_d + out_w

    def holes(self):
        """Counters (enclosed transparent areas) of the glyphs, at 1x."""
        op = self.cov >= 0.5
        H, W = op.shape
        seen = np.zeros_like(op)
        stack = [(y, x) for y in range(H) for x in (0, W - 1)] + [(y, x) for x in range(W) for y in (0, H - 1)]
        while stack:
            y, x = stack.pop()
            if seen[y, x] or op[y, x]:
                continue
            seen[y, x] = True
            for dy, dx in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                yy, xx = y + dy, x + dx
                if 0 <= yy < H and 0 <= xx < W and not seen[yy, xx] and not op[yy, xx]:
                    stack.append((yy, xx))
        return ~op & ~seen


def layer_word(cv, w, fill_fn, rim_cols, scheme, extrude_cols, top_hi=None):
    """Paint a Word's layers (back to front) and return the layer ids."""
    ids = {}
    ids["ext_out"] = cv.layer(flat(scheme["outline"]))
    e_top = w.baseline
    ids["ext"] = cv.layer(band_colour(extrude_cols, e_top, w.bottom))
    ids["out"] = cv.layer(flat(scheme["outline"]))
    if w.rim_w > 0:
        ids["rim"] = cv.layer(band_colour(rim_cols, w.cap_top - w.rim_w - 1, w.baseline + w.rim_w))
    ids["fill"] = cv.layer(fill_fn)
    cv.paint(w.ext_out, ids["ext_out"])
    cv.paint(w.ext, ids["ext"])
    cv.paint(w.sil & ~w.glyph, ids["out"])
    if w.rim_w > 0:
        cv.paint((w.dist <= w.rim_w) & ~w.glyph, ids["rim"])
    cv.paint(w.glyph, ids["fill"])
    return ids


def prism_fill(w, scheme):
    """Iridescent fill: vertical prismatic ramp + per-letter hue drift."""
    stops = scheme["prism"]
    n_letters = len(w.run.text)
    spread = scheme.get("prism_hue_spread", 0.0)
    nb = 7
    ramps = []
    for i in range(n_letters):
        off = (i / max(1, n_letters - 1) - 0.5) * spread
        base = px.ramp(stops[1:], nb)      # stops[0] reserved for highlight
        ramps.append([px.gba(px.hue_shift(c, off)) for c in base])
    ramps = np.asarray(ramps, np.float32)
    top, bot = w.cap_top, w.baseline
    owner = w.owner

    def fn(ys, xs):
        t = (ys - top) / max(1.0, bot - top)
        k = np.clip(np.floor(t * nb), 0, nb - 1).astype(int)
        return ramps[owner[ys, xs], k]
    return fn


def add_bevel(rgba, dom, fill_id, hi, lo=None, rim_id=None, rim_hi=None):
    """Hand-style highlight pixels: light top edge on the fill (and rim)."""
    Hh, Ww = dom.shape
    up = np.full_like(dom, px.TRANSPARENT)
    up[1:] = dom[:-1]
    m = (dom == fill_id) & (up != fill_id)
    rgba[m, :3] = hi
    if lo is not None:
        dn = np.full_like(dom, px.TRANSPARENT)
        dn[:-1] = dom[1:]
        m2 = (dom == fill_id) & (dn != fill_id) & ~m
        rgba[m2, :3] = lo
    if rim_id is not None and rim_hi is not None:
        m3 = (dom == rim_id) & (up != rim_id) & (up != fill_id)
        rgba[m3, :3] = rim_hi


# ---------------------------------------------------------------------------
# logo
# ---------------------------------------------------------------------------
def fit_font(text, name, cap_max, max_w, extra_w, tracking):
    for cap in range(cap_max, 6, -1):
        f = px.font_for_cap(name, cap)
        run = px.TextRun(text, f, tracking=tracking)
        if run.width + extra_w <= max_w:
            return f, cap
    raise SystemExit(f"cannot fit {text!r} in {max_w}px")


def build_logo(args, scheme):
    cv = Canvas(LOGO_W, LOGO_H, ss=6)
    word = args.word.upper()
    stacked = args.layout == "stacked"
    # ---- POKeMON (top line)
    top_rim, top_out, top_ext = 0, 2, 2
    top_cap = args.top_cap or (15 if stacked else 26)
    top_font, top_cap = fit_font(args.top, FONT_TOP, top_cap, LOGO_MAX_W - 2,
                                 2 * (top_rim + top_out), -1)
    top = Word(cv, args.top, top_font, -1, LOGO_CENTER_X, 1, top_rim, top_out, top_ext)
    top_ids = layer_word(cv, top, band_colour([px.gba(c) for c in scheme["top_fill"]],
                                              top.cap_top, top.baseline),
                         [], scheme, [px.gba(c) for c in scheme["extrude"]])
    words = [("top", top, top_ids)]
    if stacked:
        rim_w, out_w, ext_d = 2, 1, 3
        cap = args.word_cap or 23
        f, cap = fit_font(word, FONT_WORD, cap, LOGO_MAX_W - 1, 2 * (rim_w + out_w), -1)
        # place the word so that its bottom (incl. extrusion) ends at y 62
        probe = Word(Canvas(LOGO_W, LOGO_H, 1), word, f, -1, LOGO_CENTER_X, 0, rim_w, out_w, ext_d)
        y0 = 62 - probe.bottom
        y0 = max(y0, top.bottom - 9)
        w = Word(cv, word, f, -1, LOGO_CENTER_X, y0, rim_w, out_w, ext_d)
        w_ids = layer_word(cv, w, prism_fill(w, scheme), [px.gba(c) for c in scheme["gold"]],
                           scheme, [px.gba(c) for c in scheme["extrude"]])
        words.append(("word", w, w_ids))
    dom, sec = cv.resolve()
    rgba = cv.render(dom, sec)
    # bevel highlights
    for name, w, ids in words:
        if name == "top":
            add_bevel(rgba, dom, ids["fill"], px.gba(scheme["top_fill"][0]))
        else:
            add_bevel(rgba, dom, ids["fill"], px.gba(scheme["prism"][0]), None,
                      ids.get("rim"), px.gba(scheme["gold"][0]))
    rgba = px.snap_gba(rgba)
    info = dict(words=words, dom=dom)
    return rgba, info


def sparkle_logo(rgba, info, scheme):
    """A few hand-placed sparkles on letter corners (never in empty space)."""
    op = rgba[..., 3] > 0
    cols = {k: px.gba(v) for k, v in scheme["sparkle"].items()}
    for name, w, ids in info["words"]:
        if name == "word":
            # top-left corner of the first letter and top-right of the last
            b0 = w.run.boxes[0]
            x0 = w.ox + b0[0] - w.run.ink[0] + 2
            stamp_at(rgba, "small", x0, w.cap_top + 1, cols, op)
            bl = w.run.boxes[-1]
            x1 = w.ox + bl[2] - w.run.ink[0] - 3
            stamp_at(rgba, "tiny", x1, w.baseline - 3, cols, op)
        else:
            # glint on the accent
            for i, ch in enumerate(w.run.text):
                if ch in "ÉéÈè":
                    b = w.run.boxes[i]
                    xa = w.ox + (b[0] + b[2]) // 2 - w.run.ink[0] + 1
                    stamp_at(rgba, "tiny", xa, w.baseline + b[1] + 2, cols, op)


def stamp_at(rgba, kind, x, y, cols, op):
    px.stamp(rgba, kind, int(x), int(y), cols, only_on=op)


# ---------------------------------------------------------------------------
# banner sprite: rift flare (stacked layout) or the word (banner layout)
# ---------------------------------------------------------------------------
def build_rift_banner(scheme, word_bottom_screen):
    """128x32, <=15 colours: a prismatic horizontal rift with a star."""
    W, Hh = BANNER_W, BANNER_H
    rgba = np.zeros((Hh, W, 4), np.float32)
    cx = SCREEN_W // 2 - BANNER_SCREEN_X          # banner x of the screen centre (54)
    cy = max(4, min(Hh - 6, word_bottom_screen + 3 - BANNER_SCREEN_Y))
    half = 52
    core = px.gba((255, 255, 255))
    cyan = px.gba(scheme["prism"][2])
    blue = px.gba(scheme["prism"][4])
    mag = px.gba(scheme["prism"][6])
    gold = px.gba(scheme["gold"][2])
    for x in range(cx - half, cx + half + 1):
        d = abs(x - cx) / half              # 0 centre .. 1 tips
        if not (0 <= x < W):
            continue
        # core line (1px), brighter near the centre
        c = core if d < 0.35 else (px.gba(scheme["prism"][1]) if d < 0.7 else cyan)
        rgba[cy, x, :3] = c
        rgba[cy, x, 3] = 255
        # chromatic fringes: cyan above, magenta below, tapering
        if d < 0.62:
            rgba[cy - 1, x, :3] = cyan if d < 0.3 else blue
            rgba[cy - 1, x, 3] = 255
            rgba[cy + 1, x, :3] = mag if d < 0.3 else blue
            rgba[cy + 1, x, 3] = 255
        if d < 0.22:
            rgba[cy - 2, x, :3] = blue
            rgba[cy - 2, x, 3] = 255
            rgba[cy + 2, x, :3] = px.gba(scheme["extrude"][0])
            rgba[cy + 2, x, 3] = 255
    cols = {"#": core, "+": px.gba(scheme["gold"][0]), "*": gold, ".": px.gba(scheme["gold"][3])}
    px.stamp(rgba, "big", cx, cy, cols)
    # two small sparkles on the line
    small = {"#": core, "+": px.gba(scheme["prism"][1]), "*": cyan, ".": blue}
    px.stamp(rgba, "cross", cx - 34, cy, small)
    px.stamp(rgba, "cross", cx + 34, cy, small)
    return px.snap_gba(rgba)


def build_word_banner(args, scheme):
    """Original-style banner: the word itself in 15 colours."""
    cv = Canvas(BANNER_W, BANNER_H, ss=6)
    word = args.word.upper()
    rim_w, out_w, ext_d = 1, 1, 2
    cx = SCREEN_W // 2 - BANNER_SCREEN_X
    maxw = 2 * min(cx, BANNER_W - cx)
    cap = args.word_cap or 18
    try:
        f, cap = fit_font(word, FONT_WORD, cap, maxw, 2 * (rim_w + out_w), -1)
    except SystemExit:
        cx = BANNER_W // 2
        f, cap = fit_font(word, FONT_WORD, cap, BANNER_W, 2 * (rim_w + out_w), -1)
    probe = Word(Canvas(BANNER_W, BANNER_H, 1), word, f, -1, cx, 0, rim_w, out_w, ext_d)
    y0 = max(0, (BANNER_H - probe.bottom) // 2)
    w = Word(cv, word, f, -1, cx, y0, rim_w, out_w, ext_d)
    nb = 5
    fill = band_colour([px.gba(c) for c in px.ramp(scheme["prism"][1:], nb)], w.cap_top, w.baseline)
    ids = layer_word(cv, w, fill, [px.gba(scheme["gold"][1]), px.gba(scheme["gold"][3])], scheme,
                     [px.gba(scheme["extrude"][0]), px.gba(scheme["extrude"][2])])
    dom, sec = cv.resolve()
    rgba = cv.render(dom, sec)
    add_bevel(rgba, dom, ids["fill"], px.gba(scheme["prism"][0]))
    return px.snap_gba(rgba)


# ---------------------------------------------------------------------------
# background palette recolour
# ---------------------------------------------------------------------------
def recolor_bg(orig_pal, scheme):
    """Map the 16-colour Rayquaza/cloud palette onto the scheme.

    Original usage (rayquaza.png / clouds.png):
      4..10 sky gradient (bottom->top), 11 silhouette, 15 markings (code
      pulses this one every 4 frames), 2 white (clouds + eyes), 12 light
      cloud fringe; 0/1/3/13/14 unused by the pixels."""
    pal = list(orig_pal)
    sky = scheme["sky"]
    # index 10 = top of the sky ... index 4 = bottom
    for i, idx in enumerate(range(10, 3, -1)):
        pal[idx] = px.gba(sky[min(i, len(sky) - 1)])
    pal[3] = px.gba(sky[-1])
    pal[1] = px.gba(sky[-1])
    pal[11] = px.gba(scheme["silhouette"])
    pal[15] = px.gba(scheme["silhouette"])
    pal[2] = px.gba(scheme["cloud_hi"])
    pal[12] = px.gba(scheme["cloud_lo"])
    pal[13] = px.gba(scheme["cloud_hi"])
    return pal


# ---------------------------------------------------------------------------
# preview compositor (approximation of phase 3 + shine)
# ---------------------------------------------------------------------------
def load_tilemap_bg(tree, png, binf, pal):
    im = Image.open(os.path.join(tree, png))
    w = im.size[0]
    p = np.asarray(im)
    d = open(os.path.join(tree, binf), "rb").read()
    ent = struct.unpack("<%dH" % (len(d) // 2), d)
    out = np.zeros((256, 256, 4), np.uint8)
    for i, e in enumerate(ent):
        t = e & 0x3FF
        tx, ty = (t % (w // 8)) * 8, (t // (w // 8)) * 8
        tile = p[ty:ty + 8, tx:tx + 8]
        if (e >> 10) & 1:
            tile = tile[:, ::-1]
        if (e >> 11) & 1:
            tile = tile[::-1]
        cx, cy = (i % 32) * 8, (i // 32) * 8
        for yy in range(8):
            for xx in range(8):
                v = tile[yy, xx]
                if v:
                    out[cy + yy, cx + xx, :3] = pal[v]
                    out[cy + yy, cx + xx, 3] = 255
    return out


def to5(a):
    return (a.astype(np.int32) >> 3)


def from5(a):
    a = np.clip(a, 0, 31).astype(np.int32)
    return ((a << 3) | (a >> 2)).astype(np.uint8)


def compose_preview(orig_dir, logo_rgba, banner_rgba, bgpal, logo_y=0, with_bg=True, shine_x=None,
                    backdrop=(0, 0, 0), banner=True, press_start=True):
    scr = np.zeros((160, 240, 3), np.int32)
    scr[:] = to5(np.array(backdrop))
    if with_bg:
        bg0 = load_tilemap_bg(orig_dir, "rayquaza.png", "rayquaza.bin", bgpal)[:160, :240]
        bg1 = load_tilemap_bg(orig_dir, "clouds.png", "clouds.bin", bgpal)[:160, :240]
        m0 = bg0[..., 3] > 0
        scr[m0] = to5(bg0[..., :3][m0])
        m1 = bg1[..., 3] > 0
        scr[m1] = np.minimum(31, (to5(bg1[..., :3][m1]) * 6 + scr[m1] * 15) // 16)
    # BG2 logo
    for y in range(160):
        ty = y - logo_y
        if not (0 <= ty < LOGO_H):
            continue
        for x in range(240):
            tx = x - LOGO_SCREEN_X
            if 0 <= tx < LOGO_W and logo_rgba[ty, tx, 3]:
                c = to5(logo_rgba[ty, tx, :3])
                if shine_x is not None:
                    sx = x - (shine_x - 32)
                    sy = y - (SHINE_Y - 32)
                    if 0 <= sx < 64 and 0 <= sy < 64 and SHINE[sy, sx]:
                        c = c + ((31 - c) * 12) // 16
                scr[y, x] = c
    if banner and banner_rgba is not None:
        for y in range(BANNER_H):
            for x in range(BANNER_W):
                if banner_rgba[y, x, 3]:
                    scr[BANNER_SCREEN_Y + y, BANNER_SCREEN_X + x] = to5(banner_rgba[y, x, :3])
    if press_start:
        ps = Image.open(os.path.join(orig_dir, "press_start.png"))
        pp = np.asarray(ps)
        ppal = np.array(ps.getpalette()[:48]).reshape(16, 3)
        # "PRESS START" = first 5 32x8 frames of row 0 offset by 1 tile (anim frames 1,5,9..)
        def blit(row_tiles, y0):
            x0 = 128 - 64 - 16
            for k, t0 in enumerate(row_tiles):
                for j in range(4):
                    t = t0 + j
                    tx, ty = (t % 20) * 8, (t // 20) * 8
                    tile = pp[ty:ty + 8, tx:tx + 8]
                    for yy in range(8):
                        for xx in range(8):
                            v = tile[yy, xx]
                            if v:
                                X = x0 + k * 32 + j * 8 + xx
                                Y = y0 - 4 + yy
                                if 0 <= X < 240 and 0 <= Y < 160:
                                    scr[Y, X] = to5(ppal[v])
        blit([1, 5, 9, 13, 17], 108)
        blit([21, 25, 29, 33, 37], 148)
    return from5(scr)


SHINE = None


def load_shine(orig_dir):
    global SHINE
    SHINE = np.asarray(Image.open(os.path.join(orig_dir, "logo_shine.png"))) != 0


# ---------------------------------------------------------------------------
def main():
    args = parse_args()
    word = args.word.upper()
    if not word.isalpha() or len(word) > 10:
        sys.exit("--word must be letters only, at most 10")
    scheme = apply_overrides(SCHEMES[args.scheme], args.color)
    here = os.path.dirname(os.path.abspath(__file__))
    orig_dir = None
    for cand in ("/home/user/pex-orig/graphics/title_screen",
                 os.path.join(args.apply or "", "graphics/title_screen")):
        if cand and os.path.exists(os.path.join(cand, "rayquaza.png")):
            orig_dir = cand
            break
    os.makedirs(args.out, exist_ok=True)

    logo, info = build_logo(args, scheme)
    sparkle_logo(logo, info, scheme)

    # ---- logo: indexed + palette
    assert logo[0:8, 0:8, 3].max() == 0, "tile 0 must stay transparent (used by the empty map cells)"
    idx, cols = px.to_indexed(logo, LOGO_MAX_INDEX, first_index=1)
    pal = [(0, 0, 0)] + cols
    assert len(pal) - 1 <= LOGO_MAX_INDEX
    px.save_indexed_png(os.path.join(args.out, "pokemon_logo.png"), idx, pal, 256)
    px.write_jasc(os.path.join(args.out, "pokemon_logo.pal"), pal, 256)
    # identity affine tilemap: 32x8 visible tiles, rest tile 0 (blank)
    tm = bytes(list(range(256)) + [0] * (1024 - 256))
    open(os.path.join(args.out, "pokemon_logo.bin"), "wb").write(tm)

    # ---- banner
    if args.layout == "stacked":
        wword = [w for n, w, _ in info["words"] if n == "word"][0]
        banner = build_rift_banner(scheme, wword.bottom)
    else:
        banner = build_word_banner(args, scheme)
    bidx, bcols = px.to_indexed(banner, 15, first_index=1)
    bpal = [(0, 0, 0)] + bcols
    px.save_indexed_png(os.path.join(args.out, "emerald_version.png"), bidx, bpal, 16)

    # ---- background palette
    bgpal_orig = px.read_jasc(os.path.join(orig_dir, "rayquaza_and_clouds.pal"))
    bgpal = bgpal_orig if args.no_bg_recolor else recolor_bg(bgpal_orig, scheme)
    if not args.no_bg_recolor:
        px.write_jasc(os.path.join(args.out, "rayquaza_and_clouds.pal"), bgpal, 16)

    print(f"logo: {len(cols)} colours (max {LOGO_MAX_INDEX}); banner: {len(bcols)} colours (max 15)")
    for n, w, _ in info["words"]:
        ink = np.nonzero(logo[..., 3])
        print(f"  {n}: '{w.run.text}' cap={w.cap}px width={w.run.width}px baseline={w.baseline} bottom={w.bottom}")
    ys, xs = np.nonzero(logo[..., 3])
    print(f"  logo ink: x {xs.min()}..{xs.max()} (screen {xs.min() + LOGO_SCREEN_X}..{xs.max() + LOGO_SCREEN_X}),"
          f" y {ys.min()}..{ys.max()}")

    if args.preview:
        os.makedirs(args.preview, exist_ok=True)
        load_shine(orig_dir)
        logo_rgb = np.zeros_like(logo)
        logo_rgb[..., :3] = np.array(pal, np.uint8)[idx]
        logo_rgb[..., 3] = np.where(idx > 0, 255, 0)
        ban_rgb = np.zeros_like(banner)
        ban_rgb[..., :3] = np.array(bpal, np.uint8)[bidx]
        ban_rgb[..., 3] = np.where(bidx > 0, 255, 0)
        shots = {
            "final": compose_preview(orig_dir, logo_rgb, ban_rgb, bgpal),
            "phase1_shine": compose_preview(orig_dir, logo_rgb, ban_rgb, bgpal, logo_y=32, with_bg=False,
                                            shine_x=110, banner=False, press_start=False),
            "phase1_white": compose_preview(orig_dir, logo_rgb, ban_rgb, bgpal, logo_y=32, with_bg=False,
                                            shine_x=150, backdrop=(255, 255, 255), banner=False,
                                            press_start=False),
        }
        for k, im in shots.items():
            Image.fromarray(im, "RGB").resize((720, 480), Image.NEAREST).save(
                os.path.join(args.preview, f"preview_{k}.png"))
        big = np.zeros((LOGO_H, LOGO_W, 3), np.uint8)
        big[:] = (40, 40, 48)
        m = logo_rgb[..., 3] > 0
        big[m] = logo_rgb[..., :3][m]
        Image.fromarray(big).resize((LOGO_W * 4, LOGO_H * 4), Image.NEAREST).save(
            os.path.join(args.preview, "logo_x4.png"))
        bb = np.zeros((BANNER_H, BANNER_W, 3), np.uint8)
        bb[:] = (40, 40, 48)
        m = ban_rgb[..., 3] > 0
        bb[m] = ban_rgb[..., :3][m]
        Image.fromarray(bb).resize((BANNER_W * 4, BANNER_H * 4), Image.NEAREST).save(
            os.path.join(args.preview, "banner_x4.png"))

    if args.apply:
        dst = os.path.join(args.apply, "graphics", "title_screen")
        if not os.path.isdir(dst):
            sys.exit(f"{dst} does not exist")
        names = ["pokemon_logo.png", "pokemon_logo.pal", "pokemon_logo.bin", "emerald_version.png"]
        if not args.no_bg_recolor:
            names.append("rayquaza_and_clouds.pal")
        for n in names:
            shutil.copy(os.path.join(args.out, n), os.path.join(dst, n))
        print(f"applied {len(names)} files to {dst}")


if __name__ == "__main__":
    main()
