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

/* Test-only lifecycle backend; no system input is read or injected. */
#include <jni.h>
#include <pthread.h>
#include <uiohook.h>

static pthread_mutex_t hook_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t hook_condition = PTHREAD_COND_INITIALIZER;
static int running;
static int pending;
static int run_status;
static dispatcher_t dispatch;
static void *dispatch_data;

void hook_set_dispatch_proc(dispatcher_t proc, void *data) {
    dispatch = proc;
    dispatch_data = data;
}

void hook_set_logger_proc(logger_t proc, void *data) {
    // Logging behavior is outside the lifecycle regression checks.
    (void) proc;
    (void) data;
}

int hook_run(void) {
    if (run_status != UIOHOOK_SUCCESS) {
        return run_status;
    }
    pthread_mutex_lock(&hook_mutex);
    running = 1;
    pthread_mutex_unlock(&hook_mutex);
    uiohook_event event = { .type = EVENT_HOOK_ENABLED };
    dispatch(&event, dispatch_data);
    pthread_mutex_lock(&hook_mutex);
    while (running) {
        while (running && !pending) {
            pthread_cond_wait(&hook_condition, &hook_mutex);
        }
        if (pending) {
            pending = 0;
            pthread_mutex_unlock(&hook_mutex);
            event.type = EVENT_MOUSE_MOVED;
            dispatch(&event, dispatch_data);
            pthread_mutex_lock(&hook_mutex);
        }
    }
    pthread_mutex_unlock(&hook_mutex);
    event.type = EVENT_HOOK_DISABLED;
    dispatch(&event, dispatch_data);
    return UIOHOOK_SUCCESS;
}

int hook_stop(void) {
    pthread_mutex_lock(&hook_mutex);
    running = 0;
    pthread_cond_signal(&hook_condition);
    pthread_mutex_unlock(&hook_mutex);
    return UIOHOOK_SUCCESS;
}

JNIEXPORT void JNICALL Java_LifecycleTest_setRunStatus(JNIEnv *env, jclass cls, jint status) {
    run_status = status;
}

JNIEXPORT void JNICALL Java_LifecycleTest_requestHookEvent(JNIEnv *env, jclass cls) {
    pthread_mutex_lock(&hook_mutex);
    pending = 1;
    pthread_cond_signal(&hook_condition);
    pthread_mutex_unlock(&hook_mutex);
}

int hook_post_event(uiohook_event * const event) {
    return UIOHOOK_FAILURE;
}

screen_data *hook_create_screen_info(unsigned char *count) {
    *count = 0;
    return NULL;
}

long hook_get_auto_repeat_rate(void) { return 1; }
long hook_get_auto_repeat_delay(void) { return 1; }
long hook_get_pointer_acceleration_multiplier(void) { return 1; }
long hook_get_pointer_acceleration_threshold(void) { return 1; }
long hook_get_pointer_sensitivity(void) { return 1; }
long hook_get_multi_click_time(void) { return 1; }
