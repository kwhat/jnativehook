/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
 * https://github.com/kwhat/jnativehook/
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

#ifndef _Included_NativeThread_h
#define _Included_NativeThread_h

#include <stdbool.h>

// Structure get transport exceptions out of the native thread.
typedef struct {
	char *class;
	char *message;
} Exception;

// Attempt to start the global hook thread.
extern int StartNativeThread();

// Attempt to stop the global hook thread.
extern int StopNativeThread();

// Determine the status of the global hook thread.
extern bool IsNativeThreadRunning();

#endif
