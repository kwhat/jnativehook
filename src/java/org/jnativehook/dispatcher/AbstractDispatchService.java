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

// Imports.
import org.jnativehook.GlobalScreen;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseListener;
import org.jnativehook.mouse.NativeMouseMotionListener;
import org.jnativehook.mouse.NativeMouseWheelEvent;
import org.jnativehook.mouse.NativeMouseWheelListener;

import java.util.concurrent.ExecutorService;

/**
 * Default implementation of the <code>ExecutorService</code> used to dispatch native events.  This is effectively
 * the same as calling {@link java.util.concurrent.Executors#newSingleThreadExecutor}.
 * <p>
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	2.1
 * @since	2.1
 *
 * @see  ExecutorService
 * @see  GlobalScreen#setEventDispatcher
 */
public abstract class AbstractDispatchService implements DispatchService {
	/**
	 * Processes native key events by dispatching them to all registered
	 * <code>NativeKeyListener</code> objects.
	 *
	 * @param e the <code>NativeKeyEvent</code> to dispatch.
	 * @see NativeKeyEvent
	 * @see NativeKeyListener
	 * @see GlobalScreen.addNativeKeyListener(NativeKeyListener)
	 */
	protected void processKeyEvent(NativeKeyEvent e) {
		NativeKeyListener[] listeners = eventListeners.getListeners(NativeKeyListener.class);

		switch (e.getID()) {
			case NativeKeyEvent.NATIVE_KEY_PRESSED:
				for (int i = 0; i < listeners.length; i++) {
					listeners[i].nativeKeyPressed(e);
				}
				break;

			case NativeKeyEvent.NATIVE_KEY_TYPED:
				for (int i = 0; i < listeners.length; i++) {
					listeners[i].nativeKeyTyped(e);
				}
				break;

			case NativeKeyEvent.NATIVE_KEY_RELEASED:
				for (int i = 0; i < listeners.length; i++) {
					listeners[i].nativeKeyReleased(e);
				}
				break;
		}
	}

	/**
	 * Processes native mouse button events by dispatching them to all registered
	 * <code>NativeMouseListener</code> objects.
	 *
	 * @param e the <code>NativeMouseEvent</code> to dispatch.
	 * @see NativeMouseEvent
	 * @see NativeMouseListener
	 * @see #addNativeMouseListener(NativeMouseListener)
	 */
	protected void processButtonEvent(NativeMouseEvent e) {
		NativeMouseListener[] listeners = eventListeners.getListeners(NativeMouseListener.class);

		switch (e.getID()) {
			case NativeMouseEvent.NATIVE_MOUSE_CLICKED:
				for (int i = 0; i < listeners.length; i++) {
					listeners[i].nativeMouseClicked(e);
				}
				break;

			case NativeMouseEvent.NATIVE_MOUSE_PRESSED:
				for (int i = 0; i < listeners.length; i++) {
					listeners[i].nativeMousePressed(e);
				}
				break;

			case NativeMouseEvent.NATIVE_MOUSE_RELEASED:
				for (int i = 0; i < listeners.length; i++) {
					listeners[i].nativeMouseReleased(e);
				}
				break;
		}
	}

	/**
	 * Processes native mouse events by dispatching them to all registered
	 * <code>NativeMouseListener</code> objects.
	 *
	 * @param e the <code>NativeMouseEvent</code> to dispatch.
	 * @see NativeMouseEvent
	 * @see NativeMouseMotionListener
	 * @see #addNativeMouseMotionListener(NativeMouseMotionListener)
	 */
	protected void processMouseEvent(NativeMouseEvent e) {
		NativeMouseMotionListener[] listeners = eventListeners.getListeners(NativeMouseMotionListener.class);

		switch (e.getID()) {
			case NativeMouseEvent.NATIVE_MOUSE_MOVED:
				for (int i = 0; i < listeners.length; i++) {
					listeners[i].nativeMouseMoved(e);
				}
				break;

			case NativeMouseEvent.NATIVE_MOUSE_DRAGGED:
				for (int i = 0; i < listeners.length; i++) {
					listeners[i].nativeMouseDragged(e);
				}
				break;
		}
	}

	/**
	 * Processes native mouse wheel events by dispatching them to all registered
	 * <code>NativeMouseWheelListener</code> objects.
	 *
	 * @param e The <code>NativeMouseWheelEvent</code> to dispatch.
	 * @see NativeMouseWheelEvent
	 * @see NativeMouseWheelListener
	 * @see #addNativeMouseWheelListener(NativeMouseWheelListener)
	 * @since 1.1
	 */
	protected void processMouseWheelEvent(NativeMouseWheelEvent e) {
		NativeMouseWheelListener[] listeners = eventListeners.getListeners(NativeMouseWheelListener.class);

		for (int i = 0; i < listeners.length; i++) {
			listeners[i].nativeMouseWheelMoved(e);
		}
	}
}
