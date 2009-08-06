package org.jnativegrab.keyboard;

//Imports
import java.awt.Component;
import java.awt.event.KeyEvent;
 
public class GrabKeyEvent extends KeyEvent {
	private static final long serialVersionUID = -4226037291462485089L;

	public GrabKeyEvent(Component objSource, int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		super(objSource, iId, iWhen, iModifiers, iKeyCode, cKeyChar);
	}
	
	public GrabKeyEvent(Component objSource, int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar, int iKeyLocation) {
		super(objSource, iId, iWhen, iModifiers, iKeyCode, cKeyChar, iKeyLocation);
	}
}
