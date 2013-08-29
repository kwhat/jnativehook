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

#include "jni_Converter.h"
#include "org_jnativehook_NativeInputEvent.h"
#include "org_jnativehook_keyboard_NativeKeyEvent.h"
#include "org_jnativehook_mouse_NativeMouseEvent.h"
#include "org_jnativehook_mouse_NativeMouseWheelEvent.h"


jint jni_ConvertToJavaType(EventType nativeType, jint *javaType) {
	jint status = JNI_OK;

	if (nativeType < EVENT_MOUSE_PRESSED) {
		*javaType = org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST + (nativeType - 1);
	}
	else if (nativeType <= EVENT_MOUSE_WHEEL) {
		*javaType = org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_FIRST + (nativeType - 1);
	}
	else {
		*javaType = 0;
		status = JNI_ERR;
	}

	return status;
}


jint jni_ConvertToNativeType(jint javaType, EventType *nativeType) {
	jint status = JNI_OK;

	if (javaType < org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_FIRST) {
		*nativeType = (javaType - 1) - org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST;
	}
	else if (javaType <= org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_LAST) {
		*nativeType = (javaType - 1) -
				org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_FIRST + 
				(org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_LAST - 
				org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST);
	}
	else {
		*nativeType = 0;
		status = JNI_ERR;
	}

	return status;
}


jint jni_ConvertToNativeKeyCode(jint javaKeyCode, jint javaKeyLocation, unsigned short *nativeKeyCode) {
	jint status = JNI_OK;

	// FIXME There should be range checking on this.
	*nativeKeyCode = (unsigned short) javaKeyCode;

	return status;
}


jint jni_ConvertToJavaLocation(unsigned short int nativeKeyCode, jint *javaKeyLocation) {
	jint status = JNI_OK;

	switch (nativeKeyCode) {
		case VC_SHIFT_L:
		case VC_CONTROL_L:
		case VC_ALT_L:
		case VC_META_L:
			*javaKeyLocation = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;
			break;

		case VC_SHIFT_R:
		case VC_CONTROL_R:
		case VC_ALT_R:
		case VC_META_R:
			*javaKeyLocation = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;
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

		case VC_KP_ENTER:
		case VC_KP_MULTIPLY:
		case VC_KP_ADD:
		case VC_KP_SEPARATOR:
		case VC_KP_SUBTRACT:
		case VC_KP_DIVIDE:
			*javaKeyLocation = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;
			break;

		default:
			*javaKeyLocation = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD;
			break;
	}

	return status;
}
