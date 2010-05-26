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

static EventHandlerRef eventHandler;

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

int doModifierConvert(int event_mask) {
	int modifiers = 0;

	if (event_mask & shiftKey)				modifiers |= NativeToJModifier(shiftKey);
	if (event_mask & rightShiftKey)			modifiers |= NativeToJModifier(rightShiftKey);

	if (event_mask & controlKey)			modifiers |= NativeToJModifier(controlKey);
	if (event_mask & rightControlKey)		modifiers |= NativeToJModifier(rightControlKey);

	if (event_mask & optionKey)				modifiers |= NativeToJModifier(optionKey);
	if (event_mask & rightOptionKey)		modifiers |= NativeToJModifier(rightOptionKey);

	if (event_mask & cmdKey)				modifiers |= NativeToJModifier(cmdKey);
	//if (event_mask & rightCmdKey)			modifiers |= NativeToJModifier(rightCmdKey);

	return modifiers;
}

static OSStatus EventHandlerCallback(EventHandlerCallRef caller, EventRef event, void* refcon) {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Class and getInstance method id for the GlobalScreen Object
	jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
	jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");

	//A reference to the GlobalScreen Object
	jobject objGlobalScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);

	//Event Data
	Point event_pt;
	EventMouseButton button;
	EventTime event_time = GetEventTime(event);
	UInt32	keysym, event_mask;

	JKeyDatum jkey;
	jint jbutton;
	jint modifiers;
	jclass clsKeyEvent, clsMouseEvent;
	jmethodID idKeyEvent, idMouseEvent;
	jobject objKeyEvent, objMouseEvent;


	// get the event class
	switch (GetEventClass(event)) {
		case kEventClassKeyboard:

			// get the keyboard event type
			switch (GetEventKind(event)) {
				case kEventRawKeyDown:
				case kEventRawKeyRepeat:
					GetEventParameter(event, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &keysym);
					#ifdef DEBUG
						printf("Native: EventHandlerCallback - Key pressed (%i)\n", keysym);
					#endif

					//Class and Constructor for the NativeKeyEvent Object
					clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
					idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

					jkey = NativeToJKey(keysym);
					modifiers = doModifierConvert(getModifierMask());

					//Fire key pressed event.
					jmethodID idFireKeyPressed = (*env)->GetMethodID(env, clsGlobalScreen, "fireKeyPressed", "(Lorg/jnativehook/keyboard/NativeKeyEvent;)V");
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_PRESSED, (jlong) event_time, modifiers, (jint) keysym, jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idFireKeyPressed, objKeyEvent);
				break;

				case kEventRawKeyUp:
					GetEventParameter(event, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &keysym);
					#ifdef DEBUG
						printf("Native: EventHandlerCallback - Key released (%i)\n", keysym);
					#endif

					//Class and Constructor for the NativeKeyEvent Object
					clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
					idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

					jkey = NativeToJKey(keysym);
					modifiers = doModifierConvert(getModifierMask());

					//Fire key released event.
					jmethodID idFireKeyReleased = (*env)->GetMethodID(env, clsGlobalScreen, "fireKeyReleased", "(Lorg/jnativehook/keyboard/NativeKeyEvent;)V");
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_RELEASED, (jlong) event_time, modifiers, keysym, jkey.keycode, jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idFireKeyReleased, objKeyEvent);
				break;

				case kEventRawKeyModifiersChanged:
					GetEventParameter(event, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &event_mask);
					#ifdef DEBUG
						printf("Native: EventHandlerCallback - Modifiers changed (%X)\n", event_mask);
					#endif


				break;

				default:
					#ifdef DEBUG
						printf ("Native: EventHandlerCallback - Unhandled Event Type\n");
					#endif
				break;
			}
		break;


		case kEventClassMouse:

			// get the type or mouse event
			switch ( GetEventKind( event ) ) {
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
					GetEventParameter(event, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(Point), NULL, &pt);
					fprintf(stderr, "location - %d, %d - ", pt.v, pt.h);
				break;

				case kEventMouseDragged:
					#ifdef DEBUG
						printf ("Native: EventHandlerCallback - MouseDragged (Unimplemented)\n");
					#endif
				break;

				case kEventMouseWheelMoved:
					#ifdef DEBUG
						printf ("Native: EventHandlerCallback - MouseWheelMoved (Unimplemented)\n");
					#endif
				break;

				default:
					#ifdef DEBUG
						printf ("Native: EventHandlerCallback - Unhandled Event Type\n");
					#endif
				break;
			}
		break;


		default:

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

OSStatus InitEventHandlers(void) {
	EventTypeSpec	kEvents[] = {
		{ kEventClassCommand, kEventCommandUpdateStatus },
		{ kEventClassKeyboard, kEventRawKeyDown },
		{ kEventClassKeyboard, kEventRawKeyUp },
		{ kEventClassKeyboard, kEventRawKeyRepeat },
		{ kEventClassKeyboard, kEventRawKeyModifiersChanged },
		{ kEventClassMouse, kEventMouseDown },
		{ kEventClassMouse, kEventMouseUp },
		{ kEventClassMouse, kEventMouseMoved },
		{ kEventClassMouse, kEventMouseDragged },
		{ kEventClassMouse, kEventMouseWheelMoved }
		/*
		{ kEventClassTablet, kEventTabletPoint },
		{ kEventClassTablet, kEventTabletProximity }
		*/
	};

	/*
	EventTypeSpec	kCmdEvents[] = {
		{ kEventClassCommand, kEventCommandProcess }
	};

	EventTypeSpec kSuspendResumeEvents[] = {
		{ kEventClassApplication,kEventAppActivated },
		{ kEventClassApplication,kEventAppDeactivated }
	};
	*/

	//FIXME This should return something??
	InstallEventHandler(GetEventMonitorTarget(), EventHandlerCallback, GetEventTypeCount( kEvents ), kEvents, 0, &eventHandler);

	return noErr;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	SInt32 interval = -1;

	CFTypeRef pref_val = CFPreferencesCopyAppValue(CFSTR("KeyRepeat"), kCFPreferencesCurrentApplication);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		CFNumberGetValue((CFNumberRef)pref_val, kCFNumberSInt32Type, &interval);
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
	SInt32 interval = -1;

	CFTypeRef pref_val = CFPreferencesCopyAppValue(CFSTR("InitialKeyRepeat"), kCFPreferencesCurrentApplication);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		CFNumberGetValue((CFNumberRef)pref_val, kCFNumberSInt32Type, &interval);
		#ifdef DEBUG
			printf("Native: CFPreferencesCopyAppValue successful (delay: %i)\n", interval);
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
				printf("Native: LMGetKeyThresh successful (delay: %i)\n", interval);
			#endif
		}
	}

	return (jlong) interval;
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

	//Check and make sure assistive devices is enabled.
	if (! AXAPIEnabled()) {
		#ifdef DEBUG
			printf("Native: Accessibility API is not enabled.\n");
		#endif

		throwException("Please enabled access for assistive devices in the Universal Access section of the System Preferences.");
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}

	//Call listener
	bRunning = true;

	InitEventHandlers();

	if( pthread_create( &hookThreadId, NULL, (void *) &RunApplicationEventLoop, NULL) ) {
		#ifdef DEBUG
			printf("Native: RunApplicationEventLoop() start failure.\n");
		#endif

		throwException("Could not create message loop thread.");
		return JNI_ERR; //Naturaly exit so jni exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf("Native: RunApplicationEventLoop() start successful.\n");
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
