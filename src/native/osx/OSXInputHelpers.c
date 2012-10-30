/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

#include <stdbool.h>
#include "OSXInputHelpers.h"

// Keyboard Upper 16 / Mouse Lower 16
static CGEventFlags currModifierMask = 0x00000000;
#ifdef CARBON_LEGACY
static void *inputData = NULL;
#else
static CFDataRef inputData = NULL;
#endif
static UInt32 currDeadkeyState = 0;

void SetModifierMask(CGEventFlags mask) {
	currModifierMask |= mask;
}

void UnsetModifierMask(CGEventFlags mask) {
	currModifierMask ^= mask;
}

CGEventFlags GetModifiers() {
	return currModifierMask;
}

void KeyCodeToString(CGEventRef event, UniCharCount size, UniCharCount *length, UniChar *buffer) {
	// Data was not loaded, try to reload.
	if (inputData == NULL) {
		LoadInputHelper();
	}

	if (inputData != NULL) {
		#ifdef CARBON_LEGACY
		const UCKeyboardLayout *keyboardLayout = (const UCKeyboardLayout *) inputData;
		#else
		const UCKeyboardLayout *keyboardLayout = (const UCKeyboardLayout*) CFDataGetBytePtr(inputData);
		#endif

		if (keyboardLayout != NULL) {
			//Extract keycode and modifier information.
			CGKeyCode keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			CGEventFlags modifiers = CGEventGetFlags(event);

			// If the caps lock is set, make sure the shift mask is present.
			if (modifiers & kCGEventFlagMaskAlphaShift) {
				// Turn off the caps lock mask.
				modifiers &= ~kCGEventFlagMaskAlphaShift;

				// Turn on the shift mask.
				modifiers |= kCGEventFlagMaskShift;
			}

			// Disable all command modifiers for translation.  This is required
			// so UCKeyTranslate will provide a keysym for the separate event.
			static const CGEventFlags cmdModifiers = kCGEventFlagMaskCommand | kCGEventFlagMaskControl | kCGEventFlagMaskAlternate;
			modifiers &= ~cmdModifiers;

			OSStatus status = noErr;
			if (currDeadkeyState == 0) {
				// No previous deadkey, attempt a lookup.
				status = UCKeyTranslate(
									keyboardLayout,
									keycode,
									kUCKeyActionDown,
									(modifiers >> 16) & 0xFF, //(modifiers >> 16) & 0xFF, || (modifiers >> 8) & 0xFF,
									LMGetKbdType(),
									kNilOptions, //kNilOptions, //kUCKeyTranslateNoDeadKeysMask
									&currDeadkeyState,
									size,
									length,
									buffer);
			}
			else {
				// The previous key was a deadkey, lookup what it should be.
				status = UCKeyTranslate(
									keyboardLayout,
									keycode,
									kUCKeyActionDown,
									(modifiers >> 16) & 0xFF, //No Modifier
									LMGetKbdType(),
									kNilOptions, //kNilOptions, //kUCKeyTranslateNoDeadKeysMask
									&currDeadkeyState,
									size,
									length,
									buffer);
			}

			if (status != noErr) {
				// Make sure the buffer length is zero.
				*length = 0;
			}
		}
	}

	// Fallback to CGEventKeyboardGetUnicodeString if we were unable to use UCKeyTranslate().
	if (*length == 0) {
		CGEventKeyboardGetUnicodeString(event, size, length, buffer);
	}
}

void LoadInputHelper() {
	if (inputData == NULL) {
		#ifdef CARBON_LEGACY
		KeyboardLayoutRef currentKeyboardLayout;
		if (KLGetCurrentKeyboardLayout(&currentKeyboardLayout) == noErr) {
		#else
		TISInputSourceRef currentKeyboardLayout = TISCopyCurrentKeyboardLayoutInputSource();
		if (currentKeyboardLayout != NULL && CFGetTypeID(currentKeyboardLayout) == TISInputSourceGetTypeID()) {
		#endif
			#ifdef CARBON_LEGACY
			if (KLGetKeyboardLayoutProperty(currentKeyboardLayout, kKLuchrData, (const void **) &inputData) != noErr) {
				inputData = NULL;
			}
			#else
			CFDataRef data = (CFDataRef) TISGetInputSourceProperty(currentKeyboardLayout, kTISPropertyUnicodeKeyLayoutData);
			if (data != NULL && CFGetTypeID(data) == CFDataGetTypeID() && CFDataGetLength(data) > 0) {
				inputData = (CFDataRef) CFRetain(data);
			}
			#endif
		}

		#ifndef CARBON_LEGACY
		if (currentKeyboardLayout != NULL) {
			CFRelease(currentKeyboardLayout);
		}
		#endif
	}
}

void UnloadInputHelper() {
	#ifdef CARBON_LEGACY
	inputData = NULL;
	#else
	if (inputData != NULL) {
		CFRelease(inputData);
	}
	#endif
}
