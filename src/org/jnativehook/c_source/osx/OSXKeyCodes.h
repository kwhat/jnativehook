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
//Reference: http://boredzo.org/blog/wp-content/uploads/2007/05/imtx-virtual-keycodes.png
//Reference: https://svn.blender.org/svnroot/bf-blender/branches/render25/intern/ghost/intern/GHOST_SystemCocoa.mm
//Reference: http://www.mactech.com/macintosh-c/chap02-1.html

/* Apple's OSX does not track the button masks of its events so we need to do
 * it manually.  The left and right masks solve the problem of if both control
 * keys are depressed a the same time and only one is removed we should still
 * have a control mask but the key up event would have reset that.
 */

/*
unsigned short getModifierMask();
void setModifierMask(unsigned short mod);
void unsetModifierMask(unsigned short mod);
bool isModifierMask(unsigned short mod);
*/

//TODO This is probably not needed, maybe just for undefined keycodes.
/*
#define AK_ENTER						0x24
#define AK_BACK_SPACE					0x33
#define AK_TAB							0x30
#define AK_SHIFT						0x38
#define AK_CONTROL						0x3B
#define AK_ALT							0x37	//Command Key
#define AK_PAUSE						AK_F16
#define AK_CAPS_LOCK					0x39
#define AK_ESCAPE						0x35
#define AK_SPACE						0x31
#define AK_PAGE_UP						0x74
#define AK_PAGE_DOWN					0x79
#define AK_HOME							0x73
#define AK_END							0x77

#define AK_LEFT							0x7B
#define AK_UP							0x7E
#define AK_RIGHT						0x7C
#define AK_DOWN							0x7D

#define AK_COMMA						0x2B	// ','
#define AK_MINUS						0x1B	// '-'
#define AK_PERIOD						0x2F	// '.'
#define AK_SLASH						0x2C	// '/'

#define AK_0							0x1D
#define AK_1							0x12
#define AK_2							0x13
#define AK_3							0x14
#define AK_4							0x15
#define AK_5							0x17
#define AK_6							0x16
#define AK_7							0x1A
#define AK_8							0x1C
#define AK_9							0x19

#define AK_SEMICOLON					0x29	// ';'
#define AK_EQUALS						0x18	// '='

#define AK_A							0x00
#define AK_B							0x0B
#define AK_C							0x08
#define AK_D							0x02
#define AK_E							0x0E
#define AK_F							0x03
#define AK_G							0x05
#define AK_H							0x04
#define AK_I							0x22
#define AK_J							0x26
#define AK_K							0x28
#define AK_L							0x25
#define AK_M							0x2E
#define AK_N							0x2D
#define AK_O							0x1F
#define AK_P							0x23
#define AK_Q							0x0C
#define AK_R							0x0F
#define AK_S							0x01
#define AK_T							0x11
#define AK_U							0x20
#define AK_V							0x09
#define AK_W							0x0D
#define AK_X							0x07
#define AK_Y							0x10
#define AK_Z							0x06

#define AK_OPEN_BRACKET					0x21	// '['
#define AK_BACK_SLASH					0x2A	// '\'
#define AK_CLOSE_BRACKET				0x1E	// ']'

#define AK_NUMPAD0						0x52
#define AK_NUMPAD1						0x53
#define AK_NUMPAD2						0x54
#define AK_NUMPAD3						0x55
#define AK_NUMPAD4						0x56
#define AK_NUMPAD5						0x57
#define AK_NUMPAD6						0x58
#define AK_NUMPAD7						0x59
#define AK_NUMPAD8						0x5B
#define AK_NUMPAD9						0x5C
#define AK_MULTIPLY						0x43
#define AK_ADD							0x45

//#define AK_SEPARATOR					0x

#define AK_SUBTRACT						0x4E
#define AK_DECIMAL						0x41
#define AK_DIVIDE						0x4B
#define AK_DELETE						0x75
#define AK_NUM_LOCK						0x47
#define AK_SCROLL_LOCK					AK_F14

#define AK_F1							0x7A
#define AK_F2							0x78
#define AK_F3							0x63
#define AK_F4							0x76
#define AK_F5							0x60
#define AK_F6							0x61
#define AK_F7							0x62
#define AK_F8							0x64
#define AK_F9							0x65
#define AK_F10							0x6D
#define AK_F11							0x67
#define AK_F12							0x6F

#define AK_F13							0x69
#define AK_F14							0x6B
#define AK_F15							0x71
#define AK_F16							0x6A
//#define AK_F17							0x
//#define AK_F18							0x
//#define AK_F19							0x
//#define AK_F20							0x
//#define AK_F21							0x
//#define AK_F22							0x
//#define AK_F23							0x
//#define AK_F24							0x


#define AK_PRINTSCREEN					AK_F13
#define AK_INSERT						0x72	// Help Key
#define AK_HELP							AK_INSERT
#define AK_META							0x

#define AK_BACK_QUOTE					0x32
#define AK_QUOTE						0x27

//#define AK_KP_UP						0x
//#define AK_KP_DOWN						0x
//#define AK_KP_LEFT						0x
//#define AK_KP_RIGHT						0x


// For European keyboards
//#define AK_DEAD_GRAVE					0x
//#define AK_DEAD_ACUTE					0x
//#define AK_DEAD_CIRCUMFLEX			0x
//#define AK_DEAD_TILDE					0x
//#define AK_DEAD_MACRON				0x
//#define AK_DEAD_BREVE					0x
//#define AK_DEAD_ABOVEDOT				0x
//#define AK_DEAD_DIAERESIS				0x
//#define AK_DEAD_ABOVERING				0x
//#define AK_DEAD_DOUBLEACUTE			0x
//#define AK_DEAD_CARON					0x
//#define AK_DEAD_CEDILLA				0x
//#define AK_DEAD_OGONEK				0x
//#define AK_DEAD_IOTA					0x
//#define AK_DEAD_VOICED_SOUND			0x
//#define AK_DEAD_SEMIVOICED_SOUND		0x

#define AK_AMPERSAND					0x
#define AK_ASTERISK						0x
#define AK_QUOTEDBL						0x
#define AK_LESS							0x
#define AK_GREATER						0x
#define AK_BRACELEFT					0x
#define AK_BRACERIGHT					0x

#define AK_AT							0x
#define AK_COLON						0x
#define AK_CIRCUMFLEX					0x
#define AK_DOLLAR						0x
#define AK_EURO_SIGN					0x
#define AK_EXCLAMATION_MARK				0x
#define AK_INVERTED_EXCLAMATION_MARK	0x
#define AK_LEFT_PARENTHESIS				0x
#define AK_NUMBER_SIGN					0x
#define AK_PLUS							0x
#define AK_RIGHT_PARENTHESIS			0x
#define AK_UNDERSCORE					0x

#define AK_WINDOWS						AK_ALT
#define AK_CONTEXT_MENU					0x3A	//OPTION KEY

// for input method support on Asian Keyboards
//#define AK_FINAL						0x	//Unknown Win32 API
//#define AK_CONVERT					0x
//#define AK_NONCONVERT					0x
//#define AK_ACCEPT						0x
//#define AK_MODECHANGE					0x	//Unknown Win32 API
//#define AK_KANA						0x
//#define AK_KANJI						0x
//#define AK_ALPHANUMERIC				0x
//#define AK_KATAKANA					0x
//#define AK_HIRAGANA					0x
//#define AK_FULL_WIDTH					0x
//#define AK_HALF_WIDTH					0x
//#define AK_ROMAN_CHARACTERS			0x
//#define AK_ALL_CANDIDATES				0x
//#define AK_PREVIOUS_CANDIDATE			0x
//#define AK_CODE_INPUT					0x
//#define AK_JAPANESE_KATAKANA			0x
//#define AK_JAPANESE_HIRAGANA			0x
//#define AK_JAPANESE_ROMAN				0x
//#define AK_KANA_LOCK					0x
//#define AK_INPUT_METHOD_ON_OFF		0x


//#define AK_COMPOSE					0x
//#define AK_ALT_GRAPH					0x
//#define AK_BEGIN						0x

#define kVK_UNDEFINED					0xFF		//Key Undefined
*/
