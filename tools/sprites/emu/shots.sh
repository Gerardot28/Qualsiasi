#!/bin/sh
# Take the protagonist screenshots from a built ROM.
# Usage: shots.sh ROM OUTDIR [GBASHOT]
set -e
HERE=$(cd "$(dirname "$0")" && pwd)
ROM=$(realpath "$1"); OUT=$(realpath -m "$2")
G=${3:-$HERE/_mgba/gbashot}
[ -x "$G" ] || G=$("$HERE/build_harness.sh" | tail -1)
TMP=$(mktemp -d); mkdir -p "$OUT"
cd "$TMP"
"$G" "$ROM" "$HERE/intro.txt" intro.sav
"$G" "$ROM" "$HERE/showcase.txt" showcase.sav
python3 - "$OUT" <<'PY'
import glob, sys, os
from PIL import Image
out = sys.argv[1]
fs = sorted(glob.glob('*.ppm'))
for f in fs:
    Image.open(f).resize((720, 480), Image.NEAREST).save(os.path.join(out, 'shot_' + f[:-4] + '.png'))
cols = 4; rows = (len(fs) + cols - 1) // cols
sheet = Image.new('RGB', (cols * 484, rows * 324), (40, 40, 44))
for i, f in enumerate(fs):
    sheet.paste(Image.open(f).resize((480, 320), Image.NEAREST), ((i % cols) * 484 + 2, (i // cols) * 324 + 2))
sheet.save(os.path.join(out, 'screenshots_contact_sheet.png'))
print('%d screenshots -> %s' % (len(fs), out))
PY
rm -rf "$TMP"
