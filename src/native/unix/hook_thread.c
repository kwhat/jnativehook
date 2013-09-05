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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef USE_XRECORD_ASYNC
#include <time.h>
#endif

#ifdef USE_DEBUG
#include <stdio.h>
#endif

#include <nativehook.h>
#include <pthread.h>
#include <sys/time.h>

#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/record.h>

#include "hook_callback.h"
#include "x_input_helper.h"

// The pointer to the X11 display accessed by the callback.
static Display *disp_ctrl;
static XRecordContext context;

// Thread and hook handles.
#ifdef USE_XRECORD_ASYNC
static volatile bool running;
#endif
pthread_mutex_t hook_running_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t hook_control_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t hook_thread_id;
static pthread_attr_t hook_thread_attr;


static void *hook_thread_proc(void *arg) {
	int *status = (int *) arg;
	*status = NATIVEHOOK_FAILURE;

	// XRecord context for use later.
	context = 0;

	Display *disp_data = XOpenDisplay(NULL);
	if (disp_data != NULL) {
		#ifdef USE_DEBUG
		fprintf(stdout, "hook_thread_proc(): XOpenDisplay successful.\n");
		#endif

		// Make sure the data display is synchronized to prevent late event delivery!
		// See Bug 42356 for more information.
		// https://bugs.freedesktop.org/show_bug.cgi?id=42356#c4
		XSynchronize(disp_data, True);

		// Setup XRecord range.
		XRecordClientSpec clients = XRecordAllClients;
		XRecordRange *range = XRecordAllocRange();
		if (range != NULL) {
			#ifdef USE_DEBUG
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
				#ifdef USE_DEBUG
				fprintf(stdout, "hook_thread_proc(): XRecordCreateContext successful.\n");
				#endif

				// Initialize Native Input Functions.
				load_input_helper();

				#ifdef USE_XRECORD_ASYNC
				// Allow the thread loop to block.
				running = true;

				// Async requires that we loop so that our thread does not return.
				if (XRecordEnableContextAsync(disp_data, context, hook_event_proc, NULL) != 0) {
					// Set the exit status.
					*status = NATIVEHOOK_SUCCESS;

					while (running) {
						XRecordProcessReplies(disp_data);

						// Prevent 100% CPU utilization.
						#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE  >= 199309L
						nanosleep((struct timespec[]) {{0, 100 * 1000000}}, NULL);
						#else
						#pragma message "You should define _POSIX_C_SOURCE  >= 199309L with USE_XRECORD_ASYNC to prevent 100% CPU utilization!"
						#endif
					}
				}
				#else
				// Sync blocks until XRecordDisableContext() is called.
				if (XRecordEnableContext(disp_data, context, hook_event_proc, NULL) != 0) {
					// Set the exit status.
					*status = NATIVEHOOK_SUCCESS;
				}
				#endif
				else {
					#ifdef USE_DEBUG
					fprintf (stderr, "hook_thread_proc(): XRecordEnableContext failure!\n");
					#endif

					#ifdef USE_XRECORD_ASYNC
					// Reset the running state.
					running = false;
					#endif

					// Set the exit status.
					*status = NATIVEHOOK_ERROR_X_RECORD_ENABLE_CONTEXT;
				}

				// Free up the context after the run loop terminates.
				XRecordFreeContext(disp_data, context);

				// Cleanup Native Input Functions.
				unload_input_helper();
			}
			else {
				#ifdef USE_DEBUG
				fprintf(stderr, "hook_thread_proc(): XRecordCreateContext failure!\n");
				#endif

				// Set the exit status.
				*status = NATIVEHOOK_ERROR_X_RECORD_CREATE_CONTEXT;
			}

			// Free the XRecordRange.
			XFree(range);
		}
		else {
			#ifdef USE_DEBUG
			fprintf(stderr, "hook_thread_proc(): XRecordAllocRange failure!\n");
			#endif

			// Set the exit status.
			*status = NATIVEHOOK_ERROR_X_RECORD_ALLOC_RANGE;
		}

		XCloseDisplay(disp_data);
		disp_data = NULL;
	}
	else {
		#ifdef USE_DEBUG
		fprintf(stderr, "hook_thread_proc(): XOpenDisplay failure!\n");
		#endif

		// Set the exit status.
		*status = NATIVEHOOK_ERROR_X_OPEN_DISPLAY;
	}

	#ifdef USE_DEBUG
	fprintf(stdout, "hook_thread_proc(): complete.\n");
	#endif

	// Make sure we signal that we have passed any exception throwing code.
	pthread_mutex_unlock(&hook_control_mutex);

	pthread_exit(status);
}

NATIVEHOOK_API int hook_enable() {
	int status = NATIVEHOOK_FAILURE;

	// We shall use the default pthread attributes: thread is joinable
	// (not detached) and has default (non real-time) scheduling policy.
	//pthread_mutex_init(&hook_control_mutex, NULL);

	// Lock the thread control mutex.  This will be unlocked when the
	// thread has finished starting, or when it has fully stopped.
	pthread_mutex_lock(&hook_control_mutex);

	// Make sure the native thread is not already running.
	if (hook_is_enabled() != true) {
		// Open the control and data displays.
		disp_ctrl = XOpenDisplay(NULL);

		if (disp_ctrl != NULL) {
			// Check to make sure XRecord is installed and enabled.
			int major, minor;
			if (XRecordQueryVersion(disp_ctrl, &major, &minor) != 0) {
				#ifdef USE_DEBUG
				fprintf(stdout, "hook_thread_proc(): XRecord version: %d.%d.\n", major, minor);
				#endif

				// Create the thread attribute.
				int policy = 0;
				int priority = 0;

				pthread_attr_init(&hook_thread_attr);
				pthread_attr_getschedpolicy(&hook_thread_attr, &policy);
				priority = sched_get_priority_max(policy);

				if (pthread_create(&hook_thread_id, &hook_thread_attr, hook_thread_proc, malloc(sizeof(int))) == 0) {
					#ifdef USE_DEBUG
					fprintf(stdout, "enable_hook(): start successful.\n");
					#endif

					#ifdef USE_DEBUG
					if (pthread_setschedprio(hook_thread_id, priority) != 0) {
						fprintf(stderr, "enable_hook(): Could not set thread priority %i for thread 0x%X.\n", priority, (unsigned int) hook_thread_id);
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
					if (hook_is_enabled()) {
						#ifdef USE_DEBUG
						fprintf(stdout, "enable_hook(): initialization successful.\n");
						#endif

						status = NATIVEHOOK_SUCCESS;
					}
					else {
						#ifdef USE_DEBUG
						fprintf(stderr, "enable_hook(): initialization failure!\n");
						#endif

						// Wait for the thread to die.
						void *thread_status;
						pthread_join(hook_thread_id, (void *) &thread_status);
						status = *(int *) thread_status;
						free(thread_status);

						#ifdef USE_DEBUG
						fprintf(stderr, "enable_hook(): Thread Result (%i)\n", status);
						#endif
					}
				}
				else {
					#ifdef USE_DEBUG
					fprintf(stderr, "enable_hook(): Thread create failure!\n");
					#endif

					status = NATIVEHOOK_ERROR_THREAD_CREATE;
				}
			}
			else {
				#ifdef USE_DEBUG
				fprintf (stderr, "hook_thread_proc(): XRecord is not currently available!\n");
				#endif

				status = NATIVEHOOK_ERROR_X_RECORD_NOT_FOUND;
			}
		}
		else {
			#ifdef USE_DEBUG
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

NATIVEHOOK_API int hook_disable() {
	int status = NATIVEHOOK_FAILURE;

	// Lock the thread control mutex.  This will be unlocked when the
	// thread has fully stopped.
	pthread_mutex_lock(&hook_control_mutex);

	if (hook_is_enabled() == true) {
		// Try to exit the thread naturally.
		if (XRecordDisableContext(disp_ctrl, context) != 0) {
			#ifdef USE_XRECORD_ASYNC
			running = false;
			#endif

			// See Bug 42356 for more information.
			// https://bugs.freedesktop.org/show_bug.cgi?id=42356#c4
			XFlush(disp_ctrl);
			//XSync(disp_ctrl, True);

			// Wait for the thread to die.
			void *thread_status;
			pthread_join(hook_thread_id, &thread_status);
			status = *(int *) thread_status;
			free(thread_status);

			// Clean up the thread attribute.
			pthread_attr_destroy(&hook_thread_attr);

			// Close down any open displays.
			XCloseDisplay(disp_ctrl);
			disp_ctrl = NULL;
		}

		#ifdef USE_DEBUG
		fprintf(stdout, "hook_disable(): Thread Result (%i)\n", status);
		#endif
	}

	// Clean up the mutex.
	//pthread_mutex_destroy(&hook_control_mutex);

	// Make sure the mutex gets unlocked.
	pthread_mutex_unlock(&hook_control_mutex);

	return status;
}

NATIVEHOOK_API bool hook_is_enabled() {
	bool is_running = false;

	// Try to aquire a lock on the running mutex.
	if (pthread_mutex_trylock(&hook_running_mutex) == 0) {
		// Lock Successful, thread is not running.
		pthread_mutex_unlock(&hook_running_mutex);
	}
	else {
		is_running = true;
	}

	#ifdef USE_DEBUG
	fprintf(stdout, "hook_is_enabled(): State (%i)\n", is_running);
	#endif

	return is_running;
}
