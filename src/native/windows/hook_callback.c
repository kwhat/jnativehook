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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <limits.h>
#include <nativehook.h>
#ifdef USE_DEBUG
#include <stdio.h>
#endif
#include <time.h>
#include <windows.h>

#include "input_converter.h"
#include "hook_callback.h"
#include "win_unicode_helper.h"

// Modifiers for tracking key masks.
static unsigned short int current_modifiers = 0x0000;

// Structure for the current Unix epoch in milliseconds.
static FILETIME ft;

// Key typed Unicode return values.
static WCHAR keywchar = '\0', keydead = 0;

// Click count globals.
static unsigned short click_count = 0;
static DWORD click_time = 0;
static POINT last_click;

// Virtual event pointer.
static VirtualEvent event;

// Event dispatch callback
static void (*current_dispatch_proc)(VirtualEvent *const) = NULL;

extern HHOOK keyboard_event_hhook, mouse_event_hhook;

NATIVEHOOK_API void hook_set_dispatch_proc(void (*dispatch_proc)(VirtualEvent * const)) {
	#ifdef USE_DEBUG
	fprintf(stdout, "hook_set_dispatch_proc(): Setting new dispatch callback.\n");
	#endif

	current_dispatch_proc = dispatch_proc;
}

// Send out an event if a dispatcher was set.
static inline void dispatch_event(VirtualEvent *const event) {
	if (current_dispatch_proc != NULL) {
		#ifdef USE_DEBUG
		fprintf(stdout, "dispatch_event(): Dispatching event. (%d)\n", event->type);
		#endif

		current_dispatch_proc(event);
	}
	#ifdef USE_DEBUG
	else {
		fprintf(stderr, "dispatch_event(): No dispatch proc set! (%d)\n", event->type);
	}
	#endif
}

// Set the native modifier mask for future events.
static inline void set_modifier_mask(unsigned short int mask) {
	current_modifiers |= mask;
}

// Unset the native modifier mask for future events.
static inline void unset_modifier_mask(unsigned short int mask) {
	current_modifiers ^= mask;
}

// Get the current native modifier mask state.
static inline unsigned short int get_modifiers() {
	return current_modifiers;
}

/* Retrieves the mouse wheel scroll type. This function cannot be included as
 * part of the NativeHelpers.h due to platform specific calling restrictions.
 */
static inline unsigned short int get_scroll_wheel_type() {
	unsigned short int value;
	UINT wheel_type;

	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &wheel_type, 0);
	if (wheel_type == WHEEL_PAGESCROLL) {
		value = WHEEL_BLOCK_SCROLL;
	}
	else {
		value = WHEEL_UNIT_SCROLL;
	}

	return value;
}

/* Retrieves the mouse wheel scroll amount. This function cannot be included as
 * part of the NativeHelpers.h due to platform specific calling restrictions.
 */
static inline unsigned short int get_scroll_wheel_amount() {
	unsigned short int value;
	UINT wheel_amount;

	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &wheel_amount, 0);
	if (wheel_amount == WHEEL_PAGESCROLL) {
		value = 1;
	}
	else {
		value = (unsigned short int) wheel_amount;
	}

	return value;
}


LRESULT CALLBACK keyboard_event_proc(int nCode, WPARAM wParam, LPARAM lParam) {
	// MS Keyboard event struct data.
	KBDLLHOOKSTRUCT *kbhook = (KBDLLHOOKSTRUCT *) lParam;

	// Set the event_time.
	GetSystemTimeAsFileTime(&ft);
	// Convert to milliseconds = 100-nanoseconds / 10000
	__int64 system_time = (((__int64) ft.dwHighDateTime << 32) | ft.dwLowDateTime) / 10000;

	// Convert Windows epoch to Unix epoch (1970 - 1601 in milliseconds)
	event.time = system_time - 11644473600000;

	switch(wParam) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			#ifdef USE_DEBUG
			fprintf(stdout, "keyboard_event_proc(): Key pressed. (%i)\n",
					(unsigned int) kbhook->vkCode);
			#endif

			// Check and setup modifiers.
			if (kbhook->vkCode == VK_LSHIFT)		set_modifier_mask(MASK_SHIFT_L);
			else if (kbhook->vkCode == VK_RSHIFT)	set_modifier_mask(MASK_SHIFT_R);
			else if (kbhook->vkCode == VK_LCONTROL)	set_modifier_mask(MASK_CTRL_L);
			else if (kbhook->vkCode == VK_RCONTROL)	set_modifier_mask(MASK_CTRL_R);
			else if (kbhook->vkCode == VK_LMENU)	set_modifier_mask(MASK_ALT_L);
			else if (kbhook->vkCode == VK_RMENU)	set_modifier_mask(MASK_ALT_R);
			else if (kbhook->vkCode == VK_LWIN)		set_modifier_mask(MASK_META_L);
			else if (kbhook->vkCode == VK_RWIN)		set_modifier_mask(MASK_META_R);


			// Workaround for Windows numpad keys.
			/*
			if (kbhook->vkCode == VK_RETURN && (kbhook->flags & 0x01) != 0) {
				jkey = NativeToJKey(VK_NUMPAD_RETURN);
			}
			else if (kbhook->vkCode == VK_PRIOR && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_PRIOR);
			}
			else if (kbhook->vkCode == VK_NEXT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_NEXT);
			}
			else if (kbhook->vkCode == VK_END && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_END);
			}
			else if (kbhook->vkCode == VK_HOME && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_HOME);
			}
			else if (kbhook->vkCode == VK_LEFT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_LEFT);
			}
			else if (kbhook->vkCode == VK_UP && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_UP);
			}
			else if (kbhook->vkCode == VK_RIGHT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_RIGHT);
			}
			else if (kbhook->vkCode == VK_DOWN && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_DOWN);
			}
			else if (kbhook->vkCode == VK_INSERT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_INSERT);
			}
			else if (kbhook->vkCode == VK_DELETE && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_DELETE);
			}
			else {
				jkey = NativeToJKey(kbhook->vkCode);
			}
			*/
			#ifdef USE_DEBUG
			fprintf(stdout, "Test kbhook->flags = 0x%X\n", (unsigned int) kbhook->flags);
			#endif

			// Fire key pressed event.
			event.type = EVENT_KEY_PRESSED;
			event.mask = get_modifiers();

			event.data.keyboard.keycode = convert_to_virtual_key(kbhook->vkCode);
			event.data.keyboard.rawcode = kbhook->scanCode;
			event.data.keyboard.keychar = CHAR_UNDEFINED;

			dispatch_event(&event);

			if (convert_vk_to_wchar(kbhook->vkCode, &keywchar, &keydead) > 0) {
				// Fire key typed event.
				event.type = EVENT_KEY_TYPED;
				// TODO This shouldn't be necessary but double check that the
				//		ptr const makes this value immutable.
				//event.mask = get_modifiers();

				event.data.keyboard.keycode = VC_UNDEFINED;
				event.data.keyboard.keychar = keywchar;

				dispatch_event(&event);
			}
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			#ifdef USE_DEBUG
			fprintf(stdout, "keyboard_event_proc(): Key released. (%i)\n", (unsigned int) kbhook->vkCode);
			#endif

			// Check and setup modifiers.
			if (kbhook->vkCode == VK_LSHIFT)		unset_modifier_mask(MASK_SHIFT_L);
			else if (kbhook->vkCode == VK_RSHIFT)	unset_modifier_mask(MASK_SHIFT_R);
			else if (kbhook->vkCode == VK_LCONTROL)	unset_modifier_mask(MASK_CTRL_L);
			else if (kbhook->vkCode == VK_RCONTROL)	unset_modifier_mask(MASK_CTRL_R);
			else if (kbhook->vkCode == VK_LMENU)	unset_modifier_mask(MASK_ALT_L);
			else if (kbhook->vkCode == VK_RMENU)	unset_modifier_mask(MASK_ALT_R);
			else if (kbhook->vkCode == VK_LWIN)		unset_modifier_mask(MASK_META_L);
			else if (kbhook->vkCode == VK_RWIN)		unset_modifier_mask(MASK_META_R);

			// Workaround for Windows numpad keys.
			/*
			if (kbhook->vkCode == VK_RETURN && (kbhook->flags & 0x01) != 0) {
				jkey = NativeToJKey(VK_NUMPAD_RETURN);
			}
			else if (kbhook->vkCode == VK_PRIOR && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_PRIOR);
			}
			else if (kbhook->vkCode == VK_NEXT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_NEXT);
			}
			else if (kbhook->vkCode == VK_END && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_END);
			}
			else if (kbhook->vkCode == VK_HOME && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_HOME);
			}
			else if (kbhook->vkCode == VK_LEFT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_LEFT);
			}
			else if (kbhook->vkCode == VK_UP && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_UP);
			}
			else if (kbhook->vkCode == VK_RIGHT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_RIGHT);
			}
			else if (kbhook->vkCode == VK_DOWN && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_DOWN);
			}
			else if (kbhook->vkCode == VK_INSERT && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_INSERT);
			}
			else if (kbhook->vkCode == VK_DELETE && (kbhook->flags & 0x01) == 0) {
				jkey = NativeToJKey(VK_NUMPAD_DELETE);
			}
			else {
				jkey = NativeToJKey(kbhook->vkCode);
			}
			*/

			// Fire key released event.
			event.type = EVENT_KEY_RELEASED;
			event.mask = get_modifiers();

			event.data.keyboard.keycode = convert_to_virtual_key(kbhook->vkCode);
			event.data.keyboard.rawcode = kbhook->scanCode;
			event.data.keyboard.keychar = CHAR_UNDEFINED;

			dispatch_event(&event);
			break;

		#ifdef USE_DEBUG
		default:
			fprintf(stdout, "keyboard_event_proc(): Unhandled keyboard event: 0x%X\n", (unsigned int) wParam);
			break;
		#endif
	}

	return CallNextHookEx(keyboard_event_hhook, nCode, wParam, lParam);
}

LRESULT CALLBACK mouse_event_proc(int nCode, WPARAM wParam, LPARAM lParam) {
	// MS Mouse event struct data.
	MSLLHOOKSTRUCT *mshook = (MSLLHOOKSTRUCT *) lParam;

	// Set the event time.
	GetSystemTimeAsFileTime(&ft);
	// Convert to milliseconds = 100-nanoseconds / 10000
	__int64 system_time = (((__int64) ft.dwHighDateTime << 32) | ft.dwLowDateTime) / 10000;

	// Convert Windows epoch to Unix epoch (1970 - 1601 in milliseconds)
	event.time = system_time - 11644473600000;

	switch(wParam) {
		case WM_LBUTTONDOWN:
			event.data.mouse.button = convert_to_virtual_button(VK_LBUTTON);
			set_modifier_mask(MASK_BUTTON1);
			goto BUTTONDOWN;

		case WM_RBUTTONDOWN:
			event.data.mouse.button = convert_to_virtual_button(VK_RBUTTON);
			set_modifier_mask(MASK_BUTTON2);
			goto BUTTONDOWN;

		case WM_MBUTTONDOWN:
			event.data.mouse.button = convert_to_virtual_button(VK_MBUTTON);
			set_modifier_mask(MASK_BUTTON3);
			goto BUTTONDOWN;

		case WM_XBUTTONDOWN:
		case WM_NCXBUTTONDOWN:
			if (HIWORD(mshook->mouseData) == XBUTTON1) {
				event.data.mouse.button = convert_to_virtual_button(VK_XBUTTON1);
				set_modifier_mask(MASK_BUTTON4);
			}
			else if (HIWORD(mshook->mouseData) == XBUTTON2) {
				event.data.mouse.button = convert_to_virtual_button(VK_XBUTTON2);
				set_modifier_mask(MASK_BUTTON5);
			}
			else {
				event.data.mouse.button = HIWORD(mshook->mouseData);
			}

		BUTTONDOWN:
			#ifdef USE_DEBUG
			fprintf(stdout, "mouse_event_proc(): Button pressed. (%u)\n", event.data.mouse.button);
			#endif

			// Track the number of clicks.
			if ((long) (mshook->time - click_time) <= hook_get_multi_click_time()) {
				click_count++;
			}
			else {
				click_count = 1;
			}
			click_time = mshook->time;

			// Store the last click point.
			last_click.x = mshook->pt.x;
			last_click.y = mshook->pt.y;

			// Fire mouse pressed event.
			event.type = EVENT_MOUSE_PRESSED;
			event.mask = get_modifiers();

			event.data.mouse.clicks = click_count;
			event.data.mouse.x = mshook->pt.x;
			event.data.mouse.y = mshook->pt.y;

			dispatch_event(&event);
			break;

		case WM_LBUTTONUP:
			event.data.mouse.button = convert_to_virtual_button(VK_LBUTTON);
			unset_modifier_mask(MASK_BUTTON1);
			goto BUTTONUP;

		case WM_RBUTTONUP:
			event.data.mouse.button = convert_to_virtual_button(VK_RBUTTON);
			unset_modifier_mask(MASK_BUTTON2);
			goto BUTTONUP;

		case WM_MBUTTONUP:
			event.data.mouse.button = convert_to_virtual_button(VK_MBUTTON);
			unset_modifier_mask(MASK_BUTTON3);
			goto BUTTONUP;

		case WM_XBUTTONUP:
		case WM_NCXBUTTONUP:
			if (HIWORD(mshook->mouseData) == XBUTTON1) {
				event.data.mouse.button = convert_to_virtual_button(VK_XBUTTON1);
				unset_modifier_mask(MASK_BUTTON4);
			}
			else if (HIWORD(mshook->mouseData) == XBUTTON2) {
				event.data.mouse.button = convert_to_virtual_button(VK_XBUTTON2);
				unset_modifier_mask(MASK_BUTTON5);
			}
			else {
				event.data.mouse.button = HIWORD(mshook->mouseData);
			}

		BUTTONUP:
			#ifdef USE_DEBUG
			fprintf(stdout, "mouse_event_proc(): Button released. (%u)\n", event.data.mouse.button);
			#endif

			// Fire mouse released event.
			event.type = EVENT_MOUSE_RELEASED;
			event.mask = get_modifiers();

			event.data.mouse.clicks = click_count;
			event.data.mouse.x = mshook->pt.x;
			event.data.mouse.y = mshook->pt.y;

			dispatch_event(&event);

			if (last_click.x == mshook->pt.x && last_click.y == mshook->pt.y) {
				// Fire mouse clicked event.
				event.type = EVENT_MOUSE_CLICKED;
				// TODO This shouldn't be necessary but double check that the
				//		ptr const makes this value immutable.
				//event.mask = get_modifiers();
				//event.data.mouse.button = button;

				event.data.mouse.clicks = click_count;
				event.data.mouse.x = mshook->pt.x;
				event.data.mouse.y = mshook->pt.y;

				dispatch_event(&event);
			}
			break;

		case WM_MOUSEMOVE:
			#ifdef USE_DEBUG
			fprintf(stdout, "mouse_event_proc(): Motion Notified. (%li, %li)\n", mshook->pt.x, mshook->pt.y);
			#endif

			// Reset the click count.
			if (click_count != 0 && (long) (mshook->time - click_time) > hook_get_multi_click_time()) {
				click_count = 0;
			}

			// We received a mouse move event with the mouse actually moving.
			// This verifies that the mouse was moved after being depressed.
			if (last_click.x != mshook->pt.x || last_click.y != mshook->pt.y) {
				// Check the upper half of the current modifiers for non zero
				// value.  This indicates the presence of a button down mask.
				if (get_modifiers() >> 8) {
					// Create Mouse Dragged event.
					event.type = EVENT_MOUSE_DRAGGED;

				}
				else {
					// Create a Mouse Moved event.
					event.type = EVENT_MOUSE_MOVED;
				}

				// Populate common event info.
				event.mask = get_modifiers();

				event.data.mouse.button = MOUSE_NOBUTTON;
				event.data.mouse.clicks = click_count;
				event.data.mouse.x = mshook->pt.x;
				event.data.mouse.y = mshook->pt.y;

				// Fire mouse moved event.
				dispatch_event(&event);
			}
			break;

		case WM_MOUSEWHEEL:
			#ifdef USE_DEBUG
			fprintf(stdout, "mouse_event_proc(): WM_MOUSEWHEEL. (%i / %i)\n", HIWORD(mshook->mouseData), WHEEL_DELTA);
			#endif

			// Track the number of clicks.
			if ((long) (mshook->time - click_time) <= hook_get_multi_click_time()) {
				click_count++;
			}
			else {
				click_count = 1;
			}
			click_time = mshook->time;

			// Fire mouse wheel event.
			event.type = EVENT_MOUSE_WHEEL;
			event.mask = get_modifiers();

			event.data.wheel.type = get_scroll_wheel_type();
			event.data.wheel.amount = get_scroll_wheel_amount();

			/* Delta HIWORD(mshook->mouseData)
			 * A positive value indicates that the wheel was rotated
			 * forward, away from the user; a negative value indicates that
			 * the wheel was rotated backward, toward the user. One wheel
			 * click is defined as WHEEL_DELTA, which is 120. */
			event.data.wheel.rotation = ((signed short) HIWORD(mshook->mouseData) / WHEEL_DELTA) * -1;

			dispatch_event(&event);
			break;

		#ifdef USE_DEBUG
		default:
			fprintf(stdout, "mouse_event_proc(): Unhandled mouse event: 0x%X\n", (unsigned int) wParam);
			break;
		#endif
	}


	return CallNextHookEx(mouse_event_hhook, nCode, wParam, lParam);
}

void initialize_modifiers() {
		current_modifiers = 0x0000;

		if (GetKeyState(VK_LSHIFT)	 < 0)	set_modifier_mask(MASK_SHIFT_L);
		if (GetKeyState(VK_RSHIFT)   < 0)	set_modifier_mask(MASK_SHIFT_R);
		if (GetKeyState(VK_LCONTROL) < 0)	set_modifier_mask(MASK_CTRL_L);
		if (GetKeyState(VK_RCONTROL) < 0)	set_modifier_mask(MASK_CTRL_R);
		if (GetKeyState(VK_LMENU)    < 0)	set_modifier_mask(MASK_ALT_L);
		if (GetKeyState(VK_RMENU)    < 0)	set_modifier_mask(MASK_ALT_R);
		if (GetKeyState(VK_LWIN)     < 0)	set_modifier_mask(MASK_META_L);
		if (GetKeyState(VK_RWIN)     < 0)	set_modifier_mask(MASK_META_R);
}
