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
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

public class NativeMouseWheelEventTest {
	/**
	 * Test of getScrollAmount method, of class NativeMouseWheelEvent.
	 */
	@Test
	public void testGetScrollAmount() {
		System.out.println("getScrollAmount");

		NativeMouseWheelEvent event = new NativeMouseWheelEvent(
				NativeMouseEvent.NATIVE_MOUSE_WHEEL,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseWheelEvent.WHEEL_UNIT_SCROLL,
				3,		// Scroll Amount
				-1);	// Wheel Rotation

		assertEquals(3, event.getScrollAmount());
	}

	/**
	 * Test of getScrollType method, of class NativeMouseWheelEvent.
	 */
	@Test
	public void testGetScrollType() {
		System.out.println("getScrollType");

		NativeMouseWheelEvent event = new NativeMouseWheelEvent(
				NativeMouseEvent.NATIVE_MOUSE_WHEEL,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseWheelEvent.WHEEL_UNIT_SCROLL,
				3,		// Scroll Amount
				-1);	// Wheel Rotation

		assertEquals(NativeMouseWheelEvent.WHEEL_UNIT_SCROLL, event.getScrollType());
	}

	/**
	 * Test of getWheelRotation method, of class NativeMouseWheelEvent.
	 */
	@Test
	public void testGetWheelRotation() {
		System.out.println("getWheelRotation");

		NativeMouseWheelEvent event = new NativeMouseWheelEvent(
				NativeMouseEvent.NATIVE_MOUSE_WHEEL,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseWheelEvent.WHEEL_UNIT_SCROLL,
				3,		// Scroll Amount
				-1);	// Wheel Rotation

		assertEquals(-1, event.getWheelRotation());
	}

	/**
	 * Test of paramString method, of class NativeMouseWheelEvent.
	 */
	@Test
	public void testParamString() {
		System.out.println("paramString");

		NativeMouseWheelEvent event = new NativeMouseWheelEvent(
				NativeMouseEvent.NATIVE_MOUSE_WHEEL,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseWheelEvent.WHEEL_UNIT_SCROLL,
				3,		// Scroll Amount
				-1);	// Wheel Rotation

		assertFalse(event.paramString().equals(""));
	}
}
