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

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>

#include "NativeToJava.h"
#include "WinInputHelpers.h"

JKeyDatum NativeToJKey(unsigned int keysym) {
	JKeyDatum jkey;
	jkey.rawcode = keysym;
	jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD;

	switch (keysym) {
		case VK_RETURN:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ENTER;
			return jkey;
		case VK_BACK:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_SPACE;
			return jkey;
		case VK_TAB:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_TAB;
			return jkey;
		case VK_CANCEL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CANCEL;
			return jkey;


		case VK_LSHIFT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SHIFT;
			goto LOCATION_L;
		case VK_LCONTROL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTROL;
			goto LOCATION_L;
		case VK_LMENU:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT;
			goto LOCATION_L;
		case VK_LWIN:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_WINDOWS;
			goto LOCATION_L;
		LOCATION_L:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;
			return jkey;

		case VK_RSHIFT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SHIFT;
			goto LOCATION_R;
		case VK_RCONTROL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTROL;
			goto LOCATION_R;
		case VK_RMENU:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT;
			goto LOCATION_R;
		case VK_RWIN:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_WINDOWS;
			goto LOCATION_R;
		LOCATION_R:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;
			return jkey;


		case VK_APPS:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTEXT_MENU;
			return jkey;

		case VK_PAUSE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAUSE;
			return jkey;
		case VK_CAPITAL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CAPS_LOCK;
			return jkey;
		case VK_ESCAPE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ESCAPE;
			return jkey;
		case VK_SPACE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SPACE;
			return jkey;


		case VK_UP:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UP;
			return jkey;
		case VK_DOWN:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DOWN;
			return jkey;
		case VK_LEFT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT;
			return jkey;
		case VK_RIGHT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_RIGHT;
			return jkey;


		case VK_OEM_COMMA:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COMMA;
			return jkey;
		case VK_OEM_MINUS:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MINUS;
			return jkey;
		case VK_OEM_PERIOD:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PERIOD;
			return jkey;
		/* case VK_OEM_2:					// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SLASH;
			return jkey; */


		case 0x30:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_0;
			return jkey;
		case 0x31:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_1;
			return jkey;
		case 0x32:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_2;
			return jkey;
		case 0x33:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_3;
			return jkey;
		case 0x34:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_4;
			return jkey;
		case 0x35:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_5;
			return jkey;
		case 0x36:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_6;
			return jkey;
		case 0x37:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_7;
			return jkey;
		case 0x38:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_8;
			return jkey;
		case 0x39:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_9;
			return jkey;


		case VK_OEM_1:					// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SEMICOLON;
			return jkey;
		case VK_OEM_PLUS:				// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EQUALS;
			return jkey;


		case 0x41:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_A;
			return jkey;
		case 0x42:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_B;
			return jkey;
		case 0x43:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_C;
			return jkey;
		case 0x44:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_D;
			return jkey;
		case 0x45:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_E;
			return jkey;
		case 0x46:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F;
			return jkey;
		case 0x47:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_G;
			return jkey;
		case 0x48:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_H;
			return jkey;
		case 0x49:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_I;
			return jkey;
		case 0x4A:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_J;
			return jkey;
		case 0x4B:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_K;
			return jkey;
		case 0x4C:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_L;
			return jkey;
		case 0x4D:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_M;
			return jkey;
		case 0x4E:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_N;
			return jkey;
		case 0x4F:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_O;
			return jkey;
		case 0x50:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_P;
			return jkey;
		case 0x51:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Q;
			return jkey;
		case 0x52:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_R;
			return jkey;
		case 0x53:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_S;
			return jkey;
		case 0x54:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_T;
			return jkey;
		case 0x55:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_U;
			return jkey;
		case 0x56:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_V;
			return jkey;
		case 0x57:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_W;
			return jkey;
		case 0x58:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_X;
			return jkey;
		case 0x59:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Y;
			return jkey;
		case 0x5A:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Z;
			return jkey;


		case VK_OEM_4:					// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_OPEN_BRACKET;
			return jkey;
		case VK_OEM_5:					// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_SLASH;
			return jkey;
		case VK_OEM_6:					// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CLOSE_BRACKET;
			return jkey;

		case VK_CLEAR:					// This is the 5 button when numlock is off.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CLEAR;
			goto LOCATION_NP;
		case VK_NUMPAD_PRIOR:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_UP;
			goto LOCATION_NP;
		case VK_NUMPAD_NEXT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_DOWN;
			goto LOCATION_NP;
		case VK_NUMPAD_END:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_END;
			goto LOCATION_NP;
		case VK_NUMPAD_HOME:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HOME;
			goto LOCATION_NP;
		case VK_NUMPAD_LEFT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT;
			goto LOCATION_NP;
		case VK_NUMPAD_UP:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UP;
			goto LOCATION_NP;
		case VK_NUMPAD_RIGHT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UP;
			goto LOCATION_NP;
		case VK_NUMPAD_DOWN:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DOWN;
			goto LOCATION_NP;
		case VK_NUMPAD_INSERT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INSERT;
			goto LOCATION_NP;
			
		case  VK_NUMPAD0:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD0;
			goto LOCATION_NP;
		case  VK_NUMPAD1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD1;
			goto LOCATION_NP;
		case  VK_NUMPAD2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD2;
			goto LOCATION_NP;
		case  VK_NUMPAD3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD3;
			goto LOCATION_NP;
		case  VK_NUMPAD4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD4;
			goto LOCATION_NP;
		case  VK_NUMPAD5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD5;
			goto LOCATION_NP;
		case  VK_NUMPAD6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD6;
			goto LOCATION_NP;
		case  VK_NUMPAD7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD7;
			goto LOCATION_NP;
		case  VK_NUMPAD8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD8;
			goto LOCATION_NP;
		case  VK_NUMPAD9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD9;
			goto LOCATION_NP;
		case VK_MULTIPLY:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MULTIPLY;
			goto LOCATION_NP;
		case VK_ADD:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ADD;
			goto LOCATION_NP;
		/* case VK_SEPARATOR:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SEPARATOR;
			goto LOCATION_NP; */
		case VK_SUBTRACT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SUBTRACT;
			goto LOCATION_NP;
		case VK_NUMPAD_RETURN:			// Windows keypad enter fires at standard location.
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ENTER;
			goto LOCATION_NP;
		case VK_DECIMAL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DECIMAL;
			goto LOCATION_NP;
		case VK_DIVIDE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DIVIDE;
			goto LOCATION_NP;
		case VK_NUMPAD_DELETE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DELETE;
			goto LOCATION_NP;
		case VK_NUMLOCK:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUM_LOCK;
			goto LOCATION_NP;
		LOCATION_NP:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;
			return jkey;


		case VK_SCROLL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SCROLL_LOCK;
			return jkey;

		case VK_F1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F1;
			return jkey;
		case VK_F2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F2;
			return jkey;
		case VK_F3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F3;
			return jkey;
		case VK_F4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F4;
			return jkey;
		case VK_F5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F5;
			return jkey;
		case VK_F6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F6;
			return jkey;
		case VK_F7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F7;
			return jkey;
		case VK_F8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F8;
			return jkey;
		case VK_F9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F9;
			return jkey;
		case VK_F10:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F10;
			return jkey;
		case VK_F11:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F11;
			return jkey;
		case VK_F12:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F12;
			return jkey;

		case VK_F13:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F13;
			return jkey;
		case VK_F14:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F14;
			return jkey;
		case VK_F15:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F15;
			return jkey;
		case VK_F16:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F16;
			return jkey;
		case VK_F17:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F17;
			return jkey;
		case VK_F18:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F18;
			return jkey;
		case VK_F19:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F19;
			return jkey;
		case VK_F20:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F20;
			return jkey;
		case VK_F21:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F21;
			return jkey;
		case VK_F22:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F22;
			return jkey;
		case VK_F23:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F23;
			return jkey;
		case VK_F24:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F24;
			return jkey;


		case VK_SNAPSHOT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PRINTSCREEN;
			return jkey;
		case VK_INSERT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INSERT;
			return jkey;
		/* case VK_HELP:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HELP;
			return jkey; */
		case VK_DELETE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DELETE;
			return jkey;


		case VK_PRIOR:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_UP;
			return jkey;
		case VK_NEXT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_DOWN;
			return jkey;
		case VK_HOME:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HOME;
			return jkey;
		case VK_END:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_END;
			return jkey;


		case VK_OEM_7:					// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_QUOTE;
			return jkey;
		case VK_OEM_3:					// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_QUOTE;
			return jkey;

		// For European keyboards
		/*
		case XK_dead_grave:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_GRAVE;
			return jkey;
		*/
		case VK_OEM_2:					// Based on Czech Keyboard Layout
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ACUTE;
			return jkey;
		/*
		case XK_dead_circumflex:		// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CIRCUMFLEX;
			return jkey;
		case XK_dead_tilde:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_TILDE;
			return jkey;
		case XK_dead_macron:			// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_MACRON;
			return jkey;
		case XK_dead_breve:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_BREVE;
			return jkey;
		case XK_dead_abovedot:			// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ABOVEDOT;
			return jkey;
		case XK_dead_diaeresis			// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_DIAERESIS;
			return jkey;
		case XK_dead_abovering:			// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ABOVERING;
			return jkey;
		case XK_dead_doubleacute:		// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_DOUBLEACUTE;
			return jkey;
		case XK_dead_caron:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CARON;
			return jkey;
		case XK_dead_cedilla:			// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CEDILLA;
			return jkey;
		case XK_dead_ogonek:			// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_OGONEK;
			return jkey;
		case XK_dead_iota:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_IOTA;
			return jkey;
		case XK_dead_voiced_sound:		// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_VOICED_SOUND;
			return jkey;
		case XK_dead_semivoiced_sound:	// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_SEMIVOICED_SOUND;
			return jkey;
		*/

		// Unknown Keyboard Codes
		/*
		case XK_ampersand:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AMPERSAND;
			return jkey;
		case XK_asterisk:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ASTERISK;
			return jkey;
		case XK_quotedbl:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_QUOTEDBL;
			return jkey;
		case XK_less:					// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LESS;
			return jkey;
		case XK_greater:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_GREATER;
			return jkey;
		case XK_braceleft:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BRACELEFT;
			return jkey;
		case XK_braceright:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BRACERIGHT;
			return jkey;
		*/

		// Unknown Extended Keyboard Codes
		/*
		case XK_at:						// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AT;
			return jkey;
		case VK_OEM_1:					// Already Defined
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COLON;
			return jkey;
		case XK_asciicircum:			// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CIRCUMFLEX;
			return jkey;
		case XK_dollar:					// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DOLLAR;
			return jkey;
		case XK_EuroSign:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EURO_SIGN;
			return jkey;
		case XK_exclam:					// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EXCLAMATION_MARK;
			return jkey;
		case XK_exclamdown:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INVERTED_EXCLAMATION_MARK;
			return jkey;
		case XK_parenleft:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT_PARENTHESIS;
			return jkey;
		case XK_numbersign:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMBER_SIGN;
			return jkey;
		case VK_OEM_PLUS:				// Already Defined
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PLUS;
			return jkey;
		case XK_parenright:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_RIGHT_PARENTHESIS;
			return jkey;
		case XK_underscore:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDERSCORE;
			return jkey;
		*/

		// For input method support on Asian Keyboards
		/* case XK_Cancel:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FINAL;
			return jkey;
		case XK_Henkan:					// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONVERT;
			return jkey;
		case XK_Muhenkan:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NONCONVERT;
			return jkey;
		case XK_VoidSymbol:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ACCEPT;
			return jkey;
		case XK_Mode_switch:			// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MODECHANGE;
			return jkey; */
		case VK_KANA:					// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANA;
			return jkey;
		case VK_KANJI:					// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANJI;
			return jkey;
		case VK_DBE_ALPHANUMERIC:		// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALPHANUMERIC;
			return jkey;
		case VK_DBE_KATAKANA:			// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KATAKANA;
			return jkey;
		case VK_DBE_HIRAGANA:			// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HIRAGANA;
			return jkey;
		case VK_DBE_DBCSCHAR:			// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FULL_WIDTH;
			return jkey;
		case VK_DBE_SBCSCHAR:			// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HALF_WIDTH;
			return jkey;
		case VK_DBE_ROMAN:

		case VK_DBE_NOROMAN:			// TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ROMAN_CHARACTERS;
			return jkey;
		/* case XK_Zen_Koho:			// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALL_CANDIDATES;
			return jkey;
		case XK_Mae_Koho:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PREVIOUS_CANDIDATE;
			return jkey; */
		case VK_DBE_CODEINPUT:			// TODO Testing Needed
		case VK_DBE_NOCODEINPUT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CODE_INPUT;
			return jkey;
		/* case XK_Hiragana_Katakana:	// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_KATAKANA;
			return jkey;
		case XK_Hiragana_Katakana:		// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_HIRAGANA;
			return jkey;
		case XK_Romaji:					// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_ROMAN;
			return jkey;
		case XK_Kana_Lock:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANA_LOCK;
			return jkey;
		case XK_VoidSymbol:				// Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INPUT_METHOD_ON_OFF;
			return jkey; */

		// For Sun keyboards
		/*
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:					// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CUT;
			return jkey;
		case SunXK_Copy:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COPY;
			return jkey;
		case SunXK_Paste:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PASTE;
			return jkey;
		case SunXK_Undo:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDO;
			return jkey;
		case SunXK_Again:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AGAIN;
			return jkey;
		case SunXK_Find:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FIND;
			return jkey;
		case SunXK_Props:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PROPS;
			return jkey;
		case SunXK_Stop:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_STOP;
			return jkey;
		case SunXK_Compose:				// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COMPOSE;
			return jkey;
		case SunXK_AltGraph:			// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT_GRAPH;
			return jkey;
		#endif
		*/

		/* case XK_Begin:					// No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BEGIN;
			return jkey; */

		default:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED;
			return jkey;
	}
}

jint NativeToJButton (unsigned int button) {
	switch (button) {
		case VK_LBUTTON:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON1;
		case VK_RBUTTON:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON2;
		case VK_MBUTTON:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON3;
		case VK_XBUTTON1:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON4;
		case VK_XBUTTON2:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON5;

		default:
			return org_jnativehook_mouse_NativeMouseEvent_NOBUTTON;
	}
}

jint NativeToJEventMask(unsigned int mask) {
	jint java_mask = 0;

	if (mask & MOD_LSHIFT || mask & MOD_RSHIFT)
		java_mask |= org_jnativehook_NativeInputEvent_SHIFT_MASK;

	if (mask & MOD_LCONTROL || mask & MOD_RCONTROL)
		java_mask |= org_jnativehook_NativeInputEvent_CTRL_MASK;

	if (mask & MOD_LWIN || mask & MOD_RWIN)
		java_mask |= org_jnativehook_NativeInputEvent_META_MASK;

	if (mask & MOD_LALT || mask & MOD_RALT)
		java_mask |= org_jnativehook_NativeInputEvent_ALT_MASK;


	if (mask & MOD_LBUTTON)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON1_MASK;

	if (mask & MOD_RBUTTON)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON2_MASK;

	if (mask & MOD_MBUTTON)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON3_MASK;

	if (mask & MOD_XBUTTON1)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON4_MASK;

	if (mask & MOD_XBUTTON2)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON5_MASK;


	return java_mask;
}
