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

#include "jni_Errors.h"
#include "jni_Globals.h"

GlobalScreen *org_jnativehook_GlobalScreen = NULL;
NativeInputEvent *org_jnativehook_NativeInputEvent = NULL;
NativeKeyEvent *org_jnativehook_keyboard_NativeKeyEvent = NULL;
NativeMouseEvent *org_jnativehook_mouse_NativeMouseEvent = NULL;
NativeMouseWheelEvent *org_jnativehook_mouse_NativeMouseWheelEvent = NULL;

int jni_CreateGlobals(JNIEnv *env) {
	int status = JNI_ERR;

	// Allocate memory for the Java object structure representation.
	org_jnativehook_GlobalScreen = malloc(sizeof(GlobalScreen));
	org_jnativehook_NativeInputEvent = malloc(sizeof(NativeInputEvent));
	org_jnativehook_keyboard_NativeKeyEvent = malloc(sizeof(NativeKeyEvent));
	org_jnativehook_mouse_NativeMouseEvent = malloc(sizeof(NativeMouseEvent));
	org_jnativehook_mouse_NativeMouseWheelEvent = malloc(sizeof(NativeMouseWheelEvent));

	// Check to make sure memory was allocated properly.
	if (org_jnativehook_GlobalScreen != NULL
			&& org_jnativehook_NativeInputEvent != NULL
			&& org_jnativehook_keyboard_NativeKeyEvent != NULL
			&& org_jnativehook_mouse_NativeMouseEvent != NULL
			&& org_jnativehook_mouse_NativeMouseWheelEvent != NULL) {

		// Lookup a local reference for the GlobalScreen class.
		jclass GlobalScreen_class = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
		// TODO Check what happens when GlobalScreen_class == null, this check may not be required!
		if (GlobalScreen_class != NULL) {
			// Convert the local reference to a global reference.
			org_jnativehook_GlobalScreen->cls = (jclass) (*env)->NewGlobalRef(env, GlobalScreen_class);
			if (org_jnativehook_GlobalScreen->cls != NULL) {
				// Get the method ID for GlobalScreen.getInstance()
				org_jnativehook_GlobalScreen->getInstance = (*env)->GetStaticMethodID(
						env,
						org_jnativehook_GlobalScreen->cls,
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
						org_jnativehook_GlobalScreen->cls,
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
						org_jnativehook_GlobalScreen->cls,
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
						org_jnativehook_GlobalScreen->cls,
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


		// Class and Constructor for the NativeInputEvent Object.
		jclass NativeInputEvent_class = (*env)->FindClass(env, "org/jnativehook/NativeInputEvent");
		if (NativeInputEvent_class != NULL) {
			org_jnativehook_NativeInputEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeInputEvent_class);
			org_jnativehook_NativeInputEvent->init = (*env)->GetMethodID(env, org_jnativehook_keyboard_NativeKeyEvent->cls, "<init>", "(Lorg/jnativehook/GlobalScreen;IJI)V");

			if (org_jnativehook_NativeInputEvent->cls != NULL) {
				// Get the method ID for GlobalScreen.dispatchEvent().
				org_jnativehook_NativeInputEvent->getID = (*env)->GetMethodID(
						env,
						org_jnativehook_GlobalScreen->cls,
						"getID",
						"(I)V");

				if (org_jnativehook_NativeInputEvent->getID == NULL) {
					#ifdef DEBUG
					fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for GlobalScreen.dispatchEvent()!\n");
					#endif

					// FIXME Throw java.lang.NoSuchMethodError("getID(I)V")
				}
			}

			// FIXME better checking and exception throwing needs to be included.
			#ifdef DEBUG
			if (org_jnativehook_NativeInputEvent->cls == NULL || org_jnativehook_NativeInputEvent->init == NULL) {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID for NativeInputEvent.<init>(Lorg.jnativehook.GlobalScreen;IJI)V!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeKeyEvent class!\n");
		}
		#endif


		// Class and Constructor for the NativeKeyEvent Object.
		jclass NativeKeyEvent_class = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
		if (NativeKeyEvent_class != NULL) {
			org_jnativehook_keyboard_NativeKeyEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeKeyEvent_class);
			org_jnativehook_keyboard_NativeKeyEvent->init = (*env)->GetMethodID(env, org_jnativehook_keyboard_NativeKeyEvent->cls, "<init>", "(IJIIICI)V");
			org_jnativehook_keyboard_NativeKeyEvent->parent = org_jnativehook_NativeInputEvent;

			// FIXME better checking and exception throwing needs to be included.
			#ifdef DEBUG
			if (org_jnativehook_keyboard_NativeKeyEvent->cls == NULL || org_jnativehook_keyboard_NativeKeyEvent->init == NULL) {
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
			org_jnativehook_mouse_NativeMouseEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeMouseEvent_class);
			org_jnativehook_mouse_NativeMouseEvent->init = (*env)->GetMethodID(env, org_jnativehook_mouse_NativeMouseEvent->cls, "<init>", "(IJIIIII)V");
			org_jnativehook_mouse_NativeMouseEvent->parent = org_jnativehook_NativeInputEvent;

			// FIXME better checking and exception throwing needs to be included.
			#ifdef DEBUG
			if (org_jnativehook_mouse_NativeMouseEvent->cls || org_jnativehook_mouse_NativeMouseEvent->init == NULL) {
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
			org_jnativehook_mouse_NativeMouseWheelEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeMouseWheelEvent_class);
			org_jnativehook_mouse_NativeMouseWheelEvent->init = (*env)->GetMethodID(env, org_jnativehook_mouse_NativeMouseWheelEvent->cls, "<init>", "(IJIIIIIII)V");
			org_jnativehook_mouse_NativeMouseWheelEvent->parent = org_jnativehook_mouse_NativeMouseEvent;

			// FIXME better checking and exception throwing needs to be included.
			#ifdef DEBUG
			if (org_jnativehook_mouse_NativeMouseWheelEvent->cls == NULL || org_jnativehook_mouse_NativeMouseWheelEvent->init == NULL) {
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

int jni_DestroyGlobals(JNIEnv *env) {
	// Free any memory being used for Java object structures.
	if (org_jnativehook_GlobalScreen != NULL) {
		(*env)->DeleteGlobalRef(env, org_jnativehook_GlobalScreen->cls);
		free(org_jnativehook_GlobalScreen);
		org_jnativehook_GlobalScreen = NULL;
	}

	if (org_jnativehook_keyboard_NativeInputEvent != NULL) {
		(*env)->DeleteGlobalRef(env, org_jnativehook_keyboard_NativeInputEvent->cls);
		free(org_jnativehook_keyboard_NativeInputEvent);
		org_jnativehook_keyboard_NativeInputEvent = NULL;
	}

	if (org_jnativehook_keyboard_NativeKeyEvent != NULL) {
		(*env)->DeleteGlobalRef(env, org_jnativehook_keyboard_NativeKeyEvent->cls);
		free(org_jnativehook_keyboard_NativeKeyEvent);
		org_jnativehook_keyboard_NativeKeyEvent = NULL:
	}

	if (org_jnativehook_mouse_NativeMouseEvent != NULL) {
		(*env)->DeleteGlobalRef(env, org_jnativehook_mouse_NativeMouseEvent->cls);
		free(org_jnativehook_mouse_NativeMouseEvent);
		org_jnativehook_mouse_NativeMouseEvent = NULL;
	}

	if (org_jnativehook_mouse_NativeMouseWheelEvent != NULL) {
		(*env)->DeleteGlobalRef(env, org_jnativehook_mouse_NativeMouseWheelEvent->cls);
		free(org_jnativehook_mouse_NativeMouseWheelEvent);
		org_jnativehook_mouse_NativeMouseWheelEvent = NULL;
	}

	return JNI_OK;
}
