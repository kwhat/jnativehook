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



#include <signal.h>
#include <pthread.h>

#include <ApplicationServices/ApplicationServices.h>

#include "NativeErrors.h"
#include "JConvertFromNative.h"
#include "NativeThread.h"
#include "OSXButtonCodes.h"
#include "OSXKeyCodes.h"

//Thread and hook handles.
static CFRunLoopRef event_loop;
static CFRunLoopSourceRef event_source;
static pthread_mutex_t hookRunningMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t hookControlMutex;
static pthread_t hookThreadId = 0;

//GlobalScreen object and dispatch id.
static jobject objGlobalScreen = NULL;
static jmethodID idDispatchEvent;

//Java callback classes and constructor id's
static jclass clsKeyEvent, clsMouseEvent, clsMouseWheelEvent;
static jmethodID idKeyEvent, idMouseButtonEvent, idMouseMotionEvent;

static CGEventFlags prev_event_mask = 0;
static unsigned int event_modifiers[] = {
	kCGEventFlagMaskShift,
	kCGEventFlagMaskControl,
	kCGEventFlagMaskAlternate,
	kCGEventFlagMaskCommand
};

static jint doModifierConvert(CGEventFlags event_mask) {
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

static CGEventRef LowLevelProc(CGEventTapProxy UNUSED(proxy), CGEventType type, CGEventRef event, void * UNUSED(refcon)) {
	//We should already be attached to the JVM at this point.  This should only
	//be a formality that causes a NOOP.
	JNIEnv * env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
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
		switch (type) {
			case kCGEventKeyDown:
				keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
				#ifdef DEBUG
					printf("Native: LowLevelProc - Key Pressed (%i)\n", (unsigned int) keysym);
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
					printf("Native: LowLevelProc - Key Released (%i)\n", (unsigned int) keysym);
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
					printf("Native: LowLevelProc - Modifiers Changed (%X %i)\n", (unsigned int) event_mask, (unsigned int) keysym);
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
					printf("Native: LowLevelProc - Button Pressed (%i)\n", (unsigned int) button);
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
					printf("Native: LowLevelProc - Button Released (%i)\n", (unsigned int) button);
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
					printf ("Native: LowLevelProc - Mouse Dragged forwarded to Motion Notified\n");
				#endif

			case kCGEventMouseMoved:
				event_point = CGEventGetLocation(event);
				#ifdef DEBUG
					printf ("Native: LowLevelProc - Motion Notified (%f, %f)\n", event_point.x, event_point.y);
				#endif

				modifiers = doModifierConvert(event_mask);

				//Fire mouse moved event.
				objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseMotionEvent, JK_NATIVE_MOUSE_MOVED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
			break;

			case kCGEventScrollWheel:
				#ifdef DEBUG
					printf ("Native: LowLevelProc - Mouse Wheel Moved (Unimplemented)\n");
				#endif

				/*
				delta = CGEventGetIntegerValueField(event, kCGScrollWheelEventDeltaAxis#);

				kCGScrollWheelEventDeltaAxis1 = 11,
				kCGScrollWheelEventDeltaAxis2 = 12,
				*/
			break;

			case kCGEventTapDisabledByTimeout:
			case kCGEventTapDisabledByUserInput:
				//TODO Should manually stop the thread at this point.

			default:
				#ifdef DEBUG
					printf ("Native: LowLevelProc - Unhandled Event Type\n");
				#endif
			break;
		}

		//Handle any possible JNI issue that may have occured.
		if ((*env)->ExceptionCheck(env) == JNI_TRUE) {
			#ifdef DEBUG
				fprintf(stderr, "LowLevelProc: JNI error occurred!\n");
				(*env)->ExceptionDescribe(env);
			#endif
			(*env)->ExceptionClear(env);
		}
	}

	return noErr;
}

static void CreateJNIGlobals(JNIEnv * env) {
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

	//Class and Constructor for the NativeMouseWheelEvent Object
	jclass clsLocalMouseWheelEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseWheelEvent");
	clsMouseWheelEvent = (*env)->NewGlobalRef(env, clsLocalMouseWheelEvent);
	//FIXME need to change the constructor signature
	//idMouseWheelEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIII)V");
}

static void DestroyJNIGlobals(JNIEnv * env) {
	(*env)->DeleteGlobalRef(env, clsKeyEvent);
	(*env)->DeleteGlobalRef(env, clsMouseEvent);
	(*env)->DeleteGlobalRef(env, clsMouseWheelEvent);
	(*env)->DeleteGlobalRef(env, objGlobalScreen);
}

static void * ThreadProc() {
	pthread_mutex_lock(&hookControlMutex);
	pthread_mutex_lock(&hookRunningMutex);
	int status = EXIT_FAILURE;

	//Attach the current thread to the JVM.
	JNIEnv * env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		//Create all the global references up front to save time in the callbacks.
		CreateJNIGlobals(env);


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
										LowLevelProc,
										NULL
	                                );


		if (event_port != NULL) {
			#ifdef DEBUG
				fprintf(stdout, "Event tap created successfully.\n");
			#endif

			event_source = CFMachPortCreateRunLoopSource(NULL, event_port, 0);
			if (event_source != NULL) {
				#ifdef DEBUG
					printf("Native: CFMachPortCreateRunLoopSource() failure.\n");
				#endif

				event_loop = CFRunLoopGetCurrent();
				if (event_loop != NULL) {
					//Set the exit status.
					status = EXIT_SUCCESS;

					#ifdef DEBUG
						printf("Native: CFRunLoopGetCurrent() failure.\n");
					#endif

					CFRunLoopAddSource(event_loop, event_source, kCFRunLoopDefaultMode);
					pthread_mutex_unlock(&hookControlMutex);

					CFRunLoopRun();

					//Lock back up until we are done processing the exit.
					pthread_mutex_lock(&hookControlMutex);
					CFRunLoopRemoveSource(event_loop, event_source, kCFRunLoopDefaultMode);
				}
			}
		}
		else {
			#ifdef DEBUG
				fprintf(stderr, "Failed to create event port!\n");
			#endif
		}


		//Make sure we clean up the global objects.
		DestroyJNIGlobals(env);

		//Detach the current thread to the JVM.
		if ((*jvm)->DetachCurrentThread(jvm) != JNI_OK) {
			#ifdef DEBUG
				fprintf(stderr, "DetachCurrentThread(jvm, (void **)(&env), NULL) failed!\n");
			#endif
		}
	}
	else {
		//We cant do a whole lot of anything if we cant attach to the current thread.
		#ifdef DEBUG
			fprintf(stderr, "AttachCurrentThread(jvm, (void **)(&env), NULL) failed!\n");
		#endif
	}

	#ifdef DEBUG
		fprintf(stdout, "ThreadProc() has completed.\n");
	#endif

	//Make sure we signal that we have passed any exception throwing code.
	pthread_mutex_unlock(&hookRunningMutex);
	pthread_mutex_unlock(&hookControlMutex);

	pthread_exit((void **) status);
}

int StartNativeThread() {
	int status = EXIT_SUCCESS;

	//Make sure the native thread is not already running.
	if (IsNativeThreadRunning() != true) {
		//Lock the mutex handle for the thread hook.
		pthread_mutex_init(&hookControlMutex, NULL);

		//Check and make sure assistive devices is enabled.
		if (AXAPIEnabled() == true) {
			#ifdef DEBUG
				printf ("Native: Accessibility API is enabled.\n");
			#endif
		}
		else {
			#ifdef DEBUG
				printf("Native: Accessibility API is not enabled.\n");
			#endif

			//throwException("org/jnativehook/NativeHookException", "Please enabled access for assistive devices in the Universal Access section of the System Preferences.");
			//return JNI_ERR; //Naturaly exit so jni exception is thrown.
		}

		//We shall use the default pthread attributes: thread is joinable
		//(not detached) and has default (non real-time) scheduling policy.
		if (pthread_create(&hookThreadId, NULL, ThreadProc, NULL) == 0) {
			#ifdef DEBUG
				printf("Native: MsgLoop() start successful.\n");
			#endif

			//Wait for the thread to start up.
			if (pthread_mutex_lock(&hookControlMutex) == 0) {
				pthread_mutex_unlock(&hookControlMutex);
			}

			#ifdef DEBUG
				printf("Native: MsgLoop() start done.\n");
			#endif
		}
		else {
			#ifdef DEBUG
				printf("Native: MsgLoop() start failure!\n");
			#endif

			return JNI_ERR; //Naturally exit so jni exception is thrown.
		}
	}

	return status;
}

int StopNativeThread() {
	int status = EXIT_SUCCESS;

	if (IsNativeThreadRunning() == true) {
		//Lock the thread.
		pthread_mutex_lock(&hookControlMutex);

		CFRunLoopStop(event_loop);

		//Must unlock to allow the thread to finish cleaning up.
		pthread_mutex_unlock(&hookControlMutex);

		//Wait for the thread to die.
		pthread_join(hookThreadId, (void **) &status);
		printf("Thread Result: %i\n", status);

		//Clean up the mutex.
		pthread_mutex_destroy(&hookControlMutex);
	}

	return status;
}

bool IsNativeThreadRunning() {
	bool isRunning = false;

	//Wait for a lock on the thread.
	if (pthread_mutex_lock(&hookControlMutex) == 0) {
		//Lock Successful.

		if (pthread_mutex_trylock(&hookRunningMutex) == 0) {
			//Lock Successful, we are not running.
			pthread_mutex_unlock(&hookRunningMutex);
		}
		else {
			isRunning = true;
		}

		#ifdef DEBUG
			fprintf(stdout, "IsNativeThreadRunning: Running State (%i)\n", isRunning);
		#endif

		pthread_mutex_unlock(&hookControlMutex);
	}
	else {
		//Lock Failure. This should always be caused by an invalid pointer
		//and/or an uninitialized mutex.

		#ifdef DEBUG
			fprintf(stderr, "IsNativeThreadRunning: Failed to acquire control mutex lock!\n");
		#endif
	}

	return isRunning;
}
