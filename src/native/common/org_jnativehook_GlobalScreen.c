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

#include "NativeErrors.h"
#include "NativeGlobals.h"
#include "NativeHelpers.h"
#include "NativeThread.h"
#include "org_jnativehook_GlobalScreen.h"

static void SetNativeProperties(JNIEnv * env) {
	jclass clsSystem = (*env)->FindClass(env, "java/lang/System");
	jmethodID setProperty_ID = (*env)->GetStaticMethodID(env, clsSystem, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

	if (clsSystem != NULL && setProperty_ID != NULL) {
		//Create a buffer for converting numbers to strings.
		char buffer[16];

		//Set the native keyboard auto repeat rate.
		long rate = GetAutoRepeatRate();
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


		long delay = GetAutoRepeatDelay();
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

		
		// 0-Threshold X, 1-Threshold Y and 2-Speed
		long multiplier = GetPointerAccelerationMultiplier();
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


		// 0-Threshold X, 1-Threshold Y and 2-Speed
		long threshold = GetPointerAccelerationThreshold();
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


		long sensitivity = GetPointerSensitivity();
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


		long clicktime = GetMultiClickTime();
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


JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_registerNativeHook(JNIEnv * env, jobject UNUSED(obj)) {
	if (StartNativeThread() != EXIT_SUCCESS) {
		ThrowException(env, NATIVE_HOOK_EXCEPTION, "Could not register the native hook.");
	}
}


JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_unregisterNativeHook(JNIEnv * env, jobject UNUSED(obj)) {
	if (StopNativeThread() != EXIT_SUCCESS) {
		ThrowException(env, NATIVE_HOOK_EXCEPTION, "Could not unregister the native hook.");
	}
}

JNIEXPORT jboolean JNICALL Java_org_jnativehook_GlobalScreen_isNativeHookRegistered(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	return (jboolean) IsNativeThreadRunning();
}


//This is where java attaches to the native machine.  Its kind of like the java + native constructor.
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * vm, void * UNUSED(reserved)) {
	//Grab the currently running virtual machine so we can attach to it in
	//functions that are not called from java. ( I.E. ThreadLoop )
	jvm = vm;
	JNIEnv * env = 0;

	jint jni_version = JNI_VERSION_1_4;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		#ifdef DEBUG
		fprintf(stdout, "JNI_OnLoad(): AttachCurrentThread() successful.\n");
		#endif

		//Run platform specific load items.
		OnLibraryLoad();

		//Set java properties from native sources.
		SetNativeProperties(env);
	}
	#ifdef DEBUG
	else {
		fprintf(stderr, "JNI_OnLoad(): AttachCurrentThread() failed!\n");
		//TODO Throw a runtime exception
	}
	#endif

    return jni_version;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * UNUSED(vm), void * UNUSED(reserved)) {
	//Stop the native thread if its running.
	if (IsNativeThreadRunning()) {
		StopNativeThread();
	}

	//Run platform specific unload items.
	OnLibraryUnload();

	//Detach the current thread to the JVM.
	#ifdef DEBUG
	if ((*jvm)->DetachCurrentThread(jvm) == JNI_OK) {
		fprintf(stdout, "JNI_OnUnload(): DetachCurrentThread() successful.\n");
	}
	else {
		fprintf(stderr, "JNI_OnUnload(): DetachCurrentThread() failed!\n");
	}
	#else
	(*jvm)->DetachCurrentThread(jvm);
	#endif

	#ifdef DEBUG
	fprintf(stdout, "JNI Unloaded.\n");
	#endif
}
