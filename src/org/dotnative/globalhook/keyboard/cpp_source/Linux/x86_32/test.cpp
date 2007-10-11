#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xutil.h>
#include <X11/Shell.h>

char *TranslateKeyCode(XEvent *ev);
Display  *disp;
XEvent xev;

int iKeyCode;
int iKeyState;
int iKeyTime;

void snoop_x() {
	//AnyKey
	//AnyModifier
	//XGrabKey(disp, XKeysymToKeycode(disp, XStringToKeysym("b")), None, DefaultRootWindow(disp), TRUE, GrabModeSync, GrabModeSync); 
}

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
	
	XkbSetDetectableAutoRepeat(disp, TRUE, NULL);
	snoop_x();
	
	while (TRUE) {
		XGrabKeyboard(disp, DefaultRootWindow(disp), TRUE, GrabModeAsync, GrabModeAsync, CurrentTime);
		XNextEvent(disp, &xev);
		iKeyCode = xev.xkey.keycode;
		//iKeyCode = XLookupKeysym(&xev.xkey, xev.xkey.state);
		iKeyState = xev.xkey.state;
		iKeyTime = xev.xkey.time;
		int iKeyType = xev.type;
		Window tmp = xev.xkey.window;
		XUngrabKeyboard(disp, CurrentTime);
		
		
		
		XPutBackEvent(disp, &xev);
		XSelectInput(disp, tmp, KeyPressMask | KeyReleaseMask);
		XGrabKey(disp, iKeyCode, None, tmp, TRUE, GrabModeAsync, GrabModeAsync);
		XNextEvent(disp, &xev);
		
		//XSendEvent(disp, InputFocus, FALSE, xev.type, &xev); 
		XAllowEvents(disp, ReplayKeyboard, iKeyTime);
		
		

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


		
		//XSetInputFocus(disp, PointerRoot, RevertToParent, iKeyTime);
		//XSendEvent(disp, xev.xkey.subwindow, FALSE, xev.type, &xev); 

		XSync(disp, FALSE);
		XFlush(disp);

		
		//XUngrabKey(disp, xev.xkey.keycode, None, DefaultRootWindow(disp));
	}
}
