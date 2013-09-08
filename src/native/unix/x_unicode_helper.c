/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
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

/***********************************************************************
 * This module converts keysym values into the corresponding ISO 10646-1
 * (UCS, Unicode) values.
 *
 * The array keysym_unicode_table[] contains pairs of X11 keysym values for graphical
 * characters and the corresponding Unicode value. The function
 * keysym2ucs() maps a keysym onto a Unicode value using a binary search,
 * therefore keysym_unicode_table[] must remain SORTED by keysym value.
 *
 * The keysym -> UTF-8 conversion will hopefully one day be provided
 * by Xlib via XmbLookupString() and should ideally not have to be
 * done in X applications. But we are not there yet.
 *
 * We allow to represent any UCS character in the range U+00000000 to
 * U+00FFFFFF by a keysym value in the range 0x01000000 to 0x01FFFFFF.
 * This admittedly does not cover the entire 31-bit space of UCS, but
 * it does cover all of the characters up to U+10FFFF, which can be
 * represented by UTF-16, and more, and it is very unlikely that higher
 * UCS codes will ever be assigned by ISO. So to get Unicode character
 * U+ABCD you can directly use keysym 0x1000ABCD.
 *
 * NOTE: The comments in the table below contain the actual character
 * encoded in UTF-8, so for viewing and editing best use an editor in
 * UTF-8 mode.
 *
 * Author: Markus G. Kuhn <mkuhn@acm.org>, University of Cambridge, June 1999
 *
 * Special thanks to Richard Verhoeven <river@win.tue.nl> for preparing
 * an initial draft of the mapping table.
 *
 * This software is in the public domain. Share and enjoy!
 ***********************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include <X11/Xlib.h>

#include "logger.h"
#include "x_unicode_helper.h"

// KeySym to Unicode lookup table.
static struct codepair {
  uint16_t keysym;
  uint16_t ucs;
} keysym_unicode_table[] = {
  { 0x01A1, 0x0104 }, /*                     Aogonek Ą LATIN CAPITAL LETTER A WITH OGONEK */
  { 0x01A2, 0x02D8 }, /*                       breve ˘ BREVE */
  { 0x01A3, 0x0141 }, /*                     Lstroke Ł LATIN CAPITAL LETTER L WITH STROKE */
  { 0x01A5, 0x013D }, /*                      Lcaron Ľ LATIN CAPITAL LETTER L WITH CARON */
  { 0x01A6, 0x015A }, /*                      Sacute Ś LATIN CAPITAL LETTER S WITH ACUTE */
  { 0x01A9, 0x0160 }, /*                      Scaron Š LATIN CAPITAL LETTER S WITH CARON */
  { 0x01AA, 0x015E }, /*                    Scedilla Ş LATIN CAPITAL LETTER S WITH CEDILLA */
  { 0x01AB, 0x0164 }, /*                      Tcaron Ť LATIN CAPITAL LETTER T WITH CARON */
  { 0x01AC, 0x0179 }, /*                      Zacute Ź LATIN CAPITAL LETTER Z WITH ACUTE */
  { 0x01AE, 0x017D }, /*                      Zcaron Ž LATIN CAPITAL LETTER Z WITH CARON */
  { 0x01AF, 0x017B }, /*                   Zabovedot Ż LATIN CAPITAL LETTER Z WITH DOT ABOVE */
  { 0x01B1, 0x0105 }, /*                     aogonek ą LATIN SMALL LETTER A WITH OGONEK */
  { 0x01B2, 0x02DB }, /*                      ogonek ˛ OGONEK */
  { 0x01B3, 0x0142 }, /*                     lstroke ł LATIN SMALL LETTER L WITH STROKE */
  { 0x01B5, 0x013E }, /*                      lcaron ľ LATIN SMALL LETTER L WITH CARON */
  { 0x01B6, 0x015B }, /*                      sacute ś LATIN SMALL LETTER S WITH ACUTE */
  { 0x01B7, 0x02C7 }, /*                       caron ˇ CARON */
  { 0x01B9, 0x0161 }, /*                      scaron š LATIN SMALL LETTER S WITH CARON */
  { 0x01BA, 0x015F }, /*                    scedilla ş LATIN SMALL LETTER S WITH CEDILLA */
  { 0x01BB, 0x0165 }, /*                      tcaron ť LATIN SMALL LETTER T WITH CARON */
  { 0x01BC, 0x017A }, /*                      zacute ź LATIN SMALL LETTER Z WITH ACUTE */
  { 0x01BD, 0x02DD }, /*                 doubleacute ˝ DOUBLE ACUTE ACCENT */
  { 0x01BE, 0x017E }, /*                      zcaron ž LATIN SMALL LETTER Z WITH CARON */
  { 0x01BF, 0x017C }, /*                   zabovedot ż LATIN SMALL LETTER Z WITH DOT ABOVE */
  { 0x01C0, 0x0154 }, /*                      Racute Ŕ LATIN CAPITAL LETTER R WITH ACUTE */
  { 0x01C3, 0x0102 }, /*                      Abreve Ă LATIN CAPITAL LETTER A WITH BREVE */
  { 0x01C5, 0x0139 }, /*                      Lacute Ĺ LATIN CAPITAL LETTER L WITH ACUTE */
  { 0x01C6, 0x0106 }, /*                      Cacute Ć LATIN CAPITAL LETTER C WITH ACUTE */
  { 0x01C8, 0x010C }, /*                      Ccaron Č LATIN CAPITAL LETTER C WITH CARON */
  { 0x01CA, 0x0118 }, /*                     Eogonek Ę LATIN CAPITAL LETTER E WITH OGONEK */
  { 0x01CC, 0x011A }, /*                      Ecaron Ě LATIN CAPITAL LETTER E WITH CARON */
  { 0x01CF, 0x010E }, /*                      Dcaron Ď LATIN CAPITAL LETTER D WITH CARON */
  { 0x01D0, 0x0110 }, /*                     Dstroke Đ LATIN CAPITAL LETTER D WITH STROKE */
  { 0x01D1, 0x0143 }, /*                      Nacute Ń LATIN CAPITAL LETTER N WITH ACUTE */
  { 0x01D2, 0x0147 }, /*                      Ncaron Ň LATIN CAPITAL LETTER N WITH CARON */
  { 0x01D5, 0x0150 }, /*                Odoubleacute Ő LATIN CAPITAL LETTER O WITH DOUBLE ACUTE */
  { 0x01D8, 0x0158 }, /*                      Rcaron Ř LATIN CAPITAL LETTER R WITH CARON */
  { 0x01D9, 0x016E }, /*                       Uring Ů LATIN CAPITAL LETTER U WITH RING ABOVE */
  { 0x01DB, 0x0170 }, /*                Udoubleacute Ű LATIN CAPITAL LETTER U WITH DOUBLE ACUTE */
  { 0x01DE, 0x0162 }, /*                    Tcedilla Ţ LATIN CAPITAL LETTER T WITH CEDILLA */
  { 0x01E0, 0x0155 }, /*                      racute ŕ LATIN SMALL LETTER R WITH ACUTE */
  { 0x01E3, 0x0103 }, /*                      abreve ă LATIN SMALL LETTER A WITH BREVE */
  { 0x01E5, 0x013A }, /*                      lacute ĺ LATIN SMALL LETTER L WITH ACUTE */
  { 0x01E6, 0x0107 }, /*                      cacute ć LATIN SMALL LETTER C WITH ACUTE */
  { 0x01E8, 0x010D }, /*                      ccaron č LATIN SMALL LETTER C WITH CARON */
  { 0x01EA, 0x0119 }, /*                     eogonek ę LATIN SMALL LETTER E WITH OGONEK */
  { 0x01EC, 0x011B }, /*                      ecaron ě LATIN SMALL LETTER E WITH CARON */
  { 0x01EF, 0x010F }, /*                      dcaron ď LATIN SMALL LETTER D WITH CARON */
  { 0x01F0, 0x0111 }, /*                     dstroke đ LATIN SMALL LETTER D WITH STROKE */
  { 0x01F1, 0x0144 }, /*                      nacute ń LATIN SMALL LETTER N WITH ACUTE */
  { 0x01F2, 0x0148 }, /*                      ncaron ň LATIN SMALL LETTER N WITH CARON */
  { 0x01F5, 0x0151 }, /*                odoubleacute ő LATIN SMALL LETTER O WITH DOUBLE ACUTE */
  { 0x01F8, 0x0159 }, /*                      rcaron ř LATIN SMALL LETTER R WITH CARON */
  { 0x01F9, 0x016F }, /*                       uring ů LATIN SMALL LETTER U WITH RING ABOVE */
  { 0x01FB, 0x0171 }, /*                udoubleacute ű LATIN SMALL LETTER U WITH DOUBLE ACUTE */
  { 0x01FE, 0x0163 }, /*                    tcedilla ţ LATIN SMALL LETTER T WITH CEDILLA */
  { 0x01FF, 0x02D9 }, /*                    abovedot ˙ DOT ABOVE */
  { 0x02A1, 0x0126 }, /*                     Hstroke Ħ LATIN CAPITAL LETTER H WITH STROKE */
  { 0x02A6, 0x0124 }, /*                 Hcircumflex Ĥ LATIN CAPITAL LETTER H WITH CIRCUMFLEX */
  { 0x02A9, 0x0130 }, /*                   Iabovedot İ LATIN CAPITAL LETTER I WITH DOT ABOVE */
  { 0x02AB, 0x011E }, /*                      Gbreve Ğ LATIN CAPITAL LETTER G WITH BREVE */
  { 0x02AC, 0x0134 }, /*                 Jcircumflex Ĵ LATIN CAPITAL LETTER J WITH CIRCUMFLEX */
  { 0x02B1, 0x0127 }, /*                     hstroke ħ LATIN SMALL LETTER H WITH STROKE */
  { 0x02B6, 0x0125 }, /*                 hcircumflex ĥ LATIN SMALL LETTER H WITH CIRCUMFLEX */
  { 0x02B9, 0x0131 }, /*                    idotless ı LATIN SMALL LETTER DOTLESS I */
  { 0x02BB, 0x011F }, /*                      gbreve ğ LATIN SMALL LETTER G WITH BREVE */
  { 0x02BC, 0x0135 }, /*                 jcircumflex ĵ LATIN SMALL LETTER J WITH CIRCUMFLEX */
  { 0x02C5, 0x010A }, /*                   Cabovedot Ċ LATIN CAPITAL LETTER C WITH DOT ABOVE */
  { 0x02C6, 0x0108 }, /*                 Ccircumflex Ĉ LATIN CAPITAL LETTER C WITH CIRCUMFLEX */
  { 0x02D5, 0x0120 }, /*                   Gabovedot Ġ LATIN CAPITAL LETTER G WITH DOT ABOVE */
  { 0x02D8, 0x011C }, /*                 Gcircumflex Ĝ LATIN CAPITAL LETTER G WITH CIRCUMFLEX */
  { 0x02DD, 0x016C }, /*                      Ubreve Ŭ LATIN CAPITAL LETTER U WITH BREVE */
  { 0x02DE, 0x015C }, /*                 Scircumflex Ŝ LATIN CAPITAL LETTER S WITH CIRCUMFLEX */
  { 0x02E5, 0x010B }, /*                   cabovedot ċ LATIN SMALL LETTER C WITH DOT ABOVE */
  { 0x02E6, 0x0109 }, /*                 ccircumflex ĉ LATIN SMALL LETTER C WITH CIRCUMFLEX */
  { 0x02F5, 0x0121 }, /*                   gabovedot ġ LATIN SMALL LETTER G WITH DOT ABOVE */
  { 0x02F8, 0x011D }, /*                 gcircumflex ĝ LATIN SMALL LETTER G WITH CIRCUMFLEX */
  { 0x02FD, 0x016D }, /*                      ubreve ŭ LATIN SMALL LETTER U WITH BREVE */
  { 0x02FE, 0x015D }, /*                 scircumflex ŝ LATIN SMALL LETTER S WITH CIRCUMFLEX */
  { 0x03A2, 0x0138 }, /*                         kra ĸ LATIN SMALL LETTER KRA */
  { 0x03A3, 0x0156 }, /*                    Rcedilla Ŗ LATIN CAPITAL LETTER R WITH CEDILLA */
  { 0x03A5, 0x0128 }, /*                      Itilde Ĩ LATIN CAPITAL LETTER I WITH TILDE */
  { 0x03A6, 0x013B }, /*                    Lcedilla Ļ LATIN CAPITAL LETTER L WITH CEDILLA */
  { 0x03AA, 0x0112 }, /*                     Emacron Ē LATIN CAPITAL LETTER E WITH MACRON */
  { 0x03AB, 0x0122 }, /*                    Gcedilla Ģ LATIN CAPITAL LETTER G WITH CEDILLA */
  { 0x03AC, 0x0166 }, /*                      Tslash Ŧ LATIN CAPITAL LETTER T WITH STROKE */
  { 0x03B3, 0x0157 }, /*                    rcedilla ŗ LATIN SMALL LETTER R WITH CEDILLA */
  { 0x03B5, 0x0129 }, /*                      itilde ĩ LATIN SMALL LETTER I WITH TILDE */
  { 0x03B6, 0x013C }, /*                    lcedilla ļ LATIN SMALL LETTER L WITH CEDILLA */
  { 0x03BA, 0x0113 }, /*                     emacron ē LATIN SMALL LETTER E WITH MACRON */
  { 0x03BB, 0x0123 }, /*                    gcedilla ģ LATIN SMALL LETTER G WITH CEDILLA */
  { 0x03BC, 0x0167 }, /*                      tslash ŧ LATIN SMALL LETTER T WITH STROKE */
  { 0x03BD, 0x014A }, /*                         ENG Ŋ LATIN CAPITAL LETTER ENG */
  { 0x03BF, 0x014B }, /*                         eng ŋ LATIN SMALL LETTER ENG */
  { 0x03C0, 0x0100 }, /*                     Amacron Ā LATIN CAPITAL LETTER A WITH MACRON */
  { 0x03C7, 0x012E }, /*                     Iogonek Į LATIN CAPITAL LETTER I WITH OGONEK */
  { 0x03CC, 0x0116 }, /*                   Eabovedot Ė LATIN CAPITAL LETTER E WITH DOT ABOVE */
  { 0x03CF, 0x012A }, /*                     Imacron Ī LATIN CAPITAL LETTER I WITH MACRON */
  { 0x03D1, 0x0145 }, /*                    Ncedilla Ņ LATIN CAPITAL LETTER N WITH CEDILLA */
  { 0x03D2, 0x014C }, /*                     Omacron Ō LATIN CAPITAL LETTER O WITH MACRON */
  { 0x03D3, 0x0136 }, /*                    Kcedilla Ķ LATIN CAPITAL LETTER K WITH CEDILLA */
  { 0x03D9, 0x0172 }, /*                     Uogonek Ų LATIN CAPITAL LETTER U WITH OGONEK */
  { 0x03DD, 0x0168 }, /*                      Utilde Ũ LATIN CAPITAL LETTER U WITH TILDE */
  { 0x03DE, 0x016A }, /*                     Umacron Ū LATIN CAPITAL LETTER U WITH MACRON */
  { 0x03E0, 0x0101 }, /*                     amacron ā LATIN SMALL LETTER A WITH MACRON */
  { 0x03E7, 0x012F }, /*                     iogonek į LATIN SMALL LETTER I WITH OGONEK */
  { 0x03EC, 0x0117 }, /*                   eabovedot ė LATIN SMALL LETTER E WITH DOT ABOVE */
  { 0x03EF, 0x012B }, /*                     imacron ī LATIN SMALL LETTER I WITH MACRON */
  { 0x03F1, 0x0146 }, /*                    ncedilla ņ LATIN SMALL LETTER N WITH CEDILLA */
  { 0x03F2, 0x014D }, /*                     omacron ō LATIN SMALL LETTER O WITH MACRON */
  { 0x03F3, 0x0137 }, /*                    kcedilla ķ LATIN SMALL LETTER K WITH CEDILLA */
  { 0x03F9, 0x0173 }, /*                     uogonek ų LATIN SMALL LETTER U WITH OGONEK */
  { 0x03FD, 0x0169 }, /*                      utilde ũ LATIN SMALL LETTER U WITH TILDE */
  { 0x03FE, 0x016B }, /*                     umacron ū LATIN SMALL LETTER U WITH MACRON */
  { 0x047E, 0x203E }, /*                    overline ‾ OVERLINE */
  { 0x04A1, 0x3002 }, /*               kana_fullstop 。 IDEOGRAPHIC FULL STOP */
  { 0x04A2, 0x300C }, /*         kana_openingbracket 「 LEFT CORNER BRACKET */
  { 0x04A3, 0x300D }, /*         kana_closingbracket 」 RIGHT CORNER BRACKET */
  { 0x04A4, 0x3001 }, /*                  kana_comma 、 IDEOGRAPHIC COMMA */
  { 0x04A5, 0x30FB }, /*            kana_conjunctive ・ KATAKANA MIDDLE DOT */
  { 0x04A6, 0x30F2 }, /*                     kana_WO ヲ KATAKANA LETTER WO */
  { 0x04A7, 0x30A1 }, /*                      kana_a ァ KATAKANA LETTER SMALL A */
  { 0x04A8, 0x30A3 }, /*                      kana_i ィ KATAKANA LETTER SMALL I */
  { 0x04A9, 0x30A5 }, /*                      kana_u ゥ KATAKANA LETTER SMALL U */
  { 0x04AA, 0x30A7 }, /*                      kana_e ェ KATAKANA LETTER SMALL E */
  { 0x04AB, 0x30A9 }, /*                      kana_o ォ KATAKANA LETTER SMALL O */
  { 0x04AC, 0x30E3 }, /*                     kana_ya ャ KATAKANA LETTER SMALL YA */
  { 0x04AD, 0x30E5 }, /*                     kana_yu ュ KATAKANA LETTER SMALL YU */
  { 0x04AE, 0x30E7 }, /*                     kana_yo ョ KATAKANA LETTER SMALL YO */
  { 0x04AF, 0x30C3 }, /*                    kana_tsu ッ KATAKANA LETTER SMALL TU */
  { 0x04B0, 0x30FC }, /*              prolongedsound ー KATAKANA-HIRAGANA PROLONGED SOUND MARK */
  { 0x04B1, 0x30A2 }, /*                      kana_A ア KATAKANA LETTER A */
  { 0x04B2, 0x30A4 }, /*                      kana_I イ KATAKANA LETTER I */
  { 0x04B3, 0x30A6 }, /*                      kana_U ウ KATAKANA LETTER U */
  { 0x04B4, 0x30A8 }, /*                      kana_E エ KATAKANA LETTER E */
  { 0x04B5, 0x30AA }, /*                      kana_O オ KATAKANA LETTER O */
  { 0x04B6, 0x30AB }, /*                     kana_KA カ KATAKANA LETTER KA */
  { 0x04B7, 0x30AD }, /*                     kana_KI キ KATAKANA LETTER KI */
  { 0x04B8, 0x30AF }, /*                     kana_KU ク KATAKANA LETTER KU */
  { 0x04B9, 0x30B1 }, /*                     kana_KE ケ KATAKANA LETTER KE */
  { 0x04BA, 0x30B3 }, /*                     kana_KO コ KATAKANA LETTER KO */
  { 0x04BB, 0x30B5 }, /*                     kana_SA サ KATAKANA LETTER SA */
  { 0x04BC, 0x30B7 }, /*                    kana_SHI シ KATAKANA LETTER SI */
  { 0x04BD, 0x30B9 }, /*                     kana_SU ス KATAKANA LETTER SU */
  { 0x04BE, 0x30BB }, /*                     kana_SE セ KATAKANA LETTER SE */
  { 0x04BF, 0x30BD }, /*                     kana_SO ソ KATAKANA LETTER SO */
  { 0x04C0, 0x30BF }, /*                     kana_TA タ KATAKANA LETTER TA */
  { 0x04C1, 0x30C1 }, /*                    kana_CHI チ KATAKANA LETTER TI */
  { 0x04C2, 0x30C4 }, /*                    kana_TSU ツ KATAKANA LETTER TU */
  { 0x04C3, 0x30C6 }, /*                     kana_TE テ KATAKANA LETTER TE */
  { 0x04C4, 0x30C8 }, /*                     kana_TO ト KATAKANA LETTER TO */
  { 0x04C5, 0x30CA }, /*                     kana_NA ナ KATAKANA LETTER NA */
  { 0x04C6, 0x30CB }, /*                     kana_NI ニ KATAKANA LETTER NI */
  { 0x04C7, 0x30CC }, /*                     kana_NU ヌ KATAKANA LETTER NU */
  { 0x04C8, 0x30CD }, /*                     kana_NE ネ KATAKANA LETTER NE */
  { 0x04C9, 0x30CE }, /*                     kana_NO ノ KATAKANA LETTER NO */
  { 0x04CA, 0x30CF }, /*                     kana_HA ハ KATAKANA LETTER HA */
  { 0x04CB, 0x30D2 }, /*                     kana_HI ヒ KATAKANA LETTER HI */
  { 0x04CC, 0x30D5 }, /*                     kana_FU フ KATAKANA LETTER HU */
  { 0x04CD, 0x30D8 }, /*                     kana_HE ヘ KATAKANA LETTER HE */
  { 0x04CE, 0x30DB }, /*                     kana_HO ホ KATAKANA LETTER HO */
  { 0x04CF, 0x30DE }, /*                     kana_MA マ KATAKANA LETTER MA */
  { 0x04D0, 0x30DF }, /*                     kana_MI ミ KATAKANA LETTER MI */
  { 0x04D1, 0x30E0 }, /*                     kana_MU ム KATAKANA LETTER MU */
  { 0x04D2, 0x30E1 }, /*                     kana_ME メ KATAKANA LETTER ME */
  { 0x04D3, 0x30E2 }, /*                     kana_MO モ KATAKANA LETTER MO */
  { 0x04D4, 0x30E4 }, /*                     kana_YA ヤ KATAKANA LETTER YA */
  { 0x04D5, 0x30E6 }, /*                     kana_YU ユ KATAKANA LETTER YU */
  { 0x04D6, 0x30E8 }, /*                     kana_YO ヨ KATAKANA LETTER YO */
  { 0x04D7, 0x30E9 }, /*                     kana_RA ラ KATAKANA LETTER RA */
  { 0x04D8, 0x30EA }, /*                     kana_RI リ KATAKANA LETTER RI */
  { 0x04D9, 0x30EB }, /*                     kana_RU ル KATAKANA LETTER RU */
  { 0x04DA, 0x30EC }, /*                     kana_RE レ KATAKANA LETTER RE */
  { 0x04DB, 0x30ED }, /*                     kana_RO ロ KATAKANA LETTER RO */
  { 0x04DC, 0x30EF }, /*                     kana_WA ワ KATAKANA LETTER WA */
  { 0x04DD, 0x30F3 }, /*                      kana_N ン KATAKANA LETTER N */
  { 0x04DE, 0x309B }, /*                 voicedsound ゛ KATAKANA-HIRAGANA VOICED SOUND MARK */
  { 0x04DF, 0x309C }, /*             semivoicedsound ゜ KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK */
  { 0x05AC, 0x060C }, /*                Arabic_comma ، ARABIC COMMA */
  { 0x05BB, 0x061B }, /*            Arabic_semicolon ؛ ARABIC SEMICOLON */
  { 0x05BF, 0x061F }, /*        Arabic_question_mark ؟ ARABIC QUESTION MARK */
  { 0x05C1, 0x0621 }, /*                Arabic_hamza ء ARABIC LETTER HAMZA */
  { 0x05C2, 0x0622 }, /*          Arabic_maddaonalef آ ARABIC LETTER ALEF WITH MADDA ABOVE */
  { 0x05C3, 0x0623 }, /*          Arabic_hamzaonalef أ ARABIC LETTER ALEF WITH HAMZA ABOVE */
  { 0x05C4, 0x0624 }, /*           Arabic_hamzaonwaw ؤ ARABIC LETTER WAW WITH HAMZA ABOVE */
  { 0x05C5, 0x0625 }, /*       Arabic_hamzaunderalef إ ARABIC LETTER ALEF WITH HAMZA BELOW */
  { 0x05C6, 0x0626 }, /*           Arabic_hamzaonyeh ئ ARABIC LETTER YEH WITH HAMZA ABOVE */
  { 0x05C7, 0x0627 }, /*                 Arabic_alef ا ARABIC LETTER ALEF */
  { 0x05C8, 0x0628 }, /*                  Arabic_beh ب ARABIC LETTER BEH */
  { 0x05C9, 0x0629 }, /*           Arabic_tehmarbuta ة ARABIC LETTER TEH MARBUTA */
  { 0x05CA, 0x062A }, /*                  Arabic_teh ت ARABIC LETTER TEH */
  { 0x05CB, 0x062B }, /*                 Arabic_theh ث ARABIC LETTER THEH */
  { 0x05CC, 0x062C }, /*                 Arabic_jeem ج ARABIC LETTER JEEM */
  { 0x05CD, 0x062D }, /*                  Arabic_hah ح ARABIC LETTER HAH */
  { 0x05CE, 0x062E }, /*                 Arabic_khah خ ARABIC LETTER KHAH */
  { 0x05CF, 0x062F }, /*                  Arabic_dal د ARABIC LETTER DAL */
  { 0x05D0, 0x0630 }, /*                 Arabic_thal ذ ARABIC LETTER THAL */
  { 0x05D1, 0x0631 }, /*                   Arabic_ra ر ARABIC LETTER REH */
  { 0x05D2, 0x0632 }, /*                 Arabic_zain ز ARABIC LETTER ZAIN */
  { 0x05D3, 0x0633 }, /*                 Arabic_seen س ARABIC LETTER SEEN */
  { 0x05D4, 0x0634 }, /*                Arabic_sheen ش ARABIC LETTER SHEEN */
  { 0x05D5, 0x0635 }, /*                  Arabic_sad ص ARABIC LETTER SAD */
  { 0x05D6, 0x0636 }, /*                  Arabic_dad ض ARABIC LETTER DAD */
  { 0x05D7, 0x0637 }, /*                  Arabic_tah ط ARABIC LETTER TAH */
  { 0x05D8, 0x0638 }, /*                  Arabic_zah ظ ARABIC LETTER ZAH */
  { 0x05D9, 0x0639 }, /*                  Arabic_ain ع ARABIC LETTER AIN */
  { 0x05DA, 0x063A }, /*                Arabic_ghain غ ARABIC LETTER GHAIN */
  { 0x05E0, 0x0640 }, /*              Arabic_tatweel ـ ARABIC TATWEEL */
  { 0x05E1, 0x0641 }, /*                  Arabic_feh ف ARABIC LETTER FEH */
  { 0x05E2, 0x0642 }, /*                  Arabic_qaf ق ARABIC LETTER QAF */
  { 0x05E3, 0x0643 }, /*                  Arabic_kaf ك ARABIC LETTER KAF */
  { 0x05E4, 0x0644 }, /*                  Arabic_lam ل ARABIC LETTER LAM */
  { 0x05E5, 0x0645 }, /*                 Arabic_meem م ARABIC LETTER MEEM */
  { 0x05E6, 0x0646 }, /*                 Arabic_noon ن ARABIC LETTER NOON */
  { 0x05E7, 0x0647 }, /*                   Arabic_ha ه ARABIC LETTER HEH */
  { 0x05E8, 0x0648 }, /*                  Arabic_waw و ARABIC LETTER WAW */
  { 0x05E9, 0x0649 }, /*          Arabic_alefmaksura ى ARABIC LETTER ALEF MAKSURA */
  { 0x05EA, 0x064A }, /*                  Arabic_yeh ي ARABIC LETTER YEH */
  { 0x05EB, 0x064B }, /*             Arabic_fathatan ً ARABIC FATHATAN */
  { 0x05EC, 0x064C }, /*             Arabic_dammatan ٌ ARABIC DAMMATAN */
  { 0x05ED, 0x064D }, /*             Arabic_kasratan ٍ ARABIC KASRATAN */
  { 0x05EE, 0x064E }, /*                Arabic_fatha َ ARABIC FATHA */
  { 0x05EF, 0x064F }, /*                Arabic_damma ُ ARABIC DAMMA */
  { 0x05F0, 0x0650 }, /*                Arabic_kasra ِ ARABIC KASRA */
  { 0x05F1, 0x0651 }, /*               Arabic_shadda ّ ARABIC SHADDA */
  { 0x05F2, 0x0652 }, /*                Arabic_sukun ْ ARABIC SUKUN */
  { 0x06A1, 0x0452 }, /*                 Serbian_dje ђ CYRILLIC SMALL LETTER DJE */
  { 0x06A2, 0x0453 }, /*               Macedonia_gje ѓ CYRILLIC SMALL LETTER GJE */
  { 0x06A3, 0x0451 }, /*                 Cyrillic_io ё CYRILLIC SMALL LETTER IO */
  { 0x06A4, 0x0454 }, /*                Ukrainian_ie є CYRILLIC SMALL LETTER UKRAINIAN IE */
  { 0x06A5, 0x0455 }, /*               Macedonia_dse ѕ CYRILLIC SMALL LETTER DZE */
  { 0x06A6, 0x0456 }, /*                 Ukrainian_i і CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I */
  { 0x06A7, 0x0457 }, /*                Ukrainian_yi ї CYRILLIC SMALL LETTER YI */
  { 0x06A8, 0x0458 }, /*                 Cyrillic_je ј CYRILLIC SMALL LETTER JE */
  { 0x06A9, 0x0459 }, /*                Cyrillic_lje љ CYRILLIC SMALL LETTER LJE */
  { 0x06AA, 0x045A }, /*                Cyrillic_nje њ CYRILLIC SMALL LETTER NJE */
  { 0x06AB, 0x045B }, /*                Serbian_tshe ћ CYRILLIC SMALL LETTER TSHE */
  { 0x06AC, 0x045C }, /*               Macedonia_kje ќ CYRILLIC SMALL LETTER KJE */
  { 0x06AE, 0x045E }, /*         Byelorussian_shortu ў CYRILLIC SMALL LETTER SHORT U */
  { 0x06AF, 0x045F }, /*               Cyrillic_dzhe џ CYRILLIC SMALL LETTER DZHE */
  { 0x06B0, 0x2116 }, /*                  numerosign № NUMERO SIGN */
  { 0x06B1, 0x0402 }, /*                 Serbian_DJE Ђ CYRILLIC CAPITAL LETTER DJE */
  { 0x06B2, 0x0403 }, /*               Macedonia_GJE Ѓ CYRILLIC CAPITAL LETTER GJE */
  { 0x06B3, 0x0401 }, /*                 Cyrillic_IO Ё CYRILLIC CAPITAL LETTER IO */
  { 0x06B4, 0x0404 }, /*                Ukrainian_IE Є CYRILLIC CAPITAL LETTER UKRAINIAN IE */
  { 0x06B5, 0x0405 }, /*               Macedonia_DSE Ѕ CYRILLIC CAPITAL LETTER DZE */
  { 0x06B6, 0x0406 }, /*                 Ukrainian_I І CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I */
  { 0x06B7, 0x0407 }, /*                Ukrainian_YI Ї CYRILLIC CAPITAL LETTER YI */
  { 0x06B8, 0x0408 }, /*                 Cyrillic_JE Ј CYRILLIC CAPITAL LETTER JE */
  { 0x06B9, 0x0409 }, /*                Cyrillic_LJE Љ CYRILLIC CAPITAL LETTER LJE */
  { 0x06BA, 0x040A }, /*                Cyrillic_NJE Њ CYRILLIC CAPITAL LETTER NJE */
  { 0x06BB, 0x040B }, /*                Serbian_TSHE Ћ CYRILLIC CAPITAL LETTER TSHE */
  { 0x06BC, 0x040C }, /*               Macedonia_KJE Ќ CYRILLIC CAPITAL LETTER KJE */
  { 0x06BE, 0x040E }, /*         Byelorussian_SHORTU Ў CYRILLIC CAPITAL LETTER SHORT U */
  { 0x06BF, 0x040F }, /*               Cyrillic_DZHE Џ CYRILLIC CAPITAL LETTER DZHE */
  { 0x06C0, 0x044E }, /*                 Cyrillic_yu ю CYRILLIC SMALL LETTER YU */
  { 0x06C1, 0x0430 }, /*                  Cyrillic_a а CYRILLIC SMALL LETTER A */
  { 0x06C2, 0x0431 }, /*                 Cyrillic_be б CYRILLIC SMALL LETTER BE */
  { 0x06C3, 0x0446 }, /*                Cyrillic_tse ц CYRILLIC SMALL LETTER TSE */
  { 0x06C4, 0x0434 }, /*                 Cyrillic_de д CYRILLIC SMALL LETTER DE */
  { 0x06C5, 0x0435 }, /*                 Cyrillic_ie е CYRILLIC SMALL LETTER IE */
  { 0x06C6, 0x0444 }, /*                 Cyrillic_ef ф CYRILLIC SMALL LETTER EF */
  { 0x06C7, 0x0433 }, /*                Cyrillic_ghe г CYRILLIC SMALL LETTER GHE */
  { 0x06C8, 0x0445 }, /*                 Cyrillic_ha х CYRILLIC SMALL LETTER HA */
  { 0x06C9, 0x0438 }, /*                  Cyrillic_i и CYRILLIC SMALL LETTER I */
  { 0x06CA, 0x0439 }, /*             Cyrillic_shorti й CYRILLIC SMALL LETTER SHORT I */
  { 0x06CB, 0x043A }, /*                 Cyrillic_ka к CYRILLIC SMALL LETTER KA */
  { 0x06CC, 0x043B }, /*                 Cyrillic_el л CYRILLIC SMALL LETTER EL */
  { 0x06CD, 0x043C }, /*                 Cyrillic_em м CYRILLIC SMALL LETTER EM */
  { 0x06CE, 0x043D }, /*                 Cyrillic_en н CYRILLIC SMALL LETTER EN */
  { 0x06CF, 0x043E }, /*                  Cyrillic_o о CYRILLIC SMALL LETTER O */
  { 0x06D0, 0x043F }, /*                 Cyrillic_pe п CYRILLIC SMALL LETTER PE */
  { 0x06D1, 0x044F }, /*                 Cyrillic_ya я CYRILLIC SMALL LETTER YA */
  { 0x06D2, 0x0440 }, /*                 Cyrillic_er р CYRILLIC SMALL LETTER ER */
  { 0x06D3, 0x0441 }, /*                 Cyrillic_es с CYRILLIC SMALL LETTER ES */
  { 0x06D4, 0x0442 }, /*                 Cyrillic_te т CYRILLIC SMALL LETTER TE */
  { 0x06D5, 0x0443 }, /*                  Cyrillic_u у CYRILLIC SMALL LETTER U */
  { 0x06D6, 0x0436 }, /*                Cyrillic_zhe ж CYRILLIC SMALL LETTER ZHE */
  { 0x06D7, 0x0432 }, /*                 Cyrillic_ve в CYRILLIC SMALL LETTER VE */
  { 0x06D8, 0x044C }, /*           Cyrillic_softsign ь CYRILLIC SMALL LETTER SOFT SIGN */
  { 0x06D9, 0x044B }, /*               Cyrillic_yeru ы CYRILLIC SMALL LETTER YERU */
  { 0x06DA, 0x0437 }, /*                 Cyrillic_ze з CYRILLIC SMALL LETTER ZE */
  { 0x06DB, 0x0448 }, /*                Cyrillic_sha ш CYRILLIC SMALL LETTER SHA */
  { 0x06DC, 0x044D }, /*                  Cyrillic_e э CYRILLIC SMALL LETTER E */
  { 0x06DD, 0x0449 }, /*              Cyrillic_shcha щ CYRILLIC SMALL LETTER SHCHA */
  { 0x06DE, 0x0447 }, /*                Cyrillic_che ч CYRILLIC SMALL LETTER CHE */
  { 0x06DF, 0x044A }, /*           Cyrillic_hardsign ъ CYRILLIC SMALL LETTER HARD SIGN */
  { 0x06E0, 0x042E }, /*                 Cyrillic_YU Ю CYRILLIC CAPITAL LETTER YU */
  { 0x06E1, 0x0410 }, /*                  Cyrillic_A А CYRILLIC CAPITAL LETTER A */
  { 0x06E2, 0x0411 }, /*                 Cyrillic_BE Б CYRILLIC CAPITAL LETTER BE */
  { 0x06E3, 0x0426 }, /*                Cyrillic_TSE Ц CYRILLIC CAPITAL LETTER TSE */
  { 0x06E4, 0x0414 }, /*                 Cyrillic_DE Д CYRILLIC CAPITAL LETTER DE */
  { 0x06E5, 0x0415 }, /*                 Cyrillic_IE Е CYRILLIC CAPITAL LETTER IE */
  { 0x06E6, 0x0424 }, /*                 Cyrillic_EF Ф CYRILLIC CAPITAL LETTER EF */
  { 0x06E7, 0x0413 }, /*                Cyrillic_GHE Г CYRILLIC CAPITAL LETTER GHE */
  { 0x06E8, 0x0425 }, /*                 Cyrillic_HA Х CYRILLIC CAPITAL LETTER HA */
  { 0x06E9, 0x0418 }, /*                  Cyrillic_I И CYRILLIC CAPITAL LETTER I */
  { 0x06EA, 0x0419 }, /*             Cyrillic_SHORTI Й CYRILLIC CAPITAL LETTER SHORT I */
  { 0x06EB, 0x041A }, /*                 Cyrillic_KA К CYRILLIC CAPITAL LETTER KA */
  { 0x06EC, 0x041B }, /*                 Cyrillic_EL Л CYRILLIC CAPITAL LETTER EL */
  { 0x06ED, 0x041C }, /*                 Cyrillic_EM М CYRILLIC CAPITAL LETTER EM */
  { 0x06EE, 0x041D }, /*                 Cyrillic_EN Н CYRILLIC CAPITAL LETTER EN */
  { 0x06EF, 0x041E }, /*                  Cyrillic_O О CYRILLIC CAPITAL LETTER O */
  { 0x06F0, 0x041F }, /*                 Cyrillic_PE П CYRILLIC CAPITAL LETTER PE */
  { 0x06F1, 0x042F }, /*                 Cyrillic_YA Я CYRILLIC CAPITAL LETTER YA */
  { 0x06F2, 0x0420 }, /*                 Cyrillic_ER Р CYRILLIC CAPITAL LETTER ER */
  { 0x06F3, 0x0421 }, /*                 Cyrillic_ES С CYRILLIC CAPITAL LETTER ES */
  { 0x06F4, 0x0422 }, /*                 Cyrillic_TE Т CYRILLIC CAPITAL LETTER TE */
  { 0x06F5, 0x0423 }, /*                  Cyrillic_U У CYRILLIC CAPITAL LETTER U */
  { 0x06F6, 0x0416 }, /*                Cyrillic_ZHE Ж CYRILLIC CAPITAL LETTER ZHE */
  { 0x06F7, 0x0412 }, /*                 Cyrillic_VE В CYRILLIC CAPITAL LETTER VE */
  { 0x06F8, 0x042C }, /*           Cyrillic_SOFTSIGN Ь CYRILLIC CAPITAL LETTER SOFT SIGN */
  { 0x06F9, 0x042B }, /*               Cyrillic_YERU Ы CYRILLIC CAPITAL LETTER YERU */
  { 0x06FA, 0x0417 }, /*                 Cyrillic_ZE З CYRILLIC CAPITAL LETTER ZE */
  { 0x06FB, 0x0428 }, /*                Cyrillic_SHA Ш CYRILLIC CAPITAL LETTER SHA */
  { 0x06FC, 0x042D }, /*                  Cyrillic_E Э CYRILLIC CAPITAL LETTER E */
  { 0x06FD, 0x0429 }, /*              Cyrillic_SHCHA Щ CYRILLIC CAPITAL LETTER SHCHA */
  { 0x06FE, 0x0427 }, /*                Cyrillic_CHE Ч CYRILLIC CAPITAL LETTER CHE */
  { 0x06FF, 0x042A }, /*           Cyrillic_HARDSIGN Ъ CYRILLIC CAPITAL LETTER HARD SIGN */
  { 0x07A1, 0x0386 }, /*           Greek_ALPHAaccent Ά GREEK CAPITAL LETTER ALPHA WITH TONOS */
  { 0x07A2, 0x0388 }, /*         Greek_EPSILONaccent Έ GREEK CAPITAL LETTER EPSILON WITH TONOS */
  { 0x07A3, 0x0389 }, /*             Greek_ETAaccent Ή GREEK CAPITAL LETTER ETA WITH TONOS */
  { 0x07A4, 0x038A }, /*            Greek_IOTAaccent Ί GREEK CAPITAL LETTER IOTA WITH TONOS */
  { 0x07A5, 0x03AA }, /*         Greek_IOTAdiaeresis Ϊ GREEK CAPITAL LETTER IOTA WITH DIALYTIKA */
  { 0x07A7, 0x038C }, /*         Greek_OMICRONaccent Ό GREEK CAPITAL LETTER OMICRON WITH TONOS */
  { 0x07A8, 0x038E }, /*         Greek_UPSILONaccent Ύ GREEK CAPITAL LETTER UPSILON WITH TONOS */
  { 0x07A9, 0x03AB }, /*       Greek_UPSILONdieresis Ϋ GREEK CAPITAL LETTER UPSILON WITH DIALYTIKA */
  { 0x07AB, 0x038F }, /*           Greek_OMEGAaccent Ώ GREEK CAPITAL LETTER OMEGA WITH TONOS */
  { 0x07AE, 0x0385 }, /*        Greek_accentdieresis ΅ GREEK DIALYTIKA TONOS */
  { 0x07AF, 0x2015 }, /*              Greek_horizbar ― HORIZONTAL BAR */
  { 0x07B1, 0x03AC }, /*           Greek_alphaaccent ά GREEK SMALL LETTER ALPHA WITH TONOS */
  { 0x07B2, 0x03AD }, /*         Greek_epsilonaccent έ GREEK SMALL LETTER EPSILON WITH TONOS */
  { 0x07B3, 0x03AE }, /*             Greek_etaaccent ή GREEK SMALL LETTER ETA WITH TONOS */
  { 0x07B4, 0x03AF }, /*            Greek_iotaaccent ί GREEK SMALL LETTER IOTA WITH TONOS */
  { 0x07B5, 0x03CA }, /*          Greek_iotadieresis ϊ GREEK SMALL LETTER IOTA WITH DIALYTIKA */
  { 0x07B6, 0x0390 }, /*    Greek_iotaaccentdieresis ΐ GREEK SMALL LETTER IOTA WITH DIALYTIKA AND TONOS */
  { 0x07B7, 0x03CC }, /*         Greek_omicronaccent ό GREEK SMALL LETTER OMICRON WITH TONOS */
  { 0x07B8, 0x03CD }, /*         Greek_upsilonaccent ύ GREEK SMALL LETTER UPSILON WITH TONOS */
  { 0x07B9, 0x03CB }, /*       Greek_upsilondieresis ϋ GREEK SMALL LETTER UPSILON WITH DIALYTIKA */
  { 0x07BA, 0x03B0 }, /* Greek_upsilonaccentdieresis ΰ GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS */
  { 0x07BB, 0x03CE }, /*           Greek_omegaaccent ώ GREEK SMALL LETTER OMEGA WITH TONOS */
  { 0x07C1, 0x0391 }, /*                 Greek_ALPHA Α GREEK CAPITAL LETTER ALPHA */
  { 0x07C2, 0x0392 }, /*                  Greek_BETA Β GREEK CAPITAL LETTER BETA */
  { 0x07C3, 0x0393 }, /*                 Greek_GAMMA Γ GREEK CAPITAL LETTER GAMMA */
  { 0x07C4, 0x0394 }, /*                 Greek_DELTA Δ GREEK CAPITAL LETTER DELTA */
  { 0x07C5, 0x0395 }, /*               Greek_EPSILON Ε GREEK CAPITAL LETTER EPSILON */
  { 0x07C6, 0x0396 }, /*                  Greek_ZETA Ζ GREEK CAPITAL LETTER ZETA */
  { 0x07C7, 0x0397 }, /*                   Greek_ETA Η GREEK CAPITAL LETTER ETA */
  { 0x07C8, 0x0398 }, /*                 Greek_THETA Θ GREEK CAPITAL LETTER THETA */
  { 0x07C9, 0x0399 }, /*                  Greek_IOTA Ι GREEK CAPITAL LETTER IOTA */
  { 0x07CA, 0x039A }, /*                 Greek_KAPPA Κ GREEK CAPITAL LETTER KAPPA */
  { 0x07CB, 0x039B }, /*                Greek_LAMBDA Λ GREEK CAPITAL LETTER LAMDA */
  { 0x07CC, 0x039C }, /*                    Greek_MU Μ GREEK CAPITAL LETTER MU */
  { 0x07CD, 0x039D }, /*                    Greek_NU Ν GREEK CAPITAL LETTER NU */
  { 0x07CE, 0x039E }, /*                    Greek_XI Ξ GREEK CAPITAL LETTER XI */
  { 0x07CF, 0x039F }, /*               Greek_OMICRON Ο GREEK CAPITAL LETTER OMICRON */
  { 0x07D0, 0x03A0 }, /*                    Greek_PI Π GREEK CAPITAL LETTER PI */
  { 0x07D1, 0x03A1 }, /*                   Greek_RHO Ρ GREEK CAPITAL LETTER RHO */
  { 0x07D2, 0x03A3 }, /*                 Greek_SIGMA Σ GREEK CAPITAL LETTER SIGMA */
  { 0x07D4, 0x03A4 }, /*                   Greek_TAU Τ GREEK CAPITAL LETTER TAU */
  { 0x07D5, 0x03A5 }, /*               Greek_UPSILON Υ GREEK CAPITAL LETTER UPSILON */
  { 0x07D6, 0x03A6 }, /*                   Greek_PHI Φ GREEK CAPITAL LETTER PHI */
  { 0x07D7, 0x03A7 }, /*                   Greek_CHI Χ GREEK CAPITAL LETTER CHI */
  { 0x07D8, 0x03A8 }, /*                   Greek_PSI Ψ GREEK CAPITAL LETTER PSI */
  { 0x07D9, 0x03A9 }, /*                 Greek_OMEGA Ω GREEK CAPITAL LETTER OMEGA */
  { 0x07E1, 0x03B1 }, /*                 Greek_alpha α GREEK SMALL LETTER ALPHA */
  { 0x07E2, 0x03B2 }, /*                  Greek_beta β GREEK SMALL LETTER BETA */
  { 0x07E3, 0x03B3 }, /*                 Greek_gamma γ GREEK SMALL LETTER GAMMA */
  { 0x07E4, 0x03B4 }, /*                 Greek_delta δ GREEK SMALL LETTER DELTA */
  { 0x07E5, 0x03B5 }, /*               Greek_epsilon ε GREEK SMALL LETTER EPSILON */
  { 0x07E6, 0x03B6 }, /*                  Greek_zeta ζ GREEK SMALL LETTER ZETA */
  { 0x07E7, 0x03B7 }, /*                   Greek_eta η GREEK SMALL LETTER ETA */
  { 0x07E8, 0x03B8 }, /*                 Greek_theta θ GREEK SMALL LETTER THETA */
  { 0x07E9, 0x03B9 }, /*                  Greek_iota ι GREEK SMALL LETTER IOTA */
  { 0x07EA, 0x03BA }, /*                 Greek_kappa κ GREEK SMALL LETTER KAPPA */
  { 0x07EB, 0x03BB }, /*                Greek_lambda λ GREEK SMALL LETTER LAMDA */
  { 0x07EC, 0x03BC }, /*                    Greek_mu μ GREEK SMALL LETTER MU */
  { 0x07ED, 0x03BD }, /*                    Greek_nu ν GREEK SMALL LETTER NU */
  { 0x07EE, 0x03BE }, /*                    Greek_xi ξ GREEK SMALL LETTER XI */
  { 0x07EF, 0x03BF }, /*               Greek_omicron ο GREEK SMALL LETTER OMICRON */
  { 0x07F0, 0x03C0 }, /*                    Greek_pi π GREEK SMALL LETTER PI */
  { 0x07F1, 0x03C1 }, /*                   Greek_rho ρ GREEK SMALL LETTER RHO */
  { 0x07F2, 0x03C3 }, /*                 Greek_sigma σ GREEK SMALL LETTER SIGMA */
  { 0x07F3, 0x03C2 }, /*       Greek_finalsmallsigma ς GREEK SMALL LETTER FINAL SIGMA */
  { 0x07F4, 0x03C4 }, /*                   Greek_tau τ GREEK SMALL LETTER TAU */
  { 0x07F5, 0x03C5 }, /*               Greek_upsilon υ GREEK SMALL LETTER UPSILON */
  { 0x07F6, 0x03C6 }, /*                   Greek_phi φ GREEK SMALL LETTER PHI */
  { 0x07F7, 0x03C7 }, /*                   Greek_chi χ GREEK SMALL LETTER CHI */
  { 0x07F8, 0x03C8 }, /*                   Greek_psi ψ GREEK SMALL LETTER PSI */
  { 0x07F9, 0x03C9 }, /*                 Greek_omega ω GREEK SMALL LETTER OMEGA */
  { 0x08A1, 0x23B7 }, /*                 leftradical ⎷ ??? */
  { 0x08A2, 0x250C }, /*              topleftradical ┌ BOX DRAWINGS LIGHT DOWN AND RIGHT */
  { 0x08A3, 0x2500 }, /*              horizconnector ─ BOX DRAWINGS LIGHT HORIZONTAL */
  { 0x08A4, 0x2320 }, /*                 topintegral ⌠ TOP HALF INTEGRAL */
  { 0x08A5, 0x2321 }, /*                 botintegral ⌡ BOTTOM HALF INTEGRAL */
  { 0x08A6, 0x2502 }, /*               vertconnector │ BOX DRAWINGS LIGHT VERTICAL */
  { 0x08A7, 0x23A1 }, /*            topleftsqbracket ⎡ ??? */
  { 0x08A8, 0x23A3 }, /*            botleftsqbracket ⎣ ??? */
  { 0x08A9, 0x23A4 }, /*           toprightsqbracket ⎤ ??? */
  { 0x08AA, 0x23A6 }, /*           botrightsqbracket ⎦ ??? */
  { 0x08AB, 0x239B }, /*               topleftparens ⎛ ??? */
  { 0x08AC, 0x239D }, /*               botleftparens ⎝ ??? */
  { 0x08AD, 0x239E }, /*              toprightparens ⎞ ??? */
  { 0x08AE, 0x23A0 }, /*              botrightparens ⎠ ??? */
  { 0x08AF, 0x23A8 }, /*        leftmiddlecurlybrace ⎨ ??? */
  { 0x08B0, 0x23AC }, /*       rightmiddlecurlybrace ⎬ ??? */
/*  0x08B1                          topleftsummation ? ??? */
/*  0x08B2                          botleftsummation ? ??? */
/*  0x08B3                 topvertsummationconnector ? ??? */
/*  0x08B4                 botvertsummationconnector ? ??? */
/*  0x08B5                         toprightsummation ? ??? */
/*  0x08B6                         botrightsummation ? ??? */
/*  0x08B7                      rightmiddlesummation ? ??? */
  { 0x08BC, 0x2264 }, /*               lessthanequal ≤ LESS-THAN OR EQUAL TO */
  { 0x08BD, 0x2260 }, /*                    notequal ≠ NOT EQUAL TO */
  { 0x08BE, 0x2265 }, /*            greaterthanequal ≥ GREATER-THAN OR EQUAL TO */
  { 0x08BF, 0x222B }, /*                    integral ∫ INTEGRAL */
  { 0x08C0, 0x2234 }, /*                   therefore ∴ THEREFORE */
  { 0x08C1, 0x221D }, /*                   variation ∝ PROPORTIONAL TO */
  { 0x08C2, 0x221E }, /*                    infinity ∞ INFINITY */
  { 0x08C5, 0x2207 }, /*                       nabla ∇ NABLA */
  { 0x08C8, 0x223C }, /*                 approximate ∼ TILDE OPERATOR */
  { 0x08C9, 0x2243 }, /*                similarequal ≃ ASYMPTOTICALLY EQUAL TO */
  { 0x08CD, 0x21D4 }, /*                    ifonlyif ⇔ LEFT RIGHT DOUBLE ARROW */
  { 0x08CE, 0x21D2 }, /*                     implies ⇒ RIGHTWARDS DOUBLE ARROW */
  { 0x08CF, 0x2261 }, /*                   identical ≡ IDENTICAL TO */
  { 0x08D6, 0x221A }, /*                     radical √ SQUARE ROOT */
  { 0x08DA, 0x2282 }, /*                  includedin ⊂ SUBSET OF */
  { 0x08DB, 0x2283 }, /*                    includes ⊃ SUPERSET OF */
  { 0x08DC, 0x2229 }, /*                intersection ∩ INTERSECTION */
  { 0x08DD, 0x222A }, /*                       union ∪ UNION */
  { 0x08DE, 0x2227 }, /*                  logicaland ∧ LOGICAL AND */
  { 0x08DF, 0x2228 }, /*                   logicalor ∨ LOGICAL OR */
  { 0x08EF, 0x2202 }, /*           partialderivative ∂ PARTIAL DIFFERENTIAL */
  { 0x08F6, 0x0192 }, /*                    function ƒ LATIN SMALL LETTER F WITH HOOK */
  { 0x08FB, 0x2190 }, /*                   leftarrow ← LEFTWARDS ARROW */
  { 0x08FC, 0x2191 }, /*                     uparrow ↑ UPWARDS ARROW */
  { 0x08FD, 0x2192 }, /*                  rightarrow → RIGHTWARDS ARROW */
  { 0x08FE, 0x2193 }, /*                   downarrow ↓ DOWNWARDS ARROW */
/*  0x09DF                                     blank ? ??? */
  { 0x09E0, 0x25C6 }, /*                soliddiamond ◆ BLACK DIAMOND */
  { 0x09E1, 0x2592 }, /*                checkerboard ▒ MEDIUM SHADE */
  { 0x09E2, 0x2409 }, /*                          ht ␉ SYMBOL FOR HORIZONTAL TABULATION */
  { 0x09E3, 0x240C }, /*                          ff ␌ SYMBOL FOR FORM FEED */
  { 0x09E4, 0x240D }, /*                          cr ␍ SYMBOL FOR CARRIAGE RETURN */
  { 0x09E5, 0x240A }, /*                          lf ␊ SYMBOL FOR LINE FEED */
  { 0x09E8, 0x2424 }, /*                          nl ␤ SYMBOL FOR NEWLINE */
  { 0x09E9, 0x240B }, /*                          vt ␋ SYMBOL FOR VERTICAL TABULATION */
  { 0x09EA, 0x2518 }, /*              lowrightcorner ┘ BOX DRAWINGS LIGHT UP AND LEFT */
  { 0x09EB, 0x2510 }, /*               uprightcorner ┐ BOX DRAWINGS LIGHT DOWN AND LEFT */
  { 0x09EC, 0x250C }, /*                upleftcorner ┌ BOX DRAWINGS LIGHT DOWN AND RIGHT */
  { 0x09ED, 0x2514 }, /*               lowleftcorner └ BOX DRAWINGS LIGHT UP AND RIGHT */
  { 0x09EE, 0x253C }, /*               crossinglines ┼ BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL */
  { 0x09EF, 0x23BA }, /*              horizlinescan1 ⎺ HORIZONTAL SCAN LINE-1 (Unicode 3.2 draft) */
  { 0x09F0, 0x23BB }, /*              horizlinescan3 ⎻ HORIZONTAL SCAN LINE-3 (Unicode 3.2 draft) */
  { 0x09F1, 0x2500 }, /*              horizlinescan5 ─ BOX DRAWINGS LIGHT HORIZONTAL */
  { 0x09F2, 0x23BC }, /*              horizlinescan7 ⎼ HORIZONTAL SCAN LINE-7 (Unicode 3.2 draft) */
  { 0x09F3, 0x23BD }, /*              horizlinescan9 ⎽ HORIZONTAL SCAN LINE-9 (Unicode 3.2 draft) */
  { 0x09F4, 0x251C }, /*                       leftt ├ BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
  { 0x09F5, 0x2524 }, /*                      rightt ┤ BOX DRAWINGS LIGHT VERTICAL AND LEFT */
  { 0x09F6, 0x2534 }, /*                        bott ┴ BOX DRAWINGS LIGHT UP AND HORIZONTAL */
  { 0x09F7, 0x252C }, /*                        topt ┬ BOX DRAWINGS LIGHT DOWN AND HORIZONTAL */
  { 0x09F8, 0x2502 }, /*                     vertbar │ BOX DRAWINGS LIGHT VERTICAL */
  { 0x0AA1, 0x2003 }, /*                     emspace   EM SPACE */
  { 0x0AA2, 0x2002 }, /*                     enspace   EN SPACE */
  { 0x0AA3, 0x2004 }, /*                    em3space   THREE-PER-EM SPACE */
  { 0x0AA4, 0x2005 }, /*                    em4space   FOUR-PER-EM SPACE */
  { 0x0AA5, 0x2007 }, /*                  digitspace   FIGURE SPACE */
  { 0x0AA6, 0x2008 }, /*                  punctspace   PUNCTUATION SPACE */
  { 0x0AA7, 0x2009 }, /*                   thinspace   THIN SPACE */
  { 0x0AA8, 0x200A }, /*                   hairspace   HAIR SPACE */
  { 0x0AA9, 0x2014 }, /*                      emdash — EM DASH */
  { 0x0AAA, 0x2013 }, /*                      endash – EN DASH */
/*  0x0AAC                               signifblank ? ??? */
  { 0x0AAE, 0x2026 }, /*                    ellipsis … HORIZONTAL ELLIPSIS */
  { 0x0AAF, 0x2025 }, /*             doubbaselinedot ‥ TWO DOT LEADER */
  { 0x0AB0, 0x2153 }, /*                    onethird ⅓ VULGAR FRACTION ONE THIRD */
  { 0x0AB1, 0x2154 }, /*                   twothirds ⅔ VULGAR FRACTION TWO THIRDS */
  { 0x0AB2, 0x2155 }, /*                    onefifth ⅕ VULGAR FRACTION ONE FIFTH */
  { 0x0AB3, 0x2156 }, /*                   twofifths ⅖ VULGAR FRACTION TWO FIFTHS */
  { 0x0AB4, 0x2157 }, /*                 threefifths ⅗ VULGAR FRACTION THREE FIFTHS */
  { 0x0AB5, 0x2158 }, /*                  fourfifths ⅘ VULGAR FRACTION FOUR FIFTHS */
  { 0x0AB6, 0x2159 }, /*                    onesixth ⅙ VULGAR FRACTION ONE SIXTH */
  { 0x0AB7, 0x215A }, /*                  fivesixths ⅚ VULGAR FRACTION FIVE SIXTHS */
  { 0x0AB8, 0x2105 }, /*                      careof ℅ CARE OF */
  { 0x0ABB, 0x2012 }, /*                     figdash ‒ FIGURE DASH */
  { 0x0ABC, 0x2329 }, /*            leftanglebracket 〈 LEFT-POINTING ANGLE BRACKET */
/*  0x0ABD                              decimalpoint ? ??? */
  { 0x0ABE, 0x232A }, /*           rightanglebracket 〉 RIGHT-POINTING ANGLE BRACKET */
/*  0x0ABF                                    marker ? ??? */
  { 0x0AC3, 0x215B }, /*                   oneeighth ⅛ VULGAR FRACTION ONE EIGHTH */
  { 0x0AC4, 0x215C }, /*                threeeighths ⅜ VULGAR FRACTION THREE EIGHTHS */
  { 0x0AC5, 0x215D }, /*                 fiveeighths ⅝ VULGAR FRACTION FIVE EIGHTHS */
  { 0x0AC6, 0x215E }, /*                seveneighths ⅞ VULGAR FRACTION SEVEN EIGHTHS */
  { 0x0AC9, 0x2122 }, /*                   trademark ™ TRADE MARK SIGN */
  { 0x0ACA, 0x2613 }, /*               signaturemark ☓ SALTIRE */
/*  0x0ACB                         trademarkincircle ? ??? */
  { 0x0ACC, 0x25C1 }, /*            leftopentriangle ◁ WHITE LEFT-POINTING TRIANGLE */
  { 0x0ACD, 0x25B7 }, /*           rightopentriangle ▷ WHITE RIGHT-POINTING TRIANGLE */
  { 0x0ACE, 0x25CB }, /*                emopencircle ○ WHITE CIRCLE */
  { 0x0ACF, 0x25AF }, /*             emopenrectangle ▯ WHITE VERTICAL RECTANGLE */
  { 0x0AD0, 0x2018 }, /*         leftsinglequotemark ‘ LEFT SINGLE QUOTATION MARK */
  { 0x0AD1, 0x2019 }, /*        rightsinglequotemark ’ RIGHT SINGLE QUOTATION MARK */
  { 0x0AD2, 0x201C }, /*         leftdoublequotemark “ LEFT DOUBLE QUOTATION MARK */
  { 0x0AD3, 0x201D }, /*        rightdoublequotemark ” RIGHT DOUBLE QUOTATION MARK */
  { 0x0AD4, 0x211E }, /*                prescription ℞ PRESCRIPTION TAKE */
  { 0x0AD6, 0x2032 }, /*                     minutes ′ PRIME */
  { 0x0AD7, 0x2033 }, /*                     seconds ″ DOUBLE PRIME */
  { 0x0AD9, 0x271D }, /*                  latincross ✝ LATIN CROSS */
/*  0x0ADA                                  hexagram ? ??? */
  { 0x0ADB, 0x25AC }, /*            filledrectbullet ▬ BLACK RECTANGLE */
  { 0x0ADC, 0x25C0 }, /*         filledlefttribullet ◀ BLACK LEFT-POINTING TRIANGLE */
  { 0x0ADD, 0x25B6 }, /*        filledrighttribullet ▶ BLACK RIGHT-POINTING TRIANGLE */
  { 0x0ADE, 0x25CF }, /*              emfilledcircle ● BLACK CIRCLE */
  { 0x0ADF, 0x25AE }, /*                emfilledrect ▮ BLACK VERTICAL RECTANGLE */
  { 0x0AE0, 0x25E6 }, /*            enopencircbullet ◦ WHITE BULLET */
  { 0x0AE1, 0x25AB }, /*          enopensquarebullet ▫ WHITE SMALL SQUARE */
  { 0x0AE2, 0x25AD }, /*              openrectbullet ▭ WHITE RECTANGLE */
  { 0x0AE3, 0x25B3 }, /*             opentribulletup △ WHITE UP-POINTING TRIANGLE */
  { 0x0AE4, 0x25BD }, /*           opentribulletdown ▽ WHITE DOWN-POINTING TRIANGLE */
  { 0x0AE5, 0x2606 }, /*                    openstar ☆ WHITE STAR */
  { 0x0AE6, 0x2022 }, /*          enfilledcircbullet • BULLET */
  { 0x0AE7, 0x25AA }, /*            enfilledsqbullet ▪ BLACK SMALL SQUARE */
  { 0x0AE8, 0x25B2 }, /*           filledtribulletup ▲ BLACK UP-POINTING TRIANGLE */
  { 0x0AE9, 0x25BC }, /*         filledtribulletdown ▼ BLACK DOWN-POINTING TRIANGLE */
  { 0x0AEA, 0x261C }, /*                 leftpointer ☜ WHITE LEFT POINTING INDEX */
  { 0x0AEB, 0x261E }, /*                rightpointer ☞ WHITE RIGHT POINTING INDEX */
  { 0x0AEC, 0x2663 }, /*                        club ♣ BLACK CLUB SUIT */
  { 0x0AED, 0x2666 }, /*                     diamond ♦ BLACK DIAMOND SUIT */
  { 0x0AEE, 0x2665 }, /*                       heart ♥ BLACK HEART SUIT */
  { 0x0AF0, 0x2720 }, /*                maltesecross ✠ MALTESE CROSS */
  { 0x0AF1, 0x2020 }, /*                      dagger † DAGGER */
  { 0x0AF2, 0x2021 }, /*                doubledagger ‡ DOUBLE DAGGER */
  { 0x0AF3, 0x2713 }, /*                   checkmark ✓ CHECK MARK */
  { 0x0AF4, 0x2717 }, /*                 ballotcross ✗ BALLOT X */
  { 0x0AF5, 0x266F }, /*                musicalsharp ♯ MUSIC SHARP SIGN */
  { 0x0AF6, 0x266D }, /*                 musicalflat ♭ MUSIC FLAT SIGN */
  { 0x0AF7, 0x2642 }, /*                  malesymbol ♂ MALE SIGN */
  { 0x0AF8, 0x2640 }, /*                femalesymbol ♀ FEMALE SIGN */
  { 0x0AF9, 0x260E }, /*                   telephone ☎ BLACK TELEPHONE */
  { 0x0AFA, 0x2315 }, /*           telephonerecorder ⌕ TELEPHONE RECORDER */
  { 0x0AFB, 0x2117 }, /*         phonographcopyright ℗ SOUND RECORDING COPYRIGHT */
  { 0x0AFC, 0x2038 }, /*                       caret ‸ CARET */
  { 0x0AFD, 0x201A }, /*          singlelowquotemark ‚ SINGLE LOW-9 QUOTATION MARK */
  { 0x0AFE, 0x201E }, /*          doublelowquotemark „ DOUBLE LOW-9 QUOTATION MARK */
/*  0x0AFF                                    cursor ? ??? */
  { 0x0BA3, 0x003C }, /*                   leftcaret < LESS-THAN SIGN */
  { 0x0BA6, 0x003E }, /*                  rightcaret > GREATER-THAN SIGN */
  { 0x0BA8, 0x2228 }, /*                   downcaret ∨ LOGICAL OR */
  { 0x0BA9, 0x2227 }, /*                     upcaret ∧ LOGICAL AND */
  { 0x0BC0, 0x00AF }, /*                     overbar ¯ MACRON */
  { 0x0BC2, 0x22A5 }, /*                    downtack ⊥ UP TACK */
  { 0x0BC3, 0x2229 }, /*                      upshoe ∩ INTERSECTION */
  { 0x0BC4, 0x230A }, /*                   downstile ⌊ LEFT FLOOR */
  { 0x0BC6, 0x005F }, /*                    underbar _ LOW LINE */
  { 0x0BCA, 0x2218 }, /*                         jot ∘ RING OPERATOR */
  { 0x0BCC, 0x2395 }, /*                        quad ⎕ APL FUNCTIONAL SYMBOL QUAD */
  { 0x0BCE, 0x22A4 }, /*                      uptack ⊤ DOWN TACK */
  { 0x0BCF, 0x25CB }, /*                      circle ○ WHITE CIRCLE */
  { 0x0BD3, 0x2308 }, /*                     upstile ⌈ LEFT CEILING */
  { 0x0BD6, 0x222A }, /*                    downshoe ∪ UNION */
  { 0x0BD8, 0x2283 }, /*                   rightshoe ⊃ SUPERSET OF */
  { 0x0BDA, 0x2282 }, /*                    leftshoe ⊂ SUBSET OF */
  { 0x0BDC, 0x22A2 }, /*                    lefttack ⊢ RIGHT TACK */
  { 0x0BFC, 0x22A3 }, /*                   righttack ⊣ LEFT TACK */
  { 0x0CDF, 0x2017 }, /*        hebrew_doublelowline ‗ DOUBLE LOW LINE */
  { 0x0CE0, 0x05D0 }, /*                hebrew_aleph א HEBREW LETTER ALEF */
  { 0x0CE1, 0x05D1 }, /*                  hebrew_bet ב HEBREW LETTER BET */
  { 0x0CE2, 0x05D2 }, /*                hebrew_gimel ג HEBREW LETTER GIMEL */
  { 0x0CE3, 0x05D3 }, /*                hebrew_dalet ד HEBREW LETTER DALET */
  { 0x0CE4, 0x05D4 }, /*                   hebrew_he ה HEBREW LETTER HE */
  { 0x0CE5, 0x05D5 }, /*                  hebrew_waw ו HEBREW LETTER VAV */
  { 0x0CE6, 0x05D6 }, /*                 hebrew_zain ז HEBREW LETTER ZAYIN */
  { 0x0CE7, 0x05D7 }, /*                 hebrew_chet ח HEBREW LETTER HET */
  { 0x0CE8, 0x05D8 }, /*                  hebrew_tet ט HEBREW LETTER TET */
  { 0x0CE9, 0x05D9 }, /*                  hebrew_yod י HEBREW LETTER YOD */
  { 0x0CEA, 0x05DA }, /*            hebrew_finalkaph ך HEBREW LETTER FINAL KAF */
  { 0x0CEB, 0x05DB }, /*                 hebrew_kaph כ HEBREW LETTER KAF */
  { 0x0CEC, 0x05DC }, /*                hebrew_lamed ל HEBREW LETTER LAMED */
  { 0x0CED, 0x05DD }, /*             hebrew_finalmem ם HEBREW LETTER FINAL MEM */
  { 0x0CEE, 0x05DE }, /*                  hebrew_mem מ HEBREW LETTER MEM */
  { 0x0CEF, 0x05DF }, /*             hebrew_finalnun ן HEBREW LETTER FINAL NUN */
  { 0x0CF0, 0x05E0 }, /*                  hebrew_nun נ HEBREW LETTER NUN */
  { 0x0CF1, 0x05E1 }, /*               hebrew_samech ס HEBREW LETTER SAMEKH */
  { 0x0CF2, 0x05E2 }, /*                 hebrew_ayin ע HEBREW LETTER AYIN */
  { 0x0CF3, 0x05E3 }, /*              hebrew_finalpe ף HEBREW LETTER FINAL PE */
  { 0x0CF4, 0x05E4 }, /*                   hebrew_pe פ HEBREW LETTER PE */
  { 0x0CF5, 0x05E5 }, /*            hebrew_finalzade ץ HEBREW LETTER FINAL TSADI */
  { 0x0CF6, 0x05E6 }, /*                 hebrew_zade צ HEBREW LETTER TSADI */
  { 0x0CF7, 0x05E7 }, /*                 hebrew_qoph ק HEBREW LETTER QOF */
  { 0x0CF8, 0x05E8 }, /*                 hebrew_resh ר HEBREW LETTER RESH */
  { 0x0CF9, 0x05E9 }, /*                 hebrew_shin ש HEBREW LETTER SHIN */
  { 0x0CFA, 0x05EA }, /*                  hebrew_taw ת HEBREW LETTER TAV */
  { 0x0DA1, 0x0E01 }, /*                  Thai_kokai ก THAI CHARACTER KO KAI */
  { 0x0DA2, 0x0E02 }, /*                Thai_khokhai ข THAI CHARACTER KHO KHAI */
  { 0x0DA3, 0x0E03 }, /*               Thai_khokhuat ฃ THAI CHARACTER KHO KHUAT */
  { 0x0DA4, 0x0E04 }, /*               Thai_khokhwai ค THAI CHARACTER KHO KHWAI */
  { 0x0DA5, 0x0E05 }, /*                Thai_khokhon ฅ THAI CHARACTER KHO KHON */
  { 0x0DA6, 0x0E06 }, /*             Thai_khorakhang ฆ THAI CHARACTER KHO RAKHANG */
  { 0x0DA7, 0x0E07 }, /*                 Thai_ngongu ง THAI CHARACTER NGO NGU */
  { 0x0DA8, 0x0E08 }, /*                Thai_chochan จ THAI CHARACTER CHO CHAN */
  { 0x0DA9, 0x0E09 }, /*               Thai_choching ฉ THAI CHARACTER CHO CHING */
  { 0x0DAA, 0x0E0A }, /*               Thai_chochang ช THAI CHARACTER CHO CHANG */
  { 0x0DAB, 0x0E0B }, /*                   Thai_soso ซ THAI CHARACTER SO SO */
  { 0x0DAC, 0x0E0C }, /*                Thai_chochoe ฌ THAI CHARACTER CHO CHOE */
  { 0x0DAD, 0x0E0D }, /*                 Thai_yoying ญ THAI CHARACTER YO YING */
  { 0x0DAE, 0x0E0E }, /*                Thai_dochada ฎ THAI CHARACTER DO CHADA */
  { 0x0DAF, 0x0E0F }, /*                Thai_topatak ฏ THAI CHARACTER TO PATAK */
  { 0x0DB0, 0x0E10 }, /*                Thai_thothan ฐ THAI CHARACTER THO THAN */
  { 0x0DB1, 0x0E11 }, /*          Thai_thonangmontho ฑ THAI CHARACTER THO NANGMONTHO */
  { 0x0DB2, 0x0E12 }, /*             Thai_thophuthao ฒ THAI CHARACTER THO PHUTHAO */
  { 0x0DB3, 0x0E13 }, /*                  Thai_nonen ณ THAI CHARACTER NO NEN */
  { 0x0DB4, 0x0E14 }, /*                  Thai_dodek ด THAI CHARACTER DO DEK */
  { 0x0DB5, 0x0E15 }, /*                  Thai_totao ต THAI CHARACTER TO TAO */
  { 0x0DB6, 0x0E16 }, /*               Thai_thothung ถ THAI CHARACTER THO THUNG */
  { 0x0DB7, 0x0E17 }, /*              Thai_thothahan ท THAI CHARACTER THO THAHAN */
  { 0x0DB8, 0x0E18 }, /*               Thai_thothong ธ THAI CHARACTER THO THONG */
  { 0x0DB9, 0x0E19 }, /*                   Thai_nonu น THAI CHARACTER NO NU */
  { 0x0DBA, 0x0E1A }, /*               Thai_bobaimai บ THAI CHARACTER BO BAIMAI */
  { 0x0DBB, 0x0E1B }, /*                  Thai_popla ป THAI CHARACTER PO PLA */
  { 0x0DBC, 0x0E1C }, /*               Thai_phophung ผ THAI CHARACTER PHO PHUNG */
  { 0x0DBD, 0x0E1D }, /*                   Thai_fofa ฝ THAI CHARACTER FO FA */
  { 0x0DBE, 0x0E1E }, /*                Thai_phophan พ THAI CHARACTER PHO PHAN */
  { 0x0DBF, 0x0E1F }, /*                  Thai_fofan ฟ THAI CHARACTER FO FAN */
  { 0x0DC0, 0x0E20 }, /*             Thai_phosamphao ภ THAI CHARACTER PHO SAMPHAO */
  { 0x0DC1, 0x0E21 }, /*                   Thai_moma ม THAI CHARACTER MO MA */
  { 0x0DC2, 0x0E22 }, /*                  Thai_yoyak ย THAI CHARACTER YO YAK */
  { 0x0DC3, 0x0E23 }, /*                  Thai_rorua ร THAI CHARACTER RO RUA */
  { 0x0DC4, 0x0E24 }, /*                     Thai_ru ฤ THAI CHARACTER RU */
  { 0x0DC5, 0x0E25 }, /*                 Thai_loling ล THAI CHARACTER LO LING */
  { 0x0DC6, 0x0E26 }, /*                     Thai_lu ฦ THAI CHARACTER LU */
  { 0x0DC7, 0x0E27 }, /*                 Thai_wowaen ว THAI CHARACTER WO WAEN */
  { 0x0DC8, 0x0E28 }, /*                 Thai_sosala ศ THAI CHARACTER SO SALA */
  { 0x0DC9, 0x0E29 }, /*                 Thai_sorusi ษ THAI CHARACTER SO RUSI */
  { 0x0DCA, 0x0E2A }, /*                  Thai_sosua ส THAI CHARACTER SO SUA */
  { 0x0DCB, 0x0E2B }, /*                  Thai_hohip ห THAI CHARACTER HO HIP */
  { 0x0DCC, 0x0E2C }, /*                Thai_lochula ฬ THAI CHARACTER LO CHULA */
  { 0x0DCD, 0x0E2D }, /*                   Thai_oang อ THAI CHARACTER O ANG */
  { 0x0DCE, 0x0E2E }, /*               Thai_honokhuk ฮ THAI CHARACTER HO NOKHUK */
  { 0x0DCF, 0x0E2F }, /*              Thai_paiyannoi ฯ THAI CHARACTER PAIYANNOI */
  { 0x0DD0, 0x0E30 }, /*                  Thai_saraa ะ THAI CHARACTER SARA A */
  { 0x0DD1, 0x0E31 }, /*             Thai_maihanakat ั THAI CHARACTER MAI HAN-AKAT */
  { 0x0DD2, 0x0E32 }, /*                 Thai_saraaa า THAI CHARACTER SARA AA */
  { 0x0DD3, 0x0E33 }, /*                 Thai_saraam ำ THAI CHARACTER SARA AM */
  { 0x0DD4, 0x0E34 }, /*                  Thai_sarai ิ THAI CHARACTER SARA I */
  { 0x0DD5, 0x0E35 }, /*                 Thai_saraii ี THAI CHARACTER SARA II */
  { 0x0DD6, 0x0E36 }, /*                 Thai_saraue ึ THAI CHARACTER SARA UE */
  { 0x0DD7, 0x0E37 }, /*                Thai_sarauee ื THAI CHARACTER SARA UEE */
  { 0x0DD8, 0x0E38 }, /*                  Thai_sarau ุ THAI CHARACTER SARA U */
  { 0x0DD9, 0x0E39 }, /*                 Thai_sarauu ู THAI CHARACTER SARA UU */
  { 0x0DDA, 0x0E3A }, /*                Thai_phinthu ฺ THAI CHARACTER PHINTHU */
/*  0x0DDE                    Thai_maihanakat_maitho ? ??? */
  { 0x0DDF, 0x0E3F }, /*                   Thai_baht ฿ THAI CURRENCY SYMBOL BAHT */
  { 0x0DE0, 0x0E40 }, /*                  Thai_sarae เ THAI CHARACTER SARA E */
  { 0x0DE1, 0x0E41 }, /*                 Thai_saraae แ THAI CHARACTER SARA AE */
  { 0x0DE2, 0x0E42 }, /*                  Thai_sarao โ THAI CHARACTER SARA O */
  { 0x0DE3, 0x0E43 }, /*          Thai_saraaimaimuan ใ THAI CHARACTER SARA AI MAIMUAN */
  { 0x0DE4, 0x0E44 }, /*         Thai_saraaimaimalai ไ THAI CHARACTER SARA AI MAIMALAI */
  { 0x0DE5, 0x0E45 }, /*            Thai_lakkhangyao ๅ THAI CHARACTER LAKKHANGYAO */
  { 0x0DE6, 0x0E46 }, /*               Thai_maiyamok ๆ THAI CHARACTER MAIYAMOK */
  { 0x0DE7, 0x0E47 }, /*              Thai_maitaikhu ็ THAI CHARACTER MAITAIKHU */
  { 0x0DE8, 0x0E48 }, /*                  Thai_maiek ่ THAI CHARACTER MAI EK */
  { 0x0DE9, 0x0E49 }, /*                 Thai_maitho ้ THAI CHARACTER MAI THO */
  { 0x0DEA, 0x0E4A }, /*                 Thai_maitri ๊ THAI CHARACTER MAI TRI */
  { 0x0DEB, 0x0E4B }, /*            Thai_maichattawa ๋ THAI CHARACTER MAI CHATTAWA */
  { 0x0DEC, 0x0E4C }, /*            Thai_thanthakhat ์ THAI CHARACTER THANTHAKHAT */
  { 0x0DED, 0x0E4D }, /*               Thai_nikhahit ํ THAI CHARACTER NIKHAHIT */
  { 0x0DF0, 0x0E50 }, /*                 Thai_leksun ๐ THAI DIGIT ZERO */
  { 0x0DF1, 0x0E51 }, /*                Thai_leknung ๑ THAI DIGIT ONE */
  { 0x0DF2, 0x0E52 }, /*                Thai_leksong ๒ THAI DIGIT TWO */
  { 0x0DF3, 0x0E53 }, /*                 Thai_leksam ๓ THAI DIGIT THREE */
  { 0x0DF4, 0x0E54 }, /*                  Thai_leksi ๔ THAI DIGIT FOUR */
  { 0x0DF5, 0x0E55 }, /*                  Thai_lekha ๕ THAI DIGIT FIVE */
  { 0x0DF6, 0x0E56 }, /*                 Thai_lekhok ๖ THAI DIGIT SIX */
  { 0x0DF7, 0x0E57 }, /*                Thai_lekchet ๗ THAI DIGIT SEVEN */
  { 0x0DF8, 0x0E58 }, /*                Thai_lekpaet ๘ THAI DIGIT EIGHT */
  { 0x0DF9, 0x0E59 }, /*                 Thai_lekkao ๙ THAI DIGIT NINE */
  { 0x0EA1, 0x3131 }, /*               Hangul_Kiyeog ㄱ HANGUL LETTER KIYEOK */
  { 0x0EA2, 0x3132 }, /*          Hangul_SsangKiyeog ㄲ HANGUL LETTER SSANGKIYEOK */
  { 0x0EA3, 0x3133 }, /*           Hangul_KiyeogSios ㄳ HANGUL LETTER KIYEOK-SIOS */
  { 0x0EA4, 0x3134 }, /*                Hangul_Nieun ㄴ HANGUL LETTER NIEUN */
  { 0x0EA5, 0x3135 }, /*           Hangul_NieunJieuj ㄵ HANGUL LETTER NIEUN-CIEUC */
  { 0x0EA6, 0x3136 }, /*           Hangul_NieunHieuh ㄶ HANGUL LETTER NIEUN-HIEUH */
  { 0x0EA7, 0x3137 }, /*               Hangul_Dikeud ㄷ HANGUL LETTER TIKEUT */
  { 0x0EA8, 0x3138 }, /*          Hangul_SsangDikeud ㄸ HANGUL LETTER SSANGTIKEUT */
  { 0x0EA9, 0x3139 }, /*                Hangul_Rieul ㄹ HANGUL LETTER RIEUL */
  { 0x0EAA, 0x313A }, /*          Hangul_RieulKiyeog ㄺ HANGUL LETTER RIEUL-KIYEOK */
  { 0x0EAB, 0x313B }, /*           Hangul_RieulMieum ㄻ HANGUL LETTER RIEUL-MIEUM */
  { 0x0EAC, 0x313C }, /*           Hangul_RieulPieub ㄼ HANGUL LETTER RIEUL-PIEUP */
  { 0x0EAD, 0x313D }, /*            Hangul_RieulSios ㄽ HANGUL LETTER RIEUL-SIOS */
  { 0x0EAE, 0x313E }, /*           Hangul_RieulTieut ㄾ HANGUL LETTER RIEUL-THIEUTH */
  { 0x0EAF, 0x313F }, /*          Hangul_RieulPhieuf ㄿ HANGUL LETTER RIEUL-PHIEUPH */
  { 0x0EB0, 0x3140 }, /*           Hangul_RieulHieuh ㅀ HANGUL LETTER RIEUL-HIEUH */
  { 0x0EB1, 0x3141 }, /*                Hangul_Mieum ㅁ HANGUL LETTER MIEUM */
  { 0x0EB2, 0x3142 }, /*                Hangul_Pieub ㅂ HANGUL LETTER PIEUP */
  { 0x0EB3, 0x3143 }, /*           Hangul_SsangPieub ㅃ HANGUL LETTER SSANGPIEUP */
  { 0x0EB4, 0x3144 }, /*            Hangul_PieubSios ㅄ HANGUL LETTER PIEUP-SIOS */
  { 0x0EB5, 0x3145 }, /*                 Hangul_Sios ㅅ HANGUL LETTER SIOS */
  { 0x0EB6, 0x3146 }, /*            Hangul_SsangSios ㅆ HANGUL LETTER SSANGSIOS */
  { 0x0EB7, 0x3147 }, /*                Hangul_Ieung ㅇ HANGUL LETTER IEUNG */
  { 0x0EB8, 0x3148 }, /*                Hangul_Jieuj ㅈ HANGUL LETTER CIEUC */
  { 0x0EB9, 0x3149 }, /*           Hangul_SsangJieuj ㅉ HANGUL LETTER SSANGCIEUC */
  { 0x0EBA, 0x314A }, /*                Hangul_Cieuc ㅊ HANGUL LETTER CHIEUCH */
  { 0x0EBB, 0x314B }, /*               Hangul_Khieuq ㅋ HANGUL LETTER KHIEUKH */
  { 0x0EBC, 0x314C }, /*                Hangul_Tieut ㅌ HANGUL LETTER THIEUTH */
  { 0x0EBD, 0x314D }, /*               Hangul_Phieuf ㅍ HANGUL LETTER PHIEUPH */
  { 0x0EBE, 0x314E }, /*                Hangul_Hieuh ㅎ HANGUL LETTER HIEUH */
  { 0x0EBF, 0x314F }, /*                    Hangul_A ㅏ HANGUL LETTER A */
  { 0x0EC0, 0x3150 }, /*                   Hangul_AE ㅐ HANGUL LETTER AE */
  { 0x0EC1, 0x3151 }, /*                   Hangul_YA ㅑ HANGUL LETTER YA */
  { 0x0EC2, 0x3152 }, /*                  Hangul_YAE ㅒ HANGUL LETTER YAE */
  { 0x0EC3, 0x3153 }, /*                   Hangul_EO ㅓ HANGUL LETTER EO */
  { 0x0EC4, 0x3154 }, /*                    Hangul_E ㅔ HANGUL LETTER E */
  { 0x0EC5, 0x3155 }, /*                  Hangul_YEO ㅕ HANGUL LETTER YEO */
  { 0x0EC6, 0x3156 }, /*                   Hangul_YE ㅖ HANGUL LETTER YE */
  { 0x0EC7, 0x3157 }, /*                    Hangul_O ㅗ HANGUL LETTER O */
  { 0x0EC8, 0x3158 }, /*                   Hangul_WA ㅘ HANGUL LETTER WA */
  { 0x0EC9, 0x3159 }, /*                  Hangul_WAE ㅙ HANGUL LETTER WAE */
  { 0x0ECA, 0x315A }, /*                   Hangul_OE ㅚ HANGUL LETTER OE */
  { 0x0ECB, 0x315B }, /*                   Hangul_YO ㅛ HANGUL LETTER YO */
  { 0x0ECC, 0x315C }, /*                    Hangul_U ㅜ HANGUL LETTER U */
  { 0x0ECD, 0x315D }, /*                  Hangul_WEO ㅝ HANGUL LETTER WEO */
  { 0x0ECE, 0x315E }, /*                   Hangul_WE ㅞ HANGUL LETTER WE */
  { 0x0ECF, 0x315F }, /*                   Hangul_WI ㅟ HANGUL LETTER WI */
  { 0x0ED0, 0x3160 }, /*                   Hangul_YU ㅠ HANGUL LETTER YU */
  { 0x0ED1, 0x3161 }, /*                   Hangul_EU ㅡ HANGUL LETTER EU */
  { 0x0ED2, 0x3162 }, /*                   Hangul_YI ㅢ HANGUL LETTER YI */
  { 0x0ED3, 0x3163 }, /*                    Hangul_I ㅣ HANGUL LETTER I */
  { 0x0ED4, 0x11A8 }, /*             Hangul_J_Kiyeog ᆨ HANGUL JONGSEONG KIYEOK */
  { 0x0ED5, 0x11A9 }, /*        Hangul_J_SsangKiyeog ᆩ HANGUL JONGSEONG SSANGKIYEOK */
  { 0x0ED6, 0x11AA }, /*         Hangul_J_KiyeogSios ᆪ HANGUL JONGSEONG KIYEOK-SIOS */
  { 0x0ED7, 0x11AB }, /*              Hangul_J_Nieun ᆫ HANGUL JONGSEONG NIEUN */
  { 0x0ED8, 0x11AC }, /*         Hangul_J_NieunJieuj ᆬ HANGUL JONGSEONG NIEUN-CIEUC */
  { 0x0ED9, 0x11AD }, /*         Hangul_J_NieunHieuh ᆭ HANGUL JONGSEONG NIEUN-HIEUH */
  { 0x0EDA, 0x11AE }, /*             Hangul_J_Dikeud ᆮ HANGUL JONGSEONG TIKEUT */
  { 0x0EDB, 0x11AF }, /*              Hangul_J_Rieul ᆯ HANGUL JONGSEONG RIEUL */
  { 0x0EDC, 0x11B0 }, /*        Hangul_J_RieulKiyeog ᆰ HANGUL JONGSEONG RIEUL-KIYEOK */
  { 0x0EDD, 0x11B1 }, /*         Hangul_J_RieulMieum ᆱ HANGUL JONGSEONG RIEUL-MIEUM */
  { 0x0EDE, 0x11B2 }, /*         Hangul_J_RieulPieub ᆲ HANGUL JONGSEONG RIEUL-PIEUP */
  { 0x0EDF, 0x11B3 }, /*          Hangul_J_RieulSios ᆳ HANGUL JONGSEONG RIEUL-SIOS */
  { 0x0EE0, 0x11B4 }, /*         Hangul_J_RieulTieut ᆴ HANGUL JONGSEONG RIEUL-THIEUTH */
  { 0x0EE1, 0x11B5 }, /*        Hangul_J_RieulPhieuf ᆵ HANGUL JONGSEONG RIEUL-PHIEUPH */
  { 0x0EE2, 0x11B6 }, /*         Hangul_J_RieulHieuh ᆶ HANGUL JONGSEONG RIEUL-HIEUH */
  { 0x0EE3, 0x11B7 }, /*              Hangul_J_Mieum ᆷ HANGUL JONGSEONG MIEUM */
  { 0x0EE4, 0x11B8 }, /*              Hangul_J_Pieub ᆸ HANGUL JONGSEONG PIEUP */
  { 0x0EE5, 0x11B9 }, /*          Hangul_J_PieubSios ᆹ HANGUL JONGSEONG PIEUP-SIOS */
  { 0x0EE6, 0x11BA }, /*               Hangul_J_Sios ᆺ HANGUL JONGSEONG SIOS */
  { 0x0EE7, 0x11BB }, /*          Hangul_J_SsangSios ᆻ HANGUL JONGSEONG SSANGSIOS */
  { 0x0EE8, 0x11BC }, /*              Hangul_J_Ieung ᆼ HANGUL JONGSEONG IEUNG */
  { 0x0EE9, 0x11BD }, /*              Hangul_J_Jieuj ᆽ HANGUL JONGSEONG CIEUC */
  { 0x0EEA, 0x11BE }, /*              Hangul_J_Cieuc ᆾ HANGUL JONGSEONG CHIEUCH */
  { 0x0EEB, 0x11BF }, /*             Hangul_J_Khieuq ᆿ HANGUL JONGSEONG KHIEUKH */
  { 0x0EEC, 0x11C0 }, /*              Hangul_J_Tieut ᇀ HANGUL JONGSEONG THIEUTH */
  { 0x0EED, 0x11C1 }, /*             Hangul_J_Phieuf ᇁ HANGUL JONGSEONG PHIEUPH */
  { 0x0EEE, 0x11C2 }, /*              Hangul_J_Hieuh ᇂ HANGUL JONGSEONG HIEUH */
  { 0x0EEF, 0x316D }, /*     Hangul_RieulYeorinHieuh ㅭ HANGUL LETTER RIEUL-YEORINHIEUH */
  { 0x0EF0, 0x3171 }, /*    Hangul_SunkyeongeumMieum ㅱ HANGUL LETTER KAPYEOUNMIEUM */
  { 0x0EF1, 0x3178 }, /*    Hangul_SunkyeongeumPieub ㅸ HANGUL LETTER KAPYEOUNPIEUP */
  { 0x0EF2, 0x317F }, /*              Hangul_PanSios ㅿ HANGUL LETTER PANSIOS */
  { 0x0EF3, 0x3181 }, /*    Hangul_KkogjiDalrinIeung ㆁ HANGUL LETTER YESIEUNG */
  { 0x0EF4, 0x3184 }, /*   Hangul_SunkyeongeumPhieuf ㆄ HANGUL LETTER KAPYEOUNPHIEUPH */
  { 0x0EF5, 0x3186 }, /*          Hangul_YeorinHieuh ㆆ HANGUL LETTER YEORINHIEUH */
  { 0x0EF6, 0x318D }, /*                Hangul_AraeA ㆍ HANGUL LETTER ARAEA */
  { 0x0EF7, 0x318E }, /*               Hangul_AraeAE ㆎ HANGUL LETTER ARAEAE */
  { 0x0EF8, 0x11EB }, /*            Hangul_J_PanSios ᇫ HANGUL JONGSEONG PANSIOS */
  { 0x0EF9, 0x11F0 }, /*  Hangul_J_KkogjiDalrinIeung ᇰ HANGUL JONGSEONG YESIEUNG */
  { 0x0EFA, 0x11F9 }, /*        Hangul_J_YeorinHieuh ᇹ HANGUL JONGSEONG YEORINHIEUH */
  { 0x0EFF, 0x20A9 }, /*                  Korean_Won ₩ WON SIGN */
  { 0x13A4, 0x20AC }, /*                        Euro € EURO SIGN */
  { 0x13BC, 0x0152 }, /*                          OE Œ LATIN CAPITAL LIGATURE OE */
  { 0x13BD, 0x0153 }, /*                          oe œ LATIN SMALL LIGATURE OE */
  { 0x13BE, 0x0178 }, /*                  Ydiaeresis Ÿ LATIN CAPITAL LETTER Y WITH DIAERESIS */
  { 0x20AC, 0x20AC }, /*                    EuroSign € EURO SIGN */
};

KeySym unicode_to_keysym(wchar_t ucs) {
	int min = 0;
	int max = sizeof(keysym_unicode_table) / sizeof(struct codepair) - 1;
	int mid;

	#ifdef XK_LATIN1
	// First check for Latin-1 characters. (1:1 mapping)
	if ((ucs >= 0x0020 && ucs <= 0x007E) ||
		(ucs >= 0x00A0 && ucs <= 0x00FF)) {
		return ucs;
	}
	#endif

	// Binary search the lookup table.
	while (max >= min) {
		mid = (min + max) / 2;
		if (keysym_unicode_table[mid].ucs < ucs) {
			min = mid + 1;
		}
		else if (keysym_unicode_table[mid].ucs > ucs) {
			max = mid - 1;
		}
		else {
			// Found it.
			return keysym_unicode_table[mid].keysym;
		}
	}

	// No matching KeySym value found, return UCS2 with bit set.
	return ucs | 0x01000000;
}

wchar_t keysym_to_unicode(KeySym keysym) {
	int min = 0;
	int max = sizeof(keysym_unicode_table) / sizeof(struct codepair) - 1;
	int mid;

	#ifdef XK_LATIN1
    // First check for Latin-1 characters. (1:1 mapping)
	if ((keysym >= 0x0020 && keysym <= 0x007E) ||
		(keysym >= 0x00A0 && keysym <= 0x00FF)) {
		return keysym;
	}
	#endif

	// Also check for directly encoded 24-bit UCS characters.
	#if defined(XK_LATIN8) || defined(XK_ARABIC) || defined(XK_CYRILLIC) || \
		defined(XK_ARMENIAN) || defined(XK_GEORGIAN) || defined(XK_CAUCASUS) || \
		defined(XK_VIETNAMESE) || defined(XK_CURRENCY) || \
		defined(XK_MATHEMATICAL) || defined(XK_BRAILLE) || defined(XK_SINHALA)
    if ((keysym & 0xFF000000) == 0x01000000) {
		return keysym & 0x00FFFFFF;
	}
	#endif

    // Binary search in table.
    while (max >= min) {
		mid = (min + max) / 2;
		if (keysym_unicode_table[mid].keysym < keysym) {
			min = mid + 1;
		}
		else if (keysym_unicode_table[mid].keysym > keysym) {
			max = mid - 1;
		}
		else {
			// Found it.
			return keysym_unicode_table[mid].ucs;
		}
    }

    // No matching Unicode value found!
    return 0x0000;
}


/* Mapping of X11 keysyms to ISO 10646 (Universal Character Set) for the Basic
 * Multilingual Plane. This function was created from work done by Markus Kuhn
 * and Richard Verhoeven.
 * See http://www.cl.cam.ac.uk/~mgk25/ for more information.
 */
wchar_t keysym_to_unicode_old(KeySym keysym) {
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
