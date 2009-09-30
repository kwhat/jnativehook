#include<stdio.h>
#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>

LRESULT CALLBACK LowLevelProc(int nCode, WPARAM wParam, LPARAM lParam) {
	MSLLHOOKSTRUCT * p = (MSLLHOOKSTRUCT *)lParam;
	int fwButton = 0;

	switch (wParam) {
		case WM_MOUSEMOVE:
		break;

		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:


		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:

		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:

		default:
			printf("fwButton (%i) (%i)\t", HIWORD(wParam), LOWORD(wParam));
			printf("MSLLHOOKSTRUCT (%x)\t", p->mouseData);
			printf("pressed (%i) (%i)\n", wParam, lParam);
		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

main() {
	//BOOL RegisterHotKey(HWND hWnd, int id, UINT fsModifiers, UINT vk);

	/*
	BOOL ret = RegisterHotKey(NULL, 0x42, WM_KEYUP, 0x42);
	if ( ret ) {
		printf("success\n");
	}
	else {
		printf("fail\n");
		return 1;
	}
	*/

	//HHOOK hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, GetModuleHandle(NULL), 0);
	HHOOK hookHandle = SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, GetModuleHandle(NULL), 0);
	if (hookHandle != NULL) {
		printf("success\n");
	}
	else {
		printf("failed\n");
		return 1;
	}




	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		TranslateMessage(&msg);
	        DispatchMessage(&msg);
	}

	return 0;
}
