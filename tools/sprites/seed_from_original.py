#!/usr/bin/env python3
"""Authoring aid (not needed for the build): dump frames of an original indexed PNG as text
grids using the letters of a palette from data/palettes.txt (slot N -> letter of slot N).
The dumps were the starting point that the hand-edited grids in data/ were drawn over.

    python3 seed_from_original.py ORIG.png FRAME_W FRAME_H PALETTE [--vertical] [--stamp NAME X Y ...]
"""
import argparse
import os
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from make_protagonist import load_palettes  # noqa: E402


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('png')
    ap.add_argument('w', type=int)
    ap.add_argument('h', type=int)
    ap.add_argument('palette')
    ap.add_argument('--vertical', action='store_true')
    ap.add_argument('--frames', help='comma separated frame indices')
    args = ap.parse_args()
    letters = [l for l, _ in load_palettes()[args.palette]]
    im = Image.open(args.png)
    px = im.load()
    n = (im.size[1] // args.h) if args.vertical else (im.size[0] // args.w)
    sel = [int(i) for i in args.frames.split(',')] if args.frames else range(n)
    for f in sel:
        ox, oy = (0, f * args.h) if args.vertical else (f * args.w, 0)
        print('== %d' % f)
        for y in range(args.h):
            print(''.join(letters[px[ox + x, oy + y] & 15] for x in range(args.w)))
        print()


if __name__ == '__main__':
    main()
