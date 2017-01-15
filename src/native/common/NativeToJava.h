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

#ifndef _Included_JConvertFromNative
#define _Included_JConvertFromNative

#include <jni.h>

#include "org_jnativehook_NativeInputEvent.h"
#include "org_jnativehook_keyboard_NativeKeyEvent.h"
#include "org_jnativehook_mouse_NativeMouseEvent.h"
#include "org_jnativehook_mouse_NativeMouseWheelEvent.h"

typedef struct {
	jint keycode;		// Key Code
	jint rawcode;		// Raw Code
	jint location;		// Key Location
} JKeyDatum;

/* Converts the native key symbol to the virtual key code and keyboard location.
 * The raw code is the platform specific key code.
 */
extern JKeyDatum NativeToJKey(unsigned int keysym);

// Converts the native button code to its virtual counterpart.
extern jint NativeToJButton(unsigned int button);

// Converts the native modifier mask code to its virtual counterpart.
extern jint NativeToJEventMask(unsigned int mask);

#endif
