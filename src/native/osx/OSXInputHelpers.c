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
#ifdef COREFOUNDATION
#include <CoreFoundation/CoreFoundation.h>
#endif

// Keyboard Upper 16 / Mouse Lower 16
static CGEventFlags currModifierMask = 0x00000000;

// Current dead key state.
#if defined(CARBON_LEGACY) || defined(COREFOUNDATION)
static UInt32 currDeadkeyState = 0;
#endif

// Input source data for the keyboard.
#if defined(CARBON_LEGACY)
static void *inputData = NULL;
#elif defined(COREFOUNDATION)
static CFDataRef inputData = NULL;
#endif

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
	#if defined(CARBON_LEGACY) || defined(COREFOUNDATION)
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

			// Disable all command modifiers for translation.  This is required
			// so UCKeyTranslate will provide a keysym for the separate event.
			static const CGEventFlags cmdModifiers = kCGEventFlagMaskCommand | kCGEventFlagMaskControl | kCGEventFlagMaskAlternate;
			modifiers &= ~cmdModifiers;

			// I don't know why but UCKeyTranslate does not process the
			// kCGEventFlagMaskAlphaShift (A.K.A. Caps Lock Mask) correctly.
			// We need to basically turn off the mask and process the capital
			// letters after UCKeyTranslate().  Think Different, not because it
			// makes sense but because you want to be a hipster.
			bool isCapsLock = modifiers & kCGEventFlagMaskAlphaShift;
			modifiers &= ~kCGEventFlagMaskAlphaShift;


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

			if (status == noErr && *length > 0) {
				if (isCapsLock) {
					// We *had* a caps lock mask so we need to convert to uppercase.
					CFMutableStringRef keytxt = CFStringCreateMutableWithExternalCharactersNoCopy(kCFAllocatorDefault, buffer, *length, size, kCFAllocatorNull);
					if (keytxt != NULL) {
						CFLocaleRef locale = CFLocaleCopyCurrent();
						CFStringUppercase(keytxt, locale);
						CFRelease(locale);
						CFRelease(keytxt);
					}
					else {
						// There was an problem creating the CFMutableStringRef.
						*length = 0;
					}
				}
			}
			else {
				// Make sure the buffer length is zero if an error occurred.
				*length = 0;
			}
		}
	}
	#endif

	// Fallback to CGEventKeyboardGetUnicodeString if we were unable to use UCKeyTranslate().
	#if defined(CARBON_LEGACY) || defined(COREFOUNDATION)
	if (*length == 0) {
		CGEventKeyboardGetUnicodeString(event, size, length, buffer);
	}
	#else
	CGEventKeyboardGetUnicodeString(event, size, length, buffer);
	#endif
}

void LoadInputHelper() {
	#if defined(CARBON_LEGACY) || defined(COREFOUNDATION)
	if (inputData == NULL) {
		// Reinitialize dead key state each time we need to load the input helper.
		currDeadkeyState = 0;

		#if defined(CARBON_LEGACY)
		KeyboardLayoutRef currentKeyboardLayout;
		if (KLGetCurrentKeyboardLayout(&currentKeyboardLayout) == noErr) {
			if (KLGetKeyboardLayoutProperty(currentKeyboardLayout, kKLuchrData, (const void **) &inputData) != noErr) {
				inputData = NULL;
			}
		}
		#elif defined(COREFOUNDATION)
		TISInputSourceRef currentKeyboardLayout = TISCopyCurrentKeyboardLayoutInputSource();
		if (currentKeyboardLayout != NULL && CFGetTypeID(currentKeyboardLayout) == TISInputSourceGetTypeID()) {
			CFDataRef data = (CFDataRef) TISGetInputSourceProperty(currentKeyboardLayout, kTISPropertyUnicodeKeyLayoutData);
			if (data != NULL && CFGetTypeID(data) == CFDataGetTypeID() && CFDataGetLength(data) > 0) {
				inputData = (CFDataRef) CFRetain(data);
			}
		}

		if (currentKeyboardLayout != NULL) {
			CFRelease(currentKeyboardLayout);
		}
		#endif
	}
	#endif
}

void UnloadInputHelper() {
	#if defined(CARBON_LEGACY)
	inputData = NULL;
	#elif defined(COREFOUNDATION)
	if (inputData != NULL) {
		CFRelease(inputData);
	}
	#endif
}
