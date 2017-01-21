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

#ifndef _Included_NativeErrors_h
#define _Included_NativeErrors_h

#ifdef DEBUG
#include <stdio.h>
#include <unistd.h>
#endif

#include <jni.h>
#include <stdlib.h>

#ifdef UNUSED
#elif defined(__GNUC__)
#define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
#define UNUSED(x) /*@unused@*/ x
#else
#define UNUSED(x) x
#endif

// Exception class definitions.
#define INTERNAL_ERROR				"java/lang/InternalError"
#define OUT_OF_MEMORY_ERROR			"java/lang/OutOfMemoryError"
#define NO_CLASS_DEF_FOUND_ERROR	"java/lang/NoClassDefFoundError"

#define NATIVE_HOOK_EXCEPTION		"org/jnativehook/NativeHookException"

/* Produces a hard error in the virtual machine.  This error is unrecoverable
 * and Program execution will terminate immediately.
 */
extern void ThrowFatalError(const char *message);

/* Produces a recoverable error in the virtual machine.  This error should be
 * recoverable outside of the native library.
 */
extern void ThrowException(const char *classname, const char *message);

#endif
