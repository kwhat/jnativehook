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
#include <X11/X.h>
#include <X11/XKBlib.h>

void check(XkbDescPtr keyboard_map, KeyCode keycode) {
	//Returns the number of groups of symbols bound to the key corresponding to keycode
	unsigned char info = XkbKeyGroupInfo(keyboard_map, keycode);
	int num_groups = XkbKeyNumGroups(keyboard_map, keycode);

    int i, g;
	printf("%d: ", (int) keycode);
	for (g = 0; g < num_groups; g++) {
		for (i = 0; i < XkbKeyGroupWidth(keyboard_map, keycode, g); i++) {
			KeySym keysym = XkbKeySymEntry(keyboard_map, keycode, i, g);
			
			printf("%c (0x%X)\t", (char) keysym, (int) keysym);
		}
	}
	printf("\n");
}

int main(int argc, const char * argv[]) {
	Display * display;

	//Try to attach to the default X11 display.
	display = XOpenDisplay(NULL);
	if(display == NULL) {
		printf("Error: Could not open display!\n");
		return EXIT_FAILURE;
	}

	//Get the map
	XkbDescPtr keyboard_map = XkbGetMap(display, XkbAllClientInfoMask, XkbUseCoreKbd);
	
	int i;
	for (i = keyboard_map->min_key_code; i <= keyboard_map->max_key_code; i++) {
		check(keyboard_map, i);
	}
	
	//Close the connection to the selected X11 display.
	XCloseDisplay(display);

	return EXIT_SUCCESS;
}
