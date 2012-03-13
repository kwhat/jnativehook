/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h> //Reduced symbol set
//#include <X11/keysymdef.h>  //Full symbol set
#ifdef SUN_KEYBOARD
#include <X11/Sunkeysym.h>
#endif
#include "JConvertFromNative.h"
#include "XButtonCodes.h"
#include "XEventModifiers.h"

JKeyDatum NativeToJKey(unsigned int keysym) {
	JKeyDatum jkey;
	jkey.rawcode = keysym;
	jkey.location = JK_LOCATION_STANDARD;

	KeySym lower_keysym, upper_keysym;
	XConvertCase(keysym, &lower_keysym, &upper_keysym);

	switch ((unsigned int) upper_keysym) {
		case XK_Return:									jkey.keycode = JK_ENTER;						return jkey;
		case XK_BackSpace:								jkey.keycode = JK_BACK_SPACE;					return jkey;
		case XK_Tab:									jkey.keycode = JK_TAB;							return jkey;
		#ifndef SUN_KEYBOARD
		case XK_Cancel:									jkey.keycode = JK_CANCEL;						return jkey;
		#endif

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
		case XK_Meta_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_META;							return jkey;
		case XK_Meta_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_META;							return jkey;
		case XK_Super_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_WINDOWS;						return jkey;
		case XK_Super_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_WINDOWS;						return jkey;
		case XK_Menu:									jkey.keycode = JK_CONTEXT_MENU;					return jkey;

		case XK_Pause:									jkey.keycode = JK_PAUSE;						return jkey;
		case XK_Caps_Lock:								jkey.keycode = JK_CAPS_LOCK;					return jkey;
		case XK_Escape:									jkey.keycode = JK_ESCAPE;						return jkey;
		case XK_space:									jkey.keycode = JK_SPACE;						return jkey;

		case XK_Up:										jkey.keycode = JK_UP;							return jkey;
		case XK_Down:									jkey.keycode = JK_DOWN;							return jkey;
		case XK_Left:									jkey.keycode = JK_LEFT;							return jkey;
		case XK_Right:									jkey.keycode = JK_RIGHT;						return jkey;

		case XK_comma:									jkey.keycode = JK_COMMA;						return jkey;
		case XK_minus:									jkey.keycode = JK_MINUS;						return jkey;
		case XK_period:									jkey.keycode = JK_PERIOD;						return jkey;
		case XK_slash:									jkey.keycode = JK_SLASH;						return jkey;

		case XK_0:										jkey.keycode = JK_0;							return jkey;
		case XK_1:										jkey.keycode = JK_1;							return jkey;
		case XK_2:										jkey.keycode = JK_2;							return jkey;
		case XK_3:										jkey.keycode = JK_3;							return jkey;
		case XK_4:										jkey.keycode = JK_4;							return jkey;
		case XK_5:										jkey.keycode = JK_5;							return jkey;
		case XK_6:										jkey.keycode = JK_6;							return jkey;
		case XK_7:										jkey.keycode = JK_7;							return jkey;
		case XK_8:										jkey.keycode = JK_8;							return jkey;
		case XK_9:										jkey.keycode = JK_9;							return jkey;

		case XK_semicolon:								jkey.keycode = JK_SEMICOLON;					return jkey;
		case XK_equal:									jkey.keycode = JK_EQUALS;						return jkey;

		case XK_A:										jkey.keycode = JK_A;							return jkey;
		case XK_B:										jkey.keycode = JK_B;							return jkey;
		case XK_C:										jkey.keycode = JK_C;							return jkey;
		case XK_D:										jkey.keycode = JK_D;							return jkey;
		case XK_E:										jkey.keycode = JK_E;							return jkey;
		case XK_F:										jkey.keycode = JK_F;							return jkey;
		case XK_G:										jkey.keycode = JK_G;							return jkey;
		case XK_H:										jkey.keycode = JK_H;							return jkey;
		case XK_I:										jkey.keycode = JK_I;							return jkey;
		case XK_J:										jkey.keycode = JK_J;							return jkey;
		case XK_K:										jkey.keycode = JK_K;							return jkey;
		case XK_L:										jkey.keycode = JK_L;							return jkey;
		case XK_M:										jkey.keycode = JK_M;							return jkey;
		case XK_N:										jkey.keycode = JK_N;							return jkey;
		case XK_O:										jkey.keycode = JK_O;							return jkey;
		case XK_P:										jkey.keycode = JK_P;							return jkey;
		case XK_Q:										jkey.keycode = JK_Q;							return jkey;
		case XK_R:										jkey.keycode = JK_R;							return jkey;
		case XK_S:										jkey.keycode = JK_S;							return jkey;
		case XK_T:										jkey.keycode = JK_T;							return jkey;
		case XK_U:										jkey.keycode = JK_U;							return jkey;
		case XK_V:										jkey.keycode = JK_V;							return jkey;
		case XK_W:										jkey.keycode = JK_W;							return jkey;
		case XK_X:										jkey.keycode = JK_X;							return jkey;
		case XK_Y:										jkey.keycode = JK_Y;							return jkey;
		case XK_Z:										jkey.keycode = JK_Z;							return jkey;

		case XK_bracketleft:							jkey.keycode = JK_OPEN_BRACKET;					return jkey;
		case XK_backslash:								jkey.keycode = JK_BACK_SLASH;					return jkey;
		case XK_bracketright:							jkey.keycode = JK_CLOSE_BRACKET;				return jkey;

		case XK_KP_0:
		case XK_KP_Insert:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD0;						return jkey;
		case XK_KP_1:
		case XK_KP_End:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD1;						return jkey;
		case XK_KP_2:
		case XK_KP_Down:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD2;						return jkey;
		case XK_KP_3:
		case XK_KP_Page_Down:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD3;						return jkey;
		case XK_KP_4:
		case XK_KP_Left:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD4;						return jkey;
		case XK_KP_5:
		case XK_KP_Begin:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD5;						return jkey;
		case XK_KP_6:
		case XK_KP_Right:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD6;						return jkey;
		case XK_KP_7:
		case XK_KP_Home:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD7;						return jkey;
		case XK_KP_8:
		case XK_KP_Up:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD8;						return jkey;
		case XK_KP_9:
		case XK_KP_Page_Up:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUMPAD9;						return jkey;

		case XK_KP_Multiply:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_MULTIPLY;						return jkey;
		case XK_KP_Add:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_ADD;							return jkey;
		case XK_KP_Separator:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_SEPARATOR;					return jkey;
		case XK_KP_Subtract:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_SUBTRACT;						return jkey;
		case XK_KP_Enter:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_ENTER;						return jkey;
		case XK_KP_Decimal:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_DECIMAL;						return jkey;
		case XK_KP_Divide:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_DIVIDE;						return jkey;
		case XK_KP_Delete:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_DELETE;						return jkey;
		case XK_Num_Lock:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_NUM_LOCK;						return jkey;
		case XK_Clear:
			jkey.location = JK_LOCATION_NUMPAD;			jkey.keycode = JK_CLEAR;						return jkey;
		case XK_Scroll_Lock:							jkey.keycode = JK_SCROLL_LOCK;					return jkey;

		case XK_F1:										jkey.keycode = JK_F1;							return jkey;
		case XK_F2:										jkey.keycode = JK_F2;							return jkey;
		case XK_F3:										jkey.keycode = JK_F3;							return jkey;
		case XK_F4:										jkey.keycode = JK_F4;							return jkey;
		case XK_F5:										jkey.keycode = JK_F5;							return jkey;
		case XK_F6:										jkey.keycode = JK_F6;							return jkey;
		case XK_F7:										jkey.keycode = JK_F7;							return jkey;
		case XK_F8:										jkey.keycode = JK_F8;							return jkey;
		case XK_F9:										jkey.keycode = JK_F9;							return jkey;
		case XK_F10:									jkey.keycode = JK_F10;							return jkey;
		case XK_F11:									jkey.keycode = JK_F11;							return jkey;
		case XK_F12:									jkey.keycode = JK_F12;							return jkey;

		case XK_F13:									jkey.keycode = JK_F13;							return jkey;
		case XK_F14:									jkey.keycode = JK_F14;							return jkey;
		case XK_F15:									jkey.keycode = JK_F15;							return jkey;
		case XK_F16:									jkey.keycode = JK_F16;							return jkey;
		case XK_F17:									jkey.keycode = JK_F17;							return jkey;
		case XK_F18:									jkey.keycode = JK_F18;							return jkey;
		case XK_F19:									jkey.keycode = JK_F19;							return jkey;
		case XK_F20:									jkey.keycode = JK_F20;							return jkey;
		case XK_F21:									jkey.keycode = JK_F21;							return jkey;
		case XK_F22:									jkey.keycode = JK_F22;							return jkey;
		case XK_F23:									jkey.keycode = JK_F23;							return jkey;
		case XK_F24:									jkey.keycode = JK_F24;							return jkey;

		case XK_Print:									jkey.keycode = JK_PRINTSCREEN;					return jkey;
		case XK_Insert:									jkey.keycode = JK_INSERT;						return jkey;
		case XK_Delete:									jkey.keycode = JK_DELETE;						return jkey;
		case XK_Help:									jkey.keycode = JK_HELP;							return jkey;

		case XK_Page_Up:								jkey.keycode = JK_PAGE_UP;						return jkey;
		case XK_Page_Down:								jkey.keycode = JK_PAGE_DOWN;					return jkey;
		case XK_Home:									jkey.keycode = JK_HOME;							return jkey;
		case XK_End:									jkey.keycode = JK_END;							return jkey;

		case XK_apostrophe:								jkey.keycode = JK_QUOTE;						return jkey;
		case XK_grave:									jkey.keycode = JK_BACK_QUOTE;					return jkey;

		/* For European keyboards */
		case XK_dead_grave:								jkey.keycode = JK_DEAD_GRAVE;					return jkey;
		case XK_dead_acute:								jkey.keycode = JK_DEAD_ACUTE;					return jkey;
		case XK_dead_circumflex:						jkey.keycode = JK_DEAD_CIRCUMFLEX;				return jkey;
		case XK_dead_tilde:								jkey.keycode = JK_DEAD_TILDE;					return jkey;
		case XK_dead_macron:							jkey.keycode = JK_DEAD_MACRON;					return jkey;
		case XK_dead_breve:								jkey.keycode = JK_DEAD_BREVE;					return jkey;
		case XK_dead_abovedot:							jkey.keycode = JK_DEAD_ABOVEDOT;				return jkey;
		case XK_dead_diaeresis:							jkey.keycode = JK_DEAD_DIAERESIS;				return jkey;
		case XK_dead_abovering:							jkey.keycode = JK_DEAD_ABOVERING;				return jkey;
		case XK_dead_doubleacute:						jkey.keycode = JK_DEAD_DOUBLEACUTE;				return jkey;
		case XK_dead_caron:								jkey.keycode = JK_DEAD_CARON;					return jkey;
		case XK_dead_cedilla:							jkey.keycode = JK_DEAD_CEDILLA;					return jkey;
		case XK_dead_ogonek:							jkey.keycode = JK_DEAD_OGONEK;					return jkey;
		case XK_dead_iota:								jkey.keycode = JK_DEAD_IOTA;					return jkey;
		case XK_dead_voiced_sound:						jkey.keycode = JK_DEAD_VOICED_SOUND;			return jkey;
		case XK_dead_semivoiced_sound:					jkey.keycode = JK_DEAD_SEMIVOICED_SOUND;		return jkey;

		/* Unknown Keyboard Codes */
		case XK_ampersand:								jkey.keycode = JK_AMPERSAND;					return jkey;
		case XK_asterisk:								jkey.keycode = JK_ASTERISK;						return jkey;
		case XK_quotedbl:								jkey.keycode = JK_QUOTEDBL;						return jkey;
		case XK_less:									jkey.keycode = JK_LESS;							return jkey;
		case XK_greater:								jkey.keycode = JK_GREATER;						return jkey;
		case XK_braceleft:								jkey.keycode = JK_BRACELEFT;					return jkey;
		case XK_braceright:								jkey.keycode = JK_BRACERIGHT;					return jkey;

		/* Unknown Extended Keyboard Codes */
		case XK_at:										jkey.keycode = JK_AT;							return jkey;
		case XK_colon:									jkey.keycode = JK_COLON;						return jkey;
		case XK_asciicircum:							jkey.keycode = JK_CIRCUMFLEX;					return jkey;
		case XK_dollar:									jkey.keycode = JK_DOLLAR;						return jkey;
		case XK_EuroSign:								jkey.keycode = JK_EURO_SIGN;					return jkey;
		case XK_exclam:									jkey.keycode = JK_EXCLAMATION_MARK;				return jkey;
		case XK_exclamdown:								jkey.keycode = JK_INVERTED_EXCLAMATION_MARK;	return jkey;
		case XK_parenleft:								jkey.keycode = JK_LEFT_PARENTHESIS;				return jkey;
		case XK_numbersign:								jkey.keycode = JK_NUMBER_SIGN;					return jkey;
		case XK_plus:									jkey.keycode = JK_PLUS;							return jkey;
		case XK_parenright:								jkey.keycode = JK_RIGHT_PARENTHESIS;			return jkey;
		case XK_underscore:								jkey.keycode = JK_UNDERSCORE;					return jkey;


		/* For input method support on Asian Keyboards */
		//case XK_Cancel:								jkey.keycode = JK_FINAL;						return jkey;	//Unknown X11 Support
		//case XK_Henkan:								jkey.keycode = JK_CONVERT;						return jkey;	//Unknown X11 Support
		//case XK_Muhenkan:								jkey.keycode = JK_NONCONVERT;					return jkey;	//Unknown X11 Support
		//case XK_VoidSymbol:							jkey.keycode = JK_ACCEPT;						return jkey;	//Unknown X11 Support
		#ifndef SUN_KEYBOARD
		case XK_Mode_switch:							jkey.keycode = JK_MODECHANGE;					return jkey;	//TODO Testing Needed
		#endif
		case XK_Kana_Shift:								jkey.keycode = JK_KANA;							return jkey;	//TODO Testing Needed
		case XK_Kanji:									jkey.keycode = JK_KANJI;						return jkey;	//TODO Testing Needed
		case XK_Eisu_Shift:								jkey.keycode = JK_ALPHANUMERIC;					return jkey;	//TODO Testing Needed
		case XK_Katakana:								jkey.keycode = JK_KATAKANA;						return jkey;	//TODO Testing Needed
		case XK_Hiragana:								jkey.keycode = JK_HIRAGANA;						return jkey;	//TODO Testing Needed
		case XK_Zenkaku:								jkey.keycode = JK_FULL_WIDTH;					return jkey;	//TODO Testing Needed
		case XK_Hankaku:								jkey.keycode = JK_HALF_WIDTH;					return jkey;	//TODO Testing Needed
		case XK_Romaji:									jkey.keycode = JK_ROMAN_CHARACTERS;				return jkey;	//TODO Testing Needed
		case XK_MultipleCandidate:						jkey.keycode = JK_ALL_CANDIDATES;				return jkey;	//TODO Testing Needed
		case XK_PreviousCandidate:						jkey.keycode = JK_PREVIOUS_CANDIDATE;			return jkey;	//TODO Testing Needed
		case XK_Codeinput:								jkey.keycode = JK_CODE_INPUT;					return jkey;	//TODO Testing Needed
		case XK_Hiragana_Katakana:						jkey.keycode = JK_JAPANESE_KATAKANA;			return jkey;	//TODO Testing Needed
		case XK_Zenkaku_Hankaku:						jkey.keycode = JK_JAPANESE_HIRAGANA;			return jkey;	//TODO Testing Needed
		//case XK_Romaji:								jkey.keycode = JK_JAPANESE_ROMAN;				return jkey;	//TODO Testing Needed
		case XK_Kana_Lock:								jkey.keycode = JK_KANA_LOCK;					return jkey;	//TODO Testing Needed
		case XK_Eisu_toggle:							jkey.keycode = JK_INPUT_METHOD_ON_OFF;			return jkey;	//TODO Testing Needed

		/* For Sun keyboards */
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:									jkey.keycode = JK_CUT;							return jkey;
		case SunXK_Copy:								jkey.keycode = JK_COPY;							return jkey;
		case SunXK_Paste:								jkey.keycode = JK_PASTE;						return jkey;
		case SunXK_Undo:								jkey.keycode = JK_UNDO;							return jkey;
		case SunXK_Again:								jkey.keycode = JK_AGAIN;						return jkey;
		case SunXK_Find:								jkey.keycode = JK_FIND;							return jkey;
		case SunXK_Props:								jkey.keycode = JK_PROPS;						return jkey;
		case SunXK_Stop:								jkey.keycode = JK_STOP;							return jkey;
		case SunXK_Compose:								jkey.keycode = JK_COMPOSE;						return jkey;
		case SunXK_AltGraph:							jkey.keycode = JK_ALT_GRAPH;					return jkey;
		#endif

		case XK_Begin:									jkey.keycode = JK_BEGIN;						return jkey;

		default:
		case XK_VoidSymbol:								jkey.keycode = JK_UNDEFINED;					return jkey;
	}
}

jint NativeToJButton(unsigned int button) {
	switch (button) {
		case XK_Button1:								return JBUTTON1;
		case XK_Button2:								return JBUTTON3;
		case XK_Button3:								return JBUTTON2;
		case XK_Button4:								return JBUTTON4;
		case XK_Button5:								return JBUTTON5;
		default:
		case XK_NoButton:								return JNOBUTTON;
	}
}

jint NativeToJModifier(unsigned int modifier) {
	switch (modifier) {
		case KeyButMaskShift:							return JK_SHIFT_MASK;
		case KeyButMaskControl:							return JK_CTRL_MASK;
		case KeyButMaskMod4:							return JK_META_MASK;
		case KeyButMaskMod1:							return JK_ALT_MASK;

		case KeyButMaskButton1:							return JK_BUTTON1_MASK;
		case KeyButMaskButton2:							return JK_BUTTON3_MASK;
		case KeyButMaskButton3:							return JK_BUTTON2_MASK;
		case KeyButMaskButton4:							return JK_BUTTON4_MASK;
		case KeyButMaskButton5:							return JK_BUTTON5_MASK;

		default: 										return JK_UNDEFINED;
	}
}
