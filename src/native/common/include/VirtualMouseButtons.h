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

/* Redefined virtual constants for native use.  This file should be machine 
 * generated however no tool exists for doing so.
 * Reference: org/jnativehook/mouse/NativeMouseEvent.java
 */

#ifndef _Included_VirtualMouseButtons
#define _Included_VirtualMouseButtons

#define EVENT_MOUSE_PRESSED		1
#define EVENT_MOUSE_RELEASED	2
#define EVENT_MOUSE_MOVED		3
#define EVENT_MOUSE_DRAGGED		4
#define EVENT_MOUSE_WHEEL		5

#define MOUSE_NOBUTTON			0	// AnyButton
#define MOUSE_BUTTON1			1
#define MOUSE_BUTTON2			2
#define MOUSE_BUTTON3			3
#define MOUSE_BUTTON4			4	// Extra Mouse Button
#define MOUSE_BUTTON5			5	// Extra Mouse Button

#define WHEEL_UNIT_SCROLL		1
#define WHEEL_BLOCK_SCROLL		2

#endif
