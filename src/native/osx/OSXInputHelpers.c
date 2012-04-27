/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

#include <stdbool.h>

//Keyboard Upper 16 / Mouse Lower 16
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

static jint DoModifierConvert(CGEventFlags event_mask) {
	jint modifiers = 0;

	//Apply all our modifiers to the java modifiers return.
	if (event_mask & kCGEventFlagMaskShift)				modifiers |= NativeToJModifier(kCGEventFlagMaskShift);
	if (event_mask & kCGEventFlagMaskControl)			modifiers |= NativeToJModifier(kCGEventFlagMaskControl);
	if (event_mask & kCGEventFlagMaskAlternate)			modifiers |= NativeToJModifier(kCGEventFlagMaskAlternate);
	if (event_mask & kCGEventFlagMaskCommand)			modifiers |= NativeToJModifier(kCGEventFlagMaskCommand);

	if (IsModifierMask(kCGEventFlagMaskButtonLeft))		modifiers |= NativeToJModifier(kCGEventFlagMaskButtonLeft);
	if (IsModifierMask(kCGEventFlagMaskButtonRight))	modifiers |= NativeToJModifier(kCGEventFlagMaskButtonRight);
	if (IsModifierMask(kCGEventFlagMaskButtonCenter))	modifiers |= NativeToJModifier(kCGEventFlagMaskButtonCenter);
	if (IsModifierMask(kCGEventFlagMaskXButton1))		modifiers |= NativeToJModifier(kCGEventFlagMaskXButton1);
	if (IsModifierMask(kCGEventFlagMaskXButton2))		modifiers |= NativeToJModifier(kCGEventFlagMaskXButton2);

	return modifiers;
}

CFStringRef KeyCodeToString(CGKeyCode keycode) {
	CFStringRef keytxt = NULL;

	TISInputSourceRef keyboard_ref = TISCopyCurrentKeyboardLayoutInputSource();
	if (keyboard_ref) {
		CFDataRef data_ref = (CFDataRef) TISGetInputSourceProperty(keyboard_ref, kTISPropertyUnicodeKeyLayoutData);

		if (data_ref) {
			const UCKeyboardLayout * keyboard_layout = (const UCKeyboardLayout *) CFDataGetBytePtr(data_ref);

			if (keyboard_layout) {
				//FIXME Shouldn't this be defined somewhere? 
				static const UInt32 numLock = 1 << 16;

				UInt32 modifiers = 0;
				if (IsModifierMask(kCGEventFlagMaskAlphaShift))		modifiers |= alphaLock;
				if (IsModifierMask(kCGEventFlagMaskShift))			modifiers |= shiftKey;
				if (IsModifierMask(kCGEventFlagMaskControl))		modifiers |= controlKey;
				if (IsModifierMask(kCGEventFlagMaskAlternate))		modifiers |= optionKey;
				if (IsModifierMask(kCGEventFlagMaskCommand))		modifiers |= cmdKey;
				if (IsModifierMask(kCGEventFlagMaskNumericPad))		modifiers |= numLock;
				
				static const UInt32 s_commandModifiers = cmdKey | controlKey | rightControlKey;
				bool isCommand = ((modifiers & s_commandModifiers) != 0);
				modifiers &= ~s_commandModifiers;
				if (isCommand) {
					modifiers &= ~optionKey;
				}

				const UniCharCount buff_size = 4;
				UniChar buffer[buff_size];
				UniCharCount buff_len = 0;
				UInt32 deadkey_state = 0;
				OSStatus status = UCKeyTranslate(
										keyboard_layout,
										keycode,
										kUCKeyActionDown,
										(modifiers >> 8) & 0xffu,
										LMGetKbdType(),
										kUCKeyTranslateNoDeadKeysBit,
										&deadkey_state,
										buff_size,
										&buff_len,
										buffer);

				if (buff_len == 0 && deadkey_state) {
					//Convert for Dead Key with a space after.
					status = UCKeyTranslate(
										keyboard_layout,
										kVK_Space,
										kUCKeyActionDown,
										(modifiers >> 8) & 0xffu,
										LMGetKbdType(),
										kUCKeyTranslateNoDeadKeysBit,
										&deadkey_state,
										buff_size,
										&buff_len,
										buffer);
				}

				if (status == noErr && buff_len > 0) {
					//Figure out when buffer > 1
					//keytxt = CFStringCreateWithCharacters(kCFAllocatorDefault, buffer, 1);
					keytxt = CFStringCreateWithCharacters(kCFAllocatorDefault, buffer, buff_len);
				}
			}

			CFRelease(data_ref);
		}

		CFRelease(keyboard_ref);
	}

	return keytxt;
}
