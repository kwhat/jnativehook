package org.dotnative.globalhook.keyboard;

import java.awt.event.KeyEvent;
import org.dotnative.globalhook.keyboard.GlobalKeyHook;
import org.dotnative.globalhook.keyboard.GlobalKeyListener;

//Should go GPL

public class Driver implements GlobalKeyListener {
	public static void main(String[] args) throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException, GlobalKeyException {
		//GlobalKeyHook objKeyHook = new GlobalKeyHook("/home/kwhat/workspace/dotNative/dist/");
		GlobalKeyHook objKeyHook = new GlobalKeyHook();
		objKeyHook.addEventListener(new Driver());
		System.out.println("Driver Started");
	}
	
	public void keyPressed(KeyEvent objEvent) {
		System.out.println("Key Pressed: " + objEvent.getKeyCode());
	}

	public void keyReleased(KeyEvent objEvent) {
		System.out.println("Key Released: " + objEvent.getKeyCode());
	}

	public void keyTyped(KeyEvent objEvent) {
		System.out.println("Key Typed: " + objEvent.getKeyCode());
	}
}
