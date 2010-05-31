/* Copyright (c) 2006-2010 - Alexander Barker (alex@1stleg.com)
 * 
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook;

//Imports
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.lang.reflect.Field;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import javax.swing.event.EventListenerList;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyException;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseEvent;
import org.jnativehook.mouse.NativeMouseListener;
import org.jnativehook.mouse.NativeMouseMotionListener;

/**
 * GlobalScreen is used to represent the native screen area that Java does not 
 * usually have access to. This class can be thought of as the source component 
 * for native events.  
 * <p>
 * This class also handles the loading, unpacking and communication with the 
 * native library. That includes registering new key and button hooks and the 
 * event dispatchers for each.
 * 
 * @author	Alex Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	1.0
 * @since	1.0
 */
public class GlobalScreen {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = 6504561173380322679L;
	
	/** The GlobalScreen singleton. */
	private static GlobalScreen instance = new GlobalScreen();
	
	/** The list of event listeners to notify. */
	private EventListenerList eventListeners;
	
	/**
	 * Private constructor to prevent multiple instances of the global screen.
	 * The {@link #registerHook} method will be called on construction to unpack 
	 * and load the native library.
	 */
	private GlobalScreen() {
		//Setup instance variables.
		eventListeners = new EventListenerList();
		
		//Unpack and Load the native library.
		GlobalScreen.registerHook();
	}
	
	/**
	 * A deconstructor that will perform native cleanup by calling the
	 * {@link #unregisterHook} method.  This method will not run until the
	 * class is garbage collected.
	 * 
	 * @throws Throwable The <code>Exception</code> raised by this method.
	 * @see Object#finalize
	 */
	protected void finalize() throws Throwable {
		try {
			GlobalScreen.unregisterHook();
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
	public static synchronized GlobalScreen getInstance() {
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
		eventListeners.add(NativeMouseListener.class, listener);
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
		eventListeners.remove(NativeMouseListener.class, listener);
	}
	
	/**
	 * Adds the specified native mouse motion listener to receive mouse motion 
	 * events from the native system. If listener is null, no exception is 
	 * thrown and no action is performed.
	 *
	 * @param listener the native mouse motion listener
	 */
	public void addNativeMouseMotionListener(NativeMouseMotionListener listener) {
		eventListeners.add(NativeMouseMotionListener.class, listener);
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
		eventListeners.remove(NativeMouseMotionListener.class, listener);
	}
	
	/**
	 * Gets the native keyboard auto repeat rate.
	 *
	 * @return the auto repeat rate in milliseconds
	 * @throws NativeKeyException the native key exception
	 */
	public native long getAutoRepeatRate() throws NativeKeyException;
	
	/**
	 * Gets the native keyboard auto repeat delay.
	 *
	 * @return the auto repeat delay in milliseconds
	 * @throws NativeKeyException the native key exception
	 */
	public native long getAutoRepeatDelay() throws NativeKeyException;
	
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
		Object[] objListeners = eventListeners.getListenerList();
		int id = e.getID();
		
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == NativeKeyListener.class ) {
				switch (id) {
					case NativeKeyEvent.NATIVE_KEY_PRESSED:
						((NativeKeyListener) objListeners[i + 1]).keyPressed(e);
					break;
					
					case NativeKeyEvent.NATIVE_KEY_RELEASED:
						((NativeKeyListener) objListeners[i + 1]).keyReleased(e);
					break;
				}
			}
		}
	}
	
	/**
	 * Processes native mouse events by dispatching them to all registered 
	 * <code>NativeMouseListener</code> objects.
	 * 
	 * @param e The <code>NativeMouseEvent</code> to dispatch.
	 * @see NativeKeyEvent
	 * @see NativeKeyListener
	 * @see #addNativeKeyListener(NativeKeyListener)
	 */
	protected void processMouseEvent(NativeMouseEvent e) {
		Object[] objListeners = eventListeners.getListenerList();
		int id = e.getID();
		
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == NativeMouseListener.class ) {
				switch (id) {
					case NativeMouseEvent.NATIVE_MOUSE_PRESSED:
						((NativeMouseListener) objListeners[i + 1]).mousePressed(e);
					break;
					
					case NativeMouseEvent.NATIVE_MOUSE_RELEASED:
						((NativeMouseListener) objListeners[i + 1]).mouseReleased(e);
					break;
				}
			}
			else if ( objListeners[ i ] == NativeMouseMotionListener.class ) {
				switch (id) {
					case NativeMouseEvent.NATIVE_MOUSE_MOVED:
						((NativeMouseMotionListener) objListeners[i + 1]).mouseMoved(e);
					break;
				}
			}
		}
	}
	
	
	/**
	 * Perform procedures to interface with the native library. These procedures 
	 * include unpacking and loading the library into the Java Virtual Machine.
	 */
	protected static void registerHook() {
		try {
			//Try to load the native library assuming the java.library.path was
			//set correctly at launch.
			System.loadLibrary("JNativeHook");
		}
		catch (UnsatisfiedLinkError linkError) {
			//The library is not in the java.library.path so try to extract it.
			try {
				//Try to locate the jar file
				String libPath = "org/jnativehook/lib/" + NativeSystem.getFamily().toString().toLowerCase() + "-" +	NativeSystem.getArchitecture().toString().toLowerCase();
				File classFile = new File(GlobalScreen.class.getProtectionDomain().getCodeSource().getLocation().toURI()).getAbsoluteFile();
				
				if (classFile.isFile()) {
					//Found the jar file to load.
					ZipInputStream zipInputStream = new ZipInputStream(new FileInputStream(classFile));
					ZipEntry zipEntry;
					
					while ( (zipEntry = zipInputStream.getNextEntry()) != null) {
						//Check all the entires for the lib path
						if (!zipEntry.isDirectory() && zipEntry.getName().toLowerCase().startsWith( libPath.toLowerCase() )) {
							String libName = zipEntry.getName().substring(zipEntry.getName().lastIndexOf('/'));
							File libFile = new File(System.getProperty("java.io.tmpdir") + System.getProperty("file.separator", File.separator) + libName);
							
							FileOutputStream tempLibOutputStream = new FileOutputStream(libFile);
							byte[] array = new byte[8192];
						    int read = 0;
						    while ( (read = zipInputStream.read(array)) > 0) {
						    	tempLibOutputStream.write(array, 0, read);
						    }
						    tempLibOutputStream.close();
						    
						    libFile.deleteOnExit();
						    System.load(libFile.getPath());
						}
					}
					zipInputStream.close();
				}
				else if (classFile.isDirectory()) {
					//Probably IDE environment, possible manual unpack.
					//Setup the java.library.path to the load path and attempt a lib load.
					File libFolder = new File(classFile.getAbsoluteFile() + "/" + libPath);

					if (libFolder.isDirectory()) {
						System.setProperty("java.library.path", System.getProperty("java.library.path", "") + System.getProperty("path.separator", ":") + libFolder.getPath());
						
						//Refresh the library path
						Field sysPath = ClassLoader.class.getDeclaredField("sys_paths");
						sysPath.setAccessible(true);
						if (sysPath != null) {
							sysPath.set(System.class.getClassLoader(), null);
						}
						
						//Try to load the native library
						System.loadLibrary("JNativeHook");
					}
				}
			}
			catch (Exception e) {
				throw new NativeHookException(e.getMessage(), e.getCause());
			}
		}
	}
	
	/**
	 * Perform procedures to cleanup the native library. This method should be 
	 * called on garbage collection to ensure proper native cleanup.
	 */
	protected static void unregisterHook() {
		//Do Nothing
	}
}
