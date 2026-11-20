/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2026 Alexander Barker.  All Rights Reserved.
 * https://github.com/kwhat/jnativehook/
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

/* Test-only backend: no system input is captured or injected. */
#include <assert.h>
#include <jni.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <uiohook.h>
#include "jni_Errors.h"
#include "jni_Globals.h"
#include "jni_Logger.h"
static dispatcher_t dispatch;
static void *dispatch_data;
static logger_t logger;
static void *logger_data;
void hook_set_dispatch_proc(dispatcher_t proc, void *data) { dispatch = proc; dispatch_data = data; }
void hook_set_logger_proc(logger_t proc, void *data) { logger = proc; logger_data = data; }
int hook_run(void) { return 0x12345; }
int hook_stop(void) { return 0x12345; }
long hook_get_auto_repeat_rate(void) { return 1; }
long hook_get_auto_repeat_delay(void) { return 1; }
long hook_get_pointer_acceleration_multiplier(void) { return 1; }
long hook_get_pointer_acceleration_threshold(void) { return 1; }
long hook_get_pointer_sensitivity(void) { return 1; }
long hook_get_multi_click_time(void) { return 1; }
int hook_post_event(uiohook_event * const event) { return UIOHOOK_SUCCESS; }
screen_data *hook_create_screen_info(unsigned char *count) {
    static int calls;
    *count = 1;
    if (calls++ == 0) {
        *count = 0;
        return NULL;
    }
    screen_data *screens = malloc(sizeof(screen_data));
    assert(screens != NULL);
    screens[0] = (screen_data) { .number = 2, .x = -3840, .y = -2160, .width = 40000, .height = 65535 };
    return screens;
}
