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
package org.jnativehook.keyboard;

// Imports.
import org.jnativehook.keyboard.listeners.NativeKeyListenerImpl;
import org.junit.Test;
import static org.junit.Assert.assertEquals;

public class NativeKeyListenerTest {
	/**
	 * Test of nativeKeyPressed method, of class NativeKeyListener.
	 */
	@Test
	public void testNativeKeyPressed() {
		System.out.println("nativeKeyPressed");

		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_PRESSED,
				NativeKeyEvent.SHIFT_MASK,
				0x41,		// Raw Code
				NativeKeyEvent.VC_A,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_STANDARD);

		NativeKeyListenerImpl listener = new NativeKeyListenerImpl();
		listener.nativeKeyPressed(event);

		assertEquals(event, listener.getLastEvent());
	}

	/**
	 * Test of nativeKeyReleased method, of class NativeKeyListener.
	 */
	@Test
	public void testNativeKeyReleased() {
		System.out.println("nativeKeyReleased");

		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_RELEASED,
				NativeKeyEvent.SHIFT_MASK,
				0x41,		// Raw Code
				NativeKeyEvent.VC_A,
				NativeKeyEvent.CHAR_UNDEFINED,
				NativeKeyEvent.KEY_LOCATION_STANDARD);

		NativeKeyListenerImpl listener = new NativeKeyListenerImpl();
		listener.nativeKeyReleased(event);

		assertEquals(event, listener.getLastEvent());
	}

	/**
	 * Test of nativeKeyTyped method, of class NativeKeyListener.
	 */
	@Test
	public void testNativeKeyTyped() {
		System.out.println("nativeKeyTyped");

		NativeKeyEvent event = new NativeKeyEvent(
				NativeKeyEvent.NATIVE_KEY_TYPED,
				NativeKeyEvent.SHIFT_MASK,
				0x41,		// Raw Code
				NativeKeyEvent.VC_UNDEFINED,
				'A',
				NativeKeyEvent.KEY_LOCATION_STANDARD);

		NativeKeyListenerImpl listener = new NativeKeyListenerImpl();
		listener.nativeKeyTyped(event);

		assertEquals(event, listener.getLastEvent());
	}
}
