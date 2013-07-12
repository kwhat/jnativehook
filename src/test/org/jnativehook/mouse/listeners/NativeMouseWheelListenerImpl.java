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
package org.jnativehook.mouse.listeners;

import org.jnativehook.mouse.NativeMouseWheelEvent;
import org.jnativehook.mouse.NativeMouseWheelListener;

public class NativeMouseWheelListenerImpl implements NativeMouseWheelListener {
	private NativeMouseWheelEvent lastEvent;

	public void nativeMouseWheelMoved(NativeMouseWheelEvent e) {
		System.out.println("Recieved " + e.paramString());

		if (e.getID() != NativeMouseWheelEvent.NATIVE_MOUSE_WHEEL) {
			throw new IllegalArgumentException("Invalid event type recieved for nativeMouseWheelMoved!");
		}

		lastEvent = e;

		synchronized(this) {
			this.notifyAll();
		}
	}

	public NativeMouseWheelEvent getLastEvent() {
		return lastEvent;
	}
}
