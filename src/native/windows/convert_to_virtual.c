/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
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

#include <config.h>
#include <nativehook.h>
#include <stdint.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#ifdef SUN_KEYBOARD
#include <X11/Sunkeysym.h>
#endif

static uint16_t scan_code_lookup[] = {
	VC_UNDEFINED,		// 0x00 00	?
	VC_ESCAPE,			// 0x01 01
	VC_1,				// 0x02 02
	VC_2,				// 0x03 03
	VC_3,				// 0x04 04
	VC_4,				// 0x05 05
	VC_5,				// 0x06 06
	VC_6,				// 0x07 07
	VC_7,				// 0x08 08
	VC_8,				// 0x09 09
	VC_9,				// 0x0A 10
	VC_0,				// 0x0B 11
	VC_MINUS,			// 0x0C 12
	VC_PLUS,			// 0x0D 13
	VC_BACK_SPACE,		// 0x0E 14
	VC_TAB,				// 0x0F 15
	VC_Q,				// 0x10 16
	VC_W,				// 0x11 17
	VC_E,				// 0x12 18
	VC_R,				// 0x13 19
	VC_T,				// 0x14 20
	VC_Y,				// 0x15 21
	VC_U,				// 0x16 22
	VC_I,				// 0x17 23
	VC_O,				// 0x18 24
	VC_P,				// 0x19 25
	VC_OPEN_BRACKET,	// 0x1A 26
	VC_CLOSE_BRACKET,	// 0x1B 27
	VC_ENTER,			// 0x1C 28
	VC_CTRL_L,			// 0x1D 29
	VC_A,				// 0x1E 30
	VC_S,				// 0x1F 31
	VC_D,				// 0x20 32
	VC_F,				// 0x21 33
	VC_G,				// 0x21 34
	VC_H,				// 0x22 35
	VC_J,				// 0x23 36
	VC_K,				// 0x24 37
	VC_L,				// 0x25 38
	VC_SEMICOLON,		// 0x26 39
	VC_QUOTE,			// 0x27 40
	VC_BACK_QUOTE,		// 0x28 40
	VC_SHIFT_L,			// 0x29 41
	VC_BACK_SLASH,		// 0x2A 42
	VC_UNDEFINED,		// 0x2B 43	INT 2, the lower key of the two to the right of the Enter key on an international keyboard. Confusingly, it, and not the one with | and \ on it in the UK, has the same scan code in some sets as the U. S. | and \ key; and on the Canadian French keyboard, it is this key that has < and > on it. In the United Kingdom, the characters on this key are ~ and #.
	VC_Z,				// 0x2C 44
	VC_X,				// 0x2D 45
	VC_C,				// 0x2E 46
	VC_V,				// 0x2F 47
	VC_B,				// 0x30 48
	VC_N,				// 0x31 49
	VC_M,				// 0x32 50
	VC_COMMA,			// 0x33 51
	VC_PERIOD,			// 0x34 52
	VC_SLASH,			// 0x35 53
	VC_SHIFT_RIGHT,		// 0x36 54
	VC_PRINTSCREEN,		// 0x37 55
	VC_ALT_L,			// 0x38 56
	VC_SPACE,			// 0x39 57
	VC_CAPSLOCK,		// 0x3A 58
	VC_F1,				// 0x3B 59
	VC_F2,				// 0x3C 60
	VC_F3,				// 0x3D 61
	VC_F4,				// 0x3E 62
	VC_F5,				// 0x3F 63
	VC_F6,				// 0x40 64
	VC_F7,				// 0x41 65
	VC_F8,				// 0x42 66
	VC_F9,				// 0x43 67
	VC_F10,				// 0x44 68
	VC_PAUSE,			// 0x45 69
	VC_SCROLL_LOCK,		// 0x46 70
	VC_KP_7,			// 0x47 71
	VC_KP_8,			// 0x48 72
	VC_KP_9,			// 0x49 73
	VC_KP_MINUS,		// 0x4A 74
	VC_KP_4,			// 0x4B 75
	VC_KP_5,			// 0x4C 76
	VC_KP_6,			// 0x4D 77
	VC_KP_PLUS,			// 0x4E 78
	VC_KP_1,			// 0x4F 79
	VC_KP_2,			// 0x50 80
	VC_KP_3,			// 0x51 81
	VC_KP_0,			// 0x52 82
	VC_KP_DECIMAL,		// 0x53 83
	VC_UNDEFINED,		// 0x54 84	SysRq?
	VC_UNDEFINED,		// 0x55 85	Undefined?
	VC_UNDEFINED,		// 0x56 86	INT 1, the key next to the left shift key on an international keyboard. On many international keyboards, it has the characters < and >, but on the United Kingdom keyboard, it has | and \.
	VC_F11,				// 0x57 87
	VC_F12,				// 0x58 88
	VC_UNDEFINED,		// 0x59 89	?
	VC_UNDEFINED,		// 0x5A 90	?
	VC_F13,				// 0x5B 91
	VC_F14,				// 0x5C 92
	VC_F15,				// 0x5D 93
	VC_UNDEFINED,		// 0x5E 94	Power?
	VC_UNDEFINED,		// 0x5F 95	Sleep?
	VC_UNDEFINED,		// 0x60 96	?
	VC_UNDEFINED,		// 0x61 97	?
	VC_UNDEFINED,		// 0x62 98	?
	VC_F16,				// 0x63 99	Wake?
	VC_F17,				// 0x64 100
	VC_F18,				// 0x65 101
	VC_F19,				// 0x66 102
	VC_F20,				// 0x67 103
	VC_F21,				// 0x68 104
	VC_F22,				// 0x69 105
	VC_F23,				// 0x6A 106
	VC_F24,				// 0x6B 107
	VC_UNDEFINED,		// 0x6C 108	Ctrl?
	VC_UNDEFINED,		// 0x6D 109	ErEOF Record or Video?
	VC_UNDEFINED,		// 0x6E 110
	VC_UNDEFINED,		// 0x6F 111	Copy?
	VC_KATAKANA,		// 0x70 111	katakana
	VC_UNDEFINED,		// 0x71 112	Attn?
	VC_UNDEFINED,		// 0x72 113	CrSel?
	VC_UNDEFINED,		// 0x73 114	INT 3, found both on some Brazilian keyboards and on the Japanese keyboard used for a Kanji input method (it has hiragana on the keys, as opposed to the other form of a plain Japanese keyboard with katakana on the keys). On the Japanese keyboard, it has _ and \ on it, and is located next to the right shift key.
	VC_UNDEFINED,		// 0x74 115	ExSel?
	VC_UNDEFINED,		// 0x75 115	Enl?
	VC_CLEAR,			// 0x76 116
	VC_UNDEFINED,		// 0x77 117	Furigana?
	VC_UNDEFINED,		// 0x78 118
	VC_KANJI,			// 0x79 119
	VC_UNDEFINED,		// 0x7A 120
	VC_HIRAGANA,		// 0x7B 121
	VC_UNDEFINED,		// 0x7C 122
	VC_UNDEFINED,		// 0x7D 123	INT 4, found on the Japanese keyboard. It has | and ¥ on it and is located next to the backspace key.
	VC_UNDEFINED,		// 0x7E 124 INT 5, located on the numeric keypad on some Brazilian keyboards. It has the same Group 3 scan code as the extra key on the numeric keypad of 122-key keyboards when the code assignments used for keyboards used with IBM terminals are followed.
	VC_UNDEFINED,		// 0x7F 125
	VC_UNDEFINED,		// 0x80 126
	VC_UNDEFINED,		// 0x81 127
	VC_UNDEFINED,		// 0x82 128
	VC_UNDEFINED,		// 0x83 129
	VC_UNDEFINED,		// 0x84 130
	VC_UNDEFINED,		// 0x85 131
	VC_UNDEFINED		// 0x86 132
}

// Dupes
// Flag 0x0		Flag 0x1
// VC_NUMLOCK	VC_PAUSE	// 0x45 69
// VC_CTRL_L	VC_CTRL_R	// 0x1D 29
// VC_F13		VC_WIN_L	// 0x5B 91
// VC_F14		VC_WIN_R	// 0x5C 92
// VC_F15		VC_MENU		// 0x5D 93
//

// Number Lock Solution
// GetKeyState(VK_NUMLOCK) & 0x01

unsigned int convert_to_virtual_key(unsigned int native_keycode) {
	unsigned int virtual_keycode;

	switch (native_keycode) {
		case XK_Return:
			virtual_keycode = VC_ENTER;
			break;
		case XK_BackSpace:
			virtual_keycode = VC_BACK_SPACE;
			break;
		case XK_Tab:
			virtual_keycode = VC_TAB;
			break;
		#ifndef SUN_KEYBOARD
		case XK_Cancel:
			virtual_keycode = VC_CANCEL;
			break;
		#endif


		case XK_Shift_L:
			virtual_keycode = VC_SHIFT_L;
			break;
		case XK_Shift_R:
			virtual_keycode = VC_SHIFT_R;
			break;
		case XK_Control_L:
			virtual_keycode = VC_CONTROL_L;
		case XK_Control_R:
			virtual_keycode = VC_CONTROL_R;
		case XK_Alt_L:
			virtual_keycode = VC_ALT_L;
			break;
		case XK_Alt_R:
			virtual_keycode = VC_ALT_R;
			break;
		case XK_Meta_L:
			virtual_keycode = VC_META_L;
			break;
		case XK_Meta_R:
			virtual_keycode = VC_META_R;
			break;
		case XK_Super_L:
			virtual_keycode = VC_SUPER_L;
			break;
		case XK_Super_R:
			virtual_keycode = VC_SUPER_R;
			break;


		case XK_Menu:
			virtual_keycode = VC_CONTEXT_MENU;
			break;


		case XK_Pause:
			virtual_keycode = VC_PAUSE;
			break;
		case XK_Caps_Lock:
			virtual_keycode = VC_CAPS_LOCK;
			break;
		case XK_Escape:
			virtual_keycode = VC_ESCAPE;
			break;
		case XK_space:
			virtual_keycode = VC_SPACE;
			break;


		case XK_Up:
			virtual_keycode = VC_UP;
			break;
		case XK_Down:
			virtual_keycode = VC_DOWN;
			break;
		case XK_Left:
			virtual_keycode = VC_LEFT;
			break;
		case XK_Right:
			virtual_keycode = VC_RIGHT;
			break;


		case XK_comma:
			virtual_keycode = VC_COMMA;
			break;
		case XK_minus:
			virtual_keycode = VC_MINUS;
			break;
		case XK_period:
			virtual_keycode = VC_PERIOD;
			break;
		case XK_slash:
			virtual_keycode = VC_SLASH;
			break;


		case XK_0:
			virtual_keycode = VC_0;
			break;
		case XK_1:
			virtual_keycode = VC_1;
			break;
		case XK_2:
			virtual_keycode = VC_2;
			break;
		case XK_3:
			virtual_keycode = VC_3;
			break;
		case XK_4:
			virtual_keycode = VC_4;
			break;
		case XK_5:
			virtual_keycode = VC_5;
			break;
		case XK_6:
			virtual_keycode = VC_6;
			break;
		case XK_7:
			virtual_keycode = VC_7;
			break;
		case XK_8:
			virtual_keycode = VC_8;
			break;
		case XK_9:
			virtual_keycode = VC_9;
			break;


		case XK_semicolon:
			virtual_keycode = VC_SEMICOLON;
			break;
		case XK_equal:
			virtual_keycode = VC_EQUALS;
			break;

		case XK_a:
		case XK_A:
			virtual_keycode = VC_A;
			break;
		case XK_b:
		case XK_B:
			virtual_keycode = VC_B;
			break;
		case XK_c:
		case XK_C:
			virtual_keycode = VC_C;
			break;
		case XK_d:
		case XK_D:
			virtual_keycode = VC_D;
			break;
		case XK_e:
		case XK_E:
			virtual_keycode = VC_E;
			break;
		case XK_f:
		case XK_F:
			virtual_keycode = VC_F;
			break;
		case XK_g:
		case XK_G:
			virtual_keycode = VC_G;
			break;
		case XK_h:
		case XK_H:
			virtual_keycode = VC_H;
			break;
		case XK_i:
		case XK_I:
			virtual_keycode = VC_I;
			break;
		case XK_j:
		case XK_J:
			virtual_keycode = VC_J;
			break;
		case XK_k:
		case XK_K:
			virtual_keycode = VC_K;
			break;
		case XK_l:
		case XK_L:
			virtual_keycode = VC_L;
			break;
		case XK_m:
		case XK_M:
			virtual_keycode = VC_M;
			break;
		case XK_n:
		case XK_N:
			virtual_keycode = VC_N;
			break;
		case XK_o:
		case XK_O:
			virtual_keycode = VC_O;
			break;
		case XK_p:
		case XK_P:
			virtual_keycode = VC_P;
			break;
		case XK_q:
		case XK_Q:
			virtual_keycode = VC_Q;
			break;
		case XK_r:
		case XK_R:
			virtual_keycode = VC_R;
			break;
		case XK_s:
		case XK_S:
			virtual_keycode = VC_S;
			break;
		case XK_t:
		case XK_T:
			virtual_keycode = VC_T;
			break;
		case XK_u:
		case XK_U:
			virtual_keycode = VC_U;
			break;
		case XK_v:
		case XK_V:
			virtual_keycode = VC_V;
			break;
		case XK_w:
		case XK_W:
			virtual_keycode = VC_W;
			break;
		case XK_x:
		case XK_X:
			virtual_keycode = VC_X;
			break;
		case XK_y:
		case XK_Y:
			virtual_keycode = VC_Y;
			break;
		case XK_z:
		case XK_Z:
			virtual_keycode = VC_Z;
			break;

		case XK_bracketleft:
			virtual_keycode = VC_OPEN_BRACKET;
			break;
		case XK_backslash:
			virtual_keycode = VC_BACK_SLASH;
			break;
		case XK_bracketright:
			virtual_keycode = VC_CLOSE_BRACKET;
			break;

		case XK_KP_0:
		case XK_KP_Insert:
			virtual_keycode = VC_KP_0;
			break;
		case XK_KP_1:
		case XK_KP_End:
			virtual_keycode = VC_KP_1;
			break;
		case XK_KP_2:
		case XK_KP_Down:
			virtual_keycode = VC_KP_2;
			break;
		case XK_KP_3:
		case XK_KP_Page_Down:
			virtual_keycode = VC_KP_3;
			break;
		case XK_KP_4:
		case XK_KP_Left:
			virtual_keycode = VC_KP_4;
			break;
		case XK_KP_5:
		case XK_KP_Begin:
			virtual_keycode = VC_KP_5;
			break;
		case XK_KP_6:
		case XK_KP_Right:
			virtual_keycode = VC_KP_6;
			break;
		case XK_KP_7:
		case XK_KP_Home:
			virtual_keycode = VC_KP_7;
			break;
		case XK_KP_8:
		case XK_KP_Up:
			virtual_keycode = VC_KP_8;
			break;
		case XK_KP_9:
		case XK_KP_Page_Up:
			virtual_keycode = VC_KP_9;
			break;

		case XK_KP_Multiply:
			virtual_keycode = VC_KP_MULTIPLY;
			break;
		case XK_KP_Add:
			virtual_keycode = VC_KP_ADD;
			break;
		case XK_KP_Separator:
			virtual_keycode = VC_KP_SEPARATOR;
			break;
		case XK_KP_Subtract:
			virtual_keycode = VC_KP_SUBTRACT;
			break;
		case XK_KP_Enter:
			virtual_keycode = VC_KP_ENTER;
			break;
		case XK_KP_Decimal:
			virtual_keycode = VC_KP_DECIMAL;
			break;
		case XK_KP_Divide:
			virtual_keycode = VC_KP_DIVIDE;
			break;
		case XK_KP_Delete:
			virtual_keycode = VC_KP_DELETE;
			break;
		case XK_Num_Lock:
			virtual_keycode = VC_NUM_LOCK;
			break;
		case XK_Clear:
			virtual_keycode = VC_CLEAR;
			break;

		case XK_Scroll_Lock:
			virtual_keycode = VC_SCROLL_LOCK;
			break;

		// Function Keys
		case XK_F1:
			virtual_keycode = VC_F1;
			break;
		case XK_F2:
			virtual_keycode = VC_F2;
			break;
		case XK_F3:
			virtual_keycode = VC_F3;
			break;
		case XK_F4:
			virtual_keycode = VC_F4;
			break;
		case XK_F5:
			virtual_keycode = VC_F5;
			break;
		case XK_F6:
			virtual_keycode = VC_F6;
			break;
		case XK_F7:
			virtual_keycode = VC_F7;
			break;
		case XK_F8:
			virtual_keycode = VC_F8;
			break;
		case XK_F9:
			virtual_keycode = VC_F9;
			break;
		case XK_F10:
			virtual_keycode = VC_F10;
			break;
		case XK_F11:
			virtual_keycode = VC_F11;
			break;
		case XK_F12:
			virtual_keycode = VC_F12;
			break;

		// Extended Function Keys
		case XK_F13:
			virtual_keycode = VC_F13;
			break;
		case XK_F14:
			virtual_keycode = VC_F14;
			break;
		case XK_F15:
			virtual_keycode = VC_F15;
			break;
		case XK_F16:
			virtual_keycode = VC_F16;
			break;
		case XK_F17:
			virtual_keycode = VC_F17;
			break;
		case XK_F18:
			virtual_keycode = VC_F18;
			break;
		case XK_F19:
			virtual_keycode = VC_F19;
			break;
		case XK_F20:
			virtual_keycode = VC_F20;
			break;
		case XK_F21:
			virtual_keycode = VC_F21;
			break;
		case XK_F22:
			virtual_keycode = VC_F22;
			break;
		case XK_F23:
			virtual_keycode = VC_F23;
			break;
		case XK_F24:
			virtual_keycode = VC_F24;
			break;

		case XK_Print:
			virtual_keycode = VC_PRINTSCREEN;
			break;
		case XK_Insert:
			virtual_keycode = VC_INSERT;
			break;
		case XK_Delete:
			virtual_keycode = VC_DELETE;
			break;
		case XK_Help:
			virtual_keycode = VC_HELP;
			break;

		case XK_Page_Up:
			virtual_keycode = VC_PAGE_UP;
			break;
		case XK_Page_Down:
			virtual_keycode = VC_PAGE_DOWN;
			break;
		case XK_Home:
			virtual_keycode = VC_HOME;
			break;
		case XK_End:
			virtual_keycode = VC_END;
			break;

		case XK_apostrophe:
			virtual_keycode = VC_QUOTE;
			break;
		case XK_grave:
			virtual_keycode = VC_BACK_QUOTE;
			break;

		// For European keyboards
		case XK_dead_grave:
			virtual_keycode = VC_DEAD_GRAVE;
			break;
		case XK_dead_acute:
			virtual_keycode = VC_DEAD_ACUTE;
			break;
		case XK_dead_circumflex:
			virtual_keycode = VC_DEAD_CIRCUMFLEX;
			break;
		case XK_dead_tilde:
			virtual_keycode = VC_DEAD_TILDE;
			break;
		case XK_dead_macron:
			virtual_keycode = VC_DEAD_MACRON;
			break;
		case XK_dead_breve:
			virtual_keycode = VC_DEAD_BREVE;
			break;
		case XK_dead_abovedot:
			virtual_keycode = VC_DEAD_ABOVEDOT;
			break;
		case XK_dead_diaeresis:
			virtual_keycode = VC_DEAD_DIAERESIS;
			break;
		case XK_dead_abovering:
			virtual_keycode = VC_DEAD_ABOVERING;
			break;
		case XK_dead_doubleacute:
			virtual_keycode = VC_DEAD_DOUBLEACUTE;
			break;
		case XK_dead_caron:
			virtual_keycode = VC_DEAD_CARON;
			break;
		case XK_dead_cedilla:
			virtual_keycode = VC_DEAD_CEDILLA;
			break;
		case XK_dead_ogonek:
			virtual_keycode = VC_DEAD_OGONEK;
			break;
		case XK_dead_iota:
			virtual_keycode = VC_DEAD_IOTA;
			break;
		case XK_dead_voiced_sound:
			virtual_keycode = VC_DEAD_VOICED_SOUND;
			break;
		case XK_dead_semivoiced_sound:
			virtual_keycode = VC_DEAD_SEMIVOICED_SOUND;
			break;

		// Unknown Keyboard Codes
		case XK_ampersand:
			virtual_keycode = VC_AMPERSAND;
			break;
		case XK_asterisk:
			virtual_keycode = VC_ASTERISK;
			break;
		case XK_quotedbl:
			virtual_keycode = VC_QUOTEDBL;
			break;
		case XK_less:
			virtual_keycode = VC_LESS;
			break;
		case XK_greater:
			virtual_keycode = VC_GREATER;
			break;
		case XK_braceleft:
			virtual_keycode = VC_BRACELEFT;
			break;
		case XK_braceright:
			virtual_keycode = VC_BRACERIGHT;
			break;

		// Unknown Extended Keyboard Codes
		case XK_at:
			virtual_keycode = VC_AT;
			break;
		case XK_colon:
			virtual_keycode = VC_COLON;
			break;
		case XK_asciicircum:
			virtual_keycode = VC_CIRCUMFLEX;
			break;
		case XK_dollar:
			virtual_keycode = VC_DOLLAR;
			break;
		case XK_EuroSign:
			virtual_keycode = VC_EURO_SIGN;
			break;
		case XK_exclam:
			virtual_keycode = VC_EXCLAMATION_MARK;
			break;
		case XK_exclamdown:
			virtual_keycode = VC_INVERTED_EXCLAMATION_MARK;
			break;
		case XK_parenleft:
			virtual_keycode = VC_LEFT_PARENTHESIS;
			break;
		case XK_numbersign:
			virtual_keycode = VC_NUMBER_SIGN;
			break;
		case XK_plus:
			virtual_keycode = VC_PLUS;
			break;
		case XK_parenright:
			virtual_keycode = VC_RIGHT_PARENTHESIS;
			break;
		case XK_underscore:
			virtual_keycode = VC_UNDERSCORE;
			break;


		// For input method support on Asian Keyboards.
		/*
		case XK_Cancel:					// Unknown X11 Support
			keykeycode = VC_FINAL;
			return keyevent;
		case XK_Henkan:					// Unknown X11 Support
			keykeycode = VC_CONVERT;
			return keyevent;
		case XK_Muhenkan:				// Unknown X11 Support
			keykeycode = VC_NONCONVERT;
			return keyevent;
		case XK_VoidSymbol:				// Unknown X11 Support
			keykeycode = VC_ACCEPT;
			return keyevent;
		*/
		#ifndef SUN_KEYBOARD
		case XK_Mode_switch:			// TODO Testing Needed.
			virtual_keycode = VC_MODECHANGE;
			break;
		#endif
		case XK_Kana_Shift:				// TODO Testing Needed.
			virtual_keycode = VC_KANA;
			break;
		case XK_Kanji:					// TODO Testing Needed.
			virtual_keycode = VC_KANJI;
			break;
		case XK_Eisu_Shift:				// TODO Testing Needed.
			virtual_keycode = VC_ALPHANUMERIC;
			break;
		case XK_Katakana:				// TODO Testing Needed.
			virtual_keycode = VC_KATAKANA;
			break;
		case XK_Hiragana:				// TODO Testing Needed.
			virtual_keycode = VC_HIRAGANA;
			break;
		case XK_Zenkaku:				// TODO Testing Needed.
			virtual_keycode = VC_FULL_WIDTH;
			break;
		case XK_Hankaku:				// TODO Testing Needed.
			virtual_keycode = VC_HALF_WIDTH;
			break;
		case XK_Romaji:					// TODO Testing Needed.
			virtual_keycode = VC_ROMAN_CHARACTERS;
			break;
		case XK_MultipleCandidate:		// TODO Testing Needed.
			virtual_keycode = VC_ALL_CANDIDATES;
			break;
		case XK_PreviousCandidate:		// TODO Testing Needed.
			virtual_keycode = VC_PREVIOUS_CANDIDATE;
			break;
		case XK_Codeinput:				// TODO Testing Needed.
			virtual_keycode = VC_CODE_INPUT;
			break;
		case XK_Hiragana_Katakana:		// TODO Testing Needed.
			virtual_keycode = VC_JAPANESE_KATAKANA;
			break;
		case XK_Zenkaku_Hankaku:		// TODO Testing Needed.
			virtual_keycode = VC_JAPANESE_HIRAGANA;
			break;
		/* case XK_Romaji:				// TODO Testing Needed
			keykeycode = VC_JAPANESE_ROMAN;
			return keyevent; */
		case XK_Kana_Lock:				// TODO Testing Needed.
			virtual_keycode = VC_KANA_LOCK;
			break;
		case XK_Eisu_toggle:			// TODO Testing Needed.
			virtual_keycode = VC_INPUT_METHOD_ON_OFF;
			break;

		// For Sun keyboards.
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:
			virtual_keycode = VC_CUT;
			break;
		case SunXK_Copy:
			virtual_keycode = VC_COPY;
			break;
		case SunXK_Paste:
			virtual_keycode = VC_PASTE;
			break;
		case SunXK_Undo:
			virtual_keycode = VC_UNDO;
			break;
		case SunXK_Again:
			virtual_keycode = VC_AGAIN;
			break;
		case SunXK_Find:
			virtual_keycode = VC_FIND;
			break;
		case SunXK_Props:
			virtual_keycode = VC_PROPS;
			break;
		case SunXK_Stop:
			virtual_keycode = VC_STOP;
			break;
		case SunXK_Compose:
			virtual_keycode = VC_COMPOSE;
			break;
		case SunXK_AltGraph:
			virtual_keycode = VC_ALT_GRAPH;
			break;
		#endif

		case XK_Begin:
			virtual_keycode = VC_BEGIN;
			break;

		default:
		case XK_VoidSymbol:
			virtual_keycode = VC_UNDEFINED;
			break;
	}

	return virtual_keycode;
}

unsigned int convert_to_virtual_button(unsigned int native_button) {
	unsigned char virtual_button;

	switch (native_button) {
		case Button1:
			virtual_button = MOUSE_BUTTON1;
			break;
		case Button2:
			virtual_button = MOUSE_BUTTON2;
			break;
		case Button3:
			virtual_button = MOUSE_BUTTON3;
			break;
		case Button4:
			virtual_button = MOUSE_BUTTON4;
			break;
		case Button5:
			virtual_button = MOUSE_BUTTON5;
			break;

		default:
		case AnyButton:
			virtual_button = MOUSE_NOBUTTON;
			break;
	}

	return virtual_button;
}

unsigned int convert_to_virtual_mask(unsigned int native_mask) {
	unsigned int virtual_mask = 0x0000;

	if (native_mask & ShiftMask)	virtual_mask |= MASK_SHIFT_L;
	if (native_mask & ControlMask)	virtual_mask |= MASK_CTRL_L;
	if (native_mask & Mod4Mask)		virtual_mask |= MASK_META_L;
	if (native_mask & Mod1Mask)		virtual_mask |= MASK_ALT_L;

	if (native_mask & Button1Mask)	virtual_mask |= MASK_BUTTON1;
	if (native_mask & Button2Mask)	virtual_mask |= MASK_BUTTON2;
	if (native_mask & Button3Mask)	virtual_mask |= MASK_BUTTON3;
	if (native_mask & Button4Mask)	virtual_mask |= MASK_BUTTON4;
	if (native_mask & Button5Mask)	virtual_mask |= MASK_BUTTON5;

	return virtual_mask;
}
