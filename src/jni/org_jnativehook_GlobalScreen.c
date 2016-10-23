/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2016 Alexander Barker.  All Rights Received.
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


JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_00024NativeHookThread_enable(JNIEnv *env, jobject Thread_obj) {
	int status = hook_run();

	switch (status) {
		// System level errors.
		case UIOHOOK_ERROR_OUT_OF_MEMORY:
			jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
			break;

		// X11 specific errors.
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
			jni_ThrowNativeHookException(env, status, "An unknown hook error occurred.");
			break;
	}
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_00024NativeHookThread_disable(JNIEnv *env, jobject Thread_obj) {
	int status = hook_stop();

	// Only a handful of the total errors are possible on stop.
	switch (status) {
		// System level errors.
		case UIOHOOK_ERROR_OUT_OF_MEMORY:
			jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
			break;

		// Unix specific errors.
		case UIOHOOK_ERROR_X_RECORD_GET_CONTEXT:
			jni_ThrowNativeHookException(env, status, "Failed to get XRecord context.");
			break;


		// Windows specific errors.
		// There are no Windows specific errors at this time.


		// Darwin specific errors.
		// There are no Darwin specific errors at this time.

		// Default error.
		case UIOHOOK_FAILURE:
			jni_ThrowNativeHookException(env, status, "An unknown hook error occurred.");
			break;
	}
}

/*
 * Class:     org_jnativehook_GlobalScreen
 * Method:    postNativeEvent
 * Signature: (Lorg/jnativehook/NativeInputEvent;)V
 */
JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_postNativeEvent(JNIEnv *env, jclass GlobalScreen_cls, jobject NativeInputEvent_obj) {
	// Get the event type.
	jint javaType = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_NativeInputEvent->getID);

	// Allocate memory for the virtual event and set the type.
	uiohook_event virtualEvent;
	jni_ConvertToNativeType(javaType, &virtualEvent.type);

	// Convert Java event to virtual event.
	virtualEvent.mask = (unsigned int) (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_NativeInputEvent->getModifiers);

	switch (javaType) {
		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED:
			virtualEvent.data.keyboard.keychar = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_keyboard_NativeKeyEvent->getKeyChar);
			virtualEvent.data.keyboard.keycode = VC_UNDEFINED;
			virtualEvent.data.keyboard.rawcode = 0x00;
			break;

		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED:
		case org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED:
			virtualEvent.data.keyboard.keychar = CHAR_UNDEFINED;
			virtualEvent.data.keyboard.keycode = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_keyboard_NativeKeyEvent->getKeyCode);
			virtualEvent.data.keyboard.rawcode = 0x00;
			break;

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED:
		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED:
			virtualEvent.data.mouse.x = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseEvent->getX);
			virtualEvent.data.mouse.y = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseEvent->getY);
			virtualEvent.data.mouse.clicks = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseEvent->getClickCount);
			virtualEvent.data.mouse.button = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseEvent->getButton);
			break;

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED:
			virtualEvent.data.mouse.x = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseEvent->getX);
			virtualEvent.data.mouse.y = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseEvent->getY);
			virtualEvent.data.mouse.button = MOUSE_NOBUTTON;
			virtualEvent.data.mouse.clicks = 0;
			break;

		case org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL:
			virtualEvent.data.wheel.x = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseWheelEvent->parent->getX);
			virtualEvent.data.wheel.y = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseWheelEvent->parent->getY);
			virtualEvent.data.wheel.clicks = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseWheelEvent->parent->getClickCount);
			virtualEvent.data.wheel.type = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseWheelEvent->getScrollType);
			virtualEvent.data.wheel.amount = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseWheelEvent->getScrollAmount);
			virtualEvent.data.wheel.rotation = (*env)->CallIntMethod(env, NativeInputEvent_obj, org_jnativehook_mouse_NativeMouseWheelEvent->getWheelRotation);
			break;

		default:
			// TODO Should this thrown an exception?
			jni_Logger(env, LOG_LEVEL_WARN,	"%s [%u]: Invalid native event type! (%#X)\n",
					__FUNCTION__, __LINE__, javaType);
			break;
	}

	hook_post_event(&virtualEvent);
}


/*
 * Class:     org_jnativehook_GlobalScreen
 * Method:    getNativeMonitors
 * Signature: ()[Lorg/jnativehook/MonitorInfo;
 */
JNIEXPORT jobjectArray JNICALL Java_org_jnativehook_GlobalScreen_getNativeMonitors(JNIEnv *env, jclass GlobalScreen_cls) {
	unsigned char count;
	screen_data *screens = hook_create_screen_info(&count);

	jobjectArray result = (*env)->NewObjectArray(env, (jsize) count, org_jnativehook_NativeMonitorInfo->cls, NULL);
 	if (result != NULL) {
		for (int i = 0; i < count; i++) {
			jobject monitor = (*env)->NewObject(
					env,
					org_jnativehook_NativeMonitorInfo->cls,
					org_jnativehook_NativeMonitorInfo->init,
					(jshort) screens[i].number,
					(jint) screens[i].x,
					(jint) screens[i].y,
					(jshort) screens[i].width,
					(jshort) screens[i].height);

			(*env)->SetObjectArrayElement(env, result, (jsize) i, monitor);
		}
 	}
 	else {
 		jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
 	}

 	return result;
}

/*
 * Class:     org_jnativehook_GlobalScreen
 * Method:    getAutoRepeatRate
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv *env, jclass GlobalScreen_cls) {
	jobject result = NULL;

	// Set the native keyboard auto repeat rate.
	long rate = hook_get_auto_repeat_rate();
	if (rate >= 0) {
		result = (*env)->NewObject(
				env,
				java_lang_Integer->cls,
				java_lang_Integer->init,
				(jint) rate);
	}
	else {
		jni_Logger(env, LOG_LEVEL_WARN,	"%s [%u]: Invalid result returned from hook_get_auto_repeat_rate()!\n",
    			__FUNCTION__, __LINE__);
	}

	return result;
}

/*
 * Class:     org_jnativehook_GlobalScreen
 * Method:    getAutoRepeatDelay
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv *env, jclass GlobalScreen_cls) {
	jobject result = NULL;

	long delay = hook_get_auto_repeat_delay();
	if (delay >= 0) {
		result = (*env)->NewObject(
				env,
				java_lang_Integer->cls,
				java_lang_Integer->init,
				(jint) delay);
	}
	else {
		jni_Logger(env, LOG_LEVEL_WARN,	"%s [%u]: Invalid result returned from hook_get_auto_repeat_delay()!\n",
				__FUNCTION__, __LINE__);
	}

	return result;
}

/*
 * Class:     org_jnativehook_GlobalScreen
 * Method:    getPointerAccelerationMultiplier
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationMultiplier(JNIEnv *env, jclass GlobalScreen_cls) {
    jobject result = NULL;

	long multiplier = hook_get_pointer_acceleration_multiplier();
	if (multiplier >= 0) {
		result = (*env)->NewObject(
				env,
				java_lang_Integer->cls,
				java_lang_Integer->init,
				(jint) multiplier);
	}
	else {
		jni_Logger(env, LOG_LEVEL_WARN,	"%s [%u]: Invalid result returned from hook_get_pointer_acceleration_multiplier()!\n",
				__FUNCTION__, __LINE__);
	}

	return result;
}

/*
 * Class:     org_jnativehook_GlobalScreen
 * Method:    getPointerAccelerationThreshold
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationThreshold(JNIEnv *env, jclass GlobalScreen_cls) {
	jobject result = NULL;

	long threshold = hook_get_pointer_acceleration_threshold();
	if (threshold >= 0) {
		result = (*env)->NewObject(
				env,
				java_lang_Integer->cls,
				java_lang_Integer->init,
				(jint) threshold);
	}
	else {
		jni_Logger(env, LOG_LEVEL_WARN,	"%s [%u]: Invalid result returned from hook_get_pointer_acceleration_threshold()!\n",
				__FUNCTION__, __LINE__);
	}

	return result;
}

/*
 * Class:     org_jnativehook_GlobalScreen
 * Method:    getPointerSensitivity
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_org_jnativehook_GlobalScreen_getPointerSensitivity(JNIEnv *env, jclass GlobalScreen_cls) {
	jobject result = NULL;

	long sensitivity = hook_get_pointer_sensitivity();
	if (sensitivity >= 0) {
		result = (*env)->NewObject(
				env,
				java_lang_Integer->cls,
				java_lang_Integer->init,
				(jint) sensitivity);
	}
	else {
		jni_Logger(env, LOG_LEVEL_WARN,	"%s [%u]: Invalid result returned from hook_get_pointer_sensitivity()!\n",
				__FUNCTION__, __LINE__);
	}

	return result;
}

/*
 * Class:     org_jnativehook_GlobalScreen
 * Method:    getMultiClickTime
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_org_jnativehook_GlobalScreen_getMultiClickIterval(JNIEnv *env, jclass GlobalScreen_cls) {
	jobject result = NULL;

	long clicktime = hook_get_multi_click_time();
	if (clicktime >= 0) {
		result = (*env)->NewObject(
				env,
				java_lang_Integer->cls,
				java_lang_Integer->init,
				(jint) clicktime);
	}
	else {
		jni_Logger(env, LOG_LEVEL_WARN,	"%s [%u]: Invalid result returned from hook_get_multi_click_time()!\n",
				__FUNCTION__, __LINE__);
	}

	return result;
}
