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

// Reference: http://developer.apple.com/mac/library/documentation/Carbon/Reference/CarbonRefUpdate/Articles/Carbon_10.4-10.5_SymbolChanges.html
#include <ApplicationServices/ApplicationServices.h> // For CoreGraphics kCGEventFlagMask constants.

#include "NativeToJava.h"
#include "OSXInputHelpers.h"

JKeyDatum NativeToJKey(unsigned int keysym) {
	JKeyDatum jkey;
	jkey.rawcode = keysym;
	jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD;

	switch ((unsigned int) keysym) {
		case kVK_Return:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ENTER;
			return jkey;
		case kVK_Delete:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_SPACE;
			return jkey;
		case kVK_Tab:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_TAB;
			return jkey;
		/* case XK_Cancel:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CANCEL;
			return jkey; */


		case kVK_Shift:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SHIFT;
			goto LOCATION_L;
		case kVK_Control:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTROL;
			goto LOCATION_L;
		case kVK_Option:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT;
			goto LOCATION_L;
		case kVK_Command:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_META;
			goto LOCATION_L;
		/* case XK_Super_L:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_WINDOWS;
			return jkey; */
		LOCATION_L:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;

		case kVK_RightShift:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SHIFT;
			goto LOCATION_R;
		case kVK_RightControl:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTROL;
			goto LOCATION_R;
		case kVK_RightOption:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT;
			goto LOCATION_R;
		case kVK_RightCommand:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_META;
			goto LOCATION_R;
		/* case XK_Super_R:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_WINDOWS;
			goto LOCATION_R; */
		LOCATION_R:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;
			return jkey;

		case kVK_ContextMenu:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTEXT_MENU;
			return jkey;


		/* case XK_Pause:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAUSE;
			return jkey; */
		case kVK_CapsLock:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CAPS_LOCK;
			return jkey;
		case kVK_Escape:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ESCAPE;
			return jkey;
		case kVK_Space:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SPACE;
			return jkey;


		case kVK_UpArrow:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UP;
			return jkey;
		case kVK_DownArrow:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DOWN;
			return jkey;
		case kVK_LeftArrow:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT;
			return jkey;
		case kVK_RightArrow:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_RIGHT;
			return jkey;

		case kVK_ANSI_Comma:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COMMA;
			return jkey;
		case kVK_ANSI_Minus:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MINUS;
			return jkey;
		case kVK_ANSI_Period:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PERIOD;
			return jkey;
		case kVK_ANSI_Slash:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SLASH;
			return jkey;


		case kVK_ANSI_0:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_0;
			return jkey;
		case kVK_ANSI_1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_1;
			return jkey;
		case kVK_ANSI_2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_2;
			return jkey;
		case kVK_ANSI_3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_3;
			return jkey;
		case kVK_ANSI_4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_4;
			return jkey;
		case kVK_ANSI_5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_5;
			return jkey;
		case kVK_ANSI_6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_6;
			return jkey;
		case kVK_ANSI_7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_7;
			return jkey;
		case kVK_ANSI_8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_8;
			return jkey;
		case kVK_ANSI_9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_9;
			return jkey;


		case kVK_ANSI_Semicolon:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SEMICOLON;
			return jkey;
		case kVK_ANSI_Equal:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EQUALS;
			return jkey;


		case kVK_ANSI_A:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_A;
			return jkey;
		case kVK_ANSI_B:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_B;
			return jkey;
		case kVK_ANSI_C:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_C;
			return jkey;
		case kVK_ANSI_D:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_D;
			return jkey;
		case kVK_ANSI_E:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_E;
			return jkey;
		case kVK_ANSI_F:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F;
			return jkey;
		case kVK_ANSI_G:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_G;
			return jkey;
		case kVK_ANSI_H:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_H;
			return jkey;
		case kVK_ANSI_I:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_I;
			return jkey;
		case kVK_ANSI_J:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_J;
			return jkey;
		case kVK_ANSI_K:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_K;
			return jkey;
		case kVK_ANSI_L:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_L;
			return jkey;
		case kVK_ANSI_M:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_M;
			return jkey;
		case kVK_ANSI_N:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_N;
			return jkey;
		case kVK_ANSI_O:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_O;
			return jkey;
		case kVK_ANSI_P:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_P;
			return jkey;
		case kVK_ANSI_Q:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Q;
			return jkey;
		case kVK_ANSI_R:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_R;
			return jkey;
		case kVK_ANSI_S:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_S;
			return jkey;
		case kVK_ANSI_T:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_T;
			return jkey;
		case kVK_ANSI_U:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_U;
			return jkey;
		case kVK_ANSI_V:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_V;
			return jkey;
		case kVK_ANSI_W:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_W;
			return jkey;
		case kVK_ANSI_X:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_X;
			return jkey;
		case kVK_ANSI_Y:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Y;
			return jkey;
		case kVK_ANSI_Z:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Z;
			return jkey;


		case kVK_ANSI_LeftBracket:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_OPEN_BRACKET;
			return jkey;
		case kVK_ANSI_Backslash:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_SLASH;
			return jkey;
		case kVK_ANSI_RightBracket:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CLOSE_BRACKET;
			return jkey;


		case kVK_ANSI_Keypad0:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD0;
			goto LOCATION_NP;
		case kVK_ANSI_Keypad1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD1;
			goto LOCATION_NP;
		case kVK_ANSI_Keypad2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD2;
			goto LOCATION_NP;
		case kVK_ANSI_Keypad3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD3;
			goto LOCATION_NP;
		case kVK_ANSI_Keypad4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD4;
			goto LOCATION_NP;
		case kVK_ANSI_Keypad5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD5;
			goto LOCATION_NP;
		case kVK_ANSI_Keypad6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD6;
			goto LOCATION_NP;
		case kVK_ANSI_Keypad7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD7;
			goto LOCATION_NP;
		case kVK_ANSI_Keypad8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD8;
			goto LOCATION_NP;
		case kVK_ANSI_Keypad9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD9;
			goto LOCATION_NP;

		case kVK_ANSI_KeypadMultiply:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MULTIPLY;
			goto LOCATION_NP;
		case kVK_ANSI_KeypadPlus:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ADD;
			goto LOCATION_NP;
		/* case XK_KP_Separator:			// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SEPARATOR;
			goto LOCATION_NP; */
		case kVK_ANSI_KeypadMinus:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SUBTRACT;
			goto LOCATION_NP;
		case kVK_ANSI_KeypadDecimal:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DECIMAL;
			goto LOCATION_NP;
		case kVK_ANSI_KeypadDivide:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DIVIDE;
			goto LOCATION_NP;
		case kVK_ANSI_KeypadClear:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CLEAR;
			goto LOCATION_NP;
		/* case XK_Num_Lock:				// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUM_LOCK;
			goto LOCATION_NP;
		case XK_Scroll_Lock:				// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SCROLL_LOCK;
			goto LOCATION_NP; */
		LOCATION_NP:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;
			return jkey;

		case kVK_F1:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F1;
			return jkey;
		case kVK_F2:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F2;
			return jkey;
		case kVK_F3:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F3;
			return jkey;
		case kVK_F4:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F4;
			return jkey;
		case kVK_F5:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F5;
			return jkey;
		case kVK_F6:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F6;
			return jkey;
		case kVK_F7:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F7;
			return jkey;
		case kVK_F8:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F8;
			return jkey;
		case kVK_F9:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F9;
			return jkey;
		case kVK_F10:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F10;
			return jkey;
		case kVK_F11:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F11;
			return jkey;
		case kVK_F12:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F12;
			return jkey;

		case kVK_F13:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F13;
			return jkey;
		case kVK_F14:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F14;
			return jkey;
		case kVK_F15:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F15;
			return jkey;
		case kVK_F16:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F16;
			return jkey;
		case kVK_F17:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F17;
			return jkey;
		case kVK_F18:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F18;
			return jkey;
		case kVK_F19:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F19;
			return jkey;
		case kVK_F20:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F20;
			return jkey;
		/* case XK_F21:						// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F21;
			return jkey;
		case XK_F22:						// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F22;
			return jkey;
		case XK_F23:						// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F23;
			return jkey;
		case XK_F24:						// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F24;
			return jkey; */


		/* case XK_Print:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PRINTSCREEN;
			return jkey;
		case XK_Insert:						// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INSERT;
			return jkey; */
		case kVK_Help:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HELP;
			return jkey;
		case kVK_ForwardDelete:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DELETE;
			return jkey;


		case kVK_PageUp:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_UP;
			return jkey;
		case kVK_PageDown:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_DOWN;
			return jkey;
		case kVK_Home:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HOME;
			return jkey;
		case kVK_End:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_END;
			return jkey;


		case kVK_ANSI_Quote:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_QUOTE;
			return jkey;
		case kVK_ANSI_Grave:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_QUOTE;
			return jkey;


		// For European keyboards
		/*
		case XK_dead_grave:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_GRAVE;
			return jkey;
		case XK_dead_acute:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ACUTE;
			return jkey;
		case XK_dead_circumflex:			* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CIRCUMFLEX;
			return jkey;
		case XK_dead_tilde:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_TILDE;
			return jkey;
		case XK_dead_macron:				* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_MACRON;
			return jkey;
		case XK_dead_breve:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_BREVE;
			return jkey;
		case XK_dead_abovedot:				* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ABOVEDOT;
			return jkey;
		case XK_dead_diaeresis:				* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_DIAERESIS;
			return jkey;
		case XK_dead_abovering:				* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ABOVERING;
			return jkey;
		case XK_dead_doubleacute:			* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_DOUBLEACUTE;
			return jkey;
		case XK_dead_caron:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CARON;
			return jkey;
		case XK_dead_cedilla:				* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CEDILLA;
			return jkey;
		case XK_dead_ogonek:				* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_OGONEK;
			return jkey;
		case XK_dead_iota:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_IOTA;
			return jkey;
		case XK_dead_voiced_sound:			* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_VOICED_SOUND;
			return jkey;
		case XK_dead_semivoiced_sound:		* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_SEMIVOICED_SOUND;
			return jkey;
		*/


		// Unknown Keyboard Codes
		/*
		case XK_ampersand:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AMPERSAND;
			return jkey;
		case XK_asterisk:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ASTERISK;
			return jkey;
		case XK_quotedbl:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_QUOTEDBL;
			return jkey;
		case XK_less:						// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LESS;
			return jkey;
		case XK_greater:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_GREATER;
			return jkey;
		case XK_braceleft:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BRACELEFT;
			return jkey;
		case XK_braceright:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BRACERIGHT;
			return jkey;
		*/


		// Unknown Extended Keyboard Codes
		/*
		case XK_at:							* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AT;
			return jkey;
		case XK_colon:						* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COLON;
			return jkey;
		case XK_asciicircum:				* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CIRCUMFLEX;
			return jkey;
		case XK_dollar:						* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DOLLAR;
			return jkey;
		case XK_EuroSign:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EURO_SIGN;
			return jkey;
		case XK_exclam:						* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EXCLAMATION_MARK;
			return jkey;
		case XK_exclamdown:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INVERTED_EXCLAMATION_MARK;
			return jkey;
		case XK_parenleft:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT_PARENTHESIS;
			return jkey;
		case XK_numbersign:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMBER_SIGN;
			return jkey;
		case XK_plus:						* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PLUS;
			return jkey;
		case XK_parenright:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_RIGHT_PARENTHESIS;
			return jkey;
		case kVK_JIS_Underscore:			* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDERSCORE;
			return jkey;
		*/


		// For input method support on Asian Keyboards
		/* case XK_Cancel:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FINAL;
			return jkey;
		case XK_Henkan:						* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONVERT;
			return jkey;
		case XK_Muhenkan:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NONCONVERT;
			return jkey;
		case XK_VoidSymbol:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ACCEPT;
			return jkey;
		case XK_Mode_switch:				* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MODECHANGE;
			return jkey; */
		case kVK_JIS_Kana:					/* TODO Testing Needed */
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANA;
			return jkey;
		/* case XK_Kanji:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANJI;
			return jkey; */
		case kVK_JIS_Eisu:					/* TODO Testing Needed */
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALPHANUMERIC;
			return jkey;
		/* case XK_Katakana:				* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KATAKANA;
			return jkey;
		case XK_Hiragana:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HIRAGANA;
			return jkey;
		case XK_Zenkaku:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FULL_WIDTH;
			return jkey;
		case XK_Hankaku:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HALF_WIDTH;
			return jkey;
		case XK_Romaji:						* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ROMAN_CHARACTERS;
			return jkey;
		case XK_Zen_Koho:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALL_CANDIDATES;
			return jkey;
		case XK_Mae_Koho:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PREVIOUS_CANDIDATE;
			return jkey;
		case XK_Kanji_Bangou:				* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CODE_INPUT;
			return jkey;
		case XK_Hiragana_Katakana:			* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_KATAKANA;
			return jkey;
		case XK_Hiragana_Katakana:			* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_HIRAGANA;
			return jkey;
		case XK_Romaji:						* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_ROMAN;
			return jkey;
		case XK_Kana_Lock:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANA_LOCK;
			return jkey;
		case XK_VoidSymbol:					* Unknown Apple Support *
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INPUT_METHOD_ON_OFF;
			return jkey; */


		// For Sun keyboards
		/*
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:						// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CUT;
			return jkey;
		case SunXK_Copy:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COPY;
			return jkey;
		case SunXK_Paste:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PASTE;
			return jkey;
		case SunXK_Undo:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDO;
			return jkey;
		case SunXK_Again:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AGAIN;
			return jkey;
		case SunXK_Find:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FIND;
			return jkey;
		case SunXK_Props:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PROPS;
			return jkey;
		case SunXK_Stop:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_STOP;
			return jkey;
		case SunXK_Compose:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COMPOSE;
			return jkey;
		case SunXK_AltGraph:				// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT_GRAPH;
			return jkey;
		#endif
		*/

		/* case XK_Begin:					// No Apple Support
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BEGIN;
			return jkey; */

		default:
		case 0xFF:
			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED;
			return jkey;
	}
}

jint NativeToJButton(unsigned int button) {
	switch (button) {
		case kVK_LBUTTON:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON1;
		case kVK_RBUTTON:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON2;
		case kVK_MBUTTON:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON3;
		case kVK_XBUTTON1:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON4;
		case kVK_XBUTTON2:
			return org_jnativehook_mouse_NativeMouseEvent_BUTTON5;

		default:
			return org_jnativehook_mouse_NativeMouseEvent_NOBUTTON;
	}
}

jint NativeToJEventMask(unsigned int mask) {
	jint java_mask = 0;

	if (mask & kCGEventFlagMaskShift)
		java_mask |= org_jnativehook_NativeInputEvent_SHIFT_MASK;

	if (mask & kCGEventFlagMaskControl)
		java_mask |= org_jnativehook_NativeInputEvent_CTRL_MASK;

	if (mask & kCGEventFlagMaskCommand)
		java_mask |= org_jnativehook_NativeInputEvent_META_MASK;

	if (mask & kCGEventFlagMaskAlternate)
		java_mask |= org_jnativehook_NativeInputEvent_ALT_MASK;


	if (mask & kCGEventFlagMaskButtonLeft)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON1_MASK;

	if (mask & kCGEventFlagMaskButtonRight)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON2_MASK;

	if (mask & kCGEventFlagMaskButtonCenter)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON3_MASK;

	if (mask & kCGEventFlagMaskXButton1)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON4_MASK;

	if (mask & kCGEventFlagMaskXButton2)
		java_mask |= org_jnativehook_NativeInputEvent_BUTTON5_MASK;

	return java_mask;
}
