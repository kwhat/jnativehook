/* Copyright (c) 2006-2010 - Alexander Barker (alex@1stleg.com)
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "OSXKeyCodes.h"

unsigned int modifiers = 0x0000;

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
	//Start out with tmp array of the same size.
	KeyCode * tmp = malloc( keysize * sizeof(KeyCode) );

	int i = 0, j = 0;
	for (; i < keysize; i++) {
		if (grabkeys[i].keycode			== key.keycode &&
			grabkeys[i].shift_mask		== key.shift_mask &&
			grabkeys[i].control_mask	== key.control_mask &&
			grabkeys[i].alt_mask		== key.alt_mask &&
			grabkeys[i].meta_mask		== key.meta_mask
		) {
			//Found what we want to remove, so shorten the array and skip the item
			tmp = (KeyCode *) realloc( tmp, (keysize - 1) * sizeof(KeyCode) );
			keysize--;
		}
		else {
			//Not the key in question.
			tmp[j++] = grabkeys[i];
		}
	}

	free(grabkeys);
	grabkeys = tmp;

	return 0;
}

bool isKeyGrabbed(KeyCode key) {
	int i = 0;
	for (; i < keysize; i++) {
		if (grabkeys[i].keycode			== key.keycode &&
			grabkeys[i].shift_mask		== key.shift_mask &&
			grabkeys[i].control_mask	== key.control_mask &&
			grabkeys[i].alt_mask		== key.alt_mask &&
			grabkeys[i].meta_mask		== key.meta_mask
		) {
			return true;
		}
	}

	return false;
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
	ButtonCode * tmp = malloc( buttonsize * sizeof(ButtonCode) );

	int i = 0, j = 0;
	for (; i < buttonsize; i++) {
		if (grabbuttons[i].buttoncode		== button.buttoncode &&
			grabbuttons[i].shift_mask		== button.shift_mask &&
			grabbuttons[i].control_mask		== button.control_mask &&
			grabbuttons[i].alt_mask			== button.alt_mask &&
			grabbuttons[i].meta_mask		== button.meta_mask
		) {
			//Found what we want to remove, so shorten the array and skip the item
			tmp = (ButtonCode *) realloc( tmp, (buttonsize - 1) * sizeof(ButtonCode) );
			buttonsize--;
		}
		else {
			tmp[j++] = grabbuttons[i];
		}
	}

	free(grabbuttons);
	grabbuttons = tmp;

	return 0;
}

bool isButtonGrabbed(ButtonCode button) {
	int i = 0;
	for (; i < buttonsize; i++) {
		if (grabbuttons[i].buttoncode		== button.buttoncode &&
			grabbuttons[i].shift_mask		== button.shift_mask &&
			grabbuttons[i].control_mask		== button.control_mask &&
			grabbuttons[i].alt_mask			== button.alt_mask &&
			grabbuttons[i].meta_mask		== button.meta_mask
		) {
			return true;
		}
	}

	return false;
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
