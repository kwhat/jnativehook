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

#include <ApplicationServices/ApplicationServices.h>

#include "JNativeHook.h"
#include "NativeThread.h"
#include "org_jnativehook_GlobalScreen.h"

long GetAutoRepeatRate() {
	long value = -1;
	SInt32 rate;

	CFTypeRef pref_val = CFPreferencesCopyAppValue(CFSTR("KeyRepeat"), kCFPreferencesCurrentApplication);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		if (CFNumberGetValue((CFNumberRef)pref_val, kCFNumberSInt32Type, &rate)) {
			value = (long) rate;
		}
	}

	return value;
}

long GetAutoRepeatDelay() {
	long value = -1;
	SInt32 delay = -1;

	CFTypeRef pref_val = CFPreferencesCopyAppValue(CFSTR("InitialKeyRepeat"), kCFPreferencesCurrentApplication);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		if (CFNumberGetValue((CFNumberRef)pref_val, kCFNumberSInt32Type, &delay)) {
			value = (long) delay;
		}
	}

	return value;
}


long GetPointerAccelerationMultiplier() {
	//FIXME Implement.
	return -1;
}

long GetPointerAccelerationThreshold() {
	//FIXME Implement.
	return -1;
}


long GetPointerSensitivity() {
	//FIXME Implement.
	return -1;
}

long GetDoubleClickTime() {
	//FIXME Implement.
	return -1;
}


void OnLibraryLoad() {
	//Do Nothing.
}

void OnLibraryUnload() {
	//Do Nothing.
}
