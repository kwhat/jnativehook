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

//#include <ApplicationServices/ApplicationServices.h>

#ifdef IOKIT
#include <IOKit/hidsystem/IOHIDLib.h>
#include <IOKit/hidsystem/IOHIDParameter.h>
#endif

#ifdef COREFOUNDATION
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef CARBON
#include <Carbon/Carbon.h>
#endif

#include "NativeErrors.h"

/*
 * Apple's documentation is not very good.  I was finally able to find this
 * information after many hours of googling.  Value is the slider value in the
 * system preferences. That value * 15 is the rate in MS.  66 / the value is the
 * chars per second rate.
 *
 * Value	MS		Char/Sec
 *
 * 1		15		66		//Out of standard range.
 *
 * 2		30		33
 * 6		90		11
 * 12		180		5.5
 * 30		450		2.2
 * 60		900		1.1
 * 90		1350	0.73
 * 120		1800	0.55
 *
 * V = MS / 15
 * V = 66 / CharSec
 *
 * MS = V * 15
 * MS = (66 / CharSec) * 15
 *
 * CharSec = 66 / V
 * CharSec = 66 / (MS / 15)
 */

long GetAutoRepeatRate() {
	#if defined IOKIT || defined COREFOUNDATION || defined CARBON
	bool successful = false;
	SInt64 rate;
	#endif

	long value = -1;

	#ifdef IOKIT
	if (!successful) {
		io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching(kIOHIDSystemClass));
		if (service) {
			kern_return_t kren_ret = kIOReturnError;
			io_connect_t connection;

			kren_ret = IOServiceOpen(service, mach_task_self(), kIOHIDParamConnectType, &connection);
			if (kren_ret == kIOReturnSuccess) {
				IOByteCount size = sizeof(rate);

				kren_ret = IOHIDGetParameter(connection, CFSTR(kIOHIDKeyRepeatKey), (IOByteCount) sizeof(rate), &rate, &size);
				if (kren_ret == kIOReturnSuccess) {
					/* This is in some undefined unit of time that if we happen
					 * to multiply by 900 gives us the time in milliseconds. We
					 * add 0.5 to the result so that when we cast to long we
					 * actually get a rounded result.  Saves the math.h depend.
					 *
					 *    33,333,333.0 / 1000.0 / 1000.0 / 1000.0 == 0.033333333	//Fast
					 *   100,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 0.1
  					 *   200,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 0.2
  					 *   500,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 0.5
					 * 1,000,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 1
					 * 1,500,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 1.5
					 * 2,000,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 2				//Slow
					 */
					value = (long) (900.0 * ((double) rate) / 1000.0 / 1000.0 / 1000.0 + 0.5);
					successful = true;
				}
			}
		}
	}
	#endif

	#ifdef COREFOUNDATION
	if (!successful) {
		CFTypeRef pref_val = CFPreferencesCopyValue(CFSTR("KeyRepeat"), kCFPreferencesAnyApplication, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
		if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
			if (CFNumberGetValue((CFNumberRef) pref_val, kCFNumberSInt32Type, &rate)) {
				//This is the slider value, we must multiply by 15 to convert to milliseconds.
				value = (long) rate * 15;
				successful = true;
			}
		}
	}
	#endif

	#ifdef CARBON
	if (!successful) {
		//Apple documentation states that value is in 'ticks'. I am not sure
		//what that means, but it looks a lot like the arbitrary slider value.
		rate = LMGetKeyRepThresh();
		if (rate > -1) {
			//This is the slider value, we must multiply by 15 to convert to milliseconds.
			value = (long) rate * 15;
			successful = true;
		}
	}
	#endif

	return value;
}

long GetAutoRepeatDelay() {
	#if defined IOKIT || defined COREFOUNDATION || defined CARBON
	bool successful = false;
	SInt64 delay;
	#endif

	long value = -1;

	#ifdef IOKIT
	if (!successful) {
		io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching(kIOHIDSystemClass));
		if (service) {
			kern_return_t kren_ret = kIOReturnError;
			io_connect_t connection;

			kren_ret = IOServiceOpen(service, mach_task_self(), kIOHIDParamConnectType, &connection);
			if (kren_ret == kIOReturnSuccess) {
				IOByteCount size = sizeof(delay);

				kren_ret = IOHIDGetParameter(connection, CFSTR(kIOHIDInitialKeyRepeatKey), (IOByteCount) sizeof(delay), &delay, &size);
				if (kren_ret == kIOReturnSuccess) {
					/* This is in some undefined unit of time that if we happen
					 * to multiply by 900 gives us the time in milliseconds. We
					 * add 0.5 to the result so that when we cast to long we
					 * actually get a rounded result.  Saves the math.h depend.
					 *
					 *    33,333,333.0 / 1000.0 / 1000.0 / 1000.0 == 0.033333333	//Fast
					 *   100,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 0.1
  					 *   200,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 0.2
  					 *   500,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 0.5
					 * 1,000,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 1
					 * 1,500,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 1.5
					 * 2,000,000,000.0 / 1000.0 / 1000.0 / 1000.0 == 2				//Slow
					 */
					value = (long) (900.0 * ((double) delay) / 1000.0 / 1000.0 / 1000.0 + 0.5);
					successful = true;
				}
			}
		}
	}
	#endif

	#ifdef COREFOUNDATION
	CFTypeRef pref_val = CFPreferencesCopyValue(CFSTR("InitialKeyRepeat"), kCFPreferencesAnyApplication, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		if (CFNumberGetValue((CFNumberRef) pref_val, kCFNumberSInt32Type, &delay)) {
			//This is the slider value, we must multiply by 15 to convert to milliseconds.
			value = (long) delay * 15;
			successful = true;
		}
	}
	#endif

	#ifdef CARBON
	if (!successful) {
		//Apple documentation states that value is in 'ticks'. I am not sure
		//what that means, but it looks a lot like the arbitrary slider value.
		delay = LMGetKeyThresh();
		if (delay > -1) {
			//This is the slider value, we must multiply by 15 to convert to milliseconds.
			value = (long) delay * 15;
			successful = true;
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
	//"com.apple.mouse.scaling" ?
	return -1;
}

long GetMultiClickTime() {
	#if defined IOKIT || defined COREFOUNDATION || defined CARBON
	bool successful = false;
	#if defined IOKIT || defined CARBON
	//This needs to be defiend only if we have IOKIT or Carbon
	SInt64 time;
	#endif
	#endif

	long value = -1;

	#ifdef IOKIT
	if (!successful) {
		io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching(kIOHIDSystemClass));
		if (service) {
			kern_return_t kren_ret = kIOReturnError;
			io_connect_t connection;

			kren_ret = IOServiceOpen(service, mach_task_self(), kIOHIDParamConnectType, &connection);
			if (kren_ret == kIOReturnSuccess) {
				IOByteCount size = sizeof(time);

				kren_ret = IOHIDGetParameter(connection, CFSTR(kIOHIDClickTimeKey), (IOByteCount) sizeof(time), &time, &size);
				if (kren_ret == kIOReturnSuccess) {
					/* This is in some undefined unit of time that if we happen
					 * to multiply by 900 gives us the time in milliseconds. We
					 * add 0.5 to the result so that when we cast to long we
					 * actually get a rounded result.  Saves the math.h depend.
					 */
					value = (long) (900.0 * ((double) time) / 1000.0 / 1000.0 / 1000.0 + 0.5);
					successful = true;
				}
			}
		}
	}
	#endif

	#ifdef COREFOUNDATION
	Float32 clicktime;
	CFTypeRef pref_val = CFPreferencesCopyValue(CFSTR("com.apple.mouse.doubleClickThreshold"), kCFPreferencesAnyApplication, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
	if (pref_val != NULL && CFGetTypeID(pref_val) == CFNumberGetTypeID()) {
		if (CFNumberGetValue((CFNumberRef) pref_val, kCFNumberFloat32Type, &clicktime)) {
			/* This is in some undefined unit of time that if we happen
			 * to multiply by 900 gives us the time in milliseconds.  It is
			 * completely possible that this value is in seconds and should be
			 * multiplied by 1000 but because IOKit values are undocumented and
			 * I have no idea what a Carbon 'tick' is so there really is no way
			 * to confirm this.
			 */
			value = (long) (clicktime * 900);
		}
	}
	#endif

	#ifdef CARBON
	if (!successful) {
		//Apple documentation states that value is in 'ticks'. I am not sure
		//what that means, but it looks a lot like the arbitrary slider value.
		time = GetDblTime();
		if (time > -1) {
			//This is the slider value, we must multiply by 15 to convert to milliseconds.
			value = (long) time * 15;
			successful = true;
		}
	}
	#endif

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
