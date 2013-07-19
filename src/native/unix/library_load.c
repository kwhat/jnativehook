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

#include <config.h>
#include <nativehook.h>

#ifdef USE_DEBUG
#include <stdio.h>
#endif

#include <X11/Xlib.h>

#ifdef USE_XKB
#include <X11/XKBlib.h>
#endif

#ifdef USE_XT
#include <X11/Intrinsic.h>

XtAppContext xt_context;
Display *xt_disp;
#endif

#include "library_load.h"

Display *disp;

// Create a shared object constructor and destructor.
void __attribute__ ((constructor)) on_library_load(void);
void __attribute__ ((destructor)) on_library_unload(void);

void on_library_load() {
	// Tell X Threads are OK.
	XInitThreads();

	// Open local display.
	disp = XOpenDisplay(XDisplayName(NULL));
	#ifdef USE_DEBUG
	if (disp != NULL) {
		fprintf(stdout, "on_library_load(): XOpenDisplay successful.\n");
	}
	else {
		fprintf(stderr, "on_library_load(): XOpenDisplay failure!\n");
	}
	#endif

	#ifdef USE_XT
	XtToolkitInitialize();
	xt_context = XtCreateApplicationContext();

	int argc = 0;
	char ** argv = { NULL };
	xt_disp = XtOpenDisplay(xt_context, NULL, "NativeHook", "libnativehook", NULL, 0, &argc, argv);
	#endif

	// NOTE: is_auto_repeat is NOT stdbool!
	Bool is_auto_repeat = False;
	#ifdef USE_XKB
	// Enable detectable autorepeat.
	XkbSetDetectableAutoRepeat(disp, True, &is_auto_repeat);
	#else
	XAutoRepeatOn(disp);

	XKeyboardState kb_state;
	XGetKeyboardControl(disp, &kb_state);

	is_auto_repeat = (kb_state.global_auto_repeat == AutoRepeatModeOn);
	#endif

	#ifdef USE_DEBUG
	if (is_auto_repeat) {
		fprintf(stdout, "on_library_load(): Successfully enabled detectable autorepeat.\n");
	}
	else {
		fprintf(stderr, "on_library_load(): Could not enable detectable auto-repeat!\n");
	}
	#endif
}

void on_library_unload() {
	/* TODO This seems to be causing problems with SIGTERM
	// Stop the native thread if its running.
	if (hook_is_enabled()) {
		hook_disable();
	}
	*/

	#ifdef USE_XT
	XtCloseDisplay(xt_disp);
	XtDestroyApplicationContext(xt_context);
	#endif

	// Destroy the native displays.
	if (disp != NULL) {
		XCloseDisplay(disp);
		disp = NULL;
	}
}
