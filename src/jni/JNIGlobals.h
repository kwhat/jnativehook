/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _Included_JNIGlobals_h
#define _Included_JNIGlobals_h

#ifdef DEBUG
#include <stdio.h>
#include <unistd.h>
#endif

#include <jni.h>

// Global Ref to the JVM.
// FIXME Move to a
extern JavaVM *jvm;
extern jint jni_version;

/* JNI requires that all calls to FindClass be made during JNI_OnLoad to
 * prevent NoClassDefError from popping up when various Java security models
 * are used.  The following structures are used to better organize the data
 * associated with each class.  Note that these structs do not cover all of the
 * available methods for each class; only methods used in native code are
 * included.
 */
typedef struct _GlobalScreen {
	jclass class;
	jmethodID getInstance;
	jmethodID dispatchEvent;
	jmethodID startEventDispatcher;
	jmethodID stopEventDispatcher;
} GlobalScreen;

typedef struct _org_jnativehook_keyboard_NativeKeyEvent {
	jclass class;
	jmethodID init;
} NativeKeyEvent;

typedef struct _NativeMouseEvent {
	jclass class;
	jmethodID init;
} NativeMouseEvent;

typedef struct _NativeMouseWheelEvent {
	jclass class;
	jmethodID init;
} NativeMouseWheelEvent;

// Global variables for Java object struct representation.
extern GlobalScreen *org_jnativehook_GlobalScreen;
extern NativeKeyEvent *org_jnativehook_keyboard_NativeKeyEvent;
extern NativeMouseEvent *org_jnativehook_mouse_NativeMouseEvent;
extern NativeMouseWheelEvent *org_jnativehook_mouse_NativeMouseWheelEvent;

// Create all of the JNI global references used throughout the native library.
extern int CreateJNIGlobals();

// Free all of the JNI globals created by the CreateJNIGlobals() function.
extern int DestroyJNIGlobals();

#endif
