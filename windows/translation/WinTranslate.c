/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

WORD KeyCodeToKeySym(UINT scancode, UINT keycode) {
	WORD keysym = 0x00;
	
	PBYTE lpKeyState;
	if (GetKeyboardState(lpKeyState)) {
		LPWORD lpChar;
		int ret = ToAscii(keycode, scancode, lpKeyState, lpChar, 0);
		
		keysym = *lpChar;
	}
	
	return keysym;
}

int main(int argc, const char * argv[]) {
	WORD keysym = KeyCodeToKeySym(48, VK_B);
	
	printf("KeySym: %s\n", GetKeyNameText(scancode, keysym));
	
	UINT scancode = MapVirtualKey(VK_B, MAPVK_VK_TO_VSC);
	char buffer[256];
	if (GetKeyNameTextW(scancode, buffer, 256) > 0) {
		printf("KeySym: %ls\n", buffer);
	}
	return EXIT_SUCCESS;
}
