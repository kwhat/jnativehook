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

#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifdef XKB
#include <X11/XKBlib.h>
static XkbDescPtr keyboard_map;
#else
#include <X11/Xutil.h>
static KeySym *keyboard_map;
static int keysyms_per_keycode;
static bool is_caps_lock = false, is_shift_lock = false;
#endif

// Unicode-Remapse the NativeHelpers display.
extern Display *disp;


// Faster more flexible alternative to XKeycodeToKeysym...
KeySym KeyCodeToKeySym(KeyCode keycode, unsigned int event_mask) {
	KeySym keysym = NoSymbol;

	#ifdef XKB
	if (keyboard_map) {
		// What is diff between XkbKeyGroupInfo and XkbKeyNumGroups?
		unsigned char info = XkbKeyGroupInfo(keyboard_map, keycode);
		unsigned int num_groups = XkbKeyNumGroups(keyboard_map, keycode);

		// Get the group.
		unsigned int group = 0x0000;
		switch (XkbOutOfRangeGroupAction(info)) {
			case XkbRedirectIntoRange:
				/* If the RedirectIntoRange flag is set, the four least significant
				 * bits of the groups wrap control specify the index of a group to
				 * which all illegal groups correspond. If the specified group is
				 * also out of range, all illegal groups map to Group1.
				 */
				group = XkbOutOfRangeGroupInfo(info);
				if (group >= num_groups) {
					group = 0;
				}
				break;

			case XkbClampIntoRange:
				/* If the ClampIntoRange flag is set, out-of-range groups correspond
				 * to the nearest legal group. Effective groups larger than the
				 * highest supported group are mapped to the highest supported group;
				 * effective groups less than Group1 are mapped to Group1 . For
				 * example, a key with two groups of symbols uses Group2 type and
				 * symbols if the global effective group is either Group3 or Group4.
				 */
				group = num_groups - 1;
				break;

			case XkbWrapIntoRange:
				/* If neither flag is set, group is wrapped into range using integer
				 * modulus. For example, a key with two groups of symbols for which
				 * groups wrap uses Group1 symbols if the global effective group is
				 * Group3 or Group2 symbols if the global effective group is Group4.
				 */
			default:
				if (num_groups != 0) {
					group %= num_groups;
				}
				break;
		}

		XkbKeyTypePtr key_type = XkbKeyKeyType(keyboard_map, keycode, group);
		unsigned int active_mods = event_mask & key_type->mods.mask;

		int i, level = 0;
		for (i = 0; i < key_type->map_count; i++) {
			if (key_type->map[i].active && key_type->map[i].mods.mask == active_mods) {
				level = key_type->map[i].level;
			}
		}

		keysym = XkbKeySymEntry(keyboard_map, keycode, level, group);
	}
	#else
	if (keyboard_map) {
		if (event_mask & Mod2Mask &&
				((keyboard_map[keycode *keysyms_per_keycode + 1] >= 0xFF80 && keyboard_map[keycode *keysyms_per_keycode + 1] <= 0xFFBD) ||
				(keyboard_map[keycode *keysyms_per_keycode + 1] >= 0x11000000 && keyboard_map[keycode *keysyms_per_keycode + 1] <= 0x1100FFFF))
			) {

			/* If the numlock modifier is on and the second KeySym is a keypad
			 * KeySym.  In this case, if the Shift modifier is on, or if the
			 * Lock modifier is on and is interpreted as ShiftLock, then the
			 * first KeySym is used, otherwise the second KeySym is used.
			 *
			 * The standard KeySyms with the prefix ``XK_KP_'' in their name are
			 * called keypad KeySyms; these are KeySyms with numeric value in
			 * the hexadecimal range 0xFF80 to 0xFFBD inclusive. In addition,
			 * vendor-specific KeySyms in the hexadecimal range 0x11000000 to
			 * 0x1100FFFF are also keypad KeySyms.
			 */


			 /* The numlock modifier is on and the second KeySym is a keypad
			  * KeySym. In this case, if the Shift modifier is on, or if the
			  * Lock modifier is on and is interpreted as ShiftLock, then the
			  * first KeySym is used, otherwise the second KeySym is used.
			  */
			if (event_mask & ShiftMask || (event_mask & LockMask && is_shift_lock)) {
				// i = 0
				keysym = keyboard_map[keycode *keysyms_per_keycode];
			}
			else {
				// i = 1
				keysym = keyboard_map[keycode *keysyms_per_keycode + 1];
			}
		}
		else if (event_mask ^ ShiftMask && event_mask ^ LockMask) {
			/* The Shift and Lock modifiers are both off. In this case,
			 * the first KeySym is used.
			 */
			// index = 0
			keysym = keyboard_map[keycode *keysyms_per_keycode];
		}
		else if (event_mask ^ ShiftMask && event_mask & LockMask && is_caps_lock) {
			/* The Shift modifier is off, and the Lock modifier is on
			 * and is interpreted as CapsLock. In this case, the first
			 * KeySym is used, but if that KeySym is lowercase
			 * alphabetic, then the corresponding uppercase KeySym is
			 * used instead.
			 */
			// index = 0;
			keysym = keyboard_map[keycode *keysyms_per_keycode];

			if (keysym >= 'a' && keysym <= 'z') {
				// keysym is an alpha char.
				KeySym lower_keysym, upper_keysym;
				XConvertCase(keysym, &lower_keysym, &upper_keysym);
				keysym = upper_keysym;
			}
		}
		else if (event_mask & ShiftMask && event_mask & LockMask && is_caps_lock) {
			/* The Shift modifier is on, and the Lock modifier is on and
			 * is interpreted as CapsLock. In this case, the second
			 * KeySym is used, but if that KeySym is lowercase
			 * alphabetic, then the corresponding uppercase KeySym is
			 * used instead.
			 */
			// index = 1
			keysym = keyboard_map[keycode *keysyms_per_keycode + 1];

			if (keysym >= 'A' && keysym <= 'Z') {
				// keysym is an alpha char.
				KeySym lower_keysym, upper_keysym;
				XConvertCase(keysym, &lower_keysym, &upper_keysym);
				keysym = lower_keysym;
			}
		}
		else if (event_mask & ShiftMask || (event_mask & LockMask && is_shift_lock) || event_mask & (ShiftMask + LockMask)) {
			/* The Shift modifier is on, or the Lock modifier is on and
			 * is interpreted as ShiftLock, or both. In this case, the
			 * second KeySym is used.
			 */
			// index = 1
			keysym = keyboard_map[keycode *keysyms_per_keycode + 1];
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "GetChar(): Unable to determine the KeySym index.\n");
		}
		#endif
	}
	#endif

	return keysym;
}

/* Mapping of X11 keysyms to ISO 10646 (Universal Character Set) for the Basic
 * Multilingual Plane. This function was created from work done by Markus Kuhn
 * and Richard Verhoeven.
 * See http://www.cl.cam.ac.uk/~mgk25/ for more information
 */
wchar_t KeySymToUnicode(KeySym keysym) {
	switch(keysym) {
		#ifdef XK_LATIN1
		case XK_space:
		case XK_exclam:
		case XK_quotedbl:
		case XK_numbersign:
		case XK_dollar:
		case XK_percent:
		case XK_ampersand:
		case XK_apostrophe:
		case XK_parenleft:
		case XK_parenright:
		case XK_asterisk:
		case XK_plus:
		case XK_comma:
		case XK_minus:
		case XK_period:
		case XK_slash:
		case XK_0:
		case XK_1:
		case XK_2:
		case XK_3:
		case XK_4:
		case XK_5:
		case XK_6:
		case XK_7:
		case XK_8:
		case XK_9:
		case XK_colon:
		case XK_semicolon:
		case XK_less:
		case XK_equal:
		case XK_greater:
		case XK_question:
		case XK_at:
		case XK_A:
		case XK_B:
		case XK_C:
		case XK_D:
		case XK_E:
		case XK_F:
		case XK_G:
		case XK_H:
		case XK_I:
		case XK_J:
		case XK_K:
		case XK_L:
		case XK_M:
		case XK_N:
		case XK_O:
		case XK_P:
		case XK_Q:
		case XK_R:
		case XK_S:
		case XK_T:
		case XK_U:
		case XK_V:
		case XK_W:
		case XK_X:
		case XK_Y:
		case XK_Z:
		case XK_bracketleft:
		case XK_backslash:
		case XK_bracketright:
		case XK_asciicircum:
		case XK_underscore:
		case XK_grave:
		case XK_a:
		case XK_b:
		case XK_c:
		case XK_d:
		case XK_e:
		case XK_f:
		case XK_g:
		case XK_h:
		case XK_i:
		case XK_j:
		case XK_k:
		case XK_l:
		case XK_m:
		case XK_n:
		case XK_o:
		case XK_p:
		case XK_q:
		case XK_r:
		case XK_s:
		case XK_t:
		case XK_u:
		case XK_v:
		case XK_w:
		case XK_x:
		case XK_y:
		case XK_z:
		case XK_braceleft:
		case XK_bar:
		case XK_braceright:
		case XK_asciitilde:
		case XK_nobreakspace:
		case XK_exclamdown:
		case XK_cent:
		case XK_sterling:
		case XK_currency:
		case XK_yen:
		case XK_brokenbar:
		case XK_section:
		case XK_diaeresis:
		case XK_copyright:
		case XK_ordfeminine:
		case XK_guillemotleft:
		case XK_notsign:
		case XK_hyphen:
		case XK_registered:
		case XK_macron:
		case XK_degree:
		case XK_plusminus:
		case XK_twosuperior:
		case XK_threesuperior:
		case XK_acute:
		case XK_mu:
		case XK_paragraph:
		case XK_periodcentered:
		case XK_cedilla:
		case XK_onesuperior:
		case XK_masculine:
		case XK_guillemotright:
		case XK_onequarter:
		case XK_onehalf:
		case XK_threequarters:
		case XK_questiondown:
		case XK_Agrave:
		case XK_Aacute:
		case XK_Acircumflex:
		case XK_Atilde:
		case XK_Adiaeresis:
		case XK_Aring:
		case XK_AE:
		case XK_Ccedilla:
		case XK_Egrave:
		case XK_Eacute:
		case XK_Ecircumflex:
		case XK_Ediaeresis:
		case XK_Igrave:
		case XK_Iacute:
		case XK_Icircumflex:
		case XK_Idiaeresis:
		case XK_ETH:
		case XK_Ntilde:
		case XK_Ograve:
		case XK_Oacute:
		case XK_Ocircumflex:
		case XK_Otilde:
		case XK_Odiaeresis:
		case XK_multiply:
		#ifdef XK_Oslash
		case XK_Oslash:
		#else
		case XK_Ooblique:
		#endif
		case XK_Ugrave:
		case XK_Uacute:
		case XK_Ucircumflex:
		case XK_Udiaeresis:
		case XK_Yacute:
		case XK_THORN:
		case XK_ssharp:
		case XK_agrave:
		case XK_aacute:
		case XK_acircumflex:
		case XK_atilde:
		case XK_adiaeresis:
		case XK_aring:
		case XK_ae:
		case XK_ccedilla:
		case XK_egrave:
		case XK_eacute:
		case XK_ecircumflex:
		case XK_ediaeresis:
		case XK_igrave:
		case XK_iacute:
		case XK_icircumflex:
		case XK_idiaeresis:
		case XK_eth:
		case XK_ntilde:
		case XK_ograve:
		case XK_oacute:
		case XK_ocircumflex:
		case XK_otilde:
		case XK_odiaeresis:
		case XK_division:
		#ifdef XK_oslash
		case XK_oslash:
		#else
		case XK_ooblique:
		#endif
		case XK_ugrave:
		case XK_uacute:
		case XK_ucircumflex:
		case XK_udiaeresis:
		case XK_yacute:
		case XK_thorn:
		case XK_ydiaeresis:
			return keysym;
		case XK_Tab:
		#ifdef XK_MISCELLANY
		case XK_KP_Tab:
		#endif
			return '\t';
		case XK_Return:
		#ifdef XK_MISCELLANY
		case XK_KP_Enter:
		#endif
			return '\n';
		#ifdef XK_MISCELLANY
		case XK_KP_0:
		case XK_KP_1:
		case XK_KP_2:
		case XK_KP_3:
		case XK_KP_4:
		case XK_KP_5:
		case XK_KP_6:
		case XK_KP_7:
		case XK_KP_8:
		case XK_KP_9:
			return keysym - (XK_KP_0 - XK_0);		// Assumes values are defined in order.
		case XK_Linefeed:
			return '\r';
		case XK_KP_Space:
			return XK_space;
		case XK_BackSpace:
			return '\b';
		case XK_KP_Equal:
			return XK_equal;
		case XK_KP_Multiply:
			return XK_asterisk;
		case XK_KP_Add:
			return XK_plus;
		case XK_KP_Separator:
			return XK_comma;						// Often but not always a comma?
		case XK_KP_Subtract:
			return XK_minus;
		case XK_KP_Decimal:
			return XK_period;
		case XK_KP_Divide:
			return XK_slash;
		#endif
		#endif


		#ifdef XK_LATIN2
		case XK_Aogonek:
			return 0x0104;
		case XK_breve:
			return 0x02D8;
		case XK_Lstroke:
			return 0x0141;
		case XK_Lcaron:
			return 0x013D;
		case XK_Sacute:
			return 0x015A;
		case XK_Scaron:
			return 0x0160;
		case XK_Scedilla:
			return 0x015E;
		case XK_Tcaron:
			return 0x0164;
		case XK_Zacute:
			return 0x0179;
		case XK_Zcaron:
			return 0x017D;
		case XK_Zabovedot:
			return 0x017B;
		case XK_aogonek:
			return 0x0105;
		case XK_ogonek:
			return 0x02DB;
		case XK_lstroke:
			return 0x0142;
		case XK_lcaron:
			return 0x013E;
		case XK_sacute:
			return 0x015B;
		case XK_caron:
			return 0x02C7;
		case XK_scaron:
			return 0x0161;
		case XK_scedilla:
			return 0x015F;
		case XK_tcaron:
			return 0x0165;
		case XK_zacute:
			return 0x017A;
		case XK_doubleacute:
			return 0x02DD;
		case XK_zcaron:
			return 0x017E;
		case XK_zabovedot:
			return 0x017C;
		case XK_Racute:
			return 0x0154;
		case XK_Abreve:
			return 0x0102;
		case XK_Lacute:
			return 0x0139;
		case XK_Cacute:
			return 0x0106;
		case XK_Ccaron:
			return 0x010C;
		case XK_Eogonek:
			return 0x0118;
		case XK_Ecaron:
			return 0x011A;
		case XK_Dcaron:
			return 0x010E;
		case XK_Dstroke:
			return 0x0110;
		case XK_Nacute:
			return 0x0143;
		case XK_Ncaron:
			return 0x0147;
		case XK_Odoubleacute:
			return 0x0150;
		case XK_Rcaron:
			return 0x0158;
		case XK_Uring:
			return 0x016E;
		case XK_Udoubleacute:
			return 0x0170;
		case XK_Tcedilla:
			return 0x0162;
		case XK_racute:
			return 0x0155;
		case XK_abreve:
			return 0x0103;
		case XK_lacute:
			return 0x013A;
		case XK_cacute:
			return 0x0107;
		case XK_ccaron:
			return 0x010D;
		case XK_eogonek:
			return 0x0119;
		case XK_ecaron:
			return 0x011B;
		case XK_dcaron:
			return 0x010F;
		case XK_dstroke:
			return 0x0111;
		case XK_nacute:
			return 0x0144;
		case XK_ncaron:
			return 0x0148;
		case XK_odoubleacute:
			return 0x0151;
		case XK_rcaron:
			return 0x0159;
		case XK_uring:
			return 0x016F;
		case XK_udoubleacute:
			return 0x0171;
		case XK_tcedilla:
			return 0x0163;
		case XK_abovedot:
			return 0x02D9;
		#endif


		#ifdef XK_LATIN3
		case XK_Hstroke:
			return 0x0126;
		case XK_Hcircumflex:
			return 0x0124;
		case XK_Iabovedot:
			return 0x0130;
		case XK_Gbreve:
			return 0x011E;
		case XK_Jcircumflex:
			return 0x0134;
		case XK_hstroke:
			return 0x0127;
		case XK_hcircumflex:
			return 0x0125;
		case XK_idotless:
			return 0x0131;
		case XK_gbreve:
			return 0x011F;
		case XK_jcircumflex:
			return 0x0135;
		case XK_Cabovedot:
			return 0x010A;
		case XK_Ccircumflex:
			return 0x0108;
		case XK_Gabovedot:
			return 0x0120;
		case XK_Gcircumflex:
			return 0x011C;
		case XK_Ubreve:
			return 0x016C;
		case XK_Scircumflex:
			return 0x015C;
		case XK_cabovedot:
			return 0x010B;
		case XK_ccircumflex:
			return 0x0109;
		case XK_gabovedot:
			return 0x0121;
		case XK_gcircumflex:
			return 0x011D;
		case XK_ubreve:
			return 0x016D;
		case XK_scircumflex:
			return 0x015D;
		#endif


		#ifdef XK_LATIN4
		case XK_kra:
			return 0x0138;
		case XK_Rcedilla:
			return 0x0156;
		case XK_Itilde:
			return 0x0128;
		case XK_Lcedilla:
			return 0x013B;
		case XK_Emacron:
			return 0x0112;
		case XK_Gcedilla:
			return 0x0122;
		case XK_Tslash:
			return 0x0166;
		case XK_rcedilla:
			return 0x0157;
		case XK_itilde:
			return 0x0129;
		case XK_lcedilla:
			return 0x013C;
		case XK_emacron:
			return 0x0113;
		case XK_gcedilla:
			return 0x0123;
		case XK_tslash:
			return 0x0167;
		case XK_ENG:
			return 0x014A;
		case XK_eng:
			return 0x014B;
		case XK_Amacron:
			return 0x0100;
		case XK_Iogonek:
			return 0x012E;
		case XK_Eabovedot:
			return 0x0116;
		case XK_Imacron:
			return 0x012A;
		case XK_Ncedilla:
			return 0x0145;
		case XK_Omacron:
			return 0x014C;
		case XK_Kcedilla:
			return 0x0136;
		case XK_Uogonek:
			return 0x0172;
		case XK_Utilde:
			return 0x0168;
		case XK_Umacron:
			return 0x016A;
		case XK_amacron:
			return 0x0101;
		case XK_iogonek:
			return 0x012F;
		case XK_eabovedot:
			return 0x0117;
		case XK_imacron:
			return 0x012B;
		case XK_ncedilla:
			return 0x0146;
		case XK_omacron:
			return 0x014D;
		case XK_kcedilla:
			return 0x0137;
		case XK_uogonek:
			return 0x0173;
		case XK_utilde:
			return 0x0169;
		case XK_umacron:
			return 0x016B;
		#endif


		#ifdef XK_LATIN8
		case XK_Wcircumflex:
			return 0x0174;
		case XK_wcircumflex:
			return 0x0175;
		case XK_Ycircumflex:
			return 0x0176;
		case XK_ycircumflex:
			return 0x0177;
		case XK_Babovedot:
			return 0x1E02;
		case XK_babovedot:
			return 0x1E03;
		case XK_Dabovedot:
			return 0x1E0A;
		case XK_dabovedot:
			return 0x1E0B;
		case XK_Fabovedot:
			return 0x1E1E;
		case XK_fabovedot:
			return 0x1E1F;
		case XK_Mabovedot:
			return 0x1E40;
		case XK_mabovedot:
			return 0x1E41;
		case XK_Pabovedot:
			return 0x1E56;
		case XK_pabovedot:
			return 0x1E57;
		case XK_Sabovedot:
			return 0x1E60;
		case XK_sabovedot:
			return 0x1E61;
		case XK_Tabovedot:
			return 0x1E6A;
		case XK_tabovedot:
			return 0x1E6B;
		case XK_Wgrave:
			return 0x1E80;
		case XK_wgrave:
			return 0x1E81;
		case XK_Wacute:
			return 0x1E82;
		case XK_wacute:
			return 0x1E83;
		case XK_Wdiaeresis:
			return 0x1E84;
		case XK_wdiaeresis:
			return 0x1E85;
		case XK_Ygrave:
			return 0x1EF2;
		case XK_ygrave:
			return 0x1EF3;
		#endif


		#ifdef XK_LATIN9
		case XK_OE:
			return 0x0152;
		case XK_oe:
			return 0x0153;
		case XK_Ydiaeresis:
			return 0x0178;
		#endif


		#ifdef XK_KATAKANA
		case XK_overline:
			return 0x203E;
		case XK_kana_fullstop:
			return 0x3002;
		case XK_kana_openingbracket:
			return 0x300C;
		case XK_kana_closingbracket:
			return 0x300D;
		case XK_kana_comma:
			return 0x3001;
		case XK_kana_conjunctive:
			return 0x30FB;
		case XK_kana_WO:
			return 0x30F2;
		case XK_kana_a:
			return 0x30A1;
		case XK_kana_i:
			return 0x30A3;
		case XK_kana_u:
			return 0x30A5;
		case XK_kana_e:
			return 0x30A7;
		case XK_kana_o:
			return 0x30A9;
		case XK_kana_ya:
			return 0x30E3;
		case XK_kana_yu:
			return 0x30E5;
		case XK_kana_yo:
			return 0x30E7;
		case XK_kana_tsu:
			return 0x30C3;
		case XK_prolongedsound:
			return 0x30FC;
		case XK_kana_A:
			return 0x30A2;
		case XK_kana_I:
			return 0x30A4;
		case XK_kana_U:
			return 0x30A6;
		case XK_kana_E:
			return 0x30A8;
		case XK_kana_O:
			return 0x30AA;
		case XK_kana_KA:
			return 0x30AB;
		case XK_kana_KI:
			return 0x30AD;
		case XK_kana_KU:
			return 0x30AF;
		case XK_kana_KE:
			return 0x30B1;
		case XK_kana_KO:
			return 0x30B3;
		case XK_kana_SA:
			return 0x30B5;
		case XK_kana_SHI:
			return 0x30B7;
		case XK_kana_SU:
			return 0x30B9;
		case XK_kana_SE:
			return 0x30BB;
		case XK_kana_SO:
			return 0x30BD;
		case XK_kana_TA:
			return 0x30BF;
		case XK_kana_CHI:
			return 0x30C1;
		case XK_kana_TSU:
			return 0x30C4;
		case XK_kana_TE:
			return 0x30C6;
		case XK_kana_TO:
			return 0x30C8;
		case XK_kana_NA:
			return 0x30CA;
		case XK_kana_NI:
			return 0x30CB;
		case XK_kana_NU:
			return 0x30CC;
		case XK_kana_NE:
			return 0x30CD;
		case XK_kana_NO:
			return 0x30CE;
		case XK_kana_HA:
			return 0x30CF;
		case XK_kana_HI:
			return 0x30D2;
		case XK_kana_FU:
			return 0x30D5;
		case XK_kana_HE:
			return 0x30D8;
		case XK_kana_HO:
			return 0x30DB;
		case XK_kana_MA:
			return 0x30DE;
		case XK_kana_MI:
			return 0x30DF;
		case XK_kana_MU:
			return 0x30E0;
		case XK_kana_ME:
			return 0x30E1;
		case XK_kana_MO:
			return 0x30E2;
		case XK_kana_YA:
			return 0x30E4;
		case XK_kana_YU:
			return 0x30E6;
		case XK_kana_YO:
			return 0x30E8;
		case XK_kana_RA:
			return 0x30E9;
		case XK_kana_RI:
			return 0x30EA;
		case XK_kana_RU:
			return 0x30EB;
		case XK_kana_RE:
			return 0x30EC;
		case XK_kana_RO:
			return 0x30ED;
		case XK_kana_WA:
			return 0x30EF;
		case XK_kana_N:
			return 0x30F3;
		case XK_voicedsound:
			return 0x309B;
		case XK_semivoicedsound:
			return 0x309C;
		#endif


		#ifdef XK_ARABIC
		case XK_Farsi_0:
			return 0x06F0;
		case XK_Farsi_1:
			return 0x06F1;
		case XK_Farsi_2:
			return 0x06F2;
		case XK_Farsi_3:
			return 0x06F3;
		case XK_Farsi_4:
			return 0x06F4;
		case XK_Farsi_5:
			return 0x06F5;
		case XK_Farsi_6:
			return 0x06F6;
		case XK_Farsi_7:
			return 0x06F7;
		case XK_Farsi_8:
			return 0x06F8;
		case XK_Farsi_9:
			return 0x06F9;
		case XK_Arabic_percent:
			return 0x066A;
		case XK_Arabic_superscript_alef:
			return 0x0670;
		case XK_Arabic_tteh:
			return 0x0679;
		case XK_Arabic_peh:
			return 0x067E;
		case XK_Arabic_tcheh:
			return 0x0686;
		case XK_Arabic_ddal:
			return 0x0688;
		case XK_Arabic_rreh:
			return 0x0691;
		case XK_Arabic_comma:
			return 0x060C;
		case XK_Arabic_fullstop:
			return 0x06D4;
		case XK_Arabic_0:
			return 0x0660;
		case XK_Arabic_1:
			return 0x0661;
		case XK_Arabic_2:
			return 0x0662;
		case XK_Arabic_3:
			return 0x0663;
		case XK_Arabic_4:
			return 0x0664;
		case XK_Arabic_5:
			return 0x0665;
		case XK_Arabic_6:
			return 0x0666;
		case XK_Arabic_7:
			return 0x0667;
		case XK_Arabic_8:
			return 0x0668;
		case XK_Arabic_9:
			return 0x0669;
		case XK_Arabic_semicolon:
			return 0x061B;
		case XK_Arabic_question_mark:
			return 0x061F;
		case XK_Arabic_hamza:
			return 0x0621;
		case XK_Arabic_maddaonalef:
			return 0x0622;
		case XK_Arabic_hamzaonalef:
			return 0x0623;
		case XK_Arabic_hamzaonwaw:
			return 0x0624;
		case XK_Arabic_hamzaunderalef:
			return 0x0625;
		case XK_Arabic_hamzaonyeh:
			return 0x0626;
		case XK_Arabic_alef:
			return 0x0627;
		case XK_Arabic_beh:
			return 0x0628;
		case XK_Arabic_tehmarbuta:
			return 0x0629;
		case XK_Arabic_teh:
			return 0x062A;
		case XK_Arabic_theh:
			return 0x062B;
		case XK_Arabic_jeem:
			return 0x062C;
		case XK_Arabic_hah:
			return 0x062D;
		case XK_Arabic_khah:
			return 0x062E;
		case XK_Arabic_dal:
			return 0x062F;
		case XK_Arabic_thal:
			return 0x0630;
		case XK_Arabic_ra:
			return 0x0631;
		case XK_Arabic_zain:
			return 0x0632;
		case XK_Arabic_seen:
			return 0x0633;
		case XK_Arabic_sheen:
			return 0x0634;
		case XK_Arabic_sad:
			return 0x0635;
		case XK_Arabic_dad:
			return 0x0636;
		case XK_Arabic_tah:
			return 0x0637;
		case XK_Arabic_zah:
			return 0x0638;
		case XK_Arabic_ain:
			return 0x0639;
		case XK_Arabic_ghain:
			return 0x063A;
		case XK_Arabic_tatweel:
			return 0x0640;
		case XK_Arabic_feh:
			return 0x0641;
		case XK_Arabic_qaf:
			return 0x0642;
		case XK_Arabic_kaf:
			return 0x0643;
		case XK_Arabic_lam:
			return 0x0644;
		case XK_Arabic_meem:
			return 0x0645;
		case XK_Arabic_noon:
			return 0x0646;
		case XK_Arabic_ha:
			return 0x0647;
		case XK_Arabic_waw:
			return 0x0648;
		case XK_Arabic_alefmaksura:
			return 0x0649;
		case XK_Arabic_yeh:
			return 0x064A;
		case XK_Arabic_fathatan:
			return 0x064B;
		case XK_Arabic_dammatan:
			return 0x064C;
		case XK_Arabic_kasratan:
			return 0x064D;
		case XK_Arabic_fatha:
			return 0x064E;
		case XK_Arabic_damma:
			return 0x064F;
		case XK_Arabic_kasra:
			return 0x0650;
		case XK_Arabic_shadda:
			return 0x0651;
		case XK_Arabic_sukun:
			return 0x0652;
		case XK_Arabic_madda_above:
			return 0x0653;
		case XK_Arabic_hamza_above:
			return 0x0654;
		case XK_Arabic_hamza_below:
			return 0x0655;
		case XK_Arabic_jeh:
			return 0x0698;
		case XK_Arabic_veh:
			return 0x06A4;
		case XK_Arabic_keheh:
			return 0x06A9;
		case XK_Arabic_gaf:
			return 0x06AF;
		case XK_Arabic_noon_ghunna:
			return 0x06BA;
		case XK_Arabic_heh_doachashmee:
			return 0x06BE;
		#ifdef XK_Farsi_yeh
		case XK_Farsi_yeh:
		#else
		case XK_Arabic_farsi_yeh:
		#endif
			return 0x06CC;
		case XK_Arabic_yeh_baree:
			return 0x06D2;
		case XK_Arabic_heh_goal:
			return 0x06C1;
		#endif


		#ifdef XK_CYRILLIC
		case XK_Cyrillic_GHE_bar:
			return 0x0492;
		case XK_Cyrillic_ghe_bar:
			return 0x0493;
		case XK_Cyrillic_ZHE_descender:
			return 0x0496;
		case XK_Cyrillic_zhe_descender:
			return 0x0497;
		case XK_Cyrillic_KA_descender:
			return 0x049A;
		case XK_Cyrillic_ka_descender:
			return 0x049B;
		case XK_Cyrillic_KA_vertstroke:
			return 0x049C;
		case XK_Cyrillic_ka_vertstroke:
			return 0x049D;
		case XK_Cyrillic_EN_descender:
			return 0x04A2;
		case XK_Cyrillic_en_descender:
			return 0x04A3;
		case XK_Cyrillic_U_straight:
			return 0x04AE;
		case XK_Cyrillic_u_straight:
			return 0x04AF;
		case XK_Cyrillic_U_straight_bar:
			return 0x04B0;
		case XK_Cyrillic_u_straight_bar:
			return 0x04B1;
		case XK_Cyrillic_HA_descender:
			return 0x04B2;
		case XK_Cyrillic_ha_descender:
			return 0x04B3;
		case XK_Cyrillic_CHE_descender:
			return 0x04B6;
		case XK_Cyrillic_che_descender:
			return 0x04B7;
		case XK_Cyrillic_CHE_vertstroke:
			return 0x04B8;
		case XK_Cyrillic_che_vertstroke:
			return 0x04B9;
		case XK_Cyrillic_SHHA:
			return 0x04BA;
		case XK_Cyrillic_shha:
			return 0x04BB;
		case XK_Cyrillic_SCHWA:
			return 0x04D8;
		case XK_Cyrillic_schwa:
			return 0x04D9;
		case XK_Cyrillic_I_macron:
			return 0x04E2;
		case XK_Cyrillic_i_macron:
			return 0x04E3;
		case XK_Cyrillic_O_bar:
			return 0x04E8;
		case XK_Cyrillic_o_bar:
			return 0x04E9;
		case XK_Cyrillic_U_macron:
			return 0x04EE;
		case XK_Cyrillic_u_macron:
			return 0x04EF;
		case XK_Serbian_dje:
			return 0x0452;
		case XK_Macedonia_gje:
			return 0x0453;
		case XK_Cyrillic_io:
			return 0x0451;
		case XK_Ukrainian_ie:
			return 0x0454;
		case XK_Macedonia_dse:
			return 0x0455;
		case XK_Ukrainian_i:
			return 0x0456;
		case XK_Ukrainian_yi:
			return 0x0457;
		case XK_Cyrillic_je:
			return 0x0458;
		case XK_Cyrillic_lje:
			return 0x0459;
		case XK_Cyrillic_nje:
			return 0x045A;
		case XK_Serbian_tshe:
			return 0x045B;
		case XK_Macedonia_kje:
			return 0x045C;
		case XK_Ukrainian_ghe_with_upturn:
			return 0x0491;
		case XK_Byelorussian_shortu:
			return 0x045E;
		case XK_Cyrillic_dzhe:
			return 0x045F;
		case XK_numerosign:
			return 0x2116;
		case XK_Serbian_DJE:
			return 0x0402;
		case XK_Macedonia_GJE:
			return 0x0403;
		case XK_Cyrillic_IO:
			return 0x0401;
		case XK_Ukrainian_IE:
			return 0x0404;
		case XK_Macedonia_DSE:
			return 0x0405;
		case XK_Ukrainian_I:
			return 0x0406;
		case XK_Ukrainian_YI:
			return 0x0407;
		case XK_Cyrillic_JE:
			return 0x0408;
		case XK_Cyrillic_LJE:
			return 0x0409;
		case XK_Cyrillic_NJE:
			return 0x040A;
		case XK_Serbian_TSHE:
			return 0x040B;
		case XK_Macedonia_KJE:
			return 0x040C;
		case XK_Ukrainian_GHE_WITH_UPTURN:
			return 0x0490;
		case XK_Byelorussian_SHORTU:
			return 0x040E;
		case XK_Cyrillic_DZHE:
			return 0x040F;
		case XK_Cyrillic_yu:
			return 0x044E;
		case XK_Cyrillic_a:
			return 0x0430;
		case XK_Cyrillic_be:
			return 0x0431;
		case XK_Cyrillic_tse:
			return 0x0446;
		case XK_Cyrillic_de:
			return 0x0434;
		case XK_Cyrillic_ie:
			return 0x0435;
		case XK_Cyrillic_ef:
			return 0x0444;
		case XK_Cyrillic_ghe:
			return 0x0433;
		case XK_Cyrillic_ha:
			return 0x0445;
		case XK_Cyrillic_i:
			return 0x0438;
		case XK_Cyrillic_shorti:
			return 0x0439;
		case XK_Cyrillic_ka:
			return 0x043A;
		case XK_Cyrillic_el:
			return 0x043B;
		case XK_Cyrillic_em:
			return 0x043C;
		case XK_Cyrillic_en:
			return 0x043D;
		case XK_Cyrillic_o:
			return 0x043E;
		case XK_Cyrillic_pe:
			return 0x043F;
		case XK_Cyrillic_ya:
			return 0x044F;
		case XK_Cyrillic_er:
			return 0x0440;
		case XK_Cyrillic_es:
			return 0x0441;
		case XK_Cyrillic_te:
			return 0x0442;
		case XK_Cyrillic_u:
			return 0x0443;
		case XK_Cyrillic_zhe:
			return 0x0436;
		case XK_Cyrillic_ve:
			return 0x0432;
		case XK_Cyrillic_softsign:
			return 0x044C;
		case XK_Cyrillic_yeru:
			return 0x044B;
		case XK_Cyrillic_ze:
			return 0x0437;
		case XK_Cyrillic_sha:
			return 0x0448;
		case XK_Cyrillic_e:
			return 0x044D;
		case XK_Cyrillic_shcha:
			return 0x0449;
		case XK_Cyrillic_che:
			return 0x0447;
		case XK_Cyrillic_hardsign:
			return 0x044A;
		case XK_Cyrillic_YU:
			return 0x042E;
		case XK_Cyrillic_A:
			return 0x0410;
		case XK_Cyrillic_BE:
			return 0x0411;
		case XK_Cyrillic_TSE:
			return 0x0426;
		case XK_Cyrillic_DE:
			return 0x0414;
		case XK_Cyrillic_IE:
			return 0x0415;
		case XK_Cyrillic_EF:
			return 0x0424;
		case XK_Cyrillic_GHE:
			return 0x0413;
		case XK_Cyrillic_HA:
			return 0x0425;
		case XK_Cyrillic_I:
			return 0x0418;
		case XK_Cyrillic_SHORTI:
			return 0x0419;
		case XK_Cyrillic_KA:
			return 0x041A;
		case XK_Cyrillic_EL:
			return 0x041B;
		case XK_Cyrillic_EM:
			return 0x041C;
		case XK_Cyrillic_EN:
			return 0x041D;
		case XK_Cyrillic_O:
			return 0x041E;
		case XK_Cyrillic_PE:
			return 0x041F;
		case XK_Cyrillic_YA:
			return 0x042F;
		case XK_Cyrillic_ER:
			return 0x0420;
		case XK_Cyrillic_ES:
			return 0x0421;
		case XK_Cyrillic_TE:
			return 0x0422;
		case XK_Cyrillic_U:
			return 0x0423;
		case XK_Cyrillic_ZHE:
			return 0x0416;
		case XK_Cyrillic_VE:
			return 0x0412;
		case XK_Cyrillic_SOFTSIGN:
			return 0x042C;
		case XK_Cyrillic_YERU:
			return 0x042B;
		case XK_Cyrillic_ZE:
			return 0x0417;
		case XK_Cyrillic_SHA:
			return 0x0428;
		case XK_Cyrillic_E:
			return 0x042D;
		case XK_Cyrillic_SHCHA:
			return 0x0429;
		case XK_Cyrillic_CHE:
			return 0x0427;
		case XK_Cyrillic_HARDSIGN:
			return 0x042A;
		#endif


		#ifdef XK_GREEK
		case XK_Greek_ALPHAaccent:
			return 0x0386;
		case XK_Greek_EPSILONaccent:
			return 0x0388;
		case XK_Greek_ETAaccent:
			return 0x0389;
		case XK_Greek_IOTAaccent:
			return 0x038A;
		case XK_Greek_IOTAdieresis:
			return 0x03AA;
		case XK_Greek_OMICRONaccent:
			return 0x038C;
		case XK_Greek_UPSILONaccent:
			return 0x038E;
		case XK_Greek_UPSILONdieresis:
			return 0x03AB;
		case XK_Greek_OMEGAaccent:
			return 0x038F;
		case XK_Greek_accentdieresis:
			return 0x0385;
		case XK_Greek_horizbar:
			return 0x2015;
		case XK_Greek_alphaaccent:
			return 0x03AC;
		case XK_Greek_epsilonaccent:
			return 0x03AD;
		case XK_Greek_etaaccent:
			return 0x03AE;
		case XK_Greek_iotaaccent:
			return 0x03AF;
		case XK_Greek_iotadieresis:
			return 0x03CA;
		case XK_Greek_iotaaccentdieresis:
			return 0x0390;
		case XK_Greek_omicronaccent:
			return 0x03CC;
		case XK_Greek_upsilonaccent:
			return 0x03CD;
		case XK_Greek_upsilondieresis:
			return 0x03CB;
		case XK_Greek_upsilonaccentdieresis:
			return 0x03B0;
		case XK_Greek_omegaaccent:
			return 0x03CE;
		case XK_Greek_ALPHA:
			return 0x0391;
		case XK_Greek_BETA:
			return 0x0392;
		case XK_Greek_GAMMA:
			return 0x0393;
		case XK_Greek_DELTA:
			return 0x0394;
		case XK_Greek_EPSILON:
			return 0x0395;
		case XK_Greek_ZETA:
			return 0x0396;
		case XK_Greek_ETA:
			return 0x0397;
		case XK_Greek_THETA:
			return 0x0398;
		case XK_Greek_IOTA:
			return 0x0399;
		case XK_Greek_KAPPA:
			return 0x039A;
		#ifdef XK_Greek_LAMDA
		case XK_Greek_LAMDA:
		#else
		case XK_Greek_LAMBDA:
		#endif
			return 0x039B;
		case XK_Greek_MU:
			return 0x039C;
		case XK_Greek_NU:
			return 0x039D;
		case XK_Greek_XI:
			return 0x039E;
		case XK_Greek_OMICRON:
			return 0x039F;
		case XK_Greek_PI:
			return 0x03A0;
		case XK_Greek_RHO:
			return 0x03A1;
		case XK_Greek_SIGMA:
			return 0x03A3;
		case XK_Greek_TAU:
			return 0x03A4;
		case XK_Greek_UPSILON:
			return 0x03A5;
		case XK_Greek_PHI:
			return 0x03A6;
		case XK_Greek_CHI:
			return 0x03A7;
		case XK_Greek_PSI:
			return 0x03A8;
		case XK_Greek_OMEGA:
			return 0x03A9;
		case XK_Greek_alpha:
			return 0x03B1;
		case XK_Greek_beta:
			return 0x03B2;
		case XK_Greek_gamma:
			return 0x03B3;
		case XK_Greek_delta:
			return 0x03B4;
		case XK_Greek_epsilon:
			return 0x03B5;
		case XK_Greek_zeta:
			return 0x03B6;
		case XK_Greek_eta:
			return 0x03B7;
		case XK_Greek_theta:
			return 0x03B8;
		case XK_Greek_iota:
			return 0x03B9;
		case XK_Greek_kappa:
			return 0x03BA;
		#ifdef XK_Greek_lambda
		case XK_Greek_lamda:
		#else
		case XK_Greek_lambda:
		#endif
			return 0x03BB;
		case XK_Greek_mu:
			return 0x03BC;
		case XK_Greek_nu:
			return 0x03BD;
		case XK_Greek_xi:
			return 0x03BE;
		case XK_Greek_omicron:
			return 0x03BF;
		case XK_Greek_pi:
			return 0x03C0;
		case XK_Greek_rho:
			return 0x03C1;
		case XK_Greek_sigma:
			return 0x03C3;
		case XK_Greek_finalsmallsigma:
			return 0x03C2;
		case XK_Greek_tau:
			return 0x03C4;
		case XK_Greek_upsilon:
			return 0x03C5;
		case XK_Greek_phi:
			return 0x03C6;
		case XK_Greek_chi:
			return 0x03C7;
		case XK_Greek_psi:
			return 0x03C8;
		case XK_Greek_omega:
			return 0x03C9;
		#endif


		#ifdef XK_TECHNICAL
		case XK_leftradical:
			return 0x23B7;
		case XK_topleftradical:
			return 0xXBOX;
		case XK_horizconnector:
			return 0xXBOX;
		case XK_topintegral:
			return 0x2320;
		case XK_botintegral:
			return 0x2321;
		case XK_vertconnector:
			return 0xXBOX;
		case XK_topleftsqbracket:
			return 0x23A1;
		case XK_botleftsqbracket:
			return 0x23A3;
		case XK_toprightsqbracket:
			return 0x23A4;
		case XK_botrightsqbracket:
			return 0x23A6;
		case XK_topleftparens:
			return 0x239B;
		case XK_botleftparens:
			return 0x239D;
		case XK_toprightparens:
			return 0x239E;
		case XK_botrightparens:
			return 0x23A0;
		case XK_leftmiddlecurlybrace:
			return 0x23A8;
		case XK_rightmiddlecurlybrace:
			return 0x23AC;
		case XK_lessthanequal:
			return 0x2264;
		case XK_notequal:
			return 0x2260;
		case XK_greaterthanequal:
			return 0x2265;
		case XK_integral:
			return 0x222B;
		case XK_therefore:
			return 0x2234;
		case XK_variation:
			return 0x221D;
		case XK_infinity:
			return 0x221E;
		case XK_nabla:
			return 0x2207;
		case XK_approximate:
			return 0x223C;
		case XK_similarequal:
			return 0x2243;
		case XK_ifonlyif:
			return 0x21D4;
		case XK_implies:
			return 0x21D2;
		case XK_identical:
			return 0x2261;
		case XK_radical:
			return 0x221A;
		case XK_includedin:
			return 0x2282;
		case XK_includes:
			return 0x2283;
		case XK_intersection:
			return 0x2229;
		case XK_union:
			return 0x222A;
		case XK_logicaland:
			return 0x2227;
		case XK_logicalor:
			return 0x2228;
		case XK_partialderivative:
			return 0x2202;
		case XK_function:
			return 0x0192;
		case XK_leftarrow:
			return 0x2190;
		case XK_uparrow:
			return 0x2191;
		case XK_rightarrow:
			return 0x2192;
		case XK_downarrow:
			return 0x2193;
		#endif


		#ifdef XK_SPECIAL
		case XK_soliddiamond:
			return 0x25C6;
		case XK_checkerboard:
			return 0x2592;
		case XK_ht:
			return 0x2409;
		case XK_ff:
			return 0x240C;
		case XK_cr:
			return 0x240D;
		case XK_lf:
			return 0x240A;
		case XK_nl:
			return 0x2424;
		case XK_vt:
			return 0x240B;
		case XK_lowrightcorner:
			return 0x2518;
		case XK_uprightcorner:
			return 0x2510;
		case XK_upleftcorner:
			return 0x250C;
		case XK_lowleftcorner:
			return 0x2514;
		case XK_crossinglines:
			return 0x253C;
		case XK_horizlinescan1:
			return 0x23BA;
		case XK_horizlinescan3:
			return 0x23BB;
		case XK_horizlinescan5:
			return 0x2500;
		case XK_horizlinescan7:
			return 0x23BC;
		case XK_horizlinescan9:
			return 0x23BD;
		case XK_leftt:
			return 0x251C;
		case XK_rightt:
			return 0x2524;
		case XK_bott:
			return 0x2534;
		case XK_topt:
			return 0x252C;
		case XK_vertbar:
			return 0x2502;
		#endif


		#ifdef XK_PUBLISHING
		case XK_emspace:
			return 0x2003;
		case XK_enspace:
			return 0x2002;
		case XK_em3space:
			return 0x2004;
		case XK_em4space:
			return 0x2005;
		case XK_digitspace:
			return 0x2007;
		case XK_punctspace:
			return 0x2008;
		case XK_thinspace:
			return 0x2009;
		case XK_hairspace:
			return 0x200A;
		case XK_emdash:
			return 0x2014;
		case XK_endash:
			return 0x2013;
		case XK_signifblank:
			return 0x2423;
		case XK_ellipsis:
			return 0x2026;
		case XK_doubbaselinedot:
			return 0x2025;
		case XK_onethird:
			return 0x2153;
		case XK_twothirds:
			return 0x2154;
		case XK_onefifth:
			return 0x2155;
		case XK_twofifths:
			return 0x2156;
		case XK_threefifths:
			return 0x2157;
		case XK_fourfifths:
			return 0x2158;
		case XK_onesixth:
			return 0x2159;
		case XK_fivesixths:
			return 0x215A;
		case XK_careof:
			return 0x2105;
		case XK_figdash:
			return 0x2012;
		case XK_leftanglebracket:
			return 0x27E8;
		case XK_decimalpoint:
			return 0x002E;
		case XK_rightanglebracket:
			return 0x27E9;
		case XK_oneeighth:
			return 0x215B;
		case XK_threeeighths:
			return 0x215C;
		case XK_fiveeighths:
			return 0x215D;
		case XK_seveneighths:
			return 0x215E;
		case XK_trademark:
			return 0x2122;
		case XK_signaturemark:
			return 0x2613;
		case XK_leftopentriangle:
			return 0x25C1;
		case XK_rightopentriangle:
			return 0x25B7;
		case XK_emopencircle:
			return 0x25CB;
		case XK_emopenrectangle:
			return 0x25AF;
		case XK_leftsinglequotemark:
			return 0x2018;
		case XK_rightsinglequotemark:
			return 0x2019;
		case XK_leftdoublequotemark:
			return 0x201C;
		case XK_rightdoublequotemark:
			return 0x201D;
		case XK_prescription:
			return 0x211E;
		case XK_minutes:
			return 0x2032;
		case XK_seconds:
			return 0x2033;
		case XK_latincross:
			return 0x271D;
		case XK_filledrectbullet:
			return 0x25AC;
		case XK_filledlefttribullet:
			return 0x25C0;
		case XK_filledrighttribullet:
			return 0x25B6;
		case XK_emfilledcircle:
			return 0x25CF;
		case XK_emfilledrect:
			return 0x25AE;
;		case XK_enopencircbullet:
			return 0x25E6;
		case XK_enopensquarebullet:
			return 0x25AB;
		case XK_openrectbullet:
			return 0x25AD;
		case XK_opentribulletup:
			return 0x25B3;
		case XK_opentribulletdown:
			return 0x25BD;
		case XK_openstar:
			return 0x2606;
		case XK_enfilledcircbullet:
			return 0x2022;
		case XK_enfilledsqbullet:
			return 0x25AA;
		case XK_filledtribulletup:
			return 0x25B2;
		case XK_filledtribulletdown:
			return 0x25BC;
		case XK_leftpointer:
			return 0x261C;
		case XK_rightpointer:
			return 0x261E;
		case XK_club:
			return 0x2663;
		case XK_diamond:
			return 0x2666;
		case XK_heart:
			return 0x2665;
		case XK_maltesecross:
			return 0x2720;
		case XK_dagger:
			return 0x2020;
		case XK_doubledagger:
			return 0x2021;
		case XK_checkmark:
			return 0x2713;
		case XK_ballotcross:
			return 0x2717;
		case XK_musicalsharp:
			return 0x266F;
		case XK_musicalflat:
			return 0x266D;
		case XK_malesymbol:
			return 0x2642;
		case XK_femalesymbol:
			return 0x2640;
		case XK_telephone:
			return 0x260E;
		case XK_telephonerecorder:
			return 0x2315;
		case XK_phonographcopyright:
			return 0x2117;
		case XK_caret:
			return 0x2038;
		case XK_singlelowquotemark:
			return 0x201A;
		case XK_doublelowquotemark:
			return 0x201E;
		#endif


		#ifdef XK_APL
		case XK_leftcaret:
			return 0x003C;
		case XK_rightcaret:
			return 0x003E;
		case XK_downcaret:
			return 0x2228;
		case XK_upcaret:
			return 0x2227;
		case XK_overbar:
			return 0xAF;
		case XK_downtack:
			return 0x22A5;
		case XK_upshoe:
			return 0x2229;
		case XK_downstile:
			return 0x230A;
		case XK_underbar:
			return 0x005F;
		case XK_jot:
			return 0x2218;
		case XK_quad:
			return 0x2395;
		case XK_uptack:
			return 0x22A4;
		case XK_circle:
			return 0x25CB;
		case XK_upstile:
			return 0x2308;
		case XK_downshoe:
			return 0x222A;
		case XK_rightshoe:
			return 0x2283;
		case XK_leftshoe:
			return 0x2282;
		case XK_lefttack:
			return 0x22A2;
		case XK_righttack:
			return 0x22A3;
		#endif


		#ifdef XK_HEBREW
		case XK_hebrew_doublelowline:
			return 0x2017;
		case XK_hebrew_aleph:
			return 0x05D0;
		case XK_hebrew_bet:
			return 0x05D1;
		case XK_hebrew_gimel:
			return 0x05D2;
		case XK_hebrew_dalet:
			return 0x05D3;
		case XK_hebrew_he:
			return 0x05D4;
		case XK_hebrew_waw:
			return 0x05D5;
		case XK_hebrew_zain:
			return 0x05D6;
		case XK_hebrew_chet:
			return 0x05D7;
		case XK_hebrew_tet:
			return 0x05D8;
		case XK_hebrew_yod:
			return 0x05D9;
		case XK_hebrew_finalkaph:
			return 0x05DA;
		case XK_hebrew_kaph:
			return 0x05DB;
		case XK_hebrew_lamed:
			return 0x05DC;
		case XK_hebrew_finalmem:
			return 0x05DD;
		case XK_hebrew_mem:
			return 0x05DE;
		case XK_hebrew_finalnun:
			return 0x05DF;
		case XK_hebrew_nun:
			return 0x05E0;
		case XK_hebrew_samech:
			return 0x05E1;
		case XK_hebrew_ayin:
			return 0x05E2;
		case XK_hebrew_finalpe:
			return 0x05E3;
		case XK_hebrew_pe:
			return 0x05E4;
		case XK_hebrew_finalzade:
			return 0x05E5;
		case XK_hebrew_zade:
			return 0x05E6;
		case XK_hebrew_qoph:
			return 0x05E7;
		case XK_hebrew_resh:
			return 0x05E8;
		case XK_hebrew_shin:
			return 0x05E9;
		case XK_hebrew_taw:
			return 0x05EA;
		#endif


		#ifdef XK_THAI
		case XK_Thai_kokai:
			return 0x0E01;
		case XK_Thai_khokhai:
			return 0x0E02;
		case XK_Thai_khokhuat:
			return 0x0E03;
		case XK_Thai_khokhwai:
			return 0x0E04;
		case XK_Thai_khokhon:
			return 0x0E05;
		case XK_Thai_khorakhang:
			return 0x0E06;
		case XK_Thai_ngongu:
			return 0x0E07;
		case XK_Thai_chochan:
			return 0x0E08;
		case XK_Thai_choching:
			return 0x0E09;
		case XK_Thai_chochang:
			return 0x0E0A;
		case XK_Thai_soso:
			return 0x0E0B;
		case XK_Thai_chochoe:
			return 0x0E0C;
		case XK_Thai_yoying:
			return 0x0E0D;
		case XK_Thai_dochada:
			return 0x0E0E;
		case XK_Thai_topatak:
			return 0x0E0F;
		case XK_Thai_thothan:
			return 0x0E10;
		case XK_Thai_thonangmontho:
			return 0x0E11;
		case XK_Thai_thophuthao:
			return 0x0E12;
		case XK_Thai_nonen:
			return 0x0E13;
		case XK_Thai_dodek:
			return 0x0E14;
		case XK_Thai_totao:
			return 0x0E15;
		case XK_Thai_thothung:
			return 0x0E16;
		case XK_Thai_thothahan:
			return 0x0E17;
		case XK_Thai_thothong:
			return 0x0E18;
		case XK_Thai_nonu:
			return 0x0E19;
		case XK_Thai_bobaimai:
			return 0x0E1A;
		case XK_Thai_popla:
			return 0x0E1B;
		case XK_Thai_phophung:
			return 0x0E1C;
		case XK_Thai_fofa:
			return 0x0E1D;
		case XK_Thai_phophan:
			return 0x0E1E;
		case XK_Thai_fofan:
			return 0x0E1F;
		case XK_Thai_phosamphao:
			return 0x0E20;
		case XK_Thai_moma:
			return 0x0E21;
		case XK_Thai_yoyak:
			return 0x0E22;
		case XK_Thai_rorua:
			return 0x0E23;
		case XK_Thai_ru:
			return 0x0E24;
		case XK_Thai_loling:
			return 0x0E25;
		case XK_Thai_lu:
			return 0x0E26;
		case XK_Thai_wowaen:
			return 0x0E27;
		case XK_Thai_sosala:
			return 0x0E28;
		case XK_Thai_sorusi:
			return 0x0E29;
		case XK_Thai_sosua:
			return 0x0E2A;
		case XK_Thai_hohip:
			return 0x0E2B;
		case XK_Thai_lochula:
			return 0x0E2C;
		case XK_Thai_oang:
			return 0x0E2D;
		case XK_Thai_honokhuk:
			return 0x0E2E;
		case XK_Thai_paiyannoi:
			return 0x0E2F;
		case XK_Thai_saraa:
			return 0x0E30;
		case XK_Thai_maihanakat:
			return 0x0E31;
		case XK_Thai_saraaa:
			return 0x0E32;
		case XK_Thai_saraam:
			return 0x0E33;
		case XK_Thai_sarai:
			return 0x0E34;
		case XK_Thai_saraii:
			return 0x0E35;
		case XK_Thai_saraue:
			return 0x0E36;
		case XK_Thai_sarauee:
			return 0x0E37;
		case XK_Thai_sarau:
			return 0x0E38;
		case XK_Thai_sarauu:
			return 0x0E39;
		case XK_Thai_phinthu:
			return 0x0E3A;
		case XK_Thai_baht:
			return 0x0E3F;
		case XK_Thai_sarae:
			return 0x0E40;
		case XK_Thai_saraae:
			return 0x0E41;
		case XK_Thai_sarao:
			return 0x0E42;
		case XK_Thai_saraaimaimuan:
			return 0x0E43;
		case XK_Thai_saraaimaimalai:
			return 0x0E44;
		case XK_Thai_lakkhangyao:
			return 0x0E45;
		case XK_Thai_maiyamok:
			return 0x0E46;
		case XK_Thai_maitaikhu:
			return 0x0E47;
		case XK_Thai_maiek:
			return 0x0E48;
		case XK_Thai_maitho:
			return 0x0E49;
		case XK_Thai_maitri:
			return 0x0E4A;
		case XK_Thai_maichattawa:
			return 0x0E4B;
		case XK_Thai_thanthakhat:
			return 0x0E4C;
		case XK_Thai_nikhahit:
			return 0x0E4D;
		case XK_Thai_leksun:
			return 0x0E50;
		case XK_Thai_leknung:
			return 0x0E51;
		case XK_Thai_leksong:
			return 0x0E52;
		case XK_Thai_leksam:
			return 0x0E53;
		case XK_Thai_leksi:
			return 0x0E54;
		case XK_Thai_lekha:
			return 0x0E55;
		case XK_Thai_lekhok:
			return 0x0E56;
		case XK_Thai_lekchet:
			return 0x0E57;
		case XK_Thai_lekpaet:
			return 0x0E58;
		case XK_Thai_lekkao:
			return 0x0E59;
		#endif


		#ifdef XK_KOREAN
		case XK_Korean_Won:
			return 0x20A9;
		#endif


		#ifdef XK_ARMENIAN
		case XK_Armenian_ligature_ew:
			return 0x0587;
		#ifdef XK_Armenian_full_stop
		case XK_Armenian_full_stop:
		#else
		case XK_Armenian_verjaket:
		#endif
			return 0x0589;
		#ifdef XK_Armenian_separation_mark
		case XK_Armenian_separation_mark:
		#else
		case XK_Armenian_but:
		#endif
			return 0x055D;
		#ifdef XK_Armenian_hyphen
		case XK_Armenian_hyphen:
		#else
		case XK_Armenian_yentamna:
		#endif
			return 0x058A;
		#ifdef XK_Armenian_exclam
		case XK_Armenian_exclam:
		#else
		case XK_Armenian_amanak:
		#endif
			return 0x055C;
		#ifdef XK_Armenian_accent
		case XK_Armenian_accent:
		#else
		case XK_Armenian_shesht:
		#endif
			return 0x055B;
		#ifdef XK_Armenian_question
		case XK_Armenian_question:
		#else
		case XK_Armenian_paruyk:
		#endif
			return 0x055E;
		case XK_Armenian_AYB:
			return 0x0531;
		case XK_Armenian_ayb:
			return 0x0561;
		case XK_Armenian_BEN:
			return 0x0532;
		case XK_Armenian_ben:
			return 0x0562;
		case XK_Armenian_GIM:
			return 0x0533;
		case XK_Armenian_gim:
			return 0x0563;
		case XK_Armenian_DA:
			return 0x0534;
		case XK_Armenian_da:
			return 0x0564;
		case XK_Armenian_YECH:
			return 0x0535;
		case XK_Armenian_yech:
			return 0x0565;
		case XK_Armenian_ZA:
			return 0x0536;
		case XK_Armenian_za:
			return 0x0566;
		case XK_Armenian_E:
			return 0x0537;
		case XK_Armenian_e:
			return 0x0567;
		case XK_Armenian_AT:
			return 0x0538;
		case XK_Armenian_at:
			return 0x0568;
		case XK_Armenian_TO:
			return 0x0539;
		case XK_Armenian_to:
			return 0x0569;
		case XK_Armenian_ZHE:
			return 0x053A;
		case XK_Armenian_zhe:
			return 0x056A;
		case XK_Armenian_INI:
			return 0x053B;
		case XK_Armenian_ini:
			return 0x056B;
		case XK_Armenian_LYUN:
			return 0x053C;
		case XK_Armenian_lyun:
			return 0x056C;
		case XK_Armenian_KHE:
			return 0x053D;
		case XK_Armenian_khe:
			return 0x056D;
		case XK_Armenian_TSA:
			return 0x053E;
		case XK_Armenian_tsa:
			return 0x056E;
		case XK_Armenian_KEN:
			return 0x053F;
		case XK_Armenian_ken:
			return 0x056F;
		case XK_Armenian_HO:
			return 0x0540;
		case XK_Armenian_ho:
			return 0x0570;
		case XK_Armenian_DZA:
			return 0x0541;
		case XK_Armenian_dza:
			return 0x0571;
		case XK_Armenian_GHAT:
			return 0x0542;
		case XK_Armenian_ghat:
			return 0x0572;
		case XK_Armenian_TCHE:
			return 0x0543;
		case XK_Armenian_tche:
			return 0x0573;
		case XK_Armenian_MEN:
			return 0x0544;
		case XK_Armenian_men:
			return 0x0574;
		case XK_Armenian_HI:
			return 0x0545;
		case XK_Armenian_hi:
			return 0x0575;
		case XK_Armenian_NU:
			return 0x0546;
		case XK_Armenian_nu:
			return 0x0576;
		case XK_Armenian_SHA:
			return 0x0547;
		case XK_Armenian_sha:
			return 0x0577;
		case XK_Armenian_VO:
			return 0x0548;
		case XK_Armenian_vo:
			return 0x0578;
		case XK_Armenian_CHA:
			return 0x0549;
		case XK_Armenian_cha:
			return 0x0579;
		case XK_Armenian_PE:
			return 0x054A;
		case XK_Armenian_pe:
			return 0x057A;
		case XK_Armenian_JE:
			return 0x054B;
		case XK_Armenian_je:
			return 0x057B;
		case XK_Armenian_RA:
			return 0x054C;
		case XK_Armenian_ra:
			return 0x057C;
		case XK_Armenian_SE:
			return 0x054D;
		case XK_Armenian_se:
			return 0x057D;
		case XK_Armenian_VEV:
			return 0x054E;
		case XK_Armenian_vev:
			return 0x057E;
		case XK_Armenian_TYUN:
			return 0x054F;
		case XK_Armenian_tyun:
			return 0x057F;
		case XK_Armenian_RE:
			return 0x0550;
		case XK_Armenian_re:
			return 0x0580;
		case XK_Armenian_TSO:
			return 0x0551;
		case XK_Armenian_tso:
			return 0x0581;
		case XK_Armenian_VYUN:
			return 0x0552;
		case XK_Armenian_vyun:
			return 0x0582;
		case XK_Armenian_PYUR:
			return 0x0553;
		case XK_Armenian_pyur:
			return 0x0583;
		case XK_Armenian_KE:
			return 0x0554;
		case XK_Armenian_ke:
			return 0x0584;
		case XK_Armenian_O:
			return 0x0555;
		case XK_Armenian_o:
			return 0x0585;
		case XK_Armenian_FE:
			return 0x0556;
		case XK_Armenian_fe:
			return 0x0586;
		case XK_Armenian_apostrophe:
			return 0x055A;
		#endif


		#ifdef XK_GEORGIAN
		case XK_Georgian_an:
			return 0x10D0;
		case XK_Georgian_ban:
			return 0x10D1;
		case XK_Georgian_gan:
			return 0x10D2;
		case XK_Georgian_don:
			return 0x10D3;
		case XK_Georgian_en:
			return 0x10D4;
		case XK_Georgian_vin:
			return 0x10D5;
		case XK_Georgian_zen:
			return 0x10D6;
		case XK_Georgian_tan:
			return 0x10D7;
		case XK_Georgian_in:
			return 0x10D8;
		case XK_Georgian_kan:
			return 0x10D9;
		case XK_Georgian_las:
			return 0x10DA;
		case XK_Georgian_man:
			return 0x10DB;
		case XK_Georgian_nar:
			return 0x10DC;
		case XK_Georgian_on:
			return 0x10DD;
		case XK_Georgian_par:
			return 0x10DE;
		case XK_Georgian_zhar:
			return 0x10DF;
		case XK_Georgian_rae:
			return 0x10E0;
		case XK_Georgian_san:
			return 0x10E1;
		case XK_Georgian_tar:
			return 0x10E2;
		case XK_Georgian_un:
			return 0x10E3;
		case XK_Georgian_phar:
			return 0x10E4;
		case XK_Georgian_khar:
			return 0x10E5;
		case XK_Georgian_ghan:
			return 0x10E6;
		case XK_Georgian_qar:
			return 0x10E7;
		case XK_Georgian_shin:
			return 0x10E8;
		case XK_Georgian_chin:
			return 0x10E9;
		case XK_Georgian_can:
			return 0x10EA;
		case XK_Georgian_jil:
			return 0x10EB;
		case XK_Georgian_cil:
			return 0x10EC;
		case XK_Georgian_char:
			return 0x10ED;
		case XK_Georgian_xan:
			return 0x10EE;
		case XK_Georgian_jhan:
			return 0x10EF;
		case XK_Georgian_hae:
			return 0x10F0;
		case XK_Georgian_he:
			return 0x10F1;
		case XK_Georgian_hie:
			return 0x10F2;
		case XK_Georgian_we:
			return 0x10F3;
		case XK_Georgian_har:
			return 0x10F4;
		case XK_Georgian_hoe:
			return 0x10F5;
		case XK_Georgian_fi:
			return 0x10F6;
		#endif


		#ifdef XK_CAUCASUS
		case XK_Xabovedot:
			return 0x1E8A;
		case XK_Ibreve:
			return 0x012C;
		case XK_Zstroke:
			return 0x01B5;
		case XK_Gcaron:
			return 0x01E6;
		case XK_Ocaron:
			return 0x01D2;
		case XK_Obarred:
			return 0x019F;
		case XK_xabovedot:
			return 0x1E8B;
		case XK_ibreve:
			return 0x012D;
		case XK_zstroke:
			return 0x01B6;
		case XK_gcaron:
			return 0x01E7;
		case XK_ocaron:
			return 0x01D2;
		case XK_obarred:
			return 0x0275;
		case XK_SCHWA:
			return 0x018F;
		case XK_schwa:
			return 0x0259;
		case XK_Lbelowdot:
			return 0x1E36;
		case XK_lbelowdot:
			return 0x1E37;
		#endif


		#ifdef XK_VIETNAMESE
		case XK_Abelowdot:
			return 0x1EA0;
		case XK_abelowdot:
			return 0x1EA1;
		case XK_Ahook:
			return 0x1EA2;
		case XK_ahook:
			return 0x1EA3;
		case XK_Acircumflexacute:
			return 0x1EA4;
		case XK_acircumflexacute:
			return 0x1EA5;
		case XK_Acircumflexgrave:
			return 0x1EA6;
		case XK_acircumflexgrave:
			return 0x1EA7;
		case XK_Acircumflexhook:
			return 0x1EA8;
		case XK_acircumflexhook:
			return 0x1EA9;
		case XK_Acircumflextilde:
			return 0x1EAA;
		case XK_acircumflextilde:
			return 0x1EAB;
		case XK_Acircumflexbelowdot:
			return 0x1EAC;
		case XK_acircumflexbelowdot:
			return 0x1EAD;
		case XK_Abreveacute:
			return 0x1EAE;
		case XK_abreveacute:
			return 0x1EAF;
		case XK_Abrevegrave:
			return 0x1EB0;
		case XK_abrevegrave:
			return 0x1EB1;
		case XK_Abrevehook:
			return 0x1EB2;
		case XK_abrevehook:
			return 0x1EB3;
		case XK_Abrevetilde:
			return 0x1EB4;
		case XK_abrevetilde:
			return 0x1EB5;
		case XK_Abrevebelowdot:
			return 0x1EB6;
		case XK_abrevebelowdot:
			return 0x1EB7;
		case XK_Ebelowdot:
			return 0x1EB8;
		case XK_ebelowdot:
			return 0x1EB9;
		case XK_Ehook:
			return 0x1EBA;
		case XK_ehook:
			return 0x1EBB;
		case XK_Etilde:
			return 0x1EBC;
		case XK_etilde:
			return 0x1EBD;
		case XK_Ecircumflexacute:
			return 0x1EBE;
		case XK_ecircumflexacute:
			return 0x1EBF;
		case XK_Ecircumflexgrave:
			return 0x1EC0;
		case XK_ecircumflexgrave:
			return 0x1EC1;
		case XK_Ecircumflexhook:
			return 0x1EC2;
		case XK_ecircumflexhook:
			return 0x1EC3;
		case XK_Ecircumflextilde:
			return 0x1EC4;
		case XK_ecircumflextilde:
			return 0x1EC5;
		case XK_Ecircumflexbelowdot:
			return 0x1EC6;
		case XK_ecircumflexbelowdot:
			return 0x1EC7;
		case XK_Ihook:
			return 0x1EC8;
		case XK_ihook:
			return 0x1EC9;
		case XK_Ibelowdot:
			return 0x1ECA;
		case XK_ibelowdot:
			return 0x1ECB;
		case XK_Obelowdot:
			return 0x1ECC;
		case XK_obelowdot:
			return 0x1ECD;
		case XK_Ohook:
			return 0x1ECE;
		case XK_ohook:
			return 0x1ECF;
		case XK_Ocircumflexacute:
			return 0x1ED0;
		case XK_ocircumflexacute:
			return 0x1ED1;
		case XK_Ocircumflexgrave:
			return 0x1ED2;
		case XK_ocircumflexgrave:
			return 0x1ED3;
		case XK_Ocircumflexhook:
			return 0x1ED4;
		case XK_ocircumflexhook:
			return 0x1ED5;
		case XK_Ocircumflextilde:
			return 0x1ED6;
		case XK_ocircumflextilde:
			return 0x1ED7;
		case XK_Ocircumflexbelowdot:
			return 0x1ED8;
		case XK_ocircumflexbelowdot:
			return 0x1ED9;
		case XK_Ohornacute:
			return 0x1EDA;
		case XK_ohornacute:
			return 0x1EDB;
		case XK_Ohorngrave:
			return 0x1EDC;
		case XK_ohorngrave:
			return 0x1EDD;
		case XK_Ohornhook:
			return 0x1EDE;
		case XK_ohornhook:
			return 0x1EDF;
		case XK_Ohorntilde:
			return 0x1EE0;
		case XK_ohorntilde:
			return 0x1EE1;
		case XK_Ohornbelowdot:
			return 0x1EE2;
		case XK_ohornbelowdot:
			return 0x1EE3;
		case XK_Ubelowdot:
			return 0x1EE4;
		case XK_ubelowdot:
			return 0x1EE5;
		case XK_Uhook:
			return 0x1EE6;
		case XK_uhook:
			return 0x1EE7;
		case XK_Uhornacute:
			return 0x1EE8;
		case XK_uhornacute:
			return 0x1EE9;
		case XK_Uhorngrave:
			return 0x1EEA;
		case XK_uhorngrave:
			return 0x1EEB;
		case XK_Uhornhook:
			return 0x1EEC;
		case XK_uhornhook:
			return 0x1EED;
		case XK_Uhorntilde:
			return 0x1EEE;
		case XK_uhorntilde:
			return 0x1EEF;
		case XK_Uhornbelowdot:
			return 0x1EF0;
		case XK_uhornbelowdot:
			return 0x1EF1;
		case XK_Ybelowdot:
			return 0x1EF4;
		case XK_ybelowdot:
			return 0x1EF5;
		case XK_Yhook:
			return 0x1EF6;
		case XK_yhook:
			return 0x1EF7;
		case XK_Ytilde:
			return 0x1EF8;
		case XK_ytilde:
			return 0x1EF9;
		case XK_Ohorn:
			return 0x01A0;
		case XK_ohorn:
			return 0x01A1;
		case XK_Uhorn:
			return 0x01AF;
		case XK_uhorn:
			return 0x01B0;
		#endif


		#ifdef XK_CURRENCY
		case XK_EcuSign:
			return 0x20A0;
		case XK_ColonSign:
			return 0x20A1;
		case XK_CruzeiroSign:
			return 0x20A2;
		case XK_FFrancSign:
			return 0x20A3;
		case XK_LiraSign:
			return 0x20A4;
		case XK_MillSign:
			return 0x20A5;
		case XK_NairaSign:
			return 0x20A6;
		case XK_PesetaSign:
			return 0x20A7;
		case XK_RupeeSign:
			return 0x20A8;
		case XK_WonSign:
			return 0x20A9;
		case XK_NewSheqelSign:
			return 0x20AA;
		case XK_DongSign:
			return 0x20AB;
		case XK_EuroSign:
			return 0x20AC;
		#endif


		#ifdef XK_MATHEMATICAL
		case XK_zerosuperior:
			return 0x2070;
		case XK_foursuperior:
			return 0x2074;
		case XK_fivesuperior:
			return 0x2075;
		case XK_sixsuperior:
			return 0x2076;
		case XK_sevensuperior:
			return 0x2077;
		case XK_eightsuperior:
			return 0x2078;
		case XK_ninesuperior:
			return 0x2079;
		case XK_zerosubscript:
			return 0x2080;
		case XK_onesubscript:
			return 0x2081;
		case XK_twosubscript:
			return 0x2082;
		case XK_threesubscript:
			return 0x2083;
		case XK_foursubscript:
			return 0x2084;
		case XK_fivesubscript:
			return 0x2085;
		case XK_sixsubscript:
			return 0x2086;
		case XK_sevensubscript:
			return 0x2087;
		case XK_eightsubscript:
			return 0x2088;
		case XK_ninesubscript:
			return 0x2089;
		case XK_partdifferential:
			return 0x2202;
		case XK_emptyset:
			return 0x2205;
		case XK_elementof:
			return 0x2208;
		case XK_notelementof:
			return 0x2209;
		case XK_containsas:
			return 0x220B;
		case XK_squareroot:
			return 0x221A;
		case XK_cuberoot:
			return 0x221B;
		case XK_fourthroot:
			return 0x221C;
		case XK_dintegral:
			return 0x222C;
		case XK_tintegral:
			return 0x222D;
		case XK_because:
			return 0x2235;
		case XK_approxeq:
			return 0x2245;
		case XK_notapproxeq:
			return 0x2247;
		case XK_notidentical:
			return 0x2262;
		case XK_stricteq:
			return 0x2263;
		#endif


		#ifdef XK_BRAILLE
		case XK_braille_blank:
			return 0x2800;
		case XK_braille_dots_1:
			return 0x2801;
		case XK_braille_dots_2:
			return 0x2802;
		case XK_braille_dots_12:
			return 0x2803;
		case XK_braille_dots_3:
			return 0x2804;
		case XK_braille_dots_13:
			return 0x2805;
		case XK_braille_dots_23:
			return 0x2806;
		case XK_braille_dots_123:
			return 0x2807;
		case XK_braille_dots_4:
			return 0x2808;
		case XK_braille_dots_14:
			return 0x2809;
		case XK_braille_dots_24:
			return 0x280A;
		case XK_braille_dots_124:
			return 0x280B;
		case XK_braille_dots_34:
			return 0x280C;
		case XK_braille_dots_134:
			return 0x280D;
		case XK_braille_dots_234:
			return 0x280E;
		case XK_braille_dots_1234:
			return 0x280F;
		case XK_braille_dots_5:
			return 0x2810;
		case XK_braille_dots_15:
			return 0x2811;
		case XK_braille_dots_25:
			return 0x2812;
		case XK_braille_dots_125:
			return 0x2813;
		case XK_braille_dots_35:
			return 0x2814;
		case XK_braille_dots_135:
			return 0x2815;
		case XK_braille_dots_235:
			return 0x2816;
		case XK_braille_dots_1235:
			return 0x2817;
		case XK_braille_dots_45:
			return 0x2818;
		case XK_braille_dots_145:
			return 0x2819;
		case XK_braille_dots_245:
			return 0x281A;
		case XK_braille_dots_1245:
			return 0x281B;
		case XK_braille_dots_345:
			return 0x281C;
		case XK_braille_dots_1345:
			return 0x281D;
		case XK_braille_dots_2345:
			return 0x281E;
		case XK_braille_dots_12345:
			return 0x281F;
		case XK_braille_dots_6:
			return 0x2820;
		case XK_braille_dots_16:
			return 0x2821;
		case XK_braille_dots_26:
			return 0x2822;
		case XK_braille_dots_126:
			return 0x2823;
		case XK_braille_dots_36:
			return 0x2824;
		case XK_braille_dots_136:
			return 0x2825;
		case XK_braille_dots_236:
			return 0x2826;
		case XK_braille_dots_1236:
			return 0x2827;
		case XK_braille_dots_46:
			return 0x2828;
		case XK_braille_dots_146:
			return 0x2829;
		case XK_braille_dots_246:
			return 0x282A;
		case XK_braille_dots_1246:
			return 0x282B;
		case XK_braille_dots_346:
			return 0x282C;
		case XK_braille_dots_1346:
			return 0x282D;
		case XK_braille_dots_2346:
			return 0x282E;
		case XK_braille_dots_12346:
			return 0x282F;
		case XK_braille_dots_56:
			return 0x2830;
		case XK_braille_dots_156:
			return 0x2831;
		case XK_braille_dots_256:
			return 0x2832;
		case XK_braille_dots_1256:
			return 0x2833;
		case XK_braille_dots_356:
			return 0x2834;
		case XK_braille_dots_1356:
			return 0x2835;
		case XK_braille_dots_2356:
			return 0x2836;
		case XK_braille_dots_12356:
			return 0x2837;
		case XK_braille_dots_456:
			return 0x2838;
		case XK_braille_dots_1456:
			return 0x2839;
		case XK_braille_dots_2456:
			return 0x283A;
		case XK_braille_dots_12456:
			return 0x283B;
		case XK_braille_dots_3456:
			return 0x283C;
		case XK_braille_dots_13456:
			return 0x283D;
		case XK_braille_dots_23456:
			return 0x283E;
		case XK_braille_dots_123456:
			return 0x283F;
		case XK_braille_dots_7:
			return 0x2840;
		case XK_braille_dots_17:
			return 0x2841;
		case XK_braille_dots_27:
			return 0x2842;
		case XK_braille_dots_127:
			return 0x2843;
		case XK_braille_dots_37:
			return 0x2844;
		case XK_braille_dots_137:
			return 0x2845;
		case XK_braille_dots_237:
			return 0x2846;
		case XK_braille_dots_1237:
			return 0x2847;
		case XK_braille_dots_47:
			return 0x2848;
		case XK_braille_dots_147:
			return 0x2849;
		case XK_braille_dots_247:
			return 0x284A;
		case XK_braille_dots_1247:
			return 0x284B;
		case XK_braille_dots_347:
			return 0x284C;
		case XK_braille_dots_1347:
			return 0x284D;
		case XK_braille_dots_2347:
			return 0x284E;
		case XK_braille_dots_12347:
			return 0x284F;
		case XK_braille_dots_57:
			return 0x2850;
		case XK_braille_dots_157:
			return 0x2851;
		case XK_braille_dots_257:
			return 0x2852;
		case XK_braille_dots_1257:
			return 0x2853;
		case XK_braille_dots_357:
			return 0x2854;
		case XK_braille_dots_1357:
			return 0x2855;
		case XK_braille_dots_2357:
			return 0x2856;
		case XK_braille_dots_12357:
			return 0x2857;
		case XK_braille_dots_457:
			return 0x2858;
		case XK_braille_dots_1457:
			return 0x2859;
		case XK_braille_dots_2457:
			return 0x285A;
		case XK_braille_dots_12457:
			return 0x285B;
		case XK_braille_dots_3457:
			return 0x285C;
		case XK_braille_dots_13457:
			return 0x285D;
		case XK_braille_dots_23457:
			return 0x285E;
		case XK_braille_dots_123457:
			return 0x285F;
		case XK_braille_dots_67:
			return 0x2860;
		case XK_braille_dots_167:
			return 0x2861;
		case XK_braille_dots_267:
			return 0x2862;
		case XK_braille_dots_1267:
			return 0x2863;
		case XK_braille_dots_367:
			return 0x2864;
		case XK_braille_dots_1367:
			return 0x2865;
		case XK_braille_dots_2367:
			return 0x2866;
		case XK_braille_dots_12367:
			return 0x2867;
		case XK_braille_dots_467:
			return 0x2868;
		case XK_braille_dots_1467:
			return 0x2869;
		case XK_braille_dots_2467:
			return 0x286A;
		case XK_braille_dots_12467:
			return 0x286B;
		case XK_braille_dots_3467:
			return 0x286C;
		case XK_braille_dots_13467:
			return 0x286D;
		case XK_braille_dots_23467:
			return 0x286E;
		case XK_braille_dots_123467:
			return 0x286F;
		case XK_braille_dots_567:
			return 0x2870;
		case XK_braille_dots_1567:
			return 0x2871;
		case XK_braille_dots_2567:
			return 0x2872;
		case XK_braille_dots_12567:
			return 0x2873;
		case XK_braille_dots_3567:
			return 0x2874;
		case XK_braille_dots_13567:
			return 0x2875;
		case XK_braille_dots_23567:
			return 0x2876;
		case XK_braille_dots_123567:
			return 0x2877;
		case XK_braille_dots_4567:
			return 0x2878;
		case XK_braille_dots_14567:
			return 0x2879;
		case XK_braille_dots_24567:
			return 0x287A;
		case XK_braille_dots_124567:
			return 0x287B;
		case XK_braille_dots_34567:
			return 0x287C;
		case XK_braille_dots_134567:
			return 0x287D;
		case XK_braille_dots_234567:
			return 0x287E;
		case XK_braille_dots_1234567:
			return 0x287F;
		case XK_braille_dots_8:
			return 0x2880;
		case XK_braille_dots_18:
			return 0x2881;
		case XK_braille_dots_28:
			return 0x2882;
		case XK_braille_dots_128:
			return 0x2883;
		case XK_braille_dots_38:
			return 0x2884;
		case XK_braille_dots_138:
			return 0x2885;
		case XK_braille_dots_238:
			return 0x2886;
		case XK_braille_dots_1238:
			return 0x2887;
		case XK_braille_dots_48:
			return 0x2888;
		case XK_braille_dots_148:
			return 0x2889;
		case XK_braille_dots_248:
			return 0x288A;
		case XK_braille_dots_1248:
			return 0x288B;
		case XK_braille_dots_348:
			return 0x288C;
		case XK_braille_dots_1348:
			return 0x288D;
		case XK_braille_dots_2348:
			return 0x288E;
		case XK_braille_dots_12348:
			return 0x288F;
		case XK_braille_dots_58:
			return 0x2890;
		case XK_braille_dots_158:
			return 0x2891;
		case XK_braille_dots_258:
			return 0x2892;
		case XK_braille_dots_1258:
			return 0x2893;
		case XK_braille_dots_358:
			return 0x2894;
		case XK_braille_dots_1358:
			return 0x2895;
		case XK_braille_dots_2358:
			return 0x2896;
		case XK_braille_dots_12358:
			return 0x2897;
		case XK_braille_dots_458:
			return 0x2898;
		case XK_braille_dots_1458:
			return 0x2899;
		case XK_braille_dots_2458:
			return 0x289A;
		case XK_braille_dots_12458:
			return 0x289B;
		case XK_braille_dots_3458:
			return 0x289C;
		case XK_braille_dots_13458:
			return 0x289D;
		case XK_braille_dots_23458:
			return 0x289E;
		case XK_braille_dots_123458:
			return 0x289F;
		case XK_braille_dots_68:
			return 0x28A0;
		case XK_braille_dots_168:
			return 0x28A1;
		case XK_braille_dots_268:
			return 0x28A2;
		case XK_braille_dots_1268:
			return 0x28A3;
		case XK_braille_dots_368:
			return 0x28A4;
		case XK_braille_dots_1368:
			return 0x28A5;
		case XK_braille_dots_2368:
			return 0x28A6;
		case XK_braille_dots_12368:
			return 0x28A7;
		case XK_braille_dots_468:
			return 0x28A8;
		case XK_braille_dots_1468:
			return 0x28A9;
		case XK_braille_dots_2468:
			return 0x28AA;
		case XK_braille_dots_12468:
			return 0x28AB;
		case XK_braille_dots_3468:
			return 0x28AC;
		case XK_braille_dots_13468:
			return 0x28AD;
		case XK_braille_dots_23468:
			return 0x28AE;
		case XK_braille_dots_123468:
			return 0x28AF;
		case XK_braille_dots_568:
			return 0x28B0;
		case XK_braille_dots_1568:
			return 0x28B1;
		case XK_braille_dots_2568:
			return 0x28B2;
		case XK_braille_dots_12568:
			return 0x28B3;
		case XK_braille_dots_3568:
			return 0x28B4;
		case XK_braille_dots_13568:
			return 0x28B5;
		case XK_braille_dots_23568:
			return 0x28B6;
		case XK_braille_dots_123568:
			return 0x28B7;
		case XK_braille_dots_4568:
			return 0x28B8;
		case XK_braille_dots_14568:
			return 0x28B9;
		case XK_braille_dots_24568:
			return 0x28BA;
		case XK_braille_dots_124568:
			return 0x28BB;
		case XK_braille_dots_34568:
			return 0x28BC;
		case XK_braille_dots_134568:
			return 0x28BD;
		case XK_braille_dots_234568:
			return 0x28BE;
		case XK_braille_dots_1234568:
			return 0x28BF;
		case XK_braille_dots_78:
			return 0x28C0;
		case XK_braille_dots_178:
			return 0x28C1;
		case XK_braille_dots_278:
			return 0x28C2;
		case XK_braille_dots_1278:
			return 0x28C3;
		case XK_braille_dots_378:
			return 0x28C4;
		case XK_braille_dots_1378:
			return 0x28C5;
		case XK_braille_dots_2378:
			return 0x28C6;
		case XK_braille_dots_12378:
			return 0x28C7;
		case XK_braille_dots_478:
			return 0x28C8;
		case XK_braille_dots_1478:
			return 0x28C9;
		case XK_braille_dots_2478:
			return 0x28CA;
		case XK_braille_dots_12478:
			return 0x28CB;
		case XK_braille_dots_3478:
			return 0x28CC;
		case XK_braille_dots_13478:
			return 0x28CD;
		case XK_braille_dots_23478:
			return 0x28CE;
		case XK_braille_dots_123478:
			return 0x28CF;
		case XK_braille_dots_578:
			return 0x28D0;
		case XK_braille_dots_1578:
			return 0x28D1;
		case XK_braille_dots_2578:
			return 0x28D2;
		case XK_braille_dots_12578:
			return 0x28D3;
		case XK_braille_dots_3578:
			return 0x28D4;
		case XK_braille_dots_13578:
			return 0x28D5;
		case XK_braille_dots_23578:
			return 0x28D6;
		case XK_braille_dots_123578:
			return 0x28D7;
		case XK_braille_dots_4578:
			return 0x28D8;
		case XK_braille_dots_14578:
			return 0x28D9;
		case XK_braille_dots_24578:
			return 0x28DA;
		case XK_braille_dots_124578:
			return 0x28DB;
		case XK_braille_dots_34578:
			return 0x28DC;
		case XK_braille_dots_134578:
			return 0x28DD;
		case XK_braille_dots_234578:
			return 0x28DE;
		case XK_braille_dots_1234578:
			return 0x28DF;
		case XK_braille_dots_678:
			return 0x28E0;
		case XK_braille_dots_1678:
			return 0x28E1;
		case XK_braille_dots_2678:
			return 0x28E2;
		case XK_braille_dots_12678:
			return 0x28E3;
		case XK_braille_dots_3678:
			return 0x28E4;
		case XK_braille_dots_13678:
			return 0x28E5;
		case XK_braille_dots_23678:
			return 0x28E6;
		case XK_braille_dots_123678:
			return 0x28E7;
		case XK_braille_dots_4678:
			return 0x28E8;
		case XK_braille_dots_14678:
			return 0x28E9;
		case XK_braille_dots_24678:
			return 0x28EA;
		case XK_braille_dots_124678:
			return 0x28EB;
		case XK_braille_dots_34678:
			return 0x28EC;
		case XK_braille_dots_134678:
			return 0x28ED;
		case XK_braille_dots_234678:
			return 0x28EE;
		case XK_braille_dots_1234678:
			return 0x28EF;
		case XK_braille_dots_5678:
			return 0x28F0;
		case XK_braille_dots_15678:
			return 0x28F1;
		case XK_braille_dots_25678:
			return 0x28F2;
		case XK_braille_dots_125678:
			return 0x28F3;
		case XK_braille_dots_35678:
			return 0x28F4;
		case XK_braille_dots_135678:
			return 0x28F5;
		case XK_braille_dots_235678:
			return 0x28F6;
		case XK_braille_dots_1235678:
			return 0x28F7;
		case XK_braille_dots_45678:
			return 0x28F8;
		case XK_braille_dots_145678:
			return 0x28F9;
		case XK_braille_dots_245678:
			return 0x28FA;
		case XK_braille_dots_1245678:
			return 0x28FB;
		case XK_braille_dots_345678:
			return 0x28FC;
		case XK_braille_dots_1345678:
			return 0x28FD;
		case XK_braille_dots_2345678:
			return 0x28FE;
		case XK_braille_dots_12345678:
			return 0x28FF;
		#endif


		#ifdef XK_SINHALA
		case XK_Sinh_ng:
			return 0x0D82;
		case XK_Sinh_h2:
			return 0x0D83;
		case XK_Sinh_a:
			return 0x0D85;
		case XK_Sinh_aa:
			return 0x0D86;
		case XK_Sinh_ae:
			return 0x0D87;
		case XK_Sinh_aee:
			return 0x0D88;
		case XK_Sinh_i:
			return 0x0D89;
		case XK_Sinh_ii:
			return 0x0D8A;
		case XK_Sinh_u:
			return 0x0D8B;
		case XK_Sinh_uu:
			return 0x0D8C;
		case XK_Sinh_ri:
			return 0x0D8D;
		case XK_Sinh_rii:
			return 0x0D8E;
		case XK_Sinh_lu:
			return 0x0D8F;
		case XK_Sinh_luu:
			return 0x0D90;
		case XK_Sinh_e:
			return 0x0D91;
		case XK_Sinh_ee:
			return 0x0D92;
		case XK_Sinh_ai:
			return 0x0D93;
		case XK_Sinh_o:
			return 0x0D94;
		case XK_Sinh_oo:
			return 0x0D95;
		case XK_Sinh_au:
			return 0x0D96;
		case XK_Sinh_ka:
			return 0x0D9A;
		case XK_Sinh_kha:
			return 0x0D9B;
		case XK_Sinh_ga:
			return 0x0D9C;
		case XK_Sinh_gha:
			return 0x0D9D;
		case XK_Sinh_ng2:
			return 0x0D9E;
		case XK_Sinh_nga:
			return 0x0D9F;
		case XK_Sinh_ca:
			return 0x0DA0;
		case XK_Sinh_cha:
			return 0x0DA1;
		case XK_Sinh_ja:
			return 0x0DA2;
		case XK_Sinh_jha:
			return 0x0DA3;
		case XK_Sinh_nya:
			return 0x0DA4;
		case XK_Sinh_jnya:
			return 0x0DA5;
		case XK_Sinh_nja:
			return 0x0DA6;
		case XK_Sinh_tta:
			return 0x0DA7;
		case XK_Sinh_ttha:
			return 0x0DA8;
		case XK_Sinh_dda:
			return 0x0DA9;
		case XK_Sinh_ddha:
			return 0x0DAA;
		case XK_Sinh_nna:
			return 0x0DAB;
		case XK_Sinh_ndda:
			return 0x0DAC;
		case XK_Sinh_tha:
			return 0x0DAD;
		case XK_Sinh_thha:
			return 0x0DAE;
		case XK_Sinh_dha:
			return 0x0DAF;
		case XK_Sinh_dhha:
			return 0x0DB0;
		case XK_Sinh_na:
			return 0x0DB1;
		case XK_Sinh_ndha:
			return 0x0DB3;
		case XK_Sinh_pa:
			return 0x0DB4;
		case XK_Sinh_pha:
			return 0x0DB5;
		case XK_Sinh_ba:
			return 0x0DB6;
		case XK_Sinh_bha:
			return 0x0DB7;
		case XK_Sinh_ma:
			return 0x0DB8;
		case XK_Sinh_mba:
			return 0x0DB9;
		case XK_Sinh_ya:
			return 0x0DBA;
		case XK_Sinh_ra:
			return 0x0DBB;
		case XK_Sinh_la:
			return 0x0DBD;
		case XK_Sinh_va:
			return 0x0DC0;
		case XK_Sinh_sha:
			return 0x0DC1;
		case XK_Sinh_ssha:
			return 0x0DC2;
		case XK_Sinh_sa:
			return 0x0DC3;
		case XK_Sinh_ha:
			return 0x0DC4;
		case XK_Sinh_lla:
			return 0x0DC5;
		case XK_Sinh_fa:
			return 0x0DC6;
		case XK_Sinh_al:
			return 0x0DCA;
		case XK_Sinh_aa2:
			return 0x0DCF;
		case XK_Sinh_ae2:
			return 0x0DD0;
		case XK_Sinh_aee2:
			return 0x0DD1;
		case XK_Sinh_i2:
			return 0x0DD2;
		case XK_Sinh_ii2:
			return 0x0DD3;
		case XK_Sinh_u2:
			return 0x0DD4;
		case XK_Sinh_uu2:
			return 0x0DD6;
		case XK_Sinh_ru2:
			return 0x0DD8;
		case XK_Sinh_e2:
			return 0x0DD9;
		case XK_Sinh_ee2:
			return 0x0DDA;
		case XK_Sinh_ai2:
			return 0x0DDB;
		case XK_Sinh_o2:
			return 0x0DDC;
		case XK_Sinh_oo2:
			return 0x0DDD;
		case XK_Sinh_au2:
			return 0x0DDE;
		case XK_Sinh_lu2:
			return 0x0DDF;
		case XK_Sinh_ruu2:
			return 0x0DF2;
		case XK_Sinh_luu2:
			return 0x0DF3;
		case XK_Sinh_kunddaliya:
			return 0x0DF4;
		#endif

		default:
			return 0x0000;
	}
}

void LoadInputHelper() {
	#ifdef XKB
	// Get the map.
	keyboard_map = XkbGetMap(disp, XkbAllClientInfoMask, XkbUseCoreKbd);
	#else
	int min_keycode, max_keycode;
	XDisplayKeycodes(disp, &min_keycode, &max_keycode);

	keyboard_map = XGetKeyboardMapping(disp, min_keycode, (max_keycode - min_keycode + 1), &keysyms_per_keycode);
	if (keyboard_map) {
		XModifierKeymap *modifier_map = XGetModifierMapping(disp);

		if (modifier_map) {
			/* The Lock modifier is interpreted as CapsLock when the KeySym
			 * named XK_Caps_Lock is attached to some KeyCode and that KeyCode
			 * is attached to the Lock modifier. The Lock modifier is
			 * interpreted as ShiftLock when the KeySym named XK_Shift_Lock is
			 * attached to some KeyCode and that KeyCode is attached to the Lock
			 * modifier. If the Lock modifier could be interpreted as both
			 * CapsLock and ShiftLock, the CapsLock interpretation is used.
			 */

			KeyCode caps_lock = XKeysymToKeycode(disp, XK_Caps_Lock);
			KeyCode shift_lock = XKeysymToKeycode(disp, XK_Shift_Lock);
			keysyms_per_keycode--;

			// Loop over the modifier map to find out if/where shift and caps locks are set.
			for (int i = LockMapIndex; i < LockMapIndex + modifier_map->max_keypermod && !is_caps_lock; i++) {
				if (caps_lock != 0 && modifier_map->modifiermap[i] == caps_lock) {
					is_caps_lock = true;
					is_shift_lock = false;
				}
				else if (shift_lock != 0 && modifier_map->modifiermap[i] == shift_lock) {
					is_shift_lock = true;
				}
			}

			XFree(modifier_map);
		}
		else {
			XFree(keyboard_map);

			#ifdef DEBUG
			fprintf(stderr, "Initialize(): Unable to get modifier mapping table.\n");
			#endif
		}
	}
	#ifdef DEBUG
	else {
		fprintf(stderr, "Initialize(): Unable to get keyboard mapping table.\n");
	}
	#endif
	#endif
}

void UnloadInputHelper() {
	if (keyboard_map) {
		#ifdef XKB
		XkbFreeClientMap(keyboard_map, XkbAllClientInfoMask, true);
		#else
		XFree(keyboard_map);
		#endif
	}
}
