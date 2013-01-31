/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _Included_JConvertFromNative
#define _Included_JConvertFromNative

#include <stdint.h>

/* Converts the native key symbol to the virtual key code and keyboard location.
 * The raw code is the platform specific key code. 
 */
extern uint_fast16_t convert_to_native_keycode(uint_fast16_t virtual_keycode);

// Converts the native button code to its virtual counterpart.
extern uint_fast8_t convert_to_native_button(uint_fast8_t virtual_button);

// Converts the native modifier mask to its virtual counterpart.
extern uint_fast16_t convert_to_native_mask(uint_fast16_t virtual_mask);

#endif
