/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/record.h>

#include "NativeErrors.h"
#include "NativeGlobals.h"
#include "NativeHelpers.h"
#include "NativeThread.h"
#include "JConvertFromNative.h"
#include "JMouseWheel.h"
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


//The pointer to the X11 display accessed by the callback.
static Display * disp_ctrl;
static XRecordContext context;

//Thread and hook handles.
#ifdef XRECORD_ASYNC
static bool running;
#endif
static pthread_mutex_t hookRunningMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t hookControlMutex;
static pthread_t hookThreadId = 0;


//Convert the XEvent modifier mask to a Java modifier mask.
static jint DoModifierConvert(int event_mask) {
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

static void LowLevelProc(XPointer UNUSED(pointer), XRecordInterceptData * hook) {
	if (hook->category == XRecordFromServer || hook->category == XRecordFromClient) {
		//We should already be attached to the JVM at this point.  This should only
		//be a formality that causes a NOOP.
		JNIEnv * env = NULL;
		if (disp_ctrl != NULL && (*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
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
			jint scrollType, scrollAmount, wheelRotation;
			jint modifiers;

			//Java Event Objects
			jobject objKeyEvent, objMouseEvent, objMouseWheelEvent;

			switch (event_type) {
				case KeyPress:
					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Key pressed. (%i)\n", event_code);
					#endif

					keysym = XKeycodeToKeysym(disp_ctrl, event_code, 0);
					jkey = NativeToJKey(keysym);
					modifiers = DoModifierConvert(event_mask);

					//Fire key pressed event.
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_PRESSED, (jlong) event_time, modifiers, event_code, jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				break;

				case KeyRelease:
					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Key released. (%i)\n", event_code);
					#endif

					keysym = XKeycodeToKeysym(disp_ctrl, event_code, 0);
					jkey = NativeToJKey(keysym);
					modifiers = DoModifierConvert(event_mask);

					//Fire key released event.
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_RELEASED, (jlong) event_time, modifiers, event_code, jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				break;

				case ButtonPress:
					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Button pressed. (%i)\n", event_code);
					#endif

					//FIXME Button2 and 3 are reversed from other platforms.
					/* This information is all static for X11, its up to the WM to
					 * decide how to interpret the wheel events.
					 */
					modifiers = DoModifierConvert(event_mask);
					if (event_code > 0 && (event_code <= 3 || event_code == 8 || event_code == 9)) {
						jbutton = NativeToJButton(event_code);

						//Fire mouse released event.
						objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, JK_NATIVE_MOUSE_PRESSED, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y, jbutton);
						(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
					}
					else if (event_code == 4 || event_code == 5) {
						//Scroll wheel release events.
						/*
							Scroll type: WHEEL_UNIT_SCROLL
							Scroll amount: 3 unit increments per notch
							Units to scroll: 3 unit increments
							Vertical unit increment: 15 pixels
						*/

						/* X11 does not have an API call for aquiring the mouse scroll type.  This
						 * maybe part of the XInput2 (XI2) extention but I will wont know until it
						 * is available on my platform.  For the time being we will just use the
						 * unit scroll value.
						 */
						scrollType = (jint) WHEEL_UNIT_SCROLL;

						/* Some scroll wheel properties are avaiable via the new XInput2 (XI2)
						 * extention.  Unfortunately the extention is not available on my
						 * development platform at this time.  For the time being we will just
						 * use the Windows default value of 3.
						 */
						scrollAmount = (jint) 3;
						
						if (event_code == 4) {
							//Wheel Rotated Up and Away
							wheelRotation = -1;
						}
						else { //event_code == 5
							//Wheel Rotated Down and Towards
							wheelRotation = 1;
						}

						//Fire mouse wheel event.
						objMouseWheelEvent = (*env)->NewObject(env, clsMouseWheelEvent, idMouseWheelEvent, JK_NATIVE_MOUSE_WHEEL, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y, scrollType, scrollAmount, wheelRotation);
						(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseWheelEvent);
					}
				break;

				case ButtonRelease:
					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Button released. (%i)\n", event_code);
					#endif

					//FIXME Button2 and 3 are reversed from other platforms.
					if (event_code > 0 && (event_code <= 3 || event_code == 8 || event_code == 9)) {
						//Handle button release events
						jbutton = NativeToJButton(event_code);
						modifiers = DoModifierConvert(event_mask);

						//Fire mouse released event.
						objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, JK_NATIVE_MOUSE_RELEASED, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y, jbutton);
						(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
					}
					/*
					else if (event_code == 4 || event_code == 5) {
						//Scroll wheel release events, not needed for this platform.
					}
					*/
				break;

				case MotionNotify:
					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Motion Notified. (%i, %i)\n", event_root_x, event_root_y);
					#endif

					modifiers = DoModifierConvert(event_mask);

					//Check the upper half of java modifiers for non zero value.
					if (modifiers >> 4 > 0) {
						//Create Mouse Dragged event.
						objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseMotionEvent, JK_NATIVE_MOUSE_DRAGGED, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y);
					}
					else {
						//Create a Mouse Moved event
						objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseMotionEvent, JK_NATIVE_MOUSE_MOVED, (jlong) event_time, modifiers, (jint) event_root_x, (jint) event_root_y);
					}
					
					//Fire mouse moved event.
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
				break;

				#ifdef DEBUG
				default:
					fprintf(stderr, "LowLevelProc(): Unhandled Event Type!\n");
				break;
				#endif
			}

			//Handle any possible JNI issue that may have occured.
			if ((*env)->ExceptionCheck(env) == JNI_TRUE) {
				#ifdef DEBUG
				fprintf(stderr, "LowLevelProc(): JNI error occurred!\n");
				(*env)->ExceptionDescribe(env);
				#endif
				(*env)->ExceptionClear(env);
			}
		}
	}

	XRecordFreeData(hook);
}

static void * ThreadProc(void * arg) {
	pthread_mutex_lock(&hookControlMutex);
	pthread_mutex_lock(&hookRunningMutex);
	
	int * status = (int *) arg;
	*status = RETURN_FAILURE;

	//XRecord context for use later.
	context = 0;

	//Grab the default display
	char * disp_name = XDisplayName(NULL);
	disp_ctrl = XOpenDisplay(disp_name);
	Display * disp_data = XOpenDisplay(disp_name);
	if (disp_ctrl != NULL && disp_data != NULL) {
		#ifdef DEBUG
		fprintf(stdout, "ThreadProc(): XOpenDisplay successful.\n");
		#endif

		//Check to make sure XRecord is installed and enabled.
		int major, minor;
		if (XRecordQueryVersion(disp_data, &major, &minor) != false) {
			#ifdef DEBUG
			fprintf(stdout, "ThreadProc(): XRecord version: %d.%d.\n", major, minor);
			#endif

			//Setup XRecord range
			XRecordClientSpec clients = XRecordAllClients;
			XRecordRange * range = XRecordAllocRange();
			if (range != NULL) {
				#ifdef DEBUG
				fprintf(stdout, "ThreadProc(): XRecordAllocRange successful.\n");
				#endif

				//Create XRecord Context
				range->device_events.first = KeyPress;
				range->device_events.last = MotionNotify;
				context = XRecordCreateContext(disp_data, 0, &clients, 1, &range, 1);
				XFree(range);
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "ThreadProc(): XRecordAllocRange failure!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf (stderr, "ThreadProc(): XRecord is not currently available!\n");
		}
		#endif
	}
	#ifdef DEBUG
	else {
		fprintf(stderr, "ThreadProc(): XOpenDisplay failure!\n");
	}
	#endif


	if (context != 0) {
		//Set the exit status.
		*status = RETURN_SUCCESS;

		#ifdef DEBUG
		fprintf(stdout, "ThreadProc(): XRecordCreateContext successful.\n");
		#endif

		pthread_mutex_unlock(&hookControlMutex);

		#ifdef XRECORD_ASYNC
		//Async requires that we loop so that our thread does not return.
		XRecordEnableContextAsync(disp_data, context, LowLevelProc, NULL);
		while (running) {
			XRecordProcessReplies(disp_data);
		}
		XRecordDisableContext(disp_ctrl, context);
		XSync(disp_ctrl, True);
		#else
		//We should be using this but its broken upstream.
		XRecordEnableContext(disp_data, context, LowLevelProc, NULL);
		#endif

		//Lock back up until we are done processing the exit.
		pthread_mutex_lock(&hookControlMutex);
		XRecordFreeContext(disp_ctrl, context);
	}
	#ifdef DEBUG
	else {
		fprintf(stderr, "ThreadProc(): XRecordCreateContext failure!\n");
	}
	#endif

	//Close down any open displays.
	if (disp_ctrl != NULL) {
		XCloseDisplay(disp_ctrl);
		disp_ctrl = NULL;
	}

	if (disp_data != NULL) {
		XCloseDisplay(disp_data);
		disp_data = NULL;
	}

	#ifdef DEBUG
	fprintf(stdout, "ThreadProc(): complete.\n");
	#endif

	//Make sure we signal that we have passed any exception throwing code.
	pthread_mutex_unlock(&hookRunningMutex);
	pthread_mutex_unlock(&hookControlMutex);

	pthread_exit(status);
}

int StartNativeThread() {
	int status = RETURN_FAILURE;

	//Make sure the native thread is not already running.
	if (IsNativeThreadRunning() != true) {
		//Lock the mutex handle for the thread hook.
		pthread_mutex_init(&hookControlMutex, NULL);

		//Create all the global references up front to save time in the callbacks.
		if (CreateJNIGlobals() == RETURN_SUCCESS) {
			#ifdef XRECORD_ASYNC
			//Allow the thread loop to block.
			running = true;
			#endif

			//We shall use the default pthread attributes: thread is joinable
			//(not detached) and has default (non real-time) scheduling policy.
			if (pthread_create(&hookThreadId, NULL, ThreadProc, malloc(sizeof(int))) == 0) {
				#ifdef DEBUG
				fprintf(stdout, "StartNativeThread(): start successful.\n");
				#endif

				//Wait for the thread to start up.
				if (pthread_mutex_lock(&hookControlMutex) == 0) {
					pthread_mutex_unlock(&hookControlMutex);
				}

				#ifdef DEBUG
				fprintf(stdout, "StartNativeThread(): initialization successful.\n");
				#endif

				status = RETURN_SUCCESS;
			}
			else {
				#ifdef XRECORD_ASYNC
				running = false;
				#endif

				#ifdef DEBUG
				fprintf(stderr, "StartNativeThread(): start failure!\n");
				#endif
			}
		}
		else {
			//We cant do a whole lot of anything if we cant attach to the current thread.
			#ifdef DEBUG
			fprintf(stderr, "StartNativeThread(): CreateJNIGlobals() failed!\n");
			#endif


			//FIXME An exception should be raised.
		}
	}

	return status;
}

int StopNativeThread() {
	int status = RETURN_FAILURE;

	if (IsNativeThreadRunning()) {
		//Lock the thread.
		pthread_mutex_lock(&hookControlMutex);

		#ifdef XRECORD_ASYNC
		//Try to exit the thread naturally.
		running = false;
		#else
		//Try to exit the thread naturally.
		XRecordDisableContext(disp_ctrl, context);
		XSync(disp_ctrl, True);
		#endif

		//Must unlock to allow the thread to finish cleaning up.
		pthread_mutex_unlock(&hookControlMutex);

		//Wait for the thread to die.
		void * thread_status;
		pthread_join(hookThreadId, (void *) &thread_status);
		status = *(int *) thread_status;
		#ifdef DEBUG
		fprintf(stdout, "StopNativeThread(): Thread Result (%i)\n", *(int *) thread_status);
		#endif
		free(thread_status);

		//Destroy all created globals.
		#ifdef DEBUG
		if (DestroyJNIGlobals() == RETURN_FAILURE) {
			//Leaving dangling global references will leak a small amout of memory
			//but because there is nothing that can be done about it at this point
			//an exception will not be thrown.
			fprintf(stderr, "StopNativeThread(): DestroyJNIGlobals() failed!\n");
		}
		#else
		DestroyJNIGlobals();
		#endif
		
		//Clean up the mutex.
		pthread_mutex_destroy(&hookControlMutex);
	}

	return status;
}

bool IsNativeThreadRunning() {
	bool isRunning = false;

	//Wait for a lock on the thread.
	if (pthread_mutex_lock(&hookControlMutex) == 0) {
		//Lock Successful.

		if (pthread_mutex_trylock(&hookRunningMutex) == 0) {
			//Lock Successful, we are not running.
			pthread_mutex_unlock(&hookRunningMutex);
		}
		else {
			isRunning = true;
		}

		#ifdef DEBUG
		fprintf(stdout, "IsNativeThreadRunning(): State (%i)\n", isRunning);
		#endif

		pthread_mutex_unlock(&hookControlMutex);
	}
	#ifdef DEBUG
	else {
		//Lock Failure. This should always be caused by an invalid pointer
		//and/or an uninitialized mutex.
		fprintf(stderr, "IsNativeThreadRunning(): Failed to acquire control mutex lock!\n");
	}
	#endif

	return isRunning;
}
