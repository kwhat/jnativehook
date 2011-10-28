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

#include <X11/extensions/record.h>

#include "JNativeHook.h"
#include "JConvertToNative.h"
#include "NativeThread.h"
#include "XButtonCodes.h"
#include "XEventModifiers.h"

//For this struct, refer to libxnee
typedef union {
	unsigned char		type;
	xEvent				event;
	xResourceReq		req;
	xGenericReply		reply;
	xError				error;
	xConnSetupPrefix	setup;
} XRecordDatum;

//The pointer to the X11 display for the hook and data.
extern Display * disp_data;
static Display * disp_hook;



//Thread and hook handles.
static bool isRunning = true;
static pthread_t hookThreadId = 0;
static XRecordContext context = 0;

//GlobalScreen object and dispatch id.
static jobject objGlobalScreen = NULL;
static jmethodID idDispatchEvent;

//Java callback classes and constructor id's
static jclass clsKeyEvent, clsMouseEvent, clsMouseWheelEvent;
static jmethodID idKeyEvent, idMouseButtonEvent, idMouseMotionEvent;

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

static void LowLevelProc(XPointer pointer, XRecordInterceptData * hook) {
	if (hook->category == XRecordFromServer || hook->category == XRecordFromClient) {
		//We should already be attached to the JVM at this point.  This should only
		//be a formality that causes a NOOP.
		JNIEnv * env = NULL;
		if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
			//Get XRecord data.
			XRecordDatum * data = (XRecordDatum *) hook->data;

			//Native Event Data
			int event_type = data->type;
			BYTE event_code = data->event.u.u.detail;
			int event_mask = data->event.u.keyButtonPointer.state;
			int event_root_x = data->event.u.keyButtonPointer.rootX;
			int event_root_y = data->event.u.keyButtonPointer.rootY;
			int event_time = hook->server_time;
			KeySym keysym;

			//Java Event Data
			JKeyDatum jkey;
			jint jbutton;
			jint modifiers;

			//Java Event Objects
			jobject objKeyEvent, objMouseEvent;

			switch (event_type) {
				case KeyPress:
					#ifdef DEBUG
						fprintf(stdout, "LowLevelProc: Key pressed. (%i)\n", event_code);
					#endif

					keysym = XKeycodeToKeysym(disp_data, event_code, 0);
					jkey = NativeToJKey(keysym);
					modifiers = doModifierConvert(event_mask);

					//Fire key pressed event.
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_PRESSED, (jlong) event_time, modifiers, event_code, jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				break;

				case KeyRelease:
					#ifdef DEBUG
						fprintf(stdout, "LowLevelProc: Key released. (%i)\n", event_code);
					#endif

					keysym = XKeycodeToKeysym(disp_data, event_code, 0);
					jkey = NativeToJKey(keysym);
					modifiers = doModifierConvert(event_mask);

					//Fire key released event.
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_RELEASED, (jlong) event_time, modifiers, event_code, jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				break;

				case ButtonPress:
					#ifdef DEBUG
						fprintf(stdout, "LowLevelProc: Button pressed. (%i)\n", event_code);
					#endif

					//FIXME Dirty hack to prevent scroll events.
					//FIXME Button2 and 3 are reversed from other platforms.
					/* This information is all static for X11, its up to the WM to
					 * decide how to interpret the wheel events.
					 *
					Scroll type: WHEEL_UNIT_SCROLL
					Scroll amount: 3 unit increments per notch
					Units to scroll: 3 unit increments
					Vertical unit increment: 15 pixels
					 */
					if (event_code > 0 && (event_code <= 3 || event_code == 8 || event_code == 9)) {
						jbutton = NativeToJButton(event_code);
						modifiers = doModifierConvert(event_mask);

						//Fire mouse released event.
						objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, JK_NATIVE_MOUSE_PRESSED, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y, jbutton);
						(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
					}
				break;

				case ButtonRelease:
					#ifdef DEBUG
						fprintf(stdout, "LowLevelProc: Button released. (%i)\n", event_code);
					#endif

					//FIXME Dirty hack to prevent scroll events.
					//FIXME Button2 and 3 are reversed from other platforms.
					if (event_code > 0 && (event_code <= 3 || event_code == 8 || event_code == 9)) {
						jbutton = NativeToJButton(event_code);
						modifiers = doModifierConvert(event_mask);

						//Fire mouse released event.
						objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, JK_NATIVE_MOUSE_RELEASED, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y, jbutton);
						(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
					}
				break;

				case MotionNotify:
					#ifdef DEBUG
						fprintf(stdout, "LowLevelProc: Motion Notified. (%i, %i)\n", event_root_x, event_root_y);
					#endif

					modifiers = doModifierConvert(event_mask);

					//Fire mouse moved event.
					objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseMotionEvent, JK_NATIVE_MOUSE_MOVED, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
				break;

				default:
					#ifdef DEBUG
						fprintf(stderr, "LowLevelProc: Unhandled Event Type!\n");
					#endif
				break;
			}

			//Handle any possible JNI issue that may have occured.
			if ((*env)->ExceptionCheck(env) == JNI_TRUE) {
				#ifdef DEBUG
					fprintf(stderr, "LowLevelProc: JNI error occurred!\n");
					(*env)->ExceptionDescribe(env);
				#endif
				(*env)->ExceptionClear(env);
			}
		}
	}

	XRecordFreeData(hook);
}

static void CreateJNIGlobals(JNIEnv * env) {
	//Class and getInstance method id for the GlobalScreen Object
	jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
	jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");

	//A reference to the GlobalScreen Object
	jobject objScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);
	objGlobalScreen = (*env)->NewGlobalRef(env, objScreen);

	//Get the ID of the GlobalScreen Objects dispatch event method.
	idDispatchEvent = (*env)->GetMethodID(env, clsGlobalScreen, "dispatchEvent", "(Lorg/jnativehook/NativeInputEvent;)V");

	//Class and Constructor for the NativeKeyEvent Object
	jclass clsLocalKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
	clsKeyEvent = (*env)->NewGlobalRef(env, clsLocalKeyEvent);
	idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

	//Class and Constructor for the NativeMouseEvent Object
	jclass clsLocalMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
	clsMouseEvent = (*env)->NewGlobalRef(env, clsLocalMouseEvent);
	idMouseButtonEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");
	idMouseMotionEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIII)V");

	//Class and Constructor for the NativeMouseWheelEvent Object
	jclass clsLocalMouseWheelEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseWheelEvent");
	clsMouseWheelEvent = (*env)->NewGlobalRef(env, clsLocalMouseWheelEvent);
	//FIXME need to change the constructor signature
	//idMouseWheelEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIII)V");
}

static void DestroyJNIGlobals(JNIEnv * env) {
	(*env)->DeleteGlobalRef(env, clsKeyEvent);
	(*env)->DeleteGlobalRef(env, clsMouseEvent);
	(*env)->DeleteGlobalRef(env, clsMouseWheelEvent);
	(*env)->DeleteGlobalRef(env, objGlobalScreen);
}

static void * ThreadProc(LPVOID UNUSED(lpParameter)) {
	DWORD status = EXIT_FAILURE;

	//Attach the current thread to the JVM.
	JNIEnv * env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		//Create all the global references up front to save time in the callbacks.
		CreateJNIGlobals(env);

		//Grab the default display
		char * disp_name = XDisplayName(NULL);
		disp_hook = XOpenDisplay(disp_name);
		disp_data = XOpenDisplay(disp_name);
		#ifdef DEBUG
		if (disp_hook != NULL && disp_data != NULL) {
			printf("Native: XOpenDisplay successful.\n");
		}
		else {
			printf("Native: XOpenDisplay failure!\n");
		}
		#endif

		//Check to make sure XRecord is installed and enabled.
		int major, minor;
		if (XRecordQueryVersion(disp_hook, &major, &minor) != false) {
			#ifdef DEBUG
				printf ("Native: XRecord version: %d.%d.\n", major, minor);
			#endif
		}
		else {
			#ifdef DEBUG
				printf ("Native: XRecord is not currently available!\n");
			#endif

			disp_hook = NULL;
			disp_data = NULL;
		}

		//enable detectable autorepeat.
		Bool isAutoRepeat;
		XkbSetDetectableAutoRepeat(disp_data, true, &isAutoRepeat);
		#ifdef DEBUG
		if (isAutoRepeat) {
			printf("Native: XkbSetDetectableAutoRepeat successful.\n");
		}
		else {
			printf("Native: Could not enable detectable autorepeat!\n");
		}
		#endif


		//Setup XRecord range
		XRecordClientSpec clients = XRecordAllClients;
		XRecordRange * range = XRecordAllocRange();
		if (range != NULL) {
			#ifdef DEBUG
				printf("Native: XRecordAllocRange successful.\n");
			#endif

			//Create XRecord Context
			range->device_events.first = KeyPress;
			range->device_events.last = MotionNotify;
			context = XRecordCreateContext(disp_hook, 0, &clients, 1, &range, 1);
			XFree(range);
		}
		else {
			#ifdef DEBUG
				printf("Native: XRecordAllocRange failure!\n");
			#endif
		}


		if (context != 0) {
			#ifdef DEBUG
				printf("Native: XRecordCreateContext successful.\n");
			#endif

			//Async works with the loop and outside of threads.
			XRecordEnableContextAsync(disp_hook, context, callback, NULL);
			//XRecordEnableContext(disp_hook, context, callback, NULL);

			while (isRunning) {
				XRecordProcessReplies(disp_hook);
			}

		}
		else {
			#ifdef DEBUG
				printf("Native: XRecordCreateContext failure!\n");
			#endif
		}


		//Destroy the native displays.
		if (disp_data != NULL) {
			XRecordDisableContext(disp_data, context);
			XCloseDisplay(disp_data);
			disp_data = NULL;
		}

		if (disp_hook != NULL) {
			XRecordFreeContext(disp_hook, context);
			XCloseDisplay(disp_hook);
			disp_hook = NULL;
		}


		//Make sure we clean up the global objects.
		DestroyJNIGlobals(env);

		//Dettach the current thread to the JVM.
		if ((*jvm)->DetachCurrentThread(jvm) != JNI_OK) {
			#ifdef DEBUG
				fprintf(stderr, "DetachCurrentThread(jvm, (void **)(&env), NULL) failed!\n");
			#endif
		}
	}
	else {
		//We cant do a whole lot of anythign if we cant attach to the current thread.
		#ifdef DEBUG
			fprintf(stderr, "AttachCurrentThread(jvm, (void **)(&env), NULL) failed!\n");
		#endif
	}

	#ifdef DEBUG
		fprintf(stdout, "ThreadProc() has completed.\n");
	#endif

	//Make sure we signal that we have passed any exception throwing code.
	//This should only make a difference if we had an initialization exception.
	SetEvent(hookEventHandle);

	ExitThread(status);
}

DWORD GetThreadStatus() {
	DWORD status;
	GetExitCodeThread(hookThreadHandle, &status);

	return status;
}

int StartNativeThread() {
	int status = EXIT_SUCCESS;

	//Make sure the native thread is not already running.
	if (IsNativeThreadRunning() != true) {
		//Call listener
		isRunning = true;

		/*
		 * We shall use the default pthread attributes: thread is joinable
		 * (not detached) and has default (non real-time) scheduling policy.
		pthread_attr_t attr = NULL;
		pthread_attr_init(&attr);
		pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); //sets system contention scope for thread
		//pthread_attr_setstacksize(&attr, 512*1024);
		if( pthread_create( &hookThreadId, &attr, MsgLoop, NULL) ) {
		*/
		if( pthread_create( &hookThreadId, NULL, MsgLoop, NULL) ) {
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
	}

	return status;
}

int StopNativeThread() {
	int status = EXIT_SUCCESS;

	if (IsNativeThreadRunning() == true) {
		//Try to exit the thread naturally.
		isRunning = false;
		pthread_join(hookThreadId, NULL);

		//Make sure the thread has stopped.
		if (pthread_kill(hookThreadId, SIGKILL) == 0) {
			#ifdef DEBUG
				printf("Native: pthread_kill successful.\n");
			#endif
		}
	}

	return status;
}

bool IsNativeThreadRunning() {
	return GetThreadStatus() == STILL_ACTIVE;
}
