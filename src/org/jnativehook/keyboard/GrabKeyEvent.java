package org.jnativehook.keyboard;

//Imports
import java.awt.event.KeyEvent;

import org.jnativehook.GlobalScreen;
 
public class GrabKeyEvent extends KeyEvent {
	private static final long serialVersionUID = -4226037291462485089L;
	
	public GrabKeyEvent(int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		super(GlobalScreen.getInstance(), iId, iWhen, iModifiers, iKeyCode, cKeyChar);
	}
	
	public GrabKeyEvent(int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar, int iKeyLocation) {
		super(GlobalScreen.getInstance(), iId, iWhen, iModifiers, iKeyCode, cKeyChar, iKeyLocation);
	}
}
