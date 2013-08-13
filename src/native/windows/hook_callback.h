/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
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

#ifndef _included_hook_callback
#define _included_hook_callback

#include <windows.h>

// Callback used by hook_thead for keyboard events.
extern LRESULT CALLBACK keyboard_event_proc(int nCode, WPARAM wParam, LPARAM lParam);

// Callback used by hook_thead for mouse events.
extern LRESULT CALLBACK mouse_event_proc(int nCode, WPARAM wParam, LPARAM lParam);

extern void initialize_modifiers();

#endif
