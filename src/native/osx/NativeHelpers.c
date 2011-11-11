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

#ifdef COREFOUNDATION
//#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef IOKIT
#include <IOKit/hidsystem/IOHIDLib.h>
#include <IOKit/hidsystem/IOHIDParameter.h>
#endif

#include "NativeErrors.h"

/*
 * Apple doesn't document anything.  Value is the slider value in the system
 * prefernces. That value * 15 is the rate in MS.  66 / that value is the chars
 * per second rate.

Value	MS		Char/Sec

1		15		66

2		30		33
6		90		11
12		180		5.5
30		450		2.2
60		900		1.1
90		1350	0.73
120		1800	0.55


V = MS / 15
V = 66 / Char

MS = V * 15
MS = (66 / Char) * 15

Char = 66 / V
Char = 66 / (MS / 15)
*/

long GetAutoRepeatRate() {
	long value = -1;
	UInt64 rate;

	#ifdef COREFOUNDATION
	CFTypeRef pref_val = CFPreferencesCopyValue(CFSTR("KeyRepeat"), kCFPreferencesAnyApplication, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		if (CFNumberGetValue((CFNumberRef)pref_val, kCFNumberSInt32Type, &rate)) {
			value = (long) rate * 15;
		}
	}
	#endif


	#ifdef IOKIT
	//io_iterator_t iter;
	//kern_return_t kr = IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching(kIOHIDSystemClass), &iter);
	kern_return_t kr = kIOReturnSuccess;
	io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching(kIOHIDSystemClass));

	if (kr == kIOReturnSuccess) {
		//io_service_t service = IOIteratorNext(iter);
		io_connect_t sSystemService;
		kr = IOServiceOpen(service, mach_task_self(), kIOHIDParamConnectType, &sSystemService);

		if (kr == kIOReturnSuccess) {
			IOByteCount size = sizeof(rate);

			kr = IOHIDGetParameter(sSystemService, CFSTR(kIOHIDKeyRepeatKey), (IOByteCount) sizeof(rate), &rate, &size);
			if (kr == kIOReturnSuccess) {
				printf("TEST %lf", ((double) rate) / 1000.0 / 1000.0 / 1000.0);

				//FIXME I have no idea what this is return, no apple docs.
				//size is 8 i get something like 0x00BBC2 out for the rate.  Maybe floor(rate / 1000 /1000)??
				printf("*** Success %lX %i\n", (long) rate, (int) size);
			}
		}
	}

	#endif

	return value;
}

long GetAutoRepeatDelay() {
	long value = -1;
	SInt32 delay = -1;

	#ifdef COREFOUNDATION
	CFTypeRef pref_val = CFPreferencesCopyValue(CFSTR("InitialKeyRepeat"), kCFPreferencesAnyApplication, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		if (CFNumberGetValue((CFNumberRef)pref_val, kCFNumberSInt32Type, &delay)) {
			value = (long) delay;
		}
	}
	#endif

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
	long value = -1;

	#ifdef COREFOUNDATION
	Float32 clicktime = -1;
	CFTypeRef pref_val = CFPreferencesCopyValue(CFSTR("com.apple.mouse.doubleClickThreshold"), kCFPreferencesAnyApplication, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		if (CFNumberGetValue((CFNumberRef)pref_val, kCFNumberFloat32Type, &clicktime)) {
			value = (long) (clicktime * 1000);
		}
	}
	#endif

	return value;
}


void OnLibraryLoad() {
	//Do Nothing.
}

void OnLibraryUnload() {
	//Do Nothing.
}
