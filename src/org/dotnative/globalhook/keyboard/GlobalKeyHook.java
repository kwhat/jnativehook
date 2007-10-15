package org.dotnative.globalhook.keyboard;


//KeyboardHook.java
//This class appears to handle all of our event listening
import java.awt.event.KeyEvent;
import java.io.File;
import java.lang.reflect.Field;

import javax.swing.event.EventListenerList;

public class GlobalKeyHook {
	private EventListenerList objEventListeners;
	
	public GlobalKeyHook() throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException {
		this(System.getProperty("user.dir", new File("").getAbsolutePath()));
	}
	
	public GlobalKeyHook(String sLibPath) throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException {
		objEventListeners = new EventListenerList();
		
		//Dynamic Loading of Library
		//Add the applications current path to the library path so
		//we dont have to pass dirty arguments to the vm on the cli
		System.setProperty("java.library.path", System.getProperty("java.library.path", "") + System.getProperty("path.separator", ":") + sLibPath);
		Field objSysPath = ClassLoader.class.getDeclaredField("sys_paths");
		objSysPath.setAccessible(true);
		if (objSysPath != null) {
			objSysPath.set(System.class.getClassLoader(), null);
		}
		//Linux: libGlobalKeyListener.so
		//Mac OSX: libGlobalKeyListener.so ?
		//Windows: GlobalKeyListener.dll
		System.loadLibrary("GlobalKeyListener");
		
		//Register the hook inside a thread.
		new Thread() {
			public void run() {
				registerHook();
			}
		}.start();
	}
	
	public void addEventListener(GlobalKeyListener objListener) {
		objEventListeners.add(GlobalKeyListener.class, objListener);
	}
	
	public void removeEventListener(GlobalKeyListener objListener) {
		objEventListeners.remove(GlobalKeyListener.class, objListener);
	}
	
	//Notify Listners Key Down
	void fireKeyPressed(long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		Object[] objListeners = objEventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if (objListeners[i] == GlobalKeyListener.class) {
				System.out.println(iWhen + "\t" + iModifiers + "\t" + iKeyCode + "\t" + cKeyChar);
				((GlobalKeyListener) objListeners[i + 1]).keyPressed( new GlobalKeyEvent(null, KeyEvent.KEY_PRESSED, iWhen, iModifiers, iKeyCode, (char)iKeyCode) );
			}
		}
	}
	
	//Notify Listners Key Up
	void fireKeyReleased(long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		Object[] objListeners = objEventListeners.getListenerList();
		for ( int i = 0; i < objListeners.length; i += 2 ) {
			if ( objListeners[ i ] == GlobalKeyListener.class ) {
				( (GlobalKeyListener)objListeners[i + 1] ).keyReleased( new GlobalKeyEvent(null, KeyEvent.KEY_RELEASED, iWhen, iModifiers, iKeyCode, cKeyChar) );
			}
		}
	}
	
	//Notify Listners Key Up
	void fireKeyTyped(long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		Object[] objListeners = objEventListeners.getListenerList();
		for ( int i = 0; i < objListeners.length; i += 2 ) {
			if ( objListeners[i] == GlobalKeyListener.class ) {
				( (GlobalKeyListener)objListeners[i + 1] ).keyPressed( new GlobalKeyEvent(null, KeyEvent.KEY_TYPED, iWhen, iModifiers, iKeyCode, cKeyChar) );
			}
		}
	}
	
	//These are basically the constructors and deconstructors for the hook.
	native void registerHook();
	native void unregisterHook();
}
