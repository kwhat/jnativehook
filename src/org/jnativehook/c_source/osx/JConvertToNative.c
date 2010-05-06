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

#include "include/JConvertToNative.h"
#include "OSXKeyCodes.h"
#include "OSXButtonCodes.h"

//Values mapped from http://developer.apple.com/mac/library/documentation/Carbon/Reference/CarbonRefUpdate/Articles/Carbon_10.4-10.5_SymbolChanges.html
unsigned int JKeycodeToNative(JKeyCode jkey) {
	switch (jkey.keycode) {
		case JK_ENTER:									return kVK_Return;
		case JK_BACK_SPACE:								return kVK_Delete;
		case JK_TAB:									return kVK_Tab;
		case JK_CLEAR:									return kVK_ANSI_KeypadClear;
		//case JK_CANCEL:								return XK_Cancel;				//No Apple Support
		case JK_SHIFT:
			if (jkey.location == JK_LOCATION_LEFT)		return kVK_Shift;
			else										return kVK_RightShift;
		case JK_CONTROL:
			if (jkey.location == JK_LOCATION_LEFT)		return kVK_Control;
			else										return kVK_RightControl;
		case JK_ALT:
			if (jkey.location == JK_LOCATION_LEFT)		return kVK_Option;
			else										return kVK_RightOption;
		//case JK_PAUSE:								return XK_Pause;				//No Apple Support
		case JK_CAPS_LOCK:								return kVK_CapsLock;
		case JK_ESCAPE:									return kVK_Escape;
		case JK_SPACE:									return kVK_Space;
		case JK_PAGE_UP:								return kVK_PageUp;
		case JK_PAGE_DOWN:								return kVK_PageDown;
		case JK_HOME:									return kVK_Home;
		case JK_END:									return kVK_End;

		case JK_LEFT:									return kVK_LeftArrow;
		case JK_RIGHT:									return kVK_RightArrow;
		case JK_UP:										return kVK_UpArrow;
		case JK_DOWN:									return kVK_DownArrow;

		case JK_COMMA:									return kVK_ANSI_Comma;
		case JK_MINUS:									return kVK_ANSI_Minus;
		case JK_PERIOD:									return kVK_ANSI_Period;
		case JK_SLASH:									return kVK_ANSI_Slash;

		case JK_0:										return kVK_ANSI_0;
		case JK_1:										return kVK_ANSI_1;
		case JK_2:										return kVK_ANSI_2;
		case JK_3:										return kVK_ANSI_3;
		case JK_4:										return kVK_ANSI_4;
		case JK_5:										return kVK_ANSI_5;
		case JK_6:										return kVK_ANSI_6;
		case JK_7:										return kVK_ANSI_7;
		case JK_8:										return kVK_ANSI_8;
		case JK_9:										return kVK_ANSI_9;

		case JK_SEMICOLON:								return kVK_ANSI_Semicolon;
		case JK_EQUALS:									return kVK_ANSI_Equal;

		case JK_A:										return kVK_ANSI_A;
		case JK_B:										return kVK_ANSI_B;
		case JK_C:										return kVK_ANSI_C;
		case JK_D:										return kVK_ANSI_D;
		case JK_E:										return kVK_ANSI_E;
		case JK_F:										return kVK_ANSI_F;
		case JK_G:										return kVK_ANSI_G;
		case JK_H:										return kVK_ANSI_H;
		case JK_I:										return kVK_ANSI_I;
		case JK_J:										return kVK_ANSI_J;
		case JK_K:										return kVK_ANSI_K;
		case JK_L:										return kVK_ANSI_L;
		case JK_M:										return kVK_ANSI_M;
		case JK_N:										return kVK_ANSI_N;
		case JK_O:										return kVK_ANSI_O;
		case JK_P:										return kVK_ANSI_P;
		case JK_Q:										return kVK_ANSI_Q;
		case JK_R:										return kVK_ANSI_R;
		case JK_S:										return kVK_ANSI_S;
		case JK_T:										return kVK_ANSI_T;
		case JK_U:										return kVK_ANSI_U;
		case JK_V:										return kVK_ANSI_V;
		case JK_W:										return kVK_ANSI_W;
		case JK_X:										return kVK_ANSI_X;
		case JK_Y:										return kVK_ANSI_Y;
		case JK_Z:										return kVK_ANSI_Z;

		case JK_OPEN_BRACKET:							return kVK_ANSI_LeftBracket;
		case JK_BACK_SLASH:								return kVK_ANSI_Backslash;
		case JK_CLOSE_BRACKET:							return kVK_ANSI_RightBracket;

		case JK_NUMPAD0:								return kVK_ANSI_Keypad0;
		case JK_NUMPAD1:								return kVK_ANSI_Keypad1;
		case JK_NUMPAD2:								return kVK_ANSI_Keypad2;
		case JK_NUMPAD3:								return kVK_ANSI_Keypad3;
		case JK_NUMPAD4:								return kVK_ANSI_Keypad4;
		case JK_NUMPAD5:								return kVK_ANSI_Keypad5;
		case JK_NUMPAD6:								return kVK_ANSI_Keypad6;
		case JK_NUMPAD7:								return kVK_ANSI_Keypad7;
		case JK_NUMPAD8:								return kVK_ANSI_Keypad8;
		case JK_NUMPAD9:								return kVK_ANSI_Keypad9;
		case JK_MULTIPLY:								return kVK_ANSI_KeypadMultiply;
		case JK_ADD:									return kVK_ANSI_KeypadPlus;

		//case JK_SEPARATOR:							return XK_KP_Separator;				//Unknown Apple Support

		case JK_SUBTRACT:								return kVK_ANSI_KeypadMinus;
		case JK_DECIMAL:								return kVK_ANSI_KeypadDecimal;
		case JK_DIVIDE:									return kVK_ANSI_KeypadDivide;
		case JK_DELETE:									return kVK_ForwardDelete;
		//case JK_NUM_LOCK:								return XK_Num_Lock;					//No Apple Support
		//case JK_SCROLL_LOCK:							return XK_Scroll_Lock;				//No Apple Support

		case JK_F1:										return kVK_F1;
		case JK_F2:										return kVK_F2;
		case JK_F3:										return kVK_F3;
		case JK_F4:										return kVK_F4;
		case JK_F5:										return kVK_F5;
		case JK_F6:										return kVK_F6;
		case JK_F7:										return kVK_F7;
		case JK_F8:										return kVK_F8;
		case JK_F9:										return kVK_F9;
		case JK_F10:									return kVK_F10;
		case JK_F11:									return kVK_F11;
		case JK_F12:									return kVK_F12;
		case JK_F13:									return kVK_F13;
		case JK_F14:									return kVK_F14;
		case JK_F15:									return kVK_F15;
		case JK_F16:									return kVK_F16;
		case JK_F17:									return kVK_F17;
		case JK_F18:									return kVK_F18;
		case JK_F19:									return kVK_F19;
		case JK_F20:									return kVK_F20;
		//case JK_F21:									return XK_F21;						//No Apple Support
		//case JK_F22:									return XK_F22;						//No Apple Support
		//case JK_F23:									return XK_F23;						//No Apple Support
		//case JK_F24:									return XK_F24;						//No Apple Support

		//case JK_PRINTSCREEN:							return XK_Print;					//No Apple Support
		//case JK_INSERT:								return XK_Insert;					//No Apple Support
		case JK_HELP:									return kVK_Help;
		case JK_META:
			if (jkey.location == JK_LOCATION_LEFT)		return kVK_Command;					//FIXME No apple left/right support?
			else										return kVK_Command;					//Could this be function key?

		case JK_QUOTE:									return kVK_ANSI_Quote;
		case JK_BACK_QUOTE:								return kVK_ANSI_Grave;

		//case JK_KP_UP:								return XK_KP_Up;					//No Apple Support
		//case JK_KP_DOWN:								return XK_KP_Down;					//No Apple Support
		//case JK_KP_LEFT:								return XK_KP_Left;					//No Apple Support
		//case JK_KP_RIGHT:								return XK_KP_Right;					//No Apple Support

		//case JK_DEAD_GRAVE:							return XK_dead_grave;				//Unknown Apple Support
		//case JK_DEAD_ACUTE:							return XK_dead_acute;				//Unknown Apple Support
		//case JK_DEAD_CIRCUMFLEX:						return XK_dead_circumflex;			//Unknown Apple Support
		//case JK_DEAD_TILDE:							return XK_dead_tilde;				//Unknown Apple Support
		//case JK_DEAD_MACRON:							return XK_dead_macron;				//Unknown Apple Support
		//case JK_DEAD_BREVE:							return XK_dead_breve;				//Unknown Apple Support
		//case JK_DEAD_ABOVEDOT:						return XK_dead_abovedot;			//Unknown Apple Support
		//case JK_DEAD_DIAERESIS:						return XK_dead_diaeresis;			//Unknown Apple Support
		//case JK_DEAD_ABOVERING:						return XK_dead_abovering;			//Unknown Apple Support
		//case JK_DEAD_DOUBLEACUTE:						return XK_dead_doubleacute;			//Unknown Apple Support
		//case JK_DEAD_CARON:							return XK_dead_caron;				//Unknown Apple Support
		//case JK_DEAD_CEDILLA:							return XK_dead_cedilla;				//Unknown Apple Support
		//case JK_DEAD_OGONEK:							return XK_dead_ogonek;				//Unknown Apple Support
		//case JK_DEAD_IOTA:							return XK_dead_iota;				//Unknown Apple Support
		//case JK_DEAD_VOICED_SOUND:					return XK_dead_voiced_sound;		//Unknown Apple Support
		//case JK_DEAD_SEMIVOICED_SOUND:				return XK_dead_semivoiced_sound;	//Unknown Apple Support

		//case JK_AMPERSAND:							return XK_ampersand;				//Unknown Apple Support
		//case JK_ASTERISK:								return XK_asterisk;					//Unknown Apple Support
		//case JK_QUOTEDBL:								return XK_quotedbl;					//Unknown Apple Support
		//case JK_LESS:									return XK_less;						//Unknown Apple Support
		//case JK_GREATER:								return XK_greater;					//Unknown Apple Support
		//case JK_BRACELEFT:							return XK_braceleft;				//Unknown Apple Support
		//case JK_BRACERIGHT:							return XK_braceright;				//Unknown Apple Support

		//case JK_AT:									return XK_at;						//Unknown Apple Support
		//case JK_COLON:								return XK_colon;					//Unknown Apple Support
		//case JK_CIRCUMFLEX:							return XK_asciicircum;				//Unknown Apple Support
		//case JK_DOLLAR:								return XK_dollar;					//Unknown Apple Support
		//case JK_EURO_SIGN:							return XK_EuroSign;					//Unknown Apple Support
		//case JK_EXCLAMATION_MARK:						return XK_exclam;					//Unknown Apple Support
		//case JK_INVERTED_EXCLAMATION_MARK:			return XK_exclamdown;				//Unknown Apple Support
		//case JK_LEFT_PARENTHESIS:						return XK_parenleft;				//Unknown Apple Support
		//case JK_NUMBER_SIGN:							return XK_numbersign;				//Unknown Apple Support
		//case JK_PLUS:									return XK_plus;						//Unknown Apple Support
		//case JK_RIGHT_PARENTHESIS:					return XK_parenright;				//Unknown Apple Support
		case JK_UNDERSCORE:								return kVK_JIS_Underscore;

		case JK_WINDOWS:
			if (jkey.location == JK_LOCATION_LEFT)		return XK_Super_L;
			else										return XK_Super_R;
		case JK_CONTEXT_MENU:							return XK_Menu;

		/* for input method support on Asian Keyboards */
		//case JK_FINAL:								return XK_Cancel;					//Unknown Apple Support
		//case JK_CONVERT:								return XK_Henkan;					//Unknown Apple Support
		//case JK_NONCONVERT:							return XK_Muhenkan;					//Unknown Apple Support
		//case JK_ACCEPT:								return XK_VoidSymbol;				//Unknown Apple Support
		//case JK_MODECHANGE:							return XK_Mode_switch;				//Unknown Apple Support
		case JK_KANA:									return kVK_JIS_Kana;
		//case JK_KANJI:								return XK_Kanji;					//Unknown Apple Support
		case JK_ALPHANUMERIC:							return kVK_JIS_Eisu;
		//case JK_KATAKANA:								return XK_Katakana;					//Unknown Apple Support
		//case JK_HIRAGANA:								return XK_Hiragana;					//Unknown Apple Support
		//case JK_FULL_WIDTH:							return XK_Zenkaku;					//Unknown Apple Support
		//case JK_HALF_WIDTH:							return XK_Hankaku;					//Unknown Apple Support
		//case JK_ROMAN_CHARACTERS:						return XK_Romaji;					//Unknown Apple Support
		//case JK_ALL_CANDIDATES:						return XK_Zen_Koho;					//Unknown Apple Support
		//case JK_PREVIOUS_CANDIDATE:					return XK_Mae_Koho;					//Unknown Apple Support
		//case JK_CODE_INPUT:							return XK_Kanji_Bangou;				//Unknown Apple Support
		//case JK_JAPANESE_KATAKANA:					return XK_Hiragana_Katakana;		//Unknown Apple Support
		//case JK_JAPANESE_HIRAGANA:					return XK_Hiragana_Katakana;		//Unknown Apple Support
		//case JK_JAPANESE_ROMAN:						return XK_Romaji;					//Unknown Apple Support
		//case JK_KANA_LOCK:							return XK_Kana_Lock;				//Unknown Apple Support
		//case JK_INPUT_METHOD_ON_OFF:					return XK_VoidSymbol;				//Unknown Apple Support

		/* for Sun keyboards */
		//case JK_CUT:									return SunXK_Cut;					//No Apple Support
		//case JK_COPY:									return SunXK_Copy;					//No Apple Support
		//case JK_PASTE:								return SunXK_Paste;					//No Apple Support
		//case JK_UNDO:									return SunXK_Undo;					//No Apple Support
		//case JK_AGAIN:								return SunXK_Again;					//No Apple Support
		//case JK_FIND:									return SunXK_Find;					//No Apple Support
		//case JK_PROPS:								return SunXK_Props;					//No Apple Support
		//case JK_STOP:									return SunXK_Stop;					//No Apple Support

		//case JK_COMPOSE:								return SunXK_Compose;				//No Apple Support
		//case JK_ALT_GRAPH:							return SunXK_AltGraph;				//No Apple Support
		//case JK_BEGIN:								return XK_Begin;					//No Apple Support

		default:
		case JK_UNDEFINED:								return 0xFF;						//Key Undefined
	}
}

JKeyCode NativeToJKeycode(unsigned int keycode) {
	JKeyCode jkey;
	jkey.location = JK_LOCATION_STANDARD;

	KeySym lower_keysym, upper_keysym;
	XConvertCase(keycode, &lower_keysym, &upper_keysym);

	switch ((unsigned int) upper_keysym) {
		case kVK_Return:								jkey.keycode = JK_ENTER;						return jkey;
		case kVK_Delete:								jkey.keycode = JK_BACK_SPACE;					return jkey;
		case kVK_Tab:									jkey.keycode = JK_TAB;							return jkey;
		case kVK_ANSI_KeypadClear:						jkey.keycode = JK_CLEAR;						return jkey;
		//case XK_Cancel:								jkey.keycode = JK_CANCEL;						return jkey;	//No Apple Support
		case XK_Shift_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_SHIFT;						return jkey;
		case XK_Shift_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_SHIFT;						return jkey;
		case XK_Control_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_CONTROL;						return jkey;
		case XK_Control_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_CONTROL;						return jkey;
		case XK_Alt_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_ALT;							return jkey;
		case XK_Alt_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_ALT;							return jkey;
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

		case kVK_ANSI_Keypad0:							jkey.keycode = JK_NUMPAD0;						return jkey;
		case kVK_ANSI_Keypad1:							jkey.keycode = JK_NUMPAD1;						return jkey;
		case kVK_ANSI_Keypad2:							jkey.keycode = JK_NUMPAD2;						return jkey;
		case kVK_ANSI_Keypad3:							jkey.keycode = JK_NUMPAD3;						return jkey;
		case kVK_ANSI_Keypad4:							jkey.keycode = JK_NUMPAD4;						return jkey;
		case kVK_ANSI_Keypad5:							jkey.keycode = JK_NUMPAD5;						return jkey;
		case kVK_ANSI_Keypad6:							jkey.keycode = JK_NUMPAD6;						return jkey;
		case kVK_ANSI_Keypad7:							jkey.keycode = JK_NUMPAD7;						return jkey;
		case kVK_ANSI_Keypad8:							jkey.keycode = JK_NUMPAD8;						return jkey;
		case kVK_ANSI_Keypad9:							jkey.keycode = JK_NUMPAD9;						return jkey;
		case kVK_ANSI_KeypadMultiply:					jkey.keycode = JK_MULTIPLY;						return jkey;
		case kVK_ANSI_KeypadPlus:						jkey.keycode = JK_ADD;							return jkey;

		//case XK_KP_Separator:							jkey.keycode = JK_SEPARATOR;					return jkey;	//No Apple Support

		case kVK_ANSI_KeypadMinus:						jkey.keycode = JK_SUBTRACT;						return jkey;
		case kVK_ANSI_KeypadDecimal:					jkey.keycode = JK_DECIMAL;						return jkey;
		case kVK_ANSI_KeypadDivide:						jkey.keycode = JK_DIVIDE;						return jkey;
		case kVK_ForwardDelete:							jkey.keycode = JK_DELETE;						return jkey;
		//case XK_Num_Lock:								jkey.keycode = JK_NUM_LOCK;						return jkey;	//No Apple Support
		//case XK_Scroll_Lock:							jkey.keycode = JK_SCROLL_LOCK;					return jkey;	//No Apple Support

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
		case XK_Meta_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_META;							return jkey;
		case XK_Meta_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_META;							return jkey;
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
		case kVK_JIS_Underscore:						jkey.keycode = JK_UNDERSCORE;					return jkey;

		case XK_Super_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_WINDOWS;						return jkey;
		case XK_Super_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_WINDOWS;						return jkey;
		case XK_Menu:									jkey.keycode = JK_CONTEXT_MENU;					return jkey;

		/* for input method support on Asian Keyboards */
		//case XK_Cancel:								jkey.keycode = JK_FINAL;						return jkey;	//Unknown Apple Support
		//case XK_Henkan:								jkey.keycode = JK_CONVERT;						return jkey;	//Unknown Apple Support
		//case XK_Muhenkan:								jkey.keycode = JK_NONCONVERT;					return jkey;	//Unknown Apple Support
		//case XK_VoidSymbol:							jkey.keycode = JK_ACCEPT;						return jkey;	//Unknown Apple Support
		//case XK_Mode_switch:							jkey.keycode = JK_MODECHANGE;					return jkey;	//Unknown Apple Support
		case kVK_JIS_Kana:								jkey.keycode = JK_KANA;							return jkey;	//Unknown Apple Support
		//case XK_Kanji:								jkey.keycode = JK_KANJI;						return jkey;	//Unknown Apple Support
		case kVK_JIS_Eisu:								jkey.keycode = JK_ALPHANUMERIC;					return jkey;
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
		default:
		case 0:											return JNOBUTTON;
	}
}

unsigned int JButtonToNative(jint button) {
	switch (button) {
		case JBUTTON1:									return kVK_LBUTTON;
		case JBUTTON2:									return kVK_RBUTTON;
		case JBUTTON3:									return kVK_MBUTTON;
		case JBUTTON4:									return kVK_XBUTTON1;
		case JBUTTON5:									return kVK_XBUTTON2;
		default:
		case JNOBUTTON:									return 0;
	}
}

unsigned int JModifierToNative(jint modifier) {
	switch (modifier) {
		case JK_SHIFT_MASK:								return ShiftMask;
		case JK_CTRL_MASK:								return ControlMask;
		case JK_META_MASK:								return getMetaMask();
		case JK_ALT_MASK:								return getAltMask();
		default: 										return AnyModifier;
	}
}

jint NativeToJModifier(unsigned int modifier) {
	switch (modifier) {
		case ShiftMask:									return JK_SHIFT_MASK;
		case ControlMask:								return JK_CTRL_MASK;
		default:
			if (modifier == getMetaMask())				return JK_META_MASK;
			else if (modifier == getAltMask())			return JK_ALT_MASK;
			return 0;
	}
}
