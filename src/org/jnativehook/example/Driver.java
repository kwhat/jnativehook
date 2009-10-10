package org.jnativehook.example;

//Imports
import java.awt.event.KeyEvent;
import java.lang.management.ManagementFactory;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;

import org.jnativehook.GlobalScreen;
import org.jnativehook.keyboard.NativeKeyException;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.keyboard.NativeKeySet;

//Should go GPL

public class Driver implements NativeKeyListener {
	private NativeKeySet objPressedKeys;
	private NativeKeySet objHeldKeys;
	
	private long iDelayRate;
	private long iRepeatRate;
	
	public static void main(String[] args) throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException, NativeKeyException {
		new Driver();
		System.out.println("Driver Started");
	}
	
	public Driver() throws SecurityException, IllegalArgumentException, NoSuchFieldException, IllegalAccessException, NativeKeyException {
		GlobalScreen.registerHook();
		iDelayRate = GlobalScreen.getInstance().getAutoRepeatDelay();
		iRepeatRate = GlobalScreen.getInstance().getAutoRepeatRate();
		
		objPressedKeys = new NativeKeySet();
		
		GlobalScreen.getInstance().addNativeKeyListener(this);
		GlobalScreen.getInstance().grabKey(0, KeyEvent.VK_B, KeyEvent.KEY_LOCATION_STANDARD);
	}
	
	
	
	public void keyPressed(KeyEvent objEvent) {
		objPressedKeys.add(objEvent);
		
		System.out.println("Key Pressed: " + objEvent.getKeyLocation() + " " + objEvent.getKeyCode() + " " + objEvent.getKeyChar());
	}
	
	public void keyReleased(KeyEvent objEvent) {
		int i = objPressedKeys.indexOf(objEvent);
		if ( i > -1) {
			KeyEvent objOldEvent = objPressedKeys.get(i);
			objPressedKeys.remove(i);
			
			if (objEvent.getWhen() - objOldEvent.getWhen() >= iDelayRate) {
				objHeldKeys.add(objEvent);
			}
			else if ( objHeldKeys.contains(objEvent) && objEvent.getWhen() - objOldEvent.getWhen() < iRepeatRate ) {
				objHeldKeys.remove(objEvent);
			}
		}
		
		if (objHeldKeys.contains(objEvent)) {
			System.out.println("Key Released: " + objEvent.getKeyLocation() + " " + objEvent.getKeyCode() + " " + objEvent.getKeyChar());
		}
	}
	
	public void keyTyped(KeyEvent objEvent) {
		System.out.println("Key Typed: " + objEvent.getKeyLocation() + " " + objEvent.getKeyCode() + " " + objEvent.getKeyChar());
	}
}
