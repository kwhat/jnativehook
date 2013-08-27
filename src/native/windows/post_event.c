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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <nativehook.h>
#include <windows.h>

#include "input_converter.h"

static UINT keymask_lookup[8] = {
	VK_LSHIFT,
	VK_LCONTROL,
	VK_LWIN,
	VK_LMENU,

	VK_RSHIFT,
	VK_RCONTROL,
	VK_RWIN,
	VK_RMENU
};

NATIVEHOOK_API void hook_post_event(VirtualEvent * const event) {
	unsigned char events_size = 0, events_max = 28;
	INPUT *events = malloc(sizeof(INPUT) * events_max);

	if (event->mask & (MASK_SHIFT | MASK_CTRL | MASK_META | MASK_ALT)) {
		for (unsigned int i = 0; i < sizeof(keymask_lookup) / sizeof(UINT); i++) {
			if (event->mask & 1 << i) {
				events[events_size].type = INPUT_KEYBOARD;
				events[events_size].ki.wVk = convert_to_native_key(event->data.keyboard.keycode);
				events[events_size].ki.dwFlags = 0x0000;  // KEYEVENTF_KEYDOWN
				events[events_size].ki.time = 0; // Use current system time.
				events_size++;
			}
		}
	}

	if (event->mask & (MASK_BUTTON1 | MASK_BUTTON2 | MASK_BUTTON3 | MASK_BUTTON4 | MASK_BUTTON5)) {
		events[events_size].type = INPUT_MOUSE;
		events[events_size].mi.dx = 0;	// Relative mouse movement due to
		events[events_size].mi.dy = 0;	// MOUSEEVENTF_ABSOLUTE not being set.
		events[events_size].mi.mouseData = 0x00;
		events[events_size].mi.time = 0; // Use current system time.

		if (event->mask & MASK_BUTTON1) {
			events[events_size].mi.mouseData |= MOUSEEVENTF_LEFTDOWN;
		}

		if (event->mask & MASK_BUTTON2) {
			events[events_size].mi.mouseData |= MOUSEEVENTF_RIGHTDOWN;
		}

		if (event->mask & MASK_BUTTON3) {
			events[events_size].mi.mouseData |= MOUSEEVENTF_MIDDLEDOWN;
		}

		if (event->mask & MASK_BUTTON4) {
			events[events_size].mi.mouseData = XBUTTON1;
			events[events_size].mi.mouseData |= MOUSEEVENTF_XDOWN;
		}

		if (event->mask & MASK_BUTTON5) {
			events[events_size].mi.mouseData = XBUTTON2;
			events[events_size].mi.dwFlags |= MOUSEEVENTF_XDOWN;
		}

		events_size++;
	}

	char buffer[4];
	switch (event->type) {
		case EVENT_KEY_PRESSED:
			events[events_size].ki.dwFlags = 0x0000;  // KEYEVENTF_KEYDOWN
			goto EVENT_KEY;

		case EVENT_KEY_TYPED:
			// Need to convert a wchar_t to keysym!
			snprintf(buffer, 4, "%lc", event->data.keyboard.keychar);

			event->type = EVENT_KEY_PRESSED;
			// TODO Find a better way to do this.
			event->data.keyboard.keycode = convert_to_native_key(VkKeyScanEx((TCHAR) event->data.keyboard.keycode, GetKeyboardLayout(0)));
			event->data.keyboard.keychar = CHAR_UNDEFINED;
			hook_post_event(event);

		case EVENT_KEY_RELEASED:
			events[events_size].ki.dwFlags = KEYEVENTF_KEYUP;

		EVENT_KEY:
			events[events_size].type = INPUT_KEYBOARD;

			events[events_size].ki.wVk = convert_to_native_key(event->data.keyboard.keycode);
			//events[events_size].ki.wScan = MapVirtualKey(vkCode, 0); //MAPVK_VK_TO_VSC
			//events[events_size].ki.dwFlags |= KEYEVENTF_SCANCODE;

			if ((events[events_size].ki.wVk >= 33 && events[events_size].ki.wVk <= 46) ||
					(events[events_size].ki.wVk >= 91 && events[events_size].ki.wVk <= 93)) {
				//Key is an extended key.
				events[events_size].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
			}
			events[events_size].ki.time = 0; //GetSystemTime()
			events_size++;
			break;

		case EVENT_MOUSE_PRESSED:
			goto EVENT_BUTTON;

		case EVENT_MOUSE_WHEEL:
			// Wheel events should be the same as click events on X11.

		case EVENT_MOUSE_CLICKED:
			event->type = EVENT_MOUSE_PRESSED;
			hook_post_event(event);

		case EVENT_MOUSE_RELEASED:
			goto EVENT_BUTTON;

		EVENT_BUTTON:
			events[events_size].type = INPUT_MOUSE;
			events[events_size].mi.dx = event->data.mouse.x;
			events[events_size].mi.dy = event->data.mouse.y;
			events[events_size].mi.dwFlags = MOUSEEVENTF_ABSOLUTE & MOUSEEVENTF_MOVE;
			events[events_size].mi.time = 0; //GetSystemTime()
			events_size++;
			break;

		case EVENT_MOUSE_DRAGGED:
			// The button masks are all applied with the modifier masks.

		case EVENT_MOUSE_MOVED:
			events[events_size].type = INPUT_MOUSE;
			events[events_size].mi.dx = event->data.mouse.x;
			events[events_size].mi.dy = event->data.mouse.y;
			events[events_size].mi.dwFlags = MOUSEEVENTF_ABSOLUTE & MOUSEEVENTF_MOVE;
			events[events_size].mi.time = 0; //GetSystemTime()
			events_size++;
			break;
	}

	// Release the previously held modifier keys used to fake the event mask.
	if (event->mask & (MASK_SHIFT | MASK_CTRL | MASK_META | MASK_ALT)) {
		for (unsigned int i = 0; i < sizeof(keymask_lookup) / sizeof(UINT); i++) {
			if (event->mask & 1 << i) {
				events[events_size].type = INPUT_KEYBOARD;
				events[events_size].ki.wVk = convert_to_native_key(event->data.keyboard.keycode);
				events[events_size].ki.dwFlags = KEYEVENTF_KEYUP;
				events[events_size].ki.time = 0; // Use current system time.
				events_size++;
			}
		}
	}

	if (event->mask & (MASK_BUTTON1 | MASK_BUTTON2 | MASK_BUTTON3 | MASK_BUTTON4 | MASK_BUTTON5)) {
		events[events_size].type = INPUT_MOUSE;
		events[events_size].mi.dx = 0;	// Relative mouse movement due to
		events[events_size].mi.dy = 0;	// MOUSEEVENTF_ABSOLUTE not being set.
		events[events_size].mi.mouseData = 0x00;
		events[events_size].mi.time = 0; // Use current system time.

		if (event->mask & MASK_BUTTON1) {
			events[events_size].mi.mouseData |= MOUSEEVENTF_LEFTUP;
		}

		if (event->mask & MASK_BUTTON2) {
			events[events_size].mi.mouseData |= MOUSEEVENTF_RIGHTUP;
		}

		if (event->mask & MASK_BUTTON3) {
			events[events_size].mi.mouseData |= MOUSEEVENTF_MIDDLEUP;
		}

		if (event->mask & MASK_BUTTON4) {
			events[events_size].mi.mouseData = XBUTTON1;
			events[events_size].mi.mouseData |= MOUSEEVENTF_XUP;
		}

		if (event->mask & MASK_BUTTON5) {
			events[events_size].mi.mouseData = XBUTTON2;
			events[events_size].mi.dwFlags |= MOUSEEVENTF_XUP;
		}

		events_size++;
	}

	//Create the key release input
	//memcpy(key_events + 1, key_events, sizeof(INPUT));
	//key_events[1].ki.dwFlags |= KEYEVENTF_KEYUP;

	if (! SendInput(events_size, events, sizeof(INPUT)) ) {
		printf("Error 0x%X\n", (unsigned int) GetLastError());
	}

	free(events);
}
