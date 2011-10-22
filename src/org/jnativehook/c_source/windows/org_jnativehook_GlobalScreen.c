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

#ifdef UNUSED
#elif defined(__GNUC__)
	#define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
	#define UNUSED(x) /*@unused@*/ x
#else
	#define UNUSED(x) x
#endif

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>
//#include <WinDef.h> //??? DWORD for the MSDLLSTRUCT

#ifdef DEBUG
	#include <stdio.h>
	#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <jni.h>

#include "include/org_jnativehook_GlobalScreen.h"
#include "include/JConvertToNative.h"
#include "WinKeyCodes.h"

//Instance Variables
HHOOK handleKeyboardHook = NULL;
HHOOK handleMouseHook = NULL;
HINSTANCE hInst = NULL;

//JVM and Screen globals.
JavaVM * jvm = NULL;
jobject objGlobalScreen = NULL;
jmethodID idDispatchEvent;

//Java callback classes and method id's
jclass clsKeyEvent, clsMouseEvent;
jmethodID idKeyEvent, idMouseButtonEvent, idMouseMotionEvent;

//Thread information so we can clean up.
HANDLE hookThreadHandle = NULL;
LPDWORD hookThreadId = NULL;

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

jint getModifiers() {
	jint modifiers = 0;

	if (isModifierMask(MOD_LSHIFT) || isModifierMask(MOD_RSHIFT))
		modifiers |= NativeToJModifier(MOD_SHIFT);

	if (isModifierMask(MOD_LCONTROL) || isModifierMask(MOD_RCONTROL))
		modifiers |= NativeToJModifier(MOD_CONTROL);

	if (isModifierMask(MOD_LALT) || isModifierMask(MOD_RALT))
		modifiers |= NativeToJModifier(MOD_ALT);

	if (isModifierMask(MOD_LWIN) || isModifierMask(MOD_RWIN))
		modifiers |= NativeToJModifier(MOD_WIN);

	if (isModifierMask(MOD_LBUTTON))	modifiers |= NativeToJModifier(MOD_LBUTTON);
	if (isModifierMask(MOD_RBUTTON))	modifiers |= NativeToJModifier(MOD_RBUTTON);
	if (isModifierMask(MOD_MBUTTON))	modifiers |= NativeToJModifier(MOD_MBUTTON);
	if (isModifierMask(MOD_XBUTTON1))	modifiers |= NativeToJModifier(MOD_XBUTTON1);
	if (isModifierMask(MOD_XBUTTON2))	modifiers |= NativeToJModifier(MOD_XBUTTON2);

	return modifiers;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//We should already be attached to the JVM at this point.  This should only
	//be a formality that causes a NOOP.
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//MS Keyboard Event Struct Data
	KBDLLHOOKSTRUCT * kbhook = (KBDLLHOOKSTRUCT *) lParam;

	//Java Event Data
	JKeyDatum jkey;
	jint modifiers;

	//Jave Key Event Object.
	jobject objKeyEvent;

	switch(wParam) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			#ifdef DEBUG
				printf("Native: LowLevelProc - Key pressed (%i)\n", (unsigned int) kbhook->vkCode);
			#endif

			//Check and setup modifiers
			if (kbhook->vkCode == VK_LSHIFT)		setModifierMask(MOD_LSHIFT);
			else if (kbhook->vkCode == VK_RSHIFT)	setModifierMask(MOD_RSHIFT);
			else if (kbhook->vkCode == VK_LCONTROL)	setModifierMask(MOD_LCONTROL);
			else if (kbhook->vkCode == VK_RCONTROL)	setModifierMask(MOD_RCONTROL);
			else if (kbhook->vkCode == VK_LMENU)	setModifierMask(MOD_LALT);
			else if (kbhook->vkCode == VK_RMENU)	setModifierMask(MOD_RALT);
			else if (kbhook->vkCode == VK_LWIN)		setModifierMask(MOD_LWIN);
			else if (kbhook->vkCode == VK_RWIN)		setModifierMask(MOD_RWIN);

			jkey = NativeToJKey(kbhook->vkCode);
			modifiers = getModifiers();

			//Fire key pressed event.
			objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_PRESSED, (jlong) kbhook->time, modifiers, kbhook->scanCode, jkey.keycode, jkey.location);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			#ifdef DEBUG
				printf("Native: LowLevelProc - Key released (%i)\n", (unsigned int) kbhook->vkCode);
			#endif

			//Check and setup modifiers
			if (kbhook->vkCode == VK_LSHIFT)		unsetModifierMask(MOD_LSHIFT);
			else if (kbhook->vkCode == VK_RSHIFT)	unsetModifierMask(MOD_RSHIFT);
			else if (kbhook->vkCode == VK_LCONTROL)	unsetModifierMask(MOD_LCONTROL);
			else if (kbhook->vkCode == VK_RCONTROL)	unsetModifierMask(MOD_RCONTROL);
			else if (kbhook->vkCode == VK_LMENU)	unsetModifierMask(MOD_LALT);
			else if (kbhook->vkCode == VK_RMENU)	unsetModifierMask(MOD_RALT);
			else if (kbhook->vkCode == VK_LWIN)		unsetModifierMask(MOD_LWIN);
			else if (kbhook->vkCode == VK_RWIN)		unsetModifierMask(MOD_RWIN);

			jkey = NativeToJKey(kbhook->vkCode);
			modifiers = getModifiers();

			//Fire key released event.
			objKeyEvent = (*env)->NewObject(env, clsKeyEvent, idKeyEvent, JK_NATIVE_KEY_RELEASED, (jlong) kbhook->time, modifiers, kbhook->scanCode, jkey.keycode, jkey.location);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objKeyEvent);
		break;
	}

	#ifdef DEBUG
		if ((*env)->ExceptionOccurred(env)) {
			printf("Native: JNI Error Occurred.\n");
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
		}
	#endif

	return CallNextHookEx(handleKeyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//We should already be attached to the JVM at this point.  This should only
	//be a formality that causes a NOOP.
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//MS Mouse Event Struct Data
	MSLLHOOKSTRUCT * mshook = (MSLLHOOKSTRUCT *) lParam;

	//Java Event Data
	jint jbutton;
	jint modifiers;

	//Java Mouse Event Object.
	jobject objMouseEvent;

	switch(wParam) {
		case WM_LBUTTONDOWN:
			jbutton = NativeToJButton(VK_LBUTTON);
			setModifierMask(MOD_LBUTTON);
		goto BUTTONDOWN;

		case WM_RBUTTONDOWN:
			jbutton = NativeToJButton(VK_RBUTTON);
			setModifierMask(MOD_RBUTTON);
		goto BUTTONDOWN;

		case WM_MBUTTONDOWN:
			jbutton = NativeToJButton(VK_MBUTTON);
			setModifierMask(MOD_MBUTTON);
		goto BUTTONDOWN;

		case WM_XBUTTONDOWN:
		case WM_NCXBUTTONDOWN:
			if (HIWORD(mshook->mouseData) == XBUTTON1) {
				jbutton = NativeToJButton(VK_XBUTTON1);
				setModifierMask(MOD_XBUTTON1);
			}
			else if (HIWORD(mshook->mouseData) == XBUTTON2) {
				jbutton = NativeToJButton(VK_XBUTTON2);
				setModifierMask(MOD_XBUTTON2);
			}
			else {
				jbutton = NativeToJButton(HIWORD(mshook->mouseData));
			}

		BUTTONDOWN:
			#ifdef DEBUG
				printf("Native: MsgLoop - Button pressed (%i)\n", (int) jbutton);
			#endif

			modifiers = getModifiers();

			//Fire mouse pressed event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, JK_NATIVE_MOUSE_PRESSED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y, jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case WM_LBUTTONUP:
			jbutton = NativeToJButton(VK_LBUTTON);
			unsetModifierMask(MOD_LBUTTON);
		goto BUTTONUP;

		case WM_RBUTTONUP:
			jbutton = NativeToJButton(VK_RBUTTON);
			unsetModifierMask(MOD_RBUTTON);
		goto BUTTONUP;

		case WM_MBUTTONUP:
			jbutton = NativeToJButton(VK_MBUTTON);
			unsetModifierMask(MOD_MBUTTON);
		goto BUTTONUP;

		case WM_XBUTTONUP:
		case WM_NCXBUTTONUP:
			if (HIWORD(mshook->mouseData) == XBUTTON1) {
				jbutton = NativeToJButton(VK_XBUTTON1);
				unsetModifierMask(MOD_XBUTTON1);
			}
			else if (HIWORD(mshook->mouseData) == XBUTTON2) {
				jbutton = NativeToJButton(VK_XBUTTON2);
				unsetModifierMask(MOD_XBUTTON2);
			}
			else {
				jbutton = NativeToJButton(HIWORD(mshook->mouseData));
			}

		BUTTONUP:
			#ifdef DEBUG
				printf("Native: MsgLoop - Button released (%i)\n", (int) jbutton);
			#endif

			modifiers = getModifiers();

			//Fire mouse released event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseButtonEvent, JK_NATIVE_MOUSE_RELEASED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y, jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case WM_MOUSEMOVE:
			#ifdef DEBUG
				printf ("Native: MsgLoop - Motion Notified (%li, %li)\n", mshook->pt.x, mshook->pt.y);
			#endif

			modifiers = getModifiers();

			//Fire mouse moved event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseMotionEvent, JK_NATIVE_MOUSE_MOVED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case WM_MOUSEWHEEL:
			#ifdef DEBUG
				printf ("Native: MsgLoop - WM_MOUSEWHEEL (Unimplemented)\n");
			#endif

			//Delta HIWORD(mshook->mouseData)
			//A positive value indicates that the wheel was rotated forward, away from the user; a negative
			//value indicates that the wheel was rotated backward, toward the user. One wheel click is
			//defined as WHEEL_DELTA, which is 120.

		break;

		default:
			#ifdef DEBUG
				printf ("Native: MsgLoop - Unhandled Event Type\n");
			#endif
		break;
	}

	#ifdef DEBUG
		if ((*env)->ExceptionOccurred(env)) {
			printf("Native: JNI Error Occurred.\n");
			(*env)->ExceptionDescribe(env);
			(*env)->ExceptionClear(env);
		}
	#endif

	return CallNextHookEx(handleMouseHook, nCode, wParam, lParam);
}

DWORD WINAPI MsgLoop(LPVOID UNUSED(lpParameter)) {
	JNIEnv * env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) != 0) {
		#ifdef DEBUG
			printf("Native: AttachCurrentThread(jvm, (void **)(&env), NULL) failed\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Failed to attach to the current Java thread");
		return 1; //Naturally exit so JNI exception is thrown.
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

	//Setup the native hooks and their callbacks.
	//TODO Need to check to see if hInst is thread safe... May need to use GetModuleHandle(NULL) here.
	handleKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0);
	if (handleKeyboardHook != NULL) {
		#ifdef DEBUG
			printf("Native: SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0) successful\n");
		#endif
	}
	else {
		#ifdef DEBUG
			printf("Native: SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0) failed\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Failed to hook keyboard using SetWindowsHookEx");
		return 1; //Naturally exit so JNI exception is thrown.
	}

	handleMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0);
	if (handleMouseHook != NULL) {
		#ifdef DEBUG
			printf("Native: SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0) successful\n");
		#endif
	}
	else {
		#ifdef DEBUG
			printf("Native: SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, hInst, 0) failed\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Failed to hook mouse using SetWindowsHookEx");
		return 1; //Naturally exit so JNI exception is thrown.
	}

	//Keep running until we get a WM_QUIT request.
	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		//TODO Look into if we need to keep doing this when using LowLevel Mouse/Keyboard Proc
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	//Make sure we clean up the global objects.
	(*env)->DeleteGlobalRef(env, clsKeyEvent);
	(*env)->DeleteGlobalRef(env, clsMouseEvent);
	(*env)->DeleteGlobalRef(env, objGlobalScreen);

	#ifdef DEBUG
		printf("Native: MsgLoop() stop successful.\n");
	#endif

	return 0;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	long int wkb_rate;
	if (! SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &wkb_rate, 0) ) {
		#ifdef DEBUG
		printf("Native: SPI_GETKEYBOARDSPEED failure\n");
		#endif

		throwException("org/jnativehook/keyboard/NativeKeyException", "Could not determine the keyboard auto repeat rate.");
		return -1; //Naturally exit so JNI exception is thrown.
	}

	#ifdef DEBUG
		printf("Native: SPI_GETKEYBOARDSPEED successful (rate: %ldd)\n", wkb_rate);
	#endif
	return (jlong) wkb_rate;
}

JNIEXPORT jlong JNICALL Java_org_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	long int wkb_delay;
	if (! SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &wkb_delay, 0) ) {
		#ifdef DEBUG
			printf("Native: SPI_GETKEYBOARDDELAY failure\n");
		#endif

		throwException("org/jnativehook/keyboard/NativeKeyException", "Could not determine the keyboard auto repeat rate.");
		return -1; //Naturally exit so JNI exception is thrown.
	}

	#ifdef DEBUG
		printf("Native: SPI_GETKEYBOARDDELAY successful (delay: %ldd)\n", wkb_delay);
	#endif
	return (jlong) wkb_delay;
}

//This is where java attaches to the native machine.  Its kind of like the java + native constructor.
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * vm, void * UNUSED(reserved)) {
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
			printf("Native: JNI_VERSION_1_4 unavailable for use. (default: %X)\n", (unsigned int) jni_ret);
		#endif
	}

	LPTHREAD_START_ROUTINE lpStartAddress = &MsgLoop;
	//LPVOID lpParameter = lpStartAddress;
	//hookThreadHandle = CreateThread( NULL, 0, lpStartAddress, NULL, CREATE_SUSPENDED, &hookThreadId );
	hookThreadHandle = CreateThread( NULL, 0, lpStartAddress, NULL, 0, hookThreadId );
	if( hookThreadHandle == INVALID_HANDLE_VALUE ) {
		#ifdef DEBUG
			printf("Native: MsgLoop() start failure.\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Could not create message loop thread.");
		return JNI_ERR; //Naturally exit so JNI exception is thrown.
	}
	else {
		#ifdef DEBUG
			printf("Native: MsgLoop() start successful.\n");
		#endif
	}

	return jni_ret;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * UNUSED(vm), void * UNUSED(reserved)) {
	if (handleKeyboardHook != NULL) {
		UnhookWindowsHookEx(handleKeyboardHook);
		handleKeyboardHook = NULL;
	}

	if (handleMouseHook != NULL) {
		UnhookWindowsHookEx(handleMouseHook);
		handleMouseHook = NULL;
	}

	//Try to exit the thread naturally.
	PostQuitMessage(0);
	WaitForSingleObject(hookThreadHandle, 250);

	CloseHandle(hookThreadHandle);
	if(hookThreadHandle != NULL) {
		HeapFree(GetProcessHeap(), 0, hookThreadHandle);
		hookThreadHandle = NULL;

		#ifdef DEBUG
			printf("Native: HeapFree successful.\n");
		#endif
	}

	#ifdef DEBUG
		printf("Native: Thread terminated successful.\n");
	#endif
}

BOOL APIENTRY DllMain(HANDLE _hInst, DWORD reason, LPVOID UNUSED(reserved)) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			#ifdef DEBUG
				printf("Native: DllMain - DLL Process Attach.\n");
			#endif

			hInst = (HINSTANCE) _hInst; //GetModuleHandle(NULL)
		break;

		case DLL_PROCESS_DETACH:
			#ifdef DEBUG
				printf("Native: DllMain - DLL Process Detach.\n");
			#endif
		break;
	}

	return true;
}
