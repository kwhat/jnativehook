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

#ifdef SUN_KEYBOARD
#include <X11/Sunkeysym.h>
#endif

#include "NativeToJava.h"

JKeyDatum NativeToJKey(unsigned int keysym) {
	JKeyDatum jkey;
	jkey.rawcode = keysym;
	jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD;

	switch ((unsigned int) keysym) {
		case XK_Return:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ENTER;
			return jkey;
		case XK_BackSpace:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BACK_SPACE;
			return jkey;
		case XK_Tab:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_TAB;
			return jkey;
		#ifndef SUN_KEYBOARD
		case XK_Cancel:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CANCEL;
			return jkey;
		#endif


		case XK_Shift_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SHIFT;
			goto LOCATION_L;
		case XK_Control_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CONTROL;
			goto LOCATION_L;
		case XK_Alt_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ALT;
			goto LOCATION_L;
		case XK_Meta_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_META;
			goto LOCATION_L;
		case XK_Super_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_WINDOWS;
			goto LOCATION_L;
		LOCATION_L:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;
			return jkey;

		case XK_Shift_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SHIFT;
			goto LOCATION_R;
		case XK_Control_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CONTROL;
			goto LOCATION_R;
		case XK_Alt_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ALT;
			goto LOCATION_R;
		case XK_Meta_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_META;
			goto LOCATION_R;
		case XK_Super_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_WINDOWS;
			goto LOCATION_R;
		LOCATION_R:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;
			return jkey;


		case XK_Menu:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CONTEXT_MENU;
			return jkey;


		case XK_Pause:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PAUSE;
			return jkey;
		case XK_Caps_Lock:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CAPS_LOCK;
			return jkey;
		case XK_Escape:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ESCAPE;
			return jkey;
		case XK_space:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SPACE;
			return jkey;


		case XK_Up:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_UP;
			return jkey;
		case XK_Down:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DOWN;
			return jkey;
		case XK_Left:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_LEFT;
			return jkey;
		case XK_Right:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_RIGHT;
			return jkey;


		case XK_comma:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_COMMA;
			return jkey;
		case XK_minus:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_MINUS;
			return jkey;
		case XK_period:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PERIOD;
			return jkey;
		case XK_slash:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SLASH;
			return jkey;


		case XK_0:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_0;
			return jkey;
		case XK_1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_1;
			return jkey;
		case XK_2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_2;
			return jkey;
		case XK_3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_3;
			return jkey;
		case XK_4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_4;
			return jkey;
		case XK_5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_5;
			return jkey;
		case XK_6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_6;
			return jkey;
		case XK_7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_7;
			return jkey;
		case XK_8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_8;
			return jkey;
		case XK_9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_9;
			return jkey;


		case XK_semicolon:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SEMICOLON;
			return jkey;
		case XK_equal:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_EQUALS;
			return jkey;

		case XK_A:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_A;
			return jkey;
		case XK_B:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_B;
			return jkey;
		case XK_C:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_C;
			return jkey;
		case XK_D:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_D;
			return jkey;
		case XK_E:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_E;
			return jkey;
		case XK_F:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F;
			return jkey;
		case XK_G:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_G;
			return jkey;
		case XK_H:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_H;
			return jkey;
		case XK_I:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_I;
			return jkey;
		case XK_J:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_J;
			return jkey;
		case XK_K:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_K;
			return jkey;
		case XK_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_L;
			return jkey;
		case XK_M:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_M;
			return jkey;
		case XK_N:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_N;
			return jkey;
		case XK_O:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_O;
			return jkey;
		case XK_P:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_P;
			return jkey;
		case XK_Q:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_Q;
			return jkey;
		case XK_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_R;
			return jkey;
		case XK_S:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_S;
			return jkey;
		case XK_T:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_T;
			return jkey;
		case XK_U:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_U;
			return jkey;
		case XK_V:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_V;
			return jkey;
		case XK_W:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_W;
			return jkey;
		case XK_X:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_X;
			return jkey;
		case XK_Y:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_Y;
			return jkey;
		case XK_Z:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_Z;
			return jkey;

		case XK_bracketleft:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_OPEN_BRACKET;
			return jkey;
		case XK_backslash:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BACK_SLASH;
			return jkey;
		case XK_bracketright:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CLOSE_BRACKET;
			return jkey;

		case XK_KP_0:
		case XK_KP_Insert:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD0;
			goto LOCATION_KP;
		case XK_KP_1:
		case XK_KP_End:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD1;
			goto LOCATION_KP;
		case XK_KP_2:
		case XK_KP_Down:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD2;
			goto LOCATION_KP;
		case XK_KP_3:
		case XK_KP_Page_Down:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD3;
			goto LOCATION_KP;
		case XK_KP_4:
		case XK_KP_Left:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD4;
			goto LOCATION_KP;
		case XK_KP_5:
		case XK_KP_Begin:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD5;
			goto LOCATION_KP;
		case XK_KP_6:
		case XK_KP_Right:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD6;
			goto LOCATION_KP;
		case XK_KP_7:
		case XK_KP_Home:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD7;
			goto LOCATION_KP;
		case XK_KP_8:
		case XK_KP_Up:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD8;
			goto LOCATION_KP;
		case XK_KP_9:
		case XK_KP_Page_Up:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD9;
			goto LOCATION_KP;
		case XK_KP_Multiply:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_MULTIPLY;
			goto LOCATION_KP;
		case XK_KP_Add:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ADD;
			goto LOCATION_KP;
		case XK_KP_Separator:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SEPARATOR;
			goto LOCATION_KP;
		case XK_KP_Subtract:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SUBTRACT;
			goto LOCATION_KP;
		case XK_KP_Enter:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ENTER;
			goto LOCATION_KP;
		case XK_KP_Decimal:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DECIMAL;
			goto LOCATION_KP;
		case XK_KP_Divide:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DIVIDE;
			goto LOCATION_KP;
		case XK_KP_Delete:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DELETE;
			goto LOCATION_KP;
		case XK_Num_Lock:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUM_LOCK;
			goto LOCATION_KP;
		case XK_Clear:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CLEAR;
			goto LOCATION_KP;
		LOCATION_KP:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;
			return jkey;

		case XK_Scroll_Lock:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SCROLL_LOCK;
			return jkey;

		/* Function Keys */
		case XK_F1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F1;
			return jkey;
		case XK_F2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F2;
			return jkey;
		case XK_F3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F3;
			return jkey;
		case XK_F4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F4;
			return jkey;
		case XK_F5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F5;
			return jkey;
		case XK_F6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F6;
			return jkey;
		case XK_F7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F7;
			return jkey;
		case XK_F8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F8;
			return jkey;
		case XK_F9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F9;
			return jkey;
		case XK_F10:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F10;
			return jkey;
		case XK_F11:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F11;
			return jkey;
		case XK_F12:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F12;
			return jkey;

		/* Extended Function Keys */
		case XK_F13:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F13;
			return jkey;
		case XK_F14:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F14;
			return jkey;
		case XK_F15:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F15;
			return jkey;
		case XK_F16:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F16;
			return jkey;
		case XK_F17:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F17;
			return jkey;
		case XK_F18:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F18;
			return jkey;
		case XK_F19:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F19;
			return jkey;
		case XK_F20:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F20;
			return jkey;
		case XK_F21:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F21;
			return jkey;
		case XK_F22:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F22;
			return jkey;
		case XK_F23:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F23;
			return jkey;
		case XK_F24:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F24;
			return jkey;

		case XK_Print:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PRINTSCREEN;
			return jkey;
		case XK_Insert:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_INSERT;
			return jkey;
		case XK_Delete:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DELETE;
			return jkey;
		case XK_Help:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_HELP;
			return jkey;

		case XK_Page_Up:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PAGE_UP;
			return jkey;
		case XK_Page_Down:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PAGE_DOWN;
			return jkey;
		case XK_Home:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_HOME;
			return jkey;
		case XK_End:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_END;
			return jkey;

		case XK_apostrophe:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_QUOTE;
			return jkey;
		case XK_grave:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BACK_QUOTE;
			return jkey;

		/* For European keyboards */
		case XK_dead_grave:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_GRAVE;
			return jkey;
		case XK_dead_acute:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_ACUTE;
			return jkey;
		case XK_dead_circumflex:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_CIRCUMFLEX;
			return jkey;
		case XK_dead_tilde:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_TILDE;
			return jkey;
		case XK_dead_macron:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_MACRON;
			return jkey;
		case XK_dead_breve:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_BREVE;
			return jkey;
		case XK_dead_abovedot:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_ABOVEDOT;
			return jkey;
		case XK_dead_diaeresis:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_DIAERESIS;
			return jkey;
		case XK_dead_abovering:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_ABOVERING;
			return jkey;
		case XK_dead_doubleacute:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_DOUBLEACUTE;
			return jkey;
		case XK_dead_caron:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_CARON;
			return jkey;
		case XK_dead_cedilla:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_CEDILLA;
			return jkey;
		case XK_dead_ogonek:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_OGONEK;
			return jkey;
		case XK_dead_iota:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_IOTA;
			return jkey;
		case XK_dead_voiced_sound:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_VOICED_SOUND;
			return jkey;
		case XK_dead_semivoiced_sound:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_SEMIVOICED_SOUND;
			return jkey;

		/* Unknown Keyboard Codes */
		case XK_ampersand:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_AMPERSAND;
			return jkey;
		case XK_asterisk:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ASTERISK;
			return jkey;
		case XK_quotedbl:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_QUOTEDBL;
			return jkey;
		case XK_less:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_LESS;
			return jkey;
		case XK_greater:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_GREATER;
			return jkey;
		case XK_braceleft:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BRACELEFT;
			return jkey;
		case XK_braceright:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BRACERIGHT;
			return jkey;

		/* Unknown Extended Keyboard Codes */
		case XK_at:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_AT;
			return jkey;
		case XK_colon:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_COLON;
			return jkey;
		case XK_asciicircum:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CIRCUMFLEX;
			return jkey;
		case XK_dollar:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DOLLAR;
			return jkey;
		case XK_EuroSign:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_EURO_SIGN;
			return jkey;
		case XK_exclam:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_EXCLAMATION_MARK;
			return jkey;
		case XK_exclamdown:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_INVERTED_EXCLAMATION_MARK;
			return jkey;
		case XK_parenleft:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_LEFT_PARENTHESIS;
			return jkey;
		case XK_numbersign:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMBER_SIGN;
			return jkey;
		case XK_plus:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PLUS;
			return jkey;
		case XK_parenright:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_RIGHT_PARENTHESIS;
			return jkey;
		case XK_underscore:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_UNDERSCORE;
			return jkey;


		/* For input method support on Asian Keyboards */
		/*
		case XK_Cancel:					//Unknown X11 Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_FINAL;
			return jkey;
		case XK_Henkan:					//Unknown X11 Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CONVERT;
			return jkey;
		case XK_Muhenkan:				//Unknown X11 Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NONCONVERT;
			return jkey;
		case XK_VoidSymbol:				//Unknown X11 Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ACCEPT;
			return jkey;
		*/
		#ifndef SUN_KEYBOARD
		case XK_Mode_switch:			//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_MODECHANGE;
			return jkey;
		#endif
		case XK_Kana_Shift:				//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_KANA;
			return jkey;
		case XK_Kanji:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_KANJI;
			return jkey;
		case XK_Eisu_Shift:				//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ALPHANUMERIC;
			return jkey;
		case XK_Katakana:				//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_KATAKANA;
			return jkey;
		case XK_Hiragana:				//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_HIRAGANA;
			return jkey;
		case XK_Zenkaku:				//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_FULL_WIDTH;
			return jkey;
		case XK_Hankaku:				//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_HALF_WIDTH;
			return jkey;
		case XK_Romaji:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ROMAN_CHARACTERS;
			return jkey;
		case XK_MultipleCandidate:		//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ALL_CANDIDATES;
			return jkey;
		case XK_PreviousCandidate:		//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PREVIOUS_CANDIDATE;
			return jkey;
		case XK_Codeinput:				//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CODE_INPUT;
			return jkey;
		case XK_Hiragana_Katakana:		//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_JAPANESE_KATAKANA;
			return jkey;
		case XK_Zenkaku_Hankaku:		//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_JAPANESE_HIRAGANA;
			return jkey;
		//case XK_Romaji:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_JAPANESE_ROMAN;				return jkey;	//TODO Testing Needed
		case XK_Kana_Lock:				//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_KANA_LOCK;
			return jkey;
		case XK_Eisu_toggle:			//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_INPUT_METHOD_ON_OFF;
			return jkey;

		/* For Sun keyboards */
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CUT;
			return jkey;
		case SunXK_Copy:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_COPY;
			return jkey;
		case SunXK_Paste:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PASTE;
			return jkey;
		case SunXK_Undo:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_UNDO;
			return jkey;
		case SunXK_Again:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_AGAIN;
			return jkey;
		case SunXK_Find:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_FIND;
			return jkey;
		case SunXK_Props:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PROPS;
			return jkey;
		case SunXK_Stop:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_STOP;
			return jkey;
		case SunXK_Compose:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_COMPOSE;
			return jkey;
		case SunXK_AltGraph:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ALT_GRAPH;
			return jkey;
		#endif

		case XK_Begin:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BEGIN;
			return jkey;

		default:
		case XK_VoidSymbol:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_UNDEFINED;
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_UNKNOWN;
			return jkey;
	}
}

jint NativeToJButton(unsigned int button) {
	switch (button) {
		case Button1:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON1;
		case Button2:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON3;
		case Button3:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON2;
		case Button4:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON4;
		case Button5:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON5;

		default:
		case AnyButton:
			return org_jnativehook_mouse_NativeMouseEvent_NOBUTTON;
	}
}

jint NativeToJModifier(unsigned int modifier) {
	switch (modifier) {
		case ShiftMask:
			return org_jnativehook_NativeInputEvent_SHIFT_MASK;
		case ControlMask:
			return org_jnativehook_NativeInputEvent_CTRL_MASK;
		case Mod4Mask:
			return org_jnativehook_NativeInputEvent_META_MASK;
		case Mod1Mask:
			return org_jnativehook_NativeInputEvent_ALT_MASK;

		case Button1Mask:
			return org_jnativehook_NativeInputEvent_BUTTON1_MASK;
		case Button2Mask:
			return org_jnativehook_NativeInputEvent_BUTTON3_MASK;
		case Button3Mask:
			return org_jnativehook_NativeInputEvent_BUTTON2_MASK;
		case Button4Mask:
			return org_jnativehook_NativeInputEvent_BUTTON4_MASK;
		case Button5Mask:
			return org_jnativehook_NativeInputEvent_BUTTON5_MASK;

		default:
			return org_jnativehook_keyboard_NativeKeyEvent_UNDEFINED;
	}
}
