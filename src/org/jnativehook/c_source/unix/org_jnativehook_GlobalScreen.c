/* Copyright (c) 2006-2010 - Alexander Barker (alex@1stleg.com)
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *	JNativeHook - GrabKeyHook - 09/08/06
 *  Alexander Barker
 *
 *	JNI Interface for setting a Keyboard Hook and monitoring
 *	it with java.
 */

/*
Compiling Options:
	gcc -m32 -march=i586 -shared -fPIC -lX11 -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux ./org_jnativehook_keyboard_GrabKeyHook.c -o libJNativeHook_Keyboard.so
	gcc -m64 -march=k8 -shared -fPIC -lX11 -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux ./org_jnativehook_keyboard_GrabKeyHook.c -o libJNativeHook_Keyboard.so
*/

#ifdef UNUSED
#elif defined(__GNUC__)
	#define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
	#define UNUSED(x) /*@unused@*/ x
#else
	#define UNUSED(x) x
#endif


#ifdef DEBUG
	#include <stdio.h>
	#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <pthread.h>
#include <signal.h>

#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/extensions/record.h>

#include <jni.h>

#include "include/org_jnativehook_GlobalScreen.h"
#include "include/JConvertToNative.h"
#include "xEventModifers.h"

//Instance Variables
bool isRunning = true;
unsigned int xkb_timeout;
unsigned int xkb_interval;

/* for this struct, refer to libxnee */
typedef union {
	unsigned char		type;
	xEvent				event;
	xResourceReq		req;
	xGenericReply		reply;
	xError				error;
	xConnSetupPrefix	setup;
} XRecordDatum;

Display * disp_hook;
Display * disp_data;
XRecordContext context;

JavaVM * jvm = NULL;
pthread_t hookThreadId = 0;

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

int xErrorToException(Display * dpy, XErrorEvent * e) {
	char message[255];
	XGetErrorText(dpy, e->error_code, message, sizeof message);

	#ifdef DEBUG
		printf("Native: XError %i - %s.\n", e->error_code, message);
	#endif

	throwException("org/jnativehook/NativeHookException", message);

	return 0;
}

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

void callback(XPointer pointer, XRecordInterceptData * hook) {

	if (hook->category != XRecordFromServer && hook->category != XRecordFromClient) {
		XRecordFreeData(hook);
		return;
	}

	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Class and getInstance method id for the GlobalScreen Object
	jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
	jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");

	//A reference to the GlobalScreen Object
	jobject objGlobalScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);


	XRecordDatum *data = (XRecordDatum*) hook->data;

	//Event Data
	int event_type = data->type;
	BYTE event_code = data->event.u.u.detail;
	int event_mask = data->event.u.keyButtonPointer.state;
	int event_root_x = data->event.u.keyButtonPointer.rootX;
	int event_root_y = data->event.u.keyButtonPointer.rootY;
	int event_time = hook->server_time;
	KeySym keysym;


	JKeyDatum jkey;
	jint jbutton;
	jint modifiers;
	jclass clsKeyEvent, clsMouseEvent;
	jmethodID idKeyEvent, idMouseEvent;
	jmethodID idDispatchEvent = (*env)->GetMethodID(env, clsGlobalScreen, "dispatchEvent", "(Lorg/jnativehook/NativeInputEvent;)V");
	jobject objKeyEvent, objMouseEvent;

	switch (event_type) {
		case KeyPress:
			#ifdef DEBUG
				printf("Native: MsgLoop - Key pressed (%i)\n", event_code);
			#endif

			//Class and Constructor for the NativeKeyEvent Object
			clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
			idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

			keysym = XKeycodeToKeysym(disp_data, event_code, 0);
			jkey = NativeToJKey(keysym);
			modifiers = doModifierConvert(event_mask);

			//Fire key pressed event.
			objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_PRESSED, (jlong) event_time, modifiers, event_code, jkey.keycode, jkey.location);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		break;

		case KeyRelease:
			#ifdef DEBUG
				printf("Native: MsgLoop - Key released (%i)\n", event_code);
			#endif

			//Class and Constructor for the NativeKeyEvent Object
			clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
			idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

			keysym = XKeycodeToKeysym(disp_data, event_code, 0);
			jkey = NativeToJKey(keysym);
			modifiers = doModifierConvert(event_mask);

			//Fire key released event.
			objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_RELEASED, (jlong) event_time, modifiers, event_code, jkey.keycode, jkey.location);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		break;

		case ButtonPress:
			#ifdef DEBUG
				printf("Native: MsgLoop - Button pressed (%i)\n", event_code);
			#endif

			//Class and Constructor for the NativeMouseEvent Object
			clsMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
			idMouseEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");

			jbutton = NativeToJButton(event_code);
			modifiers = doModifierConvert(event_mask);

			//Fire mouse released event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseEvent, JK_NATIVE_MOUSE_PRESSED, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y, jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case ButtonRelease:
			#ifdef DEBUG
				printf("Native: MsgLoop - Button released (%i)\n", event_code);
			#endif

			//Class and Constructor for the NativeMouseEvent Object
			clsMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
			idMouseEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");

			jbutton = NativeToJButton(event_code);
			modifiers = doModifierConvert(event_mask);

			//Fire mouse released event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseEvent, JK_NATIVE_MOUSE_RELEASED, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y, jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case MotionNotify:
			#ifdef DEBUG
				printf ("Native: MsgLoop - Motion Notified (%i,%i)\n", event_root_x, event_root_y);
			#endif

			//Class and Constructor for the NativeMouseEvent Object
			clsMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
			idMouseEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIII)V");

			modifiers = doModifierConvert(event_mask);

			//Fire mouse moved event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseEvent, JK_NATIVE_MOUSE_MOVED, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case CreateNotify:
			#ifdef DEBUG
				printf ("Native: MsgLoop - Create Notified (Unimplemented)\n");
			#endif
		break;

		case DestroyNotify:
			#ifdef DEBUG
				printf ("Native: MsgLoop - Destroy Notified (Unimplemented)\n");
			#endif
		break;

		case NoExpose:
			#ifdef DEBUG
				printf ("Native: MsgLoop - Not Exposed (Unimplemented)\n");
			#endif
		break;

		case Expose:
			#ifdef DEBUG
				printf ("Native: MsgLoop - Exposed (Unimplemented)\n");
			#endif
		break;

		default:
			#ifdef DEBUG
				printf ("Native: MsgLoop - Unhandled Event Type\n");
			#endif
		break;
	}

	#ifdef DEBUG
		if ((*env)->ExceptionOccurred(env)) {
			printf("Native: JNI Error Occurred.\n");
			((*env)->ExceptionDescribe(env));
		}
	#endif

	XRecordFreeData(hook);
}

void MsgLoop() {
	//Setup XRecord range
	XRecordClientSpec clients = XRecordAllClients;
	XRecordRange * range = XRecordAllocRange();
	if (range == NULL) {
		throwException("org/jnativehook/NativeHookException", "Could not allocate XRecordRange");
		return; //Naturally exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf("Native: XRecordAllocRange successful.\n");
		#endif
	}

	//Create XRecord Context
	range->device_events.first = KeyPress;
	range->device_events.last = MotionNotify;
	context = XRecordCreateContext(disp_hook, 0, &clients, 1, &range, 1);
	XFree(range);
	if (context == 0) {
		throwException("org/jnativehook/NativeHookException", "Could not create XRecordContext");
		return; //Naturally exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf("Native: XRecordCreateContext successful.\n");
		#endif
	}

	XRecordEnableContextAsync(disp_hook, context, callback, NULL);
	//XRecordEnableContext(disp_hook, context, callback, NULL);

	while (isRunning) {
		XRecordProcessReplies(disp_hook);
	}

	#ifdef DEBUG
		printf("Native: MsgLoop() stop successful.\n");
	#endif
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

//This is where java attaches to the native machine.  Its kind of like the java + native constructor.
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * vm, void * UNUSED(reserved)) {
	#ifdef DEBUG
		printf("Native: JNI_OnLoad - Shared Object Process Attach.\n");
	#endif

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

	//Tell X Threads are OK
	XInitThreads();

	//Set the native error handler.
	XSetErrorHandler((XErrorHandler) xErrorToException);

	//Grab the default display
	char * disp_name = XDisplayName(NULL);
	disp_hook = XOpenDisplay(disp_name);
	disp_data = XOpenDisplay(disp_name);
	if (disp_hook == NULL || disp_data == NULL) {
		//We couldnt hook a display so we need to die.
		char * error_msg = "Could not open display: ";
		char * exceptoin_msg = (char *) malloc( (strlen(error_msg) + strlen(disp_name)) + 1 * sizeof(char));

		strcat(exceptoin_msg, error_msg);
		strcat(exceptoin_msg, disp_name);

		throwException("org/jnativehook/NativeHookException", exceptoin_msg);
		free(exceptoin_msg);

		return JNI_ERR; //Naturally exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf("Native: XOpenDisplay successful.\n");
		#endif
	}

	//Check to make sure XRecord is installed and enabled.
	int major, minor;
	if (!XRecordQueryVersion(disp_hook, &major, &minor)) {
		throwException("org/jnativehook/NativeHookException", "XRecord is not currently available");
		return JNI_ERR; //Naturally exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf ("Native: XRecord version: %d.%d\n", major, minor);
		#endif
	}

	//enable detectable autorepeat.
	Bool isAutoRepeat;
	XkbSetDetectableAutoRepeat(disp_data, true, &isAutoRepeat);
	if (!isAutoRepeat) {
		#ifdef DEBUG
			printf("Native: Could not enable detectable autorepeat.\n");
		#endif
	}
	else {
		#ifdef DEBUG
			printf("Native: XkbSetDetectableAutoRepeat successful\n");
		#endif
	}

	//Call listener
	isRunning = true;

	if( pthread_create( &hookThreadId, NULL, (void *) &MsgLoop, NULL) ) {
		#ifdef DEBUG
			printf("Native: MsgLoop() start failure.\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Could not create message loop thread.");
		return JNI_ERR; //Naturally exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf("Native: MsgLoop() start successful.\n");
		#endif
	}

	return jni_ret;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * UNUSED(vm), void * UNUSED(reserved)) {
	if (disp_hook != NULL) {
		XRecordDisableContext(disp_hook, context);
		XRecordFreeContext(disp_hook, context);

		XCloseDisplay(disp_hook);
		disp_hook = NULL;
	}

	if (disp_data != NULL) {
		XCloseDisplay(disp_data);
		disp_data = NULL;
	}

	//Try to exit the thread naturally.
	isRunning = false;
	pthread_join(hookThreadId, NULL);


	//Make sure the thread has stopped.
	if (pthread_kill(hookThreadId, SIGKILL) == 0) {
		#ifdef DEBUG
			printf("Native: pthread_kill successful.\n");
		#endif
	}

	#ifdef DEBUG
		printf("Native: JNI_OnUnload successful.\n");
	#endif
}
