/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
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

#ifndef _included_x_unicode_helper
#define _included_x_unicode_helper

#include <stdint.h>
#include <X11/Xlib.h>

/* Converts an X11 key symbol to a single Unicode character.  No direct X11
 * functionality exists to provide this information.
 */
extern wchar_t keysym_to_unicode(KeySym keysym);
extern wchar_t keysym_to_unicode_old(KeySym keysym);

/* Convert a single Unicode character to an X11 key symbol.  This function
 * provides a better translation than XStringToKeysym() for Unicode characters.
 */
extern KeySym unicode_to_keysym(wchar_t unicode);

#endif
