package org.dotnative.globalhook.keyboard;

//Imports
import java.awt.event.KeyEvent;
import org.dotnative.globalhook.keyboard.GlobalKeyHook;
import org.dotnative.globalhook.keyboard.GlobalKeyListener;

//Should go GPL

public class Driver implements GlobalKeyListener {
	GlobalKeyHook objKeyHook;
	
	public static void main(String[] args) throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException, GlobalKeyException {
		new Driver();
		System.out.println("Driver Started");
	}
	
	public Driver() throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException, GlobalKeyException {
		objKeyHook = new GlobalKeyHook();
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
