#include <w32api.h>
#define WINVER Windows2000
#define _WIN32_WINNT WINVER
#include <windows.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "WinKeyCodes.h"

unsigned char modifiers = 0x00;


void setModifierMask(unsigned char mod) {
	modifiers |= mod;
}

void unsetModifierMask(unsigned char mod) {
	modifiers ^= mod;
}

bool isModifierMask(unsigned char mod) {
	unsigned char left_mask = modifiers >> 4;
	unsigned char right_mask = modifiers & 0x0F;

	return (left_mask | right_mask) & mod;
}
