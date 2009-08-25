package org.jnativegrab.keyboard;

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
	public GrabKeyHook() throws GrabKeyException {
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
	
	/**
	 * Overloaded Constructor
	 *
	 * This allows the programmer to specify the explicit location 
	 * of the native libraries. 
	 * 
	 * @param sLibPath the location to look for the native library.
	 */
	public GrabKeyHook(String sLibPath) throws GrabKeyException {
		loadLibrary(sLibPath);
	}
	
	/**
	 * Attempts to load the native library based on the supplied path.
	 */
	private void loadLibrary(String sLibPath) throws GrabKeyException {
		//Set the new lib path to system property.
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
			System.loadLibrary("JNativeGrab_Keyboard");
		}
		catch (Throwable e) {
			//Known exceptions are: NoSuchFieldException, IllegalArgumentException, IllegalAccessException, UnsatisfiedLinkError
			throw new GrabKeyException(e.getMessage());
		}
		
		//Register the hook.
		new Thread() {
			public void run() {
				//Start hook should block until stopHook is called.
				startHook();
				stopHook();
			}
		}.start();
	}
	
	//These are basically the constructors and deconstructors for the hook.
	public native void grabKey(GrabKeyEvent objEvent) throws GrabKeyException;
	public native void ungrabKey(GrabKeyEvent objEvent) throws GrabKeyException;
	
	private native void startHook();
	private native void stopHook();
}
