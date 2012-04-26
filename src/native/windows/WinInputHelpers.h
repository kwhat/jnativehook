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

#ifndef _Included_WinInputHelpers
#define _Included_WinInputHelpers

#include <stdbool.h>

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

//Helper functions for tracking the modifers.
extern void SetModifier(unsigned short int vkCode);
extern void UnsetModifier(unsigned short int vkCode);
extern bool IsModifierSet(unsigned short int vkCode);
extern unsigned short int GetModifierState(unsigned short int vkCode);

extern int KeycodeToUnicode(unsigned short int vkCode, unsigned short int scanCode, WCHAR * buffer, unsigned short int buffer_size);

extern unsigned short int GetScrollWheelType();
extern unsigned short int GetScrollWheelAmount();

extern void LoadInputHelper();
extern void UnloadInputHelper();

#endif
