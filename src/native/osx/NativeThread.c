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

#include <pthread.h>
#include <sys/time.h>

#include <ApplicationServices/ApplicationServices.h>

#include "NativeErrors.h"
#include "NativeGlobals.h"
#include "NativeHelpers.h"
#include "NativeThread.h"
#include "NativeToJava.h"
#include "OSXInputHelpers.h"

// Exception global for thread initialization.
static Exception thread_ex;

// GlobalScreen object.
static jobject objGlobalScreen;

// Click count globals.
static unsigned short click_count = 0;
static CGEventTimestamp click_time = 0;
static bool mouse_dragged = false;

// Thread and hook handles.
static CFRunLoopSourceRef sourceMsgPort;

static CFRunLoopRef event_loop;
static CFRunLoopSourceRef event_source;

static pthread_mutex_t hookRunningMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t hookControlMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t hookThreadId = 0;

static CGEventFlags prev_event_mask, diff_event_mask, keyup_event_mask;
static const CGEventFlags key_event_mask = kCGEventFlagMaskShift + kCGEventFlagMaskControl + kCGEventFlagMaskAlternate + kCGEventFlagMaskCommand;



typedef struct {
	CGEventRef event;
	UniChar buffer[4];
	UniCharCount length;
} TISMessage;

static TISMessage data = {
	.event = NULL,
	.buffer = { 0x00 },
	.length = 0
};

void MessagePortProc(void *info) {
	TISMessage *data = (TISMessage *) info;

	if (data->event != NULL) {
		// Preform Unicode lookup.
		KeyCodeToString(data->event, sizeof(data->buffer), &(data->length), data->buffer);
	}

	// Unlock the control mutex to signal that we have finished on the main run loop.
	pthread_mutex_unlock(&hookControlMutex);
}

static void StartMessagePortRunLoop() {
	CFRunLoopSourceContext context = {
		.version = 0,
		.info = &data,
		.retain = NULL,
		.release = NULL,
		.copyDescription = NULL,
		.equal = NULL,
		.hash = NULL,
		.schedule = NULL,
		.cancel = NULL,

		.perform = MessagePortProc
	};

	sourceMsgPort = CFRunLoopSourceCreate(kCFAllocatorDefault, 0, &context);
	CFRunLoopAddSource(CFRunLoopGetMain(), sourceMsgPort, kCFRunLoopDefaultMode);

	#ifdef DEBUG
	fprintf(stdout, "StartMessagePortRunLoop(): start successful.\n");
	#endif
}

static void StopMessagePortRunLoop() {
	if (CFRunLoopContainsSource(CFRunLoopGetMain(), sourceMsgPort, kCFRunLoopDefaultMode)) {
		CFRunLoopRemoveSource(CFRunLoopGetMain(), sourceMsgPort, kCFRunLoopDefaultMode);
		CFRelease(sourceMsgPort);
	}

	sourceMsgPort = NULL;

	#ifdef DEBUG
	fprintf(stderr, "StopMessagePortRunLoop(): completed.\n");
	#endif
}

static void CallbackProc(CFRunLoopObserverRef UNUSED(observer), CFRunLoopActivity activity, void *UNUSED(info)) {
	switch (activity) {
		case kCFRunLoopEntry:
			pthread_mutex_lock(&hookRunningMutex);
			pthread_mutex_unlock(&hookControlMutex);
			break;

		case kCFRunLoopExit:
			//pthread_mutex_lock(&hookControlMutex);
			pthread_mutex_unlock(&hookRunningMutex);
			break;

		#ifdef DEBUG
		default:
			fprintf(stderr, "CallbackProc(): Unhandled Activity: 0x%X\n", (unsigned int) activity);
			break;
		#endif
	}
}

static CGEventRef LowLevelProc(CGEventTapProxy UNUSED(proxy), CGEventType type, CGEventRef event, void *UNUSED(refcon)) {
	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		// Check and make sure the thread is stull running to avoid the
		// potential crash associated with late event arrival.  This code is
		// guaranteed to run after all thread start.
		if (pthread_mutex_trylock(&hookRunningMutex) != 0) {
			// Event data.
			CGPoint event_point;

			struct timeval  time_val;
			gettimeofday(&time_val, NULL);
			jlong event_time = (time_val.tv_sec * 1000) + (time_val.tv_usec / 1000);

			UInt64	keysym, button;
			CGEventFlags event_mask = CGEventGetFlags(event);

			// Java event data.
			JKeyDatum jkey;
			jint jbutton;
			jint jscrollType, jscrollAmount, jwheelRotation;
			jint jmodifiers;

			// Java event objects.
			jobject objKeyEvent, objMouseEvent, objMouseWheelEvent;

			// Get the event class.
			switch (type) {
				case kCGEventKeyDown:
				EVENT_KEYDOWN:
					keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Key Pressed (%i)\n", (unsigned int) keysym);
					#endif

					jkey = NativeToJKey(keysym);
					jmodifiers = NativeToJEventMask(GetModifiers());

					// Fire key pressed event.
					objKeyEvent = (*env)->NewObject(
											env,
											clsKeyEvent,
											idKeyEvent,
											org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED,
											event_time,
											jmodifiers,
											jkey.rawcode,
											jkey.keycode,
											org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
											jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
					(*env)->DeleteLocalRef(env, objKeyEvent);


					// Lookup the Unicode representation for this event.
					CFRunLoopSourceContext context = { .version = 0 };
					CFRunLoopSourceGetContext(sourceMsgPort, &context);

					// Get the run loop context info pointer.
					TISMessage *info = (TISMessage *) context.info;

					// Set the event pointer.
					info->event = event;

					// Lock the control mutex as we enter the main run loop.
					pthread_mutex_lock(&hookControlMutex);

					// Signal the custom source and wakeup the main run loop.
					CFRunLoopSourceSignal(sourceMsgPort);
					CFRunLoopWakeUp(CFRunLoopGetMain());

					// Wait for a lock while the main run loop processes they key typed event.
					if (pthread_mutex_lock(&hookControlMutex) == 0) {
						pthread_mutex_unlock(&hookControlMutex);
					}

					if (info->length == 1) {
						// Fire key typed event.
						objKeyEvent = (*env)->NewObject(
												env,
												clsKeyEvent,
												idKeyEvent,
												org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED,
												event_time,
												jmodifiers,
												jkey.rawcode,
												org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED,
												(jchar) info->buffer[0],
												jkey.location);
						(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
						(*env)->DeleteLocalRef(env, objKeyEvent);
					}

					info->event = NULL;
					info->length = 0;
					break;

				case kCGEventKeyUp:
				EVENT_KEYUP:
					keysym = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Key Released (%i)\n", (unsigned int) keysym);
					#endif

					jkey = NativeToJKey(keysym);
					jmodifiers = NativeToJEventMask(GetModifiers());

					// Fire key released event.
					objKeyEvent = (*env)->NewObject(
											env,
											clsKeyEvent,
											idKeyEvent,
											org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED,
											event_time,
											jmodifiers,
											jkey.rawcode,
											jkey.keycode,
											org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
											jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
					(*env)->DeleteLocalRef(env, objKeyEvent);
					break;

				case kCGEventFlagsChanged:
					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Modifiers Changed (0x%X)\n", (unsigned int) event_mask);
					#endif

					/* Because of the way apple handles modifiers we need to track
					* the key changes and handle our own key up and down events.
					* Outline of what is happening on the next 3 lines.
					* 1010 1100	prev
					* 1100 1010	curr
					* 0110 0110	prev xor curr
					*
					* turned on - i.e. pressed
					* 1100 1010	curr
					* 0110 0110	(prev xor curr)
					* 0100 0010	(prev xor curr) of prev
					*
					* turned off - i.e. released
					* 1010 1100	prev
					* 0110 0110	(prev xor curr)
					* 0010 0100	(prev xor curr) and prev
					*
					* CGEventFlags diff_event_mask = prev_event_mask ^ event_mask;
					* CGEventFlags keydown_event_mask = prev_event_mask | diff_event_mask;
					* CGEventFlags keyup_event_mask = event_mask & diff_event_mask;
					*/

					prev_event_mask = GetModifiers() & 0xFFFF0000;
					diff_event_mask = prev_event_mask ^ (event_mask & 0xFFFF0000);
					keyup_event_mask = (event_mask & 0xFFFF0000) & diff_event_mask;

					// Update the previous event mask.
					UnsetModifierMask(prev_event_mask);
					SetModifierMask(event_mask & 0xFFFF0000);

					if (diff_event_mask & key_event_mask && keyup_event_mask & key_event_mask) {
						// Process as a key pressed event.
						goto EVENT_KEYDOWN;
					}
					else if (diff_event_mask & key_event_mask && keyup_event_mask ^ key_event_mask) {
						// Process as a key released event.
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

					// Track the number of clicks.
					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Click Time (%lli)\n", (CGEventGetTimestamp(event) - click_time)  / 1000000);
					#endif

					if ((long) (CGEventGetTimestamp(event) - click_time) / 1000000 <= GetMultiClickTime()) {
						click_count++;
					}
					else {
						click_count = 1;
					}
					click_time = CGEventGetTimestamp(event);

					event_point = CGEventGetLocation(event);
					jbutton = NativeToJButton(button);
					jmodifiers = NativeToJEventMask(GetModifiers());

					// Fire mouse pressed event.
					objMouseEvent = (*env)->NewObject(
												env,
												clsMouseEvent,
												idMouseButtonEvent,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED,
												(jlong) event_time,
												jmodifiers,
												(jint) event_point.x,
												(jint) event_point.y,
												(jint) click_count,
												jbutton);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
					(*env)->DeleteLocalRef(env, objMouseEvent);
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
					jmodifiers = NativeToJEventMask(GetModifiers());

					// Fire mouse released event.
					objMouseEvent = (*env)->NewObject(
												env,
												clsMouseEvent,
												idMouseButtonEvent,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED,
												(jlong) event_time,
												jmodifiers,
												(jint) event_point.x,
												(jint) event_point.y,
												(jint) click_count,
												jbutton);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
					(*env)->DeleteLocalRef(env, objMouseEvent);

					if (mouse_dragged != true) {
						// Fire mouse clicked event.
						objMouseEvent = (*env)->NewObject(
													env,
													clsMouseEvent,
													idMouseButtonEvent,
													org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED,
													(jlong) event_time,
													jmodifiers,
													(jint) event_point.x,
													(jint) event_point.y,
													(jint) click_count,
													jbutton);
						(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
						(*env)->DeleteLocalRef(env, objMouseEvent);
					}
					break;


				case kCGEventLeftMouseDragged:
				case kCGEventRightMouseDragged:
				case kCGEventOtherMouseDragged:
					event_point = CGEventGetLocation(event);

					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Motion Notified (%f, %f)\n", event_point.x, event_point.y);
					#endif

					// Reset the click count.
					if (click_count != 0 && (long) (CGEventGetTimestamp(event) - click_time) / 1000000 > GetMultiClickTime()) {
						click_count = 0;
					}
					jmodifiers = NativeToJEventMask(GetModifiers());

					// Set the mouse dragged flag.
					mouse_dragged = true;

					// Fire mouse dragged event.
					objMouseEvent = (*env)->NewObject(
												env,
												clsMouseEvent,
												idMouseMotionEvent,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED,
												(jlong) event_time,
												jmodifiers,
												(jint) event_point.x,
												(jint) event_point.y,
												(jint) click_count);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
					(*env)->DeleteLocalRef(env, objMouseEvent);
					break;

				case kCGEventMouseMoved:
					event_point = CGEventGetLocation(event);
					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Motion Notified (%f, %f)\n", event_point.x, event_point.y);
					#endif

					// Reset the click count.
					if (click_count != 0 && (long) (CGEventGetTimestamp(event) - click_time) / 1000000 > GetMultiClickTime()) {
						click_count = 0;
					}
					jmodifiers = NativeToJEventMask(GetModifiers());

					// Set the mouse dragged flag.
					mouse_dragged = false;

					// Fire mouse moved event.
					objMouseEvent = (*env)->NewObject(
												env,
												clsMouseEvent,
												idMouseMotionEvent,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED,
												(jlong) event_time,
												jmodifiers,
												(jint) event_point.x,
												(jint) event_point.y,
												(jint) click_count);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
					(*env)->DeleteLocalRef(env, objMouseEvent);
					break;

				case kCGEventScrollWheel:
					event_point = CGEventGetLocation(event);

					// TODO Figure out of kCGScrollWheelEventDeltaAxis2 causes mouse events with zero rotation.
					if (CGEventGetIntegerValueField(event, kCGScrollWheelEventIsContinuous) == 0) {
						jscrollType = (jint)  org_jnativehook_mouse_NativeMouseWheelEvent_WHEEL_UNIT_SCROLL;
					}
					else {
						jscrollType = (jint)  org_jnativehook_mouse_NativeMouseWheelEvent_WHEEL_BLOCK_SCROLL;
					}

					// Scrolling data uses a fixed-point 16.16 signed integer format (Ex: 1.0 = 0x00010000).
					jwheelRotation = (jint) CGEventGetIntegerValueField(event, kCGScrollWheelEventDeltaAxis1) * -1;

					/* TODO Figure out the scroll wheel amounts are correct.  I
					* suspect that Apples Java implementation maybe reporting a
					* static "1" inaccurately.
					*/
					jscrollAmount = (jint) CGEventGetIntegerValueField(event, kCGScrollWheelEventPointDeltaAxis1) * -1;

					#ifdef DEBUG
					fprintf(stdout, "LowLevelProc(): Mouse Wheel Moved (%i, %i, %i)\n", (int) jscrollType, (int) jscrollAmount, (int) jwheelRotation);
					#endif

					// Track the number of clicks.
					if ((long) (CGEventGetTimestamp(event) - click_time) / 1000000 <= GetMultiClickTime()) {
						click_count++;
					}
					else {
						click_count = 1;
					}
					click_time = CGEventGetTimestamp(event);

					jmodifiers = NativeToJEventMask(GetModifiers());

					// Fire mouse wheel event.
					objMouseWheelEvent = (*env)->NewObject(
													env,
													clsMouseWheelEvent,
													idMouseWheelEvent,
													org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL,
													(jlong) event_time,
													jmodifiers,
													(jint) event_point.x,
													(jint) event_point.y,
													(jint) click_count,
													jscrollType,
													jscrollAmount,
													jwheelRotation);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseWheelEvent);
					(*env)->DeleteLocalRef(env, objMouseWheelEvent);
					break;

				#ifdef DEBUG
				default:
					fprintf(stderr, "LowLevelProc(): Unhandled Event Type: 0x%X\n", type);
					break;
				#endif
			}
		}
		else {
			// Unlock the mutex incase trylock succeeded.
			pthread_mutex_unlock(&hookRunningMutex);
		}

		// Handle any possible JNI issue that may have occurred.
		if ((*env)->ExceptionCheck(env) == JNI_TRUE) {
			#ifdef DEBUG
			fprintf(stderr, "LowLevelProc(): JNI error occurred!\n");
			(*env)->ExceptionDescribe(env);
			#endif
			(*env)->ExceptionClear(env);
		}
	}

	return event;
}

// This method will be externalized with 1.2
static bool ThreadStartCallback() {
	bool status = false;

	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		#ifdef DEBUG
		fprintf(stdout, "ThreadStartCallback(): Attached to JVM successful.\n");
		#endif

		// Create the global screen references up front to save time in the callback.
		jobject objLocalScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, idGetInstance);
		if (objLocalScreen != NULL) {
			objGlobalScreen = (*env)->NewGlobalRef(env, objLocalScreen);

			// Callback and start native event dispatch thread
			(*env)->CallVoidMethod(env, objGlobalScreen, idStartEventDispatcher);

			// Call Thread.currentThread().setName("JNativeHook Native Hook");
			jobject objCurrentThread = (*env)->CallStaticObjectMethod(env, clsThread, idCurrentThread);
			(*env)->CallVoidMethod(env, objCurrentThread, idSetName, (*env)->NewStringUTF(env, "JNativeHook Native Hook"));
			(*env)->DeleteLocalRef(env, objCurrentThread);

			status = true;
		}
		else {
			// We cant do a whole lot of anything if we cant create JNI globals.
			// Any exceptions are thrown by CreateJNIGlobals().

			#ifdef DEBUG
			fprintf(stderr, "ThreadStartCallback(): CreateJNIGlobals() failed!\n");
			#endif

			thread_ex.class = NATIVE_HOOK_EXCEPTION;
			thread_ex.message = "Failed to create JNI global references";
		}



		#ifdef DEBUG
		fprintf(stdout, "ThreadStartCallback(): Detach from JVM successful.\n");
		#endif
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "ThreadStartCallback(): AttachCurrentThread() failed!\n");
		#endif

		thread_ex.class = NATIVE_HOOK_EXCEPTION;
		thread_ex.message = "Failed to attach the native thread to the virtual machine";
	}

	return status;
}

// This method will be externalized with 1.2
static bool ThreadStopCallback() {
	bool status = false;

	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		// Calling AttachCurrentThread() should result in a no-op.

		// Callback and stop native event dispatch thread.
		(*env)->CallVoidMethod(env, objGlobalScreen, idStopEventDispatcher);

		// Remove the global reference to the GlobalScren object.
		(*env)->DeleteGlobalRef(env, objGlobalScreen);

		// Detach this thread from the JVM.
		if ((*jvm)->DetachCurrentThread(jvm) == JNI_OK) {
			status = true;
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "ThreadStopCallback(): DetachCurrentThread() failed!\n");
		}
		#endif
	}
	#ifdef DEBUG
	else {
		fprintf(stderr, "ThreadStartCallback(): AttachCurrentThread() failed!\n");
	}
	#endif

	return status;
}

static void *ThreadProc(void *arg) {
	int *status = (int *) arg;
	*status = RETURN_FAILURE;

	// Check and make sure assistive devices is enabled.
	if (AXAPIEnabled() == true) {
		#ifdef DEBUG
		fprintf(stdout, "ThreadProc(): Accessibility API is enabled.\n");
		#endif

		// Setup the event mask to listen for.
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

		#ifdef DEBUG
		event_mask |=	CGEventMaskBit(kCGEventNull);
		#endif

		CFMachPortRef event_port = CGEventTapCreate(
										kCGSessionEventTap,				// kCGHIDEventTap
										kCGHeadInsertEventTap,			// kCGTailAppendEventTap
										kCGEventTapOptionListenOnly,	// kCGEventTapOptionDefault See Bug #22
										event_mask,
										LowLevelProc,
										NULL
									);


		if (event_port != NULL) {
			#ifdef DEBUG
			fprintf(stdout, "ThreadProc(): Event tap created successfully.\n");
			#endif

			event_source = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event_port, 0);
			if (event_source != NULL) {
				#ifdef DEBUG
				fprintf(stdout, "ThreadProc(): CFMachPortCreateRunLoopSource() success.\n");
				#endif

				event_loop = CFRunLoopGetCurrent();
				if (event_loop != NULL) {
					#ifdef DEBUG
					fprintf(stdout, "ThreadProc(): CFRunLoopGetCurrent() success.\n");
					#endif

					// Initialize Native Input Functions.
					LoadInputHelper();

					// Callback for additional thread initialization.
					if (ThreadStartCallback()) {
						// Create run loop observers.
						CFRunLoopObserverRef observer = CFRunLoopObserverCreate(
															kCFAllocatorDefault,
															kCFRunLoopEntry | kCFRunLoopExit, //kCFRunLoopAllActivities,
															true,
															0,
															CallbackProc,
															NULL
														);

						if (observer != NULL) {
							// Set the exit status.
							*status = RETURN_SUCCESS;

							StartMessagePortRunLoop();

							CFRunLoopAddSource(event_loop, event_source, kCFRunLoopDefaultMode);
							CFRunLoopAddObserver(event_loop, observer, kCFRunLoopDefaultMode);

							CFRunLoopRun();

							// Lock back up until we are done processing the exit.
							CFRunLoopRemoveObserver(event_loop, observer, kCFRunLoopDefaultMode);
							CFRunLoopRemoveSource(event_loop, event_source, kCFRunLoopDefaultMode);
							CFRunLoopObserverInvalidate(observer);

							StopMessagePortRunLoop();
						}
						else {
							// We cant do a whole lot of anything if we cant
							// create run loop observer.

							#ifdef DEBUG
							fprintf(stderr, "ThreadProc(): CFRunLoopObserverRef() failed!\n");
							#endif

							thread_ex.class = NATIVE_HOOK_EXCEPTION;
							thread_ex.message = "Failed to create the run loop observer";
						}

						// Callback for additional thread cleanup.
						ThreadStopCallback();
					}
				}
				else {
					#ifdef DEBUG
					fprintf(stderr, "ThreadProc(): CFRunLoopGetCurrent() failure.\n");
					#endif

					thread_ex.class = NATIVE_HOOK_EXCEPTION;
					thread_ex.message = "Failed to attach to the run loop";
				}

				// Clean up the event source.
				CFRelease(event_source);
			}
			else {
				#ifdef DEBUG
				fprintf(stderr, "ThreadProc(): CFMachPortCreateRunLoopSource() failure.\n");
				#endif

				thread_ex.class = NATIVE_HOOK_EXCEPTION;
				thread_ex.message = "Failed to create run loop";
			}

			// Stop the CFMachPort from receiving any more messages.
			CFMachPortInvalidate(event_port);
			CFRelease(event_port);
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "ThreadProc(): Failed to create event port!\n");
			#endif

			thread_ex.class = NATIVE_HOOK_EXCEPTION;
			thread_ex.message = "Failed to create event port";
		}
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "ThreadProc(): Accessibility API is not enabled.\n");
		#endif

		thread_ex.class = NATIVE_HOOK_EXCEPTION;
		thread_ex.message = "Access for assistive devices disabled";
	}

	#ifdef DEBUG
	fprintf(stdout, "ThreadProc(): complete.\n");
	#endif

	// Make sure the control mutex is unlocked.
	pthread_mutex_unlock(&hookControlMutex);

	pthread_exit(status);
}

int StartNativeThread() {
	int status = RETURN_FAILURE;

	// We shall use the default pthread attributes: thread is joinable
	// (not detached) and has default (non real-time) scheduling policy.
	//pthread_mutex_init(&hookControlMutex, NULL);

	// Lock the thread control mutex.  This will be unlocked when the
	// thread has finished starting, or when it has fully stopped.
	pthread_mutex_lock(&hookControlMutex);

	// Make sure the native thread is not already running.
	if (IsNativeThreadRunning() != true) {
		if (pthread_create(&hookThreadId, NULL, ThreadProc, malloc(sizeof(int))) == 0) {
			#ifdef DEBUG
			fprintf(stdout, "StartNativeThread(): start successful.\n");
			#endif

			// Wait for the thread to start up.
			if (pthread_mutex_lock(&hookControlMutex) == 0) {
				pthread_mutex_unlock(&hookControlMutex);
			}

			// Handle any possible JNI issue that may have occurred.
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

				// Wait for the thread to die.
				void *thread_status;
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

	// Make sure the mutex gets unlocked.
	pthread_mutex_unlock(&hookControlMutex);

	return status;
}

int StopNativeThread() {
	int status = RETURN_FAILURE;

	// Lock the thread control mutex.  This will be unlocked when the
	// thread has fully stopped.
	pthread_mutex_lock(&hookControlMutex);

	if (IsNativeThreadRunning() == true) {
		// Stop the run loop.
		CFRunLoopStop(event_loop);

		// Wait for the thread to die.
		void *thread_status;
		pthread_join(hookThreadId, &thread_status);
		status = *(int *) thread_status;
		free(thread_status);

		#ifdef DEBUG
		fprintf(stdout, "StopNativeThread(): Thread Result (%i)\n", status);
		#endif
	}

	// Clean up the mutex.
	//pthread_mutex_destroy(&hookControlMutex);

	// Make sure the mutex gets unlocked.
	pthread_mutex_unlock(&hookControlMutex);

	return status;
}

bool IsNativeThreadRunning() {
	bool isRunning = false;

	if (pthread_mutex_trylock(&hookRunningMutex) == 0) {
		// Lock Successful, we are not running.
		pthread_mutex_unlock(&hookRunningMutex);
	}
	else {
		isRunning = true;
	}

	#ifdef DEBUG
	fprintf(stdout, "IsNativeThreadRunning(): Running State (%i)\n", isRunning);
	#endif

	return isRunning;
}
