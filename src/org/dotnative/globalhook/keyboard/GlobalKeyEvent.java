package org.dotnative.globalhook.keyboard;

//KeyboardEvent.java
import java.awt.Component;
import java.awt.event.KeyEvent;
 
public class GlobalKeyEvent extends KeyEvent {
	private static final long serialVersionUID = 1L;
	/*
	boolean ts, ap, ek;
	int vk;
	
	
	 * ts = Transition State (Aka Key Down)
	 * vk = Virtual Key Code
	 * ap = Alt Pressed
	 * ek = Extended Key
	 
	public GlobalKeyEvent( Object source, boolean ts, int vk, boolean ap, boolean ek ) {
		super(source);
		this.ts = ts;
		this.vk = vk;
		this.ap = ap;
		this.ek = ek;
	}
	*/
	
	public GlobalKeyEvent(Component objSource, int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar) {
		super(objSource, iId, iWhen, iModifiers, iKeyCode, cKeyChar);
	}
	
	public GlobalKeyEvent(Component objSource, int iId, long iWhen, int iModifiers, int iKeyCode, char cKeyChar, int iKeyLocation) {
		super(objSource, iId, iWhen, iModifiers, iKeyCode, cKeyChar, iKeyLocation);
	}
}
