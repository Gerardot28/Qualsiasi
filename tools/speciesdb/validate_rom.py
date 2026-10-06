#!/usr/bin/env python3
"""validate_rom.py - cross-check species.json against a compiled ROM.

Reads gSpeciesInfo (and the evolution / learnset / form tables it points to)
straight out of a built ROM, using the ELF symbol table for the address and a
probe object compiled with arm-none-eabi-gcc for the exact struct layout
(offsets, bitfield positions).  Every value the parser produced is compared
with what the compiler actually emitted.

Usage:
    python3 -I validate_rom.py --rom baseline.gba --elf pokeemerald.elf \
        [--root /home/user/pex-orig] [--db /home/user/work/species.json]

The ROM/ELF must have been built from the same tree/config as the DB.
Exit status 0 = no mismatches.
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import speciesdb  # noqa: E402
from cparse import CSource  # noqa: E402

ROM_BASE = 0x08000000

# struct SpeciesInfo scalar fields (bitfield or not) probed by setting all bits
SCALAR_FIELDS = [
    'baseHP', 'baseAttack', 'baseDefense', 'baseSpeed', 'baseSpAttack', 'baseSpDefense',
    'catchRate', 'expYield', 'evYield_HP', 'evYield_Attack', 'evYield_Defense', 'evYield_Speed',
    'evYield_SpAttack', 'evYield_SpDefense', 'itemCommon', 'itemRare', 'genderRatio', 'eggCycles',
    'friendship', 'growthRate', 'natDexNum', 'height', 'weight', 'bodyColor', 'perfectIVCount',
    'forceTeraType',
] + speciesdb.FLAG_FIELDS
ARRAY_FIELDS = ['types', 'eggGroups', 'abilities', 'speciesName']
POINTER_FIELDS = ['levelUpLearnset', 'eggMoveLearnset', 'evolutions', 'formSpeciesIdTable', 'formChangeTable']
OTHER_STRUCTS = {
    'Evolution': (['method', 'param', 'targetSpecies'], ['params']),
    'EvolutionParam': (['condition', 'arg1', 'arg2', 'arg3'], []),
    'LevelUpMove': (['move', 'level'], []),
    'FormChange': (['method', 'targetSpecies', 'param1', 'param2', 'param3', 'param4'], []),
}


def tool(name):
    p = shutil.which(name)
    if not p:
        raise SystemExit('%s not found in PATH' % name)
    return p


def probe_layout(root):
    """Compile a probe object and derive offsets/bitfield masks."""
    lines = ['#include "global.h"', '#include "pokemon.h"', '#include <stddef.h>',
             '#pragma GCC diagnostic ignored "-Woverflow"',
             'const unsigned int gProbeOffsets[] = {',
             '  sizeof(struct SpeciesInfo),']
    names = ['__size__']
    for f in ARRAY_FIELDS:
        lines.append('  offsetof(struct SpeciesInfo, %s), sizeof(((struct SpeciesInfo *)0)->%s[0]),'
                     ' sizeof(((struct SpeciesInfo *)0)->%s),' % (f, f, f))
        names += [f, f + '.elem', f + '.size']
    for f in POINTER_FIELDS:
        lines.append('  offsetof(struct SpeciesInfo, %s),' % f)
        names.append(f)
    for st, (_, ptrs) in OTHER_STRUCTS.items():
        lines.append('  sizeof(struct %s),' % st)
        names.append(st + '.__size__')
        for f in ptrs:
            lines.append('  offsetof(struct %s, %s),' % (st, f))
            names.append(st + '.' + f)
    lines.append('};')
    objs = []
    for f in SCALAR_FIELDS:
        sym = 'gProbe_SpeciesInfo_' + f
        lines.append('const struct SpeciesInfo %s = { .%s = (unsigned)-1 };' % (sym, f))
        objs.append(('SpeciesInfo', f, sym))
    for st, (fields, _) in OTHER_STRUCTS.items():
        for f in fields:
            sym = 'gProbe_%s_%s' % (st, f)
            lines.append('const struct %s %s = { .%s = (unsigned)-1 };' % (st, sym, f))
            objs.append((st, f, sym))
    with tempfile.TemporaryDirectory(prefix='speciesdb_probe_') as td:
        c = os.path.join(td, 'probe.c')
        with open(c, 'w') as fh:
            fh.write('\n'.join(lines) + '\n')
        stub = os.path.join(td, 'stub')
        os.makedirs(stub)
        obj = os.path.join(td, 'probe.o')
        for _ in range(50):
            cmd = [tool('arm-none-eabi-gcc'), '-c', '-O0', '-mthumb', '-mthumb-interwork', '-mabi=apcs-gnu',
                   '-mtune=arm7tdmi', '-march=armv4t', '-std=gnu17', '-w',
                   '-iquote', os.path.join(root, 'include'), '-iquote', stub,
                   '-DMODERN=1', '-DTESTING=0', '-D' + speciesdb.makefile_game_version(root), c, '-o', obj]
            p = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            if p.returncode == 0:
                break
            err = p.stderr.decode('utf-8', 'replace')
            m = re.search(r'fatal error: ([^\s:]+): No such file', err)
            if not m:
                raise SystemExit('probe compile failed:\n' + err[-3000:])
            path = os.path.join(stub, m.group(1))
            os.makedirs(os.path.dirname(path), exist_ok=True)
            open(path, 'w').close()
        else:
            raise SystemExit('probe compile: too many missing headers')
        binf = os.path.join(td, 'rodata.bin')
        subprocess.run([tool('arm-none-eabi-objcopy'), '-O', 'binary', '-j', '.rodata', obj, binf], check=True)
        data = open(binf, 'rb').read()
        nm = subprocess.run([tool('arm-none-eabi-nm'), '-S', obj], stdout=subprocess.PIPE, check=True).stdout.decode()
    syms = {}
    for line in nm.splitlines():
        parts = line.split()
        if len(parts) == 4:
            syms[parts[3]] = (int(parts[0], 16), int(parts[1], 16))
    off, size = syms['gProbeOffsets']
    vals = [int.from_bytes(data[off + 4 * i: off + 4 * i + 4], 'little') for i in range(size // 4)]
    layout = {'offsets': dict(zip(names, vals)), 'masks': {}}
    for st, f, sym in objs:
        o, sz = syms[sym]
        v = int.from_bytes(data[o:o + sz], 'little')
        if v == 0:
            raise SystemExit('probe for %s.%s produced no bits' % (st, f))
        shift = (v & -v).bit_length() - 1
        layout['masks'][(st, f)] = (shift, v >> shift)
    return layout


def elf_symbols(elf):
    out = subprocess.run([tool('arm-none-eabi-nm'), '-S', elf], stdout=subprocess.PIPE, check=True).stdout.decode()
    syms = {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 4:
            syms[parts[3]] = (int(parts[0], 16), int(parts[1], 16))
    return syms


def load_charmap(root):
    cm = {}
    with open(os.path.join(root, 'charmap.txt'), encoding='utf-8') as f:
        for line in f:
            m = re.match(r"^'(.)'\s*=\s*([0-9A-Fa-f ]+)", line)
            if m and m.group(1) not in cm:
                cm[m.group(1)] = bytes(int(x, 16) for x in m.group(2).split())
            m = re.match(r"^'\\(.)'\s*=\s*([0-9A-Fa-f ]+)", line)
            if m:
                ch = {'n': '\n', "'": "'", '\\': '\\'}.get(m.group(1), m.group(1))
                cm.setdefault(ch, bytes(int(x, 16) for x in m.group(2).split()))
    return cm


class Rom:
    def __init__(self, path):
        self.b = open(path, 'rb').read()

    def u(self, addr, n):
        o = addr - ROM_BASE
        return int.from_bytes(self.b[o:o + n], 'little')

    def raw(self, addr, n):
        o = addr - ROM_BASE
        return self.b[o:o + n]


def main(argv=None):
    ap = argparse.ArgumentParser(description='Cross-check species.json against a built ROM')
    ap.add_argument('--root', default=speciesdb.DEFAULT_ROOT)
    ap.add_argument('--db', default=speciesdb.DEFAULT_OUT)
    ap.add_argument('--rom', required=True)
    ap.add_argument('--elf', required=True)
    ap.add_argument('--max-report', type=int, default=60)
    args = ap.parse_args(argv)

    db = json.load(open(args.db, encoding='utf-8'))['species']
    log = speciesdb.log
    log('probing struct layout with arm-none-eabi-gcc ...')
    lay = probe_layout(args.root)
    off, masks = lay['offsets'], lay['masks']
    log('sizeof(SpeciesInfo)=%d Evolution=%d LevelUpMove=%d FormChange=%d' % (
        off['__size__'], off['Evolution.__size__'], off['LevelUpMove.__size__'], off['FormChange.__size__']))
    log('re-reading enum values from the preprocessed tree ...')
    text, _, _, _ = speciesdb.preprocess(args.root, speciesdb.find_cpp(None))
    enums = CSource(text).enums
    syms = elf_symbols(args.elf)
    rom = Rom(args.rom)
    base, tsize = syms['gSpeciesInfo']
    stride = off['__size__']
    n_entries = tsize // stride
    log('gSpeciesInfo @%08X, %d entries of %d bytes' % (base, n_entries, stride))
    cm = load_charmap(args.root)

    mismatches = []
    stats = {'species': 0, 'fields': 0, 'evolutions': 0, 'conditions': 0, 'learnset_moves': 0,
             'egg_moves': 0, 'form_entries': 0, 'form_changes': 0, 'unresolved_symbols': set()}

    def val(x):
        """symbolic -> int using enum table; None if unknown."""
        if isinstance(x, bool):
            return int(x)
        if isinstance(x, int):
            return x
        if x is None:
            return None
        if x in ('MON_MALE',):
            return 0
        if x in ('MON_FEMALE',):
            return 0xFE
        v = enums.get(x)
        if v is None:
            stats['unresolved_symbols'].add(x)
        return v

    def cmp(sp, what, expected, actual):
        stats['fields'] += 1
        if expected is None:
            return
        if expected != actual:
            mismatches.append('%s %s: db=%r rom=%r' % (sp, what, expected, actual))

    def field(entry_addr, st, f):
        shift, mask = masks[(st, f)]
        nbytes = (shift + mask.bit_length() + 7) // 8
        return (rom.u(entry_addr, nbytes) >> shift) & mask

    def sfield(entry, f):
        return field(entry, 'SpeciesInfo', f)

    def ptr(entry, f):
        return rom.u(entry + off[f], 4)

    # which ROM entries are populated?
    populated = set()
    for i in range(n_entries):
        e = base + i * stride
        if any(rom.raw(e, stride)):
            populated.add(i)
    db_ids = {r['id'] for r in db.values()}
    extra = sorted(populated - db_ids - {0, n_entries - 1})
    missing = sorted(db_ids - populated)
    if extra:
        mismatches.append('ROM has populated gSpeciesInfo entries not in DB: %s' % extra[:20])
    if missing:
        mismatches.append('DB species with empty ROM entry: %s' % missing[:20])

    stat_map = dict(speciesdb.STAT_FIELDS)
    ev_map = dict(speciesdb.EV_FIELDS)
    for name, r in db.items():
        stats['species'] += 1
        e = base + r['id'] * stride
        for k, f in stat_map.items():
            cmp(name, f, r['baseStats'][k], sfield(e, f))
        for k, f in ev_map.items():
            cmp(name, f, r['evYield'][k], sfield(e, f))
        for f in ('catchRate', 'expYield', 'genderRatio', 'eggCycles', 'friendship', 'height', 'weight',
                  'perfectIVCount'):
            cmp(name, f, r[f], sfield(e, f))
        cmp(name, 'natDexNum', r['natDexNum'], sfield(e, 'natDexNum'))
        cmp(name, 'growthRate', val(r['growthRate']), sfield(e, 'growthRate'))
        cmp(name, 'bodyColor', val(r['bodyColor']), sfield(e, 'bodyColor'))
        cmp(name, 'itemCommon', val(r['itemCommon'] or 'ITEM_NONE'), sfield(e, 'itemCommon'))
        cmp(name, 'itemRare', val(r['itemRare'] or 'ITEM_NONE'), sfield(e, 'itemRare'))
        cmp(name, 'forceTeraType', val(r['forceTeraType']) if r['forceTeraType'] else 0, sfield(e, 'forceTeraType'))
        for fl in speciesdb.FLAG_FIELDS:
            cmp(name, fl, int(r['flags'][fl]), sfield(e, fl))
        # arrays
        el = off['types.elem']
        rt = [rom.u(e + off['types'] + i * el, el) for i in range(off['types.size'] // el)]
        dbt = [val(t) for t in r['types']]
        cmp(name, 'types', dbt if len(dbt) == 2 else dbt * 2, rt)
        el = off['eggGroups.elem']
        rg = [rom.u(e + off['eggGroups'] + i * el, el) for i in range(off['eggGroups.size'] // el)]
        dbg = [val(t) for t in r['eggGroups']]
        cmp(name, 'eggGroups', dbg if len(dbg) == 2 else dbg * 2, rg)
        el = off['abilities.elem']
        ra = [rom.u(e + off['abilities'] + i * el, el) for i in range(off['abilities.size'] // el)]
        cmp(name, 'abilities', [val(a) for a in r['abilities']], ra)
        # name
        raw = rom.raw(e + off['speciesName'], off['speciesName.size'])
        raw = raw.split(b'\xff')[0]
        enc = b''
        ok = True
        for ch in r['speciesName'] or '':
            if ch not in cm:
                ok = False
                break
            enc += cm[ch]
        if ok:
            cmp(name, 'speciesName', enc, raw)
        else:
            mismatches.append('%s speciesName %r has chars missing from charmap' % (name, r['speciesName']))
        # evolutions
        p = ptr(e, 'evolutions')
        rom_evos = []
        if p:
            esz = off['Evolution.__size__']
            for i in range(64):
                a = p + i * esz
                m = field(a, 'Evolution', 'method')
                if m == 0xFFFF:
                    break
                prm = field(a, 'Evolution', 'param')
                tgt = field(a, 'Evolution', 'targetSpecies')
                pp = rom.u(a + off['Evolution.params'], 4)
                conds = []
                if pp:
                    csz = off['EvolutionParam.__size__']
                    for j in range(16):
                        ca = pp + j * csz
                        cond = field(ca, 'EvolutionParam', 'condition')
                        if cond == enums['CONDITIONS_END']:
                            break
                        conds.append([cond] + [field(ca, 'EvolutionParam', x) for x in ('arg1', 'arg2', 'arg3')])
                rom_evos.append((m, prm, tgt, conds))
        cmp(name, 'evolutions.count', len(r['evolutions']), len(rom_evos))
        for evo, rev in zip(r['evolutions'], rom_evos):
            stats['evolutions'] += 1
            cmp(name, 'evo.method', val(evo['method']), rev[0])
            pv = val(evo['param'])
            if pv is not None:
                cmp(name, 'evo.param(%s)' % evo['param'], pv & 0xFFFF, rev[1])
            cmp(name, 'evo.target', val(evo['target']), rev[2])
            cmp(name, 'evo.conditions.count', len(evo['conditions']), len(rev[3]))
            for c, rc in zip(evo['conditions'], rev[3]):
                stats['conditions'] += 1
                cmp(name, 'cond', val(c['condition']), rc[0])
                for k, a in enumerate(c['args']):
                    av = val(a)
                    if av is not None:
                        cmp(name, 'cond.arg%d(%s)' % (k + 1, a), av & 0xFFFF, rc[k + 1])
        # level up learnset
        p = ptr(e, 'levelUpLearnset')
        rom_moves = []
        if p:
            lsz = off['LevelUpMove.__size__']
            for i in range(200):
                a = p + i * lsz
                mv = field(a, 'LevelUpMove', 'move')
                if mv == 0xFFFF:
                    break
                rom_moves.append([field(a, 'LevelUpMove', 'level'), mv])
        dbm = [[lv, val(mv)] for lv, mv in r['levelUpLearnset']]
        stats['learnset_moves'] += len(dbm)
        cmp(name, 'levelUpLearnset', dbm, rom_moves)
        # egg moves
        p = ptr(e, 'eggMoveLearnset')
        rom_egg = []
        if p:
            for i in range(200):
                mv = rom.u(p + 2 * i, 2)
                if mv == 0xFFFF:
                    break
                rom_egg.append(mv)
        stats['egg_moves'] += len(r['eggMoves'])
        cmp(name, 'eggMoves', [val(m) for m in r['eggMoves']], rom_egg)
        # forms
        p = ptr(e, 'formSpeciesIdTable')
        rom_forms = []
        if p:
            for i in range(200):
                v = rom.u(p + 2 * i, 2)
                if v == 0xFFFF:
                    break
                rom_forms.append(v)
        stats['form_entries'] += len(r['forms'])
        cmp(name, 'forms', [val(x) for x in r['forms']], rom_forms)
        p = ptr(e, 'formChangeTable')
        rom_fc = []
        if p:
            fsz = off['FormChange.__size__']
            for i in range(64):
                a = p + i * fsz
                m = field(a, 'FormChange', 'method')
                if m == enums['FORM_CHANGE_TERMINATOR']:
                    break
                rom_fc.append([m, field(a, 'FormChange', 'targetSpecies')] +
                              [field(a, 'FormChange', 'param%d' % k) for k in range(1, 5)])
        cmp(name, 'formChanges.count', len(r['formChanges']), len(rom_fc))
        for ch, rch in zip(r['formChanges'], rom_fc):
            stats['form_changes'] += 1
            cmp(name, 'fc.method', val(ch['method']), rch[0])
            cmp(name, 'fc.target', val(ch['target']), rch[1])
            for k, prm in enumerate(ch['params'][:4]):
                pv = val(prm)
                if pv is not None:
                    cmp(name, 'fc.param%d(%s)' % (k + 1, prm), pv & 0xFFFF, rch[2 + k])

    unresolved = sorted(stats.pop('unresolved_symbols'))
    log('checked: %s' % json.dumps(stats))
    if unresolved:
        log('symbols without a value (stubbed generated headers, not compared): %d e.g. %s' % (
            len(unresolved), ', '.join(unresolved[:8])))
    log('MISMATCHES: %d' % len(mismatches))
    for m in mismatches[:args.max_report]:
        log('  ' + m)
    return 1 if mismatches else 0


if __name__ == '__main__':
    sys.exit(main())
