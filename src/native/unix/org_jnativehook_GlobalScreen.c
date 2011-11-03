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

#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#include "JNativeHook.h"
#include "NativeThread.h"
#include "org_jnativehook_GlobalScreen.h"

static Display * disp;

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * env, jobject UNUSED(obj)) {
	unsigned int xkb_timeout, xkb_interval;

	if (XkbGetAutoRepeatRate(disp, XkbUseCoreKbd, &xkb_timeout, &xkb_interval) != true) {
		#ifdef DEBUG
			printf("Native: XkbGetAutoRepeatRate failure!\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat rate.");
		return JNI_ERR; //Naturally exit so jni exception is thrown.
	}
	(void) xkb_timeout;

	#ifdef DEBUG
		printf("Native: XkbGetAutoRepeatRate successful. (rate: %i)\n", xkb_interval);
	#endif
	return (jlong) xkb_interval;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * env, jobject UNUSED(obj)) {
	unsigned int xkb_timeout, xkb_interval;

	if (XkbGetAutoRepeatRate(disp, XkbUseCoreKbd, &xkb_timeout, &xkb_interval) != true) {
		#ifdef DEBUG
			printf("Native: XkbGetAutoRepeatRate failure!\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat delay.");
		return JNI_ERR; //Naturally exit so jni exception is thrown.
	}
	(void) xkb_interval;

	#ifdef DEBUG
		printf("Native: XkbGetAutoRepeatRate successful (delay: %i)\n", xkb_timeout);
	#endif
	return (jlong) xkb_timeout;
}

// 0-Threshold X, 1-Threshold Y and 2-Speed
JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationMultiplier(JNIEnv * env, jobject UNUSED(obj)) {
	ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the pointer acceleration multiplier.");

	return (jlong) JNI_ERR;
}

// 0-Threshold X, 1-Threshold Y and 2-Speed
JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationThreshold(JNIEnv * env, jobject UNUSED(obj)) {
	ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the pointer acceleration threshold.");

	//Average the x and y thresholds.
	return (jlong) JNI_ERR;
}


JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerSensitivity(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	return (jlong) JNI_ERR;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getDoubleClickTime(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	char * xkb_time = XGetDefault(disp, "*", "multiClickTime");

	#ifdef DEBUG
		printf("Native: GetDoubleClickTime() successful (time: %s)\n", xkb_time);
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

		//Tell X Threads are OK
		XInitThreads();

		//Open local display.
		disp = XOpenDisplay(XDisplayName(NULL));
		if (disp != NULL) {
			#ifdef DEBUG
				fprintf(stdout, "XOpenDisplay successful.\n");
			#endif
		}
		else {
			#ifdef DEBUG
				fprintf(stderr, "XOpenDisplay failure!\n");
			#endif

			ThrowFatalError(env, "Could not attach to the default X11 display");
		}

		//enable detectable autorepeat.
		Bool isAutoRepeat;
		XkbSetDetectableAutoRepeat(disp, True, &isAutoRepeat);
		if (isAutoRepeat == True) {
			#ifdef DEBUG
				fprintf(stdout, "Successfully enabled detectable autorepeat.\n");
			#endif
		}
		else {
			#ifdef DEBUG
				fprintf(stderr, "Could not enable detectable auto-repeat!\n");
			#endif

			ThrowException(env, NATIVE_HOOK_EXCEPTION, "Could not enable detectable auto-repeat");
		}
	}

    return jni_version;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * UNUSED(vm), void * UNUSED(reserved)) {
	//Stop the native thread if its running.
	if (IsNativeThreadRunning()) {
		StopNativeThread();
	}

	//Destroy the native displays.
	if (disp != NULL) {
		XCloseDisplay(disp);
		disp = NULL;
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
