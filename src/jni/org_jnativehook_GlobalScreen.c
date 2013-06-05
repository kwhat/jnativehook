/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

#include "nativehook.h"
#include "org_jnativehook_GlobalScreen.h"

static void JNIEventDispatcher(VirtualEvent *const event) {
	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		// Create the global screen references up front to save time in the callback.
		jobject org_jnativehook_GlobalScreen = (*env)->CallStaticObjectMethod(
				env,
				clsGlobalScreen,
				idGetInstance);

		if (org_jnativehook_GlobalScreen != NULL) {
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

static void JNISetProperties(JNIEnv *env) {
	jclass clsSystem = (*env)->FindClass(env, "java/lang/System");
	jmethodID setProperty_ID = (*env)->GetStaticMethodID(env, clsSystem, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

	if (clsSystem != NULL && setProperty_ID != NULL) {
		// Create a buffer for converting numbers to strings.
		char buffer[16];

		// Set the native keyboard auto repeat rate.
		long rate = hook_get_auto_repeat_rate();
		if (rate >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetAutoRepeatRate(): successful. (rate: %li)\n", rate);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", rate) >= 0) {
				(*env)->CallStaticObjectMethod(env, clsSystem, setProperty_ID, (*env)->NewStringUTF(env, "jnativehook.autoRepeatRate"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetAutoRepeatRate(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "GetAutoRepeatRate(): failure!\n");
		}
		#endif


		long delay = hook_get_auto_repeat_delay();
		if (delay >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetAutoRepeatDelay(): successful. (delay: %li)\n", delay);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", delay) >= 0) {
				(*env)->CallStaticObjectMethod(env, clsSystem, setProperty_ID, (*env)->NewStringUTF(env, "jnativehook.autoRepeatDelay"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetAutoRepeatDelay(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "GetAutoRepeatDelay(): failure!\n");
		}
		#endif


		// 0-Threshold X, 1-Threshold Y and 2-Speed.
		long multiplier = hook_get_pointer_acceleration_multiplier();
		if (multiplier >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetPointerAccelerationMultiplier(): successful. (multiplier: %li)\n", multiplier);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", multiplier) >= 0) {
				(*env)->CallStaticObjectMethod(env, clsSystem, setProperty_ID, (*env)->NewStringUTF(env, "jnativehook.pointerAccelerationMultiplier"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetPointerAccelerationMultiplier(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stdout, "GetPointerAccelerationMultiplier(): failure!\n");
		}
		#endif


		// 0-Threshold X, 1-Threshold Y and 2-Speed.
		long threshold = hook_get_pointer_acceleration_threshold();
		if (threshold >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetPointerAccelerationThreshold(): successful. (threshold: %li)\n", threshold);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", threshold) >= 0) {
				(*env)->CallStaticObjectMethod(env, clsSystem, setProperty_ID, (*env)->NewStringUTF(env, "jnativehook.pointerAccelerationThreshold"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetPointerAccelerationThreshold(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stdout, "GetPointerAccelerationThreshold(): failure!\n");
		}
		#endif


		long sensitivity = hook_get_pointer_sensitivity();
		if (sensitivity >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetPointerSensitivity(): successful. (sensitivity: %li)\n", sensitivity);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", sensitivity) >= 0) {
				(*env)->CallStaticObjectMethod(env, clsSystem, setProperty_ID, (*env)->NewStringUTF(env, "jnativehook.pointerSensitivity"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetPointerSensitivity(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stdout, "GetPointerSensitivity(): failure!\n");
		}
		#endif


		long clicktime = hook_get_multi_click_time();
		if (clicktime >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetMultiClickTime(): successful. (time: %li)\n", clicktime);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", clicktime) >= 0) {
				(*env)->CallStaticObjectMethod(env, clsSystem, setProperty_ID, (*env)->NewStringUTF(env, "jnativehook.multiClickInterval"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetMultiClickTime(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "GetMultiClickTime(): failure!\n");
		}
		#endif
	}
}


static void JNIClearProperties(JNIEnv *env) {
	jclass clsSystem = (*env)->FindClass(env, "java/lang/System");
	jmethodID clearProperty_ID = (*env)->GetStaticMethodID(env, clsSystem, "clearProperty", "(Ljava/lang/String;)Ljava/lang/String;");

	if (clsSystem != NULL && clearProperty_ID != NULL) {
		(*env)->CallStaticObjectMethod(env, clsSystem, clearProperty_ID, (*env)->NewStringUTF(env, "jnativehook.autoRepeatRate"));
		(*env)->CallStaticObjectMethod(env, clsSystem, clearProperty_ID, (*env)->NewStringUTF(env, "jnativehook.autoRepeatDelay"));
		(*env)->CallStaticObjectMethod(env, clsSystem, clearProperty_ID, (*env)->NewStringUTF(env, "jnativehook.pointerAccelerationMultiplier"));
		(*env)->CallStaticObjectMethod(env, clsSystem, clearProperty_ID, (*env)->NewStringUTF(env, "jnativehook.pointerAccelerationThreshold"));
		(*env)->CallStaticObjectMethod(env, clsSystem, clearProperty_ID, (*env)->NewStringUTF(env, "jnativehook.pointerSensitivity"));
		(*env)->CallStaticObjectMethod(env, clsSystem, clearProperty_ID, (*env)->NewStringUTF(env, "jnativehook.multiClickInterval"));
	}
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_postNativeEvent(JNIEnv *UNUSED(env), jclass UNUSED(cls), jobject UNUSED(event)) {
	//FIXME Use the jni globals!

	//jclass clsNativeKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
	//jclass clsNativeMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
	//jclass clsNativeMouseWheelEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseWheelEvent");

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
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_registerNativeHook(JNIEnv *UNUSED(env), jclass UNUSED(cls)) {
	// Start the java event dispatch thread.
	(*env)->CallVoidMethod(env, org_jnativehook_GlobalScreen, idStartEventDispatcher);

	hook_enable();
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_unregisterNativeHook(JNIEnv *UNUSED(env), jclass UNUSED(cls)) {
	hook_disable();

	// Stop the java event dispatch thread.
	(*env)->CallVoidMethod(env, org_jnativehook_GlobalScreen, idStopEventDispatcher);
}

JNIEXPORT jboolean JNICALL Java_org_jnativehook_GlobalScreen_isNativeHookRegistered(JNIEnv *UNUSED(env), jclass UNUSED(cls)) {
	// Simple wrapper to return the hook status.
	return (jboolean) hook_is_enable();
}


// JNI entry point, This is executed when the Java virtual machine attaches to the native library.
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *UNUSED(reserved)) {
	/* Grab the currently running virtual machine so we can attach to it in
	 * functions that are not called from java. ( I.E. ThreadProc )
	 */
	jvm = vm;
	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		#ifdef DEBUG
		fprintf(stdout, "JNI_OnLoad(): GetEnv() successful.\n");
		#endif

		// Create all the global class references onload to prevent class loader
		// issues with JNLP and some IDE's.
		if (CreateJNIGlobals() == RETURN_FAILURE) {
			#ifdef DEBUG
			fprintf(stderr, "JNI_OnLoad(): CreateJNIGlobals() failed!\n");
			#endif

			ThrowFatalError("Failed to locate one or more required classes.");
		}

		// Set java properties from native sources.
		JNISetProperties(env);

		// Set the hook callback function to dispatch events.
		hook_set_dispatch_proc(&JNIEventDispatcher);
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "JNI_OnLoad(): GetEnv() failed!\n");
		#endif

		ThrowFatalError("Failed to aquire JNI interface pointer");
	}

	#ifdef DEBUG
	fprintf(stdout, "JNI_Load(): JNI Loaded.\n");
	#endif

    return jni_version;
}

// JNI exit point, This is executed when the Java virtual machine detaches from the native library.
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *UNUSED(vm), void *UNUSED(reserved)) {
	// Stop the native thread if its running.
	if (hook_is_enable()) {
		hook_disable();
	}

	// Grab the currently JNI interface pointer so we can cleanup the
	// system properties set on load.
	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		// Clear java properties from native sources.
		JNIClearProperties(env);
	}
	#ifdef DEBUG
	else {
		// It is not critical that these values are cleared so no exception
		// will be thrown.
		fprintf(stderr, "JNI_OnUnload(): GetEnv() failed!\n");
	}
	#endif

	JNIDestroyGlobals();

	#ifdef DEBUG
	fprintf(stdout, "JNI_OnUnload(): JNI Unloaded.\n");
	#endif
}
