/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
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

#include <jni.h>
#include <nativehook.h>
#include <stdbool.h>
#include <stdlib.h>

#include "jni_Errors.h"
#include "jni_Globals.h"

static jobject logger = NULL;
static char log_buffer[1024];

bool jni_Logger(unsigned int level, const char *format, ...) {
	bool status = false;

	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		va_list args;
		va_start(args, format);
		int log_size = vsnprintf(log_buffer, sizeof(log_buffer), format, args);
		va_end(args);
	
		if (log_size >= 0) {
			jstring message = (*env)->NewStringUTF(env, log_buffer);

			switch (level) {
				case LOG_LEVEL_DEBUG:
					(*env)->CallVoidMethod(
						env, 
						logger, 
						java_util_logging_Logger->fine, 
						message);
					break;
				case LOG_LEVEL_INFO:
					(*env)->CallVoidMethod(
						env, 
						logger, 
						java_util_logging_Logger->info, 
						message);
					break;

				case LOG_LEVEL_WARN:
					(*env)->CallVoidMethod(
						env, 
						logger, 
						java_util_logging_Logger->warning, 
						message);
					break;

				case LOG_LEVEL_ERROR:
					(*env)->CallVoidMethod(
						env, 
						logger, 
						java_util_logging_Logger->severe, 
						message);
					break;
			}

			status = true;
		}
	}
	
	return status;
}
