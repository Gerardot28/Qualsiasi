#!/usr/bin/env python3
"""sdb.py - tiny loader for the species database produced by speciesdb.py.

From another script (works with python3 -I, no cwd imports):

    import sys
    sys.path.insert(0, '/home/user/Qualsiasi/tools/speciesdb')
    import sdb
    db = sdb.load()                        # default /home/user/work/species.json
    pika = db['SPECIES_PIKACHU']           # aliases accepted (SPECIES_CASTFORM ...)
    wild = db.encounterable(gen=3)         # list of records
    fam = db.family_of('SPECIES_RAICHU')   # family dict from families.json
    db.moves_at_level('SPECIES_PIKACHU', 20)   # last 4 level-up moves <= 20
    db.evolve_for_level('SPECIES_PICHU', 40)   # e.g. 'SPECIES_RAICHU'

CLI:  python3 -I sdb.py SPECIES_X [...]   prints a short summary of each species.
"""

import json
import os
import random
import sys

DEFAULT_DB = '/home/user/work/species.json'
DEFAULT_FAMILIES = '/home/user/work/families.json'


class SpeciesDB:
    def __init__(self, data, families=None):
        self.meta = data['meta']
        self.species = data['species']
        self.families = families or []
        self.alias = {}
        for name, rec in self.species.items():
            for a in rec.get('aliases', []):
                self.alias[a] = name
        self._fam_by_root = {f['root']: f for f in self.families}

    # -- lookup --------------------------------------------------------
    def resolve(self, name):
        """Canonical constant for a SPECIES_ name or alias (KeyError if unknown)."""
        name = self.alias.get(name, name)
        if name not in self.species:
            raise KeyError(name)
        return name

    def __getitem__(self, name):
        return self.species[self.resolve(name)]

    def __contains__(self, name):
        return self.alias.get(name, name) in self.species

    def __iter__(self):
        return iter(self.species.values())

    def __len__(self):
        return len(self.species)

    # -- filters -------------------------------------------------------
    def encounterable(self, gen=None, legendaryish=None, final=None):
        out = []
        for r in self.species.values():
            if not r['encounterable']:
                continue
            if gen is not None:
                gens = gen if isinstance(gen, (list, tuple, set, frozenset)) else (gen,)
                if r['generation'] not in gens:
                    continue
            if legendaryish is not None and r['isLegendaryish'] != legendaryish:
                continue
            if final is not None and r['finalStage'] != final:
                continue
            out.append(r)
        return out

    def family_of(self, name):
        return self._fam_by_root.get(self[name]['familyRoot'])

    def by_type(self, type_const, encounterable_only=True):
        return [r for r in self.species.values()
                if type_const in r['types'] and (r['encounterable'] or not encounterable_only)]

    # -- gameplay helpers ----------------------------------------------
    def moves_at_level(self, name, level, n=4):
        """Default moveset the game gives a Pokemon of this level, exactly like
        GiveBoxMonInitialMoveset (src/pokemon.c): walk the level-up learnset in
        order up to `level`, skip level-0 (evolution) moves and moves already
        known, and keep only the last n."""
        moves = []
        for lv, mv in self[name]['levelUpLearnset']:
            if lv > level:
                break
            if lv == 0 or mv in moves:
                continue
            moves.append(mv)
            if len(moves) > n:
                moves.pop(0)
        return moves

    def evolve_for_level(self, name, level, rng=None, encounterable_only=True):
        """Follow evolutions while the target's suggestedMinLevel <= level.
        Branches are chosen at random (rng: random.Random) among valid ones."""
        rng = rng or random
        cur = self.resolve(name)
        seen = set()
        while cur not in seen:
            seen.add(cur)
            opts = [t for t in self.species[cur]['evolvesInto']
                    if t in self.species and self.species[t]['suggestedMinLevel'] <= level
                    and (self.species[t]['encounterable'] or not encounterable_only)]
            if not opts:
                break
            cur = rng.choice(sorted(opts))
        return cur


def load(path=DEFAULT_DB, families_path=DEFAULT_FAMILIES):
    with open(path, encoding='utf-8') as f:
        data = json.load(f)
    fams = []
    if families_path and os.path.exists(families_path):
        with open(families_path, encoding='utf-8') as f:
            fams = json.load(f)['families']
    return SpeciesDB(data, fams)


def _summary(db, name):
    r = db[name]
    ev = ', '.join('%s %s->%s' % (e['method'], e['param'], e['target']) for e in r['evolutions']) or '-'
    return ('%s (%s) #%s gen%s %s BST %d | %s | root %s stage %d | encounterable=%s%s\n  evolutions: %s' % (
        r['constant'], r['speciesName'], r['natDexNum'], r['generation'], '/'.join(r['types']), r['bst'],
        r['formKind'], r['familyRoot'], r['stage'], r['encounterable'],
        (' (' + r['excludeReason'] + ')') if r['excludeReason'] else '', ev))


if __name__ == '__main__':
    db = load()
    if len(sys.argv) < 2:
        c = db.meta['counts']
        print('%d species, %d encounterable, %d families' % (c['totalSpeciesEntries'], c['encounterable'], c['families']))
    for n in sys.argv[1:]:
        try:
            print(_summary(db, n if n.startswith('SPECIES_') else 'SPECIES_' + n.upper()))
        except KeyError:
            print('unknown species %s' % n)
