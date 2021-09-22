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

import com.github.kwhat.jnativehook.keyboard.NativeKeyEvent;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;

public class NativeInputEventTest {
    @Test
    public void testGetID() {
        NativeInputEvent event = new NativeInputEvent(
            GlobalScreen.class,
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00);

        assertEquals(event.getID(), NativeKeyEvent.NATIVE_KEY_PRESSED);
    }

    @Test
    public void testGetWhen() {
        long when = 0;

        NativeInputEvent event = new NativeInputEvent(
            GlobalScreen.class,
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00);

        assertEquals(event.getWhen(), when);
    }

    @Test
    public void testGetModifiers() {
        int mask = NativeInputEvent.ALT_MASK
            | NativeInputEvent.CTRL_MASK
            | NativeInputEvent.META_MASK
            | NativeInputEvent.SHIFT_MASK;

        NativeInputEvent event = new NativeInputEvent(
            GlobalScreen.class,
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            mask);

        assertEquals(event.getModifiers(), mask);
    }

    @Test
    public void testSetModifiers() {
        int mask = NativeInputEvent.BUTTON1_MASK
            | NativeInputEvent.BUTTON2_MASK
            | NativeInputEvent.BUTTON3_MASK
            | NativeInputEvent.BUTTON4_MASK
            | NativeInputEvent.BUTTON5_MASK;

        NativeInputEvent event = new NativeInputEvent(
            GlobalScreen.class,
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00);

        event.setModifiers(mask);
        assertEquals(event.getModifiers(), mask);
    }

    @Test
    public void testGetModifiersText() {
        int mask = NativeInputEvent.ALT_MASK
            | NativeInputEvent.BUTTON1_MASK;

        assertNotEquals("", NativeInputEvent.getModifiersText(mask));
    }

    @Test
    public void testParamString() {
        NativeInputEvent event = new NativeInputEvent(
            GlobalScreen.class,
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            NativeInputEvent.SHIFT_MASK
                | NativeInputEvent.BUTTON5_MASK);

        assertNotEquals("", event.paramString());
    }
}
