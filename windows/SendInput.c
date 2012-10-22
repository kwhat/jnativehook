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

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>
#include <stdio.h>

int main(int argc, const char* argv[]) {
	SHORT code = VkKeyScanEx((TCHAR) 'B', GetKeyboardLayout(0));
	printf("Test Scan Code: %d\n", code);

	unsigned int vk_code = 0x42; //'B' Key

	//Create the key press and typed input
	INPUT key_events[2];
	key_events[0].type = INPUT_KEYBOARD;
	key_events[0].ki.wVk = vk_code;
	key_events[0].ki.wScan = MapVirtualKey(vk_code, 0); //MAPVK_VK_TO_VSC
	key_events[0].ki.dwFlags = 0x00;
	if ((vk_code >= 33 && vk_code <= 46) || (vk_code >= 91 && vk_code <= 93)) {
		//Key is an extended key.
		key_events[0].ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
	}
	key_events[0].ki.time = 0; //GetSystemTime()

	//Create the key release input
	memcpy(key_events + 1, key_events, sizeof(INPUT));
	key_events[1].ki.dwFlags |= KEYEVENTF_KEYUP;

	int i = 0;
	for (i = 0; i < 5; i++) {
		Sleep(1000) ;

		if (! SendInput(2, key_events, sizeof(INPUT)) ) {
			printf("Error 0x%X\n", (unsigned int) GetLastError());
		}
	}

	return 0;
}
