/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2014 Alexander Barker.  All Rights Received.
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
 * Reference: org/jnativehook/NativeHookEvent.java
 */

#ifndef _Included_org_jnativehook_NativeHookEvent
#define _Included_org_jnativehook_NativeHookEvent

#define org_jnativehook_NativeHookEvent_NATIVE_MOUSE_FIRST		2300
#define org_jnativehook_NativeHookEvent_NATIVE_MOUSE_LAST		2301
#define org_jnativehook_NativeHookEvent_NATIVE_HOOK_ENABLED		org_jnativehook_NativeHookEvent_NATIVE_MOUSE_FIRST
#define org_jnativehook_NativeHookEvent_NATIVE_HOOK_DISABLED	1 + org_jnativehook_NativeHookEvent_NATIVE_MOUSE_FIRST
#endif
