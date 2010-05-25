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
//Reference: http://developer.apple.com/mac/library/documentation/Carbon/Reference/CarbonRefUpdate/Articles/Carbon_10.4-10.5_SymbolChanges.html
#include "include/JConvertToNative.h"
#include "OSXKeyCodes.h"
#include "OSXButtonCodes.h"

#include <HIToolbox/CarbonEvents.h>

JKeyDatum NativeToJKeycode(unsigned int keysym) {
	JKeyDatum jkey;
	jkey.location = JK_LOCATION_STANDARD;

	switch ((unsigned int) keysym) {
		case kVK_Return:								jkey.keycode = JK_ENTER;						return jkey;
		case kVK_Delete:								jkey.keycode = JK_BACK_SPACE;					return jkey;
		case kVK_Tab:									jkey.keycode = JK_TAB;							return jkey;
		//case XK_Cancel:								jkey.keycode = JK_CANCEL;						return jkey;	//No Apple Support

		case kVK_Shift:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_SHIFT;						return jkey;
		case kVK_RightShift:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_SHIFT;						return jkey;
		case kVK_Control:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_CONTROL;						return jkey;
		case kVK_RightControl:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_CONTROL;						return jkey;
		case kVK_Option:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_ALT;							return jkey;
		case kVK_RightOption:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_ALT;							return jkey;
		case kVK_Command:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_META;							return jkey;
		//FIXME Not sure where apple is hiding this one.
		//case kVK_RightCommand:
		//	jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_META;							return jkey;
		//case XK_Super_L:
		//	jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_WINDOWS;						return jkey;
		//case XK_Super_R:
		//	jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_WINDOWS;						return jkey;
		//case XK_Menu:									jkey.keycode = JK_CONTEXT_MENU;					return jkey;

		//case XK_Pause:								jkey.keycode = JK_PAUSE;						return jkey;	//No Apple Support
		case kVK_CapsLock:								jkey.keycode = JK_CAPS_LOCK;					return jkey;
		case kVK_Escape:								jkey.keycode = JK_ESCAPE;						return jkey;
		case kVK_Space:									jkey.keycode = JK_SPACE;						return jkey;
		case kVK_PageUp:								jkey.keycode = JK_PAGE_UP;						return jkey;
		case kVK_PageDown:								jkey.keycode = JK_PAGE_DOWN;					return jkey;
		case kVK_Home:									jkey.keycode = JK_HOME;							return jkey;
		case kVK_End:									jkey.keycode = JK_END;							return jkey;

		case kVK_LeftArrow:								jkey.keycode = JK_LEFT;							return jkey;
		case kVK_RightArrow:							jkey.keycode = JK_RIGHT;						return jkey;
		case kVK_UpArrow:								jkey.keycode = JK_UP;							return jkey;
		case kVK_DownArrow:								jkey.keycode = JK_DOWN;							return jkey;

		case kVK_ANSI_Comma:							jkey.keycode = JK_COMMA;						return jkey;
		case kVK_ANSI_Minus:							jkey.keycode = JK_MINUS;						return jkey;
		case kVK_ANSI_Period:							jkey.keycode = JK_PERIOD;						return jkey;
		case kVK_ANSI_Slash:							jkey.keycode = JK_SLASH;						return jkey;

		case kVK_ANSI_0:								jkey.keycode = JK_0;							return jkey;
		case kVK_ANSI_1:								jkey.keycode = JK_1;							return jkey;
		case kVK_ANSI_2:								jkey.keycode = JK_2;							return jkey;
		case kVK_ANSI_3:								jkey.keycode = JK_3;							return jkey;
		case kVK_ANSI_4:								jkey.keycode = JK_4;							return jkey;
		case kVK_ANSI_5:								jkey.keycode = JK_5;							return jkey;
		case kVK_ANSI_6:								jkey.keycode = JK_6;							return jkey;
		case kVK_ANSI_7:								jkey.keycode = JK_7;							return jkey;
		case kVK_ANSI_8:								jkey.keycode = JK_8;							return jkey;
		case kVK_ANSI_9:								jkey.keycode = JK_9;							return jkey;

		case kVK_ANSI_Semicolon:						jkey.keycode = JK_SEMICOLON;					return jkey;
		case kVK_ANSI_Equal:							jkey.keycode = JK_EQUALS;						return jkey;

		case kVK_ANSI_A:								jkey.keycode = JK_A;							return jkey;
		case kVK_ANSI_B:								jkey.keycode = JK_B;							return jkey;
		case kVK_ANSI_C:								jkey.keycode = JK_C;							return jkey;
		case kVK_ANSI_D:								jkey.keycode = JK_D;							return jkey;
		case kVK_ANSI_E:								jkey.keycode = JK_E;							return jkey;
		case kVK_ANSI_F:								jkey.keycode = JK_F;							return jkey;
		case kVK_ANSI_G:								jkey.keycode = JK_G;							return jkey;
		case kVK_ANSI_H:								jkey.keycode = JK_H;							return jkey;
		case kVK_ANSI_I:								jkey.keycode = JK_I;							return jkey;
		case kVK_ANSI_J:								jkey.keycode = JK_J;							return jkey;
		case kVK_ANSI_K:								jkey.keycode = JK_K;							return jkey;
		case kVK_ANSI_L:								jkey.keycode = JK_L;							return jkey;
		case kVK_ANSI_M:								jkey.keycode = JK_M;							return jkey;
		case kVK_ANSI_N:								jkey.keycode = JK_N;							return jkey;
		case kVK_ANSI_O:								jkey.keycode = JK_O;							return jkey;
		case kVK_ANSI_P:								jkey.keycode = JK_P;							return jkey;
		case kVK_ANSI_Q:								jkey.keycode = JK_Q;							return jkey;
		case kVK_ANSI_R:								jkey.keycode = JK_R;							return jkey;
		case kVK_ANSI_S:								jkey.keycode = JK_S;							return jkey;
		case kVK_ANSI_T:								jkey.keycode = JK_T;							return jkey;
		case kVK_ANSI_U:								jkey.keycode = JK_U;							return jkey;
		case kVK_ANSI_V:								jkey.keycode = JK_V;							return jkey;
		case kVK_ANSI_W:								jkey.keycode = JK_W;							return jkey;
		case kVK_ANSI_X:								jkey.keycode = JK_X;							return jkey;
		case kVK_ANSI_Y:								jkey.keycode = JK_Y;							return jkey;
		case kVK_ANSI_Z:								jkey.keycode = JK_Z;							return jkey;

		case kVK_ANSI_LeftBracket:						jkey.keycode = JK_OPEN_BRACKET;					return jkey;
		case kVK_ANSI_Backslash:						jkey.keycode = JK_BACK_SLASH;					return jkey;
		case kVK_ANSI_RightBracket:						jkey.keycode = JK_CLOSE_BRACKET;				return jkey;

		case kVK_ANSI_Keypad0:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD0;						return jkey;
		case kVK_ANSI_Keypad1:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD1;						return jkey;
		case kVK_ANSI_Keypad2:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD2;						return jkey;
		case kVK_ANSI_Keypad3:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD3;						return jkey;
		case kVK_ANSI_Keypad4:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD4;						return jkey;
		case kVK_ANSI_Keypad5:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD5;						return jkey;
		case kVK_ANSI_Keypad6:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD6;						return jkey;
		case kVK_ANSI_Keypad7:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD7;						return jkey;
		case kVK_ANSI_Keypad8:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD8;						return jkey;
		case kVK_ANSI_Keypad9:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD9;						return jkey;

		case kVK_ANSI_KeypadMultiply:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_MULTIPLY;						return jkey;
		case kVK_ANSI_KeypadPlus:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_ADD;							return jkey;
		//case XK_KP_Separator:
		//	jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_SEPARATOR;					return jkey;	//No Apple Support
		case kVK_ANSI_KeypadMinus:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_SUBTRACT;						return jkey;
		case kVK_ANSI_KeypadDecimal:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_DECIMAL;						return jkey;
		case kVK_ANSI_KeypadDivide:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_DIVIDE;						return jkey;
		case kVK_ANSI_KeypadClear:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_CLEAR;						return jkey;
		//case XK_Num_Lock:
		//	jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUM_LOCK;						return jkey;	//No Apple Support
		//case XK_Scroll_Lock:
		//	jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_SCROLL_LOCK;					return jkey;	//No Apple Support

		case kVK_F1:									jkey.keycode = JK_F1;							return jkey;
		case kVK_F2:									jkey.keycode = JK_F2;							return jkey;
		case kVK_F3:									jkey.keycode = JK_F3;							return jkey;
		case kVK_F4:									jkey.keycode = JK_F4;							return jkey;
		case kVK_F5:									jkey.keycode = JK_F5;							return jkey;
		case kVK_F6:									jkey.keycode = JK_F6;							return jkey;
		case kVK_F7:									jkey.keycode = JK_F7;							return jkey;
		case kVK_F8:									jkey.keycode = JK_F8;							return jkey;
		case kVK_F9:									jkey.keycode = JK_F9;							return jkey;
		case kVK_F10:									jkey.keycode = JK_F10;							return jkey;
		case kVK_F11:									jkey.keycode = JK_F11;							return jkey;
		case kVK_F12:									jkey.keycode = JK_F12;							return jkey;
		case kVK_F13:									jkey.keycode = JK_F13;							return jkey;
		case kVK_F14:									jkey.keycode = JK_F14;							return jkey;
		case kVK_F15:									jkey.keycode = JK_F15;							return jkey;
		case kVK_F16:									jkey.keycode = JK_F16;							return jkey;
		case kVK_F17:									jkey.keycode = JK_F17;							return jkey;
		case kVK_F18:									jkey.keycode = JK_F18;							return jkey;
		case kVK_F19:									jkey.keycode = JK_F19;							return jkey;
		case kVK_F20:									jkey.keycode = JK_F20;							return jkey;
		//case XK_F21:									jkey.keycode = JK_F21;							return jkey;	//No Apple Support
		//case XK_F22:									jkey.keycode = JK_F22;							return jkey;	//No Apple Support
		//case XK_F23:									jkey.keycode = JK_F23;							return jkey;	//No Apple Support
		//case XK_F24:									jkey.keycode = JK_F24;							return jkey;	//No Apple Support

		//case XK_Print:								jkey.keycode = JK_PRINTSCREEN;					return jkey;	//No Apple Support
		//case XK_Insert:								jkey.keycode = JK_INSERT;						return jkey;	//No Apple Support
		case kVK_Help:									jkey.keycode = JK_HELP;							return jkey;
		case kVK_ForwardDelete:							jkey.keycode = JK_DELETE;						return jkey;

		case kVK_ANSI_Quote:							jkey.keycode = JK_QUOTE;						return jkey;
		case kVK_ANSI_Grave:							jkey.keycode = JK_BACK_QUOTE;					return jkey;

		//case XK_KP_Up:								jkey.keycode = JK_KP_UP;						return jkey;	//No Apple Support
		//case XK_KP_Down:								jkey.keycode = JK_KP_DOWN;						return jkey;	//No Apple Support
		//case XK_KP_Left:								jkey.keycode = JK_KP_LEFT;						return jkey;	//No Apple Support
		//case XK_KP_Right:								jkey.keycode = JK_KP_RIGHT;						return jkey;	//No Apple Support

		//case XK_dead_grave:							jkey.keycode = JK_DEAD_GRAVE;					return jkey;	//Unknown Apple Support
		//case XK_dead_acute:							jkey.keycode = JK_DEAD_ACUTE;					return jkey;	//Unknown Apple Support
		//case XK_dead_circumflex:						jkey.keycode = JK_DEAD_CIRCUMFLEX;				return jkey;	//Unknown Apple Support
		//case XK_dead_tilde:							jkey.keycode = JK_DEAD_TILDE;					return jkey;	//Unknown Apple Support
		//case XK_dead_macron:							jkey.keycode = JK_DEAD_MACRON;					return jkey;	//Unknown Apple Support
		//case XK_dead_breve:							jkey.keycode = JK_DEAD_BREVE;					return jkey;	//Unknown Apple Support
		//case XK_dead_abovedot:						jkey.keycode = JK_DEAD_ABOVEDOT;				return jkey;	//Unknown Apple Support
		//case XK_dead_diaeresis:						jkey.keycode = JK_DEAD_DIAERESIS;				return jkey;	//Unknown Apple Support
		//case XK_dead_abovering:						jkey.keycode = JK_DEAD_ABOVERING;				return jkey;	//Unknown Apple Support
		//case XK_dead_doubleacute:						jkey.keycode = JK_DEAD_DOUBLEACUTE;				return jkey;	//Unknown Apple Support
		//case XK_dead_caron:							jkey.keycode = JK_DEAD_CARON;					return jkey;	//Unknown Apple Support
		//case XK_dead_cedilla:							jkey.keycode = JK_DEAD_CEDILLA;					return jkey;	//Unknown Apple Support
		//case XK_dead_ogonek:							jkey.keycode = JK_DEAD_OGONEK;					return jkey;	//Unknown Apple Support
		//case XK_dead_iota:							jkey.keycode = JK_DEAD_IOTA;					return jkey;	//Unknown Apple Support
		//case XK_dead_voiced_sound:					jkey.keycode = JK_DEAD_VOICED_SOUND;			return jkey;	//Unknown Apple Support
		//case XK_dead_semivoiced_sound:				jkey.keycode = JK_DEAD_SEMIVOICED_SOUND;		return jkey;	//Unknown Apple Support

		//case XK_ampersand:							jkey.keycode = JK_AMPERSAND;					return jkey;	//Unknown Apple Support
		//case XK_asterisk:								jkey.keycode = JK_ASTERISK;						return jkey;	//Unknown Apple Support
		//case XK_quotedbl:								jkey.keycode = JK_QUOTEDBL;						return jkey;	//Unknown Apple Support
		//case XK_less:									jkey.keycode = JK_LESS;							return jkey;	//Unknown Apple Support
		//case XK_greater:								jkey.keycode = JK_GREATER;						return jkey;	//Unknown Apple Support
		//case XK_braceleft:							jkey.keycode = JK_BRACELEFT;					return jkey;	//Unknown Apple Support
		//case XK_braceright:							jkey.keycode = JK_BRACERIGHT;					return jkey;	//Unknown Apple Support

		//case XK_at:									jkey.keycode = JK_AT;							return jkey;	//Unknown Apple Support
		//case XK_colon:								jkey.keycode = JK_COLON;						return jkey;	//Unknown Apple Support
		//case XK_asciicircum:							jkey.keycode = JK_CIRCUMFLEX;					return jkey;	//Unknown Apple Support
		//case XK_dollar:								jkey.keycode = JK_DOLLAR;						return jkey;	//Unknown Apple Support
		//case XK_EuroSign:								jkey.keycode = JK_EURO_SIGN;					return jkey;	//Unknown Apple Support
		//case XK_exclam:								jkey.keycode = JK_EXCLAMATION_MARK;				return jkey;	//Unknown Apple Support
		//case XK_exclamdown:							jkey.keycode = JK_INVERTED_EXCLAMATION_MARK;	return jkey;	//Unknown Apple Support
		//case XK_parenleft:							jkey.keycode = JK_LEFT_PARENTHESIS;				return jkey;	//Unknown Apple Support
		//case XK_numbersign:							jkey.keycode = JK_NUMBER_SIGN;					return jkey;	//Unknown Apple Support
		//case XK_plus:									jkey.keycode = JK_PLUS;							return jkey;	//Unknown Apple Support
		//case XK_parenright:							jkey.keycode = JK_RIGHT_PARENTHESIS;			return jkey;	//Unknown Apple Support
		//case kVK_JIS_Underscore:						jkey.keycode = JK_UNDERSCORE;					return jkey;


		/* for input method support on Asian Keyboards */
		//case XK_Cancel:								jkey.keycode = JK_FINAL;						return jkey;	//Unknown Apple Support
		//case XK_Henkan:								jkey.keycode = JK_CONVERT;						return jkey;	//Unknown Apple Support
		//case XK_Muhenkan:								jkey.keycode = JK_NONCONVERT;					return jkey;	//Unknown Apple Support
		//case XK_VoidSymbol:							jkey.keycode = JK_ACCEPT;						return jkey;	//Unknown Apple Support
		//case XK_Mode_switch:							jkey.keycode = JK_MODECHANGE;					return jkey;	//Unknown Apple Support
		//case kVK_JIS_Kana:							jkey.keycode = JK_KANA;							return jkey;	//Unknown Apple Support
		//case XK_Kanji:								jkey.keycode = JK_KANJI;						return jkey;	//Unknown Apple Support
		//case kVK_JIS_Eisu:							jkey.keycode = JK_ALPHANUMERIC;					return jkey;
		//case XK_Katakana:								jkey.keycode = JK_KATAKANA;						return jkey;	//Unknown Apple Support
		//case XK_Hiragana:								jkey.keycode = JK_HIRAGANA;						return jkey;	//Unknown Apple Support
		//case XK_Zenkaku:								jkey.keycode = JK_FULL_WIDTH;					return jkey;	//Unknown Apple Support
		//case XK_Hankaku:								jkey.keycode = JK_HALF_WIDTH;					return jkey;	//Unknown Apple Support
		//case XK_Romaji:								jkey.keycode = JK_ROMAN_CHARACTERS;				return jkey;	//Unknown Apple Support
		//case XK_Zen_Koho:								jkey.keycode = JK_ALL_CANDIDATES;				return jkey;	//Unknown Apple Support
		//case XK_Mae_Koho:								jkey.keycode = JK_PREVIOUS_CANDIDATE;			return jkey;	//Unknown Apple Support
		//case XK_Kanji_Bangou:							jkey.keycode = JK_CODE_INPUT;					return jkey;	//Unknown Apple Support
		//case XK_Hiragana_Katakana:					jkey.keycode = JK_JAPANESE_KATAKANA;			return jkey;	//Unknown Apple Support
		//case XK_Hiragana_Katakana:					jkey.keycode = JK_JAPANESE_HIRAGANA;			return jkey;	//Unknown Apple Support
		//case XK_Romaji:								jkey.keycode = JK_JAPANESE_ROMAN;				return jkey;	//Unknown Apple Support
		//case XK_Kana_Lock:							jkey.keycode = JK_KANA_LOCK;					return jkey;	//Unknown Apple Support
		//case XK_VoidSymbol:							jkey.keycode = JK_INPUT_METHOD_ON_OFF;			return jkey;	//Unknown Apple Support

		/* for Sun keyboards */
		//case SunXK_Cut:								jkey.keycode = JK_CUT;							return jkey;	//No Apple Support
		//case SunXK_Copy:								jkey.keycode = JK_COPY;							return jkey;	//No Apple Support
		//case SunXK_Paste:								jkey.keycode = JK_PASTE;						return jkey;	//No Apple Support
		//case SunXK_Undo:								jkey.keycode = JK_UNDO;							return jkey;	//No Apple Support
		//case SunXK_Again:								jkey.keycode = JK_AGAIN;						return jkey;	//No Apple Support
		//case SunXK_Find:								jkey.keycode = JK_FIND;							return jkey;	//No Apple Support
		//case SunXK_Props:								jkey.keycode = JK_PROPS;						return jkey;	//No Apple Support
		//case SunXK_Stop:								jkey.keycode = JK_STOP;							return jkey;	//No Apple Support

		//case SunXK_Compose:							jkey.keycode = JK_COMPOSE;						return jkey;	//No Apple Support
		//case SunXK_AltGraph:							jkey.keycode = JK_ALT_GRAPH;					return jkey;	//No Apple Support
		//case XK_Begin:								jkey.keycode = JK_BEGIN;						return jkey;	//No Apple Support

		default:
		case 0xFF:										jkey.keycode = JK_UNDEFINED;					return jkey;
	}
}

jint NativeToJButton(unsigned int button) {
	switch (button) {
		case kVK_LBUTTON:								return JBUTTON1;
		case kVK_RBUTTON:								return JBUTTON2;
		case kVK_MBUTTON:								return JBUTTON3;
		case kVK_XBUTTON1:								return JBUTTON4;
		case kVK_XBUTTON2:								return JBUTTON5;

		default:										return JNOBUTTON;
	}
}

jint NativeToJModifier(unsigned int modifier) {
	switch (modifier) {
		case shiftKey:
		case rightShiftKey:								return JK_SHIFT_MASK;

		case controlKey:
		case rightControlKey:							return JK_CTRL_MASK;

		case optionKey:
		case rightOptionKey:							return JK_ALT_MASK;

		case cmdKey:									return JK_META_MASK;
		//case rightCmdKey:								//Undefined

		default:										return 0;
	}
}
