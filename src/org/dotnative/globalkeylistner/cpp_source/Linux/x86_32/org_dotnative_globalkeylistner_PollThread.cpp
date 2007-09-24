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

//g++ -o libGlobalKeyListener.so -shared -lX11 -I/opt/sun-jdk-1.5.0.08/include -I/opt/sun-jdk-1.5.0.08/include/linux ./jni_keyboard_PollThread.cpp
//g++ -m32 -o libGlobalKeyListener.so -march=i586 -shared -lX11 -I/opt/sun-jdk-1.5.0.08/include -I/opt/sun-jdk-1.5.0.08/include/linux ./jni_keyboard_PollThread.cpp
//g++ -m64 -fPIC -o libGlobalKeyListener.so -march=i586 -shared -lX11 -I/opt/sun-jdk-1.5.0.08/include -I/opt/sun-jdk-1.5.0.08/include/linux ./jni_keyboard_PollThread.cpp


#include <stdio.h>
#include <jni.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xutil.h>
#include <X11/Shell.h>
#include "org_dotnative_globalkeylistner_PollThread.h"

char *TranslateKeyCode(XEvent *ev);
Display  *d;
XEvent xev;
bool bRunning;

//Last Key Statem
bool bKeyDown;
int iKeyCode;
int iKeyState;
int iKeyTime;
unsigned int iXNumbWindows;

//Dll Main
void __attribute__ ((constructor)) Init(void);
void __attribute__ ((destructor)) Cleanup(void);

int XErrIgnore(Display *d, XErrorEvent *e) {
	return 0;
}

void snoop_all_windows(Window root_window, unsigned long type, bool bRootWindow) {
	Window parent, root_return, *children;
	unsigned int nchildren, i;
	int stat;
	
	stat = XQueryTree(d, root_window, &root_return, &parent, &children, &nchildren);
	
	if (stat == FALSE) {
		//I have no idea why this happens but it does on occasion
		//fprintf(stderr, "Can't query window tree...\n");
		return;
	}
	
	if ( (bRootWindow && iXNumbWindows < nchildren) || !bRootWindow) {
		if (bRootWindow) {
			iXNumbWindows = nchildren;
		}
		XSelectInput(d, root_return, type);
		
		for(i = 0; i < nchildren; i++) {
			XSelectInput(d, children[i], type);
			snoop_all_windows(children[i], type, FALSE);
		}
	}
	
	if (children)
		XFree((char *)children);
}


//Call Back function to java
JNIEXPORT void NotifyJava(JNIEnv *env, jobject obj) {
	jclass cls = env->GetObjectClass(obj);
	jmethodID mid;
	
	mid = env->GetMethodID(cls, "Callback", "(ZIZZ)V");
	if (mid == NULL) {
			return;
	}
	
	env->CallVoidMethod(obj, mid, (jboolean)bKeyDown, (jint)(iKeyCode), (jboolean)(FALSE), (jboolean)(FALSE));
}

JNIEXPORT void JNICALL Java_org_dotnative_globalkeylistner_PollThread_checkKeyboardChanges(JNIEnv *env, jobject obj) {
	snoop_all_windows(DefaultRootWindow(d), KeyPressMask | KeyReleaseMask, TRUE);
	XNextEvent(d, &xev);
	
	iKeyCode = XLookupKeysym(&xev.xkey, xev.xkey.state);
	switch (xev.type) {
		case KeyPress:
			bKeyDown = TRUE;
		break;
		
		case KeyRelease:
			bKeyDown = FALSE;
		break;
	}
	//iKeyCode = xev.xkey.keycode;
	iKeyState = xev.xkey.state;
	iKeyTime = xev.xkey.time;
	
	NotifyJava(env, obj);
}


void Init() {
	//fprintf(stderr, "Loading JNI Keyboard Driver\n");
	d = XOpenDisplay(NULL);
	if (d == NULL) {
		fprintf(stderr, "Can't open display: %s\n", XDisplayName(NULL));
		//TODO NEED TO KILL PROGRAM AT THIS POINT
		//exit(10);
	}
	
	//This is suppose to handle our BadWindow Error
	//but it doesnt work and apperently it slows the
	//system... Bad Idea
	//XSynchronize(d, TRUE);
	
	XSetErrorHandler(XErrIgnore);
	XAutoRepeatOff(d);
	
	//Tell the Plug we have zero windows open.
	iXNumbWindows = 0;
}
 
void Cleanup() {
	XAutoRepeatOn(d);
	XFlush(d);
	XSync(d, FALSE);
	//fprintf(stderr, "Jni Keyboard Driver Unloaded\n");
}
