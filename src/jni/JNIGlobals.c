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
#include "JNIGlobals.h"

// Global Ref to the JVM
JavaVM *jvm;
jint jni_version = JNI_VERSION_1_4;

GlobalScreen *org_jnativehook_GlobalScreen = null;
NativeKeyEvent *org_jnativehook_keyboard_NativeKeyEvent = null;
NativeMouseEvent *org_jnativehook_mouse_NativeMouseEvent = null;
NativeMouseWheelEvent *org_jnativehook_mouse_NativeMouseWheelEvent = null;

int CreateJNIGlobals(JNIEnv *env) {
	int status = JNI_ERROR;

	// Allocate memory for the Java object structure representation.
	org_jnativehook_GlobalScreen = malloc(sizeof(GlobalScreen));
	org_jnativehook_keyboard_NativeKeyEvent = malloc(sizeof(NativeKeyEvent));
	org_jnativehook_mouse_NativeMouseEvent = malloc(sizeof(NativeMouseEvent));
	org_jnativehook_mouse_NativeMouseWheelEvent = malloc(sizeof(NativeMouseWheelEvent));

	// Check to make sure memory was allocated properly.
	if (org_jnativehook_GlobalScreen != null
			&& org_jnativehook_keyboard_NativeKeyEvent != null
			&& org_jnativehook_mouse_NativeMouseEvent != null
			&& org_jnativehook_mouse_NativeMouseWheelEvent != null) {

		// Lookup a local reference for the GlobalScreen class.
		jclass GlobalScreen_class = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
		// TODO Check what happens when GlobalScreen_class == null, this check may not be required!
		if (GlobalScreen_class != NULL) {
			// Convert the local reference to a global reference.
			org_jnativehook_GlobalScreen->class = (jclass) (*env)->NewGlobalRef(env, GlobalScreen_class);
			if (org_jnativehook_GlobalScreen->class != null) {
				// Get the method ID for GlobalScreen.getInstance()
				org_jnativehook_GlobalScreen->getInstance = (*env)->GetStaticMethodID(
						env,
						org_jnativehook_GlobalScreen->class,
						"getInstance",
						"()Lorg/jnativehook/GlobalScreen;");

				if (org_jnativehook_GlobalScreen->getInstance == NULL) {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals()Lorg/jnativehook/GlobalScreen: Failed to acquire the method ID for GlobalScreen.getInstance()!\n");
					#endif

					// FIXME Throw java.lang.NoSuchMethodError("getInstance()Lorg/jnativehook/GlobalScreen;")
				}


				// Get the method ID for GlobalScreen.dispatchEvent().
				org_jnativehook_GlobalScreen->dispatchEvent = (*env)->GetMethodID(
						env,
						org_jnativehook_GlobalScreen->class,
						"dispatchEvent",
						"(Lorg/jnativehook/NativeInputEvent;)V");

				if (org_jnativehook_GlobalScreen->dispatchEvent == NULL) {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.dispatchEvent()!\n");
					#endif

					// FIXME Throw java.lang.NoSuchMethodError("dispatchEvent(Lorg/jnativehook/NativeInputEvent;)V")
				}

				// Get the method ID for GlobalScreen.startEventDispatcher().
				org_jnativehook_GlobalScreen->startEventDispatcher = (*env)->GetMethodID(
						env,
						org_jnativehook_GlobalScreen->class,
						"startEventDispatcher",
						"()V");

				if (org_jnativehook_GlobalScreen->startEventDispatcher == NULL) {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.startEventDispatcher()!\n");
					#endif

					// FIXME Throw java.lang.NoSuchMethodError("startEventDispatcher()V")
				}


				// Get the method ID for GlobalScreen.stopEventDispatcher().
				org_jnativehook_GlobalScreen->stopEventDispatcher = (*env)->GetMethodID(
						env,
						org_jnativehook_GlobalScreen->class,
						"stopEventDispatcher",
						"()V");

				if (org_jnativehook_GlobalScreen->stopEventDispatcher == NULL) {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.stopEventDispatcher()!\n");
					#endif

					// FIXME Throw java.lang.NoSuchMethodError("stopEventDispatcher()V")
				}
			}
			else {
				status = JNI_ENOMEM;

				#ifdef DEBUG
				fprintf(stderr, "CreateJNIGlobals(): Failed to allocate memory for JNI structures!\n");
				#endif

				// FIXME Throw java.lang.OutOfMemoryError
			}
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the GlobalScreen class!\n");
		}
		#endif

		// Class and Constructor for the NativeKeyEvent Object.
		jclass NativeKeyEvent_class = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
		if (NativeKeyEvent_class != NULL) {
			NativeKeyEvent->class = (jclass) (*env)->NewGlobalRef(env, NativeKeyEvent_class);
			NativeKeyEvent->init = (*env)->GetMethodID(env, NativeKeyEvent->class, "<init>", "(IJIIICI)V");

			// FIXME better checking and exception throwing needs to be included.
			#ifdef DEBUG
			if (idNativeKeyEvent == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeKeyEvent.<init>(IJIIICI)V!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeKeyEvent class!\n");
		}
		#endif


		// Class and Constructor for the NativeMouseEvent Object.
		jclass NativeMouseEvent_class = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
		if (NativeMouseEvent_class != NULL) {
			NativeMouseEvent->class = (jclass) (*env)->NewGlobalRef(env, NativeMouseEvent_class);
			NativeMouseEvent->init = (*env)->GetMethodID(env, NativeMouseEvent->class, "<init>", "(IJIIIII)V");

			// FIXME better checking and exception throwing needs to be included.
			#ifdef DEBUG
			if (idNativeMouseButtonEvent == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeMouseEvent.NativeMouseEvent(int, long, int, int, int, int)!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeMouseEvent class!\n");
		}
		#endif


		// Class and Constructor for the NativeMouseWheelEvent Object.
		jclass NativeMouseWheelEvent_class = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseWheelEvent");
		if (NativeMouseWheelEvent_class != NULL) {
			NativeMouseWheelEvent->class = (jclass) (*env)->NewGlobalRef(env, NativeMouseWheelEvent_class);
			NativeMouseWheelEvent->init = (*env)->GetMethodID(env, clsNativeMouseWheelEvent, "<init>", "(IJIIIIIII)V");

			// FIXME better checking and exception throwing needs to be included.
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
	}
	else {
		status = JNI_ENOMEM;

		#ifdef DEBUG
		fprintf(stderr, "CreateJNIGlobals(): Failed to allocate memory for JNI structures!\n");
		#endif

		// FIXME Throw java.lang.OutOfMemoryError
	}

	// Check and make sure everything is correct.
	if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
		status = JNI_OK;
	}

	return status;
}

int DestroyJNIGlobals(JNIEnv *env) {
	(*env)->DeleteGlobalRef(env, org_jnativehook_keyboard_NativeKeyEvent->class);
	org_jnativehook_keyboard_NativeKeyEvent->class = NULL;

	(*env)->DeleteGlobalRef(env, org_jnativehook_keyboard_NativeMouseEvent->class);
	clsNativeMouseEvent = NULL;

	(*env)->DeleteGlobalRef(env, org_jnativehook_keyboard_NativeMouseWheelEvent->class);
	clsNativeMouseWheelEvent = NULL;

	(*env)->DeleteGlobalRef(env, org_jnativehook_GlobalScreen->class);
	org_jnativehook_GlobalScreen->class = NULL;

	// Set all the global method ID's to null.
	org_jnativehook_GlobalScreen->getInstance = NULL;
	org_jnativehook_GlobalScreen->dispatchEvent = NULL;
	org_jnativehook_GlobalScreen->startEventDispatcher = NULL;
	org_jnativehook_GlobalScreen->stopEventDispatcher = NULL;

		idNativeKeyEvent = NULL;
		idNativeMouseButtonEvent = NULL;
		idNativeMouseMotionEvent = NULL;
		idNativeMouseWheelEvent = NULL;

	// Free any memory being used for Java object structures.
	if (org_jnativehook_mouse_NativeMouseWheelEvent != null) {
		free(org_jnativehook_GlobalScreen);
	}

	if (org_jnativehook_mouse_NativeMouseEvent != null) {
		free(org_jnativehook_GlobalScreen);
	}

	if (org_jnativehook_keyboard_NativeKeyEvent != null) {
		free(org_jnativehook_keyboard_NativeKeyEvent);
	}

	if (org_jnativehook_GlobalScreen != null) {
		free(org_jnativehook_GlobalScreen);
	}

	return JNI_OK;
}
