/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2015 Alexander Barker.  All Rights Received.
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
package org.jnativehook;

// Imports.
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseListener;
import org.jnativehook.mouse.NativeMouseMotionListener;
import org.jnativehook.mouse.NativeMouseWheelEvent;
import org.jnativehook.mouse.NativeMouseWheelListener;
import javax.swing.event.EventListenerList;
import java.io.File;
import java.util.Iterator;
import java.util.concurrent.ExecutorService;
import java.util.logging.Logger;

/**
 * GlobalScreen is used to represent the native screen area that Java does not
 * usually have access to. This class can be thought of as the source component
 * for native input events.
 * <p>
 * This class also handles the loading, unpacking and communication with the
 * native library. That includes registering and un-registering the native hook
 * with the underlying operating system and adding global keyboard and mouse
 * listeners.
 *
 * @author Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version 2.0
 * @since 1.0
 */
public final class GlobalScreen {
	private static Logger log = Logger.getLogger(GlobalScreen.class.getPackage().getName());

	/**
	 * The service to control the hook.
	 */
	private static NativeHookThread hookThread;

	/**
	 * The service to dispatch events.
	 */
	private static ExecutorService eventExecutor = new DefaultDispatchService();

	/**
	 * The list of event listeners to notify.
	 */
	private static EventListenerList eventListeners = new EventListenerList();


	static {
		String libName = System.getProperty("jnativehook.lib.name", "JNativeHook");
		try {
			// Try to load the native library assuming the java.library.path was set correctly at launch.
			System.loadLibrary(libName);
		}
		catch (UnsatisfiedLinkError linkError) {
			String libLoader = System.getProperty("jnativehook.lib.locator", "org.jnativehook.DefaultLibraryLocator");

			try {
				// Use the specified class to locator the native library.
				NativeLibraryLocator locator = Class.forName(libLoader).asSubclass(NativeLibraryLocator.class).newInstance();

				Iterator<File> libs = locator.getLibraries();
				while (libs.hasNext()) {
					File lib = libs.next();
					if (lib.exists() && lib.isFile() && lib.canRead()) {
						System.load(lib.getPath());
					}
				}
			}
			catch (Exception e) {
				// There was a problem instantiating the library loader.
				log.severe(e.getMessage());

				throw new UnsatisfiedLinkError(e.getMessage());
			}
		}
	}


	private GlobalScreen() { }


	/**
	 * Adds the specified native key listener to receive key events from the
	 * native system. If listener is null, no exception is thrown and no action
	 * is performed.
	 *
	 * @param listener a native key listener object
	 */
	public static void addNativeKeyListener(NativeKeyListener listener) {
		if (listener != null) {
			eventListeners.add(NativeKeyListener.class, listener);
		}
	}

	/**
	 * Removes the specified native key listener so that it no longer receives
	 * key events from the native system. This method performs no function if
	 * the listener specified by the argument was not previously added.  If
	 * listener is null, no exception is thrown and no action is performed.
	 *
	 * @param listener a native key listener object
	 */
	public static void removeNativeKeyListener(NativeKeyListener listener) {
		if (listener != null) {
			eventListeners.remove(NativeKeyListener.class, listener);
		}
	}

	/**
	 * Adds the specified native mouse listener to receive mouse events from the
	 * native system. If listener is null, no exception is thrown and no action
	 * is performed.
	 *
	 * @param listener a native mouse listener object
	 */
	public static void addNativeMouseListener(NativeMouseListener listener) {
		if (listener != null) {
			eventListeners.add(NativeMouseListener.class, listener);
		}
	}

	/**
	 * Removes the specified native mouse listener so that it no longer receives
	 * mouse events from the native system. This method performs no function if
	 * the listener specified by the argument was not previously added.  If
	 * listener is null, no exception is thrown and no action is performed.
	 *
	 * @param listener a native mouse listener object
	 */
	public static void removeNativeMouseListener(NativeMouseListener listener) {
		if (listener != null) {
			eventListeners.remove(NativeMouseListener.class, listener);
		}
	}

	/**
	 * Adds the specified native mouse motion listener to receive mouse motion
	 * events from the native system. If listener is null, no exception is
	 * thrown and no action is performed.
	 *
	 * @param listener a native mouse motion listener object
	 */
	public static void addNativeMouseMotionListener(NativeMouseMotionListener listener) {
		if (listener != null) {
			eventListeners.add(NativeMouseMotionListener.class, listener);
		}
	}

	/**
	 * Removes the specified native mouse motion listener so that it no longer
	 * receives mouse motion events from the native system. This method performs
	 * no function if the listener specified by the argument was not previously
	 * added.  If listener is null, no exception is thrown and no action is
	 * performed.
	 *
	 * @param listener a native mouse motion listener object
	 */
	public static void removeNativeMouseMotionListener(NativeMouseMotionListener listener) {
		if (listener != null) {
			eventListeners.remove(NativeMouseMotionListener.class, listener);
		}
	}

	/**
	 * Adds the specified native mouse wheel listener to receive mouse wheel
	 * events from the native system. If listener is null, no exception is
	 * thrown and no action is performed.
	 *
	 * @param listener a native mouse wheel listener object
	 * @since 1.1
	 */
	public static void addNativeMouseWheelListener(NativeMouseWheelListener listener) {
		if (listener != null) {
			eventListeners.add(NativeMouseWheelListener.class, listener);
		}
	}

	/**
	 * Removes the specified native mouse wheel listener so that it no longer
	 * receives mouse wheel events from the native system. This method performs
	 * no function if the listener specified by the argument was not previously
	 * added.  If listener is null, no exception is thrown and no action is
	 * performed.
	 *
	 * @param listener a native mouse wheel listener object
	 * @since 1.1
	 */
	public static void removeNativeMouseWheelListener(NativeMouseWheelListener listener) {
		if (listener != null) {
			eventListeners.remove(NativeMouseWheelListener.class, listener);
		}
	}

	/**
	 * Specialized thread implementation for the native hook.
	 */
	private static class NativeHookThread extends Thread {
		/**
		 * Exception thrown by this thread.
		 */
		private NativeHookException exception;

		/**
		 * Default constructor.
		 */
		public NativeHookThread() {
			this.setName("JNativeHook Hook Thread");
			this.setDaemon(false);
			this.setPriority(Thread.MAX_PRIORITY);
		}

		public void run() {
			exception = null;

			try {
				// NOTE enable() will call notifyAll() on this object after passing exception throwing code.
				this.enable();
			}
			catch (NativeHookException e) {
				exception = e;
			}

			synchronized (this) {
				// Notify anyone that is still waiting for the hook that it has completed.
				this.notifyAll();
			}
		}

		/**
		 * Get the exception associated with the current hook, or null of no exception was thrown.
		 *
		 * @return the <code>NativeHookException</code> or null.
		 */
		public NativeHookException getException() {
			return exception;
		}

		/**
		 * Native implementation to start the input hook.  This method blocks and should only be called by this
		 * specialized thread implementation.  This method will notifyAll() after passing any exception exception
		 * throwing code.
		 */
		private native void enable() throws NativeHookException;

		/**
		 * Native implementation to stop the input hook.  There is no other way to stop the hook.
		 */
		public native void disable() throws NativeHookException;
	}


	/**
	 * Enable the native hook if it is not currently running. If it is running,
	 * the function has no effect.
	 * <p>
	 * <b>Note:</b> This method will throw a <code>NativeHookException</code>
	 * if specific operating system features are unavailable or disabled.
	 * For example: Access for assistive devices is unchecked in the Universal
	 * Access section of the System Preferences on Apple's OS X platform or
	 * <code>Load "record"</code> is missing for the xorg.conf file on
	 * Unix/Linux/Solaris platforms.
	 *
	 * @throws NativeHookException problem registering the native hook with
	 *                             the underlying operating system.
	 * @since 1.1
	 */
	public static void registerNativeHook() throws NativeHookException {
		if (hookThread == null || !hookThread.isAlive()) {
			hookThread = new NativeHookThread();

			synchronized (hookThread) {
				hookThread.start();
				try {
					hookThread.wait();
				}
				catch (InterruptedException e) {
					throw new NativeHookException(e);
				}

				NativeHookException exception = hookThread.getException();
				if (exception != null) {
					throw exception;
				}
			}
		}
	}

	/**
	 * Disable the native hook if it is currently registered. If the native
	 * hook it is not registered the function has no effect.
	 *
	 * @throws NativeHookException hook interrupted by Java.
	 *
	 * @since 1.1
	 */
	public static void unregisterNativeHook() throws NativeHookException {
		if (isNativeHookRegistered()) {
			synchronized (hookThread) {
				hookThread.disable();

				try {
					hookThread.join();
				}
				catch (InterruptedException e) {
					throw new NativeHookException(e.getCause());
				}
			}
		}
	}

	/**
	 * Returns <code>true</code> if the native hook is currently registered.
	 *
	 * @return true if the native hook is currently registered.
	 * @since 1.1
	 */
	public static boolean isNativeHookRegistered() {
		return hookThread != null && hookThread.isAlive();
	}


	/**
	 * Add a <code>NativeInputEvent</code> to the operating system's event queue.
	 * Each type of <code>NativeInputEvent</code> is processed according to its
	 * event id.
	 * <p>
	 *
	 * For both <code>NATIVE_KEY_PRESSED</code> and
	 * <code>NATIVE_KEY_RELEASED</code> events, the virtual keycode and modifier
	 * mask are used in the creation of the native event.  Please note that some
	 * platforms may generate <code>NATIVE_KEY_PRESSED</code> and
	 * <code>NATIVE_KEY_RELEASED</code> events for each required modifier.
	 * <code>NATIVE_KEY_TYPED</code> events will first translate the associated
	 * keyChar to its respective virtual code and then produce a
	 * <code>NATIVE_KEY_PRESSED</code> followed by a <code>NATIVE_KEY_RELEASED</code>
	 * event using that virtual code.  If the JNativeHook is unable to translate
	 * the keyChar to its respective virtual code, the event is ignored.
	 * <p>
	 *
	 * <code>NativeMouseEvents</code> are processed in much the same way as the
	 * <code>NativeKeyEvents</code>.  Both <code>NATIVE_MOUSE_PRESSED</code> and
	 * <code>NATIVE_MOUSE_RELEASED</code> produce events corresponding to the
	 * event's button code.  Keyboard modifiers may be used in conjunction with
	 * button press and release events, however, they might produce events for each
	 * modifier.  <code>NATIVE_MOUSE_CLICKED</code> events produce a
	 * <code>NATIVE_MOUSE_PRESSED</code> event followed by a
	 * <code>NATIVE_MOUSE_RELEASED</code> for the assigned event button.
	 * <p>
	 *
	 * <code>NATIVE_MOUSE_DRAGGED</code> and <code>NATIVE_MOUSE_MOVED</code> events
	 * are handled identically.  In order to produce a <code>NATIVE_MOUSE_DRAGGED</code>
	 * event, you must specify a button modifier mask that contains at least one
	 * button modifier and assign it to the event.  Failure to do so will produce a
	 * <code>NATIVE_MOUSE_MOVED</code> event even if the event id was set to
	 * <code>NATIVE_MOUSE_DRAGGED</code>.
	 * <p>
	 *
	 * <code>NATIVE_MOUSE_WHEEL</code> events are identical to
	 * <code>NATIVE_MOUSE_PRESSED</code> events.  Wheel events will only produce
	 * pressed events and will never produce <code>NATIVE_MOUSE_RELEASED</code>,
	 * <code>NATIVE_MOUSE_DRAGGED</code> or <code>NATIVE_MOUSE_MOVED</code>
	 *
	 * @param e the <code>NativeInputEvent</code> sent to the native system.
	 * @since 2.0
	 */
	public static native void postNativeEvent(NativeInputEvent e);

	/**
	 * Internal class to handle event dispatching via the executor service.
	 */
	private static class EventDispatchTask implements Runnable {
		/**
		 * The event to dispatch.
		 */
		private NativeInputEvent event;

		/**
		 * Single argument constructor for dispatch task.
		 *
		 * @param event	the <code>NativeInputEvent</code> to dispatch.
		 */
		public EventDispatchTask(NativeInputEvent event) {
			this.event = event;
		}

		public void run() {
			if (event instanceof NativeKeyEvent) {
				processKeyEvent((NativeKeyEvent) event);
			}
			else if (event instanceof NativeMouseWheelEvent) {
				processMouseWheelEvent((NativeMouseWheelEvent) event);
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
		 * @see #addNativeKeyListener(NativeKeyListener)
		 */
		private void processKeyEvent(final NativeKeyEvent e) {
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
		private void processButtonEvent(final NativeMouseEvent e) {
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
		private void processMouseEvent(final NativeMouseEvent e) {
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
		private void processMouseWheelEvent(final NativeMouseWheelEvent e) {
			NativeMouseWheelListener[] listeners = eventListeners.getListeners(NativeMouseWheelListener.class);

			for (int i = 0; i < listeners.length; i++) {
				listeners[i].nativeMouseWheelMoved(e);
			}
		}
	}

	/**
	 * Dispatches an event to the appropriate processor.  This method is
	 * generally called by the native library but may be used to synthesize
	 * native events from Java without replaying them on the native system.  If
	 * you would like to send events to other applications, please use
	 * {@link #postNativeEvent},
	 * <p>
	 *
	 * <b>Note:</b> This method executes on the native system's event queue.
	 * It is imperative that all processing be off-loaded to other threads.
	 * Failure to do so might result in the delay of user input and the automatic
	 * removal of the native hook.
	 *
	 * @param e the <code>NativeInputEvent</code> sent to the registered event listeners.
	 */
	public static void dispatchEvent(NativeInputEvent e) {
		if (eventExecutor != null) {
			eventExecutor.execute(new EventDispatchTask(e));
		}
	}

	/**
	 * Set a different executor service for native event delivery.  By default,
	 * JNativeHook utilizes a single thread executor to dispatch events from
	 * the native event queue.  You may choose to use an alternative approach
	 * for event delivery by implementing an <code>ExecutorService</code>.
	 * <p>
	 * <b>Note:</b> Using null as an <code>ExecutorService</code> will cause all
	 * delivered events to be discarded until a valid <code>ExecutorService</code>
	 * is set.
	 *
	 * @param dispatcher The <code>ExecutorService</code> used to dispatch native events.
	 * @see java.util.concurrent.ExecutorService
	 * @see java.util.concurrent.Executors#newSingleThreadExecutor()
	 * @since 2.0
	 */
	public static void setEventDispatcher(ExecutorService dispatcher) {
		if (GlobalScreen.eventExecutor != null) {
			GlobalScreen.eventExecutor.shutdown();
		}

		GlobalScreen.eventExecutor = dispatcher;
	}
}
