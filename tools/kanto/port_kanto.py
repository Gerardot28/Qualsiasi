#!/usr/bin/env python3
"""
port_kanto.py - build the FRLG Kanto maps shipped with pokeemerald-expansion 1.17.x into the EMERALD build
(Pokemon Multiverse, Act 2), with new stub scripts instead of the FireRed/LeafGreen story scripts.

Idempotent and tree-independent: run it on any pokeemerald-expansion 1.17.x tree, as often as needed;
it only does what is still missing and never overwrites stub scripts.inc files once generated.

    python3 port_kanto.py --tree /home/user/pex --all            # every mainland Kanto map (253) + travel link
    python3 port_kanto.py --tree /home/user/pex --all --dry-run  # show what would change
    python3 port_kanto.py --tree T --maps PalletTown_Frlg Route1_Frlg --warp-depth 1   # partial port

--all = mainland Kanto (towns, routes, buildings, dungeons, S.S. Anne, Indigo Plateau/League); the Sevii
Islands, Trainer Tower, Navel Rock/Birth Island and the link rooms only with --include-sevii (link: never).
Map selectors (--maps): folder name (PalletTown_Frlg), map constant (MAP_PALLET_TOWN) or map group.

What it changes (every edit is marked KANTO_PORT; see docs/kanto/README.md for the full list):
  engine (once): include/constants/global.h (KANTO_IN_EMERALD), tools/mapjson/mapjson.cpp
    ("include_in_emerald" maps/layouts), FRLG tilesets/object sprites/door animations also in Emerald,
    tools/wild_encounters (FireRed Kanto tables in Emerald), data/event_scripts.s (one include),
    include/constants/flags.h (+ generated flags_kanto.h: 0x200 saved Kanto flags after the Emerald ones),
    src/data/region_map/region_map_sections.json (Italian names of the Kanto sections, if still English),
    optional runtime tweaks (PC tiles, PokeCenter monitor, FRLG flavor-text metatiles on Kanto maps)
  per map: map.json ("include_in_emerald", events made Emerald-valid, originals kept in "kanto_port_orig"),
    scripts.inc (stub; original kept in scripts_frlg_orig.inc), layouts.json (layout flagged)
  generated: data/kanto_port_scripts.inc (script list + KantoPort_EventScript_InitFlags),
    data/kanto_port_report.json, docs/kanto/mappe.json (inventory, --inventory)
  travel link: data/scripts/kanto_travel.inc (written once) + one hook line in LilycoveCity_Harbor/scripts.inc
Event rules (first port of a map):
  * object/sign/trigger scripts -> <Map>_EventScript_<Name> stubs (nurses: Emerald nurse; mart clerks:
    pokemart with the original FRLG item list; Vermilion ferry sailor: travel link back to Hoenn)
  * item balls -> Common_EventScript_FindItem with the original item; hidden items keep their item
  * FRLG flags (object hide flags, item balls, hidden items) -> FLAG_KANTO_<name> in the Kanto flag block
  * trainers -> TRAINER_TYPE_NONE (original type/sight + TRAINER_ constant in kanto_port_orig / inventory)
  * coord triggers kept but disabled (VAR_TEMP_F == 0x7FFF): FRLG scene vars alias Emerald vars here
  * FRLG "Pokemon Journal" objects (gfx 0) -> signs
Linking (every run): warps to maps that are not built point at themselves (original kept in
kanto_port_orig_dest_*); connections/clones to maps not built are parked and restored once they are.
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


# ------------------------------------------------------------------------------------- kanto data
# Mainland Kanto = every map whose region map section is one of these (Sevii Islands, Trainer Tower,
# Navel Rock, Birth Island and the link rooms are left out). Value = Italian name of the section.
MAINLAND_MAPSECS = {
    'MAPSEC_PALLET_TOWN': 'Biancavilla', 'MAPSEC_VIRIDIAN_CITY': 'Smeraldopoli', 'MAPSEC_PEWTER_CITY': 'Plumbeopoli',
    'MAPSEC_CERULEAN_CITY': 'Celestopoli', 'MAPSEC_LAVENDER_TOWN': 'Lavandonia', 'MAPSEC_VERMILION_CITY': 'Aranciopoli',
    'MAPSEC_CELADON_CITY': 'Azzurropoli', 'MAPSEC_FUCHSIA_CITY': 'Fucsiapoli', 'MAPSEC_CINNABAR_ISLAND': 'Isola Cannella',
    'MAPSEC_INDIGO_PLATEAU': 'Altopiano Blu', 'MAPSEC_SAFFRON_CITY': 'Zafferanopoli',
    'MAPSEC_VIRIDIAN_FOREST': 'Bosco Smeraldo', 'MAPSEC_MT_MOON': 'Monte Luna', 'MAPSEC_S_S_ANNE': 'M/N Anna',
    'MAPSEC_UNDERGROUND_PATH': 'Via Sotterranea', 'MAPSEC_UNDERGROUND_PATH_2': 'Via Sotterranea',
    'MAPSEC_DIGLETTS_CAVE': 'Grotta Diglett', 'MAPSEC_KANTO_VICTORY_ROAD': 'Via Vittoria',
    'MAPSEC_ROCKET_HIDEOUT': 'Rifugio Rocket', 'MAPSEC_SILPH_CO': 'Silph SpA', 'MAPSEC_POKEMON_MANSION': 'Villa Pokémon',
    'MAPSEC_KANTO_SAFARI_ZONE': 'Zona Safari', 'MAPSEC_CERULEAN_CAVE': 'Grotta Celeste',
    'MAPSEC_POKEMON_LEAGUE': 'Lega Pokémon', 'MAPSEC_ROCK_TUNNEL': 'Tunnel Roccioso',
    'MAPSEC_SEAFOAM_ISLANDS': 'Isole Spumarine', 'MAPSEC_POKEMON_TOWER': 'Torre Pokémon',
    'MAPSEC_POWER_PLANT': 'Centrale Elett.',
}
MAINLAND_MAPSECS.update({'MAPSEC_ROUTE_%d' % i: 'Percorso %d' % i for i in range(1, 26)})
DUNGEON_MAPSECS = {
    'MAPSEC_VIRIDIAN_FOREST', 'MAPSEC_MT_MOON', 'MAPSEC_UNDERGROUND_PATH', 'MAPSEC_UNDERGROUND_PATH_2',
    'MAPSEC_DIGLETTS_CAVE', 'MAPSEC_KANTO_VICTORY_ROAD', 'MAPSEC_ROCKET_HIDEOUT', 'MAPSEC_SILPH_CO',
    'MAPSEC_POKEMON_MANSION', 'MAPSEC_KANTO_SAFARI_ZONE', 'MAPSEC_CERULEAN_CAVE', 'MAPSEC_ROCK_TUNNEL',
    'MAPSEC_SEAFOAM_ISLANDS', 'MAPSEC_POKEMON_TOWER', 'MAPSEC_POWER_PLANT',
}
FLAGS_HEADER = 'include/constants/flags_kanto.h'
KANTO_FLAGS_COUNT = 0x200          # 64 bytes of SaveBlock1 (flags[] grows by this many bits)
FLAG_INIT_DONE = 'FLAG_KANTO_FLAGS_INITIALIZED'
STUB_VERSION = 2
TRAVEL_FILE = 'data/scripts/kanto_travel.inc'
VERMILION_ARRIVAL = (24, 32)       # facing the Vermilion harbor ferry sailor (VermilionCity_Frlg, 24,33)
LILYCOVE_RETURN = ('MAP_LILYCOVE_CITY_HARBOR', 8, 11)  # where the SS Tidal lets the player off
# object scripts that get a hand-written body instead of a placeholder: (map, original label) -> body lines
SPECIAL_STUBS = {
    ('VermilionCity_Frlg', 'VermilionCity_EventScript_FerrySailor'):
        ['\tgoto KantoTravel_EventScript_VermilionSailor @ KANTO_PORT travel link (data/scripts/kanto_travel.inc)'],
}


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
        self.frlg_items = self._read_frlg_item_balls()

    def p(self, rel):
        return os.path.join(self.root, rel)

    def map_json(self, name):
        return os.path.join(self.maps_dir, name, 'map.json')

    def is_kanto(self, name):
        return self.maps[name].get('region') == 'REGION_KANTO'

    def is_mainland(self, name):
        return self.is_kanto(name) and self.maps[name].get('region_map_section') in MAINLAND_MAPSECS

    def is_ported(self, name):
        return self.maps[name].get('include_in_emerald') is True

    def is_built(self, name):
        region = self.maps[name].get('region', 'REGION_HOENN')
        return region == 'REGION_HOENN' or self.is_ported(name)

    def _read_frlg_item_balls(self):
        """label -> ITEM_ from data/scripts/item_ball_scripts_frlg.inc (FRLG item ball scripts)."""
        path = self.p('data/scripts/item_ball_scripts_frlg.inc')
        if not os.path.exists(path):
            return {}
        return dict(re.findall(r'^(\w+)::\s*\n\s*finditem\s+(ITEM_\w+)\s*$', read(path), re.M))

    _shared = None

    def shared_blocks(self):
        """label -> body of the FRLG shared scripts (data/scripts/*_frlg.inc: trainers, item balls, ...)."""
        if self._shared is None:
            self._shared = {}
            sdir = self.p('data/scripts')
            for fn in sorted(os.listdir(sdir)) if os.path.isdir(sdir) else []:
                if fn.endswith('_frlg.inc'):
                    self._shared.update(parse_blocks(read(os.path.join(sdir, fn))))
        return self._shared

    def orig_blocks(self, name):
        blocks = dict(self.shared_blocks())
        blocks.update(parse_blocks(self.orig_scripts(name)))
        return blocks

    def orig_scripts(self, name):
        """Original FRLG scripts of a map (scripts_frlg_orig.inc once ported, else scripts.inc)."""
        for fn in ('scripts_frlg_orig.inc', 'scripts.inc'):
            path = os.path.join(self.maps_dir, name, fn)
            if os.path.exists(path):
                text = read(path)
                if fn == 'scripts_frlg_orig.inc' or MARK not in text:
                    return text
        return ''

    def save_map(self, name):
        write(self.map_json(name), dump_json(self.maps[name]))


def resolve_selection(tree, args):
    sel = []
    if args.all:
        for n in tree.order:
            if not tree.is_kanto(n):
                continue
            if tree.is_mainland(n) or (args.include_sevii and tree.group_of[n] != LINK_GROUP):
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
    frontier = list(dict.fromkeys(sel))
    for _ in range(args.warp_depth):
        nxt = []
        for n in frontier:
            for w in tree.maps[n].get('warp_events') or []:
                dn = tree.id2name.get(w.get('kanto_port_orig_dest_map', w.get('dest_map')))
                if dn and tree.is_kanto(dn) and dn not in sel and (tree.is_mainland(dn) or args.include_sevii):
                    sel.append(dn)
                    nxt.append(dn)
        frontier = nxt
    return list(dict.fromkeys(sel))


# ------------------------------------------------------------------------------------------ flags
class KantoFlags:
    """Append-only allocation of the Kanto saved-flag block (include/constants/flags_kanto.h).
    The index of a flag is part of the save layout: existing entries are never moved."""
    RE = re.compile(r'^#define\s+(FLAG_KANTO_\w+)\s+\(KANTO_FLAGS_START \+ 0x([0-9A-Fa-f]+)\)', re.M)

    def __init__(self, root):
        self.path = os.path.join(root, FLAGS_HEADER)
        self.names = []
        if os.path.exists(self.path):
            for name, idx in self.RE.findall(read(self.path)):
                if int(idx, 16) != len(self.names):
                    die('%s: flag indices are not contiguous at %s' % (self.path, name))
                self.names.append(name)
        self.get_name(FLAG_INIT_DONE)

    def get_name(self, name):
        if name not in self.names:
            if len(self.names) >= KANTO_FLAGS_COUNT:
                die('Kanto flag block full (%d): raise KANTO_FLAGS_COUNT' % KANTO_FLAGS_COUNT)
            self.names.append(name)
        return name

    def for_frlg(self, frlg_flag):
        return self.get_name('FLAG_KANTO_' + re.sub(r'^FLAG_', '', frlg_flag))

    def save(self):
        out = ['#ifndef GUARD_CONSTANTS_FLAGS_KANTO_H', '#define GUARD_CONSTANTS_FLAGS_KANTO_H', '',
               '// %s: generated by tools/kanto/port_kanto.py - saved flags of the Kanto maps built into Emerald.' % MARK,
               '// The block follows the Emerald flags; FLAGS_COUNT in constants/flags.h includes it, so',
               '// SaveBlock1.flags[] grows by KANTO_FLAGS_COUNT / 8 bytes.',
               '// Names are FLAG_KANTO_ + the original FRLG flag name (hide flags of objects, item balls, hidden items).',
               '// APPEND-ONLY: the index is the save layout. The script keeps existing entries and appends new ones;',
               '// story flags written by hand go after KANTO_FLAGS_FIRST_FREE (add them at the end of this list).',
               '',
               '#define KANTO_FLAGS_START (DAILY_FLAGS_END + 1)',
               '#define KANTO_FLAGS_COUNT 0x%X' % KANTO_FLAGS_COUNT,
               '#define KANTO_FLAGS_END   (KANTO_FLAGS_START + KANTO_FLAGS_COUNT - 1)', '']
        w = max(len(n) for n in self.names) + 1
        for i, n in enumerate(self.names):
            out.append('#define %s(KANTO_FLAGS_START + 0x%03X)' % (n.ljust(w), i))
        out += ['', '#define KANTO_FLAGS_FIRST_FREE (KANTO_FLAGS_START + 0x%03X) // %d of %d used' % (
            len(self.names), len(self.names), KANTO_FLAGS_COUNT), '', '#endif // GUARD_CONSTANTS_FLAGS_KANTO_H', '']
        write(self.path, '\n'.join(out))


def patch_flags_h(root):
    patch_replace(os.path.join(root, 'include/constants/flags.h'),
                  '#define FLAGS_COUNT (DAILY_FLAGS_END + 1)\n',
                  '#include "constants/flags_kanto.h" // KANTO_PORT: saved flags of the Kanto maps\n'
                  '#define FLAGS_COUNT (KANTO_FLAGS_END + 1) // KANTO_PORT: was (DAILY_FLAGS_END + 1)\n',
                  MARK, 'Kanto flag block')


# ------------------------------------------------------------------------------------- sanitizing
def label_suffix(label, idx, kind):
    m = re.search(r'_EventScript_(\w+)$', label or '')
    return m.group(1) if m else '%s%d' % (kind, idx)


def parse_blocks(text):
    """label -> script body text (until the next label)."""
    blocks, cur, buf = {}, None, []
    for line in text.split('\n'):
        m = re.match(r'^(\w+):{1,2}', line)
        if m:
            if cur:
                blocks[cur] = '\n'.join(buf)
            cur, buf = m.group(1), []
        elif cur:
            buf.append(line)
    if cur:
        blocks[cur] = '\n'.join(buf)
    return blocks


def follow(blocks, label, depth=3):
    """body of a label plus the bodies of the labels it jumps to (a few levels)."""
    seen, out, todo = set(), [], [label]
    for _ in range(depth):
        nxt = []
        for lab in todo:
            if lab in seen or lab not in blocks:
                continue
            seen.add(lab)
            out.append(blocks[lab])
            nxt += re.findall(r'\b(?:goto|call|goto_if_\w+|call_if_\w+)\b[^\n]*?\b(\w+_EventScript_\w+)', blocks[lab])
        todo = nxt
    return '\n'.join(out)


def trainer_of(blocks, label):
    m = re.search(r'trainerbattle\w*\s+(TRAINER_\w+)', follow(blocks, label, 2))
    return m.group(1) if m else None


def sanitize_map(tree, name, flags):
    """First port of a map: make its events valid in Emerald. Originals are kept in "kanto_port_orig"."""
    d = tree.maps[name]
    pre = name
    used = {}
    taken = set()

    def new_label(orig, idx, kind):
        if orig in used:
            return used[orig]
        lab = '%s_EventScript_%s' % (pre, label_suffix(orig, idx, kind))
        base, k = lab, 2
        while lab in taken:
            lab = '%s_%d' % (base, k)
            k += 1
        taken.add(lab)
        used[orig] = lab
        return lab

    objs, extra_signs = [], []
    for i, o in enumerate(d.get('object_events') or []):
        if o.get('type') == 'clone':
            objs.append(o)
            continue
        script = str(o.get('script', '0x0'))
        flag = str(o.get('flag', '0'))
        gfx = o.get('graphics_id', '')
        orig = {k: o.get(k) for k in ('graphics_id', 'script', 'flag', 'trainer_type', 'trainer_sight_or_berry_tree_id')}
        if gfx == '0' and script not in NO_SCRIPT:
            # FRLG "Pokemon Journal" (gfx 0 = player sprite, always hidden by a flag): becomes a sign
            extra_signs.append({'type': 'sign', 'x': o['x'], 'y': o['y'], 'elevation': o.get('elevation', 0),
                                'player_facing_dir': 'BG_EVENT_PLAYER_FACING_ANY', 'script': new_label(script, i + 1, 'Obj'),
                                'kanto_port_orig': dict(orig, converted_from='object')})
            continue
        o['kanto_port_orig'] = orig
        if flag not in ('0', '') and not flag.startswith('FLAG_TEMP_'):
            o['flag'] = flags.for_frlg(flag)
        if o.get('trainer_type', 'TRAINER_TYPE_NONE') != 'TRAINER_TYPE_NONE':
            o['trainer_type'] = 'TRAINER_TYPE_NONE'   # stub scripts are not trainerbattle scripts
            o['trainer_sight_or_berry_tree_id'] = '0'
        if gfx == 'OBJ_EVENT_GFX_ITEM_BALL' and script in tree.frlg_items:
            o['script'] = 'Common_EventScript_FindItem'
            o['trainer_sight_or_berry_tree_id'] = tree.frlg_items[script]
        elif script not in NO_SCRIPT and script not in KEEP_SCRIPTS:
            o['script'] = new_label(script, i + 1, 'Obj')
        objs.append(o)
    d['object_events'] = objs

    bgs = []
    for i, b in enumerate(d.get('bg_events') or []):
        if b.get('type') == 'hidden_item':
            b['kanto_port_orig'] = {'flag': b.get('flag')}
            b['flag'] = flags.for_frlg(b['flag'])
        elif b.get('type') == 'sign':
            script = str(b.get('script', '0x0'))
            if script not in NO_SCRIPT and script not in KEEP_SCRIPTS:
                b['kanto_port_orig'] = {'script': script}
                b['script'] = new_label(script, i + 1, 'Sign')
        bgs.append(b)
    d['bg_events'] = bgs + extra_signs

    for i, c in enumerate(d.get('coord_events') or []):
        if c.get('type') == 'trigger':
            # FRLG scene vars alias Emerald vars in this build: the trigger is kept but disabled
            c['kanto_port_orig'] = {'var': c.get('var'), 'var_value': c.get('var_value'), 'script': c.get('script')}
            c['var'], c['var_value'] = 'VAR_TEMP_F', '0x7FFF'
            c['script'] = new_label(str(c.get('script')), i + 1, 'Trigger')
    d['include_in_emerald'] = True
    d['kanto_port'] = STUB_VERSION


def stub_text(name, suffix, kind):
    disp = (name[:-5] if name.endswith('_Frlg') else name).replace('_', ' ')
    return '%s\\n%s %s\\p(testo Kanto da scrivere)$' % (disp, kind, suffix)


def write_stubs(tree, name, force=False):
    """Generate data/maps/<Map>/scripts.inc from the sanitized events (never overwrites unless forced)."""
    d = tree.maps[name]
    sdir = os.path.join(tree.maps_dir, name)
    spath = os.path.join(sdir, 'scripts.inc')
    opath = os.path.join(sdir, 'scripts_frlg_orig.inc')
    if os.path.exists(opath) and not force:
        return False
    orig_text = tree.orig_scripts(name)
    if not os.path.exists(opath):
        write(opath, orig_text)
    blocks = tree.orig_blocks(name)
    entries = {}  # label -> dict(kind, comments, orig)

    def add(label, kind, comment, orig_label, lid=None):
        e = entries.setdefault(label, {'kind': kind, 'comments': [], 'orig': orig_label, 'lid': lid})
        e['comments'].append(comment)

    for i, o in enumerate(d.get('object_events') or []):
        ko = o.get('kanto_port_orig')
        if not ko or not o.get('script', '').startswith(name + '_EventScript_'):
            continue
        gfx = o.get('graphics_id')
        kind = 'nurse' if gfx in NURSE_GFX else 'npc'
        info = ''
        if ko.get('trainer_type') not in (None, 'TRAINER_TYPE_NONE'):
            kind = 'trainer'
            info = '; trainer %s %s sight %s' % (trainer_of(blocks, ko['script']) or '?', ko['trainer_type'],
                                                 ko.get('trainer_sight_or_berry_tree_id'))
        elif trainer_of(blocks, ko['script']):
            info = '; battle %s' % trainer_of(blocks, ko['script'])
        if 'pokemart ' in blocks.get(ko['script'], ''):
            kind = 'mart'
        if (name, ko['script']) in SPECIAL_STUBS:
            kind = 'special'
        add(o['script'], kind, 'object %s %s at (%s,%s), was %s%s%s' % (
            o.get('local_id', i + 1), gfx, o['x'], o['y'], ko['script'],
            ' flag %s' % o['flag'] if str(o.get('flag', '0')) != '0' else '', info), ko['script'], o.get('local_id') or str(i + 1))
    for b in d.get('bg_events') or []:
        ko = b.get('kanto_port_orig')
        if b.get('type') == 'sign' and ko and b['script'].startswith(name + '_EventScript_'):
            add(b['script'], 'sign', 'sign at (%s,%s), was %s%s' % (b['x'], b['y'], ko['script'],
                ' (Pokemon Journal object)' if ko.get('converted_from') else ''), ko['script'])
    for c in d.get('coord_events') or []:
        ko = c.get('kanto_port_orig')
        if ko and c.get('type') == 'trigger':
            add(c['script'], 'trigger', 'trigger at (%s,%s), was %s == %s -> %s (DISABLED: set var/var_value in map.json)' % (
                c['x'], c['y'], ko['var'], ko['var_value'], ko['script']), ko['script'])

    respawn = [h['id'] for h in tree.heal if h.get('respawn_map') == d['id']]
    gfx_vars = sorted({o['graphics_id'] for o in d.get('object_events') or [] if o.get('graphics_id', '').startswith('OBJ_EVENT_GFX_VAR_')})
    var_inits = []
    for gv in gfx_vars:
        n = gv[len('OBJ_EVENT_GFX_VAR_'):]
        m = re.search(r'setvar\s+VAR_OBJ_GFX_ID_%s\s*,\s*(OBJ_EVENT_GFX_\w+)' % n, orig_text)
        if m:
            var_inits.append('\tsetvar VAR_OBJ_GFX_ID_%s, %s' % (n, m.group(1)))
    out = ['@ %s: generated by tools/kanto/port_kanto.py (%s stub v%d).' % (name, MARK, STUB_VERSION),
           '@ Original FRLG scripts for reference: scripts_frlg_orig.inc (not built). Inventory: docs/kanto/mappe.json.',
           '@ Never overwritten by the port script once generated: write the new Italian content here.',
           '@ Item balls use Common_EventScript_FindItem (item in map.json), hidden items keep their item:',
           '@ both need no script. Trainers were set to TRAINER_TYPE_NONE: restore trainer_type/sight in map.json',
           '@ (see "kanto_port_orig") when the stub becomes a trainerbattle script.', '']
    out.append('%s_MapScripts::' % name)
    if respawn or var_inits:
        out.append('\tmap_script MAP_SCRIPT_ON_TRANSITION, %s_OnTransition' % name)
    out += ['\t.byte 0', '']
    if respawn or var_inits:
        out.append('%s_OnTransition:' % name)
        if respawn:
            out.append('\tsetrespawn %s' % respawn[0])
        out += var_inits + ['\tend', '']
    texts = []
    for lab, e in entries.items():
        out += ['@ ' + c for c in e['comments']]
        out.append('%s::' % lab)
        kind = e['kind']
        if kind == 'special':
            out += SPECIAL_STUBS[(name, e['orig'])] + ['']
            continue
        if kind == 'nurse':
            out += ['\tsetvar VAR_0x800B, %s' % e['lid'], '\tcall Common_EventScript_PkmnCenterNurse',
                    '\twaitmessage', '\twaitbuttonpress', '\trelease', '\tend', '']
            continue
        if kind == 'mart':
            body = blocks.get(e['orig'], '')
            mart = re.search(r'pokemart\s+(\w+)', body).group(1)
            items = re.findall(r'\.2byte\s+(ITEM_\w+)', blocks.get(mart, ''))
            if items:
                ml = lab.replace('_EventScript_', '_Items_')
                out += ['\tlock', '\tfaceplayer', '\tmessage gText_HowMayIServeYou', '\twaitmessage',
                        '\tpokemart %s' % ml, '\tmsgbox gText_PleaseComeAgain, MSGBOX_DEFAULT', '\trelease', '\tend', '',
                        '\t.align 2', '%s:' % ml] + ['\t.2byte %s' % it for it in items] + ['\tpokemartlistend', '']
                continue
        tlab = lab.replace('_EventScript_', '_Text_')
        if kind == 'trigger':
            out += ['\tlockall', '\tmsgbox %s, MSGBOX_DEFAULT' % tlab, '\treleaseall', '\tend', '']
        else:
            out += ['\tmsgbox %s, %s' % (tlab, 'MSGBOX_SIGN' if kind == 'sign' else 'MSGBOX_NPC'), '\tend', '']
        texts.append((tlab, stub_text(name, lab.split('_EventScript_')[-1],
                                      {'sign': 'Cartello', 'trigger': 'Evento', 'trainer': 'Allenatore'}.get(kind, 'NPC'))))
    for tlab, txt in texts:
        out += ['%s:' % tlab, '\t.string "%s"' % txt, '']
    write(spath, '\n'.join(out))
    return True


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


# ---------------------------------------------------------------------------------- tree-wide data
def patch_mapsec_names(root):
    """Italian names of the mainland Kanto sections (only where the tree still has the English name)."""
    path = os.path.join(root, 'src/data/region_map/region_map_sections.json')
    data = json.loads(read(path))
    n = 0
    for e in data['map_sections']:
        it = MAINLAND_MAPSECS.get(e['id'])
        if it and e.get('name') != it and not any(c.islower() for c in (e.get('name') or '').replace('é', '')):
            e['name'] = it
            n += 1
    if n:
        write(path, json.dumps(data, indent=2, ensure_ascii=False) + '\n')
        print('  patched %s (%d Italian Kanto section names)' % (os.path.relpath(path), n))


def init_flags_script(tree, flags, ported):
    """FRLG new-game hide flags (EventScript_ResetAllMapFlagsFrlg) -> Kanto flags, set on the first trip."""
    text = read(tree.p('data/scripts/new_game.inc')) if os.path.exists(tree.p('data/scripts/new_game.inc')) else ''
    body = text.split('EventScript_ResetAllMapFlagsFrlg::', 1)[1].split('\tend', 1)[0] if 'EventScript_ResetAllMapFlagsFrlg::' in text else ''
    frlg = re.findall(r'setflag\s+(FLAG_\w+)', body)
    used = set()
    for n in ported:
        for o in tree.maps[n].get('object_events') or []:
            ko = o.get('kanto_port_orig') or {}
            if ko.get('flag'):
                used.add(ko['flag'])
    lines = ['@ FRLG objects that start hidden (EventScript_ResetAllMapFlagsFrlg), run once by the travel script',
             'KantoPort_EventScript_InitFlags::',
             '\tgoto_if_set %s, Common_EventScript_NopReturn' % FLAG_INIT_DONE]
    lines += ['\tsetflag %s' % flags.for_frlg(f) for f in frlg if f in used]
    lines += ['\tsetflag %s' % FLAG_INIT_DONE, '\treturn', '']
    return lines, [f for f in frlg if f in used]


TRAVEL_TEMPLATE = '''@ KANTO_PORT: travel link Hoenn <-> Kanto (generated once by tools/kanto/port_kanto.py, then edited by hand).
@ Hook: one line in LilycoveCity_Harbor_EventScript_FerryAttendant (data/maps/LilycoveCity_Harbor/scripts.inc):
@     call_if_set FLAG_IS_CHAMPION, KantoTravel_EventScript_LilycoveOffer
@ Return: the ferry sailor of Vermilion City (VermilionCity_Frlg_EventScript_FerrySailor) jumps here.

@ Lilycove harbor, after the attendant faced the player. NO -> back to the normal ferry menu.
KantoTravel_EventScript_LilycoveOffer::
	msgbox KantoTravel_Text_SailToKanto, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Common_EventScript_NopReturn
	call KantoPort_EventScript_InitFlags
	call LilycoveCity_Harbor_EventScript_BoardFerry
	warp MAP_VERMILION_CITY, {vx}, {vy}
	waitstate
	release
	end

@ Vermilion City harbor (Aranciopoli): the sailor takes the player back to Lilycove (Porto Alghepoli).
KantoTravel_EventScript_VermilionSailor::
	lock
	faceplayer
	msgbox KantoTravel_Text_SailToHoenn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, KantoTravel_EventScript_VermilionSailorNo
	msgbox KantoTravel_Text_AllAboard, MSGBOX_DEFAULT
	closemessage
	playse SE_SHIP
	warp {lmap}, {lx}, {ly}
	waitstate
	release
	end

KantoTravel_EventScript_VermilionSailorNo::
	msgbox KantoTravel_Text_SeeYou, MSGBOX_DEFAULT
	release
	end

KantoTravel_Text_SailToKanto:
	.string "Da quando sei Campione, la M/N Acqua\\n"
	.string "fa scalo anche ad Aranciopoli, a Kanto.\\p"
	.string "Vuoi salpare per Kanto?$"

KantoTravel_Text_SailToHoenn:
	.string "La M/N Acqua torna a Porto Alghepoli,\\n"
	.string "a Hoenn. Vuoi salire a bordo?$"

KantoTravel_Text_AllAboard:
	.string "Si salpa! Tutti a bordo!$"

KantoTravel_Text_SeeYou:
	.string "Quando vuoi tornare a Hoenn,\\n"
	.string "vieni pure da me.$"
'''


def patch_travel(root, force=False):
    path = os.path.join(root, TRAVEL_FILE)
    if force or not os.path.exists(path):
        write(path, TRAVEL_TEMPLATE.format(vx=VERMILION_ARRIVAL[0], vy=VERMILION_ARRIVAL[1], lmap=LILYCOVE_RETURN[0],
                                           lx=LILYCOVE_RETURN[1], ly=LILYCOVE_RETURN[2]))
        print('  wrote %s' % TRAVEL_FILE)
    hp = os.path.join(root, 'data/maps/LilycoveCity_Harbor/scripts.inc')
    text = read(hp)
    if 'KantoTravel_EventScript_LilycoveOffer' in text:
        return
    m = re.search(r'^LilycoveCity_Harbor_EventScript_FerryAttendant::\n((?:\t(?:lock|faceplayer)\n)+)', text, re.M)
    if not m:
        die(hp + ': FerryAttendant lock/faceplayer not found (add the hook by hand, see docs/kanto/README.md)')
    hook = '\tcall_if_set FLAG_IS_CHAMPION, KantoTravel_EventScript_LilycoveOffer @ KANTO_PORT travel to Kanto\n'
    write(hp, text[:m.end()] + hook + text[m.end():])
    print('  patched %s (travel hook)' % os.path.relpath(hp))


def patch_test_warp(root):
    """TEST ONLY: the emulator harness can warp anywhere by writing gKantoTestWarp/gKantoTestWarpXY."""
    ov = os.path.join(root, 'src/overworld.c')
    patch_replace(ov, 'void CB1_Overworld(void)\n{\n',
                  '#if KANTO_PORT_TEST_START // KANTO_PORT test only: emulator harness warps\n'
                  'EWRAM_DATA u32 gKantoTestWarp = 0;   // 0x80000000 | group << 8 | num\n'
                  'EWRAM_DATA u32 gKantoTestWarpXY = 0; // x | y << 16\n'
                  '#endif\n\n'
                  'void CB1_Overworld(void)\n{\n'
                  '#if KANTO_PORT_TEST_START\n'
                  '    if ((gKantoTestWarp & 0x80000000) && gMain.callback2 == CB2_Overworld && !ArePlayerFieldControlsLocked())\n'
                  '    {\n'
                  '        SetWarpDestination((gKantoTestWarp >> 8) & 0xFF, gKantoTestWarp & 0xFF, WARP_ID_NONE,\n'
                  '                           (s16)(gKantoTestWarpXY & 0xFFFF), (s16)(gKantoTestWarpXY >> 16));\n'
                  '        gKantoTestWarp = 0;\n'
                  '        DoWarp();\n'
                  '        return;\n'
                  '    }\n'
                  '#endif\n',
                  'gKantoTestWarp = 0', 'test warp hook')
    patch_replace(os.path.join(root, 'src/new_game.c'),
                  '    FlagSet(FLAG_SYS_B_DASH);\n    SetWarpDestination(',
                  '    FlagSet(FLAG_SYS_B_DASH);\n    FlagSet(FLAG_IS_CHAMPION); // KANTO_PORT_TEST travel link\n'
                  '    FlagSet(FLAG_SYS_GAME_CLEAR);\n    SetWarpDestination(',
                  'KANTO_PORT_TEST travel', 'test champion flags')


# ---------------------------------------------------------------------------------------- inventory
def map_kind(tree, name):
    d = tree.maps[name]
    sec = d.get('region_map_section')
    g = tree.group_of[name]
    if '_Gym' in name:
        return 'gym'
    if 'PokemonCenter' in name:
        return 'pokemon_center'
    if name.split('_')[1:2] == ['Mart'] or '_Mart_' in name or name.endswith('_Mart_Frlg'):
        return 'mart'
    if 'DepartmentStore' in name:
        return 'department_store'
    if g == 'gMapGroup_TownsAndRoutes_Frlg':
        return 'route' if name.startswith('Route') else 'town'
    if sec == 'MAPSEC_POKEMON_LEAGUE' or name.startswith('IndigoPlateau'):
        return 'league'
    if sec == 'MAPSEC_S_S_ANNE':
        return 'ship'
    if sec in DUNGEON_MAPSECS:
        return 'dungeon'
    if re.search(r'Gate|Entrance|_House|RestHouse', name) and name.startswith('Route'):
        return 'gate' if 'House' not in name else 'house'
    if 'House' in name:
        return 'house'
    return 'building'


def wild_tables(root):
    path = os.path.join(root, 'src/data/wild_encounters.json')
    out = {}
    try:
        data = json.loads(read(path))
    except (OSError, ValueError):
        return out
    for grp in data.get('wild_encounter_groups', []):
        for e in grp.get('encounters', []):
            base = e.get('base_label', '')
            if not base.endswith('_FireRed'):
                continue
            out.setdefault(e.get('map'), sorted(k for k in e if k.endswith('_mons')))
    return out


def build_inventory(tree, ported, flags, init_hidden):
    wild = wild_tables(tree.root)
    heal = {h['map']: h['id'] for h in tree.heal}
    respawn = {h.get('respawn_map'): h['id'] for h in tree.heal if h.get('respawn_map')}
    # specials that Emerald's own scripts use (the rest are FRLG-only: need porting/replacing when writing)
    em_text = []
    for n in tree.order:
        if not tree.is_kanto(n):
            path = os.path.join(tree.maps_dir, n, 'scripts.inc')
            if os.path.exists(path):
                em_text.append(read(path))
    sdir = tree.p('data/scripts')
    for fn in os.listdir(sdir):
        if fn.endswith('.inc') and not fn.endswith('_frlg.inc'):
            em_text.append(read(os.path.join(sdir, fn)))
    emerald_specials = set(re.findall(r'^\s*(?:special|specialvar\s+\w+,)\s+(\w+)', '\n'.join(em_text), re.M))
    maps = []
    for n in ported:
        d = tree.maps[n]
        orig_text = tree.orig_scripts(n)
        blocks = tree.orig_blocks(n)
        specials = sorted(set(re.findall(r'^\s*(?:special|specialvar\s+\w+,)\s+(\w+)', orig_text, re.M)))
        e = {'map': n, 'id': d['id'], 'group': tree.group_of[n], 'mapsec': d.get('region_map_section'),
             'name_it': MAINLAND_MAPSECS.get(d.get('region_map_section')), 'kind': map_kind(tree, n),
             'layout': d.get('layout'), 'music': d.get('music'), 'map_type': d.get('map_type'),
             'scripts': 'data/maps/%s/scripts.inc' % n, 'orig_scripts': 'data/maps/%s/scripts_frlg_orig.inc' % n,
             'heal_location': heal.get(d['id']), 'respawn_for': respawn.get(d['id']),
             'wild_encounters': wild.get(d['id'], []),
             'objects': [], 'item_balls': [], 'signs': [], 'hidden_items': [], 'coord_events': [],
             'warps': [], 'connections': [], 'orig_specials': specials,
             'orig_specials_frlg_only': [x for x in specials if x not in emerald_specials]}
        for i, o in enumerate(d.get('object_events') or []):
            if o.get('type') == 'clone':
                e['objects'].append({'index': i + 1, 'clone_of': o.get('target_map') + '/' + str(o.get('target_local_id')),
                                     'gfx': o.get('graphics_id'), 'x': o['x'], 'y': o['y']})
                continue
            ko = o.get('kanto_port_orig') or {}
            if o.get('script') == 'Common_EventScript_FindItem':
                e['item_balls'].append({'index': i + 1, 'local_id': o.get('local_id'), 'x': o['x'], 'y': o['y'],
                                        'item': o.get('trainer_sight_or_berry_tree_id'), 'flag': o.get('flag'),
                                        'orig_flag': ko.get('flag'), 'orig_script': ko.get('script')})
                continue
            entry = {'index': i + 1, 'local_id': o.get('local_id'), 'gfx': o.get('graphics_id'), 'x': o['x'], 'y': o['y'],
                     'elevation': o.get('elevation'), 'movement': o.get('movement_type'), 'script': o.get('script'),
                     'orig_script': ko.get('script'), 'flag': o.get('flag'), 'orig_flag': ko.get('flag')}
            if ko.get('flag') in init_hidden:
                entry['hidden_at_start'] = True
            if ko.get('trainer_type') not in (None, 'TRAINER_TYPE_NONE'):
                entry['orig_trainer_type'] = ko['trainer_type']
                entry['orig_trainer_sight'] = ko.get('trainer_sight_or_berry_tree_id')
            t = trainer_of(blocks, ko.get('script'))
            if t:
                entry['orig_trainer'] = t
            e['objects'].append(entry)
        for b in d.get('bg_events') or []:
            ko = b.get('kanto_port_orig') or {}
            if b.get('type') == 'hidden_item':
                e['hidden_items'].append({'x': b['x'], 'y': b['y'], 'item': b.get('item'), 'flag': b.get('flag'),
                                          'orig_flag': ko.get('flag'), 'quantity': b.get('quantity', 1),
                                          'underfoot': b.get('underfoot', False)})
            elif b.get('type') == 'sign':
                e['signs'].append({'x': b['x'], 'y': b['y'], 'script': b.get('script'), 'orig_script': ko.get('script'),
                                   'facing': b.get('player_facing_dir')})
        for c in d.get('coord_events') or []:
            ko = c.get('kanto_port_orig') or {}
            e['coord_events'].append({'x': c['x'], 'y': c['y'], 'type': c.get('type'), 'script': c.get('script'),
                                      'orig_script': ko.get('script'), 'orig_var': ko.get('var'),
                                      'orig_var_value': ko.get('var_value'), 'enabled': not ko})
        for i, w in enumerate(d.get('warp_events') or []):
            orig = w.get('kanto_port_orig_dest_map')
            e['warps'].append({'id': i, 'x': w['x'], 'y': w['y'], 'dest_map': orig or w['dest_map'],
                               'dest_warp_id': w.get('kanto_port_orig_dest_warp_id', w.get('dest_warp_id')),
                               'built': orig is None})
        for c in d.get('connections') or []:
            e['connections'].append({'direction': c['direction'], 'map': c['map'], 'offset': c['offset'], 'built': True})
        for c in d.get('kanto_port_dropped_connections') or []:
            e['connections'].append({'direction': c['direction'], 'map': c['map'], 'offset': c['offset'], 'built': False})
        maps.append(e)
    kinds = {}
    for e in maps:
        kinds[e['kind']] = kinds.get(e['kind'], 0) + 1
    return {
        'generated_by': 'tools/kanto/port_kanto.py',
        'note': 'Kanto (Atto 2) - mappe FRLG continentali costruite nella ROM Emerald. Etichette script = '
                'data/maps/<map>/scripts.inc; orig_* = FireRed originale (scripts_frlg_orig.inc).',
        'counts': {'maps': len(maps), 'kinds': kinds,
                   'objects': sum(len(e['objects']) for e in maps), 'item_balls': sum(len(e['item_balls']) for e in maps),
                   'hidden_items': sum(len(e['hidden_items']) for e in maps), 'signs': sum(len(e['signs']) for e in maps),
                   'coord_events': sum(len(e['coord_events']) for e in maps),
                   'trainers': sum(1 for e in maps for o in e['objects'] if o.get('orig_trainer_type')),
                   'kanto_flags_used': len(flags.names), 'kanto_flags_total': KANTO_FLAGS_COUNT},
        'maps': maps,
    }


# -------------------------------------------------------------------------------------------- main
def main():
    global dry_run
    here = os.path.dirname(os.path.abspath(__file__))
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('tree_pos', nargs='?', metavar='TREE', help='pokeemerald-expansion tree (same as --tree)')
    ap.add_argument('--tree', help='pokeemerald-expansion tree to modify')
    ap.add_argument('--maps', nargs='*', help='maps / map constants / map groups to port')
    ap.add_argument('--all', action='store_true', help='port every mainland Kanto map (no Sevii, Trainer Tower, link rooms)')
    ap.add_argument('--include-sevii', action='store_true', help='with --all/--warp-depth: also the Sevii Islands & co.')
    ap.add_argument('--warp-depth', type=int, default=0, help='also port maps reachable through N levels of warps')
    ap.add_argument('--wild-version', choices=('firered', 'leafgreen'), default='firered',
                    help='which FRLG encounter tables Emerald uses for Kanto (default firered)')
    ap.add_argument('--no-runtime-patches', action='store_true',
                    help='skip the optional per-map runtime tweaks (PC tiles, PokeCenter monitor, flavor text)')
    ap.add_argument('--no-travel', action='store_true', help='do not add the Lilycove <-> Vermilion ferry link')
    ap.add_argument('--force-stubs', action='store_true',
                    help='regenerate stub scripts.inc even if they exist (DESTROYS edits made to them)')
    ap.add_argument('--inventory', default=os.path.normpath(os.path.join(here, '..', '..', 'docs', 'kanto', 'mappe.json')),
                    help='where to write the machine-readable map inventory (default docs/kanto/mappe.json)')
    ap.add_argument('--test-start', metavar='MAP_ID,X,Y',
                    help='TEST ONLY: new game starts on this map (+ Pikachu, champion flags, emulator warp hook)')
    ap.add_argument('--dry-run', action='store_true')
    args = ap.parse_args()
    dry_run = args.dry_run
    root = os.path.abspath(args.tree or args.tree_pos or die('give the tree (--tree PATH)'))
    if not os.path.exists(os.path.join(root, 'tools', 'mapjson', 'mapjson.cpp')):
        die('not a pokeemerald-expansion tree: ' + root)

    tree = Tree(root)
    selection = resolve_selection(tree, args)
    if not selection and not any(tree.is_ported(n) for n in tree.order):
        die('nothing selected (use --maps or --all)')

    patch_engine(root, args.wild_version, not args.no_runtime_patches)
    patch_flags_h(root)
    patch_mapsec_names(root)
    if args.test_start:
        patch_test_start(root, args.test_start)
        patch_test_warp(root)

    flags = KantoFlags(root)
    new = [n for n in selection if not tree.is_ported(n)]
    print('maps selected: %d (new: %d, already ported: %d)' % (len(selection), len(new), len(selection) - len(new)))
    for n in selection:
        if tree.maps[n].get('kanto_port') != STUB_VERSION and not (tree.is_ported(n) and os.path.exists(
                os.path.join(tree.maps_dir, n, 'scripts_frlg_orig.inc'))):
            sanitize_map(tree, n, flags)
            tree.save_map(n)
    ported = [n for n in tree.order if tree.is_ported(n)]
    stubs_written = sum(1 for n in ported if write_stubs(tree, n, args.force_stubs))
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

    # generated script list (+ first-trip flag init) and travel link
    init_lines, init_hidden = init_flags_script(tree, flags, ported)
    lines = ['@ Generated by tools/kanto/port_kanto.py (KANTO_PORT) - do not edit by hand', '']
    lines += ['\t.include "data/maps/%s/scripts.inc"' % n for n in ported]
    lines += ['', '\t.include "%s"' % TRAVEL_FILE if not args.no_travel else '', ''] + init_lines
    write(os.path.join(root, 'data', 'kanto_port_scripts.inc'), '\n'.join(lines) + '\n')
    if not args.no_travel:
        patch_travel(root)
    flags.save()

    # summary + inventory
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
        'kanto_flags_used': len(flags.names),
    }
    write(os.path.join(root, 'data', 'kanto_port_report.json'), dump_json(summary))
    if args.inventory:
        inv = build_inventory(tree, ported, flags, set(init_hidden))
        write(args.inventory, json.dumps(inv, indent=1, ensure_ascii=False) + '\n')
        print('inventory: %s (%d maps)' % (args.inventory, len(inv['maps'])))
    print('ported maps total: %d, stubs written now: %d, layouts: %d, partially built groups: %d' % (
        len(ported), stubs_written, len(used_layouts), len(groups_partial)))
    print('Kanto flags allocated: %d / %d' % (len(flags.names), KANTO_FLAGS_COUNT))
    print('dangling references (redirected/parked): %d' % len(dangling))
    for line in dangling[:40]:
        print('   ' + line)
    if len(dangling) > 40:
        print('   ... (see data/kanto_port_report.json)')
    print('%s %d file(s)' % ('would change' if dry_run else 'changed', len(set(changed_files))))


if __name__ == '__main__':
    main()
