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

#include <jni.h>
#include <nativehook.h>

#include "jni_NativeConvert.h"
#include "org_jnativehook_NativeInputEvent.h"
#include "org_jnativehook_keyboard_NativeKeyEvent.h"

unsigned int jni_JavaKeyToNative(jint keycode, jint location) {
	unsigned int virtualkey = (unsigned int) keycode;

	switch (location) {
		case org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT:
		case org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT:
				virtualkey = (virtualkey >> 2) & 0x02FF;
			break;

		case org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD:
			virtualkey = (virtualkey >> 2) & 0x01FF;
			break;

		default:
		case org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD:
			// No conversion required.
			break;
	}

	return virtualkey;
}

jint jni_NativeToJavaKey(unsigned int virtualkey, jint *keycode, jint *location) {
	switch (virtualkey) {
		case VC_SHIFT_L:
		case VC_CONTROL_L:
		case VC_ALT_L:
		case VC_META_L:
		case VC_SUPER_L:
			*keycode = virtualkey;
			*location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;
			break;

		case VC_SHIFT_R:
		case VC_CONTROL_R:
		case VC_ALT_R:
		case VC_META_R:
		case VC_SUPER_R:
			*keycode = virtualkey & 0xFF;
			*location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;
			break;

		case VC_KP_0:
		case VC_KP_1:
		case VC_KP_2:
		case VC_KP_3:
		case VC_KP_4:
		case VC_KP_5:
		case VC_KP_6:
		case VC_KP_7:
		case VC_KP_8:
		case VC_KP_9:

		case VC_KP_UP:
		case VC_KP_DOWN:
		case VC_KP_LEFT:
		case VC_KP_RIGHT:

		case VC_KP_ENTER:
		case VC_KP_MULTIPLY:
		case VC_KP_ADD:
		case VC_KP_SEPARATOR:
		case VC_KP_SUBTRACT:
		case VC_KP_DECIMAL:
		case VC_KP_DIVIDE:
		case VC_KP_DELETE:
			*keycode = virtualkey & 0xFF;
			*location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;
			break;

		default:
			*keycode = virtualkey;
			*location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD;
			break;
	}

	return JNI_OK;
}
