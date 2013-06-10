/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
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

extern Display *disp;

void PostKeyDownEvent(KeySym keysym, unsigned int state, Time time) {
	int root_x, root_y, win_x, win_y;

	if (!XQueryPointer(disp, DefaultRootWindow(disp), NULL, NULL, &root_x, &root_y, &win_x, &win_y, NULL)) {
		root_x = 0;
		root_y = 0;
		win_x = 0;
		win_y = 0;
	}

	//FIXME Calculate the current server time based on the system timestamp in MS.
	time = CurrentTime;

	XKeyEvent event;
	event.type = KeyPress;
	event.display = disp;
	event.window = DefaultRootWindow(disp); //InputFocus; //XGetInputFocus();
	event.root = DefaultRootWindow(disp);
	event.subwindow = None;
	event.time = time;
	event.x = win_x;
	event.y = win_y;
	event.x_root = root_x;
	event.y_root = root_y;
	event.state = state; //modifiers
	event.keycode = XKeysymToKeycode(disp, keysym);
	event.same_screen = true;

	XSendEvent(disp, InputFocus, false, KeyPressMask, (XEvent *) &event);
	XFlush(disp);
}

void PostKeyUpEvent(KeySym keysym, unsigned int state, Time time) {
	int root_x, root_y, win_x, win_y;

	if (!XQueryPointer(disp, DefaultRootWindow(disp), NULL, NULL, &root_x, &root_y, &win_x, &win_y, NULL)) {
		root_x = 0;
		root_y = 0;
		win_x = 0;
		win_y = 0;
	}

	//FIXME Calculate the current server time based on the system timestamp in MS.
	time = CurrentTime;

	XKeyEvent event;
	event.type = KeyPress;
	event.display = disp;
	event.window = DefaultRootWindow(disp); //InputFocus; //XGetInputFocus();
	event.root = DefaultRootWindow(disp);
	event.subwindow = None;
	event.time = time;
	event.x = win_x;
	event.y = win_y;
	event.x_root = root_x;
	event.y_root = root_y;
	event.state = state; //modifiers
	event.keycode = XKeysymToKeycode(disp, keysym);
	event.same_screen = true;

	XSendEvent(disp, InputFocus, false, KeyPressMask, (XEvent *) &event);
	XFlush(disp);
}

void PostKeyTypedEvent(wchar_t wchar, Time time) {
	char buffer[6];
	snprintf(buffer, 6, "U%04d", wchar);

	KeySym code = XStringToKeysym(buffer);

	//TODO Send Key Down + Key Up events.
}
