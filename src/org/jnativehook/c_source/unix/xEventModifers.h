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

//xEvent Modifiers that are not defined by xorg.
#define KeyButMaskAny			0
#define KeyButMaskShift			1
#define KeyButMaskLock			2
#define KeyButMaskControl		4
#define KeyButMaskMod1			8		// Alt Mask
#define KeyButMaskMod2			16		// Number Lock Mask
#define KeyButMaskMod3			32		// ??? Kana Lock Mask ???
#define KeyButMaskMod4			64		// Meta Mask
#define KeyButMaskMod5			128
#define KeyButMaskButton1		256
#define KeyButMaskButton2		512
#define KeyButMaskButton3		1024
#define KeyButMaskButton4		2048
#define KeyButMaskButton5		4096
