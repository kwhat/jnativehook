/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2017 Alexander Barker.  All Rights Received.
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

//Imports
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseListener;
import org.jnativehook.mouse.NativeMouseMotionListener;
import org.jnativehook.mouse.NativeMouseWheelEvent;
import org.jnativehook.mouse.NativeMouseWheelListener;

import javax.swing.event.EventListenerList;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.EventListener;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;

/**
 * GlobalScreen is used to represent the native screen area that Java does not
 * usually have access to. This class can be thought of as the source component
 * for native input events.
 * <p>
 * This class also handles the loading, unpacking and communication with the
 * native library. That includes registering and unregistering the native hook
 * with the underlying operating system and adding global keyboard and mouse
 * listeners.
 *
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.1
 */
public class GlobalScreen {
	/** The GlobalScreen singleton. */
	private static final GlobalScreen instance = new GlobalScreen();

	/** The list of event listeners to notify. */
	private EventListenerList eventListeners;

	/** The service to dispatch events. */
	private ExecutorService eventExecutor;

	/**
	 * Private constructor to prevent multiple instances of the global screen.
	 * The {@link #registerNativeHook} method will be called on construction to
	 * unpack and load the native library.
	 */
	private GlobalScreen() {
		//Setup instance variables.
		eventListeners = new EventListenerList();

		//Unpack and Load the native library.
		GlobalScreen.loadNativeLibrary();
	}

	/**
	 * A destructor that will perform native cleanup by calling the
	 * {@link #unregisterNativeHook} method.  This method will not run until the
	 * class is garbage collected.
	 *
	 * @throws Throwable a <code>NativeHookException</code> raised by calling
	 * {@link #unloadNativeLibrary()}
	 * @see Object#finalize
	 */
	@Override
	protected void finalize() throws Throwable {
		if (GlobalScreen.isNativeHookRegistered()) {
			GlobalScreen.unloadNativeLibrary();
		}

		super.finalize();
	}

	/**
	 * Returns the singleton instance of <code>GlobalScreen</code>.
	 *
	 * @return singleton instance of <code>GlobalScreen</code>
	 */
	public static GlobalScreen getInstance() {
		return GlobalScreen.instance;
	}

	/**
	 * Adds the specified native key listener to receive key events from the
	 * native system. If listener is null, no exception is thrown and no action
	 * is performed.
	 *
	 * @param listener a native key listener object
	 */
	public void addNativeKeyListener(NativeKeyListener listener) {
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
	public void removeNativeKeyListener(NativeKeyListener listener) {
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
	public void addNativeMouseListener(NativeMouseListener listener) {
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
	public void removeNativeMouseListener(NativeMouseListener listener) {
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
	public void addNativeMouseMotionListener(NativeMouseMotionListener listener) {
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
	public void removeNativeMouseMotionListener(NativeMouseMotionListener listener) {
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
	 *
	 * @since 1.1
	 */
	public void addNativeMouseWheelListener(NativeMouseWheelListener listener) {
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
	 *
	 * @since 1.1
	 */
	public void removeNativeMouseWheelListener(NativeMouseWheelListener listener) {
		if (listener != null) {
			eventListeners.remove(NativeMouseWheelListener.class, listener);
		}
	}

	/**
	 * Enable the native hook if it is not currently running. If it is running
	 * the function has no effect.
	 * <p>
	 * <b>Note: </b> This method will throw a <code>NativeHookException</code>
	 * if specific operating system features are unavailable or disabled.
	 * For example: Access for assistive devices is unchecked in the Universal
	 * Access section of the System Preferences on Apple's OS X platform or
	 * <code>Load "record"</code> is missing for the xorg.conf file on
	 * Unix/Linux/Solaris platforms.
	 *
	 * @throws NativeHookException problem registering the native hook with
	 * the underlying operating system.
	 *
	 * @since 1.1
	 */
	public static native void registerNativeHook() throws NativeHookException;

	/**
	 * Disable the native hook if it is currently registered. If the native
	 * hook it is not registered the function has no effect.
	 *
	 * @since 1.1
	 */
	public static native void unregisterNativeHook();

	/**
	 * Returns <code>true</code> if the native hook is currently registered.
	 *
	 * @return true if the native hook is currently registered.
	 *
	 * @since 1.1
	 */
	public static native boolean isNativeHookRegistered();
	
	/**
	 * Dispatches an event to the appropriate processor.  This method is
	 * generally called by the native library but maybe used to synthesize
	 * native events from Java.
	 *
	 * @param e the <code>NativeInputEvent</code> to dispatch.
	 */
	public final void dispatchEvent(final NativeInputEvent e) {
		eventExecutor.execute(new Runnable() {
			public void run() {
				if (e instanceof NativeKeyEvent) {
					processKeyEvent((NativeKeyEvent) e);
				}
				else if (e instanceof NativeMouseWheelEvent) {
					processMouseWheelEvent((NativeMouseWheelEvent) e);
				}
				else if (e instanceof NativeMouseEvent) {
					processMouseEvent((NativeMouseEvent) e);
				}
			}
		});
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
	protected void processKeyEvent(NativeKeyEvent e) {
		int id = e.getID();
		EventListener[] listeners = eventListeners.getListeners(NativeKeyListener.class);

		for (int i = 0; i < listeners.length; i++) {
			switch (id) {
				case NativeKeyEvent.NATIVE_KEY_PRESSED:
					((NativeKeyListener) listeners[i]).nativeKeyPressed(e);
					break;

				case NativeKeyEvent.NATIVE_KEY_TYPED:
					((NativeKeyListener) listeners[i]).nativeKeyTyped(e);
					break;

				case NativeKeyEvent.NATIVE_KEY_RELEASED:
					((NativeKeyListener) listeners[i]).nativeKeyReleased(e);
					break;
			}
		}
	}

	/**
	 * Processes native mouse events by dispatching them to all registered
	 * <code>NativeMouseListener</code> objects.
	 *
	 * @param e the <code>NativeMouseEvent</code> to dispatch.
	 * @see NativeMouseEvent
	 * @see NativeMouseListener
	 * @see #addNativeMouseListener(NativeMouseListener)
	 */
	protected void processMouseEvent(NativeMouseEvent e) {
		int id = e.getID();

		EventListener[] listeners;
		if (id == NativeMouseEvent.NATIVE_MOUSE_MOVED || id == NativeMouseEvent.NATIVE_MOUSE_DRAGGED) {
			listeners = eventListeners.getListeners(NativeMouseMotionListener.class);
		}
		else {
			listeners = eventListeners.getListeners(NativeMouseListener.class);
		}

		for (int i = 0; i < listeners.length; i++) {
			switch (id) {
				case NativeMouseEvent.NATIVE_MOUSE_CLICKED:
					((NativeMouseListener) listeners[i]).nativeMouseClicked(e);
					break;

				case NativeMouseEvent.NATIVE_MOUSE_PRESSED:
					((NativeMouseListener) listeners[i]).nativeMousePressed(e);
					break;

				case NativeMouseEvent.NATIVE_MOUSE_RELEASED:
					((NativeMouseListener) listeners[i]).nativeMouseReleased(e);
					break;

				case NativeMouseEvent.NATIVE_MOUSE_MOVED:
					((NativeMouseMotionListener) listeners[i]).nativeMouseMoved(e);
					break;

				case NativeMouseEvent.NATIVE_MOUSE_DRAGGED:
					((NativeMouseMotionListener) listeners[i]).nativeMouseDragged(e);
					break;
			}
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
	 *
	 * @since 1.1
	 */
	protected void processMouseWheelEvent(NativeMouseWheelEvent e) {
		EventListener[] listeners = eventListeners.getListeners(NativeMouseWheelListener.class);

		for (int i = 0; i < listeners.length; i++) {
			((NativeMouseWheelListener) listeners[i]).nativeMouseWheelMoved(e);
		}
	}

	/**
	 * Initialize a local executor service for event delivery.  This method
	 * should only be called by the native library during the hook registration
	 * process.
	 *
	 * @since 1.1
	 */
	protected void startEventDispatcher() {
		// Create a new single thread executor.
		eventExecutor = Executors.newSingleThreadExecutor(new ThreadFactory() {
			public Thread newThread(Runnable r) {
				Thread t = new Thread(r);
				t.setName("JNativeHook Native Dispatch");
				
				return t;
			}
		});
	}

	/**
	 * Shutdown the local executor service for event delivery.  Any events
	 * events pending delivery will be discarded. This method should only be
	 * called by the native library during the hook deregistration process.
	 *
	 * @since 1.1
	 */
	protected void stopEventDispatcher() {
		if (eventExecutor != null) {
			// Shutdown the current Event executor.
			eventExecutor.shutdownNow();
			eventExecutor = null;
		}
	}

	/**
	 * Perform procedures to interface with the native library. These procedures
	 * include unpacking and loading the library into the Java Virtual Machine.
	 */
	protected static void loadNativeLibrary() {
		String libName = "JNativeHook";

		try {
			//Try to load the native library assuming the java.library.path was
			//set correctly at launch.
			System.loadLibrary(libName);
		}
		catch (UnsatisfiedLinkError linkError) {
			//The library is not in the java.library.path so try to extract it.
			try {
				String libResourcePath = "/org/jnativehook/lib/"
											+ NativeSystem.getFamily() + "/"
											+ NativeSystem.getArchitecture() + "/";

				//Get what the system "thinks" the library name should be.
				String libNativeName = System.mapLibraryName(libName);
				//Hack for OS X JRE  1.6 and earlier.
				libNativeName = libNativeName.replaceAll("\\.jnilib$", "\\.dylib");

				//Slice up the library name.
				int i = libNativeName.lastIndexOf('.');
				String libNativePrefix = libNativeName.substring(0, i) + '_';
				String libNativeSuffix = libNativeName.substring(i);

				//Create the temp file for this instance of the library.
				File libFile = File.createTempFile(libNativePrefix, libNativeSuffix);

				//Check and see if a copy of the native lib already exists.
				FileOutputStream libOutputStream = new FileOutputStream(libFile);
				byte[] buffer = new byte[4 * 1024];

				//This may return null in some circumstances.
				InputStream libInputStream =
								GlobalScreen.class.getResourceAsStream(
									libResourcePath.toLowerCase()
										+ libNativeName
								);

				if (libInputStream == null) {
					throw new IOException("Unable to locate the native library.");
				}

				int size;
				while ((size = libInputStream.read(buffer)) != -1) {
					libOutputStream.write(buffer, 0, size);
				}
				libOutputStream.close();
				libInputStream.close();

				libFile.deleteOnExit();

				System.load(libFile.getPath());
			}
			catch(IOException e) {
				//Tried and Failed to manually setup the java.library.path
				throw new RuntimeException(e.getMessage());
			}
		}
	}

	/**
	 * Perform procedures to cleanup the native library. This method is called
	 * on garbage collection to ensure proper native cleanup.
	 */
	protected static void unloadNativeLibrary() {
		//Make sure the native thread has stopped.
		unregisterNativeHook();
	}
}
