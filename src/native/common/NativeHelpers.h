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

#ifndef _Included_NativeHelpers_h
#define _Included_NativeHelpers_h

// Retrieves the keyboard auto repeat rate.
extern long int GetAutoRepeatRate();

// Retrieves the keyboard auto repeat delay.
extern long int GetAutoRepeatDelay();

// Retrieves the mouse acceleration multiplier.
extern long int GetPointerAccelerationMultiplier();

// Retrieves the mouse acceleration threshold.
extern long int GetPointerAccelerationThreshold();

// Retrieves the mouse sensitivity.
extern long int GetPointerSensitivity();

// Retrieves the double/triple click interval.
extern long int GetMultiClickTime();

// Native library load callback for platform specific initialization.
extern void OnLibraryLoad();

// Native library unload callback for platform specific de-initialization.
extern void OnLibraryUnload();

#endif
