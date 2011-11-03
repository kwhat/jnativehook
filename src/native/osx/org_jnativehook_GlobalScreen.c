/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
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

#include <ApplicationServices/ApplicationServices.h>

#include "JNativeHook.h"
#include "NativeThread.h"
#include "org_jnativehook_GlobalScreen.h"

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * env, jobject UNUSED(obj)) {
	SInt32 interval = -1;

	CFTypeRef pref_val = CFPreferencesCopyAppValue(CFSTR("KeyRepeat"), kCFPreferencesCurrentApplication);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		CFNumberGetValue((CFNumberRef)pref_val, kCFNumberSInt32Type, &interval);
		#ifdef DEBUG
			printf("Native: CFPreferencesCopyAppValue successful (rate: %i)\n", (int) interval);
		#endif
	}
	else {
		#ifdef DEBUG
			printf("Native: CFPreferencesCopyAppValue failure\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat rate.");
	}

	return (jlong) interval;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * env, jobject UNUSED(obj)) {
	SInt32 interval = -1;

	CFTypeRef pref_val = CFPreferencesCopyAppValue(CFSTR("InitialKeyRepeat"), kCFPreferencesCurrentApplication);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		CFNumberGetValue((CFNumberRef)pref_val, kCFNumberSInt32Type, &interval);
		#ifdef DEBUG
			printf("Native: CFPreferencesCopyAppValue successful (delay: %i)\n", (int) interval);
		#endif
	}
	else {
		#ifdef DEBUG
			printf("Native: CFPreferencesCopyAppValue failure\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat delay.");
	}

	return (jlong) interval;
}


JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationMultiplier(JNIEnv * env, jobject UNUSED(obj)) {
	ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the pointer acceleration multiplier.");

	return (jlong) JNI_ERR;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationThreshold(JNIEnv * env, jobject UNUSED(obj)) {
	ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the pointer acceleration threshold.");

	//Average the x and y thresholds.
	return (jlong) JNI_ERR;
}


JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerSensitivity(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	return (jlong) JNI_ERR;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getDoubleClickTime(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	#ifdef DEBUG
		printf("Native: GetDoubleClickTime() successful (time:)\n");
	#endif
	return (jlong) JNI_ERR;
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
			fprintf(stdout, "Successfully attached the current thread to the Java virtual machine.\n");
		#endif
	}

    return jni_version;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * UNUSED(vm), void * UNUSED(reserved)) {
	//Stop the native thread if its running.
	if (IsNativeThreadRunning()) {
		StopNativeThread();
	}

	if ((*jvm)->DetachCurrentThread(jvm) != JNI_OK) {
		#ifdef DEBUG
			fprintf(stderr, "Could not dettach the current thread from the Java virtual machine!\n");
		#endif
	}

	#ifdef DEBUG
		fprintf(stdout, "JNI Unloaded.\n");
	#endif
}
