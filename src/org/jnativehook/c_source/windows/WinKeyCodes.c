#include <winuser.h>

unsigned char modifiers = 0x00;

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
