/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

#include "JavaToNative.h"

JKeyDatum NativeToJKey(unsigned int keysym) {
	JKeyDatum jkey;
	jkey.rawcode = keysym;
	jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD;

	switch ((unsigned int) keysym) {
		case XK_KP_0:
		case XK_KP_Insert:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD0;
			goto LOCATION_NP;
		case XK_KP_1:
		case XK_KP_End:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD1;
			goto LOCATION_NP;
		case XK_KP_2:
		case XK_KP_Down:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD2;
			goto LOCATION_NP;
		case XK_KP_3:
		case XK_KP_Page_Down:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD3;
			goto LOCATION_NP;
		case XK_KP_4:
		case XK_KP_Left:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD4;
			goto LOCATION_NP;
		case XK_KP_5:
		case XK_KP_Begin:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD5;
			goto LOCATION_NP;
		case XK_KP_6:
		case XK_KP_Right:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD6;
			goto LOCATION_NP;
		case XK_KP_7:
		case XK_KP_Home:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD7;
			goto LOCATION_NP;
		case XK_KP_8:
		case XK_KP_Up:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD8;
			goto LOCATION_NP;
		case XK_KP_9:
		case XK_KP_Page_Up:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD9;
			goto LOCATION_NP;

		case XK_KP_Multiply:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MULTIPLY;
			goto LOCATION_NP;
		case XK_KP_Add:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ADD;
			goto LOCATION_NP;
		case XK_KP_Separator:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SEPARATOR;
			goto LOCATION_NP;
		case XK_KP_Subtract:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SUBTRACT;
			goto LOCATION_NP;
		case XK_KP_Enter:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ENTER;
			goto LOCATION_NP;
		case XK_KP_Decimal:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DECIMAL;
			goto LOCATION_NP;
		case XK_KP_Divide:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DIVIDE;
			goto LOCATION_NP;
		case XK_KP_Delete:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DELETE;
			goto LOCATION_NP;
		case XK_Num_Lock:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUM_LOCK;
			goto LOCATION_NP;
		case XK_Clear:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CLEAR;
			goto LOCATION_NP;
		LOCATION_NP:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;
			return jkey;



		case XK_Begin:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BEGIN;
			return jkey;

		default:
		case XK_VoidSymbol:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED;
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_UNKNOWN;
			return jkey;
	}
}

jint NativeToJButton(unsigned int button) {
	switch (button) {
		case Button1:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON1;
		case Button2:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON3;
		case Button3:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON2;
		case Button4:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON4;
		case Button5:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON5;

		default:
		case AnyButton:
			return org_jnativehook_mouse_NativeMouseEvent_NOBUTTON;
	}
}

jint NativeToJEventMask(unsigned int mask) {
	jint java_mask = 0;

	if (mask & ShiftMask)
		java_mask |= org_jnativehook_NativeInputEvent_SHIFT_MASK;

	if (mask & ControlMask)
		java_mask |= org_jnativehook_NativeInputEvent_CTRL_MASK;

	if (mask & Mod4Mask)
		java_mask |= org_jnativehook_NativeInputEvent_META_MASK;

	if (mask & Mod1Mask)
		java_mask |= org_jnativehook_NativeInputEvent_ALT_MASK;


	if (mask & Button1Mask)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON1_MASK;

	if (mask & Button2Mask)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON2_MASK;

	if (mask & Button3Mask)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON3_MASK;

	if (mask & Button4Mask)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON4_MASK;

	if (mask & Button5Mask)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON5_MASK;

	return java_mask;
}
