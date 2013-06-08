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

unsigned int convert_to_native_key(unsigned int virtual_keycode) {
	unsigned int native_keycode;

	switch (virtual_keycode) {
		case VC_ENTER:
			native_keycode = XK_Return;
			break;
		case VC_BACK_SPACE:
			native_keycode = XK_BackSpace;
			break;
		case VC_TAB:
			native_keycode = XK_Tab;
			break;
		#ifndef SUN_KEYBOARD
		case VC_CANCEL:
			native_keycode = XK_Cancel;
			break;
		#endif


		case VC_SHIFT_L:
			native_keycode = XK_Shift_L;
			break;
		case VC_SHIFT_R:
			native_keycode = XK_Shift_R;
			break;
		case VC_CONTROL_L:
			native_keycode = XK_Control_L;
			break;
		case VC_CONTROL_R:
			native_keycode = XK_Control_R;
			break;
		case VC_ALT_L:
			native_keycode = XK_Alt_L;
			break;
		case VC_ALT_R:
			native_keycode = XK_Alt_R;
			break;
		case VC_META_L:
			native_keycode = XK_Meta_L;
			break;
		case VC_META_R:
			native_keycode = XK_Meta_R;
			break;
		case VC_SUPER_L:
			native_keycode = XK_Super_L;
			break;
		case VC_SUPER_R:
			native_keycode = XK_Super_R;
			break;


		case VC_CONTEXT_MENU:
			native_keycode = XK_Menu;
			break;


		case VC_PAUSE:
			native_keycode = XK_Pause;
			break;
		case VC_CAPS_LOCK:
			native_keycode = XK_Caps_Lock;
			break;
		case VC_ESCAPE:
			native_keycode = XK_Escape;
			break;
		case VC_SPACE:
			native_keycode = XK_space;
			break;


		case VC_UP:
			native_keycode = XK_Up;
			break;
		case VC_DOWN:
			native_keycode = XK_Down;
			break;
		case VC_LEFT:
			native_keycode = XK_Left;
			break;
		case VC_RIGHT:
			native_keycode = XK_Right;
			break;


		case VC_COMMA:
			native_keycode = XK_comma;
			break;
		case VC_MINUS:
			native_keycode = XK_minus;
			break;
		case VC_PERIOD:
			native_keycode = XK_period;
			break;
		case VC_SLASH:
			native_keycode = XK_slash;
			break;


		case VC_0:
			native_keycode = XK_0;
			break;
		case VC_1:
			native_keycode = XK_1;
			break;
		case VC_2:
			native_keycode = XK_2;
			break;
		case VC_3:
			native_keycode = XK_3;
			break;
		case VC_4:
			native_keycode = XK_4;
			break;
		case VC_5:
			native_keycode = XK_5;
			break;
		case VC_6:
			native_keycode = XK_6;
			break;
		case VC_7:
			native_keycode = XK_7;
			break;
		case VC_8:
			native_keycode = XK_8;
			break;
		case VC_9:
			native_keycode = XK_9;
			break;


		case VC_SEMICOLON:
			native_keycode = XK_semicolon;
			break;
		case VC_EQUALS:
			native_keycode = XK_equal;
			break;

		case VC_A:
			native_keycode = XK_A;
			break;
		case VC_B:
			native_keycode = XK_B;
			break;
		case VC_C:
			native_keycode = XK_C;
			break;
		case VC_D:
			native_keycode = XK_D;
			break;
		case VC_E:
			native_keycode = XK_E;
			break;
		case VC_F:
			native_keycode = XK_F;
			break;
		case VC_G:
			native_keycode = XK_G;
			break;
		case VC_H:
			native_keycode = XK_H;
			break;
		case VC_I:
			native_keycode = XK_I;
			break;
		case VC_J:
			native_keycode = XK_J;
			break;
		case VC_K:
			native_keycode = XK_K;
			break;
		case VC_L:
			native_keycode = XK_L;
			break;
		case VC_M:
			native_keycode = XK_M;
			break;
		case VC_N:
			native_keycode = XK_N;
			break;
		case VC_O:
			native_keycode = XK_O;
			break;
		case VC_P:
			native_keycode = XK_P;
			break;
		case VC_Q:
			native_keycode = XK_Q;
			break;
		case VC_R:
			native_keycode = XK_R;
			break;
		case VC_S:
			native_keycode = XK_S;
			break;
		case VC_T:
			native_keycode = XK_T;
			break;
		case VC_U:
			native_keycode = XK_U;
			break;
		case VC_V:
			native_keycode = XK_V;
			break;
		case VC_W:
			native_keycode = XK_W;
			break;
		case VC_X:
			native_keycode = XK_X;
			break;
		case VC_Y:
			native_keycode = XK_Y;
			break;
		case VC_Z:
			native_keycode = XK_Z;
			break;

		case VC_OPEN_BRACKET:
			native_keycode = XK_bracketleft;
			break;
		case VC_BACK_SLASH:
			native_keycode = XK_backslash;
			break;
		case VC_CLOSE_BRACKET:
			native_keycode = XK_bracketright;
			break;

		case VC_KP_0:
			native_keycode = XK_KP_0;
			break;
		case VC_KP_1:
			native_keycode = XK_KP_1;
			break;
		case VC_KP_2:
			native_keycode = XK_KP_2;
			break;
		case VC_KP_3:
			native_keycode = XK_KP_3;
			break;
		case VC_KP_4:
			native_keycode = XK_KP_4;
			break;
		case VC_KP_5:
			native_keycode = XK_KP_5;
			break;
		case VC_KP_6:
			native_keycode = XK_KP_6;
			break;
		case VC_KP_7:
			native_keycode = XK_KP_7;
			break;
		case VC_KP_8:
			native_keycode = XK_KP_8;
			break;
		case VC_KP_9:
			native_keycode = XK_KP_9;
			break;

		case VC_KP_MULTIPLY:
			native_keycode = XK_KP_Multiply;
			break;
		case VC_KP_ADD:
			native_keycode = XK_KP_Add;
			break;
		case VC_KP_SEPARATOR:
			native_keycode = XK_KP_Separator;
			break;
		case VC_KP_SUBTRACT:
			native_keycode = XK_KP_Subtract;
			break;
		case VC_KP_ENTER:
			native_keycode = XK_KP_Enter;
			break;
		case VC_KP_DECIMAL:
			native_keycode = XK_KP_Decimal;
			break;
		case VC_KP_DIVIDE:
			native_keycode = XK_KP_Divide;
			break;
		case VC_KP_DELETE:
			native_keycode = XK_KP_Delete;
			break;
		case VC_NUM_LOCK:
			native_keycode = XK_Num_Lock;
			break;
		case VC_CLEAR:
			native_keycode = XK_Clear;
			break;

		case VC_SCROLL_LOCK:
			native_keycode = XK_Scroll_Lock;
			break;

		// Function Keys
		case VC_F1:
			native_keycode = XK_F1;
			break;
		case VC_F2:
			native_keycode = XK_F2;
			break;
		case VC_F3:
			native_keycode = XK_F3;
			break;
		case VC_F4:
			native_keycode = XK_F4;
			break;
		case VC_F5:
			native_keycode = XK_F5;
			break;
		case VC_F6:
			native_keycode = XK_F6;
			break;
		case VC_F7:
			native_keycode = XK_F7;
			break;
		case VC_F8:
			native_keycode = XK_F8;
			break;
		case VC_F9:
			native_keycode = XK_F9;
			break;
		case VC_F10:
			native_keycode = XK_F10;
			break;
		case VC_F11:
			native_keycode = XK_F11;
			break;
		case VC_F12:
			native_keycode = XK_F12;
			break;

		// Extended Function Keys
		case  VC_F13:
			native_keycode = XK_F13;
			break;
		case  VC_F14:
			native_keycode = XK_F14;
			break;
		case  VC_F15:
			native_keycode = XK_F15;
			break;
		case  VC_F16:
			native_keycode = XK_F16;
			break;
		case  VC_F17:
			native_keycode = XK_F17;
			break;
		case  VC_F18:
			native_keycode = XK_F18;
			break;
		case  VC_F19:
			native_keycode = XK_F19;
			break;
		case  VC_F20:
			native_keycode = XK_F20;
			break;
		case  VC_F21:
			native_keycode = XK_F21;
			break;
		case  VC_F22:
			native_keycode = XK_F22;
			break;
		case  VC_F23:
			native_keycode = XK_F23;
			break;
		case  VC_F24:
			native_keycode = XK_F24;
			break;

		case VC_PRINTSCREEN:
			native_keycode = XK_Print;
			break;
		case VC_INSERT:
			native_keycode = XK_Insert;
			break;
		case VC_DELETE:
			native_keycode = XK_Delete;
			break;
		case VC_HELP:
			native_keycode = XK_Help;
			break;

		case VC_PAGE_UP:
			native_keycode = XK_Page_Up;
			break;
		case VC_PAGE_DOWN:
			native_keycode = XK_Page_Down;
			break;
		case VC_HOME:
			native_keycode = XK_Home;
			break;
		case VC_END:
			native_keycode = XK_End;
			break;

		case VC_QUOTE:
			native_keycode = XK_apostrophe;
			break;
		case VC_BACK_QUOTE:
			native_keycode = XK_grave;
			break;

		// For European keyboards
		case VC_DEAD_GRAVE:
			native_keycode = XK_dead_grave;
			break;
		case VC_DEAD_ACUTE:
			native_keycode = XK_dead_acute;
			break;
		case VC_DEAD_CIRCUMFLEX:
			native_keycode = XK_dead_circumflex;
			break;
		case VC_DEAD_TILDE:
			native_keycode = XK_dead_tilde;
			break;
		case VC_DEAD_MACRON:
			native_keycode = XK_dead_macron;
			break;
		case VC_DEAD_BREVE:
			native_keycode = XK_dead_breve;
			break;
		case VC_DEAD_ABOVEDOT:
			native_keycode = XK_dead_abovedot;
			break;
		case VC_DEAD_DIAERESIS:
			native_keycode = XK_dead_diaeresis;
			break;
		case VC_DEAD_ABOVERING:
			native_keycode = XK_dead_abovering;
			break;
		case VC_DEAD_DOUBLEACUTE:
			native_keycode = XK_dead_doubleacute;
			break;
		case VC_DEAD_CARON:
			native_keycode = XK_dead_caron;
			break;
		case VC_DEAD_CEDILLA:
			native_keycode = XK_dead_cedilla;
			break;
		case VC_DEAD_OGONEK:
			native_keycode = XK_dead_ogonek;
			break;
		case VC_DEAD_IOTA:
			native_keycode = XK_dead_iota;
			break;
		case VC_DEAD_VOICED_SOUND:
			native_keycode = XK_dead_voiced_sound;
			break;
		case VC_DEAD_SEMIVOICED_SOUND:
			native_keycode = XK_dead_semivoiced_sound;
			break;

		// Unknown Keyboard Codes
		case VC_AMPERSAND:
			native_keycode = XK_ampersand;
			break;
		case VC_ASTERISK:
			native_keycode = XK_asterisk;
			break;
		case VC_QUOTEDBL:
			native_keycode = XK_quotedbl;
			break;
		case VC_LESS:
			native_keycode = XK_less;
			break;
		case VC_GREATER:
			native_keycode = XK_greater;
			break;
		case VC_BRACELEFT:
			native_keycode = XK_braceleft;
			break;
		case VC_BRACERIGHT:
			native_keycode = XK_braceright;
			break;

		// Unknown Extended Keyboard Codes
		case VC_AT:
			native_keycode = XK_at;
			break;
		case VC_COLON:
			native_keycode = XK_colon;
			break;
		case VC_CIRCUMFLEX:
			native_keycode = XK_asciicircum;
			break;
		case VC_DOLLAR:
			native_keycode = XK_dollar;
			break;
		case VC_EURO_SIGN:
			native_keycode = XK_EuroSign;
			break;
		case VC_EXCLAMATION_MARK:
			native_keycode = XK_exclam;
			break;
		case VC_INVERTED_EXCLAMATION_MARK:
			native_keycode = XK_exclamdown;
			break;
		case VC_LEFT_PARENTHESIS:
			native_keycode = XK_parenleft;
			break;
		case VC_NUMBER_SIGN:
			native_keycode = XK_numbersign;
			break;
		case VC_PLUS:
			native_keycode = XK_plus;
			break;
		case VC_RIGHT_PARENTHESIS:
			native_keycode = XK_parenright;
			break;
		case VC_UNDERSCORE:
			native_keycode = XK_underscore;
			break;


		// For input method support on Asian Keyboards.
		/*
		case VC_FINAL:					// Unknown X11 Support
			keykeyCode = XK_Cancel;
			return keyevent;
		case VC_CONVERT:				// Unknown X11 Support
			keykeyCode = XK_Henkan;
			return keyevent;
		case VC_NONCONVERT:				// Unknown X11 Support
			keykeyCode = XK_Muhenkan;
			return keyevent;
		case VC_ACCEPT:					// Unknown X11 Support
			keykeyCode = XK_VoidSymbol;
			return keyevent;
		*/
		#ifndef SUN_KEYBOARD
		case VC_MODECHANGE:				// TODO Testing Needed.
			native_keycode = XK_Mode_switch;
			break;
		#endif
		case VC_KANA:					// TODO Testing Needed.
			native_keycode = XK_Kana_Shift;
			break;
		case VC_KANJI:					// TODO Testing Needed.
			native_keycode = XK_Kanji;
			break;
		case VC_ALPHANUMERIC:			// TODO Testing Needed.
			native_keycode = XK_Eisu_Shift;
			break;
		case VC_KATAKANA:				// TODO Testing Needed.
			native_keycode = XK_Katakana;
			break;
		case VC_HIRAGANA:				// TODO Testing Needed.
			native_keycode = XK_Hiragana;
			break;
		case VC_FULL_WIDTH:				// TODO Testing Needed.
			native_keycode = XK_Zenkaku;
			break;
		case VC_HALF_WIDTH:				// TODO Testing Needed.
			native_keycode = XK_Hankaku;
			break;
		case VC_ROMAN_CHARACTERS:		// TODO Testing Needed.
			native_keycode = XK_Romaji;
			break;
		case VC_ALL_CANDIDATES:			// TODO Testing Needed.
			native_keycode = XK_MultipleCandidate;
			break;
		case VC_PREVIOUS_CANDIDATE:		// TODO Testing Needed.
			native_keycode = XK_PreviousCandidate;
			break;
		case VC_CODE_INPUT:				// TODO Testing Needed.
			native_keycode = XK_Codeinput;
			break;
		case VC_JAPANESE_KATAKANA:		// TODO Testing Needed.
			native_keycode = XK_Hiragana_Katakana;
			break;
		case VC_JAPANESE_HIRAGANA:		// TODO Testing Needed.
			native_keycode = XK_Zenkaku_Hankaku;
			break;
		/* case VC_JAPANESE_ROMAN:		// TODO Testing Needed
			keykeyCode = XK_Romaji;
			return keyevent; */
		case VC_KANA_LOCK:				// TODO Testing Needed.
			native_keycode = XK_Kana_Lock;
			break;
		case VC_INPUT_METHOD_ON_OFF:	// TODO Testing Needed.
			native_keycode = XK_Eisu_toggle;
			break;

		// For Sun keyboards.
		#ifdef SUN_KEYBOARD
		case VC_CUT:
			native_keycode = SunXK_Cut;
			break;
		case VC_COPY:
			native_keycode = SunXK_Copy;
			break;
		case VC_PASTE:
			native_keycode = SunXK_Paste;
			break;
		case VC_UNDO:
			native_keycode = SunXK_Undo;
			break;
		case VC_AGAIN:
			native_keycode = SunXK_Again;
			break;
		case VC_FIND:
			native_keycode = SunXK_Find;
			break;
		case VC_PROPS:
			native_keycode = SunXK_Props;
			break;
		case VC_STOP:
			native_keycode = SunXK_Stop;
			break;
		case VC_COMPOSE:
			native_keycode = SunXK_Compose;
			break;
		case VC_ALT_GRAPH:
			native_keycode = SunXK_AltGraph;
			break;
		#endif

		case VC_BEGIN:
			native_keycode = XK_Begin;
			break;

		default:
		case VC_UNDEFINED:
			native_keycode = XK_VoidSymbol;
			break;
	}

	return native_keycode;
}

unsigned int convert_to_native_button(unsigned short int virtual_button) {
	unsigned short int native_button;

	switch (virtual_button) {
		case MOUSE_BUTTON1:
			native_button = Button1;
			break;
		case MOUSE_BUTTON2:
			native_button = Button2;
			break;
		case MOUSE_BUTTON3:
			native_button = Button3;
			break;
		case MOUSE_BUTTON4:
			native_button = Button4;
			break;
		case MOUSE_BUTTON5:
			native_button = Button5;
			break;

		default:
		case MOUSE_NOBUTTON:
			native_button = AnyButton;
			break;
	}

	return native_button;
}

unsigned short int convert_to_native_mask(unsigned short int virtual_mask) {
	unsigned short int native_mask = 0x0000;

	if (virtual_mask & MASK_SHIFT_L)	native_mask |= ShiftMask;
	if (virtual_mask & MASK_CTRL_L)		native_mask |= ControlMask;
	if (virtual_mask & MASK_META_L)		native_mask |= Mod4Mask;
	if (virtual_mask & MASK_ALT_L)		native_mask |= Mod1Mask;

	if (virtual_mask & MASK_BUTTON1)	native_mask |= Button1Mask;
	if (virtual_mask & MASK_BUTTON2)	native_mask |= Button2Mask;
	if (virtual_mask & MASK_BUTTON3)	native_mask |= Button3Mask;
	if (virtual_mask & MASK_BUTTON4)	native_mask |= Button4Mask;
	if (virtual_mask & MASK_BUTTON5)	native_mask |= Button5Mask;

	return native_mask;
}
