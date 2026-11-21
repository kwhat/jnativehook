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

#include <jni.h>
#include <stdbool.h>
#include <stdlib.h>
#include <uiohook.h>

#include "com_github_kwhat_jnativehook_keyboard_NativeKeyEvent.h"
#include "com_github_kwhat_jnativehook_mouse_NativeMouseEvent.h"
#include "com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent.h"
#include "com_github_kwhat_jnativehook_NativeInputEvent.h"
#include "jni_Converter.h"
#include "jni_EventDispathcer.h"
#include "jni_Errors.h"
#include "jni_Globals.h"
#include "jni_Load.h"
#include "jni_Logger.h"


// Publish hook lifecycle state to the Java hook thread.
static void jni_notifyHookThread(JNIEnv *env, jboolean enabled) {
    jobject hookThread_obj = (*env)->GetStaticObjectField(
            env,
            com_github_kwhat_jnativehook_GlobalScreen->cls,
            com_github_kwhat_jnativehook_GlobalScreen->hookThread);

    if (hookThread_obj != NULL) {
        (*env)->CallVoidMethod(env, hookThread_obj,
                com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread->setNativeHookState, enabled);
        (*env)->DeleteLocalRef(env, hookThread_obj);
    }
}

static jobject jni_dispatchKeyEvent(JNIEnv *env, uiohook_event * const event, void *user_data) {
    uint16_t keycode = VC_UNDEFINED;
    jint location = com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_UNKNOWN;
    switch (event->type) {
        case EVENT_KEY_PRESSED:
            keycode = event->data.keyboard.keycode;
            if (jni_ConvertToJavaLocation(&keycode, &location) != JNI_OK) {
                return NULL;
            }

            return (*env)->NewObject(
                    env,
                    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->cls,
                    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->init,
                    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED,
                    (jint) event->mask,
                    (jint) event->data.keyboard.rawcode,
                    (jint) keycode,
                    (jchar) com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
                    location);

        case EVENT_KEY_RELEASED:
            keycode = event->data.keyboard.keycode;
            if (jni_ConvertToJavaLocation(&keycode, &location) != JNI_OK) {
                return NULL;
            }

            return (*env)->NewObject(
                    env,
                    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->cls,
                    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->init,
                    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED,
                    (jint) event->mask,
                    (jint) event->data.keyboard.rawcode,
                    (jint) keycode,
                    (jchar) com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
                    location);

        case EVENT_KEY_TYPED:
            // Java's char API uses UTF-16; wchar_t may contain a full scalar on Unix.
            if ((uint32_t) event->data.keyboard.keychar > 0xFFFF) {
                uint32_t scalar = (uint32_t) event->data.keyboard.keychar;
                if (scalar > 0x10FFFF) {
                    jni_Logger(LOG_LEVEL_WARN, "%s [%u]: Invalid Unicode scalar value %#X.\n",
                            __FUNCTION__, __LINE__, (unsigned int) scalar);
                    return NULL;
                }

                uiohook_event unit = *event;
                scalar -= 0x10000;

                unit.data.keyboard.keychar = (wchar_t) (0xD800 + (scalar >> 10));
                jni_EventDispatcher(&unit, user_data);
                if (!(unit.mask & MASK_CONSUMED)) {
                    unit.data.keyboard.keychar = (wchar_t) (0xDC00 + (scalar & 0x3FF));
                    jni_EventDispatcher(&unit, user_data);
                }

                event->mask |= unit.mask & MASK_CONSUMED;
            } else {
                return (*env)->NewObject(
                        env,
                        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->cls,
                        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->init,
                        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED,
                        (jint) event->mask,
                        (jint) event->data.keyboard.rawcode,
                        (jint) com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_UNDEFINED,
                        (jchar) event->data.keyboard.keychar,
                        location);
            }
    }

    return NULL;
}

static jobject jni_dispatchButtonEvent(JNIEnv *env, uiohook_event * const event) {
    switch (event->type)
    {
        case EVENT_MOUSE_PRESSED:
            return (*env)->NewObject(
                    env,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent->cls,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent->init,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED,
                    (jint) event->mask,
                    (jint) event->data.mouse.x,
                    (jint) event->data.mouse.y,
                    (jint) event->data.mouse.dx,
                    (jint) event->data.mouse.dy,
                    (jint) event->data.mouse.clicks,
                    (jint) event->data.mouse.button);

        case EVENT_MOUSE_RELEASED:
            return (*env)->NewObject(
                    env,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent->cls,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent->init,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED,
                    (jint) event->mask,
                    (jint) event->data.mouse.x,
                    (jint) event->data.mouse.y,
                    (jint) event->data.mouse.dx,
                    (jint) event->data.mouse.dy,
                    (jint) event->data.mouse.clicks,
                    (jint) event->data.mouse.button);

        case EVENT_MOUSE_CLICKED:
            return (*env)->NewObject(
                    env,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent->cls,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent->init,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED,
                    (jint) event->mask,
                    (jint) event->data.mouse.x,
                    (jint) event->data.mouse.y,
                    (jint) event->data.mouse.dx,
                    (jint) event->data.mouse.dy,
                    (jint) event->data.mouse.clicks,
                    (jint) event->data.mouse.button);
    }

    return NULL;
}

static jobject jni_dispatchMotionEvent(JNIEnv *env, uiohook_event * const event) {
    switch (event->type) {
        case EVENT_MOUSE_MOVED:
            return (*env)->NewObject(
                    env,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent->cls,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent->init,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED,
                    (jint) event->mask,
                    (jint) event->data.mouse.x,
                    (jint) event->data.mouse.y,
                    (jint) event->data.mouse.dx,
                    (jint) event->data.mouse.dy,
                    (jint) event->data.mouse.clicks,
                    (jint) event->data.mouse.button);

        case EVENT_MOUSE_DRAGGED:
            return (*env)->NewObject(
                    env,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent->cls,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent->init,
                    com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED,
                    (jint) event->mask,
                    (jint) event->data.mouse.x,
                    (jint) event->data.mouse.y,
                    (jint) event->data.mouse.dx,
                    (jint) event->data.mouse.dy,
                    (jint) event->data.mouse.clicks,
                    (jint) event->data.mouse.button);
    }

    return NULL;
}

static jobject jni_dispatchWheelEvent(JNIEnv *env, uiohook_event * const event) {
    switch (event->type) {
        case EVENT_MOUSE_WHEEL:
            if (event->data.wheel.delta == 0) {
                return NULL;
            }

            return (*env)->NewObject(
                env,
                com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->cls,
                com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->init,
                com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL,
                (jint) event->mask,
                (jint) event->data.wheel.x,
                (jint) event->data.wheel.y,
                (jint) 1, // clicks
                (jint) event->data.wheel.type,
                (jint) 1, // libuiohook reports scroll steps rather than platform line counts.
                (jint) event->data.wheel.rotation * -1 / event->data.wheel.delta,
                (jint) event->data.wheel.direction,
                (jdouble) event->data.wheel.rotation * -1 / (double) event->data.wheel.delta);
    }

    return NULL;
}

// NOTE: This function executes on the hook thread!  If you need to block
// please do so on another thread via your own event dispatcher.
void jni_EventDispatcher(uiohook_event * const event, void *user_data) {
    if (event == NULL) {
        return;
    }

    JNIEnv *env;
    jboolean attached;
    if (jni_GetCallbackEnv(&env, &attached) == JNI_OK) {
        if ((*env)->ExceptionCheck(env)) {
            jni_ReleaseCallbackEnv(env, attached);
            return;
        }

        if ((*env)->PushLocalFrame(env, 16) != JNI_OK) {
            jni_ReleaseCallbackEnv(env, attached);
            return;
        }

        jobject NativeInputEvent_obj = NULL;
        switch (event->type) {
            case EVENT_HOOK_DISABLED:
            case EVENT_HOOK_ENABLED:
                jni_notifyHookThread(env, event->type == EVENT_HOOK_ENABLED);
                break;

            case EVENT_KEY_PRESSED:
            case EVENT_KEY_RELEASED:
            case EVENT_KEY_TYPED:
                NativeInputEvent_obj = jni_dispatchKeyEvent(env, event, user_data);
                break;


            case EVENT_MOUSE_PRESSED:
            case EVENT_MOUSE_RELEASED:
            case EVENT_MOUSE_CLICKED:
                NativeInputEvent_obj = jni_dispatchButtonEvent(env, event);
                break;

            case EVENT_MOUSE_MOVED:
            case EVENT_MOUSE_DRAGGED:
                NativeInputEvent_obj = jni_dispatchMotionEvent(env, event);
                break;

            case EVENT_MOUSE_WHEEL:
                NativeInputEvent_obj = jni_dispatchWheelEvent(env, event);
                break;

            default:
                jni_Logger(LOG_LEVEL_INFO,    "%s [%u]: Unknown native event type: %#X.\n",
                        __FUNCTION__, __LINE__, event->type);
                break;
        }

        if (NativeInputEvent_obj != NULL && !(*env)->ExceptionCheck(env)) {
            // Set the private when field to the native event time.
            (*env)->SetLongField(
                    env,
                    NativeInputEvent_obj,
                    com_github_kwhat_jnativehook_NativeInputEvent->when,
                    (jlong) event->time);

            // Dispatch the event.
            (*env)->CallStaticVoidMethod(
                    env,
                    com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread->cls,
                    com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread->dispatchEvent,
                    NativeInputEvent_obj);

            if (!(*env)->ExceptionCheck(env)) {
                // Set the propagate flag from java.
                jint modifiers = (*env)->CallIntMethod(
                    env, 
                    NativeInputEvent_obj,
                    com_github_kwhat_jnativehook_NativeInputEvent->getModifiers);

                if ((uint32_t) modifiers & MASK_CONSUMED) {
                    event->mask |= MASK_CONSUMED;
                }
            }
        }

        (*env)->PopLocalFrame(env, NULL);
        jni_ReleaseCallbackEnv(env, attached);
    }
}
