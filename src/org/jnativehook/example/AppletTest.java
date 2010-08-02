package org.jnativehook.example;

import javax.swing.JApplet;

import org.jnativehook.GlobalScreen;
import org.jnativehook.keyboard.NativeKeyEvent;
import org.jnativehook.keyboard.NativeKeyException;
import org.jnativehook.keyboard.NativeKeyListener;

public class AppletTest extends JApplet implements NativeKeyListener {
	private static final long serialVersionUID = 4797365154908070669L;

	public void start() {
		GlobalScreen.getInstance().addNativeKeyListener(this);
		
		try {
			System.out.println("Auto Repate Rate: " + GlobalScreen.getInstance().getAutoRepeatRate());
			System.out.println("Auto Repate Delay: " + GlobalScreen.getInstance().getAutoRepeatDelay());
		} catch (NativeKeyException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void keyPressed(NativeKeyEvent e) {
		// TODO Auto-generated method stub
		System.out.println((char) e.getKeyCode());
	}

	@Override
	public void keyReleased(NativeKeyEvent e) {
		// TODO Auto-generated method stub
		
	}
}
