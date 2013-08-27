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

#include <X11/X.h>

unsigned int convert_to_virtual_key(unsigned int native_keycode) {
	unsigned int virtual_keycode = keycode_to_scancode(native_keycode);

	return virtual_keycode;
}

unsigned int convert_to_native_key(unsigned int virtual_keycode) {
	unsigned int native_keycode = scancode_to_keycode(virtual_keycode);

	return native_keycode;
}

unsigned int convert_to_virtual_button(unsigned int native_button) {
	unsigned int virtual_button = native_button;

	return virtual_button;
}

unsigned int convert_to_native_button(unsigned short int virtual_button) {
	unsigned int native_button = virtual_button;

	return native_button;
}

unsigned int convert_to_virtual_mask(unsigned int native_mask) {
	unsigned int virtual_mask = 0x0000;

	if (native_mask & ShiftMask)	virtual_mask |= MASK_SHIFT_L;
	if (native_mask & ControlMask)	virtual_mask |= MASK_CTRL_L;
	if (native_mask & Mod4Mask)		virtual_mask |= MASK_META_L;
	if (native_mask & Mod1Mask)		virtual_mask |= MASK_ALT_L;

	if (native_mask & Button1Mask)	virtual_mask |= MASK_BUTTON1;
	if (native_mask & Button2Mask)	virtual_mask |= MASK_BUTTON2;
	if (native_mask & Button3Mask)	virtual_mask |= MASK_BUTTON3;
	if (native_mask & Button4Mask)	virtual_mask |= MASK_BUTTON4;
	if (native_mask & Button5Mask)	virtual_mask |= MASK_BUTTON5;

	return virtual_mask;
}

unsigned short int convert_to_native_mask(unsigned short int virtual_mask) {
	unsigned int native_mask = 0x0000;

	if (virtual_mask & MASK_SHIFT_L)	native_mask |= ShiftMask;
	if (virtual_mask & MASK_CTRL_L)		native_mask |= ControlMask;
	if (virtual_mask & MASK_META_L)		native_mask |= Mod4Mask;
	if (virtual_mask & MASK_ALT_L)		native_mask |= Mod1Mask;

	if (virtual_mask & MASK_BUTTON1)	native_mask |= Button1Mask;
	if (virtual_mask & MASK_BUTTON2)	native_mask |= Button2Mask;
	if (virtual_mask & MASK_BUTTON3)	native_mask |= Button3Mask;
	if (virtual_mask & MASK_BUTTON4)	native_mask |= Button4Mask;
	if (virtual_mask & MASK_BUTTON5)	native_mask |= Button5Mask;

	return native_mask;
}
