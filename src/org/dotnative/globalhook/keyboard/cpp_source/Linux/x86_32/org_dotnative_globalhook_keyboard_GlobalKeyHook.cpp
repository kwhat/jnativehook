/*
 *	SysHook - 07/17/05
 *	Jacob Gohlke
 *	
 *  Linux Port - 09/08/06
 *  Alex Barker
 *  
 *	JNI Interface for setting a Keyboard Hook and monitoring
 *	it Java-side
 *
 *	(c) Copyright 2005 Jacob Gohlke
 *	
 *	Feel free to use and learn from this code, royalty-free!
 *	I only ask you acknkowlege what library you are using
 *	and who made it. Thanks, and happy hooking!
 */

// Use XEvie
/*
emerge -av libXevie

Section "Extensions"
    ...
    Option         "XEVIE" "Enable"
    ...
EndSection

*/
//g++ -m32 -march=i586 -shared -lX11 -lXevie -I/opt/sun-jdk-1.5.0.12/include -I/opt/sun-jdk-1.5.0.12/include/linux ./org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp -o libGlobalKeyListener.so
//g++ -m64 -fPIC -o libGlobalKeyListener.so -march=i586 -shared -lX11 -lXevie -I/opt/sun-jdk-1.5.0.08/include -I/opt/sun-jdk-1.5.0.08/include/linux ./jni_keyboard_PollThread.cpp

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/X.h>
#include <X11/extensions/Xevie.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/Intrinsic.h>
#include "org_dotnative_globalkeylistener_PollThread.h"

//Instance Variables
Display  *disp;
XEvent xev;
JavaVM * jvm = NULL;
jobject hookObj = NULL;
jmethodID fireKeyPressed_ID = NULL;
jmethodID fireKeyReleased_ID = NULL;
jmethodID fireKeyTyped_ID = NULL;
DWORD hookThreadId = 0;

//Dll Main Constructor and Deconstructor
void __attribute__ ((constructor)) Init(void);
void __attribute__ ((destructor)) Cleanup(void);

void MsgLoop() {
	while (TRUE) {
		XNextEvent(disp, &xev);
		
		iKeyCode = xev.xkey.keycode;
		//iKeyCode = XLookupKeysym(&xev.xkey, xev.xkey.state);
		iKeyState = xev.xkey.state;
		iKeyTime = xev.xkey.time;
		int iKeyType = xev.type;
		
		switch (iKeyType) {
			case KeyPress:
				//bKeyDown = TRUE;
				fprintf(stderr, "Press");
			break;
			
			case KeyRelease:
				//bKeyDown = FALSE;
				fprintf(stderr, "Release");
			break;
		}
		
		fprintf(stderr, "Key: %i %i %i\n", iKeyCode, xev.type, iKeyTime);

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
		printf("XevieStart(disp) finished \n");
	}
	else {
		printf("XevieStart(dpy) failed, only one client is allowed to do event interception\n");
		exit(1);
	}
	
	//Setup all the jni hook call back pointers.
	hookObj = env->NewGlobalRef(obj);
	jclass cls = env->GetObjectClass(hookObj);
	fireKeyPressed_ID = env->GetMethodID(cls, "fireKeyPressed", "(JIIC)V");
	fireKeyReleased_ID = env->GetMethodID(cls, "fireKeyReleased", "(JIIC)V");
	fireKeyTyped_ID = env->GetMethodID(cls, "fireKeyTyped", "(JIIC)V");
	env->GetJavaVM(&jvm);
	hookThreadId = GetCurrentThreadId();
	
	//Set the mask for what we want to listen to with Xevie
	XevieSelectInput(disp, KeyPressMask | KeyReleaseMask);
	
	//Make sure we can detect when the button is being held down.
	XkbSetDetectableAutoRepeat(disp, TRUE, NULL);
}

JNIEXPORT void JNICALL Java_org_dotnative_globalhook_keyboard_GlobalKeyHook_unregisterHook(JNIEnv *env, jobject object) {
	XevieEnd(disp);
}

void Init() {
	//Do Notihing
	//fprintf(stderr, "Jni Keyboard Driver Loaded\n");
}
 
void Cleanup() {
	//Do Notihing
	//fprintf(stderr, "Jni Keyboard Driver Unloaded\n");
}
