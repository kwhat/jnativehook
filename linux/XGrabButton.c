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
#include <stdbool.h>
#include <X11/Xlib.h>

int main(int argc, const char * argv[]) {
	Display * display;
	XEvent xev;

	//Try to attach to the default X11 display.
	display = XOpenDisplay(NULL);
	if(display == NULL) {
		printf("Error: Could not open display!\n");
		return EXIT_FAILURE;
	}

	//Get the default global window to listen on for the selected X11 display.
	Window grabWin = DefaultRootWindow(display);
	XAllowEvents(display, AsyncBoth, CurrentTime);
	
	//Grab mouse button 1 events on the selected X11 display.
	XGrabButton(display, Button1, AnyModifier, grabWin, true, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None);

	int i;
	for(i = 0; i < 10; i++) {
		//Block waiting for the next event.
		XNextEvent(display, &xev);

		switch(xev.type) {
			case ButtonPress:
				printf(" Button Press - %i\n", xev.xbutton.button);
			break;

			case ButtonRelease:
				printf(" Button Release - %i\n", xev.xbutton.button);
			break;
		}
	}

	//Ungrab mouse button 1 events on the selected X11 display.
	XUngrabButton(display, Button1, AnyModifier, grabWin);

	//Close the connection to the selected X11 display.
	XCloseDisplay(display);

	return EXIT_SUCCESS;
}
