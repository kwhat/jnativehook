#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Sunkeysym.h>

#include <jni.h>

#include "include/JKeyCodes.h"
#include "include/JKeyLocations.h"
#include "include/JKeyModifiers.h"

KeySym JKeycodeToXKeysym(jint key);
