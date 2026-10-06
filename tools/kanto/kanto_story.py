#!/usr/bin/env python3
"""Kanto (Act 2) gameplay layer for Pokemon Multiverse (pokeemerald-expansion 1.17.1, Emerald build).

Writes the playable logic on top of the port_kanto.py stubs, following docs/storia/kanto_scene.json:
gyms (badges, TMs, gym guides, statues, Vermilion trash cans, Cinnabar quiz), story scenes K01-K31,
gating (quarantine, Snorlax, Saffron gates, gym doors, Route 22/23 badge checks, ...), elevators,
Pokemon Mansion switches, the Indigo Plateau League (E4 rooms + Champion Blu + Act 2 ending),
legendary static encounters, the Kanto "Volo Taxi" in every Kanto Pokemon Center and the visited-town flags.

All dialogue is an Italian placeholder "[TESTO: ...]" with ONE label per text: writers replace them.

Idempotent. Per map it removes the stub labels it replaces and (re)writes one block delimited by
"@ KANTO_V2 BEGIN" / "@ KANTO_V2 END" at the end of scripts.inc. Text labels inside the block that a
writer already filled (no "[TESTO:" any more) are kept verbatim when the block is regenerated.
Shared code: data/scripts/kanto_story.inc (included by data/event_scripts.s).

    python3 -I tools/kanto/kanto_story.py --tree /home/user/pex
"""
import argparse, json, os, re, struct

HERE = os.path.dirname(os.path.abspath(__file__))
BEGIN, END = '@ KANTO_V2 BEGIN (tools/kanto/kanto_story.py: regenerated, filled texts are kept)', '@ KANTO_V2 END'
TREE = '/home/user/pex'


# ------------------------------------------------------------------ helpers
def rd(p):
    with open(p, encoding='utf-8') as f:
        return f.read()


def wr(p, s):
    old = rd(p) if os.path.exists(p) else None
    if old != s:
        with open(p, 'w', encoding='utf-8') as f:
            f.write(s)
        return True
    return False


def wrap(desc, width=32):
    """'[TESTO: ...]' placeholder, word-wrapped for the message box (\\n then \\l)."""
    words = ('[TESTO: ' + ' '.join(p.strip() for p in desc.split('|')) + ']').split()
    lines, cur = [], ''
    for w in words:
        if cur and len(cur) + 1 + len(w) > width:
            lines.append(cur)
            cur = w
        else:
            cur = (cur + ' ' + w).strip()
    lines.append(cur)
    out = lines[0]
    for i, ln in enumerate(lines[1:]):
        out += ('\\n' if i == 0 else '\\l') + ln
    return out


class Layout:
    def __init__(self, mp):
        m = json.load(open(os.path.join(TREE, 'data/maps', mp, 'map.json')))
        L = [l for l in json.load(open(os.path.join(TREE, 'data/layouts/layouts.json')))['layouts']
             if l.get('id') == m['layout']][0]
        self.w, self.h = L['width'], L['height']
        self.b = open(os.path.join(TREE, L['blockdata_filepath']), 'rb').read()

    def tile(self, x, y):
        v = struct.unpack_from('<H', self.b, 2 * (y * self.w + x))[0]
        return (v >> 10) & 3, v >> 12  # collision, elevation


class MapEdit:
    def __init__(self, ed, mp):
        self.ed, self.mp = ed, mp
        self.sp = os.path.join(TREE, 'data/maps', mp, 'scripts.inc')
        self.jp = os.path.join(TREE, 'data/maps', mp, 'map.json')
        self.s = rd(self.sp)
        self.j = json.loads(rd(self.jp))
        self.block = []
        self.mapscripts = []  # (type, label)
        self.keep_texts = {}
        if BEGIN in self.s:
            old = self.s[self.s.index(BEGIN):self.s.index(END) + len(END)]
            for m in re.finditer(r'(?m)^(\w+):\n((?:\t\.string [^\n]*\n)+)', old):
                if '[TESTO:' not in m.group(2):
                    self.keep_texts[m.group(1)] = m.group(2)
            self.s = self.s.replace(old + '\n', '').replace(old, '')
            mm = re.search(r'(?m)^%s_MapScripts::\n(?:\t[^\n]*\n)+' % re.escape(mp), old)
            if mm:  # older layout: the MapScripts label lived in the block -> put it back in the stub
                hdr = re.search(r'(?m)^(?!@)', self.s)
                self.s = self.s[:hdr.start()] + mm.group(0) + '\n' + self.s[hdr.start():]
        self.ms_label = mp + '_MapScripts'
        self.layout = None

    # --- stub surgery
    def kill(self, label, text=True):
        """Remove the stub definition of a script label (and its stub text label)."""
        self.s = re.sub(r'(?m)^%s::\n(?:\t[^\n]*\n)+\n?' % re.escape(label), '', self.s)
        if text:
            tl = label.replace('_EventScript_', '_Text_')
            self.s = re.sub(r'(?m)^%s:\n(?:\t\.string [^\n]*\n)+\n?' % re.escape(tl), '', self.s)

    def stub_has(self, label):
        return re.search(r'(?m)^%s::?\n' % re.escape(label), self.s) is not None

    def obj(self, idx):
        return self.j['object_events'][idx - 1]

    def obj_label(self, idx):
        return self.obj(idx)['script']

    # --- block content
    def add(self, text):
        self.block.append(text.rstrip('\n') + '\n')

    def text(self, label, desc):
        if label in self.keep_texts:
            self.block.append('%s:\n%s' % (label, self.keep_texts[label]))
        else:
            self.block.append('%s:\n\t.string "%s$"\n' % (label, wrap(desc)))
        return label

    def mapscript(self, kind, label):
        self.mapscripts.append((kind, label))

    # --- map.json edits
    def set_obj(self, idx, **kw):
        o = self.obj(idx)
        for k, v in kw.items():
            o[k] = v

    def add_obj(self, local_id, gfx, x, y, script, flag='0', movement='MOVEMENT_TYPE_FACE_DOWN'):
        if self.layout is None:
            self.layout = Layout(self.mp)
        col, el = self.layout.tile(x, y)
        assert col == 0, '%s: (%d,%d) is not walkable for %s' % (self.mp, x, y, local_id)
        for o in self.j['object_events']:
            if o.get('local_id') != local_id and (o['x'], o['y']) == (x, y) and o.get('flag', '0') in ('0', flag):
                raise SystemExit('%s: (%d,%d) already has object %s' % (self.mp, x, y, o.get('script')))
        new = {'local_id': local_id, 'type': 'object', 'graphics_id': gfx, 'x': x, 'y': y, 'elevation': el or 3,
               'movement_type': movement, 'movement_range_x': 0, 'movement_range_y': 0,
               'trainer_type': 'TRAINER_TYPE_NONE', 'trainer_sight_or_berry_tree_id': '0',
               'script': script, 'flag': flag}
        for i, o in enumerate(self.j['object_events']):
            if o.get('local_id') == local_id:
                self.j['object_events'][i] = new
                return local_id
        self.j['object_events'].append(new)
        return local_id

    def enable_coords(self, script):
        """Re-enable the port's disabled coord triggers that run <script> (VAR_TEMP_F == 0 always holds)."""
        n = 0
        for c in self.j.get('coord_events', []):
            if c.get('script') == script:
                c['var'], c['var_value'] = 'VAR_TEMP_F', '0'
                n += 1
        assert n, (self.mp, script)

    def add_coord(self, x, y, script):
        ce = self.j.setdefault('coord_events', [])
        for c in ce:
            if (c['x'], c['y']) == (x, y) and c['script'] == script:
                return
        ce.append({'type': 'trigger', 'x': x, 'y': y, 'elevation': 0, 'var': 'VAR_TEMP_F', 'var_value': '0',
                   'script': script})

    # --- write
    def save(self):
        s = self.s
        if self.mapscripts:
            m = re.search(r'(?m)^%s::\n((?:\t[^\n]*\n)+)' % re.escape(self.ms_label), s)
            assert m, self.mp
            mine = {lab for _, lab in self.mapscripts}
            entries = [ln for ln in m.group(1).split('\n') if ln.strip().startswith('map_script ')
                       and ln.split('@')[0].split(',')[-1].strip() not in mine]
            kinds = {re.match(r'\s*map_script (\w+),', e).group(1) for e in entries}
            for k, lab in self.mapscripts:
                assert k not in kinds, (self.mp, k)
                entries.append('\tmap_script %s, %s @ KANTO_V2' % (k, lab))
            entries = [e if e.rstrip().endswith('KANTO_V2') or '@' in e else e for e in entries]
            s = s[:m.start()] + '%s::\n%s\n\t.byte 0\n' % (self.ms_label, '\n'.join(entries)) + s[m.end():]
        s = s.rstrip('\n') + '\n\n' + BEGIN + '\n' + '\n'.join(self.block) + END + '\n'
        ch = wr(self.sp, s)
        ch |= wr(self.jp, json.dumps(self.j, indent=2, ensure_ascii=False) + '\n')
        return ch


class Editor:
    def __init__(self):
        self.maps = {}
        self.inv = {m['map']: m for m in json.load(open(os.path.join(HERE, '..', '..', 'docs/kanto/mappe.json')))['maps']}

    def __call__(self, mp):
        if mp not in self.maps:
            self.maps[mp] = MapEdit(self, mp)
        return self.maps[mp]

    def orig(self, mp, label, prefix_from, prefix_to, subs=()):
        """Lines of an original FRLG label (until the first blank line), renamed."""
        t = rd(os.path.join(TREE, 'data/maps', mp, 'scripts_frlg_orig.inc'))
        m = re.search(r'(?m)^%s::\n((?:[^\n]+\n)+)' % re.escape(label), t)
        assert m, (mp, label)
        body = m.group(0)
        body = re.sub(r'\b%s(?!Frlg_)' % re.escape(prefix_from), prefix_to, body)
        for a, b in subs:
            body = body.replace(a, b)
        return body


# ------------------------------------------------------------------ data
GYMS = [
    # map, leader trainer (frlg), short, badge, tm, tm flag, badge name, extra on win, gate
    ('VermilionCity_Gym_Frlg', 'TRAINER_LEADER_LT_SURGE', 'LtSurge', 3, 'ITEM_TM24', 'FLAG_KANTO_GOT_TM_SURGE',
     'Medaglia Tuono', ['setflag FLAG_KANTO_HIDE_VERMILION_QUARANTINE', 'setflag FLAG_KANTO_HIDE_VERMILION_ARIANNA',
                        'setvar VAR_KANTO_STORY, 2'], None),
    ('CeruleanCity_Gym_Frlg', 'TRAINER_LEADER_MISTY', 'Misty', 2, 'ITEM_TM18', 'FLAG_KANTO_GOT_TM_MISTY',
     'Medaglia Cascata', [], None),
    ('PewterCity_Gym_Frlg', 'TRAINER_LEADER_BROCK', 'Brock', 1, 'ITEM_TM37', 'FLAG_KANTO_GOT_TM_BROCK',
     'Medaglia Sasso', [], None),
    ('CeladonCity_Gym_Frlg', 'TRAINER_LEADER_ERIKA', 'Erika', 4, 'ITEM_TM19', 'FLAG_KANTO_GOT_TM_ERIKA',
     'Medaglia Arcobaleno', [], None),
    ('FuchsiaCity_Gym_Frlg', 'TRAINER_LEADER_KOGA', 'Koga', 5, 'ITEM_TM06', 'FLAG_KANTO_GOT_TM_KOGA',
     'Medaglia Anima', ['setflag FLAG_KANTO_STORY_SAFFRON_OPEN'], None),
    ('SaffronCity_Gym_Frlg', 'TRAINER_LEADER_SABRINA', 'Sabrina', 6, 'ITEM_TM29', 'FLAG_KANTO_GOT_TM_SABRINA',
     'Medaglia Palude', [], 'FLAG_KANTO_STORY_SILPH_FREED'),
    ('CinnabarIsland_Gym_Frlg', 'TRAINER_LEADER_BLAINE', 'Blaine', 7, 'ITEM_TM38', 'FLAG_KANTO_GOT_TM_BLAINE',
     'Medaglia Vulcano', [], None),
    ('ViridianCity_Gym_Frlg', 'TRAINER_LEADER_GIOVANNI', 'Giovanni', 8, 'ITEM_TM26', 'FLAG_KANTO_GOT_TM_GIOVANNI',
     'Medaglia Terra', ['setflag FLAG_KANTO_HIDE_CERULEAN_CAVE_GUARD'], None),
]

TOWNS = [  # flag suffix, town map, PC map (None = no PC), Italian name
    ('PALLET', 'PalletTown_Frlg', None, 'Biancavilla'),
    ('VIRIDIAN', 'ViridianCity_Frlg', 'ViridianCity_PokemonCenter_1F_Frlg', 'Smeraldopoli'),
    ('PEWTER', 'PewterCity_Frlg', 'PewterCity_PokemonCenter_1F_Frlg', 'Plumbeopoli'),
    ('CERULEAN', 'CeruleanCity_Frlg', 'CeruleanCity_PokemonCenter_1F_Frlg', 'Celestopoli'),
    ('VERMILION', 'VermilionCity_Frlg', 'VermilionCity_PokemonCenter_1F_Frlg', 'Aranciopoli'),
    ('LAVENDER', 'LavenderTown_Frlg', 'LavenderTown_PokemonCenter_1F_Frlg', 'Lavandonia'),
    ('CELADON', 'CeladonCity_Frlg', 'CeladonCity_PokemonCenter_1F_Frlg', 'Azzurropoli'),
    ('FUCHSIA', 'FuchsiaCity_Frlg', 'FuchsiaCity_PokemonCenter_1F_Frlg', 'Fucsiapoli'),
    ('SAFFRON', 'SaffronCity_Frlg', 'SaffronCity_PokemonCenter_1F_Frlg', 'Zafferanopoli'),
    ('CINNABAR', 'CinnabarIsland_Frlg', 'CinnabarIsland_PokemonCenter_1F_Frlg', 'Isola Cannella'),
    ('INDIGO', 'IndigoPlateau_Exterior_Frlg', 'IndigoPlateau_PokemonCenter_1F_Frlg', 'Altopiano Blu'),
]
TAXI_PCS = [t[2] for t in TOWNS if t[2]] + ['Route4_PokemonCenter_1F_Frlg', 'Route10_PokemonCenter_1F_Frlg']


def map_const(mp):
    return json.load(open(os.path.join(TREE, 'data/maps', mp, 'map.json')))['id']


def door_pos(town, target):
    """Tile in front of the door of <town> that warps into <target> (x, y+1)."""
    m = json.load(open(os.path.join(TREE, 'data/maps', town, 'map.json')))
    tid = map_const(target)
    for w in m['warp_events']:
        if w['dest_map'] == tid:
            return w['x'], w['y'] + 1
    raise SystemExit('no door %s -> %s' % (town, target))


# ------------------------------------------------------------------ shared file
def shared(ed):
    out = ['@ KANTO_V2: shared Kanto (Act 2) gameplay scripts. Generated by tools/kanto/kanto_story.py:',
           '@ do not edit by hand, run the tool. Texts here are placeholders for the writers ("[TESTO: ...]").', '']
    T = []

    def tx(label, desc):
        T.append('%s:\n\t.string "%s$"\n' % (label, wrap(desc)))
        return label

    # --- init on the first crossing (called by KantoTravel_EventScript_LilycoveOffer)
    out.append('''KantoStory_EventScript_Init::
	goto_if_set FLAG_KANTO_STORY_INIT_DONE, Common_EventScript_NopReturn
	setflag FLAG_KANTO_HIDE_MTMOON_SCENE
	setflag FLAG_KANTO_HIDE_OAKLAB_ARIANNA
	setflag FLAG_KANTO_HIDE_CREDITS_ARIANNA
	setflag FLAG_KANTO_HIDE_FUJI_HOUSE_EXILES
	setflag FLAG_KANTO_HIDE_MEWTWO
	setflag FLAG_KANTO_HIDE_RIVAL_IN_LAB
	setflag FLAG_KANTO_HIDE_BILL_CLEFAIRY
	clearflag FLAG_KANTO_HIDE_BILL_HUMAN_SEA_COTTAGE
	clearflag FLAG_KANTO_HIDE_OAK_IN_HIS_LAB
	clearflag FLAG_KANTO_HIDE_LIFT_KEY
	setflag FLAG_KANTO_STORY_CROSSED
	setflag FLAG_KANTO_STORY_INIT_DONE
	return

@ VAR_RESULT = number of Kanto badges (0-8)
KantoStory_EventScript_CountBadges::
	setvar VAR_RESULT, 0
''' + ''.join('\tcall_if_set FLAG_KANTO_BADGE%02d, KantoStory_EventScript_AddOne\n' % i for i in range(1, 9)) + '''	return

KantoStory_EventScript_AddOne::
	addvar VAR_RESULT, 1
	return

@ push-back helpers for gates (lockall already done by the caller)
KantoStory_EventScript_PushDown::
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkDown
	waitmovement 0
	releaseall
	end

KantoStory_EventScript_PushUp::
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkUp
	waitmovement 0
	releaseall
	end

KantoStory_EventScript_PushLeft::
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkLeft
	waitmovement 0
	releaseall
	end

KantoStory_EventScript_PushRight::
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkRight
	waitmovement 0
	releaseall
	end

@ shared wild legendary / obstacle battle: setwildbattle done by the caller, VAR_0x8004 = species (for the cry)
KantoStory_EventScript_StaticBattle::
	waitse
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	dowildbattle
	release
	end
''')
    # --- Volo Taxi
    lines = ['KantoTaxi_EventScript_Taxi::', '\tlock', '\tfaceplayer',
             '\tmsgbox %s, MSGBOX_DEFAULT' % tx('KantoTaxi_Text_Intro', 'Volo Taxi: dove ti porto? | (solo citta gia visitate)')]
    for i, (suf, town, pc, name) in enumerate(TOWNS):
        lines.append('\tcall_if_set FLAG_KANTO_VISITED_%s, KantoTaxi_EventScript_Push%s' % (suf, suf.capitalize()))
    lines += ['\tdynmultipush KantoTaxi_Text_Cancel, 127',
              '\tdynmultistack 0, 0, FALSE, 6, FALSE, 0, DYN_MULTICHOICE_CB_NONE', '\tswitch VAR_RESULT']
    for i, (suf, town, pc, name) in enumerate(TOWNS):
        lines.append('\tcase %d, KantoTaxi_EventScript_To%s' % (i, suf.capitalize()))
    lines += ['\tmsgbox %s, MSGBOX_DEFAULT' % tx('KantoTaxi_Text_Bye', 'Volo Taxi: alla prossima!'), '\trelease', '\tend', '']
    T.append('KantoTaxi_Text_Cancel:\n\t.string "Annulla$"\n')
    for i, (suf, town, pc, name) in enumerate(TOWNS):
        tgt = pc or 'PalletTown_ProfessorOaksLab_Frlg'
        x, y = door_pos(town, tgt)
        cap = suf.capitalize()
        lines += ['KantoTaxi_EventScript_Push%s::' % cap, '\tdynmultipush KantoTaxi_Text_Town%s, %d' % (cap, i), '\treturn', '',
                  'KantoTaxi_EventScript_To%s::' % cap,
                  '\tmsgbox KantoTaxi_Text_Go, MSGBOX_DEFAULT', '\tclosemessage', '\tplayse SE_M_FLY', '\tfadescreen FADE_TO_BLACK',
                  '\twarp %s, %d, %d' % (map_const(town), x, y), '\twaitstate', '\trelease', '\tend', '']
        T.append('KantoTaxi_Text_Town%s:\n\t.string "%s$"\n' % (cap, name))
    tx('KantoTaxi_Text_Go', 'Volo Taxi: si parte!')
    out.append('\n'.join(lines))

    # --- elevators: VAR_0x8004 = floor list id
    def elevator(name, floors, need_item=None):
        lab = 'KantoElevator_EventScript_%s' % name
        L = [lab + '::', '\tlockall']
        if need_item:
            L += ['\tcheckitem %s' % need_item, '\tgoto_if_eq VAR_RESULT, FALSE, %sNeedKey' % lab]
        L += ['\tmessage %s' % tx('KantoElevator_Text_%sWhichFloor' % name, 'Ascensore: a che piano vuoi andare?'),
              '\twaitmessage']
        for i, (fl, mp, x, y) in enumerate(floors):
            L.append('\tdynmultipush KantoElevator_Text_%s%d, %d' % (name, i, i))
            T.append('KantoElevator_Text_%s%d:\n\t.string "%s$"\n' % (name, i, fl))
        L += ['\tdynmultipush KantoTaxi_Text_Cancel, 127',
              '\tdynmultistack 0, 0, FALSE, 6, FALSE, 0, DYN_MULTICHOICE_CB_NONE', '\tclosemessage', '\tswitch VAR_RESULT']
        for i in range(len(floors)):
            L.append('\tcase %d, %sTo%d' % (i, lab, i))
        L += ['\treleaseall', '\tend', '']
        for i, (fl, mp, x, y) in enumerate(floors):
            L += ['%sTo%d::' % (lab, i), '\tsetdynamicwarp %s, %d, %d' % (map_const(mp), x, y),
                  '\tplayse SE_ELEVATOR', '\twaitse', '\tmsgbox KantoElevator_Text_Arrived, MSGBOX_DEFAULT', '\treleaseall', '\tend', '']
        if need_item:
            L += ['%sNeedKey::' % lab, '\tmsgbox %s, MSGBOX_DEFAULT' % tx('KantoElevator_Text_%sNeedKey' % name,
                                                                          'Ascensore: serve una chiave.'),
                  '\treleaseall', '\tend', '']
        out.append('\n'.join(L))
    tx('KantoElevator_Text_Arrived', 'Ding! Le porte si aprono.')
    elevator('RocketHideout', [('P1 Sotterraneo', 'RocketHideout_B1F_Frlg', 24, 25),
                               ('P2 Sotterraneo', 'RocketHideout_B2F_Frlg', 28, 16),
                               ('P4 Sotterraneo', 'RocketHideout_B4F_Frlg', 20, 23)], 'ITEM_LIFT_KEY')
    elevator('Silph', [('%dP' % n, 'SilphCo_%dF_Frlg' % n, x, 3) for n, x in
                       [(1, 22), (2, 22), (3, 22), (4, 22), (5, 22), (6, 20), (7, 23), (8, 22), (9, 24), (10, 13), (11, 13)]])
    elevator('CeladonDept', [('%dP' % n, 'CeladonCity_DepartmentStore_%dF_Frlg' % n, 6, 1) for n in range(1, 6)])

    # --- Pokemon Mansion secret switch (copy of data/scripts/pokemon_mansion.inc with a saved Kanto flag)
    src = rd(os.path.join(TREE, 'data/scripts/pokemon_mansion.inc'))
    src = src.replace('FLAG_POKEMON_MANSION_SWITCH_STATE', 'FLAG_KANTO_MANSION_SWITCH')
    src = re.sub(r'\bPokemonMansion_(EventScript|Text)_', r'KantoMansion_\1_', src)
    src = re.sub(r'(?m)^(KantoMansion_Text_\w+)::\n(?:\t\.string [^\n]*\n)+', lambda m: '', src)
    out.append('@ Pokemon Mansion switches: copy of data/scripts/pokemon_mansion.inc, saved flag FLAG_KANTO_MANSION_SWITCH\n' + src)
    for lab in sorted(set(re.findall(r'KantoMansion_Text_\w+', src))):
        tx(lab, 'Villa Pokemon: interruttore | %s' % lab.split('_')[-1])

    # --- League door helpers: copy of data/scripts/pokemon_league.inc (built only in FRLG)
    src = rd(os.path.join(TREE, 'data/scripts/pokemon_league.inc'))
    src = src.replace('PokemonLeague_EventScript_', 'KantoLeague_EventScript_').replace('PokemonLeague_Movement_', 'KantoLeague_Movement_')
    src = src.replace('Text_VoiceRangOutDontRunAway', 'KantoLeague_Text_DontRunAway')
    src = re.sub(r'(?m)^KantoLeague_Text_DontRunAway::\n(?:\t\.string [^\n]*\n)+', '', src)
    out.append('@ League doors: copy of data/scripts/pokemon_league.inc\n' + src)
    tx('KantoLeague_Text_DontRunAway', 'Una voce: non scappare!')
    # --- League: lobby reset + ending helpers
    out.append('''@ Indigo Plateau: E4 progress is reset every time the player is in the lobby (after a loss too)
KantoLeague_EventScript_Reset::
	setvar VAR_KANTO_LEAGUE, 0
	clearflag FLAG_KANTO_DEFEATED_LORELEI
	clearflag FLAG_KANTO_DEFEATED_BRUNO
	clearflag FLAG_KANTO_DEFEATED_AGATHA
	clearflag FLAG_KANTO_DEFEATED_LANCE
	return
''')
    return '\n'.join(out) + '\n' + '\n'.join(T)


# ------------------------------------------------------------------ gyms
def gyms(ed):
    for mp, leader, short, badge, tm, tmflag, bname, extra, gate in GYMS:
        e = ed(mp)
        inv = ed.inv[mp]
        lab = [o['script'] for o in inv['objects'] if o.get('orig_trainer') == leader][0]
        kid = 'TRAINER_KANTO_' + leader[len('TRAINER_'):]
        bflag = 'FLAG_KANTO_BADGE%02d' % badge
        gym_tr = sorted({'TRAINER_KANTO_' + o['orig_trainer'][8:] for o in inv['objects']
                         if o.get('orig_trainer') and o['orig_trainer'] != leader})
        P = mp + '_EventScript_' + short
        X = mp + '_Text_' + short
        e.kill(lab)
        body = [lab + '::', '\tlock', '\tfaceplayer', '\tgoto_if_set %s, %sAfterBadge' % (bflag, P)]
        if gate:
            body.append('\tgoto_if_unset %s, %sNotReady' % (gate, P))
        body += ['\tmsgbox %sIntro, MSGBOX_DEFAULT' % X,
                 '\ttrainerbattle_no_intro %s, %sDefeat' % (kid, X),
                 '\tmessage %s_Text_ReceivedBadge' % mp, '\twaitmessage', '\tcall Common_EventScript_PlayGymBadgeFanfare',
                 '\tsetflag %s' % bflag]
        body += ['\t' + x for x in extra]
        body += ['\tsettrainerflag %s' % t for t in gym_tr]
        body += ['\tmsgbox %sExplainBadge, MSGBOX_DEFAULT' % X,
                 '%sAfterBadge::' % P,
                 '\tgoto_if_set %s, %sPostBattle' % (tmflag, P),
                 '\tgiveitem %s' % tm, '\tgoto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull',
                 '\tsetflag %s' % tmflag, '\tmsgbox %sExplainTM, MSGBOX_DEFAULT' % X, '\trelease', '\tend', '',
                 '%sPostBattle::' % P, '\tmsgbox %sPostBattle, MSGBOX_DEFAULT' % X, '\trelease', '\tend', '']
        if gate:
            body += ['%sNotReady::' % P, '\tmsgbox %sNotReady, MSGBOX_DEFAULT' % X, '\trelease', '\tend', '']
        e.add('\n'.join(body))
        who = short if short != 'LtSurge' else 'Lt. Surge'
        e.text(X + 'Intro', '%s: sfida, prima della lotta' % who)
        e.text(X + 'Defeat', '%s: alla sconfitta' % who)
        e.text(mp + '_Text_ReceivedBadge', '{PLAYER} riceve la | %s!' % bname)
        e.text(X + 'ExplainBadge', '%s: spiega la %s' % (who, bname))
        e.text(X + 'ExplainTM', '%s: spiega la MT (%s)' % (who, tm[5:]))
        e.text(X + 'PostBattle', '%s: dopo la lotta' % who)
        if gate:
            e.text(X + 'NotReady', '%s: rifiuta la lotta | (la Silph e ancora occupata)' % who)
        # gym guide + statues
        evlabs = {o.get('script', '') for o in e.j['object_events'] + e.j.get('bg_events', [])}
        for gl in sorted(l for l in evlabs if l == mp + '_EventScript_GymGuy'):
            e.kill(gl)
            t = gl.replace('_EventScript_', '_Text_')
            e.add('''%s::
	lock
	faceplayer
	goto_if_set %s, %sPostVictory
	msgbox %s, MSGBOX_DEFAULT
	release
	end

%sPostVictory::
	msgbox %sPostVictory, MSGBOX_DEFAULT
	release
	end
''' % (gl, bflag, gl, t, gl, t))
            e.text(t, 'Guida della Palestra: consigli | contro il tipo del Capopalestra')
            e.text(t + 'PostVictory', 'Guida della Palestra: complimenti | dopo la Medaglia')
        for sl in sorted(l for l in evlabs if l.startswith(mp + '_EventScript_GymStatue')):
            e.kill(sl)
            t = sl.replace('_EventScript_', '_Text_')
            e.add('''%s::
	goto_if_set %s, %sCertified
	msgbox %s, MSGBOX_SIGN
	end

%sCertified::
	msgbox %sCertified, MSGBOX_SIGN
	end
''' % (sl, bflag, sl, t, sl, t))
            e.text(t, 'Statua della Palestra: | nome del Capopalestra')
            e.text(t + 'Certified', 'Statua: Allenatori premiati | ... {PLAYER}')

    # --- Vermilion trash cans (port of the FRLG logic; special SetVermilionTrashCans is shared C code)
    mp = 'VermilionCity_Gym_Frlg'
    e = ed(mp)
    subs = [('FLAG_FOUND_BOTH_VERMILION_GYM_SWITCHES', 'FLAG_KANTO_VERMILION_GYM_SWITCHES'),
            ('FOUND_FIRST_SWITCH', 'FLAG_TEMP_1'), ('SWITCH1_ID', 'VAR_0x8004'), ('SWITCH2_ID', 'VAR_0x8005'),
            ('TRASH_CAN_ID', 'VAR_0x8008')]
    pf, pt = 'VermilionCity_Gym_', 'VermilionCity_Gym_Frlg_'
    labs = ['OnLoad', 'OnTransition', 'EventScript_InitTrashCans', 'EventScript_SetOneBeamOff', 'EventScript_SetBeamsOff',
            'EventScript_SetBeamsOn', 'EventScript_TrashCan', 'EventScript_FoundSwitchOne', 'EventScript_TrySwitchTwo',
            'EventScript_FoundSwitchTwo', 'EventScript_LocksAlreadyOpen'] + ['EventScript_TrashCan%d' % i for i in range(1, 16)]
    for l in labs:
        if l.startswith('EventScript_TrashCan') and l != 'EventScript_TrashCan':
            e.kill(pt + l)
        e.add(ed.orig(mp, pf + l, pf, pt, subs))
    e.mapscript('MAP_SCRIPT_ON_LOAD', pt + 'OnLoad')
    e.mapscript('MAP_SCRIPT_ON_TRANSITION', pt + 'OnTransition')
    for t, d in [('NopeOnlyTrashHere', 'Cestino: solo spazzatura.'),
                 ('SwitchUnderTrashFirstLockOpened', 'Cestino: un interruttore! | Prima serratura aperta.'),
                 ('SecondLockOpened', 'Cestino: secondo interruttore! | Le barriere si spengono.'),
                 ('OnlyTrashLocksWereReset', 'Cestino: solo spazzatura... | Le serrature si sono richiuse!')]:
        e.text(pt + 'Text_' + t, d)

    # --- Cinnabar quiz (6 machines, 6 saved flags)
    mp = 'CinnabarIsland_Gym_Frlg'
    e = ed(mp)
    pf, pt = 'CinnabarIsland_Gym_', 'CinnabarIsland_Gym_Frlg_'
    answers = {1: 'YES', 2: 'NO', 3: 'NO', 4: 'NO', 5: 'YES', 6: 'NO'}
    quiz_tr = {1: 'Quinn', 2: 'Avery', 3: 'Ramon', 4: 'Derek', 5: 'Dusty', 6: 'Zac'}
    tr_const = {o['script'].split('_EventScript_')[1]: 'TRAINER_KANTO_' + o['orig_trainer'][8:]
                for o in ed.inv[mp]['objects'] if o.get('orig_trainer')}
    on_load = [pt + 'OnLoad::', '\tgoto_if_set FLAG_KANTO_BADGE07, %sOnLoadOpenAll' % pt]
    on_load += ['\tcall_if_set FLAG_KANTO_CINNABAR_GYM_QUIZ_%d, %sEventScript_OpenDoor%d' % (n, pt, n) for n in range(1, 7)]
    on_load += ['\tend', '', pt + 'OnLoadOpenAll::'] + ['\tcall %sEventScript_OpenDoor%d' % (pt, n) for n in range(1, 7)] + ['\tend', '']
    e.add('\n'.join(on_load))
    e.mapscript('MAP_SCRIPT_ON_LOAD', pt + 'OnLoad')
    for n in range(1, 7):
        e.add(ed.orig(mp, pf + 'EventScript_OpenDoor%d' % n, pf, pt))
        left = '%sEventScript_%s%dLeft' % (pt, 'Quz' if n == 1 else 'Quiz', n)
        right = '%sEventScript_%s%dRight' % (pt, 'Quz' if n == 1 else 'Quiz', n)
        e.kill(left)
        e.kill(right)
        Q = '%sEventScript_Quiz%d' % (pt, n)
        tr = quiz_tr[n]
        tc = tr_const[tr]
        e.kill('%sEventScript_%s' % (pt, tr), text=False)
        e.add('''%(left)s::
%(right)s::
	lockall
	goto_if_set FLAG_KANTO_CINNABAR_GYM_QUIZ_%(n)d, %(pt)sEventScript_QuizDoorOpen
	msgbox %(pt)sText_QuizRules, MSGBOX_DEFAULT
	msgbox %(pt)sText_QuizQuestion%(n)d, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, %(ans)s, %(Q)sCorrect
	msgbox %(pt)sText_QuizWrong, MSGBOX_DEFAULT
	goto_if_not_defeated %(tc)s, %(Q)sBattle
	releaseall
	end

%(Q)sCorrect::
	playfanfare MUS_LEVEL_UP
	waitfanfare
	msgbox %(pt)sText_QuizCorrect, MSGBOX_DEFAULT
	call %(Q)sComplete
	releaseall
	end

%(Q)sBattle::
	trainerbattle_no_intro %(tc)s, %(pt)sText_%(tr)sDefeat
	call %(Q)sComplete
	releaseall
	end

%(Q)sComplete::
	playse SE_UNLOCK
	waitse
	call %(pt)sEventScript_OpenDoor%(n)d
	special DrawWholeMapView
	setflag FLAG_KANTO_CINNABAR_GYM_QUIZ_%(n)d
	return

%(pt)sEventScript_%(tr)s::
	trainerbattle_single %(tc)s, %(pt)sText_%(tr)sIntro, %(pt)sText_%(tr)sDefeat, %(Q)sDefeated
	msgbox %(pt)sText_%(tr)sPostBattle, MSGBOX_AUTOCLOSE
	end

%(Q)sDefeated::
	call_if_unset FLAG_KANTO_CINNABAR_GYM_QUIZ_%(n)d, %(Q)sComplete
	release
	end
''' % dict(left=left, right=right, n=n, pt=pt, ans=answers[n], Q=Q, tc=tc, tr=tr))
        e.text('%sText_QuizQuestion%d' % (pt, n), 'Quiz %d: domanda (risposta giusta: %s)' % (n, 'SI' if answers[n] == 'YES' else 'NO'))
    e.add('%sEventScript_QuizDoorOpen::\n\treleaseall\n\tend\n' % pt)
    e.text(pt + 'Text_QuizRules', 'Quiz Pokemon: rispondi giusto | e la porta si apre!')
    e.text(pt + 'Text_QuizCorrect', 'Esatto! Prosegui pure.')
    e.text(pt + 'Text_QuizWrong', 'Sbagliato! Un Allenatore | ti sfida.')


# ------------------------------------------------------------------ story scenes and gating
def story(ed):
    # ---- K02 Vermilion: arrival scene + quarantine (K03 lifts it)
    e = ed('VermilionCity_Frlg')
    ari = e.add_obj('LOCALID_KANTO_VERMILION_ARIANNA', 'OBJ_EVENT_GFX_MAY_NORMAL', 24, 30,
                    'VermilionCity_Frlg_EventScript_Arianna', 'FLAG_KANTO_HIDE_VERMILION_ARIANNA', 'MOVEMENT_TYPE_FACE_DOWN')
    e.add_obj('LOCALID_KANTO_VERMILION_POLICE_N', 'OBJ_EVENT_GFX_POLICEMAN', 24, 1,
              'VermilionCity_Frlg_EventScript_Quarantine', 'FLAG_KANTO_HIDE_VERMILION_QUARANTINE')
    e.add_obj('LOCALID_KANTO_VERMILION_POLICE_E', 'OBJ_EVENT_GFX_POLICEMAN', 46, 19,
              'VermilionCity_Frlg_EventScript_Quarantine', 'FLAG_KANTO_HIDE_VERMILION_QUARANTINE', 'MOVEMENT_TYPE_FACE_LEFT')
    lay = Layout('VermilionCity_Frlg')
    for x in (22, 23, 24):
        e.add_coord(x, 2, 'VermilionCity_Frlg_EventScript_QuarantineNorth')
    for y in range(lay.h):
        if lay.tile(45, y)[0] == 0:
            e.add_coord(45, y, 'VermilionCity_Frlg_EventScript_QuarantineEast')
    e.add('''VermilionCity_Frlg_OnFrame::
	map_script_2 VAR_KANTO_STORY, 0, VermilionCity_Frlg_EventScript_ArrivalScene
	.2byte 0

VermilionCity_Frlg_EventScript_ArrivalScene::
	lockall
	applymovement %(ari)s, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox VermilionCity_Frlg_Text_ArrivalArianna, MSGBOX_DEFAULT
	msgbox VermilionCity_Frlg_Text_ArrivalQuarantine, MSGBOX_DEFAULT
	setvar VAR_KANTO_STORY, 1
	releaseall
	end

VermilionCity_Frlg_EventScript_Arianna::
	msgbox VermilionCity_Frlg_Text_Arianna, MSGBOX_NPC
	end

VermilionCity_Frlg_EventScript_Quarantine::
	msgbox VermilionCity_Frlg_Text_Quarantine, MSGBOX_NPC
	end

VermilionCity_Frlg_EventScript_QuarantineNorth::
	goto_if_set FLAG_KANTO_HIDE_VERMILION_QUARANTINE, Common_EventScript_NopReturn
	lockall
	msgbox VermilionCity_Frlg_Text_Quarantine, MSGBOX_DEFAULT
	goto KantoStory_EventScript_PushDown

VermilionCity_Frlg_EventScript_QuarantineEast::
	goto_if_set FLAG_KANTO_HIDE_VERMILION_QUARANTINE, Common_EventScript_NopReturn
	lockall
	msgbox VermilionCity_Frlg_Text_Quarantine, MSGBOX_DEFAULT
	goto KantoStory_EventScript_PushLeft
''' % dict(ari=ari))
    e.mapscript('MAP_SCRIPT_ON_FRAME_TABLE', 'VermilionCity_Frlg_OnFrame')
    e.text('VermilionCity_Frlg_Text_ArrivalArianna', 'K02 Arianna: siamo a Kanto! | (sbarco ad Aranciopoli)')
    e.text('VermilionCity_Frlg_Text_ArrivalQuarantine', 'K02: quarantena portuale | ordinata da Lt. Surge')
    e.text('VermilionCity_Frlg_Text_Arianna', 'K02 Arianna: vai in Palestra, | Surge decide la quarantena')
    e.text('VermilionCity_Frlg_Text_Quarantine', 'Agente: quarantena! Niente | uscite senza il via di Surge')

    # ---- K04 Cerulean: Blu mocks Arianna after the Cascade Badge
    e = ed('CeruleanCity_Frlg')
    blu = 8
    ari = e.add_obj('LOCALID_KANTO_CERULEAN_ARIANNA', 'OBJ_EVENT_GFX_MAY_NORMAL', 23, 7,
                    'CeruleanCity_Frlg_EventScript_Arianna', 'FLAG_KANTO_HIDE_CERULEAN_ARIANNA', 'MOVEMENT_TYPE_FACE_UP')
    for side in ('Left', 'Mid', 'Right'):
        lab = 'CeruleanCity_Frlg_EventScript_RivalTrigger' + side
        e.kill(lab)
        e.enable_coords(lab)
        e.add('%s::\n\tgoto CeruleanCity_Frlg_EventScript_BluScene\n' % lab)
    e.add('''CeruleanCity_Frlg_EventScript_BluScene::
	goto_if_set FLAG_KANTO_SCENE_CERULEAN_BLU, Common_EventScript_NopReturn
	goto_if_unset FLAG_KANTO_BADGE02, Common_EventScript_NopReturn
	lockall
	clearflag FLAG_KANTO_HIDE_CERULEAN_RIVAL
	addobject %(blu)d
	applymovement %(blu)d, CeruleanCity_Frlg_Movement_BluDown
	waitmovement 0
	msgbox CeruleanCity_Frlg_Text_BluScene1, MSGBOX_DEFAULT
	msgbox CeruleanCity_Frlg_Text_BluScene2, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject %(blu)d
	removeobject %(ari)s
	setflag FLAG_KANTO_HIDE_CERULEAN_RIVAL
	setflag FLAG_KANTO_HIDE_CERULEAN_ARIANNA
	clearflag FLAG_KANTO_HIDE_MTMOON_SCENE
	setflag FLAG_KANTO_SCENE_CERULEAN_BLU
	fadescreen FADE_FROM_BLACK
	releaseall
	end

CeruleanCity_Frlg_Movement_BluDown:
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

CeruleanCity_Frlg_EventScript_Arianna::
	msgbox CeruleanCity_Frlg_Text_Arianna, MSGBOX_NPC
	end
''' % dict(blu=blu, ari=ari))
    e.text('CeruleanCity_Frlg_Text_BluScene1', 'K04 Blu deride Arianna | e il suo Taccuino')
    e.text('CeruleanCity_Frlg_Text_BluScene2', 'K04 Arianna corre da sola | al Monte Luna')
    e.text('CeruleanCity_Frlg_Text_Arianna', 'Arianna a Celestopoli | (prima della scena K04)')

    # ---- K06 Mt. Moon B2F: Morgana steals the notebook; fossils after the scene
    e = ed('MtMoon_B2F_Frlg')
    mor = e.add_obj('LOCALID_KANTO_MTMOON_MORGANA', 'OBJ_EVENT_GFX_MAGMA_MEMBER_F', 13, 9,
                    'MtMoon_B2F_Frlg_EventScript_Morgana', 'FLAG_KANTO_HIDE_MTMOON_SCENE', 'MOVEMENT_TYPE_FACE_DOWN')
    ari = e.add_obj('LOCALID_KANTO_MTMOON_ARIANNA', 'OBJ_EVENT_GFX_MAY_NORMAL', 14, 9,
                    'MtMoon_B2F_Frlg_EventScript_Arianna', 'FLAG_KANTO_HIDE_MTMOON_SCENE', 'MOVEMENT_TYPE_FACE_LEFT')
    e.add('''MtMoon_B2F_Frlg_EventScript_Morgana::
	lock
	faceplayer
	msgbox MtMoon_B2F_Frlg_Text_MorganaIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_KANTO_MORGANA_MTMOON, MtMoon_B2F_Frlg_Text_MorganaDefeat
	msgbox MtMoon_B2F_Frlg_Text_MorganaAfter, MSGBOX_DEFAULT
	msgbox MtMoon_B2F_Frlg_Text_GruntFlees, MSGBOX_DEFAULT
	msgbox MtMoon_B2F_Frlg_Text_AriannaSad, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject %(mor)s
	removeobject %(ari)s
	setflag FLAG_KANTO_HIDE_MTMOON_SCENE
	setflag FLAG_KANTO_SCENE_MTMOON
	fadescreen FADE_FROM_BLACK
	release
	end

MtMoon_B2F_Frlg_EventScript_Arianna::
	msgbox MtMoon_B2F_Frlg_Text_Arianna, MSGBOX_NPC
	end
''' % dict(mor=mor, ari=ari))
    e.text('MtMoon_B2F_Frlg_Text_MorganaIntro', 'K06 Morgana: ...Analisi completata. | (ha il Taccuino)')
    e.text('MtMoon_B2F_Frlg_Text_MorganaDefeat', 'K06 Morgana: Errore.')
    e.text('MtMoon_B2F_Frlg_Text_MorganaAfter', 'K06 Morgana: il dato e gia copiato. | Lore delle Pietralunari')
    e.text('MtMoon_B2F_Frlg_Text_GruntFlees', 'K06 una Recluta scappa | col Taccuino!')
    e.text('MtMoon_B2F_Frlg_Text_AriannaSad', 'K06 Arianna: senza Taccuino | non sono niente...')
    e.text('MtMoon_B2F_Frlg_Text_Arianna', 'K06 Arianna, sconvolta')
    for idx, item, other in ((1, 'ITEM_DOME_FOSSIL', 2), (2, 'ITEM_HELIX_FOSSIL', 1)):
        lab = e.obj_label(idx)
        e.kill(lab)
        e.add('''%(lab)s::
	lock
	goto_if_unset FLAG_KANTO_SCENE_MTMOON, %(lab)sLocked
	msgbox %(tl)sTake, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, %(lab)sLocked
	giveitem %(item)s
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	removeobject %(idx)d
	removeobject %(other)d
	setflag FLAG_KANTO_HIDE_DOME_FOSSIL
	setflag FLAG_KANTO_HIDE_HELIX_FOSSIL
	release
	end

%(lab)sLocked::
	release
	end
''' % dict(lab=lab, tl=lab.replace('_EventScript_', '_Text_'), item=item, idx=idx, other=other))
        e.text(lab.replace('_EventScript_', '_Text_') + 'Take', 'Monte Luna: prendi il %s?' %
               ('Fossilcupola' if idx == 1 else 'Fossilelica'))

    # ---- K08 Pewter Museum 2F: Rocco (optional)
    e = ed('PewterCity_Museum_2F_Frlg')
    e.add_obj('LOCALID_KANTO_MUSEUM_ROCCO', 'OBJ_EVENT_GFX_STEVEN', 12, 3, 'PewterCity_Museum_2F_Frlg_EventScript_Rocco')
    e.add('''PewterCity_Museum_2F_Frlg_EventScript_Rocco::
	lock
	faceplayer
	goto_if_set FLAG_KANTO_SCENE_ROCCO, PewterCity_Museum_2F_Frlg_EventScript_RoccoAfter
	msgbox PewterCity_Museum_2F_Frlg_Text_Rocco, MSGBOX_DEFAULT
	giveitem ITEM_MOON_STONE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_KANTO_SCENE_ROCCO
	release
	end

PewterCity_Museum_2F_Frlg_EventScript_RoccoAfter::
	msgbox PewterCity_Museum_2F_Frlg_Text_RoccoAfter, MSGBOX_DEFAULT
	release
	end
''')
    e.text('PewterCity_Museum_2F_Frlg_Text_Rocco', 'K08 Rocco studia la Pietralunare | e te ne regala una')
    e.text('PewterCity_Museum_2F_Frlg_Text_RoccoAfter', 'K08 Rocco, dopo il regalo')

    # ---- K09 Bill (optional): Lucky Egg once
    e = ed('Route25_SeaCottage_Frlg')
    lab = e.obj_label(1)
    e.kill(lab)
    e.add('''%(lab)s::
	lock
	faceplayer
	goto_if_set FLAG_KANTO_SCENE_BILL, %(lab)sAfter
	msgbox Route25_SeaCottage_Frlg_Text_Bill, MSGBOX_DEFAULT
	giveitem ITEM_LUCKY_EGG
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_KANTO_SCENE_BILL
	release
	end

%(lab)sAfter::
	msgbox Route25_SeaCottage_Frlg_Text_BillAfter, MSGBOX_DEFAULT
	release
	end
''' % dict(lab=lab))
    e.text('Route25_SeaCottage_Frlg_Text_Bill', 'K09 Bill: il mio PC parla | con quello di Lanette (regalo)')
    e.text('Route25_SeaCottage_Frlg_Text_BillAfter', 'K09 Bill, dopo il regalo')

    # ---- K11 Game Corner: the grunt guards the poster; stairs hidden until he is beaten
    mp = 'CeladonCity_GameCorner_Frlg'
    e = ed(mp)
    inv = ed.inv[mp]
    gr = [o for o in inv['objects'] if o['index'] == 11][0]
    e.kill(gr['script'], text=False)
    kid = 'TRAINER_KANTO_' + gr['orig_trainer'][8:]
    base = gr['script'].replace('_EventScript_', '_Text_')
    pf, pt = 'CeladonCity_GameCorner_', 'CeladonCity_GameCorner_Frlg_'
    e.add('''%(lab)s::
	trainerbattle_single %(kid)s, %(base)sIntro, %(base)sDefeat, %(lab)sDefeated
	msgbox %(base)sPostBattle, MSGBOX_AUTOCLOSE
	end

%(lab)sDefeated::
	msgbox %(base)sPostBattle, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject 11
	setflag FLAG_KANTO_HIDE_GAME_CORNER_ROCKET
	fadescreen FADE_FROM_BLACK
	release
	end

%(pt)sOnLoad::
	call_if_unset FLAG_KANTO_HIDE_GAME_CORNER_ROCKET, %(pt)sEventScript_HideRocketHideout
	end

%(pt)sEventScript_Poster::
	lockall
	msgbox %(pt)sText_Poster, MSGBOX_DEFAULT
	call_if_set FLAG_KANTO_HIDE_GAME_CORNER_ROCKET, %(pt)sEventScript_OpenRocketHideout
	releaseall
	end
''' % dict(lab=gr['script'], kid=kid, base=base, pt=pt))
    e.kill(pt + 'EventScript_Poster')
    e.add(ed.orig(mp, pf + 'EventScript_HideRocketHideout', pf, pt))
    e.add(ed.orig(mp, pf + 'EventScript_OpenRocketHideout', pf, pt))
    e.mapscript('MAP_SCRIPT_ON_LOAD', pt + 'OnLoad')
    e.text(pt + 'Text_Poster', 'Dietro il poster c\'e | un interruttore!')

    # ---- K12 Rocket Hideout B4F: Giovanni (the Silph Scope ball appears after him)
    e = ed('RocketHideout_B4F_Frlg')
    lab = e.obj_label(1)
    e.kill(lab)
    e.add('''%(lab)s::
	lock
	faceplayer
	msgbox RocketHideout_B4F_Frlg_Text_GiovanniIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_KANTO_BOSS_GIOVANNI, RocketHideout_B4F_Frlg_Text_GiovanniDefeat
	msgbox RocketHideout_B4F_Frlg_Text_GiovanniAfter, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject 1
	setflag FLAG_KANTO_HIDE_HIDEOUT_GIOVANNI
	clearflag FLAG_KANTO_HIDE_SILPH_SCOPE
	addobject 2
	fadescreen FADE_FROM_BLACK
	release
	end
''' % dict(lab=lab))
    e.text('RocketHideout_B4F_Frlg_Text_GiovanniIntro', 'K12 Giovanni: il casello | sul Grande Varco')
    e.text('RocketHideout_B4F_Frlg_Text_GiovanniDefeat', 'K12 Giovanni: alla sconfitta')
    e.text('RocketHideout_B4F_Frlg_Text_GiovanniAfter', 'K12 Giovanni se ne va')
    e = ed('RocketHideout_Elevator_Frlg')
    e.kill('RocketHideout_Elevator_Frlg_EventScript_FloorSelect')
    e.add('RocketHideout_Elevator_Frlg_EventScript_FloorSelect::\n\tgoto KantoElevator_EventScript_RocketHideout\n')
    e = ed('SilphCo_Elevator_Frlg')
    e.kill('SilphCo_Elevator_Frlg_EventScript_FloorSelect')
    e.add('SilphCo_Elevator_Frlg_EventScript_FloorSelect::\n\tgoto KantoElevator_EventScript_Silph\n')
    e = ed('CeladonCity_DepartmentStore_Elevator_Frlg')
    e.kill('CeladonCity_DepartmentStore_Elevator_Frlg_EventScript_FloorSelect')
    e.add('CeladonCity_DepartmentStore_Elevator_Frlg_EventScript_FloorSelect::\n\tgoto KantoElevator_EventScript_CeladonDept\n')

    # ---- K13 Pokemon Tower 2F: Blu (no battle)
    e = ed('PokemonTower_2F_Frlg')
    for side in ('Right', 'Down'):
        lab = 'PokemonTower_2F_Frlg_EventScript_RivalTrigger' + side
        e.kill(lab)
        e.enable_coords(lab)
        e.add('%s::\n\tgoto PokemonTower_2F_Frlg_EventScript_BluScene\n' % lab)
    e.add('''PokemonTower_2F_Frlg_EventScript_BluScene::
	goto_if_set FLAG_KANTO_HIDE_TOWER_RIVAL, Common_EventScript_NopReturn
	lockall
	applymovement 1, Common_Movement_FacePlayer
	waitmovement 0
	msgbox PokemonTower_2F_Frlg_Text_BluScene, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject 1
	setflag FLAG_KANTO_HIDE_TOWER_RIVAL
	fadescreen FADE_FROM_BLACK
	releaseall
	end
''')
    e.text('PokemonTower_2F_Frlg_Text_BluScene', 'K13 Blu davanti a una tomba | (momento umano, nessuna lotta)')

    # ---- K14 Pokemon Tower 6F: Marowak ghost (needs the Silph Scope)
    e = ed('PokemonTower_6F_Frlg')
    lab = 'PokemonTower_6F_Frlg_EventScript_MarowakGhost'
    e.kill(lab)
    e.enable_coords(lab)
    e.add('''%(lab)s::
	goto_if_set FLAG_KANTO_STORY_TOWER_GHOST, Common_EventScript_NopReturn
	lockall
	goto_if_unset FLAG_KANTO_HIDE_HIDEOUT_GIOVANNI, %(lab)sNoScope
	msgbox PokemonTower_6F_Frlg_Text_GhostRevealed, MSGBOX_DEFAULT
	closemessage
	setwildbattle SPECIES_MAROWAK_ALOLA, 60
	playmoncry SPECIES_MAROWAK_ALOLA, CRY_MODE_ENCOUNTER
	waitmoncry
	dowildbattle
	setflag FLAG_KANTO_STORY_TOWER_GHOST
	msgbox PokemonTower_6F_Frlg_Text_GhostAtPeace, MSGBOX_DEFAULT
	releaseall
	end

%(lab)sNoScope::
	msgbox PokemonTower_6F_Frlg_Text_GhostGoAway, MSGBOX_DEFAULT
	goto KantoStory_EventScript_PushUp
''' % dict(lab=lab))
    e.text('PokemonTower_6F_Frlg_Text_GhostGoAway', 'K14 Uno spettro: Vattene...! | (serve la Spettrosonda)')
    e.text('PokemonTower_6F_Frlg_Text_GhostRevealed', 'K14 La Spettrosonda rivela | lo spettro: e un Marowak!')
    e.text('PokemonTower_6F_Frlg_Text_GhostAtPeace', 'K14 Lo spettro trova pace.')

    # ---- K15 Pokemon Tower 7F: three grunts, then Mr. Fuji
    mp = 'PokemonTower_7F_Frlg'
    e = ed(mp)
    for idx, fl in ((2, 1), (3, 2), (4, 3)):
        o = [o for o in ed.inv[mp]['objects'] if o['index'] == idx][0]
        lab, kid = o['script'], 'TRAINER_KANTO_' + o['orig_trainer'][8:]
        base = lab.replace('_EventScript_', '_Text_')
        e.kill(lab, text=False)
        e.add('''%(lab)s::
	trainerbattle_single %(kid)s, %(base)sIntro, %(base)sDefeat, %(lab)sDefeated
	end

%(lab)sDefeated::
	msgbox %(base)sPostBattle, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject %(idx)d
	setflag FLAG_KANTO_HIDE_TOWER_ROCKET_%(fl)d
	fadescreen FADE_FROM_BLACK
	release
	end
''' % dict(lab=lab, kid=kid, base=base, idx=idx, fl=fl))
    lab = e.obj_label(1)
    e.kill(lab)
    e.add('''%(lab)s::
	lock
	faceplayer
	goto_if_unset FLAG_KANTO_HIDE_TOWER_ROCKET_1, %(lab)sHelp
	goto_if_unset FLAG_KANTO_HIDE_TOWER_ROCKET_2, %(lab)sHelp
	goto_if_unset FLAG_KANTO_HIDE_TOWER_ROCKET_3, %(lab)sHelp
	msgbox PokemonTower_7F_Frlg_Text_FujiRescued, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject 1
	setflag FLAG_KANTO_HIDE_TOWER_FUJI
	setflag FLAG_KANTO_STORY_FUJI_RESCUED
	clearflag FLAG_KANTO_HIDE_POKEHOUSE_FUJI
	clearflag FLAG_KANTO_HIDE_FUJI_HOUSE_EXILES
	fadescreen FADE_FROM_BLACK
	release
	end

%(lab)sHelp::
	msgbox PokemonTower_7F_Frlg_Text_FujiHelp, MSGBOX_DEFAULT
	release
	end
''' % dict(lab=lab))
    e.text('PokemonTower_7F_Frlg_Text_FujiHelp', 'K15 Fuji: le Reclute mi | interrogano su Mewtwo...')
    e.text('PokemonTower_7F_Frlg_Text_FujiRescued', 'K15 Fuji liberato: ci vediamo | alla Casa del Volontariato')

    # ---- K16 Volunteer House: Fuji + Ettore/Ulisse, Poke Flute
    e = ed('LavenderTown_VolunteerPokemonHouse_Frlg')
    e.add_obj('LOCALID_KANTO_FUJI_HOUSE_ETTORE', 'OBJ_EVENT_GFX_MAXIE', 9, 3,
              'LavenderTown_VolunteerPokemonHouse_Frlg_EventScript_Ettore', 'FLAG_KANTO_HIDE_FUJI_HOUSE_EXILES')
    e.add_obj('LOCALID_KANTO_FUJI_HOUSE_ULISSE', 'OBJ_EVENT_GFX_ARCHIE', 10, 4,
              'LavenderTown_VolunteerPokemonHouse_Frlg_EventScript_Ulisse', 'FLAG_KANTO_HIDE_FUJI_HOUSE_EXILES',
              'MOVEMENT_TYPE_FACE_LEFT')
    lab = e.obj_label(1)
    e.kill(lab)
    e.add('''%(lab)s::
	lock
	faceplayer
	goto_if_set FLAG_KANTO_STORY_GOT_FLUTE, %(lab)sAfter
	msgbox LavenderTown_VolunteerPokemonHouse_Frlg_Text_FujiConfession, MSGBOX_DEFAULT
	giveitem ITEM_POKE_FLUTE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_KANTO_STORY_GOT_FLUTE
	msgbox LavenderTown_VolunteerPokemonHouse_Frlg_Text_FujiFlute, MSGBOX_DEFAULT
	release
	end

%(lab)sAfter::
	msgbox LavenderTown_VolunteerPokemonHouse_Frlg_Text_FujiAfter, MSGBOX_DEFAULT
	release
	end

LavenderTown_VolunteerPokemonHouse_Frlg_EventScript_Ettore::
	msgbox LavenderTown_VolunteerPokemonHouse_Frlg_Text_Ettore, MSGBOX_NPC
	end

LavenderTown_VolunteerPokemonHouse_Frlg_EventScript_Ulisse::
	msgbox LavenderTown_VolunteerPokemonHouse_Frlg_Text_Ulisse, MSGBOX_NPC
	end
''' % dict(lab=lab))
    e.text('LavenderTown_VolunteerPokemonHouse_Frlg_Text_FujiConfession', 'K16 Fuji confessa | la creazione di Mewtwo')
    e.text('LavenderTown_VolunteerPokemonHouse_Frlg_Text_FujiFlute', 'K16 Fuji: il Poke Flauto | sveglia gli Snorlax')
    e.text('LavenderTown_VolunteerPokemonHouse_Frlg_Text_FujiAfter', 'K16 Fuji, dopo il Flauto')
    e.text('LavenderTown_VolunteerPokemonHouse_Frlg_Text_Ettore', 'K16 Ettore: Morgana era la mia | migliore analista')
    e.text('LavenderTown_VolunteerPokemonHouse_Frlg_Text_Ulisse', 'K16 Ulisse: Mozzo! | (fa il volontario)')

    # ---- K17 Snorlax (Routes 12 and 16)
    for mp, idx, fl in (('Route12_Frlg', 5, 'FLAG_KANTO_HIDE_ROUTE_12_SNORLAX'),
                        ('Route16_Frlg', 10, 'FLAG_KANTO_HIDE_ROUTE_16_SNORLAX')):
        e = ed(mp)
        lab = e.obj_label(idx)
        e.kill(lab)
        t = mp + '_Text_Snorlax'
        e.add('''%(lab)s::
	lock
	faceplayer
	goto_if_unset FLAG_KANTO_STORY_GOT_FLUTE, %(lab)sAsleep
	msgbox %(t)sFlute, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, %(lab)sNo
	msgbox %(t)sWakes, MSGBOX_DEFAULT
	closemessage
	setflag %(fl)s
	removeobject %(idx)d
	setwildbattle SPECIES_SNORLAX, 60
	setvar VAR_0x8004, SPECIES_SNORLAX
	goto KantoStory_EventScript_StaticBattle

%(lab)sAsleep::
	msgbox %(t)sAsleep, MSGBOX_DEFAULT
%(lab)sNo::
	release
	end
''' % dict(lab=lab, t=t, fl=fl, idx=idx))
        e.text(t + 'Asleep', 'Un Pokemon enorme dorme | e blocca la strada.')
        e.text(t + 'Flute', 'Vuoi suonare il Poke Flauto?')
        e.text(t + 'Wakes', 'Snorlax si sveglia | ed e furioso!')

    # ---- Saffron gates (open after Koga, K18)
    for mp, sides, push in (('Route5_SouthEntrance_Frlg', ('Left', 'Mid', 'Right'), 'Up'),
                            ('Route6_NorthEntrance_Frlg', ('Left', 'Mid', 'Right'), 'Down'),
                            ('Route7_EastEntrance_Frlg', ('Top', 'Mid', 'Bottom'), 'Left'),
                            ('Route8_WestEntrance_Frlg', ('Top', 'Mid', 'Bottom'), 'Right')):
        e = ed(mp)
        for sd in sides:
            lab = '%s_EventScript_GuardTrigger%s' % (mp, sd)
            e.kill(lab)
            e.enable_coords(lab)
            e.add('%s::\n\tgoto %s_EventScript_GateClosed\n' % (lab, mp))
        e.add('''%(mp)s_EventScript_GateClosed::
	goto_if_set FLAG_KANTO_STORY_SAFFRON_OPEN, Common_EventScript_NopReturn
	lockall
	msgbox %(mp)s_Text_GateClosed, MSGBOX_DEFAULT
	goto KantoStory_EventScript_Push%(push)s
''' % dict(mp=mp, push=push))
        e.text(mp + '_Text_GateClosed', 'Guardia: Zafferanopoli e chiusa! | (si apre dopo Koga)')

    # ---- K19 Silph 7F: Arianna (Blu's object reused)
    e = ed('SilphCo_7F_Frlg')
    e.set_obj(1, graphics_id='OBJ_EVENT_GFX_MAY_NORMAL', script='SilphCo_7F_Frlg_EventScript_Arianna')
    for side in ('Top', 'Bottom'):
        lab = 'SilphCo_7F_Frlg_EventScript_RivalTrigger' + side
        e.kill(lab)
        e.enable_coords(lab)
        e.add('%s::\n\tgoto SilphCo_7F_Frlg_EventScript_AriannaBattle\n' % lab)
    e.add('''SilphCo_7F_Frlg_EventScript_Arianna::
	lock
	faceplayer
	goto SilphCo_7F_Frlg_EventScript_AriannaBattleLocked

SilphCo_7F_Frlg_EventScript_AriannaBattle::
	goto_if_set FLAG_KANTO_HIDE_SILPH_RIVAL, Common_EventScript_NopReturn
	lockall
	applymovement 1, Common_Movement_FacePlayer
	waitmovement 0
SilphCo_7F_Frlg_EventScript_AriannaBattleLocked::
	msgbox SilphCo_7F_Frlg_Text_AriannaIntro, MSGBOX_DEFAULT
	switch VAR_STARTER_MON
	case 0, SilphCo_7F_Frlg_EventScript_AriannaTreecko
	case 1, SilphCo_7F_Frlg_EventScript_AriannaTorchic
	case 2, SilphCo_7F_Frlg_EventScript_AriannaMudkip
SilphCo_7F_Frlg_EventScript_AriannaTreecko::
	trainerbattle_no_intro TRAINER_KANTO_ARIANNA_SILPH_TREECKO, SilphCo_7F_Frlg_Text_AriannaDefeat
	goto SilphCo_7F_Frlg_EventScript_AriannaAfter
SilphCo_7F_Frlg_EventScript_AriannaTorchic::
	trainerbattle_no_intro TRAINER_KANTO_ARIANNA_SILPH_TORCHIC, SilphCo_7F_Frlg_Text_AriannaDefeat
	goto SilphCo_7F_Frlg_EventScript_AriannaAfter
SilphCo_7F_Frlg_EventScript_AriannaMudkip::
	trainerbattle_no_intro TRAINER_KANTO_ARIANNA_SILPH_MUDKIP, SilphCo_7F_Frlg_Text_AriannaDefeat
SilphCo_7F_Frlg_EventScript_AriannaAfter::
	msgbox SilphCo_7F_Frlg_Text_AriannaAfter, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject 1
	setflag FLAG_KANTO_HIDE_SILPH_RIVAL
	setflag FLAG_KANTO_STORY_NOTEBOOK_BACK
	fadescreen FADE_FROM_BLACK
	releaseall
	end
''')
    e.text('SilphCo_7F_Frlg_Text_AriannaIntro', 'K19 Arianna: volevo farcela | da sola... Lottiamo!')
    e.text('SilphCo_7F_Frlg_Text_AriannaDefeat', 'K19 Arianna: alla sconfitta')
    e.text('SilphCo_7F_Frlg_Text_AriannaAfter', 'K19 Arianna riprende il Taccuino | e fa squadra con te')

    # ---- K20 Silph 11F: Giovanni, President (Master Ball)
    e = ed('SilphCo_11F_Frlg')
    e.set_obj(3, script='SilphCo_11F_Frlg_EventScript_Giovanni')
    for side in ('Left', 'Right'):
        lab = 'SilphCo_11F_Frlg_EventScript_GiovanniTrigger' + side
        e.kill(lab)
        e.enable_coords(lab)
        e.add('%s::\n\tgoto SilphCo_11F_Frlg_EventScript_GiovanniBattle\n' % lab)
    lab = e.obj_label(1)
    e.kill(lab)
    e.add('''SilphCo_11F_Frlg_EventScript_Giovanni::
	lock
	faceplayer
	goto SilphCo_11F_Frlg_EventScript_GiovanniLocked

SilphCo_11F_Frlg_EventScript_GiovanniBattle::
	goto_if_set FLAG_KANTO_STORY_SILPH_FREED, Common_EventScript_NopReturn
	lockall
	applymovement 3, Common_Movement_FacePlayer
	waitmovement 0
SilphCo_11F_Frlg_EventScript_GiovanniLocked::
	msgbox SilphCo_11F_Frlg_Text_GiovanniIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_KANTO_BOSS_GIOVANNI_2, SilphCo_11F_Frlg_Text_GiovanniDefeat
	msgbox SilphCo_11F_Frlg_Text_GiovanniAfter, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject 3
	setflag FLAG_KANTO_STORY_SILPH_FREED
	setflag FLAG_KANTO_HIDE_SILPH_ROCKETS
	setflag FLAG_KANTO_HIDE_SAFFRON_ROCKETS
	clearflag FLAG_KANTO_HIDE_SAFFRON_CIVILIANS
	clearflag FLAG_KANTO_HIDE_OAKLAB_ARIANNA
	fadescreen FADE_FROM_BLACK
	releaseall
	end

%(lab)s::
	lock
	faceplayer
	goto_if_unset FLAG_KANTO_STORY_SILPH_FREED, %(lab)sScared
	goto_if_set FLAG_KANTO_GOT_MASTER_BALL, %(lab)sAfter
	msgbox SilphCo_11F_Frlg_Text_President, MSGBOX_DEFAULT
	giveitem ITEM_MASTER_BALL
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_KANTO_GOT_MASTER_BALL
	release
	end

%(lab)sScared::
	msgbox SilphCo_11F_Frlg_Text_PresidentScared, MSGBOX_DEFAULT
	release
	end

%(lab)sAfter::
	msgbox SilphCo_11F_Frlg_Text_PresidentAfter, MSGBOX_DEFAULT
	release
	end
''' % dict(lab=lab))
    e.text('SilphCo_11F_Frlg_Text_GiovanniIntro', 'K20 Giovanni alla Silph')
    e.text('SilphCo_11F_Frlg_Text_GiovanniDefeat', 'K20 Giovanni: alla sconfitta')
    e.text('SilphCo_11F_Frlg_Text_GiovanniAfter', 'K20 Giovanni: Morgana e sparita | col prototipo della Serratura')
    e.text('SilphCo_11F_Frlg_Text_President', 'K20 Presidente: grazie! | Ecco la Master Ball')
    e.text('SilphCo_11F_Frlg_Text_PresidentScared', 'K20 Presidente, ostaggio')
    e.text('SilphCo_11F_Frlg_Text_PresidentAfter', 'K20 Presidente, dopo il regalo')

    # ---- K22 Oak's Lab: Oak, Arianna, Kanto starter
    mp = 'PalletTown_ProfessorOaksLab_Frlg'
    e = ed(mp)
    e.add_obj('LOCALID_KANTO_OAKLAB_ARIANNA', 'OBJ_EVENT_GFX_MAY_NORMAL', 5, 5,
              'PalletTown_ProfessorOaksLab_Frlg_EventScript_Arianna', 'FLAG_KANTO_HIDE_OAKLAB_ARIANNA', 'MOVEMENT_TYPE_FACE_UP')
    lab = e.obj_label(4)
    e.kill(lab)
    e.add('''%(lab)s::
	lock
	faceplayer
	goto_if_unset FLAG_KANTO_STORY_SILPH_FREED, %(lab)sGeneric
	goto_if_set FLAG_KANTO_STORY_OAK_MET, %(lab)sAfter
	msgbox PalletTown_ProfessorOaksLab_Frlg_Text_OakLore, MSGBOX_DEFAULT
	msgbox PalletTown_ProfessorOaksLab_Frlg_Text_OakArianna, MSGBOX_DEFAULT
	setflag FLAG_KANTO_STORY_OAK_MET
	release
	end

%(lab)sGeneric::
	msgbox PalletTown_ProfessorOaksLab_Frlg_Text_OakGeneric, MSGBOX_DEFAULT
	release
	end

%(lab)sAfter::
	msgbox PalletTown_ProfessorOaksLab_Frlg_Text_OakAfter, MSGBOX_DEFAULT
	release
	end

PalletTown_ProfessorOaksLab_Frlg_EventScript_Arianna::
	msgbox PalletTown_ProfessorOaksLab_Frlg_Text_Arianna, MSGBOX_NPC
	end
''' % dict(lab=lab))
    e.text('PalletTown_ProfessorOaksLab_Frlg_Text_OakGeneric', 'Oak: benvenuto da un altro mondo! | (prima della Silph)')
    e.text('PalletTown_ProfessorOaksLab_Frlg_Text_OakLore', 'K22 Oak: Mew e la Villa | (lore)')
    e.text('PalletTown_ProfessorOaksLab_Frlg_Text_OakArianna', 'K22 Oak offre ad Arianna | di fare la ricercatrice')
    e.text('PalletTown_ProfessorOaksLab_Frlg_Text_OakAfter', 'Oak: scegli un Pokemon | di Kanto, se vuoi')
    e.text('PalletTown_ProfessorOaksLab_Frlg_Text_Arianna', 'K22 Arianna nel laboratorio')
    for idx, sp in ((5, 'SPECIES_BULBASAUR'), (6, 'SPECIES_SQUIRTLE'), (7, 'SPECIES_CHARMANDER')):
        lab = e.obj_label(idx)
        e.kill(lab)
        t = lab.replace('_EventScript_', '_Text_')
        e.add('''%(lab)s::
	lock
	goto_if_unset FLAG_KANTO_STORY_OAK_MET, %(lab)sNo
	goto_if_set FLAG_KANTO_GOT_KANTO_STARTER, %(lab)sNo
	bufferspeciesname STR_VAR_1, %(sp)s
	msgbox %(t)sChoose, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, %(lab)sNo
	givemon %(sp)s, 50
	goto_if_eq VAR_RESULT, MON_CANT_GIVE, %(lab)sNo
	setflag FLAG_KANTO_GOT_KANTO_STARTER
	playfanfare MUS_OBTAIN_ITEM
	message %(t)sReceived
	waitmessage
	waitfanfare
	release
	end

%(lab)sNo::
	msgbox %(t)s, MSGBOX_DEFAULT
	release
	end
''' % dict(lab=lab, sp=sp, t=t))
        e.text(t, 'Una Poke Ball del Prof. Oak.')
        e.text(t + 'Choose', 'Vuoi {STR_VAR_1}?')
        e.text(t + 'Received', '{PLAYER} riceve {STR_VAR_1}!')

    # ---- K23 Mansion switches (statues) - the Secret Key ball B1F opens the Cinnabar gym
    for mp, n in (('PokemonMansion_1F_Frlg', 0), ('PokemonMansion_2F_Frlg', 1), ('PokemonMansion_3F_Frlg', 2),
                  ('PokemonMansion_B1F_Frlg', 3)):
        e = ed(mp)
        flr = ['1F', '2F', '3F', 'B1F'][n]
        lab = mp + '_EventScript_Statue'
        e.kill(lab)
        e.add('''%(lab)s::
	lockall
	setvar VAR_0x8004, %(n)d
	call KantoMansion_EventScript_SecretSwitch
	special DrawWholeMapView
	releaseall
	end

%(mp)s_OnLoad::
	call_if_set FLAG_KANTO_MANSION_SWITCH, KantoMansion_EventScript_PressSwitch_%(flr)s
	end
''' % dict(lab=lab, n=n, mp=mp, flr=flr))
        e.mapscript('MAP_SCRIPT_ON_LOAD', mp + '_OnLoad')

    # ---- Gym doors: Cinnabar (Secret Key) and Viridian (7 badges + Oak)
    e = ed('CinnabarIsland_Frlg')
    lab = 'CinnabarIsland_Frlg_EventScript_GymDoorLocked'
    e.kill(lab)
    e.enable_coords(lab)
    e.add('''%s::
	goto_if_set FLAG_KANTO_HIDE_POKEMON_MANSION_B1F_SECRET_KEY, Common_EventScript_NopReturn
	lockall
	msgbox CinnabarIsland_Frlg_Text_GymDoorLocked, MSGBOX_DEFAULT
	goto KantoStory_EventScript_PushDown
''' % lab)
    e.text('CinnabarIsland_Frlg_Text_GymDoorLocked', 'La porta della Palestra e chiusa. | (serve la Chiave Segreta)')
    e = ed('ViridianCity_Frlg')
    lab = 'ViridianCity_Frlg_EventScript_GymDoorLocked'
    e.kill(lab)
    e.enable_coords(lab)
    e.add('''%(lab)s::
	call KantoStory_EventScript_CountBadges
	goto_if_lt VAR_RESULT, 7, %(lab)sClosed
	goto_if_unset FLAG_KANTO_STORY_OAK_MET, %(lab)sClosed
	end

%(lab)sClosed::
	lockall
	msgbox ViridianCity_Frlg_Text_GymDoorLocked, MSGBOX_DEFAULT
	goto KantoStory_EventScript_PushDown
''' % dict(lab=lab))
    e.text('ViridianCity_Frlg_Text_GymDoorLocked', 'La Palestra e chiusa. | (7 Medaglie + visita a Oak)')

    # ---- K26 Cerulean Cave 1F: climax with Morgana
    mp = 'CeruleanCave_1F_Frlg'
    e = ed(mp)
    mor = e.add_obj('LOCALID_KANTO_CAVE_MORGANA', 'OBJ_EVENT_GFX_MAGMA_MEMBER_F', 33, 15,
                    'CeruleanCave_1F_Frlg_EventScript_Morgana', 'FLAG_KANTO_HIDE_CAVE_SCENE')
    ett = e.add_obj('LOCALID_KANTO_CAVE_ETTORE', 'OBJ_EVENT_GFX_MAXIE', 35, 14,
                    'CeruleanCave_1F_Frlg_EventScript_Ettore', 'FLAG_KANTO_HIDE_CAVE_SCENE')
    uli = e.add_obj('LOCALID_KANTO_CAVE_ULISSE', 'OBJ_EVENT_GFX_ARCHIE', 36, 15,
                    'CeruleanCave_1F_Frlg_EventScript_Ulisse', 'FLAG_KANTO_HIDE_CAVE_SCENE')
    ari = e.add_obj('LOCALID_KANTO_CAVE_ARIANNA', 'OBJ_EVENT_GFX_MAY_NORMAL', 32, 14,
                    'CeruleanCave_1F_Frlg_EventScript_Ari', 'FLAG_KANTO_HIDE_CAVE_SCENE')
    for x in (32, 33, 34):
        e.add_coord(x, 20, 'CeruleanCave_1F_Frlg_EventScript_Climax')
    e.add('''CeruleanCave_1F_Frlg_EventScript_Climax::
	goto_if_set FLAG_KANTO_STORY_RIFT_CALMED, Common_EventScript_NopReturn
	lockall
	goto CeruleanCave_1F_Frlg_EventScript_ClimaxLocked

CeruleanCave_1F_Frlg_EventScript_Morgana::
	lock
	faceplayer
CeruleanCave_1F_Frlg_EventScript_ClimaxLocked::
	msgbox CeruleanCave_1F_Frlg_Text_MorganaIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_KANTO_MORGANA_CERULEAN_CAVE, CeruleanCave_1F_Frlg_Text_MorganaDefeat
	msgbox CeruleanCave_1F_Frlg_Text_AriannaHeartbeat, MSGBOX_DEFAULT
	closemessage
	playse SE_M_PSYBEAM
	fadescreen FADE_TO_WHITE
	waitse
	msgbox CeruleanCave_1F_Frlg_Text_MewtwoFlash, MSGBOX_DEFAULT
	fadescreen FADE_FROM_WHITE
	msgbox CeruleanCave_1F_Frlg_Text_EttoreTakesMorgana, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject %(mor)s
	removeobject %(ett)s
	removeobject %(uli)s
	removeobject %(ari)s
	setflag FLAG_KANTO_HIDE_CAVE_SCENE
	setflag FLAG_KANTO_STORY_RIFT_CALMED
	clearflag FLAG_KANTO_HIDE_MEWTWO
	fadescreen FADE_FROM_BLACK
	releaseall
	end

CeruleanCave_1F_Frlg_EventScript_Ettore::
	msgbox CeruleanCave_1F_Frlg_Text_Ettore, MSGBOX_NPC
	end

CeruleanCave_1F_Frlg_EventScript_Ulisse::
	msgbox CeruleanCave_1F_Frlg_Text_Ulisse, MSGBOX_NPC
	end

CeruleanCave_1F_Frlg_EventScript_Ari::
	msgbox CeruleanCave_1F_Frlg_Text_Arianna, MSGBOX_NPC
	end
''' % dict(mor=mor, ett=ett, uli=uli, ari=ari))
    e.text('CeruleanCave_1F_Frlg_Text_MorganaIntro', 'K26 Morgana: muro, porta... | errore doppio. La Serratura!')
    e.text('CeruleanCave_1F_Frlg_Text_MorganaDefeat', 'K26 Morgana: Errore.')
    e.text('CeruleanCave_1F_Frlg_Text_AriannaHeartbeat', 'K26 Arianna trova il battito: | la Serratura si rompe!')
    e.text('CeruleanCave_1F_Frlg_Text_MewtwoFlash', 'K26 Ondata psichica: Mewtwo | guarda e torna nel buio')
    e.text('CeruleanCave_1F_Frlg_Text_EttoreTakesMorgana', 'K26 Ettore: il muro era un errore. | Mio. Vieni.')
    e.text('CeruleanCave_1F_Frlg_Text_Ettore', 'K26 Ettore')
    e.text('CeruleanCave_1F_Frlg_Text_Ulisse', 'K26 Ulisse')
    e.text('CeruleanCave_1F_Frlg_Text_Arianna', 'K26 Arianna')

    # ---- K27 Route 22 gate + Route 23 badge guards
    e = ed('Route22_NorthEntrance_Frlg')
    lab = 'Route22_NorthEntrance_Frlg_EventScript_BoulderBadgeGuardTrigger'
    e.kill(lab)
    e.enable_coords(lab)
    e.add('''%s::
	goto_if_set FLAG_KANTO_STORY_RIFT_CALMED, Common_EventScript_NopReturn
	lockall
	msgbox Route22_NorthEntrance_Frlg_Text_Closed, MSGBOX_DEFAULT
	goto KantoStory_EventScript_PushDown
''' % lab)
    e.text('Route22_NorthEntrance_Frlg_Text_Closed', 'Guardia: la strada per la Lega | e chiusa. (dopo la Grotta Celeste)')
    e = ed('Route23_Frlg')
    for nm, badge in (('Cascade', 2), ('Thunder', 3), ('Rainbow', 4), ('Soul', 5), ('Marsh', 6), ('Volcano', 7),
                      ('Earth', 8)):
        lab = 'Route23_Frlg_EventScript_%sBadgeGuardTrigger' % nm
        e.kill(lab)
        e.enable_coords(lab)
        e.add('''%(lab)s::
	goto_if_set FLAG_KANTO_BADGE%(b)02d, Common_EventScript_NopReturn
	lockall
	msgbox Route23_Frlg_Text_%(nm)sMissing, MSGBOX_DEFAULT
	goto KantoStory_EventScript_PushDown
''' % dict(lab=lab, b=badge, nm=nm))
        e.text('Route23_Frlg_Text_%sMissing' % nm, 'Guardia: senza la Medaglia | (%s) non si passa!' % nm)

    # ---- Seafoam Islands: currents always calm (the boulder logic of FRLG is not ported)
    for fl in ('B3F', 'B4F'):
        mp = 'SeafoamIslands_%s_Frlg' % fl
        e = ed(mp)
        e.add('%s_OnTransition::\n\tsetmaplayoutindex LAYOUT_SEAFOAM_ISLANDS_%s_CURRENT_STOPPED\n\tend\n' % (mp, fl))
        e.mapscript('MAP_SCRIPT_ON_TRANSITION', mp + '_OnTransition')

    # ---- K30/K31 legendaries
    for mp, idx, sp, lvl, fl in (('PowerPlant_Frlg', 6, 'SPECIES_ZAPDOS', 65, 'FLAG_KANTO_HIDE_ZAPDOS'),
                                 ('SeafoamIslands_B4F_Frlg', 3, 'SPECIES_ARTICUNO', 65, 'FLAG_KANTO_HIDE_ARTICUNO'),
                                 ('CeruleanCave_B1F_Frlg', 3, 'SPECIES_MEWTWO', 70, 'FLAG_KANTO_HIDE_MEWTWO')):
        e = ed(mp)
        lab = e.obj_label(idx)
        e.kill(lab)
        t = lab.replace('_EventScript_', '_Text_')
        e.add('''%(lab)s::
	lock
	faceplayer
	msgbox %(t)s, MSGBOX_DEFAULT
	closemessage
	setflag %(fl)s
	removeobject %(idx)d
	setwildbattle %(sp)s, %(lvl)d
	setvar VAR_0x8004, %(sp)s
	goto KantoStory_EventScript_StaticBattle
''' % dict(lab=lab, t=t, fl=fl, idx=idx, sp=sp, lvl=lvl))
        e.text(t, 'K30/K31 %s ti fissa! | (lotta Lv %d)' % (sp[8:].capitalize(), lvl))


# ------------------------------------------------------------------ League (K28/K29)
def league(ed):
    # lobby: reset + door guard
    mp = 'IndigoPlateau_PokemonCenter_1F_Frlg'
    e = ed(mp)
    m = re.search(r'(?m)^%s_OnTransition::?\n((?:\t[^\n]*\n)+)' % mp, e.s)
    body = m.group(1)
    if 'KantoLeague_EventScript_Reset' not in body:
        e.s = e.s.replace(m.group(0), '%s_OnTransition::\n\tcall KantoLeague_EventScript_Reset\n%s' % (mp, body))
    lab = mp + '_EventScript_DoorGuard'
    e.kill(lab)
    e.add('''%(lab)s::
	lock
	faceplayer
	msgbox %(mp)s_Text_DoorGuard, MSGBOX_DEFAULT
	release
	end
''' % dict(lab=lab, mp=mp))
    e.text(mp + '_Text_DoorGuard', 'Guardia: i Superquattro | ti aspettano. Buona fortuna!')

    rooms = [('PokemonLeague_LoreleisRoom_Frlg', 'Lorelei', 'LORELEI', 0, 'PokemonLeague_LoreleisRoom_'),
             ('PokemonLeague_BrunosRoom_Frlg', 'Bruno', 'BRUNO', 1, 'PokemonLeague_BrunosRoom_'),
             ('PokemonLeague_AgathasRoom_Frlg', 'Agatha', 'AGATHA', 2, 'PokemonLeague_AgathasRoom_'),
             ('PokemonLeague_LancesRoom_Frlg', 'Lance', 'LANCE', 3, 'PokemonLeague_LancesRoom_')]
    for mp, name, up, n, pf in rooms:
        e = ed(mp)
        P = mp + '_'
        lab = e.obj_label(1)
        e.kill(lab)
        lance = name == 'Lance'
        if lance:
            e.add(ed.orig(mp, pf + 'Movement_WalkThroughCorridor', pf, P))
            e.add(ed.orig(mp, pf + 'EventScript_SetEntryClosed', pf, P))
            enter = '''	applymovement LOCALID_PLAYER, %(P)sMovement_WalkThroughCorridor
	waitmovement 0
	call %(P)sEventScript_SetEntryClosed
	playse SE_UNLOCK
	special DrawWholeMapView''' % dict(P=P)
            close = 'call %sEventScript_SetEntryClosed' % P
            opendoor, setopen = 'KantoLeague_EventScript_OpenDoorLance', 'KantoLeague_EventScript_SetDoorOpenLance'
        else:
            enter = '\tcall KantoLeague_EventScript_EnterRoom'
            close = 'call KantoLeague_EventScript_CloseEntry'
            opendoor, setopen = 'KantoLeague_EventScript_OpenDoor', 'KantoLeague_EventScript_SetDoorOpen'
        e.add('''%(P)sOnLoad::
	call_if_set FLAG_KANTO_DEFEATED_%(up)s, %(P)sEventScript_SetDoorOpen
	call_if_eq VAR_KANTO_LEAGUE, %(n1)d, %(P)sEventScript_CloseEntry
	end

%(P)sEventScript_SetDoorOpen::
	call %(setopen)s
	return

%(P)sEventScript_CloseEntry::
	%(close)s
	return

%(P)sOnWarp::
	map_script_2 VAR_TEMP_1, 0, %(P)sEventScript_TurnPlayerNorth
	.2byte 0

%(P)sEventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

%(P)sOnFrame::
	map_script_2 VAR_KANTO_LEAGUE, %(n)d, %(P)sEventScript_EnterRoom
	.2byte 0

%(P)sEventScript_EnterRoom::
	lockall
%(enter)s
	setvar VAR_KANTO_LEAGUE, %(n1)d
	releaseall
	end

%(lab)s::
	lock
	faceplayer
	goto_if_set FLAG_KANTO_DEFEATED_%(up)s, %(lab)sPostBattle
	msgbox %(P)sText_%(name)sIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_KANTO_ELITE_FOUR_%(up)s, %(P)sText_%(name)sDefeat
	setflag FLAG_KANTO_DEFEATED_%(up)s
	call %(opendoor)s
%(lab)sPostBattle::
	msgbox %(P)sText_%(name)sPostBattle, MSGBOX_DEFAULT
	release
	end
''' % dict(P=P, up=up, n=n, n1=n + 1, setopen=setopen, close=close, enter=enter, lab=lab, name=name, opendoor=opendoor))
        e.mapscript('MAP_SCRIPT_ON_LOAD', P + 'OnLoad')
        e.mapscript('MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE', P + 'OnWarp')
        e.mapscript('MAP_SCRIPT_ON_FRAME_TABLE', P + 'OnFrame')
        e.text(P + 'Text_%sIntro' % name, 'K28 %s: presentazione | (Superquattro)' % name)
        e.text(P + 'Text_%sDefeat' % name, 'K28 %s: alla sconfitta' % name)
        e.text(P + 'Text_%sPostBattle' % name, 'K28 %s: vai avanti' % name)

    # Champion room: talk-free scene on entering (like FRLG), then Oak, then Hall of Fame
    mp, pf = 'PokemonLeague_ChampionsRoom_Frlg', 'PokemonLeague_ChampionsRoom_'
    P = mp + '_'
    e = ed(mp)
    e.set_obj(1, script=P + 'EventScript_Blu')
    e.set_obj(2, script=P + 'EventScript_Oak')
    for l in ('Movement_PlayerEnter', 'Movement_PlayerWatchOakEnter', 'Movement_RivalWatchOakEnter',
              'Movement_OakEnter', 'Movement_OakExit', 'Movement_PlayerExit'):
        e.add(ed.orig(mp, pf + l, pf, P))
    e.add('''%(P)sOnWarp::
	map_script_2 VAR_TEMP_1, 0, %(P)sEventScript_TurnPlayerNorth
	.2byte 0

%(P)sEventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

%(P)sOnFrame::
	map_script_2 VAR_KANTO_LEAGUE, 4, %(P)sEventScript_EnterRoom
	.2byte 0

%(P)sEventScript_EnterRoom::
	lockall
	applymovement LOCALID_PLAYER, %(P)sMovement_PlayerEnter
	waitmovement 0
	delay 20
	msgbox %(P)sText_BluIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_KANTO_CHAMPION_BLU, %(P)sText_BluDefeat
	setvar VAR_KANTO_LEAGUE, 5
	msgbox %(P)sText_BluPostBattle, MSGBOX_DEFAULT
	playbgm MUS_RG_SLOW_PALLET, 0
	clearflag FLAG_KANTO_HIDE_OAK_IN_CHAMP_ROOM
	addobject 2
	msgbox %(P)sText_OakEnters, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, %(P)sMovement_PlayerWatchOakEnter
	applymovement 1, %(P)sMovement_RivalWatchOakEnter
	applymovement 2, %(P)sMovement_OakEnter
	waitmovement 0
	msgbox %(P)sText_OakScoldsBlu, MSGBOX_DEFAULT
	msgbox %(P)sText_OakComeWithMe, MSGBOX_DEFAULT
	closemessage
	applymovement 2, %(P)sMovement_OakExit
	applymovement LOCALID_PLAYER, %(P)sMovement_PlayerExit
	waitmovement 0
	setflag FLAG_KANTO_HIDE_OAK_IN_CHAMP_ROOM
	setvar VAR_TEMP_1, 1
	warp MAP_POKEMON_LEAGUE_HALL_OF_FAME, 5, 12
	waitstate
	releaseall
	end

%(P)sEventScript_Blu::
	msgbox %(P)sText_BluAfter, MSGBOX_NPC
	end

%(P)sEventScript_Oak::
	msgbox %(P)sText_OakEnters, MSGBOX_NPC
	end
''' % dict(P=P))
    e.mapscript('MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE', P + 'OnWarp')
    e.mapscript('MAP_SCRIPT_ON_FRAME_TABLE', P + 'OnFrame')
    e.text(P + 'Text_BluIntro', 'K28 Blu: sarai il primo | a perdere qui!')
    e.text(P + 'Text_BluDefeat', 'K28 Blu: alla sconfitta')
    e.text(P + 'Text_BluPostBattle', 'K28 Blu: ...Ok. Il primo in tutto.')
    e.text(P + 'Text_OakEnters', 'K28 Oak entra: {PLAYER}!')
    e.text(P + 'Text_OakScoldsBlu', 'K28 Oak: Blu, hai dimenticato | il rispetto')
    e.text(P + 'Text_OakComeWithMe', 'K28 Oak: vieni con me, | {PLAYER}')
    e.text(P + 'Text_BluAfter', 'Blu, dopo la sconfitta')

    # Hall of Fame (Act 2 ending, simple) -> credits scene at the Indigo Plateau exterior (K29)
    mp = 'PokemonLeague_HallOfFame_Frlg'
    P = mp + '_'
    e = ed(mp)
    e.set_obj(1, script=P + 'EventScript_Oak')
    e.add('''%(P)sOnFrame::
	map_script_2 VAR_KANTO_LEAGUE, 5, %(P)sEventScript_Ceremony
	.2byte 0

%(P)sEventScript_Ceremony::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkUp4
	waitmovement 0
	msgbox %(P)sText_OakCongratulations, MSGBOX_DEFAULT
	closemessage
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	msgbox %(P)sText_Registered, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_KANTO_STORY_CHAMPION
	setvar VAR_KANTO_LEAGUE, 6
	clearflag FLAG_KANTO_HIDE_CREDITS_RIVAL
	clearflag FLAG_KANTO_HIDE_CREDITS_OAK
	clearflag FLAG_KANTO_HIDE_CREDITS_ARIANNA
	fadescreen FADE_TO_BLACK
	warp MAP_INDIGO_PLATEAU_EXTERIOR, 11, 9
	waitstate
	releaseall
	end

%(P)sEventScript_Oak::
	msgbox %(P)sText_OakCongratulations, MSGBOX_NPC
	end
''' % dict(P=P))
    e.mapscript('MAP_SCRIPT_ON_FRAME_TABLE', P + 'OnFrame')
    e.text(P + 'Text_OakCongratulations', 'K28 Oak: congratulazioni, | Campione di Kanto!')
    e.text(P + 'Text_Registered', 'I tuoi Pokemon entrano | nella Sala d\'Onore di Kanto!')

    mp = 'IndigoPlateau_Exterior_Frlg'
    P = mp + '_'
    e = ed(mp)
    e.set_obj(1, script=P + 'EventScript_Blu', x=10, y=8)
    e.set_obj(2, script=P + 'EventScript_Oak', x=12, y=8)
    e.add_obj('LOCALID_KANTO_CREDITS_ARIANNA', 'OBJ_EVENT_GFX_MAY_NORMAL', 11, 8, P + 'EventScript_Arianna',
              'FLAG_KANTO_HIDE_CREDITS_ARIANNA')
    e.add('''%(P)sOnFrame::
	map_script_2 VAR_KANTO_LEAGUE, 6, %(P)sEventScript_Ending
	.2byte 0

%(P)sEventScript_Ending::
	lockall
	turnobject LOCALID_PLAYER, DIR_NORTH
	msgbox %(P)sText_EndingRift, MSGBOX_DEFAULT
	msgbox %(P)sText_EndingOak, MSGBOX_DEFAULT
	msgbox %(P)sText_EndingArianna, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	msgbox %(P)sText_EndOfAct2, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_KANTO_HIDE_CREDITS_RIVAL
	setflag FLAG_KANTO_HIDE_CREDITS_OAK
	setflag FLAG_KANTO_HIDE_CREDITS_ARIANNA
	removeobject 1
	removeobject 2
	removeobject LOCALID_KANTO_CREDITS_ARIANNA
	setvar VAR_KANTO_LEAGUE, 7
	setrespawn HEAL_LOCATION_INDIGO_PLATEAU
	fadescreen FADE_FROM_BLACK
	releaseall
	end

%(P)sEventScript_Blu::
%(P)sEventScript_Oak::
%(P)sEventScript_Arianna::
	end
''' % dict(P=P))
    e.mapscript('MAP_SCRIPT_ON_FRAME_TABLE', P + 'OnFrame')
    e.text(P + 'Text_EndingRift', 'K29 Il Varco si apre in un | secondo corridoio a ovest (Mew)')
    e.text(P + 'Text_EndingOak', 'K29 Oak: oltre quelle montagne... | un mondo chiamato Johto.')
    e.text(P + 'Text_EndingArianna', 'K29 Arianna: stavolta passo | prima io. Segnato!')
    e.text(P + 'Text_EndOfAct2', 'Fine dell\'Atto 2. | Il multiverso vi aspetta.')


# ------------------------------------------------------------------ towns: visited flags + taxi NPCs
def towns(ed):
    for suf, town, pc, name in TOWNS:
        if pc:
            e = ed(pc)
            m = re.search(r'(?m)^%s_OnTransition::?\n(?:\t(?!setrespawn)[^\n]*\n)*\tsetrespawn [^\n]*\n' % pc, e.s)
            assert m, pc
            if 'FLAG_KANTO_VISITED_%s' % suf not in e.s:
                e.s = e.s[:m.end()] + '\tsetflag FLAG_KANTO_VISITED_%s @ KANTO_V2 Volo Taxi\n' % suf + e.s[m.end():]
        else:
            e = ed(town)
            e.add('%s_OnTransition::\n\tsetflag FLAG_KANTO_VISITED_%s\n\tend\n' % (town, suf))
            e.mapscript('MAP_SCRIPT_ON_TRANSITION', town + '_OnTransition')
    for pc in TAXI_PCS:
        e = ed(pc)
        short = pc.replace('_PokemonCenter_1F_Frlg', '').upper()
        lay = Layout(pc)
        taken = {(o['x'], o['y']) for o in e.j['object_events'] if o.get('local_id') != 'LOCALID_KANTO_TAXI_' + short}
        taken |= {(w['x'], w['y']) for w in e.j['warp_events']}
        for x, y in ((13, 3), (12, 3), (14, 3), (13, 4), (12, 4), (2, 4), (3, 4), (1, 4)):
            if (x, y) not in taken and lay.tile(x, y)[0] == 0:
                break
        else:
            raise SystemExit('no free tile for the taxi in ' + pc)
        e.add_obj('LOCALID_KANTO_TAXI_' + short, 'OBJ_EVENT_GFX_COOLTRAINER_M',
                  x, y, 'KantoTaxi_EventScript_Taxi', '0', 'MOVEMENT_TYPE_FACE_DOWN')


def main():
    global TREE
    ap = argparse.ArgumentParser()
    ap.add_argument('--tree', default=TREE)
    TREE = ap.parse_args().tree
    ed = Editor()
    sh = shared(ed)
    gyms(ed)
    story(ed)
    league(ed)
    towns(ed)
    ch = [mp for mp, e in sorted(ed.maps.items()) if e.save()]
    wr(os.path.join(TREE, 'data/scripts/kanto_story.inc'), sh)
    # include + init hook
    es = os.path.join(TREE, 'data/event_scripts.s')
    s = rd(es)
    inc = '\t.include "data/scripts/kanto_story.inc" @ KANTO_V2\n'
    if inc not in s:
        s = s.replace('\t.include "data/kanto_port_scripts.inc"\n', '\t.include "data/kanto_port_scripts.inc"\n' + inc)
        assert inc in s
        wr(es, s)
    lp = os.path.join(TREE, 'data/layouts/layouts.json')
    lj = json.loads(rd(lp))
    for l in lj['layouts']:
        if l.get('id') in ('LAYOUT_SEAFOAM_ISLANDS_B3F_CURRENT_STOPPED', 'LAYOUT_SEAFOAM_ISLANDS_B4F_CURRENT_STOPPED'):
            l['include_in_emerald'] = True
    wr(lp, json.dumps(lj, indent=2, ensure_ascii=False) + '\n')
    kt = os.path.join(TREE, 'data/scripts/kanto_travel.inc')
    s = rd(kt)
    if 'KantoStory_EventScript_Init' not in s:
        s = s.replace('\tcall KantoPort_EventScript_InitFlags\n',
                      '\tcall KantoPort_EventScript_InitFlags\n\tcall KantoStory_EventScript_Init @ KANTO_V2\n')
        wr(kt, s)
    print('maps changed: %d (%d edited)' % (len(ch), len(ed.maps)))


if __name__ == '__main__':
    main()
