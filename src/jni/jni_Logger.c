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

#include <jni.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <uiohook.h>

#include "jni_Errors.h"
#include "jni_Globals.h"

static char log_buffer[1024];
static bool logger(JNIEnv *env, unsigned int level, const char *format, va_list args) {
	bool status = false;

	int log_size = vsnprintf(log_buffer, sizeof(log_buffer), format, args);

	if (log_size >= 0) {
		jstring name = (*env)->NewStringUTF(env, "org.jnativehook");
		jstring message = (*env)->NewStringUTF(env, log_buffer);

		jobject Logger_object = (*env)->CallStaticObjectMethod(
				env,
				java_util_logging_Logger->cls,
				java_util_logging_Logger->getLogger,
				name);

		switch (level) {
			case LOG_LEVEL_DEBUG:
				(*env)->CallVoidMethod(
					env,
					Logger_object,
					java_util_logging_Logger->fine,
					message);
				break;

			case LOG_LEVEL_INFO:
				(*env)->CallVoidMethod(
					env,
					Logger_object,
					java_util_logging_Logger->info,
					message);
				break;

			case LOG_LEVEL_WARN:
				(*env)->CallVoidMethod(
					env,
					Logger_object,
					java_util_logging_Logger->warning,
					message);
				break;

			case LOG_LEVEL_ERROR:
				(*env)->CallVoidMethod(
					env,
					Logger_object,
					java_util_logging_Logger->severe,
					message);
				break;
		}

		(*env)->DeleteLocalRef(env, name);
		(*env)->DeleteLocalRef(env, message);
		(*env)->DeleteLocalRef(env, Logger_object);

		status = true;
	}

	va_end(args);

	return status;
}

bool jni_Logger(JNIEnv *env, unsigned int level, const char *format, ...) {
	va_list args;
	va_start(args, format);

	return logger(env, level, format, args);
}

bool uiohook_LoggerCallback(unsigned int level, const char *format, ...) {
	bool status = false;

	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jvm_attach_args.version) == JNI_OK) {
		va_list args;
		va_start(args, format);
		status = logger(env, level, format, args);
	}
	else {
		jni_Logger(env, LOG_LEVEL_ERROR, "%s [%u]: GetEnv failed!\n",
				__FUNCTION__, __LINE__);
	}

	return status;
}
