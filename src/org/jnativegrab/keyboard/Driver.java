package org.jnativegrab.keyboard;

//Imports
import java.awt.event.KeyEvent;
import org.jnativegrab.keyboard.GrabKeyListener;
import org.jnativegrab.keyboard.GrabKeyHook;

//Should go GPL

public class Driver implements GrabKeyListener {
	GrabKeyHook objKeyHook;
	
	public static void main(String[] args) throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException, GrabKeyException {
		new Driver();
		System.out.println("Driver Started");
	}
	
	public Driver() throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException, GrabKeyException {
		objKeyHook = new GrabKeyHook();
		objKeyHook.addEventListener(this);
	}
	
	public void keyPressed(KeyEvent objEvent) {
		System.out.println("Key Pressed: " + objEvent.getKeyLocation() + " " + objEvent.getKeyCode() + " " + objEvent.getKeyChar());
	}

	public void keyReleased(KeyEvent objEvent) {
		System.out.println("Key Released: " + objEvent.getKeyLocation() + " " + objEvent.getKeyCode() + " " + objEvent.getKeyChar());
	}

	public void keyTyped(KeyEvent objEvent) {
		System.out.println("Key Typed: " + objEvent.getKeyLocation() + " " + objEvent.getKeyCode() + " " + objEvent.getKeyChar());
	}
}
