#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>

//System.setProperty("sun.awt.enableExtraMouseButtons", "true");


unsigned int NumberLockMask = 0;
unsigned int ScrollLockMask = 0;
unsigned int CapsLockMask = 0;

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
	XAllowEvents(disp, AsyncBoth, CurrentTime);
	
	//Itterate over screens
	int screen;
	for (screen = 0; screen < ScreenCount (disp); screen++) {
		printf ("Init Screen %i\n", screen);
		XSelectInput(disp, RootWindow(disp, screen), MotionNotify);
	}

	//This will grab the number lock and scroll lock masks.
	//getModifiers (disp);
	
	/*
	int min_keycode, max_keycode;
	XDisplayKeycodes (disp, &min_keycode, &max_keycode);
	printf("Key Codes %i %i\n", min_keycode, max_keycode);
	*/
	
	char * key_string = "b";
	KeyCode key = XKeysymToKeycode(disp, XStringToKeysym(key_string));
	printf ("keycode=%d\n", key);

	int modMask = 0;
	//modmask |= AnyModifier;
	//modmask = LockMask;
	//modmask |= numlock_mask | capslock_mask | scrolllock_mask;
	//modmask = NumberLockMask;
	//printf ("modmask=%d\n", modmask);


	//No Modifier
	XGrabPointer(disp, grabWin, True, PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
	//XGrabButton(disp, Button1, AnyModifier, grabWin, True, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None);

	XEvent e;
	while ( True ) {
		//Block waiting for the next event
		XNextEvent(disp, &e);

		switch (e.type) {
			case MotionNotify:
				printf("Native: MsgLoop - Mouse moved (%i, %i)\n", e.xmotion.x, e.xmotion.y);
			break;
			
		}

	}
}
