/*
#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <winuser.h>
*/
#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>
#include "include/JConvertToNative.h"

unsigned int JKeycodeToNative(JKeyCode jkey) {
	switch (jkey.keycode) {
		case JK_ENTER:									return VK_RETURN;
		case JK_BACK_SPACE:								return VK_BACK;
		case JK_TAB:									return VK_TAB;
		case JK_SHIFT:
			if (jkey.location == JK_LOCATION_LEFT)		return VK_LSHIFT;
			else										return VK_RSHIFT;
		case JK_CONTROL:
			if (jkey.location == JK_LOCATION_LEFT)		return VK_LCONTROL;
			else										return VK_RCONTROL;
		case JK_ALT:
			if (jkey.location == JK_LOCATION_LEFT)		return VK_LMENU;
			else										return VK_RMENU;
		case JK_PAUSE:									return VK_PAUSE;
		case JK_CAPS_LOCK:								return VK_CAPITAL;
		case JK_ESCAPE:									return VK_ESCAPE;
		case JK_SPACE:									return VK_SPACE;
		case JK_PAGE_UP:								return VK_PRIOR;
		case JK_PAGE_DOWN:								return VK_NEXT;
		case JK_HOME:									return VK_HOME;
		case JK_END:									return VK_END;

		case JK_LEFT:									return VK_LEFT;
		case JK_RIGHT:									return VK_RIGHT;
		case JK_UP:										return VK_UP;
		case JK_DOWN:									return VK_DOWN;

		case JK_COMMA:									return VK_OEM_COMMA;
		case JK_MINUS:									return VK_OEM_MINUS;
		case JK_PERIOD:									return VK_OEM_PERIOD;
		case JK_SLASH:									return VK_OEM_2;

		case JK_0:										return 0x30;
		case JK_1:										return 0x31;
		case JK_2:										return 0x32;
		case JK_3:										return 0x33;
		case JK_4:										return 0x34;
		case JK_5:										return 0x35;
		case JK_6:										return 0x36;
		case JK_7:										return 0x37;
		case JK_8:										return 0x38;
		case JK_9:										return 0x39;

		case JK_SEMICOLON:								return VK_OEM_1;
		//case JK_EQUALS:								return VK_equal; 	//TODO FIND EQUALS

		case JK_A:										return 0x41;
		case JK_B:										return 0x42;
		case JK_C:										return 0x43;
		case JK_D:										return 0x44;
		case JK_E:										return 0x45;
		case JK_F:										return 0x46;
		case JK_G:										return 0x47;
		case JK_H:										return 0x48;
		case JK_I:										return 0x49;
		case JK_J:										return 0x4A;
		case JK_K:										return 0x4B;
		case JK_L:										return 0x4C;
		case JK_M:										return 0x4D;
		case JK_N:										return 0x4E;
		case JK_O:										return 0x4F;
		case JK_P:										return 0x50;
		case JK_Q:										return 0x51;
		case JK_R:										return 0x52;
		case JK_S:										return 0x53;
		case JK_T:										return 0x54;
		case JK_U:										return 0x55;
		case JK_V:										return 0x56;
		case JK_W:										return 0x57;
		case JK_X:										return 0x58;
		case JK_Y:										return 0x59;
		case JK_Z:										return 0x5A;

		case JK_OPEN_BRACKET:							return VK_OEM_4;
		case JK_BACK_SLASH:								return VK_OEM_5;
		case JK_CLOSE_BRACKET:							return VK_OEM_6;

		case JK_NUMPAD0:								return VK_NUMPAD0;
		case JK_NUMPAD1:								return VK_NUMPAD1;
		case JK_NUMPAD2:								return VK_NUMPAD2;
		case JK_NUMPAD3:								return VK_NUMPAD3;
		case JK_NUMPAD4:								return VK_NUMPAD4;
		case JK_NUMPAD5:								return VK_NUMPAD5;
		case JK_NUMPAD6:								return VK_NUMPAD6;
		case JK_NUMPAD7:								return VK_NUMPAD7;
		case JK_NUMPAD8:								return VK_NUMPAD8;
		case JK_NUMPAD9:								return VK_NUMPAD9;
		case JK_MULTIPLY:								return VK_MULTIPLY;
		case JK_ADD:									return VK_ADD;

		case JK_SEPARATOR:								return VK_SEPARATOR;

		case JK_SUBTRACT:								return VK_SUBTRACT;
		case JK_DECIMAL:								return VK_DECIMAL;
		case JK_DIVIDE:									return VK_DIVIDE;
		case JK_DELETE:									return VK_DELETE;
		case JK_NUM_LOCK:								return VK_NUMLOCK;
		case JK_SCROLL_LOCK:							return VK_SCROLL;

		case JK_F1:										return VK_F1;
		case JK_F2:										return VK_F2;
		case JK_F3:										return VK_F3;
		case JK_F4:										return VK_F4;
		case JK_F5:										return VK_F5;
		case JK_F6:										return VK_F6;
		case JK_F7:										return VK_F7;
		case JK_F8:										return VK_F8;
		case JK_F9:										return VK_F9;
		case JK_F10:									return VK_F10;
		case JK_F11:									return VK_F11;
		case JK_F12:									return VK_F12;
		case JK_F13:									return VK_F13;
		case JK_F14:									return VK_F14;
		case JK_F15:									return VK_F15;
		case JK_F16:									return VK_F16;
		case JK_F17:									return VK_F17;
		case JK_F18:									return VK_F18;
		case JK_F19:									return VK_F19;
		case JK_F20:									return VK_F20;
		case JK_F21:									return VK_F21;
		case JK_F22:									return VK_F22;
		case JK_F23:									return VK_F23;
		case JK_F24:									return VK_F24;

		case JK_PRINTSCREEN:							return VK_SNAPSHOT;
		case JK_INSERT:									return VK_INSERT;
		case JK_HELP:									return VK_HELP;
		case JK_META:
			if (jkey.location == JK_LOCATION_LEFT)		return VK_LWIN;
			else										return VK_RWIN;

		case JK_QUOTE:									return VK_OEM_7;
		case JK_BACK_QUOTE:								return VK_OEM_3;

		//Windows does not have keypad direction keys
		case JK_KP_UP:									return VK_UP;
		case JK_KP_DOWN:								return VK_DOWN;
		case JK_KP_LEFT:								return VK_LEFT;
		case JK_KP_RIGHT:								return VK_RIGHT;

		//No Windows Key Equivalent?
		//case JK_DEAD_GRAVE:							return VK_dead_grave;
		//case JK_DEAD_ACUTE:							return VK_dead_acute;
		//case JK_DEAD_CIRCUMFLEX:						return VK_dead_circumflex;
		//case JK_DEAD_TILDE:							return VK_dead_tilde;
		//case JK_DEAD_MACRON:							return VK_dead_macron;
		//case JK_DEAD_BREVE:							return VK_dead_breve;
		//case JK_DEAD_ABOVEDOT:						return VK_dead_abovedot;
		//case JK_DEAD_DIAERESIS:						return VK_dead_diaeresis;
		//case JK_DEAD_ABOVERING:						return VK_dead_abovering;
		//case JK_DEAD_DOUBLEACUTE:						return VK_dead_doubleacute;
		//case JK_DEAD_CARON:							return VK_dead_caron;
		//case JK_DEAD_CEDILLA:							return VK_dead_cedilla;
		//case JK_DEAD_OGONEK:							return VK_dead_ogonek;
		//case JK_DEAD_IOTA:							return VK_dead_iota;
		//case JK_DEAD_VOICED_SOUND:					return VK_dead_voiced_sound;
		//case JK_DEAD_SEMIVOICED_SOUND:				return VK_dead_semivoiced_sound;

		/*
		case JK_AMPERSAND:								return VK_ampersand;
		case JK_ASTERISK:								return VK_asterisk;
		case JK_QUOTEDBL:								return VK_quotedbl;
		case JK_LESS:									return VK_less;
		case JK_GREATER:								return VK_greater;
		case JK_BRACELEFT:								return VK_braceleft;
		case JK_BRACERIGHT:								return VK_braceright;
		 */

		//Some of these are defined already within the OEM vk's based on keyboard layout
		//case JK_AT:									return VK_at;
		case JK_COLON:									return VK_OEM_1;
		//case JK_CIRCUMFLEX:							return VK_asciicircum;
		//case JK_DOLLAR:								return VK_dollar;
		//case JK_EURO_SIGN:							return VK_EuroSign;
		//case JK_EXCLAMATION_MARK:						return VK_exclam;
		//case JK_INVERTED_EXCLAMATION_MARK:			return VK_exclamdown;
		//case JK_LEFT_PARENTHESIS:						return VK_parenleft;
		//case JK_NUMBER_SIGN:							return VK_numbersign;
		case JK_PLUS:									return VK_OEM_PLUS;
		//case JK_RIGHT_PARENTHESIS:					return VK_parenright;
		//case JK_UNDERSCORE:							return VK_underscore;

		case JK_WINDOWS:
			if (jkey.location == JK_LOCATION_LEFT)		return VK_LWIN;
			else										return VK_RWIN;
		case JK_CONTEXT_MENU: 							return VK_APPS;									//is the = to the apps key?
		//case JK_CONTEXT_MENU:							return VK_Menu;

		/* for input method support on Asian Keyboards */
		case JK_FINAL:									return VK_CANCEL;								//Unknown Win32 API
		//case JK_CONVERT:								return VK_Henkan;
		//case JK_NONCONVERT:							return VK_Muhenkan;
		//case JK_ACCEPT:								return VK_VoidSymbol;							//Japanese keyboard: kakutei?
		//case JK_MODECHANGE:							return VK_Mode_switch;							//Unknown Win32 API
		case JK_KANA:									return VK_KANA;
		//case JK_KANJI:									return VK_Kanji;
		//case JK_ALPHANUMERIC:							return VK_Eisu_Shift;
		//case JK_KATAKANA:								return VK_Katakana;
		//case JK_HIRAGANA:								return VK_Hiragana;
		//case JK_FULL_WIDTH:							return VK_Zenkaku;
		//case JK_HALF_WIDTH:							return VK_Hankaku;
		//case JK_ROMAN_CHARACTERS:						return VK_Romaji;
		//case JK_ALL_CANDIDATES:						return VK_Zen_Koho;
		//case JK_PREVIOUS_CANDIDATE:					return VK_Mae_Koho;
		//case JK_CODE_INPUT:							return VK_Kanji_Bangou;
		//case JK_JAPANESE_KATAKANA:					return VK_Hiragana_Katakana;					//Not Sure
		//case JK_JAPANESE_HIRAGANA:					return VK_Hiragana_Katakana;					//Not Sure
		//case JK_JAPANESE_ROMAN:						return VK_Romaji;								//Unknown
		//case JK_KANA_LOCK:							return VK_Kana_Lock;
		//case JK_INPUT_METHOD_ON_OFF:					return VK_VoidSymbol;							//Japanese keyboard: nihongo?

		/* for Sun keyboards */
		//case JK_CUT:									return SunXK_Cut;
		//case JK_COPY:									return SunXK_Copy;
		//case JK_PASTE:								return SunXK_Paste;
		//case JK_UNDO:									return SunXK_Undo;
		//case JK_AGAIN:								return SunXK_Again;
		//case JK_FIND:									return SunXK_Find;
		//case JK_PROPS:								return SunXK_Props;
		//case JK_STOP:									return SunXK_Stop;
		//case JK_COMPOSE:								return SunXK_Compose;
		//case JK_ALT_GRAPH:							return SunXK_AltGraph;
		//case JK_BEGIN:								return XK_Begin;

		default:
		case JK_UNDEFINED:								return 0x07;									//Key Undefined
	}
}

JKeyCode NativeToJKeyCode ( unsigned int keycode ){
	JKeyCode jkey;
	jkey.location = JK_LOCATION_STANDARD;

	switch (keycode) {
		case VK_RETURN:									jkey.keycode = JK_ENTER; 						return jkey;
		case VK_BACK:									jkey.keycode = JK_BACK_SPACE; 					return jkey;
		case VK_TAB:									jkey.keycode = JK_TAB; 							return jkey;
		case VK_LSHIFT:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_SHIFT; 						return jkey;
		case VK_RSHIFT:
			jkey.location = JK_LOCATION_RIGHT; 			jkey.keycode = JK_SHIFT; 						return jkey;
		case VK_LCONTROL:
			jkey.location = JK_LOCATION_LEFT; 			jkey.keycode = JK_CONTROL;						return jkey;
		case VK_RCONTROL:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_CONTROL;						return jkey;
		case VK_LMENU:
			jkey.location = JK_LOCATION_LEFT; 			jkey.keycode = JK_ALT;							return jkey;
		case VK_RMENU:
			jkey.location = JK_LOCATION_RIGHT; 			jkey.keycode = JK_ALT; 							return jkey;
		case VK_PAUSE:									jkey.keycode = JK_PAUSE; 						return jkey;
		case VK_CAPITAL:								jkey.keycode = JK_CAPS_LOCK; 					return jkey;
		case VK_ESCAPE:									jkey.keycode = JK_ESCAPE; 						return jkey;
		case VK_SPACE:									jkey.keycode = JK_SPACE; 						return jkey;
		case VK_PRIOR:									jkey.keycode = JK_PAGE_UP;						return jkey;
		case VK_NEXT:									jkey.keycode = JK_PAGE_DOWN; 					return jkey;
		case VK_HOME:									jkey.keycode = JK_HOME; 						return jkey;
		case VK_END:									jkey.keycode = JK_END; 							return jkey;

		case VK_LEFT:									jkey.keycode = JK_LEFT; 						return jkey;
		case VK_RIGHT:									jkey.keycode = JK_RIGHT; 						return jkey;
		case VK_UP:										jkey.keycode = JK_UP; 							return jkey;
		case VK_DOWN:									jkey.keycode = JK_DOWN; 						return jkey;

		case VK_OEM_COMMA:								jkey.keycode = JK_COMMA; 						return jkey;
		case VK_OEM_MINUS:								jkey.keycode = JK_MINUS; 						return jkey;
		case VK_OEM_PERIOD:								jkey.keycode = JK_PERIOD; 						return jkey;
		case VK_OEM_2:									jkey.keycode = JK_SLASH; 						return jkey;

		case 0x30:										jkey.keycode = JK_0; 							return jkey;
		case 0x31:										jkey.keycode = JK_1; 							return jkey;
		case 0x32:										jkey.keycode = JK_2; 							return jkey;
		case 0x33:										jkey.keycode = JK_3; 							return jkey;
		case 0x34:										jkey.keycode = JK_4; 							return jkey;
		case 0x35:										jkey.keycode = JK_5; 							return jkey;
		case 0x36:										jkey.keycode = JK_6; 							return jkey;
		case 0x37:										jkey.keycode = JK_7; 							return jkey;
		case 0x38:										jkey.keycode = JK_8; 							return jkey;
		case 0x39:										jkey.keycode = JK_9; 							return jkey;

		case VK_OEM_1:									jkey.keycode = JK_SEMICOLON; 					return jkey;

		case 0x41:										jkey.keycode = JK_A; 							return jkey;
		case 0x42:										jkey.keycode = JK_B; 							return jkey;
		case 0x43:										jkey.keycode = JK_C; 							return jkey;
		case 0x44:										jkey.keycode = JK_D; 							return jkey;
		case 0x45:										jkey.keycode = JK_E; 							return jkey;
		case 0x46:										jkey.keycode = JK_F; 							return jkey;
		case 0x47:										jkey.keycode = JK_G; 							return jkey;
		case 0x48:										jkey.keycode = JK_H; 							return jkey;
		case 0x49:										jkey.keycode = JK_I; 							return jkey;
		case 0x4A:										jkey.keycode = JK_J; 							return jkey;
		case 0x4B:										jkey.keycode = JK_K; 							return jkey;
		case 0x4C:										jkey.keycode = JK_L; 							return jkey;
		case 0x4D:										jkey.keycode = JK_M; 							return jkey;
		case 0x4E:										jkey.keycode = JK_N; 							return jkey;
		case 0x4F:										jkey.keycode = JK_O; 							return jkey;
		case 0x50:										jkey.keycode = JK_P; 							return jkey;
		case 0x51:										jkey.keycode = JK_Q; 							return jkey;
		case 0x52:										jkey.keycode = JK_R; 							return jkey;
		case 0x53:										jkey.keycode = JK_S; 							return jkey;
		case 0x54:										jkey.keycode = JK_T; 							return jkey;
		case 0x55:										jkey.keycode = JK_U; 							return jkey;
		case 0x56:										jkey.keycode = JK_V; 							return jkey;
		case 0x57:										jkey.keycode = JK_W; 							return jkey;
		case 0x58:										jkey.keycode = JK_X; 							return jkey;
		case 0x59:										jkey.keycode = JK_Y; 							return jkey;
		case 0x5A:										jkey.keycode = JK_Z; 							return jkey;

		case VK_OEM_4:									jkey.keycode = JK_OPEN_BRACKET; 				return jkey;
		case VK_OEM_5:									jkey.keycode = JK_BACK_SLASH; 					return jkey;

		case VK_MULTIPLY:								jkey.keycode = JK_MULTIPLY; 					return jkey;
		case VK_ADD:									jkey.keycode = JK_ADD; 							return jkey;

		case VK_SEPARATOR:								jkey.keycode = JK_SEPARATOR; 					return jkey;

		case VK_SUBTRACT:								jkey.keycode = JK_SUBTRACT; 					return jkey;
		case VK_DECIMAL:								jkey.keycode = JK_DECIMAL; 						return jkey;
		case VK_DIVIDE:									jkey.keycode = JK_DIVIDE; 						return jkey;
		case VK_DELETE:									jkey.keycode = JK_DELETE; 						return jkey;
		case VK_NUMLOCK:								jkey.keycode = JK_NUM_LOCK; 					return jkey;
		case VK_SCROLL:									jkey.keycode = JK_SCROLL_LOCK; 					return jkey;

		case VK_F1:										jkey.keycode = JK_F1; 							return jkey;
		case VK_F2:										jkey.keycode = JK_F2; 							return jkey;
		case VK_F3:										jkey.keycode = JK_F3; 							return jkey;
		case VK_F4:										jkey.keycode = JK_F4; 							return jkey;
		case VK_F5:										jkey.keycode = JK_F5; 							return jkey;
		case VK_F6:										jkey.keycode = JK_F6; 							return jkey;
		case VK_F7:										jkey.keycode = JK_F7; 							return jkey;
		case VK_F8:										jkey.keycode = JK_F8; 							return jkey;
		case VK_F9:										jkey.keycode = JK_F9; 							return jkey;
		case VK_F10:									jkey.keycode = JK_F10; 							return jkey;
		case VK_F11:									jkey.keycode = JK_F11; 							return jkey;
		case VK_F12:									jkey.keycode = JK_F12; 							return jkey;
		case VK_F13:									jkey.keycode = JK_F13; 							return jkey;
		case VK_F14:									jkey.keycode = JK_F14; 							return jkey;
		case VK_F15:									jkey.keycode = JK_F15; 							return jkey;
		case VK_F16:									jkey.keycode = JK_F16; 							return jkey;
		case VK_F17:									jkey.keycode = JK_F17; 							return jkey;
		case VK_F18:									jkey.keycode = JK_F18; 							return jkey;
		case VK_F19:									jkey.keycode = JK_F19; 							return jkey;
		case VK_F20:									jkey.keycode = JK_F20; 							return jkey;
		case VK_F21:									jkey.keycode = JK_F21; 							return jkey;
		case VK_F22:									jkey.keycode = JK_F22; 							return jkey;
		case VK_F23:									jkey.keycode = JK_F23; 							return jkey;
		case VK_F24:									jkey.keycode = JK_F24; 							return jkey;

		case VK_SNAPSHOT:								jkey.keycode = JK_PRINTSCREEN; 					return jkey;
		case VK_INSERT:									jkey.keycode = JK_INSERT; 						return jkey;
		case VK_HELP:									jkey.keycode = JK_HELP; 						return jkey;

		case VK_OEM_7:									jkey.keycode = JK_QUOTE; 						return jkey;
		case VK_OEM_3:									jkey.keycode = JK_BACK_QUOTE; 					return jkey;

		/*
		case VK_ampersand:								jkey.keycode = JK_AMPERSAND; 					return jkey;
		case VK_asterisk:								jkey.keycode = JK_ASTERISK; 					return jkey;
		case VK_quotedbl:								jkey.keycode = JK_QUOTEDBL; 					return jkey;
		case VK_less:									jkey.keycode = JK_LESS; 						return jkey;
		case VK_greater:								jkey.keycode = JK_GREATER; 						return jkey;
		case VK_braceleft:								jkey.keycode = JK_BRACELEFT; 					return jkey;
		case VK_braceright:								jkey.keycode = JK_BRACERIGHT; 					return jkey;
		 */

		//case VK_OEM_1:									jkey.keycode = JK_COLON; 						return jkey;
		case VK_OEM_PLUS:								jkey.keycode = JK_PLUS; 						return jkey;

		case VK_LWIN:
			jkey.location = JK_LOCATION_LEFT; 			jkey.keycode = JK_WINDOWS; 						return jkey;
		case VK_RWIN:
			jkey.location = JK_LOCATION_RIGHT; 			jkey.keycode = JK_WINDOWS;						return jkey;
		case VK_APPS:									jkey.keycode = JK_CONTEXT_MENU; 				return jkey;
		case VK_CANCEL:									jkey.keycode = JK_FINAL; 						return jkey;
		case VK_KANA:									jkey.keycode = JK_KANA; 						return jkey;
		//case VK_KANJI:									jkey.keycode = JK_KANJI;						return jkey;

		default:
			case 0x507:									jkey.keycode = JK_UNDEFINED; 					return jkey;
	}
}

jint NativeToJButton (unsigned int button) {
	switch (button) {
		case VK_LBUTTON:								return JBUTTON1;
		case VK_RBUTTON:								return JBUTTON2;
		case VK_MBUTTON:								return JBUTTON3;
		case VK_XBUTTON1:								return JBUTTON4;
		case VK_XBUTTON2:								return JBUTTON5;
		default:
		case 0x07:										return JNOBUTTON;
	}
}

unsigned int JButtonToNative(jint button) {
	switch (button)	{
		case JBUTTON1:									return VK_LBUTTON;
		case JBUTTON2:									return VK_RBUTTON;
		case JBUTTON3:									return VK_MBUTTON;
		case JBUTTON4:									return VK_XBUTTON1;
		case JBUTTON5:									return VK_XBUTTON2;
		default:										return 0x07;
	}
}

unsigned int JModifierToNative(jint mod) {
	switch (mod) {
		case JK_SHIFT_MASK:								return MOD_SHIFT;
		case JK_CTRL_MASK:								return MOD_CONTROL;
		case JK_META_MASK:								return MOD_WIN;
		case JK_ALT_MASK:								return MOD_ALT;
		default: 										return 0x07;		//Undefined
	}
}

jint NativeToJModifier(unsigned int mod) {
	switch (mod) {
		case MOD_SHIFT:									return JK_SHIFT_MASK;
		case MOD_CONTROL:								return JK_CTRL_MASK;
		case MOD_WIN:									return JK_META_MASK;
		case MOD_ALT:									return JK_ALT_MASK;
		default:										return JK_UNDEFINED;
	}
}
