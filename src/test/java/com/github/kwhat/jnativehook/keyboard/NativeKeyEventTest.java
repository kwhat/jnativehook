/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2018 Alexander Barker.  All Rights Received.
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
import java.awt.event.KeyEvent;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Arrays;
import java.util.HashMap;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class NativeKeyEventTest {
    @Test
    public void testGetRawCode() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00,
            0x41,
            NativeKeyEvent.VC_UNDEFINED,
            NativeKeyEvent.CHAR_UNDEFINED,
            NativeKeyEvent.KEY_LOCATION_UNKNOWN);

        assertEquals(0x41, event.getRawCode());
    }

    @Test
    public void testSetRawCode() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00,
            0x00,
            NativeKeyEvent.VC_UNDEFINED,
            NativeKeyEvent.CHAR_UNDEFINED,
            NativeKeyEvent.KEY_LOCATION_UNKNOWN);

        event.setRawCode(0x41);
        assertEquals(0x41, event.getRawCode());
    }

    @Test
    public void testGetKeyCode() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00,
            0x00,
            NativeKeyEvent.VC_A,
            NativeKeyEvent.CHAR_UNDEFINED,
            NativeKeyEvent.KEY_LOCATION_UNKNOWN);

        assertEquals(NativeKeyEvent.VC_A, event.getKeyCode());
    }

    @Test
    public void testSetKeyCode() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00,
            0x00,
            NativeKeyEvent.VC_UNDEFINED,
            NativeKeyEvent.CHAR_UNDEFINED,
            NativeKeyEvent.KEY_LOCATION_UNKNOWN);

        event.setKeyCode(NativeKeyEvent.VC_A);
        assertEquals(NativeKeyEvent.VC_A, event.getKeyCode());
    }

    @Test
    public void testGetKeyChar() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00,
            0x00,
            NativeKeyEvent.VC_UNDEFINED,
            'A',
            NativeKeyEvent.KEY_LOCATION_UNKNOWN);

        assertEquals('A', event.getKeyChar());
    }

    @Test
    public void testSetKeyChar() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00,
            0x00,
            NativeKeyEvent.VC_UNDEFINED,
            NativeKeyEvent.CHAR_UNDEFINED,
            NativeKeyEvent.KEY_LOCATION_UNKNOWN);

        event.setKeyChar('A');
        assertEquals('A', event.getKeyChar());
    }

    @Test
    public void testGetKeyLocation() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00,
            0x00,
            NativeKeyEvent.VC_UNDEFINED,
            NativeKeyEvent.CHAR_UNDEFINED,
            NativeKeyEvent.KEY_LOCATION_STANDARD);

        assertEquals(NativeKeyEvent.KEY_LOCATION_STANDARD, event.getKeyLocation());
    }

    @Test
    public void testGetKeyText() {
        assertNotEquals("", NativeKeyEvent.getKeyText(NativeKeyEvent.VC_A));
    }

    @Test
    public void testIsActionKey() {
        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00,
            0x00,
            NativeKeyEvent.VC_F1,
            NativeKeyEvent.CHAR_UNDEFINED,
            NativeKeyEvent.KEY_LOCATION_UNKNOWN);

        assertTrue(event.isActionKey());

        event.setKeyCode(NativeKeyEvent.VC_A);
        assertFalse(event.isActionKey());
    }

    @Test
    public void testParamString() {
        System.out.println("paramString");

        NativeKeyEvent event = new NativeKeyEvent(
            NativeKeyEvent.NATIVE_KEY_PRESSED,
            0x00,
            0x00,
            NativeKeyEvent.VC_UNDEFINED,
            NativeKeyEvent.CHAR_UNDEFINED,
            NativeKeyEvent.KEY_LOCATION_UNKNOWN);

        assertNotEquals("", event.paramString());
    }

    @Test
    public void testMissingConstants() throws Exception {
        // Populate all the virtual key codes from NativeKeyEvent
        HashMap<String, Integer> nativeKeyCodes = new HashMap<String, Integer>();
        Field nativeFields[] = NativeKeyEvent.class.getDeclaredFields();
        for (int i = 0; i < nativeFields.length; i++) {
            String name = nativeFields[i].getName();
            int mod = nativeFields[i].getModifiers();

            if (Modifier.isPublic(mod) && Modifier.isStatic(mod) && Modifier.isFinal(mod) && name.startsWith("VC_")) {
                nativeKeyCodes.put(name, nativeFields[i].getInt(null));
            }
        }

        // Populate all the virtual key codes from KeyEvent
        HashMap<String, Integer> javaKeyCodes = new HashMap<String, Integer>();
        Field javaFields[] = KeyEvent.class.getDeclaredFields();
        for (int i = 0; i < javaFields.length; i++) {
            String name = javaFields[i].getName();
            int mod = javaFields[i].getModifiers();

            if (Modifier.isPublic(mod) && Modifier.isStatic(mod) && Modifier.isFinal(mod) && name.startsWith("VC_")) {
                javaKeyCodes.put(name, javaFields[i].getInt(null));
            }
        }

        // Convert the key sets to arrays for comparison.
        String[] nativeSet = nativeKeyCodes.keySet().toArray(new String[0]);
        String[] javaSet = javaKeyCodes.keySet().toArray(new String[0]);

        // Arrays are sorted to allow binary search and to ensure alignment.
        Arrays.sort(nativeSet);
        Arrays.sort(javaSet);

        for (int i = 0, j = 0; i < nativeSet.length && j < javaSet.length; i++, j++) {
            // Check for key set miss-match.
            if (nativeSet[i].equals(javaSet[j]) != true) {
                int searchPosition;

                // Search for the next aviable VC_ code.
                if ((searchPosition = Arrays.binarySearch(nativeSet, javaSet[j])) >= 0) {
                    for (int offset = 0; offset < searchPosition - j; offset++) {
                        System.err.println("Extra Native Key: " + nativeSet[i + offset]);
                    }

                    // We found the current awt VC_ code in the native list.
                    // This means we need to skip ahead in the native list by
                    // the search position.
                    i = searchPosition;
                }
                else if ((searchPosition = Arrays.binarySearch(javaSet, nativeSet[i])) >= 0) {
                    for (int offset = 0; offset < searchPosition - i; offset++) {
                        System.err.println("Missing Native Key: " + javaSet[j + offset]);
                    }

                    // We found the current natve hook VC_ code in the awt list.
                    // This means we need to skip ahead in the awt list by
                    // the search positon
                    j = searchPosition;
                }
            }
            else if (nativeKeyCodes.get(nativeSet[i]).equals(javaKeyCodes.get(javaSet[j])) != true) {
                System.err.println("Key Code Missmatch for " + nativeSet[i] + ": " + nativeKeyCodes.get(nativeSet[i]) + " != " + javaKeyCodes.get(javaSet[j]));
            }
        }
    }
}
