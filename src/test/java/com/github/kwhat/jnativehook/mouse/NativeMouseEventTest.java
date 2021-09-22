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

import org.junit.jupiter.api.Test;
import java.awt.Point;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;

public class NativeMouseEventTest {
    @Test
    public void testGetButton() {
        NativeMouseEvent event = new NativeMouseEvent(
            NativeMouseEvent.NATIVE_MOUSE_PRESSED,
            0x00,
            50,
            75,
            1,
            NativeMouseEvent.BUTTON1);

        assertEquals(NativeMouseEvent.BUTTON1, event.getButton());
    }

    @Test
    public void testGetClickCount() {
        NativeMouseEvent event = new NativeMouseEvent(
            NativeMouseEvent.NATIVE_MOUSE_PRESSED,
            0x00,
            50,
            75,
            1,
            NativeMouseEvent.BUTTON1);

        assertEquals(1, event.getClickCount());
    }

    @Test
    public void testGetPoint() {
        NativeMouseEvent event = new NativeMouseEvent(
            NativeMouseEvent.NATIVE_MOUSE_PRESSED,
            0x00,
            50,
            75,
            1,
            NativeMouseEvent.BUTTON1);

        assertEquals(new Point(50, 75), event.getPoint());
    }

    @Test
    public void testGetX() {
        NativeMouseEvent event = new NativeMouseEvent(
            NativeMouseEvent.NATIVE_MOUSE_PRESSED,
            0x00,
            50,
            75,
            1,
            NativeMouseEvent.BUTTON1);

        assertEquals(50, event.getX());
    }

    @Test
    public void testGetY() {
        NativeMouseEvent event = new NativeMouseEvent(
            NativeMouseEvent.NATIVE_MOUSE_PRESSED,
            0x00,
            50,
            75,
            1,
            NativeMouseEvent.BUTTON1);

        assertEquals(75, event.getY());
    }

    @Test
    public void testParamString() {
        NativeMouseEvent event = new NativeMouseEvent(
            NativeMouseEvent.NATIVE_MOUSE_PRESSED,
            0x00,
            50,
            75,
            1,
            NativeMouseEvent.BUTTON1);

        assertNotEquals("", event.paramString());
    }
}
