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

#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>

#include "NativeErrors.h"
#include "JConvertFromNative.h"
#include "WinKeyCodes.h"

//FIXME We should store the modifiers as Java modifiers not Native Modifiers to speed up the getModifiers function.
static unsigned short current_modifiers = 0x0000;

void setModifierMask(unsigned short mod) {
	current_modifiers |= mod;
}

void unsetModifierMask(unsigned short mod) {
	current_modifiers ^= mod;
}

bool isModifierMask(unsigned short mod) {
	return current_modifiers & mod;
}

//Compile modifiers
jint getModifiers() {
	jint modifiers = 0;

	if (isModifierMask(MOD_LSHIFT) || isModifierMask(MOD_RSHIFT))
		modifiers |= NativeToJModifier(MOD_SHIFT);

	if (isModifierMask(MOD_LCONTROL) || isModifierMask(MOD_RCONTROL))
		modifiers |= NativeToJModifier(MOD_CONTROL);

	if (isModifierMask(MOD_LALT) || isModifierMask(MOD_RALT))
		modifiers |= NativeToJModifier(MOD_ALT);

	if (isModifierMask(MOD_LWIN) || isModifierMask(MOD_RWIN))
		modifiers |= NativeToJModifier(MOD_WIN);

	if (isModifierMask(MOD_LBUTTON))	modifiers |= NativeToJModifier(MOD_LBUTTON);
	if (isModifierMask(MOD_RBUTTON))	modifiers |= NativeToJModifier(MOD_RBUTTON);
	if (isModifierMask(MOD_MBUTTON))	modifiers |= NativeToJModifier(MOD_MBUTTON);
	if (isModifierMask(MOD_XBUTTON1))	modifiers |= NativeToJModifier(MOD_XBUTTON1);
	if (isModifierMask(MOD_XBUTTON2))	modifiers |= NativeToJModifier(MOD_XBUTTON2);

	return modifiers;
}
