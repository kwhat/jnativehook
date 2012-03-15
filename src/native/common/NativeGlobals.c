/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "NativeGlobals.h"

/* Global Ref to the JVM */
JavaVM * jvm;

/* GlobalScreen object and dispatch id. */
jobject objGlobalScreen;
jmethodID idDispatchEvent;

/* Java callback classes and constructor id's */
jclass clsKeyEvent, clsMouseEvent, clsMouseWheelEvent;
jmethodID idKeyEvent, idMouseButtonEvent, idMouseMotionEvent, idMouseWheelEvent;

int CreateJNIGlobals() {
	int status = RETURN_FAILURE;

	JNIEnv * env = NULL;
	/* TODO Should swtich to GetEnv */
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {

		/* Class and getInstance method id for the GlobalScreen Object */
		jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
		if (clsGlobalScreen != NULL) {
			/* Get the method ID for GlobalScreen.getInstance() */
			jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");
			if (getInstance_ID != NULL) {
				/* Create a global reference for the GlobalScreen Object */
				jobject objScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);
				objGlobalScreen = (*env)->NewGlobalRef(env, objScreen);

				#ifdef DEBUG
				if (objGlobalScreen == NULL) {
					fprintf(stderr, "CreateJNIGlobals(): Failed to create global reference to GlobalScreen!\n");
				}
				#endif

				/* Get the method ID for GlobalScreen.dispatchEvent(). */
				idDispatchEvent = (*env)->GetMethodID(env, clsGlobalScreen, "dispatchEvent", "(Lorg/jnativehook/NativeInputEvent;)V");

				#ifdef DEBUG
				if (idDispatchEvent == NULL) {
					fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID GlobalScreen.dispatchEvent()!\n");
				}
				#endif
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "CreateJNIGlobals(): Failed to acquire the method ID GlobalScreen.getInstance()!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the GlobalScreen class!\n");
		}
		#endif


		/* Class and Constructor for the NativeKeyEvent Object. */
		jclass clsLocalKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
		if (clsLocalKeyEvent != NULL) {
			clsKeyEvent = (*env)->NewGlobalRef(env, clsLocalKeyEvent);
			if (clsKeyEvent != NULL) {
				idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "CreateJNIGlobals(): Failed to create global reference to NativeKeyEvent!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeKeyEvent class!\n");
		}
		#endif

		/* Class and Constructor for the NativeMouseEvent Object. */
		jclass clsLocalMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
		if (clsLocalMouseEvent != NULL) {
			clsMouseEvent = (*env)->NewGlobalRef(env, clsLocalMouseEvent);
			if (clsMouseEvent != NULL) {
				idMouseButtonEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");
				idMouseMotionEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIII)V");
			}
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeMouseEvent class!\n");
		}
		#endif

		
		/* Class and Constructor for the NativeMouseWheelEvent Object. */
		jclass clsLocalMouseWheelEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseWheelEvent");
		if (clsLocalMouseWheelEvent != NULL) {
			clsMouseWheelEvent = (*env)->NewGlobalRef(env, clsLocalMouseWheelEvent);
			idMouseWheelEvent = (*env)->GetMethodID(env, clsMouseWheelEvent, "<init>", "(IJIIIIII)V");
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "CreateJNIGlobals(): Failed to locate the NativeMouseWheelEvent class!\n");
		}
		#endif

		//FIXME Check and make sure everything is correct.
		status = RETURN_SUCCESS;
	}
	#ifdef DEBUG
	else {
		/* We cant do a whole lot of anything if we cant attach to the current thread. */
		/* FIXME An exception should be raised. */
		fprintf(stderr, "StartNativeThread(): AttachCurrentThread() failed!\n");
	}
	#endif

	return status;
}

int DestroyJNIGlobals() {
	int status = RETURN_FAILURE;
	JNIEnv * env = NULL;

	/* TODO Should swtich to GetEnv */
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		(*env)->DeleteGlobalRef(env, clsKeyEvent);
		clsKeyEvent = NULL;

		(*env)->DeleteGlobalRef(env, clsMouseEvent);
		clsMouseEvent = NULL;

		(*env)->DeleteGlobalRef(env, clsMouseWheelEvent);
		clsMouseWheelEvent = NULL;

		(*env)->DeleteGlobalRef(env, objGlobalScreen);
		objGlobalScreen = NULL;

		//Set all the global method ID's to null.
		idDispatchEvent = NULL;
		idKeyEvent = NULL;
		idMouseButtonEvent = NULL;
		idMouseMotionEvent = NULL;
		idMouseWheelEvent = NULL;

		status = RETURN_SUCCESS;
	}
	#ifdef DEBUG
	else {
		/* We cant do a whole lot of anything if we cant attach to the current thread. */
		/* FIXME An exception should be raised. */
		fprintf(stderr, "StartNativeThread(): AttachCurrentThread() failed!\n");

	}
	#endif

	return status;
}
