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
#include "jni_Globals.h"
#include "org_jnativehook_NativeInputEvent.h"
#include "org_jnativehook_keyboard_NativeKeyEvent.h"
#include "org_jnativehook_mouse_NativeMouseEvent.h"
#include "org_jnativehook_mouse_NativeMouseWheelEvent.h"

void jni_EventDispatcher(VirtualEvent *const event) {
	JNIEnv *env = NULL;

	// NOTE: This function executes on the hook thread!  If you need to block
	// please do so on another thread via your own event dispatcher.
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		// Create the global screen references up front to save time in the callback.
		jobject GlobalScreen_object = (*env)->CallStaticObjectMethod(
				env,
				org_jnativehook_GlobalScreen->cls,
				org_jnativehook_GlobalScreen->getInstance);

		if (GlobalScreen_object != NULL) {
			jobject NativeInputEvent_object = NULL;
			jint keycode, location;

			switch (event->type) {
				case EVENT_KEY_PRESSED:
					jni_ConvertToJavaKeyCode(event->data.keyboard.keycode, &keycode, &location);

					NativeInputEvent_object = (*env)->NewObject(
											env,
											org_jnativehook_keyboard_NativeKeyEvent->cls,
											org_jnativehook_keyboard_NativeKeyEvent->init,
											org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED,
											(jlong) event->time,
											(jint) event->mask,
											(jint) event->data.keyboard.scancode,
											keycode,
											(jchar) org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
											location);
					break;

				case EVENT_KEY_RELEASED:
					jni_ConvertToJavaKeyCode(event->data.keyboard.keycode, &keycode, &location);

					NativeInputEvent_object = (*env)->NewObject(
											env,
											org_jnativehook_keyboard_NativeKeyEvent->cls,
											org_jnativehook_keyboard_NativeKeyEvent->init,
											org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED,
											(jlong) event->time,
											(jint) event->mask,
											(jint) event->data.keyboard.scancode,
											keycode,
											(jchar) org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
											location);
					break;

				case EVENT_KEY_TYPED:
					jni_ConvertToJavaKeyCode(event->data.keyboard.keycode, &keycode, &location);

					NativeInputEvent_object = (*env)->NewObject(
											env,
											org_jnativehook_keyboard_NativeKeyEvent->cls,
											org_jnativehook_keyboard_NativeKeyEvent->init,
											org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED,
											(jlong) event->time,
											(jint) event->mask,
											(jint) event->data.keyboard.scancode,
											(jint) org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED,
											(jchar) event->data.keyboard.keychar,
											location);
					break;

				case EVENT_MOUSE_PRESSED:
					NativeInputEvent_object = (*env)->NewObject(
												env,
												org_jnativehook_mouse_NativeMouseEvent->cls,
												org_jnativehook_mouse_NativeMouseEvent->init,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED,
												(jlong) event->time,
												(jint) event->mask,
												(jint) event->data.mouse.x,
												(jint) event->data.mouse.y,
												(jint) event->data.mouse.clicks,
												(jint) event->data.mouse.button);
					break;

				case EVENT_MOUSE_RELEASED:
					NativeInputEvent_object = (*env)->NewObject(
												env,
												org_jnativehook_mouse_NativeMouseEvent->cls,
												org_jnativehook_mouse_NativeMouseEvent->init,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED,
												(jlong) event->time,
												(jint) event->mask,
												(jint) event->data.mouse.x,
												(jint) event->data.mouse.y,
												(jint) event->data.mouse.clicks,
												(jint) event->data.mouse.button);
					break;

				case EVENT_MOUSE_CLICKED:
					NativeInputEvent_object = (*env)->NewObject(
												env,
												org_jnativehook_mouse_NativeMouseEvent->cls,
												org_jnativehook_mouse_NativeMouseEvent->init,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED,
												(jlong) event->time,
												(jint) event->mask,
												(jint) event->data.mouse.x,
												(jint) event->data.mouse.y,
												(jint) event->data.mouse.clicks,
												(jint) event->data.mouse.button);
					break;

				case EVENT_MOUSE_MOVED:
					NativeInputEvent_object = (*env)->NewObject(
												env,
												org_jnativehook_mouse_NativeMouseEvent->cls,
												org_jnativehook_mouse_NativeMouseEvent->init,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED,
												(jlong) event->time,
												(jint) event->mask,
												(jint) event->data.mouse.x,
												(jint) event->data.mouse.y,
												(jint) event->data.mouse.clicks,
												(jint) event->data.mouse.button);
					break;

				case EVENT_MOUSE_DRAGGED:
					NativeInputEvent_object = (*env)->NewObject(
												env,
												org_jnativehook_mouse_NativeMouseEvent->cls,
												org_jnativehook_mouse_NativeMouseEvent->init,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED,
												(jlong) event->time,
												(jint) event->mask,
												(jint) event->data.mouse.x,
												(jint) event->data.mouse.y,
												(jint) event->data.mouse.clicks,
												(jint) event->data.mouse.button);
					break;

				case EVENT_MOUSE_WHEEL:
					NativeInputEvent_object = (*env)->NewObject(
												env,
												org_jnativehook_mouse_NativeMouseWheelEvent->cls,
												org_jnativehook_mouse_NativeMouseWheelEvent->init,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL,
												(jlong) event->time,
												(jint) event->mask,
												(jint) event->data.wheel.x,
												(jint) event->data.wheel.y,
												(jint) event->data.wheel.clicks,
												(jint) event->data.wheel.type,
												(jint) event->data.wheel.amount,
												(jint) event->data.wheel.rotation);
					break;
			}

			if (NativeInputEvent_object != NULL) {
				(*env)->CallVoidMethod(
						env,
						GlobalScreen_object,
						org_jnativehook_GlobalScreen->dispatchEvent,
						NativeInputEvent_object);

				(*env)->DeleteLocalRef(env, NativeInputEvent_object);
			}
		}
		else {
			// FIXME an exception should be thrown!

			#ifdef DEBUG
			fprintf(stderr, "jni_EventDispatcher(): Failed to aquire GlobalScreen singleton!\n");
			#endif
		}
	}
	else {
		// FIXME an exception should be thrown!

		#ifdef DEBUG
		fprintf(stderr, "jni_EventDispatcher(): AttachCurrentThread() failed!\n");
		#endif
	}
}
