/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2015 Alexander Barker.  All Rights Received.
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
#include <uiohook.h>

#include "jni_Errors.h"
#include "jni_EventDispathcer.h"
#include "jni_Globals.h"
#include "jni_Logger.h"
#include "jni_Properties.h"

// JNI Related global references.
JavaVM *jvm;
JavaVMAttachArgs jvm_attach_args = {
	.version = JNI_VERSION_1_4,
	.name = "JNativeHook Native Hook",
	.group = NULL
};

// JNI entry point, This is executed when the Java virtual machine attaches to the native library.
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
	// Grab the currently running virtual machine so we can attach to it in
	// functions that are not called from java.
	jvm = vm;
	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jvm_attach_args.version) == JNI_OK) {
		// Create all the global class references onload to prevent class loader
		// issues with JNLP and some IDE's.
		if (jni_CreateGlobals(env) == JNI_OK) {
			#ifndef USE_QUIET
			jclass PrintStream_class = (*env)->FindClass(env, "java/io/PrintStream");
			jfieldID out_id = (*env)->GetStaticFieldID(env, java_lang_System->cls, "out", "Ljava/io/PrintStream;");

			if (PrintStream_class != NULL && out_id != NULL) {
				jmethodID println_id = (*env)->GetMethodID(env, PrintStream_class, "println", "(Ljava/lang/String;)V");
				jobject out = (*env)->GetStaticObjectField(env, PrintStream_class, out_id);

				if (println_id != NULL && out != NULL) {
					jstring copyright = (*env)->NewStringUTF(env, "\n"
							"JNativeHook: Global keyboard and mouse hooking for Java.\n"
							"Copyright (C) 2006-2015 Alexander Barker.  All Rights Received.\n"
							"https://github.com/kwhat/jnativehook/\n"
							"\n"
							"JNativeHook is free software: you can redistribute it and/or modify\n"
							"it under the terms of the GNU Lesser General Public License as published\n"
							"by the Free Software Foundation, either version 3 of the License, or\n"
							"(at your option) any later version.\n"
							"\n"
							"JNativeHook is distributed in the hope that it will be useful,\n"
							"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
							"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
							"GNU General Public License for more details.\n"
							"\n"
							"You should have received a copy of the GNU Lesser General Public License\n"
							"along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");

					(*env)->CallVoidMethod(env, out, println_id, copyright);
				}
			}
			#endif

			// Set Java logger for native code messages.
			hook_set_logger_proc(&uiohook_LoggerCallback);

			// Set java properties from native sources.
			jni_SetProperties(env);

			// Set the hook callback function to dispatch events.
			hook_set_dispatch_proc(&jni_EventDispatcher);
		}
	}
	else {
		jni_ThrowFatalError(env, "Failed to acquire JNI interface pointer");
	}

	return jvm_attach_args.version;
}

// JNI exit point, This is executed when the Java virtual machine detaches from the native library.
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
	// Unset the hook callback function to dispatch events.
	hook_set_dispatch_proc(NULL);

	// Unset Java logger for native code messages.
	hook_set_logger_proc(NULL);

	// Grab the currently JNI interface pointer so we can cleanup the
	// system properties set on load.
	JNIEnv *env = NULL;
	if ((*jvm)->GetEnv(jvm, (void **)(&env), jvm_attach_args.version) == JNI_OK) {
		// Clear java properties from native sources.
		jni_ClearProperties(env);

		// Cleanup JNI global memory.
		jni_DestroyGlobals(env);
	}
	else {
		// It is not critical that these values are cleared so no exception
		// will be thrown.
		jni_Logger(env, LOG_LEVEL_WARN, "%s [%u]: Failed to acquire JNI interface pointer!\n",
				__FUNCTION__, __LINE__);
	}
}
