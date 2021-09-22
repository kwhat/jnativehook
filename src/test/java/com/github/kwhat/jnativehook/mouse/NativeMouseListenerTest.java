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
package com.github.kwhat.jnativehook.mouse;

import com.github.kwhat.jnativehook.mouse.listeners.NativeMouseInputListenerTest;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class NativeMouseListenerTest {
    @Test
    public void testNativeMouseClicked() {
        NativeMouseEvent event = new NativeMouseEvent(
            NativeMouseEvent.NATIVE_MOUSE_CLICKED,
            0x00,
            50,
            75,
            1,
            NativeMouseEvent.BUTTON1);

        NativeMouseInputListenerTest listener = new NativeMouseInputListenerTest();
        listener.nativeMouseClicked(event);

        assertEquals(event, listener.getLastEvent());
    }

    @Test
    public void testNativeMousePressed() {
        System.out.println("nativeMousePressed");

        NativeMouseEvent event = new NativeMouseEvent(
            NativeMouseEvent.NATIVE_MOUSE_PRESSED,
            0x00,
            50,
            75,
            1,
            NativeMouseEvent.BUTTON1);

        NativeMouseInputListenerTest listener = new NativeMouseInputListenerTest();
        listener.nativeMousePressed(event);

        assertEquals(event, listener.getLastEvent());
    }

    @Test
    public void testNativeMouseReleased() {
        NativeMouseEvent event = new NativeMouseEvent(
            NativeMouseEvent.NATIVE_MOUSE_RELEASED,
            0x00,
            50,
            75,
            1,
            NativeMouseEvent.BUTTON1);

        NativeMouseInputListenerTest listener = new NativeMouseInputListenerTest();
        listener.nativeMouseReleased(event);

        assertEquals(event, listener.getLastEvent());
    }
}
