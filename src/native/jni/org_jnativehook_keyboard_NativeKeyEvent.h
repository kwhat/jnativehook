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

//Reference: org/jnativehook/keyboard/NativeKeyEvent.java
//http://java.sun.com/j2se/1.5.0/docs/api/constant-values.html#java.awt.event.KeyEvent.CHAR_UNDEFINED
#ifndef _Included_org_jnativehook_keyboard_NativeKeyEvent
#define _Included_org_jnativehook_keyboard_NativeKeyEvent

#define org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST			2400
#define org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_LAST				2402
#define org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED			org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST
#define org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED			1 + org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST
#define org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED			2 + org_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_FIRST


#define org_jnativehook_keyboard_NativeKeyEvent_LOCATION_UNKNOWN			0
#define org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD			1
#define org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT				2
#define org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT				3
#define org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD				4


#define org_jnativehook_keyboard_NativeKeyEvent_ENTER						'\n'
#define org_jnativehook_keyboard_NativeKeyEvent_BACK_SPACE					'\b'
#define org_jnativehook_keyboard_NativeKeyEvent_TAB							'\t'
#define org_jnativehook_keyboard_NativeKeyEvent_CANCEL						0x0C

#define org_jnativehook_keyboard_NativeKeyEvent_SHIFT						0x10
#define org_jnativehook_keyboard_NativeKeyEvent_CONTROL						0x11
#define org_jnativehook_keyboard_NativeKeyEvent_ALT							0x12	//option or alt key
#define org_jnativehook_keyboard_NativeKeyEvent_META						0x9D	//osx: command
#define org_jnativehook_keyboard_NativeKeyEvent_WINDOWS						0x020C
#define org_jnativehook_keyboard_NativeKeyEvent_CONTEXT_MENU				0x020D

#define org_jnativehook_keyboard_NativeKeyEvent_PAUSE						0x13
#define org_jnativehook_keyboard_NativeKeyEvent_CAPS_LOCK					0x14
#define org_jnativehook_keyboard_NativeKeyEvent_ESCAPE						0x1B
#define org_jnativehook_keyboard_NativeKeyEvent_SPACE						0x20

#define org_jnativehook_keyboard_NativeKeyEvent_UP							0x26
#define org_jnativehook_keyboard_NativeKeyEvent_DOWN						0x28
#define org_jnativehook_keyboard_NativeKeyEvent_LEFT						0x25
#define org_jnativehook_keyboard_NativeKeyEvent_RIGHT						0x27

#define org_jnativehook_keyboard_NativeKeyEvent_COMMA						0x2C	// ','
#define org_jnativehook_keyboard_NativeKeyEvent_MINUS						0x2D	// '-'
#define org_jnativehook_keyboard_NativeKeyEvent_PERIOD						0x2E	// '.'
#define org_jnativehook_keyboard_NativeKeyEvent_SLASH						0x2F	// '/'

#define org_jnativehook_keyboard_NativeKeyEvent_0							0x30
#define org_jnativehook_keyboard_NativeKeyEvent_1							0x31
#define org_jnativehook_keyboard_NativeKeyEvent_2							0x32
#define org_jnativehook_keyboard_NativeKeyEvent_3							0x33
#define org_jnativehook_keyboard_NativeKeyEvent_4							0x34
#define org_jnativehook_keyboard_NativeKeyEvent_5							0x35
#define org_jnativehook_keyboard_NativeKeyEvent_6							0x36
#define org_jnativehook_keyboard_NativeKeyEvent_7							0x37
#define org_jnativehook_keyboard_NativeKeyEvent_8							0x38
#define org_jnativehook_keyboard_NativeKeyEvent_9							0x39

#define org_jnativehook_keyboard_NativeKeyEvent_SEMICOLON					0x3B	// ';'
#define org_jnativehook_keyboard_NativeKeyEvent_EQUALS						0x3D	// '='

#define org_jnativehook_keyboard_NativeKeyEvent_A							0x41
#define org_jnativehook_keyboard_NativeKeyEvent_B							0x42
#define org_jnativehook_keyboard_NativeKeyEvent_C							0x43
#define org_jnativehook_keyboard_NativeKeyEvent_D							0x44
#define org_jnativehook_keyboard_NativeKeyEvent_E							0x45
#define org_jnativehook_keyboard_NativeKeyEvent_F							0x46
#define org_jnativehook_keyboard_NativeKeyEvent_G							0x47
#define org_jnativehook_keyboard_NativeKeyEvent_H							0x48
#define org_jnativehook_keyboard_NativeKeyEvent_I							0x49
#define org_jnativehook_keyboard_NativeKeyEvent_J							0x4A
#define org_jnativehook_keyboard_NativeKeyEvent_K							0x4B
#define org_jnativehook_keyboard_NativeKeyEvent_L							0x4C
#define org_jnativehook_keyboard_NativeKeyEvent_M							0x4D
#define org_jnativehook_keyboard_NativeKeyEvent_N							0x4E
#define org_jnativehook_keyboard_NativeKeyEvent_O							0x4F
#define org_jnativehook_keyboard_NativeKeyEvent_P							0x50
#define org_jnativehook_keyboard_NativeKeyEvent_Q							0x51
#define org_jnativehook_keyboard_NativeKeyEvent_R							0x52
#define org_jnativehook_keyboard_NativeKeyEvent_S							0x53
#define org_jnativehook_keyboard_NativeKeyEvent_T							0x54
#define org_jnativehook_keyboard_NativeKeyEvent_U							0x55
#define org_jnativehook_keyboard_NativeKeyEvent_V							0x56
#define org_jnativehook_keyboard_NativeKeyEvent_W							0x57
#define org_jnativehook_keyboard_NativeKeyEvent_X							0x58
#define org_jnativehook_keyboard_NativeKeyEvent_Y							0x59
#define org_jnativehook_keyboard_NativeKeyEvent_Z							0x5A

#define org_jnativehook_keyboard_NativeKeyEvent_OPEN_BRACKET				0x5B	// '['
#define org_jnativehook_keyboard_NativeKeyEvent_BACK_SLASH					0x5C	// '\'
#define org_jnativehook_keyboard_NativeKeyEvent_CLOSE_BRACKET				0x5D	// ']'

#define org_jnativehook_keyboard_NativeKeyEvent_NUMPAD0						0x60
#define org_jnativehook_keyboard_NativeKeyEvent_NUMPAD1						0x61
#define org_jnativehook_keyboard_NativeKeyEvent_NUMPAD2						0x62
#define org_jnativehook_keyboard_NativeKeyEvent_NUMPAD3						0x63
#define org_jnativehook_keyboard_NativeKeyEvent_NUMPAD4						0x64
#define org_jnativehook_keyboard_NativeKeyEvent_NUMPAD5						0x65
#define org_jnativehook_keyboard_NativeKeyEvent_NUMPAD6						0x66
#define org_jnativehook_keyboard_NativeKeyEvent_NUMPAD7						0x67
#define org_jnativehook_keyboard_NativeKeyEvent_NUMPAD8						0x68
#define org_jnativehook_keyboard_NativeKeyEvent_NUMPAD9						0x69

#define org_jnativehook_keyboard_NativeKeyEvent_KP_UP						0xE0
#define org_jnativehook_keyboard_NativeKeyEvent_KP_DOWN						0xE1
#define org_jnativehook_keyboard_NativeKeyEvent_KP_LEFT						0xE2
#define org_jnativehook_keyboard_NativeKeyEvent_KP_RIGHT					0xE3

#define org_jnativehook_keyboard_NativeKeyEvent_MULTIPLY					0x6A
#define org_jnativehook_keyboard_NativeKeyEvent_ADD							0x6B
#define org_jnativehook_keyboard_NativeKeyEvent_SEPARATOR					0x6C
#define org_jnativehook_keyboard_NativeKeyEvent_SUBTRACT					0x6D
#define org_jnativehook_keyboard_NativeKeyEvent_DECIMAL						0x6E
#define org_jnativehook_keyboard_NativeKeyEvent_DIVIDE						0x6F
#define org_jnativehook_keyboard_NativeKeyEvent_DELETE						0x7F
#define org_jnativehook_keyboard_NativeKeyEvent_NUM_LOCK					0x90
#define org_jnativehook_keyboard_NativeKeyEvent_CLEAR						0x03	//osx: number lock
#define org_jnativehook_keyboard_NativeKeyEvent_SCROLL_LOCK					0x91

#define org_jnativehook_keyboard_NativeKeyEvent_F1							0x70
#define org_jnativehook_keyboard_NativeKeyEvent_F2							0x71
#define org_jnativehook_keyboard_NativeKeyEvent_F3							0x72
#define org_jnativehook_keyboard_NativeKeyEvent_F4							0x73
#define org_jnativehook_keyboard_NativeKeyEvent_F5							0x74
#define org_jnativehook_keyboard_NativeKeyEvent_F6							0x75
#define org_jnativehook_keyboard_NativeKeyEvent_F7							0x76
#define org_jnativehook_keyboard_NativeKeyEvent_F8							0x77
#define org_jnativehook_keyboard_NativeKeyEvent_F9							0x78
#define org_jnativehook_keyboard_NativeKeyEvent_F10							0x79
#define org_jnativehook_keyboard_NativeKeyEvent_F11							0x7A
#define org_jnativehook_keyboard_NativeKeyEvent_F12							0x7B

#define org_jnativehook_keyboard_NativeKeyEvent_F13							0xF000
#define org_jnativehook_keyboard_NativeKeyEvent_F14							0xF001
#define org_jnativehook_keyboard_NativeKeyEvent_F15							0xF002
#define org_jnativehook_keyboard_NativeKeyEvent_F16							0xF003
#define org_jnativehook_keyboard_NativeKeyEvent_F17							0xF004
#define org_jnativehook_keyboard_NativeKeyEvent_F18							0xF005
#define org_jnativehook_keyboard_NativeKeyEvent_F19							0xF006
#define org_jnativehook_keyboard_NativeKeyEvent_F20							0xF007
#define org_jnativehook_keyboard_NativeKeyEvent_F21							0xF008
#define org_jnativehook_keyboard_NativeKeyEvent_F22							0xF009
#define org_jnativehook_keyboard_NativeKeyEvent_F23							0xF00A
#define org_jnativehook_keyboard_NativeKeyEvent_F24							0xF00B


#define org_jnativehook_keyboard_NativeKeyEvent_PRINTSCREEN					0x9A
#define org_jnativehook_keyboard_NativeKeyEvent_INSERT						0x9B
#define org_jnativehook_keyboard_NativeKeyEvent_HELP						0x9C

#define org_jnativehook_keyboard_NativeKeyEvent_PAGE_UP						0x21
#define org_jnativehook_keyboard_NativeKeyEvent_PAGE_DOWN					0x22
#define org_jnativehook_keyboard_NativeKeyEvent_HOME						0x24
#define org_jnativehook_keyboard_NativeKeyEvent_END							0x23

#define org_jnativehook_keyboard_NativeKeyEvent_BACK_QUOTE					0xC0
#define org_jnativehook_keyboard_NativeKeyEvent_QUOTE						0xDE


/* For European keyboards */
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_GRAVE					0x80
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_ACUTE					0x81
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_CIRCUMFLEX				0x82
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_TILDE					0x83
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_MACRON					0x84
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_BREVE					0x85
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_ABOVEDOT				0x86
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_DIAERESIS				0x87
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_ABOVERING				0x88
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_DOUBLEACUTE			0x89
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_CARON					0x8A
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_CEDILLA				0x8B
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_OGONEK					0x8C
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_IOTA					0x8D
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_VOICED_SOUND			0x8E
#define org_jnativehook_keyboard_NativeKeyEvent_DEAD_SEMIVOICED_SOUND		0x8F

/* Unknown Keyboard Codes */
#define org_jnativehook_keyboard_NativeKeyEvent_AMPERSAND					0x96
#define org_jnativehook_keyboard_NativeKeyEvent_ASTERISK					0x97
#define org_jnativehook_keyboard_NativeKeyEvent_QUOTEDBL					0x98
#define org_jnativehook_keyboard_NativeKeyEvent_LESS						0x99
#define org_jnativehook_keyboard_NativeKeyEvent_GREATER						0xA0
#define org_jnativehook_keyboard_NativeKeyEvent_BRACELEFT					0xA1
#define org_jnativehook_keyboard_NativeKeyEvent_BRACERIGHT					0xA2


/* Unknown Extended Keyboard Codes */
#define org_jnativehook_keyboard_NativeKeyEvent_AT							0x0200
#define org_jnativehook_keyboard_NativeKeyEvent_COLON						0x0201
#define org_jnativehook_keyboard_NativeKeyEvent_CIRCUMFLEX					0x0202
#define org_jnativehook_keyboard_NativeKeyEvent_DOLLAR						0x0203
#define org_jnativehook_keyboard_NativeKeyEvent_EURO_SIGN					0x0204
#define org_jnativehook_keyboard_NativeKeyEvent_EXCLAMATION_MARK			0x0205
#define org_jnativehook_keyboard_NativeKeyEvent_INVERTED_EXCLAMATION_MARK	0x0206
#define org_jnativehook_keyboard_NativeKeyEvent_LEFT_PARENTHESIS			0x0207
#define org_jnativehook_keyboard_NativeKeyEvent_NUMBER_SIGN					0x0208
#define org_jnativehook_keyboard_NativeKeyEvent_PLUS						0x0209
#define org_jnativehook_keyboard_NativeKeyEvent_RIGHT_PARENTHESIS			0x020A
#define org_jnativehook_keyboard_NativeKeyEvent_UNDERSCORE					0x020B


/* For input method support on Asian Keyboards */
#define org_jnativehook_keyboard_NativeKeyEvent_FINAL						0x0018	//Unknown Win32 API
#define org_jnativehook_keyboard_NativeKeyEvent_CONVERT						0x001C
#define org_jnativehook_keyboard_NativeKeyEvent_NONCONVERT					0x001D
#define org_jnativehook_keyboard_NativeKeyEvent_ACCEPT						0x001E
#define org_jnativehook_keyboard_NativeKeyEvent_MODECHANGE					0x001F	//Unknown Win32 API
#define org_jnativehook_keyboard_NativeKeyEvent_KANA						0x0015
#define org_jnativehook_keyboard_NativeKeyEvent_KANJI						0x0019
#define org_jnativehook_keyboard_NativeKeyEvent_ALPHANUMERIC				0x00F0
#define org_jnativehook_keyboard_NativeKeyEvent_KATAKANA					0x00F1
#define org_jnativehook_keyboard_NativeKeyEvent_HIRAGANA					0x00F2
#define org_jnativehook_keyboard_NativeKeyEvent_FULL_WIDTH					0x00F3
#define org_jnativehook_keyboard_NativeKeyEvent_HALF_WIDTH					0x00F4
#define org_jnativehook_keyboard_NativeKeyEvent_ROMAN_CHARACTERS			0x00F5
#define org_jnativehook_keyboard_NativeKeyEvent_ALL_CANDIDATES				0x0100
#define org_jnativehook_keyboard_NativeKeyEvent_PREVIOUS_CANDIDATE			0x0101
#define org_jnativehook_keyboard_NativeKeyEvent_CODE_INPUT					0x0102
#define org_jnativehook_keyboard_NativeKeyEvent_JAPANESE_KATAKANA			0x0103
#define org_jnativehook_keyboard_NativeKeyEvent_JAPANESE_HIRAGANA			0x0104
#define org_jnativehook_keyboard_NativeKeyEvent_JAPANESE_ROMAN				0x0105
#define org_jnativehook_keyboard_NativeKeyEvent_KANA_LOCK					0x0106
#define org_jnativehook_keyboard_NativeKeyEvent_INPUT_METHOD_ON_OFF			0x0107

/* For Sun keyboards */
#define org_jnativehook_keyboard_NativeKeyEvent_CUT							0xFFD1
#define org_jnativehook_keyboard_NativeKeyEvent_COPY						0xFFCD
#define org_jnativehook_keyboard_NativeKeyEvent_PASTE						0xFFCF
#define org_jnativehook_keyboard_NativeKeyEvent_UNDO						0xFFCB
#define org_jnativehook_keyboard_NativeKeyEvent_AGAIN						0xFFC9
#define org_jnativehook_keyboard_NativeKeyEvent_FIND						0xFFD0
#define org_jnativehook_keyboard_NativeKeyEvent_PROPS						0xFFCA
#define org_jnativehook_keyboard_NativeKeyEvent_STOP						0xFFC8
#define org_jnativehook_keyboard_NativeKeyEvent_COMPOSE						0xFF20
#define org_jnativehook_keyboard_NativeKeyEvent_ALT_GRAPH					0xFF7E

#define org_jnativehook_keyboard_NativeKeyEvent_BEGIN						0xFF58

#define org_jnativehook_keyboard_NativeKeyEvent_UNDEFINED					0x0		// KeyCode Unknown

#endif
