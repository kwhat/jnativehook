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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <nativehook.h>
#include <windows.h>

#ifndef MAPVK_VK_TO_VSC
#define MAPVK_VK_TO_VSC 0
#endif

#ifndef MAPVK_VSC_TO_VK
#define MAPVK_VSC_TO_VK 1
#endif

#ifndef MAPVK_VK_TO_CHAR
#define MAPVK_VK_TO_CHAR 2
#endif

#ifndef MAPVK_VSC_TO_VK_EX
#define MAPVK_VSC_TO_VK_EX 3
#endif

unsigned int convert_to_virtual_key(unsigned int native_keycode) {
	unsigned int virtual_keycode = (unsigned int) MapVirtualKey(native_keycode, MAPVK_VSC_TO_VK);

	return virtual_keycode;
}

unsigned int convert_to_native_key(unsigned int virtual_keycode) {
	unsigned int native_keycode = (unsigned int) MapVirtualKey(virtual_keycode, MAPVK_VK_TO_VSC);

	return native_keycode;
}


unsigned int convert_to_virtual_button(unsigned int native_button) {
	unsigned int virtual_button;

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

unsigned int convert_to_native_button(unsigned int virtual_button) {
	unsigned int native_button;

	switch (virtual_button) {
		case MOUSE_BUTTON1:	// VK_LBUTTON
		case MOUSE_BUTTON2:	// VK_RBUTTON
			native_button = virtual_button;
			break;

		case MOUSE_BUTTON3:	// VK_MBUTTON
		case MOUSE_BUTTON4:	// VK_XBUTTON1
		case MOUSE_BUTTON5:	// VK_XBUTTON2
			native_button = virtual_button + 1;
			break;

		default:
		case MOUSE_NOBUTTON:
			native_button = 0;
			break;
	}

	return native_button;
}


unsigned int convert_to_virtual_mask(unsigned int native_mask) {
	unsigned int virtual_mask = native_mask;

	return virtual_mask;
}

unsigned int convert_to_native_mask(unsigned int virtual_mask) {
	unsigned int native_mask = virtual_mask;

	return native_mask;
}
