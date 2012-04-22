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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <X11//Xutil.h>

KeySym KeyCodeToKeySym(Display * display, KeyCode keycode, unsigned int event_mask) {
	KeySym keysym = NoSymbol;

	int min_keycode, max_keycode, keysyms_per_keycode;//, i = 0;
	XDisplayKeycodes(display, &min_keycode, &max_keycode);

	KeySym * keyboard_map = XGetKeyboardMapping(display, min_keycode, (max_keycode - min_keycode + 1), &keysyms_per_keycode);
	if (keyboard_map) {
			XModifierKeymap * modifier_map = XGetModifierMapping(display);
			if (modifier_map) {
				/* The Lock modifier is interpreted as CapsLock when the KeySym
				 * named XK_Caps_Lock is attached to some KeyCode and that KeyCode
				 * is attached to the Lock modifier. The Lock modifier is
				 * interpreted as ShiftLock when the KeySym named XK_Shift_Lock is
				 * attached to some KeyCode and that KeyCode is attached to the Lock
				 * modifier. If the Lock modifier could be interpreted as both
				 * CapsLock and ShiftLock, the CapsLock interpretation is used.
				 */
				 
				KeyCode caps_lock = XKeysymToKeycode(display, XK_Caps_Lock);
				KeyCode shift_lock = XKeysymToKeycode(display, XK_Shift_Lock);
				Bool is_caps_lock = false, is_shift_lock = false;
				keysyms_per_keycode--;
				
				//Loop over the modfer map to find out if/where shift and caps locks are set
				for (int j = LockMapIndex; j < LockMapIndex + modifier_map->max_keypermod && !is_caps_lock; j++) {
					if (caps_lock != 0 && modifier_map->modifiermap[j] == caps_lock) {
						is_caps_lock = true;
						is_shift_lock = false;
					}
					else if (shift_lock != 0 && modifier_map->modifiermap[j] == shift_lock) {
						is_shift_lock = true;
					}
				}
				
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
				
				XFree(modifier_map);
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetChar(): Unable to get modifier mapping table.\n");
			}
			#endif

			XFree(keyboard_map);
	}
	#ifdef DEBUG
	else {
		fprintf(stderr, "GetChar(): Unable to get keyboard mapping table.\n");
	}
	#endif
	
	return keysym;
}


int main(int argc, const char * argv[]) {
	Display * display;

	//Try to attach to the default X11 display.
	display = XOpenDisplay(NULL);
	if(display == NULL) {
		printf("Error: Could not open display!\n");
		return EXIT_FAILURE;
	}
	
	KeyCode keycode = 56; // b
	unsigned int event_mask = ShiftMask | LockMask;
	KeySym keysym = KeyCodeToKeySym(display, keycode, event_mask);

	printf("KeySym: %s\n", XKeysymToString(keysym));
	
	//Close the connection to the selected X11 display.
	XCloseDisplay(display);

	return EXIT_SUCCESS;
}
