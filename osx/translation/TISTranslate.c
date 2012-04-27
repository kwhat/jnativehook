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
#include <stdio.h>

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h> //For HIToolbox

CFStringRef KeyCodeToKeySym(CGKeyCode keycode, CGEventFlags event_mask) {
	/*
	#if defined(MAC_OS_X_VERSION_10_5)
	TISInputSourceRef currentKeyboardLayout = TISCopyCurrentKeyboardLayoutInputSource(); 
	#else
	KeyboardLayoutRef currentKeyboardLayout;
	OSStatus status = KLGetCurrentKeyboardLayout(&currentKeyboardLayout);
	#endif
	*/
	/*
	 #if defined(MAC_OS_X_VERSION_10_5)
	CFDataRef ref = (CFDataRef) TISGetInputSourceProperty(currentKeyboardLayout, kTISPropertyUnicodeKeyLayoutData);
	const UCKeyboardLayout* layout = (const UCKeyboardLayout*) CFDataGetBytePtr(ref);
	const bool layoutValid = (layout != NULL);
	#else
	const void* resource;
	int err = KLGetKeyboardLayoutProperty(currentKeyboardLayout, kKLuchrData, &resource);
	const bool layoutValid = (err == noErr);
	const UCKeyboardLayout* layout = (const UCKeyboardLayout*)resource;
	#endif
	 */
	//UniChar keysym = kKeyNone;
	CFStringRef keysym = NULL;
	
	TISInputSourceRef keyboard_ref = TISCopyCurrentKeyboardLayoutInputSource();
	if (keyboard_ref) {
		CFDataRef data_ref = (CFDataRef) TISGetInputSourceProperty(keyboard_ref, kTISPropertyUnicodeKeyLayoutData);

		if (data_ref) {
			const UCKeyboardLayout * keyboard_layout = (const UCKeyboardLayout *) CFDataGetBytePtr(data_ref);
			
			if (keyboard_layout) {
				//FIXME Defined in JNativeHook
				
				CGEventFlags modifiers = event_mask;
				printf("Test1: 0x%X\n", (unsigned int) modifiers);
				/*
				static const UInt32 numLock = 1 << 16;
				UInt32 modifiers = 0;
				if ((bool) (event_mask & kCGEventFlagMaskAlphaShift))	modifiers |= alphaLock;
				if ((bool) (event_mask & kCGEventFlagMaskShift))		modifiers |= shiftKey;
				if ((bool) (event_mask & kCGEventFlagMaskControl))		modifiers |= controlKey;
				if ((bool) (event_mask & kCGEventFlagMaskAlternate))	modifiers |= optionKey;
				if ((bool) (event_mask & kCGEventFlagMaskCommand))		modifiers |= cmdKey;
				if ((bool) (event_mask & kCGEventFlagMaskNumericPad))	modifiers |= numLock;
				*/
				/* FIXME Defined in JNativeHook
				if (isModifierMask(kCGEventFlagMaskAlphaShift))		modifiers |= alphaLock;
				if (isModifierMask(kCGEventFlagMaskShift))			modifiers |= shiftKey;
				if (isModifierMask(kCGEventFlagMaskControl))		modifiers |= controlKey;
				if (isModifierMask(kCGEventFlagMaskAlternate))		modifiers |= optionKey;
				if (isModifierMask(kCGEventFlagMaskCommand))		modifiers |= cmdKey;
				if (isModifierMask(kCGEventFlagMaskNumericPad))		modifiers |= numLock;
				*/
				static const UInt32 s_commandModifiers = cmdKey | controlKey | rightControlKey;
				bool isCommand = ((modifiers & s_commandModifiers) != 0);
				modifiers &= ~s_commandModifiers;
				if (isCommand) {
					modifiers &= ~optionKey;
				}
				printf("Test2: 0x%X\n", (unsigned int) (modifiers >> 16) & 0xFF);
				
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
					//Convert for Dead Key with a space after.
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
				
				if (status == noErr && buff_len == 1) {
					//TODO Figure out when buff_len > 1
					
					//keysym = buffer[0];
					
					//Figure out when buffer > 1
					//keysym = CFStringCreateWithCharacters(kCFAllocatorDefault, buffer, 1);
					keysym = CFStringCreateWithCharacters(kCFAllocatorDefault, buffer, buff_len);
				}
				else {
					printf("Error!\n");
				}
			}
			
			//CFRelease(data_ref);
		}

		CFRelease(keyboard_ref);
	}
	
	return keysym;
}


int main(int argc, const char * argv[]) {

	UInt16 keycode = 11; // b
	UInt32 event_mask = kCGEventFlagMaskShift; //kCGEventFlagMaskAlphaShift | kCGEventFlagMaskShift | kCGEventFlagMaskControl | kCGEventFlagMaskAlternate
	CFStringRef keysym = KeyCodeToKeySym(keycode, event_mask);

	printf("KeySym: %s\n", CFStringGetCStringPtr(keysym, CFStringGetSystemEncoding()));

	return EXIT_SUCCESS;
}
