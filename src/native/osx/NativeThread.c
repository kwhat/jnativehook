/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

#include <pthread.h>

#include <ApplicationServices/ApplicationServices.h>

#include "NativeErrors.h"
#include "NativeGlobals.h"
#include "NativeHelpers.h"
#include "NativeThread.h"
#include "NativeToJava.h"
#include "OSXInputHelpers.h"

//Exception global for thread initialization.
static Exception thread_ex;

//Click count globals
static unsigned short click_count = 0;
static CGEventTimestamp click_time = 0;
static bool mouse_dragged = false;

//Thread and hook handles.
static CFRunLoopRef event_loop;
static CFRunLoopSourceRef event_source;
static pthread_mutex_t hookRunningMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t hookControlMutex;
static pthread_t hookThreadId = 0;

static CGEventFlags prev_event_mask, diff_event_mask, keyup_event_mask;
static const CGEventFlags key_event_mask = kCGEventFlagMaskShift + kCGEventFlagMaskControl + kCGEventFlagMaskAlternate + kCGEventFlagMaskCommand;

static CGEventRef LowLevelProc(CGEventTapProxy UNUSED(proxy), CGEventType type, CGEventRef event, void * UNUSED(refcon)) {
	JNIEnv * env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		//Event Data
		CGPoint event_point;
		//Make sure to convert from nanoseconds to milliseconds
		CGEventTimestamp event_time = CGEventGetTimestamp(event) / 1000000;
		UInt64	keysym, button;
		CGEventFlags event_mask = CGEventGetFlags(event);

		//Java Event Data
		JKeyDatum jkey;
		jint jbutton;
		jint scrollType, scrollAmount, wheelRotation;
		jint modifiers;
		CFStringRef keytxt;

		//Java Event Objects
		jobject objKeyEvent, objMouseEvent, objMouseWheelEvent;

		// get the event class
		switch (type) {
			case kCGEventKeyDown:
			EVENT_KEYDOWN:
				keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
				#ifdef DEBUG
				fprintf(stdout, "LowLevelProc(): Key Pressed (%i)\n", (unsigned int) keysym);
				#endif

				jkey = NativeToJKey(keysym);
				modifiers = NativeToJEventMask(GetModifiers());

				//Fire key pressed event.
				objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED, (jlong) event_time, modifiers, jkey.rawcode, jkey.keycode, org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED, jkey.location);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				
				keytxt = KeyCodeToString(jkey.rawcode, GetModifiers());
				if (CFStringGetLength(keytxt) == 1) {
					//Fire key pressed event.
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED, (jlong) event_time, modifiers, jkey.rawcode, org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED, (jchar) CFStringGetCharacterAtIndex(keytxt, 0), jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				}
				break;

			case kCGEventKeyUp:
			EVENT_KEYUP:
				keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
				#ifdef DEBUG
				fprintf(stdout, "LowLevelProc(): Key Released (%i)\n", (unsigned int) keysym);
				#endif

				jkey = NativeToJKey(keysym);
				modifiers = NativeToJEventMask(GetModifiers());

				//Fire key released event.
				objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED, (jlong) event_time, modifiers, jkey.rawcode, jkey.keycode, jkey.location);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				break;

			case kCGEventFlagsChanged:
				#ifdef DEBUG
				fprintf(stdout, "LowLevelProc(): Modifiers Changed (0x%X)\n", (unsigned int) event_mask);
				#endif

				/*	Because of the way apple handles modifiers we need to track 
					the key changes and handle our own keyup and down events.
					Outline of what is happening on the next 3 lines.
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

					CGEventFlags diff_event_mask = prev_event_mask ^ event_mask;
					CGEventFlags keydown_event_mask = prev_event_mask | diff_event_mask;
					CGEventFlags keyup_event_mask = event_mask & diff_event_mask;
				 */

				prev_event_mask = GetModifiers() & 0xFFFF0000;
				diff_event_mask = prev_event_mask ^ (event_mask & 0xFFFF0000);
				keyup_event_mask = (event_mask & 0xFFFF0000) & diff_event_mask;

				//Update the previous event mask.
				UnsetModifierMask(prev_event_mask);
				SetModifierMask(event_mask & 0xFFFF0000);

				if (diff_event_mask & key_event_mask && keyup_event_mask & key_event_mask) {
					//Process as a key pressed event.
					goto EVENT_KEYDOWN;
				}
				else if (diff_event_mask & key_event_mask && keyup_event_mask ^ key_event_mask) {
					//Process as a key released event.
					goto EVENT_KEYUP;
				}
				break;

			case kCGEventLeftMouseDown:
				button = kVK_LBUTTON;
				SetModifierMask(kCGEventFlagMaskButtonLeft);
				goto BUTTONDOWN;

			case kCGEventRightMouseDown:
				button = kVK_RBUTTON;
				SetModifierMask(kCGEventFlagMaskButtonRight);
				goto BUTTONDOWN;

			case kCGEventOtherMouseDown:
				button = CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber);

				if (button == kVK_MBUTTON) {
					SetModifierMask(kCGEventFlagMaskButtonCenter);
				}
				else if (button == kVK_XBUTTON1) {
					SetModifierMask(kCGEventFlagMaskXButton1);
				}
				else if (button == kVK_XBUTTON2) {
					SetModifierMask(kCGEventFlagMaskXButton2);
				}

			BUTTONDOWN:
				#ifdef DEBUG
				fprintf(stdout, "LowLevelProc(): Button Pressed (%i)\n", (unsigned int) button);
				#endif

				//Track the number of clicks.
				#ifdef DEBUG
				fprintf(stdout, "LowLevelProc(): Click Time (%lli)\n", (event_time - click_time));
				#endif

				if ((long) (event_time - click_time) <= GetMultiClickTime()) {
					click_count++;
				}
				else {
					click_count = 1;
				}
				click_time = event_time;

				event_point = CGEventGetLocation(event);
				jbutton = NativeToJButton(button);
				modifiers = NativeToJEventMask(GetModifiers());

				//Fire mouse pressed event.
				objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y, (jint) click_count, jbutton);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
				break;

			case kCGEventLeftMouseUp:
				button = kVK_LBUTTON;
				UnsetModifierMask(kCGEventFlagMaskButtonLeft);
				goto BUTTONUP;

			case kCGEventRightMouseUp:
				button = kVK_RBUTTON;
				UnsetModifierMask(kCGEventFlagMaskButtonRight);
				goto BUTTONUP;

			case kCGEventOtherMouseUp:
				button = CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber);

				if (button == kVK_MBUTTON) {
					UnsetModifierMask(kCGEventFlagMaskButtonCenter);
				}
				else if (button == kVK_XBUTTON1) {
					UnsetModifierMask(kCGEventFlagMaskXButton1);
				}
				else if (button == kVK_XBUTTON2) {
					UnsetModifierMask(kCGEventFlagMaskXButton2);
				}

			BUTTONUP:
				#ifdef DEBUG
				fprintf(stdout, "LowLevelProc(): Button Released (%i)\n", (unsigned int) button);
				#endif

				event_point = CGEventGetLocation(event);
				jbutton = NativeToJButton(button);
				modifiers = NativeToJEventMask(GetModifiers());

				//Fire mouse released event.
				objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y, (jint) click_count, jbutton);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);

				if (mouse_dragged != true) {
					//Fire mouse clicked event.
					objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y, (jint) click_count, jbutton);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
				}
				break;


			case kCGEventLeftMouseDragged:
			case kCGEventRightMouseDragged:
			case kCGEventOtherMouseDragged:
				event_point = CGEventGetLocation(event);
				//Call mouse move events for now.  Adding this functionality to other systems will be difficult.
				#ifdef DEBUG
				fprintf(stdout, "LowLevelProc(): Motion Notified (%f, %f)\n", event_point.x, event_point.y);
				#endif

				//Reset the clickcount
				if (click_count != 0 && (long) (event_time - click_time) > GetMultiClickTime()) {
					click_count = 0;
				}
				modifiers = NativeToJEventMask(GetModifiers());

				//Set the mouse draged flag
				mouse_dragged = true;

				//Fire mouse dragged event.
				objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseMotionEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y, (jint) click_count);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
				break;

			case kCGEventMouseMoved:
				event_point = CGEventGetLocation(event);
				#ifdef DEBUG
				fprintf(stdout, "LowLevelProc(): Motion Notified (%f, %f)\n", event_point.x, event_point.y);
				#endif

				//Reset the clickcount
				if (click_count != 0 && (long) (event_time - click_time) > GetMultiClickTime()) {
					click_count = 0;
				}
				modifiers = NativeToJEventMask(GetModifiers());

				//Set the mouse draged flag
				mouse_dragged = false;

				//Fire mouse moved event.
				objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseMotionEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y, (jint) click_count);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
				break;

			case kCGEventScrollWheel:
				event_point = CGEventGetLocation(event);
				
				//TODO Figure out of kCGScrollWheelEventDeltaAxis2 causes mouse events with zero rotation.
				if (CGEventGetIntegerValueField(event, kCGScrollWheelEventIsContinuous) == 0) {
					scrollType = (jint)  org_jnativehook_mouse_NativeMouseWheelEvent_WHEEL_UNIT_SCROLL;
				}
				else {
					scrollType = (jint)  org_jnativehook_mouse_NativeMouseWheelEvent_WHEEL_BLOCK_SCROLL;
				}
				
				//Scrolling data uses a fixed-point 16.16 signed integer format (Ex: 1.0 = 0x00010000)
				wheelRotation = (jint) CGEventGetIntegerValueField(event, kCGScrollWheelEventDeltaAxis1) * -1;

				//TODO Figure out the scroll wheel amounts are correct.  I suspect that Apples Java implementaion
				//maybe reporting a static "1" inaccuratly.
				scrollAmount = (jint) CGEventGetIntegerValueField(event, kCGScrollWheelEventPointDeltaAxis1) * -1;

				#ifdef DEBUG
				fprintf(stdout, "LowLevelProc(): Mouse Wheel Moved (%i, %i, %i)\n", (int) scrollType, (int) scrollAmount, (int) wheelRotation);
				#endif

				//Track the number of clicks.
				if ((long) (event_time - click_time) <= GetMultiClickTime()) {
					click_count++;
				}
				else {
					click_count = 1;
				}
				click_time = event_time;

				modifiers = NativeToJEventMask(GetModifiers());

				//Fire mouse wheel event.
				objMouseWheelEvent = (*env)->NewObject(env, clsMouseWheelEvent, idMouseWheelEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL, (jlong) event_time, modifiers, (jint) event_point.x, (jint) event_point.y, (jint) click_count, scrollType, scrollAmount, wheelRotation);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseWheelEvent);
				break;

			//case kCGEventTapDisabledByTimeout:
			//case kCGEventTapDisabledByUserInput:
				//TODO Should manually stop the thread at this point.

			#ifdef DEBUG
			default:
				fprintf(stderr, "LowLevelProc(): Unhandled Event Type!\n");
			break;
			#endif
		}

		//Handle any possible JNI issue that may have occured.
		if ((*env)->ExceptionCheck(env) == JNI_TRUE) {
			#ifdef DEBUG
			fprintf(stderr, "LowLevelProc(): JNI error occurred!\n");
			(*env)->ExceptionDescribe(env);
			#endif
			(*env)->ExceptionClear(env);
		}
	}

	return noErr;
}

static void * ThreadProc(void * arg) {
	pthread_mutex_lock(&hookRunningMutex);

	int * status = (int *) arg;
	*status = RETURN_FAILURE;
	JNIEnv * env = NULL;

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
		fprintf(stdout, "ThreadProc(): Event tap created successfully.\n");
		#endif

		event_source = CFMachPortCreateRunLoopSource(NULL, event_port, 0);
		if (event_source != NULL) {
			#ifdef DEBUG
			fprintf(stdout, "ThreadProc(): CFMachPortCreateRunLoopSource() success.\n");
			#endif

			event_loop = CFRunLoopGetCurrent();
			if (event_loop != NULL) {
				#ifdef DEBUG
				fprintf(stdout, "ThreadProc(): CFRunLoopGetCurrent() success.\n");
				#endif

				if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
					#ifdef DEBUG
					fprintf(stdout, "ThreadProc(): Attached to JVM successful.\n");
					#endif

					//Set the exit status.
					*status = RETURN_SUCCESS;

					CFRunLoopAddSource(event_loop, event_source, kCFRunLoopDefaultMode);
					pthread_mutex_unlock(&hookControlMutex);

					CFRunLoopRun();

					//Lock back up until we are done processing the exit.
					pthread_mutex_lock(&hookControlMutex);
					CFRunLoopRemoveSource(event_loop, event_source, kCFRunLoopDefaultMode);
				}
				else {
					#ifdef DEBUG
					fprintf(stderr, "ThreadProc(): AttachCurrentThread() failed!\n");
					#endif

					thread_ex.class = NATIVE_HOOK_EXCEPTION;
					thread_ex.message = "Failed to attach the native thread to the virtual machine";
				}
			}
			else {
				#ifdef DEBUG
				fprintf(stderr, "ThreadProc(): CFRunLoopGetCurrent() failure.\n");
				#endif

				thread_ex.class = NATIVE_HOOK_EXCEPTION;
				thread_ex.message = "Failed to attach to the run loop";
			}
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "ThreadProc(): CFMachPortCreateRunLoopSource() failure.\n");
			#endif

			thread_ex.class = NATIVE_HOOK_EXCEPTION;
			thread_ex.message = "Failed to create run loop";
		}

		//Stop the CFMachPort from receiving any more messages.
		CFMachPortInvalidate(event_port);
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "ThreadProc(): Failed to create event port!\n");
		#endif

		thread_ex.class = NATIVE_HOOK_EXCEPTION;
		thread_ex.message = "Failed to create event port";
	}

	//Detach this thread from the JVM
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		(*jvm)->DetachCurrentThread(jvm);

		#ifdef DEBUG
		fprintf(stdout, "ThreadProc(): Detach from JVM successful.\n");
		#endif
	}

	#ifdef DEBUG
	fprintf(stdout, "ThreadProc(): complete.\n");
	#endif

	//Make sure we signal that we have passed any exception throwing code.
	pthread_mutex_unlock(&hookRunningMutex);
	pthread_mutex_unlock(&hookControlMutex);

	pthread_exit(status);
}

int StartNativeThread() {
	int status = RETURN_FAILURE;

	//Make sure the native thread is not already running.
	if (IsNativeThreadRunning() != true) {
		//Lock the mutex handle for the thread hook.
		pthread_mutex_init(&hookControlMutex, NULL);

		//Create all the global references up front to save time in the callbacks.
		if (CreateJNIGlobals() == RETURN_SUCCESS) {
			//Check and make sure assistive devices is enabled.
			if (AXAPIEnabled() == true) {
				#ifdef DEBUG
				printf ("Native: Accessibility API is enabled.\n");
				#endif

				//We shall use the default pthread attributes: thread is joinable
				//(not detached) and has default (non real-time) scheduling policy.
				pthread_mutex_lock(&hookControlMutex);
				if (pthread_create(&hookThreadId, NULL, ThreadProc, malloc(sizeof(int))) == 0) {
					#ifdef DEBUG
					fprintf(stdout, "StartNativeThread(): start successful.\n");
					#endif

					//Wait for the thread to start up.
					if (pthread_mutex_lock(&hookControlMutex) == 0) {
						pthread_mutex_unlock(&hookControlMutex);
					}

					//Handle any possible JNI issue that may have occured.
					if (IsNativeThreadRunning()) {
						#ifdef DEBUG
						fprintf(stdout, "StartNativeThread(): initialization successful.\n");
						#endif

						status = RETURN_SUCCESS;
					}
					else {
						#ifdef DEBUG
						fprintf(stderr, "StartNativeThread(): initialization failure!\n");
						#endif

						//Wait for the thread to die.
						void * thread_status;
						pthread_join(hookThreadId, (void *) &thread_status);
						status = *(int *) thread_status;
						free(thread_status);

						#ifdef DEBUG
						fprintf(stderr, "StartNativeThread(): Thread Result (%i)\n", status);
						#endif

						if (thread_ex.class != NULL && thread_ex.message != NULL)  {
							ThrowException(thread_ex.class, thread_ex.message);
						}
					}
				}
				else {
					#ifdef DEBUG
					fprintf(stderr, "StartNativeThread(): start failure!\n");
					#endif

					ThrowException(NATIVE_HOOK_EXCEPTION, "Native thread start failure");
				}
			}
			else {
				#ifdef DEBUG
				printf("Native: Accessibility API is not enabled.\n");
				#endif
				
				ThrowException(NATIVE_HOOK_EXCEPTION, "Please enabled access for assistive devices in the Universal Access section of the System Preferences");
			}

			
		}
		#ifdef DEBUG
		else {
			//We cant do a whole lot of anything if we cant create JNI globals.
			//Any exceptions are thrown by CreateJNIGlobals().
			fprintf(stderr, "StartNativeThread(): CreateJNIGlobals() failed!\n");
		}
		#endif
	}

	return status;
}

int StopNativeThread() {
	int status = RETURN_FAILURE;

	if (IsNativeThreadRunning() == true) {
		if (hookThreadId != pthread_self()) {
			//Lock the thread.
			pthread_mutex_lock(&hookControlMutex);

			CFRunLoopStop(event_loop);

			//Must unlock to allow the thread to finish cleaning up.
			pthread_mutex_unlock(&hookControlMutex);

			//Wait for the thread to die.
			void * thread_status;
			pthread_join(hookThreadId, &thread_status);
			status = *(int *) thread_status;
			free(thread_status);

			#ifdef DEBUG
			fprintf(stdout, "StopNativeThread(): Thread Result (%i)\n", *(int *) thread_status);
			#endif

			//Destroy all created globals.
			#ifdef DEBUG
			if (DestroyJNIGlobals() == RETURN_FAILURE) {
				//Leaving dangling global references will leak a small amout of memory
				//but because there is nothing that can be done about it at this point
				//an exception will not be thrown.
				fprintf(stderr, "StopNativeThread(): DestroyJNIGlobals() failed!\n");
			}
			#else
			DestroyJNIGlobals();
			#endif

			//Clean up the mutex.
			pthread_mutex_destroy(&hookControlMutex);
		}
		else {
			ThrowException(NATIVE_HOOK_EXCEPTION, "Native thread stop failure");
		}
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
		fprintf(stdout, "IsNativeThreadRunning(): Running State (%i)\n", isRunning);
		#endif

		pthread_mutex_unlock(&hookControlMutex);
	}
	#ifdef DEBUG
	else {
		//Lock Failure. This should always be caused by an invalid pointer
		//and/or an uninitialized mutex.
		fprintf(stderr, "IsNativeThreadRunning(): Failed to acquire control mutex lock!\n");
	}
	#endif

	return isRunning;
}

bool IsNativeDispatchThread() {
	return hookThreadId == pthread_self();
}
