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

unsigned int ConvertToNativeKey(unsigned int virtualKeyCode) {
	unsigned int keyCode;

	switch (virtualKeyCode) {
		case VC_ENTER:
			keyCode = XK_Return;
			break;
		case VC_BACK_SPACE:
			keyCode = XK_BackSpace;
			break;
		case VC_TAB:
			keyCode = XK_Tab;
			break;
		#ifndef SUN_KEYBOARD
		case VC_CANCEL:
			keyCode = XK_Cancel;
			break;
		#endif


		case VC_SHIFT_L:
			keyCode = XK_Shift_L;
			break;
		case VC_SHIFT_R:
			keyCode = XK_Shift_R;
			break;
		case VC_CONTROL_L:
			keyCode = XK_Control_L;
			break;
		case VC_CONTROL_R:
			keyCode = XK_Control_R;
			break;
		case VC_ALT_L:
			keyCode = XK_Alt_L;
			break;
		case VC_ALT_R:
			keyCode = XK_Alt_R;
			break;
		case VC_META_L:
			keyCode = XK_Meta_L;
			break;
		case VC_META_R:
			keyCode = XK_Meta_R;
			break;
		case VC_SUPER_L:
			keyCode = XK_Super_L;
			break;
		case VC_SUPER_R:
			keyCode = XK_Super_R;
			break;


		case VC_CONTEXT_MENU:
			keyCode = XK_Menu;
			break;


		case VC_PAUSE:
			keyCode = XK_Pause;
			break;
		case VC_CAPS_LOCK:
			keyCode = XK_Caps_Lock;
			break;
		case VC_ESCAPE:
			keyCode = XK_Escape;
			break;
		case VC_SPACE:
			keyCode = XK_space;
			break;


		case VC_UP:
			keyCode = XK_Up;
			break;
		case VC_DOWN:
			keyCode = XK_Down;
			break;
		case VC_LEFT:
			keyCode = XK_Left;
			break;
		case VC_RIGHT:
			keyCode = XK_Right;
			break;


		case VC_COMMA:
			keyCode = XK_comma;
			break;
		case VC_MINUS:
			keyCode = XK_minus;
			break;
		case VC_PERIOD:
			keyCode = XK_period;
			break;
		case VC_SLASH:
			keyCode = XK_slash;
			break;


		case VC_0:
			keyCode = XK_0;
			break;
		case VC_1:
			keyCode = XK_1;
			break;
		case VC_2:
			keyCode = XK_2;
			break;
		case VC_3:
			keyCode = XK_3;
			break;
		case VC_4:
			keyCode = XK_4;
			break;
		case VC_5:
			keyCode = XK_5;
			break;
		case VC_6:
			keyCode = XK_6;
			break;
		case VC_7:
			keyCode = XK_7;
			break;
		case VC_8:
			keyCode = XK_8;
			break;
		case VC_9:
			keyCode = XK_9;
			break;


		case VC_SEMICOLON:
			keyCode = XK_semicolon;
			break;
		case VC_EQUALS:
			keyCode = XK_equal;
			break;

		case VC_A:
			keyCode = XK_A;
			break;
		case VC_B:
			keyCode = XK_B;
			break;
		case VC_C:
			keyCode = XK_C;
			break;
		case VC_D:
			keyCode = XK_D;
			break;
		case VC_E:
			keyCode = XK_E;
			break;
		case VC_F:
			keyCode = XK_F;
			break;
		case VC_G:
			keyCode = XK_G;
			break;
		case VC_H:
			keyCode = XK_H;
			break;
		case VC_I:
			keyCode = XK_I;
			break;
		case VC_J:
			keyCode = XK_J;
			break;
		case VC_K:
			keyCode = XK_K;
			break;
		case VC_L:
			keyCode = XK_L;
			break;
		case VC_M:
			keyCode = XK_M;
			break;
		case VC_N:
			keyCode = XK_N;
			break;
		case VC_O:
			keyCode = XK_O;
			break;
		case VC_P:
			keyCode = XK_P;
			break;
		case VC_Q:
			keyCode = XK_Q;
			break;
		case VC_R:
			keyCode = XK_R;
			break;
		case VC_S:
			keyCode = XK_S;
			break;
		case VC_T:
			keyCode = XK_T;
			break;
		case VC_U:
			keyCode = XK_U;
			break;
		case VC_V:
			keyCode = XK_V;
			break;
		case VC_W:
			keyCode = XK_W;
			break;
		case VC_X:
			keyCode = XK_X;
			break;
		case VC_Y:
			keyCode = XK_Y;
			break;
		case VC_Z:
			keyCode = XK_Z;
			break;

		case VC_OPEN_BRACKET:
			keyCode = XK_bracketleft;
			break;
		case VC_BACK_SLASH:
			keyCode = XK_backslash;
			break;
		case VC_CLOSE_BRACKET:
			keyCode = XK_bracketright;
			break;

		case VC_KP_0:
			keyCode = XK_KP_0;
			break;
		case VC_KP_1:
			keyCode = XK_KP_1;
			break;
		case VC_KP_2:
			keyCode = XK_KP_2;
			break;
		case VC_KP_3:
			keyCode = XK_KP_3;
			break;
		case VC_KP_4:
			keyCode = XK_KP_4;
			break;
		case VC_KP_5:
			keyCode = XK_KP_5;
			break;
		case VC_KP_6:
			keyCode = XK_KP_6;
			break;
		case VC_KP_7:
			keyCode = XK_KP_7;
			break;
		case VC_KP_8:
			keyCode = XK_KP_8;
			break;
		case VC_KP_9:
			keyCode = XK_KP_9;
			break;

		case VC_KP_MULTIPLY:
			keyCode = XK_KP_Multiply;
			break;
		case VC_KP_ADD:
			keyCode = XK_KP_Add;
			break;
		case VC_KP_SEPARATOR:
			keyCode = XK_KP_Separator;
			break;
		case VC_KP_SUBTRACT:
			keyCode = XK_KP_Subtract;
			break;
		case VC_KP_ENTER:
			keyCode = XK_KP_Enter;
			break;
		case VC_KP_DECIMAL:
			keyCode = XK_KP_Decimal;
			break;
		case VC_KP_DIVIDE:
			keyCode = XK_KP_Divide;
			break;
		case VC_KP_DELETE:
			keyCode = XK_KP_Delete;
			break;
		case VC_NUM_LOCK:
			keyCode = XK_Num_Lock;
			break;
		case VC_CLEAR:
			keyCode = XK_Clear;
			break;

		case VC_SCROLL_LOCK:
			keyCode = XK_Scroll_Lock;
			break;

		// Function Keys
		case VC_F1:
			keyCode = XK_F1;
			break;
		case VC_F2:
			keyCode = XK_F2;
			break;
		case VC_F3:
			keyCode = XK_F3;
			break;
		case VC_F4:
			keyCode = XK_F4;
			break;
		case VC_F5:
			keyCode = XK_F5;
			break;
		case VC_F6:
			keyCode = XK_F6;
			break;
		case VC_F7:
			keyCode = XK_F7;
			break;
		case VC_F8:
			keyCode = XK_F8;
			break;
		case VC_F9:
			keyCode = XK_F9;
			break;
		case VC_F10:
			keyCode = XK_F10;
			break;
		case VC_F11:
			keyCode = XK_F11;
			break;
		case VC_F12:
			keyCode = XK_F12;
			break;

		// Extended Function Keys
		case  VC_F13:
			keyCode = XK_F13;
			break;
		case  VC_F14:
			keyCode = XK_F14;
			break;
		case  VC_F15:
			keyCode = XK_F15;
			break;
		case  VC_F16:
			keyCode = XK_F16;
			break;
		case  VC_F17:
			keyCode = XK_F17;
			break;
		case  VC_F18:
			keyCode = XK_F18;
			break;
		case  VC_F19:
			keyCode = XK_F19;
			break;
		case  VC_F20:
			keyCode = XK_F20;
			break;
		case  VC_F21:
			keyCode = XK_F21;
			break;
		case  VC_F22:
			keyCode = XK_F22;
			break;
		case  VC_F23:
			keyCode = XK_F23;
			break;
		case  VC_F24:
			keyCode = XK_F24;
			break;

		case VC_PRINTSCREEN:
			keyCode = XK_Print;
			break;
		case VC_INSERT:
			keyCode = XK_Insert;
			break;
		case VC_DELETE:
			keyCode = XK_Delete;
			break;
		case VC_HELP:
			keyCode = XK_Help;
			break;

		case VC_PAGE_UP:
			keyCode = XK_Page_Up;
			break;
		case VC_PAGE_DOWN:
			keyCode = XK_Page_Down;
			break;
		case VC_HOME:
			keyCode = XK_Home;
			break;
		case VC_END:
			keyCode = XK_End;
			break;

		case VC_QUOTE:
			keyCode = XK_apostrophe;
			break;
		case VC_BACK_QUOTE:
			keyCode = XK_grave;
			break;

		// For European keyboards
		case VC_DEAD_GRAVE:
			keyCode = XK_dead_grave;
			break;
		case VC_DEAD_ACUTE:
			keyCode = XK_dead_acute;
			break;
		case VC_DEAD_CIRCUMFLEX:
			keyCode = XK_dead_circumflex;
			break;
		case VC_DEAD_TILDE:
			keyCode = XK_dead_tilde;
			break;
		case VC_DEAD_MACRON:
			keyCode = XK_dead_macron;
			break;
		case VC_DEAD_BREVE:
			keyCode = XK_dead_breve;
			break;
		case VC_DEAD_ABOVEDOT:
			keyCode = XK_dead_abovedot;
			break;
		case VC_DEAD_DIAERESIS:
			keyCode = XK_dead_diaeresis;
			break;
		case VC_DEAD_ABOVERING:
			keyCode = XK_dead_abovering;
			break;
		case VC_DEAD_DOUBLEACUTE:
			keyCode = XK_dead_doubleacute;
			break;
		case VC_DEAD_CARON:
			keyCode = XK_dead_caron;
			break;
		case VC_DEAD_CEDILLA:
			keyCode = XK_dead_cedilla;
			break;
		case VC_DEAD_OGONEK:
			keyCode = XK_dead_ogonek;
			break;
		case VC_DEAD_IOTA:
			keyCode = XK_dead_iota;
			break;
		case VC_DEAD_VOICED_SOUND:
			keyCode = XK_dead_voiced_sound;
			break;
		case VC_DEAD_SEMIVOICED_SOUND:
			keyCode = XK_dead_semivoiced_sound;
			break;

		// Unknown Keyboard Codes
		case VC_AMPERSAND:
			keyCode = XK_ampersand;
			break;
		case VC_ASTERISK:
			keyCode = XK_asterisk;
			break;
		case VC_QUOTEDBL:
			keyCode = XK_quotedbl;
			break;
		case VC_LESS:
			keyCode = XK_less;
			break;
		case VC_GREATER:
			keyCode = XK_greater;
			break;
		case VC_BRACELEFT:
			keyCode = XK_braceleft;
			break;
		case VC_BRACERIGHT:
			keyCode = XK_braceright;
			break;

		// Unknown Extended Keyboard Codes
		case VC_AT:
			keyCode = XK_at;
			break;
		case VC_COLON:
			keyCode = XK_colon;
			break;
		case VC_CIRCUMFLEX:
			keyCode = XK_asciicircum;
			break;
		case VC_DOLLAR:
			keyCode = XK_dollar;
			break;
		case VC_EURO_SIGN:
			keyCode = XK_EuroSign;
			break;
		case VC_EXCLAMATION_MARK:
			keyCode = XK_exclam;
			break;
		case VC_INVERTED_EXCLAMATION_MARK:
			keyCode = XK_exclamdown;
			break;
		case VC_LEFT_PARENTHESIS:
			keyCode = XK_parenleft;
			break;
		case VC_NUMBER_SIGN:
			keyCode = XK_numbersign;
			break;
		case VC_PLUS:
			keyCode = XK_plus;
			break;
		case VC_RIGHT_PARENTHESIS:
			keyCode = XK_parenright;
			break;
		case VC_UNDERSCORE:
			keyCode = XK_underscore;
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
			keyCode = XK_Mode_switch;
			break;
		#endif
		case VC_KANA:					// TODO Testing Needed.
			keyCode = XK_Kana_Shift;
			break;
		case VC_KANJI:					// TODO Testing Needed.
			keyCode = XK_Kanji;
			break;
		case VC_ALPHANUMERIC:			// TODO Testing Needed.
			keyCode = XK_Eisu_Shift;
			break;
		case VC_KATAKANA:				// TODO Testing Needed.
			keyCode = XK_Katakana;
			break;
		case VC_HIRAGANA:				// TODO Testing Needed.
			keyCode = XK_Hiragana;
			break;
		case VC_FULL_WIDTH:				// TODO Testing Needed.
			keyCode = XK_Zenkaku;
			break;
		case VC_HALF_WIDTH:				// TODO Testing Needed.
			keyCode = XK_Hankaku;
			break;
		case VC_ROMAN_CHARACTERS:		// TODO Testing Needed.
			keyCode = XK_Romaji;
			break;
		case VC_ALL_CANDIDATES:			// TODO Testing Needed.
			keyCode = XK_MultipleCandidate;
			break;
		case VC_PREVIOUS_CANDIDATE:		// TODO Testing Needed.
			keyCode = XK_PreviousCandidate;
			break;
		case VC_CODE_INPUT:				// TODO Testing Needed.
			keyCode = XK_Codeinput;
			break;
		case VC_JAPANESE_KATAKANA:		// TODO Testing Needed.
			keyCode = XK_Hiragana_Katakana;
			break;
		case VC_JAPANESE_HIRAGANA:		// TODO Testing Needed.
			keyCode = XK_Zenkaku_Hankaku;
			break;
		/* case VC_JAPANESE_ROMAN:		// TODO Testing Needed
			keykeyCode = XK_Romaji;
			return keyevent; */
		case VC_KANA_LOCK:				// TODO Testing Needed.
			keyCode = XK_Kana_Lock;
			break;
		case VC_INPUT_METHOD_ON_OFF:	// TODO Testing Needed.
			keyCode = XK_Eisu_toggle;
			break;

		// For Sun keyboards.
		#ifdef SUN_KEYBOARD
		case VC_CUT:
			keyCode = SunXK_Cut;
			break;
		case VC_COPY:
			keyCode = SunXK_Copy;
			break;
		case VC_PASTE:
			keyCode = SunXK_Paste;
			break;
		case VC_UNDO:
			keyCode = SunXK_Undo;
			break;
		case VC_AGAIN:
			keyCode = SunXK_Again;
			break;
		case VC_FIND:
			keyCode = SunXK_Find;
			break;
		case VC_PROPS:
			keyCode = SunXK_Props;
			break;
		case VC_STOP:
			keyCode = SunXK_Stop;
			break;
		case VC_COMPOSE:
			keyCode = SunXK_Compose;
			break;
		case VC_ALT_GRAPH:
			keyCode = SunXK_AltGraph;
			break;
		#endif

		case VC_BEGIN:
			keyCode = XK_Begin;
			break;

		default:
		case VC_UNDEFINED:
			keyCode = XK_VoidSymbol;
			break;
	}

	return keyCode;
}

unsigned short int ConvertToNativeButton(unsigned short int virtualButton) {
	unsigned short int nativeButton;

	switch (virtualButton) {
		case MOUSE_BUTTON1:
			nativeButton = Button1;
			break;
		case MOUSE_BUTTON2:
			nativeButton = Button2;
			break;
		case MOUSE_BUTTON3:
			nativeButton = Button3;
			break;
		case MOUSE_BUTTON4:
			nativeButton = Button4;
			break;
		case MOUSE_BUTTON5:
			nativeButton = Button5;
			break;

		default:
		case MOUSE_NOBUTTON:
			nativeButton = AnyButton;
			break;
	}

	return nativeButton;
}

unsigned short int ConvertToNativeMask(unsigned short int virtualMask) {
	unsigned short int nativeMask = 0x0000;

	if (virtualMask & SHIFT_MASK)	nativeMask |= ShiftMask;
	if (virtualMask & CTRL_MASK)	nativeMask |= ControlMask;
	if (virtualMask & META_MASK)	nativeMask |= Mod4Mask;
	if (virtualMask & ALT_MASK)		nativeMask |= Mod1Mask;

	if (virtualMask & BUTTON1_MASK)	nativeMask |= Button1Mask;
	if (virtualMask & BUTTON2_MASK)	nativeMask |= Button2Mask;
	if (virtualMask & BUTTON3_MASK)	nativeMask |= Button3Mask;
	if (virtualMask & BUTTON4_MASK)	nativeMask |= Button4Mask;
	if (virtualMask & BUTTON5_MASK)	nativeMask |= Button5Mask;

	return nativeMask;
}
