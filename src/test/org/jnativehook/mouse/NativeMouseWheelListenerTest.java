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
import org.jnativehook.mouse.listeners.NativeMouseWheelListenerImpl;
import org.junit.Test;
import static org.junit.Assert.assertEquals;

/**
 *
 * @author abarker
 */
public class NativeMouseWheelListenerTest {
	/**
	 * Test of nativeMouseWheelMoved method, of class NativeMouseWheelListener.
	 */
	@Test
	public void testNativeMouseWheelMoved() {
		System.out.println("nativeMouseWheelMoved");

		NativeMouseWheelEvent event = new NativeMouseWheelEvent(
				NativeMouseEvent.NATIVE_MOUSE_WHEEL,
				0x00,	// Modifiers
				50,		// X
				75,		// Y
				1,		// Click Count
				NativeMouseWheelEvent.WHEEL_UNIT_SCROLL,
				3,		// Scroll Amount
				-1);	// Wheel Rotation

		NativeMouseWheelListenerImpl listener = new NativeMouseWheelListenerImpl();
		listener.nativeMouseWheelMoved(event);

		assertEquals(event, listener.getLastEvent());
	}
}
