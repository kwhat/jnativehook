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

#ifdef UNUSED
#elif defined(__GNUC__)
	#define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
	#define UNUSED(x) /*@unused@*/ x
#else
	#define UNUSED(x) x
#endif

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>
//#include <WinDef.h> //??? DWORD for the MSDLLSTRUCT

#ifdef DEBUG
	#include <stdio.h>
	#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <jni.h>

#include "include/org_jnativehook_GlobalScreen.h"
#include "include/JConvertToNative.h"
#include "WinKeyCodes.h"

//Instance Variables
HINSTANCE hInst = NULL;

//JVM globals.
JavaVM * jvm = NULL;




//Thread information so we can clean up.
HANDLE hookThreadHandle = NULL;
LPDWORD hookThreadId = NULL;

void jniFatalError(char * message) {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	#ifdef DEBUG
		printf("Native: Fatal Error - %s\n", message);
	#endif

	(*env)->FatalError(env, message);
	exit(1);
}

void throwException(char * classname, char * message) {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Locate our exception class
	jclass clsException = (*env)->FindClass(env, classname);

	if (clsException != NULL) {
		#ifdef DEBUG
			printf("Native: Exception - %s\n", message);
		#endif

		(*env)->ThrowNew(env, clsException, message);
	}
	else {
		//Unable to find exception class, Terminate with error.
		jniFatalError("Unable to locate exception class.");
	}
}




JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	long int wkb_rate;
	if (! SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &wkb_rate, 0) ) {
		#ifdef DEBUG
		printf("Native: SPI_GETKEYBOARDSPEED failure\n");
		#endif

		throwException("org/jnativehook/keyboard/NativeKeyException", "Could not determine the keyboard auto repeat rate.");
		return -1; //Naturally exit so JNI exception is thrown.
	}

	#ifdef DEBUG
		printf("Native: SPI_GETKEYBOARDSPEED successful (rate: %ldd)\n", wkb_rate);
	#endif
	return (jlong) wkb_rate;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	long int wkb_delay;
	if (! SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &wkb_delay, 0) ) {
		#ifdef DEBUG
			printf("Native: SPI_GETKEYBOARDDELAY failure\n");
		#endif

		throwException("org/jnativehook/keyboard/NativeKeyException", "Could not determine the keyboard auto repeat rate.");
		return -1; //Naturally exit so JNI exception is thrown.
	}

	#ifdef DEBUG
		printf("Native: SPI_GETKEYBOARDDELAY successful (delay: %ldd)\n", wkb_delay);
	#endif
	return (jlong) wkb_delay;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerSensitivity(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	long int wkb_sensitivity;
	if (! SystemParametersInfo(SPI_GETMOUSESPEED, 0, &wkb_sensitivity, 0) ) {
		#ifdef DEBUG
			printf("Native: SPI_GETMOUSESPEED failure\n");
		#endif

		throwException("org/jnativehook/keyboard/NativeMouseException", "Could not determine the mouse pointer sensitivity.");
		return -1; //Naturally exit so JNI exception is thrown.
	}

	#ifdef DEBUG
		printf("Native: SPI_GETMOUSESPEED successful (speed: %ldd)\n", wkb_sensitivity);
	#endif
	return (jlong) wkb_sensitivity;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getDoubleClickTime(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	long int wkb_time = (long int) GetDoubleClickTime();

	#ifdef DEBUG
		printf("Native: GetDoubleClickTime() successful (time: %ldd)\n", wkb_time);
	#endif
	return (jlong) wkb_time;
}


//This is where java attaches to the native machine.  Its kind of like the java + native constructor.
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * vm, void * UNUSED(reserved)) {
	//Grab the currently running virtual machine so we can attach to it in
	//functions that are not called from java. ( I.E. MsgLoop )
	jvm = vm;


	JNIEnv * env = 0;
	jint jni_ret = (*jvm)->GetEnv(jvm, (void **)(&env), JNI_VERSION_1_4);
	if (jni_ret == JNI_OK) {
		jni_ret = JNI_VERSION_1_4;
	}
	else {
		#ifdef DEBUG
			printf("Native: JNI_VERSION_1_4 unavailable for use. (default: %X)\n", (unsigned int) jni_ret);
		#endif
	}

	//FIXME Start the native thread.

	return jni_ret;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * UNUSED(vm), void * UNUSED(reserved)) {
	//FIXME Stop the native thread if its running.

	#ifdef DEBUG
		printf("Native: Thread terminated successful.\n");
	#endif
}

BOOL APIENTRY DllMain(HANDLE _hInst, DWORD reason, LPVOID UNUSED(reserved)) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			#ifdef DEBUG
				printf("Native: DllMain - DLL Process Attach.\n");
			#endif

			hInst = (HINSTANCE) _hInst; //GetModuleHandle(NULL)
		break;

		case DLL_PROCESS_DETACH:
			#ifdef DEBUG
				printf("Native: DllMain - DLL Process Detach.\n");
			#endif
		break;
	}

	return true;
}
