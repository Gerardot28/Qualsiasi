#!/usr/bin/env python3
"""Generate the game icon, macOS .icns, 3DS icons/banner and a box-art cover.

All artwork is ORIGINAL pixel art built from geometric shapes and the same
layered/anti-aliasing engine as the title logo (pixelart.py), drawn
natively at every small size (no blurry downscaling):

  emblem = cosmic "rift portal": prismatic ring with gold rims, a split
  disc (magenta "sky" over a starry void - a nod to a capture ball, but not
  its design), a glowing rift for an equator and a gold medallion carrying
  the initial of the game word.

Outputs (in --out):
  icon_1024.png (+ icon_512/256/128/64/32/16.png)   macOS-style, transparent margin
  icon.icns                                          16..1024 (via Pillow)
  3ds_icon_48.png, 3ds_icon_24.png                   opaque, full bleed (SMDH)
  3ds_banner_256x128.png                             banner image for injectors (NSUI)
  cover_512.png                                      box-art style cover with the title
  preview_sheet.png                                  everything on one sheet

Example:
  make_icon.py --word MULTIVERSE --out /home/user/work/gfx-out/icons
"""
import argparse
import math
import os
import sys

import numpy as np
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import make_title as mt  # noqa: E402
import pixelart as px  # noqa: E402

G = px.gba


# ---------------------------------------------------------------------------
# small shape toolkit on a Canvas (1x pixel units, sub-pixel exact)
# ---------------------------------------------------------------------------
class Scene(mt.Canvas):
    def __init__(self, W, H, ss=8):
        super().__init__(W, H, ss)
        ys = (np.arange(H * ss) + 0.5) / ss
        xs = (np.arange(W * ss) + 0.5) / ss
        self.X, self.Y = np.meshgrid(xs, ys)

    def circle(self, cx, cy, r):
        return (self.X - cx) ** 2 + (self.Y - cy) ** 2 <= r * r

    def ring(self, cx, cy, r_in, r_out):
        d2 = (self.X - cx) ** 2 + (self.Y - cy) ** 2
        return (d2 <= r_out * r_out) & (d2 > r_in * r_in)

    def roundbox(self, x0, y0, x1, y1, r):
        cx, cy = (x0 + x1) / 2, (y0 + y1) / 2
        hx, hy = (x1 - x0) / 2 - r, (y1 - y0) / 2 - r
        qx = np.maximum(np.abs(self.X - cx) - hx, 0)
        qy = np.maximum(np.abs(self.Y - cy) - hy, 0)
        return qx * qx + qy * qy <= r * r

    def hband(self, y0, y1):
        return (self.Y >= y0) & (self.Y < y1)


def vgrad(cols, top, bottom):
    return mt.band_colour([G(c) for c in cols], top, bottom)


def conic(cols, cx, cy, offset_deg=0.0, steps=None, r_in=None, r_out=None):
    """Angle-banded colours around a ring.

    steps: number of angular bands (colours interpolated between the stops);
    r_in/r_out: if given, add radial volume - a light band on the inner
    third and a darker band on the outer third of the ring."""
    stops = list(cols)
    n = steps or len(stops)
    ring_cols = px.ramp(stops + [stops[0]], n + 1)[:n]
    base = np.asarray([G(c) for c in ring_cols], np.float32)
    light = np.asarray([G(px.mix(c, (255, 255, 255), 0.35)) for c in ring_cols], np.float32)
    dark = np.asarray([G(px.mix(c, (20, 10, 60), 0.35)) for c in ring_cols], np.float32)

    def fn(ys, xs):
        dx, dy = xs + 0.5 - cx, ys + 0.5 - cy
        a = (np.degrees(np.arctan2(dy, dx)) + offset_deg) % 360.0
        k = np.floor(a / 360.0 * n).astype(int) % n
        out = base[k]
        if r_in is not None:
            t = (np.sqrt(dx * dx + dy * dy) - r_in) / max(1e-6, r_out - r_in)
            out = np.where((t < 0.30)[:, None], light[k], np.where((t > 0.72)[:, None], dark[k], out))
        return out
    return fn


def flat(c):
    return mt.flat(G(c))


def stars(rgba, allowed, N, seed, density, cols, big=True):
    """Deterministic starfield on pixels where `allowed` is True."""
    rng = np.random.RandomState(seed)
    H, W = allowed.shape
    count = int(W * H * density)
    placed = []
    for _ in range(count * 6):
        if len(placed) >= count:
            break
        x, y = rng.randint(1, W - 1), rng.randint(1, H - 1)
        if not allowed[y - 1:y + 2, x - 1:x + 2].all():
            continue
        if any(abs(x - a) + abs(y - b) < 4 for a, b in placed):
            continue
        placed.append((x, y))
        r = rng.rand()
        if big and r < 0.10 and allowed[max(0, y - 2):y + 3, max(0, x - 2):x + 3].all():
            px.stamp(rgba, "cross", x, y, {"#": cols[0], "+": cols[2]})
        elif r < 0.45:
            rgba[y, x, :3] = cols[1]
            rgba[y, x, 3] = 255
        else:
            rgba[y, x, :3] = cols[2]
            rgba[y, x, 3] = 255


# ---------------------------------------------------------------------------
# colours
# ---------------------------------------------------------------------------
def palette(scheme):
    H = px.hexrgb
    return dict(
        outline=scheme["outline"],
        space=[H("#0e0830"), H("#150b3e"), H("#1d0f4e"), H("#27125e"), H("#33166b")],
        halo=[H("#6a4fe0"), H("#4a2fb0")],
        ring=scheme.get("ring", [H("#6fe3ff"), H("#58a6ff"), H("#7c6dff"), H("#a957f5"),
                                 H("#d64ad8"), H("#ff5fae"), H("#ff9a3c"), H("#ffd75a"),
                                 H("#7ff0c0")]),
        gold=scheme["gold"],
        lid=[H("#ff7fd0"), H("#f160c8"), H("#d24fd6"), H("#a957f5"), H("#8a5cff")],
        void=[H("#1a0c4a"), H("#120838"), H("#0c052a")],
        rift_core=H("#ffffff"), rift_up=scheme["prism"][2], rift_dn=scheme["prism"][6],
        medal=[H("#2b1570"), H("#1d0e52")],
        mono=scheme["top_fill"],
        star=[H("#ffffff"), H("#d9e6ff"), H("#8f86e8")],
        extrude=scheme["extrude"],
    )


# ---------------------------------------------------------------------------
# emblem
# ---------------------------------------------------------------------------
def emblem(N, scheme, letter="M", margin=None, rounded=True, bg=True, seed=7):
    """Render the emblem natively at N x N pixels. Returns RGBA uint8."""
    P = palette(scheme)
    sc = Scene(N, N, ss=8 if N <= 64 else 4)
    if margin is None:
        margin = round(N * 0.07) if rounded else 0
    c = N / 2.0
    o = 1.0                                   # outline width (px)
    small = N < 32
    # geometry (fractions of N)
    tile_r = N * 0.2 if rounded else 0.0
    R1 = N * (0.40 if not rounded else 0.37)  # ring outer radius
    ring_w = max(2.0, N * 0.085)
    R2 = R1 - ring_w                          # ring inner radius
    gw = 1.0 if N >= 24 else 0.0              # gold rim width
    eq = max(1.0, round(N * 0.06))            # rift core thickness
    Rm = max(2.0, N * 0.15)                   # medallion radius

    L = {}
    L["tile_out"] = sc.layer(flat(P["outline"]))
    L["tile"] = sc.layer(vgrad(P["space"], margin, N - margin))
    L["halo2"] = sc.layer(flat(P["halo"][1]))
    L["halo1"] = sc.layer(flat(P["halo"][0]))
    L["r_out"] = sc.layer(flat(P["outline"]))
    L["r_gold"] = sc.layer(vgrad(P["gold"], c - R1, c + R1))
    L["ring"] = sc.layer(conic(P["ring"], c, c, offset_deg=200,
                               steps=max(len(P["ring"]), int(2 * math.pi * R1 / 4)),
                               r_in=(R2 + gw) if N >= 48 else None, r_out=R1 - o - gw))
    L["r_in"] = sc.layer(flat(P["outline"]))
    L["lid"] = sc.layer(vgrad(P["lid"], c - R2, c))
    L["void"] = sc.layer(vgrad(P["void"], c, c + R2))
    L["eq_out"] = sc.layer(flat(P["outline"]))
    L["eq_up"] = sc.layer(flat(P["rift_up"]))
    L["eq_dn"] = sc.layer(flat(P["rift_dn"]))
    L["eq_core"] = sc.layer(flat(P["rift_core"]))
    L["m_out"] = sc.layer(flat(P["outline"]))
    L["m_gold"] = sc.layer(vgrad(P["gold"], c - Rm, c + Rm))
    L["m_in"] = sc.layer(flat(P["outline"]))
    L["medal"] = sc.layer(vgrad(P["medal"], c - Rm, c + Rm))

    if bg:
        sc.paint(sc.roundbox(margin, margin, N - margin, N - margin, tile_r), L["tile_out"])
        sc.paint(sc.roundbox(margin + o, margin + o, N - margin - o, N - margin - o,
                             max(0.0, tile_r - o)), L["tile"])
        if N >= 32:
            if N >= 64:
                sc.paint(sc.ring(c, c, R1, R1 + 2.0), L["halo2"])
            sc.paint(sc.ring(c, c, R1, R1 + 1.0), L["halo1"])
    sc.paint(sc.circle(c, c, R1), L["r_out"])
    sc.paint(sc.circle(c, c, R1 - o), L["r_gold"] if gw else L["ring"])
    sc.paint(sc.circle(c, c, R1 - o - gw), L["ring"])
    sc.paint(sc.circle(c, c, R2 + gw), L["r_gold"] if gw else L["r_in"])
    sc.paint(sc.circle(c, c, R2), L["r_in"])
    disc = sc.circle(c, c, R2 - o)
    sc.paint(disc & (sc.Y < c), L["lid"])
    sc.paint(disc & (sc.Y >= c), L["void"])
    # equator: a thin glowing rift across the disc (flare tails added later)
    core_t = float(max(1, round(N / 40.0)))
    half = core_t / 2.0
    cy_eq = c if core_t % 2 == 0 else math.floor(c) + 0.5    # keep the core on whole pixel rows
    across = np.abs(sc.X - c) <= R1 - o
    if not small:
        sc.paint(disc & sc.hband(cy_eq - half - 2 * o, cy_eq + half + 2 * o), L["eq_out"])
        sc.paint(disc & sc.hband(cy_eq - half - o, cy_eq - half), L["eq_up"])
        sc.paint(disc & sc.hband(cy_eq + half, cy_eq + half + o), L["eq_dn"])
    sc.paint(disc & sc.hband(cy_eq - half, cy_eq + half), L["eq_core"])
    # medallion
    sc.paint(sc.circle(c, c, Rm), L["m_out"])
    if N >= 32:
        sc.paint(sc.circle(c, c, Rm - o), L["m_gold"])
        sc.paint(sc.circle(c, c, Rm - o - max(1.0, N * 0.025)), L["m_in"])
        sc.paint(sc.circle(c, c, Rm - 2 * o - max(1.0, N * 0.025)), L["medal"])
    else:
        sc.paint(sc.circle(c, c, Rm - o), L["m_gold"])

    dom, sec = sc.resolve(cleanup=False)
    rgba = sc.render(dom, sec)
    # bevel light on the gold rims (top-facing pixels)
    up = np.full_like(dom, px.TRANSPARENT)
    up[1:] = dom[:-1]
    for lid in (L["r_gold"], L["m_gold"]):
        m = (dom == lid) & (up != lid) & (np.arange(N)[:, None] < c)
        rgba[m, :3] = G(P["gold"][0])
    # glossy highlight arc in the upper half (top-left), N >= 48
    if N >= 48:
        yy, xx = np.mgrid[0:N, 0:N]
        d = np.sqrt((xx + 0.5 - c) ** 2 + (yy + 0.5 - c) ** 2)
        ang = np.degrees(np.arctan2(yy + 0.5 - c, xx + 0.5 - c)) % 360
        r_g = R2 - o - (2.5 if N >= 64 else 2.0)
        m = (dom == L["lid"]) & (np.abs(d - r_g) <= 0.55) & (ang > 200) & (ang < 255)
        rgba[m, :3] = G(px.hexrgb("#ffd0f0"))
    # rift flare tails: the core line continues past the ring, tapering
    if N >= 32:
        row = int(math.floor(cy_eq - half))
        rows = list(range(row, row + int(core_t)))
        tail = [P["rift_core"], P["rift_core"], scheme["prism"][1], scheme["prism"][2],
                scheme["prism"][3], scheme["prism"][4]]
        x_ring = int(math.ceil(c + R1))
        lim = N - margin - 2
        for side in (1, -1):
            for k in range(0, lim - x_ring + 1):
                x = x_ring + k if side == 1 else N - 1 - (x_ring + k)
                col = tail[min(len(tail) - 1, int(k / max(1, lim - x_ring + 1) * len(tail)))]
                for r_ in rows[:1] if k > (lim - x_ring) * 0.5 else rows:
                    if 0 <= x < N and rgba[r_, x, 3] > 0:
                        rgba[r_, x, :3] = G(col)
    rgba = px.snap_gba(rgba)

    # starfield on the tile background and inside the void
    if bg and N >= 24:
        stars(rgba, dom == L["tile"], N, seed, 0.012 if N <= 64 else 0.008,
              [G(c_) for c_ in P["star"]], big=N >= 48)
    if N >= 32:
        stars(rgba, dom == L["void"], N, seed + 1, 0.03 if N < 96 else 0.012,
              [G(c_) for c_ in P["star"]], big=N >= 96)

    # monogram
    if N >= 32 and letter:
        mono(rgba, letter, c, Rm - 2 * o - max(1.0, N * 0.025), scheme, P)
    elif letter:
        cx = int(c)
        rgba[cx - 1:cx + 1, cx - 1:cx + 1, :3] = G(P["rift_core"])
    # sparkle on the ring (top-left) - the "diamond ring" glint
    if N >= 48:
        a = math.radians(225)
        sx, sy = c + (R1 - ring_w / 2) * math.cos(a), c + (R1 - ring_w / 2) * math.sin(a)
        px.stamp(rgba, "star" if N >= 64 else "small", int(sx), int(sy),
                 {"#": G(P["star"][0]), "+": G(P["star"][1]), "*": G(P["rift_up"]),
                  ".": G(P["halo"][0])}, only_on=rgba[..., 3] > 0)
    return rgba


def mono(rgba, letter, c, r_inner, scheme, P):
    """Gold initial centred in the medallion (inner radius r_inner)."""
    N = rgba.shape[0]
    cap = max(5, int(r_inner * 1.25))
    sc = mt.Canvas(N, N, ss=6)
    f = px.font_for_cap(mt.FONT_WORD, cap)
    run = px.TextRun(letter, f)
    # vertical centre: cap centred on c
    top_y = int(round(c - cap / 2.0)) - 1
    w = mt.Word(sc, letter, f, 0, c, top_y, 1, 0, 0, 0, close=0)
    ids = {}
    ids["in"] = sc.layer(mt.flat(G(P["outline"])))
    ids["fill"] = sc.layer(mt.band_colour([G(x) for x in P["mono"]], w.cap_top, w.baseline))
    sc.paint(w.core & ~w.glyph, ids["in"])
    sc.paint(w.glyph, ids["fill"])
    dom, sec = sc.resolve(cleanup=False)
    out = sc.render(dom, sec)
    mt.add_bevel(out, dom, ids["fill"], G(P["mono"][0]))
    out = px.snap_gba(out)
    m = out[..., 3] > 0
    rgba[m] = out[m]


# ---------------------------------------------------------------------------
# cover / banner scenes
# ---------------------------------------------------------------------------
def nebula(W, H, seed, cols):
    """Pixel-art nebula: smooth value noise quantised into colour bands."""
    rng = np.random.RandomState(seed)
    acc = np.zeros((H, W), np.float32)
    amp = 1.0
    for cell in (64, 32, 16, 8):
        gh, gw = H // cell + 2, W // cell + 2
        g = rng.rand(gh, gw).astype(np.float32)
        im = Image.fromarray((g * 255).astype(np.uint8)).resize((gw * cell, gh * cell), Image.BICUBIC)
        acc += amp * (np.asarray(im, np.float32)[:H, :W] / 255.0)
        amp *= 0.5
    acc /= acc.max()
    # diagonal bias: brighter towards the lower right
    yy, xx = np.mgrid[0:H, 0:W]
    acc = 0.65 * acc + 0.35 * ((xx / W + yy / H) / 2.0)
    k = np.clip((acc - 0.35) / 0.55 * len(cols), 0, len(cols) - 1).astype(int)
    # ordered (Bayer 2x2) dithering on band edges for a classic pixel-art look
    bayer = np.array([[0.0, 0.5], [0.75, 0.25]])
    frac = (acc - 0.35) / 0.55 * len(cols) - k
    k2 = np.where(frac > 0.5 + bayer[yy % 2, xx % 2] * 0.5, np.minimum(k + 1, len(cols) - 1), k)
    out = np.zeros((H, W, 4), np.uint8)
    out[..., :3] = np.asarray([G(c) for c in cols], np.uint8)[k2]
    out[..., 3] = 255
    return out


def portal(W, H, cx, cy, R1, ring_w, scheme, hue=0.0, vortex=True, ss=4):
    """A standalone portal ring with a spiral vortex inside (RGBA)."""
    P = palette(scheme)
    sc = Scene(W, H, ss=ss)
    R2 = R1 - ring_w
    ring_cols = [px.hue_shift(c, hue) for c in P["ring"]]
    L = {}
    L["halo2"] = sc.layer(flat(P["halo"][1]))
    L["halo1"] = sc.layer(flat(P["halo"][0]))
    L["out"] = sc.layer(flat(P["outline"]))
    L["gold"] = sc.layer(vgrad(P["gold"], cy - R1, cy + R1))
    L["ring"] = sc.layer(conic(ring_cols, cx, cy, offset_deg=200,
                               steps=max(len(ring_cols), int(2 * math.pi * R1 / 5)),
                               r_in=R2 + 1, r_out=R1 - 2) if R1 >= 24 else
                         conic(ring_cols, cx, cy, offset_deg=200,
                               steps=max(len(ring_cols), int(2 * math.pi * R1 / 4))))
    L["in"] = sc.layer(flat(P["outline"]))
    vort = [px.hue_shift(c, hue) for c in (px.hexrgb("#0c052a"), px.hexrgb("#1d0e52"),
                                           px.hexrgb("#3a1d80"), px.hexrgb("#6a3fd0"),
                                           px.hexrgb("#b07cff"), px.hexrgb("#f0d8ff"))]
    vc = np.asarray([G(c) for c in vort], np.float32)

    def vortex_fn(ys, xs):
        dx, dy = xs + 0.5 - cx, ys + 0.5 - cy
        r = np.sqrt(dx * dx + dy * dy) / max(1.0, R2)
        a = np.arctan2(dy, dx)
        spiral = (np.sin(3 * a + 9.0 * np.log(r + 0.08)) + 1) / 2   # 3 arms
        t = 1.0 - r                                                  # brighter centre
        v = np.clip(0.55 * t + 0.45 * spiral * (0.4 + 0.6 * t), 0, 0.999)
        k = (v * len(vc)).astype(int)
        return vc[k]
    L["vortex"] = sc.layer(vortex_fn if vortex else flat(P["void"][0]))
    hw = max(1.0, R1 * 0.06)
    sc.paint(sc.ring(cx, cy, R1, R1 + hw * 2), L["halo2"])
    sc.paint(sc.ring(cx, cy, R1, R1 + hw), L["halo1"])
    sc.paint(sc.circle(cx, cy, R1), L["out"])
    sc.paint(sc.circle(cx, cy, R1 - 1), L["gold"])
    sc.paint(sc.circle(cx, cy, R1 - 2), L["ring"])
    sc.paint(sc.circle(cx, cy, R2 + 1), L["gold"])
    sc.paint(sc.circle(cx, cy, R2), L["in"])
    sc.paint(sc.circle(cx, cy, R2 - 1), L["vortex"])
    dom, sec = sc.resolve(cleanup=False)
    rgba = sc.render(dom, sec)
    up = np.full_like(dom, px.TRANSPARENT)
    up[1:] = dom[:-1]
    m = (dom == L["gold"]) & (up != L["gold"]) & (np.arange(H)[:, None] < cy)
    rgba[m, :3] = G(P["gold"][0])
    return px.snap_gba(rgba), dom == L["vortex"]


def over(dst, src, x=0, y=0):
    h, w = src.shape[:2]
    H, W = dst.shape[:2]
    x0, y0 = max(0, x), max(0, y)
    x1, y1 = min(W, x + w), min(H, y + h)
    if x1 <= x0 or y1 <= y0:
        return
    s = src[y0 - y:y1 - y, x0 - x:x1 - x]
    m = s[..., 3] > 0
    d = dst[y0:y1, x0:x1]
    d[m] = s[m]


def flare(rgba, cx, cy, half, scheme, vertical=7, big=False):
    """Horizontal prismatic flare with a 4-point star (same as the title).

    big=True doubles the core line and adds diagonal glints (cover art)."""
    P = palette(scheme)
    white, pale, cyan = G((255, 255, 255)), G(scheme["prism"][1]), G(scheme["prism"][2])
    blue, peri, violet = G(scheme["prism"][3]), G(scheme["prism"][4]), G(scheme["prism"][5])
    mag, deep = G(scheme["prism"][6]), G(scheme["extrude"][0])
    H, W = rgba.shape[:2]

    def put(x, y, c):
        if 0 <= x < W and 0 <= y < H:
            rgba[y, x, :3] = c
            rgba[y, x, 3] = 255
    ramp = [white] * 3 + [pale] * 3 + [cyan] * 3 + [blue] * 2 + [peri] * 2 + [violet, deep]
    for x in range(cx - half, cx + half + 1):
        d = abs(x - cx) / half
        put(x, cy, ramp[min(len(ramp) - 1, int(d * len(ramp)))])
        if d < 0.55:
            put(x, cy - 1, cyan if d < 0.25 else (blue if d < 0.42 else peri))
            put(x, cy + 1, mag if d < 0.25 else (violet if d < 0.42 else deep))
        if d < 0.16:
            put(x, cy - 2, peri)
            put(x, cy + 2, deep)
    ray = [white, white, pale, pale, cyan, blue, peri, violet][:vertical + 1]
    for k, c in enumerate(ray, start=1):
        put(cx, cy - k, c)
        put(cx, cy + k, c)
    for k in (-1, 1):
        put(cx + k, cy - 1, white)
        put(cx + k, cy + 1, white)
        put(cx + 2 * k, cy, white)
    for dx, dy in ((2, 2), (-2, 2), (2, -2), (-2, -2)):
        put(cx + dx, cy + dy, G(scheme["gold"][2]))
    if big:
        # second core row + longer vertical ray + a soft diamond around the star
        for x in range(cx - int(half * 0.6), cx + int(half * 0.6) + 1):
            d = abs(x - cx) / (half * 0.6)
            put(x, cy + 1, white if d < 0.3 else (pale if d < 0.6 else cyan))
        put(cx, cy + 2, white)
        for k, c in enumerate([white, pale, pale, cyan, cyan, blue, blue, peri, violet], start=vertical + 2):
            put(cx, cy - k, c)
            put(cx, cy + 1 + k, c)
        for dx, dy in ((1, -1), (-1, -1), (1, 2), (-1, 2), (2, 0), (-2, 0), (2, 1), (-2, 1)):
            put(cx + dx, cy + dy, white)
        for dx, dy in ((3, -2), (-3, -2), (3, 3), (-3, 3)):
            put(cx + dx, cy + dy, G(scheme["gold"][1]))
        for dx, dy in ((4, -3), (-4, -3), (4, 4), (-4, 4)):
            put(cx + dx, cy + dy, G(scheme["gold"][3]))


def cover(scheme, word, top_text, size=256, seed=3):
    """Box-art style cover, native size x size (upscaled 2x by the caller)."""
    P = palette(scheme)
    W = H = size
    img = nebula(W, H, seed, [px.hexrgb("#0b0626"), px.hexrgb("#120a36"), px.hexrgb("#1b0e4a"),
                              px.hexrgb("#271360"), px.hexrgb("#3a1a7a"), px.hexrgb("#55208c")])
    bgmask = np.ones((H, W), bool)
    # distant mini portals (other universes), drawn first
    minis = [(34, 126, 14, 40.0), (224, 138, 12, -70.0), (210, 222, 16, 150.0), (42, 220, 11, -140.0)]
    for (x, y, r, hue) in minis:
        p, _ = portal(W, H, x, y, r, max(3, r * 0.3), scheme, hue=hue, ss=4)
        over(img, p)
    # main portal
    pcy, pr = int(H * 0.63), int(W * 0.285)
    p, vort = portal(W, H, W // 2, pcy, pr, int(W * 0.06), scheme)
    over(img, p)
    # stars on the nebula only (not on portals)
    op_portals = np.zeros((H, W), bool)
    for (x, y, r, hue) in minis:
        yy, xx = np.mgrid[0:H, 0:W]
        op_portals |= (xx - x) ** 2 + (yy - y) ** 2 <= (r + 4) ** 2
    yy, xx = np.mgrid[0:H, 0:W]
    op_portals |= (xx - W // 2) ** 2 + (yy - pcy) ** 2 <= (pr + 8) ** 2
    stars(img, ~op_portals, W, seed + 5, 0.010, [G(c) for c in P["star"]], big=True)
    # rift flare across the main portal
    flare(img, W // 2, pcy, int(W * 0.44), scheme, big=True)
    # logo
    logo, info = mt.render_logo(word, top_text, scheme, W=W, H=96, cx=W // 2, max_w=W - 18,
                                word_cap=31, top_cap=22, top_y=2, bottom=92, scale=1.4, overlap=7)
    over(img, logo, 0, 10)
    # frame: dark + gold double border
    fr = np.zeros((H, W), bool)
    fr[:2] = fr[-2:] = True
    fr[:, :2] = fr[:, -2:] = True
    img[fr, :3] = G(P["outline"])
    g = np.zeros((H, W), bool)
    g[2, 2:-2] = g[-3, 2:-2] = True
    g[2:-2, 2] = g[2:-2, -3] = True
    img[g, :3] = G(P["gold"][2])
    img[2, 2:-2, :3] = G(P["gold"][0])
    return img


def banner_3ds(scheme, word, top_text, seed=11):
    """256x128 banner image for 3DS injectors (content kept in the centre)."""
    P = palette(scheme)
    W, H = 256, 128
    img = nebula(W, H, seed, [px.hexrgb("#0b0626"), px.hexrgb("#120a36"), px.hexrgb("#1b0e4a"),
                              px.hexrgb("#271360"), px.hexrgb("#3a1a7a")])
    p, _ = portal(W, H, W // 2, 70, 58, 10, scheme)
    over(img, p)
    yy, xx = np.mgrid[0:H, 0:W]
    away = (xx - W // 2) ** 2 + (yy - 70) ** 2 > 70 ** 2
    stars(img, away, W, seed, 0.012, [G(c) for c in P["star"]], big=True)
    logo, info = mt.render_logo(word, top_text, scheme, W=W, H=88, cx=W // 2, max_w=W - 24,
                                word_cap=29, top_cap=20, top_y=1, bottom=85, scale=1.3, overlap=7)
    over(img, logo, 0, 14)
    flare(img, W // 2, 110, 96, scheme, vertical=5)
    return img


# ---------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--word", default="MULTIVERSE")
    ap.add_argument("--top", default="POKÉMON")
    ap.add_argument("--letter", default=None, help="monogram letter (default: first letter of --word)")
    ap.add_argument("--scheme", default="cosmic", choices=sorted(mt.SCHEMES))
    ap.add_argument("--color", action="append", default=[], metavar="KEY=#RRGGBB[,..]")
    ap.add_argument("--out", default="./icon_out")
    args = ap.parse_args()
    scheme = mt.apply_overrides(mt.SCHEMES[args.scheme], args.color)
    letter = (args.letter or args.word[:1]).upper()
    os.makedirs(args.out, exist_ok=True)
    out = {}

    def save(name, rgba, scale=1, opaque=False):
        im = Image.fromarray(rgba, "RGBA")
        if scale != 1:
            im = im.resize((im.width * scale, im.height * scale), Image.NEAREST)
        if opaque:
            assert rgba[..., 3].min() == 255, name + " must be fully opaque"
            im.convert("RGB").save(os.path.join(args.out, name))
        else:
            im.save(os.path.join(args.out, name))
        out[name] = im
        return im

    # macOS / generic icon: native pixel art at 16/32/64/128, larger sizes
    # are integer upscales of the 128px master (crisp pixels)
    master = emblem(128, scheme, letter)
    for n in (16, 32, 64):
        save(f"icon_{n}.png", emblem(n, scheme, letter))
    save("icon_128.png", master)
    for n in (256, 512, 1024):
        save(f"icon_{n}.png", master, n // 128)
    icns_imgs = [out[f"icon_{n}.png"] for n in (16, 32, 64, 128, 256, 512, 1024)]
    out["icon_1024.png"].save(os.path.join(args.out, "icon.icns"), append_images=icns_imgs[:-1])
    # 3DS (SMDH): opaque, full bleed, no rounded transparency
    for n in (48, 24):
        e = emblem(n, scheme, letter, margin=0, rounded=False)
        e[..., 3] = 255
        save(f"3ds_icon_{n}.png", e, opaque=True)
    save("3ds_banner_256x128.png", banner_3ds(scheme, args.word, args.top), opaque=True)
    save("cover_512.png", cover(scheme, args.word, args.top), 2, opaque=True)

    # preview sheet
    sheet = Image.new("RGB", (1024 + 512 + 48, 1024 + 40), (34, 34, 40))
    sheet.paste(out["icon_1024.png"], (0, 0), out["icon_1024.png"])
    sheet.paste(out["cover_512.png"].convert("RGB"), (1024 + 24, 0))
    b = out["3ds_banner_256x128.png"].resize((512, 256), Image.NEAREST)
    sheet.paste(b.convert("RGB"), (1024 + 24, 512 + 24))
    x = 1024 + 24
    y = 512 + 24 + 256 + 24
    for name, k in (("3ds_icon_48.png", 3), ("3ds_icon_24.png", 3), ("icon_32.png", 3), ("icon_16.png", 3)):
        im = out[name].resize((out[name].width * k, out[name].height * k), Image.NEAREST)
        sheet.paste(im, (x, y), im)
        x += im.width + 16
    sheet.save(os.path.join(args.out, "preview_sheet.png"))
    chk = Image.open(os.path.join(args.out, "icon.icns"))
    print("icns entries (w, h, scale):", sorted(chk.info.get("sizes", [])))
    print("wrote:", ", ".join(sorted(os.listdir(args.out))))


if __name__ == "__main__":
    main()
