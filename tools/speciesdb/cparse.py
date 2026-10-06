"""Minimal C tokenizer / initializer parser / constant-expression evaluator.

Works on *preprocessed* C (output of cpp).  It understands just enough of C to
read the data tables of pokeemerald-expansion:

  * enum definitions (values are evaluated, so later code can map names to ints)
  * top level initialized objects ``<type> name[...] = { ... };``
  * designated initializers, nested brace lists, compound literals
  * integer / floating constant expressions (+ - * / % << >> < > <= >= == !=
    & ^ | && || ! ~ ?: casts), with C semantics for integer division.

Unknown identifiers are kept symbolic (an ``Id`` node) so that enum-typed
fields such as ``TYPE_GRASS`` or ``MAP_PETALBURG_WOODS`` survive as names.
"""

import re

__all__ = ["CSource", "Unevaluable", "InitList"]


class Unevaluable(Exception):
    pass


_TOKEN_RE = re.compile(r'''
    (?P<str>"(?:[^"\\\n]|\\.)*")
  | (?P<chr>'(?:[^'\\\n]|\\.)*')
  | (?P<num>(?:0[xX][0-9a-fA-F]+|(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?)[uUlLfF]*)
  | (?P<id>[A-Za-z_]\w*)
  | (?P<op>\.\.\.|<<=|>>=|->|\+\+|--|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^!~<>=?:;,.()\[\]{}\#@$\\`])
  | (?P<ws>\s+)
  | (?P<bad>.)
''', re.X | re.S)

_TYPE_WORDS = {
    'const', 'volatile', 'struct', 'union', 'enum', 'unsigned', 'signed', 'int',
    'char', 'short', 'long', 'void', 'float', 'double', '_Bool', 'bool',
    'u8', 'u16', 'u32', 'u64', 's8', 's16', 's32', 's64', 'vu8', 'vu16', 'vu32',
    'bool8', 'bool16', 'bool32', 'size_t', 'uintptr_t', 'intptr_t', 'f32', 'f64',
}
_INT_CASTS = {
    'u8': (8, False), 'u16': (16, False), 'u32': (32, False), 'u64': (64, False),
    's8': (8, True), 's16': (16, True), 's32': (32, True), 's64': (64, True),
    'bool8': (8, False), 'bool16': (16, False), 'bool32': (32, False),
    'int': (32, True), 'char': (8, True), 'short': (16, True), 'long': (32, True),
}

_ESCAPES = {'n': '\n', 't': '\t', 'r': '\r', '0': '\0', '\\': '\\', '"': '"', "'": "'",
            'a': '\a', 'b': '\b', 'f': '\f', 'v': '\v'}


def c_unescape(s):
    out = []
    i = 0
    while i < len(s):
        c = s[i]
        if c == '\\' and i + 1 < len(s):
            n = s[i + 1]
            if n == 'x':
                m = re.match(r'[0-9a-fA-F]+', s[i + 2:])
                if m:
                    out.append(chr(int(m.group(0), 16)))
                    i += 2 + len(m.group(0))
                    continue
            out.append(_ESCAPES.get(n, n))
            i += 2
        else:
            out.append(c)
            i += 1
    return ''.join(out)


class InitList(list):
    """A brace-enclosed initializer: list of (designators, node).

    designators is a tuple of ('f', fieldname) / ('i', node) entries.
    """

    def positional(self):
        return [v for d, v in self]

    def fields(self, warn=None):
        out = {}
        for d, v in self:
            if not d or d[0][0] != 'f':
                continue
            name = d[0][1]
            if name in out and warn is not None:
                warn(name)
            out[name] = v if len(d) == 1 else ('nested', d[1:], v)
        return out


def _parse_number(text):
    t = text.rstrip('uUlL')
    if t[:2] in ('0x', '0X'):
        return int(t, 16)
    if re.fullmatch(r'0[0-7]+', t):
        return int(t, 8)
    if re.fullmatch(r'\d+', t):
        return int(t)
    return float(t.rstrip('fF'))


class CSource:
    def __init__(self, text):
        # drop line markers / pragmas emitted by cpp
        lines = [ln for ln in text.split('\n') if not ln.lstrip().startswith('#')]
        text = '\n'.join(lines)
        toks = []
        kinds = []
        bad = []
        for m in _TOKEN_RE.finditer(text):
            k = m.lastgroup
            if k == 'ws':
                continue
            if k == 'bad':
                bad.append(m.group(0))
            toks.append(m.group(0))
            kinds.append(k)
        self.toks = toks
        self.kinds = kinds
        self.bad_chars = bad
        self.match = self._compute_matches()
        self.enums = {}          # enumerator name -> int (or None if unevaluable)
        self.enum_types = {}     # enumerator name -> enum tag name (or '')
        self.enum_failures = []  # names whose value could not be computed
        self.objects = {}        # name -> (start '{' index, end '}' index)
        self.duplicate_objects = []
        self._scan()

    # ------------------------------------------------------------------ basics
    def _compute_matches(self):
        toks = self.toks
        match = [-1] * len(toks)
        stack = []
        pairs = {')': '(', ']': '[', '}': '{'}
        for i, t in enumerate(toks):
            if self.kinds[i] != 'op':
                continue
            if t in '([{':
                stack.append(i)
            elif t in ')]}':
                if not stack or toks[stack[-1]] != pairs[t]:
                    raise SyntaxError('unbalanced %r at token %d' % (t, i))
                j = stack.pop()
                match[i] = j
                match[j] = i
        if stack:
            raise SyntaxError('unbalanced brackets at end of input')
        return match

    def _skip_attributes(self, j):
        toks = self.toks
        while j < len(toks) and toks[j] in ('__attribute__', '__attribute'):
            j = self.match[j + 1] + 1
        return j

    def _scan(self):
        toks, kinds, match = self.toks, self.kinds, self.match
        n = len(toks)
        i = 0
        depth = 0
        while i < n:
            t = toks[i]
            if t == 'enum' and kinds[i] == 'id':
                j = self._skip_attributes(i + 1)
                tag = ''
                if j < n and kinds[j] == 'id':
                    tag = toks[j]
                    j = self._skip_attributes(j + 1)
                if j < n and toks[j] == '{':
                    end = match[j]
                    self._parse_enum(j + 1, end, tag)
                    i = end + 1
                    continue
            if kinds[i] == 'op':
                if t == '{':
                    depth += 1
                elif t == '}':
                    depth -= 1
                elif t == '=' and depth == 0 and i + 1 < n and toks[i + 1] == '{':
                    k = i - 1
                    while toks[k] == ']':
                        k = match[k] - 1
                    name = toks[k]
                    end = match[i + 1]
                    if name in self.objects:
                        self.duplicate_objects.append(name)
                    self.objects[name] = (i + 1, end)
                    i = end + 1
                    continue
            i += 1

    def _parse_enum(self, a, b, tag):
        toks = self.toks
        nxt = 0
        p = a
        while p < b:
            q = self._find_end(p, b)
            if q > p:
                name = toks[p]
                if q > p + 1 and toks[p + 1] == '=':
                    try:
                        val = self.eval_int(self.parse_expr(p + 2, q))
                    except Unevaluable:
                        val = None
                        self.enum_failures.append(name)
                else:
                    val = nxt
                    if val is None:
                        self.enum_failures.append(name)
                self.enums[name] = val
                self.enum_types[name] = tag
                nxt = None if val is None else val + 1
            p = q + 1

    def _find_end(self, p, b):
        """Index of the next top-level ',' in [p, b) (or b)."""
        toks, match = self.toks, self.match
        while p < b:
            t = toks[p]
            if t == ',':
                return p
            if t in '([{' and self.kinds[p] == 'op':
                p = match[p] + 1
            else:
                p += 1
        return b

    # ----------------------------------------------------------- initializers
    def object(self, name):
        """Parsed initializer of a top level object, or None."""
        if name not in self.objects:
            return None
        a, b = self.objects[name]
        return self.parse_init(a, b)

    def parse_init(self, a, b):
        """Parse the brace list toks[a] == '{' ... toks[b] == '}'."""
        toks = self.toks
        items = InitList()
        p = a + 1
        while p < b:
            desig = []
            while toks[p] in ('.', '['):
                if toks[p] == '.':
                    desig.append(('f', toks[p + 1]))
                    p += 2
                else:
                    e = self.match[p]
                    desig.append(('i', self.parse_expr(p + 1, e)))
                    p = e + 1
            if desig:
                if toks[p] != '=':
                    raise SyntaxError('expected = after designator near token %d (%s)' % (p, ' '.join(toks[p - 5:p + 5])))
                p += 1
            if toks[p] == '{':
                e = self.match[p]
                val = ('init', self.parse_init(p, e))
                p = e + 1
                if p < b and toks[p] != ',':
                    raise SyntaxError('junk after brace initializer near %d' % p)
            else:
                q = self._find_end(p, b)
                val = self.parse_expr(p, q)
                p = q
            items.append((tuple(desig), val))
            if p < b and toks[p] == ',':
                p += 1
        return items

    # ------------------------------------------------------------ expressions
    def parse_expr(self, a, b):
        return _ExprParser(self, a, b).parse()

    def text(self, a, b):
        return ' '.join(self.toks[a:b])

    def eval(self, node, keep_ids=True):
        """Evaluate node.  Returns int/float, str (identifier kept symbolic when
        keep_ids and the node is a bare identifier), InitList, or
        ('call', name, args) / ('str', s)."""
        kind = node[0]
        if kind == 'id' and keep_ids:
            return node[1]
        if kind == 'init':
            return node[1]
        if kind in ('str', 'call'):
            return node
        return self.eval_num(node)

    def eval_int(self, node):
        v = self.eval_num(node)
        if isinstance(v, float):
            v = int(v)  # C conversion truncates toward zero
        return v

    def eval_num(self, node):
        k = node[0]
        if k == 'num':
            return node[1]
        if k == 'id':
            v = self.enums.get(node[1])
            if v is None:
                raise Unevaluable(node[1])
            return v
        if k == 'paren':
            return self.eval_num(node[1])
        if k == 'cast':
            v = self.eval_num(node[2])
            spec = _INT_CASTS.get(node[1])
            if spec is None and node[1] in ('float', 'double', 'f32', 'f64'):
                return float(v)
            if spec is None:
                return int(v) if isinstance(v, float) else v
            bits, signed = spec
            v = int(v) & ((1 << bits) - 1)
            if signed and v >= 1 << (bits - 1):
                v -= 1 << bits
            return v
        if k == 'un':
            op = node[1]
            v = self.eval_num(node[2])
            if op == '-':
                return -v
            if op == '+':
                return v
            if op == '!':
                return 0 if v else 1
            if op == '~':
                return ~int(v)
            raise Unevaluable(op)
        if k == 'tern':
            c = self.eval_num(node[1])
            return self.eval_num(node[2] if c else node[3])
        if k == 'bin':
            op = node[1]
            if op == '&&':
                return 1 if (self.eval_num(node[2]) and self.eval_num(node[3])) else 0
            if op == '||':
                return 1 if (self.eval_num(node[2]) or self.eval_num(node[3])) else 0
            x = self.eval_num(node[2])
            y = self.eval_num(node[3])
            if op == '+': return x + y
            if op == '-': return x - y
            if op == '*': return x * y
            if op == '/':
                if y == 0:
                    raise Unevaluable('division by zero')
                if isinstance(x, int) and isinstance(y, int):
                    q = abs(x) // abs(y)
                    return q if (x >= 0) == (y >= 0) else -q
                return x / y
            if op == '%':
                if y == 0:
                    raise Unevaluable('modulo by zero')
                r = abs(x) % abs(y)
                return r if x >= 0 else -r
            if op == '<<': return int(x) << int(y)
            if op == '>>': return int(x) >> int(y)
            if op == '<': return int(x < y)
            if op == '>': return int(x > y)
            if op == '<=': return int(x <= y)
            if op == '>=': return int(x >= y)
            if op == '==': return int(x == y)
            if op == '!=': return int(x != y)
            if op == '&': return int(x) & int(y)
            if op == '|': return int(x) | int(y)
            if op == '^': return int(x) ^ int(y)
            raise Unevaluable(op)
        raise Unevaluable(k)


_BINARY_PREC = {
    '||': 1, '&&': 2, '|': 3, '^': 4, '&': 5, '==': 6, '!=': 6,
    '<': 7, '>': 7, '<=': 7, '>=': 7, '<<': 8, '>>': 8,
    '+': 9, '-': 9, '*': 10, '/': 10, '%': 10,
}


class _ExprParser:
    def __init__(self, src, a, b):
        self.s = src
        self.t = src.toks
        self.k = src.kinds
        self.p = a
        self.b = b

    def peek(self):
        return self.t[self.p] if self.p < self.b else None

    def parse(self):
        if self.p >= self.b:
            return ('empty',)
        node = self.ternary()
        if self.p != self.b:
            node = ('raw', self.s.text(self.p, self.b), node)
            raise SyntaxError('trailing tokens in expression: %s' % self.s.text(self.p, self.b))
        return node

    def ternary(self):
        c = self.binary(1)
        if self.peek() == '?':
            self.p += 1
            a = self.ternary()
            if self.peek() != ':':
                raise SyntaxError('expected : in ?: near %s' % self.s.text(self.p, self.p + 5))
            self.p += 1
            b = self.ternary()
            return ('tern', c, a, b)
        return c

    def binary(self, minprec):
        left = self.unary()
        while True:
            op = self.peek()
            prec = _BINARY_PREC.get(op)
            if prec is None or prec < minprec or self.k[self.p] != 'op':
                return left
            self.p += 1
            right = self.binary(prec + 1)
            left = ('bin', op, left, right)

    def _is_cast(self, a, e):
        t = self.t
        if a >= e:
            return False
        if t[a] not in _TYPE_WORDS:
            return False
        for i in range(a, e):
            if self.k[i] == 'id' or t[i] in ('*', '[', ']') or self.k[i] == 'num':
                continue
            return False
        return True

    def unary(self):
        t = self.peek()
        if t is None:
            raise SyntaxError('unexpected end of expression')
        kind = self.k[self.p]
        if kind == 'op' and t in ('-', '+', '!', '~'):
            self.p += 1
            return ('un', t, self.unary())
        if kind == 'op' and t == '&':
            self.p += 1
            return ('addr', self.unary())
        if kind == 'op' and t == '(':
            e = self.s.match[self.p]
            if self._is_cast(self.p + 1, e):
                typ = [x for x in self.t[self.p + 1:e] if x not in ('const', 'volatile')]
                self.p = e + 1
                if self.peek() == '{':
                    e2 = self.s.match[self.p]
                    lst = self.s.parse_init(self.p, e2)
                    self.p = e2 + 1
                    return self.postfix(('init', lst))
                return ('cast', ' '.join(typ), self.unary())
        return self.postfix(self.primary())

    def postfix(self, node):
        while self.p < self.b:
            t = self.t[self.p]
            if t == '(' and node[0] == 'id':
                e = self.s.match[self.p]
                args = []
                q = self.p + 1
                while q < e:
                    r = self.s._find_end(q, e)
                    args.append(self.s.parse_expr(q, r))
                    q = r + 1
                node = ('call', node[1], args)
                self.p = e + 1
            elif t == '[':
                e = self.s.match[self.p]
                node = ('index', node, self.s.parse_expr(self.p + 1, e))
                self.p = e + 1
            elif t in ('.', '->') and self.k[self.p] == 'op':
                node = ('member', node, self.t[self.p + 1])
                self.p += 2
            else:
                break
        return node

    def primary(self):
        t = self.t[self.p]
        kind = self.k[self.p]
        if kind == 'num':
            self.p += 1
            return ('num', _parse_number(t))
        if kind == 'chr':
            self.p += 1
            return ('num', ord(c_unescape(t[1:-1])[:1] or '\0'))
        if kind == 'str':
            parts = []
            while self.p < self.b and self.k[self.p] == 'str':
                parts.append(c_unescape(self.t[self.p][1:-1]))
                self.p += 1
            return ('str', ''.join(parts))
        if kind == 'id':
            self.p += 1
            return ('id', t)
        if t == '(':
            e = self.s.match[self.p]
            inner = self.s.parse_expr(self.p + 1, e)
            self.p = e + 1
            return inner if inner[0] in ('init', 'str', 'call', 'id', 'num') else ('paren', inner)
        if t == '{':
            e = self.s.match[self.p]
            lst = self.s.parse_init(self.p, e)
            self.p = e + 1
            return ('init', lst)
        raise SyntaxError('unexpected token %r in expression: %s' % (t, self.s.text(self.p, min(self.b, self.p + 8))))
