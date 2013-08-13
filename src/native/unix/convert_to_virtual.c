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

#include <nativehook.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#ifdef SUN_KEYBOARD
#include <X11/Sunkeysym.h>
#endif

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
