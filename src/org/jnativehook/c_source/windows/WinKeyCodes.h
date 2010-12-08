/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2010 Alexander Barker.  All Rights Received.
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

/* Windows does not track the button masks of its events so we need to do it
 * manually.  The left and right masks solve the problem of if both control
 * keys are depressed a the same time and only one is removed we should still
 * have a control mask but the key up event would have reset that.
 */

#define MOD_LALT		MOD_ALT				// 1
#define MOD_LCONTROL	MOD_CONTROL			// 2
#define MOD_LSHIFT		MOD_SHIFT			// 4
#define MOD_LWIN		MOD_WIN				// 8

#define MOD_RALT		MOD_ALT		<< 4	// 16
#define MOD_RCONTROL	MOD_CONTROL << 4	// 32
#define MOD_RSHIFT		MOD_SHIFT	<< 4	// 64
#define MOD_RWIN		MOD_WIN		<< 4	// 128

#define MOD_RBUTTON		256
#define MOD_LBUTTON		512
#define MOD_MBUTTON		1024
#define MOD_XBUTTON1	2048
#define MOD_XBUTTON2	4096

void setModifierMask(unsigned short mod);
void unsetModifierMask(unsigned short mod);
bool isModifierMask(unsigned short mod);
