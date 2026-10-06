#!/usr/bin/env python3
"""textcheck.py - validator for rewritten in-game text of pokeemerald-expansion.

Compares the script/text files of a modified tree (--root) against a pristine
tree (--orig) and reports everything that could break the build or the
display after the `.string "..."` lines have been rewritten:

  STRUCTURE    non-.string lines (labels, commands, directives) must be identical
  TERMINATION  each text block ends with "$" iff the original did; no "$" inside
  CHARMAP      every character / {TOKEN} / escape must be encodable (mirrors
               tools/preproc: charmap.cpp + string_parser.cpp + asm_file.cpp)
  WIDTH        pixel width of every displayed line (FONT_NORMAL glyph widths
               from src/fonts.c) against the limit of the window that shows it
  BOXLINES     2-line message boxes: first break of a paragraph \\n, then \\l

Run `textcheck.py --help` for usage.  Must be runnable as `python3 -I`.
"""

import argparse
import glob
import json
import os
import re
import sys
import unicodedata
from collections import Counter, defaultdict

DEFAULT_ROOT = '/home/user/pex'
DEFAULT_ORIG = '/home/user/pex-orig'
SCOPE_GLOBS = ['data/maps/*/scripts.inc', 'data/scripts/*.inc', 'data/text/*.inc']

# --------------------------------------------------------------------------
# Window geometry (pixels), see README / --calibrate for the derivation.
#   field message box : src/menu.c sStandardTextBox_WindowTemplates width 27
#                       tiles, printer x = 0 (AddTextPrinterForMessage)
#   battle message box: src/battle_bg.c [B_WIN_MSG] width 26 tiles, x = 0
#                       (src/battle_message.c sTextOnWindowsInfo_Normal)
#   match call window : src/match_call.c sMatchCallTextWindow width 28 tiles,
#                       printerTemplate.x = 32
# --------------------------------------------------------------------------
LIMIT_FIELD = 27 * 8          # 216
LIMIT_BATTLE = 26 * 8         # 208
LIMIT_POKENAV = 28 * 8 - 32   # 192
DOWN_ARROW_W = 8              # graphics/fonts/down_arrow.png, drawn at currentX

CLASS_LIMITS = {
    'field': LIMIT_FIELD,
    'battle': LIMIT_BATTLE,
    'pokenav': LIMIT_POKENAV,
}
BOX_CLASSES = ('field', 'battle', 'pokenav')

# Placeholder defaults (pixels).  FD xx ids, see charmap.txt / string_util.c
PH_PLAYER, PH_STR1, PH_STR2, PH_STR3, PH_KUN, PH_RIVAL = 0x01, 0x02, 0x03, 0x04, 0x05, 0x06
PH_NAMES = {
    0x00: 'UNKNOWN', 0x01: 'PLAYER', 0x02: 'STR_VAR_1', 0x03: 'STR_VAR_2',
    0x04: 'STR_VAR_3', 0x05: 'KUN', 0x06: 'RIVAL', 0x07: 'VERSION', 0x08: 'AQUA',
    0x09: 'MAGMA', 0x0A: 'ARCHIE', 0x0B: 'MAXIE', 0x0C: 'KYOGRE', 0x0D: 'GROUDON',
    0x0E: 'REGION',
}
# placeholders whose expansion is a fixed string in src/strings.c
PH_FIXED_STRINGS = {
    0x05: ['gText_ExpandedPlaceholder_Kun', 'gText_ExpandedPlaceholder_Chan'],
    0x06: ['gText_ExpandedPlaceholder_Brendan', 'gText_ExpandedPlaceholder_May'],
    0x07: ['gText_ExpandedPlaceholder_Emerald'],
    0x08: ['gText_ExpandedPlaceholder_Aqua'],
    0x09: ['gText_ExpandedPlaceholder_Magma'],
    0x0A: ['gText_ExpandedPlaceholder_Archie'],
    0x0B: ['gText_ExpandedPlaceholder_Maxie'],
    0x0C: ['gText_ExpandedPlaceholder_Kyogre'],
    0x0D: ['gText_ExpandedPlaceholder_Groudon'],
    0x0E: ['gText_Hoenn', 'gText_Kanto'],
}
CALIBRATED_PH = (PH_STR1, PH_STR2, PH_STR3)   # per-label calibration from vanilla

KMAX_STRING_LENGTH = 1024     # tools/preproc/preproc.h

# Characters that are NOT in the charmap and what to use instead.
SUGGEST = {
    '"': '“ or ” (a raw " ends the string)',
    '«': '“', '»': '”', '„': '“', '‟': '“', '″': '”', '〝': '“', '〞': '”',
    '`': "'", '´': "'", '′': "'", 'ʼ': "'", 'ʹ': "'", '‛': "'", '‚': "'", 'ʻ': "'",
    '–': '-', '—': '-', '―': '-', '‐': '-', '‑': '-', '−': '-', '‒': '-',
    ' ': "' ' (plain space)", ' ': "' '", ' ': "' '", ' ': "' '",
    '​': '{ZWS} or nothing', '­': 'nothing (soft hyphen)',
    '°': 'º', '˚': 'º', '€': 'spell it out', '£': 'spell it out', '$': '(end marker)',
    '*': 'nothing', '#': 'nothing', '@': 'nothing', '[': '(', ']': ')',
    '|': '/', '^': 'nothing', '\t': "' '", '\\': 'nothing',
    '‹': '<', '›': '>', '⁄': '/', '÷': '/', '·': '·', '•': '·',
    'ı': 'i', 'İ': 'I',
}

FONT_TABLES = {   # font id -> array name in src/fonts.c
    0: 'gFontSmallLatinGlyphWidths', 1: 'gFontNormalLatinGlyphWidths',
    2: 'gFontShortLatinGlyphWidths', 3: 'gFontShortLatinGlyphWidths',
    4: 'gFontShortLatinGlyphWidths', 5: 'gFontShortLatinGlyphWidths',
    7: 'gFontNarrowLatinGlyphWidths', 8: 'gFontSmallNarrowLatinGlyphWidths',
    10: 'gFontNarrowerLatinGlyphWidths', 11: 'gFontSmallNarrowerLatinGlyphWidths',
    12: 'gFontShortNarrowLatinGlyphWidths', 13: 'gFontShortNarrowerLatinGlyphWidths',
}
FONT_NORMAL = 1
KEYPAD_WIDTHS = {0: 8, 1: 8, 2: 16, 3: 16, 4: 24, 5: 24, 6: 8, 7: 8, 8: 8, 9: 8,
                 0x0A: 8, 0x0B: 8, 0x0C: 8}       # src/text.c sKeypadIcons
# EXT_CTRL_CODE (FC xx) -> number of argument bytes (include/constants/characters.h)
EXT_CTRL_ARGS = {0x01: 1, 0x02: 1, 0x03: 1, 0x04: 3, 0x05: 1, 0x06: 1, 0x07: 0,
                 0x08: 1, 0x09: 0, 0x0A: 0, 0x0B: 2, 0x0C: 1, 0x0D: 1, 0x0E: 1,
                 0x0F: 0, 0x10: 2, 0x11: 1, 0x12: 1, 0x13: 1, 0x14: 1, 0x15: 0,
                 0x16: 0, 0x17: 0, 0x18: 0, 0x19: 1, 0x1A: 1, 0x1B: 1, 0x1C: 3}


# ==========================================================================
# Charmap (mirror of tools/preproc/charmap.cpp)
# ==========================================================================
class CharmapError(Exception):
    pass


class Charmap:
    def __init__(self, path):
        self.path = path
        self.chars = {}     # unicode code point -> bytes
        self.escapes = {}   # ascii code -> bytes
        self.consts = {}    # name -> bytes
        with open(path, 'rb') as f:
            text = f.read().decode('utf-8')
        text = self._remove_comments(text)
        for lineno, line in enumerate(text.split('\n'), 1):
            s = line.strip(' \t')
            if not s:
                continue
            m = re.match(r"^'(\\?)(.)'\s*=\s*(.*)$", s) or re.match(r"^'(\\?)(\\)'\s*=\s*(.*)$", s)
            if m:
                esc, ch, seq = m.group(1), m.group(2), m.group(3)
                data = self._seq(seq, lineno)
                if esc:
                    if ch == "'":
                        self.chars[ord("'")] = data
                    else:
                        self.escapes[ord(ch)] = data
                else:
                    self.chars[ord(ch)] = data
                continue
            m = re.match(r'^([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(.*)$', s)
            if m:
                self.consts[m.group(1)] = self._seq(m.group(2), lineno)
                continue
            raise CharmapError('%s:%d: cannot parse %r' % (path, lineno, line))
        self.byte_to_char = {}
        for cp, data in self.chars.items():
            if len(data) == 1:
                self.byte_to_char.setdefault(data[0], []).append(chr(cp))

    @staticmethod
    def _remove_comments(text):
        out = []
        in_str = False
        i, n = 0, len(text)
        while i < n:
            c = text[i]
            if in_str:
                if c == '\\' and i + 1 < n and text[i + 1] == "'":
                    out.append(text[i:i + 2])
                    i += 2
                    continue
                if c == "'":
                    in_str = False
                out.append(c)
                i += 1
            elif c == '@':
                while i < n and text[i] != '\n':
                    out.append(' ')
                    i += 1
            else:
                if c == "'":
                    in_str = True
                out.append(c)
                i += 1
        return ''.join(out)

    def _seq(self, s, lineno):
        toks = s.split()
        if not toks or not all(re.fullmatch(r'[0-9A-Fa-f]{2}', t) for t in toks):
            raise CharmapError('%s:%d: bad byte sequence %r' % (self.path, lineno, s))
        return bytes(int(t, 16) for t in toks)


# ==========================================================================
# preproc-compatible comment removal (asm_file.cpp AsmFile::RemoveComments)
# ==========================================================================
def remove_asm_comments(text):
    """Blank out comments exactly like preproc does; newlines are kept so line
    numbers stay valid."""
    out = list(text)
    n = len(text)
    pos = 0
    string_char = None
    while pos < n:
        c = text[pos]
        if string_char is not None:
            if c == '\\' and pos + 1 < n and text[pos + 1] == string_char:
                pos += 2
                continue
            if c == string_char:
                string_char = None
            pos += 1
        elif c == '@' and (pos == 0 or text[pos - 1] != '\\'):
            while pos < n and text[pos] != '\n':
                out[pos] = ' '
                pos += 1
        elif c == '/' and pos + 1 < n and text[pos + 1] == '*':
            out[pos] = out[pos + 1] = ' '
            pos += 2
            while pos < n:
                if text[pos] == '*' and pos + 1 < n and text[pos + 1] == '/':
                    out[pos] = out[pos + 1] = ' '
                    pos += 2
                    break
                if text[pos] != '\n':
                    out[pos] = ' '
                pos += 1
        else:
            if c in '"\'':
                string_char = c
            pos += 1
    return ''.join(out)


# ==========================================================================
# String parsing (mirror of string_parser.cpp / asm_file.cpp ReadString)
# ==========================================================================
class Tok:
    __slots__ = ('kind', 'src', 'data', 'col')

    def __init__(self, kind, src, data, col):
        self.kind = kind      # 'char' | 'esc' | 'brace'
        self.src = src        # source text of the token
        self.data = data      # encoded bytes
        self.col = col        # 0-based column in the line

    def __repr__(self):
        return 'Tok(%s,%r,%s)' % (self.kind, self.src, self.data.hex())


def _is_ident_start(c):
    return ('A' <= c <= 'Z') or ('a' <= c <= 'z') or c == '_'


def _is_ident_char(c):
    return _is_ident_start(c) or ('0' <= c <= '9')


def suggest_for(ch, charmap):
    if ch in SUGGEST:
        return SUGGEST[ch]
    if unicodedata.combining(ch):
        return 'use the precomposed accented letter (NFC), e.g. è not e+U+0300'
    base = unicodedata.normalize('NFD', ch)[0]
    if base != ch and ord(base) in charmap.chars:
        return base
    return None


def parse_string_line(line, charmap):
    """Parse a `.string` line (comments already removed).
    Returns (tokens, errors, pad) ; errors = list of (col, message, suggestion)."""
    toks, errs = [], []
    m = re.match(r'^[ \t]*\.string', line)
    pos = m.end()
    while pos < len(line) and line[pos] in ' \t':
        pos += 1
    if pos >= len(line) or line[pos] != '"':
        errs.append((pos, 'expected UTF-8 string literal after .string', None))
        return toks, errs, None
    pos += 1
    n = len(line)
    closed = False
    while pos < n:
        c = line[pos]
        if c == '"':
            closed = True
            pos += 1
            break
        start = pos
        if c == '{':
            pos += 1
            data = b''
            ok = True
            while True:
                while pos < n and line[pos] in ' \t':
                    pos += 1
                if pos >= n:
                    errs.append((start, 'unterminated {...} (missing "}")', None))
                    ok = False
                    break
                c2 = line[pos]
                if c2 == '}':
                    # preproc only accepts "}" right after an item, not after whitespace
                    if line[pos - 1] in ' \t' and pos - 1 > start:
                        errs.append((pos, "whitespace before '}' is rejected by preproc", line[start:pos].rstrip() + '}'))
                        ok = False
                    pos += 1
                    break
                if _is_ident_start(c2):
                    s = pos
                    while pos < n and _is_ident_char(line[pos]):
                        pos += 1
                    name = line[s:pos]
                    if name in charmap.consts:
                        data += charmap.consts[name]
                    else:
                        errs.append((s, "unknown constant '%s'" % name, _suggest_const(name, charmap)))
                        ok = False
                elif '0' <= c2 <= '9':
                    s = pos
                    if line.startswith('0x', pos):
                        pos += 2
                        hs = pos
                        while pos < n and line[pos] in '0123456789abcdefABCDEF':
                            pos += 1
                        digits = line[hs:pos]
                        if len(digits) not in (2, 4, 8):
                            errs.append((s, "hex literal '0x%s' must have 2, 4 or 8 digits" % digits, None))
                            ok = False
                            continue
                        val = int(digits, 16)
                        size = len(digits) // 2
                    else:
                        while pos < n and '0' <= line[pos] <= '9':
                            pos += 1
                        val = int(line[s:pos])
                        if val >= 0xFFFFFFFF:
                            errs.append((s, 'integer literal too large', None))
                            ok = False
                            continue
                        if pos < n and line[pos] == 'H':
                            size = 2
                            pos += 1
                        elif pos < n and line[pos] == 'W':
                            size = 4
                            pos += 1
                        else:
                            size = 4 if val >= 0x10000 else 2 if val >= 0x100 else 1
                    data += val.to_bytes(size, 'little')[:size]
                else:
                    errs.append((pos, "unexpected character %r within curly brackets" % c2, None))
                    ok = False
                    pos += 1
                    # resync: skip to closing brace
                    while pos < n and line[pos] != '}':
                        pos += 1
                    if pos < n:
                        pos += 1
                    break
            toks.append(Tok('brace', line[start:pos], data if ok else b'', start))
            continue
        if c == '\\':
            pos += 1
            if pos >= n:
                errs.append((start, 'backslash at end of line', None))
                break
            c2 = line[pos]
            pos += 1
            if c2 == '"':
                if ord('"') in charmap.chars:
                    toks.append(Tok('char', '\\"', charmap.chars[ord('"')], start))
                else:
                    errs.append((start, 'no mapping exists for double quote (\\")', '“ or ”'))
                    toks.append(Tok('char', '\\"', b'', start))
                continue
            if c2 == '\\':
                if ord('\\') in charmap.chars:
                    toks.append(Tok('char', '\\\\', charmap.chars[ord('\\')], start))
                else:
                    errs.append((start, 'no mapping exists for backslash (\\\\)', None))
                    toks.append(Tok('char', '\\\\', b'', start))
                continue
            if ord(c2) >= 128:
                errs.append((start, 'escapes using non-ASCII characters are invalid (\\%s)' % c2, None))
                continue
            if ord(c2) in charmap.escapes:
                toks.append(Tok('esc', '\\' + c2, charmap.escapes[ord(c2)], start))
            else:
                errs.append((start, "unknown escape '\\%s'" % c2, '\\n, \\l or \\p'))
            continue
        # plain character
        pos += 1
        cp = ord(c)
        if 0xDC80 <= cp <= 0xDCFF:
            errs.append((start, 'invalid UTF-8 encoding', None))
            continue
        if cp < 128 and not (0x20 <= cp <= 0x7E):
            errs.append((start, 'unexpected control character U+%04X' % cp, suggest_for(c, charmap)))
            continue
        if cp in charmap.chars:
            toks.append(Tok('char', c, charmap.chars[cp], start))
        else:
            sug = suggest_for(c, charmap)
            name = unicodedata.name(c, '?')
            errs.append((start, "character %r (U+%04X %s) is not in charmap.txt" % (c, cp, name), sug))
    if not closed:
        errs.append((n, 'unterminated string literal (missing closing ")', None))
        return toks, errs, None
    # rest of line: [ , padLength ] then nothing
    rest = line[pos:]
    pad = None
    m = re.match(r'^[ \t]*,[ \t]*(0x[0-9A-Fa-f]+|[0-9]+)[ \t]*$', rest)
    if m:
        pad = int(m.group(1), 0)
        if pad > KMAX_STRING_LENGTH:
            errs.append((pos, 'pad length greater than maximum (%d)' % KMAX_STRING_LENGTH, None))
    elif rest.strip(' \t\r'):
        hint = None
        if '"' in rest:
            hint = 'a raw " inside the text ends the string: use “ ” instead'
        errs.append((pos, 'junk at end of line: %r' % rest.strip(), hint))
    total = sum(len(t.data) for t in toks)
    if total >= KMAX_STRING_LENGTH:
        errs.append((0, 'mapped string longer than %d bytes' % KMAX_STRING_LENGTH, 'split it into several .string lines'))
    return toks, errs, pad


def _suggest_const(name, charmap):
    up = name.upper()
    if up in charmap.consts:
        return '{%s}' % up
    import difflib
    close = difflib.get_close_matches(up, list(charmap.consts.keys()), n=1, cutoff=0.8)
    return '{%s}' % close[0] if close else None


# ==========================================================================
# Fonts / widths
# ==========================================================================
def _strip_c_comments(src):
    src = re.sub(r'/\*.*?\*/', ' ', src, flags=re.S)
    return re.sub(r'//[^\n]*', ' ', src)


class Fonts:
    def __init__(self, fonts_c):
        with open(fonts_c, encoding='utf-8', errors='replace') as f:
            src = _strip_c_comments(f.read())
        self.tables = {}
        for fid, name in FONT_TABLES.items():
            m = re.search(r'\b' + name + r'\s*\[\s*\]\s*=\s*\{(.*?)\};', src, re.S)
            if not m:
                continue
            vals = [int(x, 0) for x in re.findall(r'0x[0-9A-Fa-f]+|\d+', m.group(1))]
            self.tables[fid] = vals
        if FONT_NORMAL not in self.tables:
            raise RuntimeError('gFontNormalLatinGlyphWidths not found in ' + fonts_c)

    def glyph(self, font, gid):
        t = self.tables.get(font) or self.tables[FONT_NORMAL]
        if gid < len(t):
            return t[gid]
        return 0


def load_c_strings(path, charmap):
    """Return {symbol: encoded bytes} for `const u8 name[] = _("...");` in a C file."""
    res = {}
    if not os.path.exists(path):
        return res
    with open(path, encoding='utf-8', errors='replace') as f:
        src = f.read()
    for m in re.finditer(r'\b(\w+)\s*\[\s*\]\s*=\s*_\(\s*((?:"(?:[^"\\]|\\.)*"\s*)+)\)', src):
        name = m.group(1)
        data = b''
        for lit in re.findall(r'"((?:[^"\\]|\\.)*)"', m.group(2)):
            toks, errs, _ = parse_string_line('.string "%s"' % lit, charmap)
            data += b''.join(t.data for t in toks)
        res[name] = data
    return res


class Measurer:
    """Splits encoded text into displayed lines and measures them, mirroring
    src/text.c RenderText / GetStringWidth for the latin fonts."""

    def __init__(self, fonts, ph_widths):
        self.fonts = fonts
        self.ph_widths = dict(ph_widths)   # FD id -> px

    def lines(self, units, ph_override=None, font=FONT_NORMAL):
        """units: list of (bytes, srcline).  Returns list of dicts:
        {width, fixed, ph: Counter, term: 'n'|'l'|'p'|'$'|'', line: srcline,
         glyphs: bool}"""
        phw = self.ph_widths if not ph_override else dict(self.ph_widths, **ph_override)
        out = []
        cur = None
        min_spacing = 0
        japanese = False

        def new_line(srcline):
            return {'width': 0, 'fixed': 0, 'ph': Counter(), 'term': '', 'line': srcline,
                    'glyphs': False, 'text': []}

        def add(w, fixed=True):
            if min_spacing and w < min_spacing:
                w = min_spacing
            cur['width'] += w
            if fixed:
                cur['fixed'] += w
            cur['glyphs'] = True

        for data, srcline in units:
            i = 0
            n = len(data)
            while i < n:
                if cur is None:
                    cur = new_line(srcline)
                c = data[i]
                i += 1
                if c == 0xFF:
                    cur['term'] = '$'
                    out.append(cur)
                    cur = None
                    continue
                if c in (0xFE, 0xFA, 0xFB):
                    cur['term'] = {0xFE: 'n', 0xFA: 'l', 0xFB: 'p'}[c]
                    out.append(cur)
                    cur = None
                    continue
                if c == 0xFD:
                    pid = data[i] if i < n else 0
                    i += 1
                    w = phw.get(pid, 0)
                    cur['ph'][pid] += 1
                    cur['width'] += w
                    cur['glyphs'] = True
                    continue
                if c == 0xF7:      # CHAR_DYNAMIC, filled by C code
                    i += 1
                    continue
                if c == 0xF8:
                    kid = data[i] if i < n else 0
                    i += 1
                    add(KEYPAD_WIDTHS.get(kid, 8))
                    continue
                if c == 0xF9:
                    gid = (data[i] if i < n else 0) | 0x100
                    i += 1
                    add(8 if japanese else self.fonts.glyph(font, gid))
                    continue
                if c == 0xFC:
                    code = data[i] if i < n else 0
                    i += 1
                    nargs = EXT_CTRL_ARGS.get(code, 0)
                    args = data[i:i + nargs]
                    i += nargs
                    if code == 0x06 and args:
                        font = args[0]
                    elif code == 0x0C and args:            # ESCAPE -> glyph 0x1xx
                        add(self.fonts.glyph(font, 0x100 | args[0]))
                    elif code == 0x0D and args:            # SHIFT_RIGHT
                        cur['width'] = cur['fixed'] = args[0]
                    elif code == 0x11 and args:            # CLEAR
                        cur['width'] += args[0]
                        cur['fixed'] += args[0]
                    elif code == 0x12 and args:            # SKIP
                        cur['width'] = cur['fixed'] = args[0]
                    elif code == 0x13 and args:            # CLEAR_TO
                        if args[0] > cur['width']:
                            cur['fixed'] += args[0] - cur['width']
                            cur['width'] = args[0]
                    elif code == 0x14 and args:
                        min_spacing = args[0]
                    elif code == 0x15:
                        japanese = True
                    elif code == 0x16:
                        japanese = False
                    elif code == 0x0F:                     # FILL_WINDOW
                        cur['term'] = 'p'
                        out.append(cur)
                        cur = None
                    continue
                if c == 0x3A:          # ZWS: zero width
                    continue
                add(8 if japanese else self.fonts.glyph(font, c))
        if cur is not None:
            out.append(cur)
        return out


# ==========================================================================
# File model
# ==========================================================================
LABEL_RE = re.compile(r'^[ \t]*([A-Za-z_][A-Za-z0-9_]*)(::?)')
PREPROC_RE = re.compile(r'^[ \t]*#[ \t]*(if|ifdef|ifndef|elif|else|endif)\b')
STRING_RE = re.compile(r'^[ \t]*\.string')


class Line:
    __slots__ = ('no', 'raw', 'clean', 'kind', 'label', 'norm', 'toks', 'errs', 'pad')

    def __init__(self, no, raw, clean):
        self.no = no
        self.raw = raw
        self.clean = clean
        self.toks = None
        self.errs = None
        self.pad = None
        self.label = None
        self.norm = None
        s = clean.strip(' \t\r')
        if not s:
            self.kind = 'blank'
        elif STRING_RE.match(clean):
            self.kind = 'string'
        else:
            m = PREPROC_RE.match(clean)
            if m:
                self.kind = 'pp'
                self.norm = '#' + m.group(1) + ' ' + ' '.join(s[1:].split()[1:]) if len(s[1:].split()) > 1 else '#' + m.group(1)
                return
            self.kind = 'struct'
            m = LABEL_RE.match(clean)
            if m and (len(clean) == m.end() or clean[m.end()] in ' \t\r'):
                self.label = m.group(1)
            self.norm = _normalize_struct(s)


def _normalize_struct(s):
    """Collapse whitespace outside quotes."""
    out = []
    q = None
    prev_space = False
    for ch in s:
        if q:
            out.append(ch)
            if ch == q:
                q = None
            continue
        if ch in '"\'':
            q = ch
            out.append(ch)
            prev_space = False
            continue
        if ch in ' \t\r':
            if not prev_space:
                out.append(' ')
            prev_space = True
            continue
        prev_space = False
        out.append(ch)
    return ''.join(out).strip()


class Block:
    """A run of .string lines (with interleaved blank/comment/#if lines)."""

    def __init__(self, label, lines, ordinal, direct):
        self.label = label          # owner label (or None)
        self.lines = lines          # list of Line (string + pp lines)
        self.ordinal = ordinal      # index of the block among blocks of the same owner
        self.direct = direct        # True if only blank/comment/#if lines separate it from the label

    @property
    def key(self):
        return (self.label, self.ordinal)

    @property
    def first_line(self):
        for ln in self.lines:
            if ln.kind == 'string':
                return ln.no
        return self.lines[0].no

    def string_lines(self):
        return [ln for ln in self.lines if ln.kind == 'string']

    def variants(self):
        """Return list of (name, [string Line]) for the #if branch combinations
        'first' (every #if branch) and 'last' (every #else branch)."""
        if not any(ln.kind == 'pp' for ln in self.lines):
            return [('', self.string_lines())]
        # pre-scan to count branches of each group
        groups = []
        stack = []
        line_path = []
        for ln in self.lines:
            if ln.kind == 'pp':
                d = ln.norm.split()[0][1:]
                if d in ('if', 'ifdef', 'ifndef'):
                    groups.append(1)
                    stack.append([len(groups) - 1, 0])
                elif d in ('elif', 'else'):
                    if stack:
                        stack[-1][1] += 1
                        groups[stack[-1][0]] += 1
                elif d == 'endif':
                    if stack:
                        stack.pop()
                line_path.append(None)
            else:
                line_path.append([tuple(x) for x in stack])
        res = []
        for vname in ('first', 'last'):
            sel = []
            for ln, path in zip(self.lines, line_path):
                if ln.kind != 'string':
                    continue
                ok = True
                for gid, br in path:
                    want = 0 if vname == 'first' else groups[gid] - 1
                    if br != want:
                        ok = False
                        break
                if ok:
                    sel.append(ln)
            res.append((vname, sel))
        if [l.no for l in res[0][1]] == [l.no for l in res[1][1]]:
            return [('', res[0][1])]
        return res


class AsmTextFile:
    def __init__(self, path, charmap, relname=None):
        self.path = path
        self.relname = relname or path
        with open(path, 'rb') as f:
            raw = f.read()
        text = raw.decode('utf-8', errors='surrogateescape')
        self.crlf = '\r\n' in text
        clean = remove_asm_comments(text)
        raw_lines = text.split('\n')
        clean_lines = clean.split('\n')
        self.lines = [Line(i + 1, r, c) for i, (r, c) in enumerate(zip(raw_lines, clean_lines))]
        for ln in self.lines:
            if ln.kind == 'string':
                ln.toks, ln.errs, ln.pad = parse_string_line(ln.clean.rstrip('\r'), charmap)
        self._build_blocks()

    def _build_blocks(self):
        self.blocks = []
        self.struct = []          # list of struct/pp Line in order
        self.gaps = []            # gaps[i] = number of string lines after struct[i-1] (gaps[0]: before first)
        owner = None
        owner_direct = True
        counts = Counter()
        cur = None
        gap_count = 0
        pending_pp = []           # pp lines seen since last struct/string (may start a block)
        for ln in self.lines:
            if ln.kind == 'string':
                gap_count += 1
                if cur is None:
                    cur = Block(owner, pending_pp + [ln], counts[owner], owner_direct)
                    counts[owner] += 1
                else:
                    cur.lines.extend(pending_pp)
                    cur.lines.append(ln)
                pending_pp = []
            elif ln.kind == 'blank':
                continue
            elif ln.kind == 'pp':
                self.struct.append(ln)
                self.gaps.append(gap_count)
                gap_count = 0
                pending_pp.append(ln)
            else:   # struct
                self.struct.append(ln)
                self.gaps.append(gap_count)
                gap_count = 0
                if cur is not None:
                    cur.lines.extend(pending_pp)
                    self.blocks.append(cur)
                    cur = None
                pending_pp = []
                if ln.label:
                    owner = ln.label
                    owner_direct = True
                else:
                    owner_direct = False
        if cur is not None:
            cur.lines.extend(pending_pp)
            self.blocks.append(cur)
        self.gaps.append(gap_count)
        self.block_by_key = {b.key: b for b in self.blocks}
