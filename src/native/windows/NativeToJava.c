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

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>

#include "NativeToJava.h"
#include "WinKeyCodes.h"

JKeyDatum NativeToJKey(unsigned int keysym) {
	JKeyDatum jkey;
	//FIXME Check and see if we should use the virtual key code or the scan code.
	jkey.rawcode = MapVirtualKey(keysym, MAPVK_VK_TO_VSC);
	jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD;

	switch (keysym) {
		case VK_RETURN:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ENTER;
			return jkey;
		case VK_BACK:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BACK_SPACE;
			return jkey;
		case VK_TAB:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_TAB;
			return jkey;
		case VK_CANCEL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CANCEL;
			return jkey;


		case VK_LSHIFT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SHIFT;
			goto LOCATION_L;
		case VK_LCONTROL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CONTROL;
			goto LOCATION_L;
		case VK_LMENU:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ALT;
			goto LOCATION_L;
		case VK_LWIN:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_WINDOWS;
			goto LOCATION_L;
		LOCATION_L:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;
			return jkey;
			
		case VK_RSHIFT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SHIFT;
			goto LOCATION_R;
		case VK_RCONTROL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CONTROL;
			goto LOCATION_R;
		case VK_RMENU:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ALT;
			goto LOCATION_R;
		case VK_RWIN:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_WINDOWS;
			goto LOCATION_R;
		LOCATION_R:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;
			return jkey;


		case VK_APPS:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CONTEXT_MENU;
			return jkey;

		case VK_PAUSE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PAUSE;
			return jkey;
		case VK_CAPITAL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CAPS_LOCK;
			return jkey;
		case VK_ESCAPE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ESCAPE;
			return jkey;
		case VK_SPACE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SPACE;
			return jkey;


		case VK_UP:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_UP;
			return jkey;
		case VK_DOWN:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DOWN;
			return jkey;
		case VK_LEFT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_LEFT;
			return jkey;
		case VK_RIGHT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_RIGHT;
			return jkey;


		case VK_OEM_COMMA:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_COMMA;
			return jkey;
		case VK_OEM_MINUS:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_MINUS;
			return jkey;
		case VK_OEM_PERIOD:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PERIOD;
			return jkey;
		case VK_OEM_2:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SLASH;
			return jkey;


		case 0x30:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_0;
			return jkey;
		case 0x31:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_1;
			return jkey;
		case 0x32:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_2;
			return jkey;
		case 0x33:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_3;
			return jkey;
		case 0x34:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_4;
			return jkey;
		case 0x35:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_5;
			return jkey;
		case 0x36:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_6;
			return jkey;
		case 0x37:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_7;
			return jkey;
		case 0x38:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_8;
			return jkey;
		case 0x39:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_9;
			return jkey;


		case VK_OEM_1:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SEMICOLON;
			return jkey;
		case VK_OEM_PLUS:				//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_EQUALS;
			return jkey;


		case 0x41:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_A;
			return jkey;
		case 0x42:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_B;
			return jkey;
		case 0x43:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_C;
			return jkey;
		case 0x44:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_D;
			return jkey;
		case 0x45:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_E;
			return jkey;
		case 0x46:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F;
			return jkey;
		case 0x47:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_G;
			return jkey;
		case 0x48:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_H;
			return jkey;
		case 0x49:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_I;
			return jkey;
		case 0x4A:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_J;
			return jkey;
		case 0x4B:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_K;
			return jkey;
		case 0x4C:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_L;
			return jkey;
		case 0x4D:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_M;
			return jkey;
		case 0x4E:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_N;
			return jkey;
		case 0x4F:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_O;
			return jkey;
		case 0x50:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_P;
			return jkey;
		case 0x51:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_Q;
			return jkey;
		case 0x52:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_R;
			return jkey;
		case 0x53:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_S;
			return jkey;
		case 0x54:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_T;
			return jkey;
		case 0x55:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_U;
			return jkey;
		case 0x56:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_V;
			return jkey;
		case 0x57:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_W;
			return jkey;
		case 0x58:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_X;
			return jkey;
		case 0x59:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_Y;
			return jkey;
		case 0x5A:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_Z;
			return jkey;


		case VK_OEM_4:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_OPEN_BRACKET;
			return jkey;
		case VK_OEM_5:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BACK_SLASH;
			return jkey;
		case VK_OEM_6:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CLOSE_BRACKET;
			return jkey;


		case  VK_NUMPAD0:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD0;
			goto LOCATION_NP;
		case  VK_NUMPAD1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD1;
			goto LOCATION_NP;
		case  VK_NUMPAD2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD2;
			goto LOCATION_NP;
		case  VK_NUMPAD3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD3;
			goto LOCATION_NP;
		case  VK_NUMPAD4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD4;
			goto LOCATION_NP;
		case  VK_NUMPAD5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD5;
			goto LOCATION_NP;
		case  VK_NUMPAD6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD6;
			goto LOCATION_NP;
		case  VK_NUMPAD7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD7;
			goto LOCATION_NP;
		case  VK_NUMPAD8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD8;
			goto LOCATION_NP;
		case  VK_NUMPAD9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMPAD9;
			goto LOCATION_NP;
		case VK_MULTIPLY:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_MULTIPLY;
			goto LOCATION_NP;
		case VK_ADD:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ADD;
			goto LOCATION_NP;
		//case VK_SEPARATOR:
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SEPARATOR;
		//	goto LOCATION_NP;
		case VK_SUBTRACT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SUBTRACT;
			goto LOCATION_NP;
		//case XK_KP_Enter:
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ENTER;
		//	goto LOCATION_NP;
		case VK_DECIMAL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DECIMAL;
			goto LOCATION_NP;
		case VK_DIVIDE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DIVIDE;
			goto LOCATION_NP;
		//case VK_DELETE:
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DELETE;
		//	goto LOCATION_NP;
		case VK_NUMLOCK:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUM_LOCK;
			goto LOCATION_NP;
		LOCATION_NP:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;
			return jkey;


		case VK_SCROLL:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_SCROLL_LOCK;
			return jkey;

		case VK_F1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F1;
			return jkey;
		case VK_F2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F2;
			return jkey;
		case VK_F3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F3;
			return jkey;
		case VK_F4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F4;
			return jkey;
		case VK_F5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F5;
			return jkey;
		case VK_F6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F6;
			return jkey;
		case VK_F7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F7;
			return jkey;
		case VK_F8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F8;
			return jkey;
		case VK_F9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F9;
			return jkey;
		case VK_F10:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F10;
			return jkey;
		case VK_F11:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F11;
			return jkey;
		case VK_F12:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F12;
			return jkey;

		case VK_F13:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F13;
			return jkey;
		case VK_F14:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F14;
			return jkey;
		case VK_F15:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F15;
			return jkey;
		case VK_F16:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F16;
			return jkey;
		case VK_F17:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F17;
			return jkey;
		case VK_F18:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F18;
			return jkey;
		case VK_F19:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F19;
			return jkey;
		case VK_F20:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F20;
			return jkey;
		case VK_F21:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F21;
			return jkey;
		case VK_F22:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F22;
			return jkey;
		case VK_F23:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F23;
			return jkey;
		case VK_F24:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_F24;
			return jkey;


		case VK_SNAPSHOT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PRINTSCREEN;
			return jkey;
		case VK_INSERT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_INSERT;
			return jkey;
		//case VK_HELP:
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_HELP;
		//	return jkey;
		case VK_DELETE:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DELETE;
			return jkey;


		case VK_PRIOR:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PAGE_UP;
			return jkey;
		case VK_NEXT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PAGE_DOWN;
			return jkey;
		case VK_HOME:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_HOME;
			return jkey;
		case VK_END:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_END;
			return jkey;


		case VK_OEM_7:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_QUOTE;
			return jkey;
		case VK_OEM_3:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BACK_QUOTE;
			return jkey;

		/* For European keyboards */
		/*
		case XK_dead_grave:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_GRAVE;
			return jkey;
		case XK_dead_acute:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_ACUTE;
			return jkey;
		case XK_dead_circumflex:		//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_CIRCUMFLEX;
			return jkey;
		case XK_dead_tilde:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_TILDE;
			return jkey;
		case XK_dead_macron:			//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_MACRON;
			return jkey;
		case XK_dead_breve:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_BREVE;
			return jkey;
		case XK_dead_abovedot:			//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_ABOVEDOT;
			return jkey;
		case XK_dead_diaeresis			//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_DIAERESIS;
			return jkey;
		case XK_dead_abovering:			//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_ABOVERING;
			return jkey;
		case XK_dead_doubleacute:		//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_DOUBLEACUTE;
			return jkey;
		case XK_dead_caron:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_CARON;
			return jkey;
		case XK_dead_cedilla:			//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_CEDILLA;
			return jkey;
		case XK_dead_ogonek:			//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_OGONEK;
			return jkey;
		case XK_dead_iota:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_IOTA;
			return jkey;
		case XK_dead_voiced_sound:		//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_VOICED_SOUND;
			return jkey;
		case XK_dead_semivoiced_sound:	//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DEAD_SEMIVOICED_SOUND;
			return jkey;
		*/

		/* Unknown Keyboard Codes */
		/*
		case XK_ampersand:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_AMPERSAND;
			return jkey;
		case XK_asterisk:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ASTERISK;
			return jkey;
		case XK_quotedbl:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_QUOTEDBL;
			return jkey;
		case XK_less:					//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_LESS;
			return jkey;
		case XK_greater:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_GREATER;
			return jkey;
		case XK_braceleft:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BRACELEFT;
			return jkey;
		case XK_braceright:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BRACERIGHT;
			return jkey;
		*/

		/* Unknown Extended Keyboard Codes */
		/*
		case XK_at:						//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_AT;
			return jkey;
		case VK_OEM_1:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_COLON;
			return jkey;
		case XK_asciicircum:			//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CIRCUMFLEX;
			return jkey;
		case XK_dollar:					//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_DOLLAR;
			return jkey;
		case XK_EuroSign:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_EURO_SIGN;
			return jkey;
		case XK_exclam:					//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_EXCLAMATION_MARK;
			return jkey;
		case XK_exclamdown:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_INVERTED_EXCLAMATION_MARK;
			return jkey;
		case XK_parenleft:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_LEFT_PARENTHESIS;
			return jkey;
		case XK_numbersign:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NUMBER_SIGN;
			return jkey;	
		case VK_OEM_PLUS:				//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PLUS;
			return jkey;
		case XK_parenright:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_RIGHT_PARENTHESIS;
			return jkey;
		case XK_underscore:				//Unknown Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_UNDERSCORE;
			return jkey;
		*/

		/* For input method support on Asian Keyboards */
		//case XK_Cancel:				//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_FINAL;
		//	return jkey;
		//case XK_Henkan:				//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CONVERT;
		//	return jkey;
		//case XK_Muhenkan:				//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_NONCONVERT;
		//	return jkey;
		//case XK_VoidSymbol:			//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ACCEPT;
		//	return jkey;
		//case XK_Mode_switch:			//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_MODECHANGE;
		//	return jkey;
		case VK_KANA:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_KANA;
			return jkey;
		case VK_KANJI:					//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_KANJI;
			return jkey;
		case VK_DBE_ALPHANUMERIC:		//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ALPHANUMERIC;
			return jkey;
		case VK_DBE_KATAKANA:			//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_KATAKANA;
			return jkey;
		case VK_DBE_HIRAGANA:			//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_HIRAGANA;
			return jkey;
		case VK_DBE_DBCSCHAR:			//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_FULL_WIDTH;
			return jkey;
		case VK_DBE_SBCSCHAR:			//TODO Testing Needed
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_HALF_WIDTH;
			return jkey;
		case VK_DBE_ROMAN:

		case VK_DBE_NOROMAN:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ROMAN_CHARACTERS;				return jkey;	//TODO Testing Needed
		//case XK_Zen_Koho:				//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ALL_CANDIDATES;
		//	return jkey;
		//case XK_Mae_Koho:				//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PREVIOUS_CANDIDATE;
		//	return jkey;
		case VK_DBE_CODEINPUT:			//TODO Testing Needed
		case VK_DBE_NOCODEINPUT:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CODE_INPUT;
			return jkey;
		//case XK_Hiragana_Katakana:	//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_JAPANESE_KATAKANA;
		//	return jkey;
		//case XK_Hiragana_Katakana:	//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_JAPANESE_HIRAGANA;
		//	return jkey;
		//case XK_Romaji:				//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_JAPANESE_ROMAN;
		//	return jkey;
		//case XK_Kana_Lock:			//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_KANA_LOCK;
		//	return jkey;
		//case XK_VoidSymbol:			//Unknown Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_INPUT_METHOD_ON_OFF;
		//	return jkey;

		/* For Sun keyboards */
		/*
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:					//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_CUT;
			return jkey;
		case SunXK_Copy:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_COPY;
			return jkey;
		case SunXK_Paste:				//No Microsoft Support
		 	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PASTE;
			return jkey;
		case SunXK_Undo:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_UNDO;
			return jkey;
		case SunXK_Again:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_AGAIN;
			return jkey;
		case SunXK_Find:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_FIND;
			return jkey;
		case SunXK_Props:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_PROPS;
			return jkey;
		case SunXK_Stop:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_STOP;

		case SunXK_Compose:				//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_COMPOSE;
			return jkey;
		case SunXK_AltGraph:			//No Microsoft Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_ALT_GRAPH;
			return jkey;
		#endif
		*/

		//case XK_Begin:				//No Microsoft Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_BEGIN;
		//	return jkey;

		default:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_UNDEFINED;
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

jint NativeToJModifier(unsigned int mod) {
	switch (mod) {
		case MOD_SHIFT:
			return org_jnativehook_NativeInputEvent_SHIFT_MASK;
		case MOD_CONTROL:
			return org_jnativehook_NativeInputEvent_CTRL_MASK;
		case MOD_WIN:
			return org_jnativehook_NativeInputEvent_META_MASK;
		case MOD_ALT:
			return org_jnativehook_NativeInputEvent_ALT_MASK;

		case MOD_LBUTTON:
			return org_jnativehook_NativeInputEvent_BUTTON1_MASK;
		case MOD_RBUTTON:
			return org_jnativehook_NativeInputEvent_BUTTON2_MASK;
		case MOD_MBUTTON:
			return org_jnativehook_NativeInputEvent_BUTTON3_MASK;
		case MOD_XBUTTON1:
			return org_jnativehook_NativeInputEvent_BUTTON4_MASK;
		case MOD_XBUTTON2:
			return org_jnativehook_NativeInputEvent_BUTTON5_MASK;

		default:
			return org_jnativehook_keyboard_NativeKeyEvent_UNDEFINED;
	}
}
