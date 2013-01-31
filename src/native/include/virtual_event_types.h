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

#ifndef _included_virtual_event_types
#define _included_virtual_event_types

enum {
	key_pressed, key_released, key_typed
} VirtualEvent;

   
#define KEY_PRESSED_EVENT			1
#define KEY_RELEASED_EVENT			2
#define KEY_TYPED_EVENT				3

#define MOUSE_PRESSED_EVENT			4
#define MOUSE_RELEASED_EVENT		5
#define MOUSE_CLICKED_EVENT			6
#define MOUSE_WHEEL_EVENT			7
#define MOUSE_MOVED_EVENT			8
#define MOUSE_DRAGGED_EVENT			9

#endif
