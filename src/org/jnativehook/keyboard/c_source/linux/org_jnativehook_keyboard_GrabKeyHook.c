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

typedef enum { FALSE, TRUE } bool;
typedef char byte;


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
#include <string.h>

#include <pthread.h>
#include <signal.h>


#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#include <jni.h>

#include "org_jnativehook_keyboard_GrabKeyHook.h"
#include "include/JConvertToNative.h"
#include "XMapModifers.h"

//Instance Variables
bool bRunning = TRUE;

Display * disp;
Window default_win;

JavaVM * jvm = NULL;
pthread_t hookThreadId = 0;

//Shared Object Constructor and Deconstructor
void __attribute__ ((constructor)) Init(void);
void __attribute__ ((destructor)) Cleanup(void);

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
	jclass clsEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
	jmethodID constructor_ID = (*env)->GetMethodID(env, clsEvent, "<init>", "(IJIICI)V");

	//Class and getInstance method id for the GlobalScreen Object
	jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
	jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");

	//A reference to the GlobalScreen Object
	jobject objGlobalScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);

	//ID's for the pressed, typed and released callbacks
	jmethodID fireKeyPressed_ID = (*env)->GetMethodID(env, clsGlobalScreen, "fireKeyPressed", "(Lorg/jnativehook/keyboard/NativeKeyEvent;)V");
	jmethodID fireKeyReleased_ID = (*env)->GetMethodID(env, clsGlobalScreen, "fireKeyReleased", "(Lorg/jnativehook/keyboard/NativeKeyEvent;)V");


	XEvent xev;
	jobject objEvent = NULL;
	while (bRunning) {
		XNextEvent(disp, &xev);

		switch (xev.type) {
			case KeyPress:
				#ifdef DEBUG
				printf("Native: MsgLoop - Key pressed (%i)\n", xev.xkey.keycode);
				#endif

				objEvent = (*env)->NewObject(env, clsEvent, constructor_ID, (jlong) xev.xkey.time, (jint) xev.xkey.state, (jint) xev.xkey.keycode, (jchar) xev.xkey.keycode, 0);
				(*env)->CallVoidMethod(env, objGlobalScreen, fireKeyPressed_ID, objEvent);
			break;

			case KeyRelease:
				#ifdef DEBUG
				printf("Native: MsgLoop - Key released(%i)\n", xev.xkey.keycode);
				#endif

				objEvent = (*env)->NewObject(env, clsEvent, constructor_ID, (jlong) xev.xkey.time, (jint) xev.xkey.state, (jint) xev.xkey.keycode, (jchar) xev.xkey.keycode, 0);
				(*env)->CallVoidMethod(env, objGlobalScreen, fireKeyReleased_ID, objEvent);
			break;
		}
	}

	#ifdef DEBUG
	printf("Native: MsgLoop() stop successful.\n");
	#endif
}

void interruptMsgLoop() {
	//We need to create an event to send to the thread to wake it up
	//so it will listen to grab changes.
	XKeyEvent xev;
	xev.display		= disp;			// defined globally
	xev.window		= default_win;
	xev.root		= default_win;	// defined globally
	xev.subwindow	= None;
	xev.time		= CurrentTime;
	xev.x			= 1;
	xev.y			= 1;
	xev.x_root		= 1;
	xev.y_root		= 1;
	xev.same_screen	= True;
	xev.keycode		= AnyKey;
	xev.state		= 0;
	xev.type		= KeyPress;

	XSendEvent(xev.display, xev.window, True, KeyPressMask, (XEvent *)&xev);
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

JNIEXPORT void JNICALL Java_org_jnativehook_keyboard_GrabKeyHook_grabKey(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jmodifiers, jint jkeycode, jint jkeylocation) {
	XLockDisplay(disp);
	KeySym keysym = JKeycodeToNative(jkeycode, jkeylocation);
	KeyCode keycode = XKeysymToKeycode(disp, keysym);

	#ifdef DEBUG
	printf("Native: grabKey - KeyCode(%i) Modifier(%X)\n", (unsigned int) keysym, keycode);
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

			XGrabKey(disp, keycode, curr_mask, default_win, True, GrabModeAsync, GrabModeAsync);
		}
	}

	XUnlockDisplay(disp);

	//Refresh what XNextEvent is listening for.
	interruptMsgLoop();
}

JNIEXPORT void JNICALL Java_org_jnativehook_keyboard_GrabKeyHook_ungrabKey(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jmodifiers, jint jkeycode, jint jkeylocation) {
	XLockDisplay(disp);
	KeySym keysym = JKeycodeToNative(jkeycode, jkeylocation);
	KeyCode keycode = XKeysymToKeycode(disp, keysym);

	#ifdef DEBUG
	printf("Native: ungrabKey - KeyCode(%i) Modifier(%X)\n", (unsigned int) keysym, keycode);
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

//This is where java attaches to the native machine.  Its kind of like the java + native constructor.
JNIEXPORT void JNICALL Java_org_jnativehook_keyboard_GrabKeyHook_registerHook(JNIEnv * env, jobject UNUSED(obj)) {
	//Grab the currently running virtual machine so we can attach to it in
	//functions that are not called from java. ( I.E. MsgLoop )
	(*env)->GetJavaVM(env, &jvm);

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

		//Naturaly exit so jni exception is thrown.
		return;
	}
	else {
		#ifdef DEBUG
		printf("Native: XOpenDisplay successful\n");
		#endif
	}


	//Set allowed events and the default root window.
	XAllowEvents(disp, AsyncKeyboard, CurrentTime);
	default_win = DefaultRootWindow(disp);
	XkbSetDetectableAutoRepeat(disp, TRUE, NULL);

	//Iterate over screens
	int screen;
	for (screen = 0; screen < ScreenCount(disp); screen++) {
		#ifdef DEBUG
		printf ("Native: Init Screen %i\n", screen);
		#endif
		XSelectInput(disp, RootWindow(disp, screen), KeyPressMask | KeyReleaseMask);
	}

	//Setup modifieres
	getModifiers(disp);

	//Call listener
	bRunning = TRUE;

	if( pthread_create( &hookThreadId, NULL, (void *) &MsgLoop, NULL) ) {
		#ifdef DEBUG
		printf("Native: MsgLoop() start failure.\n");
		#endif
		//TODO Throw an exception
	}
	else {
		#ifdef DEBUG
		printf("Native: MsgLoop() start successful.\n");
		#endif
	}
}

JNIEXPORT void JNICALL Java_org_jnativehook_keyboard_GrabKeyHook_unregisterHook(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	if (disp != NULL) {
		XUngrabKey(disp, AnyKey, AnyModifier, default_win);
		XCloseDisplay(disp);
		disp = NULL;
	}

	//Try to exit the thread naturally.
	bRunning = FALSE;
	interruptMsgLoop();
	pthread_join(hookThreadId, NULL);

	#ifdef DEBUG
	printf("Native: Thread terminated successful.\n");
	#endif
}

void Init() {
	//Tell X Threads are OK
	XInitThreads();

	#ifdef DEBUG
	printf("Native: Init - Shared Object Process Attach.\n");
	#endif
}

void Cleanup() {
	//Make sure the thread has stopped.
	if (pthread_kill(hookThreadId, SIGKILL) == 0) {
		#ifdef DEBUG
		printf("Native: pthread_kill successful.\n");
		#endif
	}

	#ifdef DEBUG
	printf("Native: Cleanup - Shared Object Process Detach.\n");
	#endif
}
