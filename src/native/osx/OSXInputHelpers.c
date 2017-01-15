/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
 * https://github.com/kwhat/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
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
static KeyboardLayoutRef previousKeyboardLayout = NULL;
#elif defined(COREFOUNDATION)
static TISInputSourceRef previousKeyboardLayout = NULL;
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

// This method must be executed from the main runloop to avoid the seemingly random
// Exception detected while handling key input.  TSMProcessRawKeyCode failed (-192) errors.
// CFEqual(CFRunLoopGetCurrent(), CFRunLoopGetMain())
void KeyCodeToString(CGEventRef event, UniCharCount size, UniCharCount *length, UniChar *buffer) {
	#if defined(CARBON_LEGACY) || defined(COREFOUNDATION)
	#if defined(CARBON_LEGACY)
	KeyboardLayoutRef currentKeyboardLayout;
	void *inputData = NULL;
	if (KLGetCurrentKeyboardLayout(&currentKeyboardLayout) == noErr) {
		if (KLGetKeyboardLayoutProperty(currentKeyboardLayout, kKLuchrData, (const void **) &inputData) != noErr) {
			inputData = NULL;
		}
	}
	#elif defined(COREFOUNDATION)
	TISInputSourceRef currentKeyboardLayout = TISCopyCurrentKeyboardLayoutInputSource();
	CFDataRef inputData = NULL;
	if (currentKeyboardLayout != NULL && CFGetTypeID(currentKeyboardLayout) == TISInputSourceGetTypeID()) {
		CFDataRef data = (CFDataRef) TISGetInputSourceProperty(currentKeyboardLayout, kTISPropertyUnicodeKeyLayoutData);
		if (data != NULL && CFGetTypeID(data) == CFDataGetTypeID() && CFDataGetLength(data) > 0) {
			inputData = (CFDataRef) data;
		}
	}

	// Check if the keyboard layout has changed to see if the dead key state needs to be discarded.
	if (previousKeyboardLayout != NULL && currentKeyboardLayout != NULL && CFEqual(currentKeyboardLayout, previousKeyboardLayout) == false) {
		currDeadkeyState = 0;
	}

	// Release the previous keyboard layout.
	if (previousKeyboardLayout != NULL) {
		CFRelease(previousKeyboardLayout);
	}

	// Set the previous keyboard layout to the current layout.
	if (currentKeyboardLayout != NULL) {
		previousKeyboardLayout = currentKeyboardLayout;
	}
	#endif

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

	// The following codes should not be processed because they are invalid.
	// FIXME This entire function is ugly, hard to follow and needs to be reworked.
	if (*length == 1) {
		switch (buffer[0]) {
			case 0x01:		// Home
			case 0x04:		// End
			case 0x05:		// Help Key
			case 0x10:		// Function Keys
			case 0x0B:		// Page Up
			case 0x0C:		// Page Down
				*length = 0;
		}
	}
}

void LoadInputHelper() {
	#if defined(CARBON_LEGACY) || defined(COREFOUNDATION)
	// Start with a fresh dead key state.
	currDeadkeyState = 0;
	#endif
}

void UnloadInputHelper() {
	#if defined(CARBON_LEGACY) || defined(COREFOUNDATION)
	if (previousKeyboardLayout != NULL) {
		// Cleanup tracking of the previous layout.
		CFRelease(previousKeyboardLayout);
	}
	#endif
}
