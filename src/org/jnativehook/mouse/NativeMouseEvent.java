package org.jnativehook.mouse;

//Imports
import java.awt.event.MouseEvent;
import org.jnativehook.GlobalScreen;
 
public class NativeMouseEvent extends MouseEvent {
	private static final long serialVersionUID = 369524271252805186L;

	public NativeMouseEvent(int iId, long iWhen, int iModifiers, int iX, int iY, int iClickCount, boolean bPopupTrigger) {
		super(GlobalScreen.getInstance(), iId, iWhen, iModifiers, iX, iY, iClickCount, bPopupTrigger);
	}
	
	public NativeMouseEvent(int iId, long iWhen, int iModifiers, int iX, int iY, int iClickCount, boolean bPopupTrigger,  int iButton) {
		super(GlobalScreen.getInstance(), iId, iWhen, iModifiers, iX, iY, iClickCount, bPopupTrigger, iButton);
	}
}
