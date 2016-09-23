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
package org.jnativehook.mouse;

// Imports.
import org.jnativehook.mouse.listeners.NativeMouseInputListenerImpl;
import org.junit.Test;
import static org.junit.Assert.assertEquals;

/**
 *
 * @author abarker
 */
public class NativeMouseListenerTest {
	/**
	 * Test of nativeMouseClicked method, of class NativeMouseListener.
	 */
	@Test
	public void testNativeMouseClicked() {
		System.out.println("nativeMouseClicked");

		NativeMouseEvent event = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_CLICKED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		NativeMouseInputListenerImpl listener = new NativeMouseInputListenerImpl();
		listener.nativeMouseClicked(event);

		assertEquals(event, listener.getLastEvent());
	}

	/**
	 * Test of nativeMousePressed method, of class NativeMouseListener.
	 */
	@Test
	public void testNativeMousePressed() {
		System.out.println("nativeMousePressed");

		NativeMouseEvent event = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_PRESSED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		NativeMouseInputListenerImpl listener = new NativeMouseInputListenerImpl();
		listener.nativeMousePressed(event);

		assertEquals(event, listener.getLastEvent());
	}

	/**
	 * Test of nativeMouseReleased method, of class NativeMouseListener.
	 */
	@Test
	public void testNativeMouseReleased() {
		System.out.println("nativeMouseReleased");

		NativeMouseEvent event = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_RELEASED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		NativeMouseInputListenerImpl listener = new NativeMouseInputListenerImpl();
		listener.nativeMouseReleased(event);

		assertEquals(event, listener.getLastEvent());
	}
}
