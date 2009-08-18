#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Sunkeysym.h>

#include <jni.h>

#include "JKeyCodes.h"
#include "JKeyLocations.h"
#include "JKeyModifiers.h"

KeySym JKeycodeToXKeysym(jint key);
