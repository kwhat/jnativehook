/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2015 Alexander Barker.  All Rights Received.
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

#include <jni.h>
#include <stdbool.h>
#include <uiohook.h>

#include "jni_Converter.h"
#include "jni_Errors.h"
#include "jni_Globals.h"
#include "jni_Logger.h"
#include "org_jnativehook_NativeInputEvent.h"
#include "org_jnativehook_keyboard_NativeKeyEvent.h"
#include "org_jnativehook_mouse_NativeMouseEvent.h"
#include "org_jnativehook_mouse_NativeMouseWheelEvent.h"

// Simple function to notify() the hook thread.
static inline void notifyHookThread(JNIEnv *env) {
	jobject hookThread_obj = (*env)->GetStaticObjectField(env,
			java_lang_Object->cls, org_jnativehook_GlobalScreen->hookThread);

	if (hookThread_obj != NULL) {
		(*env)->MonitorEnter(env, hookThread_obj);
		(*env)->CallVoidMethod(env, hookThread_obj, java_lang_Object->notify);
		(*env)->MonitorExit(env, hookThread_obj);
	}
}

// NOTE: This function executes on the hook thread!  If you need to block
// please do so on another thread via your own event dispatcher.
void jni_EventDispatcher(uiohook_event * const event) {
	JNIEnv *env;
	if ((*jvm)->GetEnv(jvm, (void **) (&env), jvm_attach_args.version)
			== JNI_OK) {
		jobject NativeInputEvent_obj = NULL;

		jint location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_UNKNOWN;
		switch (event->type) {
		case EVENT_HOOK_DISABLED:
		case EVENT_HOOK_ENABLED:
			notifyHookThread(env);
			return;

		case EVENT_KEY_PRESSED:
			if (jni_ConvertToJavaLocation(event->data.keyboard.keycode, &location) == JNI_OK) {
				jstring keyboardName = (*env)->NewStringUTF(env, event->data.keyboard.keyboardName);
				NativeInputEvent_obj =
						(*env)->NewObject(env,
								org_jnativehook_keyboard_NativeKeyEvent->cls,
								org_jnativehook_keyboard_NativeKeyEvent->init,
								org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED,
								(jlong) event->time, (jint) event->mask,
								(jint) event->data.keyboard.rawcode,
								(jint) event->data.keyboard.keycode,
								(jchar) org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
								location,
								keyboardName);
				(*env)->DeleteLocalRef(env, keyboardName);
			}
			break;

		case EVENT_KEY_RELEASED:
			if (jni_ConvertToJavaLocation(event->data.keyboard.keycode,	&location) == JNI_OK) {
				jstring keyboardName = (*env)->NewStringUTF(env, event->data.keyboard.keyboardName);
				NativeInputEvent_obj =
						(*env)->NewObject(env,
								org_jnativehook_keyboard_NativeKeyEvent->cls,
								org_jnativehook_keyboard_NativeKeyEvent->init,
								org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED,
								(jlong) event->time, (jint) event->mask,
								(jint) event->data.keyboard.rawcode,
								(jint) event->data.keyboard.keycode,
								(jchar) org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
								location,
								keyboardName);
				(*env)->DeleteLocalRef(env, keyboardName);
			}
			break;

		case EVENT_KEY_TYPED: {
			jstring keyboardName = (*env)->NewStringUTF(env, event->data.keyboard.keyboardName);
			NativeInputEvent_obj = (*env)->NewObject(env,
					org_jnativehook_keyboard_NativeKeyEvent->cls,
					org_jnativehook_keyboard_NativeKeyEvent->init,
					org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED,
					(jlong) event->time, (jint) event->mask,
					(jint) event->data.keyboard.rawcode,
					(jint) org_jnativehook_keyboard_NativeKeyEvent_VC_UNDEFINED,
					(jchar) event->data.keyboard.keychar,
					location,
					keyboardName);
			(*env)->DeleteLocalRef(env, keyboardName);
			break;
		}

		case EVENT_MOUSE_PRESSED:
			NativeInputEvent_obj = (*env)->NewObject(env,
					org_jnativehook_mouse_NativeMouseEvent->cls,
					org_jnativehook_mouse_NativeMouseEvent->init,
					org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED,
					(jlong) event->time, (jint) event->mask,
					(jint) event->data.mouse.x, (jint) event->data.mouse.y,
					(jint) event->data.mouse.clicks,
					(jint) event->data.mouse.button);
			break;

		case EVENT_MOUSE_RELEASED:
			NativeInputEvent_obj =
					(*env)->NewObject(env,
							org_jnativehook_mouse_NativeMouseEvent->cls,
							org_jnativehook_mouse_NativeMouseEvent->init,
							org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED,
							(jlong) event->time, (jint) event->mask,
							(jint) event->data.mouse.x,
							(jint) event->data.mouse.y,
							(jint) event->data.mouse.clicks,
							(jint) event->data.mouse.button);
			break;

		case EVENT_MOUSE_CLICKED:
			NativeInputEvent_obj = (*env)->NewObject(env,
					org_jnativehook_mouse_NativeMouseEvent->cls,
					org_jnativehook_mouse_NativeMouseEvent->init,
					org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED,
					(jlong) event->time, (jint) event->mask,
					(jint) event->data.mouse.x, (jint) event->data.mouse.y,
					(jint) event->data.mouse.clicks,
					(jint) event->data.mouse.button);
			break;

		case EVENT_MOUSE_MOVED:
			NativeInputEvent_obj = (*env)->NewObject(env,
					org_jnativehook_mouse_NativeMouseEvent->cls,
					org_jnativehook_mouse_NativeMouseEvent->init,
					org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED,
					(jlong) event->time, (jint) event->mask,
					(jint) event->data.mouse.x, (jint) event->data.mouse.y,
					(jint) event->data.mouse.clicks,
					(jint) event->data.mouse.button);
			break;

		case EVENT_MOUSE_DRAGGED:
			NativeInputEvent_obj = (*env)->NewObject(env,
					org_jnativehook_mouse_NativeMouseEvent->cls,
					org_jnativehook_mouse_NativeMouseEvent->init,
					org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED,
					(jlong) event->time, (jint) event->mask,
					(jint) event->data.mouse.x, (jint) event->data.mouse.y,
					(jint) event->data.mouse.clicks,
					(jint) event->data.mouse.button);
			break;

		case EVENT_MOUSE_WHEEL:
			NativeInputEvent_obj = (*env)->NewObject(env,
					org_jnativehook_mouse_NativeMouseWheelEvent->cls,
					org_jnativehook_mouse_NativeMouseWheelEvent->init,
					org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL,
					(jlong) event->time, (jint) event->mask,
					(jint) event->data.wheel.x, (jint) event->data.wheel.y,
					(jint) event->data.wheel.clicks,
					(jint) event->data.wheel.type,
					(jint) event->data.wheel.amount,
					(jint) event->data.wheel.rotation);
			break;

		default:
			// We didn't receive an event we know what to do with.
			jni_Logger(env, LOG_LEVEL_WARN,
					"%s [%u]: Invalid native event type! (%#X)\n", __FUNCTION__,
					__LINE__, event->type);
			break;
		}

		if (NativeInputEvent_obj != NULL) {
			// Dispatch the event.
			(*env)->CallStaticVoidMethod(env, org_jnativehook_GlobalScreen->cls,
					org_jnativehook_GlobalScreen->dispatchEvent,
					NativeInputEvent_obj);

			// Set the propagate flag from java.
			event->reserved = (unsigned short) (*env)->GetShortField(env,
					NativeInputEvent_obj,
					org_jnativehook_NativeInputEvent->reserved);

			// Make sure our object is garbage collected.
			(*env)->DeleteLocalRef(env, NativeInputEvent_obj);
		}
	}
}
