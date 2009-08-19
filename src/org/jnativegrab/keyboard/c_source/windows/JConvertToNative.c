#include <Windows.h>
#include "include/JConvertToNative.h"

KeySym JKeycodeToNative(jint key, jint location) {
	switch (key) {
		case JK_ENTER:				return VK_RETURN;
		case JK_BACK_SPACE:			return VK_BACK;
		case JK_TAB:				return VK_TAB;
		case JK_SHIFT:				return VK_SHIFT;		//FIXME R and L
		case JK_CONTROL:			return VK_CONTROL;		//FIXME R and L
		case JK_ALT:				return VK_MENU;		//FIXME R and L
		case JK_PAUSE:				return VK_PAUSE;
		case JK_CAPS_LOCK:			return VK_CAPITAL;
		case JK_ESCAPE:				return VK_ESCAPE;
		case JK_SPACE:				return VK_SPACE;
		case JK_PAGE_UP:			return VK_PRIOR;
		case JK_PAGE_DOWN:			return VK_NEXT;
		case JK_HOME:				return VK_HOME;
		case JK_END:				return VK_END;

		case JK_LEFT:				return VK_LEFT;
		case JK_RIGHT:				return VK_RIGHT;
		case JK_UP:					return VK_UP;
		case JK_DOWN:				return VK_DOWN;

		case JK_COMMA:				return VK_OEM_COMMA;
		case JK_MINUS:				return VK_OEM_MINUS;
		case JK_PERIOD:				return VK_OEM_PERIOD;
		case JK_SLASH:				return VK_OEM_2;

//		TODO exists 0-9 outside of numpad?
		case JK_0:				return VK_NUMPAD0;
		case JK_1:				return VK_NUMPAD1;
		case JK_2:				return VK_NUMPAD2;
		case JK_3:				return VK_NUMPAD3;
		case JK_4:				return VK_NUMPAD4;
		case JK_5:				return VK_NUMPAD5;
		case JK_6:				return VK_NUMPAD6;
		case JK_7:				return VK_NUMPAD7;
		case JK_8:				return VK_NUMPAD8;
		case JK_9:				return VK_NUMPAD9;

		case JK_SEMICOLON:			return VK_OEM_1;
//		case JK_EQUALS:				return VK_equal; //TODO FIND EQUALS

//		TODO Verify these are correct
		case JK_A:				return VK_A;
		case JK_B:				return VK_B;
		case JK_C:				return VK_C;
		case JK_D:				return VK_D;
		case JK_E:				return VK_E;
		case JK_F:				return VK_F;
		case JK_G:				return VK_G;
		case JK_H:				return VK_H;
		case JK_I:				return VK_I;
		case JK_J:				return VK_J;
		case JK_L:				return VK_L;
		case JK_M:				return VK_M;
		case JK_N:				return VK_N;
		case JK_O:				return VK_O;
		case JK_P:				return VK_P;
		case JK_Q:				return VK_Q;
		case JK_R:				return VK_R;
		case JK_S:				return VK_S;
		case JK_T:				return VK_T;
		case JK_U:				return VK_U;
		case JK_V:				return VK_V;
		case JK_W:				return VK_W;
		case JK_X:				return VK_X;
		case JK_Y:				return VK_Y;
		case JK_Z:				return VK_Z;

//		TODO - Correct brackets? "{ ["
		case JK_OPEN_BRACKET:			return VK_OEM_4;
		case JK_BACK_SLASH:			return VK_OEM_5;
//		TODO - Correct close brackets? "} ]"
		case JK_CLOSE_BRACKET:			return VK_OEM_6;

		case JK_NUMPAD0:			return VK_NUMPAD0;
		case JK_NUMPAD1:			return VK_NUMPAD1;
		case JK_NUMPAD2:			return VK_NUMPAD2;
		case JK_NUMPAD3:			return VK_NUMPAD3;
		case JK_NUMPAD4:			return VK_NUMPAD4;
		case JK_NUMPAD5:			return VK_NUMPAD5;
		case JK_NUMPAD6:			return VK_NUMPAD6;
		case JK_NUMPAD7:			return VK_NUMPAD7;
		case JK_NUMPAD8:			return VK_NUMPAD8;
		case JK_NUMPAD9:			return VK_NUMPAD9;
		case JK_MULTIPLY:			return VK_MULTIPLY;
		case JK_ADD:				return VK_ADD;

		case JK_SEPARATOR:			return VK_SEPARATOR;

		case JK_SUBTRACT:			return VK_SUBTRACT;
		case JK_DECIMAL:			return VK_DECIMAL;
		case JK_DIVIDE:				return VK_DIVIDE;
		case JK_DELETE:				return VK_DELETE;
		case JK_NUM_LOCK:			return VK_NUMLOCK;
		case JK_SCROLL_LOCK:		return VK_SCROLL;

		case JK_F1:					return VK_F1;
		case JK_F2:					return VK_F2;
		case JK_F3:					return VK_F3;
		case JK_F4:					return VK_F4;
		case JK_F5:					return VK_F5;
		case JK_F6:					return VK_F6;
		case JK_F7:					return VK_F7;
		case JK_F8:					return VK_F8;
		case JK_F9:					return VK_F9;
		case JK_F10:				return VK_F10;
		case JK_F11:				return VK_F11;
		case JK_F12:				return VK_F12;
		case JK_F13:				return VK_F13;
		case JK_F14:				return VK_F14;
		case JK_F15:				return VK_F15;
		case JK_F16:				return VK_F16;
		case JK_F17:				return VK_F17;
		case JK_F18:				return VK_F18;
		case JK_F19:				return VK_F19;
		case JK_F20:				return VK_F20;
		case JK_F21:				return VK_F21;
		case JK_F22:				return VK_F22;
		case JK_F23:				return VK_F23;
		case JK_F24:				return VK_F24;

		case JK_PRINTSCREEN:			return VK_SNAPSHOT;
		case JK_INSERT:				return VK_INSERT;
		case JK_HELP:				return VK_HELP;
//		TODO - find meta key
//		case JK_META:				return VK_Meta_L;		//FIXME R and L

		case JK_QUOTE:				return VK_OEM_7;
		case JK_BACK_QUOTE:			return VK_OEM_3;

//		TODO - exists udlr on keypad specific?
		case JK_KP_UP:				return VK_UP;
		case JK_KP_DOWN:			return VK_DOWN;
		case JK_KP_LEFT:			return VK_LEFT;
		case JK_KP_RIGHT:			return VK_RIGHT;

//		TODO - find dead... ?
//		case JK_DEAD_GRAVE:			return VK_dead_grave;
//		case JK_DEAD_ACUTE:			return VK_dead_acute;
//		case JK_DEAD_CIRCUMFLEX:		return VK_dead_circumflex;
//		case JK_DEAD_TILDE:			return VK_dead_tilde;
//		case JK_DEAD_MACRON:			return VK_dead_macron;
//		case JK_DEAD_BREVE:			return VK_dead_breve;
//		case JK_DEAD_ABOVEDOT:			return VK_dead_abovedot;
//		case JK_DEAD_DIAERESIS:			return VK_dead_diaeresis;
//		case JK_DEAD_ABOVERING:			return VK_dead_abovering;
//		case JK_DEAD_DOUBLEACUTE:		return VK_dead_doubleacute;
//		case JK_DEAD_CARON:			return VK_dead_caron;
//		case JK_DEAD_CEDILLA:			return VK_dead_cedilla;
//		case JK_DEAD_OGONEK:			return VK_dead_ogonek;
//		case JK_DEAD_IOTA:			return VK_dead_iota;
//		case JK_DEAD_VOICED_SOUND:		return VK_dead_voiced_sound;
//		case JK_DEAD_SEMIVOICED_SOUND:		return VK_dead_semivoiced_sound;

		case JK_AMPERSAND:			return VK_ampersand;
		case JK_ASTERISK:			return VK_asterisk;
		case JK_QUOTEDBL:			return VK_quotedbl;
		case JK_LESS:				return VK_less;
		case JK_GREATER:			return VK_greater;
		case JK_BRACELEFT:			return VK_braceleft;
		case JK_BRACERIGHT:			return VK_braceright;

//		TODO - some of these are defined already within the OEM vk's; does msdn know when one is pressed
//		case JK_AT:				return VK_at;
		case JK_COLON:				return VK_OEM_1;
//		case JK_CIRCUMFLEX:			return VK_asciicircum;
//		case JK_DOLLAR:				return VK_dollar;
//		case JK_EURO_SIGN:			return VK_EuroSign;
//		case JK_EXCLAMATION_MARK:		return VK_exclam;
//		case JK_INVERTED_EXCLAMATION_MARK:	return VK_exclamdown;
//		case JK_LEFT_PARENTHESIS:		return VK_parenleft;
//		case JK_NUMBER_SIGN:			return VK_numbersign;
		case JK_PLUS:				return VK_OEM_PLUS;
//		case JK_RIGHT_PARENTHESIS:		return VK_parenright;
//		case JK_UNDERSCORE:			return VK_underscore;

		case JK_WINDOWS:			return VK_LWIN;		//FIXME R and L
//		TODO - is the = to the apps key?
		case JK_CONTEXT_MENU: 		return VK_APPS;
//		case JK_CONTEXT_MENU:			return VK_Menu;

		/* for input method support on Asian Keyboards */
//		TODO - find this support?
		case JK_FINAL:				return VK_CANCEL;		//Unknown Win32 API
//		case JK_CONVERT:			return VK_Henkan;
//		case JK_NONCONVERT:			return VK_Muhenkan;
//		case JK_ACCEPT:				return VK_VoidSymbol;		//Japanese keyboard: kakutei?
//		case JK_MODECHANGE:			return VK_Mode_switch;		//Unknown Win32 API
		case JK_KANA:				return VK_KANA;
		case JK_KANJI:				return VK_Kanji;
//		case JK_ALPHANUMERIC:			return VK_Eisu_Shift;
//		case JK_KATAKANA:			return VK_Katakana;
//		case JK_HIRAGANA:			return VK_Hiragana;
//		case JK_FULL_WIDTH:			return VK_Zenkaku;
//		case JK_HALF_WIDTH:			return VK_Hankaku;
//		case JK_ROMAN_CHARACTERS:		return VK_Romaji;
//		case JK_ALL_CANDIDATES:			return VK_Zen_Koho;
//		case JK_PREVIOUS_CANDIDATE:		return VK_Mae_Koho;
//		case JK_CODE_INPUT:			return VK_Kanji_Bangou;
//		case JK_JAPANESE_KATAKANA:		return VK_Hiragana_Katakana;	//Not Sure
//		case JK_JAPANESE_HIRAGANA:		return VK_Hiragana_Katakana;	//Not Sure
//		case JK_JAPANESE_ROMAN:			return VK_Romaji;		//Unknown
//		case JK_KANA_LOCK:			return VK_Kana_Lock;
//		case JK_INPUT_METHOD_ON_OFF:		return VK_VoidSymbol;		//Japanese keyboard: nihongo?

		/* for Sun keyboards */
//		TODO - find this support?
//		case JK_CUT:				return SunXK_Cut;
//		case JK_COPY:				return SunXK_Copy;
//		case JK_PASTE:				return SunXK_Paste;
//		case JK_UNDO:				return SunXK_Undo;
//		case JK_AGAIN:				return SunXK_Again;
//		case JK_FIND:				return SunXK_Find;
//		case JK_PROPS:				return SunXK_Props;
//		case JK_STOP:				return SunXK_Stop;
//
//		case JK_COMPOSE:			return SunXK_Compose;
//		case JK_ALT_GRAPH:			return SunXK_AltGraph;
//		case JK_BEGIN:				return XK_Begin;

		default:
		case JK_UNDEFINED:			return 0x507;		//Key Undefined
	}
}
