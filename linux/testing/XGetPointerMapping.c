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

	int count = 256, i;
    unsigned char * pointer_map = malloc(sizeof(unsigned char) * count);

    count = XGetPointerMapping(display, pointer_map, count);
    pointer_map = realloc(pointer_map, sizeof(unsigned char) * count);

    printf("There are %d pointer buttons defined.\n\n", count);
    printf("    Physical        Button\n");
    printf("     Button          Code\n");
    for (i = 0; i < count; i++) {
		printf("      %3u            %3u\n", i + 1, (unsigned int) pointer_map[i]);
    }
	printf("\n");

    free(pointer_map);

	//Close the connection to the selected X11 display.
	XCloseDisplay(display);

	return EXIT_SUCCESS;
}
