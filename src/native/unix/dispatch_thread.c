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

#include <pthread.h>
#include <sys/time.h>


static pthread_cond_t dispatch_wait_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t dispatch_running_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t dispatch_control_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t dispatch_thread_id;


struct event_cb {
    event_cb_t cb;
    void *data;
};

// Callback function typedef
typedef void (*event_cb_t)(const struct event *evt, void *userdata);

typedef struct {
	event_cb_t callback;
	queue_item *next;
} queue_item;


int event_cb_register(event_cb_t cb, void *userdata) {
	
}


static void *dispatch_thread_proc(void *arg) {
	int *status = (int *) arg;
	*status = RETURN_FAILURE;

	#ifdef DEBUG
	fprintf(stdout, "dispatch_thread_proc(): XRecordCreateContext successful.\n");
	#endif

	// Initialize Native Input Functions.
	LoadInputHelper();

	#ifdef XRECORD_ASYNC
	// Allow the thread loop to block.
	running = true;

	// Async requires that we loop so that our thread does not return.
	if (XRecordEnableContextAsync(disp_data, context, LowLevelProc, NULL) != 0) {
		// Set the exit status.
		*status = RETURN_SUCCESS;

		while (running) {
			XRecordProcessReplies(disp_data);
		}
		XRecordDisableContext(disp_ctrl, context);
	}
	else {
		#ifdef DEBUG
		fprintf (stderr, "dispatch_thread_proc(): XRecordEnableContext failure!\n");
		#endif

		// Reset the running state.
		running = false;

		thread_ex.class = NATIVE_HOOK_EXCEPTION;
		thread_ex.message = "Failed to enable XRecord context";
	}
	#else
	// Sync blocks until XRecordDisableContext() is called.
	if (XRecordEnableContext(disp_data, context, LowLevelProc, NULL) != 0) {
		// Set the exit status.
		*status = RETURN_SUCCESS;
	}
	else {
		#ifdef DEBUG
		fprintf (stderr, "dispatch_thread_proc(): XRecordEnableContext failure!\n");
		#endif

		thread_ex.class = NATIVE_HOOK_EXCEPTION;
		thread_ex.message = "Failed to enable XRecord context";
	}
	#endif

	// Cleanup Native Input Functions.
	UnloadInputHelper();

	#ifdef DEBUG
	fprintf(stdout, "dispatch_thread_proc(): complete.\n");
	#endif

	// Make sure we signal that we have passed any exception throwing code.
	pthread_mutex_unlock(&dispatch_control_mutex);

	pthread_exit(status);
}

int start_dispatch_thread() {
	int status = RETURN_FAILURE;

	// We shall use the default pthread attributes: thread is joinable
	// (not detached) and has default (non real-time) scheduling policy.
	//pthread_mutex_init(&hookControlMutex, NULL);

	// Lock the thread control mutex.  This will be unlocked when the
	// thread has finished starting, or when it has fully stopped.
	pthread_mutex_lock(&dispatch_control_mutex);

	// Make sure the native thread is not already running.
	if (is_dispatch_thread_running() != true) {
		if (pthread_create(&dispatch_thread_id, NULL, dispatch_thread_proc, malloc(sizeof(int))) == 0) {
			#ifdef DEBUG
			fprintf(stdout, "start_dispatch_thread(): start successful.\n");
			#endif

			// Wait for the thread to unlock the control mutex indicating
			// that it has started or failed.
			if (pthread_mutex_lock(&dispatch_control_mutex) == 0) {
				pthread_mutex_unlock(&dispatch_control_mutex);
			}

			// Handle any possible JNI issue that may have occurred.
			if (is_dispatch_thread_running()) {
				#ifdef DEBUG
				fprintf(stdout, "start_dispatch_thread(): initialization successful.\n");
				#endif

				status = RETURN_SUCCESS;
			}
			else {
				#ifdef DEBUG
				fprintf(stderr, "start_dispatch_thread(): initialization failure!\n");
				#endif

				// Wait for the thread to die.
				void *thread_status;
				pthread_join(dispatch_thread_id, (void *) &thread_status);
				status = *(int *) thread_status;
				free(thread_status);

				#ifdef DEBUG
				fprintf(stderr, "start_dispatch_thread(): Thread Result (%i)\n", status);
				#endif

				if (thread_ex.class != NULL && thread_ex.message != NULL)  {
					ThrowException(thread_ex.class, thread_ex.message);
				}
			}
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "start_dispatch_thread(): start failure!\n");
			#endif

			ThrowException(NATIVE_HOOK_EXCEPTION, "Native thread start failure");
		}
	}

	// Make sure the control mutex is unlocked.
	pthread_mutex_unlock(&dispatch_control_mutex);

	return status;
}

int stop_dispatch_thread() {
	int status = RETURN_FAILURE;

	// Lock the thread control mutex.  This will be unlocked when the
	// thread has fully stopped.
	pthread_mutex_lock(&dispatch_control_mutex);

	if (is_dispatch_thread_running() == true) {
		// Try to exit the thread naturally.
		#ifdef XRECORD_ASYNC
		running = false;

		// Wait for the thread to die.
		void *thread_status;
		pthread_join(dispatch_thread_id, &thread_status);
		status = *(int *) thread_status;
		free(thread_status);
		#else
		if (XRecordDisableContext(disp_ctrl, context) != 0) {
			XSync(disp_ctrl, false);

			// Wait for the thread to die.
			void *thread_status;
			pthread_join(dispatch_thread_id, &thread_status);
			status = *(int *) thread_status;
			free(thread_status);
		}
		#endif

		#ifdef DEBUG
		fprintf(stdout, "stop_dispatch_thread(): Thread Result (%i)\n", status);
		#endif
	}

	// Clean up the mutex.
	//pthread_mutex_destroy(&hookControlMutex);

	// Make sure the mutex gets unlocked.
	pthread_mutex_unlock(&dispatch_control_mutex);

	return status;
}

bool is_dispatch_thread_running() {
	bool is_running = false;

	// Try to aquire a lock on the running mutex.
	if (pthread_mutex_trylock(&dispatch_running_mutex) == 0) {
		// Lock Successful, thread is not running.
		pthread_mutex_unlock(&dispatch_running_mutex);
	}
	else {
		is_running = true;
	}

	#ifdef DEBUG
	fprintf(stdout, "is_dispatch_thread_running(): State (%i)\n", is_running);
	#endif

	return is_running;
}
