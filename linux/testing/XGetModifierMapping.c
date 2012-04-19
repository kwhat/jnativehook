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
#include <X11/Xlib.h>

int main(int argc, const char * argv[]) {
	Display * display;

	//Try to attach to the default X11 display.
	display = XOpenDisplay(NULL);
	if(display == NULL) {
		printf("Error: Could not open display!\n");
		return EXIT_FAILURE;
	}

	XModifierKeymap * modifier_map = XGetModifierMapping(display);
	if (!modifier_map) {
		printf("Unable to get modifier mapping table.\n");
		return EXIT_FAILURE;
	}

	int i, j;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < modifier_map->max_keypermod; j++) {
			int index = (i * modifier_map->max_keypermod) + j;
			if (modifier_map->modifiermap[index]) {
				KeySym keysym = XKeycodeToKeysym(display, modifier_map->modifiermap[index], 0);
				char * name = XKeysymToString(keysym);

				printf("%s  %s (0x%0x)", (j > 0 ? "," : ""), 
					(name ? name : "BadKey"), modifier_map->modifiermap[index]);
			}
		}
		
		printf("\n");
	}

    XFree(modifier_map);

	//Close the connection to the selected X11 display.
	XCloseDisplay(display);

	return EXIT_SUCCESS;
}
