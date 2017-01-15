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

#ifndef _Included_WinInputHelpers
#define _Included_WinInputHelpers

#include <stdbool.h>
#include <limits.h>

/* Some versions of MinGW do not include the extended East Asian Input Method
 * virtual keyboard definitions.
 */

#ifndef VK_DBE_ALPHANUMERIC
#define VK_DBE_ALPHANUMERIC				0x0F0		// Changes the mode to alphanumeric.
#define VK_DBE_KATAKANA					0x0F1		// Changes the mode to Katakana.
#define VK_DBE_HIRAGANA					0x0F2		// Changes the mode to Hiragana.
#define VK_DBE_SBCSCHAR					0x0F3		// Changes the mode to single-byte characters.
#define VK_DBE_DBCSCHAR					0x0F4		// Changes the mode to double-byte characters.
#define VK_DBE_ROMAN					0x0F5		// Changes the mode to Roman characters.
#define VK_DBE_NOROMAN					0x0F6		// Changes the mode to non-Roman characters.
#define VK_DBE_ENTERWORDREGISTERMODE	0x0F7		// Activates the word registration dialog box.
#define VK_DBE_ENTERIMECONFIGMODE		0x0F8		// Activates a dialog box for setting up an IME environment.
#define VK_DBE_FLUSHSTRING				0x0F9		// Deletes the undetermined string without determining it.
#define VK_DBE_CODEINPUT				0x0FA		// Changes the mode to code input.
#define VK_DBE_NOCODEINPUT				0x0FB		// Changes the mode to no-code input.
#endif

// Custom virtual keycodes for the numpad.
#define VK_NUMPAD_RETURN				0x10D		// Numpad Enter
#define VK_NUMPAD_PRIOR					0x121		// Numpad Page Up
#define VK_NUMPAD_NEXT					0x122		// Numpad Page Down
#define VK_NUMPAD_END					0x123		// Numpad End
#define VK_NUMPAD_HOME					0x124		// Numpad Home
#define VK_NUMPAD_LEFT					0x125		// Numpad Left
#define VK_NUMPAD_UP					0x126		// Numpad Up
#define VK_NUMPAD_RIGHT					0x127		// Numpad Right
#define VK_NUMPAD_DOWN					0x128		// Numpad Down
#define VK_NUMPAD_INSERT				0x12D		// Numpad Insert
#define VK_NUMPAD_DELETE				0x12E		// Numpad Delete


/* Windows does not track the button masks of its events so we need to do it
 * manually.  The left and right masks solve the problem of if both control
 * keys are depressed a the same time and only one is removed we should still
 * have a control mask but the key up event would have reset that.
 */

#define MOD_LALT						MOD_ALT				// 1
#define MOD_LCONTROL					MOD_CONTROL			// 2
#define MOD_LSHIFT						MOD_SHIFT			// 4
#define MOD_LWIN						MOD_WIN				// 8

#define MOD_RALT						MOD_ALT		<< 4	// 16
#define MOD_RCONTROL					MOD_CONTROL	<< 4	// 32
#define MOD_RSHIFT						MOD_SHIFT	<< 4	// 64
#define MOD_RWIN						MOD_WIN		<< 4	// 128

#define MOD_RBUTTON						256
#define MOD_LBUTTON						512
#define MOD_MBUTTON						1024
#define MOD_XBUTTON1					2048
#define MOD_XBUTTON2					4096

// Set the native modifier mask for future events.
extern void SetModifierMask(unsigned short int mask);

// Unset the native modifier mask for future events.
extern void UnsetModifierMask(unsigned short int mask);

// Get the current native modifier mask state.
extern unsigned short int GetModifiers();

/* Retrieves the mouse wheel scroll type. This function cannot be included as
 * part of the NativeHelpers.h due to platform specific calling restrictions.
 */
extern unsigned short int GetScrollWheelType();

/* Retrieves the mouse wheel scroll amount. This function cannot be included as
 * part of the NativeHelpers.h due to platform specific calling restrictions.
 */
extern unsigned short int GetScrollWheelAmount();

#endif
