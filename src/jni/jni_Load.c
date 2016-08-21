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
#include <uiohook.h>

#include "jni_Errors.h"
#include "jni_EventDispathcer.h"
#include "jni_Globals.h"
#include "jni_Logger.h"

// JNI Related global references.
JavaVM *jvm;
JavaVMAttachArgs jvm_attach_args = {
	.version = JNI_VERSION_1_4,
	.name = "JNativeHook Library",
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
			// Set Java logger for native code messages.
			hook_set_logger_proc(&uiohook_LoggerCallback);

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
		// It is not critical that these values are cleared so no exception
		// will be thrown if this does not succeed.

		// Cleanup JNI global memory.
		jni_DestroyGlobals(env);
	}
}
