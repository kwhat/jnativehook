/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <X11/Xlib.h>

#ifdef USE_DEBUG
#include <stdio.h>
#endif

#ifdef USE_XKB
#include <X11/XKBlib.h>
static XkbDescPtr keyboard_map;
static bool is_evdev = false;
#else
#include <X11/Xutil.h>
static KeySym *keyboard_map;
static int keysym_per_keycode;
static bool is_caps_lock = false, is_shift_lock = false;
#endif

// Unicode-Remapse the NativeHelpers display.
extern Display *disp;

/*
 * This table is taken from QEMU x_keymap.c, under the terms:
 *
 * Copyright (c) 2003 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
static const uint8_t xfree86_keycode_to_scancode_table[61] = {
	0xC7,	// 97  Home
	0xC8,	// 98  Up
	0xC9,	// 99  PgUp
	0xCb,	// 100 Left
	0x4C,	// 101 KP-5
	0xCD,	// 102 Right
	0xCF,	// 103 End
	0xD0,	// 104 Down
	0xD1,	// 105 PgDn
	0xD2,	// 106 Ins
	0xD3,	// 107 Del
	0x9C,	// 108 Enter
	0x9D,	// 109 Ctrl-R
	0x00,	// 110 Pause
	0xB7,	// 111 Print
	0xB5,	// 112 Divide
	0xB8,	// 113 Alt-R
	0xC6,	// 114 Break
	0x00,	// 115
	0x00,	// 116
	0x00,	// 117
	0x00,	// 118
	0x00,	// 119
	0x00,	// 120
	0x00,	// 121
	0x00,	// 122
	0x00,	// 123
	0x00,	// 124
	0x00,	// 125
	0x00,	// 126
	0x00,	// 127
	0x00,	// 128
	0x79,	// 129 Henkan
	0x00,	// 130
	0x7b,	// 131 Muhenkan
	0x00,	// 132
	0x7d,	// 133 Yen
	0x00,	// 134
	0x00,	// 135
	0x47,	// 136 KP_7
	0x48,	// 137 KP_8
	0x49,	// 138 KP_9
	0x4b,	// 139 KP_4
	0x4c,	// 140 KP_5
	0x4d,	// 141 KP_6
	0x4f,	// 142 KP_1
	0x50,	// 143 KP_2
	0x51,	// 144 KP_3
	0x52,	// 145 KP_0
	0x53,	// 146 KP_.
	0x47,	// 147 KP_HOME
	0x48,	// 148 KP_UP
	0x49,	// 149 KP_PgUp
	0x4b,	// 150 KP_Left
	0x4c,	// 151 KP_
	0x4d,	// 152 KP_Right
	0x4f,	// 153 KP_End
	0x50,	// 154 KP_Down
	0x51,	// 155 KP_PgDn
	0x52,	// 156 KP_Ins
	0x53,	// 157 KP_Del
};

#ifdef USE_XKB
/* This table is generated based off the xfree86 -> scancode mapping above
 * and the keycode mappings in /usr/share/X11/xkb/keycodes/evdev
 * and  /usr/share/X11/xkb/keycodes/xfree86
 */
static const uint8_t evdev_keycode_to_scancode_table[61] = {
	0x00,	// 97 EVDEV - RO   ("Internet" Keyboards)
	0x00,	// 98 EVDEV - KATA (Katakana)
	0x00,	// 99 EVDEV - HIRA (Hiragana)
	0x79,	// 100 EVDEV - HENK (Henkan)
	0x70,	// 101 EVDEV - HKTG (Hiragana/Katakana toggle)
	0x7B,	// 102 EVDEV - MUHE (Muhenkan)
	0x00,	// 103 EVDEV - JPCM (KPJPComma)
	0x9C,	// 104 KPEN
	0x9D,	// 105 RCTL
	0xB5,	// 106 KPDV
	0xB7,	// 107 PRSC
	0xB8,	// 108 RALT
	0x00,	// 109 EVDEV - LNFD ("Internet" Keyboards)
	0xC7,	// 110 HOME
	0xC8,	// 111 UP
	0xC9,	// 112 PGUP
	0xCB,	// 113 LEFT
	0xCD,	// 114 RGHT
	0xCF,	// 115 END
	0xD0,	// 116 DOWN
	0xD1,	// 117 PGDN
	0xD2,	// 118 INS
	0xD3,	// 119 DELE
	0x00,	// 120 EVDEV - I120 ("Internet" Keyboards)
	0x00,	// 121 EVDEV - MUTE
	0x00,	// 122 EVDEV - VOL-
	0x00,	// 123 EVDEV - VOL
	0x00,	// 124 EVDEV - POWR
	0x00,	// 125 EVDEV - KPEQ
	0x00,	// 126 EVDEV - I126 ("Internet" Keyboards)
	0x00,	// 127 EVDEV - PAUS
	0x00,	// 128 EVDEV - ????
	0x00,	// 129 EVDEV - I129 ("Internet" Keyboards)
	0xF1,	// 130 EVDEV - HNGL (Korean Hangul Latin toggle)
	0xF2,	// 131 EVDEV - HJCV (Korean Hangul Hanja toggle)
	0x7D,	// 132 AE13 (Yen)
	0xDB,	// 133 EVDEV - LWIN
	0xDC,	// 134 EVDEV - RWIN
	0xDD,	// 135 EVDEV - MENU
	0x00,	// 136 EVDEV - STOP
	0x00,	// 137 EVDEV - AGAI
	0x00,	// 138 EVDEV - PROP
	0x00,	// 139 EVDEV - UNDO
	0x00,	// 140 EVDEV - FRNT
	0x00,	// 141 EVDEV - COPY
	0x00,	// 142 EVDEV - OPEN
	0x00,	// 143 EVDEV - PAST
	0x00,	// 144 EVDEV - FIND
	0x00,	// 145 EVDEV - CUT
	0x00,	// 146 EVDEV - HELP
	0x00,	// 147 EVDEV - I147
	0x00,	// 148 EVDEV - I148
	0x00,	// 149 EVDEV - I149
	0x00,	// 150 EVDEV - I150
	0x00,	// 151 EVDEV - I151
	0x00,	// 152 EVDEV - I152
	0x00,	// 153 EVDEV - I153
	0x00,	// 154 EVDEV - I154
	0x00,	// 155 EVDEV - I156
	0x00,	// 156 EVDEV - I157
	0x00,	// 157 EVDEV - I158
};
#endif


/* The following code is based on vncdisplaykeymap.c under the terms:
 *
 * Copyright (C) 2008  Anthony Liguori <anthony codemonkey ws>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2 as
 * published by the Free Software Foundation.
 */
uint16_t keycode_to_scancode(KeyCode keycode) {
	uint16_t scancode = 0x00;

	//if (keycode == GDK_Pause) {
	//	return VKC_PAUSE;
	//}

	if (keycode < 9) {
		scancode = 0x00;
	}
	if (keycode < 97) {
		// Simple offset of 8
		scancode = keycode - 8;
	}
	else if (keycode < 158) {
		#ifdef USE_XKB
		if (is_evdev) {
			scancode = evdev_keycode_to_scancode_table[keycode - 97];
		}
		else {
			scancode = xfree86_keycode_to_scancode_table[keycode - 97];
		}
		#else
		scancode = xfree86_keycode_to_scancode_table[keycode - 97];
		#endif
	}
	else if (keycode == 208) {
		// Hiragana_Katakana
		scancode = 0x70;
	}
	else if (keycode == 211) {
		// Backslash
		scancode = 0x73;
	}

	return scancode;
}

// Faster more flexible alternative to XKeycodeToKeysym...
KeySym keycode_to_keysym(KeyCode keycode, unsigned int modifier_mask) {
	KeySym keysym = NoSymbol;

	#ifdef USE_XKB
	if (keyboard_map != NULL) {
		// What is diff between XkbKeyGroupInfo and XkbKeyNumGroups?
		unsigned char info = XkbKeyGroupInfo(keyboard_map, keycode);
		unsigned int num_groups = XkbKeyNumGroups(keyboard_map, keycode);

		// Get the group.
		unsigned int group = 0x0000;
		switch (XkbOutOfRangeGroupAction(info)) {
			case XkbRedirectIntoRange:
				/* If the RedirectIntoRange flag is set, the four least significant
				 * bits of the groups wrap control specify the index of a group to
				 * which all illegal groups correspond. If the specified group is
				 * also out of range, all illegal groups map to Group1.
				 */
				group = XkbOutOfRangeGroupInfo(info);
				if (group >= num_groups) {
					group = 0;
				}
				break;

			case XkbClampIntoRange:
				/* If the ClampIntoRange flag is set, out-of-range groups correspond
				 * to the nearest legal group. Effective groups larger than the
				 * highest supported group are mapped to the highest supported group;
				 * effective groups less than Group1 are mapped to Group1 . For
				 * example, a key with two groups of symbols uses Group2 type and
				 * symbols if the global effective group is either Group3 or Group4.
				 */
				group = num_groups - 1;
				break;

			case XkbWrapIntoRange:
				/* If neither flag is set, group is wrapped into range using integer
				 * modulus. For example, a key with two groups of symbols for which
				 * groups wrap uses Group1 symbols if the global effective group is
				 * Group3 or Group2 symbols if the global effective group is Group4.
				 */
			default:
				if (num_groups != 0) {
					group %= num_groups;
				}
				break;
		}

		XkbKeyTypePtr key_type = XkbKeyKeyType(keyboard_map, keycode, group);
		unsigned int active_mods = modifier_mask & key_type->mods.mask;

		int i, level = 0;
		for (i = 0; i < key_type->map_count; i++) {
			if (key_type->map[i].active && key_type->map[i].mods.mask == active_mods) {
				level = key_type->map[i].level;
			}
		}

		keysym = XkbKeySymEntry(keyboard_map, keycode, level, group);
	}
	#else
	if (keyboard_map != NULL) {
		if (modifier_mask & Mod2Mask &&
				((keyboard_map[keycode *keysym_per_keycode + 1] >= 0xFF80 && keyboard_map[keycode *keysym_per_keycode + 1] <= 0xFFBD) ||
				(keyboard_map[keycode *keysym_per_keycode + 1] >= 0x11000000 && keyboard_map[keycode *keysym_per_keycode + 1] <= 0x1100FFFF))
			) {

			/* If the numlock modifier is on and the second KeySym is a keypad
			 * KeySym.  In this case, if the Shift modifier is on, or if the
			 * Lock modifier is on and is interpreted as ShiftLock, then the
			 * first KeySym is used, otherwise the second KeySym is used.
			 *
			 * The standard KeySyms with the prefix ``XK_KP_'' in their name are
			 * called keypad KeySyms; these are KeySyms with numeric value in
			 * the hexadecimal range 0xFF80 to 0xFFBD inclusive. In addition,
			 * vendor-specific KeySyms in the hexadecimal range 0x11000000 to
			 * 0x1100FFFF are also keypad KeySyms.
			 */


			 /* The numlock modifier is on and the second KeySym is a keypad
			  * KeySym. In this case, if the Shift modifier is on, or if the
			  * Lock modifier is on and is interpreted as ShiftLock, then the
			  * first KeySym is used, otherwise the second KeySym is used.
			  */
			if (modifier_mask & ShiftMask || (modifier_mask & LockMask && is_shift_lock)) {
				// i = 0
				keysym = keyboard_map[keycode *keysym_per_keycode];
			}
			else {
				// i = 1
				keysym = keyboard_map[keycode *keysym_per_keycode + 1];
			}
		}
		else if (modifier_mask ^ ShiftMask && modifier_mask ^ LockMask) {
			/* The Shift and Lock modifiers are both off. In this case,
			 * the first KeySym is used.
			 */
			// index = 0
			keysym = keyboard_map[keycode *keysym_per_keycode];
		}
		else if (modifier_mask ^ ShiftMask && modifier_mask & LockMask && is_caps_lock) {
			/* The Shift modifier is off, and the Lock modifier is on
			 * and is interpreted as CapsLock. In this case, the first
			 * KeySym is used, but if that KeySym is lowercase
			 * alphabetic, then the corresponding uppercase KeySym is
			 * used instead.
			 */
			// index = 0;
			keysym = keyboard_map[keycode *keysym_per_keycode];

			if (keysym >= 'a' && keysym <= 'z') {
				// keysym is an alpha char.
				KeySym lower_keysym, upper_keysym;
				XConvertCase(keysym, &lower_keysym, &upper_keysym);
				keysym = upper_keysym;
			}
		}
		else if (modifier_mask & ShiftMask && modifier_mask & LockMask && is_caps_lock) {
			/* The Shift modifier is on, and the Lock modifier is on and
			 * is interpreted as CapsLock. In this case, the second
			 * KeySym is used, but if that KeySym is lowercase
			 * alphabetic, then the corresponding uppercase KeySym is
			 * used instead.
			 */
			// index = 1
			keysym = keyboard_map[keycode *keysym_per_keycode + 1];

			if (keysym >= 'A' && keysym <= 'Z') {
				// keysym is an alpha char.
				KeySym lower_keysym, upper_keysym;
				XConvertCase(keysym, &lower_keysym, &upper_keysym);
				keysym = lower_keysym;
			}
		}
		else if (modifier_mask & ShiftMask || (modifier_mask & LockMask && is_shift_lock) || modifier_mask & (ShiftMask + LockMask)) {
			/* The Shift modifier is on, or the Lock modifier is on and
			 * is interpreted as ShiftLock, or both. In this case, the
			 * second KeySym is used.
			 */
			// index = 1
			keysym = keyboard_map[keycode *keysym_per_keycode + 1];
		}
		#ifdef USE_DEBUG
		else {
			fprintf(stderr, "keycode_to_keysym(): Unable to determine the KeySym index.\n");
		}
		#endif
	}
	#endif

	return keysym;
}

void load_input_helper() {
	#ifdef USE_XKB
	/* The following code is based on vncdisplaykeymap.c under the terms:
	 *
	 * Copyright (C) 2008  Anthony Liguori <anthony codemonkey ws>
	 *
	 * This program is free software; you can redistribute it and/or modify
	 * it under the terms of the GNU Lesser General Public License version 2 as
	 * published by the Free Software Foundation.
	 */
	XkbDescPtr desc = XkbGetKeyboard(disp, XkbGBN_AllComponentsMask, XkbUseCoreKbd);
	if (desc != NULL && desc->names != NULL) {
		const char *layout_name = XGetAtomName(disp, desc->names->keycodes);
		#ifdef USE_DEBUG
		fprintf(stdout, "keycode_to_keysym(): Found keycode atom '%s' (%i).\n", layout_name, (unsigned int) desc->names->keycodes);
		#endif

		#ifdef USE_DEBUG
		const char *prefix_xfree86 = "xfree86_";
		#endif
		const char *prefix_evdev = "evdev_";
		if (strncmp(layout_name, prefix_evdev, strlen(prefix_evdev))) {
			is_evdev = true;
		}
		#ifdef USE_DEBUG
		else if (layout_name == NULL) {
			fprintf(stderr, "keycode_to_keysym(): Unable to determine the keycode name.\n");
		}
		else if (!strncmp(layout_name, prefix_xfree86, strlen(prefix_xfree86))) {
			fprintf(stderr, "keycode_to_keysym(): Unknown keycode name '%s', please file a bug report.\n", layout_name);
		}
		#endif

		XkbFreeClientMap(desc, XkbGBN_AllComponentsMask, True);
	}
	#ifdef USE_DEBUG
	else {
		fprintf(stderr, "keycode_to_keysym(): XkbGetKeyboard failed to locate a valid keyboard!\n");
	}
	#endif

	// Get the map.
	keyboard_map = XkbGetMap(disp, XkbAllClientInfoMask, XkbUseCoreKbd);
	#else
	// No known alternative to determine scancode mapping, assume XFree86!
	#pragma message("*** Warning: XKB support is required to accuratly determine keyboard scancodes!")
	#pragma message("... Assuming XFree86 keyboard layout.")

	int minKeyCode, maxKeyCode;
	XDisplayKeycodes(disp, &minKeyCode, &maxKeyCode);

	keyboard_map = XGetKeyboardMapping(disp, minKeyCode, (maxKeyCode - minKeyCode + 1), &keysym_per_keycode);
	if (keyboard_map) {
		XModifierKeymap *modifierMap = XGetModifierMapping(disp);

		if (modifierMap) {
			/* The Lock modifier is interpreted as CapsLock when the KeySym
			 * named XK_Caps_Lock is attached to some KeyCode and that KeyCode
			 * is attached to the Lock modifier. The Lock modifier is
			 * interpreted as ShiftLock when the KeySym named XK_Shift_Lock is
			 * attached to some KeyCode and that KeyCode is attached to the Lock
			 * modifier. If the Lock modifier could be interpreted as both
			 * CapsLock and ShiftLock, the CapsLock interpretation is used.
			 */

			KeyCode capsLock = XKeysymToKeycode(disp, XK_Caps_Lock);
			KeyCode shiftLock = XKeysymToKeycode(disp, XK_Shift_Lock);
			keysym_per_keycode--;

			// Loop over the modifier map to find out if/where shift and caps locks are set.
			for (int i = LockMapIndex; i < LockMapIndex + modifierMap->max_keypermod && !is_caps_lock; i++) {
				if (capsLock != 0 && modifierMap->modifiermap[i] == capsLock) {
					is_caps_lock = true;
					is_shift_lock = false;
				}
				else if (shiftLock != 0 && modifierMap->modifiermap[i] == shiftLock) {
					is_shift_lock = true;
				}
			}

			XFree(modifierMap);
		}
		else {
			XFree(keyboard_map);

			#ifdef USE_DEBUG
			fprintf(stderr, "Initialize(): Unable to get modifier mapping table.\n");
			#endif
		}
	}
	#ifdef USE_DEBUG
	else {
		fprintf(stderr, "Initialize(): Unable to get keyboard mapping table.\n");
	}
	#endif
	#endif
}

void unload_input_helper() {
	if (keyboard_map) {
		#ifdef USE_XKB
		XkbFreeClientMap(keyboard_map, XkbAllClientInfoMask, true);
		is_evdev = false;
		#else
		XFree(keyboard_map);
		#endif
	}
}
