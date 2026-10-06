#!/usr/bin/env python3
"""selftest.py - build a copy of a few vanilla files with deliberate mistakes
and verify that textcheck.py detects every one of them.

  python3 -I selftest.py [--dir /home/user/work/textcheck-test] [--orig /home/user/pex-orig] [--report]

The test tree is written to --dir (data/... relative layout), then textcheck is
run with --root DIR --orig ORIG on those files.  Exit code 0 = all expected
issues found and no unexpected errors.
"""

import argparse
import importlib.util
import io
import json
import os
import shutil
import sys
from contextlib import redirect_stdout


def _load_textcheck():
    sys.dont_write_bytecode = True     # keep the tools directory free of __pycache__
    here = os.path.dirname(os.path.abspath(__file__))
    spec = importlib.util.spec_from_file_location('textcheck', os.path.join(here, 'textcheck.py'))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


R104 = 'data/maps/Route104/scripts.inc'
TRN = 'data/text/trainers.inc'
MAUV = 'data/maps/MauvilleCity/scripts.inc'
JUAN = 'data/maps/SootopolisCity_Gym_1F/scripts.inc'
SPACE = 'data/maps/MossdeepCity_SpaceCenter_2F/scripts.inc'
LOUNGE7 = 'data/maps/BattleFrontier_Lounge7/scripts.inc'

# (file, exact original line, replacement lines, expected (severity, check, label, message substring))
MUTATIONS = [
    (R104, '\t.string "That seaside cottage is where\\n"',
     ['\t.string "Quella casetta sul mare è dove abita il vecchio\\n"'],
     ('error', 'WIDTH', 'Route104_Text_BrineyLivesInSeasideCottage', 'too wide for the field box')),
    (R104, '\t.string "of the sea?$"',
     ['\t.string "del mare?"'],
     ('error', 'TERMINATION', 'Route104_Text_WhatsItLikeAtBottomOfSea', 'must end with "$"')),
    (R104, '\t.string "If you\'re going to throw a POKé BALL,\\n"',
     ['\t.string "Lancia la BALL — «subito»!\\n"'],
     ('error', 'CHARMAP', None, 'U+2014')),
    (None, None, None, ('error', 'CHARMAP', None, 'U+00AB')),
    (R104, '\t.string "It will be easier to catch if it\'s been\\n"',
     ['\t.string "Dice: "ciao" a tutti\\n"'],
     ('error', 'CHARMAP', None, 'junk at end of line')),
    (R104, '\t.string "You\'re a thief if you try to steal\\n"',
     ['\t.string "{PLAYR}, sei un ladro {PLAYER }\\n"'],
     ('error', 'CHARMAP', None, "unknown constant 'PLAYR'")),
    (None, None, None, ('error', 'CHARMAP', None, "whitespace before '}'")),
    (R104, '\t.string "You should throw POKé BALLS only at\\n"',
     ['\t.string "Perchè lanci le BALL?\\n"'],
     ('error', 'CHARMAP', None, 'U+0300')),
    (R104, '\t.string "But that\'s right, if TRAINERS lock eyes,\\n"',
     ['\t.string "Ma se due ALLENATORI\\n"', '\t.string "si guardano negli occhi,\\n"'],
     ('error', 'BOXLINES', 'Route104_Text_ImNotATrainer', '3rd line')),
    (R104, '\t.string "A word of advice!\\p"',
     ['\t.string "Un consiglio!\\l"'],
     ('error', 'BOXLINES', 'Route104_Text_TMsAreOneTimeUse', 'first break of a paragraph is \\l')),
    (R104, '\t.string "shopping.\\p"',
     ['\t.string "di piante.$"'],
     ('error', 'TERMINATION', 'Route104_Text_DontNeedThisTakeIt', 'in the middle of the block')),
    (R104, '\t.string "shopping. Where should I put them?$"',
     ['\t.string "di piante.$ Dove le metto?$"'],
     ('error', 'TERMINATION', 'Route104_Text_FlowerShopSellingSaplings', 'text after "$"')),
    (R104, '\t.string "saplings recently.\\p"',
     ['\t.string "piantine da poco, e ne ho comprate tante!\\p"'],
     ('error', 'ARROW', None, 'no room for the 8px')),
    (R104, '\tmsgbox Route104_Text_MayWeShouldRegister, MSGBOX_DEFAULT',
     ['\tmsgbox Route104_Text_MayWeShouldRegister, MSGBOX_SIGN'],
     ('error', 'STRUCTURE', None, 'MSGBOX_SIGN')),
    (R104, '\t.string "You\'re better than I expected!$"',
     ['\t.string "Accidenti! Sei molto più forte di quel che\\n"', '\t.string "pensavo!$"'],
     ('error', 'WIDTH', 'Route104_Text_MayDefeat', 'too wide for the battle box')),
    (R104, '\t.string "{PLAYER}{KUN}. I\'m not going to lose!$"',
     ['\t.string "{PLAYER}{KUN}, ma con {STR_VAR_2} non perdo!$"'],
     ('warning', 'TOKENS', 'Route104_Text_MayIntro', '{STR_VAR_2} is not used in the original')),
    (R104, '\t.string "MR. BRINEY\'S COTTAGE$"',
     ['\t.string "CASA DELL‘ANZIANO MARINO$"'],
     ('warning', 'STYLE', None, 'used as apostrophe')),
    (TRN, '\t.string "Why keep it a secret?\\n"',
     ["\t.string \"Perché tenerlo segreto? Sono l'esperto di POKéMON\\n\""],
     ('error', 'WIDTH', 'Route104_Text_IvanIntro', 'too wide for the field box')),
    (TRN, '\t.string "I thought I wasn\'t too bad, if I may\\n"',
     ['\t.string "Pensavo di non essere male,\\n"', '\t.string "se posso dirlo, ma a quanto pare\\n"'],
     ('error', 'BOXLINES', 'Route104_Text_IvanDefeat', '2-line battle box')),
    (MAUV, '\t.string "I just wanted to tell you that\\n"',
     ['\t.string "Volevo solo dirti che mio zio mi ha da poco\\n"'],
     ('error', 'WIDTH', 'MauvilleCity_Text_WallyPokenavCall', 'too wide for the pokenav box')),
    (MAUV, '\tpokenavcall MauvilleCity_Text_WallyPokenavCall',
     ['\tpokenavcall MauvilleCity_Text_WallyPokenavCall', '\t.string "ciao$"'],
     ('error', 'STRUCTURE', None, 'inserted where the original has none')),
    (JUAN, '\t.string "Hahaha, I merely jest!\\p"',
     ['\t.string "Ahahah, stavo solo scherzando, si capisce!\\p"',
      '\t.string "Del resto, un vero ALLENATORE non ha\\n"',
      '\t.string "bisogno di abiti eleganti per brillare:\\l"',
      '\t.string "gli basta il legame con i suoi POKéMON.\\p"',
      '\t.string "E tu, questo legame, lo hai dimostrato\\n"',
      '\t.string "in ogni singolo istante della lotta.\\p"'],
     ('error', 'LENGTH', 'SootopolisCity_Gym_1F_Text_JuanDefeat', 'gDisplayedStringBattle')),
    (MAUV, '\t.string "If I combine forces with RALTS,\\n"', [],
     ('error', 'TERMINATION', 'MauvilleCity_Text_WallyWeCanBeatAnyone', None)),
    # lose text of a multi battle (multi_2_vs_2 -> setmultitrainerbattle): battle box, 213px > 208px
    (SPACE, '\t.string "I\'m with our leader…$"',
     ['\t.string "Sto con il capo… e non mi pento di niente!$"'],
     ('error', 'WIDTH', 'MossdeepCity_SpaceCenter_Text_TabithaDefeat', 'too wide for the battle box')),
    # Battle Frontier move tutor description: fixed 12x6-tile window (96px, 3 rows)
    (LOUNGE7, '\t.string "half the user\'s\\n"',
     ['\t.string "metà dei PS massimi\\n"'],
     ('error', 'WIDTH', 'BattleFrontier_Lounge7_Text_SoftboiledDesc', 'tutor_desc window')),
    (R104, '\t.string "feared the sea, however stormy.$"',
     ['\t.string "temeva il mare in tempesta.\\p$"'],
     ('warning', 'STYLE', 'Route104_Text_BrineyLivesInSeasideCottage', '\\p$')),
]


def build(test_dir, orig):
    files = sorted(set(m[0] for m in MUTATIONS if m[0]))
    if os.path.isdir(os.path.join(test_dir, 'data')):
        shutil.rmtree(os.path.join(test_dir, 'data'))
    for rel in files:
        src = os.path.join(orig, rel)
        dst = os.path.join(test_dir, rel)
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        with open(src, encoding='utf-8') as f:
            lines = f.read().split('\n')
        for frel, old, new, _ in MUTATIONS:
            if frel != rel:
                continue
            idx = lines.index(old)          # ValueError if the vanilla file changed
            lines[idx:idx + 1] = new
        with open(dst, 'w', encoding='utf-8') as f:
            f.write('\n'.join(lines))
    # the last mutation removes one of two .string lines; remove the second as well
    # so that the whole block disappears (label without text)
    p = os.path.join(test_dir, MAUV)
    with open(p, encoding='utf-8') as f:
        lines = f.read().split('\n')
    lines.remove('\t.string "we can beat anyone!$"')
    lines.remove('\t.string "WALLY: I\'m not pushing it.\\p"')
    with open(p, 'w', encoding='utf-8') as f:
        f.write('\n'.join(lines))
    return files


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument('--dir', default='/home/user/work/textcheck-test')
    ap.add_argument('--orig', default='/home/user/pex-orig')
    ap.add_argument('--report', action='store_true', help='also print the human-readable report')
    args = ap.parse_args(argv)
    tc = _load_textcheck()
    os.makedirs(args.dir, exist_ok=True)
    files = build(args.dir, args.orig)
    buf = io.StringIO()
    with redirect_stdout(buf):
        rc = tc.main(['--root', args.dir, '--orig', args.orig, '--json'] + files)
    res = json.loads(buf.getvalue())
    issues = res['issues']
    ok = True
    print('%-4s %-8s %-12s %-45s %s' % ('res', 'sev', 'check', 'label', 'expected message'))
    for _, _, _, (sev, check, label, sub) in MUTATIONS:
        hit = [i for i in issues if i['severity'] == sev and i['check'] == check
               and (label is None or i['label'] == label)
               and (sub is None or sub in i['message'] or sub in (i.get('text') or ''))]
        ok = ok and bool(hit)
        where = ('%s:%s' % (hit[0]['file'], hit[0]['line'])) if hit else '-'
        print('%-4s %-8s %-12s %-45s %s  [%s]' % ('OK' if hit else 'MISS', sev, check, label or '', sub, where))
    print('\ntextcheck exit code: %d; summary: errors=%d warnings=%d %s' % (
        rc, res['summary']['errors'], res['summary']['warnings'], res['summary']['errors_by_check']))
    if args.report:
        print('\n--- human-readable report ---')
        tc.main(['--root', args.dir, '--orig', args.orig] + files)
    if rc != 1:
        ok = False
    print('SELFTEST: %s' % ('PASS' if ok else 'FAIL'))
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
