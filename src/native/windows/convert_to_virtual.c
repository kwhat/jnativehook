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

#include <config.h>
#include <nativehook.h>
#include <windows.h>

#include "scancode_table.h"

unsigned int convert_to_virtual_key(unsigned int scancode) {
	return scan_code_lookup[scancode];
}

unsigned int convert_to_virtual_button(unsigned int native_button) {
	unsigned char virtual_button;

	switch (native_button) {
		case VK_LBUTTON:	// MOUSE_BUTTON1
		case VK_RBUTTON:	// MOUSE_BUTTON2
			virtual_button = native_button;
			break;

		case VK_MBUTTON:	// MOUSE_BUTTON3
		case VK_XBUTTON1:	// MOUSE_BUTTON4
		case VK_XBUTTON2:	// MOUSE_BUTTON5
			// offset by 1
			virtual_button = native_button - 1;
			break;

		default:
			virtual_button = MOUSE_NOBUTTON;
			break;
	}

	return virtual_button;
}

unsigned int convert_to_virtual_mask(unsigned int native_mask) {
	unsigned int virtual_mask = native_mask;

	return virtual_mask;
}
