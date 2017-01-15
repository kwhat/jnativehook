/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
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

#ifndef _Included_NativeGlobals_h
#define _Included_NativeGlobals_h

#ifdef DEBUG
#include <stdio.h>
#include <unistd.h>
#endif

#include <jni.h>

// Define some simple function return codes.
#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1

// Global Ref to the JVM.
extern JavaVM *jvm;
extern jint jni_version;

// GlobalScreen object and dispatch id.
extern jmethodID idGetInstance, idDispatchEvent, idStartEventDispatcher, idStopEventDispatcher;

// Java callback classes and constructor id's.
extern jclass clsGlobalScreen, clsKeyEvent, clsMouseEvent, clsMouseWheelEvent;
extern jmethodID idKeyEvent, idMouseButtonEvent, idMouseMotionEvent, idMouseWheelEvent;

// Thread class and methods to set the name.
extern jclass clsThread;
extern jmethodID idCurrentThread, idSetName;

// Create all of the JNI global references used throughout the native library.
extern int CreateJNIGlobals();

// Free all of the JNI globals created by the CreateJNIGlobals() function.
extern int DestroyJNIGlobals();

#endif
