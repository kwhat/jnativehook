/* Test-only libuiohook backend. No system input is read or injected. */
#include <assert.h>
#include <jni.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <uiohook.h>
static pthread_mutex_t hook_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t hook_condition = PTHREAD_COND_INITIALIZER;
static int running;
static int pending;
static int run_status;
static logger_t logger;
static void *logger_data;
static dispatcher_t dispatch;
static void *dispatch_data;
void hook_set_dispatch_proc(dispatcher_t proc, void *data) { dispatch = proc; dispatch_data = data; }
void hook_set_logger_proc(logger_t proc, void *data) { logger = proc; logger_data = data; }
int hook_run(void) {
    if (run_status != UIOHOOK_SUCCESS) return run_status;
    pthread_mutex_lock(&hook_mutex);
    running = 1;
    pthread_mutex_unlock(&hook_mutex);
    uiohook_event event = { .type = EVENT_HOOK_ENABLED };
    dispatch(&event, dispatch_data);
    pthread_mutex_lock(&hook_mutex);
    while (running) {
        while (running && !pending) pthread_cond_wait(&hook_condition, &hook_mutex);
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
static void write_log(const char *format, ...) {
    va_list args;
    va_start(args, format);
    logger(LOG_LEVEL_INFO, logger_data, format, args);
    va_end(args);
}
static void *native_callback(void *arg) {
    uiohook_event event = { .type = EVENT_MOUSE_MOVED };
    event.data.mouse.dx = -3;
    event.data.mouse.dy = 7;
    dispatch(&event, dispatch_data);
    write_log("UTF-8: %s", "\xF0\x9F\x98\x80");
    return NULL;
}
JNIEXPORT void JNICALL Java_ApiSmokeTest_emitFromNativeThread(JNIEnv *env, jclass cls) {
    pthread_t thread;
    assert(pthread_create(&thread, NULL, native_callback, NULL) == 0);
    assert(pthread_join(thread, NULL) == 0);
}
JNIEXPORT void JNICALL Java_ApiSmokeTest_emitTypedScalar(JNIEnv *env, jclass cls) {
    uiohook_event event = { .type = EVENT_KEY_TYPED };
    event.data.keyboard.keychar = 0x1F600;
    dispatch(&event, dispatch_data);
}
int hook_post_event(uiohook_event * const event) {
    assert(event->time == 0);
    if (event->type == EVENT_KEY_TYPED) return UIOHOOK_FAILURE;
    if (event->type == EVENT_KEY_PRESSED) {
        assert(event->data.keyboard.keycode == VC_CONTROL_R);
        event->mask |= MASK_EMULATED;
        dispatch(event, dispatch_data);
        assert(event->data.keyboard.keycode == VC_CONTROL_R);
        assert(event->mask & MASK_CONSUMED);
    } else if (event->type == EVENT_MOUSE_WHEEL) {
        assert((double) event->data.wheel.rotation / event->data.wheel.delta == -0.25);
        assert(event->data.wheel.delta != 0);
        assert(event->data.wheel.direction == WHEEL_HORIZONTAL_DIRECTION);
        assert(event->data.wheel.x == -12);
        dispatch(event, dispatch_data);
    } else if (event->type == EVENT_MOUSE_MOVED) {
        assert(event->data.mouse.dx == 0 && event->data.mouse.dy == 0);
        event->data.mouse.dx = -3;
        event->data.mouse.dy = 7;
        dispatch(event, dispatch_data);
    }
    return UIOHOOK_SUCCESS;
}
screen_data *hook_create_screen_info(unsigned char *count) { *count = 0; return NULL; }
long hook_get_auto_repeat_rate(void) { return 1; }
long hook_get_auto_repeat_delay(void) { return 1; }
long hook_get_pointer_acceleration_multiplier(void) { return 1; }
long hook_get_pointer_acceleration_threshold(void) { return 1; }
long hook_get_pointer_sensitivity(void) { return 1; }
long hook_get_multi_click_time(void) { return 1; }
