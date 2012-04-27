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

//Reference: http://developer.apple.com/mac/library/documentation/Carbon/Reference/CarbonRefUpdate/Articles/Carbon_10.4-10.5_SymbolChanges.html
#include <ApplicationServices/ApplicationServices.h> //For CoreGraphics kCGEventFlagMask constants

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
		//case XK_Cancel:					//No Apple Support
		//	jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CANCEL;
		//	return jkey;

		case kVK_Shift:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SHIFT;						return jkey;
		case kVK_RightShift:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SHIFT;						return jkey;
		case kVK_Control:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTROL;						return jkey;
		case kVK_RightControl:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTROL;						return jkey;
		case kVK_Option:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT;							return jkey;
		case kVK_RightOption:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT;							return jkey;
		case kVK_Command:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_META;							return jkey;
		case kVK_RightCommand:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_META;							return jkey;
		//case XK_Super_L:
		//	jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_WINDOWS;						return jkey;	//No Apple Support
		//case XK_Super_R:
		//	jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_WINDOWS;						return jkey;	//No Apple Support
		case kVK_ContextMenu:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONTEXT_MENU;					return jkey;

		//case XK_Pause:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAUSE;						return jkey;	//No Apple Support
		case kVK_CapsLock:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CAPS_LOCK;					return jkey;
		case kVK_Escape:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ESCAPE;						return jkey;
		case kVK_Space:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SPACE;						return jkey;

		case kVK_UpArrow:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UP;							return jkey;
		case kVK_DownArrow:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DOWN;							return jkey;
		case kVK_LeftArrow:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT;							return jkey;
		case kVK_RightArrow:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_RIGHT;						return jkey;

		case kVK_ANSI_Comma:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COMMA;						return jkey;
		case kVK_ANSI_Minus:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MINUS;						return jkey;
		case kVK_ANSI_Period:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PERIOD;						return jkey;
		case kVK_ANSI_Slash:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SLASH;						return jkey;

		case kVK_ANSI_0:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_0;							return jkey;
		case kVK_ANSI_1:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_1;							return jkey;
		case kVK_ANSI_2:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_2;							return jkey;
		case kVK_ANSI_3:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_3;							return jkey;
		case kVK_ANSI_4:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_4;							return jkey;
		case kVK_ANSI_5:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_5;							return jkey;
		case kVK_ANSI_6:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_6;							return jkey;
		case kVK_ANSI_7:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_7;							return jkey;
		case kVK_ANSI_8:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_8;							return jkey;
		case kVK_ANSI_9:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_9;							return jkey;

		case kVK_ANSI_Semicolon:						jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SEMICOLON;					return jkey;
		case kVK_ANSI_Equal:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EQUALS;						return jkey;

		case kVK_ANSI_A:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_A;							return jkey;
		case kVK_ANSI_B:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_B;							return jkey;
		case kVK_ANSI_C:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_C;							return jkey;
		case kVK_ANSI_D:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_D;							return jkey;
		case kVK_ANSI_E:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_E;							return jkey;
		case kVK_ANSI_F:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F;							return jkey;
		case kVK_ANSI_G:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_G;							return jkey;
		case kVK_ANSI_H:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_H;							return jkey;
		case kVK_ANSI_I:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_I;							return jkey;
		case kVK_ANSI_J:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_J;							return jkey;
		case kVK_ANSI_K:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_K;							return jkey;
		case kVK_ANSI_L:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_L;							return jkey;
		case kVK_ANSI_M:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_M;							return jkey;
		case kVK_ANSI_N:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_N;							return jkey;
		case kVK_ANSI_O:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_O;							return jkey;
		case kVK_ANSI_P:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_P;							return jkey;
		case kVK_ANSI_Q:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Q;							return jkey;
		case kVK_ANSI_R:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_R;							return jkey;
		case kVK_ANSI_S:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_S;							return jkey;
		case kVK_ANSI_T:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_T;							return jkey;
		case kVK_ANSI_U:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_U;							return jkey;
		case kVK_ANSI_V:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_V;							return jkey;
		case kVK_ANSI_W:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_W;							return jkey;
		case kVK_ANSI_X:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_X;							return jkey;
		case kVK_ANSI_Y:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Y;							return jkey;
		case kVK_ANSI_Z:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_Z;							return jkey;

		case kVK_ANSI_LeftBracket:						jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_OPEN_BRACKET;					return jkey;
		case kVK_ANSI_Backslash:						jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_SLASH;					return jkey;
		case kVK_ANSI_RightBracket:						jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CLOSE_BRACKET;				return jkey;

		case kVK_ANSI_Keypad0:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD0;						return jkey;
		case kVK_ANSI_Keypad1:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD1;						return jkey;
		case kVK_ANSI_Keypad2:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD2;						return jkey;
		case kVK_ANSI_Keypad3:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD3;						return jkey;
		case kVK_ANSI_Keypad4:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD4;						return jkey;
		case kVK_ANSI_Keypad5:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD5;						return jkey;
		case kVK_ANSI_Keypad6:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD6;						return jkey;
		case kVK_ANSI_Keypad7:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD7;						return jkey;
		case kVK_ANSI_Keypad8:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD8;						return jkey;
		case kVK_ANSI_Keypad9:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMPAD9;						return jkey;

		case kVK_ANSI_KeypadMultiply:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MULTIPLY;						return jkey;
		case kVK_ANSI_KeypadPlus:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ADD;							return jkey;
		//case XK_KP_Separator:
		//	jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SEPARATOR;					return jkey;	//No Apple Support
		case kVK_ANSI_KeypadMinus:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SUBTRACT;						return jkey;
		case kVK_ANSI_KeypadDecimal:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DECIMAL;						return jkey;
		case kVK_ANSI_KeypadDivide:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DIVIDE;						return jkey;
		case kVK_ANSI_KeypadClear:
			jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CLEAR;						return jkey;
		//case XK_Num_Lock:
		//	jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUM_LOCK;						return jkey;	//No Apple Support
		//case XK_Scroll_Lock:
		//	jkey.location = org_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD;			jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_SCROLL_LOCK;					return jkey;	//No Apple Support

		case kVK_F1:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F1;							return jkey;
		case kVK_F2:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F2;							return jkey;
		case kVK_F3:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F3;							return jkey;
		case kVK_F4:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F4;							return jkey;
		case kVK_F5:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F5;							return jkey;
		case kVK_F6:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F6;							return jkey;
		case kVK_F7:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F7;							return jkey;
		case kVK_F8:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F8;							return jkey;
		case kVK_F9:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F9;							return jkey;
		case kVK_F10:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F10;							return jkey;
		case kVK_F11:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F11;							return jkey;
		case kVK_F12:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F12;							return jkey;

		case kVK_F13:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F13;							return jkey;
		case kVK_F14:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F14;							return jkey;
		case kVK_F15:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F15;							return jkey;
		case kVK_F16:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F16;							return jkey;
		case kVK_F17:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F17;							return jkey;
		case kVK_F18:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F18;							return jkey;
		case kVK_F19:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F19;							return jkey;
		case kVK_F20:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F20;							return jkey;
		//case XK_F21:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F21;							return jkey;	//No Apple Support
		//case XK_F22:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F22;							return jkey;	//No Apple Support
		//case XK_F23:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F23;							return jkey;	//No Apple Support
		//case XK_F24:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_F24;							return jkey;	//No Apple Support

		//case XK_Print:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PRINTSCREEN;					return jkey;	//No Apple Support
		//case XK_Insert:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INSERT;						return jkey;	//No Apple Support
		case kVK_Help:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HELP;							return jkey;
		case kVK_ForwardDelete:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DELETE;						return jkey;

		case kVK_PageUp:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_UP;						return jkey;
		case kVK_PageDown:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PAGE_DOWN;					return jkey;
		case kVK_Home:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HOME;							return jkey;
		case kVK_End:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_END;							return jkey;

		case kVK_ANSI_Quote:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_QUOTE;						return jkey;
		case kVK_ANSI_Grave:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BACK_QUOTE;					return jkey;

		/* For European keyboards */
		/*
		case XK_dead_grave:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_GRAVE;					return jkey;	//Unknown Apple Support
		case XK_dead_acute:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ACUTE;					return jkey;	//Unknown Apple Support
		case XK_dead_circumflex:						jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CIRCUMFLEX;				return jkey;	//Unknown Apple Support
		case XK_dead_tilde:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_TILDE;					return jkey;	//Unknown Apple Support
		case XK_dead_macron:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_MACRON;					return jkey;	//Unknown Apple Support
		case XK_dead_breve:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_BREVE;					return jkey;	//Unknown Apple Support
		case XK_dead_abovedot:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ABOVEDOT;				return jkey;	//Unknown Apple Support
		case XK_dead_diaeresis:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_DIAERESIS;				return jkey;	//Unknown Apple Support
		case XK_dead_abovering:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_ABOVERING;				return jkey;	//Unknown Apple Support
		case XK_dead_doubleacute:						jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_DOUBLEACUTE;				return jkey;	//Unknown Apple Support
		case XK_dead_caron:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CARON;					return jkey;	//Unknown Apple Support
		case XK_dead_cedilla:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_CEDILLA;					return jkey;	//Unknown Apple Support
		case XK_dead_ogonek:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_OGONEK;					return jkey;	//Unknown Apple Support
		case XK_dead_iota:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_IOTA;					return jkey;	//Unknown Apple Support
		case XK_dead_voiced_sound:						jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_VOICED_SOUND;			return jkey;	//Unknown Apple Support
		case XK_dead_semivoiced_sound:					jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DEAD_SEMIVOICED_SOUND;		return jkey;	//Unknown Apple Support
		*/

		/* Unknown Keyboard Codes */
		/*
		case XK_ampersand:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AMPERSAND;					return jkey;	//No Apple Support
		case XK_asterisk:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ASTERISK;						return jkey;	//No Apple Support
		case XK_quotedbl:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_QUOTEDBL;						return jkey;	//No Apple Support
		case XK_less:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LESS;							return jkey;	//No Apple Support
		case XK_greater:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_GREATER;						return jkey;	//No Apple Support
		case XK_braceleft:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BRACELEFT;					return jkey;	//No Apple Support
		case XK_braceright:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BRACERIGHT;					return jkey;	//No Apple Support
		*/

		/* Unknown Extended Keyboard Codes */
		/*
		case XK_at:										jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AT;							return jkey;	//Unknown Apple Support
		case XK_colon:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COLON;						return jkey;	//Unknown Apple Support
		case XK_asciicircum:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CIRCUMFLEX;					return jkey;	//Unknown Apple Support
		case XK_dollar:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_DOLLAR;						return jkey;	//Unknown Apple Support
		case XK_EuroSign:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EURO_SIGN;					return jkey;	//Unknown Apple Support
		case XK_exclam:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_EXCLAMATION_MARK;				return jkey;	//Unknown Apple Support
		case XK_exclamdown:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INVERTED_EXCLAMATION_MARK;	return jkey;	//Unknown Apple Support
		case XK_parenleft:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_LEFT_PARENTHESIS;				return jkey;	//Unknown Apple Support
		case XK_numbersign:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NUMBER_SIGN;					return jkey;	//Unknown Apple Support
		case XK_plus:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PLUS;							return jkey;	//Unknown Apple Support
		case XK_parenright:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_RIGHT_PARENTHESIS;			return jkey;	//Unknown Apple Support
		case kVK_JIS_Underscore:						jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDERSCORE;					return jkey;
		*/

		/* For input method support on Asian Keyboards */
		//case XK_Cancel:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FINAL;						return jkey;	//Unknown Apple Support
		//case XK_Henkan:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CONVERT;						return jkey;	//Unknown Apple Support
		//case XK_Muhenkan:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_NONCONVERT;					return jkey;	//Unknown Apple Support
		//case XK_VoidSymbol:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ACCEPT;						return jkey;	//Unknown Apple Support
		//case XK_Mode_switch:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_MODECHANGE;					return jkey;	//Unknown Apple Support
		case kVK_JIS_Kana:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANA;							return jkey;	//TODO Testing Needed
		//case XK_Kanji:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANJI;						return jkey;	//Unknown Apple Support
		case kVK_JIS_Eisu:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALPHANUMERIC;					return jkey;	//TODO Testing Needed
		//case XK_Katakana:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KATAKANA;						return jkey;	//Unknown Apple Support
		//case XK_Hiragana:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HIRAGANA;						return jkey;	//Unknown Apple Support
		//case XK_Zenkaku:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FULL_WIDTH;					return jkey;	//Unknown Apple Support
		//case XK_Hankaku:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_HALF_WIDTH;					return jkey;	//Unknown Apple Support
		//case XK_Romaji:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ROMAN_CHARACTERS;				return jkey;	//Unknown Apple Support
		//case XK_Zen_Koho:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALL_CANDIDATES;				return jkey;	//Unknown Apple Support
		//case XK_Mae_Koho:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PREVIOUS_CANDIDATE;			return jkey;	//Unknown Apple Support
		//case XK_Kanji_Bangou:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CODE_INPUT;					return jkey;	//Unknown Apple Support
		//case XK_Hiragana_Katakana:					jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_KATAKANA;			return jkey;	//Unknown Apple Support
		//case XK_Hiragana_Katakana:					jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_HIRAGANA;			return jkey;	//Unknown Apple Support
		//case XK_Romaji:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_JAPANESE_ROMAN;				return jkey;	//Unknown Apple Support
		//case XK_Kana_Lock:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_KANA_LOCK;					return jkey;	//Unknown Apple Support
		//case XK_VoidSymbol:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_INPUT_METHOD_ON_OFF;			return jkey;	//Unknown Apple Support

		/* For Sun keyboards */
		/*
		#ifdef SUN_KEYBOARD
		case SunXK_Cut:									jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_CUT;							return jkey;	//No Apple Support
		case SunXK_Copy:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COPY;							return jkey;	//No Apple Support
		case SunXK_Paste:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PASTE;						return jkey;	//No Apple Support
		case SunXK_Undo:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDO;							return jkey;	//No Apple Support
		case SunXK_Again:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_AGAIN;						return jkey;	//No Apple Support
		case SunXK_Find:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_FIND;							return jkey;	//No Apple Support
		case SunXK_Props:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_PROPS;						return jkey;	//No Apple Support
		case SunXK_Stop:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_STOP;							return jkey;	//No Apple Support
		case SunXK_Compose:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_COMPOSE;						return jkey;	//No Apple Support
		case SunXK_AltGraph:							jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_ALT_GRAPH;					return jkey;	//No Apple Support
		#endif
		*/

		//case XK_Begin:								jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_BEGIN;						return jkey;	//No Apple Support

		default:
		case 0xFF:										jkey.keycode = org_jnativehook_keyboard_NativeKeyEvent_VK_UNDEFINED;					return jkey;
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
