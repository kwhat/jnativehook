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

#ifndef _Included_jni_Errors_h
#define _Included_jni_Errors_h

/* Produces a fatal error in the virtual machine.  This error is unrecoverable
 * and program execution will terminate immediately.
 */
extern void jni_ThrowFatalError(JNIEnv *env, const char *message);

/* Produces and throw a general exception to the virtual machine.  This error may or may
 * not be recoverable outside of the native library.
 */
extern void jni_ThrowException(JNIEnv *env, const char *classname, const char *message);

/* Produces a specific NativeHookException containing an error code indicating what might
 * have gone wrong.
 */
extern void jni_ThrowNativeHookException(JNIEnv *env, short code, const char *message);

#endif
