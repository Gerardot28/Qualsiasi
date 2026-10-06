"""pexmap: core reader for pokeemerald-expansion map data.

Loads layouts (map.bin / border.bin), tilesets (tiles.png, JASC palettes,
metatiles.bin, metatile_attributes.bin) and map.json files, and renders
metatiles / layouts to Pillow images.

Everything is resolved from the C sources of the tree, so new tilesets added
to the tree are picked up automatically.
"""
import json
import os
import re
import struct
from functools import lru_cache

import numpy as np
from PIL import Image, ImageDraw, ImageFont

# --- engine constants (include/fieldmap.h) ---------------------------------
EM = dict(tiles_primary=512, metatiles_primary=512, pals_primary=6)
FRLG = dict(tiles_primary=640, metatiles_primary=640, pals_primary=7)
NUM_PALS_TOTAL = 13

LAYER_NORMAL, LAYER_COVERED, LAYER_SPLIT = 0, 1, 2


def blk(metatile, collision=0, elevation=0):
    """Pack a map.bin block value."""
    return (metatile & 0x3FF) | ((collision & 3) << 10) | ((elevation & 0xF) << 12)


def unblk(v):
    return v & 0x3FF, (v >> 10) & 3, (v >> 12) & 0xF


def read_jasc(path):
    with open(path) as f:
        lines = [l.strip() for l in f.read().splitlines() if l.strip()]
    if lines[0] != 'JASC-PAL':
        raise ValueError('not a JASC palette: %s' % path)
    n = int(lines[2])
    cols = []
    for l in lines[3:3 + n]:
        r, g, b = (int(x) for x in l.split()[:3])
        # GBA is 15-bit: quantise like the hardware does
        cols.append(((r >> 3) << 3, (g >> 3) << 3, (b >> 3) << 3))
    while len(cols) < 16:
        cols.append((0, 0, 0))
    return cols[:16]


class Tileset:
    """One primary or secondary tileset, as defined in the C sources."""

    def __init__(self, project, symbol, info):
        self.project = project
        self.symbol = symbol
        self.is_secondary = info['isSecondary']
        self.is_frlg = info['is_frlg']
        root = project.root
        self.tiles_path = os.path.join(root, info['tiles']) if info.get('tiles') else None
        self.pal_paths = [os.path.join(root, p) for p in info.get('palettes', [])]
        self.metatiles_path = os.path.join(root, info['metatiles'])
        self.attrs_path = os.path.join(root, info['attributes'])
        self.dir = os.path.dirname(self.metatiles_path)
        self._tiles = None
        self._pals = None
        self._mt = None
        self._attrs = None

    # lazily loaded data -----------------------------------------------------
    @property
    def tiles(self):
        """np.uint8 array [n, 8, 8] of 4bpp colour indices."""
        if self._tiles is None:
            if not self.tiles_path or not os.path.exists(self.tiles_path):
                self._tiles = np.zeros((0, 8, 8), np.uint8)
            else:
                im = Image.open(self.tiles_path)
                if im.mode != 'P':
                    im = im.convert('P')
                a = np.array(im, dtype=np.uint8) & 0xF
                h, w = a.shape
                a = a[:h - h % 8, :w - w % 8]
                ty, tx = a.shape[0] // 8, a.shape[1] // 8
                self._tiles = a.reshape(ty, 8, tx, 8).transpose(0, 2, 1, 3).reshape(ty * tx, 8, 8)
        return self._tiles

    @property
    def palettes(self):
        if self._pals is None:
            self._pals = [read_jasc(p) for p in self.pal_paths]
        return self._pals

    @property
    def metatiles(self):
        """np.uint16 array [n, 8] (4 bottom-layer tiles then 4 top-layer)."""
        if self._mt is None:
            data = open(self.metatiles_path, 'rb').read()
            a = np.frombuffer(data, dtype='<u2')
            self._mt = a[: len(a) // 8 * 8].reshape(-1, 8)
        return self._mt

    @property
    def attrs(self):
        """list of (behavior, layer_type) per metatile."""
        if self._attrs is None:
            data = open(self.attrs_path, 'rb').read()
            if self.is_frlg:
                vals = np.frombuffer(data, dtype='<u4')
                self._attrs = [(int(v) & 0x1FF, (int(v) >> 29) & 3) for v in vals]
            else:
                vals = np.frombuffer(data, dtype='<u2')
                self._attrs = [(int(v) & 0xFF, (int(v) >> 12) & 0xF) for v in vals]
        return self._attrs

    @property
    def num_metatiles(self):
        return len(self.metatiles)


class Layout:
    def __init__(self, project, entry):
        self.project = project
        self.entry = entry
        self.id = entry['id']
        self.name = entry['name']
        self.width = int(entry['width'])
        self.height = int(entry['height'])
        self.version = entry.get('layout_version', 'emerald')
        self.is_frlg = self.version == 'frlg'
        self.primary_symbol = entry['primary_tileset']
        self.secondary_symbol = entry['secondary_tileset']
        root = project.root
        self.blockdata_path = os.path.join(root, entry['blockdata_filepath'])
        self.border_path = os.path.join(root, entry['border_filepath'])
        self._blocks = None
        self._border = None

    @property
    def blocks(self):
        """np.uint16 array [height, width] of raw block values."""
        if self._blocks is None:
            a = np.frombuffer(open(self.blockdata_path, 'rb').read(), dtype='<u2')
            self._blocks = a[: self.width * self.height].reshape(self.height, self.width).copy()
        return self._blocks

    @property
    def border(self):
        if self._border is None:
            a = np.frombuffer(open(self.border_path, 'rb').read(), dtype='<u2')
            bw = int(self.entry.get('border_width', 2))
            bh = int(self.entry.get('border_height', 2))
            self._border = a[: bw * bh].reshape(bh, bw).copy()
        return self._border

    def tilesets(self):
        return self.project.tileset(self.primary_symbol), self.project.tileset(self.secondary_symbol)


class Project:
    """A pokeemerald-expansion source tree."""

    def __init__(self, root):
        self.root = os.path.abspath(root)
        self._tilesets_info = None
        self._tilesets = {}
        self._layouts = None
        self._behaviors = None
        self._mt_cache = {}

    # --- tileset registry parsed from C sources ----------------------------
    def _parse_tilesets(self):
        srcs = []
        for rel in ['src/data/tilesets/graphics.h', 'src/data/tilesets/metatiles.h',
                    'src/data/tilesets/headers.h', 'src/graphics.c']:
            p = os.path.join(self.root, rel)
            if os.path.exists(p):
                srcs.append(open(p, encoding='utf-8', errors='replace').read())
        text = '\n'.join(srcs)
        tiles = dict(re.findall(r'(gTilesetTiles_\w+)\s*\[\]\s*=\s*INC\w+\(\s*"([^"]+)"', text))
        pals = {}
        for m in re.finditer(r'(gTilesetPalettes_\w+)\s*\[\]\s*\[16\]\s*=\s*\{(.*?)\};', text, re.S):
            pals[m.group(1)] = re.findall(r'"([^"]+\.pal)"', m.group(2))
        bins = dict(re.findall(r'(gMetatile\w+)\s*\[\]\s*=\s*INCBIN_U(?:16|32)\(\s*"([^"]+)"', text))
        info = {}
        for m in re.finditer(r'const\s+struct\s+Tileset\s+(gTileset_\w+)\s*=\s*\{(.*?)\};', text, re.S):
            body = m.group(2)
            fields = dict(re.findall(r'\.(\w+)\s*=\s*([^,]+),', body))
            mt = bins.get(fields.get('metatiles', '').strip())
            at = bins.get(fields.get('metatileAttributes', '').strip())
            if not mt or not at:
                continue
            ent = {
                'isSecondary': fields.get('isSecondary', 'FALSE').strip() == 'TRUE',
                'tiles': tiles.get(fields.get('tiles', '').strip()),
                'palettes': pals.get(fields.get('palettes', '').strip(), []),
                'metatiles': mt,
                'attributes': at,
            }
            ent['is_frlg'] = '_frlg' in mt.lower()
            info[m.group(1)] = ent
        self._tilesets_info = info

    @property
    def tileset_symbols(self):
        if self._tilesets_info is None:
            self._parse_tilesets()
        return sorted(self._tilesets_info)

    def tileset(self, symbol):
        if self._tilesets_info is None:
            self._parse_tilesets()
        if symbol not in self._tilesets:
            if symbol not in self._tilesets_info:
                raise KeyError('unknown tileset %s' % symbol)
            self._tilesets[symbol] = Tileset(self, symbol, self._tilesets_info[symbol])
        return self._tilesets[symbol]

    # --- layouts -------------------------------------------------------------
    @property
    def layouts_json_path(self):
        return os.path.join(self.root, 'data/layouts/layouts.json')

    def layouts_json(self):
        return json.load(open(self.layouts_json_path))

    @property
    def layouts(self):
        if self._layouts is None:
            d = self.layouts_json()
            self._layouts = {}
            for e in d['layouts']:
                if not e or 'id' not in e:
                    continue
                L = Layout(self, e)
                self._layouts[L.id] = L
                self._layouts[L.name] = L
                # also allow "LittlerootTown" for "LittlerootTown_Layout"
                if L.name.endswith('_Layout'):
                    self._layouts.setdefault(L.name[:-7], L)
        return self._layouts

    def layout(self, key):
        L = self.layouts.get(key)
        if L is None:
            L = self.layouts.get('LAYOUT_' + key.upper())
        if L is None:
            raise KeyError('unknown layout %s' % key)
        return L

    # --- maps ----------------------------------------------------------------
    def map_json_path(self, name):
        return os.path.join(self.root, 'data/maps', name, 'map.json')

    def map_json(self, name):
        return json.load(open(self.map_json_path(name)))

    def map_names(self):
        d = os.path.join(self.root, 'data/maps')
        return sorted(n for n in os.listdir(d) if os.path.exists(os.path.join(d, n, 'map.json')))

    def map_by_id(self, map_id):
        for n in self.map_names():
            try:
                j = self.map_json(n)
            except Exception:
                continue
            if j.get('id') == map_id:
                return n, j
        raise KeyError(map_id)

    # --- behaviors -----------------------------------------------------------
    @property
    def behaviors(self):
        """dict id -> name (MB_*)"""
        if self._behaviors is None:
            p = os.path.join(self.root, 'include/constants/metatile_behaviors.h')
            text = open(p).read()
            body = text[text.index('enum'):]
            body = body[body.index('{') + 1: body.index('}')]
            out = {}
            v = -1
            for line in body.splitlines():
                line = line.split('//')[0].strip().rstrip(',')
                if not line:
                    continue
                if '=' in line:
                    n, val = [x.strip() for x in line.split('=')]
                    v = int(val, 0)
                else:
                    n = line
                    v += 1
                out[v] = n
            self._behaviors = out
        return self._behaviors

    def behavior_id(self, name):
        for k, v in self.behaviors.items():
            if v == name:
                return k
        raise KeyError(name)

    # --- metatile helpers ----------------------------------------------------
    def consts_for(self, is_frlg):
        return FRLG if is_frlg else EM

    def metatile_info(self, primary, secondary, mid):
        """Return (Tileset, local index, behavior, layer type) for a metatile id."""
        P = self.tileset(primary)
        S = self.tileset(secondary)
        c = self.consts_for(P.is_frlg)
        if mid < c['metatiles_primary']:
            ts, li = P, mid
        else:
            ts, li = S, mid - c['metatiles_primary']
        if li < len(ts.attrs):
            b, lt = ts.attrs[li]
        else:
            b, lt = 0, 0
        return ts, li, b, lt

    def metatile_exists(self, primary, secondary, mid):
        ts, li, _, _ = self.metatile_info(primary, secondary, mid)
        return li < ts.num_metatiles

    def full_palette(self, primary, secondary):
        P = self.tileset(primary)
        S = self.tileset(secondary)
        c = self.consts_for(P.is_frlg)
        n = c['pals_primary']
        pals = []
        for i in range(16):
            if i < n:
                src = P.palettes
            else:
                src = S.palettes
            pals.append(src[i] if i < len(src) else [(255, 0, 255)] * 16)
        return pals

    def render_metatile(self, primary, secondary, mid, layers='both'):
        """16x16 RGBA image of a metatile in the context of a tileset pair."""
        key = (primary, secondary, mid, layers)
        im = self._mt_cache.get(key)
        if im is not None:
            return im
        P = self.tileset(primary)
        S = self.tileset(secondary)
        c = self.consts_for(P.is_frlg)
        pals = self.full_palette(primary, secondary)
        ts, li, _, _ = self.metatile_info(primary, secondary, mid)
        out = np.zeros((16, 16, 4), np.uint8)
        bg = pals[0][0]
        out[:, :, :3] = bg
        out[:, :, 3] = 255
        if li >= ts.num_metatiles:
            # undefined metatile: magenta checker
            out[:, :, :3] = (255, 0, 255)
            out[::4, ::4, :3] = 0
            im = Image.fromarray(out, 'RGBA')
            self._mt_cache[key] = im
            return im
        entries = ts.metatiles[li]
        rng = range(8)
        if layers == 'bottom':
            rng = range(4)
        elif layers == 'top':
            rng = range(4, 8)
            out[:, :, 3] = 0
        for i in rng:
            e = int(entries[i])
            tid = e & 0x3FF
            hf = (e >> 10) & 1
            vf = (e >> 11) & 1
            pal = (e >> 12) & 0xF
            if tid < c['tiles_primary']:
                tarr = P.tiles
                ti = tid
            else:
                tarr = S.tiles
                ti = tid - c['tiles_primary']
            if ti >= len(tarr):
                continue
            t = tarr[ti]
            if hf:
                t = t[:, ::-1]
            if vf:
                t = t[::-1, :]
            p = np.array(pals[pal], np.uint8)
            q = i % 4
            ox, oy = (q % 2) * 8, (q // 2) * 8
            mask = t != 0
            region = out[oy:oy + 8, ox:ox + 8]
            region[mask, :3] = p[t[mask]]
            region[mask, 3] = 255
        im = Image.fromarray(out, 'RGBA')
        self._mt_cache[key] = im
        return im


# --- rendering ---------------------------------------------------------------
COLL_COLORS = {1: (255, 0, 0, 90), 2: (255, 128, 0, 90), 3: (255, 0, 128, 90)}


def _font(size=10):
    for p in ['/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf',
              '/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf']:
        if os.path.exists(p):
            return ImageFont.truetype(p, size)
    return ImageFont.load_default()


def render_blocks(project, primary, secondary, blocks, scale=1, collision=False,
                  grid=False, coords=False, border=None, border_pad=0, behaviors=False):
    """Render a 2D array of block values. Returns an RGBA image."""
    h, w = blocks.shape
    pad = border_pad
    W, H = (w + 2 * pad) * 16, (h + 2 * pad) * 16
    img = Image.new('RGBA', (W, H), (0, 0, 0, 255))
    if pad and border is not None:
        bh, bw = border.shape
        for y in range(-pad, h + pad):
            for x in range(-pad, w + pad):
                if 0 <= x < w and 0 <= y < h:
                    continue
                v = int(border[(y % bh), (x % bw)])
                mt = project.render_metatile(primary, secondary, v & 0x3FF)
                img.paste(mt, ((x + pad) * 16, (y + pad) * 16))
        dim = Image.new('RGBA', (W, H), (0, 0, 0, 0))
        d = ImageDraw.Draw(dim)
        d.rectangle([0, 0, W, pad * 16 - 1], fill=(0, 0, 0, 110))
        d.rectangle([0, H - pad * 16, W, H], fill=(0, 0, 0, 110))
        d.rectangle([0, pad * 16, pad * 16 - 1, H - pad * 16 - 1], fill=(0, 0, 0, 110))
        d.rectangle([W - pad * 16, pad * 16, W, H - pad * 16 - 1], fill=(0, 0, 0, 110))
        img = Image.alpha_composite(img, dim)
    for y in range(h):
        for x in range(w):
            v = int(blocks[y, x])
            mt = project.render_metatile(primary, secondary, v & 0x3FF)
            img.paste(mt, ((x + pad) * 16, (y + pad) * 16))
    if collision or behaviors:
        ov = Image.new('RGBA', (W, H), (0, 0, 0, 0))
        d = ImageDraw.Draw(ov)
        f = _font(7)
        for y in range(h):
            for x in range(w):
                v = int(blocks[y, x])
                c = (v >> 10) & 3
                px, py = (x + pad) * 16, (y + pad) * 16
                if collision and c:
                    d.rectangle([px, py, px + 15, py + 15], fill=COLL_COLORS.get(c))
                if behaviors:
                    _, _, b, _ = project.metatile_info(primary, secondary, v & 0x3FF)
                    if b:
                        d.text((px + 1, py + 4), '%02X' % b, fill=(255, 255, 0, 255), font=f)
        img = Image.alpha_composite(img, ov)
    if scale != 1:
        img = img.resize((W * scale, H * scale), Image.NEAREST)
    if grid or coords:
        d = ImageDraw.Draw(img)
        s = 16 * scale
        f = _font(max(7, 4 * scale + 3))
        if grid:
            for x in range(w + 2 * pad + 1):
                d.line([(x * s, 0), (x * s, H * scale)], fill=(0, 0, 0, 70))
            for y in range(h + 2 * pad + 1):
                d.line([(0, y * s), (W * scale, y * s)], fill=(0, 0, 0, 70))
        if coords:
            for x in range(w):
                if x % 5 == 0 or w <= 25:
                    d.text(((x + pad) * s + 2, pad * s + 1), str(x), fill=(255, 255, 255, 255), font=f,
                           stroke_width=1, stroke_fill=(0, 0, 0, 255))
            for y in range(h):
                if y % 5 == 0 or h <= 25:
                    d.text((pad * s + 2, (y + pad) * s + 1), str(y), fill=(255, 255, 0, 255), font=f,
                           stroke_width=1, stroke_fill=(0, 0, 0, 255))
    return img


EVENT_COLORS = {
    'object': (40, 120, 255),
    'warp': (255, 40, 200),
    'coord': (255, 200, 0),
    'bg': (0, 220, 120),
}


def draw_events(img, mapj, scale=1, pad=0):
    d = ImageDraw.Draw(img)
    s = 16 * scale
    f = _font(max(8, 3 * scale + 5))

    def box(x, y, kind, label):
        col = EVENT_COLORS[kind]
        px, py = (x + pad) * s, (y + pad) * s
        d.rectangle([px + 1, py + 1, px + s - 2, py + s - 2], outline=col + (255,), width=max(1, scale))
        if label:
            d.text((px + 2, py + 2), label, fill=col + (255,), font=f, stroke_width=1, stroke_fill=(0, 0, 0, 255))

    for i, o in enumerate(mapj.get('object_events', [])):
        box(int(o['x']), int(o['y']), 'object', 'O%d' % (i + 1))
        if o.get('trainer_type', 'TRAINER_TYPE_NONE') not in ('TRAINER_TYPE_NONE', '0'):
            d.text(((int(o['x']) + pad) * s + 2, (int(o['y']) + pad) * s + s // 2), 'T', fill=(255, 60, 60, 255),
                   font=f, stroke_width=1, stroke_fill=(0, 0, 0, 255))
    for i, w in enumerate(mapj.get('warp_events', [])):
        box(int(w['x']), int(w['y']), 'warp', 'W%d' % i)
    for i, c in enumerate(mapj.get('coord_events', [])):
        box(int(c['x']), int(c['y']), 'coord', 'C')
    for i, b in enumerate(mapj.get('bg_events', [])):
        box(int(b['x']), int(b['y']), 'bg', 'S' if b.get('type') == 'sign' else 'B')
    return img


def render_layout(project, layout, **kw):
    if isinstance(layout, str):
        layout = project.layout(layout)
    pad = kw.pop('border_pad', 0)
    return render_blocks(project, layout.primary_symbol, layout.secondary_symbol, layout.blocks,
                         border=layout.border, border_pad=pad, **kw)


def render_tileset_sheet(project, primary, secondary, which='secondary', cols=16, scale=2, label=True):
    """Render all metatiles of a tileset (in the context of a pair) as a numbered sheet."""
    P = project.tileset(primary)
    c = project.consts_for(P.is_frlg)
    if which == 'primary':
        start, n = 0, P.num_metatiles
    else:
        S = project.tileset(secondary)
        start, n = c['metatiles_primary'], S.num_metatiles
    rows = (n + cols - 1) // cols
    cell = 16 * scale
    lab = 12 if label else 0
    img = Image.new('RGBA', (cols * cell + 34, rows * (cell + lab)), (40, 40, 40, 255))
    d = ImageDraw.Draw(img)
    f = _font(9)
    for i in range(n):
        mid = start + i
        mt = project.render_metatile(primary, secondary, mid).resize((cell, cell), Image.NEAREST)
        x, y = (i % cols) * cell + 34, (i // cols) * (cell + lab)
        img.paste(mt, (x, y))
        if label:
            d.text((x + 1, y + cell), '%03X' % mid, fill=(220, 220, 220, 255), font=f)
    for r in range(rows):
        d.text((1, r * (cell + lab) + cell // 2 - 5), '%03X' % (start + r * cols), fill=(255, 255, 0, 255), font=f)
    return img
