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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/extensions/record.h>

static Display * disp_hook;
static Display * disp_data;
static XRecordContext context;

#ifdef ASYNC
bool running = true;
#endif
static pthread_mutex_t hookMutexHandle = PTHREAD_MUTEX_INITIALIZER;
static pthread_t hookThreadId = 0;

//struct taken from libxnee
typedef union {
	unsigned char		type;
	xEvent				event;
	xResourceReq		req;
	xGenericReply		reply;
	xError				error;
	xConnSetupPrefix	setup;
} XRecordDatum;

void callback(XPointer pointer, XRecordInterceptData * hook) {
	//Make sure that our data come from a legitimate source.
	if (hook->category != XRecordFromServer && hook->category != XRecordFromClient) {
		XRecordFreeData(hook);
		return;
	}

	//Convert the hook data to an XRecordDatum.
	XRecordDatum * data = (XRecordDatum *) hook->data;

	switch(data->type) {
		case KeyPress:
			//Stop looping if the escape key is pressed.
			if (XKeycodeToKeysym(disp_data, data->event.u.u.detail, 0) == XK_Escape) {
				//Lock the thread.
				pthread_mutex_lock(&hookMutexHandle);

				#ifdef ASYNC
					running = false;
				#else
					//This has NO effect.
					//XRecordDisableContext(disp_data, context);

					//This stops the recording but fails to cause a return.
					XRecordDisableContext(disp_hook, context);
				#endif

				//Must unlock to allow the thread to finish cleaning up.
				pthread_mutex_unlock(&hookMutexHandle);
			}

			printf("Key Press - %i\n", (int) XKeycodeToKeysym(disp_data, data->event.u.u.detail, 0));
		break;

		case KeyRelease:
			printf("Key Release - %i\n", (int) XKeycodeToKeysym(disp_data, data->event.u.u.detail, 0));
		break;

		case ButtonPress:
			printf("Button Press - %i\n", data->event.u.u.detail);
		break;

		case ButtonRelease:
			printf("Button Release - %i\n", data->event.u.u.detail);
		break;

		case MotionNotify:
			printf("Motion Notify - %i, %i\n", data->event.u.keyButtonPointer.rootX, data->event.u.keyButtonPointer.rootY);
		break;

		default:
			printf("Unknown Event - %x\n", data->type);
		break;
	}

	XRecordFreeData(hook);
}


static void * ThreadProc() {
	pthread_mutex_lock(&hookMutexHandle);
	int status = EXIT_FAILURE;


	//Setup XRecord range
	XRecordClientSpec clients = XRecordAllClients;
	XRecordRange * range = XRecordAllocRange();
	if (range != NULL) {
		printf("Native: XRecordAllocRange successful.\n");

		//Create XRecord Context
		range->device_events.first = KeyPress;
		range->device_events.last = MotionNotify;
		context = XRecordCreateContext(disp_hook, 0, &clients, 1, &range, 1);
		XFree(range);
	}
	else {
		printf("Native: XRecordAllocRange failure!\n");
	}


	if (context != 0) {
		//Set the exit status.
		status = EXIT_SUCCESS;

		printf("Native: XRecordCreateContext successful.\n");

		pthread_mutex_unlock(&hookMutexHandle);

		//Start XRecord process
		#ifdef ASYNC
			XRecordEnableContextAsync(disp_hook, context, callback, NULL);
			while(running) {
				XRecordProcessReplies(disp_hook);
			}
			//This causes a segmentation fault
			//XRecordDisableContext(disp_hook, context);
		#else
			XRecordEnableContext(disp_hook, context, callback, NULL);
		#endif

		//Lock back up until we are done processing the exit.
		pthread_mutex_lock(&hookMutexHandle);
	}
	else {
		printf("Native: XRecordCreateContext failure!\n");
	}

	fprintf(stdout, "ThreadProc() has completed.\n");

	//Make sure we signal that we have passed any exception throwing code.
	pthread_mutex_unlock(&hookMutexHandle);

	pthread_exit((void **) status);
}


int main(int argc, const char * argv[]) {
	//Tell X Threads are OK
	XInitThreads();

	//Lock the mutex handle for the thread hook.
	pthread_mutex_init(&hookMutexHandle, NULL);

	//Try to attach to the default X11 display.
	disp_hook = XOpenDisplay(NULL);
	disp_data = XOpenDisplay(NULL);
	if(disp_hook == NULL || disp_data == NULL) {
		printf("Error: Could not open display!\n");
		return EXIT_FAILURE;
	}


	//Check to make sure XRecord is installed and enabled.
	int major, minor;
	if (XRecordQueryVersion(disp_hook, &major, &minor) != false) {
		printf("XRecord version: %d.%d\n", major, minor);
	}
	else {
		printf("XRecord is not currently available!\n");
	}



	/*
	//Setup XRecord range
	XRecordClientSpec clients = XRecordAllClients;
	XRecordRange * range = XRecordAllocRange();
	if (range == NULL) {
		printf("Error: Could not allocate XRecordRange!");
		return EXIT_FAILURE;
	}

	//Create XRecord Context
	range->device_events.first = KeyPress;
	range->device_events.last = MotionNotify;
	context = XRecordCreateContext(disp_hook, 0, &clients, 1, &range, 1);
	XFree(range);
	if (context == 0) {
		printf("Error: Could not create XRecordContext!");
		return EXIT_FAILURE;
	}
	 */


	#ifndef XRECORD_SYNC
		//Try to exit the thread naturally.
		running = true;
	#endif

	/*
	 *
	 * We shall use the default pthread attributes: thread is joinable
	 * (not detached) and has default (non real-time) scheduling policy.
	 */
	if (pthread_create(&hookThreadId, NULL, ThreadProc, NULL) == 0) {
		printf("Native: ThreadProc() start successful.\n");

		//Wait for the thread to start up.
		if (pthread_mutex_lock(&hookMutexHandle) == 0) {
			pthread_mutex_unlock(&hookMutexHandle);
		}

		printf("Native: ThreadProc() start done.\n");
	}
	else {
		running = false;

		printf("Native: ThreadProc() start failure!\n");


		return EXIT_FAILURE; //Naturally exit so jni exception is thrown.
	}








	//Wait for the thread to die.
	int status;
	pthread_join(hookThreadId, (void **) &status);
	printf("Test: %i\n", status);

    if (pthread_kill(hookThreadId, SIGKILL) == 0) {
    	printf("Native: pthread_kill successful.\n");
    }

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



	//Clean up the mutex.
	pthread_mutex_destroy(&hookMutexHandle);

/*
	//Disable the XRecord context on the data display and close the connection.
	XCloseDisplay(disp_data);

	//Free the XRecord context on the hook display and close the connection.
	XRecordFreeContext(disp_hook, context);
	XCloseDisplay(disp_hook);
*/

	return EXIT_SUCCESS;
}
