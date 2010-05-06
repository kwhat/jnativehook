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

#include <stdbool.h>

#define MOD_RALT		MOD_ALT
#define MOD_RCONTROL	MOD_CONTROL
#define MOD_RSHIFT		MOD_SHIFT
#define MOD_RWIN		MOD_WIN
#define MOD_LALT		MOD_ALT		<< 4
#define MOD_LCONTROL	MOD_CONTROL << 4
#define MOD_LSHIFT		MOD_SHIFT	<< 4
#define MOD_LWIN		MOD_WIN		<< 4

void setModifierMask(unsigned char mod);
void unsetModifierMask(unsigned char mod);
bool isModifierMask(unsigned char mod);
