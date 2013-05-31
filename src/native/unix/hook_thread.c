/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef XRECORD_ASYNC
#define _POSIX_C_SOURCE 199309L
#include <time.h>
#endif

#include <pthread.h>
#include <sys/time.h>

#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/record.h>

#include "nativehook.h"
#include "convert_to_native.h"
#include "convert_to_virtual.h"
#include "error.h"
#include "hook_thread.h"
#include "nativehook.h"
#include "system_properties.h"
#include "x_input_helpers.h"
#include "x_wheel_codes.h"

// For this struct, refer to libxnee.
typedef union {
	unsigned char		type;
	xEvent				event;
	xResourceReq		req;
	xGenericReply		reply;
	xError				error;
	xConnSetupPrefix	setup;
} XRecordDatum;

// Mouse globals.
static unsigned short clickCount = 0;
static long clickTime = 0;
static bool mouseDragged = false;

// The pointer to the X11 display accessed by the callback.
static Display *dspCtrl;
static XRecordContext context;

// Thread and hook handles.
#ifdef XRECORD_ASYNC
static volatile bool running;
#endif
static pthread_mutex_t hookRunningMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t hookControlMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t hookThreadId;
static pthread_attr_t hookThreadAttr;

// Virtual event pointers
static VritualEvent *event = null;
static KeyboardEventData *keyboardData = null;
static MouseEventData *mouseData = null;
static MouseWheelEventData *mouseWheelData = null;

static void LowLevelProc(XPointer UNUSED(pointer), XRecordInterceptData *hook) {
	if (hook->category == XRecordStartOfData) {
		pthread_mutex_lock(&hookRunningMutex);
		pthread_mutex_unlock(&hookControlMutex);
	}
	else if (hook->category == XRecordEndOfData) {
		//pthread_mutex_lock(&hookControlMutex);
		pthread_mutex_unlock(&hookRunningMutex);
	}
	else if (hook->category == XRecordFromServer || hook->category == XRecordFromClient) {
		if (dspCtrl != NULL) {
			// Check and make sure the thread is stull running to avoid the
			// potential crash associated with late event arrival.  This code is
			// guaranteed to run after all thread start.
			if (pthread_mutex_trylock(&hookRunningMutex) != 0) {
				// Get XRecord data.
				XRecordDatum *data = (XRecordDatum *) hook->data;

				// Native event data.
				struct timeval  timeVal;
				gettimeofday(&timeVal, NULL);
				unsigned long int eventTime = (timeVal.tv_sec * 1000) + (timeVal.tv_usec / 1000);
				int eventType = data->type;
				BYTE eventCode = data->event.u.u.detail;
				int eventMask = data->event.u.keyButtonPointer.state;
				int eventX = data->event.u.keyButtonPointer.rootX;
				int eventY = data->event.u.keyButtonPointer.rootY;

				KeySym keySym;
				KeyCode keyCode;
				wchar_t keyChar;
				unsigned int modifiers;

				unsigned int button;
				int scrollType;
				int scrollAmount;
				int wheelRotation;

				switch (eventType) {
					case KeyPress:
						#ifdef DEBUG
						fprintf(stdout, "LowLevelProc(): Key pressed. (%i)\n", eventCode);
						#endif


						keySym = KeyCodeToKeySym(eventCode, eventMask);
						keyCode = ConvertToVirtualKey(keySym);
						modifiers = ConvertToVirtualMask(eventMask);

						// Fire key pressed event.
						event->type = EVENT_KEY_PRESSED;
						event->time = eventTime
						event->mask = modifiers;
						event->data = keyboardData;

						keyboardData->keycode = keyCode;
						keyboardData->rawcode = eventCode;
						keyboardData->keychar = CHAR_UNDEFINED;

						dispatch_event(event);


						// Check to make sure the key is printable.
						keyChar = KeySymToUnicode(keySym);
						if (keyChar != 0x0000) {
							// Fire key typed event.
							event->type = EVENT_KEY_TYPED;

							keyboardData->keycode = VC_UNDEFINED;
							keyboardData->keychar = keyChar;

							dispatch_event(event);
						}
						break;

					case KeyRelease:
						#ifdef DEBUG
						fprintf(stdout, "LowLevelProc(): Key released. (%i)\n", eventCode);
						#endif

						keySym = KeyCodeToKeySym(eventCode, eventMask);
						keyCode = ConvertToVirtualKey(keySym);
						modifiers = ConvertToVirtualMask(eventMask);

						// Fire key released event.
						event->type = EVENT_KEY_RELEASED;
						event->time = eventTime
						event->mask = modifiers;
						event->data = keyboardData;

						keyboardData->keycode = keyCode;
						keyboardData->rawcode = eventCode;
						keyboardData->keychar = CHAR_UNDEFINED;

						dispatch_event(event);
						break;

					case ButtonPress:
						#ifdef DEBUG
						fprintf(stdout, "LowLevelProc(): Button pressed. (%i)\n", eventCode);
						#endif

						// Track the number of clicks.
						if ((long) (eventTime - clickTime) <= GetMultiClickTime()) {
							clickCount++;
						}
						else {
							clickCount = 1;
						}
						clickTime = eventTime;

						// Convert native modifiers to java modifiers.
						modifiers = ConvertToVirtualMask(eventMask);

						/* This information is all static for X11, its up to the WM to
						* decide how to interpret the wheel events.
						*/
						// TODO Should use constants and a lookup table for button codes.
						if (eventCode > 0 && (eventCode <= 3 || eventCode == 8 || eventCode == 9)) {
							button = ConvertToVirtualButton(eventCode);

							// Fire mouse pressed event.
							event->type = EVENT_MOUSE_PRESSED;
							event->time = eventTime
							event->mask = modifiers;
							event->data = mouseData;

							mouseData->button = button;
							mouseData->clicks = clickCount;
							mouseData->x = eventX;
							mouseData->y = eventY;

							dispatch_event(event);
						}
						else if (eventCode == WheelUp || eventCode == WheelDown) {
							/* Scroll wheel release events.
							* Scroll type: WHEEL_UNIT_SCROLL
							* Scroll amount: 3 unit increments per notch
							* Units to scroll: 3 unit increments
							* Vertical unit increment: 15 pixels
							*/

							/* X11 does not have an API call for acquiring the mouse scroll type.  This
							* maybe part of the XInput2 (XI2) extention but I will wont know until it
							* is available on my platform.  For the time being we will just use the
							* unit scroll value.
							*/
							scrollType = WHEEL_UNIT_SCROLL;

							/* Some scroll wheel properties are available via the new XInput2 (XI2)
							* extention.  Unfortunately the extention is not available on my
							* development platform at this time.  For the time being we will just
							* use the Windows default value of 3.
							*/
							scrollAmount = 3;

							if (eventCode == WheelUp) {
								// Wheel Rotated Up and Away.
								wheelRotation = -1;
							}
							else { // event_code == WheelDown
								// Wheel Rotated Down and Towards.
								wheelRotation = 1;
							}

							// Fire mouse wheel event.
							event->type = EVENT_MOUSE_WHEEL;
							event->time = eventTime
							event->mask = modifiers;
							event->data = mouseWheelData;

							mouseWheelData->type = scrollType;
							mouseWheelData->amount = scrollAmount;
							mouseWheelData->rotation = wheelRotation;

							dispatch_event(event);
						}
						break;

					case ButtonRelease:
						#ifdef DEBUG
						fprintf(stdout, "LowLevelProc(): Button released. (%i)\n", eventCode);
						#endif

						// TODO Should use constants for button codes.
						if (eventCode > 0 && (eventCode <= 3 || eventCode == 8 || eventCode == 9)) {
							// Handle button release events.
							button = ConvertToVirtualButton(eventCode);
							modifiers = ConvertToVirtualMask(eventMask);

							// Fire mouse released event.
							event->type = EVENT_MOUSE_RELEASED;
							event->time = eventTime
							event->mask = modifiers;
							event->data = mouseData;

							mouseData->button = button;
							mouseData->clicks = clickCount;
							mouseData->x = eventX;
							mouseData->y = eventY;

							dispatch_event(event);

							if (mouseDragged != true) {
								// Fire mouse clicked event.
								event->type = EVENT_MOUSE_CLICKED;
								event->time = eventTime
								event->mask = modifiers;
								event->data = mouseData;

								mouseData->button = button;
								mouseData->clicks = clickCount;
								mouseData->x = eventX;
								mouseData->y = eventY;

								dispatch_event(event);
							}
						}
						break;

					case MotionNotify:
						#ifdef DEBUG
						fprintf(stdout, "LowLevelProc(): Motion Notified. (%i, %i)\n", eventX, eventY);
						#endif

						// Reset the click count.
						if (clickCount != 0 && (long) (eventTime - clickTime) > GetMultiClickTime()) {
							clickCount = 0;
						}
						unsigned int modifiers = ConvertToVirtualMask(eventMask);

						// Set the mouse dragged flag.
						mouseDragged = modifiers >> 4 > 0;

						// Check the upper half of virtual modifiers for non zero value.
						if (modifiers >> 4 > 0) {
							// Create Mouse Dragged event.
							event->type = EVENT_MOUSE_DRAGGED;
							event->time = eventTime
							event->mask = modifiers;
							event->data = mouseData;

							mouseData->button = MOUSE_NOBUTTON;
							mouseData->clicks = clickCount;
							mouseData->x = eventX;
							mouseData->y = eventY;
						}
						else {
							// Create a Mouse Moved event.
							event->type = EVENT_MOUSE_MOVED;
							event->time = eventTime
							event->mask = modifiers;
							event->data = mouseData;

							mouseData->button = MOUSE_NOBUTTON;
							mouseData->clicks = clickCount;
							mouseData->x = eventX;
							mouseData->y = eventY;
						}

						// Fire mouse moved event.
						dispatch_event(event);
						break;

					#ifdef DEBUG
					default:
						fprintf(stderr, "LowLevelProc(): Unhandled Event Type: 0x%X\n", eventType);
						break;
					#endif
				}
			}
			else {
				// Unlock the mutex incase trylock succeeded.
				pthread_mutex_unlock(&hookRunningMutex);
			}
		}
	}

	XRecordFreeData(hook);
}



static void *ThreadProc(void *arg) {
	int *status = (int *) arg;
	*status = NATIVEHOOK_FAILURE;

	// XRecord context for use later.
	context = 0;

	Display *disp_data = XOpenDisplay(NULL);
	if (disp_data != NULL) {
		#ifdef DEBUG
		fprintf(stdout, "ThreadProc(): XOpenDisplay successful.\n");
		#endif

		// Setup XRecord range.
		XRecordClientSpec clients = XRecordAllClients;
		XRecordRange *range = XRecordAllocRange();
		if (range != NULL) {
			#ifdef DEBUG
			fprintf(stdout, "ThreadProc(): XRecordAllocRange successful.\n");
			#endif

			// Create XRecord Context.
			range->device_events.first = KeyPress;
			range->device_events.last = MotionNotify;

			/* Note that the documentation for this function is incorrect,
			 * disp_data should be used!
			 * See: http://www.x.org/releases/X11R7.6/doc/libXtst/recordlib.txt
			 */
			context = XRecordCreateContext(disp_data, 0, &clients, 1, &range, 1);
			if (context != 0) {
				#ifdef DEBUG
				fprintf(stdout, "ThreadProc(): XRecordCreateContext successful.\n");
				#endif

				// Initialize Native Input Functions.
				LoadInputHelper();

				// Allocate memory for the virtual events only once.
				event = (VritualEvent *) malloc(sizeof(VritualEvent));
				keyboardData = (KeyboardEventData *) malloc(sizeof(KeyboardEventData));
				mouseData = (MouseEventData *) malloc(sizeof(MouseEventData));
				mouseWheelData = (MouseWheelEventData *) malloc(sizeof(MouseWheelEventData));

				// Check and make sure we didn't run out of memory.
				if (event != null && keyboardData != null && mouseData != null && mouseWheelData != null) {
					#ifdef XRECORD_ASYNC
					// Allow the thread loop to block.
					running = true;
					#endif

					// Async requires that we loop so that our thread does not return.
					if (XRecordEnableContextAsync(disp_data, context, LowLevelProc, NULL) != 0) {
						// Set the exit status.
						*status = NATIVEHOOK_SUCCESS;

						#ifdef XRECORD_ASYNC
						while (running) {
							XRecordProcessReplies(disp_data);

							// Prevent 100% CPU utilization.
							nanosleep((struct timespec[]) {{0, 100 * 1000000}}, NULL);
						}

						XRecordDisableContext(dspCtrl, context);
						#endif
					}
					else {
						#ifdef DEBUG
						fprintf (stderr, "ThreadProc(): XRecordEnableContext failure!\n");
						#endif

						#ifdef XRECORD_ASYNC
						// Reset the running state.
						running = false;
						#endif

						// Set the exit status.
						*status = NATIVEHOOK_ERROR_X_RECORD_ENABLE_CONTEXT;
					}
				}
				else {
					#ifdef DEBUG
					fprintf(stderr, "ThreadProc(): malloc failure!\n");
					#endif

					// Set the exit status.
					*status = NATIVEHOOK_ERROR_OUT_OF_MEMORY;
				}

				// Free up the context after the run loop terminates.
				XRecordFreeContext(disp_data, context);

				// Cleanup Native Input Functions.
				UnloadInputHelper();

				// Free up memory used for virtual events.
				free(event);
				free(keyboardData);
				free(mouseData);
				free(mouseWheelData);
			}
			else {
				#ifdef DEBUG
				fprintf(stderr, "ThreadProc(): XRecordCreateContext failure!\n");
				#endif

				// Set the exit status.
				*status = NATIVEHOOK_ERROR_X_RECORD_CREATE_CONTEXT;
			}

			// Free the XRecordRange.
			XFree(range);
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "ThreadProc(): XRecordAllocRange failure!\n");
			#endif

			// Set the exit status.
			*status = NATIVEHOOK_ERROR_X_RECORD_ALLOC_RANGE;
		}

		XCloseDisplay(disp_data);
		disp_data = NULL;
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "ThreadProc(): XOpenDisplay failure!\n");
		#endif

		// Set the exit status.
		*status = NATIVEHOOK_ERROR_X_OPEN_DISPLAY;
	}

	#ifdef DEBUG
	fprintf(stdout, "ThreadProc(): complete.\n");
	#endif

	// Make sure we signal that we have passed any exception throwing code.
	pthread_mutex_unlock(&hookControlMutex);

	pthread_exit(status);
}

int StartNativeThread() {
	int status = NATIVEHOOK_FAILURE;

	// We shall use the default pthread attributes: thread is joinable
	// (not detached) and has default (non real-time) scheduling policy.
	//pthread_mutex_init(&hookControlMutex, NULL);

	// Lock the thread control mutex.  This will be unlocked when the
	// thread has finished starting, or when it has fully stopped.
	pthread_mutex_lock(&hookControlMutex);

	// Make sure the native thread is not already running.
	if (IsNativeThreadRunning() != true) {
		// Open the control and data displays.
		dspCtrl = XOpenDisplay(NULL);

		if (dspCtrl != NULL) {
			// Check to make sure XRecord is installed and enabled.
			int major, minor;
			if (XRecordQueryVersion(dspCtrl, &major, &minor) != 0) {
				#ifdef DEBUG
				fprintf(stdout, "ThreadProc(): XRecord version: %d.%d.\n", major, minor);
				#endif

				// Create the thread attribute.
				int policy = 0;
				int priority = 0;

				pthread_attr_init(&hookThreadAttr);
				pthread_attr_getschedpolicy(&hookThreadAttr, &policy);
				priority = sched_get_priority_max(policy);

				if (pthread_create(&hookThreadId, &hookThreadAttr, ThreadProc, malloc(sizeof(int))) == 0) {
					#ifdef DEBUG
					fprintf(stdout, "StartNativeThread(): start successful.\n");
					#endif

					#ifdef DEBUG
					if (pthread_setschedprio(hookThreadId, priority) != 0) {
						fprintf(stderr, "StartNativeThread(): Could not set thread priority %i for thread 0x%X.\n", priority, hookThreadId);
					}
					#else
					pthread_setschedprio(hookThreadId, priority);
					#endif

					// Wait for the thread to unlock the control mutex indicating
					// that it has started or failed.
					if (pthread_mutex_lock(&hookControlMutex) == 0) {
						pthread_mutex_unlock(&hookControlMutex);
					}

					// Handle any possible JNI issue that may have occurred.
					if (IsNativeThreadRunning()) {
						#ifdef DEBUG
						fprintf(stdout, "StartNativeThread(): initialization successful.\n");
						#endif

						status = NATIVEHOOK_SUCCESS;
					}
					else {
						#ifdef DEBUG
						fprintf(stderr, "StartNativeThread(): initialization failure!\n");
						#endif

						// Wait for the thread to die.
						void *thread_status;
						pthread_join(hookThreadId, (void *) &thread_status);
						status = *(int *) thread_status;
						free(thread_status);

						#ifdef DEBUG
						fprintf(stderr, "StartNativeThread(): Thread Result (%i)\n", status);
						#endif
					}
				}
				else {
					#ifdef DEBUG
					fprintf(stderr, "StartNativeThread(): Thread create failure!\n");
					#endif

					status = NATIVEHOOK_ERROR_THREAD_CREATE;
				}
			}
			else {
				#ifdef DEBUG
				fprintf (stderr, "ThreadProc(): XRecord is not currently available!\n");
				#endif

				status = NATIVEHOOK_ERROR_X_RECORD_NOT_FOUND;
			}
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "ThreadProc(): XOpenDisplay failure!\n");
			#endif

			// Close down any open displays.
			if (dspCtrl != NULL) {
				XCloseDisplay(dspCtrl);
				dspCtrl = NULL;
			}

			status = NATIVEHOOK_ERROR_X_OPEN_DISPLAY;
		}
	}

	// Make sure the control mutex is unlocked.
	pthread_mutex_unlock(&hookControlMutex);

	return status;
}

int StopNativeThread() {
	int status = NATIVEHOOK_FAILURE;

	// Lock the thread control mutex.  This will be unlocked when the
	// thread has fully stopped.
	pthread_mutex_lock(&hookControlMutex);

	if (IsNativeThreadRunning() == true) {
		// Try to exit the thread naturally.
		#ifdef XRECORD_ASYNC
		running = false;

		// Wait for the thread to die.
		void *thread_status;
		pthread_join(hookThreadId, &thread_status);
		status = *(int *) thread_status;
		free(thread_status);
		#else
		if (XRecordDisableContext(dspCtrl, context) != 0) {
			// See Bug 42356 for more information.
			// https://bugs.freedesktop.org/show_bug.cgi?id=42356#c4
			XFlush(dspCtrl);
			//XSync(disp_ctrl, true);

			// Wait for the thread to die.
			void *thread_status;
			pthread_join(hookThreadId, &thread_status);
			status = *(int *) thread_status;
			free(thread_status);
		}
		#endif

		// Clean up the thread attribute.
		pthread_attr_destroy(&hookThreadAttr);

		// Close down any open displays.
		XCloseDisplay(dspCtrl);
		dspCtrl = NULL;

		#ifdef DEBUG
		fprintf(stdout, "StopNativeThread(): Thread Result (%i)\n", status);
		#endif
	}

	// Clean up the mutex.
	//pthread_mutex_destroy(&hookControlMutex);

	// Make sure the mutex gets unlocked.
	pthread_mutex_unlock(&hookControlMutex);

	return status;
}

bool IsNativeThreadRunning() {
	bool isRunning = false;

	// Try to aquire a lock on the running mutex.
	if (pthread_mutex_trylock(&hookRunningMutex) == 0) {
		// Lock Successful, thread is not running.
		pthread_mutex_unlock(&hookRunningMutex);
	}
	else {
		isRunning = true;
	}

	#ifdef DEBUG
	fprintf(stdout, "IsNativeThreadRunning(): State (%i)\n", isRunning);
	#endif

	return isRunning;
}
