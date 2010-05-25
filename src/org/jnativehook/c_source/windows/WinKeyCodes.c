/* Copyright (c) 2006-2010 - Alexander Barker (alex@1stleg.com)
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "WinKeyCodes.h"

unsigned short modifiers = 0x0000;

void setModifierMask(unsigned short mod) {
	modifiers |= mod;
}

void unsetModifierMask(unsigned short mod) {
	modifiers ^= mod;
}

bool isModifierMask(unsigned short mod) {
	return modifiers & mod;
}
