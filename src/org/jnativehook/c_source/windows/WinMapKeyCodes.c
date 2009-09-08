#ifdef DEBUG
#include <stdio.h>
#include <unistd.h>
#endif

#include <windows.h>

unsigned int AltMask = 0;
unsigned int SuperMask = 0;
unsigned int MetaMask = 0;

unsigned int getAltMask() {
	return AltMask;
}

unsigned int getSuperMask() {
	return SuperMask;
}

unsigned int getMetaMask() {
	return MetaMask;
}

void getModifiers(Display * disp) {
	XModifierKeymap * modmap;
	static int mask_table[8] = {
		ShiftMask, LockMask, ControlMask, Mod1Mask,
		Mod2Mask, Mod3Mask, Mod4Mask, Mod5Mask
	};

	KeyCode numlock = XKeysymToKeycode(disp, XK_Num_Lock);
	KeyCode scrlock = XKeysymToKeycode(disp, XK_Scroll_Lock);

	KeyCode altlkey = XKeysymToKeycode(disp, XK_Alt_L);
	KeyCode altrkey = XKeysymToKeycode(disp, XK_Alt_R);

	KeyCode suplkey = XKeysymToKeycode(disp, XK_Super_L);
	KeyCode suprkey = XKeysymToKeycode(disp, XK_Super_R);

	KeyCode metalkey = XKeysymToKeycode(disp, XK_Meta_L);
	KeyCode metarkey = XKeysymToKeycode(disp, XK_Meta_R);

	// Find out the masks for the NumLock and ScrollLock modifiers,
	// so that we can bind the grabs for when they are enabled too.
	modmap = XGetModifierMapping(disp);

	if (modmap != NULL && modmap->max_keypermod > 0) {
		int i;
		for (i = 0; i < 8 * modmap->max_keypermod; i++) {
			if (modmap->modifiermap[i] != 0) {
				if (modmap->modifiermap[i] == numlock && NumberLockMask == 0) {
					#ifdef DEBUG
					printf("Native: Found NumberLockMask (%d)\n", mask_table[i / modmap->max_keypermod]);
					#endif
					NumberLockMask = mask_table[i / modmap->max_keypermod];
				}
				else if (modmap->modifiermap[i] == scrlock && ScrollLockMask == 0) {
					#ifdef DEBUG
					printf("Native: Found ScrollLockMask (%d)\n", mask_table[i / modmap->max_keypermod]);
					#endif
					ScrollLockMask = mask_table[i / modmap->max_keypermod];
				}
				else if (modmap->modifiermap[i] == altlkey && AltMask == 0) {
					#ifdef DEBUG
					printf("Native: Found AltMask Left (%d)\n", mask_table[i / modmap->max_keypermod]);
					#endif
					AltMask = mask_table[i / modmap->max_keypermod];
				}
				else if (modmap->modifiermap[i] == altrkey && AltMask == 0) {
					#ifdef DEBUG
					printf("Native: Found AltMask Right (%d)\n", mask_table[i / modmap->max_keypermod]);
					#endif
					AltMask = mask_table[i / modmap->max_keypermod];
				}
				else if (modmap->modifiermap[i] == suplkey && SuperMask == 0) {
					#ifdef DEBUG
					printf("Native: Found SuperMask Left (%d)\n", mask_table[i / modmap->max_keypermod]);
					#endif
					SuperMask = mask_table[i / modmap->max_keypermod];
				}
				else if (modmap->modifiermap[i] == suprkey && SuperMask == 0) {
					#ifdef DEBUG
					printf("Native: Found SuperMask Right (%d)\n", mask_table[i / modmap->max_keypermod]);
					#endif
					SuperMask = mask_table[i / modmap->max_keypermod];
				}
				else if (modmap->modifiermap[i] == metalkey && MetaMask == 0) {
					#ifdef DEBUG
					printf("Found MetaMask Left (%d)\n", mask_table[i / modmap->max_keypermod]);
					#endif
					MetaMask = mask_table[i / modmap->max_keypermod];
				}
				else if (modmap->modifiermap[i] == metarkey && MetaMask == 0) {
					#ifdef DEBUG
					printf("Native: Found MetaMask Right (%d)\n", mask_table[i / modmap->max_keypermod]);
					#endif
					MetaMask = mask_table[i / modmap->max_keypermod];
				}
			}
		}
	}

	CapsLockMask = LockMask;

	if (modmap) {
		XFreeModifiermap(modmap);
	}
}
