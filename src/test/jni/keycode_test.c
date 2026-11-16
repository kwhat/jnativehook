/* Conversion regression checks use public Java keycode values. */
#include <assert.h>
#include "jni_Converter.h"

static void check(uint16_t native, uint16_t java, jint location) {
    uint16_t code = native;
    jint actual_location;
    assert(jni_ConvertToJavaLocation(&code, &actual_location) == JNI_OK);
    assert(code == java && actual_location == location);
    assert(jni_ConvertToNativeKeycode(java, location, &code) == JNI_OK);
    assert(code == native);
}
int main(void) {
    check(VC_KP_8, 0x0009, 4);
    check(VC_KP_MULTIPLY, 0xEE6A, 4);
    check(VC_KP_ADD, 0xEE6B, 4);
    check(VC_KP_EQUALS, 0xEE7C, 4);
    check(VC_KP_SEPARATOR, 0xEE6C, 4);
    check(VC_A, 0x001E, 1);
    check(VC_F1, 0x003B, 1);
    check(VC_SHIFT_L, 0x002A, 2);
    check(VC_SHIFT_R, 0x002A, 3);
    check(VC_META_R, 0x0E5B, 3);
    check(VC_KP_1, 0x0002, 4);
    check(VC_KP_ENTER, 0x001C, 4);
    check(VC_KP_DOWN, 0xE050, 4);
    check(VC_KP_END, 0x0E4F, 4);
    check(VC_KP_DECIMAL, 0x0053, 4);
    check(VC_VOLUME_UP, 0xE030, 1);
    uint16_t code = 0xDEAD;
    jint location;
    assert(jni_ConvertToJavaLocation(&code, &location) == JNI_OK);
    assert(code == 0 && location == 0);
    assert(jni_ConvertToNativeKeycode(0xDEAD, 1, &code) == JNI_ERR);
    return 0;
}
