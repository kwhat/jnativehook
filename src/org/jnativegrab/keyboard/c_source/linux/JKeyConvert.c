#include "JKeyConvert.h"

KeySym JKeycodeToXKeysym(KeyCode key) {
	switch (key) {
		case JK_ENTER:				return XK_Return;
		case JK_BACK_SPACE:			return XK_BackSpace;
		case JK_TAB:				return XK_Tab;
		case JK_SHIFT:				return XK_Shift_L;		//FIXME R and L
		case JK_CONTROL:			return XK_Control_L;		//FIXME R and L
		case JK_ALT:				return XK_Alt_L;		//FIXME R and L
		case JK_PAUSE:				return XK_Pause;
		case JK_CAPS_LOCK:			return XK_Caps_Lock;
		case JK_ESCAPE:				return XK_Escape;
		case JK_SPACE:				return XK_space;
		case JK_PAGE_UP:			return XK_Page_Up;
		case JK_PAGE_DOWN:			return XK_Page_Down;
		case JK_HOME:				return XK_Home;
		case JK_END:				return XK_End;

		case JK_LEFT:				return XK_Left;
		case JK_RIGHT:				return XK_Right;
		case JK_UP:				return XK_Up;
		case JK_DOWN:				return XK_Down;

		case JK_COMMA:				return XK_comma;
		case JK_MINUS:				return XK_minus;
		case JK_PERIOD:				return XK_period;
		case JK_SLASH:				return XK_slash;

		case JK_0:				return XK_0;
		case JK_1:				return XK_1;
		case JK_2:				return XK_2;
		case JK_3:				return XK_3;
		case JK_4:				return XK_4;
		case JK_5:				return XK_5;
		case JK_6:				return XK_6;
		case JK_7:				return XK_7;
		case JK_8:				return XK_8;
		case JK_9:				return XK_9;

		case JK_SEMICOLON:			return XK_semicolon;
		case JK_EQUALS:				return XK_equal;

		case JK_A:				return XK_A;
		case JK_B:				return XK_B;
		case JK_C:				return XK_C;
		case JK_D:				return XK_D;
		case JK_E:				return XK_E;
		case JK_F:				return XK_F;
		case JK_G:				return XK_G;
		case JK_H:				return XK_H;
		case JK_I:				return XK_I;
		case JK_J:				return XK_J;
		case JK_L:				return XK_L;
		case JK_M:				return XK_M;
		case JK_N:				return XK_N;
		case JK_O:				return XK_O;
		case JK_P:				return XK_P;
		case JK_Q:				return XK_Q;
		case JK_R:				return XK_R;
		case JK_S:				return XK_S;
		case JK_T:				return XK_T;
		case JK_U:				return XK_U;
		case JK_V:				return XK_V;
		case JK_W:				return XK_W;
		case JK_X:				return XK_X;
		case JK_Y:				return XK_Y;
		case JK_Z:				return XK_Z;

		case JK_OPEN_BRACKET:			return XK_bracketleft;
		case JK_BACK_SLASH:			return XK_backslash;
		case JK_CLOSE_BRACKET:			return XK_bracketright;

		case JK_NUMPAD0:			return XK_KP_0;
		case JK_NUMPAD1:			return XK_KP_1;
		case JK_NUMPAD2:			return XK_KP_2;
		case JK_NUMPAD3:			return XK_KP_3;
		case JK_NUMPAD4:			return XK_KP_4;
		case JK_NUMPAD5:			return XK_KP_5;
		case JK_NUMPAD6:			return XK_KP_6;
		case JK_NUMPAD7:			return XK_KP_7;
		case JK_NUMPAD8:			return XK_KP_8;
		case JK_NUMPAD9:			return XK_KP_9;
		case JK_MULTIPLY:			return XK_KP_Multiply;
		case JK_ADD:				return XK_KP_Add;

		case JK_SEPARATOR:			return XK_KP_Separator;

		case JK_SUBTRACT:			return XK_KP_Subtract;
		case JK_DECIMAL:			return XK_KP_Decimal;
		case JK_DIVIDE:				return XK_KP_Divide;
		case JK_DELETE:				return XK_KP_Delete;
		case JK_NUM_LOCK:			return XK_Num_Lock;
		case JK_SCROLL_LOCK:			return XK_Scroll_Lock;

		case JK_F1:				return XK_F1;
		case JK_F2:				return XK_F2;
		case JK_F3:				return XK_F3;
		case JK_F4:				return XK_F4;
		case JK_F5:				return XK_F5;
		case JK_F6:				return XK_F6;
		case JK_F7:				return XK_F7;
		case JK_F8:				return XK_F8;
		case JK_F9:				return XK_F9;
		case JK_F10:				return XK_F10;
		case JK_F11:				return XK_F11;
		case JK_F12:				return XK_F12;
		case JK_F13:				return XK_F13;
		case JK_F14:				return XK_F14;
		case JK_F15:				return XK_F15;
		case JK_F16:				return XK_F16;
		case JK_F17:				return XK_F17;
		case JK_F18:				return XK_F18;
		case JK_F19:				return XK_F19;
		case JK_F20:				return XK_F20;
		case JK_F21:				return XK_F21;
		case JK_F22:				return XK_F22;
		case JK_F23:				return XK_F23;
		case JK_F24:				return XK_F24;

		case JK_PRINTSCREEN:			return XK_Print;
		case JK_INSERT:				return XK_Insert;
		case JK_HELP:				return XK_Help;
		case JK_META:				return XK_Meta_L;		//FIXME R and L

		case JK_QUOTE:				return XK_quoteright;
		case JK_BACK_QUOTE:			return XK_asciitilde;

		case JK_KP_UP:				return XK_KP_Up;
		case JK_KP_DOWN:			return XK_KP_Down;
		case JK_KP_LEFT:			return XK_KP_Left;
		case JK_KP_RIGHT:			return XK_KP_Right;

		case JK_DEAD_GRAVE:			return XK_dead_grave;
		case JK_DEAD_ACUTE:			return XK_dead_acute;
		case JK_DEAD_CIRCUMFLEX			return XK_dead_circumflex;
		case JK_DEAD_TILDE			return XK_dead_tilde;
		case JK_DEAD_MACRON			return XK_dead_macron;
		case JK_DEAD_BREVE			return XK_dead_breve;
		case JK_DEAD_ABOVEDOT			return XK_dead_abovedot;
		case JK_DEAD_DIAERESIS			return XK_dead_diaeresis;
		case JK_DEAD_ABOVERING			return XK_dead_abovering;
		case JK_DEAD_DOUBLEACUTE		return XK_dead_doubleacute;
		case JK_DEAD_CARON			return XK_dead_caron;
		case JK_DEAD_CEDILLA			return XK_dead_cedilla;
		case JK_DEAD_OGONEK			return XK_dead_ogonek;
		case JK_DEAD_IOTA			return XK_dead_iota;
		case JK_DEAD_VOICED_SOUND		return XK_dead_voiced_sound;
		case JK_DEAD_SEMIVOICED_SOUND		return XK_dead_semivoiced_sound;

		case JK_AMPERSAND			return XK_ampersand;
		case JK_ASTERISK			return XK_asterisk;
		case JK_QUOTEDBL			return XK_quotedbl;
		case JK_LESS				return XK_less;
		case JK_GREATER				return XK_greater;
		case JK_BRACELEFT			return XK_braceleft;
		case JK_BRACERIGHT			return XK_braceright;

		case JK_AT				return XK_at;
		case JK_COLON				return XK_colon;
		case JK_CIRCUMFLEX			return XK_asciicircum;
		case JK_DOLLAR				return XK_dollar;
		case JK_EURO_SIGN			return XK_EuroSign;
		case JK_EXCLAMATION_MARK		return XK_exclam;
		case JK_INVERTED_EXCLAMATION_MARK	return XK_exclamdown;
		case JK_LEFT_PARENTHESIS		return XK_parenleft;
		case JK_NUMBER_SIGN			return XK_numbersign;
		case JK_PLUS				return XK_plus;
		case JK_RIGHT_PARENTHESIS		return XK_parenright;
		case JK_UNDERSCORE			return XK_underscore;

		case JK_WINDOWS				return XK_Super_L;		//FIXME R and L
		case JK_CONTEXT_MENU			return XK_Menu;

		/* for input method support on Asian Keyboards */
		case JK_FINAL				return XK_Cancel;		//Unknown Win32 API
		case JK_CONVERT				return XK_Henkan;
		case JK_NONCONVERT			return XK_Muhenkan;
		case JK_ACCEPT				return XK_VoidSymbol;		//Japanese keyboard: kakutei?
		case JK_MODECHANGE			return XK_Mode_switch;		//Unknown Win32 API
		case JK_KANA				return XK_Kana_Lock;
		case JK_KANJI				return XK_Kanji;
		case JK_ALPHANUMERIC			return XK_Eisu_Shift;
		case JK_KATAKANA			return XK_Katakana;
		case JK_HIRAGANA			return XK_Hiragana;
		case JK_FULL_WIDTH			return XK_Zenkaku;
		case JK_HALF_WIDTH			return XK_Hankaku;
		case JK_ROMAN_CHARACTERS		return XK_Romaji;
		case JK_ALL_CANDIDATES			return XK_Zen_Koho;
		case JK_PREVIOUS_CANDIDATE		return XK_Mae_Koho;
		case JK_CODE_INPUT			return XK_Kanji_Bangou;
		case JK_JAPANESE_KATAKANA		return XK_Hiragana_Katakana;	//Not Sure
		case JK_JAPANESE_HIRAGANA		return XK_Hiragana_Katakana;	//Not Sure
		case JK_JAPANESE_ROMAN			return XK_Romaji;		//Unknown
		case JK_KANA_LOCK			return XK_Kana_Lock;
		case JK_INPUT_METHOD_ON_OFF		return XK_VoidSymbol;		//Japanese keyboard: nihongo?

		/* for Sun keyboards */
		case JK_CUT				return SunXK_Cut;
		case JK_COPY				return SunXK_Copy;
		case JK_PASTE				return SunXK_Paste;
		case JK_UNDO				return SunXK_Undo;
		case JK_AGAIN				return SunXK_Again;
		case JK_FIND				return SunXK_Find;
		case JK_PROPS				return SunXK_Props;
		case JK_STOP				return SunXK_Stop;

		case JK_COMPOSE				return SunXK_Compose;
		case JK_ALT_GRAPH			return SunXK_AltGraph;
		case JK_BEGIN				return XK_Begin;

		default:
		case JK_UNDEFINED			return XK_VoidSymbol;		//Key Undefined
	}
}

int main(int argc, const char* argv[]) {

//XKeysymToKeycode(display, keysym)
//	convertJkXk(JK_A);

	return 0;
}

