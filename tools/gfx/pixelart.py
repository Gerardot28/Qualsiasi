"""Shared pixel-art helpers for the title-screen / icon generators.

Only needs Python 3 + Pillow + numpy.

Pipeline used for every wordmark:
  1. text is rendered at 1x with FreeType hinting (stems snap to the pixel
     grid) as an 8-bit coverage map, letter by letter with integer advances;
  2. the coverage map is upsampled (bilinear, SS x) and thresholded, giving a
     smooth high-resolution silhouette whose straight edges stay on 1x pixel
     boundaries;
  3. outline bands, extrusion etc. are derived from a (bounded) Euclidean
     distance field at high resolution and painted into a layer-id map;
  4. the id map is box-downsampled back to 1x: a pixel gets the colour of its
     dominant layer, and only pixels where two *opaque* layers are close to
     50/50 get one intermediate blend colour (selective anti-aliasing).
     The outer silhouette is always hard (the GBA has no per-pixel alpha);
  5. a cleanup pass removes isolated / one-pixel spur pixels.
"""
import colorsys
import os

import numpy as np
from PIL import Image, ImageDraw, ImageFont

HERE = os.path.dirname(os.path.abspath(__file__))
FONT_DIR = os.path.join(HERE, "fonts")

TRANSPARENT = -1


# --------------------------------------------------------------------------
# colours
# --------------------------------------------------------------------------
def hexrgb(s):
    s = s.lstrip("#")
    return tuple(int(s[i:i + 2], 16) for i in (0, 2, 4))


def gba(c):
    """Snap an 8-bit RGB colour to the GBA's 15-bit grid.

    The returned 8-bit value v satisfies v >> 3 == 5-bit value, which is what
    gbagfx does when it writes .gbapal, and (v5 << 3 | v5 >> 2) is how mGBA
    expands it back, so previews match the hardware exactly."""
    out = []
    for v in c[:3]:
        v5 = min(31, (int(round(v)) + 4) >> 3)
        out.append((v5 << 3) | (v5 >> 2))
    return tuple(out)


def mix(a, b, t):
    return tuple(a[i] + (b[i] - a[i]) * t for i in range(3))


def hue_shift(c, deg, sat=1.0, light=0.0):
    h, l, s = colorsys.rgb_to_hls(*(v / 255 for v in c))
    h = (h + deg / 360.0) % 1.0
    s = max(0.0, min(1.0, s * sat))
    l = max(0.0, min(1.0, l + light))
    return tuple(v * 255 for v in colorsys.hls_to_rgb(h, l, s))


def ramp(stops, n):
    """n colours evenly interpolated through the list of stop colours."""
    if n == 1:
        return [tuple(stops[0])]
    out = []
    for i in range(n):
        t = i / (n - 1) * (len(stops) - 1)
        k = min(int(t), len(stops) - 2)
        out.append(mix(stops[k], stops[k + 1], t - k))
    return out


# --------------------------------------------------------------------------
# text
# --------------------------------------------------------------------------
def load_font(name, size):
    path = name if os.path.isabs(name) else os.path.join(FONT_DIR, name)
    return ImageFont.truetype(path, size)


def cap_height(font):
    bb = font.getbbox("H", anchor="ls")
    return -bb[1]


def font_for_cap(name, cap):
    """Largest font size whose 'H' is at most `cap` pixels tall."""
    best = None
    for size in range(6, 200):
        f = load_font(name, size)
        if cap_height(f) <= cap:
            best = f
        else:
            break
    return best


class TextRun:
    """A line of text rendered letter by letter on integer pixel positions."""

    def __init__(self, text, font, tracking=0, kern=True, adjust=None, custom_accents=False):
        self.custom_accents = custom_accents
        self.accents = {}
        if custom_accents:
            # draw accents ourselves (bolder than the font's at small sizes)
            base = {"É": "E", "È": "E", "À": "A", "Ì": "I", "Ò": "O", "Ù": "U", "Á": "A", "Í": "I",
                    "Ó": "O", "Ú": "U"}
            plain = []
            for i, ch in enumerate(text):
                if ch in base:
                    self.accents[i] = "acute" if ch in "ÉÁÍÓÚ" else "grave"
                    plain.append(base[ch])
                else:
                    plain.append(ch)
            text = "".join(plain)
        self.text = text
        self.font = font
        self.cap = cap_height(font)
        xs = []
        x = 0.0
        for i, ch in enumerate(text):
            xs.append(int(round(x)))
            adv = font.getlength(ch) + tracking
            if kern and i + 1 < len(text):
                nxt = text[i + 1]
                adv += font.getlength(ch + nxt) - font.getlength(ch) - font.getlength(nxt)
            if adjust and i in adjust:
                adv += adjust[i]
            x += adv
        self.xs = xs
        # ink extents
        boxes = []
        for ch, x0 in zip(text, xs):
            bb = font.getbbox(ch, anchor="ls")
            boxes.append((x0 + bb[0], bb[1], x0 + bb[2], bb[3]))
        self.cap = cap_height(font)
        acc_h = max(4, int(round(self.cap * 0.36)))
        self.acc_h = acc_h
        for i in self.accents:
            x0, y0, x1, y1 = boxes[i]
            boxes[i] = (x0, -self.cap - acc_h - 1, x1, y1)
        self.ink = (min(b[0] for b in boxes), min(b[1] for b in boxes),
                    max(b[2] for b in boxes), max(b[3] for b in boxes))
        self.boxes = boxes

    @property
    def width(self):
        return self.ink[2] - self.ink[0]

    def _accent(self, W, H, i, dx, baseline):
        """A slanted bar above letter i (supersampled polygon)."""
        k = 8
        x0, _, x1, _ = self.boxes[i]
        cxl = (x0 + x1) / 2.0 + dx
        top = baseline - self.cap - 1.0          # 1px gap above the cap height
        h = self.acc_h
        t = max(2.5, self.cap * 0.22)            # stroke thickness
        lean = 1 if self.accents[i] == "acute" else -1
        # parallelogram: bottom edge centred slightly left, top edge to the right
        bx = cxl - lean * h * 0.35
        tx = cxl + lean * h * 0.55
        pts = [(bx - t / 2, top), (bx + t / 2, top), (tx + t / 2, top - h), (tx - t / 2, top - h)]
        im = Image.new("L", (W * k, H * k), 0)
        ImageDraw.Draw(im).polygon([(x * k, y * k) for x, y in pts], fill=255)
        return np.asarray(im.resize((W, H), Image.BOX), dtype=np.float32) / 255.0

    def coverage(self, W, H, ox, baseline):
        """Return (union coverage float32 HxW, list of per-letter coverages).

        ox: x of the left ink edge; baseline: y of the baseline."""
        letters = []
        dx = ox - self.ink[0]
        for ch, x0 in zip(self.text, self.xs):
            im = Image.new("L", (W, H), 0)
            d = ImageDraw.Draw(im)
            d.fontmode = "L"
            d.text((x0 + dx, baseline), ch, font=self.font, anchor="ls", fill=255)
            cov = np.asarray(im, dtype=np.float32) / 255.0
            i = len(letters)
            if i in self.accents:
                cov = np.maximum(cov, self._accent(W, H, i, dx, baseline))
            letters.append(cov)
        union = np.max(np.stack(letters), axis=0) if letters else np.zeros((H, W), np.float32)
        return union, letters


# --------------------------------------------------------------------------
# high-resolution masks and distance fields
# --------------------------------------------------------------------------
def upsample_mask(cov, ss):
    """1x coverage -> SSx boolean mask (bilinear, threshold 0.5)."""
    H, W = cov.shape
    im = Image.fromarray(np.clip(cov * 255, 0, 255).astype(np.uint8), "L")
    # pad by one pixel so edge interpolation is symmetric
    big = im.resize((W * ss, H * ss), Image.BILINEAR)
    return np.asarray(big) >= 128


def _shift(a, dy, dx, fill):
    out = np.full_like(a, fill)
    H, W = a.shape
    ys = slice(max(dy, 0), H + min(dy, 0))
    yd = slice(max(-dy, 0), H + min(-dy, 0))
    xs = slice(max(dx, 0), W + min(dx, 0))
    xd = slice(max(-dx, 0), W + min(-dx, 0))
    out[yd, xd] = a[ys, xs]
    return out


def shift(a, dy, dx, fill=False):
    """out[y, x] = a[y - dy, x - dx] (moves content by +dy, +dx)."""
    return _shift(a, -dy, -dx, fill)


def distance_outside(mask, R):
    """Euclidean distance (in mask pixels) from each pixel to the nearest True
    pixel, exact up to R (larger distances are reported as R + 1)."""
    INF = np.float32((R + 1) ** 2)
    f = np.where(mask, np.float32(0), INF).astype(np.float32)
    g = np.full_like(f, INF)
    for d in range(-R, R + 1):
        np.minimum(g, _shift(f, 0, d, INF) + d * d, out=g)
    h = np.full_like(g, INF)
    for d in range(-R, R + 1):
        np.minimum(h, _shift(g, d, 0, INF) + d * d, out=h)
    return np.sqrt(h)


def fill_holes_hr(mask_hr, ss):
    """Fill enclosed holes of a high-res mask (holes found at 1x)."""
    Hh, Wh = mask_hr.shape
    H, W = Hh // ss, Wh // ss
    cov = mask_hr.reshape(H, ss, W, ss).mean(axis=(1, 3))
    solid = cov >= 0.5
    reach = np.zeros_like(solid)
    reach[0, :] = ~solid[0, :]
    reach[-1, :] = ~solid[-1, :]
    reach[:, 0] |= ~solid[:, 0]
    reach[:, -1] |= ~solid[:, -1]
    while True:
        grown = reach.copy()
        grown[1:] |= reach[:-1]
        grown[:-1] |= reach[1:]
        grown[:, 1:] |= reach[:, :-1]
        grown[:, :-1] |= reach[:, 1:]
        grown &= ~solid
        if (grown == reach).all():
            break
        reach = grown
    holes = ~solid & ~reach
    return mask_hr | np.kron(holes, np.ones((ss, ss), bool))


def dilate(mask, r_px, ss):
    """Disk dilation of a high-res mask by r_px (1x pixels)."""
    R = int(np.ceil(r_px * ss)) + 1
    return distance_outside(mask, R) <= r_px * ss


# --------------------------------------------------------------------------
# layer-id compositing with selective anti-aliasing
# --------------------------------------------------------------------------
def downsample_ids(idmap, ss, nlayers):
    """Per-1x-pixel coverage of every layer id (and of TRANSPARENT)."""
    Hh, Wh = idmap.shape
    H, W = Hh // ss, Wh // ss
    covs = np.zeros((nlayers + 1, H, W), np.float32)  # last = transparent
    for lid in range(nlayers):
        m = (idmap == lid).reshape(H, ss, W, ss).sum(axis=(1, 3))
        covs[lid] = m / float(ss * ss)
    covs[nlayers] = 1.0 - covs[:nlayers].sum(axis=0)
    return covs


def resolve(covs, aa_threshold=0.70, alpha_threshold=0.5):
    """Return (dominant id, secondary id or -1) per pixel.

    A pixel is transparent when the transparent coverage >= alpha_threshold.
    For opaque pixels the dominant opaque layer wins; when it covers less than
    aa_threshold of the opaque area and a second opaque layer covers at least
    (1 - aa_threshold), the pixel is flagged for a 50/50 blend."""
    n = covs.shape[0] - 1
    trans = covs[n]
    op = covs[:n]
    dom = np.argmax(op, axis=0)
    sorted_cov = np.sort(op, axis=0)
    best = sorted_cov[-1]
    second_cov = sorted_cov[-2] if n > 1 else np.zeros_like(best)
    op2 = op.copy()
    np.put_along_axis(op2, dom[None], -1, axis=0)
    sec = np.argmax(op2, axis=0)
    opaque_total = np.maximum(1e-6, 1.0 - trans)
    blend = (best / opaque_total < aa_threshold) & (second_cov / opaque_total >= 1 - aa_threshold)
    dom = np.where(trans >= alpha_threshold, TRANSPARENT, dom)
    sec = np.where(blend & (dom != TRANSPARENT), sec, -1)
    return dom, sec


def cleanup_alpha(dom, iterations=2):
    """Remove isolated opaque pixels / 1px spurs and fill 1px notches on the
    outer silhouette.  Filled notch pixels copy the most common neighbour id."""
    dom = dom.copy()
    for _ in range(iterations):
        op = dom != TRANSPARENT
        n4 = sum(shift(op, dy, dx) for dy, dx in ((1, 0), (-1, 0), (0, 1), (0, -1)))
        n8 = n4 + sum(shift(op, dy, dx) for dy, dx in ((1, 1), (1, -1), (-1, 1), (-1, -1)))
        spur = op & ((n8 <= 1) | ((n4 <= 1) & (n8 <= 2)))
        dom[spur] = TRANSPARENT
        op = dom != TRANSPARENT
        n4 = sum(shift(op, dy, dx).astype(np.int32) for dy, dx in ((1, 0), (-1, 0), (0, 1), (0, -1)))
        notch = (~op) & (n4 >= 3)
        if notch.any():
            ys, xs = np.nonzero(notch)
            H, W = dom.shape
            for y, x in zip(ys, xs):
                vals = [dom[y + dy, x + dx] for dy, dx in ((1, 0), (-1, 0), (0, 1), (0, -1))
                        if 0 <= y + dy < H and 0 <= x + dx < W and dom[y + dy, x + dx] != TRANSPARENT]
                dom[y, x] = max(set(vals), key=vals.count)
    return dom


def prune_blends(dom, sec):
    """Only keep an AA blend with layer L if a 4-neighbour actually shows L."""
    ok = np.zeros(dom.shape, bool)
    for dy, dx in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        nb = _shift(dom, dy, dx, TRANSPARENT)
        ok |= (nb == sec)
    sec = np.where(ok, sec, -1)
    return sec


def components(mask):
    """8-connected components of a 1x bool mask -> list of (ys, xs) arrays."""
    H, W = mask.shape
    seen = np.zeros_like(mask)
    comps = []
    for y0, x0 in zip(*np.nonzero(mask)):
        if seen[y0, x0]:
            continue
        stack = [(y0, x0)]
        seen[y0, x0] = True
        ys, xs = [], []
        while stack:
            y, x = stack.pop()
            ys.append(y)
            xs.append(x)
            for dy in (-1, 0, 1):
                for dx in (-1, 0, 1):
                    yy, xx = y + dy, x + dx
                    if 0 <= yy < H and 0 <= xx < W and mask[yy, xx] and not seen[yy, xx]:
                        seen[yy, xx] = True
                        stack.append((yy, xx))
        comps.append((np.array(ys), np.array(xs)))
    return comps


def drop_specks(dom, sec, lid, into, min_size):
    """Re-assign connected pieces of layer `lid` smaller than min_size pixels
    to layer `into` (kills isolated rim specks inside counters)."""
    for ys, xs in components(dom == lid):
        if len(ys) < min_size:
            dom[ys, xs] = into
            sec[ys, xs] = -1


def aa_blend(a, b):
    """Anti-aliasing colour between two layer colours (N,3 arrays).

    Plain RGB averaging of a bright and a dark colour gives muddy greys/olives
    (gold + indigo -> olive).  Instead keep the hue and saturation (HSV) of
    the brighter colour and average only the value, so an AA pixel looks like
    a deeper shade of the bright colour - the way pixel artists pick them."""
    out = (a + b) / 2.0
    for i in range(len(a)):
        ha, sa, va = colorsys.rgb_to_hsv(*(a[i] / 255.0))
        hb, sb, vb = colorsys.rgb_to_hsv(*(b[i] / 255.0))
        if abs(va - vb) < 0.3:
            continue
        h, s_ = (ha, sa) if va > vb else (hb, sb)
        out[i] = np.array(colorsys.hsv_to_rgb(h, s_, (va + vb) / 2.0)) * 255.0
    return out


def paint(dom, sec, colour_fns, H, W):
    """colour_fns[lid](ys, xs) -> (N,3) colours.  Returns RGBA uint8 image."""
    out = np.zeros((H, W, 4), np.float32)
    for lid, fn in enumerate(colour_fns):
        m = dom == lid
        if not m.any():
            continue
        ys, xs = np.nonzero(m)
        cols = np.asarray(fn(ys, xs), np.float32)
        sm = sec[ys, xs]
        bl = sm >= 0
        if bl.any():
            c2 = np.zeros_like(cols)
            for lid2 in np.unique(sm[bl]):
                k = bl & (sm == lid2)
                c2[k] = np.asarray(colour_fns[lid2](ys[k], xs[k]), np.float32)
            cols[bl] = aa_blend(cols[bl], c2[bl])
        out[ys, xs, :3] = cols
        out[ys, xs, 3] = 255
    return out


def snap_gba(rgba):
    """Quantise an RGBA float image to GBA colours (alpha is binary)."""
    a = rgba.copy()
    v5 = np.minimum(31, (np.round(a[..., :3]).astype(np.int32) + 4) >> 3)
    a[..., :3] = (v5 << 3) | (v5 >> 2)
    a[..., 3] = np.where(a[..., 3] >= 128, 255, 0)
    return a.astype(np.uint8)


# --------------------------------------------------------------------------
# palettes / indexed output
# --------------------------------------------------------------------------
def to_indexed(rgba, max_colors, first_index=1, order_key=None, reserved=None):
    """Convert RGBA (binary alpha) to (index array, palette list).

    Index 0 = transparent.  Opaque colours get indices first_index.. in
    ascending luminance (or order_key) order.  If there are more than
    max_colors distinct colours, they are reduced by Pillow's median cut
    (no dithering) on the opaque pixels only."""
    H, W, _ = rgba.shape
    op = rgba[..., 3] > 0
    cols = rgba[..., :3][op]
    uniq = np.unique(cols.reshape(-1, 3), axis=0) if len(cols) else np.zeros((0, 3), np.uint8)
    if len(uniq) > max_colors:
        im = Image.fromarray(cols.reshape(1, -1, 3).astype(np.uint8), "RGB")
        q = im.quantize(colors=max_colors, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
        qpal = np.array(q.getpalette()[:max_colors * 3]).reshape(-1, 3)
        qidx = np.asarray(q).reshape(-1)
        newcols = np.array([gba(c) for c in qpal[qidx]], np.uint8)
        rgba = rgba.copy()
        rgba[..., :3][op] = newcols
        cols = newcols
        uniq = np.unique(cols.reshape(-1, 3), axis=0)
    key = order_key or (lambda c: (0.299 * c[0] + 0.587 * c[1] + 0.114 * c[2], tuple(c)))
    uniq = sorted((tuple(int(v) for v in c) for c in uniq), key=key)
    lut = {c: i + first_index for i, c in enumerate(uniq)}
    idx = np.zeros((H, W), np.uint8)
    ys, xs = np.nonzero(op)
    for y, x in zip(ys, xs):
        idx[y, x] = lut[tuple(int(v) for v in rgba[y, x, :3])]
    return idx, uniq


def save_indexed_png(path, idx, palette, ncolors):
    """Save an indexed PNG whose PLTE has exactly `ncolors` entries.

    palette: list of RGB tuples for indices 0..len-1 (missing -> black)."""
    im = Image.fromarray(idx, "P")
    flat = []
    for i in range(ncolors):
        c = palette[i] if i < len(palette) else (0, 0, 0)
        flat.extend(int(v) for v in c)
    im.putpalette(flat, rawmode="RGB")
    bits = 8 if ncolors > 16 else 4
    im.save(path, optimize=False, bits=bits)
    # verify what the build will see
    chk = Image.open(path)
    assert chk.mode == "P" and len(chk.getpalette()) // 3 == ncolors, (path, len(chk.getpalette()) // 3)


def write_jasc(path, palette, n=256):
    with open(path, "w", newline="") as f:
        f.write("JASC-PAL\r\n0100\r\n%d\r\n" % n)
        for i in range(n):
            c = palette[i] if i < len(palette) else (0, 0, 0)
            f.write("%d %d %d\r\n" % tuple(int(v) for v in c))


def read_jasc(path):
    lines = open(path).read().split("\n")
    n = int(lines[2])
    return [tuple(int(v) for v in lines[3 + i].split()) for i in range(n)]


# --------------------------------------------------------------------------
# sparkles (hand-placed pixel stamps)
# --------------------------------------------------------------------------
# Characters: '#' core (white), '+' bright, '*' mid tint, '.' faint tint
SPARKLES = {
    "tiny": ["  .  ",
             " .+. ",
             ".+#+.",
             " .+. ",
             "  .  "],
    "small": ["   .   ",
              "   +   ",
              "  .#.  ",
              ".+###+.",
              "  .#.  ",
              "   +   ",
              "   .   "],
    "star": ["    .    ",
             "    *    ",
             "    +    ",
             "   .#.   ",
             ".*+###+*.",
             "   .#.   ",
             "    +    ",
             "    *    ",
             "    .    "],
    "big": ["     .     ",
            "     *     ",
            "     +     ",
            "     +     ",
            "    .#.    ",
            ".*++###++*.",
            "    .#.    ",
            "     +     ",
            "     +     ",
            "     *     ",
            "     .     "],
    "dot": ["#"],
    "cross": [" + ",
              "+#+",
              " + "],
}


def stamp(rgba, kind, cx, cy, colours, only_on=None):
    """Draw a sparkle centred on (cx, cy).

    colours: dict with '#', '+', '*', '.' -> RGB.  only_on: optional bool
    mask; faint pixels ('.' and '*') are only drawn where the mask is True so
    sparkles never create stray pixels in empty space (core/bright always)."""
    pat = SPARKLES[kind]
    h = len(pat)
    w = len(pat[0])
    H, W = rgba.shape[:2]
    for j, row in enumerate(pat):
        for i, ch in enumerate(row):
            if ch == " " or ch not in colours:
                continue
            x = cx - w // 2 + i
            y = cy - h // 2 + j
            if not (0 <= x < W and 0 <= y < H):
                continue
            if only_on is not None and ch in ".*" and not only_on[y, x]:
                continue
            rgba[y, x, :3] = colours[ch]
            rgba[y, x, 3] = 255
