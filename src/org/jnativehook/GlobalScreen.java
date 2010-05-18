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
 * GlobalScreen is used as a class to represent the global screen area that
 * Java does not usually have access to.  
 * <p>
 * This class also handles the loading and communication with the native 
 * library.  That includes registering new key and button hooks and the event 
 * dispatchers for each.
 * 
 * @author	Alex Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	0.9
 * @since	1.0
 */

public class GlobalScreen {
	//Instance Variables
	private static final long serialVersionUID = 6504561173380322679L;
	private static GlobalScreen instance = new GlobalScreen();
	private EventListenerList eventListeners;
	
	private GlobalScreen() {
		//Setup instance variables.
		eventListeners = new EventListenerList();
		//TODO this needs to be passed in to java -D and not like this.
		//System.setProperty("sun.awt.enableExtraMouseButtons", "true");
		GlobalScreen.registerHook();
	}
	
	/**
	 * DeConstructor
	 *
	 * This will attempt to run some of the native cleanup 
	 * when the class is garbage collected.
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

	public static synchronized GlobalScreen getInstance() {
		return GlobalScreen.instance;
	}
	
	public void addKeyListener(NativeKeyListener listener) {
		eventListeners.add(NativeKeyListener.class, listener);
	}
	
	public void removeKeyListener(NativeKeyListener listener) {
		eventListeners.remove(NativeKeyListener.class, listener);
	}
	
	public void addMouseMotionListener(NativeMouseMotionListener listener) {
		eventListeners.add(NativeMouseMotionListener.class, listener);
	}
	
	public void removeMouseMotionListener(NativeMouseMotionListener listener) {
		eventListeners.remove(NativeMouseMotionListener.class, listener);
	}
	
	public void addMouseListener(NativeMouseListener listener) {
		eventListeners.add(NativeMouseListener.class, listener);
	}
	
	public void removeMouseListener(NativeMouseListener listener) {
		eventListeners.remove(NativeMouseListener.class, listener);
	}
	
	//Get some keyboard information
	public native long getAutoRepeatRate() throws NativeKeyException;
	public native long getAutoRepeatDelay() throws NativeKeyException;
	
	protected void fireKeyPressed(NativeKeyEvent e) {
		Object[] objListeners = eventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == NativeKeyListener.class ) {
				((NativeKeyListener) objListeners[i + 1]).keyPressed(e);
			}
		}
	}
	
	protected void fireKeyReleased(NativeKeyEvent e) {
		Object[] objListeners = eventListeners.getListenerList();
		for ( int i = 0; i < objListeners.length; i += 2 ) {
			if ( objListeners[ i ] == NativeKeyListener.class ) {
				((NativeKeyListener) objListeners[i + 1]).keyReleased(e);
			}
		}
	}
	
	protected void fireMouseMoved(NativeMouseEvent e) {
		Object[] objListeners = eventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == NativeMouseMotionListener.class ) {
				((NativeMouseMotionListener) objListeners[i + 1]).mouseMoved(e);
			}
		}
	}
	
	protected void fireMousePressed(NativeMouseEvent e) {
		Object[] objListeners = eventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == NativeMouseListener.class ) {
				((NativeMouseListener) objListeners[i + 1]).mousePressed(e);
			}
		}
	}
	
	protected void fireMouseReleased(NativeMouseEvent e) {
		Object[] objListeners = eventListeners.getListenerList();
		for ( int i = 0; i < objListeners.length; i += 2 ) {
			if ( objListeners[ i ] == NativeMouseListener.class ) {
				((NativeMouseListener) objListeners[i + 1]).mouseReleased(e);
			}
		}
	}
	
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
	
	protected static void unregisterHook() {
		//Do Nothing
	}
}
