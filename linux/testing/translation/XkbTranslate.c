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
#include <stdlib.h>
#include <X11/X.h>
#include <X11/XKBlib.h>

KeySym KeyCodeToKeySym(Display * display, KeyCode keycode, unsigned int event_mask) {
	KeySym keysym = NoSymbol;
	
	//Get the map
	XkbDescPtr keyboard_map = XkbGetMap(display, XkbAllClientInfoMask, XkbUseCoreKbd);
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
		XkbFreeClientMap(keyboard_map, XkbAllClientInfoMask, true);
	}
	
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
