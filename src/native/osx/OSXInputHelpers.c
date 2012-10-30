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
static CGEventFlags current_modifiers = 0x00000000;
static UInt32 deadkey_state = 0;

void SetModifierMask(CGEventFlags mask) {
	current_modifiers |= mask;
}

void UnsetModifierMask(CGEventFlags mask) {
	current_modifiers ^= mask;
}

CGEventFlags GetModifiers() {
	return current_modifiers;
}

//#define CFObjectIsType(object, type) ((NULL!=object) && type ## GetTypeID()==CFGetTypeID(object))

void KeyCodeToString(CGEventRef event, UniCharCount size, UniCharCount *length, UniChar *buffer) {
	#ifdef CARBON_LEGACY
	KeyboardLayoutRef currentKeyboardLayout;
	if (KLGetCurrentKeyboardLayout(&currentKeyboardLayout) == noErr) {
	#else
	TISInputSourceRef keyboard_ref = TISCopyCurrentKeyboardLayoutInputSource();
	if (keyboard_ref) {
	#endif
		#ifdef CARBON_LEGACY
		const void *resource;
		if (KLGetKeyboardLayoutProperty(currentKeyboardLayout, kKLuchrData, &resource) == noErr) {
			const UCKeyboardLayout *keyboard_layout = (const UCKeyboardLayout *) resource;
		#else
		CFDataRef data_ref = (CFDataRef) TISGetInputSourceProperty(keyboard_ref, kTISPropertyUnicodeKeyLayoutData);
		const UCKeyboardLayout *keyboard_layout = NULL;
		if (data_ref && CFDataGetLength(data_ref) > 0) {
			keyboard_layout = (const UCKeyboardLayout*) CFDataGetBytePtr(data_ref);
		}

		if (keyboard_layout) {
		#endif
			CGKeyCode keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			CGEventFlags modifiers = CGEventGetFlags(event);

			// Disable all command modifiers for translation.
			/*
			static const CGEventFlags cmd_modifiers = kCGEventFlagMaskCommand | kCGEventFlagMaskControl | kCGEventFlagMaskAlternate;
			bool isCommand = ((modifiers & cmd_modifiers) != 0);
			modifiers &= ~cmd_modifiers;
			if (isCommand) {
				modifiers &= ~kCGEventFlagMaskAlternate;
			}
			*/

			printf("Modifiers: 0x%X\n", (unsigned int) (modifiers >> 16) & 0xFF);
			printf("Deadkey: 0x%X\n\n", (unsigned int) deadkey_state);

			OSStatus status = noErr;
			if (deadkey_state == 0) {
				// No previous deadkey, attempt a lookup.
				status = UCKeyTranslate(
									keyboard_layout,
									keycode,
									kUCKeyActionDown,
									0x00, //(modifiers >> 16) & 0xFF, || (modifiers >> 8) & 0xFF,
									LMGetKbdType(),
									kUCKeyTranslateNoDeadKeysBit, //kNilOptions, //kUCKeyTranslateNoDeadKeysMask
									&deadkey_state,
									size,
									length,
									buffer);
			}
			else {
				// The previous key was a deadkey, lookup what it should be.
				status = UCKeyTranslate(
									keyboard_layout,
									keycode,
									kUCKeyActionDown,
									0x00, //No Modifier
									LMGetKbdType(),
									kNilOptions, //kNilOptions, //kUCKeyTranslateNoDeadKeysMask
									&deadkey_state,
									size,
									length,
									buffer);

				// TODO Determine if or when we should reset the state. 
				//deadkey_state = 0;
			}

			if (status != noErr) {
				// Make sure the buffer length is zero.
				*length = 0;
			}
		}

		CFRelease(keyboard_ref);
	}

	// Fallback to CGEventKeyboardGetUnicodeString
	if (*length == 0) {
		CGEventKeyboardGetUnicodeString(event, size, length, buffer);
	}
}
