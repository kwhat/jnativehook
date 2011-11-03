/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _Included_OSXButtonCodes_h
#define _Included_OSXButtonCodes_h

#include <stdbool.h>

/* OSX does not track the button masks of its events so we need to do it
 * manually.
 */

//These codes do not appear to be defined anywhere by Apple.
#define kVK_LBUTTON						kCGMouseButtonLeft
#define kVK_RBUTTON						kCGMouseButtonRight
#define kVK_MBUTTON						kCGMouseButtonCenter
#define kVK_XBUTTON1					3
#define kVK_XBUTTON2					4

#define kCGEventFlagMaskButtonLeft		1
#define kCGEventFlagMaskButtonRight		2
#define kCGEventFlagMaskButtonCenter	4
#define kCGEventFlagMaskXButton1		8
#define kCGEventFlagMaskXButton2		16

void setModifierMask(unsigned short mod);
void unsetModifierMask(unsigned short mod);
bool isModifierMask(unsigned short mod);

#endif
