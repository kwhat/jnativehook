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
	// Locate our exception class.
	jclass Exception_class = (*env)->FindClass(env, classname);
	if (Exception_class != NULL) {
		(*env)->ThrowNew(env, Exception_class, message);
		(*env)->DeleteLocalRef(env, Exception_class);
	}
	else {
		// Throw a ClassNotFoundException if we could not locate the exception class above.
		Exception_class = (*env)->FindClass(env, "java/lang/ClassNotFoundException");
		if (Exception_class != NULL) {
			(*env)->ThrowNew(env, Exception_class, classname);
			(*env)->DeleteLocalRef(env, Exception_class);
		}
		else {
			jni_ThrowFatalError(env, "Failed to locate core class: java.lang.ClassNotFoundException");
		}
	}
}

void jni_ThrowNativeHookException(JNIEnv *env, short code, const char *message) {
	jobject Exception_object = (*env)->NewObject(env, org_jnativehook_NativeHookException->cls,
			org_jnativehook_NativeHookException->init, (jint) code, (*env)->NewStringUTF(env, message));
	(*env)->Throw(env, (jthrowable) Exception_object);
	(*env)->DeleteLocalRef(env, Exception_object);
}
