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
	jint keycode;		//Key Code
	jint location;		//Key Location
} JKeyCode;

unsigned int JKeycodeToNative(JKeyCode jkey);
JKeyCode NativeToJKeycode(unsigned int keycode);

jint NativeToJButton(unsigned int button);
unsigned int JButtonToNative(jint button);

unsigned int JModifierToNative(jint modifier);
jint NativeToJModifier(unsigned int modifier);
