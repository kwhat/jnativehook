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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include <nativehook.h>

const uint16_t scancode_table[] = {
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
	VC_CONTROL_L,		// 0x1D 29
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
	VC_SHIFT_R,			// 0x36 54
	VC_PRINTSCREEN,		// 0x37 55
	VC_ALT_L,			// 0x38 56
	VC_SPACE,			// 0x39 57
	VC_CAPS_LOCK,		// 0x3A 58
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
	VC_KP_SUBTRACT,		// 0x4A 74
	VC_KP_4,			// 0x4B 75
	VC_KP_5,			// 0x4C 76
	VC_KP_6,			// 0x4D 77
	VC_KP_ADD,			// 0x4E 78
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
	VC_UNDEFINED		// 0x82 128
};

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
