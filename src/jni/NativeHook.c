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

void KeyPressedCallback() {
	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		// Fire key pressed event.
		objKeyEvent = (*env)->NewObject(
								env,
								clsKeyEvent,
								idKeyEvent,
								org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED,
								event_time,
								jmodifiers,
								event_code,
								jkey.keycode,
								org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
								jkey.location);
		(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		(*env)->DeleteLocalRef(env, objKeyEvent);
	}
}

void KeyTypedCallback() {
	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		// Fire key typed event.
		objKeyEvent = (*env)->NewObject(
								env,
								clsKeyEvent,
								idKeyEvent,
								org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED,
								event_time,
								jmodifiers,
								event_code,
								org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED,
								(jchar) keytxt,
								jkey.location);
		(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		(*env)->DeleteLocalRef(env, objKeyEvent);
	}
}

void KeyReleasedCallback() {
	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		// Fire key released event.
		objKeyEvent = (*env)->NewObject(
								env,
								clsKeyEvent,
								idKeyEvent,
								org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED,
								event_time,
								jmodifiers,
								event_code,
								jkey.keycode,
								org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
								jkey.location);
		(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		(*env)->DeleteLocalRef(env, objKeyEvent);
	}
}





// This method will be externalized with 1.2
bool ThreadStartCallback() {
	bool status = false;

	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		#ifdef DEBUG
		fprintf(stdout, "ThreadStartCallback(): Attached to JVM successful.\n");
		#endif

		// Create the global screen references up front to save time in the callback.
		jobject tmpScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, idGetInstance);
		if (tmpScreen != NULL) {
			org_jnativehook_GlobalScreen = (*env)->NewGlobalRef(env, tmpScreen);

			// Callback and start native event dispatch thread
			(*env)->CallVoidMethod(env, org_jnativehook_GlobalScreen, idStartEventDispatcher);

			// Call Thread.currentThread().setName("JNativeHook Native Hook");
			jobject objCurrentThread = (*env)->CallStaticObjectMethod(env, clsThread, idCurrentThread);
			(*env)->CallVoidMethod(env, objCurrentThread, idSetName, (*env)->NewStringUTF(env, "JNativeHook Native Hook"));
			(*env)->DeleteLocalRef(env, objCurrentThread);

			status = true;
		}
		else {
			// We cant do a whole lot of anything if we cant create JNI globals.
			// Any exceptions are thrown by CreateJNIGlobals().

			#ifdef DEBUG
			fprintf(stderr, "ThreadStartCallback(): CreateJNIGlobals() failed!\n");
			#endif

			thread_ex.class = NATIVE_HOOK_EXCEPTION;
			thread_ex.message = "Failed to create JNI global references";
		}



		#ifdef DEBUG
		fprintf(stdout, "ThreadStartCallback(): Detach from JVM successful.\n");
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "ThreadStartCallback(): AttachCurrentThread() failed!\n");
		#endif

		thread_ex.class = NATIVE_HOOK_EXCEPTION;
		thread_ex.message = "Failed to attach the native thread to the virtual machine";
	}

	return status;
}

// This method will be externalized with 1.2
bool ThreadStopCallback() {
	bool status = false;

	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		// Calling AttachCurrentThread() should result in a no-op.

		// Callback and stop native event dispatch thread.
		(*env)->CallVoidMethod(env, org_jnativehook_GlobalScreen, idStopEventDispatcher);

		// Remove the global reference to the GlobalScren object.
		(*env)->DeleteGlobalRef(env, org_jnativehook_GlobalScreen);

		// Detach this thread from the JVM.
		if ((*jvm)->DetachCurrentThread(jvm) == JNI_OK) {
			status = true;
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "ThreadStopCallback(): DetachCurrentThread() failed!\n");
		}
		#endif
	}
	#ifdef DEBUG
	else {
		fprintf(stderr, "ThreadStartCallback(): AttachCurrentThread() failed!\n");
	}
	#endif

	return status;
}
