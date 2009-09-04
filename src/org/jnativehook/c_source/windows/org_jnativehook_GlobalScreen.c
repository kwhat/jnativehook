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

#include <windows.h>

#include <jni.h>

#include "include/org_jnativehook_GlobalScreen.h"
#include "include/JConvertToNative.h"
#include "WinMapKeyCodes.h"

//Instance Variables
bool bRunning = True;
HHOOK handleKeyboardHook = NULL;
HHOOK handleMouseHook = NULL;


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


LRESULT CALLBACK LowLevelProc(int nCode, WPARAM wParam, LPARAM lParam) {
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




	KBDLLHOOKSTRUCT * p = (KBDLLHOOKSTRUCT *)lParam;

	if (jvm->AttachCurrentThread((void **)&env, NULL) >= 0) {
		switch (wParam) {
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				#ifdef DEBUG
				printf("Native: MsgLoop - Key pressed (%i)\n", xev.xkey.keycode);
				#endif

				jkey = NativeToJKeycode(xev.xkey.keycode);
				modifiers = 0;
				if (xev.xkey.state & ShiftMask)			modifiers |= NativeToJModifier(ShiftMask);
				if (xev.xkey.state & ControlMask)		modifiers |= NativeToJModifier(ControlMask);
				if (xev.xkey.state & getMetaMask())		modifiers |= NativeToJModifier(getMetaMask());
				if (xev.xkey.state & getAltMask())		modifiers |= NativeToJModifier(getAltMask());

				objEvent = (*env)->NewObject(env, clsKeyEvent, KeyEvent_ID, JK_KEY_PRESSED, (jlong) xev.xkey.time, modifiers, jkey.keycode, (jchar) jkey.keycode, jkey.location);
				(*env)->CallVoidMethod(env, objGlobalScreen, fireKeyPressed_ID, objEvent);
				objEvent = NULL;
			break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
				#ifdef DEBUG
				printf("Native: MsgLoop - Key released(%i)\n", xev.xkey.keycode);
				#endif

				jkey = NativeToJKeycode(xev.xkey.keycode);
				modifiers = 0;
				if (xev.xkey.state & ShiftMask)			modifiers |= NativeToJModifier(ShiftMask);
				if (xev.xkey.state & ControlMask)		modifiers |= NativeToJModifier(ControlMask);
				if (xev.xkey.state & getMetaMask())		modifiers |= NativeToJModifier(getMetaMask());
				if (xev.xkey.state & getAltMask())		modifiers |= NativeToJModifier(getAltMask());

				objEvent = (*env)->NewObject(env, clsKeyEvent, KeyEvent_ID, JK_KEY_RELEASED, (jlong) xev.xkey.time, modifiers, jkey.keycode, (jchar) jkey.keycode, jkey.location);
				(*env)->CallVoidMethod(env, objGlobalScreen, fireKeyReleased_ID, objEvent);
				objEvent = NULL;
			break;

			default:
			break;
		}
	}
	else {
		#ifdef DEBUG
		printf("C++: MsgLoop - Error on the attach current thread.\n");
		#endif
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void MsgLoop() {
	MSG message;

	while (bRunning && GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void MsgLoop() {


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

					jkey = NativeToJKeycode(xev.xkey.keycode);
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

					jkey = NativeToJKeycode(xev.xkey.keycode);
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
					objEvent = (*env)->NewObject(env, clsButtonEvent, MouseEvent_ID, JK_MOUSE_PRESSED, (jlong) xev.xbutton.time, 0, (jint) xev.xbutton.x, (jint) xev.xbutton.y, 1, (jboolean) False, jbutton);
					(*env)->CallVoidMethod(env, objGlobalScreen, fireMousePressed_ID, objEvent);
					objEvent = NULL;
				break;

				case ButtonRelease:
					#ifdef DEBUG
					printf("Native: MsgLoop - Button released(%i)\n", xev.xbutton.button);
					#endif

					jbutton = NativeToJButton(xev.xbutton.button);
					objEvent = (*env)->NewObject(env, clsButtonEvent, MouseEvent_ID, JK_MOUSE_RELEASED, (jlong) xev.xbutton.time, 0, (jint) xev.xbutton.x, (jint) xev.xbutton.y, 0, (jboolean) False, jbutton);
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

int factorial(int n) {
	if (n <= 1) {
		return 1;
	}
	else {
		return n * factorial( n - 1 );
	}
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_grabKey(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jmodifiers, jint jkeycode, jint jkeylocation) {
	JKeyCode jkey;
	jkey.keycode = jkeycode;
	jkey.location = jkeylocation;

	KeySym keysym = JKeycodeToNative(jkey);
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
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_ungrabKey(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jmodifiers, jint jkeycode, jint jkeylocation) {
	JKeyCode jkey;
	jkey.keycode = jkeycode;
	jkey.location = jkeylocation;

	KeySym keysym = JKeycodeToNative(jkey);
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
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_grabButton(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jbutton) {
	unsigned int button = JButtonToNative(jbutton);

	#ifdef DEBUG
	printf("Native: grabButton - Button(%i)\n", (unsigned int) button);
	#endif

	XGrabButton(disp, button, AnyModifier, default_win, True, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None);
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_ungrabButton(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jbutton) {
	unsigned int button = JButtonToNative(jbutton);

	#ifdef DEBUG
	printf("Native: ungrabButton - Button(%i)\n", (unsigned int) button);
	#endif

	XUngrabKey(disp, button, AnyModifier, default_win);
}


//This is where java attaches to the native machine.  Its kind of like the java + native constructor.
JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_initialize(JNIEnv * env, jobject UNUSED(obj)) {
	//Grab the currently running virtual machine so we can attach to it in
	//functions that are not called from java. ( I.E. MsgLoop )
	(*env)->GetJavaVM(env, &jvm);

	//Set the native error handler.
	//XSetErrorHandler((XErrorHandler) xErrorToException);

	//Setup the native hooks and their callbacks.
	handleKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0);
	if (handleKeyboardHook != NULL) {
		#ifdef DEBUG
		printf("Native: SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0) successful\n");
		#endif

		UnhookWindowsHookEx(hookHandle);
	}
	else {
		#ifdef DEBUG
		printf("Native: SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0) failed\n");
		#endif

		throwException(env, "Failed to hook keyboard using SetWindowsHookEx");

		//Naturaly exit so jni exception is thrown.
		return;
	}


	handleMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, hInst, 0);
	if (handleMouseHook != NULL) {
		#ifdef DEBUG
		printf("Native: SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, hInst, 0) successful\n");
		#endif

		UnhookWindowsHookEx(hookHandle);
	}
	else {
		#ifdef DEBUG
		printf("Native: SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, hInst, 0) failed\n");
		#endif

		throwException(env, "Failed to hook mouse using SetWindowsHookEx");

		//Naturaly exit so jni exception is thrown.
		return;
	}





	//Setup modifieres
	getModifiers(disp);

	//Call listener
	bRunning = True;

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

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_deinitialize(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	if (disp != NULL) {
		XUngrabKey(disp, AnyKey, AnyModifier, default_win);
		XCloseDisplay(disp);
		disp = NULL;
	}

	//Try to exit the thread naturally.
	bRunning = False;
	interruptMsgLoop();
	pthread_join(hookThreadId, NULL);

	#ifdef DEBUG
	printf("Native: Thread terminated successful.\n");
	#endif
}

BOOL APIENTRY DllMain(HINSTANCE _hInst, DWORD reason, LPVOID reserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			#ifdef DEBUG
			printf("Native: DllMain - DLL Process attach.\n");
			#endif

			hInst = _hInst;
		break;

		case DLL_PROCESS_DETACH:
			#ifdef DEBUG
			printf("Native: DllMain - DLL Process Detach.\n");
			#endif
		break;

		default:
		break;
	}

	return TRUE;
}
