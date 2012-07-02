/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
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

/***********************************************************************
 * The bulk of this code came from thetechnofreak.com with a few minor 
 * adjustments. According to the author some parts were taken directly 
 * from Microsoft's kbd.h header file that is shipped with the Windows 
 * Driver Development Kit.
 ***********************************************************************/

#ifndef _Included_WinUnicodeHelper
#define _Included_WinUnicodeHelper
 
#include <windows.h>
#include <limits.h>

#if defined(__LP64__) || defined(_LP64) || defined(_WIN64) || defined(__x86_64__)
// If the target is 64-bit we need to build for WOW64 keyboards.
#define KBD_LONG_POINTER __ptr64
#else
#define KBD_LONG_POINTER
#endif
 
#define CAPLOK		0x01
#define WCH_NONE	0xF000
#define WCH_DEAD	0xF001


#define TYPEDEF_VK_TO_WCHARS(n) typedef struct _VK_TO_WCHARS##n { \
	BYTE VirtualKey; \
	BYTE Attributes; \
	WCHAR wch[n]; \
} VK_TO_WCHARS##n, *KBD_LONG_POINTER PVK_TO_WCHARS##n;
 
TYPEDEF_VK_TO_WCHARS(1)
TYPEDEF_VK_TO_WCHARS(2)
TYPEDEF_VK_TO_WCHARS(3)
TYPEDEF_VK_TO_WCHARS(4)
TYPEDEF_VK_TO_WCHARS(5)
TYPEDEF_VK_TO_WCHARS(6)
TYPEDEF_VK_TO_WCHARS(7)
TYPEDEF_VK_TO_WCHARS(8)
TYPEDEF_VK_TO_WCHARS(9)
TYPEDEF_VK_TO_WCHARS(10)

#define TYPEDEF_LIGATURE(n) typedef struct _LIGATURE##n { \
	BYTE VirtualKey; \
	WORD ModificationNumber; \
	WCHAR wch[n]; \
} LIGATURE##n, *KBD_LONG_POINTER PLIGATURE##n;
 
TYPEDEF_LIGATURE(1)
TYPEDEF_LIGATURE(2)
TYPEDEF_LIGATURE(3)
TYPEDEF_LIGATURE(4)
TYPEDEF_LIGATURE(5)

typedef struct {
	BYTE Vk;
	BYTE ModBits;
} VK_TO_BIT, *KBD_LONG_POINTER PVK_TO_BIT;
 
typedef struct {
	PVK_TO_BIT pVkToBit;
	WORD wMaxModBits;
	BYTE ModNumber[];
} MODIFIERS, *KBD_LONG_POINTER PMODIFIERS;
 
typedef struct _VSC_VK {
	BYTE Vsc;
	USHORT Vk;
} VSC_VK, *KBD_LONG_POINTER PVSC_VK;
 
typedef struct _VK_VSC {
	BYTE Vk;
	BYTE Vsc;
} VK_VSC, *KBD_LONG_POINTER PVK_VSC;

typedef struct _VK_TO_WCHAR_TABLE {
	PVK_TO_WCHARS1 pVkToWchars;
	BYTE nModifications;
	BYTE cbSize;
} VK_TO_WCHAR_TABLE, *KBD_LONG_POINTER PVK_TO_WCHAR_TABLE;
 
typedef struct {
	DWORD dwBoth;
	WCHAR wchComposed;
	USHORT uFlags;
} DEADKEY, *KBD_LONG_POINTER PDEADKEY;

typedef struct {
	BYTE vsc;
	WCHAR *KBD_LONG_POINTER pwsz;
} VSC_LPWSTR, *KBD_LONG_POINTER PVSC_LPWSTR;
 
typedef WCHAR *KBD_LONG_POINTER DEADKEY_LPWSTR;
 
typedef struct tagKbdLayer {
	PMODIFIERS pCharModifiers;
	PVK_TO_WCHAR_TABLE pVkToWcharTable;
	PDEADKEY pDeadKey;
	PVSC_LPWSTR pKeyNames;
	PVSC_LPWSTR pKeyNamesExt;
	WCHAR *KBD_LONG_POINTER *KBD_LONG_POINTER pKeyNamesDead;
	USHORT *KBD_LONG_POINTER pusVSCtoVK;
	BYTE bMaxVSCtoVK;
	PVSC_VK pVSCtoVK_E0;
	PVSC_VK pVSCtoVK_E1;
	DWORD fLocaleFlags;
	BYTE nLgMax;
	BYTE cbLgEntry;
	PLIGATURE1 pLigature;
	DWORD dwType;
	DWORD dwSubType;
} KBDTABLES, *KBD_LONG_POINTER PKBDTABLES;
 
typedef struct _VK_FUNCTION_PARAM {
	BYTE NLSFEProcIndex;
	ULONG NLSFEProcParam;
} VK_FPARAM, *KBD_LONG_POINTER PVK_FPARAM;
 
typedef struct _VK_TO_FUNCTION_TABLE {
	BYTE Vk;
	BYTE NLSFEProcType;
	BYTE NLSFEProcCurrent;
	BYTE NLSFEProcSwitch;
	VK_FPARAM NLSFEProc[8];
	VK_FPARAM NLSFEProcAlt[8];
} VK_F, *KBD_LONG_POINTER PVK_F;

typedef struct tagKbdNlsLayer {
	USHORT OEMIdentifier;
	USHORT LayoutInformation;
	UINT NumOfVkToF;
	PVK_F pVkToF;
	INT NumOfMouseVKey;
	USHORT *KBD_LONG_POINTER pusMouseVKey;
} KBDNLSTABLES, *KBD_LONG_POINTER PKBDNLSTABLES;
 

// Extended macros
 #define INIT_PVK_TO_WCHARS(i, n) \
	case n: \
		pVkToWchars##n = (PVK_TO_WCHARS##n) pKbd->pVkToWcharTable[i].pVkToWchars; \
		break; \

#define SEARCH_VK_IN_CONVERSION_TABLE(n) \
	i = 0; \
	if(pVkToWchars##n && (mod < n)) \
	{ \
		do \
		{ \
			if(pVkToWchars##n[i].VirtualKey == virtualKey) \
			{ \
				if((pVkToWchars##n[i].Attributes == CAPLOK) && capsLock) { \
					if(mod == shift) mod = 0; else mod = shift; \
				} \
				*outputChar = pVkToWchars##n[i].wch[mod]; \
				charCount = 1; \
				if(*outputChar == WCH_NONE) { charCount = 0; } \
				else if(*outputChar == WCH_DEAD) \
				{ \
					*deadChar = pVkToWchars##n[i + 1].wch[mod]; \
					charCount = 0; \
				} \
				break;\
			} \
		} \
		while(pVkToWchars##n[++i].VirtualKey != 0); \
	} \


HINSTANCE LoadInputHelper();
int UnloadInputHelper(HINSTANCE kbdLibrary);
int ConvertVirtualKeyToWChar(int virtualKey, PWCHAR out, PWCHAR buffer);

#endif
