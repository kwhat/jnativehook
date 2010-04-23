/*
 *	JNativeHook - GrabKeyHook - 09/08/06
 *  Alexander Barker
 *
 *	JNI Interface for setting a Keyboard Hook and monitoring
 *	it with java.
 *
 *  TODO Add LGPL License
 */

/*
Compiling Options:
	gcc -m32 -march=i586 -shared -fPIC -lX11 -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux ./org_jnativehook_keyboard_GrabKeyHook.c -o libJNativeHook_Keyboard.so
	gcc -m64 -march=k8 -shared -fPIC -lX11 -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux ./org_jnativehook_keyboard_GrabKeyHook.c -o libJNativeHook_Keyboard.so
*/

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
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


#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#include <jni.h>

#include "include/org_jnativehook_GlobalScreen.h"
#include "include/JConvertToNative.h"
#include "XMapModifers.h"

//Instance Variables
bool isRunning = true;
unsigned int xkb_timeout;
unsigned int xkb_interval;

Display * disp;
Window default_win;

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

void throwException(char * message) {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Locate our exception class
	jclass objExceptionClass = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyException");

	if (objExceptionClass != NULL) {
		#ifdef DEBUG
		printf("Native: Exception - %s\n", message);
		#endif

		(*env)->ThrowNew(env, objExceptionClass, message);
	}
	else {
		//Unable to find exception class, Terminate with error.
		jniFatalError("Unable to locate NativeKeyException class.");
	}

	free(message):
}

int xErrorToException(Display * dpy, XErrorEvent * e) {
	char message[255];
	XGetErrorText(dpy, e->error_code, message, sizeof message);

	#ifdef DEBUG
	printf("Native: XError %i - %s.\n", e->error_code, message);
	#endif

	throwException(message);

	return 0;
}

void callback(XPointer pointer, XRecordInterceptData* hook) {
	fprintf(stderr, "got an XRecord event! (%i)\n", hook->category);

	// FIXME: we need use XQueryPointer to get the first location
	static int cur_x = 0;
	static int cur_y = 0;

	if (hook->category != XRecordFromServer && hook->category != XRecordFromClient) {
		XRecordFreeData (hook);
		return;
	}


	XRecordDatum *data = (XRecordDatum*) hook->data;

	int event_type = data->type;

	BYTE btncode, keycode;
	btncode = keycode = data->event.u.u.detail;

	int rootx = data->event.u.keyButtonPointer.rootX;
	int rooty = data->event.u.keyButtonPointer.rootY;
	int time = hook->server_time;

	switch (event_type) {
		case KeyPress:
			// if escape is pressed, stop the loop and clean up, then exit
			if (keycode == 9) {
				stop = 1;
				XRecordDisableContext(XOpenDisplay( NULL ), context);
				break;
			}

			// Note: you should not use data_disp to do normal X operations !!!
			printf ("KeyPress: \t%s", XKeysymToString(XKeycodeToKeysym( XOpenDisplay( NULL ), keycode, 0)));
			//printf ("KeyPress: \t%i", XKeysymToKeycode( XOpenDisplay( NULL ), XK_A));
			break;
		case KeyRelease:
			printf ("KeyRelease: \t%s", XKeysymToString(XKeycodeToKeysym( XOpenDisplay( NULL ), keycode, 0)));
			break;
		case ButtonPress:
			printf ("ButtonPress: \t%d, rootX=%d, rootY=%d", btncode, cur_x, cur_y);
			break;
		case ButtonRelease:
			printf ("ButtonRelease: \t%d, rootX=%d, rootY=%d", btncode, cur_x, cur_y);
			break;
		case MotionNotify:
			printf ("MouseMove: \trootX=%d, rootY=%d",rootx, rooty);
			cur_x = rootx;
			cur_y = rooty;
			break;
		case CreateNotify:
			break;
		case DestroyNotify:
			break;
		case NoExpose:
			break;
		case Expose:
			break;
		default:
			break;
	}

	printf (", time=%d\n", time);

	XRecordFreeData (hook);
}

void MsgLoop() {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Class and Constructor for the NativeKeyEvent Object
	jclass clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
	jmethodID KeyEvent_ID = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIICI)V");

	//Class and Constructor for the NativeMouseEvent Object
	jclass clsButtonEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
	jmethodID MouseEvent_ID = (*env)->GetMethodID(env, clsButtonEvent, "<init>", "(IJIIIIZI)V");

	//Class and getInstance method id for the GlobalScreen Object
	jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
	jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");

	//A reference to the GlobalScreen Object
	jobject objGlobalScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);

	//ID's for the pressed, typed and released callbacks
	jmethodID fireKeyPressed_ID = (*env)->GetMethodID(env, clsGlobalScreen, "fireKeyPressed", "(Lorg/jnativehook/keyboard/NativeKeyEvent;)V");
	jmethodID fireKeyReleased_ID = (*env)->GetMethodID(env, clsGlobalScreen, "fireKeyReleased", "(Lorg/jnativehook/keyboard/NativeKeyEvent;)V");

	jmethodID fireMousePressed_ID = (*env)->GetMethodID(env, clsGlobalScreen, "fireMousePressed", "(Lorg/jnativehook/mouse/NativeMouseEvent;)V");
	jmethodID fireMouseReleased_ID = (*env)->GetMethodID(env, clsGlobalScreen, "fireMousePressed", "(Lorg/jnativehook/mouse/NativeMouseEvent;)V");

	XEvent xev;
	JKeyCode jkey;
	jint jbutton;
	jint modifiers;
	jobject objEvent = NULL;
	while (isRunning) {
		XNextEvent(disp, &xev);

		//We need to check to see if we interrupted XNextEvent or if this was for a legitimate event.
		if (!xev.xany.send_event) {
			switch (xev.type) {
				case KeyPress:
					#ifdef DEBUG
					printf("Native: MsgLoop - Key pressed (%i)\n", xev.xkey.keycode);
					#endif

					jkey = NativeToJKeycode(XLookupKeysym(&xev.xkey, 0));
					modifiers = 0;
					if (xev.xkey.state & ShiftMask)			modifiers |= NativeToJModifier(ShiftMask);
					if (xev.xkey.state & ControlMask)		modifiers |= NativeToJModifier(ControlMask);
					if (xev.xkey.state & getMetaMask())		modifiers |= NativeToJModifier(getMetaMask());
					if (xev.xkey.state & getAltMask())		modifiers |= NativeToJModifier(getAltMask());

					objEvent = (*env)->NewObject(env, clsKeyEvent, KeyEvent_ID, JK_KEY_PRESSED, (jlong) xev.xkey.time, modifiers, jkey.keycode, (jchar) jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, fireKeyPressed_ID, objEvent);
					objEvent = NULL;
				break;

				case KeyRelease:
					#ifdef DEBUG
					printf("Native: MsgLoop - Key released (%i)\n", xev.xkey.keycode);
					#endif

					jkey = NativeToJKeycode(XLookupKeysym(&xev.xkey, 0));
					modifiers = 0;
					if (xev.xkey.state & ShiftMask)			modifiers |= NativeToJModifier(ShiftMask);
					if (xev.xkey.state & ControlMask)		modifiers |= NativeToJModifier(ControlMask);
					if (xev.xkey.state & getMetaMask())		modifiers |= NativeToJModifier(getMetaMask());
					if (xev.xkey.state & getAltMask())		modifiers |= NativeToJModifier(getAltMask());

					objEvent = (*env)->NewObject(env, clsKeyEvent, KeyEvent_ID, JK_KEY_RELEASED, (jlong) xev.xkey.time, modifiers, jkey.keycode, (jchar) jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, fireKeyReleased_ID, objEvent);
					objEvent = NULL;
				break;

				case ButtonPress:
					#ifdef DEBUG
					printf("Native: MsgLoop - Button pressed (%i)\n", xev.xbutton.button);
					#endif

					jbutton = NativeToJButton(xev.xbutton.button);
					objEvent = (*env)->NewObject(env, clsButtonEvent, MouseEvent_ID, JK_MOUSE_PRESSED, (jlong) xev.xbutton.time, 0, (jint) xev.xbutton.x, (jint) xev.xbutton.y, 1, (jboolean) false, jbutton);
					(*env)->CallVoidMethod(env, objGlobalScreen, fireMousePressed_ID, objEvent);
					objEvent = NULL;
				break;

				case ButtonRelease:
					#ifdef DEBUG
					printf("Native: MsgLoop - Button released (%i)\n", xev.xbutton.button);
					#endif

					jbutton = NativeToJButton(xev.xbutton.button);
					objEvent = (*env)->NewObject(env, clsButtonEvent, MouseEvent_ID, JK_MOUSE_RELEASED, (jlong) xev.xbutton.time, 0, (jint) xev.xbutton.x, (jint) xev.xbutton.y, 0, (jboolean) false, jbutton);
					(*env)->CallVoidMethod(env, objGlobalScreen, fireMouseReleased_ID, objEvent);
					objEvent = NULL;
				break;
			}
		}

		#ifdef DEBUG
		if ((*env)->ExceptionOccurred(env)) {
			printf("Native: JNI Error Occurred.\n");
			((*env)->ExceptionDescribe(env));
		}
		#endif
	}

	#ifdef DEBUG
	printf("Native: MsgLoop() stop successful.\n");
	#endif
}

void interruptMsgLoop() {
	//We need to create an event to send to the thread to wake it up
	//so it will listen to grab changes.
	XAnyEvent xev;
	//xev.type		= KeyPress;
	xev.display		= disp;			// defined globally
	xev.window		= default_win;

	XSendEvent(xev.display, xev.window, true, KeyPressMask, (XEvent *)&xev);
	XFlush(xev.display);
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	if (! XkbGetAutoRepeatRate(disp, XkbUseCoreKbd, &xkb_timeout, &xkb_interval) ) {
		#ifdef DEBUG
		printf("Native: XkbGetAutoRepeatRate failure\n");
		#endif

		throwException("Could not determine the keyboard auto repeat rate.");
		return -1; //Naturaly exit so jni exception is thrown.
	}

	#ifdef DEBUG
	printf("Native: XkbGetAutoRepeatRate successful (rate: %i) (delay: %i)\n", xkb_interval, xkb_timeout);
	#endif
	return (jlong) xkb_interval;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	if (! XkbGetAutoRepeatRate(disp, XkbUseCoreKbd, &xkb_timeout, &xkb_interval) ) {
		#ifdef DEBUG
		printf("Native: XkbGetAutoRepeatRate failure\n");
		#endif

		throwException("Could not determine the keyboard auto repeat delay.");
		return -1; //Naturaly exit so jni exception is thrown.
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
			printf("Native: JNI_VERSION_1_4 unavailable for use. (default: %X)\n", jni_ret);
		#endif
	}

	//Tell X Threads are OK
	XInitThreads();

	//Set the native error handler.
	XSetErrorHandler((XErrorHandler) xErrorToException);

	//Grab the default display
	char * disp_name = XDisplayName(NULL);
	disp = XOpenDisplay(disp_name);
	if (disp == NULL) {
		//We couldnt hook a display so we need to die.
		char * error_msg = "Could not open display: ";
		char * exceptoin_msg = (char *) malloc( (strlen(error_msg) + strlen(disp_name)) + 1 * sizeof(char));

		strcat(exceptoin_msg, error_msg);
		strcat(exceptoin_msg, disp_name);

		throwException(exceptoin_msg);
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf("Native: XOpenDisplay successful.\n");
		#endif
	}

	//Check to make sure XRecord is installed and enabled.
	int major, minor;
	if (!XRecordQueryVersion (disp, &major, &minor)) {
		throwException("XRecord is not currently available");
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf ("Native: XRecord version: %d.%d\n", major, minor);
		#endif
	}

	//Setup XRecord range
	XRecordClientSpec clients = XRecordAllClients;
	XRecordRange * range = XRecordAllocRange();
	if (range == NULL) {
		throwException("Could not allocate XRecordRange");
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf("Native: XRecordAllocRange successful.\n");
		#endif
	}

	//Create XRecord Context
	range->device_events.first = KeyPress;
	range->device_events.last = MotionNotify;
	context = XRecordCreateContext(display, 0, &clients, 1, &range, 1);
	XFree(range);
	if (context == NULL) {
		throwException("Could not create XRecordContext");
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf("Native: XRecordCreateContext successful.\n");
		#endif
	}


	//enable detectable autorepate.
	Bool isAutoRepeat;
	XkbSetDetectableAutoRepeat(disp, true, &isAutoRepeat);
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

	XRecordEnableContextAsync(display, context, callback, NULL);
	//XRecordEnableContext(display, context, callback, NULL);

	//Call listener
	isRunning = true;

	if( pthread_create( &hookThreadId, NULL, (void *) &MsgLoop, NULL) ) {
		#ifdef DEBUG
		printf("Native: MsgLoop() start failure.\n");
		#endif
		throwException("Could not create message loop thread.");
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
		printf("Native: MsgLoop() start successful.\n");
		#endif
	}

	return jni_ret;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * UNUSED(vm), void * UNUSED(reserved)) {
	if (disp != NULL) {
		XRecordDisableContext(disp, context);
		XRecordFreeContext(disp, context);

		XCloseDisplay(disp);
		disp = NULL;
	}


	//Try to exit the thread naturally.
	isRunning = false;
	interruptMsgLoop();
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
