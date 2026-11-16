/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2026 Alexander Barker.  All Rights Reserved.
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
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;

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
    public void testConstructorPreservesButtonModifiers() {
        int mask = NativeInputEvent.BUTTON1_MASK
            | NativeInputEvent.BUTTON2_MASK
            | NativeInputEvent.BUTTON3_MASK
            | NativeInputEvent.BUTTON4_MASK
            | NativeInputEvent.BUTTON5_MASK;

        NativeInputEvent event = new NativeInputEvent(
            GlobalScreen.class,
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            mask);

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

    @Test
    public void consumePreservesModifiersAndIsIdempotent() {
        int mask = NativeInputEvent.SHIFT_R_MASK | NativeInputEvent.BUTTON5_MASK
            | NativeInputEvent.CAPS_LOCK_MASK | NativeInputEvent.EMULATED_MASK;
        NativeInputEvent event = new NativeInputEvent(GlobalScreen.class, NativeKeyEvent.NATIVE_KEY_PRESSED, mask);
        assertFalse(event.isConsumed());
        assertTrue(event.isEmulated());
        event.consume();
        event.consume();
        assertEquals(mask | NativeInputEvent.CONSUMED_MASK, event.getModifiers());
        assertTrue(event.isConsumed());
        assertTrue(event.isEmulated());
    }

    @Test
    public void flagsAreIndependent() {
        NativeInputEvent plain = new NativeInputEvent(GlobalScreen.class, 0, 0);
        assertFalse(plain.isConsumed());
        assertFalse(plain.isEmulated());
        NativeInputEvent consumed = new NativeInputEvent(GlobalScreen.class, 0, NativeInputEvent.CONSUMED_MASK);
        assertTrue(consumed.isConsumed());
        assertFalse(consumed.isEmulated());
    }

    @Test
    public void serializationPreservesEventFlags() throws Exception {
        NativeInputEvent original = new NativeInputEvent(GlobalScreen.class, NativeKeyEvent.NATIVE_KEY_PRESSED,
            NativeInputEvent.EMULATED_MASK | NativeInputEvent.CTRL_L_MASK);
        original.consume();
        java.io.ByteArrayOutputStream bytes = new java.io.ByteArrayOutputStream();
        try (java.io.ObjectOutputStream output = new java.io.ObjectOutputStream(bytes)) {
            output.writeObject(original);
        }
        try (java.io.ObjectInputStream input = new java.io.ObjectInputStream(
                new java.io.ByteArrayInputStream(bytes.toByteArray()))) {
            NativeInputEvent restored = (NativeInputEvent) input.readObject();
            assertEquals(original.getID(), restored.getID());
            assertEquals(original.getModifiers(), restored.getModifiers());
            assertEquals(original.getWhen(), restored.getWhen());
            assertNull(restored.getSource(), "EventObject source is transient");
        }
    }

}
