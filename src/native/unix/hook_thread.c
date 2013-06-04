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
static unsigned short click_count = 0;
static long click_time = 0;
static bool mouse_dragged = false;

// The pointer to the X11 display accessed by the callback.
static Display *disp_ctrl;
static XRecordContext context;

// Thread and hook handles.
#ifdef XRECORD_ASYNC
static volatile bool running;
#endif
static pthread_mutex_t hook_running_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t hook_control_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t hook_thread_id;
static pthread_attr_t hook_thread_attr;

// Virtual event pointers
static VritualEvent *event = null;
static KeyboardEventData *keyboard_data = null;
static MouseEventData *mouse_data = null;
static MouseWheelEventData *mouse_wheel_data = null;

// Event dispatch callback
static void (*current_dispatch_proc)(VirtualEvent * const) = null;

void hook_set_dispatch_proc(void (*dispatch_proc)(VirtualEvent * const)) {
	current_dispatch_proc = dispatch_proc;
}

inline static int dispatch_event(VirtualEvent * const event) {
	int status = NATIVEHOOK_FAILURE;

	if (current_dispatch_proc != null) {
		status = current_dispatch_proc(event);
	}

	return status;
}

static void hook_event_proc(XPointer UNUSED(pointer), XRecordInterceptData *hook) {
	if (hook->category == XRecordStartOfData) {
		pthread_mutex_lock(&hook_running_mutex);
		pthread_mutex_unlock(&hook_control_mutex);
	}
	else if (hook->category == XRecordEndOfData) {
		//pthread_mutex_lock(&hook_control_mutex);
		pthread_mutex_unlock(&hook_running_mutex);
	}
	else if (hook->category == XRecordFromServer || hook->category == XRecordFromClient) {
		if (disp_ctrl != NULL) {
			// Check and make sure the thread is stull running to avoid the
			// potential crash associated with late event arrival.  This code is
			// guaranteed to run after all thread start.
			if (pthread_mutex_trylock(&hook_running_mutex) != 0) {
				// Get XRecord data.
				XRecordDatum *data = (XRecordDatum *) hook->data;

				// Native event data.
				struct timeval  timeVal;
				gettimeofday(&timeVal, NULL);
				unsigned long int event_time = (timeVal.tv_sec * 1000) + (timeVal.tv_usec / 1000);
				
				// Use more readable variables.
				int event_type = data->type;
				BYTE event_code = data->event.u.u.detail;
				int event_mask = data->event.u.keyButtonPointer.state;
				int event_x = data->event.u.keyButtonPointer.rootX;
				int event_y = data->event.u.keyButtonPointer.rootY;

				switch (event_type) {
					case KeyPress:
						#ifdef DEBUG
						fprintf(stdout, "hook_event_proc(): Key pressed. (%i)\n", event_code);
						#endif

						KeySym keysym = KeyCodeToKeySym(event_code, event_mask);
						KeyCode keycode = ConvertToVirtualKey(keysym);
						unsigned int modifiers = ConvertToVirtualMask(event_mask);

						// Fire key pressed event.
						event->type = EVENT_KEY_PRESSED;
						event->time = event_time
						event->mask = modifiers;
						event->data = keyboard_data;

						keyboard_data->keycode = keycode;
						keyboard_data->rawcode = event_code;
						keyboard_data->keychar = CHAR_UNDEFINED;

						dispatch_event(event);

						// Check to make sure the key is printable.
						wchar_t keychar = KeySymToUnicode(keySym);
						if (keychar != 0x0000) {
							// Fire key typed event.
							event->type = EVENT_KEY_TYPED;

							keyboard_data->keycode = VC_UNDEFINED;
							keyboard_data->keychar = keychar;

							dispatch_event(event);
						}
						break;

					case KeyRelease:
						#ifdef DEBUG
						fprintf(stdout, "hook_event_proc(): Key released. (%i)\n", event_code);
						#endif

						KeySym keysym = KeyCodeToKeySym(event_code, event_mask);
						KeyCode keycode = ConvertToVirtualKey(keysym);
						unsigned int modifiers = ConvertToVirtualMask(event_mask);

						// Fire key released event.
						event->type = EVENT_KEY_RELEASED;
						event->time = event_time
						event->mask = modifiers;
						event->data = keyboard_data;

						keyboard_data->keycode = keyCode;
						keyboard_data->rawcode = event_code;
						keyboard_data->keychar = CHAR_UNDEFINED;

						dispatch_event(event);
						break;

					case ButtonPress:
						#ifdef DEBUG
						fprintf(stdout, "hook_event_proc(): Button pressed. (%i)\n", event_code);
						#endif

						// Track the number of clicks.
						if ((long) (event_time - click_time) <= GetMultiClickTime()) {
							click_count++;
						}
						else {
							click_count = 1;
						}
						click_time = event_time;

						// Convert native modifiers to java modifiers.
						unsigned int modifiers = ConvertToVirtualMask(event_mask);

						/* This information is all static for X11, its up to the WM to
						* decide how to interpret the wheel events.
						*/
						// TODO Should use constants and a lookup table for button codes.
						if (event_code > 0 && (event_code <= 3 || event_code == 8 || event_code == 9)) {
							unsigned int button = ConvertToVirtualButton(event_code);

							// Fire mouse pressed event.
							event->type = EVENT_MOUSE_PRESSED;
							event->time = event_time
							event->mask = modifiers;
							event->data = mouse_data;

							mouse_data->button = button;
							mouse_data->clicks = click_count;
							mouse_data->x = event_x;
							mouse_data->y = event_y;

							dispatch_event(event);
						}
						else if (event_code == WheelUp || event_code == WheelDown) {
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
							int scroll_type = WHEEL_UNIT_SCROLL;

							/* Some scroll wheel properties are available via the new XInput2 (XI2)
							* extention.  Unfortunately the extention is not available on my
							* development platform at this time.  For the time being we will just
							* use the Windows default value of 3.
							*/
							int scroll_amount = 3;

							// Wheel Rotated Down and Towards.
							int wheel_rotation = 1; // event_code == WheelDown
							if (event_code == WheelUp) {
								// Wheel Rotated Up and Away.
								wheel_rotation = -1;
							}

							// Fire mouse wheel event.
							event->type = EVENT_MOUSE_WHEEL;
							event->time = event_time
							event->mask = modifiers;
							event->data = mouse_wheel_data;

							mouse_wheel_data->type = scroll_type;
							mouse_wheel_data->amount = scroll_amount;
							mouse_wheel_data->rotation = wheel_rotation;

							dispatch_event(event);
						}
						break;

					case ButtonRelease:
						#ifdef DEBUG
						fprintf(stdout, "hook_event_proc(): Button released. (%i)\n", event_code);
						#endif

						// TODO Should use constants for button codes.
						if (event_code > 0 && (event_code <= 3 || event_code == 8 || event_code == 9)) {
							// Handle button release events.
							unsigned int button = ConvertToVirtualButton(event_code);
							unsigned int modifiers = ConvertToVirtualMask(event_mask);

							// Fire mouse released event.
							event->type = EVENT_MOUSE_RELEASED;
							event->time = event_time
							event->mask = modifiers;
							event->data = mouse_data;

							mouse_data->button = button;
							mouse_data->clicks = click_count;
							mouse_data->x = event_x;
							mouse_data->y = eventY;

							dispatch_event(event);

							if (mouse_dragged != true) {
								// Fire mouse clicked event.
								event->type = EVENT_MOUSE_CLICKED;
								event->time = event_time
								event->mask = modifiers;
								event->data = mouse_data;

								mouse_data->button = button;
								mouse_data->clicks = click_count;
								mouse_data->x = event_x;
								mouse_data->y = eventY;

								dispatch_event(event);
							}
						}
						break;

					case MotionNotify:
						#ifdef DEBUG
						fprintf(stdout, "hook_event_proc(): Motion Notified. (%i, %i)\n", event_x, eventY);
						#endif

						// Reset the click count.
						if (click_count != 0 && (long) (event_time - click_time) > GetMultiClickTime()) {
							click_count = 0;
						}
						unsigned int modifiers = ConvertToVirtualMask(event_mask);

						// Check the upper half of virtual modifiers for non zero 
						// values and set the mouse dragged flag.
						mouse_dragged = modifiers >> 4 > 0;
						if (mouse_dragged) {
							// Create Mouse Dragged event.
							event->type = EVENT_MOUSE_DRAGGED;
							event->time = event_time
							event->mask = modifiers;
							event->data = mouse_data;

							mouse_data->button = MOUSE_NOBUTTON;
							mouse_data->clicks = click_count;
							mouse_data->x = event_x;
							mouse_data->y = event_y;
						}
						else {
							// Create a Mouse Moved event.
							event->type = EVENT_MOUSE_MOVED;
							event->time = event_time
							event->mask = modifiers;
							event->data = mouse_data;

							mouse_data->button = MOUSE_NOBUTTON;
							mouse_data->clicks = click_count;
							mouse_data->x = event_x;
							mouse_data->y = event_y;
						}

						// Fire mouse moved event.
						dispatch_event(event);
						break;

					#ifdef DEBUG
					default:
						fprintf(stderr, "hook_event_proc(): Unhandled Event Type: 0x%X\n", event_type);
						break;
					#endif
				}
			}
			else {
				// Unlock the mutex incase trylock succeeded.
				pthread_mutex_unlock(&hook_running_mutex);
			}
		}
	}

	XRecordFreeData(hook);
}



static void *hook_thread_proc(void *arg) {
	int *status = (int *) arg;
	*status = NATIVEHOOK_FAILURE;

	// XRecord context for use later.
	context = 0;

	Display *disp_data = XOpenDisplay(NULL);
	if (disp_data != NULL) {
		#ifdef DEBUG
		fprintf(stdout, "hook_thread_proc(): XOpenDisplay successful.\n");
		#endif

		// Setup XRecord range.
		XRecordClientSpec clients = XRecordAllClients;
		XRecordRange *range = XRecordAllocRange();
		if (range != NULL) {
			#ifdef DEBUG
			fprintf(stdout, "hook_thread_proc(): XRecordAllocRange successful.\n");
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
				fprintf(stdout, "hook_thread_proc(): XRecordCreateContext successful.\n");
				#endif

				// Initialize Native Input Functions.
				LoadInputHelper();

				// Allocate memory for the virtual events only once.
				event = (VritualEvent *) malloc(sizeof(VritualEvent));
				keyboard_data = (KeyboardEventData *) malloc(sizeof(KeyboardEventData));
				mouse_data = (MouseEventData *) malloc(sizeof(MouseEventData));
				mouse_wheel_data = (MouseWheelEventData *) malloc(sizeof(MouseWheelEventData));

				// Check and make sure we didn't run out of memory.
				if (event != null && keyboard_data != null && mouse_data != null && mouse_wheel_data != null) {
					#ifdef XRECORD_ASYNC
					// Allow the thread loop to block.
					running = true;
					#endif

					// Async requires that we loop so that our thread does not return.
					if (XRecordEnableContextAsync(disp_data, context, hook_event_proc, NULL) != 0) {
						// Set the exit status.
						*status = NATIVEHOOK_SUCCESS;

						#ifdef XRECORD_ASYNC
						while (running) {
							XRecordProcessReplies(disp_data);

							// Prevent 100% CPU utilization.
							nanosleep((struct timespec[]) {{0, 100 * 1000000}}, NULL);
						}

						XRecordDisableContext(disp_ctrl, context);
						#endif
					}
					else {
						#ifdef DEBUG
						fprintf (stderr, "hook_thread_proc(): XRecordEnableContext failure!\n");
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
					fprintf(stderr, "hook_thread_proc(): malloc failure!\n");
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
				free(keyboard_data);
				free(mouse_data);
				free(mouse_wheel_data);
			}
			else {
				#ifdef DEBUG
				fprintf(stderr, "hook_thread_proc(): XRecordCreateContext failure!\n");
				#endif

				// Set the exit status.
				*status = NATIVEHOOK_ERROR_X_RECORD_CREATE_CONTEXT;
			}

			// Free the XRecordRange.
			XFree(range);
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "hook_thread_proc(): XRecordAllocRange failure!\n");
			#endif

			// Set the exit status.
			*status = NATIVEHOOK_ERROR_X_RECORD_ALLOC_RANGE;
		}

		XCloseDisplay(disp_data);
		disp_data = NULL;
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "hook_thread_proc(): XOpenDisplay failure!\n");
		#endif

		// Set the exit status.
		*status = NATIVEHOOK_ERROR_X_OPEN_DISPLAY;
	}

	#ifdef DEBUG
	fprintf(stdout, "hook_thread_proc(): complete.\n");
	#endif

	// Make sure we signal that we have passed any exception throwing code.
	pthread_mutex_unlock(&hook_control_mutex);

	pthread_exit(status);
}

int hook_enable() {
	int status = NATIVEHOOK_FAILURE;

	// We shall use the default pthread attributes: thread is joinable
	// (not detached) and has default (non real-time) scheduling policy.
	//pthread_mutex_init(&hook_control_mutex, NULL);

	// Lock the thread control mutex.  This will be unlocked when the
	// thread has finished starting, or when it has fully stopped.
	pthread_mutex_lock(&hook_control_mutex);

	// Make sure the native thread is not already running.
	if (hook_is_enable() != true) {
		// Open the control and data displays.
		disp_ctrl = XOpenDisplay(NULL);

		if (disp_ctrl != NULL) {
			// Check to make sure XRecord is installed and enabled.
			int major, minor;
			if (XRecordQueryVersion(disp_ctrl, &major, &minor) != 0) {
				#ifdef DEBUG
				fprintf(stdout, "hook_thread_proc(): XRecord version: %d.%d.\n", major, minor);
				#endif

				// Create the thread attribute.
				int policy = 0;
				int priority = 0;

				pthread_attr_init(&hook_thread_attr);
				pthread_attr_getschedpolicy(&hook_thread_attr, &policy);
				priority = sched_get_priority_max(policy);

				if (pthread_create(&hook_thread_id, &hook_thread_attr, hook_thread_proc, malloc(sizeof(int))) == 0) {
					#ifdef DEBUG
					fprintf(stdout, "enable_hook(): start successful.\n");
					#endif

					#ifdef DEBUG
					if (pthread_setschedprio(hook_thread_id, priority) != 0) {
						fprintf(stderr, "enable_hook(): Could not set thread priority %i for thread 0x%X.\n", priority, hook_thread_id);
					}
					#else
					pthread_setschedprio(hook_thread_id, priority);
					#endif

					// Wait for the thread to unlock the control mutex indicating
					// that it has started or failed.
					if (pthread_mutex_lock(&hook_control_mutex) == 0) {
						pthread_mutex_unlock(&hook_control_mutex);
					}

					// Handle any possible JNI issue that may have occurred.
					if (hook_is_enable()) {
						#ifdef DEBUG
						fprintf(stdout, "enable_hook(): initialization successful.\n");
						#endif

						status = NATIVEHOOK_SUCCESS;
					}
					else {
						#ifdef DEBUG
						fprintf(stderr, "enable_hook(): initialization failure!\n");
						#endif

						// Wait for the thread to die.
						void *thread_status;
						pthread_join(hook_thread_id, (void *) &thread_status);
						status = *(int *) thread_status;
						free(thread_status);

						#ifdef DEBUG
						fprintf(stderr, "enable_hook(): Thread Result (%i)\n", status);
						#endif
					}
				}
				else {
					#ifdef DEBUG
					fprintf(stderr, "enable_hook(): Thread create failure!\n");
					#endif

					status = NATIVEHOOK_ERROR_THREAD_CREATE;
				}
			}
			else {
				#ifdef DEBUG
				fprintf (stderr, "hook_thread_proc(): XRecord is not currently available!\n");
				#endif

				status = NATIVEHOOK_ERROR_X_RECORD_NOT_FOUND;
			}
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "hook_thread_proc(): XOpenDisplay failure!\n");
			#endif

			// Close down any open displays.
			if (disp_ctrl != NULL) {
				XCloseDisplay(disp_ctrl);
				disp_ctrl = NULL;
			}

			status = NATIVEHOOK_ERROR_X_OPEN_DISPLAY;
		}
	}

	// Make sure the control mutex is unlocked.
	pthread_mutex_unlock(&hook_control_mutex);

	return status;
}

int hook_disable() {
	int status = NATIVEHOOK_FAILURE;

	// Lock the thread control mutex.  This will be unlocked when the
	// thread has fully stopped.
	pthread_mutex_lock(&hook_control_mutex);

	if (hook_is_enable() == true) {
		// Try to exit the thread naturally.
		#ifdef XRECORD_ASYNC
		running = false;

		// Wait for the thread to die.
		void *thread_status;
		pthread_join(hook_thread_id, &thread_status);
		status = *(int *) thread_status;
		free(thread_status);
		#else
		if (XRecordDisableContext(disp_ctrl, context) != 0) {
			// See Bug 42356 for more information.
			// https://bugs.freedesktop.org/show_bug.cgi?id=42356#c4
			XFlush(disp_ctrl);
			//XSync(disp_ctrl, true);

			// Wait for the thread to die.
			void *thread_status;
			pthread_join(hook_thread_id, &thread_status);
			status = *(int *) thread_status;
			free(thread_status);
		}
		#endif

		// Clean up the thread attribute.
		pthread_attr_destroy(&hook_thread_attr);

		// Close down any open displays.
		XCloseDisplay(disp_ctrl);
		disp_ctrl = NULL;

		#ifdef DEBUG
		fprintf(stdout, "disable_hook(): Thread Result (%i)\n", status);
		#endif
	}

	// Clean up the mutex.
	//pthread_mutex_destroy(&hook_control_mutex);

	// Make sure the mutex gets unlocked.
	pthread_mutex_unlock(&hook_control_mutex);

	return status;
}

bool hook_is_enable() {
	bool is_running = false;

	// Try to aquire a lock on the running mutex.
	if (pthread_mutex_trylock(&hook_running_mutex) == 0) {
		// Lock Successful, thread is not running.
		pthread_mutex_unlock(&hook_running_mutex);
	}
	else {
		is_running = true;
	}

	#ifdef DEBUG
	fprintf(stdout, "is_hook_enable(): State (%i)\n", is_running);
	#endif

	return is_running;
}
