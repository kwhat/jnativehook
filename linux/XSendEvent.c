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

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

int main(int argc, const char * argv[]) {
	Display *display;

	//Try to attach to the default X11 display.
	display = XOpenDisplay(NULL);
	if(display == NULL) {
		printf("Error: Could not open display!\n");
		return 1;
	}

	XKeyEvent events[2];
	events[0].type = KeyPress;
	//events[0].serial = Filled in by the server
	//events[0].send_event = true;
	events[0].display = display;
	events[0].window = DefaultRootWindow(display); //InputFocus; //XGetInputFocus();
	events[0].root = DefaultRootWindow(display);
	events[0].subwindow = None;
	events[0].time = CurrentTime;
	events[0].x = 10; //FIXME Get the current pointer position.
	events[0].y = 10; //FIXME Get the current pointer position.
	events[0].x_root = 0;
	events[0].y_root = 0;
	events[0].state = 0x00; //modifiers
	events[0].keycode = XKeysymToKeycode(display, XK_B);
	events[0].same_screen = true;

	//Create the key release input
	memcpy(events + 1, events, sizeof(XKeyEvent));
	events[1].type = KeyRelease;

	int i;
	for(i = 0; i < 5; i++) {
		sleep(1);

		XSendEvent(display, InputFocus, false, KeyPressMask, (XEvent *) events);
		XFlush(display);

		usleep(20000);

		XSendEvent(display, InputFocus, false, KeyReleaseMask, (XEvent *) events + 1);
		XFlush(display);
		
		printf("Msg Sent.\n");
	}

	//Close the connection to the selected X11 display.
	XCloseDisplay(display);

	return 0;
}
