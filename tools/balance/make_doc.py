#!/usr/bin/env python3
"""Writes docs/bilanciamento/allenatori.md (Italian boss summary) from a generated trainers.party.

    python3 -I tools/balance/make_doc.py /path/trainers.party > docs/bilanciamento/allenatori.md
"""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import trainer_gen as tg
d = tg.db(); mv = tg.mdb.load_moves(); it = tg.mdb.load_items()
_, ents = tg.parse_party(open(sys.argv[1]).read()); E = {e['id']: e for e in ents}
def nm(sp): return d.raw[sp]['speciesName']
def team(tid):
    e = E[tid]; rows = []
    for b in e['mons']:
        m = re.match(r'^(.*?)(?:\s+@\s+(.*))?$', b[0]); sp = m.group(1)
        f = dict(l.split(': ', 1) for l in b[1:] if not l.startswith('- ') and ': ' in l)
        moves = ', '.join(mv[x[2:]]['name'] for x in b[1:] if x.startswith('- '))
        item = it[m.group(2)]['name'] if m.group(2) else '—'
        ab = f.get('Ability', '').replace('ABILITY_', '').replace('_', ' ').title() or '—'
        rows.append('| %s | %s | %s | %s | %s | %s |' % (nm(sp), f['Level'], item, ab, f.get('Nature', '—'), moves))
    hdr = e['header']; items = tg.header_get(hdr, 'Items') or '—'; ai = tg.header_get(hdr, 'AI')
    dbl = ' (lotta doppia)' if tg.is_double(hdr) else ''
    out = ['**Strumenti:** %s · **IA:** %s%s' % (items, ai, dbl), '',
           '| Pokémon | Liv. | Strumento | Abilità | Natura | Mosse |', '|---|---|---|---|---|---|'] + rows + ['']
    return '\n'.join(out)
P = print
P('# Bilanciamento allenatori — Pokémon Multiverse\n')
P('Generato da `tools/balance/trainer_gen.py` + `tools/balance/bosses.party` (pokeemerald-expansion 1.17.1). '
  'Difficoltà: elevata ma equa. Pokémon di tutte le generazioni (nessun leggendario/misterioso/UC/paradosso).\n')
P('## Tetti di livello e assi\n')
P('| Sfida | Tetto | Livello asso |\n|---|---|---|')
for a,b in [('Roxanne',15),('Brawly',19),('Wattson',24),('Flannery',29),('Norman',33),('Winona',37),('Tell & Pat (Tate & Liza)',44),('Juan',49),('Sidney',54),('Phoebe',56),('Glacia',58),('Drake',60),('Campione Wallace',62)]:
    P('| %s | %d | %d |' % (a, b, b))
P('')
P('## Capipalestra (prima sfida)\n')
for t,n in [('TRAINER_ROXANNE_1','Roxanne — Roccia'),('TRAINER_BRAWLY_1','Brawly — Lotta'),('TRAINER_WATTSON_1','Wattson — Elettro'),('TRAINER_FLANNERY_1','Flannery — Fuoco'),('TRAINER_NORMAN_1','Norman — Normale'),('TRAINER_WINONA_1','Winona — Volante'),('TRAINER_TATE_AND_LIZA_1','Tell & Pat — Psico (doppia)'),('TRAINER_JUAN_1','Juan — Acqua')]:
    P('### %s\n' % n); P(team(t))
P('## Superquattro e Campione\n')
for t,n in [('TRAINER_SIDNEY','Sidney — Buio'),('TRAINER_PHOEBE','Phoebe — Spettro'),('TRAINER_GLACIA','Glacia — Ghiaccio'),('TRAINER_DRAKE','Drake — Drago'),('TRAINER_WALLACE','Campione Wallace')]:
    P('### %s\n' % n); P(team(t))
P('## Rivale (May / Brendan)\n')
P('Il suffisso della variante vanilla indica lo starter del **giocatore** (VAR_STARTER_MON: 0 = _TREECKO = Turtwig, 1 = _TORCHIC = Fuecoco, 2 = _MUDKIP = Froakie). '
  'Il rivale usa lo starter che lo batte: giocatore Turtwig → rivale linea Fuecoco; giocatore Fuecoco → rivale linea Froakie; giocatore Froakie → rivale linea Turtwig. May e Brendan hanno squadre identiche.\n')
for var, lab in [('TREECKO','Giocatore Turtwig → rivale Fuecoco'),('TORCHIC','Giocatore Fuecoco → rivale Froakie'),('MUDKIP','Giocatore Froakie → rivale Turtwig')]:
    P('### %s\n' % lab)
    for st, place in [('ROUTE_103','Percorso 103'),('RUSTBORO','Ferrugipoli (Rustboro)'),('ROUTE_110','Percorso 110'),('ROUTE_119','Percorso 119'),('LILYCOVE','Porto Alghepoli (Lilycove)')]:
        e = E['TRAINER_MAY_%s_%s' % (st, var)]
        s = ', '.join('%s %s' % (nm(re.match(r'^(\S+)', b[0]).group(1)), b[1].split(': ')[1]) for b in e['mons'])
        P('- **%s:** %s' % (place, s))
    P('')
P('## Wally\n')
for t,n in [('TRAINER_WALLY_MAUVILLE','Ciclamipoli (Mauville)'),('TRAINER_WALLY_VR_1','Via Vittoria')]:
    P('### %s\n' % n); P(team(t))
P('## Team Magma e Team Aqua\n')
for t,n in [('TRAINER_TABITHA_MT_CHIMNEY','Tabitha — Monte Camino'),('TRAINER_MAXIE_MT_CHIMNEY','Max — Monte Camino'),('TRAINER_TABITHA_MAGMA_HIDEOUT','Tabitha — Covo Magma'),('TRAINER_MAXIE_MAGMA_HIDEOUT','Max — Covo Magma'),('TRAINER_TABITHA_MOSSDEEP','Tabitha — Centro Spaziale (multi, 3 Pokémon)'),('TRAINER_MAXIE_MOSSDEEP','Max — Centro Spaziale (multi, 3 Pokémon)'),('TRAINER_SHELLY_WEATHER_INSTITUTE','Ada (Shelly) — Istituto Meteo'),('TRAINER_MATT','Alan (Matt) — Covo Idro'),('TRAINER_SHELLY_SEAFLOOR_CAVERN','Ada (Shelly) — Grotta dei Fondali'),('TRAINER_ARCHIE','Ivan (Archie) — Grotta dei Fondali')]:
    P('### %s\n' % n); P(team(t))
P('## Rivincite (post-game)\n')
P('Rivincite _2/_3/_4/_5 di capipalestra e Wally (Via Vittoria): 6 Pokémon, asso a Lv 60/65/70/75, gli altri 1-3 livelli sotto; IV 31, EV da _4. Mosse generate con `mdb.best_moveset` (MT ammesse).\n')
for who in ['ROXANNE','BRAWLY','WATTSON','FLANNERY','NORMAN','WINONA','TATE_AND_LIZA','JUAN','WALLY_VR']:
    e = E['TRAINER_%s_5' % who]
    s = ', '.join(nm(re.match(r'^(\S+)', b[0]).group(1)) for b in e['mons'])
    P('- **%s:** %s' % (who.replace('_',' ').title(), s))
P('')
P('### Steven (Cascate Meteora, post-game)\n'); P(team('TRAINER_STEVEN'))
P('## Allenatori comuni\n')
P('- Livello = livello vanilla mappato sulla curva (2→2, 5→6, 12→13, 15→15, 19→19, 24→24, 29→29, 31→33, 33→37, 42→44, 46→49, 49→52, 55→57, 58→62, 70→72, 100→100), interpolata linearmente; le rivincite usano la stessa curva.')
P('- Specie a tema per classe, da tutte le generazioni; stadio evolutivo coerente col livello (evoluzioni per livello al livello di evoluzione; pietra/scambio/amicizia: 1° stadio ≥ 30, 2° stadio ≥ 38); pseudo-leggendari solo da Lv 35 e solo per Fantallenatori/Esperti/Domadraghi.')
P('- Mosse: `mdb.best_moveset` (normal_trainer=True; MT per classi forti e da Lv 30; sotto Lv 30 le mosse MT/uovo più potenti di 60/75 vengono sostituite). IV da 8 a 20 secondo il livello, fino a 25 nelle rivincite post-game. IA: Basic Trainer.')
