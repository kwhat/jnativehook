/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2016 Alexander Barker.  All Rights Received.
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
package org.jnativehook;

// Imports.
import org.jnativehook.keyboard.NativeKeyEvent;
import org.junit.Test;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

public class NativeInputEventTest {
	/**
	 * Test of getID method, of class NativeInputEvent.
	 */
	@Test
	public void testGetID() {
		System.out.println("getID");

		NativeInputEvent event = new NativeInputEvent(
				GlobalScreen.class,
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				0x00);

		assertEquals(event.getID(), NativeKeyEvent.NATIVE_KEY_PRESSED);
	}

	/**
	 * Test of getWhen method, of class NativeInputEvent.
	 */
	@Test
	public void testGetWhen() {
		System.out.println("getWhen");

		long when = 0;

		NativeInputEvent event = new NativeInputEvent(
				GlobalScreen.class,
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				0x00);

		assertEquals(event.getWhen(), when);
	}

	/**
	 * Test of getModifiers method, of class NativeInputEvent.
	 */
	@Test
	public void testGetModifiers() {
		System.out.println("getModifiers");

		int mask =	NativeInputEvent.ALT_MASK |
					NativeInputEvent.CTRL_MASK |
					NativeInputEvent.META_MASK |
					NativeInputEvent.SHIFT_MASK;

		NativeInputEvent event = new NativeInputEvent(
				GlobalScreen.class,
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				mask);

		assertEquals(event.getModifiers(), mask);
	}

	/**
	 * Test of setModifiers method, of class NativeInputEvent.
	 */
	@Test
	public void testSetModifiers() {
		System.out.println("setModifiers");

		int mask =	NativeInputEvent.BUTTON1_MASK |
					NativeInputEvent.BUTTON2_MASK |
					NativeInputEvent.BUTTON3_MASK |
					NativeInputEvent.BUTTON4_MASK |
					NativeInputEvent.BUTTON5_MASK;

		NativeInputEvent event = new NativeInputEvent(
				GlobalScreen.class,
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				0x00);

		event.setModifiers(mask);
		assertEquals(event.getModifiers(), mask);
	}

	/**
	 * Test of getModifiersText method, of class NativeInputEvent.
	 */
	@Test
	public void testGetModifiersText() {
		System.out.println("getModifiersText");

		int mask =	NativeInputEvent.ALT_MASK |
					NativeInputEvent.BUTTON1_MASK;

		assertFalse(NativeInputEvent.getModifiersText(mask).equals(""));
	}

	/**
	 * Test of paramString method, of class NativeInputEvent.
	 */
	@Test
	public void testParamString() {
		System.out.println("paramString");

		NativeInputEvent event = new NativeInputEvent(
				GlobalScreen.class,
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				NativeInputEvent.SHIFT_MASK |
				NativeInputEvent.BUTTON5_MASK);

		assertFalse(event.paramString().equals(""));
	}
}
