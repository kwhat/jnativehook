//
// HookTest.cpp
//
#include <windows.h>
#include <jni.h>
#include "org_dotnative_globalhook_keyboard_GlobalKeyHook.h"

HINSTANCE hInst = NULL;
JavaVM * jvm = NULL;
jobject hookObj = NULL;
jmethodID fireKeyPressed_ID = NULL;
jmethodID fireKeyReleased_ID = NULL;
jmethodID fireKeyTyped_ID = NULL;
DWORD hookThreadId = 0;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	JNIEnv * env;
	KBDLLHOOKSTRUCT * p = (KBDLLHOOKSTRUCT *)lParam;
	
	if (jvm->AttachCurrentThread((void **)&env, NULL) >= 0) {
		switch (wParam) {
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				printf("C++: LowLevelKeyboardProc - Key pressed\n");
				env->CallVoidMethod(hookObj, fireKeyPressed_ID, (jlong) p->time, (jint) p->flags, (jint) p->vkCode, (jchar) char(p->vkCode));
			break;
			case WM_KEYUP:
			case WM_SYSKEYUP:
				printf("C++: LowLevelKeyboardProc - Key released\n");
				env->CallVoidMethod(hookObj, fireKeyReleased_ID, (jlong) p->time, (jint) p->flags, (jint) p->vkCode, (jchar) char(p->vkCode));
			break;
			default:
			break;
		}
	}
	else {
		printf("C++: LowLevelKeyboardProc - Error on the attach current thread.\n");
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void MsgLoop() {
	MSG message;

	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

JNIEXPORT void JNICALL Java_org_dotnative_globalhook_keyboard_GlobalKeyHook_registerHook(JNIEnv * env, jobject obj) {
	HHOOK hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInst, 0);
	
	if (hookHandle == NULL) {
		printf("C++: Java_HookTest_registerHook - Hook failed!\n");
		return;
	}
	else {
		printf("C++: Java_HookTest_registerHook - Hook successful\n");
	}
	
	hookObj = env->NewGlobalRef(obj);
	jclass cls = env->GetObjectClass(hookObj);
	fireKeyPressed_ID = env->GetMethodID(cls, "fireKeyPressed", "(JIIC)V");
	fireKeyReleased_ID = env->GetMethodID(cls, "fireKeyReleased", "(JIIC)V");
	fireKeyTyped_ID = env->GetMethodID(cls, "fireKeyTyped", "(JIIC)V");
	env->GetJavaVM(&jvm);
	hookThreadId = GetCurrentThreadId();
	
	MsgLoop();
	
	if (!UnhookWindowsHookEx(hookHandle))
		printf("C++: Java_HookTest_registerHook - Unhook failed\n");
	else
		printf("C++: Java_HookTest_registerHook - Unhook successful\n");
}

JNIEXPORT void JNICALL Java_org_dotnative_globalhook_keyboard_GlobalKeyHook_unregisterHook(JNIEnv *env, jobject object) {
	if (hookThreadId == 0)
		return;
	
	printf("C++: Java_HookTest_unRegisterHook - call PostThreadMessage.\n");
	PostThreadMessage(hookThreadId, WM_QUIT, 0, 0L);
}

extern "C" BOOL APIENTRY DllMain(HINSTANCE _hInst, DWORD reason, LPVOID reserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			printf("C++: DllMain - DLL_PROCESS_ATTACH.\n");
			hInst = _hInst;
		break;
		default:
		break;
	}
	
	return TRUE;
}
