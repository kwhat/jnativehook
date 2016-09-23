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
import org.junit.Test;
import java.awt.Point;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

public class NativeMouseEventTest {
	/**
	 * Test of getButton method, of class NativeMouseEvent.
	 */
	@Test
	public void testGetButton() {
		System.out.println("getButton");

		NativeMouseEvent event = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_PRESSED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		assertEquals(NativeMouseEvent.BUTTON1, event.getButton());
	}

	/**
	 * Test of getClickCount method, of class NativeMouseEvent.
	 */
	@Test
	public void testGetClickCount() {
		System.out.println("getClickCount");

		NativeMouseEvent event = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_PRESSED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		assertEquals(1, event.getClickCount());
	}

	/**
	 * Test of getPoint method, of class NativeMouseEvent.
	 */
	@Test
	public void testGetPoint() {
		System.out.println("getPoint");

		NativeMouseEvent event = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_PRESSED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		assertEquals(new Point(50, 75), event.getPoint());
	}

	/**
	 * Test of getX method, of class NativeMouseEvent.
	 */
	@Test
	public void testGetX() {
		System.out.println("getX");

		NativeMouseEvent event = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_PRESSED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		assertEquals(50, event.getX());
	}

	/**
	 * Test of getY method, of class NativeMouseEvent.
	 */
	@Test
	public void testGetY() {
		System.out.println("getY");

		NativeMouseEvent event = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_PRESSED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		assertEquals(75, event.getY());
	}

	/**
	 * Test of paramString method, of class NativeMouseEvent.
	 */
	@Test
	public void testParamString() {
		System.out.println("paramString");

		NativeMouseEvent event = new NativeMouseEvent(
				NativeMouseEvent.NATIVE_MOUSE_PRESSED,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseEvent.BUTTON1);

		assertFalse(event.paramString().equals(""));
	}
}
