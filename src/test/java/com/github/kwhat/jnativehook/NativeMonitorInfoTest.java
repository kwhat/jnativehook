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
package com.github.kwhat.jnativehook;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class NativeMonitorInfoTest {
    private short number;
    private short x;
    private short y;
    private short width;
    private short height;

    private NativeMonitorInfo subject;

    @BeforeEach
    public void initialize() {
        number = 2;
        x = 100;
        y = 200;
        width = 1024;
        height = 768;

        subject = new NativeMonitorInfo(number, x, y, width, height);
    }

    @Test
    public void testConstructor() {
        assertEquals(number, subject.getNumber());
        assertEquals(x, subject.getX());
        assertEquals(y, subject.getY());
        assertEquals(width, subject.getWidth());
        assertEquals(height, subject.getHeight());
    }

    @Test
    public void testNumber() {
        number = 1;
        subject.setNumber(number);

        assertEquals(number, subject.getNumber());
    }

    @Test
    public void testX() {
        short x = 500;
        subject.setX(x);

        assertEquals(x, subject.getX());
    }

    @Test
    public void testY() {
        y = 500;
        subject.setY(y);

        assertEquals(y, subject.getY());
    }

    @Test
    public void testWidth() {
        width = 1280;
        subject.setWidth(width);

        assertEquals(width, subject.getWidth());
    }

    @Test
    public void testHeight() {
        height = 1280;
        subject.setHeight(height);

        assertEquals(height, subject.getHeight());
    }
}
