/* Copyright (c) 2006-2010 - Alexander Barker (alex@1stleg.com)
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "OSXKeyCodes.h"

/*
unsigned short modifiers = 0x0000;

unsigned short getModifierMask() {
	return modifiers;
}

void setModifierMask(unsigned short mod) {
	modifiers |= mod;
}

void unsetModifierMask(unsigned short mod) {
	modifiers ^= mod;
}

bool isModifierMask(unsigned short mod) {
	return modifiers & mod;
}
*/

/*
 * Translate the virtual keycode to a character representation.  This
 * function does not handle the modifier keys.  It also translates
 * certain keys to more human readable forms (e.g., "tab", "space",
 * "up").  Because no modifiers are considered a "shift-n" will return
 * "n", not "N".

OSStatus translate_keycode(CGKeyCode keycode, CGEventSourceRef source, CFMutableStringRef result) {
    TISInputSourceRef       currentKeyboard;
    CFDataRef               uchr                = NULL;
    UInt16                  keyAction           = kUCKeyActionDisplay;
    UInt32                  modifierKeyState    = 0;
    OptionBits              keyTranslateOptions = kUCKeyTranslateNoDeadKeysBit;
    UInt32                  deadKeyState        = 0;
    UInt32                  keyboardType;
    UniCharCount            actualStringLength  = 0;
    UniChar                 unicodeString[8];
    const UCKeyboardLayout *keyboardLayout;

    currentKeyboard = TISCopyCurrentKeyboardInputSource();
    uchr = (CFDataRef) TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
    keyboardLayout = (const UCKeyboardLayout *)CFDataGetBytePtr(uchr);
    keyboardType = CGEventSourceGetKeyboardType(source);

    if (keyboardLayout == NULL)
        printf("no keyboard layout");

    OSStatus status = UCKeyTranslate(keyboardLayout,
                                     keycode,
                                     keyAction,
                                     modifierKeyState,
                                     keyboardType,
                                     keyTranslateOptions,
                                     &deadKeyState,
                                     ARRAY_SIZE(unicodeString),
                                     &actualStringLength,
                                     unicodeString);

    if (status != noErr)
        return status;

    if (actualStringLength < 1)
        return noErr;

    for (size_t i = 0; i < ARRAY_SIZE(name2glyph_map); i++) {
        if (name2glyph_map[i].glyph == unicodeString[0]) {
            CFStringAppendCString(result, name2glyph_map[i].name, kCFStringEncodingASCII);
            return noErr;
        }
    }

    CFStringAppendCharacters(result, unicodeString, actualStringLength);
    return noErr;
}
 */
