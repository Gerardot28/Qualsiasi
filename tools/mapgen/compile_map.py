#!/usr/bin/env python3
"""compile_map.py - compile map specs (YAML/JSON) into a pokeemerald-expansion tree.

    compile_map.py SPEC.yaml [SPEC2.yaml ...] [--root TREE] [--render DIR] [--check]

  --check   validate + synthesise + render only, write nothing into the tree
  --render  directory for PNG renders of every compiled map (default
            /home/user/work/mapgen-out/render)

All specs given on one command line are compiled together, so warps,
connections and doors between them are resolved automatically.  Re-running is
idempotent: generated maps / layouts / registry entries are replaced, never
duplicated.  See AUTHORING.md for the spec format.
"""
import argparse
import collections
import copy
import json
import os
import re
import sys
import textwrap

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import pexmap  # noqa: E402
import stamps as stamplib  # noqa: E402
import terrain  # noqa: E402

DEFAULT_ROOT = os.environ.get('PEX_ROOT', '/home/user/work/mapgen-tree')
DEFAULT_RENDER = '/home/user/work/mapgen-out/render'
AGGREGATE_INC = 'data/maps/mapgen_scripts.inc'
EVENT_SCRIPTS = 'data/event_scripts.s'
MAX_MAP_DATA_SIZE = 10240

MOVES = {
    'none': 'MOVEMENT_TYPE_NONE', 'look_around': 'MOVEMENT_TYPE_LOOK_AROUND',
    'wander': 'MOVEMENT_TYPE_WANDER_AROUND', 'wander_up_down': 'MOVEMENT_TYPE_WANDER_UP_AND_DOWN',
    'wander_left_right': 'MOVEMENT_TYPE_WANDER_LEFT_AND_RIGHT',
    'face_up': 'MOVEMENT_TYPE_FACE_UP', 'face_down': 'MOVEMENT_TYPE_FACE_DOWN',
    'face_left': 'MOVEMENT_TYPE_FACE_LEFT', 'face_right': 'MOVEMENT_TYPE_FACE_RIGHT',
    'up': 'MOVEMENT_TYPE_FACE_UP', 'down': 'MOVEMENT_TYPE_FACE_DOWN',
    'left': 'MOVEMENT_TYPE_FACE_LEFT', 'right': 'MOVEMENT_TYPE_FACE_RIGHT',
    'walk_up_down': 'MOVEMENT_TYPE_WALK_UP_AND_DOWN', 'walk_left_right': 'MOVEMENT_TYPE_WALK_LEFT_AND_RIGHT',
    'rotate': 'MOVEMENT_TYPE_ROTATE_CLOCKWISE',
}
DIR_ALIASES = {'up': 'up', 'north': 'up', 'n': 'up', 'down': 'down', 'south': 'down', 's': 'down',
               'left': 'left', 'west': 'left', 'w': 'left', 'right': 'right', 'east': 'right', 'e': 'right'}
OPPOSITE = {'up': 'down', 'down': 'up', 'left': 'right', 'right': 'left'}
MAP_TYPES = {'town': 'MAP_TYPE_TOWN', 'city': 'MAP_TYPE_CITY', 'route': 'MAP_TYPE_ROUTE',
             'indoor': 'MAP_TYPE_INDOOR', 'cave': 'MAP_TYPE_UNDERGROUND', 'underground': 'MAP_TYPE_UNDERGROUND',
             'underwater': 'MAP_TYPE_UNDERWATER', 'secret_base': 'MAP_TYPE_SECRET_BASE'}
WILD_SLOTS = {'land_mons': 12, 'water_mons': 5, 'rock_smash_mons': 5, 'fishing_mons': 10}
WILD_ALIASES = {'land': 'land_mons', 'water': 'water_mons', 'rock_smash': 'rock_smash_mons', 'fishing': 'fishing_mons'}

# Templates for re-used interiors: base map whose layout + warp geometry is copied.
TEMPLATES = {
    'pokecenter_1f': {'base_map': 'OldaleTown_PokemonCenter_1F', 'music': 'MUS_POKE_CENTER', 'role': 'pokecenter'},
    'pokecenter_2f': {'base_map': 'OldaleTown_PokemonCenter_2F', 'music': 'MUS_POKE_CENTER', 'role': 'pokecenter_2f'},
    'mart': {'base_map': 'OldaleTown_Mart', 'music': 'MUS_POKE_MART', 'role': 'mart'},
    'house1': {'base_map': 'OldaleTown_House1'},
    'house2': {'base_map': 'OldaleTown_House2'},
    'house_littleroot': {'base_map': 'LittlerootTown_MaysHouse_1F'},
    'lab': {'base_map': 'LittlerootTown_ProfessorBirchsLab'},
    'gym_rustboro': {'base_map': 'RustboroCity_Gym', 'music': 'MUS_GYM'},
    'gym_dewford': {'base_map': 'DewfordTown_Gym', 'music': 'MUS_GYM'},
    'gym_petalburg': {'base_map': 'PetalburgCity_Gym', 'music': 'MUS_GYM'},
}


class CompileError(Exception):
    pass


def snake_upper(name):
    s = re.sub(r'([a-z0-9])([A-Z])', r'\1_\2', name)
    s = re.sub(r'([A-Z]+)([A-Z][a-z])', r'\1_\2', s)
    return re.sub(r'[^A-Za-z0-9]+', '_', s).upper().strip('_')


def as_int(v):
    if isinstance(v, int):
        return v
    s = str(v).strip()
    if s.lower().startswith('0x'):
        return int(s, 16)
    if re.fullmatch(r'[0-9A-Fa-f]{3}', s):
        return int(s, 16)
    return int(s, 0)


def load_spec_file(path):
    text = open(path).read()
    if path.endswith('.json'):
        d = json.loads(text)
    else:
        import yaml
        d = yaml.safe_load(text)
    if isinstance(d, list):
        d = {'maps': d}
    if 'maps' not in d:
        d = {'maps': [d]}
    defaults = {k: v for k, v in d.items() if k != 'maps'}
    out = []
    for m in d['maps']:
        mm = dict(defaults)
        mm.update(m)
        mm['_file'] = path
        out.append(mm)
    return out


# ----------------------------------------------------------------------------- text / scripts

def wrap_text(text, width=34):
    """Turn free text into pokeemerald .string lines (\\n, \\l, \\p handled)."""
    text = str(text).strip()
    paras = re.split(r'\n\s*\n|\\p', text)
    out_paras = []
    for p in paras:
        p = ' '.join(p.split())
        if not p:
            continue
        lines = textwrap.wrap(p, width=width, break_long_words=False, break_on_hyphens=False) or ['']
        s = ''
        for i, l in enumerate(lines):
            if i == 0:
                s += l
            elif i == 1:
                s += '\\n' + l
            else:
                s += '\\l' + l
        out_paras.append(s)
    full = '\\p'.join(out_paras) + '$'
    # split into .string chunks after every \n \l \p for readability
    chunks = re.split(r'(?<=\\[nlp])', full)
    res = []
    for c in chunks:
        if c:
            res.append('\t.string "%s"' % c.replace('"', '\\"'))
    return '\n'.join(res)


class Scripts:
    def __init__(self, prefix):
        self.prefix = prefix
        self.map_scripts = []       # (type, label)
        self.blocks = []            # list of str
        self.texts = []
        self.names = set()
        self.raw = []

    def label(self, base):
        base = re.sub(r'[^A-Za-z0-9_]', '', base) or 'Script'
        lab = '%s_EventScript_%s' % (self.prefix, base)
        n = 2
        while lab in self.names:
            lab = '%s_EventScript_%s%d' % (self.prefix, base, n)
            n += 1
        self.names.add(lab)
        return lab

    def text(self, base, content):
        base = re.sub(r'[^A-Za-z0-9_]', '', base) or 'Text'
        lab = '%s_Text_%s' % (self.prefix, base)
        n = 2
        while lab in self.names:
            lab = '%s_Text_%s%d' % (self.prefix, base, n)
            n += 1
        self.names.add(lab)
        self.texts.append('%s:\n%s\n' % (lab, wrap_text(content)))
        return lab

    def render(self):
        out = ['@ Generated by tools/mapgen/compile_map.py - edit the spec, not this file.', '']
        out.append('%s_MapScripts::' % self.prefix)
        for t, lab in self.map_scripts:
            out.append('\tmap_script %s, %s' % (t, lab))
        out.append('\t.byte 0')
        out.append('')
        out.extend(self.blocks)
        if self.raw:
            out.append('@ ---- raw scripts from spec ----')
            out.extend(self.raw)
            out.append('')
        out.append('@ ---- texts ----')
        out.extend(self.texts)
        return '\n'.join(out) + '\n'


# ----------------------------------------------------------------------------- constants checking

class Constants:
    def __init__(self, root):
        self.root = root
        self._cache = {}

    def _scan(self, rel_files, prefix):
        key = (tuple(rel_files), prefix)
        if key in self._cache:
            return self._cache[key]
        names = set()
        pat = re.compile(r'\b(%s\w+)\b' % re.escape(prefix))
        for rel in rel_files:
            p = os.path.join(self.root, rel)
            if os.path.isdir(p):
                files = [os.path.join(p, f) for f in os.listdir(p) if f.endswith('.h')]
            else:
                files = [p]
            for f in files:
                if os.path.exists(f):
                    names.update(pat.findall(open(f, encoding='utf-8', errors='replace').read()))
        self._cache[key] = names
        return names

    def has(self, name):
        if not isinstance(name, str):
            return True
        prefixes = [
            ('OBJ_EVENT_GFX_', ['include/constants/event_objects.h']),
            ('MOVEMENT_TYPE_', ['include/constants/event_object_movement.h']),
            ('SPECIES_', ['include/constants/species.h']),
            ('ITEM_', ['include/constants/items.h']),
            ('MUS_', ['include/constants/songs.h']),
            ('TRAINER_TYPE_', ['include/constants/trainer_types.h', 'include/constants/event_objects.h',
                               'include/constants/global.h']),
            ('TRAINER_', ['include/constants/opponents.h']),
            ('FLAG_', ['include/constants/flags.h']),
            ('VAR_', ['include/constants/vars.h']),
            ('WEATHER_', ['include/constants/weather.h']),
            ('MAPSEC_', ['include/constants/region_map_sections.h', 'src/data/region_map/region_map_sections.json']),
        ]
        for pre, files in prefixes:
            if name.startswith(pre):
                if pre == 'TRAINER_' and name.startswith('TRAINER_TYPE_'):
                    continue
                return name in self._scan(files, pre)
        return True


# ----------------------------------------------------------------------------- the compiler

class MapBuild:
    def __init__(self, spec):
        self.spec = spec
        self.name = spec['name']
        if not re.fullmatch(r'[A-Za-z][A-Za-z0-9_]*', self.name):
            raise CompileError('bad map name %r (use CamelCase / underscores)' % self.name)
        self.map_id = spec.get('id') or 'MAP_' + snake_upper(self.name)
        self.layout_id = None
        self.json = None
        self.scripts = Scripts(self.name)
        self.warps = []          # dicts with x,y,elevation,to,warp (unresolved)
        self.blocks = None
        self.border = None
        self.family = None
        self.layout_entry = None
        self.reuse_base = None
        self.exit_warp_indices = []
        self.synth_problems = []
        self.heal = None
        self.notes = []


class Compiler:
    def __init__(self, root, check=False, render_dir=DEFAULT_RENDER, verbose=True):
        self.root = os.path.abspath(root)
        self.P = pexmap.Project(self.root)
        self.check = check
        self.render_dir = render_dir
        self.verbose = verbose
        self.families = {k: v for k, v in json.load(open(os.path.join(HERE, 'families.json'))).items()
                         if not k.startswith('_')}
        self._outdoor = json.load(open(os.path.join(HERE, 'families.json')))['_outdoor_secondaries']
        self.lib = stamplib.load_library()
        self.consts = Constants(self.root)
        self.maps = collections.OrderedDict()
        self.warnings = []
        self.errors = []
        self.models = {}
        self.new_mapsecs = collections.OrderedDict()
        self.new_trainers = []
        self.patches = []

    # -- messages
    def warn(self, mb, msg):
        self.warnings.append('%s: %s' % (mb.name if mb else '-', msg))

    def err(self, mb, msg):
        self.errors.append('%s: %s' % (mb.name if mb else '-', msg))

    def need_const(self, mb, name, what):
        if isinstance(name, str) and not self.consts.has(name):
            # constants created by this run are fine
            if name in self.new_mapsecs or name in [t['id'] for t in self.new_trainers]:
                return
            self.err(mb, 'unknown %s constant %s' % (what, name))

    # -- lookup of existing maps (outside this compile)
    def existing_map(self, ref):
        """ref: folder name or MAP_ id -> (folder, json) or None"""
        if ref.startswith('MAP_'):
            try:
                return self.P.map_by_id(ref)
            except KeyError:
                return None
        p = self.P.map_json_path(ref)
        if os.path.exists(p):
            return ref, json.load(open(p))
        return None

    def resolve_map_ref(self, ref):
        """-> (MAP_ID, MapBuild or None)"""
        if ref in self.maps:
            return self.maps[ref].map_id, self.maps[ref]
        for mb in self.maps.values():
            if mb.map_id == ref:
                return ref, mb
        ex = self.existing_map(ref)
        if ex:
            return ex[1]['id'], None
        if ref.startswith('MAP_'):
            return ref, None
        raise CompileError('unknown map %r' % ref)

    def model(self, fam):
        key = fam['primary'], tuple(fam['train'] if fam['train'] != 'outdoor' else self._outdoor)
        if key not in self.models:
            self.models[key] = terrain.get_model(self.P, fam['primary'], list(key[1]))
        return self.models[key]

    # ------------------------------------------------------------------ phase 1: parse
    def add_specs(self, specs):
        for s in specs:
            if 'patch_map' in s:
                self.patches.append(s)
                continue
            mb = MapBuild(s)
            if mb.name in self.maps:
                raise CompileError('map %s defined twice' % mb.name)
            self.maps[mb.name] = mb
            for t in s.get('new_trainers', []) or []:
                self.new_trainers.append(t)

    # ------------------------------------------------------------------ phase 2: layouts
    def build_layout(self, mb):
        s = mb.spec
        if 'terrain' in s:
            self.build_terrain_layout(mb)
        elif 'template' in s or 'base_map' in s:
            self.build_reuse(mb)
        elif 'layout' in s:
            mb.layout_id = s['layout']
            mb.reuse_base = None
        else:
            raise CompileError('%s: needs one of terrain / template / base_map / layout' % mb.name)

    def build_terrain_layout(self, mb):
        s = mb.spec
        famname = s.get('family', 'petalburg')
        if famname not in self.families:
            raise CompileError('%s: unknown family %r (see families.json)' % (mb.name, famname))
        fam = self.families[famname]
        mb.family = famname
        rows = [r.rstrip() for r in str(s['terrain']).strip('\n').split('\n')]
        rows = [r for r in rows if r.strip() != '' or True]
        w = max(len(r) for r in rows)
        h = len(rows)
        size = s.get('size')
        if size:
            if [w, h] != list(size):
                self.warn(mb, 'terrain grid is %dx%d but size says %s; using grid' % (w, h, size))
        fill = s.get('fill', '.')
        rows = [r.ljust(w, fill) for r in rows]
        legend = s.get('legend', {}) or {}
        prim, sec = fam['primary'], fam['secondary']
        M = self.model(fam)
        n_primary = M.n_primary
        known_classes = set(M.win1.keys()) | {'!'}
        grid = np.full((h, w), '.', dtype='<U1')
        fixed = {}
        fixed_raw = {}
        for y, r in enumerate(rows):
            for x, ch in enumerate(r):
                if ch == ' ':
                    ch = fill
                if ch in legend:
                    L = legend[ch]
                    if isinstance(L, str):
                        ch = L
                    else:
                        if 'class' in L:
                            ch = L['class']
                        if 'metatile' in L:
                            mid = as_int(L['metatile'])
                            fixed[(x, y)] = terrain.mkey(mid, sec, n_primary)
                            fixed_raw[(x, y)] = pexmap.blk(mid, L.get('collision', M.ce.get(fixed[(x, y)], (0, 3))[0]),
                                                           L.get('elevation', M.ce.get(fixed[(x, y)], (0, 3))[1]))
                            grid[y, x] = L.get('class', '?')
                            continue
                if ch not in known_classes:
                    raise CompileError('%s: terrain char %r at (%d,%d) not available in family %s. Known: %s'
                                       % (mb.name, ch, x, y, famname, ''.join(sorted(known_classes))))
                grid[y, x] = ch
        # signs from the spec get a signpost metatile unless place: false
        for g in s.get('signs', []) or []:
            if g.get('place', True) and 0 <= int(g['y']) < h and 0 <= int(g['x']) < w:
                if grid[int(g['y']), int(g['x'])] in ('.', ',', ':', '_', 's', 'f', 'P'):
                    grid[int(g['y']), int(g['x'])] = '!'
        # signs: '!' cells
        sign_cells = [(x, y) for y in range(h) for x in range(w) if grid[y, x] == '!']
        sign_mid = 0x003 if prim == 'gTileset_General' else None
        for (x, y) in sign_cells:
            if sign_mid is None:
                raise CompileError('%s: "!" sign char only supported for gTileset_General families' % mb.name)
            fixed[(x, y)] = sign_mid
            fixed_raw[(x, y)] = pexmap.blk(sign_mid, 1, 0)
        # stamps
        mb.stamp_doors = []
        for i, st in enumerate(s.get('stamps', []) or []):
            sid = st['stamp']
            if sid not in self.lib:
                raise CompileError('%s: unknown stamp %r (stamps.py list)' % (mb.name, sid))
            S = self.lib[sid]
            if not stamplib.compatible(S, prim, sec):
                raise CompileError('%s: stamp %s needs %s/%s but family %s is %s/%s'
                                   % (mb.name, sid, S['primary'], S['secondary'], famname, prim, sec))
            sx, sy = int(st['x']), int(st['y'])
            for yy, row in enumerate(S['blocks']):
                for xx, v in enumerate(row):
                    if v is None:
                        continue
                    X, Y = sx + xx, sy + yy
                    if not (0 <= X < w and 0 <= Y < h):
                        raise CompileError('%s: stamp %s at (%d,%d) does not fit in %dx%d map'
                                           % (mb.name, sid, sx, sy, w, h))
                    if (X, Y) in fixed and fixed_raw[(X, Y)] != v:
                        self.err(mb, 'stamp %s at (%d,%d) overlaps another stamp at (%d,%d)' % (sid, sx, sy, X, Y))
                    mid = v & 0x3FF
                    k = terrain.mkey(mid, sec, n_primary)
                    fixed[(X, Y)] = k
                    fixed_raw[(X, Y)] = v
                    c = M.classes['__primary__'].get(mid) if mid < n_primary else \
                        terrain.load_class_table(sec, offset=n_primary).get(mid)
                    grid[Y, X] = c or '?'
            doors = S.get('doors', [])
            dspec = st.get('door') or st.get('doors')
            if isinstance(dspec, dict):
                dspec = [dspec]
            dspec = dspec or []
            for di, d in enumerate(doors):
                if di < len(dspec) and dspec[di]:
                    ds = dspec[di]
                    mb.warps.append({'x': sx + d['x'], 'y': sy + d['y'], 'elevation': 0,
                                     'to': ds['to'], 'warp': ds.get('warp'), 'src': 'stamp %s' % sid})
                else:
                    self.warn(mb, 'door of stamp %s at (%d,%d) has no destination (add door: {to: ...})'
                              % (sid, sx + d['x'], sy + d['y']))
        # explicit tiles
        for t in s.get('tiles', []) or []:
            mid = as_int(t['metatile'])
            k = terrain.mkey(mid, sec, n_primary)
            ce = M.ce.get(k, (0, 3))
            fixed[(t['x'], t['y'])] = k
            fixed_raw[(t['x'], t['y'])] = pexmap.blk(mid, t.get('collision', ce[0]), t.get('elevation', ce[1]))
            grid[t['y'], t['x']] = t.get('class', grid[t['y'], t['x']])
        # synthesise
        # 2x2 objects (round trees) must be complete blocks on the even grid
        ph = s.get('tree_phase', [0, 0])
        bad = set()
        for y in range(h):
            for x in range(w):
                if grid[y, x] == 'Y':
                    x0, y0 = x - (x + ph[0]) % 2, y - (y + ph[1]) % 2
                    for xx, yy in ((x0, y0), (x0 + 1, y0), (x0, y0 + 1), (x0 + 1, y0 + 1)):
                        if 0 <= xx < w and 0 <= yy < h and grid[yy, xx] != 'Y':
                            bad.add((x0, y0))
        if bad:
            self.warn(mb, 'round trees "Y" must form 2x2 blocks whose top-left is at even x,y; incomplete trees at %s'
                      % ', '.join('(%d,%d)' % b for b in sorted(bad)[:10]))
        syn = terrain.Synth(M, sec, grid, fixed=fixed, rng_seed=int(s.get('seed', 1)),
                            variety=float(s.get('variety', 0.5)), phase=tuple(ph))
        out = syn.run(sweeps=int(s.get('sweeps', 4)))
        blocks = np.zeros((h, w), np.uint16)
        for y in range(h):
            for x in range(w):
                if (x, y) in fixed_raw:
                    blocks[y, x] = fixed_raw[(x, y)]
                    continue
                k = out[y][x]
                if k is None:
                    self.err(mb, 'no metatile for class %r at (%d,%d)' % (grid[y, x], x, y))
                    continue
                c, e = M.ce.get(k, (0, 3))
                blocks[y, x] = pexmap.blk(terrain.key_to_mid(k), c, e)
        mb.synth_problems = syn.problems()
        mb.blocks = blocks
        mb.grid = grid
        bd = s.get('border', 'auto')
        if bd == 'auto':
            edge = list(grid[0, :]) + list(grid[-1, :]) + list(grid[:, 0]) + list(grid[:, -1])
            dom = collections.Counter(edge).most_common(1)[0][0]
            bd = fam.get('border_by_class', {}).get(dom, fam['border'])
        bd = [as_int(v) for v in bd]
        mb.border = np.array([pexmap.blk(v, 1, 0) for v in bd], np.uint16).reshape(2, 2)
        mb.layout_id = 'LAYOUT_' + snake_upper(mb.name)
        mb.layout_entry = {
            'id': mb.layout_id, 'name': mb.name + '_Layout', 'width': w, 'height': h,
            'primary_tileset': prim, 'secondary_tileset': sec,
            'border_filepath': 'data/layouts/%s/border.bin' % mb.name,
            'blockdata_filepath': 'data/layouts/%s/map.bin' % mb.name,
            'layout_version': 'emerald',
        }
        if (w + 15) * (h + 14) > MAX_MAP_DATA_SIZE:
            self.err(mb, 'map too big: (w+15)*(h+14) = %d > %d' % ((w + 15) * (h + 14), MAX_MAP_DATA_SIZE))
        mb.prim, mb.sec = prim, sec

    def build_reuse(self, mb):
        s = mb.spec
        tpl = TEMPLATES.get(s.get('template', ''), {})
        if s.get('template') and not tpl:
            raise CompileError('%s: unknown template %r (known: %s)' % (mb.name, s['template'], ', '.join(TEMPLATES)))
        base = s.get('base_map') or tpl.get('base_map')
        ex = self.existing_map(base)
        if not ex:
            raise CompileError('%s: base_map %s not found' % (mb.name, base))
        bname, bj = ex
        mb.reuse_base = (bname, bj)
        mb.template = tpl
        mb.layout_id = s.get('layout', bj['layout'])
        L = self.P.layout(mb.layout_id)
        mb.prim, mb.sec = L.primary_symbol, L.secondary_symbol
        mb.blocks = L.blocks
        mb.border = L.border
        # warp geometry from the base map; classify exits vs others
        warps = bj.get('warp_events', []) or []
        dests = collections.Counter(w['dest_map'] for w in warps)
        exit_dest = None
        if 'exit_dest_of_base' in s:
            exit_dest = s['exit_dest_of_base']
        elif dests:
            # the map the base interior exits to: the most common destination that is not an indoor sibling
            cands = [d for d, _ in dests.most_common()]
            for d in cands:
                ej = self.existing_map(d)
                if ej and ej[1].get('map_type') not in ('MAP_TYPE_INDOOR',):
                    exit_dest = d
                    break
            exit_dest = exit_dest or cands[0]
        extra = s.get('base_warps', {}) or {}   # index -> {to, warp} | None (drop)
        for i, w in enumerate(warps):
            ent = {'x': int(w['x']), 'y': int(w['y']), 'elevation': int(w.get('elevation', 0))}
            key = str(i)
            if key in extra or i in extra:
                e = extra.get(key, extra.get(i))
                if e is None:
                    continue
                ent.update({'to': e['to'], 'warp': e.get('warp'), 'src': 'base warp %d' % i})
            elif w['dest_map'] == exit_dest:
                if 'exit_to' not in s:
                    raise CompileError('%s: needs exit_to: <outdoor map> (base warp %d leaves the building)'
                                       % (mb.name, i))
                ent.update({'to': s['exit_to'], 'warp': s.get('exit_warp'), 'src': 'exit'})
                mb.exit_warp_indices.append(len(mb.warps))
            else:
                role = tpl.get('role')
                if role == 'pokecenter' and s.get('upstairs'):
                    ent.update({'to': s['upstairs'], 'warp': s.get('upstairs_warp', 0), 'src': 'upstairs'})
                elif role == 'pokecenter_2f' and w['dest_map'] not in ('MAP_UNION_ROOM', 'MAP_TRADE_CENTER') \
                        and s.get('downstairs'):
                    ent.update({'to': s['downstairs'], 'warp': s.get('downstairs_warp'), 'src': 'downstairs'})
                elif w['dest_map'] in ('MAP_UNION_ROOM', 'MAP_TRADE_CENTER', 'MAP_RECORD_CORNER'):
                    ent.update({'to': w['dest_map'], 'warp': w['dest_warp_id'], 'src': 'cable club'})
                else:
                    self.warn(mb, 'base warp %d of %s (-> %s) dropped; map it with base_warps: {%d: {to: ..}}'
                              % (i, bname, w['dest_map'], i))
                    continue
            mb.warps.append(ent)

    # ------------------------------------------------------------------ phase 3: events
    def build_events(self, mb):
        s = mb.spec
        sc = mb.scripts
        objs = []
        bgs = []
        coords = []
        # template specific
        role = getattr(mb, 'template', {}).get('role') if mb.reuse_base else None
        if role == 'pokecenter':
            heal_id = s.get('heal_location', 'HEAL_LOCATION_' + snake_upper(s.get('exit_to', mb.name)))
            nurse_lid = 'LOCALID_' + snake_upper(mb.name) + '_NURSE'
            bj = mb.reuse_base[1]
            nurse = None
            for o in bj.get('object_events', []):
                if o.get('graphics_id') == 'OBJ_EVENT_GFX_NURSE':
                    nurse = o
            nx, ny = (int(nurse['x']), int(nurse['y'])) if nurse else (7, 2)
            lab = sc.label('Nurse')
            objs.append({'local_id': nurse_lid, 'graphics_id': 'OBJ_EVENT_GFX_NURSE', 'x': nx, 'y': ny, 'elevation': 3,
                         'movement_type': 'MOVEMENT_TYPE_FACE_DOWN', 'movement_range_x': 0, 'movement_range_y': 0,
                         'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0',
                         'script': lab, 'flag': '0'})
            sc.blocks.append('%s::\n\tsetvar VAR_0x800B, %s\n\tcall Common_EventScript_PkmnCenterNurse\n'
                             '\twaitmessage\n\twaitbuttonpress\n\trelease\n\tend\n' % (lab, nurse_lid))
            ot = '%s_OnTransition' % mb.name
            sc.map_scripts.append(('MAP_SCRIPT_ON_TRANSITION', ot))
            sc.map_scripts.append(('MAP_SCRIPT_ON_RESUME', 'CableClub_OnResume'))
            sc.blocks.append('%s:\n\tsetrespawn %s\n\tend\n' % (ot, heal_id))
            mb.heal = {'id': heal_id, 'nurse': nurse_lid}
        elif role == 'pokecenter_2f':
            for t in ['MAP_SCRIPT_ON_FRAME_TABLE:CableClub_OnFrame', 'MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE:CableClub_OnWarp',
                      'MAP_SCRIPT_ON_LOAD:CableClub_OnLoad', 'MAP_SCRIPT_ON_TRANSITION:CableClub_OnTransition']:
                a, b = t.split(':')
                sc.map_scripts.append((a, b))
            for o in mb.reuse_base[1].get('object_events', []):
                if str(o.get('script', '')).startswith(('Common_', 'CableClub_')):
                    objs.append(copy.deepcopy(o))
        elif role == 'mart':
            items = s.get('mart_items', ['ITEM_POKE_BALL', 'ITEM_POTION', 'ITEM_ANTIDOTE', 'ITEM_PARALYZE_HEAL',
                                         'ITEM_AWAKENING', 'ITEM_ESCAPE_ROPE', 'ITEM_REPEL'])
            for it in items:
                self.need_const(mb, it, 'item')
            clerk = None
            for o in mb.reuse_base[1].get('object_events', []):
                if o.get('graphics_id') == 'OBJ_EVENT_GFX_MART_EMPLOYEE':
                    clerk = o
            cx, cy = (int(clerk['x']), int(clerk['y'])) if clerk else (1, 3)
            lab = sc.label('Clerk')
            objs.append({'graphics_id': 'OBJ_EVENT_GFX_MART_EMPLOYEE', 'x': cx, 'y': cy, 'elevation': 3,
                         'movement_type': 'MOVEMENT_TYPE_FACE_RIGHT', 'movement_range_x': 0, 'movement_range_y': 0,
                         'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0',
                         'script': lab, 'flag': '0'})
            sc.blocks.append('%s::\n\tlock\n\tfaceplayer\n\tmessage gText_HowMayIServeYou\n\twaitmessage\n'
                             '\tpokemart %s_Pokemart\n\tmsgbox gText_PleaseComeAgain, MSGBOX_DEFAULT\n\trelease\n\tend\n\n'
                             '\t.align 2\n%s_Pokemart:\n%s\n\tpokemartlistend\n'
                             % (lab, mb.name, mb.name, '\n'.join('\t.2byte %s' % i for i in items)))
        if s.get('copy_base_events') and mb.reuse_base:
            bj = mb.reuse_base[1]
            objs.extend(copy.deepcopy(bj.get('object_events', [])))
            bgs.extend(copy.deepcopy(bj.get('bg_events', [])))
            mb.notes.append('copied base events: their scripts belong to %s' % mb.reuse_base[0])

        # NPCs
        for i, n in enumerate(s.get('npcs', []) or []):
            objs.append(self.make_object(mb, n, i))
        for i, t in enumerate(s.get('trainers', []) or []):
            objs.append(self.make_trainer(mb, t, i))
        for i, it in enumerate(s.get('items', []) or []):
            objs.append(self.make_item(mb, it, i))
        for i, g in enumerate(s.get('signs', []) or []):
            if 'script' in g:
                lab = g['script']
            else:
                lab = sc.label(g.get('name', 'Sign%d' % (i + 1)))
                tl = sc.text(g.get('name', 'Sign%d' % (i + 1)), g['text'])
                sc.blocks.append('%s::\n\tmsgbox %s, MSGBOX_SIGN\n\tend\n' % (lab, tl))
            bgs.append({'type': 'sign', 'x': int(g['x']), 'y': int(g['y']), 'elevation': int(g.get('elevation', 0)),
                        'player_facing_dir': g.get('facing', 'BG_EVENT_PLAYER_FACING_ANY'), 'script': lab})
        for c in s.get('coord_events', []) or []:
            coords.append({'type': 'trigger', 'x': int(c['x']), 'y': int(c['y']), 'elevation': int(c.get('elevation', 3)),
                           'var': c['var'], 'var_value': str(c.get('value', c.get('var_value', 0))), 'script': c['script']})
        for b in s.get('bg_events', []) or []:
            bgs.append(b)
        for w in s.get('warps', []) or []:
            mb.warps.append({'x': int(w['x']), 'y': int(w['y']), 'elevation': int(w.get('elevation', 0)),
                             'to': w['to'], 'warp': w.get('warp'), 'src': 'warps'})
        if s.get('scripts'):
            sc.raw.append(str(s['scripts']))
        for ms in s.get('map_scripts', []) or []:
            sc.map_scripts.append((ms['type'], ms['script']))
        if len(objs) > 64:
            self.err(mb, '%d object events (max 64)' % len(objs))
        mb.objects, mb.bgs, mb.coords = objs, bgs, coords

    def _gfx(self, g):
        g = str(g)
        return g if g.startswith('OBJ_EVENT_GFX_') else 'OBJ_EVENT_GFX_' + g.upper()

    def _move(self, m):
        m = str(m or 'face_down')
        return m if m.startswith('MOVEMENT_TYPE_') else MOVES.get(m.lower(), 'MOVEMENT_TYPE_' + m.upper())

    def make_object(self, mb, n, i):
        sc = mb.scripts
        name = n.get('name', 'Npc%d' % (i + 1))
        gfx = self._gfx(n.get('gfx', 'BOY_1'))
        self.need_const(mb, gfx, 'graphics')
        mv = self._move(n.get('move', 'face_down'))
        self.need_const(mb, mv, 'movement')
        rng = n.get('range', [1, 1] if 'wander' in mv.lower() or 'WALK' in mv else [0, 0])
        if 'script' in n:
            lab = n['script']
        elif 'text' in n:
            lab = sc.label(name)
            tl = sc.text(name, n['text'])
            sc.blocks.append('%s::\n\tmsgbox %s, MSGBOX_NPC\n\tend\n' % (lab, tl))
        else:
            lab = '0x0'
        o = {'graphics_id': gfx, 'x': int(n['x']), 'y': int(n['y']), 'elevation': int(n.get('elevation', 3)),
             'movement_type': mv, 'movement_range_x': int(rng[0]), 'movement_range_y': int(rng[1]),
             'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0',
             'script': lab, 'flag': n.get('flag', '0')}
        if n.get('local_id'):
            o = dict({'local_id': n['local_id']}, **o)
        if n.get('flag', '0') != '0':
            self.need_const(mb, n['flag'], 'flag')
        return o

    def make_trainer(self, mb, t, i):
        sc = mb.scripts
        name = t.get('name', 'Trainer%d' % (i + 1))
        tid = t['trainer']
        self.need_const(mb, tid, 'trainer')
        gfx = self._gfx(t.get('gfx', 'YOUNGSTER'))
        self.need_const(mb, gfx, 'graphics')
        mv = self._move(t.get('move', 'face_down'))
        if 'script' in t:
            lab = t['script']
        else:
            lab = sc.label(name)
            ti = sc.text(name + 'Intro', t.get('intro', 'Hey! Our eyes met, so we battle!'))
            td = sc.text(name + 'Defeat', t.get('defeat', 'I lost...'))
            ta = sc.text(name + 'PostBattle', t.get('after', 'You are strong!'))
            sc.blocks.append('%s::\n\ttrainerbattle_single %s, %s, %s\n\tmsgbox %s, MSGBOX_AUTOCLOSE\n\tend\n'
                             % (lab, tid, ti, td, ta))
        o = {'graphics_id': gfx, 'x': int(t['x']), 'y': int(t['y']), 'elevation': int(t.get('elevation', 3)),
             'movement_type': mv, 'movement_range_x': 0, 'movement_range_y': 0,
             'trainer_type': t.get('trainer_type', 'TRAINER_TYPE_NORMAL'),
             'trainer_sight_or_berry_tree_id': str(t.get('sight', 3)),
             'script': lab, 'flag': '0'}
        return o

    def make_item(self, mb, it, i):
        sc = mb.scripts
        item = it['item']
        self.need_const(mb, item, 'item')
        flag = it.get('flag')
        if not flag:
            self.err(mb, 'item %s at (%s,%s) needs a flag (an unused FLAG_ITEM_* / FLAG_UNUSED_*)'
                     % (item, it['x'], it['y']))
            flag = '0'
        else:
            self.need_const(mb, flag, 'flag')
        lab = sc.label('Item' + snake_upper(item.replace('ITEM_', '')).title().replace('_', ''))
        sc.blocks.append('%s::\n\tfinditem %s, %d\n\tend\n' % (lab, item, int(it.get('amount', 1))))
        return {'graphics_id': 'OBJ_EVENT_GFX_ITEM_BALL', 'x': int(it['x']), 'y': int(it['y']), 'elevation': 3,
                'movement_type': 'MOVEMENT_TYPE_LOOK_AROUND', 'movement_range_x': 1, 'movement_range_y': 1,
                'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0',
                'script': lab, 'flag': flag}

    # ------------------------------------------------------------------ phase 4: warps & connections
    def resolve_warps(self):
        for mb in self.maps.values():
            mb.warp_events = []
            for w in mb.warps:
                dest_id, dmb = self.resolve_map_ref(str(w['to']))
                wid = w.get('warp')
                if wid is None:
                    if dmb is None:
                        ex = self.existing_map(dest_id)
                        if ex:
                            # first warp in the existing map that leads back here, else 0
                            cand = [i for i, ww in enumerate(ex[1].get('warp_events', []))
                                    if ww['dest_map'] == mb.map_id]
                            wid = cand[0] if cand else None
                        if wid is None:
                            self.err(mb, 'warp to existing map %s needs an explicit warp: index' % dest_id)
                            wid = 0
                    else:
                        cand = [i for i, ww in enumerate(dmb.warps)
                                if self.resolve_map_ref(str(ww['to']))[0] == mb.map_id]
                        if not cand:
                            self.err(mb, 'warp to %s: no warp in %s leads back to %s; give warp: N explicitly'
                                     % (dmb.name, dmb.name, mb.name))
                            wid = 0
                        else:
                            # pair the n-th warp to that map with the n-th warp back (double doors)
                            mine = [i for i, ww in enumerate(mb.warps) if ww['to'] == w['to'] and ww.get('warp') is None]
                            k = mine.index(mb.warps.index(w)) if mb.warps.index(w) in mine else 0
                            wid = cand[min(k, len(cand) - 1)]
                mb.warp_events.append({'x': w['x'], 'y': w['y'], 'elevation': w.get('elevation', 0),
                                       'dest_map': dest_id, 'dest_warp_id': str(wid)})

    def build_connections(self, mb):
        out = []
        for c in mb.spec.get('connections', []) or []:
            d = DIR_ALIASES.get(str(c.get('direction', c.get('dir'))).lower())
            if not d:
                self.err(mb, 'bad connection direction %r' % c)
                continue
            dest_id, dmb = self.resolve_map_ref(str(c['map']))
            off = int(c.get('offset', 0))
            out.append({'map': dest_id, 'offset': off, 'direction': d})
            # reciprocity check
            if dmb is not None:
                back = [cc for cc in (dmb.spec.get('connections') or [])
                        if self.resolve_map_ref(str(cc['map']))[0] == mb.map_id]
                if not back:
                    self.err(mb, 'connection %s -> %s has no reverse connection in %s' % (d, dmb.name, dmb.name))
                else:
                    bd = DIR_ALIASES.get(str(back[0].get('direction', back[0].get('dir'))).lower())
                    boff = int(back[0].get('offset', 0))
                    if bd != OPPOSITE[d]:
                        self.err(mb, 'connection %s -> %s but reverse is %s' % (d, dmb.name, bd))
                    if boff != -off:
                        self.err(mb, 'connection offsets must be opposite: %s->%s %d, back %d' % (mb.name, dmb.name, off, boff))
                # size sanity: shared edge
                if mb.blocks is not None and dmb.blocks is not None:
                    if d in ('up', 'down'):
                        if off >= mb.blocks.shape[1] or off + dmb.blocks.shape[1] <= 0:
                            self.err(mb, 'connection to %s does not overlap horizontally' % dmb.name)
                    else:
                        if off >= mb.blocks.shape[0] or off + dmb.blocks.shape[0] <= 0:
                            self.err(mb, 'connection to %s does not overlap vertically' % dmb.name)
        return out

    # ------------------------------------------------------------------ phase 5: map.json
    def build_json(self, mb):
        s = mb.spec
        is_reuse = mb.reuse_base is not None
        bj = mb.reuse_base[1] if is_reuse else {}
        tpl = getattr(mb, 'template', {}) if is_reuse else {}
        exit_mb = None
        if is_reuse and s.get('exit_to'):
            _, exit_mb = self.resolve_map_ref(str(s['exit_to']))
        mapsec = s.get('mapsec')
        if not mapsec:
            if exit_mb is not None and exit_mb.spec.get('mapsec'):
                mapsec = exit_mb.spec['mapsec']
            elif is_reuse and s.get('exit_to') and self.existing_map(s['exit_to']):
                mapsec = self.existing_map(s['exit_to'])[1]['region_map_section']
            else:
                mapsec = 'MAPSEC_' + snake_upper(mb.name)
        if s.get('mapsec_name') or (not self.consts.has(mapsec) and mapsec not in self.new_mapsecs):
            nm = s.get('mapsec_name') or re.sub(r'([a-z])([A-Z0-9])', r'\1 \2', mb.name).upper()
            if mapsec in self.new_mapsecs and s.get('mapsec_name') is None:
                pass
            else:
                self.new_mapsecs[mapsec] = nm
        music = s.get('music') or tpl.get('music') or (exit_mb.spec.get('music') if exit_mb else None) \
            or bj.get('music') or 'MUS_LITTLEROOT'
        self.need_const(mb, music, 'music')
        mt = s.get('map_type', 'indoor' if is_reuse else ('route' if 'route' in mb.name.lower() else 'town'))
        mt = MAP_TYPES.get(mt, mt)
        indoor = mt in ('MAP_TYPE_INDOOR',)
        j = collections.OrderedDict()
        j['id'] = mb.map_id
        j['name'] = mb.name
        j['layout'] = mb.layout_id
        j['music'] = music
        j['region'] = s.get('build_region', 'REGION_HOENN')
        if s.get('region'):
            j['mapgen_region'] = s['region']
        j['region_map_section'] = mapsec
        j['requires_flash'] = bool(s.get('requires_flash', False))
        j['weather'] = s.get('weather', 'WEATHER_NONE' if indoor or mt == 'MAP_TYPE_UNDERGROUND' else 'WEATHER_SUNNY')
        j['map_type'] = mt
        j['allow_cycling'] = bool(s.get('allow_cycling', not indoor))
        j['allow_escaping'] = bool(s.get('allow_escaping', mt == 'MAP_TYPE_UNDERGROUND'))
        j['allow_running'] = bool(s.get('allow_running', True))
        j['show_map_name'] = bool(s.get('show_map_name', not indoor))
        if s.get('floor_number') is not None:
            j['floor_number'] = s['floor_number']
        j['battle_scene'] = s.get('battle_scene', 'MAP_BATTLE_SCENE_NORMAL')
        conns = self.build_connections(mb)
        j['connections'] = conns if conns else None
        j['object_events'] = mb.objects
        j['warp_events'] = mb.warp_events
        j['coord_events'] = mb.coords
        j['bg_events'] = mb.bgs
        mb.json = j

    # ------------------------------------------------------------------ validation helpers
    def validate(self, mb):
        b = mb.blocks
        if b is None:
            return
        h, w = b.shape
        coll = (b >> 10) & 3

        def inside(x, y):
            return 0 <= x < w and 0 <= y < h
        for o in mb.objects:
            x, y = int(o['x']), int(o['y'])
            if not inside(x, y):
                self.err(mb, 'object %s at (%d,%d) outside map' % (o['graphics_id'], x, y))
            elif coll[y, x] and o['graphics_id'] not in ('OBJ_EVENT_GFX_NURSE', 'OBJ_EVENT_GFX_MART_EMPLOYEE'):
                self.warn(mb, 'object %s at (%d,%d) stands on an impassable tile' % (o['graphics_id'], x, y))
        seen = {}
        for o in mb.objects:
            k = (int(o['x']), int(o['y']))
            if k in seen:
                self.warn(mb, 'two objects on (%d,%d)' % k)
            seen[k] = o
        for i, wv in enumerate(mb.warp_events):
            x, y = wv['x'], wv['y']
            if not inside(x, y):
                self.err(mb, 'warp %d at (%d,%d) outside map' % (i, x, y))
                continue
            mid = int(b[y, x]) & 0x3FF
            _, _, beh, _ = self.P.metatile_info(mb.prim, mb.sec, mid)
            bn = self.P.behaviors.get(beh, '?')
            if mb.reuse_base is None and bn not in stamplib.DOOR_BEHAVIORS:
                self.warn(mb, 'warp %d at (%d,%d) is on %s (not a door/ladder/arrow tile): the player will '
                              'only warp when walking onto it if it is a door-like behavior' % (i, x, y, bn))
            if mb.reuse_base is None and bn in ('MB_ANIMATED_DOOR', 'MB_NON_ANIMATED_DOOR') and inside(x, y + 1) \
                    and coll[y + 1, x]:
                self.err(mb, 'door warp %d at (%d,%d): the tile below is impassable, player cannot exit' % (i, x, y))
        for g in mb.bgs:
            x, y = int(g['x']), int(g['y'])
            if not inside(x, y):
                self.err(mb, 'bg event at (%d,%d) outside map' % (x, y))
        # reachability from the first door / connection edge
        if mb.reuse_base is None and mb.warp_events:
            start = None
            for wv in mb.warp_events:
                if inside(wv['x'], wv['y'] + 1) and not coll[wv['y'] + 1, wv['x']]:
                    start = (wv['x'], wv['y'] + 1)
                    break
            if start:
                reach = self.flood(mb, start)
                for i, wv in enumerate(mb.warp_events):
                    tgt = (wv['x'], wv['y'] + 1)
                    if inside(*tgt) and tgt not in reach:
                        self.warn(mb, 'warp %d at (%d,%d) is not reachable on foot from warp 0' % (i, wv['x'], wv['y']))
                for o in mb.objects:
                    p = (int(o['x']), int(o['y']))
                    nbrs = [(p[0] + dx, p[1] + dy) for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))]
                    if not any(q in reach for q in nbrs) and p not in reach:
                        self.warn(mb, 'object at %s cannot be reached / talked to' % (p,))
                for c in mb.json.get('connections') or []:
                    d = c['direction']
                    edge = {'up': [(x, 0) for x in range(w)], 'down': [(x, h - 1) for x in range(w)],
                            'left': [(0, y) for y in range(h)], 'right': [(w - 1, y) for y in range(h)]}[d]
                    if not any(e in reach for e in edge):
                        self.warn(mb, 'no walkable tile on the %s edge reachable from the doors' % d)
        for (x, y, d) in mb.synth_problems[:40]:
            pass
        if mb.synth_problems:
            self.warn(mb, '%d tile seams never seen in real maps (check the render): %s'
                      % (len(mb.synth_problems), ', '.join('(%d,%d)%s' % p for p in mb.synth_problems[:12])))

    def flood(self, mb, start):
        b = mb.blocks
        h, w = b.shape
        coll = (b >> 10) & 3
        seen = {start}
        todo = [start]
        jumps = {}
        while todo:
            x, y = todo.pop()
            for dx, dy, dname in ((1, 0, 'E'), (-1, 0, 'W'), (0, 1, 'S'), (0, -1, 'N')):
                xx, yy = x + dx, y + dy
                if not (0 <= xx < w and 0 <= yy < h) or (xx, yy) in seen:
                    continue
                if coll[yy, xx]:
                    # ledges: jump over in the right direction
                    _, _, beh, _ = self.P.metatile_info(mb.prim, mb.sec, int(b[yy, xx]) & 0x3FF)
                    bn = self.P.behaviors.get(beh, '')
                    if (bn == 'MB_JUMP_SOUTH' and dname == 'S') or (bn == 'MB_JUMP_NORTH' and dname == 'N') or \
                            (bn == 'MB_JUMP_EAST' and dname == 'E') or (bn == 'MB_JUMP_WEST' and dname == 'W'):
                        x2, y2 = xx + dx, yy + dy
                        if 0 <= x2 < w and 0 <= y2 < h and not coll[y2, x2] and (x2, y2) not in seen:
                            seen.add((x2, y2))
                            todo.append((x2, y2))
                    continue
                seen.add((xx, yy))
                todo.append((xx, yy))
        return seen

    # ------------------------------------------------------------------ phase 6: write
    def write_all(self):
        root = self.root
        lj_path = os.path.join(root, 'data/layouts/layouts.json')
        lj = json.load(open(lj_path))
        ids = {e.get('id'): i for i, e in enumerate(lj['layouts'])}
        for mb in self.maps.values():
            if mb.layout_entry is None:
                continue
            d = os.path.join(root, 'data/layouts', mb.name)
            os.makedirs(d, exist_ok=True)
            mb.blocks.astype('<u2').tofile(os.path.join(d, 'map.bin'))
            mb.border.astype('<u2').tofile(os.path.join(d, 'border.bin'))
            if mb.layout_id in ids:
                lj['layouts'][ids[mb.layout_id]] = mb.layout_entry
            else:
                lj['layouts'].append(mb.layout_entry)
                ids[mb.layout_id] = len(lj['layouts']) - 1
        write_json(lj_path, lj)

        # maps + scripts
        for mb in self.maps.values():
            d = os.path.join(root, 'data/maps', mb.name)
            os.makedirs(d, exist_ok=True)
            write_json(os.path.join(d, 'map.json'), mb.json)
            with open(os.path.join(d, 'scripts.inc'), 'w') as f:
                f.write(mb.scripts.render())

        # aggregate scripts include
        agg = os.path.join(root, AGGREGATE_INC)
        existing = []
        if os.path.exists(agg):
            existing = [l.strip() for l in open(agg) if l.strip().startswith('.include')]
        want = list(existing)
        for mb in self.maps.values():
            line = '.include "data/maps/%s/scripts.inc"' % mb.name
            if line not in want:
                want.append(line)
        with open(agg, 'w') as f:
            f.write('@ Map scripts of maps generated by tools/mapgen (one line per map).\n')
            for l in want:
                f.write('\t%s\n' % l)
        es = os.path.join(root, EVENT_SCRIPTS)
        txt = open(es).read()
        inc = '\t.include "%s"\n' % AGGREGATE_INC
        if inc not in txt:
            anchor = re.findall(r'\t\.include "data/maps/[^"]+/scripts\.inc"\n', txt)[-1]
            pos = txt.rindex(anchor) + len(anchor)
            txt = txt[:pos] + inc + txt[pos:]
            open(es, 'w').write(txt)

        # map groups
        mg_path = os.path.join(root, 'data/maps/map_groups.json')
        mg = json.load(open(mg_path), object_pairs_hook=collections.OrderedDict)
        for mb in self.maps.values():
            g = mb.spec.get('group')
            if not g:
                raise CompileError('%s: missing group (e.g. group: Johto / JohtoIndoor)' % mb.name)
            g = g if g.startswith('gMapGroup_') else 'gMapGroup_' + g
            for k in mg['group_order']:
                if k != g and mb.name in mg.get(k, []):
                    mg[k].remove(mb.name)
            if g not in mg['group_order']:
                mg['group_order'].append(g)
                mg[g] = []
            if mb.name not in mg[g]:
                mg[g].append(mb.name)
        write_json(mg_path, mg)

        # region map sections
        if self.new_mapsecs:
            rp = os.path.join(root, 'src/data/region_map/region_map_sections.json')
            rj = json.load(open(rp), object_pairs_hook=collections.OrderedDict)
            have = {e['id']: e for e in rj['map_sections']}
            for k, nm in self.new_mapsecs.items():
                if k in have:
                    have[k]['name'] = nm
                else:
                    rj['map_sections'].append(collections.OrderedDict([('id', k), ('name', nm)]))
            if len(rj['map_sections']) + 1 > 0xFD:
                raise CompileError('too many MAPSECs (%d): the u8 space ends at 0xFC' % len(rj['map_sections']))
            write_json(rp, rj)

        # heal locations
        heals = [mb for mb in self.maps.values() if mb.heal]
        if heals:
            hp = os.path.join(root, 'src/data/heal_locations.json')
            hj = json.load(open(hp), object_pairs_hook=collections.OrderedDict)
            have = {e['id']: i for i, e in enumerate(hj['heal_locations'])}
            for mb in heals:
                exit_ref = mb.spec.get('exit_to')
                emb = self.maps.get(exit_ref)
                if emb is None:
                    self.err(mb, 'pokecenter exit_to must be a map compiled in the same run')
                    continue
                # outdoor door that leads into this PC: heal spot = tile below the door
                doors = [w for w in emb.warp_events if w['dest_map'] == mb.map_id]
                if not doors:
                    self.err(mb, 'no door in %s leads to %s' % (emb.name, mb.name))
                    continue
                ent = collections.OrderedDict([
                    ('id', mb.heal['id']), ('map', emb.map_id), ('x', doors[0]['x']), ('y', doors[0]['y'] + 1),
                    ('respawn_map', mb.map_id), ('respawn_npc', mb.heal['nurse'])])
                if ent['id'] in have:
                    hj['heal_locations'][have[ent['id']]] = ent
                else:
                    hj['heal_locations'].append(ent)
            write_json(hp, hj)

        # wild encounters
        wilds = [mb for mb in self.maps.values() if mb.spec.get('wild')]
        if wilds:
            wp = os.path.join(root, 'src/data/wild_encounters.json')
            wj = json.load(open(wp), object_pairs_hook=collections.OrderedDict)
            grp = wj['wild_encounter_groups'][0]
            fields = {f['type']: f for f in grp['fields']}
            for mb in wilds:
                ent = collections.OrderedDict([('map', mb.map_id), ('base_label', 'g' + mb.name)])
                for k, v in mb.spec['wild'].items():
                    t = WILD_ALIASES.get(k, k)
                    if t not in WILD_SLOTS:
                        self.err(mb, 'unknown wild table %s' % k)
                        continue
                    mons = []
                    for m in v['mons']:
                        if isinstance(m, dict):
                            sp, lo, hi = m['species'], m.get('min_level', m.get('level')), m.get('max_level', m.get('level'))
                        else:
                            sp, lo, hi = m[0], m[1], m[2] if len(m) > 2 else m[1]
                        sp = sp if sp.startswith('SPECIES_') else 'SPECIES_' + sp.upper()
                        self.need_const(mb, sp, 'species')
                        mons.append(collections.OrderedDict([('min_level', int(lo)), ('max_level', int(hi)),
                                                             ('species', sp)]))
                    n = WILD_SLOTS[t]
                    if len(mons) != n:
                        if len(mons) > n:
                            self.err(mb, '%s has %d mons, needs exactly %d' % (t, len(mons), n))
                        else:
                            # repeat to fill the slots (rates: %s)
                            base = list(mons)
                            while len(mons) < n:
                                mons.append(copy.deepcopy(base[len(mons) % len(base)]))
                            self.warn(mb, '%s: only %d species given, cycled to fill %d slots (rates %s)'
                                      % (t, len(base), n, fields[t]['encounter_rates']))
                    ent[t] = collections.OrderedDict([('encounter_rate', int(v.get('rate', 20))), ('mons', mons)])
                encs = grp['encounters']
                idx = [i for i, e in enumerate(encs) if e.get('map') == mb.map_id]
                if idx:
                    encs[idx[0]] = ent
                else:
                    encs.append(ent)
            write_json(wp, wj)

        # new trainers
        if self.new_trainers:
            self.write_trainers()

        # patches to existing maps
        for p in self.patches:
            self.apply_patch(p)

    def write_trainers(self):
        root = self.root
        op = os.path.join(root, 'include/constants/opponents.h')
        txt = open(op).read()
        m = re.search(r'#define TRAINERS_COUNT_EMERALD\s+(\d+)', txt)
        count = int(m.group(1))
        mx = int(re.search(r'#define MAX_TRAINERS_COUNT_EMERALD\s+(\d+)', txt).group(1))
        party_p = os.path.join(root, 'src/data/trainers.party')
        party = open(party_p).read()
        for t in self.new_trainers:
            tid = t['id']
            if re.search(r'#define %s\s' % tid, txt):
                num = None
            else:
                if count >= mx:
                    raise CompileError('no free trainer ids (MAX_TRAINERS_COUNT_EMERALD=%d)' % mx)
                anchor = '\n// NOTE: Because each Trainer uses a flag'
                line = '#define %-35s %d\n' % (tid, count)
                txt = txt.replace(anchor, '\n' + line.rstrip('\n') + anchor, 1) if anchor in txt else txt
                count += 1
            block = self.trainer_party_text(t)
            pat = re.compile(r'=== %s ===\n.*?(?=\n=== |\Z)' % re.escape(tid), re.S)
            if pat.search(party):
                party = pat.sub(block.rstrip('\n'), party)
            else:
                party = party.rstrip('\n') + '\n\n' + block
        txt = re.sub(r'(#define TRAINERS_COUNT_EMERALD\s+)\d+', r'\g<1>%d' % count, txt)
        open(op, 'w').write(txt)
        open(party_p, 'w').write(party)

    def trainer_party_text(self, t):
        lines = ['=== %s ===' % t['id'],
                 'Name: %s' % t.get('name', 'TRAINER').upper(),
                 'Class: %s' % t.get('class', 'Youngster'),
                 'Pic: %s' % t.get('pic', t.get('class', 'Youngster')),
                 'Gender: %s' % t.get('gender', 'Male'),
                 'Music: %s' % t.get('music', t.get('gender', 'Male')),
                 'Double Battle: No',
                 'AI: Check Bad Move', '']
        for p in t['party']:
            sp = p['species']
            lines.append(sp if not sp.startswith('SPECIES_') else sp)
            lines.append('Level: %d' % int(p['level']))
            for mv in p.get('moves', []):
                lines.append('- %s' % mv)
            lines.append('')
        return '\n'.join(lines) + '\n'

    def apply_patch(self, p):
        name = p['patch_map']
        path = self.P.map_json_path(name)
        j = json.load(open(path), object_pairs_hook=collections.OrderedDict)
        for sw in p.get('set_warps', []) or []:
            i = int(sw['index'])
            dest_id, dmb = self.resolve_map_ref(str(sw['to']))
            wid = sw.get('warp')
            if wid is None and dmb is not None:
                cand = [k for k, w in enumerate(dmb.warp_events) if w['dest_map'] == j['id']]
                wid = cand[0] if cand else 0
            j['warp_events'][i]['dest_map'] = dest_id
            j['warp_events'][i]['dest_warp_id'] = str(wid)
        for c in p.get('add_connections', []) or []:
            d = DIR_ALIASES[str(c.get('direction', c.get('dir'))).lower()]
            dest_id, _ = self.resolve_map_ref(str(c['map']))
            conns = [cc for cc in (j.get('connections') or []) if cc['map'] != dest_id]
            conns.append(collections.OrderedDict([('map', dest_id), ('offset', int(c.get('offset', 0))), ('direction', d)]))
            j['connections'] = conns
        write_json(path, j)

    # ------------------------------------------------------------------ render
    def render(self, mb):
        if mb.blocks is None:
            return None
        os.makedirs(self.render_dir, exist_ok=True)
        img = pexmap.render_blocks(self.P, mb.prim, mb.sec, mb.blocks, scale=2, coords=True,
                                   border=mb.border, border_pad=2 if mb.reuse_base is None else 0)
        pexmap.draw_events(img, mb.json, scale=2, pad=2 if mb.reuse_base is None else 0)
        p = os.path.join(self.render_dir, mb.name + '.png')
        img.save(p)
        if mb.reuse_base is None:
            img2 = pexmap.render_blocks(self.P, mb.prim, mb.sec, mb.blocks, scale=1, collision=True, grid=True)
            img2.save(os.path.join(self.render_dir, mb.name + '_collision.png'))
        return p

    # ------------------------------------------------------------------ driver
    def run(self):
        order = list(self.maps.values())
        for mb in order:
            try:
                self.build_layout(mb)
            except CompileError as e:
                self.errors.append(str(e))
        if self.errors:
            return False
        for mb in order:
            self.build_events(mb)
        try:
            self.resolve_warps()
        except CompileError as e:
            self.errors.append(str(e))
            return False
        for mb in order:
            self.build_json(mb)
            self.validate(mb)
        renders = []
        for mb in order:
            p = self.render(mb)
            if p:
                renders.append(p)
        self.renders = renders
        if self.errors:
            return False
        if not self.check:
            self.write_all()
        return not self.errors


def write_json(path, data):
    with open(path, 'w') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
        f.write('\n')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('specs', nargs='+')
    ap.add_argument('--root', default=DEFAULT_ROOT)
    ap.add_argument('--render', default=DEFAULT_RENDER)
    ap.add_argument('--check', action='store_true', help='do not write into the tree')
    a = ap.parse_args()
    C = Compiler(a.root, check=a.check, render_dir=a.render)
    specs = []
    for p in a.specs:
        specs.extend(load_spec_file(p))
    try:
        C.add_specs(specs)
        ok = C.run()
    except CompileError as e:
        C.errors.append(str(e))
        ok = False
    for w in C.warnings:
        print('WARNING', w)
    for e in C.errors:
        print('ERROR  ', e)
    for r in getattr(C, 'renders', []):
        print('render ', r)
    if ok:
        print('OK: %d map(s) %s' % (len(C.maps), 'checked' if a.check else 'written to ' + C.root))
        if not a.check:
            print('next: cd %s && make -j2' % C.root)
    sys.exit(0 if ok else 1)


if __name__ == '__main__':
    main()
