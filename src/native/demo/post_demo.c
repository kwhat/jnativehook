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

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <nativehook.h>
#include <stdlib.h>

// Virtual event pointers
static VirtualEvent *event = NULL;
static KeyboardEventData *keyboard_data = NULL;
static MouseEventData *mouse_data = NULL;
static MouseWheelEventData *mouse_wheel_data = NULL;

int main() {
	// Allocate memory for the virtual events only once.
	event = (VirtualEvent *) malloc(sizeof(VirtualEvent));
	keyboard_data = (KeyboardEventData *) malloc(sizeof(KeyboardEventData));
	mouse_data = (MouseEventData *) malloc(sizeof(MouseEventData));
	mouse_wheel_data = (MouseWheelEventData *) malloc(sizeof(MouseWheelEventData));


	event->type = EVENT_KEY_PRESSED;
	event->mask = 0x00;
	event->data = keyboard_data;

	keyboard_data->keycode = VC_A;
	keyboard_data->keychar = CHAR_UNDEFINED;

	hook_post_event(event);

	free(event);
	free(keyboard_data);
	free(mouse_data);
	free(mouse_wheel_data);

	return 0;
}
