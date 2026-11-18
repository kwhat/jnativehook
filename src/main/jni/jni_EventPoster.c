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

#include <math.h>
#include <stdint.h>
#include <uiohook.h>

#include "com_github_kwhat_jnativehook_NativeInputEvent.h"
#include "com_github_kwhat_jnativehook_keyboard_NativeKeyEvent.h"
#include "com_github_kwhat_jnativehook_mouse_NativeMouseEvent.h"
#include "com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent.h"
#include "jni_Converter.h"
#include "jni_Errors.h"
#include "jni_EventPoster.h"
#include "jni_Globals.h"


static jboolean jni_readInt(JNIEnv *env, jobject event, jmethodID method, jint *value) {
    *value = (*env)->CallIntMethod(env, event, method);

    return !(*env)->ExceptionCheck(env);
}

static jboolean jni_requireEventClass(JNIEnv *env, jobject event, jclass expectedClass) {
    if (!(*env)->IsInstanceOf(env, event, expectedClass)) {
        jni_ThrowException(env, "java/lang/IllegalArgumentException", "Event class does not match its event type.");
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static jboolean jni_requireRange(JNIEnv *env, jint value, jint minimum, jint maximum, const char *message) {
    if (value < minimum || value > maximum) {
        jni_ThrowException(env, "java/lang/IllegalArgumentException", message);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static jboolean jni_readMousePosition(JNIEnv *env, jobject event, jint *x, jint *y) {
    if (!jni_readInt(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseEvent->getX, x) ||
        !jni_readInt(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseEvent->getY, y)
    ) {
        return JNI_FALSE;
    }

    if (!jni_requireRange(env, *x, INT16_MIN, INT16_MAX, "Pointer x is outside the native range.") ||
        !jni_requireRange(env, *y, INT16_MIN, INT16_MAX, "Pointer y is outside the native range.")
    ) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

jboolean jni_createKeyEvent(JNIEnv *env, jobject event, uiohook_event *native) {
    if (!jni_requireEventClass(env, event, com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->cls)) {
        return JNI_FALSE;
    }

    if (native->type == EVENT_KEY_TYPED) {
        native->data.keyboard.keychar = (*env)->CallCharMethod(env, event,
                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->getKeyChar);
        native->data.keyboard.keycode = VC_UNDEFINED;
        native->data.keyboard.rawcode = 0x00;

        return !(*env)->ExceptionCheck(env);
    }

    jint keyCode, keyLocation;
    if (!jni_readInt(env, event, com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->getKeyCode, &keyCode) ||
        !jni_readInt(env, event, com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->getKeyLocation, &keyLocation)
    ) {
        return JNI_FALSE;
    }

    if (jni_ConvertToNativeKeycode(keyCode, keyLocation, &native->data.keyboard.keycode) != JNI_OK) {
        jni_ThrowException(env, "java/lang/IllegalArgumentException", "Unsupported keycode or key location.");
        return JNI_FALSE;
    }

    native->data.keyboard.keychar = CHAR_UNDEFINED;
    native->data.keyboard.rawcode = 0x00;

    return JNI_TRUE;
}

jboolean jni_createButtonEvent(JNIEnv *env, jobject event, uiohook_event *native) {
    if (!jni_requireEventClass(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseEvent->cls)) {
        return JNI_FALSE;
    }

    jint x, y;
    if (!jni_readMousePosition(env, event, &x, &y)) {
        return JNI_FALSE;
    }

    native->data.mouse.x = (int16_t) x;
    native->data.mouse.y = (int16_t) y;

    jint clicks, button;
    if (!jni_readInt(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseEvent->getClickCount, &clicks) ||
        !jni_readInt(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseEvent->getButton, &button)
    ) {
        return JNI_FALSE;
    }

    if (!jni_requireRange(env, clicks, 0, UINT16_MAX, "Click count is outside the native range.") ||
        !jni_requireRange(env, button, MOUSE_NOBUTTON, MOUSE_BUTTON5, "Invalid mouse button.")
    ) {
        return JNI_FALSE;
    }

    native->data.mouse.clicks = (uint16_t) clicks;
    native->data.mouse.button = (uint16_t) button;

    return JNI_TRUE;
}

jboolean jni_createMotionEvent(JNIEnv *env, jobject event, uiohook_event *native) {
    if (!jni_requireEventClass(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseEvent->cls)) {
        return JNI_FALSE;
    }

    jint x, y;
    if (!jni_readMousePosition(env, event, &x, &y)) {
        return JNI_FALSE;
    }

    native->data.mouse.x = (int16_t) x;
    native->data.mouse.y = (int16_t) y;

    if (native->type == EVENT_MOUSE_MOVED) {
        native->data.mouse.button = MOUSE_NOBUTTON;
        native->data.mouse.clicks = 0;

        return JNI_TRUE;
    }

    jint clicks, button;
    if (!jni_readInt(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseEvent->getClickCount, &clicks) ||
        !jni_readInt(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseEvent->getButton, &button)
    ) {
        return JNI_FALSE;
    }

    if (!jni_requireRange(env, clicks, 0, UINT16_MAX, "Click count is outside the native range.") ||
        !jni_requireRange(env, button, MOUSE_NOBUTTON, MOUSE_BUTTON5, "Invalid mouse button.")
    ) {
        return JNI_FALSE;
    }

    native->data.mouse.clicks = (uint16_t) clicks;
    native->data.mouse.button = (uint16_t) button;

    return JNI_TRUE;
}

jboolean jni_createWheelEvent(JNIEnv *env, jobject event, uiohook_event *native) {
    if (!jni_requireEventClass(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->cls)) {
        return JNI_FALSE;
    }

    jint x, y;
    if (!jni_readMousePosition(env, event, &x, &y)) {
        return JNI_FALSE;
    }

    jint scrollType, direction;
    if (!jni_readInt(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->getScrollType, &scrollType) ||
        !jni_readInt(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->getWheelDirection, &direction)
    ) {
        return JNI_FALSE;
    }

    if (!jni_requireRange(env, scrollType, WHEEL_UNIT_SCROLL, WHEEL_BLOCK_SCROLL, "Invalid scroll type.") ||
        !jni_requireRange(env, direction, WHEEL_VERTICAL_DIRECTION, WHEEL_HORIZONTAL_DIRECTION, "Invalid wheel direction.")
    ) {
        return JNI_FALSE;
    }

    double rotation = (*env)->CallDoubleMethod(env, event, com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->getPreciseWheelRotation);
    if ((*env)->ExceptionCheck(env)) {
        return JNI_FALSE;
    }

    if (!isfinite(rotation) || rotation < -(double) INT16_MAX || rotation > -(double) INT16_MIN) {
        jni_ThrowException(env, "java/lang/IllegalArgumentException", "Wheel rotation is outside the native range.");
        return JNI_FALSE;
    }

    // Use the largest denominator that fits both native 16-bit fields.
    double magnitude = rotation < 0 ? -rotation : rotation;
    double limit = rotation < 0 ? INT16_MAX : -(double) INT16_MIN;
    double maxDelta = magnitude == 0 ? UINT16_MAX : limit / magnitude;
    uint16_t delta = maxDelta >= UINT16_MAX ? UINT16_MAX : (uint16_t) maxDelta;

    // Multiples of 120 retain exact conventional wheel fractions.
    if (delta >= 120) {
        delta -= delta % 120;
    }

    double scaled = -rotation * delta;
    int32_t rounded = (int32_t) (scaled + (scaled < 0 ? -0.5 : 0.5));
    if (rotation != 0 && rounded == 0) {
        jni_ThrowException(env, "java/lang/IllegalArgumentException", "Wheel rotation is smaller than the native resolution.");
        return JNI_FALSE;
    }

    native->data.wheel.x = (int16_t) x;
    native->data.wheel.y = (int16_t) y;
    native->data.wheel.type = (uint8_t) scrollType;
    native->data.wheel.direction = (uint8_t) direction;
    native->data.wheel.delta = delta;
    native->data.wheel.rotation = (int16_t) rounded;

    return JNI_TRUE;
}
