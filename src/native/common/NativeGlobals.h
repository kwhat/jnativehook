/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _Included_NativeGlobals_h
#define _Included_NativeGlobals_h

#ifdef DEBUG
#include <stdio.h>
#include <unistd.h>
#endif

#include <jni.h>

//Define some simple function return codes.
#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1

//Global Ref to the JVM
extern JavaVM * jvm;

//GlobalScreen object and dispatch id.
extern jobject objGlobalScreen;
extern jmethodID idDispatchEvent;

//Java callback classes and constructor id's
extern jclass clsKeyEvent, clsMouseEvent, clsMouseWheelEvent;
extern jmethodID idKeyEvent, idMouseButtonEvent, idMouseMotionEvent, idMouseWheelEvent;

//Common function stubs.
extern int CreateJNIGlobals();
extern int DestroyJNIGlobals();

#endif
