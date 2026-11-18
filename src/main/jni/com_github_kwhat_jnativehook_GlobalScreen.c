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

#include <stdint.h>
#include <stdlib.h>
#include <uiohook.h>

#include "com_github_kwhat_jnativehook_GlobalScreen.h"
#include "com_github_kwhat_jnativehook_NativeInputEvent.h"
#include "jni_Converter.h"
#include "jni_Errors.h"
#include "jni_EventPoster.h"
#include "jni_Globals.h"
#include "jni_Logger.h"


JNIEXPORT void JNICALL Java_com_github_kwhat_jnativehook_GlobalScreen_00024NativeHookThread_enable(JNIEnv *env, jobject Thread_obj) {
    int status = hook_run();

    switch (status) {
        // Success
        case UIOHOOK_SUCCESS:
            break;


        // System level errors.
        case UIOHOOK_ERROR_OUT_OF_MEMORY:
            jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
            break;

        // X11 specific errors.
        case UIOHOOK_ERROR_X_OPEN_DISPLAY:
            jni_ThrowNativeHookException(env, status, "Failed to open X11 display.");
            break;

        case UIOHOOK_ERROR_X_RECORD_NOT_FOUND:
            jni_ThrowNativeHookException(env, status, "Unable to locate XRecord extension.");
            break;

        case UIOHOOK_ERROR_X_RECORD_ALLOC_RANGE:
            jni_ThrowNativeHookException(env, status, "Unable to allocate XRecord range.");
            break;

        case UIOHOOK_ERROR_X_RECORD_CREATE_CONTEXT:
            jni_ThrowNativeHookException(env, status, "Unable to allocate XRecord context.");
            break;

        case UIOHOOK_ERROR_X_RECORD_ENABLE_CONTEXT:
            jni_ThrowNativeHookException(env, status, "Failed to enable XRecord context.");
            break;


        case UIOHOOK_ERROR_EPOLL_CREATE:
            jni_ThrowNativeHookException(env, status, "Failed to create the input event polling instance.");
            break;

        // Windows specific errors.
        case UIOHOOK_ERROR_SET_WINDOWS_HOOK_EX:
            jni_ThrowNativeHookException(env, status, "Failed to register low level windows hook.");
            break;


        // Darwin specific errors.
        case UIOHOOK_ERROR_AXAPI_DISABLED:
            jni_ThrowNativeHookException(env, status, "Failed to enable access for assistive devices.");
            break;

        case UIOHOOK_ERROR_CREATE_EVENT_PORT:
            jni_ThrowNativeHookException(env, status, "Failed to create apple event port.");
            break;

        case UIOHOOK_ERROR_CREATE_RUN_LOOP_SOURCE:
            jni_ThrowNativeHookException(env, status, "Failed to create apple run loop source.");
            break;

        case UIOHOOK_ERROR_GET_RUNLOOP:
            jni_ThrowNativeHookException(env, status, "Failed to acquire apple run loop.");
            break;

        case UIOHOOK_ERROR_CREATE_OBSERVER:
            jni_ThrowNativeHookException(env, status, "Failed to create apple run loop observer.");
            break;


        // Default error.
        default:
            jni_ThrowNativeHookException(env, status, "An unknown hook error occurred.");
            break;
    }
}

JNIEXPORT void JNICALL Java_com_github_kwhat_jnativehook_GlobalScreen_00024NativeHookThread_disable(JNIEnv *env, jobject Thread_obj) {
    int status = hook_stop();

    // Only a handful of the total errors are possible on stop.
    switch (status) {
        // Success
        case UIOHOOK_SUCCESS:
            break;


        // System level errors.
        case UIOHOOK_ERROR_OUT_OF_MEMORY:
            jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
            break;

        // Unix specific errors.
        case UIOHOOK_ERROR_X_RECORD_GET_CONTEXT:
            jni_ThrowNativeHookException(env, status, "Failed to get XRecord context.");
            break;


        // Windows specific errors.
        // There are no Windows specific errors at this time.


        // Darwin specific errors.
        // There are no Darwin specific errors at this time.

        // Default error.
        default:
            jni_ThrowNativeHookException(env, status, "An unknown hook error occurred.");
            break;
    }
}

/*
 * Class:     com_github_kwhat_jnativehook_GlobalScreen
 * Method:    postNativeEvent
 * Signature: (Lcom/github/kwhat/jnativehook/NativeInputEvent;)V
 */
JNIEXPORT void JNICALL Java_com_github_kwhat_jnativehook_GlobalScreen_postNativeEvent(JNIEnv *env, jclass GlobalScreen_cls, jobject event) {
    if (event == NULL) {
        jni_ThrowException(env, "java/lang/NullPointerException", "event");
        return;
    }

    jint modifiers = (*env)->CallIntMethod(env, event, com_github_kwhat_jnativehook_NativeInputEvent->getModifiers);
    if ((*env)->ExceptionCheck(env)) {
        return;
    }
    if ((uint32_t) modifiers & MASK_CONSUMED) {
        return;
    }

    jint javaType = (*env)->CallIntMethod(env, event, com_github_kwhat_jnativehook_NativeInputEvent->getID);
    if ((*env)->ExceptionCheck(env)) {
        return;
    }

    uiohook_event native = { 0 };
    native.mask = (uint32_t) modifiers;
    if (jni_ConvertToNativeType(javaType, &native.type) != JNI_OK) {
        jni_ThrowException(env, "java/lang/IllegalArgumentException", "Unsupported native event type.");
        return;
    }

    jboolean valid = JNI_FALSE;
    switch (native.type) {
        case EVENT_KEY_TYPED:
        case EVENT_KEY_PRESSED:
        case EVENT_KEY_RELEASED:
            valid = jni_createKeyEvent(env, event, &native);
            break;

        case EVENT_MOUSE_CLICKED:
        case EVENT_MOUSE_PRESSED:
        case EVENT_MOUSE_RELEASED:
            valid = jni_createButtonEvent(env, event, &native);
            break;

        case EVENT_MOUSE_MOVED:
        case EVENT_MOUSE_DRAGGED:
            valid = jni_createMotionEvent(env, event, &native);
            break;

        case EVENT_MOUSE_WHEEL:
            valid = jni_createWheelEvent(env, event, &native);
            break;

        default:
            jni_ThrowException(env, "java/lang/IllegalArgumentException", "Unsupported native event type.");
            return;
    }

    if (!valid) {
        return;
    }

    int status = hook_post_event(&native);

    // A synchronous callback may have raised an exception while posting.
    if ((*env)->ExceptionCheck(env)) {
        return;
    }

    if (status != UIOHOOK_SUCCESS) {
        jni_ThrowNativeHookException(env, status, "Failed to post native input event.");
    }
}

/*
 * Class:     com_github_kwhat_jnativehook_GlobalScreen
 * Method:    getNativeMonitors
 * Signature: ()[Lcom/github/kwhat/jnativehook/MonitorInfo;
 */
JNIEXPORT jobjectArray JNICALL Java_com_github_kwhat_jnativehook_GlobalScreen_getNativeMonitors(JNIEnv *env, jclass GlobalScreen_cls) {
    unsigned char count = 0;
    screen_data *screens = hook_create_screen_info(&count);

    jobjectArray result = (*env)->NewObjectArray(env, (jsize) count, com_github_kwhat_jnativehook_NativeMonitorInfo->cls, NULL);
     if (result != NULL) {
        for (int i = 0; i < count; i++) {
            jobject monitor = (*env)->NewObject(
                    env,
                    com_github_kwhat_jnativehook_NativeMonitorInfo->cls,
                    com_github_kwhat_jnativehook_NativeMonitorInfo->init,
                    (jshort) screens[i].number,
                    (jint) screens[i].x,
                    (jint) screens[i].y,
                    (jint) screens[i].width,
                    (jint) screens[i].height);

            if (monitor == NULL) {
                break;
            }

            (*env)->SetObjectArrayElement(env, result, (jsize) i, monitor);
            (*env)->DeleteLocalRef(env, monitor);

            if ((*env)->ExceptionCheck(env)) {
                break;
            }
        }
     } else {
         jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
     }

     free(screens);
     return result;
}

/*
 * Class:     com_github_kwhat_jnativehook_GlobalScreen
 * Method:    getAutoRepeatRate
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_com_github_kwhat_jnativehook_GlobalScreen_getAutoRepeatRate(JNIEnv *env, jclass GlobalScreen_cls) {
    jobject result = NULL;

    // Set the native keyboard auto repeat rate.
    long rate = hook_get_auto_repeat_rate();
    if (rate >= 0) {
        result = (*env)->NewObject(
                env,
                java_lang_Integer->cls,
                java_lang_Integer->init,
                (jint) rate);
    } else {
        jni_Logger(LOG_LEVEL_WARN, "%s [%u]: Invalid result returned from hook_get_auto_repeat_rate()!\n",
                __FUNCTION__, __LINE__);
    }

    return result;
}

/*
 * Class:     com_github_kwhat_jnativehook_GlobalScreen
 * Method:    getAutoRepeatDelay
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_com_github_kwhat_jnativehook_GlobalScreen_getAutoRepeatDelay(JNIEnv *env, jclass GlobalScreen_cls) {
    jobject result = NULL;

    long delay = hook_get_auto_repeat_delay();
    if (delay >= 0) {
        result = (*env)->NewObject(
                env,
                java_lang_Integer->cls,
                java_lang_Integer->init,
                (jint) delay);
    } else {
        jni_Logger(LOG_LEVEL_WARN, "%s [%u]: Invalid result returned from hook_get_auto_repeat_delay()!\n",
                __FUNCTION__, __LINE__);
    }

    return result;
}

/*
 * Class:     com_github_kwhat_jnativehook_GlobalScreen
 * Method:    getPointerAccelerationMultiplier
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_com_github_kwhat_jnativehook_GlobalScreen_getPointerAccelerationMultiplier(JNIEnv *env, jclass GlobalScreen_cls) {
    jobject result = NULL;

    long multiplier = hook_get_pointer_acceleration_multiplier();
    if (multiplier >= 0) {
        result = (*env)->NewObject(
                env,
                java_lang_Integer->cls,
                java_lang_Integer->init,
                (jint) multiplier);
    } else {
        jni_Logger(LOG_LEVEL_WARN, "%s [%u]: Invalid result returned from hook_get_pointer_acceleration_multiplier()!\n",
                __FUNCTION__, __LINE__);
    }

    return result;
}

/*
 * Class:     com_github_kwhat_jnativehook_GlobalScreen
 * Method:    getPointerAccelerationThreshold
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_com_github_kwhat_jnativehook_GlobalScreen_getPointerAccelerationThreshold(JNIEnv *env, jclass GlobalScreen_cls) {
    jobject result = NULL;

    long threshold = hook_get_pointer_acceleration_threshold();
    if (threshold >= 0) {
        result = (*env)->NewObject(
                env,
                java_lang_Integer->cls,
                java_lang_Integer->init,
                (jint) threshold);
    } else {
        jni_Logger(LOG_LEVEL_WARN, "%s [%u]: Invalid result returned from hook_get_pointer_acceleration_threshold()!\n",
                __FUNCTION__, __LINE__);
    }

    return result;
}

/*
 * Class:     com_github_kwhat_jnativehook_GlobalScreen
 * Method:    getPointerSensitivity
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_com_github_kwhat_jnativehook_GlobalScreen_getPointerSensitivity(JNIEnv *env, jclass GlobalScreen_cls) {
    jobject result = NULL;

    long sensitivity = hook_get_pointer_sensitivity();
    if (sensitivity >= 0) {
        result = (*env)->NewObject(
                env,
                java_lang_Integer->cls,
                java_lang_Integer->init,
                (jint) sensitivity);
    } else {
        jni_Logger(LOG_LEVEL_WARN, "%s [%u]: Invalid result returned from hook_get_pointer_sensitivity()!\n",
                __FUNCTION__, __LINE__);
    }

    return result;
}

/*
 * Class:     com_github_kwhat_jnativehook_GlobalScreen
 * Method:    getMultiClickTime
 * Signature: ()Ljava/lang/Integer;
 */
JNIEXPORT jobject JNICALL Java_com_github_kwhat_jnativehook_GlobalScreen_getMultiClickIterval(JNIEnv *env, jclass GlobalScreen_cls) {
    jobject result = NULL;

    long clicktime = hook_get_multi_click_time();
    if (clicktime >= 0) {
        result = (*env)->NewObject(
                env,
                java_lang_Integer->cls,
                java_lang_Integer->init,
                (jint) clicktime);
    } else {
        jni_Logger(LOG_LEVEL_WARN, "%s [%u]: Invalid result returned from hook_get_multi_click_time()!\n",
                __FUNCTION__, __LINE__);
    }

    return result;
}
