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
 * Reference: org/jnativehook/mouse/NativeMouseEvent.java
 */

#ifndef _Included_org_jnativehook_mouse_NativeMouseEvent
#define _Included_org_jnativehook_mouse_NativeMouseEvent

#define org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_FIRST		2500
#define org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_LAST		2505
#define org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED		org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_FIRST
#define org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED		1 + org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_FIRST
#define org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED	2 + org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_FIRST
#define org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED		3 + org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_FIRST
#define org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED		4 + org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_FIRST
#define org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL		5 + org_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_FIRST


#define org_jnativehook_mouse_NativeMouseEvent_NOBUTTON					0	// AnyButton
#define org_jnativehook_mouse_NativeMouseEvent_BUTTON1					1
#define org_jnativehook_mouse_NativeMouseEvent_BUTTON2					2
#define org_jnativehook_mouse_NativeMouseEvent_BUTTON3					3
#define org_jnativehook_mouse_NativeMouseEvent_BUTTON4					4	// Extra Mouse Button
#define org_jnativehook_mouse_NativeMouseEvent_BUTTON5					5	// Extra Mouse Button

#endif
