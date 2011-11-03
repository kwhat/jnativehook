/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2011 Alexander Barker.  All Rights Received.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation either version 3 of the License or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not see <http://www.gnu.org/licenses/>.
 */

#ifndef _Included_OSXKeyCodes_h
#define _Included_OSXKeyCodes_h

//Reference: http://boredzo.org/blog/wp-content/uploads/2007/05/imtx-virtual-keycodes.png
//Reference: https://svn.blender.org/svnroot/bf-blender/branches/render25/intern/ghost/intern/GHOST_SystemCocoa.mm
//Reference: http://www.mactech.com/macintosh-c/chap02-1.html

//#include <Carbon/Carbon.h> //For HIToolbox kVK_ keycode constants.
//FIXME We should be using the IOKit constants outlined below.
//#include <IOKit/hid/IOHIDUsageTables.h> //For IOKit kHIDUsage_KeyboardA keycode constants.

//Used in lieu of the Carbon frameworks HIToolbox.
//keycodes for ansi keys
#define kVK_ANSI_A						0x00
#define kVK_ANSI_S						0x01
#define kVK_ANSI_D						0x02
#define kVK_ANSI_F						0x03
#define kVK_ANSI_H						0x04
#define kVK_ANSI_G						0x05
#define kVK_ANSI_Z						0x06
#define kVK_ANSI_X						0x07
#define kVK_ANSI_C						0x08
#define kVK_ANSI_V						0x09
#define kVK_ANSI_B						0x0B
#define kVK_ANSI_Q						0x0C
#define kVK_ANSI_W						0x0D
#define kVK_ANSI_E						0x0E
#define kVK_ANSI_R						0x0F
#define kVK_ANSI_Y						0x10
#define kVK_ANSI_T						0x11
#define kVK_ANSI_1						0x12
#define kVK_ANSI_2						0x13
#define kVK_ANSI_3						0x14
#define kVK_ANSI_4						0x15
#define kVK_ANSI_6						0x16
#define kVK_ANSI_5						0x17
#define kVK_ANSI_Equal					0x18
#define kVK_ANSI_9						0x19
#define kVK_ANSI_7						0x1A
#define kVK_ANSI_Minus					0x1B
#define kVK_ANSI_8						0x1C
#define kVK_ANSI_0						0x1D
#define kVK_ANSI_RightBracket			0x1E
#define kVK_ANSI_O						0x1F
#define kVK_ANSI_U						0x20
#define kVK_ANSI_LeftBracket			0x21
#define kVK_ANSI_I						0x22
#define kVK_ANSI_P						0x23
#define kVK_ANSI_L						0x25
#define kVK_ANSI_J						0x26
#define kVK_ANSI_Quote					0x27
#define kVK_ANSI_K						0x28
#define kVK_ANSI_Semicolon				0x29
#define kVK_ANSI_Backslash				0x2A
#define kVK_ANSI_Comma					0x2B
#define kVK_ANSI_Slash					0x2C
#define kVK_ANSI_N						0x2D
#define kVK_ANSI_M						0x2E
#define kVK_ANSI_Period				 	0x2F
#define kVK_ANSI_Grave					0x32
#define kVK_ANSI_KeypadDecimal			0x41
#define kVK_ANSI_KeypadMultiply	 		0x43
#define kVK_ANSI_KeypadPlus		 		0x45
#define kVK_ANSI_KeypadClear			0x47
#define kVK_ANSI_KeypadDivide			0x4B
#define kVK_ANSI_KeypadEnter			0x4C
#define kVK_ANSI_KeypadMinus			0x4E
#define kVK_ANSI_KeypadEquals			0x51
#define kVK_ANSI_Keypad0				0x52
#define kVK_ANSI_Keypad1				0x53
#define kVK_ANSI_Keypad2				0x54
#define kVK_ANSI_Keypad3				0x55
#define kVK_ANSI_Keypad4				0x56
#define kVK_ANSI_Keypad5				0x57
#define kVK_ANSI_Keypad6				0x58
#define kVK_ANSI_Keypad7				0x59
#define kVK_ANSI_Keypad8				0x5B
#define kVK_ANSI_Keypad9				0x5C

//keycodes for keys that are independent of keyboard layout
#define kVK_Return						0x24
#define kVK_Tab					 		0x30
#define kVK_Space						0x31
#define kVK_Delete						0x33
#define kVK_Escape						0x35
#define kVK_Command					 	0x37
#define kVK_Shift						0x38
#define kVK_CapsLock					0x39
#define kVK_Option						0x3A
#define kVK_Control					 	0x3B
#define kVK_RightShift					0x3C
#define kVK_RightOption				 	0x3D
#define kVK_RightControl				0x3E
#define kVK_Function					0x3F
#define kVK_F17						 	0x40
#define kVK_VolumeUp					0x48
#define kVK_VolumeDown					0x49
#define kVK_Mute						0x4A
#define kVK_F18						 	0x4F
#define kVK_F19						 	0x50
#define kVK_F20						 	0x5A
#define kVK_F5							0x60
#define kVK_F6							0x61
#define kVK_F7							0x62
#define kVK_F3							0x63
#define kVK_F8							0x64
#define kVK_F9							0x65
#define kVK_F11						 	0x67
#define kVK_F13						 	0x69
#define kVK_F16						 	0x6A
#define kVK_F14						 	0x6B
#define kVK_F10						 	0x6D
#define kVK_F12						 	0x6F
#define kVK_F15						 	0x71
#define kVK_Help						0x72
#define kVK_Home						0x73
#define kVK_PageUp						0x74
#define kVK_ForwardDelete				0x75
#define kVK_F4							0x76
#define kVK_End						 	0x77
#define kVK_F2							0x78
#define kVK_PageDown					0x79
#define kVK_F1							0x7A
#define kVK_LeftArrow					0x7B
#define kVK_RightArrow					0x7C
#define kVK_DownArrow					0x7D
#define kVK_UpArrow					 	0x7E

//ISO keyboards only
#define kVK_ISO_Section				 	0x0A

//JIS keyboards only
#define kVK_JIS_Yen					 	0x5D
#define kVK_JIS_Underscore				0x5E
#define kVK_JIS_KeypadComma		 		0x5F
#define kVK_JIS_Eisu					0x66
#define kVK_JIS_Kana					0x68

//Suplemental virtual key information
#define kVK_RightCommand				0x36
#define kVK_ContextMenu					0x6E	//AKA kMenuPowerGlyph

#endif
