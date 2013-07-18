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

#include <nativehook.h>

#include "jni_Globals.h"
#include "org_jnativehook_GlobalScreen.h"

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_postNativeEvent(JNIEnv *env, jclass cls, jobject event) {
	// Convert Java event to Virtual.
	jint modifiers = (*env)->CallIntMethod(env, event, idGetModifiers);

	// Convert the event type.
	unsigned int nativeType;
	jint javaType = (*env)->CallIntMethod(env, event, org_jnativehook_NativeInputEvent->getID);
	if (jni_ConvertToNativeType(javaType, &nativeType) != JNI_OK) {
		// throw new IllegalArgumentException?
	}

	// Convert the keycode and location.
	unsigned int nativeKeyCode;
	jint javaKeyCode = (*env)->CallIntMethod(env, event, org_jnativehook_NativeInputEvent->getKeyCode);
	jint javaKeyLocation = (*env)->CallIntMethod(env, event, org_jnativehook_NativeInputEvent->getKeyLocation);
	if (jni_ConvertToNativeKeyCode(javaKeyCode, javaKeyLocation, &nativeKeyCode)) {
		// throw new IllegalArgumentException?
	}

	// Allocate memory for the virtual events only once.
	VirtualEvent *virtual_event = (VirtualEvent *) malloc(sizeof(VirtualEvent));
	virtual_event->type = convert_to_virtual_type((unsigned int) id);
	virtual_event->mask = convert_to_virtual_mask((unsigned int) modifiers);

	keyboard_data = (KeyboardEventData *) malloc(sizeof(KeyboardEventData));
	mouse_data = (MouseEventData *) malloc(sizeof(MouseEventData));
	mouse_wheel_data = (MouseWheelEventData *) malloc(sizeof(MouseWheelEventData));

	switch (id) {
		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED:
		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED:
		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED:
			// TODO
			virtual_event->data = malloc(sizeof(KeyboardEventData));

			((KeyboardEventData) virtual_event->data)->keycode;
			((KeyboardEventData) virtual_event->data)->rawcode;
			((KeyboardEventData) virtual_event->data)->keychar;
			break;

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED:

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED:

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL:

		default:
			break;
	}

	hook_post_event(virtual_event);
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_registerNativeHook(JNIEnv *env, jclass cls) {
	// Request an instance of the GlobalScreen.
	jobject GlobalScreen_object = (*env)->CallStaticObjectMethod(
			env,
			org_jnativehook_GlobalScreen->cls,
			org_jnativehook_GlobalScreen->getInstance);

	if (GlobalScreen_object != NULL) {
		// Start the java event dispatch thread.
		(*env)->CallVoidMethod(env, GlobalScreen_object, org_jnativehook_GlobalScreen->startEventDispatcher);

		hook_enable();
	}
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_unregisterNativeHook(JNIEnv *env, jclass cls) {
	// Request an instance of the GlobalScreen.
	jobject GlobalScreen_object = (*env)->CallStaticObjectMethod(
			env,
			org_jnativehook_GlobalScreen->cls,
			org_jnativehook_GlobalScreen->getInstance);

	if (GlobalScreen_object != NULL) {
		hook_disable();

		// Start the java event dispatch thread.
		(*env)->CallVoidMethod(env, GlobalScreen_object, org_jnativehook_GlobalScreen->stopEventDispatcher);
	}
}

JNIEXPORT jboolean JNICALL Java_org_jnativehook_GlobalScreen_isNativeHookRegistered(JNIEnv *env, jclass cls) {
	// Simple wrapper to return the hook status.
	return (jboolean) hook_is_enabled();
}
