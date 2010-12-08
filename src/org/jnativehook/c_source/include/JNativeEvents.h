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

//TODO These should probably be pulled by JNI from the java class.
//Reference: org/jnativehook/keyboard/NativeKeyEvent.java
#define JK_NATIVE_KEY_FIRST			2400
#define JK_NATIVE_KEY_LAST			2401
#define JK_NATIVE_KEY_PRESSED		JK_NATIVE_KEY_FIRST
#define JK_NATIVE_KEY_RELEASED		1 + JK_NATIVE_KEY_FIRST

//Reference: org/jnativehook/mouse/NativeMouseEvent.java
#define JK_NATIVE_MOUSE_FIRST		2500
#define JK_NATIVE_MOUSE_LAST		2504
#define JK_NATIVE_MOUSE_PRESSED		JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_RELEASED	1 + JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_MOVED		2 + JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_DRAGGED		3 + JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_WHEEL		4 + JK_NATIVE_MOUSE_FIRST
