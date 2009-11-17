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
bool bRunning = true;
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
	while (bRunning) {
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
					printf("Native: MsgLoop - Key released(%i)\n", xev.xkey.keycode);
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
					printf("Native: MsgLoop - Button pressed(%i)\n", xev.xbutton.button);
					#endif

					jbutton = NativeToJButton(xev.xbutton.button);
					objEvent = (*env)->NewObject(env, clsButtonEvent, MouseEvent_ID, JK_MOUSE_PRESSED, (jlong) xev.xbutton.time, 0, (jint) xev.xbutton.x, (jint) xev.xbutton.y, 1, (jboolean) false, jbutton);
					(*env)->CallVoidMethod(env, objGlobalScreen, fireMousePressed_ID, objEvent);
					objEvent = NULL;
				break;

				case ButtonRelease:
					#ifdef DEBUG
					printf("Native: MsgLoop - Button released(%i)\n", xev.xbutton.button);
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
			printf("Native: JNI Error Occured.\n");
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

int factorial(int n) {
	if (n <= 1) {
		return 1;
	}
	else {
		return n * factorial( n - 1 );
	}
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_grabKey(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jmodifiers, jint jkeycode, jint jkeylocation) {
	XLockDisplay(disp);

	JKeyCode jkey;
	jkey.keycode = jkeycode;
	jkey.location = jkeylocation;

	KeySym keysym = JKeycodeToNative(jkey);
	KeyCode keycode = XKeysymToKeycode(disp, keysym);

	#ifdef DEBUG
	printf("Native: grabKey - KeySym(%i) KeyCode(%i)\n", (unsigned int) keysym, (unsigned int) keycode);
	#endif

	unsigned int mask_table[10];
	unsigned int count = 0;

	if (getCapsLockMask() != 0) {
		#ifdef DEBUG
		printf("Native: grabKey - Using CapsLockMask\n");
		#endif
		mask_table[count++] = getCapsLockMask();
	}

	if (getNumberLockMask() != 0) {
		#ifdef DEBUG
		printf("Native: grabKey - Using NumberLockMask\n");
		#endif
		mask_table[count++] = getNumberLockMask();
	}

	if (getScrollLockMask() != 0) {
		#ifdef DEBUG
		printf("Native: grabKey - Using ScrollLockMask\n");
		#endif
		mask_table[count++] = getScrollLockMask();
	}

	if (jmodifiers & JK_SHIFT_MASK) {
		#ifdef DEBUG
		printf("Native: grabKey - Using ShiftMask\n");
		#endif
		mask_table[count++] = JModifierToNative(JK_SHIFT_MASK);
	}

	if (jmodifiers & JK_CTRL_MASK) {
		#ifdef DEBUG
		printf("Native: grabKey - Using ControlMask\n");
		#endif
		mask_table[count++] = JModifierToNative(JK_CTRL_MASK);
	}

	if (jmodifiers & JK_META_MASK) {
		#ifdef DEBUG
		printf("Native: grabKey - Using MetaMask\n");
		#endif
		mask_table[count++] = JModifierToNative(JK_META_MASK);
	}

	if (jmodifiers & JK_ALT_MASK) {
		#ifdef DEBUG
		printf("Native: grabKey - Using AltMask\n");
		#endif
		mask_table[count++] = JModifierToNative(JK_ALT_MASK);
	}

	if (jmodifiers & 0) {
		#ifdef DEBUG
		printf("Native: grabKey - Using No Mask\n");
		#endif
		mask_table[count++] = JModifierToNative(JK_ALT_MASK);
	}

	int set_size, i, j;
	for (set_size = count; set_size > 0; set_size--) {
		long num_of_items = factorial(count) / (factorial(set_size) * factorial(count - set_size));

		int pos = 0;
		for (i = 0; i < num_of_items; i++) {
			int curr_mask = 0;
			for (j = 0; j < set_size; j++) {
				curr_mask |= mask_table[pos];
				pos++;
				pos %= count;
			}

			XGrabKey(disp, keycode, curr_mask, default_win, true, GrabModeAsync, GrabModeAsync);
		}
	}

	XUnlockDisplay(disp);

	//Refresh what XNextEvent is listening for.
	interruptMsgLoop();
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_ungrabKey(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jmodifiers, jint jkeycode, jint jkeylocation) {
	XLockDisplay(disp);

	JKeyCode jkey;
	jkey.keycode = jkeycode;
	jkey.location = jkeylocation;

	KeySym keysym = JKeycodeToNative(jkey);
	KeyCode keycode = XKeysymToKeycode(disp, keysym);

	#ifdef DEBUG
	printf("Native: ungrabKey - KeySym(%i) KeyCode(%i)\n", (unsigned int) keysym, (unsigned int) keycode);
	#endif

	unsigned int mask_table[10];
	unsigned int count = 0;

	if (getCapsLockMask() != 0) {
		#ifdef DEBUG
		printf("Native: ungrabKey - Using CapsLockMask\n");
		#endif
		mask_table[count++] = getCapsLockMask();
	}

	if (getNumberLockMask() != 0) {
		#ifdef DEBUG
		printf("Native: ungrabKey - Using NumberLockMask\n");
		#endif
		mask_table[count++] = getNumberLockMask();
	}

	if (getScrollLockMask() != 0) {
		#ifdef DEBUG
		printf("Native: ungrabKey - Using ScrollLockMask\n");
		#endif
		mask_table[count++] = getScrollLockMask();
	}

	if (jmodifiers & JK_SHIFT_MASK) {
		#ifdef DEBUG
		printf("Native: ungrabKey - Using ShiftMask\n");
		#endif
		mask_table[count++] = JModifierToNative(JK_SHIFT_MASK);
	}

	if (jmodifiers & JK_CTRL_MASK) {
		#ifdef DEBUG
		printf("Native: ungrabKey - Using ControlMask\n");
		#endif
		mask_table[count++] = JModifierToNative(JK_CTRL_MASK);
	}

	if (jmodifiers & JK_META_MASK) {
		#ifdef DEBUG
		printf("Native: ungrabKey - Using MetaMask\n");
		#endif
		mask_table[count++] = JModifierToNative(JK_META_MASK);
	}

	if (jmodifiers & JK_ALT_MASK) {
		#ifdef DEBUG
		printf("Native: ungrabKey - Using AltMask\n");
		#endif
		mask_table[count++] = JModifierToNative(JK_ALT_MASK);
	}

	if (jmodifiers & 0) {
		#ifdef DEBUG
		printf("Native: ungrabKey - Using No Mask\n");
		#endif
		mask_table[count++] = JModifierToNative(JK_ALT_MASK);
	}

	int set_size, i, j;
	for (set_size = count; set_size > 0; set_size--) {
		long num_of_items = factorial(count) / (factorial(set_size) * factorial(count - set_size));

		int pos = 0;
		for (i = 0; i < num_of_items; i++) {
			int curr_mask = 0;
			for (j = 0; j < set_size; j++) {
				curr_mask |= mask_table[pos];
				pos++;
				pos %= count;
			}

			XUngrabKey(disp, keycode, curr_mask, default_win);
		}
	}

	XUnlockDisplay(disp);

	//Refresh what XNextEvent is listening for.
	interruptMsgLoop();
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_grabButton(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jbutton) {
	XLockDisplay(disp);
	unsigned int button = JButtonToNative(jbutton);

	#ifdef DEBUG
	printf("Native: grabButton - Button(%i)\n", (unsigned int) button);
	#endif

	XGrabButton(disp, button, AnyModifier, default_win, true, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None);
	XUnlockDisplay(disp);

	//Refresh what XNextEvent is listening for.
	interruptMsgLoop();
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_ungrabButton(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jbutton) {
	XLockDisplay(disp);
	unsigned int button = JButtonToNative(jbutton);

	#ifdef DEBUG
	printf("Native: ungrabButton - Button(%i)\n", (unsigned int) button);
	#endif

	XUngrabKey(disp, button, AnyModifier, default_win);
	XUnlockDisplay(disp);

	//Refresh what XNextEvent is listening for.
	interruptMsgLoop();
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
		free(exceptoin_msg);
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
		printf("Native: XOpenDisplay successful\n");
		#endif
	}


	//Set allowed events and the default root window.
	XAllowEvents(disp, AsyncBoth, CurrentTime);
	default_win = DefaultRootWindow(disp);


	/* Well XkbSetDetectableAutoRepeat doesnt work, big suprise right...
	 * Look at https://bugs.freedesktop.org/show_bug.cgi?id=22515 and
	 * http://www.x.org/wiki/Server16Branch for more information on
	 * otherwise useful features that do not work.
	 *
	 * For the time being you should relay on the keyboard repeat rate
	 * and delay functions to implement your own rate limitor in java.
	 * This will allow for more flexible rate limit control regardless
	 * of platform.
	 */
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


	//Iterate over screens
	int screen;
	for (screen = 0; screen < ScreenCount(disp); screen++) {
		#ifdef DEBUG
		printf ("Native: Init Screen %i\n", screen);
		#endif
		XSelectInput(disp, RootWindow(disp, screen), KeyPressMask | KeyReleaseMask | ButtonPress | ButtonRelease);
	}

	//Setup modifieres
	getModifiers(disp);

	//Call listener
	bRunning = true;

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
		XUngrabKey(disp, AnyKey, AnyModifier, default_win);
		XCloseDisplay(disp);
		disp = NULL;
	}

	//Try to exit the thread naturally.
	bRunning = false;
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
