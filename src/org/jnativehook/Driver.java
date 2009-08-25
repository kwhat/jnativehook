package org.jnativehook;

//Imports
import java.awt.event.KeyEvent;
import org.jnativehook.keyboard.NativeKeyException;
import org.jnativehook.keyboard.GrabKeyHook;
import org.jnativehook.keyboard.NativeKeyListener;

//Should go GPL

public class Driver implements NativeKeyListener {
	GrabKeyHook objKeyHook;
	
	public static void main(String[] args) throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException, NativeKeyException {
		new Driver();
		System.out.println("Driver Started");
	}
	
	public Driver() throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException, NativeKeyException {
		GlobalScreen.getInstance().addGrabKeyListener(this);
		objKeyHook = new GrabKeyHook();
		objKeyHook.grabKey(KeyEvent.SHIFT_MASK, KeyEvent.VK_B, KeyEvent.KEY_LOCATION_STANDARD);
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
