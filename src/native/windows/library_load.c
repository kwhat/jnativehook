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
#include <windows.h>

#include "library_load.h"
#include "logger.h"
#include "win_unicode_helper.h"

// Global Variables.
HINSTANCE hInst = NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			//hInst = hinstDLL;
			on_library_load();
			break;
		case DLL_PROCESS_DETACH:
			on_library_unload();
	        break;
    }

    return TRUE;
}

void on_library_load() {
	hInst = GetModuleHandle(NULL);

	// Display the copyright on library load.
	COPYRIGHT();
	
	load_unicode_helper();
}

void on_library_unload() {
	unload_unicode_helper();
}
