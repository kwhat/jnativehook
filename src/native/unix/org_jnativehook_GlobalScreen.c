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

//Global Variables
Display * disp_data;



JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * env, jobject UNUSED(obj)) {
	unsigned int xkb_interval;
	if (!XkbGetAutoRepeatRate(disp_data, XkbUseCoreKbd, NULL, &xkb_interval)) {
		#ifdef DEBUG
			printf("Native: XkbGetAutoRepeatRate failure!\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat rate.");
		return JNI_ERR; //Naturally exit so jni exception is thrown.
	}

	#ifdef DEBUG
		printf("Native: XkbGetAutoRepeatRate successful. (rate: %i)\n", xkb_interval);
	#endif
	return (jlong) xkb_interval;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * env, jobject UNUSED(obj)) {
	unsigned int xkb_timeout;
	if (!XkbGetAutoRepeatRate(disp_data, XkbUseCoreKbd, &xkb_timeout, NULL)) {
		#ifdef DEBUG
			printf("Native: XkbGetAutoRepeatRate failure!\n");
		#endif

		ThrowException(env, NATIVE_KEY_EXCEPTION, "Could not determine the keyboard auto repeat delay.");
		return JNI_ERR; //Naturally exit so jni exception is thrown.
	}

	#ifdef DEBUG
		printf("Native: XkbGetAutoRepeatRate successful (delay: %i)\n", xkb_timeout);
	#endif
	return (jlong) xkb_timeout;
}


JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getPointerSensitivity(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	return (jlong) JNI_ERR;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getDoubleClickTime(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	char * xkb_time = XGetDefault(disp_data, "*", "multiClickTime");

	#ifdef DEBUG
		printf("Native: GetDoubleClickTime() successful (time: %ldd)\n", wkb_time);
	#endif
	return (jlong) xkb_time;
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

	//Tell X Threads are OK
	//TODO We need to check and see if this is even needed.
	XInitThreads();

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
