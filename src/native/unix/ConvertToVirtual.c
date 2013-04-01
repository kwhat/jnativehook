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
			break;
		case XK_BackSpace:
			event.keycode = VC_BACK_SPACE;
			break;
		case XK_Tab:
			event.keycode = VC_TAB;
			break;
		#ifndef SUN_KEYBOARD
		case XK_Cancel:
			event.keycode = VC_CANCEL;
			break;
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
			break;

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
			break;


		case XK_Menu:
			event.keycode = VC_CONTEXT_MENU;
			break;


		case XK_Pause:
			event.keycode = VC_PAUSE;
			break;
		case XK_Caps_Lock:
			event.keycode = VC_CAPS_LOCK;
			break;
		case XK_Escape:
			event.keycode = VC_ESCAPE;
			break;
		case XK_space:
			event.keycode = VC_SPACE;
			break;


		case XK_Up:
			event.keycode = VC_UP;
			break;
		case XK_Down:
			event.keycode = VC_DOWN;
			break;
		case XK_Left:
			event.keycode = VC_LEFT;
			break;
		case XK_Right:
			event.keycode = VC_RIGHT;
			break;


		case XK_comma:
			event.keycode = VC_COMMA;
			break;
		case XK_minus:
			event.keycode = VC_MINUS;
			break;
		case XK_period:
			event.keycode = VC_PERIOD;
			break;
		case XK_slash:
			event.keycode = VC_SLASH;
			break;


		case XK_0:
			event.keycode = VC_0;
			break;
		case XK_1:
			event.keycode = VC_1;
			break;
		case XK_2:
			event.keycode = VC_2;
			break;
		case XK_3:
			event.keycode = VC_3;
			break;
		case XK_4:
			event.keycode = VC_4;
			break;
		case XK_5:
			event.keycode = VC_5;
			break;
		case XK_6:
			event.keycode = VC_6;
			break;
		case XK_7:
			event.keycode = VC_7;
			break;
		case XK_8:
			event.keycode = VC_8;
			break;
		case XK_9:
			event.keycode = VC_9;
			break;


		case XK_semicolon:
			event.keycode = VC_SEMICOLON;
			break;
		case XK_equal:
			event.keycode = VC_EQUALS;
			break;

		case XK_a:
		case XK_A:
			event.keycode = VC_A;
			break;
		case XK_b:
		case XK_B:
			event.keycode = VC_B;
			break;
		case XK_c:
		case XK_C:
			event.keycode = VC_C;
			break;
		case XK_d:
		case XK_D:
			event.keycode = VC_D;
			break;
		case XK_e:
		case XK_E:
			event.keycode = VC_E;
			break;
		case XK_f:
		case XK_F:
			event.keycode = VC_F;
			break;
		case XK_g:
		case XK_G:
			event.keycode = VC_G;
			break;
		case XK_h:
		case XK_H:
			event.keycode = VC_H;
			break;
		case XK_i:
		case XK_I:
			event.keycode = VC_I;
			break;
		case XK_j:
		case XK_J:
			event.keycode = VC_J;
			break;
		case XK_k:
		case XK_K:
			event.keycode = VC_K;
			break;
		case XK_l:
		case XK_L:
			event.keycode = VC_L;
			break;
		case XK_m:
		case XK_M:
			event.keycode = VC_M;
			break;
		case XK_n:
		case XK_N:
			event.keycode = VC_N;
			break;
		case XK_o:
		case XK_O:
			event.keycode = VC_O;
			break;
		case XK_p:
		case XK_P:
			event.keycode = VC_P;
			break;
		case XK_q:
		case XK_Q:
			event.keycode = VC_Q;
			break;
		case XK_r:
		case XK_R:
			event.keycode = VC_R;
			break;
		case XK_s:
		case XK_S:
			event.keycode = VC_S;
			break;
		case XK_t:
		case XK_T:
			event.keycode = VC_T;
			break;
		case XK_u:
		case XK_U:
			event.keycode = VC_U;
			break;
		case XK_v:
		case XK_V:
			event.keycode = VC_V;
			break;
		case XK_w:
		case XK_W:
			event.keycode = VC_W;
			break;
		case XK_x:
		case XK_X:
			event.keycode = VC_X;
			break;
		case XK_y:
		case XK_Y:
			event.keycode = VC_Y;
			break;
		case XK_z:
		case XK_Z:
			event.keycode = VC_Z;
			break;

		case XK_bracketleft:
			event.keycode = VC_OPEN_BRACKET;
			break;
		case XK_backslash:
			event.keycode = VC_BACK_SLASH;
			break;
		case XK_bracketright:
			event.keycode = VC_CLOSE_BRACKET;
			break;

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
			break;

		case XK_Scroll_Lock:
			event.keycode = VC_SCROLL_LOCK;
			break;

		// Function Keys
		case XK_F1:
			event.keycode = VC_F1;
			break;
		case XK_F2:
			event.keycode = VC_F2;
			break;
		case XK_F3:
			event.keycode = VC_F3;
			break;
		case XK_F4:
			event.keycode = VC_F4;
			break;
		case XK_F5:
			event.keycode = VC_F5;
			break;
		case XK_F6:
			event.keycode = VC_F6;
			break;
		case XK_F7:
			event.keycode = VC_F7;
			break;
		case XK_F8:
			event.keycode = VC_F8;
			break;
		case XK_F9:
			event.keycode = VC_F9;
			break;
		case XK_F10:
			event.keycode = VC_F10;
			break;
		case XK_F11:
			event.keycode = VC_F11;
			break;
		case XK_F12:
			event.keycode = VC_F12;
			break;

		// Extended Function Keys
		case XK_F13:
			event.keycode = VC_F13;
			break;
		case XK_F14:
			event.keycode = VC_F14;
			break;
		case XK_F15:
			event.keycode = VC_F15;
			break;
		case XK_F16:
			event.keycode = VC_F16;
			break;
		case XK_F17:
			event.keycode = VC_F17;
			break;
		case XK_F18:
			event.keycode = VC_F18;
			break;
		case XK_F19:
			event.keycode = VC_F19;
			break;
		case XK_F20:
			event.keycode = VC_F20;
			break;
		case XK_F21:
			event.keycode = VC_F21;
			break;
		case XK_F22:
			event.keycode = VC_F22;
			break;
		case XK_F23:
			event.keycode = VC_F23;
			break;
		case XK_F24:
			event.keycode = VC_F24;
			break;

		case XK_Print:
			event.keycode = VC_PRINTSCREEN;
			break;
		case XK_Insert:
			event.keycode = VC_INSERT;
			break;
		case XK_Delete:
			event.keycode = VC_DELETE;
			break;
		case XK_Help:
			event.keycode = VC_HELP;
			break;

		case XK_Page_Up:
			event.keycode = VC_PAGE_UP;
			break;
		case XK_Page_Down:
			event.keycode = VC_PAGE_DOWN;
			break;
		case XK_Home:
			event.keycode = VC_HOME;
			break;
		case XK_End:
			event.keycode = VC_END;
			break;

		case XK_apostrophe:
			event.keycode = VC_QUOTE;
			break;
		case XK_grave:
			event.keycode = VC_BACK_QUOTE;
			break;

		// For European keyboards
		case XK_dead_grave:
			event.keycode = VC_DEAD_GRAVE;
			break;
		case XK_dead_acute:
			event.keycode = VC_DEAD_ACUTE;
			break;
		case XK_dead_circumflex:
			event.keycode = VC_DEAD_CIRCUMFLEX;
			break;
		case XK_dead_tilde:
			event.keycode = VC_DEAD_TILDE;
			break;
		case XK_dead_macron:
			event.keycode = VC_DEAD_MACRON;
			break;
		case XK_dead_breve:
			event.keycode = VC_DEAD_BREVE;
			break;
		case XK_dead_abovedot:
			event.keycode = VC_DEAD_ABOVEDOT;
			break;
		case XK_dead_diaeresis:
			event.keycode = VC_DEAD_DIAERESIS;
			break;
		case XK_dead_abovering:
			event.keycode = VC_DEAD_ABOVERING;
			break;
		case XK_dead_doubleacute:
			event.keycode = VC_DEAD_DOUBLEACUTE;
			break;
		case XK_dead_caron:
			event.keycode = VC_DEAD_CARON;
			break;
		case XK_dead_cedilla:
			event.keycode = VC_DEAD_CEDILLA;
			break;
		case XK_dead_ogonek:
			event.keycode = VC_DEAD_OGONEK;
			break;
		case XK_dead_iota:
			event.keycode = VC_DEAD_IOTA;
			break;
		case XK_dead_voiced_sound:
			event.keycode = VC_DEAD_VOICED_SOUND;
			break;
		case XK_dead_semivoiced_sound:
			event.keycode = VC_DEAD_SEMIVOICED_SOUND;
			break;

		// Unknown Keyboard Codes
		case XK_ampersand:
			event.keycode = VC_AMPERSAND;
			break;
		case XK_asterisk:
			event.keycode = VC_ASTERISK;
			break;
		case XK_quotedbl:
			event.keycode = VC_QUOTEDBL;
			break;
		case XK_less:
			event.keycode = VC_LESS;
			break;
		case XK_greater:
			event.keycode = VC_GREATER;
			break;
		case XK_braceleft:
			event.keycode = VC_BRACELEFT;
			break;
		case XK_braceright:
			event.keycode = VC_BRACERIGHT;
			break;

		// Unknown Extended Keyboard Codes
		case XK_at:
			event.keycode = VC_AT;
			break;
		case XK_colon:
			event.keycode = VC_COLON;
			break;
		case XK_asciicircum:
			event.keycode = VC_CIRCUMFLEX;
			break;
		case XK_dollar:
			event.keycode = VC_DOLLAR;
			break;
		case XK_EuroSign:
			event.keycode = VC_EURO_SIGN;
			break;
		case XK_exclam:
			event.keycode = VC_EXCLAMATION_MARK;
			break;
		case XK_exclamdown:
			event.keycode = VC_INVERTED_EXCLAMATION_MARK;
			break;
		case XK_parenleft:
			event.keycode = VC_LEFT_PARENTHESIS;
			break;
		case XK_numbersign:
			event.keycode = VC_NUMBER_SIGN;
			break;
		case XK_plus:
			event.keycode = VC_PLUS;
			break;
		case XK_parenright:
			event.keycode = VC_RIGHT_PARENTHESIS;
			break;
		case XK_underscore:
			event.keycode = VC_UNDERSCORE;
			break;


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
			break;
		#endif
		case XK_Kana_Shift:				// TODO Testing Needed.
			event.keycode = VC_KANA;
			break;
		case XK_Kanji:					// TODO Testing Needed.
			event.keycode = VC_KANJI;
			break;
		case XK_Eisu_Shift:				// TODO Testing Needed.
			event.keycode = VC_ALPHANUMERIC;
			break;
		case XK_Katakana:				// TODO Testing Needed.
			event.keycode = VC_KATAKANA;
			break;
		case XK_Hiragana:				// TODO Testing Needed.
			event.keycode = VC_HIRAGANA;
			break;
		case XK_Zenkaku:				// TODO Testing Needed.
			event.keycode = VC_FULL_WIDTH;
			break;
		case XK_Hankaku:				// TODO Testing Needed.
			event.keycode = VC_HALF_WIDTH;
			break;
		case XK_Romaji:					// TODO Testing Needed.
			event.keycode = VC_ROMAN_CHARACTERS;
			break;
		case XK_MultipleCandidate:		// TODO Testing Needed.
			event.keycode = VC_ALL_CANDIDATES;
			break;
		case XK_PreviousCandidate:		// TODO Testing Needed.
			event.keycode = VC_PREVIOUS_CANDIDATE;
			break;
		case XK_Codeinput:				// TODO Testing Needed.
			event.keycode = VC_CODE_INPUT;
			break;
		case XK_Hiragana_Katakana:		// TODO Testing Needed.
			event.keycode = VC_JAPANESE_KATAKANA;
			break;
		case XK_Zenkaku_Hankaku:		// TODO Testing Needed.
			event.keycode = VC_JAPANESE_HIRAGANA;
			break;
		/* case XK_Romaji:				// TODO Testing Needed
			keyevent.keycode = VC_JAPANESE_ROMAN;
			return keyevent; */
		case XK_Kana_Lock:				// TODO Testing Needed.
			event.keycode = VC_KANA_LOCK;
			break;
		case XK_Eisu_toggle:			// TODO Testing Needed.
			event.keycode = VC_INPUT_METHOD_ON_OFF;
			break;

		// For Sun keyboards.
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:
			event.keycode = VC_CUT;
			break;
		case SunXK_Copy:
			event.keycode = VC_COPY;
			break;
		case SunXK_Paste:
			event.keycode = VC_PASTE;
			break;
		case SunXK_Undo:
			event.keycode = VC_UNDO;
			break;
		case SunXK_Again:
			event.keycode = VC_AGAIN;
			break;
		case SunXK_Find:
			event.keycode = VC_FIND;
			break;
		case SunXK_Props:
			event.keycode = VC_PROPS;
			break;
		case SunXK_Stop:
			event.keycode = VC_STOP;
			break;
		case SunXK_Compose:
			event.keycode = VC_COMPOSE;
			break;
		case SunXK_AltGraph:
			event.keycode = VC_ALT_GRAPH;
			break;
		#endif

		case XK_Begin:
			event.keycode = VC_BEGIN;
			break;

		default:
		case XK_VoidSymbol:
			event.keycode = VC_UNDEFINED;
			event.location = NH_KEY_LOCATION_UNKNOWN;
			break;
	}

	return event;
}

uint_fast8_t ConvertToVirtualButton(uint_fast8_t nativeButton) {
	uint_fast8_t virtualButton;

	switch (nativeButton) {
		case Button1:
			virtualButton = NH_BUTTON1;
			break;
		case Button2:
			virtualButton = NH_BUTTON3;
			break;
		case Button3:
			virtualButton = NH_BUTTON2;
			break;
		case Button4:
			virtualButton = NH_BUTTON4;
			break;
		case Button5:
			virtualButton = NH_BUTTON5;
			break;

		default:
		case AnyButton:
			virtualButton = VC_NOBUTTON;
			break;
	}

	return virtualButton;
}

uint_fast16_t ConvertToVirtualMask(uint_fast16_t nativeMask) {
	uint_fast16_t virtualMask = 0x0000;

	if (nativeMask & ShiftMask)		virtualMask |= SHIFT_MASK;
	if (nativeMask & ControlMask)	virtualMask |= CTRL_MASK;
	if (nativeMask & Mod4Mask)		virtualMask |= META_MASK;
	if (nativeMask & Mod1Mask)		virtualMask |= ALT_MASK;

	if (nativeMask & Button1Mask)	virtualMask |= BUTTON1_MASK;
	if (nativeMask & Button2Mask)	virtualMask |= BUTTON2_MASK;
	if (nativeMask & Button3Mask)	virtualMask |= BUTTON3_MASK;
	if (nativeMask & Button4Mask)	virtualMask |= BUTTON4_MASK;
	if (nativeMask & Button5Mask)	virtualMask |= BUTTON5_MASK;

	return virtualMask;
}
