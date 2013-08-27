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

#include <stdbool.h>
#include <wchar.h>

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

#define NATIVEHOOK_ERROR_SET_WINDOWS_HOOK_EX		0x30
/* End Error Codes */


/* Begin Virtual Event Types and Data Structures */
typedef enum _EventType {
	EVENT_KEY_PRESSED = 1,
	EVENT_KEY_RELEASED,
	EVENT_KEY_TYPED,
	EVENT_MOUSE_PRESSED,
	EVENT_MOUSE_RELEASED,
	EVENT_MOUSE_CLICKED,
	EVENT_MOUSE_MOVED,
	EVENT_MOUSE_DRAGGED,
	EVENT_MOUSE_WHEEL
} EventType;

typedef struct _KeyboardEventData {
	unsigned short keycode;
	unsigned short scancode;
	wchar_t keychar;
} KeyboardEventData, KeyPressedEventData, KeyReleasedEventData, KeyTypedEventData;

typedef struct _MouseEventData {
	unsigned short button;
	unsigned short clicks;
	unsigned short x;
	unsigned short y;
} MouseEventData, MousePressedEventData, MouseReleasedEventData, MouseClickedEventData;

typedef struct _MouseWheelEventData {
	unsigned short clicks;
	unsigned short x;
	unsigned short y;
	unsigned short type;
	unsigned short amount;
	signed short rotation;
} MouseWheelEventData;

typedef struct _VritualEvent {
	EventType type;
	unsigned long long time;
	unsigned short mask;
	bool propagate;
	union {
		KeyboardEventData keyboard;
		MouseEventData mouse;
		MouseWheelEventData wheel;
	} data;
} VirtualEvent;
/* End Virtual Event Types and Data Structures */


/* Begin Virtual Key Codes */

#define VC_ESCAPE								0x0001

// Begin Function Keys
#define VC_F1									0x003B
#define VC_F2									0x003C
#define VC_F3									0x003D
#define VC_F4									0x003E
#define VC_F5									0x003F
#define VC_F6									0x0040
#define VC_F7									0x0041
#define VC_F8									0x0042
#define VC_F9									0x0043
#define VC_F10									0x0044
#define VC_F11									0x0057
#define VC_F12									0x0058

#define VC_F13									0x005B
#define VC_F14									0x005C
#define VC_F15									0x005D
#define VC_F16									0x0063
#define VC_F17									0x0064
#define VC_F18									0x0065
#define VC_F19									0x0066
#define VC_F20									0x0067
#define VC_F21									0x0068
#define VC_F22									0x0069
#define VC_F23									0x006A
#define VC_F24									0x006B
// End Function Keys


// Begin Alphanumeric Zone
#define VC_BACK_QUOTE							0x0029

#define VC_1									0x0002
#define VC_2									0x0003
#define VC_3									0x0004
#define VC_4									0x0005
#define VC_5									0x0006
#define VC_6									0x0007
#define VC_7									0x0008
#define VC_8									0x0009
#define VC_9									0x000A
#define VC_0									0x000B

#define VC_MINUS								0x000C	// '-'
#define VC_EQUALS								0x003D	// '='
#define VC_BACK_SPACE							0x000E

#define VC_TAB									0x000F
#define VC_CAPS_LOCK							0x003A

#define VC_A									0x001E
#define VC_B									0x0030
#define VC_C									0x002E
#define VC_D									0x0020
#define VC_E									0x0012
#define VC_F									0x0021
#define VC_G									0x0022
#define VC_H									0x0023
#define VC_I									0x0017
#define VC_J									0x0024
#define VC_K									0x0025
#define VC_L									0x0026
#define VC_M									0x0032
#define VC_N									0x0031
#define VC_O									0x0018
#define VC_P									0x0019
#define VC_Q									0x0010
#define VC_R									0x0013
#define VC_S									0x001F
#define VC_T									0x0014
#define VC_U									0x0016
#define VC_V									0x002F
#define VC_W									0x0011
#define VC_X									0x002D
#define VC_Y									0x0015
#define VC_Z									0x002C

#define VC_OPEN_BRACKET							0x001A	// '['
#define VC_CLOSE_BRACKET						0x001B	// ']'
#define VC_BACK_SLASH							0x002B	// '\'

#define VC_SEMICOLON							0x003B	// ';'
#define VC_QUOTE								0x0028
#define VC_ENTER								0x001C

#define VC_COMMA								0x0033	// ','
#define VC_PERIOD								0x0034	// '.'
#define VC_SLASH								0x0035	// '/'

#define VC_SPACE								0x0039
// End Alphanumeric Zone


#define VC_PRINTSCREEN							0x0E37
#define VC_SCROLL_LOCK							0x0046
//FIXME #define VC_PAUSE								0x13


// Begin Edit Key Zone
#define VC_INSERT								0x0E52
#define VC_DELETE								0x0E53
#define VC_HOME									0x0E47
#define VC_END									0x0E4F
#define VC_PAGE_UP								0x0E49
#define VC_PAGE_DOWN							0x0E51
// End Edit Key Zone


// Begin Cursor Key Zone
#define VC_UP									0xE048
#define VC_LEFT									0xE04B
#define VC_RIGHT								0xE04D
#define VC_DOWN									0xE050
// End Cursor Key Zone


// Begin Numeric Zone
#define VC_CLEAR								0x0E45	// 0x76?? OSX: Number Lock
#define VC_NUM_LOCK								0x0E45
#define VC_KP_DIVIDE							0x0E35
#define VC_KP_MULTIPLY							0x0037
#define VC_KP_SUBTRACT							0x004A

#define VC_KP_ADD								0x004E
#define VC_KP_ENTER								0x0E1C
#define VC_KP_SEPARATOR							0x0053

#define VC_KP_1									0x004F
#define VC_KP_2									0x0050
#define VC_KP_3									0x0051
#define VC_KP_4									0x004B
#define VC_KP_5									0x004C
#define VC_KP_6									0x004D
#define VC_KP_7									0x0047
#define VC_KP_8									0x0048
#define VC_KP_9									0x0049
#define VC_KP_0									0x0052
// End Numeric Zone


// Begin Modifier and Control Keys
#define VC_SHIFT_L								0x002A
#define VC_SHIFT_R								0x0E36
#define VC_CONTROL_L							0x001D
#define VC_CONTROL_R							0x0E1D
#define VC_ALT_L								0xD038	// Option or Alt Key
#define VC_ALT_R								0xDE38	// Option or Alt Key
#define VC_META_L								0x0E5B	// Windows or Command Key
#define VC_META_R								0x0E5C	// Windows or Command Key
#define VC_CONTEXT_MENU							0x0E5D
// End Modifier and Control Keys


// Begin Media Control Keys
#define VC_POWER								0xE05E
#define VC_SLEEP								0xE05F
#define VC_WAKE									0xE063
#define VC_MUTE									0xE020
#define VC_VOLUME_UP							0xE030
#define VC_VOLUME_DOWN							0xE02E
#define VC_CUT									0xE017
#define VC_COPY									0xE018
#define VC_PASTE								0xE00A
#define VC_HELP									0xE03B
#define VC_UNDO									0xE008
#define VC_REDO									0xE007
#define VC_PLAY									0xE022
#define VC_STOP									0xE024
#define VC_REWIND								0xE010
#define VC_FAST_FORWARD							0xE019
#define VC_EJECT								0xE02C
#define VC_MAIL									0xE01E
#define VC_WEB									0xE032
#define VC_MUSIC								0xE03C
#define VC_PICTURES								0xE064
#define VC_VIDEO								0xE06D
// End Media Control Keys


#define VC_KATAKANA								0x0070
#define VC_FURIGANA								0x0077
#define VC_KANJI								0x0079
#define VC_HIRAGANA								0x007B


// For European keyboards.
/*
#define VC_DEAD_GRAVE							0x80
#define VC_DEAD_ACUTE							0x81
#define VC_DEAD_CIRCUMFLEX						0x82
#define VC_DEAD_TILDE							0x83
#define VC_DEAD_MACRON							0x84
#define VC_DEAD_BREVE							0x85
#define VC_DEAD_ABOVEDOT						0x86
#define VC_DEAD_DIAERESIS						0x87
#define VC_DEAD_ABOVERING						0x88
#define VC_DEAD_DOUBLEACUTE						0x89
#define VC_DEAD_CARON							0x8A
#define VC_DEAD_CEDILLA							0x8B
#define VC_DEAD_OGONEK							0x8C
#define VC_DEAD_IOTA							0x8D
#define VC_DEAD_VOICED_SOUND					0x8E
#define VC_DEAD_SEMIVOICED_SOUND				0x8F
*/

// For input method support on Asian Keyboards.
#define VC_FINAL								0x0018	// Unknown Win32 API
#define VC_CONVERT								0x001C
#define VC_NONCONVERT							0x001D
#define VC_ACCEPT								0x001E
#define VC_MODECHANGE							0x001F	// Unknown Win32 API
#define VC_KANA									0x0015

#define VC_ALPHANUMERIC							0x00F0



#define VC_FULL_WIDTH							0x00F3
#define VC_HALF_WIDTH							0x00F4
#define VC_ROMAN_CHARACTERS						0x00F5
#define VC_ALL_CANDIDATES						0x0100
#define VC_PREVIOUS_CANDIDATE					0x0101
#define VC_CODE_INPUT							0x0102
#define VC_JAPANESE_KATAKANA					0x0103
#define VC_JAPANESE_HIRAGANA					0x0104
#define VC_JAPANESE_ROMAN						0x0105
#define VC_KANA_LOCK							0x0106
#define VC_INPUT_METHOD_ON_OFF					0x0107

// For Sun keyboards.
/*
#define VC_FIND									0xE012
#define VC_PROPS								//0xFFCA
#define VC_COMPOSE								0xFF20
#define VC_ALT_GRAPH							0xFF7E
*/

#define VC_UNDEFINED							0x0000	// KeyCode Unknown

#define CHAR_UNDEFINED							0xFFFF	// CharCode Unknown
/* End Virtual Key Codes */


/* Begin Virtual Modifier Masks */
#define MASK_SHIFT_L							1 << 0
#define MASK_CTRL_L								1 << 1
#define MASK_META_L								1 << 2
#define MASK_ALT_L								1 << 3

#define MASK_SHIFT_R							1 << 4
#define MASK_CTRL_R								1 << 5
#define MASK_META_R								1 << 6
#define MASK_ALT_R								1 << 7

#define MASK_SHIFT								MASK_SHIFT_L | MASK_SHIFT_R
#define MASK_CTRL								MASK_CTRL_L  | MASK_CTRL_R
#define MASK_META								MASK_META_L  | MASK_META_R
#define MASK_ALT								MASK_ALT_L   | MASK_ALT_R

#define MASK_BUTTON1							1 << 8
#define MASK_BUTTON2							1 << 9
#define MASK_BUTTON3							1 << 10
#define MASK_BUTTON4							1 << 11
#define MASK_BUTTON5							1 << 12
/* End Virtual Modifier Masks */


/* Begin Virtual Mouse Buttons */
#define MOUSE_NOBUTTON							0	// AnyButton
#define MOUSE_BUTTON1							1
#define MOUSE_BUTTON2							2
#define MOUSE_BUTTON3							3
#define MOUSE_BUTTON4							4	// Extra Mouse Button
#define MOUSE_BUTTON5							5	// Extra Mouse Button

#define WHEEL_UNIT_SCROLL						1
#define WHEEL_BLOCK_SCROLL						2
/* End Virtual Mouse Buttons */


#ifdef _WIN32
#define NATIVEHOOK_API __declspec(dllexport)
#else
#define NATIVEHOOK_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	// Send a virtual event back to the system.
	NATIVEHOOK_API void hook_post_event(VirtualEvent * const event);

	// Set the event callback function.
	NATIVEHOOK_API void hook_set_dispatch_proc(void (*dispatch_proc)(VirtualEvent * const));

	// Insert the event hook.
	NATIVEHOOK_API int hook_enable();

	// Withdraw the event hook.
	NATIVEHOOK_API int hook_disable();

	// Check the event hook status.
	NATIVEHOOK_API bool hook_is_enabled();

	// Retrieves the keyboard auto repeat rate.
	NATIVEHOOK_API long int hook_get_auto_repeat_rate();

	// Retrieves the keyboard auto repeat delay.
	NATIVEHOOK_API long int hook_get_auto_repeat_delay();

	// Retrieves the mouse acceleration multiplier.
	NATIVEHOOK_API long int hook_get_pointer_acceleration_multiplier();

	// Retrieves the mouse acceleration threshold.
	NATIVEHOOK_API long int hook_get_pointer_acceleration_threshold();

	// Retrieves the mouse sensitivity.
	NATIVEHOOK_API long int hook_get_pointer_sensitivity();

	// Retrieves the double/triple click interval.
	NATIVEHOOK_API long int hook_get_multi_click_time();

#ifdef __cplusplus
}
#endif

#endif
