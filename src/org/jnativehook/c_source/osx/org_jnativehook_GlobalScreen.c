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
	#define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
	#define UNUSED(x) /*@unused@*/ x
#else
	#define UNUSED(x) x
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
JavaVM * jvm = NULL;
pthread_t hookThreadId = 0;
CGEventFlags prev_event_mask = 0;
unsigned int event_modifiers[] = {
	shiftKey,
	rightShiftKey,
	controlKey,
	rightControlKey,
	optionKey,
	rightOptionKey,
	cmdKey
	/*, rightCmdKey */
};

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

jint doModifierConvert(CGEventFlags event_mask) {
	jint modifiers = 0;

	//Apply all our modifiers to the java modifiers return.
	int i, size = sizeof(event_modifiers) / sizeof(unsigned int);
	for (i = 0; i < size; i++) {
		if (event_mask & event_modifiers[i]) {
			modifiers |= NativeToJModifier(event_modifiers[i]);
		}
	}

	return modifiers;
}


CGEventRef eventHandlerCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void * refcon) {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Class and getInstance method id for the GlobalScreen Object
	jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
	jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");

	//A reference to the GlobalScreen Object
	jobject objGlobalScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);

	//Event Data
	CGPoint event_point;
	CGEventTimestamp event_time = CGEventGetTimestamp(event);
	UInt64	keysym, button;
	CGEventFlags event_mask = CGEventGetFlags(event);

	JKeyDatum jkey;
	jint jbutton;
	jint modifiers;
	jclass clsKeyEvent, clsMouseEvent;
	jmethodID idKeyEvent, idMouseEvent;
	jmethodID idDispatchEvent = (*env)->GetMethodID(env, clsGlobalScreen, "dispatchEvent", "(Lorg/jnativehook/NativeInputEvent;)V");
	jobject objKeyEvent, objMouseEvent;


	// get the event class
	switch (CGEventGetType(event)) {
		case kCGEventKeyDown:
			keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			#ifdef DEBUG
				printf("Native: eventHandlerCallback - Key pressed (%i)\n", (unsigned int) keysym);
			#endif

			//Class and Constructor for the NativeKeyEvent Object
			clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
			idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

			jkey = NativeToJKey(keysym);
			modifiers = doModifierConvert(event_mask);

			//Fire key pressed event.
			objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_PRESSED, (jlong) event_time, modifiers, (jint) keysym, jkey.keycode, jkey.location);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		break;

		case kCGEventKeyUp:
			keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			#ifdef DEBUG
				printf("Native: eventHandlerCallback - Key released (%i)\n", (unsigned int) keysym);
			#endif

			//Class and Constructor for the NativeKeyEvent Object
			clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
			idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

			jkey = NativeToJKey(keysym);
			modifiers = doModifierConvert(event_mask);

			//Fire key released event.
			objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_RELEASED, (jlong) event_time, modifiers, keysym, jkey.keycode, jkey.location);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		break;

		case kCGEventFlagsChanged:
			keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			#ifdef DEBUG
				printf("Native: eventHandlerCallback - Modifiers changed (%X %i)\n", (unsigned int) event_mask, (unsigned int) keysym);
			#endif

			//Class and Constructor for the NativeKeyEvent Object
			clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
			idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

			jkey = NativeToJKey(keysym);
			modifiers = doModifierConvert(event_mask);

			//Because of the way apple handles modifiers we need to send a key up or key down event for each time this changes.
			/*	Outline of what is happening on the next 3 lines.
				1010 1100	prev
				1100 1010	curr
				0110 0110	prev xor curr

				//truned on - i.e. pressed
				1100 1010	curr
				0110 0110	(prev xor curr)
				0100 0010	(prev xor curr) of prev

				//turned off - i.e. released
				1010 1100	prev
				0110 0110	(prev xor curr)
				0010 0100	(prev xor curr) and prev
			 */
			CGEventFlags diff_event_mask = prev_event_mask ^ event_mask;
			CGEventFlags keydown_event_mask = prev_event_mask | diff_event_mask;
			CGEventFlags keyup_event_mask = event_mask & diff_event_mask;

			//Update the previous event mask.
			prev_event_mask = event_mask;

			int i, size = sizeof(event_modifiers) / sizeof(unsigned int);
			for (i = 0; i < size; i++) {
				if (keydown_event_mask & event_modifiers[i]) {
					//Fire key pressed event.
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_PRESSED, (jlong) event_time, modifiers, (jint) keysym, jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				}
				else if (keyup_event_mask & event_modifiers[i]) {
					//Fire key released event.
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_RELEASED, (jlong) event_time, modifiers, keysym, jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				}
			}
		break;

		case kCGEventLeftMouseDown:
		case kCGEventRightMouseDown:
		case kCGEventOtherMouseDown:
			button = CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber);
			event_point = CGEventGetLocation(event);
			#ifdef DEBUG
				printf("Native: MsgLoop - Button pressed (%i)\n", (unsigned int) button);
			#endif

			//Class and Constructor for the NativeMouseEvent Object
			clsMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
			idMouseEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");

			jbutton = NativeToJButton(button);
			modifiers = doModifierConvert(event_mask);

			//Fire mouse pressed event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseEvent, JK_NATIVE_MOUSE_PRESSED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y, jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case kCGEventLeftMouseUp:
		case kCGEventRightMouseUp:
		case kCGEventOtherMouseUp:
			button = CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber);
			event_point = CGEventGetLocation(event);
			#ifdef DEBUG
				printf("Native: MsgLoop - Button released (%i)\n", (unsigned int) button);
			#endif

			//Class and Constructor for the NativeMouseEvent Object
			clsMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
			idMouseEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");

			jbutton = NativeToJButton(button);
			modifiers = doModifierConvert(event_mask);

			//Fire mouse released event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseEvent, JK_NATIVE_MOUSE_RELEASED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y, jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;


		case kCGEventMouseMoved:
			event_point = CGEventGetLocation(event);
			#ifdef DEBUG
				printf ("Native: MsgLoop - Motion Notified (%f,%f)\n", event_point.x, event_point.y);
			#endif

			//Class and Constructor for the NativeMouseEvent Object
			clsMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
			idMouseEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIII)V");

			modifiers = doModifierConvert(event_mask);

			//Fire mouse moved event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseEvent, JK_NATIVE_MOUSE_MOVED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case kCGEventLeftMouseDragged:
		case kCGEventRightMouseDragged:
		case kCGEventOtherMouseDragged:
			event_point = CGEventGetLocation(event);
			#ifdef DEBUG
				printf ("Native: EventHandlerCallback - MouseDragged (Unimplemented)\n");
			#endif

			//FIXME Call mouse move event for now.
		break;

		case kCGEventScrollWheel:
			#ifdef DEBUG
				printf ("Native: EventHandlerCallback - MouseWheelMoved (Unimplemented)\n");
			#endif
		break;


		default:
			#ifdef DEBUG
				printf ("Native: eventHandlerCallback - Unhandled Event Type\n");
			#endif
		break;
	}

	#ifdef DEBUG
		if ((*env)->ExceptionOccurred(env)) {
			printf("Native: JNI Error Occured.\n");
			((*env)->ExceptionDescribe(env));
		}
	#endif

	return noErr;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	SInt32 interval = -1;

	CFTypeRef pref_val = CFPreferencesCopyAppValue(CFSTR("KeyRepeat"), kCFPreferencesCurrentApplication);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		CFNumberGetValue((CFNumberRef)pref_val, kCFNumberSInt32Type, &interval);
		#ifdef DEBUG
			printf("Native: CFPreferencesCopyAppValue successful (rate: %i)\n", (int) interval);
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
				printf("Native: LMGetKeyRepThresh successful (rate: %i)\n", (int) interval);
			#endif
		}
	}

	return (jlong) interval;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	SInt32 interval = -1;

	CFTypeRef pref_val = CFPreferencesCopyAppValue(CFSTR("InitialKeyRepeat"), kCFPreferencesCurrentApplication);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		CFNumberGetValue((CFNumberRef)pref_val, kCFNumberSInt32Type, &interval);
		#ifdef DEBUG
			printf("Native: CFPreferencesCopyAppValue successful (delay: %i)\n", (int) interval);
		#endif
	}
	else {
		interval = LMGetKeyThresh() * 60;
		if (interval < 0) {
			#ifdef DEBUG
				printf("Native: LMGetKeyRepThresh failure\n");
			#endif

			throwException("Could not determine the keyboard auto repeat delay.");
			return -1; //Naturaly exit so jni exception is thrown.
		}
		else {
			#ifdef DEBUG
				printf("Native: LMGetKeyThresh successful (delay: %i)\n", (int) interval);
			#endif
		}
	}

	return (jlong) interval;
}

void MsgLoop() {
	CGEventMask event_mask =	CGEventMaskBit(kCGEventKeyDown) |
								CGEventMaskBit(kCGEventKeyUp) |
								CGEventMaskBit(kCGEventFlagsChanged) |

								CGEventMaskBit(kCGEventLeftMouseDown) |
								CGEventMaskBit(kCGEventLeftMouseUp) |
								CGEventMaskBit(kCGEventLeftMouseDragged) |

								CGEventMaskBit(kCGEventRightMouseDown) |
								CGEventMaskBit(kCGEventRightMouseUp) |
								CGEventMaskBit(kCGEventRightMouseDragged) |

								CGEventMaskBit(kCGEventOtherMouseDown) |
								CGEventMaskBit(kCGEventOtherMouseUp) |
								CGEventMaskBit(kCGEventOtherMouseDragged) |

								CGEventMaskBit(kCGEventMouseMoved) |
								CGEventMaskBit(kCGEventScrollWheel);


    CFMachPortRef event_port = CGEventTapCreate(
									kCGSessionEventTap,
									kCGHeadInsertEventTap,
									kCGEventTapOptionListenOnly,
									event_mask,
									eventHandlerCallback,
									NULL
                                );
	if (event_port == NULL) {
		#ifdef DEBUG
			printf("Native: CGEventTapCreate() failure.\n");
		#endif

		throwException("Could not create event tap.");
		return; //Naturaly exit so jni exception is thrown.
	}


	CFRunLoopSourceRef event_source = CFMachPortCreateRunLoopSource(NULL, event_port, 0);
	if (event_source == NULL) {
		#ifdef DEBUG
			printf("Native: CFMachPortCreateRunLoopSource() failure.\n");
		#endif

		throwException("Could not create run loop source.");
		return; //Naturaly exit so jni exception is thrown.
	}


	CFRunLoopRef event_loop = CFRunLoopGetCurrent();
	if (event_loop == NULL) {
		#ifdef DEBUG
			printf("Native: CFRunLoopGetCurrent() failure.\n");
		#endif

		throwException("Could not attach to the current run loop.");
		return; //Naturaly exit so jni exception is thrown.
	}

	CFRunLoopAddSource(event_loop, event_source, kCFRunLoopDefaultMode);
	CFRunLoopRun();
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
			printf("Native: JNI_VERSION_1_4 unavailable for use. (default: %X)\n", (int) jni_ret);
		#endif
	}

	//Check and make sure assistive devices is enabled.
	if (! AXAPIEnabled()) {
		#ifdef DEBUG
			printf("Native: Accessibility API is not enabled.\n");
		#endif

		throwException("Please enabled access for assistive devices in the Universal Access section of the System Preferences.");
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}


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
