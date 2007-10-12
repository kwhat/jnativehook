package org.dotnative.globalhook.keyboard;

//Imports
import java.awt.event.KeyEvent;
import java.util.EventListener;

public interface GlobalKeyListener extends EventListener {
	public void keyPressed(KeyEvent objEvent);
	public void keyReleased(KeyEvent objEvent);
	public void keyTyped(KeyEvent objEvent);
}
