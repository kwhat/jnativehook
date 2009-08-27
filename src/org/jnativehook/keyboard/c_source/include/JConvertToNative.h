#include <jni.h>

#include "JKeyCodes.h"
#include "JKeyLocations.h"
#include "JKeyModifiers.h"
#include "JKeyEvents.h"

typedef struct {
	jint keycode;		//Key Code
	jint location;		//Key Location
} JKeyCode;


unsigned int JKeycodeToNative(JKeyCode jkey);
JKeyCode NativeToJKeycode(unsigned int keycode);

unsigned int JModifierToNative(jint modifier);
jint NativeToJModifier(unsigned int modifier);
