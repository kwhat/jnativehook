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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <nativehook.h>
#include <pthread.h>
#include <sys/time.h>
#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/extensions/record.h>

#include "input_converter.h"
#include "logger.h"
#include "x_input_helper.h"
#include "x_unicode_helper.h"
#include "x_wheel_codes.h"

// For this struct, refer to libxnee, requires Xlibint.h
typedef union {
	unsigned char		type;
	xEvent				event;
	xResourceReq		req;
	xGenericReply		reply;
	xError				error;
	xConnSetupPrefix	setup;
} XRecordDatum;

// Mouse globals.
static unsigned short click_count = 0;
static long click_time = 0;
static bool mouse_dragged = false;

// Structure for the current Unix epoch in milliseconds.
static struct timeval system_time;

// Virtual event pointer.
static virtual_event event;

// Event dispatch callback.
static dispatcher_t dispatcher = NULL;

extern pthread_mutex_t hook_running_mutex, hook_control_mutex;

NATIVEHOOK_API void hook_set_dispatch_proc(dispatcher_t dispatch_proc) {
	logger(LOG_LEVEL_DEBUG,	"%s [%u]: Setting new dispatch callback to %#p.\n", 
			__FUNCTION__, __LINE__, dispatch_proc);

	dispatcher = dispatch_proc;
}

// Send out an event if a dispatcher was set.
static inline void dispatch_event(virtual_event *const event) {
	if (dispatcher != NULL) {
		logger(LOG_LEVEL_DEBUG,	"%s [%u]: Dispatching event type %u.\n", 
				__FUNCTION__, __LINE__, event->type);

		dispatcher(event);
	}
	else {
		logger(LOG_LEVEL_WARN,	"%s [%u]: No dispatch callback set!\n", 
				__FUNCTION__, __LINE__);
	}
}

void hook_event_proc(XPointer pointer, XRecordInterceptData *hook) {
	if (hook->category == XRecordStartOfData) {
		pthread_mutex_lock(&hook_running_mutex);
		pthread_mutex_unlock(&hook_control_mutex);
	}
	else if (hook->category == XRecordEndOfData) {
		//pthread_mutex_lock(&hook_control_mutex);
		pthread_mutex_unlock(&hook_running_mutex);
	}
	else if (hook->category == XRecordFromServer || hook->category == XRecordFromClient) {
		// Get XRecord data.
		XRecordDatum *data = (XRecordDatum *) hook->data;

		// Set the event.time.
		gettimeofday(&system_time, NULL);
		event.time = (system_time.tv_sec * 1000) + (system_time.tv_usec / 1000);

		// Use more readable variables.
		int event_type = data->type;
		BYTE event_code = data->event.u.u.detail;
		int event_mask = data->event.u.keyButtonPointer.state;
		int event_x = data->event.u.keyButtonPointer.rootX;
		int event_y = data->event.u.keyButtonPointer.rootY;

		KeySym keysym;
		unsigned short int button;
		switch (event_type) {
			case KeyPress:
				// Fire key pressed event.
				event.type = EVENT_KEY_PRESSED;
				event.mask = convert_to_virtual_mask(event_mask);

				keysym = keycode_to_keysym(event_code, event_mask);
				event.data.keyboard.keycode = keycode_to_scancode(event_code);
				event.data.keyboard.rawcode = keysym;
				event.data.keyboard.keychar = CHAR_UNDEFINED;

				logger(LOG_LEVEL_INFO,	"%s [%u]: Key %#X pressed. (%#X)\n", 
					__FUNCTION__, __LINE__, event.data.keyboard.keycode, event.data.keyboard.rawcode);
				dispatch_event(&event);

				// Check to make sure the key is printable.
				wchar_t keychar = keysym_to_unicode(keysym);
				if (keychar != 0x0000) {
					// Fire key typed event.
					event.type = EVENT_KEY_TYPED;

					event.data.keyboard.keycode = VC_UNDEFINED;
					event.data.keyboard.keychar = keychar;

					logger(LOG_LEVEL_INFO,	"%s [%u]: Key %#X typed. (%ls)\n", 
						__FUNCTION__, __LINE__, event.data.keyboard.keycode, event.data.keyboard.keychar);
					dispatch_event(&event);
				}
				break;

			case KeyRelease:
				// Fire key released event.
				event.type = EVENT_KEY_RELEASED;
				event.mask = convert_to_virtual_mask(event_mask);

				keysym = keycode_to_keysym(event_code, event_mask);
				event.data.keyboard.keycode = keycode_to_scancode(event_code);
				event.data.keyboard.rawcode = keysym;
				event.data.keyboard.keychar = CHAR_UNDEFINED;

				logger(LOG_LEVEL_INFO,	"%s [%u]: Key %#X released. (%#X)\n", 
					__FUNCTION__, __LINE__, event.data.keyboard.keycode, event.data.keyboard.rawcode);
				dispatch_event(&event);
				break;

			case ButtonPress:
				#ifdef USE_DEBUG
				fprintf(stdout, "hook_event_proc(): Button pressed. (%i)\n", event_code);
				#endif

				// Track the number of clicks.
				if ((long) (event.time - click_time) <= hook_get_multi_click_time()) {
					click_count++;
				}
				else {
					click_count = 1;
				}
				click_time = event.time;

				/* This information is all static for X11, its up to the WM to
				* decide how to interpret the wheel events.
				*/
				// TODO Should use constants and a lookup table for button codes.
				if (event_code > 0 && (event_code <= 3 || event_code == 8 || event_code == 9)) {
					unsigned int button = convert_to_virtual_button(event_code);

					// Fire mouse pressed event.
					event.type = EVENT_MOUSE_PRESSED;
					event.mask = convert_to_virtual_mask(event_mask);

					event.data.mouse.button = button;
					event.data.mouse.clicks = click_count;
					event.data.mouse.x = event_x;
					event.data.mouse.y = event_y;

					logger(LOG_LEVEL_INFO,	"%s [%u]: Button%#X  pressed %u time(s). (%u, %u)\n", 
							__FUNCTION__, __LINE__, event.data.mouse.button, event.data.mouse.clicks, event.data.mouse.x, event.data.mouse.y);
					dispatch_event(&event);
				}
				else if (event_code == WheelUp || event_code == WheelDown) {
					/* Scroll wheel release events.
					* Scroll type: WHEEL_UNIT_SCROLL
					* Scroll amount: 3 unit increments per notch
					* Units to scroll: 3 unit increments
					* Vertical unit increment: 15 pixels
					*/

					/* X11 does not have an API call for acquiring the mouse scroll type.  This
					* maybe part of the XInput2 (XI2) extention but I will wont know until it
					* is available on my platform.  For the time being we will just use the
					* unit scroll value.
					*/
					int scroll_type = WHEEL_UNIT_SCROLL;

					/* Some scroll wheel properties are available via the new XInput2 (XI2)
					* extention.  Unfortunately the extention is not available on my
					* development platform at this time.  For the time being we will just
					* use the Windows default value of 3.
					*/
					int scroll_amount = 3;

					// Wheel Rotated Down and Towards.
					int wheel_rotation = 1; // event_code == WheelDown
					if (event_code == WheelUp) {
						// Wheel Rotated Up and Away.
						wheel_rotation = -1;
					}

					// Fire mouse wheel event.
					event.type = EVENT_MOUSE_WHEEL;
					event.mask = convert_to_virtual_mask(event_mask);

					event.data.wheel.type = scroll_type;
					event.data.wheel.amount = scroll_amount;
					event.data.wheel.rotation = wheel_rotation;

					logger(LOG_LEVEL_INFO,	"%s [%u]: Mouse wheel rotated %d units. (%u)\n", 
							__FUNCTION__, __LINE__, event.data.wheel.amount * event.data.wheel.rotation, event.data.wheel.type);
					dispatch_event(&event);
				}
				break;

			case ButtonRelease:
				// TODO Should use constants for button codes.
				if (event_code > 0 && (event_code <= 3 || event_code == 8 || event_code == 9)) {
					// Handle button release events.

					// Fire mouse released event.
					event.type = EVENT_MOUSE_RELEASED;
					event.mask = convert_to_virtual_mask(event_mask);

					button = convert_to_virtual_button(event_code);
					event.data.mouse.button = button;
					event.data.mouse.clicks = click_count;
					event.data.mouse.x = event_x;
					event.data.mouse.y = event_y;

					logger(LOG_LEVEL_INFO,	"%s [%u]: Button%#X released %u time(s). (%u, %u)\n", 
							__FUNCTION__, __LINE__, event.data.mouse.button, event.data.mouse.clicks, event.data.mouse.x, event.data.mouse.y);
					dispatch_event(&event);

					if (mouse_dragged != true) {
						// Fire mouse clicked event.
						event.type = EVENT_MOUSE_CLICKED;
						event.mask = convert_to_virtual_mask(event_mask);

						event.data.mouse.button = button;
						event.data.mouse.clicks = click_count;
						event.data.mouse.x = event_x;
						event.data.mouse.y = event_y;

						logger(LOG_LEVEL_INFO,	"%s [%u]: Button%#X clicked %u time(s). (%u, %u)\n", 
								__FUNCTION__, __LINE__, event.data.mouse.button, event.data.mouse.clicks, event.data.mouse.x, event.data.mouse.y);
						dispatch_event(&event);
					}
				}
				break;

			case MotionNotify:
				#ifdef USE_DEBUG
				fprintf(stdout, "hook_event_proc(): Motion Notified. (%i, %i)\n", event_x, event_y);
				#endif

				// Reset the click count.
				if (click_count != 0 && (long) (event.time - click_time) > hook_get_multi_click_time()) {
					click_count = 0;
				}
				unsigned int modifiers = convert_to_virtual_mask(event_mask);

				// Check the upper half of virtual modifiers for non zero
				// values and set the mouse dragged flag.
				mouse_dragged = modifiers >> 4 > 0;
				if (mouse_dragged) {
					// Create Mouse Dragged event.
					event.type = EVENT_MOUSE_DRAGGED;
				}
				else {
					// Create a Mouse Moved event.
					event.type = EVENT_MOUSE_MOVED;
				}

				// Populate common event info.
				event.mask = modifiers;

				event.data.mouse.button = MOUSE_NOBUTTON;
				event.data.mouse.clicks = click_count;
				event.data.mouse.x = event_x;
				event.data.mouse.y = event_y;

				logger(LOG_LEVEL_INFO,	"%s [%u]: Mouse moved to %u, %u.\n", 
						__FUNCTION__, __LINE__, event.data.mouse.x, event.data.mouse.y);
				dispatch_event(&event);
				break;

			default:
				// In theory this *should* never execute.
				logger(LOG_LEVEL_WARN,	"%s [%u]: Unhandled Unix event! (%#X)\n", 
						__FUNCTION__, __LINE__, (unsigned int) event_type);
				break;
		}
	}
	else {
		logger(LOG_LEVEL_WARN,	"%s [%u]: Unhandled Unix hook category! (%#X)\n", 
				__FUNCTION__, __LINE__, hook->category);
	}

	XRecordFreeData(hook);
}
