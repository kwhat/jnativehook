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

// Reference: http://boredzo.org/blog/wp-content/uploads/2007/05/imtx-virtual-keycodes.png
// Reference: https://svn.blender.org/svnroot/bf-blender/branches/render25/intern/ghost/intern/GHOST_SystemCocoa.mm
// Reference: http://www.mactech.com/macintosh-c/chap02-1.html

#ifndef _Included_OSXInputHelpers
#define _Included_OSXInputHelpers

#include <stdbool.h>

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>	// For HIToolbox kVK_ keycodes and TIS funcitons.


// These virtual key codes do not appear to be defined anywhere by Apple.
#define kVK_RightCommand				0x36
#define kVK_ContextMenu					0x6E	// AKA kMenuPowerGlyph


// These button codes do not appear to be defined anywhere by Apple.
#define kVK_LBUTTON						kCGMouseButtonLeft
#define kVK_RBUTTON						kCGMouseButtonRight
#define kVK_MBUTTON						kCGMouseButtonCenter
#define kVK_XBUTTON1					3
#define kVK_XBUTTON2					4

// These button masks do not appear to be defined anywhere by Apple.
#define kCGEventFlagMaskButtonLeft		1
#define kCGEventFlagMaskButtonRight		2
#define kCGEventFlagMaskButtonCenter	4
#define kCGEventFlagMaskXButton1		8
#define kCGEventFlagMaskXButton2		16


/* Set the native modifier mask for future events.
 */
extern void SetModifierMask(CGEventFlags mask);

/* Unset the native modifier mask for future events.
 */
extern void UnsetModifierMask(CGEventFlags mask);

/* Get the current native modifier mask state.
 */
extern CGEventFlags GetModifiers();

/* Converts an OSX key code and event mask to the appropriate Unicode character
 * representation.
 */
extern void KeyCodeToString(CGEventRef event, UniCharCount size, UniCharCount *length, UniChar *buffer);

/* Initialize items required for KeyCodeToKeySym() and KeySymToUnicode()
 * functionality.  This method is called by OnLibraryLoad() and may need to be
 * called in combination with UnloadInputHelper() if the native keyboard layout
 * is changed.
 */
extern void LoadInputHelper();

/* De-initialize items required for KeyCodeToKeySym() and KeySymToUnicode()
 * functionality.  This method is called by OnLibraryUnload() and may need to be
 * called in combination with LoadInputHelper() if the native keyboard layout
 * is changed.
 */
extern void UnloadInputHelper();

#endif
