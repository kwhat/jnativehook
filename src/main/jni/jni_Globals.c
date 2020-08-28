/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2020 Alexander Barker.  All Rights Received.
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

GlobalScreen *com_github_kwhat_jnativehook_GlobalScreen = NULL;
NativeHookThread *com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread = NULL;
NativeHookException *com_github_kwhat_jnativehook_NativeHookException = NULL;
NativeMonitorInfo *com_github_kwhat_jnativehook_NativeMonitorInfo = NULL;
NativeInputEvent *com_github_kwhat_jnativehook_NativeInputEvent = NULL;
NativeKeyEvent *com_github_kwhat_jnativehook_keyboard_NativeKeyEvent = NULL;
NativeMouseEvent *com_github_kwhat_jnativehook_mouse_NativeMouseEvent = NULL;
NativeMouseWheelEvent *com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent = NULL;
Object *java_lang_Object = NULL;
Integer *java_lang_Integer = NULL;
System *java_lang_System = NULL;
Logger *java_util_logging_Logger = NULL;

static int create_GlobalScreen(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the GlobalScreen Object.
    jclass GlobalScreen_class = (*env)->FindClass(env, "com/github/kwhat/jnativehook/GlobalScreen");
    if (GlobalScreen_class != NULL) {
        // Get the field ID for hookThread.
        jfieldID hookThread = (*env)->GetStaticFieldID(env, GlobalScreen_class, "hookThread", "Lcom/github/kwhat/jnativehook/GlobalScreen$NativeHookThread;");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            com_github_kwhat_jnativehook_GlobalScreen = malloc(sizeof(GlobalScreen));
            if (com_github_kwhat_jnativehook_GlobalScreen != NULL) {
                // Populate our structure for later use.
                com_github_kwhat_jnativehook_GlobalScreen->cls = (jclass) (*env)->NewGlobalRef(env, GlobalScreen_class);
                com_github_kwhat_jnativehook_GlobalScreen->hookThread = hookThread;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static void destroy_GlobalScreen(JNIEnv *env) {
    if (com_github_kwhat_jnativehook_GlobalScreen != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (com_github_kwhat_jnativehook_GlobalScreen->cls != NULL) {
            (*env)->DeleteGlobalRef(env, com_github_kwhat_jnativehook_GlobalScreen->cls);
        }

        // Free struct memory.
        free(com_github_kwhat_jnativehook_GlobalScreen);
        com_github_kwhat_jnativehook_GlobalScreen = NULL;
    }
}


static int create_NativeHookThread(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the GlobalScreen Object.
    jclass NativeHookThread_class = (*env)->FindClass(env, "com/github/kwhat/jnativehook/GlobalScreen$NativeHookThread");
    if (NativeHookThread_class != NULL) {
        // Get the method ID for GlobalScreen.dispatchEvent().
        jmethodID dispatchEvent = (*env)->GetStaticMethodID(env, NativeHookThread_class, "dispatchEvent", "(Lcom/github/kwhat/jnativehook/NativeInputEvent;)V");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread = malloc(sizeof(NativeHookThread));
            if (com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread != NULL) {
                // Populate our structure for later use.
                com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread->cls = (jclass) (*env)->NewGlobalRef(env, NativeHookThread_class);
                com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread->dispatchEvent = dispatchEvent;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static void destroy_NativeHookThread(JNIEnv *env) {
    if (com_github_kwhat_jnativehook_GlobalScreen != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread->cls != NULL) {
            (*env)->DeleteGlobalRef(env, com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread->cls);
        }

        // Free struct memory.
        free(com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread);
        com_github_kwhat_jnativehook_GlobalScreen$NativeHookThread = NULL;
    }
}


static int create_NativeHookException(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the NativeHookException Object.
    jclass NativeHookException_class = (*env)->FindClass(env, "com/github/kwhat/jnativehook/NativeHookException");
    if (NativeHookException_class != NULL) {
        // Get the method ID for NativeInputEvent constructor.
        jmethodID init = (*env)->GetMethodID(env, NativeHookException_class, "<init>", "(ILjava/lang/String;)V");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            com_github_kwhat_jnativehook_NativeHookException = malloc(sizeof(NativeInputEvent));
            if (com_github_kwhat_jnativehook_NativeHookException != NULL) {
                // Populate our structure for later use.
                com_github_kwhat_jnativehook_NativeHookException->cls = (jclass) (*env)->NewGlobalRef(env, NativeHookException_class);
                com_github_kwhat_jnativehook_NativeHookException->init = init;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static void destroy_NativeHookException(JNIEnv *env) {
    if (com_github_kwhat_jnativehook_NativeHookException != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (com_github_kwhat_jnativehook_NativeHookException->cls != NULL) {
            (*env)->DeleteGlobalRef(env, com_github_kwhat_jnativehook_NativeHookException->cls);
        }

        // Free struct memory.
        free(com_github_kwhat_jnativehook_NativeHookException);
        com_github_kwhat_jnativehook_NativeHookException = NULL;
    }
}


static int create_NativeMonitorInfo(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the NativeMonitorInfo Object.
    jclass NativeMonitorInfo_class = (*env)->FindClass(env, "com/github/kwhat/jnativehook/NativeMonitorInfo");
    if (NativeMonitorInfo_class != NULL) {
        // Get the method ID for NativeInputEvent constructor.
        jmethodID init = (*env)->GetMethodID(env, NativeMonitorInfo_class, "<init>", "(SIISS)V");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            com_github_kwhat_jnativehook_NativeMonitorInfo = malloc(sizeof(NativeMonitorInfo));
            if (com_github_kwhat_jnativehook_NativeMonitorInfo != NULL) {
                // Populate our structure for later use.
                com_github_kwhat_jnativehook_NativeMonitorInfo->cls = (jclass) (*env)->NewGlobalRef(env, NativeMonitorInfo_class);
                com_github_kwhat_jnativehook_NativeMonitorInfo->init = init;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static void destroy_NativeMonitorInfo(JNIEnv *env) {
    if (com_github_kwhat_jnativehook_NativeMonitorInfo != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (com_github_kwhat_jnativehook_NativeMonitorInfo->cls != NULL) {
            (*env)->DeleteGlobalRef(env, com_github_kwhat_jnativehook_NativeMonitorInfo->cls);
        }

        // Free struct memory.
        free(com_github_kwhat_jnativehook_NativeMonitorInfo);
        com_github_kwhat_jnativehook_NativeMonitorInfo = NULL;
    }
}

static int create_NativeInputEvent(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the NativeInputEvent Object.
    jclass NativeInputEvent_class = (*env)->FindClass(env, "com/github/kwhat/jnativehook/NativeInputEvent");
    if (NativeInputEvent_class != NULL) {
        // Get the field ID for NativeInputEvent.when.
        jfieldID when = (*env)->GetFieldID(env, NativeInputEvent_class, "when", "J");

        // Get the field ID for NativeInputEvent.reserved.
        jfieldID reserved = (*env)->GetFieldID(env, NativeInputEvent_class, "reserved", "S");

        // Get the method ID for NativeInputEvent constructor.
        jmethodID init = (*env)->GetMethodID(env, NativeInputEvent_class, "<init>", "(Ljava/lang/Class;II)V");

        // Get the method ID for NativeInputEvent.getID().
        jmethodID getID = (*env)->GetMethodID(env, NativeInputEvent_class, "getID", "()I");

        // Get the method ID for NativeInputEvent.getModifiers().
        jmethodID getModifiers = (*env)->GetMethodID(env, NativeInputEvent_class, "getModifiers", "()I");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            com_github_kwhat_jnativehook_NativeInputEvent = malloc(sizeof(NativeInputEvent));
            if (com_github_kwhat_jnativehook_NativeInputEvent != NULL) {
                // Populate our structure for later use.
                com_github_kwhat_jnativehook_NativeInputEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeInputEvent_class);
                com_github_kwhat_jnativehook_NativeInputEvent->when = when;
                com_github_kwhat_jnativehook_NativeInputEvent->reserved = reserved;
                com_github_kwhat_jnativehook_NativeInputEvent->init = init;
                com_github_kwhat_jnativehook_NativeInputEvent->getID = getID;
                com_github_kwhat_jnativehook_NativeInputEvent->getModifiers = getModifiers;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static void destroy_NativeInputEvent(JNIEnv *env) {
    if (com_github_kwhat_jnativehook_NativeInputEvent != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (com_github_kwhat_jnativehook_NativeInputEvent->cls != NULL) {
            (*env)->DeleteGlobalRef(env, com_github_kwhat_jnativehook_NativeInputEvent->cls);
        }

        // Free struct memory.
        free(com_github_kwhat_jnativehook_NativeInputEvent);
        com_github_kwhat_jnativehook_NativeInputEvent = NULL;
    }
}


static int create_NativeKeyEvent(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the NativeKeyEvent Object.
    jclass NativeKeyEvent_class = (*env)->FindClass(env, "com/github/kwhat/jnativehook/keyboard/NativeKeyEvent");
    if (NativeKeyEvent_class != NULL) {
        // Get the method ID for NativeKeyEvent constructor.
        jmethodID init = (*env)->GetMethodID(env, NativeKeyEvent_class, "<init>", "(IIIICI)V");

        // Get the method ID for NativeKeyEvent.getKeyCode().
        jmethodID getKeyCode = (*env)->GetMethodID(env, NativeKeyEvent_class, "getKeyCode", "()I");

        // Get the method ID for NativeKeyEvent.getKeyLocation().
        jmethodID getKeyLocation = (*env)->GetMethodID(env, NativeKeyEvent_class, "getKeyLocation", "()I");

        // Get the method ID for NativeKeyEvent.getKeyChar().
        jmethodID getKeyChar = (*env)->GetMethodID(env, NativeKeyEvent_class, "getKeyChar", "()C");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent = malloc(sizeof(NativeKeyEvent));
            if (com_github_kwhat_jnativehook_keyboard_NativeKeyEvent != NULL) {
                // Populate our structure for later use.
                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeKeyEvent_class);
                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->parent = com_github_kwhat_jnativehook_NativeInputEvent;
                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->init = init;
                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->getKeyCode = getKeyCode;
                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->getKeyLocation = getKeyLocation;
                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->getKeyChar = getKeyChar;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static void destroy_NativeKeyEvent(JNIEnv *env) {
    if (com_github_kwhat_jnativehook_keyboard_NativeKeyEvent != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->cls != NULL) {
            (*env)->DeleteGlobalRef(env, com_github_kwhat_jnativehook_keyboard_NativeKeyEvent->cls);
        }

        // Free struct memory.
        free(com_github_kwhat_jnativehook_keyboard_NativeKeyEvent);
        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent = NULL;
    }
}


static int create_NativeMouseEvent(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the NativeMouseEvent Object.
    jclass NativeMouseEvent_class = (*env)->FindClass(env, "com/github/kwhat/jnativehook/mouse/NativeMouseEvent");
    if (NativeMouseEvent_class != NULL) {
        // Get the method ID for NativeMouseEvent constructor.
        jmethodID init = (*env)->GetMethodID(env, NativeMouseEvent_class, "<init>", "(IIIIII)V");

        // Get the method ID for NativeMouseEvent.getButton().
        jmethodID getButton = (*env)->GetMethodID(env, NativeMouseEvent_class, "getButton", "()I");

        // Get the method ID for NativeMouseEvent.getClickCount().
        jmethodID getClickCount = (*env)->GetMethodID(env, NativeMouseEvent_class, "getClickCount", "()I");

        // Get the method ID for NativeMouseEvent.getX().
        jmethodID getX = (*env)->GetMethodID(env, NativeMouseEvent_class, "getX", "()I");

        // Get the method ID for NativeMouseEvent.getY().
        jmethodID getY = (*env)->GetMethodID(env, NativeMouseEvent_class, "getY", "()I");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            com_github_kwhat_jnativehook_mouse_NativeMouseEvent = malloc(sizeof(NativeMouseEvent));
            if (com_github_kwhat_jnativehook_mouse_NativeMouseEvent != NULL) {
                // Populate our structure for later use.
                com_github_kwhat_jnativehook_mouse_NativeMouseEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeMouseEvent_class);
                com_github_kwhat_jnativehook_mouse_NativeMouseEvent->parent = com_github_kwhat_jnativehook_NativeInputEvent;
                com_github_kwhat_jnativehook_mouse_NativeMouseEvent->init = init;
                com_github_kwhat_jnativehook_mouse_NativeMouseEvent->getButton = getButton;
                com_github_kwhat_jnativehook_mouse_NativeMouseEvent->getClickCount = getClickCount;
                com_github_kwhat_jnativehook_mouse_NativeMouseEvent->getX = getX;
                com_github_kwhat_jnativehook_mouse_NativeMouseEvent->getY = getY;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static void destroy_NativeMouseEvent(JNIEnv *env) {
    if (com_github_kwhat_jnativehook_mouse_NativeMouseEvent != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (com_github_kwhat_jnativehook_mouse_NativeMouseEvent->cls != NULL) {
            (*env)->DeleteGlobalRef(env, com_github_kwhat_jnativehook_mouse_NativeMouseEvent->cls);
        }

        // Free struct memory.
        free(com_github_kwhat_jnativehook_mouse_NativeMouseEvent);
        com_github_kwhat_jnativehook_mouse_NativeMouseEvent = NULL;
    }
}


static int create_NativeMouseWheelEvent(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the NativeMouseWheelEvent Object.
    jclass NativeMouseWheelEvent_class = (*env)->FindClass(env, "com/github/kwhat/jnativehook/mouse/NativeMouseWheelEvent");
    if (NativeMouseWheelEvent_class != NULL) {
        // Get the method ID for NativeMouseWheelEvent constructor.
        jmethodID init = (*env)->GetMethodID(env, NativeMouseWheelEvent_class, "<init>", "(IIIIIIIII)V");

        // Get the method ID for NativeMouseWheelEvent.getScrollAmount().
        jmethodID getScrollAmount = (*env)->GetMethodID(env, NativeMouseWheelEvent_class, "getScrollAmount", "()I");

        // Get the method ID for NativeMouseWheelEvent.getScrollType().
        jmethodID getScrollType = (*env)->GetMethodID(env, NativeMouseWheelEvent_class, "getScrollType", "()I");

        // Get the method ID for NativeMouseWheelEvent.getWheelRotation().
        jmethodID getWheelRotation = (*env)->GetMethodID(env, NativeMouseWheelEvent_class, "getWheelRotation", "()I");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent = malloc(sizeof(NativeMouseWheelEvent));
            if (com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent != NULL) {
                // Populate our structure for later use.
                com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->cls = (jclass) (*env)->NewGlobalRef(env, NativeMouseWheelEvent_class);
                com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->parent = com_github_kwhat_jnativehook_mouse_NativeMouseEvent;
                com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->init = init;
                com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->getScrollAmount = getScrollAmount;
                com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->getScrollType = getScrollType;
                com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->getWheelRotation = getWheelRotation;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static void destroy_NativeMouseWheelEvent(JNIEnv *env) {
    if (com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->cls != NULL) {
            (*env)->DeleteGlobalRef(env, com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent->cls);
        }

        // Free struct memory.
        free(com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent);
        com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent = NULL;
    }
}


static int create_Object(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the Object object.
    jclass Object_class = (*env)->FindClass(env, "java/lang/Object");
    if (Object_class != NULL) {
        // Get the method ID for Object.notify().
        jmethodID notify = (*env)->GetMethodID(env, Object_class, "notify", "()V");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            java_lang_Object = malloc(sizeof(Object));
            if (java_lang_Object != NULL) {
                // Populate our structure for later use.
                java_lang_Object->cls = (jclass) (*env)->NewGlobalRef(env, Object_class);
                java_lang_Object->notify = notify;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static void destroy_Object(JNIEnv *env) {
    if (java_lang_Object != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (java_lang_Object->cls != NULL) {
            (*env)->DeleteGlobalRef(env, java_lang_Object->cls);
        }

        // Free struct memory.
        free(java_lang_Object);
        java_lang_Object = NULL;
    }
}

static int create_Integer(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the Object object.
    jclass Integer_class = (*env)->FindClass(env, "java/lang/Integer");
    if (Integer_class != NULL) {
        // Get the method ID for NativeMouseWheelEvent constructor.
        jmethodID init = (*env)->GetMethodID(env, Integer_class, "<init>", "(I)V");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            java_lang_Integer = malloc(sizeof(Integer));
            if (java_lang_Integer != NULL) {
                // Populate our structure for later use.
                java_lang_Integer->cls = (jclass) (*env)->NewGlobalRef(env, Integer_class);
                java_lang_Integer->init = init;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static void destroy_Integer(JNIEnv *env) {
    if (java_lang_Integer != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (java_lang_Integer->cls != NULL) {
            (*env)->DeleteGlobalRef(env, java_lang_Integer->cls);
        }

        // Free struct memory.
        free(java_lang_Integer);
        java_lang_Integer = NULL;
    }
}

static inline int create_System(JNIEnv *env) {
    int status = JNI_ERR;

    // Class and Constructor for the System Object.
    jclass System_class = (*env)->FindClass(env, "java/lang/System");
    if (System_class != NULL) {
        // Get the method ID for System.setProperty().
        jmethodID setProperty = (*env)->GetStaticMethodID(env, System_class, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

        // Get the method ID for System.clearProperty().
        jmethodID clearProperty = (*env)->GetStaticMethodID(env, System_class, "clearProperty", "(Ljava/lang/String;)Ljava/lang/String;");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            java_lang_System = malloc(sizeof(System));
            if (java_lang_System != NULL) {
                // Populate our structure for later use.
                java_lang_System->cls = (jclass) (*env)->NewGlobalRef(env, System_class);
                java_lang_System->setProperty = setProperty;
                java_lang_System->clearProperty = clearProperty;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static inline void destroy_System(JNIEnv *env) {
    if (java_lang_System != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (java_lang_System->cls != NULL) {
            (*env)->DeleteGlobalRef(env, java_lang_System->cls);
        }

        // Free struct memory.
        free(java_lang_System);
        java_lang_System = NULL;
    }
}



static inline int create_Logger(JNIEnv *env) {
    int status = JNI_ERR;

    // Class for the Logger object.
    jclass Logger_class = (*env)->FindClass(env, "java/util/logging/Logger");
    if (Logger_class != NULL) {
        // Get the method ID for Logger.getLogger().
        jmethodID getLogger = (*env)->GetStaticMethodID(env, Logger_class, "getLogger", "(Ljava/lang/String;)Ljava/util/logging/Logger;");

        // Get the method ID for Logger.fine().
        jmethodID fine = (*env)->GetMethodID(env, Logger_class, "fine", "(Ljava/lang/String;)V");

        // Get the method ID for Logger.info().
        jmethodID info = (*env)->GetMethodID(env, Logger_class, "info", "(Ljava/lang/String;)V");

        // Get the method ID for Logger.warning().
        jmethodID warning = (*env)->GetMethodID(env, Logger_class, "warning", "(Ljava/lang/String;)V");

        // Get the method ID for Logger.severe().
        jmethodID severe = (*env)->GetMethodID(env, Logger_class, "severe", "(Ljava/lang/String;)V");

        if ((*env)->ExceptionCheck(env) == JNI_FALSE) {
            java_util_logging_Logger = malloc(sizeof(Logger));
            if (java_util_logging_Logger != NULL) {
                // Populate our structure for later use.
                java_util_logging_Logger->cls = (jclass) (*env)->NewGlobalRef(env, Logger_class);
                java_util_logging_Logger->getLogger = getLogger;
                java_util_logging_Logger->fine = fine;
                java_util_logging_Logger->info = info;
                java_util_logging_Logger->warning = warning;
                java_util_logging_Logger->severe = severe;

                status = JNI_OK;
            } else {
                jni_ThrowException(env, "java/lang/OutOfMemoryError", "Failed to allocate native memory.");
                status = JNI_ENOMEM;
            }
        }
    }

    return status;
}

static inline void destroy_Logger(JNIEnv *env) {
    if (java_util_logging_Logger != NULL) {
        // The class *should* never be null if the struct was allocated, but we will check anyway.
        if (java_util_logging_Logger->cls != NULL) {
            (*env)->DeleteGlobalRef(env, java_util_logging_Logger->cls);
        }

        // Free struct memory.
        free(java_util_logging_Logger);
        java_util_logging_Logger = NULL;
    }
}


int jni_CreateGlobals(JNIEnv *env) {
    int status = create_GlobalScreen(env);

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_NativeHookThread(env);
    }

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_NativeHookException(env);
    }

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_NativeMonitorInfo(env);
    }

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_NativeInputEvent(env);
    }

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_NativeKeyEvent(env);
    }

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_NativeMouseEvent(env);
    }

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_NativeMouseWheelEvent(env);
    }

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_Object(env);
    }

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_Integer(env);
    }

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_System(env);
    }

    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_FALSE) {
        status = create_Logger(env);
    }

    // Check and make sure we don't have a pending exception and a JNI_OK status.
    if (status == JNI_OK && (*env)->ExceptionCheck(env) == JNI_TRUE) {
        status = JNI_ERR;
    }

    return status;
}

int jni_DestroyGlobals(JNIEnv *env) {
    destroy_GlobalScreen(env);
    destroy_NativeHookThread(env);
    destroy_NativeHookException(env);
    destroy_NativeMonitorInfo(env);
    destroy_NativeInputEvent(env);
    destroy_NativeKeyEvent(env);
    destroy_NativeMouseEvent(env);
    destroy_NativeMouseWheelEvent(env);
    destroy_Object(env);
    destroy_Integer(env);
    destroy_System(env);
    destroy_Logger(env);

    return JNI_OK;
}
