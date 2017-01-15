/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
 * https://github.com/kwhat/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>

#include <stdlib.h>
#include "NativeToJava.h"
#include "WinInputHelpers.h"

static unsigned short int current_modifiers = 0x0000;

void SetModifierMask(unsigned short int mask) {
	current_modifiers |= mask;
}

void UnsetModifierMask(unsigned short int mask) {
	current_modifiers ^= mask;
}

unsigned short int GetModifiers() {
	return current_modifiers;
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
