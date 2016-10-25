/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2015 Alexander Barker.  All Rights Received.
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

#ifndef _Included_jni_Globals_h
#define _Included_jni_Globals_h

#include <jni.h>

// Globals for the jvm and jni version, this is set in JNILoad.c
extern JavaVM *jvm;
extern JavaVMAttachArgs jvm_attach_args;

/* JNI requires that all calls to FindClass be made during JNI_OnLoad to
 * prevent NoClassDefError from popping up when various Java security models
 * are used.  The following structures are used to better organize the data
 * associated with each class.  Note that these structs do not cover all of the
 * available methods for each class; only methods used in native code are
 * included.
 */
typedef struct _org_jnativehook_GlobalScreen {
	jclass cls;
	jfieldID hookThread;
	jmethodID dispatchEvent;
} GlobalScreen;

typedef struct _org_jnativehook_NativeHookException {
	jclass cls;
	jmethodID init;
} NativeHookException;

typedef struct _org_jnativehook_NativeInputEvent {
	jclass cls;
	jfieldID reserved;
	jmethodID init;
	jmethodID getID;
	jmethodID getModifiers;
} NativeInputEvent;

typedef struct _org_jnativehook_keyboard_NativeKeyEvent {
	jclass cls;
	jmethodID init;
	NativeInputEvent *parent;
	jmethodID getKeyCode;
	jmethodID getKeyLocation;
	jmethodID getKeyChar;
	jmethodID getKeyboardName;
} NativeKeyEvent;

typedef struct _org_jnativehook_mouse_NativeMouseEvent {
	jclass cls;
	jmethodID init;
	NativeInputEvent *parent;
	jmethodID getButton;
	jmethodID getClickCount;
	jmethodID getX;
	jmethodID getY;
} NativeMouseEvent;

typedef struct _org_jnativehook_mouse_NativeMouseWheelEvent {
	jclass cls;
	jmethodID init;
	NativeMouseEvent *parent;
	jmethodID getScrollAmount;
	jmethodID getScrollType;
	jmethodID getWheelRotation;
} NativeMouseWheelEvent;

typedef struct _java_lang_Object {
	jclass cls;
	jmethodID notify;
} Object;

typedef struct _java_lang_System {
	jclass cls;
	jmethodID setProperty;
	jmethodID clearProperty;
} System;

typedef struct _java_util_logging_Logger {
	jclass cls;
	jmethodID getLogger;
	jmethodID fine;
	jmethodID info;
	jmethodID warning;
	jmethodID severe;
} Logger;

// Global variables for Java object struct representation.
extern GlobalScreen *org_jnativehook_GlobalScreen;
extern NativeHookException *org_jnativehook_NativeHookException;
extern NativeInputEvent *org_jnativehook_NativeInputEvent;
extern NativeKeyEvent *org_jnativehook_keyboard_NativeKeyEvent;
extern NativeMouseEvent *org_jnativehook_mouse_NativeMouseEvent;
extern NativeMouseWheelEvent *org_jnativehook_mouse_NativeMouseWheelEvent;
extern Object *java_lang_Object;
extern System *java_lang_System;
extern Logger *java_util_logging_Logger;

// Create all of the JNI global references used throughout the native library.
extern int jni_CreateGlobals(JNIEnv *env);

// Free all of the JNI globals created by the CreateJNIGlobals() function.
extern int jni_DestroyGlobals(JNIEnv *env);

#endif
