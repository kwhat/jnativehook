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
#include "NativeHelpers.h"
#include "NativeThread.h"
#include "org_jnativehook_GlobalScreen.h"

static void SetNativeProperties(JNIEnv *env) {
	jclass clsSystem = (*env)->FindClass(env, "java/lang/System");
	jmethodID setProperty_ID = (*env)->GetStaticMethodID(env, clsSystem, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

	if (clsSystem != NULL && setProperty_ID != NULL) {
		// Create a buffer for converting numbers to strings.
		char buffer[16];

		// Set the native keyboard auto repeat rate.
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


		// 0-Threshold X, 1-Threshold Y and 2-Speed.
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


		// 0-Threshold X, 1-Threshold Y and 2-Speed.
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


static void ClearNativeProperties(JNIEnv *env) {
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


JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_registerNativeHook(JNIEnv *UNUSED(env), jclass UNUSED(cls)) {
	StartNativeThread();
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_unregisterNativeHook(JNIEnv *UNUSED(env), jclass UNUSED(cls)) {
	StopNativeThread();
}

JNIEXPORT jboolean JNICALL Java_org_jnativehook_GlobalScreen_isNativeHookRegistered(JNIEnv *UNUSED(env), jclass UNUSED(cls)) {
	return (jboolean) IsNativeThreadRunning();
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

		// Run platform specific load items.
		OnLibraryLoad();

		// Set java properties from native sources.
		SetNativeProperties(env);
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
	if (IsNativeThreadRunning()) {
		StopNativeThread();
	}

	// Run platform specific unload items.
	OnLibraryUnload();

	// Grab the currently JNI interface pointer so we can cleanup the
	// system properties set on load.
	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		// Clear java properties from native sources.
		ClearNativeProperties(env);
	}
	#ifdef DEBUG
	else {
		// It is not critical that these values are cleared so no exception
		// will be thrown.
		fprintf(stderr, "JNI_OnUnload(): GetEnv() failed!\n");
	}
	#endif

	// Destroy all created globals.
	#ifdef DEBUG
	if (DestroyJNIGlobals() == RETURN_FAILURE) {
		fprintf(stderr, "ThreadProc(): DestroyJNIGlobals() failed!\n");
	}
	#else
	DestroyJNIGlobals();
	#endif

	#ifdef DEBUG
	fprintf(stdout, "JNI_OnUnload(): JNI Unloaded.\n");
	#endif
}
