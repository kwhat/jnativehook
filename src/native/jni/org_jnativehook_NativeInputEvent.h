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

/* Redefined virtual constants for native use.  This file should be machine
 * generated however no tool exists for doing so.
 * Reference: org/jnativehook/NativeInputEvent.java
 */

#ifndef _Included_org_jnativehook_NativeInputEvent
#define _Included_org_jnativehook_NativeInputEvent

#define org_jnativehook_NativeInputEvent_SHIFT_MASK				1 << 0
#define org_jnativehook_NativeInputEvent_CTRL_MASK				1 << 1
#define org_jnativehook_NativeInputEvent_META_MASK				1 << 2
#define org_jnativehook_NativeInputEvent_ALT_MASK				1 << 3

#define org_jnativehook_NativeInputEvent_BUTTON1_MASK			1 << 4
#define org_jnativehook_NativeInputEvent_BUTTON2_MASK			1 << 5
#define org_jnativehook_NativeInputEvent_BUTTON3_MASK			1 << 6
#define org_jnativehook_NativeInputEvent_BUTTON4_MASK			1 << 7
#define org_jnativehook_NativeInputEvent_BUTTON5_MASK			1 << 8

#endif
