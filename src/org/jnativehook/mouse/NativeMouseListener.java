package org.jnativehook.mouse;

//Imports
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public interface NativeMouseListener extends KeyListener {
	public void keyPressed(KeyEvent objEvent);
	public void keyReleased(KeyEvent objEvent);
	public void keyTyped(KeyEvent objEvent);
}
