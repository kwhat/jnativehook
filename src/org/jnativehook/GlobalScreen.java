package org.jnativehook;

//Imports
import java.awt.Component;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
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
	private static final long serialVersionUID = -3980582715467809283L;
	private static GlobalScreen instance = null;
	private EventListenerList objEventListeners;
	
	private GlobalScreen() {
		//Setup instance variables.
		objEventListeners = new EventListenerList();
		System.setProperty("sun.awt.enableExtraMouseButtons", "true");
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
	
	public static void registerHook() {
		try {
			String sLoadPath = "org/jnativehook/lib/" + OperatingSystem.getFamily() + "-" +	OperatingSystem.getArchitecture();
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
					
					//Linux: libJNativeGrab_Keyboard.so
					//Mac OSX: libJNativeGrab_Keyboard.so ?
					//Windows: JNativeGrab_Keyboard.dll
					System.loadLibrary("JNativeHook");
				}
			}
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
