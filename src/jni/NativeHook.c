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

// GlobalScreen object.
static jobject org_jnativehook_GlobalScreen;

int dispatch_proc(VirtualEvent * const event) {
	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		// Create the global screen references up front to save time in the callback.
		jobject org_jnativehook_GlobalScreen = (*env)->CallStaticObjectMethod(
				env,
				clsGlobalScreen,
				idGetInstance);

		if (org_jnativehook_GlobalScreen != NULL) {
			// Callback and start native event dispatch thread
			(*env)->CallVoidMethod(env, org_jnativehook_GlobalScreen, idStartEventDispatcher);

			// Call Thread.currentThread().setName("JNativeHook Native Hook");
			//jobject objCurrentThread = (*env)->CallStaticObjectMethod(env, clsThread, idCurrentThread);
			//(*env)->CallVoidMethod(env, objCurrentThread, idSetName, (*env)->NewStringUTF(env, "JNativeHook Native Hook"));
			//(*env)->DeleteLocalRef(env, objCurrentThread);

			switch (event->type) {
				case EVENT_KEY_PRESSED:
					objInputEvent = (*env)->NewObject(
											env,
											clsKeyEvent,
											idKeyEvent,
											org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED,
											(jlong) event->time,
											event->mask,
											event->data->rawcode,
											event->data->keycode,
											org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
											jkey.location);

					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objInputEvent);
					(*env)->DeleteLocalRef(env, objInputEvent);
					break;

				case EVENT_KEY_RELEASED:
					objInputEvent = (*env)->NewObject(
											env,
											clsKeyEvent,
											idKeyEvent,
											org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED,
											(jlong) event->time,
											event->mask,
											event->data->rawcode,
											event->data->keycode,
											org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
											jkey.location);

					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objInputEvent);
					(*env)->DeleteLocalRef(env, objInputEvent);
					break;

				case EVENT_KEY_TYPED:
					objInputEvent = (*env)->NewObject(
											env,
											clsKeyEvent,
											idKeyEvent,
											org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED,
											(jlong) event->time,
											event->mask,
											event->data->rawcode,
											org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED,
											event->data->keytxt,
											jkey.location);

					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objInputEvent);
					(*env)->DeleteLocalRef(env, objInputEvent);
					break;

				case EVENT_MOUSE_PRESSED:
					objInputEvent = (*env)->NewObject(
												env,
												clsMouseEvent,
												idMouseButtonEvent,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED,
												(jlong) event->time,
												event->mask,
												(jint) event->data->x,
												(jint) event->data->y,
												(jint) event->data->clicks,
												(jint) event->data->button);

					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objInputEvent);
					(*env)->DeleteLocalRef(env, objInputEvent);
					break;

				case EVENT_MOUSE_RELEASED:
					objInputEvent = (*env)->NewObject(
												env,
												clsMouseEvent,
												idMouseButtonEvent,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED,
												(jlong) event->time,
												event->mask,
												(jint) event->data->x,
												(jint) event->data->y,
												(jint) event->data->clicks,
												(jint) event->data->button);

					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objInputEvent);
					(*env)->DeleteLocalRef(env, objInputEvent);
					break;

				case EVENT_MOUSE_CLICKED:
					break;

				case EVENT_MOUSE_MOVED:
					break;

				case EVENT_MOUSE_DRAGGED:
					break;

				case EVENT_MOUSE_WHEEL:
					break;
			}

			(*env)->DeleteLocalRef(env, org_jnativehook_GlobalScreen);
		}
		else {
			// We cant do a whole lot of anything if we cant create JNI globals.
			// Any exceptions are thrown by CreateJNIGlobals().

			#ifdef DEBUG
			fprintf(stderr, "ThreadStartCallback(): CreateJNIGlobals() failed!\n");
			#endif

			//thread_ex.class = NATIVE_HOOK_EXCEPTION;
			//thread_ex.message = "Failed to create JNI global references";
		}
	}
}
