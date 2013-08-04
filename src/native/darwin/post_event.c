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

#include <config.h>
#include <nativehook.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifdef USE_XTEST
#include <X11/extensions/XTest.h>
#endif

#include "convert_to_native.h"
#include "convert_to_virtual.h"

extern Display *disp;

// This lookup table must be in the same order the masks are defined.
#ifdef USE_XTEST
static KeySym keymask_lookup[8] = {
	XK_Shift_L,
	XK_Control_L,
	XK_Meta_L,
	XK_Alt_L,

	XK_Shift_R,
	XK_Control_R,
	XK_Meta_R,
	XK_Alt_R
};

static unsigned int btnmask_lookup[5] = {
	MASK_BUTTON1,
	MASK_BUTTON2,
	MASK_BUTTON3,
	MASK_BUTTON4,
	MASK_BUTTON5
};
#endif

NATIVEHOOK_API void hook_post_event(VirtualEvent * const event) {
	char buffer[4];

	#ifdef USE_XTEST
	Bool is_press;

	// XTest does not have modifier support, so we fake it by depressing the
	// appropriate modifier keys.
	// TODO Check and see if GCC unrolls these loops with -02
	for (unsigned int i = 0; i < sizeof(keymask_lookup) / sizeof(KeySym); i++) {
		if (event->mask & 1 << i) {
			XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, keymask_lookup[i]), True, 0);
		}
	}

	for (unsigned int i = 0; i < sizeof(btnmask_lookup) / sizeof(unsigned int); i++) {
		if (event->mask & btnmask_lookup[i]) {
			XTestFakeButtonEvent(disp, i + 1, True, 0);
		}
	}

	switch (event->type) {
		case EVENT_KEY_PRESSED:
			is_press = True;
			goto EVENT_KEY;

		case EVENT_KEY_TYPED:
			// Need to convert a wchar_t to keysym!
			snprintf(buffer, 4, "%lc", event->data.keyboard.keychar);

			event->type = EVENT_KEY_PRESSED;
			event->data.keyboard.keycode = convert_to_virtual_key(XStringToKeysym(buffer));
			event->data.keyboard.keychar = CHAR_UNDEFINED;
			hook_post_event(event);

		case EVENT_KEY_RELEASED:
			is_press = False;

		EVENT_KEY:
			XTestFakeKeyEvent(
				disp, 
				XKeysymToKeycode(disp, convert_to_native_key(event->data.keyboard.keycode)), 
				is_press, 
				0);
			break;

		case EVENT_MOUSE_PRESSED:
			is_press = True;
			goto EVENT_BUTTON;

		case EVENT_MOUSE_WHEEL:
			// Wheel events should be the same as click events on X11.

		case EVENT_MOUSE_CLICKED:
			event->type = EVENT_MOUSE_PRESSED;
			hook_post_event(event);

		case EVENT_MOUSE_RELEASED:
			is_press = False;
			goto EVENT_BUTTON;

		EVENT_BUTTON:
			XTestFakeButtonEvent(disp, event->data.mouse.button, is_press, 0);
			break;

		case EVENT_MOUSE_DRAGGED:
			// The button masks are all applied with the modifier masks.

		case EVENT_MOUSE_MOVED:
			XTestFakeMotionEvent(disp, -1, event->data.mouse.x, event->data.mouse.y, 0);
			break;
	}

	// Release the previously held modifier keys used to fake the event mask.
	for (unsigned int i = 0; i < sizeof(keymask_lookup) / sizeof(KeySym); i++) {
		if (event->mask & 1 << i) {
			XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, keymask_lookup[i]), False, 0);
		}
	}

	for (unsigned int i = 0; i < sizeof(btnmask_lookup) / sizeof(unsigned int); i++) {
		if (event->mask & btnmask_lookup[i]) {
			XTestFakeButtonEvent(disp, i + 1, False, 0);
		}
	}
	#else
	XEvent *x_event = NULL;
	long x_mask = NoEventMask;

	Window root_win, child_win;
	int root_x, root_y;
	int win_x, win_y;
	unsigned int mask;
	if (!XQueryPointer(disp, DefaultRootWindow(disp), &root_win, &child_win, &root_x, &root_y, &win_x, &win_y, &mask)) {
		root_x = 0;
		root_y = 0;
		win_x = 0;
		win_y = 0;
	}

	void *data = event->data;
	switch (event->type) {
		case EVENT_KEY_PRESSED:
			x_event = (XEvent *) malloc(sizeof(XKeyEvent));
			((XKeyEvent *) x_event)->type = KeyPress;
			x_mask = KeyPressMask;
			goto EVENT_KEY;

		case EVENT_KEY_TYPED:
			// Need to convert a wchar_t to keysym!
			snprintf(buffer, 4, "U%04d", event->data.keyboard.keychar);

			event->type = EVENT_KEY_PRESSED;
			event->data.keyboard.keycode = convert_to_virtual_key(XStringToKeysym(buffer));
			event->data.keyboard.keychar = CHAR_UNDEFINED;
			hook_post_event(event);

		case EVENT_KEY_RELEASED:
			x_event = (XEvent *) malloc(sizeof(XKeyEvent));
			((XKeyEvent *) x_event)->type = KeyRelease;
			x_mask = KeyReleaseMask;

		EVENT_KEY:
			((XKeyEvent *) x_event)->display = disp;
			((XKeyEvent *) x_event)->window = root_win; //InputFocus; //XGetInputFocus();
			((XKeyEvent *) x_event)->root = root_win;
			((XKeyEvent *) x_event)->subwindow = None;
			((XKeyEvent *) x_event)->time = CurrentTime;
			((XKeyEvent *) x_event)->x = win_x;
			((XKeyEvent *) x_event)->y = win_y;
			((XKeyEvent *) x_event)->x_root = root_x;
			((XKeyEvent *) x_event)->y_root = root_y;
			((XKeyEvent *) x_event)->state = convert_to_native_mask(event->mask);
			((XKeyEvent *) x_event)->keycode = XKeysymToKeycode(disp, convert_to_native_key(event->data.keyboard.keycode));
			((XKeyEvent *) x_event)->same_screen = True;
			break;

		case EVENT_MOUSE_PRESSED:
			x_event = (XEvent *) malloc(sizeof(XButtonEvent));
			x_mask = ButtonPressMask;
			goto EVENT_BUTTON;

		case EVENT_MOUSE_WHEEL:
			// Wheel events should be the same as click events on X11.

		case EVENT_MOUSE_CLICKED:
			event->type = EVENT_MOUSE_PRESSED;
			hook_post_event(event);

		case EVENT_MOUSE_RELEASED:
			x_event = (XEvent *) malloc(sizeof(XButtonEvent));
			x_mask = KeyReleaseMask;
			goto EVENT_BUTTON;

		EVENT_BUTTON:
			((XButtonEvent *) x_event)->display = disp;
			((XButtonEvent *) x_event)->window = root_win; //InputFocus; //XGetInputFocus();
			((XButtonEvent *) x_event)->root = root_win;
			((XButtonEvent *) x_event)->subwindow = None;
			((XButtonEvent *) x_event)->time = CurrentTime;
			((XButtonEvent *) x_event)->x = win_x;
			((XButtonEvent *) x_event)->y = win_y;
			((XButtonEvent *) x_event)->x_root = root_x;
			((XButtonEvent *) x_event)->y_root = root_y;
			((XButtonEvent *) x_event)->state = convert_to_native_mask(event->mask);
			((XButtonEvent *) x_event)->button = event->data.mouse.button;
			((XButtonEvent *) x_event)->same_screen = True;
			break;

		case EVENT_MOUSE_DRAGGED:
			x_event = (XEvent *) malloc(sizeof(XMotionEvent));
			((XMotionEvent *) x_event)->state = convert_to_native_mask(event->mask);

			// TODO Dump the ASM and check how GCC optimizes the following...
			if (((XMotionEvent *) x_event)->state & Button1Mask) {
				x_mask |= Button1MotionMask;
			}

			if (((XMotionEvent *) x_event)->state & Button2Mask) {
				x_mask |= Button2MotionMask;
			}

			if (((XMotionEvent *) x_event)->state & Button3Mask) {
				x_mask |= Button3MotionMask;
			}

			if (((XMotionEvent *) x_event)->state & Button4Mask) {
				x_mask |= Button4MotionMask;
			}

			if (((XMotionEvent *) x_event)->state & Button5Mask) {
				x_mask |= Button5MotionMask;
			}

			// This little trick works because Button#MotionMasks align with
			// the Button#Masks.  The compiler *SHOULD* do this for us!
			/*
			x_mask = ((XButtonEvent *) x_event)->state &
					(Button1MotionMask | Button2MotionMask |
					Button2MotionMask | Button3MotionMask | Button5MotionMask);
			*/
			goto EVENT_MOTION;

		case EVENT_MOUSE_MOVED:
			x_event = (XEvent *) malloc(sizeof(XMotionEvent));
			((XMotionEvent *) x_event)->state = convert_to_native_mask(event->mask);
			goto EVENT_MOTION;

		EVENT_MOTION:
			((XMotionEvent *) x_event)->display = disp;
			((XMotionEvent *) x_event)->window = root_win; //InputFocus; //XGetInputFocus();
			((XMotionEvent *) x_event)->root = root_win;
			((XMotionEvent *) x_event)->subwindow = None;
			((XMotionEvent *) x_event)->time = CurrentTime;
			((XMotionEvent *) x_event)->x = win_x; // Not sure what to do this
			((XMotionEvent *) x_event)->y = win_y; // Not sure what to do this
			((XMotionEvent *) x_event)->x_root = event->data.mouse.x;
			((XMotionEvent *) x_event)->y_root = event->data.mouse.y;
			((XMotionEvent *) x_event)->state = convert_to_native_mask(event->mask);;
			((XMotionEvent *) x_event)->is_hint = NotifyNormal;
			((XMotionEvent *) x_event)->same_screen = True;
			break;
	}

	XSendEvent(disp, InputFocus, False, x_mask, x_event);
	#endif

	// Don't forget to flush!
	XFlush(disp);
}
