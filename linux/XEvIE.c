#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/Xevie.h>

int main(int argc, char **argv) {
	// xlib
	Display *display;

	// evdev
	XEvent xev;
	XClientMessageEvent *client_msg;

	
	display = XOpenDisplay(NULL);
	if(display == NULL) {
		printf("Error: Could not open display!\n");
		return 1;
	}
	
	if(!XevieStart(display)) {
		printf("XevieStart(disp) failure!\n");
		return 1;
	}
	
	//Select the input masks we would like to listen for.
	XevieSelectInput(display, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

	//Detectable auto repeate is broken in XEvIE.
	//XkbSetDetectableAutoRepeat(display, True, NULL);
	
	// main loop after selecting window
	while(true) {
		//Get the next event
		XNextEvent(display, &xev);
		client_msg = (XClientMessageEvent *) &event;
		
		//unsigned int iKeyCode = xev.xkey.keycode;
		//unsigned int iKeyCode = XLookupKeysym(&xev.xkey, xev.xkey.state);
		unsigned int iKeyCode = XLookupKeysym(&xev.xkey, 0);
		//unsigned int iKeyState = xev.xkey.state;
		//unsigned long iKeyTime = xev.xkey.time;
		int iKeyType = xev.type;
		
		switch (xev.type) {
			case KeyPress:
				XKeyEvent *key_ev = (XKeyEvent *) xev;
				unsigned int key_code = XLookupKeysym(&key_ev.keycode, 0);
				printf("Key Press - %i\n", key_code);
			break;

			case KeyRelease:
				XKeyEvent *key_ev = (XKeyEvent *) xev;
				unsigned int key_code = XLookupKeysym(&key_ev.keycode, 0);
				printf("Key Release - %i\n", key_code);
			break;

			case ButtonPress:
				XButtonEvent *button_ev = (XButtonEvent *) xev;
				printf(" Button Press - %i\n", button_ev.button);
			break;

			case ButtonRelease:
				XButtonEvent *button_ev = (XButtonEvent *) xev;
				printf(" Button Release - %i\n", button_ev.button);
			break;

			case MotionNotify:
				XMotionEvent *motion_ev = (XMotionEvent *) xev;
				printf("Motion Notify - %i x %i\n", motion_ev.x_root, motion_ev.y_root);
			break;

			default:
				printf(" unknown event %x\n", event.type);
			break;
		}
		
		XevieSendEvent(display, &xev, XEVIE_UNMODIFIED);
	}

	XevieEnd(display);
	printf("XevieEnd finished\n");

	return 0;
}
