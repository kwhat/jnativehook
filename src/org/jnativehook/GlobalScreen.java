package org.jnativehook;

//Imports
import java.awt.Component;
import javax.swing.event.EventListenerList;

import org.jnativehook.keyboard.GrabKeyEvent;
import org.jnativehook.keyboard.NativeKeyListener;

public class GlobalScreen extends Component {
	//Instance Variables
	private static final long serialVersionUID = -3980582715467809283L;
	private static GlobalScreen instance = null;
	private EventListenerList objEventListeners;
	
	public GlobalScreen() {
		//Setup instance variables.
		objEventListeners = new EventListenerList();
	}
	
	public static GlobalScreen getInstance() {
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
	
	public void addGrabButtonListener(NativeKeyListener objListener) {
		//objEventListeners.add(GrabKeyListener.class, objListener);
	}
	
	public void removeGrabButtonListener(NativeKeyListener objListener) {
		//objEventListeners.remove(GrabKeyListener.class, objListener);
	}
	
	@SuppressWarnings("unused")
	private void fireKeyPressed(GrabKeyEvent objEvent) {
		Object[] objListeners = objEventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == NativeKeyListener.class ) {
				((NativeKeyListener) objListeners[i + 1]).keyPressed( objEvent );
			}
		}
	}
	
	@SuppressWarnings("unused")
	private void fireKeyReleased(GrabKeyEvent objEvent) {
		Object[] objListeners = objEventListeners.getListenerList();
		for ( int i = 0; i < objListeners.length; i += 2 ) {
			if ( objListeners[ i ] == NativeKeyListener.class ) {
				( (NativeKeyListener)objListeners[i + 1] ).keyReleased( objEvent );
			}
		}
	}
}
