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

#ifndef _Included_jni_EventPoster_h
#define _Included_jni_EventPoster_h

#include <jni.h>
#include <uiohook.h>


/* Creates native keyboard data from a Java keyboard event.
 * Returns JNI_FALSE if conversion fails or a Java exception is pending. */
extern jboolean jni_createKeyEvent(JNIEnv *env, jobject event, uiohook_event *nativeEvent);

/* Creates native button data from a Java mouse event.
 * Returns JNI_FALSE if conversion fails or a Java exception is pending. */
extern jboolean jni_createButtonEvent(JNIEnv *env, jobject event, uiohook_event *nativeEvent);

/* Creates native motion data from a Java mouse event.
 * Returns JNI_FALSE if conversion fails or a Java exception is pending. */
extern jboolean jni_createMotionEvent(JNIEnv *env, jobject event, uiohook_event *nativeEvent);

/* Creates native wheel data from a Java wheel event.
 * Returns JNI_FALSE if conversion fails or a Java exception is pending. */
extern jboolean jni_createWheelEvent(JNIEnv *env, jobject event, uiohook_event *nativeEvent);

#endif
