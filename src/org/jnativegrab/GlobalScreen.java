package org.jnativegrab;

//Imports
import java.awt.Component;
import java.awt.event.KeyEvent;

import javax.swing.event.EventListenerList;

import org.jnativegrab.keyboard.GrabKeyEvent;
import org.jnativegrab.keyboard.GrabKeyListener;

import java.util.EventListener;

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
	
	public void addGrabKeyListener(GrabKeyListener objListener) {
		objEventListeners.add(GrabKeyListener.class, objListener);
	}
	
	public void removeGrabKeyListener(GrabKeyListener objListener) {
		objEventListeners.remove(GrabKeyListener.class, objListener);
	}
	
	public void addGrabButtonListener(GrabKeyListener objListener) {
		//objEventListeners.add(GrabKeyListener.class, objListener);
	}
	
	public void removeGrabButtonListener(GrabKeyListener objListener) {
		//objEventListeners.remove(GrabKeyListener.class, objListener);
	}
	
	@SuppressWarnings("unused")
	private void fireKeyPressed(GrabKeyEvent objEvent) {
		Object[] objListeners = objEventListeners.getListenerList();
		for (int i = 0; i < objListeners.length; i += 2) {
			if ( objListeners[ i ] == GrabKeyListener.class ) {
				((GrabKeyListener) objListeners[i + 1]).keyPressed( objEvent );
			}
		}
	}
	
	@SuppressWarnings("unused")
	private void fireKeyReleased(GrabKeyEvent objEvent) {
		Object[] objListeners = objEventListeners.getListenerList();
		for ( int i = 0; i < objListeners.length; i += 2 ) {
			if ( objListeners[ i ] == GrabKeyListener.class ) {
				( (GrabKeyListener)objListeners[i + 1] ).keyReleased( objEvent );
			}
		}
	}
}
