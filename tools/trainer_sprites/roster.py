"""Canonical roster for Pokemon Multiverse: league members, kahunas/captains,
professors and villain bosses of every region, with filename patterns used
to find candidate sprites in the source repositories.

key -> (region, role, display name, regex on the normalised file stem)
The stem is the lower-cased basename without extension and without the
`_front_pic` / `_front` suffix.
"""
import re

R = []


def add(region, role, key, name, pat=None):
    R.append((key, region, role, name, re.compile(pat or r'(^|_)%s($|_)' % key)))


# ---------------------------------------------------------------- Kanto
for k, n in [('brock', 'Brock'), ('misty', 'Misty'), ('erika', 'Erika'),
             ('sabrina', 'Sabrina'), ('blaine', 'Blaine')]:
    add('kanto', 'gym_leader', k, n)
add('kanto', 'gym_leader', 'lt_surge', 'Lt. Surge', r'(^|_)(lt_?surge|surge)($|_)')
add('kanto', 'gym_leader', 'koga', 'Koga')
add('kanto', 'gym_leader/boss', 'giovanni', 'Giovanni')
add('kanto', 'gym_leader (GSC/HGSS)', 'janine', 'Janine')
add('kanto', 'champion / gym_leader (GSC)', 'blue', 'Blue',
    r'(^|_)(blue|champion_rival|leader_blue|rival_late)($|_)')
add('kanto', 'elite_four', 'lorelei', 'Lorelei')
add('kanto', 'elite_four', 'bruno', 'Bruno')
add('kanto', 'elite_four', 'agatha', 'Agatha')
add('kanto', 'elite_four / champion (GSC)', 'lance', 'Lance')
add('kanto', 'champion (HGSS post-game) / player', 'red', 'Red', r'(^|_)(red|champion_red)($|_)')
add('kanto', 'professor', 'oak', 'Prof. Oak', r'(^|_)(oak|professor_oak)($|_)')
# ---------------------------------------------------------------- Johto
for k, n in [('falkner', 'Falkner'), ('bugsy', 'Bugsy'), ('whitney', 'Whitney'),
             ('morty', 'Morty'), ('chuck', 'Chuck'), ('jasmine', 'Jasmine'),
             ('pryce', 'Pryce'), ('clair', 'Clair')]:
    add('johto', 'gym_leader', k, n)
add('johto', 'elite_four', 'will', 'Will')
add('johto', 'elite_four', 'karen', 'Karen')
add('johto', 'professor', 'elm', 'Prof. Elm')
add('johto', 'rival', 'silver', 'Silver')
# ---------------------------------------------------------------- Hoenn
for k, n in [('roxanne', 'Roxanne'), ('brawly', 'Brawly'), ('wattson', 'Wattson'),
             ('flannery', 'Flannery'), ('norman', 'Norman'), ('winona', 'Winona'),
             ('juan', 'Juan')]:
    add('hoenn', 'gym_leader', k, n)
add('hoenn', 'gym_leader', 'tate_and_liza', 'Tate & Liza', r'(^|_)(tate_?and_?liza|tateandliza)($|_)')
add('hoenn', 'elite_four', 'sidney', 'Sidney')
add('hoenn', 'elite_four', 'phoebe', 'Phoebe')
add('hoenn', 'elite_four', 'glacia', 'Glacia')
add('hoenn', 'elite_four', 'drake', 'Drake')
add('hoenn', 'champion (E) / gym_leader (RS)', 'wallace', 'Wallace')
add('hoenn', 'champion (RS)', 'steven', 'Steven')
add('hoenn', 'professor', 'birch', 'Prof. Birch')
add('hoenn', 'villain_boss', 'maxie', 'Maxie')
add('hoenn', 'villain_boss', 'archie', 'Archie')
add('hoenn', 'rival', 'wally', 'Wally')
# ---------------------------------------------------------------- Sinnoh
for k, n in [('roark', 'Roark'), ('gardenia', 'Gardenia'), ('maylene', 'Maylene'),
             ('fantina', 'Fantina'), ('byron', 'Byron'), ('candice', 'Candice'),
             ('volkner', 'Volkner')]:
    add('sinnoh', 'gym_leader', k, n)
add('sinnoh', 'gym_leader', 'crasher_wake', 'Crasher Wake', r'(^|_)(crasher_?wake|wake)($|_)')
for k, n in [('aaron', 'Aaron'), ('bertha', 'Bertha'), ('flint', 'Flint'), ('lucian', 'Lucian')]:
    add('sinnoh', 'elite_four', k, n)
add('sinnoh', 'champion', 'cynthia', 'Cynthia')
add('sinnoh', 'professor', 'rowan', 'Prof. Rowan')
add('sinnoh', 'villain_boss', 'cyrus', 'Cyrus', r'(^|_)(cyrus|galactic_boss)($|_)')
add('sinnoh', 'rival', 'barry', 'Barry')
# ---------------------------------------------------------------- Unova
for k, n in [('cilan', 'Cilan'), ('chili', 'Chili'), ('cress', 'Cress'),
             ('lenora', 'Lenora'), ('burgh', 'Burgh'), ('elesa', 'Elesa'),
             ('clay', 'Clay'), ('skyla', 'Skyla'), ('brycen', 'Brycen'),
             ('drayden', 'Drayden'), ('roxie', 'Roxie'), ('marlon', 'Marlon')]:
    add('unova', 'gym_leader', k, n)
add('unova', 'gym_leader (BW2) / rival', 'cheren', 'Cheren')
add('unova', 'gym_leader (BW) / champion (BW2)', 'iris', 'Iris')
for k, n in [('shauntal', 'Shauntal'), ('grimsley', 'Grimsley'), ('caitlin', 'Caitlin'),
             ('marshal', 'Marshal')]:
    add('unova', 'elite_four', k, n)
add('unova', 'champion (BW)', 'alder', 'Alder')
add('unova', 'professor', 'juniper', 'Prof. Juniper', r'(^|_)juniper($|_)')
add('unova', 'villain_boss', 'ghetsis', 'Ghetsis')
add('unova', 'villain_boss', 'n', 'N', r'(^|/)n($|_)')
add('unova', 'villain_boss (BW2)', 'colress', 'Colress')
# ---------------------------------------------------------------- Kalos
for k, n in [('viola', 'Viola'), ('grant', 'Grant'), ('korrina', 'Korrina'),
             ('ramos', 'Ramos'), ('clemont', 'Clemont'), ('valerie', 'Valerie'),
             ('olympia', 'Olympia'), ('wulfric', 'Wulfric')]:
    add('kalos', 'gym_leader', k, n)
for k, n in [('malva', 'Malva'), ('siebold', 'Siebold'), ('wikstrom', 'Wikstrom'),
             ('drasna', 'Drasna')]:
    add('kalos', 'elite_four', k, n)
add('kalos', 'champion', 'diantha', 'Diantha')
add('kalos', 'professor', 'sycamore', 'Prof. Sycamore')
add('kalos', 'villain_boss', 'lysandre', 'Lysandre')
# ---------------------------------------------------------------- Alola
for k, n in [('ilima', 'Ilima'), ('lana', 'Lana'), ('kiawe', 'Kiawe'),
             ('mallow', 'Mallow'), ('sophocles', 'Sophocles'), ('mina', 'Mina')]:
    add('alola', 'captain', k, n)
add('alola', 'captain / elite_four', 'acerola', 'Acerola')
add('alola', 'kahuna / elite_four', 'hala', 'Hala')
add('alola', 'kahuna / elite_four', 'olivia', 'Olivia')
add('alola', 'kahuna', 'nanu', 'Nanu')
add('alola', 'kahuna', 'hapu', 'Hapu')
add('alola', 'elite_four', 'kahili', 'Kahili')
add('alola', 'elite_four (USUM)', 'molayne', 'Molayne')
add('alola', 'professor / champion (SM)', 'kukui', 'Prof. Kukui')
add('alola', 'champion (USUM) / rival', 'hau', 'Hau')
add('alola', 'villain_boss', 'lusamine', 'Lusamine')
add('alola', 'villain_boss', 'guzma', 'Guzma')
add('alola', 'rival', 'gladion', 'Gladion')
# ---------------------------------------------------------------- Galar
for k, n in [('milo', 'Milo'), ('nessa', 'Nessa'), ('kabu', 'Kabu'), ('bea', 'Bea'),
             ('allister', 'Allister'), ('opal', 'Opal'), ('gordie', 'Gordie'),
             ('melony', 'Melony'), ('piers', 'Piers'), ('raihan', 'Raihan')]:
    add('galar', 'gym_leader', k, n)
add('galar', 'gym_leader / rival', 'bede', 'Bede')
add('galar', 'gym_leader / rival', 'marnie', 'Marnie')
add('galar', 'champion', 'leon', 'Leon')
add('galar', 'rival', 'hop', 'Hop')
add('galar', 'professor', 'magnolia', 'Prof. Magnolia')
add('galar', 'professor', 'sonia', 'Prof. Sonia')
add('galar', 'villain_boss', 'rose', 'Chairman Rose')
# ---------------------------------------------------------------- Paldea
for k, n in [('katy', 'Katy'), ('brassius', 'Brassius'), ('iono', 'Iono'),
             ('kofu', 'Kofu'), ('ryme', 'Ryme'), ('tulip', 'Tulip'), ('grusha', 'Grusha')]:
    add('paldea', 'gym_leader', k, n)
add('paldea', 'gym_leader / elite_four', 'larry', 'Larry')
for k, n in [('rika', 'Rika'), ('poppy', 'Poppy'), ('hassel', 'Hassel')]:
    add('paldea', 'elite_four', k, n)
add('paldea', 'top_champion', 'geeta', 'Geeta')
add('paldea', 'champion / rival', 'nemona', 'Nemona')
add('paldea', 'professor / final boss', 'sada', 'Prof. Sada')
add('paldea', 'professor / final boss', 'turo', 'Prof. Turo')
add('paldea', 'villain_boss (Team Star)', 'penny', 'Penny')
add('paldea', 'rival', 'arven', 'Arven')
add('paldea', 'director', 'clavell', 'Director Clavell')

REGIONS = ['kanto', 'johto', 'hoenn', 'sinnoh', 'unova', 'kalos', 'alola', 'galar', 'paldea']


def stem(path):
    s = path.lower().replace('\\', '/').rsplit('/', 1)[-1]
    s = s.rsplit('.', 1)[0]
    for suf in ('_front_pic', '_front', '_frontpic'):
        if s.endswith(suf):
            s = s[: -len(suf)]
    return s
