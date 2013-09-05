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

bool jni_LogDebug(const char * message) {
	bool status = false;
	
	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		(*env)->CallVoidMethod(
				env, 
				logger, 
				java_util_logging_Logger->fine, 
				(*env)->NewStringUTF(env, message));
		
		status = true;
	}
	
	return status;
}

bool jni_LogInfo(const char * message) {
	bool status = false;
	
	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		(*env)->CallVoidMethod(
				env, 
				logger, 
				java_util_logging_Logger->info, 
				(*env)->NewStringUTF(env, message));
		
		status = true;
	}
	
	return status;
}

bool jni_LogWarn(const char * message) {
	bool status = false;
	
	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		(*env)->CallVoidMethod(
				env, 
				logger, 
				java_util_logging_Logger->warning, 
				(*env)->NewStringUTF(env, message));
	}
	
	return status;
}

bool jni_LogError(const char * message) {
	bool status = false;

	JNIEnv *env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) == JNI_OK) {
		(*env)->CallVoidMethod(
				env, 
				logger, 
				java_util_logging_Logger->severe, 
				(*env)->NewStringUTF(env, message));
	}
	
	return status;
}

void jni_SetLogger(JNIEnv *env) {
	bool hook_set_logger_proc(log_level level, bool (*logger_proc)(const char *));

	logger = (*env)->CallStaticObjectMethod(
			env, 
			java_util_logging_Logger->cls, 
			java_util_logging_Logger->getLogger, 
			(*env)->NewStringUTF(env, "GlobalScreen"));
	
	// TODO Verify return values.
	hook_set_logger_proc(LOG_LEVEL_DEBUG, &jni_LogDebug);
	hook_set_logger_proc(LOG_LEVEL_INFO, &jni_LogInfo);
	hook_set_logger_proc(LOG_LEVEL_WARN, &jni_LogWarn);
	hook_set_logger_proc(LOG_LEVEL_ERROR, &jni_LogError);
}
