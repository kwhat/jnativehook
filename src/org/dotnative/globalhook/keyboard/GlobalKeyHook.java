package org.dotnative.globalhook.keyboard;


//KeyboardHook.java
//This class appears to handle all of our event listening
import java.io.File;
import java.lang.reflect.Field;

public class GlobalKeyHook extends Thread {
	public GlobalKeyHook() throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException {
		//Dynamic Loading of Library
		//Add the applications current path to the library path so
		//we dont have to pass dirty arguments to the vm on the cli
		System.setProperty("java.library.path", System.getProperty("user.dir", new File("").getAbsolutePath()) + System.getProperty("path.separator", ":") + System.getProperty("java.library.path", ""));
		Field objSysPath = ClassLoader.class.getDeclaredField("sys_paths");
		objSysPath.setAccessible(true);
		if (objSysPath != null) {
			objSysPath.set(System.class.getClassLoader(), null);
		}
		//Linux: libGlobalKeyListener.so
		//Mac OSX: libGlobalKeyListener.so ?
		//Windows: GlobalKeyListener.dll
		System.loadLibrary("GlobalKeyListener");
		
		this.start();
	}
	
	public GlobalKeyHook(String sLibPath) throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException {
		//Dynamic Loading of Library
		//Add the applications current path to the library path so
		//we dont have to pass dirty arguments to the vm on the cli
		System.setProperty("java.library.path", sLibPath + System.getProperty("path.separator", ":") + System.getProperty("java.library.path", ""));
		Field objSysPath = ClassLoader.class.getDeclaredField("sys_paths");
		objSysPath.setAccessible(true);
		if (objSysPath != null) {
			objSysPath.set(System.class.getClassLoader(), null);
		}
		//Linux: libGlobalKeyListener.so
		//Mac OSX: libGlobalKeyListener.so ?
		//Windows: GlobalKeyListener.dll
		System.loadLibrary("GlobalKeyListener");
		
		this.start();
	}
	
	protected javax.swing.event.EventListenerList listenerList = new javax.swing.event.EventListenerList();
	
	public void addEventListener(GlobalKeyListener listener) {
		listenerList.add(GlobalKeyListener.class, listener);
	}
	
	public void removeEventListener(GlobalKeyListener listener) {
		listenerList.remove(GlobalKeyListener.class, listener);
	}
	
	//Notify Listners Key Down
	void fireKeyPressed(GlobalKeyEvent event) {
		Object[] listeners = listenerList.getListenerList();
		for ( int i = 0; i < listeners.length; i += 2 ) {
			if ( listeners[ i ] == GlobalKeyListener.class ) {
				( (GlobalKeyListener)listeners[i + 1] ).keyPressed( event );
			}
		}
	}
	
	//Notify Listners Key Up
	void fireKeyReleased(GlobalKeyEvent event) {
		Object[] listeners = listenerList.getListenerList();
		for ( int i = 0; i < listeners.length; i += 2 ) {
			if ( listeners[ i ] == GlobalKeyListener.class ) {
				( (GlobalKeyListener)listeners[i + 1] ).keyReleased( event );
			}
		}
	}
	
	//Notify Listners Key Up
	void fireKeyTyped(GlobalKeyEvent event) {
		Object[] listeners = listenerList.getListenerList();
		for ( int i = 0; i < listeners.length; i += 2 ) {
			if ( listeners[ i ] == GlobalKeyListener.class ) {
				( (GlobalKeyListener)listeners[i + 1] ).keyPressed( event );
			}
		}
	}
}
