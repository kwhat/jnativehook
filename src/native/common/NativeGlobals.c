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
jobject objGlobalScreen;
jmethodID idDispatchEvent, idStartEventDispatcher, idStopEventDispatcher;

// Java callback classes and constructor id's.
jclass clsKeyEvent, clsMouseEvent, clsMouseWheelEvent;
jmethodID idKeyEvent, idMouseButtonEvent, idMouseMotionEvent, idMouseWheelEvent;


int CreateJNIGlobals() {
	int status = RETURN_FAILURE;

	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		// Class and getInstance method id for the GlobalScreen Object.
		jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
		if (clsGlobalScreen != NULL) {
			// Get the method ID for GlobalScreen.getInstance()
			jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");
			if (getInstance_ID != NULL) {
				// Create a global reference for the GlobalScreen Object.
				jobject objScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);
				objGlobalScreen = (*env)->NewGlobalRef(env, objScreen);

				if (objGlobalScreen != NULL) {
					// Get the method ID for GlobalScreen.dispatchEvent().
					idDispatchEvent = (*env)->GetMethodID(env, clsGlobalScreen, "dispatchEvent", "(Lorg/jnativehook/NativeInputEvent;)V");
					if (idDispatchEvent == NULL) {
						#ifdef DEBUG
						fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.dispatchEvent()!\n");
						#endif

						ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.GlobalScreen#dispatchEvent()");
					}
					
					// Get the method ID for GlobalScreen.startEventDispatcher().
					idStartEventDispatcher = (*env)->GetMethodID(env, clsGlobalScreen, "startEventDispatcher", "()V");
					if (idStartEventDispatcher == NULL) {
						#ifdef DEBUG
						fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.startEventDispatcher()!\n");
						#endif

						ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.GlobalScreen#startEventDispatcher()");
					}
					
					// Get the method ID for GlobalScreen.stopEventDispatcher().
					idStopEventDispatcher = (*env)->GetMethodID(env, clsGlobalScreen, "stopEventDispatcher", "()V");
					if (idStopEventDispatcher == NULL) {
						#ifdef DEBUG
						fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.stopEventDispatcher()!\n");
						#endif

						ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.GlobalScreen#startEventDispatcher()");
					}
				}
				else {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals(): Failed to create global reference for GlobalScreen!\n");
					#endif

					ThrowException(OUT_OF_MEMORY_ERROR, "Failed to create JNI global reference for org.jnativehook.GlobalScreen");
				}
			}
			else {
				#ifdef DEBUG
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID GlobalScreen.getInstance()!\n");
				#endif

				ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.GlobalScreen#getInstance()");
			}
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the GlobalScreen class!\n");
			#endif

			ThrowException(NO_CLASS_DEF_FOUND_ERROR, "Failed to locate the org.jnativehook.GlobalScreen class");
		}


		// Class and Constructor for the NativeKeyEvent Object.
		jclass clsLocalKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
		if (clsLocalKeyEvent != NULL) {
			clsKeyEvent = (*env)->NewGlobalRef(env, clsLocalKeyEvent);
			if (clsKeyEvent != NULL) {
				idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIICI)V");
				if (idKeyEvent == NULL) {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeKeyEvent.NativeKeyEvent(int, long, int, int, int, int)!\n");
					#endif

					ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.keyboard.NativeKeyEvent#NativeKeyEvent(int, long, int, int, int, int)");
				}
			}
			else {
				#ifdef DEBUG
				fprintf(stderr, "CreateJNIGlobals(): Failed to create global reference to NativeKeyEvent!\n");
				#endif

				ThrowException(OUT_OF_MEMORY_ERROR, "Failed to create JNI global reference for org.jnativehook.keyboard.NativeKeyEvent");
			}

		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeKeyEvent class!\n");
			#endif

			ThrowException(NO_CLASS_DEF_FOUND_ERROR, "Failed to locate the org.jnativehook.keyboard.NativeKeyEvent class");
		}


		// Class and Constructor for the NativeMouseEvent Object.
		jclass clsLocalMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
		if (clsLocalMouseEvent != NULL) {
			clsMouseEvent = (*env)->NewGlobalRef(env, clsLocalMouseEvent);
			if (clsMouseEvent != NULL) {
				idMouseButtonEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIIII)V");
				if (idMouseButtonEvent == NULL) {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeMouseEvent.NativeMouseEvent(int, long, int, int, int, int)!\n");
					#endif

					ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.mouse.NativeMouseEvent#NativeMouseEvent(int, long, int, int, int, int)");
				}

				idMouseMotionEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");
				if (idMouseMotionEvent == NULL) {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeMouseEvent.NativeMouseEvent(int, long, int, int, int)!\n");
					#endif

					ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.mouse.NativeMouseEvent#NativeMouseEvent(int, long, int, int, int)");
				}
			}
			else {
				#ifdef DEBUG
				fprintf(stderr, "CreateJNIGlobals(): Failed to create global reference for NativeMouseEvent!\n");
				#endif

				ThrowException(OUT_OF_MEMORY_ERROR, "Failed to create JNI global reference for org.jnativehook.mouse.NativeMouseEvent");
			}
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeMouseEvent class!\n");
			#endif

			ThrowException(NO_CLASS_DEF_FOUND_ERROR, "Failed to locate the org.jnativehook.mouse.NativeMouseEvent class");
		}


		// Class and Constructor for the NativeMouseWheelEvent Object.
		jclass clsLocalMouseWheelEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseWheelEvent");
		if (clsLocalMouseWheelEvent != NULL) {
			clsMouseWheelEvent = (*env)->NewGlobalRef(env, clsLocalMouseWheelEvent);
			if (clsMouseWheelEvent != NULL) {
				idMouseWheelEvent = (*env)->GetMethodID(env, clsMouseWheelEvent, "<init>", "(IJIIIIIII)V");
				if (idMouseWheelEvent == NULL) {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeMouseWheelEvent.NativeMouseWheelEvent(int, long, int, int, int, int, int, int)!\n");
					#endif

					ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.mouse.NativeMouseWheelEvent#NativeMouseWheelEvent(int, long, int, int, int, int, int, int)");
				}
			}
			else {
				#ifdef DEBUG
				fprintf(stderr, "CreateJNIGlobals(): Failed to create global reference for NativeMouseWheelEvent!\n");
				#endif

				ThrowException(OUT_OF_MEMORY_ERROR, "Failed to create JNI global reference for org.jnativehook.mouse.NativeMouseWheelEvent");
			}
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeMouseWheelEvent class!\n");
			#endif

			ThrowException(NO_CLASS_DEF_FOUND_ERROR, "Failed to locate the org.jnativehook.mouse.NativeMouseWheelEvent class");
		}


		// Check and make sure everything is correct.
		if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
			status = RETURN_SUCCESS;
		}
	}
	else {
		// We cant do a whole lot of anything if we cant attach to the current thread.
		#ifdef DEBUG
		fprintf(stderr, "StartNativeThread(): GetEnv() failed!\n");
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

		(*env)->DeleteGlobalRef(env, objGlobalScreen);
		objGlobalScreen = NULL;

		// Set all the global method ID's to null.
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
		 * but because there is nothing that can be done about it at this point
		 * an exception will not be thrown.
		 */
		fprintf(stderr, "DestroyJNIGlobals(): GetEnv() failed!\n");
	}
	#endif

	return status;
}
