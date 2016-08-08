package org.jnativehook.dispatcher;

import org.jnativehook.GlobalScreen;
import org.jnativehook.NativeInputEvent;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseInputListener;
import org.jnativehook.mouse.NativeMouseListener;
import org.jnativehook.mouse.NativeMouseMotionListener;
import org.jnativehook.mouse.NativeMouseWheelEvent;
import org.jnativehook.mouse.NativeMouseWheelListener;

public class DispatchTaskFactory {


	public Runnable create(NativeInputEvent event) {
		if (event instanceof NativeKeyEvent) {

		}
		else if (event instanceof NativeMouseWheelEvent) {

		}
		else if (event instanceof NativeMouseEvent) {
			switch (event.getID()) {
				case NativeMouseEvent.NATIVE_MOUSE_PRESSED:
				case NativeMouseEvent.NATIVE_MOUSE_CLICKED:
				case NativeMouseEvent.NATIVE_MOUSE_RELEASED:
					processButtonEvent((NativeMouseEvent) event);
					break;

				case NativeMouseEvent.NATIVE_MOUSE_MOVED:
				case NativeMouseEvent.NATIVE_MOUSE_DRAGGED:
					processMouseEvent((NativeMouseEvent) event);
					break;
			}
		}
		else if (event instanceof NativeMouseWheelEvent) {
			processMouseWheelEvent((NativeMouseWheelEvent) event);
		}
	}


	/**
	 * Processes native key events by dispatching them to all registered
	 * <code>NativeKeyListener</code> objects.
	 *
	 * @param e the <code>NativeKeyEvent</code> to dispatch.
	 * @see NativeKeyEvent
	 * @see NativeKeyListener
	 * @see GlobalScreen#addNativeKeyListener(NativeKeyListener)
	 */
	private void processKeyEvent(NativeKeyEvent e) {
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
	 * @see GlobalScreen#addNativeMouseListener(NativeMouseListener)
	 */
	private void processButtonEvent(NativeMouseEvent e) {
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
	 * @see GlobalScreen#addNativeMouseMotionListener(NativeMouseMotionListener)
	 */
	private void processMouseEvent(NativeMouseEvent e) {
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
	 * @see GlobalScreen#addNativeMouseWheelListener(NativeMouseWheelListener)
	 * @since 1.1
	 */
	private void processMouseWheelEvent(NativeMouseWheelEvent e) {
		NativeMouseWheelListener[] listeners = eventListeners.getListeners(NativeMouseWheelListener.class);

		for (int i = 0; i < listeners.length; i++) {
			listeners[i].nativeMouseWheelMoved(e);
		}
	}
}
