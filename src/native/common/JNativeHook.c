/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
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

#include "JNativeHook.h"

JavaVM * jvm = NULL;
JNIEnv * env_dll;
void jniFatalError(JNIEnv * env, const char * message) {
	#ifdef DEBUG
		fprintf(stderr, "Fatal Error - %s\n", message);
	#endif

	(*env)->FatalError(env, message);
	exit(EXIT_FAILURE);
}

void throwException(JNIEnv * _env, const char * classname, const char * message) {
	JNIEnv * env;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Locate our exception class
	jclass clsException = (*env)->FindClass(env, classname);

	if (clsException != NULL) {
		#ifdef DEBUG
			fprintf(stderr, "Exception - %s\n", message);
		#endif

		(*env)->ThrowNew(env, clsException, message);
		(*env)->DeleteLocalRef(env, clsException);
	}
	else {
		//Unable to find exception class, Terminate with error.
		jniFatalError(env, "Unable to locate exception class.");
	}
}
