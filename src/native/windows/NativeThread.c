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

//Exception global for thread initialization.
static Exception thread_ex;

//Click count globals
static unsigned short click_count = 0;
static DWORD click_time = 0;
static bool mouse_dragged = false;

//The handle to the DLL module pulled in DllMain on DLL_PROCESS_ATTACH
extern HINSTANCE hInst;

//Thread and hook handles.
static DWORD hookThreadId = 0;
static HANDLE hookThreadHandle = NULL, hookEventHandle = NULL;
static HHOOK handleKeyboardHook = NULL, handleMouseHook = NULL;


static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	JNIEnv * env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		//MS Keyboard Event Struct Data
		KBDLLHOOKSTRUCT * kbhook = (KBDLLHOOKSTRUCT *) lParam;

		//Java Event Data
		JKeyDatum jkey;
		jint modifiers;
		WCHAR keytxt[4];
		
		//Jave Key Event Object.
		jobject objKeyEvent;

		switch(wParam) {
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				#ifdef DEBUG
				fprintf(stdout, "LowLevelKeyboardProc(): Key pressed. (%i)\n", (unsigned int) kbhook->vkCode);
				#endif

				//Check and setup modifiers
				switch (kbhook->vkCode) {
					case VK_CAPITAL:
					case VK_NUMLOCK:
					case VK_SCROLL:

					case VK_LSHIFT:
					case VK_RSHIFT:
					case VK_LCONTROL:
					case VK_RCONTROL:
					case VK_LMENU:
					case VK_RMENU:
					case VK_LWIN:
					case VK_RWIN:
						SetModifier(kbhook->vkCode);
				}


				jkey = NativeToJKey(kbhook->vkCode);
				modifiers = NativeToJEventMask(0x00);

				//Fire key pressed event.
				//TODO Should rawCode be the scanCode or the vkCode
				objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED, (jlong) kbhook->time, modifiers, kbhook->scanCode, jkey.keycode, org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED, jkey.location);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);

				if (KeycodeToUnicode(kbhook->vkCode, kbhook->scanCode, keytxt, sizeof(keytxt)) > 0) {
					//Fire key typed event.
					objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED, (jlong) kbhook->time, modifiers, kbhook->scanCode, jkey.keycode, (jchar) keytxt[0], jkey.location);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
				}
			break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
				#ifdef DEBUG
				fprintf(stdout, "LowLevelKeyboardProc(): Key released. (%i)\n", (unsigned int) kbhook->vkCode);
				#endif

				//Check and setup modifiers
				switch (kbhook->vkCode) {
					case VK_CAPITAL:
					case VK_NUMLOCK:
					case VK_SCROLL:

					case VK_LSHIFT:
					case VK_RSHIFT:
					case VK_LCONTROL:
					case VK_RCONTROL:
					case VK_LMENU:
					case VK_RMENU:
					case VK_LWIN:
					case VK_RWIN:
						UnsetModifier(kbhook->vkCode);
				}

				jkey = NativeToJKey(kbhook->vkCode);
				modifiers = NativeToJEventMask(0x00);

				//Fire key released event.
				objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED, (jlong) kbhook->time, modifiers, kbhook->scanCode, jkey.keycode, org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED, jkey.location);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
			break;
		}

		//Handle any possible JNI issue that may have occured.
		if ((*env)->ExceptionCheck(env) == JNI_TRUE) {
			#ifdef DEBUG
			fprintf(stderr, "LowLevelKeyboardProc(): JNI error occurred!\n");
			(*env)->ExceptionDescribe(env);
			#endif
			(*env)->ExceptionClear(env);
		}
	}

	return CallNextHookEx(handleKeyboardHook, nCode, wParam, lParam);
}

static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	JNIEnv * env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		//MS Mouse Event Struct Data
		MSLLHOOKSTRUCT * mshook = (MSLLHOOKSTRUCT *) lParam;

		//Java Event Data
		jint jbutton;
		jint scrollType, scrollAmount, wheelRotation;
		jint modifiers;

		//Java Mouse Event Object.
		jobject objMouseEvent, objMouseWheelEvent;

		/* Code to track the click count, It maybe easier to track our own click
		 * counts to allow for triple click detection.
		int clickCount=0;
		if (button!=MouseButtons.None)
			if (wParam == WM_LBUTTONDBLCLK || wParam = WM_MBUTTONDBLCLK || wParam == WM_RBUTTONDBLCLK || wParam == WM_XBUTTONDBLCLK)
				clickCount=2;
			else
				clickCount=1;
		*/
		switch(wParam) {
			case WM_LBUTTONDOWN:
				jbutton = NativeToJButton(VK_LBUTTON);
				SetModifier(VK_LBUTTON);
			goto BUTTONDOWN;

			case WM_RBUTTONDOWN:
				jbutton = NativeToJButton(VK_RBUTTON);
				SetModifier(VK_RBUTTON);
			goto BUTTONDOWN;

			case WM_MBUTTONDOWN:
				jbutton = NativeToJButton(VK_MBUTTON);
				SetModifier(VK_MBUTTON);
			goto BUTTONDOWN;

			case WM_XBUTTONDOWN:
			case WM_NCXBUTTONDOWN:
				if (HIWORD(mshook->mouseData) == XBUTTON1) {
					jbutton = NativeToJButton(VK_XBUTTON1);
					SetModifier(VK_XBUTTON1);
				}
				else if (HIWORD(mshook->mouseData) == XBUTTON2) {
					jbutton = NativeToJButton(VK_XBUTTON2);
					SetModifier(VK_XBUTTON2);
				}
				else {
					jbutton = NativeToJButton(HIWORD(mshook->mouseData));
				}

			BUTTONDOWN:
				#ifdef DEBUG
				fprintf(stdout, "LowLevelMouseProc(): Button pressed. (%i)\n", (int) jbutton);
				#endif

				//Track the number of clicks.
				if ((long) (mshook->time - click_time) <= GetMultiClickTime()) {
					click_count++;
				}
				else {
					click_count = 1;
				}
				click_time = mshook->time;

				//Convert native modifiers to java modifiers.
				modifiers = NativeToJEventMask(0x00);

				//Fire mouse pressed event.
				objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y, (jint) click_count, jbutton);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
			break;

			case WM_LBUTTONUP:
				jbutton = NativeToJButton(VK_LBUTTON);
				UnsetModifier(VK_LBUTTON);
			goto BUTTONUP;

			case WM_RBUTTONUP:
				jbutton = NativeToJButton(VK_RBUTTON);
				UnsetModifier(VK_RBUTTON);
			goto BUTTONUP;

			case WM_MBUTTONUP:
				jbutton = NativeToJButton(VK_MBUTTON);
				UnsetModifier(VK_MBUTTON);
			goto BUTTONUP;

			case WM_XBUTTONUP:
			case WM_NCXBUTTONUP:
				if (HIWORD(mshook->mouseData) == XBUTTON1) {
					jbutton = NativeToJButton(VK_XBUTTON1);
					UnsetModifier(VK_XBUTTON1);
				}
				else if (HIWORD(mshook->mouseData) == XBUTTON2) {
					jbutton = NativeToJButton(VK_XBUTTON2);
					UnsetModifier(VK_XBUTTON2);
				}
				else {
					jbutton = NativeToJButton(HIWORD(mshook->mouseData));
				}

			BUTTONUP:
				#ifdef DEBUG
				fprintf(stdout, "LowLevelMouseProc(): Button released. (%i)\n", (int) jbutton);
				#endif

				modifiers = NativeToJEventMask(0x00);

				//Fire mouse released event.
				objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y, (jint) click_count, jbutton);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);

				if (mouse_dragged != true) {
					//Fire mouse clicked event.
					objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y, (jint) click_count, jbutton);
					(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
				}
			break;

			case WM_MOUSEMOVE:
				#ifdef DEBUG
				fprintf(stdout, "LowLevelMouseProc(): Motion Notified. (%li, %li)\n", mshook->pt.x, mshook->pt.y);
				#endif

				//Reset the clickcount
				if (click_count != 0 && (long) (mshook->time - click_time) > GetMultiClickTime()) {
					click_count = 0;
				}
				modifiers = NativeToJEventMask(0x00);

				//Set the mouse draged flag
				mouse_dragged = modifiers >> 4 > 0;

				//Check the upper half of java modifiers for non zero value.
				if (modifiers >> 4 > 0) {
					//Create Mouse Dragged event.
					objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseMotionEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y, (jint) click_count);
				}
				else {
					//Create a Mouse Moved event
					objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseMotionEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y, (jint) click_count);
				}

				//Fire mouse moved event.
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
			break;

			case WM_MOUSEWHEEL:
				#ifdef DEBUG
				fprintf(stdout, "LowLevelMouseProc(): WM_MOUSEWHEEL. (%i / %i)\n", HIWORD(mshook->mouseData), WHEEL_DELTA);
				#endif

				//Track the number of clicks.
				if ((long) (mshook->time - click_time) <= GetMultiClickTime()) {
					click_count++;
				}
				else {
					click_count = 1;
				}
				click_time = mshook->time;

				modifiers = NativeToJEventMask(0x00);

				//Delta HIWORD(mshook->mouseData)
				//A positive value indicates that the wheel was rotated forward, away from the user; a negative
				//value indicates that the wheel was rotated backward, toward the user. One wheel click is
				//defined as WHEEL_DELTA, which is 120.
				
				scrollType = (jint) GetScrollWheelType();
				scrollAmount = (jint) GetScrollWheelAmount();
				wheelRotation = (jint) ((signed short) HIWORD(mshook->mouseData) / WHEEL_DELTA) * -1;
				
				//Fire mouse wheel event.
				objMouseWheelEvent = (*env)->NewObject(env, clsMouseWheelEvent, idMouseWheelEvent, org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y, (jint) click_count, scrollType, scrollAmount, wheelRotation);
				(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseWheelEvent);
			break;

			/*
			// Settings change message notifies us when change is made
			// through the SystemParametersInfo() API
			case WM_SETTINGCHANGE:
				//TODO Reset all the system props.
				return OnSettingChange( (UINT)wParam );
			*/

			#ifdef DEBUG
			default:
				fprintf(stdout, "LowLevelMouseProc(): Unhandled mouse event. (%X)\n", (unsigned int) wParam);
			break;
			#endif
		}

		//Handle any possible JNI issue that may have occured.
		if ((*env)->ExceptionCheck(env) == JNI_TRUE) {
			#ifdef DEBUG
			fprintf(stderr, "LowLevelMouseProc(): JNI error occurred!\n");
			(*env)->ExceptionDescribe(env);
			#endif
			(*env)->ExceptionClear(env);
		}
	}

	return CallNextHookEx(handleMouseHook, nCode, wParam, lParam);
}

//
// Handle the WM_SETTINGCHANGE message and cache changes to
// SPI_GETWHEELSCROLLLINES and SPI_GETWHEELSCROLLCHARS which indicate
// the amount to scroll when a scrolling message is handled.
//
/*
static LRESULT CALLBACK SettingChangeProc(UINT setting) {
	if (setting == SPI_SETWHEELSCROLLLINES) {
		linesToScrollUserSetting = GetLinesToScrollUserSetting();
	}
	else if (setting == SPI_SETWHEELSCROLLCHARS) {
		charsToScrollUserSetting = GetCharsToScrollUserSetting();
	}

	return 0;
}
*/

static DWORD WINAPI ThreadProc(LPVOID UNUSED(lpParameter)) {
	DWORD status = RETURN_FAILURE;
	JNIEnv * env = NULL;

	//Create the native hooks.
	handleKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0);
	
	if (handleKeyboardHook == NULL) {
		#ifdef DEBUG
		fprintf(stderr, "ThreadProc(): SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0) failed!\n");
		#endif

		thread_ex.class = NATIVE_HOOK_EXCEPTION;
		thread_ex.message = "Failed to hook low level keyboard events";
	}
	#ifdef DEBUG
	else {
		fprintf(stdout, "ThreadProc(): SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0) successful.\n");
	}
	#endif
	

	handleMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0);
	if (handleMouseHook == NULL) {
		#ifdef DEBUG
		fprintf(stderr, "ThreadProc(): SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0) failed!\n");
		#endif
		
		thread_ex.class = NATIVE_HOOK_EXCEPTION;
		thread_ex.message = "Failed to hook low level mouse events";

	}
	#ifdef DEBUG
	else {
		fprintf(stdout, "ThreadProc(): SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0) successful.\n");
	}
	#endif


	//If we did not encounter a problem, start processing events.
	if (handleKeyboardHook != NULL && handleMouseHook != NULL) {
		if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
			#ifdef DEBUG
			fprintf(stdout, "ThreadProc(): Attached to JVM successful.\n");
			#endif

			//Set the exit status.
			status = RETURN_SUCCESS;

			//Signal that we have passed the thread initialization.
			SetEvent(hookEventHandle);

			//Block until the thread receives an WM_QUIT request.
			MSG message;
			while (GetMessage(&message, (HWND) -1, 0, 0 ) > 0) {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else {
			#ifdef DEBUG
			fprintf(stderr, "ThreadProc(): AttachCurrentThread() failed!\n");
			#endif

			thread_ex.class = NATIVE_HOOK_EXCEPTION;
			thread_ex.message = "Failed to attach the native thread to the virtual machine";
		}

	}

	//Destroy the native hooks.
	if (handleKeyboardHook != NULL) {
		UnhookWindowsHookEx(handleKeyboardHook);
		handleKeyboardHook = NULL;
	}

	if (handleMouseHook != NULL) {
		UnhookWindowsHookEx(handleMouseHook);
		handleMouseHook = NULL;
	}

	#ifdef DEBUG
	fprintf(stdout, "ThreadProc(): complete.\n");
	#endif

	//Detach this thread from the JVM
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jni_version) == JNI_OK) {
		(*jvm)->DetachCurrentThread(jvm);
	}

	//Make sure we signal that we have passed any exception throwing code.
	//This should only make a difference if we had an initialization exception.
	SetEvent(hookEventHandle);

	ExitThread(status);
}

int StartNativeThread() {
	int status = RETURN_FAILURE;

	//Make sure the native thread is not already running.
	if (IsNativeThreadRunning() != true) {
		//Create event handle for the thread hook.
		hookEventHandle = CreateEvent(NULL, TRUE, FALSE, "hookEventHandle");

		//Create all the global references up front to save time in the callbacks.
		if (CreateJNIGlobals() == RETURN_SUCCESS) {
			//Initialize Native Input Functions
			LoadInputHelper();

			LPTHREAD_START_ROUTINE lpStartAddress = &ThreadProc;
			hookThreadHandle = CreateThread(NULL, 0, lpStartAddress, NULL, 0, &hookThreadId);
			if (hookThreadHandle != INVALID_HANDLE_VALUE) {
				#ifdef DEBUG
				fprintf(stdout, "StartNativeThread(): start successful.\n");
				#endif

				//Wait for any possible thread exceptions to get thrown into the queue.
				WaitForSingleObject(hookEventHandle, INFINITE);

				//TODO Set the return status to the thread exit code.
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
		//Try to exit the thread naturally.
		PostThreadMessage(hookThreadId, WM_QUIT, (WPARAM) NULL, (LPARAM) NULL);
		WaitForSingleObject(hookThreadHandle, 5000);

		CloseHandle(hookThreadHandle);
		hookThreadHandle = NULL;

		//Cleanup Native Input Functions
		UnloadInputHelper();

		status = RETURN_SUCCESS;

		//Destroy all created globals.
		#ifdef DEBUG
		if (DestroyJNIGlobals() == RETURN_FAILURE) {
			fprintf(stderr, "StopNativeThread(): DestroyJNIGlobals() failed!\n");
		}
		#else
		DestroyJNIGlobals();
		#endif

		CloseHandle(hookEventHandle);
	}

	return status;
}

bool IsNativeThreadRunning() {
	DWORD status;
	GetExitCodeThread(hookThreadHandle, &status);

	return status == STILL_ACTIVE;
}
