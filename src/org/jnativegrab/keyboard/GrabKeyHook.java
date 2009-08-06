package org.jnativegrab.keyboard;

import java.awt.event.KeyEvent;
import java.io.File;
import java.lang.reflect.Field;
import java.net.URISyntaxException;
import java.util.ArrayList;
import javax.swing.event.EventListenerList;

import org.jnativegrab.GlobalScreen;

public class GrabKeyHook {
	private EventListenerList objEventListeners;
	private ArrayList<Integer> objKeysDown;
	private GlobalScreen objScreen;
	
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
		
		//Set the new lib path to system property.
		System.setProperty("java.library.path", System.getProperty("java.library.path", "") + System.getProperty("path.separator", ":") + sLibPath);
		
		init();
	}
	
	public GrabKeyHook(String sLibPath) throws GrabKeyException {
		//Dynamic Loading of Library
		//Add the applications current path to the library path so
		//we can find the binary lib
		System.setProperty("java.library.path", System.getProperty("java.library.path", "") + System.getProperty("path.separator", ":") + sLibPath);
		
		init();
	}
	
	private void init() throws GrabKeyException {
		objEventListeners = new EventListenerList();
		objKeysDown = new ArrayList<Integer>();
		objScreen = new GlobalScreen();
		
		try {
			//Try to load the library.
			Field objSysPath = ClassLoader.class.getDeclaredField("sys_paths");
			objSysPath.setAccessible(true);
			if (objSysPath != null) {
				objSysPath.set(System.class.getClassLoader(), null);
			}
			
			//Linux: libGlobalHook_Keyboard.so
			//Mac OSX: libGlobalHook_Keyboard.so ?
			//Windows: GlobalHook_Keyboard.dll
			System.loadLibrary("GlobalHook_Keyboard");
		}
		catch (Throwable e) {
			//Known exceptions are: NoSuchFieldException, IllegalArgumentException, IllegalAccessException, UnsatisfiedLinkError
			throw new GrabKeyException(e.getMessage());
		}
		
		//Register the hook.
		registerHook();
		
		new Thread() {
			public void run() {
				//Start hook should block untill stopHook is called.
				startHook();
				stopHook();
				unregisterHook();
			}
		}.start();
	}
	
	public void addEventListener(GrabKeyListener objListener) {
		objEventListeners.add(GrabKeyListener.class, objListener);
	}
	
	public void removeEventListener(GrabKeyListener objListener) {
		objEventListeners.remove(GrabKeyListener.class, objListener);
	}
	
	//Notify Listners Key Down
	@SuppressWarnings("unused")
	private void fireKeyPressed(long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		int iCharIndex = objKeysDown.indexOf(iKeyCode);
		
		if (iCharIndex == -1) {
			objKeysDown.add(iKeyCode);
		}
		
		Object[] objListeners = objEventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if (objListeners[i] == GrabKeyListener.class) {
				switch (iCharIndex) {
					case -1:
						//TODO Figure out why KeyEvent.KEY_TYPED doesnt work.
						//((GlobalKeyListener) objListeners[i + 1]).keyPressed( new GlobalKeyEvent(objScreen, KeyEvent.KEY_TYPED, iWhen, iModifiers, iKeyCode, cKeyChar) );
						((GrabKeyListener) objListeners[i + 1]).keyPressed( new GrabKeyEvent(objScreen, KeyEvent.KEY_PRESSED, iWhen, iModifiers, iKeyCode, cKeyChar) );
					default:
						((GrabKeyListener) objListeners[i + 1]).keyTyped( new GrabKeyEvent(objScreen, KeyEvent.KEY_PRESSED, iWhen, iModifiers, iKeyCode, cKeyChar) );
				}
			}
		}
	}
	
	//Notify Listeners Key Up
	void fireKeyReleased(long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		int iCharIndex = objKeysDown.indexOf(iKeyCode);
		
		if (iCharIndex >= 0) {
			objKeysDown.remove(iCharIndex);
		}
		
		Object[] objListeners = objEventListeners.getListenerList();
		for ( int i = 0; i < objListeners.length; i += 2 ) {
			if ( objListeners[ i ] == GrabKeyListener.class ) {
				( (GrabKeyListener)objListeners[i + 1] ).keyReleased( new GrabKeyEvent(objScreen, KeyEvent.KEY_RELEASED, iWhen, iModifiers, iKeyCode, cKeyChar) );
			}
		}
	}
	
	//These are basically the constructors and deconstructors for the hook.
	private native void registerHook() throws GrabKeyException;
	private native void unregisterHook();
	
	private native void startHook();
	private native void stopHook();
}
