#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>
#include <stdio.h>

LRESULT WINAPI LowLevelProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//MS Event Struct Data
	KBDLLHOOKSTRUCT * kbhook = (KBDLLHOOKSTRUCT *) lParam;
	MSLLHOOKSTRUCT * mshook = (MSLLHOOKSTRUCT *) lParam;

	//Event Data
	unsigned int vk_code = 0;

	switch(wParam) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (kbhook->vkCode == VK_ESCAPE) {
				PostQuitMessage(0);
			}

			printf("Key Press - %i\n", (unsigned int) kbhook->vkCode);
		break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			printf("Key Release - %i\n", (unsigned int) kbhook->vkCode);
		break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_NCXBUTTONDOWN:
			if (wParam == WM_LBUTTONDOWN) {
				vk_code = VK_LBUTTON;
			}
			else if (wParam == WM_RBUTTONDOWN) {
				vk_code = VK_RBUTTON;
			}
			else if (wParam == WM_MBUTTONDOWN) {
				vk_code = VK_MBUTTON;
			}
			else if (wParam == WM_XBUTTONDOWN || wParam == WM_NCXBUTTONDOWN) {
				if (HIWORD(mshook->mouseData) == XBUTTON1) {
					vk_code = VK_XBUTTON1;
				}
				else if (HIWORD(mshook->mouseData) == XBUTTON2) {
					vk_code = VK_XBUTTON2;
				}
			}

			printf("Button Press - %i\n", vk_code);
		break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		case WM_NCXBUTTONUP:
			if (wParam == WM_LBUTTONDOWN) {
				vk_code = VK_LBUTTON;
			}
			else if (wParam == WM_RBUTTONDOWN) {
				vk_code = VK_RBUTTON;
			}
			else if (wParam == WM_MBUTTONDOWN) {
				vk_code = VK_MBUTTON;
			}
			else if (wParam == WM_XBUTTONDOWN || wParam == WM_NCXBUTTONDOWN) {
				if (HIWORD(mshook->mouseData) == XBUTTON1) {
					vk_code = VK_XBUTTON1;
				}
				else if (HIWORD(mshook->mouseData) == XBUTTON2) {
					vk_code = VK_XBUTTON2;
				}
			}

			printf("Button Release - %i\n", vk_code);
		break;

		case WM_MOUSEMOVE:
			printf("Motion Notify - %i, %i\n", (unsigned int) mshook->pt.x, (unsigned int) mshook->pt.y);
		break;

		default:
			printf("Unknown Event - %x\n", wParam);
		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main( int argc, const char* argv[] ) {
	MSG message;
	HINSTANCE hInst = NULL;

	//Setup the native hooks and their callbacks.
	HHOOK handleKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0);
	if (handleKeyboardHook == NULL) {
		printf("SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0) failed\n");
		return 1;
	}

	HHOOK handleMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, GetModuleHandle(NULL), 0);
	if (handleMouseHook == NULL) {
		printf("SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, hInst, 0) failed\n");
		return 1;
	}

	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(handleKeyboardHook);
	UnhookWindowsHookEx(handleMouseHook);

	return 0;
}
