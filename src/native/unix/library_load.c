/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <nativehook.h>
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
#include "logger.h"

Display *disp;

// Create a shared object constructor and destructor.
void __attribute__ ((constructor)) on_library_load(void);
void __attribute__ ((destructor)) on_library_unload(void);

void on_library_load() {
	// Display the copyright on library load.
	COPYRIGHT();
	
	// Tell X Threads are OK.
	XInitThreads();

	// Open local display.
	disp = XOpenDisplay(XDisplayName(NULL));
	if (disp == NULL) {
		logger(LOG_LEVEL_ERROR,	"%s [%u]: %s\n", 
				__FUNCTION__, __LINE__, "XOpenDisplay failure!");
	}
	else {
		logger(LOG_LEVEL_DEBUG,	"%s [%u]: %s\n", 
				__FUNCTION__, __LINE__, "XOpenDisplay success.");
	}

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

	if (is_auto_repeat == False) {
		logger(LOG_LEVEL_ERROR,	"%s [%u]: %s\n", 
				__FUNCTION__, __LINE__, "Could not enable detectable auto-repeat!");
	}
	else {
		logger(LOG_LEVEL_DEBUG,	"%s [%u]: Successfully enabled detectable autorepeat.\n", 
				__FUNCTION__, __LINE__);
	}
}

void on_library_unload() {
	/* FIXME This seems to be causing problems with SIGTERM
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
