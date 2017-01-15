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

/***********************************************************************
 * The following code is based on code provided by Marc-André Moreau
 * to work around a failure to support dead keys in the ToUnicode() API.
 * According to the author some parts were taken directly from
 * Microsoft's  kbd.h header file that is shipped with the Windows
 * Driver Development Kit.
 *
 * The original code was substantially modified to provide the following:
 *   1) More dynamic code structure.
 *   2) Support for compilers that do not implement _ptr64 (GCC / LLVM).
 *   3) Support for Wow64 at runtime via 32-bit binary.
 *   4) Support for contextual language switching.
 *
 * I have contacted Marc-André Moreau who has granted permission for
 * his original source code to be used under the Public Domain.  Although
 * the JNativeHook library as a whole is currently covered under the GPLv3,
 * please feel free to use and learn from the source code contained in this
 * file under the terms of the Public Domain.
 *
 * For further reading and the original code, please visit:
 *   http://legacy.docdroppers.org/wiki/index.php?title=Writing_Keyloggers
 *   http://www.techmantras.com/content/writing-keyloggers-full-length-tutorial
 *
 ***********************************************************************/

#ifndef _Included_WinUnicodeHelper
#define _Included_WinUnicodeHelper

#include <windows.h>
#include <limits.h>

/* Assume that any compiler defining _MSC_VER knows about LPFN_ISWOW64PROCESS.
 * This assertion has not been tested.  If you are using a compiler that
 * defines _MSC_VER but does not define LPFN_ISWOW64PROCESS, such as ICC for
 * windows, please file a bug report.
 */
#ifndef _MSC_VER
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
#endif

typedef void* (CALLBACK *KbdLayerDescriptor) (VOID);

#define CAPLOK		0x01
#define WCH_NONE	0xF000
#define WCH_DEAD	0xF001

typedef struct _VK_TO_WCHARS {
	BYTE VirtualKey;
	BYTE Attributes;
	WCHAR wch[];
} VK_TO_WCHARS, *PVK_TO_WCHARS;

typedef struct _LIGATURE {
	BYTE VirtualKey;
	WORD ModificationNumber;
	WCHAR wch[];
} LIGATURE, *PLIGATURE;

typedef struct _VK_TO_BIT {
	BYTE Vk;
	BYTE ModBits;
} VK_TO_BIT, *PVK_TO_BIT;

typedef struct _MODIFIERS {
	PVK_TO_BIT pVkToBit; // __ptr64
	WORD wMaxModBits;
	BYTE ModNumber[];
} MODIFIERS, *PMODIFIERS;

typedef struct _VSC_VK {
	BYTE Vsc;
	USHORT Vk;
} VSC_VK, *PVSC_VK;

typedef struct _VK_TO_WCHAR_TABLE {
	//PVK_TO_WCHARS1 pVkToWchars; // __ptr64
	PVK_TO_WCHARS pVkToWchars; // __ptr64
	BYTE nModifications;
	BYTE cbSize;
} VK_TO_WCHAR_TABLE, *PVK_TO_WCHAR_TABLE;

typedef struct _DEADKEY {
	DWORD dwBoth;
	WCHAR wchComposed;
	USHORT uFlags;
} DEADKEY, *PDEADKEY;

typedef struct _VSC_LPWSTR {
	BYTE vsc;
	WCHAR *pwsz; // __ptr64
} VSC_LPWSTR, *PVSC_LPWSTR;

typedef struct tagKbdLayer {
	PMODIFIERS pCharModifiers; // __ptr64
	PVK_TO_WCHAR_TABLE pVkToWcharTable; // __ptr64
	PDEADKEY pDeadKey; // __ptr64
	PVSC_LPWSTR pKeyNames; // __ptr64
	PVSC_LPWSTR pKeyNamesExt; // __ptr64
	WCHAR **pKeyNamesDead; // __ptr64
	USHORT *pusVSCtoVK; // __ptr64
	BYTE bMaxVSCtoVK;
	PVSC_VK pVSCtoVK_E0; // __ptr64
	PVSC_VK pVSCtoVK_E1; // __ptr64
	DWORD fLocaleFlags;
	BYTE nLgMax;
	BYTE cbLgEntry;
	//PLIGATURE1 pLigature; // __ptr64
	PLIGATURE pLigature; // __ptr64
	DWORD dwType;
	DWORD dwSubType;
} KBDTABLES, *PKBDTABLES; // __ptr64

int LoadUnicodeHelper();
int UnloadUnicodeHelper();
int ConvertVirtualKeyToWChar(int virtualKey, PWCHAR out, PWCHAR buffer);

#endif
