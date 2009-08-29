package org.jnativehook.mouse;

//Imports
import java.awt.event.KeyEvent;

import org.jnativehook.GlobalScreen;
 
public class NativeMouseEvent extends KeyEvent {
	private static final long serialVersionUID = -4226037291462485089L;
	
	public NativeMouseEvent(int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		super(GlobalScreen.getInstance(), iId, iWhen, iModifiers, iKeyCode, cKeyChar);
	}
	
	public NativeMouseEvent(int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar, int iKeyLocation) {
		super(GlobalScreen.getInstance(), iId, iWhen, iModifiers, iKeyCode, cKeyChar, iKeyLocation);
	}
}
