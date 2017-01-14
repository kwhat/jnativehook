/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
 * https://github.com/kwhat/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

#include <ApplicationServices/ApplicationServices.h>

#include <jni.h>

#include "org_jnativehook_GlobalScreen.h"
#include "JConvertToNative.h"
#include "OSXKeyCodes.h"
#include "OSXButtonCodes.h"

//JVM and Screen globals.
JavaVM * jvm = NULL;
jobject objGlobalScreen = NULL;
jmethodID idDispatchEvent;

//Java callback classes and method id's
jclass clsKeyEvent, clsMouseEvent;
jmethodID idKeyEvent, idMouseButtonEvent, idMouseMotionEvent;

//Thread information so we can clean up.
pthread_t hookThreadId = 0;
CGEventFlags prev_event_mask = 0;
CFRunLoopRef event_loop;
unsigned int event_modifiers[] = {
	kCGEventFlagMaskShift,
	kCGEventFlagMaskControl,
	kCGEventFlagMaskAlternate,
	kCGEventFlagMaskCommand
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

void throwException(char * classname, char * message) {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Locate our exception class
	jclass clsException = (*env)->FindClass(env, classname);

	if (clsException != NULL) {
		#ifdef DEBUG
			printf("Native: Exception - %s\n", message);
		#endif

		(*env)->ThrowNew(env, clsException, message);
	}
	else {
		//Unable to find exception class, Terminate with error.
		jniFatalError("Unable to locate exception class.");
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

	if (isModifierMask(kCGEventFlagMaskButtonLeft))		modifiers |= NativeToJModifier(kCGEventFlagMaskButtonLeft);
	if (isModifierMask(kCGEventFlagMaskButtonRight))	modifiers |= NativeToJModifier(kCGEventFlagMaskButtonRight);
	if (isModifierMask(kCGEventFlagMaskButtonCenter))	modifiers |= NativeToJModifier(kCGEventFlagMaskButtonCenter);
	if (isModifierMask(kCGEventFlagMaskXButton1))		modifiers |= NativeToJModifier(kCGEventFlagMaskXButton1);
	if (isModifierMask(kCGEventFlagMaskXButton2))		modifiers |= NativeToJModifier(kCGEventFlagMaskXButton2);

	return modifiers;
}


CGEventRef eventHandlerCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void * refcon) {
	//We should already be attached to the JVM at this point.  This should only
	//be a formality that causes a NOOP.
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Event Data
	CGPoint event_point;
	CGEventTimestamp event_time = CGEventGetTimestamp(event);
	UInt64	keysym, button;
	CGEventFlags event_mask = CGEventGetFlags(event);

	//Java Event Data
	JKeyDatum jkey;
	jint jbutton;
	jint modifiers;

	//Java Event Objects
	jobject objKeyEvent, objMouseEvent;

	// get the event class
	switch (CGEventGetType(event)) {
		case kCGEventKeyDown:
			keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			#ifdef DEBUG
				printf("Native: eventHandlerCallback - Key Pressed (%i)\n", (unsigned int) keysym);
			#endif

			jkey = NativeToJKey(keysym);
			modifiers = doModifierConvert(event_mask);

			//Fire key pressed event.
			objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_PRESSED, (jlong) event_time, modifiers, jkey.rawcode, jkey.keycode, jkey.location);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		break;

		case kCGEventKeyUp:
			keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			#ifdef DEBUG
				printf("Native: eventHandlerCallback - Key Released (%i)\n", (unsigned int) keysym);
			#endif

			jkey = NativeToJKey(keysym);
			modifiers = doModifierConvert(event_mask);

			//Fire key released event.
			objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_RELEASED, (jlong) event_time, modifiers, jkey.rawcode, jkey.keycode, jkey.location);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		break;

		case kCGEventFlagsChanged:
			keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			#ifdef DEBUG
				printf("Native: eventHandlerCallback - Modifiers Changed (%X %i)\n", (unsigned int) event_mask, (unsigned int) keysym);
			#endif

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
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_PRESSED, (jlong) event_time, modifiers, jkey.rawcode, jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				}
				else if (keyup_event_mask & event_modifiers[i]) {
					//Fire key released event.
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_RELEASED, (jlong) event_time, modifiers, jkey.rawcode, jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				}
			}
		break;

		case kCGEventLeftMouseDown:
			button = kVK_LBUTTON;
			setModifierMask(kCGEventFlagMaskButtonLeft);
		goto BUTTONDOWN;

		case kCGEventRightMouseDown:
			button = kVK_RBUTTON;
			setModifierMask(kCGEventFlagMaskButtonRight);
		goto BUTTONDOWN;

		case kCGEventOtherMouseDown:
			button = CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber);

			if (button == kVK_MBUTTON) {
				setModifierMask(kCGEventFlagMaskButtonCenter);
			}
			else if (button == kVK_XBUTTON1) {
				setModifierMask(kCGEventFlagMaskXButton1);
			}
			else if (button == kVK_XBUTTON2) {
				setModifierMask(kCGEventFlagMaskXButton2);
			}

		BUTTONDOWN:
			#ifdef DEBUG
				printf("Native: eventHandlerCallback - Button Pressed (%i)\n", (unsigned int) button);
			#endif

			event_point = CGEventGetLocation(event);
			jbutton = NativeToJButton(button);
			modifiers = doModifierConvert(event_mask);

			//Fire mouse pressed event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, JK_NATIVE_MOUSE_PRESSED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y, jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case kCGEventLeftMouseUp:
			button = kVK_LBUTTON;
			unsetModifierMask(kCGEventFlagMaskButtonLeft);
		goto BUTTONUP;

		case kCGEventRightMouseUp:
			button = kVK_RBUTTON;
			unsetModifierMask(kCGEventFlagMaskButtonRight);
		goto BUTTONUP;

		case kCGEventOtherMouseUp:
			button = CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber);

			if (button == kVK_MBUTTON) {
				unsetModifierMask(kCGEventFlagMaskButtonCenter);
			}
			else if (button == kVK_XBUTTON1) {
				unsetModifierMask(kCGEventFlagMaskXButton1);
			}
			else if (button == kVK_XBUTTON2) {
				unsetModifierMask(kCGEventFlagMaskXButton2);
			}

		BUTTONUP:
			#ifdef DEBUG
				printf("Native: eventHandlerCallback - Button Released (%i)\n", (unsigned int) button);
			#endif

			event_point = CGEventGetLocation(event);
			jbutton = NativeToJButton(button);
			modifiers = doModifierConvert(event_mask);

			//Fire mouse released event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, JK_NATIVE_MOUSE_RELEASED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y, jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;


		case kCGEventLeftMouseDragged:
		case kCGEventRightMouseDragged:
		case kCGEventOtherMouseDragged:
			//Call mouse move events for now.  Adding this functionality to other systems will be difficult.
			#ifdef DEBUG
				printf ("Native: eventHandlerCallback - Mouse Dragged forwarded to Motion Notified\n");
			#endif

		case kCGEventMouseMoved:
			event_point = CGEventGetLocation(event);
			#ifdef DEBUG
				printf ("Native: eventHandlerCallback - Motion Notified (%f,%f)\n", event_point.x, event_point.y);
			#endif

			modifiers = doModifierConvert(event_mask);

			//Fire mouse moved event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseMotionEvent, JK_NATIVE_MOUSE_MOVED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case kCGEventScrollWheel:
			#ifdef DEBUG
				printf ("Native: eventHandlerCallback - Mouse Wheel Moved (Unimplemented)\n");
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
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
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
		#ifdef DEBUG
			printf("Native: CFPreferencesCopyAppValue failure\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Could not determine the keyboard auto repeat rate.");
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
		#ifdef DEBUG
			printf("Native: CFPreferencesCopyAppValue failure\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Could not determine the keyboard auto repeat delay.");
	}

	return (jlong) interval;
}

void MsgLoop() {
	JNIEnv * env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) != 0) {
		#ifdef DEBUG
			printf("Native: AttachCurrentThread(jvm, (void **)(&env), NULL) failed\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Failed to attach to the current Java thread");
		return; //Naturally exit so JNI exception is thrown.
	}

	//Class and getInstance method id for the GlobalScreen Object
	jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
	jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");

	//A reference to the GlobalScreen Object
	jobject objScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);
	objGlobalScreen = (*env)->NewGlobalRef(env, objScreen);

	//Get the ID of the GlobalScreen Objects dispatch event method.
	idDispatchEvent = (*env)->GetMethodID(env, clsGlobalScreen, "dispatchEvent", "(Lorg/jnativehook/NativeInputEvent;)V");

	//Class and Constructor for the NativeKeyEvent Object
	jclass clsLocalKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
	clsKeyEvent = (*env)->NewGlobalRef(env, clsLocalKeyEvent);
	idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

	//Class and Constructor for the NativeMouseEvent Object
	jclass clsLocalMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
	clsMouseEvent = (*env)->NewGlobalRef(env, clsLocalMouseEvent);
	idMouseButtonEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");
	idMouseMotionEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIII)V");


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

		throwException("org/jnativehook/NativeHookException", "Could not create event tap.");
		return; //Naturaly exit so jni exception is thrown.
	}


	CFRunLoopSourceRef event_source = CFMachPortCreateRunLoopSource(NULL, event_port, 0);
	if (event_source == NULL) {
		#ifdef DEBUG
			printf("Native: CFMachPortCreateRunLoopSource() failure.\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Could not create run loop source.");
		return; //Naturaly exit so jni exception is thrown.
	}


	event_loop = CFRunLoopGetCurrent();
	if (event_loop == NULL) {
		#ifdef DEBUG
			printf("Native: CFRunLoopGetCurrent() failure.\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Could not attach to the current run loop.");
		return; //Naturaly exit so jni exception is thrown.
	}

	CFRunLoopAddSource(event_loop, event_source, kCFRunLoopDefaultMode);
	CFRunLoopRun();
	CFRunLoopRemoveSource(event_loop, event_source, kCFRunLoopDefaultMode);

	//Make sure we clean up the global objects.
	(*env)->DeleteGlobalRef(env, clsKeyEvent);
	(*env)->DeleteGlobalRef(env, clsMouseEvent);
	(*env)->DeleteGlobalRef(env, objGlobalScreen);

	#ifdef DEBUG
		printf("Native: MsgLoop() stop successful.\n");
	#endif
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

		throwException("org/jnativehook/NativeHookException", "Please enabled access for assistive devices in the Universal Access section of the System Preferences.");
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}


	if( pthread_create( &hookThreadId, NULL, (void *) &MsgLoop, NULL) ) {
		#ifdef DEBUG
			printf("Native: MsgLoop() start failure.\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Could not create message loop thread.");
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
	//Terminate the RunLoop.
	CFRunLoopStop(event_loop);

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
