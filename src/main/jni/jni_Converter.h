/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2021 Alexander Barker.  All Rights Reserved.
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

#ifndef _Included_jni_Converter_h
#define _Included_jni_Converter_h

#include <jni.h>
#include <uiohook.h>


/* Converts a libuiohook input event type to a Java event ID. */
extern jint jni_ConvertToJavaType(event_type nativeType, jint *javaType);

/* Converts a Java event ID to a libuiohook input event type. */
extern jint jni_ConvertToNativeType(jint javaType, event_type *nativeType);

/* Converts a native keycode in place to its Java keycode and determines its location.
 * Unmapped keys produce Java VC_UNDEFINED and KEY_LOCATION_UNKNOWN. */
extern jint jni_ConvertToJavaLocation(unsigned short int *nativeKeyCode, jint *javaKeyLocation);

/* Converts a Java keycode and location to a libuiohook keycode.
 * KEY_LOCATION_UNKNOWN selects the first matching keycode in the conversion table. */
extern jint jni_ConvertToNativeKeycode(jint javaKeyCode, jint javaKeyLocation, uint16_t *nativeKeyCode);

#endif
