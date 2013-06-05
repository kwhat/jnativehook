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

#ifndef __NATIVEHOOK_H
#define __NATIVEHOOK_H

/* Begin Error Codes */
#define NATIVEHOOK_SUCCESS							0x00
#define NATIVEHOOK_FAILURE							0x01

#define NATIVEHOOK_ERROR_OUT_OF_MEMORY				0x02

#define NATIVEHOOK_ERROR_THREAD_CREATE				0x10
#define NATIVEHOOK_ERROR_THREAD_INIT				0x11
#define NATIVEHOOK_ERROR_THREAD_START				0x12
#define NATIVEHOOK_ERROR_THREAD_STOP				0x13

#define NATIVEHOOK_ERROR_X_OPEN_DISPLAY				0x20
#define NATIVEHOOK_ERROR_X_RECORD_NOT_FOUND			0x21
#define NATIVEHOOK_ERROR_X_RECORD_ALLOC_RANGE		0x22
#define NATIVEHOOK_ERROR_X_RECORD_CREATE_CONTEXT	0x23
#define NATIVEHOOK_ERROR_X_RECORD_ENABLE_CONTEXT	0x24
/* End Error Codes */


/* Begin Virtual Event Types and Data Structures */
#define EVENT_KEY_PRESSED		1
#define EVENT_KEY_RELEASED		2
#define EVENT_KEY_TYPED			3
#define EVENT_MOUSE_PRESSED		4
#define EVENT_MOUSE_RELEASED	5
#define EVENT_MOUSE_CLICKED		6
#define EVENT_MOUSE_MOVED		7
#define EVENT_MOUSE_DRAGGED		8
#define EVENT_MOUSE_WHEEL		9

typedef struct {
	unsigned short int type;
	unsigned long int time;
	unsigned short int mask;
	void *data;
} VritualEvent;

typedef struct {
	unsigned short int keycode;
	unsigned short int rawcode;
	wchar_t keychar;
} KeyboardEventData, KeyPressedEventData, KeyReleasedEventData, KeyTypedEventData;

typedef struct {
	unsigned short int button;
	unsigned short int clicks;
	unsigned short int x;
	unsigned short int y;
} MouseEventData, MousePressedEventData, MouseReleasedEventData, MouseClickedEventData;

typedef struct {
	unsigned short int type;
	unsigned short int amount;
	signed int rotation;
} MouseWheelEventData;
/* End Virtual Event Types and Data Structures */


/* Begin Virtual Key Codes */
#define VC_ENTER						'\n'
#define VC_BACK_SPACE					'\b'
#define VC_TAB							'\t'
#define VC_CANCEL						0x03

#define VC_SHIFT_L						0x10
#define VC_SHIFT_R						0x0210
#define VC_CONTROL_L					0x11
#define VC_CONTROL_R					0x0211
#define VC_ALT_L						0x12	// Option or Alt Key
#define VC_ALT_R						0x0212	// Option or Alt Key
#define VC_META_L						0x9D	// OSX: Command
#define VC_META_R						0x029D	// OSX: Command
#define VC_SUPER_L						0x0C	// Windows Key
#define VC_SUPER_R						0x020C	// Windows Key
#define VC_CONTEXT_MENU					0x020D

#define VC_PAUSE						0x13
#define VC_CAPS_LOCK					0x14
#define VC_ESCAPE						0x1B


// Begin Cursor Key Zone
#define VC_LEFT							0x0025
#define VC_UP							0x0026
#define VC_RIGHT						0x0027
#define VC_DOWN							0x0028
// End Cursor Key Zone

// Begin Alphanumeric Zone
#define VC_SPACE						0x0020

#define VC_COMMA						0x002C	// ','
#define VC_MINUS						0x002D	// '-'
#define VC_PERIOD						0x002E	// '.'
#define VC_SLASH						0x002F	// '/'

#define VC_0							0x0030
#define VC_1							0x0031
#define VC_2							0x0032
#define VC_3							0x0033
#define VC_4							0x0034
#define VC_5							0x0035
#define VC_6							0x0036
#define VC_7							0x0037
#define VC_8							0x0038
#define VC_9							0x0039

#define VC_SEMICOLON					0x003B	// ';'
#define VC_EQUALS						0x003D	// '='

#define VC_A							0x0041
#define VC_B							0x0042
#define VC_C							0x0043
#define VC_D							0x0044
#define VC_E							0x0045
#define VC_F							0x0046
#define VC_G							0x0047
#define VC_H							0x0048
#define VC_I							0x0049
#define VC_J							0x004A
#define VC_K							0x004B
#define VC_L							0x004C
#define VC_M							0x004D
#define VC_N							0x004E
#define VC_O							0x004F
#define VC_P							0x0050
#define VC_Q							0x0051
#define VC_R							0x0052
#define VC_S							0x0053
#define VC_T							0x0054
#define VC_U							0x0055
#define VC_V							0x0056
#define VC_W							0x0057
#define VC_X							0x0058
#define VC_Y							0x0059
#define VC_Z							0x005A

#define VC_OPEN_BRACKET					0x005B	// '['
#define VC_BACK_SLASH					0x005C	// '\'
#define VC_CLOSE_BRACKET				0x005D	// ']'

// Begin Numeric Zone
#define VC_KP_0							0x0130
#define VC_KP_1							0x0131
#define VC_KP_2							0x0132
#define VC_KP_3							0x0133
#define VC_KP_4							0x0134
#define VC_KP_5							0x0135
#define VC_KP_6							0x0136
#define VC_KP_7							0x0137
#define VC_KP_8							0x0138
#define VC_KP_9							0x0139

#define VC_KP_LEFT						0x0125
#define VC_KP_UP						0x0126
#define VC_KP_DOWN						0x0127
#define VC_KP_RIGHT						0x0128

#define VC_KP_ENTER						0x016A
#define VC_KP_MULTIPLY					0x016A
#define VC_KP_ADD						0x016B
#define VC_KP_SEPARATOR					0x016C
#define VC_KP_SUBTRACT					0x016D
#define VC_KP_DECIMAL					0x016E
#define VC_KP_DIVIDE					0x016F
#define VC_KP_DELETE					0x017F

#define VC_DELETE						0x7F
#define VC_NUM_LOCK						0x90
#define VC_CLEAR						0x03	// OSX: Number Lock
#define VC_SCROLL_LOCK					0x91


// Begin Function Keys
#define VC_F1							0x70
#define VC_F2							0x71
#define VC_F3							0x72
#define VC_F4							0x73
#define VC_F5							0x74
#define VC_F6							0x75
#define VC_F7							0x76
#define VC_F8							0x77
#define VC_F9							0x78
#define VC_F10							0x79
#define VC_F11							0x7A
#define VC_F12							0x7B

#define VC_F13							0xF000
#define VC_F14							0xF001
#define VC_F15							0xF002
#define VC_F16							0xF003
#define VC_F17							0xF004
#define VC_F18							0xF005
#define VC_F19							0xF006
#define VC_F20							0xF007
#define VC_F21							0xF008
#define VC_F22							0xF009
#define VC_F23							0xF00A
#define VC_F24							0xF00B
// End Function Keys


#define VC_PRINTSCREEN					0x9A
#define VC_INSERT						0x9B
#define VC_HELP							0x9C

#define VC_PAGE_UP						0x21
#define VC_PAGE_DOWN					0x22
#define VC_HOME							0x24
#define VC_END							0x23

#define VC_BACK_QUOTE					0xC0
#define VC_QUOTE						0xDE


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
#define VC_LESS							0x99
#define VC_GREATER						0xA0
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

// For Sun keyboards.
#define VC_CUT							0xFFD1
#define VC_COPY							0xFFCD
#define VC_PASTE						0xFFCF
#define VC_UNDO							0xFFCB
#define VC_AGAIN						0xFFC9
#define VC_FIND							0xFFD0
#define VC_PROPS						0xFFCA
#define VC_STOP							0xFFC8
#define VC_COMPOSE						0xFF20
#define VC_ALT_GRAPH					0xFF7E

#define VC_BEGIN						0xFF58

#define CHAR_UNDEFINED					0xFFFF	// CharCode Unknown

#define VC_UNDEFINED					0x0000	// KeyCode Unknown
/* End Virtual Key Codes */


/* Begin Virtual Modifier Masks */
#define MASK_SHIFT_L		1 << 0
#define MASK_CTRL_L			1 << 1
#define MASK_META_L			1 << 2
#define MASK_ALT_L			1 << 3

#define MASK_SHIFT_R		1 << 4
#define MASK_CTRL_R			1 << 5
#define MASK_META_R			1 << 6
#define MASK_ALT_R			1 << 7

#define MASK_BUTTON1		1 << 8
#define MASK_BUTTON2		1 << 9
#define MASK_BUTTON3		1 << 10
#define MASK_BUTTON4		1 << 11
#define MASK_BUTTON5		1 << 12
/* End Virtual Modifier Masks */


/* Begin Virtual Mouse Buttons */
#define MOUSE_NOBUTTON			0	// AnyButton
#define MOUSE_BUTTON1			1
#define MOUSE_BUTTON2			2
#define MOUSE_BUTTON3			3
#define MOUSE_BUTTON4			4	// Extra Mouse Button
#define MOUSE_BUTTON5			5	// Extra Mouse Button

#define WHEEL_UNIT_SCROLL		1
#define WHEEL_BLOCK_SCROLL		2
/* End Virtual Mouse Buttons */


#ifdef _WIN32
      #define NATIVEHOOK_API __declspec(dllexport)
#else
      #define NATIVEHOOK_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	NATIVEHOOK_API void hook_set_dispatch_proc(void (*dispatch_proc)(VirtualEvent * const));
	NATIVEHOOK_API int hook_enable();
	NATIVEHOOK_API int hook_disable();
	NATIVEHOOK_API bool hook_is_enabled();

#ifdef __cplusplus
}
#endif

#endif
