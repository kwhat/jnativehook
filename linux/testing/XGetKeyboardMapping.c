/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
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
#include <X11/Xlib.h>

int main(int argc, const char * argv[]) {
	Display * display;

	//Try to attach to the default X11 display.
	display = XOpenDisplay(NULL);
	if(display == NULL) {
		printf("Error: Could not open display!\n");
		return EXIT_FAILURE;
	}

	int min_keycode, max_keycode, keysyms_per_keycode, i;
	XDisplayKeycodes(display, &min_keycode, &max_keycode);
	KeySym * keyboard_map = XGetKeyboardMapping(display, min_keycode, (max_keycode - min_keycode + 1), &keysyms_per_keycode);
	if (!keyboard_map) {
		printf("Unable to get keyboard mapping table.\n");
		return EXIT_FAILURE;
	}

	KeySym * map_ptr = keyboard_map;

	for (i = min_keycode; i <= max_keycode; i++) {
		printf("keycode %3d =", i);

		int  j, max = keysyms_per_keycode - 1;
		while ((max >= 0) && (keyboard_map[max] == NoSymbol)) {
			max--;
		}

		for (j = 0; j <= max; j++) {
			register KeySym ks = keyboard_map[j];
			char *s;

			if (ks != NoSymbol) {
				s = XKeysymToString(ks);
			}
			else {
				s = "NoSymbol";
			}

			if (s) {
				printf(" %s", s);
			}
			else {
				printf(" 0x%04x", (unsigned int) ks);
			}
		}
		keyboard_map += keysyms_per_keycode;
		printf("\n");
	}

    XFree(map_ptr);

	//Close the connection to the selected X11 display.
	XCloseDisplay(display);

	return EXIT_SUCCESS;
}
