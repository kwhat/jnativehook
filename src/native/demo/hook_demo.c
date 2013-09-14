/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
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

#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#include <nativehook.h>

// Program's is running sentinel.
static bool running = false;

// NOTE: This function executes on the hook thread!  If you need to block
// please do so on another thread with your own event dispatcher implementation.
void dispatch_proc(VirtualEvent * const event) {
	#if defined(_WIN32) && !defined(_WIN64)
	fprintf(stdout,	"id=%i,when=%I64u,mask=0x%X",
					event->type, event->time, event->mask);
	#else
	fprintf(stdout,	"id=%i,when=%llu,mask=0x%X",
					event->type, event->time, event->mask);
	#endif

	switch (event->type) {
		case EVENT_KEY_PRESSED:
			// If the escape key is pressed, naturally terminate the program.
			if (event->data.keyboard.keycode == VC_ESCAPE) {
				running = false;
			}
		case EVENT_KEY_RELEASED:
			fprintf(stdout, ",keycode=%u,scancode=0x%X",
							event->data.keyboard.keycode,
							event->data.keyboard.scancode);
			break;

		case EVENT_KEY_TYPED:
			fprintf(stdout, "keychar=%lc,scancode=%u",
							event->data.keyboard.keychar, 
							event->data.keyboard.scancode);
			break;

		case EVENT_MOUSE_PRESSED:
		case EVENT_MOUSE_RELEASED:
		case EVENT_MOUSE_CLICKED:
		case EVENT_MOUSE_MOVED:
		case EVENT_MOUSE_DRAGGED:
			fprintf(stdout, ",x=%i,y=%i,button=%i,clicks=%i",
							event->data.mouse.x, event->data.mouse.y,
							event->data.mouse.button, event->data.mouse.clicks);
			break;

		case EVENT_MOUSE_WHEEL:
			fprintf(stdout, ",type=%i,amount=%i,rotation=%i",
							event->data.wheel.type, event->data.wheel.amount,
							event->data.wheel.rotation);
			break;
	}

	fprintf(stdout, "\n");
}

int main() {
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
