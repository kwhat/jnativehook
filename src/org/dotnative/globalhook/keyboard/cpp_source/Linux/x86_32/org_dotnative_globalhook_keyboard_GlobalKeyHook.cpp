/*
 *	dotNative - GlobalKeyHook - 09/08/06
 *  Alex Barker
 *  
 *	JNI Interface for setting a Keyboard Hook and monitoring
 *	it with java.
 *
 *	Original windows code created by Jean-Francois Briere.  No license or 
 *  contact information was provided.
 *  http://www.velocityreviews.com/forums/showpost.php?p=1899928&postcount=5
 */

/*
This lib requires XEvie as XKeyGrap XKeyboard will not work.

emerge -av libXevie

Make sure to update the xorg.conf file extention section.
Section "Extensions"
    ...
    Option         "XEVIE" "Enable"
    ...
EndSection
*/

/*
Compiling options
g++ -m32 -march=i586 -shared -fPIC -lX11 -lXevie -I/opt/sun-jdk-1.5.0.12/include -I/opt/sun-jdk-1.5.0.12/include/linux ./org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp -o libGlobalKeyListener.so
g++ -m64 -fPIC -o libGlobalKeyListener.so -march=i586 -shared -lX11 -lXevie -I/opt/sun-jdk-1.5.0.08/include -I/opt/sun-jdk-1.5.0.08/include/linux ./jni_keyboard_PollThread.cpp
*/

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/X.h>
#include <X11/extensions/Xevie.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/Intrinsic.h>
#include "org_dotnative_globalhook_keyboard_GlobalKeyHook.h"

//Instance Variables
Display  *disp;
XEvent xev;
JavaVM * jvm = NULL;
jobject hookObj = NULL;
jmethodID fireKeyPressed_ID = NULL;
jmethodID fireKeyReleased_ID = NULL;
jmethodID fireKeyTyped_ID = NULL;
pthread_t hookThreadId = 0;

//Dll Main Constructor and Deconstructor
void __attribute__ ((constructor)) Init(void);
void __attribute__ ((destructor)) Cleanup(void);

void MsgLoop(JNIEnv * env) {
	while (TRUE) {
		XNextEvent(disp, &xev);
		
		//unsigned int iKeyCode = xev.xkey.keycode;
		unsigned int iKeyCode = XLookupKeysym(&xev.xkey, xev.xkey.state);
		unsigned int iKeyState = xev.xkey.state;
		unsigned long iKeyTime = xev.xkey.time;
		int iKeyType = xev.type;
		
		switch (iKeyType) {
			case KeyPress:
				printf("C++: MsgLoop - Key pressed\n");
				env->CallVoidMethod(hookObj, fireKeyPressed_ID, (jlong) iKeyTime, (jint) iKeyType, (jint) iKeyCode, (jchar) char(iKeyCode));
			break;
			
			case KeyRelease:
				printf("C++: MsgLoop - Key released\n");
				env->CallVoidMethod(hookObj, fireKeyReleased_ID, (jlong) iKeyTime, (jint) iKeyType, (jint) iKeyCode, (jchar) char(iKeyCode));
			break;
		}
		
		XevieSendEvent(disp, &xev, XEVIE_UNMODIFIED);
	}
}

JNIEXPORT void JNICALL Java_org_dotnative_globalhook_keyboard_GlobalKeyHook_registerHook(JNIEnv * env, jobject obj) {
	disp = XOpenDisplay(NULL);
	if (disp == NULL) {
		//We couldnt hook a display so we need to die.
		fprintf(stderr, "Can't open display: %s\n", XDisplayName(NULL));
		exit(1);
	}
	
	if(XevieStart(disp)) {
		printf("C++: XevieStart(disp) successful\n");
	}
	else {
		printf("C++: XevieStart(disp) failed, only one client is allowed to do event interception\n");
		exit(1);
	}
	
	//Setup all the jni hook call back pointers.
	hookObj = env->NewGlobalRef(obj);
	jclass cls = env->GetObjectClass(hookObj);
	fireKeyPressed_ID = env->GetMethodID(cls, "fireKeyPressed", "(JIIC)V");
	fireKeyReleased_ID = env->GetMethodID(cls, "fireKeyReleased", "(JIIC)V");
	fireKeyTyped_ID = env->GetMethodID(cls, "fireKeyTyped", "(JIIC)V");
	env->GetJavaVM(&jvm);
	hookThreadId = pthread_self();
	
	//Set the mask for what we want to listen to with Xevie
	XevieSelectInput(disp, KeyPressMask | KeyReleaseMask);
	
	//Make sure we can detect when the button is being held down.
	XkbSetDetectableAutoRepeat(disp, TRUE, NULL);
	
	//Call listener
	MsgLoop(env);
}

JNIEXPORT void JNICALL Java_org_dotnative_globalhook_keyboard_GlobalKeyHook_unregisterHook(JNIEnv *env, jobject object) {
	XevieEnd(disp);
	printf("C++: XevieEnd(disp) successful\n");
	
	pthread_cancel(hookThreadId);
	printf("C++: pthread_cancel(hookThreadId) successful\n");
}

void Init() {
	//Do Notihing
	printf("C++: Init - Shared Object Process Attach.\n");
}
 
void Cleanup() {
	//Do Notihing
	printf("C++: Init - Shared Object Process Detach.\n");
}
