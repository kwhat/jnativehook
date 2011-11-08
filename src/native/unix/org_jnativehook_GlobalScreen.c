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

#ifdef XKB
#include <X11/XKBlib.h>
#endif

#ifdef XF86MISC
#include <X11/extensions/xf86misc.h>
#include <X11/extensions/xf86mscstr.h>
#endif

#ifdef XT
#include <X11/Intrinsic.h>
#endif

#include "JNativeHook.h"
#include "NativeThread.h"
#include "org_jnativehook_GlobalScreen.h"

static Display * disp;

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * env, jobject UNUSED(obj)) {
	bool successful = false;
	long value = JNI_ERR;
	unsigned int kb_delay = 0, kb_rate = 0;

	#ifdef XKB
	//Attempt to acquire the keyboard auto repeat rate using the XKB extension if available.
	if (!successful) {
		successful = XkbGetAutoRepeatRate(disp, XkbUseCoreKbd, &kb_delay, &kb_rate);
	}
	#endif

	#ifdef XF86MISC
	//Fallback to the XF86 Misc extension if available and other efforts failed.
	if (!successful) {
		XF86MiscKbdSettings kb_info;
		successful = (bool) XF86MiscGetKbdSettings(disp, &kb_info);
		if (successful) {
			kb_delay = (unsigned int) kbdinfo.delay;
			kb_rate = (unsigned int) kbdinfo.rate;
		}
	}
	#endif

	if (successful) {
		value = (long) kb_rate;

		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(): successful. (rate: %li)\n", value);
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(): failure!\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat rate.");
	}

	(void) kb_delay;
	return (jlong) value;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * env, jobject UNUSED(obj)) {
	bool successful = false;
	long value = JNI_ERR;
	unsigned int kb_delay = 0, kb_rate = 0;

	#ifdef XKB
	if (!successful) {
		successful = XkbGetAutoRepeatRate(disp, XkbUseCoreKbd, &kb_delay, &kb_rate);
	}
	#endif

	#ifdef XF86MISC
	if (!successful) {
		XF86MiscKbdSettings kb_info;
		Status ret = XF86MiscGetKbdSettings(disp, &kb_info);
		successful = (bool) ret->status;
		if (successful) {
			kb_delay = (unsigned int) kbdinfo.delay;
			kb_rate = (unsigned int) kbdinfo.rate;
		}
	}
	#endif

	if (successful) {
		value = (long) kb_delay;

		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(): successful. (delay: %li)\n", value);
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(): failure!\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat delay.");
	}

	(void) kb_rate;
	return (jlong) value;
}

// 0-Threshold X, 1-Threshold Y and 2-Speed
JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationMultiplier(JNIEnv * env, jobject UNUSED(obj)) {
	int accel_numerator, accel_denominator, threshold;
	XGetPointerControl(disp, &accel_numerator, &accel_denominator, &threshold);
	if (accel_denominator < 0) {
		ThrowException(env, NATIVE_MOUSE_EXCEPTION, "Could not determine the pointer acceleration multiplier.");
		return JNI_ERR; //Naturally exit so jni exception is thrown.
	}
	(void) accel_numerator;
	(void) threshold;

	return (jlong) accel_denominator;
}

// 0-Threshold X, 1-Threshold Y and 2-Speed
JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationThreshold(JNIEnv * env, jobject UNUSED(obj)) {
	int accel_numerator, accel_denominator, threshold;
	XGetPointerControl(disp, &accel_numerator, &accel_denominator, &threshold);
	if (threshold < 0) {
		ThrowException(env, NATIVE_MOUSE_EXCEPTION, "Could not determine the pointer acceleration threshold.");
		return JNI_ERR; //Naturally exit so jni exception is thrown.
	}
	(void) accel_numerator;
	(void) accel_denominator;

	return (jlong) threshold;
}


JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerSensitivity(JNIEnv * env, jobject UNUSED(obj)) {
	int accel_numerator, accel_denominator, threshold;
	XGetPointerControl(disp, &accel_numerator, &accel_denominator, &threshold);
	if (accel_numerator < 0) {
		ThrowException(env, NATIVE_MOUSE_EXCEPTION, "Could not determine the pointer sensitivity.");
		return JNI_ERR; //Naturally exit so jni exception is thrown.
	}
	(void) accel_denominator;
	(void) threshold;

	return (jlong) accel_numerator;
}


JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getDoubleClickTime(JNIEnv * env, jobject UNUSED(obj)) {
	int xm_clicktime;
	bool successful = false;

	//Try and acquire the multi-click time from the user defined Xdefaults
	char * xprop = XGetDefault(disp, "*", "multiClickTime");
	if (xprop != NULL && sscanf(xprop, "%i", &xm_clicktime) != EOF) {
		successful = true;
	}

	#ifdef XT
	//Fall back to the X Toolkit extension if available and other efforts failed.
	if (!successful) {
		xm_clicktime = XtGetMultiClickTime(disp);
	}
	#endif

	if (successful) {
		#ifdef DEBUG
		fprintf(stdout, "Java_org_jnativehook_GlobalScreen_getDoubleClickTime(): successful. (time: %i)\n", xm_clicktime);
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "Java_org_jnativehook_GlobalScreen_getDoubleClickTime(): failure!\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the mouse double click time.");
		xm_clicktime = JNI_ERR; //Naturally exit so jni exception is thrown.
	}

	return (jlong) xm_clicktime;
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

		Bool isAutoRepeat = false;
		#ifdef XKB
		//enable detectable autorepeat.
		XkbSetDetectableAutoRepeat(disp, True, &isAutoRepeat);
		#else
		XAutoRepeatOn(disp);

		XKeyboardState kb_state;
		XGetKeyboardControl(disp, &kb_state);

		isAutoRepeat = (kb_state.global_auto_repeat == AutoRepeatModeOn);
		#endif

		if (isAutoRepeat) {
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

	//Detach the current thread to the JVM.
	if ((*jvm)->DetachCurrentThread(jvm) == JNI_OK) {
		#ifdef DEBUG
		fprintf(stdout, "JNI_OnUnload(): DetachCurrentThread(jvm, (void **)(&env), NULL) successful.\n");
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "JNI_OnUnload(): DetachCurrentThread(jvm, (void **)(&env), NULL) failed!\n");
		#endif
	}

	#ifdef DEBUG
	fprintf(stdout, "JNI Unloaded.\n");
	#endif
}
