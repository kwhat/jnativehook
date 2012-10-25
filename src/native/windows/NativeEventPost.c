/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

#include <stdio.h>
#include <X11/Xlib.h>

void PostKeyDownEvent(BYTE vkCode, unsigned int state, Time time) {
	//Create the key press and typed input
	INPUT key_events[2];
	key_events[0].type = INPUT_KEYBOARD;
	key_events[0].ki.wVk = vkCode;
	key_events[0].ki.wScan = MapVirtualKey(vkCode, 0); //MAPVK_VK_TO_VSC
	key_events[0].ki.dwFlags = 0x00;
	if ((vkCode >= 33 && vkCode <= 46) || (vkCode >= 91 && vkCode <= 93)) {
		//Key is an extended key.
		key_events[0].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
	}
	key_events[0].ki.time = 0; //GetSystemTime()

	//Create the key release input
	memcpy(key_events + 1, key_events, sizeof(INPUT));
	key_events[1].ki.dwFlags |= KEYEVENTF_KEYUP;

	if (! SendInput(2, key_events, sizeof(INPUT)) ) {
		printf("Error 0x%X\n", (unsigned int) GetLastError());
	}
}

void PostKeyUpEvent(KeySym keysym, unsigned int state, Time time) {
	
}

void PostKeyTypedEvent(wchar_t wchar, Time time) {
	SHORT code = VkKeyScanEx((TCHAR) 'B', GetKeyboardLayout(0));
	printf("Test Scan Code: %d\n", code);
}
