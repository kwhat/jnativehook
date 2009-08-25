package org.jnativehook.keyboard;

//Imports
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public interface GrabKeyListener extends KeyListener {
	public void keyPressed(KeyEvent objEvent);
	public void keyReleased(KeyEvent objEvent);
	public void keyTyped(KeyEvent objEvent);
}
