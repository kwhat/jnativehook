/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
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

#include "NativeErrors.h"
#include "NativeGlobals.h"

// Global Ref to the JVM
JavaVM *jvm;
jint jni_version = JNI_VERSION_1_4;

// GlobalScreen object and dispatch id.
jmethodID idGetInstance, idDispatchEvent, idStartEventDispatcher, idStopEventDispatcher;

// Java callback classes and constructor id's.
jclass clsGlobalScreen, clsKeyEvent, clsMouseEvent, clsMouseWheelEvent;
jmethodID idKeyEvent, idMouseButtonEvent, idMouseMotionEvent, idMouseWheelEvent;

// Thread class and methods to set the name.
jclass clsThread;
jmethodID idCurrentThread, idSetName;

int CreateJNIGlobals() {
	int status = RETURN_FAILURE;

	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		// Class and getInstance method id for the GlobalScreen Object.
		jclass clsLocalThread = (*env)->FindClass(env, "java/lang/Thread");
		if (clsLocalThread != NULL) {
			clsThread = (jclass) (*env)->NewGlobalRef(env, clsLocalThread);

			// Get the method ID for GlobalScreen.getInstance()
			idCurrentThread = (*env)->GetStaticMethodID(env, clsThread, "currentThread", "()Ljava/lang/Thread;");
			#ifdef DEBUG
			if (idCurrentThread == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for Thread.currentThread()!\n");
			}
			#endif

			// Get the method ID for GlobalScreen.dispatchEvent().
			idSetName = (*env)->GetMethodID(env, clsThread, "setName", "(Ljava/lang/String;)V");
			#ifdef DEBUG
			if (idSetName == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for Thread.setName(String)!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the Thread class!\n");
		}
		#endif
		
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
			idDispatchEvent = (*env)->GetMethodID(env, clsGlobalScreen, "dispatchEvent", "(Lorg/jnativehook/NativeInputEvent;)V");
			#ifdef DEBUG
			if (idDispatchEvent == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.dispatchEvent()!\n");
			}
			#endif

			// Get the method ID for GlobalScreen.startEventDispatcher().
			idStartEventDispatcher = (*env)->GetMethodID(env, clsGlobalScreen, "startEventDispatcher", "()V");
			#ifdef DEBUG
			if (idStartEventDispatcher == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.startEventDispatcher()!\n");
			}
			#endif

			// Get the method ID for GlobalScreen.stopEventDispatcher().
			idStopEventDispatcher = (*env)->GetMethodID(env, clsGlobalScreen, "stopEventDispatcher", "()V");
			#ifdef DEBUG
			if (idStopEventDispatcher == NULL) {
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
			clsKeyEvent = (jclass) (*env)->NewGlobalRef(env, clsLocalKeyEvent);

			idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIICI)V");
			#ifdef DEBUG
			if (idKeyEvent == NULL) {
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
			clsMouseEvent = (jclass) (*env)->NewGlobalRef(env, clsLocalMouseEvent);

			idMouseButtonEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIIII)V");
			#ifdef DEBUG
			if (idMouseButtonEvent == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeMouseEvent.NativeMouseEvent(int, long, int, int, int, int)!\n");
			}
			#endif

			idMouseMotionEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");
			#ifdef DEBUG
			if (idMouseMotionEvent == NULL) {
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
			clsMouseWheelEvent = (jclass) (*env)->NewGlobalRef(env, clsLocalMouseWheelEvent);

			idMouseWheelEvent = (*env)->GetMethodID(env, clsMouseWheelEvent, "<init>", "(IJIIIIIII)V");
			#ifdef DEBUG
			if (idMouseWheelEvent == NULL) {
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
		(*env)->DeleteGlobalRef(env, clsKeyEvent);
		clsKeyEvent = NULL;

		(*env)->DeleteGlobalRef(env, clsMouseEvent);
		clsMouseEvent = NULL;

		(*env)->DeleteGlobalRef(env, clsMouseWheelEvent);
		clsMouseWheelEvent = NULL;

		(*env)->DeleteGlobalRef(env, clsGlobalScreen);
		clsGlobalScreen = NULL;
		
		(*env)->DeleteGlobalRef(env, clsThread);
		clsThread = NULL;

		// Set all the global method ID's to null.
		idCurrentThread = NULL;
		idSetName = NULL;
		idGetInstance = NULL;
		idDispatchEvent = NULL;
		idStartEventDispatcher = NULL;
		idStopEventDispatcher = NULL;
		idKeyEvent = NULL;
		idMouseButtonEvent = NULL;
		idMouseMotionEvent = NULL;
		idMouseWheelEvent = NULL;

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
