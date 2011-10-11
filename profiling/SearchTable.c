/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/keysym.h>
#include "JKeyCodes.h"
#include "JKeyLocations.h"

typedef struct {
	int rawcode;		//Raw Code
	int keycode;		//Key Code
	int location;		//Key Location
} JKeyDatum;

JKeyDatum keycode_table[] = {
	{XK_Return,					JK_ENTER,						JK_LOCATION_STANDARD},
	{XK_BackSpace,				JK_BACK_SPACE,					JK_LOCATION_STANDARD},
	{XK_Tab,					JK_TAB,							JK_LOCATION_STANDARD},
	#ifndef SUN_KEYBOARD
	{XK_Cancel,					JK_CANCEL,						JK_LOCATION_STANDARD},
	#endif

	{XK_Shift_L,				JK_SHIFT, 						JK_LOCATION_LEFT},
	{XK_Shift_R,				JK_SHIFT,						JK_LOCATION_RIGHT},
	{XK_Control_L,				JK_CONTROL,						JK_LOCATION_LEFT},
	{XK_Control_R,				JK_CONTROL,						JK_LOCATION_RIGHT},
	{XK_Alt_L,					JK_ALT, 						JK_LOCATION_LEFT},
	{XK_Alt_R,					JK_ALT,							JK_LOCATION_RIGHT},
	{XK_Meta_L,					JK_META,						JK_LOCATION_LEFT},
	{XK_Meta_R,					JK_META,						JK_LOCATION_RIGHT},
	{XK_Super_L,				JK_WINDOWS,						JK_LOCATION_LEFT},
	{XK_Super_R,				JK_WINDOWS,						JK_LOCATION_RIGHT},
	{XK_Menu,					JK_CONTEXT_MENU,				JK_LOCATION_STANDARD},

	{XK_Pause,					JK_PAUSE,						JK_LOCATION_STANDARD},
	{XK_Caps_Lock,				JK_CAPS_LOCK,					JK_LOCATION_STANDARD},
	{XK_Escape,					JK_ESCAPE,						JK_LOCATION_STANDARD},
	{XK_space,					JK_SPACE,						JK_LOCATION_STANDARD},

	{XK_Up,						JK_UP,							JK_LOCATION_STANDARD},
	{XK_Down,					JK_DOWN,						JK_LOCATION_STANDARD},
	{XK_Left,					JK_LEFT,						JK_LOCATION_STANDARD},
	{XK_Right,					JK_RIGHT,						JK_LOCATION_STANDARD},

	{XK_comma,					JK_COMMA,						JK_LOCATION_STANDARD},
	{XK_minus,					JK_MINUS,						JK_LOCATION_STANDARD},
	{XK_period,					JK_PERIOD,						JK_LOCATION_STANDARD},
	{XK_slash,					JK_SLASH,						JK_LOCATION_STANDARD},

	{XK_0,						JK_0,							JK_LOCATION_STANDARD},
	{XK_1,						JK_1,							JK_LOCATION_STANDARD},
	{XK_2,						JK_2,							JK_LOCATION_STANDARD},
	{XK_3,						JK_3,							JK_LOCATION_STANDARD},
	{XK_4,						JK_4,							JK_LOCATION_STANDARD},
	{XK_5,						JK_5,							JK_LOCATION_STANDARD},
	{XK_6,						JK_6,							JK_LOCATION_STANDARD},
	{XK_7,						JK_7,							JK_LOCATION_STANDARD},
	{XK_8,						JK_8,							JK_LOCATION_STANDARD},
	{XK_9,						JK_9,							JK_LOCATION_STANDARD},


	{XK_semicolon,				JK_SEMICOLON,					JK_LOCATION_STANDARD},
	{XK_equal,					JK_EQUALS,						JK_LOCATION_STANDARD},

	{XK_A,						JK_A,							JK_LOCATION_STANDARD},
	{XK_B,						JK_B,							JK_LOCATION_STANDARD},
	{XK_C,						JK_C,							JK_LOCATION_STANDARD},
	{XK_D,						JK_D,							JK_LOCATION_STANDARD},
	{XK_E,						JK_E,							JK_LOCATION_STANDARD},
	{XK_F,						JK_F,							JK_LOCATION_STANDARD},
	{XK_G,						JK_G,							JK_LOCATION_STANDARD},
	{XK_H,						JK_H,							JK_LOCATION_STANDARD},
	{XK_I,						JK_I,							JK_LOCATION_STANDARD},
	{XK_J,						JK_J,							JK_LOCATION_STANDARD},
	{XK_K,						JK_K,							JK_LOCATION_STANDARD},
	{XK_L,						JK_L,							JK_LOCATION_STANDARD},
	{XK_M,						JK_M,							JK_LOCATION_STANDARD},
	{XK_N,						JK_N,							JK_LOCATION_STANDARD},
	{XK_O,						JK_O,							JK_LOCATION_STANDARD},
	{XK_P,						JK_P,							JK_LOCATION_STANDARD},
	{XK_Q,						JK_Q,							JK_LOCATION_STANDARD},
	{XK_R,						JK_R,							JK_LOCATION_STANDARD},
	{XK_S,						JK_S,							JK_LOCATION_STANDARD},
	{XK_T,						JK_T,							JK_LOCATION_STANDARD},
	{XK_U,						JK_U,							JK_LOCATION_STANDARD},
	{XK_V,						JK_V,							JK_LOCATION_STANDARD},
	{XK_W,						JK_W,							JK_LOCATION_STANDARD},
	{XK_X,						JK_X,							JK_LOCATION_STANDARD},
	{XK_Y,						JK_Y,							JK_LOCATION_STANDARD},
	{XK_Z,						JK_Z,							JK_LOCATION_STANDARD},

	{XK_bracketleft,			JK_OPEN_BRACKET,				JK_LOCATION_STANDARD},
	{XK_backslash,				JK_BACK_SLASH,					JK_LOCATION_STANDARD},
	{XK_bracketright,			JK_CLOSE_BRACKET,				JK_LOCATION_STANDARD},

	{XK_KP_0,					JK_NUMPAD0,						JK_LOCATION_NUMPAD},
	{XK_KP_Insert,				JK_NUMPAD0,						JK_LOCATION_NUMPAD},
	{XK_KP_1,					JK_NUMPAD1,						JK_LOCATION_NUMPAD},
	{XK_KP_End,					JK_NUMPAD1,						JK_LOCATION_NUMPAD},
	{XK_KP_2,					JK_NUMPAD2,						JK_LOCATION_NUMPAD},
	{XK_KP_Down,				JK_NUMPAD2,						JK_LOCATION_NUMPAD},
	{XK_KP_3,					JK_NUMPAD3,						JK_LOCATION_NUMPAD},
	{XK_KP_Page_Down,			JK_NUMPAD3,						JK_LOCATION_NUMPAD},
	{XK_KP_4,					JK_NUMPAD4,						JK_LOCATION_NUMPAD},
	{XK_KP_Left,				JK_NUMPAD4,						JK_LOCATION_NUMPAD},
	{XK_KP_5,					JK_NUMPAD5,						JK_LOCATION_NUMPAD},
	{XK_KP_Begin,				JK_NUMPAD5,						JK_LOCATION_NUMPAD},
	{XK_KP_6,					JK_NUMPAD6,						JK_LOCATION_NUMPAD},
	{XK_KP_Right,				JK_NUMPAD6,						JK_LOCATION_NUMPAD},
	{XK_KP_7,					JK_NUMPAD7,						JK_LOCATION_NUMPAD},
	{XK_KP_Home,				JK_NUMPAD7,						JK_LOCATION_NUMPAD},
	{XK_KP_8,					JK_NUMPAD8,						JK_LOCATION_NUMPAD},
	{XK_KP_Up,					JK_NUMPAD8,						JK_LOCATION_NUMPAD},
	{XK_KP_9,					JK_NUMPAD9,						JK_LOCATION_NUMPAD},
	{XK_KP_Page_Up,				JK_NUMPAD9,						JK_LOCATION_NUMPAD},

	{XK_KP_Multiply,			JK_MULTIPLY,					JK_LOCATION_NUMPAD},
	{XK_KP_Add,					JK_ADD,							JK_LOCATION_NUMPAD},
	{XK_KP_Separator,			JK_SEPARATOR,					JK_LOCATION_NUMPAD},
	{XK_KP_Subtract,			JK_SUBTRACT,					JK_LOCATION_NUMPAD},
	{XK_KP_Enter,				JK_ENTER,						JK_LOCATION_NUMPAD},
	{XK_KP_Decimal,				JK_DECIMAL,						JK_LOCATION_NUMPAD},
	{XK_KP_Divide,				JK_DIVIDE,						JK_LOCATION_NUMPAD},
	{XK_KP_Delete,				JK_DELETE,						JK_LOCATION_NUMPAD},
	{XK_Num_Lock,				JK_NUM_LOCK,					JK_LOCATION_NUMPAD},
	{XK_Clear,					JK_CLEAR,						JK_LOCATION_NUMPAD},
	{XK_Scroll_Lock,			JK_SCROLL_LOCK,					JK_LOCATION_NUMPAD},


	{XK_F1,						JK_F1,							JK_LOCATION_STANDARD},
	{XK_F2,						JK_F2,							JK_LOCATION_STANDARD},
	{XK_F3,						JK_F3,							JK_LOCATION_STANDARD},
	{XK_F4,						JK_F4,							JK_LOCATION_STANDARD},
	{XK_F5,						JK_F5,							JK_LOCATION_STANDARD},
	{XK_F6,						JK_F6,							JK_LOCATION_STANDARD},
	{XK_F7,						JK_F7,							JK_LOCATION_STANDARD},
	{XK_F8,						JK_F8,							JK_LOCATION_STANDARD},
	{XK_F9,						JK_F9,							JK_LOCATION_STANDARD},
	{XK_F10,					JK_F10,							JK_LOCATION_STANDARD},
	{XK_F11,					JK_F11,							JK_LOCATION_STANDARD},
	{XK_F12,					JK_F12,							JK_LOCATION_STANDARD},

	{XK_F13,					JK_F13,							JK_LOCATION_STANDARD},
	{XK_F14,					JK_F14,							JK_LOCATION_STANDARD},
	{XK_F15,					JK_F15,							JK_LOCATION_STANDARD},
	{XK_F16,					JK_F16,							JK_LOCATION_STANDARD},
	{XK_F17,					JK_F17,							JK_LOCATION_STANDARD},
	{XK_F18,					JK_F18,							JK_LOCATION_STANDARD},
	{XK_F19,					JK_F19,							JK_LOCATION_STANDARD},
	{XK_F20,					JK_F20,							JK_LOCATION_STANDARD},
	{XK_F21,					JK_F21,							JK_LOCATION_STANDARD},
	{XK_F22,					JK_F22,							JK_LOCATION_STANDARD},
	{XK_F23,					JK_F23,							JK_LOCATION_STANDARD},
	{XK_F24,					JK_F24,							JK_LOCATION_STANDARD},

	{XK_Print,					JK_PRINTSCREEN,					JK_LOCATION_STANDARD},
	{XK_Insert,					JK_INSERT,						JK_LOCATION_STANDARD},
	{XK_Delete,					JK_DELETE,						JK_LOCATION_STANDARD},
	{XK_Help,					JK_HELP,						JK_LOCATION_STANDARD},

	{XK_Page_Up,				JK_PAGE_UP,						JK_LOCATION_STANDARD},
	{XK_Page_Down,				JK_PAGE_DOWN,					JK_LOCATION_STANDARD},
	{XK_Home,					JK_HOME,						JK_LOCATION_STANDARD},
	{XK_End,					JK_END,							JK_LOCATION_STANDARD},

	{XK_apostrophe,				JK_QUOTE,						JK_LOCATION_STANDARD},
	{XK_grave,					JK_BACK_QUOTE,					JK_LOCATION_STANDARD},

	/* For European keyboards */
	{XK_dead_grave,				JK_DEAD_GRAVE,					JK_LOCATION_STANDARD},
	{XK_dead_acute,				JK_DEAD_ACUTE,					JK_LOCATION_STANDARD},
	{XK_dead_circumflex,		JK_DEAD_CIRCUMFLEX,				JK_LOCATION_STANDARD},
	{XK_dead_tilde,				JK_DEAD_TILDE,					JK_LOCATION_STANDARD},
	{XK_dead_macron,			JK_DEAD_MACRON,					JK_LOCATION_STANDARD},
	{XK_dead_breve,				JK_DEAD_BREVE,					JK_LOCATION_STANDARD},
	{XK_dead_abovedot,			JK_DEAD_ABOVEDOT,				JK_LOCATION_STANDARD},
	{XK_dead_diaeresis,			JK_DEAD_DIAERESIS,				JK_LOCATION_STANDARD},
	{XK_dead_abovering,			JK_DEAD_ABOVERING,				JK_LOCATION_STANDARD},
	{XK_dead_doubleacute,		JK_DEAD_DOUBLEACUTE,			JK_LOCATION_STANDARD},
	{XK_dead_caron,				JK_DEAD_CARON,					JK_LOCATION_STANDARD},
	{XK_dead_cedilla,			JK_DEAD_CEDILLA,				JK_LOCATION_STANDARD},
	{XK_dead_ogonek,			JK_DEAD_OGONEK,					JK_LOCATION_STANDARD},
	{XK_dead_iota,				JK_DEAD_IOTA,					JK_LOCATION_STANDARD},
	{XK_dead_voiced_sound,		JK_DEAD_VOICED_SOUND,			JK_LOCATION_STANDARD},
	{XK_dead_semivoiced_sound,	JK_DEAD_SEMIVOICED_SOUND,		JK_LOCATION_STANDARD},

	/* Unknown Keyboard Codes */
	{XK_ampersand,				JK_AMPERSAND,					JK_LOCATION_STANDARD},
	{XK_asterisk,				JK_ASTERISK,					JK_LOCATION_STANDARD},
	{XK_quotedbl,				JK_QUOTEDBL,					JK_LOCATION_STANDARD},
	{XK_less,					JK_LESS,						JK_LOCATION_STANDARD},
	{XK_greater,				JK_GREATER,						JK_LOCATION_STANDARD},
	{XK_braceleft,				JK_BRACELEFT,					JK_LOCATION_STANDARD},
	{XK_braceright,				JK_BRACERIGHT,					JK_LOCATION_STANDARD},

	/* Unknown Extended Keyboard Codes */
	{XK_at,						JK_AT,							JK_LOCATION_STANDARD},
	{XK_colon,					JK_COLON,						JK_LOCATION_STANDARD},
	{XK_asciicircum,			JK_CIRCUMFLEX,					JK_LOCATION_STANDARD},
	{XK_dollar,					JK_DOLLAR,						JK_LOCATION_STANDARD},
	{XK_EuroSign,				JK_EURO_SIGN,					JK_LOCATION_STANDARD},
	{XK_exclam,					JK_EXCLAMATION_MARK,			JK_LOCATION_STANDARD},
	{XK_exclamdown,				JK_INVERTED_EXCLAMATION_MARK,	JK_LOCATION_STANDARD},
	{XK_parenleft,				JK_LEFT_PARENTHESIS,			JK_LOCATION_STANDARD},
	{XK_numbersign,				JK_NUMBER_SIGN,					JK_LOCATION_STANDARD},
	{XK_plus,					JK_PLUS,						JK_LOCATION_STANDARD},
	{XK_parenright,				JK_RIGHT_PARENTHESIS,			JK_LOCATION_STANDARD},
	{XK_underscore,				JK_UNDERSCORE,					JK_LOCATION_STANDARD},

	/* For input method support on Asian Keyboards */
	/*
	{XK_Cancel,					JK_FINAL,						JK_LOCATION_STANDARD},	//Unknown X11 Support
	{XK_Henkan,					JK_CONVERT,						JK_LOCATION_STANDARD},	//Unknown X11 Support
	{XK_Muhenkan,				JK_NONCONVERT,					JK_LOCATION_STANDARD},	//Unknown X11 Support
	{XK_VoidSymbol,				JK_ACCEPT,						JK_LOCATION_STANDARD},	//Unknown X11 Support
	*/

	#ifndef SUN_KEYBOARD
	{XK_Mode_switch,			JK_MODECHANGE,					JK_LOCATION_STANDARD},	//TODO Testing Needed
	#endif

	{XK_Kana_Shift,				JK_KANA,						JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Kanji,					JK_KANJI,						JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Eisu_Shift,				JK_ALPHANUMERIC,				JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Katakana,				JK_KATAKANA,					JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Hiragana,				JK_HIRAGANA,					JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Zenkaku,				JK_FULL_WIDTH,					JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Hankaku,				JK_HALF_WIDTH,					JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Romaji,					JK_ROMAN_CHARACTERS,			JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_MultipleCandidate,		JK_ALL_CANDIDATES,				JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_PreviousCandidate,		JK_PREVIOUS_CANDIDATE,			JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Codeinput,				JK_CODE_INPUT,					JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Hiragana_Katakana,		JK_JAPANESE_KATAKANA,			JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Zenkaku_Hankaku,		JK_JAPANESE_HIRAGANA,			JK_LOCATION_STANDARD},	//TODO Testing Needed
	//{XK_Romaji,				JK_JAPANESE_ROMAN,				JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Kana_Lock,				JK_KANA_LOCK,					JK_LOCATION_STANDARD},	//TODO Testing Needed
	{XK_Eisu_toggle,			JK_INPUT_METHOD_ON_OFF,			JK_LOCATION_STANDARD},	//TODO Testing Needed

	/* For Sun keyboards */
	#ifdef SUN_KEYBOARD
	{SunXK_Cut,					JK_CUT,							JK_LOCATION_STANDARD},
	{SunXK_Copy,				JK_COPY,						JK_LOCATION_STANDARD},
	{SunXK_Paste,				JK_PASTE,						JK_LOCATION_STANDARD},
	{SunXK_Undo,				JK_UNDO,						JK_LOCATION_STANDARD},
	{SunXK_Again,				JK_AGAIN,						JK_LOCATION_STANDARD},
	{SunXK_Find,				JK_FIND,						JK_LOCATION_STANDARD},
	{SunXK_Props,				JK_PROPS,						JK_LOCATION_STANDARD},
	{SunXK_Stop,				JK_STOP,						JK_LOCATION_STANDARD},
	{SunXK_Compose,				JK_COMPOSE,						JK_LOCATION_STANDARD},
	{SunXK_AltGraph,			JK_ALT_GRAPH,					JK_LOCATION_STANDARD},
	#endif

	{XK_Begin,					JK_BEGIN,						JK_LOCATION_STANDARD}
};

int compare(const void * x, const void * y) {
	return ( *(JKeyDatum *) x).rawcode - ( *(JKeyDatum *) y).rawcode;
}

JKeyDatum lookup(unsigned int keysym) {
	JKeyDatum jkey = {keysym, JK_UNDEFINED, JK_LOCATION_STANDARD};

	JKeyDatum * item = (JKeyDatum *) bsearch(&jkey, keycode_table, sizeof(keycode_table) / sizeof(*keycode_table), sizeof(JKeyDatum), compare);
	if (item != NULL) {
		jkey = *item;
	}

	return jkey;
}

int main(int argc, const char * argv[]) {
	clock_t clock_start, clock_end, clock_diff;

	clock_start = clock();
	qsort(keycode_table, sizeof(keycode_table) / sizeof(*keycode_table), sizeof(JKeyDatum), compare);
	clock_end = clock();

	clock_diff = clock_end - clock_start;
	printf("Initialized!  Time: %.2lf, Clocks: %.0lf\n", (double) clock_diff / CLOCKS_PER_SEC, (double) clock_diff);

	//Init random number generator with one.
	srand(1);

	clock_start = clock();
	long i;
	for (i = 0; i < 200000000; i++) {
		int r = rand() % (255 - 20); //i % (255 - 20);
		r += 20;

		lookup(r);
	}
	clock_end = clock();

	clock_diff = clock_end - clock_start;
	printf("Complete!  Time: %.2lf, Clocks: %.0lf\n", (double) clock_diff / CLOCKS_PER_SEC, (double) clock_diff);

	return EXIT_SUCCESS;
}
