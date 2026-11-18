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

#include <jni.h>
#include <stdlib.h>

#include "jni_Errors.h"
#include "jni_Globals.h"
#include "jni_Logger.h"


void jni_ThrowFatalError(JNIEnv *env, const char *message) {
    // Throw a fatal error to the JVM.
    (*env)->FatalError(env, message);

    exit(EXIT_FAILURE);
}

void jni_ThrowException(JNIEnv *env, const char *classname, const char *message) {
    if ((*env)->ExceptionCheck(env)) {
        return;
    }

    jclass exception = (*env)->FindClass(env, classname);
    if (exception != NULL) {
        (*env)->ThrowNew(env, exception, message);
        (*env)->DeleteLocalRef(env, exception);
    }
}

void jni_ThrowNativeHookException(JNIEnv *env, jint code, const char *message) {
    if ((*env)->ExceptionCheck(env)) {
        return;
    }

    jstring text = (*env)->NewStringUTF(env, message);
    if (text == NULL) {
        return;
    }

    jobject exception = (*env)->NewObject(
        env, 
        com_github_kwhat_jnativehook_NativeHookException->cls,
        com_github_kwhat_jnativehook_NativeHookException->init, 
        (jint) code, 
        text);
    (*env)->DeleteLocalRef(env, text);

    if (exception != NULL) {
        (*env)->Throw(env, (jthrowable) exception);
        (*env)->DeleteLocalRef(env, exception);
    }
}
