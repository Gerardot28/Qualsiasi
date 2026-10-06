#!/usr/bin/env python3
"""TEST-ONLY patch for a work tree: a new game skips the truck and starts in
Littleroot Town below May's house door (the door that compile_map's PoC
re-targets to Test Town), with a lv 12 Mudkip, running shoes and 5 Poke Balls.

    test_start_patch.py apply  [--root TREE] [--x 14 --y 9 --map MAP_LITTLEROOT_TOWN]
    test_start_patch.py revert [--root TREE]

Never apply this to a release tree: it is only meant for emulator tests.
"""
import argparse
import os
import re

MARK = 'MAPGEN_TEST_START'


def apply(root, mapid, x, y):
    ng = os.path.join(root, 'src/new_game.c')
    s = open(ng).read()
    if MARK not in s:
        code = ('\n#if %s // test-only start, see tools/mapgen/test/test_start_patch.py\n'
                '    VarSet(VAR_LITTLEROOT_INTRO_STATE, 7);\n'
                '    FlagSet(FLAG_SYS_POKEMON_GET);\n'
                '    FlagSet(FLAG_SYS_B_DASH);\n'
                '    ScriptGiveMon(SPECIES_MUDKIP, 12, ITEM_NONE);\n'
                '    AddBagItem(ITEM_POKE_BALL, 5);\n'
                '    SetWarpDestination(MAP_GROUP(%s), MAP_NUM(%s), WARP_ID_NONE, %d, %d);\n'
                '    WarpIntoMap();\n'
                '#endif\n') % (MARK, mapid, mapid, x, y)
        s = s.replace('    ClearFollowerNPCData();\n}\n', '    ClearFollowerNPCData();' + code + '}\n', 1)
        s = s.replace('#include "global.h"\n', '#include "global.h"\n#define %s 1\n#include "script_pokemon_util.h"\n#include "item.h"\n' % MARK, 1)
        open(ng, 'w').write(s)
    ow = os.path.join(root, 'src/overworld.c')
    s = open(ow).read()
    if MARK not in s:
        s = s.replace('        gFieldCallback = ExecuteTruckSequence;',
                      '        gFieldCallback = FieldCB_WarpExitFadeFromBlack; // %s' % MARK, 1)
        open(ow, 'w').write(s)
    print('applied test start patch to', root)


def revert(root):
    ng = os.path.join(root, 'src/new_game.c')
    s = open(ng).read()
    s = s.replace('#define %s 1\n#include "script_pokemon_util.h"\n#include "item.h"\n' % MARK, '')  # noqa
    s = re.sub(r'\n#if %s.*?#endif\n' % MARK, '\n', s, flags=re.S)
    s = s.replace('    ClearFollowerNPCData();\n\n}', '    ClearFollowerNPCData();\n}')
    open(ng, 'w').write(s)
    ow = os.path.join(root, 'src/overworld.c')
    s = open(ow).read()
    s = s.replace('        gFieldCallback = FieldCB_WarpExitFadeFromBlack; // %s' % MARK,
                  '        gFieldCallback = ExecuteTruckSequence;')
    open(ow, 'w').write(s)
    print('reverted test start patch in', root)


if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('cmd', choices=['apply', 'revert'])
    ap.add_argument('--root', default='/home/user/work/mapgen-tree')
    ap.add_argument('--map', default='MAP_LITTLEROOT_TOWN')
    ap.add_argument('--x', type=int, default=14)
    ap.add_argument('--y', type=int, default=9)
    a = ap.parse_args()
    apply(a.root, a.map, a.x, a.y) if a.cmd == 'apply' else revert(a.root)
