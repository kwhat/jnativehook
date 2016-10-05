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
package org.jnativehook.dispatcher;

import org.jnativehook.GlobalScreen;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseMotionListener;

public class MouseMotionEventTask implements Runnable {
	/**
	 * The listeners to dispatch the event to.
	 */
	private NativeMouseMotionListener[] listeners;

	/**
	 * The event to dispatch.
	 */
	private NativeMouseEvent event;

	/**
	 * Single argument constructor for dispatch task.
	 *
	 * @param listeners an array of <code>NativeKeyListener</code> objects to dispatch to.
	 * @param event	the <code>NativeInputEvent</code> to dispatch.
	 * @see NativeMouseEvent
	 * @see NativeMouseMotionListener
	 * @see GlobalScreen#addNativeMouseMotionListener(NativeMouseMotionListener)
	 */
	public MouseMotionEventTask(NativeMouseMotionListener[] listeners, NativeMouseEvent event) {
		this.listeners = listeners;
		this.event = event;
	}

	public void run() {
		switch (this.event.getID()) {
			case NativeMouseEvent.NATIVE_MOUSE_MOVED:
				for (int i = 0; i < this.listeners.length; i++) {
					this.listeners[i].nativeMouseMoved(this.event);
				}
				break;

			case  NativeMouseEvent.NATIVE_MOUSE_DRAGGED:
				for (int i = 0; i < this.listeners.length; i++) {
					this.listeners[i].nativeMouseDragged(this.event);
				}
				break;
		}
	}
}
