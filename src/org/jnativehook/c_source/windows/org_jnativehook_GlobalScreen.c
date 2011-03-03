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
bool isRunning = true;
HHOOK handleKeyboardHook = NULL;
HHOOK handleMouseHook = NULL;
HINSTANCE hInst = NULL;

JavaVM * jvm = NULL;
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


LRESULT WINAPI LowLevelProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Class and getInstance method id for the GlobalScreen Object
	jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
	jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");

	//A reference to the GlobalScreen Object
	jobject objGlobalScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);


	//MS Event Struct Data
	KBDLLHOOKSTRUCT * kbhook = (KBDLLHOOKSTRUCT *)lParam;
	MSLLHOOKSTRUCT * mshook = (MSLLHOOKSTRUCT *)lParam;

	//Event Data
	unsigned int vk_code = 0;
	unsigned int modifiers = 0;

	JKeyDatum jkey;
	jint jbutton;
	jclass clsKeyEvent, clsMouseEvent;
	jmethodID idKeyEvent, idMouseEvent;
	jmethodID idDispatchEvent = (*env)->GetMethodID(env, clsGlobalScreen, "dispatchEvent", "(Lorg/jnativehook/NativeInputEvent;)V");
	jobject objKeyEvent, objMouseEvent;

	switch (wParam) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			#ifdef DEBUG
				printf("Native: LowLevelProc - Key pressed (%i)\n", (unsigned int) kbhook->vkCode);
			#endif

			//Class and Constructor for the NativeKeyEvent Object
			clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
			idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

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
				printf("Native: LowLevelProc - Key released(%i)\n", (unsigned int) kbhook->vkCode);
			#endif

			//Class and Constructor for the NativeKeyEvent Object
			clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
			idKeyEvent = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIIII)V");

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


		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_NCXBUTTONDOWN:
			if (wParam == WM_LBUTTONDOWN) {
				vk_code = VK_LBUTTON;
				setModifierMask(MOD_LBUTTON);
			}
			else if (wParam == WM_RBUTTONDOWN) {
				vk_code = VK_RBUTTON;
				setModifierMask(MOD_RBUTTON);
			}
			else if (wParam == WM_MBUTTONDOWN) {
				vk_code = VK_MBUTTON;
				setModifierMask(MOD_MBUTTON);
			}
			else if (wParam == WM_XBUTTONDOWN || wParam == WM_NCXBUTTONDOWN) {
				if (HIWORD(mshook->mouseData) == XBUTTON1) {
					vk_code = VK_XBUTTON1;
					setModifierMask(MOD_XBUTTON1);
				}
				else if (HIWORD(mshook->mouseData) == XBUTTON2) {
					vk_code = VK_XBUTTON2;
					setModifierMask(MOD_XBUTTON2);
				}
			}

			#ifdef DEBUG
				printf("Native: MsgLoop - Button pressed (%i)\n", vk_code);
			#endif

			//Class and Constructor for the NativeMouseEvent Object
			clsMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
			idMouseEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");

			jbutton = NativeToJButton(vk_code);
			modifiers = getModifiers();

			//Fire mouse pressed event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseEvent, JK_NATIVE_MOUSE_PRESSED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y, jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;


		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		case WM_NCXBUTTONUP:
			if (wParam == WM_LBUTTONUP) {
				vk_code = VK_LBUTTON;
				unsetModifierMask(MOD_LBUTTON);
			}
			else if (wParam == WM_RBUTTONUP) {
				vk_code = VK_RBUTTON;
				unsetModifierMask(MOD_RBUTTON);
			}
			else if (wParam == WM_MBUTTONUP) {
				vk_code = VK_MBUTTON;
				unsetModifierMask(MOD_MBUTTON);
			}
			else if (wParam == WM_XBUTTONUP || wParam == WM_NCXBUTTONUP) {
				if (HIWORD(mshook->mouseData) == XBUTTON1) {
					vk_code = VK_XBUTTON1;
					unsetModifierMask(MOD_XBUTTON1);
				}
				else if (HIWORD(mshook->mouseData) == XBUTTON2) {
					vk_code = VK_XBUTTON2;
					unsetModifierMask(MOD_XBUTTON2);
				}
			}

			#ifdef DEBUG
				printf("Native: MsgLoop - Button released(%i)\n", vk_code);
			#endif

			//Class and Constructor for the NativeMouseEvent Object
			clsMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
			idMouseEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIIII)V");

			jbutton = NativeToJButton(vk_code);
			modifiers = getModifiers();

			//Fire mouse released event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseEvent, JK_NATIVE_MOUSE_RELEASED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y, jbutton);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case WM_MOUSEMOVE:
			#ifdef DEBUG
				printf ("Native: MsgLoop - Motion Notified (%li,%li)\n", mshook->pt.x, mshook->pt.y);
			#endif

			//Class and Constructor for the NativeMouseEvent Object
			clsMouseEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
			idMouseEvent = (*env)->GetMethodID(env, clsMouseEvent, "<init>", "(IJIII)V");

			modifiers = getModifiers();

			//Fire mouse moved event.
			objMouseEvent = (*env)->NewObject(env, clsMouseEvent, idMouseEvent, JK_NATIVE_MOUSE_MOVED, (jlong) mshook->time, modifiers, (jint) mshook->pt.x, (jint) mshook->pt.y);
			(*env)->CallVoidMethod(env, objGlobalScreen, idDispatchEvent, objMouseEvent);
		break;

		case WM_MOUSEWHEEL:
			#ifdef DEBUG
				printf ("Native: MsgLoop - WM_MOUSEWHEEL (Unimplemented)\n");
			#endif
		default:
			#ifdef DEBUG
				printf ("Native: MsgLoop - Unhandled Event Type\n");
			#endif
		break;
	}

	#ifdef DEBUG
		if ((*env)->ExceptionOccurred(env)) {
			printf("Native: JNI Error Occurred.\n");
			((*env)->ExceptionDescribe(env));
		}
	#endif

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//void MsgLoop() {
DWORD WINAPI MsgLoop(LPVOID UNUSED(lpParameter)) {
	MSG message;

	//Setup the native hooks and their callbacks.
	handleKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0);
	if (handleKeyboardHook != NULL) {
		#ifdef DEBUG
			printf("Native: SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0) successful\n");
		#endif
	}
	else {
		#ifdef DEBUG
			printf("Native: SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0) failed\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Failed to hook keyboard using SetWindowsHookEx");
		return 1; //Naturaly exit so jni exception is thrown.
	}


	handleMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, GetModuleHandle(NULL), 0);
	if (handleMouseHook != NULL) {
		#ifdef DEBUG
			printf("Native: SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, hInst, 0) successful\n");
		#endif
	}
	else {
		#ifdef DEBUG
			printf("Native: SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, hInst, 0) failed\n");
		#endif

		throwException("org/jnativehook/NativeHookException", "Failed to hook mouse using SetWindowsHookEx");
		return 1; //Naturaly exit so jni exception is thrown.
	}


	signed char bRet;
	while (isRunning && (bRet = GetMessage(&message, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
			#ifdef DEBUG
				printf("Native: MsgLoop() GetMessage returend -1.\n");
			#endif
			//TODO Not sure if this can be recovered.
		}
		else {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

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
		return -1; //Naturaly exit so jni exception is thrown.
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
		return -1; //Naturaly exit so jni exception is thrown.
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


	isRunning = true;
	LPTHREAD_START_ROUTINE lpStartAddress = &MsgLoop;
	//LPVOID lpParameter = lpStartAddress;
	//hookThreadHandle = CreateThread( NULL, 0, lpStartAddress, NULL, CREATE_SUSPENDED, &hookThreadId );
	hookThreadHandle = CreateThread( NULL, 0, lpStartAddress, NULL, 0, hookThreadId );
	if( hookThreadHandle == INVALID_HANDLE_VALUE ) {
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
	if (handleKeyboardHook != NULL) {
		UnhookWindowsHookEx(handleKeyboardHook);
		handleKeyboardHook = NULL;
	}

	//Try to exit the thread naturally.
	isRunning = false;
	WaitForSingleObject(hookThreadHandle, 1000);

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

BOOL APIENTRY DllMain(HINSTANCE _hInst, DWORD reason, LPVOID UNUSED(reserved)) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			#ifdef DEBUG
				printf("Native: DllMain - DLL Process Attach.\n");
			#endif

			hInst = _hInst; //GetModuleHandle(NULL)
		break;

		case DLL_PROCESS_DETACH:
			#ifdef DEBUG
				printf("Native: DllMain - DLL Process Detach.\n");
			#endif
		break;

		default:
		break;
	}

	return true;
}
