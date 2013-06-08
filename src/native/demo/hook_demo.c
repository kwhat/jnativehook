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

#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#include "nativehook.h"

// Program's is running sentinel.
static bool running = false;

void dispatch_proc(VirtualEvent * const event) {
	fprintf(stdout,	"id=%d,when=%ld,mask=%#0X",
			event->type, event->time, event->mask);

	void *data = event->data;
	switch (event->type) {
		case EVENT_KEY_PRESSED:
		case EVENT_KEY_RELEASED:
		case EVENT_KEY_TYPED:
			fprintf(stdout,
					",keycode=%d,keychar=%lc,rawcode=%d\n",
					((KeyboardEventData *) data)->keycode,
					((KeyboardEventData *) data)->keychar,
					((KeyboardEventData *) data)->keycode);
			break;

		case EVENT_MOUSE_PRESSED:
		case EVENT_MOUSE_RELEASED:
		case EVENT_MOUSE_CLICKED:
		case EVENT_MOUSE_MOVED:
		case EVENT_MOUSE_DRAGGED:
			fprintf(stdout,
					",(%d,%d),button=%d,clicks=%d\n",
					((MouseEventData *) data)->x,
					((MouseEventData *) data)->y,
					((MouseEventData *) data)->button,
					((MouseEventData *) data)->clicks);
			break;

		case EVENT_MOUSE_WHEEL:
			fprintf(stdout,
					",type=%d,amount=%d,rotation=%d\n",
					((MouseWheelEventData *) data)->type,
					((MouseWheelEventData *) data)->amount,
					((MouseWheelEventData *) data)->rotation);
			break;
	}
}

int main(int argc, const char * argv[]) {
	hook_set_dispatch_proc(&dispatch_proc);

	int status = hook_enable();
	running = (status == NATIVEHOOK_SUCCESS);
	while (running) {
		#ifdef _WIN32
		Sleep(100);
		#else
		nanosleep((struct timespec[]) {{0, 100 * 1000000}}, NULL);
		#endif
	}

	if (hook_is_enabled()) {
		hook_disable();
	}

	return status;
}
