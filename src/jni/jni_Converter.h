/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2016 Alexander Barker.  All Rights Received.
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

extern jint jni_ConvertToJavaType(event_type nativeType, jint *javaType);

extern jint jni_ConvertToNativeType(jint javaType, event_type *nativeType);

extern jint jni_ConvertToJavaLocation(unsigned short int *nativeKeyCode, jint *javaKeyLocation);

#endif
