/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2014 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
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

#include <stdlib.h>
#include <uiohook.h>

#include "jni_Converter.h"
#include "jni_Globals.h"
#include "jni_Logger.h"
#include "org_jnativehook_NativeInputEvent.h"
#include "org_jnativehook_keyboard_NativeKeyEvent.h"
#include "org_jnativehook_mouse_NativeMouseEvent.h"
#include "org_jnativehook_mouse_NativeMouseWheelEvent.h"
#include "org_jnativehook_GlobalScreen.h"

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_registerNativeHook(JNIEnv *env, jclass cls) {
	// Request an instance of the GlobalScreen.
	jobject GlobalScreen_object = (*env)->CallStaticObjectMethod(
			env,
			org_jnativehook_GlobalScreen->cls,
			org_jnativehook_GlobalScreen->getInstance);

	// FIXME I dont think this check is required anymore...
	if (GlobalScreen_object != NULL) {
		hook_enable();
	}
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_unregisterNativeHook(JNIEnv *env, jclass cls) {
	// Request an instance of the GlobalScreen.
	jobject GlobalScreen_object = (*env)->CallStaticObjectMethod(
			env,
			org_jnativehook_GlobalScreen->cls,
			org_jnativehook_GlobalScreen->getInstance);

	// FIXME I dont think this check is required anymore...
	if (GlobalScreen_object != NULL) {
		hook_disable();
	}
}

JNIEXPORT jboolean JNICALL Java_org_jnativehook_GlobalScreen_isNativeHookRegistered(JNIEnv *env, jclass cls) {
	// Simple wrapper to return the hook status.
	return (jboolean) hook_is_enabled();
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_postNativeEvent(JNIEnv *env, jclass cls, jobject event) {
	// Convert the event type.
	jint javaType = (*env)->CallIntMethod(env, event, org_jnativehook_NativeInputEvent->getID);

	// Allocate memory for the virtual event and set the type.
	uiohook_event *virtualEvent = (uiohook_event *) malloc(sizeof(uiohook_event));
	jni_ConvertToNativeType(javaType, &(virtualEvent->type));

	// Convert Java event to virtual event.
	virtualEvent->mask = (unsigned int) (*env)->CallIntMethod(env, event, org_jnativehook_NativeInputEvent->getModifiers);

	switch (javaType) {
		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED:
			virtualEvent->data.keyboard.keychar = (*env)->CallIntMethod(env, event, org_jnativehook_keyboard_NativeKeyEvent->getKeyChar);
			virtualEvent->data.keyboard.keycode = VC_UNDEFINED;
			virtualEvent->data.keyboard.rawcode = 0x00;
			break;

		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED:
		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED:
			virtualEvent->data.keyboard.keychar = CHAR_UNDEFINED;
			virtualEvent->data.keyboard.keycode = (*env)->CallIntMethod(env, event, org_jnativehook_keyboard_NativeKeyEvent->getKeyCode);
			virtualEvent->data.keyboard.rawcode = 0x00;
			break;

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED:
        case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED:
        	virtualEvent->data.mouse.button = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseEvent->getButton);
			virtualEvent->data.mouse.clicks = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseEvent->getClickCount);
			virtualEvent->data.mouse.x = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseEvent->getX);
            virtualEvent->data.mouse.y = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseEvent->getY);
            break;

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED:
			virtualEvent->data.mouse.button = MOUSE_NOBUTTON;
			virtualEvent->data.mouse.clicks = 0;
		    virtualEvent->data.mouse.x = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseEvent->getX);
            virtualEvent->data.mouse.y = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseEvent->getY);
            break;

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL:
            virtualEvent->data.wheel.clicks = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseWheelEvent->parent->getClickCount);
			virtualEvent->data.wheel.x = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseWheelEvent->parent->getX);
			virtualEvent->data.wheel.y = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseWheelEvent->parent->getY);
			virtualEvent->data.wheel.type = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseWheelEvent->getScrollType);
			virtualEvent->data.wheel.amount = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseWheelEvent->getScrollAmount);
			virtualEvent->data.wheel.rotation = (*env)->CallIntMethod(env, event, org_jnativehook_mouse_NativeMouseWheelEvent->getWheelRotation);
            break;

		default:
			// TODO Should this thrown an exception?

			jni_Logger(LOG_LEVEL_WARN,	"%s [%u]: Invalid native event type!\n",
					__FUNCTION__, __LINE__);
			break;
	}

	hook_post_event(virtualEvent);
}
