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
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <uiohook.h>

#include "jni_Errors.h"
#include "jni_Globals.h"
#include "jni_Logger.h"




void jni_Logger(unsigned int level, const char *format, ...) {
    va_list args;

    va_start(args, format);
    jni_LoggerProc(level, NULL, format, args);
    va_end(args);
}

void jni_LoggerProc(unsigned int level, void *user_data, const char *format, va_list args) {
    char buffer[1024];
    JNIEnv *env = NULL;
    jboolean attached;
    if (jni_GetCallbackEnv(&env, &attached) != JNI_OK) return;
    if ((*env)->ExceptionCheck(env)) return;
    if ((*env)->PushLocalFrame(env, 16) != JNI_OK) {
        jni_ReleaseCallbackEnv(env, attached);
        return;
    }
    int size = vsnprintf(buffer, sizeof(buffer), format, args);
    if (size < 0) goto cleanup;
    // Native log strings use UTF-8, whereas NewStringUTF expects modified UTF-8.
    jsize length = (jsize) (size < (int) sizeof(buffer) ? size : (int) sizeof(buffer) - 1);
    jbyteArray bytes = (*env)->NewByteArray(env, length);
    if (bytes == NULL) goto cleanup;
    (*env)->SetByteArrayRegion(env, bytes, 0, length, (const jbyte *) buffer);
    if ((*env)->ExceptionCheck(env)) goto cleanup;
    jclass stringClass = (*env)->FindClass(env, "java/lang/String");
    if (stringClass == NULL) goto cleanup;
    jmethodID constructor = (*env)->GetMethodID(env, stringClass, "<init>", "([BLjava/lang/String;)V");
    if (constructor == NULL) goto cleanup;
    jstring charset = (*env)->NewStringUTF(env, "UTF-8");
    if (charset == NULL) goto cleanup;
    jstring message = (*env)->NewObject(env, stringClass, constructor, bytes, charset);
    if (message == NULL) goto cleanup;
    // Logger.getLogger is caller-sensitive and fails on native-created threads.
    jobject logger = java_util_logging_Logger->object;
    jmethodID method = NULL;
    switch (level) {
        case LOG_LEVEL_DEBUG: method = java_util_logging_Logger->fine; break;
        case LOG_LEVEL_INFO: method = java_util_logging_Logger->info; break;
        case LOG_LEVEL_WARN: method = java_util_logging_Logger->warning; break;
        case LOG_LEVEL_ERROR: method = java_util_logging_Logger->severe; break;
    }
    if (method != NULL) {
        (*env)->CallVoidMethod(env, logger, method, message);
        (*env)->ExceptionCheck(env);
    }
cleanup:
    (*env)->PopLocalFrame(env, NULL);
    jni_ReleaseCallbackEnv(env, attached);
}
