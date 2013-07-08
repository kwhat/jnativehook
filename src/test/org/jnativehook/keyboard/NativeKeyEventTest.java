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
		int rawCode = 0x98;

		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				rawCode,
				NativeKeyEvent.VK_UNDEFINED,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		assertEquals(rawCode, event.getRawCode());
	}

	/**
	 * Test of setRawCode method, of class NativeKeyEvent.
	 */
	@Test
	public void testSetRawCode() {
		int rawCode = 0x99;

		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				System.currentTimeMillis(),
				0x00,
				rawCode,
				NativeKeyEvent.VK_UNDEFINED,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		event.setRawCode(rawCode);

		assertEquals(event.getRawCode(), rawCode);
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

		event.setKeyCode(NativeKeyEvent.VK_B);
		assertEquals(event.getKeyCode(), NativeKeyEvent.VK_B);
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
				'c',
				NativeKeyEvent.KEY_LOCATION_UNKNOWN);

		assertEquals(event.getKeyChar(), 'c');
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

		event.setKeyChar('d');
		assertEquals(event.getKeyChar(), 'd');
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
		assertFalse(NativeKeyEvent.getKeyText(NativeKeyEvent.VK_C).equals(""));
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
}
