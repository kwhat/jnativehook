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

#include <stdlib.h>
#include <nativehook.h>

#include "jni_Converter.h"
#include "jni_Globals.h"
#include "org_jnativehook_NativeInputEvent.h"
#include "org_jnativehook_keyboard_NativeKeyEvent.h"
#include "org_jnativehook_mouse_NativeMouseEvent.h"
#include "org_jnativehook_mouse_NativeMouseWheelEvent.h"
#include "org_jnativehook_GlobalScreen.h"

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_postNativeEvent(JNIEnv *env, jclass cls, jobject event) {
	// Convert the event type.
	unsigned int nativeType;
	jint javaType = (*env)->CallIntMethod(env, event, org_jnativehook_NativeInputEvent->getID);

	// Allocate memory for the virtual event and set the type.
	VirtualEvent *virtualEvent = (VirtualEvent *) malloc(sizeof(VirtualEvent));
	jni_ConvertToNativeType(javaType, &(virtualEvent->type));

	// Convert Java event to virtual event.
	virtualEvent->mask = (unsigned int) (*env)->CallIntMethod(env, event, org_jnativehook_NativeInputEvent->getModifiers);

	switch (javaType) {
		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED:
			virtualEvent->data.keyboard.keychar =
					(*env)->CallIntMethod(env, event, org_jnativehook_keyboard_NativeKeyEvent->getKeyChar);
			break;

		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED:
		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED:
			virtualEvent->data.keyboard.keycode = 
					(*env)->CallIntMethod(env, event, org_jnativehook_keyboard_NativeKeyEvent->getKeyCode);
			break;

		// FIXME Implement!
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED:

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED:

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL:

		default:
			break;
	}

	hook_post_event(virtualEvent);
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
