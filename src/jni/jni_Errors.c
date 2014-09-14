/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2014 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
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

void jni_ThrowFatalError(JNIEnv *env, const char *message) {
	#ifdef USE_DEBUG
	fprintf(stderr, "Fatal Error: %s\n", message);
	#endif

	(*env)->FatalError(env, message);

	exit(EXIT_FAILURE);
}

void jni_ThrowException(JNIEnv *env, const char *classname, const char *message) {
	// Locate our exception class.
	// FIXME This needs to be relocated to a jni_Global.
	jclass Exception_class = (*env)->FindClass(env, classname);

	if (Exception_class != NULL) {
		(*env)->ThrowNew(env, Exception_class, message);
		#ifdef USE_DEBUG
		fprintf(stderr, "%s: %s\n", classname, message);
		#endif
		(*env)->DeleteLocalRef(env, Exception_class);
	}
	else {
		Exception_class = (*env)->FindClass(env, java_lang_NoClassDefFoundError);

		if (Exception_class != NULL) {
			#ifdef USE_DEBUG
			fprintf(stderr, "%s: %s\n", java_lang_NoClassDefFoundError, classname);
			#endif

			(*env)->ThrowNew(env, Exception_class, classname);
			(*env)->DeleteLocalRef(env, Exception_class);
		}
		else {
			// Unable to find exception class, Terminate with error.
			jni_ThrowFatalError(env, "Unable to locate exception class.");
		}
	}
}
