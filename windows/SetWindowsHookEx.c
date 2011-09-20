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

HHOOK handleKeyboardHook = NULL, handleMouseHook = NULL;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//MS Keyboard Event Struct Data
	KBDLLHOOKSTRUCT * kbhook = (KBDLLHOOKSTRUCT *) lParam;

	switch(wParam) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (kbhook->vkCode == VK_ESCAPE) {
				PostQuitMessage(0);
			}

			printf("Key Press - %i\n", (unsigned int) kbhook->vkCode);
		break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			printf("Key Release - %i\n", (unsigned int) kbhook->vkCode);
		break;
	}

	return CallNextHookEx(handleKeyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//MS Mouse Event Struct Data
	MSLLHOOKSTRUCT * mshook = (MSLLHOOKSTRUCT *) lParam;

	//Event Data
	unsigned int vk_code = 0;

	switch(wParam) {
		case WM_LBUTTONDOWN:
			vk_code = VK_LBUTTON;
		goto BUTTONDOWN;

		case WM_RBUTTONDOWN:
			vk_code = VK_RBUTTON;
		goto BUTTONDOWN;

		case WM_MBUTTONDOWN:
			vk_code = VK_MBUTTON;
		goto BUTTONDOWN;

		case WM_XBUTTONDOWN:
		case WM_NCXBUTTONDOWN:
			if (HIWORD(mshook->mouseData) == XBUTTON1) {
				vk_code = VK_XBUTTON1;
			}
			else if (HIWORD(mshook->mouseData) == XBUTTON2) {
				vk_code = VK_XBUTTON2;
			}

		BUTTONDOWN:
			printf("Button Press - %i\n", vk_code);
		break;

		case WM_LBUTTONUP:
			vk_code = VK_LBUTTON;
		goto BUTTONUP;

		case WM_RBUTTONUP:
			vk_code = VK_RBUTTON;
		goto BUTTONUP;

		case WM_MBUTTONUP:
			vk_code = VK_MBUTTON;
		goto BUTTONUP;

		case WM_XBUTTONUP:
		case WM_NCXBUTTONUP:
			if (HIWORD(mshook->mouseData) == XBUTTON1) {
				vk_code = VK_XBUTTON1;
			}
			else if (HIWORD(mshook->mouseData) == XBUTTON2) {
				vk_code = VK_XBUTTON2;
			}

		BUTTONUP:
			printf("Button Release - %i\n", vk_code);
		break;

		case WM_MOUSEMOVE:
			printf("Motion Notify - %i, %i\n", (unsigned int) mshook->pt.x, (unsigned int) mshook->pt.y);
		break;

		default:
			printf("Unknown Event - %x\n", wParam);
		break;
	}

	return CallNextHookEx(handleMouseHook, nCode, wParam, lParam);
}

int main( int argc, const char* argv[] ) {
	//Get the handle of this process.
	HINSTANCE hInst = GetModuleHandle(NULL);

	//Setup the low level keyboard hook.
	handleKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0);
	if (handleKeyboardHook == NULL) {
		printf("SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0) failed\n");
		return 1;
	}

	//Setup the low level mouse hook.
	handleMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0);
	if (handleMouseHook == NULL) {
		printf("SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0) failed\n");
		return 1;
	}

	//Block while we wait for a WM_QUIT messgae to be delivered.
	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	//Unhook the low level keyboard.
	UnhookWindowsHookEx(handleKeyboardHook);

	//Unhook the low level mouse.
	UnhookWindowsHookEx(handleMouseHook);

	return 0;
}
