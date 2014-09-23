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

	jclass Exception_class = NULL;
	const char *message = NULL;

	// Cant seem to figure out why org.jnativehook.NativeHookException is found but wont throw!
	Exception_class = (*env)->FindClass(env, "org/jnativehook/NativeHookException");
	message = "Test of throw new!";


	if (Exception_class == NULL) {
		jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: NativeException_object was null!\n",
				__FUNCTION__, __LINE__);
		return;
	}

	//(*env)->ThrowNew(env, Exception_class, message);
	//return;

	jmethodID init = (*env)->GetMethodID(
						env,
						Exception_class,
						"<init>",
						"(SLjava/lang/String;)V");

	if (init == NULL) {
		jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: init was null!\n",
				__FUNCTION__, __LINE__);
		return;
	}

	jobject NativeException_object = (*env)->NewObject(
				env,
				Exception_class,
				init,
				(jshort) status,
				(*env)->NewStringUTF(env, message));

	if (NativeException_object == NULL) {
		jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: NativeException_object was null!\n",
				__FUNCTION__, __LINE__);
		return;
	}

	(*env)->Throw(env, (jthrowable) NativeException_object);
	(*env)->DeleteLocalRef(env, NativeException_object);
    return;
	/*
	Exception_class = (*env)->FindClass(env, "org/jnativehook/NativeHookException");
	message = "TESTING!!!!";


	jmethodID init = (*env)->GetMethodID(
						env,
						Exception_class,
						"<init>",
						"(SLjava/lang/String;)V");
	jstring jmsg = (*env)->NewStringUTF(env, message);
	if (jmsg != NULL) {
		jobject NativeException_object = (*env)->NewObject(
				env,
				Exception_class,
				init,
				(jshort)status,
				(jstring) jmsg);

		if (NativeException_object == NULL) {
							jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: NativeException_object was null!\n",
														__FUNCTION__, __LINE__);
		}
		else {
		jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: NativeException_object was NOT null!\n",
																		__FUNCTION__, __LINE__);


		}

		(*env)->Throw(env, (jthrowable) NativeException_object);
		return;
	}
	else {
			jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: jmsg was null!\n",
					__FUNCTION__, __LINE__);
	}
	*/
    /*
	switch (status) {
		case UIOHOOK_SUCCESS:
			// Everything is ok.
			break;

		// System level errors.
		case UIOHOOK_ERROR_OUT_OF_MEMORY:
			Exception_class = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
			if (Exception_class != NULL) {
				(*env)->ThrowNew(env, Exception_class, "Failed to allocate native memory.");
			}
			break;

		// Native thread errors.
		case UIOHOOK_ERROR_THREAD_CREATE:
			Exception_class = (*env)->FindClass(env, "org/jnativehook/NativeHookException");
			message = "Failed to create native thread.";
			jmethodID init = (*env)->GetMethodID(
            					env,
            					Exception_class,
            					"<init>",
            					"(SLjava/lang/String;)V");
			jstring jmsg = (*env)->NewStringUTF(env, message);
			if (jmsg != NULL) {
				jobject NativeInputEvent_object = (*env)->NewObject(
						env,
						Exception_class,
						init,
						(jshort)status,
						(jstring) jmsg);

				(*env)->Throw(env, (jthrowable) NativeInputEvent_object);
			}

			break;

		case UIOHOOK_ERROR_THREAD_INIT:
			Exception_class = (*env)->FindClass(env, "org/jnativehook/NativeHookException");
			message = "Failed to initialize native thread.";
			break;

		case UIOHOOK_ERROR_THREAD_START:
			Exception_class = (*env)->FindClass(env, "org/jnativehook/NativeHookException");
			message = "Failed to start native thread.";
			break;

		// Unix specific errors.
		case UIOHOOK_ERROR_X_OPEN_DISPLAY:
			Exception_class = (*env)->FindClass(env, "org/jnativehook/NativeHookException");
			message = "Failed to open X11 display.";
			break;

		case UIOHOOK_ERROR_X_RECORD_NOT_FOUND:
			Exception_class = (*env)->FindClass(env, "org/jnativehook/NativeHookException");
			message = "Unable to locate XRecord extension.";
			break;

		case UIOHOOK_ERROR_X_RECORD_ALLOC_RANGE:
			Exception_class = (*env)->FindClass(env, "org/jnativehook/NativeHookException");
			message = "Failed to allocate xrecord range.";
			break;

		case UIOHOOK_ERROR_X_RECORD_CREATE_CONTEXT:
			Exception_class = (*env)->FindClass(env, "org/jnativehook/NativeHookException");
			message = "Failed to allocate xrecord context.";
			break;

		case UIOHOOK_ERROR_X_RECORD_ENABLE_CONTEXT:
			Exception_class = (*env)->FindClass(env, "org/jnativehook/NativeHookException");
			message = "Failed to allocate xrecord context.";

			jni_ThrowException(env, "org/jnativehook/NativeHookException", "Failed to enable xrecord context.");
			break;

		// Windows specific errors.
		case UIOHOOK_ERROR_SET_WINDOWS_HOOK_EX:
			jni_ThrowException(env, "org/jnativehook/NativeHookException", "Failed to register low level windows hook.");
			break;

		// Darwin specific errors.
		case UIOHOOK_ERROR_AXAPI_DISABLED:
			jni_ThrowException(env, "org/jnativehook/NativeHookException", "Failed to enable access for assistive devices.");
			break;

		case UIOHOOK_ERROR_CREATE_EVENT_PORT:
			jni_ThrowException(env, "org/jnativehook/NativeHookException", "Failed to create apple event port.");
			break;

		case UIOHOOK_ERROR_CREATE_RUN_LOOP_SOURCE:
			jni_ThrowException(env, "org/jnativehook/NativeHookException", "Failed to create apple run loop source.");
			break;

		case UIOHOOK_ERROR_GET_RUNLOOP:
			jni_ThrowException(env, "org/jnativehook/NativeHookException", "Failed to acquire apple run loop.");
			break;

		case UIOHOOK_ERROR_CREATE_OBSERVER:
			jni_ThrowException(env, "org/jnativehook/NativeHookException", "Failed to create apple run loop observer.");
			break;

		case UIOHOOK_FAILURE:
		default:
			jni_ThrowException(env, "org/jnativehook/NativeHookException", "An unknown uiohook error occurred.");
			break;
	}
    */
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

			jni_Logger(LOG_LEVEL_WARN,	"%s [%u]: Invalid native event type!\n",
					__FUNCTION__, __LINE__);
			break;
	}

	hook_post_event(virtualEvent);
}
