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
        """units: list of (bytes, srcline, srctext) - normally one per token.
        Returns a list of display lines (dicts):
          width  : pixel width (placeholders at their assumed width)
          fixed  : width without placeholders
          ph     : Counter of placeholder ids on the line
          term   : what ended the line: 'n' '\\n', 'l' '\\l', 'p' '\\p', '$' EOS, '' end of data
          line   : source line number where the display line starts
          glyphs : True if anything visible is drawn
          text   : source text of the display line (for reports)
        """
        phw = dict(self.ph_widths)
        if ph_override:
            phw.update(ph_override)
        flat = []          # (byte, unit index)
        for ui, u in enumerate(units):
            for byte in u[0]:
                flat.append((byte, ui))
        out = []
        st = {'cur': None, 'min_spacing': 0, 'japanese': False, 'font': font, 'last_unit': -1}

        def start(ui):
            st['cur'] = {'width': 0, 'fixed': 0, 'ph': Counter(), 'term': '', 'line': units[ui][1],
                         'glyphs': False, 'text': [], 'units': []}
            st['last_unit'] = -1

        def note_unit(ui):
            cur = st['cur']
            if ui != st['last_unit']:
                st['last_unit'] = ui
                cur['units'].append(ui)
                cur['text'].append(units[ui][2])

        def finish(term):
            cur = st['cur']
            cur['term'] = term
            out.append(cur)
            st['cur'] = None

        def add(w):
            if st['min_spacing'] and w < st['min_spacing']:
                w = st['min_spacing']
            cur = st['cur']
            cur['width'] += w
            cur['fixed'] += w
            cur['glyphs'] = True

        n = len(flat)
        i = 0

        def arg(k):
            return flat[k][0] if k < n else 0

        while i < n:
            c, ui = flat[i]
            if st['cur'] is None:
                start(ui)
            note_unit(ui)
            cur = st['cur']
            i += 1
            if c == 0xFF:
                finish('$')
                continue
            if c in (0xFE, 0xFA, 0xFB):
                finish({0xFE: 'n', 0xFA: 'l', 0xFB: 'p'}[c])
                continue
            if c == 0xFD:
                pid = arg(i)
                if i < n:
                    note_unit(flat[i][1])
                i += 1
                cur['ph'][pid] += 1
                cur['width'] += phw.get(pid, 0)
                cur['glyphs'] = True
                continue
            if c == 0xF7:          # CHAR_DYNAMIC, filled in by C code
                i += 1
                continue
            if c == 0xF8:          # keypad icon
                add(KEYPAD_WIDTHS.get(arg(i), 8))
                i += 1
                continue
            if c == 0xF9:          # extra symbol: glyph 0x100 | id
                gid = arg(i) | 0x100
                i += 1
                add(8 if st['japanese'] else self.fonts.glyph(st['font'], gid))
                continue
            if c == 0xFC:
                code = arg(i)
                i += 1
                nargs = EXT_CTRL_ARGS.get(code, 0)
                args = [arg(k) for k in range(i, i + nargs)]
                for k in range(i, min(i + nargs, n)):
                    note_unit(flat[k][1])
                i += nargs
                if code == 0x06 and args:
                    st['font'] = args[0]
                elif code == 0x07:
                    st['font'] = font
                elif code == 0x0C and args:            # ESCAPE -> glyph 0x1xx
                    add(self.fonts.glyph(st['font'], 0x100 | args[0]))
                elif code in (0x0D, 0x12) and args:    # SHIFT_RIGHT / SKIP: absolute x
                    cur['width'] = cur['fixed'] = args[0]
                elif code == 0x11 and args:            # CLEAR: advance
                    cur['width'] += args[0]
                    cur['fixed'] += args[0]
                elif code == 0x13 and args:            # CLEAR_TO
                    if args[0] > cur['width']:
                        cur['fixed'] += args[0] - cur['width']
                        cur['width'] = args[0]
                elif code == 0x14 and args:
                    st['min_spacing'] = args[0]
                elif code == 0x15:
                    st['japanese'] = True
                elif code == 0x16:
                    st['japanese'] = False
                elif code == 0x0F:                     # FILL_WINDOW: like a new box
                    finish('p')
                continue
            if c == 0x3A:          # ZWS: zero width
                continue
            add(8 if st['japanese'] else self.fonts.glyph(st['font'], c))
        if st['cur'] is not None:
            finish('')
        for dl in out:
            dl['text'] = ''.join(dl['text'])
            del dl['units']
        return out


# ==========================================================================
# File model
# ==========================================================================
LABEL_RE = re.compile(r'^[ \t]*([A-Za-z_][A-Za-z0-9_]*)(::?)')
# C preprocessor and GNU as conditionals: both may wrap alternative .string lines
PREPROC_RE = re.compile(r'^[ \t]*(?:#[ \t]*(if|ifdef|ifndef|elif|else|endif)|\.(if\w*|else|elseif|endif))\b')
COND_OPEN = ('if', 'ifdef', 'ifndef')
COND_ELSE = ('elif', 'else', 'elseif')
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
                d = m.group(1) or m.group(2)
                self.label = ('open' if (d in COND_OPEN or (m.group(2) and d.startswith('if')))
                              else 'else' if d in COND_ELSE else 'close')
                self.norm = _normalize_struct(s)
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
                d = ln.label        # 'open' / 'else' / 'close'
                if d == 'open':
                    groups.append(1)
                    stack.append([len(groups) - 1, 0])
                elif d == 'else':
                    if stack:
                        stack[-1][1] += 1
                        groups[stack[-1][0]] += 1
                elif d == 'close':
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


# ==========================================================================
# Usage index: how is each text label displayed?
# ==========================================================================
# Primitive script commands (or macros treated as primitives) -> (arg index -> class)
BASE_USAGE = {
    'message': {0: 'field'},
    'messageautoscroll': {0: 'field'},
    'messageinstant': {0: 'field'},
    'vmessage': {0: 'field'},
    'pokenavcall': {0: 'pokenav'},
    'bufferstring': {1: 'buffer'},
    'vbufferstring': {1: 'buffer'},
    'vbuffermessage': {0: 'buffer'},
    'dynmultipush': {0: 'menu'},
    # scripts/trainer_battle.inc + battle_setup.c: intro/cannot-battle texts are
    # shown with ShowFieldMessage, defeat/victory texts inside the battle box.
    'trainerbattle': {2: 'field', 3: 'battle', 7: 'field', 8: 'battle', 10: 'battle', 11: 'field'},
    '.4byte': {},
}
# `loadword 0, Text` + callstd -> standard message box (msgbox macro)
LOADWORD_MSG = ('loadword', 1)
# C usage patterns that clearly show a field message box
C_FIELD_CALLS = ('ShowFieldMessage', 'ShowFieldAutoScrollMessage', 'DisplayItemMessageOnField',
                 'ShowFieldMessageFromBuffer')
# C files whose text tables are displayed in a known window (see --calibrate)
# (verified in the C sources: tv.c / battle_pyramid.c / birch_pc.c call ShowFieldMessage,
#  apprentice texts are expanded into gStringVar4 and shown with `message`,
#  match_call.c prints in sMatchCallTextWindow at x=32, pokenav_match_call_gfx.c
#  prints call messages in sCallMsgBoxWindowTemplate (28 tiles) at x=32)
C_FILE_CLASSES = {
    'src/tv.c': 'field',
    'src/battle_pyramid.c': 'field',
    'src/birch_pc.c': 'field',
    'src/data/battle_frontier/apprentice.h': 'field',
    'src/match_call.c': 'pokenav',
    'src/pokenav_match_call_data.c': 'pokenav',
}

IDENT_RE = re.compile(r'[A-Za-z_]\w*')


def _split_args(rest):
    rest = rest.strip()
    if not rest:
        return []
    return [a.strip() for a in rest.split(',')]


class UsageIndex:
    def __init__(self, root, text_labels):
        self.root = root
        self.text_labels = text_labels
        self.macro_usage = {}     # (macro, argidx) -> set(classes)
        self.refs = defaultdict(list)    # label -> [(cmd, argidx, classes, file, line)]
        self._load_macros()
        self._scan_scripts()
        self._scan_c()

    def _load_macros(self):
        macros = {}
        for path in sorted(glob.glob(os.path.join(self.root, 'asm', 'macros', '**', '*.inc'), recursive=True)) + \
                [os.path.join(self.root, 'asm', 'macros', 'event.inc')]:
            if not os.path.exists(path):
                continue
            with open(path, encoding='utf-8', errors='replace') as f:
                text = remove_asm_comments(f.read())
            cur = None
            for line in text.split('\n'):
                s = line.strip()
                m = re.match(r'^\.macro\s+(\w+)\s*(.*)$', s)
                if m:
                    ps = re.sub(r':\s*req', ':req', m.group(2))
                    ps = re.sub(r'\s*=\s*', '=', ps)
                    params = [p.split(':')[0].split('=')[0] for p in re.split(r'[\s,]+', ps) if p]
                    cur = (m.group(1), params, [])
                    macros[m.group(1)] = cur
                    continue
                if s.startswith('.endm'):
                    cur = None
                    continue
                if cur is not None and s:
                    cur[2].append(s)
        usage = defaultdict(set)
        for cmd, d in BASE_USAGE.items():
            for idx, cls in d.items():
                usage[(cmd, idx)].add(cls)
        changed = True
        while changed:
            changed = False
            for name, params, body in macros.values():
                if name in BASE_USAGE:
                    continue
                for s in body:
                    m = re.match(r'^([.\w]+)\s*(.*)$', s)
                    if not m:
                        continue
                    cmd, args = m.group(1), _split_args(m.group(2))
                    for i, a in enumerate(args):
                        for pm in re.finditer(r'\\(\w+)', a):
                            if pm.group(1) not in params:
                                continue
                            pidx = params.index(pm.group(1))
                            cls = set(usage.get((cmd, i), ()))
                            if (cmd, i) == LOADWORD_MSG and args and args[0] == '0':
                                cls.add('field')
                            if cls - usage[(name, pidx)]:
                                usage[(name, pidx)] |= cls
                                changed = True
        self.macro_usage = {k: v for k, v in usage.items() if v}
        self.macros = macros

    def _scan_scripts(self):
        files = glob.glob(os.path.join(self.root, 'data', '**', '*.inc'), recursive=True) + \
            glob.glob(os.path.join(self.root, 'data', '*.s'))
        for path in sorted(files):
            rel = os.path.relpath(path, self.root)
            with open(path, encoding='utf-8', errors='replace') as f:
                text = remove_asm_comments(f.read())
            for no, line in enumerate(text.split('\n'), 1):
                s = line.strip()
                if not s or s.startswith('.string') or s.startswith('.braille'):
                    continue
                m = re.match(r'^([.\w]+)\s*(.*)$', s)
                if not m or s.startswith('#'):
                    continue
                cmd = m.group(1)
                if m.group(2).startswith(':'):
                    continue
                args = _split_args(m.group(2))
                for i, a in enumerate(args):
                    if a in self.text_labels:
                        cls = set(self.macro_usage.get((cmd, i), ()))
                        if (cmd, i) == LOADWORD_MSG and args[0] == '0':
                            cls.add('field')
                        self.refs[a].append((cmd, i, frozenset(cls), rel, no))

    def _scan_c(self):
        files = glob.glob(os.path.join(self.root, 'src', '**', '*.c'), recursive=True) + \
            glob.glob(os.path.join(self.root, 'src', '**', '*.h'), recursive=True)
        for path in sorted(files):
            rel = os.path.relpath(path, self.root)
            with open(path, encoding='utf-8', errors='replace') as f:
                text = _strip_c_comments(f.read())
            if not any(lbl in text for lbl in ('gText', 'Text_', '_Text')):
                pass
            for no, line in enumerate(text.split('\n'), 1):
                for m in IDENT_RE.finditer(line):
                    lbl = m.group(0)
                    if lbl not in self.text_labels:
                        continue
                    pre = line[:m.start()]
                    call = None
                    depth = 0
                    for j in range(len(pre) - 1, -1, -1):
                        ch = pre[j]
                        if ch == ')':
                            depth += 1
                        elif ch == '(':
                            if depth == 0:
                                cm = re.search(r'(\w+)\s*$', pre[:j])
                                call = cm.group(1) if cm else None
                                break
                            depth -= 1
                    cls = set()
                    if call in C_FIELD_CALLS:
                        cls.add('field')
                    elif rel in C_FILE_CLASSES:
                        cls.add(C_FILE_CLASSES[rel])
                    self.refs[lbl].append(('C:' + (call or '-'), -1, frozenset(cls), rel, no))

    def classify(self, label):
        """Returns (classes:set, description:str)."""
        refs = self.refs.get(label, [])
        classes = set()
        for r in refs:
            classes |= r[2]
        if not refs:
            return set(), 'unreferenced'
        if classes:
            return classes, ','.join(sorted(classes))
        kinds = sorted(set(r[0] for r in refs))
        return set(), 'unclassified(' + ','.join(kinds[:4]) + ')'


# ==========================================================================
# Issues
# ==========================================================================
class Issue:
    def __init__(self, sev, check, file, line, label, msg, width=None, limit=None, text=None, suggestion=None):
        self.sev = sev
        self.check = check
        self.file = file
        self.line = line
        self.label = label
        self.msg = msg
        self.width = width
        self.limit = limit
        self.text = text
        self.suggestion = suggestion

    def to_dict(self):
        d = {'severity': self.sev, 'check': self.check, 'file': self.file, 'line': self.line,
             'label': self.label, 'message': self.msg}
        if self.width is not None:
            d['width'] = self.width
            d['limit'] = self.limit
        if self.text is not None:
            d['text'] = self.text
        if self.suggestion:
            d['suggestion'] = self.suggestion
        return d

    def format(self):
        s = '%s:%s: %s [%s]' % (self.file, self.line, self.sev, self.check)
        if self.label:
            s += ' %s:' % self.label
        s += ' ' + self.msg
        if self.width is not None:
            s += ' (%dpx > %dpx)' % (self.width, self.limit)
        if self.text is not None:
            s += '\n    | ' + self.text
        if self.suggestion:
            s += '\n    -> suggestion: ' + self.suggestion
        return s


# ==========================================================================
# Checker
# ==========================================================================
class Context:
    def __init__(self, root, orig, charmap_path=None, player_width=42, strvar_width=60):
        self.root = root
        self.orig = orig
        cm_path = charmap_path or (os.path.join(root, 'charmap.txt') if root and os.path.exists(os.path.join(root, 'charmap.txt'))
                                   else os.path.join(orig, 'charmap.txt'))
        self.charmap = Charmap(cm_path)
        fonts_c = os.path.join(root, 'src', 'fonts.c') if root and os.path.exists(os.path.join(root or '', 'src', 'fonts.c')) \
            else os.path.join(orig, 'src', 'fonts.c')
        self.fonts = Fonts(fonts_c)
        self.player_width = player_width
        self.strvar_width = strvar_width
        self.ph_widths = self._placeholder_widths()
        self.measurer = Measurer(self.fonts, self.ph_widths)
        self.index = None

    def _placeholder_widths(self):
        strings_c = os.path.join(self.root, 'src', 'strings.c')
        if not os.path.exists(strings_c):
            strings_c = os.path.join(self.orig, 'src', 'strings.c')
        cstr = load_c_strings(strings_c, self.charmap)
        tmp = Measurer(self.fonts, {})
        w = {PH_PLAYER: self.player_width, PH_STR1: self.strvar_width,
             PH_STR2: self.strvar_width, PH_STR3: self.strvar_width}
        self.ph_sources = {}
        for pid, names in PH_FIXED_STRINGS.items():
            best = 0
            for nm in names:
                if nm in cstr:
                    ls = tmp.lines([(cstr[nm], 0, '')])
                    best = max([best] + [dl['width'] for dl in ls])
            w[pid] = best
            self.ph_sources[pid] = names
        w[PH_RIVAL] = max(w.get(PH_RIVAL, 0), self.player_width)
        return w

    def build_index(self, text_labels):
        self.index = UsageIndex(self.orig, text_labels)


def collect_text_labels(paths, charmap):
    labels = set()
    for p in paths:
        if os.path.exists(p):
            af = AsmTextFile(p, charmap)
            labels.update(b.label for b in af.blocks if b.label)
    return labels


def _units(lines):
    units = []
    for ln in lines:
        for t in ln.toks or ():
            units.append((t.data, ln.no, t.src))
    return units


def _segments(dlines):
    """split display lines at '$' into segments"""
    segs, cur = [], []
    for dl in dlines:
        cur.append(dl)
        if dl['term'] == '$':
            segs.append(cur)
            cur = []
    if cur:
        segs.append(cur)
    return segs


def box_violations(dlines):
    """Simulate the 2-line message box.  Returns list of (kind, dline):
       'third_line'  : text drawn on a 3rd row (\\n used where \\l was needed)
       'scroll_first': \\l used as the first break of a paragraph"""
    res = []
    row = 0
    for dl in dlines:
        if dl['glyphs'] and row >= 2:
            res.append(('third_line', dl))
        t = dl['term']
        if t == 'n':
            row += 1
        elif t == 'l':
            if row == 0 and dl['glyphs']:
                res.append(('scroll_first', dl))
            row = max(row, 1)
        elif t in ('p', '$', ''):
            row = 0
    return res


class LabelInfo:
    """Per-label limits calibrated on the vanilla text."""

    def __init__(self, ctx, label, orig_blocks):
        self.label = label
        classes, desc = ctx.index.classify(label) if ctx.index else (set(), 'unclassified')
        self.classes = classes
        self.desc = desc
        box = [c for c in classes if c in CLASS_LIMITS]
        self.box = bool(box)
        self.cls = min(box, key=lambda c: CLASS_LIMITS[c]) if box else None
        base = CLASS_LIMITS[self.cls] if box else LIMIT_FIELD
        self.base_limit = base
        # a line followed by \p or \l must leave room for the down arrow
        self.base_arrow = base - DOWN_ARROW_W if self.box else base

        def eff(dl):
            return self.base_arrow if dl['term'] in ('p', 'l') else base
        # --- calibrate STR_VAR widths on vanilla lines
        dlines = []
        for b in orig_blocks:
            for _, sl in b.variants():
                dlines.extend(ctx.measurer.lines(_units(sl)))
        dflt = ctx.ph_widths
        scale = {}
        for dl in dlines:
            cal = [p for p in dl['ph'] if p in CALIBRATED_PH]
            if not cal:
                continue
            other = dl['fixed'] + sum(dflt.get(p, 0) * k for p, k in dl['ph'].items() if p not in CALIBRATED_PH)
            tok = sum(dflt[p] * dl['ph'][p] for p in cal)
            if tok <= 0:
                continue
            sc = (eff(dl) - other) / tok
            for p in cal:
                scale[p] = min(scale.get(p, 1.0), sc)
        self.ph_override = {p: max(0, int(dflt[p] * sc)) for p, sc in scale.items() if sc < 1.0}
        # --- vanilla maxima under the calibrated model
        self.vanilla_max = 0          # widest line not followed by \p/\l
        self.vanilla_max_arrow = 0    # widest line followed by \p/\l
        self.vanilla_box = Counter()
        for b in orig_blocks:
            for _, sl in b.variants():
                dl2 = ctx.measurer.lines(_units(sl), self.ph_override)
                for dl in dl2:
                    if dl['term'] in ('p', 'l'):
                        self.vanilla_max_arrow = max(self.vanilla_max_arrow, dl['width'])
                    else:
                        self.vanilla_max = max(self.vanilla_max, dl['width'])
                for seg in _segments(dl2):
                    for kind, _ in box_violations(seg):
                        self.vanilla_box[kind] += 1
        if self.box:
            self.limit = max(base, self.vanilla_max)
            self.limit_arrow = max(self.base_arrow, self.vanilla_max_arrow)
        else:
            # unclassified: max(vanilla width of this label, standard limit)
            self.limit = self.limit_arrow = max(base, self.vanilla_max, self.vanilla_max_arrow)
        self.preexisting_overflow = self.vanilla_max > base or self.vanilla_max_arrow > self.base_arrow

    def line_limit(self, dl):
        return self.limit_arrow if dl['term'] in ('p', 'l') else self.limit


class Checker:
    def __init__(self, ctx, warn_arrow=False):
        self.ctx = ctx
        self.issues = []
        self.stats = Counter()
        self.label_infos = {}
        self.warn_arrow = warn_arrow

    def add(self, *a, **kw):
        self.issues.append(Issue(*a, **kw))

    def label_info(self, label, orig_af):
        li = self.label_infos.get(label)
        if li is None:
            blocks = [b for b in orig_af.blocks if b.label == label] if orig_af else []
            li = LabelInfo(self.ctx, label, blocks)
            self.label_infos[label] = li
        return li

    # ------------------------------------------------------------------
    def check_file(self, rel, new_path, orig_path):
        ctx = self.ctx
        self.stats['files'] += 1
        try:
            new = AsmTextFile(new_path, ctx.charmap, rel)
        except OSError as e:
            self.add('error', 'IO', rel, 0, None, 'cannot read new file: %s' % e)
            return
        orig = None
        if orig_path and os.path.exists(orig_path):
            orig = AsmTextFile(orig_path, ctx.charmap, rel)
        else:
            self.add('warning', 'STRUCTURE', rel, 0, None, 'no original file to compare with (%s)' % orig_path)

        self._check_charmap(rel, new)
        if orig is None:
            return
        if new.crlf and not orig.crlf:
            self.add('warning', 'STRUCTURE', rel, 0, None, 'file now uses CRLF line endings')
        struct_ok = self._check_structure(rel, new, orig)
        self._check_blocks(rel, new, orig, struct_ok)

    # ------------------------------------------------------------------
    def _check_charmap(self, rel, af):
        for ln in af.lines:
            if ln.kind != 'string':
                continue
            self.stats['string_lines'] += 1
            for col, msg, sug in ln.errs:
                self.add('error', 'CHARMAP', rel, ln.no, None, '%s (col %d)' % (msg, col + 1),
                         text=ln.raw.strip(), suggestion=sug)
            toks = ln.toks or []
            for i, t in enumerate(toks):
                if t.src == '‘' and i > 0 and toks[i - 1].src.isalpha() and \
                        i + 1 < len(toks) and toks[i + 1].src.isalpha():
                    self.add('warning', 'STYLE', rel, ln.no, None,
                             "‘ (opening quote) used as apostrophe (col %d)" % (t.col + 1),
                             text=ln.raw.strip(), suggestion="use ' or ’")

    def _check_structure(self, rel, new, orig):
        a = [ln.norm for ln in orig.struct]
        b = [ln.norm for ln in new.struct]
        if a != b:
            n = min(len(a), len(b))
            i = 0
            while i < n and a[i] == b[i]:
                i += 1
            import difflib
            ndiff = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in
                        difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes() if tag != 'equal')
            nl = new.struct[i].no if i < len(b) else (new.lines[-1].no if new.lines else 0)
            exp = a[i] if i < len(a) else '<end of file>'
            got = b[i] if i < len(b) else '<end of file>'
            self.add('error', 'STRUCTURE', rel, nl, None,
                     'non-.string line differs from original (orig line %s): expected %r, found %r; %d differing line(s) in total'
                     % (orig.struct[i].no if i < len(a) else '-', exp, got, ndiff))
            return False
        for i, (ga, gb) in enumerate(zip(orig.gaps, new.gaps)):
            if (ga > 0) != (gb > 0):
                where = new.struct[i - 1].no if i > 0 else 1
                if ga == 0:
                    msg = '.string line(s) inserted where the original has none (after line %d) - would inject bytes into the script' % where
                else:
                    msg = 'all .string lines removed after line %d' % where
                self.add('error', 'STRUCTURE', rel, where, None, msg)
        return True

    # ------------------------------------------------------------------
    def _check_blocks(self, rel, new, orig, struct_ok):
        ctx = self.ctx
        for ob in orig.blocks:
            nb = new.block_by_key.get(ob.key)
            label = ob.label
            if nb is None:
                self.add('error', 'TERMINATION', rel, ob.first_line, label,
                         'text block missing in new file (orig line %d)' % ob.first_line)
                continue
            self.stats['blocks'] += 1
            if [l.raw for l in ob.lines] != [l.raw for l in nb.lines]:
                self.stats['blocks_changed'] += 1
            li = self.label_info(label, orig)
            ov = dict(ob.variants())
            for vname, nsl in nb.variants():
                osl = ov.get(vname)
                if osl is None:
                    osl = list(ov.values())[0]
                self._check_termination(rel, label, vname, osl, nsl, nb)
                self._check_display(rel, label, li, nsl)
        okeys = set(b.key for b in orig.blocks)
        for nb in new.blocks:
            if nb.key not in okeys:
                self.add('error', 'TERMINATION', rel, nb.first_line, nb.label,
                         'text block not present in the original (label %s, block #%d)' % (nb.label, nb.ordinal + 1))

    def _check_termination(self, rel, label, vname, osl, nsl, nb):
        def info(sl):
            data = b''.join(t.data for l in sl for t in (l.toks or ()))
            return data, data.count(b'\xff'), data.endswith(b'\xff')
        od, ok_, oend = info(osl)
        nd, nk, nend = info(nsl)
        vtag = (' [%s #if branch]' % vname) if vname else ''
        last = nsl[-1].no if nsl else nb.first_line
        # "$" must be the last thing on its .string line
        for ln in nsl:
            toks = [t for t in (ln.toks or ()) if t.data]
            for i, t in enumerate(toks):
                if b'\xff' in t.data and i != len(toks) - 1:
                    self.add('error', 'TERMINATION', rel, ln.no, label,
                             'text after "$" on the same line is never displayed' + vtag, text=ln.raw.strip())
        if oend and not nend:
            self.add('error', 'TERMINATION', rel, last, label,
                     'block must end with "$" (original does); the text would run into the next label' + vtag,
                     text=nsl[-1].raw.strip() if nsl else None)
        elif nend and not oend:
            self.add('error', 'TERMINATION', rel, last, label,
                     'block ends with "$" but the original does not (it continues into the next label)' + vtag)
        if nk > ok_:
            # find the first premature terminator
            seen = 0
            for ln in nsl:
                cnt = sum(t.data.count(b'\xff') for t in (ln.toks or ()))
                seen += cnt
                if cnt and seen <= nk - (1 if nend else 0) and ln is not nsl[-1]:
                    self.add('error', 'TERMINATION', rel, ln.no, label,
                             '"$" in the middle of the block: the text after it is cut off' + vtag,
                             text=ln.raw.strip())
                    break
            else:
                self.add('error', 'TERMINATION', rel, last, label,
                         'more "$" terminators (%d) than the original (%d)%s' % (nk, ok_, vtag))
        elif nk < ok_ and not (oend and not nend):
            self.add('error', 'TERMINATION', rel, last, label,
                     'fewer "$" terminators (%d) than the original (%d)%s' % (nk, ok_, vtag))

    def _check_display(self, rel, label, li, nsl):
        ctx = self.ctx
        dlines = ctx.measurer.lines(_units(nsl), li.ph_override)
        for dl in dlines:
            self.stats['display_lines'] += 1
            lim = li.line_limit(dl)
            if dl['width'] <= lim:
                continue
            arrow_only = dl['width'] <= li.limit
            if li.box:
                what = 'the %s box' % li.cls
                if li.preexisting_overflow:
                    what += ' (limit raised to the vanilla width of this label)'
            else:
                what = 'unclassified text [%s] (limit = max(vanilla %dpx, %dpx))' % (
                    li.desc, max(li.vanilla_max, li.vanilla_max_arrow), LIMIT_FIELD)
            if arrow_only:
                msg = ('line before \\%s leaves no room for the %dpx "more text" arrow in %s'
                       % (dl['term'], DOWN_ARROW_W, what))
                sev = 'warning' if self.warn_arrow else 'error'
                chk = 'ARROW'
            else:
                msg = 'line too wide for %s' % what
                sev = 'error'
                chk = 'WIDTH'
            self.add(sev, chk, rel, dl['line'], label, msg + _ph_note(dl, li, ctx),
                     width=dl['width'], limit=lim, text=dl['text'])
        if 'buffer' in li.classes:
            w = max([dl['width'] for dl in dlines] + [0])
            vw = max(li.vanilla_max, li.vanilla_max_arrow)
            if w > max(vw, ctx.strvar_width):
                self.add('warning', 'WIDTH', rel, nsl[0].no if nsl else 0, label,
                         'text inserted via bufferstring is wider than the vanilla one (%dpx vs %dpx): '
                         'check the lines that print it through {STR_VAR_n}' % (w, vw))
        if not li.box:
            return
        for seg in _segments(dlines):
            for kind, dl in box_violations(seg):
                pre = li.vanilla_box[kind] > 0
                sev = 'warning' if pre else 'error'
                note = ' (also in vanilla)' if pre else ''
                if kind == 'third_line':
                    self.add(sev, 'BOXLINES', rel, dl['line'], label,
                             'text on a 3rd line of the 2-line %s box: after the first \\n of a paragraph '
                             'continue with \\l (scroll) or start a new box with \\p%s' % (li.cls, note),
                             text=dl['text'])
                elif kind == 'scroll_first':
                    self.add(sev, 'BOXLINES', rel, dl['line'], label,
                             'first break of a paragraph is \\l (it scrolls a single line away): '
                             'use \\n for the first break, \\l for the following ones%s' % note,
                             text=dl['text'])


def _ph_note(dl, li, ctx):
    if not dl['ph']:
        return ''
    parts = []
    for p in sorted(dl['ph']):
        w = li.ph_override.get(p, ctx.ph_widths.get(p, 0))
        parts.append('{%s}=%dpx' % (PH_NAMES.get(p, 'FD %02X' % p), w))
    return '; assuming ' + ', '.join(parts)


# ==========================================================================
# File selection
# ==========================================================================
def scope_files(root, orig):
    rels = set()
    for base in (orig, root):
        if not base:
            continue
        for g in SCOPE_GLOBS:
            for p in glob.glob(os.path.join(base, g)):
                rels.add(os.path.relpath(p, base))
    return sorted(rels)


def resolve_files(args_files, root, orig):
    rels = []
    for f in args_files:
        cands = []
        if os.path.isabs(f):
            for base in (root, orig):
                if base and os.path.abspath(f).startswith(os.path.abspath(base) + os.sep):
                    cands = [os.path.relpath(os.path.abspath(f), base)]
                    break
            else:
                cands = [f]
        else:
            g = sorted(glob.glob(os.path.join(root, f))) or sorted(glob.glob(os.path.join(orig, f)))
            cands = [os.path.relpath(p, root if p.startswith(root) else orig) for p in g] or [f]
        rels.extend(cands)
    return rels


# ==========================================================================
# Calibration report
# ==========================================================================
def calibrate(ctx, rels, out):
    ctx_lines = defaultdict(list)    # class -> [(width, file, line, label, text, term)]
    raw_over = Counter()
    player_lines = []
    cal_labels = 0
    box_v = defaultdict(Counter)
    arrow = Counter()
    preexist = []
    unclassified = Counter()
    c_groups = defaultdict(list)
    for rel in rels:
        p = os.path.join(ctx.orig, rel)
        if not os.path.exists(p):
            continue
        af = AsmTextFile(p, ctx.charmap, rel)
        seen = set()
        for b in af.blocks:
            if b.label in seen:
                continue
            seen.add(b.label)
            li = LabelInfo(ctx, b.label, [x for x in af.blocks if x.label == b.label])
            cls = li.cls if li.box else ('unclassified' if li.desc != 'unreferenced' else 'unreferenced')
            if li.ph_override:
                cal_labels += 1
            if li.preexisting_overflow:
                preexist.append((max(li.vanilla_max, li.vanilla_max_arrow), li.base_limit, rel, b.label, cls))
            if not li.box:
                unclassified[li.desc.split('(')[0]] += 1
                for r in ctx.index.refs.get(b.label, []):
                    if r[0].startswith('C'):
                        c_groups[r[3]].append(li.vanilla_max)
                        break
            for bb in [x for x in af.blocks if x.label == b.label]:
                for _, sl in bb.variants():
                    d0 = ctx.measurer.lines(_units(sl))
                    for dl in d0:
                        if dl['width'] > li.base_limit:
                            raw_over[cls] += 1
                        if PH_PLAYER in dl['ph']:
                            player_lines.append((dl['fixed'], dl['ph'][PH_PLAYER], rel, dl['line'], dl['text'], cls))
                    d1 = ctx.measurer.lines(_units(sl), li.ph_override)
                    for dl in d1:
                        ctx_lines[cls].append((dl['width'], rel, dl['line'], b.label, dl['text'], dl['term']))
                        if not dl['ph']:
                            ctx_lines[cls + ' (no placeholders)'].append(
                                (dl['width'], rel, dl['line'], b.label, dl['text'], dl['term']))
                        if dl['term'] in ('p', 'l') and not dl['ph']:
                            ctx_lines[cls + ' (no placeholders, before \\p/\\l)'].append(
                                (dl['width'], rel, dl['line'], b.label, dl['text'], dl['term']))
                        if li.box and dl['term'] in ('p', 'l') and dl['width'] > li.base_arrow:
                            arrow[cls] += 1
                    if li.box:
                        for seg in _segments(d1):
                            for kind, dl in box_violations(seg):
                                box_v[cls][kind] += 1
                                if box_v[cls][kind] <= 3:
                                    box_v[cls]['ex_' + kind + '_%d' % box_v[cls][kind]] = 0
                                    out.append('    vanilla %s %s: %s:%d %s | %s' % (cls, kind, rel, dl['line'], b.label, dl['text']))
    w = out.append
    w('')
    w('== Placeholder widths (px) ==')
    for pid in sorted(ctx.ph_widths):
        w('  {%s} = %d' % (PH_NAMES.get(pid, pid), ctx.ph_widths[pid]))
    w('== Display line width distribution per usage class (calibrated placeholders) ==')
    buckets = [0, 160, 176, 192, 200, 208, 216, 224, 1000]
    for cls in sorted(ctx_lines):
        L = ctx_lines[cls]
        ws = sorted(x[0] for x in L)
        hist = []
        for lo, hi in zip(buckets, buckets[1:]):
            hist.append('%d-%d:%d' % (lo, hi - 1, sum(1 for x in ws if lo <= x < hi)))
        lim = CLASS_LIMITS.get(cls.split()[0], LIMIT_FIELD)
        w('  %-13s lines=%6d max=%3d p99=%3d p999=%3d  >%d: %d   %s' % (
            cls, len(ws), ws[-1] if ws else 0, ws[int(len(ws) * .99)] if ws else 0,
            ws[int(len(ws) * .999)] if ws else 0, lim, sum(1 for x in ws if x > lim), ' '.join(hist)))
        for item in sorted(L, reverse=True)[:5]:
            w('      %3dpx %s:%d %s | %s' % (item[0], item[1], item[2], item[3], item[4]))
    w('== Lines over the class limit with UNcalibrated STR_VAR=%dpx: %s' % (ctx.strvar_width, dict(raw_over)))
    w('== Labels whose STR_VAR width was lowered by vanilla calibration: %d' % cal_labels)
    w('== {PLAYER} lines: %d; widest fixed part + %dpx per {PLAYER}:' % (len(player_lines), ctx.player_width))
    pl = sorted(player_lines, key=lambda x: -(x[0] + x[1] * ctx.player_width))[:5]
    for fx, k, rel, ln, txt, cls in pl:
        w('      %3dpx (%s) %s:%d | %s' % (fx + k * ctx.player_width, cls, rel, ln, txt))
    for pw in (42, 48, 54, 60):
        over = sum(1 for fx, k, rel, ln, txt, cls in player_lines
                   if fx + k * pw > CLASS_LIMITS.get(cls, LIMIT_FIELD))
        w('      PLAYER=%dpx -> %d vanilla lines over their limit' % (pw, over))
    w('== Pre-existing vanilla overflows (label limit raised to vanilla width): %d' % len(preexist))
    for item in sorted(preexist, reverse=True)[:25]:
        w('      %3dpx > %3d  %s %s (%s)' % item)
    w('== Box rule in vanilla (field/battle/pokenav): %s' % {k: {kk: vv for kk, vv in v.items() if not kk.startswith('ex_')} for k, v in box_v.items()})
    w('== Lines before \\p/\\l whose down arrow (+%dpx) would be clipped: %s' % (DOWN_ARROW_W, dict(arrow)))
    w('== Unclassified labels: %s' % dict(unclassified))
    w('== Unclassified labels referenced from C, by file (count, max vanilla width):')
    for f, ws in sorted(c_groups.items(), key=lambda x: -len(x[1])):
        w('      %-55s %4d  max=%d' % (f, len(ws), max(ws)))


# ==========================================================================
# main
# ==========================================================================
def main(argv=None):
    ap = argparse.ArgumentParser(
        description='Validate rewritten .string text against the original pokeemerald-expansion tree.',
        epilog='Exit status: 0 = no errors, 1 = errors found, 2 = usage problem.')
    ap.add_argument('files', nargs='*', help='files to check, relative to --root (globs allowed). '
                    'Default: ' + ' '.join(SCOPE_GLOBS))
    ap.add_argument('--root', default=DEFAULT_ROOT, help='modified tree (default %(default)s)')
    ap.add_argument('--orig', default=DEFAULT_ORIG, help='pristine tree (default %(default)s)')
    ap.add_argument('--single', nargs=2, metavar=('NEW', 'ORIG'),
                    help='check one file given explicit paths (e.g. an agent\'s draft vs the original)')
    ap.add_argument('--charmap', help='charmap.txt to use (default: ROOT/charmap.txt, else ORIG/charmap.txt)')
    ap.add_argument('--json', action='store_true', help='machine-readable output on stdout')
    ap.add_argument('--player-width', type=int, default=42, help='px assumed for {PLAYER} (default 42 = 7 x 6px)')
    ap.add_argument('--strvar-width', type=int, default=60, help='px assumed for {STR_VAR_n} (default 60 = 10 x 6px)')
    ap.add_argument('--arrow-warning', action='store_true',
                    help='report a line before \\p/\\l that leaves no room for the 8px arrow as a warning, not an error')
    ap.add_argument('--no-warnings', action='store_true', help='only print errors')
    ap.add_argument('--strict', action='store_true', help='exit 1 on warnings too')
    ap.add_argument('--list-unclassified', action='store_true', help='list labels whose display window is unknown')
    ap.add_argument('--max-issues', type=int, default=0, help='print at most N issues (0 = all)')
    ap.add_argument('--calibrate', action='store_true', help='print calibration statistics of the ORIG tree and exit')
    args = ap.parse_args(argv)

    root = os.path.abspath(args.root) if args.root else None
    orig = os.path.abspath(args.orig)
    if not os.path.isdir(orig):
        print('error: --orig %s is not a directory' % orig, file=sys.stderr)
        return 2
    if root and not os.path.isdir(root):
        print('error: --root %s is not a directory' % root, file=sys.stderr)
        return 2
    try:
        ctx = Context(root or orig, orig, args.charmap, args.player_width, args.strvar_width)
    except (OSError, CharmapError, RuntimeError) as e:
        print('error: %s' % e, file=sys.stderr)
        return 2

    all_rels = scope_files(None, orig)
    labels = collect_text_labels([os.path.join(orig, r) for r in all_rels], ctx.charmap)
    ctx.build_index(labels)

    if args.calibrate:
        out = []
        calibrate(ctx, all_rels, out)
        print('\n'.join(out))
        return 0

    checker = Checker(ctx, warn_arrow=args.arrow_warning)
    if args.single:
        new_path, orig_path = args.single
        ap_orig = os.path.abspath(orig_path)
        rel = os.path.relpath(ap_orig, orig) if ap_orig.startswith(orig + os.sep) else orig_path
        checker.check_file(new_path if not args.json else new_path, new_path, orig_path)
        targets = [new_path]
    else:
        rels = resolve_files(args.files, root, orig) if args.files else scope_files(root, orig)
        for rel in rels:
            np_ = os.path.join(root, rel)
            if not os.path.exists(np_):
                checker.add('error', 'STRUCTURE', rel, 0, None, 'file missing in --root')
                continue
            checker.check_file(rel, np_, os.path.join(orig, rel))
        targets = rels

    issues = checker.issues
    if args.no_warnings:
        issues = [i for i in issues if i.sev == 'error']
    errors = [i for i in checker.issues if i.sev == 'error']
    warnings = [i for i in checker.issues if i.sev == 'warning']
    by_check = defaultdict(Counter)
    for i in checker.issues:
        by_check[i.sev][i.check] += 1
    labels_seen = checker.label_infos
    cls_count = Counter((li.cls if li.box else ('unreferenced' if li.desc == 'unreferenced' else 'unclassified'))
                        for li in labels_seen.values())
    unclassified = sorted(l for l, li in labels_seen.items() if not li.box)
    summary = {
        'files': checker.stats['files'],
        'string_lines': checker.stats['string_lines'],
        'text_blocks': checker.stats['blocks'],
        'text_blocks_changed': checker.stats['blocks_changed'],
        'display_lines': checker.stats['display_lines'],
        'errors': len(errors),
        'warnings': len(warnings),
        'errors_by_check': dict(by_check['error']),
        'warnings_by_check': dict(by_check['warning']),
        'labels_by_class': dict(cls_count),
        'preexisting_vanilla_overflows': sum(1 for li in labels_seen.values() if li.preexisting_overflow),
        'limits_px': {'field': LIMIT_FIELD, 'battle': LIMIT_BATTLE, 'pokenav': LIMIT_POKENAV,
                      'unclassified': 'max(vanilla, %d)' % LIMIT_FIELD},
        'placeholder_px': {PH_NAMES.get(k, str(k)): v for k, v in sorted(ctx.ph_widths.items())},
    }
    rc = 1 if errors or (args.strict and warnings) else 0
    if args.json:
        res = {'summary': summary, 'issues': [i.to_dict() for i in issues]}
        if args.list_unclassified:
            res['unclassified'] = [{'label': l, 'usage': labels_seen[l].desc,
                                    'limit': labels_seen[l].limit} for l in unclassified]
        json.dump(res, sys.stdout, ensure_ascii=False, indent=1)
        print()
        return rc
    shown = issues if not args.max_issues else issues[:args.max_issues]
    for i in shown:
        print(i.format())
    if args.max_issues and len(issues) > args.max_issues:
        print('... %d more issue(s) not shown' % (len(issues) - args.max_issues))
    if args.list_unclassified:
        print('\nUnclassified labels (limit = max(vanilla, %dpx), no box-line rule):' % LIMIT_FIELD)
        for l in unclassified:
            print('  %-60s %-30s limit=%d' % (l, labels_seen[l].desc, labels_seen[l].limit))
    print('\n== textcheck summary ==')
    print('files: %d   .string lines: %d   text blocks: %d (changed: %d)   display lines: %d' % (
        summary['files'], summary['string_lines'], summary['text_blocks'],
        summary['text_blocks_changed'], summary['display_lines']))
    print('labels by display class: %s' % ', '.join('%s=%d' % kv for kv in sorted(cls_count.items())))
    print('limits: field %dpx, battle %dpx, pokenav %dpx, unclassified max(vanilla, %dpx); '
          'placeholders: PLAYER %d, RIVAL %d, STR_VAR %d (calibrated per label)' % (
              LIMIT_FIELD, LIMIT_BATTLE, LIMIT_POKENAV, LIMIT_FIELD,
              ctx.ph_widths[PH_PLAYER], ctx.ph_widths[PH_RIVAL], ctx.ph_widths[PH_STR1]))
    print('errors: %d %s' % (len(errors), dict(by_check['error']) if errors else ''))
    print('warnings: %d %s' % (len(warnings), dict(by_check['warning']) if warnings else ''))
    print('RESULT: %s' % ('FAIL' if rc else 'OK'))
    return rc


if __name__ == '__main__':
    sys.exit(main())
