/* Copyright (c) 2007-2010 - Alex Barker (alex@1stleg.com)
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
import java.awt.Component;
import java.awt.event.MouseListener;
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
 * Java does not usually have access to.  This allows our native events to 
 * extend Java's InputEvent class and maintain compatability between Java input 
 * events and native input events.
 * <p>
 * This class also hanldes the loading and communicaiton with the native 
 * library.  That includes registering new key and button hooks and the event 
 * dispatchers for each.
 * 
 * @author	Alex Barker (<a href="mailto:alex@1stleg.com">alex@1stleg.com</a>)
 * @version	0.9
 * @since	1.0
 */

public class GlobalScreen extends Component {
	//Instance Variables
	private static final long serialVersionUID = 6504561173380322679L;
	private static GlobalScreen instance = new GlobalScreen();
	private EventListenerList eventListeners;
	
	private GlobalScreen() {
		//Setup instance variables.
		eventListeners = new EventListenerList();
		System.setProperty("sun.awt.enableExtraMouseButtons", "true");
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
	
	public void addNativeKeyListener(NativeKeyListener listener) {
		eventListeners.add(NativeKeyListener.class, listener);
	}
	
	public void removeNativeKeyListener(NativeKeyListener listener) {
		eventListeners.remove(NativeKeyListener.class, listener);
	}
	
	public void addNativeMouseMotionListener(NativeMouseMotionListener listener) {
		eventListeners.add(NativeMouseMotionListener.class, listener);
	}
	
	public void removeNativeMouseMotionListener(NativeMouseMotionListener listener) {
		eventListeners.remove(NativeMouseMotionListener.class, listener);
	}
	
	public void addNativeMouseListener(NativeMouseListener listener) {
		eventListeners.add(NativeMouseListener.class, listener);
	}
	
	public void removeNativeMouseListener(NativeMouseListener listener) {
		eventListeners.remove(NativeMouseListener.class, listener);
	}
	
	//TODO TEST
	public void addMouseListener(MouseListener listener) {
		addNativeMouseListener((NativeMouseListener) listener);
	}
	
	
	
	//Get seme keyboard information
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
	
	protected void fireKeyTyped(NativeKeyEvent e) {
		Object[] objListeners = eventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == NativeKeyListener.class ) {
				((NativeKeyListener) objListeners[i + 1]).keyTyped(e);
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
	
	protected void fireMouseClicked(NativeMouseEvent e) {
		Object[] objListeners = eventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == NativeMouseListener.class ) {
				((NativeMouseListener) objListeners[i + 1]).mouseClicked(e);
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
			//Try to locate the jar file
			String sLoadPath = "org/jnativehook/lib/" + NativeSystem.getFamily().toString().toLowerCase() + "-" +	NativeSystem.getArchitecture().toString().toLowerCase();
			File objCode = new File(GlobalScreen.class.getProtectionDomain().getCodeSource().getLocation().toURI()).getAbsoluteFile();
			
			if (objCode.isFile()) {
				//Found the jar file to load.
				ZipInputStream objZipInputStream = new ZipInputStream(new FileInputStream(objCode));
				ZipEntry objEntry;
				
				while ( (objEntry = objZipInputStream.getNextEntry()) != null) {
					if (!objEntry.isDirectory() && objEntry.getName().toLowerCase().startsWith( sLoadPath.toLowerCase() )) {
						String sFileName = objEntry.getName().substring(objEntry.getName().lastIndexOf('/'));
						File objLibFile = new File(System.getProperty("java.io.tmpdir") + System.getProperty("file.separator", File.separator) + sFileName);
						
						FileOutputStream objTempLibOutputStream = new FileOutputStream(objLibFile);
						byte[] array = new byte[8192];
					    int read = 0;
					    while ( (read = objZipInputStream.read(array)) > 0) {
					    	objTempLibOutputStream.write(array, 0, read);
					    }
					    objTempLibOutputStream.close();
					    
					    objLibFile.deleteOnExit();
					    System.load(objLibFile.getPath());
					}
				}
				objZipInputStream.close();
			}
			else if (objCode.isDirectory()) {
				//Probably IDE enviroment, possible manual unpack.
				//Setup the java.library.path to the load path and attempt a lib load.
				File objLibFolder = new File(objCode.getAbsoluteFile() + "/" + sLoadPath);
				if (objLibFolder.isDirectory()) {
					System.setProperty("java.library.path", System.getProperty("java.library.path", "") + System.getProperty("path.separator", ":") + objLibFolder.getPath());
					
					//Refresh the library path
					Field objSysPath = ClassLoader.class.getDeclaredField("sys_paths");
					objSysPath.setAccessible(true);
					if (objSysPath != null) {
						objSysPath.set(System.class.getClassLoader(), null);
					}
					
					//Try to load the native library
					System.loadLibrary("JNativeHook");
				}
			}
		}
		catch (Throwable e) {
			//Known exceptions are: NoSuchFieldException, IllegalArgumentException, IllegalAccessException, UnsatisfiedLinkError
			throw new NativeHookException(e);
		}
	}
	
	protected static void unregisterHook() {
		//GlobalScreen.deinitialize();
	}
}
