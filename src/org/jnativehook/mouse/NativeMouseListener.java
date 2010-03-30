package org.jnativehook.mouse;

//Imports
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public interface NativeMouseListener extends MouseListener {
	public void mouseClicked(MouseEvent objEvent);
	public void mouseEntered(MouseEvent objEvent);
	public void mouseExited(MouseEvent objEvent);
	public void mousePressed(MouseEvent objEvent);
	public void mouseReleased(MouseEvent objEvent);
}
