/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook.keyboard;

import java.awt.event.KeyEvent;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Arrays;
import java.util.HashMap;
import org.junit.Test;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;

public class NativeKeyEventTest {
	/**
	 * Test of getRawCode method, of class NativeKeyEvent.
	 */
	@Test
	public void testGetRawCode() {
		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				0x41,
				NativeKeyEvent.VK_UNDEFINED,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		assertEquals(event.getRawCode(), 0x41);
	}

	/**
	 * Test of setRawCode method, of class NativeKeyEvent.
	 */
	@Test
	public void testSetRawCode() {
		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				0x00,
				NativeKeyEvent.VK_UNDEFINED,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		event.setRawCode(0x41);
		assertEquals(event.getRawCode(), 0x41);
	}

	/**
	 * Test of getKeyCode method, of class NativeKeyEvent.
	 */
	@Test
	public void testGetKeyCode() {
		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				0x00,
				NativeKeyEvent.VK_A,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		assertEquals(event.getKeyCode(), NativeKeyEvent.VK_A);
	}

	/**
	 * Test of setKeyCode method, of class NativeKeyEvent.
	 */
	@Test
	public void testSetKeyCode() {
		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				0x00,
				NativeKeyEvent.VK_UNDEFINED,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		event.setKeyCode(NativeKeyEvent.VK_A);
		assertEquals(event.getKeyCode(), NativeKeyEvent.VK_A);
	}

	/**
	 * Test of getKeyChar method, of class NativeKeyEvent.
	 */
	@Test
	public void testGetKeyChar() {
		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				0x00,
				NativeKeyEvent.VK_UNDEFINED,
				'A',
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		assertEquals(event.getKeyChar(), 'A');
	}

	/**
	 * Test of setKeyChar method, of class NativeKeyEvent.
	 */
	@Test
	public void testSetKeyChar() {
		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				0x00,
				NativeKeyEvent.VK_UNDEFINED,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		event.setKeyChar('A');
		assertEquals(event.getKeyChar(), 'A');
	}

	/**
	 * Test of getKeyLocation method, of class NativeKeyEvent.
	 */
	@Test
	public void testGetKeyLocation() {
		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				0x00,
				NativeKeyEvent.VK_UNDEFINED,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_STANDARD);

		assertEquals(event.getKeyLocation(), NativeKeyEvent.KEY_LOCATION_STANDARD);
	}

	/**
	 * Test of getKeyText method, of class NativeKeyEvent.
	 */
	@Test
	public void testGetKeyText() {
		assertFalse(NativeKeyEvent.getKeyText(NativeKeyEvent.VK_A).equals(""));
	}

	/**
	 * Test of isActionKey method, of class NativeKeyEvent.
	 */
	@Test
	public void testIsActionKey() {
		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				0x00,
				NativeKeyEvent.VK_F1,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		assertTrue(event.isActionKey());

		event.setKeyCode(NativeKeyEvent.VK_A);
		assertFalse(event.isActionKey());
	}

	/**
	 * Test of paramString method, of class NativeKeyEvent.
	 */
	@Test
	public void testParamString() {
		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				0x00,
				NativeKeyEvent.VK_UNDEFINED,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		assertFalse(event.paramString().equals(""));
	}

	/**
	 * Test for missing constants, of class NativeKeyEvent.
	 */
	@Test
	public void testMissingConstants() throws Exception {
		// Populate all the virtual key codes from NativeKeyEvent
		HashMap<String, Integer> nativeKeyCodes = new HashMap<String, Integer>();
		Field nativeFields[] = NativeKeyEvent.class.getDeclaredFields();
		for (int i = 0; i < nativeFields.length; i++) {
			String name = nativeFields[i].getName();
			int mod = nativeFields[i].getModifiers();

			if (Modifier.isPublic(mod) && Modifier.isStatic(mod) && Modifier.isFinal(mod) && name.startsWith("VK_")) {
				nativeKeyCodes.put(name, nativeFields[i].getInt(null));
			}
		}

		// Populate all the virtual key codes from KeyEvent
		HashMap<String, Integer> javaKeyCodes = new HashMap<String, Integer>();
		Field javaFields[] = KeyEvent.class.getDeclaredFields();
		for (int i = 0; i < javaFields.length; i++) {
			String name = javaFields[i].getName();
			int mod = javaFields[i].getModifiers();

			if (Modifier.isPublic(mod) && Modifier.isStatic(mod) && Modifier.isFinal(mod) && name.startsWith("VK_")) {
				javaKeyCodes.put(name, javaFields[i].getInt(null));
			}
		}

		// Convert the key sets to arrays for comparision.
		String[] nativeSet = nativeKeyCodes.keySet().toArray(new String[0]);
		String[] javaSet = javaKeyCodes.keySet().toArray(new String[0]);

		// Arrays are sorted to allow binary search and to ensure alignment.
		Arrays.sort(nativeSet);
		Arrays.sort(javaSet);

		for (int i = 0, j = 0; i < nativeSet.length && j < javaSet.length; i++, j++) {
			// Check for key set miss-match.
			if (nativeSet[i].equals(javaSet[j]) != true) {
				int searchPosition;

				// Search for the next aviable VK_ code.
				if ((searchPosition = Arrays.binarySearch(nativeSet, javaSet[j])) >= 0) {
					for (int offset = 0; offset < searchPosition - j; offset++) {
						System.out.println("Extra Native Key: " + nativeSet[i + offset]);
					}

					// We found the current awt VK_ code in the native list.
					// This means we need to skip ahead in the native list by
					// the search position.
					i = searchPosition;
				}
				else if ((searchPosition = Arrays.binarySearch(javaSet, nativeSet[i])) >= 0) {
					for (int offset = 0; offset < searchPosition - i; offset++) {
						System.out.println("Missing Native Key: " + javaSet[j + offset]);
					}

					// We found the current natve hook VK_ code in the awt list.
					// This means we need to skip ahead in the awt list by
					// the search positon
					j = searchPosition;
				}
			}
			else if (nativeKeyCodes.get(nativeSet[i]) != javaKeyCodes.get(javaSet[j])) {
				System.out.println("Key Code Missmatch: " + nativeKeyCodes.get(nativeSet[i]) + " != " + javaKeyCodes.get(javaSet[j]));
			}
		}
	}
}
