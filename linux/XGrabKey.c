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
#include <X11/keysym.h>
#include <X11/XKBlib.h>

unsigned int NumberLockMask = 0;
unsigned int ScrollLockMask = 0;
unsigned int CapsLockMask = 0;

/* 
 * This method locates key masks like scroll lock, number lock and caps lock
 * because X11 does not provide fixed constants for them.  Variables populated
 * are located above this comment.
 */
void getModifiers(Display * display) {
	int i;
	XModifierKeymap *modmap;
	KeyCode nlock, slock;
	static int mask_table[8] = {
		ShiftMask, LockMask, ControlMask, Mod1Mask,
		Mod2Mask, Mod3Mask, Mod4Mask, Mod5Mask
	};
	
	nlock = XKeysymToKeycode (display, XK_Num_Lock);
	slock = XKeysymToKeycode (display, XK_Scroll_Lock);
	
	//Find out the masks for the NumLock and ScrollLock modifiers, so that we
	//can bind the grabs for when they are enabled too.
	modmap = XGetModifierMapping (display);
	
	if (modmap != NULL && modmap->max_keypermod > 0) {
		for (i = 0; i < 8 * modmap->max_keypermod; i++) {
			if (modmap->modifiermap[i] == nlock && nlock != 0) {
				NumberLockMask = mask_table[i / modmap->max_keypermod];
			}
			else if (modmap->modifiermap[i] == slock && slock != 0) {
				ScrollLockMask = mask_table[i / modmap->max_keypermod];
			}
		}
	}
	
	CapsLockMask = LockMask;
	
	if (modmap) {
		XFreeModifiermap(modmap);
	}
}

int main(int argc, const char * argv[]) {
	Display * display;
	XEvent xev;

	//Try to attach to the default X11 display.
	display = XOpenDisplay(NULL);
	if(display == NULL) {
		printf("Error: Could not open display!\n");
		return 1;
	}

	//Get the default global window to listen on for the selected X11 display.
	Window grabWin = DefaultRootWindow(display);
	XAllowEvents(display, AsyncKeyboard, CurrentTime);
	XkbSetDetectableAutoRepeat(display, true, NULL);

	//Locate the number lock and scroll lock masks.
	getModifiers(display);
	
	//Find the X11 KeyCode we are listening for.
	KeyCode key = XKeysymToKeycode(display, XK_B);

	//No Modifier
	XGrabKey(display, key, None, grabWin, true, GrabModeAsync, GrabModeAsync);
	
	//ShiftMask
	XGrabKey(display, key, ShiftMask, grabWin, true, GrabModeAsync, GrabModeAsync);

	//NumberLockMask
	XGrabKey(display, key, NumberLockMask, grabWin, true, GrabModeAsync, GrabModeAsync);

	//ScrollLockMask
	XGrabKey(display, key, ScrollLockMask, grabWin, true, GrabModeAsync, GrabModeAsync);

	//CapsLockMask
	XGrabKey(display, key, CapsLockMask, grabWin, true, GrabModeAsync, GrabModeAsync);

	//NumberLockMask | ScrollLockMask
	XGrabKey(display, key, NumberLockMask | ScrollLockMask, grabWin, true, GrabModeAsync, GrabModeAsync);

	//NumberLockMask | CapsLockMask
	XGrabKey(display, key, NumberLockMask | CapsLockMask, grabWin, true, GrabModeAsync, GrabModeAsync);
	
	//ScrollLockMask | CapsLockMask
	XGrabKey(display, key, ScrollLockMask | CapsLockMask, grabWin, true, GrabModeAsync, GrabModeAsync);
	
	//NumberLockMask | ScrollLockMask | CapsLockMask
	XGrabKey(display, key, NumberLockMask | ScrollLockMask | CapsLockMask, grabWin, true, GrabModeAsync, GrabModeAsync);

	int i;
	for(i = 0; i < 10; i++) {
		//Block waiting for the next event.
		XNextEvent(display, &xev);

		switch (xev.type) {
			case KeyPress:
				printf("Key Press - %i\n", (int) XLookupKeysym(&xev.xkey, 0));
			break;

			case KeyRelease:
				printf("Key Release - %i\n", (int) XLookupKeysym(&xev.xkey, 0));
			break;
		}
	}

	//No Modifier
	XUngrabKey(display, key, None, grabWin);

	//ShiftMask
	XUngrabKey(display, key, ShiftMask, grabWin);

	//NumberLockMask
	XUngrabKey(display, key, NumberLockMask, grabWin);

	//ScrollLockMask
	XUngrabKey(display, key, ScrollLockMask, grabWin);

	//CapsLockMask
	XUngrabKey(display, key, CapsLockMask, grabWin);

	//NumberLockMask | ScrollLockMask
	XUngrabKey(display, key, NumberLockMask | ScrollLockMask, grabWin);

	//NumberLockMask | CapsLockMask
	XUngrabKey(display, key, NumberLockMask | CapsLockMask, grabWin);

	//ScrollLockMask | CapsLockMask
	XUngrabKey(display, key, ScrollLockMask | CapsLockMask, grabWin);

	//NumberLockMask | ScrollLockMask | CapsLockMask
	XUngrabKey(display, key, NumberLockMask | ScrollLockMask | CapsLockMask, grabWin);

	//Close the connection to the selected X11 display.
	XCloseDisplay(display);

	return 0;
}
