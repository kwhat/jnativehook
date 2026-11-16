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

#include "jni_Converter.h"
#include "jni_Errors.h"
#include "jni_Globals.h"
#include "jni_Logger.h"
#include "com_github_kwhat_jnativehook_keyboard_NativeKeyEvent.h"
#include "com_github_kwhat_jnativehook_mouse_NativeMouseEvent.h"
#include "com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent.h"
#include "com_github_kwhat_jnativehook_NativeInputEvent.h"


// Publish explicit lifecycle state instead of treating any wakeup as startup success.
static void notifyHookThread(JNIEnv *env, jboolean enabled) {
    jobject thread = (*env)->GetStaticObjectField(env,
            com_github_kwhat_jnativehook_GlobalScreen->cls,
            com_github_kwhat_jnativehook_GlobalScreen->hookThread);
    if (thread != NULL) {
        (*env)->CallVoidMethod(env, thread,
                com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread->setNativeHookState, enabled);
        (*env)->ExceptionCheck(env);
        (*env)->DeleteLocalRef(env, thread);
    }
}

// NOTE: This function executes on the hook thread!  If you need to block
// please do so on another thread via your own event dispatcher.
void jni_EventDispatcher(uiohook_event * const event, void *user_data) {
    JNIEnv *env;
    jboolean attached;
    if (event == NULL) {
        return;
    }
    // Java's char API uses UTF-16; wchar_t may contain a full scalar on Unix.
    if (event->type == EVENT_KEY_TYPED && (uint32_t) event->data.keyboard.keychar > 0xFFFF) {
        uint32_t scalar = (uint32_t) event->data.keyboard.keychar;
        if (scalar > 0x10FFFF) {
            return;
        }
        uiohook_event unit = *event;
        scalar -= 0x10000;
        unit.data.keyboard.keychar = (wchar_t) (0xD800 + (scalar >> 10));
        jni_EventDispatcher(&unit, user_data);
        unit.data.keyboard.keychar = (wchar_t) (0xDC00 + (scalar & 0x3FF));
        jni_EventDispatcher(&unit, user_data);
        event->mask |= unit.mask & MASK_CONSUMED;
        return;
    }
    uint16_t keycode = VC_UNDEFINED;
    if (event->type == EVENT_KEY_PRESSED || event->type == EVENT_KEY_RELEASED) {
        keycode = event->data.keyboard.keycode;
    }
    if (jni_GetCallbackEnv(&env, &attached) == JNI_OK) {
        if ((*env)->ExceptionCheck(env)) {
            return;
        }
        if ((*env)->PushLocalFrame(env, 16) != JNI_OK) {
            jni_ReleaseCallbackEnv(env, attached);
            return;
        }
        jobject NativeInputEvent_obj = NULL;
        jint location = com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_UNKNOWN;
        switch (event->type) {
            case EVENT_HOOK_DISABLED:
            case EVENT_HOOK_ENABLED:
                notifyHookThread(env, event->type == EVENT_HOOK_ENABLED);
                goto cleanup;


            case EVENT_KEY_PRESSED:
                if (jni_ConvertToJavaLocation(&keycode, &location) == JNI_OK) {
                    NativeInputEvent_obj = (*env)->NewObject(
                            env,
                            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->cls,
                            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->init,
                            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED,
                            (jint) event->mask,
                            (jint) event->data.keyboard.rawcode,
                            (jint) keycode,
                            (jchar) com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
                            location);
                }
                break;

            case EVENT_KEY_RELEASED:
                if (jni_ConvertToJavaLocation(&keycode, &location) == JNI_OK) {
                    NativeInputEvent_obj = (*env)->NewObject(
                            env,
                            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->cls,
                            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->init,
                            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED,
                            (jint) event->mask,
                            (jint) event->data.keyboard.rawcode,
                            (jint) keycode,
                            (jchar) com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_CHAR_UNDEFINED,
                            location);
                }
                break;

            case EVENT_KEY_TYPED:
                    NativeInputEvent_obj = (*env)->NewObject(
                            env,
                            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->cls,
                            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->init,
                            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED,
                            (jint) event->mask,
                            (jint) event->data.keyboard.rawcode,
                            (jint) com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_UNDEFINED,
                            (jchar) event->data.keyboard.keychar,
                            location);
                break;


            case EVENT_MOUSE_PRESSED:
                NativeInputEvent_obj = (*env)->NewObject(
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
                break;

            case EVENT_MOUSE_RELEASED:
                NativeInputEvent_obj = (*env)->NewObject(
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
                break;

            case EVENT_MOUSE_CLICKED:
                NativeInputEvent_obj = (*env)->NewObject(
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
                break;

            case EVENT_MOUSE_MOVED:
                NativeInputEvent_obj = (*env)->NewObject(
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
                break;

            case EVENT_MOUSE_DRAGGED:
                NativeInputEvent_obj = (*env)->NewObject(
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
                break;

            case EVENT_MOUSE_WHEEL:
                if (event->data.wheel.delta == 0) {
                    break;
                }

                NativeInputEvent_obj = (*env)->NewObject(
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

            if ((*env)->ExceptionCheck(env)) {
                goto cleanup;
            }

            // Set the propagate flag from java.
            jshort reserved = (*env)->GetShortField(
                    env,
                    NativeInputEvent_obj,
                    com_github_kwhat_jnativehook_NativeInputEvent->reserved);

            jint modifiers = (*env)->CallIntMethod(env, NativeInputEvent_obj,
                    com_github_kwhat_jnativehook_NativeInputEvent->getModifiers);
            if ((reserved & 0x01) || ((uint32_t) modifiers & MASK_CONSUMED)) {
                event->mask |= MASK_CONSUMED;
            }

            // Make sure our object is garbage collected.
            (*env)->DeleteLocalRef(env, NativeInputEvent_obj);
        }
cleanup:
        (*env)->PopLocalFrame(env, NULL);
        jni_ReleaseCallbackEnv(env, attached);
    }
}
