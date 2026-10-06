#!/usr/bin/env python3
"""movedb -- move / learnset / item database extractor for pokeemerald-expansion.

Builds JSON databases from a pokeemerald-expansion (1.17.x) source tree WITHOUT
modifying it:

    moves.json            every MOVE_ constant in gMovesInfo (src/data/moves_info.h)
    learnsets.json        per-species level-up / TM / tutor / egg learnsets
    items.json            every ITEM_ in gItemsInfo (src/data/items.h)
    tmhm.json             the TM/HM list (include/constants/tms_hms.h) with move data
    movedb_species.json   minimal species data (types, base stats, evolutions) used by
                          mdb.py when /home/user/work/species.json is not available

Method: every table is run through the C preprocessor with the same include
paths / defines as the Makefile's modern build (CPPFLAGS), so all config
conditionals (B_UPDATED_MOVE_DATA >= GEN_6 ? ... , P_LVL_UP_LEARNSETS,
P_FAMILY_*, ...) are resolved exactly like the real build.  The expanded C
designated initializers are then parsed and constant-folded in Python.

Generated (git-ignored) inputs are recreated in a private build directory:
  * teachable_learnsets.h -- by running the repo's own tools/learnset_helpers
    scripts inside a shadow tree (symlinks to the read-only sources);
  * mapjson outputs (map_groups.h, layouts.h, ...) -- replaced by empty stubs
    (not needed for these tables).

Usage:
    python3 -I movedb.py --root /home/user/pex-orig --out /home/user/work/moves.json

Only the Python standard library is used.  Requires a C preprocessor
(arm-none-eabi-cpp preferred, falls back to cpp / gcc -E).
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

# ---------------------------------------------------------------------------
# Tokenizer
# ---------------------------------------------------------------------------

_TOKEN_RE = re.compile(r'''
    (?P<ws>\s+)
  | (?P<str>"(?:[^"\\\n]|\\.)*")
  | (?P<chr>'(?:[^'\\\n]|\\.)*')
  | (?P<num>(?:0[xX][0-9a-fA-F]+|0[bB][01]+|(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?)[uUlLfF]*)
  | (?P<id>[A-Za-z_]\w*)
  | (?P<op>\.\.\.|<<=|>>=|->|\+\+|--|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^!~<>=?:;,.()\[\]{}\#])
  | (?P<other>.)
''', re.X | re.S)


class Tok(tuple):
    __slots__ = ()
    kind = property(lambda s: s[0])
    text = property(lambda s: s[1])


def tokenize(text, start=0, stop_at_balanced=False):
    """Tokenize text[start:].  If stop_at_balanced, stop after the first
    top-level brace group closes (used to grab one initializer)."""
    toks = []
    depth = 0
    seen_brace = False
    for m in _TOKEN_RE.finditer(text, start):
        k = m.lastgroup
        if k == 'ws':
            continue
        t = m.group(k)
        toks.append(Tok((k, t)))
        if stop_at_balanced and k == 'op':
            if t == '{':
                depth += 1
                seen_brace = True
            elif t == '}':
                depth -= 1
                if seen_brace and depth == 0:
                    break
            elif t == ';' and depth == 0 and seen_brace:
                break
    return toks


_ESC = {'n': '\n', 't': '\t', 'r': '\r', '0': '\0', '\\': '\\', '"': '"', "'": "'",
        'a': '\a', 'b': '\b', 'f': '\f', 'v': '\v'}


def c_unescape(s):
    out = []
    i = 0
    n = len(s)
    while i < n:
        c = s[i]
        if c == '\\' and i + 1 < n:
            e = s[i + 1]
            if e == 'x':
                m = re.match(r'[0-9a-fA-F]+', s[i + 2:])
                if m:
                    out.append(chr(int(m.group(0), 16)))
                    i += 2 + len(m.group(0))
                    continue
            out.append(_ESC.get(e, e))
            i += 2
        else:
            out.append(c)
            i += 1
    return ''.join(out)


def parse_int_literal(t):
    s = t.rstrip('uUlL')
    if s[:2] in ('0x', '0X'):
        return int(s, 16)
    if s[:2] in ('0b', '0B'):
        return int(s[2:], 2)
    if re.fullmatch(r'0[0-7]+', s):
        return int(s, 8)
    if re.fullmatch(r'\d+', s):
        return int(s)
    return float(s.rstrip('fF'))


# ---------------------------------------------------------------------------
# Parser: C initializers and constant expressions
# ---------------------------------------------------------------------------

TYPE_WORDS = {
    'const', 'volatile', 'struct', 'union', 'enum', 'unsigned', 'signed', 'int',
    'char', 'short', 'long', 'void', 'float', 'double', '_Bool', 'bool',
    'u8', 'u16', 'u32', 'u64', 's8', 's16', 's32', 's64', 'vu8', 'vu16', 'vu32',
    'vs8', 'vs16', 'vs32', 'bool8', 'bool16', 'bool32', 'size_t', 'uintptr_t',
    'intptr_t', 'f32', 'f64', '__attribute__',
}


class InitList(list):
    """Brace initializer: list of (designators, node).  designators is a tuple
    of ('.', name) / ('[', exprnode) entries."""


class ParseError(Exception):
    pass


class Parser:
    def __init__(self, toks, typedefs=()):
        self.t = toks
        self.i = 0
        self.typedefs = set(typedefs)

    # -- helpers --
    def peek(self, k=0):
        j = self.i + k
        return self.t[j] if j < len(self.t) else Tok(('eof', ''))

    def next(self):
        tok = self.peek()
        self.i += 1
        return tok

    def accept(self, text):
        if self.peek().text == text and self.peek().kind == 'op':
            self.i += 1
            return True
        return False

    def expect(self, text):
        tok = self.next()
        if tok.text != text:
            ctx = ' '.join(x.text for x in self.t[max(0, self.i - 12):self.i + 6])
            raise ParseError('expected %r got %r near: %s' % (text, tok.text, ctx))
        return tok

    def is_type_start(self, k=0):
        tok = self.peek(k)
        if tok.kind != 'id':
            return False
        if tok.text in TYPE_WORDS:
            return True
        if tok.text in self.typedefs:
            nxt = self.peek(k + 1).text
            return nxt in (')', '*', '[') or self.peek(k + 1).kind == 'id'
        return False

    def skip_type_name(self):
        """Consume tokens up to the matching ')' (we are just after '(')."""
        depth = 0
        out = []
        while True:
            tok = self.next()
            if tok.kind == 'eof':
                raise ParseError('eof in type name')
            if tok.text in ('(', '['):
                depth += 1
            elif tok.text in (')', ']'):
                if depth == 0 and tok.text == ')':
                    return ' '.join(out)
                depth -= 1
            out.append(tok.text)

    # -- initializers --
    def initializer(self):
        if self.peek().text == '{':
            return self.init_list()
        return self.assign_expr()

    def init_list(self):
        self.expect('{')
        items = InitList()
        while not self.accept('}'):
            desig = []
            while True:
                if self.peek().text == '.' and self.peek(1).kind == 'id':
                    self.i += 1
                    desig.append(('.', self.next().text))
                elif self.peek().text == '[':
                    self.i += 1
                    lo = self.cond_expr()
                    if self.accept('...'):
                        hi = self.cond_expr()
                        desig.append(('[', ('range', lo, hi)))
                    else:
                        desig.append(('[', lo))
                    self.expect(']')
                else:
                    break
            if desig:
                self.expect('=')
            items.append((tuple(desig), self.initializer()))
            if not self.accept(','):
                self.expect('}')
                break
        return items

    # -- expressions --
    def assign_expr(self):
        return self.cond_expr()

    def comma_expr(self):
        e = self.assign_expr()
        while self.peek().text == ',':
            self.i += 1
            e = ('comma', e, self.assign_expr())
        return e

    def cond_expr(self):
        c = self.binary(0)
        if self.accept('?'):
            a = self.comma_expr()
            self.expect(':')
            b = self.cond_expr()
            return ('tern', c, a, b)
        return c

    _PREC = [
        ('||',), ('&&',), ('|',), ('^',), ('&',), ('==', '!='),
        ('<', '>', '<=', '>='), ('<<', '>>'), ('+', '-'), ('*', '/', '%'),
    ]

    def binary(self, level):
        if level == len(self._PREC):
            return self.unary()
        ops = self._PREC[level]
        e = self.binary(level + 1)
        while self.peek().kind == 'op' and self.peek().text in ops:
            op = self.next().text
            e = ('bin', op, e, self.binary(level + 1))
        return e

    def unary(self):
        tok = self.peek()
        if tok.kind == 'op' and tok.text in ('-', '+', '!', '~', '&', '*'):
            self.i += 1
            return ('un', tok.text, self.unary())
        if tok.kind == 'id' and tok.text in ('sizeof', '_Alignof', '__alignof__'):
            self.i += 1
            if self.peek().text == '(' and self.is_type_start(1):
                self.i += 1
                return ('sizeof_type', self.skip_type_name())
            return ('sizeof', self.unary())
        if tok.text == '(' and self.is_type_start(1):
            self.i += 1
            tname = self.skip_type_name()
            if self.peek().text == '{':
                return self.postfix(('compound', tname, self.init_list()))
            return ('cast', tname, self.unary())
        return self.postfix(self.primary())

    def postfix(self, e):
        while True:
            t = self.peek().text
            if t == '[':
                self.i += 1
                idx = self.comma_expr()
                self.expect(']')
                e = ('index', e, idx)
            elif t == '(':
                self.i += 1
                args = []
                if not self.accept(')'):
                    while True:
                        if self.peek().text == '{':
                            args.append(self.init_list())
                        else:
                            args.append(self.assign_expr())
                        if self.accept(')'):
                            break
                        self.expect(',')
                e = ('call', e, args)
            elif t in ('.', '->') and self.peek(1).kind == 'id':
                self.i += 1
                e = ('member', e, self.next().text)
            else:
                return e

    def primary(self):
        tok = self.next()
        if tok.kind == 'num':
            return ('num', parse_int_literal(tok.text))
        if tok.kind == 'str':
            s = c_unescape(tok.text[1:-1])
            while self.peek().kind == 'str':
                s += c_unescape(self.next().text[1:-1])
            return ('str', s)
        if tok.kind == 'chr':
            return ('num', ord(c_unescape(tok.text[1:-1]) or '\0'))
        if tok.kind == 'id':
            return ('id', tok.text)
        if tok.text == '(':
            if self.peek().text == '{':      # GNU statement expression
                self.init_list()
                self.expect(')')
                return ('opaque', 'stmt-expr')
            e = self.comma_expr()
            self.expect(')')
            return e
        ctx = ' '.join(x.text for x in self.t[max(0, self.i - 12):self.i + 6])
        raise ParseError('unexpected token %r near: %s' % (tok.text, ctx))


# ---------------------------------------------------------------------------
# Evaluator
# ---------------------------------------------------------------------------

class Unevaluable(Exception):
    pass


class Sym(object):
    """An identifier; value is its enum value when known."""
    __slots__ = ('name', 'value')

    def __init__(self, name, value=None):
        self.name = name
        self.value = value

    def __repr__(self):
        return 'Sym(%s=%r)' % (self.name, self.value)


def _cdiv(a, b):
    if isinstance(a, float) or isinstance(b, float):
        return a / b
    q = abs(a) // abs(b)
    return q if (a >= 0) == (b >= 0) else -q


def _cmod(a, b):
    return a - b * _cdiv(a, b)


_BINOPS = {
    '+': lambda a, b: a + b, '-': lambda a, b: a - b, '*': lambda a, b: a * b,
    '/': _cdiv, '%': _cmod, '<<': lambda a, b: a << b, '>>': lambda a, b: a >> b,
    '<': lambda a, b: int(a < b), '>': lambda a, b: int(a > b),
    '<=': lambda a, b: int(a <= b), '>=': lambda a, b: int(a >= b),
    '==': lambda a, b: int(a == b), '!=': lambda a, b: int(a != b),
    '&': lambda a, b: a & b, '|': lambda a, b: a | b, '^': lambda a, b: a ^ b,
}


class Evaluator:
    def __init__(self, consts):
        self.consts = consts   # name -> int (enum constants)

    def num(self, v):
        if isinstance(v, Sym):
            if v.value is None:
                raise Unevaluable(v.name)
            return v.value
        if isinstance(v, (int, float)):
            return v
        raise Unevaluable(repr(v)[:60])

    def ev(self, node):
        if isinstance(node, InitList):
            return node
        kind = node[0]
        if kind == 'num':
            return node[1]
        if kind == 'str':
            return node[1]
        if kind == 'id':
            return Sym(node[1], self.consts.get(node[1]))
        if kind == 'tern':
            return self.ev(node[2]) if self.num(self.ev(node[1])) else self.ev(node[3])
        if kind == 'bin':
            op = node[1]
            if op == '&&':
                return int(bool(self.num(self.ev(node[2]))) and bool(self.num(self.ev(node[3]))))
            if op == '||':
                return int(bool(self.num(self.ev(node[2]))) or bool(self.num(self.ev(node[3]))))
            return _BINOPS[op](self.num(self.ev(node[2])), self.num(self.ev(node[3])))
        if kind == 'un':
            op = node[1]
            if op == '&' or op == '*':
                v = self.ev(node[2])
                return v
            v = self.num(self.ev(node[2]))
            return {'-': lambda x: -x, '+': lambda x: x, '!': lambda x: int(not x),
                    '~': lambda x: ~x}[op](v)
        if kind == 'cast':
            v = self.ev(node[2])
            if isinstance(v, (int, Sym)) and not isinstance(v, bool):
                try:
                    iv = self.num(v)
                except Unevaluable:
                    return v
                t = node[1].split()
                bits = {'u8': 8, 'u16': 16, 'u32': 32, 's8': 8, 's16': 16, 's32': 32}
                for w in t:
                    if w in bits:
                        b = bits[w]
                        iv &= (1 << b) - 1
                        if w.startswith('s') and iv >= 1 << (b - 1):
                            iv -= 1 << b
                return iv
            return v
        if kind == 'compound':
            return node[2]
        if kind == 'call':
            fn = node[1]
            if fn[0] == 'id' and fn[1] in ('COMPOUND_STRING', '_', '__') and node[2]:
                return self.ev(node[2][0])
            raise Unevaluable('call')
        if kind == 'comma':
            return self.ev(node[2])
        raise Unevaluable(kind)

    def value(self, node, default=None):
        """Evaluate to a JSON-able value: int / str / symbol name / None."""
        try:
            v = self.ev(node)
        except Unevaluable:
            return default
        if isinstance(v, Sym):
            return v.name
        if isinstance(v, InitList):
            return default
        return v

    def int_value(self, node, default=None):
        try:
            return self.num(self.ev(node))
        except Unevaluable:
            return default


def first_string(node):
    """First string literal anywhere in an expression tree (for names that are
    wrapped in size-checking macros like ITEM_NAME())."""
    if isinstance(node, InitList):
        for _, v in node:
            s = first_string(v)
            if s is not None:
                return s
        return None
    if isinstance(node, tuple):
        if node and node[0] == 'str':
            return node[1]
        for x in node[1:]:
            if isinstance(x, (tuple, list)):
                s = first_string(x)
                if s is not None:
                    return s
    if isinstance(node, list):
        for x in node:
            s = first_string(x)
            if s is not None:
                return s
    return None


def fields(initlist):
    """Map '.field' designators of an InitList to their nodes (last wins, like C)."""
    out = {}
    for desig, node in initlist:
        if desig and desig[0][0] == '.':
            if len(desig) == 1:
                out[desig[0][1]] = node
            else:
                sub = out.setdefault(desig[0][1], InitList())
                if isinstance(sub, InitList):
                    sub.append((desig[1:], node))
    return out


# ---------------------------------------------------------------------------
# Preprocessed-source helpers
# ---------------------------------------------------------------------------

def strip_linemarkers(text):
    return re.sub(r'(?m)^#.*$', '', text)


_ENUM_RE = re.compile(r'\benum\b(?:\s+__attribute__\s*\(\(.*?\)\))?\s*(\w+)?\s*(?:__attribute__\s*\(\(.*?\)\)\s*)?(?::\s*\w+\s*)?\{')


def parse_enums(text, consts=None):
    """Evaluate every enum in the preprocessed text.  Returns (consts, enums)
    where consts is name->value and enums is enumname->[(name, value)]."""
    consts = {} if consts is None else consts
    enums = {}
    ev = Evaluator(consts)
    for m in _ENUM_RE.finditer(text):
        ename = m.group(1) or '<anon@%d>' % m.start()
        toks = tokenize(text, m.end() - 1, stop_at_balanced=True)
        p = Parser(toks)
        p.expect('{')
        cur = -1
        members = []
        try:
            while not p.accept('}'):
                name = p.next().text
                if p.accept('='):
                    v = ev.int_value(p.cond_expr())
                    cur = v if v is not None else None
                else:
                    cur = None if cur is None else cur + 1
                if cur is not None:
                    consts[name] = cur
                members.append((name, cur))
                if not p.accept(','):
                    p.expect('}')
                    break
        except ParseError:
            pass
        if members:
            enums.setdefault(ename, []).extend(members)
    return consts, enums


def find_object_init(text, name):
    """Find '<decl> name[...] = {' and return the parsed initializer."""
    m = re.search(r'\b%s\s*(\[[^\]=;]*\])*\s*=\s*\{' % re.escape(name), text)
    if not m:
        return None
    toks = tokenize(text, m.end() - 1, stop_at_balanced=True)
    return Parser(toks).init_list()


_ARRAY_DEF_RE = re.compile(
    r'\bstatic\s+const\s+(?:struct\s+(\w+)|(u16|enum\s+\w+))\s+(s\w+?)\s*\[\s*\]\s*=\s*\{')


def find_static_arrays(text, suffix_filter):
    """Yield (name, InitList) for 'static const <type> sXxx[] = {...}' arrays
    whose name ends with one of suffix_filter."""
    for m in _ARRAY_DEF_RE.finditer(text):
        name = m.group(3)
        if not name.endswith(suffix_filter):
            continue
        toks = tokenize(text, m.end() - 1, stop_at_balanced=True)
        yield name, Parser(toks).init_list()


# ---------------------------------------------------------------------------
# Build environment: cpp, stubs, generated headers
# ---------------------------------------------------------------------------

def find_cpp():
    for cand in (['arm-none-eabi-cpp'], ['cpp'], ['gcc', '-E'], ['clang', '-E']):
        if shutil.which(cand[0]):
            return cand
    sys.exit('error: no C preprocessor found (need arm-none-eabi-cpp, cpp or gcc)')


class Builder:
    def __init__(self, root, build_dir, game_version='EMERALD', cpp=None, verbose=True):
        self.root = os.path.abspath(root)
        self.build = os.path.abspath(build_dir)
        self.stub = os.path.join(self.build, 'stub')
        self.game = game_version
        self.cpp = cpp or find_cpp()
        self.verbose = verbose
        os.makedirs(self.stub, exist_ok=True)

    def log(self, *a):
        if self.verbose:
            print('[movedb]', *a, file=sys.stderr)

    def preprocess(self, wrapper_name, source):
        """Write `source` as <build>/<wrapper_name>.c and preprocess it with the
        Makefile's CPPFLAGS.  Missing (generated) headers become empty stubs."""
        cfile = os.path.join(self.build, wrapper_name + '.c')
        ifile = os.path.join(self.build, wrapper_name + '.i')
        with open(cfile, 'w') as f:
            f.write(source)
        cmd = self.cpp + [
            '-iquote', os.path.join(self.root, 'include'),
            '-iquote', os.path.join(self.root, 'src'),
            '-iquote', self.stub,
            '-Wno-trigraphs', '-DMODERN=1', '-DTESTING=0', '-D' + self.game,
            '-std=gnu17', cfile, '-o', ifile,
        ]
        stubbed = []
        for _ in range(64):
            r = subprocess.run(cmd, cwd=self.root, capture_output=True, text=True)
            if r.returncode == 0:
                break
            m = re.search(r'fatal error: ([^:\s]+): No such file or directory', r.stderr)
            if not m:
                sys.exit('error: preprocessing %s failed:\n%s' % (wrapper_name, r.stderr[-4000:]))
            missing = m.group(1)
            path = os.path.join(self.stub, missing)
            if os.path.exists(path):
                sys.exit('error: cannot satisfy include %s\n%s' % (missing, r.stderr[-2000:]))
            os.makedirs(os.path.dirname(path), exist_ok=True)
            with open(path, 'w') as f:
                f.write('// empty stub for generated header (movedb)\n')
            stubbed.append(missing)
        else:
            sys.exit('error: too many missing headers for %s' % wrapper_name)
        if stubbed:
            self.log('stubbed generated headers:', ', '.join(stubbed))
        with open(ifile, encoding='utf-8', errors='replace') as f:
            return strip_linemarkers(f.read())

    # -- teachable learnsets (generated by tools/learnset_helpers) --
    def generate_teachables(self):
        """Re-run the repo's learnset helper scripts in a shadow tree.
        Returns (path_to_teachable_learnsets_h, tutor_moves_list or None)."""
        root = self.root
        cfg = read(os.path.join(root, 'include/config/pokemon.h'))
        m = re.search(r'^#define P_LEARNSET_HELPER_TEACHABLE\s+(\S+)', cfg, re.M)
        enabled = m is not None and m.group(1) in ('TRUE', '1')
        existing = os.path.join(root, 'src/data/pokemon/teachable_learnsets.h')
        if not enabled:
            if not os.path.exists(existing):
                sys.exit('error: P_LEARNSET_HELPER_TEACHABLE is off and %s is missing' % existing)
            self.log('P_LEARNSET_HELPER_TEACHABLE off: using committed teachable_learnsets.h')
            return existing, None

        shadow = os.path.join(self.build, 'shadow')
        if os.path.exists(shadow):
            shutil.rmtree(shadow)
        os.makedirs(os.path.join(shadow, 'src/data/pokemon'))
        os.makedirs(os.path.join(shadow, 'tools/learnset_helpers/build'))
        for d in ('include', 'data'):
            os.symlink(os.path.join(root, d), os.path.join(shadow, d))
        pk = os.path.join(root, 'src/data/pokemon')
        for n in ('species_info', 'species_info.h', 'special_movesets.json'):
            os.symlink(os.path.join(pk, n), os.path.join(shadow, 'src/data/pokemon', n))
        helpers = os.path.join(root, 'tools/learnset_helpers')
        for n in os.listdir(helpers):
            if n.endswith('.py'):
                shutil.copy(os.path.join(helpers, n), os.path.join(shadow, 'tools/learnset_helpers', n))
        learnables = os.path.join(pk, 'all_learnables.json')
        if os.path.exists(learnables):
            os.symlink(learnables, os.path.join(shadow, 'src/data/pokemon/all_learnables.json'))
        else:
            os.symlink(os.path.join(helpers, 'porymoves_files'),
                       os.path.join(shadow, 'tools/learnset_helpers/porymoves_files'))
            self.run_py(shadow, 'make_learnables.py', 'tools/learnset_helpers/porymoves_files',
                        'src/data/pokemon/all_learnables.json')
        b = 'tools/learnset_helpers/build'
        self.run_py(shadow, 'make_tutors.py', b + '/all_tutors.json')
        self.run_py(shadow, 'make_teaching_types.py', b + '/all_teaching_types.json')
        self.run_py(shadow, 'make_teachables.py', b)
        out = os.path.join(shadow, 'src/data/pokemon/teachable_learnsets.h')
        if not os.path.exists(out):
            sys.exit('error: make_teachables.py did not produce teachable_learnsets.h')
        dest_dir = os.path.join(self.build, 'data/pokemon')
        os.makedirs(dest_dir, exist_ok=True)
        dest = os.path.join(dest_dir, 'teachable_learnsets.h')
        shutil.copy(out, dest)
        tutors = json.loads(read(os.path.join(shadow, b, 'all_tutors.json')))
        special = json.loads(read(os.path.join(pk, 'special_movesets.json')))
        tutors = sorted(set(tutors) | set(special.get('extraTutors', [])))
        self.log('generated teachable_learnsets.h (%d tutor moves)' % len(tutors))
        return dest, tutors

    def run_py(self, cwd, script, *args):
        cmd = [sys.executable, '-I', 'tools/learnset_helpers/' + script] + list(args)
        r = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
        if r.returncode != 0:
            sys.exit('error: %s failed:\n%s%s' % (script, r.stdout, r.stderr))


def read(path):
    with open(path, encoding='utf-8') as f:
        return f.read()


# ---------------------------------------------------------------------------
# Extraction
# ---------------------------------------------------------------------------

MOVE_FLAG_FIELDS = [
    'makesContact', 'ignoresProtect', 'magicCoatAffected', 'snatchAffected',
    'ignoresKingsRock', 'punchingMove', 'bitingMove', 'pulseMove', 'soundMove',
    'ballisticMove', 'powderMove', 'danceMove', 'windMove', 'slicingMove',
    'healingMove', 'minimizeDoubleDamage', 'ignoresTargetAbility',
    'ignoresTargetDefenseEvasionStages', 'damagesUnderground', 'damagesUnderwater',
    'damagesAirborne', 'damagesAirborneDoubleDamage', 'ignoreTypeIfFlyingAndUngrounded',
    'thawsUser', 'ignoresSubstitute', 'forcePressure', 'cantUseTwice',
    'alwaysHitsInRain', 'accuracy50InSun', 'alwaysHitsInHailSnow',
    'alwaysHitsOnSameType', 'noAffectOnSameTypeTarget', 'accDecreaseIfUserNotSameType',
]
MOVE_BAN_FIELDS = [
    'gravityBanned', 'mirrorMoveBanned', 'meFirstBanned', 'mimicBanned',
    'metronomeBanned', 'copycatBanned', 'assistBanned', 'sleepTalkBanned',
    'instructBanned', 'encoreBanned', 'parentalBondBanned', 'skyBattleBanned',
    'sketchBanned', 'dampBanned',
]
STAT_FIELDS = ('attack', 'defense', 'spAtk', 'spDef', 'speed', 'accuracy', 'evasion')
SELF_KO_EFFECTS = {'EFFECT_EXPLOSION', 'EFFECT_MISTY_EXPLOSION', 'EFFECT_FINAL_GAMBIT',
                   'EFFECT_MEMENTO', 'EFFECT_HEALING_WISH', 'EFFECT_LUNAR_DANCE'}


def to_json_value(ev, node):
    """Generic conversion of an initializer node to JSON (dicts for designated
    lists, lists for positional ones)."""
    if isinstance(node, InitList):
        if node and all(d and all(x[0] == '.' for x in d) for d, _ in node):
            out = {}
            for d, v in node:
                cur = out
                for _, name in d[:-1]:
                    nxt = cur.get(name)
                    if not isinstance(nxt, dict):
                        nxt = cur[name] = {}
                    cur = nxt
                cur[d[-1][1]] = to_json_value(ev, v)
            return out
        return [to_json_value(ev, v) for _, v in node]
    if isinstance(node, tuple) and node[0] == 'compound':
        return to_json_value(ev, node[2])
    return ev.value(node)


def extract_moves(text, consts):
    ev = Evaluator(consts)
    init = find_object_init(text, 'gMovesInfo')
    if init is None:
        sys.exit('error: gMovesInfo not found')
    moves = {}
    for desig, body in init:
        if not desig or desig[0][0] != '[' or desig[0][1][0] != 'id':
            continue
        const = desig[0][1][1]
        f = fields(body)

        def iv(name, default=0):
            return ev.int_value(f[name], default) if name in f else default

        def sv(name, default=None):
            return ev.value(f[name], default) if name in f else default

        name = first_string(f['name']) if 'name' in f else None
        desc = first_string(f['description']) if 'description' in f else None
        effect = sv('effect', 'EFFECT_HIT')
        add = []
        if 'additionalEffects' in f:
            node = f['additionalEffects']
            lst = node[2] if isinstance(node, tuple) and node[0] == 'compound' else node
            if isinstance(lst, InitList):
                for _, e in lst:
                    if isinstance(e, InitList):
                        d = to_json_value(ev, e)
                        if isinstance(d, dict):
                            d = {k: v for k, v in d.items() if v not in (0, None) or k == 'chance'}
                            d.setdefault('chance', 0)
                            add.append(d)
        argument = to_json_value(ev, f['argument']) if 'argument' in f else None
        if isinstance(argument, dict):
            argument = {k: (to_json_value(ev, v) if isinstance(v, InitList) else v)
                        for k, v in argument.items()}
        flags = [k for k in MOVE_FLAG_FIELDS if iv(k)]
        bans = [k for k in MOVE_BAN_FIELDS if iv(k)]
        strike = iv('strikeCount', 0)
        multi = bool(iv('multiHit'))
        explosion = bool(iv('explosion'))
        recoil = 0
        if isinstance(argument, dict) and effect in ('EFFECT_RECOIL', 'EFFECT_RECOIL_IF_MISS',
                                                      'EFFECT_FLARE_BLITZ', 'EFFECT_LIGHT_OF_RUIN'):
            recoil = argument.get('recoilPercentage') or 0
        if not recoil and isinstance(argument, dict) and 'recoilPercentage' in argument \
                and effect.startswith('EFFECT_RECOIL'):
            recoil = argument['recoilPercentage'] or 0
        recharge = any(e.get('moveEffect') == 'MOVE_EFFECT_RECHARGE' for e in add)
        self_ko = explosion or effect in SELF_KO_EFFECTS
        secondary = []
        for e in add:
            me = e.get('moveEffect')
            if not me:
                continue
            s = me.replace('MOVE_EFFECT_', '')
            extra = []
            if e.get('self'):
                extra.append('self')
            stats = [(k, e[k]) for k in STAT_FIELDS if e.get(k)]
            if stats:
                sign = '-' if 'MINUS' in me else '+'
                extra.append(' '.join('%s%s%d' % (k, sign, v) for k, v in stats))
            arg = e.get('argument')
            if isinstance(arg, dict) and arg.get('absorbPercentage'):
                extra.append('%d%% drain' % arg['absorbPercentage'])
            if extra:
                s += '(%s)' % '; '.join(extra)
            ch = e.get('chance', 0)
            secondary.append('%s %d%%' % (s, ch) if ch else s)
        prio = iv('priority')
        moves[const] = {
            'id': consts.get(const),
            'name': name,
            'type': sv('type'),
            'power': iv('power'),
            'accuracy': iv('accuracy'),
            'pp': iv('pp'),
            'priority': prio,
            'category': sv('category'),
            'effect': effect,
            'target': sv('target'),
            'strikeCount': strike if strike else 1,
            'multiHit': multi,
            'criticalHitStage': iv('criticalHitStage'),
            'alwaysCriticalHit': bool(iv('alwaysCriticalHit')),
            'recoil': recoil,
            'recharge': recharge,
            'selfKO': self_ko,
            'explosion': explosion,
            'flags': flags,
            'bans': bans,
            'argument': argument,
            'additionalEffects': add,
            'secondary': ', '.join(secondary),
            'description': desc,
        }
    return moves


def extract_items(text, consts, enums):
    ev = Evaluator(consts)
    init = find_object_init(text, 'gItemsInfo')
    if init is None:
        sys.exit('error: gItemsInfo not found')
    by_value = {}
    for name, v in enums.get('Item', []) or []:
        if v is not None:
            by_value.setdefault(v, []).append(name)
    items = {}
    for desig, body in init:
        if not desig or desig[0][0] != '[' or desig[0][1][0] != 'id':
            continue
        const = desig[0][1][1]
        f = fields(body)

        def iv(name, default=0):
            return ev.int_value(f[name], default) if name in f else default

        def sv(name, default=None):
            return ev.value(f[name], default) if name in f else default

        name = first_string(f['name']) if 'name' in f else None
        if 'name' in f and f['name'][0] == 'id':          # e.g. gQuestionMarksItemName
            name = '????????'
        iid = consts.get(const)
        hold = sv('holdEffect', 0)
        items[const] = {
            'id': iid,
            'name': name,
            'pluralName': first_string(f['pluralName']) if 'pluralName' in f else None,
            'price': iv('price'),
            'pocket': sv('pocket'),
            'holdEffect': hold if hold else 'HOLD_EFFECT_NONE',
            'holdEffectParam': iv('holdEffectParam'),
            'secondaryId': sv('secondaryId', 0),
            'importance': iv('importance'),
            'type': sv('type'),
            'battleUsage': sv('battleUsage', 0),
            'flingPower': iv('flingPower'),
            'sortType': sv('sortType'),
            'aliases': [a for a in by_value.get(iid, []) if a != const],
            'description': first_string(f['description']) if 'description' in f else None,
        }
    return items


def parse_tmhm(root):
    text = read(os.path.join(root, 'include/constants/tms_hms.h'))
    text = re.sub(r'//[^\n]*', '', text)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    out = {}
    for kind in ('TM', 'HM'):
        m = re.search(r'#define\s+FOREACH_%s\(F\)((?:[^\n]*\\\n)*[^\n]*)' % kind, text)
        out[kind] = re.findall(r'F\((\w+)\)', m.group(1)) if m else []
    return out


def extract_species(text, consts, ev):
    init = find_object_init(text, 'gSpeciesInfo')
    if init is None:
        sys.exit('error: gSpeciesInfo not found')
    species = {}
    for desig, body in init:
        if not desig or desig[0][0] != '[' or desig[0][1][0] != 'id':
            continue
        const = desig[0][1][1]
        if not isinstance(body, InitList):
            continue
        f = fields(body)

        def iv(name, default=0):
            return ev.int_value(f[name], default) if name in f else default

        def ref(name):
            if name not in f:
                return None
            v = ev.value(f[name])
            return v if isinstance(v, str) else None

        types = []
        if 'types' in f and isinstance(f['types'], InitList):
            types = [ev.value(v) for _, v in f['types']]
        abilities = []
        if 'abilities' in f and isinstance(f['abilities'], InitList):
            abilities = [ev.value(v) for _, v in f['abilities']]
        evos = []
        if 'evolutions' in f:
            node = f['evolutions']
            lst = node[2] if isinstance(node, tuple) and node[0] == 'compound' else node
            if isinstance(lst, InitList):
                for _, e in lst:
                    if not isinstance(e, InitList) or len(e) < 3:
                        continue
                    method = ev.value(e[0][1])
                    if method in (None, 0xFFFF, 'EVOLUTIONS_END'):
                        continue
                    evos.append({'method': method, 'param': ev.value(e[1][1]),
                                 'target': ev.value(e[2][1])})
        species[const] = {
            'id': consts.get(const),
            'name': first_string(f['speciesName']) if 'speciesName' in f else None,
            'types': types,
            'abilities': abilities,
            'baseHP': iv('baseHP'), 'baseAttack': iv('baseAttack'),
            'baseDefense': iv('baseDefense'), 'baseSpeed': iv('baseSpeed'),
            'baseSpAttack': iv('baseSpAttack'), 'baseSpDefense': iv('baseSpDefense'),
            'levelUpLearnset': ref('levelUpLearnset'),
            'teachableLearnset': ref('teachableLearnset'),
            'eggMoveLearnset': ref('eggMoveLearnset'),
            'formSpeciesIdTable': ref('formSpeciesIdTable'),
            'evolutions': evos,
            'isMegaEvolution': bool(iv('isMegaEvolution')),
            'isGigantamax': bool(iv('isGigantamax')),
            'isTotem': bool(iv('isTotem')),
            'isPrimalReversion': bool(iv('isPrimalReversion')),
            'isUltraBurst': bool(iv('isUltraBurst')),
            'isTeraForm': bool(iv('isTeraForm')),
            'cannotBeTraded': bool(iv('cannotBeTraded')),
        }
    return species


def build_all(args):
    root = os.path.abspath(args.root)
    out_moves = os.path.abspath(args.out)
    out_dir = os.path.dirname(out_moves)
    os.makedirs(out_dir, exist_ok=True)
    build_dir = args.build_dir or os.path.join(out_dir, '.movedb_build')
    if os.path.exists(os.path.join(build_dir, 'data')):
        shutil.rmtree(os.path.join(build_dir, 'data'))
    b = Builder(root, build_dir, args.game_version,
                cpp=args.cpp.split() if args.cpp else None, verbose=not args.quiet)
    b.log('root=%s cpp=%s build=%s' % (root, ' '.join(b.cpp), build_dir))

    # ---- config values used (for the report) ----
    cfg = {}
    for f_, keys in (('include/config/pokemon.h', ('P_LVL_UP_LEARNSETS', 'P_TM_LITERACY',
                                                  'P_LEARNSET_HELPER_TEACHABLE')),
                     ('include/config/battle.h', ('B_UPDATED_MOVE_DATA', 'B_UPDATED_MOVE_TYPES',
                                                  'B_UPDATED_MOVE_FLAGS'))):
        t = read(os.path.join(root, f_))
        for k in keys:
            m = re.search(r'^#define\s+%s\s+(\S+)' % k, t, re.M)
            cfg[k] = m.group(1) if m else None

    # ---- moves ----
    mtext = b.preprocess('moves_wrap', '#include "global.h"\n#include "battle.h"\n'
                         '#include "main.h"\n#include "move.h"\n#include "data/moves_info.h"\n')
    consts, enums = parse_enums(mtext)
    moves = extract_moves(mtext, consts)
    b.log('moves: %d' % len(moves))

    # ---- items ----
    itext = b.preprocess('items_wrap', '#include "global.h"\n#include "item.h"\n'
                         '#include "constants/battle.h"\n#include "constants/items.h"\n'
                         '#include "constants/moves.h"\n#include "constants/item_effects.h"\n'
                         '#include "constants/hold_effects.h"\n#include "data/items.h"\n')
    iconsts, ienums = parse_enums(itext)
    items = extract_items(itext, iconsts, ienums)
    b.log('items: %d' % len(items))

    # ---- learnsets / species ----
    teach_path, tutors = b.generate_teachables()
    pokemon_c = read(os.path.join(root, 'src/pokemon.c'))
    m = re.search(r'^#if P_LVL_UP_LEARNSETS.*?^#endif[^\n]*$', pokemon_c, re.S | re.M)
    lvl_chain = m.group(0) if m else '#include "data/pokemon/level_up_learnsets/gen_9.h"'
    if teach_path.startswith(b.build):
        teach_inc = '#include "data/pokemon/teachable_learnsets.h"'   # resolves to build dir first
    else:
        teach_inc = '#include "%s"' % teach_path
    src = '\n'.join([
        '#include "global.h"', '#include "pokemon.h"', '#include "constants/abilities.h"',
        '#include "constants/items.h"', '#include "constants/moves.h"',
        '#include "constants/species.h"', '#include "constants/form_change_types.h"',
        lvl_chain, teach_inc, '#include "data/pokemon/egg_moves.h"',
        '#include "data/pokemon/form_species_tables.h"',
        '#include "data/pokemon/form_change_tables.h"',
        '#include "data/pokemon/species_info.h"', ''])
    ptext = b.preprocess('species_wrap', src)
    pconsts, penums = parse_enums(ptext)
    pev = Evaluator(pconsts)
    species = extract_species(ptext, pconsts, pev)
    # which gen file did the preprocessor pick?  (from the .i line markers)
    mm = re.search(r'level_up_learnsets/(gen_\d+)\.h', read(os.path.join(b.build, 'species_wrap.i')))
    used_lvl = mm.group(1) if mm else None

    levelup_arrays = {}
    for name, lst in find_static_arrays(ptext, ('LevelUpLearnset',)):
        out = []
        for _, e in lst:
            if not isinstance(e, InitList):
                continue
            ef = fields(e)
            mv = pev.value(ef.get('move', ('num', 0)))
            lv = pev.int_value(ef.get('level', ('num', 0)), 0)
            if mv in (0xFFFF, 'LEVEL_UP_MOVE_END') or mv is None:
                break
            out.append([lv, mv])
        levelup_arrays[name] = out
    u16_arrays = {}
    for name, lst in find_static_arrays(ptext, ('TeachableLearnset', 'EggMoveLearnset',
                                                'FormSpeciesIdTable')):
        out = []
        for _, e in lst:
            v = pev.value(e)
            if v in (0xFFFF, 'MOVE_UNAVAILABLE', 'FORM_SPECIES_END') or v is None:
                break
            out.append(v)
        u16_arrays[name] = out
    b.log('arrays: %d level-up, %d teachable, %d egg' % (
        len(levelup_arrays), sum(1 for k in u16_arrays if k.endswith('TeachableLearnset')),
        sum(1 for k in u16_arrays if k.endswith('EggMoveLearnset'))))

    # ---- TM / HM ----
    tmhm = parse_tmhm(root)
    tm_moves = ['MOVE_' + x for x in tmhm['TM'] + tmhm['HM']]
    tm_set = set(tm_moves)
    tmhm_list = []
    for kind in ('TM', 'HM'):
        for i, x in enumerate(tmhm[kind], 1):
            mv = 'MOVE_' + x
            item = 'ITEM_%s_%s' % (kind, x)
            md = moves.get(mv, {})
            tmhm_list.append({
                'num': '%s%02d' % (kind, i), 'item': item,
                'item_id': iconsts.get(item), 'move': mv, 'name': md.get('name'),
                'type': md.get('type'), 'power': md.get('power'),
                'accuracy': md.get('accuracy'), 'category': md.get('category'),
            })
    if tutors is None:
        tutors = []
    for t in tmhm_list:
        if t['move'] in moves:
            moves[t['move']]['tmhm'] = t['num']

    # ---- pre-evolutions (for inherited egg moves) ----
    prevo = {}
    for sp, d in species.items():
        for e in d['evolutions']:
            t = e['target']
            if isinstance(t, str) and t in species and t != sp:
                prevo.setdefault(t, sp)
    base_form = {}
    for sp, d in species.items():
        tbl = u16_arrays.get(d['formSpeciesIdTable'] or '', [])
        if tbl and tbl[0] != sp:
            base_form[sp] = tbl[0]

    def family_root_egg(sp):
        seen = set()
        cur = sp
        while cur and cur not in seen:
            seen.add(cur)
            eg = u16_arrays.get(species[cur]['eggMoveLearnset'] or '', [])
            if eg and cur != sp:
                return cur, eg
            nxt = prevo.get(cur) or base_form.get(cur)
            cur = nxt if nxt in species else None
        return None, []

    learnsets = {}
    missing_arrays = set()
    for sp, d in species.items():
        lu_name = d['levelUpLearnset']
        if lu_name and lu_name not in levelup_arrays:
            missing_arrays.add(lu_name)
        lu = levelup_arrays.get(lu_name or '', [])
        teach = u16_arrays.get(d['teachableLearnset'] or '', [])
        egg = u16_arrays.get(d['eggMoveLearnset'] or '', [])
        entry = {
            'levelup': lu,
            'tm': [m_ for m_ in teach if m_ in tm_set],
            'tutor': [m_ for m_ in teach if m_ not in tm_set],
            'egg': egg,
        }
        if not egg:
            src_sp, inh = family_root_egg(sp)
            if inh:
                entry['egg_inherited'] = inh
                entry['egg_source'] = src_sp
        if prevo.get(sp):
            entry['prevo'] = prevo[sp]
        if base_form.get(sp):
            entry['base_form'] = base_form[sp]
        entry['arrays'] = {'levelup': lu_name, 'teachable': d['teachableLearnset'],
                           'egg': d['eggMoveLearnset']}
        learnsets[sp] = entry
    if missing_arrays:
        b.log('warning: %d referenced level-up arrays not found' % len(missing_arrays))

    # ---- write ----
    def dump(path, obj):
        with open(path, 'w', encoding='utf-8') as fp:
            json.dump(obj, fp, indent=1, ensure_ascii=False)
            fp.write('\n')
        b.log('wrote %s' % path)

    meta = {'root': root, 'config': cfg, 'levelup_file': used_lvl,
            'cpp': ' '.join(b.cpp), 'game_version': args.game_version}
    dump(out_moves, moves)
    learn_path = args.learnsets or os.path.join(out_dir, 'learnsets.json')
    dump(learn_path, learnsets)
    dump(args.items or os.path.join(out_dir, 'items.json'), items)
    dump(os.path.join(out_dir, 'tmhm.json'), {'tm_hm': tmhm_list, 'tutors': tutors, 'meta': meta})
    sp_out = {}
    for sp, d in species.items():
        sp_out[sp] = {k: d[k] for k in ('id', 'name', 'types', 'abilities', 'baseHP', 'baseAttack',
                                        'baseDefense', 'baseSpeed', 'baseSpAttack',
                                        'baseSpDefense', 'evolutions', 'isMegaEvolution',
                                        'isGigantamax', 'isTotem', 'isPrimalReversion',
                                        'isUltraBurst', 'isTeraForm')}
        if prevo.get(sp):
            sp_out[sp]['prevo'] = prevo[sp]
        if base_form.get(sp):
            sp_out[sp]['baseForm'] = base_form[sp]
    dump(os.path.join(out_dir, 'movedb_species.json'), sp_out)

    # ---- report ----
    n_lu = sum(1 for v in learnsets.values() if v['levelup']
               and v['arrays']['levelup'] != 'sNoneLevelUpLearnset')
    n_any = sum(1 for v in learnsets.values() if v['levelup'] or v['tm'] or v['tutor'] or v['egg'])
    print('moves: %d (MOVE_ constants in gMovesInfo)' % len(moves))
    print('items: %d' % len(items))
    print('TM/HM: %d TMs, %d HMs; tutor moves: %d' % (len(tmhm['TM']), len(tmhm['HM']), len(tutors)))
    print('species entries: %d; with own level-up learnset: %d; with any learnset: %d'
          % (len(learnsets), n_lu, n_any))
    print('level-up learnset file: %s (P_LVL_UP_LEARNSETS=%s)' % (used_lvl, cfg.get('P_LVL_UP_LEARNSETS')))
    ok = validate(moves, learnsets, items)
    if not args.keep_build:
        for n in ('shadow',):
            shutil.rmtree(os.path.join(build_dir, n), ignore_errors=True)
    return 0 if ok else 1


# ---------------------------------------------------------------------------
# Validation against known (Gen 9 / GEN_LATEST) values
# ---------------------------------------------------------------------------

KNOWN_MOVES = {
    'MOVE_TACKLE': dict(power=40, accuracy=100, pp=35, type='TYPE_NORMAL', category='DAMAGE_CATEGORY_PHYSICAL', flags_has='makesContact'),
    'MOVE_FLAMETHROWER': dict(power=90, accuracy=100, pp=15, type='TYPE_FIRE', category='DAMAGE_CATEGORY_SPECIAL', secondary_has='BURN 10%'),
    'MOVE_THUNDERBOLT': dict(power=90, accuracy=100, pp=15, type='TYPE_ELECTRIC', category='DAMAGE_CATEGORY_SPECIAL', secondary_has='PARALYSIS 10%'),
    'MOVE_EARTHQUAKE': dict(power=100, accuracy=100, pp=10, type='TYPE_GROUND', category='DAMAGE_CATEGORY_PHYSICAL'),
    'MOVE_FAKE_OUT': dict(power=40, accuracy=100, priority=3, type='TYPE_NORMAL', secondary_has='FLINCH'),
    'MOVE_WILL_O_WISP': dict(power=0, accuracy=85, type='TYPE_FIRE', category='DAMAGE_CATEGORY_STATUS'),
    'MOVE_DRAGON_DARTS': dict(power=50, accuracy=100, strikeCount=2, type='TYPE_DRAGON', category='DAMAGE_CATEGORY_PHYSICAL'),
    'MOVE_BODY_PRESS': dict(power=80, accuracy=100, type='TYPE_FIGHTING', effect='EFFECT_BODY_PRESS', category='DAMAGE_CATEGORY_PHYSICAL'),
    'MOVE_TERA_BLAST': dict(power=80, accuracy=100, type='TYPE_NORMAL', effect='EFFECT_TERA_BLAST', category='DAMAGE_CATEGORY_SPECIAL'),
    'MOVE_POPULATION_BOMB': dict(power=20, accuracy=90, strikeCount=10, type='TYPE_NORMAL', flags_has='slicingMove'),
    'MOVE_HYPER_BEAM': dict(power=150, accuracy=90, recharge=True),
    'MOVE_EXPLOSION': dict(power=250, selfKO=True),
    'MOVE_DOUBLE_EDGE': dict(power=120, recoil=33),
    'MOVE_BULLET_SEED': dict(power=25, multiHit=True, type='TYPE_GRASS'),
    'MOVE_SUCKER_PUNCH': dict(power=70, priority=1),
    'MOVE_PROTECT': dict(priority=4, category='DAMAGE_CATEGORY_STATUS'),
    'MOVE_HURRICANE': dict(power=110, accuracy=70, type='TYPE_FLYING'),
    'MOVE_FIRE_BLAST': dict(power=110, accuracy=85),
    'MOVE_MAKE_IT_RAIN': dict(power=120, accuracy=100, type='TYPE_STEEL', category='DAMAGE_CATEGORY_SPECIAL'),
    'MOVE_SHADOW_BALL': dict(power=80, type='TYPE_GHOST', category='DAMAGE_CATEGORY_SPECIAL'),
    'MOVE_THUNDER_PUNCH': dict(power=75, flags_has='punchingMove'),
    'MOVE_CRUNCH': dict(power=80, flags_has='bitingMove'),
    'MOVE_HYPER_VOICE': dict(power=90, flags_has='soundMove'),
    'MOVE_KARATE_CHOP': dict(type='TYPE_FIGHTING', criticalHitStage=1),
    'MOVE_SWORDS_DANCE': dict(category='DAMAGE_CATEGORY_STATUS', pp=20),
    'MOVE_SPLASH': dict(power=0, category='DAMAGE_CATEGORY_STATUS'),
    'MOVE_GIGA_DRAIN': dict(power=75, pp=10, secondary_has='50% drain'),
    'MOVE_CLOSE_COMBAT': dict(power=120, accuracy=100, secondary_has='defense-1 spDef-1'),
    'MOVE_BITE': dict(type='TYPE_DARK', power=60),
    'MOVE_CHARM': dict(type='TYPE_FAIRY'),
}


def validate(moves, learnsets, items):
    ok = True
    fails = []
    checked = 0
    for mv, exp in KNOWN_MOVES.items():
        d = moves.get(mv)
        if d is None:
            fails.append('%s missing' % mv)
            continue
        for k, v in exp.items():
            checked += 1
            if k == 'flags_has':
                good = v in d['flags']
            elif k == 'secondary_has':
                good = v in d['secondary']
            else:
                good = d.get(k) == v
            if not good:
                fails.append('%s.%s = %r (expected %r)' % (mv, k.replace('_has', ''),
                                                           d.get(k.replace('flags_has', 'flags').replace('secondary_has', 'secondary')), v))
    # learnset sanity
    def has_lu(sp, mv, lvl=None):
        return any(m == mv and (lvl is None or l == lvl) for l, m in learnsets.get(sp, {}).get('levelup', []))
    lchecks = [
        ('SPECIES_BULBASAUR levelup Vine Whip @3', has_lu('SPECIES_BULBASAUR', 'MOVE_VINE_WHIP', 3)),
        ('SPECIES_CHARIZARD tm has MOVE_FLAMETHROWER', 'MOVE_FLAMETHROWER' in learnsets.get('SPECIES_CHARIZARD', {}).get('tm', [])),
        ('SPECIES_PIKACHU tm has MOVE_THUNDERBOLT', 'MOVE_THUNDERBOLT' in learnsets.get('SPECIES_PIKACHU', {}).get('tm', [])),
        ('SPECIES_GARCHOMP tm has MOVE_EARTHQUAKE', 'MOVE_EARTHQUAKE' in learnsets.get('SPECIES_GARCHOMP', {}).get('tm', [])),
        ('SPECIES_GARCHOMP levelup has MOVE_DRAGON_CLAW @42', has_lu('SPECIES_GARCHOMP', 'MOVE_DRAGON_CLAW', 42)),
        ('SPECIES_GHOLDENGO levelup has MOVE_MAKE_IT_RAIN', has_lu('SPECIES_GHOLDENGO', 'MOVE_MAKE_IT_RAIN')),
        ('SPECIES_BULBASAUR egg has MOVE_PETAL_DANCE', 'MOVE_PETAL_DANCE' in learnsets.get('SPECIES_BULBASAUR', {}).get('egg', [])),
        ('SPECIES_MAGIKARP levelup has MOVE_SPLASH', has_lu('SPECIES_MAGIKARP', 'MOVE_SPLASH')),
        ('SPECIES_GYARADOS tm has MOVE_EARTHQUAKE', 'MOVE_EARTHQUAKE' in learnsets.get('SPECIES_GYARADOS', {}).get('tm', [])),
        ('SPECIES_DRAGAPULT levelup has MOVE_DRAGON_DARTS', has_lu('SPECIES_DRAGAPULT', 'MOVE_DRAGON_DARTS')),
        ('SPECIES_SPRIGATITO levelup has MOVE_LEAFAGE', has_lu('SPECIES_SPRIGATITO', 'MOVE_LEAFAGE')),
    ]
    for desc, good in lchecks:
        checked += 1
        if not good:
            fails.append('learnset: ' + desc)
    ichecks = [
        ('ITEM_LEFTOVERS', 'name', 'Leftovers'), ('ITEM_LEFTOVERS', 'holdEffect', 'HOLD_EFFECT_LEFTOVERS'),
        ('ITEM_POTION', 'pocket', 'POCKET_ITEMS'), ('ITEM_POKE_BALL', 'name', 'Poké Ball'),
        ('ITEM_CHOICE_BAND', 'holdEffect', 'HOLD_EFFECT_CHOICE_BAND'),
        ('ITEM_SITRUS_BERRY', 'pocket', 'POCKET_BERRIES'),
    ]
    for it, k, v in ichecks:
        checked += 1
        got = items.get(it, {}).get(k)
        if got != v:
            fails.append('%s.%s = %r (expected %r)' % (it, k, got, v))
    if fails:
        ok = False
        print('VALIDATION: %d/%d checks FAILED:' % (len(fails), checked))
        for f in fails:
            print('  FAIL', f)
    else:
        print('VALIDATION: all %d checks passed' % checked)
    return ok


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--root', required=True, help='pokeemerald-expansion source tree (read only)')
    ap.add_argument('--out', required=True, help='moves.json output path; other JSONs go next to it')
    ap.add_argument('--learnsets', help='learnsets.json path (default: next to --out)')
    ap.add_argument('--items', help='items.json path (default: next to --out)')
    ap.add_argument('--build-dir', help='scratch dir (default: <outdir>/.movedb_build)')
    ap.add_argument('--game-version', default='EMERALD', help='GAME_VERSION define (default EMERALD)')
    ap.add_argument('--cpp', help='preprocessor command (default: arm-none-eabi-cpp, cpp, gcc -E)')
    ap.add_argument('--keep-build', action='store_true', help='keep the shadow tree used for teachables')
    ap.add_argument('--quiet', action='store_true')
    args = ap.parse_args(argv)
    return build_all(args)


if __name__ == '__main__':
    sys.exit(main())
