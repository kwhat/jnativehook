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
	HINSTANCE hWnd = NULL;
	int id = 1;
	unsigned int vk_code = 0x42;

	if (!RegisterHotKey(hWnd, id, MOD_ALT | MOD_NOREPEAT, vk_code)) {
		printf("RegisterHotKey(hWnd, %i, MOD_ALT | MOD_NOREPEAT, %X) failed\n", id, vk_code);
		return 1;
	}

	while (GetMessage(&message, NULL, 0, 0)) {
		if (message.message == WM_HOTKEY) {
			printf("Hot Key Press - %i\n", (unsigned int) message.lParam->vkCode);
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnregisterHotKey(hWnd, id);

	return 0;
}
