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
#include <X11/extensions/Xevie.h>

bool running = true;

int main(int argc, const char * argv[]) {
	Display * display;
	XEvent xev;
	
	//Try to attach to the default X11 display.
	display = XOpenDisplay(NULL);
	if(display == NULL) {
		printf("Error: Could not open display!\n");
		return 1;
	}
	
	//Enable XEvIE on the default display.
	if(!XevieStart(display)) {
		printf("XevieStart() failure!\n");
		return 1;
	}
	
	//Select the input masks we would like to listen for.
	XevieSelectInput(display, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
	
	while(running) {
		//Block waiting for the next event.
		XNextEvent(display, &xev);
		
		switch(xev.type) {
			case KeyPress:
				//Stop looping if the escape key is pressed.
				if (XKeycodeToKeysym(disp_data, data->event.u.u.detail, 0) == XK_Escape) {
					running = false;
				}

				printf("Key Press - %i\n", (int) XLookupKeysym(&xev.xkey, 0));
			break;

			case KeyRelease:
				printf("Key Release - %i\n", (int) XLookupKeysym(&xev.xkey, 0));
			break;

			case ButtonPress:
				printf("Button Press - %i\n", xev.xbutton.button);
			break;

			case ButtonRelease:
				printf("Button Release - %i\n", xev.xbutton.button);
			break;

			case MotionNotify:
				printf("Motion Notify - %i, %i\n", xev.xmotion.x_root, xev.xmotion.y_root);
			break;

			default:
				printf("Unknown Event - %x\n", xev.type);
			break;
		}
		
		//Send the event back to the selected X11 display.
		XevieSendEvent(display, &xev, XEVIE_UNMODIFIED);
	}

	//Disable XEvIE on the selected X11 display.
	XevieEnd(display);

	//Close the connection to the selected X11 display.
	XCloseDisplay(display);

	return 0;
}
