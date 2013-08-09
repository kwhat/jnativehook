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

#include <windows.h>

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

NATIVEHOOK_API void hook_post_event(VirtualEvent * const event) {
	unsigned char events_size = 0, events_max = 28;
	INPUT *events = malloc(sizeof(INPUT) * events_max);
	
	// XTest does not have modifier support, so we fake it by depressing the
	// appropriate modifier keys.
	for (unsigned int i = 0; i < sizeof(keymask_lookup) / sizeof(KeySym); i++) {
		if (event->mask & 1 << i) {
			events[events_count].type = INPUT_KEYBOARD;
			events[events_count].ki.wVk = vkCode;
			events[events_count].ki.wScan = MapVirtualKey(vkCode, 0); //MAPVK_VK_TO_VSC
			events[events_count].ki.dwFlags = 0x00;
			events[events_count].ki.time = 0; //GetSystemTime()
			events_count++;
		}
	}

	if (event->mask & (MASK_BUTTON1 | MASK_BUTTON2 | MASK_BUTTON3 | MASK_BUTTON4 | MASK_BUTTON5)) {
		events[events_count].type = INPUT_MOUSE;
		events[events_count].mi.dx = 0;	// Relative mouse movement due to 
		events[events_count].mi.dy = 0;	// MOUSEEVENTF_ABSOLUTE not being set.
		events[events_count].mi.mouseData = 0x00;
		events[events_count].mi.time = 0; //GetSystemTime()
		
		if (event->mask & MASK_BUTTON1) {
			events[events_count].mi.mouseData &= MOUSEEVENTF_LEFTDOWN;
		}
		
		if (event->mask & MASK_BUTTON2) {
			events[events_count].mi.mouseData &= MOUSEEVENTF_RIGHTDOWN;
		}
		
		if (event->mask & MASK_BUTTON3) {
			events[events_count].mi.mouseData &= MOUSEEVENTF_MIDDLEDOWN;
		}
		
		if (event->mask & MASK_BUTTON4) {
			events[events_count].mi.mouseData = XBUTTON1;
			events[events_count].mi.mouseData &= MOUSEEVENTF_XDOWN;
		}
		
		if (event->mask & MASK_BUTTON5) {
			events[events_count].mi.mouseData = XBUTTON2;
			events[events_count].mi.dwFlags &= MOUSEEVENTF_XDOWN;
		}
		
		events_count++;
	}

	switch (event->type) {
		case EVENT_KEY_PRESSED:
			is_press = True;
			goto EVENT_KEY;

		case EVENT_KEY_TYPED:
			// Need to convert a wchar_t to keysym!
			char buffer[4];
			snprintf(buffer, 4, "%lc", event->data.keyboard.keychar);

			event->type = EVENT_KEY_PRESSED;
			event->data.keyboard.keycode = convert_to_virtual_key(XStringToKeysym(buffer));
			event->data.keyboard.keychar = CHAR_UNDEFINED;
			hook_post_event(event);

		case EVENT_KEY_RELEASED:
			is_press = False;

		EVENT_KEY:
			events[events_count].type = INPUT_KEYBOARD;
			events[events_count].ki.wVk = vkCode;
			events[events_count].ki.wScan = MapVirtualKey(vkCode, 0); //MAPVK_VK_TO_VSC
			events[events_count].ki.dwFlags = 0x00;
			if ((vkCode >= 33 && vkCode <= 46) || (vkCode >= 91 && vkCode <= 93)) {
				//Key is an extended key.
				key_events[0].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
			}
			events[events_count].ki.time = 0; //GetSystemTime()
			events_count++;
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
			events[events_count].type = INPUT_MOUSE;
			events[events_count].mi.dx = event->data.mouse.x;
			events[events_count].mi.dy = event->data.mouse.y;
			events[events_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE & MOUSEEVENTF_MOVE;
			events[events_count].mi.time = 0; //GetSystemTime()
			events_count++;
			break;

		case EVENT_MOUSE_DRAGGED:
			// The button masks are all applied with the modifier masks.

		case EVENT_MOUSE_MOVED:
			events[events_count].type = INPUT_MOUSE;
			events[events_count].mi.dx = event->data.mouse.x;
			events[events_count].mi.dy = event->data.mouse.y;
			events[events_count].mi.dwFlags = MOUSEEVENTF_ABSOLUTE & MOUSEEVENTF_MOVE;
			events[events_count].mi.time = 0; //GetSystemTime()
			events_count++;
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
	
	//Create the key release input
	memcpy(key_events + 1, key_events, sizeof(INPUT));
	key_events[1].ki.dwFlags |= KEYEVENTF_KEYUP;

	if (! SendInput(events_count, events, sizeof(INPUT)) ) {
		printf("Error 0x%X\n", (unsigned int) GetLastError());
	}
	
	free(events);
}

void PostKeyTypedEvent(wchar_t wchar, Time time) {
	SHORT code = VkKeyScanEx((TCHAR) 'B', GetKeyboardLayout(0));
	printf("Test Scan Code: %d\n", code);
}
