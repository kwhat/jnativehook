#include <stdbool.h>

#define MOD_RALT		MOD_ALT
#define MOD_RCONTROL	MOD_CONTROL
#define MOD_RSHIFT		MOD_SHIFT
#define MOD_RWIN		MOD_WIN
#define MOD_LALT		MOD_ALT		<< 4
#define MOD_LCONTROL	MOD_CONTROL << 4
#define MOD_LSHIFT		MOD_SHIFT	<< 4
#define MOD_LWIN		MOD_WIN		<< 4

void setModifierMask(unsigned char mod);
void unsetModifierMask(unsigned char mod);
bool isModifierMask(unsigned char mod);
