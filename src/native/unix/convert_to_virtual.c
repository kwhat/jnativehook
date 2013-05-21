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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#ifdef SUN_KEYBOARD
#include <X11/Sunkeysym.h>
#endif

#include "nativehook.h"

unsigned int ConvertToVirtualKey(unsigned int nativeKeyCode) {
	unsigned int keycode;

	switch (nativeKeyCode) {
		case XK_Return:
			keycode = VC_ENTER;
			break;
		case XK_BackSpace:
			keycode = VC_BACK_SPACE;
			break;
		case XK_Tab:
			keycode = VC_TAB;
			break;
		#ifndef SUN_KEYBOARD
		case XK_Cancel:
			keycode = VC_CANCEL;
			break;
		#endif


		case XK_Shift_L:
			keycode = VC_SHIFT_L;
			break;
		case XK_Shift_R:
			keycode = VC_SHIFT_R;
			break;
		case XK_Control_L:
			keycode = VC_CONTROL_L;
		case XK_Control_R:
			keycode = VC_CONTROL_R;
		case XK_Alt_L:
			keycode = VC_ALT_L;
			break;
		case XK_Alt_R:
			keycode = VC_ALT_R;
			break;
		case XK_Meta_L:
			keycode = VC_META_L;
			break;
		case XK_Meta_R:
			keycode = VC_META_R;
			break;
		case XK_Super_L:
			keycode = VC_SUPER_L;
			break;
		case XK_Super_R:
			keycode = VC_SUPER_R;
			break;


		case XK_Menu:
			keycode = VC_CONTEXT_MENU;
			break;


		case XK_Pause:
			keycode = VC_PAUSE;
			break;
		case XK_Caps_Lock:
			keycode = VC_CAPS_LOCK;
			break;
		case XK_Escape:
			keycode = VC_ESCAPE;
			break;
		case XK_space:
			keycode = VC_SPACE;
			break;


		case XK_Up:
			keycode = VC_UP;
			break;
		case XK_Down:
			keycode = VC_DOWN;
			break;
		case XK_Left:
			keycode = VC_LEFT;
			break;
		case XK_Right:
			keycode = VC_RIGHT;
			break;


		case XK_comma:
			keycode = VC_COMMA;
			break;
		case XK_minus:
			keycode = VC_MINUS;
			break;
		case XK_period:
			keycode = VC_PERIOD;
			break;
		case XK_slash:
			keycode = VC_SLASH;
			break;


		case XK_0:
			keycode = VC_0;
			break;
		case XK_1:
			keycode = VC_1;
			break;
		case XK_2:
			keycode = VC_2;
			break;
		case XK_3:
			keycode = VC_3;
			break;
		case XK_4:
			keycode = VC_4;
			break;
		case XK_5:
			keycode = VC_5;
			break;
		case XK_6:
			keycode = VC_6;
			break;
		case XK_7:
			keycode = VC_7;
			break;
		case XK_8:
			keycode = VC_8;
			break;
		case XK_9:
			keycode = VC_9;
			break;


		case XK_semicolon:
			keycode = VC_SEMICOLON;
			break;
		case XK_equal:
			keycode = VC_EQUALS;
			break;

		case XK_a:
		case XK_A:
			keycode = VC_A;
			break;
		case XK_b:
		case XK_B:
			keycode = VC_B;
			break;
		case XK_c:
		case XK_C:
			keycode = VC_C;
			break;
		case XK_d:
		case XK_D:
			keycode = VC_D;
			break;
		case XK_e:
		case XK_E:
			keycode = VC_E;
			break;
		case XK_f:
		case XK_F:
			keycode = VC_F;
			break;
		case XK_g:
		case XK_G:
			keycode = VC_G;
			break;
		case XK_h:
		case XK_H:
			keycode = VC_H;
			break;
		case XK_i:
		case XK_I:
			keycode = VC_I;
			break;
		case XK_j:
		case XK_J:
			keycode = VC_J;
			break;
		case XK_k:
		case XK_K:
			keycode = VC_K;
			break;
		case XK_l:
		case XK_L:
			keycode = VC_L;
			break;
		case XK_m:
		case XK_M:
			keycode = VC_M;
			break;
		case XK_n:
		case XK_N:
			keycode = VC_N;
			break;
		case XK_o:
		case XK_O:
			keycode = VC_O;
			break;
		case XK_p:
		case XK_P:
			keycode = VC_P;
			break;
		case XK_q:
		case XK_Q:
			keycode = VC_Q;
			break;
		case XK_r:
		case XK_R:
			keycode = VC_R;
			break;
		case XK_s:
		case XK_S:
			keycode = VC_S;
			break;
		case XK_t:
		case XK_T:
			keycode = VC_T;
			break;
		case XK_u:
		case XK_U:
			keycode = VC_U;
			break;
		case XK_v:
		case XK_V:
			keycode = VC_V;
			break;
		case XK_w:
		case XK_W:
			keycode = VC_W;
			break;
		case XK_x:
		case XK_X:
			keycode = VC_X;
			break;
		case XK_y:
		case XK_Y:
			keycode = VC_Y;
			break;
		case XK_z:
		case XK_Z:
			keycode = VC_Z;
			break;

		case XK_bracketleft:
			keycode = VC_OPEN_BRACKET;
			break;
		case XK_backslash:
			keycode = VC_BACK_SLASH;
			break;
		case XK_bracketright:
			keycode = VC_CLOSE_BRACKET;
			break;

		case XK_KP_0:
		case XK_KP_Insert:
			keycode = VC_KP_0;
			break;
		case XK_KP_1:
		case XK_KP_End:
			keycode = VC_KP_1;
			break;
		case XK_KP_2:
		case XK_KP_Down:
			keycode = VC_KP_2;
			break;
		case XK_KP_3:
		case XK_KP_Page_Down:
			keycode = VC_KP_3;
			break;
		case XK_KP_4:
		case XK_KP_Left:
			keycode = VC_KP_4;
			break;
		case XK_KP_5:
		case XK_KP_Begin:
			keycode = VC_KP_5;
			break;
		case XK_KP_6:
		case XK_KP_Right:
			keycode = VC_KP_6;
			break;
		case XK_KP_7:
		case XK_KP_Home:
			keycode = VC_KP_7;
			break;
		case XK_KP_8:
		case XK_KP_Up:
			keycode = VC_KP_8;
			break;
		case XK_KP_9:
		case XK_KP_Page_Up:
			keycode = VC_KP_9;
			break;

		case XK_KP_Multiply:
			keycode = VC_KP_MULTIPLY;
			break;
		case XK_KP_Add:
			keycode = VC_KP_ADD;
			break;
		case XK_KP_Separator:
			keycode = VC_KP_SEPARATOR;
			break;
		case XK_KP_Subtract:
			keycode = VC_KP_SUBTRACT;
			break;
		case XK_KP_Enter:
			keycode = VC_KP_ENTER;
			break;
		case XK_KP_Decimal:
			keycode = VC_KP_DECIMAL;
			break;
		case XK_KP_Divide:
			keycode = VC_KP_DIVIDE;
			break;
		case XK_KP_Delete:
			keycode = VC_KP_DELETE;
			break;
		case XK_Num_Lock:
			keycode = VC_NUM_LOCK;
			break;
		case XK_Clear:
			keycode = VC_CLEAR;
			break;

		case XK_Scroll_Lock:
			keycode = VC_SCROLL_LOCK;
			break;

		// Function Keys
		case XK_F1:
			keycode = VC_F1;
			break;
		case XK_F2:
			keycode = VC_F2;
			break;
		case XK_F3:
			keycode = VC_F3;
			break;
		case XK_F4:
			keycode = VC_F4;
			break;
		case XK_F5:
			keycode = VC_F5;
			break;
		case XK_F6:
			keycode = VC_F6;
			break;
		case XK_F7:
			keycode = VC_F7;
			break;
		case XK_F8:
			keycode = VC_F8;
			break;
		case XK_F9:
			keycode = VC_F9;
			break;
		case XK_F10:
			keycode = VC_F10;
			break;
		case XK_F11:
			keycode = VC_F11;
			break;
		case XK_F12:
			keycode = VC_F12;
			break;

		// Extended Function Keys
		case XK_F13:
			keycode = VC_F13;
			break;
		case XK_F14:
			keycode = VC_F14;
			break;
		case XK_F15:
			keycode = VC_F15;
			break;
		case XK_F16:
			keycode = VC_F16;
			break;
		case XK_F17:
			keycode = VC_F17;
			break;
		case XK_F18:
			keycode = VC_F18;
			break;
		case XK_F19:
			keycode = VC_F19;
			break;
		case XK_F20:
			keycode = VC_F20;
			break;
		case XK_F21:
			keycode = VC_F21;
			break;
		case XK_F22:
			keycode = VC_F22;
			break;
		case XK_F23:
			keycode = VC_F23;
			break;
		case XK_F24:
			keycode = VC_F24;
			break;

		case XK_Print:
			keycode = VC_PRINTSCREEN;
			break;
		case XK_Insert:
			keycode = VC_INSERT;
			break;
		case XK_Delete:
			keycode = VC_DELETE;
			break;
		case XK_Help:
			keycode = VC_HELP;
			break;

		case XK_Page_Up:
			keycode = VC_PAGE_UP;
			break;
		case XK_Page_Down:
			keycode = VC_PAGE_DOWN;
			break;
		case XK_Home:
			keycode = VC_HOME;
			break;
		case XK_End:
			keycode = VC_END;
			break;

		case XK_apostrophe:
			keycode = VC_QUOTE;
			break;
		case XK_grave:
			keycode = VC_BACK_QUOTE;
			break;

		// For European keyboards
		case XK_dead_grave:
			keycode = VC_DEAD_GRAVE;
			break;
		case XK_dead_acute:
			keycode = VC_DEAD_ACUTE;
			break;
		case XK_dead_circumflex:
			keycode = VC_DEAD_CIRCUMFLEX;
			break;
		case XK_dead_tilde:
			keycode = VC_DEAD_TILDE;
			break;
		case XK_dead_macron:
			keycode = VC_DEAD_MACRON;
			break;
		case XK_dead_breve:
			keycode = VC_DEAD_BREVE;
			break;
		case XK_dead_abovedot:
			keycode = VC_DEAD_ABOVEDOT;
			break;
		case XK_dead_diaeresis:
			keycode = VC_DEAD_DIAERESIS;
			break;
		case XK_dead_abovering:
			keycode = VC_DEAD_ABOVERING;
			break;
		case XK_dead_doubleacute:
			keycode = VC_DEAD_DOUBLEACUTE;
			break;
		case XK_dead_caron:
			keycode = VC_DEAD_CARON;
			break;
		case XK_dead_cedilla:
			keycode = VC_DEAD_CEDILLA;
			break;
		case XK_dead_ogonek:
			keycode = VC_DEAD_OGONEK;
			break;
		case XK_dead_iota:
			keycode = VC_DEAD_IOTA;
			break;
		case XK_dead_voiced_sound:
			keycode = VC_DEAD_VOICED_SOUND;
			break;
		case XK_dead_semivoiced_sound:
			keycode = VC_DEAD_SEMIVOICED_SOUND;
			break;

		// Unknown Keyboard Codes
		case XK_ampersand:
			keycode = VC_AMPERSAND;
			break;
		case XK_asterisk:
			keycode = VC_ASTERISK;
			break;
		case XK_quotedbl:
			keycode = VC_QUOTEDBL;
			break;
		case XK_less:
			keycode = VC_LESS;
			break;
		case XK_greater:
			keycode = VC_GREATER;
			break;
		case XK_braceleft:
			keycode = VC_BRACELEFT;
			break;
		case XK_braceright:
			keycode = VC_BRACERIGHT;
			break;

		// Unknown Extended Keyboard Codes
		case XK_at:
			keycode = VC_AT;
			break;
		case XK_colon:
			keycode = VC_COLON;
			break;
		case XK_asciicircum:
			keycode = VC_CIRCUMFLEX;
			break;
		case XK_dollar:
			keycode = VC_DOLLAR;
			break;
		case XK_EuroSign:
			keycode = VC_EURO_SIGN;
			break;
		case XK_exclam:
			keycode = VC_EXCLAMATION_MARK;
			break;
		case XK_exclamdown:
			keycode = VC_INVERTED_EXCLAMATION_MARK;
			break;
		case XK_parenleft:
			keycode = VC_LEFT_PARENTHESIS;
			break;
		case XK_numbersign:
			keycode = VC_NUMBER_SIGN;
			break;
		case XK_plus:
			keycode = VC_PLUS;
			break;
		case XK_parenright:
			keycode = VC_RIGHT_PARENTHESIS;
			break;
		case XK_underscore:
			keycode = VC_UNDERSCORE;
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
			keycode = VC_MODECHANGE;
			break;
		#endif
		case XK_Kana_Shift:				// TODO Testing Needed.
			keycode = VC_KANA;
			break;
		case XK_Kanji:					// TODO Testing Needed.
			keycode = VC_KANJI;
			break;
		case XK_Eisu_Shift:				// TODO Testing Needed.
			keycode = VC_ALPHANUMERIC;
			break;
		case XK_Katakana:				// TODO Testing Needed.
			keycode = VC_KATAKANA;
			break;
		case XK_Hiragana:				// TODO Testing Needed.
			keycode = VC_HIRAGANA;
			break;
		case XK_Zenkaku:				// TODO Testing Needed.
			keycode = VC_FULL_WIDTH;
			break;
		case XK_Hankaku:				// TODO Testing Needed.
			keycode = VC_HALF_WIDTH;
			break;
		case XK_Romaji:					// TODO Testing Needed.
			keycode = VC_ROMAN_CHARACTERS;
			break;
		case XK_MultipleCandidate:		// TODO Testing Needed.
			keycode = VC_ALL_CANDIDATES;
			break;
		case XK_PreviousCandidate:		// TODO Testing Needed.
			keycode = VC_PREVIOUS_CANDIDATE;
			break;
		case XK_Codeinput:				// TODO Testing Needed.
			keycode = VC_CODE_INPUT;
			break;
		case XK_Hiragana_Katakana:		// TODO Testing Needed.
			keycode = VC_JAPANESE_KATAKANA;
			break;
		case XK_Zenkaku_Hankaku:		// TODO Testing Needed.
			keycode = VC_JAPANESE_HIRAGANA;
			break;
		/* case XK_Romaji:				// TODO Testing Needed
			keykeycode = VC_JAPANESE_ROMAN;
			return keyevent; */
		case XK_Kana_Lock:				// TODO Testing Needed.
			keycode = VC_KANA_LOCK;
			break;
		case XK_Eisu_toggle:			// TODO Testing Needed.
			keycode = VC_INPUT_METHOD_ON_OFF;
			break;

		// For Sun keyboards.
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:
			keycode = VC_CUT;
			break;
		case SunXK_Copy:
			keycode = VC_COPY;
			break;
		case SunXK_Paste:
			keycode = VC_PASTE;
			break;
		case SunXK_Undo:
			keycode = VC_UNDO;
			break;
		case SunXK_Again:
			keycode = VC_AGAIN;
			break;
		case SunXK_Find:
			keycode = VC_FIND;
			break;
		case SunXK_Props:
			keycode = VC_PROPS;
			break;
		case SunXK_Stop:
			keycode = VC_STOP;
			break;
		case SunXK_Compose:
			keycode = VC_COMPOSE;
			break;
		case SunXK_AltGraph:
			keycode = VC_ALT_GRAPH;
			break;
		#endif

		case XK_Begin:
			keycode = VC_BEGIN;
			break;

		default:
		case XK_VoidSymbol:
			keycode = VC_UNDEFINED;
			break;
	}

	return keycode;
}

unsigned int ConvertToVirtualButton(unsigned int nativeButton) {
	unsigned char virtualButton;

	switch (nativeButton) {
		case Button1:
			virtualButton = MOUSE_BUTTON1;
			break;
		case Button2:
			virtualButton = MOUSE_BUTTON2;
			break;
		case Button3:
			virtualButton = MOUSE_BUTTON3;
			break;
		case Button4:
			virtualButton = MOUSE_BUTTON4;
			break;
		case Button5:
			virtualButton = MOUSE_BUTTON5;
			break;

		default:
		case AnyButton:
			virtualButton = MOUSE_NOBUTTON;
			break;
	}

	return virtualButton;
}

unsigned int ConvertToVirtualMask(unsigned int nativeMask) {
	unsigned int virtualMask = 0x0000;

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
