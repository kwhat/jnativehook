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
package org.jnativehook.keyboard.listeners;

// Imports.

import org.jnativehook.NativeInputEvent;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;

import java.lang.reflect.Field;

import static org.junit.Assert.fail;

public class NativeKeyListenerImpl implements NativeKeyListener {
	private NativeKeyEvent lastEvent;

	public void nativeKeyPressed(NativeKeyEvent e) {
		System.out.println("Received " + e.paramString());

		if (e.getID() != NativeKeyEvent.NATIVE_KEY_PRESSED) {
			fail("Invalid event type received for nativeKeyPressed!");
		}

		lastEvent = e;

		synchronized(this) {
			this.notifyAll();
		}

		try {
			Field f = NativeInputEvent.class.getDeclaredField("reserved");
			f.setAccessible(true);
			f.setShort(e, (short) 0x01);
		}
		catch (Exception e1) {
			e1.printStackTrace();
		}
	}

	public void nativeKeyReleased(NativeKeyEvent e) {
		System.out.println("Received " + e.paramString());

		if (e.getID() != NativeKeyEvent.NATIVE_KEY_RELEASED) {
			fail("Invalid event type received for nativeKeyPressed!");
		}

		lastEvent = e;

		synchronized(this) {
			this.notifyAll();
		}

		try {
			Field f = NativeInputEvent.class.getDeclaredField("reserved");
			f.setAccessible(true);
			f.setShort(e, (short) 0x01);
		}
		catch (Exception e1) {
			e1.printStackTrace();
		}
	}

	public void nativeKeyTyped(NativeKeyEvent e) {
		System.out.println("Received " + e.paramString());

		if (e.getID() != NativeKeyEvent.NATIVE_KEY_TYPED) {
			fail("Invalid event type received for nativeKeyPressed!");
		}

		lastEvent = e;

		synchronized(this) {
			this.notifyAll();
		}

		try {
			Field f = NativeInputEvent.class.getDeclaredField("reserved");
			f.setAccessible(true);
			f.setShort(e, (short) 0x01);
		}
		catch (Exception e1) {
			e1.printStackTrace();
		}
	}

	public NativeKeyEvent getLastEvent() {
		return lastEvent;
	}
}