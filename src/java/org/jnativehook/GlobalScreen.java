/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2012 Alexander Barker.  All Rights Received.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook;

//Imports
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.net.URISyntaxException;
import java.util.EventListener;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import javax.swing.SwingUtilities;
import javax.swing.event.EventListenerList;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseListener;
import org.jnativehook.mouse.NativeMouseMotionListener;
import org.jnativehook.mouse.NativeMouseWheelEvent;
import org.jnativehook.mouse.NativeMouseWheelListener;

/**
 * GlobalScreen is used to represent the native screen area that Java does not 
 * usually have access to. This class can be thought of as the source component 
 * for native events.  
 * <p>
 * This class also handles the loading, unpacking and communication with the 
 * native library. That includes registering new key and button hooks and the 
 * event dispatchers for each.
 * 
 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.1
 * @since	1.0
 */
public class GlobalScreen {
	/** The GlobalScreen singleton. */
	private static final GlobalScreen instance = new GlobalScreen();
	
	/** The list of event listeners to notify. */
	private EventListenerList eventListeners;
	
	/**
	 * Private constructor to prevent multiple instances of the global screen.
	 * The {@link #registerNativeHook} method will be called on construction to unpack
	 * and load the native library.
	 */
	private GlobalScreen() {
		//Enable extra mouse button support in java.
		System.setProperty("sun.awt.enableExtraMouseButtons", "true");
		
		//Setup instance variables.
		eventListeners = new EventListenerList();
		
		//Unpack and Load the native library.
		GlobalScreen.loadNativeLibrary();
	}
	
	/**
	 * A deconstructor that will perform native cleanup by calling the
	 * {@link #unregisterNativeHook} method.  This method will not run until the
	 * class is garbage collected.
	 *
	 * @throws Throwable The <code>Exception</code> raised by this method.
	 * @see Object#finalize
	 */
	@Override
	protected void finalize() throws Throwable {
		try {
			GlobalScreen.unloadNativeLibrary();
		}
		catch(Exception e) {
			//Do Nothing
		}
		finally {
			super.finalize();
		}
	}

	/**
	 * Gets the single instance of GlobalScreen.
	 *
	 * @return single instance of GlobalScreen
	 */
	public static GlobalScreen getInstance() {
		return GlobalScreen.instance;
	}
	
	/**
	 * Adds the specified native key listener to receive key events from the 
	 * native system. If listener is null, no exception is thrown and no action 
	 * is performed.
	 *
	 * @param listener the native key listener
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
	 * @param listener the native key listener
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
	 * @param listener the native mouse listener
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
	 * @param listener the native mouse listener
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
	 * @param listener the native mouse motion listener
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
	 * @param listener the native mouse motion listener
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
	 * @param listener the native mouse wheel listener
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
	 * @param listener the native mouse wheel listener
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
	 * the function has no effect. <b>Note that this method may block the AWT
	 * event dispatching thread.</b> It is recomended to call this method from
	 * outside the scope of the graphical user interface event queue.
	 *
	 * @throws NativeHookException the native hook exception
	 * 
	 * @since 1.1
	 */
	public native void registerNativeHook() throws NativeHookException;
	
	/**
	 * Disable the native hook if it is currently running. If it is not running
	 * the function has no effect. <b>Note that this method may block the AWT
	 * event dispatching thread.</b> It is recomended to call this method from
	 * outside the scope of the graphical user interface event queue.
	 *
	 * @throws NativeHookException the native hook exception
	 * 
	 * @since 1.1
	 */
	public native void unregisterNativeHook() throws NativeHookException;
	
	/**
	 * Gets the current state of the native hook.
	 *
	 * @return the state of the native hook.
	 * @throws NativeHookException the native hook exception
	 * 
	 * @since 1.1
	 */
	public native boolean isNativeHookRegistered();
	
	
	/**
	 * Dispatches an event to the appropriate processor.  This method is 
	 * generally called by the native library but maybe used to synthesize 
	 * native events from Java.
	 * 
	 * @param e the native input event
	 */
	public final void dispatchEvent(NativeInputEvent e) {
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
	
	/**
	 * Processes native key events by dispatching them to all registered 
	 * <code>NativeKeyListener</code> objects.
	 * 
	 * @param e The <code>NativeKeyEvent</code> to dispatch.
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
					if (SwingUtilities.isEventDispatchThread()) {
						SwingUtilities.invokeLater(new AWTDispatchRunnable(listeners[i], e) {
							public void run() {
								((NativeKeyListener) this.getListener()).keyPressed((NativeKeyEvent) this.getEvent());
							}
						});
					}
					else {
						((NativeKeyListener) listeners[i]).keyPressed(e);
					}
				break;

				case NativeKeyEvent.NATIVE_KEY_TYPED:
					if (SwingUtilities.isEventDispatchThread()) {
						SwingUtilities.invokeLater(new AWTDispatchRunnable(listeners[i], e) {
							public void run() {
								((NativeKeyListener) this.getListener()).keyTyped((NativeKeyEvent) this.getEvent());
							}
						});
					}
					else {
						((NativeKeyListener) listeners[i]).keyTyped(e);
					}
				break;
				
				case NativeKeyEvent.NATIVE_KEY_RELEASED:
					if (SwingUtilities.isEventDispatchThread()) {
						SwingUtilities.invokeLater(new AWTDispatchRunnable(listeners[i], e) {
							public void run() {
								((NativeKeyListener) this.getListener()).keyReleased((NativeKeyEvent) this.getEvent());
							}
						});
					}
					else {
						((NativeKeyListener) listeners[i]).keyReleased(e);
					}
				break;
			}
		}
	}
	
	/**
	 * Processes native mouse events by dispatching them to all registered 
	 * <code>NativeMouseListener</code> objects.
	 * 
	 * @param e The <code>NativeMouseEvent</code> to dispatch.
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
					if (SwingUtilities.isEventDispatchThread()) {
						SwingUtilities.invokeLater(new AWTDispatchRunnable(listeners[i], e) {
							public void run() {
								((NativeMouseListener) this.getListener()).mouseClicked((NativeMouseEvent) this.getEvent());
							}
						});
					}
					else {
						((NativeMouseListener) listeners[i]).mouseClicked(e);
					}
				break;

				case NativeMouseEvent.NATIVE_MOUSE_PRESSED:
					if (SwingUtilities.isEventDispatchThread()) {
						SwingUtilities.invokeLater(new AWTDispatchRunnable(listeners[i], e) {
							public void run() {
								((NativeMouseListener) this.getListener()).mousePressed((NativeMouseEvent) this.getEvent());
							}
						});
					}
					else {
						((NativeMouseListener) listeners[i]).mousePressed(e);
					}
				break;
				
				case NativeMouseEvent.NATIVE_MOUSE_RELEASED:
					if (SwingUtilities.isEventDispatchThread()) {
						SwingUtilities.invokeLater(new AWTDispatchRunnable(listeners[i], e) {
							public void run() {
								((NativeMouseListener) this.getListener()).mouseReleased((NativeMouseEvent) this.getEvent());
							}
						});
					}
					else {
						((NativeMouseListener) listeners[i]).mouseReleased(e);
					}
				break;
				
				case NativeMouseEvent.NATIVE_MOUSE_MOVED:
					if (SwingUtilities.isEventDispatchThread()) {
						SwingUtilities.invokeLater(new AWTDispatchRunnable(listeners[i], e) {
							public void run() {
								((NativeMouseMotionListener) this.getListener()).mouseMoved((NativeMouseEvent) this.getEvent());
							}
						});
					}
					else {
						((NativeMouseMotionListener) listeners[i]).mouseMoved(e);
					}
				break;

				case NativeMouseEvent.NATIVE_MOUSE_DRAGGED:
					if (SwingUtilities.isEventDispatchThread()) {
						SwingUtilities.invokeLater(new AWTDispatchRunnable(listeners[i], e) {
							public void run() {
								((NativeMouseMotionListener) this.getListener()).mouseDragged((NativeMouseEvent) this.getEvent());
							}
						});
					}
					else {
						((NativeMouseMotionListener) listeners[i]).mouseDragged(e);
					}
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
			if (SwingUtilities.isEventDispatchThread()) {
				SwingUtilities.invokeLater(new AWTDispatchRunnable(listeners[i], e) {
					public void run() {
						((NativeMouseWheelListener) this.getListener()).mouseWheelMoved((NativeMouseWheelEvent) this.getEvent());
					}
				});
			}
			else {
				((NativeMouseWheelListener) listeners[i]).mouseWheelMoved(e);
			}
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
				//Try to locate the jar file
				String jarLibPath =
						"org/jnativehook/lib/" +
						NativeSystem.getFamily().toString().toLowerCase() + "/" +
						NativeSystem.getArchitecture().toString().toLowerCase() + "/";
				
				File classFile = new File(GlobalScreen.class.getProtectionDomain().getCodeSource().getLocation().toURI()).getAbsoluteFile();

				if (classFile.isFile()) {
					//Load the jar file and get the lib entry.
					JarFile jarFile = new JarFile(classFile);
					JarEntry jarLibEntry = jarFile.getJarEntry(jarLibPath + System.mapLibraryName(libName));

					//Create a temp lib file in the systems tmp folder.
					File tmpLibFile = new File(System.getProperty("java.io.tmpdir") + System.getProperty("file.separator", File.separator) + System.mapLibraryName(libName));

					//Extract the lib from inside of the jar file.
					InputStream jarInputStream = jarFile.getInputStream(jarLibEntry);
					FileOutputStream tempLibOutputStream = new FileOutputStream(tmpLibFile);
					
					byte[] array = new byte[8192];
					int read = 0;
					while ( (read = jarInputStream.read(array)) > 0) {
						tempLibOutputStream.write(array, 0, read);
					}
					tempLibOutputStream.close();
					
					tmpLibFile.deleteOnExit();
					System.load(tmpLibFile.getPath());
				}
				else if (classFile.isDirectory()) {
					//Probably IDE environment, possible manual unpack.
					//Setup the java.library.path to the load path and attempt a lib load.
					File libFolder = new File(classFile.getAbsoluteFile() + "/" + jarLibPath);

					if (libFolder.isDirectory()) {
						System.setProperty("java.library.path", System.getProperty("java.library.path", ".") + System.getProperty("path.separator", ":") + libFolder.getPath());
						
						//Refresh the library path
						Field sysPath = ClassLoader.class.getDeclaredField("sys_paths");
						sysPath.setAccessible(true);
						if (sysPath != null) {
							sysPath.set(System.class.getClassLoader(), null);
						}
						
						//Try to load the native library
						System.loadLibrary(libName);
					}
				}
			}
			catch (URISyntaxException e) {
				//Tried and Failed to unpak the JAR container.
				throw new RuntimeException(e.getMessage());
			}
			catch (Exception e) {
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
		try {
			//Make sure the native thread has stopped.
			instance.unregisterNativeHook();
		}
		catch (NativeHookException e) {
			e.printStackTrace();
		}
	}
	
	
	/**
	 * AWTDispatchRunnable is a small abstract inner class used for dispatching  
	 * events that may conflict with the AWT Event dispatching thread.  This 
	 * custom runnable allows <code>SwingUtilities.invokeLater(java.lang.Runnable)</code> 
	 * to be called without requring that event objects be declared final. 
	 * 
	 * @author	Alexander Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
	 * @since	1.1
	 * 
	 * @see javax.swing.SwingUtilities#invokeLater(java.lang.Runnable)
	 */
	private abstract class AWTDispatchRunnable implements Runnable {
		/** The EventListener to send events to. */
		private EventListener listener;
		
		/** The NativeInputEvent to send out. */
		private NativeInputEvent event;
		
		/**
		 * Instantiates a new <code>AWTDispatchRunnable</code> for use with 
		 * <code>SwingUtilities.invokeLater()</code>.
		 * 
		 * @param listener The listener to dispatch events to.
		 * @param e The event to dispatch.
		 */
		public AWTDispatchRunnable(EventListener listener, NativeInputEvent e) {
			this.listener = listener;
			this.event = e;
		}
		
		/**
		 * Gets the event listener.
		 *
		 * @return the event listener
		 */
		public EventListener getListener() {
			return listener;
		}
		
		/**
		 * Gets the event.
		 *
		 * @return the event
		 */
		public NativeInputEvent getEvent() {
			return event;
		}
	}
}
