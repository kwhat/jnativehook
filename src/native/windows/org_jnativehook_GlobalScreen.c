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

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>

#include "JNativeHook.h"
#include "NativeThread.h"
#include "org_jnativehook_GlobalScreen.h"

//Global Variables
HINSTANCE hInst = NULL;

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * env, jobject UNUSED(obj)) {
	long int wkb_rate;
	if (! SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &wkb_rate, 0) ) {
		#ifdef DEBUG
			fprintf(stdout, "SPI_GETKEYBOARDSPEED failure\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat rate.");
		return JNI_ERR; //Naturally exit so JNI exception is thrown.
	}

	#ifdef DEBUG
		fprintf(stdout, "SPI_GETKEYBOARDSPEED successful (rate: %li)\n", wkb_rate);
	#endif
	return (jlong) wkb_rate;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * env, jobject UNUSED(obj)) {
	long int wkb_delay;
	if (! SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &wkb_delay, 0) ) {
		#ifdef DEBUG
			fprintf(stdout, "SPI_GETKEYBOARDDELAY failure\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat rate.");
		return JNI_ERR; //Naturally exit so JNI exception is thrown.
	}

	#ifdef DEBUG
		fprintf(stdout, "SPI_GETKEYBOARDDELAY successful (delay: %li)\n", wkb_delay);
	#endif
	return (jlong) wkb_delay;
}

// 0-Threshold X, 1-Threshold Y and 2-Speed
JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationMultiplier(JNIEnv * env, jobject UNUSED(obj)) {
	int wkb_mouse[3];
	if (! SystemParametersInfo(SPI_GETMOUSE, 0, &wkb_mouse, 0) ) {
		#ifdef DEBUG
			fprintf(stdout, "SPI_GETMOUSE failure\n");
		#endif

		ThrowException(env, NATIVE_MOUSE_EXCEPTION, "Could not determine the mouse acceleration multiplier.");
		return JNI_ERR; //Naturally exit so JNI exception is thrown.
	}

	#ifdef DEBUG
		fprintf(stdout, "SPI_GETMOUSE successful (%i, %i, %i)\n", wkb_mouse[0], wkb_mouse[1], wkb_mouse[2]);
	#endif

	return (jlong) wkb_mouse[2];
}

// 0-Threshold X, 1-Threshold Y and 2-Speed
JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerAccelerationThreshold(JNIEnv * env, jobject UNUSED(obj)) {
	int wkb_mouse[3];
	if (! SystemParametersInfo(SPI_GETMOUSE, 0, &wkb_mouse, 0) ) {
		#ifdef DEBUG
			fprintf(stdout, "SPI_GETMOUSE failure\n");
		#endif

		ThrowException(env, NATIVE_MOUSE_EXCEPTION, "Could not determine the mouse acceleration threshold.");
		return JNI_ERR; //Naturally exit so JNI exception is thrown.
	}

	#ifdef DEBUG
		fprintf(stdout, "SPI_GETMOUSE successful (%i, %i, %i)\n", wkb_mouse[0], wkb_mouse[1], wkb_mouse[2]);
	#endif

	//Average the x and y thresholds.
	return (jlong) (wkb_mouse[0] + wkb_mouse[1]) / 2;
}



JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerSensitivity(JNIEnv * env, jobject UNUSED(obj)) {
	long int wkb_sensitivity;
	if (! SystemParametersInfo(SPI_GETMOUSESPEED, 0, &wkb_sensitivity, 0) ) {
		#ifdef DEBUG
			fprintf(stdout, "SPI_GETMOUSESPEED failure\n");
		#endif

		ThrowException(env, NATIVE_MOUSE_EXCEPTION, "Could not determine the mouse pointer sensitivity.");
		return JNI_ERR; //Naturally exit so JNI exception is thrown.
	}

	#ifdef DEBUG
		fprintf(stdout, "SPI_GETMOUSESPEED successful (speed: %li)\n", wkb_sensitivity);
	#endif
	return (jlong) wkb_sensitivity;
}


JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getDoubleClickTime(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	long int wkb_time = (long int) GetDoubleClickTime();

	#ifdef DEBUG
		fprintf(stdout, "GetDoubleClickTime() successful (time: %li)\n", wkb_time);
	#endif
	return (jlong) wkb_time;
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

	//FIXME use better var names, should not reutrn jni_version
	jint jni_version = JNI_VERSION_1_4;
	jint jni_ret = (*jvm)->GetEnv(jvm, (void **)(&env), jni_version);
	switch (jni_ret) {
		case JNI_EVERSION:
			#ifdef DEBUG
				fprintf(stderr, "JNI_VERSION_1_4 unavailable for use.\n");
			#endif

			jni_version = JNI_ERR;
		break;

		case JNI_EDETACHED:
			//TODO Eval if this is needed.
			//Not attached to the current thread.
			if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) != JNI_OK) {
				#ifdef DEBUG
					fprintf(stderr, "Could not attach the current thread to the Java virtual machine.\n");
				#endif

				jni_version = JNI_ERR;
			}
			else {
				#ifdef DEBUG
					fprintf(stdout, "Successfully attached the current thread to the Java virtual machine.\n");
				#endif
			}
		case JNI_OK:
			#ifdef DEBUG
				fprintf(stdout, "Successfully acquired a Java environment reference.\n");
			#endif
		break;

		default:
			#ifdef DEBUG
				fprintf(stderr, "An unknown error occurred while acquiring a Java environment reference.\n");
			#endif

			jni_version = JNI_ERR;
		break;
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


BOOL APIENTRY DllMain(HANDLE _hInst, DWORD reason, LPVOID UNUSED(reserved)) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			hInst = (HINSTANCE) _hInst;
			//hInst = GetModuleHandle(NULL);
		break;

		case DLL_PROCESS_DETACH:
			//TODO Make sure the native thread has stopped.
		break;
	}

	return TRUE;
}
