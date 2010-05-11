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
//Reference: ${JAVA_HOME}/src.zip/java/awt/event/KeyEvent.java
//http://java.sun.com/j2se/1.5.0/docs/api/constant-values.html#java.awt.event.KeyEvent.CHAR_UNDEFINED

#define JK_ENTER						'\n'
#define JK_BACK_SPACE					'\b'
#define JK_TAB							'\t'
//#define JK_CANCEL						0x0C

#define JK_SHIFT						0x10
#define JK_CONTROL						0x11
#define JK_ALT							0x12	//option or alt key
#define JK_META							0x9D	//osx: command
#define JK_WINDOWS						0x020C
#define JK_CONTEXT_MENU					0x020D

#define JK_PAUSE						0x13
#define JK_CAPS_LOCK					0x14
#define JK_ESCAPE						0x1B
#define JK_SPACE						0x20

#define JK_UP							0x26
#define JK_DOWN							0x28
#define JK_LEFT							0x25
#define JK_RIGHT						0x27

#define JK_COMMA						0x2C	// ','
#define JK_MINUS						0x2D	// '-'
#define JK_PERIOD						0x2E	// '.'
#define JK_SLASH						0x2F	// '/'

#define JK_0							0x30
#define JK_1							0x31
#define JK_2							0x32
#define JK_3							0x33
#define JK_4							0x34
#define JK_5							0x35
#define JK_6							0x36
#define JK_7							0x37
#define JK_8							0x38
#define JK_9							0x39

#define JK_SEMICOLON					0x3B	// ';'
#define JK_EQUALS						0x3D	// '='

#define JK_A							0x41
#define JK_B							0x42
#define JK_C							0x43
#define JK_D							0x44
#define JK_E							0x45
#define JK_F							0x46
#define JK_G							0x47
#define JK_H							0x48
#define JK_I							0x49
#define JK_J							0x4A
#define JK_K							0x4B
#define JK_L							0x4C
#define JK_M							0x4D
#define JK_N							0x4E
#define JK_O							0x4F
#define JK_P							0x50
#define JK_Q							0x51
#define JK_R							0x52
#define JK_S							0x53
#define JK_T							0x54
#define JK_U							0x55
#define JK_V							0x56
#define JK_W							0x57
#define JK_X							0x58
#define JK_Y							0x59
#define JK_Z							0x5A

#define JK_OPEN_BRACKET					0x5B	// '['
#define JK_BACK_SLASH					0x5C	// '\'
#define JK_CLOSE_BRACKET				0x5D	// ']'

#define JK_NUMPAD0						0x60
#define JK_NUMPAD1						0x61
#define JK_NUMPAD2						0x62
#define JK_NUMPAD3						0x63
#define JK_NUMPAD4						0x64
#define JK_NUMPAD5						0x65
#define JK_NUMPAD6						0x66
#define JK_NUMPAD7						0x67
#define JK_NUMPAD8						0x68
#define JK_NUMPAD9						0x69

#define JK_KP_UP						0xE0
#define JK_KP_DOWN						0xE1
#define JK_KP_LEFT						0xE2
#define JK_KP_RIGHT						0xE3

#define JK_MULTIPLY						0x6A
#define JK_ADD							0x6B
//#define JK_SEPARATOR					0x6C
#define JK_SUBTRACT						0x6D
#define JK_DECIMAL						0x6E
#define JK_DIVIDE						0x6F
#define JK_DELETE						0x7F
#define JK_NUM_LOCK						0x90
#define JK_CLEAR						0x03	//osx: number lock
#define JK_SCROLL_LOCK					0x91

#define JK_F1							0x70
#define JK_F2							0x71
#define JK_F3							0x72
#define JK_F4							0x73
#define JK_F5							0x74
#define JK_F6							0x75
#define JK_F7							0x76
#define JK_F8							0x77
#define JK_F9							0x78
#define JK_F10							0x79
#define JK_F11							0x7A
#define JK_F12							0x7B

#define JK_F13							0xF000
#define JK_F14							0xF001
#define JK_F15							0xF002
#define JK_F16							0xF003
#define JK_F17							0xF004
#define JK_F18							0xF005
#define JK_F19							0xF006
#define JK_F20							0xF007
#define JK_F21							0xF008
#define JK_F22							0xF009
#define JK_F23							0xF00A
#define JK_F24							0xF00B


#define JK_PRINTSCREEN					0x9A
#define JK_INSERT						0x9B
#define JK_HELP							0x9C

#define JK_PAGE_UP						0x21
#define JK_PAGE_DOWN					0x22
#define JK_HOME							0x24
#define JK_END							0x23

#define JK_BACK_QUOTE					0xC0
#define JK_QUOTE						0xDE


/* For European keyboards */
/* not implemented yet
#define JK_DEAD_GRAVE					0x80
#define JK_DEAD_ACUTE					0x81
#define JK_DEAD_CIRCUMFLEX				0x82
#define JK_DEAD_TILDE					0x83
#define JK_DEAD_MACRON					0x84
#define JK_DEAD_BREVE					0x85
#define JK_DEAD_ABOVEDOT				0x86
#define JK_DEAD_DIAERESIS				0x87
#define JK_DEAD_ABOVERING				0x88
#define JK_DEAD_DOUBLEACUTE				0x89
#define JK_DEAD_CARON					0x8A
#define JK_DEAD_CEDILLA					0x8B
#define JK_DEAD_OGONEK					0x8C
#define JK_DEAD_IOTA					0x8D
#define JK_DEAD_VOICED_SOUND			0x8E
#define JK_DEAD_SEMIVOICED_SOUND		0x8F
*/

/*
#define JK_AMPERSAND					0x96
#define JK_ASTERISK						0x97
#define JK_QUOTEDBL						0x98
#define JK_LESS							0x99
#define JK_GREATER						0xA0
#define JK_BRACELEFT					0xA1
#define JK_BRACERIGHT					0xA2
*/

/*
#define JK_AT							0x0200
#define JK_COLON						0x0201
#define JK_CIRCUMFLEX					0x0202
#define JK_DOLLAR						0x0203
#define JK_EURO_SIGN					0x0204
#define JK_EXCLAMATION_MARK				0x0205
#define JK_INVERTED_EXCLAMATION_MARK	0x0206
#define JK_LEFT_PARENTHESIS				0x0207
#define JK_NUMBER_SIGN					0x0208
#define JK_PLUS							0x0209
#define JK_RIGHT_PARENTHESIS			0x020A
#define JK_UNDERSCORE					0x020B
*/

/* for input method support on Asian Keyboards */
/* not implemented yet
#define JK_FINAL						0x0018	//Unknown Win32 API
#define JK_CONVERT						0x001C
#define JK_NONCONVERT					0x001D
#define JK_ACCEPT						0x001E
#define JK_MODECHANGE					0x001F	//Unknown Win32 API
#define JK_KANA							0x0015
#define JK_KANJI						0x0019
#define JK_ALPHANUMERIC					0x00F0
#define JK_KATAKANA						0x00F1
#define JK_HIRAGANA						0x00F2
#define JK_FULL_WIDTH					0x00F3
#define JK_HALF_WIDTH					0x00F4
#define JK_ROMAN_CHARACTERS				0x00F5
#define JK_ALL_CANDIDATES				0x0100
#define JK_PREVIOUS_CANDIDATE			0x0101
#define JK_CODE_INPUT					0x0102
#define JK_JAPANESE_KATAKANA			0x0103
#define JK_JAPANESE_HIRAGANA			0x0104
#define JK_JAPANESE_ROMAN				0x0105
#define JK_KANA_LOCK					0x0106
#define JK_INPUT_METHOD_ON_OFF			0x0107
*/

/* for Sun keyboards */
/* not implemented yet
#define JK_CUT							0xFFD1
#define JK_COPY							0xFFCD
#define JK_PASTE						0xFFCF
#define JK_UNDO							0xFFCB
#define JK_AGAIN						0xFFC9
#define JK_FIND							0xFFD0
#define JK_PROPS						0xFFCA
#define JK_STOP							0xFFC8
#define JK_COMPOSE						0xFF20
#define JK_ALT_GRAPH					0xFF7E
*/

#define JK_BEGIN						0xFF58

#define JK_UNDEFINED					0x0		// KeyCode Unknown
