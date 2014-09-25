/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2014 Alexander Barker.  All Rights Received.
 * https://github.com/kwhat/jnativehook/
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
#include "jni_Errors.h"
#include "org_jnativehook_NativeInputEvent.h"
#include "org_jnativehook_keyboard_NativeKeyEvent.h"
#include "org_jnativehook_mouse_NativeMouseEvent.h"
#include "org_jnativehook_mouse_NativeMouseWheelEvent.h"
#include "org_jnativehook_GlobalScreen.h"

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_registerNativeHook(JNIEnv *env, jclass cls) {
	int status = hook_enable();

	switch (status) {
		case UIOHOOK_SUCCESS:
			// Everything is ok.
			break;


		// System level errors.
		case UIOHOOK_ERROR_OUT_OF_MEMORY:
			jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
			break;


		// Native thread errors.
		case UIOHOOK_ERROR_THREAD_CREATE:
			jni_ThrowNativeHookException(env, status, "Failed to create native thread.");
			break;

		case UIOHOOK_ERROR_THREAD_INIT:
			jni_ThrowNativeHookException(env, status, "Failed to initialize native thread.");
			break;

		case UIOHOOK_ERROR_THREAD_START:
			jni_ThrowNativeHookException(env, status, "Failed to start native thread.");
			break;


		// Unix specific errors.
		case UIOHOOK_ERROR_X_OPEN_DISPLAY:
			jni_ThrowNativeHookException(env, status, "Failed to open X11 display.");
			break;

		case UIOHOOK_ERROR_X_RECORD_NOT_FOUND:
			jni_ThrowNativeHookException(env, status, "Unable to locate XRecord extension.");
			break;

		case UIOHOOK_ERROR_X_RECORD_ALLOC_RANGE:
			jni_ThrowNativeHookException(env, status, "Unable to allocate XRecord range.");
			break;

		case UIOHOOK_ERROR_X_RECORD_CREATE_CONTEXT:
			jni_ThrowNativeHookException(env, status, "Unable to allocate XRecord context.");
			break;

		case UIOHOOK_ERROR_X_RECORD_ENABLE_CONTEXT:
			jni_ThrowNativeHookException(env, status, "Failed to enable XRecord context.");
			break;


		// Windows specific errors.
		case UIOHOOK_ERROR_SET_WINDOWS_HOOK_EX:
			jni_ThrowNativeHookException(env, status, "Failed to register low level windows hook.");
			break;


		// Darwin specific errors.
		case UIOHOOK_ERROR_AXAPI_DISABLED:
			jni_ThrowNativeHookException(env, status, "Failed to enable access for assistive devices.");
			break;

		case UIOHOOK_ERROR_CREATE_EVENT_PORT:
			jni_ThrowNativeHookException(env, status, "Failed to create apple event port.");
			break;

		case UIOHOOK_ERROR_CREATE_RUN_LOOP_SOURCE:
			jni_ThrowNativeHookException(env, status, "Failed to create apple run loop source.");
			break;

		case UIOHOOK_ERROR_GET_RUNLOOP:
			jni_ThrowNativeHookException(env, status, "Failed to acquire apple run loop.");
			break;

		case UIOHOOK_ERROR_CREATE_OBSERVER:
			jni_ThrowNativeHookException(env, status, "Failed to create apple run loop observer.");
			break;

		// Default error.
		case UIOHOOK_FAILURE:
		default:
			jni_ThrowNativeHookException(env, status, "An unknown hook error occurred.");
			break;
	}
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_unregisterNativeHook(JNIEnv *env, jclass cls) {
	hook_disable();
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

			jni_Logger(env, LOG_LEVEL_WARN,	"%s [%u]: Invalid native event type!\n",
					__FUNCTION__, __LINE__);
			break;
	}

	hook_post_event(virtualEvent);
}
