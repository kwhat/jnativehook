/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2021 Alexander Barker.  All Rights Reserved.
 * https://github.com/kwhat/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <jni.h>
#include <uiohook.h>

#include "com_github_kwhat_jnativehook_NativeInputEvent.h"
#include "com_github_kwhat_jnativehook_keyboard_NativeKeyEvent.h"
#include "com_github_kwhat_jnativehook_mouse_NativeMouseEvent.h"
#include "com_github_kwhat_jnativehook_mouse_NativeMouseWheelEvent.h"
#include "jni_Converter.h"


static const struct { event_type native; jint java; } eventTypes[] = {
    { EVENT_KEY_TYPED,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_TYPED     },
    { EVENT_KEY_PRESSED,    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_PRESSED   },
    { EVENT_KEY_RELEASED,   com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_NATIVE_KEY_RELEASED  },
    { EVENT_MOUSE_CLICKED,  com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_CLICKED  },
    { EVENT_MOUSE_PRESSED,  com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_PRESSED  },
    { EVENT_MOUSE_RELEASED, com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_RELEASED },
    { EVENT_MOUSE_MOVED,    com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_MOVED    },
    { EVENT_MOUSE_DRAGGED,  com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_DRAGGED  },
    { EVENT_MOUSE_WHEEL,    com_github_kwhat_jnativehook_mouse_NativeMouseEvent_NATIVE_MOUSE_WHEEL    },
};

/* Keep the public Java keycodes stable across libuiohook's keycode revision.
 * Location distinguishes keypad and left/right variants in Java. */
static const struct { uint16_t native; jint java; jint location; } keycodes[] = {
    { VC_ESCAPE,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_ESCAPE,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F1,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F1,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F2,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F2,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F3,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F3,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F4,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F4,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F5,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F5,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F6,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F6,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F7,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F7,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F8,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F8,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F9,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F9,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F10,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F10,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F11,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F11,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F12,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F12,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F13,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F13,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F14,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F14,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F15,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F15,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F16,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F16,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F17,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F17,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F18,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F18,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F19,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F19,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F20,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F20,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F21,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F21,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F22,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F22,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F23,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F23,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F24,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F24,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BACKQUOTE,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_BACKQUOTE,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_1,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_1,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_2,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_2,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_3,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_3,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_4,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_4,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_5,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_5,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_6,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_6,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_7,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_7,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_8,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_8,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_9,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_9,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_0,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_0,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_MINUS,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_MINUS,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_EQUALS,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_EQUALS,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BACKSPACE,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_BACKSPACE,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_TAB,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_TAB,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_CAPS_LOCK,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_CAPS_LOCK,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_A,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_A,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_B,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_B,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_C,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_C,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_D,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_D,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_E,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_E,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_F,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_F,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_G,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_G,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_H,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_H,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_I,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_I,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_J,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_J,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_K,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_K,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_L,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_L,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_M,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_M,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_N,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_N,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_O,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_O,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_P,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_P,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_Q,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_Q,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_R,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_R,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_S,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_S,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_T,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_T,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_U,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_U,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_V,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_V,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_W,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_W,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_X,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_X,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_Y,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_Y,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_Z,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_Z,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_OPEN_BRACKET,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_OPEN_BRACKET,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_CLOSE_BRACKET,     com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_CLOSE_BRACKET,     com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BACK_SLASH,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_BACK_SLASH,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SEMICOLON,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SEMICOLON,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_QUOTE,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_QUOTE,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_ENTER,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_ENTER,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_COMMA,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_COMMA,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_PERIOD,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_PERIOD,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SLASH,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SLASH,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SPACE,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SPACE,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_PRINTSCREEN,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_PRINTSCREEN,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SCROLL_LOCK,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SCROLL_LOCK,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_PAUSE,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_PAUSE,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_INSERT,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_INSERT,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_DELETE,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_DELETE,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_HOME,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_HOME,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_END,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_END,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_PAGE_UP,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_PAGE_UP,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_PAGE_DOWN,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_PAGE_DOWN,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_UP,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_UP,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_LEFT,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_LEFT,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BEGIN,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_CLEAR,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_RIGHT,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_RIGHT,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_DOWN,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_DOWN,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_NUM_LOCK,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_NUM_LOCK,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_DECIMAL,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_PERIOD,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_SHIFT_L,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SHIFT,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT     },
    { VC_CONTROL_L,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_CONTROL,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT     },
    { VC_ALT_L,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_ALT,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT     },
    { VC_META_L,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_META,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_LEFT     },
    { VC_CONTEXT_MENU,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_CONTEXT_MENU,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_POWER,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_POWER,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SLEEP,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SLEEP,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_WAKE,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_WAKE,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_MEDIA_PLAY,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_MEDIA_PLAY,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_MEDIA_STOP,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_MEDIA_STOP,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_MEDIA_PREVIOUS,    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_MEDIA_PREVIOUS,    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_MEDIA_NEXT,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_MEDIA_NEXT,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_MEDIA_SELECT,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_MEDIA_SELECT,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_MEDIA_EJECT,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_MEDIA_EJECT,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_VOLUME_MUTE,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_VOLUME_MUTE,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_VOLUME_UP,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_VOLUME_UP,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_VOLUME_DOWN,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_VOLUME_DOWN,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_APP_MAIL,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_APP_MAIL,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_APP_CALCULATOR,    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_APP_CALCULATOR,    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_APP_MUSIC,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_APP_MUSIC,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_APP_PICTURES,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_APP_PICTURES,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BROWSER_SEARCH,    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_BROWSER_SEARCH,    com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BROWSER_HOME,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_BROWSER_HOME,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BROWSER_BACK,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_BROWSER_BACK,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BROWSER_FORWARD,   com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_BROWSER_FORWARD,   com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BROWSER_STOP,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_BROWSER_STOP,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BROWSER_REFRESH,   com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_BROWSER_REFRESH,   com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_BROWSER_FAVORITES, com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_BROWSER_FAVORITES, com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_KATAKANA,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_KATAKANA,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_UNDERSCORE,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_UNDERSCORE,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_KANA,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_FURIGANA,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_KANJI,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_KANJI,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_HIRAGANA,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_HIRAGANA,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_HELP,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_HELP,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_STOP,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_STOP,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_PROPS,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_PROPS,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_FRONT,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_FRONT,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_OPEN,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_OPEN,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_FIND,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_FIND,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_AGAIN,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_AGAIN,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_UNDO,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_UNDO,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_COPY,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_COPY,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_INSERT,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_INSERT,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SUN_CUT,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SUN_CUT,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_STANDARD },
    { VC_SHIFT_R,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SHIFT,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT    },
    { VC_CONTROL_R,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_CONTROL,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT    },
    { VC_ALT_R,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_ALT,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT    },
    { VC_META_R,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_META,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_RIGHT    },
    { VC_KP_0,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_0,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_1,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_1,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_2,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_2,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_3,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_3,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_4,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_4,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_5,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_5,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_6,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_6,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_7,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_7,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_8,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_8,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_9,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_9,                 com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_ENTER,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_ENTER,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_COMMA,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_COMMA,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_END,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_END,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_DOWN,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_DOWN,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_PAGE_DOWN,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_PAGE_DOWN,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_LEFT,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_LEFT,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_RIGHT,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_RIGHT,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_HOME,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_HOME,              com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_UP,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_UP,                com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_PAGE_UP,        com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_PAGE_UP,           com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_INSERT,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_INSERT,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_DELETE,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_DELETE,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_MULTIPLY,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_MULTIPLY,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_ADD,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_ADD,               com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_SUBTRACT,       com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_MINUS,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_DIVIDE,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SLASH,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_CLEAR,          com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_CLEAR,             com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_EQUALS,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_EQUALS,            com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
    { VC_KP_SEPARATOR,      com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_SEPARATOR,         com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_NUMPAD   },
};


jint jni_ConvertToJavaType(event_type nativeType, jint *javaType) {
    if (javaType == NULL) {
        return JNI_ERR;
    }

    for (size_t i = 0; i < sizeof(eventTypes) / sizeof(eventTypes[0]); i++) {
        if (eventTypes[i].native == nativeType) {
            *javaType = eventTypes[i].java;
            return JNI_OK;
        }
    }

    *javaType = 0;
    return JNI_ERR;
}

jint jni_ConvertToNativeType(jint javaType, event_type *nativeType) {
    if (nativeType == NULL) {
        return JNI_ERR;
    }

    for (size_t i = 0; i < sizeof(eventTypes) / sizeof(eventTypes[0]); i++) {
        if (eventTypes[i].java == javaType) {
            *nativeType = eventTypes[i].native;
            return JNI_OK;
        }
    }

    *nativeType = 0;
    return JNI_ERR;
}

jint jni_ConvertToJavaLocation(unsigned short *nativeKeyCode, jint *javaKeyLocation) {
    if (nativeKeyCode == NULL || javaKeyLocation == NULL) {
        return JNI_ERR;
    }

    for (size_t i = 0; i < sizeof(keycodes) / sizeof(keycodes[0]); i++) {
        if (keycodes[i].native == *nativeKeyCode) {
            *nativeKeyCode = (uint16_t) keycodes[i].java;
            *javaKeyLocation = keycodes[i].location;
            return JNI_OK;
        }
    }

    *nativeKeyCode = com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_VC_UNDEFINED;
    *javaKeyLocation = com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_UNKNOWN;
    return JNI_OK;
}

jint jni_ConvertToNativeKeycode(jint javaKeyCode, jint javaKeyLocation, uint16_t *nativeKeyCode) {
    if (nativeKeyCode == NULL) {
        return JNI_ERR;
    }

    for (size_t i = 0; i < sizeof(keycodes) / sizeof(keycodes[0]); i++) {
        if (keycodes[i].java == javaKeyCode && (keycodes[i].location == javaKeyLocation ||
                (javaKeyLocation == com_github_kwhat_jnativehook_keyboard_NativeKeyEvent_LOCATION_UNKNOWN))) {
            *nativeKeyCode = keycodes[i].native;
            return JNI_OK;
        }
    }

    *nativeKeyCode = VC_UNDEFINED;
    return JNI_ERR;
}
