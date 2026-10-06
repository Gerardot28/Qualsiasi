#!/usr/bin/env python3
"""ctextcheck.py - validator for translated C source strings of pokeemerald-expansion.

Compares C files of a modified tree (--root) with a pristine tree (--orig) after
the contents of game-string literals (_("..."), __("..."), COMPOUND_STRING("..."),
ITEM_NAME("..."), ITEM_PLURAL_NAME("..."), COMPOUND_STRING_SIZE_LIMIT("...", n))
have been rewritten.  Checks:

  STRUCTURE  the file with every game-string argument blanked must be identical
  CHARMAP    every character / escape / {TOKEN} must be encodable by preproc
  TOKENS     {placeholders}/control codes of the original must all be kept
  WIDTH      pixel width of every displayed line (FONT_NORMAL) vs the limit
  LENGTH     byte length of names stored in fixed-size buffers

Charmap parsing, the preproc string parser and the font-width renderer are
imported from ../textcheck/textcheck.py (exact port of tools/preproc + src/text.c).
Run with `python3 -I ctextcheck.py --help`.
"""

import argparse
import bisect
import fnmatch
import importlib.util
import json
import os
import re
import sys
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
TEXTCHECK = os.path.join(HERE, '..', 'textcheck', 'textcheck.py')


def _load_textcheck():
    sys.dont_write_bytecode = True
    spec = importlib.util.spec_from_file_location('textcheck', TEXTCHECK)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


tc = _load_textcheck()

DEFAULT_ROOT = '/home/user/pex'
DEFAULT_ORIG = '/home/user/pex-orig'
DEFAULT_ALLOW = os.path.join(HERE, 'allow.json')

# macros whose string-literal argument is converted by tools/preproc (after cpp):
#   _( / __(  and COMPOUND_STRING(  directly (tools/preproc/c_file.cpp),
#   ITEM_NAME / ITEM_PLURAL_NAME -> COMPOUND_STRING_SIZE_LIMIT(str, n) -> both of them
GAME_MACROS = ('_', '__', 'COMPOUND_STRING', 'ITEM_NAME', 'ITEM_PLURAL_NAME', 'COMPOUND_STRING_SIZE_LIMIT')

# --------------------------------------------------------------------------
# Widths
# --------------------------------------------------------------------------
LIMIT_BATTLE = 26 * 8          # 208: battle message box (src/battle_bg.c B_WIN_MSG, 26 tiles)
LIMIT_BATTLE_ARROW = LIMIT_BATTLE - 8   # 200: line followed by \p or \l carries the down arrow
BATTLE_FILES = ('src/battle_message.c',)
PX_PER_CHAR = 6                # assumed average FONT_NORMAL glyph advance for placeholders
FONTS_REPORTED = (('normal', 1), ('small', 0), ('narrow', 7))

# assumed expansion length (characters) of placeholders, by charmap name
PH_CHARS_EXACT = {
    'PLAYER': 7, 'RIVAL': 7, 'STR_VAR_1': 10, 'STR_VAR_2': 10, 'STR_VAR_3': 10,
    'B_BUFF1': 10, 'B_BUFF2': 10, 'B_BUFF3': 10,
    'B_COPY_VAR_1': 10, 'B_COPY_VAR_2': 10, 'B_COPY_VAR_3': 10,
    'B_CURRENT_MOVE': 12, 'B_LAST_MOVE': 12, 'B_LAST_ITEM': 12,
    'B_PLAYER_NAME': 7, 'B_PC_CREATOR_NAME': 7, 'B_DEF_NAME': 10,
    'B_TRAINER1_LOSE_TEXT': 0, 'B_TRAINER1_WIN_TEXT': 0,
    'B_TRAINER2_LOSE_TEXT': 0, 'B_TRAINER2_WIN_TEXT': 0, 'B_26': 0,
}
PH_CHARS_PATTERNS = [            # first match wins
    (r'_NAME_WITH_PREFIX', 13),   # "Il/La/Lo <name>" / "<name> nemico"
    (r'_NAME_WITH_CLASS$', 20),   # trainer class + name
    (r'_MON\d_NAME$', 10),
    (r'_PARTNER_NAME$', 10),
    (r'ABILITY$', 12),
    (r'_CLASS$', 12),
    (r'_NAME$', 7),               # trainer / link player names (PLAYER_NAME_LENGTH)
    (r'_PREFIX\d$', 6),
    (r'_TEAM\d$', 10),
]
PH_CHARS_DEFAULT = 10

# --------------------------------------------------------------------------
# Length limits: (file glob, top symbol or None, field/macro, constant, extra, what)
#   bytes allowed = value(constant) + extra   (encoded bytes, terminator excluded)
# --------------------------------------------------------------------------
LENGTH_RULES = [
    # COMPOUND_STRING_SIZE_LIMIT(str, ITEM_NAME_LENGTH): sizeof(incl. EOS) <= limit
    ('src/data/items.h', None, 'macro:ITEM_NAME', 'ITEM_NAME_LENGTH', -1, 'item name'),
    ('src/data/items.h', None, 'macro:ITEM_PLURAL_NAME', 'ITEM_NAME_PLURAL_LENGTH', -1, 'item plural name'),
    ('src/data/moves_info.h', None, 'name', 'MOVE_NAME_LENGTH', 0, 'move name'),
    ('src/data/abilities.h', None, 'name', 'ABILITY_NAME_LENGTH', 0, 'ability name'),     # u8 name[ABILITY_NAME_LENGTH + 1]
    ('src/data/pokemon/species_info*', None, 'speciesName', 'POKEMON_NAME_LENGTH', 0, 'species name'),  # [POKEMON_NAME_LENGTH + 1]
    ('src/data/pokemon/species_info*', None, 'categoryName', '=12', 0, 'species category'),            # u8 categoryName[13]
    ('src/data/types_info.h', None, 'name', 'TYPE_NAME_LENGTH', 0, 'type name'),          # [TYPE_NAME_LENGTH + 1]
    ('src/data/types_info.h', None, 'generic', '=16', 0, 'type generic'),                 # u8 generic[17]
    ('src/battle_main.c', 'gTrainerClasses', '*', '=12', 0, 'trainer class name'),       # u8 name[13]
    ('src/berry.c', None, 'name', 'BERRY_NAME_LENGTH', 0, 'berry name'),                 # [BERRY_NAME_LENGTH + 1]
    ('src/data/decoration/header.h', None, 'name', '=15', 0, 'decoration name'),         # u8 name[16]
]
CONST_DEFAULTS = {'ITEM_NAME_LENGTH': 20, 'ITEM_NAME_PLURAL_LENGTH': 22, 'MOVE_NAME_LENGTH': 16,
                  'ABILITY_NAME_LENGTH': 16, 'POKEMON_NAME_LENGTH': 12, 'TYPE_NAME_LENGTH': 8,
                  'BERRY_NAME_LENGTH': 6}
CONST_FILES = ('include/constants/global.h', 'include/global.berry.h')

# bytes that make a {TOKEN} "functional" (placeholder / control code / end / line break)
FUNCTIONAL_BYTES = {0xF7, 0xF8, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF}


# ==========================================================================
# C lexer
# ==========================================================================
TOK_RE = re.compile(r'''
 (?P<ws>[ \t\r\n\f\v]+)
|(?P<lc>//[^\n]*)
|(?P<bc>/\*.*?\*/)
|(?P<str>"(?:[^"\\\n]|\\.)*")
|(?P<chr>'(?:[^'\\\n]|\\.)*')
|(?P<id>[A-Za-z_]\w*)
|(?P<num>\.?\d[\w.]*)
|(?P<p>.)
''', re.S | re.X)


class GString:
    """One game string: the literal(s) inside one macro call."""
    __slots__ = ('macro', 'lits', 'start', 'end', 'line', 'field', 'index', 'top', 'ordinal')

    def __init__(self, **kw):
        for k, v in kw.items():
            setattr(self, k, v)

    @property
    def text(self):
        return ''.join(self.lits)

    def symbol(self):
        parts = [p for p in (self.top, self.index) if p]
        s = '/'.join(parts) if parts else '?'
        if self.field:
            s += '.' + self.field
        return s


class CFileModel:
    def __init__(self, src, name):
        self.src = src
        self.name = name
        self.nl = [m.start() for m in re.finditer('\n', src)]
        self.strings = []
        self.blanked = ''
        self.anchors = []      # (blank_off, src_off) at the start of every verbatim span
        self._lex()

    def lineno(self, off):
        return bisect.bisect_right(self.nl, off - 1) + 1 if off > 0 else 1

    def _lex(self):
        src = self.src
        toks = [(m.lastgroup, m.start(), m.end()) for m in TOK_RE.finditer(src)]
        sig = [i for i, t in enumerate(toks) if t[0] not in ('ws', 'lc', 'bc')]
        out, blen, span_start = [], 0, 0
        depth = 0
        top = None
        stmt = []             # significant (kind, text) at depth 0 since last ; or }
        in_pp = False
        index_stack = []      # (depth, name)
        prev_sig = []         # last few significant token texts
        k = 0
        n = len(toks)
        skip_until = -1
        while k < n:
            kind, s, e = toks[k]
            text = src[s:e]
            if k < skip_until:
                k += 1
                continue
            if kind == 'ws':
                if '\n' in text:
                    in_pp = False
                k += 1
                continue
            if kind in ('lc', 'bc'):
                k += 1
                continue
            if kind == 'id' and text in GAME_MACROS:
                gs = self._try_macro(toks, k, text)
                if gs is not None:
                    j_after, lits, lstart, lend = gs
                    field = None
                    if len(prev_sig) >= 3 and prev_sig[-1] == '=' and prev_sig[-3] == '.':
                        field = prev_sig[-2]
                    fld = field
                    cur_top = top if depth > 0 else self._stmt_name(stmt)
                    idx = index_stack[-1][1] if index_stack else None
                    g = GString(macro=text, lits=lits, start=lstart, end=lend, line=self.lineno(lstart),
                                field=fld, index=idx, top=cur_top, ordinal=len(self.strings))
                    self.strings.append(g)
                    # emit verbatim up to lstart, then "" placeholder
                    out.append(src[span_start:lstart])
                    blen += lstart - span_start
                    out.append('""')
                    blen += 2
                    span_start = lend
                    self.anchors.append((blen, lend))
                    prev_sig = [text, '(', '""']
                    skip_until = j_after
                    k += 1
                    continue
            # bookkeeping on significant tokens
            if text == '#' and depth == 0:
                in_pp = True
                stmt = []
            elif text == '{':
                if depth == 0:
                    top = self._stmt_name(stmt)
                depth += 1
            elif text == '}':
                depth = max(0, depth - 1)
                while index_stack and index_stack[-1][0] > depth:
                    index_stack.pop()
                if depth == 0:
                    stmt = []
            elif text == ';' and depth == 0:
                stmt = []
            elif text == '=' and depth > 0 and len(prev_sig) >= 3 and prev_sig[-1] == ']' and prev_sig[-3] == '[' \
                    and re.match(r'[A-Za-z_]\w*$', prev_sig[-2]):
                while index_stack and index_stack[-1][0] >= depth:
                    index_stack.pop()
                index_stack.append((depth, prev_sig[-2]))
            if depth == 0 and not in_pp and text not in (';', '}'):
                stmt.append((kind, text))
            prev_sig.append(text)
            if len(prev_sig) > 6:
                prev_sig = prev_sig[-6:]
            k += 1
        out.append(src[span_start:])
        self.blanked = ''.join(out)
        self.anchors.insert(0, (0, 0))

    def _try_macro(self, toks, k, name):
        """If toks[k] starts MACRO("..." ...), return (index after last literal, lits, start, end)."""
        n = len(toks)
        j = k + 1
        while j < n and toks[j][0] in ('ws', 'lc', 'bc'):
            j += 1
        if j >= n or self.src[toks[j][1]:toks[j][2]] != '(':
            return None
        j += 1
        lits = []
        lstart = lend = None
        while j < n:
            kind, s, e = toks[j]
            if kind in ('ws', 'lc', 'bc'):
                j += 1
                continue
            if kind == 'str':
                if lstart is None:
                    lstart = s
                lend = e
                lits.append(self.src[s + 1:e - 1])
                j += 1
                continue
            break
        if not lits:
            return None
        # the region ends at the last literal; following ws/comments stay verbatim
        last = j - 1
        while toks[last][0] != 'str':
            last -= 1
        return last + 1, lits, lstart, lend

    @staticmethod
    def _stmt_name(stmt):
        for i, (kind, text) in enumerate(stmt):
            if kind != 'id':
                continue
            nxt = stmt[i + 1][1] if i + 1 < len(stmt) else ''
            if nxt in ('[', '='):
                return text
            if nxt == '(' and not re.match(r'^[A-Z0-9_]+$', text):
                return text
        return None

    def src_off(self, blank_off):
        i = bisect.bisect_right([a[0] for a in self.anchors], blank_off) - 1
        b, s = self.anchors[i]
        return s + (blank_off - b)


# ==========================================================================
# Checker
# ==========================================================================
class Issue:
    def __init__(self, sev, check, file, line, msg, symbol=None, text=None, extra=None):
        self.sev, self.check, self.file, self.line, self.msg = sev, check, file, line, msg
        self.symbol, self.text, self.extra = symbol, text, extra

    def fmt(self):
        s = '%s:%s: %s %s: %s' % (self.file, self.line, self.sev, self.check, self.msg)
        if self.symbol:
            s += '  [%s]' % self.symbol
        if self.text is not None:
            s += '\n    "%s"' % self.text
        return s

    def as_dict(self):
        d = {'severity': self.sev, 'check': self.check, 'file': self.file, 'line': self.line,
             'message': self.msg}
        if self.symbol:
            d['symbol'] = self.symbol
        if self.text is not None:
            d['text'] = self.text
        if self.extra:
            d.update(self.extra)
        return d


def read_consts(root, orig):
    vals = dict(CONST_DEFAULTS)
    for base in (orig, root):
        for rel in CONST_FILES:
            p = os.path.join(base, rel)
            if not os.path.exists(p):
                continue
            with open(p, encoding='utf-8', errors='replace') as f:
                txt = f.read()
            for name in list(vals):
                m = re.search(r'#define\s+%s\s+(\d+)' % name, txt)
                if m:
                    vals[name] = int(m.group(1))
                elif name == 'ITEM_NAME_PLURAL_LENGTH':
                    m = re.search(r'#define\s+ITEM_NAME_PLURAL_LENGTH\s+ITEM_NAME_LENGTH\s*\+\s*(\d+)', txt)
                    if m:
                        vals[name] = vals['ITEM_NAME_LENGTH'] + int(m.group(1))
    return vals


def ph_chars(name):
    if name in PH_CHARS_EXACT:
        return PH_CHARS_EXACT[name]
    for pat, c in PH_CHARS_PATTERNS:
        if re.search(pat, name):
            return c
    return PH_CHARS_DEFAULT


class Checker:
    def __init__(self, root, orig, allow_path=None):
        self.root, self.orig = root, orig
        self.ctx = tc.Context(root if os.path.isdir(root or '') else orig, orig)
        self.charmap = self.ctx.charmap
        self.consts = read_consts(root, orig)
        self.allow = {}
        if allow_path and os.path.exists(allow_path):
            with open(allow_path, encoding='utf-8') as f:
                self.allow = {k: v for k, v in json.load(f).items() if not k.startswith('_')}
        self.issues = []
        self.stats = Counter()
        self.width_rows = []

    # ---- helpers ---------------------------------------------------------
    def parse(self, lit):
        toks, errs, _ = tc.parse_string_line('.string "%s"' % lit, self.charmap)
        off = len('.string "')
        return toks, [(max(0, c - off), m, s) for c, m, s in errs]

    def tokens_of(self, g):
        toks, errs = [], []
        for lit in g.lits:
            t, e = self.parse(lit)
            toks += t
            errs += e
        return toks, errs

    def ph_override(self, toks):
        ov = {}
        for t in toks:
            if t.kind == 'brace' and len(t.data) == 2 and t.data[0] == 0xFD:
                name = t.src.strip('{} \t')
                if name in ('KUN', 'VERSION', 'AQUA', 'MAGMA', 'ARCHIE', 'MAXIE', 'KYOGRE', 'GROUDON', 'REGION'):
                    ov[t.data[1]] = self.ctx.ph_widths.get(t.data[1], PH_CHARS_DEFAULT * PX_PER_CHAR)
                else:
                    ov[t.data[1]] = ph_chars(name) * PX_PER_CHAR
        return ov

    def measure(self, toks, font=1):
        units = [(t.data, 1, t.src) for t in toks]
        return self.ctx.measurer.lines(units, ph_override=self.ph_override(toks), font=font)

    @staticmethod
    def brace_names(toks):
        return [t.src.strip('{}').strip() for t in toks if t.kind == 'brace']

    def functional(self, toks):
        res = []
        for t in toks:
            if t.kind == 'brace' and (not t.data or any(b in FUNCTIONAL_BYTES for b in t.data)):
                res.append(re.sub(r'\s+', ' ', t.src.strip('{}').strip()))
        return res

    def allowance(self, rel, g):
        keys = ['%s:%d' % (rel, g.line)]
        syms = [g.symbol()]
        if g.index:
            syms.append(g.index + ('.' + g.field if g.field else ''))
            syms.append(g.index)
        if g.top:
            syms.append(g.top)
        for s in syms:
            keys += ['%s:%s' % (rel, s), s]
        for k in keys:
            if k in self.allow:
                return self.allow[k], k
        for k, v in self.allow.items():
            if ('*' in k or '?' in k) and fnmatch.fnmatch(rel, k):
                return v, k
            if k == rel:
                return v, k
        return None, None

    def length_limit(self, rel, g):
        for glob_, top, fld, const, extra, what in LENGTH_RULES:
            if not fnmatch.fnmatch(rel, glob_):
                continue
            if top and g.top != top:
                continue
            if fld.startswith('macro:'):
                if g.macro != fld[6:]:
                    continue
            elif fld != '*' and g.field != fld:
                continue
            base = int(const[1:]) if const.startswith('=') else self.consts[const]
            label = const if not const.startswith('=') else 'array size %d' % (base + 1)
            if extra:
                label += '%+d' % extra
            return base + extra, what, label
        return None

    def add(self, *a, **kw):
        iss = Issue(*a, **kw)
        self.issues.append(iss)
        self.stats[iss.sev] += 1

    # ---- main entry --------------------------------------------------------
    check_all = False

    def check_pair(self, rel, new_src, orig_src, new_name=None):
        fname = new_name or rel
        nm = CFileModel(new_src, fname)
        om = CFileModel(orig_src, rel)
        self.stats['files'] += 1
        if nm.blanked != om.blanked:
            a, b = nm.blanked, om.blanked
            i = 0
            lim = min(len(a), len(b))
            # fast common prefix
            step = 1 << 16
            while i + step <= lim and a[i:i + step] == b[i:i + step]:
                i += step
            while i < lim and a[i] == b[i]:
                i += 1
            ns, os_ = nm.src_off(i), om.src_off(i)
            nl, ol = nm.lineno(ns), om.lineno(os_)
            new_line = new_src[new_src.rfind('\n', 0, ns) + 1:].split('\n', 1)[0]
            old_line = orig_src[orig_src.rfind('\n', 0, os_) + 1:].split('\n', 1)[0]
            self.add('ERROR', 'STRUCTURE', fname, nl,
                     'code outside game strings differs from original (orig line %d); first difference at col %d'
                     % (ol, ns - new_src.rfind('\n', 0, ns)),
                     extra={'orig_line': ol, 'new_text': new_line, 'orig_text': old_line})
            self.issues[-1].text = None
            self.issues[-1].msg += '\n    new : %s\n    orig: %s' % (new_line.strip()[:160], old_line.strip()[:160])
            return
        if len(nm.strings) != len(om.strings):     # cannot happen when blanked texts match
            self.add('ERROR', 'STRUCTURE', fname, 1, 'game string count differs (%d vs %d)'
                     % (len(nm.strings), len(om.strings)))
            return
        is_battle = rel in BATTLE_FILES
        for g, o in zip(nm.strings, om.strings):
            self.stats['strings'] += 1
            if g.text == o.text and not self.check_all:
                continue
            self.stats['changed'] += 1
            self.check_string(rel, fname, g, o, is_battle)

    def check_string(self, rel, fname, g, o, is_battle):
        sym = g.symbol()
        toks, errs = self.tokens_of(g)
        otoks, oerrs = self.tokens_of(o)
        # 2. CHARMAP
        for col, msg, sug in errs:
            self.add('ERROR', 'CHARMAP', fname, g.line, msg + (' -> use %s' % sug if sug else ''),
                     symbol=sym, text=g.text)
        if errs:
            return
        # 3. TOKENS
        of, nf = self.functional(otoks), self.functional(toks)
        oc, nc = Counter(of), Counter(nf)
        missing = oc - nc
        added = nc - oc
        if missing:
            self.add('ERROR', 'TOKENS', fname, g.line, 'missing {%s} (original has %s)'
                     % ('}, {'.join(sorted(missing.elements())), ' '.join('{%s}' % x for x in of)),
                     symbol=sym, text=g.text)
        if added:
            self.add('ERROR', 'TOKENS', fname, g.line, 'new placeholder/control code {%s} not in the original'
                     % '}, {'.join(sorted(added.elements())), symbol=sym, text=g.text)
        if not missing and not added and of != nf:
            self.add('WARNING', 'TOKENS', fname, g.line, 'placeholder order changed: %s -> %s'
                     % (' '.join('{%s}' % x for x in of), ' '.join('{%s}' % x for x in nf)),
                     symbol=sym, text=g.text)
        # 5. LENGTH
        ll = self.length_limit(rel, g)
        nbytes = sum(len(t.data) for t in toks)
        if ll:
            lim, what, label = ll
            obytes = sum(len(t.data) for t in otoks)
            if obytes > lim:        # e.g. Z-move names: original already longer -> no growth
                lim, label = obytes, 'original length; %s' % label
            if nbytes > lim:
                self.add('ERROR', 'LENGTH', fname, g.line, '%s is %d bytes, max %d (%s)'
                         % (what, nbytes, lim, label), symbol=sym, text=g.text)
        # 4. WIDTH
        nl = self.measure(toks)
        ol = self.measure(otoks)
        owide = max([d['width'] for d in ol] + [0])
        allow, akey = self.allowance(rel, g)
        name_px = ll[0] * PX_PER_CHAR if ll else 0   # names: room for a max-length name
        row = {'file': fname, 'line': g.line, 'symbol': sym, 'orig_max': owide, 'lines': []}
        extra_fonts = {}
        for fname_, fid in FONTS_REPORTED[1:]:
            extra_fonts[fname_] = [d['width'] for d in self.measure(toks, fid)]
        for i, d in enumerate(nl):
            if allow is not None:
                lim = max(owide, allow) if is_battle else allow
                why = 'allow.json %s' % akey
            elif is_battle:
                box = LIMIT_BATTLE_ARROW if d['term'] in ('p', 'l') else LIMIT_BATTLE
                lim = max(owide, box)
                why = 'battle box %d' % box
            elif name_px > owide:
                lim = name_px
                why = '%s: %d bytes x %dpx' % (ll[1], ll[0], PX_PER_CHAR)
            else:
                lim = owide
                why = 'original widest line'
            ent = {'width': d['width'], 'limit': lim, 'term': d['term'], 'text': d['text']}
            for fn_, ws in extra_fonts.items():
                if i < len(ws):
                    ent[fn_] = ws[i]
            row['lines'].append(ent)
            if d['width'] > lim:
                self.add('ERROR', 'WIDTH', fname, g.line, 'line %d is %dpx > %dpx (%s; orig widest %dpx)'
                         % (i + 1, d['width'], lim, why, owide), symbol=sym, text=d['text'],
                         extra={'width': d['width'], 'limit': lim})
        self.width_rows.append(row)
        # lines per paragraph (warning only)
        def per_par(lines):
            best = cur = 0
            for d in lines:
                cur += 1
                best = max(best, cur)
                if d['term'] in ('p', '$', ''):
                    cur = 0
            return best
        npar, opar = per_par(nl), per_par(ol)
        cap = max(opar, 2) if is_battle else opar
        if npar > cap:
            self.add('WARNING', 'LINES', fname, g.line, '%d lines in one box/paragraph, original has %d'
                     % (npar, opar), symbol=sym, text=g.text)


def infer_rel(path, orig):
    ap = os.path.abspath(path)
    if orig and ap.startswith(os.path.abspath(orig) + os.sep):
        return os.path.relpath(ap, orig)
    b = re.sub(r'\.part\d+$', '', os.path.basename(path))
    return b.replace('__', '/')


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('files', nargs='*', help='C files relative to --root')
    ap.add_argument('--root', default=DEFAULT_ROOT)
    ap.add_argument('--orig', default=DEFAULT_ORIG)
    ap.add_argument('--single', nargs=2, metavar=('NEW', 'ORIG'), action='append',
                    help='compare two arbitrary files (e.g. split parts); repeatable')
    ap.add_argument('--as', dest='as_rel', help='with --single: path whose rules apply (default: inferred '
                    'from ORIG, e.g. src__data__items.h.part3 -> src/data/items.h)')
    ap.add_argument('--allow', default=DEFAULT_ALLOW, help='per-string width allowances (default tools/loc/allow.json)')
    ap.add_argument('--json', action='store_true', help='machine-readable output')
    ap.add_argument('--widths', action='store_true', help='also print widths of every changed string')
    ap.add_argument('--all', action='store_true', help='check every game string, not only changed ones')
    ap.add_argument('--no-warn', action='store_true', help='hide warnings')
    args = ap.parse_args(argv)
    if not args.files and not args.single:
        ap.error('no files given')
    ck = Checker(args.root, args.orig, args.allow)
    ck.check_all = args.all
    jobs = []
    for f in args.files:
        rel = os.path.relpath(f, args.root) if os.path.isabs(f) else f
        jobs.append((rel, os.path.join(args.root, rel), os.path.join(args.orig, rel), rel))
    for new, old in (args.single or []):
        rel = args.as_rel or infer_rel(old, args.orig)
        jobs.append((rel, new, old, new))
    for rel, newp, oldp, shown in jobs:
        for p in (newp, oldp):
            if not os.path.exists(p):
                ck.add('ERROR', 'IO', shown, 0, 'file not found: %s' % p)
                break
        else:
            with open(newp, 'rb') as f:
                nb = f.read()
            with open(oldp, 'rb') as f:
                ob = f.read()
            try:
                ns = nb.decode('utf-8')
            except UnicodeDecodeError as e:
                ck.add('ERROR', 'CHARMAP', shown, nb[:e.start].count(b'\n') + 1, 'file is not valid UTF-8')
                continue
            ck.check_pair(rel, ns, ob.decode('utf-8', errors='surrogateescape'), shown)
    issues = [i for i in ck.issues if not (args.no_warn and i.sev == 'WARNING')]
    if args.json:
        json.dump({'issues': [i.as_dict() for i in issues], 'widths': ck.width_rows if args.widths else None,
                   'stats': dict(ck.stats)}, sys.stdout, ensure_ascii=False, indent=1)
        sys.stdout.write('\n')
    else:
        for i in issues:
            print(i.fmt())
        if args.widths:
            for r in ck.width_rows:
                print('%s:%d [%s] orig widest %dpx' % (r['file'], r['line'], r['symbol'], r['orig_max']))
                for e in r['lines']:
                    print('   %4dpx /%4d  (small %s, narrow %s) %s| %s' % (
                        e['width'], e['limit'], e.get('small', '-'), e.get('narrow', '-'),
                        {'n': '\\n', 'l': '\\l', 'p': '\\p'}.get(e['term'], '  '), e['text']))
        print('%d file(s), %d game strings, %d changed: %d error(s), %d warning(s)' % (
            ck.stats['files'], ck.stats['strings'], ck.stats['changed'], ck.stats['ERROR'], ck.stats['WARNING']))
    return 1 if ck.stats['ERROR'] else 0


if __name__ == '__main__':
    sys.exit(main())
