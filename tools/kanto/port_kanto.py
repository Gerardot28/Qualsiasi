#!/usr/bin/env python3
"""
port_kanto.py - build FRLG Kanto maps (shipped with pokeemerald-expansion 1.17.x) into the EMERALD build,
with brand-new stub scripts instead of the FireRed/LeafGreen story scripts.

Idempotent: run it again with the same (or a bigger) selection and it only does what is still missing.
Never touches maps that are not selected (except: re-linking warps/connections of maps ported earlier).

Examples
    # proof of concept set (+ the indoor maps those maps warp into)
    python3 port_kanto.py /path/to/tree --maps PalletTown_Frlg Route1_Frlg ViridianCity_Frlg \\
        PewterCity_Frlg Route22_Frlg --warp-depth 1
    # every Kanto map, without the Sevii Islands
    python3 port_kanto.py /path/to/tree --all --no-sevii
    # see what would happen
    python3 port_kanto.py /path/to/tree --all --dry-run

Map selectors (--maps): map folder name (PalletTown_Frlg), map constant (MAP_PALLET_TOWN) or
a whole map group (gMapGroup_IndoorPewter_Frlg).

What it changes in the tree (all edits are marked with the string KANTO_PORT):
  engine (once):
    include/constants/global.h          KANTO_IN_EMERALD switch
    tools/mapjson/mapjson.cpp           "include_in_emerald" for maps/layouts + map-group index placeholders
    src/data/tilesets/{headers,graphics,metatiles}.h   FRLG tilesets also in Emerald
    src/data/object_events/*.h, src/event_object_movement.c   FRLG overworld sprites + palettes
    src/field_door.c                    FRLG door animations
    tools/wild_encounters/wild_encounters_to_header.py   FireRed (or LeafGreen) Kanto encounters in Emerald
    data/event_scripts.s                includes data/kanto_port_scripts.inc (generated list of stub scripts)
    src/field_specials.c, src/field_effect.c, src/field_control_avatar.c, data/event_scripts.s
                                        (unless --no-runtime-patches) PC on/off tiles, PokeCenter monitor
                                        sprite and FRLG "flavor text" metatiles chosen at runtime per map
  per ported map:
    data/maps/<Map>/map.json            "include_in_emerald": true, events sanitized (see below)
    data/maps/<Map>/scripts.inc         new stub scripts (original kept as scripts_frlg_orig.inc)
    data/layouts/layouts.json           "include_in_emerald": true on the layouts used
Event sanitizing (first port of a map only; details are written as comments into the stub scripts.inc):
  * every object/sign script -> <Map>_EventScript_<Name> stub (placeholder msgbox; nurses -> Emerald nurse)
  * FRLG-only object flags -> "0", trainers -> TRAINER_TYPE_NONE (stub scripts are not trainerbattle)
  * coord triggers and hidden items removed (they need FRLG vars/flags that do not exist / alias Emerald ones)
Linking (every run, for every ported map):
  * warps to maps that are not built are pointed at themselves (original kept in kanto_port_orig_dest_*)
  * connections / clone objects pointing at maps that are not built are parked in kanto_port_dropped_*
  and restored automatically once the target map is ported.
"""
import argparse
import json
import os
import re
import sys

MARK = 'KANTO_PORT'
KEEP_SCRIPTS = {
    # shared Emerald scripts that work as-is on Kanto maps
    'EventScript_RockSmash', 'EventScript_StrengthBoulder', 'EventScript_CutTree',
    'Common_EventScript_UnionRoomAttendant', 'Common_EventScript_WirelessClubAttendant',
    'Common_EventScript_DirectCornerAttendant',
}
NO_SCRIPT = {'0x0', '0', '', 'NULL'}
NURSE_GFX = {'OBJ_EVENT_GFX_NURSE_FRLG', 'OBJ_EVENT_GFX_NURSE'}
LINK_GROUP = 'gMapGroup_Link_Frlg'
SEVII_FALLBACK = {
    'MAPSEC_ONE_ISLAND', 'MAPSEC_TWO_ISLAND', 'MAPSEC_THREE_ISLAND', 'MAPSEC_KINDLE_ROAD',
    'MAPSEC_TREASURE_BEACH', 'MAPSEC_CAPE_BRINK', 'MAPSEC_BOND_BRIDGE', 'MAPSEC_THREE_ISLE_PORT',
    'MAPSEC_MT_EMBER', 'MAPSEC_BERRY_FOREST', 'MAPSEC_THREE_ISLE_PATH', 'MAPSEC_EMBER_SPA',
}

dry_run = False
changed_files = []


# ----------------------------------------------------------------------------------------------- io
pending = {}  # path -> new text (lets --dry-run apply several patches to the same file in memory)


def read(path):
    if path in pending:
        return pending[path]
    with open(path, encoding='utf-8') as f:
        return f.read()


def write(path, text):
    old = read(path) if (path in pending or os.path.exists(path)) else None
    if old == text:
        return False
    changed_files.append(path)
    pending[path] = text
    if not dry_run:
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, 'w', encoding='utf-8', newline='\n') as f:
            f.write(text)
    return True


def dump_json(data):
    return json.dumps(data, indent=2, ensure_ascii=False) + '\n'


def die(msg):
    print('ERROR: ' + msg, file=sys.stderr)
    sys.exit(1)


# ------------------------------------------------------------------------------------------ patches
def patch_replace(path, old, new, marker, what):
    """Replace exactly one occurrence of `old` by `new`, unless `marker` already present."""
    text = read(path)
    if marker in text:
        return
    if text.count(old) != 1:
        die('%s: pattern for "%s" found %d times (tree version mismatch?)' % (path, what, text.count(old)))
    write(path, text.replace(old, new))
    print('  patched %s (%s)' % (os.path.relpath(path), what))


def split_frlg_guards(path):
    """'#if !IS_FRLG ... #else ... #endif' -> emerald block + '#if IS_FRLG || KANTO_IN_EMERALD' block;
       '#if IS_FRLG' -> '#if IS_FRLG || KANTO_IN_EMERALD'."""
    lines = read(path).split('\n')
    if any(MARK in l for l in lines):
        return
    out = []
    stack = []  # one entry per open #if: 'em' (inside #if !IS_FRLG), 'other'
    n = 0
    for line in lines:
        s = line.strip()
        if re.match(r'#\s*if(n?def)?\b', s):
            if re.match(r'#\s*if\s+!\s*IS_FRLG\s*$', s):
                stack.append('em')
            elif re.match(r'#\s*if\s+IS_FRLG\s*(//.*)?$', s):
                stack.append('other')
                line = '#if IS_FRLG || KANTO_IN_EMERALD // %s' % MARK
                n += 1
            else:
                stack.append('other')
        elif re.match(r'#\s*else\b', s) and stack and stack[-1] == 'em':
            out.append('#endif // !IS_FRLG')
            line = '#if IS_FRLG || KANTO_IN_EMERALD // %s' % MARK
            stack[-1] = 'other'
            n += 1
        elif re.match(r'#\s*endif\b', s):
            if stack:
                stack.pop()
        out.append(line)
    if n == 0:
        die('%s: no IS_FRLG guard found' % path)
    write(path, '\n'.join(out))
    print('  patched %s (%d FRLG guard(s) opened)' % (os.path.relpath(path), n))


def patch_engine(root, wild_version, runtime_patches):
    p = lambda rel: os.path.join(root, rel)
    print('engine patches:')
    # 1. switch
    patch_replace(p('include/constants/global.h'),
                  '#define GAME_LANGUAGE (LANGUAGE_ENGLISH)\n',
                  '#define GAME_LANGUAGE (LANGUAGE_ENGLISH)\n\n'
                  '// KANTO_PORT: also build FRLG tilesets, overworld sprites, door animations and the Kanto maps\n'
                  '// flagged "include_in_emerald" into the Emerald build (see tools/kanto/port_kanto.py).\n'
                  '#define KANTO_IN_EMERALD 1\n',
                  MARK, 'KANTO_IN_EMERALD switch')

    # 2. mapjson
    mj = p('tools/mapjson/mapjson.cpp')
    patch_replace(mj,
                  '''        if ((version == "emerald" && region != "REGION_HOENN")
         || (version == "firered" && region != "REGION_KANTO")) {
            invalid_maps.push_back(map_name);''',
                  '''        // KANTO_PORT: maps with "include_in_emerald": true are built in emerald too
        bool include_in_emerald = json_to_string(map_data, "include_in_emerald", true) == "TRUE";
        if ((version == "emerald" && region != "REGION_HOENN" && !include_in_emerald)
         || (version == "firered" && region != "REGION_KANTO")) {
            invalid_maps.push_back(map_name);''',
                  'maps with "include_in_emerald": true are built', 'map filter')
    patch_replace(mj,
                  '''        if (valid_maps.size() > 0) {
            text << group << "::\\n";
            for (string map : valid_maps)
                text << "\\t.4byte " << map << "\\n";''',
                  '''        if (valid_maps.size() > 0) {
            text << group << "::\\n";
            // KANTO_PORT: MAP_* constants number every map of the group, so a map that is not built must
            // still occupy its slot, otherwise every later map of a partially built group is off by one.
            for (Json &map_name : maps) {
                string map_name_str = json_to_string(map_name);
                if (find(invalid_maps.begin(), invalid_maps.end(), map_name_str) == invalid_maps.end())
                    text << "\\t.4byte " << map_name_str << "\\n";
                else
                    text << "\\t.4byte " << valid_maps[0] << " @ placeholder, not built: " << map_name_str << "\\n";
            }''',
                  'must\n            // still occupy its slot', 'map group index placeholders')
    patch_replace(mj,
                  '''        if ((version == "emerald" && layout_version != "emerald")
         || (version == "firered" && layout_version != "frlg"))
            continue;''',
                  '''        if ((version == "emerald" && layout_version != "emerald"
             && json_to_string(layout, "include_in_emerald", true) != "TRUE") // KANTO_PORT
         || (version == "firered" && layout_version != "frlg"))
            continue;''',
                  '"include_in_emerald", true) != "TRUE") // KANTO_PORT\n         || (version == "firered" && layout_version != "frlg"))\n            continue;',
                  'layout headers')
    patch_replace(mj,
                  '''        if ((version == "emerald" && layout_version != "emerald") || (version == "firered" && layout_version != "frlg")) {''',
                  '''        if ((version == "emerald" && layout_version != "emerald" && json_to_string(layout, "include_in_emerald", true) != "TRUE") // KANTO_PORT
         || (version == "firered" && layout_version != "frlg")) {''',
                  '"TRUE") // KANTO_PORT\n         || (version == "firered" && layout_version != "frlg")) {',
                  'layout table')

    # 3. FRLG data blocks
    for rel in ('src/data/tilesets/headers.h', 'src/data/tilesets/graphics.h', 'src/data/tilesets/metatiles.h',
                'src/data/object_events/object_event_graphics_info_pointers.h',
                'src/data/object_events/object_event_pic_tables.h',
                'src/data/object_events/object_event_graphics_info.h',
                'src/data/object_events/object_event_graphics.h',
                'src/event_object_movement.c', 'src/field_door.c'):
        split_frlg_guards(p(rel))

    # 4. wild encounters: emit the FireRed (or LeafGreen) Kanto tables in the emerald build too
    we = p('tools/wild_encounters/wild_encounters_to_header.py')
    text = read(we)
    if MARK not in text:
        guard_re = re.compile(r'^(\s*)self\.WriteLine\(f"#ifdef \{version\}"\)$', re.M)
        if len(guard_re.findall(text)) != 2:
            die(we + ': wild encounter version guard not found')
        cond = '"FIRERED"' if wild_version == 'firered' else '"LEAFGREEN"'
        text = guard_re.sub(r'\1self.WriteLine(kanto_port_guard(version))', text)
        helper = ('\n# KANTO_PORT: Kanto encounter tables of one FRLG version are also built into Emerald\n'
                  'KANTO_PORT_EMERALD_USES = %s\n\n'
                  'def kanto_port_guard(version):\n'
                  '    if version == KANTO_PORT_EMERALD_USES:\n'
                  '        return f"#if defined({version}) || defined(EMERALD)"\n'
                  '    return f"#ifdef {version}"\n\n' % cond)
        idx = text.index('\nclass ')
        text = text[:idx] + '\n' + helper + text[idx:]
        write(we, text)
        print('  patched %s (Kanto encounters: %s)' % (os.path.relpath(we), wild_version))

    # 5. event scripts include list
    patch_replace(p('data/event_scripts.s'),
                  '\t.include "data/scripts/std_msgbox.inc"\n',
                  '.if !IS_FRLG\n@ KANTO_PORT: stub scripts of the Kanto maps built into Emerald (generated list)\n'
                  '\t.include "data/kanto_port_scripts.inc"\n.endif\n\n'
                  '\t.include "data/scripts/std_msgbox.inc"\n',
                  'data/kanto_port_scripts.inc', 'stub script list')

    if not runtime_patches:
        return
    # 6. runtime "is this a FRLG layout" instead of compile-time IS_FRLG for map-dependent visuals
    fs = p('src/field_specials.c')
    text = read(fs)
    if MARK not in text:
        n0 = text.count('IS_FRLG')
        for old, new in (
            ('static bool32 IsBuildingPCTile(u32 tileId)\n{\n    if (IS_FRLG)',
             'static bool32 IsBuildingPCTile(u32 tileId)\n{\n    if (IS_FRLG || gMapHeader.mapLayout->isFrlg) // KANTO_PORT'),
            ('static bool32 IsBuildingPCTileFrlg(u32 tileId)\n{\n    if (IS_FRLG)',
             'static bool32 IsBuildingPCTileFrlg(u32 tileId)\n{\n    if (IS_FRLG || gMapHeader.mapLayout->isFrlg)'),
            ('static bool32 IsPlayerHousePCTile(u32 tileId)\n{\n    if (IS_FRLG)',
             'static bool32 IsPlayerHousePCTile(u32 tileId)\n{\n    if (IS_FRLG || gMapHeader.mapLayout->isFrlg)'),
            ('static bool32 IsPlayerHousePCTileFrlg(u32 tileId)\n{\n    if (IS_FRLG)',
             'static bool32 IsPlayerHousePCTileFrlg(u32 tileId)\n{\n    if (IS_FRLG || gMapHeader.mapLayout->isFrlg)'),
        ):
            if text.count(old) != 1:
                die(fs + ': PC tile pattern not found')
            text = text.replace(old, new)
        for old in ('metatileId = IS_FRLG ? METATILE_BuildingFrlg_PCOff : METATILE_Building_PC_Off;',
                    'metatileId = IS_FRLG ? METATILE_BuildingFrlg_PCOn : METATILE_Building_PC_On;'):
            if old not in text:
                die(fs + ': PC metatile pattern not found')
            text = text.replace(old, old.replace('IS_FRLG ?', '(IS_FRLG || gMapHeader.mapLayout->isFrlg) ?'))
        write(fs, text)
        print('  patched %s (PC on/off tiles per map, %d IS_FRLG uses before)' % (os.path.relpath(fs), n0))
    patch_replace(p('src/field_effect.c'),
                  '    struct Sprite *sprite;\n    if (IS_FRLG)\n    {\n        spriteId = CreateSpriteAtEnd(&sSpriteTemplate_PokecenterMonitor_FrLg',
                  '    struct Sprite *sprite;\n    if (IS_FRLG || gMapHeader.mapLayout->isFrlg) // KANTO_PORT\n    {\n        spriteId = CreateSpriteAtEnd(&sSpriteTemplate_PokecenterMonitor_FrLg',
                  MARK, 'PokeCenter monitor sprite per map')
    fca = p('src/field_control_avatar.c')
    text = read(fca)
    if MARK not in text:
        old = '    if (IS_FRLG)\n    {\n        if (MetatileBehavior_IsFood(metatileBehavior) == TRUE)'
        if text.count(old) != 1:
            die(fca + ': flavor text block not found')
        text = text.replace(old, '    if (IS_FRLG || gMapHeader.mapLayout->isFrlg) // KANTO_PORT\n    {\n'
                                 '        if (MetatileBehavior_IsFood(metatileBehavior) == TRUE)')
        # these three need FRLG-only scripts (trainer tower / FRLG cable club): keep them FRLG-build only
        for name in ('TrainerTower_EventScript_ShowTime', 'CableClub_EventScript_ShowWirelessCommunicationScreen_Frlg',
                     'CableClub_EventScript_ShowBattleRecords_Frlg'):
            o = '            return %s;' % name
            if text.count(o) != 1:
                die(fca + ': ' + name + ' not found')
            text = text.replace(o, '            return IS_FRLG ? %s : NULL;' % name)
        write(fca, text)
        print('  patched %s (FRLG flavor-text metatiles on Kanto maps)' % os.path.relpath(fca))
    patch_replace(p('data/event_scripts.s'),
                  '.if !IS_FRLG\n@ KANTO_PORT: stub scripts',
                  '.if !IS_FRLG\n\t.include "data/scripts/flavor_text.inc" @ KANTO_PORT_FLAVOR\n.endif\n\n'
                  '.if !IS_FRLG\n@ KANTO_PORT: stub scripts',
                  'KANTO_PORT_FLAVOR', 'flavor text scripts')


def patch_test_start(root, spec):
    """Test only: new game starts directly on a Kanto map with a Pokemon in the party."""
    mapid, x, y = spec.split(',')
    ng = os.path.join(root, 'src/new_game.c')
    patch_replace(ng,
                  'static void WarpToTruck(void)\n{\n',
                  'static void WarpToTruck(void)\n{\n'
                  '#if KANTO_PORT_TEST_START // KANTO_PORT test only\n'
                  '    ScriptGiveMon(SPECIES_PIKACHU, 15, ITEM_NONE);\n'
                  '    FlagSet(FLAG_SYS_POKEMON_GET);\n'
                  '    FlagSet(FLAG_SYS_B_DASH);\n'
                  '    SetWarpDestination(MAP_GROUP(%s), MAP_NUM(%s), WARP_ID_NONE, %s, %s);\n'
                  '    WarpIntoMap();\n'
                  '    return;\n'
                  '#endif\n' % (mapid, mapid, x, y),
                  'KANTO_PORT_TEST_START', 'test start')
    patch_replace(ng, '#include "follower_npc.h"\n',
                  '#include "follower_npc.h"\n#include "script_pokemon_util.h" // KANTO_PORT test\n',
                  'script_pokemon_util.h" // KANTO_PORT', 'test start include')
    patch_replace(os.path.join(root, 'src/overworld.c'),
                  '    if (IS_FRLG)\n        gFieldCallback = FieldCB_WarpExitFadeFromBlack;',
                  '    if (IS_FRLG || KANTO_PORT_TEST_START) // KANTO_PORT test only\n        gFieldCallback = FieldCB_WarpExitFadeFromBlack;',
                  'KANTO_PORT_TEST_START', 'test start field callback')
    patch_replace(os.path.join(root, 'include/constants/global.h'),
                  '#define KANTO_IN_EMERALD 1\n',
                  '#define KANTO_IN_EMERALD 1\n#define KANTO_PORT_TEST_START 1 // test only: new game starts in Kanto\n',
                  'KANTO_PORT_TEST_START', 'test start switch')


# ---------------------------------------------------------------------------------------- map model
class Tree:
    def __init__(self, root):
        self.root = root
        self.maps_dir = os.path.join(root, 'data', 'maps')
        self.groups_path = os.path.join(self.maps_dir, 'map_groups.json')
        self.groups = json.loads(read(self.groups_path))
        self.maps = {}
        self.group_of = {}
        self.order = []
        for g in self.groups['group_order']:
            for name in self.groups[g]:
                self.maps[name] = json.loads(read(self.map_json(name)))
                self.group_of[name] = g
                self.order.append(name)
        self.id2name = {d['id']: n for n, d in self.maps.items()}
        self.layouts_path = os.path.join(root, 'data', 'layouts', 'layouts.json')
        self.layouts = json.loads(read(self.layouts_path))
        self.heal = json.loads(read(os.path.join(root, 'src', 'data', 'heal_locations.json')))['heal_locations']
        self.sevii_mapsecs = self._read_sevii_mapsecs()

    def map_json(self, name):
        return os.path.join(self.maps_dir, name, 'map.json')

    def is_kanto(self, name):
        return self.maps[name].get('region') == 'REGION_KANTO'

    def is_ported(self, name):
        return self.maps[name].get('include_in_emerald') is True

    def is_built(self, name):
        region = self.maps[name].get('region', 'REGION_HOENN')
        return region == 'REGION_HOENN' or self.is_ported(name)

    def _read_sevii_mapsecs(self):
        path = os.path.join(self.root, 'src', 'regions.c')
        try:
            text = read(path)
            secs = set(re.findall(r'\b(MAPSEC_\w+)', text.split('[KANTO_SUBREGION_SEVII123]', 1)[1]))
            secs.discard('MAPSEC_NONE')
            if secs:
                return secs
        except (OSError, IndexError):
            pass
        return SEVII_FALLBACK

    def is_sevii(self, name):
        return self.maps[name].get('region_map_section') in self.sevii_mapsecs

    def save_map(self, name):
        write(self.map_json(name), dump_json(self.maps[name]))


def resolve_selection(tree, args):
    sel = []
    if args.all:
        for n in tree.order:
            if not tree.is_kanto(n):
                continue
            if tree.group_of[n] == LINK_GROUP and not args.include_link:
                continue
            if args.no_sevii and tree.is_sevii(n):
                continue
            sel.append(n)
    for token in args.maps or []:
        if token in tree.maps:
            names = [token]
        elif token in tree.id2name:
            names = [tree.id2name[token]]
        elif token in tree.groups:
            names = list(tree.groups[token])
        elif token + '_Frlg' in tree.maps:
            names = [token + '_Frlg']
        else:
            die('unknown map/group: ' + token)
        for n in names:
            if not tree.is_kanto(n):
                die('%s is not a REGION_KANTO map' % n)
            sel.append(n)
    # optional: follow warps into the maps they lead to
    frontier = list(dict.fromkeys(sel))
    for _ in range(args.warp_depth):
        nxt = []
        for n in frontier:
            for w in tree.maps[n].get('warp_events') or []:
                dest = w.get('kanto_port_orig_dest_map', w.get('dest_map'))
                dn = tree.id2name.get(dest)
                if dn and tree.is_kanto(dn) and dn not in sel:
                    if args.no_sevii and tree.is_sevii(dn):
                        continue
                    sel.append(dn)
                    nxt.append(dn)
        frontier = nxt
    return list(dict.fromkeys(sel))


# ------------------------------------------------------------------------------------- sanitizing
def stub_prefix(name):
    return name  # folder name (ends with _Frlg) -> never collides with Emerald or other Kanto labels


def label_suffix(label, idx, kind):
    m = re.search(r'_EventScript_(\w+)$', label or '')
    if m:
        return m.group(1)
    return '%s%d' % (kind, idx)


def text_for(name, suffix, kind):
    disp = name[:-5] if name.endswith('_Frlg') else name
    return '%s\\n%s %s\\p(Kanto placeholder text)$' % (disp, kind, suffix)


def sanitize_and_stub(tree, name, force_stubs=False):
    """First port of a map: rewrite events, write stub scripts.inc. Returns list of notes."""
    d = tree.maps[name]
    pre = stub_prefix(name)
    stubs = []         # (label, kind, comment)
    notes = []
    used = {}

    def new_label(orig, idx, kind):
        if orig in used:
            return used[orig]
        lab = '%s_EventScript_%s' % (pre, label_suffix(orig, idx, kind))
        base, k = lab, 2
        while any(lab == s[0] for s in stubs):
            lab = '%s_%d' % (base, k)
            k += 1
        used[orig] = lab
        return lab

    objs = []
    for i, o in enumerate(d.get('object_events') or []):
        t = o.get('type', 'object')
        if t == 'clone':
            objs.append(o)
            continue
        info = []
        flag = str(o.get('flag', '0'))
        if flag not in ('0', '') and not flag.startswith('FLAG_TEMP_'):
            info.append('flag %s -> 0' % flag)
            o['flag'] = '0'
        tt = o.get('trainer_type', 'TRAINER_TYPE_NONE')
        if tt != 'TRAINER_TYPE_NONE':
            info.append('trainer_type %s (sight %s) -> NONE' % (tt, o.get('trainer_sight_or_berry_tree_id')))
            o['trainer_type'] = 'TRAINER_TYPE_NONE'
        script = str(o.get('script', '0x0'))
        if script not in NO_SCRIPT and script not in KEEP_SCRIPTS:
            gfx = o.get('graphics_id', '')
            kind = 'nurse' if gfx in NURSE_GFX else ('item' if gfx == 'OBJ_EVENT_GFX_ITEM_BALL' else 'npc')
            lab = new_label(script, i + 1, 'Obj')
            if not any(lab == s[0] for s in stubs):
                lid = o.get('local_id') or str(i + 1)
                stubs.append((lab, kind, 'object %d %s at (%s,%s), was %s%s' % (
                    i + 1, gfx, o.get('x'), o.get('y'), script, ('; ' + '; '.join(info)) if info else ''), lid))
            o['script'] = lab
        elif info:
            notes.append('object %d %s at (%s,%s): %s' % (i + 1, o.get('graphics_id'), o.get('x'), o.get('y'), '; '.join(info)))
        objs.append(o)
    d['object_events'] = objs

    bgs = []
    for i, b in enumerate(d.get('bg_events') or []):
        if b.get('type') == 'hidden_item':
            notes.append('removed hidden item %s at (%s,%s) flag %s' % (b.get('item'), b.get('x'), b.get('y'), b.get('flag')))
            continue
        script = str(b.get('script', '0x0'))
        if b.get('type') == 'sign' and script not in NO_SCRIPT and script not in KEEP_SCRIPTS:
            lab = new_label(script, i + 1, 'Sign')
            if not any(lab == s[0] for s in stubs):
                stubs.append((lab, 'sign', 'sign at (%s,%s), was %s' % (b.get('x'), b.get('y'), script), None))
            b['script'] = lab
        bgs.append(b)
    d['bg_events'] = bgs

    coords = []
    for c in d.get('coord_events') or []:
        if c.get('type') == 'trigger':
            notes.append('removed trigger at (%s,%s) %s == %s -> %s' % (c.get('x'), c.get('y'), c.get('var'), c.get('var_value'), c.get('script')))
            continue
        coords.append(c)
    d['coord_events'] = coords
    d['include_in_emerald'] = True

    # stub scripts.inc
    sdir = os.path.join(tree.maps_dir, name)
    spath = os.path.join(sdir, 'scripts.inc')
    opath = os.path.join(sdir, 'scripts_frlg_orig.inc')
    if os.path.exists(opath) and not force_stubs:
        return notes  # stubs already generated once: never overwrite (may contain new content)
    if os.path.exists(spath) and not os.path.exists(opath):
        write(opath, read(spath))
    respawn = [h['id'] for h in tree.heal if h.get('respawn_map') == d['id']]
    out = ['@ %s: generated by tools/kanto/port_kanto.py (%s stub).' % (name, MARK),
           '@ Original FRLG scripts kept for reference in scripts_frlg_orig.inc (not built).',
           '@ This file is never overwritten by the port script once it exists: write the new content here.', '']
    if notes:
        out.append('@ Events changed/removed by the port:')
        out += ['@   ' + n for n in notes]
        out.append('')
    out.append('%s_MapScripts::' % name)
    if respawn:
        out.append('\tmap_script MAP_SCRIPT_ON_TRANSITION, %s_OnTransition' % name)
    out.append('\t.byte 0')
    out.append('')
    if respawn:
        out += ['%s_OnTransition:' % name, '\tsetrespawn %s' % respawn[0], '\tend', '']
    texts = []
    for lab, kind, comment, lid in stubs:
        out.append('@ ' + comment)
        out.append('%s::' % lab)
        if kind == 'nurse':
            out += ['\tsetvar VAR_0x800B, %s' % lid, '\tcall Common_EventScript_PkmnCenterNurse',
                    '\twaitmessage', '\twaitbuttonpress', '\trelease', '\tend', '']
            continue
        tlab = lab.replace('_EventScript_', '_Text_')
        box = 'MSGBOX_SIGN' if kind == 'sign' else 'MSGBOX_NPC'
        out += ['\tmsgbox %s, %s' % (tlab, box), '\tend', '']
        texts.append((tlab, text_for(name, lab.split('_EventScript_')[-1], {'sign': 'Sign', 'item': 'Item', 'npc': 'NPC'}[kind])))
    for tlab, txt in texts:
        out += ['%s:' % tlab, '\t.string "%s"' % txt, '']
    write(spath, '\n'.join(out))
    return notes


def link_map(tree, name):
    """Point warps/connections/clones only at maps that exist in the Emerald build (restore when they do)."""
    d = tree.maps[name]
    before = json.dumps(d, sort_keys=True)
    self_id = d['id']
    built_ids = lambda mid: (mid in ('MAP_DYNAMIC', 'MAP_UNDEFINED', 'MAP_NONE')
                             or (mid in tree.id2name and tree.is_built(tree.id2name[mid])))
    for i, w in enumerate(d.get('warp_events') or []):
        orig = w.get('kanto_port_orig_dest_map', w.get('dest_map'))
        orig_id = w.get('kanto_port_orig_dest_warp_id', w.get('dest_warp_id'))
        if built_ids(orig):
            if 'kanto_port_orig_dest_map' in w:
                w['dest_map'], w['dest_warp_id'] = orig, orig_id
                del w['kanto_port_orig_dest_map']
                del w['kanto_port_orig_dest_warp_id']
        else:
            w['kanto_port_orig_dest_map'], w['kanto_port_orig_dest_warp_id'] = orig, orig_id
            w['dest_map'], w['dest_warp_id'] = self_id, str(i)
    conns = (d.get('connections') or []) + d.pop('kanto_port_dropped_connections', [])
    keep = [c for c in conns if built_ids(c['map'])]
    drop = [c for c in conns if not built_ids(c['map'])]
    d['connections'] = keep
    if drop:
        d['kanto_port_dropped_connections'] = drop
    objs = d.get('object_events') or []
    parked = d.pop('kanto_port_dropped_clones', [])
    new_objs = [o for o in objs if not (o.get('type') == 'clone' and not built_ids(o.get('target_map')))]
    dropped = [o for o in objs if o.get('type') == 'clone' and not built_ids(o.get('target_map'))]
    restored = [o for o in parked if built_ids(o.get('target_map'))]
    parked = [o for o in parked if not built_ids(o.get('target_map'))] + dropped
    d['object_events'] = new_objs + restored
    if parked:
        d['kanto_port_dropped_clones'] = parked
    return json.dumps(d, sort_keys=True) != before


# -------------------------------------------------------------------------------------------- main
def main():
    global dry_run
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('tree', help='pokeemerald-expansion tree to modify')
    ap.add_argument('--maps', nargs='*', help='maps / map constants / map groups to port')
    ap.add_argument('--all', action='store_true', help='port every REGION_KANTO map')
    ap.add_argument('--no-sevii', action='store_true', help='with --all/--warp-depth: skip Sevii Islands maps')
    ap.add_argument('--include-link', action='store_true', help='with --all: also the FRLG link rooms group')
    ap.add_argument('--warp-depth', type=int, default=0, help='also port maps reachable through N levels of warps')
    ap.add_argument('--wild-version', choices=('firered', 'leafgreen'), default='firered',
                    help='which FRLG encounter tables Emerald uses for Kanto (default firered)')
    ap.add_argument('--no-runtime-patches', action='store_true',
                    help='skip the optional per-map runtime tweaks (PC tiles, PokeCenter monitor, flavor text)')
    ap.add_argument('--force-stubs', action='store_true',
                    help='regenerate stub scripts.inc even if they exist (DESTROYS edits made to them)')
    ap.add_argument('--test-start', metavar='MAP_ID,X,Y',
                    help='TEST ONLY: new game starts on this map with a Pikachu (do not use on the main tree)')
    ap.add_argument('--dry-run', action='store_true')
    args = ap.parse_args()
    dry_run = args.dry_run
    root = os.path.abspath(args.tree)
    if not os.path.exists(os.path.join(root, 'tools', 'mapjson', 'mapjson.cpp')):
        die('not a pokeemerald-expansion tree: ' + root)

    tree = Tree(root)
    selection = resolve_selection(tree, args)
    if not selection and not any(tree.is_ported(n) for n in tree.order):
        die('nothing selected (use --maps or --all)')

    patch_engine(root, args.wild_version, not args.no_runtime_patches)
    if args.test_start:
        patch_test_start(root, args.test_start)

    new = [n for n in selection if not tree.is_ported(n)]
    print('maps selected: %d (new: %d, already ported: %d)' % (len(selection), len(new), len(selection) - len(new)))
    report = {}
    for n in selection:
        if not tree.is_ported(n) or args.force_stubs:
            report[n] = sanitize_and_stub(tree, n, args.force_stubs)
            tree.save_map(n)
    ported = [n for n in tree.order if tree.is_ported(n)]
    for n in ported:
        if link_map(tree, n):
            tree.save_map(n)

    # layouts
    used_layouts = {tree.maps[n]['layout'] for n in ported}
    lay_changed = False
    for lay in tree.layouts['layouts']:
        if lay.get('id') in used_layouts and lay.get('layout_version') == 'frlg' and lay.get('include_in_emerald') is not True:
            lay['include_in_emerald'] = True
            lay_changed = True
    if lay_changed:
        write(tree.layouts_path, dump_json(tree.layouts))

    # stub script list
    lines = ['@ Generated by tools/kanto/port_kanto.py (KANTO_PORT) - do not edit by hand', '']
    lines += ['\t.include "data/maps/%s/scripts.inc"' % n for n in ported]
    write(os.path.join(root, 'data', 'kanto_port_scripts.inc'), '\n'.join(lines) + '\n')

    # summary
    dangling = []
    for n in ported:
        d = tree.maps[n]
        for w in d.get('warp_events') or []:
            if 'kanto_port_orig_dest_map' in w:
                dangling.append('%s: warp (%s,%s) -> %s (not built, points to itself)' % (n, w['x'], w['y'], w['kanto_port_orig_dest_map']))
        for c in d.get('kanto_port_dropped_connections', []):
            dangling.append('%s: connection %s -> %s parked' % (n, c['direction'], c['map']))
        for c in d.get('kanto_port_dropped_clones', []):
            dangling.append('%s: clone of %s/%s parked' % (n, c.get('target_map'), c.get('target_local_id')))
    groups_partial = sorted({tree.group_of[n] for n in ported
                             if any(not tree.is_ported(m) for m in tree.groups[tree.group_of[n]])})
    summary = {
        'ported_maps': ported,
        'newly_ported': new,
        'layouts_flagged': sorted(used_layouts),
        'partially_built_groups': groups_partial,
        'dangling_references': dangling,
        'event_notes': report,
        'changed_files': sorted(set(os.path.relpath(f, root) for f in changed_files)),
    }
    if not dry_run:
        write(os.path.join(root, 'data', 'kanto_port_report.json'), dump_json(summary))
    print('ported maps total: %d, layouts: %d, partially built groups: %d' % (len(ported), len(used_layouts), len(groups_partial)))
    print('dangling references (redirected/parked): %d' % len(dangling))
    for line in dangling[:40]:
        print('   ' + line)
    if len(dangling) > 40:
        print('   ... (see data/kanto_port_report.json)')
    print('%s %d file(s)' % ('would change' if dry_run else 'changed', len(set(changed_files))))


if __name__ == '__main__':
    main()
