#!/usr/bin/env python3
"""Lists every Kanto text label that still holds a placeholder, per file (for the writers).

Two kinds of placeholder:
  * "[TESTO: ...]"                 written by kanto_trainers.py / kanto_story.py (description inside)
  * "... (testo Kanto da scrivere)" written by port_kanto.py for NPCs, signs and objects

    python3 -I tools/kanto/list_placeholders.py --tree /home/user/pex --out docs/kanto/testi_da_scrivere.md
"""
import argparse, glob, json, os, re

HERE = os.path.dirname(os.path.abspath(__file__))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--tree', default='/home/user/pex')
    ap.add_argument('--out', default=os.path.join(HERE, '..', '..', 'docs', 'kanto', 'testi_da_scrivere.md'))
    a = ap.parse_args()
    inv = {m['map']: m for m in json.load(open(os.path.join(HERE, '..', '..', 'docs/kanto/mappe.json')))['maps']}
    files = sorted(glob.glob(os.path.join(a.tree, 'data/maps/*_Frlg/scripts.inc')))
    files += [os.path.join(a.tree, 'data/scripts/kanto_story.inc'), os.path.join(a.tree, 'data/scripts/kanto_travel.inc')]
    rows, n_t, n_s = [], 0, 0
    for f in files:
        mp = os.path.basename(os.path.dirname(f))
        if mp.endswith('_Frlg') and mp not in inv:
            continue  # Sevii / not built
        s = open(f, encoding='utf-8').read()
        items = []
        for m in re.finditer(r'(?m)^(\w+)::?\n((?:\t\.string [^\n]*\n)+)', s):
            body = ''.join(re.findall(r'"([^"]*)"', m.group(2)))
            if '[TESTO:' in body:
                d = body.replace('\\n', ' ').replace('\\l', ' ').replace('\\p', ' ').replace('$', '')
                items.append('`%s` — %s' % (m.group(1), d.strip()))
                n_t += 1
            elif 'testo Kanto da scrivere' in body:
                items.append('`%s`' % m.group(1))
                n_s += 1
        if items:
            name = inv[mp]['name_it'] if mp in inv else ''
            rows.append((os.path.relpath(f, a.tree), name, items))
    out = ['# Kanto: testi da scrivere', '',
           'Generato da `tools/kanto/list_placeholders.py` (rilanciarlo dopo ogni consegna dei testi).', '',
           '* `[TESTO: …]`: segnaposto con la descrizione di cosa deve dire il testo (trama, allenatori, '
           'Palestre, Lega, gating). %d etichette.' % n_t,
           '* solo etichetta: segnaposto generico del port (NPC, cartelli, oggetti), "(testo Kanto da scrivere)". '
           '%d etichette.' % n_s, '',
           'Regole: una etichetta = un testo; non rinominare le etichette; ~36 caratteri per riga; '
           '`\\n` nuova riga, `\\l` scorre, `\\p` nuovo riquadro; chiudere con `$`. '
           'Nei blocchi `@ KANTO_V2 BEGIN/END` i testi già scritti (senza "[TESTO:") vengono conservati se '
           'si rilancia `kanto_story.py`.', '']
    for path, name, items in rows:
        out.append('## %s%s' % (path, (' — ' + name) if name else ''))
        out.append('')
        out += ['- ' + i for i in items]
        out.append('')
    with open(a.out, 'w', encoding='utf-8') as f:
        f.write('\n'.join(out))
    print('%d files, %d [TESTO] + %d stub placeholders -> %s' % (len(rows), n_t, n_s, a.out))


if __name__ == '__main__':
    main()
