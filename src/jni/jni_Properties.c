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

void jni_SetProperties(JNIEnv *env) {
	jclass System_class = (*env)->FindClass(env, "java/lang/System");
	jmethodID setProperty_id = (*env)->GetStaticMethodID(env, System_class, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

	if (System_class != NULL && setProperty_id != NULL) {
		// Create a buffer for converting numbers to strings.
		char buffer[16];

		// Set the native keyboard auto repeat rate.
		long rate = hook_get_auto_repeat_rate();
		if (rate >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetAutoRepeatRate(): successful. (rate: %li)\n", rate);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", rate) >= 0) {
				(*env)->CallStaticObjectMethod(env, System_class, setProperty_id, (*env)->NewStringUTF(env, "jnativehook.autoRepeatRate"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetAutoRepeatRate(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "GetAutoRepeatRate(): failure!\n");
		}
		#endif


		long delay = hook_get_auto_repeat_delay();
		if (delay >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetAutoRepeatDelay(): successful. (delay: %li)\n", delay);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", delay) >= 0) {
				(*env)->CallStaticObjectMethod(env, System_class, setProperty_id, (*env)->NewStringUTF(env, "jnativehook.autoRepeatDelay"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetAutoRepeatDelay(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "GetAutoRepeatDelay(): failure!\n");
		}
		#endif


		// 0-Threshold X, 1-Threshold Y and 2-Speed.
		long multiplier = hook_get_pointer_acceleration_multiplier();
		if (multiplier >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetPointerAccelerationMultiplier(): successful. (multiplier: %li)\n", multiplier);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", multiplier) >= 0) {
				(*env)->CallStaticObjectMethod(env, System_class, setProperty_id, (*env)->NewStringUTF(env, "jnativehook.pointerAccelerationMultiplier"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetPointerAccelerationMultiplier(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stdout, "GetPointerAccelerationMultiplier(): failure!\n");
		}
		#endif


		// 0-Threshold X, 1-Threshold Y and 2-Speed.
		long threshold = hook_get_pointer_acceleration_threshold();
		if (threshold >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetPointerAccelerationThreshold(): successful. (threshold: %li)\n", threshold);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", threshold) >= 0) {
				(*env)->CallStaticObjectMethod(env, System_class, setProperty_id, (*env)->NewStringUTF(env, "jnativehook.pointerAccelerationThreshold"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetPointerAccelerationThreshold(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stdout, "GetPointerAccelerationThreshold(): failure!\n");
		}
		#endif


		long sensitivity = hook_get_pointer_sensitivity();
		if (sensitivity >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetPointerSensitivity(): successful. (sensitivity: %li)\n", sensitivity);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", sensitivity) >= 0) {
				(*env)->CallStaticObjectMethod(env, System_class, setProperty_id, (*env)->NewStringUTF(env, "jnativehook.pointerSensitivity"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetPointerSensitivity(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stdout, "GetPointerSensitivity(): failure!\n");
		}
		#endif


		long clicktime = hook_get_multi_click_time();
		if (clicktime >= 0) {
			#ifdef DEBUG
			fprintf(stdout, "GetMultiClickTime(): successful. (time: %li)\n", clicktime);
			#endif

			if (snprintf(buffer, sizeof(buffer), "%li", clicktime) >= 0) {
				(*env)->CallStaticObjectMethod(env, System_class, setProperty_id, (*env)->NewStringUTF(env, "jnativehook.multiClickInterval"), (*env)->NewStringUTF(env, buffer));
			}
			#ifdef DEBUG
			else {
				fprintf(stderr, "GetMultiClickTime(): failure converting value to string!\n");
			}
			#endif
		}
		#ifdef DEBUG
		else {
			fprintf(stderr, "GetMultiClickTime(): failure!\n");
		}
		#endif
	}
}


void jni_ClearProperties(JNIEnv *env) {
	// FIXME Use globals
	jclass System_class = (*env)->FindClass(env, "java/lang/System");
	jmethodID clearProperty_id = (*env)->GetStaticMethodID(env, System_class, "clearProperty", "(Ljava/lang/String;)Ljava/lang/String;");

	if (System_class != NULL && clearProperty_id != NULL) {
		(*env)->CallStaticObjectMethod(env, System_class, clearProperty_id, (*env)->NewStringUTF(env, "jnativehook.autoRepeatRate"));
		(*env)->CallStaticObjectMethod(env, System_class, clearProperty_id, (*env)->NewStringUTF(env, "jnativehook.autoRepeatDelay"));
		(*env)->CallStaticObjectMethod(env, System_class, clearProperty_id, (*env)->NewStringUTF(env, "jnativehook.pointerAccelerationMultiplier"));
		(*env)->CallStaticObjectMethod(env, System_class, clearProperty_id, (*env)->NewStringUTF(env, "jnativehook.pointerAccelerationThreshold"));
		(*env)->CallStaticObjectMethod(env, System_class, clearProperty_id, (*env)->NewStringUTF(env, "jnativehook.pointerSensitivity"));
		(*env)->CallStaticObjectMethod(env, System_class, clearProperty_id, (*env)->NewStringUTF(env, "jnativehook.multiClickInterval"));
	}
}
