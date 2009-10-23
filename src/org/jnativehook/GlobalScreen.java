package org.jnativehook;

//Imports
import java.awt.Component;
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

public class GlobalScreen extends Component {
	//Instance Variables
	private static final long serialVersionUID = 6504561173380322679L;
	private static GlobalScreen instance = new GlobalScreen();
	private EventListenerList objEventListeners;
	
	private GlobalScreen() {
		//Setup instance variables.
		objEventListeners = new EventListenerList();
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
	
	public void addNativeKeyListener(NativeKeyListener objListener) {
		objEventListeners.add(NativeKeyListener.class, objListener);
	}
	
	public void removeNativeKeyListener(NativeKeyListener objListener) {
		objEventListeners.remove(NativeKeyListener.class, objListener);
	}
	
	public void addNativeButtonListener(NativeMouseListener objListener) {
		objEventListeners.add(NativeMouseListener.class, objListener);
	}
	
	public void removeNativeButtonListener(NativeMouseListener objListener) {
		objEventListeners.remove(NativeMouseListener.class, objListener);
	}
	
	//Native hooks to add and remove key bindings.
	public native void grabKey(int iModifiers, int iKeyCode, int iKeyLocation) throws NativeKeyException;
	public native void ungrabKey(int iModifiers, int iKeyCode, int iKeyLocation) throws NativeKeyException;
	public native void grabButton(int iButton) throws NativeKeyException;
	public native void ungrabButton(int iButton) throws NativeKeyException;
	
	//Get seme keyboard information
	public native long getAutoRepeatRate() throws NativeKeyException;
	public native long getAutoRepeatDelay() throws NativeKeyException;
	
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
	
	protected void fireMousePressed(NativeMouseEvent objEvent) {
		Object[] objListeners = objEventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == NativeMouseListener.class ) {
				((NativeMouseListener) objListeners[i + 1]).mousePressed( objEvent );
			}
		}
	}
	
	protected void fireMouseReleased(NativeMouseEvent objEvent) {
		Object[] objListeners = objEventListeners.getListenerList();
		for ( int i = 0; i < objListeners.length; i += 2 ) {
			if ( objListeners[ i ] == NativeMouseListener.class ) {
				((NativeMouseListener) objListeners[i + 1]).mouseReleased( objEvent );
			}
		}
	}
	
	protected static void registerHook() {
		try {
			//Try to locate the jar file
			String sLoadPath = "org/jnativehook/lib/" + OperatingSystem.getFamily().toString().toLowerCase() + "-" +	OperatingSystem.getArchitecture().toString().toLowerCase();
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
					    System.out.println("Found: " + objLibFile.getPath());
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
