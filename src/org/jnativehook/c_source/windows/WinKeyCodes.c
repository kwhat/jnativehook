#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>
#include <stdbool.h>
#include <limits.h>
#include "WinKeyCodes.h"

unsigned char modifiers = 0x00;

unsigned short int keysize = 0;
KeyCode * grabkeys = NULL;

unsigned short int buttonsize = 0;
ButtonCode * grabbuttons = NULL;

int grabKey(KeyCode key) {
	if (keysize == USHRT_MAX) {
		//This shouldn't happen but just in case.
		return 1;
	}

	KeyCode * tmp = malloc( (keysize + 1) * sizeof(KeyCode) );

	int i = 0;
	for (; i < keysize; i++) {
		if (grabkeys[i].keycode			== key.keycode &&
			grabkeys[i].shift_mask		== key.shift_mask &&
			grabkeys[i].control_mask	== key.control_mask &&
			grabkeys[i].alt_mask		== key.alt_mask &&
			grabkeys[i].meta_mask		== key.meta_mask
		) {
			//We have a duplicate.
			free(tmp);
			return 0;
		}

		tmp[i] = grabkeys[i];
	}

	free(grabkeys);
	grabkeys = tmp;
	grabkeys[keysize] = key;
	keysize++;

	return 0;
}

int ungrabKey(KeyCode key) {
	KeyCode * tmp = malloc( (keysize - 1) * sizeof(KeyCode) );

	int i = 0, j = 0;
	for (; i < keysize; i++) {
		if (grabkeys[i].keycode			!= key.keycode ||
			grabkeys[i].shift_mask		!= key.shift_mask ||
			grabkeys[i].control_mask	!= key.control_mask ||
			grabkeys[i].alt_mask		!= key.alt_mask ||
			grabkeys[i].meta_mask		!= key.meta_mask
		) {
			tmp[j++] = grabkeys[i];
		}
	}

	free(grabkeys);
	keysize--;
	grabkeys = tmp;

	return 0;
}

int grabButton(ButtonCode button) {
	if (buttonsize == USHRT_MAX) {
		//This shouldn't happen but just in case.
		return 1;
	}

	ButtonCode * tmp = malloc( (buttonsize + 1) * sizeof(ButtonCode) );

	int i = 0;
	for (; i < buttonsize; i++) {
		if (grabbuttons[i].buttoncode		== button.buttoncode &&
			grabbuttons[i].shift_mask		== button.shift_mask &&
			grabbuttons[i].control_mask		== button.control_mask &&
			grabbuttons[i].alt_mask			== button.alt_mask &&
			grabbuttons[i].meta_mask		== button.meta_mask
		) {
			//We have a duplicate.
			free(tmp);
			return 0;
		}

		tmp[i] = grabbuttons[i];
	}

	free(grabbuttons);
	grabbuttons = tmp;
	grabbuttons[buttonsize] = button;
	buttonsize++;

	return 0;
}

int ungrabButton(ButtonCode button) {
	ButtonCode * tmp = malloc( (buttonsize - 1) * sizeof(ButtonCode) );

	int i = 0, j = 0;
	for (; i < buttonsize; i++) {
		if (grabbuttons[i].buttoncode		!= button.buttoncode ||
			grabbuttons[i].shift_mask		!= button.shift_mask ||
			grabbuttons[i].control_mask		!= button.control_mask ||
			grabbuttons[i].alt_mask			!= button.alt_mask ||
			grabbuttons[i].meta_mask		!= button.meta_mask
		) {
			tmp[j++] = grabbuttons[i];
		}
	}

	free(grabbuttons);
	grabbuttons = tmp;
	buttonsize--;

	return 0;
}

void setModifierMask(unsigned char mod) {
	modifiers |= mod;
}

void unsetModifierMask(unsigned char mod) {
	modifiers ^= mod;
}

bool isMaskSet(unsigned char mod) {
	unsigned char left_mask = modifiers >> 4;
	unsigned char right_mask = modifiers & 0x0F;

	return (left_mask | right_mask) & mod;
}
