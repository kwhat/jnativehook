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
 * The following code is based on code provided by thetechnofreak.com 
 * to work around a failure to support dead keys in the ToUnicode() API.
 * According to the author some parts were taken directly from 
 * Microsoft's  kbd.h header file that is shipped with the Windows 
 * Driver Development Kit.
 * 
 * The original code was substantially modified to provide the following:
 *   1) More dynamic code structure.
 *   2) Support for compilers that do not implement _ptr64 (GCC / LLVM).
 *   3) Support for Wow64 at runtime via 32-bit binary.
 * 
 * For further reading and the original code, please visit:
 *   http://thetechnofreak.com/technofreak/keylogger-visual-c/
 ***********************************************************************/

#include <stddef.h>
#include "WinUnicodeHelper.h"
#include "NativeGlobals.h"

// These are pointers to arrays of their respective structures.
static PVK_TO_BIT pVkToBit;
static PVK_TO_WCHAR_TABLE pVkToWcharTable;
static PDEADKEY pDeadKey;

// Amount of pointer padding to apply for Wow64 instances.
static short int ptrPadding = 0;

// Keyboard DLL instance.
HINSTANCE kbdLibrary = NULL;

#if defined(_WIN32) && !defined(_WIN64)
// Small function to check and see if we are executing under Wow64.
static BOOL IsWow64() {
	BOOL bIsWow64 = FALSE;

	LPFN_ISWOW64PROCESS pIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle("kernel32"), "IsWow64Process");

	if(pIsWow64Process != NULL) {
		if (!pIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
			bIsWow64 = FALSE;
			#ifdef DEBUG
			fprintf(stderr, "IsWow64(): pIsWow64Process(GetCurrentProcess(), &bIsWow64) failed!\n");
			#endif
		}
	}
	
	return bIsWow64;
}
#endif

// Locate the DLL that contains the current keyboard layout.
static int GetKeyboardLayoutFile(char *layoutFile, DWORD bufferSize) {
	int status = RETURN_FAILURE;
	HKEY hKey;
	DWORD varType = REG_SZ;

	char kbdName[KL_NAMELENGTH];
	if (GetKeyboardLayoutName(kbdName)) {
		char kbdKeyPath[51 + KL_NAMELENGTH];
		snprintf(kbdKeyPath, 51 + KL_NAMELENGTH, "SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\%s", kbdName);

		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCTSTR) kbdKeyPath, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
			if(RegQueryValueEx(hKey, "Layout File", NULL, &varType, (LPBYTE) layoutFile, &bufferSize) == ERROR_SUCCESS) {
				RegCloseKey(hKey);
				status = RETURN_SUCCESS;
			}
		}
	}

	return status;
}

int LoadUnicodeHelper() {
	int status = RETURN_FAILURE;
	
	#if defined(_WIN32) && !defined(_WIN64)
	if (IsWow64()) {
		ptrPadding = sizeof(void *);
	}
	#endif

	char layoutFile[MAX_PATH];
	if(GetKeyboardLayoutFile(layoutFile, sizeof(layoutFile)) == RETURN_SUCCESS) {
		// Get the path of the system directory.
		char systemDirectory[MAX_PATH];
		if (GetSystemDirectory(systemDirectory, MAX_PATH) != 0) {
			char kbdLayoutFilePath[MAX_PATH];
			snprintf(kbdLayoutFilePath, MAX_PATH, "%s\\%s", systemDirectory, layoutFile);

			kbdLibrary = LoadLibrary(layoutFile);

			KbdLayerDescriptor pKbdLayerDescriptor = (KbdLayerDescriptor) GetProcAddress(kbdLibrary, "KbdLayerDescriptor");

			if(pKbdLayerDescriptor != NULL) {
				PKBDTABLES pKbd = pKbdLayerDescriptor();

				// Store the memory address of the following 3 structures.
				BYTE *base = (BYTE *) pKbd;

				// First element of each structure, no offset adjustment needed.
				pVkToBit = pKbd->pCharModifiers->pVkToBit;
				
				// Second element of pKbd, +4 byte offset on wow64.
				pVkToWcharTable = *((PVK_TO_WCHAR_TABLE *) (base + offsetof(KBDTABLES, pVkToWcharTable) + ptrPadding));
				
				// Third element of pKbd, +8 byte offset on wow64.
				pDeadKey = *((PDEADKEY *) (base + offsetof(KBDTABLES, pDeadKey) + (ptrPadding * 2)));
				
				status = RETURN_SUCCESS;
			}
			else {
				FreeLibrary(kbdLibrary);
				kbdLibrary = NULL;
			}
		}
	}

	return status;
}

// Should probably return a boolean
int UnloadUnicodeHelper() {
	int status = RETURN_FAILURE;

	if(kbdLibrary != NULL && FreeLibrary(kbdLibrary)) {
		kbdLibrary = NULL;
		status = RETURN_SUCCESS;
	}

	return status;
}

int ConvertVirtualKeyToWChar(int virtualKey, PWCHAR outputChar, PWCHAR deadChar) {
	int charCount = 0;
	*outputChar = 0;
	*deadChar = 0;
	
	//Check and make sure the unicode helper was loaded.
	if (kbdLibrary != NULL) {
		short state = 0;
		int shift = -1;
		int mod = 0;

		WCHAR baseChar;
		WCHAR diacritic;

		int capsLock = (GetKeyState(VK_CAPITAL) & 0x01);

		/* Because this is only a structure of two bytes, we don't need to worry 
		* about the structure padding of __ptr64 offsets on Wow64.
		*/
		for (int i = 0; pVkToBit[i].Vk != 0; i++) {
			state = GetAsyncKeyState(pVkToBit[i].Vk);

			if(pVkToBit[i].Vk == VK_SHIFT) {
				shift = i + 1; // Get modification number for Shift key
			}

			if(state & ~SHRT_MAX) {
				if (mod == 0) {
					mod = i + 1;
				}
				else {
					mod = 0; // Two modifiers at the same time!
				}
			}

		}

		// Default 32 bit structure size should be 6 bytes (4 for the pointer and 2
		// additional byte fields) that are padded out to 8 bytes by the compiler.
		unsigned short sizeVkToWcharTable = sizeof(VK_TO_WCHAR_TABLE);
		#if defined(_WIN32) && !defined(_WIN64)
		if (IsWow64()) {
			// If we are running under Wow64 the size of the first pointer will be
			// 8 bringing the total size to 10 bytes padded out to 16.
			sizeVkToWcharTable = (sizeVkToWcharTable + ptrPadding + 7) & -8;
		}
		#endif

		BYTE *ptrCurrentVkToWcharTable = (BYTE *) pVkToWcharTable;

		int cbSize, n;
		do {
			// cbSize is used to calculate n, and n is used for the size of pVkToWchars[j].wch[n]
			cbSize = *(ptrCurrentVkToWcharTable + offsetof(VK_TO_WCHAR_TABLE, cbSize) + ptrPadding);
			n = (cbSize - 2) / 2;

			// Same as VK_TO_WCHARS pVkToWchars[] = pVkToWcharTable[i].pVkToWchars
			PVK_TO_WCHARS pVkToWchars = (PVK_TO_WCHARS) ((PVK_TO_WCHAR_TABLE) ptrCurrentVkToWcharTable)->pVkToWchars;

			if(pVkToWchars != NULL && mod < n) {
				// pVkToWchars[j].VirtualKey
				BYTE *pCurrentVkToWchars = (BYTE *) pVkToWchars;

				do {
					if (((PVK_TO_WCHARS) pCurrentVkToWchars)->VirtualKey == virtualKey) {
						if ((((PVK_TO_WCHARS) pCurrentVkToWchars)->Attributes == CAPLOK) && capsLock) {
							if(mod == shift) mod = 0; else mod = shift;
						}
						*outputChar = ((PVK_TO_WCHARS) pCurrentVkToWchars)->wch[mod];
						charCount = 1;

						// Increment the pCurrentVkToWchars by the size of wch[n].
						pCurrentVkToWchars += sizeof(VK_TO_WCHARS) + (sizeof(WCHAR) * n);

						if(*outputChar == WCH_NONE) {
							charCount = 0;
						}
						else if(*outputChar == WCH_DEAD) {
							*deadChar = ((PVK_TO_WCHARS) pCurrentVkToWchars)->wch[mod];
							charCount = 0;
						}
						break;
					}
					else {
						// Add sizeof WCHAR because we are really an array of WCHAR[n] not WCHAR[]
						pCurrentVkToWchars += sizeof(VK_TO_WCHARS) + (sizeof(WCHAR) * n);
					}
				} while ( ((PVK_TO_WCHARS) pCurrentVkToWchars)->VirtualKey != 0 );
			}

			// This is effectively the same as: ptrCurrentVkToWcharTable = pVkToWcharTable[++i];
			ptrCurrentVkToWcharTable += sizeVkToWcharTable;
		} while (cbSize != 0);


		// Code to check for dead characters...
		if( *deadChar != 0) {
			for (int i = 0; pDeadKey[i].dwBoth != 0; i++) {
				baseChar = (WCHAR) pDeadKey[i].dwBoth;
				diacritic = (WCHAR) (pDeadKey[i].dwBoth >> 16);

				if((baseChar == *outputChar) && (diacritic == *deadChar)) {
					*deadChar = 0;
					*outputChar = (WCHAR) pDeadKey[i].wchComposed;
				}
			}
		}
	}

	return charCount;
}
