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
	//FIXME Use the jni globals!

	//jclass clsNativeKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
	//jclass clsNativeMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
	//jclass clsNativeMouseWheelEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseWheelEvent");
/*
	jclass clsNativeInputEvent = (*env)->FindClass(env, "org/jnativehook/NativeInputEvent");
	jmethodID idGetID = (*env)->GetMethodID(env, clsNativeInputEvent, "getID", "()I");
	jmethodID idGetModifiers = (*env)->GetMethodID(env, clsNativeInputEvent, "getModifiers", "()I");

	jint id = (*env)->CallIntMethod(env, event, idGetID);
	jint modifiers = (*env)->CallIntMethod(env, event, idGetModifiers);

	switch (id) {
		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED:
			PostKeyDownEvent();
			break;

		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED:
			PostKeyTypedEvent();
			break;

		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED:
			PostKeyUpEvent();
			break;

		default:
			break;
	}
*/
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
