"""terrain: example-based autotiling of ASCII terrain grids.

The idea: every terrain metatile of a tileset is labelled with a terrain class
(one ASCII char, see classes/<tileset>.json).  From all existing layouts that
use the tileset we learn
  * P(metatile | class of the cell and of its 8 neighbours)   (with back-off)
  * which metatiles may sit next to each other (horizontal / vertical pairs)
  * the usual collision / elevation of each metatile.
To synthesise a map we take the class grid written by the author, the fixed
cells (stamps) and pick, for every free cell, the metatile maximising
  log P(m | window) + sum(log compat(m, neighbour))
with a greedy raster pass followed by a few ICM sweeps.  Edges, shores, tree
crowns, 2x2 trees, ... all come out of the statistics of the real maps.
"""
import collections
import hashlib
import json
import math
import os
import pickle

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
CLASS_DIR = os.path.join(HERE, 'classes')
CACHE_DIR = os.path.join(HERE, 'cache')
MODEL_VERSION = 3

UNKNOWN = '?'
DIRS8 = [(0, -1), (1, 0), (0, 1), (-1, 0), (1, -1), (1, 1), (-1, 1), (-1, -1)]  # N E S W NE SE SW NW


def _expand(ids):
    out = []
    for s in ids:
        s = str(s)
        if '-' in s:
            a, b = s.split('-')
            out.extend(range(int(a, 16), int(b, 16) + 1))
        else:
            out.append(int(s, 16))
    return out


def load_phase_table(symbol, offset=0):
    """{metatile_id: (x_parity or None, y_parity or None)} from the "_phase" section of a class file.

    Multi-cell objects (2x2 round trees) must sit on a fixed grid; the phase
    table says which half of the object a metatile is."""
    p = os.path.join(CLASS_DIR, symbol + '.json')
    if not os.path.exists(p):
        return {}
    ph = json.load(open(p)).get('_phase', {})
    out = {}
    for key, axis, val in (('x_even', 0, 0), ('x_odd', 0, 1), ('y_even', 1, 0), ('y_odd', 1, 1)):
        for m in _expand(ph.get(key, [])):
            cur = list(out.get(m + offset, (None, None)))
            cur[axis] = val
            out[m + offset] = tuple(cur)
    return out


def load_class_table(symbol, offset=0):
    """Return {metatile_id: class_char} for a tileset label file (or {})."""
    p = os.path.join(CLASS_DIR, symbol + '.json')
    if not os.path.exists(p):
        return {}
    d = json.load(open(p))
    out = {}
    for cls, ids in d.items():
        if len(cls) != 1:
            continue
        for m in _expand(ids):
            out[m + offset] = cls
    return out


class Model:
    """Statistics learned from existing layouts sharing a primary tileset."""

    def __init__(self):
        self.win9 = collections.defaultdict(collections.Counter)
        self.win4 = collections.defaultdict(collections.Counter)
        self.win1 = collections.defaultdict(collections.Counter)
        self.H = collections.Counter()   # (left, right)
        self.V = collections.Counter()   # (top, bottom)
        self.ce = collections.defaultdict(collections.Counter)  # metatile -> (coll, elev)
        self.layouts = []

    # metatile keys: ints < n_primary for primary metatiles, ('Secondary', id) else
    def finalize(self):
        self.win9 = dict(self.win9)
        self.win4 = dict(self.win4)
        self.win1 = dict(self.win1)
        self.ce = {k: v.most_common(1)[0][0] for k, v in self.ce.items()}


def mkey(mid, secondary, n_primary):
    return mid if mid < n_primary else (secondary, mid)


def class_grid_for_layout(project, layout, classes_by_sec, n_primary):
    b = layout.blocks & 0x3FF
    h, w = b.shape
    sec_tab = classes_by_sec.get(layout.secondary_symbol, {})
    prim_tab = classes_by_sec.get('__primary__', {})
    g = np.full((h, w), UNKNOWN, dtype='<U1')
    for y in range(h):
        for x in range(w):
            m = int(b[y, x])
            c = prim_tab.get(m) if m < n_primary else sec_tab.get(m)
            if c:
                g[y, x] = c
    return g


def window(g, x, y):
    h, w = g.shape
    out = [g[y, x]]
    for dx, dy in DIRS8:
        xx = min(max(x + dx, 0), w - 1)
        yy = min(max(y + dy, 0), h - 1)
        out.append(g[yy, xx])
    return ''.join(out)


def train(project, primary, secondaries=None, verbose=False, focus=None, focus_weight=8, exclude=()):
    P = project
    n_primary = P.consts_for(P.tileset(primary).is_frlg)['metatiles_primary']
    classes = {'__primary__': load_class_table(primary)}
    M = Model()
    seen = set()
    for key, L in P.layouts.items():
        if key != L.id or L.primary_symbol != primary:
            continue
        if secondaries and L.secondary_symbol not in secondaries:
            continue
        if L.name in seen or L.id in exclude:
            continue
        seen.add(L.name)
        try:
            blocks = L.blocks
        except Exception:
            continue
        if L.secondary_symbol not in classes:
            classes[L.secondary_symbol] = load_class_table(L.secondary_symbol, offset=n_primary)
        g = class_grid_for_layout(P, L, classes, n_primary)
        h, w = blocks.shape
        if h < 3 or w < 3:
            continue
        M.layouts.append(L.name)
        wgt = focus_weight if focus and L.secondary_symbol == focus else 1
        mids = blocks & 0x3FF
        keys = [[mkey(int(mids[y, x]), L.secondary_symbol, n_primary) for x in range(w)] for y in range(h)]
        for y in range(h):
            for x in range(w):
                k = keys[y][x]
                v = int(blocks[y, x])
                M.ce[k][((v >> 10) & 3, (v >> 12) & 0xF)] += wgt
                if x + 1 < w:
                    M.H[(k, keys[y][x + 1])] += wgt
                if y + 1 < h:
                    M.V[(k, keys[y + 1][x])] += wgt
                c = g[y, x]
                if c == UNKNOWN:
                    continue
                wd = window(g, x, y)
                M.win9[wd][k] += wgt
                M.win4[wd[:5]][k] += wgt
                M.win1[c][k] += wgt
    M.finalize()
    M.primary = primary
    M.n_primary = n_primary
    M.classes = classes
    if verbose:
        print('trained on %d layouts' % len(M.layouts))
    return M


def get_model(project, primary, secondaries=None, rebuild=False, focus=None, exclude=()):
    """Model trained on layouts using `primary` (and, if given, one of `secondaries`)."""
    os.makedirs(CACHE_DIR, exist_ok=True)
    secondaries = sorted(secondaries) if secondaries else None
    # cache is invalidated when class files change
    stamp = [MODEL_VERSION, project.root, os.path.getmtime(os.path.abspath(__file__))]
    for f in sorted(os.listdir(CLASS_DIR)):
        stamp.append((f, os.path.getmtime(os.path.join(CLASS_DIR, f))))
    stamp.append(secondaries)
    stamp.append(focus)
    stamp.append(sorted(exclude))
    if secondaries and len(secondaries) > 3:
        tag = primary + '__%d_secondaries_%08x' % (len(secondaries), int(hashlib.md5('|'.join(secondaries).encode()).hexdigest()[:8], 16))
    else:
        tag = primary if not secondaries else primary + '__' + '_'.join(x.replace('gTileset_', '') for x in secondaries)
    if focus:
        tag += '__focus_' + focus.replace('gTileset_', '')
    p = os.path.join(CACHE_DIR, 'model_%s.pickle' % tag)
    if not rebuild and os.path.exists(p):
        try:
            with open(p, 'rb') as f:
                st, M = pickle.load(f)
            if st == stamp:
                return M
        except Exception:
            pass
    M = train(project, primary, secondaries, focus=focus, exclude=set(exclude))
    with open(p, 'wb') as f:
        pickle.dump((stamp, M), f)
    return M


class Synth:
    """Synthesise metatiles for a class grid."""

    PENALTY = math.log(1e-3)

    PHASE_PENALTY = -8.0

    def __init__(self, model, secondary, grid, fixed=None, fixed_class=None, rng_seed=0, variety=0.0,
                 phase=(0, 0)):
        self.M = model
        self.sec = secondary
        self.g = grid                      # np array of class chars
        self.h, self.w = grid.shape
        self.fixed = fixed or {}           # (x,y) -> metatile key
        self.variety = variety
        self.rng = np.random.RandomState(rng_seed)
        self.sec_classes = model.classes.get(secondary)
        if self.sec_classes is None:
            self.sec_classes = load_class_table(secondary, offset=model.n_primary)
        self.class_of_key = {}
        for m, c in model.classes['__primary__'].items():
            self.class_of_key[m] = c
        for m, c in self.sec_classes.items():
            self.class_of_key[(secondary, m)] = c
        self.phase_off = phase
        self.phase = {}
        for m, ph in load_phase_table(model.primary).items():
            self.phase[m] = ph
        for m, ph in load_phase_table(secondary, offset=model.n_primary).items():
            self.phase[(secondary, m)] = ph
        self.out = [[None] * self.w for _ in range(self.h)]
        for (x, y), k in self.fixed.items():
            self.out[y][x] = k

    def valid_key(self, k):
        return not isinstance(k, tuple) or k[0] == self.sec

    def dist(self, wd):
        """candidate distribution for a window: list of (key, logp)."""
        M = self.M
        c = wd[0]
        base = M.win1.get(c, {})
        base = {k: v for k, v in base.items() if self.valid_key(k)}
        if not base:
            return []
        tot1 = sum(base.values())
        d9 = {k: v for k, v in M.win9.get(wd, {}).items() if self.valid_key(k)}
        d4 = {k: v for k, v in M.win4.get(wd[:5], {}).items() if self.valid_key(k)}
        t9, t4 = sum(d9.values()), sum(d4.values())
        out = []
        for k, v in base.items():
            p = 1e-4 * v / tot1
            if t4:
                p += (0.3 if t9 else 1.0) * d4.get(k, 0) / t4
            if t9:
                p += 1.0 * d9.get(k, 0) / t9
            out.append((k, math.log(p)))
        return out

    def compat(self, k, x, y, use_all):
        M = self.M
        s = 0.0
        nb = [(-1, 0, 'L'), (0, -1, 'U')]
        if use_all:
            nb += [(1, 0, 'R'), (0, 1, 'D')]
        else:
            # always respect fixed neighbours on the right/below
            for dx, dy, t in [(1, 0, 'R'), (0, 1, 'D')]:
                if (x + dx, y + dy) in self.fixed:
                    nb.append((dx, dy, t))
        for dx, dy, t in nb:
            xx, yy = x + dx, y + dy
            if not (0 <= xx < self.w and 0 <= yy < self.h):
                continue
            o = self.out[yy][xx]
            if o is None:
                continue
            if t == 'L':
                c = M.H.get((o, k), 0)
            elif t == 'R':
                c = M.H.get((k, o), 0)
            elif t == 'U':
                c = M.V.get((o, k), 0)
            else:
                c = M.V.get((k, o), 0)
            if c == 0:
                s += self.PENALTY
            else:
                s += min(0.0, 0.25 * math.log(c / 4.0))  # tiny preference for well attested pairs
        return s

    def phase_cost(self, k, x, y):
        ph = self.phase.get(k)
        if not ph:
            return 0.0
        c = 0.0
        if ph[0] is not None and (x + self.phase_off[0]) % 2 != ph[0]:
            c += self.PHASE_PENALTY
        if ph[1] is not None and (y + self.phase_off[1]) % 2 != ph[1]:
            c += self.PHASE_PENALTY
        return c

    def pick(self, x, y, use_all):
        wd = window(self.g, x, y)
        cands = self.dist(wd)
        if not cands:
            return None
        best = None
        scored = []
        for k, lp in cands:
            sc = lp + self.compat(k, x, y, use_all) + self.phase_cost(k, x, y)
            scored.append((sc, k))
        scored.sort(key=lambda t: (-t[0], str(t[1])))
        best = scored[0]
        if self.variety > 0 and len(scored) > 1:
            # sample among near-best candidates for natural variation
            near = [t for t in scored if t[0] >= best[0] - self.variety]
            if len(near) > 1:
                ws = np.array([math.exp(t[0] - best[0]) for t in near])
                i = self.rng.choice(len(near), p=ws / ws.sum())
                return near[i][1]
        return best[1]

    def run(self, sweeps=3):
        free = [(x, y) for y in range(self.h) for x in range(self.w)
                if (x, y) not in self.fixed and self.g[y, x] != UNKNOWN]
        for x, y in free:
            self.out[y][x] = self.pick(x, y, use_all=False)
        v = self.variety
        self.variety = 0.0
        for _ in range(sweeps):
            changed = 0
            for x, y in free:
                k = self.pick(x, y, use_all=True)
                if k != self.out[y][x]:
                    self.out[y][x] = k
                    changed += 1
            if not changed:
                break
        self.variety = v
        return self.out

    def problems(self):
        """List (x, y, why) of cells whose neighbours were never seen together."""
        M = self.M
        res = []
        for y in range(self.h):
            for x in range(self.w):
                k = self.out[y][x]
                if k is None:
                    continue
                if x + 1 < self.w and self.out[y][x + 1] is not None and not M.H.get((k, self.out[y][x + 1])):
                    res.append((x, y, 'E'))
                if y + 1 < self.h and self.out[y + 1][x] is not None and not M.V.get((k, self.out[y + 1][x])):
                    res.append((x, y, 'S'))
        return res


def key_to_mid(k):
    return k[1] if isinstance(k, tuple) else k
