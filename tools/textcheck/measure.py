#!/usr/bin/env python3
"""measure.py - pixel width of each displayed line of an in-game string.

Examples
  python3 -I measure.py 'Ciao, {PLAYER}! Come va?\nTutto bene.$'
  python3 -I measure.py --class battle 'Non ci credo! Ho perso!$'
  grep -A3 'Route104_Text_Foo::' data/maps/Route104/scripts.inc | python3 -I measure.py
  echo 'Un testo lungo da spezzare automaticamente...' | python3 -I measure.py --wrap

Text is written exactly as inside .string "..." (escapes \\n \\l \\p, {TOKENS}).
On stdin, `.string "..."` lines are concatenated into one text block; other
non-empty lines are measured one by one.  With --wrap the input is plain prose
(paragraphs separated by blank lines or \\p) and ready-to-paste .string lines
are printed.
"""

import argparse
import importlib.util
import json
import os
import re
import sys


def _load_textcheck():
    sys.dont_write_bytecode = True     # keep the tools directory free of __pycache__
    here = os.path.dirname(os.path.abspath(__file__))
    spec = importlib.util.spec_from_file_location('textcheck', os.path.join(here, 'textcheck.py'))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


tc = _load_textcheck()

TERM_SRC = {'n': '\\n', 'l': '\\l', 'p': '\\p', '$': '$', '': ''}


def tokens_of(text, charmap):
    toks, errs, _ = tc.parse_string_line('.string "%s"' % text, charmap)
    # columns are relative to the synthetic line: shift back
    off = len('.string "')
    errs = [(max(0, c - off), m, s) for c, m, s in errs]
    return toks, errs


def limits_for(cls):
    lim = tc.CLASS_LIMITS[cls]
    return lim, lim - tc.DOWN_ARROW_W


def measure_text(ctx, text, cls):
    toks, errs = tokens_of(text, ctx.charmap)
    dlines = ctx.measurer.lines([(t.data, 1, t.src) for t in toks])
    lim, lim_arrow = limits_for(cls)
    box = dict((id(dl), kind) for kind, dl in tc.box_violations(dlines))
    rows = []
    for dl in dlines:
        l = lim_arrow if dl['term'] in ('p', 'l') else lim
        flags = []
        if dl['width'] > lim:
            flags.append('OVER')
        elif dl['width'] > l:
            flags.append('ARROW')
        if id(dl) in box:
            flags.append('3RD-LINE' if box[id(dl)] == 'third_line' else 'L-FIRST')
        rows.append({'width': dl['width'], 'limit': l, 'free': l - dl['width'], 'term': dl['term'],
                     'text': dl['text'], 'flags': flags})
    return rows, errs


def print_rows(text, rows, errs, cls, out):
    out.write('%s\n' % text)
    for col, msg, sug in errs:
        out.write('  CHARMAP ERROR col %d: %s%s\n' % (col + 1, msg, (' -> ' + sug) if sug else ''))
    for r in rows:
        status = ','.join(r['flags']) if r['flags'] else 'ok'
        out.write('  %4dpx / %3d  %+5d  %-9s| %s\n' % (r['width'], r['limit'], r['free'], status, r['text']))


# --------------------------------------------------------------------------
# word wrapping
# --------------------------------------------------------------------------
def _width(ctx, toks):
    w = 0
    for dl in ctx.measurer.lines([(t.data, 1, t.src) for t in toks]):
        w = max(w, dl['width'])
    return w


def wrap(ctx, text, cls, style):
    """Return list of (line_text_with_terminator, width, limit)."""
    lim, lim_arrow = limits_for(cls)
    paras = [p.strip() for p in re.split(r'\\p|\n\s*\n', text) if p.strip()]
    out = []
    errors = []
    for pi, para in enumerate(paras):
        para = ' '.join(para.replace('\\n', ' ').replace('\\l', ' ').split())
        para = para.rstrip('$')
        toks, errs = tokens_of(para, ctx.charmap)
        errors.extend(errs)
        words, cur = [], []
        for t in toks:
            if t.src == ' ':
                if cur:
                    words.append(cur)
                cur = []
            else:
                cur.append(t)
        if cur:
            words.append(cur)
        last_para = pi == len(paras) - 1
        space = [tc.Tok('char', ' ', ctx.charmap.chars[ord(' ')], 0)]
        first_lim = lim
        for _attempt in range(2):
            lines = []
            line = []
            for wd in words:
                idx = len(lines)
                if style == 'box':
                    limit = lim if idx % 2 == 0 else lim_arrow
                else:
                    limit = first_lim if idx == 0 else lim_arrow
                cand = line + space + wd if line else wd
                if not line or _width(ctx, cand) <= limit:
                    line = cand
                else:
                    lines.append(line)
                    line = wd
            if line:
                lines.append(line)
            # a single-line paragraph ending in \p needs room for the arrow
            if len(lines) == 1 and not last_para and _width(ctx, lines[0]) > lim_arrow and first_lim != lim_arrow:
                first_lim = lim_arrow
                continue
            break
        for i, ln in enumerate(lines):
            last_line = i == len(lines) - 1
            if last_line:
                term = '$' if last_para else '\\p'
            elif style == 'box':
                term = '\\n' if i % 2 == 0 else '\\p'
            else:
                term = '\\n' if i == 0 else '\\l'
            w = _width(ctx, ln)
            limit = lim if term in ('\\n', '$') else lim_arrow
            out.append((''.join(t.src for t in ln) + term, w, limit))
    return out, errors


def main(argv=None):
    ap = argparse.ArgumentParser(description='Measure the pixel width of displayed lines (FONT_NORMAL).')
    ap.add_argument('text', nargs='*', help='text as inside .string "..." (default: read stdin)')
    ap.add_argument('--class', dest='cls', choices=sorted(tc.CLASS_LIMITS), default='field',
                    help='window: field 216px, battle 208px, pokenav 192px (default field)')
    ap.add_argument('--root', default=tc.DEFAULT_ROOT)
    ap.add_argument('--orig', default=tc.DEFAULT_ORIG)
    ap.add_argument('--player-width', type=int, default=42)
    ap.add_argument('--strvar-width', type=int, default=60)
    ap.add_argument('--wrap', action='store_true', help='word-wrap plain text into .string lines')
    ap.add_argument('--style', choices=('scroll', 'box'), default='scroll',
                    help='--wrap: scroll = \\n then \\l (default), box = new box (\\p) every 2 lines')
    ap.add_argument('--json', action='store_true')
    args = ap.parse_args(argv)

    root = args.root if os.path.isdir(args.root) else args.orig
    ctx = tc.Context(root, args.orig, None, args.player_width, args.strvar_width)

    if args.text:
        texts = args.text
        raw = None
    else:
        raw = sys.stdin.read()
        texts = None

    if args.wrap:
        src = ' '.join(texts) if texts else raw
        lines, errs = wrap(ctx, src, args.cls, args.style)
        if args.json:
            json.dump({'lines': [{'text': t, 'width': w, 'limit': l} for t, w, l in lines],
                       'errors': [m for _, m, _ in errs]}, sys.stdout, ensure_ascii=False, indent=1)
            print()
        else:
            for col, msg, sug in errs:
                print('@ CHARMAP ERROR: %s%s' % (msg, (' -> ' + sug) if sug else ''))
            for t, w, l in lines:
                flag = '' if w <= l else '  @ OVER: a single word is wider than the box!'
                print('\t.string "%s"%s' % (t, flag))
            print('@ widths: ' + ', '.join('%d/%d' % (w, l) for _, w, l in lines), file=sys.stderr)
        return 1 if errs or any(w > l for _, w, l in lines) else 0

    if texts is None:
        if re.search(r'^\s*\.string', raw, re.M):
            parts = []
            for line in raw.split('\n'):
                m = re.match(r'^\s*\.string\s*"(.*)"\s*$', line)
                if m:
                    parts.append(m.group(1))
            texts = [''.join(parts)]
        else:
            texts = [l for l in raw.split('\n') if l.strip()]
    bad = False
    results = []
    for text in texts:
        rows, errs = measure_text(ctx, text, args.cls)
        bad = bad or bool(errs) or any(r['flags'] for r in rows)
        if args.json:
            results.append({'text': text, 'lines': rows, 'errors': [m for _, m, _ in errs]})
        else:
            print_rows(text, rows, errs, args.cls, sys.stdout)
    if args.json:
        json.dump(results, sys.stdout, ensure_ascii=False, indent=1)
        print()
    else:
        lim, la = limits_for(args.cls)
        print('(%s box: %dpx per line, %dpx for a line followed by \\p or \\l; '
              '{PLAYER}=%dpx, {STR_VAR_n}=%dpx)' % (args.cls, lim, la, ctx.ph_widths[tc.PH_PLAYER],
                                                    ctx.ph_widths[tc.PH_STR1]))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
