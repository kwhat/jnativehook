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

#include "WinUnicodeHelper.h"
#include "NativeGlobals.h"

typedef PKBDTABLES(CALLBACK *KbdLayerDescriptor) (VOID);

// These are pointers to arrays of their respective structures.
PVK_TO_WCHARS1 pVkToWchars1 = NULL;
PVK_TO_WCHARS2 pVkToWchars2 = NULL;
PVK_TO_WCHARS3 pVkToWchars3 = NULL;
PVK_TO_WCHARS4 pVkToWchars4 = NULL;
PVK_TO_WCHARS5 pVkToWchars5 = NULL;
PVK_TO_WCHARS6 pVkToWchars6 = NULL;
PVK_TO_WCHARS7 pVkToWchars7 = NULL;
PVK_TO_WCHARS8 pVkToWchars8 = NULL;
PVK_TO_WCHARS9 pVkToWchars9 = NULL;
PVK_TO_WCHARS10 pVkToWchars10 = NULL;

PMODIFIERS pCharModifiers;
PDEADKEY pDeadKey;

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

HINSTANCE LoadInputHelper() {
	PKBDTABLES pKbd;
	HINSTANCE kbdLibrary = NULL;

	char layoutFile[MAX_PATH];
	if(GetKeyboardLayoutFile(layoutFile, sizeof(layoutFile)) == RETURN_SUCCESS) {
		// Get the path of the system directory.
		char systemDirectory[MAX_PATH];
		if (GetSystemDirectory(systemDirectory, MAX_PATH) != 0) {
			char kbdLayoutFilePath[MAX_PATH];
			snprintf(kbdLayoutFilePath, MAX_PATH, "%s\\%s", systemDirectory, layoutFile);
			
			kbdLibrary = LoadLibrary(kbdLayoutFilePath);

			KbdLayerDescriptor pKbdLayerDescriptor = (KbdLayerDescriptor) GetProcAddress(kbdLibrary, "KbdLayerDescriptor");

			if(pKbdLayerDescriptor != NULL) {
				pKbd = pKbdLayerDescriptor();
				
				for (int i = 0; pKbd->pVkToWcharTable[i].cbSize != 0; i++) {
					// Size should be 1 - 10
					switch ((pKbd->pVkToWcharTable[i].cbSize - 2) / 2) {
						INIT_PVK_TO_WCHARS(i, 1)
						INIT_PVK_TO_WCHARS(i, 2)
						INIT_PVK_TO_WCHARS(i, 3)
						INIT_PVK_TO_WCHARS(i, 4)
						INIT_PVK_TO_WCHARS(i, 5)
						INIT_PVK_TO_WCHARS(i, 6)
						INIT_PVK_TO_WCHARS(i, 7)
						INIT_PVK_TO_WCHARS(i, 8)
						INIT_PVK_TO_WCHARS(i, 9)
						INIT_PVK_TO_WCHARS(i, 10)
					}
				}

				pCharModifiers = pKbd->pCharModifiers;
				pDeadKey = pKbd->pDeadKey;
			}
			else {
				FreeLibrary(kbdLibrary);
				kbdLibrary = NULL;
			}
		}
	}
	
	return kbdLibrary;
}

// Should probably return a boolean
int UnloadInputHelper(HINSTANCE kbdLibrary) {
	int status = RETURN_FAILURE;
	
	if(kbdLibrary != 0 && FreeLibrary(kbdLibrary) != 0) {
		status = RETURN_SUCCESS;
	}
	
	return status;
}

int ConvertVirtualKeyToWChar(int virtualKey, PWCHAR outputChar, PWCHAR deadChar) {
	int i = 0;
	short state = 0;
	int shift = -1;
	int mod = 0;
	int charCount = 0;
	
	WCHAR baseChar;
	WCHAR diacritic;
	*outputChar = 0;

	int capsLock = (GetKeyState(VK_CAPITAL) & 0x01);
	
	for (i = 0; pCharModifiers->pVkToBit[i].Vk != 0; i++) {
		state = GetAsyncKeyState(pCharModifiers->pVkToBit[i].Vk);
		

		if(pCharModifiers->pVkToBit[i].Vk == VK_SHIFT) {
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

	
	// There really is no way around this.
	SEARCH_VK_IN_CONVERSION_TABLE(1)
	SEARCH_VK_IN_CONVERSION_TABLE(2)
	SEARCH_VK_IN_CONVERSION_TABLE(3)
	SEARCH_VK_IN_CONVERSION_TABLE(4)
	SEARCH_VK_IN_CONVERSION_TABLE(5)
	SEARCH_VK_IN_CONVERSION_TABLE(6)
	SEARCH_VK_IN_CONVERSION_TABLE(7)
	SEARCH_VK_IN_CONVERSION_TABLE(8)
	SEARCH_VK_IN_CONVERSION_TABLE(9)
	SEARCH_VK_IN_CONVERSION_TABLE(10)


	// I see dead characters...
	if( *deadChar != 0) {
		for (i = 0; pDeadKey[i].dwBoth != 0; i++) {
			baseChar = (WCHAR) pDeadKey[i].dwBoth;
			diacritic = (WCHAR) (pDeadKey[i].dwBoth >> 16);
			
			if((baseChar == *outputChar) && (diacritic == *deadChar)) {
				*deadChar = 0;
				*outputChar = (WCHAR)pDeadKey[i].wchComposed;
			}
		}
	}

	return charCount;
}
