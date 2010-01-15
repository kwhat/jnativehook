#include "include/JConvertToNative.h"
#include "OSXKeyCodes.h"
#include "OSXButtonCodes.h"

unsigned int JKeycodeToNative(JKeyCode jkey) {
	switch (jkey.keycode) {
		case JK_ENTER:									return XK_Return;
		case JK_BACK_SPACE:								return XK_BackSpace;
		case JK_TAB:									return XK_Tab;
		case JK_SHIFT:
			if (jkey.location == JK_LOCATION_LEFT)		return XK_Shift_L;
			else										return XK_Shift_R;
		case JK_CONTROL:
			if (jkey.location == JK_LOCATION_LEFT)		return XK_Control_L;
			else										return XK_Control_R;
		case JK_ALT:
			if (jkey.location == JK_LOCATION_LEFT)		return XK_Alt_L;
			else										return XK_Alt_R;
		case JK_PAUSE:									return XK_Pause;
		case JK_CAPS_LOCK:								return XK_Caps_Lock;
		case JK_ESCAPE:									return XK_Escape;
		case JK_SPACE:									return XK_space;
		case JK_PAGE_UP:								return XK_Page_Up;
		case JK_PAGE_DOWN:								return XK_Page_Down;
		case JK_HOME:									return XK_Home;
		case JK_END:									return XK_End;

		case JK_LEFT:									return XK_Left;
		case JK_RIGHT:									return XK_Right;
		case JK_UP:										return XK_Up;
		case JK_DOWN:									return XK_Down;

		case JK_COMMA:									return XK_comma;
		case JK_MINUS:									return XK_minus;
		case JK_PERIOD:									return XK_period;
		case JK_SLASH:									return XK_slash;

		case JK_0:										return XK_0;
		case JK_1:										return XK_1;
		case JK_2:										return XK_2;
		case JK_3:										return XK_3;
		case JK_4:										return XK_4;
		case JK_5:										return XK_5;
		case JK_6:										return XK_6;
		case JK_7:										return XK_7;
		case JK_8:										return XK_8;
		case JK_9:										return XK_9;

		case JK_SEMICOLON:								return XK_semicolon;
		case JK_EQUALS:									return XK_equal;

		case JK_A:										return XK_A;
		case JK_B:										return XK_B;
		case JK_C:										return XK_C;
		case JK_D:										return XK_D;
		case JK_E:										return XK_E;
		case JK_F:										return XK_F;
		case JK_G:										return XK_G;
		case JK_H:										return XK_H;
		case JK_I:										return XK_I;
		case JK_J:										return XK_J;
		case JK_L:										return XK_L;
		case JK_M:										return XK_M;
		case JK_N:										return XK_N;
		case JK_O:										return XK_O;
		case JK_P:										return XK_P;
		case JK_Q:										return XK_Q;
		case JK_R:										return XK_R;
		case JK_S:										return XK_S;
		case JK_T:										return XK_T;
		case JK_U:										return XK_U;
		case JK_V:										return XK_V;
		case JK_W:										return XK_W;
		case JK_X:										return XK_X;
		case JK_Y:										return XK_Y;
		case JK_Z:										return XK_Z;

		case JK_OPEN_BRACKET:							return XK_bracketleft;
		case JK_BACK_SLASH:								return XK_backslash;
		case JK_CLOSE_BRACKET:							return XK_bracketright;

		case JK_NUMPAD0:								return XK_KP_0;
		case JK_NUMPAD1:								return XK_KP_1;
		case JK_NUMPAD2:								return XK_KP_2;
		case JK_NUMPAD3:								return XK_KP_3;
		case JK_NUMPAD4:								return XK_KP_4;
		case JK_NUMPAD5:								return XK_KP_5;
		case JK_NUMPAD6:								return XK_KP_6;
		case JK_NUMPAD7:								return XK_KP_7;
		case JK_NUMPAD8:								return XK_KP_8;
		case JK_NUMPAD9:								return XK_KP_9;
		case JK_MULTIPLY:								return XK_KP_Multiply;
		case JK_ADD:									return XK_KP_Add;

		case JK_SEPARATOR:								return XK_KP_Separator;

		case JK_SUBTRACT:								return XK_KP_Subtract;
		case JK_DECIMAL:								return XK_KP_Decimal;
		case JK_DIVIDE:									return XK_KP_Divide;
		case JK_DELETE:									return XK_KP_Delete;
		case JK_NUM_LOCK:								return XK_Num_Lock;
		case JK_SCROLL_LOCK:							return XK_Scroll_Lock;

		case JK_F1:										return XK_F1;
		case JK_F2:										return XK_F2;
		case JK_F3:										return XK_F3;
		case JK_F4:										return XK_F4;
		case JK_F5:										return XK_F5;
		case JK_F6:										return XK_F6;
		case JK_F7:										return XK_F7;
		case JK_F8:										return XK_F8;
		case JK_F9:										return XK_F9;
		case JK_F10:									return XK_F10;
		case JK_F11:									return XK_F11;
		case JK_F12:									return XK_F12;
		case JK_F13:									return XK_F13;
		case JK_F14:									return XK_F14;
		case JK_F15:									return XK_F15;
		case JK_F16:									return XK_F16;
		case JK_F17:									return XK_F17;
		case JK_F18:									return XK_F18;
		case JK_F19:									return XK_F19;
		case JK_F20:									return XK_F20;
		case JK_F21:									return XK_F21;
		case JK_F22:									return XK_F22;
		case JK_F23:									return XK_F23;
		case JK_F24:									return XK_F24;

		case JK_PRINTSCREEN:							return XK_Print;
		case JK_INSERT:									return XK_Insert;
		case JK_HELP:									return XK_Help;
		case JK_META:
			if (jkey.location == JK_LOCATION_LEFT)		return XK_Meta_L;
			else										return XK_Meta_R;

		case JK_QUOTE:									return XK_quoteright;
		case JK_BACK_QUOTE:								return XK_asciitilde;

		case JK_KP_UP:									return XK_KP_Up;
		case JK_KP_DOWN:								return XK_KP_Down;
		case JK_KP_LEFT:								return XK_KP_Left;
		case JK_KP_RIGHT:								return XK_KP_Right;

		case JK_DEAD_GRAVE:								return XK_dead_grave;
		case JK_DEAD_ACUTE:								return XK_dead_acute;
		case JK_DEAD_CIRCUMFLEX:						return XK_dead_circumflex;
		case JK_DEAD_TILDE:								return XK_dead_tilde;
		case JK_DEAD_MACRON:							return XK_dead_macron;
		case JK_DEAD_BREVE:								return XK_dead_breve;
		case JK_DEAD_ABOVEDOT:							return XK_dead_abovedot;
		case JK_DEAD_DIAERESIS:							return XK_dead_diaeresis;
		case JK_DEAD_ABOVERING:							return XK_dead_abovering;
		case JK_DEAD_DOUBLEACUTE:						return XK_dead_doubleacute;
		case JK_DEAD_CARON:								return XK_dead_caron;
		case JK_DEAD_CEDILLA:							return XK_dead_cedilla;
		case JK_DEAD_OGONEK:							return XK_dead_ogonek;
		case JK_DEAD_IOTA:								return XK_dead_iota;
		case JK_DEAD_VOICED_SOUND:						return XK_dead_voiced_sound;
		case JK_DEAD_SEMIVOICED_SOUND:					return XK_dead_semivoiced_sound;

		case JK_AMPERSAND:								return XK_ampersand;
		case JK_ASTERISK:								return XK_asterisk;
		case JK_QUOTEDBL:								return XK_quotedbl;
		case JK_LESS:									return XK_less;
		case JK_GREATER:								return XK_greater;
		case JK_BRACELEFT:								return XK_braceleft;
		case JK_BRACERIGHT:								return XK_braceright;

		case JK_AT:										return XK_at;
		case JK_COLON:									return XK_colon;
		case JK_CIRCUMFLEX:								return XK_asciicircum;
		case JK_DOLLAR:									return XK_dollar;
		case JK_EURO_SIGN:								return XK_EuroSign;
		case JK_EXCLAMATION_MARK:						return XK_exclam;
		case JK_INVERTED_EXCLAMATION_MARK:				return XK_exclamdown;
		case JK_LEFT_PARENTHESIS:						return XK_parenleft;
		case JK_NUMBER_SIGN:							return XK_numbersign;
		case JK_PLUS:									return XK_plus;
		case JK_RIGHT_PARENTHESIS:						return XK_parenright;
		case JK_UNDERSCORE:								return XK_underscore;

		case JK_WINDOWS:
			if (jkey.location == JK_LOCATION_LEFT)		return XK_Super_L;
			else										return XK_Super_R;
		case JK_CONTEXT_MENU:							return XK_Menu;

		/* for input method support on Asian Keyboards */
		case JK_FINAL:									return XK_Cancel;								//Unknown Win32 API
		case JK_CONVERT:								return XK_Henkan;
		case JK_NONCONVERT:								return XK_Muhenkan;
		case JK_ACCEPT:									return XK_VoidSymbol;							//Japanese keyboard: kakutei?
		case JK_MODECHANGE:								return XK_Mode_switch;							//Unknown Win32 API
		case JK_KANA:									return XK_Kana_Lock;
		case JK_KANJI:									return XK_Kanji;
		case JK_ALPHANUMERIC:							return XK_Eisu_Shift;
		case JK_KATAKANA:								return XK_Katakana;
		case JK_HIRAGANA:								return XK_Hiragana;
		case JK_FULL_WIDTH:								return XK_Zenkaku;
		case JK_HALF_WIDTH:								return XK_Hankaku;
		case JK_ROMAN_CHARACTERS:						return XK_Romaji;
		case JK_ALL_CANDIDATES:							return XK_Zen_Koho;
		case JK_PREVIOUS_CANDIDATE:						return XK_Mae_Koho;
		case JK_CODE_INPUT:								return XK_Kanji_Bangou;
		case JK_JAPANESE_KATAKANA:						return XK_Hiragana_Katakana;					//Not Sure
		case JK_JAPANESE_HIRAGANA:						return XK_Hiragana_Katakana;					//Not Sure
		case JK_JAPANESE_ROMAN:							return XK_Romaji;								//Unknown
		case JK_KANA_LOCK:								return XK_Kana_Lock;
		case JK_INPUT_METHOD_ON_OFF:					return XK_VoidSymbol;							//Japanese keyboard: nihongo?

		/* for Sun keyboards */
		case JK_CUT:									return SunXK_Cut;
		case JK_COPY:									return SunXK_Copy;
		case JK_PASTE:									return SunXK_Paste;
		case JK_UNDO:									return SunXK_Undo;
		case JK_AGAIN:									return SunXK_Again;
		case JK_FIND:									return SunXK_Find;
		case JK_PROPS:									return SunXK_Props;
		case JK_STOP:									return SunXK_Stop;

		case JK_COMPOSE:								return SunXK_Compose;
		case JK_ALT_GRAPH:								return SunXK_AltGraph;
		case JK_BEGIN:									return XK_Begin;

		default:
		case JK_UNDEFINED:								return XK_VoidSymbol;							//Key Undefined
	}
}

JKeyCode NativeToJKeycode(unsigned int keycode) {
	JKeyCode jkey;
	jkey.location = JK_LOCATION_STANDARD;

	KeySym lower_keysym, upper_keysym;
	XConvertCase(keycode, &lower_keysym, &upper_keysym);

	switch ((unsigned int) upper_keysym) {
		case XK_Return:									jkey.keycode = JK_ENTER;						return jkey;
		case XK_BackSpace:								jkey.keycode = JK_BACK_SPACE;					return jkey;
		case XK_Tab:									jkey.keycode = JK_TAB;							return jkey;
		case XK_Shift_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_SHIFT;						return jkey;
		case XK_Shift_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_SHIFT;						return jkey;
		case XK_Control_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_CONTROL;						return jkey;
		case XK_Control_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_CONTROL;						return jkey;
		case XK_Alt_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_ALT;							return jkey;
		case XK_Alt_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_ALT;							return jkey;
		case XK_Pause:									jkey.keycode = JK_PAUSE;						return jkey;
		case XK_Caps_Lock:								jkey.keycode = JK_CAPS_LOCK;					return jkey;
		case XK_Escape:									jkey.keycode = JK_ESCAPE;						return jkey;
		case XK_space:									jkey.keycode = JK_SPACE;						return jkey;
		case XK_Page_Up:								jkey.keycode = JK_PAGE_UP;						return jkey;
		case XK_Page_Down:								jkey.keycode = JK_PAGE_DOWN;					return jkey;
		case XK_Home:									jkey.keycode = JK_HOME;							return jkey;
		case XK_End:									jkey.keycode = JK_END;							return jkey;

		case XK_Left:									jkey.keycode = JK_LEFT;							return jkey;
		case XK_Right:									jkey.keycode = JK_RIGHT;						return jkey;
		case XK_Up:										jkey.keycode = JK_UP;							return jkey;
		case XK_Down:									jkey.keycode = JK_DOWN;							return jkey;

		case XK_comma:									jkey.keycode = JK_COMMA;						return jkey;
		case XK_minus:									jkey.keycode = JK_MINUS;						return jkey;
		case XK_period:									jkey.keycode = JK_PERIOD;						return jkey;
		case XK_slash:									jkey.keycode = JK_SLASH;						return jkey;

		case XK_0:										jkey.keycode = JK_0;							return jkey;
		case XK_1:										jkey.keycode = JK_1;							return jkey;
		case XK_2:										jkey.keycode = JK_2;							return jkey;
		case XK_3:										jkey.keycode = JK_3;							return jkey;
		case XK_4:										jkey.keycode = JK_4;							return jkey;
		case XK_5:										jkey.keycode = JK_5;							return jkey;
		case XK_6:										jkey.keycode = JK_6;							return jkey;
		case XK_7:										jkey.keycode = JK_7;							return jkey;
		case XK_8:										jkey.keycode = JK_8;							return jkey;
		case XK_9:										jkey.keycode = JK_9;							return jkey;

		case XK_semicolon:								jkey.keycode = JK_SEMICOLON;					return jkey;
		case XK_equal:									jkey.keycode = JK_EQUALS;						return jkey;

		case XK_A:										jkey.keycode = JK_A;							return jkey;
		case XK_B:										jkey.keycode = JK_B;							return jkey;
		case XK_C:										jkey.keycode = JK_C;							return jkey;
		case XK_D:										jkey.keycode = JK_D;							return jkey;
		case XK_E:										jkey.keycode = JK_E;							return jkey;
		case XK_F:										jkey.keycode = JK_F;							return jkey;
		case XK_G:										jkey.keycode = JK_G;							return jkey;
		case XK_H:										jkey.keycode = JK_H;							return jkey;
		case XK_I:										jkey.keycode = JK_I;							return jkey;
		case XK_J:										jkey.keycode = JK_J;							return jkey;
		case XK_K:										jkey.keycode = JK_K;							return jkey;
		case XK_L:										jkey.keycode = JK_L;							return jkey;
		case XK_M:										jkey.keycode = JK_M;							return jkey;
		case XK_N:										jkey.keycode = JK_N;							return jkey;
		case XK_O:										jkey.keycode = JK_O;							return jkey;
		case XK_P:										jkey.keycode = JK_P;							return jkey;
		case XK_Q:										jkey.keycode = JK_Q;							return jkey;
		case XK_R:										jkey.keycode = JK_R;							return jkey;
		case XK_S:										jkey.keycode = JK_S;							return jkey;
		case XK_T:										jkey.keycode = JK_T;							return jkey;
		case XK_U:										jkey.keycode = JK_U;							return jkey;
		case XK_V:										jkey.keycode = JK_V;							return jkey;
		case XK_W:										jkey.keycode = JK_W;							return jkey;
		case XK_X:										jkey.keycode = JK_X;							return jkey;
		case XK_Y:										jkey.keycode = JK_Y;							return jkey;
		case XK_Z:										jkey.keycode = JK_Z;							return jkey;

		case XK_bracketleft:							jkey.keycode = JK_OPEN_BRACKET;					return jkey;
		case XK_backslash:								jkey.keycode = JK_BACK_SLASH;					return jkey;
		case XK_bracketright:							jkey.keycode = JK_CLOSE_BRACKET;				return jkey;

		case XK_KP_0:									jkey.keycode = JK_NUMPAD0;						return jkey;
		case XK_KP_1:									jkey.keycode = JK_NUMPAD1;						return jkey;
		case XK_KP_2:									jkey.keycode = JK_NUMPAD2;						return jkey;
		case XK_KP_3:									jkey.keycode = JK_NUMPAD3;						return jkey;
		case XK_KP_4:									jkey.keycode = JK_NUMPAD4;						return jkey;
		case XK_KP_5:									jkey.keycode = JK_NUMPAD5;						return jkey;
		case XK_KP_6:									jkey.keycode = JK_NUMPAD6;						return jkey;
		case XK_KP_7:									jkey.keycode = JK_NUMPAD7;						return jkey;
		case XK_KP_8:									jkey.keycode = JK_NUMPAD8;						return jkey;
		case XK_KP_9:									jkey.keycode = JK_NUMPAD9;						return jkey;
		case XK_KP_Multiply:							jkey.keycode = JK_MULTIPLY;						return jkey;
		case XK_KP_Add:									jkey.keycode = JK_ADD;							return jkey;

		case XK_KP_Separator:							jkey.keycode = JK_SEPARATOR;					return jkey;

		case XK_KP_Subtract:							jkey.keycode = JK_SUBTRACT;						return jkey;
		case XK_KP_Decimal:								jkey.keycode = JK_DECIMAL;						return jkey;
		case XK_KP_Divide:								jkey.keycode = JK_DIVIDE;						return jkey;
		case XK_KP_Delete:								jkey.keycode = JK_DELETE;						return jkey;
		case XK_Num_Lock:								jkey.keycode = JK_NUM_LOCK;						return jkey;
		case XK_Scroll_Lock:							jkey.keycode = JK_SCROLL_LOCK;					return jkey;

		case XK_F1:										jkey.keycode = JK_F1;							return jkey;
		case XK_F2:										jkey.keycode = JK_F2;							return jkey;
		case XK_F3:										jkey.keycode = JK_F3;							return jkey;
		case XK_F4:										jkey.keycode = JK_F4;							return jkey;
		case XK_F5:										jkey.keycode = JK_F5;							return jkey;
		case XK_F6:										jkey.keycode = JK_F6;							return jkey;
		case XK_F7:										jkey.keycode = JK_F7;							return jkey;
		case XK_F8:										jkey.keycode = JK_F8;							return jkey;
		case XK_F9:										jkey.keycode = JK_F9;							return jkey;
		case XK_F10:									jkey.keycode = JK_F10;							return jkey;
		case XK_F11:									jkey.keycode = JK_F11;							return jkey;
		case XK_F12:									jkey.keycode = JK_F12;							return jkey;
		case XK_F13:									jkey.keycode = JK_F13;							return jkey;
		case XK_F14:									jkey.keycode = JK_F14;							return jkey;
		case XK_F15:									jkey.keycode = JK_F15;							return jkey;
		case XK_F16:									jkey.keycode = JK_F16;							return jkey;
		case XK_F17:									jkey.keycode = JK_F17;							return jkey;
		case XK_F18:									jkey.keycode = JK_F18;							return jkey;
		case XK_F19:									jkey.keycode = JK_F19;							return jkey;
		case XK_F20:									jkey.keycode = JK_F20;							return jkey;
		case XK_F21:									jkey.keycode = JK_F21;							return jkey;
		case XK_F22:									jkey.keycode = JK_F22;							return jkey;
		case XK_F23:									jkey.keycode = JK_F23;							return jkey;
		case XK_F24:									jkey.keycode = JK_F24;							return jkey;

		case XK_Print:									jkey.keycode = JK_PRINTSCREEN;					return jkey;
		case XK_Insert:									jkey.keycode = JK_INSERT;						return jkey;
		case XK_Help:									jkey.keycode = JK_HELP;							return jkey;
		case XK_Meta_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_META;							return jkey;
		case XK_Meta_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_META;							return jkey;
		case XK_quoteright:								jkey.keycode = JK_QUOTE;						return jkey;
		case XK_asciitilde:								jkey.keycode = JK_BACK_QUOTE;					return jkey;

		case XK_KP_Up:									jkey.keycode = JK_KP_UP;						return jkey;
		case XK_KP_Down:								jkey.keycode = JK_KP_DOWN;						return jkey;
		case XK_KP_Left:								jkey.keycode = JK_KP_LEFT;						return jkey;
		case XK_KP_Right:								jkey.keycode = JK_KP_RIGHT;						return jkey;

		case XK_dead_grave:								jkey.keycode = JK_DEAD_GRAVE;					return jkey;
		case XK_dead_acute:								jkey.keycode = JK_DEAD_ACUTE;					return jkey;
		case XK_dead_circumflex:						jkey.keycode = JK_DEAD_CIRCUMFLEX;				return jkey;
		case XK_dead_tilde:								jkey.keycode = JK_DEAD_TILDE;					return jkey;
		case XK_dead_macron:							jkey.keycode = JK_DEAD_MACRON;					return jkey;
		case XK_dead_breve:								jkey.keycode = JK_DEAD_BREVE;					return jkey;
		case XK_dead_abovedot:							jkey.keycode = JK_DEAD_ABOVEDOT;				return jkey;
		case XK_dead_diaeresis:							jkey.keycode = JK_DEAD_DIAERESIS;				return jkey;
		case XK_dead_abovering:							jkey.keycode = JK_DEAD_ABOVERING;				return jkey;
		case XK_dead_doubleacute:						jkey.keycode = JK_DEAD_DOUBLEACUTE;				return jkey;
		case XK_dead_caron:								jkey.keycode = JK_DEAD_CARON;					return jkey;
		case XK_dead_cedilla:							jkey.keycode = JK_DEAD_CEDILLA;					return jkey;
		case XK_dead_ogonek:							jkey.keycode = JK_DEAD_OGONEK;					return jkey;
		case XK_dead_iota:								jkey.keycode = JK_DEAD_IOTA;					return jkey;
		case XK_dead_voiced_sound:						jkey.keycode = JK_DEAD_VOICED_SOUND;			return jkey;
		case XK_dead_semivoiced_sound:					jkey.keycode = JK_DEAD_SEMIVOICED_SOUND;		return jkey;

		case XK_ampersand:								jkey.keycode = JK_AMPERSAND;					return jkey;
		case XK_asterisk:								jkey.keycode = JK_ASTERISK;						return jkey;
		case XK_quotedbl:								jkey.keycode = JK_QUOTEDBL;						return jkey;
		case XK_less:									jkey.keycode = JK_LESS;							return jkey;
		case XK_greater:								jkey.keycode = JK_GREATER;						return jkey;
		case XK_braceleft:								jkey.keycode = JK_BRACELEFT;					return jkey;
		case XK_braceright:								jkey.keycode = JK_BRACERIGHT;					return jkey;

		case XK_at:										jkey.keycode = JK_AT;							return jkey;
		case XK_colon:									jkey.keycode = JK_COLON;						return jkey;
		case XK_asciicircum:							jkey.keycode = JK_CIRCUMFLEX;					return jkey;
		case XK_dollar:									jkey.keycode = JK_DOLLAR;						return jkey;
		case XK_EuroSign:								jkey.keycode = JK_EURO_SIGN;					return jkey;
		case XK_exclam:									jkey.keycode = JK_EXCLAMATION_MARK;				return jkey;
		case XK_exclamdown:								jkey.keycode = JK_INVERTED_EXCLAMATION_MARK;	return jkey;
		case XK_parenleft:								jkey.keycode = JK_LEFT_PARENTHESIS;				return jkey;
		case XK_numbersign:								jkey.keycode = JK_NUMBER_SIGN;					return jkey;
		case XK_plus:									jkey.keycode = JK_PLUS;							return jkey;
		case XK_parenright:								jkey.keycode = JK_RIGHT_PARENTHESIS;			return jkey;
		case XK_underscore:								jkey.keycode = JK_UNDERSCORE;					return jkey;

		case XK_Super_L:
			jkey.location = JK_LOCATION_LEFT;			jkey.keycode = JK_WINDOWS;						return jkey;
		case XK_Super_R:
			jkey.location = JK_LOCATION_RIGHT;			jkey.keycode = JK_WINDOWS;						return jkey;
		case XK_Menu:									jkey.keycode = JK_CONTEXT_MENU;					return jkey;

		/* for input method support on Asian Keyboards */
		case XK_Cancel:									jkey.keycode = JK_FINAL;						return jkey;
		case XK_Henkan:									jkey.keycode = JK_CONVERT;						return jkey;
		case XK_Muhenkan:								jkey.keycode = JK_NONCONVERT;					return jkey;
		//case XK_VoidSymbol:							jkey.keycode = JK_ACCEPT;						return jkey;
		case XK_Mode_switch:							jkey.keycode = JK_MODECHANGE;					return jkey;
		//case XK_Kana_Lock:							jkey.keycode = JK_KANA;							return jkey;
		case XK_Kanji:									jkey.keycode = JK_KANJI;						return jkey;
		case XK_Eisu_Shift:								jkey.keycode = JK_ALPHANUMERIC;					return jkey;
		case XK_Katakana:								jkey.keycode = JK_KATAKANA;						return jkey;
		case XK_Hiragana:								jkey.keycode = JK_HIRAGANA;						return jkey;
		case XK_Zenkaku:								jkey.keycode = JK_FULL_WIDTH;					return jkey;
		case XK_Hankaku:								jkey.keycode = JK_HALF_WIDTH;					return jkey;
		//case XK_Romaji:								jkey.keycode = JK_ROMAN_CHARACTERS;				return jkey;
		case XK_Zen_Koho:								jkey.keycode = JK_ALL_CANDIDATES;				return jkey;
		case XK_Mae_Koho:								jkey.keycode = JK_PREVIOUS_CANDIDATE;			return jkey;
		case XK_Kanji_Bangou:							jkey.keycode = JK_CODE_INPUT;					return jkey;
		case XK_Hiragana_Katakana:						jkey.keycode = JK_JAPANESE_KATAKANA;			return jkey;
		//case XK_Hiragana_Katakana:					jkey.keycode = JK_JAPANESE_HIRAGANA;			return jkey;
		case XK_Romaji:									jkey.keycode = JK_JAPANESE_ROMAN;				return jkey;
		case XK_Kana_Lock:								jkey.keycode = JK_KANA_LOCK;					return jkey;
		//case XK_VoidSymbol:							jkey.keycode = JK_INPUT_METHOD_ON_OFF;			return jkey;

		/* for Sun keyboards */
		case SunXK_Cut:									jkey.keycode = JK_CUT;							return jkey;
		case SunXK_Copy:								jkey.keycode = JK_COPY;							return jkey;
		case SunXK_Paste:								jkey.keycode = JK_PASTE;						return jkey;
		case SunXK_Undo:								jkey.keycode = JK_UNDO;							return jkey;
		case SunXK_Again:								jkey.keycode = JK_AGAIN;						return jkey;
		case SunXK_Find:								jkey.keycode = JK_FIND;							return jkey;
		case SunXK_Props:								jkey.keycode = JK_PROPS;						return jkey;
		//case SunXK_Stop:								jkey.keycode = JK_STOP;							return jkey;

		case SunXK_Compose:								jkey.keycode = JK_COMPOSE;						return jkey;
		//case SunXK_AltGraph:							jkey.keycode = JK_ALT_GRAPH;					return jkey;
		case XK_Begin:									jkey.keycode = JK_BEGIN;						return jkey;

		default:
		case XK_VoidSymbol:								jkey.keycode = JK_UNDEFINED;					return jkey;
	}
}

jint NativeToJButton(unsigned int button) {
	switch (button) {
		case Button1:									return JBUTTON1;
		case Button2:									return JBUTTON2;
		case Button3:									return JBUTTON3;
		case Button4:									return JBUTTON4;
		case Button5:									return JBUTTON5;
		default:
		case AnyButton:									return JNOBUTTON;
	}
}

unsigned int JButtonToNative(jint button) {
	switch (button) {
		case JBUTTON1:									return Button1;
		case JBUTTON2:									return Button2;
		case JBUTTON3:									return Button3;
		case JBUTTON4:									return Button4;
		case JBUTTON5:									return Button5;
		default:
		case JNOBUTTON:									return AnyButton;
	}
}

unsigned int JModifierToNative(jint modifier) {
	switch (modifier) {
		case JK_SHIFT_MASK:								return ShiftMask;
		case JK_CTRL_MASK:								return ControlMask;
		case JK_META_MASK:								return getMetaMask();
		case JK_ALT_MASK:								return getAltMask();
		default: 										return AnyModifier;
	}
}

jint NativeToJModifier(unsigned int modifier) {
	switch (modifier) {
		case ShiftMask:									return JK_SHIFT_MASK;
		case ControlMask:								return JK_CTRL_MASK;
		default:
			if (modifier == getMetaMask())				return JK_META_MASK;
			else if (modifier == getAltMask())			return JK_ALT_MASK;
			return 0;
	}
}
