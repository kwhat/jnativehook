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
jmethodID idDispatchNativeEvent, idStartNativeEventDispatcher, idStopNativeEventDispatcher;

// Java callback classes and constructor id's.
jclass clsNativeKeyEvent, clsNativeMouseEvent, clsNativeMouseWheelEvent;
jmethodID idNativeKeyEvent, idNativeMouseButtonEvent, idNativeMouseMotionEvent, idNativeMouseWheelEvent;


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
					idDispatchNativeEvent = (*env)->GetMethodID(env, clsGlobalScreen, "dispatchNativeEvent", "(Lorg/jnativehook/NativeInputEvent;)V");
					if (idDispatchNativeEvent == NULL) {
						#ifdef DEBUG
						fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.dispatchNativeEvent()!\n");
						#endif

						ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.GlobalScreen#dispatchNativeEvent()");
					}
					
					// Get the method ID for GlobalScreen.startEventDispatcher().
					idStartNativeEventDispatcher = (*env)->GetMethodID(env, clsGlobalScreen, "startNativeEventDispatcher", "()V");
					if (idStartNativeEventDispatcher == NULL) {
						#ifdef DEBUG
						fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.startNativeEventDispatcher()!\n");
						#endif

						ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.GlobalScreen#startNativeEventDispatcher()");
					}
					
					// Get the method ID for GlobalScreen.stopEventDispatcher().
					idStopNativeEventDispatcher = (*env)->GetMethodID(env, clsGlobalScreen, "stopNativeEventDispatcher", "()V");
					if (idStopNativeEventDispatcher == NULL) {
						#ifdef DEBUG
						fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.stopNativeEventDispatcher()!\n");
						#endif

						ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.GlobalScreen#startNativeEventDispatcher()");
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
			clsNativeKeyEvent = (*env)->NewGlobalRef(env, clsLocalKeyEvent);
			if (clsNativeKeyEvent != NULL) {
				idNativeKeyEvent = (*env)->GetMethodID(env, clsNativeKeyEvent, "<init>", "(IJIIICI)V");
				if (idNativeKeyEvent == NULL) {
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
			clsNativeMouseEvent = (*env)->NewGlobalRef(env, clsLocalMouseEvent);
			if (clsNativeMouseEvent != NULL) {
				idNativeMouseButtonEvent = (*env)->GetMethodID(env, clsNativeMouseEvent, "<init>", "(IJIIIII)V");
				if (idNativeMouseButtonEvent == NULL) {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeMouseEvent.NativeMouseEvent(int, long, int, int, int, int)!\n");
					#endif

					ThrowException(INTERNAL_ERROR, "Failed to acquire the method ID for org.jnativehook.mouse.NativeMouseEvent#NativeMouseEvent(int, long, int, int, int, int)");
				}

				idNativeMouseMotionEvent = (*env)->GetMethodID(env, clsNativeMouseEvent, "<init>", "(IJIIII)V");
				if (idNativeMouseMotionEvent == NULL) {
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
			clsNativeMouseWheelEvent = (*env)->NewGlobalRef(env, clsLocalMouseWheelEvent);
			if (clsNativeMouseWheelEvent != NULL) {
				idNativeMouseWheelEvent = (*env)->GetMethodID(env, clsNativeMouseWheelEvent, "<init>", "(IJIIIIIII)V");
				if (idNativeMouseWheelEvent == NULL) {
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
		(*env)->DeleteGlobalRef(env, clsNativeKeyEvent);
		clsNativeKeyEvent = NULL;

		(*env)->DeleteGlobalRef(env, clsNativeMouseEvent);
		clsNativeMouseEvent = NULL;

		(*env)->DeleteGlobalRef(env, clsNativeMouseWheelEvent);
		clsNativeMouseWheelEvent = NULL;

		(*env)->DeleteGlobalRef(env, objGlobalScreen);
		objGlobalScreen = NULL;

		// Set all the global method ID's to null.
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
		 * but because there is nothing that can be done about it at this point
		 * an exception will not be thrown.
		 */
		fprintf(stderr, "DestroyJNIGlobals(): GetEnv() failed!\n");
	}
	#endif

	return status;
}
