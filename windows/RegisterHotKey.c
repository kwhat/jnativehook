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

int main( int argc, const char* argv[] ) {
	MSG message;
	HWND hWnd = NULL;
	int id = 1; //Hot key unique ID.
	unsigned int vk_code = 0x42; //'B' Key

	//Attempt the register a global hot key for virtual code 0x42 and no modifiers.
	if (!RegisterHotKey(hWnd, id, 0x0000, vk_code)) {
		printf("RegisterHotKey(hWnd, %i, 0x0000, %i) failed\n", id, vk_code);
		return 1;
	}

	//Loop over messages looking for hot key events.
	while (GetMessage(&message, NULL, 0, 0)) {
		if (message.message == WM_HOTKEY) {
			printf("Key Press - %i\n", (unsigned int) HIWORD(message.lParam));
		}
	}

	//Unregister the hot key ID.
	UnregisterHotKey(hWnd, id);

	return 0;
}
