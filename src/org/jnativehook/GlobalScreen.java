package org.jnativehook;

//Imports
import java.awt.Component;
import java.io.File;
import java.lang.reflect.Field;
import java.net.URISyntaxException;

import javax.swing.event.EventListenerList;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyException;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseListener;

public class GlobalScreen extends Component {
	//Instance Variables
	private static final long serialVersionUID = -3980582715467809283L;
	private static GlobalScreen instance = null;
	private EventListenerList objEventListeners;
	
	private GlobalScreen() {
		//Setup instance variables.
		objEventListeners = new EventListenerList();
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
		if (GlobalScreen.instance == null) {
			GlobalScreen.instance = new GlobalScreen();
		}
		
		return GlobalScreen.instance;
	}
	
	public void addGrabKeyListener(NativeKeyListener objListener) {
		objEventListeners.add(NativeKeyListener.class, objListener);
	}
	
	public void removeGrabKeyListener(NativeKeyListener objListener) {
		objEventListeners.remove(NativeKeyListener.class, objListener);
	}
	
	public void addGrabButtonListener(NativeMouseListener objListener) {
		objEventListeners.add(NativeMouseListener.class, objListener);
	}
	
	public void removeGrabButtonListener(NativeMouseListener objListener) {
		objEventListeners.remove(NativeMouseListener.class, objListener);
	}
	
	//Native hooks to add and remove key bindings.
	public native void grabKey(int iModifiers, int iKeyCode, int iKeyLocation) throws NativeKeyException;
	public native void ungrabKey(int iModifiers, int iKeyCode, int iKeyLocation) throws NativeKeyException;
	
	
	protected void fireKeyPressed(NativeKeyEvent objEvent) {
		Object[] objListeners = objEventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == NativeKeyListener.class ) {
				((NativeKeyListener) objListeners[i + 1]).keyPressed( objEvent );
			}
		}
	}
	
	protected void fireKeyReleased(NativeKeyEvent objEvent) {
		Object[] objListeners = objEventListeners.getListenerList();
		for ( int i = 0; i < objListeners.length; i += 2 ) {
			if ( objListeners[ i ] == NativeKeyListener.class ) {
				((NativeKeyListener) objListeners[i + 1]).keyReleased( objEvent );
			}
		}
	}
	

	public static void registerHook() {
		//Setup default location to the pwd.
		String sLibPath = System.getProperty("user.dir", new File("").getAbsolutePath());
		
		try {
			//Try to locate our jar file.
			
			File objFile = new File(GlobalScreen.class.getProtectionDomain().getCodeSource().getLocation().toURI()).getAbsoluteFile();
			
			sLibPath = objFile.getPath();
			if (objFile.isFile()) {
				//Make sure we got a folder not a file.
				sLibPath = sLibPath.substring(0, sLibPath.length() - objFile.getName().length());
			}
		}
		catch (URISyntaxException e) { /* Do Nothing */ }
		
		//Set the new library path to system property.
		System.setProperty("java.library.path", System.getProperty("java.library.path", "") + System.getProperty("path.separator", ":") + sLibPath);
		
		try {
			//Try to load the library.
			Field objSysPath = ClassLoader.class.getDeclaredField("sys_paths");
			objSysPath.setAccessible(true);
			if (objSysPath != null) {
				objSysPath.set(System.class.getClassLoader(), null);
			}
			
			//Linux: libJNativeGrab_Keyboard.so
			//Mac OSX: libJNativeGrab_Keyboard.so ?
			//Windows: JNativeGrab_Keyboard.dll
			System.loadLibrary("JNativeHook");
		}
		catch (Throwable e) {
			//Known exceptions are: NoSuchFieldException, IllegalArgumentException, IllegalAccessException, UnsatisfiedLinkError
			throw new NativeHookException(e);
		}
		
		//Register the hook.
		initialize();
	}
	
	public static void unregisterHook() {
		deinitialize();
	}
	
	//These are basically the constructors and deconstructors for the native hook.
	private native static void initialize();
	private native static void deinitialize();
}
