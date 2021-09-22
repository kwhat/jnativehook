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
package com.github.kwhat.jnativehook.keyboard;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class NativeKeyListenerTest {
    @Test
    public void testNativeKeyPressed() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            NativeKeyEvent.SHIFT_MASK,
            0x41,
            NativeKeyEvent.VC_A,
            NativeKeyEvent.CHAR_UNDEFINED,
            NativeKeyEvent.KEY_LOCATION_STANDARD);

        com.github.kwhat.jnativehook.keyboard.listeners.NativeKeyListenerTest listener = new com.github.kwhat.jnativehook.keyboard.listeners.NativeKeyListenerTest();
        listener.nativeKeyPressed(event);

        assertEquals(event, listener.getLastEvent());
    }

    @Test
    public void testNativeKeyReleased() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_RELEASED,
            NativeKeyEvent.SHIFT_MASK,
            0x41,
            NativeKeyEvent.VC_A,
            NativeKeyEvent.CHAR_UNDEFINED,
            NativeKeyEvent.KEY_LOCATION_STANDARD);

        com.github.kwhat.jnativehook.keyboard.listeners.NativeKeyListenerTest listener = new com.github.kwhat.jnativehook.keyboard.listeners.NativeKeyListenerTest();
        listener.nativeKeyReleased(event);

        assertEquals(event, listener.getLastEvent());
    }

    @Test
    public void testNativeKeyTyped() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_TYPED,
            NativeKeyEvent.SHIFT_MASK,
            0x41,
            NativeKeyEvent.VC_UNDEFINED,
            'A',
            NativeKeyEvent.KEY_LOCATION_STANDARD);

        com.github.kwhat.jnativehook.keyboard.listeners.NativeKeyListenerTest listener = new com.github.kwhat.jnativehook.keyboard.listeners.NativeKeyListenerTest();
        listener.nativeKeyTyped(event);

        assertEquals(event, listener.getLastEvent());
    }
}
