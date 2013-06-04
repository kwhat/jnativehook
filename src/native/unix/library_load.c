/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <X11/Xlib.h>
#include "library_load.h"

Display *disp;

// Create a shared object constructor and destructor.
void __attribute__ ((constructor)) on_library_load(void);
void __attribute__ ((destructor)) on_library_unload(void);

void on_library_load() {
	// Tell X Threads are OK.
	XInitThreads();

	#ifdef XT
	XtToolkitInitialize();
	app_context = XtCreateApplicationContext();
	#endif

	// Open local display.
	disp = XOpenDisplay(XDisplayName(NULL));
	#ifdef DEBUG
	if (disp != NULL) {
		fprintf(stdout, "on_library_load(): XOpenDisplay successful.\n");
	}
	else {
		fprintf(stderr, "on_library_load(): XOpenDisplay failure!\n");
	}
	#endif

	Bool isAutoRepeat = False;
	#ifdef XKB
	// Enable detectable autorepeat.
	XkbSetDetectableAutoRepeat(disp, True, &isAutoRepeat);
	#else
	XAutoRepeatOn(disp);

	XKeyboardState kb_state;
	XGetKeyboardControl(disp, &kb_state);

	isAutoRepeat = (kb_state.global_auto_repeat == AutoRepeatModeOn);
	#endif

	#ifdef DEBUG
	if (isAutoRepeat) {
		fprintf(stdout, "on_library_load(): Successfully enabled detectable autorepeat.\n");
	}
	else {
		fprintf(stderr, "on_library_load(): Could not enable detectable auto-repeat!\n");
	}
	#endif
}

void on_library_unload() {
	#ifdef XT
	XtDestroyApplicationContext(app_context);
	#endif

	// Destroy the native displays.
	if (disp != NULL) {
		XCloseDisplay(disp);
		disp = NULL;
	}
}
