/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>

#ifdef XKB
#include <X11/XKBlib.h>
static XkbDescPtr keyboard_map;
#else
#include <X11//Xutil.h>
static KeySym * keyboard_map;
static int keysyms_per_keycode;
static Bool is_caps_lock = false, is_shift_lock = false;
#endif

//Use the NativeHelpers display
extern Display * disp;

//FIXME I dont think this is correct or that it is needed.
unsigned short KeyCodeToScanCode(KeyCode keycode) {
	//if (keycode == 0xFF13) //GDK_Pause
	//	return 0x100//VKC_PAUSE;

	if (keycode < 9)
		return 0;

	if (keycode < 97)
		return keycode - 8; /* just an offset */

	if (keycode < 158)
		return keyboard_map[keycode - 97];

	if (keycode == 208) /* Hiragana_Katakana */
		return 0x70;

	if (keycode == 211) /* backslash */
		return 0x73;

	return 0;
}


//Faster more flexible alternative to XKeycodeToKeysym...
KeySym KeyCodeToKeySym(KeyCode keycode, unsigned int event_mask) {
	KeySym keysym = NoSymbol;

	#ifdef XKB
	if (keyboard_map) {
		//What is diff between XkbKeyGroupInfo and XkbKeyNumGroups?
		unsigned char info = XkbKeyGroupInfo(keyboard_map, keycode);
		unsigned int num_groups = XkbKeyNumGroups(keyboard_map, keycode);

		//Get the group
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
		unsigned int active_mods = event_mask & key_type->mods.mask;

		int i, level = 0;
		for (i = 0; i < key_type->map_count; i++) {
			if (key_type->map[i].active && key_type->map[i].mods.mask == active_mods) {
				level = key_type->map[i].level;
			}
		}

		keysym = XkbKeySymEntry(keyboard_map, keycode, level, group);
	}
	#else
	if (keyboard_map) {
		if (event_mask & Mod2Mask &&
				((keyboard_map[keycode * keysyms_per_keycode + 1] >= 0xFF80 && keyboard_map[keycode * keysyms_per_keycode + 1] <= 0xFFBD) ||
				(keyboard_map[keycode * keysyms_per_keycode + 1] >= 0x11000000 && keyboard_map[keycode * keysyms_per_keycode + 1] <= 0x1100FFFF))
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
			if (event_mask & ShiftMask || (event_mask & LockMask && is_shift_lock)) {
				//i = 0;
				keysym = keyboard_map[keycode * keysyms_per_keycode];
			}
			else {
				//i = 1;
				keysym = keyboard_map[keycode * keysyms_per_keycode + 1];
			}
		}
		else if (event_mask ^ ShiftMask && event_mask ^ LockMask) {
			/* The Shift and Lock modifiers are both off. In this case,
			 * the first KeySym is used.
			 */
			//i = 0;
			keysym = keyboard_map[keycode * keysyms_per_keycode];
		}
		else if (event_mask ^ ShiftMask && event_mask & LockMask && is_caps_lock) {
			/* The Shift modifier is off, and the Lock modifier is on
			 * and is interpreted as CapsLock. In this case, the first
			 * KeySym is used, but if that KeySym is lowercase
			 * alphabetic, then the corresponding uppercase KeySym is
			 * used instead.
			 */
			//i = 0;
			keysym = keyboard_map[keycode * keysyms_per_keycode];

			//FIXME if alpha
			KeySym lower_keysym, upper_keysym;
			XConvertCase(keysym, &lower_keysym, &upper_keysym);
			keysym = upper_keysym;
		}
		else if (event_mask & ShiftMask && event_mask & LockMask && is_caps_lock) {
			/* The Shift modifier is on, and the Lock modifier is on and
			 * is interpreted as CapsLock. In this case, the second
			 * KeySym is used, but if that KeySym is lowercase
			 * alphabetic, then the corresponding uppercase KeySym is
			 * used instead.
			 */
			//i = 1;
			keysym = keyboard_map[keycode * keysyms_per_keycode + 1];

			//FIXME if alpha
			KeySym lower_keysym, upper_keysym;
			XConvertCase(keysym, &lower_keysym, &upper_keysym);
			keysym = lower_keysym;
		}
		else if (event_mask & ShiftMask || (event_mask & LockMask && is_shift_lock) || event_mask & (ShiftMask + LockMask)) {
			/* The Shift modifier is on, or the Lock modifier is on and
			 * is interpreted as ShiftLock, or both. In this case, the
			 * second KeySym is used.
			 */
			//i = 1;
			keysym = keyboard_map[keycode * keysyms_per_keycode + 1];
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "GetChar(): Unable to determine the KeySym index.\n");
		}
		#endif
	}
	#endif

	return keysym;
}


void KeyboardHelperInit() {
	#ifdef XKB
	//Get the map
	keyboard_map = XkbGetMap(disp, XkbAllClientInfoMask, XkbUseCoreKbd);
	#else
	int min_keycode, max_keycode;
	XDisplayKeycodes(disp, &min_keycode, &max_keycode);

	keyboard_map = XGetKeyboardMapping(disp, min_keycode, (max_keycode - min_keycode + 1), &keysyms_per_keycode);
	if (keyboard_map) {
		XModifierKeymap * modifier_map = XGetModifierMapping(disp);

		if (modifier_map) {
			/* The Lock modifier is interpreted as CapsLock when the KeySym
			 * named XK_Caps_Lock is attached to some KeyCode and that KeyCode
			 * is attached to the Lock modifier. The Lock modifier is
			 * interpreted as ShiftLock when the KeySym named XK_Shift_Lock is
			 * attached to some KeyCode and that KeyCode is attached to the Lock
			 * modifier. If the Lock modifier could be interpreted as both
			 * CapsLock and ShiftLock, the CapsLock interpretation is used.
			 */

			KeyCode caps_lock = XKeysymToKeycode(disp, XK_Caps_Lock);
			KeyCode shift_lock = XKeysymToKeycode(disp, XK_Shift_Lock);
			keysyms_per_keycode--;

			//Loop over the modfer map to find out if/where shift and caps locks are set
			for (int i = LockMapIndex; i < LockMapIndex + modifier_map->max_keypermod && !is_caps_lock; i++) {
				if (caps_lock != 0 && modifier_map->modifiermap[i] == caps_lock) {
					is_caps_lock = true;
					is_shift_lock = false;
				}
				else if (shift_lock != 0 && modifier_map->modifiermap[i] == shift_lock) {
					is_shift_lock = true;
				}
			}

			XFree(modifier_map);
		}
		else {
			XFree(keyboard_map);

			#ifdef DEBUG
			fprintf(stderr, "Initialize(): Unable to get modifier mapping table.\n");
			#endif
		}
	}
	#ifdef DEBUG
	else {
		fprintf(stderr, "Initialize(): Unable to get keyboard mapping table.\n");
	}
	#endif
	#endif
}

void KeyboardHelperCleanup() {
	if (keyboard_map) {
		#ifdef XKB
		XkbFreeClientMap(keyboard_map, XkbAllClientInfoMask, true);
		#else
		XFree(keyboard_map);
		#endif
	}
}
