//Test CPP.
// Use XEvie
/*
emerge -av libXevie

Section "Extensions"
    ...
    Option         "XEVIE" "Enable"
    ...
EndSection

*/

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
//#include <X11/StringDefs.h>
//#include <X11/Shell.h>

/*
#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/extensions/Xevie.h>
*/

char *TranslateKeyCode(XEvent *ev);
Display  *disp;
XEvent xev;

int iKeyCode;
int iKeyState;
int iKeyTime;


int main(int argc, char* argv[]) {
	//fprintf(stderr, "Loading JNI Keyboard Driver\n");
	disp = XOpenDisplay(NULL);
	if (disp == NULL) {
		fprintf(stderr, "Can't open display: %s\n", XDisplayName(NULL));
		//TODO NEED TO KILL PROGRAM AT THIS POINT
		//exit(10);
	}
	
	//This is suppose to handle our BadWindow Error
	//but it doesnt work and apperently it slows the
	//system... Bad Idea
	//XSynchronize(disp, TRUE);
	
	//XSetErrorHandler(XErrIgnore);
	//XAutoRepeatOff(disp);
	
	if(XevieStart(disp)) {
		printf("XevieStart(dpy) finished \n");
	}
	else {
		printf("XevieStart(dpy) failed, only one client is allowed to do event interception\n");
		exit(1);
	}
	
	
	XevieSelectInput(disp, KeyPressMask | KeyReleaseMask);
	XkbSetDetectableAutoRepeat(disp, TRUE, NULL);
	
	while (TRUE) {
		XNextEvent(disp, &xev);
		
		iKeyCode = xev.xkey.keycode;
		//iKeyCode = XLookupKeysym(&xev.xkey, xev.xkey.state);
		iKeyState = xev.xkey.state;
		iKeyTime = xev.xkey.time;
		int iKeyType = xev.type;
		Window tmp = xev.xkey.window;
		
		//
		
		//XAllowEvents(disp, ReplayKeyboard, CurrentTime);
		
		
		//XPutBackEvent(disp, &xev);
		//XSelectInput(disp, tmp, KeyPressMask | KeyReleaseMask);
		//XGrabKey(disp, iKeyCode, None, tmp, TRUE, GrabModeAsync, GrabModeAsync);
		//XNextEvent(disp, &xev);
		
		//XSendEvent(disp, tmp, FALSE, xev.type, &xev); 
		//XSync(disp, FALSE);
		//XFlush(disp);
		
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
	
	XevieEnd(disp);
}
