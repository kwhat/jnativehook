//
// HookTest.cpp
//
#include <windows.h>
#include <jni.h>
#include "org_dotnative_globalhook_keyboard_GlobalKeyHook.h"

//Instance Variables
bool bRunning = TRUE;
HHOOK hookHandle = NULL;
HINSTANCE hInst = NULL;
JavaVM * jvm = NULL;
jobject hookObj = NULL;
jmethodID fireKeyPressed_ID = NULL;
jmethodID fireKeyReleased_ID = NULL;
jclass objExceptionClass = NULL;
DWORD hookThreadId = 0;

void throwException(JNIEnv * env, char * sMessage) {
	if (objExceptionClass != NULL) {
		#ifdef DEBUG
		printf("C++ Exception: %s\n", sMessage);
		#endif
		
		env->ThrowNew(objExceptionClass, sMessage);
	}
	else {
		//Unable to find exception class
		
		#ifdef DEBUG
		printf("C++: Unable to locate exception class.\n");
		#endif
	}
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	JNIEnv * env;
	KBDLLHOOKSTRUCT * p = (KBDLLHOOKSTRUCT *)lParam;
	
	if (jvm->AttachCurrentThread((void **)&env, NULL) >= 0) {
		switch (wParam) {
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				#ifdef DEBUG
				printf("C++: MsgLoop - Key pressed\n");
				#endif
				
				env->CallVoidMethod(hookObj, fireKeyPressed_ID, (jlong) p->time, (jint) p->flags, (jint) p->vkCode, (jchar) char(p->vkCode));
			break;
			
			case WM_KEYUP:
			case WM_SYSKEYUP:
				#ifdef DEBUG
				printf("C++: MsgLoop - Key released\n");
				#endif
				
				env->CallVoidMethod(hookObj, fireKeyReleased_ID, (jlong) p->time, (jint) p->flags, (jint) p->vkCode, (jchar) char(p->vkCode));
			break;
			
			default:
			break;
		}
	}
	else {
		#ifdef DEBUG
		printf("C++: MsgLoop - Error on the attach current thread.\n");
		#endif
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void MsgLoop() {
	MSG message;
	
	while (bRunning && GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

JNIEXPORT void JNICALL Java_org_dotnative_globalhook_keyboard_GlobalKeyHook_registerHook(JNIEnv * env, jobject obj) {
	//Setup exception handleing
	objExceptionClass = env->FindClass("org/dotnative/globalhook/keyboard/GlobalKeyException");
	
	hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0);
	
	if (hookHandle != NULL) {
		#ifdef DEBUG
		printf("C++: SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0) successful\n");
		#endif
		
		UnhookWindowsHookEx(hookHandle);
	}
	else {
		#ifdef DEBUG
		printf("C++: SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0) failed\n");
		#endif
		
		throwException(env, "Failed to hook keyboared using SetWindowsHookEx");
		
		//Naturaly exit so jni exception is thrown.
		return;
	}
	
	hookObj = env->NewGlobalRef(obj);
	jclass cls = env->GetObjectClass(hookObj);
	fireKeyPressed_ID = env->GetMethodID(cls, "fireKeyPressed", "(JIIC)V");
	fireKeyReleased_ID = env->GetMethodID(cls, "fireKeyReleased", "(JIIC)V");
	env->GetJavaVM(&jvm);
	hookThreadId = GetCurrentThreadId();
}

JNIEXPORT void JNICALL Java_org_dotnative_globalhook_keyboard_GlobalKeyHook_startHook(JNIEnv * env, jobject obj) {
	//Call listener
	hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0);
	
	#ifdef DEBUG
	printf("C++: MsgLoop() start successful.\n");
	#endif
	
	MsgLoop();
}

JNIEXPORT void JNICALL Java_org_dotnative_globalhook_keyboard_GlobalKeyHook_unregisterHook(JNIEnv *env, jobject object) {
	if (!UnhookWindowsHookEx(hookHandle)) {
		#ifdef DEBUG
		printf("C++: Java_HookTest_registerHook - Unhook failed\n");
		#endif
	}
	else {
		#ifdef DEBUG
		printf("C++: Java_HookTest_registerHook - Unhook successful\n");
		#endif
	}
	
	if (hookThreadId != 0) {
		#ifdef DEBUG
		printf("C++: unregisterHook - call PostThreadMessage.\n");
		#endif
		
		PostThreadMessage(hookThreadId, WM_QUIT, 0, 0L);
	}
}

JNIEXPORT void JNICALL Java_org_dotnative_globalhook_keyboard_GlobalKeyHook_stopHook(JNIEnv * env, jobject obj) {
	bRunning = FALSE;
	
	#ifdef DEBUG
	printf("C++: MsgLoop() stop successful.\n");
	#endif
}

extern "C" BOOL APIENTRY DllMain(HINSTANCE _hInst, DWORD reason, LPVOID reserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			#ifdef DEBUG
			printf("C++: DllMain - DLL Process attach.\n");
			#endif
			
			hInst = _hInst;
		break;
		
		case DLL_PROCESS_DETACH:
			#ifdef DEBUG
			printf("C++: DllMain - DLL Process Detach.\n");
			#endif
		break;
		
		default:
		break;
	}
	
	return TRUE;
}
