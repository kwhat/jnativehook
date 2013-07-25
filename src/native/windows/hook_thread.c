/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>

#include "NativeErrors.h"
#include "NativeGlobals.h"
#include "NativeHelpers.h"
#include "NativeThread.h"
#include "NativeToJava.h"
#include "WinInputHelpers.h"
#include "WinUnicodeHelper.h"

// Exception global for thread initialization.
static Exception thread_ex;

// GlobalScreen object.
static jobject objGlobalScreen;

// Click count globals.
static unsigned short click_count = 0;
static DWORD click_time = 0;
static POINT last_click;

// The handle to the DLL module pulled in DllMain on DLL_PROCESS_ATTACH.
extern HINSTANCE hInst;

// Thread and hook handles.
static DWORD hook_thread_id = 0;
static HANDLE hook_thread_handle = NULL, hook_control_handle = NULL;
static HHOOK keyboard_event_hhook = NULL, mouse_event_hhook = NULL;

static WCHAR keytxt = '\0', keydead = 0;

// Virtual event pointer.
static VirtualEvent *event = NULL;

// Event dispatch callback
static void (*current_dispatch_proc)(VirtualEvent *const) = NULL;

NATIVEHOOK_API void hook_set_dispatch_proc(void (*dispatch_proc)(VirtualEvent * const)) {
	#ifdef USE_DEBUG
	fprintf(stdout, "hook_set_dispatch_proc(): Setting new dispatch callback.\n");
	#endif

	current_dispatch_proc = dispatch_proc;
}

inline static void dispatch_event(VirtualEvent *const event) {
	if (current_dispatch_proc != NULL) {
		#ifdef USE_DEBUG
		fprintf(stdout, "dispatch_event(): Dispatching event. (%d)\n", event->type);
		#endif

		current_dispatch_proc(event);
	}
	#ifdef USE_DEBUG
	else {
		fprintf(stderr, "dispatch_event(): No dispatch proc set! (%d)\n", event->type);
	}
	#endif
}

static LRESULT CALLBACK keyboard_event_proc(int nCode, WPARAM wParam, LPARAM lParam) {
	// MS Keyboard event struct data.
	KBDLLHOOKSTRUCT *kbhook = (KBDLLHOOKSTRUCT *) lParam;

	// Set the event_time
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	__int64 system_time = ft.dwHighDateTime;
	system_time <<= 32;
	system_time |= ft.dwLowDateTime;

	// Convert to milliseconds = 100-nanoseconds / 10000
	system_time /= 10000;

	// Convert Windows epoch to Unix epoch (1970 - 1601 in milliseconds)
	unsigned long int event_time = system_time - 11644473600000;

	int event_mask;
	switch(wParam) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			#ifdef DEBUG
			fprintf(stdout, "LowLevelKeyboardProc(): Key pressed. (%i)\n", (unsigned int) kbhook->vkCode);
			#endif

			// Check and setup modifiers.
			if (kbhook->vkCode == VK_LSHIFT)		SetModifierMask(MOD_LSHIFT);
			else if (kbhook->vkCode == VK_RSHIFT)	SetModifierMask(MOD_RSHIFT);
			else if (kbhook->vkCode == VK_LCONTROL)	SetModifierMask(MOD_LCONTROL);
			else if (kbhook->vkCode == VK_RCONTROL)	SetModifierMask(MOD_RCONTROL);
			else if (kbhook->vkCode == VK_LMENU)	SetModifierMask(MOD_LALT);
			else if (kbhook->vkCode == VK_RMENU)	SetModifierMask(MOD_RALT);
			else if (kbhook->vkCode == VK_LWIN)		SetModifierMask(MOD_LWIN);
			else if (kbhook->vkCode == VK_RWIN)		SetModifierMask(MOD_RWIN);

			// Workaround for Windows numpad keys.
			if (kbhook->vkCode == VK_RETURN && (kbhook->flags & 0x01) != 0) {
				jkey = NativeToJKey(VK_NUMPAD_RETURN);
			}
			else if (kbhook->vkCode == VK_PRIOR && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_PRIOR);
			}
			else if (kbhook->vkCode == VK_NEXT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_NEXT);
			}
			else if (kbhook->vkCode == VK_END && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_END);
			}
			else if (kbhook->vkCode == VK_HOME && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_HOME);
			}
			else if (kbhook->vkCode == VK_LEFT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_LEFT);
			}
			else if (kbhook->vkCode == VK_UP && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_UP);
			}
			else if (kbhook->vkCode == VK_RIGHT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_RIGHT);
			}
			else if (kbhook->vkCode == VK_DOWN && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_DOWN);
			}
			else if (kbhook->vkCode == VK_INSERT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_INSERT);
			}
			else if (kbhook->vkCode == VK_DELETE && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_DELETE);
			}
			else {
				jkey = NativeToJKey(kbhook->vkCode);
			}

			event_mask = GetModifiers();

			// Fire key pressed event.
			event->type = EVENT_KEY_PRESSED;
			event->time = event_time;
			event->mask = convert_to_virtual_mask(event_mask);

			keysym = keycode_to_keysym(event_code, event_mask);
			event->data.keyboard.keycode = convert_to_virtual_key(kbhook->vkCode);
			event->data.keyboard.rawcode = kbhook->scanCode;
			event->data.keyboard.keychar = CHAR_UNDEFINED;

			dispatch_event(event);

			if (ConvertVirtualKeyToWChar(kbhook->vkCode, &keytxt, &keydead) > 0) {
				// Fire key typed event.
				event->type = EVENT_KEY_TYPED;

				event->data.keyboard.keycode = VC_UNDEFINED;
				event->data.keyboard.keychar = keychar;

				dispatch_event(event);
			}
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			#ifdef DEBUG
			fprintf(stdout, "LowLevelKeyboardProc(): Key released. (%i)\n", (unsigned int) kbhook->vkCode);
			#endif

			// Check and setup modifiers.
			if (kbhook->vkCode == VK_LSHIFT)		UnsetModifierMask(MOD_LSHIFT);
			else if (kbhook->vkCode == VK_RSHIFT)	UnsetModifierMask(MOD_RSHIFT);
			else if (kbhook->vkCode == VK_LCONTROL)	UnsetModifierMask(MOD_LCONTROL);
			else if (kbhook->vkCode == VK_RCONTROL)	UnsetModifierMask(MOD_RCONTROL);
			else if (kbhook->vkCode == VK_LMENU)	UnsetModifierMask(MOD_LALT);
			else if (kbhook->vkCode == VK_RMENU)	UnsetModifierMask(MOD_RALT);
			else if (kbhook->vkCode == VK_LWIN)		UnsetModifierMask(MOD_LWIN);
			else if (kbhook->vkCode == VK_RWIN)		UnsetModifierMask(MOD_RWIN);

			// Workaround for Windows numpad keys.
			if (kbhook->vkCode == VK_RETURN && (kbhook->flags & 0x01) != 0) {
				jkey = NativeToJKey(VK_NUMPAD_RETURN);
			}
			else if (kbhook->vkCode == VK_PRIOR && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_PRIOR);
			}
			else if (kbhook->vkCode == VK_NEXT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_NEXT);
			}
			else if (kbhook->vkCode == VK_END && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_END);
			}
			else if (kbhook->vkCode == VK_HOME && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_HOME);
			}
			else if (kbhook->vkCode == VK_LEFT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_LEFT);
			}
			else if (kbhook->vkCode == VK_UP && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_UP);
			}
			else if (kbhook->vkCode == VK_RIGHT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_RIGHT);
			}
			else if (kbhook->vkCode == VK_DOWN && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_DOWN);
			}
			else if (kbhook->vkCode == VK_INSERT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_INSERT);
			}
			else if (kbhook->vkCode == VK_DELETE && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_DELETE);
			}
			else {
				jkey = NativeToJKey(kbhook->vkCode);
			}

			event_mask = GetModifiers();

			// Fire key released event.
			event->type = EVENT_KEY_RELEASED;
			event->time = event_time;
			event->mask = convert_to_virtual_mask(event_mask);

			keysym = keycode_to_keysym(event_code, event_mask);
			event->data.keyboard.keycode = convert_to_virtual_key(kbhook->vkCode);
			event->data.keyboard.rawcode = kbhook->scanCode;
			event->data.keyboard.keychar = CHAR_UNDEFINED;

			dispatch_event(event);
			break;

		#ifdef DEBUG
		default:
			fprintf(stdout, "LowLevelKeyboardProc(): Unhandled keyboard event: 0x%X\n", (unsigned int) wParam);
			break;
		#endif
	}

	return CallNextHookEx(keyboard_event_hhook, nCode, wParam, lParam);
}

static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	// MS Mouse event struct data.
	MSLLHOOKSTRUCT *mshook = (MSLLHOOKSTRUCT *) lParam;

	// Java event data.
	jint jbutton;
	jint scrollType, scrollAmount, wheelRotation;
	jint jmodifiers;

	// Set the event_time
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	__int64 system_time = ft.dwHighDateTime;
	system_time <<= 32;
	system_time |= ft.dwLowDateTime;

	// Convert to milliseconds = 100-nanoseconds / 10000
	system_time /= 10000;

	// Convert Windows epoch to Unix epoch (1970 - 1601 in milliseconds)
	unsigned long int event_time = system_time - 11644473600000;

	switch(wParam) {
		case WM_LBUTTONDOWN:
			jbutton = NativeToJButton(VK_LBUTTON);
			SetModifierMask(MOD_LBUTTON);
			goto BUTTONDOWN;

		case WM_RBUTTONDOWN:
			jbutton = NativeToJButton(VK_RBUTTON);
			SetModifierMask(MOD_RBUTTON);
			goto BUTTONDOWN;

		case WM_MBUTTONDOWN:
			jbutton = NativeToJButton(VK_MBUTTON);
			SetModifierMask(MOD_MBUTTON);
			goto BUTTONDOWN;

		case WM_XBUTTONDOWN:
		case WM_NCXBUTTONDOWN:
			if (HIWORD(mshook->mouseData) == XBUTTON1) {
				jbutton = NativeToJButton(VK_XBUTTON1);
				SetModifierMask(MOD_XBUTTON1);
			}
			else if (HIWORD(mshook->mouseData) == XBUTTON2) {
				jbutton = NativeToJButton(VK_XBUTTON2);
				SetModifierMask(MOD_XBUTTON2);
			}
			else {
				jbutton = NativeToJButton(HIWORD(mshook->mouseData));
			}

		BUTTONDOWN:
			#ifdef DEBUG
			fprintf(stdout, "LowLevelMouseProc(): Button pressed. (%i)\n", (int) jbutton);
			#endif

			// Track the number of clicks.
			if ((long) (mshook->time - click_time) <= GetMultiClickTime()) {
				click_count++;
			}
			else {
				click_count = 1;
			}
			click_time = mshook->time;

			// Convert native modifiers to java modifiers.
			jmodifiers = NativeToJEventMask(GetModifiers());

			// Store the last click point.
			last_click.x = mshook->pt.x;
			last_click.y = mshook->pt.y;

			// Fire mouse pressed event.
			objMouseEvent = (*env)->NewObject(
										env,
										clsMouseEvent,
										idMouseButtonEvent,
										org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED,
										event_time,
										jmodifiers,
										(jint) mshook->pt.x,
										(jint) mshook->pt.y,
										(jint) click_count,
										jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
			(*env)->DeleteLocalRef(env, objMouseEvent);
			break;

		case WM_LBUTTONUP:
			jbutton = NativeToJButton(VK_LBUTTON);
			UnsetModifierMask(MOD_LBUTTON);
			goto BUTTONUP;

		case WM_RBUTTONUP:
			jbutton = NativeToJButton(VK_RBUTTON);
			UnsetModifierMask(MOD_RBUTTON);
			goto BUTTONUP;

		case WM_MBUTTONUP:
			jbutton = NativeToJButton(VK_MBUTTON);
			UnsetModifierMask(MOD_MBUTTON);
			goto BUTTONUP;

		case WM_XBUTTONUP:
		case WM_NCXBUTTONUP:
			if (HIWORD(mshook->mouseData) == XBUTTON1) {
				jbutton = NativeToJButton(VK_XBUTTON1);
				UnsetModifierMask(MOD_XBUTTON1);
			}
			else if (HIWORD(mshook->mouseData) == XBUTTON2) {
				jbutton = NativeToJButton(VK_XBUTTON2);
				UnsetModifierMask(MOD_XBUTTON2);
			}
			else {
				jbutton = NativeToJButton(HIWORD(mshook->mouseData));
			}

		BUTTONUP:
			#ifdef DEBUG
			fprintf(stdout, "LowLevelMouseProc(): Button released. (%i)\n", (int) jbutton);
			#endif

			jmodifiers = NativeToJEventMask(GetModifiers());

			// Fire mouse released event.
			objMouseEvent = (*env)->NewObject(
										env,
										clsMouseEvent,
										idMouseButtonEvent,
										org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED,
										event_time,
										jmodifiers,
										(jint) mshook->pt.x,
										(jint) mshook->pt.y,
										(jint) click_count,
										jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
			(*env)->DeleteLocalRef(env, objMouseEvent);

			if (last_click.x == mshook->pt.x && last_click.y == mshook->pt.y) {
				// Fire mouse clicked event.
				objMouseEvent = (*env)->NewObject(
											env,
											clsMouseEvent,
											idMouseButtonEvent,
											org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED,
											event_time,
											jmodifiers,
											(jint) mshook->pt.x,
											(jint) mshook->pt.y,
											(jint) click_count,
											jbutton);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
				(*env)->DeleteLocalRef(env, objMouseEvent);
			}
			break;

		case WM_MOUSEMOVE:
			#ifdef DEBUG
			fprintf(stdout, "LowLevelMouseProc(): Motion Notified. (%li, %li)\n", mshook->pt.x, mshook->pt.y);
			#endif

			// Reset the click count.
			if (click_count != 0 && (long) (mshook->time - click_time) > GetMultiClickTime()) {
				click_count = 0;
			}
			jmodifiers = NativeToJEventMask(GetModifiers());

			// We received a mouse move event with the mouse actually moving.
			// This verifies that the mouse was moved after being depressed.
			if (last_click.x != mshook->pt.x || last_click.y != mshook->pt.y) {
				// Check the upper half of java modifiers for non zero value.
				// This indicates the presence of a button down mask.
				if (jmodifiers >> 4 > 0) {
					// Create Mouse dragged event.
					objMouseEvent = (*env)->NewObject(
												env,
												clsMouseEvent,
												idMouseMotionEvent,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED,
												event_time,
												jmodifiers,
												(jint) mshook->pt.x,
												(jint) mshook->pt.y,
												(jint) click_count);
				}
				else {
					// Create a Mouse Moved event.
					objMouseEvent = (*env)->NewObject(
												env,
												clsMouseEvent,
												idMouseMotionEvent,
												org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED,
												event_time,
												jmodifiers,
												(jint) mshook->pt.x,
												(jint) mshook->pt.y,
												(jint) click_count);
				}

				// Fire mouse moved event.
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
				(*env)->DeleteLocalRef(env, objMouseEvent);
			}
			break;

		case WM_MOUSEWHEEL:
			#ifdef DEBUG
			fprintf(stdout, "LowLevelMouseProc(): WM_MOUSEWHEEL. (%i / %i)\n", HIWORD(mshook->mouseData), WHEEL_DELTA);
			#endif

			// Track the number of clicks.
			if ((long) (mshook->time - click_time) <= GetMultiClickTime()) {
				click_count++;
			}
			else {
				click_count = 1;
			}
			click_time = mshook->time;

			jmodifiers = NativeToJEventMask(GetModifiers());

			/* Delta HIWORD(mshook->mouseData)
			 * A positive value indicates that the wheel was rotated
			 * forward, away from the user; a negative value indicates that
			 * the wheel was rotated backward, toward the user. One wheel
			 * click is defined as WHEEL_DELTA, which is 120.
			 */

			scrollType = (jint) GetScrollWheelType();
			scrollAmount = (jint) GetScrollWheelAmount();
			wheelRotation = (jint) ((signed short) HIWORD(mshook->mouseData) / WHEEL_DELTA) * -1;

			// Fire mouse wheel event.
			objMouseWheelEvent = (*env)->NewObject(
											env,
											clsMouseWheelEvent,
											idMouseWheelEvent,
											org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL,
											event_time,
											jmodifiers,
											(jint) mshook->pt.x,
											(jint) mshook->pt.y,
											(jint) click_count,
											scrollType,
											scrollAmount,
											wheelRotation);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseWheelEvent);
			(*env)->DeleteLocalRef(env, objMouseWheelEvent);
			break;

		#ifdef DEBUG
		default:
			fprintf(stdout, "LowLevelMouseProc(): Unhandled mouse event: 0x%X\n", (unsigned int) wParam);
			break;
		#endif
	}


	return CallNextHookEx(mouse_event_hhook, nCode, wParam, lParam);
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



static DWORD WINAPI ThreadProc(LPVOID UNUSED(lpParameter)) {
	DWORD status = RETURN_FAILURE;

	// Create the native hooks.
	keyboard_event_hhook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_event_proc, hInst, 0);
	mouse_event_hhook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0);

	// If we did not encounter a problem, start processing events.
	if (keyboard_event_hhook != NULL && mouse_event_hhook != NULL) {
		#ifdef DEBUG
		fprintf(stdout, "ThreadProc(): SetWindowsHookEx() successful.\n");
		#endif

		// Callback for additional thread initialization.
		if (ThreadStartCallback()) {
			// Check and setup modifiers.
			if (GetKeyState(VK_LSHIFT)	 < 0)	SetModifierMask(MOD_LSHIFT);
			if (GetKeyState(VK_RSHIFT)   < 0)	SetModifierMask(MOD_RSHIFT);
			if (GetKeyState(VK_LCONTROL) < 0)	SetModifierMask(MOD_LCONTROL);
			if (GetKeyState(VK_RCONTROL) < 0)	SetModifierMask(MOD_RCONTROL);
			if (GetKeyState(VK_LMENU)    < 0)	SetModifierMask(MOD_LALT);
			if (GetKeyState(VK_RMENU)    < 0)	SetModifierMask(MOD_RALT);
			if (GetKeyState(VK_LWIN)     < 0)	SetModifierMask(MOD_LWIN);
			if (GetKeyState(VK_RWIN)     < 0)	SetModifierMask(MOD_RWIN);

			// Set the exit status.
			status = RETURN_SUCCESS;

			// Signal that we have passed the thread initialization.
			SetEvent(hook_control_handle);

			// Block until the thread receives an WM_QUIT request.
			MSG message;
			while (GetMessage(&message, (HWND) -1, 0, 0 ) > 0) {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			// Callback for additional thread cleanup.
			ThreadStopCallback();
		}
	}
	else {
		#ifdef DEBUG
		fprintf(stderr, "ThreadProc(): SetWindowsHookEx() failed!\n");
		#endif

		thread_ex.class = NATIVE_HOOK_EXCEPTION;
		thread_ex.message = "Failed to create low level event hook";
	}

	// Destroy the native hooks.
	if (keyboard_event_hhook != NULL) {
		UnhookWindowsHookEx(keyboard_event_hhook);
		keyboard_event_hhook = NULL;
	}

	if (mouse_event_hhook != NULL) {
		UnhookWindowsHookEx(mouse_event_hhook);
		mouse_event_hhook = NULL;
	}

	#ifdef DEBUG
	fprintf(stdout, "ThreadProc(): complete.\n");
	#endif

	// Make sure we signal that we have passed any exception throwing code.
	// This should only make a difference if we had an initialization exception.
	SetEvent(hook_control_handle);

	ExitThread(status);
}

int StartNativeThread() {
	int status = RETURN_FAILURE;

	// Make sure the native thread is not already running.
	if (IsNativeThreadRunning() != true) {
		// Create event handle for the thread hook.
		hook_control_handle = CreateEvent(NULL, TRUE, FALSE, "hookEventHandle");

		LPTHREAD_START_ROUTINE lpStartAddress = &ThreadProc;
		hook_thread_handle = CreateThread(NULL, 0, lpStartAddress, NULL, 0, &hook_thread_id);
		if (hook_thread_handle != INVALID_HANDLE_VALUE) {
			#ifdef DEBUG
			fprintf(stdout, "StartNativeThread(): start successful.\n");
			#endif

			// Wait for any possible thread exceptions to get thrown into
			// the queue
			WaitForSingleObject(hook_control_handle, INFINITE);

			// TODO Set the return status to the thread exit code.
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
				WaitForSingleObject(hook_thread_handle,  INFINITE);

				DWORD thread_status;
				GetExitCodeThread(hook_thread_handle, &thread_status);
				status = (int) thread_status;

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

	return status;
}

int StopNativeThread() {
	int status = NATIVEHOOK_FAILURE;

	if (hook_is_enabled() == true) {
		// Try to exit the thread naturally.
		PostThreadMessage(hook_thread_id, WM_QUIT, (WPARAM) NULL, (LPARAM) NULL);
		WaitForSingleObject(hook_thread_handle,  INFINITE);

		DWORD thread_status;
		GetExitCodeThread(hook_thread_handle, &thread_status);
		status = (int) thread_status;

		CloseHandle(hook_thread_handle);
		hook_thread_handle = NULL;

		CloseHandle(hook_control_handle);
		hook_control_handle = NULL;

		#ifdef DEBUG
		fprintf(stdout, "StopNativeThread(): Thread Result (%i)\n", status);
		#endif
	}

	return status;
}

NATIVEHOOK_API bool hook_is_enabled() {
	bool is_running = false;

	DWORD status;
	GetExitCodeThread(hook_thread_handle, &status);

	if (status == STILL_ACTIVE) {
		is_running = true;
	}

	#ifdef DEBUG
	fprintf(stdout, "IsNativeThreadRunning(): State (%i)\n", is_running);
	#endif

	return is_running;
}
