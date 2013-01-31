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

#include "NativeErrors.h"
#include "NativeGlobals.h"

// Global Ref to the JVM
JavaVM *jvm;
jint jni_version = JNI_VERSION_1_4;

// GlobalScreen object and dispatch id.
jmethodID idGetInstance, idDispatchNativeEvent, idStartNativeEventDispatcher, idStopNativeEventDispatcher;

// Java callback classes and constructor id's.
jclass clsGlobalScreen, clsNativeKeyEvent, clsNativeMouseEvent, clsNativeMouseWheelEvent;
jmethodID idNativeKeyEvent, idNativeMouseButtonEvent, idNativeMouseMotionEvent, idNativeMouseWheelEvent;


int CreateJNIGlobals() {
	int status = RETURN_FAILURE;

	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		// Class and getInstance method id for the GlobalScreen Object.
		jclass clsLocalGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
		if (clsLocalGlobalScreen != NULL) {
			clsGlobalScreen = (jclass) (*env)->NewGlobalRef(env, clsLocalGlobalScreen);

			// Get the method ID for GlobalScreen.getInstance()
			idGetInstance = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");
			#ifdef DEBUG
			if (idGetInstance == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.getInstance()!\n");
			}
			#endif

			// Get the method ID for GlobalScreen.dispatchEvent().
			idDispatchNativeEvent = (*env)->GetMethodID(env, clsGlobalScreen, "dispatchNativeEvent", "(Lorg/jnativehook/NativeInputEvent;)V");
			#ifdef DEBUG
			if (idDispatchNativeEvent == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.dispatchEvent()!\n");
			}
			#endif

			// Get the method ID for GlobalScreen.startEventDispatcher().
			idStartNativeEventDispatcher = (*env)->GetMethodID(env, clsGlobalScreen, "startNativeEventDispatcher", "()V");
			#ifdef DEBUG
			if (idStartNativeEventDispatcher == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.startEventDispatcher()!\n");
			}
			#endif

			// Get the method ID for GlobalScreen.stopEventDispatcher().
			idStopNativeEventDispatcher = (*env)->GetMethodID(env, clsGlobalScreen, "stopNativeEventDispatcher", "()V");
			#ifdef DEBUG
			if (idStopNativeEventDispatcher == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.stopEventDispatcher()!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the GlobalScreen class!\n");
		}
		#endif

		// Class and Constructor for the NativeKeyEvent Object.
		jclass clsLocalKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
		if (clsLocalKeyEvent != NULL) {
			clsNativeKeyEvent = (jclass) (*env)->NewGlobalRef(env, clsLocalKeyEvent);

			idNativeKeyEvent = (*env)->GetMethodID(env, clsNativeKeyEvent, "<init>", "(IJIIICI)V");
			#ifdef DEBUG
			if (idNativeKeyEvent == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeKeyEvent.NativeKeyEvent(int, long, int, int, int, int)!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeKeyEvent class!\n");
		}
		#endif


		// Class and Constructor for the NativeMouseEvent Object.
		jclass clsLocalMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
		if (clsLocalMouseEvent != NULL) {
			clsNativeMouseEvent = (jclass) (*env)->NewGlobalRef(env, clsLocalMouseEvent);

			idNativeMouseButtonEvent = (*env)->GetMethodID(env, clsNativeMouseEvent, "<init>", "(IJIIIII)V");
			#ifdef DEBUG
			if (idNativeMouseButtonEvent == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeMouseEvent.NativeMouseEvent(int, long, int, int, int, int)!\n");
			}
			#endif

			idNativeMouseMotionEvent = (*env)->GetMethodID(env, clsNativeMouseEvent, "<init>", "(IJIIII)V");
			#ifdef DEBUG
			if (idNativeMouseMotionEvent == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeMouseEvent.NativeMouseEvent(int, long, int, int, int)!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeMouseEvent class!\n");
		}
		#endif


		// Class and Constructor for the NativeMouseWheelEvent Object.
		jclass clsLocalMouseWheelEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseWheelEvent");
		if (clsLocalMouseWheelEvent != NULL) {
			clsNativeMouseWheelEvent = (jclass) (*env)->NewGlobalRef(env, clsLocalMouseWheelEvent);

			idNativeMouseWheelEvent = (*env)->GetMethodID(env, clsNativeMouseWheelEvent, "<init>", "(IJIIIIIII)V");
			#ifdef DEBUG
			if (idNativeMouseWheelEvent == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeMouseWheelEvent.NativeMouseWheelEvent(int, long, int, int, int, int, int, int)!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeMouseWheelEvent class!\n");
		}
		#endif

		// Check and make sure everything is correct.
		if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
			status = RETURN_SUCCESS;
		}
	}
	else {
		// We cant do a whole lot of anything if we cant attach to the current thread.
		#ifdef DEBUG
		fprintf(stderr, "CreateJNIGlobals(): GetEnv() failed!\n");
		#endif

		ThrowFatalError("Failed to aquire JNI interface pointer");
	}

	return status;
}

int DestroyJNIGlobals() {
	int status = RETURN_FAILURE;

	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		(*env)->DeleteGlobalRef(env, clsNativeKeyEvent);
		clsNativeKeyEvent = NULL;

		(*env)->DeleteGlobalRef(env, clsNativeMouseEvent);
		clsNativeMouseEvent = NULL;

		(*env)->DeleteGlobalRef(env, clsNativeMouseWheelEvent);
		clsNativeMouseWheelEvent = NULL;

		(*env)->DeleteGlobalRef(env, clsGlobalScreen);
		clsGlobalScreen = NULL;

		// Set all the global method ID's to null.
		idGetInstance = NULL;
		idDispatchNativeEvent = NULL;
		idStartNativeEventDispatcher = NULL;
		idStopNativeEventDispatcher = NULL;
		idNativeKeyEvent = NULL;
		idNativeMouseButtonEvent = NULL;
		idNativeMouseMotionEvent = NULL;
		idNativeMouseWheelEvent = NULL;

		status = RETURN_SUCCESS;
	}
	#ifdef DEBUG
	else {
		/* Leaving dangling global references will leak a small amount of memory
		 * but because this function is only called on JNI unload, that memory
		 * should be freed by the JVM after the unload compleats.
		 */
		fprintf(stderr, "DestroyJNIGlobals(): GetEnv() failed!\n");
	}
	#endif

	return status;
}
