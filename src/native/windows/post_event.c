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

NATIVEHOOK_API void hook_post_event(VirtualEvent * const event) {
	char buffer[4];
	
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

	for (unsigned int i = 0; i < sizeof(btnmask_lookup) / sizeof(unsigned int); i++) {
		if (event->mask & btnmask_lookup[i]) {
			events[events_count].type = INPUT_MOUSE;
			events[events_count].mi.dx = 0;
			events[events_count].mi.dy = 0;
			events[events_count].mi.mouseData = ;
			events[events_count].mi.dwFlags = 0x00;
			events[events_count].mi.time = 0; //GetSystemTime()
			events_count++;
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
