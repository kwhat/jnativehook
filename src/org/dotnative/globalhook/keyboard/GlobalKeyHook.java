package org.dotnative.globalhook.keyboard;


//KeyboardHook.java
//This class appears to handle all of our event listening
import java.awt.event.KeyEvent;
import java.io.File;
import java.lang.reflect.Field;
import java.net.URISyntaxException;
import java.util.ArrayList;
import javax.swing.event.EventListenerList;
import org.dotnative.globalhook.GlobalScreen;

public class GlobalKeyHook {
	private EventListenerList objEventListeners;
	private ArrayList<Integer> objKeysDown;
	private GlobalScreen objScreen;
	
	public GlobalKeyHook() throws GlobalKeyException {
		//Setup default loction to the pwd.
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
	
	public GlobalKeyHook(String sLibPath) throws GlobalKeyException {
		//Dynamic Loading of Library
		//Add the applications current path to the library path so
		//we can find the binary lib
		System.setProperty("java.library.path", System.getProperty("java.library.path", "") + System.getProperty("path.separator", ":") + sLibPath);
		
		init();
	}
	
	private void init() throws GlobalKeyException {
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
			throw new GlobalKeyException(e.getMessage());
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
	
	public void addEventListener(GlobalKeyListener objListener) {
		objEventListeners.add(GlobalKeyListener.class, objListener);
	}
	
	public void removeEventListener(GlobalKeyListener objListener) {
		objEventListeners.remove(GlobalKeyListener.class, objListener);
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
			if (objListeners[i] == GlobalKeyListener.class) {
				switch (iCharIndex) {
					case -1:
						//TODO Figure out why KeyEvent.KEY_TYPED doesnt work.
						//((GlobalKeyListener) objListeners[i + 1]).keyPressed( new GlobalKeyEvent(objScreen, KeyEvent.KEY_TYPED, iWhen, iModifiers, iKeyCode, cKeyChar) );
						((GlobalKeyListener) objListeners[i + 1]).keyPressed( new GlobalKeyEvent(objScreen, KeyEvent.KEY_PRESSED, iWhen, iModifiers, iKeyCode, cKeyChar) );
					default:
						((GlobalKeyListener) objListeners[i + 1]).keyTyped( new GlobalKeyEvent(objScreen, KeyEvent.KEY_PRESSED, iWhen, iModifiers, iKeyCode, cKeyChar) );
				}
			}
		}
	}
	
	//Notify Listners Key Up
	@SuppressWarnings("unused")
	void fireKeyReleased(long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		int iCharIndex = objKeysDown.indexOf(iKeyCode);
		
		if (iCharIndex >= 0) {
			objKeysDown.remove(iCharIndex);
		}
		
		Object[] objListeners = objEventListeners.getListenerList();
		for ( int i = 0; i < objListeners.length; i += 2 ) {
			if ( objListeners[ i ] == GlobalKeyListener.class ) {
				( (GlobalKeyListener)objListeners[i + 1] ).keyReleased( new GlobalKeyEvent(objScreen, KeyEvent.KEY_RELEASED, iWhen, iModifiers, iKeyCode, cKeyChar) );
			}
		}
	}
	
	//These are basically the constructors and deconstructors for the hook.
	private native void registerHook() throws GlobalKeyException;
	private native void unregisterHook();
	
	private native void startHook();
	private native void stopHook();
}
