/* JNativeHook: Global keyboard and mouse listeners for Java.
 * Copyright (C) 2006-2021 Alexander Barker.  All Rights Reserved.
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
package com.github.kwhat.jnativehook.mouse;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class NativeMouseWheelListenerTest {
	@Test
	public void testNativeMouseWheelMoved() {
		NativeMouseWheelEvent event = new NativeMouseWheelEvent(
				NativeMouseEvent.NATIVE_MOUSE_WHEEL,
				0x00,
				50,
				75,
				1,
				NativeMouseWheelEvent.WHEEL_UNIT_SCROLL,
				3,
				-1);

		com.github.kwhat.jnativehook.mouse.listeners.NativeMouseWheelListenerTest listener = new com.github.kwhat.jnativehook.mouse.listeners.NativeMouseWheelListenerTest();
		listener.nativeMouseWheelMoved(event);

		assertEquals(event, listener.getLastEvent());
	}
}
