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

#include <stdbool.h>

unsigned short _current_modifiers = 0x0000;

void setModifierMask(unsigned short mod) {
	_current_modifiers |= mod;
}

void unsetModifierMask(unsigned short mod) {
	_current_modifiers ^= mod;
}

bool isModifierMask(unsigned short mod) {
	return _current_modifiers & mod;
}
