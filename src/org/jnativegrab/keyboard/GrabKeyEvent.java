package org.jnativegrab.keyboard;

//Imports
import java.awt.event.KeyEvent;
 
public class GrabKeyEvent extends KeyEvent {
	private static final long serialVersionUID = -4226037291462485089L;

	public GrabKeyEvent(int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		super(null, iId, iWhen, iModifiers, iKeyCode, cKeyChar);
	}
}
