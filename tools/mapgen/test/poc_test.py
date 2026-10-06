#!/usr/bin/env python3
"""End-to-end emulator test of the mapgen proof of concept (examples/test_town.yaml).

Requires: the PoC compiled into TREE, test_start_patch.py applied, `make` done.
    python3 poc_test.py [--tree TREE] [--out DIR]
Writes screenshots + results.json + results.md into DIR.
"""
import argparse
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from play import Emu, EmuError  # noqa: E402

results = []


def check(name, ok, detail='', shot=None):
    results.append({'test': name, 'ok': bool(ok), 'detail': detail, 'shot': shot})
    print('%s %-45s %s' % ('PASS' if ok else 'FAIL', name, detail), flush=True)
    return ok


def enter_door(e, x, y, expect_map, shot):
    """stand below door (x,y), walk in, check destination map"""
    e.walk_to(x, y + 1)
    e.run(['hold UP 16', 'wait 150'], 'enter')
    m, px, py = e.where()
    p = e.shot(shot)
    return check('door (%d,%d) -> %s' % (x, y, expect_map), m == expect_map, '%s (%d,%d)' % (m, px, py), p)


def exit_mat(e, x, y, expect_map, expect_pos, shot):
    e.walk_to(x, y)
    e.run(['hold DOWN 16', 'wait 180'], 'exit')
    m, px, py = e.where()
    p = e.shot(shot)
    return check('exit mat (%d,%d) -> %s %s' % (x, y, expect_map, expect_pos), m == expect_map and (px, py) == expect_pos,
                 '%s (%d,%d)' % (m, px, py), p)


def party_hp(e):
    e.run(['read16 gParties+0x56 hp', 'read16 gParties+0x58 maxhp', 'read8 gParties+0x54 lvl'], 'hp')
    return e.last['hp'], e.last['maxhp'], e.last['lvl']


def in_battle(e):
    e.run(['wait 1', 'read32 gMain+4 cb2b'], 'cb2')
    return e.last['cb2b']


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--tree', default='/home/user/work/mapgen-tree')
    ap.add_argument('--out', default='/home/user/work/mapgen-out/emu_poc')
    a = ap.parse_args()
    import shutil
    shutil.rmtree(a.out, ignore_errors=True)
    e = Emu(a.tree, a.out)
    syms = {}
    for l in open(e.syms):
        p = l.split()
        if len(p) == 3:
            syms[p[2]] = int(p[0], 16)
    cb2_overworld = syms['CB2_Overworld'] | 1

    e.boot_new_game()
    e.run(['wait 120'], 'settle')
    m, x, y = e.where()
    check('new game starts in Littleroot', m == 'MAP_LITTLEROOT_TOWN', '%s (%d,%d)' % (m, x, y), e.shot('p00_littleroot'))

    # ---- Littleroot -> Test Town through the re-targeted door
    enter_door(e, 14, 8, 'MAP_TEST_TOWN', 'p01_testtown_arrival')
    m, x, y = e.where()
    check('arrive in front of the link house door', (x, y) == (26, 20), '(%d,%d)' % (x, y))
    # ---- and back
    enter_door(e, 26, 19, 'MAP_LITTLEROOT_TOWN', 'p02_back_in_littleroot')
    enter_door(e, 14, 8, 'MAP_TEST_TOWN', 'p03_testtown_again')

    # ---- NPC + sign in town
    e.walk_to(12, 4)
    e.face('UP')
    e.talk('p04_town_sign', presses=3)
    e.walk_to(20, 4)
    e.face('UP')
    e.talk('p05_npc_girl', presses=3)

    # ---- Pokemon Center: heal
    hp, mx, lvl = party_hp(e)
    check('party: Mudkip lv12 present', lvl == 12 and mx > 0, 'hp=%d/%d lvl=%d' % (hp, mx, lvl))
    e.run(['write16 gParties+0x56 3'], 'hurt')
    hp2, _, _ = party_hp(e)
    check('hp lowered for the nurse test', hp2 == 3, 'hp=%d' % hp2)
    enter_door(e, 7, 8, 'MAP_TEST_TOWN_POKEMON_CENTER_1F', 'p06_pokecenter')
    e.walk_to(7, 4)
    e.face('UP')
    e.run(['press A', 'wait 20', 'waitstable 8 200', 'shot p07_nurse'], 'nurse')
    lines = []
    for _ in range(12):
        lines += ['press A', 'wait 60']
    lines += ['shot p08_nurse_done']
    e.run(lines, 'nurse2')
    lines = []
    for _ in range(4):
        lines += ['press B', 'wait 40']
    e.run(lines, 'nurse3')
    hp3, mx3, _ = party_hp(e)
    check('nurse heals the party', hp3 == mx3, 'hp=%d/%d' % (hp3, mx3), os.path.join(a.out, 'p07_nurse.png'))
    # upstairs and back
    e.walk_to(1, 6)
    e.run(['wait 120'], 'stairs')
    m, x, y = e.where()
    check('PC stairs -> 2F', m == 'MAP_TEST_TOWN_POKEMON_CENTER_2F', '%s (%d,%d)' % (m, x, y), e.shot('p09_pc_2f'))
    if m == 'MAP_TEST_TOWN_POKEMON_CENTER_2F':
        e.run(['hold DOWN 16', 'wait 30', 'hold LEFT 16', 'wait 150'], 'down')
        m, x, y = e.where()
        check('2F stairs -> 1F', m == 'MAP_TEST_TOWN_POKEMON_CENTER_1F', '%s (%d,%d)' % (m, x, y))
    exit_mat(e, 7, 8, 'MAP_TEST_TOWN', (7, 9), 'p10_out_of_pc')

    # ---- Mart
    enter_door(e, 23, 8, 'MAP_TEST_TOWN_MART', 'p11_mart')
    e.walk_to(3, 3)
    e.face('LEFT')
    e.run(['press A', 'wait 30', 'waitstable 8 200', 'press A', 'wait 60', 'waitstable 8 200', 'shot p12_mart_menu'], 'clerk')
    e.run(['press A', 'wait 60', 'shot p13_mart_buy'] + ['press B', 'wait 40'] * 6, 'clerk2')
    exit_mat(e, 3, 7, 'MAP_TEST_TOWN', (23, 9), 'p14_out_of_mart')

    # ---- houses + gym
    enter_door(e, 7, 16, 'MAP_TEST_TOWN_HOUSE1', 'p15_house1')
    e.walk_to(6, 5)
    e.face('UP')
    e.talk('p16_house1_npc', presses=3)
    exit_mat(e, 3, 8, 'MAP_TEST_TOWN', (7, 17), 'p17_out_house1')
    enter_door(e, 22, 15, 'MAP_TEST_TOWN_HOUSE2', 'p18_house2')
    exit_mat(e, 3, 7, 'MAP_TEST_TOWN', (22, 16), 'p19_out_house2')
    enter_door(e, 16, 19, 'MAP_TEST_TOWN_GYM', 'p20_gym')
    e.walk_to(5, 3)
    e.face('UP')
    e.talk('p21_gym_leader', presses=4)
    exit_mat(e, 5, 19, 'MAP_TEST_TOWN', (16, 20), 'p22_out_gym')

    # ---- connection north to the route
    e.walk_to(15, 0)
    e.run(['hold UP 32', 'wait 60'], 'north')
    m, x, y = e.where()
    check('walk north across the connection', m == 'MAP_TEST_ROUTE1', '%s (%d,%d)' % (m, x, y), e.shot('p23_route1'))

    # ---- trainer: walking up the path through the Youngster's line of sight (row 13)
    battle = False
    for _ in range(12):
        e.run(['hold UP 16', 'wait 4'], 'up')
        e.run(['read32 gMain+4 cb2b', 'wait 1'], 'cb')
        if e.last['cb2b'] != cb2_overworld:
            battle = True
            break
        m, x, y = e.where()
        if y <= 10:
            break
    e.run(['wait 60'], 'spot')
    # the trainer walks up and talks: advance until the battle starts
    for _ in range(25):
        e.run(['press A', 'wait 30', 'read32 gMain+4 cb2b'], 'tr')
        if e.last['cb2b'] not in (cb2_overworld,):
            break
    e.run(['wait 200', 'shot p24_trainer_battle'], 'trshot')
    in_b = e.last['cb2b'] != cb2_overworld
    check('trainer spots player and battle starts', in_b, 'cb2=0x%x' % e.last['cb2b'],
          os.path.join(a.out, 'p24_trainer_battle.png'))
    # fight: mash A (FIGHT -> first move) until back in the overworld
    for i in range(60):
        e.run(['press A', 'wait 40', 'read32 gMain+4 cb2b'], 'fight')
        if e.last['cb2b'] == cb2_overworld:
            break
    e.run(['wait 60'] + ['press A', 'wait 40'] * 4, 'after')
    e.run(['read32 gMain+4 cb2b'], 'x')
    check('trainer battle won, back on the route', e.last['cb2b'] == cb2_overworld, '', e.shot('p25_after_trainer'))

    # ---- wild encounter in tall grass (west patch rows 12-14, x 3-10)
    m, x, y = e.where()
    e.walk_to(11, 12)
    got = False
    for i in range(40):
        d = 'LEFT' if i % 2 == 0 else 'RIGHT'
        e.run(['hold %s 32' % d, 'wait 6', 'read32 gMain+4 cb2b'], 'grass')
        if e.last['cb2b'] != cb2_overworld:
            got = True
            break
    e.run(['wait 240', 'shot p26_wild_battle', 'read32 gMain+4 cb2b'], 'wild')
    check('wild battle in tall grass', got, 'after %d moves' % (i + 1), os.path.join(a.out, 'p26_wild_battle.png'))
    if got:
        # RUN: bottom-right of the action menu
        for _ in range(6):
            e.run(['press A', 'wait 30'], 'w')
            e.run(['press RIGHT', 'wait 8', 'press DOWN', 'wait 8', 'press A', 'wait 120', 'read32 gMain+4 cb2b'], 'run')
            if e.last['cb2b'] == cb2_overworld:
                break
        e.run(['wait 60', 'read32 gMain+4 cb2b'], 'x')
        check('ran from the wild battle', e.last['cb2b'] == cb2_overworld, '', e.shot('p27_after_wild'))

    # ---- back south across the connection
    e.walk_to(15, 23)
    e.run(['hold DOWN 32', 'wait 60'], 'south')
    m, x, y = e.where()
    check('walk south back into Test Town', m == 'MAP_TEST_TOWN', '%s (%d,%d)' % (m, x, y), e.shot('p28_back_in_town'))

    json.dump(results, open(os.path.join(a.out, 'results.json'), 'w'), indent=1)
    with open(os.path.join(a.out, 'results.md'), 'w') as f:
        f.write('| result | test | detail | screenshot |\n|---|---|---|---|\n')
        for r in results:
            f.write('| %s | %s | %s | %s |\n' % ('PASS' if r['ok'] else 'FAIL', r['test'], r['detail'], r['shot'] or ''))
    print('%d/%d passed' % (sum(r['ok'] for r in results), len(results)))


if __name__ == '__main__':
    main()
