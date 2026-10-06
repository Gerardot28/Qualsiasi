"""Source-repository helpers: find candidate pics and their real palettes.

Many decomp hacks keep the palette of a front pic in a separate
`graphics/trainers/palettes/*.pal` (FRLG-style) instead of the PNG's own
palette; rendering the PNG alone then shows wrong colours. `palette_for()`
reads the repo's `src/data/graphics/trainers.h` to find the palette that the
game actually loads for a given front-pic PNG.
"""
import os
import re

_PIC_RE = re.compile(r'gTrainerFrontPic_(\w+)\[\]\s*=\s*INC\w+\("([^"]+)"')
_PAL_RE = re.compile(r'gTrainerPalette_(\w+)\[\]\s*=\s*INC\w+\("([^"]+)"')
_cache = {}


def _norm(p):
    p = re.sub(r'\.(4bpp|gbapal)(\.lz|\.smol)?$', '', p)
    p = re.sub(r'\.(png|pal)$', '', p)
    return p


def _table(repo_root):
    if repo_root in _cache:
        return _cache[repo_root]
    pics, pals = {}, {}
    hdr = os.path.join(repo_root, 'src/data/graphics/trainers.h')
    if os.path.exists(hdr):
        txt = open(hdr, encoding='utf-8', errors='replace').read()
        for sym, path in _PIC_RE.findall(txt):
            pics[_norm(path)] = sym
        for sym, path in _PAL_RE.findall(txt):
            pals[sym] = path
    _cache[repo_root] = (pics, pals)
    return pics, pals


def palette_for(repo_root, png_path):
    """Return the external .pal the game uses for this PNG, or None when the
    PNG's embedded palette is used (or the repo has no table)."""
    rel = os.path.relpath(png_path, repo_root)
    pics, pals = _table(repo_root)
    sym = pics.get(_norm(rel))
    if not sym or sym not in pals:
        sib = os.path.splitext(png_path)[0] + '.pal'  # asset packs ship X.png + X.pal
        return sib if os.path.exists(sib) else None
    palpath = pals[sym]
    if _norm(palpath) == _norm(rel):
        return None  # palette embedded in the PNG
    cand = os.path.join(repo_root, _norm(palpath) + '.pal')
    if os.path.exists(cand):
        return cand
    return None
