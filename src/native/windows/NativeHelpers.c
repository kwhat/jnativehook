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

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>

//Global Variables
HINSTANCE hInst = NULL;

long GetAutoRepeatRate() {
	long value = -1;
	long int rate;

	if (SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &rate, 0)) {
		value = rate;
	}

	return rate;
}

long GetAutoRepeatDelay() {
	long value = -1;
	long int delay;

	if (SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &delay, 0)) {
		value = delay;
	}

	return delay;
}

long GetPointerAccelerationMultiplier() {
	long value = -1;
	int mouse[3]; // 0-Threshold X, 1-Threshold Y and 2-Speed

	if (SystemParametersInfo(SPI_GETMOUSE, 0, &mouse, 0)) {
		value = mouse[2];
	}

	return value;
}

long GetPointerAccelerationThreshold() {
	long value = -1;
	int mouse[3]; // 0-Threshold X, 1-Threshold Y and 2-Speed

	if (SystemParametersInfo(SPI_GETMOUSE, 0, &mouse, 0)) {
		//Average the x and y thresholds.
		value = (wkb_mouse[0] + wkb_mouse[1]) / 2;
	}

	return value;
}

long GetPointerSensitivity() {
	long value = -1;
	PVOID sensitivity;

	if (SystemParametersInfo(SPI_GETMOUSESPEED, 0, sensitivity, 0)) {
		value = *sensitivity;
	}

	return value;
}

long GetMultiClickTime() {
	long value = -1;
	UINT WINAPI clicktime;

	clicktime = GetDoubleClickTime();
	if (clicktime >= 0) {
		value = (long) clicktime;
	}

	return value;
}

long GetScrollWheelType() {
	//FIXME Implement.

	return (long) WHEEL_UNIT_SCROLL;
}

long GetScrollWheelAmount() {
	//FIXME Implement.

	return 3;
}

void OnLibraryLoad() {
	//Do Nothing.
}

void OnLibraryUnload() {
	//Do Nothing.
}

BOOL APIENTRY DllMain(HANDLE _hInst, DWORD reason, LPVOID UNUSED(reserved)) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			hInst = (HINSTANCE) _hInst;
			//hInst = GetModuleHandle(NULL);
		break;
	}

	return TRUE;
}
