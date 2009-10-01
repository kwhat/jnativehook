#include <winuser.h>
#define MOD_RALT		MOD_ALT
#define MOD_RCONTROL	MOD_CONTROL
#define MOD_RSHIFT		MOD_SHIFT
#define MOD_RWIN		MOD_WIN
#define MOD_LALT		MOD_ALT		<< 4
#define MOD_LCONTROL	MOD_CONTROL << 4
#define MOD_LSHIFT		MOD_SHIFT	<< 4
#define MOD_LWIN		MOD_WIN		<< 4

typedef struct {
	unsigned char keycode;
	bool shift_mask;
	bool control_mask;
	bool alt_mask;
	bool meta_mask;
} KeyCode;

typedef struct {
	unsigned char buttoncode;
	bool shift_mask;
	bool control_mask;
	bool alt_mask;
	bool meta_mask;
} ButtonCode;

int grabKey(KeyCode key);
int ungrabKey(KeyCode key);
int grabButton(ButtonCode button);
int ungrabButton(ButtonCode button);
void setModifierMask(unsigned char mod);
void unsetModifierMask(unsigned char mod);
bool isMaskSet(unsigned char mod);
