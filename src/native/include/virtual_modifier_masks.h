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
 * Reference: org/jnativehook/NativeInputEvent.java
 */

#ifndef _VirtualModifierMasks
#define _VirtualModifierMasks

#define SHIFT_MASK			1 << 0
#define CTRL_MASK			1 << 1
#define META_MASK			1 << 2
#define ALT_MASK			1 << 3

#define BUTTON1_MASK		1 << 8
#define BUTTON2_MASK		1 << 9
#define BUTTON3_MASK		1 << 10
#define BUTTON4_MASK		1 << 11
#define BUTTON5_MASK		1 << 12

#endif
