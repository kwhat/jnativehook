/* Copyright (c) 2006-2010 - Alexander Barker (alex@1stleg.com)
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *	JNativeHook - GrabKeyHook - 01/12/10
 *  Alexander Barker
 *
 *	JNI Interface for setting a Keyboard Hook and monitoring
 *	it with java.
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

#include <Carbon/Carbon.h>

#include <jni.h>

#include "include/org_jnativehook_GlobalScreen.h"
#include "include/JConvertToNative.h"
#include "OSXKeyCodes.h"

//Instance Variables
bool bRunning = true;

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


static OSStatus EventHandlerCallback(EventHandlerCallRef caller, EventRef event, void* refcon) {
	UInt32	key;
	Point	mouse;
	EventMouseButton button;

	switch (GetEventKind(event)) {
		case kEventRawKeyDown:
			kindName = "KeyDown";
			GetEventParameter(event, kEventParamKeyCode, typeUInt32, NULL, sizeof(key), NULL, &key);
			fprintf(stderr, "keycode - %d - ", (int)ch);
		break;

		case kEventRawKeyUp:
			kindName = "KeyUp";
			GetEventParameter(event, kEventParamKeyCode, typeUInt32, NULL, sizeof(key), NULL, &key);
			fprintf(stderr, "keycode - %d - ", (int)ch);
		break;

		case kEventRawKeyRepeat:
			kindName = "KeyRepeat";
			GetEventParameter(event, kEventParamKeyCode, typeUInt32, NULL, sizeof(key), NULL, &key);
			fprintf(stderr, "keycode - %d - ", (int)ch);
		break;

		case kEventRawKeyModifiersChanged:
			GetEventParameter(event, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(key), NULL, &key);
			fprintf(stderr, "0x%x - ", (unsigned int)ch);
			kindName = "ModifiersChanged";
		break;

		case kEventMouseDown:
			kindName = "MouseDown";
			GetEventParameter(event, kEventParamMouseButton, typeMouseButton, NULL, sizeof(button), NULL, &button);
			fprintf(stderr, "Mouse button %d pressed\n", (int) button);
		break;

		case kEventMouseUp:
			kindName = "MouseUp";
			GetEventParameter(event, kEventParamMouseButton, typeMouseButton, NULL, sizeof(button), NULL, &button);
			fprintf(stderr, "Mouse button %d released\n", (int) button);
		break;

		case kEventMouseMoved:
			kindName = "MouseMoved";
			GetEventParameter(event, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(mouse), NULL, &mouse);
			fprintf(stderr, "location - %d, %d - ", mouse.v, mouse.h);
		break;

		case kEventMouseDragged:
			kindName = "MouseDragged";
		break;

		case kEventMouseWheelMoved:
			kindName = "MouseWheel";
		break;

		default:
		break;
	}



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

OSStatus InitEventHandlers(void) {
	EventTypeSpec	kEvents[] = {
		{ kEventClassKeyboard, kEventRawKeyDown },
		{ kEventClassKeyboard, kEventRawKeyUp },
		{ kEventClassKeyboard, kEventRawKeyRepeat },
		{ kEventClassKeyboard, kEventRawKeyModifiersChanged },
		{ kEventClassMouse, kEventMouseDown },
		{ kEventClassMouse, kEventMouseUp },
		{ kEventClassMouse, kEventMouseMoved },
		{ kEventClassMouse, kEventMouseDragged },
		{ kEventClassMouse, kEventMouseWheelMoved }
	};

	//FIXME This should return something??
	InstallEventHandler(GetEventMonitorTarget(), MonitorHandler, GetEventTypeCount( kEvents ), kEvents, 0, &sHandler);

	return noErr;
}



JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_grabKey(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jmodifiers, jint jkeycode, jint jkeylocation) {

}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_ungrabKey(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jmodifiers, jint jkeycode, jint jkeylocation) {

}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_grabButton(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jbutton) {

}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_ungrabButton(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jbutton) {

}


JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	SInt32 interval = -1;

	CFTypeRef pref_val = CFPreferencesCopyAppValue(CFSTR("KeyRepeat"), kCFPreferencesCurrentApplication);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		CFNumberGetValue((CFNumberRef)value, kCFNumberSInt32Type, &interval);
		#ifdef DEBUG
		printf("Native: CFPreferencesCopyAppValue successful (rate: %i)\n", interval);
		#endif
	}
	else {
		interval = LMGetKeyRepThresh() * 60;
		if (interval < 0) {
			#ifdef DEBUG
			printf("Native: LMGetKeyRepThresh failure\n");
			#endif

			throwException("Could not determine the keyboard auto repeat rate.");
			return -1; //Naturaly exit so jni exception is thrown.
		}
		else {
			#ifdef DEBUG
			printf("Native: LMGetKeyRepThresh successful (rate: %i)\n", interval);
			#endif
		}
	}

	return (jlong) interval;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	SInt32 timeout = -1;

	CFTypeRef pref_val = CFPreferencesCopyAppValue(CFSTR("InitialKeyRepeat"), kCFPreferencesCurrentApplication);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		CFNumberGetValue((CFNumberRef)value, kCFNumberSInt32Type, &interval);
		#ifdef DEBUG
		printf("Native: CFPreferencesCopyAppValue successful (delay: %i)\n", timeout);
		#endif
	}
	else {
		timeout = LMGetKeyThresh() * 60;
		if (interval < 0) {
			#ifdef DEBUG
			printf("Native: LMGetKeyRepThresh failure\n");
			#endif

			throwException("Could not determine the keyboard auto repeat delay.");
			return -1; //Naturaly exit so jni exception is thrown.
		}
		else {
			#ifdef DEBUG
			printf("Native: LMGetKeyThresh successful (delay: %i)\n", timeout);
			#endif
		}
	}

	return (jlong) timeout;
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

	//Check and make sure assistive devices is enabled.
	if (! AXAPIEnabled()) {
		#ifdef DEBUG
		printf("Native: Accessibility API is not enabled.\n");
		#endif
		throwException("Please enabled access for assistive devices in the Universal Access section of the System Preferences.");
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}


	//Iterate over screens
	int screen;
	for (screen = 0; screen < ScreenCount(disp); screen++) {
		#ifdef DEBUG
		printf ("Native: Init Screen %i\n", screen);
		#endif
		XSelectInput(disp, RootWindow(disp, screen), KeyPressMask | KeyReleaseMask | ButtonPress | ButtonRelease);
	}

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
