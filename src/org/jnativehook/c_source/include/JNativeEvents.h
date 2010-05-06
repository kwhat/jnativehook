/* Copyright (c) 2006-2010 - Alexander Barker (alex@1stleg.com)
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
//Reference: org/jnativehook/keyboard/NativeKeyEvent.java
#define JK_NATIVE_KEY_FIRST			2400
#define JK_NATIVE_KEY_LAST			2402
#define JK_NATIVE_KEY_TYPED			JK_NATIVE_KEY_FIRST
#define JK_NATIVE_KEY_PRESSED		1 + JK_NATIVE_KEY_FIRST
#define JK_NATIVE_KEY_RELEASED		2 + JK_NATIVE_KEY_FIRST

//Reference: org/jnativehook/mouse/NativeMouseEvent.java
#define JK_NATIVE_MOUSE_FIRST		2500
#define JK_NATIVE_MOUSE_LAST		2507
#define JK_NATIVE_MOUSE_CLICKED		JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_PRESSED		1 + JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_RELEASED	2 + JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_MOVED		3 + JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_ENTERED		4 + JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_EXITED		5 + JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_DRAGGED		6 + JK_NATIVE_MOUSE_FIRST
#define JK_NATIVE_MOUSE_WHEEL		7 + JK_NATIVE_MOUSE_FIRST
