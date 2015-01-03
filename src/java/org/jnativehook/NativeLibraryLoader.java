package org.jnativehook;

/**
 * The Interface NativeLibraryLoader used to define custom way to load native library.
 * It instance must to be registered to GlobalScreen before use GlobalScreen.getInstance(). 
 */
public interface NativeLibraryLoader {

	/**
	 * Perform procedures to interface with the native library. These procedures
	 * include unpacking and loading the library into the Java Virtual Machine.
	 */
	public void load();
}
