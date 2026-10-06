#!/usr/bin/env python3
"""Builds bosses.party = bosses_core.party (hand-written) + the templated rival
and rematch sections below (rosters are hand-picked; levels follow fixed rules).

    python3 -I tools/balance/boss_templates.py      # rewrites tools/balance/bosses.party
"""
import os

HERE = os.path.dirname(os.path.abspath(__file__))

# ------------------------------------------------------------------ rivals
# Vanilla variant suffix = the PLAYER's starter slot (VAR_STARTER_MON 0/1/2 ->
# _TREECKO/_TORCHIC/_MUDKIP, see data/maps/Route103/scripts.inc). The rival owns
# the starter that beats it: player grass (Turtwig) -> rival fire (Fuecoco line),
# player fire (Fuecoco) -> rival water (Froakie line), player water (Froakie) ->
# rival grass (Turtwig line). The two partner lines are the other two types.
ACE = {
    'TREECKO': ['Fuecoco', 'Crocalor', 'Crocalor', 'Crocalor', 'Skeledirge'],
    'TORCHIC': ['Froakie', 'Frogadier', 'Frogadier', 'Frogadier', 'Greninja'],
    'MUDKIP':  ['Turtwig', 'Turtwig', 'Grotle', 'Torterra', 'Torterra'],
}
# partner lines per stage (Rustboro, Route110, Route119, Lilycove)
FIRE = ['Houndour', 'Houndour', 'Houndoom', 'Houndoom']
WATER = ['Chewtle', 'Chewtle', 'Drednaw', 'Drednaw']
GRASS = ['Bounsweet', 'Steenee', 'Steenee', 'Tsareena']
PARTNERS = {'TREECKO': (WATER, GRASS), 'TORCHIC': (FIRE, GRASS), 'MUDKIP': (FIRE, WATER)}
BIRD = ['Rookidee', 'Corvisquire', 'Corvisquire', 'Corviknight']
STAGES = ['ROUTE_103', 'RUSTBORO', 'ROUTE_110', 'ROUTE_119', 'LILYCOVE']
ACE_LV = [6, 16, 21, 33, 39]
ACE_ITEM = [None, 'Oran Berry', 'Sitrus Berry', 'Sitrus Berry', 'Sitrus Berry']
ACE_NATURE = {'TREECKO': 'Modest', 'TORCHIC': 'Timid', 'MUDKIP': 'Adamant'}


def mon(name, lvl, item=None, nature=None, extra=()):
    s = name + (' @ ' + item if item else '') + '\nLevel: %d\n' % lvl
    if nature:
        s += 'Nature: %s\n' % nature
    for e in extra:
        s += e + '\n'
    return s


def rivals():
    out = ['/* ===================== RIVAL (May / Brendan, identical teams) ===================== */\n']
    for who in ('MAY', 'BRENDAN'):
        for var in ('TREECKO', 'TORCHIC', 'MUDKIP'):
            pa, pb = PARTNERS[var]
            for st, stage in enumerate(STAGES):
                tid = 'TRAINER_%s_%s_%s' % (who, stage, var)
                ai = 'Basic Trainer / HP Aware' if st == 0 else \
                    'Basic Trainer / Smart Mon Choices / HP Aware / Ace Pokemon'
                items = {3: 'Super Potion', 4: 'Hyper Potion'}.get(st)
                s = '=== %s ===\n' % tid
                if items:
                    s += 'Items: %s\n' % items
                s += 'AI: %s\n\n' % ai
                al = ACE_LV[st]
                mons = []
                if st == 1:
                    mons = [mon(BIRD[0], al - 2), mon(pa[0], al - 1)]
                elif st == 2:
                    mons = [mon(BIRD[1], al - 2), mon(pa[1], al - 2), mon(pb[1], al - 1)]
                elif st == 3:
                    mons = [mon(BIRD[2], al - 2), mon('Lucario', al - 2), mon(pa[2], al - 1), mon(pb[2], al - 1)]
                elif st == 4:
                    mons = [mon(BIRD[3], al - 1), mon('Lucario', al - 2), mon('Hattrem', al - 3),
                            mon(pa[3], al - 1), mon(pb[3], al - 1)]
                mons.append(mon(ACE[var][st], al, ACE_ITEM[st], ACE_NATURE[var]))
                s += '\n'.join(mons)
                out.append(s)
    return '\n'.join(out)


# ------------------------------------------------------------------ rematches
# (species, item, nature); last entry is the ace. Levels: _2 ace 60, _3 65, _4 70, _5 75,
# others 1-3 below. EVs on _4/_5. IVs 31 (leaders) by default.
ROSTERS = {
    'ROXANNE': [('Probopass', 'Leftovers', 'Relaxed'), ('Aerodactyl', 'Focus Sash', 'Jolly'),
                ('Gigalith', 'Sitrus Berry', 'Adamant'), ('Garganacl', 'Leftovers', 'Careful'),
                ('Rhyperior', 'Life Orb', 'Adamant'), ('Tyrantrum', 'Lum Berry', 'Adamant')],
    'BRAWLY': [('Pawmot', 'Sitrus Berry', 'Jolly'), ('Hariyama', 'Flame Orb', 'Adamant'),
               ('Mienshao', 'Life Orb', 'Jolly'), ('Toxicroak', 'Black Sludge', 'Adamant'),
               ('Lucario', 'Life Orb', 'Timid'), ('Conkeldurr', 'Flame Orb', 'Adamant')],
    'WATTSON': [('Rotom', 'Sitrus Berry', 'Timid'), ('Heliolisk', 'Choice Specs', 'Timid'),
                ('Bellibolt', 'Leftovers', 'Modest'), ('Luxray', 'Life Orb', 'Adamant'),
                ('Electivire', 'Expert Belt', 'Adamant'), ('Magnezone', 'Leftovers', 'Modest')],
    'FLANNERY': [('Torkoal', 'Heat Rock', 'Modest'), ('Houndoom', 'Life Orb', 'Timid'),
                 ('Salazzle', 'Focus Sash', 'Timid'), ('Centiskorch', 'Sitrus Berry', 'Adamant'),
                 ('Turtonator', 'White Herb', 'Modest'), ('Volcarona', 'Lum Berry', 'Timid')],
    'NORMAN': [('Kangaskhan', 'Silk Scarf', 'Adamant'), ('Lopunny', 'Life Orb', 'Jolly'),
               ('Bewear', 'Sitrus Berry', 'Adamant'), ('Porygon-Z', 'Choice Specs', 'Modest'),
               ('Ursaring', 'Flame Orb', 'Adamant'), ('Slaking', 'Choice Band', 'Adamant')],
    'WINONA': [('Skarmory', 'Leftovers', 'Impish'), ('Hawlucha', 'White Herb', 'Jolly'),
               ('Talonflame', 'Sharp Beak', 'Jolly'), ('Noivern', 'Life Orb', 'Timid'),
               ('Altaria', 'Lum Berry', 'Adamant'), ('Dragonite', 'Sitrus Berry', 'Adamant')],
    'TATE_AND_LIZA': [('Solrock', 'Sitrus Berry', 'Brave'), ('Lunatone', 'Sitrus Berry', 'Quiet'),
                      ('Bronzong', 'Leftovers', 'Relaxed'), ('Reuniclus', 'Life Orb', 'Quiet'),
                      ('Hatterene', 'Leftovers', 'Quiet'), ('Metagross', 'Lum Berry', 'Adamant')],
    'JUAN': [('Pelipper', 'Damp Rock', 'Modest'), ('Barraskewda', 'Life Orb', 'Jolly'),
             ('Ludicolo', 'Leftovers', 'Modest'), ('Crawdaunt', 'Focus Sash', 'Adamant'),
             ('Lapras', 'Sitrus Berry', 'Modest'), ('Kingdra', 'Mystic Water', 'Modest')],
    'WALLY_VR': [('Magnezone', 'Leftovers', 'Modest'), ('Altaria', 'Lum Berry', 'Adamant'),
                 ('Roserade', 'Life Orb', 'Timid'), ('Sylveon', 'Leftovers', 'Modest'),
                 ('Tinkaton', 'Sitrus Berry', 'Adamant'), ('Gardevoir', 'Choice Specs', 'Modest')],
}
ABIL = {'Torkoal': 'Drought', 'Pelipper': 'Drizzle', 'Barraskewda': 'Swift Swim', 'Ludicolo': 'Swift Swim',
        'Kingdra': 'Swift Swim', 'Hariyama': 'Guts', 'Conkeldurr': 'Guts', 'Ursaring': 'Guts',
        'Reuniclus': 'Magic Guard', 'Hatterene': 'Magic Bounce', 'Salazzle': 'Corrosion',
        'Kangaskhan': 'Scrappy', 'Bewear': 'Fluffy', 'Crawdaunt': 'Adaptability', 'Sylveon': 'Pixilate',
        'Magnezone': 'Sturdy', 'Altaria': 'Natural Cure', 'Volcarona': 'Flame Body'}
SPECIAL = {'Modest', 'Timid', 'Quiet', 'Calm', 'Bold'}
REMATCH_ACE = {2: 60, 3: 65, 4: 70, 5: 75}
OFFS = [3, 3, 2, 2, 1, 0]


def rematches():
    out = ['/* ===================== REMATCHES (post-game, 55-75) ===================== */\n']
    for who, roster in ROSTERS.items():
        for n in (2, 3, 4, 5):
            tid = 'TRAINER_%s_%d' % (who, n)
            ace = REMATCH_ACE[n]
            s = '=== %s ===\nItems: Full Restore / Full Restore\n' % tid
            s += 'AI: Basic Trainer / Smart Switching / Smart Mon Choices / HP Aware / Ace Pokemon\n\n'
            mons = []
            for (sp, item, nat), off in zip(roster, OFFS):
                extra = []
                if sp in ABIL:
                    extra.append('Ability: ' + ABIL[sp])
                if n >= 4:
                    extra.append('EVs: 4 HP / 252 %s / 252 Spe' % ('SpA' if nat in SPECIAL else 'Atk'))
                mons.append(mon(sp, ace - off, item, nat, extra))
            s += '\n'.join(mons)
            out.append(s)
    return '\n'.join(out)


def main():
    core = open(os.path.join(HERE, 'bosses_core.party')).read()
    text = core.rstrip('\n') + '\n\n' + rivals() + '\n' + rematches()
    open(os.path.join(HERE, 'bosses.party'), 'w').write(text)
    print('wrote', os.path.join(HERE, 'bosses.party'))


if __name__ == '__main__':
    main()
