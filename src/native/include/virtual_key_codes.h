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

#ifndef _included_virtual_key_codes
#define _included_virtual_key_codes

#define NH_EVENT_KEY_PRESSED			1
#define NH_EVENT_KEY_RELEASED			2
#define NH_EVENT_KEY_TYPED				3

#define VC_UNKNOWN						0x00	// KeyCode Unknown

#define VC_A							0x04
#define VC_B							0x05
#define VC_C							0x06
#define VC_D							0x07
#define VC_E							0x08
#define VC_F							0x09
#define VC_G							0x0A
#define VC_H							0x0B
#define VC_I							0x0C
#define VC_J							0x0D
#define VC_K							0x0E
#define VC_L							0x0F
#define VC_M							0x10
#define VC_N							0x11
#define VC_O							0x12
#define VC_P							0x13
#define VC_Q							0x14
#define VC_R							0x15
#define VC_S							0x16
#define VC_T							0x17
#define VC_U							0x18
#define VC_V							0x19
#define VC_W							0x1A
#define VC_X							0x1B
#define VC_Y							0x1C
#define VC_Z							0x1D

#define VC_0							0x1E
#define VC_1							0x1F
#define VC_2							0x20
#define VC_3							0x21
#define VC_4							0x22
#define VC_5							0x23
#define VC_6							0x24
#define VC_7							0x25
#define VC_8							0x26
#define VC_9							0x27

#define VC_ENTER						0x28
#define VC_ESCAPE						0x29
#define VC_BACK_SPACE					0x2A
#define VC_TAB							0x2B
#define VC_SPACE						0x2C

#define VC_MINUS						0x2D	// '-'
#define VC_EQUALS						0x2E	// '='
#define VC_OPEN_BRACKET					0x2F	// '['
#define VC_CLOSE_BRACKET				0x30	// ']'
#define VC_BACK_SLASH					0x31	// '\'
//Non-US # and ~						0x32
#define VC_SEMICOLON					0x33	// ';'
#define VC_QUOTE						0x34	// '''
#define VC_BACK_QUOTE					0x35	// '`'
#define VC_LESS							0x36	// '<'
#define VC_GREATER						0x37	// '>'
#define VC_SLASH						0x38	// '/'
#define VC_CAPS_LOCK					0x39

#define VC_F1							0x3A
#define VC_F2							0x3B
#define VC_F3							0x3C
#define VC_F4							0x3D
#define VC_F5							0x3E
#define VC_F6							0x3F
#define VC_F7							0x40
#define VC_F8							0x41
#define VC_F9							0x42
#define VC_F10							0x43
#define VC_F11							0x44
#define VC_F12							0x45

#define VC_PRINT_SCREEN					0x46
#define VC_SCROLL_LOCK					0x47
#define VC_PAUSE						0x48
#define VC_INSERT						0x49
#define VC_HOME							0x4A
#define VC_PAGE_UP						0x4B
#define VC_DELETE						0x4C
#define VC_END							0x4D
#define VC_PAGE_DOWN					0x4E
#define VC_RIGHT						0x4F
#define VC_LEFT							0x50
#define VC_DOWN							0x51
#define VC_UP							0x52
#define VC_KP_NUM_LOCK					0x53	// Clear
#define VC_KP_DIVIDE					0x54
#define VC_KP_MULTIPLY					0x55
#define VC_KP_SUBTRACT					0x56
#define VC_KP_ADD						0x57
#define VC_KP_ENTER						0x58

#define VC_KP_1							0x59
#define VC_KP_2							0x5A
#define VC_KP_3							0x5B
#define VC_KP_4							0x5C
#define VC_KP_5							0x5D
#define VC_KP_6							0x5E
#define VC_KP_7							0x5F
#define VC_KP_8							0x60
#define VC_KP_9							0x61
#define VC_KP_0							0x62
#define VC_KP_DECIMAL					0x63
//Keyboard Non-US \ and |				0x64	// '\'	Non-US
#define VC_MENU							0x65	// Application or Compose?
//Keyboard Power						0x66
#define VC_KP_EQUALS					0x67	// '='	Non-US
#define VC_F13							0x68
#define VC_F14							0x69
#define VC_F15							0x6A
#define VC_F16							0x6B
#define VC_F17							0x6C
#define VC_F18							0x6D
#define VC_F19							0x6E
#define VC_F20							0x6F
#define VC_F21							0x70
#define VC_F22							0x71
#define VC_F23							0x72
#define VC_F24							0x73
//Keyboard Execute						0x74
#define VC_HELP							0x75
//Keyboard Menu							0x76	// Application or Compose?
//Keyboard Select						0x77
#define VC_STOP							0x78
#define VC_AGAIN						0x79
#define VC_UNDO							0x7A
#define VC_CUT							0x7B
#define VC_COPY							0x7C
#define VC_PASTE						0x7D
#define VC_FIND							0x7E
//Keyboard Mute							0x7F
//Keyboard Volume Up					0x80
//Keyboard Volume Down					0x81
//Keyboard Locking Caps Lock			0x82
//Keyboard Locking Num Lock				0x83
//Keyboard Locking Scroll Lock			0x84
#define VC_KP_COMMA						0x85	// ','
//Keypad Equal Sign						0x86

#define VC_LCONTROL						0xE0
#define VC_LSHIFT						0xE1
#define VC_LALT							0xE2	// Option or Alt Key
#define VC_LWINDOWS						0xE3

#define VC_RCONTROL						0xE4
#define VC_RSHIFT						0xE5
#define VC_RALT							0xE6	// Option or Alt Key
#define VC_RWINDOWS						0xE7




#define VC_PROPS						0xFFCA
#define VC_COMPOSE						0xFF20
#define VC_ALT_GRAPH					0xFF7E

#define VC_SEPARATOR					0x6C
#define VC_CANCEL						0x0C

#define VC_LMETA						0x9D	// OSX: Command
#define VC_RMETA						0x9D	// OSX: Command

#define VC_PERIOD						0x2E	// '.'

#define VC_CLEAR						0x03	// OSX: Number Lock


// For European keyboards.
#define VC_DEAD_GRAVE					0x80
#define VC_DEAD_ACUTE					0x81
#define VC_DEAD_CIRCUMFLEX				0x82
#define VC_DEAD_TILDE					0x83
#define VC_DEAD_MACRON					0x84
#define VC_DEAD_BREVE					0x85
#define VC_DEAD_ABOVEDOT				0x86
#define VC_DEAD_DIAERESIS				0x87
#define VC_DEAD_ABOVERING				0x88
#define VC_DEAD_DOUBLEACUTE				0x89
#define VC_DEAD_CARON					0x8A
#define VC_DEAD_CEDILLA					0x8B
#define VC_DEAD_OGONEK					0x8C
#define VC_DEAD_IOTA					0x8D
#define VC_DEAD_VOICED_SOUND			0x8E
#define VC_DEAD_SEMIVOICED_SOUND		0x8F

// Unknown Keyboard Codes.
#define VC_AMPERSAND					0x96
#define VC_ASTERISK						0x97
#define VC_QUOTEDBL						0x98



#define VC_BRACELEFT					0xA1
#define VC_BRACERIGHT					0xA2


// Unknown Extended Keyboard Codes.
#define VC_AT							0x0200
#define VC_COLON						0x0201
#define VC_CIRCUMFLEX					0x0202
#define VC_DOLLAR						0x0203
#define VC_EURO_SIGN					0x0204
#define VC_EXCLAMATION_MARK				0x0205
#define VC_INVERTED_EXCLAMATION_MARK	0x0206
#define VC_LEFT_PARENTHESIS				0x0207
#define VC_NUMBER_SIGN					0x0208
#define VC_PLUS							0x0209
#define VC_RIGHT_PARENTHESIS			0x020A
#define VC_UNDERSCORE					0x020B


// For input method support on Asian Keyboards.
#define VC_FINAL						0x0018	// Unknown Win32 API
#define VC_CONVERT						0x001C
#define VC_NONCONVERT					0x001D
#define VC_ACCEPT						0x001E
#define VC_MODECHANGE					0x001F	// Unknown Win32 API
#define VC_KANA							0x0015
#define VC_KANJI						0x0019
#define VC_ALPHANUMERIC					0x00F0
#define VC_KATAKANA						0x00F1
#define VC_HIRAGANA						0x00F2
#define VC_FULL_WIDTH					0x00F3
#define VC_HALF_WIDTH					0x00F4
#define VC_ROMAN_CHARACTERS				0x00F5
#define VC_ALL_CANDIDATES				0x0100
#define VC_PREVIOUS_CANDIDATE			0x0101
#define VC_CODE_INPUT					0x0102
#define VC_JAPANESE_KATAKANA			0x0103
#define VC_JAPANESE_HIRAGANA			0x0104
#define VC_JAPANESE_ROMAN				0x0105
#define VC_KANA_LOCK					0x0106
#define VC_INPUT_METHOD_ON_OFF			0x0107

#define VC_BEGIN						0xFF58



#endif
