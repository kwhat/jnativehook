/* Copyright (c) 2007-2010 - Alex Barker (alex@1stleg.com)
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

#include <jni.h>

#include "JInputModifiers.h"
#include "JKeyCodes.h"
#include "JKeyLocations.h"
#include "JMouseButtons.h"
#include "JNativeEvents.h"

typedef struct {
	jint modifiers;		//Key Mask
	jint keycode;		//Key Code
	jchar keychar;		//Key Char
	jint location;		//Key Location
} JKeyDatum;

typedef struct {
	jint modifiers;		//Key Mask
	jint button;		//Key Code
} JButtonDatum;


JKeyDatum NativeToJKeyCode(unsigned int keysym, unsigned int state);

jint NativeToJButton(unsigned int button);

jint NativeToJModifier(unsigned int modifier);
