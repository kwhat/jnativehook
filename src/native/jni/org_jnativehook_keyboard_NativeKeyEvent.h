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

/* Redefined virtual constants for native use.  This file should be machine
 * generated however no tool exists for doing so.
 * Reference: org/jnativehook/keyboard/NativeKeyEvent.java
 */

#ifndef _Included_org_jnativehook_keyboard_NativeKeyEvent
#define _Included_org_jnativehook_keyboard_NativeKeyEvent

#define org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST				2400
#define org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_LAST					2402
#define org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED				org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST
#define org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED				1 + org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST
#define org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED				2 + org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST


#define org_jnativehook_keyboard_NativeKeyEvent_LOCATION_UNKNOWN				0
#define org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD				1
#define org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT					2
#define org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT					3
#define org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD					4


#define org_jnativehook_keyboard_NativeKeyEvent_VK_ENTER						'\n'
#define org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_SPACE					'\b'
#define org_jnativehook_keyboard_NativeKeyEvent_VK_TAB							'\t'
#define org_jnativehook_keyboard_NativeKeyEvent_VK_CANCEL						0x0C

#define org_jnativehook_keyboard_NativeKeyEvent_VK_SHIFT						0x10
#define org_jnativehook_keyboard_NativeKeyEvent_VK_CONTROL						0x11
#define org_jnativehook_keyboard_NativeKeyEvent_VK_ALT							0x12	// Option or Alt Key
#define org_jnativehook_keyboard_NativeKeyEvent_VK_META							0x9D	// OSX: Command
#define org_jnativehook_keyboard_NativeKeyEvent_VK_WINDOWS						0x020C
#define org_jnativehook_keyboard_NativeKeyEvent_VK_CONTEXT_MENU					0x020D

#define org_jnativehook_keyboard_NativeKeyEvent_VK_PAUSE						0x13
#define org_jnativehook_keyboard_NativeKeyEvent_VK_CAPS_LOCK					0x14
#define org_jnativehook_keyboard_NativeKeyEvent_VK_ESCAPE						0x1B
#define org_jnativehook_keyboard_NativeKeyEvent_VK_SPACE						0x20

#define org_jnativehook_keyboard_NativeKeyEvent_VK_UP							0x26
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DOWN							0x28
#define org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT							0x25
#define org_jnativehook_keyboard_NativeKeyEvent_VK_RIGHT						0x27

#define org_jnativehook_keyboard_NativeKeyEvent_VK_COMMA						0x2C	// ','
#define org_jnativehook_keyboard_NativeKeyEvent_VK_MINUS						0x2D	// '-'
#define org_jnativehook_keyboard_NativeKeyEvent_VK_PERIOD						0x2E	// '.'
#define org_jnativehook_keyboard_NativeKeyEvent_VK_SLASH						0x2F	// '/'

#define org_jnativehook_keyboard_NativeKeyEvent_VK_0							0x30
#define org_jnativehook_keyboard_NativeKeyEvent_VK_1							0x31
#define org_jnativehook_keyboard_NativeKeyEvent_VK_2							0x32
#define org_jnativehook_keyboard_NativeKeyEvent_VK_3							0x33
#define org_jnativehook_keyboard_NativeKeyEvent_VK_4							0x34
#define org_jnativehook_keyboard_NativeKeyEvent_VK_5							0x35
#define org_jnativehook_keyboard_NativeKeyEvent_VK_6							0x36
#define org_jnativehook_keyboard_NativeKeyEvent_VK_7							0x37
#define org_jnativehook_keyboard_NativeKeyEvent_VK_8							0x38
#define org_jnativehook_keyboard_NativeKeyEvent_VK_9							0x39

#define org_jnativehook_keyboard_NativeKeyEvent_VK_SEMICOLON					0x3B	// ';'
#define org_jnativehook_keyboard_NativeKeyEvent_VK_EQUALS						0x3D	// '='

#define org_jnativehook_keyboard_NativeKeyEvent_VK_A							0x41
#define org_jnativehook_keyboard_NativeKeyEvent_VK_B							0x42
#define org_jnativehook_keyboard_NativeKeyEvent_VK_C							0x43
#define org_jnativehook_keyboard_NativeKeyEvent_VK_D							0x44
#define org_jnativehook_keyboard_NativeKeyEvent_VK_E							0x45
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F							0x46
#define org_jnativehook_keyboard_NativeKeyEvent_VK_G							0x47
#define org_jnativehook_keyboard_NativeKeyEvent_VK_H							0x48
#define org_jnativehook_keyboard_NativeKeyEvent_VK_I							0x49
#define org_jnativehook_keyboard_NativeKeyEvent_VK_J							0x4A
#define org_jnativehook_keyboard_NativeKeyEvent_VK_K							0x4B
#define org_jnativehook_keyboard_NativeKeyEvent_VK_L							0x4C
#define org_jnativehook_keyboard_NativeKeyEvent_VK_M							0x4D
#define org_jnativehook_keyboard_NativeKeyEvent_VK_N							0x4E
#define org_jnativehook_keyboard_NativeKeyEvent_VK_O							0x4F
#define org_jnativehook_keyboard_NativeKeyEvent_VK_P							0x50
#define org_jnativehook_keyboard_NativeKeyEvent_VK_Q							0x51
#define org_jnativehook_keyboard_NativeKeyEvent_VK_R							0x52
#define org_jnativehook_keyboard_NativeKeyEvent_VK_S							0x53
#define org_jnativehook_keyboard_NativeKeyEvent_VK_T							0x54
#define org_jnativehook_keyboard_NativeKeyEvent_VK_U							0x55
#define org_jnativehook_keyboard_NativeKeyEvent_VK_V							0x56
#define org_jnativehook_keyboard_NativeKeyEvent_VK_W							0x57
#define org_jnativehook_keyboard_NativeKeyEvent_VK_X							0x58
#define org_jnativehook_keyboard_NativeKeyEvent_VK_Y							0x59
#define org_jnativehook_keyboard_NativeKeyEvent_VK_Z							0x5A

#define org_jnativehook_keyboard_NativeKeyEvent_VK_OPEN_BRACKET					0x5B	// '['
#define org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_SLASH					0x5C	// '\'
#define org_jnativehook_keyboard_NativeKeyEvent_VK_CLOSE_BRACKET				0x5D	// ']'

#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD0						0x60
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD1						0x61
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD2						0x62
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD3						0x63
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD4						0x64
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD5						0x65
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD6						0x66
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD7						0x67
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD8						0x68
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD9						0x69

#define org_jnativehook_keyboard_NativeKeyEvent_VK_KP_UP						0xE0
#define org_jnativehook_keyboard_NativeKeyEvent_VK_KP_DOWN						0xE1
#define org_jnativehook_keyboard_NativeKeyEvent_VK_KP_LEFT						0xE2
#define org_jnativehook_keyboard_NativeKeyEvent_VK_KP_RIGHT						0xE3

#define org_jnativehook_keyboard_NativeKeyEvent_VK_MULTIPLY						0x6A
#define org_jnativehook_keyboard_NativeKeyEvent_VK_ADD							0x6B
#define org_jnativehook_keyboard_NativeKeyEvent_VK_SEPARATOR					0x6C
#define org_jnativehook_keyboard_NativeKeyEvent_VK_SUBTRACT						0x6D
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DECIMAL						0x6E
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DIVIDE						0x6F
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DELETE						0x7F
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUM_LOCK						0x90
#define org_jnativehook_keyboard_NativeKeyEvent_VK_CLEAR						0x03	// OSX: Number Lock
#define org_jnativehook_keyboard_NativeKeyEvent_VK_SCROLL_LOCK					0x91

#define org_jnativehook_keyboard_NativeKeyEvent_VK_F1							0x70
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F2							0x71
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F3							0x72
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F4							0x73
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F5							0x74
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F6							0x75
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F7							0x76
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F8							0x77
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F9							0x78
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F10							0x79
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F11							0x7A
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F12							0x7B

#define org_jnativehook_keyboard_NativeKeyEvent_VK_F13							0xF000
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F14							0xF001
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F15							0xF002
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F16							0xF003
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F17							0xF004
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F18							0xF005
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F19							0xF006
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F20							0xF007
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F21							0xF008
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F22							0xF009
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F23							0xF00A
#define org_jnativehook_keyboard_NativeKeyEvent_VK_F24							0xF00B


#define org_jnativehook_keyboard_NativeKeyEvent_VK_PRINTSCREEN					0x9A
#define org_jnativehook_keyboard_NativeKeyEvent_VK_INSERT						0x9B
#define org_jnativehook_keyboard_NativeKeyEvent_VK_HELP							0x9C

#define org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_UP						0x21
#define org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_DOWN					0x22
#define org_jnativehook_keyboard_NativeKeyEvent_VK_HOME							0x24
#define org_jnativehook_keyboard_NativeKeyEvent_VK_END							0x23

#define org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_QUOTE					0xC0
#define org_jnativehook_keyboard_NativeKeyEvent_VK_QUOTE						0xDE


// For European keyboards.
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_GRAVE					0x80
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ACUTE					0x81
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CIRCUMFLEX				0x82
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_TILDE					0x83
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_MACRON					0x84
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_BREVE					0x85
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ABOVEDOT				0x86
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_DIAERESIS				0x87
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ABOVERING				0x88
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_DOUBLEACUTE				0x89
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CARON					0x8A
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CEDILLA					0x8B
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_OGONEK					0x8C
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_IOTA					0x8D
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_VOICED_SOUND			0x8E
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_SEMIVOICED_SOUND		0x8F

// Unknown Keyboard Codes.
#define org_jnativehook_keyboard_NativeKeyEvent_VK_AMPERSAND					0x96
#define org_jnativehook_keyboard_NativeKeyEvent_VK_ASTERISK						0x97
#define org_jnativehook_keyboard_NativeKeyEvent_VK_QUOTEDBL						0x98
#define org_jnativehook_keyboard_NativeKeyEvent_VK_LESS							0x99
#define org_jnativehook_keyboard_NativeKeyEvent_VK_GREATER						0xA0
#define org_jnativehook_keyboard_NativeKeyEvent_VK_BRACELEFT					0xA1
#define org_jnativehook_keyboard_NativeKeyEvent_VK_BRACERIGHT					0xA2


// Unknown Extended Keyboard Codes.
#define org_jnativehook_keyboard_NativeKeyEvent_VK_AT							0x0200
#define org_jnativehook_keyboard_NativeKeyEvent_VK_COLON						0x0201
#define org_jnativehook_keyboard_NativeKeyEvent_VK_CIRCUMFLEX					0x0202
#define org_jnativehook_keyboard_NativeKeyEvent_VK_DOLLAR						0x0203
#define org_jnativehook_keyboard_NativeKeyEvent_VK_EURO_SIGN					0x0204
#define org_jnativehook_keyboard_NativeKeyEvent_VK_EXCLAMATION_MARK				0x0205
#define org_jnativehook_keyboard_NativeKeyEvent_VK_INVERTED_EXCLAMATION_MARK	0x0206
#define org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT_PARENTHESIS				0x0207
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NUMBER_SIGN					0x0208
#define org_jnativehook_keyboard_NativeKeyEvent_VK_PLUS							0x0209
#define org_jnativehook_keyboard_NativeKeyEvent_VK_RIGHT_PARENTHESIS			0x020A
#define org_jnativehook_keyboard_NativeKeyEvent_VK_UNDERSCORE					0x020B


// For input method support on Asian Keyboards.
#define org_jnativehook_keyboard_NativeKeyEvent_VK_FINAL						0x0018	// Unknown Win32 API
#define org_jnativehook_keyboard_NativeKeyEvent_VK_CONVERT						0x001C
#define org_jnativehook_keyboard_NativeKeyEvent_VK_NONCONVERT					0x001D
#define org_jnativehook_keyboard_NativeKeyEvent_VK_ACCEPT						0x001E
#define org_jnativehook_keyboard_NativeKeyEvent_VK_MODECHANGE					0x001F	// Unknown Win32 API
#define org_jnativehook_keyboard_NativeKeyEvent_VK_KANA							0x0015
#define org_jnativehook_keyboard_NativeKeyEvent_VK_KANJI						0x0019
#define org_jnativehook_keyboard_NativeKeyEvent_VK_ALPHANUMERIC					0x00F0
#define org_jnativehook_keyboard_NativeKeyEvent_VK_KATAKANA						0x00F1
#define org_jnativehook_keyboard_NativeKeyEvent_VK_HIRAGANA						0x00F2
#define org_jnativehook_keyboard_NativeKeyEvent_VK_FULL_WIDTH					0x00F3
#define org_jnativehook_keyboard_NativeKeyEvent_VK_HALF_WIDTH					0x00F4
#define org_jnativehook_keyboard_NativeKeyEvent_VK_ROMAN_CHARACTERS				0x00F5
#define org_jnativehook_keyboard_NativeKeyEvent_VK_ALL_CANDIDATES				0x0100
#define org_jnativehook_keyboard_NativeKeyEvent_VK_PREVIOUS_CANDIDATE			0x0101
#define org_jnativehook_keyboard_NativeKeyEvent_VK_CODE_INPUT					0x0102
#define org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_KATAKANA			0x0103
#define org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_HIRAGANA			0x0104
#define org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_ROMAN				0x0105
#define org_jnativehook_keyboard_NativeKeyEvent_VK_KANA_LOCK					0x0106
#define org_jnativehook_keyboard_NativeKeyEvent_VK_INPUT_METHOD_ON_OFF			0x0107

// For Sun keyboards.
#define org_jnativehook_keyboard_NativeKeyEvent_VK_CUT							0xFFD1
#define org_jnativehook_keyboard_NativeKeyEvent_VK_COPY							0xFFCD
#define org_jnativehook_keyboard_NativeKeyEvent_VK_PASTE						0xFFCF
#define org_jnativehook_keyboard_NativeKeyEvent_VK_UNDO							0xFFCB
#define org_jnativehook_keyboard_NativeKeyEvent_VK_AGAIN						0xFFC9
#define org_jnativehook_keyboard_NativeKeyEvent_VK_FIND							0xFFD0
#define org_jnativehook_keyboard_NativeKeyEvent_VK_PROPS						0xFFCA
#define org_jnativehook_keyboard_NativeKeyEvent_VK_STOP							0xFFC8
#define org_jnativehook_keyboard_NativeKeyEvent_VK_COMPOSE						0xFF20
#define org_jnativehook_keyboard_NativeKeyEvent_VK_ALT_GRAPH					0xFF7E

#define org_jnativehook_keyboard_NativeKeyEvent_VK_BEGIN						0xFF58

#define org_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED					0xFFFF	// CharCode Unknown

#define org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED					0x0000	// KeyCode Unknown

#endif
