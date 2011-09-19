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

HINSTANCE hInst = NULL;
HHOOK handleMouseHook = NULL;


int main( int argc, const char* argv[] ) {
	//Get the handle of this process.
	hInst = GetModuleHandle(NULL);

	//Setup the low level keyboard hook.
	/*
	HHOOK handleKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0);
	if (handleKeyboardHook == NULL) {
		printf("SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0) failed\n");
		return 1;
	}
	 */


	HINSTANCE hinstDLL = LoadLibrary(TEXT("HookExCallback.dll"));
	HOOKPROC hkprcSysMsg = (HOOKPROC) GetProcAddress(hinstDLL, "LowLevelProc");
	if(NULL == hkprcSysMsg)
	   {
	      printf("NULL\n");
	   }

	//Setup the low level mouse hook.
	handleMouseHook = SetWindowsHookEx(WH_MOUSE_LL, hkprcSysMsg, hinstDLL, 0);
	if (handleMouseHook == NULL) {
		printf("SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, hInst, 0) failed (%i)\n", GetLastError());

		return FALSE;
	}

	//Block while we wait for a WM_QUIT message to be delivered.
	MSG message;
	while (GetMessage(&message, NULL, WM_QUIT, WM_QUIT)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	//Unhook the low level keyboard.
	//UnhookWindowsHookEx(handleKeyboardHook);

	//Unhook the low level mouse.
	UnhookWindowsHookEx(handleMouseHook);

	FreeLibrary(hinstDLL);

	return 0;
}
