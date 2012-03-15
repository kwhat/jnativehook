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

#include "NativeErrors.h"
#include "NativeGlobals.h"

void ThrowFatalError(JNIEnv * env, const char * message) {
	#ifdef DEBUG
	fprintf(stderr, "Fatal Error: %s\n", message);
	#endif

	(*env)->FatalError(env, message);
	exit(EXIT_FAILURE);
}

void ThrowException(JNIEnv * env, const char * classname, const char * message) {
	/* Locate our exception class */
	jclass clsException = (*env)->FindClass(env, classname);

	if (clsException != NULL) {
		(*env)->ThrowNew(env, clsException, message);
		#ifdef DEBUG
		(*env)->ExceptionDescribe(env);
		#endif
		(*env)->DeleteLocalRef(env, clsException);
	}
	else {
		clsException = (*env)->FindClass(env, "java/lang/NoClassDefFoundError");

		if (clsException != NULL) {
			(*env)->ThrowNew(env, clsException, classname);
			#ifdef DEBUG
			(*env)->ExceptionDescribe(env);
			#endif
			(*env)->DeleteLocalRef(env, clsException);
		}
		else {
			/* Unable to find exception class, Terminate with error. */
			ThrowFatalError(env, "Unable to locate exception class.");
		}
	}
}
