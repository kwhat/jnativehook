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
#include <nativehook.h>

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>

#include "NativeErrors.h"
#include "WinUnicodeHelper.h"

NATIVEHOOK_API long int get_autorepeat_rate() {
	long int value = -1;
	long int rate;

	if (SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &rate, 0)) {
		value = rate;
	}

	return value;
}

NATIVEHOOK_API long int get_auto_repeat_delay() {
	long int value = -1;
	long int delay;

	if (SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &delay, 0)) {
		value = delay;
	}

	return value;
}

NATIVEHOOK_API long int GetPointerAccelerationMultiplier() {
	long int value = -1;
	int mouse[3]; // 0-Threshold X, 1-Threshold Y and 2-Speed.

	if (SystemParametersInfo(SPI_GETMOUSE, 0, &mouse, 0)) {
		value = mouse[2];
	}

	return value;
}

NATIVEHOOK_API long int get_pointer_acceleration_threshold() {
	long int value = -1;
	int mouse[3]; // 0-Threshold X, 1-Threshold Y and 2-Speed.

	if (SystemParametersInfo(SPI_GETMOUSE, 0, &mouse, 0)) {
		// Average the x and y thresholds.
		value = (mouse[0] + mouse[1]) / 2;
	}

	return value;
}

NATIVEHOOK_API long int get_pointer_sensitivity() {
	long int value = -1;
	int sensitivity;

	if (SystemParametersInfo(SPI_GETMOUSESPEED, 0, &sensitivity, 0)) {
		value = sensitivity;
	}

	return value;
}

NATIVEHOOK_API long int get_multi_click_time() {
	long int value = -1;
	UINT clicktime;

	clicktime = GetDoubleClickTime();
	value = (long int) clicktime;

	return value;
}
