/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
 * https://github.com/kwhat/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
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
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ENTER;
			return jkey;
		case XK_BackSpace:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_SPACE;
			return jkey;
		case XK_Tab:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_TAB;
			return jkey;
		#ifndef SUN_KEYBOARD
		case XK_Cancel:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CANCEL;
			return jkey;
		#endif


		case XK_Shift_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SHIFT;
			goto LOCATION_L;
		case XK_Control_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTROL;
			goto LOCATION_L;
		case XK_Alt_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT;
			goto LOCATION_L;
		case XK_Meta_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_META;
			goto LOCATION_L;
		case XK_Super_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_WINDOWS;
			goto LOCATION_L;
		LOCATION_L:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;
			return jkey;

		case XK_Shift_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SHIFT;
			goto LOCATION_R;
		case XK_Control_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTROL;
			goto LOCATION_R;
		case XK_Alt_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT;
			goto LOCATION_R;
		case XK_Meta_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_META;
			goto LOCATION_R;
		case XK_Super_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_WINDOWS;
			goto LOCATION_R;
		LOCATION_R:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;
			return jkey;


		case XK_Menu:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTEXT_MENU;
			return jkey;


		case XK_Pause:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAUSE;
			return jkey;
		case XK_Caps_Lock:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CAPS_LOCK;
			return jkey;
		case XK_Escape:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ESCAPE;
			return jkey;
		case XK_space:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SPACE;
			return jkey;


		case XK_Up:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UP;
			return jkey;
		case XK_Down:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DOWN;
			return jkey;
		case XK_Left:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT;
			return jkey;
		case XK_Right:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_RIGHT;
			return jkey;


		case XK_comma:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COMMA;
			return jkey;
		case XK_minus:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MINUS;
			return jkey;
		case XK_period:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PERIOD;
			return jkey;
		case XK_slash:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SLASH;
			return jkey;


		case XK_0:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_0;
			return jkey;
		case XK_1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_1;
			return jkey;
		case XK_2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_2;
			return jkey;
		case XK_3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_3;
			return jkey;
		case XK_4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_4;
			return jkey;
		case XK_5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_5;
			return jkey;
		case XK_6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_6;
			return jkey;
		case XK_7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_7;
			return jkey;
		case XK_8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_8;
			return jkey;
		case XK_9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_9;
			return jkey;


		case XK_semicolon:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SEMICOLON;
			return jkey;
		case XK_equal:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EQUALS;
			return jkey;

		case XK_a:
		case XK_A:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_A;
			return jkey;
		case XK_b:
		case XK_B:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_B;
			return jkey;
		case XK_c:
		case XK_C:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_C;
			return jkey;
		case XK_d:
		case XK_D:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_D;
			return jkey;
		case XK_e:
		case XK_E:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_E;
			return jkey;
		case XK_f:
		case XK_F:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F;
			return jkey;
		case XK_g:
		case XK_G:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_G;
			return jkey;
		case XK_h:
		case XK_H:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_H;
			return jkey;
		case XK_i:
		case XK_I:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_I;
			return jkey;
		case XK_j:
		case XK_J:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_J;
			return jkey;
		case XK_k:
		case XK_K:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_K;
			return jkey;
		case XK_l:
		case XK_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_L;
			return jkey;
		case XK_m:
		case XK_M:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_M;
			return jkey;
		case XK_n:
		case XK_N:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_N;
			return jkey;
		case XK_o:
		case XK_O:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_O;
			return jkey;
		case XK_p:
		case XK_P:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_P;
			return jkey;
		case XK_q:
		case XK_Q:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Q;
			return jkey;
		case XK_r:
		case XK_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_R;
			return jkey;
		case XK_s:
		case XK_S:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_S;
			return jkey;
		case XK_t:
		case XK_T:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_T;
			return jkey;
		case XK_u:
		case XK_U:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_U;
			return jkey;
		case XK_v:
		case XK_V:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_V;
			return jkey;
		case XK_w:
		case XK_W:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_W;
			return jkey;
		case XK_x:
		case XK_X:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_X;
			return jkey;
		case XK_y:
		case XK_Y:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Y;
			return jkey;
		case XK_z:
		case XK_Z:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Z;
			return jkey;

		case XK_bracketleft:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_OPEN_BRACKET;
			return jkey;
		case XK_backslash:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_SLASH;
			return jkey;
		case XK_bracketright:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CLOSE_BRACKET;
			return jkey;

		case XK_KP_0:
		case XK_KP_Insert:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD0;
			goto LOCATION_NP;
		case XK_KP_1:
		case XK_KP_End:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD1;
			goto LOCATION_NP;
		case XK_KP_2:
		case XK_KP_Down:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD2;
			goto LOCATION_NP;
		case XK_KP_3:
		case XK_KP_Page_Down:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD3;
			goto LOCATION_NP;
		case XK_KP_4:
		case XK_KP_Left:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD4;
			goto LOCATION_NP;
		case XK_KP_5:
		case XK_KP_Begin:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD5;
			goto LOCATION_NP;
		case XK_KP_6:
		case XK_KP_Right:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD6;
			goto LOCATION_NP;
		case XK_KP_7:
		case XK_KP_Home:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD7;
			goto LOCATION_NP;
		case XK_KP_8:
		case XK_KP_Up:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD8;
			goto LOCATION_NP;
		case XK_KP_9:
		case XK_KP_Page_Up:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD9;
			goto LOCATION_NP;

		case XK_KP_Multiply:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MULTIPLY;
			goto LOCATION_NP;
		case XK_KP_Add:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ADD;
			goto LOCATION_NP;
		case XK_KP_Separator:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SEPARATOR;
			goto LOCATION_NP;
		case XK_KP_Subtract:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SUBTRACT;
			goto LOCATION_NP;
		case XK_KP_Enter:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ENTER;
			goto LOCATION_NP;
		case XK_KP_Decimal:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DECIMAL;
			goto LOCATION_NP;
		case XK_KP_Divide:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DIVIDE;
			goto LOCATION_NP;
		case XK_KP_Delete:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DELETE;
			goto LOCATION_NP;
		case XK_Num_Lock:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUM_LOCK;
			goto LOCATION_NP;
		case XK_Clear:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CLEAR;
			goto LOCATION_NP;
		LOCATION_NP:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;
			return jkey;

		case XK_Scroll_Lock:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SCROLL_LOCK;
			return jkey;

		// Function Keys
		case XK_F1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F1;
			return jkey;
		case XK_F2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F2;
			return jkey;
		case XK_F3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F3;
			return jkey;
		case XK_F4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F4;
			return jkey;
		case XK_F5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F5;
			return jkey;
		case XK_F6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F6;
			return jkey;
		case XK_F7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F7;
			return jkey;
		case XK_F8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F8;
			return jkey;
		case XK_F9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F9;
			return jkey;
		case XK_F10:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F10;
			return jkey;
		case XK_F11:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F11;
			return jkey;
		case XK_F12:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F12;
			return jkey;

		// Extended Function Keys
		case XK_F13:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F13;
			return jkey;
		case XK_F14:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F14;
			return jkey;
		case XK_F15:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F15;
			return jkey;
		case XK_F16:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F16;
			return jkey;
		case XK_F17:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F17;
			return jkey;
		case XK_F18:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F18;
			return jkey;
		case XK_F19:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F19;
			return jkey;
		case XK_F20:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F20;
			return jkey;
		case XK_F21:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F21;
			return jkey;
		case XK_F22:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F22;
			return jkey;
		case XK_F23:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F23;
			return jkey;
		case XK_F24:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F24;
			return jkey;

		case XK_Print:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PRINTSCREEN;
			return jkey;
		case XK_Insert:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INSERT;
			return jkey;
		case XK_Delete:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DELETE;
			return jkey;
		case XK_Help:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HELP;
			return jkey;

		case XK_Page_Up:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_UP;
			return jkey;
		case XK_Page_Down:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_DOWN;
			return jkey;
		case XK_Home:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HOME;
			return jkey;
		case XK_End:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_END;
			return jkey;

		case XK_apostrophe:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_QUOTE;
			return jkey;
		case XK_grave:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_QUOTE;
			return jkey;

		// For European keyboards
		case XK_dead_grave:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_GRAVE;
			return jkey;
		case XK_dead_acute:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ACUTE;
			return jkey;
		case XK_dead_circumflex:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CIRCUMFLEX;
			return jkey;
		case XK_dead_tilde:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_TILDE;
			return jkey;
		case XK_dead_macron:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_MACRON;
			return jkey;
		case XK_dead_breve:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_BREVE;
			return jkey;
		case XK_dead_abovedot:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ABOVEDOT;
			return jkey;
		case XK_dead_diaeresis:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_DIAERESIS;
			return jkey;
		case XK_dead_abovering:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ABOVERING;
			return jkey;
		case XK_dead_doubleacute:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_DOUBLEACUTE;
			return jkey;
		case XK_dead_caron:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CARON;
			return jkey;
		case XK_dead_cedilla:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CEDILLA;
			return jkey;
		case XK_dead_ogonek:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_OGONEK;
			return jkey;
		case XK_dead_iota:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_IOTA;
			return jkey;
		case XK_dead_voiced_sound:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_VOICED_SOUND;
			return jkey;
		case XK_dead_semivoiced_sound:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_SEMIVOICED_SOUND;
			return jkey;

		// Unknown Keyboard Codes
		case XK_ampersand:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AMPERSAND;
			return jkey;
		case XK_asterisk:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ASTERISK;
			return jkey;
		case XK_quotedbl:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_QUOTEDBL;
			return jkey;
		case XK_less:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LESS;
			return jkey;
		case XK_greater:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_GREATER;
			return jkey;
		case XK_braceleft:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BRACELEFT;
			return jkey;
		case XK_braceright:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BRACERIGHT;
			return jkey;

		// Unknown Extended Keyboard Codes
		case XK_at:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AT;
			return jkey;
		case XK_colon:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COLON;
			return jkey;
		case XK_asciicircum:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CIRCUMFLEX;
			return jkey;
		case XK_dollar:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DOLLAR;
			return jkey;
		case XK_EuroSign:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EURO_SIGN;
			return jkey;
		case XK_exclam:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EXCLAMATION_MARK;
			return jkey;
		case XK_exclamdown:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INVERTED_EXCLAMATION_MARK;
			return jkey;
		case XK_parenleft:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT_PARENTHESIS;
			return jkey;
		case XK_numbersign:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMBER_SIGN;
			return jkey;
		case XK_plus:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PLUS;
			return jkey;
		case XK_parenright:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_RIGHT_PARENTHESIS;
			return jkey;
		case XK_underscore:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDERSCORE;
			return jkey;


		// For input method support on Asian Keyboards.
		/*
		case XK_Cancel:					// Unknown X11 Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FINAL;
			return jkey;
		case XK_Henkan:					// Unknown X11 Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONVERT;
			return jkey;
		case XK_Muhenkan:				// Unknown X11 Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NONCONVERT;
			return jkey;
		case XK_VoidSymbol:				// Unknown X11 Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ACCEPT;
			return jkey;
		*/
		#ifndef SUN_KEYBOARD
		case XK_Mode_switch:			// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MODECHANGE;
			return jkey;
		#endif
		case XK_Kana_Shift:				// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANA;
			return jkey;
		case XK_Kanji:					// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANJI;
			return jkey;
		case XK_Eisu_Shift:				// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALPHANUMERIC;
			return jkey;
		case XK_Katakana:				// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KATAKANA;
			return jkey;
		case XK_Hiragana:				// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HIRAGANA;
			return jkey;
		case XK_Zenkaku:				// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FULL_WIDTH;
			return jkey;
		case XK_Hankaku:				// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HALF_WIDTH;
			return jkey;
		case XK_Romaji:					// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ROMAN_CHARACTERS;
			return jkey;
		case XK_MultipleCandidate:		// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALL_CANDIDATES;
			return jkey;
		case XK_PreviousCandidate:		// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PREVIOUS_CANDIDATE;
			return jkey;
		case XK_Codeinput:				// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CODE_INPUT;
			return jkey;
		case XK_Hiragana_Katakana:		// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_KATAKANA;
			return jkey;
		case XK_Zenkaku_Hankaku:		// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_HIRAGANA;
			return jkey;
		/* case XK_Romaji:				// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_ROMAN;
			return jkey; */
		case XK_Kana_Lock:				// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANA_LOCK;
			return jkey;
		case XK_Eisu_toggle:			// TODO Testing Needed.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INPUT_METHOD_ON_OFF;
			return jkey;

		// For Sun keyboards.
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CUT;
			return jkey;
		case SunXK_Copy:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COPY;
			return jkey;
		case SunXK_Paste:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PASTE;
			return jkey;
		case SunXK_Undo:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDO;
			return jkey;
		case SunXK_Again:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AGAIN;
			return jkey;
		case SunXK_Find:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FIND;
			return jkey;
		case SunXK_Props:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PROPS;
			return jkey;
		case SunXK_Stop:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_STOP;
			return jkey;
		case SunXK_Compose:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COMPOSE;
			return jkey;
		case SunXK_AltGraph:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT_GRAPH;
			return jkey;
		#endif

		case XK_Begin:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BEGIN;
			return jkey;

		default:
		case XK_VoidSymbol:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED;
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

jint NativeToJEventMask(unsigned int mask) {
	jint java_mask = 0;

	if (mask & ShiftMask)
		java_mask |= org_jnativehook_NativeInputEvent_SHIFT_MASK;

	if (mask & ControlMask)
		java_mask |= org_jnativehook_NativeInputEvent_CTRL_MASK;

	if (mask & Mod4Mask)
		java_mask |= org_jnativehook_NativeInputEvent_META_MASK;

	if (mask & Mod1Mask)
		java_mask |= org_jnativehook_NativeInputEvent_ALT_MASK;


	if (mask & Button1Mask)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON1_MASK;

	if (mask & Button2Mask)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON2_MASK;

	if (mask & Button3Mask)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON3_MASK;

	if (mask & Button4Mask)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON4_MASK;

	if (mask & Button5Mask)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON5_MASK;

	return java_mask;
}
