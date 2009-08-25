#include <jni.h>

#include "JKeyCodes.h"
#include "JKeyLocations.h"
#include "JKeyModifiers.h"

unsigned int JKeycodeToNative(jint key, jint location);
unsigned int JModifierToNative(jint modifier);
