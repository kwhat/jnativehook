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

#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#ifdef SUN_KEYBOARD
#include <X11/Sunkeysym.h>
#endif

#include "virtual_key_codes.h"
#include "virtual_modifer_masks.h"
#include "virtual_mouse_buttons.h"

uint_fast16_t convert_to_virtual_keycode(uint_fast16_t native_keycode) {
	VirtualKeyEvent event;
	event.rawcode = native_keycode;
	event.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD;

	switch (native_keycode) {
		case XK_Return:
			event.keycode = VC_ENTER;
			return event;
		case XK_BackSpace:
			event.keycode = VC_BACK_SPACE;
			return event;
		case XK_Tab:
			event.keycode = VC_TAB;
			return event;
		#ifndef SUN_KEYBOARD
		case XK_Cancel:
			event.keycode = VC_CANCEL;
			return event;
		#endif


		case XK_Shift_L:
			event.keycode = VC_SHIFT;
			goto LOCATION_L;
		case XK_Control_L:
			event.keycode = VC_CONTROL;
			goto LOCATION_L;
		case XK_Alt_L:
			event.keycode = VC_ALT;
			goto LOCATION_L;
		case XK_Meta_L:
			event.keycode = VC_META;
			goto LOCATION_L;
		case XK_Super_L:
			event.keycode = VC_WINDOWS;
			goto LOCATION_L;
		LOCATION_L:
			event.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;
			return event;

		case XK_Shift_R:
			event.keycode = VC_SHIFT;
			goto LOCATION_R;
		case XK_Control_R:
			event.keycode = VC_CONTROL;
			goto LOCATION_R;
		case XK_Alt_R:
			event.keycode = VC_ALT;
			goto LOCATION_R;
		case XK_Meta_R:
			event.keycode = VC_META;
			goto LOCATION_R;
		case XK_Super_R:
			event.keycode = VC_WINDOWS;
			goto LOCATION_R;
		LOCATION_R:
			event.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;
			return event;


		case XK_Menu:
			event.keycode = VC_CONTEXT_MENU;
			return event;


		case XK_Pause:
			event.keycode = VC_PAUSE;
			return event;
		case XK_Caps_Lock:
			event.keycode = VC_CAPS_LOCK;
			return event;
		case XK_Escape:
			event.keycode = VC_ESCAPE;
			return event;
		case XK_space:
			event.keycode = VC_SPACE;
			return event;


		case XK_Up:
			event.keycode = VC_UP;
			return event;
		case XK_Down:
			event.keycode = VC_DOWN;
			return event;
		case XK_Left:
			event.keycode = VC_LEFT;
			return event;
		case XK_Right:
			event.keycode = VC_RIGHT;
			return event;


		case XK_comma:
			event.keycode = VC_COMMA;
			return event;
		case XK_minus:
			event.keycode = VC_MINUS;
			return event;
		case XK_period:
			event.keycode = VC_PERIOD;
			return event;
		case XK_slash:
			event.keycode = VC_SLASH;
			return event;


		case XK_0:
			event.keycode = VC_0;
			return event;
		case XK_1:
			event.keycode = VC_1;
			return event;
		case XK_2:
			event.keycode = VC_2;
			return event;
		case XK_3:
			event.keycode = VC_3;
			return event;
		case XK_4:
			event.keycode = VC_4;
			return event;
		case XK_5:
			event.keycode = VC_5;
			return event;
		case XK_6:
			event.keycode = VC_6;
			return event;
		case XK_7:
			event.keycode = VC_7;
			return event;
		case XK_8:
			event.keycode = VC_8;
			return event;
		case XK_9:
			event.keycode = VC_9;
			return event;


		case XK_semicolon:
			event.keycode = VC_SEMICOLON;
			return event;
		case XK_equal:
			event.keycode = VC_EQUALS;
			return event;

		case XK_a:
		case XK_A:
			event.keycode = VC_A;
			return event;
		case XK_b:
		case XK_B:
			event.keycode = VC_B;
			return event;
		case XK_c:
		case XK_C:
			event.keycode = VC_C;
			return event;
		case XK_d:
		case XK_D:
			event.keycode = VC_D;
			return event;
		case XK_e:
		case XK_E:
			event.keycode = VC_E;
			return event;
		case XK_f:
		case XK_F:
			event.keycode = VC_F;
			return event;
		case XK_g:
		case XK_G:
			event.keycode = VC_G;
			return event;
		case XK_h:
		case XK_H:
			event.keycode = VC_H;
			return event;
		case XK_i:
		case XK_I:
			event.keycode = VC_I;
			return event;
		case XK_j:
		case XK_J:
			event.keycode = VC_J;
			return event;
		case XK_k:
		case XK_K:
			event.keycode = VC_K;
			return event;
		case XK_l:
		case XK_L:
			event.keycode = VC_L;
			return event;
		case XK_m:
		case XK_M:
			event.keycode = VC_M;
			return event;
		case XK_n:
		case XK_N:
			event.keycode = VC_N;
			return event;
		case XK_o:
		case XK_O:
			event.keycode = VC_O;
			return event;
		case XK_p:
		case XK_P:
			event.keycode = VC_P;
			return event;
		case XK_q:
		case XK_Q:
			event.keycode = VC_Q;
			return event;
		case XK_r:
		case XK_R:
			event.keycode = VC_R;
			return event;
		case XK_s:
		case XK_S:
			event.keycode = VC_S;
			return event;
		case XK_t:
		case XK_T:
			event.keycode = VC_T;
			return event;
		case XK_u:
		case XK_U:
			event.keycode = VC_U;
			return event;
		case XK_v:
		case XK_V:
			event.keycode = VC_V;
			return event;
		case XK_w:
		case XK_W:
			event.keycode = VC_W;
			return event;
		case XK_x:
		case XK_X:
			event.keycode = VC_X;
			return event;
		case XK_y:
		case XK_Y:
			event.keycode = VC_Y;
			return event;
		case XK_z:
		case XK_Z:
			event.keycode = VC_Z;
			return event;

		case XK_bracketleft:
			event.keycode = VC_OPEN_BRACKET;
			return event;
		case XK_backslash:
			event.keycode = VC_BACK_SLASH;
			return event;
		case XK_bracketright:
			event.keycode = VC_CLOSE_BRACKET;
			return event;

		case XK_KP_0:
		case XK_KP_Insert:
			event.keycode = VC_NUMPAD0;
			goto LOCATION_NP;
		case XK_KP_1:
		case XK_KP_End:
			event.keycode = VC_NUMPAD1;
			goto LOCATION_NP;
		case XK_KP_2:
		case XK_KP_Down:
			event.keycode = VC_NUMPAD2;
			goto LOCATION_NP;
		case XK_KP_3:
		case XK_KP_Page_Down:
			event.keycode = VC_NUMPAD3;
			goto LOCATION_NP;
		case XK_KP_4:
		case XK_KP_Left:
			event.keycode = VC_NUMPAD4;
			goto LOCATION_NP;
		case XK_KP_5:
		case XK_KP_Begin:
			event.keycode = VC_NUMPAD5;
			goto LOCATION_NP;
		case XK_KP_6:
		case XK_KP_Right:
			event.keycode = VC_NUMPAD6;
			goto LOCATION_NP;
		case XK_KP_7:
		case XK_KP_Home:
			event.keycode = VC_NUMPAD7;
			goto LOCATION_NP;
		case XK_KP_8:
		case XK_KP_Up:
			event.keycode = VC_NUMPAD8;
			goto LOCATION_NP;
		case XK_KP_9:
		case XK_KP_Page_Up:
			event.keycode = VC_NUMPAD9;
			goto LOCATION_NP;

		case XK_KP_Multiply:
			event.keycode = VC_MULTIPLY;
			goto LOCATION_NP;
		case XK_KP_Add:
			event.keycode = VC_ADD;
			goto LOCATION_NP;
		case XK_KP_Separator:
			event.keycode = VC_SEPARATOR;
			goto LOCATION_NP;
		case XK_KP_Subtract:
			event.keycode = VC_SUBTRACT;
			goto LOCATION_NP;
		case XK_KP_Enter:
			event.keycode = VC_ENTER;
			goto LOCATION_NP;
		case XK_KP_Decimal:
			event.keycode = VC_DECIMAL;
			goto LOCATION_NP;
		case XK_KP_Divide:
			event.keycode = VC_DIVIDE;
			goto LOCATION_NP;
		case XK_KP_Delete:
			event.keycode = VC_DELETE;
			goto LOCATION_NP;
		case XK_Num_Lock:
			event.keycode = VC_NUM_LOCK;
			goto LOCATION_NP;
		case XK_Clear:
			event.keycode = VC_CLEAR;
			goto LOCATION_NP;
		LOCATION_NP:
			event.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;
			return event;

		case XK_Scroll_Lock:
			event.keycode = VC_SCROLL_LOCK;
			return event;

		// Function Keys
		case XK_F1:
			event.keycode = VC_F1;
			return event;
		case XK_F2:
			event.keycode = VC_F2;
			return event;
		case XK_F3:
			event.keycode = VC_F3;
			return event;
		case XK_F4:
			event.keycode = VC_F4;
			return event;
		case XK_F5:
			event.keycode = VC_F5;
			return event;
		case XK_F6:
			event.keycode = VC_F6;
			return event;
		case XK_F7:
			event.keycode = VC_F7;
			return event;
		case XK_F8:
			event.keycode = VC_F8;
			return event;
		case XK_F9:
			event.keycode = VC_F9;
			return event;
		case XK_F10:
			event.keycode = VC_F10;
			return event;
		case XK_F11:
			event.keycode = VC_F11;
			return event;
		case XK_F12:
			event.keycode = VC_F12;
			return event;

		// Extended Function Keys
		case XK_F13:
			event.keycode = VC_F13;
			return event;
		case XK_F14:
			event.keycode = VC_F14;
			return event;
		case XK_F15:
			event.keycode = VC_F15;
			return event;
		case XK_F16:
			event.keycode = VC_F16;
			return event;
		case XK_F17:
			event.keycode = VC_F17;
			return event;
		case XK_F18:
			event.keycode = VC_F18;
			return event;
		case XK_F19:
			event.keycode = VC_F19;
			return event;
		case XK_F20:
			event.keycode = VC_F20;
			return event;
		case XK_F21:
			event.keycode = VC_F21;
			return event;
		case XK_F22:
			event.keycode = VC_F22;
			return event;
		case XK_F23:
			event.keycode = VC_F23;
			return event;
		case XK_F24:
			event.keycode = VC_F24;
			return event;

		case XK_Print:
			event.keycode = VC_PRINTSCREEN;
			return event;
		case XK_Insert:
			event.keycode = VC_INSERT;
			return event;
		case XK_Delete:
			event.keycode = VC_DELETE;
			return event;
		case XK_Help:
			event.keycode = VC_HELP;
			return event;

		case XK_Page_Up:
			event.keycode = VC_PAGE_UP;
			return event;
		case XK_Page_Down:
			event.keycode = VC_PAGE_DOWN;
			return event;
		case XK_Home:
			event.keycode = VC_HOME;
			return event;
		case XK_End:
			event.keycode = VC_END;
			return event;

		case XK_apostrophe:
			event.keycode = VC_QUOTE;
			return event;
		case XK_grave:
			event.keycode = VC_BACK_QUOTE;
			return event;

		// For European keyboards
		case XK_dead_grave:
			event.keycode = VC_DEAD_GRAVE;
			return event;
		case XK_dead_acute:
			event.keycode = VC_DEAD_ACUTE;
			return event;
		case XK_dead_circumflex:
			event.keycode = VC_DEAD_CIRCUMFLEX;
			return event;
		case XK_dead_tilde:
			event.keycode = VC_DEAD_TILDE;
			return event;
		case XK_dead_macron:
			event.keycode = VC_DEAD_MACRON;
			return event;
		case XK_dead_breve:
			event.keycode = VC_DEAD_BREVE;
			return event;
		case XK_dead_abovedot:
			event.keycode = VC_DEAD_ABOVEDOT;
			return event;
		case XK_dead_diaeresis:
			event.keycode = VC_DEAD_DIAERESIS;
			return event;
		case XK_dead_abovering:
			event.keycode = VC_DEAD_ABOVERING;
			return event;
		case XK_dead_doubleacute:
			event.keycode = VC_DEAD_DOUBLEACUTE;
			return event;
		case XK_dead_caron:
			event.keycode = VC_DEAD_CARON;
			return event;
		case XK_dead_cedilla:
			event.keycode = VC_DEAD_CEDILLA;
			return event;
		case XK_dead_ogonek:
			event.keycode = VC_DEAD_OGONEK;
			return event;
		case XK_dead_iota:
			event.keycode = VC_DEAD_IOTA;
			return event;
		case XK_dead_voiced_sound:
			event.keycode = VC_DEAD_VOICED_SOUND;
			return event;
		case XK_dead_semivoiced_sound:
			event.keycode = VC_DEAD_SEMIVOICED_SOUND;
			return event;

		// Unknown Keyboard Codes
		case XK_ampersand:
			event.keycode = VC_AMPERSAND;
			return event;
		case XK_asterisk:
			event.keycode = VC_ASTERISK;
			return event;
		case XK_quotedbl:
			event.keycode = VC_QUOTEDBL;
			return event;
		case XK_less:
			event.keycode = VC_LESS;
			return event;
		case XK_greater:
			event.keycode = VC_GREATER;
			return event;
		case XK_braceleft:
			event.keycode = VC_BRACELEFT;
			return event;
		case XK_braceright:
			event.keycode = VC_BRACERIGHT;
			return event;

		// Unknown Extended Keyboard Codes
		case XK_at:
			event.keycode = VC_AT;
			return event;
		case XK_colon:
			event.keycode = VC_COLON;
			return event;
		case XK_asciicircum:
			event.keycode = VC_CIRCUMFLEX;
			return event;
		case XK_dollar:
			event.keycode = VC_DOLLAR;
			return event;
		case XK_EuroSign:
			event.keycode = VC_EURO_SIGN;
			return event;
		case XK_exclam:
			event.keycode = VC_EXCLAMATION_MARK;
			return event;
		case XK_exclamdown:
			event.keycode = VC_INVERTED_EXCLAMATION_MARK;
			return event;
		case XK_parenleft:
			event.keycode = VC_LEFT_PARENTHESIS;
			return event;
		case XK_numbersign:
			event.keycode = VC_NUMBER_SIGN;
			return event;
		case XK_plus:
			event.keycode = VC_PLUS;
			return event;
		case XK_parenright:
			event.keycode = VC_RIGHT_PARENTHESIS;
			return event;
		case XK_underscore:
			event.keycode = VC_UNDERSCORE;
			return event;


		// For input method support on Asian Keyboards.
		/*
		case XK_Cancel:					// Unknown X11 Support
			keyevent.keycode = VC_FINAL;
			return keyevent;
		case XK_Henkan:					// Unknown X11 Support
			keyevent.keycode = VC_CONVERT;
			return keyevent;
		case XK_Muhenkan:				// Unknown X11 Support
			keyevent.keycode = VC_NONCONVERT;
			return keyevent;
		case XK_VoidSymbol:				// Unknown X11 Support
			keyevent.keycode = VC_ACCEPT;
			return keyevent;
		*/
		#ifndef SUN_KEYBOARD
		case XK_Mode_switch:			// TODO Testing Needed.
			event.keycode = VC_MODECHANGE;
			return event;
		#endif
		case XK_Kana_Shift:				// TODO Testing Needed.
			event.keycode = VC_KANA;
			return event;
		case XK_Kanji:					// TODO Testing Needed.
			event.keycode = VC_KANJI;
			return event;
		case XK_Eisu_Shift:				// TODO Testing Needed.
			event.keycode = VC_ALPHANUMERIC;
			return event;
		case XK_Katakana:				// TODO Testing Needed.
			event.keycode = VC_KATAKANA;
			return event;
		case XK_Hiragana:				// TODO Testing Needed.
			event.keycode = VC_HIRAGANA;
			return event;
		case XK_Zenkaku:				// TODO Testing Needed.
			event.keycode = VC_FULL_WIDTH;
			return event;
		case XK_Hankaku:				// TODO Testing Needed.
			event.keycode = VC_HALF_WIDTH;
			return event;
		case XK_Romaji:					// TODO Testing Needed.
			event.keycode = VC_ROMAN_CHARACTERS;
			return event;
		case XK_MultipleCandidate:		// TODO Testing Needed.
			event.keycode = VC_ALL_CANDIDATES;
			return event;
		case XK_PreviousCandidate:		// TODO Testing Needed.
			event.keycode = VC_PREVIOUS_CANDIDATE;
			return event;
		case XK_Codeinput:				// TODO Testing Needed.
			event.keycode = VC_CODE_INPUT;
			return event;
		case XK_Hiragana_Katakana:		// TODO Testing Needed.
			event.keycode = VC_JAPANESE_KATAKANA;
			return event;
		case XK_Zenkaku_Hankaku:		// TODO Testing Needed.
			event.keycode = VC_JAPANESE_HIRAGANA;
			return event;
		/* case XK_Romaji:				// TODO Testing Needed
			keyevent.keycode = VC_JAPANESE_ROMAN;
			return keyevent; */
		case XK_Kana_Lock:				// TODO Testing Needed.
			event.keycode = VC_KANA_LOCK;
			return event;
		case XK_Eisu_toggle:			// TODO Testing Needed.
			event.keycode = VC_INPUT_METHOD_ON_OFF;
			return event;

		// For Sun keyboards.
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:
			event.keycode = VC_CUT;
			return event;
		case SunXK_Copy:
			event.keycode = VC_COPY;
			return event;
		case SunXK_Paste:
			event.keycode = VC_PASTE;
			return event;
		case SunXK_Undo:
			event.keycode = VC_UNDO;
			return event;
		case SunXK_Again:
			event.keycode = VC_AGAIN;
			return event;
		case SunXK_Find:
			event.keycode = VC_FIND;
			return event;
		case SunXK_Props:
			event.keycode = VC_PROPS;
			return event;
		case SunXK_Stop:
			event.keycode = VC_STOP;
			return event;
		case SunXK_Compose:
			event.keycode = VC_COMPOSE;
			return event;
		case SunXK_AltGraph:
			event.keycode = VC_ALT_GRAPH;
			return event;
		#endif

		case XK_Begin:
			event.keycode = VC_BEGIN;
			return event;

		default:
		case XK_VoidSymbol:
			event.keycode = VC_UNDEFINED;
			event.location = NH_KEY_LOCATION_UNKNOWN;
			return event;
	}
}

uint_fast8_t convert_to_virtual_button(uint_fast8_t native_button) {
	uint_fast8_t virtual_button;
	
	switch (native_button) {
		case Button1:
			virtual_button = NH_BUTTON1;
		case Button2:
			virtual_button = NH_BUTTON3;
		case Button3:
			virtual_button = NH_BUTTON2;
		case Button4:
			virtual_button = NH_BUTTON4;
		case Button5:
			virtual_button = NH_BUTTON5;

		default:
		case AnyButton:
			virtual_button = VC_NOBUTTON;
	}
	
	return virtual_button;
}

uint_fast16_t convert_to_virtual_mask(uint_fast16_t native_mask) {
	uint_fast16_t virtual_mask = 0x0000;

	if (native_mask & ShiftMask)	virtual_mask |= SHIFT_MASK;
	if (native_mask & ControlMask)	virtual_mask |= CTRL_MASK;
	if (native_mask & Mod4Mask)		virtual_mask |= META_MASK;
	if (native_mask & Mod1Mask)		virtual_mask |= ALT_MASK;

	if (native_mask & Button1Mask)	virtual_mask |= BUTTON1_MASK;
	if (native_mask & Button2Mask)	virtual_mask |= BUTTON2_MASK;
	if (native_mask & Button3Mask)	virtual_mask |= BUTTON3_MASK;
	if (native_mask & Button4Mask)	virtual_mask |= BUTTON4_MASK;
	if (native_mask & Button5Mask)	virtual_mask |= BUTTON5_MASK;

	return virtual_mask;
}
