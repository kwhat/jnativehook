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

#include <stdlib.h>
#include "NativeToJava.h"
#include "WinInputHelpers.h"

static BYTE * keyboard_map;

void SetModifier(unsigned short int vkCode) {
	BYTE mask = 1 << 7;
	switch (vkCode) {
		case VK_CAPITAL:
		case VK_NUMLOCK:
		case VK_SCROLL:
			mask = 1;
			break;

		case VK_LSHIFT:
		case VK_RSHIFT:
			keyboard_map[VK_SHIFT] |= mask;
			break;
		case VK_LCONTROL:
		case VK_RCONTROL:
			keyboard_map[VK_CONTROL] |= mask;
			break;

		case VK_LMENU:
		case VK_RMENU:
			keyboard_map[VK_MENU] |= mask;
			break;
	}
	
	keyboard_map[vkCode] |= mask;
}

void UnsetModifier(unsigned short int vkCode) {
	BYTE mask = 1 << 7;
	switch (vkCode) {
		case VK_CAPITAL:
		case VK_NUMLOCK:
		case VK_SCROLL:
			mask = 1;
			break;
	}

	if ((vkCode == VK_LSHIFT && ! (keyboard_map[VK_RSHIFT] & mask)) ||
		(vkCode == VK_RSHIFT && ! (keyboard_map[VK_LSHIFT] & mask))) {
		keyboard_map[VK_SHIFT] ^= mask;
	}
	else if ((vkCode == VK_LCONTROL && ! (keyboard_map[VK_RCONTROL] & mask)) ||
			(vkCode == VK_RCONTROL && ! (keyboard_map[VK_LCONTROL] & mask))) {
		keyboard_map[VK_CONTROL] ^= mask;
	}
	else if ((vkCode == VK_LMENU && ! (keyboard_map[VK_RMENU] & mask)) ||
			(vkCode == VK_RMENU && ! (keyboard_map[VK_LMENU] & mask))) {
		keyboard_map[VK_MENU] ^= mask;
	}
	
	keyboard_map[vkCode] ^= mask;
}

unsigned short int GetModifierState(unsigned short int vkCode) {
	BYTE mask = 1 << 7;
	switch (vkCode) {
		case VK_CAPITAL:
		case VK_NUMLOCK:
		case VK_SCROLL:
			mask = 1;
			break;
	}

	return keyboard_map[vkCode] &= mask;;
}

int KeycodeToUnicode(unsigned short int vkCode, unsigned short int scanCode, WCHAR * buffer, unsigned short int buffer_size) {
	return ToUnicode(vkCode, scanCode, keyboard_map, buffer, buffer_size, 0);
}

unsigned short int GetScrollWheelType() {
	unsigned short int value;
	UINT wheeltype;

	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &wheeltype, 0);
	if (wheeltype == WHEEL_PAGESCROLL) {
		value = org_jnativehook_mouse_NativeMouseWheelEvent_WHEEL_BLOCK_SCROLL;
	}
	else {
		value = org_jnativehook_mouse_NativeMouseWheelEvent_WHEEL_UNIT_SCROLL;
	}

	return value;
}

unsigned short int GetScrollWheelAmount() {
	unsigned short int value;
	UINT wheelamount;

	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &wheelamount, 0);
	if (wheelamount == WHEEL_PAGESCROLL) {
		value = 1;
	}
	else {
		value = (unsigned short int) wheelamount;
	}

	return value;
}


void LoadInputHelper() {
	keyboard_map = malloc(256);
	if (!GetKeyboardState(keyboard_map)) {
		//FIXME Thorw exception!
		printf("Throw Error!!!\n");
	}

}

void UnloadInputHelper() {
	free(keyboard_map);
}
