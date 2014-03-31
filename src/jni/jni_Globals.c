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
#include "jni_Logger.h"

GlobalScreen *org_jnativehook_GlobalScreen = NULL;
NativeInputEvent *org_jnativehook_NativeInputEvent = NULL;
NativeKeyEvent *org_jnativehook_keyboard_NativeKeyEvent = NULL;
NativeMouseEvent *org_jnativehook_mouse_NativeMouseEvent = NULL;
NativeMouseWheelEvent *org_jnativehook_mouse_NativeMouseWheelEvent = NULL;
System *java_lang_System = NULL;
Logger *java_util_logging_Logger = NULL;

int jni_CreateGlobals(JNIEnv *env) {
	int status = JNI_ERR;

	// Allocate memory for the Java object structure representation.
	org_jnativehook_GlobalScreen = malloc(sizeof(GlobalScreen));
	org_jnativehook_NativeInputEvent = malloc(sizeof(NativeInputEvent));
	org_jnativehook_keyboard_NativeKeyEvent = malloc(sizeof(NativeKeyEvent));
	org_jnativehook_mouse_NativeMouseEvent = malloc(sizeof(NativeMouseEvent));
	org_jnativehook_mouse_NativeMouseWheelEvent = malloc(sizeof(NativeMouseWheelEvent));
	java_lang_System = malloc(sizeof(System));
	java_util_logging_Logger = malloc(sizeof(Logger));

	// Check to make sure memory was allocated properly.
	if (org_jnativehook_GlobalScreen != NULL
			&& org_jnativehook_NativeInputEvent != NULL
			&& org_jnativehook_keyboard_NativeKeyEvent != NULL
			&& org_jnativehook_mouse_NativeMouseEvent != NULL
			&& org_jnativehook_mouse_NativeMouseWheelEvent != NULL
			&& java_lang_System != NULL
			&& java_util_logging_Logger != NULL) {

		// Lookup a local reference for the GlobalScreen class and create a global reference.
		jclass GlobalScreen_class = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
		if (GlobalScreen_class != NULL) {
			org_jnativehook_GlobalScreen->cls = (jclass) (*env)->NewGlobalRef(env, GlobalScreen_class);
			
			// Get the method ID for GlobalScreen.getInstance()
			org_jnativehook_GlobalScreen->getInstance = (*env)->GetStaticMethodID(
					env,
					org_jnativehook_GlobalScreen->cls,
					"getInstance",
					"()Lorg/jnativehook/GlobalScreen;");

			if (org_jnativehook_GlobalScreen->getInstance == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for GlobalScreen.getInstance()Lorg/jnativehook/GlobalScreen;!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for GlobalScreen.dispatchEvent().
			org_jnativehook_GlobalScreen->dispatchEvent = (*env)->GetMethodID(
					env,
					org_jnativehook_GlobalScreen->cls,
					"dispatchEvent",
					"(Lorg/jnativehook/NativeInputEvent;)V");

			if (org_jnativehook_GlobalScreen->dispatchEvent == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for GlobalScreen.dispatchEvent(Lorg/jnativehook/NativeInputEvent;)V!\n",
						__FUNCTION__, __LINE__);
			}
		}
		else {
			jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to locate the GlobalScreen class!\n",
						__FUNCTION__, __LINE__);
		}


		// Class and Constructor for the NativeInputEvent Object.
		jclass NativeInputEvent_class = (*env)->FindClass(env, "org/jnativehook/NativeInputEvent");
		if (NativeInputEvent_class != NULL) {
			org_jnativehook_NativeInputEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeInputEvent_class);
			
			// Get the field ID for NativeInputEvent.reserved.
			org_jnativehook_NativeInputEvent->reserved = (*env)->GetFieldID(
					env,
					org_jnativehook_NativeInputEvent->cls,
					"reserved",
					"S");

			if (org_jnativehook_NativeInputEvent->reserved == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the field ID for NativeInputEvent.reserved S!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeInputEvent constructor.
			org_jnativehook_NativeInputEvent->init = (*env)->GetMethodID(
					env,
					org_jnativehook_NativeInputEvent->cls,
					"<init>",
					"(Lorg/jnativehook/GlobalScreen;IJI)V");

			if (org_jnativehook_NativeInputEvent->init == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeInputEvent.<init>(Lorg.jnativehook.GlobalScreen;IJI)V!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeInputEvent.getID().
			org_jnativehook_NativeInputEvent->getID = (*env)->GetMethodID(
					env,
					org_jnativehook_NativeInputEvent->cls,
					"getID",
					"()I");

			if (org_jnativehook_NativeInputEvent->getID == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeInputEvent.getID()I!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeInputEvent.getModifiers().
			org_jnativehook_NativeInputEvent->getModifiers = (*env)->GetMethodID(
					env,
					org_jnativehook_NativeInputEvent->cls,
					"getModifiers",
					"()I");

			if (org_jnativehook_NativeInputEvent->getModifiers == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeInputEvent.getModifiers()I!\n",
						__FUNCTION__, __LINE__);
			}
		}
		else {
			jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to locate the NativeInputEvent class!\n",
					__FUNCTION__, __LINE__);
		}


		// Class and Constructor for the NativeKeyEvent Object.
		jclass NativeKeyEvent_class = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
		if (NativeKeyEvent_class != NULL) {
			org_jnativehook_keyboard_NativeKeyEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeKeyEvent_class);
			
			// Super class struct of the NativeKeyEvent.
			org_jnativehook_keyboard_NativeKeyEvent->parent = org_jnativehook_NativeInputEvent;
			
			// Get the method ID for NativeKeyEvent constructor.
			org_jnativehook_keyboard_NativeKeyEvent->init = (*env)->GetMethodID(
					env, 
					org_jnativehook_keyboard_NativeKeyEvent->cls, 
					"<init>", 
					"(IJIIICI)V");
			
			if (org_jnativehook_keyboard_NativeKeyEvent->init == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeKeyEvent.<init>(IJIIICI)V!\n",
						__FUNCTION__, __LINE__);
			}

			
			// Get the method ID for NativeKeyEvent.getKeyCode().
			org_jnativehook_keyboard_NativeKeyEvent->getKeyCode = (*env)->GetMethodID(
					env,
					org_jnativehook_keyboard_NativeKeyEvent->cls,
					"getKeyCode",
					"()I");

			if (org_jnativehook_keyboard_NativeKeyEvent->getKeyCode == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeKeyEvent.getKeyCode()!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeKeyEvent.getKeyLocation().
			org_jnativehook_keyboard_NativeKeyEvent->getKeyLocation = (*env)->GetMethodID(
					env,
					org_jnativehook_keyboard_NativeKeyEvent->cls,
					"getKeyLocation",
					"()I");

			if (org_jnativehook_keyboard_NativeKeyEvent->getKeyLocation == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeKeyEvent.getKeyLocation()!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeKeyEvent.getKeyChar().
			org_jnativehook_keyboard_NativeKeyEvent->getKeyChar = (*env)->GetMethodID(
					env,
					org_jnativehook_keyboard_NativeKeyEvent->cls,
					"getKeyChar",
					"()C");

			if (org_jnativehook_keyboard_NativeKeyEvent->getKeyChar == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeKeyEvent.getKeyChar()!\n",
						__FUNCTION__, __LINE__);
			}
		}
		else {
			jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to locate the NativeKeyEvent class!\n",
					__FUNCTION__, __LINE__);
		}


		// Class and Constructor for the NativeMouseEvent Object.
		jclass NativeMouseEvent_class = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
		if (NativeMouseEvent_class != NULL) {
			org_jnativehook_mouse_NativeMouseEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeMouseEvent_class);
			
			// Super class struct of the NativeKeyEvent.
			org_jnativehook_mouse_NativeMouseEvent->parent = org_jnativehook_NativeInputEvent;
			
			// Get the method ID for NativeMouseEvent constructor.
			org_jnativehook_mouse_NativeMouseEvent->init = (*env)->GetMethodID(
					env, 
					org_jnativehook_mouse_NativeMouseEvent->cls, 
					"<init>", 
					"(IJIIIII)V");
			
			if (org_jnativehook_mouse_NativeMouseEvent->init == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeMouseEvent.<init>(IJIIIII)V!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeMouseEvent.getButton().
			org_jnativehook_mouse_NativeMouseEvent->getButton = (*env)->GetMethodID(
					env,
					org_jnativehook_mouse_NativeMouseEvent->cls,
					"getButton",
					"()I");

			if (org_jnativehook_mouse_NativeMouseEvent->getButton == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeMouseEvent.getButton()!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeMouseEvent.getClickCount().
			org_jnativehook_mouse_NativeMouseEvent->getClickCount = (*env)->GetMethodID(
					env,
					org_jnativehook_mouse_NativeMouseEvent->cls,
					"getClickCount",
					"()I");

			if (org_jnativehook_mouse_NativeMouseEvent->getClickCount == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeMouseEvent.getClickCount()!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeMouseEvent.getX().
			org_jnativehook_mouse_NativeMouseEvent->getX = (*env)->GetMethodID(
					env,
					org_jnativehook_mouse_NativeMouseEvent->cls,
					"getX",
					"()I");

			if (org_jnativehook_mouse_NativeMouseEvent->getX == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeMouseEvent.getX()!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeMouseEvent.getY().
			org_jnativehook_mouse_NativeMouseEvent->getY = (*env)->GetMethodID(
					env,
					org_jnativehook_mouse_NativeMouseEvent->cls,
					"getY",
					"()I");

			if (org_jnativehook_mouse_NativeMouseEvent->getY == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeMouseEvent.getY()!\n",
						__FUNCTION__, __LINE__);
			}
		}
		else {
			jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to locate the NativeMouseEvent class!\n",
					__FUNCTION__, __LINE__);
		}


		// Class and Constructor for the NativeMouseWheelEvent Object.
		jclass NativeMouseWheelEvent_class = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseWheelEvent");
		if (NativeMouseWheelEvent_class != NULL) {
			org_jnativehook_mouse_NativeMouseWheelEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeMouseWheelEvent_class);
			
			// Super class struct of the NativeMouseEvent.
			org_jnativehook_mouse_NativeMouseWheelEvent->parent = org_jnativehook_mouse_NativeMouseEvent;
			
			// Get the method ID for NativeMouseWheelEvent constructor.
			org_jnativehook_mouse_NativeMouseWheelEvent->init = (*env)->GetMethodID(
					env, 
					org_jnativehook_mouse_NativeMouseWheelEvent->cls, 
					"<init>", 
					"(IJIIIIIII)V");
			
			if (org_jnativehook_mouse_NativeMouseWheelEvent->init == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeMouseEvent.<init>(IJIIIIIII)V!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeMouseEvent.getButton().
			org_jnativehook_mouse_NativeMouseWheelEvent->getScrollAmount = (*env)->GetMethodID(
					env,
					org_jnativehook_mouse_NativeMouseWheelEvent->cls,
					"getScrollAmount",
					"()I");

			if (org_jnativehook_mouse_NativeMouseWheelEvent->getScrollAmount == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeMouseWheelEvent.getScrollAmount()!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeMouseEvent.getScrollType().
			org_jnativehook_mouse_NativeMouseWheelEvent->getScrollType = (*env)->GetMethodID(
					env,
					org_jnativehook_mouse_NativeMouseWheelEvent->cls,
					"getScrollType",
					"()I");

			if (org_jnativehook_mouse_NativeMouseWheelEvent->getScrollType == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeMouseWheelEvent.getScrollType()!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for NativeMouseEvent.getWheelRotation().
			org_jnativehook_mouse_NativeMouseWheelEvent->getWheelRotation = (*env)->GetMethodID(
					env,
					org_jnativehook_mouse_NativeMouseWheelEvent->cls,
					"getWheelRotation",
					"()I");

			if (org_jnativehook_mouse_NativeMouseWheelEvent->getWheelRotation == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for NativeMouseWheelEvent.getWheelRotation()!\n",
						__FUNCTION__, __LINE__);
			}
		}
		else {
			jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to locate the NativeMouseWheelEvent class!\n",
					__FUNCTION__, __LINE__);
		}

		
		// Class and Constructor for the Logger Object.
		jclass System_class = (*env)->FindClass(env, "java/lang/System");
		if (System_class != NULL) {
			java_lang_System->cls = (jclass) (*env)->NewGlobalRef(env, System_class);
			
			// Get the method ID for System.setProperty().
			java_lang_System->setProperty = (*env)->GetStaticMethodID(
					env, 
					java_lang_System->cls, 
					"setProperty", 
					"(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

			if (java_lang_System->setProperty == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for System.setProperty(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;!\n",
						__FUNCTION__, __LINE__);
			}


			// Get the method ID for System.setProperty().
			java_lang_System->clearProperty = (*env)->GetStaticMethodID(
					env, 
					java_lang_System->cls, 
					"clearProperty", 
					"(Ljava/lang/String;)Ljava/lang/String;");

			if (java_lang_System->clearProperty == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for System.clearProperty(Ljava/lang/String;)Ljava/lang/String;!\n",
						__FUNCTION__, __LINE__);
			}
		}
		else {
			jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to locate the System class!\n",
					__FUNCTION__, __LINE__);
		}


		// Class and Constructor for the Logger Object.
		jclass Logger_class = (*env)->FindClass(env, "java/util/logging/Logger");
		if (Logger_class != NULL) {
			java_util_logging_Logger->cls = (jclass) (*env)->NewGlobalRef(env, Logger_class);
			
			// Get the method ID for Logger.getLogger constructor.
			java_util_logging_Logger->getLogger = (*env)->GetStaticMethodID(
					env, 
					java_util_logging_Logger->cls, 
					"getLogger", 
					"(Ljava/lang/String;)Ljava/util/logging/Logger;");

			if (java_util_logging_Logger->getLogger == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for Logger.getLogger(Ljava/lang/String;)Ljava/util/logging/Logger;!\n",
						__FUNCTION__, __LINE__);
			}

			
			java_util_logging_Logger->fine = (*env)->GetMethodID(
					env, 
					java_util_logging_Logger->cls, 
					"fine", 
					"(Ljava/lang/String;)V");

			if (java_util_logging_Logger->fine == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for Logger.fine(Ljava/lang/String;)V!\n",
						__FUNCTION__, __LINE__);
			}

			java_util_logging_Logger->info = (*env)->GetMethodID(
					env, 
					java_util_logging_Logger->cls, 
					"info", 
					"(Ljava/lang/String;)V");

			if (java_util_logging_Logger->info == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for Logger.info(Ljava/lang/String;)V!\n",
						__FUNCTION__, __LINE__);
			}

			java_util_logging_Logger->warning = (*env)->GetMethodID(
					env, 
					java_util_logging_Logger->cls, 
					"warning",
					"(Ljava/lang/String;)V");

			if (java_util_logging_Logger->warning == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for Logger.warning(Ljava/lang/String;)V!\n",
						__FUNCTION__, __LINE__);
			}

			java_util_logging_Logger->severe = (*env)->GetMethodID(
					env, 
					java_util_logging_Logger->cls, 
					"severe", 
					"(Ljava/lang/String;)V");

			if (java_util_logging_Logger->severe == NULL) {
				jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to acquire the method ID for Logger.severe(Ljava/lang/String;)V!\n",
						__FUNCTION__, __LINE__);
			}
		}
		else {
			jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to locate the Logger class!\n",
					__FUNCTION__, __LINE__);
		}
	}
	else {
		status = JNI_ENOMEM;

		jni_Logger(LOG_LEVEL_ERROR, "%s [%u]: Failed to allocate memory for JNI structures!\n",
				__FUNCTION__, __LINE__);

		// FIXME Throw java.lang.OutOfMemoryError
	}

	// Check and make sure everything is correct.
	if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
		status = JNI_OK;
	}

	return status;
}

int jni_DestroyGlobals(JNIEnv *env) {
	// Free any memory being used for Java object structures.
	if (org_jnativehook_GlobalScreen != NULL) {
		(*env)->DeleteGlobalRef(env, org_jnativehook_GlobalScreen->cls);
		free(org_jnativehook_GlobalScreen);
		org_jnativehook_GlobalScreen = NULL;
	}

	if (org_jnativehook_NativeInputEvent != NULL) {
		(*env)->DeleteGlobalRef(env, org_jnativehook_NativeInputEvent->cls);
		free(org_jnativehook_NativeInputEvent);
		org_jnativehook_NativeInputEvent = NULL;
	}

	if (org_jnativehook_keyboard_NativeKeyEvent != NULL) {
		(*env)->DeleteGlobalRef(env, org_jnativehook_keyboard_NativeKeyEvent->cls);
		free(org_jnativehook_keyboard_NativeKeyEvent);
		org_jnativehook_keyboard_NativeKeyEvent = NULL;
	}

	if (org_jnativehook_mouse_NativeMouseEvent != NULL) {
		(*env)->DeleteGlobalRef(env, org_jnativehook_mouse_NativeMouseEvent->cls);
		free(org_jnativehook_mouse_NativeMouseEvent);
		org_jnativehook_mouse_NativeMouseEvent = NULL;
	}

	if (org_jnativehook_mouse_NativeMouseWheelEvent != NULL) {
		(*env)->DeleteGlobalRef(env, org_jnativehook_mouse_NativeMouseWheelEvent->cls);
		free(org_jnativehook_mouse_NativeMouseWheelEvent);
		org_jnativehook_mouse_NativeMouseWheelEvent = NULL;
	}

	if (java_lang_System != NULL) {
		(*env)->DeleteGlobalRef(env, java_lang_System->cls);
		free(java_lang_System);
		java_lang_System = NULL;
	}

	if (java_util_logging_Logger != NULL) {
		(*env)->DeleteGlobalRef(env, java_util_logging_Logger->cls);
		free(java_util_logging_Logger);
		java_util_logging_Logger = NULL;
	}

	return JNI_OK;
}
