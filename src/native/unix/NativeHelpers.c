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
#include <stdio.h>
#include <X11/Xlib.h>
#include "XEventModifiers.h"

#ifdef XKB
#include <X11/XKBlib.h>
#endif

#ifdef XF86MISC
#include <X11/extensions/xf86misc.h>
#include <X11/extensions/xf86mscstr.h>
#endif

#ifdef XT
#include <X11/Intrinsic.h>
#endif

static Display * disp;
#ifdef XT
static XtAppContext app_context;
#endif

long GetAutoRepeatRate() {
	bool successful = false;
	long value = -1;
	unsigned int kb_delay = 0, kb_rate = 0;

	#ifdef XKB
	//Attempt to acquire the keyboard auto repeat rate using the XKB extension if available.
	if (!successful) {
		successful = XkbGetAutoRepeatRate(disp, XkbUseCoreKbd, &kb_delay, &kb_rate);
	}
	#endif

	#ifdef XF86MISC
	//Fallback to the XF86 Misc extension if available and other efforts failed.
	if (!successful) {
		XF86MiscKbdSettings kb_info;
		successful = (bool) XF86MiscGetKbdSettings(disp, &kb_info);
		if (successful) {
			kb_delay = (unsigned int) kbdinfo.delay;
			kb_rate = (unsigned int) kbdinfo.rate;
		}
	}
	#endif

	if (successful) {
		value = (long) kb_delay;
	}

	(void) kb_rate;
	return value;
}

long GetAutoRepeatDelay() {
	bool successful = false;
	long value = -1;
	unsigned int kb_delay = 0, kb_rate = 0;

	#ifdef XKB
	//Attempt to acquire the keyboard auto repeat rate using the XKB extension if available.
	if (!successful) {
		successful = XkbGetAutoRepeatRate(disp, XkbUseCoreKbd, &kb_delay, &kb_rate);
	}
	#endif

	#ifdef XF86MISC
	//Fallback to the XF86 Misc extension if available and other efforts failed.
	if (!successful) {
		XF86MiscKbdSettings kb_info;
		successful = (bool) XF86MiscGetKbdSettings(disp, &kb_info);
		if (successful) {
			kb_delay = (unsigned int) kbdinfo.delay;
			kb_rate = (unsigned int) kbdinfo.rate;
		}
	}
	#endif

	if (successful) {
		value = (long) kb_rate;
	}

	(void) kb_delay;
	return value;
}

long GetPointerAccelerationMultiplier() {
	long value = -1;
	int accel_numerator, accel_denominator, threshold;

	XGetPointerControl(disp, &accel_numerator, &accel_denominator, &threshold);
	if (accel_denominator >= 0) {
		value = accel_denominator;
	}
	(void) accel_numerator;
	(void) threshold;

	return value;
}

long GetPointerAccelerationThreshold() {
	long value = -1;
	int accel_numerator, accel_denominator, threshold;

	XGetPointerControl(disp, &accel_numerator, &accel_denominator, &threshold);
	if (threshold >= 0) {
		value = threshold;
	}
	(void) accel_numerator;
	(void) accel_denominator;

	return value;
}

long GetPointerSensitivity() {
	long value = -1;
	int accel_numerator, accel_denominator, threshold;

	XGetPointerControl(disp, &accel_numerator, &accel_denominator, &threshold);
	if (accel_numerator >= 0) {
		value = accel_numerator;
	}
	(void) accel_denominator;
	(void) threshold;

	return value;
}

long GetMultiClickTime() {
	long value = -1;
	int clicktime;
	bool successful = false;

	//Try and acquire the multi-click time from the user defined Xdefaults
	char * xprop = XGetDefault(disp, "*", "multiClickTime");
	if (xprop != NULL && sscanf(xprop, "%i", &clicktime) != EOF) {
		successful = true;
	}

	xprop = XGetDefault(disp, "OpenWindows", "MultiClickTimeout");
	if (xprop != NULL && sscanf(xprop, "%i", &clicktime) != EOF) {
		successful = true;
	}

	#ifdef XT
	int argc = 0;
	char ** argv = {NULL};

	//Fall back to the X Toolkit extension if available and other efforts failed.
    XtDisplayInitialize(app_context, disp, "JNativeHook", "JNativeHook", NULL, 0, &argc, argv);
	if (!successful) {
		clicktime = XtGetMultiClickTime(disp);
		if (clicktime >= 0) {
			successful = true;
		}
	}
	#endif

	if (successful) {
		value = clicktime;
	}

	return value;
}


KeySym KeyCodeToKeySym(KeyCode keycode, unsigned int event_mask) {
	KeySym keysym = NoSymbol;

	#ifdef XKB
	//Get the map
	XkbDescPtr keyboard_map = XkbGetMap(disp, XkbAllClientInfoMask, XkbUseCoreKbd);
	if (keyboard_map) {
		//What is diff between XkbKeyGroupInfo and XkbKeyNumGroups?
		unsigned char info = XkbKeyGroupInfo(keyboard_map, keycode);
		unsigned int num_groups = XkbKeyNumGroups(keyboard_map, keycode);

		//Get the group
		unsigned int group = 0x00;
		switch (XkbOutOfRangeGroupAction(info)) {
			case XkbRedirectIntoRange:
				/* If the RedirectIntoRange flag is set, the four least significant
				 * bits of the groups wrap control specify the index of a group to
				 * which all illegal groups correspond. If the specified group is
				 * also out of range, all illegal groups map to Group1.
				 */
				printf("XkbRedirectIntoRange\n");
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
				printf("XkbClampIntoRange\n");
				group = num_groups - 1;
			break;

			case XkbWrapIntoRange:
				/* If neither flag is set, group is wrapped into range using integer
				 * modulus. For example, a key with two groups of symbols for which
				 * groups wrap uses Group1 symbols if the global effective group is
				 * Group3 or Group2 symbols if the global effective group is Group4.
				 */
				printf("XkbWrapIntoRange\n");
			default:
				printf("Default\n");
				if (num_groups != 0) {
					group %= num_groups;
				}
			break;
		}

		XkbKeyTypePtr key_type = XkbKeyKeyType(keyboard_map, keycode, group);
		unsigned int active_mods = event_mask & key_type->mods.mask;

		KeySym keysym = NoSymbol;
		int i, level = 0;
		for (i = 0; i < key_type->map_count; i++) {
			if (key_type->map[i].active && key_type->map[i].mods.mask == active_mods) {
				level = key_type->map[i].level;
			}
		}

		keysym = XkbKeySymEntry(keyboard_map, keycode, level, group);
		XkbFreeClientMap(keyboard_map, XkbAllClientInfoMask, true);
	}
	#else
	int min_keycode, max_keycode, keysyms_per_keycode;//, i = 0;
	XDisplayKeycodes(disp, &min_keycode, &max_keycode);

	KeySym * keyboard_map = XGetKeyboardMapping(disp, min_keycode, (max_keycode - min_keycode + 1), &keysyms_per_keycode);
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

				KeyCode caps_lock = XKeysymToKeycode(disp, XK_Caps_Lock);
				KeyCode shift_lock = XKeysymToKeycode(disp, XK_Shift_Lock);
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
	#endif

	return keysym;
}

void OnLibraryLoad() {
	//Tell X Threads are OK
	XInitThreads();

	#ifdef XT
	XtToolkitInitialize();
	app_context = XtCreateApplicationContext();
	#endif

	//Open local display.
	disp = XOpenDisplay(XDisplayName(NULL));
	#ifdef DEBUG
	if (disp != NULL) {
		fprintf(stdout, "OnLibraryLoad(): XOpenDisplay successful.\n");
	}
	else {
		fprintf(stderr, "OnLibraryLoad(): XOpenDisplay failure!\n");
	}
	#endif
	
	Bool isAutoRepeat = false;
	#ifdef XKB
	//enable detectable autorepeat.
	XkbSetDetectableAutoRepeat(disp, True, &isAutoRepeat);
	#else
	XAutoRepeatOn(disp);

	XKeyboardState kb_state;
	XGetKeyboardControl(disp, &kb_state);

	isAutoRepeat = (kb_state.global_auto_repeat == AutoRepeatModeOn);
	#endif

	#ifdef DEBUG
	if (isAutoRepeat) {
		fprintf(stdout, "OnLibraryLoad(): Successfully enabled detectable autorepeat.\n");
	}
	else {
		fprintf(stderr, "OnLibraryLoad(): Could not enable detectable auto-repeat!\n");
	}
	#endif
}

void OnLibraryUnload() {
	#ifdef XT
	XtDestroyApplicationContext(app_context);
	#endif

	//Destroy the native displays.
	if (disp != NULL) {
		XCloseDisplay(disp);
		disp = NULL;
	}
}
