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

#include <pthread.h>
#include <signal.h>

#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/extensions/record.h>


#include "org_jnativehook_GlobalScreen.h"
#include "JConvertToNative.h"
#include "xEventModifers.h"

//Instance Variables
bool isRunning = true;
unsigned int xkb_timeout;
unsigned int xkb_interval;

//For this struct, refer to libxnee
typedef union {
	unsigned char		type;
	xEvent				event;
	xResourceReq		req;
	xGenericReply		reply;
	xError				error;
	xConnSetupPrefix	setup;
} XRecordDatum;



//Display * disp_hook;
//Display * disp_data;


//Convert the XEvent modifier mask to a Java modifier mask.
jint doModifierConvert(int event_mask) {
	jint modifiers = 0;

	if (event_mask & KeyButMaskShift)		modifiers |= NativeToJModifier(KeyButMaskShift);
	if (event_mask & KeyButMaskControl)		modifiers |= NativeToJModifier(KeyButMaskControl);
	if (event_mask & KeyButMaskMod4)		modifiers |= NativeToJModifier(KeyButMaskMod4);
	if (event_mask & KeyButMaskMod1)		modifiers |= NativeToJModifier(KeyButMaskMod1);

	if (event_mask & KeyButMaskButton1)		modifiers |= NativeToJModifier(KeyButMaskButton1);
	if (event_mask & KeyButMaskButton2)		modifiers |= NativeToJModifier(KeyButMaskButton2);
	if (event_mask & KeyButMaskButton3)		modifiers |= NativeToJModifier(KeyButMaskButton3);
	if (event_mask & KeyButMaskButton4)		modifiers |= NativeToJModifier(KeyButMaskButton4);
	if (event_mask & KeyButMaskButton5)		modifiers |= NativeToJModifier(KeyButMaskButton5);

	return modifiers;
}



JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	if (!XkbGetAutoRepeatRate(disp_data, XkbUseCoreKbd, &xkb_timeout, &xkb_interval) ) {
		#ifdef DEBUG
			printf("Native: XkbGetAutoRepeatRate failure\n");
		#endif

		throwException("org/jnativehook/keyboard/NativeKeyException", "Could not determine the keyboard auto repeat rate.");
		return -1; //Naturally exit so jni exception is thrown.
	}

	#ifdef DEBUG
		printf("Native: XkbGetAutoRepeatRate successful (rate: %i) (delay: %i)\n", xkb_interval, xkb_timeout);
	#endif
	return (jlong) xkb_interval;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	if (!XkbGetAutoRepeatRate(disp_data, XkbUseCoreKbd, &xkb_timeout, &xkb_interval) ) {
		#ifdef DEBUG
			printf("Native: XkbGetAutoRepeatRate failure\n");
		#endif

		throwException("org/jnativehook/keyboard/NativeKeyException", "Could not determine the keyboard auto repeat delay.");
		return -1; //Naturally exit so jni exception is thrown.
	}

	#ifdef DEBUG
		printf("Native: XkbGetAutoRepeatRate successful (rate: %i) (delay: %i)\n", xkb_interval, xkb_timeout);
	#endif
	return (jlong) xkb_timeout;
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
	char * xkb_time = XGetDefault(XToolkit.getDisplay(), "*", "multiClickTime");

	#ifdef DEBUG
		printf("Native: GetDoubleClickTime() successful (time: %ldd)\n", wkb_time);
	#endif
	return (jlong) wkb_time;
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
