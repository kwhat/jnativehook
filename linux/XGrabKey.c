#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <stdio.h>

unsigned int NumberLockMask = 0;
unsigned int ScrollLockMask = 0;
unsigned int CapsLockMask = 0;

/* 
This method locates key masks like scroll lock, number lock and caps lock
because x does not provide fixed constatns for them.  Vars to populate are 
located above this comment.
*/
void getModifiers (Display * dpy) {
	int i;
	XModifierKeymap *modmap;
	KeyCode nlock, slock;
	static int mask_table[8] = {
		ShiftMask, LockMask, ControlMask, Mod1Mask,
		Mod2Mask, Mod3Mask, Mod4Mask, Mod5Mask
	};
	
	nlock = XKeysymToKeycode (dpy, XK_Num_Lock);
	slock = XKeysymToKeycode (dpy, XK_Scroll_Lock);
	
	// Find out the masks for the NumLock and ScrollLock modifiers,
	// so that we can bind the grabs for when they are enabled too.
	modmap = XGetModifierMapping (dpy);
	
	if (modmap != NULL && modmap->max_keypermod > 0) {
		for (i = 0; i < 8 * modmap->max_keypermod; i++) {
			if (modmap->modifiermap[i] == nlock && nlock != 0) {
				NumberLockMask = mask_table[i / modmap->max_keypermod];
			}
			else if (modmap->modifiermap[i] == slock && slock != 0) {
				ScrollLockMask = mask_table[i / modmap->max_keypermod];
			}
		}
	}
	
	CapsLockMask = LockMask;
	
	if (modmap) {
		XFreeModifiermap (modmap);
	}
}

int main( int argc, const char* argv[] ) {
	char *dispName = XDisplayName(NULL);
	Display *disp = XOpenDisplay(dispName);
	Window grabWin = DefaultRootWindow(disp);
	XAllowEvents(disp, AsyncKeyboard, CurrentTime);
	XkbSetDetectableAutoRepeat(disp, 1, NULL);

	// Print repate reate and repate delay.	
	unsigned int xkb_timeout;
	unsigned int xkb_interval;
	if (! XkbGetAutoRepeatRate(disp, XkbUseCoreKbd, &xkb_timeout, &xkb_interval) ) {
		printf("XkbGetAutoRepeatRate failure!\n", xkb_interval, xkb_timeout);
	}
	else {
		printf("XkbGetAutoRepeatRate successful (rate: %i) (delay: %i)\n", xkb_interval, xkb_timeout);
	}

	//Itterate over screens
	int screen;
	for (screen = 0; screen < ScreenCount (disp); screen++) {
		printf ("Init Screen %i\n", screen);
		XSelectInput (disp, RootWindow (disp, screen), KeyPressMask | KeyReleaseMask);
	}

	//This will grab the number lock and scroll lock masks.
	getModifiers (disp);
	
	/*
	int min_keycode, max_keycode;
	XDisplayKeycodes (disp, &min_keycode, &max_keycode);
	printf("Key Codes %i %i\n", min_keycode, max_keycode);
	*/
	
	// char * key_string = "b";
	KeyCode key = XKeysymToKeycode(disp, XK_B);
	printf ("keycode=%d\n", key);

	int modMask = 0;
	//modmask |= AnyModifier;
	//modmask = LockMask;
	//modmask |= numlock_mask | capslock_mask | scrolllock_mask;
	//modmask = NumberLockMask;
	//printf ("modmask=%d\n", modmask);

	//*
	//No Modifier
	XGrabKey(disp, key, modMask, grabWin, True, GrabModeAsync, GrabModeAsync);
	
	//ShiftMask
	XGrabKey(disp, key, modMask | ShiftMask, grabWin, True, GrabModeAsync, GrabModeAsync);

	//NumberLockMask
	XGrabKey(disp, key, modMask | NumberLockMask, grabWin, True, GrabModeAsync, GrabModeAsync);

	//ScrollLockMask
	XGrabKey(disp, key, modMask | ScrollLockMask, grabWin, True, GrabModeAsync, GrabModeAsync);

	//CapsLockMask
	XGrabKey(disp, key, modMask | CapsLockMask, grabWin, True, GrabModeAsync, GrabModeAsync);

	//NumberLockMask | ScrollLockMask
	XGrabKey(disp, key, modMask | NumberLockMask | ScrollLockMask, grabWin, True, GrabModeAsync, GrabModeAsync);

	//NumberLockMask | CapsLockMask
	XGrabKey(disp, key, modMask | NumberLockMask | CapsLockMask, grabWin, True, GrabModeAsync, GrabModeAsync);
	
	//ScrollLockMask | CapsLockMask
	XGrabKey(disp, key, modMask | ScrollLockMask | CapsLockMask, grabWin, True, GrabModeAsync, GrabModeAsync);
	
	//NumberLockMask | ScrollLockMask | CapsLockMask
	XGrabKey(disp, key, modMask | NumberLockMask | ScrollLockMask | CapsLockMask, grabWin, True, GrabModeAsync, GrabModeAsync);
	//*/

	XEvent e;
	while ( True ) {
		//Block waiting for the next event
		XNextEvent(disp, &e);

		switch (e.type) {
			case KeyPress:
				printf ("Key press !\n");
				printf ("e.xkey.keycode=%d\n", e.xkey.keycode);
				printf ("e.xkey.state=%d\n", e.xkey.state);
			break;
			
			case KeyRelease:
				printf ("Key release !\n");
				printf ("e.xkey.keycode=%d\n", e.xkey.keycode);
				printf ("e.xkey.state=%d\n", e.xkey.state);
			break;
		}
	}
}
