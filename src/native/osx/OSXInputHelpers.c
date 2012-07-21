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

void SetModifierMask(CGEventFlags mask) {
	current_modifiers |= mask;
}

void UnsetModifierMask(CGEventFlags mask) {
	current_modifiers ^= mask;
}

CGEventFlags GetModifiers() {
	return current_modifiers;
}

CFStringRef KeyCodeToString(CGKeyCode keycode, CGEventFlags modifiers) {
	CFStringRef keytxt = CFSTR("");

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
		if (data_ref) {
			keyboard_layout = (const UCKeyboardLayout*) CFDataGetBytePtr(data_ref);
		}
		
		if (keyboard_layout) {
		#endif
			static const CGEventFlags cmd_modifiers = kCGEventFlagMaskCommand | kCGEventFlagMaskControl | kCGEventFlagMaskAlternate;
			bool isCommand = ((modifiers & cmd_modifiers) != 0);
			modifiers &= ~cmd_modifiers;
			if (isCommand) {
				modifiers &= ~kCGEventFlagMaskAlternate;
			}

			const UniCharCount buff_size = 4;
			UniChar buffer[buff_size];
			UniCharCount buff_len = 0;
			UInt32 deadkey_state = 0;

			OSStatus status = UCKeyTranslate(
									keyboard_layout,
									keycode,
									kUCKeyActionDown,
									(modifiers >> 16) & 0xFF,
									LMGetKbdType(),
									kUCKeyTranslateNoDeadKeysBit,
									&deadkey_state,
									buff_size,
									&buff_len,
									buffer);

			if (buff_len == 0 && deadkey_state) {
				// Convert for Dead Key with a space after
				status = UCKeyTranslate(
									keyboard_layout,
									kVK_Space,
									kUCKeyActionDown,
									(modifiers >> 16) & 0xFF,
									LMGetKbdType(),
									kUCKeyTranslateNoDeadKeysBit,
									&deadkey_state,
									buff_size,
									&buff_len,
									buffer);
			}


			if (status == noErr && buff_len > 0) {
				// Figure out when buffer > 1
				// keytxt = CFStringCreateWithCharacters(kCFAllocatorDefault, buffer, 1);
				keytxt = CFStringCreateWithCharacters(kCFAllocatorDefault, buffer, buff_len);
			}
		}


		CFRelease(keyboard_ref);
	}

	return keytxt;
}
