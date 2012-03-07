/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
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

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>

#include "JConvertFromNative.h"
#include "WinKeyCodes.h"

JKeyDatum NativeToJKey(unsigned int keysym) {
	JKeyDatum jkey;
	jkey.rawcode = keysym;
	jkey.location = JK_LOCATION_STANDARD;

	switch (keysym) {
		case VK_RETURN:									jkey.keycode = JK_ENTER; 						return jkey;
		case VK_BACK:									jkey.keycode = JK_BACK_SPACE; 					return jkey;
		case VK_TAB:									jkey.keycode = JK_TAB; 							return jkey;
		case VK_CANCEL:									jkey.keycode = JK_CANCEL;						return jkey;

		case VK_LSHIFT:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_SHIFT; 						return jkey;
		case VK_RSHIFT:
			jkey.location = JK_LOCATION_RIGHT; 			jkey.keycode = JK_SHIFT; 						return jkey;
		case VK_LCONTROL:
			jkey.location = JK_LOCATION_LEFT; 			jkey.keycode = JK_CONTROL;						return jkey;
		case VK_RCONTROL:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_CONTROL;						return jkey;
		case VK_LMENU:
			jkey.location = JK_LOCATION_LEFT; 			jkey.keycode = JK_ALT;							return jkey;
		case VK_RMENU:
			jkey.location = JK_LOCATION_RIGHT; 			jkey.keycode = JK_ALT; 							return jkey;
		case VK_LWIN:
			jkey.location = JK_LOCATION_LEFT; 			jkey.keycode = JK_WINDOWS; 						return jkey;
		case VK_RWIN:
			jkey.location = JK_LOCATION_RIGHT; 			jkey.keycode = JK_WINDOWS;						return jkey;
		case VK_APPS:									jkey.keycode = JK_CONTEXT_MENU; 				return jkey;

		case VK_PAUSE:									jkey.keycode = JK_PAUSE; 						return jkey;
		case VK_CAPITAL:								jkey.keycode = JK_CAPS_LOCK; 					return jkey;
		case VK_ESCAPE:									jkey.keycode = JK_ESCAPE; 						return jkey;
		case VK_SPACE:									jkey.keycode = JK_SPACE; 						return jkey;

		case VK_UP:										jkey.keycode = JK_UP; 							return jkey;
		case VK_DOWN:									jkey.keycode = JK_DOWN; 						return jkey;
		case VK_LEFT:									jkey.keycode = JK_LEFT; 						return jkey;
		case VK_RIGHT:									jkey.keycode = JK_RIGHT; 						return jkey;

		case VK_OEM_COMMA:								jkey.keycode = JK_COMMA; 						return jkey;
		case VK_OEM_MINUS:								jkey.keycode = JK_MINUS; 						return jkey;
		case VK_OEM_PERIOD:								jkey.keycode = JK_PERIOD; 						return jkey;
		case VK_OEM_2:									jkey.keycode = JK_SLASH; 						return jkey;	//TODO Testing Needed

		case 0x30:										jkey.keycode = JK_0; 							return jkey;
		case 0x31:										jkey.keycode = JK_1; 							return jkey;
		case 0x32:										jkey.keycode = JK_2; 							return jkey;
		case 0x33:										jkey.keycode = JK_3; 							return jkey;
		case 0x34:										jkey.keycode = JK_4; 							return jkey;
		case 0x35:										jkey.keycode = JK_5; 							return jkey;
		case 0x36:										jkey.keycode = JK_6; 							return jkey;
		case 0x37:										jkey.keycode = JK_7; 							return jkey;
		case 0x38:										jkey.keycode = JK_8; 							return jkey;
		case 0x39:										jkey.keycode = JK_9; 							return jkey;

		case VK_OEM_1:									jkey.keycode = JK_SEMICOLON; 					return jkey;	//TODO Testing Needed
		case VK_OEM_PLUS:								jkey.keycode = JK_EQUALS;						return jkey;	//TODO Testing Needed

		case 0x41:										jkey.keycode = JK_A; 							return jkey;
		case 0x42:										jkey.keycode = JK_B; 							return jkey;
		case 0x43:										jkey.keycode = JK_C; 							return jkey;
		case 0x44:										jkey.keycode = JK_D; 							return jkey;
		case 0x45:										jkey.keycode = JK_E; 							return jkey;
		case 0x46:										jkey.keycode = JK_F; 							return jkey;
		case 0x47:										jkey.keycode = JK_G; 							return jkey;
		case 0x48:										jkey.keycode = JK_H; 							return jkey;
		case 0x49:										jkey.keycode = JK_I; 							return jkey;
		case 0x4A:										jkey.keycode = JK_J; 							return jkey;
		case 0x4B:										jkey.keycode = JK_K; 							return jkey;
		case 0x4C:										jkey.keycode = JK_L; 							return jkey;
		case 0x4D:										jkey.keycode = JK_M; 							return jkey;
		case 0x4E:										jkey.keycode = JK_N; 							return jkey;
		case 0x4F:										jkey.keycode = JK_O; 							return jkey;
		case 0x50:										jkey.keycode = JK_P; 							return jkey;
		case 0x51:										jkey.keycode = JK_Q; 							return jkey;
		case 0x52:										jkey.keycode = JK_R; 							return jkey;
		case 0x53:										jkey.keycode = JK_S; 							return jkey;
		case 0x54:										jkey.keycode = JK_T; 							return jkey;
		case 0x55:										jkey.keycode = JK_U; 							return jkey;
		case 0x56:										jkey.keycode = JK_V; 							return jkey;
		case 0x57:										jkey.keycode = JK_W; 							return jkey;
		case 0x58:										jkey.keycode = JK_X; 							return jkey;
		case 0x59:										jkey.keycode = JK_Y; 							return jkey;
		case 0x5A:										jkey.keycode = JK_Z; 							return jkey;

		case VK_OEM_4:									jkey.keycode = JK_OPEN_BRACKET; 				return jkey;	//TODO Testing Needed
		case VK_OEM_5:									jkey.keycode = JK_BACK_SLASH; 					return jkey;	//TODO Testing Needed
		case VK_OEM_6:									jkey.keycode = JK_CLOSE_BRACKET;				return jkey;	//TODO Testing Needed

		case  VK_NUMPAD0:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD0;						return jkey;
		case  VK_NUMPAD1:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD1;						return jkey;
		case  VK_NUMPAD2:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD2;						return jkey;
		case  VK_NUMPAD3:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD3;						return jkey;
		case  VK_NUMPAD4:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD4;						return jkey;
		case  VK_NUMPAD5:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD5;						return jkey;
		case  VK_NUMPAD6:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD6;						return jkey;
		case  VK_NUMPAD7:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD7;						return jkey;
		case  VK_NUMPAD8:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD8;						return jkey;
		case  VK_NUMPAD9:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD9;						return jkey;

		case VK_MULTIPLY:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_MULTIPLY; 					return jkey;
		case VK_ADD:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_ADD; 							return jkey;
		//case VK_SEPARATOR:
		//	jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_SEPARATOR; 					return jkey;
		case VK_SUBTRACT:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_SUBTRACT; 					return jkey;
		//case XK_KP_Enter:
		//	jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_ENTER;						return jkey;
		case VK_DECIMAL:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_DECIMAL; 						return jkey;
		case VK_DIVIDE:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_DIVIDE; 						return jkey;
		//case VK_DELETE:
		//	jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_DELETE; 						return jkey;
		case VK_NUMLOCK:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUM_LOCK; 					return jkey;
		case VK_SCROLL:									jkey.keycode = JK_SCROLL_LOCK; 					return jkey;

		case VK_F1:										jkey.keycode = JK_F1; 							return jkey;
		case VK_F2:										jkey.keycode = JK_F2; 							return jkey;
		case VK_F3:										jkey.keycode = JK_F3; 							return jkey;
		case VK_F4:										jkey.keycode = JK_F4; 							return jkey;
		case VK_F5:										jkey.keycode = JK_F5; 							return jkey;
		case VK_F6:										jkey.keycode = JK_F6; 							return jkey;
		case VK_F7:										jkey.keycode = JK_F7; 							return jkey;
		case VK_F8:										jkey.keycode = JK_F8; 							return jkey;
		case VK_F9:										jkey.keycode = JK_F9; 							return jkey;
		case VK_F10:									jkey.keycode = JK_F10; 							return jkey;
		case VK_F11:									jkey.keycode = JK_F11; 							return jkey;
		case VK_F12:									jkey.keycode = JK_F12; 							return jkey;

		case VK_F13:									jkey.keycode = JK_F13; 							return jkey;
		case VK_F14:									jkey.keycode = JK_F14; 							return jkey;
		case VK_F15:									jkey.keycode = JK_F15; 							return jkey;
		case VK_F16:									jkey.keycode = JK_F16; 							return jkey;
		case VK_F17:									jkey.keycode = JK_F17; 							return jkey;
		case VK_F18:									jkey.keycode = JK_F18; 							return jkey;
		case VK_F19:									jkey.keycode = JK_F19; 							return jkey;
		case VK_F20:									jkey.keycode = JK_F20; 							return jkey;
		case VK_F21:									jkey.keycode = JK_F21; 							return jkey;
		case VK_F22:									jkey.keycode = JK_F22; 							return jkey;
		case VK_F23:									jkey.keycode = JK_F23; 							return jkey;
		case VK_F24:									jkey.keycode = JK_F24; 							return jkey;

		case VK_SNAPSHOT:								jkey.keycode = JK_PRINTSCREEN; 					return jkey;
		case VK_INSERT:									jkey.keycode = JK_INSERT; 						return jkey;
		//case VK_HELP:									jkey.keycode = JK_HELP; 						return jkey;
		case VK_DELETE:									jkey.keycode = JK_DELETE; 						return jkey;

		case VK_PRIOR:									jkey.keycode = JK_PAGE_UP;						return jkey;
		case VK_NEXT:									jkey.keycode = JK_PAGE_DOWN; 					return jkey;
		case VK_HOME:									jkey.keycode = JK_HOME; 						return jkey;
		case VK_END:									jkey.keycode = JK_END; 							return jkey;

		case VK_OEM_7:									jkey.keycode = JK_QUOTE; 						return jkey;	//TODO Testing Needed
		case VK_OEM_3:									jkey.keycode = JK_BACK_QUOTE; 					return jkey;	//TODO Testing Needed

		/* For European keyboards */
		/*
		case XK_dead_grave:								jkey.keycode = JK_DEAD_GRAVE;					return jkey;	//Unknown Microsoft Support
		case XK_dead_acute:								jkey.keycode = JK_DEAD_ACUTE;					return jkey;	//Unknown Microsoft Support
		case XK_dead_circumflex:						jkey.keycode = JK_DEAD_CIRCUMFLEX;				return jkey;	//Unknown Microsoft Support
		case XK_dead_tilde:								jkey.keycode = JK_DEAD_TILDE;					return jkey;	//Unknown Microsoft Support
		case XK_dead_macron:							jkey.keycode = JK_DEAD_MACRON;					return jkey;	//Unknown Microsoft Support
		case XK_dead_breve:								jkey.keycode = JK_DEAD_BREVE;					return jkey;	//Unknown Microsoft Support
		case XK_dead_abovedot:							jkey.keycode = JK_DEAD_ABOVEDOT;				return jkey;	//Unknown Microsoft Support
		case XK_dead_diaeresis:							jkey.keycode = JK_DEAD_DIAERESIS;				return jkey;	//Unknown Microsoft Support
		case XK_dead_abovering:							jkey.keycode = JK_DEAD_ABOVERING;				return jkey;	//Unknown Microsoft Support
		case XK_dead_doubleacute:						jkey.keycode = JK_DEAD_DOUBLEACUTE;				return jkey;	//Unknown Microsoft Support
		case XK_dead_caron:								jkey.keycode = JK_DEAD_CARON;					return jkey;	//Unknown Microsoft Support
		case XK_dead_cedilla:							jkey.keycode = JK_DEAD_CEDILLA;					return jkey;	//Unknown Microsoft Support
		case XK_dead_ogonek:							jkey.keycode = JK_DEAD_OGONEK;					return jkey;	//Unknown Microsoft Support
		case XK_dead_iota:								jkey.keycode = JK_DEAD_IOTA;					return jkey;	//Unknown Microsoft Support
		case XK_dead_voiced_sound:						jkey.keycode = JK_DEAD_VOICED_SOUND;			return jkey;	//Unknown Microsoft Support
		case XK_dead_semivoiced_sound:					jkey.keycode = JK_DEAD_SEMIVOICED_SOUND;		return jkey;	//Unknown Microsoft Support
		*/

		/* Unknown Keyboard Codes */
		/*
		case XK_ampersand:								jkey.keycode = JK_AMPERSAND;					return jkey;	//No Microsoft Support
		case XK_asterisk:								jkey.keycode = JK_ASTERISK;						return jkey;	//No Microsoft Support
		case XK_quotedbl:								jkey.keycode = JK_QUOTEDBL;						return jkey;	//No Microsoft Support
		case XK_less:									jkey.keycode = JK_LESS;							return jkey;	//No Microsoft Support
		case XK_greater:								jkey.keycode = JK_GREATER;						return jkey;	//No Microsoft Support
		case XK_braceleft:								jkey.keycode = JK_BRACELEFT;					return jkey;	//No Microsoft Support
		case XK_braceright:								jkey.keycode = JK_BRACERIGHT;					return jkey;	//No Microsoft Support
		*/

		/* Unknown Extended Keyboard Codes */
		/*
		case XK_at:										jkey.keycode = JK_AT;							return jkey;	//Unknown Microsoft Support
		case VK_OEM_1:									jkey.keycode = JK_COLON;						return jkey;	//TODO Testing Needed
		case XK_asciicircum:							jkey.keycode = JK_CIRCUMFLEX;					return jkey;	//Unknown Microsoft Support
		case XK_dollar:									jkey.keycode = JK_DOLLAR;						return jkey;	//Unknown Microsoft Support
		case XK_EuroSign:								jkey.keycode = JK_EURO_SIGN;					return jkey;	//Unknown Microsoft Support
		case XK_exclam:									jkey.keycode = JK_EXCLAMATION_MARK;				return jkey;	//Unknown Microsoft Support
		case XK_exclamdown:								jkey.keycode = JK_INVERTED_EXCLAMATION_MARK;	return jkey;	//Unknown Microsoft Support
		case XK_parenleft:								jkey.keycode = JK_LEFT_PARENTHESIS;				return jkey;	//Unknown Microsoft Support
		case XK_numbersign:								jkey.keycode = JK_NUMBER_SIGN;					return jkey;	//Unknown Microsoft Support
		case VK_OEM_PLUS:								jkey.keycode = JK_PLUS;							return jkey;	//TODO Testing Needed
		case XK_parenright:								jkey.keycode = JK_RIGHT_PARENTHESIS;			return jkey;	//Unknown Microsoft Support
		case XK_underscore:								jkey.keycode = JK_UNDERSCORE;					return jkey;
		*/

		/* For input method support on Asian Keyboards */
		//case XK_Cancel:								jkey.keycode = JK_FINAL;						return jkey;	//Unknown Microsoft Support
		//case XK_Henkan:								jkey.keycode = JK_CONVERT;						return jkey;	//Unknown Microsoft Support
		//case XK_Muhenkan:								jkey.keycode = JK_NONCONVERT;					return jkey;	//Unknown Microsoft Support
		//case XK_VoidSymbol:							jkey.keycode = JK_ACCEPT;						return jkey;	//Unknown Microsoft Support
		//case XK_Mode_switch:							jkey.keycode = JK_MODECHANGE;					return jkey;	//Unknown Microsoft Support
		case VK_KANA:									jkey.keycode = JK_KANA;							return jkey;	//TODO Testing Needed
		case VK_KANJI:									jkey.keycode = JK_KANJI;						return jkey;	//TODO Testing Needed
		case VK_DBE_ALPHANUMERIC:						jkey.keycode = JK_ALPHANUMERIC;					return jkey;	//TODO Testing Needed
		case VK_DBE_KATAKANA:							jkey.keycode = JK_KATAKANA;						return jkey;	//TODO Testing Needed
		case VK_DBE_HIRAGANA:							jkey.keycode = JK_HIRAGANA;						return jkey;	//TODO Testing Needed
		case VK_DBE_DBCSCHAR:							jkey.keycode = JK_FULL_WIDTH;					return jkey;	//TODO Testing Needed
		case VK_DBE_SBCSCHAR:							jkey.keycode = JK_HALF_WIDTH;					return jkey;	//TODO Testing Needed
		case VK_DBE_ROMAN:
		case VK_DBE_NOROMAN:							jkey.keycode = JK_ROMAN_CHARACTERS;				return jkey;	//TODO Testing Needed
		//case XK_Zen_Koho:								jkey.keycode = JK_ALL_CANDIDATES;				return jkey;	//Unknown Microsoft Support
		//case XK_Mae_Koho:								jkey.keycode = JK_PREVIOUS_CANDIDATE;			return jkey;	//Unknown Microsoft Support
		case VK_DBE_CODEINPUT:
		case VK_DBE_NOCODEINPUT:						jkey.keycode = JK_CODE_INPUT;					return jkey;	//TODO Testing Needed
		//case XK_Hiragana_Katakana:					jkey.keycode = JK_JAPANESE_KATAKANA;			return jkey;	//Unknown Microsoft Support
		//case XK_Hiragana_Katakana:					jkey.keycode = JK_JAPANESE_HIRAGANA;			return jkey;	//Unknown Microsoft Support
		//case XK_Romaji:								jkey.keycode = JK_JAPANESE_ROMAN;				return jkey;	//Unknown Microsoft Support
		//case XK_Kana_Lock:							jkey.keycode = JK_KANA_LOCK;					return jkey;	//Unknown Microsoft Support
		//case XK_VoidSymbol:							jkey.keycode = JK_INPUT_METHOD_ON_OFF;			return jkey;	//Unknown Microsoft Support

		/* For Sun keyboards */
		/*
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:									jkey.keycode = JK_CUT;							return jkey;	//No Microsoft Support
		case SunXK_Copy:								jkey.keycode = JK_COPY;							return jkey;	//No Microsoft Support
		case SunXK_Paste:								jkey.keycode = JK_PASTE;						return jkey;	//No Microsoft Support
		case SunXK_Undo:								jkey.keycode = JK_UNDO;							return jkey;	//No Microsoft Support
		case SunXK_Again:								jkey.keycode = JK_AGAIN;						return jkey;	//No Microsoft Support
		case SunXK_Find:								jkey.keycode = JK_FIND;							return jkey;	//No Microsoft Support
		case SunXK_Props:								jkey.keycode = JK_PROPS;						return jkey;	//No Microsoft Support
		case SunXK_Stop:								jkey.keycode = JK_STOP;							return jkey;	//No Microsoft Support
		case SunXK_Compose:								jkey.keycode = JK_COMPOSE;						return jkey;	//No Microsoft Support
		case SunXK_AltGraph:							jkey.keycode = JK_ALT_GRAPH;					return jkey;	//No Microsoft Support
		#endif
		*/

		//case XK_Begin:								jkey.keycode = JK_BEGIN;						return jkey;	//No Microsoft Support

		default:										jkey.keycode = JK_UNDEFINED; 					return jkey;
	}
}

jint NativeToJButton (unsigned int button) {
	switch (button) {
		case VK_LBUTTON:								return JBUTTON1;
		case VK_RBUTTON:								return JBUTTON2;
		case VK_MBUTTON:								return JBUTTON3;
		case VK_XBUTTON1:								return JBUTTON4;
		case VK_XBUTTON2:								return JBUTTON5;

		default:										return JNOBUTTON;
	}
}

jint NativeToJModifier(unsigned int mod) {
	switch (mod) {
		case MOD_SHIFT:									return JK_SHIFT_MASK;
		case MOD_CONTROL:								return JK_CTRL_MASK;
		case MOD_WIN:									return JK_META_MASK;
		case MOD_ALT:									return JK_ALT_MASK;

		case MOD_LBUTTON:								return JK_BUTTON1_MASK;
		case MOD_RBUTTON:								return JK_BUTTON2_MASK;
		case MOD_MBUTTON:								return JK_BUTTON3_MASK;
		case MOD_XBUTTON1:								return JK_BUTTON4_MASK;
		case MOD_XBUTTON2:								return JK_BUTTON5_MASK;

		default:										return JK_UNDEFINED;
	}
}
