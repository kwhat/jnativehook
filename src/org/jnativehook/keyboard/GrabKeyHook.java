package org.jnativehook.keyboard;

//Imports
import java.io.File;
import java.lang.reflect.Field;
import java.net.URISyntaxException;

public class GrabKeyHook {
	/**
	 * Default Constructor
	 *
	 * This will attempt to locate the native shared objects by 
	 * looking in the same location as the calling jar.  
	 */
	public GrabKeyHook() throws NativeKeyException {
		//Setup default location to the pwd.
		String sLibPath = System.getProperty("user.dir", new File("").getAbsolutePath());
		
		try {
			//Try to locate our jar file.
			File objFile = new File(this.getClass().getProtectionDomain().getCodeSource().getLocation().toURI()).getAbsoluteFile();
			
			sLibPath = objFile.getPath();
			if (objFile.isFile()) {
				//Make sure we got a folder not a file.
				sLibPath = sLibPath.substring(0, sLibPath.length() - objFile.getName().length());
			}
		}
		catch (URISyntaxException e) { /* Do Nothing */ }
		
		loadLibrary(sLibPath);
	}
	
	protected void finalize() throws Throwable {
	    try {
	    	unregisterHook();
	    }
	    catch(Exception e) {
	        //Do Nothing
	    }
	    finally {
	        super.finalize();
	    }
	}
	
	/**
	 * Overloaded Constructor
	 *
	 * This allows the programmer to specify the explicit location 
	 * of the native libraries. 
	 * 
	 * @param sLibPath the location to look for the native library.
	 */
	public GrabKeyHook(String sLibPath) throws NativeKeyException {
		loadLibrary(sLibPath);
	}
	
	/**
	 * Attempts to load the native library based on the supplied path.
	 */
	private void loadLibrary(String sLibPath) throws NativeKeyException {
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
			System.loadLibrary("JNativeHook_Keyboard");
		}
		catch (Throwable e) {
			//Known exceptions are: NoSuchFieldException, IllegalArgumentException, IllegalAccessException, UnsatisfiedLinkError
			throw new NativeKeyException(e.getMessage());
		}
		
		//Register the hook.
		registerHook();
	}
	
	public native void grabKey(int iModifiers, int iKeyCode, int iKeyLocation) throws NativeKeyException;
	public native void ungrabKey(int iModifiers, int iKeyCode, int iKeyLocation) throws NativeKeyException;
	
	//These are basically the constructors and deconstructors for the hook.
	private native void registerHook();
	private native void unregisterHook();
}
